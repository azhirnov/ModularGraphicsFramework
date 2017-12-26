// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_Utils.h"
#include "Engine/PipelineCompiler/Common/ToGLSL.h"

namespace PipelineCompiler
{

	struct CLSource
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
			uint						arraySize	= 1;
			uint						binding		= UMax;

		// methods
			TypeInfo() {}

			String ToString () const;
			String ArgToString () const;
		};

		struct Node
		{
			TypeInfo		typeInfo;
			uint			uid		= 0;
			String			src;
		};

		using NodeMap_t		= Map< uint, Node >;
		using LocalVarSet_t	= Set< uint >;
		using PendingVars_t	= Map< uint, TypeInfo >;

	// variables
		String				src;
		NodeMap_t			nodes;
		LocalVarSet_t		definedLocalVars;
		PendingVars_t		pendingVars;
		Array< TypeInfo >	externalObjects;
		String				entryPoint;
		uint				uid	= 0;		// counter
	};


	static bool RecursiveProcessAggregateNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool RecursiveProcessNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool RecursiveProcessBranchNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool RecursiveProcessSwitchNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool RecursiveProcessConstUnionNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool RecursiveProcessSelectionNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool RecursiveProcessMethodNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool RecursiveProcessSymbolNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool RecursiveProcessTypedNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool RecursiveProcessOperatorNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool RecursiveProcessUnaryNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool RecursiveProcessBinaryNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	//static bool ReplaceTypesInNode (TIntermNode* root, glslang::TIntermBinary* node, CLSource &clSource);
	static bool TranslateFunction (glslang::TIntermAggregate* aggr, const uint uid, CLSource &clSource);
	static bool TranslateExternalObjects (glslang::TIntermAggregate* aggr, CLSource &clSource);
	static bool ConvertType (TIntermNode* node, glslang::TType const &type, glslang::TSourceLoc const &loc, const CLSource::TypeInfo *parent, OUT CLSource::TypeInfo &typeInfo);
	static bool TranslateBuiltinFunctionCall (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool TranslateVectorSwizzle (TIntermNode* root, glslang::TIntermOperator* node, const uint uid, CLSource &clSource);
	static bool TranslateConstructorCall (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool TranslateImageFuncCall (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool TranslateFunctionCall (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource);
	static bool TranslateMain (TIntermNode* root, const uint uid, CLSource &clSource);

/*
=================================================
	_TranslateGLSLtoCL
=================================================
*/
	bool ShaderCompiler::_TranslateGXSLtoCL (const Config &cfg, const _GLSLangResult &glslangData, OUT String &log, OUT BinaryArray &result) const
	{
		const glslang::TIntermediate* intermediate = glslangData.prog.getIntermediate( glslangData.shader->getStage() );
		CHECK_ERR( intermediate );

		TIntermNode*	root	= intermediate->getTreeRoot();
		CLSource		cl_src;

		cl_src.entryPoint = intermediate->getEntryPointName().c_str();

		CHECK_ERR( TranslateMain( root, cl_src.uid, OUT cl_src ) );

		result = BinArrayCRef::From( cl_src.src );
		return true;
	}
	
/*
=================================================
	_CompileCL
=================================================
*/
	bool ShaderCompiler::_CompileCL (const Config &cfg, const _ShaderData &shader, OUT String &log, OUT BinaryArray &result) const
	{
		TODO( "_CompileCL" );
		return true;
	}

/*
=================================================
	ToString
=================================================
*/
	String CLSource::TypeInfo::ToString () const
	{
		String	res;

		// access
		if ( memoryModel != EGpuMemoryModel::None )
			res << ToStringGLSL( memoryModel ) << ' ';

		// layout
		if ( format != EPixelFormat::Unknown )
			res << "layout(" << ToStringGLSL( format ) << ") ";

		// precision
		if ( precision != EPrecision::Default )
			res << ToStringGLSL( precision ) << ' ';

		// type
		if ( not typeName.Empty() ) {
			res << typeName;
		} else {
			res << ToStringGLSL( type );
		}

		res << (arraySize == 1 ? "" : (arraySize == 0 ? "[]" : "["_str << arraySize << "]"));

		//ASSERT( qualifier.IsZero() );
		return res;
	}
	
/*
=================================================
	ArgToString
=================================================
*/
	String CLSource::TypeInfo::ArgToString () const
	{
		String	res;

		// qualifies
		if ( qualifier[ EVariableQualifier::Constant ] )
			res << "const ";
		else
		if ( qualifier[ EVariableQualifier::InArg ] and qualifier[ EVariableQualifier::OutArg ] )
			res << "inout ";
		else
		if ( qualifier[ EVariableQualifier::InArg ] )
			res << "in ";
		else
		if ( qualifier[ EVariableQualifier::OutArg ] )
			res << "out ";

		return (res << ToString());
	}

/*
=================================================
	TranslateMain
=================================================
*/
	static bool TranslateMain (TIntermNode* root, const uint uid, CLSource &clSource)
	{
		glslang::TIntermAggregate* aggr = root->getAsAggregate();

		CHECK_ERR( aggr and aggr->getOp() == glslang::TOperator::EOpSequence );
		
		// get external objects
		for (size_t i = 0; i < aggr->getSequence().size(); ++i)
		{
			TIntermNode *				node = aggr->getSequence()[i];
			glslang::TIntermAggregate*	aggr2 = node->getAsAggregate();

			if ( aggr2 and aggr2->getOp() == glslang::TOperator::EOpLinkerObjects )
			{
				CHECK_ERR( TranslateExternalObjects( aggr2, clSource ) );
				break;
			}
		}

		for (size_t i = 0; i < aggr->getSequence().size(); ++i)
		{
			TIntermNode*	node = aggr->getSequence()[i];

			if ( node->getAsAggregate() and node->getAsAggregate()->getOp() == glslang::TOperator::EOpLinkerObjects )
				continue;

			const uint	arg_uid = ++clSource.uid;
			CHECK_ERR( RecursiveProcessNode( aggr, node, arg_uid, clSource ) );

			const auto&	arg = clSource.nodes( arg_uid );
			CHECK_ERR( not arg.src.Empty() );

			clSource.src << arg.src << "\n";
		}
		return true;
	}

/*
=================================================
	RecursiveProcessAggregateNode
=================================================
*/
	static bool RecursiveProcessAggregateNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermAggregate* aggr = node->getAsAggregate();
		
		// continue
		if ( aggr->getOp() == glslang::TOperator::EOpSequence )
		{
			CLSource::Node	dst_node;
			dst_node.uid = uid;

			for (size_t i = 0; i < aggr->getSequence().size(); ++i)
			{
				const uint	arg_uid = ++clSource.uid;
				CHECK_ERR( RecursiveProcessNode( aggr, aggr->getSequence()[i], arg_uid, clSource ) );

				// add local variable declaration
				FOR( j, clSource.pendingVars ) {
					dst_node.src << clSource.pendingVars[j].second.ToString() << " " << clSource.pendingVars[j].second.name << ";\n";
				}
				clSource.pendingVars.Clear();

				const auto&	arg = clSource.nodes( arg_uid );
				CHECK_ERR( not arg.src.Empty() );

				dst_node.src << arg.src << (arg.src.EndsWith(";\n") ? "" : ";\n");
			}

			clSource.nodes.Add( uid, RVREF(dst_node) );
			return true;
		}

		// function definition
		if ( aggr->getOp() == glslang::TOperator::EOpFunction )
		{
			CHECK_ERR( TranslateFunction( aggr, uid, clSource ) );
			return true;
		}

		// uniforms, buffers, images, ...
		if ( aggr->getOp() == glslang::TOperator::EOpLinkerObjects )
		{
			RETURN_ERR( "not supported here, must be used in 'TranslateMain'" );
		}

		// call constructor
		if ( aggr->getOp() > glslang::TOperator::EOpConstructGuardStart and
			 aggr->getOp() < glslang::TOperator::EOpConstructGuardEnd )
		{
			CHECK_ERR( TranslateConstructorCall( root, node, uid, clSource ) );
			return true;
		}

		// call image functions
		if ( aggr->getOp() > glslang::TOperator::EOpImageGuardBegin and
			 aggr->getOp() < glslang::TOperator::EOpImageGuardEnd )
		{
			CHECK_ERR( TranslateImageFuncCall( root, node, uid, clSource ) );
			return true;
		}

		// function call
		if ( aggr->getOp() == glslang::TOperator::EOpFunctionCall )
		{
			CHECK_ERR( TranslateFunctionCall( root, node, uid, clSource ) );
			return true;
		}

		// builtin function call
		if ( aggr->getOp() >= glslang::TOperator::EOpRadians and
			 aggr->getOp() <= glslang::TOperator::EOpAll )
		{
			CHECK_ERR( TranslateBuiltinFunctionCall( root, node, uid, clSource ) );
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
	static bool TranslateFunction (glslang::TIntermAggregate* aggr, const uint uid, CLSource &clSource)
	{
		ASSERT( aggr->getOp() == glslang::TOperator::EOpFunction );

		CLSource::Node				dst_node;
		Array< CLSource::TypeInfo >	func_args;
		String						func_name;
		const uint					body_uid = ++clSource.uid;

		// get result type
		CHECK_ERR( ConvertType( aggr, aggr->getType(), aggr->getLoc(), null, OUT dst_node.typeInfo ) );
		
		// get name
		{
			String	signature	= aggr->getName().c_str();
			usize	pos			= 0;

			if ( signature.Find( '(', OUT pos ) )
				func_name = signature.SubString( 0, pos );
			else
				func_name = signature;
		}
		
		// find arguments
		for (size_t i = 0; i < aggr->getSequence().size(); ++i)
		{
			TIntermNode* n = aggr->getSequence()[i];

			if ( n->getAsAggregate() and 
				 n->getAsAggregate()->getOp() == glslang::TOperator::EOpParameters )
			{
				// get arguemnts
				glslang::TIntermAggregate*	args_node = n->getAsAggregate();
				
				func_args.Reserve( func_args.Count() + args_node->getSequence().size() );

				for (size_t j = 0; j < args_node->getSequence().size(); ++j)
				{
					CLSource::TypeInfo			arg;
					glslang::TIntermTyped *		nn = args_node->getSequence()[j]->getAsTyped();

					CHECK_ERR( ConvertType( nn, nn->getType(), nn->getLoc(), null, OUT arg ) );

					func_args.PushBack( RVREF(arg) );
				}
			}
			else
			{
				CHECK_ERR( RecursiveProcessNode( aggr, n, body_uid, clSource ) );
			}
		}

		const bool	is_entry = (func_name == clSource.entryPoint);
		String		body	 = clSource.nodes( body_uid ).src;

		CHECK_ERR( not body.Empty() );
		dst_node.src << dst_node.typeInfo.ToString() << " " << func_name << " (";

		if ( is_entry )
		{
			CHECK_ERR( func_args.Empty() );

			FOR( i, clSource.externalObjects )
			{
				const auto&	obj = clSource.externalObjects[i];

				dst_node.src << (i ? ", " : "") << obj.ToString() << " " << obj.name;
			}
		}

		FOR( i, func_args ) {
			dst_node.src << (i ? ", " : "") << func_args[i].ArgToString() << ' ' << func_args[i].name;
		}

		StringParser::IncreaceIndent( INOUT body );
		dst_node.src << ")\n{\n" << body << "}\n\n";

		clSource.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	ConvertType
=================================================
*/
	static bool ConvertType (TIntermNode* node, glslang::TType const &type, glslang::TSourceLoc const &loc, const CLSource::TypeInfo *parent, OUT CLSource::TypeInfo &result)
	{
		auto const&		qual = type.getQualifier();

		switch ( qual.storage )
		{
			case glslang::TStorageQualifier::EvqIn :			// argument
			case glslang::TStorageQualifier::EvqOut :			// argument
			case glslang::TStorageQualifier::EvqInOut :			// argument
			case glslang::TStorageQualifier::EvqConstReadOnly :	// argument
			case glslang::TStorageQualifier::EvqGlobal :		// result
			case glslang::TStorageQualifier::EvqTemporary :		// variable
			case glslang::TStorageQualifier::EvqVaryingIn :		// shader global variable in
			case glslang::TStorageQualifier::EvqVaryingOut :	// shader global variable out
			case glslang::TStorageQualifier::EvqUniform :
			case glslang::TStorageQualifier::EvqBuffer :
			case glslang::TStorageQualifier::EvqShared :
			case glslang::TStorageQualifier::EvqConst :
			{
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
					result.arraySize = Max( 0, type.getOuterArraySize() );
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
					 type.getBasicType() == glslang::TBasicType::EbtFloat	or
					 type.getBasicType() == glslang::TBasicType::EbtDouble	or
					 type.getBasicType() == glslang::TBasicType::EbtInt		or
					 type.getBasicType() == glslang::TBasicType::EbtUint	or
					 type.getBasicType() == glslang::TBasicType::EbtInt64	or
					 type.getBasicType() == glslang::TBasicType::EbtUint64	or
					#ifdef AMD_EXTENSIONS
					 type.getBasicType() == glslang::TBasicType::EbtFloat16	or
					 type.getBasicType() == glslang::TBasicType::EbtInt16	or
					 type.getBasicType() == glslang::TBasicType::EbtUint16	or
					#endif
					 type.getBasicType() == glslang::TBasicType::EbtBool	or
					 type.getBasicType() == glslang::TBasicType::EbtUint64 )
				{
					result.type = ConvertBasicType( type.getBasicType(), type.getVectorSize(), type.getMatrixCols(), type.getMatrixRows() );
				}

				// qualifiers
				{
					auto const&	q = type.getQualifier();
			
					if ( type.isBuiltIn()					or
						 result.typeName.StartsWith("gl_")	or
						 result.name.StartsWith("gl_")		or
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

					if ( q.coherent )
						result.memoryModel |= EGpuMemoryModel::Coherent;

					if ( q.volatil )
						result.memoryModel |= EGpuMemoryModel::Volatile;

					if ( q.restrict )
						result.memoryModel |= EGpuMemoryModel::Restrict;

					if ( q.readonly )
						result.memoryModel |= EGpuMemoryModel::ReadOnly;

					if ( q.writeonly )
						result.memoryModel |= EGpuMemoryModel::WriteOnly;

					if ( q.specConstant )
						result.qualifier |= EVariableQualifier::Specialization;

					switch ( q.storage )
					{
						case glslang::TStorageQualifier::EvqTemporary :			result.qualifier |= EVariableQualifier::Local;		break;
						//case glslang::TStorageQualifier::EvqGlobal :			break;
						case glslang::TStorageQualifier::EvqConst :				result.qualifier |= EVariableQualifier::Constant;	break;
						case glslang::TStorageQualifier::EvqVaryingIn :			result.qualifier |= EVariableQualifier::In;			break;
						case glslang::TStorageQualifier::EvqVaryingOut :		result.qualifier |= EVariableQualifier::Out;		break;
						case glslang::TStorageQualifier::EvqUniform :			result.qualifier |= EVariableQualifier::Uniform;	break;
						//case glslang::TStorageQualifier::EvqBuffer :			break;
						case glslang::TStorageQualifier::EvqShared :			result.qualifier |= EVariableQualifier::Shared;		break;

						// function arguments
						case glslang::TStorageQualifier::EvqIn :				result.qualifier |= EVariableQualifier::InArg;		break;
						case glslang::TStorageQualifier::EvqOut :				result.qualifier |= EVariableQualifier::OutArg;		break;
						case glslang::TStorageQualifier::EvqInOut :				result.qualifier |= EVariableQualifier::InArg;
																				result.qualifier |= EVariableQualifier::OutArg;		break;
						case glslang::TStorageQualifier::EvqConstReadOnly :		result.qualifier |= EVariableQualifier::InArg;
																				result.qualifier |= EVariableQualifier::Constant;	break;
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
					result.qualifier.Or( EVariableQualifier::Specialization, parent->qualifier[EVariableQualifier::Specialization] );

					if ( result.memoryModel == EGpuMemoryModel::None )
						result.memoryModel = parent->memoryModel;

					if ( result.precision == EPrecision::Default )
						result.precision = parent->precision;
				}

				if ( type.isStruct() )
				{
					auto const&	type_list = *type.getStruct();
			
					result.type		= EShaderVariable::Struct;
					result.typeName = type.getTypeName().c_str();
					result.fields.Reserve( result.fields.Count() + type_list.size() );

					for (size_t i = 0; i < type_list.size(); ++i)
					{
						CLSource::TypeInfo	arg;
						CHECK_ERR( ConvertType( null, *type_list[i].type, type_list[i].loc, &result, OUT arg ) );
						result.fields.PushBack( RVREF(arg) );
					}
				}
				return true;
			}
			default :
				RETURN_ERR( "unknown function argument qualifier!" );
		}
	}

/*
=================================================
	TranslateExternalObjects
=================================================
*/
	static bool TranslateExternalObjects (glslang::TIntermAggregate* aggr, CLSource &clSource)
	{
		CHECK_ERR( aggr and aggr->getOp() == glslang::TOperator::EOpLinkerObjects );

		for (size_t i = 0; i < aggr->getSequence().size(); ++i)
		{
			TIntermNode*	node = aggr->getSequence()[i];

			if ( node->getAsTyped() )
			{
				glslang::TIntermTyped*	typed = node->getAsTyped();
				CLSource::TypeInfo		info;

				CHECK_ERR( ConvertType( node, typed->getType(), typed->getLoc(), null, OUT info ) );

				clSource.externalObjects.PushBack( RVREF(info) );
			}
		}
		return true;
	}

/*
=================================================
	RecursiveProcessNode
=================================================
*/
	static bool RecursiveProcessNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		if ( not node )
			return true;

		if ( node->getAsAggregate() )
		{
			CHECK_ERR( RecursiveProcessAggregateNode( root, node, uid, clSource ) );
			return true;
		}

		if ( node->getAsUnaryNode() )
		{
			CHECK_ERR( RecursiveProcessUnaryNode( root, node, uid, clSource ) );
			return true;
		}

		if ( node->getAsBinaryNode() )
		{
			CHECK_ERR( RecursiveProcessBinaryNode( root, node, uid, clSource ) );
			return true;
		}

		if ( node->getAsOperator() )
		{
			CHECK_ERR( RecursiveProcessOperatorNode( root, node, uid, clSource ) );
			return true;
		}

		if ( node->getAsBranchNode() )
		{
			CHECK_ERR( RecursiveProcessBranchNode( root, node, uid, clSource ) );
			return true;
		}

		if ( node->getAsSwitchNode() )
		{
			CHECK_ERR( RecursiveProcessSwitchNode( root, node, uid, clSource ) );
			return true;
		}

		if ( node->getAsConstantUnion() )
		{
			CHECK_ERR( RecursiveProcessConstUnionNode( root, node, uid, clSource ) );
			return true;
		}

		if ( node->getAsSelectionNode() )
		{
			CHECK_ERR( RecursiveProcessSelectionNode( root, node, uid, clSource ) );
			return true;
		}

		if ( node->getAsMethodNode() )
		{
			CHECK_ERR( RecursiveProcessMethodNode( root, node, uid, clSource ) );
			return true;
		}

		if ( node->getAsSymbolNode() )
		{
			CHECK_ERR( RecursiveProcessSymbolNode( root, node, uid, clSource ) );
			return true;
		}

		if ( node->getAsTyped() )
		{
			CHECK_ERR( RecursiveProcessTypedNode( root, node, uid, clSource ) );
			return true;
		}
		
		TODO( "" );
		return false;
	}
	
/*
=================================================
	RecursiveProcessBranchNode
=================================================
*/
	static bool RecursiveProcessBranchNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermBranch*		branch	= node->getAsBranchNode();
		CLSource::Node				dst_node;	dst_node.uid = uid;

		switch ( branch->getFlowOp() )
		{
			case glslang::TOperator::EOpKill :		dst_node.src << "discard";	break;
			case glslang::TOperator::EOpReturn :	dst_node.src << "return";	break;
			case glslang::TOperator::EOpBreak :		dst_node.src << "break";	break;
			case glslang::TOperator::EOpContinue :	dst_node.src << "continue";	break;
			case glslang::TOperator::EOpCase :		dst_node.src << "case";		break;
			case glslang::TOperator::EOpDefault :	dst_node.src << "default";	break;
			default :								RETURN_ERR( "unknown operator!" );
		}

		if ( branch->getExpression() )
		{
			const uint	arg_uid = ++clSource.uid;
			CHECK_ERR( RecursiveProcessNode( branch, branch->getExpression(), arg_uid, clSource ) );

			const auto&	arg = clSource.nodes( arg_uid );
			CHECK_ERR( not arg.src.Empty() );

			dst_node.src << " " << arg.src;
		}

		clSource.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	RecursiveProcessSwitchNode
=================================================
*/
	static bool RecursiveProcessSwitchNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermSwitch*		sw = node->getAsSwitchNode();

		CHECK_ERR( RecursiveProcessNode( sw, sw->getCondition(), ++clSource.uid, clSource ) );
		CHECK_ERR( RecursiveProcessNode( sw, sw->getBody(), ++clSource.uid, clSource ) );

		TODO( "" );
		return true;
	}
	
/*
=================================================
	RecursiveProcessConstUnionNode
=================================================
*/
	struct CU_ToString_Func
	{
		String &	str;

		CU_ToString_Func (String &str) : str(str) {}

		template <typename T>
		void operator () (const T &value) const {
			str << "(" << ToString( value ) << ")";
		}
		
		template <typename T, usize I>
		void operator () (const Vec<T,I> &value) const
		{
			if ( All( value == value.x ) )
				str << "(" << ToString( value.x ) << ")";
			else
				str << "(" << ToString( value ) << ")";
		}
	};

	static bool RecursiveProcessConstUnionNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermConstantUnion*		cu		= node->getAsConstantUnion();
		const glslang::TConstUnionArray&	cu_arr	= cu->getConstArray();
		CLSource::Node						dst_node;
		
		dst_node.uid = uid;
		CHECK_ERR( ConvertType( node, cu->getType(), cu->getLoc(), null, OUT dst_node.typeInfo ) );

		DeserializedShader::Constant::ValueArray_t	values;
		CHECK_ERR( DeserializeConstant::Process( dst_node.typeInfo.type, cu_arr, OUT values ) );
		
		CU_ToString_Func	func( dst_node.src );

		FOR( i, values )
		{
			dst_node.src << (i ? ", " : "") << dst_node.typeInfo.ToString();
			values[i].Apply( func );
		}

		clSource.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	RecursiveProcessSelectionNode
=================================================
*/
	static bool RecursiveProcessSelectionNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermSelection*	selection = node->getAsSelectionNode();

		CHECK_ERR( RecursiveProcessNode( selection, selection->getCondition(), ++clSource.uid, clSource ) );
		CHECK_ERR( RecursiveProcessNode( selection, selection->getTrueBlock(), ++clSource.uid, clSource ) );
		CHECK_ERR( RecursiveProcessNode( selection, selection->getFalseBlock(), ++clSource.uid, clSource ) );
		
		TODO( "" );
		return true;
	}
	
/*
=================================================
	RecursiveProcessMethodNode
=================================================
*/
	static bool RecursiveProcessMethodNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermMethod*		method = node->getAsMethodNode();

		CHECK_ERR( RecursiveProcessNode( method, method->getObject(), ++clSource.uid, clSource ) );
		
		TODO( "" );
		return true;
	}
	
/*
=================================================
	RecursiveProcessSymbolNode
=================================================
*/
	static bool RecursiveProcessSymbolNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermSymbol*		symbol = node->getAsSymbolNode();
		CLSource::Node				dst_node;

		CHECK_ERR( ConvertType( symbol, symbol->getType(), symbol->getLoc(), null, OUT dst_node.typeInfo ) );

		dst_node.uid	= uid;
		dst_node.src	= symbol->getName().c_str();

		if ( symbol->getQualifier().storage == glslang::TStorageQualifier::EvqTemporary and
			 not clSource.definedLocalVars.IsExist( symbol->getId() ) )
		{
			clSource.definedLocalVars.Add( symbol->getId() );
			clSource.pendingVars.Add( symbol->getId(), dst_node.typeInfo );
		}

		clSource.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	RecursiveProcessTypedNode
=================================================
*/
	static bool RecursiveProcessTypedNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermTyped*		typed = node->getAsTyped();

		TODO( "" );
		return true;
	}
	
/*
=================================================
	RecursiveProcessOperatorNode
=================================================
*/
	static bool RecursiveProcessOperatorNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermOperator*	op = node->getAsOperator();
		
		TODO( "" );
		return true;
	}
	
/*
=================================================
	RecursiveProcessUnaryNode
=================================================
*/
	static bool RecursiveProcessUnaryNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermUnary*		unary	= node->getAsUnaryNode();
		const uint					arg_uid	= ++clSource.uid;
		CLSource::Node				dst_node;	dst_node.uid = uid;	

		CHECK_ERR( ConvertType( node, unary->getType(), unary->getLoc(), null, OUT dst_node.typeInfo ) );
		CHECK_ERR( RecursiveProcessNode( unary, unary->getOperand(), arg_uid, clSource ) );

		const auto&		arg_node = clSource.nodes( arg_uid );
		
		CHECK_ERR( not arg_node.src.Empty() );

		if ( unary->getOp() >= glslang::TOperator::EOpConvIntToBool and
			 unary->getOp() <= glslang::TOperator::EOpConvUint16ToUint64 )
		{
			dst_node.src << dst_node.typeInfo.ToString() << "(" << arg_node.src << ")";
		}
		else
		switch ( unary->getOp() )
		{
			case glslang::TOperator::EOpNegative :			dst_node.src << "-(" << arg_node.src << ")";	break;
			case glslang::TOperator::EOpLogicalNot :		dst_node.src << "!(" << arg_node.src << ")";	break;
			case glslang::TOperator::EOpVectorLogicalNot :	dst_node.src << "not(" << arg_node.src << ")";	break;
			case glslang::TOperator::EOpBitwiseNot :		dst_node.src << "~(" << arg_node.src << ")";	break;
			case glslang::TOperator::EOpPostIncrement :		dst_node.src << "(" << arg_node.src << ")++";	break;
			case glslang::TOperator::EOpPostDecrement :		dst_node.src << "(" << arg_node.src << ")--";	break;
			case glslang::TOperator::EOpPreIncrement :		dst_node.src << "++(" << arg_node.src << ")";	break;
			case glslang::TOperator::EOpPreDecrement :		dst_node.src << "--(" << arg_node.src << ")";	break;
			default :										RETURN_ERR( "unknown operator!" );
		}

		clSource.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	RecursiveProcessBinaryNode
=================================================
*/
	static bool RecursiveProcessBinaryNode (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermBinary*		binary = node->getAsBinaryNode();

		if ( binary->getOp() == glslang::TOperator::EOpVectorSwizzle )
		{
			CHECK_ERR( TranslateVectorSwizzle( root, binary, uid, clSource ) );
			return true;
		}

		const uint	lhs_uid	= ++clSource.uid;
		const uint	rhs_uid	= ++clSource.uid;

		CHECK_ERR( RecursiveProcessNode( binary, binary->getLeft(), lhs_uid, clSource ) );
		CHECK_ERR( RecursiveProcessNode( binary, binary->getRight(), rhs_uid, clSource ) );

		const auto &		lhs = clSource.nodes( lhs_uid ).src;
		const auto &		rhs = clSource.nodes( rhs_uid ).src;
		CLSource::Node		dst_node;

		dst_node.uid = uid;
		CHECK_ERR( ConvertType( node, binary->getType(), binary->getLoc(), null, OUT dst_node.typeInfo ) );
		CHECK_ERR( not lhs.Empty() and not rhs.Empty() );

		switch ( binary->getOp() )
		{
			case glslang::TOperator::EOpAdd :						dst_node.src << '('<<lhs <<'+' << rhs<<')';	break;
			case glslang::TOperator::EOpSub :						dst_node.src << '('<<lhs <<'-' << rhs<<')';	break;
			case glslang::TOperator::EOpMul :						dst_node.src << '('<<lhs <<'*' << rhs<<')';	break;
			case glslang::TOperator::EOpDiv :						dst_node.src << '('<<lhs <<'/' << rhs<<')';	break;
			case glslang::TOperator::EOpMod :						dst_node.src << '('<<lhs <<'%' << rhs<<')';	break;
			case glslang::TOperator::EOpRightShift :				dst_node.src << '('<<lhs <<">>"<< rhs<<')';	break;
			case glslang::TOperator::EOpLeftShift :					dst_node.src << '('<<lhs <<"<<"<< rhs<<')';	break;
			case glslang::TOperator::EOpAnd :						dst_node.src << '('<<lhs <<'&' << rhs<<')';	break;
			case glslang::TOperator::EOpInclusiveOr :				dst_node.src << '('<<lhs <<'|' << rhs<<')';	break;
			case glslang::TOperator::EOpExclusiveOr :				dst_node.src << '('<<lhs <<'^' << rhs<<')';	break;
			case glslang::TOperator::EOpEqual :						dst_node.src << '('<<lhs <<"=="<< rhs<<')';	break;
			case glslang::TOperator::EOpNotEqual :					dst_node.src << '('<<lhs <<"!="<< rhs<<')';	break;
			case glslang::TOperator::EOpVectorEqual :				dst_node.src << "equal("<<lhs<<"), ("<<rhs<<"))";		break;
			case glslang::TOperator::EOpVectorNotEqual :			dst_node.src << "notEqual("<<lhs<<"), ("<<rhs<<"))";	break;
			case glslang::TOperator::EOpLessThan :					dst_node.src << '('<<lhs <<'<' << rhs<<')';	break;
			case glslang::TOperator::EOpGreaterThan :				dst_node.src << '('<<lhs <<'>' << rhs<<')';	break;
			case glslang::TOperator::EOpLessThanEqual :				dst_node.src << '('<<lhs <<"<="<< rhs<<')';	break;
			case glslang::TOperator::EOpGreaterThanEqual :			dst_node.src << '('<<lhs <<">="<< rhs<<')';	break;
			//case glslang::TOperator::EOpComma :					dst_node.src << '('<<lhs <<',' << rhs<<')';	break;
			case glslang::TOperator::EOpVectorTimesScalar :			dst_node.src << '('<<lhs <<'*' << rhs<<')';	break;
			case glslang::TOperator::EOpVectorTimesMatrix :			dst_node.src << '('<<lhs <<'*' << rhs<<')';	break;
			case glslang::TOperator::EOpMatrixTimesVector :			dst_node.src << '('<<lhs <<'*' << rhs<<')';	break;
			case glslang::TOperator::EOpMatrixTimesScalar :			dst_node.src << '('<<lhs <<'*' << rhs<<')';	break;
			case glslang::TOperator::EOpLogicalOr :					dst_node.src << '('<<lhs <<"||"<< rhs<<')';	break;
			case glslang::TOperator::EOpLogicalXor :				dst_node.src << '('<<lhs <<"^^"<< rhs<<')';	break;
			case glslang::TOperator::EOpLogicalAnd :				dst_node.src << '('<<lhs <<"&&"<< rhs<<')';	break;
			case glslang::TOperator::EOpIndexDirect :				dst_node.src << '('<<lhs <<'['<<rhs<<']';			break;
			//case glslang::TOperator::EOpIndexIndirect :			break;
			case glslang::TOperator::EOpIndexDirectStruct :			dst_node.src << '('<<lhs <<'.' <<rhs;				break;
			//case glslang::TOperator::EOpMethod :					break;
			//case glslang::TOperator::EOpScoping :					break;
			case glslang::TOperator::EOpAssign :					dst_node.src << lhs <<'=' << rhs;						break;
			case glslang::TOperator::EOpAddAssign :					dst_node.src << '('<<lhs <<"+="<< rhs<<')';	break;
			case glslang::TOperator::EOpSubAssign :					dst_node.src << '('<<lhs <<"-="<< rhs<<')';	break;
			case glslang::TOperator::EOpMulAssign :					dst_node.src << '('<<lhs <<"*="<< rhs<<')';	break;
			case glslang::TOperator::EOpVectorTimesMatrixAssign :	dst_node.src << '('<<lhs <<"*="<< rhs<<')';	break;
			case glslang::TOperator::EOpVectorTimesScalarAssign :	dst_node.src << '('<<lhs <<"*="<< rhs<<')';	break;
			case glslang::TOperator::EOpMatrixTimesScalarAssign :	dst_node.src << '('<<lhs <<"*="<< rhs<<')';	break;
			case glslang::TOperator::EOpMatrixTimesMatrixAssign :	dst_node.src << '('<<lhs <<"*="<< rhs<<')';	break;
			case glslang::TOperator::EOpDivAssign :					dst_node.src << '('<<lhs <<"/="<< rhs<<')';	break;
			case glslang::TOperator::EOpModAssign :					dst_node.src << '('<<lhs <<"%="<< rhs<<')';	break;
			case glslang::TOperator::EOpAndAssign :					dst_node.src << '('<<lhs <<"&="<< rhs<<')';	break;
			case glslang::TOperator::EOpInclusiveOrAssign :			dst_node.src << '('<<lhs <<"|="<< rhs<<')';	break;
			case glslang::TOperator::EOpExclusiveOrAssign :			dst_node.src << '('<<lhs <<"^="<< rhs<<')';	break;
			case glslang::TOperator::EOpLeftShiftAssign :			dst_node.src << '('<<lhs <<"<<="<< rhs<<')';	break;
			case glslang::TOperator::EOpRightShiftAssign :			dst_node.src << '('<<lhs <<">>="<< rhs<<')';	break;
			default :												RETURN_ERR( "unknown operator!" );
		}
		
		clSource.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	TranslateVectorSwizzle
=================================================
*/
	static bool TranslateVectorSwizzle (TIntermNode* root, glslang::TIntermOperator* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermBinary*	binary = node->getAsBinaryNode();
		CLSource::Node			dst_node;

		CHECK_ERR( binary and binary->getOp() == glslang::TOperator::EOpVectorSwizzle );
		
		const uint	lhs_uid	= ++clSource.uid;
		CHECK_ERR( RecursiveProcessNode( binary, binary->getLeft(), lhs_uid, clSource ) );
		
		const auto&	lhs = clSource.nodes( lhs_uid );
		CHECK_ERR( not lhs.src.Empty() );

		dst_node.uid = uid;
		dst_node.src << lhs.src << '.';

		glslang::TIntermAggregate*	aggr = binary->getRight()->getAsAggregate();
		CHECK_ERR( aggr and aggr->getOp() == glslang::TOperator::EOpSequence );

		for (size_t i = 0; i < aggr->getSequence().size(); ++i)
		{
			glslang::TIntermConstantUnion*		cu		= aggr->getSequence()[i]->getAsConstantUnion();
			glslang::TConstUnionArray const&	cu_arr	= cu->getConstArray();

			CHECK_ERR( cu_arr.size() == 1 and cu->getType().getBasicType() == glslang::EbtInt );
			CHECK_ERR( cu_arr[0].getType() == glslang::EbtInt and cu_arr[0].getIConst() >= 0 and cu_arr[0].getIConst() < 4 );

			const char xyzw[] = "xyzw";

			dst_node.src << xyzw[ cu_arr[0].getIConst() ];
		}

		clSource.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	TranslateConstructorCall
=================================================
*/
	static bool TranslateConstructorCall (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		CLSource::Node				dst_node;
		glslang::TIntermOperator*	op = node->getAsOperator();
		CHECK_ERR( ConvertType( node, op->getType(), op->getLoc(), null, OUT dst_node.typeInfo ) );

		/*switch ( op->getOp() )
		{
			case glslang::TOperator::EOpConstructInt :				dst_node.src = "int";		break;
			case glslang::TOperator::EOpConstructUint :				dst_node.src = "uint";		break;
			case glslang::TOperator::EOpConstructInt64 :			dst_node.src = "int64";		break;
			case glslang::TOperator::EOpConstructUint64 :			dst_node.src = "uint64";	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpConstructInt16 :			break;
			//case glslang::TOperator::EOpConstructUint16 :			break;
			#endif
			case glslang::TOperator::EOpConstructBool :				dst_node.src = "bool";		break;
			case glslang::TOperator::EOpConstructFloat :			dst_node.src = "float";		break;
			case glslang::TOperator::EOpConstructDouble :			dst_node.src = "double";	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpConstructFloat16 :		break;
			#endif
			case glslang::TOperator::EOpConstructVec2 :				dst_node.src = "vec2";		break;
			case glslang::TOperator::EOpConstructVec3 :				dst_node.src = "vec3";		break;
			case glslang::TOperator::EOpConstructVec4 :				dst_node.src = "vec4";		break;
			case glslang::TOperator::EOpConstructDVec2 :			dst_node.src = "dvec2";		break;
			case glslang::TOperator::EOpConstructDVec3 :			dst_node.src = "dvec3(";	break;
			case glslang::TOperator::EOpConstructDVec4 :			dst_node.src = "dvec4(";	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpConstructF16Vec2 :		break;
			//case glslang::TOperator::EOpConstructF16Vec3 :		break;
			//case glslang::TOperator::EOpConstructF16Vec4 :		break;
			#endif
			case glslang::TOperator::EOpConstructBVec2 :			dst_node.src = "bvec2(";	break;
			case glslang::TOperator::EOpConstructBVec3 :			dst_node.src = "bvec3(";	break;
			case glslang::TOperator::EOpConstructBVec4 :			dst_node.src = "bvec4(";	break;
			case glslang::TOperator::EOpConstructIVec2 :			dst_node.src = "ivec2(";	break;
			case glslang::TOperator::EOpConstructIVec3 :			dst_node.src = "ivec3(";	break;
			case glslang::TOperator::EOpConstructIVec4 :			dst_node.src = "ivec4(";	break;
			case glslang::TOperator::EOpConstructUVec2 :			dst_node.src = "uvec2(";	break;
			case glslang::TOperator::EOpConstructUVec3 :			dst_node.src = "uvec3(";	break;
			case glslang::TOperator::EOpConstructUVec4 :			dst_node.src = "uvec4(";	break;
			case glslang::TOperator::EOpConstructI64Vec2 :			dst_node.src = "i64vec2(";	break;
			case glslang::TOperator::EOpConstructI64Vec3 :			dst_node.src = "i64vec3(";	break;
			case glslang::TOperator::EOpConstructI64Vec4 :			dst_node.src = "i64vec4(";	break;
			case glslang::TOperator::EOpConstructU64Vec2 :			dst_node.src = "u64vec2(";	break;
			case glslang::TOperator::EOpConstructU64Vec3 :			dst_node.src = "u64vec3(";	break;
			case glslang::TOperator::EOpConstructU64Vec4 :			dst_node.src = "u64vec4(";	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpConstructI16Vec2 :		break;
			//case glslang::TOperator::EOpConstructI16Vec3 :		break;
			//case glslang::TOperator::EOpConstructI16Vec4 :		break;
			//case glslang::TOperator::EOpConstructU16Vec2 :		break;
			//case glslang::TOperator::EOpConstructU16Vec3 :		break;
			//case glslang::TOperator::EOpConstructU16Vec4 :		break;
			#endif
			case glslang::TOperator::EOpConstructMat2x2 :			dst_node.src = "mat2x2(";	break;
			case glslang::TOperator::EOpConstructMat2x3 :			dst_node.src = "mat2x3(";	break;
			case glslang::TOperator::EOpConstructMat2x4 :			dst_node.src = "mat2x4(";	break;
			case glslang::TOperator::EOpConstructMat3x2 :			dst_node.src = "mat3x2(";	break;
			case glslang::TOperator::EOpConstructMat3x3 :			dst_node.src = "mat3x3(";	break;
			case glslang::TOperator::EOpConstructMat3x4 :			dst_node.src = "mat3x4(";	break;
			case glslang::TOperator::EOpConstructMat4x2 :			dst_node.src = "mat4x2(";	break;
			case glslang::TOperator::EOpConstructMat4x3 :			dst_node.src = "mat4x3(";	break;
			case glslang::TOperator::EOpConstructMat4x4 :			dst_node.src = "mat4x4(";	break;
			case glslang::TOperator::EOpConstructDMat2x2 :			dst_node.src = "dmat2x2(";	break;
			case glslang::TOperator::EOpConstructDMat2x3 :			dst_node.src = "dmat2x3(";	break;
			case glslang::TOperator::EOpConstructDMat2x4 :			dst_node.src = "dmat2x4(";	break;
			case glslang::TOperator::EOpConstructDMat3x2 :			dst_node.src = "dmat3x2(";	break;
			case glslang::TOperator::EOpConstructDMat3x3 :			dst_node.src = "dmat3x3(";	break;
			case glslang::TOperator::EOpConstructDMat3x4 :			dst_node.src = "dmat3x4(";	break;
			case glslang::TOperator::EOpConstructDMat4x2 :			dst_node.src = "dmat4x2(";	break;
			case glslang::TOperator::EOpConstructDMat4x3 :			dst_node.src = "dmat4x3(";	break;
			case glslang::TOperator::EOpConstructDMat4x4 :			dst_node.src = "dmat4x4(";	break;
			case glslang::TOperator::EOpConstructIMat2x2 :			dst_node.src = "imat2x2(";	break;
			case glslang::TOperator::EOpConstructIMat2x3 :			dst_node.src = "imat2x3(";	break;
			case glslang::TOperator::EOpConstructIMat2x4 :			dst_node.src = "imat2x4(";	break;
			case glslang::TOperator::EOpConstructIMat3x2 :			dst_node.src = "imat3x2(";	break;
			case glslang::TOperator::EOpConstructIMat3x3 :			dst_node.src = "imat3x3(";	break;
			case glslang::TOperator::EOpConstructIMat3x4 :			dst_node.src = "imat3x4(";	break;
			case glslang::TOperator::EOpConstructIMat4x2 :			dst_node.src = "imat4x2(";	break;
			case glslang::TOperator::EOpConstructIMat4x3 :			dst_node.src = "imat4x3(";	break;
			case glslang::TOperator::EOpConstructIMat4x4 :			dst_node.src = "imat4x4(";	break;
			case glslang::TOperator::EOpConstructUMat2x2 :			dst_node.src = "umat2x2(";	break;
			case glslang::TOperator::EOpConstructUMat2x3 :			dst_node.src = "umat2x3(";	break;
			case glslang::TOperator::EOpConstructUMat2x4 :			dst_node.src = "umat2x4(";	break;
			case glslang::TOperator::EOpConstructUMat3x2 :			dst_node.src = "umat3x2(";	break;
			case glslang::TOperator::EOpConstructUMat3x3 :			dst_node.src = "umat3x3(";	break;
			case glslang::TOperator::EOpConstructUMat3x4 :			dst_node.src = "umat3x4(";	break;
			case glslang::TOperator::EOpConstructUMat4x2 :			dst_node.src = "umat4x2(";	break;
			case glslang::TOperator::EOpConstructUMat4x3 :			dst_node.src = "umat4x3(";	break;
			case glslang::TOperator::EOpConstructUMat4x4 :			dst_node.src = "umat4x4(";	break;
			case glslang::TOperator::EOpConstructBMat2x2 :			dst_node.src = "bmat2x2(";	break;
			case glslang::TOperator::EOpConstructBMat2x3 :			dst_node.src = "bmat2x3(";	break;
			case glslang::TOperator::EOpConstructBMat2x4 :			dst_node.src = "bmat2x4(";	break;
			case glslang::TOperator::EOpConstructBMat3x2 :			dst_node.src = "bmat3x2(";	break;
			case glslang::TOperator::EOpConstructBMat3x3 :			dst_node.src = "bmat3x3(";	break;
			case glslang::TOperator::EOpConstructBMat3x4 :			dst_node.src = "bmat3x4(";	break;
			case glslang::TOperator::EOpConstructBMat4x2 :			dst_node.src = "bmat4x2(";	break;
			case glslang::TOperator::EOpConstructBMat4x3 :			dst_node.src = "bmat4x3(";	break;
			case glslang::TOperator::EOpConstructBMat4x4 :			dst_node.src = "bmat4x4(";	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpConstructF16Mat2x2 :		break;
			//case glslang::TOperator::EOpConstructF16Mat2x3 :		break;
			//case glslang::TOperator::EOpConstructF16Mat2x4 :		break;
			//case glslang::TOperator::EOpConstructF16Mat3x2 :		break;
			//case glslang::TOperator::EOpConstructF16Mat3x3 :		break;
			//case glslang::TOperator::EOpConstructF16Mat3x4 :		break;
			//case glslang::TOperator::EOpConstructF16Mat4x2 :		break;
			//case glslang::TOperator::EOpConstructF16Mat4x3 :		break;
			//case glslang::TOperator::EOpConstructF16Mat4x4 :		break;
			#endif
			//case glslang::TOperator::EOpConstructStruct :			dst_node.src = "struct(";	break;
			//case glslang::TOperator::EOpConstructTextureSampler :	dst_node.src = "sampler(";	break;
			default :												RETURN_ERR( "unknown constructor!" );
		}*/

		dst_node.src << dst_node.typeInfo.ToString() << "(";

		if ( node->getAsAggregate() )
		{
			glslang::TIntermAggregate* aggr = node->getAsAggregate();
			
			for (size_t i = 0; i < aggr->getSequence().size(); ++i)
			{
				const uint	arg_uid = ++clSource.uid;

				CHECK_ERR( RecursiveProcessNode( aggr, aggr->getSequence()[i], arg_uid, clSource ) );

				const auto&	arg = clSource.nodes( arg_uid ).src;
				CHECK_ERR( not arg.Empty() );

				dst_node.src << (i ? ',' : ' ') << arg;
			}
		}

		dst_node.src << " )";

		clSource.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	TranslateImageFuncCall
=================================================
*/
	static bool TranslateImageFuncCall (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermOperator*	op = node->getAsOperator();
		CLSource::Node				dst_node;

		CHECK_ERR( ConvertType( node, op->getType(), op->getLoc(), null, OUT dst_node.typeInfo ) );

		switch ( op->getOp() )
		{
			case glslang::TOperator::EOpImageQuerySize :		dst_node.src << "imageSize(";		break;
			case glslang::TOperator::EOpImageQuerySamples :		dst_node.src << "imageSamples(";	break;
			case glslang::TOperator::EOpImageLoad :				dst_node.src << "imageLoad(";		break;
			case glslang::TOperator::EOpImageStore :			dst_node.src << "imageStore(";		break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpImageLoadLod :		dst_node.src << "imageLoadLod(";	break;
			//case glslang::TOperator::EOpImageStoreLod :		dst_node.src << "imageStoreLod(";	break;
			#endif
			case glslang::TOperator::EOpImageAtomicAdd :		dst_node.src << "imageAtomicAdd(";	break;
			case glslang::TOperator::EOpImageAtomicMin :		dst_node.src << "imageAtomicMin(";	break;
			case glslang::TOperator::EOpImageAtomicMax :		dst_node.src << "imageAtomicMax(";	break;
			case glslang::TOperator::EOpImageAtomicAnd :		dst_node.src << "imageAtomicAnd(";	break;
			case glslang::TOperator::EOpImageAtomicOr :			dst_node.src << "imageAtomicOr(";	break;
			case glslang::TOperator::EOpImageAtomicXor :		dst_node.src << "imageAtomicXor(";	break;
			case glslang::TOperator::EOpImageAtomicExchange :	dst_node.src << "imageAtomicExchange(";	break;
			case glslang::TOperator::EOpImageAtomicCompSwap :	dst_node.src << "imageAtomicCompSwap(";	break;
			//case glslang::TOperator::EOpSubpassLoad :			dst_node.src << "image(";	break;
			//case glslang::TOperator::EOpSubpassLoadMS :		dst_node.src << "image(";	break;
			//case glslang::TOperator::EOpSparseImageLoad :		dst_node.src << "image(";	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpSparseImageLoadLod :	dst_node.src << "image(";	break;
			#endif
			default :											RETURN_ERR( "unknown function!" );
		}
		
		if ( node->getAsAggregate() )
		{
			glslang::TIntermAggregate* aggr = node->getAsAggregate();
			
			for (size_t i = 0; i < aggr->getSequence().size(); ++i)
			{
				const uint	arg_uid = ++clSource.uid;

				CHECK_ERR( RecursiveProcessNode( aggr, aggr->getSequence()[i], arg_uid, clSource ) );

				const auto&	arg = clSource.nodes( arg_uid ).src;
				CHECK_ERR( not arg.Empty() );

				dst_node.src << (i ? ", " : " ") << arg;
			}
		}

		dst_node.uid = uid;
		dst_node.src << " )";

		clSource.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
/*
=================================================
	TranslateFunctionCall
=================================================
*/
	static bool TranslateFunctionCall (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermAggregate*	aggr = node->getAsAggregate();
		CLSource::Node				dst_node;

		CHECK_ERR( aggr and aggr->getOp() == glslang::TOperator::EOpFunctionCall );
		CHECK_ERR( ConvertType( node, aggr->getType(), aggr->getLoc(), null, OUT dst_node.typeInfo ) );

		String	name = aggr->getName().c_str();
		usize	pos	 = 0;

		if ( name.Find( '(', OUT pos ) )
			name = name.SubString( 0, pos );

		dst_node.src << name << "(";

		for (size_t i = 0; i < aggr->getSequence().size(); ++i)
		{
			const uint	arg_uid = ++clSource.uid;

			CHECK_ERR( RecursiveProcessNode( aggr, aggr->getSequence()[i], arg_uid, clSource ) );

			const auto&	arg = clSource.nodes( arg_uid ).src;
			CHECK_ERR( not arg.Empty() );

			dst_node.src << (i ? ", " : " ") << arg;
		}

		dst_node.uid = uid;
		dst_node.src << " )";

		clSource.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}
	
	static bool TranslateBuiltinFunctionCall (TIntermNode* root, TIntermNode* node, const uint uid, CLSource &clSource)
	{
		glslang::TIntermAggregate*	aggr = node->getAsAggregate();
		CLSource::Node				dst_node;
		
		CHECK_ERR( ConvertType( node, aggr->getType(), aggr->getLoc(), null, OUT dst_node.typeInfo ) );

		switch ( aggr->getOp() )
		{
			case glslang::TOperator::EOpRadians :				dst_node.src << "radians";				break;
			case glslang::TOperator::EOpDegrees :				dst_node.src << "degrees";				break;
			case glslang::TOperator::EOpSin :					dst_node.src << "sin";					break;
			case glslang::TOperator::EOpCos :					dst_node.src << "cos";					break;
			case glslang::TOperator::EOpTan :					dst_node.src << "tan";					break;
			case glslang::TOperator::EOpAsin :					dst_node.src << "asin";					break;
			case glslang::TOperator::EOpAcos :					dst_node.src << "acos";					break;
			case glslang::TOperator::EOpAtan :					dst_node.src << "atan";					break;
			case glslang::TOperator::EOpSinh :					dst_node.src << "sinh";					break;
			case glslang::TOperator::EOpCosh :					dst_node.src << "cosh";					break;
			case glslang::TOperator::EOpTanh :					dst_node.src << "tanh";					break;
			case glslang::TOperator::EOpAsinh :					dst_node.src << "asinh";				break;
			case glslang::TOperator::EOpAcosh :					dst_node.src << "acosh";				break;
			case glslang::TOperator::EOpAtanh :					dst_node.src << "atanh";				break;
			case glslang::TOperator::EOpPow :					dst_node.src << "pow";					break;
			case glslang::TOperator::EOpExp :					dst_node.src << "exp";					break;
			case glslang::TOperator::EOpLog :					dst_node.src << "log";					break;
			case glslang::TOperator::EOpExp2 :					dst_node.src << "exp2";					break;
			case glslang::TOperator::EOpLog2 :					dst_node.src << "log2";					break;
			case glslang::TOperator::EOpSqrt :					dst_node.src << "sqrt";					break;
			case glslang::TOperator::EOpInverseSqrt :			dst_node.src << "inversesqrt";			break;
			case glslang::TOperator::EOpAbs :					dst_node.src << "abs";					break;
			case glslang::TOperator::EOpSign :					dst_node.src << "sign";					break;
			case glslang::TOperator::EOpFloor :					dst_node.src << "floor";				break;
			case glslang::TOperator::EOpTrunc :					dst_node.src << "trunc";				break;
			case glslang::TOperator::EOpRound :					dst_node.src << "round";				break;
			case glslang::TOperator::EOpRoundEven :				dst_node.src << "roundEven";			break;
			case glslang::TOperator::EOpCeil :					dst_node.src << "ceil";					break;
			case glslang::TOperator::EOpFract :					dst_node.src << "fract";				break;
			case glslang::TOperator::EOpModf :					dst_node.src << "modf";					break;
			case glslang::TOperator::EOpMin :					dst_node.src << "min";					break;
			case glslang::TOperator::EOpMax :					dst_node.src << "max";					break;
			case glslang::TOperator::EOpClamp :					dst_node.src << "clamp";				break;
			case glslang::TOperator::EOpMix :					dst_node.src << "mix";					break;
			case glslang::TOperator::EOpStep :					dst_node.src << "step";					break;
			case glslang::TOperator::EOpSmoothStep :			dst_node.src << "smoothstep";			break;
			case glslang::TOperator::EOpIsNan :					dst_node.src << "isnan";				break;
			case glslang::TOperator::EOpIsInf :					dst_node.src << "isinf";				break;
			case glslang::TOperator::EOpFma :					dst_node.src << "fma";					break;
			case glslang::TOperator::EOpFrexp :					dst_node.src << "frexp";				break;
			case glslang::TOperator::EOpLdexp :					dst_node.src << "ldexp";				break;
			case glslang::TOperator::EOpFloatBitsToInt :		dst_node.src << "floatBitsToInt";		break;
			case glslang::TOperator::EOpFloatBitsToUint :		dst_node.src << "floatBitsToUint";		break;
			case glslang::TOperator::EOpIntBitsToFloat :		dst_node.src << "intBitsToFloat";		break;
			case glslang::TOperator::EOpUintBitsToFloat :		dst_node.src << "uintBitsToFloat";		break;
			case glslang::TOperator::EOpDoubleBitsToInt64 :		dst_node.src << "doubleBitsToInt64";	break;
			case glslang::TOperator::EOpDoubleBitsToUint64 :	dst_node.src << "doubleBitsToUint64";	break;
			case glslang::TOperator::EOpInt64BitsToDouble :		dst_node.src << "int64BitsToDouble";	break;
			case glslang::TOperator::EOpUint64BitsToDouble :	dst_node.src << "uint64BitsToDouble";	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpFloat16BitsToInt16,
			//case glslang::TOperator::EOpFloat16BitsToUint16,
			//case glslang::TOperator::EOpInt16BitsToFloat16,
			//case glslang::TOperator::EOpUint16BitsToFloat16,
			#endif
			case glslang::TOperator::EOpPackSnorm2x16 :			dst_node.src << "packSnorm2x16";	break;
			case glslang::TOperator::EOpUnpackSnorm2x16 :		dst_node.src << "unpackSnorm2x16";	break;
			case glslang::TOperator::EOpPackUnorm2x16 :			dst_node.src << "packUnorm2x16";	break;
			case glslang::TOperator::EOpUnpackUnorm2x16 :		dst_node.src << "unpackUnorm2x16";	break;
			case glslang::TOperator::EOpPackSnorm4x8 :			dst_node.src << "packSnorm4x8";		break;
			case glslang::TOperator::EOpUnpackSnorm4x8 :		dst_node.src << "unpackSnorm4x8";	break;
			case glslang::TOperator::EOpPackUnorm4x8 :			dst_node.src << "packUnorm4x8";		break;
			case glslang::TOperator::EOpUnpackUnorm4x8 :		dst_node.src << "unpackUnorm4x8";	break;
			case glslang::TOperator::EOpPackHalf2x16 :			dst_node.src << "packHalf2x16";		break;
			case glslang::TOperator::EOpUnpackHalf2x16 :		dst_node.src << "unpackHalf2x16";	break;
			case glslang::TOperator::EOpPackDouble2x32 :		dst_node.src << "packDouble2x32";	break;
			case glslang::TOperator::EOpUnpackDouble2x32 :		dst_node.src << "unpackDouble2x32";	break;
			case glslang::TOperator::EOpPackInt2x32 :			dst_node.src << "packInt2x32";		break;
			case glslang::TOperator::EOpUnpackInt2x32 :			dst_node.src << "unpackInt2x32";	break;
			case glslang::TOperator::EOpPackUint2x32 :			dst_node.src << "packUint2x32";		break;
			case glslang::TOperator::EOpUnpackUint2x32 :		dst_node.src << "unpackUint2x32";	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpPackFloat2x16,
			//case glslang::TOperator::EOpUnpackFloat2x16,
			//case glslang::TOperator::EOpPackInt2x16,
			//case glslang::TOperator::EOpUnpackInt2x16,
			//case glslang::TOperator::EOpPackUint2x16,
			//case glslang::TOperator::EOpUnpackUint2x16,
			//case glslang::TOperator::EOpPackInt4x16,
			//case glslang::TOperator::EOpUnpackInt4x16,
			//case glslang::TOperator::EOpPackUint4x16,
			//case glslang::TOperator::EOpUnpackUint4x16,
			#endif
			case glslang::TOperator::EOpLength :				dst_node.src << "length";		break;
			case glslang::TOperator::EOpDistance :				dst_node.src << "distance";		break;
			case glslang::TOperator::EOpDot :					dst_node.src << "dot";			break;
			case glslang::TOperator::EOpCross :					dst_node.src << "cross";		break;
			case glslang::TOperator::EOpNormalize :				dst_node.src << "normalize";	break;
			case glslang::TOperator::EOpFaceForward :			dst_node.src << "faceForward";	break;
			case glslang::TOperator::EOpReflect :				dst_node.src << "reflect";		break;
			case glslang::TOperator::EOpRefract :				dst_node.src << "refract";		break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpMin3,
			//case glslang::TOperator::EOpMax3,
			//case glslang::TOperator::EOpMid3,
			#endif
			case glslang::TOperator::EOpDPdx :						dst_node.src << "dFdx";						break;
			case glslang::TOperator::EOpDPdy :						dst_node.src << "dFdy";						break;
			case glslang::TOperator::EOpFwidth :					dst_node.src << "fwidth";					break;
			case glslang::TOperator::EOpDPdxFine :					dst_node.src << "dFdxFine";					break;
			case glslang::TOperator::EOpDPdyFine :					dst_node.src << "dFdyFine";					break;
			case glslang::TOperator::EOpFwidthFine :				dst_node.src << "fwidthFine";				break;
			case glslang::TOperator::EOpDPdxCoarse :				dst_node.src << "dFdxCoarse";				break;
			case glslang::TOperator::EOpDPdyCoarse :				dst_node.src << "dFdyCoarse";				break;
			case glslang::TOperator::EOpFwidthCoarse :				dst_node.src << "fwidthCoarse";				break;
			case glslang::TOperator::EOpInterpolateAtCentroid :		dst_node.src << "interpolateAtCentroid";	break;
			case glslang::TOperator::EOpInterpolateAtSample :		dst_node.src << "interpolateAtSample";		break;
			case glslang::TOperator::EOpInterpolateAtOffset :		dst_node.src << "interpolateAtOffset";		break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpInterpolateAtVertex,
			#endif
			case glslang::TOperator::EOpMatrixTimesMatrix :			dst_node.src << "matrixTimesMatrix";	break;
			case glslang::TOperator::EOpOuterProduct :				dst_node.src << "outerProduct";			break;
			case glslang::TOperator::EOpDeterminant :				dst_node.src << "";						break;
			case glslang::TOperator::EOpMatrixInverse :				dst_node.src << "inverse";				break;
			case glslang::TOperator::EOpTranspose :					dst_node.src << "transpose";			break;
			case glslang::TOperator::EOpFtransform :				dst_node.src << "ftransform";			break;
			case glslang::TOperator::EOpNoise :						dst_node.src << "oise";					break;
			case glslang::TOperator::EOpEmitVertex :				dst_node.src << "EmitVertex";			break;
			case glslang::TOperator::EOpEndPrimitive :				dst_node.src << "endPrimitive";			break;
			case glslang::TOperator::EOpEmitStreamVertex :			dst_node.src << "emitStreamVertex";		break;
			case glslang::TOperator::EOpEndStreamPrimitive :		dst_node.src << "endStreamPrimitive";	break;
			case glslang::TOperator::EOpBarrier :					dst_node.src << "barrier";				break;
			case glslang::TOperator::EOpMemoryBarrier :				dst_node.src << "memoryBarrier";		break;
			case glslang::TOperator::EOpMemoryBarrierAtomicCounter:	dst_node.src << "memoryBarrierAtomicCounter";	break;
			case glslang::TOperator::EOpMemoryBarrierBuffer :		dst_node.src << "memoryBarrierBuffer";	break;
			case glslang::TOperator::EOpMemoryBarrierImage :		dst_node.src << "memoryBarrierImage";	break;
			case glslang::TOperator::EOpMemoryBarrierShared :		dst_node.src << "memoryBarrierShared";	break;
			case glslang::TOperator::EOpGroupMemoryBarrier :		dst_node.src << "groupMemoryBarrier";	break;
			case glslang::TOperator::EOpBallot :					dst_node.src << "ballotARB";			break;
			case glslang::TOperator::EOpReadInvocation :			dst_node.src << "readInvocation";		break;
			case glslang::TOperator::EOpReadFirstInvocation :		dst_node.src << "readFirstInvocatin";	break;
			case glslang::TOperator::EOpAnyInvocation :				dst_node.src << "anyInvocation";		break;
			case glslang::TOperator::EOpAllInvocations :			dst_node.src << "allInvocations";		break;
			case glslang::TOperator::EOpAllInvocationsEqual :		dst_node.src << "allInvocationsEqual";	break;
			#ifdef AMD_EXTENSIONS
			//case glslang::TOperator::EOpMinInvocations,
			//case glslang::TOperator::EOpMaxInvocations,
			//case glslang::TOperator::EOpAddInvocations,
			//case glslang::TOperator::EOpMinInvocationsNonUniform,
			//case glslang::TOperator::EOpMaxInvocationsNonUniform,
			//case glslang::TOperator::EOpAddInvocationsNonUniform,
			//case glslang::TOperator::EOpMinInvocationsInclusiveScan,
			//case glslang::TOperator::EOpMaxInvocationsInclusiveScan,
			//case glslang::TOperator::EOpAddInvocationsInclusiveScan,
			//case glslang::TOperator::EOpMinInvocationsInclusiveScanNonUniform,
			//case glslang::TOperator::EOpMaxInvocationsInclusiveScanNonUniform,
			//case glslang::TOperator::EOpAddInvocationsInclusiveScanNonUniform,
			//case glslang::TOperator::EOpMinInvocationsExclusiveScan,
			//case glslang::TOperator::EOpMaxInvocationsExclusiveScan,
			//case glslang::TOperator::EOpAddInvocationsExclusiveScan,
			//case glslang::TOperator::EOpMinInvocationsExclusiveScanNonUniform,
			//case glslang::TOperator::EOpMaxInvocationsExclusiveScanNonUniform,
			//case glslang::TOperator::EOpAddInvocationsExclusiveScanNonUniform,
			//case glslang::TOperator::EOpSwizzleInvocations,
			//case glslang::TOperator::EOpSwizzleInvocationsMasked,
			//case glslang::TOperator::EOpWriteInvocation,
			//case glslang::TOperator::EOpMbcnt,
			//case glslang::TOperator::EOpCubeFaceIndex,
			//case glslang::TOperator::EOpCubeFaceCoord,
			//case glslang::TOperator::EOpTime,
			#endif
			case glslang::TOperator::EOpAtomicAdd :					dst_node.src << "atomicAdd";				break;
			case glslang::TOperator::EOpAtomicMin :					dst_node.src << "atomicMin";				break;
			case glslang::TOperator::EOpAtomicMax :					dst_node.src << "atomicMax";				break;
			case glslang::TOperator::EOpAtomicAnd :					dst_node.src << "atomicAnd";				break;
			case glslang::TOperator::EOpAtomicOr :					dst_node.src << "atomicOr";					break;
			case glslang::TOperator::EOpAtomicXor :					dst_node.src << "atomicXor";				break;
			case glslang::TOperator::EOpAtomicExchange :			dst_node.src << "atomicExchange";			break;
			case glslang::TOperator::EOpAtomicCompSwap :			dst_node.src << "atomicCompSwap";			break;
			case glslang::TOperator::EOpAtomicCounterIncrement :	dst_node.src << "atomicCounterIncrement";	break;
			case glslang::TOperator::EOpAtomicCounterDecrement :	dst_node.src << "atomicCounterDecrement";	break;
			case glslang::TOperator::EOpAtomicCounter :				dst_node.src << "atomicCounter";			break;
			case glslang::TOperator::EOpAtomicCounterAdd :			dst_node.src << "atomicCounterAdd";			break;
			case glslang::TOperator::EOpAtomicCounterSubtract :		dst_node.src << "atomicCounterSub";			break;
			case glslang::TOperator::EOpAtomicCounterMin :			dst_node.src << "atomicCounterMin";			break;
			case glslang::TOperator::EOpAtomicCounterMax :			dst_node.src << "atomicCounterMax";			break;
			case glslang::TOperator::EOpAtomicCounterAnd :			dst_node.src << "atomicCounterAnd";			break;
			case glslang::TOperator::EOpAtomicCounterOr :			dst_node.src << "atomicCounterOr";			break;
			case glslang::TOperator::EOpAtomicCounterXor :			dst_node.src << "atomicCounterXor";			break;
			case glslang::TOperator::EOpAtomicCounterExchange :		dst_node.src << "atomicCounterExchange";	break;
			case glslang::TOperator::EOpAtomicCounterCompSwap :		dst_node.src << "atomicCounterCompSwap";	break;
			case glslang::TOperator::EOpAny :						dst_node.src << "any";						break;
			case glslang::TOperator::EOpAll :						dst_node.src << "all";						break;
			default :												RETURN_ERR( "unknown builtin function!" );
		}
		
		dst_node.src << "(";

		for (size_t i = 0; i < aggr->getSequence().size(); ++i)
		{
			const uint	arg_uid = ++clSource.uid;

			CHECK_ERR( RecursiveProcessNode( aggr, aggr->getSequence()[i], arg_uid, clSource ) );

			const auto&	arg = clSource.nodes( arg_uid ).src;
			CHECK_ERR( not arg.Empty() );

			dst_node.src << (i ? ", " : " ") << arg;
		}

		dst_node.uid = uid;
		dst_node.src << " )";

		clSource.nodes.Add( uid, RVREF(dst_node) );
		return true;
	}

}	// PipelineCompiler
