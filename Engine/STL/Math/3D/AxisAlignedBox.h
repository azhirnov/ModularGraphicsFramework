// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Math/Matrix4.h"

namespace GX_STL
{
namespace GXMath
{

	//
	// Axis Aligned Bounding Box
	//

	template <typename T>
	struct AABBox : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		typedef AABBox<T>		Self;
		typedef T				value_t;
		
		struct ECorner
		{
			enum type
			{
				FarLeftBottom,
				FarLeftTop,
				FarRightTop,
				FarRightBottom,
				NearRightTop,
				NearLeftTop,
				NearLeftBottom,
				NearRightBottom,
				_Count
			};
		};
		typedef typename ECorner::type	_Corner_t;


	// variables
	private:
		Vec<T,3>	_min,
					_max;

	// methods
	public:
		AABBox ();
		AABBox (const AABBox<T> &box);
		AABBox (const Vec<T,3> &min, const Vec<T,3> &max);

		Vec<T,3>		&	Max ()					{ return _max; }
		Vec<T,3>		&	Min ()					{ return _min; }

		Vec<T,3> const &	Max ()			const	{ return _max; }
		Vec<T,3> const &	Min ()			const	{ return _min; }

		Vec<T,3> const		Center ()		const	{ return (_min + _max) * T(0.5); }
		Vec<T,3> const		Extent ()		const	{ return _max - _min; }
		Vec<T,3> const		HalfExtent ()	const	{ return Extent() * T(0.5); }
		bool				Empty ()		const	{ return All( _max == _min ); }

		T		*			ptr ()					{ return _min.ptr(); }
		T const	*			ptr ()			const	{ return _min.ptr(); }


		bool	operator == (const AABBox<T> &right) const;
		bool	operator != (const AABBox<T> &right) const;

		Self &	operator += (const Vec<T,3> &right);
		Self &	operator += (const AABBox<T> &right);

		//Self &	operator *= (const Matrix<T,4,4> &right);
		//Self &	operator *= (const Matrix<T,3,3> &right);

		Self	operator +  (const Vec<T,3> &right) const;
		Self	operator +  (const AABBox<T> &right) const;

		//Self	operator *  (const Matrix<T,4,4> &right) const;
		//Self	operator *  (const Matrix<T,3,3> &right) const;


		T		Volume () const;
		T		MinRadius () const;
		T		MaxRadius () const;
		
		void	Add (const Vec<T,3> &point);
		void	Add (const AABBox<T> &box);
		
		void	Repair ();

		void	Move (const Vec<T,3> &delta);
		void	Scale (const Vec<T,3> &scale);

		//void	Rotate (const Matrix<T,3,3> &mat);
		//void	Rotate (const Matrix<T,4,4> &mat);
		//void	Transform (const Matrix<T,4,4> &mat);
		//void	TransformExt (const Matrix<T,4,4> &mat);

		//void	InterpolateL (const AABBox<T> &sBox1, const AABBox<T> &sBox2, T tK);
		//void	InterpolateQ (const AABBox<T> &sBox1, const AABBox<T> &sBox2, const AABBox<T> &sBox3, T tK);

		bool	Intersect (const AABBox<T> &box) const;
		Self	Intersection (const AABBox<T> &box) const;

		bool	IsInside (const AABBox<T> &box) const;
		bool	IsInside (const Vec<T,3> &point) const;

		bool	IsFullInside (const AABBox<T> &box) const;
		bool	IsFullInside (const Vec<T,3> &point) const;

		Self &	Set (const Vec<T,3> &point);
		Self &	Set (const Vec<T,3> &min, const Vec<T,3> &max);
		Self &	SetBBox (const Vec<T,3> &center, const Vec<T,3> &extents);

		Vec<T,3>	GetCorner (_Corner_t corner) const;
		void		GetPoints (Vec<T,3> aPoints[8]) const;

		template <typename T2>
		AABBox<T2>	Convert() const;

		//Sphere<T>	ToSphere() const;
	};


	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline AABBox<T>::AABBox () : _min(), _max()
	{}

	template <typename T>
	inline AABBox<T>::AABBox (const AABBox<T> &box) : _min(box._min), _max(box._max)
	{}

	template <typename T>
	inline AABBox<T>::AABBox (const Vec<T,3> &min, const Vec<T,3> &max) : _min(min), _max(max)
	{}
	
/*
=================================================
	Set
=================================================
*/
	template <typename T>
	inline AABBox<T> & AABBox<T>::Set (const Vec<T,3> &point)
	{
		_min = point;
		_max = point;
		return *this;
	}
	
/*
=================================================
	Set
=================================================
*/
	template <typename T>
	inline AABBox<T> & AABBox<T>::Set (const Vec<T,3> &min, const Vec<T,3> &max)
	{
		_min = min;
		_max = max;
		return *this;
	}
	
/*
=================================================
	SetBBox
=================================================
*/
	template <typename T>
	inline AABBox<T> & AABBox<T>::SetBBox (const Vec<T,3> &center, const Vec<T,3> &extents)
	{
		_min = center - extents * T(0.5);
		_max = center + extents * T(0.5);
		return *this;
	}
	
/*
=================================================
	Add
=================================================
*/
	template <typename T>
	inline void AABBox<T>::Add (const Vec<T,3> &point)
	{
		_max.MakeCeil( point );
		_min.MakeFloor( point );
	}
	
/*
=================================================
	Add
=================================================
*/
	template <typename T>
	inline void AABBox<T>::Add (const AABBox<T> &box)
	{
		Add( box.Min() );
		Add( box.Max() );
	}
	
/*
=================================================
	Repair
=================================================
*/
	template <typename T>
	inline void AABBox<T>::Repair ()
	{
		if ( _min.x > _max.x )	SwapValues( _min.x, _max.x );
		if ( _min.y > _max.y )	SwapValues( _min.y, _max.y );
		if ( _min.z > _max.z )	SwapValues( _min.z, _max.z );
	}
	
/*
=================================================
	Move
=================================================
*/
	template <typename T>
	inline void AABBox<T>::Move (const Vec<T,3> &delta)
	{
		_min += delta;
		_max += delta;
	}
	
/*
=================================================
	Scale
=================================================
*/
	template <typename T>
	inline void AABBox<T>::Scale (const Vec<T,3> &scale)
	{
		_min *= scale;
		_max *= scale;
	}
	
/*
=================================================
	Rotate
=================================================
*
	template <typename T>
	inline void AABBox<T>::Rotate (const Matrix<T,3,3> &mat)
	{
		Vec<T,3>	v_min	 = _min,
					v_max	 = _max,
					v_corner = _min;

		Set( mat.RotateVect( v_corner ) );

		v_corner.z = v_max.z;
		Add( mat.RotateVect( v_corner ) );

		v_corner.y = v_max.y;
		Add( mat.RotateVect( v_corner ) );

		v_corner.z = v_min.z;
		Add( mat.RotateVect( v_corner ) );

		v_corner.x = v_max.x;
		Add( mat.RotateVect( v_corner ) );

		v_corner.z = v_max.z;
		Add( mat.RotateVect( v_corner ) );

		v_corner.y = v_min.y;
		Add( mat.RotateVect( v_corner ) );

		v_corner.z = v_min.z;
		Add( mat.RotateVect( v_corner ) );
	}
	
/*
=================================================
	Rotate
=================================================
*
	template <typename T>
	inline void AABBox<T>::Rotate (const Matrix<T,4,4> &mat)
	{
		Vec<T,3>	v_min	 = _min,
					v_max	 = _max,
					v_corner = _min;

		Set( mat.RotateVect( v_corner ) );

		v_corner.z = v_max.z;
		Add( mat.RotateVect( v_corner ) );

		v_corner.y = v_max.y;
		Add( mat.RotateVect( v_corner ) );

		v_corner.z = v_min.z;
		Add( mat.RotateVect( v_corner ) );

		v_corner.x = v_max.x;
		Add( mat.RotateVect( v_corner ) );

		v_corner.z = v_max.z;
		Add( mat.RotateVect( v_corner ) );

		v_corner.y = v_min.y;
		Add( mat.RotateVect( v_corner ) );

		v_corner.z = v_min.z;
		Add( mat.RotateVect( v_corner ) );
	}
	
/*
=================================================
	Transform
=================================================
*
	template <typename T>
	inline void AABBox<T>::Transform (const Matrix<T,4,4> &mat)
	{
		Vec<T,3>	v_min	 = _min,
					v_max	 = _max,
					v_corner = _min;

		Set( mat.TransformVect( v_corner ) );

		v_corner.z = v_max.z;
		Add( mat.TransformVect( v_corner ) );

		v_corner.y = v_max.y;
		Add( mat.TransformVect( v_corner ) );

		v_corner.z = v_min.z;
		Add( mat.TransformVect( v_corner ) );

		v_corner.x = v_max.x;
		Add( mat.TransformVect( v_corner ) );

		v_corner.z = v_max.z;
		Add( mat.TransformVect( v_corner ) );

		v_corner.y = v_min.y;
		Add( mat.TransformVect( v_corner ) );

		v_corner.z = v_min.z;
		Add( mat.TransformVect( v_corner ) );
	}
	
/*
=================================================
	TransformExt
=================================================
*
	template <typename T>
	inline void AABBox<T>::TransformExt (const Matrix<T,4,4> &mat)
	{
		Vec<T,3>	v_center	= Center() + mat.Translation(),
					v_extent	= HalfExtent(),
					v_min		= -v_extent,
					v_max		= v_extent;

		_min = _max = v_center;

		for (uint i = 0; i < 3; ++i)
		{
			for (uint j = 0; j < 3; ++j)
			{
				const T	a = mat(j,i) * v_min[i];
				const T	b = mat(j,i) * v_max[j];

				if ( a < b ) {
					_min[i] += a;
					_max[i] += b;
				}
				else {
					_min[i] += b;
					_max[i] += a;
				}
			}
		}
	}
	
/*
=================================================
	InterpolateL
=================================================
*
	template <typename T>
	inline void AABBox<T>::InterpolateL (const AABBox<T> &sBox1, const AABBox<T> &sBox2, T tK)
	{
		_min.InterpolateL( sBox1.Min(), sBox2.Min(), tK );
		_max.InterpolateL( sBox1.Max(), sBox2.Max(), tK );
	}
	
/*
=================================================
	InterpolateQ
=================================================
*
	template <typename T>
	inline void AABBox<T>::InterpolateQ (const AABBox<T> &sBox1, const AABBox<T> &sBox2, const AABBox<T> &sBox3, T tK)
	{
		_min.InterpolateQ( sBox1.Min(), sBox2.Min(), sBox3.Min(), tK );
		_max.InterpolateQ( sBox1.Max(), sBox2.Max(), sBox3.Max(), tK );
	}
	
/*
=================================================
	operator ==
=================================================
*/
	template <typename T>
	inline bool AABBox<T>::operator == (const AABBox<T> &right) const
	{
		return ( _min == right._min and _max == right._max );
	}
	
	template <typename T>
	inline bool AABBox<T>::operator != (const AABBox<T> &right) const
	{
		return not ( *this == right );
	}
	
/*
=================================================
	operator +=
=================================================
*/
	template <typename T>
	inline AABBox<T> & AABBox<T>::operator += (const Vec<T,3> &right)
	{
		Add( right );
		return *this;
	}

	template <typename T>
	inline AABBox<T> & AABBox<T>::operator += (const AABBox<T> &right)
	{
		Add( right );
		return *this;
	}
	
/*
=================================================
	operator +
=================================================
*/
	template <typename T>
	inline AABBox<T> AABBox<T>::operator + (const Vec<T,3> &right) const
	{
		return AABBox<T>(*this) += right;
	}

	template <typename T>
	inline AABBox<T> AABBox<T>::operator + (const AABBox<T> &right) const
	{
		return AABBox<T>(*this) += right;
	}
	
/*
=================================================
	operator *=
=================================================
*
	template <typename T>
	inline AABBox<T> & AABBox<T>::operator *= (const Matrix<T,4,4> &right)
	{
		TransformExt( right );
		return *this;
	}

	template <typename T>
	inline AABBox<T> & AABBox<T>::operator *= (const Matrix<T,3,3> &right)
	{
		Rotate( right );
		return *this;
	}
	
/*
=================================================
	operator *
=================================================
*
	template <typename T>
	inline AABBox<T> AABBox<T>::operator * (const Matrix<T,4,4> &right) const
	{
		AABBox<T>	ret(*this);
		ret.TransformExt( right );
		return ret;
	}

	template <typename T>
	inline AABBox<T> AABBox<T>::operator * (const Matrix<T,3,3> &right) const
	{
		AABBox<T>	ret(*this);
		ret.Transform( right );
		return ret;
	}
	
/*
=================================================
	Volume
=================================================
*/
	template <typename T>
	inline T AABBox<T>::Volume () const
	{
		Vec<T,3> const	ext = Extent();
		return ext.x * ext.y * ext.z;
	}
	
/*
=================================================
	MinRadius
=================================================
*/
	template <typename T>
	inline T AABBox<T>::MinRadius () const
	{
		const Vec<T,3>	side = Extent();
		return T(0.5) * GXMath::Min( side.x, side.y, side.z );
	}
	
/*
=================================================
	MaxRadius
=================================================
*/
	template <typename T>
	inline T AABBox<T>::MaxRadius () const
	{
		const Vec<T,3>	side		= Extent();
		const T			sq3_div2	= T(0.86602540378443864676372317075294);

		return sq3_div2 * GXMath::Max( side.x, side.y, side.z );
	}
	
/*
=================================================
	IsInside
=================================================
*/
	template <typename T>
	inline bool AABBox<T>::IsInside (const Vec<T,3> &point) const
	{
		return All( point >= _min ) and All( point <= _max );
	}
	
/*
=================================================
	IsFullInside
=================================================
*/
	template <typename T>
	inline bool AABBox<T>::IsFullInside (const Vec<T,3> &point) const
	{
		return All( point > _min ) and All( point < _max );
	}
	
/*
=================================================
	IsInside
=================================================
*/
	template <typename T>
	inline bool AABBox<T>::IsInside (const AABBox<T> &box) const
	{
		return All( box._min >= _min ) and All( box._max <= _max );
	}
	
/*
=================================================
	IsFullInside
=================================================
*/
	template <typename T>
	inline bool AABBox<T>::IsFullInside (const AABBox<T> &box) const
	{
		return All( box._min > _min ) and All( box._max < _max );
	}
	
/*
=================================================
	Intersect
=================================================
*/
	template <typename T>
	inline bool AABBox<T>::Intersect (const AABBox<T> &box) const
	{
		return ( _max.x < box._min.x or _max.y < box._min.y or _max.z < box._min.z or
				 _min.x > box._max.x or _min.y > box._max.y or _min.z > box._max.z );
	}
	
/*
=================================================
	Intersection
=================================================
*/
	template <typename T>
	inline AABBox<T> AABBox<T>::Intersection (const AABBox<T> &box) const
	{
		AABBox<T>	ret( *this );

		ret.Min().MakeCeil( box.Min() );
		ret.Max().MakeFloor( box.Max() );
		ret.Repair();

		return ret;
	}
	
/*
=================================================
	GetCorner
=================================================
*/
	template <typename T>
	inline Vec<T,3> AABBox<T>::GetCorner (_Corner_t corner) const
	{
		switch ( corner )
		{
			case ECorner::FarLeftBottom		:	return _min;
			case ECorner::FarLeftTop		:	return Vec<T,3>( _min.x, _max.y, _min.z );
			case ECorner::FarRightTop		:	return Vec<T,3>( _max.x, _max.y, _min.z );
			case ECorner::FarRightBottom	:	return Vec<T,3>( _max.x, _min.y, _min.z );
			case ECorner::NearRightBottom	:	return Vec<T,3>( _max.x, _min.y, _max.z );
			case ECorner::NearLeftBottom	:	return Vec<T,3>( _min.x, _min.y, _max.z );
			case ECorner::NearLeftTop		:	return Vec<T,3>( _min.x, _max.y, _max.z );
			case ECorner::NearRightTop		:	return _max;
		}

		WARNING( "unknown corner type" );
		return Vec<T,3>();
	}
	
/*
=================================================
	GetPoints
=================================================
*/
	template <typename T>
	inline void AABBox<T>::GetPoints (Vec<T,3> aPoints[8]) const
	{
		aPoints[0] = _min;
		aPoints[1] = Vec<T,3>( _min.x, _max.y, _min.z );
		aPoints[2] = Vec<T,3>( _max.x, _max.y, _min.z );
		aPoints[3] = Vec<T,3>( _max.x, _min.y, _min.z );
		aPoints[4] = Vec<T,3>( _max.x, _min.y, _max.z );
		aPoints[5] = Vec<T,3>( _min.x, _min.y, _max.z );
		aPoints[6] = Vec<T,3>( _min.x, _max.y, _max.z );
		aPoints[7] = _max;
	}
	
/*
=================================================
	Convert
=================================================
*/
	template <typename T>
	template <typename T2>
	inline AABBox<T2>  AABBox<T>::Convert () const
	{
		return AABBox<T2>( _min.Convert<T2>(), _max.Convert<T2>() );
	}
	
/*
=================================================
	ToSphere
=================================================
*
	template <typename T>
	inline Sphere<T>  AABBox<T>::ToSphere () const
	{
		return Sphere<T>( Center(), MaxRadius() );
	}
	*/


}	// GXMath
}	// GX_STL
