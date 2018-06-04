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
	bfcg.AddCommandSet( ECommandSet_FloatArithmetic );	// provide minus, add, mul, div, greater, less, select commands
	bfcg.SetMaxCommands( 6 );  // maximum number of commands
	bfcg.Run( "result.txt" );  // output will be writen to this file
}
```
3. Run CodeGen.exe with command line `-G test.as` and wait until generation complete. 
You can explicitly select graphics API by adding to command line: `-api "CL 1.2"`, supported versions: `CL 2.0`, `GL 4.5`, `VK 1.0`... 
Use `-dev nvidia` or `-dev "GTX 1050 Ti"` to explicitly select compute device.<br/>
If result file is empty then increase number of commands or increase max accuracy to get approximations.<br/>
<br/>
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

## Perfomance tests
test4.as with 11 commands runs 14.2min on Intel HD630, 5.6min on NVidia 1050Ti, 1.37min on NVidia 980Ti.


## Troubleshooting
1. OpenGL shader compilation crashed on Intel GPU.<br/>
Resolve: it is bug in shader compiler, try to use OpenCL version.

2. SPIR-V module validation failed.<br/>
Resolve: it is bug in PipelineCompiler or in SPIR-V compiler, try to use OpenCL version.

3. Driver crashed when I set max commands 10 or greater.<br/>
Resolve: for Windows add registry keys `TdrDelay` and `TdrDdiDelay` in `HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\GraphicsDrivers` with big values, for example 150 and 200, maybe you need to restart PC, [link to docs](https://docs.microsoft.com/en-us/windows-hardware/drivers/display/tdr-registry-keys).

4. Driver crashed or error with 'invalid command queue' or 'out of resources' on laptop.<br/>
Resolve: on laptops with integrated and descrete GPU in control panel select integrated GPU as primary and run program synthesis in descrete GPU.


## AngelScript functions
```cpp
// float, double:
Abs(x), Floor(x), Ceil(x), Trunc(x), Fract(x), Round(x), RoundTo(x), Mod(x,y), Sqrt(x),
Sin(x), Cos(x), Tan(x), SinH(x), CosH(x), TanH(x), ASin(x), ACos(x), ATan(x,y),
Pow(x,y), Ln(x), Log(x,y), Log2(x), Log10(x), Exp(x), Exp2(x), Exp10(x),
Lerp(x,y,a), Step(x,e), LinearStep(x,e0,e1), SmoothStep(x,e0,e1), BumpStep(x,e0,e1), SmoothBumpStep(x,e0,e1),
Rand_FloatRange(x,y), Rand_UNorm(), Rand_UNormD(), Rand_SNorm(), Rand_SNormD(), Rand_Float(), Rand_Double()

// int, uint, int64, uint64:
IntLog2(x), BitRotateLeft(x), BitRotateRight(x), ReverseBitOrder(x),
Rand_IntRange(x,y), Rand_Int(), Rand_UInt(), Rand_ILong(), Rand_ULong()

// float, double, int, uint, int64, uint64:
Square(x), Min(x,y), Max(x,y), MinAbs(x,y), MaxAbs(x,y), MinMag(x,y), MaxMag(x,y), Clamp(x,min,max), ClampOut(x,min,max), Wrap(x,min,max)

// logging:
LogErr, LogWarn, LogInfo, LogDbg
```
