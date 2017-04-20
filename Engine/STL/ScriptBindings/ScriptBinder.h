// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "ScriptEngine.h"

#ifdef GX_ANGELSCRIPT_ENABLED

namespace GX_STL
{
namespace GXScript
{

	namespace _script_hidden_
	{
		struct OperatorBinderHelper
		{
		public:
			enum EUnaryOperator
			{
				OP_INVERSE,
				OP_NOT,
				OP_PRE_INC,
				OP_PRE_DEC,
				OP_POST_INC,
				OP_POST_DEC
			};

			enum EBinaryOperator
			{
				OP_ADD,
				OP_SUB,
				OP_MUL,
				OP_DIV,
				OP_MOD,
				OP_AND,
				OP_OR,
				OP_XOR,
				OP_SHIFT_LEFT,
				OP_SHIFT_RIGHT
			};

		protected:
			static const char *  _UnaryToStr (EUnaryOperator op);
			static const char *  _BinToStr (EBinaryOperator op);
			static const char *  _BinAssignToStr (EBinaryOperator op);
			static const char *  _BinRightToStr (EBinaryOperator op);
		};

	}	// _script_hidden_



	//
	// Class Binder
	//
	
	template <typename T>
	struct ClassBinder
	{
	// types
	public:
		typedef ClassBinder<T>	Self;
		typedef T				type;

		//
		// Operator Binder (helper class)
		// see http://www.angelcode.com/angelscript/sdk/docs/manual/doc_script_class_ops.html
		//
		struct OperatorBinder : _script_hidden_::OperatorBinderHelper
		{
			friend struct ClassBinder<T>;

		// types
		public:
			typedef OperatorBinder		Self;


		// variables
		private:
			Ptr< ClassBinder<T> >		_binder;


		// methods
		private:
			OperatorBinder () {}
			OperatorBinder (ClassBinder<T> *ptr) : _binder(ptr) {}

		public:
			// unary
			template <typename OutType>	Self &	Inverse ();		// ~x
			template <typename OutType>	Self &	Not ();			// !x
			template <typename OutType>	Self &	PreInc ();		// ++x
			template <typename OutType>	Self &	PreDec ();		// --x
			template <typename OutType>	Self &	PostInc ();		// x++
			template <typename OutType>	Self &	PostDec ();		// x++
			
			template <typename OutType>	Self &	Unary (EUnaryOperator op, OutType (T::*) ());
			template <typename OutType>	Self &	Unary (EUnaryOperator op, OutType (T::*) () const);
			template <typename OutType>	Self &	Unary (EUnaryOperator op, OutType (*) (T& ));
			template <typename OutType>	Self &	Unary (EUnaryOperator op, OutType (*) (const T& ));

			// only for PostInc or PostDec
			template <typename OutType>	Self &	Unary (EUnaryOperator op, OutType (T::*) (int));


			// unary assign
			template <typename InType>	Self &	Assign ();				// x = y
			template <typename InType>	Self &	AddAssign ();			// x += y
			template <typename InType>	Self &	SubAssign ();			// x -= y
			template <typename InType>	Self &	MulAssign ();			// x *= y
			template <typename InType>	Self &	DivAssign ();			// x /= y
			template <typename InType>	Self &	ModAssign ();			// x %= y
			template <typename InType>	Self &	AndAssign ();			// x &= y
			template <typename InType>	Self &	OrAssign ();			// x |= y
			template <typename InType>	Self &	XorAssign ();			// x ^= y
			template <typename InType>	Self &	ShiftLeftAssign ();		// x <<= y
			template <typename InType>	Self &	ShiftRightAssign ();	// x >>= y

			template <typename InType>	Self &	BinaryAssign (EBinaryOperator op, T& (T::*) (InType));
			template <typename InType>	Self &	BinaryAssign (EBinaryOperator op, T& (*) (T&, InType));


			// binary
			template <typename InType, typename OutType>	Self &	Add ();			// x + y
			template <typename InType, typename OutType>	Self &	Sub ();			// x - y
			template <typename InType, typename OutType>	Self &	Mul ();			// x * y
			template <typename InType, typename OutType>	Self &	Div ();			// x / y
			template <typename InType, typename OutType>	Self &	Mod ();			// x % y
			template <typename InType, typename OutType>	Self &	And ();			// x & y
			template <typename InType, typename OutType>	Self &	Or ();			// x | y
			template <typename InType, typename OutType>	Self &	Xor ();			// x ^ y
			template <typename InType, typename OutType>	Self &	ShiftLeft ();	// x << y
			template <typename InType, typename OutType>	Self &	ShiftRight ();	// x >> y
			
			template <typename InType, typename OutType>	Self &	Binary (EBinaryOperator op, OutType (T::*) (InType));
			template <typename InType, typename OutType>	Self &	Binary (EBinaryOperator op, OutType (T::*) (InType) const);
			template <typename InType, typename OutType>	Self &	Binary (EBinaryOperator op, OutType (*) (T&, InType));
			template <typename InType, typename OutType>	Self &	Binary (EBinaryOperator op, OutType (*) (const T&, InType));


			// binary right
			//template <typename InType, typename OutType>	Self &	AddR ();		// y + x
			//template <typename InType, typename OutType>	Self &	SubR ();		// y - x
			//template <typename InType, typename OutType>	Self &	MulR ();		// y * x
			//template <typename InType, typename OutType>	Self &	DivR ();		// y / x
			//template <typename InType, typename OutType>	Self &	ModR ();		// y % x
			//template <typename InType, typename OutType>	Self &	AndR ();		// y & x
			//template <typename InType, typename OutType>	Self &	OrR ();			// y | x
			//template <typename InType, typename OutType>	Self &	XorR ();		// y ^ x
			//template <typename InType, typename OutType>	Self &	ShiftLeftR ();	// y << x
			//template <typename InType, typename OutType>	Self &	ShiftRightR ();	// y >> x
			
			template <typename InType, typename OutType>	Self &	BinaryRight (EBinaryOperator op, OutType (*) (InType, T&));
			template <typename InType, typename OutType>	Self &	BinaryRight (EBinaryOperator op, OutType (*) (InType, const T&));
			

			// index
			template <typename OutType, typename ...InTypes>	Self &	Index ();	// x[...]
			template <typename OutType, typename ...InTypes>	Self &	Index ( OutType (T::*) (InTypes...) );
			template <typename OutType, typename ...InTypes>	Self &	Index ( OutType (T::*) (InTypes...) const );
			template <typename OutType, typename ...InTypes>	Self &	Index ( OutType (*) (T&, InTypes...) );
			template <typename OutType, typename ...InTypes>	Self &	Index ( OutType (*) (const T&, InTypes...) );


			// call
			template <typename OutType, typename ...InTypes>	Self &	Call ( OutType (T::*) (InTypes...) );	// x(...)
			template <typename OutType, typename ...InTypes>	Self &	Call ( OutType (T::*) (InTypes...) const );
			template <typename OutType, typename ...InTypes>	Self &	Call ( OutType (*) (T&, InTypes...) );
			template <typename OutType, typename ...InTypes>	Self &	Call ( OutType (*) (const T&, InTypes...) );


			// convert
			template <typename OutType> Self &	Convert ();		// y(x)
			template <typename OutType> Self &	Convert ( OutType (T::*) () const );
			template <typename OutType> Self &	Convert ( OutType (*) (const T &) );


			// cast
			template <typename OutType> Self &	Cast ();		// y(x)
			template <typename OutType> Self &	Cast ( OutType& (T::*) () );
			template <typename OutType> Self &	Cast ( OutType const& (T::*) () const );
			template <typename OutType> Self &	Cast ( OutType& (*) (T &) );
			template <typename OutType> Self &	Cast ( OutType const& (*) (const T &) );


			// compare
			template <typename InType>	Self &	Equals ( bool (T::*) (InType) const );		// x == y
			template <typename InType>	Self &	Equals ( bool (*) (const T&, InType) );		// x == y

			template <typename InType>	Self &	Compare ( int (T::*) (InType) const );		// x <> y
			template <typename InType>	Self &	Compare ( int (*) (const T&, InType) );		// x <> y
		};


	// variables
	private:
		Ptr< AngelScript::asIScriptEngine >		_engine;
		String									_name;
		int										_flags;


	// methods
	public:
		explicit
		ClassBinder (AngelScript::asIScriptEngine *eng);

		explicit
		ClassBinder (AngelScript::asIScriptEngine *eng, StringCRef name);

		bool CreatePodValue (int flags = 0);
		bool CreateClassValue (int flags = 0);
		bool CreateRef (int flags = 0, bool hasFactory = true);
		bool CreateRef (T* (*create)(), void (T:: *addRef)(), void (T:: *releaseRef)(), int flags = 0);


		template <typename B>
		bool AddConstructor (B ctorPtr);

		template <typename B>
		bool AddProperty (B T::* value, StringCRef name);

		template <typename B>
		bool AddMethod (B methodPtr, StringCRef name);
		
		template <typename B>
		bool AddMethodFromGlobal (B funcPtr, StringCRef name);
		
		template <typename B>
		bool AddMethodFromGlobalObjFirst (B funcPtr, StringCRef name);
		
		template <typename B>
		bool AddMethodFromGlobalObjLast (B funcPtr, StringCRef name);

		OperatorBinder						Operators ()			{ return OperatorBinder( this ); }

		String const &						Name ()			const	{ return _name; }

		Ptr< AngelScript::asIScriptEngine >	GetEngine ()			{ return _engine; }


	private:
		bool _Create (int flags);
		
		struct _Util
		{
			template <typename TIn, typename TOut>
			static String Desc (StringCRef name)
			{
				String s;
				ScriptTypeInfo< TOut >::Name( s );		s << " " << name << "(";
				ScriptTypeInfo< TIn >::ArgName( s );	s << ") const";
				return s;
			}
		};
	};


	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline ClassBinder<T>::ClassBinder (AngelScript::asIScriptEngine *eng) :
		_engine(eng), _flags(0)
	{
		ScriptTypeInfo< T >::Name( _name );
	}

/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline ClassBinder<T>::ClassBinder (AngelScript::asIScriptEngine *eng, StringCRef name) :
		_engine(eng), _flags(0), _name(name)
	{}

/*
=================================================
	CreatePodValue
=================================================
*/
	template <typename T>
	inline bool ClassBinder<T>::CreatePodValue (int flags)
	{
		using namespace AngelScript;

		_flags = asOBJ_VALUE | asOBJ_POD | flags;
		AS_CALL_R( _engine->RegisterObjectType( _name.cstr(), sizeof(T),  ) );
		
		CHECK_ERR( _Create( _flags ) );
		return true;
	}
	
/*
=================================================
	CreateClassValue
=================================================
*/
	template <typename T>
	inline bool ClassBinder<T>::CreateClassValue (int flags)
	{
		using namespace AngelScript;

		_flags = asOBJ_VALUE | asOBJ_APP_CLASS_CDAK | flags;
		AS_CALL_R( _engine->RegisterObjectType( _name.cstr(), sizeof(T), _flags ) );

		CHECK_ERR( _Create( _flags ) );
		return true;
	}

/*
=================================================
	CreateRef
=================================================
*/
	template <typename T>
	inline bool ClassBinder<T>::CreateRef (int flags, const bool hasFactory)
	{
		typedef T * (* constructor_t) ();

		constructor_t create = null;

		if ( hasFactory )
			create = &AngelScriptHelper::FactoryCreate<T>;
	
		return CreateRef( create, &T::__AddRef, &T::__Release, flags );
	}
	
/*
=================================================
	CreateRef
=================================================
*/
	template <typename T>
	inline bool ClassBinder<T>::CreateRef (T* (*create)(), void (T:: *addRef)(), void (T:: *releaseRef)(), int flags)
	{
		using namespace AngelScript;

		_flags = asOBJ_REF | flags | (addRef != null and releaseRef != null ? 0 : asOBJ_NOCOUNT);

		AS_CALL_R( _engine->RegisterObjectType( _name.cstr(), sizeof(T), _flags ) );
		
		if ( addRef != null )
		{
			AS_CALL_R( _engine->RegisterObjectBehaviour( _name.cstr(), asBEHAVE_ADDREF,		"void AddRef()",
							asSMethodPtr<sizeof(void (T::*)())>::Convert((void (T::*)())(addRef)),	asCALL_THISCALL ) );
		}

		if ( releaseRef != null )
		{
			AS_CALL_R( _engine->RegisterObjectBehaviour( _name.cstr(), asBEHAVE_RELEASE,	"void Release()",
							asSMethodPtr<sizeof(void (T::*)())>::Convert((void (T::*)())(releaseRef)),	asCALL_THISCALL ) );
		}

		if ( create != null )
		{
			AS_CALL_R( _engine->RegisterObjectBehaviour( _name.cstr(), asBEHAVE_FACTORY,
														(String(_name) << "@ new_" << _name << "()").cstr(),
														asFUNCTION( create ),	asCALL_CDECL ) );
		}
		/*
		AS_CALL_R( _engine->RegisterObjectMethod( _name.cstr(),
										(String(_name) << " & opAssign(const " << _name << " &in)").cstr(),
										asMETHOD( T, operator = ), asCALL_THISCALL ) );
		*/
		return true;
	}

/*
=================================================
	_Create
=================================================
*/
	template <typename T>
	inline bool ClassBinder<T>::_Create (const int flags)
	{
		using namespace AngelScript;

		// constructor
		if ( EnumEq( flags, asOBJ_APP_CLASS_CONSTRUCTOR ) )
		{
			AS_CALL_R( _engine->RegisterObjectBehaviour( _name.cstr(), asBEHAVE_CONSTRUCT, "void f()",
											asFUNCTION( &AngelScriptHelper::Constructor<T> ), asCALL_GENERIC ) );
		}

		// destructor
		if ( EnumEq( flags, asOBJ_APP_CLASS_DESTRUCTOR ) )
		{
			AS_CALL_R( _engine->RegisterObjectBehaviour( _name.cstr(), asBEHAVE_DESTRUCT,  "void f()",
											asFUNCTION( &AngelScriptHelper::Destructor<T> ), asCALL_GENERIC ) );
		}

		// copy constructor
		if ( EnumEq( flags, asOBJ_APP_CLASS_COPY_CONSTRUCTOR ) )
		{
			AS_CALL_R( _engine->RegisterObjectBehaviour( _name.cstr(), asBEHAVE_CONSTRUCT,
											(String("void f(const ") << _name << " &in)").cstr(),
											asFUNCTION( &AngelScriptHelper::CopyConstructor<T> ), asCALL_GENERIC ) );
		}

		// assignment 
		if ( EnumEq( flags, asOBJ_APP_CLASS_ASSIGNMENT ) )
		{
			AS_CALL_R( _engine->RegisterObjectMethod( _name.cstr(),
											(String(_name) << " & opAssign(const " << _name << " &in)").cstr(),
											asMETHOD( T, operator = ), asCALL_THISCALL ) );
		}
		return true;
	}
	
/*
=================================================
	AddConstructor
=================================================
*/
	template <typename T>
	template <typename B>
	inline bool ClassBinder<T>::AddConstructor (B ctorPtr)
	{
		using namespace AngelScript;

		STATIC_ASSERT( (CompileTime::IsSameTypes< void *, GlobalFunction<B>::TypeList_t::Get<0> >) );

		String	s("void f ");
		GlobalFunction<B>::GetArgs( s, 1 );	// skip	(void *)

		AS_CALL_R( _engine->RegisterObjectBehaviour( _name.cstr(), asBEHAVE_CONSTRUCT,
										s.cstr(), asFUNCTION( *ctorPtr ), asCALL_CDECL_OBJFIRST ) );
		return true;
	}
	
/*
=================================================
	AddProperty
=================================================
*/
	template <typename T>
	template <typename B>
	inline bool ClassBinder<T>::AddProperty (B T::* value, StringCRef name)
	{
		String	s;
		ScriptTypeInfo<B>::Name( s );
		s << ' ' << name;

		AS_CALL_R( _engine->RegisterObjectProperty( _name.cstr(), s.cstr(), (int)offsetof( T, *value ) ) );
		return true;
	}
	
/*
=================================================
	AddMethod
=================================================
*/
	template <typename T>
	template <typename B>
	inline bool ClassBinder<T>::AddMethod (B methodPtr, StringCRef name)
	{
		using namespace AngelScript;

		String	s;
		MemberFunction<B>::GetDescriptor( s, name );

		AS_CALL_R( _engine->RegisterObjectMethod( _name.cstr(), s.cstr(),
							asSMethodPtr< sizeof( void (T::*)() ) >::Convert( (void (T::*)()) (methodPtr) ),
							asCALL_THISCALL ) );
		return true;
	}
	
/*
=================================================
	AddMethodFromGlobal
=================================================
*/
	template <typename T>
	template <typename B>
	inline bool ClassBinder<T>::AddMethodFromGlobal (B funcPtr, StringCRef name)
	{
		const bool obj_first =	(CompileTime::IsSameTypesWithoutQualifiers< T *, GlobalFunction<B>::TypeList_t::Front >()) or
								(CompileTime::IsSameTypesWithoutQualifiers< T &, GlobalFunction<B>::TypeList_t::Front >());
		
		const bool obj_last =	(CompileTime::IsSameTypesWithoutQualifiers< T *, GlobalFunction<B>::TypeList_t::Back >()) or
								(CompileTime::IsSameTypesWithoutQualifiers< T &, GlobalFunction<B>::TypeList_t::Back >());

		STATIC_ASSERT( obj_first or obj_last );

		if ( obj_first )
			return AddMethodFromGlobalObjFirst( funcPtr, name );
		else
			return AddMethodFromGlobalObjLast( funcPtr, name );
	}
	
/*
=================================================
	AddMethodFromGlobalObjFirst
=================================================
*/
	template <typename T>
	template <typename B>
	inline bool ClassBinder<T>::AddMethodFromGlobalObjFirst (B funcPtr, StringCRef name)
	{
		using namespace AngelScript;
		
		String	s;
		GlobalFunction<B>::GetDescriptor( s, name );
		
		// TODO: why?
		//STATIC_ASSERT(
		//	(CompileTime::IsSameTypesWithoutQualifiers< T *, GlobalFunction<B>::TypeList_t::Front >()) or
		//	(CompileTime::IsSameTypesWithoutQualifiers< T &, GlobalFunction<B>::TypeList_t::Front >())
		//);
		
		AS_CALL_R( _engine->RegisterObjectMethod( _name.cstr(), s.cstr(), asFUNCTION( *funcPtr ), asCALL_CDECL_OBJFIRST ) );
		return true;
	}
		
/*
=================================================
	AddMethodFromGlobalObjLast
=================================================
*/
	template <typename T>
	template <typename B>
	inline bool ClassBinder<T>::AddMethodFromGlobalObjLast (B funcPtr, StringCRef name)
	{
		using namespace AngelScript;
		
		String	s;
		GlobalFunction<B>::GetDescriptor( s, name );

		// TODO: why?
		//STATIC_ASSERT(
		//	(CompileTime::IsSameTypesWithoutQualifiers< T *, GlobalFunction<B>::TypeList_t::Back >()) or
		//	(CompileTime::IsSameTypesWithoutQualifiers< T &, GlobalFunction<B>::TypeList_t::Back >())
		//);
		
		AS_CALL_R( _engine->RegisterObjectMethod( _name.cstr(), s.cstr(), asFUNCTION( *funcPtr ), asCALL_CDECL_OBJLAST ) );
		return true;
	}

/*
=================================================
	Unary
=================================================
*/
	template <typename T>
	template <typename OutType>	
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Inverse ()
	{
		return Unary( OP_INVERSE, static_cast< OutType (T::*) () const >( &T::operator ~ ) );
	}
	
	template <typename T>
	template <typename OutType>	
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Not ()
	{
		return Unary( OP_NOT, static_cast< OutType (T::*) () const >( &T::operator ! ) );
	}
	
	template <typename T>
	template <typename OutType>	
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::PreInc ()
	{
		return Unary( OP_PRE_INC, static_cast< OutType (T::*) () >( &T::operator ++ ) );
	}
	
	template <typename T>
	template <typename OutType>	
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::PreDec ()
	{
		return Unary( OP_PRE_DEC, static_cast< OutType (T::*) () >( &T::operator -- ) );
	}
	
	template <typename T>
	template <typename OutType>	
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::PostInc ()
	{
		return Unary( OP_POST_INC, static_cast< OutType (T::*) (int) >( &T::operator ++ ) );
	}
	
	template <typename T>
	template <typename OutType>	
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::PostDec ()
	{
		return Unary( OP_POST_DEC, static_cast< OutType (T::*) (int) >( &T::operator -- ) );
	}
			
	template <typename T>
	template <typename OutType>	
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Unary (EUnaryOperator op, OutType (T::*func) ())
	{
		_binder->AddMethod( func, _UnaryToStr( op ) );
		return *this;
	}
	
	template <typename T>
	template <typename OutType>	
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Unary (EUnaryOperator op, OutType (T::*func) () const)
	{
		_binder->AddMethod( func, _UnaryToStr( op ) );
		return *this;
	}
	
	template <typename T>
	template <typename OutType>	
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Unary (EUnaryOperator op, OutType (*func) (T& ))
	{
		_binder->AddMethodFromGlobalObjFirst( func, _UnaryToStr( op ) );
		return *this;
	}
	
	template <typename T>
	template <typename OutType>	
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Unary (EUnaryOperator op, OutType (*func) (const T& ))
	{
		_binder->AddMethodFromGlobalObjFirst( func, _UnaryToStr( op ) );
		return *this;
	}
	
	template <typename T>
	template <typename OutType>	
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Unary (EUnaryOperator op, OutType (T::*func) (int))
	{
		ASSERT( op == OP_POST_INC or op == OP_POST_DEC );

		_binder->AddMethod( func, _UnaryToStr( op ) );
		return *this;
	}

/*
=================================================
	BinaryAssign
=================================================
*/
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Assign ()
	{
		_binder->AddMethod( static_cast< T& (T::*) (InType) >( &T::operator = ), "opAssign" );
		return *this;
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::AddAssign ()
	{
		return BinaryAssign( OP_ADD, static_cast< T& (T::*) (InType) >( &T::operator += ) );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::SubAssign ()
	{
		return BinaryAssign( OP_SUB, static_cast< T& (T::*) (InType) >( &T::operator -= ) );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::MulAssign ()
	{
		return BinaryAssign( OP_MUL, static_cast< T& (T::*) (InType) >( &T::operator *= ) );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::DivAssign ()
	{
		return BinaryAssign( OP_DIV, static_cast< T& (T::*) (InType) >( &T::operator /= ) );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::ModAssign ()
	{
		return BinaryAssign( OP_MOD, static_cast< T& (T::*) (InType) >( &T::operator %= ) );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::AndAssign ()
	{
		return BinaryAssign( OP_AND, static_cast< T& (T::*) (InType) >( &T::operator &= ) );
	}

	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::OrAssign ()
	{
		return BinaryAssign( OP_OR, static_cast< T& (T::*) (InType) >( &T::operator |= ) );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::XorAssign ()
	{
		return BinaryAssign( OP_XOR, static_cast< T& (T::*) (InType) >( &T::operator ^= ) );
	}

	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::ShiftLeftAssign ()
	{
		return BinaryAssign( OP_SHIFT_LEFT, static_cast< T& (T::*) (InType) >( &T::operator <<= ) );
	}

	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::ShiftRightAssign ()
	{
		return BinaryAssign( OP_SHIFT_RIGHT, static_cast< T& (T::*) (InType) >( &T::operator >>= ) );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::BinaryAssign (EBinaryOperator op, T& (T::*func) (InType))
	{
		_binder->AddMethod( func, _BinAssignToStr( op ) );
		return *this;
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::BinaryAssign (EBinaryOperator op, T& (*func) (T&, InType))
	{
		_binder->AddMethodFromGlobalObjFirst( func, _BinAssignToStr( op ) );
		return *this;
	}

/*
=================================================
	Inverse
=================================================
*/
	template <typename T>
	template <typename OutType, typename ...InTypes>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Index ()
	{
		if ( TypeTraits::IsConst<OutType>() )
			return Index( static_cast< OutType (T::*) (InTypes...) const >( &T::operator [] ) );
		else
			return Index( static_cast< OutType (T::*) (InTypes...) >( &T::operator [] ) );
	}
	
	template <typename T>
	template <typename OutType, typename ...InTypes>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Index ( OutType (T::*func) (InTypes...) )
	{
		_binder->AddMethod( func, "opIndex" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType, typename ...InTypes>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Index ( OutType (T::*func) (InTypes...) const )
	{
		_binder->AddMethod( func, "opIndex" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType, typename ...InTypes>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Index ( OutType (*func) (T&, InTypes...) )
	{
		_binder->AddMethodFromGlobalObjFirst( func, "opIndex" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType, typename ...InTypes>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Index ( OutType (*func) (const T&, InTypes...) )
	{
		_binder->AddMethodFromGlobalObjFirst( func, "opIndex" );
		return *this;
	}
	
/*
=================================================
	Call
=================================================
*/
	template <typename T>
	template <typename OutType, typename ...InTypes>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Call ( OutType (T::*func) (InTypes...) )
	{
		_binder->AddMethod( func, "opCall" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType, typename ...InTypes>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Call ( OutType (T::*func) (InTypes...) const )
	{
		_binder->AddMethod( func, "opCall" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType, typename ...InTypes>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Call ( OutType (*func) (T&, InTypes...) )
	{
		_binder->AddMethodFromGlobalObjFirst( func, "opCall" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType, typename ...InTypes>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Call ( OutType (*func) (const T&, InTypes...) )
	{
		_binder->AddMethodFromGlobalObjFirst( func, "opCall" );
		return *this;
	}

/*
=================================================
	Convert
=================================================
*/
	template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Convert ()
	{
		return Convert( static_cast< OutType (T::*) () const >() );
	}
	
	template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Convert ( OutType (T::*func) () const )
	{
		_binder->AddMethod( func, "opConv" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Convert ( OutType (*func) (const T &) )
	{
		_binder->AddMethodFromGlobalObjFirst( func, "opConv" );
		return *this;
	}
	
/*
=================================================
	Cast
=================================================
*/
	template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Cast ()
	{
		return Cast( static_cast< OutType const& (T::*) () const >( &T::operator OutType ) );
	}
	
	template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Cast ( OutType& (T::*func) () )
	{
		_binder->AddMethod( func, "opCast" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Cast ( OutType const& (T::*func) () const )
	{
		_binder->AddMethod( func, "opCast" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Cast ( OutType& (*func) (T &) )
	{
		_binder->AddMethodFromGlobalObjFirst( func, "opCast" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Cast ( OutType const& (*func) (const T &) )
	{
		_binder->AddMethodFromGlobalObjFirst( func, "opCast" );
		return *this;
	}
	
/*
=================================================
	Binary
=================================================
*/
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Add ()
	{
		return Binary( OP_ADD, static_cast< OutType (T::*) (InType) const >( &T::operator + ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Sub ()
	{
		return Binary( OP_SUB, static_cast< OutType (T::*) (InType) const >( &T::operator - ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Mul ()
	{
		return Binary( OP_MUL, static_cast< OutType (T::*) (InType) const >( &T::operator * ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Div ()
	{
		return Binary( OP_DIV, static_cast< OutType (T::*) (InType) const >( &T::operator / ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Mod ()
	{
		return Binary( OP_MOD, static_cast< OutType (T::*) (InType) const >( &T::operator % ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::And ()
	{
		return Binary( OP_AND, static_cast< OutType (T::*) (InType) const >( &T::operator & ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Or ()
	{
		return Binary( OP_OR, static_cast< OutType (T::*) (InType) const >( &T::operator | ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Xor ()
	{
		return Binary( OP_XOR, static_cast< OutType (T::*) (InType) const >( &T::operator ^ ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::ShiftLeft ()
	{
		return Binary( OP_SHIFT_LEFT, static_cast< OutType (T::*) (InType) const >( &T::operator << ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::ShiftRight ()
	{
		return Binary( OP_SHIFT_RIGHT, static_cast< OutType (T::*) (InType) const >( &T::operator >> ) );
	}
			
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Binary (EBinaryOperator op, OutType (T::*func) (InType))
	{
		_binder->AddMethod( func, _BinToStr( op ) );
		return *this;
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Binary (EBinaryOperator op, OutType (T::*func) (InType) const)
	{
		_binder->AddMethod( func, _BinToStr( op ) );
		return *this;
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Binary (EBinaryOperator op, OutType (*func) (T&, InType))	
	{
		_binder->AddMethodFromGlobalObjFirst( func, _BinToStr( op ) );
		return *this;
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Binary (EBinaryOperator op, OutType (*func) (const T&, InType))
	{
		_binder->AddMethodFromGlobalObjFirst( func, _BinToStr( op ) );
		return *this;
	}

/*
=================================================
	BinaryRight
=================================================
*/
	/*template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::AddR ()
	{
		return BinaryRight( OP_ADD, static_cast< OutType (*) (InType, const T&) >( &operator + ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::SubR ()
	{
		return BinaryRight( OP_SUB, static_cast< OutType (*) (InType, const T&) >( &operator - ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::MulR ()
	{
		return BinaryRight( OP_MUL, static_cast< OutType (*) (InType, const T&) >( &operator * ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::DivR ()
	{
		return BinaryRight( OP_DIV, static_cast< OutType (*) (InType, const T&) >( &operator / ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::ModR ()
	{
		return BinaryRight( OP_MOD, static_cast< OutType (*) (InType, const T&) >( &operator % ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::AndR ()
	{
		return BinaryRight( OP_AND, static_cast< OutType (*) (InType, const T&) >( &operator & ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::OrR ()
	{
		return BinaryRight( OP_OR, static_cast< OutType (*) (InType, const T&) >( &operator | ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::XorR ()
	{
		return BinaryRight( OP_XOR, static_cast< OutType (*) (InType, const T&) >( &operator ^ ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::ShiftLeftR ()
	{
		return BinaryRight( OP_SHIFT_LEFT, static_cast< OutType (*) (InType, const T&) >( &operator << ) );
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::ShiftRightR ()
	{
		return BinaryRight( OP_SHIFT_RIGHT, static_cast< OutType (*) (InType, const T&) >( &operator >> ) );
	}*/
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::BinaryRight (EBinaryOperator op, OutType (*func) (InType, T&))
	{
		_binder->AddMethodFromGlobalObjLast( func, _BinRightToStr( op ) );
		return *this;
	}
	
	template <typename T>
	template <typename InType, typename OutType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::BinaryRight (EBinaryOperator op, OutType (*func) (InType, const T&))
	{
		_binder->AddMethodFromGlobalObjLast( func, _BinRightToStr( op ) );
		return *this;
	}

/*
=================================================
	Equals
=================================================
*/
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Equals ( bool (T::*func) (InType) const )
	{
		_binder->AddMethod( func, "opEquals" );
		return *this;
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Equals ( bool (*func) (const T&, InType) )
	{
		_binder->AddMethodFromGlobalObjFirst( func, "opEquals" );
		return *this;
	}
	
/*
=================================================
	Compare
=================================================
*/
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Compare ( int (T::*func) (InType) const )
	{
		_binder->AddMethod( func, "opCmp" );
		return *this;
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder &  ClassBinder<T>::OperatorBinder::Compare ( int (*func) (const T&, InType) )
	{
		_binder->AddMethodFromGlobalObjFirst( func, "opCmp" );
		return *this;
	}


}	// GXScript
}	// GX_STL

#endif	// GX_ANGELSCRIPT_ENABLED
