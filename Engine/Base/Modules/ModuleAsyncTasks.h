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
						where->GetModuleByID( _mngrID );

			if ( not result )
				LOG( ("Module '"_str << ToString(GModID::type( _mngrID )) << "' not found").cstr(), ELog::Warning );

			if ( result )
				CHECK( SendTo< ModuleMsg::AddToManager >( result, { CurrentThreadModule() } ));
		}
	};


}	// Base
}	// Engine
