// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/Common.h"

namespace Engine
{
namespace Base
{

	//
	// Thread ID
	//

	struct ThreadID
	{
	// types
		using Self = ThreadID;

	// varaibles
	private:
		usize	_value;

	// method
	private:
		explicit
		ThreadID (usize id) : _value(id)
		{}

	public:
		ThreadID (GX_DEFCTOR) : _value(0)
		{}

		ND_ static ThreadID GetCurrent ()
		{
			return ThreadID( OS::CurrentThread::GetCurrentThreadId() );
		}

		_GX_DIM_CMP_OPERATORS_SELF( _value );
	};

	
}	// Base
}	// Engine
