// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "MathTypes3D.h"

namespace GX_STL
{
namespace GXMath
{

	//
	// Transformation
	//

	template <typename T>
	struct Transformation : public CompileTime::CopyQualifiers< T >
	{
		// priority:
		//	move, rotate, scale

	// types
	public:
		typedef Vec<T,3>			Vec_t;
		typedef Quaternion<T>		quat_t;
		typedef Matrix<T,4,4>		mat_t;
		typedef T					Value_t;
		typedef Transformation<T>	Self;
			

	// variables
	private:
		quat_t		_orientation;
		Vec_t		_position;
		//Vec_t		_scale;
		Value_t		_scale;


	// methods
	public:
		Transformation (GX_DEFCTOR) : _scale(T(1)) {}
		Transformation (const Self &tr);
		Transformation (const Vec_t &pos, const quat_t &orient, const T &scale = T(1));
		//Transformation (const Vec_t &pos, const quat_t &orient, const Vec_t &scale = Vec_t(T(1)));
		explicit Transformation (const mat_t &mat);

		Vec_t		 &	Position ()									{ return _position; }
		quat_t		 &	Orientation ()								{ return _orientation; }
		Value_t		 &	GetScale ()									{ return _scale; }

		const Vec_t	 &	Position ()		const						{ return _position; }
		const quat_t &	Orientation ()	const						{ return _orientation; }
		//const Vec_t  &	GetScale ()		const						{ return _scale; }
		Value_t			GetScale ()		const						{ return _scale; }

		Self &	operator += (const Self &right);
		Self	operator +  (const Self &right)	const;
		
		Self &	operator -= (const Self &right);
		Self	operator -  (const Self &right)	const;

		bool	operator == (const Self &right)	const;
		bool	operator != (const Self &right)	const;

		bool	operator >  (const Self &right) const;
		bool	operator <  (const Self &right) const;

		Self &	Move (const Vec_t &delta);
		Self &	Rotate (const quat_t &delta);
		Self &	Scale (const Vec_t &scale);

		Self &	Inverse ();
		Self	Inversed ()	const;

		void	GetMatrix (OUT mat_t &matrix) const;
		void	GetModelMatrix (const Vec_t &pos, OUT mat_t &matrix) const;

		bool	IsZero () const;

		Vec_t	Transform (const Vec_t &point)			const	{ return GetGlobalPosition( point ); }


		// local space to global
		Vec_t	GetGlobalVector (const Vec_t &local)	const;
		Vec_t	GetGlobalPosition (const Vec_t &local)	const;

		// global space to local
		Vec_t	GetLocalVector (const Vec_t &global)	const;
		Vec_t	GetLocalPosition (const Vec_t &global)	const;
		

		template <typename T2>
		Transformation<T2>	Convert() const;
	};

	
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline Transformation<T>::Transformation (const Self &tr) :
		_orientation(tr._orientation), _position(tr._position), _scale(tr._scale)
	{}
	
	template <typename T>
	inline Transformation<T>::Transformation (const Vec<T,3> &pos, const Quaternion<T> &orient, const T &scale) :
		_orientation(orient), _position(pos), _scale(scale)
	{}

	template <typename T>
	inline Transformation<T>::Transformation (const Matrix<T,4,4> &mat) :
		_orientation( mat ), _position( mat.GetTranslation() ), _scale( T(1) )
	{}
	
/*
=================================================
	operator +=
=================================================
*/
	template <typename T>
	inline Transformation<T> & Transformation<T>::operator += (const Self &right)
	{
		_position		+= _orientation * (right._position * _scale);
		_orientation	*= right._orientation;
		_scale			*= right._scale;
		return *this;
	}
	
/*
=================================================
	operator +
=================================================
*/
	template <typename T>
	inline Transformation<T> Transformation<T>::operator + (const Self &right) const
	{
		return Transformation<T>( *this ) += right;
	}
	
/*
=================================================
	operator -=
=================================================
*/
	template <typename T>
	inline Transformation<T> & Transformation<T>::operator -= (const Self &right)
	{
		return (*this) += right.Inversed();
	}
	
/*
=================================================
	operator -
=================================================
*/
	template <typename T>
	inline Transformation<T> Transformation<T>::operator -  (const Self &right)	const
	{
		return Self(*this) -= right;
	}
	
/*
=================================================
	operator ==
=================================================
*/
	template <typename T>
	inline bool Transformation<T>::operator == (const Self &right) const
	{
		return	_orientation	== right._orientation	and
				All( _position	== right._position )	and
				All( _scale		== right._scale );
	}
	
/*
=================================================
	operator !=
=================================================
*/
	template <typename T>
	inline bool Transformation<T>::operator != (const Self &right) const
	{
		return not ( *this == right );
	}
	
/*
=================================================
	operator >
=================================================
*/
	template <typename T>
	inline bool Transformation<T>::operator >  (const Self &right) const
	{
		return	_position.x		!= right._position.x	?	_position.x		> right._position.x		:
				_position.y		!= right._position.y	?	_position.y		> right._position.y		:
				_position.z		!= right._position.z	?	_position.z		> right._position.z		:
				_orientation.x	!= right._orientation.x	?	_orientation.x	> right._orientation.x	:
				_orientation.y	!= right._orientation.y	?	_orientation.y	> right._orientation.y	:
				_orientation.z	!= right._orientation.z	?	_orientation.z	> right._orientation.z	:
				_orientation.w	!= right._orientation.w	?	_orientation.w	> right._orientation.w	:
															_scale			> _scale;

				//_scale.x		!= right._scale.x		?	_scale.x		> right._scale.x		:
				//_scale.y		!= right._scale.y		?	_scale.y		> right._scale.y		:
				//											_scale.z		> right._scale.z;
	}
	
/*
=================================================
	operator <
=================================================
*/
	template <typename T>
	inline bool Transformation<T>::operator <  (const Self &right) const
	{
		return	_position.x		!= right._position.x	?	_position.x		< right._position.x		:
				_position.y		!= right._position.y	?	_position.y		< right._position.y		:
				_position.z		!= right._position.z	?	_position.z		< right._position.z		:
				_orientation.x	!= right._orientation.x	?	_orientation.x	< right._orientation.x	:
				_orientation.y	!= right._orientation.y	?	_orientation.y	< right._orientation.y	:
				_orientation.z	!= right._orientation.z	?	_orientation.z	< right._orientation.z	:
				_orientation.w	!= right._orientation.w	?	_orientation.w	< right._orientation.w	:
															_scale			< _scale;

				//_scale.x		!= right._scale.x		?	_scale.x		< right._scale.x		:
				//_scale.y		!= right._scale.y		?	_scale.y		< right._scale.y		:
				//											_scale.z		< right._scale.z;
	}
	
/*
=================================================
	GetMatrix
=================================================
*/
	template <typename T>
	inline void Transformation<T>::GetMatrix (OUT mat_t &matrix) const
	{
		GetModelMatrix( Vec_t(), matrix );
	}
	
/*
=================================================
	GetModelMatrix
=================================================
*/
	template <typename T>
	inline void Transformation<T>::GetModelMatrix (const Vec_t &cameraPos, OUT mat_t &matrix) const
	{
		matrix = Matrix<T,4,4>::FromQuat( Orientation() );
		matrix.Translation() = _position - cameraPos;
		matrix = matrix * mat_t::Scale( Vec_t( _scale ) );
	}
	
/*
=================================================
	Inverse
=================================================
*/
	template <typename T>
	inline Transformation<T> & Transformation<T>::Inverse ()
	{
		_orientation.Inverse();
		_scale		= T(1) / _scale;
		_position	= _orientation * (-_position * _scale);
		return *this;
	}

	template <typename T>
	inline Transformation<T> Transformation<T>::Inversed () const
	{
		return Transformation( *this ).Inverse();
	}
	
/*
=================================================
	Move
=================================================
*/
	template <typename T>
	inline Transformation<T> & Transformation<T>::Move (const Vec<T,3> &delta)
	{
		_position += _orientation * (delta * _scale);
		return *this;
	}
	
/*
=================================================
	Rotate
=================================================
*/
	template <typename T>
	inline Transformation<T> & Transformation<T>::Rotate (const Quaternion<T> &delta)
	{
		_orientation *= delta;
		return *this;
	}
	
/*
=================================================
	Scale
=================================================
*/
	template <typename T>
	inline Transformation<T> & Transformation<T>::Scale (const Vec_t &scale)
	{
		_scale *= scale;
		return *this;
	}
	
/*
=================================================
	IsZero
=================================================
*/
	template <typename T>
	inline bool Transformation<T>::IsZero () const
	{
		return GXMath::IsZero( _position ) and GXMath::IsZero( _orientation ) and Equals( _scale, T(1) );
	}
	
/*
=================================================
	GetGlobalVector
=================================================
*/
	template <typename T>
	inline Vec<T,3> Transformation<T>::GetGlobalVector (const Vec<T,3> &local) const
	{
		return _orientation * (local * _scale);
	}
	
/*
=================================================
	GetGlobalPosition
=================================================
*/
	template <typename T>
	inline Vec<T,3> Transformation<T>::GetGlobalPosition (const Vec<T,3> &local) const
	{
		return GetGlobalVector( local ) + _position;
	}
	
/*
=================================================
	GetLocalVector
=================================================
*/
	template <typename T>
	inline Vec<T,3> Transformation<T>::GetLocalVector (const Vec<T,3> &global) const
	{
		return (_orientation.Inversed() * global) / _scale;
	}
	
/*
=================================================
	GetLocalPosition
=================================================
*/
	template <typename T>
	inline Vec<T,3> Transformation<T>::GetLocalPosition (const Vec<T,3> &global) const
	{
		return GetLocalVector( global - _position );
	}
	
/*
=================================================
	Convert
=================================================
*/
	template <typename T>
	template <typename T2>
	inline Transformation<T2> Transformation<T>::Convert () const
	{
		return Transformation<T2>( _position.Convert<T2>(), _orientation.Convert<T2>(), T2(_scale) );
	}


}	// GXMath
}	// GX_STL
