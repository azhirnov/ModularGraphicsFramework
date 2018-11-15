// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Pipeline compiling steps:
	- deserialize GXSL or GLSL source
	- extract resources (buffers, samplers, ...)
	- translate GXSL/GLSL to GLSL without resources
	- for all shaders:
		- find same buffers type names and make align filds for GLSL/HLSL/SPIRV/CL/C++ compatibility
		- find same samplers and optimize bindings for OpenGL and DirectX 11
	- for targets:
		- use target bindings
		- parse GXSL/GLSL/HLSL with glslang
		- replace buffer fields by aligned types
		- HLSL/CL/C++: translate to target
		- SPIRV: translate with glslang and optimize with SPIRV-tools
*/

#pragma once

#include "Engine/PipelineCompiler/Common/Common.h"
#include "Engine/PipelineCompiler/Serializers/ISerializer.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"
#include "Engine/PipelineCompiler/Common/ArraySize.h"


#define __DECL_PIPELINE2( _base_, _typeName_, ... ) \
	static const struct _typeName_ final : _base_ \
	{ \
		_typeName_ () : _base_( __FILE__ ) \
		{ \
			__VA_ARGS__ \
		} \
	} g_##_typeName_; \

#define DECL_PIPELINE( _typeName_, _base_, ... ) \
	__DECL_PIPELINE2( _base_, _typeName_, __VA_ARGS__ )
	

namespace PipelineCompiler
{
	class BasePipeline;
	SHARED_POINTER( BasePipeline );



	//
	// Base Pipeline
	//

	class BasePipeline : public RefCountedObject<>
	{
		friend class PipelineManager;

	// types
	protected:

		struct Location final
		{
		// variables
			uint							index				= UMax;	// location or binding index for OpenGL
			uint							uniqueIndex			= UMax;	// binding index for Vulkan
			uint							descriptorSet		= UMax;
			
		// methods
			String BindingToStringGLSL (EShaderFormat::type shaderApi) const;
			String LocationToStringGLSL (EShaderFormat::type shaderApi) const;
		};

		struct TextureUniform
		{
		// variables
			String							name;
			EImage::type					imageType				= EImage::Tex2D;
			EPixelFormatClass::type			format					= EPixelFormatClass::AnyFloat | EPixelFormatClass::AnyNorm | EPixelFormatClass::AnyColorChannels;
			Optional<SamplerDescription>	defaultSampler;
			bool							samplerCanBeOverridden	= false;
			EShader::bits					shaderUsage;
			Location						location;
			
		// methods
			String ToStringGLSL (EShaderFormat::type shaderApi) const;
		};


		struct ImageUniform
		{
		// variables
			String							name;
			EImage::type					imageType		= EImage::Tex2D;
			EShaderMemoryModel::type		memoryModel		= EShaderMemoryModel::Coherent;
			EPixelFormat::type				format			= EPixelFormat::Unknown;
			EShader::bits					shaderUsage;
			Location						location;
			
		// methods
			String ToStringGLSL (EShaderFormat::type shaderApi) const;
		};


		struct SubpassInput
		{
		// variables
			String							name;
			uint							attachmentIndex		= UMax;
			bool							isMultisample		= false;
			EShader::bits					shaderUsage;
			Location						location;
			
		// methods
			String ToStringGLSL (EShaderFormat::type shaderApi) const;
		};


		struct _StructField : CompileTime::FastCopyable
		{
			String							name;
			String							typeName;										// for 'Struct' only
			Array<_StructField>				fields;
			EShaderVariable::type			type			= EShaderVariable::Unknown;
			EPrecision::type				precision		= EPrecision::Default;
			EVariableQualifier::bits		qualifier;										// only for varyings
			EShaderMemoryModel::type		memoryModel		= EShaderMemoryModel::Default;	// for image and buffer
			EVariablePacking::bits			packing;
			EPixelFormat::type				format			= EPixelFormat::Unknown;		// for image only
			ArraySize						arraySize;
			BytesU							offset			= ~0_b;
			BytesU							align			= 0_b;
			BytesU							stride			= 0_b;

			bool operator == (const _StructField &right) const;
			bool operator != (const _StructField &right) const		{ return not (*this == right); }
		};
		using StructTypes	= HashMap< /*typename*/String, _StructField >;


		struct UniformBuffer
		{
		// variables
			String							name;
			String							typeName;
			BytesU							size;
			EVariablePacking::type			packing			= EVariablePacking::Std140;
			EShader::bits					shaderUsage;
			Location						location;
			
		// methods
			String ToStringGLSL (StringCRef fields, EShaderFormat::type shaderApi) const;
		};


		struct StorageBuffer
		{
		// variables
			String							name;
			String							typeName;
			BytesU							staticSize;
			BytesU							arrayStride;	// for dynamic part of buffer
			EShaderMemoryModel::type		memoryModel		= EShaderMemoryModel::Coherent;
			EVariablePacking::type			packing			= EVariablePacking::Std430;
			EShader::bits					shaderUsage;
			Location						location;
			
		// methods
			String ToStringGLSL (StringCRef fields, EShaderFormat::type shaderApi) const;
		};


		struct Bindings
		{
		// types
			using _Uniform = Union< TextureUniform, ImageUniform, SubpassInput, UniformBuffer, StorageBuffer >;

		// variables
			Array<_Uniform>		uniforms;

		// methods
			Bindings () {}

			Bindings&  Texture (EImage::type imageType, StringCRef name, EShader::bits shaderUsage, EPixelFormatClass::type format);

			Bindings&  Sampler (StringCRef texName, const SamplerDescription &descr, bool canBeOverridden = false);	// set default sampler

			Bindings&  Image (EImage::type imageType, StringCRef name, EPixelFormat::type format, EShader::bits shaderUsage,
							  EShaderMemoryModel::type access = EShaderMemoryModel::Default);
			
			Bindings&  Subpass (StringCRef name, uint attachmentIndex, bool isMultisample, EShader::bits shaderUsage);

			Bindings&  UniformBuffer (StringCRef name, StringCRef typeName, EShader::bits shaderUsage);
			Bindings&  StorageBuffer (StringCRef name, StringCRef typeName, EShader::bits shaderUsage,
									  EShaderMemoryModel::type access = EShaderMemoryModel::Default);
		};
		
		struct _VaryingField : CompileTime::FastCopyable
		{
		// variables
			String						name;
			String						typeName;
			EShaderVariable::type		type		= EShaderVariable::Unknown;
			EPrecision::type			precision	= EPrecision::Default;
			EVariableQualifier::bits	qualifier;
			ArraySize					arraySize;
			uint						location	= UMax;
			
		// methods
			ND_ bool operator == (const _VaryingField &) const;
		};

		struct Varying : _VaryingField
		{
		// variables
			Array<_VaryingField>		fields;
			
		// methods
			ND_ bool operator == (const Varying &) const;
		};


		struct ShaderModule
		{
		// variables
			Array<String>			_source;		// origin source
			Array<Varying>			_io;			// for compute shader must be empty
			TimeL					_maxEditTime;

			String					entry;
			const EShader::type		type;
		
		// methods
			ShaderModule () : type{EShader::Unknown} {}
			ShaderModule (const ShaderModule &other);

			explicit ShaderModule (EShader::type shaderType) : entry("main"), type(shaderType) {}
			
			ShaderModule& operator = (const ShaderModule &right);

			ShaderModule& Load (StringCRef path);
			ShaderModule& Load (StringCRef path, StringCRef defines);
			ShaderModule& Load (StringCRef path, StringCRef filename, StringCRef defines);
			ShaderModule& Source (StringCRef src);

			ShaderModule& Depends (StringCRef filename);

			ND_ bool IsEnabled () const;

			ND_ TimeL LastEditTime () const;
		};


		struct ShaderDisasembly
		{
		// variables
			Array<Varying>				input;
			Array<Varying>				output;
			Array<_StructField>			structTypes;
			Array<TextureUniform>		textures;
			Array<ImageUniform>			images;
			Array<SubpassInput>			subpasses;
			Array<UniformBuffer>		uniformBuffers;
			Array<StorageBuffer>		storageBuffers;

		// methods
			ShaderDisasembly () {}

			ND_ bool IsEnabled () const		{ return not input.Empty() or not output.Empty(); /*not source.Empty()*/; }	// TODO
		};

		using ShaderDisasemblies = StaticArray< ShaderDisasembly, EShader::_Count >;

		using CompiledShader_t = Map< EShaderFormat::type, BinaryArray >;


	public:
		struct ConverterConfig
		{
			// add custom includings.
			Array<String>					includings;

			// declare namespace for functions and types.
			String							nameSpace;

			// set target mask.
			Set<EShaderFormat::type>	targets;

			// error will be generated if file that program trying to write is already created.
			bool							errorIfFileExist		= false;

			// you can recompile shaders without changing C++ code,
			// error will be generated if new C++ code is different than current code.
			//bool							updateShaders			= false;

			// set 'false' for run-time shader editing.
			bool							optimizeSource			= false;

			// all same structs and blocks in shader will be saved to separate file.
			bool							searchForSharedTypes	= true;

			// padding fields for same align between glsl and cpp
			bool							addPaddingToStructs		= true;

			// search for same names and set same locations/binding indices for all shaders.
			bool							optimizeBindings		= true;
			bool							optimizeVertexInput		= false;
			bool							optimizeFragmentOutput	= false;

			// validate comiled shader to check errors
			bool							validation				= false;

			// allow minimal rebuild based on file modification time.
			bool							minimalRebuild			= true;
		};


	private:
		struct _BindingsToString_Func;
		struct _MergeTextures_Func;
		struct _MergeImages_Func;
		struct _MergeSubpasses_Func;
		struct _MergeUniformBuffers_Func;
		struct _MergeStorageBuffers_Func;
		struct _BindingsToLayout_Func;
		struct _CalculateOffsets_Helper;
		struct _ReplaceTypes_Helper;
		struct _UpdateBindingIndices_Func;
		struct _UpdateDescriptorSets_Func;
		struct _UpdateBufferSizes_Func;


	// variables
	private:
		const String			_path;
		String					_name;

	protected:
		mutable StructTypes		_structTypes;
		mutable StructTypes		_originTypes;
		TimeL					_lastEditTime;
		
		VertexAttribs			attribs;
		FragmentOutputState		fragOutput;
		bool					earlyFragmentTests	= true;

		uint					patchControlPoints	= 0;
		uint3					localGroupSize;

		Bindings				bindings;
		EShaderFormat::type		shaderFormat;


	// methods
	protected:
		explicit BasePipeline (StringCRef path);
		explicit BasePipeline (StringCRef path, StringCRef name);

		BasePipeline (const BasePipeline &) = delete;
		BasePipeline (BasePipeline &&) = delete;

		virtual ~BasePipeline ();
		

	public:
		ND_ StringCRef	Name () const			{ return _name; }
		ND_ String		Path () const;
		ND_ TimeL		LastEditTime () const	{ return _lastEditTime; }

		void Depends (StringCRef filename);

		virtual bool Prepare (const ConverterConfig &cfg) = 0;
		virtual bool Convert (OUT String &src, Ptr<ISerializer> ser, const ConverterConfig &cfg) const = 0;


	// Preparing Pass (1) //
	protected:
		//virtual bool Prepare (const ConverterConfig &cfg) = 0;
		
		bool _DisasembleShader (const ConverterConfig &cfg, INOUT ShaderModule &shader, OUT ShaderDisasembly &compiled);
		bool _UpdateBindings ();
		bool _UpdateBufferSizes ();
		bool _UpdateDescriptorSets ();
		
		void _AddBindings (INOUT Array<TextureUniform> &textures);
		void _AddBindings (INOUT Array<ImageUniform> &images);
		void _AddBindings (INOUT Array<SubpassInput> &subpasses);
		void _AddBindings (INOUT Array<UniformBuffer> &buffers);
		void _AddBindings (INOUT Array<StorageBuffer> &buffers);

		static bool _MergeStructTypes (const Array<_StructField> &newTypes, INOUT StructTypes &currTypes);
		static bool _ExtractTextures (const DeserializedShader &shader, OUT Array<TextureUniform> &result);
		static bool _ExtractImages (const DeserializedShader &shader, OUT Array<ImageUniform> &result);
		static bool _ExtractSubpasses (const DeserializedShader &shader, OUT Array<SubpassInput> &result);
		static bool _ExtractUniformBuffers (const DeserializedShader &shader, OUT Array<UniformBuffer> &result);
		static bool _ExtractStorageBuffers (const DeserializedShader &shader, OUT Array<StorageBuffer> &result);
		static bool _ExtractTypes (const DeserializedShader &shader, OUT Array<_StructField> &result);
		static bool _ExtractVaryings (const DeserializedShader &shader, OUT Array<Varying> &input, OUT Array<Varying> &output);
		static bool _ExtractAttribs (const Array<Varying> &input, OUT VertexAttribs &attribs);
		static bool _ExtractFragOutput (const Array<Varying> &output, OUT FragmentOutputState &fragOutput);
		static bool _AttribsToStructTypes (StringCRef name, const VertexAttribs &attribs, INOUT StructTypes &types);

		static bool _RecursiveProcessBuffer (const Array<DeserializedShader::BufferVariable> &fields, EVariablePacking::type packing,
											 OUT _StructField &curr, INOUT Array<_StructField> &result);
		
		static bool _RecursiveProcessVarying (const Array<DeserializedShader::IOVariable> &fields,
											  INOUT _StructField *curr, INOUT Array<_StructField> &result);
		
		static bool _RecursiveProcessVarying (const Array<DeserializedShader::IOVariable> &fields, StringCRef parent,
											  INOUT Array<Varying> &input, INOUT Array<Varying> &output);


	// Converting Pass (2) //
	protected:
		//virtual bool Convert (OUT String &src, Ptr<ISerializer> ser, const ConverterConfig &cfg) const = 0;
		
		bool _ConvertLayout (StringCRef name, INOUT String &src, Ptr<ISerializer> ser) const;
		void _BindingsToString (EShader::type shaderType, EShaderFormat::type shaderApi, bool useOriginTypes, OUT String &str) const;
		bool _TypeReplacer (StringCRef typeName, INOUT ShaderCompiler::FieldTypeInfo &field) const;
		bool _CompileShader (const ShaderModule &shader, const ConverterConfig &cfg, OUT CompiledShader_t &compiled) const;

		ND_ static String _StructToString (const StructTypes &types, StringCRef typeName, bool skipLayouts);

		static bool _CalculateOffsets (INOUT StructTypes &structTypes);
		static bool _AddPaddingToStructs (INOUT StructTypes &structTypes);
		static bool _StructsToString (const StructTypes &structTypes, OUT String &glslSource);
		static bool _SerializeStructs (const StructTypes &structTypes, Ptr<ISerializer> ser, OUT String &serialized);
		static bool _MergeStructTypes (const StructTypes &newTypes, INOUT StructTypes &currTypes);

		ND_ static bool _ValidateShader (EShader::type, const CompiledShader_t &);


	// Utils for Preparing and Converting Passes //
	private:
		bool _OnCompilationFailed (EShader::type shaderType, EShaderFormat::type fmt, ArrayCRef<StringCRef> source, StringCRef log) const;
		
		static void _VaryingsToString (const Array<Varying> &varyings, OUT String &str);

		static bool _AddStructType (const _StructField &structType, INOUT StructTypes &currTypes);

		ND_ static String     _GetVersionGLSL (EShaderFormat::type fmt);
		ND_ static StringCRef _GetDefaultHeaderGLSL ();
		ND_ static StringCRef _GetTypeRedefinitionGLSL ();
		ND_ static StringCRef _GetPerShaderHeaderGLSL (EShader::type type);
		ND_ static String     _LocalGroupSizeToStringGLSL (const uint3 &value);
	};


}	// PipelineCompiler
