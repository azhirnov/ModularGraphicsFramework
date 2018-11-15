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
	// methods
		template <typename Collection, typename Msg>
		static void Send (const Collection &list, const Msg &msg);

		template <typename Msg>
		static void Send (InitializerList<ModulePtr> list, const Msg &msg);
		
		template <typename Collection>
		static bool Initialize (const Collection &list);
		static bool Initialize (InitializerList<ModulePtr> list);
		
		template <typename Collection>
		static void Destroy (const Collection &list);
		static void Destroy (InitializerList<ModulePtr> list);
	};
	


/*
=================================================
	Send
=================================================
*/
	template <typename Collection, typename Msg>
	inline void ModuleUtils::Send (const Collection &list, const Msg &msg)
	{
		for (auto& mod : list)
		{
			if ( mod )
				mod->Send( msg );
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
	inline bool ModuleUtils::Initialize (const Collection &list)
	{
		ModuleMsg::Link		link_msg;
		ModuleMsg::Compose	comp_msg;

		auto	IsInitialized	= LAMBDA() (Module::EState state)	{ return state == Module::EState::ComposedImmutable or state == Module::EState::ComposedMutable; };
		bool	result			= true;
		
		for (auto& mod : list)
		{
			if ( mod )
				mod->Send( link_msg );
		}
		
		for (auto& mod : list)
		{
			if ( mod )
				mod->Send( comp_msg );
		}
		
		// wait when all modules will be composed
		for (auto& mod : list)
		{
			if ( mod )
				result &= IsInitialized( mod->GetState() );
		}
		return result;
	}

	inline bool ModuleUtils::Initialize (InitializerList<ModulePtr> list)
	{
		return Initialize( ArrayCRef<ModulePtr>( list ) );
	}
	
/*
=================================================
	Destroy
=================================================
*/
	template <typename Collection>
	inline void ModuleUtils::Destroy (const Collection &list)
	{
		ModuleMsg::Delete	del_msg;
		
		for (auto& mod : list)
		{
			if ( mod )
				mod->Send( del_msg );
		}
	}

	inline void ModuleUtils::Destroy (InitializerList<ModulePtr> list)
	{
		return Destroy( ArrayCRef<ModulePtr>( list ) );
	}

}	// Base
}	// Engine
