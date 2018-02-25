// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	This class contains resources for shaders and
	prepare resources for shader invocations.
*/

#pragma once

#include "Engine/Platforms/Soft/ShaderLang/SWLangBuffer.h"
#include "Engine/Platforms/Soft/ShaderLang/SWLangImage.h"
#include "Engine/Platforms/Soft/ShaderLang/SWLangTexture.h"

#ifdef GRAPHICS_API_SOFT

namespace SWShaderLang
{
namespace Impl
{

	//
	// Soft Renderer Shader Language Helper
	//

	class SWShaderHelper final
	{
	// types
	private:
		using StringCRef		= GX_STL::GXTypes::StringCRef;
		using EShader			= Engine::Platforms::EShader;

		using ModulePtr			= Engine::Base::ModulePtr;
		using VertexBuffers_t	= GX_STL::GXTypes::FixedSizeArray< ModulePtr, Engine::GlobalConst::Graphics_MaxAttribs >;
		
		using Fwd_GetSWBufferMemoryLayout		= Message< GpuMsg::ResourceTableForwardMsg< Message< GpuMsg::GetSWBufferMemoryLayout > > >;
		using Fwd_GetSWImageViewMemoryLayout	= Message< GpuMsg::ResourceTableForwardMsg< Message< GpuMsg::GetSWImageViewMemoryLayout > > >;
		using Fwd_GetSWTextureMemoryLayout		= Message< GpuMsg::ResourceTableForwardMsg< Message< GpuMsg::GetSWTextureMemoryLayout > > >;

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


	// variables
	private:
		VertexShader		_vertexShaderState;
		GeometryShader		_geometryShaderState;
		FragmentShader		_fragmentShaderState;
		ComputeShader		_computeShaderState;

		ModulePtr			_resourceTable;


	// methods
	private:
		SWShaderHelper (SWShaderHelper &&) = default;
		SWShaderHelper (const SWShaderHelper &) = default;

		EShader::type	_CurrentStage () const;


	public:
		SWShaderHelper ();

		// control
		void Reset ();

		// vertex shader only
		bool VS_GetVertexBufferPtr (StringCRef bufferName, OUT void *&ptr) const;
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
		bool FS_DepthOutput (float value) const;

		
		VertexShader const&		GetVertexShaderState () const		{ return _vertexShaderState; }
		GeometryShader const&	GetGeometryShaderState () const		{ return _geometryShaderState; }
		FragmentShader const&	GetFragmentShaderState () const		{ return _fragmentShaderState; }
		ComputeShader const&	GetComputeShaderState () const		{ return _computeShaderState; }


		// for all shaders
		//template <typename T>
		//bool GetUniform (uint uniqueIndex, OUT T &value) const;

		template <typename T>
		bool GetUniformBuffer (uint uniqueIndex, OUT UniformBuffer<T> &value) const;

		template <typename T, EStorageAccess::type A>
		bool GetStorageBuffer (uint uniqueIndex, OUT StorageBuffer<T,A> &value) const;
		
		template <typename T, EStorageAccess::type A>
		bool GetImage (uint uniqueIndex, OUT Image2D<T,A> &value) const;
		
		template <typename T, EStorageAccess::type A>
		bool GetImage (uint uniqueIndex, OUT Image2DArray<T,A> &value) const;

		template <typename T, EStorageAccess::type A>
		bool GetImage (uint uniqueIndex, OUT Image3D<T,A> &value) const;

		template <typename T>
		bool GetTexture (uint uniqueIndex, OUT Texture2D<T> &value) const;
		
		//template <typename T, typename R>
		//bool GetTexture (uint uniqueIndex, OUT Texture2DArray<T,R> &value) const;
		
		//template <typename T, typename R>
		//bool GetTexture (uint uniqueIndex, OUT Texture3D<T,R> &value) const;
	};

	
/*
=================================================
	GetUniformBuffer
=================================================
*/
	template <typename T>
	inline bool SWShaderHelper::GetUniformBuffer (uint uniqueIndex, OUT UniformBuffer<T> &value) const
	{
		Fwd_GetSWBufferMemoryLayout		req_buf{ {}, uniqueIndex };

		value = UniformBuffer<T>{ *req_buf->message->result };
		return true;
	}
	
/*
=================================================
	GetStorageBuffer
=================================================
*/
	template <typename T, EStorageAccess::type A>
	inline bool SWShaderHelper::GetStorageBuffer (uint uniqueIndex, OUT StorageBuffer<T,A> &value) const
	{
		Fwd_GetSWBufferMemoryLayout		req_buf{ Message< GpuMsg::GetSWBufferMemoryLayout >{}, uniqueIndex };

		value = RVREF(StorageBuffer<T,A>{ *req_buf->message->result });
		return true;
	}
		
/*
=================================================
	GetImage
=================================================
*/
	template <typename T, EStorageAccess::type A>
	inline bool SWShaderHelper::GetImage (uint uniqueIndex, OUT Image2D<T,A> &value) const
	{
		Fwd_GetSWImageViewMemoryLayout	req_img{ Message< GpuMsg::GetSWImageViewMemoryLayout >{}, uniqueIndex };

		value = RVREF(Image2D<T,A>{ RVREF(*req_img->message->result) });
		return true;
	}
		
	template <typename T, EStorageAccess::type A>
	inline bool SWShaderHelper::GetImage (uint uniqueIndex, OUT Image2DArray<T,A> &value) const
	{
		Fwd_GetSWImageViewMemoryLayout	req_img{ Message< GpuMsg::GetSWImageViewMemoryLayout >{}, uniqueIndex };

		value = RVREF(Image2DArray<T,A>{ RVREF(*req_img->message->result) });
		return true;
	}

	template <typename T, EStorageAccess::type A>
	inline bool SWShaderHelper::GetImage (uint uniqueIndex, OUT Image3D<T,A> &value) const
	{
		Fwd_GetSWImageViewMemoryLayout	req_img{ Message< GpuMsg::GetSWImageViewMemoryLayout >{}, uniqueIndex };

		value = RVREF(Image3D<T,A>{ RVREF(*req_img->message->result) });
		return true;
	}
	
/*
=================================================
	GetTexture
=================================================
*/
	template <typename T>
	inline bool SWShaderHelper::GetTexture (uint uniqueIndex, OUT Texture2D<T> &value) const
	{
		TODO( "GetTexture" );
		return true;
	}


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
