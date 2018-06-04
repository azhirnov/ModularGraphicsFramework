// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Generators/BigInteger.h"

namespace CodeGen
{
#	define TEST		CHECK_FATAL

	struct BigIntegerTest
	{
	private:
		static void TestAdd ()
		{
			BigInteger< uint, 4 >	bi;
			ulong					j = 0;
			ulong const *			r = (ulong const *)bi._value.ptr();

			bi += MaxValue<uint>()-5;
			j += MaxValue<uint>()-5;
			TEST( *r == j );

			bi += MaxValue<uint>()-3;
			j += MaxValue<uint>()-3;
			TEST( *r == j );

			TEST( bi._value[0] == MaxValue<uint>()-9 );
			TEST( bi._value[1] == 1 );

			bi += MaxValue<uint>()-2;
			j += MaxValue<uint>()-2;
			TEST( *r == j );

			TEST( bi._value[0] == MaxValue<uint>()-12 );
			TEST( bi._value[1] == 2 );
			TEST( bi._value[2] == 0 );
		}


		static void TestRead ()
		{
			BigInteger< ulong, 4 >	bi;

			bi._value[0] = MaxValue<ulong>();
			bi._value[1] = MaxValue<uint>();
			bi._lastBit	= bi._CalcBits( 1 );

			BitsU	pos( 64 + 32 - 4 );
			BitsU	count( 8 );

			ulong	res = bi.Read( pos, count );

			TEST( res == ((1 << 4)-1) );
			
			TEST( not bi.IsZero( pos + 3 ) );
			TEST( bi.IsZero( pos + 4 ) );
			TEST( bi.IsZero( pos + count ) );
		}


		static void TestRead2 ()
		{
			BigInteger< ulong, 4 >	bi;

			bi._value[0] = MaxValue<ulong>();
			bi._value[1] = 0x123;
			bi._lastBit	= bi._CalcBits( 1 );

			BitsU	pos( 64 - 4 );
			BitsU	count( 16 );

			ulong	res = bi.Read( pos, count );

			TEST( res == 0x123f );

			TEST( not bi.IsZero( pos + 12 ) );
			TEST( bi.IsZero( pos + 13 ) );
			TEST( bi.IsZero( pos + count ) );

			pos   = BitsU(0);
			count = BitsU(4);

			res = bi.Read( pos, count );
			TEST( res == 0xf );
		}


		static void TestLastBit ()
		{
			BigInteger< ulong, 4 >	bi;
			
			TEST( bi._lastBit == 0 );

			bi += 0;
			TEST( bi._lastBit == 0 );
			
			bi += 1;
			TEST( bi._lastBit == 1 );

			bi += MaxValue<ulong>();
			TEST( bi._lastBit == 65 );
		}


		static void TestWrite ()
		{
			BigInteger< ulong, 4 >	bi;
			
			BitsU	pos( 64 - 4 );
			BitsU	count( 16 );
			ulong	value = 0xF555;

			bi.Write( value, pos, count );

			TEST( bi._lastBit == pos + count );
		}


		static void TestWrite2 ()
		{
			BigInteger< uint, 4 >	bi;

			bi.Write( 0xFFFFF0, 0_bit, 32_bit );
			bi.Write( 0x11, 8_bit, 8_bit );

			TEST( bi._value[0] == 0xFF11F0 );
		}


		static void TestAssign ()
		{
			BigInteger< ulong, 4 >	bi0;
			BigInteger< ulong, 4 >	bi1;
			BigInteger< uint,  8 >	bi2;

			bi1 += MaxValue<uint>();
			bi2 += MaxValue<uint>();
			
			bi1 += MaxValue<uint>() >> 16;
			bi2 += MaxValue<uint>() >> 16;
			
			bi1 += MaxValue<uint>() >> 6;
			bi2 += MaxValue<uint>() >> 6;
			
			bi1 += 0x12345678;
			bi2 += 0x12345678;

			bi0.Assign( bi2 );

			FOR( i, bi0._value ) {
				TEST( bi0._value[i] == bi1._value[i] );
			}
			TEST( bi0._lastBit == bi1._lastBit );
		}


		static void TestAddBigInt ()
		{
			BigInteger< uint, 4 >	src;
			BigInteger< uint, 4 >	dst;
			
			dst += MaxValue<uint>() >> 18;
			
			src += MaxValue<uint>();
			src += MaxValue<uint>() >> 16;

			dst += src;
			
			src += MaxValue<uint>() >> 18;

			TEST( src == dst );
		}


		static void TestFill ()
		{
			BigInteger< uint, 4 >	bi;

			bi.Write( 1, 60_bit, 1_bit );
			
			BigInteger< uint, 4 >	a0 = bi;
			a0.Fill( 0_bit, 8_bit );
			TEST( a0._value[0] == 0x000000FF );
			TEST( a0._value[1] == 0x10000000 );
			TEST( a0._lastBit == 61 );

			BigInteger< uint, 4 >	a1 = bi;
			a1.Fill( 8_bit, 16_bit );
			TEST( a1._value[0] == 0x00FFFF00 );
			TEST( a1._value[1] == 0x10000000 );
			TEST( a1._lastBit == 61 );

			BigInteger< uint, 4 >	a2 = bi;
			a2.Fill( 16_bit, 24_bit );
			TEST( a2._value[0] == 0xFFFF0000 );
			TEST( a2._value[1] == 0x100000FF );
			TEST( a2._lastBit == 61 );

			BigInteger< uint, 4 >	a3 = bi;
			a3.Fill( 31_bit, 33_bit );
			TEST( a3._value[0] == 0x80000000 );
			TEST( a3._value[1] == 0xFFFFFFFF );
			TEST( a3._lastBit == 64 );
		}


	public:
		static void RunTests ()
		{
			TestAdd();
			TestRead();
			TestRead2();
			TestLastBit();
			TestWrite();
			TestWrite2();
			TestAssign();
			TestAddBigInt();
			TestFill();
		}
	};


	extern void Test_BigInteger ()
	{
		BigIntegerTest::RunTests();
		
		LOG( "Test_BigInteger - OK", ELog::Info );
	}

}	// CodeGen
