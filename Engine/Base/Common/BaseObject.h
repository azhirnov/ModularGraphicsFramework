// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.
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
		const SubSystemsRef		_globalSystems;


	// methods
	public:
		BaseObject (const SubSystemsRef gs) :
			_globalSystems(gs)
		{}
		
		SubSystemsRef	GlobalSystems ()	const	{ return _globalSystems; }

		StringCRef		GetDebugName ()		const	{ return _debugName; }


		virtual void SetDebugName (StringCRef name)
		{
			DEBUG_ONLY( _debugName = name; )
		}


	protected:
		template <typename T, typename ...Args>
		forceinline SHARED_POINTER_TYPE(T)  New (Args ...args) const noexcept
		{
			return GX_STL::GXTypes::New<T>( GlobalSystems(), FW<Args>(args)... );
		}
	};


	SHARED_POINTER( BaseObject );


}	// Base
}	// Engine