// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_Utils.h"

namespace PipelineCompiler
{

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
			EGpuMemoryModel::type		memoryModel	= EGpuMemoryModel::None;
			EShaderVariable::type		type		= EShaderVariable::Unknown;
			EPrecision::type			precision	= EPrecision::Unknown;
			EPixelFormat::type			format		= EPixelFormat::Unknown;
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

		using NodeMap_t			= Map< uint, Node >;
		using LocalVarSet_t		= Set< uint >;
		using PendingVars_t		= Map< uint, TypeInfo >;
		using PendingStrings_t	= Array< String >;
		using InlFunctionsMap_t	= Map< String, TIntermNode* >;		// function signature and node
		using LocalNames_t		= Set< String >;
		using StringStack_t		= Stack< String >;
		using LocalReplacer_t	= HashMap< String, String >;
		using Constants_t		= Set< glslang::TIntermSymbol* >;
		using CustomTypes_t		= Map< String, glslang::TIntermTyped* >;

		class IDstLanguage;
		using IDstLanguagePtr	= UniquePtr< IDstLanguage >;


	// variables
		String				src;
		String				log;
		NodeMap_t			nodes;
		Constants_t			constNodes;
		
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
			PendingVars_t		pendingVars;			// this variables will be added before current line
			PendingStrings_t	addBeforeLine;			// this source lines will be added before current line

		}					fwd;

		// types
		struct {
			CustomTypes_t		globalTypes;

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
		virtual bool TranslateArg (const TypeInfo &, INOUT String &src) = 0;
		virtual bool TranslateType (const TypeInfo &, INOUT String &src) = 0;

		virtual bool TranslateExternal (glslang::TIntermTyped *, const TypeInfo &, INOUT String &src) = 0;
		virtual bool TranslateOperator (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, INOUT String &src) = 0;
		virtual bool TranslateSwizzle (const TypeInfo &type, StringCRef val, StringCRef swizzle, INOUT String &src) = 0;
	};
	
	

	//
	// Constant Union to String
	//

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
	};

}	// PipelineCompiler
