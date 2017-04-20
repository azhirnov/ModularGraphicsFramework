// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "DefaultBindings.h"

#ifdef GX_ANGELSCRIPT_ENABLED

namespace GX_STL
{
namespace GXScript
{
	
	namespace _def_bindings_hidden_
	{
		using namespace GXMath;
		

		
		template <typename T>
		struct InitVecFields
		{
			static void Init (ClassBinder<T> &binder)
			{
				STATIC_WARNING( "not supported" );
			}
		};


		template <typename T>
		struct BaseVecCtors
		{
			static void Ctor1 (void *mem, typename T::value_t value)
			{
				new( mem ) T( value );
			}
			
			static void Ctor2 (void *mem, Vec< typename T::value_t, 2 > value)
			{
				new( mem ) T( value );
			}
			
			static void Ctor3 (void *mem, Vec< typename T::value_t, 3 > value)
			{
				new( mem ) T( value );
			}
			
			static void Ctor4 (void *mem, Vec< typename T::value_t, 4 > value)
			{
				new( mem ) T( value );
			}
		};

		
		template <typename T>
		struct InitVecFields< Vec< T, 2 > > : BaseVecCtors< Vec< T, 2 > >
		{
			static void CtorArg2 (void *mem, T x, T y)
			{
				new( mem ) Vec<T,2>( x, y );
			}
			
			static void Init (ClassBinder< Vec< T, 2 > > &binder)
			{
				typedef Vec< T, 2 >	vec_t;
				typedef T			value_t;

				binder.AddProperty( &vec_t::x, "x" );
				binder.AddProperty( &vec_t::y, "y" );

				binder.AddConstructor( &Ctor1 );
				binder.AddConstructor( &Ctor3 );
				binder.AddConstructor( &Ctor4 );
				binder.AddConstructor( &CtorArg2 );
			}
		};

		
		template <typename T>
		struct InitVecFields< Vec< T, 3 > > : BaseVecCtors< Vec< T, 3 > >
		{
			static void CtorArg2 (void *mem, const Vec<T,2> &xy, T z)
			{
				new( mem ) Vec<T,3>( xy, z );
			}

			static void CtorArg3 (void *mem, T x, T y, T z)
			{
				new( mem ) Vec<T,3>( x, y, z );
			}

			static void Init (ClassBinder< Vec< T, 3 > > &binder)
			{
				typedef Vec< T, 3 >	vec_t;
				typedef T			value_t;

				binder.AddProperty( &vec_t::x, "x" );
				binder.AddProperty( &vec_t::y, "y" );
				binder.AddProperty( &vec_t::z, "z" );

				binder.AddConstructor( &Ctor1 );
				binder.AddConstructor( &Ctor2 );
				binder.AddConstructor( &Ctor4 );
				binder.AddConstructor( &CtorArg2 );
				binder.AddConstructor( &CtorArg3 );
			}
		};

		
		template <typename T>
		struct InitVecFields< Vec< T, 4 > > : BaseVecCtors< Vec< T, 4 > >
		{
			static void CtorArg2 (void *mem, const Vec<T,2> &xy, const Vec<T,2> &zw)
			{
				new( mem ) Vec<T,4>( xy, zw );
			}
			
			static void CtorArg3 (void *mem, const Vec<T,3> &xyz, T w)
			{
				new( mem ) Vec<T,4>( xyz, w );
			}

			static void CtorArg4 (void *mem, T x, T y, T z, T w)
			{
				new( mem ) Vec<T,4>( x, y, z, w );
			}

			static void Init (ClassBinder< Vec< T, 4 > > &binder)
			{
				typedef Vec< T, 4 >	vec_t;
				typedef T			value_t;

				binder.AddProperty( &vec_t::x, "x" );
				binder.AddProperty( &vec_t::y, "y" );
				binder.AddProperty( &vec_t::z, "z" );
				binder.AddProperty( &vec_t::w, "w" );

				binder.AddConstructor( &Ctor1 );
				binder.AddConstructor( &Ctor2 );
				binder.AddConstructor( &Ctor3 );
				binder.AddConstructor( &CtorArg2 );
				binder.AddConstructor( &CtorArg3 );
				binder.AddConstructor( &CtorArg4 );
			}
		};


		template <typename T, usize I>
		static bool CompareVectors (const Vec<T,I> &left, const Vec<T,I> &right)
		{
			return All( left == right );
		}


		template <typename T>
		void BindVecFloatOperators (ClassBinder<T> &binder)
		{
			typedef typename ClassBinder<T>::type			vec_t;
			typedef typename vec_t::value_t					value_t;
			typedef typename ClassBinder<T>::OperatorBinder	op_t;
			
			binder.Operators()
				.PreInc< vec_t &>()
				.PreDec< vec_t &>()
				.PostInc< const vec_t >()
				.PostDec< const vec_t >()

				//.Assign< const vec_t &>()
				.AddAssign< const vec_t &>()					.AddAssign< const value_t &>()
				.SubAssign< const vec_t &>()					.SubAssign< const value_t &>()
				.MulAssign< const vec_t &>()					.MulAssign< const value_t &>()
				.DivAssign< const vec_t &>()					.DivAssign< const value_t &>()
				.ModAssign< const vec_t &>()					.ModAssign< const value_t &>()

				.Add< const vec_t &, const vec_t >()			.Add< const value_t &, const vec_t >()
				.Sub< const vec_t &, const vec_t >()			.Sub< const value_t &, const vec_t >()
				.Mul< const vec_t &, const vec_t >()			.Mul< const value_t &, const vec_t >()
				.Div< const vec_t &, const vec_t >()			.Div< const value_t &, const vec_t >()
				.Mod< const vec_t &, const vec_t >()			.Mod< const value_t &, const vec_t >()

				.BinaryRight( op_t::OP_ADD, static_cast< const vec_t (*) (const value_t&, const vec_t&) >( &operator + ) )
				.BinaryRight( op_t::OP_SUB, static_cast< const vec_t (*) (const value_t&, const vec_t&) >( &operator - ) )
				.BinaryRight( op_t::OP_MUL, static_cast< const vec_t (*) (const value_t&, const vec_t&) >( &operator * ) )
				.BinaryRight( op_t::OP_DIV, static_cast< const vec_t (*) (const value_t&, const vec_t&) >( &operator / ) )
				.BinaryRight( op_t::OP_MOD, static_cast< const vec_t (*) (const value_t&, const vec_t&) >( &operator % ) )
				
				.Index( static_cast< const value_t& (vec_t::*)(usize) const >( &vec_t::operator [] ) )
				.Index( static_cast< value_t& (vec_t::*)(usize) >( &vec_t::operator [] ) )

				.Equals< const vec_t& >( &CompareVectors )
				;
		}


		template <typename T>
		void BindVecIntegerOperators (ClassBinder<T> &binder)
		{
			typedef typename ClassBinder<T>::type	vec_t;
			typedef typename vec_t::value_t			value_t;

			BindVecFloatOperators( binder );
			
			binder.Operators()
				.Inverse< const vec_t >()
				.Not< const vec_t >()

				.AndAssign< const vec_t &>()					.AndAssign< const value_t &>()
				.OrAssign< const vec_t &>()						.OrAssign< const value_t &>()
				.XorAssign< const vec_t &>()					.XorAssign< const value_t &>()
				.ShiftLeftAssign< const vec_t &>()				.ShiftLeftAssign< const value_t &>()
				.ShiftRightAssign< const vec_t &>()				.ShiftRightAssign< const value_t &>()

				.Or< const vec_t &, const vec_t >()				.Or< const value_t &, const vec_t >()
				.Xor< const vec_t &, const vec_t >()			.Xor< const value_t &, const vec_t >()
				.ShiftLeft< const vec_t &, const vec_t >()		.ShiftLeft< const value_t &, const vec_t >()
				.ShiftRight< const vec_t &, const vec_t >()		.ShiftRight< const value_t &, const vec_t >()

				/*.AddR< const value_t &, const vec_t >()
				.SubR< const value_t &, const vec_t >()
				.MulR< const value_t &, const vec_t >()
				.DivR< const value_t &, const vec_t >()
				.ModR< const value_t &, const vec_t >()
				.OrR< const value_t &, const vec_t >()
				.XorR< const value_t &, const vec_t >()
				.ShiftLeftR< const value_t &, const vec_t >()
				.ShiftRightR< const value_t &, const vec_t >()*/
				;
		}
		
#		define _ADD_METHOD( _name_ ) \
			binder.AddMethod( &vec_t::_name_, TOSTRING( _name_ ) )

		template <typename T>
		void BindVecFloatMethods (ClassBinder<T> &binder)
		{
			typedef typename ClassBinder<T>::type	vec_t;
			typedef typename vec_t::value_t			value_t;

			_ADD_METHOD( Length );
			_ADD_METHOD( LengthSqr );
			_ADD_METHOD( Normalized );
			_ADD_METHOD( Normalize );
			_ADD_METHOD( SetLength );
			_ADD_METHOD( Dot );
			_ADD_METHOD( DotAbs );
			_ADD_METHOD( Distance );
			_ADD_METHOD( DistanceSqr );
			_ADD_METHOD( InterpolateL );
			_ADD_METHOD( InterpolateQ );
			_ADD_METHOD( GetInterpolatedL );
			_ADD_METHOD( GetInterpolatedQ );
			_ADD_METHOD( Reflect );
			_ADD_METHOD( Refract );
			_ADD_METHOD( Sum );
			_ADD_METHOD( SumAbs );
			_ADD_METHOD( Area );
			_ADD_METHOD( Volume );
			_ADD_METHOD( Min );
			_ADD_METHOD( Max );
		}

		template <typename T>
		void BindVecIntMethods (ClassBinder<T> &binder)
		{
			typedef typename ClassBinder<T>::type	vec_t;
			typedef typename vec_t::value_t			value_t;

			_ADD_METHOD( Sum );
			_ADD_METHOD( SumAbs );
			_ADD_METHOD( Area );
			_ADD_METHOD( Volume );
			_ADD_METHOD( Min );
			_ADD_METHOD( Max );
		}

#		undef _ADD_METHOD


#		define _ADD_FUNC( _type_, _name_ ) \
			se.AddFunction< _type_ >( &_name_, TOSTRING( _name_ ) )

		template <typename T, ubyte I>
		static Vec<T,I> ModVec (const Vec<T,I> &left, const Vec<T,I> &right)
		{
			return GXMath::Mod<T,T,I>( left, right );
		}

		template <typename T>
		void BindScalarAndVecFloatFuncs (Ptr< AngelScript::asIScriptEngine > eng)
		{
			ScriptEngine	se( eng );

			_ADD_FUNC( T (*) (const T &),			Sin		);
			_ADD_FUNC( T (*) (const T &),			Cos		);
			_ADD_FUNC( T (*) (const T &),			ASin	);
			_ADD_FUNC( T (*) (const T &),			ACos	);
			_ADD_FUNC( T (*) (const T &),			SinH	);
			_ADD_FUNC( T (*) (const T &),			CosH	);
			_ADD_FUNC( T (*) (const T &),			ASinH	);
			_ADD_FUNC( T (*) (const T &),			ACosH	);
			_ADD_FUNC( T (*) (const T &),			Tan		);
			_ADD_FUNC( T (*) (const T &),			CoTan	);
			_ADD_FUNC( T (*) (const T &),			ATan	);

			_ADD_FUNC( T (*) (const T &, const T &),	ATan	);

			_ADD_FUNC( T (*) (const T &),			ACoTan	);
			_ADD_FUNC( T (*) (const T &),			ATanH	);
			_ADD_FUNC( T (*) (const T &),			ACoTanH );

			_ADD_FUNC( T (*) (const T &),			Abs		);
			_ADD_FUNC( T (*) (const T&, bool),		SetSign );
			_ADD_FUNC( T (*) (const T&, const T&),	CopySign );

			_ADD_FUNC( bool (*) (const T &),		All );
			_ADD_FUNC( bool (*) (const T &),		Any );
			_ADD_FUNC( bool (*) (const T &),		IsZero );
			_ADD_FUNC( bool (*) (const T &),		IsNotZero );
			
			_ADD_FUNC( T (*) (const T&, const T&),						Max );
			_ADD_FUNC( T (*) (const T&, const T&, const T&),			Max );
			_ADD_FUNC( T (*) (const T&, const T&, const T&, const T&),	Max );
			_ADD_FUNC( T (*) (const T&, const T&),						Min );
			_ADD_FUNC( T (*) (const T&, const T&, const T&),			Min );
			_ADD_FUNC( T (*) (const T&, const T&, const T&, const T&),	Min );
			_ADD_FUNC( T (*) (const T&, const T&),						MaxAbs );
			_ADD_FUNC( T (*) (const T&, const T&),						MinAbs );

			_ADD_FUNC( T (*) (const T&, const T&, const T&),	Clamp );
			_ADD_FUNC( T (*) (const T&, const T&, const T&),	ClampOut );

			_ADD_FUNC( T (*) (const T&, const T&),	Pow );
			_ADD_FUNC( T (*) (const T&),			Ln );
			_ADD_FUNC( T (*) (const T&, const T&),	Log );
			_ADD_FUNC( T (*) (const T&),			Log2 );
			_ADD_FUNC( T (*) (const T&),			Log10 );
			_ADD_FUNC( T (*) (const T&),			Exp );
			_ADD_FUNC( T (*) (const T&),			Exp2 );
			_ADD_FUNC( T (*) (const T&),			Exp10 );

			_ADD_FUNC( T (*) (const T&),			Square );
			_ADD_FUNC( T (*) (const T&),			Sqrt );
			_ADD_FUNC( T (*) (const T&),			SquareSign );
			_ADD_FUNC( T (*) (const T&),			InvSqrt );
			
			_ADD_FUNC( T (*) (const T&),			Floor );
			_ADD_FUNC( T (*) (const T&),			Ceil );
			_ADD_FUNC( T (*) (const T&),			Fract );
			_ADD_FUNC( T (*) (const T&),			Trunc );
			_ADD_FUNC( T (*) (const T&, const T&),	ModVec );

			_ADD_FUNC( T (*) (const T&, const T&, const T&),	SafeDiv );

		}
		

		template <typename T>
		void BindVecFloatFuncs (Ptr< AngelScript::asIScriptEngine > eng)
		{
			typedef typename T::value_t	V;
			enum { C = T::STATIC_COUNT };

			ScriptEngine	se( eng );

			typedef Vec<int,C>		ivec_t;
			typedef Vec<uint,C>		uvec_t;
			typedef Vec<bool,C>		bvec_t;
			
			typedef CompileTime::NearInt::FromType< V >		sint_t;
			typedef Vec< sint_t, C >						sivec_t;

			_ADD_FUNC( T (*) (const T&),		Sign );
			_ADD_FUNC( T (*) (const T&),		SignOrZero );
			
			//_ADD_FUNC( bvec_t (*) (const T&, const T&),					Equals );
			//_ADD_FUNC( bvec_t (*) (const T&, const T&, const uint&),	Equals );

			_ADD_FUNC( T (*) (const T&, const T&, const V&),			Mix );
			_ADD_FUNC( T (*) (const T&, const T&, const T&, const V&),	QMix );
			_ADD_FUNC( T (*) (const T&, const T&, const T&, const T&, const V&, const V&),	BiMix );

			//_ADD_FUNC( sivec_t (*) (const T&),			Round );

			BindScalarAndVecFloatFuncs<T>( eng );
		}
		

		template <typename T>
		void BindVecIntFuncs (Ptr< AngelScript::asIScriptEngine > eng)
		{
			typedef typename T::value_t	V;
			enum { C = T::STATIC_COUNT };

			ScriptEngine	se( eng );

			typedef Vec<int,C>		ivec_t;
			typedef Vec<uint,C>	uvec_t;
			typedef Vec<bool,C>	bvec_t;
			/*
			_ADD_FUNC( uvec_t (*) (const T&),	IntLog2 );
			_ADD_FUNC( uvec_t (*) (const T&),	CeilPowerOfTwo );
			_ADD_FUNC( uvec_t (*) (const T&),	FloorPowerOfTwo );
			_ADD_FUNC( uvec_t (*) (const T&),	NearPowerOfTwo );
			_ADD_FUNC( bvec_t (*) (const T&),	IsPowerOfTwo );*/
			/*
			_ADD_FUNC( T (*) (const ivec_t&),				ToBit );
			_ADD_FUNC( T (*) (const ivec_t&),				ARGS( ToMask<V,C,int> ) );
			_ADD_FUNC( T (*) (const T&),					GetMaskForType );
			_ADD_FUNC( T (*) (const T&, const ivec_t&),		SafeLeftBitShift );
			_ADD_FUNC( T (*) (const T&, const ivec_t&),		SafeRightBitShift );*/
			
			_ADD_FUNC( bvec_t (*) (const T&),			IsOdd );
			_ADD_FUNC( bvec_t (*) (const T&),			IsEven );
		}


		template <typename T>
		void BindScalarFloatFuncs (Ptr< AngelScript::asIScriptEngine > eng)
		{
			ScriptEngine	se( eng );

			_ADD_FUNC( T (*)(),						Pi );
			_ADD_FUNC( T (*)(),						Exp );
			_ADD_FUNC( T (*)(),						ReciporalPi );
			
			typedef Vec<T,2>	vec2_t;

			_ADD_FUNC( vec2_t (*)(const T &),		SinCos );
			_ADD_FUNC( vec2_t (*)(const T &),		ASinCos );
			_ADD_FUNC( vec2_t (*)(const T &),		SinCosH );
			_ADD_FUNC( vec2_t (*)(const T &),		ASinCosH );

			_ADD_FUNC( bool (*) (const T&, const T&),					Equals );
			_ADD_FUNC( bool (*) (const T&, const T&, const uint&),		Equals );
			
			_ADD_FUNC( uint (*) (const T&),		IntLog2 );
			_ADD_FUNC( T	(*) (const T&),		CeilPowerOfTwo );
			_ADD_FUNC( T	(*) (const T&),		FloorPowerOfTwo );
			_ADD_FUNC( T	(*) (const T&),		NearPowerOfTwo );
			_ADD_FUNC( bool (*) (const T&),		IsPowerOfTwo );
			
			_ADD_FUNC( T (*) (const T&, const T&, const T&),								Mix );
			_ADD_FUNC( T (*) (const T&, const T&, const T&, const T&),						QMix );
			_ADD_FUNC( T (*) (const T&, const T&, const T&, const T&, const T&, const T&),	BiMix );
			
			_ADD_FUNC( bool (*) (const T&),		IsNAN );
			_ADD_FUNC( bool (*) (const T&),		IsOdd );
			_ADD_FUNC( bool (*) (const T&),		IsEven );

			BindScalarAndVecFloatFuncs<T>( eng );
		}


		template <typename T>
		void BindScalarIntFuncs (Ptr< AngelScript::asIScriptEngine > eng)
		{
			ScriptEngine	se( eng );

			_ADD_FUNC( uint (*) (const T&),		IntLog2 );
			_ADD_FUNC( T	(*) (const T&),		CeilPowerOfTwo );
			_ADD_FUNC( T	(*) (const T&),		FloorPowerOfTwo );
			_ADD_FUNC( T	(*) (const T&),		NearPowerOfTwo );
			_ADD_FUNC( bool (*) (const T&),		IsPowerOfTwo );
		}

#		undef _ADD_FUNC


		// float
		template <bool IsFloat, bool isBool>
		struct VecSwitch
		{
			template <typename T>
			static void BindVec (ClassBinder<T> &binder)
			{
				BindVecFloatOperators( binder );
				BindVecFloatMethods( binder );
				BindVecFloatFuncs<T>( binder.GetEngine() );
			}
		};

		// int
		template <>
		struct VecSwitch <false, false>
		{
			template <typename T>
			static void BindVec (ClassBinder<T> &binder)
			{
				BindVecIntegerOperators( binder );
				BindVecIntMethods( binder );
				BindVecIntFuncs<T>( binder.GetEngine() );
			}
		};

		// bool
		template <>
		struct VecSwitch <false, true>
		{
			template <typename T>
			static void BindVec (ClassBinder<T> &binder)
			{
				TODO("");
				GX_UNUSED( binder );
			}
		};


		template <typename T>
		void VecForwardDecl (AngelScript::asIScriptEngine *se)
		{
			ClassBinder<T>	binder( se );

			binder.CreateClassValue();
		}


		template <typename T>
		void BindVec (AngelScript::asIScriptEngine *se)
		{
			ClassBinder<T>	binder( se );

			InitVecFields<T>::Init( binder );

			VecSwitch< CompileTime::IsFloat< typename T::value_t >(),
						CompileTime::IsSameTypes< typename T::value_t, bool >
					 >::BindVec( binder );
		}

	}	// _def_bindings_hidden_
	
/*
=================================================
	BindMath
=================================================
*/
	void DefaultBindings::BindMath (AngelScript::asIScriptEngine *se)
	{
		GX_UNUSED( se );
		TODO( "" );
	}
	
/*
=================================================
	BindVector
=================================================
*/
	void DefaultBindings::BindVector (AngelScript::asIScriptEngine *se)
	{
		_def_bindings_hidden_::VecForwardDecl< GXMath::bool2 >( se );
		_def_bindings_hidden_::VecForwardDecl< GXMath::int2 >( se );
		//_def_bindings_hidden_::VecForwardDecl< GXMath::uint2 >( se );
		_def_bindings_hidden_::VecForwardDecl< GXMath::ilong2 >( se );
		//_def_bindings_hidden_::VecForwardDecl< GXMath::ulong2 >( se );
		_def_bindings_hidden_::VecForwardDecl< GXMath::float2 >( se );
		_def_bindings_hidden_::VecForwardDecl< GXMath::double2 >( se );
		
		_def_bindings_hidden_::VecForwardDecl< GXMath::bool3 >( se );
		_def_bindings_hidden_::VecForwardDecl< GXMath::int3 >( se );
		//_def_bindings_hidden_::VecForwardDecl< GXMath::uint3 >( se );
		_def_bindings_hidden_::VecForwardDecl< GXMath::ilong3 >( se );
		//_def_bindings_hidden_::VecForwardDecl< GXMath::ulong3 >( se );
		_def_bindings_hidden_::VecForwardDecl< GXMath::float3 >( se );
		_def_bindings_hidden_::VecForwardDecl< GXMath::double3 >( se );
		
		_def_bindings_hidden_::VecForwardDecl< GXMath::bool4 >( se );
		_def_bindings_hidden_::VecForwardDecl< GXMath::int4 >( se );
		//_def_bindings_hidden_::VecForwardDecl< GXMath::uint4 >( se );
		_def_bindings_hidden_::VecForwardDecl< GXMath::ilong4 >( se );
		//_def_bindings_hidden_::VecForwardDecl< GXMath::ulong4 >( se );
		_def_bindings_hidden_::VecForwardDecl< GXMath::float4 >( se );
		_def_bindings_hidden_::VecForwardDecl< GXMath::double4 >( se );

		_def_bindings_hidden_::BindVec< GXMath::bool2 >( se );
		_def_bindings_hidden_::BindVec< GXMath::int2 >( se );
		//_def_bindings_hidden_::BindVec< GXMath::uint2 >( se );
		_def_bindings_hidden_::BindVec< GXMath::ilong2 >( se );
		//_def_bindings_hidden_::BindVec< GXMath::ulong2 >( se );
		_def_bindings_hidden_::BindVec< GXMath::float2 >( se );
		_def_bindings_hidden_::BindVec< GXMath::double2 >( se );
		
		_def_bindings_hidden_::BindVec< GXMath::bool3 >( se );
		_def_bindings_hidden_::BindVec< GXMath::int3 >( se );
		//_def_bindings_hidden_::BindVec< GXMath::uint3 >( se );
		_def_bindings_hidden_::BindVec< GXMath::ilong3 >( se );
		//_def_bindings_hidden_::BindVec< GXMath::ulong3 >( se );
		_def_bindings_hidden_::BindVec< GXMath::float3 >( se );
		_def_bindings_hidden_::BindVec< GXMath::double3 >( se );
		
		_def_bindings_hidden_::BindVec< GXMath::bool4 >( se );
		_def_bindings_hidden_::BindVec< GXMath::int4 >( se );
		//_def_bindings_hidden_::BindVec< GXMath::uint4 >( se );
		_def_bindings_hidden_::BindVec< GXMath::ilong4 >( se );
		//_def_bindings_hidden_::BindVec< GXMath::ulong4 >( se );
		_def_bindings_hidden_::BindVec< GXMath::float4 >( se );
		_def_bindings_hidden_::BindVec< GXMath::double4 >( se );
	}
	
/*
=================================================
	BindVectorMath
=================================================
*/
	void DefaultBindings::BindVectorMath (AngelScript::asIScriptEngine *se)
	{
		TODO( "" );
		GX_UNUSED( se );
	}
	
//-------------------------------------------------------------------

}	// GXScript
}	// GX_STL

#endif	// GX_ANGELSCRIPT_ENABLED
