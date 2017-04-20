// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

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
		template <typename T> Self&		operator _op_##= (const T& right)		{ Set( vec3_t(*this) _op_ right );  return *this; } \
		template <typename T> Self		operator _op_  (const T& right) const	{ return Self( vec3_t(*this) _op_ right ); }



	//
	// RGB9_E5 Float Vector
	//

	template <typename FT>
	struct TRGB9_E5 : public CompileTime::CopyQualifiers< FT >
	{
	// types
	public:
		typedef TRGB9_E5<FT>		Self;
		typedef Vec<FT,3>			vec3_t;
		typedef FT					float_t;

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
			Set( float_t(R), float_t(G), float_t(B) );
		}

		template <typename VT>
		TRGB9_E5 (const VT& vec)
		{
			Set( vec3_t( vec ) );
		}
		

		// set/get
		void  Set (const vec3_t &v);
		void  Set (float_t R, float_t G, float_t B);
		void  Get (OUT float_t& R, OUT float_t& G, OUT float_t& B) const;
		void  Get (OUT vec3_t &v) const;


		// type cast
		operator const vec3_t () const		{ vec3_t  ret;  Get(ret);  return ret; }


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
		vec3_t	v;
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
	inline void  TRGB9_E5<FT>::Set (const vec3_t &v)
	{
		enum {
			N		= 9,
			B		= 15,
			E_MAX	= 31,
		};

		static const uint	s_uSharedExpMax = ( (1<<N)-1 ) * ( 1<<(E_MAX-B-N) );

		ASSERT( Any( v < vec3_t(0) ) and "only unsigned value supported" );

		vec3_t		v_color = Clamp( v, vec3_t(0), vec3_t( (FT)s_uSharedExpMax ) );

		float_t		f_max_c	= v_color.Max();
		int			i_exp_p	= Max( -B-1, (int)Log2( f_max_c ) ) + B + 1;
		float_t		f_max_s	= f_max_c / _math_hidden_::_Pow2< float_t, int >( i_exp_p - B - N ) + FT(0.5);
		int			i_exp_s	= i_exp_p + not ( f_max_s < float_t(1<<N) );
		float_t		f_2e	= _math_hidden_::_Pow2< float_t, int >( i_exp_s - B - N );

		v_color = v_color / f_2e + 0.5f;

		_bits.r_m = uint( v_color.x );
		_bits.g_m = uint( v_color.y );
		_bits.b_m = uint( v_color.z );
		_bits.e   = i_exp_s;
	}

	
	template <typename FT>
	inline void  TRGB9_E5<FT>::Set (float_t R, float_t G, float_t B)
	{
		Set( vec3_t( R, G, B ) );
	}

	
	template <typename FT>
	inline void  TRGB9_E5<FT>::Get (float_t& Red, float_t& Green, float_t& Blue) const
	{
		enum {
			N		= 9,
			B		= 15,
			E_MAX	= 31,
		};

		float_t	f_exp	= _math_hidden_::_Pow2< float_t, int >( _bits.e - B - N );

		Red   = float_t( _bits.r_m ) * f_exp;
		Green = float_t( _bits.g_m ) * f_exp;
		Blue  = float_t( _bits.b_m ) * f_exp;
	}

	
	template <typename FT>
	inline void  TRGB9_E5<FT>::Get (vec3_t &v) const
	{
		Get( v.x, v.y, v.z );
	}


#	undef _VEC_OPERATOR


}	// GXMath

namespace GXTypes
{
	
	template <typename T>
	struct Hash< GXMath::TRGB9_E5<T> > : public Hash< typename GXMath::TRGB9_E5<T>::vec3_t >
	{};

}	// GXTypes
}	// GX_STL
