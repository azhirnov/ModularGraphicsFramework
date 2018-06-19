// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Script/Impl/ScriptModule.h"
#include "Core/STL/Math/BinaryMath.h"

namespace GXScript
{

/*
=================================================
	constructor
=================================================
*/
	ScriptModule::ScriptModule (const ScriptEnginePtr &eng) : _engine{ eng }
	{
	}
		
/*
=================================================
	destructor
=================================================
*/
	ScriptModule::~ScriptModule ()
	{
		Destroy();
	}

/*
=================================================
	Create
=================================================
*/
	bool ScriptModule::Create (StringCRef name)
	{
		using namespace AngelScript;

		CHECK_ERR( _engine and _engine->Get() );

		_module = _engine->Get()->GetModule( name.cstr(), asGM_ALWAYS_CREATE );
		CHECK_ERR( _module );

		return true;
	}
	
/*
=================================================
	Destroy
=================================================
*/
	void ScriptModule::Destroy ()
	{
		if ( _module )
		{
			_module->Discard();
			_module = null;
		}

		_engine = null;
	}
	
/*
=================================================
	Run
=================================================
*/
	bool ScriptModule::Run (StringCRef script, StringCRef entry)
	{
		return _Run<void>( script, entry, CtxResult<void>{} );
	}
	
/*
=================================================
	RunFromFile
=================================================
*/
	bool ScriptModule::RunFromFile (StringCRef filename, StringCRef entry)
	{
		GXFile::HddRFile	file;
		CHECK_ERR( file.Open( filename ) );

		String			data;
		const BytesU	len = file.Size();

		data.Reserve( usize(len)+1 );
		file.Read( data.ptr(), len );
		data.SetLength( usize(len) );

		file.Close();

		return Run( data, entry );
	}


}	// GXScript
