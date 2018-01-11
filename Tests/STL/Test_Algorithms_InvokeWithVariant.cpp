// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;


static bool TestFunc (int i, float f, double d, OUT uint &outU, OUT float &outF)
{
	outU = i + uint(f);
	outF = f + float(d);
	return true;
}


void InvokeWithVariant_Test1 ()
{
	/*int		inI		= 2;
	float	inF		= 6.87632f;
	double	inD		= 9.2342343;
	uint	outU	= 0;
	float	outF	= 0.0f;

	Array<VariantRef>	varArr;
	varArr << VariantRef( inI ) << VariantRef( inF ) << VariantRef( inD ) << VariantRef( outU ) << VariantRef( outF );

	InvokeWithVariant( 
		&TestFunc,
		VariantRefIndexedContainerFrom< CompileTime::FunctionInfo<decltype(TestFunc)>::args >( varArr )
	);

	TEST( outU == uint(inI + uint(inF)) );
	TEST( outF == float(inF + float(inD)) );*/
}


void InvokeWithVariant_Test2 ()
{
	/*auto TestFuncDel = DelegateBuilder( &TestFunc );

	int		inI		= 2;
	float	inF		= 6.87632f;
	double	inD		= 9.2342343;
	uint	outU	= 0;
	float	outF	= 0.0f;
	
	Array<VariantCRef>	constVarArr;
	Array<VariantRef>	varArr;

	constVarArr << VariantCRef( inI ) << VariantCRef( inF ) << VariantCRef( inD );
	varArr << VariantRef( outU ) << VariantRef( outF );

	InvokeWithVariant( 
		TestFuncDel,
		VariantRefIndexedContainerFrom< CompileTime::TypeListFrom< int, float, double > >( constVarArr ),
		VariantRefIndexedContainerFrom< CompileTime::TypeListFrom< uint&, float& > >( varArr )
	);

	TEST( outU == uint(inI + uint(inF)) );
	TEST( outF == float(inF + float(inD)) );*/
}


void InvokeWithVariant_Test3 ()
{
	/*auto TestFuncDel = std::function< bool (int, float, double, uint &, float &) >( &TestFunc );

	int		inI		= 2;
	float	inF		= 6.87632f;
	double	inD		= 9.2342343;
	uint	outU	= 0;
	float	outF	= 0.0f;

	Array<VariantRef>	varArr;
	varArr << VariantRef( inI ) << VariantRef( inF ) << VariantRef( inD ) << VariantRef( outU ) << VariantRef( outF );

	InvokeWithVariant( 
		TestFuncDel,
		VariantRefIndexedContainerFrom< CompileTime::FunctionInfo<decltype(TestFuncDel)>::args >( varArr )
	);

	TEST( outU == uint(inI + uint(inF)) );
	TEST( outF == float(inF + float(inD)) );*/
}


extern void Test_Algorithms_InvokeWithVariant ()
{
	InvokeWithVariant_Test1();
	InvokeWithVariant_Test2();
	InvokeWithVariant_Test3();
}
