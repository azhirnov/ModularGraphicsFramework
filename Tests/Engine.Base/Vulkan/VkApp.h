// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "../Common.h"


class VkApp final : public StaticRefCountedObject
{
// variables
public:
	Ptr< Module >		ms;

private:
	bool				looping		= true;
	uint				cmdBufIndex	= 0;

	ModulePtr			graphicsPipeline;
	ModulePtr			pipelineTemplate;
	ModulePtr			resourceTable;
	ModulePtr			texture;
	ModulePtr			sampler;
	Array<ModulePtr>	cmdBuffers;
	ModulePtr			cmdBuilder;


// methods
public:
	VkApp ();
	void Initialize ();
	void Quit ();
	bool Update ();


private:
	bool _OnWindowClosed (const Message<ModuleMsg::WindowAfterDestroy> &);
	bool _OnKey (const Message< ModuleMsg::InputKey > &);
	bool _OnMotion (const Message< ModuleMsg::InputMotion > &);
	bool _Draw (const Message< ModuleMsg::Update > &);
	bool _VkInit (const Message< GpuMsg::DeviceCreated > &);
	bool _VkDelete (const Message< GpuMsg::DeviceBeforeDestroy > &);
	
	bool _CreatePipeline ();
	bool _CreateCmdBuffers ();
};
