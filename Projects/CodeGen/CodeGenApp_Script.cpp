// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CodeGenApp.h"
#include "Generators/TestCase.h"
#include "Generators/Scalar.h"
#include "Generators/Bruteforce/BruteforceGenerator.h"
#include "Engine/Script/Bindings/DefaultBindings.h"

namespace CodeGen
{
	using namespace GX_STL::GXScript;
	
	static Ptr<CodeGenApp>	_appInstance;


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
		float				_maxAccuracy;
		uint				_maxCommands;
		ECommandSet::bits	_commandSet;
		EConstantSet::bits	_constantSet;
		VariantCRef			_tests;

	// methods
	public:
		BruteforceCodeGen () :
			_maxAccuracy{0.0f}, _maxCommands{1}
		{}

		void SetTestF1 (const TestCaseArray<float,1> &tc) { _tests = tc.GetUntyped(); }
		void SetTestF2 (const TestCaseArray<float,2> &tc) { _tests = tc.GetUntyped(); }
		void SetTestF3 (const TestCaseArray<float,3> &tc) { _tests = tc.GetUntyped(); }
		void SetTestF4 (const TestCaseArray<float,4> &tc) { _tests = tc.GetUntyped(); }
		void SetTestF5 (const TestCaseArray<float,5> &tc) { _tests = tc.GetUntyped(); }
		void SetTestF6 (const TestCaseArray<float,6> &tc) { _tests = tc.GetUntyped(); }
		void SetTestF7 (const TestCaseArray<float,7> &tc) { _tests = tc.GetUntyped(); }
		void SetTestF8 (const TestCaseArray<float,8> &tc) { _tests = tc.GetUntyped(); }

		void SetTestD1 (const TestCaseArray<double,1> &tc) { _tests = tc.GetUntyped(); }
		void SetTestD2 (const TestCaseArray<double,2> &tc) { _tests = tc.GetUntyped(); }
		void SetTestD3 (const TestCaseArray<double,3> &tc) { _tests = tc.GetUntyped(); }
		void SetTestD4 (const TestCaseArray<double,4> &tc) { _tests = tc.GetUntyped(); }
		void SetTestD5 (const TestCaseArray<double,5> &tc) { _tests = tc.GetUntyped(); }
		void SetTestD6 (const TestCaseArray<double,6> &tc) { _tests = tc.GetUntyped(); }
		void SetTestD7 (const TestCaseArray<double,7> &tc) { _tests = tc.GetUntyped(); }
		void SetTestD8 (const TestCaseArray<double,8> &tc) { _tests = tc.GetUntyped(); }
		
		void SetTestI1 (const TestCaseArray<int,1> &tc) { _tests = tc.GetUntyped(); }
		void SetTestI2 (const TestCaseArray<int,2> &tc) { _tests = tc.GetUntyped(); }
		void SetTestI3 (const TestCaseArray<int,3> &tc) { _tests = tc.GetUntyped(); }
		void SetTestI4 (const TestCaseArray<int,4> &tc) { _tests = tc.GetUntyped(); }
		void SetTestI5 (const TestCaseArray<int,5> &tc) { _tests = tc.GetUntyped(); }
		void SetTestI6 (const TestCaseArray<int,6> &tc) { _tests = tc.GetUntyped(); }
		void SetTestI7 (const TestCaseArray<int,7> &tc) { _tests = tc.GetUntyped(); }
		void SetTestI8 (const TestCaseArray<int,8> &tc) { _tests = tc.GetUntyped(); }

		void SetTestSF1 (const TestCaseArray<ScalarF,1> &tc) { _tests = tc.GetUntyped(); }
		void SetTestSF2 (const TestCaseArray<ScalarF,2> &tc) { _tests = tc.GetUntyped(); }
		void SetTestSF3 (const TestCaseArray<ScalarF,3> &tc) { _tests = tc.GetUntyped(); }
		void SetTestSF4 (const TestCaseArray<ScalarF,4> &tc) { _tests = tc.GetUntyped(); }
		void SetTestSF5 (const TestCaseArray<ScalarF,5> &tc) { _tests = tc.GetUntyped(); }
		void SetTestSF6 (const TestCaseArray<ScalarF,6> &tc) { _tests = tc.GetUntyped(); }
		void SetTestSF7 (const TestCaseArray<ScalarF,7> &tc) { _tests = tc.GetUntyped(); }
		void SetTestSF8 (const TestCaseArray<ScalarF,8> &tc) { _tests = tc.GetUntyped(); }

		void SetMaxAccuracy (float value) {
			_maxAccuracy = value;
		}

		void SetMaxCommands (uint value) {
			_maxCommands = value;
		}

		void AddCommandType (ECommandSet::type value) {
			_commandSet |= value;
		}

		void AddCommandSet (ECommandSet::bits value) {
			_commandSet |= value;
		}

		void Run (const String &output)
		{
			BruteforceGenerator			gen;
			IGenerator::GenFunctions_t	funcs;

			CHECK_ERR( gen.Generate( _tests, _commandSet, _constantSet, _maxAccuracy, _maxCommands, OUT funcs ), void() );

			String	src;
			FOR( i, funcs )
			{
				String	str;
				gen.ToSource( funcs[i], OUT str );

				src << str << "//====================================\n\n\n";
			}

			WFilePtr	file = File::HddWFile::New( output );
			CHECK_ERR( file, void() );

			file->Write( StringCRef(src) );

			if ( _appInstance ) {
				_appInstance->Exit();
				_appInstance = null;
			}
		}
	};

}	// CodeGen
	

namespace GX_STL::GXScript
{
	using ScalarF = CodeGen::ScalarF;

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

	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<1>::TC<ScalarF>,	"TestCasesSF1" );
	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<2>::TC<ScalarF>,	"TestCasesSF2" );
	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<3>::TC<ScalarF>,	"TestCasesSF3" );
	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<4>::TC<ScalarF>,	"TestCasesSF4" );
	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<5>::TC<ScalarF>,	"TestCasesSF5" );
	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<6>::TC<ScalarF>,	"TestCasesSF6" );
	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<7>::TC<ScalarF>,	"TestCasesSF7" );
	//GX_DECL_SCRIPT_TYPE( CodeGen::_TestCaseSizedArray<8>::TC<ScalarF>,	"TestCasesSF8" );
	
	GX_DECL_SCRIPT_TYPE( CodeGen::BruteforceCodeGen,					"BruteforceCodeGen" );

	GX_DECL_SCRIPT_TYPE( CodeGen::ECommandSet::type,					"ECommandSet" );
	GX_DECL_SCRIPT_TYPE( CodeGen::ECommandSet::bits,					"ECommandSetBits" );

	GX_DECL_SCRIPT_TYPE( CodeGen::EConstantSet::type,					"EConstantSet" );
	GX_DECL_SCRIPT_TYPE( CodeGen::EConstantSet::bits,					"EConstantSetBits" );
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
		//CHECK_ERR( BindTestCase<ScalarF>( se ) );
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

		binder.AddMethod( &BFCG::SetTestF1, "SetTests" );
		binder.AddMethod( &BFCG::SetTestF2, "SetTests" );
		binder.AddMethod( &BFCG::SetTestF3, "SetTests" );
		binder.AddMethod( &BFCG::SetTestF4, "SetTests" );
		binder.AddMethod( &BFCG::SetTestF5, "SetTests" );
		binder.AddMethod( &BFCG::SetTestF6, "SetTests" );
		binder.AddMethod( &BFCG::SetTestF7, "SetTests" );
		binder.AddMethod( &BFCG::SetTestF8, "SetTests" );
		
		binder.AddMethod( &BFCG::SetTestD1, "SetTests" );
		binder.AddMethod( &BFCG::SetTestD2, "SetTests" );
		binder.AddMethod( &BFCG::SetTestD3, "SetTests" );
		binder.AddMethod( &BFCG::SetTestD4, "SetTests" );
		binder.AddMethod( &BFCG::SetTestD5, "SetTests" );
		binder.AddMethod( &BFCG::SetTestD6, "SetTests" );
		binder.AddMethod( &BFCG::SetTestD7, "SetTests" );
		binder.AddMethod( &BFCG::SetTestD8, "SetTests" );
		
		binder.AddMethod( &BFCG::SetTestI1, "SetTests" );
		binder.AddMethod( &BFCG::SetTestI2, "SetTests" );
		binder.AddMethod( &BFCG::SetTestI3, "SetTests" );
		binder.AddMethod( &BFCG::SetTestI4, "SetTests" );
		binder.AddMethod( &BFCG::SetTestI5, "SetTests" );
		binder.AddMethod( &BFCG::SetTestI6, "SetTests" );
		binder.AddMethod( &BFCG::SetTestI7, "SetTests" );
		binder.AddMethod( &BFCG::SetTestI8, "SetTests" );
		
		binder.AddMethod( &BFCG::SetMaxAccuracy, "SetMaxAccuracy" );
		binder.AddMethod( &BFCG::SetMaxCommands, "SetMaxCommands" );
		binder.AddMethod( &BFCG::AddCommandType, "AddCommandSet" );
		binder.AddMethod( &BFCG::AddCommandSet,  "AddCommandSet" );

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
			binder.AddValue( "FloatBaseCommands",	ECommandSet::FloatBaseCommands );
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
	_BindToScript
=================================================
*/
	void CodeGenApp::_BindToScript ()
	{
		_appInstance = this;
		
		DefaultBindings::BindScalarMath( &_scriptEngine );
		DefaultBindings::BindVectorMath( &_scriptEngine );
		DefaultBindings::BindString( &_scriptEngine );
		DefaultBindings::BindLog( &_scriptEngine );

		CHECK( BindTestCaseToScript( &_scriptEngine ) );
		CHECK( BindECommandSet( &_scriptEngine ) );
		CHECK( BindEConstantSet( &_scriptEngine ) );
		CHECK( BindBruteforceToScript( &_scriptEngine ) );
	}

}	// CodeGen
