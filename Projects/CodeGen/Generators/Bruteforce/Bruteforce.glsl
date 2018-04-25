/*
	Bruteforce generator
*/
R"#(
#define MAX_NODES		(MAX_COMMANDS + MAX_INPUTS + /*reserve*/4)

struct TestCase
{
	T		inArgs[MAX_INPUTS];		// input
	T		result;					// output
};

struct Bruteforce_Result
{
	BigInt	hash;
	float	ticks;
	float	accuracy;
};

struct Bruteforce_Atomics
{
	uint	total;
	uint	total2;		// if 'total' overflow

	uint	results;

	//uint	checked;
};

struct Bruteforce_ConstData
{
	BigInt	initHash;
	float	maxAccuracy;
	uint	resultsCapacity;
	uint	testCasesCount;
};


layout(binding=0, std430) readonly buffer Bruteforce_SSBO
{
	Bruteforce_ConstData	constData;
	TestCase				tests[];

} ssb_input;

layout(binding=1, std430) buffer Bruteforce_Results
{
	Bruteforce_Atomics	stat;
	Bruteforce_Result	funcs[];

} ssb_output;


struct CmdNode
{
	uint	id;					// function id
	int		args[MAX_ARGS];		// command input args indices
	int		argsCount;
	T		result;
};

struct NodeArray
{
	CmdNode	nodes[MAX_NODES];
	int		count;
};


float ValueDifference (T a, T b)
{
	if ( IsZero( a - b ) )
		return 0.0f;

	float	diff	= abs( float(a - b) );
	float	div		= abs( float(a) );

	return IsZero( div ) ? 1.0e+38f : diff / div;	// 0 is the best
}


void Node_Create (out CmdNode n)
{
	n.id		= -1;
	n.argsCount	= 0;
	n.result	= 0.0f;

	for (uint i = 0; i < MAX_ARGS; ++i) {
		n.args[i] = 0;
	}
}

bool BuildCommandsList (const BigInt    hash,
						out   float     outTicks,
						inout NodeArray tmp,
						inout NodeArray commands)
{
	int		pos		= 0;
	int		arg_idx	= 0;

	outTicks	= 0.0f;
	tmp.count	= 0;

	do {
		const int	i = tmp.count;

		Node_Create( OUT tmp.nodes[i] );

		tmp.nodes[i].id = BigInt_Read( hash, pos, BITS_PER_COMMAND );

		const int	cnt = GetFunctionArgsCount( tmp.nodes[i].id );
		//ASSERT( cnt <= MAX_ARGS );

		outTicks += float( GetFunctionTicks( tmp.nodes[i].id ) );
		pos		 += BITS_PER_COMMAND;
		arg_idx	  = max( arg_idx, i );

		for (int j = cnt-1; j >= 0; --j)
		{
			tmp.nodes[i].args[j] = ++arg_idx;
		}

		tmp.nodes[i].argsCount = cnt;

		++(tmp.count);
	}
	while ( not BigInt_IsZero( hash, pos ) and tmp.count < MAX_COMMANDS );

	if ( tmp.count <= 0 )
		return false;


	// build nodes
	commands.count = MAX_INPUTS;

	const int	cnt = tmp.count - 1;

	for (int i = cnt; i >= 0; --i)
	{
		for (int j = tmp.nodes[i].argsCount-1; j >= 0; --j)
		{
			const int	idx = tmp.nodes[i].args[j];

			if ( idx <= cnt )
				tmp.nodes[i].args[j] = (cnt - idx) + MAX_INPUTS;		// from node
			else
				tmp.nodes[i].args[j] = (arg_idx - idx) % MAX_INPUTS;	// from input

			//ASSERT( tmp.nodes[i].args[j] < commands.count );
		}

		commands.nodes[ commands.count ] = tmp.nodes[i];
		++(commands.count);
	}

	return true;
}


void RunCommands (inout NodeArray commands, const T inArgs[MAX_INPUTS])
{
	T	reg[MAX_ARGS];
	reg[0] = T(0);	// temp

	// copy input args
	for (uint i = 0; i < MAX_INPUTS; ++i)
	{
		commands.nodes[i].result = inArgs[i];
	}

	const int	cmd_cnt = commands.count;

	// run
	for (int i = MAX_INPUTS; i < cmd_cnt; ++i)
	{
		const int	args_cnt = commands.nodes[i].argsCount;

		for (int j = 0; j < args_cnt; ++j)
		{
			int		idx = commands.nodes[i].args[j];
			//ASSERT( idx < i );

			reg[j] = commands.nodes[ idx ].result;
		}

		commands.nodes[i].result = CallFunction( commands.nodes[i].id, reg, args_cnt );
	}
}


void main ()
{
	if ( atomicAdd( ssb_output.stat.total, 1 ) == 0xFFFFFFFF )
	{
		atomicAdd( ssb_output.stat.total2, 1 );
	}

	BigInt	hash;
	BigInt_Create( OUT hash );
	BigInt_Add( INOUT hash, gl_GlobalInvocationID.x );


	// build commands
	NodeArray	tmp_commands;
	NodeArray	commands;
	float		ticks	= 0.0f;

	if ( not BuildCommandsList( hash, OUT ticks, INOUT tmp_commands, INOUT commands ) )
		return;


	// run tests
	float	accuracy = 0.0f;

	for (uint i = 0; i < ssb_input.constData.testCasesCount; ++i)
	{
		// run commands
		RunCommands( INOUT commands, ssb_input.tests[i].inArgs );

		const uint	idx = commands.count-1;
		const T		res = commands.nodes[idx].result;

		if ( isinf( res ) or isnan( res ) )
			return;

		//atomicAdd( ssb_output.stat.checked, 1 );

		accuracy += ValueDifference( ssb_input.tests[i].result, res );

		if ( accuracy > ssb_input.constData.maxAccuracy * max(i+1, 2u) )
			return;
	}
	
	accuracy /= float(ssb_input.constData.testCasesCount);

	if ( accuracy > ssb_input.constData.maxAccuracy )
		return;

	
	// save result
	const uint	idx = atomicAdd( ssb_output.stat.results, 1 );

	if ( idx >= ssb_input.constData.resultsCapacity )
	{
		atomicExchange( ssb_output.stat.results, ssb_input.constData.resultsCapacity );
		return;
	}

	ssb_output.funcs[idx].hash		= hash;
	ssb_output.funcs[idx].ticks		= ticks;
	ssb_output.funcs[idx].accuracy	= accuracy;

}

)#"