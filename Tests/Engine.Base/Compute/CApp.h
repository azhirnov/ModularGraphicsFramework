// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "../Common.h"


class CApp final : public StaticRefCountedObject
{
// types
private:
	using RenderPassMsgList_t = CompileTime::TypeListFrom< Message<GpuMsg::GetRenderPassDescriptor> >;
	
	struct Vertex;


// variables
public:
	Ptr< Module >		ms;

private:
	bool				looping		= true;
	uint				cmdBufIndex	= 0;
	ComputeModuleIDs	ids;

	ModulePtr			syncManager;

	ModulePtr			cpipeline;
	ModulePtr			pipelineTemplate;
	ModulePtr			resourceTable;

	ModulePtr			buffer;
	ModulePtr			image;

	Array<ModulePtr>	cmdBuffers;
	Array<GpuFenceId>	cmdFence;
	ModulePtr			cmdBuilder;


// methods
public:
	CApp ();
	bool Initialize (GAPI::type api);
	void Quit ();
	bool Update ();


private:
	bool _OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &);
	bool _Draw (const Message< ModuleMsg::Update > &);
	bool _GInit (const Message< GpuMsg::DeviceCreated > &);
	bool _GDelete (const Message< GpuMsg::DeviceBeforeDestroy > &);
	
	bool _CreatePipeline ();
	bool _CreateCmdBuffers ();

	static ModulePtr GetGThread (GlobalSystemsRef gs);
};
