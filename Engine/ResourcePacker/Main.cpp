// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/ResourcePacker/Packer/ResourcePacker.h"

using namespace ResPack;

int main (int argc, char** argv)
{
	CHECK_ERR( argv > 0 );

	auto packer = ResPack::ResourcePacker::Instance();

	CHECK_ERR( packer->Initialize(), -1 );

	Array<String>	res_files;

	for (int i = 1; i < argc; ++i)
	{
		StringCRef	key = argv[i];
		StringCRef	value;

		if ( ++i < argc )
			value = argv[i];

		if ( key == "-R" or key == "-r" )
		{
			res_files << value;
		}
		else
		{
			RETURN_ERR( "unsupported command arg: " << key, 1 );
		}
	}

	packer->SetExecutable( argv[0] );

	for (auto& res : res_files) {
		packer->Run( res );
	}	
	return 0;
}
