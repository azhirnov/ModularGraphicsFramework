// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Types.h"
#include "AllFunc.h"

namespace GX_STL
{
namespace GXTypes
{
	
#	define _FLOAT_OPERATOR_E( _op_, _ret0_, _func0_, _ret1_, _func1_, _ret2_, _func2_, _ret3_, _func3_ ) \
		template <typename T>		 _ret0_ operator _op_##= (const T& right)					_func0_ \
									 _ret1_ operator _op_    (const Self& right) const			_func1_ \
		template <typename T>		 _ret2_ operator _op_    (const T& right) const				_func2_ \
		template <typename T> friend _ret3_ operator _op_    (const T& left, const Self& right) _func3_ \

#	define _FLOAT_OPERATOR( _op_ ) \
		_FLOAT_OPERATOR_E( _op_, \
				Self &,	{ _f _op_##= FT(right);  return *this; }, \
				Self,	{ return _f _op_ right._f; }, \
				Self,	{ return _f _op_ FT(right); }, \
				T,		{ return left _op_ (T)right; } ) \
				
#	define _FLOAT_OPERATOR_B( _op_ ) \
		_FLOAT_OPERATOR_E( _op_, \
				Self &,	{ _i _op_##= IT(right);  return *this; }, \
				Self,	{ return Self(*this) _op_ right._i; }, \
				Self,	{ return Self(*this) _op_ right; }, \
				T,		{ return left _op_ T(right._i); } ) \



	//
	// Floating Point Type
	//

	template <typename FT, typename IT, typename UT, usize Mbits, usize Ebits>
	struct TFloat : public CompileTime::PODType
	{
		// types
		typedef TFloat<FT,IT,UT,Mbits,Ebits>	Self;
		typedef IT								Int_t;
		typedef UT								UInt_t;
		typedef FT								Float_t;

		struct TFloatBits
		{
			UT	m	: Mbits;	// mantissa bits
			UT	e	: Ebits;	// exponent bits
			UT	s	: 1;		// sign bit
		};


		// variables
		union {
			TFloatBits	_bits;
			FT			_f;
			IT			_i;
		};


		// constructors
		TFloat (): _f(FT(0))								{}
		TFloat (const Self &f): _f(f._f)					{}

		template <typename B>
		explicit
		TFloat (const B& val): _f(FT(val))					{}
	

		// type cast
		operator FT & ()									{ return _f; }
		operator const FT () const							{ return _f; }
	

		// unary operators
		Self &	operator ++ ()								{ ++_f; return *this; }
		Self &	operator -- ()								{ --_f; return *this; }
		Self	operator ++ (int) 							{ Self temp(*this);  ++_f;  return temp; }
		Self	operator -- (int) 							{ Self temp(*this);  --_f;  return temp; }
		IT		operator ~ ()	const						{ return ~_i; }
		bool	operator ! ()	const						{ return not _f; }


		// binary operators
#	if 0
		template <typename T>	bool	operator ==  (const T& right) const	{ return Equal( FT(right) ); }
		template <typename T>	bool	operator >=  (const T& right) const	{ const FT f(right);  return _f > f ||  Equal(f); }
		template <typename T>	bool	operator >   (const T& right) const	{ const FT f(right);  return _f > f and not Equal(f); }
		template <typename T>	bool	operator <=  (const T& right) const	{ const FT f(right);  return _f < f ||  Equal(f); }
		template <typename T>	bool	operator <   (const T& right) const	{ const FT f(right);  return _f < f and not Equal(f); }
#	endif

		
		_FLOAT_OPERATOR( + );
		_FLOAT_OPERATOR( - );
		_FLOAT_OPERATOR( * );
		_FLOAT_OPERATOR( / );
		_FLOAT_OPERATOR_E( %,
				Self&,	{ _f = ::fmod( _f, right );  return *this; },
				Self,	{ return ::fmod( _f, right._f ); },
				Self,	{ return ::fmod( _f, right ); },
				T,		{ return ::fmod( left, (T)right ); }
		);
		_FLOAT_OPERATOR_B( |  );
		_FLOAT_OPERATOR_B( &  );
		_FLOAT_OPERATOR_B( ^  );
		_FLOAT_OPERATOR_E( >>,
				Self&,	{ return FastPow2( -right ); },
				Self,	{ return Self(*this) >>= right._i; },
				Self,	{ return Self(*this) >>= right; },
				T,		{ return left >> (T)right; }
		);
		_FLOAT_OPERATOR_E( <<,
				Self&,	{ return FastPow2( right ); },
				Self,	{ return Self(*this) <<= right._i; },
				Self,	{ return Self(*this) <<= right; },
				T,		{ return left << (T)right; }
		);

		Self &	operator |=  (const Self& right)		{ _i |= right._i;  return *this; }
		Self &	operator &=  (const Self& right)		{ _i &= right._i;  return *this; }
		Self &	operator ^=  (const Self& right)		{ _i ^= right._i;  return *this; }


		// methods
		static usize	NumMantissaBits ()				{ return Mbits; }
		static usize	NumExponentBits ()				{ return Ebits; }

		static Self		FromBits (TFloatBits bits)		{ Self res;  res._bits = bits;  return res; }

		void	SetF (const FT &f)						{ _f = f; }
		void	SetI (const IT &i)						{ _i = i; }
		FT		GetF ()				const				{ return _f; }
		IT		GetI ()				const				{ return _i; }
		FT &	GetFRef ()								{ return _f; }
		IT &	GetIRef ()								{ return _i; }
		IT		GetMantissaBit ()	const				{ return _bits.m; }
		IT		GetExponentBit ()	const				{ return IT(_bits.e) - (1<<(Ebits-1)) + 1; }
		byte	GetSign ()			const				{ return _bits.s ? -1 : 1; }

		FT		GetMantissa ()		const				{ return FT(1) + FT(_bits.m) / FT( (1<<Mbits)-1 ); }
		FT		GetExponent ()		const;

		Self &	FastPow2 (int off)						{ _bits.e += off;  return *this; }

		
		// checks
		STATIC_ASSERT( sizeof(FT)  == sizeof(IT), "types size mismatch" );
	};
	

	// types
	typedef TFloat< float,  int,   uint,  23,  8 >		Float_t;
	typedef TFloat< double, ilong, ulong, 52, 11 >		Double_t;


	template <typename T>	struct TFloatSimilar;
	template <>				struct TFloatSimilar<float>		{ typedef Float_t		type; };
	template <>				struct TFloatSimilar<double>	{ typedef Double_t		type; };
	template <>				struct TFloatSimilar<Float_t>	{ typedef Float_t		type; };
	template <>				struct TFloatSimilar<Double_t>	{ typedef Double_t		type; };


/*
=================================================
	GetExponent
=================================================
*/
	template <typename FT, typename IT, typename UT, usize Mbits, usize Ebits>
	inline FT TFloat<FT,IT,UT,Mbits,Ebits>::GetExponent () const
	{
		IT	p = GetExponentBit();

		if ( p >= 0 )
			return FT( IT(1) << p );
		else
			return FT(1) / FT( IT(1) << (-p) ); 
	}


#	undef _FLOAT_OPERATOR

	
	template <typename FT, typename IT, typename UT, usize Mbits, usize Ebits>
	struct Hash< TFloat< FT, IT, UT, Mbits, Ebits > > :
		private Hash< typename TFloat<FT,IT,UT,Mbits,Ebits>::Float_t >
	{
		typedef TFloat< FT, IT, UT, Mbits, Ebits >	key_t;
		typedef Hash< typename key_t::Float_t >		base_t;
		typedef typename base_t::result_t			result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x );
		}
	};

	
	/*GX_TYPE_INFO_WITH_INNER_TYPE( Float_t,	Float_t::Float_t,	"Float" );
	GX_TYPE_INFO_WITH_INNER_TYPE( Double_t,	Double_t::Float_t,	"Double" );*/

}	// GXMath
}	// GX_STL
