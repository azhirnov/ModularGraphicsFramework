// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Types.h"
#include "AllFunc.h"
#include "Engine/STL/CompileTime/TemplateUtils.h"

namespace GX_STL
{
namespace GXTypes
{

#	define _FIXED_OPERATOR_E( _op_, _ret1_, _func1_, _ret2_, _func2_, _ret3_, _func3_ ) \
									 _ret1_ operator _op_ (const Self& right) const				_func1_ \
		template <typename T>		 _ret2_ operator _op_ (const T& right) const				_func2_ \
		template <typename T> friend _ret3_ operator _op_ (const T& left, const Self& right)	_func3_ \

#	define _FIXED_OPERATOR( _op_ ) \
		_FIXED_OPERATOR_E( _op_, \
				Self,	{ return Self(*this) _op_##= right; }, \
				Self,	{ return Self(*this) _op_##= right; }, \
				T,		{ return left _op_ (T)right; } ) \

#	define _FIXED_CMP_OPERATOR( _op_ ) \
				bool operator _op_ (const int& right) const					{ return GetInt() _op_ right; } \
				bool operator _op_ (const float_t& right) const				{ return GetFloat() _op_ right; } \
		friend	bool operator _op_ (const int& left, const Self& right)		{ return left _op_ right.GetInt(); } \
		friend	bool operator _op_ (const float_t& left, const Self& right)	{ return left _op_ right.GetFloat(); } \



	//
	// Fixed Point Value
	//

	template <typename I, typename U, typename IT, typename FT>
	struct TFixed : public CompileTime::PODType
	{
	// types
	public:
		typedef TFixed<I,U,IT,FT>	Self;
		typedef I					half_int_t;
		typedef U					half_uint_t;
		typedef IT					int_t;
		typedef FT					float_t;


	// variables
	private:
		union {
			struct {
				half_int_t		_int;
				half_uint_t		_frac;
			};
			int_t				_value;
		};

		static const uint		_s_uFracRoundOffset = CompileTime::SizeOf<U>::bits - 1;


	// methods
	public:

		// constructors
		TFixed () : _int(0), _frac(0)							{}
		TFixed (const Self &f) : _int(f._int), _frac(f._frac)	{}
		TFixed (const I& A, const U& B) : _int(A), _frac(B)		{}

		explicit TFixed (const int &val)						{ Set( val ); }
		explicit TFixed (const float &val)						{ Set( val ); }
		explicit TFixed (const double &val)						{ Set( val ); }
	

		// set/get
		int_t	GetU () const								{ return _value; }
		int_t &	GetU ()										{ return _value; }
		int		GetInt () const								{ return _int + (_frac>>_s_uFracRoundOffset); }
		float_t	GetFloat () const;
		void	Set (const int &val);
		void	Set (const float &val);
		void	Set (const double &val);


		// type cast
		operator int () const								{ return GetInt(); }
		operator uint () const								{ return GetInt(); }
		operator float_t () const							{ return GetFloat(); }


		// unary operators
		Self &	operator ++ ()								{ ++_int;  return *this; }
		Self &	operator -- ()								{ --_int;  return *this; }
		Self	operator ++ (int) 							{ Self ret(*this);  ++_int;  return ret; }
		Self	operator -- (int)							{ Self ret(*this);  --_int;  return ret; }
		bool	operator !  () const						{ return not GetInt(); }
		Self	operator +  () const						{ return Self( +_int, _frac ); }
		Self	operator -  () const						{ return Self( -_int, _frac ); }
		Self	operator ~  () const						{ Self  ret;  ret._value = ~_value;  return ret; }
	

		// binary operators
		bool	operator == (const Self& right) const		{ return _value == right._value; }
		bool	operator != (const Self& right) const		{ return _value != right._value; }
		bool	operator >  (const Self& right) const		{ return (_int > right._int) or (_int == right._int and ((_int >= 0 and _frac > right._frac) or (_int < 0 and _frac < right._frac))) ; }
		bool	operator >= (const Self& right) const		{ return ( *this > right or *this == right ); }
		bool	operator <  (const Self& right) const		{ return not ( *this >= right ); }
		bool	operator <= (const Self& right) const		{ return not ( *this > right ); }

		Self &	operator =  (const int& right)				{ Set( right );				return *this; }
		Self &	operator += (const int& right)				{ Set( GetInt() + right );  return *this; }
		Self &	operator -= (const int& right)				{ Set( GetInt() - right );  return *this; }
		
		Self &	operator =  (const float_t& right)			{ Set( right );				  return *this; }
		Self &	operator += (const float_t& right)			{ Set( GetFloat() + right );  return *this; }
		Self &	operator -= (const float_t& right)			{ Set( GetFloat() - right );  return *this; }
		
		Self &	operator =  (const Self& right)				{ _value = right._value;	  return *this; }
		Self &	operator += (const Self& right)				{ Set( GetFloat() + right );  return *this; }
		Self &	operator -= (const Self& right)				{ Set( GetFloat() - right );  return *this; }

		template <typename T>	Self& operator *=  (const T& right)			{ Set( GetFloat() * float_t(right) );  return *this; }
		template <typename T>	Self& operator /=  (const T& right)			{ Set( GetFloat() / float_t(right) );  return *this; }
		template <typename T>	Self& operator %=  (const T& right)			{ Set( ::fmod( GetFloat(), float_t(right) ) );  return *this; }
		
		template <typename T>	Self&	operator |=  (const T& right)		{ _value |= int_t(right);  return *this; }
		template <typename T>	Self&	operator &=  (const T& right)		{ _value &= int_t(right);  return *this; }
		template <typename T>	Self&	operator ^=  (const T& right)		{ _value ^= int_t(right);  return *this; }
		/*template <>*/			Self&	operator |=  (const Self& right)	{ _value |= right._value;  return *this; }
		/*template <>*/			Self&	operator &=  (const Self& right)	{ _value &= right._value;  return *this; }
		/*template <>*/			Self&	operator ^=  (const Self& right)	{ _value ^= right._value;  return *this; }

		template <typename T>	Self&	operator >>= (const T& right)		{ _int >>= int(right);  return *this; }
		template <typename T>	Self&	operator <<= (const T& right)		{ _int <<= int(right);  return *this; }


		_FIXED_OPERATOR( + );
		_FIXED_OPERATOR( - );
		_FIXED_OPERATOR( * );
		_FIXED_OPERATOR( / );
		_FIXED_OPERATOR_E( %,
				Self,	{ return ::fmod( GetFloat(), float_t(right) ); },
				Self,	{ return ::fmod( GetFloat(), right ); },
				T,		{ return ::fmod( left, T(right) ); }
		);
		_FIXED_OPERATOR( | );
		_FIXED_OPERATOR( & );
		_FIXED_OPERATOR_E( ^,
				Self,	{ return Self(*this) ^= right; },
				Self,	{ return Self(*this) ^= right; },
				T,		{ return left ^ T(right); }
		);
		_FIXED_OPERATOR_E( >>,
				Self,	{ return Self(*this) >>= right; },
				Self,	{ return Self(*this) >>= right; },
				T,		{ return left >> T(right); }
		);
		_FIXED_OPERATOR_E( <<,
				Self,	{ return Self(*this) <<= right; },
				Self,	{ return Self(*this) <<= right; },
				T,		{ return left << T(right); }
		);
		_FIXED_CMP_OPERATOR( == );
		_FIXED_CMP_OPERATOR( != );
		_FIXED_CMP_OPERATOR( >= );
		_FIXED_CMP_OPERATOR( >  );
		_FIXED_CMP_OPERATOR( <= );
		_FIXED_CMP_OPERATOR( <  );


		// checks //
		STATIC_ASSERT( sizeof(IT) == 2*sizeof(I) and sizeof(I) == sizeof(U), "incorrect types" );
	};


	typedef TFixed< int, uint, ilong, double >			Fixed64_t;
	typedef TFixed< short, ushort, int, float >			Fixed32_t;
	typedef TFixed< byte,  ubyte,  short, float >		Fixed16_t;


	
	template <typename I, typename U, typename IT, typename FT>
	inline void TFixed<I,U,IT,FT>::Set (const int &val)
	{
		_int  = half_int_t(val);
		_frac = 0;
	}

	
	template <typename I, typename U, typename IT, typename FT>
	inline void TFixed<I,U,IT,FT>::Set (const float &val)
	{
		float const	f = GXMath::Floor(val);
		float const d = GXMath::Abs(val) - GXMath::Abs(f);
	
		_int  = half_int_t ( f );
		_frac = half_uint_t( d * MaxValue<half_uint_t>() + FT(0.5) );
	}

	
	template <typename I, typename U, typename IT, typename FT>
	inline void TFixed<I,U,IT,FT>::Set (const double &val)
	{
		double const f = GXMath::Floor(val);
		double const d = GXMath::Abs(val) - GXMath::Abs(f);

		_int  = half_int_t( f );
		_frac = half_uint_t( d * MaxValue<half_uint_t>() + FT(0.5) );
	}
	
	
	template <typename I, typename U, typename IT, typename FT>
	inline FT TFixed<I,U,IT,FT>::GetFloat () const
	{
		return ( float_t(_int) + float_t(_frac) / float_t( MaxValue<half_uint_t>() ) );
	}
	
	
#	undef _FIXED_OPERATOR
#	undef _FIXED_OPERATOR_E
#	undef _FIXED_CMP_OPERATOR

	
	template <typename I, typename U, typename IT, typename FT>
	struct Hash< TFixed< I, U, IT, FT > > :
		private Hash< typename TFixed<I,U,IT,FT>::int_t >
	{
		typedef TFixed< I, U, IT, FT >			key_t;
		typedef Hash< typename key_t::int_t >	base_t;
		typedef typename base_t::result_t		result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x );
		}
	};
	
	/*template <>
	struct ::GX_STL::CompileTime::TypeInfo< Fixed16_t >
	{
		typedef Fixed16_t		type;
		typedef type			inner_type;
		
		template <typename OtherType>
		using CreateWith = OtherType;

		enum {
			FLAGS	= (int)_ctime_hidden_::FIXED | (int)_ctime_hidden_::SIGNED | (int)_ctime_hidden_::SOFTWARE,
		};

		static			 type	Max()		{ return type( TypeInfo< type::half_int_t >::Max(), TypeInfo< type::half_uint_t >::Max() ); }
		static			 type	Min()		{ return type( TypeInfo< type::half_int_t >::Min(), TypeInfo< type::half_uint_t >::Max() ); }
		
		static			 type	Epsilon()	{ return type( 0, 1 ); }
		static constexpr uint	SignBit()	{ return CompileTime::SizeOf<type>::bits-1; }
		static constexpr uint	Count()		{ return 1; }
	};


	template <>
	struct ::GX_STL::CompileTime::TypeInfo< Fixed32_t >
	{
		typedef Fixed32_t		type;
		typedef type			inner_type;
		
		template <typename OtherType>
		using CreateWith = OtherType;

		enum {
			FLAGS	= (int)_ctime_hidden_::FIXED | (int)_ctime_hidden_::SIGNED | (int)_ctime_hidden_::SOFTWARE,
		};

		static			 type	Max()		{ return type( TypeInfo< type::half_int_t >::Max(), TypeInfo< type::half_uint_t >::Max() ); }
		static			 type	Min()		{ return type( TypeInfo< type::half_int_t >::Min(), TypeInfo< type::half_uint_t >::Max() ); }
		
		static			 type	Epsilon()	{ return type( 0, 1 ); }
		static constexpr uint	SignBit()	{ return CompileTime::SizeOf<type>::bits-1; }
		static constexpr uint	Count()		{ return 1; }
	};


	template <>
	struct ::GX_STL::CompileTime::TypeInfo< Fixed64_t >
	{
		typedef Fixed64_t		type;
		typedef type			inner_type;
		
		template <typename OtherType>
		using CreateWith = OtherType;

		enum {
			FLAGS	= (int)_ctime_hidden_::FIXED | (int)_ctime_hidden_::SIGNED | (int)_ctime_hidden_::SOFTWARE,
		};

		static			 type	Max()		{ return type( TypeInfo< type::half_int_t >::Max(), TypeInfo< type::half_uint_t >::Max() ); }
		static			 type	Min()		{ return type( TypeInfo< type::half_int_t >::Min(), TypeInfo< type::half_uint_t >::Max() ); }
		
		static			 type	Epsilon()	{ return type( 0, 1 ); }
		static constexpr uint	SignBit()	{ return CompileTime::SizeOf<type>::bits-1; }
		static constexpr uint	Count()		{ return 1; }
	};*/

}	// GXTypes
}	// GX_STL
