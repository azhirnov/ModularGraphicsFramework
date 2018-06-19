// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Shaders/glslang_include.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"

namespace PipelineCompiler
{

	//
	// Shader Includer
	//

	class ShaderCompiler::ShaderIncluder final : public glslang::TShader::Includer
	{
	// types
	private:
		struct IncludeResultImpl final : IncludeResult
		{
		// variables
			const String	_data;

		// methods
            IncludeResultImpl (String &&data, const std::string& headerName, void* userData = null);

			StringCRef GetSource () const	{ return _data; }
		};

		using IncludeResultPtr_t	= UniquePtr< IncludeResultImpl >;
		using IncludeResults_t		= Set< IncludeResultPtr_t >;
		using IncludeDirectories_t	= Array< String >;
		using IncludedFiles_t		= HashMap< String, Ptr<IncludeResultImpl> >;


	// variables
	private:
		IncludeResults_t		_results;
		IncludedFiles_t			_includedFiles;
		IncludeDirectories_t	_directories;
		String					_baseFolder;


	// methods
	public:
		explicit ShaderIncluder (StringCRef baseFolder);
		~ShaderIncluder ();

		void AddDirectory (StringCRef path);
		bool GetHeaderSource (StringCRef header, OUT StringCRef &source) const;

		// TShader::Includer //
        IncludeResult* includeSystem (const char* headerName, const char* includerName, size_t inclusionDepth) override;
        IncludeResult* includeLocal (const char* headerName, const char* includerName, size_t inclusionDepth) override;

        void releaseInclude (IncludeResult *) override;
	};

}	// PipelineCompiler
