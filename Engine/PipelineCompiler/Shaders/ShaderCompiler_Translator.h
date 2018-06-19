// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Bugs:
		- incorrect scope of local variable, initialize local variable to fix this issue.
		- incorrect global variable initialization.

	TODO:
		- check variable names that equal to reserved keywords, unsupported prefixes and other.
*/

#pragma once

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_Utils.h"
#include "Core/STL/Containers/Stack.h"
#include "Core/STL/Algorithms/StringParser.h"

namespace PipelineCompiler
{

	//
	// Translator
	//

	struct Translator final
	{
	// types
		enum SymbolID : uint {};

		struct TypeInfo : CompileTime::ComplexType
		{
		// variables
			Array< TypeInfo >			fields;			// struct fields
			String						typeName;		// for struct only
			String						name;			// if part of struct, empty otherwise
			EVariableQualifier::bits	qualifier;
			EShaderMemoryModel::type	memoryModel		= EShaderMemoryModel::Default;
			EShaderVariable::type		type			= EShaderVariable::Unknown;
			EPrecision::type			precision		= EPrecision::Unknown;
			EPixelFormat::type			format			= EPixelFormat::Unknown;
			ArraySize					arraySize;
			bool						isGlobal		= false;	// in some languages globals are forbbiden
			BytesU						sizeOf;

		// methods
			TypeInfo () {}
		};

		struct Symbol : TypeInfo
		{
		// variables
			// 'name' - symbol name
			SymbolID		id				= SymbolID(0);
			uint			binding			= UMax;
			uint			specConstID		= UMax;		// specialization const id
			
		// methods
			Symbol () {}
			explicit Symbol (const TypeInfo &other) : TypeInfo{other} {}
		};


		struct Node
		{
			TypeInfo		typeInfo;
			uint			uid		= 0;
			String			src;
		};


		struct ConstKey
		{
			glslang::TConstUnionArray const*	cu;
			TypeInfo							info;

			bool operator == (const ConstKey &right) const;
			bool operator >  (const ConstKey &right) const;
		};

		struct ConstName
		{
			String					name;
			glslang::TIntermSymbol*	node	= null;
		};
		
		struct ConstKeyHash
		{
			HashResult operator () (const ConstKey &key) const;
		};

		using ConstMap_t		= HashMap< ConstKey, ConstName, ConstKeyHash >;

		using NodeMap_t			= Map< uint, Node >;
		using LocalVarSet_t		= Map< SymbolID, String >;					// variable ID, name with prefix from inline function
		using PendingVars_t		= Map< SymbolID, Symbol >;
		using PendingStrings_t	= Array< String >;
		using InlFunctionsMap_t	= HashMap< String, TIntermNode* >;			// function signature and node
		using LocalNames_t		= HashSet< String >;
		using LocalReplacer_t	= HashMap< SymbolID, String >;
		using CustomTypes_t		= HashMap< String, TypeInfo >;				// typename, fields
		using AtomicTypes_t		= MultiHashMap< String, Array<String> >;	// typename, field names
		using FnLocalVarStack_t	= Array< LocalVarSet_t >;

		struct Scope
		{
			PendingVars_t		pendingVars;		// this variables will be added before current line
			PendingStrings_t	addBeforeLine;		// this source lines will be added before current line
			bool				reqEndLine	= true;
		};

		struct InlFuncScope
		{
			String				prefix;				// 
			Set<SymbolID>		localVars;			// 
		};

		class IDstLanguage;
		using IDstLanguagePtr	= UniquePtr< IDstLanguage >;
		using InlFuncStack_t	= Stack< InlFuncScope >;


	// variables
		String				src;
		String &			log;
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
			InlFunctionsMap_t	allFunctions;
			LocalNames_t		localNames;			// names in inline function must be unique, so we need to store all (local?) names
			InlFuncStack_t		prefixStack;		// if current string not empty then it is inline function and all local variables must have prefix
			LocalReplacer_t		localsReplacer;		// replace func args and local variables by new name

		}					inl;

		// function and variable forward declaration
		struct {
			//LocalVarSet_t		definedLocalVars;		// list of defined (local) variables
			LocalVarSet_t		funcArguments;			// list of function arguments
			Array<Scope>		scope;
			FnLocalVarStack_t	fnScope;

		}					fwd;

		// types
		struct {
			CustomTypes_t		globalTypes;
			HashSet<String>		definedInExteranal;		// this type must be skiped if 'skipExternals' is true
			AtomicTypes_t		atomics;
			Set<TIntermNode*>	bufferNodes;			// used for buffer load/store functions

		}					types;

		// states
		struct {
			bool				isMain				= false;
			bool				useGXrules			= true;
			bool				inlineAll			= false;

		}					states;

		Array<TIntermNode*>	nodeStack;

		IDstLanguagePtr		language;
		String				entryPoint;
		String				dstEntryPoint;			// after translation entry point may be renamed
		uint				uid			= 0;		// counter


	// methods
		explicit Translator (String &log) : log{log}
		{
			inl.prefixStack.SetDefault({ "", {} });
		}

		bool IsInline () const		{ return inl.prefixStack.Count() > 1; }

		bool Main (TIntermNode* root, bool skipExternals);
	};
	


	//
	// Destination Language interface
	//

	class Translator::IDstLanguage
	{
	// types
	public:
		using TypeInfo	= Translator::TypeInfo;
		using Symbol	= Translator::Symbol;
		using SymbolID	= Translator::SymbolID;


	// interface
	public:
		virtual ~IDstLanguage () {}
		virtual bool TranslateLocalVar (const Symbol &, INOUT String &src) = 0;
		virtual bool TranslateStruct (const TypeInfo &, INOUT String &src) = 0;
		virtual bool TranslateType (const TypeInfo &, INOUT String &src) = 0;
		virtual bool TranslateName (const Symbol &, INOUT String &src) = 0;
		virtual bool ValidateSymbolName (INOUT Symbol &, uint fnScopeId) = 0;	// local variables, function arguments, externals and other
		virtual bool ValidateFieldName (INOUT TypeInfo &) = 0;	// struct fields only
		virtual bool TranslateFunctionDecl (StringCRef sign, const TypeInfo &resultType, ArrayCRef<Symbol> args, INOUT String &src) = 0;
		virtual bool TranslateFunctionCall (StringCRef sign, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, INOUT String &src) = 0;
		virtual bool MustBeInlined (StringCRef sign, ArrayCRef<TypeInfo const*> argTypes) = 0;
		virtual bool TranslateConstant (const glslang::TConstUnionArray &, const TypeInfo &, INOUT String &src) = 0;
		virtual bool TranslateExternal (glslang::TIntermTyped *, const Symbol &, INOUT String &src) = 0;
		virtual bool TranslateOperator (glslang::TOperator op, const TypeInfo &resultType, ArrayCRef<String> args, ArrayCRef<TypeInfo const*> argTypes, INOUT String &src) = 0;
		virtual bool TranslateSwizzle (const TypeInfo &result, const TypeInfo &valType, StringCRef val, ArrayCRef<uint> swizzle, INOUT String &src) = 0;
		virtual bool TranslateEntry (const TypeInfo &ret, StringCRef name, ArrayCRef<Symbol> args, StringCRef body, OUT String &entryPoint, INOUT String &src) = 0;
		virtual bool TranslateStructAccess (SymbolID id, const TypeInfo &stType, StringCRef objName, const TypeInfo &fieldType, INOUT String &src) = 0;
		virtual bool TranslateBufferLoad (SymbolID id, StringCRef objName, StringCRef fieldOffset, const TypeInfo &fieldType, INOUT String &src) = 0;
		virtual bool TranslateBufferStore (SymbolID id, StringCRef objName, StringCRef fieldOffset, const TypeInfo &fieldType, StringCRef dataStr, const TypeInfo &dataType, INOUT String &src) = 0;
		virtual bool TranslateValue (VariantCRef value, INOUT String &src) const = 0;
		virtual bool DeclExternalTypes () const	= 0;
		virtual bool ReplaceStructByBuffer () const = 0;
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
		Ptr< Translator::IDstLanguage >		_language;

		Array<String>			_strArr;
		Array<TypeInfo const*>	_typeArr;
		TypeInfo				_type;
		bool					_supportVecFromScalar = true;	// except HLSL


	// methods
	public:
		explicit CU_ToArray_Func (Ptr<Translator::IDstLanguage> lang, bool supportVecFromScalar = true) :
			_language{lang}, _supportVecFromScalar{supportVecFromScalar}
		{}

		ArrayCRef<String>			GetStrings ()	const	{ return _strArr; }
		ArrayCRef<TypeInfo const*>	GetTypes ()		const	{ return _typeArr; }
		
		template <typename T>
		void operator () (const T &value)
		{
			_CreateType<T>( OUT _type );
			_strArr.PushBack( _ToString( value ) );
			_typeArr.PushBack( &_type );
		}
		
		template <typename T, usize I>
		void operator () (const Vec<T,I> &value)
		{
			_CreateType<T>( OUT _type );
			
			if ( _supportVecFromScalar and All( value == value.x ) )
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
			_CreateType<T>( OUT _type );
			
			FOR( i, value )
			{
				_strArr.PushBack( _ToString( value[i] ) );
				_typeArr.PushBack( &_type );
			}
		}


	private:
		template <typename T>
		static void _CreateType (OUT TypeInfo &type)
		{
			type.arraySize.MakeNonArray();
			type.isGlobal		= false;
			type.memoryModel	= Uninitialized;
			type.precision		= Uninitialized;
			type.qualifier		= Uninitialized;
			type.type			= EShaderVariable::ToScalar<T>();
		}

		template <typename T>
		String _ToString (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsScalar<T> );

			String	str;
			_language->TranslateValue( value, OUT str );
			return str;
		}
	};


}	// PipelineCompiler
