// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Common.h"
#include "Engine/Platforms/OpenGL/Impl/gl4.h"

class GLApp : public StaticRefCountedObject
{
// variables
public:
	Ptr< Module >	ms;

private:
	gl::GLuint		texID	= 0;
	bool			looping	= true;


// methods
public:
	GLApp ()
	{
		ms = GetMainSystemInstace();

		Platforms::RegisterPlatforms();
	}

	void Initialize ()
	{
		ms->AddModule( "win.platform"_GModID, CreateInfo::Platform() );
		ms->AddModule( "input.mngr"_GModID, CreateInfo::InputManager() );
		ms->AddModule( "stream.mngr"_GModID, CreateInfo::StreamManager() );
		ms->AddModule( "gl4.ctx"_GModID, CreateInfo::GpuContext() );

		auto	thread	= ms->GlobalSystems()->Get< ParallelThread >();
	
		thread->AddModule( "win.window"_TModID, CreateInfo::Window() );
		thread->AddModule( "input.thrd"_TModID, CreateInfo::InputThread() );
		thread->AddModule( "gl4.thrd"_TModID, CreateInfo::GpuThread() );

		auto	window		= thread->GetModule( "win.window"_TModID );
		auto	input		= thread->GetModule( "input.thrd"_TModID );
		auto	glthread	= thread->GetModule( "gl4.thrd"_TModID );

		window->AddModule( "win.keys"_OModID, CreateInfo::RawInputHandler() );
		window->AddModule( "win.mouse"_OModID, CreateInfo::RawInputHandler() );

		window->Subscribe( this, &GLApp::_OnWindowClosed );
		input->Subscribe( this, &GLApp::_OnKey );
		//input->Subscribe( this, &GLApp::_OnMotion );
		glthread->Subscribe( this, &GLApp::_Draw );
		glthread->Subscribe( this, &GLApp::_GLInit );
		glthread->Subscribe( this, &GLApp::_GLDelete );

		input = null;
		window = null;
		thread = null;
		glthread = null;


		// finish initialization
		ms->Send( Message< ModuleMsg::Link >() );
		ms->Send( Message< ModuleMsg::Compose >() );
	}

	void Quit ()
	{
		ms->Send( Message< ModuleMsg::Delete >() );
	}

	bool Update ()
	{
		ms->Send( Message< ModuleMsg::Update >() );
		return looping;
	}


private:
	bool _OnWindowClosed (const Message<ModuleMsg::WindowAfterDestroy> &)
	{
		looping = false;

		ms->Send( Message< ModuleMsg::Delete >() );
		return true;
	}

	bool _OnKey (const Message< ModuleMsg::InputKey > &msg)
	{
		DEBUG_CONSOLE( ("Key '"_str << KeyID::ToString( msg->key ) <<
						"', pressure " << msg->pressure).cstr() );
		return true;
	}

	bool _OnMotion (const Message< ModuleMsg::InputMotion > &msg)
	{
		DEBUG_CONSOLE( ("Motion '"_str << MotionID::ToString( msg->motion ) <<
						"', relative " << msg->relative <<
						", absolute " << msg->absolute).cstr() );
		return true;
	}

	bool _Draw (const Message< ModuleMsg::Update > &msg)
	{
		using namespace gl;

		GL_CALL( glViewport( 0, 0, 800, 600 ) );
		GL_CALL( glClearColor( 1.0f, 0.0f, 0.0f, 1.0f ) );
		GL_CALL( glClear( GL_COLOR_BUFFER_BIT ) );

		return true;
	}

	bool _GLInit (const Message< ModuleMsg::GpuDeviceCreated > &msg)
	{
		using namespace gl;

		GL_CALL( glGenTextures( 1, &texID ) );
		GL_CALL( glBindTexture( GL_TEXTURE_2D, texID ) );
		GL_CALL( glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, null ) );
		return true;
	}

	bool _GLDelete (const Message< ModuleMsg::GpuDeviceBeforeDestory > &msg)
	{
		using namespace gl;

		if ( texID )
		{
			GL_CALL( glDeleteTextures( 1, &texID ) );
			texID = 0;
		}
		return true;
	}
};
