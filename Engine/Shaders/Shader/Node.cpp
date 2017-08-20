// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Shaders/Shader/Node.h"

namespace ShaderEditor
{
namespace _ShaderNodesHidden_
{
	
/*
=================================================
	constructor
=================================================
*/
	ISrcNode::ImageUnit::ImageUnit (const Engine::Base::ModulePtr &img) :
		image(img), format( Engine::Platforms::EPixelFormat::Unknown )
	{
		if ( image )
		{
			Engine::Base::Message< Engine::ModuleMsg::GetGpuTextureDescriptor >	msg;
			image->Send( msg );
			format = msg->result->format;
		}
	}

/*
=================================================
	constructor
=================================================
*/
	ISrcNode::ISrcNode (const ISrcNodePtr &parent, VariantCRef valueRef, StringCRef name,
						StringCRef typeName, ENodeType type, uint fieldIndex) :
		_parent( parent ),
		_name( name ),
		_typeName( typeName ),
		_fieldIndex( fieldIndex ),
		_nodeType( type ),
		_graphPassIndex( 0 ),
		_graphReferences( 0 ),
		_nodeIndex( 0 )
	{
		CHECK( _SetConstant( valueRef ) );

		// add to parent
		if ( _parent ) {
			_parent->AddField( this );
		}

		CHECK( _Validate() );
	}
	
/*
=================================================
	_Validate
=================================================
*/
	bool ISrcNode::_Validate () const
	{
		// check parent type
		if ( _IsEditableType( Type() ) or Type() == ENodeType::ShaderOutput )
		{
			//CHECK_ERR( _parent and (_parent->Type() == ENodeType::Root or 
			//		   (_parent->Parent() and _parent->Parent()->Type() == ENodeType::Root)) );
		}

		// find same names

		// find same indices

		// is attached to parent fields

		return true;
	}
	
/*
=================================================
	_IsEditableType
=================================================
*/
	bool ISrcNode::_IsEditableType (ENodeType type)
	{
		return	type == ENodeType::Buffer		 or
				type == ENodeType::PushConstants or
				type == ENodeType::Image		 or
				type == ENodeType::Sampler		 or
				type == ENodeType::Function;
	}

/*
=================================================
	AddField
=================================================
*/
	bool ISrcNode::AddField (const ISrcNodePtr &node)
	{
		CHECK_ERR( node );

		if ( _nodeType != ENodeType::Function )
		{
			//CHECK_ERR( node->Parent() == this or (node->Parent().IsNull() and node->Type() == ENodeType::Function) );

			FOR( i, _fields )
			{
				CHECK_ERR( _fields[i] != node );
				CHECK_ERR( _fields[i]->Name() != node->Name() );
				CHECK_ERR( _fields[i]->Index() != node->Index() or node->Index() == ~0u );
			}
		}

		_fields.PushBack( node );
		return true;
	}

	bool ISrcNode::AddField (const Node &node)
	{
		return AddField( node._self );
	}
	
/*
=================================================
	_CopyVariantToUnion_Func
=================================================
*/
	struct ISrcNode::_CopyVariantToUnion_Func
	{
		VariantCRef				_ref;
		ISrcNode::Value_t &		_union;

		_CopyVariantToUnion_Func (VariantCRef ref, OUT ISrcNode::Value_t &val) :
			_ref( ref ), _union( val )
		{}

		template <typename T, usize Index>
		void Process ()
		{
			auto type = TypeIdOf<T>();

			if ( _ref.GetValueTypeId() == type ) {
				_union.Create( _ref.Get<T>() );
			}
		}
	};

/*
=================================================
	_SetConstant
=================================================
*/
	bool ISrcNode::_SetConstant (VariantCRef valueRef)
	{
		_value.Destroy();
		
		if ( valueRef.GetValueTypeId() == TypeId() )
			return true;

		//ASSERT( Runtime::VirtualTypeList(UninitializedT< Value_t::TypeList_t >()).HasType( valueRef.GetValueTypeId() ) );
		
		_CopyVariantToUnion_Func	func( valueRef, OUT _value );

		Value_t::TypeList_t::RuntimeForEach( func );

		CHECK_ERR( _value.IsDefined() );

		ASSERT( _ValidateConstant() );
		return true;
	}
	
/*
=================================================
	_ValidateConstant
=================================================
*/
	bool ISrcNode::_ValidateConstant () const
	{
		switch ( Type() )
		{
			case ENodeType::Scalar :
				CHECK_ERR( not _value.IsDefined() or _value.Is<bool>() or _value.Is<int>() or
							_value.Is<uint>() or _value.Is<ilong>() or _value.Is<ulong>() or
							_value.Is<float>() or _value.Is<double>() );
				break;

			case ENodeType::Image :
				CHECK_ERR( _value.Is<ImageUnit>() and
							_value.Get<ImageUnit>().image.IsNotNull() and
							_value.Get<ImageUnit>().image->GetSupportedMessages().HasAllTypes<
								CompileTime::TypeListFrom< Engine::ModuleMsg::GetGpuTextureDescriptor > >() );
				break;

			case ENodeType::Buffer :
				CHECK_ERR( _value.Is<BufferUnit>() and
							_value.Get<BufferUnit>().buffer.IsNotNull() and
							_value.Get<BufferUnit>().buffer->GetSupportedMessages().HasAllTypes<
								CompileTime::TypeListFrom< Engine::ModuleMsg::GetGpuBufferDescriptor > >() );
				break;

			case ENodeType::Sampler :
				CHECK_ERR( _value.Is<SamplerUnit>() and
							_value.Get<SamplerUnit>().sampler.IsNotNull() and
							_value.Get<SamplerUnit>().sampler->GetSupportedMessages().HasAllTypes<
								CompileTime::TypeListFrom< Engine::ModuleMsg::GetGpuSamplerDescriptor > >() );
				break;

			case ENodeType::Function :
				CHECK_ERR( _value.Is<FuncInfo>() );
				break;

			default :
				CHECK_ERR( not _value.IsDefined() );	// unknown constant
		}
		return true;
	}

/*
=================================================
	Root
=================================================
*/
	ISrcNodePtr ISrcNode::Root ()
	{
		ISrcNodePtr node = this;

		for (; node->Parent(); node = node->Parent()) {}

		return node;
	}
	
/*
=================================================
	IsConst
=================================================
*/
	bool ISrcNode::IsConst () const
	{
		return	_value.IsDefined() and
				Type() == ENodeType::Scalar;

				//Type() != ENodeType::Function;	// function may contains source code in '_value' but it is not a constant!
	}

//=============================================================================



/*
=================================================
	constructor
=================================================
*/
	Node::Node (Ptr<Node> parent, StringCRef name, StringCRef typeName,	ENodeType nodeType, uint fieldIndex, VariantCRef valueRef) :
		_self( new ISrcNode( parent ? parent->_self : null, valueRef, name, typeName, nodeType, fieldIndex ) )
	{}
	
/*
=================================================
	_Move
=================================================
*/
	void Node::_Move (INOUT Node &other)
	{
		this->_self	= other._self;
		other._self	= null;
	}
		
/*
=================================================
	_Set
=================================================
*/
	void Node::_Set (const ISrcNodePtr &ptr)
	{
		this->_self = ptr;
	}

//=============================================================================
	


/*
=================================================
	constructor
=================================================
*/
	NodeFunctions::NodeFunctions ()
	{
		_RegisterCompilers();
		_RegisterDefaultGLSLFunctions();
	}
	
/*
=================================================
	BindAPI
=================================================
*/
	void NodeFunctions::BindAPI (StringCRef name)
	{
		CHECK( _compilers.IsExist( name ) );

		_apiName = name;
	}
	
/*
=================================================
	GetSource
=================================================
*/
	bool  NodeFunctions::GetSource (StringCRef signature, OUT String &src) const
	{
		Functions_t::const_iterator	iter;

		usize	in_pos	= ~0u;
		usize	out_pos	= ~0u;

		signature.Find( "_in", OUT in_pos, 0 );
		signature.Find( "_out", OUT out_pos, 0 );

		StringCRef	name = signature.SubString( 0, Min( in_pos, out_pos ) );

		if ( _funcs.Find( name, OUT iter ) )
		{
			CHECK_ERR( iter->second.sourceGen( signature, OUT src ) );
			return true;
		}

		if ( not _apiName.Empty() )
		{
			String	sig;	sig << _apiName << '.' << name;

			if ( _funcs.Find( sig, OUT iter ) )
			{
				CHECK_ERR( iter->second.sourceGen( signature, OUT src ) );
				return true;
			}
		}

		RETURN_ERR( "can't find function with signature '" << signature << "'" );
	}

/*
=================================================
	UnregisterAll
=================================================
*/
	void NodeFunctions::UnregisterAll ()
	{
		_funcs.Clear();
	}
	
/*
=================================================
	Compile
=================================================
*/
	bool NodeFunctions::Compile (const Array<ISrcNodePtr> &nodes, EShader::type shaderType, OUT ModulePtr &program) const
	{
		CompilerMap_t::const_iterator	iter;

		CHECK_ERR( _compilers.Find( _apiName, OUT iter ) );

		CHECK_ERR( iter->second.Call( nodes, shaderType, OUT program ) );
		return true;
	}
	
/*
=================================================
	Register
=================================================
*/
	bool  NodeFunctions::Register (StringCRef name, const Func_t &func, const SourceGen_t &srcGen)
	{
		CHECK_ERR( not _funcs.IsExist( name ) );

		_funcs.Add( name, { func, srcGen } );
		return true;
	}
	
	bool  NodeFunctions::Register (StringCRef name, Func_t::Function_t *func, SourceGen_t::Function_t *srcGen)
	{
		return Register( name, DelegateBuilder( func ), DelegateBuilder( srcGen ) );
	}

/*
=================================================
	RegisterCompiler
=================================================
*/
	bool NodeFunctions::RegisterCompiler (StringCRef name, const CompileFunc_t &compiler)
	{
		CHECK_ERR( not _compilers.IsExist( name ) );

		_compilers.Add( name, compiler );
		return true;
	}
	
/*
=================================================
	UnregisterCompilers
=================================================
*/
	void NodeFunctions::UnregisterCompilers ()
	{
		_compilers.Clear();
	}

/*
=================================================
	Instance
=================================================
*/
	Ptr<NodeFunctions>  NodeFunctions::Instance ()
	{
		return SingletonMultiThread::Instance<NodeFunctions>();
	}


}	// _ShaderNodesHidden_
}	// ShaderEditor
