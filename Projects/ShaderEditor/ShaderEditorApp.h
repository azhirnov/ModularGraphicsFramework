// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Projects/ShaderEditor/Renderer.h"

namespace ShaderEditor
{

	//
	// Shader Editor App
	//

	class ShaderEditorApp final : public StaticRefCountedObject
	{
	// types
	private:


	// variables
	private:
		ModulePtr		_camera;
		ModulePtr		_surface;

		Renderer		_renderer;

		bool			_looping;

	
	// methods
	public:
		ShaderEditorApp ();

		bool Initialize (GAPI::type api);
		void Quit ();
		bool Update ();

	private:
		bool _OnWindowClosed (const Message< OSMsg::WindowBeforeDestroy > &);
		bool _Init (const Message< ModuleMsg::Compose > &);
		bool _GInit (const Message< GpuMsg::DeviceCreated > &);
		bool _Draw (const Message< SceneMsg::CameraRequestUpdate > &);
	};

}	// ShaderEditor
