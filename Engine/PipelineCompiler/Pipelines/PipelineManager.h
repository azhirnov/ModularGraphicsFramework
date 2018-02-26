// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Pipelines/BasePipeline.h"

namespace PipelineCompiler
{

	//
	// Pipeline Manager
	//

	class PipelineManager final
	{
	// types
	public:
		using ConverterConfig	= BasePipeline::ConverterConfig;

	private:
		using StructTypes		= BasePipeline::StructTypes;
		using BindableTypes		= BasePipeline::Bindings::_Uniform::TypeList_t;
		using NamedTypes		= MultiHashMap< String, BasePipeline::Bindings::_Uniform >;
		using SharedTypes		= StaticArray< NamedTypes, BindableTypes::Count >;
		using TextureUniform	= BasePipeline::TextureUniform;
		using ImageUniform		= BasePipeline::ImageUniform;
		using UniformBuffer		= BasePipeline::UniformBuffer;
		using StorageBuffer		= BasePipeline::StorageBuffer;

		struct _AddBinding_Func;
		struct _ReplaceBinding_Func;


	// variables
	private:
		Set< BasePipeline* >	_pipelines;
		mutable StructTypes		_structTypes;
		mutable SharedTypes		_bindings;


	// methods
	public:
		PipelineManager ();
		~PipelineManager ();

		void Add (Ptr<BasePipeline> pp);
		void Remove (Ptr<BasePipeline> pp);

		bool Convert (StringCRef filename, Ptr<ISerializer> ser, const ConverterConfig &cfg) const;

		static Ptr<PipelineManager>	Instance ();

	private:
		bool _ProcessSharedTypes (StringCRef path, Ptr<ISerializer> ser, INOUT ConverterConfig &cfg) const;

		bool _SaveSharedTypes (StringCRef path, Ptr<ISerializer> ser, StringCRef nameSpace, OUT String &glslSource, OUT String &filename) const;
	};


}	// PipelineCompiler
