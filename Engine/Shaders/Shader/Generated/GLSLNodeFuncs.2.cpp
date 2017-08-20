// This is generated file, don't change anything!

#include "GLSLNodeFuncs.Shared.h"

namespace ShaderEditor
{
namespace GLSLShaderNodes
{

	// All, Any

/*
=================================================
	All
=================================================
*/
	struct All_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		All_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.all", 1, in.Get<T>() );
				Replace( out.Get<Bool>(), "All.out", func );
				result = true;
			}
		}
	};

	static bool All_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( out.Front().GetValueTypeId() == TypeIdOf<Bool>() );

		All_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool All_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

/*
=================================================
	Any
=================================================
*/
	struct Any_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Any_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.any", 1, in.Get<T>() );
				Replace( out.Get<Bool>(), "Any.out", func );
				result = true;
			}
		}
	};

	static bool Any_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( out.Front().GetValueTypeId() == TypeIdOf<Bool>() );

		Any_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Any_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	// Sin, Cos, Tan, ASin, ACos, ATan, SinH, CosH, TanH, ASinH, ACosH, ATanH

/*
=================================================
	Sin
=================================================
*/
	struct Sin_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Sin_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.sin", 1, in.Get<T>() );
				Replace( out.Get<T>(), "Sin.out", func );
				result = true;
			}
		}
	};

	static bool Sin_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		Sin_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Sin_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

/*
=================================================
	Cos
=================================================
*/
	struct Cos_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Cos_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.cos", 1, in.Get<T>() );
				Replace( out.Get<T>(), "Cos.out", func );
				result = true;
			}
		}
	};

	static bool Cos_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		Cos_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Cos_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

/*
=================================================
	Tan
=================================================
*/
	struct Tan_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Tan_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.tan", 1, in.Get<T>() );
				Replace( out.Get<T>(), "Tan.out", func );
				result = true;
			}
		}
	};

	static bool Tan_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		Tan_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Tan_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

/*
=================================================
	ASin
=================================================
*/
	struct ASin_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		ASin_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.asin", 1, in.Get<T>() );
				Replace( out.Get<T>(), "ASin.out", func );
				result = true;
			}
		}
	};

	static bool ASin_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		ASin_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool ASin_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

/*
=================================================
	ACos
=================================================
*/
	struct ACos_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		ACos_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.acos", 1, in.Get<T>() );
				Replace( out.Get<T>(), "ACos.out", func );
				result = true;
			}
		}
	};

	static bool ACos_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		ACos_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool ACos_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

/*
=================================================
	ATan
=================================================
*/
	struct ATan_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		ATan_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.atan", 1, in.Get<T>() );
				Replace( out.Get<T>(), "ATan.out", func );
				result = true;
			}
		}
	};

	static bool ATan_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		ATan_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool ATan_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

/*
=================================================
	SinH
=================================================
*/
	struct SinH_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		SinH_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.sinh", 1, in.Get<T>() );
				Replace( out.Get<T>(), "SinH.out", func );
				result = true;
			}
		}
	};

	static bool SinH_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		SinH_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool SinH_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

/*
=================================================
	CosH
=================================================
*/
	struct CosH_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		CosH_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.cosh", 1, in.Get<T>() );
				Replace( out.Get<T>(), "CosH.out", func );
				result = true;
			}
		}
	};

	static bool CosH_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		CosH_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool CosH_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

/*
=================================================
	TanH
=================================================
*/
	struct TanH_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		TanH_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.tanh", 1, in.Get<T>() );
				Replace( out.Get<T>(), "TanH.out", func );
				result = true;
			}
		}
	};

	static bool TanH_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		TanH_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool TanH_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

/*
=================================================
	ASinH
=================================================
*/
	struct ASinH_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		ASinH_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.asinh", 1, in.Get<T>() );
				Replace( out.Get<T>(), "ASinH.out", func );
				result = true;
			}
		}
	};

	static bool ASinH_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		ASinH_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool ASinH_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

/*
=================================================
	ACosH
=================================================
*/
	struct ACosH_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		ACosH_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.acosh", 1, in.Get<T>() );
				Replace( out.Get<T>(), "ACosH.out", func );
				result = true;
			}
		}
	};

	static bool ACosH_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		ACosH_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool ACosH_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

/*
=================================================
	ATanH
=================================================
*/
	struct ATanH_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		ATanH_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.atanh", 1, in.Get<T>() );
				Replace( out.Get<T>(), "ATanH.out", func );
				result = true;
			}
		}
	};

	static bool ATanH_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		ATanH_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool ATanH_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	// CoTan, ACoTan, CoTanH, ACoTanH

/*
=================================================
	CoTan
=================================================
*/
	struct CoTan_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		CoTan_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.tan", 1, in.Get<T>() );
				Replace( out.Get<T>(), "CoTan.out", func );
				result = true;
			}
		}
	};

	static bool CoTan_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		CoTan_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool CoTan_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "CoTan" );

		src << ToLowerCase( out_args.Front() ) << " CoTan (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n\treturn (1.0F / tan(x));\n}\n\n";

		return true;
	}

/*
=================================================
	ACoTan
=================================================
*/
	struct ACoTan_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		ACoTan_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.atan", 1, in.Get<T>() );
				Replace( out.Get<T>(), "ACoTan.out", func );
				result = true;
			}
		}
	};

	static bool ACoTan_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		ACoTan_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool ACoTan_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "ACoTan" );

		src << ToLowerCase( out_args.Front() ) << " ACoTan (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n\treturn (1.0F / atan(x));\n}\n\n";

		return true;
	}

/*
=================================================
	CoTanH
=================================================
*/
	struct CoTanH_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		CoTanH_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.tanh", 1, in.Get<T>() );
				Replace( out.Get<T>(), "CoTanH.out", func );
				result = true;
			}
		}
	};

	static bool CoTanH_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		CoTanH_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool CoTanH_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "CoTanH" );

		src << ToLowerCase( out_args.Front() ) << " CoTanH (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n\treturn (1.0F / tanh(x));\n}\n\n";

		return true;
	}

/*
=================================================
	ACoTanH
=================================================
*/
	struct ACoTanH_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		ACoTanH_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GLSL.atanh", 1, in.Get<T>() );
				Replace( out.Get<T>(), "ACoTanH.out", func );
				result = true;
			}
		}
	};

	static bool ACoTanH_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		ACoTanH_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool ACoTanH_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "ACoTanH" );

		src << ToLowerCase( out_args.Front() ) << " ACoTanH (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n\treturn (1.0F / atanh(x));\n}\n\n";

		return true;
	}

	// Abs, Square, SquareSign, Sign, SignOrZero

/*
=================================================
	Abs
=================================================
*/
	struct Abs_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Abs_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Abs", 1, in.Get<T>() );
				Replace( out.Get<T>(), "Abs.out", func );
				result = true;
			}
		}
	};

	static bool Abs_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		Abs_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Abs_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "Abs" );

		src << ToLowerCase( out_args.Front() ) << " Abs (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << "	return abs(x);" << "\n}";

		return true;
	}

/*
=================================================
	SignOrZero
=================================================
*/
	struct SignOrZero_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		SignOrZero_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "SignOrZero", 1, in.Get<T>() );
				Replace( out.Get<T>(), "SignOrZero.out", func );
				result = true;
			}
		}
	};

	static bool SignOrZero_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		SignOrZero_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool SignOrZero_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "SignOrZero" );

		src << ToLowerCase( out_args.Front() ) << " SignOrZero (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << "	return sign(x);" << "\n}";

		return true;
	}

/*
=================================================
	Square
=================================================
*/
	struct Square_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Square_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Square", 1, in.Get<T>() );
				Replace( out.Get<T>(), "Square.out", func );
				result = true;
			}
		}
	};

	static bool Square_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		Square_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Square_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "Square" );

		src << ToLowerCase( out_args.Front() ) << " Square (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << "	return x * x;" << "\n}";

		return true;
	}

/*
=================================================
	SquareSign
=================================================
*/
	struct SquareSign_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		SquareSign_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "SquareSign", 1, in.Get<T>() );
				Replace( out.Get<T>(), "SquareSign.out", func );
				result = true;
			}
		}
	};

	static bool SquareSign_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		SquareSign_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool SquareSign_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "SquareSign" );

		src << ToLowerCase( out_args.Front() ) << " SquareSign (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << "\treturn x * x * $Sign_in" << in_args.Front() << "_out" << out_args.Front() << "(x);" << "\n}";

		return true;
	}

/*
=================================================
	Sign
=================================================
*/
	struct Sign_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Sign_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Sign", 1, in.Get<T>() );
				Replace( out.Get<T>(), "Sign.out", func );
				result = true;
			}
		}
	};

	static bool Sign_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );

		Sign_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Sign_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front() == out_args.Front() );
		ASSERT( name == "Sign" );

		src << ToLowerCase( out_args.Front() ) << " Sign (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << "\treturn " << out_args.Front() << "($LessThan_in" << in_args.Front() << "_in" << in_args.Front()
																<< "_out" << TypeFromType( in_args.Front(), "Bool" ) << "(x, 0) * 2 - 1;" << "\n}";

		return true;
	}

	// SafeDiv, SafeMod, IsZero, IsNotZero, IsOdd, IsEven

/*
=================================================
	IsZero
=================================================
*/
	struct IsZero_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		IsZero_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and
				 in.GetValueTypeId() == TypeIdOf<T>() and
				 out.GetValueTypeId() == TypeIdOf< NVec<bool, T::COUNT> >() )
			{
				FunctionNode	func( "IsZero", 1, in.Get<T>() );
				Replace( out.Get< NVec<bool, T::COUNT> >(), "IsZero.out", func );
				result = true;
			}
		}
	};

	static bool IsZero_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );

		IsZero_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool IsZero_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		ASSERT( name == "IsZero" );

		src << ToLowerCase( out_args.Front() ) << " IsZero (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << (IsFloatTypename( in_args.Front() ) ?
																(String() << "\treturn $LessThan_in" << in_args.Front() << "_in"
																	<< in_args.Front() << "_out" << TypeFromType( in_args.Front(), "Bool" )
																	<< "( $Abs_in" << in_args.Front() << "_out" << in_args.Front() << "(x), $Epsilon_out"
																	<< in_args.Front() << "() );") :
																(String() << "\treturn $Equal_in" << in_args.Front() << "_in" << in_args.Front() << "_out"
																	<< TypeFromType(in_args.Front(), "Bool") << "( x, " << ToLowerCase( in_args.Front() ) << "(0) );")) << "\n}";

		return true;
	}

/*
=================================================
	IsNotZero
=================================================
*/
	struct IsNotZero_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		IsNotZero_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and
				 in.GetValueTypeId() == TypeIdOf<T>() and
				 out.GetValueTypeId() == TypeIdOf< NVec<bool, T::COUNT> >() )
			{
				FunctionNode	func( "IsNotZero", 1, in.Get<T>() );
				Replace( out.Get< NVec<bool, T::COUNT> >(), "IsNotZero.out", func );
				result = true;
			}
		}
	};

	static bool IsNotZero_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );

		IsNotZero_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool IsNotZero_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		ASSERT( name == "IsNotZero" );

		src << ToLowerCase( out_args.Front() ) << " IsNotZero (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << "\treturn $Not_in" << TypeFromType(in_args.Front(), "Bool") << "_out" << TypeFromType(in_args.Front(), "Bool")
															<< "( $IsZero_in" << in_args.Front() << "_out" << TypeFromType(in_args.Front(), "Bool") << "(x) );" << "\n}";

		return true;
	}

/*
=================================================
	IsOdd
=================================================
*/
	struct IsOdd_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		IsOdd_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and
				 in.GetValueTypeId() == TypeIdOf<T>() and
				 out.GetValueTypeId() == TypeIdOf< NVec<bool, T::COUNT> >() )
			{
				FunctionNode	func( "IsOdd", 1, in.Get<T>() );
				Replace( out.Get< NVec<bool, T::COUNT> >(), "IsOdd.out", func );
				result = true;
			}
		}
	};

	static bool IsOdd_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );

		IsOdd_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool IsOdd_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		ASSERT( name == "IsOdd" );

		src << ToLowerCase( out_args.Front() ) << " IsOdd (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << "\treturn $IsNotZero_in" << in_args.Front() << "_out" << TypeFromType(in_args.Front(), "Bool")
															<< "( $And_in" << in_args.Front() << "_in" << in_args.Front() << "_out" 
															<< TypeFromType(in_args.Front(), "Bool") << "( x, " << ToLowerCase( in_args.Front() ) << "(1) ) );" << "\n}";

		return true;
	}

/*
=================================================
	IsEven
=================================================
*/
	struct IsEven_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		IsEven_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and
				 in.GetValueTypeId() == TypeIdOf<T>() and
				 out.GetValueTypeId() == TypeIdOf< NVec<bool, T::COUNT> >() )
			{
				FunctionNode	func( "IsEven", 1, in.Get<T>() );
				Replace( out.Get< NVec<bool, T::COUNT> >(), "IsEven.out", func );
				result = true;
			}
		}
	};

	static bool IsEven_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );

		IsEven_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool IsEven_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		ASSERT( name == "IsEven" );

		src << ToLowerCase( out_args.Front() ) << " IsEven (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n" << "\treturn $IsZero_in" << in_args.Front() << "_out" << TypeFromType(in_args.Front(), "Bool")
															<< "( $And_in" << in_args.Front() << "_in" << in_args.Front() << "_out" 
															<< TypeFromType(in_args.Front(), "Bool") << "( x, " << ToLowerCase( in_args.Front() ) << "(1) ) );" << "\n}";

		return true;
	}


}	// GLSLShaderNodes

namespace _ShaderNodesHidden_
{
	void RegisterGLSLShaderNodes2 (NodeFunctions *nf)
	{
		nf->Register( "GL4.All", GLSLShaderNodes::All_Node, &GLSLShaderNodes::All_Source );
		nf->Register( "GL4.Any", GLSLShaderNodes::Any_Node, &GLSLShaderNodes::Any_Source );
		nf->Register( "GL4.Sin", GLSLShaderNodes::Sin_Node, &GLSLShaderNodes::Sin_Source );
		nf->Register( "GL4.Cos", GLSLShaderNodes::Cos_Node, &GLSLShaderNodes::Cos_Source );
		nf->Register( "GL4.Tan", GLSLShaderNodes::Tan_Node, &GLSLShaderNodes::Tan_Source );
		nf->Register( "GL4.ASin", GLSLShaderNodes::ASin_Node, &GLSLShaderNodes::ASin_Source );
		nf->Register( "GL4.ACos", GLSLShaderNodes::ACos_Node, &GLSLShaderNodes::ACos_Source );
		nf->Register( "GL4.ATan", GLSLShaderNodes::ATan_Node, &GLSLShaderNodes::ATan_Source );
		nf->Register( "GL4.SinH", GLSLShaderNodes::SinH_Node, &GLSLShaderNodes::SinH_Source );
		nf->Register( "GL4.CosH", GLSLShaderNodes::CosH_Node, &GLSLShaderNodes::CosH_Source );
		nf->Register( "GL4.TanH", GLSLShaderNodes::TanH_Node, &GLSLShaderNodes::TanH_Source );
		nf->Register( "GL4.ASinH", GLSLShaderNodes::ASinH_Node, &GLSLShaderNodes::ASinH_Source );
		nf->Register( "GL4.ACosH", GLSLShaderNodes::ACosH_Node, &GLSLShaderNodes::ACosH_Source );
		nf->Register( "GL4.ATanH", GLSLShaderNodes::ATanH_Node, &GLSLShaderNodes::ATanH_Source );
		nf->Register( "GL4.CoTan", GLSLShaderNodes::CoTan_Node, &GLSLShaderNodes::CoTan_Source );
		nf->Register( "GL4.ACoTan", GLSLShaderNodes::ACoTan_Node, &GLSLShaderNodes::ACoTan_Source );
		nf->Register( "GL4.CoTanH", GLSLShaderNodes::CoTanH_Node, &GLSLShaderNodes::CoTanH_Source );
		nf->Register( "GL4.ACoTanH", GLSLShaderNodes::ACoTanH_Node, &GLSLShaderNodes::ACoTanH_Source );
		nf->Register( "GL4.Abs", GLSLShaderNodes::Abs_Node, &GLSLShaderNodes::Abs_Source );
		nf->Register( "GL4.SignOrZero", GLSLShaderNodes::SignOrZero_Node, &GLSLShaderNodes::SignOrZero_Source );
		nf->Register( "GL4.Square", GLSLShaderNodes::Square_Node, &GLSLShaderNodes::Square_Source );
		nf->Register( "GL4.SquareSign", GLSLShaderNodes::SquareSign_Node, &GLSLShaderNodes::SquareSign_Source );
		nf->Register( "GL4.Sign", GLSLShaderNodes::Sign_Node, &GLSLShaderNodes::Sign_Source );
		nf->Register( "GL4.IsZero", GLSLShaderNodes::IsZero_Node, &GLSLShaderNodes::IsZero_Source );
		nf->Register( "GL4.IsNotZero", GLSLShaderNodes::IsNotZero_Node, &GLSLShaderNodes::IsNotZero_Source );
		nf->Register( "GL4.IsOdd", GLSLShaderNodes::IsOdd_Node, &GLSLShaderNodes::IsOdd_Source );
		nf->Register( "GL4.IsEven", GLSLShaderNodes::IsEven_Node, &GLSLShaderNodes::IsEven_Source );
	}


}	// _ShaderNodesHidden_
}	// ShaderEditor
