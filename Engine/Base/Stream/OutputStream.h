// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Stream/Stream.h"
#include "Engine/Base/Modules/Module.h"

namespace Engine
{
namespace Base
{
	
	//
	// Output Stream Module
	//

	class OutputStream final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= MessageListFrom< 
											ModuleMsg::AttachModule,
											ModuleMsg::DetachModule,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached,
											ModuleMsg::FindModule,
											ModuleMsg::ModulesDeepSearch,
											ModuleMsg::Delete,
											ModuleMsg::GetStreamDescriptor,
											ModuleMsg::WriteToStream
										>;

		using SupportedEvents_t		= MessageListFrom< 
											ModuleMsg::DataRegionChanged,
											ModuleMsg::Delete
										>;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		File::WFilePtr	_file;
		BinaryArray		_cache;


	// methods
	public:
		OutputStream (const GlobalSystemsRef gs, const CreateInfo::OutStreamFromUri &ci);
		OutputStream (const GlobalSystemsRef gs, const CreateInfo::OutStreamFromFile &ci);
		//OutputStream (const GlobalSystemsRef gs, const CreateInfo::OutStreamToMemory &ci);
		~OutputStream ();

		static OModID::type		GetStaticID ()			{ return "out-stream"_OModID; }


	// message handlers
	private:
		bool _GetFileStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &);
		bool _GetMemStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &);
		bool _WriteToFileStream (const Message< ModuleMsg::WriteToStream > &);
		bool _WriteToMemStream (const Message< ModuleMsg::WriteToStream > &);
	};

	
}	// Base
}	// Engine
