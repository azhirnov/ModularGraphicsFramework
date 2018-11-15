// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Soft/ShaderLang/SWShaderHelper.h"

namespace Engine
{
namespace PlatformSW
{

	//
	// Software Renderer Shader Model
	//

	class SWShaderModel final : protected SWShaderLang::Impl::SWShaderHelper::IShaderModel
	{
	// types
	private:
		using ShaderFunc_t		= PipelineTemplateDescription::ShaderSource::SWInvoke_t;

		using Key_t				= Pair<uint, uint>;		// invocation, index

		using SharedMemory_t	= UniquePtr< BinaryArray >;
		using SharedMemMap_t	= HashMap< Key_t, SharedMemory_t >;

		using Barrier_t			= UniquePtr< Atomic_t >;
		using BarrierMap_t		= HashMap< Key_t, Barrier_t >;

		class ComputeThreadPool;


	// variables
	private:
		ModulePtr				_resourceTable;
		int						_localSize;

		mutable SharedMemMap_t	_sharedMemory;
		mutable BarrierMap_t	_barriers;

		mutable Mutex			_lock;


	// methods
	public:
		SWShaderModel ();

		bool DispatchCompute (const uint3 &workGroups, const ModulePtr &pipeline, const ModulePtr &resourceTable);
		bool DispatchComputeOffset (const uint3 &groupOffset, const uint3 &workGroups, const ModulePtr &pipeline, const ModulePtr &resourceTable);


	private:
		// IShaderModel //
		void InitBarrier (uint invocationID, uint index, OUT Atomic_t *&) const override;
		void GetSharedMemory (uint invocationID, uint index, BytesU size, OUT BinaryArray *&) const override;
		void GetBufferMemoryLayout (Fwd_GetSWBufferMemoryLayout &) const override;
		void GetImageViewMemoryLayout (Fwd_GetSWImageViewMemoryLayout &) const override;
		void GetTextureMemoryLayout (Fwd_GetSWTextureMemoryLayout &) const override;

	private:
		void _Reset ();

		static void _Invoke (void *param);
	};


}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT
