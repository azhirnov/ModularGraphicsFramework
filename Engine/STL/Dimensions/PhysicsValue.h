// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "PhysicsValueUtils.h"

#ifdef GX_PHYSICS_DIMENSIONS_ENABLED

namespace GX_STL
{
namespace GXMath
{

	//
	// Physics Value
	//

	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale
			 >
	struct PhysicsValue : public CompileTime::CopyQualifiers< ValueType >
	{
		STATIC_ASSERT( CompileTime::IsFloat<ValueType> and
					   CompileTime::IsScalar<ValueType> );

	// types
	public:	
		typedef ValueType												value_t;
		typedef Dimensions												dimensions_t;	// PhysDimList
		typedef ValueScale												value_scale_t;	// PhysicsDimensionScale::BaseConversion
		typedef PhysicsValue< value_t, dimensions_t, value_scale_t >	Self;

	private:
		template <typename D, typename S>
		struct _Add {
			typedef PhysicsValue< value_t, D, S >											Right;
			typedef typename value_scale_t::template _Add4< typename Right::value_scale_t >	conv_add_op_t;
			typedef PhysicsValue< value_t,
						typename dimensions_t::template Add< Right::dimensions_t >,
						typename conv_add_op_t::type >										type;

			static value_t Get (const value_t &left, const value_t &right) {
				return conv_add_op_t::Get( left, right );
			}
		};
		
		template <typename D, typename S>
		struct _Sub {
			typedef PhysicsValue< value_t, D, S >											Right;
			typedef typename value_scale_t::template _Sub4< typename Right::value_scale_t >	conv_sub_op_t;
			typedef PhysicsValue< value_t,
						typename dimensions_t::template Sub< Right::dimensions_t >,
						typename conv_sub_op_t::type >										type;

			static value_t Get (const value_t &left, const value_t &right) {
				return conv_sub_op_t::Get( left, right );
			}
		};
		
		template <typename D, typename S>
		struct _Mul {
			typedef PhysicsValue< value_t, D, S >											Right;
			typedef typename value_scale_t::template _Mul4< typename Right::value_scale_t >	conv_mul_op_t;
			typedef PhysicsValue< value_t,
						typename dimensions_t::template Mul< Right::dimensions_t >,
						typename conv_mul_op_t::type >										type;

			static value_t Get (const value_t &left, const value_t &right) {
				return conv_mul_op_t::Get( left, right );
			}
		};
		
		template <typename D, typename S>
		struct _Div {
			typedef PhysicsValue< value_t, D, S >											Right;
			typedef typename value_scale_t::template _Div4< typename Right::value_scale_t >	conv_div_op_t;
			typedef PhysicsValue< value_t,
						typename dimensions_t::template Div< Right::dimensions_t >,
						typename conv_div_op_t::type >										type;

			static value_t Get (const value_t &left, const value_t &right) {
				return conv_div_op_t::Get( left, right );
			}
		};
		
		template <isize PowNum, isize PowDenom = 1>
		struct _Pow {
			typedef typename CompileTime::Fractional32< PowNum, PowDenom >		pow_t;
			typedef PhysicsValue< value_t,
						typename dimensions_t::template Power< pow_t >,
						typename value_scale_t::template Pow< pow_t > >			type;
		};

		template <typename NewValueType>
		struct _NewConv {
			typedef typename PhysicsDimensionScale< NewValueType >::template 
						BaseConversion< value_scale_t::Scale::MANTISSA, value_scale_t::Scale::EXPONENT,
										value_scale_t::Power::N, value_scale_t::Power::D >	type;
		};

		typedef PhysicsValue< ValueType, typename dimensions_t::Inverse,
							  typename value_scale_t::Inverse >				SelfInversed;


	// variables
	private:
		value_t		_value;


	// methods
	public:
		constexpr PhysicsValue (GX_DEFCTOR) : _value(0) {}
	
		explicit
		constexpr PhysicsValue (value_t value) : _value(value) {}

		constexpr PhysicsValue (const Self &other) : _value(other.ref()) {}
		
		template <typename S>
		PhysicsValue (const PhysicsValue< value_t, dimensions_t, S > &other) :
			_value(other.ToScale< value_scale_t >().ref())
		{}

		value_t &		ref ()							{ return _value; }
		value_t const &	ref ()			const			{ return _value; }

		value_t			Get ()			const			{ return value_scale_t::Func::Get( _value ); }

		Self	operator -  ()			const			{ return Self( -_value ); }

		Self &	operator =  (const Self &right)			{ _value = right.ref();  return *this; }

		bool	IsZero ()						const	{ return GXMath::IsZero( _value ); }

		bool	operator == (const Self &right)	const	{ return ( _value == right.ref() ); }
		bool	operator != (const Self &right)	const	{ return not ( *this == right ); }
		bool	operator >  (const Self &right)	const	{ return _value >  right.ref(); }
		bool	operator >= (const Self &right)	const	{ return _value >= right.ref(); }
		bool	operator <  (const Self &right)	const	{ return _value <  right.ref(); }
		bool	operator <= (const Self &right)	const	{ return _value <= right.ref(); }

		Self &	operator += (const Self &right)			{ _value += right.ref();  return *this; }
		Self &	operator -= (const Self &right)			{ _value -= right.ref();  return *this; }

		Self	operator +  (const Self &right)	const	{ return Self( _value + right.ref() ); }
		Self	operator -  (const Self &right)	const	{ return Self( _value - right.ref() ); }

		Self &	operator *= (value_t right)				{ _value *= right;  return *this; }
		Self &	operator /= (value_t right)				{ _value /= right;  return *this; }

		Self	operator *  (value_t right)		const	{ return Self( _value * right ); }
		Self	operator /  (value_t right)		const	{ return Self( _value / right ); }

		
		template <typename D, typename S>
		typename _Add< D, S >::type  operator +  (const PhysicsValue<value_t,D,S> &right) const
		{
			typedef _Add< D, S >			add_op;
			typedef typename add_op::type	result_t;

			return result_t( add_op::Get( _value, right.ref() ) );
		}
		
		template <typename D, typename S>
		typename _Sub< D, S >::type  operator -  (const PhysicsValue<value_t,D,S> &right) const
		{
			typedef _Sub< D, S >			sub_op;
			typedef typename sub_op::type	result_t;

			return result_t( sub_op::Get( _value, right.ref() ) );
		}
		
		template <typename D, typename S>
		typename _Mul< D, S >::type  operator *  (const PhysicsValue<value_t,D,S> &right) const
		{
			typedef _Mul< D, S >			mul_op;
			typedef typename mul_op::type	result_t;

			return result_t( mul_op::Get( _value, right.ref() ) );
		}
		
		template <typename D, typename S>
		typename _Div< D, S >::type  operator /  (const PhysicsValue<value_t,D,S> &right) const
		{
			typedef _Div< D, S >			div_op;
			typedef typename div_op::type	result_t;

			return result_t( div_op::Get( _value, right.ref() ) );
		}


		friend Self	operator * (value_t left, const Self &right)
		{
			return Self( left * right.ref() );
		}
	

		friend SelfInversed	operator / (value_t left, const Self &right)
		{
			return SelfInversed( left / right.ref() );
		}

		
		template <isize PowNum, isize PowDenom>
		typename _Pow< PowNum, PowDenom >::type  Pow () const
		{
			typedef typename _Pow< PowNum, PowDenom >::type						result_t;
			typedef typename CompileTime::Fractional32< PowNum, PowDenom >		pow_t;
			typedef typename CompileTime::NearFloat::FromType<ValueType>		float_t;

			return result_t( GXMath::Pow( _value, pow_t::ToFloat< float_t >() ) );
		}
		

		template <isize Power>
		typename _Pow< Power >::type  Pow () const
		{
			typedef typename CompileTime::NearFloat::FromType<ValueType>		float_t;

			return typename _Pow< Power >::type( GXMath::Pow( _value, float_t( Power ) ) );
		}


		typename _Pow< 2 >::type  Square () const
		{
			return Pow< 2 >();
		}


		typename _Pow< 1, 2 >::type  Sqrt () const
		{
			return Pow< 1, 2 >();
		}


		SelfInversed  Inverse () const
		{
			return value_t(1) / (*this);
		}


		Self &	SetMax (const Self &right)
		{
			_value = Max( _value, right._value );
			return *this;
		}


		Self &	SetMin (const Self &right)
		{
			_value = Min( _value, right._value );
			return *this;
		}


		String ToString () const
		{
			return String().FormatF( Get() ) << '[' << dimensions_t::ToString("*") << ']';
		}


		String ToDebugString () const
		{
			return String().FormatF( ref() ) << " * " << value_scale_t::ToString() << " [" << dimensions_t::ToString("*") << ']';
		}


		template <typename T>
		PhysicsValue< T, dimensions_t, typename _NewConv<T>::type >  Convert () const
		{
			return PhysicsValue< T, dimensions_t, typename _NewConv<T>::type >( T( _value ) );
		}


		template <typename T>
		T	To () const
		{
			STATIC_ASSERT( dimensions_t::Equal< typename T::dimensions_t >::value );

			typedef typename CompileTime::MainType< value_t, typename T::value_t >		main_value_t;
			typedef typename value_scale_t::To< main_value_t >							scale1_t;
			typedef typename T::value_scale_t::To< main_value_t >						scale2_t;
			typedef typename scale1_t::_Div4< scale2_t >								div_op_t;

			return T( (T::value_t) div_op_t::Get( _value, main_value_t(1) ) );
		}


		template <typename ToValueScale>
		PhysicsValue< value_t, dimensions_t, ToValueScale >	ToScale () const
		{
			return PhysicsValue< value_t, dimensions_t, ToValueScale >(
						Get() / ToValueScale::Func::Get( value_t(1) ) );
		}
	};
	


	//
	// Non-Dimensional Physics Value
	//

	template <typename ValueType,
			  typename ValueScale
			 >
	struct PhysicsValue< ValueType, DefaultPhysicsDimensionsList::CreateNonDimensional, ValueScale > :
		public CompileTime::CopyQualifiers< ValueType >
	{
	// types
	public:
		typedef ValueType													value_t;
		typedef DefaultPhysicsDimensionsList::CreateNonDimensional			dimensions_t;
		typedef ValueScale													value_scale_t;
		typedef PhysicsValue< value_t, dimensions_t, value_scale_t >		Self;
		
		STATIC_ASSERT( dimensions_t::IsNonDimensional::value );

	private:
		typedef PhysicsValue< ValueType, typename dimensions_t::Inverse,
							  typename value_scale_t::Inverse >				SelfInversed;


	// variables
	private:
		value_t		_value;


	// methods
	public:
		constexpr PhysicsValue (GX_DEFCTOR) : _value(0) {}
	
		explicit
		constexpr PhysicsValue (value_t value) : _value(value) {}

		constexpr PhysicsValue (const Self &other) : _value(other.ref()) {}
		

		value_t &		ref ()							{ return _value; }
		value_t const &	ref ()			const			{ return _value; }

		value_t			Get ()			const			{ return value_scale_t::Func::Get( _value ); }

		operator value_t ()				const			{ return Get(); }

		Self &	operator =  (const Self &right)			{ _value = right.ref();  return *this; }

		bool	operator == (const Self &right)	const	{ return ( _value == right.ref() ); }
		bool	operator != (const Self &right)	const	{ return not ( *this == right ); }
		bool	operator >  (const Self &right)	const	{ return _value >  right.ref(); }
		bool	operator >= (const Self &right)	const	{ return _value >= right.ref(); }
		bool	operator <  (const Self &right)	const	{ return _value <  right.ref(); }
		bool	operator <= (const Self &right)	const	{ return _value <= right.ref(); }

		Self &	operator += (const Self &right)			{ _value += right.ref();  return *this; }
		Self &	operator -= (const Self &right)			{ _value -= right.ref();  return *this; }
		
		Self	operator +  (const Self &right)	const	{ return Self( _value + right.ref() ); }
		Self	operator -  (const Self &right)	const	{ return Self( _value - right.ref() ); }

		Self &	operator *= (value_t right)				{ _value *= right;  return *this; }
		Self &	operator /= (value_t right)				{ _value /= right;  return *this; }
		
		Self	operator *  (value_t right)		const	{ return Self( _value * right ); }
		Self	operator /  (value_t right)		const	{ return Self( _value / right ); }


		friend Self	operator * (value_t left, const Self &right)
		{
			return Self( left * right.ref() );
		}
	
		friend SelfInversed	operator / (value_t left, const Self &right)
		{
			return SelfInversed( left / right.ref() );
		}


		template <isize PowNum, isize PowDenom>
		value_t  Pow () const
		{
			typedef typename CompileTime::Fractional32< PowNum, PowDenom >		pow_t;
			typedef typename CompileTime::NearFloat::FromType< value_t >		float_t;

			return GXMath::Pow( Get(), pow_t::ToFloat< float_t >() );
		}
		

		template <isize Power>
		value_t  Pow () const
		{
			return GXMath::Pow< Power >( Get() );
		}


		value_t  Square () const
		{
			return GXMath::Square( Get() );
		}


		value_t  Sqrt () const
		{
			return GXMath::Sqrt( Get() );
		}


		SelfInversed  Inverse () const
		{
			return SelfInversed( value_t(1) / _value );
		}


		String ToString () const
		{
			return String().FormatF( Get() ) << " []";
		}


		String ToDebugString () const
		{
			return String().FormatF( ref() ) << " * (" << value_scale_t::Scale::ToString()
					<< ")^" << value_scale_t::Power::ToString() << " []";
		}
	};


	
	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale
			 >
	struct ::GX_STL::CompileTime::TypeInfo < GXMath::PhysicsValue< ValueType, Dimensions, ValueScale > >
	{
	private:
		typedef CompileTime::TypeInfo<ValueType>	_value_type_info;

	public:
		typedef GXMath::PhysicsValue< ValueType, Dimensions, ValueScale >	type;
		typedef ValueType													inner_type;
		
		template <typename OtherType>
		using CreateWith =  GXMath::PhysicsValue< OtherType, Dimensions, ValueScale >;

		enum {
			FLAGS	= (int)_value_type_info::FLAGS | (int)GX_STL::CompileTime::_ctime_hidden_::WRAPPER,
		};

		static constexpr type	Max()		{ return type( _value_type_info::Max() ); }
		static constexpr type	Min()		{ return type( _value_type_info::Min() ); }
		static			 type	Inf()		{ return type( _value_type_info::Inf() ); }
		static			 type	NaN()		{ return type( _value_type_info::NaN() ); }
		
		static constexpr type	Epsilon()	{ return type( _value_type_info::Epsilon() ); }
		static constexpr uint	SignBit()	{ return _value_type_info::SignBit(); }
		static constexpr uint	Count()		{ return _value_type_info::Count(); }
	};


	template <typename ValueType,
			  typename Dimensions,
			  typename ValueScale
			 >
	struct ::GX_STL::GXTypes::Hash< GXMath::PhysicsValue< ValueType, Dimensions, ValueScale > > : private Hash< ValueType >
	{
		typedef GXMath::PhysicsValue< ValueType, Dimensions, ValueScale >	key_t;
		typedef Hash< ValueType >											base_t;
		typedef typename base_t::result_t									result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x.ref() );
		}
	};

}	// GXMath
}	// GX_STL

#endif	// GX_PHYSICS_DIMENSIONS_ENABLED
