// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Stream/Stream.h"
#include "Engine/Base/Modules/Module.h"

namespace Engine
{
namespace Base
{
	
	//
	// Input Stream Module
	//

	class InputStream final : public Module
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
											ModuleMsg::ReadFromStream
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
		File::RFilePtr	_file;
		BinaryArray		_cache;


	// methods
	public:
		InputStream (const GlobalSystemsRef gs, const CreateInfo::InStreamFromUri &ci);
		InputStream (const GlobalSystemsRef gs, const CreateInfo::InStreamFromFile &ci);
		InputStream (const GlobalSystemsRef gs, const CreateInfo::InStreamFromMemory &ci);
		~InputStream ();

		static OModID::type		GetStaticID ()			{ return "in-stream"_OModID; }


	// message handlers
	private:
		bool _GetFileStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &);
		bool _GetMemStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &);
		bool _ReadFromFileStream (const Message< ModuleMsg::ReadFromStream > &);
		bool _ReadFromMemStream (const Message< ModuleMsg::ReadFromStream > &);

	private:
		void _CreateCache ();

		static BytesU	_MaxCacheSize ()	{ return BytesU::FromMb( 1 ); }
	};

	
}	// Base
}	// Engine
