// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		template <typename Collection, typename Msg>
		static void Send (const Collection &list, const Msg &msg);

		template <typename Msg>
		static void Send (InitializerList<ModulePtr> list, const Msg &msg);
		
		template <typename Collection>
		static bool Initialize (const Collection &list, BaseObjectPtr sender = null);
		static bool Initialize (InitializerList<ModulePtr> list, BaseObjectPtr sender = null);
	};
	


/*
=================================================
	Send
=================================================
*/
	template <typename Collection, typename Msg>
	inline void ModuleUtils::Send (const Collection &list, const Msg &msg)
	{
		FOR( i, list ) {
			list[i]->Send( msg );
		}
	}

	template <typename Msg>
	inline void ModuleUtils::Send (InitializerList<ModulePtr> list, const Msg &msg)
	{
		return Send( ArrayCRef<ModulePtr>( list ), msg );
	}
	
/*
=================================================
	Initialize
=================================================
*/
	template <typename Collection>
	inline bool ModuleUtils::Initialize (const Collection &list, BaseObjectPtr sender)
	{
		Message< ModuleMsg::Link >		link_msg;
		Message< ModuleMsg::Compose >	comp_msg;

		auto	IsInitialized	= LAMBDA() (Module::EState state)	{ return state == Module::EState::ComposedImmutable or state == Module::EState::ComposedMutable; };
		bool	result			= true;

		FOR( i, list ) {
			list[i]->Send( link_msg.From( sender ) );
		}

		FOR( i, list ) {
			list[i]->Send( comp_msg.From( sender ) );
		}
		
		// wait when all modules will be composed
		FOR( i, list ) {
			result &= IsInitialized( list[i]->GetState() );
		}
		return result;
	}

	inline bool ModuleUtils::Initialize (InitializerList<ModulePtr> list, BaseObjectPtr sender)
	{
		return Initialize( ArrayCRef<ModulePtr>( list ), sender );
	}

}	// Base
}	// Engine
