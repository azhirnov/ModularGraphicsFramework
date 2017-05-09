// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Temporary tests.
*/

#include "Engine/STL/Engine.STL.h"
#include "Debug.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;



/*
template <typename Class, typename Field, Field (Class::*member)>
struct Property
{
	const ubyte		_offset;

	Property (Class *ptr) :
		_offset( (usize const &)(this) - (usize const&)(ptr) )
	{}
};
	
#define PROPERTY( _name_, _field_ ) \
	Property< Self, decltype(Self::_field_), &Self::_field_ >	_name_ { this }


struct Test
{
	using Self = Test;

	int		i;

	PROPERTY( x, i );
};*/


extern void Test_Temp ()
{
	//Property< Test, decltype(Test::i), &Test::i >	t;


	WARNING( "!!!" );
}
