// This is generated file
// Created at: 2018/04/29 - 14:47:12

#pragma once

namespace Pipelines
{
// Packing: Std140
struct BigIntTest_SSBO final
{
	uint  result;    // offset: 0, align: 4
	float  _padding0;    // offset: 4, align: 4
	float  _padding1;    // offset: 8, align: 4
	float  _padding2;    // offset: 12, align: 4

	BigIntTest_SSBO ()
	{
		STATIC_ASSERT( sizeof(BigIntTest_SSBO) == 16 );
		STATIC_ASSERT( (offsetof( BigIntTest_SSBO, result ) == 0) and (sizeof( result ) == 4) );
		STATIC_ASSERT( (offsetof( BigIntTest_SSBO, _padding0 ) == 4) and (sizeof( _padding0 ) == 4) );
		STATIC_ASSERT( (offsetof( BigIntTest_SSBO, _padding1 ) == 8) and (sizeof( _padding1 ) == 4) );
		STATIC_ASSERT( (offsetof( BigIntTest_SSBO, _padding2 ) == 12) and (sizeof( _padding2 ) == 4) );
	}
};

};
