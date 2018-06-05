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
	FileGLSL
=================================================
*/
	bool PipelineTemplateDescriptor::ShaderSource::FileGLSL (const RFilePtr &file)
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
	void PipelineTemplateDescriptor::ShaderSource::StringCL (StringCRef data)
	{
		src[ ESource::OpenCL ].Create2( String(data) );
	}
	
/*
=================================================
	FileCL
=================================================
*/
	bool PipelineTemplateDescriptor::ShaderSource::FileCL (const RFilePtr &file)
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
	void PipelineTemplateDescriptor::ShaderSource::StringCLAsm (StringCRef data)
	{
		src[ ESource::OpenCL_Asm ].Create2( String(data) );
	}
	
/*
=================================================
	FileCLAsm
=================================================
*/
	bool PipelineTemplateDescriptor::ShaderSource::FileCLAsm (const RFilePtr &file)
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
	void PipelineTemplateDescriptor::ShaderSource::StringSpirvAsm (StringCRef data)
	{
		src[ ESource::SPIRV_Asm ].Create2( String(data) );
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
	StringHLSL
=================================================
*/
	void PipelineTemplateDescriptor::ShaderSource::StringHLSL (StringCRef data)
	{
		src[ ESource::HLSL ].Create( String(data) );
	}
	
/*
=================================================
	StringBinHLSL
=================================================
*/
	void PipelineTemplateDescriptor::ShaderSource::StringBinHLSL (BinArrayCRef data)
	{
		src[ ESource::HLSL_Bin ].Create( BinaryArray(data) );
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

		//RETURN_ERR( "GLSL source doesn't exists!" );
		return Uninitialized;
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

		//RETURN_ERR( "SPIR-V binary doesn't exists!" );
		return Uninitialized;
	}
		
/*
=================================================
	GetSpirvAsm
=================================================
*/
	StringCRef	PipelineTemplateDescriptor::ShaderSource::GetSpirvAsm () const
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
	StringCRef  PipelineTemplateDescriptor::ShaderSource::GetCL () const
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
	StringCRef PipelineTemplateDescriptor::ShaderSource::GetCLAsm () const
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
	PipelineTemplateDescriptor::ShaderSource::SWInvoke
		PipelineTemplateDescriptor::ShaderSource::GetSW () const
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
	StringCRef  PipelineTemplateDescriptor::ShaderSource::GetHLSL () const
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
	BinArrayCRef  PipelineTemplateDescriptor::ShaderSource::GetHLSLBin () const
	{
		const auto&		data = src[ ESource::HLSL_Bin ];

		if ( data.Is< BinaryArray >() )
			return data.Get< BinaryArray >();

		//RETURN_ERR( "HLSL binary doesn't exists!" );
		return Uninitialized;
	}

}	// Platforms
}	// Engine
