// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "../Common.h"
#include "Engine/Platforms/OpenGL/Impl/gl4.h"

class GLApp final : public StaticRefCountedObject
{
// variables
public:
	Ptr< Module >	ms;

private:
	gl::GLuint		texID	= 0;
	bool			looping	= true;


// methods
public:
	GLApp ();

	void Initialize ();
	void Quit ();
	bool Update ();

private:
	bool _OnWindowClosed (const Message<ModuleMsg::WindowAfterDestroy> &);
	bool _OnKey (const Message< ModuleMsg::InputKey > &msg);
	bool _OnMotion (const Message< ModuleMsg::InputMotion > &msg);
	bool _Draw (const Message< ModuleMsg::Update > &msg);

	bool _GLInit (const Message< GpuMsg::DeviceCreated > &msg);
	bool _GLDelete (const Message< GpuMsg::DeviceBeforeDestroy > &msg);
};
