// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"
#include "Debug.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


typedef TDebugInstCounter<1>			Elem_t;
typedef TDebugInstCounter<2>			VElem_t;

typedef TDebugInstCounter<3, String>	ElemStr_t;
typedef TDebugInstCounter<4, String>	VElemStr_t;

typedef UniquePtr<Elem_t>				UnqElem_t;
typedef UniquePtr<VElem_t>				UnqVElem_t;

typedef Array< Elem_t >					Array_t;
typedef std::vector< VElem_t >			Vector_t;

typedef Array< ElemStr_t >				StrArray_t;
typedef std::vector< VElemStr_t >		StrVector_t;

typedef Array< UnqElem_t >				UnqArray_t;
typedef std::vector< UnqVElem_t >		UnqVector_t;


static void Array_Test1 ()
{
	VElem_t::ClearStatistic();

	Array_t		arr;
	Vector_t	vec;

	arr.Resize( 12, false );
	vec.resize( 12 );
	TEST( (arr == ArrayRef<Elem_t>( (Elem_t *)&vec[0], vec.size() )) );
	
	arr.Resize( 36, false );
	vec.resize( 36 );
	TEST( (arr == ArrayRef<Elem_t>( (Elem_t *)&vec[0], vec.size() )) );
	
	arr.Resize( 6 );
	vec.resize( 6 );
	TEST( (arr == ArrayRef<Elem_t>( (Elem_t *)&vec[0], vec.size() )) );
	
	const uint	cnt2 = 10;
	Array_t		arr2; arr2.Resize( cnt2 );
	Vector_t	vec2( cnt2 );
	arr.Insert( arr2, 2 );
	vec.insert( vec.begin()+2, vec2.begin(), vec2.end() );
	TEST( (arr == ArrayRef<Elem_t>( (Elem_t *)&vec[0], vec.size() )) );
	
	const uint	cnt3 = 4;
	Array_t		arr3; arr3.Resize( cnt3 );
	Vector_t	vec3( cnt3 );
	vec.insert( vec.begin()+5, vec3.begin(), vec3.end() );
	arr.Insert( arr3, 5 );
	TEST( (arr == ArrayRef<Elem_t>( (Elem_t *)&vec[0], vec.size() )) );
}


static void Array_Test2 ()
{
	Array_t		arr;

	arr.Resize( 12, false );

	const Elem_t	e1;

	arr << Elem_t() << Elem_t() << Elem_t();
	arr.Erase( 6, 1 );
	arr << e1 << e1;
	arr << RVREF( Elem_t() );
	arr.PopBack();
	arr << RVREF( Elem_t() );
	arr.PopFront();
	arr << RVREF( Elem_t() );
	arr.Insert( e1, 16 );
}


static void Array_Test3 ()
{
	Array_t		arr;
	arr.Resize( 12, false );

	Elem_t() >> (Elem_t() >> arr);
	
	Array_t		arr2;
	arr2.Resize( 12, false );

	arr << RVREF( arr2 );

	arr << Array_t();
	arr << ArrayCRef<Elem_t>();
}


static void Array_Test4 ()
{
	ElemStr_t::ClearStatistic();
	VElemStr_t::ClearStatistic();

	StrArray_t	arr;
	StrVector_t	vec;
	
	for (uint i = 0; i < 100; ++i)
	{
		String str;
		Random::RandomString('a', 'z', 20, OUT str );

		arr << ElemStr_t(str);
		vec.push_back( VElemStr_t(str) );
	}

	QuickSort(	arr,
				LAMBDA()(const auto &x, const auto &y) -> bool
				{{
					return x.secondVal > y.secondVal;
				}}
	);

	std::sort(	vec.begin(), vec.end(),
				LAMBDA()(const auto &x, const auto &y) -> bool
				{{
					return x.secondVal < y.secondVal;
				}}
	);

	TEST(( arr == ArrayRef<ElemStr_t>( (ElemStr_t *)&vec[0], vec.size() ) ));
}


static void Array_Test5 ()
{
	const char	data0[] = "111111";
	const char	data1[] = "121212";
	const char	data2[] = "101010";
	const wchar	data3[] = L"111";


	TEST( BinArrayCRef::From( data0 ).BinEquals( BinArrayCRef::From( data0 ) ) );
	TEST( not BinArrayCRef::From( data0 ).BinEquals( BinArrayCRef::From( data1 ) ) );
	TEST( not BinArrayCRef::From( data0 ).BinEquals( BinArrayCRef::From( data2 ) ) );
	TEST( not BinArrayCRef::From( data0 ).BinEquals( BinArrayCRef::From( data3 ) ) );
	
	TEST( BinArrayCRef::From( data0 ).BinLess( BinArrayCRef::From( data1 ) ) );
	TEST( BinArrayCRef::From( data0 ).BinGreater( BinArrayCRef::From( data2 ) ) );
}


static void Array_Test6 ()
{
	UnqArray_t		arr;

	arr.Resize( 12, false );

	arr << UnqElem_t(new Elem_t()) << UnqElem_t(new Elem_t()) << UnqElem_t(new Elem_t());
	arr.Erase( 6, 1 );
	arr << RVREF( UnqElem_t( new Elem_t() ) );
	arr.PopBack();
	arr << UnqElem_t(new Elem_t());
	arr.PopFront();
	arr << UnqElem_t(new Elem_t());
}


extern void Test_Containers_Array ()
{
	Elem_t::ClearStatistic();

	Array_Test1();
	TEST( Elem_t::CheckStatistic() );
	TEST( VElem_t::CheckStatistic() );
	Elem_t::ClearStatistic();

	Array_Test2();
	TEST( Elem_t::CheckStatistic() );
	Elem_t::ClearStatistic();
	
	Array_Test3();
	TEST( Elem_t::CheckStatistic() );
	Elem_t::ClearStatistic();
	
	Array_Test4();
	TEST( ElemStr_t::CheckStatistic() );
	TEST( VElemStr_t::CheckStatistic() );
	ElemStr_t::ClearStatistic();

	Array_Test5();
	
	Elem_t::ClearStatistic();
	Array_Test6();
	TEST( Elem_t::CheckStatistic() );
}
