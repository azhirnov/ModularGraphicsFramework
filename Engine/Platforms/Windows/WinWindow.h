// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/Platforms/Shared/Window.h"
#include "Engine/Platforms/Shared/Display.h"
#include "Engine/Platforms/Shared/Platform.h"

#if defined( PLATFORM_WINDOWS )

namespace Engine
{
namespace Platforms
{

	//
	// Windows Window
	//

	class WinWindow final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Link,
											ModuleMsg::Compose
										> >
										::Append< MessageListFrom<
											ModuleMsg::WindowSetDescriptor,
											ModuleMsg::WindowGetDescriptor,
											ModuleMsg::PlatformCreated,
											ModuleMsg::WindowGetHandle
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::WindowDescriptorChanged,
											ModuleMsg::WindowVisibilityChanged,
											ModuleMsg::WindowBeforeCreate,
											ModuleMsg::WindowCreated,
											ModuleMsg::WindowBeforeDestroy,
											ModuleMsg::WindowAfterDestroy,
											ModuleMsg::WindowRawMessage
										> >;
		
		using Handle_t				= OS::HiddenOSTypeFrom<void*>;
		using EVisibility			= CreateInfo::Window::EVisibility;
		

	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		CreateInfo::Window	_createInfo;

		WindowDesc			_windowDesc;
		
		Handle_t			_wnd;	// HWND

		TimeProfilerD		_timer;

		bool				_requestQuit;
		bool				_looping;


	// methods
	public:
		WinWindow (const SubSystemsRef gs, const CreateInfo::Window &ci);
		~WinWindow ();

		static TModID::type		GetStaticID ()			{ return "win.window"_TModID; }
		

	// message handlers
	private:
		void _Link (const Message< ModuleMsg::Link > &);
		void _Update (const Message< ModuleMsg::Update > &);
		void _Delete (const Message< ModuleMsg::Delete > &);
		void _PlatformCreated (const Message< ModuleMsg::PlatformCreated > &);
		void _PlatformDeleted (const Message< ModuleMsg::Delete > &);
		void _WindowSetDescriptor (const Message< ModuleMsg::WindowSetDescriptor > &);
		void _WindowGetDescriptor (const Message< ModuleMsg::WindowGetDescriptor > &);
		void _WindowGetHandle (const Message< ModuleMsg::WindowGetHandle > &);


	private:
		bool _IsCreated () const;
		bool _Create (const ModulePtr &sender, const ModuleMsg::PlatformCreated &platformInfo);
		bool _CreateWindow (const Display &disp, const CreateInfo::Window &info, StringCRef className, Handle_t instance);
		
		isize _ProcessMessage (uint uMsg, usize wParam, isize lParam);

		void _ShowWindow (EVisibility flags);
		void _StartMessageProc ();
		void _WindowTick ();

		bool _UpdateDescriptor ();
		//void _Resize ();
		void _Destroy ();
	};


}	// Platforms


namespace ModuleMsg
{

	//
	// On Window Created Message
	//
	struct WindowCreated
	{
		Platforms::WindowDesc			descr;
		OS::HiddenOSTypeFrom<void*>		hwnd;
	};
	

	//
	// Request Window Handle
	//
	struct WindowGetHandle
	{
		Out< OS::HiddenOSTypeFrom<void*> >	hwnd;
	};


	//
	// Window Raw Message from OS
	//
	struct WindowRawMessage
	{
		const uint		uMsg			= 0;
		const usize		wParam			= 0;
		const isize		lParam			= 0;
		mutable bool	wasProcessed	= false;

		WindowRawMessage (uint uMsg, usize wParam, isize lParam) :
			uMsg(uMsg), wParam(wParam), lParam(lParam)
		{}
	};

}	// ModuleMsg
}	// Engine

#endif	// PLATFORM_WINDOWS
