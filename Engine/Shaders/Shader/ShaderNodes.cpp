// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Shaders/Shader/ShaderNodes.h"

namespace ShaderEditor
{
namespace _ShaderNodesHidden_
{

	struct NodeGetter
	{
		static ISrcNodePtr Get (const Node &node)	{ return node._self; }
	};

	struct NodeGraph
	{
		static bool SetPassIndex (const ISrcNodePtr &ptr, uint passIndex)
		{
			if ( ptr->_graphPassIndex != passIndex )
			{
				ptr->_nodeIndex			= 0;
				ptr->_graphReferences	= 1;
				ptr->_graphPassIndex	= passIndex;
				return false;
			}

			ptr->_graphReferences++;
			return true;
		}

		static bool ReleaseReference (const ISrcNodePtr &ptr, uint passIndex)
		{
			ASSERT( passIndex == ptr->_graphPassIndex );

			if ( ptr->_graphReferences > 0 )
			{
				--ptr->_graphReferences;
				return ptr->_graphReferences == 0;
			}
			return false;
		}

		static void SetNodeIndex (const ISrcNodePtr &ptr, uint index)
		{
			ptr->_nodeIndex = index;
		}

		static uint GetPassIndex (const ISrcNodePtr &ptr)	{ return ptr->_graphPassIndex; }
		static uint	GetRefCount (const ISrcNodePtr &ptr)	{ return ptr->_graphReferences; }
		static uint GetNodeIndex (const ISrcNodePtr &ptr)	{ return ptr->_nodeIndex; }

		static ISrcNode::Value_t const&	GetConst (const ISrcNodePtr &ptr)	{ return ptr->_value; }

		static Array< ISrcNodePtr >&	GetFields (const ISrcNodePtr &ptr)	{ return ptr->_fields; }
	};

}	// _ShaderNodesHidden_


namespace ShaderNodes
{
	
	//
	// Shader Compiler
	//
		
	struct Shader::ShaderCompiler
	{
	// types
	private:
		using Self			= ShaderCompiler;
		using NodePtr		= _ShaderNodesHidden_::ISrcNodePtr;
		using NodeArray_t	= GX_STL::GXTypes::Array< NodePtr >;
		using NodeGraph		= _ShaderNodesHidden_::NodeGraph;
		using String		= GX_STL::GXTypes::String;
		using usize			= GX_STL::GXTypes::usize;
		using isize			= GX_STL::GXTypes::isize;

		template <typename T>	using Set	= GX_STL::GXTypes::Set<T>;
		template <typename T>	using Queue = GX_STL::GXTypes::Queue<T>;


	// variables
	private:
		NodeArray_t		_nodes;


	// methods
	public:
		ShaderCompiler ();

		Self&	operator << (const Node &node);

		bool Link (EShader::type shaderType, const NodePtr &root);

	private:
		bool _GetInputOutputNodes (const NodePtr &root, OUT NodeArray_t &allInputs, OUT NodeArray_t &allOutputs) const;
		
		bool _GetOutputNodes (const NodePtr &root, OUT NodeArray_t &allOutputs) const;
		bool _ProcessGraphPass1 (uint passIndex, const NodeArray_t &output, OUT NodeArray_t &allNodes) const;
		bool _ProcessGraphPass2 (uint passIndex, INOUT NodeArray_t &allNodes) const;
		bool _SetIndices (INOUT NodeArray_t &allNodes) const;

		// optimizations
		bool _Optimize (INOUT NodeArray_t &allNodes) const;
		bool _RemoveDuplicateConstants (INOUT NodeArray_t &allNodes) const;

		bool _BuildDebugSource (const NodeArray_t &allNodes) const;
	};
	

	
/*
=================================================
	constructor
=================================================
*/
	Shader::ShaderCompiler::ShaderCompiler ()
	{
	}

/*
=================================================
	operator <<
=================================================
*/
	Shader::ShaderCompiler&  Shader::ShaderCompiler::operator << (const Node &node)
	{
		_nodes << _ShaderNodesHidden_::NodeGetter::Get( node );
		return *this;
	}
		
/*
=================================================
	Link
=================================================
*/
	bool Shader::ShaderCompiler::Link (EShader::type shaderType, const NodePtr &root)
	{
		CHECK_ERR( root and root->Type() == ENodeType::Root );

		const uint	pass_index	= NodeGraph::GetPassIndex( root ) + 1;

		NodeArray_t		active_outputs;
		CHECK_ERR( _GetOutputNodes( root, OUT active_outputs ) );

		NodeArray_t		all_nodes;
		CHECK_ERR( _ProcessGraphPass1( pass_index, active_outputs, OUT all_nodes ) );
		CHECK_ERR( _ProcessGraphPass2( pass_index, INOUT all_nodes ) );

		// TODO: optimize (vectorise, ...)
		//CHECK_ERR( _RemoveDuplicateConstants( all_nodes ) );
		CHECK_ERR( _Optimize( all_nodes ) );
		
		CHECK_ERR( _SetIndices( all_nodes ) );

		CHECK_ERR( _BuildDebugSource( all_nodes ) );

		Engine::Base::ModulePtr	prog;
		CHECK_ERR( _ShaderNodesHidden_::NodeFunctions::Instance()->Compile( all_nodes, shaderType, OUT prog ) );
		return true;
	}
	
/*
=================================================
	_GetOutputNodes
=================================================
*/
	bool Shader::ShaderCompiler::_GetOutputNodes (const NodePtr &root, OUT NodeArray_t &allOutputs) const
	{
		FOR( i, root->Fields() )
		{
			auto const&		node = root->Fields()[i];

			switch ( node->Type() )
			{
				case ENodeType::ShaderOutput :
				{
					allOutputs << node;
					break;
				}
			}
		}
		return true;
	}
	
/*
=================================================
	_ProcessGraphPass1
=================================================
*/
	bool Shader::ShaderCompiler::_ProcessGraphPass1 (uint passIndex, const NodeArray_t &output, OUT NodeArray_t &allNodes) const
	{
		Queue< NodePtr >	pending;	pending.Reserve( 1<<10 );

		// find all nodes
		FOR( i, output )
		{
			pending.PushBack( output[i] );

			for (; not pending.Empty();)
			{
				NodePtr		curr = pending.Front();		pending.PopFront();

				// check for recursion
				if ( NodeGraph::GetPassIndex( curr ) == passIndex )
					continue;

				NodeGraph::SetPassIndex( curr, passIndex );

				allNodes << curr;

				FOR_rev( j, curr->Fields() )
				{
					pending << curr->Fields()[j];
				}
				
				if ( curr->Parent() and curr->Parent()->Type() != ENodeType::Root )
				{
					pending << curr->Parent();
				}
			}
		}

		return true;
	}
	
/*
=================================================
	_ProcessGraphPass2
=================================================
*/
	bool Shader::ShaderCompiler::_ProcessGraphPass2 (uint passIndex, INOUT NodeArray_t &allNodes) const
	{
		// sort
		for (bool completed = false; not completed;)
		{
			completed = true;

			FOR( i, allNodes )
			{
				NodePtr		node = allNodes[i];
				bool		keep_fields	= false;

				FOR( j, node->Fields() )
				{
					keep_fields |= NodeGraph::GetPassIndex( node->Fields()[j] ) == passIndex;
				}

				if ( keep_fields )
				{
					FOR( j, node->Fields() )
					{
						NodePtr		field	= node->Fields()[j];

						for (usize c = 0; c < i and node != allNodes[c]; ++c)
						{
							if ( field == allNodes[c] )
							{
								allNodes.Erase( c );
								--c;

								ASSERT( allNodes[i-1] == node );
								allNodes.Insert( field, i );

								completed = false;
								continue;
							}
						}

						
						/*for (usize c = 0; c < i; ++c)
						{
							if ( (field->Type() == ENodeType::Vector or field->Type() == ENodeType::Struct) and
									field->Parent() == allNodes[c] )
							{
								allNodes.Erase( c );
								//--c;

								ASSERT( allNodes[i-1] == node );
								allNodes.Insert( field->Parent(), i );

								completed = false;
								continue;
							}
						}*/

						if ( not completed )
							break;
					}
				}
				
				if ( node->Parent() and node->Parent()->Type() == ENodeType::Function )
				{
					NodePtr		func		= node->Parent();
					usize		last_idx	= i-1;

					for (isize c = i-1; c >= 0; --c)
					{
						NodePtr		other = allNodes[c];

						// it is output node too
						if ( other->Parent() and other->Parent() == func )
						{
							last_idx = c;
							continue;
						}
						else
						if ( other == func )
						{
							if ( last_idx != c )
							{
								// move function node below output nodes
								allNodes.Erase( c );
								allNodes.Insert( func, last_idx );

								completed = false;
							}
							break;
						}
					}
				}

				if ( not completed )
					break;
			}
		}

		return true;
	}
	
/*
=================================================
	_Optimize
=================================================
*/
	bool Shader::ShaderCompiler::_Optimize (INOUT NodeArray_t &allNodes) const
	{
		FOR( i, allNodes )
		{
			NodePtr		node = allNodes[i];

			// vectorize
			if ( node->Type() == ENodeType::Scalar and
				 node->Parent() and node->Parent()->Type() == ENodeType::Vector )
			{
				allNodes.Erase( i );
				--i;
				continue;
			}

			/*if ( node->Type() == ENodeType::Scalar and
				 node->Parent() and node->Parent()->Type() != ENodeType::Function and
				 node->Parent()->Parent() and node->Parent()->Parent()->Type() == ENodeType::Function )
			{
				allNodes.Erase( i );
				--i;
				continue;
			}*/

			// TODO: is initialized
		}
		return true;
	}
	
/*
=================================================
	_RemoveDuplicateConstants
=================================================
*/
	bool Shader::ShaderCompiler::_RemoveDuplicateConstants (INOUT NodeArray_t &allNodes) const
	{
		using NodeGraph = _ShaderNodesHidden_::NodeGraph;


		struct ConstNode
		{
		private:
			NodePtr		_ptr;

		public:
			ConstNode (const NodePtr &ptr) : _ptr(ptr)
			{}

			NodePtr const& Get () const {
				return _ptr;
			}

			bool operator == (const ConstNode &right) const	{
				return NodeGraph::GetConst( _ptr ) == NodeGraph::GetConst( right._ptr );
			}
			
			bool operator >  (const ConstNode &right) const	{
				return NodeGraph::GetConst( _ptr ) > NodeGraph::GetConst( right._ptr );
			}
			
			bool operator <  (const ConstNode &right) const	{
				return NodeGraph::GetConst( _ptr ) < NodeGraph::GetConst( right._ptr );
			}
		};

		Set< ConstNode >					constants;
		Set< ConstNode >::const_iterator	iter;

		FOR_rev( i, allNodes )
		{
			NodePtr		node = allNodes[i];

			if ( node->IsConst() )
			{
				constants.AddOrSkip( node );
			}
			else
			{
				// replace constants
				FOR( j, node->Fields() )
				{
					NodePtr &	field = NodeGraph::GetFields( node )[j];

					if ( field->IsConst() and constants.Find( field, OUT iter ) )
					{
						field = iter->Get();
					}
				}
			}
		}

		return true;
	}

/*
=================================================
	_SetIndices
=================================================
*/
	bool Shader::ShaderCompiler::_SetIndices (INOUT NodeArray_t &allNodes) const
	{
		uint	index = 1;

		FOR_rev( i, allNodes )
		{
			NodePtr		node = allNodes[i];

			if ( node->Type() == ENodeType::Function )
				continue;

			NodeGraph::SetNodeIndex( node, index );
			++index;
		}
		return true;
	}

/*
=================================================
	_BuildDebugSource
=================================================
*/
	bool Shader::ShaderCompiler::_BuildDebugSource (const NodeArray_t &allNodes) const
	{
		String	src;	src.Reserve( 1024 );

		FOR_rev( i, allNodes )
		{
			NodePtr		node	= allNodes[i];
			const uint	index	= NodeGraph::GetNodeIndex( node );

			// skip functions
			if ( node->Type() == ENodeType::Function )
				continue;

			src << node->TypeName() << "  node_" << index << "(\"" << node->Name() << "\") ";

			switch ( node->Type() )
			{
				case ENodeType::Scalar :		src << "- Scalar";			break;
				case ENodeType::Vector :		src << "- Vector";			break;
				case ENodeType::Array :			src << "- Array";			break;
				case ENodeType::Struct :		src << "- Struct";			break;
				case ENodeType::Buffer :		src << "- Buffer";			break;

				case ENodeType::PushConstants :	src << "- PushConstants";	break;
				case ENodeType::LocalShared :	src << "- LocalShared";		break;
				case ENodeType::Image :			src << "- Image";			break;
				case ENodeType::Sampler :		src << "- Sampler";			break;
				case ENodeType::ShaderOutput :	src << "- ShaderOutput";	break;
				case ENodeType::ShaderSync :	src << "- ShaderSync";		break;
				case ENodeType::Root :			src << "- Root";			break;
			}

			// dependencies
			bool	keep_fields	= false;

			FOR( j, node->Fields() )
			{
				const uint	idx = NodeGraph::GetNodeIndex( node->Fields()[j] );
				keep_fields |= (idx != 0);
			}

			if ( keep_fields )
			{
				src << " { ";
				FOR( j, node->Fields() )
				{
					NodePtr		field = node->Fields()[j];

					const uint	idx = NodeGraph::GetNodeIndex( field );
					ASSERT( idx > 0 and idx < index );

					src << (j ? ", " : "") << "node_" << idx;
				}
				src << " }";
			}

			if ( node->Parent() and node->Parent()->Type() == ENodeType::Function )
			{
				NodePtr	func = node->Parent();

				src << " = " << func->TypeName() << " { ";

				FOR( j, func->Fields() )
				{
					NodePtr		field = func->Fields()[j];

					const uint	idx = NodeGraph::GetNodeIndex( field );

					src << (j ? ", " : "");
					
					if ( idx > 0 )
					{
						ASSERT( idx <= index );
						src << "node_" << idx;
					}
					else
					if ( field->Parent() and not field->Name().Empty() )
					{
						const uint	idx2 = NodeGraph::GetNodeIndex( field->Parent() );
						ASSERT( idx2 > 0 and idx2 <= index );

						src << "node_" << idx2 << '.' << field->Name();
					}
					else
					{
						WARNING( "invalid node index" );
					}
				}
				src << " }";
			}
			src << "\n";
		}

		LOG( src.cstr(), GX_STL::ELog::Info | GX_STL::ELog::OpenSpoilerFlag );
		return true;
	}

/*
=================================================
	_GetInputOutputNodes
=================================================
*/
	bool Shader::ShaderCompiler::_GetInputOutputNodes (const NodePtr &root, OUT NodeArray_t &allInputs, OUT NodeArray_t &allOutputs) const
	{
		FOR( i, root->Fields() )
		{
			auto const&		node = root->Fields()[i];

			switch ( node->Type() )
			{
				case ENodeType::Buffer :
				{
					if (true)
						allOutputs << node;
					else
						allInputs << node;
					break;
				}
				case ENodeType::Image :
				{
					if (true)
						allOutputs << node;
					else
						allInputs << node;
					break;
				}
				case ENodeType::Sampler :
				{
					allInputs << node;
					break;
				}
				case ENodeType::PushConstants :
				{
					allInputs << node;
					break;
				}
				case ENodeType::ShaderOutput :
				{
					allOutputs << node;
					break;
				}
				case ENodeType::ShaderSync :
				{
					allOutputs << node;
					break;
				}
			}
		}
		return true;
	}

//=============================================================================



/*
=================================================
	constructor
=================================================
*/
	Shader::Shader (EShader::type shaderType) :
		Node( null, "root", GX_STL::GXTypes::String( EShader::ToString( shaderType ) ) << " shader", ENodeType::Root ),
		_shaderType( shaderType )
	{}

/*
=================================================
	_Compile
=================================================
*/
	bool Shader::_Compile ()
	{
		ShaderCompiler	compiler;
		_currCompiler = &compiler;

		Main();

		_currCompiler = null;

		CHECK_ERR( compiler.Link( _shaderType, _SelfNode() ) );
		return true;
	}
	
/*
=================================================
	AddOutputNode
=================================================
*/
	bool Shader::AddOutputNode (const Node &node)
	{
		CHECK_ERR( _currCompiler );

		(*_currCompiler) << node;
		return true;
	}


}	// ShaderNodes
}	// ShaderEditor
