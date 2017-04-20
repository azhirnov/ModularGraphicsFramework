// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Common/Types.h"
#include "Engine/STL/Common/AllFunc.h"
#include "Engine/STL/CompileTime/NewTypeInfo.h"
#include "Engine/STL/Algorithms/Hash.h"

namespace GX_STL
{
namespace GXTypes
{
	
#	define _HALF_OPERATOR( _op_, _ret1_, _func1_, _ret2_, _func2_, _ret3_, _func3_ ) \
									 _ret1_ operator _op_ (const Self& right) const				_func1_ \
		template <typename T>		 _ret2_ operator _op_ (const T& right) const				_func2_ \
		template <typename T> friend _ret3_ operator _op_ (const T& left, const THalf& right)	_func3_

#	define _HALF_CMP_OPERATOR( _op_ ) \
		bool operator _op_ (const Self& right) const	{ return Get() _op_ right.Get(); } \
		bool operator _op_ (const float& right) const	{ return Get() _op_ right; }




	//
	// Half Float
	//

	struct THalf : public CompileTime::PODType
	{
	// types
	public:
		typedef THalf	Self;
		typedef ushort	int_t;

		struct THalfBits
		{
			ushort	m	: 10;	// mantissa
			ushort	e	: 5;	// exponent
			ushort	s	: 1;	// sign
		};


	// variables
	private:
		union {
			THalfBits	_bits;
			ushort		_value;
		};


	// methods
	public:
		// constructors
		THalf (): _value(0)								{}
		THalf (const Self &h): _value( h._value )		{}
	
		template <typename T>
		explicit THalf (const T& val)					{ Set( float(val) ); }


		// set/get
		ushort	GetU () const							{ return _value; }
		//ushort&	GetU ()									{ return _value; }
		int		GetI () const							{ return int(Get()); }
		float	Get () const;
		void	Set (const float& f);


		// type cast
		//operator int () const							{ return GetI(); }
		operator float () const							{ return Get(); }
	

		// unary operators
		Self &	operator ++ ()							{ ++_bits.m; return *this; }
		Self &	operator -- ()							{ --_bits.m; return *this; }
		float	operator ++ (int) 						{ float ret(*this);  ++(*this);  return ret; }
		float	operator -- (int)						{ float ret(*this);  --(*this);  return ret; }
		bool	operator !  () const					{ return GXMath::IsZero( Get() ); }
		float	operator +  () const					{ return +Get(); }
		float	operator -  () const					{ return -Get(); }
		Self	operator ~  () const					{ Self  ret;  ret._value = ushort(~_value);  return ret; }


		// binary operators
		Self & operator =  (const Self& h)				{ _value = h._value;  return *this; }

		template <typename T>	Self & operator =   (const T& right)	{ Set( float(right) );  return *this; }
		template <typename T>	Self & operator +=  (const T& right)	{ Set( Get() + right );  return *this; }
		template <typename T>	Self & operator -=  (const T& right)	{ Set( Get() - right );  return *this; }
		template <typename T>	Self & operator *=  (const T& right)	{ Set( Get() * right );  return *this; }
		template <typename T>	Self & operator /=  (const T& right)	{ Set( Get() / right );  return *this; }
		template <typename T>	Self & operator %=  (const T& right)	{ Set( Mod( Get(), right ) );  return *this; }

		template <typename T>	Self & operator <<= (const T& right)	{ _bits.e += byte(right);  return *this; }
		template <typename T>	Self & operator >>= (const T& right)	{ _bits.e -= byte(right);  return *this; }

		_HALF_OPERATOR( +,
				float,	{ return Get() + right.Get(); },
				float,	{ return Get() + float(right); },
				T,		{ return left  + T(right.Get()); }
		);
		_HALF_OPERATOR( -,
				float,	{ return Get() - right.Get(); },
				float,	{ return Get() - float(right); },
				T,		{ return left  - T(right.Get()); }
		);
		_HALF_OPERATOR( *,
				float,	{ return Get() * right.Get(); },
				float,	{ return Get() * float(right); },
				T,		{ return left  * T(right.Get()); }
		);
		_HALF_OPERATOR( /,
				float,	{ return Get() / right.Get(); },
				float,	{ return Get() / float(right); },
				T,		{ return left  / T(right.Get()); }
		)
		_HALF_OPERATOR( %,
				float,	{ return (float)::fmod( Get(), right.Get() ); },
				float,	{ return (float)::fmod( Get(), float(right) ); },
				T,		{ return (float)::fmod( left, T(right.Get()) ); }
		);

		_HALF_CMP_OPERATOR( == );
		_HALF_CMP_OPERATOR( != );
		_HALF_CMP_OPERATOR( >= );
		_HALF_CMP_OPERATOR( >  );
		_HALF_CMP_OPERATOR( <= );
		_HALF_CMP_OPERATOR( <  );


		// methods
		ushort	GetMantissaBits ()	const	{ return _bits.m; }
		short	GetExponentBits ()	const	{ return short(_bits.e) - (1<<4) + 1; }
		byte	GetSign ()			const	{ return _bits.s ? -1 : 1; }

		float	GetMantissa ()		const	{ return float(1) + float(_bits.m) / float( (1<<10)-1 ); }
		float	GetExponent ()		const;


		// checks //
		STATIC_ASSERT( sizeof(ushort) == 2, "incorrect size" );
	};


	typedef THalf	half;



/*
=================================================
	GetExponent
=================================================
*/
	inline float THalf::GetExponent () const
	{
		short	p = GetExponentBits();
		
		if ( p >= 0 )
			return float( 1 << p );
		else
			return 1.f / float( 1 << (-p) ); 
	}

/*
=================================================
	Get
=================================================
*/
	inline float THalf::Get () const
	{
#	if 0
		// fast conversion

		float_t	f;
		f._bits.s = _bits.s;
		f._bits.e = _bits.e + (127 - 15);
		f._bits.m = _bits.m << (23-10);
		return f;
#	else
		// accurate conversion
		// based on code from OpenGL Mathematics (GLM)
		// from http://glm.g-truc.net/

		int s = (_value >> 15) & 0x00000001;
		int e = (_value >> 10) & 0x0000001f;
		int m =  _value        & 0x000003ff;

		if (e == 0)
		{
			if (m == 0)
				return ReferenceCast<float>( s << 31 );
			else
			{
				while (not (m & 0x00000400))
				{
					m <<= 1;
					e -=  1;
				}

				e += 1;
				m &= ~0x00000400;
			}
		}
		else
		if (e == 31)
		{
			if (m == 0)
				return ReferenceCast<float>( (s << 31) | 0x7f800000 );
			else
				return ReferenceCast<float>( (s << 31) | 0x7f800000 | (m << 13) );
		}

		e = e + (127 - 15);
		m = m << 13;

		return ReferenceCast<float>( (s << 31) | (e << 23) | m );
#	endif
	}
	
/*
=================================================
	Set
=================================================
*/
	inline void THalf::Set (const float& F)
	{
#	if 0
		// fast conversion

		float_t	f(F);
		_bits.s = f._bits.s;
		_bits.e = f._bits.e - (127 - 15);
		_bits.m = f._bits.m >> (23-10);
#	else
		// accurate conversion
		// based on code from OpenGL Mathematics (GLM)
		// from http://glm.g-truc.net/

		int i = ReferenceCast<int>(F);
		int s =  (i >> 16) & 0x00008000;
		int e = ((i >> 23) & 0x000000ff) - (127 - 15);
		int m =   i        & 0x007fffff;

		if (e <= 0)
		{
			if (e < -10) {
				_value = 0;
				return;
			}

			m = (m | 0x00800000) >> (1 - e);

			if (m & 0x00001000) 
				m += 0x00002000;

			_value = ushort( s | (m >> 13) );
			return;
		}
		else
		if (e == 0xff - (127 - 15))
		{
			if (m == 0)
			{
				_value = ushort( s | 0x7c00 );
				return;
			}
			else
			{
				m >>= 13;
				_value = ushort( s | 0x7c00 | m | (m == 0) );
				return;
			}
		}
		else
		{
			if (m &  0x00001000)
			{
				m += 0x00002000;

				if (m & 0x00800000)
				{
					m =  0;
					e += 1;
				}
			}

			if (e > 30)
			{
				_value = ushort( s | 0x7c00 );
				return;
			}

			_value = ushort( s | (e << 10) | (m >> 13) );
			return;
		}
#	endif
	}
	

#	undef _HALF_OPERATOR
#	undef _HALF_CMP_OPERATOR

	
	template <>
	struct Hash< THalf > :
		private Hash< THalf::int_t >
	{
		typedef THalf						key_t;
		typedef Hash< key_t::int_t >		base_t;
		typedef typename base_t::result_t	result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x.GetU() );
		}
	};
	
}	// GXTypes

namespace CompileTime
{

	template <>
	struct TypeInfo< GXTypes::half >
	{
		typedef GXTypes::half	type;
		typedef type			inner_type;
		
		template <typename OtherType>
		using CreateWith = OtherType;

		enum {
			FLAGS	= (int)_ctime_hidden_::FLOAT | (int)_ctime_hidden_::SIGNED | (int)_ctime_hidden_::SOFTWARE,
		};

		static			 type	Max()		{ return (type) GXTypes::MaxValue<float>(); }
		static			 type	Min()		{ return (type) GXTypes::MinValue<float>(); }
		static			 type	Inf()		{ return type(0); }	// TODO
		static			 type	NaN()		{ return type(0); }	// TODO
		
		static			 type	Epsilon()	{ return (type) GXTypes::Epsilon<float>(); }
		static constexpr uint	SignBit()	{ return SizeOf<type>::bits-1; }
		static constexpr uint	Count()		{ return 1; }
	};

}	// CompileTime
}	// GX_STL
