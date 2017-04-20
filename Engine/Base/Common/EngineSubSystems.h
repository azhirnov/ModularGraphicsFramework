// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.
/*
	EngineSubSystems class used for access to engine systems at any place in code.
*/

#pragma once

#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Common/ThreadID.h"

namespace Engine
{
namespace Base
{


	//
	// Const Reference
	//

	template <typename T>
	struct ConstReference
	{
	// types
	public:
		using Self	= ConstReference<T>;


	// variables
	private:
		mutable Ptr< T >	_value;


	// methods
	private:
		ConstReference () {}
		
		ConstReference (T &&value) {}

		void operator = (const Self &) {}

	public:
		explicit ConstReference (T &value)			: _value(&value) {}
		explicit ConstReference (const T *value)	: _value((T*)value) {}
				 ConstReference (const Self &other) : _value(other._value) {}
		explicit ConstReference (NullPtr_t)			: _value(null) {}

		T *			operator -> ()	const			{ return _value; }
	};

}	// Base



	//
	// SubSystems TypeList
	//

	template <typename Typelist>
	struct SubSystemsTypeList
	{
	private:
		STATIC_ASSERT( CompileTime::IsTypeList< Typelist > );

		template <typename Type>
		struct _IndexOf {
			static const usize	value = Typelist::IndexOf< Type >;
			STATIC_ASSERT( value != usize(-1) );
		};

		template <typename Typelist2>
		struct _Append {
			STATIC_ASSERT( CompileTime::IsTypeList< Typelist2 > );
			typedef typename Typelist::template Append< Typelist2 >	type;
		};

	public:
		template <typename Typelist2>
		using Append = SubSystemsTypeList< typename _Append< Typelist2 >::type >;
		
		template <typename Type>
		static constexpr usize IndexOf = _IndexOf< Type >::value;
	};
	
	template <typename Typelist>
	template <typename Type>
	constexpr usize SubSystemsTypeList< Typelist >::IndexOf;


	
	// Global
	using GlobalSystemsTypeList_t		= SubSystemsTypeList< CompileTime::TypeListFrom<
												Base::MainSystem,		// is it needed here?
												Base::ModulesFactory
											> >;

	// PerThread
	using PerThreadSystemsTypeList_t	= GlobalSystemsTypeList_t::Append< CompileTime::TypeListFrom<
												Base::TaskModule,		// epic fail for abstract modules, but I need a type for access to typelist
												Base::ParallelThread	// epic fail for abstract modules, but I need a type for access to typelist
											> >;



	//
	// All Engine subsystems
	//

	class EngineSubSystems : public Noncopyable
	{
	// types
	private:
		template <typename T> struct Setter {};

		struct Item
		{
			mutable void *	ptr = null;
			TypeId			id;

			template <typename T>
			void Set (T *p)
			{
				CompileTime::MustBeSameSize< decltype(ptr), decltype(p) >();

				ASSERT( (ptr == null) == (p != null) );
				ASSERT( TypeIdOf<T>() == id or id == TypeId() );

				ptr = reinterpret_cast< void *>( p );
				id	= TypeIdOf<T>();
			}

			template <typename T>
			Ptr<T> Get () const
			{
				ASSERT( TypeIdOf<T>() == id );
				return reinterpret_cast< T *>( ptr );
			}
		};

		using ItemsArray_t	= StaticArray< Item, GlobalConst::Base_MaxSubSystems >;


	// variables
	private:
		ItemsArray_t		_items;
		Base::ThreadID		_threadId;


	// methods
	public:
		EngineSubSystems () :
			_threadId( Base::ThreadID::GetCurrent() )
		{}

		template <typename T>
		Ptr<T> Get () const;

		template <typename T>
		Setter<T> GetSetter ()
		{
			ASSERT( _CheckThread() );
			return Setter<T>( this );
		}


	private:
		bool _CheckThread () const	{ return _threadId == Base::ThreadID::GetCurrent(); }
	};


	using SubSystemsRef	= Base::ConstReference< EngineSubSystems >;


#	define GX_SUBSYSTEM_DECL( _typelist_, _type_, _availableFor_ ) \
		namespace Engine \
		{ \
			template <> \
			struct EngineSubSystems::Setter< _type_ > \
			{ \
				friend class _type_; \
				friend class _availableFor_; \
				Setter (EngineSubSystems *ss) : _ss(ss) {} \
				\
			private: \
				EngineSubSystems *_ss; \
				\
				void Set (_type_ *ptr) \
				{ \
					_ss->_items[ _typelist_::IndexOf< _type_ > ].Set( ptr ); \
				} \
			}; \
			\
			template <> \
			inline Ptr< _type_ > EngineSubSystems::Get () const \
			{ \
				ASSERT( _CheckThread() ); \
				return this->_items[ _typelist_::IndexOf< _type_ > ].Get< _type_ >(); \
			} \
		}

}	// Engine


GX_SUBSYSTEM_DECL( GlobalSystemsTypeList_t,		Base::MainSystem,		Base::ThreadManager );
GX_SUBSYSTEM_DECL( GlobalSystemsTypeList_t,		Base::ModulesFactory,	Base::ThreadManager );

GX_SUBSYSTEM_DECL( PerThreadSystemsTypeList_t,	Base::TaskModule,		Base::TaskModule );
GX_SUBSYSTEM_DECL( PerThreadSystemsTypeList_t,	Base::ParallelThread,	Base::ParallelThread );
