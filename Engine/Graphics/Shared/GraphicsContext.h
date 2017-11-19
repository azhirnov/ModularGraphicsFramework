// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

//#include "Engine/Graphics/2D/Font.h"
#include "Engine/Graphics/BatchRenderer/BatchRenderer.h"

namespace Engine
{
namespace CreateInfo
{

	//
	// Graphics Context
	//
	struct GraphicsContext
	{
	// types
		using GAPI		= Platforms::GAPI;
		using EFlags	= Platforms::GraphicsSettings::EFlags;

	// variables
		Platforms::GraphicsSettings		settings;	// this is default settings for all threads

	// methods
		explicit GraphicsContext (GAPI::type version = GAPI::type(0)) : settings{ version } {}
		explicit GraphicsContext (const Platforms::GraphicsSettings &settings) : settings{ settings } {}

		explicit GraphicsContext (const Platforms::ComputeSettings &settings) :
			settings{ settings.version, EFlags::bits() | (settings.isDebug ? EFlags::DebugContext : EFlags::type(0)),
					  Platforms::EPixelFormat::Unknown, Platforms::EPixelFormat::Unknown,
					  Platforms::MultiSamples(), settings.device, 0 }
		{}
	};


	//
	// Graphics Thread
	//
	struct GraphicsThread
	{
	// variables
		ModulePtr				context;		// can be null
		GpuThread				gpuThread;
		Optional< Window >		window;			// if defined window will be created

		// TODO
		//UntypedID_t				keyInputID		= Platforms::WinKeyInputModuleID;
		//UntypedID_t				mouseInputID	= Platforms::WinMouseInputModuleID;

	// methods
		GraphicsThread (const ModulePtr &ctx, const Platforms::GraphicsSettings &settings, const Window &wnd) :
			context(ctx), gpuThread(settings), window(wnd)
		{}
	};

}	// CreateInfo


namespace Graphics
{

	//
	// Graphics Context
	//
	
	class GraphicsContext final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;

		using GraphicsThreads_t		= Set< ModulePtr >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		GraphicsThreads_t		_threads;
		

	// methods
	public:
		GraphicsContext (GlobalSystemsRef gs, const CreateInfo::GraphicsContext &ci);
		~GraphicsContext ();
		
		friend void RegisterGraphics ();
		static void Register (GlobalSystemsRef);
		static void Unregister (GlobalSystemsRef);

		
	// message handlers
	private:
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		
	private:
		//static ModulePtr _CreateFont (GlobalSystemsRef, const CreateInfo::Font &);
		//static ModulePtr _CreateFontManager (GlobalSystemsRef, const CreateInfo::FontManager &);
		static ModulePtr _CreateBatchRenderer (GlobalSystemsRef, const CreateInfo::BatchRenderer &);
		static ModulePtr _CreateGraphicsThread (GlobalSystemsRef, const CreateInfo::GraphicsThread &);
		static ModulePtr _CreateGraphicsContext (GlobalSystemsRef, const CreateInfo::GraphicsContext &);
	};


}	// Graphics
}	// Engine
