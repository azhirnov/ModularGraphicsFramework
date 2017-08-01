// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1Pipeline.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
/*
=================================================
	constructor
=================================================
*/
	Vk1PipelineCache::Vk1PipelineCache (VkSystemsRef vkSys) :
		_vkSystems( vkSys ),
		_graphicsPipelinesCache( VK_NULL_HANDLE ),
		_emptyLayout( VK_NULL_HANDLE )
	{
		_graphicsPipelines.Reserve( 128 );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1PipelineCache::~Vk1PipelineCache ()
	{
		ASSERT( _graphicsPipelinesCache == VK_NULL_HANDLE );
	}

/*
=================================================
	Create
=================================================
*/
	Vk1PipelineCache::Vk1GraphicsPipelinePtr  Vk1PipelineCache::Create (const GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci)
	{
		// TODO: validate and cache

		auto	result = New< Vk1GraphicsPipeline >( gs, ci );
		
		result->Send( Message< ModuleMsg::Link >() );
		result->Send( Message< ModuleMsg::Compose >() );
		
		CHECK_ERR( result->GetState() == Module::EState::ComposedImmutable );

		_graphicsPipelines.Add( result );
		return result;
	}
	
/*
=================================================
	_CreateCache
=================================================
*/
	bool Vk1PipelineCache::_CreateCache (vk::VkDevice dev)
	{
		using namespace vk;

		if ( _graphicsPipelinesCache != VK_NULL_HANDLE )
			return true;
		
		VkPipelineCacheCreateInfo	info = {};

		info.sType				= VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		info.pNext				= null;
		info.flags				= 0;
		info.initialDataSize	= 0;
		info.pInitialData		= null;

		VK_CHECK( vkCreatePipelineCache( dev, &info, null, OUT &_graphicsPipelinesCache ) );

		CHECK_ERR( _CreateLayout( dev ) );
		return true;
	}
	
/*
=================================================
	DestroyCache
=================================================
*/
	void Vk1PipelineCache::DestroyCache ()
	{
		using namespace vk;

		auto	dev = VkSystems()->Get< Vk1Device >()->GetLogicalDevice();

		if ( dev != VK_NULL_HANDLE and _graphicsPipelinesCache != VK_NULL_HANDLE )
		{
			// TODO: force delete
			_graphicsPipelines.Clear();

			vkDestroyPipelineCache( dev, _graphicsPipelinesCache, null );
		}

		_graphicsPipelinesCache = VK_NULL_HANDLE;

		_DestroyLayout( dev );
	}
	
/*
=================================================
	_CreateLayout
=================================================
*/
	bool Vk1PipelineCache::_CreateLayout (vk::VkDevice dev)
	{
		using namespace vk;

		if ( _emptyLayout != VK_NULL_HANDLE )
			return true;

		VkPipelineLayoutCreateInfo	info = {};
		info.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		info.setLayoutCount			= 0;
		info.pushConstantRangeCount	= 0;

		VK_CHECK( vkCreatePipelineLayout( dev, &info, null, OUT &_emptyLayout ) );
		return true;
	}
	
/*
=================================================
	_DestroyLayout
=================================================
*/
	void Vk1PipelineCache::_DestroyLayout (vk::VkDevice dev)
	{
		using namespace vk;

		if ( dev != VK_NULL_HANDLE and _emptyLayout != VK_NULL_HANDLE )
		{
			vkDestroyPipelineLayout( dev, _emptyLayout, null );
		}

		_emptyLayout = VK_NULL_HANDLE;
	}

/*
=================================================
	CreatePipeline
=================================================
*/
	bool Vk1PipelineCache::CreatePipeline (OUT vk::VkPipeline &pipelineId,
											ArrayRef< ShaderModule > shaders,
											vk::VkPipelineLayout layout,
											const VertexAttribs &attribs,
											const RenderState &renderState,
											const DynamicStates &dynamicStates,
											uint patchControlPoints,
											const RenderPassDescriptor &rpDescr,
											vk::VkRenderPass renderPass,
											uint subpass)
	{
		using namespace vk;

		Ptr<Vk1Device>	device	= VkSystems()->Get< Vk1Device >();
		auto			dev		= device->GetLogicalDevice();
		
		CHECK_ERR( _CreateCache( dev ) );
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

		_SetShaderStages( OUT _stages, OUT _specialization, shaders );
		_SetDynamicState( OUT dynamic_state_info, OUT _dynamicStates, dynamicStates );
		_SetColorBlendState( OUT blend_info, OUT _attachments, renderState.color, rpDescr.Subpasses()[subpass] );
		_SetMultisampleState( OUT multisample_info, renderState.multisample );
		_SetTessellationState( OUT tessellation_info, patchControlPoints );
		_SetDepthStencilState( OUT depth_stencil_info, renderState.depth, renderState.stencil );
		_SetRasterizationState( OUT rasterization_info, renderState.rasterization );
		_SetupPipelineInputAssemblyState( OUT input_assembly_info, renderState.inputAssembly );
		_SetVertexInputState( OUT vertex_input_info, OUT _bindingDescr, OUT _attribDescr, attribs );
		_SetViewportState( OUT viewport_info, OUT _viewports, OUT _scissors, rpDescr.Subpasses()[subpass] );

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
		pipeline_info.pDynamicState			= dynamicStates.IsZero() ? null : &dynamic_state_info;
		pipeline_info.basePipelineIndex		= -1;
		pipeline_info.basePipelineHandle	= VK_NULL_HANDLE;
		pipeline_info.layout				= layout != VK_NULL_HANDLE ? layout : _emptyLayout;
		pipeline_info.stageCount			= _stages.Count();
		pipeline_info.pStages				= _stages.ptr();
		pipeline_info.renderPass			= renderPass;
		pipeline_info.subpass				= subpass;

		VK_CHECK( vkCreateGraphicsPipelines( dev, _graphicsPipelinesCache, 1, &pipeline_info, null, OUT &pipelineId ) );
		return true;
	}
	
/*
=================================================
	_SetViewportState
=================================================
*/
	void Vk1PipelineCache::_SetViewportState (OUT vk::VkPipelineViewportStateCreateInfo &outState,
											  OUT Viewports_t &tmpViewports,
											  OUT Scissors_t &tmpScissors,
											  const RenderPassDescriptor::Subpass_t &subpass)
	{
		using namespace vk;

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
		outState.viewportCount	= tmpViewports.Count();
		outState.pScissors		= tmpScissors.RawPtr();
		outState.scissorCount	= tmpScissors.Count();
	}

/*
=================================================
	_SetColorBlendState
=================================================
*/
	void Vk1PipelineCache::_SetColorBlendState (OUT vk::VkPipelineColorBlendStateCreateInfo &outState,
												OUT Attachment_t &attachment,
												const RenderState::ColorBuffersState &inState,
												const RenderPassDescriptor::Subpass_t &subpass) const
	{
		using namespace vk;

		const bool	logic_op_enabled	= ( inState.logicOp != ELogicOp::None );
		const usize	count				= Min( subpass.colors.Count(), inState.buffers.Count() );

		for (usize i = 0; i < count; ++i) {
			_SetColorBlendAttachmentState( OUT attachment[i], inState.buffers[i], logic_op_enabled );
		}

		outState.sType				= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		outState.pNext				= null;
		outState.flags				= 0;
		outState.attachmentCount	= (vk::uint32_t) count;
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
	void Vk1PipelineCache::_SetColorBlendAttachmentState (OUT vk::VkPipelineColorBlendAttachmentState &outState,
														  const RenderState::ColorBuffer &inState,
														  bool logicOpEnabled) const
	{
		using namespace vk;

		outState.blendEnable			= inState.blend;
		outState.srcColorBlendFactor	= Vk1Enum( inState.blendFuncSrcColor );
		outState.srcAlphaBlendFactor	= Vk1Enum( inState.blendFuncSrcAlpha );
		outState.dstColorBlendFactor	= Vk1Enum( inState.blendFuncDstColor );
		outState.dstAlphaBlendFactor	= Vk1Enum( inState.blendFuncDstAlpha );
		outState.colorBlendOp			= Vk1Enum( inState.blendModeColor );
		outState.alphaBlendOp			= Vk1Enum( inState.blendModeAlpha );
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
	void Vk1PipelineCache::_SetDepthStencilState (OUT vk::VkPipelineDepthStencilStateCreateInfo &outState,
												  const RenderState::DepthBufferState &depth,
												  const RenderState::StencilBufferState &stencil) const
	{
		using namespace vk;

		float2	range = depth.range.Get( float2(0.0f, 1.0f) );

		outState.sType					= VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		outState.pNext					= null;
		outState.flags					= 0;

		// depth
		outState.depthTestEnable		= depth.test;
		outState.depthWriteEnable		= depth.write;
		outState.depthCompareOp			= Vk1Enum( depth.func );
		outState.depthBoundsTestEnable	= depth.range.IsDefined();
		outState.minDepthBounds			= range.x;
		outState.maxDepthBounds			= range.y;
		
		// stencil
		outState.stencilTestEnable		= stencil.test;
		_SetStencilOpState( OUT outState.front, stencil.front );
		_SetStencilOpState( OUT outState.back,  stencil.back );
	}
	
/*
=================================================
	_SetStencilOpState
=================================================
*/
	void Vk1PipelineCache::_SetStencilOpState (OUT vk::VkStencilOpState &outState,
											   const RenderState::StencilFaceState &inState) const
	{
		using namespace vk;

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
	void Vk1PipelineCache::_SetupPipelineInputAssemblyState (OUT vk::VkPipelineInputAssemblyStateCreateInfo &outState,
															 const RenderState::InputAssemblyState &inState) const
	{
		using namespace vk;

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
	void Vk1PipelineCache::_SetRasterizationState (OUT vk::VkPipelineRasterizationStateCreateInfo &outState,
													const RenderState::RasterizationState &inState) const
	{
		using namespace vk;

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
	void Vk1PipelineCache::_SetMultisampleState (OUT vk::VkPipelineMultisampleStateCreateInfo &outState,
												 const RenderState::MultisampleState &inState) const
	{
		using namespace vk;

		outState.sType					= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		outState.pNext					= null;
		outState.flags					= 0;
		outState.rasterizationSamples	= Vk1Enum( inState.samples );
		outState.sampleShadingEnable	= inState.sampleShading;
		outState.minSampleShading		= inState.minSampleShading;
		outState.pSampleMask			= null;
		outState.alphaToCoverageEnable	= inState.alphaToCoverage;
		outState.alphaToOneEnable		= inState.alphaToOne;
	}
	
/*
=================================================
	_SetTessellationState
=================================================
*/
	void Vk1PipelineCache::_SetTessellationState (OUT vk::VkPipelineTessellationStateCreateInfo &outState,
												  uint patchSize) const
	{
		using namespace vk;

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
	void Vk1PipelineCache::_SetDynamicState (OUT vk::VkPipelineDynamicStateCreateInfo &outState,
											 OUT Vk1DynamicStates &states,
											 const DynamicStates &inState) const
	{
		using namespace vk;

		outState.sType	= VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		outState.pNext	= null;
		outState.flags	= 0;

		uint	j = 0;

		FOR( i, inState )
		{
			auto t = EPipelineDynamicState::type(j);

			if ( inState.Get( t ) )
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
	void Vk1PipelineCache::_SetShaderStages (OUT Array< vk::VkPipelineShaderStageCreateInfo > &stages,
											 OUT Array< vk::VkSpecializationInfo > &specialization,
											 ArrayRef< ShaderModule > shaders) const
	{
		using namespace vk;

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
	void Vk1PipelineCache::_SetVertexInputState (OUT vk::VkPipelineVertexInputStateCreateInfo &outState,
												 OUT Array< vk::VkVertexInputBindingDescription > &bindingDescr,
												 OUT Array< vk::VkVertexInputAttributeDescription > &attribDescr,
												 const VertexAttribs &inState) const
	{
		using namespace vk;

		bindingDescr.Clear();
		attribDescr.Clear();

		outState.sType	= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		outState.pNext	= null;
		outState.flags	= 0;

		FOR( i, inState )
		{
			auto	pair = inState[i];

			// binding description
			if ( pair.second.IsEnabled() )
			{
				VkVertexInputBindingDescription	descr = {};

				descr.binding	= pair.second.index;
				descr.inputRate	= Vk1Enum( pair.second.rate );
				descr.stride	= (vk::uint32_t) pair.second.stride;

				bindingDescr.PushBack( descr );
			}

			// attrib description
			if ( pair.first.IsEnabled() )
			{
				VkVertexInputAttributeDescription descr = {};

				descr.binding	= pair.first.bindingIndex;
				descr.format	= Vk1Enum( pair.first.type );
				descr.location	= pair.first.index;
				descr.offset	= (vk::uint32_t) pair.first.offset;

				attribDescr.PushBack( descr );
			}
		}

		outState.pVertexAttributeDescriptions		= attribDescr.RawPtr();
		outState.vertexAttributeDescriptionCount	= attribDescr.Count();

		outState.pVertexBindingDescriptions			= bindingDescr.RawPtr();
		outState.vertexBindingDescriptionCount		= bindingDescr.Count();
	}
	
//-----------------------------------------------------------------------------


	
	const Runtime::VirtualTypeList	Vk1GraphicsPipeline::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1GraphicsPipeline::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1GraphicsPipeline::Vk1GraphicsPipeline (const GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ GetStaticID(), ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_renderPass( ci.renderPass ),
		_shaders( ci.shaders ),
		_pipelineId( VK_NULL_HANDLE )
	{
		SetDebugName( "Vk1GraphicsPipeline" );

		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_Compose );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_Delete );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GpuDeviceBeforeDestory );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GetVkGraphicsPipelineID );
		_SubscribeOnMsg( this, &Vk1GraphicsPipeline::_GetGraphicsPipelineDescriptor );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, VulkanThread::GetStaticID(), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1GraphicsPipeline::~Vk1GraphicsPipeline ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1GraphicsPipeline::_Compose (const  Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( GetState() == EState::Linked );

		CHECK_ERR( _CreatePipeline() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedImmutable ) );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Vk1GraphicsPipeline::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyPipeline();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetVkGraphicsPipelineID
=================================================
*/
	bool Vk1GraphicsPipeline::_GetVkGraphicsPipelineID (const Message< ModuleMsg::GetVkGraphicsPipelineID > &msg)
	{
		msg->result.Set( _pipelineId );
		return true;
	}
	
/*
=================================================
	_GetGraphicsPipelineDescriptor
=================================================
*/
	bool Vk1GraphicsPipeline::_GetGraphicsPipelineDescriptor (const Message< ModuleMsg::GetGraphicsPipelineDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1GraphicsPipeline::_IsCreated () const
	{
		return _pipelineId != VK_NULL_HANDLE;
	}
	
/*
=================================================
	_CreatePipeline
=================================================
*/
	bool Vk1GraphicsPipeline::_CreatePipeline ()
	{
		using namespace vk;

		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( _GetManager() );

		if ( not _renderPass )
		{
			_renderPass		= GetDevice()->GetDefaultRenderPass();
			_descr.subpass	= 0;

			LOG( "used default render pass", ELog::Debug );
		}

		
		// get render pass id
		Message< ModuleMsg::GetVkRenderPassID >			rp_request;
		Message< ModuleMsg::GetGpuRenderPassDescriptor >	descr_request;

		SendTo( _renderPass, rp_request );
		SendTo( _renderPass, descr_request );

		RenderPassDescriptor	rp_descr;	rp_descr << descr_request->result;
		VkRenderPass			rp			= rp_request->result.Get( VK_NULL_HANDLE );
		CHECK_ERR( rp != VK_NULL_HANDLE );
		

		// get shader modules
		Message< ModuleMsg::GetVkShaderModuleIDs >				shaders_request;
		Array< ModuleMsg::GetVkShaderModuleIDs::ShaderModule >	modules;

		SendTo( _shaders, shaders_request );

		shaders_request->result.MoveTo( OUT modules );
		CHECK_ERR( not modules.Empty() );


		// create graphics pipeline
		CHECK_ERR( _GetManager().ToPtr< VulkanThread >()->GetPipelineCache()->
						CreatePipeline( OUT _pipelineId,
										modules,
										VK_NULL_HANDLE,
										_descr.attribs,
										_descr.renderState,
										_descr.dynamicStates,
										_descr.patchControlPoints,
										rp_descr,
										rp,
										_descr.subpass )
		);
		
		return true;
	}
	
/*
=================================================
	_DestroyPipeline
=================================================
*/
	void Vk1GraphicsPipeline::_DestroyPipeline ()
	{
		using namespace vk;

		auto	dev = GetLogicalDevice();

		if ( dev != VK_NULL_HANDLE and _pipelineId != VK_NULL_HANDLE )
		{
			vkDestroyPipeline( dev, _pipelineId, null );
		}

		_pipelineId	= VK_NULL_HANDLE;
		_descr		= Uninitialized;
	}


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
