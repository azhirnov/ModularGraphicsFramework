// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/Platforms/Shared/ScreenCoords.h"
#include "Engine/Platforms/Common/OSEnums.h"

namespace Engine
{
namespace Platforms
{

	//
	// Display
	//
	
	struct Display
	{
		friend struct DisplayEditor;

	// types
	public:
		using DipCoord		= ScreenCoords::Dip;
		using PhysicsCoord	= ScreenCoords::Physics;
		using PixelsCoord	= ScreenCoords::Pixels;
		using SNormCoord	= ScreenCoords::SNorm;
		using UNormCoord	= ScreenCoords::UNorm;
		using EOrientation	= EDisplayOrientation::type;


	// variables
	protected:
		PixelsCoord		_res;		// screen resolution in pixels
		PhysicsCoord	_size;		// physical size in meters
		float			_ppi;		// pixels per inch
		uint			_freq;		// update frequency, 0 - default
		EOrientation	_orient;


	// methods
	public:
		Display () :
			_ppi(96.0f), _freq(0), _orient(EDisplayOrientation::Default)
		{}

		Display (const uint2 &resolution, float ppi, uint frequency, EOrientation orient) :
			_res(int2(resolution)), _ppi(ppi), _freq(frequency), _orient(orient)
		{
			_size	= CalcPhysicsSize();
		}

		void SetResolution (const uint2 &res)					{ _res = PixelsCoord( int2(res) ); }
		void SetPhysicsSize (const float2 &size)				{ _size = PhysicsCoord( size ); }
		void SetPixelsPerInch (float ppi)						{ _ppi = ppi; }
		void SetPixelsPerMilimeter (float ppmm)					{ _ppi = ppmm / Square( _MilimetersInInch() ); }
		void SetOrientation (EOrientation orient)				{ _orient = orient; }
		void SetFrequency (uint freq)							{ _freq = freq; }

		uint2			Resolution ()					const	{ return uint2(_res.Get()); }
		float2			PhysicsSize ()					const	{ return _size.Get(); }
		EOrientation	Orientation ()					const	{ return _orient; }
		float			PixelsPerInch ()				const	{ return _ppi; }
		float			AspectRatio ()					const	{ return _res.x / (float)_res.y; }
		uint			Frequency ()					const	{ return _freq; }

		bool			IsHorizontal ()					const	{ return _res.x > _res.y; }
		bool			IsVertical ()					const	{ return not IsHorizontal(); }

		// calculation
		PhysicsCoord	CalcPhysicsSize ()				const	{ return PhysicsCoord( _res.Get().To<float2>() / _ppi * _MilimetersInInch() ); }
		float2			CalcPixelsPerMilimeter ()		const	{ return float2( _res.x / _size.x, _res.y / _size.y ); }
		float2			CalcPixelsPerInch ()			const	{ return CalcPixelsPerMilimeter() / Square( _MilimetersInInch() ); }
		DipCoord		CalcDip ()						const	{ return ToDip( _res ); }

		// converter
		DipCoord		ToDip (const PixelsCoord &c)	const	{ return DipCoord( c.Get().To<float2>() * _DipToPixel() / _ppi ); }
		DipCoord		ToDip (const PhysicsCoord &c)	const	{ return ToDip( ToPixels( c ) ); }
		DipCoord		ToDip (const SNormCoord &c)		const	{ return ToDip( ToPixels( c ) ); }
		DipCoord		ToDip (const UNormCoord &c)		const	{ return ToDip( ToPixels( c ) ); }

		PhysicsCoord	ToPhysics (const DipCoord &c)	const	{ return ToPhysics( ToUNorm( c ) ); }
		PhysicsCoord	ToPhysics (const PixelsCoord &c)const	{ return ToPhysics( ToUNorm( c ) ); }
		PhysicsCoord	ToPhysics (const SNormCoord &c)	const	{ return ToPhysics( UNormCoord( c ) ); }
		PhysicsCoord	ToPhysics (const UNormCoord &c)	const	{ return PhysicsCoord( c.Get() * _size.Get() ); }

		PixelsCoord		ToPixels (const DipCoord &c)	const	{ return PixelsCoord( RoundToInt( c.Get() * _ppi / _DipToPixel() ) ); }
		PixelsCoord		ToPixels (const PhysicsCoord &c)const	{ return ToPixels( ToUNorm( c ) ); }
		PixelsCoord		ToPixels (const SNormCoord &c)	const	{ return ToPixels( UNormCoord( c ) ); }
		PixelsCoord		ToPixels (const UNormCoord &c)	const	{ return PixelsCoord( RoundToInt( c.Get() * _res.Get().To<float2>() ) ); }

		UNormCoord		ToUNorm (const DipCoord &c)		const	{ return ToUNorm( ToPixels( c ) ); }
		UNormCoord		ToUNorm (const PhysicsCoord &c)	const	{ return UNormCoord( c.Get() / _size.Get() ); }
		UNormCoord		ToUNorm (const PixelsCoord &c)	const	{ return UNormCoord( c.Get().To<float2>() / _res.Get().To<float2>() ); }
		UNormCoord		ToUNorm (const SNormCoord &c)	const	{ return UNormCoord( c ); }

		SNormCoord		ToSNorm (const DipCoord &c)		const	{ return SNormCoord( ToUNorm( c ) ); }
		SNormCoord		ToSNorm (const PhysicsCoord &c)	const	{ return SNormCoord( ToUNorm( c ) ); }
		SNormCoord		ToSNorm (const PixelsCoord &c)	const	{ return SNormCoord( ToUNorm( c ) ); }
		SNormCoord		ToSNorm (const UNormCoord &c)	const	{ return SNormCoord( c ); }

		PixelsCoord		Clamp (const PixelsCoord &c)	const	{ return PixelsCoord(	GXMath::Clamp( c.Get(), int2(0),		_res.Get()		) ); }
		PhysicsCoord	Clamp (const PhysicsCoord &c)	const	{ return PhysicsCoord(	GXMath::Clamp( c.Get(), float2(0),		_size.Get()		) ); }
		DipCoord		Clamp (const DipCoord &c)		const	{ return DipCoord(		GXMath::Clamp( c.Get(), float2(0),		CalcDip().Get()	) ); }
		SNormCoord		Clamp (const SNormCoord &c)		const	{ return SNormCoord(	GXMath::Clamp( c.Get(), float2(-1.0f),	float2(1.0f)		) ); }
		UNormCoord		Clamp (const UNormCoord &c)		const	{ return UNormCoord(	GXMath::Clamp( c.Get(), float2(0),		float2(1.0f)		) ); }

	private:
		static float	_MilimetersInInch ()					{ return 25.4f; }
		static float	_DipToPixel ()							{ return 160.0f; }
	};



	//
	// Display Emulator
	//

	struct DisplayEmulator : Display
	{
		friend struct DisplayEditor;

	// variables
	private:
		Display		_realDisplay;
		float2		_scale;


	// methods
	public:
		DisplayEmulator () : _scale(1.0f)
		{}

		int2			RealResolution ()	const	{ return _realDisplay.ToPixels( DipCoord( _ppi * _scale ) ).Get(); }

		Display const &	RealDisplay ()		const	{ return _realDisplay; }

		Display &		RealDisplay ()				{ return _realDisplay; }
	};


}	// Platforms
}	// Engine
