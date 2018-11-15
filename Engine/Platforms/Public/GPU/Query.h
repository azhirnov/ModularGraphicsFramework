// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/QueryEnums.h"

namespace Engine
{
namespace Platforms
{
	
	//
	// Query Pool Description
	//
	struct QueryPoolDescription
	{
	// variables
		EQuery::type				queryType		= Uninitialized;
		EPipelineStatistic::bits	statisticFlags;
		uint						count			= 0;

	// methods
		QueryPoolDescription () {}

		QueryPoolDescription (EQuery::type type, uint count) :
			queryType{type}, count{count} {}

		QueryPoolDescription (EPipelineStatistic::bits flags, uint count) :
			queryType{EQuery::PipelineStatistic}, statisticFlags{flags}, count{count} {}
	};

}	// Platforms


namespace CreateInfo
{

	//
	// Query Pool Create Info
	//
	struct GpuQueryPool
	{
	// variables
		ModulePtr							gpuThread;
		Platforms::QueryPoolDescription		descr;

	// methods
		GpuQueryPool () {}
		GpuQueryPool (const ModulePtr &gpuThread, const Platforms::QueryPoolDescription &desc) : gpuThread{gpuThread}, descr{desc} {}
		explicit GpuQueryPool (const Platforms::QueryPoolDescription &desc) : descr{desc} {}
	};

}	// CreateInfo


namespace GpuMsg
{

	//
	// Get Query Pool Description
	//
	struct GetQueryPoolDescription : _MsgBase_
	{
		Out< Platforms::QueryPoolDescription >	result;
	};


}	// GpuMsg
}	// Engine
