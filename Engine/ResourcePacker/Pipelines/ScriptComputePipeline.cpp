// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/ResourcePacker/Pipelines/ScriptComputePipeline.h"
#include "Engine/ResourcePacker/Packer/ResourcePacker.h"
#include "Engine/Script/Bindings/DefaultBindings.h"

namespace ResPack
{

/*
=================================================
	constructor
=================================================
*/
	ScriptComputePipeline::ScriptComputePipeline () :
		_compute{ PipelineCompiler::EShader::Compute }
	{
		_pipeline = new Pipeline( ScriptHelper::CurrentFileName() );

		ResourcePacker::Instance()->AddPipeline( _pipeline );
	}
	
/*
=================================================
	constructor
=================================================
*/
	ScriptComputePipeline::ScriptComputePipeline (const PipelineCompiler::ComputePipeline *other) :
		ScriptComputePipeline()
	{
		auto*	ppln = static_cast<Pipeline *>(const_cast<PipelineCompiler::ComputePipeline *>(other));

		_shaderFormat	= ppln->GetShaderFormat();
		_compute		= ppln->GetShader();
		_localGroup		= ppln->GetLocalGroup();
	}

/*
=================================================
	destructor
=================================================
*/
	ScriptComputePipeline::~ScriptComputePipeline ()
	{
		if ( _pipeline )
		{
			_pipeline->GetShaderFormat()	= _shaderFormat;
			_pipeline->GetShader()			= _compute;
			_pipeline->GetLocalGroup()		= _localGroup;
		}
	}
	
/*
=================================================
	Bind
=================================================
*/	
	void ScriptComputePipeline::Bind (ScriptEnginePtr se)
	{
		using namespace GX_STL::GXScript;
		using Self	= ScriptComputePipeline;

		ClassBinder<Self>	binder( se );

		binder.CreateClassValue();
		binder.AddProperty( &Self::_shaderFormat,	"shaderFormat" );
		binder.AddProperty( &Self::_compute,		"compute" );
		binder.AddProperty( &Self::_localGroup,		"localGroupSize" );
	}

}	// ResPack
