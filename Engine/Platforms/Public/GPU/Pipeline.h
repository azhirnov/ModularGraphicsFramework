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
		uint							patchControlPoints;
		uint							subpass;
		bool							earlyFragmentTests;
		// TODO: viewports
		// TODO: specialization constants

	// methods
		GraphicsPipelineDescription (GX_DEFCTOR);

		GraphicsPipelineDescription (const VertexInputState &vertexInput,
									 const RenderState &renderState,
									 EPipelineDynamicState::bits dynamicStates,
									 const FragmentOutputState &fragOutput,
									 bool earlyFragmentTests = true,
									 uint patchControlPoints = 0,
									 uint subpass = 0);

		DEBUG_ONLY( ND_ String ToString () const; )
	};



	//
	// Compute Pipeline Description
	//

	struct ComputePipelineDescription final : CompileTime::FastCopyable
	{
	// types
		using SpecializationConstants_t	= GraphicsPipelineDescription::SpecializationConstants_t;

	// variables
		uint3			localGroupSize;
		// TODO: specialization constants

	// methods
		ComputePipelineDescription (GX_DEFCTOR);

		explicit ComputePipelineDescription (const uint3 &localGroupSize);
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
			using SWInvoke_t	= void (*) (const SWShaderLang::Impl::SWShaderHelper &);
			using Data_t		= Union< String, BinaryArray, Array<uint>, SWInvoke_t >;
			using Sources_t		= Map< EShaderLangFormat::type, Data_t >;
			using RFilePtr		= GXFile::RFilePtr;

		// variables
			Sources_t		_sources;

		// methods
			ShaderSource () {}

			void AddSource (EShaderLangFormat::type fmt, String &&src);
			void AddBinary (EShaderLangFormat::type fmt, BinArrayCRef bin);
			void AddSpirv (EShaderLangFormat::type fmt, ArrayCRef<uint> bin);
			void AddInvocable (EShaderLangFormat::type fmt, SWInvoke_t ptr);

			bool AddSource (EShaderLangFormat::type fmt, const RFilePtr &file);
			bool AddBinary (EShaderLangFormat::type fmt, const RFilePtr &file);
			bool AddSpirv (EShaderLangFormat::type fmt, const RFilePtr &file);

			ND_ StringCRef GetString (EShaderLangFormat::type fmt) const;
			ND_ BinArrayCRef GetBinary (EShaderLangFormat::type fmt) const;
			ND_ ArrayCRef<uint> GetSpirv (EShaderLangFormat::type fmt) const;
			ND_ SWInvoke_t GetInvocable (EShaderLangFormat::type fmt) const;

			ND_ bool IsExists (EShaderLangFormat::type fmt) const;
			ND_ bool Empty () const									{ return _sources.Empty(); }
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
		using Description_t	= Platforms::GraphicsPipelineDescription;
		using Layout_t		= Platforms::PipelineLayoutDescription;

	// variables
		ModulePtr			gpuThread;		// can be null
		Description_t		descr;
		Layout_t			layout;

	// methods
		GraphicsPipeline () {}
		
		GraphicsPipeline (const Description_t &descr, const Layout_t &layout) :
			descr{descr}, layout{layout} {}
		
		GraphicsPipeline (const ModulePtr &gpuThread, const Description_t &descr, const Layout_t &layout) :
			gpuThread{gpuThread}, descr{descr}, layout{layout} {}
	};


	//
	// Compute Pipeline Create Info
	//
	struct ComputePipeline
	{
	// types
		using Description_t	= Platforms::ComputePipelineDescription;
		using Layout_t		= Platforms::PipelineLayoutDescription;
		
	// variables
		ModulePtr			gpuThread;		// can be null
		Description_t		descr;
		Layout_t			layout;

	// methods
		ComputePipeline () {}

		ComputePipeline (const Description_t &descr, const Layout_t &layout) :
			descr{descr}, layout{layout} {}
		
		ComputePipeline (const ModulePtr &gpuThread, const Description_t &descr, const Layout_t &layout) :
			gpuThread{gpuThread}, descr{descr}, layout{layout} {}
	};


	//
	// Pipeline Template Create Info
	//
	struct PipelineTemplate
	{
	// types
		using Description_t	= Platforms::PipelineTemplateDescription;

	// variables
		Description_t		descr;

	// methods
		PipelineTemplate () {}
		PipelineTemplate (Description_t &&desc) : descr{RVREF(desc)} {}
		PipelineTemplate (const Description_t &desc) : descr{desc} {}
	};


	//
	// Pipeline Resource Table Create Info
	//
	struct PipelineResourceTable
	{
	// variables
		ModulePtr		gpuThread;			// can be null

	// methods
		PipelineResourceTable () {}
	};

}	// CreateInfo


namespace GpuMsg
{
	//
	// Get Graphics Pipeline Description
	//
	struct GetGraphicsPipelineDescription : _MsgBase_
	{
		Out< Platforms::GraphicsPipelineDescription >	result;
	};


	//
	// Get Compute Pipeline Description
	//
	struct GetComputePipelineDescription : _MsgBase_
	{
		Out< Platforms::ComputePipelineDescription >	result;
	};


	//
	// Get Pipeline Layout Description
	//
	struct GetPipelineLayoutDescription : _MsgBase_
	{
		Out< Platforms::PipelineLayoutDescription >		result;
	};


	//
	// Bind Buffer to Pipeline
	//
	struct PipelineAttachBuffer : ModuleMsg::AttachModule
	{
	// variables
		BytesU		offset;
		BytesU		size;

	// methods
		PipelineAttachBuffer (StringCRef name, const ModulePtr &mod, BytesU off, BytesU size) :
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
	struct GetPipelineTemplateInfo : _MsgBase_
	{
	// types
		using EShader	= Platforms::EShader;

		struct Info {
			EShader::bits		shaders;
		};

	// variables
		Out< Info >			result;
	};

	
	//
	// Create GraphicsPipelineDescription from PipelineTemplate
	//
	struct CreateGraphicsPipelineDescription : _MsgBase_
	{
	// types
		using SpecializationConstants_t	= Platforms::GraphicsPipelineDescription::SpecializationConstants_t;
		struct Result {
			Platforms::GraphicsPipelineDescription	descr;
			Platforms::PipelineLayoutDescription	layout;
		};

	// variables
		Platforms::VertexInputState		vertexInput;
		SpecializationConstants_t		constants;	// optional
		Platforms::EPrimitive::type		topology	= Uninitialized;
		Out< Result >					result;

	// methods
		CreateGraphicsPipelineDescription (const Platforms::VertexInputState &vertexInput, Platforms::EPrimitive::type topology) :
			vertexInput(vertexInput), topology(topology) {}
	};


	//
	// Create ComputePipelineDescription from PipelineTemplate
	//
	struct CreateComputePipelineDescription : _MsgBase_
	{
	// types
		using SpecializationConstants_t	= Platforms::ComputePipelineDescription::SpecializationConstants_t;
		struct Result {
			Platforms::ComputePipelineDescription	descr;
			Platforms::PipelineLayoutDescription	layout;
		};

	// variables
		Optional< uint3 >			localGroupSize;
		SpecializationConstants_t	constants;	// optional
		Out< Result >				result;

	// methods
		CreateComputePipelineDescription () {}
		explicit CreateComputePipelineDescription (const uint3 &localSize) : localGroupSize{ localSize } {}
	};


	//
	// Create GraphicsPipeline Module from PipelineTemplate
	//
	struct CreateGraphicsPipeline : _MsgBase_
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
	struct CreateComputePipeline : _MsgBase_
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
