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
		using Varying			= BasePipeline::Varying;
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
		Set< BasePipeline* >	_pipelines;		// registered pipelines
		mutable StructTypes		_sharedStructTypes;
		mutable SharedTypes		_sharedBindings;
		mutable Array<Varying>	_sharedVertexInput;
		mutable Array<Varying>	_sharedFragmentOutput;


	// methods
	public:
		PipelineManager ();
		~PipelineManager ();

		void Add (Ptr<BasePipeline> pp);
		void Remove (Ptr<BasePipeline> pp);

		bool ConvertAll (StringCRef filename, Ptr<ISerializer> ser, const ConverterConfig &cfg) const;
		
		bool Convert (ArrayCRef<BasePipeline *> pipelines, StringCRef filename, Ptr<ISerializer> ser, const ConverterConfig &cfg) const;
		bool Convert (ArrayCRef<BasePipelinePtr> pipelines, StringCRef filename, Ptr<ISerializer> ser, const ConverterConfig &cfg) const;

		static Ptr<PipelineManager>	Instance ();

	private:
		template <typename PplnCollection>
		bool _Convert (const PplnCollection &pipelines, StringCRef filename, Ptr<ISerializer> ser, const ConverterConfig &cfg) const;
		
		template <typename PplnCollection>
		bool _ProcessSharedTypes (const PplnCollection &pipelines, Ptr<ISerializer> ser, OUT String &fileSource, INOUT ConverterConfig &cfg) const;

		bool _SaveSharedTypes (Ptr<ISerializer> ser, StringCRef nameSpace, OUT String &fileSource) const;

		static bool _MergeVertexInput (const ArrayCRef<Varying> &input, INOUT Array<Varying> &output);
		static bool _MergeFragmentOutput (const ArrayCRef<Varying> &input, INOUT Array<Varying> &output);
	};

}	// PipelineCompiler
