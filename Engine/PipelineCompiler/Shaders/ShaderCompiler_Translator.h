// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Bugs:
		- incorrect scope of local variable, initialize local variable to fix this issue.
		- incorrect global variable initialization.
*/

#pragma once

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_Utils.h"

namespace PipelineCompiler
{

	struct ConstUnionHash
	{
		using Result_t = HashResult;

		HashResult operator () (const glslang::TConstUnionArray *key) const;

		HashResult _ArrHash (const glslang::TConstUnionArray &key) const;
		HashResult _ValHash (const glslang::TConstUnion &key) const;
	};


	//
	// Translator
	//

	struct Translator
	{
	// types
		struct TypeInfo : CompileTime::ComplexType
		{
		// variables
			Array< TypeInfo >			fields;		// struct fields
			String						typeName;	// for struct only
			String						name;		// if part of struct, empty otherwise
			EVariableQualifier::bits	qualifier;
			EGpuMemoryModel::type		memoryModel	= EGpuMemoryModel::Default;
			EShaderVariable::type		type		= EShaderVariable::Unknown;
			EPrecision::type			precision	= EPrecision::Unknown;
			//EPixelFormat::type		format		= EPixelFormat::Unknown;
			uint						arraySize	= 0;		// 0 - not array, > 0 - static array, ~0 - dynamic
			uint						binding		= UMax;
			bool						isGlobal	= false;	// in some languages globals are forbbiden

		// methods
			TypeInfo() {}
		};


		struct Node
		{
			TypeInfo		typeInfo;
			uint			uid		= 0;
			String			src;
		};


		struct Const
		{
			String					name;
			glslang::TIntermSymbol*	node	= null;
		};

		using ConstMap_t		= HashMap< const glslang::TConstUnionArray*, Const, ConstUnionHash >;

		using NodeMap_t			= Map< uint, Node >;
		using LocalVarSet_t		= Set< uint >;
		using PendingVars_t		= Map< uint, TypeInfo >;
		using PendingStrings_t	= Array< String >;
		using InlFunctionsMap_t	= HashMap< String, TIntermNode* >;		// function signature and node
		using LocalNames_t		= HashSet< String >;
		using StringStack_t		= Stack< String >;
		using LocalReplacer_t	= HashMap< String, String >;
		using CustomTypes_t		= HashMap< String, TypeInfo >;

		class IDstLanguage;
		using IDstLanguagePtr	= UniquePtr< IDstLanguage >;


	// variables
		String				src;
		String				log;
		NodeMap_t			nodes;

		// constants
		struct {
			LocalNames_t		uniqueNames;
			ConstMap_t			symbNodes;
			String				source;
			bool				optimize	= false;	// use global constants instead of local

		}					constants;

		// for inllining
		struct {
			InlFunctionsMap_t	functions;
			LocalNames_t		localNames;			// names in inline function must be unique, so we need to store all (local?) names
			StringStack_t		prefixStack;		// if current string not empty then it is inline function and all local variables must have prefix
			LocalReplacer_t		localsReplacer;		// replace func args and local variables by new name

		}					inl;

		// function and variable forward declaration
		struct {
			LocalVarSet_t		definedLocalVars;		// list of defined (local) variables
			LocalVarSet_t		funcArguments;			// list of function arguments
			PendingVars_t		pendingVars;			// this variables will be added before current line
			PendingStrings_t	addBeforeLine;			// this source lines will be added before current line

		}					fwd;

		// types
		struct {
			CustomTypes_t		globalTypes;
			HashSet<String>		definedInExteranal;		// this type must be skiped if 'skipExternals' is true

		}					types;

		IDstLanguagePtr		language;
		String				entryPoint;
		uint				uid			= 0;		// counter
		bool				isMain		= false;
		bool				useGXrules	= true;


	// methods
		Translator ()
		{
			inl.prefixStack.SetDefault( "" );
		}

		bool IsInline () const		{ return inl.prefixStack.Count() > 1; }

		bool Main (TIntermNode* root, const uint uid, bool skipExternals);
	};
	


	//
	// Destination Language interface
	//

	class Translator::IDstLanguage
	{
	public:
		virtual ~IDstLanguage () {}

		virtual bool TranslateLocalVar (const TypeInfo &, INOUT String &src) = 0;
		virtual bool TranslateStruct (const TypeInfo &, INOUT String &src) = 0;
		virtual bool TranslateArg (const TypeInfo &, INOUT String &src) = 0;
		virtual bool TranslateType (const TypeInfo &, INOUT String &src) = 0;
		virtual bool TranslateName (const TypeInfo &, INOUT String &src) = 0;

		virtual bool TranslateExternal (glslang::TIntermTyped *, const TypeInfo &, INOUT String &src) = 0;
		virtual bool TranslateOperator (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, INOUT String &src) = 0;
		virtual bool TranslateSwizzle (const TypeInfo &type, StringCRef val, StringCRef swizzle, INOUT String &src) = 0;

		virtual bool TranslateEntry (const TypeInfo &ret, StringCRef name, ArrayCRef<TypeInfo> args, INOUT String &src) = 0;

		virtual bool TranslateStructAccess (const TypeInfo &stType, StringCRef objName, const TypeInfo &fieldType, INOUT String &src) = 0;

		virtual bool DeclExternalTypes () const	{ return false; }
	};
	
	

	//
	// Constant Union to String
	//

	struct CU_ToArray_Func
	{
	// types
	private:
		using TypeInfo = Translator::TypeInfo;
		

	// variables
	private:
		Array<String>			_strArr;
		Array<TypeInfo const*>	_typeArr;
		TypeInfo				_type;


	// methods
	public:
		CU_ToArray_Func () {}

		ArrayCRef<String>			GetStrings ()	const	{ return _strArr; }
		ArrayCRef<TypeInfo const*>	GetTypes ()		const	{ return _typeArr; }
		
		template <typename T>
		void operator () (const T &value)
		{
			_CreateType<T>( _type );
			_strArr.PushBack( _ToString( value ) );
			_typeArr.PushBack( &_type );
		}
		
		template <typename T, usize I>
		void operator () (const Vec<T,I> &value)
		{
			_CreateType<T>( _type );
			
			if ( All( value == value.x ) )
			{
				_strArr.PushBack( _ToString( value.x ) );
				_typeArr.PushBack( &_type );
			}
			else
			{
				FOR( i, value )
				{
					_strArr.PushBack( _ToString( value[i] ) );
					_typeArr.PushBack( &_type );
				}
			}
		}

		template <typename T, usize C, usize R>
		void operator () (const Matrix<T,C,R> &value)
		{
			_CreateType<T>( _type );

			TODO( "" );
		}


	private:
		template <typename T>
		static void _CreateType (OUT TypeInfo &type)
		{
			type.arraySize		= 0;
			type.binding		= UMax;
			type.isGlobal		= false;
			type.memoryModel	= Uninitialized;
			type.precision		= Uninitialized;
			type.qualifier		= Uninitialized;
			type.type			= EShaderVariable::ToScalar<T>();
		}

		static String _ToString (float value)
		{
			return String().FormatF( value, StringFormatF().Fmt(0,8).CutZeros() );
		}

		static String _ToString (double value)
		{
			return String().FormatF( value, StringFormatF().Fmt(0,16).CutZeros() );
		}

		template <typename T>
		static String _ToString (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsScalar<T> );
			return ToString( value );
		}
	};


	/*
	struct CU_ToString_Func
	{
	public:
		String &	str;

		CU_ToString_Func (String &str) : str(str)
		{}

		template <typename T>
		void operator () (const T &value) const
		{
			str << "(" << _ToString( value ) << ")";
		}
		
		template <typename T, usize I>
		void operator () (const Vec<T,I> &value) const
		{
			if ( All( value == value.x ) )
				str << "(" << _ToString( value.x ) << ")";
			else
				str << "(" << _ToString( value ) << ")";
		}

		template <typename T, usize C, usize R>
		void operator () (const Matrix<T,C,R> &value) const
		{
			str << "(" << _ToString( value ) << ")";
		}


	private:
		static String _ToString (float value)
		{
			return String().FormatF( value, StringFormatF().Fmt(0,8).CutZeros() );
		}

		static String _ToString (double value)
		{
			return String().FormatF( value, StringFormatF().Fmt(0,16).CutZeros() );
		}

		template <typename T>
		static String _ToString (const T &value)
		{
			return ToString( value );
		}

		template <typename T, usize I>
		static String _ToString (const Vec<T,I> &value)
		{
			String	str;

			FOR( i, value ) {
				str << (i ? ", " : "") << _ToString( value[i] );
			}
			return str;
		}

		template <typename T, usize C, usize R>
		static String _ToString (const Matrix<T,C,R> &value)
		{
			String	str;

			FOR( i, value ) {
				str << (i ? ", " : "") << _ToString( value[i] );
			}
			return str;
		}
	};*/

}	// PipelineCompiler
