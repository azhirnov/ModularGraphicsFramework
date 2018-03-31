// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/ResourcePacker/Packer/ResourcePacker.h"

using namespace ResPack;

int main (int argc, char** argv)
{
	CHECK_ERR( argv > 0 );

	auto packer = ResPack::ResourcePacker::Instance();

	CHECK_ERR( packer->Initialize(), -1 );

	String	res_file;

	for (int i = 1; i < argc; ++i)
	{
		StringCRef	key = argv[i];
		StringCRef	value;

		if ( ++i < argc )
			value = argv[i];

		if ( key == "-R" )
		{
			res_file = value;
		}
		else
		{
			RETURN_ERR( "unsupported command arg: " << key, 1 );
		}
	}

	packer->Run( res_file );
	
	return 0;
}
