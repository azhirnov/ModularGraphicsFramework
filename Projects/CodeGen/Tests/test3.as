float expectedFunc (float a, float b, float c, float d)
{
	return (a * b + c / d) * 2;
}

float rnd ()
{
	return Rand_FloatRange( -5.0f, 5.0f );
}

void main ()
{
	// gen test cases
	TestCasesF4 tc;

	for (int i = 0; i < 10; ++i)
	{
		float a = rnd();
		float b = rnd();
		float c = rnd();
		float d = rnd();

		tc.Add( a, b, c, d, expectedFunc( a, b, c, d ) );
	}

	BruteforceCodeGen	bfcg;
	bfcg.SetTests( tc );
	bfcg.SetMaxAccuracy( 0.001f );
	bfcg.SetMaxCommands( 9 );
	bfcg.AddCommandSet( ECommandSet_FloatBaseCommands );
	bfcg.Run( "test3_res.txt" );
}
