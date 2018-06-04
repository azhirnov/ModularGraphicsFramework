// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Script/Bindings/DefaultBindings.h"
#include "Engine/Script/Impl/ClassBinder.h"
#include "Engine/Script/Impl/ScriptEngine.inl.h"

#include "Engine/STL/Math/Mathematics.h"
#include "Engine/STL/Math/Interpolations.h"
#include "Engine/STL/Math/Trigonometry.h"
#include "Engine/STL/Math/BinaryMath.h"

namespace GX_STL
{
namespace GXScript
{
	using namespace GX_STL::GXMath;

/*
=================================================
	ScalarFunc
=================================================
*/
	struct ScalarFunc
	{
		template <typename T>	static T Abs (T value)						{ return GXMath::Abs( value ); }
		template <typename T>	static T Floor (T value)					{ return GXMath::Floor( value ); }
		template <typename T>	static T Ceil (T value)						{ return GXMath::Ceil( value ); }
		template <typename T>	static T Trunc (T value)					{ return GXMath::Trunc( value ); }
		template <typename T>	static T Fract (T value)					{ return GXMath::Fract( value ); }
		template <typename T>	static T Round (T value)					{ return GXMath::Round( value ); }
		template <typename T>	static T RoundTo (T x, T base)				{ return GXMath::RoundTo( x, base ); }

		template <typename T>	static T Pow (T x, T y)						{ return GXMath::Pow( x, y ); }
		template <typename T>	static T Ln (T x)							{ return GXMath::Ln( x ); }
		template <typename T>	static T Log (T x, T base)					{ return GXMath::Log( x, base ); }
		template <typename T>	static T Log2 (T x)							{ return GXMath::Log2( x ); }
		template <typename T>	static T Log10 (T x)						{ return GXMath::Log10( x ); }
		template <typename T>	static T IntLog2 (T x)						{ return GXMath::IntLog2( x ); }
		template <typename T>	static T Exp (T x)							{ return GXMath::Exp( x ); }
		template <typename T>	static T Exp2 (T x)							{ return GXMath::Exp2( x ); }
		template <typename T>	static T Exp10 (T x)						{ return GXMath::Exp10( x ); }

		template <typename T>	static T Sin (T value)						{ return GXMath::Sin( Radians<T>{value} ); }
		template <typename T>	static T SinH (T value)						{ return GXMath::SinH( Radians<T>{value} ); }
		template <typename T>	static T ASin (T value)						{ return (T) GXMath::ASin( value ); }
		template <typename T>	static T Cos (T value)						{ return GXMath::Cos( Radians<T>{value} ); }
		template <typename T>	static T CosH (T value)						{ return GXMath::CosH( Radians<T>{value} ); }
		template <typename T>	static T ACos (T value)						{ return (T) GXMath::ACos( value ); }
		template <typename T>	static T Tan (T value)						{ return GXMath::Tan( Radians<T>{value} ); }
		template <typename T>	static T TanH (T value)						{ return GXMath::TanH( Radians<T>{value} ); }
		template <typename T>	static T ATan (T y, T x)					{ return (T) GXMath::ATan( y, x ); }

		template <typename T>	static T Square (T value)					{ return GXMath::Square( value ); }
		template <typename T>	static T Sqrt (T value)						{ return GXMath::Sqrt( value ); }
		template <typename T>	static T Mod (T x, T y)						{ return GXMath::Mod( x, y ); }

		template <typename T>	static T Lerp (T x, T y, T factor)			{ return GXMath::Lerp( x, y, factor ); }
		template <typename T>	static T Step (T x, T edge)					{ return GXMath::Step( x, edge ); }
		template <typename T>	static T LinearStep (T x, T e0, T e1)		{ return GXMath::LinearStep( x, e0, e1 ); }
		template <typename T>	static T SmoothStep (T x, T e0, T e1)		{ return GXMath::SmoothStep( x, e0, e1 ); }
		template <typename T>	static T BumpStep (T x, T e0, T e1)			{ return GXMath::BumpStep( x, e0, e1 ); }
		template <typename T>	static T SmoothBumpStep (T x, T e0, T e1)	{ return GXMath::SmoothBumpStep( x, e0, e1 ); }

		template <typename T>	static T Min (T x, T y)						{ return GXMath::Min( x, y ); }
		template <typename T>	static T MinAbs (T x, T y)					{ return GXMath::MinAbs( x, y ); }
		template <typename T>	static T MinMag (T x, T y)					{ return GXMath::MinMag( x, y ); }
		template <typename T>	static T Max (T x, T y)						{ return GXMath::Max( x, y ); }
		template <typename T>	static T MaxAbs (T x, T y)					{ return GXMath::MaxAbs( x, y ); }
		template <typename T>	static T MaxMag (T x, T y)					{ return GXMath::MaxMag( x, y ); }
		template <typename T>	static T Clamp (T x, T min, T max)			{ return GXMath::Clamp( x, min, max ); }
		template <typename T>	static T ClampOut (T x, T min, T max)		{ return GXMath::ClampOut( x, min, max ); }
		template <typename T>	static T Wrap (T x, T min, T max)			{ return GXMath::Wrap( x, min, max ); }

		template <typename T>	static T Rand_FloatRange (T min, T max)		{ return ImprovedRandom::FloatRange( min, max ); }
		template <typename T>	static T Rand_UNorm ()						{ return ImprovedRandom::UNorm<T>(); }
		template <typename T>	static T Rand_SNorm ()						{ return ImprovedRandom::SNorm<T>(); }
		template <typename T>	static T Rand_Float ()						{ return ImprovedRandom::Float<T>(); }

		template <typename T>	static T Rand_IntRange (T min, T max)		{ return ImprovedRandom::IntRange( min, max ); }
		template <typename T>	static T Rand_Int ()						{ return ImprovedRandom::Int<T>(); }

		template <typename T>	static T BitRotateLeft (T x, T shift)		{ return GXMath::BitRotateLeft( x, BitsU(usize(shift)) ); }
		template <typename T>	static T BitRotateRight (T x, T shift)		{ return GXMath::BitRotateRight( x, BitsU(usize(shift)) ); }
		template <typename T>	static T ReverseBitOrder (T x)				{ return GXMath::ReverseBitOrder( x ); }
	};
	
/*
=================================================
	BindBoolScalar
=================================================
*/
	template <typename T>
	static void BindBoolScalar (ScriptEngine *se)
	{
	}
	
/*
=================================================
	BindIntScalar
=================================================
*/
	template <typename T>
	static void BindIntScalar (ScriptEngine *se)
	{
		const String	suffix =	CompileTime::IsSameTypes<T,int>		? "I" :
									CompileTime::IsSameTypes<T,uint>	? "U" :
									CompileTime::IsSameTypes<T,ilong>	? "L" :
									CompileTime::IsSameTypes<T,ulong>	? "J" : "";

		se->AddFunction( &ScalarFunc::IntLog2<T>,			"IntLog2" );
		
		se->AddFunction( &ScalarFunc::BitRotateLeft<T>,		"BitRotateLeft" );
		se->AddFunction( &ScalarFunc::BitRotateRight<T>,	"BitRotateRight" );
		se->AddFunction( &ScalarFunc::ReverseBitOrder<T>,	"ReverseBitOrder" );
		
		// random
		se->AddFunction( &ScalarFunc::Rand_IntRange<T>,	"Rand_IntRange" );

		if_constexpr( CompileTime::IsSameTypes<T,int> )
		{
			se->AddFunction( &ScalarFunc::Rand_Int<T>,	"Rand_Int" );
		}
		if_constexpr( CompileTime::IsSameTypes<T,uint> )
		{
			se->AddFunction( &ScalarFunc::Rand_Int<T>,	"Rand_UInt" );
		}
		if_constexpr( CompileTime::IsSameTypes<T,ilong> )
		{
			se->AddFunction( &ScalarFunc::Rand_Int<T>,	"Rand_ILong" );
		}
		if_constexpr( CompileTime::IsSameTypes<T,ulong> )
		{
			se->AddFunction( &ScalarFunc::Rand_Int<T>,	"Rand_ULong" );
		}
	}
	
/*
=================================================
	BindFloatScalar
=================================================
*/
	template <typename T>
	static void BindFloatScalar (ScriptEngine *se)
	{
		const String	suffix = CompileTime::IsSameTypes<T,float> ? "" : "D";
		
		se->AddFunction( &ScalarFunc::Floor<T>,		"Floor" );
		se->AddFunction( &ScalarFunc::Ceil<T>,		"Ceil" );
		se->AddFunction( &ScalarFunc::Trunc<T>,		"Trunc" );
		se->AddFunction( &ScalarFunc::Fract<T>,		"Fract" );
		se->AddFunction( &ScalarFunc::Round<T>,		"Round" );
		se->AddFunction( &ScalarFunc::RoundTo<T>,	"RoundTo" );
		se->AddFunction( &ScalarFunc::Mod<T>,		"Mod" );
		
		se->AddFunction( &ScalarFunc::Sqrt<T>,		"Sqrt" );
		se->AddFunction( &ScalarFunc::Pow<T>,		"Pow" );
		se->AddFunction( &ScalarFunc::Ln<T>,		"Ln" );
		se->AddFunction( &ScalarFunc::Log<T>,		"Log" );
		se->AddFunction( &ScalarFunc::Log2<T>,		"Log2" );
		se->AddFunction( &ScalarFunc::Log10<T>,		"Log10" );
		se->AddFunction( &ScalarFunc::Exp<T>,		"Exp" );
		se->AddFunction( &ScalarFunc::Exp2<T>,		"Exp2" );
		se->AddFunction( &ScalarFunc::Exp10<T>,		"Exp10" );

		// trigonometry
		se->AddFunction( &ScalarFunc::Sin<T>,		"Sin" );
		se->AddFunction( &ScalarFunc::SinH<T>,		"SinH" );
		se->AddFunction( &ScalarFunc::ASin<T>,		"ASin" );
		se->AddFunction( &ScalarFunc::Cos<T>,		"Cos" );
		se->AddFunction( &ScalarFunc::CosH<T>,		"CosH" );
		se->AddFunction( &ScalarFunc::ACos<T>,		"ACos" );
		se->AddFunction( &ScalarFunc::Tan<T>,		"Tan" );
		se->AddFunction( &ScalarFunc::TanH<T>,		"TanH" );
		se->AddFunction( &ScalarFunc::ATan<T>,		"ATan" );

		// interpolation
		se->AddFunction( &ScalarFunc::Lerp<T>,				"Lerp" );
		se->AddFunction( &ScalarFunc::Step<T>,				"Step" );
		se->AddFunction( &ScalarFunc::LinearStep<T>,		"LinearStep" );
		se->AddFunction( &ScalarFunc::SmoothStep<T>,		"SmoothStep" );
		se->AddFunction( &ScalarFunc::BumpStep<T>,			"BumpStep" );
		se->AddFunction( &ScalarFunc::SmoothBumpStep<T>,	"SmoothBumpStep" );
		
		// random
		se->AddFunction( &ScalarFunc::Rand_FloatRange<T>,	"Rand_FloatRange" );
		se->AddFunction( &ScalarFunc::Rand_UNorm<T>,		"Rand_UNorm" + suffix );
		se->AddFunction( &ScalarFunc::Rand_SNorm<T>,		"Rand_SNorm" + suffix );

		if_constexpr( CompileTime::IsSameTypes<T,float> )
		{
			se->AddFunction( &ScalarFunc::Rand_Float<T>,	"Rand_Float" );
		}
		if_constexpr( CompileTime::IsSameTypes<T,double> )
		{
			se->AddFunction( &ScalarFunc::Rand_Float<T>,	"Rand_Double" );
		}
	}
	
/*
=================================================
	BindIntFloatScalar
=================================================
*/
	template <typename T>
	static void BindIntFloatScalar (ScriptEngine *se)
	{
		if_constexpr( CompileTime::IsSigned<T> )
		{
			se->AddFunction( &ScalarFunc::Abs<T>,	"Abs" );
		}

		se->AddFunction( &ScalarFunc::Square<T>,	"Square" );

		se->AddFunction( &ScalarFunc::Min<T>,		"Min" );
		se->AddFunction( &ScalarFunc::MinAbs<T>,	"MinAbs" );
		se->AddFunction( &ScalarFunc::MinMag<T>,	"MinMag" );
		se->AddFunction( &ScalarFunc::Max<T>,		"Max" );
		se->AddFunction( &ScalarFunc::MaxAbs<T>,	"MaxAbs" );
		se->AddFunction( &ScalarFunc::MaxMag<T>,	"MaxMag" );
		se->AddFunction( &ScalarFunc::Clamp<T>,		"Clamp" );
		se->AddFunction( &ScalarFunc::ClampOut<T>,	"ClampOut" );
		se->AddFunction( &ScalarFunc::Wrap<T>,		"Wrap" );
	}

/*
=================================================
	BindScalar_Func
=================================================
*/
	struct BindScalar_Func
	{
		ScriptEngine *	_se;

		explicit BindScalar_Func (ScriptEngine *se) : _se{se}
		{}

		template <typename T, usize Index>
		void Process ()
		{
			if_constexpr( CompileTime::IsSameTypes< T, bool > )
			{
				BindBoolScalar<T>( _se );
			}
			else
			{
				BindIntFloatScalar<T>( _se );

				if_constexpr( CompileTime::IsInteger< T > )
				{
					BindIntScalar<T>( _se );
				}
				else
				if_constexpr( CompileTime::IsFloat< T > )
				{
					BindFloatScalar<T>( _se );
				}
			}
		}
	};

/*
=================================================
	BindScalarMath
=================================================
*/
	void DefaultBindings::BindScalarMath (ScriptEngine *se)
	{
		using ScalarTypes = CompileTime::TypeListFrom<
								int, uint, ilong, ulong, float, double
							>;
		
		BindScalar_Func	func{ se };
		
		ScalarTypes::RuntimeForEach( func );

		se->AddConstProperty( Pi<real>,	"Pi" );
	}
	

}	// GXScript
}	// GX_STL
