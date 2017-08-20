// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Shader/Node.h"
#include "Engine/Shaders/Shader/Generated/VecSwizzle.h"

namespace ShaderEditor
{
namespace ShaderNodes
{

	using Node		= _ShaderNodesHidden_::Node;

	using Float		= NVec<float, 1>;
	using Float2	= NVec<float, 2>;
	using Float3	= NVec<float, 3>;
	using Float4	= NVec<float, 4>;

	using Double	= NVec<double, 1>;
	using Double2	= NVec<double, 2>;
	using Double3	= NVec<double, 3>;
	using Double4	= NVec<double, 4>;

	using Int		= NVec<int, 1>;
	using Int2		= NVec<int, 2>;
	using Int3		= NVec<int, 3>;
	using Int4		= NVec<int, 4>;
	
	using UInt		= NVec<uint, 1>;
	using UInt2		= NVec<uint, 2>;
	using UInt3		= NVec<uint, 3>;
	using UInt4		= NVec<uint, 4>;
	
	using ILong		= NVec<ilong, 1>;
	using ILong2	= NVec<ilong, 2>;
	using ILong3	= NVec<ilong, 3>;
	using ILong4	= NVec<ilong, 4>;
	
	using ULong		= NVec<ulong, 1>;
	using ULong2	= NVec<ulong, 2>;
	using ULong3	= NVec<ulong, 3>;
	using ULong4	= NVec<ulong, 4>;

	using Bool		= NVec<bool, 1>;
	using Bool2		= NVec<bool, 2>;
	using Bool3		= NVec<bool, 3>;
	using Bool4		= NVec<bool, 4>;

	struct Shader;



	//
	// Scalar (Vec1)
	//

	template <typename T>
	struct NVec < T, 1 > final : Node
	{
	// types
	public:
		using Self		= NVec<T,1>;
		using Value_t	= T;

		static const uint	COUNT	= 1;
		
	private:
		template <typename T, typename ...Args>
		using Property	= _ShaderNodesHidden_::Property< T, Self, Args... >;
		using _X		= _ShaderNodesHidden_::ClassToType< Self >;


	// variables
	public:
		union {
			NVEC1_SWIZZLE( x, _X );
		};


	// methods
	public:
		NVec (GX_DEFCTOR) : xx{this} {}

		NVec (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Scalar ),
			xx{this}
		{}

		NVec (Self &&node) : xx{this}
		{
			_Move( node );
		}

		explicit NVec (const T &value) :
			Node( null, "", _Typename(), ENodeType::Scalar, ~0u, GX_STL::GXTypes::VariantCRef::FromConst( value ) ),
			xx{this}
		{}

		// TODO
		//template <typename ...T>
		//explicit Scalar (const T& ...args)
		//{
		//	IntCast( args... );
		//}

		Self &	operator = (const T &value);

		static Name_t  _Typename ();
	};



	//
	// NVec (Vec2)
	//

	template <typename T>
	struct NVec < T, 2 > final : Node
	{
	// types
	public:
		using Self		= NVec<T,2>;
		using Scalar_t	= NVec<T,1>;
		using Value_t	= T;
		
		static const uint	COUNT	= 2;


	// variables
	public:
		Scalar_t		x	{ this, "x" };
		Scalar_t		y	{ this, "y" };
		

	// types
	private:
		template <typename T, typename ...Args>
		using Property	= _ShaderNodesHidden_::Property< T, Self, Args... >;
		using _X		= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::x >;
		using _Y		= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::y >;


	// variables
	public:
		union {
			NVEC1_SWIZZLE( x, _X );
			NVEC1_SWIZZLE( y, _Y );
			NVEC2_SWIZZLE( x, y, _X, _Y );
		};


	// methods
	public:
		NVec (GX_DEFCTOR) : xx{this} {}

		NVec (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Vector ),
			xx{ this }
		{}
		
		NVec (Self &&node) :
			x( RVREF(node.x) ),
			y( RVREF(node.y) ),
			xx{ this }
		{
			_Move( node );
		}
		
		Self &		operator = (const GX_STL::GXMath::Vec<T,2> &value);
		
		T const&	operator [] (uint index) const;

		static Name_t  _Typename ();
	};



	//
	// NVec (Vec3)
	//

	template <typename T>
	struct NVec < T, 3 > final : Node
	{
	// types
	public:
		using Self		= NVec<T,3>;
		using Scalar_t	= NVec<T,1>;
		using Value_t	= T;
		
		static const uint	COUNT	= 3;


	// variables
	public:
		Scalar_t		x	{ this, "x" };
		Scalar_t		y	{ this, "y" };
		Scalar_t		z	{ this, "z" };

		
	// types
	private:
		template <typename T, typename ...Args>
		using Property	= _ShaderNodesHidden_::Property< T, Self, Args... >;
		using _X		= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::x >;
		using _Y		= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::y >;
		using _Z		= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::z >;
		

	// variables
	public:
		union {
			NVEC1_SWIZZLE( x, _X );
			NVEC1_SWIZZLE( y, _Y );
			NVEC1_SWIZZLE( z, _Z );
			NVEC2_SWIZZLE( x, y, _X, _Y );
			NVEC2_SWIZZLE( x, z, _X, _Z );
			NVEC2_SWIZZLE( y, z, _Y, _Z );
			NVEC3_SWIZZLE( x, y, z, _X, _Y, _Z );
		};


	// methods
	public:
		NVec (GX_DEFCTOR) : xx{this} {}

		NVec (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Vector ),
			xx{this}
		{}
		
		NVec (Self &&node) :
			x( RVREF(node.x) ),
			y( RVREF(node.y) ),
			z( RVREF(node.z) ),
			xx{this}
		{
			_Move( node );
		}
		
		Self &		operator = (const GX_STL::GXMath::Vec<T,3> &value);
		
		T const&	operator [] (uint index) const;

		static Name_t  _Typename ();
	};



	//
	// NVec (Vec4)
	//

	template <typename T>
	struct NVec < T, 4 > final : Node
	{
	// types
	public:
		using Self		= NVec<T,4>;
		using Scalar_t	= NVec<T,1>;
		using Value_t	= T;
		
		static const uint	COUNT	= 4;


	// variables
	public:
		Scalar_t		x	{ this, "x" };
		Scalar_t		y	{ this, "y" };
		Scalar_t		z	{ this, "z" };
		Scalar_t		w	{ this, "w" };
		

	// types
	private:
		template <typename T, typename ...Args>
		using Property	= _ShaderNodesHidden_::Property< T, Self, Args... >;
		using _X		= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::x >;
		using _Y		= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::y >;
		using _Z		= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::z >;
		using _W		= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::w >;
		

	// variables
	public:
		union {
			NVEC1_SWIZZLE( x, _X );
			NVEC1_SWIZZLE( y, _Y );
			NVEC1_SWIZZLE( z, _Z );
			NVEC1_SWIZZLE( w, _W );
			NVEC2_SWIZZLE( x, y, _X, _Y );
			NVEC2_SWIZZLE( x, z, _X, _Z );
			NVEC2_SWIZZLE( y, z, _Y, _Z );
			NVEC2_SWIZZLE( x, w, _X, _W );
			NVEC2_SWIZZLE( y, w, _Y, _W );
			NVEC2_SWIZZLE( z, w, _Z, _W );
			NVEC3_SWIZZLE( x, y, z, _X, _Y, _Z );
			NVEC3_SWIZZLE( x, y, w, _X, _Y, _W );
			NVEC3_SWIZZLE( x, z, w, _X, _Z, _W );
			NVEC3_SWIZZLE( y, z, w, _Y, _Z, _W );
			NVEC4_SWIZZLE( x, y, z, w, _X, _Y, _Z, _W );
		};


	// methods
	public:
		NVec (GX_DEFCTOR) : xx{this} {}

		NVec (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Vector ),
			xx{this}
		{}
		
		NVec (Self &&node) :
			x( RVREF(node.x) ),
			y( RVREF(node.y) ),
			z( RVREF(node.z) ),
			w( RVREF(node.w) ),
			xx{this}
		{
			_Move( node );
		}
		
		Self &		operator = (const GX_STL::GXMath::Vec<T,4> &value);

		T const&	operator [] (uint index) const;

		static Name_t  _Typename ();
	};

	
	
/*
=================================================
	_Typename
=================================================
*/
	template <>	inline Node::Name_t  NVec<bool, 1>::_Typename ()	{ return "Bool"; }
	template <>	inline Node::Name_t  NVec<int, 1>::_Typename ()		{ return "Int"; }
	template <>	inline Node::Name_t  NVec<uint, 1>::_Typename ()	{ return "UInt"; }
	template <>	inline Node::Name_t  NVec<ilong, 1>::_Typename ()	{ return "ILong"; }
	template <>	inline Node::Name_t  NVec<ulong, 1>::_Typename ()	{ return "ULong"; }
	template <>	inline Node::Name_t  NVec<float, 1>::_Typename ()	{ return "Float"; }
	template <>	inline Node::Name_t  NVec<double, 1>::_Typename ()	{ return "Double"; }

	template <typename T>
	inline Node::Name_t  NVec<T,2>::_Typename ()
	{
		return Scalar_t::_Typename() << "2";
	}

	template <typename T>
	inline Node::Name_t  NVec<T,3>::_Typename ()
	{
		return Scalar_t::_Typename() << "3";
	}

	template <typename T>
	inline Node::Name_t  NVec<T,4>::_Typename ()
	{
		return Scalar_t::_Typename() << "4";
	}
	
/*
=================================================
	operator =
=================================================
*
	template <typename T>
	inline Scalar<T> &  Scalar<T>::operator = (const T &value)
	{
		_Set( new _ShaderNodesHidden_::ISrcNode( _SelfNode()->Parent(),
												GX_STL::GXTypes::VariantCRef::FromConst( value ),
												_SelfNode()->Name(),
												_SelfNode()->TypeName(),
												_SelfNode()->Type(),
												_SelfNode()->Index() ));
		return *this;
	}
	

	template <typename T>
	inline Vec2<T> &  Vec2<T>::operator = (const GX_STL::GXMath::Vec<T,2> &value)
	{
		x = value.x;
		y = value.y;
		return *this;
	}
	

	template <typename T>
	inline Vec3<T> &  Vec3<T>::operator = (const GX_STL::GXMath::Vec<T,3> &value)
	{
		x = value.x;
		y = value.y;
		z = value.z;
		return *this;
	}
	

	template <typename T>
	inline Vec4<T> &  Vec4<T>::operator = (const GX_STL::GXMath::Vec<T,4> &value)
	{
		x = value.x;
		y = value.y;
		z = value.z;
		w = value.w;
		return *this;
	}
	
/*
=================================================
	operator []
=================================================
*/
	template <typename T>
	inline T const&  NVec<T,2>::operator [] (uint index) const
	{
		ASSERT( index < 2 );

		return	index == 0 ?	x :
				index == 1 ?	y :
				*((T const*) null);
	}
	
	template <typename T>
	inline T const&  NVec<T,3>::operator [] (uint index) const
	{
		ASSERT( index < 3 );

		return	index == 0 ?	x :
				index == 1 ?	y :
				index == 2 ?	z :
				*((T const*) null);
	}
	
	template <typename T>
	inline T const&  NVec<T,4>::operator [] (uint index) const
	{
		ASSERT( index < 4 );

		return	index == 0 ?	x :
				index == 1 ?	y :
				index == 2 ?	z :
				index == 3 ?	w :
				*((T const*) null);
	}


}	// ShaderNodes
}	// ShaderEditor
