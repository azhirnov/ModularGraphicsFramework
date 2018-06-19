// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/ResourcePacker/Pipelines/ScriptGraphicsPipeline.h"
#include "Engine/ResourcePacker/Packer/ResourcePacker.h"
#include "Core/Script/Bindings/DefaultBindings.h"

namespace ResPack
{

/*
=================================================
	constructor
=================================================
*/
	ScriptGraphicsPipeline::ScriptGraphicsPipeline ()
	{
		_pipeline = new Pipeline( ScriptHelper::CurrentFileName() );

		ResourcePacker::Instance()->AddPipeline( _pipeline );
	}
	
/*
=================================================
	constructor
=================================================
*/
	ScriptGraphicsPipeline::ScriptGraphicsPipeline (const PipelineCompiler::GraphicsPipeline *other) :
		ScriptGraphicsPipeline()
	{
		auto*	ppln = Cast<Pipeline *>(const_cast<PipelineCompiler::GraphicsPipeline *>( other ));

		_shaderFormat		= ppln->GetShaderFormat();
		_shaders			= ppln->GetShaders();
		_renderState		= ppln->GetRenderState();
		_supportedTopology	= ppln->GetSupportedTopology();
		_dynamicStates		= ppln->GetDynamicStates();
		_patchControlPoints	= ppln->GetPatchControlPoints();
	}

/*
=================================================
	destructor
=================================================
*/
	ScriptGraphicsPipeline::~ScriptGraphicsPipeline ()
	{
		if ( _pipeline )
		{
			_pipeline->GetShaderFormat()		= _shaderFormat;
			_pipeline->GetShaders()				= _shaders;
			_pipeline->GetRenderState()			= _renderState;
			_pipeline->GetSupportedTopology()	= _supportedTopology;
			_pipeline->GetDynamicStates()		= _dynamicStates;
			_pipeline->GetPatchControlPoints()	= _patchControlPoints;
		}
	}

/*
=================================================
	Bind
=================================================
*/
	void ScriptGraphicsPipeline::Bind (ScriptEnginePtr se)
	{
		using namespace GXScript;
		using Self	= ScriptGraphicsPipeline;

		ClassBinder<Self>	binder( se );

		binder.CreateClassValue();
		binder.AddProperty( &Self::_shaderFormat,		"shaderFormat" );
		binder.AddProperty( &Self::_shaders,			"shaders" );
		binder.AddProperty( &Self::_renderState,		"renderState" );
		binder.AddProperty( &Self::_supportedTopology,	"supportedTopology" );
		binder.AddProperty( &Self::_dynamicStates,		"dynamicStates" );
		binder.AddProperty( &Self::_patchControlPoints,	"patchControlPoints" );
	}
	
}	// ResPack
