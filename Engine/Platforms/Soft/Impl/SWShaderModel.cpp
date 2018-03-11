// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Soft/Impl/SWShaderModel.h"
#include "Engine/Platforms/Soft/Impl/SWMessages.h"
#include "Engine/Platforms/Soft/Impl/SWDeviceProperties.h"

namespace Engine
{
namespace PlatformSW
{

	//
	// Compute Shader Thread Pool
	//
	class SWShaderModel::ComputeThreadPool final
	{
	// types
	private:
		class ShaderHelper final : public SWShaderLang::Impl::SWShaderHelper
		{
		public:
		// variables
			ShaderFunc_t		_shaderFunc;
			OS::Thread			_thread;

			const uint3			_localID;
			const uint3			_localSize;
			const uint3			_groupOffset;	// id = group_id + offset
			const uint3			_groupSize;

		// methods
			ShaderHelper (ShaderHelper &&) = default;

			ShaderHelper (Ptr<IShaderModel> shader, ShaderFunc_t func, const uint3 &localID, const uint3 &localSize, const uint3 &groupOffset, const uint3 &groupSize) :
				SWShaderHelper{shader}, _shaderFunc{func}, _localID{localID}, _localSize{localSize}, _groupOffset{groupOffset}, _groupSize{groupSize} {}

			ComputeShader&	Init ()			{ return _shaderState.Create( ComputeShader{} ).Get< ComputeShader >(); }
			uint&			Invocation ()	{ return _invocationID; }
		};


	// methods
	public:
		ComputeThreadPool () {}

		void Invoke (Ptr<IShaderModel> shader, ShaderFunc_t func, const uint3 &localSize,
					 const uint3 &groupOffset, const uint3 &groupSize);
	};


/*
=================================================
	InvokeCompute
=================================================
*/
	void SWShaderModel::ComputeThreadPool::Invoke (Ptr<IShaderModel> shader, ShaderFunc_t func, const uint3 &localSize,
													const uint3 &groupOffset, const uint3 &groupSize)
	{
		Array< ShaderHelper >	shaders;

		shaders.Reserve( localSize.Volume() );
		
		// initialize
		for (uint3 local_id; local_id.z < localSize.z; ++local_id.z)
		for (local_id.y = 0; local_id.y < localSize.y; ++local_id.y)
		for (local_id.x = 0; local_id.x < localSize.x; ++local_id.x)
		{
			shaders.PushBack(ShaderHelper{ shader, func, local_id, localSize, groupOffset, groupSize });
		}

		// run
		for (auto& shaderThread : Range(shaders))
		{
			shaderThread._thread.Create( LAMBDA() (void *param)
				{
					ShaderHelper*	self	= (ShaderHelper *)param;
					auto&			state	= self->Init();
					const uint3		size	= self->_groupSize + self->_groupOffset;

					state.inNumWorkGroups			= glm::uvec3( size.x, size.y, size.z );
					state.constWorkGroupSize		= glm::uvec3( self->_localSize.x, self->_localSize.y, self->_localSize.z );
					state.inLocalInvocationID		= glm::uvec3( self->_localID.x, self->_localID.y, self->_localID.z );
					state.inLocalInvocationIndex	= self->_localID.x +
													  (self->_localID.y * self->_localSize.x) +
													  (self->_localID.z * self->_localSize.x * self->_localSize.y);
				
					for (uint3 group_id; group_id.z < self->_groupSize.z; ++group_id.z)
					for (group_id.y = 0; group_id.y < self->_groupSize.y; ++group_id.y)
					for (group_id.x = 0; group_id.x < self->_groupSize.x; ++group_id.x)
					{
						const uint3	id = group_id + self->_groupOffset;

						state.inWorkGroupID			= glm::uvec3( id.x, id.y, id.z );
						state.inGlobalInvocationID	= glm::uvec3( id.x * self->_localSize.x + self->_localID.x,
																  id.y * self->_localSize.y + self->_localID.y,
																  id.z * self->_localSize.z + self->_localID.z );

						self->Invocation() =	group_id.x +
												(group_id.y * self->_groupSize.x) +
												(group_id.z * self->_groupSize.x * self->_groupSize.y);

						self->_shaderFunc( *self );
					}
				}, &shaderThread );
		}
		
		// wait until the end
		for (auto& shaderThread : Range(shaders)) {
			shaderThread._thread.Wait();
		}
	}
//-----------------------------------------------------------------------------



/*
=================================================
	constructor
=================================================
*/
	SWShaderModel::SWShaderModel () : _localSize{0}
	{}
	
/*
=================================================
	_Reset
=================================================
*/
	void SWShaderModel::_Reset ()
	{
		_resourceTable	= null;
		_localSize		= 0;

		_barriers.Clear();
		_sharedMemory.Clear();
	}

/*
=================================================
	DispatchCompute
=================================================
*/
	bool SWShaderModel::DispatchCompute (const uint3 &workGroups, const ModulePtr &pipeline, const ModulePtr &resourceTable)
	{
		return DispatchComputeOffset( uint3(), workGroups, pipeline, resourceTable );
	}
	
/*
=================================================
	DispatchComputeOffset
=================================================
*/
	bool SWShaderModel::DispatchComputeOffset (const uint3 &groupOffset, const uint3 &workGroups, const ModulePtr &pipeline, const ModulePtr &resourceTable)
	{
		Message< GpuMsg::GetSWPipelineStage >			req_shader{ EShader::Compute };
		Message< GpuMsg::GetComputePipelineDescriptor >	req_descr;

		CHECK( pipeline->Send( req_shader ) );
		CHECK( pipeline->Send( req_descr ) );

		CHECK_ERR( req_shader->result and req_shader->result->func );
		
		ShaderFunc_t	func	= req_shader->result->func;
		const uint3		groups	= Max( 1u, workGroups );
		const uint3		local	= Max( 1u, req_descr->result->localGroupSize );

		CHECK_ERR(All( groups + groupOffset <= SWDeviceProperties.limits.maxComputeWorkGroupCount ));
		CHECK_ERR(All( local <= SWDeviceProperties.limits.maxComputeWorkGroupSize ));
		CHECK_ERR( local.Volume() < SWDeviceProperties.limits.maxComputeWorkGroupInvocations );

		_resourceTable	= resourceTable;
		_localSize		= local.Volume();

		ComputeThreadPool	thread_pool;
		thread_pool.Invoke( this, func, local, groupOffset, groups );
		
		_Reset();
		return true;
	}

/*
=================================================
	InitBarrier
=================================================
*/
	void SWShaderModel::InitBarrier (const uint invocationID, const uint index, OUT Atomic_t *&atomic) const
	{
		SCOPELOCK( _lock );

		const Key_t				key = {invocationID, index};
		BarrierMap_t::iterator	iter;

		if ( _barriers.Find( key, OUT iter ) )
		{
			atomic = iter->second.ptr();
		}
		else
		{
			atomic = _barriers.Add({ key, Barrier_t{new Atomic_t{_localSize}} })->second.ptr();
		}
	}

/*
=================================================
	GetSharedMemory
=================================================
*/
	void SWShaderModel::GetSharedMemory (const uint invocationID, const uint index, const BytesU size, OUT BinaryArray *&arr) const
	{
		SCOPELOCK( _lock );
		
		const Key_t				key = {invocationID, index};
		SharedMemMap_t::iterator	iter;

		if ( _sharedMemory.Find( key, OUT iter ) )
		{
			arr = iter->second.ptr();
		}
		else
		{
			arr = _sharedMemory.Add({ key, SharedMemory_t{new BinaryArray()} })->second.ptr();
			arr->Resize( usize(size), false );
		}
	}

/*
=================================================
	GetBufferMemoryLayout
=================================================
*/
	void SWShaderModel::GetBufferMemoryLayout (Fwd_GetSWBufferMemoryLayout &msg) const
	{
		SCOPELOCK( _lock );

		msg->message.Async();

		CHECK( _resourceTable->Send( msg.Async() ) );
	}
	
/*
=================================================
	GetImageViewMemoryLayout
=================================================
*/
	void SWShaderModel::GetImageViewMemoryLayout (Fwd_GetSWImageViewMemoryLayout &msg) const
	{
		SCOPELOCK( _lock );
		
		msg->message.Async();

		CHECK( _resourceTable->Send( msg.Async() ) );
	}
	
/*
=================================================
	GetTextureMemoryLayout
=================================================
*/
	void SWShaderModel::GetTextureMemoryLayout (Fwd_GetSWTextureMemoryLayout &msg) const
	{
		SCOPELOCK( _lock );
		
		msg->message.Async();

		CHECK( _resourceTable->Send( msg.Async() ) );
	}

}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT
