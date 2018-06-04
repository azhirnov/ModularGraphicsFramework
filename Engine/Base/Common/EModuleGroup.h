// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/Common.h"

namespace Engine
{
namespace Base
{

	namespace EModuleGroup
	{
		enum type
		{
			// 0 - reserved
			Global	= 1,
			Thread,
			Object,
			Component,

			// free to use: Object+1 .. _Mask

			_Mask	= CompileTime::_ctime_hidden_::_StringToID_Helper::MASK,

			// max ID's in DECL_STRING_TO_ID
			_Count	= _Mask + 1,
		};
	}

	DECL_STRING_TO_ID( GModID, EModuleGroup::Global );
	DECL_STRING_TO_ID( TModID, EModuleGroup::Thread );
	DECL_STRING_TO_ID( OModID, EModuleGroup::Object );
	DECL_STRING_TO_ID( CModID, EModuleGroup::Component );


}	// Base
}	// Engine


namespace GX_STL
{
namespace GXTypes
{
	
/*
=================================================
	ToString (GModID)
=================================================
*/
	ND_ inline decltype(auto) ToString (Engine::Base::GModID::type value)
	{
		return Engine::Base::GModID::ToString( value );
	}

}	// GXTypes
}	// GX_STL

