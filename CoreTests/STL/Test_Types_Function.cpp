// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


struct Test1
{
	typedef Delegate< void () >		delegate_t;
	typedef Event< delegate_t >		event_t;

	static void Func ()
	{
		DEBUG_CONSOLE( GX_FUNCTION_NAME );
	}

	static void Test ()
	{
		delegate_t	d = DelegateBuilder( Func );
		event_t		e;

		e.Add( d );
		e.Add( d );
		e.Call();

		d.Call();
	}
};

struct Test2
{
	typedef Delegate< void (int) >		delegate_t;
	typedef Event< delegate_t >			event_t;

	static void Func (int i)
	{
		DEBUG_CONSOLE( (String(GX_FUNCTION_NAME) << " " << i).cstr() );
	}

	static void Test ()
	{
		delegate_t	d = DelegateBuilder( Func );
		event_t		e;

		e.Add( d );
		e.Add( d );
		e.Call( 3 );

		d.Call( 1 );

		d.SafeCall( 4 );
	}
};

struct Test3
{
	typedef Delegate< void () >		delegate_t;
	typedef Event< delegate_t >		event_t;

	void Func ()
	{
		DEBUG_CONSOLE( GX_FUNCTION_NAME );
	}

	static void Test ()
	{
		Test3 t;
		delegate_t	d = DelegateBuilder( &t, &Test3::Func );
		event_t		e;

		e.Add( d );
		e.Add( d );
		e.Call();

		d.Call();
	}
};

struct Test4
{
	typedef Delegate< void (const float &) >	delegate_t;
	typedef Event< delegate_t >					event_t;

	void Func (const float &f) const
	{
		DEBUG_CONSOLE( (String(GX_FUNCTION_NAME) << " " << f).cstr() );
	}

	static void Test ()
	{
		Test4 t;
		delegate_t	d  = DelegateBuilder( &t, &Test4::Func );
		event_t		e;

		e.Add( d );
		e.Add( d );
		e.Call( 3.0f );

		d.Call( 1.0f );
	}
};


class Test5 : public RefCountedObject<>
{
public:
	typedef Delegate< void (int, int) >		delegate_t;
	typedef Event< delegate_t >				event_t;

	SHARED_POINTER( Test5 );

	~Test5 () { DEBUG_CONSOLE( GX_FUNCTION_NAME ); }

	void Func (int a, int b) const
	{
		DEBUG_CONSOLE( (String(GX_FUNCTION_NAME) << " " << a << ", " << b).cstr() );
	}

	static void Test ()
	{
		delegate_t	d;
		event_t		e;

		{
			const Test5Ptr t = new Test5();

			d = DelegateBuilder( t, &Test5::Func );
			
			e.Add( d );
			e.Add( d );
			TEST( e.Count() == 1 );
		}

		e.Call( 3, 4 );

		d.Call( 5, 6 );
	}
};


class Test6 : public RefCountedObject<>
{
public:
	typedef Delegate< void (int, int) >		delegate_t;
	typedef Event< delegate_t >				event_t;
	
	SHARED_POINTER( Test6 );
	
	void Func (int a, int b) const
	{
		DEBUG_CONSOLE( (String(GX_FUNCTION_NAME) << " " << a << ", " << b).cstr() );
	}

	static void Test ()
	{
		delegate_t	d;
		event_t		e;

		Test6Ptr t = new Test6();
		
		d = DelegateBuilder( t, &Test6::Func );

		e.Add( d );
		e.RemoveAllFor( t.ptr() );
		TEST( e.Count() == 0 );

		e.Add( d );
		e.RemoveAllFor( t );
		TEST( e.Count() == 0 );
	}
};


extern void Test_Types_Function ()
{
	Test1::Test();
	Test2::Test();
	Test3::Test();
	Test4::Test();
	Test5::Test();
	Test6::Test();
}
