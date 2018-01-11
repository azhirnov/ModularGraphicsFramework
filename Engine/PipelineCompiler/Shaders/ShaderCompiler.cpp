// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/LunarGLASS_Include.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"
#include <sstream>

namespace PipelineCompiler
{

/*
=================================================
	FieldTypeInfo::operator ==
=================================================
*/
	bool ShaderCompiler::FieldTypeInfo::operator == (const FieldTypeInfo &right) const
	{
		return	name		== right.name		and
				typeName	== right.typeName	and
				type		== right.type;
	}
//-----------------------------------------------------------------------------


/*
=================================================
	UnsupportedFunctionalityHandler
=================================================
*/
	static bool glaHasTranslationErrors = false;
	
	static String& _GetTranslationErrors ()
	{
		static String	str;
		return str;
	}

	static void UnsupportedFunctionalityHandler (const std::string &message, gla::EAbortType at)
	{
		if ( at == gla::EAbortType::EATAbort )
			glaHasTranslationErrors = true;

		_GetTranslationErrors() << message << ", file: \"LunarGLASS\"\n";

		//LOG( ("LunarGLASS: "_str << message).cstr(), ELog::Error );
	}
//-----------------------------------------------------------------------------


/*
=================================================
	constructor
=================================================
*/
	ShaderCompiler::ShaderCompiler ()
	{
		#ifdef GX_DEPENDS_OF_PLATFORMS
			_app.Initialize();
			_app.Update();
		#endif

		glslang::InitializeProcess();
		gla::RegisterUnsupportedFunctionalityHandler( &UnsupportedFunctionalityHandler );
	}

/*
=================================================
	destructor
=================================================
*/
	ShaderCompiler::~ShaderCompiler ()
	{
		#ifdef GX_DEPENDS_OF_PLATFORMS
			_app.Quit();
		#endif

		//GetMainSystemInstance()->Send( Message< ModuleMsg::Delete >() );

		glslang::FinalizeProcess();
	}
	
/*
=================================================
	Instance
=================================================
*/
	Ptr<ShaderCompiler>  ShaderCompiler::Instance ()
	{
		return SingletonMultiThread::Instance<ShaderCompiler>();
	}

/*
=================================================
	_CheckGLAErrors
=================================================
*/
	bool ShaderCompiler::_CheckGLAErrors (OUT String *log) const
	{
		if ( log )
		{
			*log << '\n' << _GetTranslationErrors();
		}

		_GetTranslationErrors().Clear();

		bool res = glaHasTranslationErrors;

		glaHasTranslationErrors = false;
		return not res;
	}
	
/*
=================================================
	_OnCompilationFailed
=================================================
*/
	bool ShaderCompiler::_OnCompilationFailed (EShader::type shaderType, EShaderSrcFormat::type fmt, ArrayCRef<StringCRef> source, INOUT String &log) const
	{
		// pattern: <error/warning>: <number:<line>: <description>		// glslang errors format
		
		Array< StringCRef >		lines;
		Array< StringCRef >		tokens;
		Array< usize >			num_lines;	num_lines.Resize( source.Count() );
		String					str;		str.Reserve( log.Length() * 2 );
		usize					prev_line = UMax;

		StringParser::DivideLines( log, OUT lines );
		
		FOR( i, lines )
		{
			StringParser::DivideString_CPP( lines[i], OUT tokens );
			
			bool	added = false;

			if ( tokens.Count() > 8 and
				 tokens[1] == ":" and
				 tokens[3] == ":" and
				 tokens[5] == ":" )
			{
				const usize	line		= (usize) StringUtils::ToInt32( String(tokens[4]) );
				const bool	is_err		= tokens[0].EqualsIC( "error" );
				usize		cur_line	= 0;
				usize		pos			= 0;
				StringCRef	line_str;
				
				// unite error in same source lines
				if ( prev_line == line )
				{
					str << lines[i] << "\n";
					continue;
				}
				
				prev_line = line;
				
				FOR( j, source )
				{
					if ( num_lines[j] == 0 ) {
						num_lines[j] = StringParser::CalculateNumberOfLines( source[j] ) + 1;
					}

					cur_line += num_lines[j];

					if ( cur_line <= line )
						continue;

					usize	local_line	= line - (cur_line - num_lines[j]);

					CHECK( local_line < num_lines[j] );

					CHECK( StringParser::MoveToLine( source[j], INOUT pos, local_line-1 ) );

					StringParser::ReadLineToEnd( source[j], INOUT pos, OUT line_str );

					str << "in source (" << j << ":" << local_line << "):\n\"" << line_str << "\"\n" << lines[i] << "\n";
					
					added = true;
					break;
				}
			}

			if ( not added )
			{
				str << DEBUG_ONLY( "<unknown> " << ) lines[i] << "\n";
			}
		}
		
		log = RVREF(str);
		return false;
	}
	
/*
=================================================
	GenerateResources
=================================================
*/
	void ShaderCompiler::_GenerateResources (OUT TBuiltInResource& resources)
	{
		resources.maxLights = 32;
		resources.maxClipPlanes = 6;
		resources.maxTextureUnits = 32;
		resources.maxTextureCoords = 32;
		resources.maxVertexAttribs = 64;
		resources.maxVertexUniformComponents = 4096;
		resources.maxVaryingFloats = 64;
		resources.maxVertexTextureImageUnits = 32;
		resources.maxCombinedTextureImageUnits = 80;
		resources.maxTextureImageUnits = 32;
		resources.maxFragmentUniformComponents = 4096;
		resources.maxDrawBuffers = 32;
		resources.maxVertexUniformVectors = 128;
		resources.maxVaryingVectors = 8;
		resources.maxFragmentUniformVectors = 16;
		resources.maxVertexOutputVectors = 16;
		resources.maxFragmentInputVectors = 15;
		resources.minProgramTexelOffset = -8;
		resources.maxProgramTexelOffset = 7;
		resources.maxClipDistances = 8;
		resources.maxComputeWorkGroupCountX = 65535;
		resources.maxComputeWorkGroupCountY = 65535;
		resources.maxComputeWorkGroupCountZ = 65535;
		resources.maxComputeWorkGroupSizeX = 1024;
		resources.maxComputeWorkGroupSizeY = 1024;
		resources.maxComputeWorkGroupSizeZ = 64;
		resources.maxComputeUniformComponents = 1024;
		resources.maxComputeTextureImageUnits = 16;
		resources.maxComputeImageUniforms = 8;
		resources.maxComputeAtomicCounters = 8;
		resources.maxComputeAtomicCounterBuffers = 1;
		resources.maxVaryingComponents = 60;
		resources.maxVertexOutputComponents = 64;
		resources.maxGeometryInputComponents = 64;
		resources.maxGeometryOutputComponents = 128;
		resources.maxFragmentInputComponents = 128;
		resources.maxImageUnits = 8;
		resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
		resources.maxImageSamples = 0;
		resources.maxVertexImageUniforms = 0;
		resources.maxTessControlImageUniforms = 0;
		resources.maxTessEvaluationImageUniforms = 0;
		resources.maxGeometryImageUniforms = 0;
		resources.maxFragmentImageUniforms = 8;
		resources.maxCombinedImageUniforms = 8;
		resources.maxGeometryTextureImageUnits = 16;
		resources.maxGeometryOutputVertices = 256;
		resources.maxGeometryTotalOutputComponents = 1024;
		resources.maxGeometryUniformComponents = 1024;
		resources.maxGeometryVaryingComponents = 64;
		resources.maxTessControlInputComponents = 128;
		resources.maxTessControlOutputComponents = 128;
		resources.maxTessControlTextureImageUnits = 16;
		resources.maxTessControlUniformComponents = 1024;
		resources.maxTessControlTotalOutputComponents = 4096;
		resources.maxTessEvaluationInputComponents = 128;
		resources.maxTessEvaluationOutputComponents = 128;
		resources.maxTessEvaluationTextureImageUnits = 16;
		resources.maxTessEvaluationUniformComponents = 1024;
		resources.maxTessPatchComponents = 120;
		resources.maxPatchVertices = 32;
		resources.maxTessGenLevel = 64;
		resources.maxViewports = 16;
		resources.maxVertexAtomicCounters = 0;
		resources.maxTessControlAtomicCounters = 0;
		resources.maxTessEvaluationAtomicCounters = 0;
		resources.maxGeometryAtomicCounters = 0;
		resources.maxFragmentAtomicCounters = 8;
		resources.maxCombinedAtomicCounters = 8;
		resources.maxAtomicCounterBindings = 1;
		resources.maxVertexAtomicCounterBuffers = 0;
		resources.maxTessControlAtomicCounterBuffers = 0;
		resources.maxTessEvaluationAtomicCounterBuffers = 0;
		resources.maxGeometryAtomicCounterBuffers = 0;
		resources.maxFragmentAtomicCounterBuffers = 1;
		resources.maxCombinedAtomicCounterBuffers = 1;
		resources.maxAtomicCounterBufferSize = 16384;
		resources.maxTransformFeedbackBuffers = 4;
		resources.maxTransformFeedbackInterleavedComponents = 64;
		resources.maxCullDistances = 8;
		resources.maxCombinedClipAndCullDistances = 8;
		resources.maxSamples = 4;

		resources.limits.nonInductiveForLoops = 1;
		resources.limits.whileLoops = 1;
		resources.limits.doWhileLoops = 1;
		resources.limits.generalUniformIndexing = 1;
		resources.limits.generalAttributeMatrixVectorIndexing = 1;
		resources.limits.generalVaryingIndexing = 1;
		resources.limits.generalSamplerIndexing = 1;
		resources.limits.generalVariableIndexing = 1;
		resources.limits.generalConstantMatrixVectorIndexing = 1;
	}
	
/*
=================================================
	_GLSLangParse
=================================================
*/
	bool ShaderCompiler::_GLSLangParse (const Config &cfg, const _ShaderData &shaderData, OUT String &log, OUT _GLSLangResult &result) const
	{
		log.Clear();
		
		glslang::EShClient			client			= glslang::EShClientOpenGL;
		uint						client_version	= GLSL_VERSION;

		glslang::EShTargetLanguage	target			= glslang::EShTargetNone;
		uint						target_version	= 0;

		uint						version			= 0;
		glslang::EShSource			sh_source;

		bool						is_vulkan		= false;

		switch ( cfg.source )
		{
			case EShaderSrcFormat::GXSL :
				sh_source = glslang::EShSourceGxsl;
				version	  = GLSL_VERSION;
				break;

			case EShaderSrcFormat::GLSL :
				sh_source = glslang::EShSourceGlsl;
				version	  = GLSL_VERSION;
				break;
				
			case EShaderSrcFormat::GXSL_Vulkan :
				sh_source = glslang::EShSourceGxsl;
				version	  = GLSL_VERSION;
				is_vulkan = true;
				break;

			case EShaderSrcFormat::GLSL_Vulkan :
				sh_source = glslang::EShSourceGlsl;
				version	  = GLSL_VERSION;
				is_vulkan = true;
				break;

			case EShaderSrcFormat::GLSL_ES :
				sh_source = glslang::EShSourceGlsl;
				version   = GLSL_ES_VERSION;
				break;

			case EShaderSrcFormat::HLSL :
				sh_source = glslang::EShSourceHlsl;
				version   = HLSL_VERSION;
				break;

			default :
				RETURN_ERR( "unsupported shader format" );
		}

		switch ( cfg.target )
		{
			case EShaderDstFormat::SPIRV_Source :
			case EShaderDstFormat::SPIRV_Binary :
				version			= VULKAN_VERSION;
				client			= glslang::EShClientVulkan;		// is_vulkan ? glslang::EShClientVulkan : glslang::EShClientOpenGL
				client_version	= VULKAN_VERSION;				// is_vulkan ? VULKAN_VERSION : GLSL_VERSION;
				target			= glslang::EshTargetSpv;
				target_version	= SPIRV_VERSION;
				break;

			case EShaderDstFormat::GLSL_Source :
			default :
				break;
		}

		String				src;
		EShMessages			messages	= EShMsgDefault;
		TBuiltInResource	resources;	_GenerateResources( OUT resources );
		EShLanguage			stage		= ConvertShaderType( shaderData.type );
		auto&				shader		= result.shader;

		shader = new glslang::TShader( stage );
		src.Clear();

		FOR( i, shaderData.src ) {
			src << shaderData.src[i] << '\n';
		}
			
		// add version
		if ( not src.HasSubString( "#version" ) )
			("#version "_str << GLSL_VERSION << " core\n") >> src;

		// parse shader
		const char *	src_ptr		= src.ptr();
		const char *	entry_point	= shaderData.entry.Empty() ? "main" : shaderData.entry.cstr();

		shader->setStrings( &src_ptr, 1 );
		shader->setEntryPoint( entry_point );
		shader->setEnvInput( sh_source, stage, client, version );
		shader->setEnvClient( client, client_version );
		shader->setEnvTarget( target, target_version );
		
		if ( not shader->parse( &resources, GLSL_VERSION, false, messages ) )
		{
			log << shader->getInfoLog();
			_OnCompilationFailed( shaderData.type, cfg.source, { StringCRef(src) }, INOUT log );
			return false;
		}
		
		result.prog.addShader( shader.ptr() );

		if ( not result.prog.link( messages ) )
		{
			log << result.prog.getInfoLog();
			_OnCompilationFailed( shaderData.type, cfg.source, { StringCRef(src) }, INOUT log );
			return false;
		}
		return true;
	}
	
/*
=================================================
	Translate
=================================================
*/
	bool ShaderCompiler::Translate (EShader::type shaderType, ArrayCRef<StringCRef> source, StringCRef entryPoint,
									const Config &cfg, OUT String &log, OUT BinaryArray &result)
	{
		CHECK_ERR( not source.Empty() and not entryPoint.Empty() );

		log.Clear();
		result.Clear();

		_ShaderData	data;
		data.entry	= entryPoint;
		data.src	= source;
		data.type	= shaderType;

		switch ( cfg.target )
		{
			case EShaderDstFormat::GLSL_Source :
			//case EShaderDstFormat::GLSL_ES_Source :
			{
				switch ( cfg.source )
				{
					case EShaderSrcFormat::GLSL :
					//case EShaderSrcFormat::GLSL_ES :
					{
						if ( cfg.optimize )
							return _OptimizeGLSL( cfg, data, OUT log, OUT result );
						else
						if ( cfg.skipExternals or cfg.typeReplacer )
							;	// go to 'case GXSL'
						else
							return _CopySource( data, OUT result );
					}
					case EShaderSrcFormat::GXSL :
					{
						_GLSLangResult	glslang_data;
						Config			cfg2 = cfg;
						
						cfg2.skipExternals = cfg.optimize ? false : cfg.skipExternals;

						CHECK_COMP( _GLSLangParse( cfg2, data, OUT log, OUT glslang_data ) );
						CHECK_COMP( _ReplaceTypes( glslang_data, cfg2 ) );
						CHECK_COMP( _TranslateGXSLtoGLSL( cfg2, glslang_data, OUT log, OUT result ) );
						
						if ( cfg.optimize )
						{
							_ShaderData	data2 = data;
							data2.src.Clear();
							data2.src << (const char*) result.ptr();
							data2.entry = "main";

							cfg2				= cfg;
							cfg2.source			= EShaderSrcFormat::GLSL;
							cfg2.typeReplacer	= Uninitialized;

							BinaryArray	temp;
							CHECK_COMP( _OptimizeGLSL( cfg2, data2, OUT log, OUT temp ) );

							result = RVREF( temp );
						}
						return true;
					}
					default :
						RETURN_ERR( "unsupported combination!" );
				}
			}

			case EShaderDstFormat::GLSL_Binary :
			//case EShaderDstFormat::GLSL_ES_Binary :
			{
				Config		cfg2	= cfg;
				cfg2.skipExternals	= false;
				cfg2.target			= EShaderDstFormat::GLSL_Source;

				BinaryArray	temp;
				CHECK_COMP( Translate( shaderType, source, entryPoint, cfg2, OUT log, OUT temp ) );

				_ShaderData	data2 = data;
				data2.src.Clear();
				data2.src << (const char*) temp.ptr();
				data2.entry = "main";

				cfg2		= cfg;
				cfg2.source	= cfg.target == EShaderDstFormat::GLSL_Binary ? EShaderSrcFormat::GLSL : EShaderSrcFormat::GLSL_ES;

				CHECK_COMP( _CompileGLSL( cfg2, data2, OUT log, OUT result ) );
				return true;
			}

			case EShaderDstFormat::SPIRV_Binary :
			case EShaderDstFormat::SPIRV_Source :
			{
				Config		cfg2	= cfg;
				cfg2.optimize		= false;
				cfg2.skipExternals	= false;
				cfg2.target			= EShaderDstFormat::GLSL_Source;
				cfg2.typeReplacer	= Uninitialized;

				BinaryArray	temp;
				CHECK_COMP( Translate( shaderType, source, entryPoint, cfg2, OUT log, OUT temp ) );
				
				_ShaderData	data2 = data;
				data2.src.Clear();
				data2.src << (const char*) temp.ptr();
				data2.entry = "main";

				cfg2		= cfg;
				cfg2.source	= EShaderSrcFormat::GLSL;

				_GLSLangResult	glslang_data;
				CHECK_COMP( _GLSLangParse( cfg2, data2, OUT log, OUT glslang_data ) );
				CHECK_COMP( _ReplaceTypes( glslang_data, cfg2 ) );

				cfg2.typeReplacer = Uninitialized;
				CHECK_COMP( _CompileWithLunarGOO( cfg2, glslang_data, OUT log, OUT result ) );

				return true;
			}
				
			case EShaderDstFormat::CL_Source :
			{
				Config		cfg2	= cfg;
				_ShaderData	data2	= data;
				BinaryArray	temp;

				switch ( cfg.source )
				{
					case EShaderSrcFormat::CL :
						CHECK_COMP( not cfg.typeReplacer );
						return _CopySource( data, OUT result );

					case EShaderSrcFormat::GXSL :
					case EShaderSrcFormat::GXSL_Vulkan :
						break;

					default :
					{
						cfg2.optimize		= true;
						cfg2.skipExternals	= false;
						cfg2.target			= EShaderDstFormat::GLSL_Source;
						cfg2.typeReplacer	= Uninitialized;

						CHECK_COMP( Translate( shaderType, source, entryPoint, cfg2, OUT log, OUT temp ) );

						data2.src.Clear();
						data2.src << (const char*) temp.ptr();
						data2.entry = "main";

						cfg2				= cfg;
						cfg2.source			= EShaderSrcFormat::GLSL;
						cfg2.typeReplacer	= cfg.typeReplacer;
					}
				}

				_GLSLangResult	glslang_data;
				CHECK_COMP( _GLSLangParse( cfg2, data2, OUT log, OUT glslang_data ) );
				CHECK_COMP( _ReplaceTypes( glslang_data, cfg2 ) );
				CHECK_COMP( _TranslateGXSLtoCL( cfg2, glslang_data, OUT log, OUT result ) );
				return true;
			}

			case EShaderDstFormat::CL_Binary :
			{
				BinaryArray	temp;
				_ShaderData	data2 = data;
				Config		cfg2  = cfg;

				cfg2.skipExternals	= false;
				cfg2.target			= EShaderDstFormat::CL_Source;

				CHECK_COMP( Translate( shaderType, source, entryPoint, cfg2, OUT log, OUT temp ) );

				data2.src.Clear();
				data2.src << (const char*) temp.ptr();
				data2.entry = "main";

				cfg2				= cfg;
				cfg2.source			= EShaderSrcFormat::CL;
				cfg2.typeReplacer	= Uninitialized;

				CHECK_COMP( _CompileCL( cfg2, data2, OUT log, OUT result ) );
				return true;
			}

			case EShaderDstFormat::CPP_Module :
			{
				Config		cfg2	= cfg;
				_ShaderData	data2	= data;
				BinaryArray	temp;

				switch ( cfg.source )
				{
					case EShaderSrcFormat::GXSL :
					case EShaderSrcFormat::GXSL_Vulkan :
						break;

					default :
					{
						cfg2.optimize		= true;
						cfg2.skipExternals	= false;
						cfg2.target			= EShaderDstFormat::GLSL_Source;
						cfg2.typeReplacer	= Uninitialized;

						CHECK_COMP( Translate( shaderType, source, entryPoint, cfg2, OUT log, OUT temp ) );

						data2.src.Clear();
						data2.src << (const char*) temp.ptr();
						data2.entry = "main";

						cfg2				= cfg;
						cfg2.source			= EShaderSrcFormat::GLSL;
						cfg2.typeReplacer	= cfg.typeReplacer;
					}
				}
				
				_GLSLangResult	glslang_data;
				CHECK_COMP( _GLSLangParse( cfg2, data2, OUT log, OUT glslang_data ) );
				CHECK_COMP( _ReplaceTypes( glslang_data, cfg2 ) );
				CHECK_COMP( _TranslateGXSLtoCPP( cfg2, glslang_data, OUT log, OUT result ) );
				return true;
			}
				
			case EShaderDstFormat::HLSL_Source :
			{
				Config		cfg2	= cfg;
				_ShaderData	data2	= data;
				BinaryArray	temp;

				switch ( cfg.source )
				{
					case EShaderSrcFormat::GLSL :
					case EShaderSrcFormat::GLSL_Vulkan :
					case EShaderSrcFormat::GXSL :
					case EShaderSrcFormat::GXSL_Vulkan :
						break;

					default :
					{
						cfg2.optimize		= true;
						cfg2.skipExternals	= false;
						cfg2.target			= EShaderDstFormat::GLSL_Source;
						cfg2.typeReplacer	= Uninitialized;

						CHECK_COMP( Translate( shaderType, source, entryPoint, cfg2, OUT log, OUT temp ) );

						data2.src.Clear();
						data2.src << (const char*) temp.ptr();
						data2.entry = "main";

						cfg2				= cfg;
						cfg2.source			= EShaderSrcFormat::GLSL;
						cfg2.typeReplacer	= cfg.typeReplacer;
					}
				}
				
				_GLSLangResult	glslang_data;
				CHECK_COMP( _GLSLangParse( cfg2, data2, OUT log, OUT glslang_data ) );
				CHECK_COMP( _ReplaceTypes( glslang_data, cfg2 ) );
				CHECK_COMP( _TranslateGXSLtoHLSL( cfg2, glslang_data, OUT log, OUT result ) );
				return true;
			}

			case EShaderDstFormat::HLSL_Binary :
			{
				BinaryArray	temp;
				_ShaderData	data2 = data;
				Config		cfg2  = cfg;

				cfg2.skipExternals	= false;
				cfg2.target			= EShaderDstFormat::HLSL_Source;

				CHECK_COMP( Translate( shaderType, source, entryPoint, cfg2, OUT log, OUT temp ) );

				data2.src.Clear();
				data2.src << (const char*) temp.ptr();
				data2.entry = "main";

				cfg2				= cfg;
				cfg2.source			= EShaderSrcFormat::HLSL;
				cfg2.typeReplacer	= Uninitialized;

				CHECK_COMP( _CompileHLSL( cfg2, data2, OUT log, OUT result ) );
				return true;
			}

			case EShaderDstFormat::Metal_Source :
			case EShaderDstFormat::Metal_Binary :
			default :
				RETURN_ERR( "unsupported shader compilation target!" );
		}
		return true;
	}
	
/*
=================================================
	_CompileWithLunarGOO
=================================================
*/
	bool ShaderCompiler::_CompileWithLunarGOO (const Config &cfg, const _GLSLangResult &glslangData, OUT String &log, OUT BinaryArray &result) const
	{
		ASSERT( not cfg.typeReplacer );

		// choose target
		EProfile	shader_profile	= ENoProfile;
		uint		shader_version	= 0;
		bool		use_spirv		= false;
		//bool		is_vulkan		= false;

		switch ( cfg.source )
		{
			case EShaderSrcFormat::GLSL :
			case EShaderSrcFormat::GXSL :
			case EShaderSrcFormat::GLSL_ES :
				break;

			case EShaderSrcFormat::GLSL_Vulkan :
			case EShaderSrcFormat::GXSL_Vulkan :
				//is_vulkan = true;
				break;

			default :
				RETURN_ERR( "unsupported shader source!" );
		}

		switch ( cfg.target )
		{
			case EShaderDstFormat::GLSL_Source :
				shader_profile	= ECoreProfile;
				shader_version	= GLSL_VERSION;
				break;

			case EShaderDstFormat::GLSL_ES_Source :
				shader_profile	= EEsProfile;
				shader_version	= GLSL_ES_VERSION;
				break;

			case EShaderDstFormat::SPIRV_Source :
			case EShaderDstFormat::SPIRV_Binary :
				shader_profile	= ECoreProfile;
				shader_version	= GLSL_VERSION;
				use_spirv		= true;
				CHECK_ERR( not cfg.skipExternals );
				break;
			
			default :
				RETURN_ERR( "unsupported shader compilation target!" );
		}

		//CHECK_COMP( is_vulkan == use_spirv );	// TODO


		// optimize shader
		const int		substitution_level	= 1;
		
		const glslang::TIntermediate* intermediate = glslangData.prog.getIntermediate( glslangData.shader->getStage() );
		CHECK_ERR( intermediate );

        gla::GlslManager manager( cfg.obfuscate, cfg.filterInactive, substitution_level );

        manager.options.backend								= gla::BackendOption::GLSL;

		manager.options.optimizations.adce					= true;
		manager.options.optimizations.coalesce				= true;
		manager.options.optimizations.flattenHoistThreshold = 20;
		manager.options.optimizations.gvn					= true;
		manager.options.optimizations.inlineThreshold		= cfg.inlineThreshold;
		manager.options.optimizations.loopUnrollThreshold	= cfg.loopUnrollThreshold;
		manager.options.optimizations.reassociate			= true;

		if ( use_spirv )
		{
			std::vector<unsigned int>	spirv;
			std::stringstream			spv_source;
			glslang::SpvOptions			spv_options;

			spv_options.generateDebugInfo	= not cfg.optimize;
			spv_options.disableOptimizer	= not cfg.optimize;
			spv_options.optimizeSize		= cfg.optimize;
			
			glslang::GlslangToSpv( *intermediate, OUT spirv, &spv_options );

			if ( cfg.target == EShaderDstFormat::SPIRV_Source )
			{
				spv::Parameterize();
				spv::Disassemble( OUT spv_source, spirv );

				result = BinArrayCRef::FromStd( spv_source.str() );
				return true;
			}
			else
			{
				result = BinArrayCRef::FromStd( spirv );
				return true;
			}
		}
		else
		{
			TranslateGlslangToTop( *intermediate, manager );
		}

		manager.setVersion( shader_version );
		manager.setProfile( shader_profile );

		_CheckGLAErrors();	// clear errors

		manager.translateTopToBottom();
		manager.translateBottomToTarget();

		CHECK_COMP( _CheckGLAErrors( OUT &log ) );

		result = BinArrayCRef::From(StringCRef( cfg.skipExternals ? manager.getIndexShader() : manager.getGeneratedShader() ));
		return true;
	}
	
/*
=================================================
	_OptimizeGLSL
=================================================
*/
	bool ShaderCompiler::_OptimizeGLSL (const Config &cfg, const _ShaderData &data, OUT String &log, OUT BinaryArray &result) const
	{
		Config			cfg2 = cfg;
		_GLSLangResult	glslang_data;

		CHECK_COMP( _GLSLangParse( cfg2, data, OUT log, OUT glslang_data ) );
		CHECK_COMP( _ReplaceTypes( glslang_data, cfg2 ) );

		cfg2.typeReplacer = Uninitialized;
		CHECK_COMP( _CompileWithLunarGOO( cfg2, glslang_data, OUT log, OUT result ) );
		return true;
	}
	
/*
=================================================
	_CopySource
=================================================
*/
	bool ShaderCompiler::_CopySource (const _ShaderData &data, OUT BinaryArray &result) const
	{
		const ubyte	newline[] = { '\n' };

		FOR( i, data.src )
		{
			if ( data.src[i].Empty() )
				continue;

			result << BinArrayCRef::From( data.src[i] );
			result << newline;
		}
		return true;
	}

}	// PipelineCompiler
