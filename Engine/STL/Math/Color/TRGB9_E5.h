// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Math/Mathematics.h"
#include "Engine/STL/Math/Vec.h"

namespace GX_STL
{
namespace GXMath
{
	namespace _math_hidden_
	{
		template <typename R, typename T>
		inline R _Pow2 (const int& p)
		{
			if ( p >= 0 )
				return R( T(1) << p );
			else
				return R(1) / R( T(1) << (-p) ); 
		}

	}	// _math_hidden_


#	define _VEC_OPERATOR( _op_ ) \
		template <typename T> Self&		operator _op_##= (const T& right)		{ Set( Vec3_t(*this) _op_ right );  return *this; } \
		template <typename T> Self		operator _op_  (const T& right) const	{ return Self( Vec3_t(*this) _op_ right ); }



	//
	// RGB9_E5 Float Vector
	//

	template <typename FT>
	struct TRGB9_E5 : public CompileTime::CopyQualifiers< FT >
	{
	// types
	public:
		typedef TRGB9_E5<FT>		Self;
		typedef Vec<FT,3>			Vec3_t;
		typedef FT					Float_t;

		struct TBits
		{
			uint	r_m	: 9;
			uint	g_m	: 9;
			uint	b_m	: 9;
			uint	e	: 5;
		};


	// variables
	private:
		union {
			TBits		_bits;
			uint		_value;
		};


	// methods
	public:
		// constructors
		TRGB9_E5 (GX_DEFCTOR): _value(0) {}

		template <typename RT, typename GT, typename BT>
		TRGB9_E5 (const RT& R, const GT& G, const BT& B)
		{
			Set( Float_t(R), Float_t(G), Float_t(B) );
		}

		template <typename VT>
		TRGB9_E5 (const VT& vec)
		{
			Set( Vec3_t( vec ) );
		}
		

		// set/get
		void  Set (const Vec3_t &v);
		void  Set (Float_t R, Float_t G, Float_t B);
		void  Get (OUT Float_t& R, OUT Float_t& G, OUT Float_t& B) const;
		void  Get (OUT Vec3_t &v) const;


		// type cast
		operator const Vec3_t () const		{ Vec3_t  ret;  Get(ret);  return ret; }


		// unary operators
		Self	operator - () const;
		Self	operator ~ () const;

		// binary operators
		_VEC_OPERATOR( +  );
		_VEC_OPERATOR( -  );
		_VEC_OPERATOR( *  );
		_VEC_OPERATOR( /  );
		_VEC_OPERATOR( %  );
		_VEC_OPERATOR( &  );
		_VEC_OPERATOR( |  );
		_VEC_OPERATOR( ^  );
		_VEC_OPERATOR( >> );
		_VEC_OPERATOR( << );


		// check
		STATIC_ASSERT( sizeof(uint) == 4 and sizeof(FT) == sizeof(uint), "incorrect types" );
	};

	typedef TRGB9_E5<float>		rgb9_e5_t;

	


	template <typename FT>
	inline TRGB9_E5<FT>  TRGB9_E5<FT>::operator - () const
	{
		Vec3_t	v;
		Get( v );
		return Self( -v );
	}


	template <typename FT>
	inline TRGB9_E5<FT>  TRGB9_E5<FT>::operator ~ () const
	{
		Self	ret;
		ret._value = ~_value;
		return ret;
	}


	template <typename FT>
	inline void  TRGB9_E5<FT>::Set (const Vec3_t &v)
	{
		enum {
			N		= 9,
			B		= 15,
			E_MAX	= 31,
		};

		static const uint	s_uSharedExpMax = ( (1<<N)-1 ) * ( 1<<(E_MAX-B-N) );

		ASSERT( Any( v < Vec3_t(0) ) and "only unsigned value supported" );

		Vec3_t		v_color = Clamp( v, Vec3_t(0), Vec3_t( (FT)s_uSharedExpMax ) );

		Float_t		f_max_c	= v_color.Max();
		int			i_exp_p	= Max( -B-1, (int)Log2( f_max_c ) ) + B + 1;
		Float_t		f_max_s	= f_max_c / _math_hidden_::_Pow2< Float_t, int >( i_exp_p - B - N ) + FT(0.5);
		int			i_exp_s	= i_exp_p + not ( f_max_s < Float_t(1<<N) );
		Float_t		f_2e	= _math_hidden_::_Pow2< Float_t, int >( i_exp_s - B - N );

		v_color = v_color / f_2e + 0.5f;

		_bits.r_m = uint( v_color.x );
		_bits.g_m = uint( v_color.y );
		_bits.b_m = uint( v_color.z );
		_bits.e   = i_exp_s;
	}

	
	template <typename FT>
	inline void  TRGB9_E5<FT>::Set (Float_t R, Float_t G, Float_t B)
	{
		Set( Vec3_t( R, G, B ) );
	}

	
	template <typename FT>
	inline void  TRGB9_E5<FT>::Get (Float_t& Red, Float_t& Green, Float_t& Blue) const
	{
		enum {
			N		= 9,
			B		= 15,
			E_MAX	= 31,
		};

		Float_t	f_exp	= _math_hidden_::_Pow2< Float_t, int >( _bits.e - B - N );

		Red   = Float_t( _bits.r_m ) * f_exp;
		Green = Float_t( _bits.g_m ) * f_exp;
		Blue  = Float_t( _bits.b_m ) * f_exp;
	}

	
	template <typename FT>
	inline void  TRGB9_E5<FT>::Get (Vec3_t &v) const
	{
		Get( v.x, v.y, v.z );
	}


#	undef _VEC_OPERATOR


}	// GXMath

namespace GXTypes
{
	
	template <typename T>
	struct Hash< GXMath::TRGB9_E5<T> > : public Hash< typename GXMath::TRGB9_E5<T>::Vec3_t >
	{};

}	// GXTypes
}	// GX_STL
