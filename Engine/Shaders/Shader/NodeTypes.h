// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Shader/Node.h"

namespace ShaderEditor
{
namespace ShaderNodes
{

	using uint			= GX_STL::GXTypes::uint;
	using ilong			= GX_STL::GXTypes::ilong;
	using ulong			= GX_STL::GXTypes::ulong;
	using Node			= _ShaderNodesHidden_::Node;
	
	template <typename T>	struct Scalar;
	template <typename T>	struct Vec2;
	template <typename T>	struct Vec3;
	template <typename T>	struct Vec4;



	//
	// Scalar
	//

	template <typename T>
	struct Scalar final : Node
	{
	// types
	public:
		using Self		= Scalar<T>;
		using Value_t	= T;

		static const uint	COUNT	= 1;


	// methods
	public:
		Scalar (GX_DEFCTOR) {}

		Scalar (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Scalar )
		{}

		Scalar (Self &&node)
		{
			_Move( node );
		}

		explicit Scalar (const T &value) :
			Node( null, "", _Typename(), ENodeType::Scalar, ~0u, GX_STL::GXTypes::VariantCRef::FromConst( value ) )
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
	// Vec2
	//

	template <typename T>
	struct Vec2 final : Node
	{
	// types
	public:
		using Self		= Vec2<T>;
		using Scalar_t	= Scalar<T>;
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

		using _X	= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::x >;
		using _Y	= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::y >;


	// variables
	public:
		union {
			Property< Vec2<T>, _X, _X >				xx;
			Property< Vec2<T>, _X, _Y >				xy;
			Property< Vec3<T>, _X, _X, _X >			xxx;
			Property< Vec4<T>, _X, _X, _X, _X >		xxxx;
		};


	// methods
	public:
		Vec2 (GX_DEFCTOR) : xx{this} {}

		Vec2 (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Vector ), xx{ this }
		{}
		
		Vec2 (Self &&node) :
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
	// Vec3
	//

	template <typename T>
	struct Vec3 final : Node
	{
	// types
	public:
		using Self		= Vec3<T>;
		using Scalar_t	= Scalar<T>;
		using Value_t	= T;
		
		static const uint	COUNT	= 3;


	// variables
	public:
		Scalar<T>		x	{ this, "x" };
		Scalar<T>		y	{ this, "y" };
		Scalar<T>		z	{ this, "z" };

		
	// types
	private:
		template <typename T, typename ...Args>
		using Property	= _ShaderNodesHidden_::Property< T, Self, Args... >;

		using _X	= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::x >;
		using _Y	= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::y >;
		using _Z	= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::z >;
		

	// variables
	public:
		union {
			Property< Vec2<T>, _X, _X >				xx;
			Property< Vec2<T>, _X, _Y >				xy;
			Property< Vec3<T>, _X, _X, _X >			xxx;
			Property< Vec4<T>, _X, _X, _X, _X >		xxxx;
		};


	// methods
	public:
		Vec3 (GX_DEFCTOR) {}

		Vec3 (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Vector )
		{}
		
		Vec3 (Self &&node) :
			x( RVREF(node.x) ),
			y( RVREF(node.y) ),
			z( RVREF(node.z) )
		{
			_Move( node );
		}
		
		Self &		operator = (const GX_STL::GXMath::Vec<T,3> &value);
		
		T const&	operator [] (uint index) const;

		static Name_t  _Typename ();
	};



	//
	// Vec4
	//

	template <typename T>
	struct Vec4 final : Node
	{
	// types
	public:
		using Self		= Vec4<T>;
		using Scalar_t	= Scalar<T>;
		using Value_t	= T;
		
		static const uint	COUNT	= 4;


	// variables
	public:
		Scalar<T>		x	{ this, "x" };
		Scalar<T>		y	{ this, "y" };
		Scalar<T>		z	{ this, "z" };
		Scalar<T>		w	{ this, "w" };
		

	// types
	private:
		template <typename T, typename ...Args>
		using Property	= _ShaderNodesHidden_::Property< T, Self, Args... >;

		using _X	= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::x >;
		using _Y	= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::y >;
		using _Z	= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::z >;
		using _W	= _ShaderNodesHidden_::MemberToType< Self, Scalar_t, &Self::w >;
		

	// variables
	public:
		union {
			Property< Vec2<T>, _X, _X >				xx;
			Property< Vec2<T>, _X, _Y >				xy;
			Property< Vec3<T>, _X, _X, _X >			xxx;
			Property< Vec4<T>, _X, _X, _X, _X >		xxxx;
		};


	// methods
	public:
		Vec4 (GX_DEFCTOR) {}

		Vec4 (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Vector )
		{}
		
		Vec4 (Self &&node) :
			x( RVREF(node.x) ),
			y( RVREF(node.y) ),
			z( RVREF(node.z) ),
			w( RVREF(node.w) )
		{
			_Move( node );
		}
		
		Self &		operator = (const GX_STL::GXMath::Vec<T,4> &value);

		T const&	operator [] (uint index) const;

		static Name_t  _Typename ();
	};



	//
	// Array (static)
	//

	template <typename T, uint I>
	struct Array : Node
	{
	// types
	public:
		using Self		= Array< T, I >;
		using Value_t	= T;
		

	// variables
	private:
		GX_STL::GXTypes::StaticArray< T, I >		_values;


	// methods
	public:
		Array (GX_DEFCTOR) {}


		Array (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Array )
		{
			TODO( "" );
			/*FOR( i, _values ) {
				_values[i] = RVREF( T( this, Name_t("[") << i << "]" ) );
			}*/
		}


		Array (Self &&node)
		{
			TODO( "" );
			/*FOR( i, _values ) {
				_values[i] = RVREF( other._values[i] );
			}
			_Move( node );*/
		}


		T const&	operator [] (uint index) const;

		static Name_t  _Typename ();
	};



	//
	// Shader Input
	//

	template <typename T>
	struct Input : Node
	{
	};



	//
	// Shader Output
	//

	template <typename T>
	struct Output : Node
	{
	};



	//
	// Local Shared
	//

	template <typename T>
	struct LocalShared : Node
	{
	// types
	public:
		using Self		= LocalShared<T>;
		using Value_t	= T;


	// variables
	private:
		T		_data;


	// methods
	public:
		LocalShared (Node *parent, StringCRef name) :
			Node( parent, name, "local_shared", ENodeType::LocalShared ),
			_data{ this, "data" }
		{}
	};


	using Float		= Scalar<float>;
	using Float2	= Vec2<float>;
	using Float3	= Vec3<float>;
	using Float4	= Vec4<float>;

	using Double	= Scalar<double>;
	using Double2	= Vec2<double>;
	using Double3	= Vec3<double>;
	using Double4	= Vec4<double>;

	using Int		= Scalar<int>;
	using Int2		= Vec2<int>;
	using Int3		= Vec3<int>;
	using Int4		= Vec4<int>;
	
	using UInt		= Scalar<uint>;
	using UInt2		= Vec2<uint>;
	using UInt3		= Vec3<uint>;
	using UInt4		= Vec4<uint>;
	
	using Long		= Scalar<ilong>;
	using Long2		= Vec2<ilong>;
	using Long3		= Vec3<ilong>;
	using Long4		= Vec4<ilong>;
	
	using ULong		= Scalar<ulong>;
	using ULong2	= Vec2<ulong>;
	using ULong3	= Vec3<ulong>;
	using ULong4	= Vec4<ulong>;

	using Bool		= Scalar<bool>;
	using Bool2		= Vec2<bool>;
	using Bool3		= Vec3<bool>;
	using Bool4		= Vec4<bool>;


	template <typename T, uint I>	struct _BaseTypeFrom;
	template <typename T>			struct _BaseTypeFrom< T, 1 > { using type = Scalar<T>; };
	template <typename T>			struct _BaseTypeFrom< T, 2 > { using type = Vec2<T>; };
	template <typename T>			struct _BaseTypeFrom< T, 3 > { using type = Vec3<T>; };
	template <typename T>			struct _BaseTypeFrom< T, 4 > { using type = Vec4<T>; };
	
	template <typename T, uint I>
	using BaseTypeFrom	= typename _BaseTypeFrom<T,I>::type;
	
	
/*
=================================================
	_Typename
=================================================
*/
	template <>	inline Node::Name_t  Scalar<bool>::_Typename ()		{ return "bool"; }
	template <>	inline Node::Name_t  Scalar<int>::_Typename ()		{ return "int"; }
	template <>	inline Node::Name_t  Scalar<uint>::_Typename ()		{ return "uint"; }
	template <>	inline Node::Name_t  Scalar<ilong>::_Typename ()	{ return "ilong"; }
	template <>	inline Node::Name_t  Scalar<ulong>::_Typename ()	{ return "ulong"; }
	template <>	inline Node::Name_t  Scalar<float>::_Typename ()	{ return "float"; }
	template <>	inline Node::Name_t  Scalar<double>::_Typename ()	{ return "double"; }

	template <typename T>
	inline Node::Name_t  Vec2<T>::_Typename ()
	{
		return Scalar<T>::_Typename() << "2";
	}

	template <typename T>
	inline Node::Name_t  Vec3<T>::_Typename ()
	{
		return Scalar<T>::_Typename() << "3";
	}

	template <typename T>
	inline Node::Name_t  Vec4<T>::_Typename ()
	{
		return Scalar<T>::_Typename() << "4";
	}
	
	template <typename T, uint I>
	inline Node::Name_t  Array<T,I>::_Typename ()
	{
		return T::_Typename() << '[' << Name_t().FormatI( I, 10 ) << ']';
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
	inline T const&  Vec2<T>::operator [] (uint index) const
	{
		ASSERT( index < 2 );

		return	index == 0 ?	x :
				index == 1 ?	y :
				*((T const*) null);
	}
	
	template <typename T>
	inline T const&  Vec3<T>::operator [] (uint index) const
	{
		ASSERT( index < 3 );

		return	index == 0 ?	x :
				index == 1 ?	y :
				index == 2 ?	z :
				*((T const*) null);
	}
	
	template <typename T>
	inline T const&  Vec4<T>::operator [] (uint index) const
	{
		ASSERT( index < 4 );

		return	index == 0 ?	x :
				index == 1 ?	y :
				index == 2 ?	z :
				index == 3 ?	w :
				*((T const*) null);
	}

	template <typename T, uint I>
	inline T const&  Array<T,I>::operator [] (uint index) const
	{
		ASSERT( index < I );
		ASSERT( I == _SelfNode()->Fields().Count() );

		return	index < I ? _SelfNode()->Fields()[ index ] :
				*((T const*) null);
	}


}	// ShaderNodes
}	// ShaderEditor
