// This is generated file, don't change anything!

#include "GLSLNodeFuncs.Shared.h"

namespace ShaderEditor
{
namespace GLSLShaderNodes
{
	// Math operators

/*
=================================================
	Not
=================================================
*/
	struct Not_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Not_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Not", 1, in.Get<T>() );
				Replace( out.Get<T>(), "Not.out", func );
				result = true;
			}
		}
	};

	static bool Not_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		Not_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Not_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front()	== out_args.Front() );
		ASSERT( name == "Not" );

		const uint n = IsNumber( in_args.Front().Back() ) ? (in_args.Front().Back() - '0') : 1;

		src << ToLowerCase( out_args.Front() ) << " Not (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n\treturn !" << " (x);\n}";

		return true;
	}

/*
=================================================
	BitInverse
=================================================
*/
	struct BitInverse_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		BitInverse_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "BitInverse", 1, in.Get<T>() );
				Replace( out.Get<T>(), "BitInverse.out", func );
				result = true;
			}
		}
	};

	static bool BitInverse_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		BitInverse_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool BitInverse_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front()	== out_args.Front() );
		ASSERT( name == "BitInverse" );

		const uint n = IsNumber( in_args.Front().Back() ) ? (in_args.Front().Back() - '0') : 1;

		src << ToLowerCase( out_args.Front() ) << " BitInverse (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n\treturn " << " (x);\n}";

		return true;
	}

/*
=================================================
	Plus
=================================================
*/
	struct Plus_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Plus_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Plus", 1, in.Get<T>() );
				Replace( out.Get<T>(), "Plus.out", func );
				result = true;
			}
		}
	};

	static bool Plus_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		Plus_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Plus_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front()	== out_args.Front() );
		ASSERT( name == "Plus" );

		const uint n = IsNumber( in_args.Front().Back() ) ? (in_args.Front().Back() - '0') : 1;

		src << ToLowerCase( out_args.Front() ) << " Plus (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n\treturn " << " (x);\n}";

		return true;
	}

/*
=================================================
	Minus
=================================================
*/
	struct Minus_Func
	{
		VariantCRef		in;
		VariantRef		out;
		bool			result { false };

		Minus_Func (VariantCRef in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Minus", 1, in.Get<T>() );
				Replace( out.Get<T>(), "Minus.out", func );
				result = true;
			}
		}
	};

	static bool Minus_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		Minus_Func	func( in.Front(), out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Minus_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 1 and out_args.Count() == 1 );
		CHECK_ERR( in_args.Front()	== out_args.Front() );
		ASSERT( name == "Minus" );

		const uint n = IsNumber( in_args.Front().Back() ) ? (in_args.Front().Back() - '0') : 1;

		src << ToLowerCase( out_args.Front() ) << " Minus (const "
			<< ToLowerCase( in_args.Front() ) << " x) {\n\treturn " << " (x);\n}";

		return true;
	}

/*
=================================================
	Add
=================================================
*/
	struct Add_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		Add_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Add", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "Add.out", func );
				result = true;
			}
		}
	};

	static bool Add_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		Add_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Add_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == out_args.Front() or in_args[1] == out_args.Front() );
		ASSERT( name == "Add" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " Add (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << shared << "(x) + " << shared << "(y)" << ";\n}";

		return true;
	}

/*
=================================================
	Sub
=================================================
*/
	struct Sub_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		Sub_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Sub", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "Sub.out", func );
				result = true;
			}
		}
	};

	static bool Sub_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		Sub_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Sub_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == out_args.Front() or in_args[1] == out_args.Front() );
		ASSERT( name == "Sub" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " Sub (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << shared << "(x) - " << shared << "(y)" << ";\n}";

		return true;
	}

/*
=================================================
	Mul
=================================================
*/
	struct Mul_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		Mul_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Mul", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "Mul.out", func );
				result = true;
			}
		}
	};

	static bool Mul_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		Mul_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Mul_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == out_args.Front() or in_args[1] == out_args.Front() );
		ASSERT( name == "Mul" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " Mul (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << shared << "(x) * " << shared << "(y)" << ";\n}";

		return true;
	}

/*
=================================================
	Div
=================================================
*/
	struct Div_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		Div_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Div", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "Div.out", func );
				result = true;
			}
		}
	};

	static bool Div_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		Div_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Div_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == out_args.Front() or in_args[1] == out_args.Front() );
		ASSERT( name == "Div" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " Div (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << shared << "(x) / " << shared << "(y)" << ";\n}";

		return true;
	}

/*
=================================================
	Mod
=================================================
*/
	struct Mod_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		Mod_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Mod", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "Mod.out", func );
				result = true;
			}
		}
	};

	static bool Mod_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		Mod_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Mod_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == out_args.Front() or in_args[1] == out_args.Front() );
		ASSERT( name == "Mod" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " Mod (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << shared << "(x) % " << shared << "(y)" << ";\n}";

		return true;
	}

/*
=================================================
	And
=================================================
*/
	struct And_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		And_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "And", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "And.out", func );
				result = true;
			}
		}
	};

	static bool And_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		And_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool And_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == out_args.Front() or in_args[1] == out_args.Front() );
		ASSERT( name == "And" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " And (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << shared << "(x) & " << shared << "(y)" << ";\n}";

		return true;
	}

/*
=================================================
	Or
=================================================
*/
	struct Or_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		Or_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Or", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "Or.out", func );
				result = true;
			}
		}
	};

	static bool Or_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		Or_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Or_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == out_args.Front() or in_args[1] == out_args.Front() );
		ASSERT( name == "Or" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " Or (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << shared << "(x) | " << shared << "(y)" << ";\n}";

		return true;
	}

/*
=================================================
	Xor
=================================================
*/
	struct Xor_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		Xor_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Xor", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "Xor.out", func );
				result = true;
			}
		}
	};

	static bool Xor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		Xor_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Xor_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == out_args.Front() or in_args[1] == out_args.Front() );
		ASSERT( name == "Xor" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " Xor (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << shared << "(x) ^ " << shared << "(y)" << ";\n}";

		return true;
	}

/*
=================================================
	BitLShift
=================================================
*/
	struct BitLShift_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		BitLShift_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "BitLShift", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "BitLShift.out", func );
				result = true;
			}
		}
	};

	static bool BitLShift_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		BitLShift_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool BitLShift_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == out_args.Front() or in_args[1] == out_args.Front() );
		ASSERT( name == "BitLShift" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " BitLShift (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << shared << "(x) << " << shared << "(y)" << ";\n}";

		return true;
	}

/*
=================================================
	BitRShift
=================================================
*/
	struct BitRShift_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		BitRShift_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "BitRShift", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "BitRShift.out", func );
				result = true;
			}
		}
	};

	static bool BitRShift_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		BitRShift_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool BitRShift_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == out_args.Front() or in_args[1] == out_args.Front() );
		ASSERT( name == "BitRShift" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " BitRShift (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << shared << "(x) >> " << shared << "(y)" << ";\n}";

		return true;
	}

/*
=================================================
	BoolAnd
=================================================
*/
	struct BoolAnd_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		BoolAnd_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "BoolAnd", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "BoolAnd.out", func );
				result = true;
			}
		}
	};

	static bool BoolAnd_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		BoolAnd_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool BoolAnd_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == out_args.Front() or in_args[1] == out_args.Front() );
		ASSERT( name == "BoolAnd" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " BoolAnd (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << shared << "(x) && " << shared << "(y)" << ";\n}";

		return true;
	}

/*
=================================================
	BoolOr
=================================================
*/
	struct BoolOr_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		BoolOr_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "BoolOr", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "BoolOr.out", func );
				result = true;
			}
		}
	};

	static bool BoolOr_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		BoolOr_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool BoolOr_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == out_args.Front() or in_args[1] == out_args.Front() );
		ASSERT( name == "BoolOr" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " BoolOr (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << shared << "(x) || " << shared << "(y)" << ";\n}";

		return true;
	}

/*
=================================================
	Equal
=================================================
*/
	struct Equal_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		Equal_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "Equal", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "Equal.out", func );
				result = true;
			}
		}
	};

	static bool Equal_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		Equal_Func	func( in, out.Front() );
		Helpers::IntegerOrFloatTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool Equal_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == in_args[1] and TypeFromType(in_args[0], "Bool") == out_args.Front() );
		ASSERT( name == "Equal" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " Equal (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << (GX_STL::GXMath::Max(nl,nr) == 1 ? (String(shared) << "\"(x) == " << shared << "(y)") :
			(String("equal( ") << shared << "(x), " << shared << "(y) )")) << ";\n}";

		return true;
	}

/*
=================================================
	NotEqual
=================================================
*/
	struct NotEqual_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		NotEqual_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "NotEqual", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "NotEqual.out", func );
				result = true;
			}
		}
	};

	static bool NotEqual_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		NotEqual_Func	func( in, out.Front() );
		Helpers::IntegerOrFloatTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool NotEqual_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == in_args[1] and TypeFromType(in_args[0], "Bool") == out_args.Front() );
		ASSERT( name == "NotEqual" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " NotEqual (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << (GX_STL::GXMath::Max(nl,nr) == 1 ? (String(shared) << "\"(x) != " << shared << "(y)") :
			(String("notEqual( ") << shared << "(x), " << shared << "(y) )")) << ";\n}";

		return true;
	}

/*
=================================================
	GreaterThan
=================================================
*/
	struct GreaterThan_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		GreaterThan_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GreaterThan", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "GreaterThan.out", func );
				result = true;
			}
		}
	};

	static bool GreaterThan_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		GreaterThan_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool GreaterThan_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == in_args[1] and TypeFromType(in_args[0], "Bool") == out_args.Front() );
		ASSERT( name == "GreaterThan" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " GreaterThan (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << (GX_STL::GXMath::Max(nl,nr) == 1 ? (String(shared) << "\"(x) > " << shared << "(y)") :
			(String("greaterThan( ") << shared << "(x), " << shared << "(y) )")) << ";\n}";

		return true;
	}

/*
=================================================
	LessThan
=================================================
*/
	struct LessThan_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		LessThan_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "LessThan", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "LessThan.out", func );
				result = true;
			}
		}
	};

	static bool LessThan_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		LessThan_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool LessThan_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == in_args[1] and TypeFromType(in_args[0], "Bool") == out_args.Front() );
		ASSERT( name == "LessThan" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " LessThan (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << (GX_STL::GXMath::Max(nl,nr) == 1 ? (String(shared) << "\"(x) < " << shared << "(y)") :
			(String("lessThan( ") << shared << "(x), " << shared << "(y) )")) << ";\n}";

		return true;
	}

/*
=================================================
	GreaterThanEqual
=================================================
*/
	struct GreaterThanEqual_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		GreaterThanEqual_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "GreaterThanEqual", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "GreaterThanEqual.out", func );
				result = true;
			}
		}
	};

	static bool GreaterThanEqual_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		GreaterThanEqual_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool GreaterThanEqual_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == in_args[1] and TypeFromType(in_args[0], "Bool") == out_args.Front() );
		ASSERT( name == "GreaterThanEqual" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " GreaterThanEqual (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << (GX_STL::GXMath::Max(nl,nr) == 1 ? (String(shared) << "\"(x) >= " << shared << "(y)") :
			(String("greaterThanEqual( ") << shared << "(x), " << shared << "(y) )")) << ";\n}";

		return true;
	}

/*
=================================================
	LessThanEqual
=================================================
*/
	struct LessThanEqual_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		LessThanEqual_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and in[0].GetValueTypeId() == TypeIdOf<T>() and in[1].GetValueTypeId() == TypeIdOf<T>() )
			{
				FunctionNode	func( "LessThanEqual", 1, in[0].Get<T>(), in[1].Get<T>() );
				Replace( out.Get<T>(), "LessThanEqual.out", func );
				result = true;
			}
		}
	};

	static bool LessThanEqual_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 2 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == out.Front().GetValueTypeId() );
		
		LessThanEqual_Func	func( in, out.Front() );
		Helpers::AllTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool LessThanEqual_Source (StringCRef sig, OUT String &src)
	{
		StringCRef			name;
		ExtractedArgs_t		in_args;
		ExtractedArgs_t		out_args;

		ExtractArgs( sig, OUT name, OUT in_args, OUT out_args );
		CHECK_ERR( in_args.Count() == 2 and out_args.Count() == 1 );
		CHECK_ERR( in_args[0] == in_args[1] and TypeFromType(in_args[0], "Bool") == out_args.Front() );
		ASSERT( name == "LessThanEqual" );

		uint	nl, nr;
		String	shared;
		CHECK_ERR( IsSameBaseType( in_args[0], in_args[1], OUT shared, OUT nl, OUT nr ) );
		shared = ToLowerCase( shared );

		src << ToLowerCase( out_args.Front() ) << " LessThanEqual (const "
			<< ToLowerCase( in_args[0] ) << " x, const " << ToLowerCase( in_args[1] ) << " y) {\n"
			<< "\treturn " << (GX_STL::GXMath::Max(nl,nr) == 1 ? (String(shared) << "\"(x) <= " << shared << "(y)") :
			(String("lessThanEqual( ") << shared << "(x), " << shared << "(y) )")) << ";\n}";

		return true;
	}


}	// GLSLShaderNodes

namespace _ShaderNodesHidden_
{
	void RegisterGLSLShaderNodes0 (NodeFunctions *nf)
	{
		nf->Register( "GL4.Not", GLSLShaderNodes::Not_Node, &GLSLShaderNodes::Not_Source );
		nf->Register( "GL4.BitInverse", GLSLShaderNodes::BitInverse_Node, &GLSLShaderNodes::BitInverse_Source );
		nf->Register( "GL4.Plus", GLSLShaderNodes::Plus_Node, &GLSLShaderNodes::Plus_Source );
		nf->Register( "GL4.Minus", GLSLShaderNodes::Minus_Node, &GLSLShaderNodes::Minus_Source );
		nf->Register( "GL4.Add", GLSLShaderNodes::Add_Node, &GLSLShaderNodes::Add_Source );
		nf->Register( "GL4.Sub", GLSLShaderNodes::Sub_Node, &GLSLShaderNodes::Sub_Source );
		nf->Register( "GL4.Mul", GLSLShaderNodes::Mul_Node, &GLSLShaderNodes::Mul_Source );
		nf->Register( "GL4.Div", GLSLShaderNodes::Div_Node, &GLSLShaderNodes::Div_Source );
		nf->Register( "GL4.Mod", GLSLShaderNodes::Mod_Node, &GLSLShaderNodes::Mod_Source );
		nf->Register( "GL4.And", GLSLShaderNodes::And_Node, &GLSLShaderNodes::And_Source );
		nf->Register( "GL4.Or", GLSLShaderNodes::Or_Node, &GLSLShaderNodes::Or_Source );
		nf->Register( "GL4.Xor", GLSLShaderNodes::Xor_Node, &GLSLShaderNodes::Xor_Source );
		nf->Register( "GL4.BitLShift", GLSLShaderNodes::BitLShift_Node, &GLSLShaderNodes::BitLShift_Source );
		nf->Register( "GL4.BitRShift", GLSLShaderNodes::BitRShift_Node, &GLSLShaderNodes::BitRShift_Source );
		nf->Register( "GL4.BoolAnd", GLSLShaderNodes::BoolAnd_Node, &GLSLShaderNodes::BoolAnd_Source );
		nf->Register( "GL4.BoolOr", GLSLShaderNodes::BoolOr_Node, &GLSLShaderNodes::BoolOr_Source );
		nf->Register( "GL4.Equal", GLSLShaderNodes::Equal_Node, &GLSLShaderNodes::Equal_Source );
		nf->Register( "GL4.NotEqual", GLSLShaderNodes::NotEqual_Node, &GLSLShaderNodes::NotEqual_Source );
		nf->Register( "GL4.GreaterThan", GLSLShaderNodes::GreaterThan_Node, &GLSLShaderNodes::GreaterThan_Source );
		nf->Register( "GL4.LessThan", GLSLShaderNodes::LessThan_Node, &GLSLShaderNodes::LessThan_Source );
		nf->Register( "GL4.GreaterThanEqual", GLSLShaderNodes::GreaterThanEqual_Node, &GLSLShaderNodes::GreaterThanEqual_Source );
		nf->Register( "GL4.LessThanEqual", GLSLShaderNodes::LessThanEqual_Node, &GLSLShaderNodes::LessThanEqual_Source );
	}


}	// _ShaderNodesHidden_
}	// ShaderEditor
