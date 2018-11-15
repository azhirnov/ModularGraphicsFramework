// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "../Common.h"


class SApp final : public StaticRefCountedObject
{
// types
private:
	using TestFunc_t	= bool (SApp::*) ();
	using TestQueue_t	= Queue< TestFunc_t >;

	using ImageRange	= GpuMsg::ImageRange;

	struct GPU
	{
		ModulePtr			gpuThread;
		ModulePtr			syncManager;
		ModulePtr			cmdBuilder;
		ComputeModuleIDs	gpuIDs;
	};


// variables
private:
	Ptr< Module >		ms;
	bool				looping					= true;
	bool				sharedThreadInitialized	= false;
	bool				debugContext			= false;
	ComputeModuleIDs	gpuIDs;
	
	ModulePtr			gpuThread;
	ModulePtr			syncManager;
	ModulePtr			cmdBuilder;

	struct
	{
		GAPI::type			api				= Uninitialized;
		ModulePtr			gpuThread;
		ModulePtr			syncManager;
		ModulePtr			cmdBuilder;
		ComputeModuleIDs	gpuIDs;

	}					sharedGpu;

	TestQueue_t			tests;
	uint				testsPassed	= 0;
	uint				testsFailed	= 0;


// methods
public:
	SApp ();
	~SApp ();

	bool Initialize (GAPI::type api, GAPI::type sharedApi, StringCRef dev, bool debug);
	void Quit ();
	bool Update ();


private:
	bool _OnWindowClosed (const OSMsg::WindowAfterDestroy &);

	bool _Draw (const ModuleMsg::Update &);
	bool _GInit (const GpuMsg::DeviceCreated &);
	bool _GDelete (const GpuMsg::DeviceBeforeDestroy &);
	
	bool _GInit2 (const GpuMsg::DeviceCreated &);
	bool _GDelete2 (const GpuMsg::DeviceBeforeDestroy &);

	bool _CreateMainGPU (GAPI::type api, StringCRef dev);
	bool _CreateSharedGPU (GAPI::type api, StringCRef dev);


// Tests
private:
	// buffer
	bool _Test_CopyBuffer ();
	bool _Test_ClearBuffer ();
	bool _Test_UpdateBuffer ();
	bool _Test_BufferAlign ();
	bool _Test_DynamicBuffer ();
	bool _Test_BufferRange ();
	bool _Test_SpecializationConstants ();
	bool _Test_ShaderBarrier ();
	//bool _Test_PushConstants ();

	// image
	bool _Test_CopyImage2D ();
	bool _Test_CopyBufferToImage2D ();
	bool _Test_CopyImage2DToBuffer ();
	bool _Test_ConvertFloatImage2D ();
	//bool _Test_CopyImage3D ();

	// shader functions
	bool _Test_FindMSB ();			// emulated in OpenCL
	bool _Test_FindLSB ();			// emulated in OpenCL
	bool _Test_BitfieldReverse ();	// emulated in OpenCL
	bool _Test_AtomicAdd ();

	// sharing
	bool _Test_BufferSharing ();
	bool _Test_ImageSharing ();
};
