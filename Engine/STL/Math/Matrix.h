// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Trigonometry.h"
#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"

#undef MAT_BIN_OPERATOR_SCALAR
#undef MAT_BIN_OPERATOR_MAT
#undef MAT_BIN_OPERATOR

namespace GX_STL
{
namespace GXTypes
{
	
	template <typename T, usize C, usize R, ulong U>
	struct Hash< GXMath::Matrix<T,C,R,U> > :
		private Hash< ArrayCRef<T> >
	{
		typedef GXMath::Matrix<T,C,R,U>		Key_t;
		typedef Hash< ArrayCRef<T> >		Base_t;
		typedef typename Base_t::Result_t	Result_t;

		Result_t operator () (const Key_t &x) const noexcept
		{
			return Base_t::operator ()( Base_t::Key_t( x.ptr(), x.Count() ) );
		}
	};

}	// GXTypes

namespace CompileTime
{

	//
	// TypeInfo
	//
	template <typename T, usize C, usize R, ulong U>
	struct TypeInfo < GXMath::Matrix<T,C,R,U> >
	{
	private:
		typedef CompileTime::TypeInfo<T>	_value_type_info;

	public:
		typedef GXMath::Matrix<T,C,R,U>	type;
		typedef T						inner_type;
		
		template <typename OtherType>
		using CreateWith =  GXMath::Matrix< OtherType, C, R >;

		enum {
			FLAGS	= (int)_value_type_info::FLAGS | (int)GX_STL::CompileTime::_ctime_hidden_::VECTOR,
		};

		static constexpr type	Max()		{ return type( _value_type_info::Max() ); }
		static constexpr type	Min()		{ return type( _value_type_info::Min() ); }
		static			 type	Inf()		{ return type( _value_type_info::Inf() ); }
		static			 type	NaN()		{ return type( _value_type_info::NaN() ); }
		
		static constexpr type	Epsilon()	{ return type( _value_type_info::Epsilon() ); }
		static constexpr uint	SignBit()	{ return _value_type_info::SignBit(); }
		static constexpr uint	Count()		{ return C * R * _value_type_info::Count(); }
	};

}	// CompileTime
}	// GX_STL
