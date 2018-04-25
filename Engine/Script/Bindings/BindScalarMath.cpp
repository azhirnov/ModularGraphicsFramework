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
	using namespace GXMath;

/*
=================================================
	ScalarFunc
=================================================
*/
	struct ScalarFunc
	{
		template <typename T>
		static T Sin (T value) {
			return GXMath::Sin( Radians<T>{value} );
		}

		template <typename T>
		static T Cos (T value) {
			return GXMath::Cos( Radians<T>{value} );
		}

		template <typename T>
		static T Tan (T value) {
			return GXMath::Tan( Radians<T>{value} );
		}

		template <typename T>
		static T Square (T value) {
			return GXMath::Square( value );
		}

		template <typename T>
		static T Sqrt (T value) {
			return GXMath::Sqrt( value );
		}

		template <typename T>
		static T Lerp (T x, T y, T factor) {
			return GXMath::Lerp( x, y, factor );
		}

		template <typename T>
		static T Rand_FloatRange (T min, T max) {
			return ImprovedRandom::FloatRange( min, max );
		}
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
		se->AddFunction( &ScalarFunc::Square<T>,	"Square" );
	}
	
/*
=================================================
	BindFloatScalar
=================================================
*/
	template <typename T>
	static void BindFloatScalar (ScriptEngine *se)
	{
		// trigonometry
		se->AddFunction( &ScalarFunc::Sin<T>,		"Sin" );
		se->AddFunction( &ScalarFunc::Cos<T>,		"Cos" );
		se->AddFunction( &ScalarFunc::Tan<T>,		"Tan" );

		se->AddFunction( &ScalarFunc::Square<T>,	"Square" );
		se->AddFunction( &ScalarFunc::Sqrt<T>,		"Sqrt" );

		// interpolation
		se->AddFunction( &ScalarFunc::Lerp<T>,		"Lerp" );
		
		// random
		se->AddFunction( &ScalarFunc::Rand_FloatRange<T>,	"Rand_FloatRange" );
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
