// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Shader/Node.h"

#define NVEC1_SWIZZLE( _a_, _getA_ ) \
	Property< NVec<T,2>, _getA_, _getA_ >						_a_##_a_; \
	Property< NVec<T,3>, _getA_, _getA_, _getA_ >				_a_##_a_##_a_; \
	Property< NVec<T,4>, _getA_, _getA_, _getA_, _getA_ >		_a_##_a_##_a_##_a_; \


#define NVEC2_SWIZZLE( _a_, _b_, _getA_, _getB_ ) \
	Property< NVec<T,2>, _getA_, _getB_ >						_a_##_b_; \
	Property< NVec<T,2>, _getB_, _getA_ >						_b_##_a_; \
	\
	Property< NVec<T,3>, _getA_, _getA_, _getB_ >				_a_##_a_##_b_; \
	Property< NVec<T,3>, _getA_, _getB_, _getA_ >				_a_##_b_##_a_; \
	Property< NVec<T,3>, _getA_, _getB_, _getB_ >				_a_##_b_##_b_; \
	Property< NVec<T,3>, _getB_, _getA_, _getA_ >				_b_##_a_##_a_; \
	Property< NVec<T,3>, _getB_, _getA_, _getB_ >				_b_##_a_##_b_; \
	Property< NVec<T,3>, _getB_, _getB_, _getA_ >				_b_##_b_##_a_; \
	\
	Property< NVec<T,4>, _getA_, _getA_, _getA_, _getB_ >		_a_##_a_##_a_##_b_; \
	Property< NVec<T,4>, _getA_, _getA_, _getB_, _getA_ >		_a_##_a_##_b_##_a_; \
	Property< NVec<T,4>, _getA_, _getA_, _getB_, _getB_ >		_a_##_a_##_b_##_b_; \
	Property< NVec<T,4>, _getA_, _getB_, _getA_, _getA_ >		_a_##_b_##_a_##_a_; \
	Property< NVec<T,4>, _getA_, _getB_, _getA_, _getB_ >		_a_##_b_##_a_##_b_; \
	Property< NVec<T,4>, _getA_, _getB_, _getB_, _getA_ >		_a_##_b_##_b_##_a_; \
	Property< NVec<T,4>, _getA_, _getB_, _getB_, _getB_ >		_a_##_b_##_b_##_b_; \
	Property< NVec<T,4>, _getB_, _getA_, _getA_, _getA_ >		_b_##_a_##_a_##_a_; \
	Property< NVec<T,4>, _getB_, _getA_, _getA_, _getB_ >		_b_##_a_##_a_##_b_; \
	Property< NVec<T,4>, _getB_, _getA_, _getB_, _getA_ >		_b_##_a_##_b_##_a_; \
	Property< NVec<T,4>, _getB_, _getA_, _getB_, _getB_ >		_b_##_a_##_b_##_b_; \
	Property< NVec<T,4>, _getB_, _getB_, _getA_, _getA_ >		_b_##_b_##_a_##_a_; \
	Property< NVec<T,4>, _getB_, _getB_, _getA_, _getB_ >		_b_##_b_##_a_##_b_; \
	Property< NVec<T,4>, _getB_, _getB_, _getB_, _getA_ >		_b_##_b_##_b_##_a_; \


#define NVEC3_SWIZZLE( _a_, _b_, _c_, _getA_, _getB_, _getC_ ) \
	Property< NVec<T,3>, _getA_, _getB_, _getC_ >				_a_##_b_##_c_; \
	Property< NVec<T,3>, _getB_, _getC_, _getA_ >				_b_##_c_##_a_; \
	Property< NVec<T,3>, _getC_, _getA_, _getB_ >				_c_##_a_##_b_; \
	Property< NVec<T,3>, _getC_, _getB_, _getA_ >				_c_##_b_##_a_; \
	Property< NVec<T,3>, _getB_, _getA_, _getC_ >				_b_##_a_##_c_; \
	Property< NVec<T,3>, _getA_, _getC_, _getB_ >				_a_##_c_##_b_; \
	\
	/*Property< NVec<T,4>, _getA_, _getA_, _getB_, _getC_ >		_a_##_a_##_a_##_b_; \
	/* TODO */


#define NVEC4_SWIZZLE( _a_, _b_, _c_, _d_, _getA_, _getB_, _getC_, _getD_ ) \
	Property< NVec<T,4>, _getA_, _getB_, _getC_, _getD_ >		_a_##_b_##_c_##_d_; \
	/* TODO */

