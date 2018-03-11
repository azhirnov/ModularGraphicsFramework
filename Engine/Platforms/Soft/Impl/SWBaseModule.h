// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/Platforms/Soft/Impl/SWMessages.h"
#include "Engine/Platforms/Soft/Impl/SWDeviceProperties.h"
#include "Engine/Platforms/Soft/Impl/SWDevice.h"

namespace Engine
{
namespace GpuMsg
{
	//
	// Get Private Classes
	//
	struct GetSWPrivateClasses
	{
		struct Classes {
			PlatformSW::SWDevice *	device	= null;
		};

		Out< Classes >		result;
	};
}	// GpuMsg

namespace PlatformSW
{

	//
	// Software Renderer Base Module
	//

	class SWBaseModule : public Module
	{
	// types
	protected:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >
										::Append< MessageListFrom<
											ModuleMsg::OnManagerChanged,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetSWDeviceInfo,
											GpuMsg::GetSWPrivateClasses
										> >;

		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Link,
											ModuleMsg::AfterLink,
											ModuleMsg::Compose,
											ModuleMsg::AfterCompose,
											ModuleMsg::Delete,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached
										>;

		using EDbgReport	= SWDevice::EDbgReport;


	// variables
	private:
		Ptr< SWDevice >		_swDevice;


	// methods
	protected:
		SWBaseModule  (const GlobalSystemsRef gs,
					   const ModuleConfig &config,
					   const TypeIdList *msgTypes,
					   const TypeIdList *eventTypes);
		
		ModulePtr _GetGPUThread (const ModulePtr &);
		
		Ptr< SWDevice >	GetDevice ()	const	{ return _swDevice; }


	// message handlers
	protected:
		bool _OnManagerChanged (const Message< ModuleMsg::OnManagerChanged > &);
		bool _GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &);
		bool _GetSWDeviceInfo (const Message< GpuMsg::GetSWDeviceInfo > &);
		bool _GetSWPrivateClasses (const Message< GpuMsg::GetSWPrivateClasses > &);
		

	// event handlers
	private:
		bool _DeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);
		bool _DeviceDeleted (const Message< ModuleMsg::Delete > &);


	protected:
		void _DbgReport (StringCRef log, EDbgReport::bits flags, StringCRef file, int line) const;
	};


#	define SW_DEBUG_REPORT( _condition_, _message_, _flags_ ) \
		if ( SWDeviceProperties.vulkanCompatibility ) { \
			if ( not (_condition_) ) { \
				_DbgReport( _message_, _flags_, __FILE__, __LINE__ ); \
			} \
		}

#	define SW_DEBUG_REPORT2( _condition_, _flags_ ) \
		SW_DEBUG_REPORT( (_condition_), GX_TO_ANSI_STRING(_condition_), _flags_ )

}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT
