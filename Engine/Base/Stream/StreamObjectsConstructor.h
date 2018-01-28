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

		static ModulePtr CreateStreamManager (GlobalSystemsRef, const CreateInfo::StreamManager &);
		static ModulePtr CreateInStreamFromFile (GlobalSystemsRef, const CreateInfo::InStreamFromFile &);
		static ModulePtr CreateInStreamFromUri (GlobalSystemsRef, const CreateInfo::InStreamFromUri &);
		static ModulePtr CreateInStreamFromMemory (GlobalSystemsRef, const CreateInfo::InStreamFromMemory &);
		static ModulePtr CreateOutStreamFromFile (GlobalSystemsRef, const CreateInfo::OutStreamFromFile &);
		static ModulePtr CreateOutStreamFromUri (GlobalSystemsRef, const CreateInfo::OutStreamFromUri &);
	};


}	// Base
}	// Engine
