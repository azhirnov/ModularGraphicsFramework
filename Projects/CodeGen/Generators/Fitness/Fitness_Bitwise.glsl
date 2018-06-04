/*
	Fitness Function.

	smallest is the best.
*/
R"#(
float Fitness_Bitwise (int a, int b)
{
	return float(bitCount( a ^ b ));
}


float Fitness_Bitwise (uint a, uint b)
{
	return float(bitCount( a ^ b ));
}


float Fitness_Bitwise (float a, float b)
{
	if ( not IsFinite( a ) or not IsFinite( b ) )
		return 1.0e+38f;

	if ( abs( a - b ) < 1.0e-4f )
		return 0.0f;

	return Fitness_Bitwise( floatBitsToInt(a), floatBitsToInt(b) );
}


#ifdef ARB_gpu_shader_int64_enabled
float Fitness_Bitwise (ilong a, ilong b)
{
	return float(bitCount( a ^ b ));
}


float Fitness_Bitwise (ulong a, ulong b)
{
	return float(bitCount( a ^ b ));
}


float Fitness_Bitwise (double a, double b)
{
	if ( not IsFinite( a ) or not IsFinite( b ) )
		return 1.0e+38f;

	if ( abs( a - b ) < 1.0e-4 )
		return 0.0f;

	return Fitness_Bitwise( doubleBitsToInt64(a), doubleBitsToInt64(b) );
}
#endif


float ValueDifference (T a, T b) {
	return Fitness_Bitwise( a, b );
}
)#"