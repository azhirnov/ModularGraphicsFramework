// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1CommandBuffer.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Command Buffer Builder
	//

	class Vk1CommandBuilder final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GpuCmdSetViewport,
											ModuleMsg::GpuCmdSetScissor,
											ModuleMsg::GpuCmdSetDepthBounds,
											ModuleMsg::GpuCmdSetBlendColor,
											ModuleMsg::GpuCmdSetDepthBias,
											ModuleMsg::GpuCmdSetLineWidth,
											ModuleMsg::GpuCmdSetStencilCompareMask,
											ModuleMsg::GpuCmdSetStencilWriteMask,
											ModuleMsg::GpuCmdSetStencilReference,
											ModuleMsg::GpuCmdBegin,
											ModuleMsg::GpuCmdEnd,
											ModuleMsg::GpuCmdBeginRenderPass,
											ModuleMsg::GpuCmdBeginRenderPassID,
											ModuleMsg::GpuCmdEndRenderPass,
											ModuleMsg::GpuCmdNextSubpass,
											ModuleMsg::GpuCmdBindPipeline,
											ModuleMsg::GpuCmdBindGraphicsPipelineID,
											ModuleMsg::GpuCmdBindComputePipelineID,
											ModuleMsg::GpuCmdBindVertexBuffers,
											ModuleMsg::GpuCmdBindVertexBufferIDs,
											ModuleMsg::GpuCmdBindIndexBuffer,
											ModuleMsg::GpuCmdBindIndexBufferID,
											ModuleMsg::GpuCmdDraw,
											ModuleMsg::GpuCmdDrawIndexed,
											ModuleMsg::GpuCmdDrawIndirect,
											ModuleMsg::GpuCmdDrawIndexedIndirect,
											ModuleMsg::GpuCmdDrawIndirectID,
											ModuleMsg::GpuCmdDrawIndexedIndirectID,
											ModuleMsg::GpuCmdExecute,
											ModuleMsg::GpuCmdExecuteID,
											ModuleMsg::GpuCmdBindDescriptorSet,
											ModuleMsg::GpuCmdCopyBuffer,
											ModuleMsg::GpuCmdCopyImage,
											ModuleMsg::GpuCmdCopyBufferToImage,
											ModuleMsg::GpuCmdCopyImageToBuffer,
											ModuleMsg::GpuCmdBlitImage,
											ModuleMsg::GpuCmdUpdateBuffer,
											ModuleMsg::GpuCmdFillBuffer,
											ModuleMsg::GpuCmdClearAttachments,
											ModuleMsg::GpuCmdClearColorImage,
											ModuleMsg::GpuCmdClearDepthStencilImage
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;

		using DynamicStates_t		= EPipelineDynamicState::bits;

		enum class EScope
		{
			None,
			Command,
			RenderPass,
		};

		SHARED_POINTER( Vk1CommandBuffer );


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		vk::VkCommandPool		_cmdPool;

		Vk1CommandBufferPtr		_cmdBuffer;
		EScope					_scope;

		DynamicStates_t			_dynamicStates;
		uint					_subpassIndex;
		uint					_maxSubpasses;
		//bool					_hasIndexBuffer;


	// methods
	public:
		Vk1CommandBuilder (const GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci);
		~Vk1CommandBuilder ();

		static OModID::type		GetStaticID ()			{ return "vk1.cmdbld"_OModID; }


	//protected:
		vk::VkCommandPool		GetCmdPoolID ()	const	{ return _cmdPool; }


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		
		bool _GpuCmdSetViewport (const Message< ModuleMsg::GpuCmdSetViewport > &);
		bool _GpuCmdSetScissor (const Message< ModuleMsg::GpuCmdSetScissor > &);
		bool _GpuCmdSetDepthBounds (const Message< ModuleMsg::GpuCmdSetDepthBounds > &);
		bool _GpuCmdSetBlendColor (const Message< ModuleMsg::GpuCmdSetBlendColor > &);
		bool _GpuCmdSetDepthBias (const Message< ModuleMsg::GpuCmdSetDepthBias > &);
		bool _GpuCmdSetLineWidth (const Message< ModuleMsg::GpuCmdSetLineWidth > &);
		bool _GpuCmdSetStencilCompareMask (const Message< ModuleMsg::GpuCmdSetStencilCompareMask > &);
		bool _GpuCmdSetStencilWriteMask (const Message< ModuleMsg::GpuCmdSetStencilWriteMask > &);
		bool _GpuCmdSetStencilReference (const Message< ModuleMsg::GpuCmdSetStencilReference > &);
		bool _GpuCmdBegin (const Message< ModuleMsg::GpuCmdBegin > &);
		bool _GpuCmdEnd (const Message< ModuleMsg::GpuCmdEnd > &);
		bool _GpuCmdBeginRenderPass (const Message< ModuleMsg::GpuCmdBeginRenderPass > &);
		bool _GpuCmdBeginRenderPassID (const Message< ModuleMsg::GpuCmdBeginRenderPassID > &);
		bool _GpuCmdEndRenderPass (const Message< ModuleMsg::GpuCmdEndRenderPass > &);
		bool _GpuCmdNextSubpass (const Message< ModuleMsg::GpuCmdNextSubpass > &);
		bool _GpuCmdBindPipeline (const Message< ModuleMsg::GpuCmdBindPipeline > &);
		bool _GpuCmdBindGraphicsPipelineID (const Message< ModuleMsg::GpuCmdBindGraphicsPipelineID > &);
		bool _GpuCmdBindComputePipelineID (const Message< ModuleMsg::GpuCmdBindComputePipelineID > &);
		bool _GpuCmdBindVertexBuffers (const Message< ModuleMsg::GpuCmdBindVertexBuffers > &);
		bool _GpuCmdBindVertexBufferIDs (const Message< ModuleMsg::GpuCmdBindVertexBufferIDs > &);
		bool _GpuCmdBindIndexBuffer (const Message< ModuleMsg::GpuCmdBindIndexBuffer > &);
		bool _GpuCmdBindIndexBufferID (const Message< ModuleMsg::GpuCmdBindIndexBufferID > &);
		bool _GpuCmdDraw (const Message< ModuleMsg::GpuCmdDraw > &);
		bool _GpuCmdDrawIndexed (const Message< ModuleMsg::GpuCmdDrawIndexed > &);
		bool _GpuCmdDrawIndirect (const Message< ModuleMsg::GpuCmdDrawIndirect > &);
		bool _GpuCmdDrawIndexedIndirect (const Message< ModuleMsg::GpuCmdDrawIndexedIndirect > &);
		bool _GpuCmdDrawIndirectID (const Message< ModuleMsg::GpuCmdDrawIndirectID > &);
		bool _GpuCmdDrawIndexedIndirectID (const Message< ModuleMsg::GpuCmdDrawIndexedIndirectID > &);
		bool _GpuCmdExecute (const Message< ModuleMsg::GpuCmdExecute > &);
		bool _GpuCmdExecuteID (const Message< ModuleMsg::GpuCmdExecuteID > &);
		bool _GpuCmdBindDescriptorSet (const Message< ModuleMsg::GpuCmdBindDescriptorSet > &);
		bool _GpuCmdCopyBuffer (const Message< ModuleMsg::GpuCmdCopyBuffer > &);
		bool _GpuCmdCopyImage (const Message< ModuleMsg::GpuCmdCopyImage > &);
		bool _GpuCmdCopyBufferToImage (const Message< ModuleMsg::GpuCmdCopyBufferToImage > &);
		bool _GpuCmdCopyImageToBuffer (const Message< ModuleMsg::GpuCmdCopyImageToBuffer > &);
		bool _GpuCmdBlitImage (const Message< ModuleMsg::GpuCmdBlitImage > &);
		bool _GpuCmdUpdateBuffer (const Message< ModuleMsg::GpuCmdUpdateBuffer > &);
		bool _GpuCmdFillBuffer (const Message< ModuleMsg::GpuCmdFillBuffer > &);
		bool _GpuCmdClearAttachments (const Message< ModuleMsg::GpuCmdClearAttachments > &);
		bool _GpuCmdClearColorImage (const Message< ModuleMsg::GpuCmdClearColorImage > &);
		bool _GpuCmdClearDepthStencilImage (const Message< ModuleMsg::GpuCmdClearDepthStencilImage > &);
	
	private:
		bool _IsCreated () const;
		bool _CreateCmdBufferPool ();
		void _DestroyCmdBufferPool ();

		bool _CheckDynamicState (EPipelineDynamicState::type state) const;
		bool _CheckCompatibility (const FramebufferDescriptor &fbDescr,
								  const RenderPassDescriptor &rpDescr) const;
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
