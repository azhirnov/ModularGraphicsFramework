// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Common/ShaderEnums.h"

namespace PipelineCompiler
{

	//
	// Deserialized Shader (from source)
	//

	struct DeserializedShader
	{
		friend class ShaderCompiler;

	// types
	public:
		struct _Token : CompileTime::ComplexType
		{
			String							source;					// part of source code
			usize							line				= 0;
			usize							column				= 0;
		};


		struct TextureUniform final : _Token
		{
			String							name;
			uint							unit				= UMax;
			uint							descriptorSet		= UMax;
			EImage::type					imageType			= EImage::Tex2D;
			EPixelFormatClass::type			format				= EPixelFormatClass::Unknown;
			bool							isShadow			= false;

			String ToString () const;
		};


		struct ImageUniform final : _Token
		{
			String							name;
			uint							unit				= UMax;
			uint							descriptorSet		= UMax;
			EImage::type					imageType			= EImage::Tex2D;
			EGpuMemoryModel::type			memoryModel			= EGpuMemoryModel::Coherent;
			EPixelFormat::type				format				= EPixelFormat::Unknown;
			
			String ToString () const;
		};
		

		struct _Variable : _Token
		{
			String							name;
			String							typeName;											// for 'Struct' only
			EShaderVariable::type			type				= EShaderVariable::Unknown;
			EPrecision::type				precision			= EPrecision::Default;
			EVariableQualifier::bits		qualifier;
			EGpuMemoryModel::type			memoryModel			= EGpuMemoryModel::Default;		// for image and buffer
			EPixelFormat::type				format				= EPixelFormat::Unknown;		// for image only
			uint							arraySize			= 1;							// 0 - dynamic array	// TODO: array of array
			
			String _ToString () const;
		};


		struct Uniform final : _Variable
		{
			uint							location			= ~0u;

			String ToString () const;
		};


		struct BufferVariable final : _Variable
		{
			Array<BufferVariable>			fields;												// for 'Struct' only
			BytesU							offset				= ~0_b;
			BytesU							stride				= 0_b;							// size of 1 element in array
			BytesU							align				= 0_b;							// explicit defined align, 0 - default

			String ToString () const;
		};


		struct UniformBuffer final : _Token
		{
			String							name;
			String							typeName;											// type of uniform buffer
			Array<BufferVariable>			fields;
			uint							bindingIndex		= UMax;
			uint							descriptorSet		= UMax;
			EVariablePacking::type			packing				= EVariablePacking::Unknown;
			BytesU							size				= 0_b;
			
			String ToString () const;
		};


		struct StorageBuffer final : _Token
		{
			String							name;
			String							typeName;											// type of shader storage buffer
			Array<BufferVariable>			fields;
			uint							bindingIndex		= UMax;
			uint							descriptorSet		= UMax;
			EGpuMemoryModel::type			memoryModel			= EGpuMemoryModel::Coherent;
			EVariablePacking::type			packing				= EVariablePacking::Unknown;
			BytesU							size				= 0_b;
			
			String ToString () const;
		};


		struct PushConstant final : _Variable
		{
			Array< PushConstant >			fields;	
			BytesU							offset;
			BytesU							size;

			String ToString () const;
		};


		struct IOVariable final : _Variable
		{
			Array< IOVariable >				fields;											// for 'Struct' only
			uint							location			= UMax;

			String ToString () const;
		};


		struct Constant final : _Variable
		{
			using Value_t		= Union< bool, bool2, bool3, bool4,
										 int, int2, int3, int4,				uint, uint2, uint3, uint4,
										 ilong, ilong2, ilong3, ilong4,		ulong, ulong2, ulong3, ulong4,
										 float, float2, float3, float4,		float2x2, float2x3, float2x4,
										 float3x2, float3x3, float3x4,		float4x2, float4x3, float4x4,
										 double, double2, double3, double4,	double2x2, double2x3, double2x4,
										 double3x2, double3x3, double3x4,	double4x2, double4x3, double4x4 >;
			using ValueArray_t	= Array< Value_t >;

			ValueArray_t	values;

			String ToString () const;
		};


		struct FunctionArgument final : _Variable
		{
			Array< FunctionArgument >		fields;											// for 'Struct' only

			String ToString () const;
		};


		struct Function final : _Token
		{
			String							name;
			String							signature;
			Array<FunctionArgument>			args;
			FunctionArgument				result;
			
			String ToString () const;
		};

		// TODO: subroutine, transform-feedback


		struct VertexShaderInfo final
		{
			String ToString () const;
		};


		struct TessControlShaderInfo final
		{
			uint								patchSize			= 0;	// output control points

			String ToString () const;
		};


		struct TessEvaluationShaderInfo final
		{
			ETessellationSpacing::type			spacing				= ETessellationSpacing::Unknown;
			ETessellationInputPrimitive::type	primitive			= ETessellationInputPrimitive::Unknown;
			bool								ccw					= true;

			String ToString () const;
		};


		struct GeometryShaderInfo final
		{
			EGeometryInputPrimitive::type		inputPrimitive		= EGeometryInputPrimitive::Unknown;
			EGeometryOutputPrimitive::type		outputPrimitive		= EGeometryOutputPrimitive::Unknown;
			uint								invocations			= 0;
			uint								maxOutputVertices	= 0;

			String ToString () const;
		};


		struct FragmentShaderInfo final
		{
			EFragmentShaderParams::type			flags				= EFragmentShaderParams::None;

			String ToString () const;
		};


		struct ComputeShaderInfo final
		{
			uint3								localSize;		// 0 - auto size

			String ToString () const;
		};


	// variables
	private:
		Array<TextureUniform>				_textures;
		Array<ImageUniform>					_images;
		Array<Uniform>						_uniforms;			// without samplers, images, subpass, subroutine...

		Array<UniformBuffer>				_uniformBuffers;
		Array<StorageBuffer>				_storageBuffers;
		Array<PushConstant>					_pushConstants;

		Array<IOVariable>					_variables;			// shader input/output variables only, including builtin variables
		Array<Constant>						_constants;			// shader local constants
		Array<Function>						_functions;

		Array<String>						_extensions;

		VertexShaderInfo					_vertex;
		TessControlShaderInfo				_tessControl;
		TessEvaluationShaderInfo			_tessEvaluation;
		GeometryShaderInfo					_geometry;
		FragmentShaderInfo					_fragment;
		ComputeShaderInfo					_compute;

		EShader::type						_shaderType				= EShader::Unknown;


	// methods
	public:
		DeserializedShader () {}
		
		String ToString () const;

		bool CalculateOffsets ();
		bool CalculateLocations ();
		bool ReplaceTypesInStructs ();

		EShader::type					Type ()					const	{ return _shaderType; }

		ArrayCRef<TextureUniform>		Textures ()				const	{ return _textures; }
		ArrayCRef<ImageUniform>			Images ()				const	{ return _images; }
		ArrayCRef<Uniform>				Uniforms ()				const	{ return _uniforms; }

		ArrayCRef<UniformBuffer>		UniformBuffers ()		const	{ return _uniformBuffers; }
		ArrayCRef<StorageBuffer>		StorageBuffers ()		const	{ return _storageBuffers; }
		ArrayCRef<PushConstant>			PushConstants ()		const	{ return _pushConstants; }

		ArrayCRef<IOVariable>			Variables ()			const	{ return _variables; }
		ArrayCRef<Constant>				Constants ()			const	{ return _constants; }
		ArrayCRef<Function>				Functions ()			const	{ return _functions; }

		ArrayCRef<String>				Extensions ()			const	{ return _extensions; }

		VertexShaderInfo const&			VertexShader ()			const	{ return _vertex; }
		TessControlShaderInfo const&	TessControlShader ()	const	{ return _tessControl; }
		TessEvaluationShaderInfo const&	TessEvaluationShader ()	const	{ return _tessEvaluation; }
		GeometryShaderInfo const&		GeometryShader ()		const	{ return _geometry; }
		FragmentShaderInfo const&		FragmentShader ()		const	{ return _fragment; }
		ComputeShaderInfo const&		ComputeShader ()		const	{ return _compute; }
	};

	
}	// PipelineCompiler
