// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	BaseObject - base class for all modules, systems and other engine classes.
	BaseObject can be static or dynamic, for dynamic creation use static function,
	which return shared pointer type, 'new' operator supported, but not safe.
*/

#pragma once

#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Common/EngineSubSystems.h"


namespace Engine
{
namespace Base
{

	//
	// Base Object
	//

	class BaseObject : public StaticRefCountedObject
	{
	// variables
	private:
		DEBUG_ONLY(
			String				_debugName;
		)
		const GlobalSystemsRef		_globalSystems;


	// methods
	public:
		BaseObject (const GlobalSystemsRef gs) :
			_globalSystems(gs)
		{}
		
		GlobalSystemsRef	GlobalSystems ()	const	{ return _globalSystems; }

		StringCRef			GetDebugName ()		const
		{
			DEBUG_ONLY( return _debugName; )
			RELEASE_ONLY( return ""; )
		}

		virtual void SetDebugName (StringCRef name)
		{
			DEBUG_ONLY( _debugName = name; )
		}


	protected:
		// helper to capture sender
		template <typename T>
		bool SendTo (const ModulePtr &target, const Message<T> &msg) const;
	};


	SHARED_POINTER( BaseObject );


}	// Base
}	// Engine
