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
		- GLSL: optimize with LunarGLASS
		- HLSL/CL/C++: translate to target
		- SPIRV: translate with LunarGLASS and optimize with SPIRV-tools
*/

#pragma once

#include "Engine/PipelineCompiler/Common/Common.h"
#include "Engine/PipelineCompiler/Serializers/ISerializer.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"


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
			String BindingToStringGLSL (EShaderType shaderApi) const;
			String LocationToStringGLSL (EShaderType shaderApi) const;
		};

		struct TextureUniform
		{
		// variables
			String							name;
			EImage::type					imageType				= EImage::Tex2D;
			EPixelFormatClass::type			format					= EPixelFormatClass::AnyFloat | EPixelFormatClass::RGBA;
			Optional<SamplerDescriptor>		defaultSampler;
			bool							samplerCanBeOverridden	= false;
			EShader::bits					shaderUsage;
			Location						location;
			
		// methods
			String ToStringGLSL (EShaderType shaderApi) const;
		};


		struct ImageUniform
		{
		// variables
			String							name;
			EImage::type					imageType		= EImage::Tex2D;
			EGpuMemoryModel::type			memoryModel		= EGpuMemoryModel::Coherent;
			EPixelFormat::type				format			= EPixelFormat::Unknown;
			EShader::bits					shaderUsage;
			Location						location;
			
		// methods
			String ToStringGLSL (EShaderType shaderApi) const;
		};


		struct _StructField : CompileTime::FastCopyable
		{
			String							name;
			String							typeName;											// for 'Struct' only
			Array<_StructField>				fields;
			EShaderVariable::type			type				= EShaderVariable::Unknown;
			EPrecision::type				precision			= EPrecision::Default;
			EVariableQualifier::bits		qualifier;											// only for varyings
			EGpuMemoryModel::type			memoryModel			= EGpuMemoryModel::None;		// for image only
			EVariablePacking::bits			packing;
			//EPixelFormat::type			format				= EPixelFormat::Unknown;		// for image only
			uint							arraySize			= 1;							// 0 - dynamic array	// TODO: array of array
			BytesU							offset				= ~0_b;
			BytesU							align				= 0_b;
			BytesU							stride				= 0_b;

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
			String ToStringGLSL (StringCRef fields, EShaderType shaderApi) const;
		};


		struct StorageBuffer
		{
		// variables
			String							name;
			String							typeName;
			BytesU							staticSize;
			BytesU							arrayStride;	// for dynamic part of buffer
			EGpuMemoryModel::type			memoryModel		= EGpuMemoryModel::Coherent;
			EVariablePacking::type			packing			= EVariablePacking::Std430;
			EShader::bits					shaderUsage;
			Location						location;
			
		// methods
			String ToStringGLSL (StringCRef fields, EShaderType shaderApi) const;
		};


		struct Bindings
		{
		// types
			using _Uniform = Union< TextureUniform, ImageUniform, UniformBuffer, StorageBuffer >;

		// variables
			Array<_Uniform>		uniforms;

		// methods
			Bindings () {}

			Bindings&  Texture (EImage::type imageType, StringCRef name, EShader::bits shaderUsage = EShader::bits().SetAll(),
								EPixelFormatClass::type format = EPixelFormatClass::AnyFloat | EPixelFormatClass::RGBA);

			Bindings&  Sampler (StringCRef texName, const SamplerDescriptor &descr, bool canBeOverridden = false);	// set default sampler

			Bindings&  Image (EImage::type imageType, StringCRef name, EPixelFormat::type format, EShader::bits shaderUsage = EShader::bits().SetAll(),
							  EGpuMemoryModel::type access = EGpuMemoryModel::Coherent);

			Bindings&  UniformBuffer (StringCRef name, StringCRef typeName, EShader::bits shaderUsage = EShader::bits().SetAll());
			Bindings&  StorageBuffer (StringCRef name, StringCRef typeName, EShader::bits shaderUsage = EShader::bits().SetAll(),
									  EGpuMemoryModel::type access = EGpuMemoryModel::Coherent);

			// helpers
			template <typename T> Bindings&  Texture2D (StringCRef name, EShader::bits shaderUsage = EShader::bits().SetAll());
			template <typename T> Bindings&  Texture2DArray (StringCRef name, EShader::bits shaderUsage = EShader::bits().SetAll());
		
			template <typename T> Bindings&  Image2D (StringCRef name, EShader::bits shaderUsage = EShader::bits().SetAll(),
													  EGpuMemoryModel::type access = EGpuMemoryModel::Coherent);
			template <typename T> Bindings&  Image2DMS (StringCRef name, EShader::bits shaderUsage = EShader::bits().SetAll(),
														EGpuMemoryModel::type access = EGpuMemoryModel::Coherent);
			template <typename T> Bindings&  Image2DArray (StringCRef name, EShader::bits shaderUsage = EShader::bits().SetAll(),
														   EGpuMemoryModel::type access = EGpuMemoryModel::Coherent);
			template <typename T> Bindings&  Image2DMSArray (StringCRef name, EShader::bits shaderUsage = EShader::bits().SetAll(),
															 EGpuMemoryModel::type access = EGpuMemoryModel::Coherent);
			template <typename T> Bindings&  ImageCube (StringCRef name, EShader::bits shaderUsage = EShader::bits().SetAll(),
														EGpuMemoryModel::type access = EGpuMemoryModel::Coherent);
			template <typename T> Bindings&  Image3D (StringCRef name, EShader::bits shaderUsage = EShader::bits().SetAll(),
													  EGpuMemoryModel::type access = EGpuMemoryModel::Coherent);
		};


		struct Varying
		{
		// variables
			String						name;
			String						typeName;
			EShaderVariable::type		type		= EShaderVariable::Unknown;
			EPrecision::type			precision	= EPrecision::Default;
			EVariableQualifier::bits	qualifier;
			uint						arraySize	= 1;
			uint						location	= UMax;
			
		// methods
			bool operator == (const Varying &) const;
		};


		struct ShaderModule
		{
		// variables
			Array<String>			_source;		// origin source
			Array<Varying>			_io;			// for compute shader must be empty

			String					entry;
			const EShader::type		type;
		
		// methods
			explicit ShaderModule (EShader::type shaderType) : entry("main"), type(shaderType) {}
			
			ShaderModule& Load (StringCRef path);
			ShaderModule& Load (StringCRef path, StringCRef defines);
			ShaderModule& Load (StringCRef path, StringCRef filename, StringCRef defines);
			ShaderModule& Source (StringCRef src);

			bool IsEnabled () const;
		};


		struct ShaderDisasembly
		{
		// variables
			Array<Varying>				input;
			Array<Varying>				output;
			Array<_StructField>			structTypes;
			Array<TextureUniform>		textures;
			Array<ImageUniform>			images;
			Array<UniformBuffer>		uniformBuffers;
			Array<StorageBuffer>		storageBuffers;

		// methods
			ShaderDisasembly () {}

			bool IsEnabled () const		{ return not input.Empty() or not output.Empty(); /*not source.Empty()*/; }	// TODO
		};

		using ShaderDisasemblies = StaticArray< ShaderDisasembly, EShader::_Count >;


		struct CompiledShader
		{
			BinaryArray		glsl;
			BinaryArray		glslBinary;
			
			BinaryArray		spirv;
			BinaryArray		spirvAsm;
			
			BinaryArray		cl;
			BinaryArray		clAsm;

			BinaryArray		cpp;

			BinaryArray		hlsl;
			BinaryArray		hlslBinary;
		};


	public:
		struct ConverterConfig
		{
			// reserved
			String						_glslTypes;

			// add custom includings.
			Array<String>				includings;

			// declare namespace for functions and types.
			String						nameSpace;

			// set target mask.
			EShaderDstFormat::bits		target;

			// error will be generated if file that program trying to write is already created.
			bool						errorIfFileExist		= false;

			// you can recompile shaders without changing C++ code,
			// error will be generated if new C++ code is different than current code.
			//bool						updateShaders			= false;

			// set 'false' for run-time shader editing.
			bool						optimizeSource			= true;

			// all same structs and blocks in shader will be saved to separate file.
			bool						searchForSharedTypes	= true;

			// padding fields for same align between glsl and cpp
			bool						addPaddingToStructs		= true;

			// search for same names and set same locations/binding indices for all shaders.
			bool						optimizeBindings		= true;

			// 
			bool						obfuscate				= false;

			// validate comiled shader to check errors
			bool						validation				= false;
		};


	private:
		struct _BindingsToString_Func;
		struct _MergeTextures_Func;
		struct _MergeImages_Func;
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
		
		VertexAttribs			attribs;
		FragmentOutputState		fragOutput;
		uint					patchControlPoints	= 0;
		uint3					localGroupSize;

		Bindings				bindings;
		EShaderSrcFormat::type	shaderFormat;


	// methods
	protected:
		explicit BasePipeline (StringCRef name);

		BasePipeline (const BasePipeline &) = delete;
		BasePipeline (BasePipeline &&) = delete;

		virtual ~BasePipeline ();
		

	public:
		StringCRef	Name () const	{ return _name; }
		StringCRef	Path () const	{ return _path; }

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
		void _AddBindings (INOUT Array<UniformBuffer> &buffers);
		void _AddBindings (INOUT Array<StorageBuffer> &buffers);

		static bool _MergeStructTypes (const Array<_StructField> &newTypes, INOUT StructTypes &currTypes);
		static bool _ExtractTextures (const DeserializedShader &shader, OUT Array<TextureUniform> &result);
		static bool _ExtractImages (const DeserializedShader &shader, OUT Array<ImageUniform> &result);
		static bool _ExtractUniformBuffers (const DeserializedShader &shader, OUT Array<UniformBuffer> &result);
		static bool _ExtractStorageBuffers (const DeserializedShader &shader, OUT Array<StorageBuffer> &result);
		static bool _ExtractTypes (const DeserializedShader &shader, OUT Array<_StructField> &result);
		static bool _ExtractVaryings (const DeserializedShader &shader, OUT Array<Varying> &input, OUT Array<Varying> &output);
		static bool _ExtractAttribs (const Array<Varying> &input, OUT VertexAttribs &attribs);
		static bool _ExtractFragOutput (const Array<Varying> &output, OUT FragmentOutputState &fragOutput);

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
		bool _CompileShader (const ShaderModule &shader, const ConverterConfig &cfg, OUT CompiledShader &compiled) const;
		void _BindingsToString (EShader::type shaderType, EShaderType shaderApi, bool useOriginTypes, OUT String &str) const;
		bool _TypeReplacer (StringCRef typeName, INOUT ShaderCompiler::FieldTypeInfo &field) const;

		static String _StructToString (const StructTypes &types, StringCRef typeName, bool skipLayouts);

		static bool _CalculateOffsets (INOUT StructTypes &structTypes);
		static bool _AddPaddingToStructs (INOUT StructTypes &structTypes);
		static bool _AllStructsToString (const StructTypes &structTypes, Ptr<ISerializer> ser, OUT String &serialized, OUT String &glsl);
		static bool _MergeStructTypes (const StructTypes &newTypes, INOUT StructTypes &currTypes);

		static bool _ValidateShader (EShader::type, const CompiledShader &);


	// Utils for Preparing and Converting Passes //
	private:
		bool _OnCompilationFailed (EShader::type shaderType, EShaderSrcFormat::type fmt, ArrayCRef<StringCRef> source, StringCRef log) const;
		
		String _VaryingsToString (const Array<Varying> &varyings) const;
		
		static bool _AddStructType (const _StructField &structType, INOUT StructTypes &currTypes);

		static String     _GetVersionGLSL ();
		static StringCRef _GetDefaultHeaderGLSL ();
		static StringCRef _GetPerShaderHeaderGLSL (EShader::type type);
	};


/*
	template <typename T>
	inline BasePipeline::Bindings&  BasePipeline::Bindings::Texture2D (StringCRef name, EShader::bits shaderUsage) {
		return Texture( EImage::Tex2D, name, shaderUsage, EPixelFormatClass::From<T>() );
	}

	template <typename T>
	inline BasePipeline::Bindings&  BasePipeline::Bindings::Texture2DArray (StringCRef name, EShader::bits shaderUsage) {
		return Texture( EImage::Tex2DArray, name, shaderUsage, EPixelFormatClass::From<T>() );
	}
		
	template <typename T>
	inline BasePipeline::Bindings&  BasePipeline::Bindings::Image2D (StringCRef name, EShader::bits shaderUsage, EGpuMemoryModel::type access) {
		return Image( EImage::Tex2D, name, EPixelFormat::From<T>(), shaderUsage, access );
	}

	template <typename T>
	inline BasePipeline::Bindings&  BasePipeline::Bindings::Image2DMS (StringCRef name, EShader::bits shaderUsage, EGpuMemoryModel::type access) {
		return Image( EImage::Tex2DMS, name, EPixelFormat::From<T>(), shaderUsage, access );
	}

	template <typename T>
	inline BasePipeline::Bindings&  BasePipeline::Bindings::Image2DArray (StringCRef name, EShader::bits shaderUsage, EGpuMemoryModel::type access) {
		return Image( EImage::Tex2DArray, name, EPixelFormat::From<T>(), shaderUsage, access );
	}

	template <typename T>
	inline BasePipeline::Bindings&  BasePipeline::Bindings::Image2DMSArray (StringCRef name, EShader::bits shaderUsage, EGpuMemoryModel::type access) {
		return Image( EImage::Tex2DMSArray, name, EPixelFormat::From<T>(), shaderUsage, access );
	}

	template <typename T>
	inline BasePipeline::Bindings&  BasePipeline::Bindings::ImageCube (StringCRef name, EShader::bits shaderUsage, EGpuMemoryModel::type access) {
		return Image( EImage::TexCube, name, EPixelFormat::From<T>(), shaderUsage, access );
	}

	template <typename T>
	inline BasePipeline::Bindings&  BasePipeline::Bindings::Image3D (StringCRef name, EShader::bits shaderUsage, EGpuMemoryModel::type access) {
		return Image( EImage::Tex3D, name, EPixelFormat::From<T>(), shaderUsage, access );
	}*/


}	// PipelineCompiler
