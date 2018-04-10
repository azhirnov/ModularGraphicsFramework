// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/ResourcePacker/Packer/Common.h"

namespace ResPack
{

	//
	// File Packer interface
	//

	class IFilePacker
	{
	// interface
	public:
		virtual ~IFilePacker () {}

		virtual bool Load (StringCRef filename) = 0;
		virtual bool Save (StringCRef filename) = 0;
	};


}	// ResPack
