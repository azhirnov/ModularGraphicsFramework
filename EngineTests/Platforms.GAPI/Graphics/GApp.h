// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "../Common.h"


class GApp final : public StaticRefCountedObject
{
// types
private:
	using TestFunc_t	= bool (GApp::*) ();
	using TestQueue_t	= Queue< TestFunc_t >;

	using ImageRange	= GpuMsg::ImageRange;


// variables
private:
	Ptr< Module >		ms;
	bool				looping		= true;
	GraphicsModuleIDs	gpuIDs;
	ComputeModuleIDs	computeIDs;

	ModulePtr			syncManager;
	ModulePtr			gpuThread;
	ModulePtr			cmdBuilder;
	
	TestQueue_t			tests;


// methods
public:
	GApp ();
	bool Initialize (GAPI::type api, StringCRef device, bool debug);
	void Quit ();
	bool Update ();


private:
	bool _OnWindowClosed (const OSMsg::WindowAfterDestroy &);
	bool _Draw (const ModuleMsg::Update &);
	bool _GInit (const GpuMsg::DeviceCreated &);
	bool _GDelete (const GpuMsg::DeviceBeforeDestroy &);
	
// Tests
private:

	// texture
	bool _Test_Texture2DBilinearFilter ();
	bool _Test_Texture2DNearestFilter ();
};
