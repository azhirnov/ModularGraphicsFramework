// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Transform.h"
#include "Frustum.h"

namespace GX_STL
{
namespace GXMath
{

	//
	// Perspective Camera
	//

	template <typename T>
	struct PerspectiveCamera : public CompileTime::CopyQualifiers<T>, public CompileTime::MustBeFloat<T>
	{
	// types
	public:
		typedef PerspectiveCamera<T>	Self;
		typedef Transformation<T>		Transform_t;
		typedef Frustum<T>				Frustum_t;
		typedef Quaternion<T>			Quat_t;
		typedef Matrix<T,4,4>			Mat4_t;

		typedef Vec<T,3>				Vec3_t;
		typedef Vec<T,2>				Vec2_t;

		typedef Radians<T>				Radians_t;
		typedef RadiansVec<T,2>			Radians2_t;
		typedef RadiansVec<T,3>			Radians3_t;

		typedef DegreesVec<T,2>			Degrees2_t;
		typedef DegreesVec<T,3>			Degrees3_t;

	private:
		struct EChangedFlag {
			enum {
				ViewProjMat = 0,
				Frustum		= 1,
			};
		};


	// variables
	protected:
		Transform_t		_transform;
		Mat4_t			_projMatrix;
		Mat4_t			_viewMatrix;
		Mat4_t			_cachedViewProj;
		Frustum_t		_frustum;

		Radians3_t		_rotation;
		Radians3_t		_rotationMinAngle;
		Radians3_t		_rotationMaxAngle;
		bool3			_rotationLimitMask;

		Vec2_t			_clipPlanes;			// near, far
		Radians_t		_fovY;
		T				_viewAspect;			// width/height
		T				_zoom;
		Bitfield<2>		_changed;				// 1- view proj matrix, 2 - for frustum
		bool			_limitRotationEnabled : 1;
	

	// methods
	protected:
		Vec3_t	&		_Position ()				{ return _transform.Position(); }
		Quat_t	&		_Orientation ()				{ return _transform.Orientation(); }
		Transform_t	&	_Transform ()				{ return _transform; }
		Mat4_t	&		_ProjMatrix ()				{ return _projMatrix; }
		Mat4_t	&		_ViewMatrix ()				{ return _viewMatrix; }

		void _Rotate (const Radians3_t &rotation);
		void _LimitRotation (Radians3_t &rotation) const;

	public:
		PerspectiveCamera (GX_DEFCTOR);
		
		Radians3_t const &	Rotation ()			const	{ return _rotation; }
		Radians3_t const &	RotationMinAngle ()	const	{ return _rotationMinAngle; }
		Radians3_t const &	RotationMaxAngle ()	const	{ return _rotationMaxAngle; }

		Vec3_t const &		Position ()			const	{ return _transform.Position(); }
		Quat_t const &		Orientation ()		const	{ return _transform.Orientation(); }
		Mat4_t const &		ViewMatrix ()		const	{ return _viewMatrix; }
		Transform_t const &	Transform ()		const	{ return _transform; }
		Mat4_t const &		ProjMatrix ()		const	{ return _projMatrix; }
		
		Frustum_t const &	GetFrustum ();
		Mat4_t const &		ViewProjMatrix ();

		Mat4_t const		GetModelMatrix ()	const;

		Vec3_t	const		GetAxisX ()			const	{ return Vec3_t( _viewMatrix(0,0), _viewMatrix(1,0), _viewMatrix(2,0) ); }
		Vec3_t	const		GetAxisY ()			const	{ return Vec3_t( _viewMatrix(0,1), _viewMatrix(1,1), _viewMatrix(2,1) ); }
		Vec3_t	const		GetAxisZ ()			const	{ return Vec3_t( _viewMatrix(0,2), _viewMatrix(1,2), _viewMatrix(2,2) ); }

		Vec2_t const &		ClipPlanes ()		const	{ return _clipPlanes; }
		T					GetAspect ()		const	{ return _viewAspect; }

		Radians_t			GetDefaultFovY()	const	{ return _fovY; }
		Radians_t			GetFovY ()			const	{ return _fovY * _zoom; }
		Radians2_t			GetFOVs ()			const	{ return Radians2_t( _fovY / _viewAspect, _fovY ) * _zoom; }
		T					GetZoom ()			const	{ return _zoom; }

		T					VisibilityRange ()	const	{ return _clipPlanes.y - _clipPlanes.x; }

		void Create (const Transform_t &transform, Radians_t fovY, T viewAspect, const Vec2_t &clipPlanes);
		void Create (const Transform_t &transform, const Vec2_t &viewSize, T distanceInMeters, const Vec2_t &clipPlanes);
	
		void SetClipPlanes (const Vec2_t &clipPlanes);
		void Recalculate ();

		void Resize (Radians_t fovY, T viewAspect);
		void Resize (const Vec2_t &viewSize, T distanceInMeters);

		void Increase (T newZoom = T(1));

		void LimitRotation (bool enabled, const Radians3_t &minAngle = Radians3_t(), const Radians3_t &maxAngle = Radians3_t());
		void LimitRotation (bool enabled, const Degrees3_t &minAngle = Degrees3_t(), const Degrees3_t &maxAngle = Degrees3_t());


		// FPS Camera //
		void RotateFPS (const Radians2_t &rotation);
		void RotateFPS (const Degrees2_t &rotation);
		void MoveFPS (const Vec3_t &delta);
		void TransformFPS (const Vec3_t &delta, Vec3_t &pos);

		// Free FPS Camera //
		void MoveFPSFree (const Vec3_t &delta);
		void TransformFPSFree (const Vec3_t &delta, Vec3_t &pos);

		// Flight Camera //
		void RotateFlight (const Radians3_t &rotation);
		void RotateFlight (const Degrees3_t &rotation);
		void MoveFlight (const Vec3_t &delta);
		void TransformFlight (const Vec3_t &delta, Vec3_t &pos);

		// Other //
		void MoveToPosition (const Vec3_t &pos);


		static const Vec3_t UpDir ()		{ return Vec3_t( T(0), T(1), T(0) ); }
		static const Vec3_t RightDir ()		{ return Vec3_t( T(1), T(0), T(0) ); }
		static const Vec3_t ForwardDir ()	{ return Vec3_t( T(0), T(0), T(1) ); }
	};

	

	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline PerspectiveCamera<T>::PerspectiveCamera (UninitializedType):
		_transform(),					_projMatrix(),
		_clipPlanes( T(1), T(100) ),	_fovY( T(0.5) ),
		_viewAspect( T(1) ),			_zoom( T(1) ),
		_limitRotationEnabled( false )
	{
		_changed.SetAll();

		_projMatrix		= Mat4_t::Identity();
		_viewMatrix		= Mat4_t::Identity();
		_cachedViewProj	= Mat4_t::Identity();
	}
	
/*
=================================================
	GetModelMatrix
=================================================
*/
	template <typename T>
	CHECKRES inline Matrix<T,4,4> const  PerspectiveCamera<T>::GetModelMatrix () const
	{
		return Mat4_t::Translate( -Position() );
	}
	
/*
=================================================
	GetFrustum
=================================================
*/
	template <typename T>
	CHECKRES inline Frustum<T> const &  PerspectiveCamera<T>::GetFrustum ()
	{
		if ( _changed[ EChangedFlag::Frustum ] )
		{
			_frustum.Setup( ViewProjMatrix() );
			_changed.Reset( EChangedFlag::Frustum );
		}

		return _frustum;
	}
	
/*
=================================================
	ViewProjMatrix
=================================================
*/
	template <typename T>
	CHECKRES inline Matrix<T,4,4> const &  PerspectiveCamera<T>::ViewProjMatrix ()
	{
		if ( _changed[ EChangedFlag::ViewProjMat ] )
		{
			_cachedViewProj = _projMatrix * _viewMatrix;
			_changed.Reset( EChangedFlag::ViewProjMat );
		}

		return _cachedViewProj;
	}
	
/*
=================================================
	Create
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::Create (const Transform_t &transform, Radians_t fovY, T viewAspect, const Vec2_t &clipPlanes)
	{
		_zoom		= T(1);
		_transform	= transform;
		_clipPlanes	= clipPlanes;
		_viewMatrix	= Mat4_t::FromQuat( transform.Orientation() );
		Resize( fovY, viewAspect );
	}
	
/*
=================================================
	Create
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::Create (const Transform_t &transform, const Vec2_t &viewSize, T distanceInMeters, const Vec2_t &clipPlanes)
	{
		_zoom		= T(1);
		_transform	= transform;
		_clipPlanes	= clipPlanes;
		_viewMatrix	= Mat4_t::FromQuat( transform.Orientation() );
		Resize( viewSize, distanceInMeters );
	}
	
/*
=================================================
	SetClipPlanes
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::SetClipPlanes (const Vec2_t &clipPlanes)
	{
		_clipPlanes = clipPlanes;

		_changed.SetAll();
		Recalculate();
	}
	
/*
=================================================
	Recalculate
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::Recalculate ()
	{
		_projMatrix = Mat4_t::BuildPerspective( Clamp( _fovY / _zoom, T(0.1), T(170) ), _viewAspect, _clipPlanes );
	}
	
/*
=================================================
	Resize
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::Resize (Radians_t fovY, T viewAspect)
	{
		_fovY		 = fovY;
		_viewAspect	 = viewAspect;

		_changed.SetAll();
		Recalculate();
	}
	
/*
=================================================
	Resize
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::Resize (const Vec2_t &viewSize, T distanceInMeters)
	{
		_fovY		 = 2 * ATan( viewSize.y / (2 * distanceInMeters) );
		_viewAspect	 = viewSize.x / viewSize.y;

		_changed.SetAll();
		Recalculate();
	}
	
/*
=================================================
	Increase
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::Increase (T newZoom)
	{
		if ( not Equals( newZoom, _zoom ) )
		{
			_zoom = newZoom;

			_changed.SetAll();
			Recalculate();
		}
	}
	
/*
=================================================
	LimitRotation
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::LimitRotation (bool enabled, const Radians3_t &minAngle, const Radians3_t &maxAngle)
	{
		_limitRotationEnabled	= enabled;
		_rotationMinAngle		= minAngle;
		_rotationMaxAngle		= maxAngle;
		_rotationLimitMask		= minAngle == maxAngle;
	}
	
	template <typename T>
	inline void PerspectiveCamera<T>::LimitRotation (bool enabled, const Degrees3_t &minAngle, const Degrees3_t &maxAngle)
	{
		LimitRotation( enabled, minAngle.template To< Radians3_t >(), maxAngle.template To< Radians3_t >() );
	}
	
/*
=================================================
	_LimitRotation
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::_LimitRotation (OUT Radians3_t &rotation) const
	{
		if ( _limitRotationEnabled )
		{
			Radians3_t	last_rot = _rotation + rotation;

			if ( _rotationLimitMask.x )	rotation.x = Clamp( last_rot.x, _rotationMinAngle.x, _rotationMaxAngle.x ) - _rotation.x;
			if ( _rotationLimitMask.y )	rotation.y = Clamp( last_rot.y, _rotationMinAngle.y, _rotationMaxAngle.y ) - _rotation.y;
			if ( _rotationLimitMask.z ) rotation.z = Clamp( last_rot.z, _rotationMinAngle.z, _rotationMaxAngle.z ) - _rotation.z;
		}
	}
	
/*
=================================================
	_Rotate
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::_Rotate (const Radians3_t &rotation)
	{
		const Radians_t	max_angle = Radians_t::Pi() * T(2);

		_rotation += rotation;

		if ( Abs( _rotation.x ) > max_angle )	_rotation.x -= CopySign( _rotation.x, max_angle );
		if ( Abs( _rotation.y ) > max_angle )	_rotation.y -= CopySign( _rotation.y, max_angle );
		if ( Abs( _rotation.z ) > max_angle )	_rotation.z -= CopySign( _rotation.z, max_angle );
	}
	
/*
=================================================
	RotateFPS
----
	x - horizontal
	y - vertical
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::RotateFPS (const Radians2_t &rotation)
	{
		if ( IsZero( rotation ) )
			return;

		Quat_t&		q		= _Orientation();
		Radians3_t	rot		= Radians3_t( rotation, Radians_t() );
		bool		changed	= false;

		_LimitRotation( rot );
		
		if ( IsNotZero( rot.y ) ) {
			q = Quat_t().RotationX( rot.y ) * q;
			changed = true;
		}

		if ( IsNotZero( rot.x ) ) {
			q = q * Quat_t().RotationY( rot.x );
			changed = true;
		}

		if ( changed )
		{
			_Rotate( rot );
			_changed.SetAll();

			q.Normalize();
			_viewMatrix = Mat4_t::FromQuat( q );
		}
	}
	
	template <typename T>
	inline void PerspectiveCamera<T>::RotateFPS (const Degrees2_t &rotation)
	{
		RotateFPS( rotation.template To< Radians2_t >() );
	}
	
/*
=================================================
	TransformFPS
----
	x - forward/backward
	y - side
	z - up/down
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::TransformFPS (const Vec3_t &delta, Vec3_t &pos)
	{
		Vec3_t const	forwards = Cross( UpDir(), GetAxisX() ).Normalized();
		
		pos += forwards   * delta.x;
		pos += GetAxisX() * delta.y;
		pos += UpDir()    * delta.z;
	}
	
/*
=================================================
	MoveFPS
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::MoveFPS (const Vec3_t &delta)
	{
		TransformFPS( delta, _Position() );
	}
	
/*
=================================================
	TransformFPSFree
----
	x - forward/backward
	y - side
	z - up/down
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::TransformFPSFree (const Vec3_t &delta, Vec3_t &pos)
	{
		pos += GetAxisZ() * -delta.x;
		pos += GetAxisX() *  delta.y;
		pos += UpDir()    *  delta.z;
	}
	
/*
=================================================
	MoveFPSFree
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::MoveFPSFree (const Vec3_t &delta)
	{
		TransformFPSFree( delta, _Position() );
	}
	
/*
=================================================
	RotateFlight
----
	x - roll
	y - pitch
	z - yaw
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::RotateFlight (const Radians3_t &rotation)
	{
		if ( IsZero( rotation ) )
			return;
		
		Radians3_t	rot = rotation;
		_LimitRotation( rot );

		Quat_t & q = _Orientation();
		q = q * Quat_t().Set( rot.y, rot.x, rot.z );
	
		_Rotate( rot );
		_changed.SetAll();

		q.Normalize();
		_viewMatrix = Mat4_t::FromQuat( q );
	}
	
	template <typename T>
	inline void PerspectiveCamera<T>::RotateFlight (const Degrees3_t &rotation)
	{
		RotateFlight( rotation.template To< Radians3_t >() );
	}
	
/*
=================================================
	TransformFlight
----
	x - forward/backward
	y - side
	z - up/down
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::TransformFlight (const Vec3_t &delta, Vec3_t &pos)
	{
		pos += GetAxisX() *  delta.y;
		pos += UpDir()    *  delta.z;
		pos += GetAxisZ() * -delta.x;
	}
	
	template <typename T>
	inline void PerspectiveCamera<T>::MoveFlight (const Vec3_t &delta)
	{
		TransformFlight( delta, _Position() );
	}
	
/*
=================================================
	MoveToPosition
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::MoveToPosition (const Vec3_t &pos)
	{
		_Position() = pos;
	}


}	// GXMath
}	// GX_STL
