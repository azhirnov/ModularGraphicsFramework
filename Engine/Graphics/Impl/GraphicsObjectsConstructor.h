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

		static ModulePtr CreateFont (GlobalSystemsRef, const CreateInfo::Font &);
		static ModulePtr CreateFontManager (GlobalSystemsRef, const CreateInfo::FontManager &);
		static ModulePtr CreateBatchRenderer (GlobalSystemsRef, const CreateInfo::BatchRenderer &);
		static ModulePtr CreateAsyncCommandBuffer (GlobalSystemsRef, const CreateInfo::AsyncCommandBuffer &);
		static ModulePtr CreateCommandBufferManager (GlobalSystemsRef, const CreateInfo::CommandBufferManager &);
		static ModulePtr CreateMemoryInputStream (GlobalSystemsRef, const CreateInfo::GpuMemoryInStream &);
		static ModulePtr CreateMemoryOutputStream (GlobalSystemsRef, const CreateInfo::GpuMemoryOutStream &);
	};


}	// Graphics
}	// Engine