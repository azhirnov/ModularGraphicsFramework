// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Stream/Stream.h"

namespace Engine
{
namespace Base
{

	//
	// Stream Manager
	//

	class StreamManager : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t;
		using SupportedEvents_t		= Module::SupportedEvents_t;
		

	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// methods
	public:
		StreamManager (GlobalSystemsRef gs, const CreateInfo::StreamManager &info);
		~StreamManager ();
		
		static void Register (GlobalSystemsRef);
		static void Unregister (GlobalSystemsRef);


	private:
		static ModulePtr _CreateStreamManager (GlobalSystemsRef, const CreateInfo::StreamManager &);
		static ModulePtr _CreateInStreamFromFile (GlobalSystemsRef, const CreateInfo::InStreamFromFile &);
		static ModulePtr _CreateInStreamFromUri (GlobalSystemsRef, const CreateInfo::InStreamFromUri &);
		static ModulePtr _CreateInStreamFromMemory (GlobalSystemsRef, const CreateInfo::InStreamFromMemory &);
		static ModulePtr _CreateOutStreamFromFile (GlobalSystemsRef, const CreateInfo::OutStreamFromFile &);
		static ModulePtr _CreateOutStreamFromUri (GlobalSystemsRef, const CreateInfo::OutStreamFromUri &);
	};


}	// Base
}	// Engine
