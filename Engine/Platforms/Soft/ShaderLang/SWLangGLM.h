// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/Impl/SWEnums.h"

#ifdef GRAPHICS_API_SOFT

#ifdef COMPILER_MSVC
# pragma warning (push, 1)
# pragma warning (disable: 4201)
#endif

#ifndef GX_ENABLE_GLM
# error GLM required to use C++ shaders
#endif

#define GLM_FORCE_SWIZZLE
//#define GLM_FORCE_CXX14
//#define GLM_FORCE_PURE
//#define GLM_FORCE_EXPLICIT_CTOR
#include "glm/glm/glm.hpp"

#include <atomic>
#include <type_traits>

namespace glm
{

	using Int		= int;
	using Int2		= ivec2;
	using Int3		= ivec3;
	using Int4		= ivec4;

	using UInt		= uint;
	using UInt2		= uvec2;
	using UInt3		= uvec3;
	using UInt4		= uvec4;

	using Float		= float;
	using Float2	= vec2;
	using Float3	= vec3;
	using Float4	= vec4;
	using Float2x2	= mat2x2;
	using Float2x3	= mat2x3;
	using Float2x4	= mat2x4;
	using Float3x2	= mat3x2;
	using Float3x3	= mat3x3;
	using Float3x4	= mat3x4;
	using Float4x2	= mat4x2;
	using Float4x3	= mat4x3;
	using Float4x4	= mat4x4;

	using Long		= int64_t;
	using Long2		= tvec2<Long, highp>;
	using Long3		= tvec3<Long, highp>;
	using Long4		= tvec4<Long, highp>;

	using ULong		= uint64_t;
	using ULong2	= tvec2<ULong, highp>;
	using ULong3	= tvec3<ULong, highp>;
	using ULong4	= tvec4<ULong, highp>;

	using Double	= double;
	using Double2	= dvec2;
	using Double3	= dvec3;
	using Double4	= dvec4;
	using Double2x2	= dmat2x2;
	using Double2x3	= dmat2x3;
	using Double2x4	= dmat2x4;
	using Double3x2	= dmat3x2;
	using Double3x3	= dmat3x3;
	using Double3x4	= dmat3x4;
	using Double4x2	= dmat4x2;
	using Double4x3	= dmat4x3;
	using Double4x4	= dmat4x4;


	STATIC_ASSERT( sizeof(Int2) == sizeof(int)*2 );
	STATIC_ASSERT( sizeof(Int3) == sizeof(int)*3 );
	STATIC_ASSERT( sizeof(Int4) == sizeof(int)*4 );

	STATIC_ASSERT( sizeof(UInt2) == sizeof(uint)*2 );
	STATIC_ASSERT( sizeof(UInt3) == sizeof(uint)*3 );
	STATIC_ASSERT( sizeof(UInt4) == sizeof(uint)*4 );

	STATIC_ASSERT( sizeof(Long2) == sizeof(int64_t)*2 );
	STATIC_ASSERT( sizeof(Long3) == sizeof(int64_t)*3 );
	STATIC_ASSERT( sizeof(Long4) == sizeof(int64_t)*4 );

	STATIC_ASSERT( sizeof(ULong2) == sizeof(uint64_t)*2 );
	STATIC_ASSERT( sizeof(ULong3) == sizeof(uint64_t)*3 );
	STATIC_ASSERT( sizeof(ULong4) == sizeof(uint64_t)*4 );

	STATIC_ASSERT( sizeof(Float2) == sizeof(float)*2 );
	STATIC_ASSERT( sizeof(Float3) == sizeof(float)*3 );
	STATIC_ASSERT( sizeof(Float4) == sizeof(float)*4 );
	STATIC_ASSERT( sizeof(Float2x2) == sizeof(float)*2*2 );
	STATIC_ASSERT( sizeof(Float2x3) == sizeof(float)*2*3 );
	STATIC_ASSERT( sizeof(Float2x4) == sizeof(float)*2*4 );
	STATIC_ASSERT( sizeof(Float3x2) == sizeof(float)*3*2 );
	STATIC_ASSERT( sizeof(Float3x3) == sizeof(float)*3*3 );
	STATIC_ASSERT( sizeof(Float3x4) == sizeof(float)*3*4 );
	STATIC_ASSERT( sizeof(Float4x2) == sizeof(float)*4*2 );
	STATIC_ASSERT( sizeof(Float4x3) == sizeof(float)*4*3 );
	STATIC_ASSERT( sizeof(Float4x4) == sizeof(float)*4*4 );

	STATIC_ASSERT( sizeof(Double2) == sizeof(double)*2 );
	STATIC_ASSERT( sizeof(Double3) == sizeof(double)*3 );
	STATIC_ASSERT( sizeof(Double4) == sizeof(double)*4 );
	STATIC_ASSERT( sizeof(Double2x2) == sizeof(double)*2*2 );
	STATIC_ASSERT( sizeof(Double2x3) == sizeof(double)*2*3 );
	STATIC_ASSERT( sizeof(Double2x4) == sizeof(double)*2*4 );
	STATIC_ASSERT( sizeof(Double3x2) == sizeof(double)*3*2 );
	STATIC_ASSERT( sizeof(Double3x3) == sizeof(double)*3*3 );
	STATIC_ASSERT( sizeof(Double3x4) == sizeof(double)*3*4 );
	STATIC_ASSERT( sizeof(Double4x2) == sizeof(double)*4*2 );
	STATIC_ASSERT( sizeof(Double4x3) == sizeof(double)*4*3 );
	STATIC_ASSERT( sizeof(Double4x4) == sizeof(double)*4*4 );



	//
	// Bool
	//
	struct Bool final
	{
	private:
		uint	_value;

	public:
		Bool () : _value(0) {}
		Bool (bool v) : _value(v) {}
		explicit Bool (Int v) : _value(v) {}
		explicit Bool (UInt v) : _value(v) {}

		operator bool () const				{ return !!_value; }

		Bool operator ! () const			{ return !_value; }

		bool operator == (Bool right) const	{ return _value == right._value; }
		bool operator != (Bool right) const	{ return ! (*this == right); }
	};


	template <typename T, precision P> struct Bool2_t;
	template <typename T, precision P> struct Bool3_t;
	template <typename T, precision P> struct Bool4_t;


	//
	// Bool2
	//
	template <typename T = Bool, precision P = defaultp>
	struct Bool2_t final
	{
	public:
		union {
			struct{ T x, y; };

#			if GLM_SWIZZLE == GLM_SWIZZLE_ENABLED
				_GLM_SWIZZLE2_2_MEMBERS(T, P, glm::Bool2_t, x, y)
				_GLM_SWIZZLE2_3_MEMBERS(T, P, glm::Bool3_t, x, y)
				_GLM_SWIZZLE2_4_MEMBERS(T, P, glm::Bool4_t, x, y)
#			endif//GLM_SWIZZLE
		};

	public:
		Bool2_t () {}
		Bool2_t (const bvec2 &v) : x(v.x), y(v.y) {}
		Bool2_t (bool X, bool Y) : x(X), y(Y) {}
		Bool2_t (T X, T Y) : x(X), y(Y) {}
		explicit Bool2_t (bool v) : x(v), y(v) {}
		explicit Bool2_t (const Int2 &v) : x(!!v.x), y(!!v.y) {}
		explicit Bool2_t (const UInt2 &v) : x(!!v.x), y(!!v.y) {}

		operator const bvec2 () const							{ return bvec2(x, y); }

		Bool2_t operator ! () const								{ return Bool2_t(!x, !y); }
		
		typedef length_t length_type;
		static constexpr length_type length ()					{ return 2; }

		T &			operator [] (length_type i)					{ assert(i >= 0 && i < length());  return (&x)[i]; }
		T const&	operator [] (length_type i) const			{ assert(i >= 0 && i < length());  return (&x)[i]; }
		
		Bool2_t		operator == (const Bool2_t &right) const	{ return Bool2_t{ x == right.x, y == right.y }; }
		Bool2_t		operator != (const Bool2_t &right) const	{ return ! (*this == right); }
	};

	
	//
	// Bool3
	//
	template <typename T = Bool, precision P = defaultp>
	struct Bool3_t final
	{
	public:
		union
		{
			struct{ T x, y, z; };

#			if GLM_SWIZZLE == GLM_SWIZZLE_ENABLED
				_GLM_SWIZZLE3_2_MEMBERS(T, P, glm::Bool2_t, x, y, z)
				_GLM_SWIZZLE3_3_MEMBERS(T, P, glm::Bool3_t, x, y, z)
				_GLM_SWIZZLE3_4_MEMBERS(T, P, glm::Bool4_t, x, y, z)
#			endif//GLM_SWIZZLE
		};

	public:
		Bool3_t () {}
		Bool3_t (const bvec3 &v) : x(v.x), y(v.y), z(v.z) {}
		Bool3_t (bool X, bool Y, bool Z) : x(X), y(Y), z(Z) {}
		Bool3_t (T X, T Y, T Z) : x(X), y(Y), z(Z) {}
		explicit Bool3_t (bool v) : x(v), y(v), z(v) {}
		explicit Bool3_t (const Int3 &v) : x(!!v.x), y(!!v.y), z(!!v.z) {}
		explicit Bool3_t (const UInt3 &v) : x(!!v.x), y(!!v.y), z(!!v.z) {}

		operator const bvec3 () const							{ return bvec3(x, y, z); }

		Bool3_t operator ! () const								{ return Bool3_t(!x, !y, !z); }
		
		typedef length_t length_type;
		static constexpr length_type length ()					{ return 3; }
		
		T &			operator [] (length_type i)					{ assert(i >= 0 && i < length());  return (&x)[i]; }
		T const&	operator [] (length_type i) const			{ assert(i >= 0 && i < length());  return (&x)[i]; }
		
		Bool3_t		operator == (const Bool3_t &right) const	{ return Bool3_t{ x == right.x, y == right.y, z == right.z }; }
		Bool3_t		operator != (const Bool3_t &right) const	{ return ! (*this == right); }
	};

	
	//
	// Bool4
	//
	template <typename T = Bool, precision P = defaultp>
	struct Bool4_t final
	{
	public:
		union
		{
			struct { T x, y, z, w; };

#			if GLM_SWIZZLE == GLM_SWIZZLE_ENABLED
				_GLM_SWIZZLE4_2_MEMBERS(T, P, glm::Bool2_t, x, y, z, w)
				_GLM_SWIZZLE4_3_MEMBERS(T, P, glm::Bool3_t, x, y, z, w)
				_GLM_SWIZZLE4_4_MEMBERS(T, P, glm::Bool4_t, x, y, z, w)
#			endif//GLM_SWIZZLE
		};

	public:
		Bool4_t () {}
		Bool4_t (const bvec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
		Bool4_t (bool X, bool Y, bool Z, bool W) : x(X), y(Y), z(Z), w(W) {}
		Bool4_t (T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}
		explicit Bool4_t (bool v) : x(v), y(v), z(v), w(v) {}
		explicit Bool4_t (const Int4 &v) : x(!!v.x), y(!!v.y), z(!!v.z), w(!!v.w) {}
		explicit Bool4_t (const UInt4 &v) : x(!!v.x), y(!!v.y), z(!!v.z), w(!!v.w) {}

		operator const bvec4 () const							{ return bvec4(x, y, z, w); }

		Bool4_t operator ! () const								{ return Bool4_t(!x, !y, !z, !w); }
		
		typedef length_t length_type;
		static constexpr length_type length ()					{ return 4; }

		T &			operator [] (length_type i)					{ assert(i >= 0 && i < length());  return (&x)[i]; }
		T const&	operator [] (length_type i) const			{ assert(i >= 0 && i < length());  return (&x)[i]; }

		Bool4_t		operator == (const Bool4_t &right) const	{ return Bool4_t{ x == right.x, y == right.y, z == right.z, w == right.w }; }
		Bool4_t		operator != (const Bool4_t &right) const	{ return ! (*this == right); }
	};


	using Bool2	= Bool2_t<>;
	using Bool3	= Bool3_t<>;
	using Bool4	= Bool4_t<>;


	ND_ inline Bool all (Bool value)
	{
		return value;
	}

	ND_ inline Bool all (const Bool2 &value)
	{
		return value.x && value.y;
	}
	
	ND_ inline Bool all (const Bool3 &value)
	{
		return value.x && value.y && value.z;
	}
	
	ND_ inline Bool all (const Bool4 &value)
	{
		return value.x && value.y && value.z && value.w;
	}


	ND_ inline Bool any (const Bool2 &value)
	{
		return value.x || value.y;
	}
	
	ND_ inline Bool any (const Bool3 &value)
	{
		return value.x || value.y || value.z;
	}
	
	ND_ inline Bool any (const Bool4 &value)
	{
		return value.x || value.y || value.z || value.w;
	}



	//
	// Atomic
	//
	template <typename T>
	struct Atomic final
	{
	private:
		std::atomic<T>	_value;

	public:
		Atomic () : _value{0} {}
		explicit Atomic (T value) : _value{value} {}
		explicit Atomic (const Atomic<T> &other) : _value{other._value} {}

		Atomic& operator = (const Atomic<T> &right)
		{
			_value = right._value;
			return *this;
		}

		explicit operator const T () const noexcept
		{
			return _value.load();
		}

		friend T atomicAdd (INOUT Atomic<T> &mem, const T data) noexcept
		{
			return mem._value.fetch_add( data );	// TODO: relaxed ?
		}

		friend T atomicAnd (INOUT Atomic<T> &mem, const T data) noexcept
		{
			return mem._value.fetch_and( data );	// TODO: relaxed ?
		}

		friend T atomicCompSwap (INOUT Atomic<T> &mem, T compare, const T data) noexcept
		{
			mem._value.compare_exchange_strong( INOUT compare, data );	// TODO: relaxed ?
			return compare;
		}

		friend T atomicExchange (INOUT Atomic<T> &mem, const T data) noexcept
		{
			return mem._value.exchange( data );	// TODO: relaxed ?
		}
		
		friend T atomicMax (INOUT Atomic<T> &mem, const T data) noexcept
		{
			return std::max( mem._value.load(), data );	// TODO: relaxed ?
		}

		friend T atomicMin (INOUT Atomic<T> &mem, const T data) noexcept
		{
			return std::min( mem._value.load(), data );	// TODO: relaxed ?
		}

		friend T atomicOr (INOUT Atomic<T> &mem, const T data) noexcept
		{
			return mem._value.fetch_or( data );	// TODO: relaxed ?
		}

		friend T atomicXor (INOUT Atomic<T> &mem, const T data) noexcept
		{
			return mem._value.fetch_xor( data );	// TODO: relaxed ?
		}

		static_assert( std::is_integral_v<T>, "atomic type must be integer scalar!" );
	};
	
	
/*
=================================================
	memory barriers
=================================================
*/
	inline void memoryBarrier ()
	{
		std::atomic_thread_fence( std::memory_order_acq_rel );	// TODO: check mem order
	}

	inline void memoryBarrierAtomicCounter ()
	{
		// in CPU there is only one memory fence type
		memoryBarrier();
	}

	inline void memoryBarrierBuffer ()
	{
		// in CPU there is only one memory fence type
		memoryBarrier();
	}

	inline void memoryBarrierImage ()
	{
		// in CPU there is only one memory fence type
		memoryBarrier();
	}

	inline void memoryBarrierShared ()
	{
		// in CPU there is only one memory fence type
		memoryBarrier();
	}

	inline void groupMemoryBarrier ()
	{
		// in CPU there is only one memory fence type
		memoryBarrier();
	}

}	// glm


#ifdef COMPILER_MSVC
# pragma warning (pop)
#endif

#endif	// GRAPHICS_API_SOFT
