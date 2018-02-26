// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/ShaderLang/SWShaderHelper.h"

#ifdef GRAPHICS_API_SOFT

namespace Engine
{
namespace PlatformSW
{

	//
	// Software Renderer Shader
	//

	class SWShader final : protected SWShaderLang::Impl::SWShaderHelper
	{
	// types
	private:
		using ShaderFunc_t		= PipelineTemplateDescriptor::ShaderSource::SWInvoke;


	// variables
	private:
		ModulePtr		_resourceTable;


	// methods
	public:
		SWShader ();

		bool DispatchCompute (const uint3 &workGroups, const ModulePtr &pipeline, const ModulePtr &resourceTable);


	private:
		// SWShaderHelper //
		void _GetBufferMemoryLayout (const Fwd_GetSWBufferMemoryLayout &) const override;
		void _GetImageViewMemoryLayout (const Fwd_GetSWImageViewMemoryLayout &) const override;
		void _GetTextureMemoryLayout (const Fwd_GetSWTextureMemoryLayout &) const override;

		void _Reset () override;
	};


}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT
