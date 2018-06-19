// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_Translator.h"
#include "Engine/PipelineCompiler/Common/ToGLSL.h"
#include "Core/STL/Algorithms/Range.h"

namespace PipelineCompiler
{

#	define CHECK_COMP2( _func_ ) \
	{ \
		String&	log = translator.log; \
		CHECK_COMP( _func_ ); \
	}

	static bool RecursiveProcessAggregateNode (TIntermNode* node, const uint uid, Translator &translator);
	static bool RecursiveProcessNode (TIntermNode* node, const uint uid, Translator &translator);
	static bool RecursiveProcessBranchNode (TIntermNode* node, const uint uid, Translator &translator);
	static bool RecursiveProcessSwitchNode (TIntermNode* node, const uint uid, Translator &translator);
	static bool RecursiveProcessConstUnionNode (TIntermNode* node, const uint uid, Translator &translator);
	static bool RecursiveProcessSelectionNode (TIntermNode* node, const uint uid, Translator &translator);
	static bool RecursiveProcessMethodNode (TIntermNode* node, const uint uid, Translator &translator);
	static bool RecursiveProcessSymbolNode (TIntermNode* node, const uint uid, Translator &translator);
	//static bool RecursiveProcessTypedNode (TIntermNode* node, const uint uid, Translator &translator);
	static bool RecursiveProcessOperatorNode (TIntermNode* node, const uint uid, Translator &translator);
	static bool RecursiveProcessUnaryNode (TIntermNode* node, const uint uid, Translator &translator);
	static bool RecursiveProcessBinaryNode (TIntermNode* node, const uint uid, Translator &translator);
	static bool RecursiveProcessLoopNode (TIntermNode* node, const uint uid, Translator &translator);

	static bool RecursivePrepass (TIntermNode* node, Translator &translator);
	static bool RegisterBUfferAccess (TIntermNode* node, Translator &translator);
	static bool AddAtomicArgQualifier (TIntermNode* node, Translator &translator);
	static bool RegisterFunctionDeclaration (TIntermNode* node, Translator &translator);
	static bool RegisterStructType (glslang::TIntermSymbol* node, Translator &translator);

	static bool TranslateFunction (glslang::TIntermAggregate* aggr, const uint uid, Translator &translator);
	static bool TranslateExternalObjects (glslang::TIntermAggregate* aggr, const uint uid, Translator &translator);
	static bool TranslateSharedObjects (glslang::TIntermAggregate* aggr, const uint uid, Translator &translator);
	static bool TranslateGlobals (glslang::TIntermAggregate* aggr, const uint uid, Translator &translator);
	static bool ConvertType (TIntermNode* node, glslang::TType const &, const Translator::TypeInfo *parent, const Translator &, OUT Translator::TypeInfo &);
	static bool ConvertSymbol (TIntermNode* node, glslang::TType const &, const Translator::TypeInfo *parent, const Translator &, OUT Translator::Symbol &);
	static bool IsBuiltinName (StringCRef name);
	static bool TranslateVectorSwizzle (glslang::TIntermOperator* node, const uint uid, Translator &translator);
	static bool TranslateIndexDirectStruct (glslang::TIntermOperator* node, const uint uid, Translator &translator);
	static bool TranslateLoadStoreOp (TIntermNode* node, const uint uid, Translator &translator);
	static bool TranslateOperatorCall (TIntermNode* node, const uint uid, Translator &translator);
	static bool TranslateFunctionCall (TIntermNode* node, const uint uid, Translator &translator);
	static bool TranslateInlineFunctionCall (TIntermNode* node, const uint uid, TIntermNode* func, Translator &translator);
	static bool TranslateMain (TIntermNode* node, const uint uid, bool skipExternals, Translator &translator);
	static bool TranslateConstUnionAccess (TIntermNode* node, const uint uid, Translator &translator);
	static bool TranslateFunctionForwardDecl (glslang::TIntermAggregate* node, const Translator &translator, INOUT String &src);

	static bool GXCheckAccessToExternal (const Translator &translator, const Translator::Node &node);
	static bool GXCheckExternalQualifiers (const Translator &translator, const glslang::TQualifier &qual);
	static bool GXCheckBuiltinAccess (const Translator &translator, const Translator::Node &node);

	static Translator::SymbolID GetSymbolID (TIntermNode *node);
	static bool IsProgramEntry (StringCRef signature, const Translator &translator);
	static BytesU GetElementSize (const glslang::TType &info, bool forArrayElement = false);
	
		
/*
=================================================
	Main
=================================================
*/
	bool Translator::Main (TIntermNode* node, bool skipExternals)
	{
		nodeStack.PushBack( node );
		fwd.scope.PushBack( {} );
		fwd.fnScope.PushBack( {} );

		CHECK_ERR( TranslateMain( node, this->uid, skipExternals, *this ) );

		nodeStack.PopBack();
		fwd.scope.PopBack();
		fwd.fnScope.PopBack();

		CHECK( nodeStack.Empty() );
		CHECK( fwd.scope.Empty() );
		CHECK( fwd.fnScope.Empty() );

		return true;
	}

/*
=================================================
	GXCheckAccessToExternal
=================================================
*/
	static bool GXCheckAccessToExternal (const Translator &translator, const Translator::Node &node)
	{
		if ( translator.states.useGXrules )
		{
			// can't access to global objects directly from non-main function.
			// this rule used for compatibility with OpenCL and C++.
			if ( node.typeInfo.isGlobal )
			{
				return	translator.states.isMain	or
						translator.states.inlineAll;
			}
		}
		return true;
	}
	
/*
=================================================
	GXCheckBuiltinAccess
=================================================
*/
	static bool GXCheckBuiltinAccess (const Translator &translator, const Translator::Node &node)
	{
		if ( translator.states.useGXrules )
		{
			// can't access to builtin variables directly from non-main function.
			// this rule used for compatibility with OpenCL and C++.
			if ( node.typeInfo.qualifier[ EVariableQualifier::BuiltIn ] )
			{
				return	translator.states.isMain	or
						translator.states.inlineAll;
			}
		}
		return true;
	}

/*
=================================================
	GXCheckExternalQualifiers
=================================================
*/
	static bool GXCheckExternalQualifiers (const Translator &translator, const glslang::TQualifier &qual)
	{
		if ( translator.states.useGXrules )
		{
			switch ( qual.storage )
			{
				case glslang::TStorageQualifier::EvqBuffer :
				case glslang::TStorageQualifier::EvqConst :
				case glslang::TStorageQualifier::EvqUniform :
				case glslang::TStorageQualifier::EvqVaryingIn :
				case glslang::TStorageQualifier::EvqVaryingOut :
				case glslang::TStorageQualifier::EvqShared :
					return true;

				default :
					return false;
			}
		}
		return true;
	}
	
/*
=================================================
	GetSymbolID
=================================================
*/
	static Translator::SymbolID  GetSymbolID (TIntermNode *node)
	{
		if (auto* symb = node->getAsSymbolNode())
		{
			return Translator::SymbolID( symb->getId() );
		}

		return Translator::SymbolID(~0u);
	}
	
/*
=================================================
	IsProgramEntry
=================================================
*/
	static bool IsProgramEntry (StringCRef signature, const Translator &translator)
	{
		return signature.StartsWith( translator.entryPoint + '(' );
	}
	
/*
=================================================
	TConstUnionHash
=================================================
*/
	static HashResult  TConstUnionHash (const glslang::TConstUnion &key)
	{
		switch (key.getType())
		{
			case glslang::TBasicType::EbtFloat :
			case glslang::TBasicType::EbtDouble :	return HashOf( key.getDConst() );
			#ifdef AMD_EXTENSIONS
			//case glslang::TBasicType::EbtFloat16
			#endif
			case glslang::TBasicType::EbtInt :		return HashOf( key.getIConst() );
			case glslang::TBasicType::EbtUint :		return HashOf( key.getUConst() );
			case glslang::TBasicType::EbtInt64 :	return HashOf( key.getI64Const() );
			case glslang::TBasicType::EbtUint64 :	return HashOf( key.getU64Const() );
			#ifdef AMD_EXTENSIONS
			//case glslang::TBasicType::EbtInt16
			//case glslang::TBasicType::EbtUint16
			#endif
			case glslang::TBasicType::EbtBool :		return HashOf( key.getBConst() );
			//case glslang::TBasicType::EbtAtomicUint
			//case glslang::TBasicType::EbtSampler
			//case glslang::TBasicType::EbtStruct
			//case glslang::TBasicType::EbtBlock
			case glslang::TBasicType::EbtString :	return HashOf( key.getSConst()->c_str() );
			default :								RETURN_ERR( "unsupported type!" );
		}
	}

/*
=================================================
	TConstUnionArrayHash
=================================================
*/
	static HashResult  TConstUnionArrayHash (const glslang::TConstUnionArray &key)
	{
		HashResult	result;
		for (int i = 0; i < key.size(); ++i) {
			result += TConstUnionHash( key[i] );
		}
		return result;
	}
	
/*
=================================================
	TypeInfoHash
=================================================
*/
	static HashResult  TypeInfoHash (const Translator::TypeInfo &key)
	{
		HashResult	result;
		// ignore '.fields'
		result += HashOf( key.typeName );
		// ignore '.name'
		// ignore '.qualifier'
		// ignore '.memoryModel'
		result += HashOf( key.type );
		// ignore '.precision'
		// ignore '.format'
		// ignore '.specConstID'
		result += HashOf( key.arraySize );
		// ignore '.binding'
		// ignore '.isGlobal'
		return result;
	}

/*
=================================================
	hash of ConstKey
=================================================
*/
	HashResult  Translator::ConstKeyHash::operator () (const ConstKey &key) const
	{
		HashResult	result;
		result += TConstUnionArrayHash( *key.cu );
		result += TypeInfoHash( key.info );
		return result;
	}

/*
=================================================
	ConstKey::operator ==
=================================================
*/
	bool Translator::ConstKey::operator == (const ConstKey &right) const
	{
		return	*cu				== *right.cu			and
				info.typeName	== right.info.typeName	and
				info.type		== right.info.type		and
				info.arraySize	== right.info.arraySize;
	}
	
/*
=================================================
	ConstKey::operator >
=================================================
*/
	bool Translator::ConstKey::operator >  (const ConstKey &right) const
	{
		return	cu				!= right.cu				?	cu				> right.cu				:
				info.typeName	!= right.info.typeName	?	info.typeName	> right.info.typeName	:
				info.type		!= right.info.type		?	info.type		> right.info.type		:
															info.arraySize	> right.info.arraySize;
	}

/*
=================================================
	TranslateCustomTypes
=================================================
*/
	static bool TranslateCustomTypes (Translator &translator, bool skipExternals)
	{
		auto&	src = translator.src;

		if ( skipExternals )
		{
			// remove external types
			FOR( i, translator.types.definedInExteranal )
			{
				translator.types.globalTypes.Erase( translator.types.definedInExteranal[i] );
			}
		}

		translator.types.definedInExteranal.Clear();

		// sort by dependencies
		for (uint k = 0; k < 10 and not translator.types.globalTypes.Empty(); ++k)
		{
			bool	changed = false;

			FOR( i, translator.types.globalTypes )
			{
				const auto&	info	= translator.types.globalTypes[i].second;
				bool		blocked	= false;

				// check is all dependent types was declared
				FOR( j, info.fields )
				{
					auto&	fl = info.fields[j];

					if ( not fl.typeName.Empty() )
					{
						if ( translator.types.globalTypes.IsExist( fl.typeName ) )
						{
							blocked = true;
							break;
						}
					}
				}

				// waiting for dependent types
				if ( blocked )
					continue;
			
				// process and remove from map
				CHECK_ERR( translator.language->TranslateStruct( info, INOUT src ) );

				translator.types.globalTypes.EraseByIndex( i );
				--i;

				changed = true;
			}

			if ( changed )
				k = 0;
		}

		CHECK_ERR( translator.types.globalTypes.Empty() );

		src << "\n//---------------------------------\n\n";
		return true;
	}

/*
=================================================
	TranslateMain
=================================================
*/
	static bool TranslateMain (TIntermNode* rootNode, const uint, bool skipExternals, Translator &translator)
	{
		using DeclFunction_t	= Set< glslang::TIntermAggregate* >;

		glslang::TIntermAggregate* aggr = rootNode->getAsAggregate();
		CHECK_ERR( aggr and aggr->getOp() == glslang::TOperator::EOpSequence );
		
		// find atomics and add 'volatile' qualifier,
		// find function definitions
		CHECK_ERR( RecursivePrepass( rootNode, translator ) );

		// get external objects
		const uint		ext_uid		= ++translator.uid;
		const uint		global_uid	= ++translator.uid;
		const uint		shared_uid	= ++translator.uid;
		DeclFunction_t	funcs;

		FOR( i, aggr->getSequence() )
		{
			TIntermNode *				node = aggr->getSequence()[i];
			glslang::TIntermAggregate*	aggr2 = node->getAsAggregate();

			if ( aggr2 and aggr2->getOp() == glslang::TOperator::EOpLinkerObjects )
			{
				CHECK_ERR( TranslateExternalObjects( aggr2, ext_uid, translator ) );
				CHECK_ERR( TranslateSharedObjects( aggr2, shared_uid, translator ) );
				CHECK_ERR( TranslateGlobals( aggr2, global_uid, translator ) );
			}
			else
			if ( aggr2 and aggr2->getOp() == glslang::TOperator::EOpFunction )
			{
				funcs.Add( aggr2 );
			}
		}

		// custom types declaration
		if ( not translator.types.globalTypes.Empty() )
		{
			CHECK_ERR( TranslateCustomTypes( translator, skipExternals ) );
		}

		// source
		String	src;
		FOR( i, aggr->getSequence() )
		{
			TIntermNode*	node = aggr->getSequence()[i];

			if ( node->getAsAggregate() and node->getAsAggregate()->getOp() == glslang::TOperator::EOpLinkerObjects )
				continue;

			const uint	arg_uid = ++translator.uid;
			CHECK_ERR( RecursiveProcessNode( node, arg_uid, translator ) );
			
			// node may not exist if it is a function declaration
			if ( not translator.nodes.IsExist( arg_uid ) )
				continue;

			const auto&	arg = translator.nodes( arg_uid );
			CHECK_ERR( not arg.src.Empty() );

			src << arg.src << "\n";
		}

		// add externals
		if ( not skipExternals )
		{
			StringCRef	ext_src = translator.nodes( ext_uid ).src;

			if ( not ext_src.Empty() ) {
				translator.src	<< ext_src
								<< "\n//---------------------------------\n\n";
			}
		}

		// add shared
		{
			StringCRef	shared_src = translator.nodes( shared_uid ).src;

			if ( not shared_src.Empty() ) {
				translator.src	<< shared_src
								<< "\n//---------------------------------\n\n";
			}
		}

		// add constants and globals
		{
			StringCRef	global_src	= translator.nodes( global_uid ).src;
			StringCRef	const_src	= translator.constants.source;

			if ( not global_src.Empty() or not const_src.Empty() )
			{
				translator.src	<< global_src
								<< const_src
								<< "\n//---------------------------------\n\n";
			}
		}

		// function forward declaration
		if ( not funcs.Empty() )
		{
			const usize	len = translator.src.Length();

			FOR( i, funcs ) {
				CHECK_ERR( TranslateFunctionForwardDecl( funcs[i], translator, INOUT translator.src ) );
			}

			if ( len != translator.src.Length() ) {
				translator.src << "\n//---------------------------------\n\n";
			}
		}
		
		translator.src << src;
		return true;
	}

/*
=================================================
	TranslateFunctionForwardDecl
=================================================
*/
	static bool TranslateFunctionForwardDecl (glslang::TIntermAggregate* aggr, const Translator &translator, INOUT String &src)
	{
		ASSERT( aggr->getOp() == glslang::TOperator::EOpFunction );
		
		Translator::TypeInfo			ret_type;
		Array< Translator::Symbol >		func_args;
		const String					signature		= aggr->getName().c_str();
		bool							will_inlined	= false;

		// get result type
		CHECK_ERR( ConvertType( aggr, aggr->getType(), null, translator, OUT ret_type ) );
		
		if ( IsProgramEntry( signature, translator ) )
			return true;
		
		// find arguments
		FOR( i, aggr->getSequence() )
		{
			TIntermNode* n = aggr->getSequence()[i];

			if ( n->getAsAggregate() and 
				 n->getAsAggregate()->getOp() == glslang::TOperator::EOpParameters )
			{
				// get arguemnts
				glslang::TIntermAggregate*	args_node = n->getAsAggregate();
				
				func_args.Resize( func_args.Count() + args_node->getSequence().size() );

				FOR( j, args_node->getSequence() )
				{
					Translator::Symbol &		arg	= func_args[j];
					glslang::TIntermTyped *		nn	= args_node->getSequence()[j]->getAsTyped();

					CHECK_ERR( ConvertSymbol( nn, nn->getType(), null, translator, OUT arg ) );

					// function with dynamic array must be inlined
					if ( arg.arraySize.IsDynamicArray() )
						will_inlined = true;
				}
			}
		}
		
		src << (will_inlined ? "// " : "");

		CHECK_ERR( translator.language->TranslateFunctionDecl( signature, ret_type, func_args, INOUT src ) );

		src << ";" << (will_inlined ? "\t// will be inlined\n" : "\n");
		return true;
	}

/*
=================================================
	RecursiveProcessAggregateNode
=================================================
*/
	static bool RecursiveProcessAggregateNode (TIntermNode* node, const uint uid, Translator &translator)
	{
		glslang::TIntermAggregate* aggr = node->getAsAggregate();
		
		if ( aggr->getOp() == glslang::TOperator::EOpSequence )
		{
			// search 'for' loop node
			if ( not aggr->getSequence().empty() )
			{
				if ( glslang::TIntermLoop* loop = aggr->getSequence().back()->getAsLoopNode() )
				{
					if ( loop->testFirst() and loop->getTerminal() )
					{
						CHECK_ERR( RecursiveProcessLoopNode( aggr, uid, translator ) );
						return true;
					}
				}
			}

			Translator::Node	dst_node;
			dst_node.uid = uid;

			translator.fwd.scope.PushBack( {} );

			FOR( i, aggr->getSequence() )
			{
				auto& scope = translator.fwd.scope.Back();
				scope.reqEndLine = true;

				const uint	arg_uid = ++translator.uid;
				CHECK_ERR( RecursiveProcessNode( aggr->getSequence()[i], arg_uid, translator ) );

				// add local variable declaration
				FOR( j, scope.pendingVars )
				{
					Translator::Symbol const&	info = scope.pendingVars[j].second;

					CHECK_ERR( translator.language->TranslateLocalVar( info, INOUT dst_node.src ) );
					dst_node.src << ";\n";
				}

				// add strings
				FOR( j, scope.addBeforeLine ) {
					dst_node.src << scope.addBeforeLine[j] << ";\n";
				}

				scope.pendingVars.Clear();
				scope.addBeforeLine.Clear();

				const auto&	arg = translator.nodes( arg_uid );
				CHECK_ERR( not arg.src.Empty() );

				dst_node.src << arg.src;

				if ( scope.reqEndLine )
				{
					scope.reqEndLine = false;
					dst_node.src << ";\n";
				}
			}

			translator.fwd.scope.PopBack();

			translator.nodes.Add( uid, RVREF(dst_node) );
			return true;
		}

		// function definition
		if ( aggr->getOp() == glslang::TOperator::EOpFunction )
		{
			CHECK_ERR( TranslateFunction( aggr, uid, translator ) );
			return true;
		}

		// uniforms, buffers, images, ...
		if ( aggr->getOp() == glslang::TOperator::EOpLinkerObjects )
		{
			RETURN_ERR( "not supported here, must be used in 'TranslateMain'" );
		}

		// function call
		if ( aggr->getOp() == glslang::TOperator::EOpFunctionCall )
		{
			CHECK_ERR( TranslateFunctionCall( node, uid, translator ) );
			return true;
		}

		// builtin function call
		if ( (aggr->getOp() >= glslang::TOperator::EOpAdd and aggr->getOp() <= glslang::TOperator::EOpLogicalAnd) or
			 (aggr->getOp() >= glslang::TOperator::EOpRadians and aggr->getOp() <= glslang::TOperator::EOpAll) or
			 (aggr->getOp() > glslang::TOperator::EOpConstructGuardStart and aggr->getOp() < glslang::TOperator::EOpConstructGuardEnd) or
			 (aggr->getOp() > glslang::TOperator::EOpImageGuardBegin and aggr->getOp() < glslang::TOperator::EOpImageGuardEnd) or
			 (aggr->getOp() >= glslang::TOperator::EOpTextureGuardBegin and aggr->getOp() <= glslang::TOperator::EOpTextureGuardEnd) or
			 (aggr->getOp() >= glslang::TOperator::EOpAddCarry and aggr->getOp() <= glslang::TOperator::EOpFindMSB) )
		{
			CHECK_ERR( TranslateOperatorCall( node, uid, translator ) );
			return true;
		}

		TODO( "" );
		return true;
	}

/*
=================================================
	TranslateFunction
=================================================
*/
	static bool TranslateFunction (glslang::TIntermAggregate* aggr, const uint uid, Translator &translator)
	{
		ASSERT( aggr->getOp() == glslang::TOperator::EOpFunction );

		Translator::Node				dst_node;
		Array< Translator::Symbol >		func_args;
		const String					signature	= aggr->getName().c_str();
		const uint						body_uid	= ++translator.uid;
		const bool						is_entry	= IsProgramEntry( signature, translator );

		// get result type
		CHECK_ERR( ConvertType( aggr, aggr->getType(), null, translator, OUT dst_node.typeInfo ) );
		
		if ( translator.inl.functions.IsExist( signature ) )
			return true;

		// find arguments
		FOR( i, aggr->getSequence() )
		{
			TIntermNode* n = aggr->getSequence()[i];

			if ( n->getAsAggregate() and 
				 n->getAsAggregate()->getOp() == glslang::TOperator::EOpParameters )
			{
				// get arguemnts
				glslang::TIntermAggregate*	args_node = n->getAsAggregate();
				
				func_args.Resize( func_args.Count() + args_node->getSequence().size() );

				FOR( j, args_node->getSequence() )
				{
					Translator::Symbol &		arg		= func_args[j];
					glslang::TIntermSymbol *	symbol	= args_node->getSequence()[j]->getAsSymbolNode();

					CHECK_ERR( ConvertSymbol( symbol, symbol->getType(), null, translator, OUT arg ) );

					ASSERT( not translator.fwd.funcArguments.IsExist( GetSymbolID( symbol ) ) );
					translator.fwd.funcArguments.Add( GetSymbolID( symbol ), arg.name );
				}
			}
			else
			{
				ASSERT( not translator.states.isMain );
				translator.states.isMain = is_entry;
				translator.fwd.fnScope.PushBack( {} );
	
				CHECK_ERR( RecursiveProcessNode( n, body_uid, translator ) );
				
				translator.states.isMain = false;
				translator.fwd.fnScope.PopBack();
			}
		}

		StringCRef	body = translator.nodes( body_uid ).src;
		String		str;

		CHECK_ERR( not body.Empty() );
		CHECK_ERR( translator.language->TranslateFunctionDecl( signature, dst_node.typeInfo, func_args, INOUT str ) );

		if ( is_entry ) {
			CHECK_ERR( translator.language->TranslateEntry( dst_node.typeInfo, signature, func_args, body, OUT translator.dstEntryPoint, INOUT str ) );
			dst_node.src << str;
		}
		else {
			String	body_src = body;
			StringParser::IncreaceIndent( INOUT body_src );
			dst_node.src << str << "\n{\n" << body_src << "}\n\n";
		}

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	ConvertType
=================================================
*/
	static bool ConvertType (TIntermNode* node, glslang::TType const &type, const Translator::TypeInfo *parent,
							 const Translator &translator, OUT Translator::TypeInfo &result)
	{
		auto const&		qual = type.getQualifier();

		// copy typename
		if ( &type.getTypeName() != null )
		{
			result.typeName = type.getTypeName().c_str();
		}

		// arrays
		if ( type.isArray() )
		{
			if ( type.isExplicitlySizedArray() )
				result.arraySize.MakeStatic( type.getOuterArraySize() );
			else
				result.arraySize.MakeDynamic();
		}

		// buffer
		if ( type.getBasicType() == glslang::TBasicType::EbtBlock )
		{
			// do nothing
		}
		else
		if ( type.getBasicType() == glslang::TBasicType::EbtSampler and not type.isSubpass() )
		{
			glslang::TSampler const&	samp = type.getSampler();
			EShaderVariable::type		samp_type;

			switch ( samp.dim )
			{
				case glslang::TSamplerDim::Esd1D :
				{
					if ( samp.isShadow() and samp.isArrayed() )		samp_type = EShaderVariable::Sampler1DArrayShadow;	else
					if ( samp.isShadow() )							samp_type = EShaderVariable::Sampler1DShadow;		else
					if ( samp.isArrayed() )							samp_type = EShaderVariable::Sampler1DArray;		else
																	samp_type = EShaderVariable::Sampler1D;
					break;
				}
				case glslang::TSamplerDim::Esd2D :
				{
					if ( samp.isShadow() and samp.isArrayed() )			samp_type = EShaderVariable::Sampler2DArrayShadow;	else
					if ( samp.isShadow() )								samp_type = EShaderVariable::Sampler2DShadow;		else
					if ( samp.isMultiSample() and samp.isArrayed() )	samp_type = EShaderVariable::Sampler2DMSArray;		else
					if ( samp.isArrayed() )								samp_type = EShaderVariable::Sampler2DArray;		else
					if ( samp.isMultiSample() )							samp_type = EShaderVariable::Sampler2DMS;			else
																		samp_type = EShaderVariable::Sampler2D;
					break;
				}
				case glslang::TSamplerDim::Esd3D :
				{
					samp_type = EShaderVariable::Sampler3D;
					break;
				}
				case glslang::TSamplerDim::EsdCube :
				{
					if ( samp.isShadow() )		samp_type = EShaderVariable::SamplerCubeShadow;	else
					if ( samp.isArrayed() )		samp_type = EShaderVariable::SamplerCubeArray;	else
												samp_type = EShaderVariable::SamplerCube;
					break;
				}
				case glslang::TSamplerDim::EsdBuffer :
				{
					samp_type = EShaderVariable::SamplerBuffer;
					break;
				}
				default :
					RETURN_ERR( "unknown sampler dimension type!" );
			}
					
			// sampler
			if ( samp.isCombined() )
			{
				result.type = EShaderVariable::ToSampler( samp_type, ConvertBasicType( samp.type, samp.vectorSize ) );
			}
			else
			// image
			if ( samp.isImage() )
			{
				result.type   = EShaderVariable::ToImage( samp_type, ConvertBasicType( samp.type, samp.vectorSize ) );
				result.format = ConvertImageLayoutFormat( qual.layoutFormat );
			}
			else
				RETURN_ERR( "unsupported sampler type!" );
		}
		else
		// subpass
		if ( type.isSubpass() )
		{
			// TODO
		}
		else
		// for scalar, vector and matrix types only
		if ( type.getBasicType() == glslang::TBasicType::EbtVoid	or
			 type.getBasicType() == glslang::TBasicType::EbtBool	or
			 type.getBasicType() == glslang::TBasicType::EbtFloat	or
			 type.getBasicType() == glslang::TBasicType::EbtDouble	or
			 type.getBasicType() == glslang::TBasicType::EbtInt		or
			 type.getBasicType() == glslang::TBasicType::EbtUint	or
			 type.getBasicType() == glslang::TBasicType::EbtInt64	or
			 type.getBasicType() == glslang::TBasicType::EbtUint64	or
			#ifdef AMD_EXTENSIONS
			 type.getBasicType() == glslang::TBasicType::EbtFloat16	or
			 type.getBasicType() == glslang::TBasicType::EbtInt16	or
			 type.getBasicType() == glslang::TBasicType::EbtUint16
			#endif
			)
		{
			result.type = ConvertBasicType( type.getBasicType(), type.getVectorSize(), type.getMatrixCols(), type.getMatrixRows() );
		}

		// qualifiers
		{
			if ( type.isBuiltIn()					or
				 IsBuiltinName( result.typeName )	or
				 IsBuiltinName( result.name )		or
				(parent and parent->qualifier[ EVariableQualifier::BuiltIn ]) )
			{
				result.qualifier |= EVariableQualifier::BuiltIn;
			}

			if ( qual.invariant )
				result.qualifier |= EVariableQualifier::Invariant;

			if ( qual.noContraction )
				result.qualifier |= EVariableQualifier::Precise;

			if ( qual.centroid )
				result.qualifier |= EVariableQualifier::Centroid;

			if ( qual.smooth )
				result.qualifier |= EVariableQualifier::Smooth;

			if ( qual.flat )
				result.qualifier |= EVariableQualifier::Flat;

			if ( qual.nopersp )
				result.qualifier |= EVariableQualifier::NoPerspective;

			if ( qual.patch )
				result.qualifier |= EVariableQualifier::Patch;

			if ( qual.sample )
				result.qualifier |= EVariableQualifier::Sample;
			
			if ( qual.specConstant or qual.hasSpecConstantId() ) {
				result.qualifier  |= EVariableQualifier::Specialization;
			}
			
			CHECK_ERR( (qual.coherent + qual.volatil + qual.restrict + qual.readonly + qual.writeonly) <= 1 );

			if ( qual.coherent )
				result.memoryModel = EShaderMemoryModel::Coherent;

			if ( qual.volatil )
				result.memoryModel = EShaderMemoryModel::Volatile;

			if ( qual.restrict )
				result.memoryModel = EShaderMemoryModel::Restrict;

			if ( qual.readonly )
				result.memoryModel = EShaderMemoryModel::ReadOnly;

			if ( qual.writeonly )
				result.memoryModel = EShaderMemoryModel::WriteOnly;

			switch ( qual.storage )
			{
				case glslang::TStorageQualifier::EvqTemporary :			result.qualifier |= EVariableQualifier::Local;		break;
				//case glslang::TStorageQualifier::EvqGlobal :			break;
				case glslang::TStorageQualifier::EvqConst :				result.qualifier |= EVariableQualifier::Constant;	break;
				case glslang::TStorageQualifier::EvqVaryingIn :			result.qualifier |= EVariableQualifier::In;			result.isGlobal = true;		break;
				case glslang::TStorageQualifier::EvqVaryingOut :		result.qualifier |= EVariableQualifier::Out;		result.isGlobal = true;		break;
				case glslang::TStorageQualifier::EvqUniform :			result.qualifier |= EVariableQualifier::Uniform;	result.isGlobal = true;		break;
				case glslang::TStorageQualifier::EvqBuffer :			result.isGlobal = true;								break;
				case glslang::TStorageQualifier::EvqShared :			result.qualifier |= EVariableQualifier::Shared;		result.isGlobal = true;		break;

				// function arguments
				case glslang::TStorageQualifier::EvqIn :				result.qualifier |= EVariableQualifier::InArg;		break;
				case glslang::TStorageQualifier::EvqOut :				result.qualifier |= EVariableQualifier::OutArg;		break;
				case glslang::TStorageQualifier::EvqInOut :				result.qualifier |= EVariableQualifier::InArg;
																		result.qualifier |= EVariableQualifier::OutArg;		break;
				case glslang::TStorageQualifier::EvqConstReadOnly :
				{
					glslang::TIntermSymbol*	symb = node ? node->getAsSymbolNode() : null;	// TODO: move to ConvertSymbol
					
					if ( symb and translator.fwd.funcArguments.IsExist( GetSymbolID( symb ) ) )
						result.qualifier |= EVariableQualifier::InArg;
					else
						result.qualifier |= EVariableQualifier::Local;

					result.qualifier |= EVariableQualifier::Constant;
					break;
				}
			}

			switch ( qual.precision )
			{
				case glslang::TPrecisionQualifier::EpqNone :			result.precision = EPrecision::Default;	break;
				case glslang::TPrecisionQualifier::EpqLow :				result.precision = EPrecision::Low;		break;
				case glslang::TPrecisionQualifier::EpqMedium :			result.precision = EPrecision::Medium;	break;
				case glslang::TPrecisionQualifier::EpqHigh :			result.precision = EPrecision::High;	break;
				default :												RETURN_ERR( "unknown precision type!" );
			}
		}

		// copy from parent
		if ( parent )
		{
			result.qualifier.Or( EVariableQualifier::In, parent->qualifier[EVariableQualifier::In] );
			result.qualifier.Or( EVariableQualifier::Out, parent->qualifier[EVariableQualifier::Out] );
			result.qualifier.Or( EVariableQualifier::InArg, parent->qualifier[EVariableQualifier::InArg] );
			result.qualifier.Or( EVariableQualifier::OutArg, parent->qualifier[EVariableQualifier::OutArg] );
			result.qualifier.Or( EVariableQualifier::Shared, parent->qualifier[EVariableQualifier::Shared] );
			result.qualifier.Or( EVariableQualifier::BuiltIn, parent->qualifier[EVariableQualifier::BuiltIn] );
			result.qualifier.Or( EVariableQualifier::Local, parent->qualifier[EVariableQualifier::Local] );
			
			CHECK_ERR( not parent->qualifier[EVariableQualifier::Specialization] );

			//if ( result.memoryModel == EShaderMemoryModel::Default )
			//	result.memoryModel = parent->memoryModel;

			if ( result.precision == EPrecision::Default )
				result.precision = parent->precision;
		}
		
		// copy name
		if ( &type.getFieldName() != null )
		{
			result.name = type.getFieldName().c_str();

			if ( not type.isBuiltIn() ) {
				CHECK_ERR( translator.language->ValidateFieldName( INOUT result ) );
			}
		}

		if ( type.isStruct() )
		{
			auto const&	type_list = *type.getStruct();
			
			result.typeName = type.getTypeName().c_str();
			result.type		= type.getBasicType() == glslang::TBasicType::EbtBlock ?
									(qual.storage == glslang::TStorageQualifier::EvqBuffer ? EShaderVariable::StorageBlock :
									 qual.storage == glslang::TStorageQualifier::EvqUniform ? EShaderVariable::UniformBlock :
									 qual.storage == glslang::TStorageQualifier::EvqVaryingIn ? EShaderVariable::VaryingsBlock :
									 qual.storage == glslang::TStorageQualifier::EvqVaryingOut ? EShaderVariable::VaryingsBlock : EShaderVariable::Unknown) :
									EShaderVariable::Struct;

			result.fields.Reserve( result.fields.Count() + type_list.size() );

			FOR( i, type_list )
			{
				Translator::TypeInfo	arg;
				CHECK_ERR( ConvertType( null, *type_list[i].type, &result, translator, OUT arg ) );
				result.fields.PushBack( RVREF(arg) );
			}
		}

		if ( not (type.isSubpass() or type.getSampler().isCombined() or type.getSampler().isImage()) )
		{
			Translator::CustomTypes_t::const_iterator	iter;

			if ( translator.types.globalTypes.Find( result.typeName, OUT iter ) ) {
				result.sizeOf = iter->second.sizeOf;
			} else {
				result.sizeOf = GetElementSize( type );
			}
		}
		return true;
	}
	
/*
=================================================
	ConvertSymbol
=================================================
*/
	static bool ConvertSymbol (TIntermNode* node, glslang::TType const &type, const Translator::TypeInfo *parent,
								const Translator &translator, OUT Translator::Symbol &result)
	{
		CHECK_ERR( ConvertType( node, type, parent, translator, OUT result ) );
		CHECK_ERR( node );

		auto const&		qual = type.getQualifier();

		// binding
		result.binding = qual.hasBinding() ? uint(qual.layoutBinding) : UMax;

		// specialization
		if ( result.qualifier[ EVariableQualifier::Specialization ] )
			result.specConstID = qual.layoutSpecConstantId;
		
		// symbol id
		glslang::TIntermSymbol*	symb = node->getAsSymbolNode();
		CHECK_ERR( symb );

		result.id = Translator::SymbolID( symb->getId() );
		
		// symbol name
		result.name = symb->getName().c_str();

		if ( result.name.StartsWithIC( "anon@" ) )
			result.name.Clear();
		
		CHECK_ERR( translator.language->ValidateSymbolName( INOUT result, 0 ) );
		return true;
	}

/*
=================================================
	IsBuiltinName
=================================================
*/
	static bool IsBuiltinName (StringCRef name)
	{
		return name.StartsWithIC( "gl_" ) or name.StartsWithIC( "gles_" );
	}

/*
=================================================
	RecursiveExtractTypesFromExternal
=================================================
*/
	static void RecursiveExtractTypesFromExternal (Translator &translator, const Translator::TypeInfo &info)
	{
		// search for struct types
		for (auto& field : info.fields)
		{
			if ( EShaderVariable::IsStruct( field.type ) and
				 not translator.types.definedInExteranal.IsExist( field.typeName ) )
			{
				translator.types.globalTypes.Add( field.typeName, field );
				translator.types.definedInExteranal.Add( field.typeName );

				RecursiveExtractTypesFromExternal( translator, field );
			}
		}
	}
	
/*
=================================================
	ApplyAtomicTypes
=================================================
*/
	static bool ApplyAtomicTypes (Translator &translator, INOUT Translator::TypeInfo &info)
	{
		auto&	atomics	= translator.types.atomics;
		usize	idx		= UMax;

		if ( not atomics.FindFirstIndex( info.typeName, OUT idx ) )
			return true;

		for (; idx < atomics.Count() and atomics[idx].first == info.typeName; ++idx)
		{
			Translator::TypeInfo *	curr			= &info;
			const auto &			atomic_fields	= atomics[idx].second;
			
			for (auto& fld_name : atomic_fields)
			{
				bool	found = false;

				FOR( i, curr->fields )
				{
					if ( curr->fields[i].name == fld_name )
					{
						curr  = &curr->fields[i];
						found = true;
						break;
					}
				}

				CHECK_ERR( found );
			}

			CHECK_ERR( EShaderVariable::IsInt( curr->type ) );

			curr->qualifier |= EVariableQualifier::Volatile;
		}
		return true;
	}

/*
=================================================
	TranslateExternalObjects
=================================================
*/
	static bool TranslateExternalObjects (glslang::TIntermAggregate* aggr, const uint uid, Translator &translator)
	{
		CHECK_ERR( aggr and aggr->getOp() == glslang::TOperator::EOpLinkerObjects );

		Translator::Node	dst_node;
		String &			str = dst_node.src;

		dst_node.uid = uid;

		FOR( i, aggr->getSequence() )
		{
			TIntermNode*	node = aggr->getSequence()[i];
			CHECK_ERR( node->getAsTyped() );

			glslang::TIntermTyped *		typed	= node->getAsTyped();
			glslang::TType const &		type	= typed->getType();
			Translator::Symbol			info;

			// skip shared
			if ( type.getQualifier().storage == glslang::TStorageQualifier::EvqShared )
				continue;

			CHECK_ERR( ConvertSymbol( node, type, null, translator, OUT info ) );
			
			// skip builtin
			if ( type.isBuiltIn() or IsBuiltinName( info.typeName ) ) {
				continue;
			}

			CHECK_ERR( GXCheckExternalQualifiers( translator, type.getQualifier() ) );
			
			CHECK_ERR( ApplyAtomicTypes( translator, info ) );
			
			translator.types.definedInExteranal.Add( info.typeName );
			RecursiveExtractTypesFromExternal( translator, info );

			if ( EShaderVariable::IsStruct( info.type ) )
			{
				if ( translator.language->DeclExternalTypes() )
					translator.types.globalTypes.Add( info.typeName, info );
				else
					translator.types.globalTypes.Erase( info.typeName );
			}

			// skip global variables
			if ( type.getQualifier().storage == glslang::TStorageQualifier::EvqConst or
				 type.getQualifier().storage == glslang::TStorageQualifier::EvqGlobal )
				continue;

			CHECK_ERR( translator.language->TranslateExternal( typed, info, OUT str ) );
		}

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	TranslateSharedObjects
=================================================
*/
	static bool TranslateSharedObjects (glslang::TIntermAggregate* aggr, const uint uid, Translator &translator)
	{
		CHECK_ERR( aggr and aggr->getOp() == glslang::TOperator::EOpLinkerObjects );

		Translator::Node	dst_node;
		String &			str = dst_node.src;

		dst_node.uid = uid;

		FOR( i, aggr->getSequence() )
		{
			TIntermNode*	node = aggr->getSequence()[i];
			CHECK_ERR( node->getAsTyped() );

			glslang::TIntermTyped *		typed	= node->getAsTyped();
			glslang::TType const &		type	= typed->getType();
			Translator::Symbol			info;

			// keep shared only
			if ( type.getQualifier().storage != glslang::TStorageQualifier::EvqShared )
				continue;

			CHECK_ERR( ConvertSymbol( node, type, null, translator, OUT info ) );
			
			// skip builtin
			if ( type.isBuiltIn() or IsBuiltinName( info.typeName ) ) {
				continue;
			}

			CHECK_ERR( GXCheckExternalQualifiers( translator, type.getQualifier() ) );
			
			translator.types.definedInExteranal.Add( info.typeName );
			RecursiveExtractTypesFromExternal( translator, info );
			
			if ( EShaderVariable::IsStruct( info.type ) )
			{
				if ( translator.language->DeclExternalTypes() )
					translator.types.globalTypes.Add( info.typeName, info );
				else
					translator.types.globalTypes.Erase( info.typeName );
			}

			CHECK_ERR( translator.language->TranslateExternal( typed, info, OUT str ) );
		}

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}

/*
=================================================
	TranslateGlobals
=================================================
*/
	static bool TranslateGlobals (glslang::TIntermAggregate* aggr, const uint uid, Translator &translator)
	{
		CHECK_ERR( aggr and aggr->getOp() == glslang::TOperator::EOpLinkerObjects );

		Translator::Node	dst_node;
		String &			str = dst_node.src;

		dst_node.uid = uid;

		FOR( i, aggr->getSequence() )
		{
			TIntermNode*	node = aggr->getSequence()[i];
			CHECK_ERR( node->getAsTyped() );

			glslang::TIntermTyped*		typed	= node->getAsTyped();
			glslang::TType const&		type	= typed->getType();
			Translator::Symbol			info;

			CHECK_ERR( ConvertSymbol( node, type, null, translator, OUT info ) );
			
			// skip builtin
			if ( type.isBuiltIn() or IsBuiltinName( info.typeName ) ) {
				continue;
			}
			
			// keep global variables only
			if ( type.getQualifier().storage != glslang::TStorageQualifier::EvqConst and
				 type.getQualifier().storage != glslang::TStorageQualifier::EvqGlobal )
				continue;
			
			auto*	symb_node	= node->getAsSymbolNode();
			auto*	cu_arr		= &symb_node->getConstArray();
			String	name		= symb_node->getName().c_str();

			CHECK_ERR( not translator.constants.uniqueNames.IsExist( name ) );

			translator.constants.uniqueNames.Add( name );
			translator.constants.symbNodes.Add( Translator::ConstKey{ cu_arr, info }, Translator::ConstName{ name, symb_node } );

			CHECK_ERR( translator.language->TranslateExternal( typed, info, OUT str ) );
		}

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}

/*
=================================================
	RecursiveProcessNode
=================================================
*/
	static bool RecursiveProcessNode (TIntermNode* node, const uint uid, Translator &translator)
	{
		if ( not node )
			return false;

		translator.nodeStack.PushBack( node );
		
		if ( translator.language->ReplaceStructByBuffer() and translator.types.bufferNodes.IsExist( node ) )
		{
			CHECK_ERR( TranslateLoadStoreOp( node, uid, translator ) );
		}
		else
		if ( node->getAsAggregate() )
		{
			CHECK_ERR( RecursiveProcessAggregateNode( node, uid, translator ) );
		}
		else
		if ( node->getAsUnaryNode() )
		{
			CHECK_ERR( RecursiveProcessUnaryNode( node, uid, translator ) );
		}
		else
		if ( node->getAsBinaryNode() )
		{
			CHECK_ERR( RecursiveProcessBinaryNode( node, uid, translator ) );
		}
		else
		if ( node->getAsOperator() )
		{
			CHECK_ERR( RecursiveProcessOperatorNode( node, uid, translator ) );
		}
		else
		if ( node->getAsBranchNode() )
		{
			CHECK_ERR( RecursiveProcessBranchNode( node, uid, translator ) );
		}
		else
		if ( node->getAsSwitchNode() )
		{
			CHECK_ERR( RecursiveProcessSwitchNode( node, uid, translator ) );
		}
		else
		if ( node->getAsConstantUnion() )
		{
			CHECK_ERR( RecursiveProcessConstUnionNode( node, uid, translator ) );
		}
		else
		if ( node->getAsSelectionNode() )
		{
			CHECK_ERR( RecursiveProcessSelectionNode( node, uid, translator ) );
		}
		else
		if ( node->getAsMethodNode() )
		{
			CHECK_ERR( RecursiveProcessMethodNode( node, uid, translator ) );
		}
		else
		if ( node->getAsSymbolNode() )
		{
			CHECK_ERR( RecursiveProcessSymbolNode( node, uid, translator ) );
		}
		else
		//if ( node->getAsTyped() )
		//{
		//	CHECK_ERR( RecursiveProcessTypedNode( node, uid, translator ) );
		//}
		//else
		if ( node->getAsLoopNode() )
		{
			CHECK_ERR( RecursiveProcessLoopNode( node, uid, translator ) );
		}
		else
		{
			TODO( "" );
			return false;
		}

		translator.nodeStack.PopBack();
		return true;
	}
	
/*
=================================================
	RecursiveProcessBranchNode
=================================================
*/
	static bool RecursiveProcessBranchNode (TIntermNode* node, const uint uid, Translator &translator)
	{
		glslang::TIntermBranch*		branch			= node->getAsBranchNode();
		Translator::Node			dst_node;		dst_node.uid = uid;
		bool						replaced_return = false;
		const String				prefix			= translator.inl.prefixStack.Get().prefix;

		switch ( branch->getFlowOp() )
		{
			case glslang::TOperator::EOpKill :		dst_node.src << "discard";	break;
			case glslang::TOperator::EOpBreak :		dst_node.src << "break";	break;
			case glslang::TOperator::EOpContinue :	dst_node.src << "continue";	break;
			case glslang::TOperator::EOpReturn :
			{
				if ( prefix.Empty() )
					dst_node.src << "return";
				else
					replaced_return = true;
				break;
			}
			default :								RETURN_ERR( "unknown operator!" );
		}

		if ( branch->getExpression() )
		{
			const uint	arg_uid = ++translator.uid;
			CHECK_ERR( RecursiveProcessNode( branch->getExpression(), arg_uid, translator ) );

			const auto&	arg = translator.nodes( arg_uid );
			CHECK_ERR( not arg.src.Empty() );

			dst_node.src << " " << arg.src;

			// inside inline function 'return' must be replaced by exit of cycle
			if ( replaced_return ) {
				("{\n\t"_str << prefix << "return =") >> dst_node.src;
				dst_node.src << ";\n\t" << prefix << "exit = true;\n\tbreak;\n}";
			}
		}
		else
		// function return void, but in inline function need to break loop
		if ( replaced_return )
		{
			dst_node.src << prefix << "exit = true;\nbreak";
		}

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	RecursiveProcessSwitchNode
=================================================
*/
	static bool RecursiveProcessSwitchNode (TIntermNode* node, const uint uid, Translator &translator)
	{
		glslang::TIntermSwitch*		sw = node->getAsSwitchNode();

		const uint	cond_uid = ++translator.uid;
		CHECK_ERR( RecursiveProcessNode( sw->getCondition(), cond_uid, translator ) );
		
		// body
		String	body_src;
		{
			glslang::TIntermAggregate *	aggr = sw->getBody()->getAsAggregate();
			Translator::Node			dst_node;

			CHECK_ERR( aggr and aggr->getOp() == glslang::TOperator::EOpSequence );
			
			translator.nodeStack.PushBack( aggr );
			
			FOR( i, aggr->getSequence() )
			{
				if ( aggr->getSequence()[i]->getAsBranchNode() )
				{
					glslang::TIntermBranch *	branch	= aggr->getSequence()[i]->getAsBranchNode();

					dst_node.uid = ++translator.uid;

					switch ( branch->getFlowOp() )
					{
						case glslang::TOperator::EOpCase : {
							dst_node.src << "case";
							break;
						}

						case glslang::TOperator::EOpDefault : {
							dst_node.src << "default : {\n";
							translator.fwd.scope.Back().reqEndLine = false;
							break;
						}

						default :
							RETURN_ERR( "unknown operator!" );
					}

					if ( branch->getExpression() )
					{
						const uint	arg_uid = ++translator.uid;
						CHECK_ERR( RecursiveProcessNode( branch->getExpression(), arg_uid, translator ) );

						const auto&	arg = translator.nodes( arg_uid );
						CHECK_ERR( not arg.src.Empty() );

						dst_node.src << " " << arg.src;
			
						if ( branch->getFlowOp() == glslang::TOperator::EOpCase )
						{
							dst_node.src << ": {\n";
							translator.fwd.scope.Back().reqEndLine = false;
						}
					}
				}
				else
				{
					const uint	body_uid = ++translator.uid;
					CHECK_ERR( RecursiveProcessNode( aggr->getSequence()[i], body_uid, translator ) );
					
					String		body = translator.nodes( body_uid ).src;
					CHECK_ERR( not body.Empty() );
					
					StringParser::IncreaceIndent( INOUT body );

					dst_node.src << body << "}\n";
					body_src << dst_node.src;

					translator.nodes.Add( dst_node.uid, RVREF(dst_node) );

					dst_node = Translator::Node();
				}
			}

			translator.nodeStack.PopBack();
		}
		
		Translator::Node	dst_node;

		const auto&	cond = translator.nodes( cond_uid );

		CHECK_ERR( not cond.src.Empty() );
		CHECK_ERR( not body_src.Empty() );

		dst_node.uid = uid;
		dst_node.src << "switch( " << cond.src << " )\n{\n" << body_src << "}\n";

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}

/*
=================================================
	RecursiveProcessConstUnionNode
=================================================
*/
	static bool RecursiveProcessConstUnionNode (TIntermNode* node, const uint uid, Translator &translator)
	{
		glslang::TIntermConstantUnion*		cu		= node->getAsConstantUnion();
		const glslang::TConstUnionArray&	cu_arr	= cu->getConstArray();
		Translator::Node					dst_node;
		
		dst_node.uid = uid;
		CHECK_ERR( ConvertType( node, cu->getType(), null, translator, OUT dst_node.typeInfo ) );
		CHECK_COMP2( GXCheckAccessToExternal( translator, dst_node ) );
		
		// find in constants
		bool						found = false;
		const Translator::ConstKey	cu_key { &cu_arr, dst_node.typeInfo };

		for (const auto& cached : translator.constants.symbNodes)
		{
			if ( cu_key == cached.first )
			{
				dst_node.src << cached.second.name;
				found = true;
				break;
			}
		}

		// create constant
		if ( not found )
		{
			if ( translator.constants.optimize or
				 dst_node.typeInfo.arraySize.IsArray() )
			{
				String				name;
				String&				src			= translator.constants.source;
				String const		type_name	= (dst_node.typeInfo.typeName.Empty() ? ToStringGLSL( dst_node.typeInfo.type ) : StringCRef(dst_node.typeInfo.typeName));
				Translator::Symbol	symb		{ dst_node.typeInfo };

				// choose unique name
				for (uint i = 0; i < 1000; ++i)
				{
					name << "C_" << type_name << "_" << i;

					if ( not translator.constants.uniqueNames.IsExist( name ) )
						break;
				
					name.Clear();
				}
			
				CHECK_ERR( not name.Empty() );
				translator.constants.uniqueNames.Add( name );
				translator.constants.symbNodes.Add( cu_key, { name, null } );

				symb.name					= name;
				symb.isGlobal				= true;	// global const
				dst_node.typeInfo.isGlobal	= true;

				CHECK_ERR( translator.language->TranslateLocalVar( symb, INOUT src ) );

				src << " = ";
				CHECK_ERR( translator.language->TranslateConstant( cu_arr, dst_node.typeInfo, INOUT src ) );
				src << ";\n";

				// copy
				dst_node.src << name;
			}
			else
			{
				CHECK_ERR( translator.language->TranslateConstant( cu_arr, dst_node.typeInfo, INOUT dst_node.src ) );
			}
		}

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}

/*
=================================================
	RecursiveProcessSelectionNode
=================================================
*/
	static bool RecursiveProcessSelectionNode (TIntermNode* node, const uint uid, Translator &translator)
	{
		glslang::TIntermSelection*	selection = node->getAsSelectionNode();
		Translator::Node			dst_node;		dst_node.uid = uid;
		
		CHECK_ERR( ConvertType( selection, selection->getType(), null, translator, OUT dst_node.typeInfo ) );

		const bool	is_if_else_block = (dst_node.typeInfo.type == EShaderVariable::Void);

		String	cond_src;
		if ( selection->getCondition() )
		{
			const uint	cond_uid = ++translator.uid;
			CHECK_ERR( RecursiveProcessNode( selection->getCondition(), cond_uid, translator ) );
			cond_src = translator.nodes( cond_uid ).src;
		}

		String	true_src;
		if ( selection->getTrueBlock() )
		{
			const uint	true_uid = ++translator.uid;
			CHECK_ERR( RecursiveProcessNode( selection->getTrueBlock(), true_uid, translator ) );
			true_src = translator.nodes( true_uid ).src;
		}

		String	false_src;
		if ( selection->getFalseBlock() )
		{
			const uint	false_uid = ++translator.uid;
			CHECK_ERR( RecursiveProcessNode( selection->getFalseBlock(), false_uid, translator ) );
			false_src = translator.nodes( false_uid ).src;
		}

		if ( is_if_else_block )
		{
			CHECK_ERR( not cond_src.Empty() and (not true_src.Empty() or not false_src.Empty()) );

			dst_node.src << "if (" << cond_src << ")\n";
			
			if ( not true_src.Empty() )
			{
				StringParser::IncreaceIndent( true_src );
				dst_node.src << "{\n" << true_src << ";\n}\n";
			}
			else
				dst_node.src << "{}\n";
			
			if ( not false_src.Empty() )
			{
				StringParser::IncreaceIndent( false_src );
				dst_node.src << "else\n{\n" << false_src << ";\n}\n";
			}
		}
		else
		{
			CHECK_ERR( not cond_src.Empty() and not true_src.Empty() and not false_src.Empty() );

			dst_node.src << "( (" << cond_src << ") ? (" << true_src << ") : (" << false_src << ") )";
		}

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	RecursiveProcessMethodNode
=================================================
*/
	static bool RecursiveProcessMethodNode (TIntermNode* node, const uint, Translator &translator)
	{
		glslang::TIntermMethod*		method = node->getAsMethodNode();

		CHECK_ERR( RecursiveProcessNode( method->getObject(), ++translator.uid, translator ) );
		
		TODO( "" );
		return true;
	}
	
/*
=================================================
	RecursiveProcessSymbolNode
=================================================
*/
	static bool RecursiveProcessSymbolNode (TIntermNode* node, const uint uid, Translator &translator)
	{
		TIntermNode*				root	= translator.nodeStack.Count() > 2 ? translator.nodeStack[ translator.nodeStack.Count()-2 ] : null;
		glslang::TIntermSymbol*		symbol	= node->getAsSymbolNode();
		Translator::Node			dst_node;
		Translator::Symbol			info;
		const String				prefix	= translator.inl.prefixStack.Get().prefix;

		CHECK_ERR( ConvertSymbol( symbol, symbol->getType(), null, translator, OUT info ) );

		// if inside inline function
		if ( info.qualifier[ EVariableQualifier::InArg ] or
			 info.qualifier[ EVariableQualifier::OutArg ] )
		{
			prefix >> info.name;
			
			// replace some function arguments to external objects to support GLSL specification.
			Translator::LocalReplacer_t::iterator	iter;
			if ( translator.inl.localsReplacer.Find( GetSymbolID( symbol ), OUT iter ) ) {
				info.name		= iter->second;
				info.qualifier &= ~(EVariableQualifier::InArg | EVariableQualifier::OutArg);
			}
		}

		dst_node.uid = uid;
		
		// local variable or const
		if ( info.qualifier[ EVariableQualifier::Local ] )
		{
			Translator::LocalVarSet_t::const_iterator	local_iter;
			
			if ( translator.fwd.fnScope.Back().Find( GetSymbolID( symbol ), OUT local_iter ) )
			{
				// reuse
				dst_node.src = local_iter->second;
			}
			else
			// new variable
			{
				prefix >> info.name;
				translator.fwd.fnScope.Back().Add( GetSymbolID( symbol ), info.name );
				translator.inl.prefixStack.Get().localVars.Add( GetSymbolID( symbol ) );

				// pattern: <type> <name> = ...;
				if ( root and root->getAsOperator() and root->getAsOperator()->getOp() == glslang::TOperator::EOpAssign )
				{
					CHECK_ERR( translator.language->TranslateLocalVar( info, OUT dst_node.src ) );
				}
				else
				{
					translator.fwd.scope.Back().pendingVars.Add( GetSymbolID( symbol ), info );
					dst_node.src = info.name;
				}
			}
		}
		else
		{
			// builtin variable/const may be replaced by target language implementation
			CHECK_ERR( translator.language->TranslateName( info, OUT dst_node.src ) );
		}

		dst_node.typeInfo = info;
		
		CHECK_COMP2( GXCheckAccessToExternal( translator, dst_node ) );
		CHECK_COMP2( GXCheckBuiltinAccess( translator, dst_node ) );

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	RecursiveProcessTypedNode
=================================================
*
	static bool RecursiveProcessTypedNode (TIntermNode*, const uint, Translator &)
	{
		//glslang::TIntermTyped*		typed = node->getAsTyped();

		TODO( "" );
		return true;
	}
	
/*
=================================================
	RecursiveProcessOperatorNode
=================================================
*/
	static bool RecursiveProcessOperatorNode (TIntermNode*, const uint, Translator &)
	{
		//glslang::TIntermOperator*	op = node->getAsOperator();
		
		TODO( "" );
		return true;
	}
	
/*
=================================================
	RecursiveProcessUnaryNode
=================================================
*/
	static bool RecursiveProcessUnaryNode (TIntermNode* node, const uint uid, Translator &translator)
	{
		glslang::TIntermUnary*		unary	= node->getAsUnaryNode();
		const uint					arg_uid	= ++translator.uid;
		Translator::Node			dst_node;	dst_node.uid = uid;	

		CHECK_ERR( ConvertType( node, unary->getType(), null, translator, OUT dst_node.typeInfo ) );
		CHECK_COMP2( GXCheckAccessToExternal( translator, dst_node ) );

		CHECK_ERR( RecursiveProcessNode( unary->getOperand(), arg_uid, translator ) );

		auto	arg_node = translator.nodes( arg_uid );
		CHECK_ERR( not arg_node.src.Empty() );

		CHECK_ERR( translator.language->TranslateOperator( unary->getOp(), dst_node.typeInfo, { arg_node.src }, { &arg_node.typeInfo }, OUT dst_node.src ) );

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	RecursiveProcessBinaryNode
=================================================
*/
	static bool RecursiveProcessBinaryNode (TIntermNode* node, const uint uid, Translator &translator)
	{
		glslang::TIntermBinary*		binary = node->getAsBinaryNode();

		if ( binary->getOp() == glslang::TOperator::EOpVectorSwizzle or
			(binary->getOp() == glslang::TOperator::EOpIndexDirect and not binary->getLeft()->isArray() and
			(binary->getLeft()->isScalar() or binary->getLeft()->isVector())) )
		{
			CHECK_ERR( TranslateVectorSwizzle( binary, uid, translator ) );
			return true;
		}

		if ( binary->getOp() == glslang::TOperator::EOpIndexDirectStruct )
		{
			CHECK_ERR( TranslateIndexDirectStruct( binary, uid, translator ) );
			return true;
		}

		if ( binary->getLeft()->getAsConstantUnion() and
			(binary->getOp() == glslang::TOperator::EOpIndexDirect or binary->getOp() == glslang::TOperator::EOpIndexIndirect) )
		{
			CHECK_ERR( TranslateConstUnionAccess( node, uid, translator ) );
			return true;
		}

		const uint	lhs_uid	= ++translator.uid;
		const uint	rhs_uid	= ++translator.uid;

		CHECK_ERR( RecursiveProcessNode( binary->getLeft(), lhs_uid, translator ) );
		CHECK_ERR( RecursiveProcessNode( binary->getRight(), rhs_uid, translator ) );

		auto	lhs = translator.nodes( lhs_uid );
		auto	rhs = translator.nodes( rhs_uid );

		Translator::Node	dst_node;
		dst_node.uid = uid;

		CHECK_ERR( ConvertType( node, binary->getType(), null, translator, OUT dst_node.typeInfo ) );
		CHECK_ERR( not lhs.src.Empty() and not rhs.src.Empty() );
		
		CHECK_COMP2( GXCheckAccessToExternal( translator, dst_node ) );

		CHECK_ERR( translator.language->TranslateOperator( binary->getOp(), dst_node.typeInfo, { lhs.src, rhs.src },
														   { &lhs.typeInfo, &rhs.typeInfo }, INOUT dst_node.src ) );

		if ( binary->getOp() == glslang::TOperator::EOpIndexIndirect )
			dst_node.typeInfo.isGlobal |= lhs.typeInfo.isGlobal;

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	RecursiveProcessLoopNode
=================================================
*/
	static bool RecursiveProcessLoopNode (TIntermNode* node, const uint uid, Translator &translator)
	{
		glslang::TIntermLoop*	loop = node->getAsLoopNode();
		String					variables_src;

		// initialize variables in 'for' loop
		if ( not loop and node->getAsAggregate() )
		{
			glslang::TIntermAggregate*	aggr = node->getAsAggregate();

			CHECK_ERR( aggr->getSequence().size() == 2 );

			loop = aggr->getSequence()[1]->getAsLoopNode();
			aggr = aggr->getSequence()[0]->getAsAggregate();
			CHECK_ERR( loop and aggr );
			
			Translator::Symbol	var_type;

			FOR( i, aggr->getSequence() )
			{
				if ( glslang::TIntermBinary* binary = aggr->getSequence()[i]->getAsBinaryNode() )
				{
					Translator::Symbol	symb_type;
					CHECK_ERR( ConvertSymbol( binary->getLeft(), binary->getLeft()->getType(), null, translator, OUT symb_type ) );

					if ( i == 0 )
					{
						var_type = symb_type;

						CHECK_ERR( translator.language->TranslateType( var_type, OUT variables_src ) );
						CHECK_ERR( var_type.qualifier[ EVariableQualifier::Local ] and EShaderVariable::IsScalar( var_type.type ) );
					}
					else
					{
						CHECK_ERR( var_type.type == symb_type.type );
					}

					CHECK_ERR( binary->getOp() == glslang::TOperator::EOpAssign );

					translator.fwd.fnScope.Back().Add( symb_type.id, symb_type.name );
					translator.inl.prefixStack.Get().localVars.Add( symb_type.id );

					if ( i > 0 )
						variables_src << ',';

					variables_src << ' ';

					const uint	rhs_uid = ++translator.uid;
					CHECK_ERR( RecursiveProcessNode( binary->getRight(), rhs_uid, translator ) );

					const auto&	rhs_node = translator.nodes( rhs_uid );
					CHECK_ERR( not rhs_node.src.Empty() );

					CHECK_ERR( translator.language->TranslateOperator( glslang::TOperator::EOpAssign, var_type, { symb_type.name, rhs_node.src },
																	   { &symb_type, &rhs_node.typeInfo }, INOUT variables_src ) );
				}
				else
				{
					RETURN_ERR( "unknown node type" );
				}
			}
		}

		String				loop_src;
		String				test_src;
		String				terminal_src;
		Translator::Node	dst_node;	dst_node.uid = uid;

		// loop body
		if ( loop->getBody() )
		{
			const uint	body_uid = ++translator.uid;
			CHECK_ERR( RecursiveProcessNode( loop->getBody(), body_uid, translator ) );

			loop_src = translator.nodes( body_uid ).src;
			CHECK_ERR( not loop_src.Empty() );
		}

		// loop exit condition
		if ( loop->getTest() )
		{
			const uint	test_uid = ++translator.uid;
			CHECK_ERR( RecursiveProcessNode( loop->getTest(), test_uid, translator ) );

			test_src = translator.nodes( test_uid ).src;
			CHECK_ERR( not test_src.Empty() );
		}

		// loop ...
		if ( loop->getTerminal() )
		{
			const uint	terminal_uid = ++translator.uid;
			CHECK_ERR( RecursiveProcessNode( loop->getTerminal(), terminal_uid, translator ) );

			terminal_src = translator.nodes( terminal_uid ).src;
			CHECK_ERR( not terminal_src.Empty() );
		}
		
		auto &	prefix_stack = translator.inl.prefixStack.Get().prefix;

		if ( not prefix_stack.Empty() )
		{
			test_src = "(!"_str << prefix_stack << "exit && " << test_src << ')';
		}

		StringParser::IncreaceIndent( loop_src );

		if ( loop->testFirst() )
		{
			// 'for'
			if ( not terminal_src.Empty() )
			{
				loop_src = "for ("_str << variables_src << "; " << test_src << "; " << terminal_src << ")\n{\n" << loop_src << "}\n";
			}
			else
			// 'while'
			{
				CHECK_ERR( variables_src.Empty() );
				loop_src = "while ( "_str << test_src << ")\n{\n" << loop_src << "};\n";
			}
		}
		else
		{
			// 'do-while'
			CHECK_ERR( terminal_src.Empty() and variables_src.Empty() );

			loop_src = "do {\n"_str << loop_src << "} while ( " << test_src << ");\n";
		}

		// check exit condition after loop
		if ( not prefix_stack.Empty() )
		{
			loop_src << "if (" << prefix_stack << "exit) break;\n";
		}

		dst_node.src = RVREF(loop_src);

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}

/*
=================================================
	TranslateVectorSwizzle
=================================================
*/
	static bool TranslateVectorSwizzle (glslang::TIntermOperator* node, const uint uid, Translator &translator)
	{
		glslang::TIntermBinary*			binary	= node->getAsBinaryNode();
		Translator::Node				dst_node;		dst_node.uid = uid;
		Array<uint>						sw_mask;
		Array<glslang::TIntermBinary*>	swizzle_op;		swizzle_op << binary;

		CHECK_ERR( binary and (binary->getOp() == glslang::TOperator::EOpVectorSwizzle or binary->getOp() == glslang::TOperator::EOpIndexDirect) );
		
		CHECK_ERR( ConvertType( node, binary->getType(), null, translator, OUT dst_node.typeInfo ) );

		// extract swizzle mask
		for (glslang::TIntermTyped* n = binary->getLeft();
			 n->getAsBinaryNode() and n->getAsBinaryNode()->getOp() == glslang::TOperator::EOpVectorSwizzle;)
		{
			swizzle_op << n->getAsBinaryNode();

			n = swizzle_op.Back()->getLeft();
		}

		binary = swizzle_op.Back();

		const uint	lhs_uid	= ++translator.uid;
		CHECK_ERR( RecursiveProcessNode( binary->getLeft(), lhs_uid, translator ) );
		
		const auto&	lhs = translator.nodes( lhs_uid );
		CHECK_ERR( not lhs.src.Empty() );

		const auto ProcessUnion = LAMBDA(&sw_mask) (TIntermNode *n, ArrayCRef<uint> mask) -> bool
		{{
			glslang::TIntermConstantUnion*		cu		= n->getAsConstantUnion();
			glslang::TConstUnionArray const&	cu_arr	= cu->getConstArray();

			CHECK_ERR( cu_arr.size() == 1 and cu->getType().getBasicType() == glslang::EbtInt );
			CHECK_ERR( cu_arr[0].getType() == glslang::EbtInt and cu_arr[0].getIConst() >= 0 and cu_arr[0].getIConst() < 4 );

			if ( mask.Empty() )
				sw_mask << cu_arr[0].getIConst();
			else
				sw_mask << mask[ cu_arr[0].getIConst() ];
			return true;
		}};

		// optimize swizzle
		FOR_rev( i, swizzle_op )
		{
			glslang::TIntermBinary*	bin		= swizzle_op[i];
			const Array<uint>		mask	= sw_mask;

			sw_mask.Clear();

			if ( bin->getRight()->getAsAggregate() )
			{
				glslang::TIntermAggregate*	aggr = bin->getRight()->getAsAggregate();
				CHECK_ERR( aggr->getOp() == glslang::TOperator::EOpSequence );

				FOR( j, aggr->getSequence() )
				{
					CHECK_ERR( ProcessUnion( aggr->getSequence()[j], mask ) );
				}
			}
			else
			if ( bin->getRight()->getAsConstantUnion() )
			{
				CHECK_ERR( ProcessUnion( bin->getRight(), mask ) );
			}
			else
				RETURN_ERR( "not supported!" );
		}

		CHECK_ERR( translator.language->TranslateSwizzle( dst_node.typeInfo, lhs.typeInfo, lhs.src, sw_mask, INOUT dst_node.src ) );
		
		dst_node.typeInfo.isGlobal |= lhs.typeInfo.isGlobal;

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	GetFieldType
=================================================
*/
	static bool GetFieldType (glslang::TIntermConstantUnion* cu, const glslang::TTypeList &stTypeList, OUT glslang::TType* &field)
	{
		glslang::TConstUnionArray const&	cu_arr	= cu->getConstArray();
		
		// constant union must be correct index of struct field
		CHECK_ERR( cu_arr.size() == 1 and (cu->getType().getBasicType() == glslang::EbtInt or cu->getType().getBasicType() == glslang::EbtUint) );
		CHECK_ERR( (cu_arr[0].getType() == glslang::EbtInt or cu_arr[0].getType() == glslang::EbtUint) and
				   cu_arr[0].getIConst() >= 0 and cu_arr[0].getIConst() < int(stTypeList.size()) );
		
		field = stTypeList[ cu_arr[0].getIConst() ].type;
		return true;
	}
	
/*
=================================================
	GetElementSize
=================================================
*/
	static BytesU GetElementSize (const glslang::TType &info, bool forArrayElement)
	{
		BytesU	scalar_size;
		uint	array_size	= 1;

		switch ( info.getBasicType() )
		{
			case glslang::TBasicType::EbtBool :
			case glslang::TBasicType::EbtInt :
			case glslang::TBasicType::EbtUint :
			case glslang::TBasicType::EbtFloat :		scalar_size = 4_b;	break;
			
			case glslang::TBasicType::EbtInt64 :
			case glslang::TBasicType::EbtUint64 :
			case glslang::TBasicType::EbtDouble :		scalar_size = 8_b;	break;

			case glslang::TBasicType::EbtInt16 :
			case glslang::TBasicType::EbtUint16 :
			case glslang::TBasicType::EbtFloat16 :		scalar_size = 2_b;	break;

			case glslang::TBasicType::EbtInt8 :
			case glslang::TBasicType::EbtUint8 :		scalar_size = 1_b;	break;
		}
		
		if ( info.isArray() and not forArrayElement )
		{
			if ( info.isExplicitlySizedArray() )
				array_size = info.getOuterArraySize();
			else
				array_size = 0;
		}

		if ( info.isStruct() )
		{
			BytesU	size;
			for (auto& fld : *info.getStruct())
			{
				size += GetElementSize( *fld.type );
			}

			size = AlignToLarge( size, 16_b );
			return size * array_size;
		}

		if ( info.isMatrix() )
		{
			return AlignToLarge( scalar_size * info.getMatrixRows(), 16_b ) * info.getMatrixCols() * array_size;
		}

		if ( info.getVectorSize() > 0 )
		{
			return scalar_size * info.getVectorSize() * array_size;
		}

		RETURN_ERR( "unknown element type!" );
	}

/*
=================================================
	GetFieldOffset
=================================================
*/
	static BytesU GetFieldOffset (const glslang::TTypeList &stTypeList, const glslang::TType *field)
	{
		if ( field->getQualifier().hasOffset() )
		{
			return BytesU(field->getQualifier().layoutOffset);
		}

		BytesU	offset;

		for (auto& fld : stTypeList)
		{
			if ( fld.type == field )
				return offset;

			offset += GetElementSize( *fld.type );
		}
		
		RETURN_ERR( "field is not found!" );
	}
	
/*
=================================================
	TranslateBufferLoadOp2
=================================================
*/
	static bool TranslateBufferLoadOp2 (OUT Translator::SymbolID &id, OUT String &offsetStr, OUT String &bufferSrc,
										TIntermNode* node, const uint uid, Translator &translator)
	{
		using Offset_t		= Union< String, BytesU >;
		using OffsetArr_t	= Array< Offset_t >;

		Translator::Node		dst_node;	dst_node.uid = uid;
		OffsetArr_t				offsets;	offsets.PushBack( Offset_t{""_str} );
		const uint				lhs_uid		= ++translator.uid;
		glslang::TIntermBinary*	binary		= node->getAsBinaryNode();

		CHECK_ERR( binary );
		CHECK_ERR( ConvertType( binary, binary->getType(), null, translator, OUT dst_node.typeInfo ) );

		while ( binary )
		{
			switch ( binary->getOp() )
			{
				case glslang::TOperator::EOpIndexDirectStruct :
				{
					const auto&		type = binary->getLeft()->getType();
					CHECK_ERR( type.isStruct() );
					
					glslang::TType*	field_type = null;
					CHECK_ERR( GetFieldType( binary->getRight()->getAsConstantUnion(), *type.getStruct(), OUT field_type ) );

					offsets.PushBack( Offset_t{ GetFieldOffset( *type.getStruct(), field_type ) } );
					break;
				}

				case glslang::TOperator::EOpIndexDirect :
				{
					const BytesU						elem_size	= GetElementSize( binary->getLeft()->getType(), true );
					glslang::TIntermConstantUnion*		cu			= binary->getRight()->getAsConstantUnion();
					CHECK_ERR( cu and elem_size > 0_b );

					glslang::TConstUnionArray const&	cu_arr		= cu->getConstArray();
					CHECK_ERR( cu_arr.size() == 1 and cu_arr[0].getType() == glslang::TBasicType::EbtInt );

					offsets.PushBack( Offset_t{ elem_size * cu_arr[0].getIConst() } );
					break;
				}

				case glslang::TOperator::EOpIndexIndirect :
				{
					const BytesU	elem_size	= GetElementSize( binary->getLeft()->getType(), true );
					const uint		index_uid	= ++translator.uid;
					
					CHECK_ERR( RecursiveProcessNode( binary->getRight(), index_uid, translator ) );

					const auto&		index = translator.nodes( index_uid );
					CHECK_ERR( not index.src.Empty() );

					offsets.PushBack( Offset_t{ index.src + "*" + usize(elem_size) } );
					break;
				}

				default :
					RETURN_ERR( "unsupported operator!" );
			}

			glslang::TIntermBinary*	next_bin = binary->getLeft()->getAsBinaryNode();
			if ( not next_bin )
			{
				CHECK_ERR( binary->getLeft()->getAsSymbolNode() );
				id = GetSymbolID( binary->getLeft() );

				CHECK_ERR( RecursiveProcessNode( binary->getLeft(), lhs_uid, translator ) );
				break;
			}

			binary = next_bin;
		}

		BytesU	offset_bytes;

		for (auto& off : RevRange(offsets))
		{
			if ( off.Is<BytesU>() )
			{
				offset_bytes += off.Get<BytesU>();
			}
			else
			{
				if ( offset_bytes > 0 )
				{
					const String	off_src = String().FormatI( usize(offset_bytes), 10 );

					offsetStr    = offsetStr.Empty() ? off_src : ("("_str << offsetStr << " + " << off_src << ')');
					offset_bytes = 0_b;
				}

				if ( not off.Get<String>().Empty() ) {
					offsetStr = offsetStr.Empty() ? off.Get<String>() : (String(offsetStr) << " + " << off.Get<String>());
				}
			}
		}

		if ( offsetStr.Empty() )
			offsetStr = "0";

		const auto&	lhs = translator.nodes( lhs_uid );

		CHECK_ERR( EShaderVariable::IsBuffer( lhs.typeInfo.type ) );
		CHECK_COMP2( GXCheckAccessToExternal( translator, lhs ) );

		CHECK_ERR( translator.language->TranslateBufferLoad( id, lhs.src, offsetStr, dst_node.typeInfo, OUT dst_node.src ) );
		
		dst_node.typeInfo.isGlobal |= lhs.typeInfo.isGlobal;

		bufferSrc = lhs.src;
		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}

/*
=================================================
	TranslateBufferLoadOp
=================================================
*/
	static bool TranslateBufferLoadOp (TIntermNode* node, const uint uid, Translator &translator)
	{
		Translator::SymbolID	id	= Translator::SymbolID(~0u);
		String					offset_str;
		String					buffer_src;

		return TranslateBufferLoadOp2( OUT id, OUT offset_str, OUT buffer_src, node, uid, translator );
	}

/*
=================================================
	TranslateLoadStoreOp
=================================================
*/
	static bool TranslateLoadStoreOp (TIntermNode* node, const uint uid, Translator &translator)
	{
		using TOperator = glslang::TOperator;

		if ( glslang::TIntermBinary* binary = node->getAsBinaryNode() )
		{
			if ( binary->getOp() >= TOperator::EOpAssign and
				 binary->getOp() <= TOperator::EOpRightShiftAssign )
			{
				TOperator	op = TOperator::EOpNull;

				switch ( binary->getOp() )
				{
					case TOperator::EOpAssign :						op = TOperator::EOpAssign;				break;
					case TOperator::EOpAddAssign :					op = TOperator::EOpAdd;					break;
					case TOperator::EOpSubAssign :					op = TOperator::EOpSub;					break;
					case TOperator::EOpMulAssign :					op = TOperator::EOpMul;					break;
					case TOperator::EOpVectorTimesMatrixAssign :	op = TOperator::EOpVectorTimesMatrix;	break;
					case TOperator::EOpVectorTimesScalarAssign :	op = TOperator::EOpVectorTimesScalar;	break;
					case TOperator::EOpMatrixTimesScalarAssign :	op = TOperator::EOpMatrixTimesScalar;	break;
					case TOperator::EOpMatrixTimesMatrixAssign :	op = TOperator::EOpMatrixTimesMatrix;	break;
					case TOperator::EOpDivAssign :					op = TOperator::EOpDiv;					break;
					case TOperator::EOpModAssign :					op = TOperator::EOpMod;					break;
					case TOperator::EOpAndAssign :					op = TOperator::EOpAnd;					break;
					case TOperator::EOpInclusiveOrAssign :			op = TOperator::EOpInclusiveOr;			break;
					case TOperator::EOpExclusiveOrAssign :			op = TOperator::EOpExclusiveOr;			break;
					case TOperator::EOpLeftShiftAssign :			op = TOperator::EOpLeftShift;			break;
					case TOperator::EOpRightShiftAssign :			op = TOperator::EOpRightShift;			break;
				}
				
				Translator::Node		dst_node;	dst_node.uid = uid;
				const uint				lhs_uid		= ++translator.uid;
				const uint				rhs_uid		= ++translator.uid;
				
				Translator::SymbolID	id			= Translator::SymbolID(~0u);
				String					offset_str;
				String					buffer_src;

				CHECK_ERR( ConvertType( binary, binary->getType(), null, translator, OUT dst_node.typeInfo ) );
				CHECK_ERR( TranslateBufferLoadOp2( OUT id, OUT offset_str, OUT buffer_src, binary->getLeft(), lhs_uid, translator ) );
				CHECK_ERR( RecursiveProcessNode( binary->getRight(), rhs_uid, translator ) );
				
				const auto&	lhs = translator.nodes( lhs_uid );
				const auto&	rhs = translator.nodes( rhs_uid );
				CHECK_ERR( not rhs.src.Empty() );

				if ( op == TOperator::EOpAssign ) {
					CHECK_ERR( translator.language->TranslateBufferStore( id, buffer_src, offset_str, dst_node.typeInfo, rhs.src, rhs.typeInfo, OUT dst_node.src ) );
				} else {
					String	data;
					CHECK_ERR( translator.language->TranslateOperator( op, dst_node.typeInfo, { lhs.src, rhs.src }, { &lhs.typeInfo, &rhs.typeInfo }, OUT data ) );
					CHECK_ERR( translator.language->TranslateBufferStore( id, buffer_src, offset_str, dst_node.typeInfo, data, dst_node.typeInfo, OUT dst_node.src ) );
				}

				translator.nodes.Add( uid, RVREF(dst_node) );
				return true;
			}

			CHECK_ERR( TranslateBufferLoadOp( binary, uid, translator ) );
			return true;
		}

		return false;
	}

/*
=================================================
	TranslateIndexDirectStruct
=================================================
*/
	static bool TranslateIndexDirectStruct (glslang::TIntermOperator* node, const uint uid, Translator &translator)
	{
		glslang::TIntermBinary*	binary		= node->getAsBinaryNode();
		Translator::Node		dst_node;	dst_node.uid = uid;
		
		CHECK_ERR( ConvertType( node, node->getType(), null, translator, OUT dst_node.typeInfo ) );

		CHECK_ERR( binary and binary->getOp() == glslang::TOperator::EOpIndexDirectStruct );
		CHECK_ERR( binary->getLeft()->getType().isStruct() and binary->getRight()->getAsConstantUnion() );
		
		const auto&	st_type = *binary->getLeft()->getType().getStruct();

		const uint	lhs_uid	= ++translator.uid;
		CHECK_ERR( RecursiveProcessNode( binary->getLeft(), lhs_uid, translator ) );
		
		const auto&	lhs = translator.nodes( lhs_uid );
		CHECK_COMP2( GXCheckAccessToExternal( translator, lhs ) );

		glslang::TType *	field_type = null;
		CHECK_ERR( GetFieldType( binary->getRight()->getAsConstantUnion(), st_type, OUT field_type ) );
		
		Translator::TypeInfo	field_info;
		CHECK_ERR( ConvertType( null, *field_type, null, translator, OUT field_info ) );

		CHECK_ERR( translator.language->TranslateStructAccess( GetSymbolID( binary->getLeft() ), lhs.typeInfo, lhs.src, field_info, OUT dst_node.src ) );

		dst_node.typeInfo.isGlobal |= lhs.typeInfo.isGlobal;

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	TranslateConstUnionAccess
=================================================
*/
	static bool TranslateConstUnionAccess (TIntermNode* node, const uint uid, Translator &translator)
	{
		glslang::TIntermBinary *			binary	= node->getAsBinaryNode();
		glslang::TIntermConstantUnion*		cu		= binary->getLeft()->getAsConstantUnion();
		glslang::TConstUnionArray const&	cu_arr	= cu->getConstArray();
		Translator::Node					dst_node;

		dst_node.uid = uid;
		CHECK_ERR( ConvertType( cu, cu->getType(), null, translator, OUT dst_node.typeInfo ) );
		
		const Translator::ConstKey	cu_key { &cu_arr, dst_node.typeInfo };

		for (const auto& cached : translator.constants.symbNodes)
		{
			if ( cu_key == cached.first )
			{
				dst_node.src << cached.second.name;
				break;
			}
		}
		CHECK_ERR( not dst_node.src.Empty() );

		const uint		rhs_uid	= ++translator.uid;
		CHECK_ERR( RecursiveProcessNode( binary->getRight(), rhs_uid, translator ) );

		const auto&		rhs = translator.nodes( rhs_uid ).src;
		CHECK_ERR( not rhs.Empty() );

		dst_node.src << '[' << rhs << ']';

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	TranslateFunctionCall
=================================================
*/
	static bool TranslateFunctionCall (TIntermNode* node, const uint uid, Translator &translator)
	{
		glslang::TIntermAggregate*	aggr = node->getAsAggregate();
		CHECK_ERR( aggr and aggr->getOp() == glslang::TOperator::EOpFunctionCall );

		const String	signature = aggr->getName().c_str();

		Translator::InlFunctionsMap_t::iterator	iter;

		if ( translator.inl.functions.Find( signature, OUT iter ) ) {
			return TranslateInlineFunctionCall( node, uid, iter->second, translator );
		} else {
			CHECK_ERR( not translator.states.inlineAll );
		}

		Translator::Node						dst_node;
		Array< uint >							args_uids;
		Array< String >							args_src;
		Array< Translator::TypeInfo const* >	args_types;

		// get result type
		CHECK_ERR( ConvertType( node, aggr->getType(), null, translator, OUT dst_node.typeInfo ) );
		
		FOR( i, aggr->getSequence() )
		{
			const uint	arg_uid = ++translator.uid;

			CHECK_ERR( RecursiveProcessNode( aggr->getSequence()[i], arg_uid, translator ) );

			args_uids.PushBack( arg_uid );
		}

		FOR( i, args_uids )
		{
			const auto&	arg = translator.nodes( args_uids[i] );
			CHECK_ERR( not arg.src.Empty() );

			args_src.PushBack( arg.src );
			args_types.PushBack( &arg.typeInfo );
		}

		if ( translator.language->MustBeInlined( signature, args_types ) )
		{
			CHECK_ERR( translator.inl.allFunctions.Find( signature, OUT iter ) );
			return TranslateInlineFunctionCall( node, uid, iter->second, translator );
		}

		CHECK_ERR( translator.language->TranslateFunctionCall( signature, dst_node.typeInfo, args_src, args_types, OUT dst_node.src ) );

		dst_node.uid = uid;

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}

/*
=================================================
	UidToPrefix
=================================================
*/
	static String UidToPrefix (const uint uid, Translator &translator)
	{
		const usize	az		= usize('Z' - 'A');
		const usize	hash1	= HashOf( uid ).Get();
		usize		hash	= hash1;
		String		result;

		if_constexpr ( sizeof(hash) > sizeof(uint) ) {
			hash = ((hash >> 31) ^ hash) & 0xFFFFFFFF;
		}
		hash = ((hash >> 16) ^ hash) & 0xFFFF;

		for (uint i = 1; i < 4; ++i)
		{
			result << 'x';
			for (uint j = 1; j <= i; ++j)
			{
				const char	c = ('A' + char( (hash >> (j*4)) % az ));
				result << c;
			}
			result << '_';

			if ( not translator.inl.localNames.IsExist( result ) )
			{
				translator.inl.localNames.Add( result );
				return result;
			}
		}

		RETURN_ERR( "bad hash function!", (String("x") << String().FormatAlignedI( hash1, sizeof(hash1)*2, '0', 16 ) << '_') );
	}
	
/*
=================================================
	TranslateInlineFunctionCall
=================================================
*/
	static bool TranslateInlineFunctionCall (TIntermNode* node, const uint uid, TIntermNode* funcNode, Translator &translator)
	{
		glslang::TIntermAggregate*	aggr = node->getAsAggregate();
		glslang::TIntermAggregate*	func = funcNode->getAsAggregate();

		CHECK_ERR( aggr and aggr->getOp() == glslang::TOperator::EOpFunctionCall );
		CHECK_ERR( func and func->getOp() == glslang::TOperator::EOpFunction and func->getSequence().size() == 2 );
		
		const String		prefix	= UidToPrefix( uid, translator );
		Translator::Node	dst_node;
		String				temp;
	
		dst_node.uid = uid;
		
		temp << "\n// " << aggr->getName().c_str() << "\n";

		// decl args
		{
			Translator::Symbol				func_result;
			glslang::TIntermAggregate*		params	= func->getSequence()[0]->getAsAggregate();

			CHECK_ERR( ConvertType( func, func->getType(), null, translator, OUT func_result ) );
			CHECK_ERR( params and params->getOp() == glslang::TOperator::EOpParameters );
			CHECK_ERR( params->getSequence().size() == aggr->getSequence().size() );

			FOR( i, aggr->getSequence() )
			{
				// get argument type
				Translator::Symbol			param_type;
				glslang::TIntermTyped *		nn		= params->getSequence()[i]->getAsTyped();
				
				CHECK_ERR( ConvertSymbol( nn, nn->getType(), null, translator, OUT param_type ) );
				
				prefix >> param_type.name;

				translator.fwd.funcArguments.Add( param_type.id, param_type.name );

				// get argument value
				const uint	arg_uid = ++translator.uid;
				CHECK_ERR( RecursiveProcessNode( aggr->getSequence()[i], arg_uid, translator ) );

				const auto&	arg = translator.nodes( arg_uid );
				CHECK_ERR( not arg.src.Empty() );

				const bool	is_external =	EShaderVariable::IsTexture( param_type.type ) or
											EShaderVariable::IsImage( param_type.type ) or
											arg.typeInfo.qualifier[ EVariableQualifier::Shared ];
				const bool	is_large	=	param_type.arraySize.IsArray() or EShaderVariable::IsStruct( param_type.type );
				const bool	is_const	=	param_type.qualifier[ EVariableQualifier::Constant ];
				const bool	is_output	=	param_type.qualifier[ EVariableQualifier::OutArg ] or param_type.arraySize.IsDynamicArray();

				// external types must be replaced, no temporary variables
				if ( is_external or is_output or (is_large and is_const) )
				{
					translator.inl.localsReplacer.Add( param_type.id, arg.src );
					continue;
				}
				
				// build source
				CHECK_ERR( translator.language->TranslateLocalVar( param_type, INOUT temp ) );
				temp << " = " << arg.src << ";\n";
			}

			// add result type
			if ( func_result.type != EShaderVariable::Void )
			{
				String	arg = EShaderVariable::IsStruct(func_result.type) ? " " : "0";

				func_result.name = prefix + "return";
				CHECK_ERR( translator.language->TranslateLocalVar( func_result, INOUT temp ) );
				temp << " = ";
				CHECK_ERR( translator.language->TranslateOperator( glslang::TOperator::EOpConstructGuardStart,
																  func_result, {arg}, {&func_result}, INOUT temp ) );
				temp << ";\n";
				dst_node.src << func_result.name;
			}
			else
				dst_node.src << " ";	// to avoid empty string checking

			// add exit flag
			{
				Translator::Symbol	bool_flag;
				bool_flag.name	= prefix + "exit";	// TODO: check is unique name
				bool_flag.type	= EShaderVariable::Bool;
				
				CHECK_ERR( translator.language->TranslateLocalVar( bool_flag, INOUT temp ) );
				temp << " = ";
				CHECK_ERR( translator.language->TranslateOperator( glslang::TOperator::EOpConstructGuardStart,
																  bool_flag, {"false"}, {&bool_flag}, INOUT temp ) );
				temp << ";\n";
			}
		}


		// place function in scope
		{
			translator.inl.prefixStack.Push({ prefix, {} });
			translator.fwd.fnScope.PushBack( {} );

			glslang::TIntermAggregate*	body		= func->getSequence()[1]->getAsAggregate();
			const uint					body_uid	= ++translator.uid;
			
			CHECK_ERR( RecursiveProcessNode( body, body_uid, translator ) );

			String		body_src = translator.nodes( body_uid ).src;
			CHECK_ERR( not body_src.Empty() );
			
			StringParser::IncreaceIndent( INOUT body_src );

			temp << "do {\n" << body_src << "\n} while(false);\n// end " << aggr->getName().c_str();

			ASSERT( prefix == translator.inl.prefixStack.Get().prefix );
			
			translator.inl.prefixStack.Pop();
			translator.fwd.fnScope.PopBack();
		}

		translator.fwd.scope.Back().addBeforeLine.PushBack( RVREF(temp) );
		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}

/*
=================================================
	TranslateOperatorCall
=================================================
*/
	static bool TranslateOperatorCall (TIntermNode* node, const uint uid, Translator &translator)
	{
		glslang::TIntermOperator*				op = node->getAsOperator();
		Translator::Node						dst_node;
		Array< String >							args_src;
		Array< Translator::TypeInfo const* >	args_types;
		Array< uint >							args_uids;

		// get result type
		dst_node.uid = uid;
		CHECK_ERR( ConvertType( node, op->getType(), null, translator, OUT dst_node.typeInfo ) );
		
		// convert args
		if ( node->getAsAggregate() )
		{
			glslang::TIntermAggregate* aggr = node->getAsAggregate();
			
			FOR( i, aggr->getSequence() )
			{
				const uint	arg_uid = ++translator.uid;

				CHECK_ERR( RecursiveProcessNode( aggr->getSequence()[i], arg_uid, translator ) );

				args_uids.PushBack( arg_uid );
			}
		}

		FOR( i, args_uids )
		{
			const auto&	arg = translator.nodes( args_uids[i] );
			CHECK_ERR( not arg.src.Empty() );

			args_src.PushBack( arg.src );
			args_types.PushBack( &arg.typeInfo );
		}
		

		if ( op->getOp() == glslang::TOperator::EOpComma )
		{
			FOR( i, args_src ) {
				dst_node.src << (i ? ", " : "") << args_src[i];
			}
		}
		else
		{
			CHECK_ERR( translator.language->TranslateOperator( op->getOp(), dst_node.typeInfo, args_src, args_types, OUT dst_node.src ) );
		}

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	RecursivePrepass
=================================================
*/
	static bool RecursivePrepass (TIntermNode* node, Translator &translator)
	{
		if ( not node )
			return true;

		translator.nodeStack.PushBack( node );

		if ( glslang::TIntermAggregate* aggr = node->getAsAggregate() )
		{
			switch ( aggr->getOp() )
			{
				case glslang::TOperator::EOpLinkerObjects :
					break;

				// 2 args, first is atomic
				case glslang::TOperator::EOpAtomicAdd :
				case glslang::TOperator::EOpAtomicAnd :
				case glslang::TOperator::EOpAtomicOr :
				case glslang::TOperator::EOpAtomicXor :
				case glslang::TOperator::EOpAtomicMin :
				case glslang::TOperator::EOpAtomicMax :
				case glslang::TOperator::EOpAtomicExchange :

				// 3 args, first is atomic
				case glslang::TOperator::EOpAtomicCompSwap :
					CHECK_ERR( AddAtomicArgQualifier( aggr->getSequence()[0], translator ) );
					break;

				case glslang::TOperator::EOpFunction :
					CHECK_ERR( RegisterFunctionDeclaration( aggr, translator ) );
					break;
			}

			FOR( i, aggr->getSequence() )
			{
				CHECK_ERR( RecursivePrepass( aggr->getSequence()[i], translator ) );
			}
		}
		else
		if ( glslang::TIntermUnary* unary = node->getAsUnaryNode() )
		{
			CHECK_ERR( RecursivePrepass( unary->getOperand(), translator ) );
		}
		else
		if ( glslang::TIntermBinary* binary = node->getAsBinaryNode() )
		{
			if ( binary->getOp() == glslang::TOperator::EOpIndexDirectStruct and
				 binary->getLeft()->getAsSymbolNode() and
				 binary->getLeft()->getAsSymbolNode()->getType().getQualifier().storage == glslang::TStorageQualifier::EvqBuffer )
			{
				CHECK_ERR( RegisterBUfferAccess( binary, translator ) );
			}

			CHECK_ERR( RecursivePrepass( binary->getLeft(), translator ) );
			CHECK_ERR( RecursivePrepass( binary->getRight(), translator ) );
		}
		else
		if ( glslang::TIntermBranch* branch = node->getAsBranchNode() )
		{
			CHECK_ERR( RecursivePrepass( branch->getExpression(), translator ) );
		}
		else
		if ( glslang::TIntermSwitch* sw = node->getAsSwitchNode() )
		{
			CHECK_ERR( RecursivePrepass( sw->getCondition(), translator ) );
			CHECK_ERR( RecursivePrepass( sw->getBody(), translator ) );
		}
		else
		if ( glslang::TIntermSelection* selection = node->getAsSelectionNode() )
		{
			CHECK_ERR( RecursivePrepass( selection->getCondition(), translator ) );
			CHECK_ERR( RecursivePrepass( selection->getTrueBlock(), translator ) );
			CHECK_ERR( RecursivePrepass( selection->getFalseBlock(), translator ) );
		}
		else
		if ( glslang::TIntermLoop* loop = node->getAsLoopNode() )
		{
			CHECK_ERR( RecursivePrepass( loop->getBody(), translator ) );
			CHECK_ERR( RecursivePrepass( loop->getTest(), translator ) );
			CHECK_ERR( RecursivePrepass( loop->getTerminal(), translator ) );
		}
		else
		if ( glslang::TIntermSymbol* symbol = node->getAsSymbolNode() )
		{
			CHECK_ERR( RegisterStructType( symbol, translator ) );
		}

		translator.nodeStack.PopBack();
		return true;
	}
	
/*
=================================================
	RegisterBUfferAccess
=================================================
*/
	static bool RegisterBUfferAccess (TIntermNode* node, Translator &translator)
	{
		TIntermNode*	last_node = node;

		for (auto& n : RevRange(translator.nodeStack))
		{
			if ( glslang::TIntermBinary* binary = n->getAsBinaryNode() )
			{
				if ( binary->getOp() == glslang::TOperator::EOpIndexDirectStruct or
					 binary->getOp() == glslang::TOperator::EOpIndexDirect or
					 binary->getOp() == glslang::TOperator::EOpIndexIndirect )
				{
					last_node = n;
					continue;
				}

				if ( binary->getOp() >= glslang::TOperator::EOpAssign and
					 binary->getOp() <= glslang::TOperator::EOpRightShiftAssign and
					 binary->getLeft() == last_node )
				{
					last_node = n;
					continue;
				}
			}
			break;
		}

		translator.types.bufferNodes.Add( last_node );
		return true;
	}

/*
=================================================
	AddAtomicArgQualifier
=================================================
*/
	static bool AddAtomicArgQualifier (TIntermNode* node, Translator &translator)
	{
		Array< glslang::TIntermBinary *>	stack;	stack.PushBack( node->getAsBinaryNode() );
		CHECK_ERR( stack.Back() );

		for (; stack.Back()->getLeft()->getAsBinaryNode();)
		{
			stack.PushBack( stack.Back()->getLeft()->getAsBinaryNode() );
		}

		// add SSBO type
		const auto&		ssbo_typename	= stack.Back()->getLeft()->getType().getTypeName();
		CHECK_ERR( &ssbo_typename != null );
		
		String			ssb_type	= ssbo_typename.c_str();
		Array<String>	ssb_fields;

		// add fields
		for (; not stack.Empty();)
		{
			glslang::TIntermBinary *	binary = stack.Back();		stack.PopBack();

			CHECK_ERR( binary and binary->getOp() == glslang::TOperator::EOpIndexDirectStruct  );
			CHECK_ERR( binary->getLeft()->getType().isStruct() and binary->getRight()->getAsConstantUnion() );

			const auto&	st_type = *binary->getLeft()->getType().getStruct();
				
			glslang::TIntermConstantUnion*		cu		= binary->getRight()->getAsConstantUnion();
			glslang::TConstUnionArray const&	cu_arr	= cu->getConstArray();
		
			// constant union must be correct index of struct field
			CHECK_ERR( cu_arr.size() == 1 and (cu->getType().getBasicType() == glslang::EbtInt or cu->getType().getBasicType() == glslang::EbtUint) );
			CHECK_ERR( (cu_arr[0].getType() == glslang::EbtInt or cu_arr[0].getType() == glslang::EbtUint) and
						cu_arr[0].getIConst() >= 0 and cu_arr[0].getIConst() < int(st_type.size()) );

			const auto&	field_name = st_type[ cu_arr[0].getIConst() ].type->getFieldName();
			CHECK_ERR( &field_name != null );

			ssb_fields.PushBack( field_name.c_str() );
		}

		// check for duplicates
		usize	idx		= UMax;
		auto&	atomics	= translator.types.atomics;

		if ( atomics.FindFirstIndex( ssb_type, OUT idx ) )
		{
			for (; idx < atomics.Count() and atomics[idx].first == ssb_type; ++idx)
			{
				const auto& fields = atomics[idx].second;

				if ( ssb_fields == fields )
				{
					// fields already added
					return true;
				}
			}
		}

		atomics.Add( RVREF(ssb_type), RVREF(ssb_fields) );
		return true;
	}
	
/*
=================================================
	RegisterFunctionDeclaration
=================================================
*/
	static bool RegisterFunctionDeclaration (TIntermNode* node, Translator &translator)
	{
		glslang::TIntermAggregate *		aggr		= node->getAsAggregate();
		const String					signature	= aggr->getName().c_str();
		Array< Translator::Symbol >		func_args;

		CHECK_ERR( aggr and aggr->getOp() == glslang::TOperator::EOpFunction );
		
		Translator::TypeInfo	ret_type;
		CHECK_ERR( ConvertType( aggr, aggr->getType(), null, translator, OUT ret_type ) );
		
		const bool	is_entry	= IsProgramEntry( signature, translator );
		bool		make_inline = translator.states.inlineAll;

		// find arguments
		FOR( i, aggr->getSequence() )
		{
			TIntermNode* n = aggr->getSequence()[i];

			if ( n->getAsAggregate() and n->getAsAggregate()->getOp() == glslang::TOperator::EOpParameters )
			{
				// get arguemnts
				glslang::TIntermAggregate*	args_node = n->getAsAggregate();
				
				func_args.Resize( func_args.Count() + args_node->getSequence().size() );

				FOR( j, args_node->getSequence() )
				{
					Translator::Symbol &		arg	= func_args[j];
					glslang::TIntermTyped *		nn	= args_node->getSequence()[j]->getAsTyped();

					CHECK_ERR( ConvertSymbol( nn, nn->getType(), null, translator, OUT arg ) );
					
					// function with dynamic array must be inlined
					if ( arg.arraySize.IsDynamicArray() and not is_entry and translator.states.useGXrules )
					{
						make_inline = true;
					}
				}
			}
		}

		// add function declaration to cache
		String	str;
		CHECK_ERR( translator.language->TranslateFunctionDecl( signature, ret_type, func_args, OUT str ) );
		
		if ( not is_entry and make_inline )
		{
			translator.inl.functions.Add( signature, aggr );
		}
		
		translator.inl.allFunctions.Add( signature, aggr );
		return true;
	}

/*
=================================================
	RecursiveExtractTypesFromInternalTypes
=================================================
*/
	static void RecursiveExtractTypesFromInternalTypes (Translator &translator, const Translator::TypeInfo &info)
	{
		// search for struct types
		for (auto& field : info.fields)
		{
			if ( EShaderVariable::IsStruct( field.type ) and
				 not translator.types.globalTypes.IsExist( field.typeName ) )
			{
				translator.types.globalTypes.Add( field.typeName, field );

				RecursiveExtractTypesFromInternalTypes( translator, field );
			}
		}
	}

/*
=================================================
	RegisterStructType
=================================================
*/
	static bool RegisterStructType (glslang::TIntermSymbol* node, Translator &translator)
	{
		glslang::TIntermSymbol*		symbol		= node->getAsSymbolNode();
		glslang::TType const&		type		= symbol->getType();

		if ( type.isStruct()									and
			 not type.isBuiltIn()								and
			 not IsBuiltinName( type.getTypeName().c_str() )	and
			 not translator.types.globalTypes.IsExist( type.getTypeName().c_str() ) )
		{
			Translator::Symbol	info;
			CHECK_ERR( ConvertSymbol( symbol, type, null, translator, OUT info ) );

			translator.types.globalTypes.Add( info.typeName, info );

			RecursiveExtractTypesFromInternalTypes( translator, info );
		}

		return true;
	}

}	// PipelineCompiler
