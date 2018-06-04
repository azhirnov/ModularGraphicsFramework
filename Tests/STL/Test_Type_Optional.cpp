// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"
#include "Debug.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;


struct CopyCopyableElem : TDebugInstCounter<2>
{
	CopyCopyableElem () = default;
	CopyCopyableElem (const CopyCopyableElem &) = default;
	CopyCopyableElem (CopyCopyableElem &&) = delete;
	
	CopyCopyableElem& operator = (const CopyCopyableElem &) = delete;
	CopyCopyableElem& operator = (CopyCopyableElem &&) = delete;

	explicit CopyCopyableElem (const int &val) : TDebugInstCounter<2>{ val } {}
};

struct MoveCopyableElem : TDebugInstCounter<3>
{
	MoveCopyableElem () = default;
	MoveCopyableElem (const MoveCopyableElem &) = delete;
	MoveCopyableElem (MoveCopyableElem &&) = default;
	
	MoveCopyableElem& operator = (const MoveCopyableElem &) = delete;
	MoveCopyableElem& operator = (MoveCopyableElem &&) = delete;

	explicit MoveCopyableElem (const int &val) : TDebugInstCounter<3>{ val } {}
};

using Elem1_t = TDebugInstCounter<1>;
using Elem2_t = CopyCopyableElem;
using Elem3_t = MoveCopyableElem;


static void Optional_Test1 ()
{
	using Elem_t = Elem1_t;

	Elem_t::ClearStatistic();
	{
		const Elem_t		elem(10);

		Optional<Elem_t>	op1;
		Optional<Elem_t>	op2{ Elem_t(1) };
		Optional<Elem_t>	op3 = Elem_t(2);
		Optional<Elem_t>	op4;				op4 = Elem_t(3);
		Optional<Elem_t>	op5{ Elem_t(4) };	op5.Undefine();
		Optional<Elem_t>	op6;				op6.CreateDefault();
		Optional<Elem_t>	op7;				(void)(op7.GetOrCreate());
		Optional<Elem_t>	op8{ Elem_t(5) };	(void)(op8.Get());
		Optional<Elem_t>	op9{ RVREF(op2) };
		Optional<Elem_t>	op10 = op4;
		Optional<Elem_t>	op11 = elem;
	}
	TEST( Elem_t::CheckStatistic() );
}


static void Optional_Test2 ()
{
	using Elem_t = Elem2_t;

	Elem_t::ClearStatistic();
	{
		const Elem_t		elem(10);

		Optional<Elem_t>	op1;
		Optional<Elem_t>	op2{ Elem_t(1) };
		Optional<Elem_t>	op3 = Elem_t(2);
		Optional<Elem_t>	op4;				op4 = Elem_t(3);
		Optional<Elem_t>	op5{ Elem_t(4) };	op5.Undefine();
		Optional<Elem_t>	op6;				op6.CreateDefault();
		Optional<Elem_t>	op7;				(void)(op7.GetOrCreate());
		Optional<Elem_t>	op8{ Elem_t(5) };	(void)(op8.Get());
		Optional<Elem_t>	op9{ RVREF(op2) };
		Optional<Elem_t>	op10 = op4;
		Optional<Elem_t>	op11 = elem;
	}
	TEST( Elem_t::CheckStatistic() );
}


static void Optional_Test3 ()
{
	// not supported
	/*using Elem_t = Elem3_t;

	Elem_t::ClearStatistic();
	{
		const Elem_t		elem(10);

		Optional<Elem_t>	op1;
		Optional<Elem_t>	op2{ Elem_t(1) };
		Optional<Elem_t>	op3 = Elem_t(2);
		Optional<Elem_t>	op4;				op4 = Elem_t(3);
		Optional<Elem_t>	op5{ Elem_t(4) };	op5.Undefine();
		Optional<Elem_t>	op6;				op6.CreateDefault();
		Optional<Elem_t>	op7;				(void)(op7.GetOrCreate());
		Optional<Elem_t>	op8{ Elem_t(5) };	(void)(op8.Get());
		Optional<Elem_t>	op9{ RVREF(op2) };
		Optional<Elem_t>	op10 = op4;
		Optional<Elem_t>	op11 = elem;
	}
	TEST( Elem_t::CheckStatistic() );*/
}


extern void Test_Type_Optional ()
{
	Optional_Test1();
	Optional_Test2();
	Optional_Test3();
}
