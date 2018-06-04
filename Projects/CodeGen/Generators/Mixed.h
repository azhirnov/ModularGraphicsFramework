// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Common.h"

namespace CodeGen
{

	//
	// Mixed Type
	//

	template <typename FT, typename IT>
	struct Mixed final : CompileTime::PODStruct
	{
		STATIC_ASSERT( sizeof(FT) == sizeof(IT) );

	// types
	public:
		using Self	= Mixed< FT, IT >;

	private:
		enum EType {
			EType_Float	= 0,
			EType_Int	= 1,
		};


	// variables
	private:
		union {
			FT		_f;
			IT		_i;
		};
		EType		_type;


	// methods
	public:
		constexpr Mixed () : _f{ 0.0f }, _type{ EType_Float } {}

		explicit constexpr Mixed (float value) : _f{ FT(value) }, _type{ EType_Float } {}
		explicit constexpr Mixed (double value) : _f{ FT(value) }, _type{ EType_Float } {}

		explicit constexpr Mixed (int value) : _i{ IT(value) }, _type{ EType_Int } {}
		explicit constexpr Mixed (ilong value) : _i{ IT(value) }, _type{ EType_Int } {}

		Mixed (const Self &) = default;
		Mixed (Self &&) = default;

		Self& operator = (const Self &) = default;
		Self& operator = (Self &&) = default;

		bool operator == (const Self &right) const
		{
			return	_type == right._type and
					(_type == EType_Float ? _f == right._f :
					 _type == EType_Int   ? _i == right._i : false);
		}

		bool operator != (const Self &right) const
		{
			return not (*this == right);
		}

		bool operator > (const Self &right) const
		{
			return	_type != right._type	?	_type > right._type :
						(_type == EType_Float	?	_f > right._f	:
						 _type == EType_Int		?	_i > right._i	:	false);
		}
		
		bool operator < (const Self &right) const
		{
			return (right > *this);
		}

		bool operator >= (const Self &right) const
		{
			return not (*this < right);
		}

		bool operator <= (const Self &right) const
		{
			return not (*this > right);
		}

		bool		IsFloat ()	const	{ return _type == EType_Float; }
		bool		IsInt ()	const	{ return _type == EType_Int; }

		const FT	GetFloat ()	const	{ return _f; }
		const IT	GetInt ()	const	{ return _i; }
	};


	using MixedF	= Mixed< float, int >;
	using MixedD	= Mixed< double, ilong >;


}	// CodeGen

namespace GX_STL::GXTypes
{

	template <typename FT, typename IT>
	inline String ToString (const CodeGen::Mixed<FT,IT> &value)
	{
		if ( value.IsFloat() )
			return ToString( value.GetFloat() );

		if ( value.IsInt() )
			return ToString( value.GetInt() );

		return "";
	}

}	// GX_STL::GXTypes
