// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/OS/ScreenCoords.h"
#include "Engine/Platforms/Shared/OS/OSEnums.h"

namespace Engine
{
namespace Platforms
{

	//
	// Display
	//
	
	struct Display : CompileTime::FastCopyable
	{
		friend struct DisplayEditor;

	// types
	public:
		using DipCoord		= ScreenCoords::dipsf2;
		using PhysicalCoord	= ScreenCoords::metersf2;
		using PixelsCoord	= ScreenCoords::pixelsi2;
		using SNormCoord	= ScreenCoords::snormf2;
		using UNormCoord	= ScreenCoords::unormf2;

		using PixelsRect	= ScreenCoords::pixelsri;
		using DipsRect		= ScreenCoords::dipsrf;
		using SNormRect		= ScreenCoords::snormrectf;
		using UNormRect		= ScreenCoords::unormrectf;

		using EOrientation	= EDisplayOrientation::type;

		enum DisplayID_t : ulong {};


	// variables
	protected:
		DisplayID_t		_id;

		PixelsRect		_workRect;	// area available for window
		PixelsRect		_rect;		// area available for fullscreen window only

		PhysicalCoord	_size;		// physical size in meters
		float2			_ppi;		// pixels per inch
		uint			_freq;		// update frequency, 0 - default
		EOrientation	_orient;


	// methods
	public:
		Display () :
			_id(DisplayID_t(0)), _ppi(96.0f), _freq(0), _orient(EDisplayOrientation::Default)
		{}

		Display (ulong id, const RectI &workArea, const RectI &area, float ppi, uint frequency, EOrientation orient) :
			_id(DisplayID_t(id)), _workRect( workArea.To<PixelsRect>() ), _rect( area.To<PixelsRect>() ),
			_ppi(ppi), _freq(frequency), _orient(orient)
		{
			_size	= CalcPhysicalSize();
		}

		void SetWorkArea (const RectI &value)					{ _workRect = value.To<PixelsRect>(); }
		void SetFullArea (const RectI &value)					{ _rect = value.To<PixelsRect>(); }
		void SetPhysicalSize (const float2 &value)				{ _size = value.To<PhysicalCoord>(); }
		void SetPixelsPerInch (const float2 &value)				{ _ppi = value; }
		void SetPixelsPerMilimeter (const float2 &value)		{ _ppi = value / _MilimetersInInch(); }
		void SetOrientation (EOrientation value)				{ _orient = value; }
		void SetFrequency (uint value)							{ _freq = value; }

		uint2			Resolution ()					const	{ return FullArea().Size().To<uint2>(); }
		float2			PhysicalSize ()					const	{ return _size.To<float2>(); }
		EOrientation	Orientation ()					const	{ return _orient; }
		float2			PixelsPerInch ()				const	{ return _ppi; }
		float			AspectRatio ()					const	{ return _rect.Width() / float(_rect.Height()); }
		uint			Frequency ()					const	{ return _freq; }

		DisplayID_t		DisplayID ()					const	{ return _id; }

		RectI			WorkArea ()						const	{ return _workRect.To<RectI>(); }
		RectI			FullArea ()						const	{ return _rect.To<RectI>(); }

		bool			IsHorizontal ()					const	{ return _rect.Width() > _rect.Height(); }
		bool			IsVertical ()					const	{ return not IsHorizontal(); }

		// calculation
		PhysicalCoord	CalcPhysicalSize ()				const	{ return (_rect.Size().To<float2>() / _ppi * _MetersInInch()).To<PhysicalCoord>(); }
		float2			CalcPixelsPerMilimeter ()		const	{ return _rect.Size().To<float2>() / (_size.To<float2>() * 1000.0f); }
		float2			CalcPixelsPerInch ()			const	{ return _rect.Size().To<float2>() / _size.To<float2>() * _MetersInInch(); }
		DipCoord		CalcDip ()						const	{ return ToDip( _rect.Size() ); }

		// converter
		DipCoord		ToDip (const PixelsCoord &c)	const	{ return (c.To<float2>() * _DipToPixel() / _ppi).To<DipCoord>(); }
		DipCoord		ToDip (const PhysicalCoord &c)	const	{ return ToDip( ToPixels( c ) ); }
		DipCoord		ToDip (const SNormCoord &c)		const	{ return ToDip( ToPixels( c ) ); }
		DipCoord		ToDip (const UNormCoord &c)		const	{ return ToDip( ToPixels( c ) ); }

		PhysicalCoord	ToPhysical (const DipCoord &c)	const	{ return ToPhysical( ToUNorm( c ) ); }
		PhysicalCoord	ToPhysical (const PixelsCoord &c)const	{ return ToPhysical( ToUNorm( c ) ); }
		PhysicalCoord	ToPhysical (const SNormCoord &c)const	{ return ToPhysical( ToUNorm( c ) ); }
		PhysicalCoord	ToPhysical (const UNormCoord &c)const	{ return (c.To<float2>() * _size.To<float2>()).To<PhysicalCoord>(); }

		PixelsCoord		ToPixels (const DipCoord &c)	const	{ return RoundToInt( c.To<float2>() * _ppi / _DipToPixel() ).To<PixelsCoord>(); }
		PixelsCoord		ToPixels (const PhysicalCoord &c)const	{ return ToPixels( ToUNorm( c ) ); }
		PixelsCoord		ToPixels (const SNormCoord &c)	const	{ return ToPixels( ToUNorm( c ) ); }
		PixelsCoord		ToPixels (const UNormCoord &c)	const	{ return RoundToInt( c.To<float2>() * _rect.Size().To<float2>() ).To<PixelsCoord>(); }

		UNormCoord		ToUNorm (const DipCoord &c)		const	{ return ToUNorm( ToPixels( c ) ); }
		UNormCoord		ToUNorm (const PhysicalCoord &c)const	{ return (c.To<float2>() / _size.To<float2>()).To<UNormCoord>(); }
		UNormCoord		ToUNorm (const PixelsCoord &c)	const	{ return (c.To<float2>() / _rect.Size().To<float2>()).To<UNormCoord>(); }
		UNormCoord		ToUNorm (const SNormCoord &c)	const	{ return TypeCast::ToUNorm( c ); }

		SNormCoord		ToSNorm (const DipCoord &c)		const	{ return TypeCast::ToSNorm( ToUNorm( c ) ); }
		SNormCoord		ToSNorm (const PhysicalCoord &c)const	{ return TypeCast::ToSNorm( ToUNorm( c ) ); }
		SNormCoord		ToSNorm (const PixelsCoord &c)	const	{ return TypeCast::ToSNorm( ToUNorm( c ) ); }
		SNormCoord		ToSNorm (const UNormCoord &c)	const	{ return TypeCast::ToSNorm( c ); }

		PixelsCoord		Clamp (const PixelsCoord &c)	const	{ return GXMath::Clamp( c.To<int2>(),   int2(0),		_rect.Size().To<int2>()	).To<PixelsCoord>(); }
		PhysicalCoord	Clamp (const PhysicalCoord &c)	const	{ return GXMath::Clamp( c.To<float2>(), float2(0),		_size.To<float2>()		).To<PhysicalCoord>(); }
		DipCoord		Clamp (const DipCoord &c)		const	{ return GXMath::Clamp( c.To<float2>(), float2(0),		CalcDip().To<float2>()	).To<DipCoord>(); }
		SNormCoord		Clamp (const SNormCoord &c)		const	{ return GXMath::Clamp( c.To<float2>(), float2(-1.0f),	float2(1.0f)			).To<SNormCoord>(); }
		UNormCoord		Clamp (const UNormCoord &c)		const	{ return GXMath::Clamp( c.To<float2>(), float2(0),		float2(1.0f)			).To<UNormCoord>(); }

	private:
		static float	_MilimetersInInch ()					{ return 25.4f; }
		static float	_MetersInInch ()						{ return 0.0254f; }
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

		int2			RealResolution ()	const	{ return _realDisplay.ToPixels( (_ppi * _scale).To<DipCoord>() ).To<int2>(); }

		Display const &	RealDisplay ()		const	{ return _realDisplay; }

		Display &		RealDisplay ()				{ return _realDisplay; }
	};


}	// Platforms
}	// Engine
