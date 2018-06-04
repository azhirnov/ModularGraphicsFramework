/*
	Big integer
*/
R"#(

#ifndef BIGINT_SIZE
#	define BIGINT_SIZE		4
#endif


struct BigInt
{
	uint	value [BIGINT_SIZE];
	uint	lastBit;
};


void BigInt_Create (out BigInt bi);
void BigInt_CreateFrom (out BigInt bi, const uint values[], const uint count);
void BigInt_Copy (out BigInt bi, const BigInt other);

void BigInt_Inc (inout BigInt bi);
void BigInt_Add (inout BigInt bi, uint value);
void BigInt_Add (inout BigInt bi, const BigInt value);
bool BigInt_IsZero (const BigInt bi, const uint pos);
uint BigInt_Read (const BigInt bi, uint pos, const uint count);
void BigInt_Write (inout BigInt bi, uint value, uint pos, const uint count);
uint BigInt_MaxSize ();

//-----------------------------------------------------------------------------


#define BIGINT_VALUE_SIZE	(32)

uint _BigInt_CalcBits (const BigInt bi, const uint i);


void BigInt_Create (out BigInt bi)
{
	for (uint i = 0; i < BIGINT_SIZE; ++i) {
		bi.value[i] = 0;
	}
	bi.lastBit = 0;
}


void BigInt_CreateFrom (out BigInt bi, const uint values[], const uint count)
{
	uint	last_nonzero = 0;

	for (uint i = 0; i < BIGINT_SIZE; ++i)
	{
		bi.value[i] = i < count ? values[i] : 0;

		if ( bi.value[i] != 0 )
			last_nonzero = i;
	}

	bi.lastBit = _BigInt_CalcBits( bi, last_nonzero );
}


void BigInt_Copy (out BigInt bi, const BigInt other)
{
	uint	last_nonzero = 0;

	for (uint i = 0; i < BIGINT_SIZE; ++i)
	{
		bi.value[i] = other.value[i];

		if ( bi.value[i] != 0 )
			last_nonzero = i;
	}

	bi.lastBit = _BigInt_CalcBits( bi, last_nonzero );
}


void BigInt_Inc (inout BigInt bi)
{
	BigInt_Add( bi, 1 );
}


void BigInt_Add (inout BigInt bi, uint right)
{
	for (uint i = 0; i < BIGINT_SIZE; ++i)
	{
		const uint	prev = bi.value[i];

		bi.value[i] += right;

		if ( bi.value[i] >= prev )
		{
			bi.lastBit = max( bi.lastBit, _BigInt_CalcBits( bi, i ) );
			break;
		}

		right = 1;
	}
}


void BigInt_Add (inout BigInt bi, const BigInt right)
{
	for (uint j = 0; j < BIGINT_SIZE; ++j)
	{
		uint	val = right.value[j];

		if ( val == 0 )
			continue;

		for (uint i = j; i < BIGINT_SIZE; ++i)
		{
			const uint	prev = bi.value[i];

			bi.value[i] += val;

			if ( bi.value[i] >= prev )
			{
				bi.lastBit = max( bi.lastBit, _BigInt_CalcBits( bi, i ) );
				break;
			}

			val = 1;
		}
	}
}


bool BigInt_IsZero (const BigInt bi, const uint pos)
{
	return pos >= bi.lastBit;
}


uint BigInt_Read (const BigInt bi, uint pos, const uint count)
{
	const uint	maxBits = BIGINT_VALUE_SIZE;
	const uint	i		= clamp( pos / maxBits, 0u, uint(BIGINT_SIZE-1) );

			pos			-= maxBits * i;
	uint	result		= (bi.value[i] >> pos) & ToMask( count );
	uint	max_count	= maxBits - pos;

	if ( count > max_count and i+1 < BIGINT_SIZE )
	{
		result |= (bi.value[i+1] & ToMask( count - max_count )) << max_count;
	}
	return result;
}


void BigInt_Write (inout BigInt bi, uint value, uint pos, const uint count)
{
	const uint	maxBits = BIGINT_VALUE_SIZE;

	uint	i			 = clamp( pos / maxBits, 0u, uint(BIGINT_SIZE-1) );
			pos			-= maxBits * i;
			value		&= ToMask( count );
	uint	max_count	 = maxBits - pos;

	bi.value[i] |= (value << pos);

	if ( count > max_count and i+1 < BIGINT_SIZE )
	{
		bi.value[++i] |= (value >> max_count);
	}

	bi.lastBit = _BigInt_CalcBits( bi, i );
}


uint BigInt_MaxSize ()
{
	return (BIGINT_VALUE_SIZE * BIGINT_SIZE);
}


uint _BigInt_CalcBits (const BigInt bi, const uint i)
{
	return BitScanReverse( bi.value[i] ) + 1 + (i * BIGINT_VALUE_SIZE);
}

)#"