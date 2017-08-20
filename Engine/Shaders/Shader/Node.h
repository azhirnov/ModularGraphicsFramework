// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Common/Common.h"

namespace ShaderEditor
{
namespace ShaderNodes
{
	using uint		= GX_STL::GXTypes::uint;
	using ilong		= GX_STL::GXTypes::ilong;
	using ulong		= GX_STL::GXTypes::ulong;

	template <typename T, uint I>	struct NVec;

	// for swizzling
	template <typename T, typename ...Args>
	inline T  NVecCtor (const Args& ...args);

}	// ShaderNodes


namespace _ShaderNodesHidden_
{
	
	struct Node;

	class ISrcNode;
	SHARED_POINTER( ISrcNode );



	//
	// Node Source interface
	//

	class ISrcNode final : protected RefCountedObject
	{
		friend struct NodeGraph;

	// types
	public:
		using Name_t	= StaticString< 128 >;

		enum class ENodeType
		{
			Unknown,
			Scalar,
			Vector,		// TODO: same as Struct
			Array,
			Struct,
			Buffer,
			PushConstants,
			LocalShared,
			Image,
			Sampler,
			Function,
			ShaderOutput,
			ShaderSync,
			Root,
		};

		struct ImageUnit
		{
			Engine::Base::ModulePtr					image;
			Engine::Platforms::EPixelFormat::type	format;

			explicit ImageUnit (const Engine::Base::ModulePtr &img);

			bool operator == (const ImageUnit &right) const		{ return image == right.image; }
			bool operator >  (const ImageUnit &right) const		{ return image == right.image; }
			bool operator <  (const ImageUnit &right) const		{ return image == right.image; }
		};

		struct BufferUnit
		{
			Engine::Base::ModulePtr					buffer;
			
			explicit BufferUnit (const Engine::Base::ModulePtr &buf) : buffer(buf) {}

			bool operator == (const BufferUnit &right) const	{ return buffer == right.buffer; }
			bool operator >  (const BufferUnit &right) const	{ return buffer >  right.buffer; }
			bool operator <  (const BufferUnit &right) const	{ return buffer <  right.buffer; }
		};

		struct SamplerUnit
		{
			Engine::Base::ModulePtr					sampler;
			
			explicit SamplerUnit (const Engine::Base::ModulePtr &samp) : sampler(samp) {}

			bool operator == (const SamplerUnit &right) const	{ return sampler == right.sampler; }
			bool operator >  (const SamplerUnit &right) const	{ return sampler >  right.sampler; }
			bool operator <  (const SamplerUnit &right) const	{ return sampler <  right.sampler; }
		};

		struct FuncInfo
		{
			ushort		inArgsCount		= 0;
			ushort		outArgsCount	= 0;

			FuncInfo (usize in, usize out) :
				inArgsCount(ushort(in)), outArgsCount(ushort(out))
			{}

			bool operator == (const FuncInfo &right) const	{ return inArgsCount == right.inArgsCount and outArgsCount == right.outArgsCount; }
			bool operator >  (const FuncInfo &right) const	{ return inArgsCount != right.inArgsCount ? inArgsCount > right.inArgsCount : outArgsCount > right.outArgsCount; }
			bool operator <  (const FuncInfo &right) const	{ return inArgsCount != right.inArgsCount ? inArgsCount < right.inArgsCount : outArgsCount < right.outArgsCount; }
		};


	private:
		struct _CopyVariantToUnion_Func;

		using ModulePtr	= Engine::Base::ModulePtr;
		using Fields_t	= Array< ISrcNodePtr >;
		using Value_t	= Union< bool, int, uint, ilong, ulong, float, double,
								 ImageUnit, BufferUnit, SamplerUnit, FuncInfo >;


	// variables
	private:
		ISrcNodePtr		_parent;		// parent node
		Fields_t		_fields;		// child nodes
		Value_t			_value;
		Name_t			_name;			// name of field
		Name_t			_typeName;		// name of type
		uint			_fieldIndex;	// field or array element index for Struct, Vector, Array
		ENodeType		_nodeType;

		uint			_graphPassIndex;
		uint			_graphReferences;
		uint			_nodeIndex;
		

	// methods
	public:
		ISrcNode (const ISrcNodePtr &parent, VariantCRef valueRef, StringCRef name,
				  StringCRef typeName, ENodeType type, uint fieldIndex);

		StringCRef			Name ()		const	{ return _name; }
		StringCRef			TypeName ()	const	{ return _typeName; }
		uint				Index ()	const	{ return _fieldIndex; }
		ENodeType			Type ()		const	{ return _nodeType; }
		ISrcNodePtr const&	Parent ()	const	{ return _parent; }
		Fields_t const&		Fields ()	const	{ return _fields; }

		ISrcNodePtr			Root ();
		bool				IsConst ()	const;

		bool AddField (const ISrcNodePtr &node);
		bool AddField (const Node &node);

		template <typename T>
		bool SetConstant (const T &newValue);

	private:
		bool _Validate () const;
		bool _ValidateConstant () const;

		bool _SetConstant (VariantCRef newValue);

		static bool _IsEditableType (ENodeType type);
	};



	//
	// Node
	//

	struct Node : CompileTime::FastCopyable
	{
		friend class ISrcNode;
		friend struct NodeGetter;

	// types
	protected:
		using StringCRef	= GX_STL::GXTypes::StringCRef;
		using ENodeType		= ISrcNode::ENodeType;
		using Name_t		= ISrcNode::Name_t;


	// variables
	private:
		ISrcNodePtr		_self;


	// methods
	public:
		Node (GX_DEFCTOR) {}

		Node (Ptr<Node> parent, StringCRef name, StringCRef typeName,
			  ENodeType nodeType, uint fieldIndex = ~0u, VariantCRef valueRef = VariantCRef());
		
		Node (Node &&) = delete;
		Node (const Node &) = delete;

		Node& operator = (const Node &) = delete;
		Node& operator = (Node &&) = delete;

	protected:
		template <typename ...Args>
		void _AddFields (const Node &first, const Args& ...args);
		void _AddFields (const Node &first);

		void _Move (INOUT Node &other);
		void _Set (const ISrcNodePtr &ptr);

		ISrcNodePtr const&	_SelfNode ()	const	{ return _self; }
	};



	//
	// Shader Output Node
	//

	struct ShaderOutputNode : Node
	{
	// methods
	public:
		ShaderOutputNode (GX_DEFCTOR) {}

		ShaderOutputNode (ShaderOutputNode &&node)
		{
			_Move( node );
		}

		explicit ShaderOutputNode (Ptr<Node> parent) :
			Node( parent, "", _Typename(), ENodeType::ShaderOutput )
		{}

		template <typename ...Args>
		explicit ShaderOutputNode (Ptr<Node> parent, const Args& ...args) :
			ShaderOutputNode( parent )
		{
			_AddFields( args... );
		}
		
		static Name_t  _Typename ()		{ return "Sync"; }
	};



	//
	// Function Node
	//

	struct FunctionNode : Node
	{
	// methods
	public:
		explicit FunctionNode (StringCRef funcName, usize inArgsCount, usize outArgsCount) :		// TODO: signature may be too long for static string
			Node( null, "", funcName, ENodeType::Function, ~0u,
				  VariantCRef::FromConst( ISrcNode::FuncInfo(inArgsCount, outArgsCount) ) )
		{
		}

		template <typename ...Args>
		explicit FunctionNode (StringCRef funcName, uint outArgsCount, const Args& ...args) :
			FunctionNode( funcName, CompileTime::TypeListFrom< Args... >::Count, outArgsCount )
		{
			_AddFields( args... );
		}

		template <typename T>
		void AddField (const T &arg)
		{
			_AddFields( arg );
		}
	};



	//
	// Barrier
	//
	/*
	struct Barrier : Node
	{
	// methods
	public:
		Barrier (GX_DEFCTOR) {}

		template <typename ...Args>
		explicit Barrier (Ptr<Node> parent, const Args& ...args) :
			Node( parent, "", "barrier", ENodeType::ShaderSync )
		{
			_AddFields( args... );
		}
	};*/



	//
	// Image / Buffer Access Type
	//

	template <bool ReadAccess, bool WriteAccess>
	struct AccessType
	{
		static constexpr bool	readAccess		= ReadAccess;
		static constexpr bool	writeAccess		= WriteAccess;
	};



	//
	// Property
	//
	
	template <typename T, typename Class, typename ...Args>
	struct Property final
	{
	// types
	private:
		using Self		= Property< T, Class, Args... >;


	// variables
	private:
		const ubyte		_offset;


	// methods
	public:
		explicit Property (const Class *classPtr) :
			_offset( ubyte( ReferenceCast<usize>(this) - ReferenceCast<usize>(classPtr) ) )
		{
			ASSERT( (void *)this > (void *)classPtr );
		}

		forceinline operator T () const
		{
			Class const* cl = _GetClass();
			return ShaderNodes::NVecCtor<T>( Args::Get( cl )... );
		}

	private:
		Property () = delete;
		Property (const Self &) = delete;
		Property (Self &&) = delete;

		Self& operator = (const Self &) = delete;
		Self& operator = (Self &&) = delete;

		forceinline Class const* _GetClass () const
		{
			return (Class const *)( ((byte const *)this) - _offset );
		}
	};



	//
	// ClassToType and MemberToType (helpers for Property)
	//

	template <typename Class, typename Field, Field (Class::*member)>
	struct MemberToType
	{
		static Field const&  Get (const Class *classPtr)
		{
			return classPtr->*member;
		}
	};

	template <typename Class>
	struct ClassToType
	{
		static Class const&  Get (const Class *classPtr)
		{
			return *classPtr;
		}
	};



	//
	// Functions
	//

	class NodeFunctions final
	{
	// types
	private:
		using Func_t		= Delegate< bool (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out) >;
		using SourceGen_t	= Delegate< bool (StringCRef sig, OUT String &src) >;

		struct FuncInfo
		{
			Func_t			funcNodeCtor;
			SourceGen_t		sourceGen;
		};

		using Functions_t	= HashMap< String, FuncInfo >;

		using ModulePtr		= Engine::Base::ModulePtr;
		using EShader		= Engine::Platforms::EShader;
		using CompileFunc_t	= Delegate< bool (const Array<ISrcNodePtr> &, EShader::type, ModulePtr &) >;
		using CompilerMap_t	= HashMap< String, CompileFunc_t >;

		struct _TypeToStr_Func;
		
		template <typename T>
		struct _NodeTypeToString;

		template <typename ...Args>
		struct _ArgTypeToInOut;

		template <typename FuncType, typename InArgTypeList, typename OutArgTypeList>
		class _FuncWrapper;
		
		template <bool NoInput, typename FuncType, typename InArgTypeList, typename OutArgTypeList>
		struct _FuncWrapperImpl;


	// variables
	private:
		Functions_t		_funcs;
		CompilerMap_t	_compilers;
		String			_apiName;	// DX, GL, VK, CL, CPP ...


	// methods
	public:
		NodeFunctions ();

		void BindAPI (StringCRef name);

		template <typename Result, typename ...Args>
		Result Build (StringCRef name, const Args& ...args) const;

		bool GetSource (StringCRef signature, OUT String &src) const;

		bool Compile (const Array<ISrcNodePtr> &nodes, EShader::type shaderType, OUT ModulePtr &program) const;
		
		bool Register (StringCRef name, const Func_t &func, const SourceGen_t &srcGen);
		bool Register (StringCRef name, Func_t::Function_t *func, SourceGen_t::Function_t *srcGen);

		void UnregisterAll ();

		bool RegisterCompiler (StringCRef name, const CompileFunc_t &compiler);
		void UnregisterCompilers ();

		static Ptr<NodeFunctions>	Instance ();


	private:
		template <typename InArgTypeList, typename OutArgTypeList>
		static String _BuildSignature (StringCRef name);

		template <typename ConstRefArray, typename RefArray, typename T, typename ...Args>
		static void _BuildRefArray (INOUT ConstRefArray &constRefArr, INOUT RefArray &refArr, T *first, Args* ...args);
		
		template <typename ConstRefArray, typename RefArray, typename T>
		static void _BuildRefArray (INOUT ConstRefArray &constRefArr, INOUT RefArray &refArr, T *first);

		void _RegisterDefaultGLSLFunctions ();

		void _RegisterCompilers ();
	};

	
/*
=================================================
	_AddFields
=================================================
*/
	inline void Node::_AddFields (const Node &first)
	{
		_self->AddField( first._self );
	}

	template <typename ...Args>
	inline void Node::_AddFields (const Node &first, const Args& ...args)
	{
		_self->AddField( first._self );
		_AddFields( args... );
	}
	
/*
=================================================
	SetConstant
=================================================
*/
	template <typename T>
	inline bool ISrcNode::SetConstant (const T &newValue)
	{
		CHECK_ERR( _IsEditableType( Type() ) );

		return _SetConstant( VariantCRef::FromConst( newValue ) );
	}

/*
=================================================
	Build
=================================================
*/
	template <typename Result, typename ...Args>
	inline Result  NodeFunctions::Build (StringCRef name, const Args& ...args) const
	{
		using InArray_t		= FixedSizeArray< VariantCRef, CompileTime::Max< usize, CountOf< Args... >(), 1 > >;
		using OutArray_t	= FixedSizeArray< VariantRef, 1 >;
		using ArgList		= _ArgTypeToInOut< Result&, const Args&... >;

		STATIC_ASSERT(( ArgList::OutArgs::Count == 1 and
						CompileTime::IsSameTypes< ArgList::OutArgs::Get<0>, Result& > ));

		const String				sig = String(_apiName) << '.' << name;
		Functions_t::const_iterator	iter;
		InArray_t					in_args_arr;
		OutArray_t					out_args_arr;
		Result						result;

		if ( not _funcs.Find( sig, OUT iter ) )
		{
			RETURN_ERR( "can't find function with signature \"" << sig << "\"" );
		}

		_BuildRefArray( OUT in_args_arr, OUT out_args_arr, &result, &args... );

		CHECK_ERR( iter->second.funcNodeCtor.Call( in_args_arr, out_args_arr ) );
		return result;
	}
	
/*
=================================================
	_FuncWrapperImpl
=================================================
*/
	template <bool NoInput, typename FuncType, typename InArgTypeList, typename OutArgTypeList>
	struct NodeFunctions::_FuncWrapperImpl
	{
		static bool Call (const FuncType &func, ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
		{
			STATIC_ASSERT( InArgTypeList::Empty and not OutArgTypeList::Empty );

			return InvokeWithVariant( func, VariantRefIndexedContainerFrom< OutArgTypeList >( out ) );
		}
	};

	template <typename FuncType, typename InArgTypeList, typename OutArgTypeList>
	struct NodeFunctions::_FuncWrapperImpl < false, FuncType, InArgTypeList, OutArgTypeList >
	{
		static bool Call (const FuncType &func, ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
		{
			STATIC_ASSERT( not InArgTypeList::Empty and not OutArgTypeList::Empty );

			return InvokeWithVariant( func,
						VariantRefIndexedContainerFrom< InArgTypeList >( in ),
						VariantRefIndexedContainerFrom< OutArgTypeList >( out ) );
		}
	};
	
/*
=================================================
	_FuncWrapper
=================================================
*/
	template <typename FuncType, typename InArgTypeList, typename OutArgTypeList>
	class NodeFunctions::_FuncWrapper : public RefCountedObject
	{
	// variables
	private:
		FuncType	_func;

	// methods
	public:
		explicit _FuncWrapper (const FuncType &func) : _func( func )
		{}
		
		bool Call (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
		{
			return _FuncWrapperImpl< InArgTypeList::Empty, FuncType, InArgTypeList, OutArgTypeList >::Call( _func, in, out );
		}
	};
	
/*
=================================================
	_ArgTypeToInOut
=================================================
*/
	template <typename T, typename ...Types>
	struct NodeFunctions::_ArgTypeToInOut< T, Types... >
	{};

	template <typename T, typename ...Types>
	struct NodeFunctions::_ArgTypeToInOut< const T&, Types... >
	{
		using _next		= _ArgTypeToInOut< Types... >;

		using InArgs	= CompileTime::TypeList< const T&, typename _next::InArgs >;
		using OutArgs	= typename _next::OutArgs;
	};

	template <typename T, typename ...Types>
	struct NodeFunctions::_ArgTypeToInOut< T&, Types... >
	{
		using _next		= _ArgTypeToInOut< Types... >;

		using InArgs	= typename _next::InArgs;
		using OutArgs	= CompileTime::TypeList< T&, typename _next::OutArgs >;
	};

	template <typename T>
	struct NodeFunctions::_ArgTypeToInOut< T >
	{};

	template <typename T>
	struct NodeFunctions::_ArgTypeToInOut< const T& >
	{
		using InArgs	= CompileTime::TypeList< const T& >;
		using OutArgs	= CompileTime::TypeList< CompileTime::TypeListEnd >;
	};

	template <typename T>
	struct NodeFunctions::_ArgTypeToInOut< T& >
	{
		using InArgs	= CompileTime::TypeList< CompileTime::TypeListEnd >;
		using OutArgs	= CompileTime::TypeList< T& >;
	};

/*
=================================================
	_NodeTypeToString
=================================================
*/
	template <typename T>
	struct NodeFunctions::_NodeTypeToString {
		forceinline static String  Get ()	{ return String() << "_in" << T::_Typename(); }		// input
	};

	template <typename T>
	struct NodeFunctions::_NodeTypeToString< const T& > {
		forceinline static String  Get ()	{ return String() << "_in" << T::_Typename(); }		// input
	};
	
	template <typename T>
	struct NodeFunctions::_NodeTypeToString< T &> {
		forceinline static String  Get ()	{ return String() << "_out" << T::_Typename(); }	// output
	};

/*
=================================================
	_TypeToStr_Func
=================================================
*/
	struct NodeFunctions::_TypeToStr_Func
	{
		String& result;

		_TypeToStr_Func (String& str) : result(str) {}
		
		template <typename T, usize Index>
		forceinline void Process ()		{ result << _NodeTypeToString<T>::Get(); }
	};
	
/*
=================================================
	_BuildSignature
=================================================
*/
	template <typename InArgTypeList, typename OutArgTypeList>
	forceinline String  NodeFunctions::_BuildSignature (StringCRef name)
	{
		String			sig = name;
		_TypeToStr_Func	func( sig );

		InArgTypeList::RuntimeForEach( func );
		OutArgTypeList::RuntimeForEach( func );
		return sig;
	}
	
/*
=================================================
	_BuildRefArray
=================================================
*/
	template <typename ConstRefArray, typename RefArray, typename T, typename ...Args>
	forceinline void  NodeFunctions::_BuildRefArray (INOUT ConstRefArray &constRefArr,
													 INOUT RefArray &refArr,
													 T *first, Args* ...args)
	{
		_BuildRefArray( INOUT constRefArr, INOUT refArr, first );
		_BuildRefArray( INOUT constRefArr, INOUT refArr, args... );
	}
		
	template <typename ConstRefArray, typename RefArray, typename T>
	forceinline void  NodeFunctions::_BuildRefArray (INOUT ConstRefArray &constRefArr,
													 INOUT RefArray &refArr,
													 T *first)
	{
		if ( TypeTraits::IsConst<T> )
			constRefArr << VariantCRef::FromConst( *first );
		else
			refArr << VariantRef::FromConst( *first );
	}
//=============================================================================

}	// _ShaderNodesHidden_
}	// ShaderEditor
