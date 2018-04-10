// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Stream/Stream.h"

namespace Engine
{
namespace Base
{

	//
	// Stream Objects Constructor
	//

	class StreamObjectsConstructor final : public Noninstancable
	{
	public:
		static void Register (GlobalSystemsRef);
		static void Unregister (GlobalSystemsRef);

		static ModulePtr CreateStreamManager (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::StreamManager &);
		static ModulePtr CreateInStreamFromFile (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::InStreamFromFile &);
		static ModulePtr CreateInStreamFromUri (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::InStreamFromUri &);
		static ModulePtr CreateInStreamFromMemory (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::InStreamFromMemory &);
		static ModulePtr CreateOutStreamFromFile (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::OutStreamFromFile &);
		static ModulePtr CreateOutStreamFromUri (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::OutStreamFromUri &);
	};


}	// Base
}	// Engine
