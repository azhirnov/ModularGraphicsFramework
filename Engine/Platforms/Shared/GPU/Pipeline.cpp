// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Pipeline.h"

namespace Engine
{
namespace Platforms
{
	
/*
=================================================
	constructor
=================================================
*/
	GraphicsPipelineDescriptor::GraphicsPipelineDescriptor (UninitializedType) :
		patchControlPoints(0), subpass(0)
	{}
	
	GraphicsPipelineDescriptor::GraphicsPipelineDescriptor (const VertexInputState &vertexInput,
															const RenderState &renderState,
															const PipelineLayoutDescriptor &layout,
															EPipelineDynamicState::bits dynamicStates,
															const FragmentOutputState &fragOutput,
															uint patchControlPoints,
															uint subpass) :
		vertexInput(vertexInput),		renderState(renderState),
		dynamicStates(dynamicStates),	fragOutput(fragOutput),
		layout(layout),					patchControlPoints(patchControlPoints),
		subpass(subpass)
	{}
//-----------------------------------------------------------------------------
	


/*
=================================================
	constructor
=================================================
*/
	ComputePipelineDescriptor::ComputePipelineDescriptor (UninitializedType)
	{}

	ComputePipelineDescriptor::ComputePipelineDescriptor (const PipelineLayoutDescriptor &layout,
														  const uint3 &localGroupSize) :
		layout(layout), localGroupSize(localGroupSize)
	{}
//-----------------------------------------------------------------------------
	


/*
=================================================
	constructor
=================================================
*/
	PipelineTemplateDescriptor::PipelineTemplateDescriptor (UninitializedType) :
		patchControlPoints(0), subpass(0)
	{
	}
		
/*
=================================================
	StringGLSL
=================================================
*/
	void PipelineTemplateDescriptor::ShaderSource::StringGLSL (StringCRef data)
	{
		src[ ESource::GLSL ].Create2( String(data) );
	}
	
/*
=================================================
	StringCL
=================================================
*/
	void PipelineTemplateDescriptor::ShaderSource::StringCL (StringCRef data)
	{
		src[ ESource::OpenCL ].Create2( String(data) );
	}

/*
=================================================
	StringSPIRV
=================================================
*/
	void PipelineTemplateDescriptor::ShaderSource::StringSPIRV (StringCRef data)
	{
		src[ ESource::SPIRV ].Create2( String(data) );
	}
	
/*
=================================================
	ArraySPIRV
=================================================
*/
	void PipelineTemplateDescriptor::ShaderSource::ArraySPIRV (ArrayCRef<uint> data)
	{
		src[ ESource::SPIRV ].Create2( Array<uint>(data) );
	}

/*
=================================================
	FileSPIRV
=================================================
*/
	bool PipelineTemplateDescriptor::ShaderSource::FileSPIRV (const RFilePtr &file)
	{
		CHECK_ERR( file );

		usize	len = (usize)file->RemainingSize();
		String	str;	str.Resize( len );

		CHECK_ERR( file->Read(ArrayRef<char>( str )) );

		StringSPIRV( str );
		return true;
	}
	
/*
=================================================
	FuncSW
=================================================
*/
	void PipelineTemplateDescriptor::ShaderSource::FuncSW (const SWInvoke &func)
	{
		src[ ESource::SoftRenderer ].Create( func );
	}

/*
=================================================
	GetGLSL
=================================================
*/
	StringCRef  PipelineTemplateDescriptor::ShaderSource::GetGLSL () const
	{
		const auto&		data = src[ ESource::GLSL ];

		if ( data.Is< String >() )
			return data.Get< String >();

		RETURN_ERR( "no GLSL source!" );
	}
	
/*
=================================================
	GetSPIRV
=================================================
*/
	ArrayCRef<uint>  PipelineTemplateDescriptor::ShaderSource::GetSPIRV () const
	{
		const auto&		data = src[ ESource::SPIRV ];

		if ( data.Is< Array<uint> >() )
			return data.Get< Array<uint> >();

		RETURN_ERR( "no SPIR-V binary!" );
	}
		
/*
=================================================
	GetCL
=================================================
*/
	StringCRef  PipelineTemplateDescriptor::ShaderSource::GetCL () const
	{
		const auto&		data = src[ ESource::OpenCL ];

		if ( data.Is< String >() )
			return data.Get< String >();

		RETURN_ERR( "no CL source" );
	}
	
/*
=================================================
	GetSW
=================================================
*/
	PipelineTemplateDescriptor::ShaderSource::SWInvoke
		PipelineTemplateDescriptor::ShaderSource::GetSW () const
	{
		const auto&		data = src[ ESource::SoftRenderer ];

		if ( data.Is< SWInvoke >() )
			return data.Get< SWInvoke >();

		RETURN_ERR( "no SW function" );
	}

}	// Platforms
}	// Engine
