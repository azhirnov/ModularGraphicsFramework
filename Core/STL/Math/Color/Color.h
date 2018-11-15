// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Math/Vec.h"
#include "Core/STL/Math/Rand/Random.h"
#include "Core/STL/Containers/String.h"
#include "Core/STL/Algorithms/Swap.h"

namespace GX_STL
{
namespace GXMath
{


	//
	// Color Functions
	//

	struct ColorUtils : public Noninstancable
	{
	// types
	private:
		template <typename T>
		struct Limits
		{
			template <bool bFloat>	static T _Max()	{ return bFloat ? T(1) : MaxValue<T>(); }
									static T Max()	{ return _Max< CompileTime::IsFloat<T> >(); }
									static T Min()	{ return T(0); }
		};

		template <typename T>
		struct LuminanceK {
			//static Vec<T,3>	Get()				{ return Vec<T,3>( 0.27,   0.67,   0.06   ); }
			//static Vec<T,3>	Get()				{ return Vec<T,3>( 0.3,    0.59,   0.11   ); }
			static Vec<T,3>	Get()				{ return Vec<T,3>( T(0.2126), T(0.7152), T(0.0722) ); }
		};


	// methods
	public:
		template <typename T, usize I, ulong U>	static	Vec<T,I,U>	Saturate (const Vec<T,I,U> &x);
		template <typename T, usize I, ulong U>	static	Vec<T,I,U>	Normalize (const Vec<T,I,U> &x);

		template <typename T, usize I, ulong U>	static	Vec<T,I,U>	Random ();
		template <typename T, usize I, ulong U>	static	Vec<T,I,U>	Random (const Vec<T,I,U> &minValue, const Vec<T,I,U> &maxValue);

		template <typename T, usize I, ulong U>	static	T			Luminance (const Vec<T,I,U> &x);
		template <typename T, usize I, ulong U>	static	T			LuminanceNorm (const Vec<T,I,U> &x);

		template <typename T, usize I, ulong U>	static	T			Negative (const Vec<T,I,U> &x);
		template <typename T, usize I, ulong U>	static	T			NegativeAlpha (const Vec<T,I,U> &x);
		template <typename T, usize I, ulong U>	static	T			AdjustContrast (const Vec<T,I,U> &x, T k);
		template <typename T, usize I, ulong U>	static	T			AdjustSaturation (const Vec<T,I,U> &x, T k);
		
		// conversion to non linear color space
		template <typename T, usize I, ulong U>	static	Vec<T,I,U>	ToSRGB (const Vec<T,I,U> &x);
		template <typename T, usize I, ulong U>	static	Vec<T,I,U>	FromSRGB (const Vec<T,I,U> &x);

		template <typename T>			static	Vec<T,4>			FromRGBA8 (uint x);
		template <typename T>			static	uint				ToRGBA8 (const Vec<T,4> &x);

										static	float3				RGBtoHSV (const float3 &rgb);
										static	float3				HSVtoRGB (const float3 &hsv);
										static	float3				RainbowRGB (const float x);

		template <typename B, typename T, usize I, ulong U>
										static	Vec<B,I,U>			Convert (const Vec<T,I,U> &x);

		template <typename T>			static	Matrix<T,4,4>		SaturationMat (const T& x);
	};


	//
	// Color4
	//

	struct color4u : ubyte4
	{
	// methods
	public:
		color4u () {}

		explicit color4u (const color4f &c);
		
		explicit color4u (ubyte val) : ubyte4( val ) {}

		explicit color4u (int val) : ubyte4( ubyte(val) ) {}

		color4u (ubyte r, ubyte g, ubyte b, ubyte a = 255) : ubyte4( r, g, b, a ) {}

		template <typename T>
		explicit color4u (const Vec<T,4> &v): ubyte4( ColorUtils::Convert<ubyte>( v ) ) {}

		//operator ubyte4 & ()					{ return static_cast< ubyte4& >( *this ); }
		//operator ubyte4   () const				{ return ubyte4( x, y, z, w ); }

		uint		ToIntRGBA () const			{ return ReferenceCast<uint>( ABGR() ); }
		uint		ToIntARGB () const			{ return ReferenceCast<uint>( ARGB() ); }
		uint		ToIntABGR () const			{ return ReferenceCast<uint>( *this ); }
		uint		ToIntRGB  () const			{ return ToIntRGBA() >> 8; }

		color4u &	FromIntABGR (uint abgr)		{ *this = ReferenceCast<color4u>( abgr );  return *this; }
		color4u &	FromIntARGB (uint argb)		{ *this = ReferenceCast<color4u>( argb ).BGRA();  return *this; }
		color4u &	FromIntRGBA (uint rgba)		{ *this = ReferenceCast<color4u>( rgba ).ABGR();  return *this; }
		color4u &	FromIntRGB  (uint rgb)		{ return FromIntRGBA( (rgb << 8) | 0xFF ); }

		Value_t		R ()	const				{ return (*this)[0]; }
		Value_t &	R ()						{ return (*this)[0]; }
		Value_t		G ()	const				{ return (*this)[1]; }
		Value_t &	G ()						{ return (*this)[1]; }
		Value_t		B ()	const				{ return (*this)[2]; }
		Value_t &	B ()						{ return (*this)[2]; }
		Value_t		A ()	const				{ return (*this)[3]; }
		Value_t &	A ()						{ return (*this)[3]; }

		color4u		BGRA ()	const				{ return color4u( z, y, x, w ); }
		color4u		ABGR ()	const				{ return color4u( w, z, y, x ); }
		color4u		ARGB () const				{ return color4u( w, x, y, z ); }

		color4u operator * (float right) const
		{
			return color4u( ColorUtils::Convert<float>( *this ) * right );
		}

		static color4u Interpolate (color4u left, color4u right, float a)
		{
			const float4 l = ColorUtils::Convert<float>( left );
			const float4 r = ColorUtils::Convert<float>( right );
			return color4u( Lerp( l, r, a ) );
		}


	// types
	public:
		struct Default
		{
			static color4u	Black ()	{ return color4u(   0,   0,   0, 255 ); }
			static color4u	White ()	{ return color4u( 255, 255, 255, 255 ); }
			static color4u	Gray ()		{ return color4u( 192, 192, 192, 255 ); }
			static color4u	Red ()		{ return color4u( 255,   0,   0, 255 ); }
			static color4u	Green ()	{ return color4u(   0, 255,   0, 255 ); }
			static color4u	Blue ()		{ return color4u(   0,   0, 255, 255 ); }
			static color4u	Yellow ()	{ return color4u( 255, 255,   0, 255 ); }
		};

		struct Html
		{
			// see https://www.w3schools.com/colors/colors_names.asp
#			define __DEF_COLOR( _name_, _color_ )	static color4u	_name_ () { return color4u().FromIntRGB( _color_ ); }

			__DEF_COLOR( AliceBlue,				0xF0F8FF )
			__DEF_COLOR( AntiqueWhite,			0xFAEBD7 )
			__DEF_COLOR( Aqua,					0x00FFFF )
			__DEF_COLOR( Aquamarine,			0x7FFFD4 )
			__DEF_COLOR( Azure,					0xF0FFFF )
			__DEF_COLOR( Beige,					0xF5F5DC )
			__DEF_COLOR( Bisque,				0xFFE4C4 )
			__DEF_COLOR( Black,					0x000000 )
			__DEF_COLOR( BlanchedAlmond,		0xFFEBCD )
			__DEF_COLOR( Blue,					0x0000FF )
			__DEF_COLOR( BlueViolet,			0x8A2BE2 )
			__DEF_COLOR( Brown,					0xA52A2A )
			__DEF_COLOR( BurlyWood,				0xDEB887 )
			__DEF_COLOR( CadetBlue,				0x5F9EA0 )
			__DEF_COLOR( Chartreuse,			0x7FFF00 )
			__DEF_COLOR( Chocolate,				0xD2691E )
			__DEF_COLOR( Coral,					0xFF7F50 )
			__DEF_COLOR( CornflowerBlue,		0x6495ED )
			__DEF_COLOR( Cornsilk,				0xFFF8DC )
			__DEF_COLOR( Crimson,				0xDC143C )
			__DEF_COLOR( Cyan,					0x00FFFF )
			__DEF_COLOR( DarkBlue,				0x00008B )
			__DEF_COLOR( DarkCyan,				0x008B8B )
			__DEF_COLOR( DarkGoldenRod,			0xB8860B )
			__DEF_COLOR( DarkGray,				0xA9A9A9 )
			__DEF_COLOR( DarkGreen,				0x006400 )
			__DEF_COLOR( DarkKhaki,				0xBDB76B )
			__DEF_COLOR( DarkMagenta,			0x8B008B )
			__DEF_COLOR( DarkOliveGreen,		0x556B2F )
			__DEF_COLOR( DarkOrange,			0xFF8C00 )
			__DEF_COLOR( DarkOrchid,			0x9932CC )
			__DEF_COLOR( DarkRed,				0x8B0000 )
			__DEF_COLOR( DarkSalmon,			0xE9967A )
			__DEF_COLOR( DarkSeaGreen,			0x8FBC8F )
			__DEF_COLOR( DarkSlateBlue,			0x483D8B )
			__DEF_COLOR( DarkSlateGray,			0x2F4F4F )
			__DEF_COLOR( DarkTurquoise,			0x00CED1 )
			__DEF_COLOR( DarkViolet,			0x9400D3 )
			__DEF_COLOR( DeepPink,				0xFF1493 )
			__DEF_COLOR( DeepSkyBlue,			0x00BFFF )
			__DEF_COLOR( DimGray,				0x696969 )
			__DEF_COLOR( DodgerBlue,			0x1E90FF )
			__DEF_COLOR( FireBrick,				0xB22222 )
			__DEF_COLOR( FloralWhite,			0xFFFAF0 )
			__DEF_COLOR( ForestGreen,			0x228B22 )
			__DEF_COLOR( Fuchsia,				0xFF00FF )
			__DEF_COLOR( Gainsboro,				0xDCDCDC )
			__DEF_COLOR( GhostWhite,			0xF8F8FF )
			__DEF_COLOR( Gold,					0xFFD700 )
			__DEF_COLOR( GoldenRod,				0xDAA520 )
			__DEF_COLOR( Gray,					0x808080 )
			__DEF_COLOR( Green,					0x008000 )
			__DEF_COLOR( GreenYellow,			0xADFF2F )
			__DEF_COLOR( HoneyDew,				0xF0FFF0 )
			__DEF_COLOR( HotPink,				0xFF69B4 )
			__DEF_COLOR( IndianRed,				0xCD5C5C )
			__DEF_COLOR( Indigo,				0x4B0082 )
			__DEF_COLOR( Ivory,					0xFFFFF0 )
			__DEF_COLOR( Khaki,					0xF0E68C )
			__DEF_COLOR( Lavender,				0xE6E6FA )
			__DEF_COLOR( LavenderBlush,			0xFFF0F5 )
			__DEF_COLOR( LawnGreen,				0x7CFC00 )
			__DEF_COLOR( LemonChiffon,			0xFFFACD )
			__DEF_COLOR( LightBlue,				0xADD8E6 )
			__DEF_COLOR( LightCoral,			0xF08080 )
			__DEF_COLOR( LightCyan,				0xE0FFFF )
			__DEF_COLOR( LightGoldenRodYellow,	0xFAFAD2 )
			__DEF_COLOR( LightGray,				0xD3D3D3 )
			__DEF_COLOR( LightGreen,			0x90EE90 )
			__DEF_COLOR( LightPink,				0xFFB6C1 )
			__DEF_COLOR( LightSalmon,			0xFFA07A )
			__DEF_COLOR( LightSeaGreen,			0x20B2AA )
			__DEF_COLOR( LightSkyBlue,			0x87CEFA )
			__DEF_COLOR( LightSlateGray,		0x778899 )
			__DEF_COLOR( LightSteelBlue,		0xB0C4DE )
			__DEF_COLOR( LightYellow,			0xFFFFE0 )
			__DEF_COLOR( Lime,					0x00FF00 )
			__DEF_COLOR( LimeGreen,				0x32CD32 )
			__DEF_COLOR( Linen,					0xFAF0E6 )
			__DEF_COLOR( Magenta,				0xFF00FF )
			__DEF_COLOR( Maroon,				0x800000 )
			__DEF_COLOR( MediumAquaMarine,		0x66CDAA )
			__DEF_COLOR( MediumBlue,			0x0000CD )
			__DEF_COLOR( MediumOrchid,			0xBA55D3 )
			__DEF_COLOR( MediumPurple,			0x9370DB )
			__DEF_COLOR( MediumSeaGreen,		0x3CB371 )
			__DEF_COLOR( MediumSlateBlue,		0x7B68EE )
			__DEF_COLOR( MediumSpringGreen,		0x00FA9A )
			__DEF_COLOR( MediumTurquoise,		0x48D1CC )
			__DEF_COLOR( MediumVioletRed,		0xC71585 )
			__DEF_COLOR( MidnightBlue,			0x191970 )
			__DEF_COLOR( MintCream,				0xF5FFFA )
			__DEF_COLOR( MistyRose,				0xFFE4E1 )
			__DEF_COLOR( Moccasin,				0xFFE4B5 )
			__DEF_COLOR( NavajoWhite,			0xFFDEAD )
			__DEF_COLOR( Navy,					0x000080 )
			__DEF_COLOR( OldLace,				0xFDF5E6 )
			__DEF_COLOR( Olive,					0x808000 )
			__DEF_COLOR( OliveDrab,				0x6B8E23 )
			__DEF_COLOR( Orange,				0xFFA500 )
			__DEF_COLOR( OrangeRed,				0xFF4500 )
			__DEF_COLOR( Orchid,				0xDA70D6 )
			__DEF_COLOR( PaleGoldenRod,			0xEEE8AA )
			__DEF_COLOR( PaleGreen,				0x98FB98 )
			__DEF_COLOR( PaleTurquoise,			0xAFEEEE )
			__DEF_COLOR( PaleVioletRed,			0xDB7093 )
			__DEF_COLOR( PapayaWhip,			0xFFEFD5 )
			__DEF_COLOR( PeachPuff,				0xFFDAB9 )
			__DEF_COLOR( Peru,					0xCD853F )
			__DEF_COLOR( Pink,					0xFFC0CB )
			__DEF_COLOR( Plum,					0xDDA0DD )
			__DEF_COLOR( PowderBlue,			0xB0E0E6 )
			__DEF_COLOR( Purple,				0x800080 )
			__DEF_COLOR( Red,					0xFF0000 )
			__DEF_COLOR( RosyBrown,				0xBC8F8F )
			__DEF_COLOR( RoyalBlue,				0x4169E1 )
			__DEF_COLOR( SaddleBrown,			0x8B4513 )
			__DEF_COLOR( Salmon,				0xFA8072 )
			__DEF_COLOR( SandyBrown,			0xF4A460 )
			__DEF_COLOR( SeaGreen,				0x2E8B57 )
			__DEF_COLOR( SeaShell,				0xFFF5EE )
			__DEF_COLOR( Sienna,				0xA0522D )
			__DEF_COLOR( Silver,				0xC0C0C0 )
			__DEF_COLOR( SkyBlue,				0x87CEEB )
			__DEF_COLOR( SlateBlue,				0x6A5ACD )
			__DEF_COLOR( SlateGray,				0x708090 )
			__DEF_COLOR( Snow,					0xFFFAFA )
			__DEF_COLOR( SpringGreen,			0x00FF7F )
			__DEF_COLOR( SteelBlue,				0x4682B4 )
			__DEF_COLOR( Tan,					0xD2B48C )
			__DEF_COLOR( Teal,					0x008080 )
			__DEF_COLOR( Thistle,				0xD8BFD8 )
			__DEF_COLOR( Tomato,				0xFF6347 )
			__DEF_COLOR( Turquoise,				0x40E0D0 )
			__DEF_COLOR( Violet,				0xEE82EE )
			__DEF_COLOR( Wheat,					0xF5DEB3 )
			__DEF_COLOR( White,					0xFFFFFF )
			__DEF_COLOR( WhiteSmoke,			0xF5F5F5 )
			__DEF_COLOR( Yellow,				0xFFFF00 )
			__DEF_COLOR( YellowGreen,			0x9ACD32 )

#			undef __DEF_COLOR
		};
	};


	struct color4f : float4
	{
		color4f () {}

		explicit
		color4f (const color4u &c);

		explicit
		color4f (float val): float4( val ) {}

		color4f (float R, float G, float B, float A): float4( R, G, B, A) {}
		
		template <typename T>
		color4f (const Vec<T,4> &v): float4( ColorUtils::Convert<float>( v ) ) {}

		Value_t		R ()	const		{ return (*this)[0]; }
		Value_t &	R ()				{ return (*this)[0]; }
		Value_t		G ()	const		{ return (*this)[1]; }
		Value_t &	G ()				{ return (*this)[1]; }
		Value_t		B ()	const		{ return (*this)[2]; }
		Value_t &	B ()				{ return (*this)[2]; }
		Value_t		A ()	const		{ return (*this)[3]; }
		Value_t &	A ()				{ return (*this)[3]; }
	};

	
	inline color4u::color4u (const color4f &c) : ubyte4( ColorUtils::Convert<ubyte>( float4(c) ) ) {}
	inline color4f::color4f (const color4u &c) : float4( ColorUtils::Convert<float>( ubyte4(c) ) ) {}

	


	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  ColorUtils::Saturate (const Vec<T,I,U> &x)
	{
		return Clamp( x, Limits<T>::Min(), Limits<T>::Max() );
	}


	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  ColorUtils::Normalize (const Vec<T,I,U> &x)
	{
		typedef typename CompileTime::GenType< T, float >	main_t;
		
		static const main_t		s_tAdd = main_t( CompileTime::IsInteger<T>() ? 0.5 : 0 );
		const main_t			t_max  = x.Max();

		if ( IsZero( t_max ) )
			return x;

		Vec<T,I,U>	ret;
		FOR( i, ret )	ret[i] = T( x[i] / t_max + s_tAdd );
		return ret;
	}

	
	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  ColorUtils::Random ()
	{
		Vec<T,I,U>	ret;
		FOR( i, ret )	ret[i] = GXMath::Random::Range( Limits<T>::Min(), Limits<T>::Max() );
		return ret;
	}
	

	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  ColorUtils::Random (const Vec<T,I,U> &minValue, const Vec<T,I,U> &maxValue)
	{
		ASSERT( All( minValue >= Vec<T,I,U>( Limits<T>::Min() ) ) );
		ASSERT( All( maxValue <= Vec<T,I,U>( Limits<T>::Max() ) ) );

		Vec<T,I,U>	ret;
		FOR( i, ret )	ret[i] = GXMath::Random::Range( minValue, maxValue );
		return ret;
	}


	template <typename T, usize I, ulong U>
	ND_ inline T  ColorUtils::Luminance (const Vec<T,I,U> &x)
	{
		return Dot( Vec<T,3>( x ), LuminanceK<T>::Get() );
	}


	template <typename T, usize I, ulong U>
	ND_ inline T  ColorUtils::LuminanceNorm (const Vec<T,I,U> &x)
	{
		return Clamp( Luminance( x ), Limits<T>::Min(), Limits<T>::Max() );
	}


	template <typename T, usize I, ulong U>
	ND_ inline T  ColorUtils::Negative (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>	ret;
		FOR( i, ret )	ret[i] = i < 3 ? Limits<T>::Max() - x[i] : x[i];
		return ret;
	}


	template <typename T, usize I, ulong U>
	ND_ inline T  ColorUtils::NegativeAlpha (const Vec<T,I,U> &x)
	{
		return Vec<T,I,U>( Limits<T>::Max() ) - x;
	}


	template <typename T, usize I, ulong U>
	ND_ inline T  ColorUtils::AdjustContrast (const Vec<T,I,U> &x, T k)
	{
		static const T		s_tMiddle	= Limits<T>::Max() / 2;
		Vec<T,I,U>	ret;
		FOR( i, ret )	ret[i] = i < 3 ? s_tMiddle + k * ( x[i] - s_tMiddle ) : x[i];
		return ret;
	}


	template <typename T, usize I, ulong U>
	ND_ inline T  ColorUtils::AdjustSaturation (const Vec<T,I,U> &x, T k)
	{
		const T		lum		= Luminance( x );
		Vec<T,I,U>	ret;
		FOR( i, ret )	ret[i] = i < 3 ? lum + k * ( x[i] - lum ) : x[i];
		return ret;
	}

		
	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  ColorUtils::ToSRGB (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>	ret;
		FOR( i, ret )	ret[i] = i < 3 ? ( x[i] <= T(0.0031308) ?  T(12.92) * x[i] :
											T(1.055) * Pow( x[i], T(0.41666) ) - T(0.0055) ) : T(0);
		return ret;
	}


	template <typename T, usize I, ulong U>
	ND_ inline Vec<T,I,U>  ColorUtils::FromSRGB (const Vec<T,I,U> &x)
	{
		Vec<T,I,U>	ret;
		FOR( i, ret )	ret[i] = i < 3 ? ( x[i] <= T(0.04045) ?  x[i] / T(12.92) :
											Pow( ( x[i] + T(0.055) ) / T(1.055), T(0.41666) ) ) : T(0);
		return ret;
	}
	

	template <typename T>
	ND_ inline Vec<T,4>  ColorUtils::FromRGBA8 (uint x)
	{
		return Convert<T>( FromRGBA8<ubyte>( x ) );
	}
	

	template <>
	ND_ inline Vec<ubyte,4>  ColorUtils::FromRGBA8<ubyte> (uint x)
	{
		return Vec<ubyte, 4>{ ubyte((x >> 24) & 0xFF),
							  ubyte((x >> 16) & 0xFF),
							  ubyte((x >>  8) & 0xFF),
							  ubyte((x) & 0xFF)	};
	}


	template <typename T>
	ND_ inline uint  ColorUtils::ToRGBA8 (const Vec<T,4> &x)
	{
		return ToRGBA8<ubyte>( Convert<ubyte>( x ) );
	}


	template <>
	ND_ inline uint  ColorUtils::ToRGBA8<ubyte> (const Vec<ubyte,4> &c)
	{
		uint	u_rgba =	(uint(c.x) << 24) |
							(uint(c.y) << 16) |
							(uint(c.z) <<  8) |
							(uint(c.w));
		return u_rgba;
	}


	template <typename T>
	ND_ inline Matrix<T,4,4>  ColorUtils::SaturationMat (const T& s)
	{
		const Vec<T,3>		c = (T(1) - s) * LuminanceK<T>::Get();

		Matrix<T,4,4>		ret( 1 );

		ret(0,0) = c[0] + s;	ret(0,1) = c[0];		ret(0,2) = c[0];
		ret(1,0) = c[1];		ret(1,1) = c[1] + s;	ret(1,2) = c[1];
		ret(2,0) = c[2];		ret(2,1) = c[2];		ret(2,2) = c[2] + s;

		return ret;
	}


	template <typename B, typename T, usize I, ulong U>
	ND_ inline Vec<B,I,U>  ColorUtils::Convert (const Vec<T,I,U> &x)
	{
		typedef typename CompileTime::GenType< B, T, float >		main_t;

		static const main_t		s_tK   = main_t(Limits<B>::Max()) / main_t(Limits<T>::Max());
		static const main_t		s_tAdd = main_t(CompileTime::IsInteger<B> ? 0.5 : 0);

		Vec<B,I,U>		ret;
		FOR( i, ret )	ret[i] = B( x[i] * s_tK + s_tAdd );
		return ret;
	}
	

	ND_ inline float3  ColorUtils::RGBtoHSV (const float3 &rgb)
	{
		// from http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv

		float	k = float(0);
		float	r = rgb.x;
		float	g = rgb.y;
		float	b = rgb.z;

		if ( g < b )
		{
			SwapValues( g, b );
			k = -1.0f;
		}

		if ( r < g )
		{
			SwapValues( r, g );
			k = -2.0f / 6.0f - k;
		}

		const float	chroma = r - Min( g, b );

		return float3( Abs( k + (g - b) / 6.0f * chroma + 1e-20f ),
					  chroma / ( r + 1e-20f ),
					  r );
	}


	ND_ inline float3  ColorUtils::HSVtoRGB (const float3 &hsv)
	{
		// from http://chilliant.blogspot.ru/2014/04/rgbhsv-in-hlsl-5.html

		float3 const	col( Abs( hsv.x * 6.0f - 3.0f ) - 1.0f,
							 2.0f - Abs( hsv.x * 6.0f - 2.0f ),
							 2.0f - Abs( hsv.x * 6.0f - 4.0f ) );

		return (( GXMath::Clamp( col, float3(0.0f), float3(1.0f) ) - 1.0f ) * hsv.y + 1.0f ) * hsv.z;
	}
	

	ND_ inline float3  ColorUtils::RainbowRGB (const float value)
	{
		return HSVtoRGB( float3( value, 1.0f, 1.0f ) );
	}



}	// GXMath

namespace GXTypes
{
	template <>
	struct Hash< GXMath::color4u > : public Hash< GXMath::ubyte4 >
	{};

	template <>
	struct Hash< GXMath::color4f > : public Hash< GXMath::float4 >
	{};

}	// GXTypes
}	// GX_STL
