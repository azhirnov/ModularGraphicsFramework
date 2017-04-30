// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Tasks/AsyncTask.h"

namespace Engine
{
namespace Base
{
	
	//
	// Attach Module to Manager Async Task
	//

	class Module::AttachModuleToManagerAsyncTask : public AsyncTask< ModulePtr >
	{
	// variables
	private:
		const UntypedID_t	_mngrID;


	// methods
	public:
		AttachModuleToManagerAsyncTask (const ModulePtr &self, UntypedID_t managerID) :
			AsyncTask( self, self->GlobalSystems()->Get<MainSystem>().ptr() ),
			_mngrID( managerID )
		{}

		AttachModuleToManagerAsyncTask (const ModulePtr &self, const ModulePtr &manager) :
			AsyncTask( self, manager ),
			_mngrID( manager->GetModuleID() )
		{}

		void PostExecute (const ModulePtr &self, ModulePtr &&result) override
		{
			self->_SetManager( result );
		}
		
		void ExecuteInBackground (const ModulePtr &mainSystem, OUT ModulePtr &result) override
		{
			result = (_mngrID == mainSystem->GetModuleID()) ? 
						mainSystem :
						mainSystem->GetModule( _mngrID );	// TODO: use deep search

			if ( result )
				result->Send( Message< ModuleMsg::AddToManager >{ this, CurrentThreadModule() } );
		}
	};
	


	//
	// Detach Module from Manager Async Task
	//
	
	class Module::DetachModuleFromManagerAsyncTask : public AsyncTask<>
	{
	// variables
	private:
		const UntypedID_t	_mngrID;
		

	// methods
	public:
		DetachModuleFromManagerAsyncTask (const ModulePtr &self, UntypedID_t managerID) :
			AsyncTask( self, self->GlobalSystems()->Get<MainSystem>().ptr() ),
			_mngrID( managerID )
		{}

		DetachModuleFromManagerAsyncTask (const ModulePtr &self, const ModulePtr &manager) :
			AsyncTask( self, manager ),
			_mngrID( manager->GetModuleID() )
		{}

		void PostExecute (const ModulePtr &self, UninitializedType &&) override
		{
			self->_SetManager( null );
		}
		
		void ExecuteInBackground (const ModulePtr &mainSystem, OUT UninitializedType &) override
		{
			ModulePtr	result = (_mngrID == mainSystem->GetModuleID()) ?
									mainSystem :
									mainSystem->GetModule( _mngrID );	// TODO: use deep search
			
			if ( result )
				result->Send( Message< ModuleMsg::RemoveFromManager >{ this, CurrentThreadModule() } );
		}
	};


}	// Base
}	// Engine
