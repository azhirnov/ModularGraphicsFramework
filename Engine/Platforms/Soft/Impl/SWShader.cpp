// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Soft/Impl/SWShader.h"
#include "Engine/Platforms/Soft/Impl/SWMessages.h"
#include "Engine/Platforms/Soft/Impl/SWDeviceLimits.h"

#ifdef GRAPHICS_API_SOFT

namespace Engine
{
namespace PlatformSW
{

/*
=================================================
	constructor
=================================================
*/
	SWShader::SWShader ()
	{}
	
/*
=================================================
	DispatchCompute
=================================================
*/
	bool SWShader::DispatchCompute (const uint3 &workGroups, const ModulePtr &pipeline, const ModulePtr &resourceTable)
	{
		Message< GpuMsg::GetSWPipelineStage >			req_shader{ EShader::Compute };
		Message< GpuMsg::GetComputePipelineDescriptor >	req_descr;

		CHECK( pipeline->Send( req_shader ) );
		CHECK( pipeline->Send( req_descr ) );

		CHECK_ERR( req_shader->result and req_shader->result->func );

		_resourceTable = resourceTable;

		ShaderFunc_t	shader_func = req_shader->result->func;
		const uint3		groups		= Max( uint3(1), workGroups );
		const uint3		local		= Max( uint3(1), req_descr->result->localGroupSize );

		CHECK_ERR(All( groups <= SWDeviceLimits::maxComputeWorkGroupCount ));
		CHECK_ERR(All( local <= SWDeviceLimits::maxComputeWorkGroupSize ));
		CHECK_ERR( local.Volume() < SWDeviceLimits::maxComputeWorkGroupInvocations );

		_stage = EShader::Compute;
		_computeShaderState.inNumWorkGroups		= glm::uvec3( groups.x, groups.y, groups.z );
		_computeShaderState.constWorkGroupSize	= glm::uvec3( local.x, local.y, local.z );

		for (uint3 group_id; group_id.z < groups.z; ++group_id.z)
		for (group_id.y = 0; group_id.y < groups.y; ++group_id.y)
		for (group_id.x = 0; group_id.x < groups.x; ++group_id.x)
		{
			_computeShaderState.inWorkGroupID = glm::uvec3( group_id.x, group_id.y, group_id.z );

			for (uint3 local_id; local_id.z < local.z; ++local_id.z)
			for (local_id.y = 0; local_id.y < local.y; ++local_id.y)
			for (local_id.x = 0; local_id.x < local.x; ++local_id.x)
			{
				_computeShaderState.inLocalInvocationID		= glm::uvec3( local_id.x, local_id.y, local_id.z );
				_computeShaderState.inLocalInvocationIndex	= local_id.x + local_id.y * local.x + local_id.z * local.x * local.y;
				_computeShaderState.inGlobalInvocationID	= glm::uvec3( group_id.x * local.x + local_id.x,
																		  group_id.y * local.y + local_id.y,
																		  group_id.z * local.z + local_id.z );

				shader_func( *this );
			}
		}

		_Reset();
		return true;
	}
	
/*
=================================================
	_Reset
=================================================
*/
	void SWShader::_Reset ()
	{
		SWShaderHelper::_Reset();

		_resourceTable = null;
	}
	
/*
=================================================
	_GetBufferMemoryLayout
=================================================
*/
	void SWShader::_GetBufferMemoryLayout (const Fwd_GetSWBufferMemoryLayout &msg) const
	{
		CHECK( _resourceTable->Send( msg ) );
	}
	
/*
=================================================
	_GetImageViewMemoryLayout
=================================================
*/
	void SWShader::_GetImageViewMemoryLayout (const Fwd_GetSWImageViewMemoryLayout &msg) const
	{
		CHECK( _resourceTable->Send( msg ) );
	}
	
/*
=================================================
	_GetTextureMemoryLayout
=================================================
*/
	void SWShader::_GetTextureMemoryLayout (const Fwd_GetSWTextureMemoryLayout &msg) const
	{
		CHECK( _resourceTable->Send( msg ) );
	}

}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT
