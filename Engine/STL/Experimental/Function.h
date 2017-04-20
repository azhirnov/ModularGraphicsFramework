// Copyright © 2014-2016  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/CompileTime/NewTypeInfo.h"
#include "Engine/STL/CompileTime/TypeTraits.h"
#include "Engine/STL/CompileTime/TemplateUtils.h"
#include "Engine/STL/CompileTime/TypeList.h"
#include "Engine/STL/Containers/Set.h"
#include "UniquePtr.h"
#include "Ptr.h"
#include "BufferedPtr.h"

namespace GX_STL
{
namespace GXTypes
{

#	define UNREF( _t_ )		typename _types_hidden_::RemoveReference<_t_>::type
#	define REF( _t_ )		typename _types_hidden_::AddConstReference<_t_>::type
#	define MOVE( _v_ )		_v_( RVREF( o._v_ ) )


	namespace _types_hidden_
	{
		//
		// Remove Reference qualifiers (for function class fields)
		//

		template <typename T>
		struct RemoveReference {
			typedef T			type;
		};

		template <typename T>
		struct RemoveReference <const T &> {
			typedef const T		type;
		};

		template <typename T>
		struct RemoveReference <T &> {
			typedef void		type;	// reference not supported
		};


		//
		// Add Const Reference qualifiers (for function and delegate arguments)
		//

		template <typename T>
		struct AddConstReference {
			typedef typename CompileTime::SwitchType< (sizeof(T) > sizeof(void*)), const T &, T >	type;
		};

		template <typename T>
		struct AddConstReference <const T &> {
			typedef const T &		type;
		};

		template <typename T>
		struct AddConstReference <T &> {
			typedef T &				type;	// reference supported only for delegates
		};


		//
		// Delegate Interface
		//

		template <typename Func>
		struct TDelegateInterface {};
	

		//
		// Raw Pointer type
		//

		template <typename T>	struct RawPointer		{ typedef typename T::value_t	type;	typedef T		ptr_t; };
		template <typename T>	struct RawPointer<T *>	{ typedef T						type;	typedef Ptr<T>	ptr_t; };

	}	// _types_hidden_
		
	

	//
	// Function Builder
	//

	namespace FunctionBuilder
	{
		template <typename Func>	struct Type {};
	}



	//
	// Function
	//

	template <typename Ret, usize BufSize = GlobalConst::STL_FunctionInterfaceSize>
	struct Function
	{
		template <typename Func>
		friend struct FunctionBuilder::Type;

	// types
	public:
		struct Interface
		{
		// types
			typedef BufferedPtr< Interface, BufSize >	ptr_t;

		// interface
			virtual ~Interface () {}

			virtual bool		IsValid ()				const = 0;
			virtual Ret			Call ()					const = 0;
			virtual void		CloneTo (OUT ptr_t &)	const = 0;
			virtual void		MoveTo (OUT ptr_t &p)				{ CloneTo( p ); }
			virtual TypeId_t	TypeId ()				const = 0;
			virtual usize		Size ()					const = 0;
			
		// methods
			bool Cmp (const Interface *p) const
			{
				const usize size = Size();
				return	( size == p->Size() ) and
						( TypeId() == p->TypeId() ) and
						UnsafeMem::MemCmp( (void *)this, (void *)p, size ) == 0;
			}
			
			bool Less (const Interface *p) const
			{
				const usize		size0 = Size();
				const usize		size1 = p->Size();
				const TypeId_t	type0 = TypeId();
				const TypeId_t	type1 = p->TypeId();
				
				if ( type0 == type1 ) {
					if ( size0 == size1 )
						return UnsafeMem::MemCmp( (void *)this, (void *)p, size0 ) < 0;
					else
						return size0 < size1;
				}
				return type0 < type1;
			}
			
			bool Greater (const Interface *p) const
			{
				const usize		size0 = Size();
				const usize		size1 = p->Size();
				const TypeId_t	type0 = TypeId();
				const TypeId_t	type1 = p->TypeId();
				
				if ( type0 == type1 ) {
					if ( size0 == size1 )
						return UnsafeMem::MemCmp( (void *)this, (void *)p, size0 ) > 0;
					else
						return size0 > size1;
				}
				return type0 > type1;
			}
		};

		typedef Function< Ret, BufSize >	Self;
		typedef typename Interface::ptr_t	interface_ptr_t;


	// variables
	private:
		interface_ptr_t		_func;


	// methods
	public:
		Function (GX_DEFCTOR) : _func()
		{}

		Function (const Self &other)
		{
			if ( other._func ) {
				other._func->CloneTo( _func );
			}
		}

		Function (Self &&other)
		{
			if ( other._func ) {
				other._func->MoveTo( _func );
			}
		}
		

		Self &	operator =  (const Self &right)
		{
			if ( right._func ) {
				right._func->CloneTo( _func );
			}
			return *this;
		}


		void MoveTo (Self &other)
		{
			if ( other._func ) {
				other._func->MoveTo( _func );
			}
		}


		bool	IsValid ()						const	{ return _func and _func->IsValid(); }

		Ret		Call ()							const	{ ASSERT( IsValid() );  return _func->Call(); }
		Ret		SafeCall ()						const	{ return IsValid() ? _func->Call() : Ret(); }
		
		Ret		operator () ()					const	{ ASSERT( IsValid() );  return _func->Call(); }

		bool	operator == (const Self &right)	const	{ ASSERT( IsValid() );  return _func->Cmp( right ); }
		bool	operator <  (const Self &right)	const	{ ASSERT( IsValid() );  return _func->Less( right._func.ptr() ); } \
		bool	operator >  (const Self &right)	const	{ ASSERT( IsValid() );  return _func->Greater( right._func.ptr() ); } \
		bool	operator != (const Self &right)	const	{ return not (*this == right); } \
		bool	operator <= (const Self &right)	const	{ return not (*this >  right); } \
		bool	operator >= (const Self &right)	const	{ return not (*this <  right); } \
	};


	//
	// Delegate
	//

	template <typename Func>
	struct Delegate {};

	namespace DelegateBuilder {}

				
	//
	// Event
	//

	template <typename Func>
	struct Event {};


	//
	// Static Function & Member Function
	// Static Delegate & Member Delegate
	//
#	define F_RETURN \
		Ret
	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0, typename T1, typename T2, typename T3, typename T4, \
		typename T5, typename T6, typename T7, typename T8, typename T9, \
		typename T10, typename T11, typename T12, typename T13, typename T14

#	define F_TEMPLATE_TYPES \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14


#	define F_DECL_PARAMS \
		T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0, REF(T1) v1, REF(T2) v2, REF(T3) v3, REF(T4) v4, REF(T5) v5, REF(T6) v6, REF(T7) v7, \
		REF(T8) v8, REF(T9) v9, REF(T10) v10, REF(T11) v11, REF(T12) v12, REF(T13) v13, REF(T14) v14

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0; UNREF(T1) _v1; UNREF(T2) _v2; UNREF(T3) _v3; UNREF(T4) _v4; \
		UNREF(T5) _v5; UNREF(T6) _v6; UNREF(T7) _v7; UNREF(T8) _v8; UNREF(T9) _v9; \
		UNREF(T10) _v10; UNREF(T11) _v11; UNREF(T12) _v12; UNREF(T13) _v13; UNREF(T14) _v14;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0), _v1(v1), _v2(v2), _v3(v3), _v4(v4), _v5(v5), _v6(v6), _v7(v7), _v8(v8), \
		_v9(v9), _v10(v10), _v11(v11), _v12(v12), _v13(v13), _v14(v14)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0), MOVE(_v1), MOVE(_v2), MOVE(_v3), MOVE(_v4), MOVE(_v5), MOVE(_v6), MOVE(_v7), \
		MOVE(_v8), MOVE(_v9), MOVE(_v10), MOVE(_v11), MOVE(_v12), MOVE(_v13), MOVE(_v14)

#	define F_ARG_PARAMS \
		_v0, _v1, _v2, _v3, _v4, _v5, _v6, _v7, _v8, _v9, _v10, _v11, _v12, _v13, _v14

#	define F_FUNC_ARGS \
		v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14

#	define F_UID \
		15

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0, typename T1, typename T2, typename T3, typename T4, \
		typename T5, typename T6, typename T7, typename T8, typename T9, \
		typename T10, typename T11, typename T12, typename T13

#	define F_TEMPLATE_TYPES \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13

#	define F_DECL_PARAMS \
		T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0, REF(T1) v1, REF(T2) v2, REF(T3) v3, REF(T4) v4, REF(T5) v5, REF(T6) v6, REF(T7) v7, \
		REF(T8) v8, REF(T9) v9, REF(T10) v10, REF(T11) v11, REF(T12) v12, REF(T13) v13

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0; UNREF(T1) _v1; UNREF(T2) _v2; UNREF(T3) _v3; UNREF(T4) _v4; \
		UNREF(T5) _v5; UNREF(T6) _v6; UNREF(T7) _v7; UNREF(T8) _v8; UNREF(T9) _v9; \
		UNREF(T10) _v10; UNREF(T11) _v11; UNREF(T12) _v12; UNREF(T13) _v13;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0), _v1(v1), _v2(v2), _v3(v3), _v4(v4), _v5(v5), _v6(v6), _v7(v7), _v8(v8), \
		_v9(v9), _v10(v10), _v11(v11), _v12(v12), _v13(v13)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0), MOVE(_v1), MOVE(_v2), MOVE(_v3), MOVE(_v4), MOVE(_v5), MOVE(_v6), MOVE(_v7), \
		MOVE(_v8), MOVE(_v9), MOVE(_v10), MOVE(_v11), MOVE(_v12), MOVE(_v13)

#	define F_ARG_PARAMS \
		_v0, _v1, _v2, _v3, _v4, _v5, _v6, _v7, _v8, _v9, _v10, _v11, _v12, _v13

#	define F_FUNC_ARGS \
		v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13

#	define F_UID \
		14

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0, typename T1, typename T2, typename T3, typename T4, \
		typename T5, typename T6, typename T7, typename T8, typename T9, \
		typename T10, typename T11, typename T12

#	define F_TEMPLATE_TYPES \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, void, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12

#	define F_DECL_PARAMS \
		T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0, REF(T1) v1, REF(T2) v2, REF(T3) v3, REF(T4) v4, REF(T5) v5, REF(T6) v6, REF(T7) v7, \
		REF(T8) v8, REF(T9) v9, REF(T10) v10, REF(T11) v11, REF(T12) v12

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0; UNREF(T1) _v1; UNREF(T2) _v2; UNREF(T3) _v3; UNREF(T4) _v4; \
		UNREF(T5) _v5; UNREF(T6) _v6; UNREF(T7) _v7; UNREF(T8) _v8; UNREF(T9) _v9; \
		UNREF(T10) _v10; UNREF(T11) _v11; UNREF(T12) _v12;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0), _v1(v1), _v2(v2), _v3(v3), _v4(v4), _v5(v5), _v6(v6), _v7(v7), _v8(v8), \
		_v9(v9), _v10(v10), _v11(v11), _v12(v12)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0), MOVE(_v1), MOVE(_v2), MOVE(_v3), MOVE(_v4), MOVE(_v5), MOVE(_v6), MOVE(_v7), \
		MOVE(_v8), MOVE(_v9), MOVE(_v10), MOVE(_v11), MOVE(_v12)

#	define F_ARG_PARAMS \
		_v0, _v1, _v2, _v3, _v4, _v5, _v6, _v7, _v8, _v9, _v10, _v11, _v12

#	define F_FUNC_ARGS \
		v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12

#	define F_UID \
		13

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0, typename T1, typename T2, typename T3, typename T4, \
		typename T5, typename T6, typename T7, typename T8, typename T9, \
		typename T10, typename T11

#	define F_TEMPLATE_TYPES \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, void, void, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11

#	define F_DECL_PARAMS \
		T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0, REF(T1) v1, REF(T2) v2, REF(T3) v3, REF(T4) v4, REF(T5) v5, REF(T6) v6, REF(T7) v7, \
		REF(T8) v8, REF(T9) v9, REF(T10) v10, REF(T11) v11

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0; UNREF(T1) _v1; UNREF(T2) _v2; UNREF(T3) _v3; UNREF(T4) _v4; \
		UNREF(T5) _v5; UNREF(T6) _v6; UNREF(T7) _v7; UNREF(T8) _v8; UNREF(T9) _v9; \
		UNREF(T10) _v10; UNREF(T11) _v11;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0), _v1(v1), _v2(v2), _v3(v3), _v4(v4), _v5(v5), _v6(v6), _v7(v7), _v8(v8), \
		_v9(v9), _v10(v10), _v11(v11)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0), MOVE(_v1), MOVE(_v2), MOVE(_v3), MOVE(_v4), MOVE(_v5), MOVE(_v6), MOVE(_v7), \
		MOVE(_v8), MOVE(_v9), MOVE(_v10), MOVE(_v11)

#	define F_ARG_PARAMS \
		_v0, _v1, _v2, _v3, _v4, _v5, _v6, _v7, _v8, _v9, _v10, _v11

#	define F_FUNC_ARGS \
		v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11

#	define F_UID \
		12

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0, typename T1, typename T2, typename T3, typename T4, \
		typename T5, typename T6, typename T7, typename T8, typename T9, \
		typename T10

#	define F_TEMPLATE_TYPES \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, void, void, void, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10

#	define F_DECL_PARAMS \
		T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0, REF(T1) v1, REF(T2) v2, REF(T3) v3, REF(T4) v4, REF(T5) v5, REF(T6) v6, REF(T7) v7, \
		REF(T8) v8, REF(T9) v9, REF(T10) v10

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0; UNREF(T1) _v1; UNREF(T2) _v2; UNREF(T3) _v3; UNREF(T4) _v4; \
		UNREF(T5) _v5; UNREF(T6) _v6; UNREF(T7) _v7; UNREF(T8) _v8; UNREF(T9) _v9; \
		UNREF(T10) _v10;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0), _v1(v1), _v2(v2), _v3(v3), _v4(v4), _v5(v5), _v6(v6), _v7(v7), _v8(v8), \
		_v9(v9), _v10(v10)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0), MOVE(_v1), MOVE(_v2), MOVE(_v3), MOVE(_v4), MOVE(_v5), MOVE(_v6), MOVE(_v7), \
		MOVE(_v8), MOVE(_v9), MOVE(_v10)

#	define F_ARG_PARAMS \
		_v0, _v1, _v2, _v3, _v4, _v5, _v6, _v7, _v8, _v9, _v10

#	define F_FUNC_ARGS \
		v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10

#	define F_UID \
		11

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0, typename T1, typename T2, typename T3, typename T4, \
		typename T5, typename T6, typename T7, typename T8, typename T9

#	define F_TEMPLATE_TYPES \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, void, void, void, void, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9

#	define F_DECL_PARAMS \
		T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0, REF(T1) v1, REF(T2) v2, REF(T3) v3, REF(T4) v4, REF(T5) v5, REF(T6) v6, REF(T7) v7, \
		REF(T8) v8, REF(T9) v9

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0; UNREF(T1) _v1; UNREF(T2) _v2; UNREF(T3) _v3; UNREF(T4) _v4; \
		UNREF(T5) _v5; UNREF(T6) _v6; UNREF(T7) _v7; UNREF(T8) _v8; UNREF(T9) _v9;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0), _v1(v1), _v2(v2), _v3(v3), _v4(v4), _v5(v5), _v6(v6), _v7(v7), _v8(v8), \
		_v9(v9)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0), MOVE(_v1), MOVE(_v2), MOVE(_v3), MOVE(_v4), MOVE(_v5), MOVE(_v6), MOVE(_v7), \
		MOVE(_v8), MOVE(_v9)

#	define F_ARG_PARAMS \
		_v0, _v1, _v2, _v3, _v4, _v5, _v6, _v7, _v8, _v9

#	define F_FUNC_ARGS \
		v0, v1, v2, v3, v4, v5, v6, v7, v8, v9

#	define F_UID \
		10

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0, typename T1, typename T2, typename T3, typename T4, \
		typename T5, typename T6, typename T7, typename T8

#	define F_TEMPLATE_TYPES \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, T8, void, void, void, void, void, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, T8

#	define F_DECL_PARAMS \
		T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0, REF(T1) v1, REF(T2) v2, REF(T3) v3, REF(T4) v4, REF(T5) v5, REF(T6) v6, REF(T7) v7, \
		REF(T8) v8

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0; UNREF(T1) _v1; UNREF(T2) _v2; UNREF(T3) _v3; UNREF(T4) _v4; \
		UNREF(T5) _v5; UNREF(T6) _v6; UNREF(T7) _v7; UNREF(T8) _v8;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0), _v1(v1), _v2(v2), _v3(v3), _v4(v4), _v5(v5), _v6(v6), _v7(v7), _v8(v8)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0), MOVE(_v1), MOVE(_v2), MOVE(_v3), MOVE(_v4), MOVE(_v5), MOVE(_v6), MOVE(_v7), \
		MOVE(_v8)

#	define F_ARG_PARAMS \
		_v0, _v1, _v2, _v3, _v4, _v5, _v6, _v7, _v8

#	define F_FUNC_ARGS \
		v0, v1, v2, v3, v4, v5, v6, v7, v8

#	define F_UID \
		9

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0, typename T1, typename T2, typename T3, typename T4, \
		typename T5, typename T6, typename T7

#	define F_TEMPLATE_TYPES \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7, void, void, void, void, void, void, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0, T1, T2, T3, T4, T5, T6, T7

#	define F_DECL_PARAMS \
		T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0, REF(T1) v1, REF(T2) v2, REF(T3) v3, REF(T4) v4, REF(T5) v5, REF(T6) v6, REF(T7) v7

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0; UNREF(T1) _v1; UNREF(T2) _v2; UNREF(T3) _v3; UNREF(T4) _v4; \
		UNREF(T5) _v5; UNREF(T6) _v6; UNREF(T7) _v7;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0), _v1(v1), _v2(v2), _v3(v3), _v4(v4), _v5(v5), _v6(v6), _v7(v7)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0), MOVE(_v1), MOVE(_v2), MOVE(_v3), MOVE(_v4), MOVE(_v5), MOVE(_v6), MOVE(_v7)

#	define F_ARG_PARAMS \
		_v0, _v1, _v2, _v3, _v4, _v5, _v6, _v7

#	define F_FUNC_ARGS \
		v0, v1, v2, v3, v4, v5, v6, v7

#	define F_UID \
		8

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0, typename T1, typename T2, typename T3, typename T4, \
		typename T5, typename T6

#	define F_TEMPLATE_TYPES \
		Ret, T0, T1, T2, T3, T4, T5, T6, void, void, void, void, void, void, void, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0, T1, T2, T3, T4, T5, T6

#	define F_DECL_PARAMS \
		T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0, REF(T1) v1, REF(T2) v2, REF(T3) v3, REF(T4) v4, REF(T5) v5, REF(T6) v6

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0; UNREF(T1) _v1; UNREF(T2) _v2; UNREF(T3) _v3; UNREF(T4) _v4; \
		UNREF(T5) _v5; UNREF(T6) _v6;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0), _v1(v1), _v2(v2), _v3(v3), _v4(v4), _v5(v5), _v6(v6)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0), MOVE(_v1), MOVE(_v2), MOVE(_v3), MOVE(_v4), MOVE(_v5), MOVE(_v6)

#	define F_ARG_PARAMS \
		_v0, _v1, _v2, _v3, _v4, _v5, _v6

#	define F_FUNC_ARGS \
		v0, v1, v2, v3, v4, v5, v6

#	define F_UID \
		7

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0, typename T1, typename T2, typename T3, typename T4, \
		typename T5

#	define F_TEMPLATE_TYPES \
		Ret, T0, T1, T2, T3, T4, T5, void, void, void, void, void, void, void, void, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0, T1, T2, T3, T4, T5

#	define F_DECL_PARAMS \
		T0 v0, T1 v1, T2 v2, T3 v3, T4 v4, T5 v5

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0, REF(T1) v1, REF(T2) v2, REF(T3) v3, REF(T4) v4, REF(T5) v5

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0; UNREF(T1) _v1; UNREF(T2) _v2; UNREF(T3) _v3; UNREF(T4) _v4; \
		UNREF(T5) _v5;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0), _v1(v1), _v2(v2), _v3(v3), _v4(v4), _v5(v5)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0), MOVE(_v1), MOVE(_v2), MOVE(_v3), MOVE(_v4), MOVE(_v5)

#	define F_ARG_PARAMS \
		_v0, _v1, _v2, _v3, _v4, _v5

#	define F_FUNC_ARGS \
		v0, v1, v2, v3, v4, v5

#	define F_UID \
		6

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0, typename T1, typename T2, typename T3, typename T4

#	define F_TEMPLATE_TYPES \
		Ret, T0, T1, T2, T3, T4, void, void, void, void, void, void, void, void, void, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0, T1, T2, T3, T4

#	define F_DECL_PARAMS \
		T0 v0, T1 v1, T2 v2, T3 v3, T4 v4

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0, REF(T1) v1, REF(T2) v2, REF(T3) v3, REF(T4) v4

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0; UNREF(T1) _v1; UNREF(T2) _v2; UNREF(T3) _v3; UNREF(T4) _v4;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0), _v1(v1), _v2(v2), _v3(v3), _v4(v4)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0), MOVE(_v1), MOVE(_v2), MOVE(_v3), MOVE(_v4)

#	define F_ARG_PARAMS \
		_v0, _v1, _v2, _v3, _v4

#	define F_FUNC_ARGS \
		v0, v1, v2, v3, v4

#	define F_UID \
		5

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0, typename T1, typename T2, typename T3

#	define F_TEMPLATE_TYPES \
		Ret, T0, T1, T2, T3, void, void, void, void, void, void, void, void, void, void, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0, T1, T2, T3

#	define F_DECL_PARAMS \
		T0 v0, T1 v1, T2 v2, T3 v3

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0, REF(T1) v1, REF(T2) v2, REF(T3) v3

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0; UNREF(T1) _v1; UNREF(T2) _v2; UNREF(T3) _v3;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0), _v1(v1), _v2(v2), _v3(v3)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0), MOVE(_v1), MOVE(_v2), MOVE(_v3)

#	define F_ARG_PARAMS \
		_v0, _v1, _v2, _v3

#	define F_FUNC_ARGS \
		v0, v1, v2, v3

#	define F_UID \
		4

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0, typename T1, typename T2

#	define F_TEMPLATE_TYPES \
		Ret, T0, T1, T2, void, void, void, void, void, void, void, void, void, void, void, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0, T1, T2

#	define F_DECL_PARAMS \
		T0 v0, T1 v1, T2 v2

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0, REF(T1) v1, REF(T2) v2

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0; UNREF(T1) _v1; UNREF(T2) _v2;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0), _v1(v1), _v2(v2)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0), MOVE(_v1), MOVE(_v2)

#	define F_ARG_PARAMS \
		_v0, _v1, _v2

#	define F_FUNC_ARGS \
		v0, v1, v2

#	define F_UID \
		3

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0, typename T1

#	define F_TEMPLATE_TYPES \
		Ret, T0, T1, void, void, void, void, void, void, void, void, void, void, void, void, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0, T1

#	define F_DECL_PARAMS \
		T0 v0, T1 v1

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0, REF(T1) v1

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0; UNREF(T1) _v1;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0), _v1(v1)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0), MOVE(_v1)

#	define F_ARG_PARAMS \
		_v0, _v1

#	define F_FUNC_ARGS \
		v0, v1

#	define F_UID \
		2

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret, \
		typename T0

#	define F_TEMPLATE_TYPES \
		Ret, T0, void, void, void, void, void, void, void, void, void, void, void, void, void, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret, T0

#	define F_DECL_PARAMS \
		T0 v0

#	define F_DECL_PARAMS_WITH_REF \
		REF(T0) v0

#	define F_MEMBER_PARAMS \
		UNREF(T0) _v0;
		
#	define F_ARGS_TO_MEMBERS , \
		_v0(v0)

#	define F_MEMBERS_MOVE_CTOR , \
		MOVE(_v0)

#	define F_ARG_PARAMS \
		_v0

#	define F_FUNC_ARGS \
		v0

#	define F_UID \
		1

#	include "FunctionImpl.h"

	
#	define F_TEMPLATE_PARAMS \
		typename Ret

#	define F_TEMPLATE_TYPES \
		Ret, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void

#	define F_TEMPLATE_TYPES_VAR \
		Ret

#	define F_DECL_PARAMS

#	define F_DECL_PARAMS_WITH_REF

#	define F_MEMBER_PARAMS
		
#	define F_ARGS_TO_MEMBERS

#	define F_MEMBERS_MOVE_CTOR

#	define F_ARG_PARAMS

#	define F_FUNC_ARGS

#	define F_UID \
		0

#	include "FunctionImpl.h"
	
	

#	undef UNREF
#	undef MOVE
#	undef REF
#	undef F_RETURN
#	undef F_FIRST_DECLARATION


}	// GXTypes
}	// GX_STL
