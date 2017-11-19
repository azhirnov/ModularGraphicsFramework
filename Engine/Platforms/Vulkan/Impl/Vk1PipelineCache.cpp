// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1PipelineCache.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Pipeline.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	using namespace vk;

/*
=================================================
	constructor
=================================================
*/
	Vk1PipelineCache::Vk1PipelineCache (Ptr<Vk1Device> dev) :
		Vk1BaseObject( dev ),
		_pipelinesCache( VK_NULL_HANDLE )
	{
		_graphicsPipelines.Reserve( 128 );
		_computePipelines.Reserve( 128 );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1PipelineCache::~Vk1PipelineCache ()
	{
		ASSERT( _pipelinesCache == VK_NULL_HANDLE );
	}

/*
=================================================
	Create
=================================================
*/
	Vk1PipelineCache::Vk1GraphicsPipelinePtr  Vk1PipelineCache::Create (GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci)
	{
		// TODO: validate and cache

		auto	result = New< Vk1GraphicsPipeline >( gs, ci );

		ModuleUtils::Initialize({ result });

		CHECK_ERR( result->GetState() == Module::EState::ComposedImmutable );

		_graphicsPipelines.Add( result );
		return result;
	}
	
/*
=================================================
	Create
=================================================
*/
	Vk1PipelineCache::Vk1ComputePipelinePtr  Vk1PipelineCache::Create (GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci)
	{
		// TODO: validate and cache

		auto	result = New< Vk1ComputePipeline >( gs, ci );

		ModuleUtils::Initialize({ result });

		CHECK_ERR( result->GetState() == Module::EState::ComposedImmutable );

		_computePipelines.Add( result );
		return result;
	}

/*
=================================================
	_CreatePipelineCache
=================================================
*/
	bool Vk1PipelineCache::_CreatePipelineCache ()
	{
		if ( _pipelinesCache != VK_NULL_HANDLE )
			return true;
		
		VkPipelineCacheCreateInfo	info = {};

		info.sType				= VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		info.pNext				= null;
		info.flags				= 0;
		info.initialDataSize	= 0;
		info.pInitialData		= null;

		VK_CHECK( vkCreatePipelineCache( GetVkDevice(), &info, null, OUT &_pipelinesCache ) );
		return true;
	}
	
/*
=================================================
	_DestroyPipelineCache
=================================================
*/
	void Vk1PipelineCache::_DestroyPipelineCache ()
	{
		auto	dev = GetVkDevice();

		if ( dev != VK_NULL_HANDLE and _pipelinesCache != VK_NULL_HANDLE )
		{
			vkDestroyPipelineCache( dev, _pipelinesCache, null );
		}

		_pipelinesCache = VK_NULL_HANDLE;
	}
	
/*
=================================================
	_DestroyPipelines
=================================================
*/
	void Vk1PipelineCache::_DestroyPipelines ()
	{
		Message< ModuleMsg::Delete >	del_msg;

		FOR( i, _graphicsPipelines ) {
			_graphicsPipelines[i]->Send( del_msg );
		}

		FOR( i, _computePipelines ) {
			_computePipelines[i]->Send( del_msg );
		}

		_graphicsPipelines.Clear();
		_computePipelines.Clear();
	}

/*
=================================================
	_ClearTemp
=================================================
*/
	void Vk1PipelineCache::_ClearTemp ()
	{
		_tempStages.Clear();
		_tempSpecialization.Clear();
		_tempVertexBinding.Clear();
		_tempVertexAttribs.Clear();
		_tempViewports.Clear();
		_tempScissors.Clear();
		_tempDynamicStates.Clear();
		_tempAttachments.Clear();
	}
	
/*
=================================================
	_ClearTemp
=================================================
*/
	void Vk1PipelineCache::Destroy ()
	{
		_DestroyPipelines();
		_DestroyPipelineCache();
		_ClearTemp();
	}

/*
=================================================
	CreatePipeline
=================================================
*/
	bool Vk1PipelineCache::CreatePipeline (OUT VkPipeline &pipelineId,
											ArrayCRef< ShaderModule > shaders,
											VkPipelineLayout layout,
											const VertexInputState &attribs,
											const RenderState &renderState,
											const EDynamicStates &dynamicStates,
											uint patchControlPoints,
											const RenderPassDescriptor &rpDescr,
											VkRenderPass renderPass,
											uint subpass)
	{
		_ClearTemp();
		CHECK_ERR( _CreatePipelineCache() );
		CHECK_ERR( subpass < rpDescr.Subpasses().Count() );

		VkGraphicsPipelineCreateInfo			pipeline_info		= {};
		VkPipelineInputAssemblyStateCreateInfo	input_assembly_info	= {};
		VkPipelineColorBlendStateCreateInfo		blend_info			= {};
		VkPipelineDepthStencilStateCreateInfo	depth_stencil_info	= {};
		VkPipelineMultisampleStateCreateInfo	multisample_info	= {};
		VkPipelineRasterizationStateCreateInfo	rasterization_info	= {};
		VkPipelineTessellationStateCreateInfo	tessellation_info	= {};
		VkPipelineDynamicStateCreateInfo		dynamic_state_info	= {};
		VkPipelineVertexInputStateCreateInfo	vertex_input_info	= {};
		VkPipelineViewportStateCreateInfo		viewport_info		= {};

		_SetShaderStages( OUT _tempStages, OUT _tempSpecialization, shaders );
		_SetDynamicState( OUT dynamic_state_info, OUT _tempDynamicStates, dynamicStates );
		_SetColorBlendState( OUT blend_info, OUT _tempAttachments, renderState.color, rpDescr.Subpasses()[subpass] );
		_SetMultisampleState( OUT multisample_info, renderState.multisample );
		_SetTessellationState( OUT tessellation_info, patchControlPoints );
		_SetDepthStencilState( OUT depth_stencil_info, renderState.depth, renderState.stencil );
		_SetRasterizationState( OUT rasterization_info, renderState.rasterization );
		_SetupPipelineInputAssemblyState( OUT input_assembly_info, renderState.inputAssembly );
		_SetVertexInputState( OUT vertex_input_info, OUT _tempVertexBinding, OUT _tempVertexAttribs, attribs );
		_SetViewportState( OUT viewport_info, OUT _tempViewports, OUT _tempScissors, rpDescr.Subpasses()[subpass] );

		pipeline_info.sType					= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipeline_info.pNext					= null;
		pipeline_info.flags					= 0;		// TODO
		pipeline_info.pInputAssemblyState	= &input_assembly_info;
		pipeline_info.pRasterizationState	= &rasterization_info;
		pipeline_info.pColorBlendState		= &blend_info;
		pipeline_info.pDepthStencilState	= &depth_stencil_info;
		pipeline_info.pMultisampleState		= &multisample_info;
		pipeline_info.pTessellationState	= patchControlPoints > 0 ? &tessellation_info : null;
		pipeline_info.pVertexInputState		= &vertex_input_info;
		pipeline_info.pViewportState		= &viewport_info;
		pipeline_info.pDynamicState			= dynamicStates.Empty() ? null : &dynamic_state_info;
		pipeline_info.basePipelineIndex		= -1;
		pipeline_info.basePipelineHandle	= VK_NULL_HANDLE;
		pipeline_info.layout				= layout;
		pipeline_info.stageCount			= (uint32_t) _tempStages.Count();
		pipeline_info.pStages				= _tempStages.RawPtr();
		pipeline_info.renderPass			= renderPass;
		pipeline_info.subpass				= subpass;

		VK_CHECK( vkCreateGraphicsPipelines( GetVkDevice(), _pipelinesCache, 1, &pipeline_info, null, OUT &pipelineId ) );
		return true;
	}
	
/*
=================================================
	CreatePipeline
=================================================
*/
	bool Vk1PipelineCache::CreatePipeline (OUT VkPipeline &pipelineId,
											const ShaderModule &shader,
											VkPipelineLayout layout)
	{
		_ClearTemp();
		CHECK_ERR( _CreatePipelineCache() );

		VkComputePipelineCreateInfo		pipeline_info = {};
		
		pipeline_info.sType			= VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipeline_info.layout		= layout;
		pipeline_info.flags			= 0;
		pipeline_info.stage.sType	= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipeline_info.stage.module	= shader.id;
		pipeline_info.stage.pName	= shader.entry.cstr();
		pipeline_info.stage.flags	= 0;
		pipeline_info.stage.stage	= VK_SHADER_STAGE_COMPUTE_BIT;

		VK_CHECK( vkCreateComputePipelines( GetVkDevice(), _pipelinesCache, 1, &pipeline_info, null, OUT &pipelineId ) );
		return true;
	}

/*
=================================================
	_SetViewportState
=================================================
*/
	void Vk1PipelineCache::_SetViewportState (OUT VkPipelineViewportStateCreateInfo &outState,
											  OUT Viewports_t &tmpViewports,
											  OUT Scissors_t &tmpScissors,
											  const RenderPassDescriptor::Subpass_t &subpass) const
	{
		tmpViewports.Resize( subpass.colors.Count() );
		tmpScissors.Resize( subpass.colors.Count() );

		FOR( i, subpass.colors )
		{
			tmpViewports[i] = VkViewport{ 0, 0, 1024, 1024, 0.0f, 1.0f };
			tmpScissors[i]	= VkRect2D{ VkOffset2D{ 0, 0 }, VkExtent2D{ 1024, 1024 } };
		}

		outState.sType			= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		outState.pNext			= null;
		outState.flags			= 0;
		outState.pViewports		= tmpViewports.RawPtr();
		outState.viewportCount	= (uint32_t) tmpViewports.Count();
		outState.pScissors		= tmpScissors.RawPtr();
		outState.scissorCount	= (uint32_t) tmpScissors.Count();
	}

/*
=================================================
	_SetColorBlendState
=================================================
*/
	void Vk1PipelineCache::_SetColorBlendState (OUT VkPipelineColorBlendStateCreateInfo &outState,
												OUT Attachment_t &attachment,
												const RenderState::ColorBuffersState &inState,
												const RenderPassDescriptor::Subpass_t &subpass) const
	{
		const bool	logic_op_enabled	= ( inState.logicOp != ELogicOp::None );
		const usize	count				= Min( subpass.colors.Count(), inState.buffers.Count() );

		for (usize i = 0; i < count; ++i) {
			_SetColorBlendAttachmentState( OUT attachment[i], inState.buffers[i], logic_op_enabled );
		}

		outState.sType				= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		outState.pNext				= null;
		outState.flags				= 0;
		outState.attachmentCount	= (uint32_t) count;
		outState.pAttachments		= count ? attachment.ptr() : null;
		outState.logicOpEnable		= logic_op_enabled;
		outState.logicOp			= logic_op_enabled ? Vk1Enum( inState.logicOp ) : VK_LOGIC_OP_COPY;

		FOR( i, inState.blendColor ) {
			outState.blendConstants[i] = inState.blendColor[i];
		}
	}

/*
=================================================
	_SetColorBlendAttachmentState
=================================================
*/
	void Vk1PipelineCache::_SetColorBlendAttachmentState (OUT VkPipelineColorBlendAttachmentState &outState,
														  const RenderState::ColorBuffer &inState,
														  bool logicOpEnabled) const
	{
		outState.blendEnable			= logicOpEnabled ? false : inState.blend;
		outState.srcColorBlendFactor	= Vk1Enum( inState.blendFuncSrc.color );
		outState.srcAlphaBlendFactor	= Vk1Enum( inState.blendFuncSrc.alpha );
		outState.dstColorBlendFactor	= Vk1Enum( inState.blendFuncDst.color );
		outState.dstAlphaBlendFactor	= Vk1Enum( inState.blendFuncDst.alpha );
		outState.colorBlendOp			= Vk1Enum( inState.blendMode.color );
		outState.alphaBlendOp			= Vk1Enum( inState.blendMode.alpha );
		outState.colorWriteMask			= (inState.colorMask.x ? VK_COLOR_COMPONENT_R_BIT : 0) |
										  (inState.colorMask.y ? VK_COLOR_COMPONENT_G_BIT : 0) |
										  (inState.colorMask.z ? VK_COLOR_COMPONENT_B_BIT : 0) |
										  (inState.colorMask.w ? VK_COLOR_COMPONENT_A_BIT : 0);
	}
	
/*
=================================================
	_SetDepthStencilState
=================================================
*/
	void Vk1PipelineCache::_SetDepthStencilState (OUT VkPipelineDepthStencilStateCreateInfo &outState,
												  const RenderState::DepthBufferState &depth,
												  const RenderState::StencilBufferState &stencil) const
	{
		float2	range = depth.range.Get( float2(0.0f, 1.0f) );

		outState.sType					= VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		outState.pNext					= null;
		outState.flags					= 0;

		// depth
		outState.depthTestEnable		= depth.enabled;
		outState.depthWriteEnable		= depth.write;
		outState.depthCompareOp			= Vk1Enum( depth.func );
		outState.depthBoundsTestEnable	= depth.range.IsDefined();
		outState.minDepthBounds			= range.x;
		outState.maxDepthBounds			= range.y;
		
		// stencil
		outState.stencilTestEnable		= stencil.enabled;
		_SetStencilOpState( OUT outState.front, stencil.front );
		_SetStencilOpState( OUT outState.back,  stencil.back );
	}
	
/*
=================================================
	_SetStencilOpState
=================================================
*/
	void Vk1PipelineCache::_SetStencilOpState (OUT VkStencilOpState &outState,
											   const RenderState::StencilFaceState &inState) const
	{
		outState.failOp			= Vk1Enum( inState.sfail );
		outState.passOp			= Vk1Enum( inState.dppass );
		outState.depthFailOp	= Vk1Enum( inState.dfail );
		outState.compareOp		= Vk1Enum( inState.func );
		outState.compareMask	= inState.mask;				// TODO: check
		outState.writeMask		= inState.funcMask;			// TODO: check
		outState.reference		= inState.funcRef;			// TODO: check
	}

/*
=================================================
	_SetupPipelineInputAssemblyState
=================================================
*/
	void Vk1PipelineCache::_SetupPipelineInputAssemblyState (OUT VkPipelineInputAssemblyStateCreateInfo &outState,
															 const RenderState::InputAssemblyState &inState) const
	{
		outState.sType					= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		outState.pNext					= null;
		outState.flags					= 0;
		outState.topology				= Vk1Enum( inState.topology );
		outState.primitiveRestartEnable	= inState.primitiveRestart;
	}
	
/*
=================================================
	_SetRasterizationState
=================================================
*/
	void Vk1PipelineCache::_SetRasterizationState (OUT VkPipelineRasterizationStateCreateInfo &outState,
													const RenderState::RasterizationState &inState) const
	{
		VkCullModeFlagBits	cull_mode_flags;
		Vk1Enum( inState.cullMode, OUT cull_mode_flags );

		outState.sType						= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		outState.pNext						= null;
		outState.flags						= 0;
		outState.polygonMode				= Vk1Enum( inState.polygonMode );
		outState.lineWidth					= inState.lineWidth;
		outState.depthBiasConstantFactor	= inState.depthBiasConstFactor;
		outState.depthBiasClamp				= inState.depthBiasClamp;
		outState.depthBiasSlopeFactor		= inState.depthBiasSlopeFactor;
		outState.depthBiasEnable			= inState.depthBias;
		outState.depthClampEnable			= inState.depthClamp;
		outState.rasterizerDiscardEnable	= inState.rasterizerDiscard;
		outState.frontFace					= inState.frontFaceCCW ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
		outState.cullMode					= (VkCullModeFlags) cull_mode_flags;
	}
	
/*
=================================================
	_SetMultisampleState
=================================================
*/
	void Vk1PipelineCache::_SetMultisampleState (OUT VkPipelineMultisampleStateCreateInfo &outState,
												 const RenderState::MultisampleState &inState) const
	{
		outState.sType					= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		outState.pNext					= null;
		outState.flags					= 0;
		outState.rasterizationSamples	= Vk1Enum( inState.samples );
		outState.sampleShadingEnable	= inState.sampleShading;
		outState.minSampleShading		= inState.minSampleShading;
		outState.pSampleMask			= null;						// TODO
		outState.alphaToCoverageEnable	= inState.alphaToCoverage;
		outState.alphaToOneEnable		= inState.alphaToOne;
	}
	
/*
=================================================
	_SetTessellationState
=================================================
*/
	void Vk1PipelineCache::_SetTessellationState (OUT VkPipelineTessellationStateCreateInfo &outState,
												  uint patchSize) const
	{
		outState.sType				= VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		outState.pNext				= null;
		outState.flags				= 0;
		outState.patchControlPoints	= patchSize;
	}
	
/*
=================================================
	_SetDynamicState
=================================================
*/
	void Vk1PipelineCache::_SetDynamicState (OUT VkPipelineDynamicStateCreateInfo &outState,
											 OUT Vk1DynamicStates &states,
											 const EDynamicStates &inState) const
	{
		outState.sType	= VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		outState.pNext	= null;
		outState.flags	= 0;

		uint	j = 0;

		FOR( i, states )
		{
			const auto t = EPipelineDynamicState::type(i);

			if ( inState[t] )
				states[j++] = Vk1Enum( t );
		}

		outState.dynamicStateCount	= j;
		outState.pDynamicStates		= states.ptr();
	}
	
/*
=================================================
	_SetShaderStages
=================================================
*/
	void Vk1PipelineCache::_SetShaderStages (OUT Array< VkPipelineShaderStageCreateInfo > &stages,
											 OUT Array< VkSpecializationInfo > &specialization,
											 ArrayCRef< ShaderModule > shaders) const
	{
		FOR( i, shaders )
		{
			if ( shaders[i].id == VK_NULL_HANDLE )
				continue;

			VkPipelineShaderStageCreateInfo	info = {};

			info.sType	= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			info.pNext	= null;
			info.flags	= 0;
			info.module	= shaders[i].id;
			info.pName	= shaders[i].entry.cstr();
			info.stage	= Vk1Enum( shaders[i].type );

			stages.PushBack( info );
		}
	}
	
/*
=================================================
	_SetVertexInputState
=================================================
*/
	void Vk1PipelineCache::_SetVertexInputState (OUT VkPipelineVertexInputStateCreateInfo &outState,
												 OUT Array< VkVertexInputBindingDescription > &vertexBinding,
												 OUT Array< VkVertexInputAttributeDescription > &attribDescr,
												 const VertexInputState &inState) const
	{
		outState.sType	= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		outState.pNext	= null;
		outState.flags	= 0;

		FOR( i, inState.Attribs() )
		{
			const auto&							attr	= inState.Attribs()[i].second;
			VkVertexInputAttributeDescription	descr	= {};

			descr.binding	= attr.bindingIndex;
			descr.format	= Vk1Enum( attr.type );
			descr.location	= attr.index;
			descr.offset	= (uint32_t) attr.offset;

			attribDescr.PushBack( descr );
		}

		FOR( i, inState.Bindings() )
		{
			const auto&						binding	= inState.Bindings()[i].second;
			VkVertexInputBindingDescription	descr	= {};

			descr.binding	= binding.index;
			descr.inputRate	= Vk1Enum( binding.rate );
			descr.stride	= (uint32_t) binding.stride;

			vertexBinding.PushBack( descr );
		}

		// if pipeline has attributes then buffer binding must be defined
		CHECK( attribDescr.Empty() == vertexBinding.Empty() );

		outState.pVertexAttributeDescriptions		= attribDescr.RawPtr();
		outState.vertexAttributeDescriptionCount	= (uint32_t) attribDescr.Count();

		outState.pVertexBindingDescriptions			= vertexBinding.RawPtr();
		outState.vertexBindingDescriptionCount		= (uint32_t) vertexBinding.Count();
	}


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
