// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/glslang_include.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_Includer.h"
#include "Core/STL/Algorithms/StringParser.h"
#include "Core/STL/ThreadSafe/Singleton.h"

#ifdef COMPILER_MSVC
# pragma warning (push, 1)
# pragma warning (disable: 4456)
# pragma warning (disable: 4458)
# pragma warning (disable: 4244)
# pragma warning (disable: 4668)
# pragma warning (disable: 4996)
# pragma warning (disable: 4946)
#endif

#ifdef COMPILER_MSVC
# pragma warning (pop)
#endif

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


#ifdef GX_PIPELINECOMPILER_USE_PLATFORMS
/*
=================================================
	Initialize
=================================================
*/
	bool ShaderCompiler::_BaseApp::Initialize ()
	{
		auto		ms		= GetMainSystemInstance();
		auto		mf		= ms->GlobalSystems()->modulesFactory;
		auto		thread	= ms->GlobalSystems()->parallelThread;
		
		ms->AddModule( GModID::type(0), CreateInfo::Platform{} );
		thread->AddModule( WinWindowModuleID, CreateInfo::Window{ "", Uninitialized, uint2(1), int2(0), CreateInfo::Window::EVisibility::Invisible } );

		ModulePtr	glcontext;
		ModulePtr	clcontext;

		CHECK_ERR( mf->Create(
						0,
						ms->GlobalSystems(),
						CreateInfo::GpuContext{ ComputeSettings{ "CL 1.2"_GAPI } },
						OUT clcontext ) );

		CHECK_ERR( mf->Create(
						0,
						ms->GlobalSystems(),
						CreateInfo::GpuContext{ ComputeSettings{ "GL 4.5"_GAPI } },
						OUT glcontext ) );

		ms->Send( ModuleMsg::AttachModule{ glcontext });
		ms->Send( ModuleMsg::AttachModule{ clcontext });

		CHECK_ERR( mf->Create(
						0,
						ms->GlobalSystems(),
						CreateInfo::GpuThread{ ComputeSettings{ "CL 1.2"_GAPI } },
						OUT _clthread ) );

		CHECK_ERR( mf->Create(
						0,
						ms->GlobalSystems(),
						CreateInfo::GpuThread{ ComputeSettings{ "GL 4.5"_GAPI } },
						OUT _glthread ) );
	
		thread->Send( ModuleMsg::AttachModule{ _glthread });
		thread->Send( ModuleMsg::AttachModule{ _clthread });

		_glthread->Subscribe( this, &_BaseApp::_GLInit );
		_clthread->Subscribe( this, &_BaseApp::_CLInit );

		// finish initialization
		ModuleUtils::Initialize({ ms });

		return true;
	}

/*
=================================================
	_CLInit
=================================================
*/
	bool ShaderCompiler::_BaseApp::_CLInit (const GpuMsg::DeviceCreated &)
	{
		_clInit = true;
		return true;
	}
	
/*
=================================================
	_GLInit
=================================================
*/
	bool ShaderCompiler::_BaseApp::_GLInit (const GpuMsg::DeviceCreated &)
	{
		_glInit = true;
		return true;
	}
	
/*
=================================================
	IsContextInit
=================================================
*/
	bool ShaderCompiler::_BaseApp::IsContextInit () const
	{
		return _glInit and _clInit;
	}

/*
=================================================
	Quit
=================================================
*/
	void ShaderCompiler::_BaseApp::Quit ()
	{
		_looping = false;
		_clthread = null;
		_glthread = null;
	}
	
/*
=================================================
	Update
=================================================
*/
	bool ShaderCompiler::_BaseApp::Update ()
	{
		if ( not _looping )
			return false;

		GetMainSystemInstance()->Send( ModuleMsg::Update{} );
		return true;
	}
#endif	// GX_PIPELINECOMPILER_USE_PLATFORMS
//-----------------------------------------------------------------------------


/*
=================================================
	constructor
=================================================
*/
	ShaderCompiler::ShaderCompiler ()
	{
		glslang::InitializeProcess();
	}

/*
=================================================
	destructor
=================================================
*/
	ShaderCompiler::~ShaderCompiler ()
	{
		DestroyContext();

		glslang::FinalizeProcess();
	}
	
/*
=================================================
	InitializeContext
=================================================
*/
	bool ShaderCompiler::InitializeContext ()
	{
	#ifdef GX_PIPELINECOMPILER_USE_PLATFORMS
		if ( not _app )
		{
			Platforms::RegisterPlatforms();

			_app = New<_BaseApp>();
			CHECK_ERR( _app->Initialize() );

			for (; not _app->IsContextInit() and _app->Update(); )
			{
				OS::CurrentThread::Yield();
			}
		}
	#endif

		return true;
	}
	
/*
=================================================
	DestroyContext
=================================================
*/
	void ShaderCompiler::DestroyContext ()
	{
	#ifdef GX_PIPELINECOMPILER_USE_PLATFORMS
		if ( not _app )
			return;

		_app->Quit();
		_app = null;

		GetMainSystemInstance()->Send( ModuleMsg::Delete{} );
	#endif
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
	ParseGLSLError
=================================================
*/
	struct GLSLErrorInfo
	{
	// variables
		StringCRef	description;
		StringCRef	fileName;
		uint		sourceIndex;
		usize		line;
		bool		isError;

	// methods
		GLSLErrorInfo () : sourceIndex{0}, line{UMax}, isError{false} {}
	};

	static bool ParseGLSLError (StringCRef line, OUT GLSLErrorInfo &info)
	{
		const StringCRef	c_error		= "error";
		const StringCRef	c_warning	= "warning";

		usize				pos = 0;

		const auto			ReadToken	= LAMBDA( &line, &pos ) (OUT bool &isNumber)
		{{
						isNumber= true;
			const usize	start	= pos;

			for (; pos < line.Length() and line[pos] != ':'; ++pos) {
				isNumber &= (line[pos] >= '0' and line[pos] <= '9');
			}
			return line.SubString( start, pos - start );
		}};

		const auto			SkipSeparator = LAMBDA( &line, &pos ) ()
		{{
			if ( line[pos] == ':' and line[pos+1] == ' ' )
				pos += 2;
			else if ( line[pos] == ':' )
				pos += 1;
			else
				return false;

			return true;
		}};

		
		// parse error/warning/...
		if ( line.StartsWithIC( c_error ) )
		{
			pos			+= c_error.Length();
			info.isError = true;
		}
		else
		if ( line.StartsWithIC( c_warning ) )
		{
			pos			+= c_warning.Length();
			info.isError = false;
		}
		else
			return false;

		if ( not SkipSeparator() )
			return false;


		// parse source index or header name
		{
			bool				is_number;
			const StringCRef	src		= ReadToken( OUT is_number );

			if ( not SkipSeparator() )
				return false;

			if ( not is_number )
				info.fileName = src;
			else
				info.sourceIndex = StringUtils::ToInt32( String(src) );
		}


		// parse line number
		{
			bool				is_number;
			const StringCRef	src		= ReadToken( OUT is_number );

			if ( not SkipSeparator() or not is_number )
				return false;

			info.line = StringUtils::ToInt32( String(src) );
		}

		info.description = line.SubString( pos );
		return true;
	}

/*
=================================================
	_OnCompilationFailed
=================================================
*/
	bool ShaderCompiler::_OnCompilationFailed (EShader::type, EShaderSrcFormat::type, ArrayCRef<StringCRef> source,
											   const ShaderIncluder &includer, INOUT String &log) const
	{
		// glslang errors format:
		// pattern: <error/warning>: <number>:<line>: <description>
		// pattern: <error/warning>: <file>:<line>: <description>
		
		Array< StringCRef >		lines;
		Array< StringCRef >		tokens;
		Array< usize >			num_lines;	num_lines.Resize( source.Count() );
		String					str;		str.Reserve( log.Length() * 2 );
		usize					prev_line = UMax;

		StringParser::DivideLines( log, OUT lines );
		
		FOR( i, lines )
		{
			bool			added = false;
			GLSLErrorInfo	error_info;

			if ( ParseGLSLError( lines[i], OUT error_info ) )
			{
				// unite error in same source lines
				if ( prev_line == error_info.line )
				{
					str << lines[i] << "\n";
					continue;
				}

				prev_line = error_info.line;

				if ( error_info.fileName.Empty() )
				{
					// search in sources
					StringCRef	cur_source	= error_info.sourceIndex < source.Count() ? source[ error_info.sourceIndex ] : "";
					usize		lines_count	= error_info.sourceIndex < num_lines.Count() ? num_lines[ error_info.sourceIndex ] : 0;

					if ( lines_count == 0 )
					{
						lines_count = StringParser::CalculateNumberOfLines( cur_source ) + 1;

						if ( error_info.sourceIndex < num_lines.Count() )
							num_lines[ error_info.sourceIndex ] = lines_count;
					}
					
					CHECK( error_info.line < lines_count );

					usize		pos = 0;
					CHECK( StringParser::MoveToLine( cur_source, INOUT pos, error_info.line-1 ) );

					StringCRef	line_str;
					StringParser::ReadLineToEnd( cur_source, INOUT pos, OUT line_str );

					str << "in source (" << error_info.sourceIndex << ": " << error_info.line << "):\n\"" << line_str << "\"\n" << lines[i] << "\n";
					added = true;
				}
				else
				{
					// search in header
					StringCRef	src;
					if ( includer.GetHeaderSource( error_info.fileName, OUT src ) )
					{
						const usize	lines_count = StringParser::CalculateNumberOfLines( src ) + 1;
						const usize	local_line	= error_info.line;
						usize		pos			= 0;
						StringCRef	line_str;

						CHECK( local_line < lines_count );

						CHECK( StringParser::MoveToLine( src, INOUT pos, local_line-1 ) );
						
						StringParser::ReadLineToEnd( src, INOUT pos, OUT line_str );

						str << "in source (" << error_info.fileName << ": " << local_line << "):\n\"" << line_str << "\"\n" << lines[i] << "\n";
						added = true;
					}
				}
			}

			if ( not added )
			{
				str << DEBUG_ONLY( "<unknown> " << ) lines[i] << "\n";
			}
		}
		
		log = RVREF(str);

		WARNING( log.cstr() );
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
	bool ShaderCompiler::_GLSLangParse (const Config &cfg, const _ShaderData &shaderData, StringCRef baseFolder, OUT String &log, OUT _GLSLangResult &result) const
	{
		log.Clear();
		
		glslang::EShClient					client			= glslang::EShClientOpenGL;
		glslang::EshTargetClientVersion		client_version	= glslang::EShTargetOpenGL_450;

		glslang::EShTargetLanguage			target			= glslang::EShTargetNone;
		glslang::EShTargetLanguageVersion	target_version	= glslang::EShTargetLanguageVersion(0);

		uint								version			= 0;
		glslang::EShSource					sh_source;

		bool								is_vulkan		= false;

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

			case EShaderSrcFormat::GLSL_ES_2 :
				sh_source = glslang::EShSourceGlsl;
				version   = 200;
				break;

			case EShaderSrcFormat::GLSL_ES_3 :
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
			{
				version			= VULKAN_VERSION;
				client			= is_vulkan ? glslang::EShClientVulkan : glslang::EShClientOpenGL;
				client_version	= is_vulkan ?
										(VULKAN_VERSION == 110 ? glslang::EShTargetVulkan_1_1 : glslang::EShTargetVulkan_1_0) :
										glslang::EShTargetOpenGL_450;
				target			= glslang::EshTargetSpv;
				target_version	= SPIRV_VERSION == 130 ? glslang::EShTargetSpv_1_3 : glslang::EShTargetSpv_1_0;
				break;
			}
			case EShaderDstFormat::GLSL_Source :
			default :
				break;
		}

		Array<const char*>	sources;
		EShMessages			messages	= EShMsgDefault;
		TBuiltInResource	resources;	_GenerateResources( OUT resources );
		EShLanguage			stage		= ConvertShaderType( shaderData.type );
		auto&				shader		= result.shader;
		bool				has_version	= false;
		const String		version_str	= "#version "_str << GLSL_VERSION << " core\n";

		shader = new glslang::TShader( stage );

		for (auto& src : shaderData.src)
		{
			sources << (src.Empty() ? "" : src.cstr());

			if ( src.HasSubString( "#version" ) )
				has_version |= true;
		}
		
		// add version
		if ( not has_version ) {
			sources.PushFront( version_str.cstr() );
		}

		// parse shader
		const char *	entry_point	= shaderData.entry.Empty() ? "main" : shaderData.entry.cstr();
		ShaderIncluder	includer	{ baseFolder };

		shader->setStrings( sources.ptr(), int(sources.Count()) );
		shader->setEntryPoint( entry_point );
		shader->setEnvInput( sh_source, stage, client, version );
		shader->setEnvClient( client, client_version );
		shader->setEnvTarget( target, target_version );
		shader->getIntermediate()->addRequestedExtension( "GL_GOOGLE_include_directive" );
		shader->getIntermediate()->addRequestedExtension( "GL_GOOGLE_cpp_style_line_directive" );

		if ( not shader->parse( &resources, GLSL_VERSION, false, messages, includer ) )
		{
			log << shader->getInfoLog();
			_OnCompilationFailed( shaderData.type, cfg.source, shaderData.src, includer, INOUT log );
			return false;
		}
		
		result.prog.addShader( shader.ptr() );

		if ( not result.prog.link( messages ) )
		{
			log << result.prog.getInfoLog();
			_OnCompilationFailed( shaderData.type, cfg.source, shaderData.src, includer, INOUT log );
			return false;
		}
		return true;
	}
	
/*
=================================================
	Translate
=================================================
*/
	bool ShaderCompiler::Translate (EShader::type shaderType, ArrayCRef<StringCRef> source, StringCRef entryPoint, StringCRef baseFolder,
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
			//case EShaderDstFormat::GLSL_VulkanSource :
			//case EShaderDstFormat::GLSL_ES_Source :
			{
				switch ( cfg.source )
				{
					case EShaderSrcFormat::GLSL :
					//case EShaderSrcFormat::GLSL_ES_2 :
					//case EShaderSrcFormat::GLSL_ES_3 :
					{
						ASSERT( not cfg.optimize and "not supported" );

						if ( cfg.skipExternals or cfg.typeReplacer )
							;	// go to 'case GXSL'
						else
							return _CopySource( data, OUT result );
					}
					case EShaderSrcFormat::GXSL :
					{
						_GLSLangResult	glslang_data;

						CHECK_COMP( _GLSLangParse( cfg, data, baseFolder, OUT log, OUT glslang_data ) );
						CHECK_COMP( _ReplaceTypes( glslang_data, cfg ) );
						CHECK_COMP( _TranslateGXSLtoGLSL( cfg, glslang_data, OUT log, OUT result ) );
						return true;
					}
					//case EShaderSrcFormat::GLSL_Vulkan :
					case EShaderSrcFormat::GXSL_Vulkan :
					{
						_GLSLangResult	glslang_data;
						Config			cfg2 = cfg;
						
						cfg2.target = EShaderDstFormat::SPIRV_Source;
						CHECK_COMP( _GLSLangParse( cfg2, data, baseFolder, OUT log, OUT glslang_data ) );
						CHECK_COMP( _ReplaceTypes( glslang_data, cfg2 ) );
						
						cfg2.target = cfg.target;
						CHECK_COMP( _TranslateGXSLtoGLSL( cfg2, glslang_data, OUT log, OUT result ) );
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
				CHECK_COMP( Translate( shaderType, source, entryPoint, baseFolder, cfg2, OUT log, OUT temp ) );

				_ShaderData	data2 = data;
				data2.src.Clear();
				data2.src << StringCRef::From( temp );
				data2.entry = "main";

				cfg2		= cfg;
				cfg2.source	= (cfg.target == EShaderDstFormat::GLSL_Binary ? EShaderSrcFormat::GLSL : EShaderSrcFormat::GLSL_ES_3);

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
				CHECK_COMP( Translate( shaderType, source, entryPoint, baseFolder, cfg2, OUT log, OUT temp ) );
				
				_ShaderData	data2 = data;
				data2.src.Clear();
				data2.src << StringCRef::From( temp );
				data2.entry = "main";

				cfg2		= cfg;
				cfg2.source	= EShaderSrcFormat::GLSL;

				_GLSLangResult	glslang_data;
				CHECK_COMP( _GLSLangParse( cfg2, data2, baseFolder, OUT log, OUT glslang_data ) );
				CHECK_COMP( _ReplaceTypes( glslang_data, cfg2 ) );

				cfg2.typeReplacer = Uninitialized;
				CHECK_COMP( _CompileSPIRV( cfg2, glslang_data, OUT log, OUT result ) );

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
						cfg2.skipExternals	= false;
						cfg2.target			= EShaderDstFormat::GLSL_Source;
						cfg2.typeReplacer	= Uninitialized;

						CHECK_COMP( Translate( shaderType, source, entryPoint, baseFolder, cfg2, OUT log, OUT temp ) );

						data2.src.Clear();
						data2.src << StringCRef::From( temp );
						data2.entry = "main";

						cfg2				= cfg;
						cfg2.source			= EShaderSrcFormat::GLSL;
						cfg2.typeReplacer	= cfg.typeReplacer;
					}
				}

				_GLSLangResult	glslang_data;
				CHECK_COMP( _GLSLangParse( cfg2, data2, baseFolder, OUT log, OUT glslang_data ) );
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

				CHECK_COMP( Translate( shaderType, source, entryPoint, baseFolder, cfg2, OUT log, OUT temp ) );

				data2.src.Clear();
				data2.src << StringCRef::From( temp );
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
						cfg2.skipExternals	= false;
						cfg2.target			= EShaderDstFormat::GLSL_Source;
						cfg2.typeReplacer	= Uninitialized;

						CHECK_COMP( Translate( shaderType, source, entryPoint, baseFolder, cfg2, OUT log, OUT temp ) );

						data2.src.Clear();
						data2.src << StringCRef::From( temp );
						data2.entry = "main";

						cfg2				= cfg;
						cfg2.source			= EShaderSrcFormat::GLSL;
						cfg2.typeReplacer	= cfg.typeReplacer;
					}
				}
				
				_GLSLangResult	glslang_data;
				CHECK_COMP( _GLSLangParse( cfg2, data2, baseFolder, OUT log, OUT glslang_data ) );
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
						cfg2.skipExternals	= false;
						cfg2.target			= EShaderDstFormat::GLSL_Source;
						cfg2.typeReplacer	= Uninitialized;

						CHECK_COMP( Translate( shaderType, source, entryPoint, baseFolder, cfg2, OUT log, OUT temp ) );

						data2.src.Clear();
						data2.src << StringCRef::From( temp );
						data2.entry = "main";

						cfg2				= cfg;
						cfg2.source			= EShaderSrcFormat::GLSL;
						cfg2.typeReplacer	= cfg.typeReplacer;
					}
				}
				
				_GLSLangResult	glslang_data;
				CHECK_COMP( _GLSLangParse( cfg2, data2, baseFolder, OUT log, OUT glslang_data ) );
				CHECK_COMP( _ReplaceTypes( glslang_data, cfg2 ) );
				CHECK_COMP( _TranslateGXSLtoHLSL( cfg2, glslang_data, OUT log, OUT result ) );
				return true;
			}

			case EShaderDstFormat::HLSL_Binary :
			{
				_ShaderData	data2 = data;
				Config		cfg2  = cfg;
				BinaryArray	temp;
				
				switch ( cfg.source )
				{
					case EShaderSrcFormat::HLSL :
						break;

					default :
					{
						cfg2.skipExternals	= false;
						cfg2.target			= EShaderDstFormat::HLSL_Source;

						CHECK_COMP( Translate( shaderType, source, entryPoint, baseFolder, cfg2, OUT log, OUT temp ) );

						data2.src.Clear();
						data2.src << StringCRef::From( temp );
						data2.entry = "main";

						cfg2				= cfg;
						cfg2.source			= EShaderSrcFormat::HLSL;
						cfg2.typeReplacer	= Uninitialized;
					}
				}

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
	
/*
=================================================
	Validate
=================================================
*/
	bool ShaderCompiler::Validate (EShaderDstFormat::type shaderFmt, EShader::type shaderType, BinArrayCRef data)
	{
		switch ( shaderFmt )
		{
			case EShaderDstFormat::GLSL_Source :	return _ValidateGLSLSource( shaderType, StringCRef::From( data ) );
			case EShaderDstFormat::GLSL_Binary :	return _ValidateGLSLBinary( shaderType, data );
			case EShaderDstFormat::CL_Source :		return _ValidateCLSource( shaderType, StringCRef::From( data ) );
			case EShaderDstFormat::CL_Binary :		return _ValidateCLBinary( shaderType, data );
			case EShaderDstFormat::HLSL_Source :	return _ValidateHLSLSource( shaderType, StringCRef::From( data ) );
			case EShaderDstFormat::HLSL_Binary :	return _ValidateHLSLBinary( shaderType, data );
			case EShaderDstFormat::SPIRV_Binary :	return _ValidateSPIRV( shaderType, data );
		}
		return true;
	}

}	// PipelineCompiler
