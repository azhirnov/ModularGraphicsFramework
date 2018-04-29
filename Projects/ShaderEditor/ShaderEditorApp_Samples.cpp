// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Projects/ShaderEditor/ShaderEditorApp.h"

namespace ShaderEditor
{

/*
=================================================
	_InitSamples
=================================================
*/
	void ShaderEditorApp::_InitSamples ()
	{
		_samples.PushBack( LAMBDA() (Renderer &renderer)
		{
			Renderer::ShaderDescr	sh_main;
			sh_main.Pipeline( Pipelines::Create_experimental );
			sh_main.InChannel( "C:/Projects/graphxgenengine/Projects/ShaderEditor/res/img1.png", 0 );
			CHECK( renderer.Add( "main", sh_main ) );
		});

		_samples.PushBack( LAMBDA() (Renderer &renderer)
		{
			Renderer::ShaderDescr	sh_main;
			sh_main.Pipeline( Pipelines::Create_voronnoirecursion );
			CHECK( renderer.Add( "main", sh_main ) );
		});
		
		/*_samples.PushBack( LAMBDA() (Renderer &renderer)
		{
			Renderer::ShaderDescr	sh_bufA;
			sh_bufA.Pipeline( Pipelines::Create_sireniandawn1 );
			sh_bufA.InChannel( "noise", 0 );
			sh_bufA.InChannel( "bufA", 1 );
			CHECK( renderer.Add( "bufA", sh_bufA ) );

			Renderer::ShaderDescr	sh_main;
			sh_main.Pipeline( Pipelines::Create_sireniandawn2 );
			sh_bufA.InChannel( "bufA", 0 );
			CHECK( renderer.Add( "main", sh_main ) );
		});*/
		
		_samples.PushBack( LAMBDA() (Renderer &renderer)
		{
			Renderer::ShaderDescr	sh_main;
			sh_main.Pipeline( Pipelines::Create_glowballs );
			sh_main.InChannel( "main", 0 );
			CHECK( renderer.Add( "main", sh_main ) );
		});

		_samples.PushBack( LAMBDA() (Renderer &renderer)
		{
			Renderer::ShaderDescr	sh_main;
			sh_main.Pipeline( Pipelines::Create_skyline );
			CHECK( renderer.Add( "main", sh_main ) );
		});
	}

}	// ShaderEditor
