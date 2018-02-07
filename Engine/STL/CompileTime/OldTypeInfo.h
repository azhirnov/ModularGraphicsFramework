// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/TypeId.h"
#include "TypeTraits.h"
#include "TemplateUtils.h"
#include "TemplateMath.h"
#include "PlainOldDataType.h"
#include "TypeQualifier.h"

namespace GX_STL
{
namespace CompileTime
{

	namespace _ctime_hidden_
	{
		// Limits
# ifdef COMPILER_MSVC
#	pragma warning (push)
#	pragma warning (disable: 4723)	// potential divide by 0
# endif
		
#	undef UINT8_MAX
#	undef UINT16_MAX
#	undef UINT32_MAX
#	undef UINT64_MAX
#	undef INT8_MIN
#	undef INT16_MIN
#	undef INT32_MIN
#	undef INT64_MIN
#	undef INT8_MAX
#	undef INT16_MAX
#	undef INT32_MAX
#	undef INT64_MAX

#	ifdef GX_MATH_REAL_LIMITS

		constexpr	ubyte	UINT8_MAX			= ubyte ( -1 );
		constexpr	ushort	UINT16_MAX			= ushort( -1 );
		constexpr	uint	UINT32_MAX			= uint( -1 );
		constexpr	ulong	UINT64_MAX			= ulong( -1 );

		constexpr	byte	INT8_MIN			= byte ( 1 << (CompileTime::SizeOf<byte>::bits-1) );
		constexpr	short	INT16_MIN			= short( 1 << (CompileTime::SizeOf<short>::bits-1) );
		constexpr	int		INT32_MIN			= int(1) << (CompileTime::SizeOf<int>::bits-1);
		constexpr	ilong	INT64_MIN			= ilong(1) << (CompileTime::SizeOf<ilong>::bits-1);

		constexpr	byte	INT8_MAX			= UINT8_MAX  & ~INT8_MIN;
		constexpr	short	INT16_MAX			= UINT16_MAX & ~INT16_MIN;
		constexpr	int		INT32_MAX			= UINT32_MAX & ~INT32_MIN;
		constexpr	ilong	INT64_MAX			= UINT64_MAX & ~INT64_MIN;

		constexpr	float	FLOAT_MAX			= static_cast< float const &>( 0x7F7FFFFF );	// TODO: fix
		constexpr	float	FLOAT_MIN			= static_cast< float const &>( 0x00800000 );	// TODO: fix

		constexpr	double	DOUBLE_MAX			= static_cast< double const &>( 0x7FEFFFFFFFFFFFFFull );	// TODO: fix
		constexpr	double	DOUBLE_MIN			= static_cast< double const &>( 0x0010000000000000ull );	// TODO: fix

		constexpr	uint	FLOAT_MAX_POW		= 38;
		constexpr	uint	DOUBLE_MAX_POW		= 308;

		const		float	FLOAT_INF			= 1.0f / static_cast< float const  &>( uint(0) );
		const		double	DOUBLE_INF			= 1.0  / static_cast< double const &>( ulong(0) );

		const		float	FLOAT_NAN			= static_cast< float const  &>( uint(0) ) / static_cast< float const  &>( uint(0) );
		const		double	DOUBLE_NAN			= static_cast< double const &>( ulong(0) ) / static_cast< double const &>( ulong(0) );

#	else

		constexpr	ubyte	UINT8_MAX			= ubyte(0xFF);
		constexpr	ushort	UINT16_MAX			= ushort(0xFFFF);
		constexpr	uint	UINT32_MAX			= uint(0xFFFFFFFFu);
		constexpr	ulong	UINT64_MAX			= ulong(0xFFFFFFFFFFFFFFFFull);

		constexpr	byte	INT8_MIN			= byte (0x80);
		constexpr	short	INT16_MIN			= short(0x8000);
		constexpr	int		INT32_MIN			= int(0x80000000);
		constexpr	ilong	INT64_MIN			= ilong(0x8000000000000000ll);

		constexpr	byte	INT8_MAX			= UINT8_MAX  & ~INT8_MIN;
		constexpr	short	INT16_MAX			= UINT16_MAX & ~INT16_MIN;
		constexpr	int		INT32_MAX			= UINT32_MAX & ~INT32_MIN;
		constexpr	ilong	INT64_MAX			= UINT64_MAX & ~INT64_MIN;

		constexpr	float	FLOAT_MAX			= 3.402823466e+38F;
		constexpr	float	FLOAT_MIN			= 1.175494351e-38F;

		constexpr	double	DOUBLE_MAX			= 1.7976931348623158e+308;
		constexpr	double	DOUBLE_MIN			= 2.2250738585072014e-308;
	
		constexpr	uint	FLOAT_MAX_POW		= 38;
		constexpr	uint	DOUBLE_MAX_POW		= 308;
		
		const		float	FLOAT_INF			= static_cast< float const  &>(uint( 0x7F800000 ));
		const		double	DOUBLE_INF			= static_cast< double const &>(ulong( 0x7FF0000000000000ull ));

		const		float	FLOAT_NAN			= static_cast< float const  &>(uint( 0xFFC00000 ));
		const		double	DOUBLE_NAN			= static_cast< double const &>(ulong( 0xFFF8000000000000ull ));

#	endif	// GX_MATH_REAL_LIMITS

		constexpr	float	FLOAT_EPSILON		= 1.0e-6f;
		constexpr	double	DOUBLE_EPSILON		= 1.0e-8;

# ifdef COMPILER_MSVC
#	pragma warning (pop)
# endif

		enum ETypeDescriptor
		{
			UNKNOWN		= 0,
			VOID		= 1 << 1,
			FLOAT		= 1 << 2,
			INTEGER		= 1 << 3,
			FIXED		= 1 << 4,
			//BOOL		= 1 << 5,

			SIGNED		= 1 << 6,
			UNSIGNED	= 1 << 7,

			_MASK		= (1 << 8) - 1,

			COMPOUND	= 1 << 8,	// has inner type
			VECTOR		= 1 << 9,	// vector or matrix type
			SOFTWARE	= 1 << 10,	// software type (it is slower than hardware types)
			WRAPPER		= 1 << 11,	// fast wrapper for ariphmetic types
		};

	}	// _ctime_hidden_


	
	template <typename T>	struct TypeInfo;

	GX_CREATE_MEMBER_DETECTOR( Count );



	//
	// Type Descriptor
	//

	namespace TypeDescriptor
	{

		// Get Inner Type
		template <typename T>
		struct InnerType
		{
			template <typename U, bool I>
			struct _GetInner;

			template <typename U>
			struct _GetInner<U,false>
			{
				typedef typename TypeInfo< U >::inner_type	type;
			};

			template <typename U>
			struct _GetInner<U,true>
			{
				typedef typename _GetInner< typename TypeInfo< U >::inner_type,
											(TypeInfo< U >::FLAGS & _ctime_hidden_::COMPOUND) != 0 >::type	type;
			};

			typedef typename _GetInner< T, true >::type		type;
		};


		// get type info
		template <typename T>
		using GetTypeInfo = TypeInfo< typename TypeTraits::RemoveConstVolatile<T> >;
		

		// Is<Type>
		template <typename T>
		struct IsFloat {
			static const bool	value = ( int(TypeInfo< T >::FLAGS) & int(_ctime_hidden_::FLOAT) ) != 0;
		};

		template <typename T>
		struct IsInteger {
			static const bool	value = ( int(TypeInfo< T >::FLAGS) & int(_ctime_hidden_::INTEGER) ) != 0;
		};

		template <typename T>
		struct IsFixed {
			static const bool	value = ( int(TypeInfo< T >::FLAGS) & int(_ctime_hidden_::FIXED) ) != 0;
		};

		template <typename T>
		struct IsSigned {
			static const bool	value = ( int(TypeInfo< T >::FLAGS) & int(_ctime_hidden_::SIGNED) ) != 0;
		};

		template <typename T>
		struct IsUnsigned {
			static const bool	value = ( int(TypeInfo< T >::FLAGS) & int(_ctime_hidden_::UNSIGNED) ) != 0;
		};

		template <typename T>
		struct IsSoftware {
			static const bool	value = ( int(TypeInfo< T >::FLAGS) & int(_ctime_hidden_::SOFTWARE) ) != 0;
		};

		template <typename T>
		struct IsVoid {
			static const bool	value = false;
		};

		template <>
		struct IsVoid<void> {
			static const bool	value = true;
		};

		template <typename T>
		struct IsWrapper {
			static const bool	value = (int(TypeInfo<T>::FLAGS) & int(_ctime_hidden_::WRAPPER)) != 0;
		};

		template <typename T>
		struct IsArithmetic {
			static const bool	_is_simple	= ( int(TypeInfo< T >::FLAGS) & int(_ctime_hidden_::COMPOUND | _ctime_hidden_::VECTOR) ) == 0;
			static const bool	_is_arith	= ( int(TypeInfo< T >::FLAGS) & int(_ctime_hidden_::FLOAT | _ctime_hidden_::INTEGER) ) != 0;
			static const bool	value		= _is_simple or _is_arith or IsWrapper<T>::value;
		};

		
		template <typename T, bool Exist>
		struct _GetCapacityImpl {
			static constexpr uint	Count ()	{ return TypeInfo<T>::Count(); }
		};

		template <typename T>
		struct _GetCapacityImpl< T, false > {
			static constexpr uint	Count ()	{ return 0; }
		};
		
		template <typename T>
		static constexpr uint	GetCapacity = _GetCapacityImpl< T, Detect_Count< TypeInfo<T> >::value >::Count();


		template <typename T>
		struct IsVector {
			static const bool	value = ( int(TypeInfo< T >::FLAGS) & int(_ctime_hidden_::VECTOR) ) != 0 or
										GetCapacity<T> > 1;
		};

		template <typename T>
		struct IsScalar {
			static const bool	value = ( int(TypeInfo< T >::FLAGS) & int(_ctime_hidden_::VECTOR) ) == 0 and
										GetCapacity<T> == 1 and
										not IsVoid<T>::value;
		};


		// Strong comparison
		template <typename T1, typename T2>
		struct IsSameTypes {
			static const bool	value = false;
		};

		template <typename T>
		struct IsSameTypes <T,T> {
			static const bool	value = true;
		};


		// Weak comparison
		template <typename T1, typename T2>
		struct IsSameTypesWithoutQualifiers {
			static const bool	value = IsSameTypes< 
											typename TypeTraits::RemoveConstVolatile<T1>, 
											typename TypeTraits::RemoveConstVolatile<T2> >::value;
		};
	

		template <template <typename ...> class Left, template <typename ...> class Right>
		struct IsSameTemplates			{ static const bool  value = false; };

		template <template <typename ...> class T>
		struct IsSameTemplates< T, T >	{ static const bool  value = true; };


		template <typename ToType, typename FromType>
		static inline bool CanCastTo (FromType *p)
		{
			if ( p == null )
				return true;

			DESCTOP_ONLY( return dynamic_cast< ToType *>( p ) != null );
			MOBILE_ONLY( return true );
		}

	}	// TypeDescriptor

	

	//
	// Type Info (example)
	//
	
	template <typename T>
	struct TypeInfo
	{
		typedef T		type;
		typedef T		inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum { FLAGS = _ctime_hidden_::UNKNOWN };	// type descriptor flags
		
		//static constexpr type	Max()		{ return type(); }
		//static constexpr type	Min()		{ return type(); }
		//static constexpr type	Inf()		{ return type(); }
		//static constexpr type	NaN()		{ return type(); }

		//static constexpr type	Epsilon()	{ return type(); }
		//static constexpr uint	SignBit()	{ return 0; }
		//static constexpr uint	Count()		{ return 1; }
	};
	

	template <typename T>	struct TypeInfo< const T > : TypeInfo<T> {};
	template <typename T>	struct TypeInfo< volatile T > : TypeInfo<T> {};


	template <>
	struct TypeInfo<void>
	{
		typedef void		type;
		typedef type		inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum {
			FLAGS	= _ctime_hidden_::VOID,
		};

		static void				Max()		{ return void(); }
		static void				Min()		{ return void(); }
		static void				Inf()		{ return void(); }
		static void				NaN()		{ return void(); }
		
		static void				Epsilon()	{ return void(); }
		static constexpr uint	SignBit()	{ return 0; }
		static constexpr uint	Count()		{ return 0; }
	};


	template <>
	struct TypeInfo<bool>
	{
		typedef bool		type;
		typedef type		inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum {
			FLAGS	= (int)_ctime_hidden_::INTEGER | (int)_ctime_hidden_::UNSIGNED,
		};

		static constexpr type	Max()		{ return true; }
		static constexpr type	Min()		{ return false; }
		
		static constexpr type	Epsilon()	{ return type(0); }
		static constexpr uint	SignBit()	{ return ~0u; }
		static constexpr uint	Count()		{ return 1; }
	};


	template <>
	struct TypeInfo<byte>
	{
		typedef byte		type;
		typedef type		inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum {
			FLAGS	= (int)_ctime_hidden_::INTEGER | (int)_ctime_hidden_::SIGNED,
		};

		static constexpr type	Max()		{ return _ctime_hidden_::INT8_MAX; }
		static constexpr type	Min()		{ return _ctime_hidden_::INT8_MIN; }
		
		static constexpr type	Epsilon()	{ return type(0); }
		static constexpr uint	SignBit()	{ return SizeOf<type>::bits-1; }
		static constexpr uint	Count()		{ return 1; }
	};


	template <>
	struct TypeInfo<ubyte>
	{
		typedef ubyte		type;
		typedef type		inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum {
			FLAGS	= (int)_ctime_hidden_::INTEGER | (int)_ctime_hidden_::UNSIGNED,
		};

		static constexpr type	Max()		{ return _ctime_hidden_::UINT8_MAX; }
		static constexpr type	Min()		{ return type(0); }
		
		static constexpr type	Epsilon()	{ return type(0); }
		static constexpr uint	SignBit()	{ return ~0u; }
		static constexpr uint	Count()		{ return 1; }
	};


	template <>
	struct TypeInfo<char>
	{
		typedef char		type;
		typedef type		inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum {
			FLAGS	= (int)_ctime_hidden_::INTEGER | (int)_ctime_hidden_::SIGNED,
		};

		static constexpr type	Max()		{ return _ctime_hidden_::INT8_MAX; }
		static constexpr type	Min()		{ return _ctime_hidden_::INT8_MIN; }
		
		static constexpr type	Epsilon()	{ return type(0); }
		static constexpr uint	SignBit()	{ return SizeOf<type>::bits-1; }
		static constexpr uint	Count()		{ return 1; }
	};


	template <>
	struct TypeInfo<short>
	{
		typedef short		type;
		typedef type		inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum {
			FLAGS	= (int)_ctime_hidden_::INTEGER | (int)_ctime_hidden_::SIGNED,
		};

		static constexpr type	Max()		{ return _ctime_hidden_::INT16_MAX; }
		static constexpr type	Min()		{ return _ctime_hidden_::INT16_MIN; }
		
		static constexpr type	Epsilon()	{ return type(0); }
		static constexpr uint	SignBit()	{ return CompileTime::SizeOf<type>::bits-1; }
		static constexpr uint	Count()		{ return 1; }
	};


	template <>
	struct TypeInfo<ushort>
	{
		typedef ushort		type;
		typedef type		inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum {
			FLAGS	= (int)_ctime_hidden_::INTEGER | (int)_ctime_hidden_::UNSIGNED,
		};

		static constexpr type	Max()		{ return _ctime_hidden_::UINT16_MAX; }
		static constexpr type	Min()		{ return type(0); }
		
		static constexpr type	Epsilon()	{ return type(0); }
		static constexpr uint	SignBit()	{ return ~0u; }
		static constexpr uint	Count()		{ return 1; }
	};


	template <> struct TypeInfo<wchar> : TypeInfo<short> {};


	template <>
	struct TypeInfo<int>
	{
		typedef int			type;
		typedef type		inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum {
			FLAGS	= (int)_ctime_hidden_::INTEGER | (int)_ctime_hidden_::SIGNED,
		};

		static constexpr type	Max()		{ return _ctime_hidden_::INT32_MAX; }
		static constexpr type	Min()		{ return _ctime_hidden_::INT32_MIN; }
		
		static constexpr type	Epsilon()	{ return type(0); }
		static constexpr uint	SignBit()	{ return SizeOf<type>::bits-1; }
		static constexpr uint	Count()		{ return 1; }
	};


	template <>
	struct TypeInfo<uint>
	{
		typedef uint		type;
		typedef type		inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum {
			FLAGS	= (int)_ctime_hidden_::INTEGER | (int)_ctime_hidden_::UNSIGNED,
		};

		static constexpr type	Max()		{ return _ctime_hidden_::UINT32_MAX; }
		static constexpr type	Min()		{ return type(0); }
		
		static constexpr type	Epsilon()	{ return type(0); }
		static constexpr uint	SignBit()	{ return ~0u; }
		static constexpr uint	Count()		{ return 1; }
	};


	template <>
	struct TypeInfo<ilong>
	{
		typedef ilong		type;
		typedef type		inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum {
			FLAGS	= (int)_ctime_hidden_::INTEGER | (int)_ctime_hidden_::SIGNED,
		};

		static constexpr type	Max()		{ return _ctime_hidden_::INT64_MAX; }
		static constexpr type	Min()		{ return _ctime_hidden_::INT64_MIN; }
		
		static constexpr type	Epsilon()	{ return type(0); }
		static constexpr uint	SignBit()	{ return SizeOf<type>::bits-1; }
		static constexpr uint	Count()		{ return 1; }
	};


	template <>
	struct TypeInfo<ulong>
	{
		typedef ulong		type;
		typedef type		inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum {
			FLAGS	= (int)_ctime_hidden_::INTEGER | (int)_ctime_hidden_::UNSIGNED,
		};

		static constexpr type	Max()		{ return _ctime_hidden_::UINT64_MAX; }
		static constexpr type	Min()		{ return 0; }
		
		static constexpr type	Epsilon()	{ return type(0); }
		static constexpr uint	SignBit()	{ return ~0u; }
		static constexpr uint	Count()		{ return 1; }
	};


	template <>
	struct TypeInfo<float>
	{
		typedef float		type;
		typedef type		inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum {
			FLAGS	= (int)_ctime_hidden_::FLOAT | (int)_ctime_hidden_::SIGNED,
		};

		static constexpr type	Max()		{ return _ctime_hidden_::FLOAT_MAX; }
		static constexpr type	Min()		{ return _ctime_hidden_::FLOAT_MIN; }
		static const     type	Inf()		{ return _ctime_hidden_::FLOAT_INF; }
		static const     type	NaN()		{ return _ctime_hidden_::FLOAT_NAN; }
		
		static constexpr type	Epsilon()	{ return _ctime_hidden_::FLOAT_EPSILON; }
		static constexpr uint	SignBit()	{ return CompileTime::SizeOf<type>::bits-1; }
		static constexpr uint	Count()		{ return 1; }
	};


	template <>
	struct TypeInfo<double>
	{
		typedef double		type;
		typedef type		inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum {
			FLAGS	= (int)_ctime_hidden_::FLOAT | (int)_ctime_hidden_::SIGNED,
		};

		static constexpr type	Max()		{ return _ctime_hidden_::DOUBLE_MAX; }
		static constexpr type	Min()		{ return _ctime_hidden_::DOUBLE_MIN; }
		static const     type	Inf()		{ return _ctime_hidden_::DOUBLE_INF; }
		static const     type	NaN()		{ return _ctime_hidden_::DOUBLE_NAN; }
		
		static constexpr type	Epsilon()	{ return _ctime_hidden_::DOUBLE_EPSILON; }
		static constexpr uint	SignBit()	{ return CompileTime::SizeOf<type>::bits-1; }
		static constexpr uint	Count()		{ return 1; }
	};


	template <>
	struct TypeInfo<long double>
	{
		typedef long double		type;
		typedef type			inner_type;
		
		// type constructor //
		template <typename OtherType>
		using CreateWith = OtherType;

		// type descriptor //
		enum {
			FLAGS	= (int)_ctime_hidden_::FLOAT | (int)_ctime_hidden_::SIGNED,
		};

		static constexpr type	Max()		{ return _ctime_hidden_::DOUBLE_MAX; }
		static constexpr type	Min()		{ return _ctime_hidden_::DOUBLE_MIN; }
		static const     type	Inf()		{ return _ctime_hidden_::DOUBLE_INF; }
		static const     type	NaN()		{ return _ctime_hidden_::DOUBLE_NAN; }
		
		static constexpr type	Epsilon()	{ return _ctime_hidden_::DOUBLE_EPSILON; }
		static constexpr uint	SignBit()	{ return CompileTime::SizeOf<type>::bits-1; }
		static constexpr uint	Count()		{ return 1; }
	};

}	// CompileTime
}	// GX_STL
