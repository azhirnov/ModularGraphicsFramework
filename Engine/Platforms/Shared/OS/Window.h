// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/OS/OSEnums.h"

namespace Engine
{
namespace CreateInfo
{

	//
	// Window Create Info
	//
	struct Window
	{
	// types
		struct EWindowFlags
		{
			enum type : uint
			{
				Fullscreen = 0,
				Resizable,
				_Count,
			};

			using bits = EnumBitfield< EWindowFlags >;
		};
		
		enum class EVisibility
		{
			Visible,		// visible and focused
			Unfocused,		// may be visible
			Invisible,		// minimized
		};

		using EWindowOrientation	= Platforms::EDisplayOrientation;


	// variables
		String						caption;
		EWindowFlags::bits			flags				= EWindowFlags::bits().Set( EWindowFlags::Resizable );
		GXMath::uint2				surfaceSize			= GXMath::uint2( 800, 600 );
		GXMath::int2				position			= MinValue<GXMath::int2>();
		EVisibility					initialVisibility	= EVisibility::Visible;
		EWindowOrientation::type	orientation			= EWindowOrientation::Default;
	};

}	// CreateInfo


namespace Platforms
{

	//
	// Window Descriptor
	//
	struct WindowDesc
	{
	// types
		using EWindowFlags			= CreateInfo::Window::EWindowFlags;
		using EWindowOrientation	= EDisplayOrientation;
		using EVisibility			= CreateInfo::Window::EVisibility;

	// variables
		EWindowFlags::bits			flags;
		uint2						size;
		uint2						surfaceSize;
		int2						position;
		EVisibility					visibility		= EVisibility::Visible;
		EWindowOrientation::type	orientation		= EWindowOrientation::Default;
	};
	
}	// Platforms


namespace ModuleMsg
{

	// platform-dependent
	struct WindowCreated;
	struct WindowGetHandle;
	struct WindowRawMessage;

	struct WindowBeforeDestroy {};
	struct WindowAfterDestroy {};

	
	//
	// Set / Get / OnChanged Window Descriptor
	//
	struct WindowSetDescriptor
	{
		CreateInfo::Window				desc;
	};

	struct WindowGetDescriptor
	{
		Out< Platforms::WindowDesc >	result;
	};

	struct WindowDescriptorChanged
	{
		Platforms::WindowDesc			desc;
	};
	

	//
	// On Window Visibility / Focus Changed
	//
	struct WindowVisibilityChanged
	{
		CreateInfo::Window::EVisibility		state;
	};


	//
	// Before Create Window
	//
	struct WindowBeforeCreate
	{
	// variables
		const	CreateInfo::Window		info;
		mutable	CreateInfo::Window		editable;

	// methods
		WindowBeforeCreate ()
		{}

		WindowBeforeCreate (const WindowBeforeCreate &msg) :
			info(msg.info), editable(msg.editable)
		{}

		explicit
		WindowBeforeCreate (const CreateInfo::Window &info) :
			info(info), editable(info)
		{}
	};


}	// ModuleMsg
}	// Engine
