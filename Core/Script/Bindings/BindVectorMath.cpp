// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Script/Bindings/DefaultBindings.h"
#include "Core/Script/Impl/ClassBinder.h"
#include "Core/Script/Impl/ScriptEngine.inl.h"

#include "Core/STL/Math/Mathematics.h"
#include "Core/STL/Math/Interpolations.h"
#include "Core/STL/Math/Trigonometry.h"
#include "Core/STL/Math/BinaryMath.h"

namespace GXScript
{
	using namespace GX_STL::GXMath;


	template <typename T>
	struct InitVecFields
	{};

/*
=================================================
	BaseVecCtors
=================================================
*/
	template <typename T>
	struct BaseVecCtors
	{
		static void Ctor1 (void *mem, typename T::Value_t value)
		{
			new( mem ) T( value );
		}
			
		static void Ctor2 (void *mem, Vec< typename T::Value_t, 2 > value)
		{
			new( mem ) T( value );
		}
			
		static void Ctor3 (void *mem, Vec< typename T::Value_t, 3 > value)
		{
			new( mem ) T( value );
		}
			
		static void Ctor4 (void *mem, Vec< typename T::Value_t, 4 > value)
		{
			new( mem ) T( value );
		}
	};
		
/*
=================================================
	InitVecFields (Vec2)
=================================================
*/
	template <typename T>
	struct InitVecFields< Vec< T, 2 > > : BaseVecCtors< Vec< T, 2 > >
	{
		using Base_t = BaseVecCtors< Vec< T, 2 > >;

		static void CtorArg2 (void *mem, T x, T y)
		{
			new( mem ) Vec<T,2>( x, y );
		}
			
		static void Init (ClassBinder< Vec< T, 2 > > &binder)
		{
			typedef Vec< T, 2 >	Vec_t;
			typedef T			Value_t;

			binder.AddProperty( &Vec_t::x, "x" );
			binder.AddProperty( &Vec_t::y, "y" );

			binder.AddConstructor( &Base_t::Ctor1 );
			binder.AddConstructor( &Base_t::Ctor3 );
			binder.AddConstructor( &Base_t::Ctor4 );
			binder.AddConstructor( &CtorArg2 );
		}
	};
		
/*
=================================================
	InitVecFields (Vec3)
=================================================
*/
	template <typename T>
	struct InitVecFields< Vec< T, 3 > > : BaseVecCtors< Vec< T, 3 > >
	{
		using Base_t = BaseVecCtors< Vec< T, 3 > >;

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
			typedef Vec< T, 3 >	Vec_t;
			typedef T			Value_t;

			binder.AddProperty( &Vec_t::x, "x" );
			binder.AddProperty( &Vec_t::y, "y" );
			binder.AddProperty( &Vec_t::z, "z" );

			binder.AddConstructor( &Base_t::Ctor1 );
			binder.AddConstructor( &Base_t::Ctor2 );
			binder.AddConstructor( &Base_t::Ctor4 );
			binder.AddConstructor( &CtorArg2 );
			binder.AddConstructor( &CtorArg3 );
		}
	};
		
/*
=================================================
	InitVecFields (Vec4)
=================================================
*/
	template <typename T>
	struct InitVecFields< Vec< T, 4 > > : BaseVecCtors< Vec< T, 4 > >
	{
		using Base_t = BaseVecCtors< Vec< T, 4 > >;

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
			typedef Vec< T, 4 >	Vec_t;
			typedef T			Value_t;

			binder.AddProperty( &Vec_t::x, "x" );
			binder.AddProperty( &Vec_t::y, "y" );
			binder.AddProperty( &Vec_t::z, "z" );
			binder.AddProperty( &Vec_t::w, "w" );

			binder.AddConstructor( &Base_t::Ctor1 );
			binder.AddConstructor( &Base_t::Ctor2 );
			binder.AddConstructor( &Base_t::Ctor3 );
			binder.AddConstructor( &CtorArg2 );
			binder.AddConstructor( &CtorArg3 );
			binder.AddConstructor( &CtorArg4 );
		}
	};
		
/*
=================================================
	InitVecFields (color4u)
=================================================
*
	template <>
	struct InitVecFields< color4u >
	{
		using T = color4u::Value_t;
			
		static void Ctor1 (void *mem, T value)
		{
			new( mem ) color4u( value );
		}

		static void CtorArg4 (void *mem, T x, T y, T z, T w)
		{
			new( mem ) color4u( x, y, z, w );
		}

		static void Init (ClassBinder< color4u > &binder)
		{
			typedef color4u		Vec_t;
			typedef T			Value_t;

			binder.AddProperty( &Vec_t::x, "r" );
			binder.AddProperty( &Vec_t::y, "g" );
			binder.AddProperty( &Vec_t::z, "b" );
			binder.AddProperty( &Vec_t::w, "a" );

			binder.AddConstructor( &Ctor1 );
			binder.AddConstructor( &CtorArg4 );
		}
	};
		
/*
=================================================
	InitVecFields (color4f)
=================================================
*
	template <>
	struct InitVecFields< color4f >
	{
		using T = color4f::Value_t;
			
		static void Ctor1 (void *mem, T value)
		{
			new( mem ) color4f( value );
		}

		static void CtorArg4 (void *mem, T x, T y, T z, T w)
		{
			new( mem ) color4f( x, y, z, w );
		}

		static void Init (ClassBinder< color4f > &binder)
		{
			typedef color4f		Vec_t;
			typedef T			Value_t;

			binder.AddProperty( &Vec_t::x, "r" );
			binder.AddProperty( &Vec_t::y, "g" );
			binder.AddProperty( &Vec_t::z, "b" );
			binder.AddProperty( &Vec_t::w, "a" );

			binder.AddConstructor( &Ctor1 );
			binder.AddConstructor( &CtorArg4 );
		}
	};
	
/*
=================================================
	VecFunc
=================================================
*/
	struct VecFunc final : Noninstancable
	{
		template <typename V>
		static bool Equal (const V &left, const V &right) {
			return GXMath::All( left == right );
		}

		template <typename V>
		static int Cmp (const V &left, const V &right) {
			FOR( i, left ) {
				if ( left[i] > right[i] )	return +1;
				if ( left[i] < right[i] )	return -1;
			}
			return 0;
		}

		template <typename V>
		static V Add (typename V::Value_t left, const V &right) {
			return left + right;
		}

		template <typename V>
		static V Sub (typename V::Value_t left, const V &right) {
			return left - right;
		}

		template <typename V>
		static V Mul (typename V::Value_t left, const V &right) {
			return left * right;
		}

		template <typename V>
		static V Div (typename V::Value_t left, const V &right) {
			return left / right;
		}

		template <typename V>
		static V Mod (typename V::Value_t left, const V &right) {
			return left % right;
		}

		template <typename V>
		static V And (typename V::Value_t left, const V &right) {
			return left & right;
		}

		template <typename V>
		static V Or (typename V::Value_t left, const V &right) {
			return left | right;
		}

		template <typename V>
		static V Xor (typename V::Value_t left, const V &right) {
			return left ^ right;
		}

		template <typename V>
		static V ShiftLeft (typename V::Value_t left, const V &right) {
			return left << right;
		}

		template <typename V>
		static V ShiftRight (typename V::Value_t left, const V &right) {
			return left >> right;
		}

		template <typename V>
		static bool All (const V &x) {
			return GXMath::All( x );
		}

		template <typename V>
		static bool Any (const V &x) {
			return GXMath::Any( x );
		}

		template <typename V>
		static typename V::Value_t Dot (const V &x, const V &y) {
			return GXMath::Dot( x, y );
		}

		template <typename V>
		static V Cross (const V &x, const V &y) {
			return GXMath::Cross( x, y );
		}

		template <typename V>
		static V Sin (const V &x) {
			return GXMath::Sin( x.template To< RadiansVec<typename V::Value_t, V::STATIC_COUNT> >() );
		}
		
		template <typename V>
		static V Cos (const V &x) {
			return GXMath::Cos( x.template To< RadiansVec<typename V::Value_t, V::STATIC_COUNT> >() );
		}

		template <typename V>
		static V Tan (const V &x) {
			return GXMath::Tan( x.template To< RadiansVec<typename V::Value_t, V::STATIC_COUNT> >() );
		}

		template <typename V>
		static V Square (const V &x) {
			return GXMath::Square( x );
		}

		template <typename V>
		static V Sqrt (const V &x) {
			return GXMath::Sqrt( x );
		}

		template <typename V>
		static V Lerp (const V &x, const V &y, typename V::Value_t factor) {
			return GXMath::Lerp( x, y, factor );
		}
	};

/*
=================================================
	BindBoolVec
=================================================
*/
	template <typename T>
	static void BindBoolVec (ClassBinder<T> &binder, ScriptEngine *se)
	{
		using Vec_t		= T;
		using Value_t	= typename Vec_t::Value_t;

		binder.Operators()
			.Not()
			.Equals( &VecFunc::template Equal< Vec_t > )
			.Compare( &VecFunc::template Cmp< Vec_t > );

		se->AddFunction( &VecFunc::template All< Vec_t >,	"All" );
		se->AddFunction( &VecFunc::template Any< Vec_t >,	"Any" );

		binder.AddMethod( &Vec_t::Equal,		"Equal" );
	}
	
/*
=================================================
	BindIntVec
=================================================
*/
	template <typename T>
	static void BindIntVec (ClassBinder<T> &binder, ScriptEngine *se)
	{
		using Vec_t		= T;
		using Value_t	= typename Vec_t::Value_t;

		binder.Operators()
			.Inverse()
			.PreInc()
			.PreDec()
			.PostInc()
			.PostDec()

			.template AddAssign< Vec_t >()			.template AddAssign< Value_t >()
			.template SubAssign< Vec_t >()			.template SubAssign< Value_t >()
			.template MulAssign< Vec_t >()			.template MulAssign< Value_t >()
			.template DivAssign< Vec_t >()			.template DivAssign< Value_t >()
			.template ModAssign< Vec_t >()			.template ModAssign< Value_t >()
			.template AndAssign< Vec_t >()			.template AndAssign< Value_t >()
			.template OrAssign< Vec_t >()			.template OrAssign< Value_t >()
			.template XorAssign< Vec_t >()			.template XorAssign< Value_t >()
			.template ShiftLeftAssign< Vec_t >()	.template ShiftLeftAssign< Value_t >()
			.template ShiftRightAssign< Vec_t >()	.template ShiftRightAssign< Value_t >()

			.template Add< Vec_t >()				.template Add< Value_t >()
			.template Sub< Vec_t >()				.template Sub< Value_t >()
			.template Mul< Vec_t >()				.template Mul< Value_t >()
			.template Div< Vec_t >()				.template Div< Value_t >()
			.template Mod< Vec_t >()				.template Mod< Value_t >()
			.template And< Vec_t >()				.template And< Value_t >()
			.template Or< Vec_t >()					.template Or< Value_t >()
			.template Xor< Vec_t >()				.template Xor< Value_t >()
			.template ShiftLeft< Vec_t >()			.template ShiftLeft< Value_t >()
			.template ShiftRight< Vec_t >()			.template ShiftRight< Value_t >()

			.BinaryRH( EBinaryOperator::Add,		&VecFunc::template Add< Vec_t > )
			.BinaryRH( EBinaryOperator::Sub,		&VecFunc::template Sub< Vec_t > )
			.BinaryRH( EBinaryOperator::Mul,		&VecFunc::template Mul< Vec_t > )
			.BinaryRH( EBinaryOperator::Div,		&VecFunc::template Div< Vec_t > )
			.BinaryRH( EBinaryOperator::Mod,		&VecFunc::template Mod< Vec_t > )
			.BinaryRH( EBinaryOperator::And,		&VecFunc::template And< Vec_t > )
			.BinaryRH( EBinaryOperator::Or, 		&VecFunc::template Or< Vec_t > )
			.BinaryRH( EBinaryOperator::Xor,		&VecFunc::template Xor< Vec_t > )
			.BinaryRH( EBinaryOperator::ShiftLeft,	&VecFunc::template ShiftLeft< Vec_t > )
			.BinaryRH( EBinaryOperator::ShiftRight,	&VecFunc::template ShiftRight< Vec_t > )

			.Equals( &VecFunc::template Equal< Vec_t > )
			.Compare( &VecFunc::template Cmp< Vec_t > );

		binder.AddMethod( &Vec_t::Sum,			"Sum" );
		binder.AddMethod( &Vec_t::Area,			"Area" );
		binder.AddMethod( &Vec_t::Volume,		"Volume" );
		
		binder.AddMethod( &Vec_t::Equal,		"Equal" );
		binder.AddMethod( &Vec_t::Less,			"Less" );
		binder.AddMethod( &Vec_t::LEqual,		"LEqual" );
		binder.AddMethod( &Vec_t::Greater,		"Greater" );
		binder.AddMethod( &Vec_t::GEqual,		"GEqual" );
		binder.AddMethod( &Vec_t::IsZero,		"IsZero" );

		se->AddFunction( &VecFunc::template Square<Vec_t>,	"Square" );
	}
	
/*
=================================================
	BindFloatVec
=================================================
*/
	template <typename T>
	static void BindFloatVec (ClassBinder<T> &binder, ScriptEngine *se)
	{
		using Vec_t		= T;
		using Value_t	= typename Vec_t::Value_t;

		binder.Operators()
			.PreInc()
			.PreDec()
			.PostInc()
			.PostDec()

			.template AddAssign< Vec_t >()	.template AddAssign< Value_t >()
			.template SubAssign< Vec_t >()	.template SubAssign< Value_t >()
			.template MulAssign< Vec_t >()	.template MulAssign< Value_t >()
			.template DivAssign< Vec_t >()	.template DivAssign< Value_t >()
			.template ModAssign< Vec_t >()	.template ModAssign< Value_t >()

			.template Add< Vec_t >()		.template Add< Value_t >()
			.template Sub< Vec_t >()		.template Sub< Value_t >()
			.template Mul< Vec_t >()		.template Mul< Value_t >()
			.template Div< Vec_t >()		.template Div< Value_t >()
			.template Mod< Vec_t >()		.template Mod< Value_t >()

			.BinaryRH( EBinaryOperator::Add,	&VecFunc::template Add< Vec_t > )
			.BinaryRH( EBinaryOperator::Sub,	&VecFunc::template Sub< Vec_t > )
			.BinaryRH( EBinaryOperator::Mul,	&VecFunc::template Mul< Vec_t > )
			.BinaryRH( EBinaryOperator::Div,	&VecFunc::template Div< Vec_t > )
			.BinaryRH( EBinaryOperator::Mod,	&VecFunc::template Mod< Vec_t > )

			.Equals( &VecFunc::template Equal< Vec_t > )
			.Compare( &VecFunc::template Cmp< Vec_t > );

		binder.AddMethod( &Vec_t::Length,		"Length" );
		binder.AddMethod( &Vec_t::Normalized,	"Normalized" );
		binder.AddMethod( &Vec_t::Distance,		"Distance" );

		binder.AddMethod( &Vec_t::Reflect,		"Reflect" );
		binder.AddMethod( &Vec_t::Refract,		"Refract" );
		binder.AddMethod( &Vec_t::Sum,			"Sum" );
		binder.AddMethod( &Vec_t::Area,			"Area" );
		binder.AddMethod( &Vec_t::Volume,		"Volume" );
		
		binder.AddMethod( &Vec_t::Equal,		"Equal" );
		binder.AddMethod( &Vec_t::Less,			"Less" );
		binder.AddMethod( &Vec_t::LEqual,		"LEqual" );
		binder.AddMethod( &Vec_t::Greater,		"Greater" );
		binder.AddMethod( &Vec_t::GEqual,		"GEqual" );
		binder.AddMethod( &Vec_t::IsZero,		"IsZero" );

		se->AddFunction( &VecFunc::template Dot<Vec_t>,		"Dot" );
		se->AddFunction( &VecFunc::template Sin<Vec_t>,		"Sin" );
		se->AddFunction( &VecFunc::template Cos<Vec_t>,		"Cos" );
		se->AddFunction( &VecFunc::template Tan<Vec_t>,		"Tan" );
		se->AddFunction( &VecFunc::template Square<Vec_t>,	"Square" );
		se->AddFunction( &VecFunc::template Sqrt<Vec_t>,	"Sqrt" );
		se->AddFunction( &VecFunc::template Lerp<Vec_t>,	"Lerp" );

		if_constexpr( Vec_t::STATIC_COUNT == 3 ) {
			se->AddFunction( &VecFunc::template Cross<Vec_t>,	"Cross" );
		}
	}

/*
=================================================
	DefineVector_Func
=================================================
*/
	struct DefineVector_Func
	{
		ScriptEngine *	_se;

		explicit DefineVector_Func (ScriptEngine *se) : _se{se}
		{}

		template <typename T, usize Index>
		void Process ()
		{
			ClassBinder<T>	binder( _se );
			
			binder.CreateClassValue();
		}
	};
	
/*
=================================================
	BindVector_Func
=================================================
*/
	struct BindVector_Func
	{
		ScriptEngine *	_se;

		explicit BindVector_Func (ScriptEngine *se) : _se{se}
		{}

		template <typename T, usize Index>
		void Process ()
		{
			using Value_t = typename T::Value_t;

			ClassBinder<T>	binder( _se );

			InitVecFields<T>::Init( binder );

			if_constexpr( CompileTime::IsSameTypes< Value_t, bool > )
			{
				BindBoolVec( binder, _se );
			}
			else
			if_constexpr( CompileTime::IsInteger< Value_t > )
			{
				BindIntVec( binder, _se );
			}
			else
			if_constexpr( CompileTime::IsFloat< Value_t > )
			{
				BindFloatVec( binder, _se );
			}
		}
	};

/*
=================================================
	BindVectorMath
=================================================
*/
	void DefaultBindings::BindVectorMath (ScriptEngine *se)
	{
		using VecTypes = CompileTime::TypeListFrom<
							bool2, bool3, bool4,
							int2, int3, int4,
							uint2, uint3, uint4,
							ilong2, ilong3, ilong4,
							ulong2, ulong3, ulong4,
							float2, float3, float4,
							double2, double3, double4
						>;

		// declare
		{
			DefineVector_Func	func{ se };
		
			VecTypes::RuntimeForEach( func );
		}

		// bind
		{
			BindVector_Func	func{ se };
		
			VecTypes::RuntimeForEach( func );
		}
	}
	

}	// GXScript
