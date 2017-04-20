// Copyright © 2014-2016  Zhirnov Andrey. All rights reserved.

#pragma once

#include "MathTypes3D.h"
#include "AxisAlignedBox.h"

namespace GX_STL
{
namespace GXMath
{
	/*
	//
	// Bounding Sphere
	//

	template <typename T>
	struct TSphere : public CompileTime::CopyQualifiers< T >
	{
	public:
		typedef TSphere<T>	Self;

	private:
		Vec<T,3>	_vCenter;
		T			_tRadius;

	public:
		TSphere();
		TSphere(const TSphere<T> &s);
		TSphere(const Vec<T,3> &center, const T& radius);

		T		&			Radius()			{ return _tRadius; }
		T const	&			Radius()	const	{ return _tRadius; }

		Vec<T,3>	&		Center()			{ return _vCenter; }
		Vec<T,3> const &	Center()	const	{ return _vCenter; }
		
		Vec<T,3> const		Max()		const	{ return _vCenter + _tRadius; }
		Vec<T,3> const		Min()		const	{ return _vCenter - _tRadius; }
	
		bool				Empty()		const	{ return _tRadius <= (T)0; }

		T		*			ptr()				{ return _vCenter.ptr(); }
		T const	*			ptr()		const	{ return _vCenter.ptr(); }
		

		bool	operator == (const TSphere<T> &right) const;
		bool	operator != (const TSphere<T> &right) const;

		Self &	operator += (const Vec<T,3> &right);
		Self &	operator += (const TSphere<T> &right);

		Self &	operator *= (const Matrix<T,4,4> &right);
		Self &	operator *= (const Matrix<T,3,3> &right);

		Self	operator +  (const Vec<T,3> &right) const;
		Self	operator +  (const TSphere<T> &right) const;

		Self	operator *  (const Matrix<T,4,4> &right) const;
		Self	operator *  (const Matrix<T,3,3> &right) const;
		

		void	Add(const Vec<T,3> &vPoint);
		void	Add(const TSphere<T> &sphere);
		
		T		Volume() const;
		T		Depth(const Vec<T,3> &point)	const;
		T		DepthSqr(const Vec<T,3> &point) const;
		
		void	Move(const Vec<T,3> &vDelta);
		void	Scale(const Vec<T,3> &vScale);

		bool	Intersect(const TSphere<T> &sphere) const;
		
		void	Rotate(const Matrix<T,3,3> &sMatrix);
		void	Rotate(const Matrix<T,4,4> &sMatrix);
		void	Transform(const Matrix<T,4,4> &sMatrix);

		void	InterpolateL(const TSphere<T> &s1, const TSphere<T> &s2, T tK);
		void	InterpolateQ(const TSphere<T> &s1, const TSphere<T> &s2, const TSphere<T> &s3, T tK);

		Self &	Set(const Vec<T,3> &center, const T& radius);


		template <typename T2>
		TSphere<T2>	Convert() const;

		String		ToString() const;

		TAABBox<T>	ToAABBox() const;
	};



	template <typename T>
	inline TSphere<T>::TSphere(): _tRadius(-1)
	{}

		
	template <typename T>
	inline TSphere<T>::TSphere(const TSphere<T> &s): _vCenter(s._vCenter), _tRadius(s._tRadius)
	{}

		
	template <typename T>
	inline TSphere<T>::TSphere(const Vec<T,3> &center, const T& radius): _vCenter(center), _tRadius(radius)
	{}
	
		
	template <typename T>
	inline TSphere<T> & TSphere<T>::Set(const Vec<T,3> &center, const T& radius)
	{
		_vCenter	= center;
		_tRadius	= radius;
		return *this;
	}
	
		
	template <typename T>
	inline bool TSphere<T>::operator == (const TSphere<T> &right) const
	{
		return Equals( Radius(), right.Radius() ) and Center() == right.Center();
	}
	

	template <typename T>
	inline bool TSphere<T>::operator != (const TSphere<T> &right) const
	{
		return not ( *this == right ); 
	}

		
	template <typename T>
	inline TSphere<T> & TSphere<T>::operator += (const Vec<T,3> &right)
	{
		Add( right );
		return *this;
	}
	

	template <typename T>
	inline TSphere<T> & TSphere<T>::operator += (const TSphere<T> &right)
	{
		Add( right );
		return *this;
	}

		
	template <typename T>
	inline TSphere<T> & TSphere<T>::operator *= (const Matrix<T,4,4> &right)
	{
		Transform( right );
		return *this;
	}

		
	template <typename T>
	inline TSphere<T> & TSphere<T>::operator *= (const Matrix<T,3,3> &right)
	{
		Rotate( right );
		return *this;
	}

		
	template <typename T>
	inline TSphere<T> TSphere<T>::operator + (const Vec<T,3> &right) const
	{
		return TSphere<T>( *this ) += right;
	}

		
	template <typename T>
	inline TSphere<T> TSphere<T>::operator + (const TSphere<T> &right) const
	{
		return TSphere<T>( *this ) += right;
	}

		
	template <typename T>
	inline TSphere<T> TSphere<T>::operator * (const Matrix<T,4,4> &right) const
	{
		return TSphere<T>( *this ) *= right;
	}

		
	template <typename T>
	inline TSphere<T> TSphere<T>::operator * (const Matrix<T,3,3> &right) const
	{
		return TSphere<T>( *this ) *= right;
	}
	

	template <typename T>
	inline void TSphere<T>::Add(const Vec<T,3> &vPoint)
	{
		T	r = ( vPoint - Center() ).LengthSqr();

		if ( r > Square( Radius() ) )
		{
			r = Sqrt( r );
			_vCenter += ( vPoint - Center() ) * 0.5 * ( 1.0 - Radius() / r );
			_tRadius += ( r - Radius() ) * 0.5;
		}
	}
	

	template <typename T>
	inline void TSphere<T>::Add(const TSphere<T> &sphere)
	{
		T	r = ( sphere.Center() - Center() ).LengthSqr();

		if ( r > Square( Radius() + sphere.Radius() ) )
		{
			r = Sqrt( r );
			_vCenter += ( sphere.Center() - Center() ) * 0.5 * ( 1.0 - Radius() / ( r + sphere.Radius() ) );
			_tRadius += ( ( r + sphere.Radius() ) - Radius() ) * 0.5;
		}
	}

	
	template <typename T>
	inline T TSphere<T>::Volume() const
	{
		return ( T(4) / T(3) * Pi<T> * Pow<3>( Radius() ) );
	}
		
	
	template <typename T>
	inline T TSphere<T>::Depth(const Vec<T,3> &point) const
	{
		return _tRadius - (_vCenter - point).Length();
	}

		
	template <typename T>
	inline T TSphere<T>::DepthSqr(const Vec<T,3> &point) const
	{
		return Square( _tRadius ) - (_vCenter - point).LengthSqr();
	}
		
	
	template <typename T>
	inline void TSphere<T>::Move(const Vec<T,3> &vDelta)
	{
		_vCenter += vDelta;
	}

	
	template <typename T>
	inline void TSphere<T>::Scale(const Vec<T,3> &vScale)
	{
		TAABBox<T>	box = ToAABBox();
		box.Scale( vScale );
		*this = box.ToSphere();
	}

	
	template <typename T>
	inline bool TSphere<T>::Intersect(const TSphere<T> &sphere) const
	{
		return ( Center().DistanceSqr( sphere.Center() ) <= Square( Radius() + sphere.Radius() ) );
	}
	
	
	template <typename T>
	inline void TSphere<T>::Rotate(const Matrix<T,3,3> &sMatrix)
	{
		TAABBox<T>	box = ToAABBox();
		box.Rotate( sMatrix );
		*this = box.ToSphere();
	}
	

	template <typename T>
	inline void TSphere<T>::Rotate(const Matrix<T,4,4> &sMatrix)
	{
		TAABBox<T>	box = ToAABBox();
		box.Rotate( sMatrix );
		*this = box.ToSphere();
	}


	template <typename T>
	inline void TSphere<T>::Transform(const Matrix<T,4,4> &sMatrix)
	{
		TAABBox<T>	box = ToAABBox();
		box.Transform( sMatrix );
		*this = box.ToSphere();
	}


	template <typename T>
	inline void TSphere<T>::InterpolateL(const TSphere<T> &s1, const TSphere<T> &s2, T tK)
	{
		_vCenter.InterpolateL( s1.Center(), s2.Center(), tK );
		_tRadius = Mix( s1.Radius(), s2.Radius(), tK );
	}
	

	template <typename T>
	inline void TSphere<T>::InterpolateQ(const TSphere<T> &s1, const TSphere<T> &s2, const TSphere<T> &s3, T tK)
	{
		_vCenter.InterpolateQ( s1.Center(), s2.Center(), s3.Center(), tK );
		_tRadius = QMix( s1.Radius(), s2.Radius(), s3.Radius(), tK );
	}

		
	template <typename T>
	template <typename T2>
	inline TSphere<T2> TSphere<T>::Convert() const
	{
		return TSphere<T2>( _vCenter.Convert<T2>(), (T2)_tRadius );
	}

		
	template <typename T>
	inline String TSphere<T>::ToString() const
	{
		return (String("center( ") << _vCenter.ToStringFull() << ", radius: " << _tRadius);
	}
	
	
	template <typename T>
	inline TAABBox<T> TSphere<T>::ToAABBox() const
	{
		return TAABBox<T>( Min(), Max() );
	}
	*/

}	// GXMath
}	// GX_STL
