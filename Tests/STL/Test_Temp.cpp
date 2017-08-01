// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Temporary tests.
*/

#include "Engine/STL/Engine.STL.h"
#include "Debug.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


template <typename T, typename Class, typename ...Args>
struct Property
{
private:
	using Self	= Property< T, Class, Args... >;


private:
	const ubyte		_offset;


public:
	Property ()
	{}

	Property (Class *classPtr) :
		_offset( ubyte( ReferenceCast<usize>(this) - ReferenceCast<usize>(classPtr) ) )
	{
		ASSERT( (void *)this > (void *)classPtr );
	}

	operator T () const
	{
		return T( Args::Get( _GetClass() )... );
	}


private:
	Property (const Self &) = delete;
	Property (Self &&) = delete;

	Self& operator = (const Self &) = delete;
	Self& operator = (Self &&) = delete;

	Class* _GetClass () const
	{
		return (Class *)( ((byte *)this) - _offset );
	}
};


template <typename Class, typename Field, Field (Class::*member)>
struct MemberToType
{
	static Field const&  Get (Class *classPtr)
	{
		return classPtr->*member;
	}
};


struct VecNode
{
	float	x;
	float	y;

	using Prop_XY	= Property< VecNode, VecNode,
								MemberToType< VecNode, decltype(VecNode::x), &VecNode::x >,
								MemberToType< VecNode, decltype(VecNode::y), &VecNode::y > >;
	union {
		Prop_XY		xy;
		Prop_XY		yx;
	};

	VecNode () :
		xy{ this }
	{}
};


extern void Test_Temp ()
{
	/*StaticArray<usize, 5>	fields;

	usize	k	= 4;

	for (uint bitfield = 0; bitfield <= 0xfffff; ++bitfield)
	{
		fields.Clear();

		bool	skip	= true;
		usize	sum		= 0;
		usize	i		= 0;
		usize	off		= 1;

		String	str;
		str << "Cast" << k << "( ";

		for (; i < fields.Count(); ++i)
		{
			fields[i]	= ((bitfield / off) % 5);
			off			*= 5;
			sum			+= fields[i];

			if ( fields[i] == 0 )
			{
				skip = not( (bitfield / off) == 0 and sum > 0 and sum <= 4 and (sum == k or i == 1) );
				break;
			}

			str << (i ? ", " : "") << fields[i];
		}

		usize sum2 = 0;
		FOR( j, fields ) {
			sum2 += fields[j];
		}

		if ( sum2 == 4*4 )
			break;

		if ( skip )
			continue;

		DEBUG_CONSOLE( (str << " )").cstr() );

		sum2 = 0;
	}

	WARNING( "!!!" );*/
}
