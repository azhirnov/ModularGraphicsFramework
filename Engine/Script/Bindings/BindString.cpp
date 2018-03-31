// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Script/Bindings/DefaultBindings.h"
#include "Engine/Script/Impl/ClassBinder.h"
#include "Engine/Script/Impl/ScriptEngine.inl.h"

#include "Engine/STL/Containers/HashMap.h"
#include "Engine/STL/Math/BinaryMath.h"
#include "Engine/STL/Math/Interpolations.h"
#include "Engine/STL/Log/ToString.h"

#include <unordered_map>

namespace std
{
	template <>
	struct hash <GX_STL::GXTypes::String>
	{
		size_t operator () (const GX_STL::GXTypes::String &str) const
		{
			return (size_t) GX_STL::GXTypes::HashOf( str ).Get();
		}
	};

}	// std

namespace GX_STL
{
namespace GXScript
{
	
	class StringFactory final :
					public ScriptEngine::ScriptSharedObj,
					public AngelScript::asIStringFactory
	{
	// types
	private:
		using Strings_t	= Set< String *>;


	// variables
	private:
		Strings_t		_instances;


	// methods
	public:
		StringFactory ();
		~StringFactory ();

		// asIStringFactory //
		const void *GetStringConstant (const char *data, AngelScript::asUINT length) override;
		int         ReleaseStringConstant (const void *str) override;
		int         GetRawStringData (const void *str, char *data, AngelScript::asUINT *length) const override;
	};
	

	
/*
=================================================
	constructor
=================================================
*/
	StringFactory::StringFactory ()
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	StringFactory::~StringFactory ()
	{
		ASSERT( _instances.Empty() );
	}

/*
=================================================
	GetStringConstant
=================================================
*/
	const void * StringFactory::GetStringConstant (const char *data, AngelScript::asUINT length)
	{
		auto*	inst = new String{ data, length };

		_instances.Add( inst );

		return reinterpret_cast<const void *>( inst );
	}
	
/*
=================================================
	ReleaseStringConstant
=================================================
*/
	int StringFactory::ReleaseStringConstant (const void *ptr)
	{
		using namespace AngelScript;

		if ( ptr == null )
			return asERROR;

		auto*	str = (String *) const_cast<void*>( ptr );

		_instances.Erase( str );

		str->~String();

		return asSUCCESS;
	}
	
/*
=================================================
	GetRawStringData
=================================================
*/
	int StringFactory::GetRawStringData (const void *ptr, char *data, AngelScript::asUINT *length) const
	{
		using namespace AngelScript;

		if ( ptr == null )
			return asERROR;

		auto*	str = reinterpret_cast<const String *>( ptr );

		if ( length )
			*length = (asUINT)str->Length();

		if ( data )
		{
			if ( not str->Empty() )
				UnsafeMem::MemCopy( data, str->cstr(), str->LengthInBytes() );
			else
				*data = 0;
		}

		return asSUCCESS;
	}
//-----------------------------------------------------------------------------



	class StringCacheFactory final :
					public ScriptEngine::ScriptSharedObj,
					public AngelScript::asIStringFactory
	{
	// types
	private:
		using StringCache_t	= std::unordered_map< String, uint >;


	// variables
	private:
		StringCache_t		_cache;


	// methods
	public:
		StringCacheFactory ();
		~StringCacheFactory ();

		// asIStringFactory //
		const void *GetStringConstant (const char *data, AngelScript::asUINT length) override;
		int         ReleaseStringConstant (const void *str) override;
		int         GetRawStringData (const void *str, char *data, AngelScript::asUINT *length) const override;
	};
	

	
/*
=================================================
	constructor
=================================================
*/
	StringCacheFactory::StringCacheFactory ()
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	StringCacheFactory::~StringCacheFactory ()
	{
		ASSERT( _cache.empty() );
	}

/*
=================================================
	GetStringConstant
=================================================
*/
	const void * StringCacheFactory::GetStringConstant (const char *data, AngelScript::asUINT length)
	{
		String str{ data, length };
		
		StringCache_t::iterator iter = _cache.find( str );

		if ( iter != _cache.end() )
			++iter->second;
		else
			iter = _cache.insert({ str, 1 }).first;

		return reinterpret_cast<const void *>( &iter->first );
	}
	
/*
=================================================
	ReleaseStringConstant
=================================================
*/
	int StringCacheFactory::ReleaseStringConstant (const void *str)
	{
		using namespace AngelScript;
		
		if ( str == null )
			return asERROR;

		StringCache_t::iterator	iter = _cache.find( *reinterpret_cast<const String *>(str) );

		if ( iter == _cache.end() )
			return asERROR;

		if ( --iter->second == 0 )
			_cache.erase( iter );

		return asSUCCESS;
	}
	
/*
=================================================
	GetRawStringData
=================================================
*/
	int StringCacheFactory::GetRawStringData (const void *ptr, char *data, AngelScript::asUINT *length) const
	{
		using namespace AngelScript;
		
		if ( ptr == null )
			return asERROR;

		auto* str = reinterpret_cast<const String *>( ptr );

		if ( length )
			*length = (asUINT)str->Length();

		if ( data )
			memcpy( data, str->cstr(), str->Length() );

		return asSUCCESS;
	}
//-----------------------------------------------------------------------------



	//
	// String Functions
	//
	struct StringFunc final : Noninstancable
	{
		static String& Assign (String &self, const String &other) {
			return self = other;
		}

		static String& AddAssign (String &self, const String &other) {
			return self += other;
		}

		static String Add (const String &left, const String &right) {
			return left + right;
		}

		static bool Equal (const String &left, const String &right) {
			return left == right;
		}

		static int Cmp (const String &left, const String &right)
		{
			// TODO: optimize
			if ( left < right ) return -1;
			if ( left > right ) return +1;
			return 0;
		}

		static ubyte& EditElem (String &self, uint index) {
			return ReferenceCast<ubyte>( self[index] );
		}

		static ubyte GetElem (const String &self, uint index) {
			return ReferenceCast<ubyte>( self[index] );
		}

		static bool Empty (const String &self) {
			return self.Empty();
		}

		static uint Length (const String &self) {
			return uint(self.Length());
		}

		static String& AppendBool (String &self, bool value) {
			return self += value;
		}

		static String AddBool (const String &self, bool value) {
			return self + ToString(value);
		}

		static String& AppendChar (String &self, ubyte value) {
			return self += char(value);
		}

		static String AddChar (const String &self, ubyte value) {
			return self + ToString( char(value) );
		}

		static String& AppendInt (String &self, int value) {
			return self += value;
		}

		static String AddInt (const String &self, int value) {
			return self + ToString( value );
		}

		static String& AppendUInt (String &self, uint value) {
			return self += value;
		}

		static String AddUInt (const String &self, uint value) {
			return self + ToString( value );
		}

		static String& AppendLong (String &self, ilong value) {
			return self += value;
		}

		static String AddLong (const String &self, ilong value) {
			return self + ToString( value );
		}

		static String& AppendULong (String &self, ulong value) {
			return self += value;
		}

		static String AddULong (const String &self, ulong value) {
			return self + ToString( value );
		}

		static String& AppendFloat (String &self, float value) {
			return self += value;
		}

		static String AddFloat (const String &self, float value) {
			return self + ToString( value );
		}

		static String& AppendDouble (String &self, double value) {
			return self += value;
		}

		static String AddDouble (const String &self, double value) {
			return self + ToString( value );
		}

		static bool EqualIgnoreCase (const String &left, const String &right) {
			return left.EqualsIC( right );
		}

		static bool HasSubString (const String &left, const String &right) {
			return left.HasSubString( right );
		}

		static bool HasSubStringIC (const String &left, const String &right) {
			return left.HasSubStringIC( right );
		}

		static bool StartsWith (const String &left, const String &right) {
			return left.StartsWith( right );
		}

		static bool StartsWithIC (const String &left, const String &right) {
			return left.StartsWithIC( right );
		}

		static bool EndsWith (const String &left, const String &right) {
			return left.EndsWith( right );
		}

		static bool EndsWithIC (const String &left, const String &right) {
			return left.EndsWithIC( right );
		}

		static void SetLength (String &self, uint newLength) {
			self.SetLength( usize(newLength) );
		}

		static void Clear (String &self) {
			self.Clear();
		}

		static void InsertChar (String &self, ubyte symbol, uint pos) {
			self.Insert( symbol, usize(pos) );
		}

		static void InsertString (String &self, const String &str, uint pos) {
			self.Insert( str, usize(pos) );
		}

		static uint FindChar (const String &self, ubyte symbol) {
			return FindChar2( self, symbol, 0 );
		}

		static uint FindChar2 (const String &self, ubyte symbol, uint start) {
			usize	pos = 0;
			return self.Find( symbol, OUT pos, start ) ? uint(pos) : ~0u;
		}

		static uint FindString (const String &self, const String &str) {
			return FindString2( self, str, 0 );
		}

		static uint FindString2 (const String &self, const String &str, uint start) {
			usize	pos = 0;
			return self.Find( str, OUT pos, start ) ? uint(pos) : ~0u;
		}
	};

/*
=================================================
	BindString
=================================================
*/
	void DefaultBindings::BindString (ScriptEngine *se)
	{
		ClassBinder< String >	binder{ se };
		auto					factory = New<StringFactory>();	// StringCacheFactory

		se->AddSharedObject( factory );

		binder.CreateClassValue();
		
		se->Get()->RegisterStringFactory( binder.Name().cstr(), factory.ptr() );

		binder.Operators()
			.BinaryAssign(	EBinaryOperator::Add,		&StringFunc::AddAssign )
			.Binary(		EBinaryOperator::Add,		&StringFunc::Add )
			//.Binary(		EBinaryOperator::ShiftLeft,	&StringFunc::AddAssign )
			.BinaryAssign(	EBinaryOperator::Add,		&StringFunc::AppendBool )
			.Binary(		EBinaryOperator::Add,		&StringFunc::AddBool )
			.BinaryAssign(	EBinaryOperator::Add,		&StringFunc::AppendChar )
			.Binary(		EBinaryOperator::Add,		&StringFunc::AddChar )
			.BinaryAssign(	EBinaryOperator::Add,		&StringFunc::AppendInt )
			.Binary(		EBinaryOperator::Add,		&StringFunc::AddInt )
			.BinaryAssign(	EBinaryOperator::Add,		&StringFunc::AppendUInt )
			.Binary(		EBinaryOperator::Add,		&StringFunc::AddUInt )
			.BinaryAssign(	EBinaryOperator::Add,		&StringFunc::AppendLong )
			.Binary(		EBinaryOperator::Add,		&StringFunc::AddLong )
			.BinaryAssign(	EBinaryOperator::Add,		&StringFunc::AppendULong )
			.Binary(		EBinaryOperator::Add,		&StringFunc::AddULong )
			.BinaryAssign(	EBinaryOperator::Add,		&StringFunc::AppendFloat )
			.Binary(		EBinaryOperator::Add,		&StringFunc::AddFloat )
			.BinaryAssign(	EBinaryOperator::Add,		&StringFunc::AppendDouble )
			.Binary(		EBinaryOperator::Add,		&StringFunc::AddDouble )
			.Equals(		&StringFunc::Equal )
			.Compare(		&StringFunc::Cmp )
			.Index(			&StringFunc::EditElem )
			.Index(			&StringFunc::GetElem );

		binder.AddMethodFromGlobal( &StringFunc::Empty,				"Empty" );
		binder.AddMethodFromGlobal( &StringFunc::Length,			"Length" );
		binder.AddMethodFromGlobal( &StringFunc::EqualIgnoreCase,	"EqualsIC" );
		binder.AddMethodFromGlobal( &StringFunc::HasSubString,		"HasSubString" );
		binder.AddMethodFromGlobal( &StringFunc::HasSubStringIC,	"HasSubStringIC" );
		binder.AddMethodFromGlobal( &StringFunc::StartsWith,		"StartsWith" );
		binder.AddMethodFromGlobal( &StringFunc::StartsWithIC,		"StartsWithIC" );
		binder.AddMethodFromGlobal( &StringFunc::EndsWith,			"EndsWith" );
		binder.AddMethodFromGlobal( &StringFunc::EndsWithIC,		"EndsWithIC" );
		binder.AddMethodFromGlobal( &StringFunc::SetLength,			"SetLength" );
		binder.AddMethodFromGlobal( &StringFunc::Clear,				"Clear" );
		binder.AddMethodFromGlobal( &StringFunc::InsertChar,		"InsertChar" );
		binder.AddMethodFromGlobal( &StringFunc::InsertString,		"InsertString" );
		binder.AddMethodFromGlobal( &StringFunc::FindChar,			"FindChar" );
		binder.AddMethodFromGlobal( &StringFunc::FindChar2,			"FindChar" );
		binder.AddMethodFromGlobal( &StringFunc::FindString,		"FindString" );
		binder.AddMethodFromGlobal( &StringFunc::FindString2,		"FindString" );
	}
	
/*
=================================================
	LogFunc
=================================================
*/
	struct LogFunc
	{
		static void Err (const String &msg) {
			LOG( msg.cstr(), ELog::Error );
		}

		static void Warn (const String &msg) {
			LOG( msg.cstr(), ELog::Warning );
		}

		static void Info (const String &msg) {
			LOG( msg.cstr(), ELog::Info );
		}

		static void Dbg (const String &msg) {
			LOG( msg.cstr(), ELog::Debug );
		}
	};
	
/*
=================================================
	BindLog
=================================================
*/
	void DefaultBindings::BindLog (ScriptEngine *se)
	{
		se->AddFunction( &LogFunc::Err,		"LogErr" );
		se->AddFunction( &LogFunc::Warn,	"LogWarn" );
		se->AddFunction( &LogFunc::Info,	"LogInfo" );
		se->AddFunction( &LogFunc::Dbg,		"LogDbg" );
	}


}	// GXScript
}	// GX_STL
