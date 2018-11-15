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
	bool ShaderCompiler::_OnCompilationFailed (EShader::type, EShaderFormat::type, ArrayCRef<StringCRef> source,
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
		using namespace glslang;

		EShClient					client			= EShClientOpenGL;
		EshTargetClientVersion		client_version	= EShTargetOpenGL_450;

		EShTargetLanguage			target			= EShTargetNone;
		EShTargetLanguageVersion	target_version	= EShTargetLanguageVersion(0);
		
		uint						version			= 0;
		uint						sh_version		= 450;		// TODO
		EProfile					sh_profile		= ENoProfile;
		EShSource					sh_source;

		switch ( EShaderFormat::GetAPI( cfg.source ) )
		{
			case EShaderFormat::OpenGL :
				sh_source		= EShSourceGlsl;
				version			= EShaderFormat::GetVersion( cfg.source );
				sh_profile		= version >= 330 ? ECoreProfile : ENoProfile;
				break;

			case EShaderFormat::OpenGLES :
				sh_source		= EShSourceGlsl;
				version			= EShaderFormat::GetVersion( cfg.source );
				sh_profile		= EEsProfile;
				break;

			case EShaderFormat::DirectX :
				sh_source		= EShSourceHlsl;
				version			= EShaderFormat::GetVersion( cfg.source );
				sh_profile		= ENoProfile;	// TODO
				break;

			case EShaderFormat::GX_API :
				sh_source		= EShSourceGxsl;
				version			= EShaderFormat::GetVersion( cfg.source );
				sh_profile		= ECoreProfile;
				break;

			case EShaderFormat::Vulkan :
				sh_source		= EShSourceGlsl;
				version			= EShaderFormat::GetVersion( cfg.source );
				sh_profile		= ECoreProfile;
				break;

			default :
				RETURN_ERR( "unsupported shader format" );
		}

		switch ( EShaderFormat::GetAPI( cfg.target ) )
		{
			case EShaderFormat::Vulkan :
			case EShaderFormat::GX_API :
			{
				version			= EShaderFormat::GetAPI( cfg.source ) == EShaderFormat::GX_API ?
									SPIRV_VERSION :
									EShaderFormat::GetVersion( cfg.target );
				client			= EShClientVulkan;
				client_version	= (version == 110 ? EShTargetVulkan_1_1 : EShTargetVulkan_1_0);
				target			= EshTargetSpv;
				target_version	= (version == 110 ? EShTargetSpv_1_3 : EShTargetSpv_1_0);
				break;
			}

			case EShaderFormat::OpenGL :
				if ( EShaderFormat::GetFormat( cfg.target ) == EShaderFormat::SPIRV or
					 EShaderFormat::GetFormat( cfg.target ) == EShaderFormat::Assembler )
				{
					target			= EshTargetSpv;
					target_version	= EShTargetSpv_1_0;
				}
				break;
		}

		Array<const char*>	sources;
		EShMessages			messages	= EShMsgDefault;
		TBuiltInResource	resources;	_GenerateResources( OUT resources );
		EShLanguage			stage		= ConvertShaderType( shaderData.type );
		auto&				shader		= result.shader;

		for (auto& src : shaderData.src) {
			sources << (src.Empty() ? "" : src.cstr());
		}

		// parse shader
		const char *	entry_point	= shaderData.entry.Empty() ? "main" : shaderData.entry.cstr();
		ShaderIncluder	includer	{ baseFolder };

		shader = new TShader( stage );
		shader->setStrings( sources.ptr(), int(sources.Count()) );
		shader->setEntryPoint( entry_point );
		shader->setEnvInput( sh_source, stage, client, version );
		shader->setEnvClient( client, client_version );
		shader->setEnvTarget( target, target_version );
		
		shader->setAutoMapLocations( true );
		shader->setAutoMapBindings( true );

		// for '#include' in shader
		shader->getIntermediate()->addRequestedExtension( "GL_GOOGLE_include_directive" );
		shader->getIntermediate()->addRequestedExtension( "GL_GOOGLE_cpp_style_line_directive" );

		// for vulkan compatibility
		shader->getIntermediate()->addRequestedExtension( "GL_ARB_separate_shader_objects" );
		shader->getIntermediate()->addRequestedExtension( "GL_ARB_shading_language_420pack" );

		if ( not shader->parse( &resources, sh_version, sh_profile, false, true, messages, includer ) )
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
		CHECK_ERR( EShaderFormat::IsValid( cfg.source ) );
		CHECK_ERR( EShaderFormat::IsValid( cfg.target ) );

		log.Clear();
		result.Clear();

		_ShaderData	data;
		data.entry	= entryPoint;
		data.src	= source;
		data.type	= shaderType;

		switch ( EShaderFormat::GetApiFormat( cfg.target ) )
		{
			case EShaderFormat::ESSL :
			case EShaderFormat::ESSL_Bin :
			case EShaderFormat::GLSL :
			case EShaderFormat::GLSL_Bin :
			case EShaderFormat::GXSL :
			case EShaderFormat::VKSL :
			{
				CHECK_ERR( _TranslateToGLSL( data, baseFolder, cfg, OUT log, OUT result ) );
				break;
			}

			case EShaderFormat::VK_SPIRV :
			case EShaderFormat::VK_SPIRV_Asm :
			case EShaderFormat::GL_SPIRV :
			case EShaderFormat::GL_SPIRV_Asm :
			case EShaderFormat::ES_SPIRV :
			case EShaderFormat::ES_SPIRV_Asm :
			case EShaderFormat::CL_SPIRV :
			{
				CHECK_ERR( _TranslateToSPV( data, baseFolder, cfg, OUT log, OUT result ) );
				break;
			}
				
			case EShaderFormat::CL_Src :
			case EShaderFormat::CL_Asm :
			{
				CHECK_ERR( _TranslateToCL( data, baseFolder, cfg, OUT log, OUT result ) );
				break;
			}

			case EShaderFormat::Software | EShaderFormat::CPP_Invocable :
			{
				CHECK_ERR( _TranslateToCPP( data, baseFolder, cfg, OUT log, OUT result ) );
				break;
			}

			case EShaderFormat::HLSL :
			case EShaderFormat::HLSL_BC :
			case EShaderFormat::HLSL_IL :
			{
				CHECK_ERR( _TranslateToHLSL( data, baseFolder, cfg, OUT log, OUT result ) );
				break;
			}

			case EShaderFormat::Metal :
			case EShaderFormat::CUDA :
			default :
				RETURN_ERR( "unsupported shader compilation target!" );
		}

		return true;
	}
	
/*
=================================================
	_TranslateToGLSL
=================================================
*/
	bool ShaderCompiler::_TranslateToGLSL (const _ShaderData &data, StringCRef baseFolder, const Config &cfg, OUT String &log, OUT BinaryArray &result)
	{
		log << "Translate " << EShaderFormat::ToString( cfg.source ) << " to " << EShaderFormat::ToString( cfg.target ) << '\n';

		switch ( EShaderFormat::GetApiFormat( cfg.target ) )
		{
			case EShaderFormat::ESSL :
			case EShaderFormat::GLSL :
			case EShaderFormat::VKSL :
			{
				switch ( EShaderFormat::GetApiFormat( cfg.source ) )
				{
					case EShaderFormat::ESSL :
					case EShaderFormat::GLSL :
					case EShaderFormat::VKSL :
					{
						ASSERT( not cfg.optimize and "not supported" );

						if ( cfg.skipExternals or cfg.typeReplacer )
							;	// go to 'case GXSL'
						else
							return _CopySource( data, OUT result );
					}
					case EShaderFormat::GXSL :
					{
						_GLSLangResult	glslang_data;

						CHECK_COMP( _GLSLangParse( cfg, data, baseFolder, OUT log, OUT glslang_data ) );
						CHECK_COMP( _ReplaceTypes( glslang_data, cfg ) );
						CHECK_COMP( _TranslateGXSLtoGLSL( cfg, glslang_data, OUT log, OUT result ) );
						return true;
					}
					default :
						RETURN_ERR( "unsupported combination!" );
				}
			}

			case EShaderFormat::GLSL_Bin :
			case EShaderFormat::ESSL_Bin :
			{
				Config		cfg2	= cfg;
				cfg2.skipExternals	= false;
				cfg2.target			= EShaderFormat::IntermediateSrc;

				BinaryArray	temp;
				CHECK_COMP( _TranslateToGLSL( data, baseFolder, cfg2, OUT log, OUT temp ) );

				_ShaderData	data2 = data;
				data2.src.Clear();
				data2.src << StringCRef::From( temp );
				data2.entry = "main";

				cfg2		= cfg;
				cfg2.source	= EShaderFormat::IntermediateSrc;

				CHECK_COMP( _CompileGLSL( cfg2, data2, OUT log, OUT result ) );
				return true;
			}
		}

		RETURN_ERR( "not supported" );
	}
		
/*
=================================================
	_TranslateToHLSL
=================================================
*/
	bool ShaderCompiler::_TranslateToHLSL (const _ShaderData &data, StringCRef baseFolder, const Config &cfg, OUT String &log, OUT BinaryArray &result)
	{
		log << "Translate " << EShaderFormat::ToString( cfg.source ) << " to " << EShaderFormat::ToString( cfg.target ) << '\n';

		switch ( EShaderFormat::GetApiFormat( cfg.target ) )
		{
			case EShaderFormat::HLSL :
			{
				Config		cfg2	= cfg;
				_ShaderData	data2	= data;
				BinaryArray	temp;

				switch ( EShaderFormat::GetApiFormat( cfg.source ) )
				{
					case EShaderFormat::HLSL :
						CHECK_COMP( not cfg.typeReplacer );
						return _CopySource( data, OUT result );

					case EShaderFormat::GXSL :
					case EShaderFormat::GLSL :
						break;

					// try to translate from any language to GLSL
					default :
					{
						cfg2.skipExternals	= false;
						cfg2.target			= EShaderFormat::IntermediateSrc;
						cfg2.typeReplacer	= Uninitialized;

						CHECK_COMP( _TranslateToGLSL( data, baseFolder, cfg2, OUT log, OUT temp ) );

						data2.src.Clear();
						data2.src << StringCRef::From( temp );
						data2.entry = "main";

						cfg2				= cfg;
						cfg2.source			= EShaderFormat::IntermediateSrc;
						cfg2.typeReplacer	= cfg.typeReplacer;
					}
				}
				
				_GLSLangResult	glslang_data;
				CHECK_COMP( _GLSLangParse( cfg2, data2, baseFolder, OUT log, OUT glslang_data ) );
				CHECK_COMP( _ReplaceTypes( glslang_data, cfg2 ) );
				CHECK_COMP( _TranslateGXSLtoHLSL( cfg2, glslang_data, OUT log, OUT result ) );
				return true;
			}

			case EShaderFormat::HLSL_BC :
			case EShaderFormat::HLSL_IL :	// TODO
			{
				_ShaderData	data2 = data;
				Config		cfg2  = cfg;
				BinaryArray	temp;
				
				if ( cfg.source == EShaderFormat::HLSL_11_BC )
				{
					CHECK_COMP( not cfg.typeReplacer );
					return _CopySource( data, OUT result );
				}
				else
				if ( cfg.source == EShaderFormat::HLSL_11 )
				{
					// skip
				}
				else
				// try to translate from any language to HLSL
				{
					cfg2.skipExternals	= false;
					cfg2.target			= EShaderFormat::HLSL_11;

					CHECK_COMP( _TranslateToHLSL( data, baseFolder, cfg2, OUT log, OUT temp ) );

					data2.src.Clear();
					data2.src << StringCRef::From( temp );
					data2.entry = "main";

					cfg2				= cfg;
					cfg2.source			= EShaderFormat::HLSL_11;
					cfg2.typeReplacer	= Uninitialized;
				}

				CHECK_COMP( _CompileHLSL( cfg2, data2, OUT log, OUT result ) );
				return true;
			}
		}

		RETURN_ERR( "not supported" );
	}
		
/*
=================================================
	_TranslateToSPV
=================================================
*/
	bool ShaderCompiler::_TranslateToSPV (const _ShaderData &data, StringCRef baseFolder, const Config &cfg, OUT String &log, OUT BinaryArray &result)
	{
		log << "Translate " << EShaderFormat::ToString( cfg.source ) << " to " << EShaderFormat::ToString( cfg.target ) << '\n';

		switch ( EShaderFormat::GetApiFormat( cfg.target ) )
		{
			case EShaderFormat::VK_SPIRV :
			case EShaderFormat::GL_SPIRV :
			{
				Config		cfg2	= cfg;
				_ShaderData	data2	= data;
				BinaryArray	temp;
				
				switch ( EShaderFormat::GetApiFormat( cfg.source ) )
				{
					case EShaderFormat::VK_SPIRV :
					case EShaderFormat::GL_SPIRV :
						CHECK_COMP( not cfg.typeReplacer );
						return _CopySource( data, OUT result );

					case EShaderFormat::VK_SPIRV_Asm :
					case EShaderFormat::GL_SPIRV_Asm :
						CHECK_COMP( not cfg.typeReplacer );
						CHECK_COMP( data.src.Count() == 1 );
						return _CompileSPIRVAsm( cfg, data.src.Front(), OUT log, OUT result );

					case EShaderFormat::HLSL :
						RETURN_ERR( "not supported, yet" );

					case EShaderFormat::GLSL :
					case EShaderFormat::VKSL :
					case EShaderFormat::GXSL :
						break;

					/*case EShaderFormat::GXSL :
					{
						cfg2.optimize		= false;
						cfg2.skipExternals	= false;
						cfg2.target			= EShaderFormat::GLSL_450;
						cfg2.typeReplacer	= Uninitialized;

						CHECK_COMP( _TranslateToGLSL( data, baseFolder, cfg2, OUT log, OUT temp ) );

						data2 = data;
						data2.src.Clear();
						data2.src << StringCRef::From( temp );
						data2.entry = "main";

						cfg2		= cfg;
						cfg2.source	= (cfg.source == EShaderFormat::GXSL_Vulkan ? EShaderFormat::GLSL_Vulkan : EShaderFormat::GLSL);
						break;
					}*/

					default :
						RETURN_ERR( "not supported" );
				}
				
				_GLSLangResult	glslang_data;
				CHECK_COMP( _GLSLangParse( cfg2, data2, baseFolder, OUT log, OUT glslang_data ) );
				CHECK_COMP( _ReplaceTypes( glslang_data, cfg2 ) );

				cfg2.typeReplacer = Uninitialized;
				CHECK_COMP( _CompileGLSLtoSPIRV( cfg2, glslang_data, OUT log, OUT result ) );

				return true;
			}
			
			case EShaderFormat::VK_SPIRV_Asm :
			case EShaderFormat::GL_SPIRV_Asm :
			{
				switch ( EShaderFormat::GetApiFormat( cfg.source ) )
				{
					case EShaderFormat::VK_SPIRV_Asm :
					case EShaderFormat::GL_SPIRV_Asm :
						CHECK_COMP( not cfg.typeReplacer );
						return _CopySource( data, OUT result );
					
					case EShaderFormat::VK_SPIRV :
					case EShaderFormat::GL_SPIRV :
						CHECK_COMP( not cfg.typeReplacer );
						return _DisasambleSPIRV( cfg, ArrayCRef<uint>::From( data.src.Front() ), OUT log, OUT result );
					
					case EShaderFormat::GLSL :
					case EShaderFormat::GXSL :
					case EShaderFormat::VKSL :
					{
						Config		cfg2	= cfg;
						BinaryArray	temp;

						cfg2.target = EShaderFormat::GetApiVersion( cfg.target ) | EShaderFormat::SPIRV;

						CHECK_ERR( _TranslateToSPV( data, baseFolder, cfg2, OUT log, OUT temp ) );
						
						cfg2.typeReplacer	= Uninitialized;
						cfg2.source			= cfg2.target;
						cfg2.target			= cfg.target;
						return _DisasambleSPIRV( cfg2, ArrayCRef<uint>::From( temp ), OUT log, OUT result );
					}

					default :
						RETURN_ERR( "not supported" );
				}
			}
		}

		RETURN_ERR( "not supported" );
	}
		
/*
=================================================
	_TranslateToCL
=================================================
*/
	bool ShaderCompiler::_TranslateToCL (const _ShaderData &data, StringCRef baseFolder, const Config &cfg, OUT String &log, OUT BinaryArray &result)
	{
		log << "Translate " << EShaderFormat::ToString( cfg.source ) << " to " << EShaderFormat::ToString( cfg.target ) << '\n';

		switch ( EShaderFormat::GetApiFormat( cfg.target ) )
		{
			case EShaderFormat::CL_Src :
			{
				Config		cfg2	= cfg;
				_ShaderData	data2	= data;
				BinaryArray	temp;
				
				switch ( EShaderFormat::GetApiFormat( cfg.source ) )
				{
					case EShaderFormat::CL_Src :
						CHECK_COMP( not cfg.typeReplacer );
						return _CopySource( data, OUT result );

					case EShaderFormat::GLSL :
					case EShaderFormat::GXSL :
					case EShaderFormat::VKSL :
						break;

					default :
					{
						cfg2.skipExternals	= false;
						cfg2.target			= EShaderFormat::IntermediateSrc;
						cfg2.typeReplacer	= Uninitialized;

						CHECK_COMP( _TranslateToGLSL( data, baseFolder, cfg2, OUT log, OUT temp ) );

						data2.src.Clear();
						data2.src << StringCRef::From( temp );
						data2.entry = "main";

						cfg2				= cfg;
						cfg2.source			= EShaderFormat::IntermediateSrc;
						cfg2.typeReplacer	= cfg.typeReplacer;
					}
				}

				_GLSLangResult	glslang_data;
				CHECK_COMP( _GLSLangParse( cfg2, data2, baseFolder, OUT log, OUT glslang_data ) );
				CHECK_COMP( _ReplaceTypes( glslang_data, cfg2 ) );
				CHECK_COMP( _TranslateGXSLtoCL( cfg2, glslang_data, OUT log, OUT result ) );
				return true;
			}

			case EShaderFormat::CL_Asm :
			{
				BinaryArray	temp;
				_ShaderData	data2 = data;
				Config		cfg2  = cfg;

				cfg2.skipExternals	= false;
				cfg2.target			= EShaderFormat::CL_Src;

				CHECK_COMP( _TranslateToCL( data, baseFolder, cfg2, OUT log, OUT temp ) );

				data2.src.Clear();
				data2.src << StringCRef::From( temp );
				data2.entry = "main";

				cfg2				= cfg;
				cfg2.source			= EShaderFormat::CL_Src;
				cfg2.typeReplacer	= Uninitialized;

				CHECK_COMP( _CompileCL( cfg2, data2, OUT log, OUT result ) );
				return true;
			}
		}

		RETURN_ERR( "not supported" );
	}
	
/*
=================================================
	_TranslateToCPP
=================================================
*/
	bool ShaderCompiler::_TranslateToCPP (const _ShaderData &data, StringCRef baseFolder, const Config &cfg, OUT String &log, OUT BinaryArray &result)
	{
		log << "Translate " << EShaderFormat::ToString( cfg.source ) << " to " << EShaderFormat::ToString( cfg.target ) << '\n';

		CHECK_ERR( cfg.target == EShaderFormat::Soft_100_Exe );

		Config		cfg2	= cfg;
		_ShaderData	data2	= data;
		BinaryArray	temp;

		switch ( EShaderFormat::GetApiFormat( cfg.source ) )
		{
			case EShaderFormat::GLSL :
			case EShaderFormat::GXSL :
			case EShaderFormat::VKSL :
				break;

			default :
			{
				cfg2.skipExternals	= false;
				cfg2.target			= EShaderFormat::IntermediateSrc;
				cfg2.typeReplacer	= Uninitialized;

				CHECK_COMP( _TranslateToGLSL( data, baseFolder, cfg2, OUT log, OUT temp ) );

				data2.src.Clear();
				data2.src << StringCRef::From( temp );
				data2.entry = "main";

				cfg2				= cfg;
				cfg2.source			= EShaderFormat::IntermediateSrc;
				cfg2.typeReplacer	= cfg.typeReplacer;
			}
		}
				
		_GLSLangResult	glslang_data;
		CHECK_COMP( _GLSLangParse( cfg2, data2, baseFolder, OUT log, OUT glslang_data ) );
		CHECK_COMP( _ReplaceTypes( glslang_data, cfg2 ) );
		CHECK_COMP( _TranslateGXSLtoCPP( cfg2, glslang_data, OUT log, OUT result ) );
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
	bool ShaderCompiler::Validate (EShaderFormat::type shaderFmt, EShader::type shaderType, BinArrayCRef data)
	{
		switch ( EShaderFormat::GetApiFormat( shaderFmt ) )
		{
			case EShaderFormat::GLSL :			return _ValidateGLSLSource( shaderType, StringCRef::From( data ) );
			case EShaderFormat::GLSL_Bin :		return _ValidateGLSLBinary( shaderType, data );
			case EShaderFormat::CL_Src :		return _ValidateCLSource( shaderType, StringCRef::From( data ) );
			case EShaderFormat::CL_Asm :		return _ValidateCLBinary( shaderType, data );
			case EShaderFormat::HLSL :			return _ValidateHLSLSource( shaderType, StringCRef::From( data ) );
			case EShaderFormat::HLSL_BC :		return _ValidateHLSLBinary( shaderType, data );
			case EShaderFormat::VK_SPIRV_Asm :
			case EShaderFormat::GL_SPIRV_Asm :	return _ValidateSPIRV( shaderType, shaderFmt, data );
		}
		return true;
	}

}	// PipelineCompiler
