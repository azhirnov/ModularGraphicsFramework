// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Graphics/Public/IDs.h"

namespace Engine
{
namespace CreateInfo
{
	struct Font;
	struct FontManager;
	struct BatchRenderer;
	struct AsyncCommandBuffer;
	struct CommandBufferManager;
	struct GpuMemoryInStream;
	struct GpuMemoryOutStream;

}	// CreateInfo


namespace Graphics
{

	//
	// Graphics Objects Constructor
	//

	class GraphicsObjectsConstructor final : public Noninstancable
	{
	// methods
	public:
		static void Register ();
		static void Unregister ();

		static ModulePtr CreateFont (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::Font &);
		static ModulePtr CreateFontManager (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::FontManager &);
		static ModulePtr CreateBatchRenderer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::BatchRenderer &);
		static ModulePtr CreateAsyncCommandBuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::AsyncCommandBuffer &);
		static ModulePtr CreateCommandBufferManager (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::CommandBufferManager &);
		static ModulePtr CreateMemoryInputStream (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuMemoryInStream &);
		static ModulePtr CreateMemoryOutputStream (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuMemoryOutStream &);
	};


}	// Graphics
}	// Engine