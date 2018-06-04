// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CodeGenApp.h"
#include "Generators/TestCase.h"
#include "Generators/Mixed.h"
#include "Generators/GenFunctionSerializer.h"
#include "Generators/Bruteforce/BruteforceGenerator.h"
#include "Engine/Script/Bindings/DefaultBindings.h"

namespace CodeGen
{
	using namespace GX_STL::GXScript;
	

	//
	// Test Case Array
	//
	template <typename T, usize Count>
	struct TestCaseArray final
	{
	// variables
	private:
		Array< TestCase<T> >	_tests;

	// methods
	public:
		VariantCRef GetUntyped () const
		{
			return _tests;
		}

		void Add1 (T a0, T res)
		{
			TestCase<T> tc;
			tc.input << a0;
			tc.output = res;
			_tests.PushBack( tc );
		}

		void Add2 (T a0, T a1, T res)
		{
			TestCase<T> tc;
			tc.input << a0 << a1;
			tc.output = res;
			_tests.PushBack( tc );
		}

		void Add3 (T a0, T a1, T a2, T res)
		{
			TestCase<T> tc;
			tc.input << a0 << a1 << a2;
			tc.output = res;
			_tests.PushBack( tc );
		}

		void Add4 (T a0, T a1, T a2, T a3, T res)
		{
			TestCase<T> tc;
			tc.input << a0 << a1 << a2 << a3;
			tc.output = res;
			_tests.PushBack( tc );
		}

		void Add5 (T a0, T a1, T a2, T a3, T a4, T res)
		{
			TestCase<T> tc;
			tc.input << a0 << a1 << a2 << a3 << a4;
			tc.output = res;
			_tests.PushBack( tc );
		}

		void Add6 (T a0, T a1, T a2, T a3, T a4, T a5, T res)
		{
			TestCase<T> tc;
			tc.input << a0 << a1 << a2 << a3 << a4 << a5;
			tc.output = res;
			_tests.PushBack( tc );
		}

		void Add7 (T a0, T a1, T a2, T a3, T a4, T a5, T a6, T res)
		{
			TestCase<T> tc;
			tc.input << a0 << a1 << a2 << a3 << a4 << a5 << a6;
			tc.output = res;
			_tests.PushBack( tc );
		}

		void Add8 (T a0, T a1, T a2, T a3, T a4, T a5, T a6, T a7, T res)
		{
			TestCase<T> tc;
			tc.input << a0 << a1 << a2 << a3 << a4 << a5 << a6 << a7;
			tc.output = res;
			_tests.PushBack( tc );
		}
	};


	template <usize Count>
	struct _TestCaseSizedArray
	{
		template <typename T> using TC = TestCaseArray<T,Count>;
	};


	//
	// Bruteforce CodeGen
	//
	struct BruteforceCodeGen
	{
	// variables
	private:
		IGenerator::Config		_config;
		VariantCRef				_tests;

	// methods
	public:
		BruteforceCodeGen ()
		{
			_config.maxCommands	= 10;
			_config.maxAccuracy	= 0.001f;
			_config.fitnessFunc	= EFitnessFunction::FloatLinear;
		}
		
		template <typename T, usize Count>
		void SetTests (const TestCaseArray<T,Count> &tc) {
			_tests = tc.GetUntyped();
		}

		void SetMaxAccuracy (float value) {
			_config.maxAccuracy = value;
		}

		void SetMaxCommands (uint value) {
			_config.maxCommands = value;
		}

		void SetMaxConstants (uint value) {
			_config.maxConstants = value;
		}

		void AddCommandType (ECommandSet::type value) {
			_config.commandSetType |= value;
		}

		void AddCommandSet (ECommandSet::bits value) {
			_config.commandSetType |= value;
		}

		void AddConstantType (EConstantSet::type value) {
			_config.constantType |= value;
		}

		void AddConstantSet (EConstantSet::bits value) {
			_config.constantType |= value;
		}

		void SetCommands (ECommandSet::bits value) {
			_config.commandSetType = value;
		}

		void SetFitnessFunction (EFitnessFunction::type value) {
			_config.fitnessFunc = value;
		}

		void Run (const String &output)
		{
			UniquePtr<IGenerator>	gen{ new BruteforceGenerator() };

			CHECK_ERR( gen->Initialize( _tests, _config ), void() );

			CodeGenApp::Instance()->SetGenerator(
				RVREF(gen),
				LAMBDA( str = output ) (IGenerator *gen) {
					OnGenerationCompleted( gen, str );
				}
			);
		}

		static void OnGenerationCompleted (IGenerator *gen, StringCRef output)
		{
			IGenerator::GenFunctions_t	funcs;
			IGenerator::Statistic		stat;
			String						src;

			CHECK( gen->GetResults( OUT stat, OUT funcs ) );

			GXFile::WFilePtr	file = GXFile::HddWFile::New( output );
			CHECK_ERR( file, void() );

			LOG( "Program synthesing time "_str << ToString( stat.duration ), ELog::Debug );

			if ( not funcs.Empty() )
			{
				FOR( i, funcs )
				{
					String	str;
					GenFunctionSerializer::ToSource( funcs[i], OUT str );

					src << str << "//====================================\n\n\n";
				}

				file->Write( StringCRef(src) );
			}
			else
			{
				src = "// 0 results found, increase max commands count or max accuracy value\n";
				file->Write( StringCRef(src) );
			}
		}
	};

}	// CodeGen
	

namespace GX_STL::GXScript
{
	using MixedF = CodeGen::MixedF;

	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<1>::TC<float>,	"TestCasesF1" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<2>::TC<float>,	"TestCasesF2" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<3>::TC<float>,	"TestCasesF3" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<4>::TC<float>,	"TestCasesF4" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<5>::TC<float>,	"TestCasesF5" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<6>::TC<float>,	"TestCasesF6" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<7>::TC<float>,	"TestCasesF7" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<8>::TC<float>,	"TestCasesF8" );
	
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<1>::TC<double>,	"TestCasesD1" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<2>::TC<double>,	"TestCasesD2" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<3>::TC<double>,	"TestCasesD3" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<4>::TC<double>,	"TestCasesD4" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<5>::TC<double>,	"TestCasesD5" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<6>::TC<double>,	"TestCasesD6" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<7>::TC<double>,	"TestCasesD7" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<8>::TC<double>,	"TestCasesD8" );
	
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<1>::TC<int>,		"TestCasesI1" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<2>::TC<int>,		"TestCasesI2" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<3>::TC<int>,		"TestCasesI3" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<4>::TC<int>,		"TestCasesI4" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<5>::TC<int>,		"TestCasesI5" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<6>::TC<int>,		"TestCasesI6" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<7>::TC<int>,		"TestCasesI7" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<8>::TC<int>,		"TestCasesI8" );
	
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<1>::TC<uint>,		"TestCasesU1" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<2>::TC<uint>,		"TestCasesU2" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<3>::TC<uint>,		"TestCasesU3" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<4>::TC<uint>,		"TestCasesU4" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<5>::TC<uint>,		"TestCasesU5" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<6>::TC<uint>,		"TestCasesU6" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<7>::TC<uint>,		"TestCasesU7" );
	GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<8>::TC<uint>,		"TestCasesU8" );

	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<1>::TC<MixedF>,	"TestCasesMF1" );
	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<2>::TC<MixedF>,	"TestCasesMF2" );
	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<3>::TC<MixedF>,	"TestCasesMF3" );
	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<4>::TC<MixedF>,	"TestCasesMF4" );
	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<5>::TC<MixedF>,	"TestCasesMF5" );
	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<6>::TC<MixedF>,	"TestCasesMF6" );
	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<7>::TC<MixedF>,	"TestCasesMF7" );
	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<8>::TC<MixedF>,	"TestCasesMF8" );
	
	GX_DECL_SCRIPT_TYPE( CodeGen::BruteforceCodeGen,					"BruteforceCodeGen" );

	GX_DECL_SCRIPT_TYPE( CodeGen::ECommandSet::type,					"ECommandSet" );
	GX_DECL_SCRIPT_TYPE( CodeGen::ECommandSet::bits,					"ECommandSetBits" );

	GX_DECL_SCRIPT_TYPE( CodeGen::EConstantSet::type,					"EConstantSet" );
	GX_DECL_SCRIPT_TYPE( CodeGen::EConstantSet::bits,					"EConstantSetBits" );
	
	GX_DECL_SCRIPT_TYPE( CodeGen::EFitnessFunction::type,				"EFitnessFunction" );
}

namespace CodeGen
{
/*
=================================================
	BindTestCase
=================================================
*/
	template <typename T>
	static bool BindTestCase (const ScriptEnginePtr &se)
	{
		// 1
		{
			using TC = TestCaseArray<T,1>;
			ClassBinder<TC> binder( se );
			binder.CreateClassValue();
			binder.AddMethod( &TC::Add1, "Add" );
		}
		// 2
		{
			using TC = TestCaseArray<T,2>;
			ClassBinder<TC> binder( se );
			binder.CreateClassValue();
			binder.AddMethod( &TC::Add2, "Add" );
		}
		// 3
		{
			using TC = TestCaseArray<T,3>;
			ClassBinder<TC> binder( se );
			binder.CreateClassValue();
			binder.AddMethod( &TC::Add3, "Add" );
		}
		// 4
		{
			using TC = TestCaseArray<T,4>;
			ClassBinder<TC> binder( se );
			binder.CreateClassValue();
			binder.AddMethod( &TC::Add4, "Add" );
		}
		// 5
		{
			using TC = TestCaseArray<T,5>;
			ClassBinder<TC> binder( se );
			binder.CreateClassValue();
			binder.AddMethod( &TC::Add5, "Add" );
		}
		// 6
		{
			using TC = TestCaseArray<T,6>;
			ClassBinder<TC> binder( se );
			binder.CreateClassValue();
			binder.AddMethod( &TC::Add6, "Add" );
		}
		// 7
		{
			using TC = TestCaseArray<T,7>;
			ClassBinder<TC> binder( se );
			binder.CreateClassValue();
			binder.AddMethod( &TC::Add7, "Add" );
		}
		// 8
		{
			using TC = TestCaseArray<T,8>;
			ClassBinder<TC> binder( se );
			binder.CreateClassValue();
			binder.AddMethod( &TC::Add8, "Add" );
		}
		return true;
	}

/*
=================================================
	BindTestCaseToScript
=================================================
*/
	static bool BindTestCaseToScript (const ScriptEnginePtr &se)
	{
		CHECK_ERR( BindTestCase<float>(   se ) );
		CHECK_ERR( BindTestCase<double>(  se ) );
		CHECK_ERR( BindTestCase<int>(     se ) );
		CHECK_ERR( BindTestCase<uint>(    se ) );
		//CHECK_ERR( BindTestCase<MixedF>( se ) );
		return true;
	}
	
/*
=================================================
	BindBruteforceToScript
=================================================
*/
	static bool BindBruteforceToScript (const ScriptEnginePtr &se)
	{
		using BFCG	= BruteforceCodeGen;

		ClassBinder<BFCG>	binder( se );
		binder.CreateClassValue();

		binder.AddMethod( &BFCG::SetTests<float,1>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<float,2>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<float,3>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<float,4>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<float,5>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<float,6>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<float,7>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<float,8>, "SetTests" );
		
		binder.AddMethod( &BFCG::SetTests<double,1>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<double,2>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<double,3>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<double,4>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<double,5>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<double,6>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<double,7>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<double,8>, "SetTests" );
		
		binder.AddMethod( &BFCG::SetTests<int,1>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<int,2>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<int,3>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<int,4>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<int,5>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<int,6>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<int,7>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<int,8>, "SetTests" );
		
		binder.AddMethod( &BFCG::SetTests<uint,1>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<uint,2>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<uint,3>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<uint,4>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<uint,5>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<uint,6>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<uint,7>, "SetTests" );
		binder.AddMethod( &BFCG::SetTests<uint,8>, "SetTests" );
		
		binder.AddMethod( &BFCG::SetMaxAccuracy,		"SetMaxAccuracy" );
		binder.AddMethod( &BFCG::SetMaxCommands,		"SetMaxCommands" );
		binder.AddMethod( &BFCG::SetMaxConstants,		"SetMaxConstants" );
		binder.AddMethod( &BFCG::AddCommandType,		"AddCommandSet" );
		binder.AddMethod( &BFCG::AddCommandSet,			"AddCommandSet" );
		binder.AddMethod( &BFCG::AddConstantType,		"AddConstantSet" );
		binder.AddMethod( &BFCG::AddConstantSet,		"AddConstantSet" );
		binder.AddMethod( &BFCG::SetFitnessFunction,	"SetFitnessFunction" );

		binder.AddMethod( &BFCG::Run, "Run" );

		return true;
	}
	
/*
=================================================
	BindECommandSet
=================================================
*/
	static bool BindECommandSet (const ScriptEnginePtr &se)
	{
		// enums
		{
			EnumBinder< ECommandSet::type >	binder{ se };
		
			binder.Create();
			binder.AddValue( "FloatArithmetic",		ECommandSet::FloatArithmetic );
			binder.AddValue( "IntArithmetic",		ECommandSet::IntArithmetic );
			binder.AddValue( "IntBitwise",			ECommandSet::IntBitwise );
		}

		// bits
		{
			EnumBitfieldBinder< ECommandSet::bits >	binder{ se };

			binder.BindDefaults();
		}
		return true;
	}
	
/*
=================================================
	BindEConstantSet
=================================================
*/
	static bool BindEConstantSet (const ScriptEnginePtr &se)
	{
		// enums
		{
			EnumBinder< EConstantSet::type >	binder{ se };
		
			binder.Create();
			binder.AddValue( "Math",					EConstantSet::Math );
			binder.AddValue( "Physics",					EConstantSet::Physics );
			binder.AddValue( "MathMostPopularOnly",		EConstantSet::MathMostPopularOnly );
			binder.AddValue( "PhysicsMostPopularOnly",	EConstantSet::PhysicsMostPopularOnly );
			binder.AddValue( "PrimeNumbers",			EConstantSet::PrimeNumbers );
			binder.AddValue( "Values0to255",			EConstantSet::Values0to255 );
		}

		// bits
		{
			EnumBitfieldBinder< EConstantSet::bits >	binder{ se };

			binder.BindDefaults();
		}
		return true;
	}
	
/*
=================================================
	BindEFitnessFunction
=================================================
*/
	static bool BindEFitnessFunction (const ScriptEnginePtr &se)
	{
		EnumBinder< EFitnessFunction::type >	binder{ se };
		
		binder.Create();
		binder.AddValue( "FloatLinear",		EFitnessFunction::FloatLinear );
		binder.AddValue( "Bitwise",			EFitnessFunction::Bitwise );
		return true;
	}

/*
=================================================
	_BindToScript
=================================================
*/
	void CodeGenApp::_BindToScript ()
	{
		DefaultBindings::BindScalarMath( &_scriptEngine );
		DefaultBindings::BindVectorMath( &_scriptEngine );
		DefaultBindings::BindString( &_scriptEngine );
		DefaultBindings::BindLog( &_scriptEngine );

		CHECK( BindTestCaseToScript( &_scriptEngine ) );
		CHECK( BindECommandSet( &_scriptEngine ) );
		CHECK( BindEConstantSet( &_scriptEngine ) );
		CHECK( BindEFitnessFunction( &_scriptEngine ) );
		CHECK( BindBruteforceToScript( &_scriptEngine ) );
	}

}	// CodeGen
