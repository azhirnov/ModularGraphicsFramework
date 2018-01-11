// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Modules/Module.h"

namespace Engine
{
namespace Base
{

	//
	// Module Utils
	//

	struct ModuleUtils final
	{
	// types
		using Sender_t		= WeakPointerType< BaseObject >;

	// methods
		template <typename Collection, typename Msg>
		static void Send (const Collection &list, const Msg &msg, const Sender_t &sender = null);

		template <typename Msg>
		static void Send (InitializerList<ModulePtr> list, const Msg &msg, const Sender_t &sender = null);
		
		template <typename Collection>
		static bool Initialize (const Collection &list, const Sender_t &sender = null);
		static bool Initialize (InitializerList<ModulePtr> list, const Sender_t &sender = null);
		
		template <typename Collection>
		static void Destroy (const Collection &list, const Sender_t &sender = null);
		static void Destroy (InitializerList<ModulePtr> list, const Sender_t &sender = null);
	};
	


/*
=================================================
	Send
=================================================
*/
	template <typename Collection, typename Msg>
	inline void ModuleUtils::Send (const Collection &list, const Msg &msg, const Sender_t &sender)
	{
		FOR( i, list ) {
			if ( list[i] ) {
				list[i]->Send( msg.From( sender ) );
			}
		}
	}

	template <typename Msg>
	inline void ModuleUtils::Send (InitializerList<ModulePtr> list, const Msg &msg, const Sender_t &sender)
	{
		return Send( ArrayCRef<ModulePtr>( list ), msg, sender );
	}
	
/*
=================================================
	Initialize
=================================================
*/
	template <typename Collection>
	inline bool ModuleUtils::Initialize (const Collection &list, const Sender_t &sender)
	{
		Message< ModuleMsg::Link >		link_msg;
		Message< ModuleMsg::Compose >	comp_msg;

		auto	IsInitialized	= LAMBDA() (Module::EState state)	{ return state == Module::EState::ComposedImmutable or state == Module::EState::ComposedMutable; };
		bool	result			= true;

		FOR( i, list ) {
			if ( list[i] ) {
				list[i]->Send( link_msg.From( sender ) );
			}
		}

		FOR( i, list ) {
			if ( list[i] ) {
				list[i]->Send( comp_msg.From( sender ) );
			}
		}
		
		// wait when all modules will be composed
		FOR( i, list ) {
			if ( list[i] ) {
				result &= IsInitialized( list[i]->GetState() );
			}
		}
		return result;
	}

	inline bool ModuleUtils::Initialize (InitializerList<ModulePtr> list, const Sender_t &sender)
	{
		return Initialize( ArrayCRef<ModulePtr>( list ), sender );
	}
	
/*
=================================================
	Destroy
=================================================
*/
	template <typename Collection>
	inline void ModuleUtils::Destroy (const Collection &list, const Sender_t &sender)
	{
		Message< ModuleMsg::Delete >	del_msg;

		FOR( i, list ) {
			if ( list[i] ) {
				list[i]->Send( del_msg.From( sender ) );
			}
		}
	}

	inline void ModuleUtils::Destroy (InitializerList<ModulePtr> list, const Sender_t &sender)
	{
		return Destroy( ArrayCRef<ModulePtr>( list ), sender );
	}

}	// Base
}	// Engine
