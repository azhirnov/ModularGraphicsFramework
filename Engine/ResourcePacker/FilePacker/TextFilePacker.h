// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/ResourcePacker/FilePacker/IFilePacker.h"

namespace ResPack
{

	//
	// Text File Packer
	//

	class TextFilePacker final : public IFilePacker
	{
	// variables
	private:
		String		_data;


	// methods
	public:
		TextFilePacker ();

		bool Load (StringCRef filename) override;
		bool Save (StringCRef filename) override;
	};

}	// ResPack
