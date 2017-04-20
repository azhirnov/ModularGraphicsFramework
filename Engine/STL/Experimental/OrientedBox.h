// Copyright © 2014-2016  Zhirnov Andrey. All rights reserved.

#pragma once

#include "MathTypes3D.h"

namespace GX_STL
{
namespace GXMath
{

	/*
	//
	// Object-Oriented Bounding Box
	//

	template <typename T>
	struct OOBB : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		typedef OOBB< T >			Self;
		typedef Vec<T,3>			vec3_t;
		typedef Vec<T,4>			vec4_t;
		typedef Matrix<T,4,4>		mat4x4_t;
		typedef Matrix<T,4,3>		mat4x3_t;
		typedef Transformation<T>	transform_t;
		typedef T					value_t;


	// variables
	private:
		StaticArray< vec3_t, 8 >	_vertices;


	// methods
	public:
		OOBB () {}

		void Create (const vec3_t &size, const mat4x4_t &mat);
		void Create (const vec3_t &size, const mat4x3_t &mat);
		void Create (const vec3_t &size, const transform_t &transform);

		vec3_t const &	operator [] (usize index)	const	{ return _vertices[ index ]; }
		usize			Count ()					const	{ return _vertices.Count(); }
	};

	

	template <typename T>
	inline void OOBB<T>::Create (const vec3_t &size, const mat4x4_t &mat)
	{
		vec3_t const		hsize = size * T(0.5);

		_vertices[0] = (mat * vec4_t( -hsize.x, -hsize.y, -hsize.z, T(1) )).xyz();
		_vertices[1] = (mat * vec4_t( +hsize.x, -hsize.y, -hsize.z, T(1) )).xyz();
		_vertices[2] = (mat * vec4_t( +hsize.x, +hsize.y, -hsize.z, T(1) )).xyz();
		_vertices[3] = (mat * vec4_t( -hsize.x, +hsize.y, -hsize.z, T(1) )).xyz();
		_vertices[4] = (mat * vec4_t( -hsize.x, -hsize.y, +hsize.z, T(1) )).xyz();
		_vertices[5] = (mat * vec4_t( +hsize.x, -hsize.y, +hsize.z, T(1) )).xyz();
		_vertices[6] = (mat * vec4_t( +hsize.x, +hsize.y, +hsize.z, T(1) )).xyz();
		_vertices[7] = (mat * vec4_t( -hsize.x, +hsize.y, +hsize.z, T(1) )).xyz();
	}
	

	template <typename T>
	inline void OOBB<T>::Create (const vec3_t &size, const mat4x3_t &mat)
	{
		vec3_t const		hsize = size * T(0.5);

		_vertices[0] = mat * vec4_t( -hsize.x, -hsize.y, -hsize.z, T(1) );
		_vertices[1] = mat * vec4_t( +hsize.x, -hsize.y, -hsize.z, T(1) );
		_vertices[2] = mat * vec4_t( +hsize.x, +hsize.y, -hsize.z, T(1) );
		_vertices[3] = mat * vec4_t( -hsize.x, +hsize.y, -hsize.z, T(1) );
		_vertices[4] = mat * vec4_t( -hsize.x, -hsize.y, +hsize.z, T(1) );
		_vertices[5] = mat * vec4_t( +hsize.x, -hsize.y, +hsize.z, T(1) );
		_vertices[6] = mat * vec4_t( +hsize.x, +hsize.y, +hsize.z, T(1) );
		_vertices[7] = mat * vec4_t( -hsize.x, +hsize.y, +hsize.z, T(1) );
	}
	

	template <typename T>
	inline void OOBB<T>::Create (const vec3_t &size, const transform_t &transform)
	{
		mat4x4_t	mat;
		transform.GetMatrix( mat );
		Create( size, mat );
	}
	*/


}	// GXMath
}	// GX_STL
