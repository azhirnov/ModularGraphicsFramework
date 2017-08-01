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

	ModulePtr			graphicsPipeline;
	ModulePtr			shaderModules;
	//ModulePtr			commandBuilder;


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
	bool _VkInit (const Message< ModuleMsg::GpuDeviceCreated > &);
	bool _VkDelete (const Message< ModuleMsg::GpuDeviceBeforeDestory > &);
	
	bool _CreatePipeline ();
};
