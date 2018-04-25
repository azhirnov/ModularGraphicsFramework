// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Common.h"

namespace CodeGen
{

	//
	// Scalar
	//

	template <typename FT, typename IT>
	struct Scalar final : CompileTime::PODStruct
	{
		STATIC_ASSERT( sizeof(FT) == sizeof(IT) );

	// types
	public:
		using Self	= Scalar< FT, IT >;

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
		Scalar () : _f{ 0.0f }, _type{EType_Float} {}


	};


	using ScalarF	= Scalar< float, int >;
	using ScalarD	= Scalar< double, ilong >;


}	// CodeGen
