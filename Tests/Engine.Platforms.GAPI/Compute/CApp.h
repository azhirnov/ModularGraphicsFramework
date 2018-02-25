// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "../Common.h"


class CApp final : public StaticRefCountedObject
{
// types
private:
	using TestFunc_t	= bool (CApp::*) ();
	using TestQueue_t	= Queue< TestFunc_t >;


// variables
public:
	Ptr< Module >		ms;

private:
	bool				looping		= true;
	uint				cmdBufIndex	= 0;
	ComputeModuleIDs	gpuIDs;

	ModulePtr			gpuThread;
	ModulePtr			syncManager;
	ModulePtr			cmdBuilder;

	TestQueue_t			tests;


// methods
public:
	CApp ();
	~CApp ();

	bool Initialize (GAPI::type api);
	void Quit ();
	bool Update ();


private:
	bool _OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &);
	bool _Draw (const Message< ModuleMsg::Update > &);
	bool _GInit (const Message< GpuMsg::DeviceCreated > &);
	bool _GDelete (const Message< GpuMsg::DeviceBeforeDestroy > &);

// Tests
private:
	bool _Test_CopyBuffer ();
	bool _Test_CopyImage2D ();
	bool _Test_CopyImage3D ();
	bool _Test_UpdateBuffer ();
	bool _Test_PushConstants ();
};
