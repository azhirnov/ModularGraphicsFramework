// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1Framebuffer.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Pipeline.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Buffer.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1CommandBuffer.h"
#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Command Buffer Builder
	//

	class Vk1CommandBufferBuilder final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::CommandBufferSetViewport,
											ModuleMsg::CommandBufferSetScissor,
											ModuleMsg::CommandBufferSetDepthBounds,
											ModuleMsg::CommandBufferSetBlendColor,
											ModuleMsg::CommandBufferSetDepthBias,
											ModuleMsg::CommandBufferSetLineWidth,
											ModuleMsg::CommandBufferSetStencilCompareMask,
											ModuleMsg::CommandBufferSetStencilWriteMask,
											ModuleMsg::CommandBufferSetStencilReference,
											ModuleMsg::CommandBufferBegin,
											ModuleMsg::CommandBufferEnd,
											ModuleMsg::CommandBufferBeginRenderPass,
											ModuleMsg::CommandBufferBeginRenderPassID,
											ModuleMsg::CommandBufferEndRenderPass,
											ModuleMsg::CommandBufferNextSubpass,
											ModuleMsg::CommandBufferBindPipeline,
											ModuleMsg::CommandBufferBindGraphicsPipelineID,
											ModuleMsg::CommandBufferBindComputePipelineID,
											ModuleMsg::CommandBufferBindVertexBuffers,
											ModuleMsg::CommandBufferBindVertexBufferIDs,
											ModuleMsg::CommandBufferBindIndexBuffer,
											ModuleMsg::CommandBufferBindIndexBufferID,
											ModuleMsg::CommandBufferDraw,
											ModuleMsg::CommandBufferDrawIndexed,
											ModuleMsg::CommandBufferDrawIndirect,
											ModuleMsg::CommandBufferDrawIndexedIndirect,
											ModuleMsg::CommandBufferDrawIndirectID,
											ModuleMsg::CommandBufferDrawIndexedIndirectID,
											ModuleMsg::CommandBufferExecute,
											ModuleMsg::CommandBufferExecuteID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;
		
		using Viewports_t		= FixedSizeArray< vk::VkViewport, GlobalConst::Graphics_MaxColorBuffers >;
		using Scissors_t		= FixedSizeArray< vk::VkRect2D, GlobalConst::Graphics_MaxColorBuffers >;
		using ClearValues_t		= FixedSizeArray< vk::VkClearValue, GlobalConst::Graphics_MaxColorBuffers >;
		using VertexBuffers_t	= FixedSizeArray< vk::VkBuffer, GlobalConst::Graphics_MaxAttribs >;
		using CmdBuffers_t		= FixedSizeArray< vk::VkCommandBuffer, 32 >;
		using DynamicStates_t	= EPipelineDynamicState::bits;

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
		Vk1CommandBufferBuilder (const GlobalSystemsRef gs, const CreateInfo::GpuCommandBufferBuilder &ci);
		~Vk1CommandBufferBuilder ();

		static OModID::type		GetStaticID ()			{ return "vk1.cmdbld"_OModID; }


	//protected:
		vk::VkCommandPool		GetCmdPoolID ()	const	{ return _cmdPool; }


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		
		bool _CommandBufferSetViewport (const Message< ModuleMsg::CommandBufferSetViewport > &);
		bool _CommandBufferSetScissor (const Message< ModuleMsg::CommandBufferSetScissor > &);
		bool _CommandBufferSetDepthBounds (const Message< ModuleMsg::CommandBufferSetDepthBounds > &);
		bool _CommandBufferSetBlendColor (const Message< ModuleMsg::CommandBufferSetBlendColor > &);
		bool _CommandBufferSetDepthBias (const Message< ModuleMsg::CommandBufferSetDepthBias > &);
		bool _CommandBufferSetLineWidth (const Message< ModuleMsg::CommandBufferSetLineWidth > &);
		bool _CommandBufferSetStencilCompareMask (const Message< ModuleMsg::CommandBufferSetStencilCompareMask > &);
		bool _CommandBufferSetStencilWriteMask (const Message< ModuleMsg::CommandBufferSetStencilWriteMask > &);
		bool _CommandBufferSetStencilReference (const Message< ModuleMsg::CommandBufferSetStencilReference > &);
		bool _CommandBufferBegin (const Message< ModuleMsg::CommandBufferBegin > &);
		bool _CommandBufferEnd (const Message< ModuleMsg::CommandBufferEnd > &);
		bool _CommandBufferBeginRenderPass (const Message< ModuleMsg::CommandBufferBeginRenderPass > &);
		bool _CommandBufferBeginRenderPassID (const Message< ModuleMsg::CommandBufferBeginRenderPassID > &);
		bool _CommandBufferEndRenderPass (const Message< ModuleMsg::CommandBufferEndRenderPass > &);
		bool _CommandBufferNextSubpass (const Message< ModuleMsg::CommandBufferNextSubpass > &);
		bool _CommandBufferBindPipeline (const Message< ModuleMsg::CommandBufferBindPipeline > &);
		bool _CommandBufferBindGraphicsPipelineID (const Message< ModuleMsg::CommandBufferBindGraphicsPipelineID > &);
		bool _CommandBufferBindComputePipelineID (const Message< ModuleMsg::CommandBufferBindComputePipelineID > &);
		bool _CommandBufferBindVertexBuffers (const Message< ModuleMsg::CommandBufferBindVertexBuffers > &);
		bool _CommandBufferBindVertexBufferIDs (const Message< ModuleMsg::CommandBufferBindVertexBufferIDs > &);
		bool _CommandBufferBindIndexBuffer (const Message< ModuleMsg::CommandBufferBindIndexBuffer > &);
		bool _CommandBufferBindIndexBufferID (const Message< ModuleMsg::CommandBufferBindIndexBufferID > &);
		bool _CommandBufferDraw (const Message< ModuleMsg::CommandBufferDraw > &);
		bool _CommandBufferDrawIndexed (const Message< ModuleMsg::CommandBufferDrawIndexed > &);
		bool _CommandBufferDrawIndirect (const Message< ModuleMsg::CommandBufferDrawIndirect > &);
		bool _CommandBufferDrawIndexedIndirect (const Message< ModuleMsg::CommandBufferDrawIndexedIndirect > &);
		bool _CommandBufferDrawIndirectID (const Message< ModuleMsg::CommandBufferDrawIndirectID > &);
		bool _CommandBufferDrawIndexedIndirectID (const Message< ModuleMsg::CommandBufferDrawIndexedIndirectID > &);
		bool _CommandBufferExecute (const Message< ModuleMsg::CommandBufferExecute > &);
		bool _CommandBufferExecuteID (const Message< ModuleMsg::CommandBufferExecuteID > &);

	private:
		bool _IsCreated () const;
		bool _CreateCmdBufferPool ();
		void _DestroyCmdBufferPool ();

		bool _CheckDynamicState (EPipelineDynamicState::type state) const;
		bool _CheckCompatibility (const FramebufferDescriptor &fbDescr,
								  const RenderPassDescriptor &rpDescr) const;
	};




	//
	// Command Buffer Builder
	//
	/*
	class Vk1CommandBufferBuilder final : public BaseObject
	{
	// types
	private:
		SHARED_POINTER( Vk1CommandBuffer );
		SHARED_POINTER( Vk1Framebuffer );
		SHARED_POINTER( Vk1RenderPass );
		SHARED_POINTER( Vk1Buffer );

		using BindedVertexBuffers	= StaticArray< vk::VkBuffer, GlobalConst::Graphics_MaxAttribs >;
		using BindedVertexOffsets	= StaticArray< vk::VkDeviceSize, GlobalConst::Graphics_MaxAttribs >;
		using DynamicStates			= EPipelineDynamicState::bits;
		using CommandBufferPtr		= Vk1CommandBufferPtr;
		using FramebufferPtr		= Vk1FramebufferPtr;
		using RenderPassPtr			= Vk1RenderPassPtr;
		using BufferPtr				= Vk1BufferPtr;
		using VertexBufferPtr		= Vk1BufferPtr;
		using IndexBufferPtr		= Vk1BufferPtr;
		
		struct Viewport
		{
			uint2	offset;
			uint2	size;
			float2	depthRange;

			Viewport (const RectU &rect, const float2 &depth) :
				offset( rect.LeftBottom() ),
				size( rect.Size() ),
				depthRange( depth )
			{}
		};


	// variables
	private:
		const VkSystemsRef		_vkSystems;

		vk::VkCommandPool		_cmdPool;

		CommandBufferPtr		_cmdBuffer;
		FramebufferPtr			_framebuffer;
		RenderPassPtr			_renderPass;
		uint					_subpassIndex;
		DynamicStates			_enabledDynamicStates;	// copied from pipeline

		BindedVertexBuffers		_vertexBuffers;
		BindedVertexOffsets		_vertexOffsets;
		bool					_vbChanged;
		BufferPtr				_indexBuffer;


	// methods
	public:
		explicit Vk1CommandBufferBuilder (const GlobalSystemsRef gs);
		Vk1CommandBufferBuilder (const GlobalSystemsRef gs, const VkSystemsRef vkSys);
		~Vk1CommandBufferBuilder ();
		
		bool Create ();
		void Destroy ();


		// dynamic states
		bool SetViewport (ArrayCRef<Viewport> values, uint offset);
		bool SetScissor (ArrayCRef<RectU> values, uint offset);
		bool SetDepthBounds (const float2 &limits);
		bool SetBlendColor (const color4f &color);
		bool SetDepthBias (float biasConstFactor, float biasClamp, float biasSlopeFactor);
		bool SetLineWidth (float value);
		bool SetStencilCompareMask (EPolygonFace::type face, uint value);
		bool SetStencilWriteMask (EPolygonFace::type face, uint value);
		bool SetStencilReference (EPolygonFace::type face, uint value);


		// recording
		bool Begin (const CommandBufferPtr &cmdBuffer = null);
		bool BeginSecondary (const CommandBufferPtr &cmdBuffer = null);
		bool End (OUT CommandBufferPtr &cmdBuffer);

		bool BeginRenderPass (const FramebufferPtr &framebuffer, const RenderPassPtr &pass, const RectU &area);
		bool NextSubPass ();
		bool EndRenderPass ();


		// record draw commands
		/*bool BindPipeline (const ShaderProgramPtr &program,
						   const VertexAttribs &attribs,
						   const RenderState &renderStates,
						   const DynamicStates &dynamicStates,
						   uint subpass);*//*

		bool BindUniforms ();
		bool BindBuffer (uint slot, const VertexBufferPtr &vb, BytesU offset);
		bool BindBuffer (const IndexBufferPtr &ib, BytesU offset);

		bool Draw (uint vertexCount, uint instanceCount, uint firstVertex, uint firstInstance);
		bool DrawIndexed (uint indexCount, uint instanceCount, uint firstVertex, int vertexOffset, uint firstInstance);

		bool DrawIndirect (const BufferPtr &indirectBuffer, BytesU offset, uint drawCount, BytesU stride);
		bool DrawIndexedIndirect (const BufferPtr &indirectBuffer, BytesU offset, uint drawCount, BytesU stride);


		// record copy commands
		bool CopyBuffer ();
		bool CopyImage ();
		bool BlitImage ();
		bool UpdateBuffer ();
		bool FillBuffer ();
		bool ClearImage ();


		// record secondary command buffers
		bool Execute (ArrayCRef<CommandBufferPtr> commands);


		// record events


		//
		vk::VkCommandPool	CommandPoolId ()	const	{ return _cmdPool; }


	private:
		VkSystemsRef		VkSystems ()		const	{ return _vkSystems; }
		Ptr< Vk1Device >	GetDevice ()		const	{ return VkSystems()->Get< Vk1Device >(); }
		vk::VkDevice		GetLogicalDevice ()	const	{ return GetDevice()->GetLogicalDevice(); }


		bool _Begin (const CommandBufferPtr &cmdBuffer, bool isSecondary);

		void _BindVertexBuffers ();
		void _BeforeDraw ();

		void _Destroy ();
	};*/


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
