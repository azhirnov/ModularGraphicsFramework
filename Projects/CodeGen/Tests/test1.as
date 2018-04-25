float expectedFunc (float a, float b)
{
	return a + b;
}

float rnd ()
{
	return Rand_FloatRange( -5.0f, 5.0f );
}

void main ()
{
	// gen test cases
	TestCasesF2 tc;

	for (int i = 0; i < 10; ++i)
	{
		float a = rnd();
		float b = rnd();

		tc.Add( a, b, expectedFunc( a, b ) );
	}

	BruteforceCodeGen	bfcg;
	bfcg.SetTests( tc );
	bfcg.SetMaxAccuracy( 0.001f );
	bfcg.SetMaxCommands( 3 );
	bfcg.AddCommandSet( ECommandSet_FloatBaseCommands );
	bfcg.Run( "test1_res.txt" );
}
