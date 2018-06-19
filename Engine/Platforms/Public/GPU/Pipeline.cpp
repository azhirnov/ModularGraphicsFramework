// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Public/GPU/Pipeline.h"

namespace Engine
{
namespace Platforms
{
	
/*
=================================================
	constructor
=================================================
*/
	GraphicsPipelineDescription::GraphicsPipelineDescription (UninitializedType) :
		patchControlPoints(0), subpass(0)
	{}
	
	GraphicsPipelineDescription::GraphicsPipelineDescription  (const VertexInputState &				vertexInput,
																const RenderState &					renderState,
																const PipelineLayoutDescription&	layout,
																EPipelineDynamicState::bits			dynamicStates,
																const FragmentOutputState &			fragOutput,
																uint								patchControlPoints,
																uint								subpass) :
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
	ComputePipelineDescription::ComputePipelineDescription (UninitializedType)
	{}

	ComputePipelineDescription::ComputePipelineDescription (const PipelineLayoutDescription &layout,
														  const uint3 &localGroupSize) :
		layout(layout), localGroupSize(localGroupSize)
	{}
//-----------------------------------------------------------------------------
	


/*
=================================================
	constructor
=================================================
*/
	PipelineTemplateDescription::PipelineTemplateDescription (UninitializedType) :
		patchControlPoints(0), subpass(0)
	{
	}
		
/*
=================================================
	StringGLSL
=================================================
*/
	void PipelineTemplateDescription::ShaderSource::StringGLSL (StringCRef data)
	{
		src[ ESource::GLSL ].Create2( String(data) );
	}
	
/*
=================================================
	FileGLSL
=================================================
*/
	bool PipelineTemplateDescription::ShaderSource::FileGLSL (const RFilePtr &file)
	{
		CHECK_ERR( file );

		const usize	len		= usize(file->RemainingSize());
		String		str;	str.Resize( len );

		CHECK_ERR( file->Read( str.ptr(), str.LengthInBytes() ) );

		StringGLSL( str );
		return true;
	}

/*
=================================================
	StringCL
=================================================
*/
	void PipelineTemplateDescription::ShaderSource::StringCL (StringCRef data)
	{
		src[ ESource::OpenCL ].Create2( String(data) );
	}
	
/*
=================================================
	FileCL
=================================================
*/
	bool PipelineTemplateDescription::ShaderSource::FileCL (const RFilePtr &file)
	{
		CHECK_ERR( file );
		
		const usize	len		= usize(file->RemainingSize());
		String		str;	str.Resize( len );

		CHECK_ERR( file->Read( str.ptr(), str.LengthInBytes() ) );

		StringCL( str );
		return true;
	}
	
/*
=================================================
	StringCLAsm
=================================================
*/
	void PipelineTemplateDescription::ShaderSource::StringCLAsm (StringCRef data)
	{
		src[ ESource::OpenCL_Asm ].Create2( String(data) );
	}
	
/*
=================================================
	FileCLAsm
=================================================
*/
	bool PipelineTemplateDescription::ShaderSource::FileCLAsm (const RFilePtr &file)
	{
		CHECK_ERR( file );
		
		const usize	len		= usize(file->RemainingSize());
		String		str;	str.Resize( len );

		CHECK_ERR( file->Read( str.ptr(), str.LengthInBytes() ) );

		StringCLAsm( str );
		return true;
	}

/*
=================================================
	ArraySPIRV
=================================================
*/
	void PipelineTemplateDescription::ShaderSource::ArraySPIRV (ArrayCRef<uint> data)
	{
		src[ ESource::SPIRV ].Create2( Array<uint>(data) );
	}

/*
=================================================
	FileSPIRV
=================================================
*/
	bool PipelineTemplateDescription::ShaderSource::FileSPIRV (const RFilePtr &file)
	{
		CHECK_ERR( file );

		usize		len		= usize(file->RemainingSize());
		Array<uint>	arr;	arr.Resize( len / sizeof(uint) );

		CHECK_ERR( file->Read(ArrayRef<uint>( arr )) );

		ArraySPIRV( arr );
		return true;
	}
	
/*
=================================================
	StringSpirvAsm
=================================================
*/
	void PipelineTemplateDescription::ShaderSource::StringSpirvAsm (StringCRef data)
	{
		src[ ESource::SPIRV_Asm ].Create2( String(data) );
	}

/*
=================================================
	FuncSW
=================================================
*/
	void PipelineTemplateDescription::ShaderSource::FuncSW (const SWInvoke &func)
	{
		src[ ESource::SoftRenderer ].Create( func );
	}
	
/*
=================================================
	StringHLSL
=================================================
*/
	void PipelineTemplateDescription::ShaderSource::StringHLSL (StringCRef data)
	{
		src[ ESource::HLSL ].Create( String(data) );
	}
	
/*
=================================================
	StringBinHLSL
=================================================
*/
	void PipelineTemplateDescription::ShaderSource::StringBinHLSL (BinArrayCRef data)
	{
		src[ ESource::HLSL_Bin ].Create( BinaryArray(data) );
	}

/*
=================================================
	GetGLSL
=================================================
*/
	StringCRef  PipelineTemplateDescription::ShaderSource::GetGLSL () const
	{
		const auto&		data = src[ ESource::GLSL ];

		if ( data.Is< String >() )
			return data.Get< String >();

		//RETURN_ERR( "GLSL source doesn't exists!" );
		return Uninitialized;
	}
	
/*
=================================================
	GetSPIRV
=================================================
*/
	ArrayCRef<uint>  PipelineTemplateDescription::ShaderSource::GetSPIRV () const
	{
		const auto&		data = src[ ESource::SPIRV ];

		if ( data.Is< Array<uint> >() )
			return data.Get< Array<uint> >();

		//RETURN_ERR( "SPIR-V binary doesn't exists!" );
		return Uninitialized;
	}
		
/*
=================================================
	GetSpirvAsm
=================================================
*/
	StringCRef	PipelineTemplateDescription::ShaderSource::GetSpirvAsm () const
	{
		const auto&		data = src[ ESource::SPIRV_Asm ];

		if ( data.Is< String >() )
			return data.Get< String >();

		//RETURN_ERR( "SPIR-V assembly doesn't exists!" );
		return Uninitialized;
	}

/*
=================================================
	GetCL
=================================================
*/
	StringCRef  PipelineTemplateDescription::ShaderSource::GetCL () const
	{
		const auto&		data = src[ ESource::OpenCL ];

		if ( data.Is< String >() )
			return data.Get< String >();

		//RETURN_ERR( "CL source doesn't exists!" );
		return Uninitialized;
	}
	
/*
=================================================
	GetCLAsm
=================================================
*/
	StringCRef PipelineTemplateDescription::ShaderSource::GetCLAsm () const
	{
		const auto&		data = src[ ESource::OpenCL_Asm ];

		if ( data.Is< String >() )
			return data.Get< String >();

		//RETURN_ERR( "CL assembly doesn't exists!" );
		return Uninitialized;
	}
	
/*
=================================================
	GetSW
=================================================
*/
	PipelineTemplateDescription::ShaderSource::SWInvoke
		PipelineTemplateDescription::ShaderSource::GetSW () const
	{
		const auto&		data = src[ ESource::SoftRenderer ];

		if ( data.Is< SWInvoke >() )
			return data.Get< SWInvoke >();

		//RETURN_ERR( "SW function doesn't exists!" );
		return Uninitialized;
	}
	
/*
=================================================
	GetHLSL
=================================================
*/
	StringCRef  PipelineTemplateDescription::ShaderSource::GetHLSL () const
	{
		const auto&		data = src[ ESource::HLSL ];

		if ( data.Is< String >() )
			return data.Get< String >();

		//RETURN_ERR( "HLSL source doesn't exists!" );
		return Uninitialized;
	}
	
/*
=================================================
	GetHLSLBin
=================================================
*/
	BinArrayCRef  PipelineTemplateDescription::ShaderSource::GetHLSLBin () const
	{
		const auto&		data = src[ ESource::HLSL_Bin ];

		if ( data.Is< BinaryArray >() )
			return data.Get< BinaryArray >();

		//RETURN_ERR( "HLSL binary doesn't exists!" );
		return Uninitialized;
	}

}	// Platforms
}	// Engine
