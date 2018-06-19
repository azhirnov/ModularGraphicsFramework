// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


extern void Test_Math_Bit ()
{
	uint	a0 = ToMask<uint>( 0_bit );		TEST( a0 == 0 );
	uint	a1 = ToMask<uint>( 1_bit );		TEST( a1 == 0x00000001 );		TEST( ((1 << ( 1-1)) & a1) != 0 );
	uint	a2 = ToMask<uint>( 16_bit );	TEST( a2 == 0x0000FFFF );		TEST( ((1 << (16-1)) & a2) != 0 );
	uint	a3 = ToMask<uint>( 31_bit );	TEST( a3 == 0x7FFFFFFF );		TEST( ((1 << (31-1)) & a3) != 0 );
	uint	a4 = ToMask<uint>( 32_bit );	TEST( a4 == 0xFFFFFFFF );		TEST( ((1 << (32-1)) & a4) != 0 );

	TEST(( a0 == CompileTime::ToMask<uint, 0> ));
	TEST(( a1 == CompileTime::ToMask<uint, 1> ));
	TEST(( a2 == CompileTime::ToMask<uint, 16> ));
	TEST(( a3 == CompileTime::ToMask<uint, 31> ));
	TEST(( a4 == CompileTime::ToMask<uint, 32> ));
	
	uint	b0 = ToMask<uint>( 0_bit,  1_bit );		TEST( b0 == 0x00000001 );	TEST( ((1 <<  0) & b0) != 0 );
	uint	b1 = ToMask<uint>( 1_bit,  3_bit );		TEST( b1 == 0x00000006 );	TEST( ((1 <<  1) & b1) != 0 );	TEST( ((1 <<  2) & b1) != 0 );
	uint	b2 = ToMask<uint>( 16_bit, 18_bit );	TEST( b2 == 0x00030000 );	TEST( ((1 << 16) & b2) != 0 );	TEST( ((1 << 17) & b2) != 0 );
	uint	b3 = ToMask<uint>( 30_bit, 31_bit );	TEST( b3 == 0x40000000 );	TEST( ((1 << 30) & b3) != 0 );	TEST( ((1 << 30) & b3) != 0 );
	uint	b4 = ToMask<uint>( 32_bit, 34_bit );	TEST( b4 == 0 );
	
	TEST(( b0 == CompileTime::ToMask2<uint, 0, 1> ));
	TEST(( b1 == CompileTime::ToMask2<uint, 1, 3> ));
	TEST(( b2 == CompileTime::ToMask2<uint, 16, 18> ));
	TEST(( b3 == CompileTime::ToMask2<uint, 30, 31> ));
	TEST(( b4 == CompileTime::ToMask2<uint, 32, 34> ));

	uint	c0 = SafeLeftBitShift( 1u,  1_bit );			TEST( c0 == 0x00000002 );
	uint	c1 = SafeLeftBitShift( 1u, 16_bit );			TEST( c1 == 0x00010000 );
	uint	c2 = SafeLeftBitShift( 1u, 31_bit );			TEST( c2 == 0x80000000 );

	uint	c3 = SafeRightBitShift( 1u << 12,  11_bit );	TEST( c3 == 0x00000002 );
	uint	c4 = SafeRightBitShift( 1u << 12,  12_bit );	TEST( c4 == 0x00000001 );
	uint	c5 = SafeRightBitShift( 1u << 12,  13_bit );	TEST( c5 == 0 );

	uint	d0 = BitRotateLeft( 1u,			1_bit );		TEST( d0 == 0x00000002 );
	uint	d1 = BitRotateLeft( 1u << 31,	1_bit );		TEST( d1 == 0x00000001 );
	uint	d2 = BitRotateLeft( 1u,			31_bit );		TEST( d2 == 0x80000000 );
	uint	d3 = BitRotateLeft( 1u,			32_bit );		TEST( d3 == 0x00000001 );

	uint	d4 = BitRotateRight( 1u,		1_bit );		TEST( d4 == 0x80000000 );
	uint	d5 = BitRotateRight( 1u << 31,	1_bit );		TEST( d5 == 0x40000000 );
	uint	d6 = BitRotateRight( 1u,		31_bit );		TEST( d6 == 0x00000002 );
	uint	d7 = BitRotateRight( 1u,		32_bit );		TEST( d7 == 0x00000001 );

	BitsU	e0 = BitScanReverse( 0x101u );			TEST( e0 == 8 );
	BitsU	e1 = BitScanReverse( 0x202u );			TEST( e1 == 9 );
	BitsU	e2 = BitScanReverse( 0x80001000u );		TEST( e2 == 31 );
	BitsU	e3 = BitScanReverse( 0x80001002u );		TEST( e3 == 31 );
	BitsU	e4 = BitScanReverse( 0x0u );			TEST( e4 == ~0u );

	BitsU	f0 = BitScanForward( 0x101u );			TEST( f0 == 0 );
	BitsU	f1 = BitScanForward( 0x202u );			TEST( f1 == 1 );
	BitsU	f2 = BitScanForward( 0x80001000u );		TEST( f2 == 12 );
	BitsU	f3 = BitScanForward( 0x80001002u );		TEST( f3 == 1 );
	BitsU	f4 = BitScanForward( 0x0u );			TEST( f4 == ~0u );
	BitsU	f5 = BitScanForward( ushort(0x04) );	TEST( f5 == 2 );

	uint	i0 = ReverseBitOrder( 0x00000001u );	TEST( i0 == 0x80000000u );
	uint	i1 = ReverseBitOrder( 0x80000000u );	TEST( i1 == 0x00000001u );
	uint	i2 = ReverseBitOrder( 0x00001001u );	TEST( i2 == 0x80080000u );
	uint	i3 = ReverseBitOrder( 0x10100001u );	TEST( i3 == 0x80000808u );

	Bitfield<10>	g;
	g.SetAll( true );	usize	g0 = uint(g);		TEST( g0 == ToMask<uint>( 10_bit ) );
	g.SetAll( false );	usize	g1 = uint(g);		TEST( g1 == 0 );
	g.Set( 2 );			usize	g2 = g.MinIndex();	TEST( g2 == 2 );
	g.Set( 1 );			usize	g3 = g.MinIndex();	TEST( g3 == 1 );
	g.Set( 3 );			usize	g4 = g.MaxIndex();	TEST( g4 == 3 );
	g.Set( 4 );			usize	g5 = g.MaxIndex();	TEST( g5 == 4 );

	g.SetAll( false );	g.SetInterval( 0, 1 );		TEST( g.Get(0) and g.Get(1) and not g.Get(2) );
	g.SetAll( false );	g.SetInterval( 3, 5 );		TEST( g.HasInterval( 3, 5 ) and not g.HasInterval( 2, 6 ) );
	
	Bitfield<10>	h0, h1;
	h0.Set( 0 ).Set( 1 ).Set( 2 );
	h1.SetInterval( 0, 2 );
	TEST( h0 == h1 );
}
