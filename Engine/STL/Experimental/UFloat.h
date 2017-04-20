// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Float.h"
#include "TRGB9_E5.h"

namespace GX_STL
{
namespace GXTypes
{

#	define _UFLOAT_OVERFLOW( _large_, _real_ )	ASSERT( _large_ != _real_ and "overflow" )

#	define _UFLOAT_OPERATOR( _op_ ) \
		template <typename T>		 Self&	operator _op_##= (const T& right)					{ Set( Get() _op_ FT(right) );  return *this; } \
									 Self	operator _op_    (const Self& right) const			{ return Get() _op_ right.Get(); } \
		template <typename T>		 Self	operator _op_    (const T& right) const				{ return Get() _op_ right; } \
		template <typename T> friend T		operator _op_    (const T& left, const Self& right)	{ return left _op_ right.Get(); } \

#	define _UFLOAT_CMP_OPERATOR( _op_ ) \
									 bool operator _op_ (const Self& right) const			{ return Get() _op_ right.Get(); } \
		template <typename T>		 bool operator _op_ (const T& right) const				{ return Get() _op_ right; } \
		template <typename T> friend bool operator _op_ (const T& left, const Self& right)	{ return left _op_ right.Get(); } \



	//
	// Unsigned Float
	//

	template <typename FT, typename IT, usize Mbits, usize Ebits>
	struct TUFloat : public CompileTime::PODType
	{
	// types
	public:
		typedef TUFloat< FT, IT, Mbits, Ebits >		Self;
		typedef FT									float_t;
		typedef IT									int_t;

		struct TBits
		{
			IT	m	: Mbits;
			IT	e	: Ebits;
		};


	// variables
	private:
		union {
			TBits	_bits;
			IT		_value;
		};


	// methods
	public:
		// constructor
		TUFloat () : _value(0) {}

		template <typename T>
		explicit
		TUFloat (const T& t)					{ Set( FT( t ) ); }


		// type cast
		operator float_t () const				{ return Get(); }


		// unary operators
		Self &	operator ++ ()					{ Set( Get()+1 );  return *this; }
		Self &	operator -- ()					{ Set( Get()-1 );  return *this; }
		float_t	operator ++ (int) 				{ float_t  temp( Get() );  ++(*this);  return temp; }
		float_t	operator -- (int) 				{ float_t  temp( Get() );  --(*this);  return temp; }
		bool	operator ! ()	const			{ return not _value; }


		// binary operators
		_UFLOAT_OPERATOR( +  );
		_UFLOAT_OPERATOR( -  );
		_UFLOAT_OPERATOR( *  );
		_UFLOAT_OPERATOR( /  );
		_UFLOAT_OPERATOR( %  );
		_UFLOAT_OPERATOR( |  );
		_UFLOAT_OPERATOR( &  );
		_UFLOAT_OPERATOR( ^  );
		_UFLOAT_OPERATOR( >> );
		_UFLOAT_OPERATOR( << );

		_UFLOAT_CMP_OPERATOR( == );
		_UFLOAT_CMP_OPERATOR( != );
		_UFLOAT_CMP_OPERATOR( >  );
		_UFLOAT_CMP_OPERATOR( >= );
		_UFLOAT_CMP_OPERATOR( <  );
		_UFLOAT_CMP_OPERATOR( <= );


		// methods
		static usize	NumMantissaBits ()		{ return Mbits; }
		static usize	NumExponentBits ()		{ return Ebits; }
		
		IT		GetMantissaBit ()	const		{ return _bits.m; }
		IT		GetExponentBit ()	const		{ return IT(_bits.e) - (1<<(Ebits-1)) + 1; }
		FT		GetMantissa ()		const		{ return FT(1) + FT(_bits.m) / FT( (1<<Mbits)-1 ); }
		FT		GetExponent ()		const		{ return _math_hidden_::_Pow2<FT,IT>( GetExponentBit() ); }
		FT		Get ()				const		{ return GetMantissa() * GetExponent(); }
		int_t	GetInt ()			const		{ return _value; }

		void	Set (const float_t &f);
	};


	typedef TUFloat< float, ubyte,   4, 4 >		float_m4_e4_t;
	typedef TUFloat< float, ushort,  5, 5 >		float_m5_e5_t;
	typedef TUFloat< float, ushort,  6, 5 >		float_m6_e5_t;
	typedef TUFloat< float, ushort,  9, 5 >		float_m9_e5_t;
	typedef TUFloat< float, ushort, 11, 5 >		float_m11_e5_t;

	typedef float_m4_e4_t						float8u_t;
	typedef float_m5_e5_t						float10u_t;
	typedef float_m6_e5_t						float11u_t;
	typedef float_m9_e5_t						float14u_t;
	typedef float_m11_e5_t						float16u_t;


	
/*
=================================================
	Set
=================================================
*/
	template <typename FT, typename IT, usize Mbits, usize Ebits>
	inline void TUFloat<FT,IT,Mbits,Ebits>::Set (const float_t &val)
	{
		static const uint EXP = (1 << (Ebits-1)) - 1;

		const typename TFloatSimilar<float_t>::type	f( val );

		ASSERT( f._bits.s == 0 and "only unsigned values suppoted" );

		IT	tmp;

		_bits.e = tmp = IT( f.GetExponentBit() + EXP );
		_UFLOAT_OVERFLOW( tmp, _bits.e );

		_bits.m = tmp = IT( f.GetMantissaBit() >> ( f.NumMantissaBits() - NumMantissaBits() ) );
		_UFLOAT_OVERFLOW( tmp, _bits.m );
	}


#	undef _UFLOAT_OVERFLOW
#	undef _UFLOAT_OPERATOR
#	undef _UFLOAT_CMP_OPERATOR

	
	template <typename FT, typename IT, usize Mbits, usize Ebits>
	struct Hash< TUFloat< FT, IT, Mbits, Ebits > > :
		private Hash< typename TUFloat<FT,IT,Mbits,Ebits>::int_t >
	{
		typedef TUFloat< FT, IT, Mbits, Ebits >		key_t;
		typedef Hash< typename key_t::int_t >		base_t;
		typedef typename base_t::result_t			result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x.GetInt() );
		}
	};
	
	/*template <>
	struct ::GX_STL::CompileTime::TypeInfo< float8u_t >
	{
		typedef float8u_t	type;
		typedef type		inner_type;
		
		template <typename OtherType>
		using CreateWith = OtherType;

		enum {
			FLAGS	= (int)_ctime_hidden_::FLOAT | (int)_ctime_hidden_::UNSIGNED | (int)_ctime_hidden_::SOFTWARE,
		};

		static			 type			Max()		{ return (type) _ctime_hidden_::FLOAT_MAX; }
		static			 type			Min()		{ return (type) _ctime_hidden_::FLOAT_MIN; }
		static			 type			Inf()		{ return type(0); }	// TODO
		static			 type			NaN()		{ return type(0); }	// TODO
		
		static			 type			Epsilon()	{ return (type) _ctime_hidden_::FLOAT_EPSILON; }
		static constexpr uint			SignBit()	{ return uint(-1); }
	};*/

}	// GXTypes
}	// GX_STL
