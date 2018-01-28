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
		typedef Transformation<T>		transform_t;
		typedef Frustum<T>				frustum_t;
		typedef Quaternion<T>			quat_t;
		typedef Matrix<T,4,4>			mat4_t;

		typedef Vec<T,3>				Vec3_t;
		typedef Vec<T,2>				Vec2_t;

		typedef Radians<T>				radians_t;
		typedef RadiansVec<T,2>			radians2_t;
		typedef RadiansVec<T,3>			radians3_t;

		typedef DegreesVec<T,2>			degrees2_t;
		typedef DegreesVec<T,3>			degrees3_t;

	private:
		struct EChangedFlag {
			enum {
				ViewProjMat = 0,
				Frustum		= 1,
			};
		};


	// variables
	protected:
		transform_t		_transform;
		mat4_t			_projMatrix;
		mat4_t			_viewMatrix;
		mat4_t			_cachedViewProj;
		frustum_t		_frustum;

		radians3_t		_rotation;
		radians3_t		_rotationMinAngle;
		radians3_t		_rotationMaxAngle;
		bool3			_rotationLimitMask;

		Vec2_t			_clipPlanes;			// near, far
		radians_t		_fovY;
		T				_viewAspect;			// width/height
		T				_zoom;
		Bitfield<2>		_changed;				// 1- view proj matrix, 2 - for frustum
		bool			_limitRotationEnabled : 1;
	

	// methods
	protected:
		Vec3_t	&		_Position ()				{ return _transform.Position(); }
		quat_t	&		_Orientation ()				{ return _transform.Orientation(); }
		transform_t	&	_Transform ()				{ return _transform; }
		mat4_t	&		_ProjMatrix ()				{ return _projMatrix; }
		mat4_t	&		_ViewMatrix ()				{ return _viewMatrix; }

		void _Rotate (const radians3_t &rotation);
		void _LimitRotation (radians3_t &rotation) const;

	public:
		PerspectiveCamera (GX_DEFCTOR);
		
		radians3_t const &	Rotation ()			const	{ return _rotation; }
		radians3_t const &	RotationMinAngle ()	const	{ return _rotationMinAngle; }
		radians3_t const &	RotationMaxAngle ()	const	{ return _rotationMaxAngle; }

		Vec3_t const &		Position ()			const	{ return _transform.Position(); }
		quat_t const &		Orientation ()		const	{ return _transform.Orientation(); }
		mat4_t const &		ViewMatrix ()		const	{ return _viewMatrix; }
		transform_t const &	Transform ()		const	{ return _transform; }
		mat4_t const &		ProjMatrix ()		const	{ return _projMatrix; }
		
		frustum_t const &	Frustum ();
		mat4_t const &		ViewProjMatrix ();

		mat4_t const		GetModelMatrix ()	const;

		Vec3_t	const		GetAxisX ()			const	{ return Vec3_t( _viewMatrix(0,0), _viewMatrix(1,0), _viewMatrix(2,0) ); }
		Vec3_t	const		GetAxisY ()			const	{ return Vec3_t( _viewMatrix(0,1), _viewMatrix(1,1), _viewMatrix(2,1) ); }
		Vec3_t	const		GetAxisZ ()			const	{ return Vec3_t( _viewMatrix(0,2), _viewMatrix(1,2), _viewMatrix(2,2) ); }

		Vec2_t const &		ClipPlanes ()		const	{ return _clipPlanes; }
		T					GetAspect ()		const	{ return _viewAspect; }

		radians_t			GetDefaultFovY()	const	{ return _fovY; }
		radians_t			GetFovY ()			const	{ return _fovY * _zoom; }
		radians2_t			GetFOVs ()			const	{ return radians2_t( _fovY / _viewAspect, _fovY ) * _zoom; }
		T					GetZoom ()			const	{ return _zoom; }

		T					VisibilityRange ()	const	{ return _clipPlanes.y - _clipPlanes.x; }

		void Create (const transform_t &transform, radians_t fovY, T viewAspect, const Vec2_t &clipPlanes);
		void Create (const transform_t &transform, const Vec2_t &viewSize, T distanceInMeters, const Vec2_t &clipPlanes);
	
		void SetClipPlanes (const Vec2_t &clipPlanes);
		void Recalculate ();

		void Resize (radians_t fovY, T viewAspect);
		void Resize (const Vec2_t &viewSize, T distanceInMeters);

		void Increase (T newZoom = T(1));

		void LimitRotation (bool enabled, const radians3_t &minAngle = radians3_t(), const radians3_t &maxAngle = radians3_t());
		void LimitRotation (bool enabled, const degrees3_t &minAngle = degrees3_t(), const degrees3_t &maxAngle = degrees3_t());


		// FPS Camera //
		void RotateFPS (const radians2_t &rotation);
		void RotateFPS (const degrees2_t &rotation);
		void MoveFPS (const Vec3_t &delta);
		void TransformFPS (const Vec3_t &delta, Vec3_t &pos);

		// Free FPS Camera //
		void MoveFPSFree (const Vec3_t &delta);
		void TransformFPSFree (const Vec3_t &delta, Vec3_t &pos);

		// Flight Camera //
		void RotateFlight (const radians3_t &rotation);
		void RotateFlight (const degrees3_t &rotation);
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
		_fovY( T(0.5) ),				_viewAspect( T(1) ),
		_clipPlanes( T(1), T(100) ),	_zoom( T(1) ),
		_limitRotationEnabled( false )
	{
		_changed.SetAll();

		_projMatrix		= mat4_t::Identity();
		_viewMatrix		= mat4_t::Identity();
		_cachedViewProj	= mat4_t::Identity();
	}
	
/*
=================================================
	GetModelMatrix
=================================================
*/
	template <typename T>
	inline CHECKRES(Matrix<T,4,4> const)  PerspectiveCamera<T>::GetModelMatrix () const
	{
		return mat4_t::Translate( -Position() );
	}
	
/*
=================================================
	Frustum
=================================================
*/
	template <typename T>
	inline CHECKRES(Frustum<T> const &)  PerspectiveCamera<T>::Frustum ()
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
	inline CHECKRES(Matrix<T,4,4> const &)  PerspectiveCamera<T>::ViewProjMatrix ()
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
	inline void PerspectiveCamera<T>::Create (const transform_t &transform, radians_t fovY, T viewAspect, const Vec2_t &clipPlanes)
	{
		_zoom		= T(1);
		_transform	= transform;
		_clipPlanes	= clipPlanes;
		_viewMatrix	= mat4_t::FromQuat( transform.Orientation() );
		Resize( fovY, viewAspect );
	}
	
/*
=================================================
	Create
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::Create (const transform_t &transform, const Vec2_t &viewSize, T distanceInMeters, const Vec2_t &clipPlanes)
	{
		_zoom		= T(1);
		_transform	= transform;
		_clipPlanes	= clipPlanes;
		_viewMatrix	= mat4_t::FromQuat( transform.Orientation() );
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
		_projMatrix = mat4_t::BuildPerspective( Clamp( _fovY / _zoom, T(0.1), T(170) ), _viewAspect, _clipPlanes );
	}
	
/*
=================================================
	Resize
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::Resize (radians_t fovY, T viewAspect)
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
	inline void PerspectiveCamera<T>::LimitRotation (bool enabled, const radians3_t &minAngle, const radians3_t &maxAngle)
	{
		_limitRotationEnabled	= enabled;
		_rotationMinAngle		= minAngle;
		_rotationMaxAngle		= maxAngle;
		_rotationLimitMask		= minAngle == maxAngle;
	}
	
	template <typename T>
	inline void PerspectiveCamera<T>::LimitRotation (bool enabled, const degrees3_t &minAngle, const degrees3_t &maxAngle)
	{
		LimitRotation( enabled, minAngle.template To< radians3_t >(), maxAngle.template To< radians3_t >() );
	}
	
/*
=================================================
	_LimitRotation
=================================================
*/
	template <typename T>
	inline void PerspectiveCamera<T>::_LimitRotation (OUT radians3_t &rotation) const
	{
		if ( _limitRotationEnabled )
		{
			radians3_t	last_rot = _rotation + rotation;

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
	inline void PerspectiveCamera<T>::_Rotate (const radians3_t &rotation)
	{
		const radians_t	max_angle = radians_t::Pi() * T(2);

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
	inline void PerspectiveCamera<T>::RotateFPS (const radians2_t &rotation)
	{
		if ( IsZero( rotation ) )
			return;

		quat_t&		q		= _Orientation();
		radians3_t	rot		= radians3_t( rotation, radians_t() );
		bool		changed	= false;

		_LimitRotation( rot );
		
		if ( IsNotZero( rot.y ) ) {
			q = quat_t().RotationX( rot.y ) * q;
			changed = true;
		}

		if ( IsNotZero( rot.x ) ) {
			q = q * quat_t().RotationY( rot.x );
			changed = true;
		}

		if ( changed )
		{
			_Rotate( rot );
			_changed.SetAll();

			q.Normalize();
			_viewMatrix = mat4_t::FromQuat( q );
		}
	}
	
	template <typename T>
	inline void PerspectiveCamera<T>::RotateFPS (const degrees2_t &rotation)
	{
		RotateFPS( rotation.template To< radians2_t >() );
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
	inline void PerspectiveCamera<T>::RotateFlight (const radians3_t &rotation)
	{
		if ( IsZero( rotation ) )
			return;
		
		radians3_t	rot = rotation;
		_LimitRotation( rot );

		quat_t & q = _Orientation();
		q = q * quat_t().Set( rot.y, rot.x, rot.z );
	
		_Rotate( rot );
		_changed.SetAll();

		q.Normalize();
		_viewMatrix = mat4_t::FromQuat( q );
	}
	
	template <typename T>
	inline void PerspectiveCamera<T>::RotateFlight (const degrees3_t &rotation)
	{
		RotateFlight( rotation.template To< radians3_t >() );
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
