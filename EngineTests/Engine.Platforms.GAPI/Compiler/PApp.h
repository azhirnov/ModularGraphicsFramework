// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "../Common.h"


class PApp final : public StaticRefCountedObject
{
// types
private:
	using TestFunc_t	= bool (PApp::*) ();
	using TestQueue_t	= Queue< TestFunc_t >;


// variables
private:
	Ptr< Module >		ms;
	bool				looping		= true;
	ComputeModuleIDs	gpuIDs;
	
	ModulePtr			gpuThread;
	ModulePtr			syncManager;
	ModulePtr			cmdBuilder;

	TestQueue_t			tests;
	uint				testsPassed	= 0;
	uint				testsFailed	= 0;


// methods
public:
	PApp ();
	~PApp ();

	bool Initialize (GAPI::type api, StringCRef dev);
	void Quit ();
	bool Update ();


private:
	bool _OnWindowClosed (const OSMsg::WindowAfterDestroy &);
	bool _Draw (const ModuleMsg::Update &);
	bool _GInit (const GpuMsg::DeviceCreated &);
	bool _GDelete (const GpuMsg::DeviceBeforeDestroy &);

// Tests
private:
	// shader functions
	bool _Test_FindMSB ();			// emulated in OpenCL
	bool _Test_FindLSB ();			// emulated in OpenCL
	bool _Test_BitfieldReverse ();	// emulated in OpenCL
	bool _Test_AtomicAdd ();

	bool _Test_InlineAll ();		// OpenCL, C++
	bool _Test_VecSwizzle ();		// C++
	bool _Test_GlobalToLocal ();	// OpenCL
	bool _Test_UnnamedBuffer ();	// OpenCL, C++
	
	bool _Test_Include ();
};
