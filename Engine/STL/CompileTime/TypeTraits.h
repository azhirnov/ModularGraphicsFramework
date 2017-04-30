// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Types.h"
#include "TemplateUtils.h"

namespace GX_STL
{
namespace TypeTraits
{
	namespace _ttraits_hidden_
	{
		// _TypeQualifierInfo
		template <typename T>
		struct _BaseTypeQualifierInfo
		{
			typedef T			type;

			static const bool	is_final		= false;
			static const bool	is_pointer		= false;	// *
			static const bool	is_const		= false;	// const
			static const bool	is_volatile		= false;	// volatile
			static const bool	is_reference	= false;	// &
			static const bool	is_rvalue_ref	= false;	// &&
			static const bool	is_array		= false;	// [...]
		};

		template <typename T>
		struct _TypeQualifierInfo : _BaseTypeQualifierInfo<T>
		{
			typedef T			input_type;
			static const bool	is_final		= true;
		};

		template <typename T>
		struct _TypeQualifierInfo <const T> : _BaseTypeQualifierInfo<T>
		{
			typedef const T		input_type;
			static const bool	is_const		= true;
		};

		template <typename T>
		struct _TypeQualifierInfo <volatile T> : _BaseTypeQualifierInfo<T>
		{
			typedef volatile T	input_type;
			static const bool	is_volatile		= true;
		};

		template <typename T>
		struct _TypeQualifierInfo <volatile const T> : _BaseTypeQualifierInfo<T>
		{
			typedef volatile const T	input_type;
			static const bool	is_const		= true;
			static const bool	is_volatile		= true;
		};

		template <typename T>
		struct _TypeQualifierInfo <T &> : _BaseTypeQualifierInfo<T>
		{
			typedef T &			input_type;
			static const bool	is_reference	= true;
		};

		template <typename T>
		struct _TypeQualifierInfo <T *> : _BaseTypeQualifierInfo<T>
		{
			typedef T *			input_type;
			static const bool	is_pointer		= true;
		};

		template <typename T>
		struct _TypeQualifierInfo <T []> : _BaseTypeQualifierInfo<T>
		{
			typedef T			input_type[];
			static const bool	is_pointer		= true;
			//static const bool	is_array		= true;
		};

		template <typename T, usize I>
		struct _TypeQualifierInfo <T [I]> : _BaseTypeQualifierInfo<T>
		{
			typedef T			input_type[I];
			static const bool	is_array		= true;
		};

		template <typename T>
		struct _TypeQualifierInfo <T &&> : _BaseTypeQualifierInfo<T>
		{
			typedef T &&		input_type;
			static const bool	is_rvalue_ref	= true;
		};

	}	// _ttraits_hidden_


		
	//--------- Remove Qualifier --------//

	// RemoveQualifier
	template <typename T>
	using RemoveQualifier = typename _ttraits_hidden_::_TypeQualifierInfo<T>::type;

		

	//--------- Get Base Type --------//

	// IsBaseType
	template <typename T>
	constexpr bool IsBaseType	= _ttraits_hidden_::_TypeQualifierInfo<T>::is_final;

	namespace _ttraits_hidden_
	{
		template <typename T, bool Stop = IsBaseType<T>>
		struct _GetBaseType {
			typedef typename _GetBaseType< RemoveQualifier<T> >::type	type;
		};

		template <typename T>
		struct _GetBaseType < T, true > {
			typedef T	type;
		};
	}	// _ttraits_hidden_

	// GetBaseType
	template <typename T>
	using GetBaseType = typename _ttraits_hidden_::_GetBaseType<T>::type;


		
	//--------- Apply Type Qualifiers --------//

	namespace _ttraits_hidden_
	{
		template <typename T, bool IsPointer, bool IsConst, bool IsVolatile, bool IsReference, bool IsRValueRef>
		struct _ApplyTypeQualifiers
		{
			STATIC_ASSERT( (IsPointer + IsReference + IsRValueRef) <= 1,
							"type must be Pointer(*) or Reference(&) or RightValueReference(&&)" );

			typedef T																					raw_type;

			typedef typename CompileTime::SwitchType< IsPointer, raw_type *, raw_type >					_pointer_t;
			typedef typename CompileTime::SwitchType< IsReference, _pointer_t &, _pointer_t >			_reference_t;
			typedef typename CompileTime::SwitchType< IsRValueRef, _reference_t &&, _reference_t >		_rvalue_ref_t;
			typedef typename CompileTime::SwitchType< IsConst, const _rvalue_ref_t, _rvalue_ref_t >		_const_t;
			typedef typename CompileTime::SwitchType< IsVolatile, volatile _const_t, _const_t >			_volatile_t;

			typedef _volatile_t																			type;
		};
		
		template <bool IsPointer, bool IsConst, bool IsVolatile, bool IsReference, bool IsRValueRef>
		struct _ApplyTypeQualifiers <void, IsPointer, IsConst, IsVolatile, IsReference, IsRValueRef>
		{
			STATIC_ASSERT( (IsPointer + IsReference + IsRValueRef) <= 1,
							"type must be Pointer(*) or Reference(&) or RightValueReference(&&)" );

			STATIC_ASSERT( not IsReference and not IsRValueRef, "void type can't be the reference type!" );
			
			typedef void																				raw_type;

			typedef typename CompileTime::SwitchType< IsPointer, raw_type *, raw_type >					_pointer_t;
			typedef _pointer_t																			_reference_t;
			typedef _reference_t																		_rvalue_ref_t;
			typedef typename CompileTime::SwitchType< IsConst, const _rvalue_ref_t, _rvalue_ref_t >		_const_t;
			typedef typename CompileTime::SwitchType< IsVolatile, volatile _const_t, _const_t >			_volatile_t;

			typedef _volatile_t																			type;
		};
	}	// _ttraits_hidden_


		
	//--------- Const & Volatile Utils --------//

	namespace _ttraits_hidden_
	{
		template <typename T>
		struct _IsConstOrVolatile {
			typedef _TypeQualifierInfo<T>		_qinfo;
			static const bool	value = (_qinfo::is_const | _qinfo::is_volatile);
		};
	}	// _ttraits_hidden_
		
	// IsConstOrVolatile
	template <typename T>
	constexpr bool IsConstOrVolatile = _ttraits_hidden_::_IsConstOrVolatile<T>::value;
		

	namespace _ttraits_hidden_
	{
		template <typename T, bool Stop = not IsConstOrVolatile<T> >
		struct _RemoveConstVolatile {
			typedef typename _RemoveConstVolatile< RemoveQualifier<T> >::type	_base_type;
			typedef _TypeQualifierInfo<T>										_qinfo;
			typedef typename _ApplyTypeQualifiers<
								_base_type,
								_qinfo::is_pointer,
								false,
								false,
								_qinfo::is_reference,
								_qinfo::is_rvalue_ref >::type		type;
		};

		template <typename T>
		struct _RemoveConstVolatile < T, true > {
			typedef T	type;
		};
	}	// _ttraits_hidden_

	// RemoveConstVolatile
	template <typename T>
	using RemoveConstVolatile = typename _ttraits_hidden_::_RemoveConstVolatile<T>::type;



	//--------- Pointer Utils --------//

	// IsPointer
	template <typename T>
	constexpr bool IsPointer = _ttraits_hidden_::_TypeQualifierInfo< RemoveConstVolatile<T> >::is_pointer;

	namespace _ttraits_hidden_
	{
		template <typename T>
		struct _RemovePointer {
			typedef _TypeQualifierInfo<T>				_qinfo;

			typedef typename CompileTime::SwitchType< not IsPointer<T>,
						T,
						typename CompileTime::SwitchType< _qinfo::is_pointer,
								typename _qinfo::type,
								RemoveQualifier< RemoveConstVolatile<T> > >
						>														type;
		};
	}	// _ttraits_hidden_

	// RemovePointer
	template <typename T>
	using RemovePointer = typename _ttraits_hidden_::_RemovePointer< T >::type;


		
	//--------- Reference Utils --------//
	
	// IsLValueReference
	template <typename T>
	constexpr bool IsLValueReference	= _ttraits_hidden_::_TypeQualifierInfo< RemoveConstVolatile<T> >::is_reference;

	// IsRValueReference
	template <typename T>
	constexpr bool IsRValueReference	= _ttraits_hidden_::_TypeQualifierInfo< RemoveConstVolatile<T> >::is_rvalue_ref;

	// IsAnyReference
	template <typename T>
	constexpr bool IsAnyReference		= IsLValueReference<T> or IsRValueReference<T>;


	namespace _ttraits_hidden_
	{
		template <typename T>
		struct _RemoveReference {
			typedef _TypeQualifierInfo<T>				_qinfo;

			typedef typename CompileTime::SwitchType< not IsLValueReference<T>,
						T,
						typename CompileTime::SwitchType< _qinfo::is_reference,
								typename _qinfo::type,
								RemoveQualifier< RemoveConstVolatile<T> > >
						>														type;
		};
		
		template <typename T>
		struct _RemoveRValueReference {
			typedef _TypeQualifierInfo<T>				_qinfo;

			typedef typename CompileTime::SwitchType< not IsRValueReference<T>,
						T,
						typename CompileTime::SwitchType< _qinfo::is_rvalue_ref,
								typename _qinfo::type,
								RemoveQualifier< RemoveConstVolatile<T> > >
						>														type;
		};

		template <typename T>
		struct _RemoveAnyReference {
			typedef _TypeQualifierInfo<T>				_qinfo;

			typedef typename CompileTime::SwitchType< not IsAnyReference<T>,
						T,
						typename CompileTime::SwitchType< _qinfo::is_reference or _qinfo::is_rvalue_ref,
								typename _qinfo::type,
								RemoveQualifier< RemoveConstVolatile<T> > >
						>														type;
		};
	}	// _ttraits_hidden_

	// RemoveReference
	template <typename T>
	using RemoveReference		= typename _ttraits_hidden_::_RemoveReference<T>::type;

	// RemoveRValueReference
	template <typename T>
	using RemoveRValueReference	= typename _ttraits_hidden_::_RemoveRValueReference<T>::type;

	// RemoveAnyReference
	template <typename T>
	using RemoveAnyReference	= typename _ttraits_hidden_::_RemoveAnyReference<T>::type;



	//--------- Const Utils --------//
		
	namespace _ttraits_hidden_
	{
		template <typename T>
		struct _RemoveConst {
			typedef _TypeQualifierInfo<T>						_qinfo;
			typedef typename _ApplyTypeQualifiers<
								typename _qinfo::type,
								_qinfo::is_pointer,
								false,
								_qinfo::is_volatile,
								_qinfo::is_reference,
								_qinfo::is_rvalue_ref >::type	type;
		};

		template <typename T>
		struct _InvertConst {
			typedef typename _RemoveConst<T>::type										_nonconst_t;
			typedef typename CompileTime::SwitchType< _TypeQualifierInfo<T>::is_const,
								_nonconst_t, const _nonconst_t >						type;
		};

		template <typename FromType, typename ToType>
		struct _CopyConst {
			STATIC_ASSERT( not IsPointer< ToType > );

			static const bool	_is_const	= _TypeQualifierInfo< FromType >::is_const;
			
			typedef typename _RemoveConst< ToType >::type								_dst;
			typedef typename CompileTime::SwitchType< _is_const, const _dst, _dst >		type;
			typedef typename CompileTime::SwitchType< _is_const, _dst, const _dst >		inv_type;
		};

		template <typename FromType, typename ToType>
		struct _CopyConstToPointer {
			STATIC_ASSERT( IsPointer< ToType > );

			static const bool	_is_const	= _TypeQualifierInfo< FromType >::is_const;
			
			typedef typename _RemoveConst< ToType >::type										_dst;
			typedef typename RemovePointer< _dst >												_unptr;
			typedef typename CompileTime::SwitchType< _is_const, _unptr const *, _unptr * >		type;
			typedef typename CompileTime::SwitchType< _is_const, _unptr *, _unptr const * >		inv_type;
		};

		template <typename FromType, typename ToType>
		struct _CopyConstToReference {
			STATIC_ASSERT( IsLValueReference< ToType > );

			static const bool	_is_const	= _TypeQualifierInfo< FromType >::is_const;
			
			typedef typename _RemoveConst< ToType >::type										_dst;
			typedef typename _RemoveReference< _dst >::type										_unref;
			typedef typename CompileTime::SwitchType< _is_const, _unref const &, _unref & >		type;
			typedef typename CompileTime::SwitchType< _is_const, _unref &, _unref const & >		inv_type;
		};
	}	// _ttraits_hidden_

	// RemoveConst
	template <typename T>
	using RemoveConst			= typename _ttraits_hidden_::_RemoveConst<T>::type;

	// IsConst
	template <typename T>
	constexpr bool IsConst		= _ttraits_hidden_::_TypeQualifierInfo<T>::is_const;

	// InvertConst
	template <typename T>
	using InvertConst			= typename _ttraits_hidden_::_InvertConst<T>::type;

	// CopyConst
	template <typename FromType, typename ToType>
	using CopyConst				= typename _ttraits_hidden_::_CopyConst< FromType, ToType >::type;
		
	// CopyConstToPointer
	template <typename FromType, typename ToType>
	using CopyConstToPointer	= typename _ttraits_hidden_::_CopyConstToPointer< FromType, ToType >::type;
		
	// CopyConstToReference
	template <typename FromType, typename ToType>
	using CopyConstToReference	= typename _ttraits_hidden_::_CopyConstToReference< FromType, ToType >::type;


		
	//--------- Volatile Utils --------//
		
	namespace _ttraits_hidden_
	{
		template <typename T>
		struct _RemoveVolatile {
			typedef _TypeQualifierInfo<T>						_qinfo;
			typedef typename _ApplyTypeQualifiers<
								typename _qinfo::type,
								_qinfo::is_pointer,
								_qinfo::is_const,
								false,
								_qinfo::is_reference,
								_qinfo::is_rvalue_ref >::type		type;
		};

		template <typename T>
		struct _InvertVolatile {
			typedef typename _RemoveVolatile<T>::type										_nonvolatile_t;
			typedef typename CompileTime::SwitchType< _TypeQualifierInfo<T>::is_volatile,
								_nonvolatile_t, volatile _nonvolatile_t >					type;
		};

		template <typename T, typename B>
		struct _CopyVolatile {
			typedef typename CompileTime::SwitchType< _TypeQualifierInfo<T>::is_volatile, volatile B, B >	type;
			typedef typename CompileTime::SwitchType< _TypeQualifierInfo<B>::is_volatile, B, volatile B >	inv_type;
		};
	}	// _ttraits_hidden_

	// RemoveVolatile
	template <typename T>
	using RemoveVolatile		= typename _ttraits_hidden_::_RemoveVolatile<T>::type;

	// IsVolatile
	template <typename T>
	constexpr bool IsVolatile	= _ttraits_hidden_::_TypeQualifierInfo<T>::is_volatile;

	// InvertVolatile
	template <typename T>
	using InvertVolatile		= typename _ttraits_hidden_::_InvertVolatile<T>::type;

	// CopyVolatile
	template <typename T, typename B>
	using CopyVolatile			= typename _ttraits_hidden_::_CopyVolatile<T,B>::type;


		
	//--------- Pointers --------//
		
	namespace _ttraits_hidden_
	{
		template <typename T>
		struct _IsMemberVariablePointer											{ static const bool	value = false; };

		template <typename Class, typename T>
		struct _IsMemberVariablePointer< T Class::* >							{ static const bool	value = true; };
		
		template <typename Class, typename Result, typename ...Args>
		struct _IsMemberVariablePointer< Result (Class::*) (Args...) >			{ static const bool	value = false; };
		
		template <typename Class, typename Result, typename ...Args>
		struct _IsMemberVariablePointer< Result (Class::*) (Args...) const >	{ static const bool	value = false; };

	}	// _ttraits_hidden_

	template <typename T>
	constexpr bool IsMemberVariablePointer	= _ttraits_hidden_::_IsMemberVariablePointer< T >::value;


	namespace _ttraits_hidden_
	{
		template <typename T>
		struct _IsMemberFunctionPointer											{ static const bool	value = false; };

		template <typename Class, typename Result, typename ...Args>
		struct _IsMemberFunctionPointer< Result (Class::*) (Args...) >			{ static const bool	value = true; };
		
		template <typename Class, typename Result, typename ...Args>
		struct _IsMemberFunctionPointer< Result (Class::*) (Args...) const >	{ static const bool	value = true; };

	}	// _ttraits_hidden_

	template <typename T>
	constexpr bool IsMemberFunctionPointer	= _ttraits_hidden_::_IsMemberFunctionPointer< T >::value;


	namespace _ttraits_hidden_
	{
		template <typename T>
		struct _IsFunctionPointer												{ static const bool	value = false; };

		template <typename Result, typename ...Args>
		struct _IsFunctionPointer< Result (*) (Args...) >						{ static const bool	value = true; };
		
		template <typename T>
		struct _IsFunctionPointer< T * >										{ static const bool	value = false; };

	}	// _ttraits_hidden_

	template <typename T>
	constexpr bool IsFunctionPointer	= _ttraits_hidden_::_IsFunctionPointer< T >::value;


		
	//--------- Enums --------//
		
	namespace _ttraits_hidden_
	{
		template <typename T>
		struct _IsEnum			{ static const bool	value = std::is_enum<T>::value; };

	}	// _ttraits_hidden_

	template <typename T>
	constexpr bool IsEnum	= _ttraits_hidden_::_IsEnum< T >::value;

		

	//--------- ResultOf --------//
		
	namespace _ttraits_hidden_
	{
		template <typename T>
		struct _ResultOfFunction										{ typedef typename std::result_of_t<T>	type; };

		template <typename Class, typename Result, typename ...Args>
		struct _ResultOfFunction< Result (Class::*) (Args...) >			{ typedef Result	type; };
		
		template <typename Class, typename Result, typename ...Args>
		struct _ResultOfFunction< Result (Class::*) (Args...) const >	{ typedef Result	type; };

		template <typename Result, typename ...Args>
		struct _ResultOfFunction< Result (*) (Args...) >				{ typedef Result	type; };
		
		template <typename T>
		struct _ResultOfFunction< T * >									{};

	}	// _ttraits_hidden_

	template <typename ...Types>
	using ResultOf	= typename _ttraits_hidden_::_ResultOfFunction< Types... >::type;


	
	//--------- IsMovable --------//
	template <typename T>
	constexpr bool IsMovable	= IsRValueReference<T> or (not IsLValueReference<T> and not IsConstOrVolatile<T> and not IsPointer<T>);


/*
=================================================
	RValueRef
=================================================
*/
	template <typename T>
	struct RValueRef
	{
		typedef typename RemoveAnyReference<T> &&		type;
		STATIC_ASSERT( not IsLValueReference< type > );
		STATIC_ASSERT( not IsConstOrVolatile< type > );
		STATIC_ASSERT( IsRValueReference< type > );
	};

	// std::move
	template <typename T>
	forceinline constexpr typename RValueRef<T>::type  ToRValueRef (T&& arg) noexcept
	{
		return static_cast< typename RValueRef<T>::type >( arg );
	}

/*
=================================================
	Forward
=================================================
*/
	// std::forward
	template <typename T>
	forceinline constexpr T&&  Forward (typename RemoveAnyReference<T>& arg) noexcept
	{
		return static_cast< T&& >( arg );
	}
	
	template <typename T>
	forceinline constexpr T&&  Forward (typename RemoveAnyReference<T>&& arg) noexcept
	{
		return static_cast< T&& >( arg );
	}
	
}	// TypeTraits
}	// GX_STL
