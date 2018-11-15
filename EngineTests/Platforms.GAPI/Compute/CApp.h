// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "../Common.h"


class CApp final : public StaticRefCountedObject
{
// types
private:
	using TestFunc_t	= bool (CApp::*) ();
	using TestQueue_t	= Queue< TestFunc_t >;

	using ImageRange	= GpuMsg::ImageRange;


// variables
private:
	Ptr< Module >		ms;
	bool				looping		= true;
	ComputeModuleIDs	gpuIDs;
	
	ModulePtr			gpuThread;
	ModulePtr			cmdQueue;
	ModulePtr			cmdQueue2;
	ModulePtr			syncManager;
	ModulePtr			cmdBuilder;

	TestQueue_t			tests;
	uint				testsPassed	= 0;
	uint				testsFailed	= 0;


// methods
public:
	CApp ();
	~CApp ();

	bool Initialize (GAPI::type api, StringCRef dev, bool debug);
	void Quit ();
	bool Update ();


private:
	bool _OnWindowClosed (const OSMsg::WindowAfterDestroy &);
	bool _Draw (const ModuleMsg::Update &);
	bool _GInit (const GpuMsg::DeviceCreated &);
	bool _GDelete (const GpuMsg::DeviceBeforeDestroy &);

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
	bool _Test_Image2DNearestFilter ();
	bool _Test_Image2DBilinearFilter ();

	// memory
	bool _Test_ExplicitMemoryObjectSharing ();
};
