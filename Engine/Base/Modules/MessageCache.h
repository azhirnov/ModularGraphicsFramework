// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Modules/Module.h"
#include "Engine/STL/Containers/Tuple.h"

namespace Engine
{
namespace Base
{

	namespace _engbase_hidden_
	{
		template <typename T>	struct ExtractMsgResultType									{ using type = typename ExtractMsgResultType< decltype(T::result) >::type; };
		template <typename T>	struct ExtractMsgResultType< Engine::ModuleMsg::Out<T> >	{ using type = T; };
		template <typename T>	struct ExtractMsgResultType< Engine::Base::Message<T> >		{ using type = typename ExtractMsgResultType< decltype(T::result) >::type; };


		template <usize Index, typename Type, typename PrevFuncResult, typename Typelist>
		struct MessagesToResultTypes_Func
		{
			using res		= typename ExtractMsgResultType< Type >::type;
			using result	= CompileTime::TypeList< res, PrevFuncResult >;
		};

		template <typename MsgList>
		using MessagesToResultTypes = typename MsgList::template ReverseForEach< MessagesToResultTypes_Func >;

	}	// _engbase_hidden_



	//
	// Message Cache
	//

	template <typename T0, typename ...Types>
	struct MessageCache
	{
	// types
	public:
		using Messages_t	= CompileTime::TypeListFrom< T0, Types... >;
		using CacheData_t	= Tuple< _engbase_hidden_::MessagesToResultTypes< Messages_t > >;

	private:
		using CacheMap_t	= Map< const void*, CacheData_t >;

		struct _Init_Func
		{
			const ModulePtr&	_module;
			CacheData_t&		_data;

			_Init_Func (const ModulePtr &mod, CacheData_t &data) :
				_module{mod}, _data{data}
			{}

			template <typename T, usize Index>
			void Process ()
			{
				Message<T>	msg;
				CHECK( _module->Send( msg ) );
				ASSERT( msg->result.IsDefined() );

				_data.template Set<Index>( msg->result.Get({}) );
			}
		};


	// variables
	private:
		mutable CacheMap_t		_cache;


	// methods
	public:
		MessageCache ()
		{}


		ND_ CacheData_t const&  Get (const ModulePtr &module) const
		{
			ASSERT( module and (module->GetState() == Module::EState::ComposedImmutable or
								module->GetState() == Module::EState::ComposedMutable) );

			typename CacheMap_t::iterator	iter;

			if ( _cache.Find( module.RawPtr(), OUT iter ) )
			{
				return iter->second;
			}

			// worst case
			iter = _cache.Add( module.RawPtr(), {} );

			_Init_Func	func{ module, iter->second };
			Messages_t::RuntimeForEach( func );

			return iter->second;
		}


		void Erase (const ModulePtr &module)
		{
			_cache.Erase( module.RawPtr() );
		}


		void Clear ()
		{
			_cache.Clear();
		}
	};


}	// Base
}	// Engine
