// This is generated file, don't change anything!

#include "GLSLNodeFuncs.Shared.h"

namespace ShaderEditor
{
namespace GLSLShaderNodes
{

	// Floor, Ceil, Trunc, Fract, Round, RoundEven, RoundToInt, RoundEvenToInt

/*
=================================================
	Floor
=================================================
*/
	struct Floor_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Floor_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Floor", 1, in.Get<T>() );
				Replace( out.Get<T>(), "Floor.out", func );
				result = true;
			}
		}
	};

	static bool Floor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		Floor_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Floor_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "Floor" );

		src << ToLowerCase( out_args.Front() ) << " Floor (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << "\treturn floor(x);" << "\n}";

		return true;
	}

/*
=================================================
	Ceil
=================================================
*/
	struct Ceil_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Ceil_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Ceil", 1, in.Get<T>() );
				Replace( out.Get<T>(), "Ceil.out", func );
				result = true;
			}
		}
	};

	static bool Ceil_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		Ceil_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Ceil_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "Ceil" );

		src << ToLowerCase( out_args.Front() ) << " Ceil (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << "\treturn ceil(x);" << "\n}";

		return true;
	}

/*
=================================================
	Trunc
=================================================
*/
	struct Trunc_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Trunc_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Trunc", 1, in.Get<T>() );
				Replace( out.Get<T>(), "Trunc.out", func );
				result = true;
			}
		}
	};

	static bool Trunc_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		Trunc_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Trunc_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "Trunc" );

		src << ToLowerCase( out_args.Front() ) << " Trunc (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << "\treturn trunc(x);" << "\n}";

		return true;
	}

/*
=================================================
	Fract
=================================================
*/
	struct Fract_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Fract_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Fract", 1, in.Get<T>() );
				Replace( out.Get<T>(), "Fract.out", func );
				result = true;
			}
		}
	};

	static bool Fract_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		Fract_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Fract_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "Fract" );

		src << ToLowerCase( out_args.Front() ) << " Fract (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << "\treturn fract(x);" << "\n}";

		return true;
	}

/*
=================================================
	Round
=================================================
*/
	struct Round_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Round_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Round", 1, in.Get<T>() );
				Replace( out.Get<T>(), "Round.out", func );
				result = true;
			}
		}
	};

	static bool Round_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		Round_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Round_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "Round" );

		src << ToLowerCase( out_args.Front() ) << " Round (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << "\treturn round(x);" << "\n}";

		return true;
	}

/*
=================================================
	RoundEven
=================================================
*/
	struct RoundEven_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		RoundEven_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "RoundEven", 1, in.Get<T>() );
				Replace( out.Get<T>(), "RoundEven.out", func );
				result = true;
			}
		}
	};

	static bool RoundEven_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		RoundEven_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool RoundEven_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "RoundEven" );

		src << ToLowerCase( out_args.Front() ) << " RoundEven (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << "\treturn roundEven(x);" << "\n}";

		return true;
	}

	// SafeDiv, SafeMod

	// RoundToInt, RoundEvenToInt


}	// GLSLShaderNodes

namespace _ShaderNodesHidden_
{
	void RegisterGLSLShaderNodes3 (NodeFunctions *nf)
	{
		nf->Register( "GL4.Floor", GLSLShaderNodes::Floor_Node, &GLSLShaderNodes::Floor_Source );
		nf->Register( "GL4.Ceil", GLSLShaderNodes::Ceil_Node, &GLSLShaderNodes::Ceil_Source );
		nf->Register( "GL4.Trunc", GLSLShaderNodes::Trunc_Node, &GLSLShaderNodes::Trunc_Source );
		nf->Register( "GL4.Fract", GLSLShaderNodes::Fract_Node, &GLSLShaderNodes::Fract_Source );
		nf->Register( "GL4.Round", GLSLShaderNodes::Round_Node, &GLSLShaderNodes::Round_Source );
		nf->Register( "GL4.RoundEven", GLSLShaderNodes::RoundEven_Node, &GLSLShaderNodes::RoundEven_Source );
	}


}	// _ShaderNodesHidden_
}	// ShaderEditor
