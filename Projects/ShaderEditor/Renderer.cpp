// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Renderer.h"
#include "Engine/Platforms/Public/Tools/GPUThreadHelper.h"
#include "Engine/ImportExport/Engine.ImportExport.h"
#include "images/vfs_images_main.h"

namespace ShaderEditor
{
	using RenderPassMsgList_t = CompileTime::TypeListFrom< GpuMsg::GetRenderPassDescription >;

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
		CHECK_ERR( _UpdateUBuffer( builder, data, passIdx ) );

		if ( _isCompute )
			return _UpdateCompute( builder, data, passIdx );
		else
			return _UpdateGraphics( builder, data, passIdx );
	}
	
/*
=================================================
	_UpdateGraphics
=================================================
*/
	bool Renderer::Shader::_UpdateGraphics (const ModulePtr &builder, const ShaderData &data, uint passIdx)
	{
		auto&		pass		= _perPass[ passIdx ];
		ModulePtr	render_pass	= pass.framebuffer->GetModuleByMsg< RenderPassMsgList_t >();

		RectU		area		= RectU( 0, 0, pass.viewport.x, pass.viewport.y );

		GpuMsg::CmdClearAttachments	clear;
		clear.attachments.PushBack({ EImageAspect::Color, 0, float4(0.0f) });
		clear.clearRects.PushBack({ area });

		builder->Send( GpuMsg::CmdBeginRenderPass{ render_pass, pass.framebuffer, area });
		builder->Send( clear );
		builder->Send( GpuMsg::CmdBindGraphicsPipeline{ _pipeline });
		builder->Send( GpuMsg::CmdBindGraphicsResourceTable{ pass.resourceTable });
		builder->Send( GpuMsg::CmdSetViewport{ area, float2(0.0f, 1.0f) });
		builder->Send( GpuMsg::CmdSetScissor{ area });
		builder->Send( GpuMsg::CmdDraw{ 4u });
		builder->Send( GpuMsg::CmdEndRenderPass{} );
	
		return true;
	}
	
/*
=================================================
	_UpdateCompute
=================================================
*/
	bool Renderer::Shader::_UpdateCompute (const ModulePtr &builder, const ShaderData &data, uint passIdx)
	{
		auto&	pass = _perPass[ passIdx ];

		//builder->Send( GpuMsg::CmdClearColorImage{ pass.image, EImageLayout::General }.Clear( float4(0.0f) ).AddRange({ EImageAspect::Color, 0_mipmap }) );
		builder->Send( GpuMsg::CmdBindComputePipeline{ _pipeline });
		builder->Send( GpuMsg::CmdBindComputeResourceTable{ pass.resourceTable });
		builder->Send( GpuMsg::CmdDispatch{ uint3( pass.viewport.x, pass.viewport.y, 1 ) });

		return true;
	}

/*
=================================================
	_UpdateUBuffer
=================================================
*/
	bool Renderer::Shader::_UpdateUBuffer (const ModulePtr &builder, const ShaderData &data, const uint passIdx)
	{
		auto&					pass	= _perPass[ passIdx ];
		Pipelines::ShadertoyUB	ub_data	= {};

		FOR( i, ub_data.iChannelResolution )
		{
			ModulePtr	mod = pass.resourceTable->GetModuleByName( "iChannel"_str << i );

			if ( mod )
			{
				GpuMsg::GetImageDescription	req_descr;
				mod->Send( req_descr );

				ub_data.iChannelResolution[i]	= float4( req_descr.result->dimension.To<float2>(), float2() );
				ub_data.iChannelTime[i]			= float4( data.iTime, float3() );
			}
		}

		ub_data.iDate				= data.iDate;
		ub_data.iFrame				= data.iFrame;
		ub_data.iMouse				= data.iMouse;
		ub_data.iResolution			= data.iResolution;
		ub_data.iSampleRate			= data.iSampleRate;
		ub_data.iTime				= data.iTime;
		ub_data.iTimeDelta			= data.iTimeDelta;
		ub_data.iCameraFrustum[0]	= float4(data.iCameraFrustumRay0);
		ub_data.iCameraFrustum[1]	= float4(data.iCameraFrustumRay1);
		ub_data.iCameraFrustum[2]	= float4(data.iCameraFrustumRay2);
		ub_data.iCameraFrustum[3]	= float4(data.iCameraFrustumRay3);
		ub_data.iCameraPos			= float4(data.iCameraPos);

		builder->Send( GpuMsg::CmdUpdateBuffer{ pass.ubuffer, BinArrayCRef::FromValue(ub_data) });
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
			pass.viewport		= uint2();
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
	bool Renderer::Inititalize (float surfaceScale)
	{
		_sufaceScale = surfaceScale;

		// get graphics module ids
		{
			ModulePtr	gthread = PlatformTools::GPUThreadHelper::FindGraphicsThread( _gs );

			GpuMsg::GetGraphicsModules	req_ids;
			CHECK( gthread->Send( req_ids ) );

			_ids		= req_ids.result->graphics;
			_computeIDs	= req_ids.result->compute;
		}

		CHECK_ERR( _CreateSamplers() );

		// create data provider
		{
			ModulePtr	mngr = _gs->mainSystem->GetModuleByID( DataProviderManagerModuleID );

			CHECK_ERR( _gs->modulesFactory->Create(
								BuiltinStorageDataProviderModuleID,
								_gs,
								CreateInfo::BuiltinStorageDataProvider{ &vfs_images::GetBuiltinFileLoader },
								OUT _builtinFileProvider ) );

			ModuleUtils::Initialize({ _builtinFileProvider });

			_gs->parallelThread->Send( ModuleMsg::AttachModule{ _builtinFileProvider });
		}

		// subscribe on input events
		{
			using InputThreadMsgList_t	= CompileTime::TypeListFrom< 
												ModuleMsg::InputKeyBind,
												ModuleMsg::InputMotionBind,
												ModuleMsg::InputKeyUnbindAll,
												ModuleMsg::InputMotionUnbindAll >;

			ModulePtr	input;
			CHECK_ERR( input = _gs->parallelThread->GetModuleByMsg< InputThreadMsgList_t >() );

			input->Send( ModuleMsg::InputMotionBind{ this, &Renderer::_OnMouseX,	"mouse.x"_MotionID });
			input->Send( ModuleMsg::InputMotionBind{ this, &Renderer::_OnMouseY,	"mouse.y"_MotionID });

			input->Send( ModuleMsg::InputKeyBind{ this, &Renderer::_OnMouseLeftButtonDown,	"mouse 0"_KeyID,	EKeyState::OnKeyDown });
			input->Send( ModuleMsg::InputKeyBind{ this, &Renderer::_OnMouseLeftButtonUp,	"mouse 0"_KeyID,	EKeyState::OnKeyUp });
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
	Reset
=================================================
*/
	void Renderer::Reset ()
	{
		_surfaceSize		= uint2();
		_scaledSurfaceSize	= uint2();
		_frameCounter		= 0;
		
		_shaders.Clear();
		_ordered.Clear();
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

			GpuMsg::CreateGraphicsPipeline	create_gpp;

			create_gpp.gpuThread	= PlatformTools::GPUThreadHelper::FindGraphicsThread( _gs );
			create_gpp.moduleID		= _ids.pipeline;
			create_gpp.topology		= EPrimitive::TriangleStrip;
	
			ppln_template->Send( create_gpp );
			_drawTexQuadPipeline = *create_gpp.result;

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

			_resourceTables[i]->Send( ModuleMsg::AttachModule{ "pipeline", _drawTexQuadPipeline });
			_resourceTables[i]->Send( GpuMsg::PipelineAttachTexture{ "un_ColorTexture", iter->second->_perPass[i].image, _nearestClampSampler, EImageLayout::ShaderReadOnlyOptimal });
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
		ModulePtr	gthread = PlatformTools::GPUThreadHelper::FindGraphicsThread( _gs );
		auto		factory = _gs->modulesFactory;

		CHECK_ERR( factory->Create(
						_ids.sampler,
						_gs,
						CreateInfo::GpuSampler{
							SamplerDescription::Builder()
								.SetAddressMode( EAddressMode::Clamp )
								.SetFilter( EFilter::MinMagMipNearest )
								.Finish()
						},
						OUT _nearestClampSampler ) );
		
		CHECK_ERR( factory->Create(
						_ids.sampler,
						_gs,
						CreateInfo::GpuSampler{
							SamplerDescription::Builder()
								.SetAddressMode( EAddressMode::Clamp )
								.SetFilter( EFilter::MinMagMipLinear )
								.Finish()
						},
						OUT _linearClampSampler ) );
		
		CHECK_ERR( factory->Create(
						_ids.sampler,
						_gs,
						CreateInfo::GpuSampler{
							SamplerDescription::Builder()
								.SetAddressMode( EAddressMode::Repeat )
								.SetFilter( EFilter::MinMagMipNearest )
								.Finish()
						},
						OUT _nearestRepeatSampler ) );
		
		CHECK_ERR( factory->Create(
						_ids.sampler,
						_gs,
						CreateInfo::GpuSampler{
							SamplerDescription::Builder()
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
	_CreateCmdBuffer
=================================================
*/
	bool Renderer::_CreateCmdBuffer (const ModulePtr &cmdBuilder)
	{
		if ( _asyncCmdBuilder )
			return true;
		
		CHECK_ERR( _gs->modulesFactory->Create(
						Graphics::AsyncCommandBufferModuleID,
						_gs,
						CreateInfo::AsyncCommandBuffer{ null, cmdBuilder },
						OUT _asyncCmdBuilder ) );

		ModuleUtils::Initialize({ _asyncCmdBuilder });
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

		_surfaceSize		= newSize;
		_scaledSurfaceSize	= RoundToUInt( float2(newSize) * _sufaceScale );

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
		ModulePtr	gthread = PlatformTools::GPUThreadHelper::FindGraphicsThread( _gs );
		auto		factory = _gs->modulesFactory;
	
		// check dependencies
		FOR( i, shader->_descr._channels )
		{
			auto&	ch = shader->_descr._channels[i];

			// find channel in shader passes
			ShadersMap_t::iterator	iter;
			if ( _shaders.Find( ch.first, OUT iter ) )
			{
				if ( iter->second == shader )
					continue;

				FOR( j, iter->second->_perPass )
				{
					auto&	pass = iter->second->_perPass[j];

					// wait until all dependencies will be initialized
					if ( pass.framebuffer.IsNull() or pass.image.IsNull() )
						return false;
				}
				continue;
			}

			// find channel in loadable images
			ModulePtr	mod;
			if ( _LoadImage( ch.first, OUT mod ) )
			{
				continue;
			}

			RETURN_ERR( "unknown channel type, it is not a shader pass and not a file" );
		}

		// load pipeline
		{
			CreateInfo::PipelineTemplate	pp_templ;
			shader->_descr._pplnCtor( OUT pp_templ.descr );

			CHECK_ERR( factory->Create(
							PipelineTemplateModuleID,
							_gs,
							pp_templ,
							OUT shader->_pipelineTemplate ) );

			GpuMsg::GetPipelineTemplateInfo		req_info;
			shader->_pipelineTemplate->Send( req_info );

			shader->_isCompute = req_info.result->shaders[ EShader::Compute ];
		}
		
		FOR( i, shader->_perPass )
		{
			auto&	pass = shader->_perPass[i];

			pass.viewport = newSize;
			
			CHECK_ERR( factory->Create(
							_ids.image,
							_gs,
							CreateInfo::GpuImage{
								ImageDescription{
									EImage::Tex2D,
									uint4( newSize, 0, 0 ),
									EPixelFormat::RGBA8_UNorm,
									EImageUsage::TransferDst | EImageUsage::Storage | EImageUsage::Sampled | EImageUsage::ColorAttachment },
								EGpuMemory::LocalInGPU,
								EMemoryAccess::GpuReadWrite },
							OUT pass.image ) );

			CHECK_ERR( ModuleUtils::Initialize({ pass.image }) );
		}

		// setup compute pipeline
		if ( shader->_isCompute )
		{
			GpuMsg::CreateComputePipeline	create_cpp;
			create_cpp.gpuThread	= gthread;
			create_cpp.moduleID	= _computeIDs.pipeline;
	
			shader->_pipelineTemplate->Send( create_cpp );
			shader->_pipeline = *create_cpp.result;

			CHECK_ERR( shader->_pipeline );
			CHECK_ERR( ModuleUtils::Initialize({ shader->_pipeline }) );

			GpuMsg::GetComputePipelineDescription	req_ppln_descr;
			shader->_pipeline->Send( req_ppln_descr );

			const uint2	kernel_size = req_ppln_descr.result->localGroupSize.xy();

			for (auto& pass : shader->_perPass)
			{
				pass.viewport = (pass.viewport + kernel_size - 1) / kernel_size;
			}
		}
		else
		// setup graphics pipeline
		{
			// create framebuffers
			FOR( i, shader->_perPass )
			{
				auto&	pass = shader->_perPass[i];

				pass.viewport = newSize;

				CHECK_ERR( factory->Create(
								_ids.framebuffer,
								_gs,
								CreateInfo::GpuFramebuffer{ newSize },
								OUT pass.framebuffer ) );
			
				ImageViewDescription		view_descr;
				pass.framebuffer->Send( GpuMsg::FramebufferAttachImage{ "Color0", pass.image, view_descr });

				CHECK_ERR( ModuleUtils::Initialize({ pass.framebuffer }) );
			}

			// create pipeline
			{
				GpuMsg::CreateGraphicsPipeline	create_gpp;
				create_gpp.gpuThread	= gthread;
				create_gpp.moduleID		= _ids.pipeline;
				create_gpp.topology		= EPrimitive::TriangleStrip;
				create_gpp.renderPass	= shader->_perPass.Front().framebuffer->GetModuleByMsg< RenderPassMsgList_t >();
	
				shader->_pipeline = shader->_pipelineTemplate->Request( create_gpp );

				CHECK_ERR( ModuleUtils::Initialize({ shader->_pipeline }) );
			}
		}

		// create uniform buffers
		FOR( i, shader->_perPass )
		{
			auto&	pass = shader->_perPass[i];
			
			CHECK_ERR( factory->Create(
							_ids.buffer,
							_gs,
							CreateInfo::GpuBuffer{
								BufferDescription{ SizeOf<Pipelines::ShadertoyUB>, EBufferUsage::Uniform | EBufferUsage::TransferDst },
								EGpuMemory::LocalInGPU,
								EMemoryAccess::GpuReadWrite },
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

			pass.resourceTable->Send( ModuleMsg::AttachModule{ "pipeline", shader->_pipeline });
			pass.resourceTable->Send( ModuleMsg::AttachModule{ "", pass.ubuffer });

			if ( shader->_isCompute )
			{
				ImageViewDescription	view_descr;
				pass.resourceTable->Send( GpuMsg::PipelineAttachImage{ "un_DstImage", pass.image, view_descr, EImageLayout::General });
			}

			FOR( j, shader->_descr._channels )
			{
				auto&	ch = shader->_descr._channels[j];
				
				// find channel in shader passes
				ShadersMap_t::iterator	iter;
				if ( _shaders.Find( ch.first, OUT iter ) )
				{
					if ( iter->second == shader )
					{
						// use image from previous pass
						pass.resourceTable->Send( GpuMsg::PipelineAttachTexture{ "iChannel"_str << j, shader->_perPass[(i-1) & 1].image, _linearClampSampler });	// TODO: sampler
					}
					else
						pass.resourceTable->Send( GpuMsg::PipelineAttachTexture{ "iChannel"_str << j, iter->second->_perPass[i].image, _linearClampSampler });	// TODO: sampler

					continue;
				}
				
				// find channel in loadable images
				ModulePtr	image_mod;
				if ( _LoadImage( ch.first, OUT image_mod ) )
				{
					pass.resourceTable->Send( GpuMsg::PipelineAttachTexture{ "iChannel"_str << j, image_mod, _linearClampSampler });	// TODO: sampler
					continue;
				}
				
				RETURN_ERR( "unknown channel type, it is not a shader pass and not a file" );
			}
			
			CHECK_ERR( ModuleUtils::Initialize({ pass.resourceTable }) );
		}
		return true;
	}
	
/*
=================================================
	_LoadImage
=================================================
*/
	bool Renderer::_LoadImage (StringCRef filename, OUT ModulePtr &mod)
	{
		String	path = filename;
		FileAddress::FormatPath( path );

		LoadedImages_t::iterator	iter;

		if ( _loadedImages.Find( path, OUT iter ) )
		{
			mod = iter->second;
			return true;
		}

		CHECK_ERR( _asyncCmdBuilder );

		auto	factory = _gs->modulesFactory;

		ModulePtr	data_input;
		CHECK_ERR( factory->Create( 0, _gs, CreateInfo::DataInput{ filename, _builtinFileProvider }, OUT data_input ) );
		
		ModuleUtils::Initialize({ data_input });

		ModulePtr	image;
		CHECK_ERR( factory->Create(
						_ids.image,
						_gs,
						CreateInfo::GpuImage{
							ImageDescription{ EImage::Tex2D, uint4(1), EPixelFormat::RGBA8_UNorm, EImageUsage::Sampled | EImageUsage::TransferDst },
							EGpuMemory::LocalInGPU,
							EMemoryAccess::GpuReadWrite },
						OUT image ) );

		ModulePtr	img_loader;
		CHECK_ERR( factory->Create(
							ImportExport::GXImageLoaderModuleID,
							_gs,
							CreateInfo::ImageLoader{ data_input, EImageLayout::ShaderReadOnlyOptimal },
							OUT img_loader ) );

		image->Send( ModuleMsg::AttachModule{ "loader", img_loader });
		img_loader->Send( ModuleMsg::AttachModule{ "cmd", _asyncCmdBuilder });

		ModuleUtils::Initialize({ image });

		mod = image;
		_loadedImages.Add( path, image );
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
		
		GpuMsg::GetFramebufferDescription	req_descr;
		msg.framebuffers.Front().framebuffer->Send( req_descr );

		ModulePtr	builder = msg.cmdBuilder;

		CHECK_ERR( _CreateCmdBuffer( builder ) );

		// recreate shaders
		if ( Any( _surfaceSize != req_descr.result->size ) )
		{
			CHECK_ERR( _RecreateAll( req_descr.result->size ) );
		}
		
		++_passIdx;

		CHECK( builder->Send( GraphicsMsg::CmdBegin{} ) );

		// draw quad to screen
		FOR( i, msg.framebuffers )
		{
			const uint pass_idx = uint(_passIdx) * msg.framebuffers.Count() + i;
			
			_UpdateShaderData( msg.cameras[i] );

			// run shaders
			FOR( j, _ordered )
			{
				_ordered[j]->Update( builder, _ubData, pass_idx );
			}
		
			CHECK_ERR( msg.framebuffers[i].layer == 0 );	// not supported

			ModulePtr	eye_fb		= msg.framebuffers[i].framebuffer;
			ModulePtr	render_pass	= eye_fb->GetModuleByMsg< RenderPassMsgList_t >();

			GpuMsg::GetFramebufferDescription	fb_descr_request;
			eye_fb->Send( fb_descr_request );

			auto const&	fb_descr	= *fb_descr_request.result;
			RectU		area		= RectU( 0, 0, fb_descr.size.x, fb_descr.size.y );
			
			GpuMsg::CmdClearAttachments	clear;
			clear.attachments.PushBack({ EImageAspect::Color, 0, float4(1.0f) });
			clear.clearRects.PushBack({ area });

			builder->Send( GpuMsg::CmdBeginRenderPass{ render_pass, eye_fb, area });
			builder->Send( clear );
			builder->Send( GpuMsg::CmdBindGraphicsPipeline{ _drawTexQuadPipeline });
			builder->Send( GpuMsg::CmdBindGraphicsResourceTable{ _resourceTables[pass_idx] });
			builder->Send( GpuMsg::CmdSetViewport{ area, float2(0.0f, 1.0f) });
			builder->Send( GpuMsg::CmdSetScissor{ area });
			builder->Send( GpuMsg::CmdDraw{ 4u });
			builder->Send( GpuMsg::CmdEndRenderPass{} );
		}

		CHECK( builder->Send( GraphicsMsg::CmdEnd{} ) );
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

		Date	date;	date.Now();

		_ubData.iResolution			= _scaledSurfaceSize.To<float3>();
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
