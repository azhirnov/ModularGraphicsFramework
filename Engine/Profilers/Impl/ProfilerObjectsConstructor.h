// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Profilers/Public/IDs.h"

namespace Engine
{
namespace CreateInfo
{
	struct FPSCounter;

}	// CreateInfo


namespace Profilers
{

	//
	// Profiler Objects Constructor
	//

	class ProfilerObjectsConstructor final : public Noninstancable
	{
	// methods
	public:
		static ModulePtr CreateFPSCounter (GlobalSystemsRef, const CreateInfo::FPSCounter &);
	};


}	// Profilers
}	// Engine
