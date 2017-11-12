// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "../Common.h"
#include "Engine/Platforms/OpenGL/Impl/gl4.h"

class GLApp final : public StaticRefCountedObject
{
// types
private:
	using RenderPassMsgList_t = CompileTime::TypeListFrom< Message<GpuMsg::GetRenderPassDescriptor> >;


// variables
public:
	Ptr< Module >		ms;

private:
	bool				looping	= true;
	uint				cmdBufIndex	= 0;
	
	// triangle
	ModulePtr			gpipeline1;
	ModulePtr			pipelineTemplate1;
	ModulePtr			resourceTable1;

	// quad
	ModulePtr			gpipeline2;
	ModulePtr			pipelineTemplate2;
	ModulePtr			resourceTable2;

	ModulePtr			framebuffer;
	ModulePtr			fbColorImage;

	ModulePtr			texture;
	ModulePtr			sampler;

	ModulePtr			vbuffer;
	ModulePtr			ibuffer;
	ModulePtr			ubuffer;
	
	Array<ModulePtr>	cmdBuffers;
	ModulePtr			cmdBuilder;


// methods
public:
	GLApp ();

	void Initialize ();
	void Quit ();
	bool Update ();

private:
	bool _OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &);
	bool _OnKey (const Message< ModuleMsg::InputKey > &msg);
	bool _OnMotion (const Message< ModuleMsg::InputMotion > &msg);
	bool _Draw (const Message< ModuleMsg::Update > &msg);

	bool _GLInit (const Message< GpuMsg::DeviceCreated > &msg);
	bool _GLDelete (const Message< GpuMsg::DeviceBeforeDestroy > &msg);

	bool _CreatePipeline1 ();
	bool _CreatePipeline2 ();
	bool _CreateCmdBuffers ();
};
