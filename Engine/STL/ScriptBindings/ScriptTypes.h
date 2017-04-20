// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Containers/String.h"
#include "Engine/STL/CompileTime/TypeList.h"
#include "Engine/STL/Types/Ptr.h"
#include "Engine/STL/Files/HDDFile.h"
#include "Engine/STL/Log/ELog.h"

#ifdef GX_ANGELSCRIPT_ENABLED

// AngelScript + Addons //
#define AS_USE_NAMESPACE
#include "External/angelscript/angelscript/include/angelscript.h"
#include "External/angelscript/add_on/scriptstdstring/scriptstdstring.h"


namespace GX_STL
{
namespace GXScript
{

#	define AS_CALL( ... ) \
	{ \
		int __as_result = ( __VA_ARGS__ ); \
		ScriptEngine::_CheckError( __as_result, TOSTRING( __VA_ARGS__ ), GX_FUNCTION_NAME, __FILE__, __LINE__ ); \
	}
	
#	define AS_CALL_R( ... ) \
	{ \
		int __as_result = ( __VA_ARGS__ ); \
		if ( not ScriptEngine::_CheckError( __as_result, TOSTRING( __VA_ARGS__ ), GX_FUNCTION_NAME, __FILE__, __LINE__ ) ) \
			return false; \
	}





	//
	// Script Type Info
	//

	template <typename T>
	struct ScriptTypeInfo
	{
		typedef T	type;

		static void Name (INOUT String &)		{ STATIC_WARNING( "type not defined" ); }
		static void ArgName (INOUT String &s)	{ }
		static uint SizeOf ()					{ return sizeof(T); }
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
			typedef ScriptTypeInfo<T>	base_t; \
			\
			static void Name (INOUT String &s)		{ _buildName_; } \
			static void ArgName (INOUT String &s)	{ _buildArg_; } \
			static uint SizeOf ()					{ return sizeof(type); } \
		}
	/*
#	define GX_DECL_SCRIPT_TYPE_EXT( _type_, _name_, _refName_, _refConstName_ ) \
		GX_DECL_SCRIPT_TYPE( _type_, _name_ ); \
		GX_DECL_SCRIPT_WRAP( const _type_ &,	ARGS( base_t::Name( s );  s << ' ' << _refConstName_ ) ); \
		GX_DECL_SCRIPT_WRAP( _type_ &,			ARGS( base_t::Name( s );  s << ' ' << _refName_		 ) ); \
		GX_DECL_SCRIPT_WRAP( const _type_ *,	ARGS( base_t::Name( s );  s << ' ' << _refConstName_ ) ); \
		GX_DECL_SCRIPT_WRAP( _type_ *,			ARGS( base_t::Name( s );  s << ' ' << _refName_		 ) );
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
							base_t::Name( s );
						),
						ARGS(
							s << "const ";
							base_t::Name( s );
						));

	GX_DECL_SCRIPT_WRAP( const T &,
						ARGS(
							s << "const ";
							base_t::Name( s );
							s << " &";
						),
						ARGS(
							s << "const ";
							base_t::Name( s );
							s << " &in";
						));
		
	GX_DECL_SCRIPT_WRAP( T &,
						ARGS(
							base_t::Name( s );
							s << " &";
						),
						ARGS(
							base_t::Name( s );
							s << " &out";
						));
		
	GX_DECL_SCRIPT_WRAP( const T *,
						ARGS(
							s << "const ";
							base_t::Name( s );
							s << " &";
						),
						ARGS(
							s << "const ";
							base_t::Name( s );
							s << " &in";
						));
		
	GX_DECL_SCRIPT_WRAP( T *,
						ARGS(
							base_t::Name( s );
							s << " &";
						),
						ARGS(
							base_t::Name( s );
							s << " &out";
						));




	//
	// Angel Script Helper
	//

	struct AngelScriptHelper
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
			T * a = static_cast<T *>( gen->GetArgObject(0) );
			new( gen->GetObject() ) T(*a);
		}
		

		template <typename T>
		static void Destructor (AngelScript::asIScriptGeneric *gen)
		{
			((T *) gen->GetObject() )->~T();
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
			static void GetDescriptor (OUT String &, StringCRef )	{ STATIC_WARNING( "not supported" ); }
		};


		template <typename T>
		struct MemberFunction
		{
			static void GetDescriptor (OUT String &, StringCRef )	{ STATIC_WARNING( "not supported" ); }
		};
		
		struct ArgsToString_Func
		{
			String &	result;
			uint		offset;
		
			ArgsToString_Func (uint off, String &str) : 
				result(str), offset(off)
			{}

			template <typename T, usize Index>
			void Process ()
			{
				if ( Index < offset )	return;
				if ( Index > offset )	result << ", ";
				ScriptTypeInfo<T>::ArgName( result );
			}
		};

		template <typename Typelist>
		struct ArgsToString
		{
			static void Get (OUT String &str, uint offset = 0)
			{
				ArgsToString_Func	func( offset, str );
				Typelist::RuntimeForEach( func );
			}
			
			static void GetArgs (OUT String &str, uint offset = 0)
			{
				str << '(';
				Get( str, offset );
				str << ')';
			}
		};

		template <typename Ret, typename ...Types>
		struct GlobalFunction < Ret (__cdecl *) (Types...) >
		{
			typedef typename CompileTime::TypeListFrom< Types... >	TypeList_t;
				
			static void GetDescriptor (OUT String &str, StringCRef name, uint offset = 0)
			{
				ScriptTypeInfo< Ret >::Name( str );
				str << ' ' << name;
				GetArgs( str, offset );
			}

			static void GetArgs (OUT String &str, uint offset = 0)
			{
				ArgsToString< TypeList_t >::GetArgs( str, offset );
			}
		};

		template <typename Ret>
		struct GlobalFunction < Ret (__cdecl *) () >
		{
			typedef typename CompileTime::TypeList< CompileTime::TypeListEnd >	TypeList_t;
				
			static void GetDescriptor (OUT String &str, StringCRef name, uint = 0)
			{
				ScriptTypeInfo< Ret >::Name( str );
				str << ' ' << name << "()";
			}

			static void GetArgs (OUT String &str, uint = 0)
			{
				str << "()";
			}
		};
			
		template <typename C, typename Ret, typename ...Types>
		struct MemberFunction < Ret (__thiscall C:: *) (Types...) >
		{
			typedef typename CompileTime::TypeListFrom< Types... >	TypeList_t;
				
			static void GetDescriptor (OUT String &str, StringCRef name, uint offset = 0)
			{
				GlobalFunction< Ret (*) (Types...) >::GetDescriptor( str, name, offset );
			}

			static void GetArgs (OUT String &str, uint offset = 0)
			{
				GlobalFunction< Ret (*) (Types...) >::GetArgs( str, offset );
			}
		};
			
		template <typename C, typename Ret>
		struct MemberFunction < Ret (__thiscall C:: *) () >
		{
			typedef typename CompileTime::TypeList< CompileTime::TypeListEnd >	TypeList_t;
				
			static void GetDescriptor (OUT String &str, StringCRef name, uint offset = 0)
			{
				GlobalFunction< Ret (*) () >::GetDescriptor( str, name, offset );
			}

			static void GetArgs (OUT String &str, uint offset = 0)
			{
				GlobalFunction< Ret (*) () >::GetArgs( str, offset );
			}
		};
			
		template <typename C, typename Ret, typename ...Types>
		struct MemberFunction < Ret (__thiscall C:: *) (Types...) const >
		{
			typedef typename CompileTime::TypeListFrom< Types... >	TypeList_t;
				
			static void GetDescriptor (OUT String &str, StringCRef name, uint offset = 0)
			{
				GlobalFunction< Ret (*) (Types...) >::GetDescriptor( str, name, offset );
				str << " const";
			}

			static void GetArgs (OUT String &str, uint offset = 0)
			{
				GlobalFunction< Ret (*) (Types...) >::GetArgs( str, offset );
			}
		};
			
		template <typename C, typename Ret>
		struct MemberFunction < Ret (__thiscall C:: *) () const >
		{
			typedef typename CompileTime::TypeList< CompileTime::TypeListEnd >	TypeList_t;
				
			static void GetDescriptor (OUT String &str, StringCRef name, uint offset = 0)
			{
				GlobalFunction< Ret (*) () >::GetDescriptor( str, name, offset );
				str << " const";
			}

			static void GetArgs (OUT String &str, uint offset = 0)
			{
				GlobalFunction< Ret (*) () >::GetArgs( str, offset );
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


	//
	// Context Setter Getter
	//

	template <typename T>
	struct ContextSetterGetter
	{
		static T	Get (AngelScript::asIScriptContext *ctx)							{ STATIC_WARNING( "not supported for this type" );  return T(); }
		static int	Set (AngelScript::asIScriptContext *ctx, int arg, const T &value)	{ STATIC_WARNING( "not supported for this type" );  return 0; }
	};

#	define DECL_CONTEXT_RESULT( _type_, _get_, _set_ ) \
		template <> \
		struct ContextSetterGetter < _type_ > \
		{ \
			static _type_ Get (AngelScript::asIScriptContext *ctx)									{ return (_type_) ctx->_get_(); } \
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
		static T *  Get (AngelScript::asIScriptContext *ctx)									{ return (T *) ctx->GetReturnAddress(); }
		static int  Set (AngelScript::asIScriptContext *ctx, int arg, const T * const & ptr)	{ return ctx->SetArgAddress( arg, (void *) ptr ); }
	};

#	undef DECL_CONTEXT_RESULT


}	// GXScript
}	// GX_STL

#endif	// GX_ANGELSCRIPT_ENABLED
