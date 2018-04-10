// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	This class contains resources for shaders and
	prepare resources for shader invocations.
*/

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Soft/ShaderLang/SWLangBuffer.h"
#include "Engine/Platforms/Soft/ShaderLang/SWLangImage.h"
#include "Engine/Platforms/Soft/ShaderLang/SWLangTexture.h"
#include "Engine/Platforms/Soft/ShaderLang/SWLangShared.h"
#include "Engine/Platforms/Soft/ShaderLang/SWLangBarrier.h"

namespace SWShaderLang
{
namespace Impl
{

	//
	// Soft Renderer Shader Language Helper
	//

	class SWShaderHelper
	{
	// types
	protected:
		using StringCRef		= GX_STL::GXTypes::StringCRef;
		using EShader			= Engine::Platforms::EShader;
		using EPipelineAccess	= Engine::Platforms::EPipelineAccess;
		using EPipelineStage	= Engine::Platforms::EPipelineStage;

		using ModulePtr			= Engine::Base::ModulePtr;
		using VertexBuffers_t	= GX_STL::GXTypes::FixedSizeArray< ModulePtr, Engine::GlobalConst::GAPI_MaxAttribs >;

		using Atomic_t			= Barrier::Atomic_t;

		template <typename T> using Message = Engine::Base::Message<T>;

		using Fwd_GetSWBufferMemoryLayout		= Message< Engine::GpuMsg::ResourceTableForwardMsg< Message< Engine::GpuMsg::GetSWBufferMemoryLayout > > >;
		using Fwd_GetSWImageViewMemoryLayout	= Message< Engine::GpuMsg::ResourceTableForwardMsg< Message< Engine::GpuMsg::GetSWImageViewMemoryLayout > > >;
		using Fwd_GetSWTextureMemoryLayout		= Message< Engine::GpuMsg::ResourceTableForwardMsg< Message< Engine::GpuMsg::GetSWTextureMemoryLayout > > >;

		struct VertexShader
		{
			VertexBuffers_t		vertexBuffers;
			ModulePtr			indexBuffer;
			
			mutable float		outClipDistance[1]	= {};		// gl_out.gl_ClipDistance
			mutable float		outCullDistance[1]	= {};		// gl_out.gl_CullDistance
			int					inInstanceID		= 0;		// gl_InstanceID
			mutable float		outPointSize		= 1.0f;		// gl_out.gl_PointSize
			mutable glm::vec4	outPosition;					// gl_out.gl_Position
			int					inVertexID			= 0;		// gl_VertexID
		};

		struct GeometryShader
		{
			float				inClipDistance[1]	= {};		// gl_in[].gl_ClipDistance
			mutable float		outClipDistance[1]	= {};		// gl_out.gl_ClipDistance
			float				inCullDistance[1]	= {};		// gl_in[].gl_CullDistance
			mutable float		outCullDistance[1]	= {};		// gl_out.gl_CullDistance
			int					inInvocationID		= 0;		// gl_InvocationID
			mutable int			outLayer			= 0;		// gl_Layer
			float				inPointSize			= 1.0f;		// gl_in[].gl_PointSize
			mutable float		outPointSize		= 1.0f;		// gl_out.gl_PointSize
			glm::vec4			inPosition;						// gl_in[].gl_Position
			mutable glm::vec4	outPosition;					// gl_out.gl_Position
			mutable int			outPrimitiveID		= 0;		// gl_PrimitiveID
			int					inPrimitiveID		= 0;		// gl_PrimitiveIDIn
			mutable int			outViewportIndex	= 0;		// gl_ViewportIndex
		};

		struct FragmentShader
		{
			float				inCullDistance[1]	= {};		// gl_CullDistance
			glm::vec4			inFragCoord;					// gl_FragCoord
			mutable float		outFragDepth		= 0.0f;		// gl_FragDepth
			bool				inFrontFacing		= true;		// gl_FrontFacing
			bool				inHelperInvocation	= false;	// gl_HelperInvocation
			int					inLayer				= 0;		// gl_Layer
			int					inNumSamples		= 1;		// gl_NumSamples
			glm::vec2			inPointCoord;					// gl_PointCoord
			int					inPrimitiveID		= 0;		// gl_PrimitiveID
			int					inSampleID			= 0;		// gl_SampleID
			mutable int			outSampleMask[8]	= {};		// gl_SampleMask
			int					inSampleMask[8]		= {};		// gl_SampleMaskIn
			glm::ivec2			inSamplePosition;				// gl_SamplePosition
			int					inViewportIndex		= 0;		// gl_ViewportIndex
		};

		struct ComputeShader
		{
			glm::uvec3			inGlobalInvocationID;			// gl_GlobalInvocationID
			glm::uvec3			inLocalInvocationID;			// gl_LocalInvocationID
			glm::uint			inLocalInvocationIndex	= 0;	// gl_LocalInvocationIndex
			glm::uvec3			inNumWorkGroups;				// gl_NumWorkGroups
			glm::uvec3			inWorkGroupID;					// gl_WorkGroupID
			glm::uvec3			constWorkGroupSize;				// gl_WorkGroupSize
		};

		using ShaderState_t	= Union< VertexShader, GeometryShader,
									 FragmentShader, ComputeShader >;

		
	public:
		class IShaderModel
		{
		// types
		public:
			using Atomic_t							= SWShaderHelper::Atomic_t;
			using Fwd_GetSWBufferMemoryLayout		= SWShaderHelper::Fwd_GetSWBufferMemoryLayout;
			using Fwd_GetSWImageViewMemoryLayout	= SWShaderHelper::Fwd_GetSWImageViewMemoryLayout;
			using Fwd_GetSWTextureMemoryLayout		= SWShaderHelper::Fwd_GetSWTextureMemoryLayout;

		// interface
		public:
			virtual void InitBarrier (uint invocationID, uint index, OUT Atomic_t *&) const = 0;
			virtual void GetSharedMemory (uint invocationID, uint index, BytesU size, OUT BinaryArray *&) const = 0;
			virtual void GetBufferMemoryLayout (Fwd_GetSWBufferMemoryLayout &) const = 0;
			virtual void GetImageViewMemoryLayout (Fwd_GetSWImageViewMemoryLayout &) const = 0;
			virtual void GetTextureMemoryLayout (Fwd_GetSWTextureMemoryLayout &) const = 0;
		};


	// variables
	protected:
		Ptr<IShaderModel>	_shader;
		uint				_invocationID	= 0;

		ShaderState_t		_shaderState;


	// methods
	public:
		explicit SWShaderHelper (Ptr<IShaderModel> shader) : _shader{shader} {}

		SWShaderHelper (SWShaderHelper &&) = default;
		SWShaderHelper (const SWShaderHelper &) = default;

	// shader interface
	public:
		// vertex shader only
		/*bool VS_GetVertexBufferPtr (StringCRef bufferName, OUT void *&ptr) const;
		bool VS_GetVertexBufferPtr (uint bindingIndex, OUT void *&ptr) const;
		bool VS_GetIndexBufferPtr (OUT void *&ptr) const;


		// fragment shader only
		bool FS_FragmentOutput (uint index, const float4 &value) const;
		bool FS_FragmentOutput (uint index, const uint4 &value) const;
		bool FS_FragmentOutput (uint index, const int4 &value) const;

		bool FS_FragmentOutput (StringCRef name, const float4 &value) const;
		bool FS_FragmentOutput (StringCRef name, const uint4 &value) const;
		bool FS_FragmentOutput (StringCRef name, const int4 &value) const;

		bool FS_StencilOutput (int value) const;
		bool FS_DepthOutput (float value) const;*/

		
		VertexShader const&		GetVertexShaderState () const		{ return _shaderState.Get< VertexShader >(); }
		GeometryShader const&	GetGeometryShaderState () const		{ return _shaderState.Get< GeometryShader >(); }
		FragmentShader const&	GetFragmentShaderState () const		{ return _shaderState.Get< FragmentShader >(); }
		ComputeShader const&	GetComputeShaderState () const		{ return _shaderState.Get< ComputeShader >(); }


		// for all shaders
		template <typename T>
		void GetShared (uint index, usize arraySize, OUT SharedMemory<T> &value) const;

		void InitBarrier (uint index, OUT Barrier &value) const;

		template <typename T>
		void GetUniformBuffer (uint uniqueIndex, OUT UniformBuffer<T> &value) const;

		template <typename T, EStorageAccess::type A>
		void GetStorageBuffer (uint uniqueIndex, OUT StorageBuffer<T,A> &value) const;
		
		template <typename T, EStorageAccess::type A>
		void GetImage (uint uniqueIndex, OUT Image2D<T,A> &value) const;
		
		template <typename T, EStorageAccess::type A>
		void GetImage (uint uniqueIndex, OUT Image2DArray<T,A> &value) const;

		template <typename T, EStorageAccess::type A>
		void GetImage (uint uniqueIndex, OUT Image3D<T,A> &value) const;

		template <typename T>
		void GetTexture (uint uniqueIndex, OUT Texture2D<T> &value) const;
		
		//template <typename T, typename R>
		//void GetTexture (uint uniqueIndex, OUT Texture2DArray<T,R> &value) const;
		
		//template <typename T, typename R>
		//void GetTexture (uint uniqueIndex, OUT Texture3D<T,R> &value) const;

	private:
		EShader::type			_GetShader () const;
		EPipelineStage::type	_GetStage () const;
	};


/*
=================================================
	GetShared
=================================================
*/
	template <typename T>
	inline void SWShaderHelper::GetShared (uint index, usize arraySize, OUT SharedMemory<T> &value) const
	{
		BinaryArray*	arr = null;
		_shader->GetSharedMemory( _invocationID, index, SizeOf<T> * arraySize, OUT arr );
		value = SharedMemory<T>( *arr, arraySize );
	}
	
/*
=================================================
	InitBarrier
=================================================
*/
	inline void SWShaderHelper::InitBarrier (uint index, OUT Barrier &value) const
	{
		Atomic_t*	atomic = null;
		_shader->InitBarrier( _invocationID, index, OUT atomic );
		value = Barrier( atomic );
	}

/*
=================================================
	GetUniformBuffer
=================================================
*/
	template <typename T>
	inline void SWShaderHelper::GetUniformBuffer (uint uniqueIndex, OUT UniformBuffer<T> &value) const
	{
		Fwd_GetSWBufferMemoryLayout		req_buf{ uniqueIndex, EPipelineAccess::UniformRead, _GetStage() };
		_shader->GetBufferMemoryLayout( req_buf );

		value = UniformBuffer<T>{ *req_buf->message->result };
	}
	
/*
=================================================
	GetStorageBuffer
=================================================
*/
	template <typename T, EStorageAccess::type A>
	inline void SWShaderHelper::GetStorageBuffer (uint uniqueIndex, OUT StorageBuffer<T,A> &value) const
	{
		Fwd_GetSWBufferMemoryLayout		req_buf{ uniqueIndex,
												 EPipelineAccess::bits() |
												 (EStorageAccess::HasReadAccess( A ) ? EPipelineAccess::ShaderRead : EPipelineAccess::type(0)) |
												 (EStorageAccess::HasWriteAccess( A ) ? EPipelineAccess::ShaderWrite : EPipelineAccess::type(0)),
												 _GetStage() };
		_shader->GetBufferMemoryLayout( req_buf );

		value = RVREF(StorageBuffer<T,A>{ *req_buf->message->result });
	}
		
/*
=================================================
	GetImage
=================================================
*/
	template <typename T, EStorageAccess::type A>
	inline void SWShaderHelper::GetImage (uint uniqueIndex, OUT Image2D<T,A> &value) const
	{
		Fwd_GetSWImageViewMemoryLayout	req_img{ uniqueIndex,
												 EPipelineAccess::bits() |
												 (EStorageAccess::HasReadAccess( A ) ? EPipelineAccess::ShaderRead : EPipelineAccess::type(0)) |
												 (EStorageAccess::HasWriteAccess( A ) ? EPipelineAccess::ShaderWrite : EPipelineAccess::type(0)),
												 _GetStage() };
		_shader->GetImageViewMemoryLayout( req_img );

		value = RVREF(Image2D<T,A>{ RVREF(*req_img->message->result) });
	}
		
	template <typename T, EStorageAccess::type A>
	inline void SWShaderHelper::GetImage (uint uniqueIndex, OUT Image2DArray<T,A> &value) const
	{
		Fwd_GetSWImageViewMemoryLayout	req_img{ uniqueIndex,
												 EPipelineAccess::bits() |
												 (EStorageAccess::HasReadAccess( A ) ? EPipelineAccess::ShaderRead : EPipelineAccess::type(0)) |
												 (EStorageAccess::HasWriteAccess( A ) ? EPipelineAccess::ShaderWrite : EPipelineAccess::type(0)),
												 _GetStage() };
		_shader->GetImageViewMemoryLayout( req_img );

		value = RVREF(Image2DArray<T,A>{ RVREF(*req_img->message->result) });
	}

	template <typename T, EStorageAccess::type A>
	inline void SWShaderHelper::GetImage (uint uniqueIndex, OUT Image3D<T,A> &value) const
	{
		Fwd_GetSWImageViewMemoryLayout	req_img{ uniqueIndex,
												 EPipelineAccess::bits() |
												 (EStorageAccess::HasReadAccess( A ) ? EPipelineAccess::ShaderRead : EPipelineAccess::type(0)) |
												 (EStorageAccess::HasWriteAccess( A ) ? EPipelineAccess::ShaderWrite : EPipelineAccess::type(0)),
												 _GetStage() };
		_shader->GetImageViewMemoryLayout( req_img );

		value = RVREF(Image3D<T,A>{ RVREF(*req_img->message->result) });
	}
	
/*
=================================================
	GetTexture
=================================================
*/
	template <typename T>
	inline void SWShaderHelper::GetTexture (uint uniqueIndex, OUT Texture2D<T> &value) const
	{
		TODO( "GetTexture" );
	}
	
/*
=================================================
	_GetShader
=================================================
*/
	inline SWShaderHelper::EShader::type  SWShaderHelper::_GetShader () const
	{
		if ( _shaderState.GetCurrentTypeId() == TypeIdOf<VertexShader>() )		return EShader::Vertex;
		if ( _shaderState.GetCurrentTypeId() == TypeIdOf<GeometryShader>() )	return EShader::Geometry;
		if ( _shaderState.GetCurrentTypeId() == TypeIdOf<FragmentShader>() )	return EShader::Fragment;
		if ( _shaderState.GetCurrentTypeId() == TypeIdOf<ComputeShader>() )		return EShader::Compute;

		return EShader::Unknown;
	}
	
/*
=================================================
	_GetStage
=================================================
*/
	inline SWShaderHelper::EPipelineStage::type  SWShaderHelper::_GetStage () const
	{
		if ( _shaderState.GetCurrentTypeId() == TypeIdOf<VertexShader>() )		return EPipelineStage::VertexShader;
		if ( _shaderState.GetCurrentTypeId() == TypeIdOf<GeometryShader>() )	return EPipelineStage::GeometryShader;
		if ( _shaderState.GetCurrentTypeId() == TypeIdOf<FragmentShader>() )	return EPipelineStage::FragmentShader;
		if ( _shaderState.GetCurrentTypeId() == TypeIdOf<ComputeShader>() )		return EPipelineStage::ComputeShader;

		return EPipelineStage::Unknown;
	}

}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
