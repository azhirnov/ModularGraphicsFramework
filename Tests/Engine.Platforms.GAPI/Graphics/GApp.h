// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "../Common.h"


class GApp final : public StaticRefCountedObject
{
// types
private:
	using TestFunc_t	= bool (GApp::*) ();
	using TestQueue_t	= Queue< TestFunc_t >;


// variables
public:
	Ptr< Module >		ms;

private:
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
	bool Initialize (GAPI::type api, StringCRef device);
	void Quit ();
	bool Update ();


private:
	bool _OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &);
	bool _Draw (const Message< ModuleMsg::Update > &);
	bool _GInit (const Message< GpuMsg::DeviceCreated > &);
	bool _GDelete (const Message< GpuMsg::DeviceBeforeDestroy > &);
	
// Tests
private:

	// texture
	bool _Test_Texture2DBilinearFilter ();
	bool _Test_Texture2DNearestFilter ();
};
