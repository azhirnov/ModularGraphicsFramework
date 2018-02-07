// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		STATIC_ASSERT( _perPass.Count() == MAX_PASSES );
	}
	
/*
=================================================
	Update
=================================================
*/
	bool Renderer::Shader::Update (const ModulePtr &builder, const ShaderData &data, const uint passIdx)
	{
		CHECK_ERR( passIdx < _perPass.Count() );
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

		ub_data.iDate				= data.iDate;
		ub_data.iFrame				= data.iFrame;
		ub_data.iMouse				= data.iMouse;
		ub_data.iResolution			= float4( data.iResolution );
		ub_data.iSampleRate			= data.iSampleRate;
		ub_data.iTime				= data.iTime;
		ub_data.iTimeDelta			= data.iTimeDelta;
		ub_data.iCameraFrustum[0]	= float4(data.iCameraFrustumRay0);
		ub_data.iCameraFrustum[1]	= float4(data.iCameraFrustumRay1);
		ub_data.iCameraFrustum[2]	= float4(data.iCameraFrustumRay2);
		ub_data.iCameraFrustum[3]	= float4(data.iCameraFrustumRay3);
		ub_data.iCameraPos			= float4(data.iCameraPos);

		builder->Send< GpuMsg::CmdUpdateBuffer >({ pass.ubuffer, BinArrayCRef::FromValue(ub_data) });
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
		_gs{ gs },		_frameCounter{ 0 },
		_ubData{},		_passIdx{ 0 }
	{
	}
	
/*
=================================================
	Inititalize
=================================================
*/
	bool Renderer::Inititalize ()
	{
		// get graphics module ids
		{
			ModulePtr	gthread = _GetGpuThread();

			Message< GpuMsg::GetGraphicsModules >	req_ids;
			CHECK( gthread->Send( req_ids ) );

			_ids = *req_ids->graphics;
		}

		CHECK_ERR( _CreateSamplers() );
		
		// subscribe on input events
		{
			using InputThreadMsgList_t	= CompileTime::TypeListFrom< 
												Message< ModuleMsg::InputKeyBind >,
												Message< ModuleMsg::InputMotionBind >,
												Message< ModuleMsg::InputKeyUnbindAll >,
												Message< ModuleMsg::InputMotionUnbindAll > >;

			ModulePtr	input;
			CHECK_ERR( input = _gs->parallelThread->GetModuleByMsg< InputThreadMsgList_t >() );

			input->Send< ModuleMsg::InputMotionBind >({ this, &Renderer::_OnMouseX,	"mouse.x"_MotionID });
			input->Send< ModuleMsg::InputMotionBind >({ this, &Renderer::_OnMouseY,	"mouse.y"_MotionID });

			input->Send< ModuleMsg::InputKeyBind >({ this, &Renderer::_OnMouseLeftButtonDown,	"mouse 0"_KeyID,	EKeyState::OnKeyDown });
			input->Send< ModuleMsg::InputKeyBind >({ this, &Renderer::_OnMouseLeftButtonUp,		"mouse 0"_KeyID,	EKeyState::OnKeyUp });
		}
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
		// TODO: delete command buffers?

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
				auto&	ch = shader->_descr._channels[j];
				
				ShadersMap_t::iterator	iter;
				CHECK_ERR( _shaders.Find( ch.first, OUT iter ) );

				if ( iter->second == shader )
				{
					// use image from previous pass
					pass.resourceTable->Send< ModuleMsg::AttachModule >({ "iChannel"_str << j, shader->_perPass[(i-1) & 1].image });
					pass.resourceTable->Send< ModuleMsg::AttachModule >({ "iChannel"_str << j << ".sampler", _linearClampSampler });	// TODO
					continue;
				}
				
				pass.resourceTable->Send< ModuleMsg::AttachModule >({ "iChannel"_str << j, iter->second->_perPass[i].image });
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
		CHECK_ERR( msg.framebuffers.Count() == 1 or msg.framebuffers.Count() == 2 );
		CHECK_ERR( msg.framebuffers.Count() == msg.cameras.Count() );
		
		Message< GpuMsg::GetFramebufferDescriptor >	req_descr;
		msg.framebuffers.Front().framebuffer->Send( req_descr );

		if ( Any( _surfaceSize != req_descr->result->size ) )
		{
			CHECK_ERR( _RecreateAll( req_descr->result->size ) );
		}
		
		++_passIdx;

		ModulePtr	builder = msg.cmdBuilder;
		CHECK( builder->Send< GraphicsMsg::CmdBegin >({}) );

		// draw quad to screen
		FOR( i, msg.framebuffers )
		{
			const uint pass_idx = _passIdx * msg.framebuffers.Count() + i;
			
			_UpdateShaderData( msg.cameras[i] );

			// run shaders
			FOR( j, _ordered )
			{
				_ordered[j]->Update( builder, _ubData, pass_idx );
			}
		
			CHECK_ERR( msg.framebuffers[i].layer == 0 );	// not supported

			ModulePtr	eye_fb		= msg.framebuffers[i].framebuffer;
			ModulePtr	render_pass	= eye_fb->GetModuleByMsg< RenderPassMsgList_t >();

			Message< GpuMsg::GetFramebufferDescriptor >	fb_descr_request;
			eye_fb->Send( fb_descr_request );

			auto const&	fb_descr	= fb_descr_request->result.Get();
			RectU		area		= RectU( 0, 0, fb_descr.size.x, fb_descr.size.y );
			
			GpuMsg::CmdClearAttachments	clear;
			clear.attachments.PushBack({ EImageAspect::bits() | EImageAspect::Color, 0, float4(1.0f) });
			clear.clearRects.PushBack({ area });

			builder->Send< GpuMsg::CmdBeginRenderPass >({ render_pass, eye_fb, area });
			builder->Send< GpuMsg::CmdClearAttachments >( clear );
			builder->Send< GpuMsg::CmdBindGraphicsPipeline >({ _drawTexQuadPipeline });
			builder->Send< GpuMsg::CmdBindGraphicsResourceTable >({ _resourceTables[pass_idx] });
			builder->Send< GpuMsg::CmdSetViewport >({ area, float2(0.0f, 1.0f) });
			builder->Send< GpuMsg::CmdSetScissor >({ area });
			builder->Send< GpuMsg::CmdDraw >({ 4u });
			builder->Send< GpuMsg::CmdEndRenderPass >({});
		}

		CHECK( builder->Send< GraphicsMsg::CmdEnd >({}) );
		return true;
	}

/*
=================================================
	_UpdateShaderData
=================================================
*/
	void Renderer::_UpdateShaderData (const SceneMsg::CameraGetState::State &state)
	{
		if ( _frameCounter == 0 )
			_timer.Start();

		OS::Date	date;	date.Now();

		_ubData.iResolution			= _surfaceSize.To<float3>();
		_ubData.iTime				= float(_timer.GetTimeDelta().Seconds());
		_ubData.iTimeDelta			= float((_timer.GetCurrentTime() - _lastUpdateTime).Seconds());
		_ubData.iFrame				= _frameCounter;
		_ubData.iMouse				= float4( _mousePressed ? _mousePos : float2(), float2() );	// TODO: click
		_ubData.iDate				= float4(uint3( date.Year(), date.Month(), date.DayOfMonth()).To<float3>(), float(date.Second()) + float(date.Milliseconds()) * 0.001f);
		_ubData.iSampleRate			= 0.0f;	// not supported yet
		_ubData.iCameraPos			= state.transform.Position();

		state.frustum.GetRays( OUT _ubData.iCameraFrustumRay2, OUT _ubData.iCameraFrustumRay0,
							   OUT _ubData.iCameraFrustumRay3, OUT _ubData.iCameraFrustumRay1 );

		++_frameCounter;
		_lastUpdateTime = _timer.GetCurrentTime();
	}
	
/*
=================================================
	_OnMouse*
=================================================
*/
	void Renderer::_OnMouseX (const ModuleMsg::InputMotion &m)
	{
		_mousePos.x = m.absolute;
	}

	void Renderer::_OnMouseY (const ModuleMsg::InputMotion &m)
	{
		_mousePos.y = m.absolute;
	}
	
	void Renderer::_OnMouseLeftButtonDown (const ModuleMsg::InputKey &)
	{
		//_mousePressed = true;
	}

	void Renderer::_OnMouseLeftButtonUp (const ModuleMsg::InputKey &)
	{
		//_mousePressed = false;
	}

}	// ShaderEditor
