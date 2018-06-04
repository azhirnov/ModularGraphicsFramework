/*
	Fitness Function.

	smallest is the best.
*/
R"#(
float Fitness_FloatLinear (const float a, const float b)
{
	if ( not IsFinite( a ) or not IsFinite( b ) )
		return 1.0e+38f;

	if ( abs( a - b ) < 1.0e-4f )
		return 0.0f;

	float	diff = abs( a - b );
	float	div  = abs( a );

	return div > 1.0e-4f ? (diff / div) : 1.0e+38f;
}


float Fitness_FloatLinear (const double a, const double b)
{
	if ( not IsFinite( a ) or not IsFinite( b ) )
		return 1.0e+38f;

	if ( abs( a - b ) < 1.0e-4 )
		return 0.0f;

	double	diff = abs( a - b );
	double	div  = abs( a );

	return div > 1.0e-4 ? float(diff / div) : 1.0e+38f;
}


float Fitness_FloatLinear (const int a, const int b)
{
	if ( ((b < 0) and (a > MaxValue(int(0)) + b)) or
		 ((b > 0) and (a < MinValue(int(0))+1 + b)) )
	{
		return 1.0e+38f;	// integer overflow
	}

	if ( a == b )
		return 0.0f;

	float	diff = float(abs( a - b ));
	float	div  = float(abs( a ));

	return div > 0.0f ? (diff / div) : 1.0e+38f;
}


float Fitness_FloatLinear (uint a, uint b)
{
	if ( a < b )
	{
		// swap values
		uint	c = a;
		a = b;
		b = c;
	}

	if ( a == b )
		return 0.0f;

	float	diff = float( a - b );
	float	div  = float( a );

	return div > 0.0f ? (diff / div) : 1.0e+38f;
}


float ValueDifference (const T a, const T b) {
	return Fitness_FloatLinear( a, b );
}
)#"