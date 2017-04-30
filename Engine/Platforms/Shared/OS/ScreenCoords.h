// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace ScreenCoords
{
	using namespace GX_STL::GXMath;


	//
	// Pixels
	//

	struct Pixels
	{
	// types
	public:
		using vec_t		= int2;
		using value_t	= vec_t::value_t;


	// variables
	public:
		value_t		x, y;


	// methods
	public:
		Pixels () : x(0), y(0) {}

		explicit
		Pixels (const vec_t &p) : x(p.x), y(p.y) {}

		Pixels (value_t X, value_t Y) : x(X), y(Y) {}

		const vec_t &	Get ()	const	{ return ReferenceCast< vec_t >( *this ); }
	};



	//
	// Dip
	//

	struct Dip
	{
	// types
	public:
		using vec_t		= float2;
		using value_t	= vec_t::value_t;


	// variables
	public:
		value_t		x, y;


	// methods
	public:
		Dip () : x(0), y(0) {}

		explicit
		Dip (const vec_t &d) : x(d.x), y(d.y) {}

		Dip (value_t X, value_t Y) : x(X), y(Y) {}
			
		const vec_t &	Get ()	const	{ return ReferenceCast< vec_t >( *this ); }
	};



	//
	// Physics (mm)
	//

	struct Physics
	{
	// types
	public:
		using vec_t		= float2;
		using value_t	= vec_t::value_t;


	// variables
	public:
		value_t		x, y;


	// methods
	public:
		Physics () : x(0), y(0) {}

		explicit
		Physics (const vec_t &p) : x(p.x), y(p.y) {}

		Physics (value_t X, value_t Y) : x(X), y(Y) {}
			
		const vec_t &	Get ()		const	{ return ReferenceCast< vec_t >( *this ); }

		vec_t			ToMeters () const	{ return Get() * 1000.0f; }
	};



	struct UNorm;
	struct SNorm;


	//
	// Unsigned Normalized
	//

	struct UNorm
	{
	// types
	public:
		using vec_t		= float2;
		using value_t	= vec_t::value_t;


	// variables
	public:
		value_t		x, y;


	// methods
	public:
		UNorm () : x(0), y(0) {}

		explicit
		UNorm (const vec_t &v) : x(v.x), y(v.y) {}

		UNorm (value_t x, value_t y) : x(x), y(y) {}

		explicit
		UNorm (const SNorm &n);
			
		const vec_t &	Get ()	const	{ return ReferenceCast< vec_t >( *this ); }
	};



	//
	// Signed Normalized
	//

	struct SNorm
	{
	// types
	public:
		using vec_t		= float2;
		using value_t	= vec_t::value_t;


	// variables
	public:
		value_t		x, y;


	// methods
	public:
		SNorm () : x(0), y(0) {}

		explicit
		SNorm (const vec_t &v) : x(v.x), y(v.y) {}

		SNorm (value_t X, value_t Y) : x(X), y(Y) {}

		explicit
		SNorm (const UNorm &n) : x( (n.x * 2.0f) - 1.0f ), y( (n.y * 2.0f) - 1.0f ) {}
			
		const vec_t &	Get ()	const	{ return ReferenceCast< vec_t >( *this ); }
	};
		

	inline UNorm::UNorm (const SNorm &n) :
		x( (n.x - 1.0f) * 0.5f ), y( (n.y - 1.0f) * 0.5f )
	{}


}	// ScreenCoords
}	// Engine
