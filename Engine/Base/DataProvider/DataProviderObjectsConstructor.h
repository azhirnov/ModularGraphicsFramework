// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Public/DataProvider.h"

namespace Engine
{
namespace Base
{

	//
	// Data Provider Objects Constructor
	//

	class DataProviderObjectsConstructor final : public Noninstancable
	{
	public:
		static void Register (GlobalSystemsRef);
		static void Unregister (GlobalSystemsRef);

		static ModulePtr CreateDataProviderManager (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::DataProviderManager &);

		static ModulePtr CreateLocalStorageDataProvider (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::LocalStorageDataProvider &);
		//static ModulePtr CreateInMemoryDataProvider (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::InMemoryDataProvider &);
		//static ModulePtr CreateInternetDataProvider (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::InternetDataProvider &);
		static ModulePtr CreateBuiltinStorageDataProvider (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::BuiltinStorageDataProvider &);

		static ModulePtr CreateFileInputStream (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::InputStream &);
		static ModulePtr CreateFileOutputStream (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::OutputStream &);
		//static ModulePtr CreateInputStreamFromDataSource (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::InputStreamFromDataSource &);
		//static ModulePtr CreateOutputStreamFromDataSource (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::OutputStreamFromDataSource &);
		static ModulePtr CreateFileDataInput (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::DataInput &);
		static ModulePtr CreateFileDataOutput (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::DataOutput &);
	};


}	// Base
}	// Engine
