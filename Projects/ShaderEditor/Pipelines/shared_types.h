// This is generated file
#pragma once

namespace Pipelines
{
// Packing: Std140
struct ShadertoyUB final
{
	float3  iResolution;    // offset: 0, align: 16
	float  iTime;    // offset: 12, align: 4
	float  iTimeDelta;    // offset: 16, align: 4
	int  iFrame;    // offset: 20, align: 4
	float2  _padding0;    // offset: 24, align: 8
	float4  iChannelTime[4];    // offset: 32, align: 16
	float4  iChannelResolution[4];    // offset: 96, align: 16
	float4  iMouse;    // offset: 160, align: 16
	float4  iDate;    // offset: 176, align: 16
	float  iSampleRate;    // offset: 192, align: 4
	float  _padding1;    // offset: 196, align: 4
	float  _padding2;    // offset: 200, align: 4
	float  _padding3;    // offset: 204, align: 4
	float4  iCameraFrustum[4];    // offset: 208, align: 16
	float4  iCameraPos;    // offset: 272, align: 16

	ShadertoyUB ()
	{
		STATIC_ASSERT( sizeof(ShadertoyUB) == 288 );
		STATIC_ASSERT( (offsetof( ShadertoyUB, iResolution ) == 0) and (sizeof( iResolution ) == 12) );
		STATIC_ASSERT( (offsetof( ShadertoyUB, iTime ) == 12) and (sizeof( iTime ) == 4) );
		STATIC_ASSERT( (offsetof( ShadertoyUB, iTimeDelta ) == 16) and (sizeof( iTimeDelta ) == 4) );
		STATIC_ASSERT( (offsetof( ShadertoyUB, iFrame ) == 20) and (sizeof( iFrame ) == 4) );
		STATIC_ASSERT( (offsetof( ShadertoyUB, iChannelTime ) == 32) and (sizeof( iChannelTime ) == 64) );
		STATIC_ASSERT( (offsetof( ShadertoyUB, iChannelResolution ) == 96) and (sizeof( iChannelResolution ) == 64) );
		STATIC_ASSERT( (offsetof( ShadertoyUB, iMouse ) == 160) and (sizeof( iMouse ) == 16) );
		STATIC_ASSERT( (offsetof( ShadertoyUB, iDate ) == 176) and (sizeof( iDate ) == 16) );
		STATIC_ASSERT( (offsetof( ShadertoyUB, iSampleRate ) == 192) and (sizeof( iSampleRate ) == 4) );
		STATIC_ASSERT( (offsetof( ShadertoyUB, iCameraFrustum ) == 208) and (sizeof( iCameraFrustum ) == 64) );
		STATIC_ASSERT( (offsetof( ShadertoyUB, iCameraPos ) == 272) and (sizeof( iCameraPos ) == 16) );
	}
};

}	// Pipelines

namespace GX_STL::CompileTime::_ctime_hidden_ {
	template <> struct _IsPOD< Pipelines::ShadertoyUB > { static const bool value = true; };
}

