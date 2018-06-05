// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Script/Impl/ScriptEngine.h"

namespace GX_STL
{
namespace GXScript
{

	//
	// Script Type Info
	//

	template <typename T>
	struct ScriptTypeInfo
	{
		typedef T	type;

		static void Name (INOUT String &);
		static void ArgName (INOUT String &s);
		static uint SizeOf ();
	};

#	define GX_DECL_SCRIPT_TYPE( _type_, _name_ ) \
		template <> \
		struct ScriptTypeInfo < _type_ > \
		{ \
			typedef _type_	type; \
			\
			static void Name (INOUT String &s)		{ s << (_name_); } \
			static void ArgName (INOUT String &s)	{ s << (_name_); } \
			static uint SizeOf ()					{ return sizeof(type); } \
		}

#	define DECL_SCRIPT_TYPE( _type_ )	GX_DECL_SCRIPT_TYPE( _type_, TOSTRING( _type_ ) )

#	define GX_DECL_SCRIPT_WRAP( _templ_, _buildName_, _buildArg_ ) \
		template <typename T> \
		struct ScriptTypeInfo < _templ_ > \
		{ \
			typedef _templ_				type; \
			typedef ScriptTypeInfo<T>	Base_t; \
			\
			static void Name (INOUT String &s)		{ _buildName_; } \
			static void ArgName (INOUT String &s)	{ _buildArg_; } \
			static uint SizeOf ()					{ return sizeof(type); } \
		}
	/*
#	define GX_DECL_SCRIPT_TYPE_EXT( _type_, _name_, _refName_, _refConstName_ ) \
		GX_DECL_SCRIPT_TYPE( _type_, _name_ ); \
		GX_DECL_SCRIPT_WRAP( const _type_ &,	ARGS( Base_t::Name( s );  s << ' ' << _refConstName_ ) ); \
		GX_DECL_SCRIPT_WRAP( _type_ &,			ARGS( Base_t::Name( s );  s << ' ' << _refName_		 ) ); \
		GX_DECL_SCRIPT_WRAP( const _type_ *,	ARGS( Base_t::Name( s );  s << ' ' << _refConstName_ ) ); \
		GX_DECL_SCRIPT_WRAP( _type_ *,			ARGS( Base_t::Name( s );  s << ' ' << _refName_		 ) );
	*/


	template <>
	struct ScriptTypeInfo <void>
	{
		typedef void	type;

		static void Name (INOUT String &s)		{ s << "void"; }
		static void ArgName (INOUT String &s)	{ s << "void"; }
		static uint SizeOf ()					{ return 0; }
	};

	DECL_SCRIPT_TYPE( bool );
	DECL_SCRIPT_TYPE( float );
	DECL_SCRIPT_TYPE( double );
	DECL_SCRIPT_TYPE( int );
	DECL_SCRIPT_TYPE( uint );
	GX_DECL_SCRIPT_TYPE( byte,			"int8" );
	GX_DECL_SCRIPT_TYPE( ubyte,			"uint8" );
	GX_DECL_SCRIPT_TYPE( short,			"int16" );
	GX_DECL_SCRIPT_TYPE( ushort,		"uint16" );
	GX_DECL_SCRIPT_TYPE( ilong,			"int64" );
	GX_DECL_SCRIPT_TYPE( ulong,			"uint64" );
	GX_DECL_SCRIPT_TYPE( std::string,	"string" );

#	undef DECL_SCRIPT_TYPE

	// only 'in' and 'out' are supported
	GX_DECL_SCRIPT_WRAP( const T,
						ARGS(
							s << "const ";
							Base_t::Name( s );
						),
						ARGS(
							s << "const ";
							Base_t::Name( s );
						));

	GX_DECL_SCRIPT_WRAP( const T &,
						ARGS(
							s << "const ";
							Base_t::Name( s );
							s << " &";
						),
						ARGS(
							s << "const ";
							Base_t::Name( s );
							s << " &in";
						));
		
	GX_DECL_SCRIPT_WRAP( T &,
						ARGS(
							Base_t::Name( s );
							s << " &";
						),
						ARGS(
							Base_t::Name( s );
							s << " &out";
						));
		
	GX_DECL_SCRIPT_WRAP( const T *,
						ARGS(
							s << "const ";
							Base_t::Name( s );
							s << " &";
						),
						ARGS(
							s << "const ";
							Base_t::Name( s );
							s << " &in";
						));
		
	GX_DECL_SCRIPT_WRAP( T *,
						ARGS(
							Base_t::Name( s );
							s << " &";
						),
						ARGS(
							Base_t::Name( s );
							s << " &out";
						));




	//
	// Angel Script Helper
	//

	struct AngelScriptHelper final : Noninstancable
	{
		struct SimpleRefCounter
		{
		private:
			int		_counter;

		private:
			SimpleRefCounter (const SimpleRefCounter &) {}

			void operator = (const SimpleRefCounter &) {}

		public:
			SimpleRefCounter () : _counter(1) {}
		
			void __AddRef ()
			{
				_counter++;
			}

			void __Release() 
			{ 
				if ( (--_counter) == 0 ) {
					delete this;
				}
			} 
		};

		template <typename T>
		static T * FactoryCreate ()
		{
			return new T();
		}


		template <typename T>
		static void Print (T *)
		{
			TODO("Print");
		}


		template <typename T>
		static void Constructor (AngelScript::asIScriptGeneric *gen)
		{
			new( gen->GetObject() ) T();
		}

		
		template <typename T>
		static void CopyConstructor (AngelScript::asIScriptGeneric *gen)
		{
			T const*	src = Cast<const T *>( gen->GetArgObject(0) );
			void *		dst = gen->GetObject();
			new( dst ) T( *src );
		}
		

		template <typename T>
		static void Destructor (AngelScript::asIScriptGeneric *gen)
		{
			static_cast<T *>(gen->GetObject())->~T();
		}


		template <typename T>
		static void CopyAssign (AngelScript::asIScriptGeneric *gen)
		{
			T const*	src = Cast<const T *>( gen->GetArgObject(0) );
			T*			dst = Cast<T *>( gen->GetObject() );
			//*dst = *src;

			dst->~T();
			new( dst ) T( *src );
		}
	};


	//
	// Script Function Descriptor
	//

	namespace _script_hidden_
	{

		template <typename T>
		struct GlobalFunction
		{
			static void GetDescriptor (OUT String &, StringCRef, uint, uint);
		};

		template <typename T>
		struct MemberFunction
		{
			static void GetDescriptor (OUT String &, StringCRef, uint, uint);
		};
		

		struct ArgsToString_Func
		{
			String &	result;
			const uint	first;
			const uint	last;
		
			ArgsToString_Func (uint first, uint last, INOUT String &str) : 
				result(str), first(first), last(last)
			{
				ASSERT( first <= last );
			}

			template <typename T, usize Index>
			void Process ()
			{
				if ( Index < first or Index > last )	return;
				if ( Index > first )					result << ", ";
				ScriptTypeInfo<T>::ArgName( INOUT result );
			}
		};

		template <typename Typelist>
		struct ArgsToString
		{
			static void Get (OUT String &str, uint first, uint last)
			{
				ArgsToString_Func	func( first, last, INOUT str );
				Typelist::RuntimeForEach( func );
			}
			
			static void GetArgs (OUT String &str, uint offsetFromStart, uint offsetFromEnd)
			{
				ASSERT( offsetFromEnd < Typelist::Count );

				str << '(';
				Get( INOUT str, offsetFromStart, Typelist::Count - offsetFromEnd );
				str << ')';
			}
		};


		template <typename Ret, typename ...Types>
		struct GlobalFunction < Ret (__cdecl *) (Types...) >
		{
			typedef typename CompileTime::TypeListFrom< Types... >	TypeList_t;
				
			static void GetDescriptor (OUT String &str, StringCRef name, uint offsetFromStart = 0, uint offsetFromEnd = 0)
			{
				ScriptTypeInfo< Ret >::Name( OUT str );
				str << ' ' << name;
				GetArgs( INOUT str, offsetFromStart, offsetFromEnd );
			}

			static void GetArgs (OUT String &str, uint offsetFromStart = 0, uint offsetFromEnd = 0)
			{
				ArgsToString< TypeList_t >::GetArgs( OUT str, offsetFromStart, offsetFromEnd );
			}
		};

		template <typename Ret>
		struct GlobalFunction < Ret (__cdecl *) () >
		{
			typedef typename CompileTime::TypeList< CompileTime::TypeListEnd >	TypeList_t;
				
			static void GetDescriptor (OUT String &str, StringCRef name, uint = 0, uint = 0)
			{
				ScriptTypeInfo< Ret >::Name( OUT str );
				str << ' ' << name << "()";
			}

			static void GetArgs (OUT String &str, uint = 0, uint = 0)
			{
				str << "()";
			}
		};
			
		template <typename C, typename Ret, typename ...Types>
		struct MemberFunction < Ret (__thiscall C:: *) (Types...) >
		{
			typedef typename CompileTime::TypeListFrom< Types... >	TypeList_t;
				
			static void GetDescriptor (OUT String &str, StringCRef name, uint offsetFromStart = 0, uint offsetFromEnd = 0)
			{
				GlobalFunction< Ret (*) (Types...) >::GetDescriptor( OUT str, name, offsetFromStart, offsetFromEnd );
			}

			static void GetArgs (OUT String &str, uint offsetFromStart = 0, uint offsetFromEnd = 0)
			{
				GlobalFunction< Ret (*) (Types...) >::GetArgs( OUT str, offsetFromStart, offsetFromEnd );
			}
		};
			
		template <typename C, typename Ret>
		struct MemberFunction < Ret (__thiscall C:: *) () >
		{
			typedef typename CompileTime::TypeList< CompileTime::TypeListEnd >	TypeList_t;
				
			static void GetDescriptor (OUT String &str, StringCRef name, uint offsetFromStart = 0, uint offsetFromEnd = 0)
			{
				GlobalFunction< Ret (*) () >::GetDescriptor( OUT str, name, offsetFromStart, offsetFromEnd );
			}

			static void GetArgs (OUT String &str, uint offsetFromStart = 0, uint offsetFromEnd = 0)
			{
				GlobalFunction< Ret (*) () >::GetArgs( OUT str, offsetFromStart, offsetFromEnd );
			}
		};
			
		template <typename C, typename Ret, typename ...Types>
		struct MemberFunction < Ret (__thiscall C:: *) (Types...) const >
		{
			typedef typename CompileTime::TypeListFrom< Types... >	TypeList_t;
				
			static void GetDescriptor (OUT String &str, StringCRef name, uint offsetFromStart = 0, uint offsetFromEnd = 0)
			{
				GlobalFunction< Ret (*) (Types...) >::GetDescriptor( OUT str, name, offsetFromStart, offsetFromEnd );
				str << " const";
			}

			static void GetArgs (OUT String &str, uint offsetFromStart = 0, uint offsetFromEnd = 0)
			{
				GlobalFunction< Ret (*) (Types...) >::GetArgs( OUT str, offsetFromStart, offsetFromEnd );
			}
		};
			
		template <typename C, typename Ret>
		struct MemberFunction < Ret (__thiscall C:: *) () const >
		{
			typedef typename CompileTime::TypeList< CompileTime::TypeListEnd >	TypeList_t;
				
			static void GetDescriptor (OUT String &str, StringCRef name, uint offsetFromStart = 0, uint offsetFromEnd = 0)
			{
				GlobalFunction< Ret (*) () >::GetDescriptor( OUT str, name, offsetFromStart, offsetFromEnd );
				str << " const";
			}

			static void GetArgs (OUT String &str, uint offsetFromStart = 0, uint offsetFromEnd = 0)
			{
				GlobalFunction< Ret (*) () >::GetArgs( OUT str, offsetFromStart, offsetFromEnd );
			}
		};

	}	// _script_hidden_

	

	//
	// Global Function
	//

	template <typename T>
	struct GlobalFunction : _script_hidden_::GlobalFunction<T>
	{
	};


	//
	// Member Function
	//

	template <typename T>
	struct MemberFunction : _script_hidden_::MemberFunction<T>
	{
	};


	namespace _script_hidden_
	{

		//
		// Context Setter Getter
		//
		template <typename T>
		struct ContextSetterGetter
		{
			static T	Get (AngelScript::asIScriptContext *ctx);
			static int	Set (AngelScript::asIScriptContext *ctx, int arg, const T &value);
		};
		
		template <>
		struct ContextSetterGetter <void>
		{
			static void	Get (AngelScript::asIScriptContext *)		{}
			static int	Set (AngelScript::asIScriptContext *, int)	{ return 0; }
		};

#		define DECL_CONTEXT_RESULT( _type_, _get_, _set_ ) \
			template <> \
			struct ContextSetterGetter < _type_ > \
			{ \
				static _type_ Get (AngelScript::asIScriptContext *ctx)									{ return _type_(ctx->_get_()); } \
				static int    Set (AngelScript::asIScriptContext *ctx, int arg, const _type_ &value)	{ return ctx->_set_( arg, value ); } \
			}

		DECL_CONTEXT_RESULT( byte,		GetReturnByte,		SetArgByte );
		DECL_CONTEXT_RESULT( ubyte,		GetReturnByte,		SetArgByte );
		DECL_CONTEXT_RESULT( short,		GetReturnWord,		SetArgWord );
		DECL_CONTEXT_RESULT( ushort,	GetReturnWord,		SetArgWord );
		DECL_CONTEXT_RESULT( int,		GetReturnDWord,		SetArgDWord );
		DECL_CONTEXT_RESULT( uint,		GetReturnDWord,		SetArgDWord );
		DECL_CONTEXT_RESULT( ilong,		GetReturnQWord,		SetArgQWord );
		DECL_CONTEXT_RESULT( ulong,		GetReturnQWord,		SetArgQWord );
		DECL_CONTEXT_RESULT( float,		GetReturnFloat,		SetArgFloat );
		DECL_CONTEXT_RESULT( double,	GetReturnDouble,	SetArgDouble );
	
		template <typename T>
		struct ContextSetterGetter < T * >
		{
			static T *  Get (AngelScript::asIScriptContext *ctx)									{ return Cast<T *>(ctx->GetReturnAddress()); }
			static int  Set (AngelScript::asIScriptContext *ctx, int arg, const T * const & ptr)	{ return ctx->SetArgAddress( arg, Cast<void *>(ptr) ); }
		};

#		undef DECL_CONTEXT_RESULT


		//
		// Set Context Args
		//
		template <typename ...Args>
		struct SetContextArgs;
			
		template <typename Arg0, typename ...Args>
		struct SetContextArgs< Arg0, Args...>
		{
			static void Set (AngelScript::asIScriptContext *ctx, int index, Arg0 arg0, Args ...args)
			{
				AS_CALL( ContextSetterGetter<Arg0>::Set( ctx, index, arg0 ) );
				SetContextArgs<Args...>::Set( ctx, index+1, FW<Args>(args)... );
			}
		};

		template <typename Arg0>
		struct SetContextArgs< Arg0 >
		{
			static void Set (AngelScript::asIScriptContext *ctx, int index, Arg0 arg0)
			{
				AS_CALL( ContextSetterGetter<Arg0>::Set( ctx, index, arg0 ) );
			}
		};

		template <>
		struct SetContextArgs<>
		{
			static void Set (AngelScript::asIScriptContext *, int)
			{}
		};


		//
		// Context Result
		//
		template <typename T>
		struct ContextResult
		{
			using type	= T;

			T&	value;

			explicit ContextResult (T &value) : value{value} {}
		};

		template <>
		struct ContextResult <void>
		{
			using type	= void;
		};

	}	// _script_hidden_

}	// GXScript
}	// GX_STL
