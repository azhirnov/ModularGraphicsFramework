// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Projects/ShaderEditor/Renderer.h"

namespace ShaderEditor
{
	using RenderPassMsgList_t = CompileTime::TypeListFrom< Message<GpuMsg::GetRenderPassDescriptor> >;

/*
=================================================
	constructor
=================================================
*/
	Renderer::ShaderDescr::ShaderDescr ()
	{}
	
	Renderer::ShaderDescr&  Renderer::ShaderDescr::Pipeline (const PplnCtor_t &value)
	{
		_pplnCtor = value;
		return *this;
	}

	Renderer::ShaderDescr&  Renderer::ShaderDescr::InChannel (StringCRef name, uint index)
	{
		_channels.PushBack({ name, index });
		return *this;
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	Renderer::Shader::Shader (const ShaderDescr &descr) :
		_descr(descr)
	{
	}
	
/*
=================================================
	Update
=================================================
*/
	bool Renderer::Shader::Update (const ModulePtr &builder, const ShaderData &data, const uint passIdx)
	{
		CHECK_ERR( UpdateUBuffer( builder, data, passIdx ) );

		auto&		pass		= _perPass[ passIdx ];
		ModulePtr	render_pass	= pass.framebuffer->GetModuleByMsg< RenderPassMsgList_t >();

		Message< GpuMsg::GetFramebufferDescriptor >	fb_descr_request;
		pass.framebuffer->Send( fb_descr_request );

		auto const&	fb_descr	= fb_descr_request->result.Get();
		RectU		area		= RectU( 0, 0, fb_descr.size.x, fb_descr.size.y );

		GpuMsg::CmdClearAttachments	clear;
		clear.attachments.PushBack({ EImageAspect::bits() | EImageAspect::Color, 0, float4(0.0f) });
		clear.clearRects.PushBack({ area });

		builder->Send< GpuMsg::CmdBeginRenderPass >({ render_pass, pass.framebuffer, area });
		builder->Send< GpuMsg::CmdClearAttachments >( clear );
		builder->Send< GpuMsg::CmdBindGraphicsPipeline >({ _pipeline });
		builder->Send< GpuMsg::CmdBindGraphicsResourceTable >({ pass.resourceTable });
		builder->Send< GpuMsg::CmdSetViewport >({ area, float2(0.0f, 1.0f) });
		builder->Send< GpuMsg::CmdSetScissor >({ area });
		builder->Send< GpuMsg::CmdDraw >({ 4u });
		builder->Send< GpuMsg::CmdEndRenderPass >({});
	
		return true;
	}
	
/*
=================================================
	UpdateUBuffer
=================================================
*/
	bool Renderer::Shader::UpdateUBuffer (const ModulePtr &builder, const ShaderData &data, const uint passIdx)
	{
		auto&					pass	= _perPass[ passIdx ];
		Pipelines::ShadertoyUB	ub_data	= {};

		FOR( i, ub_data.iChannelResolution )
		{
			ModulePtr	mod = pass.resourceTable->GetModuleByName( "iChannel"_str << i );

			if ( mod )
			{
				Message< GpuMsg::GetImageDescriptor >	req_descr;
				mod->Send( req_descr );

				ub_data.iChannelResolution[i]	= float4( req_descr->result->dimension.To<float2>(), float2() );
				ub_data.iChannelTime[i]			= float4( data.iTime, float3() );
			}
		}

		ub_data.iDate		= data.iDate;
		ub_data.iFrame		= data.iFrame;
		ub_data.iMouse		= data.iMouse;
		ub_data.iResolution	= float4( data.iResolution );
		ub_data.iSampleRate	= data.iSampleRate;
		ub_data.iTime		= data.iTime;
		ub_data.iTimeDelta	= data.iTimeDelta;

		builder->Send< GpuMsg::CmdUpdateBuffer >({ pass.ubuffer, ub_data });
		return true;
	}

/*
=================================================
	Destroy
=================================================
*/
	void Renderer::Shader::Destroy ()
	{
		_pipelineTemplate	= null;
		_pipeline			= null;

		FOR( i, _perPass )
		{
			auto&	pass = _perPass[i];

			pass.framebuffer	= null;
			pass.image			= null;
			pass.resourceTable	= null;
			pass.ubuffer		= null;
		}
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	Renderer::Renderer (GlobalSystemsRef gs) :
		_gs{ gs },
		_frameCounter{ 0 }, _passIdx{ 0 }
	{
	}
	
/*
=================================================
	Inititalize
=================================================
*/
	bool Renderer::Inititalize ()
	{
		ModulePtr	gthread = _GetGpuThread();

		Message< GpuMsg::GetGraphicsModules >	req_ids;
		CHECK( gthread->Send( req_ids ) );

		_ids = *req_ids->graphics;

		CHECK_ERR( _CreateCmdBuffers() );
		CHECK_ERR( _CreateSamplers() );
		return true;
	}

/*
=================================================
	Add
=================================================
*/
	bool Renderer::Add (StringCRef name, const ShaderDescr &descr)
	{
		CHECK_ERR( not _shaders.IsExist( name ) );

		_shaders.Add( name, New<Shader>( descr ) );
		return true;
	}
	
/*
=================================================
	_GetGpuThread
=================================================
*/
	ModulePtr Renderer::_GetGpuThread () const
	{
		using ThrdMsgList_t		= CompileTime::TypeListFrom< Message<GpuMsg::ThreadBeginFrame>, Message<GpuMsg::ThreadEndFrame> >;
		using ThrdEventList_t	= CompileTime::TypeListFrom< Message<GpuMsg::DeviceBeforeDestroy> >;

		return _gs->parallelThread->GetModuleByMsgEvent< ThrdMsgList_t, ThrdEventList_t >();
	}

/*
=================================================
	_CreateCmdBuffers
=================================================
*/
	bool Renderer::_CreateCmdBuffers ()
	{
		ModulePtr	gthread = _GetGpuThread();
		auto		factory = _gs->modulesFactory;

		CHECK_ERR( factory->Create(
						_ids.commandBuilder,
						gthread->GlobalSystems(),
						CreateInfo::GpuCommandBuilder{ gthread },
						OUT _cmdBuilder )
		);

		FOR( i, _cmdBuffers )
		{
			CHECK_ERR( factory->Create(
							_ids.commandBuffer,
							gthread->GlobalSystems(),
							CreateInfo::GpuCommandBuffer{
								gthread,
								null,
								CommandBufferDescriptor{ ECmdBufferCreate::ImplicitResetable | ECmdBufferCreate::UseFence }
							},
							OUT _cmdBuffers[i] )
			);
			_cmdBuilder->Send< ModuleMsg::AttachModule >({ ""_str << i, _cmdBuffers[i] });
		}

		ModuleUtils::Initialize({ _cmdBuilder });
		//ModuleUtils::Initialize( _cmdBuffers );
		return true;
	}
	
/*
=================================================
	_CreateDrawTexQuadPipeline
=================================================
*/
	bool Renderer::_CreateDrawTexQuadPipeline ()
	{
		// create pipeline
		{
			ModulePtr	ppln_template;

			CreateInfo::PipelineTemplate	pp_templ;
			Pipelines::Create_drawtexturedquad( OUT pp_templ.descr );

			CHECK_ERR( _gs->modulesFactory->Create(
							PipelineTemplateModuleID,
							_gs,
							pp_templ,
							OUT ppln_template ) );

			Message< GpuMsg::CreateGraphicsPipeline >	create_gpp;

			create_gpp->gpuThread	= _GetGpuThread();
			create_gpp->moduleID	= _ids.pipeline;
			create_gpp->topology	= EPrimitive::TriangleStrip;
	
			ppln_template->Send( create_gpp );
			_drawTexQuadPipeline = create_gpp->result.Get();

			CHECK_ERR( _drawTexQuadPipeline );
			CHECK_ERR( ModuleUtils::Initialize({ _drawTexQuadPipeline }) );
		}

		// create resource table
		ShadersMap_t::iterator	iter;
		CHECK_ERR( _shaders.Find( "main", OUT iter ) );

		FOR( i, _resourceTables )
		{
			CHECK_ERR( _gs->modulesFactory->Create(
							_ids.resourceTable,
							_gs,
							CreateInfo::PipelineResourceTable{},
							OUT _resourceTables[i] ) );

			_resourceTables[i]->Send< ModuleMsg::AttachModule >({ "pipeline", _drawTexQuadPipeline });
			_resourceTables[i]->Send< ModuleMsg::AttachModule >({ "un_ColorTexture", iter->second->_perPass[i].image });
			_resourceTables[i]->Send< ModuleMsg::AttachModule >({ "un_ColorTexture.sampler", _nearestClampSampler });

		}
		CHECK_ERR( ModuleUtils::Initialize( _resourceTables ) );

		return true;
	}

/*
=================================================
	_CreateSamplers
=================================================
*/
	bool Renderer::_CreateSamplers ()
	{
		ModulePtr	gthread = _GetGpuThread();
		auto		factory = _gs->modulesFactory;

		CHECK_ERR( factory->Create(
						_ids.sampler,
						_gs,
						CreateInfo::GpuSampler{
							gthread,
							SamplerDescriptor::Builder()
								.SetAddressMode( EAddressMode::Clamp )
								.SetFilter( EFilter::MinMagMipNearest )
								.Finish()
						},
						OUT _nearestClampSampler ) );
		
		CHECK_ERR( factory->Create(
						_ids.sampler,
						_gs,
						CreateInfo::GpuSampler{
							gthread,
							SamplerDescriptor::Builder()
								.SetAddressMode( EAddressMode::Clamp )
								.SetFilter( EFilter::MinMagMipLinear )
								.Finish()
						},
						OUT _linearClampSampler ) );
		
		CHECK_ERR( factory->Create(
						_ids.sampler,
						_gs,
						CreateInfo::GpuSampler{
							gthread,
							SamplerDescriptor::Builder()
								.SetAddressMode( EAddressMode::Repeat )
								.SetFilter( EFilter::MinMagMipNearest )
								.Finish()
						},
						OUT _nearestRepeatSampler ) );
		
		CHECK_ERR( factory->Create(
						_ids.sampler,
						_gs,
						CreateInfo::GpuSampler{
							gthread,
							SamplerDescriptor::Builder()
								.SetAddressMode( EAddressMode::Repeat )
								.SetFilter( EFilter::MinMagMipLinear )
								.Finish()
						},
						OUT _linearRepeatSampler ) );

		CHECK_ERR( ModuleUtils::Initialize({ _nearestClampSampler, _linearClampSampler, _nearestRepeatSampler, _linearRepeatSampler }) );
		return true;
	}

/*
=================================================
	_RecreateAll
=================================================
*/
	bool Renderer::_RecreateAll (const uint2 &newSize)
	{
		CHECK_ERR( _CreateCmdBuffers() );

		_surfaceSize = newSize;

		Array< ShaderPtr >	sorted;

		// destroy all
		FOR( i, _shaders )
		{
			sorted.PushBack( _shaders[i].second );

			_shaders[i].second->Destroy();
		}

		_ordered.Clear();

		// create all
		while ( not sorted.Empty() )
		{
			FOR( i, sorted )
			{
				if ( _CreateShader( sorted[i], newSize ) )
				{
					_ordered.PushBack( sorted[i] );
					sorted.Erase( i );
					--i;
				}
			}
		}

		CHECK_ERR( _CreateDrawTexQuadPipeline() );
		return true;
	}
	
/*
=================================================
	_CreateShader
=================================================
*/
	bool Renderer::_CreateShader (const ShaderPtr &shader, const uint2 &newSize)
	{
		ModulePtr	gthread = _GetGpuThread();
		auto		factory = _gs->modulesFactory;
	
		// check dependencies
		FOR( i, shader->_descr._channels )
		{
			auto&	ch = shader->_descr._channels[i];

			ShadersMap_t::iterator	iter;
			CHECK_ERR( _shaders.Find( ch.first, OUT iter ) );

			if ( iter->second == shader )
				continue;

			FOR( j, iter->second->_perPass )
			{
				auto&	pass = iter->second->_perPass[j];

				// wait until all dependencies will be initialized
				if ( pass.framebuffer.IsNull() or pass.image.IsNull() )
					return false;
			}
		}

		// create framebuffers
		FOR( i, shader->_perPass )
		{
			auto&	pass = shader->_perPass[i];

			CHECK_ERR( factory->Create(
							_ids.framebuffer,
							_gs,
							CreateInfo::GpuFramebuffer{
								gthread,
								newSize, 1
							},
							OUT pass.framebuffer ) );

			CHECK_ERR( factory->Create(
							_ids.image,
							_gs,
							CreateInfo::GpuImage{
								ImageDescriptor{
									EImage::Tex2D,
									uint4( newSize, 0, 0 ),
									EPixelFormat::RGBA8_UNorm,
									EImageUsage::bits() | EImageUsage::Sampled | EImageUsage::ColorAttachment
								},
								EGpuMemory::bits() | EGpuMemory::LocalInGPU,
								EMemoryAccess::GpuRead | EMemoryAccess::GpuWrite
							},
							OUT pass.image ) );
			
			ImageViewDescriptor		view_descr;
			pass.framebuffer->Send< GpuMsg::FramebufferAttachImage >({ "Color0", pass.image, view_descr });

			CHECK_ERR( ModuleUtils::Initialize({ pass.image, pass.framebuffer }) );
		}

		// create pipeline
		{
			CreateInfo::PipelineTemplate	pp_templ;
			shader->_descr._pplnCtor( OUT pp_templ.descr );

			CHECK_ERR( factory->Create(
							PipelineTemplateModuleID,
							_gs,
							pp_templ,
							OUT shader->_pipelineTemplate ) );

			Message< GpuMsg::CreateGraphicsPipeline >	create_gpp;

			create_gpp->gpuThread	= gthread;
			create_gpp->moduleID	= _ids.pipeline;
			create_gpp->topology	= EPrimitive::TriangleStrip;
			create_gpp->renderPass	= shader->_perPass.Front().framebuffer->GetModuleByMsg< RenderPassMsgList_t >();
	
			shader->_pipelineTemplate->Send( create_gpp );
			shader->_pipeline = create_gpp->result.Get();

			CHECK_ERR( shader->_pipeline );
			CHECK_ERR( ModuleUtils::Initialize({ shader->_pipeline }) );
		}
		
		// create uniform buffers
		FOR( i, shader->_perPass )
		{
			auto&	pass = shader->_perPass[i];
			
			CHECK_ERR( factory->Create(
							_ids.buffer,
							_gs,
							CreateInfo::GpuBuffer{
								BufferDescriptor{
									SizeOf<Pipelines::ShadertoyUB>(),
									EBufferUsage::bits() | EBufferUsage::Uniform | EBufferUsage::TransferDst
								},
								EGpuMemory::bits() | EGpuMemory::LocalInGPU,
								EMemoryAccess::GpuReadWrite
							},
							OUT pass.ubuffer ) );

			CHECK_ERR( ModuleUtils::Initialize({ pass.ubuffer }) );
		}

		// setup pipeline resource table
		FOR( i, shader->_perPass )
		{
			auto&	pass = shader->_perPass[i];

			CHECK_ERR( factory->Create(
							_ids.resourceTable,
							_gs,
							CreateInfo::PipelineResourceTable{},
							OUT pass.resourceTable ) );

			pass.resourceTable->Send< ModuleMsg::AttachModule >({ "pipeline", shader->_pipeline });
			pass.resourceTable->Send< ModuleMsg::AttachModule >({ "", pass.ubuffer });

			FOR( j, shader->_descr._channels )
			{
				auto&	ch = shader->_descr._channels[i];
				
				ShadersMap_t::iterator	iter;
				CHECK_ERR( _shaders.Find( ch.first, OUT iter ) );

				if ( iter->second == shader )
				{
					// use image from previous pass
					pass.resourceTable->Send< ModuleMsg::AttachModule >({ "iChannel"_str << j, shader->_perPass[(i+1) & 1] });
					pass.resourceTable->Send< ModuleMsg::AttachModule >({ "iChannel"_str << j << ".sampler", _linearClampSampler });	// TODO
					continue;
				}
				
				pass.resourceTable->Send< ModuleMsg::AttachModule >({ "iChannel"_str << j, iter->second->_perPass[i] });
				pass.resourceTable->Send< ModuleMsg::AttachModule >({ "iChannel"_str << j << ".sampler", _linearClampSampler });	// TODO
			}
			
			CHECK_ERR( ModuleUtils::Initialize({ pass.resourceTable }) );
		}
		return true;
	}

/*
=================================================
	Update
=================================================
*/
	bool Renderer::Update (const SceneMsg::CameraRequestUpdate &msg)
	{
		CHECK_ERR( msg.framebuffers.Count() == 1 );
		CHECK_ERR( msg.framebuffers.Count() == msg.frustums.Count() );

		Message< GpuMsg::GetFramebufferDescriptor >	req_descr;
		msg.framebuffers.Front()->Send( req_descr );

		if ( Any( _surfaceSize != req_descr->result->size ) )
		{
			CHECK_ERR( _RecreateAll( req_descr->result->size ) );
		}
		
		++_passIdx;
		_UpdateShaderData();

		ModulePtr	cmd_buf = _cmdBuffers[ _passIdx ];
		_cmdBuilder->Send< GpuMsg::CmdBegin >({ cmd_buf });

		// run shaders
		FOR( i, _ordered )
		{
			_ordered[i]->Update( _cmdBuilder, _ubData, _passIdx );
		}
		
		// draw quad to screen
		{
			ModulePtr	system_fb	= msg.framebuffers.Front();
			ModulePtr	render_pass	= system_fb->GetModuleByMsg< RenderPassMsgList_t >();

			Message< GpuMsg::GetFramebufferDescriptor >	fb_descr_request;
			system_fb->Send( fb_descr_request );

			auto const&	fb_descr	= fb_descr_request->result.Get();
			RectU		area		= RectU( 0, 0, fb_descr.size.x, fb_descr.size.y );
			
			GpuMsg::CmdClearAttachments	clear;
			clear.attachments.PushBack({ EImageAspect::bits() | EImageAspect::Color, 0, float4(1.0f) });
			clear.clearRects.PushBack({ area });

			_cmdBuilder->Send< GpuMsg::CmdBeginRenderPass >({ render_pass, system_fb, area });
			_cmdBuilder->Send< GpuMsg::CmdClearAttachments >( clear );
			_cmdBuilder->Send< GpuMsg::CmdBindGraphicsPipeline >({ _drawTexQuadPipeline });
			_cmdBuilder->Send< GpuMsg::CmdBindGraphicsResourceTable >({ _resourceTables[_passIdx] });
			_cmdBuilder->Send< GpuMsg::CmdSetViewport >({ area, float2(0.0f, 1.0f) });
			_cmdBuilder->Send< GpuMsg::CmdSetScissor >({ area });
			_cmdBuilder->Send< GpuMsg::CmdDraw >({ 4u });
			_cmdBuilder->Send< GpuMsg::CmdEndRenderPass >({});
		}

		_cmdBuilder->Send< GpuMsg::CmdEnd >({});

		msg.cmdBuffers.PushBack( cmd_buf );
		return true;
	}
	
/*
=================================================
	_UpdateShaderData
=================================================
*/
	void Renderer::_UpdateShaderData ()
	{
		if ( _frameCounter == 0 )
			_timer.Start();

		_ubData.iResolution = _surfaceSize.To<float3>();
		_ubData.iTime		= float(_timer.GetTimeDelta().Seconds());
		_ubData.iTimeDelta	= float((_timer.GetCurrentTime() - _lastUpdateTime).Seconds());
		_ubData.iFrame		= _frameCounter;
		_ubData.iMouse		= float4();	// TODO
		_ubData.iDate		= float4();	// TODO
		_ubData.iSampleRate	= 0.0f;	// not supported yet

		++_frameCounter;
		_lastUpdateTime = _timer.GetCurrentTime();
	}

}	// ShaderEditor
