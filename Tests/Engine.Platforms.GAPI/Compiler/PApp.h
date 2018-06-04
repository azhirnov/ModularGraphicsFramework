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
public:
	Ptr< Module >		ms;

private:
	bool				looping		= true;
	
	ModulePtr			gpuThread;
	ModulePtr			syncManager;
	ModulePtr			cmdBuilder;
	ComputeModuleIDs	gpuIDs;

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
	bool _OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &);
	bool _Draw (const Message< ModuleMsg::Update > &);
	bool _GInit (const Message< GpuMsg::DeviceCreated > &);
	bool _GDelete (const Message< GpuMsg::DeviceBeforeDestroy > &);

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
};
