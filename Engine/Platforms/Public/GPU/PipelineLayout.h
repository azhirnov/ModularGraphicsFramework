// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/ShaderEnums.h"
#include "Engine/Platforms/Public/GPU/ImageEnums.h"
#include "Engine/Platforms/Public/GPU/VertexEnums.h"
#include "Core/STL/Types/Bool.h"

namespace Engine
{
namespace Platforms
{
	using Bool32	= GX_STL::GXTypes::Bool< int >;
	using Bool32_2	= GX_STL::GXMath::Vec< Bool32, 2 >;
	using Bool32_3	= GX_STL::GXMath::Vec< Bool32, 3 >;
	using Bool32_4	= GX_STL::GXMath::Vec< Bool32, 4 >;



	//
	// Pipeline Layout Description
	//

	struct PipelineLayoutDescription final : CompileTime::PODStruct
	{
	// types
	public:
		struct Builder;

		using Self		= PipelineLayoutDescription;
		using Name_t	= StaticString<64>;

		struct BaseUniform : CompileTime::CopyQualifiers< Name_t >
		{
			Name_t				name;
			EShader::bits		stageFlags;
			uint				binding		= UMax;	// resource dependend index, may be optimized to minimize resource switches between pipelines, used in OpenGL, DirectX
			uint				uniqueIndex	= UMax;	// resource unique index in current pipeline, used in Vulkan, OpenCL, software renderer
		};


		struct TextureUniform final : BaseUniform
		{
		// variables
			EImage::type			textureType		= EImage::Unknown;
			EPixelFormatClass::type	format			= EPixelFormatClass::Any;

		// methods
			bool operator == (const TextureUniform &right) const;
			bool operator >  (const TextureUniform &right) const;
		};


		struct SamplerUniform final : BaseUniform
		{
		// variables
			// TODO

		// methods
			bool operator == (const SamplerUniform &right) const;
			bool operator >  (const SamplerUniform &right) const;
		};


		struct SubpassInput final : BaseUniform
		{
		// variables
			uint			attachmentIndex		= UMax;
			bool			isMultisample		= false;

		// methods
			bool operator == (const SubpassInput &right) const;
			bool operator >  (const SubpassInput &right) const;
		};


		struct ImageUniform final : BaseUniform
		{
		// variables
			EImage::type				imageType		= EImage::Unknown;
			EPixelFormat::type			format			= EPixelFormat::Unknown;
			EShaderMemoryModel::type	access;

		// methods
			bool operator == (const ImageUniform &right) const;
			bool operator >  (const ImageUniform &right) const;
		};


		struct UniformBuffer : BaseUniform
		{
		// variables
			// TODO: buffer typename

			BytesU					size;

		// methods
			bool operator == (const UniformBuffer &right) const;
			bool operator >  (const UniformBuffer &right) const;
		};


		struct StorageBuffer final : BaseUniform
		{
		// variables
			// TODO: static buffer typename, dynamic buffer typename

			BytesU						staticSize;
			BytesU						arrayStride;
			EShaderMemoryModel::type	access;

		// methods
			bool operator == (const StorageBuffer &right) const;
			bool operator >  (const StorageBuffer &right) const;
		};


		struct PushConstant final : CompileTime::CopyQualifiers< Name_t >
		{
		// variables
			Name_t					name;
			EShader::bits			stageFlags;
			BytesU					offset;
			BytesU					size;
			// TODO: type info

		// methods
			bool operator == (const PushConstant &right) const;
			bool operator >  (const PushConstant &right) const;
		};


		// push constants emulation for OpenGL, OpenCL, DirectX 11
		struct PushConstantsBuffer final : UniformBuffer
		{};


		using Uniform_t		= Union< TextureUniform, SamplerUniform, ImageUniform, PushConstantsBuffer,
									 UniformBuffer, StorageBuffer, PushConstant, SubpassInput >;
		using Uniforms_t	= Array< Uniform_t >;


	// variables
	private:
		Uniforms_t		_uniforms;
		HashResult		_hash;


	// methods
	public:
		PipelineLayoutDescription (GX_DEFCTOR) {}

		ND_ HashResult				GetHash ()			const	{ return _hash; }
		ND_ ArrayCRef<Uniform_t>	GetUniforms ()		const	{ return _uniforms; }

		ND_ bool operator == (const Self &right) const;
		ND_ bool operator >  (const Self &right) const;
	};
	


	//
	// Pipeline Layout Description Builder
	//

	struct PipelineLayoutDescription::Builder
	{
	// variables
	private:
		PipelineLayoutDescription	_descr;
		bool						_changed	= true;


	// methods
	public:
		Builder () {}

		explicit Builder (const PipelineLayoutDescription &descr) : _descr(descr) {}

		Builder& AddTexture (StringCRef name, EImage::type dimension, EPixelFormatClass::type format,
							 uint binding, uint uniqueIndex, EShader::bits stageFlags);

		Builder& AddImage (StringCRef name, EImage::type dimension, EPixelFormat::type format, EShaderMemoryModel::type access,
						   uint binding, uint uniqueIndex, EShader::bits stageFlags);

		Builder& AddSubpass (StringCRef name, uint attachmentIndex, bool isMultisample, uint binding, uint uniqueIndex, EShader::bits stageFlags);

		Builder& AddUniformBuffer (StringCRef name, BytesU size, uint binding, uint uniqueIndex, EShader::bits stageFlags);
		
		Builder& AddStorageBuffer (StringCRef name, BytesU size, BytesU stride, EShaderMemoryModel::type access,
									uint binding, uint uniqueIndex, EShader::bits stageFlags);

		Builder& AddPushConstant (StringCRef name, BytesU offset, BytesU size, EShader::bits stageFlags);
		Builder& AddPushConstantsBuffer (StringCRef name, BytesU size, uint binding, uint uniqueIndex, EShader::bits stageFlags);
		
		ND_ ArrayCRef<Uniform_t>	GetUniforms ()	const	{ return _descr.GetUniforms(); }

		// validate, calculate hash and return
		ND_ PipelineLayoutDescription const& Finish ();
	};

}	// Platforms
}	// Engine


namespace GX_STL
{
namespace GXTypes
{
	//
	// Hash (PipelineLayoutDescription)
	//
	template <>
	struct Hash< Engine::Platforms::PipelineLayoutDescription > final
	{
		ND_ HashResult  operator () (const Engine::Platforms::PipelineLayoutDescription &key) const noexcept
		{
			HashResult	res;

			res += HashOf( key.GetUniforms().Count() );

			FOR( i, key.GetUniforms() ) {
				res += HashOf( key.GetUniforms()[i] );
			}
			return res;
		}
	};
	
}	// GXTypes
}	// GX_STL
