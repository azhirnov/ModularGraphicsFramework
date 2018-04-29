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

	uint	checked;
};


layout(binding=0, std430) readonly buffer Bruteforce_InputsSSBO
{
	BigInt		initialHash;
	float		maxAccuracy;
	float		_padding0;
	TestCase	tests[MAX_TESTS];

} ssb_input;

layout(binding=1, std430) buffer Bruteforce_OutputsSSBO
{
	Bruteforce_Atomics	stat;
	Bruteforce_Result	funcs[MAX_RESULTS];

} ssb_output;


struct CmdNode
{
	uint	id;					// function id
	int		args[MAX_CMD_ARGS];	// command input args indices
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

	for (uint i = 0; i < MAX_CMD_ARGS; ++i) {
		n.args[i] = 0;
	}
}


bool BuildCommandsList (in    BigInt    hash,
						out   float     outTicks,
						inout NodeArray commands)
{
	int			pos		= 0;
	int			arg_idx	= 0;

	NodeArray	tmp;
	tmp.count	= 0;

	outTicks	= 0.0f;

	do {
		const int	i = tmp.count;

		Node_Create( OUT tmp.nodes[i] );

		tmp.nodes[i].id = BigInt_Read( hash, pos, BITS_PER_COMMAND );

		const int	cnt = GetFunctionArgsCount( tmp.nodes[i].id );

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
	T	reg[MAX_CMD_ARGS];
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
	BigInt	initial_hash = ssb_input.initialHash;

	BigInt_Create( OUT hash );
	BigInt_Add( INOUT hash, initial_hash );
	BigInt_Add( INOUT hash, gl_GlobalInvocationID.x );


	// build commands
	NodeArray	commands;
	float		ticks	= 0.0f;

	if ( not BuildCommandsList( hash, OUT ticks, INOUT commands ) )
		return;


	// run tests
	float	accuracy = 0.0f;
	T		inputs[MAX_INPUTS];		inputs[0] = T(0);	// temp

	for (uint i = 0; i < MAX_TESTS; ++i)
	{
		// run commands
		for (uint j = 0; j < MAX_INPUTS; ++j) {
			inputs[j] = ssb_input.tests[i].inArgs[j];
		}

		RunCommands( INOUT commands, inputs );

		const uint	idx = commands.count-1;
		const T		res = commands.nodes[idx].result;

		if ( isinf( res ) or isnan( res ) )
			return;

		atomicAdd( ssb_output.stat.checked, 1 );

		accuracy += ValueDifference( ssb_input.tests[i].result, res );

		if ( accuracy > ssb_input.maxAccuracy * max(i+1, 2u) )
			return;
	}
	
	accuracy /= float(MAX_TESTS);

	if ( accuracy > ssb_input.maxAccuracy )
		return;

	
	// save result
	const uint	idx = atomicAdd( ssb_output.stat.results, 1 );

	if ( idx >= MAX_RESULTS )
	{
		atomicExchange( ssb_output.stat.results, MAX_RESULTS );
		return;
	}

	ssb_output.funcs[idx].hash		= hash;
	ssb_output.funcs[idx].ticks		= ticks;
	ssb_output.funcs[idx].accuracy	= accuracy;

}

)#"