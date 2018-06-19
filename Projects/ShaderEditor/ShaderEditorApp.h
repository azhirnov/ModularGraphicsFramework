// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Renderer.h"

namespace ShaderEditor
{

	//
	// Shader Editor App
	//

	class ShaderEditorApp final : public StaticRefCountedObject
	{
	// types
	private:
		using Samples_t	= Array< Function< void (Renderer&) > >;


	// variables
	private:
		ModulePtr		_camera;
		ModulePtr		_surface;

		Renderer		_renderer;

		Samples_t		_samples;
		usize			_currSample;
		usize			_nextSample;

		bool			_looping;
		const bool		_vrMode;

	
	// methods
	public:
		ShaderEditorApp ();

		bool Initialize (GAPI::type api);
		void Quit ();
		bool Update ();

	private:
		bool _OnWindowClosed (const OSMsg::WindowBeforeDestroy &);
		bool _Init (const ModuleMsg::Compose &);
		bool _GInit (const GpuMsg::DeviceCreated &);
		bool _Draw (const SceneMsg::CameraRequestUpdate &);

		void _OnKey (const ModuleMsg::InputKey &);

		void _InitSamples ();
	};

}	// ShaderEditor
