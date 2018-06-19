// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/RenderState.h"
#include "Engine/Platforms/Public/GPU/VertexAttribs.h"
#include "Engine/Platforms/Public/GPU/VertexInputState.h"
#include "Engine/Platforms/Public/GPU/FragmentOutputState.h"
#include "Engine/Platforms/Public/GPU/PipelineLayout.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/IDs.h"

namespace SWShaderLang {
namespace Impl {
	class SWShaderHelper;
}}

namespace Engine
{
namespace Platforms
{

	//
	// Graphics Pipeline Description
	//

	struct GraphicsPipelineDescription final : CompileTime::FastCopyable
	{
	// types
		using ConstantName_t			= StaticString<32>;
		using Constant_t				= Union< int, uint, ilong, ulong, float, double >;
		using SpecializationConstants_t	= FixedSizeHashMap< ConstantName_t, Constant_t, GlobalConst::GAPI_MaxSpecializationConstants >;

	// variables
		VertexInputState				vertexInput;
		RenderState						renderState;
		EPipelineDynamicState::bits		dynamicStates;
		FragmentOutputState				fragOutput;
		PipelineLayoutDescription		layout;
		uint							patchControlPoints;
		uint							subpass;
		// TODO: viewports
		// TODO: specialization constants

	// methods
		GraphicsPipelineDescription (GX_DEFCTOR);

		GraphicsPipelineDescription (const VertexInputState &vertexInput,
									 const RenderState &renderState,
									 const PipelineLayoutDescription &layout,
									 EPipelineDynamicState::bits dynamicStates,
									 const FragmentOutputState &fragOutput,
									 uint patchControlPoints = 0,
									 uint subpass = 0);
	};



	//
	// Compute Pipeline Description
	//

	struct ComputePipelineDescription final : CompileTime::FastCopyable
	{
	// types
		using SpecializationConstants_t	= GraphicsPipelineDescription::SpecializationConstants_t;

	// variables
		PipelineLayoutDescription		layout;
		uint3							localGroupSize;
		// TODO: specialization constants

	// methods
		ComputePipelineDescription (GX_DEFCTOR);

		explicit
		ComputePipelineDescription (const PipelineLayoutDescription &layout,
								    const uint3 &localGroupSize);
	};



	//
	// Pipeline Template Description
	//

	struct PipelineTemplateDescription final : CompileTime::FastCopyable
	{
	// types
		struct ShaderSource
		{
		// types
			struct ESource { enum type {
				GLSL,
				GLSL_Bin,
				SPIRV,
				SPIRV_Asm,
				OpenCL,
				OpenCL_Asm,
				SoftRenderer,
				HLSL,
				HLSL_Bin,
				_Count
			}; };

			using SWInvoke	= void (*) (const SWShaderLang::Impl::SWShaderHelper &);
			using Data		= Union< uint, String, BinaryArray, Array<uint>, SWInvoke >;
			using Sources	= StaticArray< Data, ESource::_Count >;
			using RFilePtr	= GXFile::RFilePtr;
			
		// variables
			Sources		src;

		// methods
			ShaderSource () {}

			// GLSL source
			void StringGLSL (StringCRef data);
			bool FileGLSL (const RFilePtr &file);
			ND_ StringCRef  GetGLSL () const;

			// GLSL binary
			//void ArrayGLSLBin (BinArrayCRef data);
			//bool FileGLSLBin (const RFilePtr &file);
			//BinArrayCRef GetGLSLBin () const;

			// CL source
			void StringCL (StringCRef data);
			bool FileCL (const RFilePtr &file);
			ND_ StringCRef  GetCL () const;

			// CL binary
			void StringCLAsm (StringCRef data);
			bool FileCLAsm (const RFilePtr &file);
			ND_ StringCRef  GetCLAsm () const;

			// SPIRV binary
			void ArraySPIRV (ArrayCRef<uint> data);
			bool FileSPIRV (const RFilePtr &file);
			ND_ ArrayCRef<uint>  GetSPIRV () const;

			// SPIRV assembly
			void StringSpirvAsm (StringCRef data);
			StringCRef	GetSpirvAsm () const;

			// HLSL Source
			void StringHLSL (StringCRef data);
			ND_ StringCRef GetHLSL () const;

			// HLSL Binary
			void StringBinHLSL (BinArrayCRef data);
			ND_ BinArrayCRef GetHLSLBin () const;

			// Software
			void FuncSW (const SWInvoke &func);
			ND_ SWInvoke  GetSW () const;
		};

		using Sources = StaticArray< ShaderSource, EShader::_Count >;


	// variables
		Sources							shaders;
		VertexAttribs					attribs;
		RenderState						renderState;
		EPipelineDynamicState::bits		dynamicStates;
		FragmentOutputState				fragOutput;
		PipelineLayoutDescription		layout;
		uint							patchControlPoints;
		uint							subpass;
		uint3							localGroupSize;
		EPrimitive::bits				supportedPrimitives;
		EShader::bits					supportedShaders;


	// methods
		PipelineTemplateDescription (GX_DEFCTOR);

		ND_ ShaderSource&  Vertex ()			{ return shaders[ EShader::Vertex ]; }
		ND_ ShaderSource&  TessControl ()		{ return shaders[ EShader::TessControl ]; }
		ND_ ShaderSource&  TessEvaluation ()	{ return shaders[ EShader::TessEvaluation ]; }
		ND_ ShaderSource&  Geometry ()			{ return shaders[ EShader::Geometry ]; }
		ND_ ShaderSource&  Fragment ()			{ return shaders[ EShader::Fragment ]; }
		ND_ ShaderSource&  Compute ()			{ return shaders[ EShader::Compute ]; }
	};

}	// Platforms


namespace CreateInfo
{

	//
	// Graphics Pipeline Create Info
	//
	struct GraphicsPipeline
	{
	// types
		using Description	= Platforms::GraphicsPipelineDescription;

	// variables
		ModulePtr		gpuThread;			// can be null
		Description		descr;
	};


	//
	// Compute Pipeline Create Info
	//
	struct ComputePipeline
	{
	// types
		using Description	= Platforms::ComputePipelineDescription;
		
	// variables
		ModulePtr		gpuThread;			// can be null
		Description		descr;
	};


	//
	// Pipeline Template Create Info
	//
	struct PipelineTemplate
	{
	// types
		using Description	= Platforms::PipelineTemplateDescription;

	// variables
		Description		descr;
	};


	//
	// Pipeline Resource Table Create Info
	//
	struct PipelineResourceTable
	{
	// variables
		ModulePtr		gpuThread;			// can be null

	// methods
		//PipelineResourceTable () {}
	};

}	// CreateInfo


namespace GpuMsg
{
	//
	// Get Graphics Pipeline Description
	//
	struct GetGraphicsPipelineDescription : _MessageBase_
	{
		Out< Platforms::GraphicsPipelineDescription >	result;
	};


	//
	// Get Compute Pipeline Description
	//
	struct GetComputePipelineDescription : _MessageBase_
	{
		Out< Platforms::ComputePipelineDescription >	result;
	};


	//
	// Get Pipeline Layout Description
	//
	struct GetPipelineLayoutDescription : _MessageBase_
	{
		Out< Platforms::PipelineLayoutDescription >		result;
	};


	//
	// Bind Buffer to Pipeline
	//
	struct PipelineAttachBuffer : ModuleMsg::AttachModule
	{
	// variables
		BytesUL		offset;
		BytesUL		size;

	// methods
		PipelineAttachBuffer (StringCRef name, const ModulePtr &mod, Bytes<uint> size, Bytes<uint> off = Uninitialized) :
			AttachModule{name, mod}, offset{BytesUL(off)}, size{BytesUL(size)} {}

		PipelineAttachBuffer (StringCRef name, const ModulePtr &mod, Bytes<ulong> size, Bytes<ulong> off = Uninitialized) :
			AttachModule{name, mod}, offset{off}, size{size} {}
	};


	//
	// Bind Image to Pipeline
	//
	struct PipelineAttachImage : ModuleMsg::AttachModule
	{
	// types
		using ImageViewDescription	= Platforms::ImageViewDescription;
		using EImageLayout			= Platforms::EImageLayout;

	// variables
		ImageViewDescription	descr;
		EImageLayout::type		layout	= Uninitialized;

	// methods
		PipelineAttachImage (StringCRef name, const ModulePtr &mod, const ImageViewDescription &descr, EImageLayout::type layout = EImageLayout::General) :
			AttachModule{name, mod}, descr{descr}, layout{layout} {}
	};


	//
	// Bind Texture to Pipeline
	//
	struct PipelineAttachTexture : ModuleMsg::AttachModule
	{
	// types
		using ImageViewDescription	= Platforms::ImageViewDescription;
		using EImageLayout			= Platforms::EImageLayout;

	// variables
		ModulePtr						sampler;
		Optional<ImageViewDescription>	descr;
		EImageLayout::type				layout	= Uninitialized;

	// methods
		PipelineAttachTexture (StringCRef name, const ModulePtr &texture, const ModulePtr &sampler, EImageLayout::type layout = EImageLayout::General) :
			AttachModule{name, texture}, sampler{sampler}, layout{layout}
		{}

		PipelineAttachTexture (StringCRef name, const ModulePtr &texture, const ModulePtr &sampler,
							   const ImageViewDescription &descr, EImageLayout::type layout = EImageLayout::General) :
			AttachModule{name, texture}, sampler{sampler}, descr{descr}, layout{layout}
		{}
	};


	//
	// Update Resources in Pipeline Resource Table
	//
	//struct PipelineResourceTableUpdate
	//{};


	//
	// Request Pipeline information
	//
	struct GetPipelineTemplateInfo : _MessageBase_
	{
	// types
		struct EGraphicsAPI {
			enum type {
				OpenGL_210,
				OpenGL_330,
				OpenGL_450,

				OpenGLES_200,
				OpenGLES_320,

				DirectX_11,
				DirectX_12,

				Vulkan_10,
				Vulkan_11,

				OpenCL_120,
				OpenCL_210,

				Soft,
			};

			GX_ENUM_BITFIELD( type );
		};

		using EShader	= Platforms::EShader;

		struct Info {
			EGraphicsAPI::bits		apiVersions;
			EShader::bits			shaders;
		};

	// variables
		Out< Info >			result;
	};

	
	//
	// Create GraphicsPipelineDescription from PipelineTemplate
	//
	struct CreateGraphicsPipelineDescription : _MessageBase_
	{
	// types
		using SpecializationConstants_t	= Platforms::GraphicsPipelineDescription::SpecializationConstants_t;

	// variables
		Platforms::VertexInputState						vertexInput;
		SpecializationConstants_t						constants;	// optional
		Platforms::EPrimitive::type						topology	= Uninitialized;
		Out< Platforms::GraphicsPipelineDescription >	result;

	// methods
		CreateGraphicsPipelineDescription (const Platforms::VertexInputState &vertexInput, Platforms::EPrimitive::type topology) :
			vertexInput(vertexInput), topology(topology) {}
	};


	//
	// Create ComputePipelineDescription from PipelineTemplate
	//
	struct CreateComputePipelineDescription : _MessageBase_
	{
	// types
		using SpecializationConstants_t	= Platforms::ComputePipelineDescription::SpecializationConstants_t;

	// variables
		Optional< uint3 >								localGroupSize;
		SpecializationConstants_t						constants;	// optional
		Out< Platforms::ComputePipelineDescription >	result;

	// methods
		CreateComputePipelineDescription () {}
		explicit CreateComputePipelineDescription (const uint3 &localSize) : localGroupSize{ localSize } {}
	};


	//
	// Create GraphicsPipeline Module from PipelineTemplate
	//
	struct CreateGraphicsPipeline : _MessageBase_
	{
	// types
		using SpecializationConstants_t	= Platforms::GraphicsPipelineDescription::SpecializationConstants_t;

	// variables
		Out< ModulePtr >				result;
		UntypedID_t						moduleID	= 0;
		ModulePtr						gpuThread;
		ModulePtr						renderPass;
		Platforms::VertexInputState		vertexInput;
		Platforms::EPrimitive::type		topology	= Uninitialized;
		SpecializationConstants_t		constants;	// optional

	// methods
		CreateGraphicsPipeline () {}

		CreateGraphicsPipeline (UntypedID_t moduleID,
								const ModulePtr &gpuThread,
								const ModulePtr &renderPass,
								const Platforms::VertexInputState &vertexInput,
								Platforms::EPrimitive::type topology) :
			moduleID(moduleID),			gpuThread(gpuThread),
			renderPass(renderPass),		vertexInput(vertexInput),
			topology(topology)
		{}
	};


	//
	// Create ComputePipeline Module from PipelineTemplate
	//
	struct CreateComputePipeline : _MessageBase_
	{
	// types
		using SpecializationConstants_t	= Platforms::ComputePipelineDescription::SpecializationConstants_t;

	// variables
		Out< ModulePtr >				result;
		UntypedID_t						moduleID	= 0;
		ModulePtr						gpuThread;
		Optional< uint3 >				localGroupSize;
		SpecializationConstants_t		constants;	// optional
		
	// methods
		CreateComputePipeline () {}

		explicit CreateComputePipeline (UntypedID_t moduleID, const ModulePtr &gpuThread = null) :
			moduleID{ moduleID },	gpuThread{ gpuThread }
		{}

		CreateComputePipeline (UntypedID_t moduleID, const uint3 &localSize, const ModulePtr &gpuThread = null) :
			moduleID{ moduleID },		gpuThread{ gpuThread },
			localGroupSize{ localSize }
		{}
	};


}	// GpuMsg
}	// Engine
