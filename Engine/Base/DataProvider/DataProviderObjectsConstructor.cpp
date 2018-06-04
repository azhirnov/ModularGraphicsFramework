// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/DataProvider/DataProviderObjectsConstructor.h"
#include "Engine/Base/Modules/ModulesFactory.h"

namespace Engine
{
namespace Base
{

	using ProviderManagerMsgList_t	= _BaseMessages_::MessageListFrom< DSMsg::GetDataProviderForURI >;
	using DataProviderMsgList_t		= _BaseMessages_::MessageListFrom< DSMsg::IsUriExists >;

/*
=================================================
	DefaultCreateFileInputStream
=================================================
*/
	static ModulePtr DefaultCreateFileInputStream (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::InputStream &ci)
	{
		ModulePtr	provider = ci.provider;

		if ( not provider )
		{
			ModulePtr	data_mngr = gs->mainSystem->GetModuleByMsg< ProviderManagerMsgList_t >();
			CHECK_ERR( data_mngr );

			Message< DSMsg::GetDataProviderForURI >		get_provider{ ci.uri };
			data_mngr->Send( get_provider );
			CHECK_ERR( get_provider->result );

			provider = *get_provider->result;
		}

		Message< DSMsg::CreateDataInputModule >		ctor{ ci.uri };
		CHECK( provider->Send( ctor ) );

		return *ctor->result;
	}
	
/*
=================================================
	DefaultCreateFileOutputStream
=================================================
*/
	static ModulePtr DefaultCreateFileOutputStream (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::OutputStream &ci)
	{
		ModulePtr	provider = ci.provider;

		if ( not provider )
		{
			ModulePtr	data_mngr = gs->mainSystem->GetModuleByMsg< ProviderManagerMsgList_t >();
			CHECK_ERR( data_mngr );

			Message< DSMsg::GetDataProviderForURI >		get_provider{ ci.uri };
			data_mngr->Send( get_provider );
			CHECK_ERR( get_provider->result );

			provider = *get_provider->result;
		}

		Message< DSMsg::CreateDataOutputModule >	ctor{ ci.uri };
		CHECK( provider->Send( ctor ) );

		return *ctor->result;
	}
	
/*
=================================================
	DefaultCreateFileDataInput
=================================================
*/
	static ModulePtr DefaultCreateFileDataInput (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::DataInput &ci)
	{
		ModulePtr	provider = ci.provider;

		if ( not provider )
		{
			ModulePtr	data_mngr = gs->mainSystem->GetModuleByMsg< ProviderManagerMsgList_t >();
			CHECK_ERR( data_mngr );

			Message< DSMsg::GetDataProviderForURI >		get_provider{ ci.uri };
			data_mngr->Send( get_provider );
			CHECK_ERR( get_provider->result );

			provider = *get_provider->result;
		}

		Message< DSMsg::CreateDataInputModule >		ctor{ ci.uri };
		CHECK( provider->Send( ctor ) );

		return *ctor->result;
	}
	
/*
=================================================
	DefaultCreateFileDataOutput
=================================================
*/
	static ModulePtr DefaultCreateFileDataOutput (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::DataOutput &ci)
	{
		ModulePtr	provider = ci.provider;

		if ( not provider )
		{
			ModulePtr	data_mngr = gs->mainSystem->GetModuleByMsg< ProviderManagerMsgList_t >();
			CHECK_ERR( data_mngr );

			Message< DSMsg::GetDataProviderForURI >		get_provider{ ci.uri };
			data_mngr->Send( get_provider );
			CHECK_ERR( get_provider->result );

			provider = *get_provider->result;
		}

		Message< DSMsg::CreateDataOutputModule >	ctor{ ci.uri };
		CHECK( provider->Send( ctor ) );

		return *ctor->result;
	}

/*
=================================================
	Register
=================================================
*/
	void DataProviderObjectsConstructor::Register (GlobalSystemsRef gs)
	{
		auto	mf = gs->modulesFactory;

		CHECK( mf->Register( DataProviderManagerModuleID, &CreateDataProviderManager ) );
		
		CHECK( mf->Register( LocalStorageDataProviderModuleID, &CreateLocalStorageDataProvider ) );
		//CHECK( mf->Register( InMemoryDataProviderModuleID, &CreateInMemoryDataProvider ) );
		//CHECK( mf->Register( InternetDataProviderModuleID, &CreateInternetDataProvider ) );
		CHECK( mf->Register( BuiltinStorageDataProviderModuleID, &CreateBuiltinStorageDataProvider ) );

		CHECK( mf->Register( FileInputStreamModuleID, &CreateFileInputStream ) );
		CHECK( mf->Register( FileOutputStreamModuleID, &CreateFileOutputStream ) );
		//CHECK( mf->Register( InputStreamModuleID, &CreateInputStreamFromDataSource ) );
		//CHECK( mf->Register( OutputStreamModuleID, &CreateOutputStreamFromDataSource ) );
		CHECK( mf->Register( FileDataInputModuleID, &CreateFileDataInput ) );
		CHECK( mf->Register( FileDataOutputModuleID, &CreateFileDataOutput ) );
		
		CHECK( mf->Register( 0, &DefaultCreateFileInputStream ) );
		CHECK( mf->Register( 0, &DefaultCreateFileOutputStream ) );
		CHECK( mf->Register( 0, &DefaultCreateFileDataInput ) );
		CHECK( mf->Register( 0, &DefaultCreateFileDataOutput ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void DataProviderObjectsConstructor::Unregister (GlobalSystemsRef gs)
	{
		auto	mf = gs->modulesFactory;

		mf->UnregisterAll( DataProviderManagerModuleID );
		mf->UnregisterAll( LocalStorageDataProviderModuleID );
		mf->UnregisterAll( InMemoryDataProviderModuleID );
		mf->UnregisterAll( InternetDataProviderModuleID );
		mf->UnregisterAll( BuiltinStorageDataProviderModuleID );
		mf->UnregisterAll( FileInputStreamModuleID );
		mf->UnregisterAll( FileOutputStreamModuleID );
		mf->UnregisterAll( FileDataInputModuleID );
		mf->UnregisterAll( FileDataOutputModuleID );
	}
	
}	// Base
}	// Engine
