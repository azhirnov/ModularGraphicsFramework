// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Math/Mathematics.h"
#include "Engine/STL/Math/Vec.h"
#include "TR11G11B10F.h"
#include "TRGB9_E5.h"
#include "Half.h"

namespace GX_STL
{
namespace GXMath
{

	namespace _color_hidden_
	{
		struct EColorFormatDescriptor { enum type
		{
			UNKNOWN		= 0,
			SIGNED		= 1 << 0,
			UNSIGNED	= 1 << 1,
			INTEGER		= 1 << 2,
			FLOAT		= 1 << 3,
			NORMALIZED	= 1 << 4,

			// default //
			SINT		= SIGNED | INTEGER,
			UINT		= UNSIGNED | INTEGER,
			SFLOAT		= SIGNED | FLOAT,
			UFLOAT		= UNSIGNED | FLOAT,
		};};


#	if GX_COLOR_FORMAT_HIGH_PRECISION
		typedef ilong4	IntFormat;
		typedef double4	FloatFormat;
#	else
		typedef int4	IntFormat;
		typedef float4	FloatFormat;
#	endif


		//
		// Max int color format components size in bits
		//
		struct IntColorFormatInfo
		{
		// variables
		private:
			IntFormat	sizeInBits;
			bool		isSigned;

		// methods
		private:
			template <typename ColorType>
			IntColorFormatInfo (uint sizeR, uint sizeG, uint sizeB, uint sizeA, const ColorType &) :
				sizeInBits( sizeR, sizeG, sizeB, sizeA ),
				isSigned( (ColorType::format & EColorFormatDescriptor::SIGNED) != 0 )
			{
				STATIC_ASSERT( (ColorType::format & EColorFormatDescriptor::INTEGER) != 0,
								"color format must be integer" );

				ASSERT( (sizeR + sizeG + sizeB + sizeA) == CompileTime::SizeOf<ColorType>::bits );
			}

		public:
			template <uint sizeR, uint sizeG, uint sizeB, uint sizeA, typename ColorType>
			static IntColorFormatInfo PackedType (const ColorType &color)
			{
				STATIC_ASSERT( (sizeR + sizeG + sizeB + sizeA) == CompileTime::SizeOf<ColorType>::bits );
				return IntColorFormatInfo( sizeR, sizeG, sizeB, sizeA, color );
			}


			template <typename ValueType, uint ChannelsCount, typename ColorType>
			static IntColorFormatInfo SimpleType (const ColorType &color)
			{
				const uint channel_size = CompileTime::SizeOf<ValueType>::bits;

				STATIC_ASSERT( (channel_size * ChannelsCount) == CompileTime::SizeOf<ColorType>::bits );
				STATIC_ASSERT( ChannelsCount > 0 and ChannelsCount <= 4 );

				return IntColorFormatInfo( ChannelsCount > 0 ? channel_size : 0,
									ChannelsCount > 1 ? channel_size : 0,
									ChannelsCount > 2 ? channel_size : 0,
									ChannelsCount > 3 ? channel_size : 0,
									color );
			}


			template <uint R, uint G, uint B, uint A>
			IntColorFormatInfo GetSwizzledFormat () const
			{
				IntColorFormatInfo	res;
				res.sizeInBits = IntFormat( this->sizeInBits[R&3], this->sizeInBits[G&3],
											this->sizeInBits[B&3], this->sizeInBits[A&3] );
				res.isSigned = this->isSigned;
				return res;
			}


			IntFormat MaxValue() const
			{
				return ToMask< IntFormat::value_t >( Max( sizeInBits - int(isSigned), IntFormat() ).To<BitsVec<usize,4>>() );
			}

			FloatFormat MaxValueF() const
			{
				return this->MaxValue().To<FloatFormat>();
			}

			IntFormat MinValue() const
			{
				return isSigned ? (-MaxValue()-1) : IntFormat(0);
			}

			IntFormat GetShiftTo (const IntColorFormatInfo &fmt) const
			{
				return fmt.sizeInBits - sizeInBits;
			}

			IntFormat GetShiftFrom (const IntColorFormatInfo &fmt) const
			{
				return -GetShiftTo( fmt );
			}

			IntFormat Clamp (const IntFormat &value) const
			{
				typedef Vec< CompileTime::NearInt::FromType< IntFormat::value_t >, 4 >	sign_t;
				typedef Vec< CompileTime::NearUInt::FromType< IntFormat::value_t >, 4 >	unsign_t;

				return isSigned ?
						GXMath::Clamp( value, MinValue(), MaxValue() ) :
						GXMath::Clamp( value.To<unsign_t>(), MinValue().To<unsign_t>(), MaxValue().To<unsign_t>() ).To<sign_t>();
			}
		};



		//
		// Float format
		//
		struct FloatColorFormatInfo
		{
		// variables
		private:
			FloatFormat	maxValue,
						minValue;
			bool		isSigned;


		// methods
		private:
			FloatColorFormatInfo (const FloatFormat &maxValue, const FloatFormat &minValue, bool isSigned) :
				maxValue(maxValue), minValue(minValue), isSigned(isSigned)
			{}

		public:
			template <typename ValueType>
			static FloatColorFormatInfo SimpleType ()
			{
				const ValueType	min_value = GXTypes::MinValue<ValueType>();
				const ValueType	max_value = GXTypes::MaxValue<ValueType>();

				return FloatColorFormatInfo( FloatFormat(max_value), FloatFormat(min_value), true );
			}


			static FloatColorFormatInfo PackedType (const FloatFormat &maxValue)
			{
				return FloatColorFormatInfo( maxValue, FloatFormat(0), false );
			}


			template <uint R, uint G, uint B, uint A>
			FloatColorFormatInfo GetSwizzledFormat () const
			{
				FloatColorFormatInfo	res( FloatFormat( this->maxValue[R&3], this->maxValue[G&3],
														  this->maxValue[B&3], this->maxValue[A&3] ),
											 FloatFormat( this->minValue[R&3], this->minValue[G&3],
														  this->minValue[B&3], this->minValue[A&3] ),
											 this->isSigned );
				return res;
			}


			FloatFormat MaxValue() const
			{
				return maxValue;
			}

			FloatFormat MinValue() const
			{
				return isSigned ? -maxValue : FloatFormat(0);
			}

			FloatFormat Clamp (const FloatFormat &value) const
			{
				FloatFormat	ret;
				FOR( i, ret )	ret[i] = Max( value[i], FloatFormat::value_t(0) );
				return ret;
			}
		};



		//
		// Color Format Converter
		//
		struct ColorFormatConverter
		{
			template <typename ColorType>
			static void MustBeInteger ()
			{
				STATIC_ASSERT( (ColorType::format & EColorFormatDescriptor::INTEGER) != 0,
								"color format must be integer" );
			}
		
			template <typename ColorType>
			static void MustBeFloat ()
			{
				STATIC_ASSERT( (ColorType::format & EColorFormatDescriptor::FLOAT) != 0,
								"color format must be float" );
			}

			template <typename ColorType>
			static void MustBeNormalized ()
			{
				STATIC_ASSERT( (ColorType::format & EColorFormatDescriptor::NORMALIZED) != 0,
								"color format must be normalized" );
			}

			template <typename ColorType>
			static void MustBeUnnormalized ()
			{
				STATIC_ASSERT( (ColorType::format & EColorFormatDescriptor::NORMALIZED) == 0,
								"color format must be not normalized" );
			}

			// normalized int to normalized int
			template <typename SrcColorType, typename DstColorType>
			static void NormIntToNormInt (DstColorType &dst, const SrcColorType &src)
			{
				typedef IntFormat::value_t	int_t;

				MustBeInteger< SrcColorType >();
				MustBeInteger< DstColorType >();
				MustBeNormalized< DstColorType >();

				IntColorFormatInfo const	dfmt(dst);
				IntColorFormatInfo const	sfmt(src);

				IntFormat const		shift = sfmt.GetShiftTo( dfmt );
				IntFormat const		scolor(src);
				IntFormat			dcolor;
				int_t const			one = int_t(1);

				FOR( i, dcolor )
				{
					int_t const	half_val = ( one << Max( Abs(shift[i])-1, 0 ) ) - 1;

					dcolor[i] = shift[i] >= 0 ?
								(scolor[i] << +shift[i]) + half_val :
								(scolor[i] + half_val) >> -shift[i];
				}

				dst = dfmt.Clamp( dcolor );
			}

			// int to normalized int

			// normalized int to int
			/*template <typename SrcColorType, typename DstColorType>
			static void NormIntToInt (DstColorType &dst, const SrcColorType &src)
			{
				MustBeNormalized< SrcColorType >();
				MustBeInteger< SrcColorType >();
				MustBeInteger< DstColorType >();

				IntColorFormatInfo const	dfmt(dst);
				IntColorFormatInfo const	sfmt(src);

				IntFormat const		scolor(src);
				IntFormat			dcolor = Round< IntFormat::value_t >( scolor.To<FloatFormat>() / sfmt.MaxValueF() );
				
				dst = dfmt.Clamp( dcolor );
			}*/

			// int to int (not normalized)
			template <typename SrcColorType, typename DstColorType>
			static void IntToInt (DstColorType &dst, const SrcColorType &src)
			{
				MustBeInteger< SrcColorType >();
				MustBeInteger< DstColorType >();
				MustBeUnnormalized< DstColorType >();

				IntColorFormatInfo const	dfmt(dst);
				IntFormat const				scolor(src);
				
				dst = dfmt.Clamp( scolor );
			}

			// normalized int to normalized float
			template <typename SrcColorType, typename DstColorType>
			static void NormIntToNormFloat (DstColorType &dst, const SrcColorType &src)
			{
				MustBeNormalized< SrcColorType >();
				MustBeInteger< SrcColorType >();
				MustBeFloat< DstColorType >();

				IntColorFormatInfo const	sfmt(src);
				FloatColorFormatInfo const	dfmt(dst);
				IntFormat   const			scolor(src);
				FloatFormat const			dcolor = scolor.To<FloatFormat>() / sfmt.MaxValueF();

				dst = dfmt.Clamp( dcolor );
			}

			// int to normalized float
			template <typename SrcColorType, typename DstColorType>
			static void IntToNormFloat (DstColorType &dst, const SrcColorType &src)
			{
				MustBeUnnormalized< SrcColorType >();
				MustBeInteger< SrcColorType >();
				MustBeFloat< DstColorType >();

				IntColorFormatInfo const	sfmt(src);
				FloatColorFormatInfo const	dfmt(dst);
				IntFormat   const			scolor(src);
				FloatFormat const			dcolor = scolor.To<FloatFormat>();

				dst = dfmt.Clamp( dcolor );
			}

			// int to float (not normalized)
			template <typename SrcColorType, typename DstColorType>
			static void IntToFloat (DstColorType &dst, const SrcColorType &src)
			{
				MustBeInteger< SrcColorType >();
				MustBeFloat< DstColorType >();

				IntColorFormatInfo const	sfmt(src);
				FloatColorFormatInfo const	dfmt(dst);
				IntFormat   const			scolor(src);
				FloatFormat const			dcolor = scolor.To<FloatFormat>();

				dst = dfmt.Clamp( dcolor );
			}

			// float to normalized int
			template <typename SrcColorType, typename DstColorType>
			static void FloatToNormInt (DstColorType &dst, const SrcColorType &src)
			{
				MustBeFloat< SrcColorType >();
				MustBeInteger< DstColorType >();
				MustBeNormalized< DstColorType >();

				FloatColorFormatInfo const	sfmt(src);
				IntColorFormatInfo const	dfmt(dst);
				FloatFormat const			scolor(src);
				IntFormat   const			dcolor = Round< IntFormat::value_t >( scolor );

				dst = dfmt.Clamp( dcolor );
			}

			// normalized float to normalized int
			template <typename SrcColorType, typename DstColorType>
			static void NormFloatToNormInt (DstColorType &dst, const SrcColorType &src)
			{
				MustBeFloat< SrcColorType >();
				MustBeInteger< DstColorType >();

				FloatColorFormatInfo const	sfmt(src);
				IntColorFormatInfo const	dfmt(dst);
				FloatFormat const			scolor(src);
				IntFormat   const			dcolor = RoundToInt( scolor * dfmt.MaxValueF() );
	
				dst = dfmt.Clamp( dcolor );
			}

			// float to int (not normalized)
			template <typename SrcColorType, typename DstColorType>
			static void FloatToInt (DstColorType &dst, const SrcColorType &src)
			{
				MustBeFloat< SrcColorType >();
				MustBeInteger< DstColorType >();

				FloatColorFormatInfo const	sfmt(src);
				IntColorFormatInfo const	dfmt(dst);
				FloatFormat const			scolor(src);
				IntFormat   const			dcolor = RoundToInt( scolor );
	
				dst = dfmt.Clamp( dcolor );
			}
		
			// float to float (normalized or not normalized)
			template <typename SrcColorType, typename DstColorType>
			static void FloatToFloat (DstColorType &dst, const SrcColorType &src)
			{
				MustBeFloat< SrcColorType >();
				MustBeFloat< DstColorType >();
		
				FloatColorFormatInfo const	dfmt(dst);

				dst = dfmt.Clamp( FloatFormat(src) );
			}
		
			template <typename SrcColorType, typename DstColorType>
			static void Convert (DstColorType &dst, const SrcColorType &src);
		};


		template <typename Src, typename Dst, bool SrcInt, bool DstInt, bool SrcNorm, bool DstNorm>
		struct ConverterSwitch
		{
			static void Convert (Dst &dst, const Src &src)
			{
				DESCTOP_ONLY( STATIC_WARNING( "not implementation for this formats!" ) );
			}
		};

#		define CONVERTER_SWITCH( _srcInt_, _dstInt_, _srcNorm_, _dstNorm_, _func_ ) \
			template <typename Src, typename Dst> \
			struct ConverterSwitch < Src, Dst, _srcInt_, _dstInt_, _srcNorm_, _dstNorm_ > \
			{ \
				static void Convert (Dst &dst, const Src &src) \
				{ \
					ColorFormatConverter::_func_( dst, src ); \
				} \
			}

		//				  srcInt, dstInt, srcNorm, dstNorm,   func
		CONVERTER_SWITCH( false,  false,  false,   false,     FloatToFloat );
		CONVERTER_SWITCH( false,  false,  false,   true,      FloatToFloat );
		CONVERTER_SWITCH( false,  false,  true,    false,     FloatToFloat );
		CONVERTER_SWITCH( false,  false,  true,    true,      FloatToFloat );
			
		CONVERTER_SWITCH( false,  true,   false,   false,     FloatToInt );
		CONVERTER_SWITCH( false,  true,   false,   true,      FloatToNormInt );
		CONVERTER_SWITCH( false,  true,   true,    false,     FloatToInt );
		CONVERTER_SWITCH( false,  true,   true,    true,      NormFloatToNormInt );
		
		CONVERTER_SWITCH( true,   false,  false,   false,     IntToFloat );
		CONVERTER_SWITCH( true,   false,  false,   true,      IntToNormFloat );
		CONVERTER_SWITCH( true,   false,  true,    false,     IntToFloat );
		CONVERTER_SWITCH( true,   false,  true,    true,      NormIntToNormFloat );
			
		CONVERTER_SWITCH( true,   true,   false,   false,     IntToInt );
		CONVERTER_SWITCH( true,   true,   false,   true,      NormIntToNormInt );
		CONVERTER_SWITCH( true,   true,   true,    false,     IntToInt );
		CONVERTER_SWITCH( true,   true,   true,    true,      NormIntToNormInt );

#		undef CONVERTER_SWITCH

	
		template <typename SrcColorType, typename DstColorType>
		inline void ColorFormatConverter::Convert (DstColorType &dst, const SrcColorType &src)
		{
			ConverterSwitch< SrcColorType,
							 DstColorType,
							 (SrcColorType::format & EColorFormatDescriptor::INTEGER) != 0,
							 (DstColorType::format & EColorFormatDescriptor::INTEGER) != 0,
							 (SrcColorType::format & EColorFormatDescriptor::NORMALIZED) != 0,
							 (DstColorType::format & EColorFormatDescriptor::NORMALIZED) != 0 >
				::Convert( dst, src );
		}

	
#	pragma warning (push)
#	pragma warning (disable: 4244)		// convert to smallest type


		//
		// Base Color Type
		//
		template <typename T, bool Normalized = false>
		struct BaseColor : public CompileTime::CopyQualifiers< T >
		{
			static const bool	_is_float		= CompileTime::IsFloat<T>;
			static const bool	_is_integer		= CompileTime::IsInteger<T> or not _is_float;
			static const bool	_is_signed		= CompileTime::IsSigned<T>;
			static const bool	_is_unsigned	= CompileTime::IsUnsigned<T> or not _is_signed;
			static const bool	_is_normalized	= Normalized;

			static const uint	format	=
									(_is_integer ? EColorFormatDescriptor::INTEGER :  EColorFormatDescriptor::FLOAT) |
									(_is_unsigned ? EColorFormatDescriptor::UNSIGNED : EColorFormatDescriptor::SIGNED) |
									(_is_normalized ? EColorFormatDescriptor::NORMALIZED : 0 );

			STATIC_ASSERT( _is_signed != _is_unsigned );
		};


	//-----------------------------------------------
	// Integer Formats


		//
		// Red or Alpha
		//
		template <typename T, bool Normalized = false>
		struct RedI : BaseColor<T, Normalized>
		{
			T	r;

			RedI () : r(0) {}
			RedI (T R) : r(R) {}
			RedI (const IntFormat &c) : r(c[0]) {}

			operator IntFormat () const				{ return IntFormat( r, 0, 0, 0 ); }
			operator IntColorFormatInfo () const	{ return IntColorFormatInfo::SimpleType<T,1>( *this ); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( IntFormat(*this) == IntFormat(right) );
			}
		};


		//
		// RG
		//
		template <typename T, bool Normalized = false>
		struct RGi : BaseColor<T, Normalized>
		{
			T	r, g;

			RGi () : r(0), g(0) {}
			RGi (T R, T G) : r(R), g(G) {}
			RGi (const IntFormat &c) : r(c[0]), g(c[1]) {}

			operator IntFormat () const				{ return IntFormat( r, g, 0, 0 ); }
			operator IntColorFormatInfo () const	{ return IntColorFormatInfo::SimpleType<T,2>( *this ); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( IntFormat(*this) == IntFormat(right) );
			}
		};


		//
		// RGB
		//
		template <typename T, bool Normalized = false>
		struct RGBi : BaseColor<T, Normalized>
		{
			T	r, g, b;

			RGBi () : r(0), g(0), b(0) {}
			RGBi (T R, T G, T B) : r(R), g(G), b(B) {}
			RGBi (const IntFormat &c) : r(c[0]), g(c[1]), b(c[2]) {}

			operator IntFormat () const				{ return IntFormat( r, g, b, 0 ); }
			operator IntColorFormatInfo () const	{ return IntColorFormatInfo::SimpleType<T,3>( *this ); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( IntFormat(*this) == IntFormat(right) );
			}
		};


		//
		// RGBA
		//
		template <typename T, bool Normalized = false>
		struct RGBAi : BaseColor<T, Normalized>
		{
			T	r, g, b, a;

			RGBAi () : r(0), g(0), b(0), a(0) {}
			RGBAi (T R, T G, T B, T A) : r(R), g(G), b(B), a(A) {}
			RGBAi (const IntFormat &c) : r(c[0]), g(c[1]), b(c[2]), a(c[3]) {}

			operator IntFormat () const				{ return IntFormat( r, g, b, a ); }
			operator IntColorFormatInfo () const	{ return IntColorFormatInfo::SimpleType<T,4>( *this ); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( IntFormat(*this) == IntFormat(right) );
			}
		};


		//
		// RGBA 16-bit
		//
		template <bool Normalized = false>
		struct RGBA4u
		{
			enum { format = EColorFormatDescriptor::UINT | (Normalized ? EColorFormatDescriptor::NORMALIZED : 0) };

			struct Bits {
				ushort	a : 4,
						b : 4,
						g : 4,
						r : 4;
			};
			union {
				Bits	bits;
				ushort	data;
			};

			RGBA4u () : data(0)							{}
			RGBA4u (ubyte R, ubyte G, ubyte B, ubyte A)	{ bits.r = R;  bits.g = G;  bits.b = B;  bits.a = A; }
			RGBA4u (const IntFormat &c)					{ bits.r = c[0];  bits.g = c[1];  bits.b = c[2];  bits.a = c[3]; }
		
			operator IntFormat () const					{ return IntFormat( bits.r, bits.g, bits.b, bits.a ); }
			operator IntColorFormatInfo () const		{ return IntColorFormatInfo::PackedType< 4, 4, 4, 4 >(*this); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( IntFormat(*this) == IntFormat(right) );
			}
		};
		

		//
		// RGBA 16-bit Reverse
		//
		template <bool Normalized = false>
		struct RGBA4u_Rev
		{
			enum { format = EColorFormatDescriptor::UINT | (Normalized ? EColorFormatDescriptor::NORMALIZED : 0) };

			struct Bits {
				ushort	r : 4,
						g : 4,
						b : 4,
						a : 4;
			};
			union {
				Bits	bits;
				ushort	data;
			};

			RGBA4u_Rev () : data(0)							{}
			RGBA4u_Rev (ubyte R, ubyte G, ubyte B, ubyte A)	{ bits.r = R;  bits.g = G;  bits.b = B;  bits.a = A; }
			RGBA4u_Rev (const IntFormat &c)					{ bits.r = c[0];  bits.g = c[1];  bits.b = c[2];  bits.a = c[3]; }
		
			operator IntFormat () const						{ return IntFormat( bits.r, bits.g, bits.b, bits.a ); }
			operator IntColorFormatInfo () const			{ return IntColorFormatInfo::PackedType< 4, 4, 4, 4 >(*this); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( IntFormat(*this) == IntFormat(right) );
			}
		};


		//
		// RGBA 16-bit
		//
		template <bool Normalized = false>
		struct RGB5_A1u
		{
			enum { format = EColorFormatDescriptor::UINT | (Normalized ? EColorFormatDescriptor::NORMALIZED : 0) };

			struct Bits {
				ushort	a : 1,
						b : 5,
						g : 5,
						r : 5;
			};
			union {
				Bits	bits;
				ushort	data;
			};

			RGB5_A1u () : data(0) {}
			RGB5_A1u (ubyte R, ubyte G, ubyte B, bool A)	{ bits.r = R;  bits.g = G;  bits.b = B;  bits.a = A; }
			RGB5_A1u (const IntFormat &c)					{ bits.r = c[0];  bits.g = c[1];  bits.b = c[2];  bits.a = c[3]; }
		
			operator IntFormat () const						{ return IntFormat( bits.r, bits.g, bits.b, bits.a ); }
			operator IntColorFormatInfo () const			{ return IntColorFormatInfo::PackedType< 5, 5, 5, 1 >(*this); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( IntFormat(*this) == IntFormat(right) );
			}
		};


		//
		// RGBA 16-bit Reverse
		//
		template <bool Normalized = false>
		struct RGB5_A1u_Rev
		{
			enum { format = EColorFormatDescriptor::UINT | (Normalized ? EColorFormatDescriptor::NORMALIZED : 0) };

			struct Bits {
				ushort	r : 5,
						g : 5,
						b : 5,
						a : 1;
			};
			union {
				Bits	bits;
				ushort	data;
			};

			RGB5_A1u_Rev () : data(0) {}
			RGB5_A1u_Rev (ubyte R, ubyte G, ubyte B, bool A)	{ bits.r = R;  bits.g = G;  bits.b = B;  bits.a = A; }
			RGB5_A1u_Rev (const IntFormat &c)					{ bits.r = c[0];  bits.g = c[1];  bits.b = c[2];  bits.a = c[3]; }
		
			operator IntFormat () const							{ return IntFormat( bits.r, bits.g, bits.b, bits.a ); }
			operator IntColorFormatInfo () const				{ return IntColorFormatInfo::PackedType< 5, 5, 5, 1 >(*this); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( IntFormat(*this) == IntFormat(right) );
			}
		};


		//
		// RGB 16-bit
		//
		template <bool Normalized = false>
		struct R5_G6_B5u
		{
			enum { format = EColorFormatDescriptor::UINT | (Normalized ? EColorFormatDescriptor::NORMALIZED : 0) };

			struct Bits {
				ushort	b : 5,
						g : 6,
						r : 5;
			};
			union {
				Bits	bits;
				ushort	data;
			};
		
			R5_G6_B5u () : data(0) {}
			R5_G6_B5u (ubyte R, ubyte G, ubyte B)	{ bits.r = R;  bits.g = G;  bits.b = B; }
			R5_G6_B5u (const IntFormat &c)			{ bits.r = c[0];  bits.g = c[1];  bits.b = c[2]; }

			operator IntFormat () const				{ return IntFormat( bits.r, bits.g, bits.b, 0 ); }
			operator IntColorFormatInfo () const	{ return IntColorFormatInfo::PackedType< 5, 6, 5, 0 >(*this); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( IntFormat(*this) == IntFormat(right) );
			}
		};
		

		//
		// RGB 16-bit Reverse
		//
		template <bool Normalized = false>
		struct R5_G6_B5u_Rev
		{
			enum { format = EColorFormatDescriptor::UINT | (Normalized ? EColorFormatDescriptor::NORMALIZED : 0) };

			struct Bits {
				ushort	r : 5,
						g : 6,
						b : 5;
			};
			union {
				Bits	bits;
				ushort	data;
			};
		
			R5_G6_B5u_Rev () : data(0) {}
			R5_G6_B5u_Rev (ubyte R, ubyte G, ubyte B)	{ bits.r = R;  bits.g = G;  bits.b = B; }
			R5_G6_B5u_Rev (const IntFormat &c)			{ bits.r = c[0];  bits.g = c[1];  bits.b = c[2]; }

			operator IntFormat () const					{ return IntFormat( bits.r, bits.g, bits.b, 0 ); }
			operator IntColorFormatInfo () const		{ return IntColorFormatInfo::PackedType< 5, 6, 5, 0 >(*this); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( IntFormat(*this) == IntFormat(right) );
			}
		};


		//
		// RGBA 32-bit
		//
		template <bool Normalized = false>
		struct RGB10_A2u
		{
			enum { format = EColorFormatDescriptor::UINT | (Normalized ? EColorFormatDescriptor::NORMALIZED : 0) };

			struct Bits {
				uint	a : 2,
						b : 10,
						g : 10,
						r : 10;
			};
			union {
				Bits	bits;
				uint	data;
			};

			RGB10_A2u () : data(0) {}
			RGB10_A2u (ushort R, ushort G, ushort B, ubyte A)	{ bits.r = R;  bits.g = G;  bits.b = B;  bits.a = A; }
			RGB10_A2u (const IntFormat &c)						{ bits.r = c[0];  bits.g = c[1];  bits.b = c[2];  bits.a = c[3]; }

			operator IntFormat () const							{ return IntFormat( bits.r, bits.g, bits.b, bits.a ); }
			operator IntColorFormatInfo () const				{ return IntColorFormatInfo::PackedType< 10, 10, 10, 2 >(*this); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( IntFormat(*this) == IntFormat(right) );
			}
		};


		//
		// RGBA 32-bit Reverse
		//
		template <bool Normalized = false>
		struct RGB10_A2u_Rev
		{
			enum { format = EColorFormatDescriptor::UINT | (Normalized ? EColorFormatDescriptor::NORMALIZED : 0) };

			struct Bits {
				uint	r : 10,
						g : 10,
						b : 10,
						a : 2;
			};
			union {
				Bits	bits;
				uint	data;
			};

			RGB10_A2u_Rev () : data(0) {}
			RGB10_A2u_Rev (ushort R, ushort G, ushort B, ubyte A)	{ bits.r = R;  bits.g = G;  bits.b = B;  bits.a = A; }
			RGB10_A2u_Rev (const IntFormat &c)						{ bits.r = c[0];  bits.g = c[1];  bits.b = c[2];  bits.a = c[3]; }

			operator IntFormat () const								{ return IntFormat( bits.r, bits.g, bits.b, bits.a ); }
			operator IntColorFormatInfo () const					{ return IntColorFormatInfo::PackedType< 10, 10, 10, 2 >(*this); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( IntFormat(*this) == IntFormat(right) );
			}
		};

	


	//-----------------------------------------------
	// Float Formats

		//
		// RGB 32-bit
		//
		template <bool Normalized = false>
		struct R11_G11_B10f
		{
			enum { format = EColorFormatDescriptor::UFLOAT | (Normalized ? EColorFormatDescriptor::NORMALIZED : 0) };

			typedef r11g11b10f_t::vec3_t	vec3_t;

			r11g11b10f_t	data;

			R11_G11_B10f () {}
			R11_G11_B10f (float R, float G, float B)	{ data.SetR(R);  data.SetG(G);  data.SetB(B); }
			R11_G11_B10f (const FloatFormat &c)			{ data.SetR(c[0]);  data.SetG(c[1]);  data.SetB(c[2]); }

			operator FloatFormat () const				{ return vec3_t(data).To<FloatFormat>(); }
			operator FloatColorFormatInfo () const		{ return FloatColorFormatInfo::PackedType( FloatFormat(*this) ); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( FloatFormat(*this) == FloatFormat(right) );
			}
		};


		//
		// RGB9 E5 32-bit
		//
		template <bool Normalized = false>
		struct RGB9_E5f_Rev
		{
			enum { format = EColorFormatDescriptor::UFLOAT | (Normalized ? EColorFormatDescriptor::NORMALIZED : 0) };

			typedef rgb9_e5_t::vec3_t	vec3_t;

			rgb9_e5_t	data;

			RGB9_E5f_Rev () {}
			RGB9_E5f_Rev (float R, float G, float B)	{ data.Set( R, G, B ); }
			RGB9_E5f_Rev (const FloatFormat &c)			{ data.Set( c[0], c[1], c[2] ); }

			operator FloatFormat () const				{ return vec3_t(data).To<FloatFormat>(); }
			operator FloatColorFormatInfo () const		{ return FloatColorFormatInfo::PackedType( FloatFormat(*this) ); }
			
			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( FloatFormat(*this) == FloatFormat(right) );
			}
		};

		// TODO: RGB9_E5f


		//
		// Red
		//
		template <typename T, bool Normalized = false>
		struct RedF : BaseColor<T, Normalized>
		{
			T	r;

			RedF () : r(0) {}
			RedF (T R) : r(R) {}
			RedF (const FloatFormat &c)				{ r = c[0]; }

			operator FloatFormat () const			{ return FloatFormat( r, 0, 0, 0 ); }
			operator FloatColorFormatInfo () const	{ return FloatColorFormatInfo::SimpleType<T>(); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( FloatFormat(*this) == FloatFormat(right) );
			}
		};


		//
		// RG
		//
		template <typename T, bool Normalized = false>
		struct RGf : BaseColor<T, Normalized>
		{
			T	r, g;

			RGf () : r(0), g(0)						{}
			RGf (T R, T G) : r(R), g(G)				{}
			RGf (const FloatFormat &c)				{ r = c[0];  g = c[1]; }

			operator FloatFormat () const			{ return FloatFormat( r, g, 0, 0 ); }
			operator FloatColorFormatInfo () const	{ return FloatColorFormatInfo::SimpleType<T>(); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( FloatFormat(*this) == FloatFormat(right) );
			}
		};


		//
		// RGB
		//
		template <typename T, bool Normalized = false>
		struct RGBf : BaseColor<T, Normalized>
		{
			T	r, g, b;

			RGBf () : r(0), g(0), b(0)				{}
			RGBf (T R, T G, T B) : r(R), g(G), b(B) {}
			RGBf (const FloatFormat &c)				{ r = c[0];  g = c[1];  b = c[2]; }

			operator FloatFormat () const			{ return FloatFormat( r, g, b, 0 ); }
			operator FloatColorFormatInfo () const	{ return FloatColorFormatInfo::SimpleType<T>(); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( FloatFormat(*this) == FloatFormat(right) );
			}
		};


		//
		// RGBA
		//
		template <typename T, bool Normalized = false>
		struct RGBAf : BaseColor<T, Normalized>
		{
			T	r, g, b, a;

			RGBAf () : r(0), g(0), b(0), a(0) {}
			RGBAf (T R, T G, T B, T A) : r(R), g(G), b(B), a(A) {}
			RGBAf (const FloatFormat &c)			{ r = c[0];  g = c[1];  b = c[2];  a = c[3]; }

			operator FloatFormat () const			{ return FloatFormat( r, g, b, a ); }
			operator FloatColorFormatInfo () const	{ return FloatColorFormatInfo::SimpleType<T>(); }

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( FloatFormat(*this) == FloatFormat(right) );
			}
		};


#	pragma warning (pop)

	}	// _color_hidden_

	
	namespace ColorFormat
	{
		// unnormalized formats
		typedef _color_hidden_::RedI<ubyte>		R8u;
		typedef _color_hidden_::RedI<ushort>	R16u;
		typedef _color_hidden_::RedI<uint>		R32u;
		typedef _color_hidden_::RedI<ulong>		R64u;
		typedef _color_hidden_::RedI<byte>		R8i;
		typedef _color_hidden_::RedI<short>		R16i;
		typedef _color_hidden_::RedI<int>		R32i;
		typedef _color_hidden_::RedI<ilong>		R64i;
	
		typedef _color_hidden_::RGi<ubyte>		RG8u;
		typedef _color_hidden_::RGi<ushort>		RG16u;
		typedef _color_hidden_::RGi<uint>		RG32u;
		typedef _color_hidden_::RGi<ulong>		RG64u;
		typedef _color_hidden_::RGi<byte>		RG8i;
		typedef _color_hidden_::RGi<short>		RG16i;
		typedef _color_hidden_::RGi<int>		RG32i;
		typedef _color_hidden_::RGi<ilong>		RG64i;

		typedef _color_hidden_::RGBi<ubyte>		RGB8u;
		typedef _color_hidden_::RGBi<ushort>	RGB16u;
		typedef _color_hidden_::RGBi<uint>		RGB32u;
		typedef _color_hidden_::RGBi<ulong>		RGB64u;
		typedef _color_hidden_::RGBi<byte>		RGB8i;
		typedef _color_hidden_::RGBi<short>		RGB16i;
		typedef _color_hidden_::RGBi<int>		RGB32i;
		typedef _color_hidden_::RGBi<ilong>		RGB64i;
	
		typedef _color_hidden_::RGBAi<ubyte>	RGBA8u;
		typedef _color_hidden_::RGBAi<ushort>	RGBA16u;
		typedef _color_hidden_::RGBAi<uint>		RGBA32u;
		typedef _color_hidden_::RGBAi<ulong>	RGBA64u;
		typedef _color_hidden_::RGBAi<byte>		RGBA8i;
		typedef _color_hidden_::RGBAi<short>	RGBA16i;
		typedef _color_hidden_::RGBAi<int>		RGBA32i;
		typedef _color_hidden_::RGBAi<ilong>	RGBA64i;
	
		typedef _color_hidden_::RedF<half,		false>	R16f;
		typedef _color_hidden_::RedF<float,		false>	R32f;
		typedef _color_hidden_::RedF<double,	false>	R64f;
	
		typedef _color_hidden_::RGf<half,		false>	RG16f;
		typedef _color_hidden_::RGf<float,		false>	RG32f;
		typedef _color_hidden_::RGf<double,		false>	RG64f;
	
		typedef _color_hidden_::RGBf<half,		false>	RGB16f;
		typedef _color_hidden_::RGBf<float,		false>	RGB32f;
		typedef _color_hidden_::RGBf<double,	false>	RGB64f;
	
		typedef _color_hidden_::RGBAf<half,		false>	RGBA16f;
		typedef _color_hidden_::RGBAf<float,	false>	RGBA32f;
		typedef _color_hidden_::RGBAf<double,	false>	RGBA64f;
		
		typedef _color_hidden_::RGBA4u<false>			RGBA4u;
		typedef _color_hidden_::RGB5_A1u<false>			RGB5_A1u;
		typedef _color_hidden_::R5_G6_B5u<false>		R5_G6_B5u;
		typedef _color_hidden_::RGBA4u_Rev<false>		RGBA4u_Rev;
		typedef _color_hidden_::RGB5_A1u_Rev<false>		RGB5_A1u_Rev;
		typedef _color_hidden_::R5_G6_B5u_Rev<false>	R5_G6_B5u_Rev;

		typedef _color_hidden_::RGB10_A2u<false>		RGB10_A2u;
		typedef _color_hidden_::RGB10_A2u_Rev<false>	RGB10_A2u_Rev;
		typedef _color_hidden_::R11_G11_B10f<false>		R11_G11_B10f;
		typedef _color_hidden_::RGB9_E5f_Rev<false>		RGB9_E5f_Rev;


		// normalized formats
		typedef _color_hidden_::RedI<ubyte,   true>		R8_UNorm;
		typedef _color_hidden_::RedI<ushort,  true>		R16_UNorm;
		typedef _color_hidden_::RGi<ubyte,    true>		RG8_UNorm;
		typedef _color_hidden_::RGi<ushort,   true>		RG16_UNorm;
		typedef _color_hidden_::RGBi<ubyte,   true>		RGB8_UNorm;
		typedef _color_hidden_::RGBi<ushort,  true>		RGB16_UNorm;
		typedef _color_hidden_::RGBAi<ubyte,  true>		RGBA8_UNorm;
		typedef _color_hidden_::RGBAi<ushort, true>		RGBA16_UNorm;

		typedef _color_hidden_::RGBA4u<true>			RGBA4_UNorm;
		typedef _color_hidden_::RGB5_A1u<true>			RGB5_A1_UNorm;
		typedef _color_hidden_::R5_G6_B5u<true>			R5_G6_B5_UNorm;
		typedef _color_hidden_::RGB10_A2u<true>			RGB10_A2_UNorm;
		typedef _color_hidden_::RGBA4u_Rev<true>		RGBA4_Rev_UNorm;
		typedef _color_hidden_::RGB5_A1u_Rev<true>		RGB5_A1_Rev_UNorm;
		typedef _color_hidden_::R5_G6_B5u_Rev<true>		R5_G6_B5_Rev_UNorm;
		typedef _color_hidden_::RGB10_A2u_Rev<true>		RGB10_A2_Rev_UNorm;

		typedef _color_hidden_::RedI<byte,    true>		R8_SNorm;
		typedef _color_hidden_::RedI<short,   true>		R16_SNorm;
		typedef _color_hidden_::RGi<byte,     true>		RG8_SNorm;
		typedef _color_hidden_::RGi<short,    true>		RG16_SNorm;
		typedef _color_hidden_::RGBi<byte,    true>		RGB8_SNorm;
		typedef _color_hidden_::RGBi<short,   true>		RGB16_SNorm;
		typedef _color_hidden_::RGBAi<byte,   true>		RGBA8_SNorm;
		typedef _color_hidden_::RGBAi<short,  true>		RGBA16_SNorm;
	
		typedef _color_hidden_::RedF<half,		true>	R16f_Norm;
		typedef _color_hidden_::RedF<float,		true>	R32f_Norm;
		typedef _color_hidden_::RedF<double,	true>	R64f_Norm;
	
		typedef _color_hidden_::RGf<half,		true>	RG16f_Norm;
		typedef _color_hidden_::RGf<float,		true>	RG32f_Norm;
		typedef _color_hidden_::RGf<double,		true>	RG64f_Norm;
	
		typedef _color_hidden_::RGBf<half,		true>	RGB16f_Norm;
		typedef _color_hidden_::RGBf<float,		true>	RGB32f_Norm;
		typedef _color_hidden_::RGBf<double,	true>	RGB64f_Norm;
	
		typedef _color_hidden_::RGBAf<half,		true>	RGBA16f_Norm;
		typedef _color_hidden_::RGBAf<float,	true>	RGBA32f_Norm;
		typedef _color_hidden_::RGBAf<double,	true>	RGBA64f_Norm;

		typedef _color_hidden_::R11_G11_B10f<true>		R11_G11_B10f_Norm;
		typedef _color_hidden_::RGB9_E5f_Rev<true>		RGB9_E5f_Rev_Norm;
		

		typedef _color_hidden_::IntFormat				IntFormat;
		typedef	_color_hidden_::FloatFormat				FloatFormat;

		typedef _color_hidden_::IntColorFormatInfo		IntColorFormatInfo;
		typedef _color_hidden_::FloatColorFormatInfo	FloatColorFormatInfo;
		typedef _color_hidden_::ColorFormatConverter	ColorFormatConverter;


		


		
		//
		// Color Swizzle
		//
		
		template <typename T, ubyte R, ubyte G, ubyte B, ubyte A>
		struct ColorSwizzle
		{
		// constants
			static const uint	format		= T::format;
			static const bool	_is_float	= (format & _color_hidden_::EColorFormatDescriptor::FLOAT) != 0;

		// types
			typedef T															color_t;
			typedef ColorSwizzle< T, R, G, B, A >								Self;

			typedef typename CompileTime::SwitchType<
					_is_float, FloatFormat, IntFormat >							format_t;

			typedef typename CompileTime::SwitchType<
					_is_float, FloatColorFormatInfo, IntColorFormatInfo >		info_t;

		// variables
			color_t		_color;

		// methods
			ColorSwizzle () {}

			ColorSwizzle (const format_t &c) : _color( format_t( c[R&3], c[G&3], c[B&3], c[A&3] ) ) {}

			operator format_t () const
			{
				format_t	c( _color );
				return format_t( c[R&3], c[G&3], c[B&3], c[A&3] );
			}

			operator info_t () const
			{
				return info_t( _color ).template GetSwizzledFormat< R, G, B, A >();
			}

			template <typename ColorType>
			bool operator == (const ColorType &right) const
			{
				return All( format_t(*this) == format_t(right) );
			}
		};

	}	// ColorFormat


}	// GXMath
}	// GX_STL
