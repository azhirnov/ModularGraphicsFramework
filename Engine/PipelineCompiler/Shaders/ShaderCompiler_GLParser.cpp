// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_Utils.h"


namespace PipelineCompiler
{

/*
=================================================
	DeserializeGLSL
=================================================
*/
	bool ShaderCompiler::Deserialize (EShaderSrcFormat::type shaderFmt, EShader::type shaderType, ArrayCRef<StringCRef> source, StringCRef entryPoint,
									  OUT String &log, OUT DeserializedShader &result)
	{
		result = DeserializedShader();
		
		Config			cfg;
		_GLSLangResult	glslang_data;
		_ShaderData		shader_data;

		cfg.source	= shaderFmt;
		cfg.target	= EShaderSrcFormat::IsVulkan( shaderFmt ) ? EShaderDstFormat::SPIRV_Source : EShaderDstFormat::GLSL_Source;

		shader_data.entry	= entryPoint;
		shader_data.type	= shaderType;
		shader_data.src		<< source;

		const int		substitution_level	= 1;
		
		if ( not _GLSLangParse( cfg, shader_data, OUT log, OUT glslang_data ) )
			return false;
		
		const glslang::TIntermediate* intermediate = glslang_data.prog.getIntermediate( glslang_data.shader->getStage() );
		CHECK_ERR( intermediate );

		// deserialize shader
		TIntermNode*	root	= intermediate->getTreeRoot();
		
		result._shaderType		= shaderType;
		
		CHECK_ERR( _ProcessExternalObjects( null, root, OUT result ) );
		CHECK_ERR( _RecursiveProcessAggregateNode( null, root, OUT result ) );
		CHECK_ERR( _ProcessShaderInfo( intermediate, INOUT result ) );
		return true;
	}
	
/*
=================================================
	_ProcessExternalObjects
=================================================
*/
	bool ShaderCompiler::_ProcessExternalObjects (TIntermNode*, TIntermNode* node, INOUT DeserializedShader &result)
	{
		glslang::TIntermAggregate* aggr = node->getAsAggregate();

		switch ( aggr->getOp() )
		{
			// continue deserializing
			case glslang::TOperator::EOpSequence :
			{
				FOR( i, aggr->getSequence() )
				{
					CHECK_ERR( _ProcessExternalObjects( aggr, aggr->getSequence()[i], INOUT result ) );
				}
				return true;
			}

			// uniforms, buffers, ...
			case glslang::TOperator::EOpLinkerObjects :
			{
				FOR( i, aggr->getSequence() )
				{
					CHECK_ERR( _DeserializeExternalObjects( aggr->getSequence()[i], INOUT result ) );
				}
				return true;
			}
		}
		return true;
	}

/*
=================================================
	_RecursiveProcessAggregateNode
=================================================
*/
	bool ShaderCompiler::_RecursiveProcessAggregateNode (TIntermNode*, TIntermNode* node, INOUT DeserializedShader &result)
	{
		glslang::TIntermAggregate* aggr = node->getAsAggregate();

		switch ( aggr->getOp() )
		{
			// continue deserializing
			case glslang::TOperator::EOpSequence :
			{
				FOR( i, aggr->getSequence() )
				{
					CHECK_ERR( _RecursiveProcessNode( aggr, aggr->getSequence()[i], INOUT result ) );
				}
				return true;
			}

			// function definition
			case glslang::TOperator::EOpFunction :
			{
				CHECK_ERR( _DeserializeFunction( aggr, INOUT result ) );
				return true;
			}
			
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
				CHECK_ERR( _DeserializeAtomicArg( aggr->getSequence()[0], INOUT result ) );
				return true;

			// ignore
			default :
				break;
		}
		return true;
	}
	
/*
=================================================
	_RecursiveProcessNode
=================================================
*/
	bool ShaderCompiler::_RecursiveProcessNode (TIntermNode* root, TIntermNode* node, INOUT DeserializedShader &result)
	{
		if ( not node )
			return true;

		if ( node->getAsAggregate() )
		{
			CHECK_ERR( _RecursiveProcessAggregateNode( root, node, INOUT result ) );
			return true;
		}

		return true;
	}
	
/*
=================================================
	_RecursiveProcessNode
=================================================
*/
	bool ShaderCompiler::_DeserializeAtomicArg (TIntermNode* node, INOUT DeserializedShader &result)
	{
		Array< glslang::TIntermBinary *>	stack;	stack.PushBack( node->getAsBinaryNode() );
		CHECK_ERR( stack.Back() );

		for (; stack.Back()->getLeft()->getAsBinaryNode();)
		{
			stack.PushBack( stack.Back()->getLeft()->getAsBinaryNode() );
		}


		// find SSBO
		const auto&							left_typename	= stack.Back()->getLeft()->getType().getTypeName();
		DeserializedShader::StorageBuffer*	ssb_ptr			= null;

		if ( &left_typename != null )
		{
			String	ssbo_type	= left_typename.c_str();

			FOR( i, result._storageBuffers )
			{
				if ( result._storageBuffers[i].typeName == ssbo_type )
				{
					ssb_ptr = &result._storageBuffers[i];
					break;
				}
			}
		}
		CHECK_ERR( ssb_ptr );


		// find struct field
		DeserializedShader::BufferVariable*	var_ptr = null;

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

			glslang::TType const&	field_type = *st_type[ cu_arr[0].getIConst() ].type;
		
			DeserializedShader::BufferVariable*	old_ptr = var_ptr;
			var_ptr = null;

			if ( old_ptr )
			{
				FOR( i, old_ptr->fields )
				{
					if ( old_ptr->fields[i].name == field_type.getFieldName().c_str() )
					{
						var_ptr = &old_ptr->fields[i];
						break;
					}
				}
			}
			else
			{
				FOR( i, ssb_ptr->fields )
				{
					if ( ssb_ptr->fields[i].name == field_type.getFieldName().c_str() )
					{
						var_ptr = &ssb_ptr->fields[i];
						break;
					}
				}
			}

			CHECK_ERR( var_ptr );
		}
		
		CHECK_ERR( var_ptr );
		
		var_ptr->qualifier |= EVariableQualifier::Volatile;

		return true;
	}

/*
=================================================
	_DeserializeFunction
=================================================
*/
	bool ShaderCompiler::_DeserializeFunction (TIntermNode* node, INOUT DeserializedShader &result)
	{
		glslang::TIntermAggregate*		aggr = node->getAsAggregate();
		DeserializedShader::Function	func;

		ASSERT( aggr->getOp() == glslang::EOpFunction );
		CHECK_ERR( _DeserializeFunctionArg( aggr, aggr->getType(), aggr->getLoc(), null, OUT func.result ) );
		
		// get name
		{
			func.signature = aggr->getName().c_str();
			usize	pos  = 0;

			if ( func.signature.Find( '(', OUT pos ) )
				func.name = func.signature.SubString( 0, pos );
			else
				func.name = func.signature;
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
				
				func.args.Reserve( func.args.Count() + args_node->getSequence().size() );

				FOR( j, args_node->getSequence() )
				{
					DeserializedShader::FunctionArgument	arg;
					glslang::TIntermTyped *					nn = args_node->getSequence()[j]->getAsTyped();

					CHECK_ERR( _DeserializeFunctionArg( nn, nn->getType(), nn->getLoc(), null, OUT arg ) );

					func.args.PushBack( RVREF(arg) );
				}
			}
			else
			{
				CHECK_ERR( _RecursiveProcessNode( aggr, n, INOUT result ) );
			}
		}

		result._functions.PushBack( RVREF(func) );
		return true;
	}
	
/*
=================================================
	_DeserializeVariable
=================================================
*/
	bool ShaderCompiler::_DeserializeVariable (TIntermNode* node, glslang::TType const &type, glslang::TSourceLoc const &loc,
												const DeserializedShader::_Variable *parent, OUT DeserializedShader::_Variable &arg)
	{
		arg.line	= loc.line;
		arg.column	= loc.column;
		
		// copy name
		if ( &type.getFieldName() != null )
		{
			arg.name = type.getFieldName().c_str();
		}
		else
		if ( node and node->getAsSymbolNode() )
		{
			arg.name = node->getAsSymbolNode()->getName().c_str();

			if ( arg.name.StartsWithIC( "anon@" ) )
				arg.name.Clear();
		}

		// copy typename
		if ( &type.getTypeName() != null )
		{
			arg.typeName = type.getTypeName().c_str();
		}

		// arrays
		if ( type.isArray() )
		{
			arg.arraySize = Max( 0, type.getOuterArraySize() );
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
				arg.type = EShaderVariable::ToSampler( samp_type, ConvertBasicType( samp.type, samp.vectorSize ) );
			}
			else
			// image
			if ( samp.isImage() )
			{
				arg.type = EShaderVariable::ToImage( samp_type, ConvertBasicType( samp.type, samp.vectorSize ) );
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
		if ( type.getBasicType() == glslang::TBasicType::EbtVoid or
			 type.getBasicType() == glslang::TBasicType::EbtBool or
			 type.getBasicType() == glslang::TBasicType::EbtFloat or
			 type.getBasicType() == glslang::TBasicType::EbtDouble or
			 type.getBasicType() == glslang::TBasicType::EbtInt or
			 type.getBasicType() == glslang::TBasicType::EbtUint or
			 type.getBasicType() == glslang::TBasicType::EbtInt64 or
			 type.getBasicType() == glslang::TBasicType::EbtUint64 or
			#ifdef AMD_EXTENSIONS
			 type.getBasicType() == glslang::TBasicType::EbtFloat16 or
			 type.getBasicType() == glslang::TBasicType::EbtInt16 or
			 type.getBasicType() == glslang::TBasicType::EbtUint16
			#endif
			)
		{
			arg.type = ConvertBasicType( type.getBasicType(), type.getVectorSize(), type.getMatrixCols(), type.getMatrixRows() );
		}

		// qualifiers
		{
			auto const&	q = type.getQualifier();
			
			if ( type.isBuiltIn()					or
				 arg.typeName.StartsWith("gl_")		or
				 arg.name.StartsWith("gl_")			or
				 (parent and parent->qualifier[ EVariableQualifier::BuiltIn ]) )
			{
				arg.qualifier |= EVariableQualifier::BuiltIn;
			}

			if ( q.invariant )
				arg.qualifier |= EVariableQualifier::Invariant;

			if ( q.noContraction )
				arg.qualifier |= EVariableQualifier::Precise;

			if ( q.centroid )
				arg.qualifier |= EVariableQualifier::Centroid;

			if ( q.smooth )
				arg.qualifier |= EVariableQualifier::Smooth;

			if ( q.flat )
				arg.qualifier |= EVariableQualifier::Flat;

			if ( q.nopersp )
				arg.qualifier |= EVariableQualifier::NoPerspective;

			if ( q.patch )
				arg.qualifier |= EVariableQualifier::Patch;

			if ( q.sample )
				arg.qualifier |= EVariableQualifier::Sample;
			
			if ( q.specConstant or q.hasSpecConstantId() ) {
				arg.qualifier  |= EVariableQualifier::Specialization;
				arg.specConstID = q.layoutSpecConstantId;
			}

			CHECK_ERR( (q.coherent + q.volatil + q.restrict + q.readonly + q.writeonly) <= 1 );

			if ( q.coherent )
				arg.memoryModel = EShaderMemoryModel::Coherent;

			if ( q.volatil )
				arg.memoryModel = EShaderMemoryModel::Volatile;

			if ( q.restrict )
				arg.memoryModel = EShaderMemoryModel::Restrict;

			if ( q.readonly )
				arg.memoryModel = EShaderMemoryModel::ReadOnly;

			if ( q.writeonly )
				arg.memoryModel = EShaderMemoryModel::WriteOnly;

			switch ( q.storage )
			{
				//case glslang::TStorageQualifier::EvqTemporary :		arg.qualifier |= EVariableQualifier::Local;		break;	// not needed here
				//case glslang::TStorageQualifier::EvqGlobal :			break;
				case glslang::TStorageQualifier::EvqConst :				arg.qualifier |= EVariableQualifier::Constant;	break;
				case glslang::TStorageQualifier::EvqVaryingIn :			arg.qualifier |= EVariableQualifier::In;		break;
				case glslang::TStorageQualifier::EvqVaryingOut :		arg.qualifier |= EVariableQualifier::Out;		break;
				case glslang::TStorageQualifier::EvqUniform :			arg.qualifier |= EVariableQualifier::Uniform;	break;
				//case glslang::TStorageQualifier::EvqBuffer :			break;
				case glslang::TStorageQualifier::EvqShared :			arg.qualifier |= EVariableQualifier::Shared;	break;

				// function arguments
				case glslang::TStorageQualifier::EvqIn :				arg.qualifier |= EVariableQualifier::InArg;		break;
				case glslang::TStorageQualifier::EvqOut :				arg.qualifier |= EVariableQualifier::OutArg;	break;
				case glslang::TStorageQualifier::EvqInOut :				arg.qualifier |= EVariableQualifier::InArg;
																		arg.qualifier |= EVariableQualifier::OutArg;	break;
				case glslang::TStorageQualifier::EvqConstReadOnly :		arg.qualifier |= EVariableQualifier::InArg;
																		arg.qualifier |= EVariableQualifier::Constant;	break;
			}

			switch ( q.precision )
			{
				case glslang::TPrecisionQualifier::EpqNone :			arg.precision = EPrecision::Default;	break;
				case glslang::TPrecisionQualifier::EpqLow :				arg.precision = EPrecision::Low;		break;
				case glslang::TPrecisionQualifier::EpqMedium :			arg.precision = EPrecision::Medium;		break;
				case glslang::TPrecisionQualifier::EpqHigh :			arg.precision = EPrecision::High;		break;
				default :												RETURN_ERR( "unknown precision type!" );
			}
		}

		// copy from parent
		if ( parent )
		{
			arg.qualifier.Or( EVariableQualifier::In, parent->qualifier[EVariableQualifier::In] );
			arg.qualifier.Or( EVariableQualifier::Out, parent->qualifier[EVariableQualifier::Out] );
			arg.qualifier.Or( EVariableQualifier::InArg, parent->qualifier[EVariableQualifier::InArg] );
			arg.qualifier.Or( EVariableQualifier::OutArg, parent->qualifier[EVariableQualifier::OutArg] );
			arg.qualifier.Or( EVariableQualifier::Shared, parent->qualifier[EVariableQualifier::Shared] );
			arg.qualifier.Or( EVariableQualifier::BuiltIn, parent->qualifier[EVariableQualifier::BuiltIn] );
			
			CHECK_ERR( not parent->qualifier[EVariableQualifier::Specialization] );

			//if ( arg.memoryModel == EShaderMemoryModel::Default )
			//	arg.memoryModel = parent->memoryModel;

			if ( arg.precision == EPrecision::Default )
				arg.precision = parent->precision;
		}

		return true;
	}
	
/*
=================================================
	_DeserializeFunctionArg
=================================================
*/
	bool ShaderCompiler::_DeserializeFunctionArg (TIntermNode* node, glslang::TType const &type, glslang::TSourceLoc const &loc,
												  const DeserializedShader::FunctionArgument *parent, OUT DeserializedShader::FunctionArgument &result)
	{
		auto const&		qual = type.getQualifier();

		switch ( qual.storage )
		{
			case glslang::TStorageQualifier::EvqIn :			// argument
			case glslang::TStorageQualifier::EvqOut :			// argument
			case glslang::TStorageQualifier::EvqInOut :			// argument
			case glslang::TStorageQualifier::EvqConstReadOnly :	// argument
			case glslang::TStorageQualifier::EvqGlobal :		// result
			{
				CHECK_ERR( _DeserializeVariable( node, type, loc, parent, OUT result ) );
				
				if ( type.isStruct() )
				{
					auto const&	type_list = *type.getStruct();
			
					result.type		= EShaderVariable::Struct;
					result.typeName = type.getTypeName().c_str();
					result.fields.Reserve( result.fields.Count() + type_list.size() );

					FOR( i, type_list )
					{
						DeserializedShader::FunctionArgument	arg;
						CHECK_ERR( _DeserializeFunctionArg( null, *type_list[i].type, type_list[i].loc, &result, OUT arg ) );
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
	_DeserializeBufferVariable
=================================================
*/
	bool ShaderCompiler::_DeserializeBufferVariable (TIntermNode* node, glslang::TType const &type, glslang::TSourceLoc const &loc,
													 const DeserializedShader::BufferVariable *parent, OUT DeserializedShader::BufferVariable &var)
	{
		auto const&		qual = type.getQualifier();

		CHECK_ERR( _DeserializeVariable( node, type, loc, parent, OUT var ) );

		var.offset	= BytesU( qual.hasOffset() ? qual.layoutOffset : -1 );
		var.align	= BytesU( qual.hasAlign() ? qual.layoutAlign : 0 );
		
		if ( type.isStruct() )
		{
			auto const&	type_list = *type.getStruct();
			
			var.type		= EShaderVariable::Struct;
			var.typeName	= type.getTypeName().c_str();
			var.fields.Reserve( var.fields.Count() + type_list.size() );

			FOR( i, type_list )
			{
				DeserializedShader::BufferVariable	bufvar;
				CHECK_ERR( _DeserializeBufferVariable( null, *type_list[i].type, type_list[i].loc, &var, OUT bufvar ) );
				var.fields.PushBack( RVREF(bufvar) );
			}
		}
		return true;
	}
	
/*
=================================================
	_DeserializeIOVariable
=================================================
*/
	bool ShaderCompiler::_DeserializeIOVariable (TIntermNode* node, glslang::TType const &type, glslang::TSourceLoc const &loc,
												 const DeserializedShader::IOVariable *parent, OUT DeserializedShader::IOVariable &var)
	{
		auto const&		qual = type.getQualifier();
		
		CHECK_ERR( _DeserializeVariable( node, type, loc, parent, OUT var ) );

		var.location = (qual.hasLocation() ? qual.layoutLocation : UMax);

		if ( type.isStruct() )
		{
			auto const&	type_list = *type.getStruct();
			
			var.type		= EShaderVariable::Struct;
			var.typeName	= type.getTypeName().c_str();
			var.fields.Reserve( var.fields.Count() + type_list.size() );

			FOR( i, type_list )
			{
				DeserializedShader::IOVariable	iovar;
				CHECK_ERR( _DeserializeIOVariable( null, *type_list[i].type, type_list[i].loc, &var, OUT iovar ) );
				var.fields.PushBack( RVREF(iovar) );
			}
		}
		return true;
	}
	
/*
=================================================
	_DeserializeUniform
=================================================
*/
	bool ShaderCompiler::_DeserializeUniform (TIntermNode* node, glslang::TType const &type, glslang::TSourceLoc const &loc,
											  const DeserializedShader::Uniform *parent, OUT DeserializedShader::Uniform &result)
	{
		auto const&		qual = type.getQualifier();

		CHECK_ERR( _DeserializeVariable( node, type, loc, parent, OUT result ) );

		result.location	= (qual.hasLocation() ? qual.layoutLocation : UMax);
		
		CHECK_ERR( not type.isStruct() );	// uniform is a single uniform declaration, struct must be parsed as block/buffer
		return true;
	}
	
/*
=================================================
	_DeserializeConstant
=================================================
*/
	bool ShaderCompiler::_DeserializeConstant (TIntermNode* node, glslang::TType const &type, glslang::TSourceLoc const &loc,
												OUT DeserializedShader::Constant &result)
	{
		CHECK_ERR( _DeserializeVariable( node, type, loc, null, OUT result ) );
		CHECK_ERR( not type.isStruct() );	// constant must be scalar/vector/matrix type!

		glslang::TIntermSymbol*	symb = node->getAsSymbolNode();

		CHECK_ERR( symb );	// must be not null if it is constant
		ASSERT( not EShaderVariable::IsStruct( result.type ) );		// TODO: support structures

		CHECK_ERR( DeserializeConstant::Process( result.type, symb->getConstArray(), OUT result.values ) );
		return true;
	}

/*
=================================================
	_DeserializeExternalObjects
=================================================
*/
	bool ShaderCompiler::_DeserializeExternalObjects (TIntermNode* node, INOUT DeserializedShader &result)
	{
		glslang::TIntermTyped *			tnode	= node->getAsTyped();
		auto const&						type	= tnode->getType();
		auto const&						qual	= type.getQualifier();
		DeserializedShader::_Variable	var;

		CHECK_ERR( _DeserializeVariable( tnode, type, tnode->getLoc(), null, OUT var ) );

		if ( type.getBasicType() == glslang::TBasicType::EbtSampler )
		{
			// sampler
			if ( type.getSampler().isCombined() )
			{
				DeserializedShader::TextureUniform	sampler;
				sampler.line			= var.line;
				sampler.column			= var.column;
				sampler.name			= var.name;
				sampler.unit			= qual.hasBinding() ? qual.layoutBinding : UMax;
				sampler.descriptorSet	= qual.hasSet() ? qual.layoutSet : UMax;
				sampler.format			= EShaderVariable::ToPixelFormatClass( var.type );
				CHECK_ERR( EShaderVariable::ToTexture( var.type, OUT sampler.imageType, OUT sampler.isShadow ) );

				result._textures.PushBack( RVREF(sampler) );
				return true;
			}

			// image
			if ( type.getSampler().isImage() )
			{
				DeserializedShader::ImageUniform	image;
				image.line			= var.line;
				image.column		= var.column;
				image.name			= var.name;
				image.unit			= qual.hasBinding() ? qual.layoutBinding : UMax;
				image.descriptorSet	= qual.hasSet() ? qual.layoutSet : UMax;
				image.format		= ConvertImageLayoutFormat( qual.layoutFormat );	//EShaderVariable::ToPixelFormat( var.type );
				image.memoryModel	= var.memoryModel;

				bool	is_shadow;
				CHECK_ERR( EShaderVariable::ToTexture( var.type, OUT image.imageType, OUT is_shadow ) );
				ASSERT( not is_shadow );	// not supported for image

				result._images.PushBack( RVREF(image) );
				return true;
			}

			// TODO: subpass
		}

		// push constants
		if ( qual.layoutPushConstant )
		{
			DeserializedShader::PushConstant	pc;

			result._pushConstants.PushBack( RVREF(pc) );
			return true;
		}

		if ( type.getBasicType() == glslang::TBasicType::EbtBlock		and
			 (qual.storage == glslang::TStorageQualifier::EvqUniform	or 
			  qual.storage == glslang::TStorageQualifier::EvqBuffer) )
		{
			CHECK_ERR( type.isStruct() );

			auto const&				type_list	= *type.getStruct();
			EVariablePacking::type	packing		= EVariablePacking::Unknown;
			
			Array<DeserializedShader::BufferVariable>	struct_values;
			struct_values.Reserve( type_list.size() );

			FOR( i, type_list )
			{
				DeserializedShader::BufferVariable	bufvar;
				CHECK_ERR( _DeserializeBufferVariable( tnode, *type_list[i].type, type_list[i].loc, null, OUT bufvar ) );
				struct_values.PushBack( RVREF(bufvar) );
			}
			
			// get buffer packing
			switch ( qual.layoutPacking )
			{
				case glslang::TLayoutPacking::ElpNone :			break;
				case glslang::TLayoutPacking::ElpShared :		packing = EVariablePacking::Shared;	break;
				case glslang::TLayoutPacking::ElpStd140 :		packing = EVariablePacking::Std140;	break;
				case glslang::TLayoutPacking::ElpStd430 :		packing = EVariablePacking::Std430;	break;
				case glslang::TLayoutPacking::ElpPacked :		packing = EVariablePacking::Packed;	break;
			}

			// uniform block
			if ( qual.storage == glslang::TStorageQualifier::EvqUniform )
			{
				DeserializedShader::UniformBuffer	buf;
				buf.line			= var.line;
				buf.column			= var.column;
				buf.name			= var.name;
				buf.typeName		= type.getTypeName().c_str();
				buf.bindingIndex	= qual.hasBinding() ? qual.layoutBinding : UMax;
				buf.descriptorSet	= qual.hasSet() ? qual.layoutSet : UMax;
				buf.packing			= packing;
				buf.size			= 0_b;
				buf.fields			= RVREF(struct_values);

				result._uniformBuffers.PushBack( RVREF(buf) );
				return true;
			}

			// storage block
			if ( qual.storage == glslang::TStorageQualifier::EvqBuffer )
			{
				DeserializedShader::StorageBuffer	buf;
				buf.line			= var.line;
				buf.column			= var.column;
				buf.name			= var.name;
				buf.typeName		= type.getTypeName().c_str();
				buf.bindingIndex	= qual.hasBinding() ? qual.layoutBinding : UMax;
				buf.descriptorSet	= qual.hasSet() ? qual.layoutSet : UMax;
				buf.memoryModel		= var.memoryModel;
				buf.packing			= packing;
				buf.size			= 0_b;
				buf.fields			= RVREF(struct_values);

				result._storageBuffers.PushBack( RVREF(buf) );
				return true;
			}
		}

		// shader input
		if ( qual.storage == glslang::TStorageQualifier::EvqVaryingIn )
		{
			DeserializedShader::IOVariable	iovar;
			CHECK_ERR( _DeserializeIOVariable( tnode, type, tnode->getLoc(), null, OUT iovar ) );
			result._variables.PushBack( RVREF(iovar) );
			return true;
		}
		
		// shader output
		if ( qual.storage == glslang::TStorageQualifier::EvqVaryingOut )
		{
			DeserializedShader::IOVariable	iovar;
			CHECK_ERR( _DeserializeIOVariable( tnode, type, tnode->getLoc(), null, OUT iovar ) );
			result._variables.PushBack( RVREF(iovar) );
			return true;
		}

		// uniform
		if ( qual.storage == glslang::TStorageQualifier::EvqUniform )
		{
			DeserializedShader::Uniform		un;
			CHECK_ERR( _DeserializeUniform( tnode, type, tnode->getLoc(), null, OUT un ) );
			result._uniforms.PushBack( RVREF(un) );
			return true;
		}

		// global constant
		if ( qual.isConstant() )
		{
			DeserializedShader::Constant	cnst;
			CHECK_ERR( _DeserializeConstant( tnode, type, tnode->getLoc(), OUT cnst ) );
			result._constants.PushBack( RVREF(cnst) );
			return true;
		}

		// skip builtin
		if ( type.isBuiltIn() )
			return true;

		return true;	// unknown type, maybe it is an error
	}
	
/*
=================================================
	ConvertTessellationSpacing
=================================================
*/
	static ETessellationSpacing::type  ConvertTessellationSpacing (glslang::TVertexSpacing type)
	{
		switch ( type )
		{
			//case glslang::TVertexSpacing::EvsNone :			
			case glslang::TVertexSpacing::EvsEqual :			return ETessellationSpacing::Equal;
			case glslang::TVertexSpacing::EvsFractionalEven :	return ETessellationSpacing::FractionalEven;
			case glslang::TVertexSpacing::EvsFractionalOdd :	return ETessellationSpacing::FractionalOdd;
		}
		RETURN_ERR( "invalid tessellation spacing!" );
	}
	
/*
=================================================
	ConvertGeometryInputPrimitive
=================================================
*/
	static EGeometryInputPrimitive::type  ConvertGeometryInputPrimitive (glslang::TLayoutGeometry type)
	{
		switch ( type )
		{
			case glslang::TLayoutGeometry::ElgPoints :				return EGeometryInputPrimitive::Points;
			case glslang::TLayoutGeometry::ElgLines :				return EGeometryInputPrimitive::Lines;
			case glslang::TLayoutGeometry::ElgLinesAdjacency :		return EGeometryInputPrimitive::LinesAdjacency;
			case glslang::TLayoutGeometry::ElgTriangles :			return EGeometryInputPrimitive::Triangles;
			case glslang::TLayoutGeometry::ElgTrianglesAdjacency :	return EGeometryInputPrimitive::TriangleAdjacency;
		}
		RETURN_ERR( "invalid geometry input primitive type!" );
	}
	
/*
=================================================
	ConvertGeometryOutputPrimitive
=================================================
*/
	static EGeometryOutputPrimitive::type  ConvertGeometryOutputPrimitive (glslang::TLayoutGeometry type)
	{
		switch ( type )
		{
			case glslang::TLayoutGeometry::ElgPoints :				return EGeometryOutputPrimitive::Points;
			case glslang::TLayoutGeometry::ElgLineStrip :			return EGeometryOutputPrimitive::LineStrip;
			case glslang::TLayoutGeometry::ElgTriangleStrip :		return EGeometryOutputPrimitive::TriangleStrip;
		}
		RETURN_ERR( "invalid geometry output primitive type!" );
	}
	
/*
=================================================
	ConvertTessellationInputPrimitive
=================================================
*/
	static ETessellationInputPrimitive::type  ConvertTessellationInputPrimitive (glslang::TLayoutGeometry type, bool pointMode)
	{
		if ( pointMode )
			return ETessellationInputPrimitive::Points;

		switch ( type )
		{
			//case glslang::TLayoutGeometry::ElgPoints :	return ETessellationInputPrimitive::Points;
			case glslang::TLayoutGeometry::ElgIsolines :	return ETessellationInputPrimitive::Isolines;
			case glslang::TLayoutGeometry::ElgTriangles :	return ETessellationInputPrimitive::Triangles;
			case glslang::TLayoutGeometry::ElgQuads :		return ETessellationInputPrimitive::Quads;
		}
		RETURN_ERR( "invalid tessellation primitive type!" );
	}

/*
=================================================
	_ProcessShaderInfo
=================================================
*/
	bool ShaderCompiler::_ProcessShaderInfo (glslang::TIntermediate const* intermediate, INOUT DeserializedShader &result)
	{
		switch ( result._shaderType )
		{
			case EShader::Vertex :
			{
				break;
			}

			case EShader::TessControl :
			{
				result._tessControl.patchSize		= intermediate->getVertices();
				break;
			}

			case EShader::TessEvaluation :
			{
				result._tessEvaluation.spacing		= ConvertTessellationSpacing( intermediate->getVertexSpacing() );
				result._tessEvaluation.ccw			= not (intermediate->getVertexOrder() == glslang::TVertexOrder::EvoCw);
				result._tessEvaluation.primitive	= ConvertTessellationInputPrimitive( intermediate->getInputPrimitive(), intermediate->getPointMode() );
				break;
			}

			case EShader::Geometry :
			{
				result._geometry.invocations		= intermediate->getInvocations();
				result._geometry.maxOutputVertices	= intermediate->getVertices();
				result._geometry.inputPrimitive		= ConvertGeometryInputPrimitive( intermediate->getInputPrimitive() );
				result._geometry.outputPrimitive	= ConvertGeometryOutputPrimitive( intermediate->getOutputPrimitive() );
				break;
			}

			case EShader::Fragment :
			{
				result._fragment.flags |= (intermediate->getOriginUpperLeft() ? EFragmentShaderParams::OriginUpperLeft : EFragmentShaderParams::None);
				result._fragment.flags |= (intermediate->getPixelCenterInteger() ? EFragmentShaderParams::PixelCenterInteger : EFragmentShaderParams::None);
				result._fragment.flags |= (intermediate->getEarlyFragmentTests() ? EFragmentShaderParams::EarlyFragmentTests : EFragmentShaderParams::None);
				result._fragment.flags |= (intermediate->getPostDepthCoverage() ? EFragmentShaderParams::PostDepthCoverage : EFragmentShaderParams::None);
				result._fragment.flags |= (intermediate->isDepthReplacing() ? EFragmentShaderParams::DepthExport : EFragmentShaderParams::None);
				// getBlendEquations
				
				switch ( intermediate->getDepth() )
				{
					case glslang::TLayoutDepth::EldNone :		break;
					case glslang::TLayoutDepth::EldAny :		result._fragment.flags |= EFragmentShaderParams::ConservativeDepth_Any;			break;
					case glslang::TLayoutDepth::EldGreater :	result._fragment.flags |= EFragmentShaderParams::ConservativeDepth_Greater;		break;
					case glslang::TLayoutDepth::EldLess :		result._fragment.flags |= EFragmentShaderParams::ConservativeDepth_Less;		break;
					case glslang::TLayoutDepth::EldUnchanged :	result._fragment.flags |= EFragmentShaderParams::ConservativeDepth_Unchanged;	break;
				}

				break;
			}

			case EShader::Compute :
			{
				result._compute.localSize.x	= intermediate->getLocalSize(0);	// getLocalSizeSpecId
				result._compute.localSize.y = intermediate->getLocalSize(1);
				result._compute.localSize.z	= intermediate->getLocalSize(2);
				break;
			}
		}
		return true;
	}

}	// PipelineCompiler
