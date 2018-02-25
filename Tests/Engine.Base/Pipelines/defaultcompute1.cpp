// This is generated file
// Origin file: 'C:\Projects\graphxgenengine\Tests\PipelineCompiler\Pipelines\DefaultCompute1.cpp'
// Created at: 2018/02/19 - 23:22:39

#include "all_pipelines.h"
// C++ shader
#ifdef GRAPHICS_API_SOFT
namespace SWShaderLang
{
	struct SSBO{
		vec4 data;
	};
	
	
	//---------------------------------
	
	static void sw_defaultcompute1_comp (const Impl::SWShaderHelper &_helper_)
	{
		// prepare externals
		Impl::StorageBuffer< SSBO, Impl::EStorageAccess::ReadOnly > ssb;	_helper_.GetStorageBuffer( 1, ssb );
		Impl::Image2D< vec4, Impl::EStorageAccess::WriteOnly > un_OutImage;		_helper_.GetImage( 0, un_OutImage );
		const float C_1d0 = float(1.0);
		auto& gl_GlobalInvocationID = _helper_.GetComputeShaderState().inGlobalInvocationID;
		auto& gl_NumWorkGroups = _helper_.GetComputeShaderState().inNumWorkGroups;
	
		// shader
	{
		ivec2 H_fjkh5n;
		H_fjkh5n=ivec3( gl_GlobalInvocationID ).xy();
		vec2 H_xulukk;
		H_xulukk=vec2( H_fjkh5n );
		ivec2 H_k5ommu;
		H_k5ommu=ivec3( gl_NumWorkGroups ).xy();
		vec2 H_nnu7tn1;
		H_nnu7tn1=vec2( H_k5ommu );
		vec2 uv;
		uv=(H_xulukk/H_nnu7tn1);
		float fragColor;
		fragColor=sin( uv.x );
		float fragColor1;
		fragColor1=cos( uv.y );
		vec4 H_imjy6u1;
		H_imjy6u1=vec4( fragColor, fragColor1, C_1d0, ssb->data.x );
		imageStore( un_OutImage, H_fjkh5n, H_imjy6u1 );
	}
	
	
	}
}	// SWShaderLang
#endif	// GRAPHICS_API_SOFT


namespace Pipelines
{

void Create_defaultcompute1 (PipelineTemplateDescriptor& descr)
{
	descr = PipelineTemplateDescriptor();
	descr.supportedShaders = EShader::bits() | EShader::Compute;

	descr.localGroupSize = uint3(1, 1, 1);
	descr.layout = PipelineLayoutDescriptor::Builder()
			.AddImage( "un_OutImage", EImage::Tex2D, EPixelFormat::RGBA8_UNorm, true, false, 0, 0, EShader::bits() | EShader::Compute )
			.AddStorageBuffer( "ssb", 16_b, 0_b, false, true, 0, 1, EShader::bits() | EShader::Compute )
			.Finish();

	descr.Compute().FuncSW( &SWShaderLang::sw_defaultcompute1_comp );

};
};
