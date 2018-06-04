// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GX_EMULATOR_VR

#include "Engine/Platforms/Public/GPU/VR.h"
#include "Engine/Platforms/Public/GPU/Framebuffer.h"
#include "Engine/Platforms/Public/GPU/RenderPass.h"
#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/OS/Window.h"
#include "Engine/Platforms/Public/OS/Input.h"
#include "Engine/Platforms/VR/VRObjectsConstructor.h"

#include "Engine/STL/Math/3D/PerspectiveCamera.h"

#ifdef GRAPHICS_API_OPENGL
#	include "Engine/Platforms/OpenGL/450/GL4Messages.h"
#	include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#endif

#ifdef GRAPHICS_API_VULKAN
#	include "Engine/Platforms/Vulkan/110/Vk1BaseModule.h"
#endif

namespace Engine
{
namespace PlatformVR
{
	using namespace Engine::Platforms;
	

	//
	// Emulator VR Thread
	//

	class EmulatorVRThread final : public Module
	{
	// types
	private:
		using GThreadMsgList_t		= MessageListFrom<
											GpuMsg::GetGraphicsModules,
											GpuMsg::ThreadBeginFrame,
											GpuMsg::ThreadEndFrame,
											GpuMsg::SubmitGraphicsQueueCommands,
											GpuMsg::SubmitComputeQueueCommands,
											GpuMsg::GetGraphicsSettings
										>;

		using PrivateMsgList_t		= MessageListFrom<
										#ifdef GRAPHICS_API_OPENGL
											GpuMsg::GetGLPrivateClasses,
											GpuMsg::GetGLDeviceInfo,
										#endif
										#ifdef GRAPHICS_API_VULKAN
											GpuMsg::GetVkPrivateClasses,
											GpuMsg::GetVkDeviceInfo,
										#endif
											CompileTime::TypeListEnd
										>;

		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Compose,
											ModuleMsg::Update
										> >::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::OnManagerChanged,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetVRDeviceInfo,
											GpuMsg::ThreadBeginVRFrame,
											GpuMsg::ThreadEndVRFrame
										> >
										::Append< GThreadMsgList_t >
										::Append< PrivateMsgList_t >;

		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::ThreadBeginVRFrame,
											GpuMsg::ThreadEndVRFrame,
											GpuMsg::DeviceCreated,
											GpuMsg::DeviceBeforeDestroy
										> >;
		
		using InputThreadMsgList_t	= MessageListFrom< 
											ModuleMsg::InputKeyBind,
											ModuleMsg::InputMotionBind,
											ModuleMsg::InputKeyUnbindAll,
											ModuleMsg::InputMotionUnbindAll >;

		using WindowEventList_t		= MessageListFrom< OSMsg::WindowCreated, OSMsg::WindowBeforeDestroy >;

		using PerEye_t				= GpuMsg::ThreadBeginVRFrame::PerEye;
		using Framebuffers_t		= StaticArray< PerEye_t, 2 >;
		using Camera_t				= PerspectiveCamera<float>;
		using Transform_t			= Transformation<float>;

		struct PerFrame : CompileTime::FastCopyable
		{
			ModulePtr			command;
			GpuSemaphoreId		mainFrameRendered	= GpuSemaphoreId::Unknown;	// signaled after rendering to eye framebuffers
			GpuSemaphoreId		lastFrameRendered	= GpuSemaphoreId::Unknown;	// signaled after blitting eye framebuffers to screen
		};

		using Commands_t			= FixedSizeArray< PerFrame, 8 >;
		
		enum class Eye {
			Left	= 0,
			Right	= 1,
		};


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		GraphicsSettings	_settings;
		uint2				_eyeTextureDimension;
		
		ModulePtr			_gpuThread;

		ModulePtr			_builder;
		Commands_t			_perFrame;
		uint				_frameIndex;
		uint				_lastFrameIndex;

		Framebuffers_t		_framebuffers;
		ModulePtr			_renderPass;		// default renderpass for eye framebuffer
		ModulePtr			_onScreenFramebuffer;

		Camera_t			_camera;
		float2				_mouseDiff;

		bool				_isCreated;
		bool				_isFrameStarted;
		bool				_isOpenGL;			// this flag indicates what blit command to use


	// methods
	public:
		EmulatorVRThread (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::VRThread &ci);
		~EmulatorVRThread ();

		
	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);

		bool _GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &);
		bool _GetVRDeviceInfo (const Message< GpuMsg::GetVRDeviceInfo > &);
		bool _ThreadBeginVRFrame (const Message< GpuMsg::ThreadBeginVRFrame > &);
		bool _ThreadEndVRFrame (const Message< GpuMsg::ThreadEndVRFrame > &);
		
		bool _GpuDeviceCreated (const Message< GpuMsg::DeviceCreated > &);
		bool _GpuDeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);
		
	#ifdef GRAPHICS_API_OPENGL
		bool _GetGLDeviceInfo_Empty (const Message< GpuMsg::GetGLDeviceInfo > &)			{ return false; }
		bool _GetGLPrivateClasses_Empty (const Message< GpuMsg::GetGLPrivateClasses > &)	{ return false; }
	#endif

	#ifdef GRAPHICS_API_VULKAN
		bool _GetVkDeviceInfo_Empty (const Message< GpuMsg::GetVkDeviceInfo > &)			{ return false; }
		bool _GetVkPrivateClasses_Empty (const Message< GpuMsg::GetVkPrivateClasses > &)	{ return false; }
	#endif

		// event handlers
		void _OnTouchX (const ModuleMsg::InputMotion &);
		void _OnTouchY (const ModuleMsg::InputMotion &);

	private:
		bool _CreateDevice ();
		bool _DestroyDevice ();

		bool _CreateFramebuffers ();
		bool _CreateCommandBuffers ();
		bool _InitFrame (PerFrame &pf, const ModulePtr &cmd);
		bool _PresentVR ();
	};
//-----------------------------------------------------------------------------


	const TypeIdList	EmulatorVRThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	EmulatorVRThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

	static constexpr float3	eyeOffset{ 0.0f, 0.5f, 0.0f };

/*
=================================================
	constructor
=================================================
*/
	EmulatorVRThread::EmulatorVRThread (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::VRThread &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
		_settings( ci.settings ),		_eyeTextureDimension( ci.eyeTextureDimension ),
		_gpuThread( ci.gpuThread ),		_frameIndex( 0 ),				_lastFrameIndex( 0 ),
		_isCreated( false ),			_isFrameStarted( false ),		_isOpenGL( false )
	{
		SetDebugName( "EmulatorVRThread" );
		
		_SubscribeOnMsg( this, &EmulatorVRThread::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &EmulatorVRThread::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &EmulatorVRThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &EmulatorVRThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &EmulatorVRThread::_AddToManager );
		_SubscribeOnMsg( this, &EmulatorVRThread::_RemoveFromManager );
		_SubscribeOnMsg( this, &EmulatorVRThread::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &EmulatorVRThread::_FindModule_Impl );
		_SubscribeOnMsg( this, &EmulatorVRThread::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &EmulatorVRThread::_Link );
		_SubscribeOnMsg( this, &EmulatorVRThread::_Delete );
		_SubscribeOnMsg( this, &EmulatorVRThread::_ThreadBeginVRFrame );
		_SubscribeOnMsg( this, &EmulatorVRThread::_ThreadEndVRFrame );
		_SubscribeOnMsg( this, &EmulatorVRThread::_GetDeviceInfo );
		_SubscribeOnMsg( this, &EmulatorVRThread::_GetVRDeviceInfo );
		
	#ifdef GRAPHICS_API_OPENGL
		_SubscribeOnMsg( this, &EmulatorVRThread::_GetGLDeviceInfo_Empty );
		_SubscribeOnMsg( this, &EmulatorVRThread::_GetGLPrivateClasses_Empty );
	#endif	
	#ifdef GRAPHICS_API_VULKAN
		_SubscribeOnMsg( this, &EmulatorVRThread::_GetVkDeviceInfo_Empty );
		_SubscribeOnMsg( this, &EmulatorVRThread::_GetVkPrivateClasses_Empty );
	#endif

		_frameIndex = 1;
		_perFrame.Resize( 2 );
	}
	
/*
=================================================
	destructor
=================================================
*/
	EmulatorVRThread::~EmulatorVRThread ()
	{
		LOG( "EmulatorVRThread finalized", ELog::Debug );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool EmulatorVRThread::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyDevice();

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool EmulatorVRThread::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );

		// attach to input
		ModulePtr	input;
		CHECK_LINKING( input = GlobalSystems()->parallelThread->GetModuleByMsg< InputThreadMsgList_t >() );

		input->Send< ModuleMsg::InputMotionBind >({ this, &EmulatorVRThread::_OnTouchX, "touch.x"_MotionID });
		input->Send< ModuleMsg::InputMotionBind >({ this, &EmulatorVRThread::_OnTouchY, "touch.y"_MotionID });
		

		// create GPU thread
		if ( not _gpuThread )
		{
			GraphicsSettings	gpu_thread_settings{ _settings };

			gpu_thread_settings.depthStencilFmt	= EPixelFormat::Unknown;
			gpu_thread_settings.flags[GraphicsSettings::EFlags::VSync] = false;

			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								0,
								GlobalSystems(),
								CreateInfo::GpuThread{ gpu_thread_settings },
								OUT _gpuThread
							) );
		}

		_gpuThread->Send( msg );

		_gpuThread->Subscribe( this, &EmulatorVRThread::_GpuDeviceCreated );
		_gpuThread->Subscribe( this, &EmulatorVRThread::_GpuDeviceBeforeDestroy );

		CHECK( _CopySubscriptions< GThreadMsgList_t >( _gpuThread ) );
		CHECK( _CopySubscriptions< PrivateMsgList_t >( _gpuThread, true ) );

		CHECK_ERR( Module::_Link_Impl( msg ) );

		// if window already created
		if ( _IsComposedState( _gpuThread->GetState() ) )
		{
			_GpuDeviceCreated({});
		}
		return true;
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool EmulatorVRThread::_AddToManager (const Message< ModuleMsg::AddToManager > &)
	{
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool EmulatorVRThread::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &)
	{
		return true;
	}
	
/*
=================================================
	_GetDeviceInfo
=================================================
*/
	bool EmulatorVRThread::_GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &msg)
	{
		if ( not _gpuThread )
			return false;

		Message< GpuMsg::GetDeviceInfo >	req_dev;
		CHECK( _gpuThread->Send( req_dev ) );

		req_dev->result->renderPass = _renderPass;

		msg->result.Set( *req_dev->result );
		return true;
	}

/*
=================================================
	_GetVRDeviceInfo
=================================================
*/
	bool EmulatorVRThread::_GetVRDeviceInfo (const Message< GpuMsg::GetVRDeviceInfo > &)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_ThreadBeginVRFrame
=================================================
*/
	bool EmulatorVRThread::_ThreadBeginVRFrame (const Message< GpuMsg::ThreadBeginVRFrame > &msg)
	{
		CHECK_ERR( _isCreated and not _isFrameStarted );
		
		//_mouseDiff *= 0.1f;

		if ( IsNotZero( _mouseDiff ) )
		{
			_camera.RotateFPS( Deg2( _mouseDiff ) );
			_mouseDiff = float2();
		
			_framebuffers[0].projMat = _framebuffers[1].projMat = _camera.ProjMatrix();

			_camera.MoveFPSFree( eyeOffset * -2.0f );
			_framebuffers[0].viewMat = _camera.ViewMatrix() * _camera.GetModelMatrix();
			
			_camera.MoveFPSFree( eyeOffset * 2.0f );
			_framebuffers[1].viewMat = _camera.ViewMatrix() * _camera.GetModelMatrix();
		}

		Message< GpuMsg::ThreadBeginFrame >	begin;
		_gpuThread->Send( begin );

		_lastFrameIndex	= _frameIndex;
		_frameIndex		= begin->result->frameIndex;

		if ( _frameIndex >= _perFrame.Count() )
			_perFrame.Resize( _frameIndex+1 );
		
		Message< GpuMsg::CmdBegin >	cmd_begin;
		cmd_begin->flags			|= ECmdBufferCreate::ImplicitResetable;
		cmd_begin->targetCmdBuffer	 = _perFrame[ _frameIndex ].command;
		_builder->Send( cmd_begin );

		_isFrameStarted		 = true;
		_onScreenFramebuffer = begin->result->framebuffer;

		msg->result.Set({
			_framebuffers[0],
			_framebuffers[1],
			_frameIndex
		});
		return true;
	}
	
/*
=================================================
	_ThreadEndVRFrame
=================================================
*/
	bool EmulatorVRThread::_ThreadEndVRFrame (const Message< GpuMsg::ThreadEndVRFrame > &msg)
	{
		CHECK_ERR( _isCreated and _isFrameStarted );
		
		CHECK_ERR( _PresentVR() );

		auto&	per_frame	= _perFrame[ _frameIndex ];
		auto&	last_frame	= _perFrame[ _lastFrameIndex ];

		Message< GpuMsg::CmdEnd >	cmd_end;
		_builder->Send( cmd_end );
		_InitFrame( per_frame, *cmd_end->result );
		

		// submit frame rendering commands
		Message< GpuMsg::SubmitGraphicsQueueCommands >	submit;
		submit->commands			= msg->commands;
		submit->waitSemaphores		= msg->waitSemaphores;
		submit->signalSemaphores	<< per_frame.mainFrameRendered;
		_gpuThread->Send( submit );


		// submit blitting commands
		Message< GpuMsg::ThreadEndFrame >	end;
		end->commands			<< per_frame.command;
		end->framebuffer		= _onScreenFramebuffer;
		end->fence				= msg->fence;
		end->signalSemaphores	= msg->signalSemaphores;
		end->signalSemaphores	<< per_frame.lastFrameRendered;
		end->waitSemaphores.PushBack({ per_frame.mainFrameRendered, EPipelineStage::AllGraphics });

		if ( last_frame.lastFrameRendered != GpuSemaphoreId::Unknown ) {
			end->waitSemaphores.PushBack({ last_frame.lastFrameRendered, EPipelineStage::AllGraphics });
		}
		_gpuThread->Send( end );


		_isFrameStarted		 = false;
		_onScreenFramebuffer = null;
		return true;
	}

/*
=================================================
	_WindowDescriptorChanged
=================================================
*
	bool EmulatorVRThread::_WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &msg)
	{
		if ( msg->desc.visibility != WindowDesc::EVisibility::Invisible )
		{
			_surfaceSize = msg->desc.surfaceSize;
		}
		return true;
	}

/*
=================================================
	_CreateDevice
=================================================
*/
	bool EmulatorVRThread::_CreateDevice ()
	{
		_isCreated = true;

		CHECK_ERR( _CreateFramebuffers() );
		CHECK_ERR( _CreateCommandBuffers() );

		// initialize camera
		{
			const float2	size{ _eyeTextureDimension };

			_camera.Create( Transform_t(), Rad(90.0_deg), size.x / size.y, float2(0.1f, 100.0f) );	// TODO: use infinite projection
			
			_framebuffers[0].projMat = _framebuffers[1].projMat = _camera.ProjMatrix();

			_camera.MoveFPSFree(-eyeOffset);
			_framebuffers[0].viewMat = _camera.ViewMatrix() * _camera.GetModelMatrix();
			
			_camera.MoveFPSFree(eyeOffset * 2.0f);
			_framebuffers[1].viewMat = _camera.ViewMatrix() * _camera.GetModelMatrix();
		}
		
		CHECK( _DefCompose( false ) );

		_SendEvent< GpuMsg::DeviceCreated >({});
		return true;
	}

/*
=================================================
	_GpuDeviceCreated
=================================================
*/
	bool EmulatorVRThread::_GpuDeviceCreated (const Message< GpuMsg::DeviceCreated > &)
	{
		CHECK_COMPOSING( _CreateDevice() );
		return true;
	}
	
/*
=================================================
	_GpuDeviceBeforeDestroy
=================================================
*/
	bool EmulatorVRThread::_GpuDeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &)
	{
		_DestroyDevice();

		CHECK( _SetState( EState::Initial ) );
		return true;
	}
	
/*
=================================================
	_GpuDeviceBeforeDestroy
=================================================
*/
	bool EmulatorVRThread::_DestroyDevice ()
	{
		Message< ModuleMsg::Delete >	del_msg;

		if ( _gpuThread )
		{
			ModulePtr	temp = _gpuThread;
			
			_gpuThread = null;

			temp->Send( del_msg );
			temp->UnsubscribeAll( this );
		}

		if ( _isCreated )
		{
			_isCreated = false;
			_SendEvent( Message< GpuMsg::DeviceBeforeDestroy >{} );
		}

		FOR( i, _framebuffers )
		{
			if ( _framebuffers[i].framebuffer )
				_framebuffers[i].framebuffer->Send( del_msg );
		}
		_framebuffers.Clear();

		if ( _builder ) {
			_builder->Send( del_msg );
			_builder = null;
		}

		_onScreenFramebuffer = null;
		return true;
	}
	
/*
=================================================
	_PresentVR
=================================================
*/
	bool EmulatorVRThread::_PresentVR ()
	{
		Message< GpuMsg::GetGraphicsSettings >	req_settings;
		_gpuThread->Send( req_settings );

		const uint2		surface_size = req_settings->result->surfaceSize;
		ModulePtr		col_image	 = _onScreenFramebuffer->GetModuleByMsg<MessageListFrom< GpuMsg::GetImageDescriptor >>();

		FOR( i, _framebuffers )
		{
			if ( _isOpenGL )
			{
			#ifdef GRAPHICS_API_OPENGL
				Message< GpuMsg::CmdBlitGLFramebuffers >	blit;
				blit->srcFramebuffer	= _framebuffers[i].framebuffer;
				blit->dstFramebuffer	= _onScreenFramebuffer;
				blit->linearFilter		= true;
				blit->imageAspect		|= EImageAspect::Color;
				blit->regions.PushBack({});
				blit->regions.Back().srcOffset0 = uint2(0);
				blit->regions.Back().srcOffset1 = _eyeTextureDimension;

				switch ( Eye(i) )
				{
				case Eye::Left :
					blit->regions.Back().dstOffset0 = uint2(0);
					blit->regions.Back().dstOffset1 = uint2(surface_size.x / 2, surface_size.y);
					break;

				case Eye::Right :
					blit->regions.Back().dstOffset0 = uint2(surface_size.x / 2, 0);
					blit->regions.Back().dstOffset1 = surface_size;
					break;
				}

				CHECK( _builder->Send( blit ) );
			#endif
			}
			else
			{
				Message< GpuMsg::CmdBlitImage >		blit;
				
				blit->srcImage			= _framebuffers[i].framebuffer->GetModuleByName( "Color0" );
				blit->srcLayout			= EImageLayout::ColorAttachmentOptimal;
				blit->dstImage			= col_image;
				blit->dstLayout			= EImageLayout::ColorAttachmentOptimal;
				blit->linearFilter		= true;
				blit->regions.PushBack({});
				blit->regions.Back().srcOffset0 = uint3(0);
				blit->regions.Back().srcOffset1 = uint3(_eyeTextureDimension, 0);
				blit->regions.Back().srcLayers.aspectMask	|= EImageAspect::Color;
				blit->regions.Back().dstLayers.aspectMask	|= EImageAspect::Color;

				switch ( Eye(i) )
				{
				case Eye::Left :
					blit->regions.Back().dstOffset0 = uint3(0);
					blit->regions.Back().dstOffset1 = uint3(surface_size.x / 2, surface_size.y, 0);
					break;

				case Eye::Right :
					blit->regions.Back().dstOffset0 = uint3(surface_size.x / 2, 0, 0);
					blit->regions.Back().dstOffset1 = uint3(surface_size, 0);
					break;
				}

				CHECK( _builder->Send( blit ) );
			}
		}
		return true;
	}
	
/*
=================================================
	_OnTouch*
=================================================
*/
	void EmulatorVRThread::_OnTouchX (const ModuleMsg::InputMotion &m)
	{
		_mouseDiff.x += m.relative;
	}

	void EmulatorVRThread::_OnTouchY (const ModuleMsg::InputMotion &m)
	{
		_mouseDiff.y += m.relative;
	}
	
/*
=================================================
	_CreateFramebuffers
=================================================
*/
	bool EmulatorVRThread::_CreateFramebuffers ()
	{
		FOR( i, _framebuffers )
		{
			Message< GpuMsg::GetGraphicsModules >	req_ids;
			Message< GpuMsg::GetGraphicsSettings >	req_settings;

			CHECK( _gpuThread->Send( req_ids ) );
			CHECK( _gpuThread->Send( req_settings ) );

			const EImage::type	img_type	= req_settings->result->samples.IsEnabled() ? EImage::Tex2DMS : EImage::Tex2D;

			ModulePtr	framebuffer;
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
										req_ids->graphics->framebuffer,
										GlobalSystems(),
										CreateInfo::GpuFramebuffer{ _eyeTextureDimension },
										OUT framebuffer ) );

			// create color target
			{
				ModulePtr	color_image;
				CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											req_ids->graphics->image,
											GlobalSystems(),
											CreateInfo::GpuImage{
												ImageDescriptor{
													img_type,
													uint4( _eyeTextureDimension, 0, 0 ),
													req_settings->result->colorFmt,
													EImageUsage::ColorAttachment | EImageUsage::TransferSrc | EImageUsage::TransferDst,
													MipmapLevel(1),
													req_settings->result->samples
												},
												EGpuMemory::LocalInGPU,
												EMemoryAccess::GpuReadWrite
											},
											OUT color_image ) );

				framebuffer->Send< GpuMsg::FramebufferAttachImage >({ "Color0", color_image });
			}

			// create depth-stencil target
			if ( req_settings->result->depthStencilFmt != EPixelFormat::Unknown )
			{
				ModulePtr	depth_image;
				CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											req_ids->graphics->image,
											GlobalSystems(),
											CreateInfo::GpuImage{
												ImageDescriptor{
													img_type,
													uint4( _eyeTextureDimension, 0, 0 ),
													req_settings->result->depthStencilFmt,
													EImageUsage::DepthStencilAttachment | EImageUsage::TransferSrc | EImageUsage::TransferDst,
													MipmapLevel(1),
													req_settings->result->samples
												},
												EGpuMemory::LocalInGPU,
												EMemoryAccess::GpuReadWrite
											},
											OUT depth_image ) );

				framebuffer->Send< GpuMsg::FramebufferAttachImage >({ "Depth", depth_image });
			}

			ModuleUtils::Initialize({ framebuffer });

			_framebuffers[i].framebuffer = framebuffer;
			_framebuffers[i].layer		 = 0;
		}

		CHECK_ERR( _Attach( "left_eye_fb",  _framebuffers[0].framebuffer ) );
		CHECK_ERR( _Attach( "right_eye_fb", _framebuffers[1].framebuffer ) );

		#ifdef GRAPHICS_API_OPENGL
		_isOpenGL = _framebuffers[0].framebuffer->GetSupportedMessages().HasType< Message<GpuMsg::GetGLFramebufferID> >();
		#endif

		_renderPass = _framebuffers[0].framebuffer->GetModuleByMsg< MessageListFrom<GpuMsg::GetRenderPassDescriptor> >();
		CHECK_ERR( _renderPass );

		return true;
	}
	
/*
=================================================
	_CreateCommandBuffers
=================================================
*/
	bool EmulatorVRThread::_CreateCommandBuffers ()
	{
		Message< GpuMsg::GetGraphicsModules >	req_ids;
		_gpuThread->Send( req_ids );

		CHECK_ERR( GlobalSystems()->modulesFactory->Create(
									req_ids->graphics->commandBuilder,
									GlobalSystems(),
									CreateInfo::GpuCommandBuilder{},
									OUT _builder ) );

		ModuleUtils::Initialize({ _builder });
		return true;
	}
	
/*
=================================================
	_InitFrame
=================================================
*/
	bool EmulatorVRThread::_InitFrame (PerFrame &perFrame, const ModulePtr &cmd)
	{
		ASSERT( perFrame.command == null or perFrame.command == cmd );

		perFrame.command = cmd;

		// TODO: optimize
		Message< GpuMsg::GetDeviceInfo >	req_dev;
		_gpuThread->Send( req_dev );

		if ( perFrame.mainFrameRendered == GpuSemaphoreId::Unknown )
		{
			Message< GpuMsg::CreateSemaphore >	sem_ctor;
			req_dev->result->syncManager->Send( sem_ctor );

			perFrame.mainFrameRendered = *sem_ctor->result;
		}

		if ( perFrame.lastFrameRendered == GpuSemaphoreId::Unknown )
		{
			Message< GpuMsg::CreateSemaphore >	sem_ctor;
			req_dev->result->syncManager->Send( sem_ctor );

			perFrame.lastFrameRendered = *sem_ctor->result;
		}

		return true;
	}

}	// PlatformVR
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VRObjectsConstructor::CreateEmulatorVRThread (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::VRThread &ci)
	{
		return New< PlatformVR::EmulatorVRThread >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GX_EMULATOR_VR
