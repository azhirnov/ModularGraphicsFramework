// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_Translator.h"
#include "Engine/PipelineCompiler/Common/ToGLSL.h"

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
	static bool AddAtomicArgQualifier (TIntermNode* node, Translator &translator);
	static bool RegisterFunctionDeclaration (TIntermNode* node, Translator &translator);

	static bool TranslateFunction (glslang::TIntermAggregate* aggr, const uint uid, Translator &translator);
	static bool TranslateExternalObjects (glslang::TIntermAggregate* aggr, const uint uid, Translator &translator);
	static bool TranslateSharedObjects (glslang::TIntermAggregate* aggr, const uint uid, Translator &translator);
	static bool TranslateGlobals (glslang::TIntermAggregate* aggr, const uint uid, Translator &translator);
	static bool ConvertType (TIntermNode* node, glslang::TType const &, glslang::TSourceLoc const &, const Translator::TypeInfo *parent, const Translator &, OUT Translator::TypeInfo &);
	static bool IsBuiltinName (StringCRef name);
	static bool TranslateVectorSwizzle (glslang::TIntermOperator* node, const uint uid, Translator &translator);
	static bool TranslateIndexDirectStruct (glslang::TIntermOperator* node, const uint uid, Translator &translator);
	static bool TranslateOperatorCall (TIntermNode* node, const uint uid, Translator &translator);
	static bool TranslateFunctionCall (TIntermNode* node, const uint uid, Translator &translator);
	static bool TranslateInlineFunctionCall (TIntermNode* node, const uint uid, TIntermNode* func, Translator &translator);
	static bool TranslateMain (TIntermNode* node, const uint uid, bool skipExternals, Translator &translator);
	static bool TranslateConstUnionAccess (TIntermNode* node, const uint uid, Translator &translator);
	static bool TranslateFunctionForwardDecl (glslang::TIntermAggregate* node, const Translator &translator, INOUT String &src);

	static bool GXCheckAccessToExternal (const Translator &translator, const Translator::Node &node);
	static bool GXCheckExternalQualifiers (const Translator &translator, const glslang::TQualifier &qual);
	
		
/*
=================================================
	Main
=================================================
*/
	bool Translator::Main (TIntermNode* node, bool skipExternals)
	{
		nodeStack.PushBack( node );
		fwd.scope.PushBack( {} );

		CHECK_ERR( TranslateMain( node, this->uid, skipExternals, *this ) );

		nodeStack.PopBack();
		fwd.scope.PopBack();

		CHECK( nodeStack.Empty() );
		CHECK( fwd.scope.Empty() );

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
			// this rule used for compatibility with OpenCL.
			if ( not translator.states.isMain ) {
				return not node.typeInfo.isGlobal;
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
	hash of TConstUnionArray
=================================================
*/
	HashResult  ConstUnionHash::operator () (const glslang::TConstUnionArray *key) const
	{
		return _ArrHash( *key );
	}

	HashResult  ConstUnionHash::_ArrHash (const glslang::TConstUnionArray &key) const
	{
		HashResult	result;
		for (int i = 0; i < key.size(); ++i) {
			result += _ValHash( key[i] );
		}
		return result;
	}
	
/*
=================================================
	_ValHash
=================================================
*/
	HashResult  ConstUnionHash::_ValHash (const glslang::TConstUnion &key) const
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
	TranslateCustomTypes
=================================================
*/
	static bool TranslateCustomTypes (Translator &translator, bool skipExternals)
	{
		auto&	src = translator.src;

		if ( skipExternals )
		{
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

		// custom types declaration
		if ( not translator.types.globalTypes.Empty() )
		{
			CHECK_ERR( TranslateCustomTypes( translator, skipExternals ) );
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
		Array< Translator::TypeInfo >	func_args;
		const String					func_name		= aggr->getName().c_str();
		StringCRef						short_name		= func_name;
		bool							will_inlined	= false;

		// get result type
		CHECK_ERR( ConvertType( aggr, aggr->getType(), aggr->getLoc(), null, translator, OUT ret_type ) );
		
		// get name
		{
			usize	pos = 0;
			if ( func_name.Find( '(', OUT pos ) )
				short_name = func_name.SubString( 0, pos );
		}
		
		if ( short_name == translator.entryPoint )
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
				
				func_args.Reserve( func_args.Count() + args_node->getSequence().size() );

				FOR( j, args_node->getSequence() )
				{
					Translator::TypeInfo		arg;
					glslang::TIntermTyped *		nn = args_node->getSequence()[j]->getAsTyped();

					CHECK_ERR( ConvertType( nn, nn->getType(), nn->getLoc(), null, translator, OUT arg ) );

					// function with dynamic array must be inlined
					if ( arg.arraySize == UMax )
						will_inlined = true;

					func_args.PushBack( RVREF(arg) );
				}
			}
		}
		
		src << (will_inlined ? "// " : "");

		CHECK_ERR( translator.language->TranslateFunctionDecl( func_name, ret_type, func_args, INOUT src ) );

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
					Translator::TypeInfo const&	info = scope.pendingVars[j].second;

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
		Array< Translator::TypeInfo >	func_args;
		const String					func_name	= aggr->getName().c_str();
		StringCRef						short_name	= func_name;
		const uint						body_uid	= ++translator.uid;

		// get result type
		CHECK_ERR( ConvertType( aggr, aggr->getType(), aggr->getLoc(), null, translator, OUT dst_node.typeInfo ) );
		
		// get name
		{
			usize	pos = 0;
			if ( func_name.Find( '(', OUT pos ) )
				short_name = func_name.SubString( 0, pos );
		}

		const bool	is_entry = (short_name == translator.entryPoint);
		
		if ( not is_entry and translator.states.inlineAll )
		{
			translator.inl.functions.Add( func_name, aggr );
			return true;
		}

		// find arguments
		FOR( i, aggr->getSequence() )
		{
			TIntermNode* n = aggr->getSequence()[i];

			if ( n->getAsAggregate() and 
				 n->getAsAggregate()->getOp() == glslang::TOperator::EOpParameters )
			{
				// get arguemnts
				glslang::TIntermAggregate*	args_node = n->getAsAggregate();
				
				func_args.Reserve( func_args.Count() + args_node->getSequence().size() );

				FOR( j, args_node->getSequence() )
				{
					Translator::TypeInfo		arg;
					glslang::TIntermTyped *		nn		= args_node->getSequence()[j]->getAsTyped();
					glslang::TIntermSymbol *	symbol	= nn->getAsSymbolNode();

					CHECK_ERR( ConvertType( nn, nn->getType(), nn->getLoc(), null, translator, OUT arg ) );

					if ( symbol )
					{
						ASSERT( not translator.fwd.funcArguments.IsExist( symbol->getId() ) );
						translator.fwd.funcArguments.Add( symbol->getId(), arg.name );
					}

					// function with dynamic array must be inlined
					if ( arg.arraySize == UMax and not is_entry and translator.states.useGXrules )
					{
						translator.inl.functions.Add( func_name, aggr );
						return true;
					}

					// TODO: check rules?

					func_args.PushBack( RVREF(arg) );
				}
			}
			else
			{
				ASSERT( not translator.states.isMain );
				translator.states.isMain = is_entry;
	
				CHECK_ERR( RecursiveProcessNode( n, body_uid, translator ) );
				
				translator.states.isMain = false;
			}
		}

		String	body = translator.nodes( body_uid ).src;
		String	str;

		CHECK_ERR( not body.Empty() );
		CHECK_ERR( translator.language->TranslateFunctionDecl( func_name, dst_node.typeInfo, func_args, INOUT str ) );

		if ( is_entry ) {
			CHECK_ERR( translator.language->TranslateEntry( dst_node.typeInfo, func_name, func_args, INOUT str ) );
		}

		StringParser::IncreaceIndent( INOUT body );
		dst_node.src << str << "\n{\n" << body << "}\n\n";

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	ConvertType
=================================================
*/
	static bool ConvertType (TIntermNode* node, glslang::TType const &type, glslang::TSourceLoc const &loc, const Translator::TypeInfo *parent,
							 const Translator &translator, OUT Translator::TypeInfo &result)
	{
		auto const&		qual = type.getQualifier();

		// copy name
		if ( &type.getFieldName() != null )
		{
			result.name = type.getFieldName().c_str();
		}
		else
		if ( node and node->getAsSymbolNode() )
		{
			result.name = node->getAsSymbolNode()->getName().c_str();

			if ( result.name.StartsWithIC( "anon@" ) )
				result.name.Clear();
		}

		// copy typename
		if ( &type.getTypeName() != null )
		{
			result.typeName = type.getTypeName().c_str();
		}

		// arrays
		if ( type.isArray() )
		{
			result.arraySize = type.isExplicitlySizedArray() ? Max( 1, type.getOuterArraySize() ) : -1;
		}

		// binding
		result.binding = qual.hasBinding() ? qual.layoutBinding : UMax;

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
			auto const&	q = type.getQualifier();
			
			if ( type.isBuiltIn()					or
				 IsBuiltinName( result.typeName )	or
				 IsBuiltinName( result.name )		or
				(parent and parent->qualifier[ EVariableQualifier::BuiltIn ]) )
			{
				result.qualifier |= EVariableQualifier::BuiltIn;
			}

			if ( q.invariant )
				result.qualifier |= EVariableQualifier::Invariant;

			if ( q.noContraction )
				result.qualifier |= EVariableQualifier::Precise;

			if ( q.centroid )
				result.qualifier |= EVariableQualifier::Centroid;

			if ( q.smooth )
				result.qualifier |= EVariableQualifier::Smooth;

			if ( q.flat )
				result.qualifier |= EVariableQualifier::Flat;

			if ( q.nopersp )
				result.qualifier |= EVariableQualifier::NoPerspective;

			if ( q.patch )
				result.qualifier |= EVariableQualifier::Patch;

			if ( q.sample )
				result.qualifier |= EVariableQualifier::Sample;
			
			if ( q.specConstant or q.hasSpecConstantId() ) {
				result.qualifier  |= EVariableQualifier::Specialization;
				result.specConstID = q.layoutSpecConstantId;
			}
			
			CHECK_ERR( (q.coherent + q.volatil + q.restrict + q.readonly + q.writeonly) <= 1 );

			if ( q.coherent )
				result.memoryModel = EShaderMemoryModel::Coherent;

			if ( q.volatil )
				result.memoryModel = EShaderMemoryModel::Volatile;

			if ( q.restrict )
				result.memoryModel = EShaderMemoryModel::Restrict;

			if ( q.readonly )
				result.memoryModel = EShaderMemoryModel::ReadOnly;

			if ( q.writeonly )
				result.memoryModel = EShaderMemoryModel::WriteOnly;

			switch ( q.storage )
			{
				case glslang::TStorageQualifier::EvqTemporary :			result.qualifier |= EVariableQualifier::Local;		break;
				//case glslang::TStorageQualifier::EvqGlobal :			break;
				case glslang::TStorageQualifier::EvqConst :				result.qualifier |= EVariableQualifier::Constant;	break;
				case glslang::TStorageQualifier::EvqVaryingIn :			result.qualifier |= EVariableQualifier::In;			result.isGlobal = true;		break;
				case glslang::TStorageQualifier::EvqVaryingOut :		result.qualifier |= EVariableQualifier::Out;		result.isGlobal = true;		break;
				case glslang::TStorageQualifier::EvqUniform :			result.qualifier |= EVariableQualifier::Uniform;	result.isGlobal = true;		break;
				case glslang::TStorageQualifier::EvqBuffer :			result.isGlobal = true;		break;
				case glslang::TStorageQualifier::EvqShared :			result.qualifier |= EVariableQualifier::Shared;		result.isGlobal = true;		break;

				// function arguments
				case glslang::TStorageQualifier::EvqIn :				result.qualifier |= EVariableQualifier::InArg;		break;
				case glslang::TStorageQualifier::EvqOut :				result.qualifier |= EVariableQualifier::OutArg;		break;
				case glslang::TStorageQualifier::EvqInOut :				result.qualifier |= EVariableQualifier::InArg;
																		result.qualifier |= EVariableQualifier::OutArg;		break;
				case glslang::TStorageQualifier::EvqConstReadOnly :
				{
					glslang::TIntermSymbol*	symb = node ? node->getAsSymbolNode() : null;
					
					if ( symb and translator.fwd.funcArguments.IsExist( symb->getId() ) )
						result.qualifier |= EVariableQualifier::InArg;
					else
						result.qualifier |= EVariableQualifier::Local;

					result.qualifier |= EVariableQualifier::Constant;
					break;
				}
			}

			switch ( q.precision )
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

		if ( type.isStruct() )
		{
			auto const&	type_list = *type.getStruct();
			
			result.typeName = type.getTypeName().c_str();
			result.type		= type.getBasicType() == glslang::TBasicType::EbtBlock ?
									(type.getQualifier().storage == glslang::TStorageQualifier::EvqBuffer ?
										EShaderVariable::StorageBlock :
										EShaderVariable::UniformBlock) :
									EShaderVariable::Struct;

			result.fields.Reserve( result.fields.Count() + type_list.size() );

			FOR( i, type_list )
			{
				Translator::TypeInfo	arg;
				CHECK_ERR( ConvertType( null, *type_list[i].type, type_list[i].loc, &result, translator, OUT arg ) );
				result.fields.PushBack( RVREF(arg) );
			}
		}
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
		for (auto& field : Range(info.fields))
		{
			if ( EShaderVariable::IsStruct( field.type ) and
				 not translator.types.globalTypes.IsExist( field.typeName ) )
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
			
			for (auto& fld_name : Range(atomic_fields))
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
			Translator::TypeInfo		info;

			// skip shared
			if ( type.getQualifier().storage == glslang::TStorageQualifier::EvqShared )
				continue;

			CHECK_ERR( ConvertType( node, type, typed->getLoc(), null, translator, OUT info ) );
			
			// skip builtin
			if ( type.isBuiltIn() or IsBuiltinName( info.typeName ) ) {
				continue;
			}

			CHECK_ERR( GXCheckExternalQualifiers( translator, type.getQualifier() ) );
			
			CHECK_ERR( ApplyAtomicTypes( translator, info ) );
			
			RecursiveExtractTypesFromExternal( translator, info );

			if ( EShaderVariable::IsStruct( info.type )	and
				 translator.language->DeclExternalTypes() )
			{
				translator.types.globalTypes.Add( info.typeName, info );
				translator.types.definedInExteranal.Add( info.typeName );
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
			Translator::TypeInfo		info;

			// keep shared only
			if ( type.getQualifier().storage != glslang::TStorageQualifier::EvqShared )
				continue;

			CHECK_ERR( ConvertType( node, type, typed->getLoc(), null, translator, OUT info ) );
			
			// skip builtin
			if ( type.isBuiltIn() or IsBuiltinName( info.typeName ) ) {
				continue;
			}

			CHECK_ERR( GXCheckExternalQualifiers( translator, type.getQualifier() ) );
			
			RecursiveExtractTypesFromExternal( translator, info );

			if ( EShaderVariable::IsStruct( info.type )	and
				 translator.language->DeclExternalTypes() )
			{
				translator.types.globalTypes.Add( info.typeName, info );
				translator.types.definedInExteranal.Add( info.typeName );
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
			Translator::TypeInfo		info;

			CHECK_ERR( ConvertType( node, type, typed->getLoc(), null, translator, OUT info ) );
			
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
			translator.constants.symbNodes.Add( cu_arr, Translator::Const{ name, symb_node } );

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
		glslang::TIntermBranch*		branch	= node->getAsBranchNode();
		Translator::Node			dst_node;					dst_node.uid = uid;
		bool						replaced_return = false;

		switch ( branch->getFlowOp() )
		{
			case glslang::TOperator::EOpKill :		dst_node.src << "discard";	break;
			case glslang::TOperator::EOpBreak :		dst_node.src << "break";	break;
			case glslang::TOperator::EOpContinue :	dst_node.src << "continue";	break;
			case glslang::TOperator::EOpCase :		dst_node.src << "case";		break;
			case glslang::TOperator::EOpDefault :	dst_node.src << "default";	break;
			case glslang::TOperator::EOpReturn :
			{
				if ( translator.inl.prefixStack.Get().Empty() ) {
					dst_node.src << "return";
					break;
				}
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
			
			if ( branch->getFlowOp() == glslang::TOperator::EOpCase )
			{
				dst_node.src << ": ";
				translator.fwd.scope.Back().reqEndLine = false;
			}

			// inside inline function 'return' must be replaced by exit of cycle
			if ( replaced_return ) {
				("{\n\t"_str << translator.inl.prefixStack.Get() << "return = ") >> dst_node.src;
				dst_node.src << ";\n\tbreak;\n}";
			}
		}
		else
		// function return void, but in inline function need to break loop
		if ( replaced_return )
		{
			dst_node.src << "break";
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
		Translator::Node			dst_node;		dst_node.uid = uid;

		const uint	cond_uid	= ++translator.uid;
		const uint	body_uid	= ++translator.uid;

		CHECK_ERR( RecursiveProcessNode( sw->getCondition(), cond_uid, translator ) );
		CHECK_ERR( RecursiveProcessNode( sw->getBody(), body_uid, translator ) );
		
		const auto&	cond = translator.nodes( cond_uid );
		const auto&	body = translator.nodes( body_uid );
		
		CHECK_ERR( not cond.src.Empty() );
		CHECK_ERR( not body.src.Empty() );

		dst_node.src << "switch( " << cond.src << " )\n{\n" << body.src << "}\n";

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
		CHECK_ERR( ConvertType( node, cu->getType(), cu->getLoc(), null, translator, OUT dst_node.typeInfo ) );
		CHECK_COMP2( GXCheckAccessToExternal( translator, dst_node ) );
		
		// find in constants
		bool found = false;

		FOR( i, translator.constants.symbNodes )
		{
			glslang::TConstUnionArray const&	arr = *translator.constants.symbNodes[i].first;

			if ( cu_arr == arr )
			{
				dst_node.src << translator.constants.symbNodes[i].second.name;
				found = true;
				break;
			}
		}

		// create constant
		if ( not found )
		{
			if ( translator.constants.optimize )
			{
				String			name;
				String&			src			= translator.constants.source;
				String const	type_name	= (dst_node.typeInfo.typeName.Empty() ? ToStringGLSL( dst_node.typeInfo.type ) : dst_node.typeInfo.typeName);

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
				translator.constants.symbNodes.Add( &cu_arr, { name, null } );

				dst_node.typeInfo.name = name;
				CHECK_ERR( translator.language->TranslateLocalVar( dst_node.typeInfo, INOUT src ) );

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
		
		CHECK_ERR( ConvertType( selection, selection->getType(), selection->getLoc(), null, translator, OUT dst_node.typeInfo ) );

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

		CHECK_ERR( ConvertType( symbol, symbol->getType(), symbol->getLoc(), null, translator, OUT dst_node.typeInfo ) );
		CHECK_COMP2( GXCheckAccessToExternal( translator, dst_node ) );
		
		// add struct to global types
		if ( EShaderVariable::IsStruct( dst_node.typeInfo.type ) and
			 not dst_node.typeInfo.isGlobal and
			 not translator.types.globalTypes.IsExist( dst_node.typeInfo.typeName ) )
		{
			translator.types.globalTypes.Add( dst_node.typeInfo.typeName, dst_node.typeInfo );
		}

		// if inside inline function
		if ( dst_node.typeInfo.qualifier[ EVariableQualifier::InArg ] or
			 dst_node.typeInfo.qualifier[ EVariableQualifier::OutArg ] )
		{
			translator.inl.prefixStack.Get() >> dst_node.typeInfo.name;
			
			// replace some function arguments to external objects to support GLSL specification.
			Translator::LocalReplacer_t::iterator	iter;
			if ( translator.inl.localsReplacer.Find( dst_node.typeInfo.name, OUT iter ) ) {
				dst_node.typeInfo.name = iter->second;
			}
		}

		dst_node.uid = uid;
		
		// local variable or const
		if ( dst_node.typeInfo.qualifier[ EVariableQualifier::Local ] )
		{
			Translator::LocalVarSet_t::const_iterator	local_iter;
			
			if ( translator.fwd.definedLocalVars.Find( symbol->getId(), OUT local_iter ) )
			{
				// reuse
				dst_node.src = local_iter->second;
			}
			else
			// new variable
			{
				translator.inl.prefixStack.Get() >> dst_node.typeInfo.name;
				translator.fwd.definedLocalVars.Add( symbol->getId(), dst_node.typeInfo.name );

				// pattern: <type> <name> = ...;
				if ( root and root->getAsOperator() and root->getAsOperator()->getOp() == glslang::TOperator::EOpAssign )
				{
					CHECK_ERR( translator.language->TranslateLocalVar( dst_node.typeInfo, OUT dst_node.src ) );
				}
				else
				{
					translator.fwd.scope.Back().pendingVars.Add( symbol->getId(), dst_node.typeInfo );
					dst_node.src = dst_node.typeInfo.name;
				}
			}
		}
		else
		{
			// builtin variable/const may be replaced by target language implementation
			CHECK_ERR( translator.language->TranslateName( dst_node.typeInfo, OUT dst_node.src ) );
		}

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

		CHECK_ERR( ConvertType( node, unary->getType(), unary->getLoc(), null, translator, OUT dst_node.typeInfo ) );
		CHECK_COMP2( GXCheckAccessToExternal( translator, dst_node ) );

		CHECK_ERR( RecursiveProcessNode( unary->getOperand(), arg_uid, translator ) );

		const auto&		arg_node = translator.nodes( arg_uid );
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

		const auto &		lhs = translator.nodes( lhs_uid );
		const auto &		rhs = translator.nodes( rhs_uid );
		Translator::Node	dst_node;

		dst_node.uid = uid;
		CHECK_ERR( ConvertType( node, binary->getType(), binary->getLoc(), null, translator, OUT dst_node.typeInfo ) );
		CHECK_ERR( not lhs.src.Empty() and not rhs.src.Empty() );
		
		CHECK_COMP2( GXCheckAccessToExternal( translator, dst_node ) );

		CHECK_ERR( translator.language->TranslateOperator( binary->getOp(), dst_node.typeInfo, { lhs.src, rhs.src }, { &lhs.typeInfo, &rhs.typeInfo }, INOUT dst_node.src ) );

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
			
			Translator::TypeInfo	var_type;

			FOR( i, aggr->getSequence() )
			{
				if ( glslang::TIntermBinary* binary = aggr->getSequence()[i]->getAsBinaryNode() )
				{
					Translator::TypeInfo	symb_type;
					CHECK_ERR( ConvertType( binary, binary->getType(), binary->getLoc(), null, translator, OUT symb_type ) );

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

					glslang::TIntermSymbol* symbol = binary->getLeft()->getAsSymbolNode();
					CHECK_ERR( symbol );
					
					translator.fwd.definedLocalVars.Add( symbol->getId(), symbol->getName().c_str() );

					if ( i > 0 )
						variables_src << ',';

					variables_src << ' ' << symbol->getName().c_str();

					const uint	arg_uid = ++translator.uid;
					CHECK_ERR( RecursiveProcessNode( binary->getRight(), arg_uid, translator ) );

					const auto&	arg_node = translator.nodes( arg_uid );
					CHECK_ERR( not arg_node.src.Empty() );

					variables_src << " = " << arg_node.src;
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

		const uint	body_uid = ++translator.uid;
		CHECK_ERR( RecursiveProcessNode( loop->getBody(), body_uid, translator ) );

		loop_src = translator.nodes( body_uid ).src;
		CHECK_ERR( not loop_src.Empty() );
		
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

		StringParser::IncreaceIndent( loop_src );

		if ( loop->testFirst() )
		{
			// 'for'
			if ( not terminal_src.Empty() )
			{
				loop_src = "for("_str << variables_src << "; " << test_src << "; " << terminal_src << ")\n{\n" << loop_src << "}\n";
			}
			else
			// 'while'
			{
				CHECK_ERR( variables_src.Empty() );
				loop_src = "while( "_str << test_src << ")\n{\n" << loop_src << "};\n";
			}
		}
		else
		{
			// 'do-while'
			CHECK_ERR( terminal_src.Empty() and variables_src.Empty() );

			loop_src = "do {\n"_str << loop_src << "} while( " << test_src << ");\n";
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
		glslang::TIntermBinary*	binary = node->getAsBinaryNode();
		Translator::Node		dst_node;		dst_node.uid = uid;
		String					sw_mask;

		CHECK_ERR( binary and (binary->getOp() == glslang::TOperator::EOpVectorSwizzle or binary->getOp() == glslang::TOperator::EOpIndexDirect) );

		const uint	lhs_uid	= ++translator.uid;
		CHECK_ERR( RecursiveProcessNode( binary->getLeft(), lhs_uid, translator ) );
		
		const auto&	lhs = translator.nodes( lhs_uid );
		CHECK_ERR( not lhs.src.Empty() );

		const auto ProcessUnion = LAMBDA(&sw_mask) (TIntermNode *node) -> bool
		{
			glslang::TIntermConstantUnion*		cu		= node->getAsConstantUnion();
			glslang::TConstUnionArray const&	cu_arr	= cu->getConstArray();

			CHECK_ERR( cu_arr.size() == 1 and cu->getType().getBasicType() == glslang::EbtInt );
			CHECK_ERR( cu_arr[0].getType() == glslang::EbtInt and cu_arr[0].getIConst() >= 0 and cu_arr[0].getIConst() < 4 );

			const char xyzw[] = "xyzw";

			sw_mask << xyzw[ cu_arr[0].getIConst() ];
			return true;
		};

		if ( binary->getRight()->getAsAggregate() )
		{
			glslang::TIntermAggregate*	aggr = binary->getRight()->getAsAggregate();
			CHECK_ERR( aggr->getOp() == glslang::TOperator::EOpSequence );

			FOR( i, aggr->getSequence() )
			{
				CHECK_ERR( ProcessUnion( aggr->getSequence()[i] ) );
			}
		}
		else
		if ( binary->getRight()->getAsConstantUnion() )
		{
			CHECK_ERR( ProcessUnion( binary->getRight() ) );
		}
		else
			RETURN_ERR( "not supported!" );

		CHECK_ERR( translator.language->TranslateSwizzle( lhs.typeInfo, lhs.src, sw_mask, INOUT dst_node.src ) );

		translator.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	TranslateIndexDirectStruct
=================================================
*/
	static bool TranslateIndexDirectStruct (glslang::TIntermOperator* node, const uint uid, Translator &translator)
	{
		glslang::TIntermBinary*	binary = node->getAsBinaryNode();
		Translator::Node		dst_node;
		
		CHECK_ERR( ConvertType( node, binary->getType(), binary->getLoc(), null, translator, OUT dst_node.typeInfo ) );

		CHECK_ERR( binary and binary->getOp() == glslang::TOperator::EOpIndexDirectStruct );
		CHECK_ERR( binary->getLeft()->getType().isStruct() and binary->getRight()->getAsConstantUnion() );
		
		const auto&	st_type = *binary->getLeft()->getType().getStruct();

		const uint	lhs_uid	= ++translator.uid;
		CHECK_ERR( RecursiveProcessNode( binary->getLeft(), lhs_uid, translator ) );
		
		const auto&	lhs = translator.nodes( lhs_uid );

		glslang::TIntermConstantUnion*		cu		= binary->getRight()->getAsConstantUnion();
		glslang::TConstUnionArray const&	cu_arr	= cu->getConstArray();
		
		// constant union must be correct index of struct field
		CHECK_ERR( cu_arr.size() == 1 and (cu->getType().getBasicType() == glslang::EbtInt or cu->getType().getBasicType() == glslang::EbtUint) );
		CHECK_ERR( (cu_arr[0].getType() == glslang::EbtInt or cu_arr[0].getType() == glslang::EbtUint) and
				   cu_arr[0].getIConst() >= 0 and cu_arr[0].getIConst() < int(st_type.size()) );
		
		dst_node.uid = uid;
		//dst_node.src << (lhs.src.Empty() ? "" : lhs.src + ".") << st_type[ cu_arr[0].getIConst() ].type->getFieldName().c_str();
		
		Translator::TypeInfo	field_info;
		CHECK_ERR( ConvertType( null, *st_type[ cu_arr[0].getIConst() ].type, binary->getLoc(), null, translator, OUT field_info ) );

		CHECK_ERR( translator.language->TranslateStructAccess( lhs.typeInfo, lhs.src, field_info, OUT dst_node.src ) );

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
		CHECK_ERR( ConvertType( cu, cu->getType(), cu->getLoc(), null, translator, OUT dst_node.typeInfo ) );

		FOR( i, translator.constants.symbNodes )
		{
			glslang::TConstUnionArray const&	arr = *translator.constants.symbNodes[i].first;

			if ( cu_arr == arr )
			{
				dst_node.src << translator.constants.symbNodes[i].second.name;
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

		String	func_name = aggr->getName().c_str();

		if ( translator.states.useGXrules or translator.states.inlineAll )
		{
			Translator::InlFunctionsMap_t::iterator	iter;
			if ( translator.inl.functions.Find( func_name, OUT iter ) )
				return TranslateInlineFunctionCall( node, uid, iter->second, translator );
		}

		Translator::Node						dst_node;
		Array< uint >							args_uids;
		Array< String >							args_src;
		Array< Translator::TypeInfo const* >	args_types;

		// get result type
		CHECK_ERR( ConvertType( node, aggr->getType(), aggr->getLoc(), null, translator, OUT dst_node.typeInfo ) );
		
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

		CHECK_ERR( translator.language->TranslateFunctionCall( func_name, dst_node.typeInfo, args_src, args_types, OUT dst_node.src ) );

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
		
		const String			prefix	= UidToPrefix( uid, translator );
		Translator::Node		dst_node;
		String					temp;
	
		dst_node.uid = uid;
		
		temp << "\n// " << aggr->getName().c_str() << "\n";

		// decl args
		{
			Translator::TypeInfo			func_result;
			Array< Translator::TypeInfo >	func_args;
			glslang::TIntermAggregate*		params	= func->getSequence()[0]->getAsAggregate();

			CHECK_ERR( ConvertType( func, func->getType(), func->getLoc(), null, translator, OUT func_result ) );
			CHECK_ERR( params and params->getOp() == glslang::TOperator::EOpParameters );
			CHECK_ERR( params->getSequence().size() == aggr->getSequence().size() );

			FOR( i, aggr->getSequence() )
			{
				// get argument type
				Translator::TypeInfo		param_type;
				glslang::TIntermTyped *		nn		= params->getSequence()[i]->getAsTyped();
				glslang::TIntermSymbol *	symbol	= nn->getAsSymbolNode();

				CHECK_ERR( ConvertType( nn, nn->getType(), nn->getLoc(), null, translator, OUT param_type ) );
				
				prefix >> param_type.name;

				if ( symbol ) {
					translator.fwd.funcArguments.Add( symbol->getId(), param_type.name );
				}

				// get argument value
				const uint	arg_uid = ++translator.uid;
				CHECK_ERR( RecursiveProcessNode( aggr->getSequence()[i], arg_uid, translator ) );

				const auto&	arg = translator.nodes( arg_uid );
				CHECK_ERR( not arg.src.Empty() );
				
				// external types must be replaced, no temporary variables
				if ( param_type.arraySize > 0 or
					 arg.typeInfo.qualifier[ EVariableQualifier::Shared ] or
					 EShaderVariable::IsStruct( param_type.type ) or
					 EShaderVariable::IsTexture( param_type.type ) or
					 EShaderVariable::IsImage( param_type.type ) )
				{
					translator.inl.localsReplacer.Add( param_type.name, arg.src );
					continue;
				}
				
				// build source
				CHECK_ERR( translator.language->TranslateLocalVar( param_type, INOUT temp ) );
				temp << " = " << arg.src << ";\n";
			}

			// add result type
			if ( func_result.type != EShaderVariable::Void )
			{
				func_result.name = prefix + "return";
				CHECK_ERR( translator.language->TranslateLocalVar( func_result, INOUT temp ) );
				temp << " = ";
				CHECK_ERR( translator.language->TranslateType( func_result, INOUT temp ) );
				temp << "(0);\n";		// TODO
				dst_node.src << func_result.name;
			}
			else
				dst_node.src << " ";	// to avoid empty string checking
		}


		// place function in scope
		{
			translator.inl.prefixStack.Push( prefix );

			glslang::TIntermAggregate*	body		= func->getSequence()[1]->getAsAggregate();
			const uint					body_uid	= ++translator.uid;
			
			CHECK_ERR( RecursiveProcessNode( body, body_uid, translator ) );

			String		body_src = translator.nodes( body_uid ).src;
			CHECK_ERR( not body_src.Empty() );
			
			StringParser::IncreaceIndent( INOUT body_src );

			temp << "for (int " << prefix << " = 0; " << prefix << " == 0; ++" << prefix << ")\n{\n" << body_src << "\n}";

			ASSERT( prefix == translator.inl.prefixStack.Get() );
			translator.inl.prefixStack.Pop();
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
		CHECK_ERR( ConvertType( node, op->getType(), op->getLoc(), null, translator, OUT dst_node.typeInfo ) );
		
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

		if ( glslang::TIntermAggregate* aggr = node->getAsAggregate() )
		{
			switch ( aggr->getOp() )
			{
				case glslang::TOperator::EOpLinkerObjects :
					return true;

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
		const String					func_name	= aggr->getName().c_str();
		Array< Translator::TypeInfo >	func_args;

		CHECK_ERR( aggr and aggr->getOp() == glslang::TOperator::EOpFunction );
		
		Translator::TypeInfo	ret_type;
		CHECK_ERR( ConvertType( aggr, aggr->getType(), aggr->getLoc(), null, translator, OUT ret_type ) );
		
		// find arguments
		FOR( i, aggr->getSequence() )
		{
			TIntermNode* n = aggr->getSequence()[i];

			if ( n->getAsAggregate() and n->getAsAggregate()->getOp() == glslang::TOperator::EOpParameters )
			{
				// get arguemnts
				glslang::TIntermAggregate*	args_node = n->getAsAggregate();
				
				func_args.Reserve( func_args.Count() + args_node->getSequence().size() );

				FOR( j, args_node->getSequence() )
				{
					Translator::TypeInfo		arg;
					glslang::TIntermTyped *		nn	= args_node->getSequence()[j]->getAsTyped();

					CHECK_ERR( ConvertType( nn, nn->getType(), nn->getLoc(), null, translator, OUT arg ) );

					func_args.PushBack( RVREF(arg) );
				}
			}
		}

		// add function declaration to cache
		String	str;
		CHECK_ERR( translator.language->TranslateFunctionDecl( func_name, ret_type, func_args, OUT str ) );

		return true;
	}

}	// PipelineCompiler
