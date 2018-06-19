// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Script/Impl/ScriptEngine.h"
#include "Core/Script/Impl/ScriptTypes.h"
#include "Core/STL/CompileTime/FunctionInfo.h"

namespace GXScript
{

	enum class EUnaryOperator
	{
		Inverse,
		Not,
		PreInc,
		PreDec,
		PostInc,
		PostDec
	};

	enum class EBinaryOperator
	{
		Assign,
		Add,
		Sub,
		Mul,
		Div,
		Mod,
		And,
		Or,
		Xor,
		ShiftLeft,
		ShiftRight
	};


	namespace _script_hidden_
	{
		struct OperatorBinderHelper
		{
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
	struct ClassBinder final
	{
	// types
	public:
		using Self		= ClassBinder<T>;
		using Class_t	= T;
		

		//
		// Operator Binder (helper class)
		// see http://www.angelcode.com/angelscript/sdk/docs/manual/doc_script_class_ops.html
		//
		struct OperatorBinder final : _script_hidden_::OperatorBinderHelper
		{
			friend struct ClassBinder<T>;

		// types
		public:
			using Self = OperatorBinder;


		// variables
		private:
			Ptr< ClassBinder<T> >		_binder;


		// methods
		private:
			OperatorBinder () {}
			explicit OperatorBinder (ClassBinder<T> *ptr) : _binder(ptr) {}

			template <typename Ret> Self &	_UnaryMatcher (EUnaryOperator op, Ret (Class_t::*)());
			template <typename Ret> Self &	_UnaryMatcher (EUnaryOperator op, Ret (Class_t::*)() const);
			template <typename Ret> Self &	_UnaryMatcher (EUnaryOperator op, Ret (Class_t::*)(int));
			
			template <typename Ret, typename RArg> Self &	_BinaryAssignMatcher (EBinaryOperator op, Ret (Class_t::*)(const RArg));
			template <typename Ret, typename RArg> Self &	_BinaryAssignMatcher (EBinaryOperator op, Ret (Class_t::*)(const RArg &));

			template <typename Ret, typename RArg> Self &	_BinaryMatcherLH (EBinaryOperator op, Ret (Class_t::*)(const RArg));
			template <typename Ret, typename RArg> Self &	_BinaryMatcherLH (EBinaryOperator op, Ret (Class_t::*)(const RArg &));
			template <typename Ret, typename RArg> Self &	_BinaryMatcherLH (EBinaryOperator op, Ret (Class_t::*)(const RArg) const);
			template <typename Ret, typename RArg> Self &	_BinaryMatcherLH (EBinaryOperator op, Ret (Class_t::*)(const RArg &) const);
			
			template <typename Ret, typename LArg> Self &	_BinaryMatcherRH (EBinaryOperator op, Ret (*)(const LArg, const Class_t));
			template <typename Ret, typename LArg> Self &	_BinaryMatcherRH (EBinaryOperator op, Ret (*)(const LArg, const Class_t &));
			template <typename Ret, typename LArg> Self &	_BinaryMatcherRH (EBinaryOperator op, Ret (*)(const LArg &, const Class_t));
			template <typename Ret, typename LArg> Self &	_BinaryMatcherRH (EBinaryOperator op, Ret (*)(const LArg &, const Class_t &));

		public:
			// unary
			Self &	Inverse ();		// ~x
			Self &	Not ();			// !x
			Self &	PreInc ();		// ++x
			Self &	PreDec ();		// --x
			Self &	PostInc ();		// x++
			Self &	PostDec ();		// x++
			
			template <typename Func>	Self &	Unary (EUnaryOperator op, Func func);


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

			template <typename Func>	Self &	BinaryAssign (EBinaryOperator op, Func func);


			// binary
			template <typename InType>	Self &	Add ();			// x + y
			template <typename InType>	Self &	Sub ();			// x - y
			template <typename InType>	Self &	Mul ();			// x * y
			template <typename InType>	Self &	Div ();			// x / y
			template <typename InType>	Self &	Mod ();			// x % y
			template <typename InType>	Self &	And ();			// x & y
			template <typename InType>	Self &	Or ();			// x | y
			template <typename InType>	Self &	Xor ();			// x ^ y
			template <typename InType>	Self &	ShiftLeft ();	// x << y
			template <typename InType>	Self &	ShiftRight ();	// x >> y
			
			template <typename Func>	Self &	Binary (EBinaryOperator op, Func func);


			// binary right
			template <typename InType>	Self &	AddR ();		// y + x
			template <typename InType>	Self &	SubR ();		// y - x
			template <typename InType>	Self &	MulR ();		// y * x
			template <typename InType>	Self &	DivR ();		// y / x
			template <typename InType>	Self &	ModR ();		// y % x
			template <typename InType>	Self &	AndR ();		// y & x
			template <typename InType>	Self &	OrR ();			// y | x
			template <typename InType>	Self &	XorR ();		// y ^ x
			template <typename InType>	Self &	ShiftLeftR ();	// y << x
			template <typename InType>	Self &	ShiftRightR ();	// y >> x
			
			template <typename Func>	Self &	BinaryRH (EBinaryOperator op, Func func);
			

			// index
			template <typename OutType, typename ...InTypes>	Self &	Index ();	// x[...]
			template <typename Func>							Self &	Index (Func func);


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
			template <typename Func>	Self &	Equals (Func func);		// x == y
			template <typename Func>	Self &	Compare (Func func);	// x <> y
		};


	// variables
	private:
		ScriptEnginePtr		_engine;
		String				_name;
		int					_flags;


	// methods
	public:
		explicit ClassBinder (const ScriptEnginePtr &eng);
		explicit ClassBinder (const ScriptEnginePtr &eng, StringCRef name);
		

		bool CreatePodValue (int flags = 0);
		bool CreateClassValue (int flags = 0);
		bool CreateRef (int flags = 0, bool hasFactory = true);
		bool CreateRef (T* (*create)(), void (T:: *addRef)(), void (T:: *releaseRef)(), int flags = 0);


		template <typename Func>
		bool AddConstructor (Func ctorPtr);

		template <typename B>
		bool AddProperty (B T::* value, StringCRef name);

		template <typename B>
		bool AddProperty (const T &self, B &value, StringCRef name);

		template <typename Func>
		bool AddMethod (Func methodPtr, StringCRef name);
		
		template <typename Func>
		bool AddMethodFromGlobal (Func funcPtr, StringCRef name);
		
		template <typename Func>
		bool AddMethodFromGlobalObjFirst (Func funcPtr, StringCRef name);
		
		template <typename Func>
		bool AddMethodFromGlobalObjLast (Func funcPtr, StringCRef name);

		OperatorBinder						Operators ()			{ return OperatorBinder( this ); }

		String const &						Name ()			const	{ return _name; }

		const ScriptEnginePtr &				GetEngine ()	const	{ return _engine; }
		Ptr< AngelScript::asIScriptEngine >	GetASEngine ()			{ return _engine->Get(); }


	private:
		bool _Create (int flags);
		
		template <typename T1>
		struct _IsSame;

		struct _Util;
	};


/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline ClassBinder<T>::ClassBinder (const ScriptEnginePtr &eng) :
		_engine{ eng }, _flags{ 0 }
	{
		ScriptTypeInfo< T >::Name( OUT _name );
	}
	
	template <typename T>
	inline ClassBinder<T>::ClassBinder (const ScriptEnginePtr &eng, StringCRef name) :
		_engine{ eng }, _name{ name }, _flags{ 0 }
	{}
	
/*
=================================================
	_IsSame
=================================================
*/
	template <typename T>
	template <typename T1>
	struct ClassBinder<T>::_IsSame {
		static constexpr bool	value =		CompileTime::IsSameTypes< T *, T1 > or
											CompileTime::IsSameTypes< T &, T1 > or
											CompileTime::IsSameTypes< T const *, T1 > or
											CompileTime::IsSameTypes< T const &, T1 >;
	};
	
/*
=================================================
	_Util
=================================================
*/
	struct _Util
	{
		template <typename TIn, typename TOut>
		static String Desc (StringCRef name)
		{
			String signature;
			ScriptTypeInfo< TOut >::Name( OUT signature );		signature << " " << name << "(";
			ScriptTypeInfo< TIn >::ArgName( OUT signature );	signature << ") const";
			return signature;
		}
	};

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
		AS_CALL_R( GetASEngine()->RegisterObjectType( _name.cstr(), sizeof(T), _flags ) );
		
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
		AS_CALL_R( GetASEngine()->RegisterObjectType( _name.cstr(), sizeof(T), _flags ) );

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

		AS_CALL_R( GetASEngine()->RegisterObjectType( _name.cstr(), sizeof(T), _flags ) );
		
		if ( addRef != null )
		{
			AS_CALL_R( GetASEngine()->RegisterObjectBehaviour( _name.cstr(), asBEHAVE_ADDREF,		"void AddRef()",
							asSMethodPtr<sizeof(void (T::*)())>::Convert(Cast<void (T::*)()>(addRef)),	asCALL_THISCALL ) );
		}

		if ( releaseRef != null )
		{
			AS_CALL_R( GetASEngine()->RegisterObjectBehaviour( _name.cstr(), asBEHAVE_RELEASE,	"void Release()",
							asSMethodPtr<sizeof(void (T::*)())>::Convert(Cast<void (T::*)()>(releaseRef)),	asCALL_THISCALL ) );
		}

		if ( create != null )
		{
			AS_CALL_R( GetASEngine()->RegisterObjectBehaviour( _name.cstr(), asBEHAVE_FACTORY,
														(String(_name) << "@ new_" << _name << "()").cstr(),
														asFUNCTION( create ),	asCALL_CDECL ) );
		}
		/*
		AS_CALL_R( GetASEngine()->RegisterObjectMethod( _name.cstr(),
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
			AS_CALL_R( GetASEngine()->RegisterObjectBehaviour( _name.cstr(), asBEHAVE_CONSTRUCT, "void f()",
											asFUNCTION( &AngelScriptHelper::Constructor<T> ), asCALL_GENERIC ) );
		}

		// destructor
		if ( EnumEq( flags, asOBJ_APP_CLASS_DESTRUCTOR ) )
		{
			AS_CALL_R( GetASEngine()->RegisterObjectBehaviour( _name.cstr(), asBEHAVE_DESTRUCT,  "void f()",
											asFUNCTION( &AngelScriptHelper::Destructor<T> ), asCALL_GENERIC ) );
		}

		// copy constructor
		if ( EnumEq( flags, asOBJ_APP_CLASS_COPY_CONSTRUCTOR ) )
		{
			AS_CALL_R( GetASEngine()->RegisterObjectBehaviour( _name.cstr(), asBEHAVE_CONSTRUCT,
											(String("void f(const ") << _name << " &in)").cstr(),
											asFUNCTION( &AngelScriptHelper::CopyConstructor<T> ), asCALL_GENERIC ) );
		}

		// assignment 
		if ( EnumEq( flags, asOBJ_APP_CLASS_ASSIGNMENT ) )
		{
			AS_CALL_R( GetASEngine()->RegisterObjectMethod( _name.cstr(),
											(String(_name) << " & opAssign(const " << _name << " &in)").cstr(),
											asFUNCTION( &AngelScriptHelper::CopyAssign<T> ), asCALL_GENERIC ) );
		}
		return true;
	}
	
/*
=================================================
	AddConstructor
=================================================
*/
	template <typename T>
	template <typename Func>
	inline bool ClassBinder<T>::AddConstructor (Func ctorPtr)
	{
		using namespace AngelScript;

		STATIC_ASSERT(( CompileTime::IsSameTypes< void *, typename GlobalFunction<Func>::TypeList_t::template Get<0> > ));

		String	signature("void f ");
		GlobalFunction<Func>::GetArgs( OUT signature, 1 );	// skip	(void *)

		AS_CALL_R( GetASEngine()->RegisterObjectBehaviour( _name.cstr(), asBEHAVE_CONSTRUCT,
										signature.cstr(), asFUNCTION( *ctorPtr ), asCALL_CDECL_OBJFIRST ) );
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
		String	signature;
		ScriptTypeInfo<B>::Name( OUT signature );
		signature << ' ' << name;

		AS_CALL_R( GetASEngine()->RegisterObjectProperty( _name.cstr(), signature.cstr(), int(OffsetOf( value )) ) );
		return true;
	}
	
/*
=================================================
	AddProperty
=================================================
*/
	template <typename T>
	template <typename B>
	inline bool ClassBinder<T>::AddProperty (const T &self, B &value, StringCRef name)
	{
		String	signature;
		ScriptTypeInfo<B>::Name( OUT signature );
		signature << ' ' << name;

		const isize	offset = isize(&value) - isize(&self);
		CHECK_ERR( offset >= 0 and offset < isize(sizeof(T) - sizeof(B)) );

		AS_CALL_R( GetASEngine()->RegisterObjectProperty( _name.cstr(), signature.cstr(), int(offset) ) );
		return true;
	}

/*
=================================================
	AddMethod
=================================================
*/
	template <typename T>
	template <typename Func>
	inline bool ClassBinder<T>::AddMethod (Func methodPtr, StringCRef name)
	{
		using namespace AngelScript;

		String	signature;
		MemberFunction<Func>::GetDescriptor( OUT signature, name );

		AS_CALL_R( GetASEngine()->RegisterObjectMethod( _name.cstr(), signature.cstr(),
							asSMethodPtr< sizeof( void (T::*)() ) >::Convert( reinterpret_cast<void (T::*)()>(methodPtr) ),
							asCALL_THISCALL ) );
		return true;
	}
	
/*
=================================================
	AddMethodFromGlobal
=================================================
*/
	template <typename T>
	template <typename Func>
	inline bool ClassBinder<T>::AddMethodFromGlobal (Func funcPtr, StringCRef name)
	{
		constexpr bool	obj_first	= _IsSame< typename GlobalFunction<Func>::TypeList_t::Front >::value;
		constexpr bool	obj_last	= _IsSame< typename GlobalFunction<Func>::TypeList_t::Back >::value;

		STATIC_ASSERT( obj_first or obj_last );

		if_constexpr( obj_first )
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
	template <typename Func>
	inline bool ClassBinder<T>::AddMethodFromGlobalObjFirst (Func funcPtr, StringCRef name)
	{
		using namespace AngelScript;
		
		using FuncInfo	= CompileTime::FunctionInfo<Func>;
		using FrontArg	= typename FuncInfo::args::Front;

		STATIC_ASSERT( _IsSame< FrontArg >::value );

		String	signature;
		GlobalFunction<Func>::GetDescriptor( OUT signature, name, 1, 0 );

		if_constexpr( TypeTraits::IsConst< FrontArg > or TypeTraits::IsConst<TypeTraits::RemoveQualifier< FrontArg >> )
		{
			signature << " const";
		}

		AS_CALL_R( GetASEngine()->RegisterObjectMethod( _name.cstr(), signature.cstr(), asFUNCTION( *funcPtr ), asCALL_CDECL_OBJFIRST ) );
		return true;
	}
		
/*
=================================================
	AddMethodFromGlobalObjLast
=================================================
*/
	template <typename T>
	template <typename Func>
	inline bool ClassBinder<T>::AddMethodFromGlobalObjLast (Func funcPtr, StringCRef name)
	{
		using namespace AngelScript;
		
		using FuncInfo	= CompileTime::FunctionInfo<Func>;
		using BackArg	= typename FuncInfo::args::Back;

		STATIC_ASSERT( _IsSame< BackArg >::value );

		String	signature;
		GlobalFunction<Func>::GetDescriptor( OUT signature, name, 0, 1 );
		
		if_constexpr( TypeTraits::IsConst< BackArg > or TypeTraits::IsConst<TypeTraits::RemoveQualifier< BackArg >> )
		{
			signature << " const";
		}

		AS_CALL_R( GetASEngine()->RegisterObjectMethod( _name.cstr(), signature.cstr(), asFUNCTION( *funcPtr ), asCALL_CDECL_OBJLAST ) );
		return true;
	}
	
/*
=================================================
	_UnaryMatcher
=================================================
*/
	template <typename T> template <typename Ret>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::_UnaryMatcher (EUnaryOperator op, Ret (Class_t::*func)()) { return Unary( op, func ); }
	
	template <typename T> template <typename Ret>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::_UnaryMatcher (EUnaryOperator op, Ret (Class_t::*func)() const) { return Unary( op, func ); }
	
	template <typename T> template <typename Ret>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::_UnaryMatcher (EUnaryOperator op, Ret (Class_t::*func)(int)) { return Unary( op, func ); }

/*
=================================================
	Unary
=================================================
*/
	template <typename T>
	template <typename Func>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Unary (EUnaryOperator op, Func func)
	{
		if_constexpr( CompileTime::IsVoid< typename CompileTime::FunctionInfo<Func>::clazz > )
		{
			_binder->AddMethodFromGlobalObjFirst( func, _UnaryToStr( op ) );
		} else {
			_binder->AddMethod( func, _UnaryToStr( op ) );
		}
		return *this;
	}

	template <typename T>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Inverse ()
	{
		return _UnaryMatcher< decltype(T().operator ~()) >( EUnaryOperator::Inverse, &T::operator ~ );
	}
	
	template <typename T>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Not ()
	{
		return _UnaryMatcher< decltype(T().operator !()) >( EUnaryOperator::Not, &T::operator ! );
	}
	
	template <typename T>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::PreInc ()
	{
		return _UnaryMatcher< decltype(T().operator ++()) >( EUnaryOperator::PreInc, &T::operator ++ );
	}
	
	template <typename T>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::PreDec ()
	{
		return _UnaryMatcher< decltype(T().operator --()) >( EUnaryOperator::PreDec, &T::operator -- );
	}
	
	template <typename T>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::PostInc ()
	{
		return _UnaryMatcher< decltype(T().operator ++(int())) >( EUnaryOperator::PostInc, &T::operator ++ );
	}
	
	template <typename T>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::PostDec ()
	{
		return _UnaryMatcher< decltype(T().operator --(int())) >( EUnaryOperator::PostDec, &T::operator -- );
	}
	
/*
=================================================
	_BinaryAssignMatcher
=================================================
*/
	template <typename T> template <typename Ret, typename RArg>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::_BinaryAssignMatcher (EBinaryOperator op, Ret (Class_t::*func)(const RArg)) { return BinaryAssign( op, func ); }

	template <typename T> template <typename Ret, typename RArg>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::_BinaryAssignMatcher (EBinaryOperator op, Ret (Class_t::*func)(const RArg &)) { return BinaryAssign( op, func ); }

/*
=================================================
	BinaryAssign
=================================================
*/
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Assign ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator = (CInType()) );
		using Func		= Result (Class_t::*) (const InType &);

		_binder->AddMethod( Cast< Func >( &T::operator = ), "opAssign" );
		return *this;
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::AddAssign ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator += (CInType()) );

		return _BinaryAssignMatcher< Result, InType >( EBinaryOperator::Add, &T::operator += );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::SubAssign ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator -= (CInType()) );

		return _BinaryAssignMatcher< Result, InType >( EBinaryOperator::Sub, &T::operator -= );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::MulAssign ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator *= (CInType()) );

		return _BinaryAssignMatcher< Result, InType >( EBinaryOperator::Mul, &T::operator *= );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::DivAssign ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator /= (CInType()) );

		return _BinaryAssignMatcher< Result, InType >( EBinaryOperator::Div, &T::operator /= );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::ModAssign ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator %= (CInType()) );

		return _BinaryAssignMatcher< Result, InType >( EBinaryOperator::Mod, &T::operator %= );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::AndAssign ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator &= (CInType()) );

		return _BinaryAssignMatcher< Result, InType >( EBinaryOperator::And, &T::operator &= );
	}

	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::OrAssign ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator |= (CInType()) );
		
		return _BinaryAssignMatcher< Result, InType >( EBinaryOperator::Or, &T::operator |= );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::XorAssign ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator ^= (CInType()) );
		
		return _BinaryAssignMatcher< Result, InType >( EBinaryOperator::Xor, &T::operator ^= );
	}

	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::ShiftLeftAssign ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator <<= (CInType()) );
		
		return _BinaryAssignMatcher< Result, InType >( EBinaryOperator::ShiftLeft, &T::operator <<= );
	}

	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::ShiftRightAssign ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator >>= (CInType()) );
		
		return _BinaryAssignMatcher< Result, InType >( EBinaryOperator::ShiftRight, &T::operator >>= );
	}
	
	template <typename T>
	template <typename Func>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::BinaryAssign (EBinaryOperator op, Func func)
	{
		if_constexpr( CompileTime::IsVoid< typename CompileTime::FunctionInfo<Func>::clazz > )
		{
			_binder->AddMethodFromGlobalObjFirst( func, _BinAssignToStr( op ) );
		} else {
			_binder->AddMethod( func, _BinAssignToStr( op ) );
		}
		return *this;
	}

/*
=================================================
	Index
=================================================
*/
	template <typename T>
	template <typename OutType, typename ...InTypes>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Index ()
	{
		if_constexpr( TypeTraits::IsConst<OutType>() )
			return Index( Cast< OutType (T::*) (InTypes...) const >( &T::operator [] ) );
		else
			return Index( Cast< OutType (T::*) (InTypes...) >( &T::operator [] ) );
	}
	
	template <typename T>
	template <typename Func>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Index (Func func)
	{
		if_constexpr( CompileTime::IsVoid< typename CompileTime::FunctionInfo<Func>::clazz > )
		{
			_binder->AddMethodFromGlobalObjFirst( func, "opIndex" );
		} else {
			_binder->AddMethod( func, "opIndex" );
		}
		return *this;
	}
	
/*
=================================================
	Call
=================================================
*/
	template <typename T>
	template <typename OutType, typename ...InTypes>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Call ( OutType (T::*func) (InTypes...) )
	{
		_binder->AddMethod( func, "opCall" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType, typename ...InTypes>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Call ( OutType (T::*func) (InTypes...) const )
	{
		_binder->AddMethod( func, "opCall" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType, typename ...InTypes>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Call ( OutType (*func) (T&, InTypes...) )
	{
		_binder->AddMethodFromGlobalObjFirst( func, "opCall" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType, typename ...InTypes>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Call ( OutType (*func) (const T&, InTypes...) )
	{
		_binder->AddMethodFromGlobalObjFirst( func, "opCall" );
		return *this;
	}

/*
=================================================
	Convert
=================================================
*/
	/*template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Convert ()
	{
		return Convert( Cast< OutType (T::*) () const >() );
	}*/
	
	template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Convert ( OutType (T::*func) () const )
	{
		_binder->AddMethod( func, "opConv" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Convert ( OutType (*func) (const T &) )
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
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Cast ()
	{
		return Cast( Cast< OutType const& (T::*) () const >( &T::operator OutType ) );
	}
	
	template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Cast ( OutType& (T::*func) () )
	{
		_binder->AddMethod( func, "opCast" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Cast ( OutType const& (T::*func) () const )
	{
		_binder->AddMethod( func, "opCast" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Cast ( OutType& (*func) (T &) )
	{
		_binder->AddMethodFromGlobalObjFirst( func, "opCast" );
		return *this;
	}
	
	template <typename T>
	template <typename OutType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Cast ( OutType const& (*func) (const T &) )
	{
		_binder->AddMethodFromGlobalObjFirst( func, "opCast" );
		return *this;
	}

/*
=================================================
	_BinaryMatcherLH
=================================================
*/
	template <typename T> template <typename Ret, typename RArg>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::_BinaryMatcherLH (EBinaryOperator op, Ret (Class_t::*func)(const RArg)) { return Binary( op, func ); }
	
	template <typename T> template <typename Ret, typename RArg>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::_BinaryMatcherLH (EBinaryOperator op, Ret (Class_t::*func)(const RArg &)) { return Binary( op, func ); }
	
	template <typename T> template <typename Ret, typename RArg>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::_BinaryMatcherLH (EBinaryOperator op, Ret (Class_t::*func)(const RArg) const) { return Binary( op, func ); }
	
	template <typename T> template <typename Ret, typename RArg>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::_BinaryMatcherLH (EBinaryOperator op, Ret (Class_t::*func)(const RArg &) const) { return Binary( op, func ); }

	
/*
=================================================
	Binary
=================================================
*/
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Add ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator + (CInType()) );

		return _BinaryMatcherLH< Result, InType >( EBinaryOperator::Add, &T::operator + );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Sub ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator - (CInType()) );

		return _BinaryMatcherLH< Result, InType >( EBinaryOperator::Sub, &T::operator - );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Mul ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator * (CInType()) );

		return _BinaryMatcherLH< Result, InType >( EBinaryOperator::Mul, &T::operator * );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Div ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator / (CInType()) );

		return _BinaryMatcherLH< Result, InType >( EBinaryOperator::Div, &T::operator / );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Mod ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator % (CInType()) );

		return _BinaryMatcherLH< Result, InType >( EBinaryOperator::Mod, &T::operator % );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::And ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator & (CInType()) );

		return _BinaryMatcherLH< Result, InType >( EBinaryOperator::And, &T::operator & );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Or ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator | (CInType()) );

		return _BinaryMatcherLH< Result, InType >( EBinaryOperator::Or, &T::operator | );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Xor ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator ^ (CInType()) );

		return _BinaryMatcherLH< Result, InType >( EBinaryOperator::Xor, &T::operator ^ );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::ShiftLeft ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator << (CInType()) );

		return _BinaryMatcherLH< Result, InType >( EBinaryOperator::ShiftLeft, &T::operator << );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::ShiftRight ()
	{
		using CInType	= const InType;
		using Result	= decltype( T().operator >> (CInType()) );

		return _BinaryMatcherLH< Result, InType >( EBinaryOperator::ShiftRight, &T::operator >> );
	}
	
	template <typename T>
	template <typename Func>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Binary (EBinaryOperator op, Func func)
	{
		if_constexpr( CompileTime::IsVoid< typename CompileTime::FunctionInfo<Func>::clazz > )
		{
			_binder->AddMethodFromGlobalObjFirst( func, _BinToStr( op ) );
		} else {
			_binder->AddMethod( func, _BinToStr( op ) );
		}
		return *this;
	}

/*
=================================================
	_BinaryMatcherRH
=================================================
*/
	template <typename T> template <typename Ret, typename LArg>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::_BinaryMatcherRH (EBinaryOperator op, Ret (*func)(const LArg, const Class_t)) { return BinaryRH( op, func ); }
	
	template <typename T> template <typename Ret, typename LArg>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::_BinaryMatcherRH (EBinaryOperator op, Ret (*func)(const LArg, const Class_t &)) { return BinaryRH( op, func ); }
	
	template <typename T> template <typename Ret, typename LArg>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::_BinaryMatcherRH (EBinaryOperator op, Ret (*func)(const LArg &, const Class_t)) { return BinaryRH( op, func ); }
	
	template <typename T> template <typename Ret, typename LArg>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::_BinaryMatcherRH (EBinaryOperator op, Ret (*func)(const LArg &, const Class_t &)) { return BinaryRH( op, func ); }

/*
=================================================
	BinaryRH
=================================================
*/
	/*template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::AddR ()
	{
		using CLeft		= const InType;
		using CRight	= const T;
		using Result	= decltype( CLeft() + CRight() );

		return _BinaryMatcherRH< Result, InType >( EBinaryOperator::Add, &operator + );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::SubR ()
	{
		using CLeft		= const InType;
		using CRight	= const T;
		using Result	= decltype( CLeft() - CRight() );

		return _BinaryMatcherRH< Result, InType >( EBinaryOperator::Sub, &operator - );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::MulR ()
	{
		using CLeft		= const InType;
		using CRight	= const T;
		using Result	= decltype( CLeft() * CRight() );

		return _BinaryMatcherRH< Result, InType >( EBinaryOperator::Mul, &operator * );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::DivR ()
	{
		using CLeft		= const InType;
		using CRight	= const T;
		using Result	= decltype( CLeft() / CRight() );

		return _BinaryMatcherRH< Result, InType >( EBinaryOperator::Div, &operator / );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::ModR ()
	{
		using CLeft		= const InType;
		using CRight	= const T;
		using Result	= decltype( CLeft() % CRight() );

		return _BinaryMatcherRH< Result, InType >( EBinaryOperator::Mod, &operator % );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::AndR ()
	{
		using CLeft		= const InType;
		using CRight	= const T;
		using Result	= decltype( CLeft() & CRight() );

		return _BinaryMatcherRH< Result, InType >( EBinaryOperator::And, &operator & );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::OrR ()
	{
		using CLeft		= const InType;
		using CRight	= const T;
		using Result	= decltype( CLeft() | CRight() );

		return _BinaryMatcherRH< Result, InType >( EBinaryOperator::Or, &operator | );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::XorR ()
	{
		using CLeft		= const InType;
		using CRight	= const T;
		using Result	= decltype( CLeft() ^ CRight() );

		return _BinaryMatcherRH< Result, InType >( EBinaryOperator::Xor, &operator ^ );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::ShiftLeftR ()
	{
		using CLeft		= const InType;
		using CRight	= const T;
		using Result	= decltype( CLeft() << CRight() );

		return _BinaryMatcherRH< Result, InType >( EBinaryOperator::ShiftLeft, &operator << );
	}
	
	template <typename T>
	template <typename InType>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::ShiftRightR ()
	{
		using CLeft		= const InType;
		using CRight	= const T;
		using Result	= decltype( CLeft() >> CRight() );

		return _BinaryMatcherRH< Result, InType >( EBinaryOperator::ShiftRight, &operator >> );
	}*/
	
	template <typename T>
	template <typename Func>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::BinaryRH (EBinaryOperator op, Func func)
	{
		STATIC_ASSERT( CompileTime::IsVoid< typename CompileTime::FunctionInfo<Func>::clazz > );
		
		_binder->AddMethodFromGlobalObjLast( func, _BinRightToStr( op ) );
		return *this;
	}

/*
=================================================
	Equals
=================================================
*/
	template <typename T>
	template <typename Func>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Equals (Func func)
	{
		using FuncInfo = CompileTime::FunctionInfo<Func>;

		STATIC_ASSERT( CompileTime::IsSameTypes< typename FuncInfo::result, bool > );
		STATIC_ASSERT( FuncInfo::args::Count == 2 );
		STATIC_ASSERT(( CompileTime::IsSameTypes< typename FuncInfo::args::template Get<0>, T > or CompileTime::IsSameTypes< typename FuncInfo::args::template Get<0>, const T& > ));
		STATIC_ASSERT(( CompileTime::IsSameTypes< typename FuncInfo::args::template Get<1>, T > or CompileTime::IsSameTypes< typename FuncInfo::args::template Get<1>, const T& > ));
		
		if_constexpr( CompileTime::IsVoid< typename FuncInfo::clazz > )
		{
			_binder->AddMethodFromGlobalObjFirst( func, "opEquals" );
		} else {
			_binder->AddMethod( func, "opEquals" );
		}
		return *this;
	}
	
/*
=================================================
	Compare
=================================================
*/
	template <typename T>
	template <typename Func>
	inline typename ClassBinder<T>::OperatorBinder&  ClassBinder<T>::OperatorBinder::Compare (Func func)
	{
		using FuncInfo = CompileTime::FunctionInfo<Func>;

		STATIC_ASSERT( CompileTime::IsSameTypes< typename FuncInfo::result, int > );
		STATIC_ASSERT( FuncInfo::args::Count == 2 );
		STATIC_ASSERT(( CompileTime::IsSameTypes< typename FuncInfo::args::template Get<0>, T > or CompileTime::IsSameTypes< typename FuncInfo::args::template Get<0>, const T& > ));
		STATIC_ASSERT(( CompileTime::IsSameTypes< typename FuncInfo::args::template Get<1>, T > or CompileTime::IsSameTypes< typename FuncInfo::args::template Get<1>, const T& > ));
		
		if_constexpr( CompileTime::IsVoid< typename FuncInfo::clazz > )
		{
			_binder->AddMethodFromGlobalObjFirst( func, "opCmp" );
		} else {
			_binder->AddMethod( func, "opCmp" );
		}
		return *this;
	}


}	// GXScript
