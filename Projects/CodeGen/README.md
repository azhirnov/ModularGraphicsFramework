# GPU accelerated program synthesis

## How to use
1. Create script file, for example `test.as`
2. Write script
```cpp
float expectedFunc (float a, float b, float c, float d)
{
	return (c * -b) + (d * a);
}

float rnd ()
{
	return Rand_FloatRange( -5.0f, 5.0f );
}

void main ()
{
	// gen test cases
	TestCasesF4 tc;		// test cases container with type float and 4 input arguments

	for (int i = 0; i < 10; ++i)
	{
		float a = rnd();
		float b = rnd();
		float c = rnd();
		float d = rnd();

		tc.Add( a, b, c, d, expectedFunc( a, b, c, d ) );	// add 4 input arguments and expected result
	}

	BruteforceCodeGen	bfcg;
	bfcg.SetTests( tc );
	bfcg.SetMaxAccuracy( 0.001f );	// results that has accuracy greater than maximum will be ignored
	bfcg.AddCommandSet( ECommandSet_FloatBaseCommands );	// provide minus, add, mul, div, greater, less, select commands
	bfcg.SetMaxCommands( 6 );  // maximum number of commands
	bfcg.Run( "result.txt" );  // output will be writen to this file
}
```
3. Run CodeGen.exe with command line `-G test.as` and wait until generation complete.
You can explicitly select graphics API by adding to command line: `-api CL 1.2`, supported versions: `CL 2.0`, `GL 4.5`, `VK 1.0`, ... 
If result file is empty then increase number of commands or increase max accuracy to get approximations.
The result would be like this:
```cpp
float main (float arg0, float arg1, float arg2, float arg3)
{
	// hash:     { 000016C2, 00000000, 00000000, 00000000 }
	// accuracy: 0.0
	// ticks:    13
	// commands: 5
	return ((arg3 * arg0) + -(arg1 * arg2))
}
```


## Troubleshooting
1. OpenGL shader compilation crashed on Intel GPU.
Resolve: it is bug in shader compiler, try to use OpenCL version.

2. SPIR-V module validation failed.
Resolve: it is bug in PipelineCompiler or in SPIR-V compiler, try to use OpenCL version.

3. OpenCL and OpenGL driver crashed when I set max commands 10 or greater.
Resolve: for Windows add registry keys TdrDelay and TdrDdiDelay with big values, for example 150 and 200, [link to docs](https://docs.microsoft.com/en-us/windows-hardware/drivers/display/tdr-registry-keys).
