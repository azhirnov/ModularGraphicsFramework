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

	class Module::AttachModuleToManagerAsyncTask final : public AsyncTask< ModulePtr >
	{
	// variables
	private:
		const UntypedID_t	_mngrID;


	// methods
	public:
		AttachModuleToManagerAsyncTask (const ModulePtr &self, const ModulePtr &where, UntypedID_t managerID) :
			AsyncTask( self, where ),
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
		
		void ExecuteInBackground (const ModulePtr &where, OUT ModulePtr &result) override
		{
			result = (_mngrID == where->GetModuleID()) ? 
						where :
						where->GetModule( _mngrID );

			ASSERT( result );

			if ( result )
				CHECK( SendTo( result, Message< ModuleMsg::AddToManager >{ CurrentThreadModule() } ) );
		}
	};
	


	//
	// Detach Module from Manager Async Task
	//
	
	class Module::DetachModuleFromManagerAsyncTask final : public AsyncTask<>
	{
	// variables
	private:
		const UntypedID_t	_mngrID;
		

	// methods
	public:
		DetachModuleFromManagerAsyncTask (const ModulePtr &self, const ModulePtr &manager) :
			AsyncTask( self, manager ),
			_mngrID( manager->GetModuleID() )
		{}

		void PostExecute (const ModulePtr &self, UninitializedType &&) override
		{
			self->_SetManager( null );
		}
		
		void ExecuteInBackground (const ModulePtr &manager, OUT UninitializedType &) override
		{
			ModulePtr	result = (_mngrID == manager->GetModuleID()) ?
									manager :
									manager->GetModule( _mngrID );
			
			if ( result )
				CHECK( SendTo( result, Message< ModuleMsg::RemoveFromManager >{ CurrentThreadModule() } ) );
		}
	};


}	// Base
}	// Engine
