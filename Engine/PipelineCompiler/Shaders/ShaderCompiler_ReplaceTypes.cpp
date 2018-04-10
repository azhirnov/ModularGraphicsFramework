// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_Utils.h"

#if 0
#	define _SLOG( ... )		LOG( __VA_ARGS__ )
#else
#	define _SLOG( ... )
#endif

namespace PipelineCompiler
{
	using ReplaceStructTypesFunc_t	= ShaderCompiler::ReplaceStructTypesFunc_t;
	using FieldTypeInfo				= ShaderCompiler::FieldTypeInfo;
	
	struct Replacer
	{
		ReplaceStructTypesFunc_t	func;
		Array<TIntermNode*>			nodes;
		uint						uid		= 0;
	};

	static bool RecursiveProcessAggregateNode (TIntermNode* node, Replacer &replacer);
	static bool RecursiveProcessNode (TIntermNode* node, Replacer &replacer);
	static bool RecursiveProcessBranchNode (TIntermNode* node, Replacer &replacer);
	static bool RecursiveProcessSwitchNode (TIntermNode* node, Replacer &replacer);
	static bool RecursiveProcessConstUnionNode (TIntermNode* node, Replacer &replacer);
	static bool RecursiveProcessSelectionNode (TIntermNode* node, Replacer &replacer);
	static bool RecursiveProcessMethodNode (TIntermNode* node, Replacer &replacer);
	static bool RecursiveProcessSymbolNode (TIntermNode* node, Replacer &replacer);
	static bool RecursiveProcessTypedNode (TIntermNode* node, Replacer &replacer);
	static bool RecursiveProcessOperatorNode (TIntermNode* node, Replacer &replacer);
	static bool RecursiveProcessUnaryNode (TIntermNode* node, Replacer &replacer);
	static bool RecursiveProcessBinaryNode (TIntermNode* node, Replacer &replacer);
	static bool ReplaceTypesInNode (glslang::TIntermBinary* node, Replacer &replacer);
	static bool ReplaceTypesInNode2 (glslang::TIntermBinary* node, const FieldTypeInfo &oldField, const FieldTypeInfo &newField, const Replacer &replacer);
	static bool ReplaceTypesInNode2Arr (glslang::TIntermBinary* node, const FieldTypeInfo &oldField, const FieldTypeInfo &newField, const Replacer &replacer);
	static bool RecursiveProccessLoop (TIntermNode* node, Replacer &replacer);

/*
=================================================
	_ReplaceTypes
=================================================
*/
	bool ShaderCompiler::_ReplaceTypes (const _GLSLangResult &glslangData, const Config &cfg) const
	{
		if ( not cfg.typeReplacer )
			return true;
		
		const glslang::TIntermediate* intermediate = glslangData.prog.getIntermediate( glslangData.shader->getStage() );
		CHECK_ERR( intermediate );

		TIntermNode*	root	= intermediate->getTreeRoot();
		Replacer		replacer;

		replacer.func = cfg.typeReplacer;

		CHECK_ERR( RecursiveProcessAggregateNode( root, replacer ) );
		return true;
	}
	
/*
=================================================
	RecursiveProcessBinaryNode
=================================================
*/
	static bool RecursiveProcessBinaryNode (TIntermNode* node, Replacer &replacer)
	{
		glslang::TIntermBinary*		binary = node->getAsBinaryNode();
		
		_SLOG( ("TIntermBinary ("_str << uid << ") op:" << uint(binary->getOp()) << "; " << binary->getCompleteString().c_str()).cstr(), ELog::Debug );

		if ( binary->getOp() == glslang::TOperator::EOpIndexDirectStruct	or
			 binary->getOp() == glslang::TOperator::EOpIndexDirect			or
			 binary->getOp() == glslang::TOperator::EOpIndexIndirect )
		{
			CHECK_ERR( ReplaceTypesInNode( binary, replacer ) );
		}

		replacer.nodes << binary;
		CHECK_ERR( RecursiveProcessNode( binary->getLeft(), replacer ) );
		CHECK_ERR( RecursiveProcessNode( binary->getRight(), replacer ) );

		CHECK( replacer.nodes.Back() == binary );
		replacer.nodes.PopBack();
		return true;
	}
	
/*
=================================================
	ReplaceTypesInNode
=================================================
*/
	static bool ReplaceTypesInNode (glslang::TIntermBinary* binary, Replacer &replacer)
	{
		const glslang::TType&	type		= binary->getType();
		FieldTypeInfo			field;
		String					st_type;
		bool					has_index	= false;
		
		// skip builtin
		if ( type.isBuiltIn() )
			return true;

		// get field info
		if ( &type.getFieldName() != null ) {
			field.name		= type.getFieldName().data();
		}
		if ( &type.getTypeName() != null ) {
			field.typeName	= type.getTypeName().data();
			field.type		= type.getBasicType() == glslang::TBasicType::EbtBlock ?
									(type.getQualifier().storage == glslang::TStorageQualifier::EvqBuffer ?
										EShaderVariable::StorageBlock :
										EShaderVariable::UniformBlock) :
									EShaderVariable::Struct;
		}
		else {
			field.type		= ConvertBasicType( type.getBasicType(), type.getVectorSize(), type.getMatrixCols(), type.getMatrixRows() );
		}

		// get struct info
		if ( binary->getLeft() and binary->getLeft()->getAsTyped() )
		{
			glslang::TIntermTyped*	left	= binary->getLeft()->getAsTyped();
			const glslang::TType&	ltype	= left->getType();

			if ( (ltype.getBasicType() == glslang::TBasicType::EbtStruct or
				 ltype.getBasicType() == glslang::TBasicType::EbtBlock) and
				 &ltype.getTypeName() != null and
				 not ltype.isBuiltIn() )
			{
				st_type = ltype.getTypeName().data();
			}
		}

		// get field index
		if ( binary->getRight() and binary->getRight()->getAsConstantUnion() )
		{
			const glslang::TConstUnionArray&	arr = binary->getRight()->getAsConstantUnion()->getConstArray();

			if ( arr.size() == 1 and
				(arr[0].getType() == glslang::TBasicType::EbtInt or
				 arr[0].getType() == glslang::TBasicType::EbtUint) )
			{
				field.index	= arr[0].getIConst();
				has_index	= true;
			}
		}

		if ( field.name.Empty() or st_type.Empty() or not has_index )
			return true;	// continue parsing

		// skip scalar/vector with swizzle operator
		{
			TIntermNode *			root		= replacer.nodes.Back();
			glslang::TIntermBinary*	bin_root	= root->getAsBinaryNode();

			if ( bin_root and (
				 bin_root->getOp() == glslang::TOperator::EOpVectorSwizzle or
				(bin_root->getOp() == glslang::TOperator::EOpIndexDirect and not bin_root->getLeft()->isArray() and
				 (bin_root->getLeft()->isScalar() or bin_root->getLeft()->isVector()))
				) )
			{
				return true;
			}
		}

		//LOG( "TIntermBinary: struct field access: "_str << st_type << "::" << field.name, ELog::Info );

		const FieldTypeInfo		src_field = field;

		if ( not replacer.func( st_type, INOUT field ) or src_field == field )
			return true;	// not replaced or not changed

		if ( type.getArraySizes() == null ) {
			CHECK_ERR( ReplaceTypesInNode2( binary, src_field, field, replacer ) );
		} else {
			CHECK_ERR( ReplaceTypesInNode2Arr( binary, src_field, field, replacer ) );
		}
		return true;
	}
	
/*
=================================================
	ConvertToBasicType
=================================================
*/
	static glslang::TBasicType  ConvertToBasicType (EShaderVariable::type valueType)
	{
		switch ( EShaderVariable::ToScalar( valueType ) )
		{
			case EShaderVariable::Bool :		return glslang::TBasicType::EbtBool;
			case EShaderVariable::Int :			return glslang::TBasicType::EbtInt;
			case EShaderVariable::UInt :		return glslang::TBasicType::EbtUint;
			case EShaderVariable::Long :		return glslang::TBasicType::EbtInt64;
			case EShaderVariable::ULong :		return glslang::TBasicType::EbtUint64;
			case EShaderVariable::Float :		return glslang::TBasicType::EbtFloat;
			case EShaderVariable::Double :		return glslang::TBasicType::EbtDouble;
		}
		RETURN_ERR( "unsupported type!", glslang::TBasicType::EbtNumTypes );
	}
	
/*
=================================================
	ConvertToConstructor
=================================================
*/
	static glslang::TOperator  ConvertToConstructor (EShaderVariable::type valueType)
	{
		switch ( valueType )
		{
			case EShaderVariable::Bool :		return glslang::TOperator::EOpConstructBool;
			case EShaderVariable::Bool2 :		return glslang::TOperator::EOpConstructBVec2;
			case EShaderVariable::Bool3 :		return glslang::TOperator::EOpConstructBVec3;
			case EShaderVariable::Bool4 :		return glslang::TOperator::EOpConstructBVec4;

			case EShaderVariable::Int :			return glslang::TOperator::EOpConstructInt;
			case EShaderVariable::Int2 :		return glslang::TOperator::EOpConstructIVec2;
			case EShaderVariable::Int3 :		return glslang::TOperator::EOpConstructIVec3;
			case EShaderVariable::Int4 :		return glslang::TOperator::EOpConstructIVec4;

			case EShaderVariable::UInt :		return glslang::TOperator::EOpConstructUint;
			case EShaderVariable::UInt2 :		return glslang::TOperator::EOpConstructUVec2;
			case EShaderVariable::UInt3 :		return glslang::TOperator::EOpConstructUVec3;
			case EShaderVariable::UInt4 :		return glslang::TOperator::EOpConstructUVec4;

			case EShaderVariable::Long :		return glslang::TOperator::EOpConstructInt64;
			case EShaderVariable::Long2 :		return glslang::TOperator::EOpConstructI64Vec2;
			case EShaderVariable::Long3 :		return glslang::TOperator::EOpConstructI64Vec3;
			case EShaderVariable::Long4 :		return glslang::TOperator::EOpConstructI64Vec4;

			case EShaderVariable::ULong :		return glslang::TOperator::EOpConstructUint64;
			case EShaderVariable::ULong2 :		return glslang::TOperator::EOpConstructU64Vec2;
			case EShaderVariable::ULong3 :		return glslang::TOperator::EOpConstructU64Vec3;
			case EShaderVariable::ULong4 :		return glslang::TOperator::EOpConstructU64Vec4;

			case EShaderVariable::Float :		return glslang::TOperator::EOpConstructFloat;
			case EShaderVariable::Float2 :		return glslang::TOperator::EOpConstructVec2;
			case EShaderVariable::Float3 :		return glslang::TOperator::EOpConstructVec3;
			case EShaderVariable::Float4 :		return glslang::TOperator::EOpConstructVec4;

			case EShaderVariable::Double :		return glslang::TOperator::EOpConstructDouble;
			case EShaderVariable::Double2 :		return glslang::TOperator::EOpConstructDVec2;
			case EShaderVariable::Double3 :		return glslang::TOperator::EOpConstructDVec3;
			case EShaderVariable::Double4 :		return glslang::TOperator::EOpConstructDVec4;
				
			case EShaderVariable::Float2x2 :	return glslang::TOperator::EOpConstructMat2x2;
			case EShaderVariable::Float2x3 :	return glslang::TOperator::EOpConstructMat2x3;
			case EShaderVariable::Float2x4 :	return glslang::TOperator::EOpConstructMat2x4;
			case EShaderVariable::Float3x2 :	return glslang::TOperator::EOpConstructMat3x2;
			case EShaderVariable::Float3x3 :	return glslang::TOperator::EOpConstructMat3x3;
			case EShaderVariable::Float3x4 :	return glslang::TOperator::EOpConstructMat3x4;
			case EShaderVariable::Float4x2 :	return glslang::TOperator::EOpConstructMat4x2;
			case EShaderVariable::Float4x3 :	return glslang::TOperator::EOpConstructMat4x3;
			case EShaderVariable::Float4x4 :	return glslang::TOperator::EOpConstructMat4x4;

			case EShaderVariable::Double2x2 :	return glslang::TOperator::EOpConstructDMat2x2;
			case EShaderVariable::Double2x3 :	return glslang::TOperator::EOpConstructDMat2x3;
			case EShaderVariable::Double2x4 :	return glslang::TOperator::EOpConstructDMat2x4;
			case EShaderVariable::Double3x2 :	return glslang::TOperator::EOpConstructDMat3x2;
			case EShaderVariable::Double3x3 :	return glslang::TOperator::EOpConstructDMat3x3;
			case EShaderVariable::Double3x4 :	return glslang::TOperator::EOpConstructDMat3x4;
			case EShaderVariable::Double4x2 :	return glslang::TOperator::EOpConstructDMat4x2;
			case EShaderVariable::Double4x3 :	return glslang::TOperator::EOpConstructDMat4x3;
			case EShaderVariable::Double4x4 :	return glslang::TOperator::EOpConstructDMat4x4;
		}
		RETURN_ERR( "unsupported type!", glslang::TOperator::EOpNull );
	}

/*
=================================================
	ReplaceNodeInRoot
=================================================
*/
	static bool ReplaceNodeInRoot (TIntermNode *root, TIntermNode* srcNode, TIntermNode* dstNode)
	{
		if ( root->getAsAggregate() )
		{
			glslang::TIntermAggregate*	aggr = root->getAsAggregate();

			FOR( i, aggr->getSequence() ) {
				if ( aggr->getSequence()[i] == srcNode ) {
					aggr->getSequence()[i] = dstNode;
					return true;
				}
			}
		}

		if ( root->getAsBinaryNode() )
		{
			glslang::TIntermBinary*		binary = root->getAsBinaryNode();

			if ( binary->getLeft() == srcNode )
			{
				CHECK_ERR( dstNode->getAsTyped() );
				binary->setLeft( dstNode->getAsTyped() );
				return true;
			}
			if ( binary->getRight() == srcNode )
			{
				CHECK_ERR( dstNode->getAsTyped() );
				binary->setRight( dstNode->getAsTyped() );
				return true;
			}
		}

		if ( root->getAsBranchNode() )
		{
			glslang::TIntermBranch*		branch = root->getAsBranchNode();

			if ( branch->getExpression() == srcNode ) {
				RETURN_ERR( "go to root of 'branch' node and create new branch node with 'dstNode' as expression!" );
			}
		}
		
		if ( root->getAsSwitchNode() )
		{
			glslang::TIntermSwitch*		sw = root->getAsSwitchNode();

			if ( sw->getCondition() == srcNode ) {
				RETURN_ERR( "go to root of 'switch' node and create new switch node with 'dstNode' as condition!" );
			}

			// 'body' is aggregate node and cannot be a binary node 'srcNode'
		}

		if ( root->getAsSelectionNode() )
		{
			glslang::TIntermSelection*	selection = root->getAsSelectionNode();

			if ( selection->getCondition() == srcNode ) {
				RETURN_ERR( "go to root of 'selection' node and create new selection node with 'dstNode' as condition!" );
			}
			if ( selection->getTrueBlock() == srcNode ) {
				RETURN_ERR( "go to root of 'selection' node and create new selection node with 'dstNode' as true block!" );
			}
			if ( selection->getFalseBlock() == srcNode ) {
				RETURN_ERR( "go to root of 'selection' node and create new selection node with 'dstNode' as false block!" );
			}
		}

		if ( root->getAsUnaryNode() )
		{
			glslang::TIntermUnary*		unary = root->getAsUnaryNode();

			if ( unary->getOperand() == srcNode )
			{
				CHECK_ERR( dstNode->getAsTyped() );
				unary->setOperand( dstNode->getAsTyped() );
				return true;
			}
		}

		RETURN_ERR( "can't replace nodes!" );
	}
	
/*
=================================================
	CreateSwizzleNode
=================================================
*/
	static bool CreateSwizzleNode (INOUT TIntermNode* &root, glslang::TIntermBinary* binary,
									const FieldTypeInfo &oldField, const FieldTypeInfo &newField,
									const glslang::TType &oldType, INOUT glslang::TPublicType &newType)
	{
		newType.basicType	= ConvertToBasicType( newField.type );
		newType.vectorSize	= EShaderVariable::VecSize( newField.type );
		newType.matrixCols	= 0;
		newType.matrixRows	= 0;
			
		// add constructor to convert new type to old type
		glslang::TType*				sw_type			= oldType.clone();		sw_type->makeTemporary();
		glslang::TPublicType		mask_pub_type	= newType;
		glslang::TPublicType		cu_pub_type;	cu_pub_type.init( binary->getLoc() );
				
		mask_pub_type.vectorSize = 1;
		mask_pub_type.qualifier.storage = glslang::TStorageQualifier::EvqTemporary;

		cu_pub_type.basicType = glslang::TBasicType::EbtInt;
		cu_pub_type.vectorSize = 1;
		cu_pub_type.qualifier.storage = glslang::TStorageQualifier::EvqConst;

		// all glslang objects use GC
		glslang::TIntermBinary*		swizzle_op		= new glslang::TIntermBinary( glslang::TOperator::EOpVectorSwizzle );
		glslang::TIntermAggregate*	swizzle_mask	= new glslang::TIntermAggregate();
		glslang::TType*				mask_type		= new glslang::TType( mask_pub_type );
		glslang::TType*				cu_type			= new glslang::TType( cu_pub_type );

		swizzle_op->setType( *sw_type );
		swizzle_op->setOperationPrecision( oldType.getQualifier().precision );
		swizzle_op->setLeft( binary );
		swizzle_op->setRight( swizzle_mask );
		swizzle_op->setLoc( binary->getLoc() );

		swizzle_mask->setType( *mask_type );
		swizzle_mask->setOp( glslang::TOperator::EOpSequence );
		swizzle_mask->setOperationPrecision( oldType.getQualifier().precision );
		swizzle_mask->setUserDefined();
		swizzle_mask->setOptimize( true );
		swizzle_mask->setDebug( true );
		swizzle_mask->setLoc( binary->getLoc() );
		swizzle_mask->getSequence().resize( oldType.getVectorSize() );

		FOR( i, swizzle_mask->getSequence() )
		{
			glslang::TConstUnionArray		arr(1);		arr[0].setIConst( i );
			glslang::TIntermConstantUnion*	c_union		= new glslang::TIntermConstantUnion( arr, *cu_type );

			swizzle_mask->getSequence()[i] = c_union;
		}
				
		CHECK_ERR( ReplaceNodeInRoot( root, binary, swizzle_op ) );

		root = swizzle_op;
		return true;
	}
	
/*
=================================================
	CreateLValueNode
=================================================
*/
	static bool CreateLValueNode (TIntermNode* root, const FieldTypeInfo &newField, INOUT glslang::TPublicType &newType)
	{
		newType.basicType	= ConvertToBasicType( newField.type );
		newType.vectorSize	= EShaderVariable::VecSize( newField.type );
		newType.matrixCols	= EShaderVariable::MatSize( newField.type ).x;
		newType.matrixRows	= EShaderVariable::MatSize( newField.type ).y;

		CHECK_ERR( root->getAsBinaryNode() );

		// add constructor to convert new type to old type
		glslang::TIntermBinary*		bin_root	= root->getAsBinaryNode();
		glslang::TIntermAggregate*	func_call	= new glslang::TIntermAggregate();
		glslang::TIntermTyped*		right		= bin_root->getRight();

		func_call->getSequence().resize(1);
		func_call->getSequence()[0] = right;
		func_call->setOp( ConvertToConstructor( newField.type ) );
		func_call->setOperationPrecision( right->getQualifier().precision );
		func_call->setOptimize( true );
		func_call->setDebug( true );
		func_call->setLoc( right->getLoc() );

		glslang::TPublicType	res_type;
		res_type.init( right->getLoc() );
		res_type.basicType			= newType.basicType;
		res_type.sampler			= newType.sampler;
		res_type.vectorSize			= newType.vectorSize;
		res_type.matrixCols			= newType.matrixCols;
		res_type.matrixRows			= newType.matrixRows;
		res_type.qualifier.storage	= glslang::TStorageQualifier::EvqTemporary;

		func_call->setType( *(new glslang::TType( res_type )) );

		bin_root->setRight( func_call );
		return true;
	}
	
/*
=================================================
	CreateConstructorNode
=================================================
*/
	static bool CreateConstructorNode (INOUT TIntermNode* &root, glslang::TIntermBinary* binary,
										const FieldTypeInfo &oldField, const FieldTypeInfo &newField,
										const glslang::TType &oldType, INOUT glslang::TPublicType &newType)
	{
		newType.basicType	= ConvertToBasicType( newField.type );
		newType.vectorSize	= EShaderVariable::VecSize( newField.type );
		newType.matrixCols	= EShaderVariable::MatSize( newField.type ).x;
		newType.matrixRows	= EShaderVariable::MatSize( newField.type ).y;

		// add constructor to convert new type to old type
		glslang::TIntermAggregate*	func_call	= new glslang::TIntermAggregate();

		func_call->getSequence().resize(1);
		func_call->getSequence()[0] = binary;
		func_call->setOp( ConvertToConstructor( oldField.type ) );
		func_call->setOperationPrecision( oldType.getQualifier().precision );
		func_call->setOptimize( true );
		func_call->setDebug( true );
		func_call->setLoc( binary->getLoc() );

		glslang::TPublicType	res_type;
		res_type.init( binary->getLoc() );
		res_type.basicType			= oldType.getBasicType();
		res_type.sampler			= oldType.getSampler();
		res_type.vectorSize			= oldType.getVectorSize();
		res_type.matrixCols			= oldType.getMatrixCols();
		res_type.matrixRows			= oldType.getMatrixRows();
		res_type.qualifier.storage	= glslang::TStorageQualifier::EvqTemporary;

		func_call->setType( *(new glslang::TType( res_type )) );

		CHECK_ERR( ReplaceNodeInRoot( root, binary, func_call ) );

		root = func_call;
		return true;
	}

/*
=================================================
	GetValueQualifier
=================================================
*/
	static void GetValueQualifier (TIntermNode* root, glslang::TIntermBinary* binary, OUT bool &isRValue, OUT bool &isOutArgument)
	{
		isOutArgument = isRValue = false;

		if ( root->getAsAggregate() )
		{
			glslang::TIntermAggregate*	aggr = root->getAsAggregate();

			if ( aggr->getOp() == glslang::TOperator::EOpFunctionCall )
			{
				const size_t	count = Min( aggr->getQualifierList().size(), aggr->getSequence().size() );

				for (size_t i = 0; i < count; ++i)
				{
					auto	qual = aggr->getQualifierList()[i];
					auto	seq  = aggr->getSequence()[i];

					if ( seq == binary and (qual == glslang::TStorageQualifier::EvqOut or qual == glslang::TStorageQualifier::EvqInOut) )
						isOutArgument = true;
				}
			}
		}

		if ( root->getAsOperator() )
		{
			switch ( root->getAsOperator()->getOp() )
			{
				case glslang::TOperator::EOpAssign :
				case glslang::TOperator::EOpAddAssign :
				case glslang::TOperator::EOpSubAssign :
				case glslang::TOperator::EOpMulAssign :
				case glslang::TOperator::EOpVectorTimesMatrixAssign :
				case glslang::TOperator::EOpVectorTimesScalarAssign :
				case glslang::TOperator::EOpMatrixTimesScalarAssign :
				case glslang::TOperator::EOpMatrixTimesMatrixAssign :
				case glslang::TOperator::EOpDivAssign :
				case glslang::TOperator::EOpModAssign :
				case glslang::TOperator::EOpAndAssign :
				case glslang::TOperator::EOpInclusiveOrAssign :
				case glslang::TOperator::EOpExclusiveOrAssign :
				case glslang::TOperator::EOpLeftShiftAssign :
				case glslang::TOperator::EOpRightShiftAssign :
					isRValue = true;
					break;
			}
		}
	}

/*
=================================================
	ReplaceTypesInNode2
=================================================
*/
	static bool ReplaceTypesInNode2 (glslang::TIntermBinary* binary, const FieldTypeInfo &oldField, const FieldTypeInfo &newField, const Replacer &replacer)
	{
		TIntermNode *			root	= replacer.nodes.Back();

		glslang::TType const&	old_type	= binary->getType();
		glslang::TPublicType	new_type;	new_type.init( binary->getLoc() );

		new_type.basicType		= old_type.getBasicType();
		new_type.sampler		= old_type.getSampler();
		new_type.qualifier		= old_type.getQualifier();
		new_type.vectorSize		= old_type.getVectorSize();
		new_type.matrixCols		= old_type.getMatrixCols();
		new_type.matrixRows		= old_type.getMatrixRows();

		// not supported
		CHECK_ERR( old_type.getArraySizes() == null );
		CHECK_ERR( not old_type.isStruct() );

		// changed name or index
		if ( oldField.name != newField.name or oldField.index != newField.index )
		{
			glslang::TIntermTyped*			right		= binary->getRight();
			glslang::TConstUnionArray		arr(1);		arr[0].setIConst( newField.index );
			glslang::TIntermConstantUnion*	const_union = new glslang::TIntermConstantUnion( arr, *right->getType().clone() );
			glslang::TIntermConstantUnion*	r_cunion	= right->getAsConstantUnion();

			CHECK_ERR( r_cunion and
					   r_cunion->getConstArray().size() == 1 and
					   r_cunion->getConstArray()[0].getType() == glslang::TBasicType::EbtInt and
					   r_cunion->getConstArray()[0].getUConst() == oldField.index );

			if ( r_cunion->isLiteral() )
				const_union->setLiteral();
			else
				const_union->setExpression();

			const_union->setLoc( right->getLoc() );

			binary->setRight( const_union );
		}

		// changed scalar/vector/matrix type of field
		if ( oldField.type != newField.type and oldField.typeName.Empty() and newField.typeName.Empty() )
		{
			// check is 'root' a assign operator or out/inout argument in function call
			bool	is_lvalue	= false;
			bool	is_out_arg	= false;

			GetValueQualifier( root, binary, OUT is_lvalue, OUT is_out_arg );


			// create swizzle operator, value may be Rvalue or Lvalue
			if ( EShaderVariable::VecSize( newField.type ) > 0 )
			{
				CHECK_ERR( CreateSwizzleNode( INOUT root, binary, oldField, newField, old_type, INOUT new_type ) );
			}
			else
			// change other type for compatibility
			if ( is_lvalue )
			{
				CHECK_ERR( CreateLValueNode( root, newField, INOUT new_type ) );
			}
			else
			if ( is_out_arg )
			{
				RETURN_ERR( "output argument is not supported if type is not a scalar or vector!" );
			}
			else
			// create constructor, value must be the Rvalue
			{
				CHECK_ERR( CreateConstructorNode( INOUT root, binary, oldField, newField, old_type, INOUT new_type ) );
			}
		}

		// changed struct type of field
		if ( not oldField.typeName.Empty() or not newField.typeName.Empty() )
		{
			RETURN_ERR( "struct fields not supported yet!" );
		}

		// set new type to node
		glslang::TType*		ttype	= new glslang::TType( new_type );	// all glslang objects use GC

		ttype->setFieldName( glslang::TString( newField.name.cstr() ) );

		if ( not newField.typeName.Empty() )
			ttype->setTypeName( glslang::TString( newField.typeName.cstr() ) );

		binary->setType( *ttype );
		return true;
	}
	
/*
=================================================
	ReplaceTypesInNode2Arr
=================================================
*/
	static bool ReplaceTypesInNode2Arr (glslang::TIntermBinary* node, const FieldTypeInfo &oldField, const FieldTypeInfo &newField, const Replacer &replacer)
	{
		CHECK_ERR( replacer.nodes.Count() >= 2 );

		TIntermNode *	root1  = *(replacer.nodes.End() - 1);
		TIntermNode *	root2 = *(replacer.nodes.End() - 2);

		if ( root1 == null or
			 root2 == null or
			 root1->getAsBinaryNode() == null or
			(root1->getAsBinaryNode()->getOp() != glslang::TOperator::EOpIndexDirect and
			 root1->getAsBinaryNode()->getOp() != glslang::TOperator::EOpIndexIndirect) )
		{
			RETURN_ERR( "not supported" );
		}

		glslang::TIntermBinary *	binary	= root1->getAsBinaryNode();
		TIntermNode *				root	= root2;
		
		glslang::TType const&	old_type	= binary->getType();
		glslang::TPublicType	new_type;	new_type.init( binary->getLoc() );

		new_type.basicType		= old_type.getBasicType();
		new_type.sampler		= old_type.getSampler();
		new_type.qualifier		= old_type.getQualifier();
		new_type.vectorSize		= old_type.getVectorSize();
		new_type.matrixCols		= old_type.getMatrixCols();
		new_type.matrixRows		= old_type.getMatrixRows();
		
		if ( old_type.getArraySizes() )
		{
			new_type.arraySizes		= new glslang::TArraySizes();
			*new_type.arraySizes	= *old_type.getArraySizes();
		}

		// not supported
		CHECK_ERR( not old_type.isStruct() );

		// changed name or index
		if ( oldField.name != newField.name or oldField.index != newField.index )
		{
			RETURN_ERR( "not supported" );
		}

		// changed scalar/vector/matrix type of field
		if ( oldField.type != newField.type and oldField.typeName.Empty() and newField.typeName.Empty() )
		{
			// check is 'root' a assign operator or out/inout argument in function call
			bool	is_lvalue	= false;
			bool	is_out_arg	= false;

			GetValueQualifier( root, binary, OUT is_lvalue, OUT is_out_arg );


			// create swizzle operator, value may be Rvalue or Lvalue
			if ( EShaderVariable::VecSize( newField.type ) > 0 )
			{
				CHECK_ERR( CreateSwizzleNode( INOUT root, binary, oldField, newField, old_type, INOUT new_type ) );
			}
			else
			// change other type for compatibility
			if ( is_lvalue )
			{
				CHECK_ERR( CreateLValueNode( root, newField, INOUT new_type ) );
			}
			else
			if ( is_out_arg )
			{
				RETURN_ERR( "output argument is not supported if type is not a scalar or vector!" );
			}
			else
			// create constructor, value must be the Rvalue
			{
				CHECK_ERR( CreateConstructorNode(  INOUT root, binary, oldField, newField, old_type, INOUT new_type ) );
			}
		}

		// changed struct type of field
		if ( not oldField.typeName.Empty() or not newField.typeName.Empty() )
		{
			RETURN_ERR( "struct fields not supported yet!" );
		}

		// set new type to node
		glslang::TType*		ttype	= new glslang::TType( new_type );	// all glslang objects use GC

		ttype->setFieldName( glslang::TString( newField.name.cstr() ) );

		if ( not newField.typeName.Empty() )
			ttype->setTypeName( glslang::TString( newField.typeName.cstr() ) );

		binary->setType( *ttype );
		return true;
	}

/*
=================================================
	RecursiveProcessAggregateNode
=================================================
*/
	static bool RecursiveProcessAggregateNode (TIntermNode* node, Replacer &replacer)
	{
		glslang::TIntermAggregate* aggr = node->getAsAggregate();
	
		_SLOG( ("TIntermAggregate ("_str << uid << ") op:" << uint(aggr->getOp()) << 
				"; " << aggr->getType().getCompleteString().c_str()).cstr(), ELog::Debug );
		
		replacer.nodes << aggr;

		FOR( i, aggr->getSequence() ) {
			CHECK_ERR( RecursiveProcessNode( aggr->getSequence()[i], replacer ) );
		}
		
		CHECK( replacer.nodes.Back() == aggr );
		replacer.nodes.PopBack();
		return true;
	}
	
/*
=================================================
	RecursiveProcessNode
=================================================
*/
	static bool RecursiveProcessNode (TIntermNode* node, Replacer &replacer)
	{
		if ( not node )
			return true;

		if ( node->getAsAggregate() )
		{
			CHECK_ERR( RecursiveProcessAggregateNode( node, replacer ) );
			return true;
		}

		if ( node->getAsUnaryNode() )
		{
			CHECK_ERR( RecursiveProcessUnaryNode( node, replacer ) );
			return true;
		}

		if ( node->getAsBinaryNode() )
		{
			CHECK_ERR( RecursiveProcessBinaryNode( node, replacer ) );
			return true;
		}

		if ( node->getAsOperator() )
		{
			CHECK_ERR( RecursiveProcessOperatorNode( node, replacer ) );
			return true;
		}

		if ( node->getAsBranchNode() )
		{
			CHECK_ERR( RecursiveProcessBranchNode( node, replacer ) );
			return true;
		}

		if ( node->getAsSwitchNode() )
		{
			CHECK_ERR( RecursiveProcessSwitchNode( node, replacer ) );
			return true;
		}

		if ( node->getAsConstantUnion() )
		{
			CHECK_ERR( RecursiveProcessConstUnionNode( node, replacer ) );
			return true;
		}

		if ( node->getAsSelectionNode() )
		{
			CHECK_ERR( RecursiveProcessSelectionNode( node, replacer ) );
			return true;
		}

		if ( node->getAsMethodNode() )
		{
			CHECK_ERR( RecursiveProcessMethodNode( node, replacer ) );
			return true;
		}

		if ( node->getAsSymbolNode() )
		{
			CHECK_ERR( RecursiveProcessSymbolNode( node, replacer ) );
			return true;
		}

		if ( node->getAsTyped() )
		{
			CHECK_ERR( RecursiveProcessTypedNode( node, replacer ) );
			return true;
		}

		if ( node->getAsLoopNode() )
		{
			CHECK_ERR( RecursiveProccessLoop( node, replacer ) );
			return true;
		}

		TODO("");
		return false;
	}
	
/*
=================================================
	RecursiveProcessBranchNode
=================================================
*/
	static bool RecursiveProcessBranchNode (TIntermNode* node, Replacer &replacer)
	{
		glslang::TIntermBranch*		branch = node->getAsBranchNode();
		
		_SLOG( ("TIntermBranch ("_str << uid << ") op: " << uint(branch->getFlowOp())).cstr(), ELog::Debug );
		
		replacer.nodes << branch;
		CHECK_ERR( RecursiveProcessNode( branch->getExpression(), replacer ) );
		
		CHECK( replacer.nodes.Back() == branch );
		replacer.nodes.PopBack();
		return true;
	}
	
/*
=================================================
	RecursiveProcessSwitchNode
=================================================
*/
	static bool RecursiveProcessSwitchNode (TIntermNode* node, Replacer &replacer)
	{
		glslang::TIntermSwitch*		sw = node->getAsSwitchNode();
		
		_SLOG( ("TIntermSwitch ("_str << uid << ") ").cstr(), ELog::Debug );
		
		replacer.nodes << sw;
		CHECK_ERR( RecursiveProcessNode( sw->getCondition(), replacer ) );
		CHECK_ERR( RecursiveProcessNode( sw->getBody(), replacer ) );
		
		CHECK( replacer.nodes.Back() == sw );
		replacer.nodes.PopBack();
		return true;
	}
	
/*
=================================================
	RecursiveProcessConstUnionNode
=================================================
*/
	static bool RecursiveProcessConstUnionNode (TIntermNode* node, Replacer &replacer)
	{
		glslang::TIntermConstantUnion*	cu = node->getAsConstantUnion();
		
		_SLOG( ("TIntermConstantUnion ("_str << uid << ") " << cu->getType().getCompleteString().c_str()).cstr(), ELog::Debug );

		// do nothing
		return true;
	}
	
/*
=================================================
	RecursiveProcessSelectionNode
=================================================
*/
	static bool RecursiveProcessSelectionNode (TIntermNode* node, Replacer &replacer)
	{
		glslang::TIntermSelection*	selection = node->getAsSelectionNode();
		
		_SLOG( ("TIntermSelection ("_str << uid << ") " << selection->getType().getCompleteString().c_str()).cstr(), ELog::Debug );

		replacer.nodes << selection;
		CHECK_ERR( RecursiveProcessNode( selection->getCondition(), replacer ) );
		CHECK_ERR( RecursiveProcessNode( selection->getTrueBlock(), replacer ) );
		CHECK_ERR( RecursiveProcessNode( selection->getFalseBlock(), replacer ) );
		
		CHECK( replacer.nodes.Back() == selection );
		replacer.nodes.PopBack();
		return true;
	}
	
/*
=================================================
	RecursiveProcessMethodNode
=================================================
*/
	static bool RecursiveProcessMethodNode (TIntermNode* node, Replacer &replacer)
	{
		glslang::TIntermMethod*		method = node->getAsMethodNode();
		
		_SLOG( ("TIntermMethod ("_str << uid << ") " << method->getMethodName().c_str() << "; " << method->getType().getCompleteString().c_str()).cstr(), ELog::Debug );
		
		replacer.nodes << method;
		CHECK_ERR( RecursiveProcessNode( method->getObject(), replacer ) );
		
		CHECK( replacer.nodes.Back() == method );
		replacer.nodes.PopBack();
		return true;
	}
	
/*
=================================================
	RecursiveProcessSymbolNode
=================================================
*/
	static bool RecursiveProcessSymbolNode (TIntermNode* node, Replacer &replacer)
	{
		glslang::TIntermSymbol*		symbol = node->getAsSymbolNode();
		
		_SLOG( ("TIntermSymbol ("_str << uid << ") " << symbol->getName().c_str() << "; " << symbol->getId() << 
				"; " << symbol->getType().getCompleteString().c_str()).cstr(), ELog::Debug );

		// do nothing
		return true;
	}
	
/*
=================================================
	RecursiveProcessTypedNode
=================================================
*/
	static bool RecursiveProcessTypedNode (TIntermNode* node, Replacer &replacer)
	{
		glslang::TIntermTyped*		typed = node->getAsTyped();
		
		_SLOG( ("TIntermTyped ("_str << uid << ") " << typed->getType().getCompleteString().c_str()).cstr(), ELog::Debug );

		// do nothing
		return true;
	}

/*
=================================================
	RecursiveProcessOperatorNode
=================================================
*/
	static bool RecursiveProcessOperatorNode (TIntermNode* node, Replacer &replacer)
	{
		glslang::TIntermOperator*	op = node->getAsOperator();
		
		_SLOG( ("TIntermOperator ("_str << uid << ") " << op->getCompleteString().c_str()).cstr(), ELog::Debug );

		// do nothing
		return true;
	}
	
/*
=================================================
	RecursiveProcessUnaryNode
=================================================
*/
	static bool RecursiveProcessUnaryNode (TIntermNode* node, Replacer &replacer)
	{
		glslang::TIntermUnary*		unary = node->getAsUnaryNode();
		
		_SLOG( ("TIntermUnary ("_str << uid << ") op:" << uint(unary->getOp()) << "; " << unary->getCompleteString().c_str()).cstr(), ELog::Debug );

		replacer.nodes << unary;
		CHECK_ERR( RecursiveProcessNode( unary->getOperand(), replacer ) );

		CHECK( replacer.nodes.Back() == unary );
		replacer.nodes.PopBack();
		return true;
	}
	
/*
=================================================
	RecursiveProccessLoop
=================================================
*/
	static bool RecursiveProccessLoop (TIntermNode* node, Replacer &replacer)
	{
		glslang::TIntermLoop *	loop = node->getAsLoopNode();
		
		replacer.nodes << loop;
		CHECK_ERR( RecursiveProcessNode( loop->getBody(), replacer ) );

		if ( loop->getTerminal() ) {
			CHECK_ERR( RecursiveProcessNode( loop->getTerminal(), replacer ) );
		}

		if ( loop->getTest() ) {
			CHECK_ERR( RecursiveProcessNode( loop->getTest(), replacer ) );
		}
		
		CHECK( replacer.nodes.Back() == loop );
		replacer.nodes.PopBack();
		return true;
	}


}	// PipelineCompiler
