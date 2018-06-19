// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Type qualifiers used to optimize copy strategies.
*/

#pragma once

#include "Core/STL/CompileTime/PlainOldDataType.h"

namespace GX_STL
{
namespace CompileTime
{

	//
	// Type Qualifier
	//
	
	struct ETypeQualifier
	{
		enum type : uint
		{
			// flags
			Unknown				= 0,
			MemCopyAvailable	= 1 << 0,	// MemCopy, MemMove
			ZeroMemAvailable	= 1 << 2,	// ZeroMem instead of ctor or dtor
			MemCopyFromFile		= 1 << 3,	// can be initialized by copy memory from binary buffer or file
			WithCtor			= 1 << 4,	// has custom ctor
			WithDtor			= 1 << 5,	// has custom dtor
			WithCompareOp		= 1 << 6,	// has custom compare operators
			Noncopyable			= 1 << 7,

			// default combinations
			Def_SimplePOD		= MemCopyAvailable | MemCopyFromFile | ZeroMemAvailable,
			Def_ComplexPOD		= MemCopyAvailable | MemCopyFromFile | WithCtor,
			Def_FastCopyable	= MemCopyAvailable | WithCtor | WithDtor | WithCompareOp,
			Def_Complex			= WithCtor | WithDtor | WithCompareOp,
			Def_Noncopyable		= Noncopyable,
		};

		//GX_ENUM_BIT_OPERATIONS( type );
	};


	namespace _ctime_hidden_
	{
		//
		// Type Qualifier
		//
		struct _TypeQualifierBase : NonPODType {};

		template <ETypeQualifier::type Q>
		struct _TypeQualifier : SwitchType< Q == ETypeQualifier::Def_SimplePOD, PODType, _TypeQualifierBase >	// IsPOD<> can be used too
		{
			static constexpr ETypeQualifier::type	__typeQualifierValue__ = Q;
		};

		
		//
		// Get Type Qualifier
		//
		template <typename T, bool HasQualifier>
		struct _GetTypeQualifier2 {
			static constexpr ETypeQualifier::type	value = IsPOD<T> ? ETypeQualifier::Def_SimplePOD : ETypeQualifier::Def_Complex;
		};
		
		template <typename T>
		struct _GetTypeQualifier2< T, true > {
			static constexpr ETypeQualifier::type	value = T::__typeQualifierValue__;
		};

		template <typename T>
		struct _GetTypeQualifier {
			static constexpr ETypeQualifier::type	value = _GetTypeQualifier2< T, IsBaseOf< _TypeQualifierBase, T > >::value;
		};

		
		//
		// Switch Stronger Qualifier
		//
		template <ETypeQualifier::type Q1, ETypeQualifier::type Q2>
		struct _SwitchStrongerQualifier
		{
		private:
			typedef ETypeQualifier	Q;
			
			static const uint	share	= Q1 == Q::Unknown ? Q2 : (Q2 == Q::Unknown ? Q1 : (uint(Q1) & uint(Q2)));
			static const uint	all		= uint(Q1) | uint(Q2);
			static const uint	a		= all & Q::WithCtor ? share | Q::WithCtor : share;
			static const uint	b		= all & Q::WithDtor ? a | Q::WithDtor : a;
			static const uint	c		= all & Q::Noncopyable ? b | Q::Noncopyable : b;
			static const uint	d		= all & Q::WithCompareOp ? c | Q::WithCompareOp : c;

		public:
			static constexpr ETypeQualifier::type	value = ETypeQualifier::type( d );
		};

	}	// _ctime_hidden_

	
	template <ETypeQualifier::type Q>
	using TypeQualifier = _ctime_hidden_::_TypeQualifier< Q >;

	using PODStruct		= _ctime_hidden_::_TypeQualifier< ETypeQualifier::Def_ComplexPOD >;
	using FastCopyable	= _ctime_hidden_::_TypeQualifier< ETypeQualifier::Def_FastCopyable >;
	using ComplexType	= _ctime_hidden_::_TypeQualifier< ETypeQualifier::Def_Complex >;


	template <typename T>
	static constexpr bool IsMemCopyAvailable = (_ctime_hidden_::_GetTypeQualifier<T>::value & ETypeQualifier::MemCopyAvailable) != 0;
	
	template <typename T>
	static constexpr bool IsZeroMemAvailable = (_ctime_hidden_::_GetTypeQualifier<T>::value & ETypeQualifier::ZeroMemAvailable) != 0;
	
	template <typename T>
	static constexpr bool IsMemCopyFromFileAvailable = (_ctime_hidden_::_GetTypeQualifier<T>::value & ETypeQualifier::MemCopyFromFile) != 0;
	
	template <typename T>
	static constexpr bool IsNoncopyable = (_ctime_hidden_::_GetTypeQualifier<T>::value & ETypeQualifier::Noncopyable) != 0;
	
	template <typename T>
	static constexpr bool IsCtorAvailable = (_ctime_hidden_::_GetTypeQualifier<T>::value & ETypeQualifier::WithCtor) != 0;
	
	template <typename T>
	static constexpr bool IsDtorAvailable = (_ctime_hidden_::_GetTypeQualifier<T>::value & ETypeQualifier::WithDtor) != 0;
	
	template <typename T>
	static constexpr bool IsCompareOpAvailable = (_ctime_hidden_::_GetTypeQualifier<T>::value & ETypeQualifier::WithCompareOp) != 0;
	

	template <ETypeQualifier::type Q, typename ...BaseTypes>
	struct InheritWithTypeQualifier : public BaseTypes...
	{
		static constexpr ETypeQualifier::type	__typeQualifierValue__ = Q;	// TODO: use qual of first parent?
	};


}	// CompileTime
}	// GX_STL
