// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Graphics/Engine.Graphics.h"

using namespace Engine;
using namespace Engine::Base;
using namespace Engine::Platforms;
using namespace Engine::Graphics;


class GApp final : public StaticRefCountedObject
{
// types
private:
	using RenderPassMsgList_t = CompileTime::TypeListFrom< Message<GpuMsg::GetRenderPassDescriptor> >;


// variables
public:
	Ptr< Module >		ms;

private:
	bool				looping		= true;
	GraphicsModuleIDs	ids;

	ModulePtr			batchRenderer;

	ModulePtr			gpipeline;
	ModulePtr			pipelineTemplate;
	ModulePtr			resourceTable;
	
	ModulePtr			texture;
	ModulePtr			sampler;

	uint				cmdBufIndex	= 0;
	Array<ModulePtr>	cmdBuffers;

	
// methods
public:
	GApp ();

	bool Initialize (GAPI::type api);
	void Quit ();
	bool Update ();
	
	bool _GraphicsInit (const Message< GpuMsg::DeviceCreated > &);
	bool _GraphicsDelete (const Message< GpuMsg::DeviceBeforeDestroy > &);
	bool _Draw (const Message< ModuleMsg::Update > &);
	bool _OnWindowClosed (const Message< OSMsg::WindowAfterDestroy > &);

	bool _CreatePipeline ();
};

