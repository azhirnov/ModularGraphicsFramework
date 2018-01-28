// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/LunarGLASS_Include.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"
#include "External/SDL2/include/SDL.h"

namespace PipelineCompiler
{

#if 0

	class GLApp final : public StaticRefCountedObject
	{
	// variables
	public:
		Ptr< Module >	ms;
		bool			looping	= true;
		bool			initialized = false;

	// methods
	public:
		GLApp ();

		void Initialize ();
		void Quit ();
		bool Update ();

		bool Compile (const _GLSLangCompiler &comp, const Config &cfg, OUT BinaryArray &result) const;

	private:
		bool _OnWindowClosed (const Message<ModuleMsg::WindowAfterDestroy> &);
		bool _Draw (const Message< ModuleMsg::Update > &msg);

		bool _GLInit (const Message< ModuleMsg::GpuDeviceCreated > &msg);
		bool _GLDelete (const Message< ModuleMsg::GpuDeviceBeforeDestory > &msg);
			
		bool _CompileShader (uint shader, EShader::type type, ArrayCRef<StringCRef> source) const;
		bool _LinkProgram (uint prog, ArrayCRef<uint> shaders, const Config &cfg) const;
		bool _GetProgramBinary (uint prog, OUT BinaryArray &result) const;
		void _DumpProgramResources (uint prog) const;
		void _DumpUniformBlocksInfo (uint prog, INOUT String &str) const;
		void _DumpBufferBlockInfo (uint prog, INOUT String &str) const;
		void _DumpUniformsInfo (uint prog, INOUT String &str) const;

	}	_app;

/*
=================================================
	constructor
=================================================
*/
	ShaderCompiler::GLApp::GLApp ()
	{
		ms = GetMainSystemInstance();

		Platforms::RegisterPlatforms();
	}
	
/*
=================================================
	Initialize
=================================================
*/
	void ShaderCompiler::GLApp::Initialize ()
	{
		ms->AddModule( WinPlatformModuleID, CreateInfo::Platform() );
		ms->AddModule( GLContextModuleID, CreateInfo::GpuContext() );

		auto	thread	= ms->GlobalSystems()->parallelThread;
	
		thread->AddModule( WinWindowModuleID, CreateInfo::Window() );
		thread->AddModule( GLThreadModuleID, CreateInfo::GpuThread() );

		auto	window		= thread->GetModule( WinWindowModuleID );
		auto	glthread	= thread->GetModule( GLThreadModuleID );

		window->Subscribe( this, &GLApp::_OnWindowClosed );
		glthread->Subscribe( this, &GLApp::_Draw );
		glthread->Subscribe( this, &GLApp::_GLInit );
		glthread->Subscribe( this, &GLApp::_GLDelete );

		window = null;
		thread = null;
		glthread = null;

		// finish initialization
		ms->Send( Message< ModuleMsg::Link >() );
		ms->Send( Message< ModuleMsg::Compose >() );
	}
	
/*
=================================================
	Quit
=================================================
*/
	void ShaderCompiler::GLApp::Quit ()
	{
		looping = false;
	}

/*
=================================================
	Update
=================================================
*/
	bool ShaderCompiler::GLApp::Update ()
	{
		ms->Send( Message< ModuleMsg::Update >() );
		return looping;
	}
	
/*
=================================================
	_OnWindowClosed
=================================================
*/
	bool ShaderCompiler::GLApp::_OnWindowClosed (const Message<ModuleMsg::WindowAfterDestroy> &)
	{
		looping = false;
		return true;
	}
	
/*
=================================================
	_Draw
=================================================
*/
	bool ShaderCompiler::GLApp::_Draw (const Message< ModuleMsg::Update > &msg)
	{
		using namespace gl;

		return true;
	}
	
/*
=================================================
	_GLInit
=================================================
*/
	bool ShaderCompiler::GLApp::_GLInit (const Message< ModuleMsg::GpuDeviceCreated > &msg)
	{
		using namespace gl;

		initialized = true;
		return true;
	}
	
/*
=================================================
	_GLDelete
=================================================
*/
	bool ShaderCompiler::GLApp::_GLDelete (const Message< ModuleMsg::GpuDeviceBeforeDestory > &msg)
	{
		using namespace gl;


		return true;
	}
	
/*
=================================================
	GL4Enum (EShader)
=================================================
*/
	enum GL4Shader {};
	inline GL4Shader GL4Enum (EShader::type value)
	{
		switch ( value )
		{
			case EShader::Vertex			:	return (GL4Shader) gl::GL_VERTEX_SHADER;
			case EShader::TessControl		:	return (GL4Shader) gl::GL_TESS_CONTROL_SHADER;
			case EShader::TessEvaluation	:	return (GL4Shader) gl::GL_TESS_EVALUATION_SHADER;
			case EShader::Geometry			:	return (GL4Shader) gl::GL_GEOMETRY_SHADER;
			case EShader::Fragment			:	return (GL4Shader) gl::GL_FRAGMENT_SHADER;
			case EShader::Compute			:	return (GL4Shader) gl::GL_COMPUTE_SHADER;
		}

		RETURN_ERR( "invalid shader type", GL4Shader() );
	}

/*
=================================================
	Compile
=================================================
*/
	bool ShaderCompiler::GLApp::Compile (const _GLSLangCompiler &comp, const Config &cfg, OUT BinaryArray &programBinary) const
	{
		using namespace gl;

		bool			result	= true;

		uint			prog	= 0;
		Array<uint>		shader_ids;
		shader_ids.Resize( comp.shaders.Count() );


		// compile shaders
		FOR( i, comp.shaders )
		{
			const auto&	sh = comp.shaders[i];

			GL_CALL( shader_ids[i] = glCreateShader( GL4Enum(sh.type) ) );

			result &= _CompileShader( shader_ids[i], sh.type, sh.src );
		}

		// link program
		if ( result ) {
			GL_CALL( prog = glCreateProgram() );

			result &= _LinkProgram( prog, shader_ids, cfg );
		}

		// dump resource info
		if ( result ) {
			_DumpProgramResources( prog );
		}

		// get compiled program
		if ( result ) {
			result &= _GetProgramBinary( prog, OUT programBinary );
		}


		// delete resources
		FOR( i, shader_ids ) {
			GL_CALL( glDeleteShader( shader_ids[i] ) );
		}

		GL_CALL( glDeleteProgram( prog ) );

		CHECK_ERR( result );
		return true;
	}
	
/*
=================================================
	_CompileShader
=================================================
*/
	bool ShaderCompiler::GLApp::_CompileShader (uint shader, EShader::type shaderType, ArrayCRef<StringCRef> sources) const
	{
		using namespace gl;

		CHECK_ERR( shader != 0 and not sources.Empty() );

		Array<const char *>	src;

		FOR( i, sources ) {
			src << sources[i].cstr();
		}

		GL_CALL( glShaderSource( shader, GLsizei(src.Count()), src.ptr(), null ) );
		GL_CALL( glCompileShader( shader ) );
		
		GLint	compile_status = 0;
		GL_CALL( glGetShaderiv( shader, GL_COMPILE_STATUS,  &compile_status ) );

		const bool	compiled = ( compile_status == GL_TRUE );

		GLint	log_size = 0;
		GL_CALL( glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &log_size ) );

		if ( log_size > 5 )
		{
			String	info;
			info.Reserve( log_size+1 );
			
			GL_CALL( glGetShaderInfoLog( shader, log_size, null, info.ptr() ) );
			info.CalculateLength();
			
			LOG( ("Shader compilation error: "_str << info).cstr(), compiled ? ELog::Debug : ELog::Error );
		}

		return compiled;
	}

/*
=================================================
	_LinkProgram
=================================================
*/
	bool ShaderCompiler::GLApp::_LinkProgram (uint prog, ArrayCRef<uint> shaders, const Config &cfg) const
	{
		using namespace gl;

		CHECK_ERR( prog != 0 and not shaders.Empty() );
		
		GL_CALL( glProgramParameteri( prog, GL_PROGRAM_SEPARABLE, GL_TRUE ) );
		GL_CALL( glProgramParameteri( prog, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE ) );

		FOR( i, shaders ) {
			GL_CALL( glAttachShader( prog, shaders[i] ) );
		}
		
		GL_CALL( glLinkProgram( prog ) );
		
		GLint	link_status = 0;
		GL_CALL( glGetProgramiv( prog, GL_LINK_STATUS, &link_status ) );
		
		const bool	linked = ( link_status == GL_TRUE );

		GLint	log_size = 0;
		GL_CALL( glGetProgramiv( prog, GL_INFO_LOG_LENGTH, &log_size ) );

		if ( log_size > 5 )
		{
			String	info;
			info.Reserve( log_size + 1 );

			GL_CALL( glGetProgramInfoLog( prog, log_size, null, info.ptr() ) );
			info.CalculateLength();
			
			LOG( ("Program compilation error: "_str << info).cstr(), linked ? ELog::Debug : ELog::Error );
		}
		
		FOR( i, shaders ) {
			GL_CALL( glDetachShader( prog, shaders[i] ) );
		}
		return linked;
	}
	
/*
=================================================
	_GetProgramBinary
=================================================
*/
	bool ShaderCompiler::GLApp::_GetProgramBinary (uint prog, OUT BinaryArray &result) const
	{
		using namespace gl;

		GLint		i_size = 0;
		GLsizei		s_size = 0;

		GLenum		format = 0;

		//Array<GLenum>	formats;

		// choose format
		/*{
			GL_CALL( glGetIntegerv( GL_NUM_PROGRAM_BINARY_FORMATS, OUT &i_size ) );
			CHECK_ERR( i_size > 0 );
			formats.Resize( i_size );

			GL_CALL( glGetIntegerv( GL_PROGRAM_BINARY_FORMATS, (GLint *)formats.ptr() ) );
		}*/

		// get binary
		{
			GL_CALL( glGetProgramiv( prog, GL_PROGRAM_BINARY_LENGTH, OUT &i_size ) );
			result.Resize( i_size );

			GL_CALL( glGetProgramBinary( prog, result.Count(), OUT &s_size, OUT &format, OUT result.ptr() ) );
			result.Resize( s_size );

			// first 4 bytes stores the program binary format
			result.Insert( BinArrayCRef::FromValue(format), 0 );
		}
		return true;
	}

/*
=================================================
	GLVariableTypeToStr
=================================================
*/
	static const char * GLVariableTypeToStr (gl::GLenum type)
	{
		using namespace gl;

		switch ( type )
		{
			case GL_BOOL :										return "bool";
			case GL_BOOL_VEC2 :									return "bool2";
			case GL_BOOL_VEC3 :									return "bool3";
			case GL_BOOL_VEC4 :									return "bool4";
			case GL_FLOAT :										return "float";
			case GL_FLOAT_VEC2 :								return "float2";
			case GL_FLOAT_VEC3 :								return "float3";
			case GL_FLOAT_VEC4 :								return "float4";
			case GL_FLOAT_MAT2 :								return "float2x2";
			case GL_FLOAT_MAT3 :								return "float3x3";
			case GL_FLOAT_MAT4 :								return "float4x4";
			case GL_FLOAT_MAT2x3 :								return "float2x3";
			case GL_FLOAT_MAT2x4 :								return "float2x4";
			case GL_FLOAT_MAT3x2 :								return "float3x2";
			case GL_FLOAT_MAT3x4 :								return "float3x4";
			case GL_FLOAT_MAT4x2 :								return "float4x2";
			case GL_FLOAT_MAT4x3 :								return "float4x3";
			case GL_DOUBLE :									return "double";
			case GL_DOUBLE_VEC2 :								return "double2";
			case GL_DOUBLE_VEC3 :								return "double3";
			case GL_DOUBLE_VEC4 :								return "double4";
			case GL_DOUBLE_MAT2 :								return "double2x2";
			case GL_DOUBLE_MAT3 :								return "double3x3";
			case GL_DOUBLE_MAT4 :								return "double4x4";
			case GL_DOUBLE_MAT2x3 :								return "double2x3";
			case GL_DOUBLE_MAT2x4 :								return "double2x4";
			case GL_DOUBLE_MAT3x2 :								return "double3x2";
			case GL_DOUBLE_MAT3x4 :								return "double3x4";
			case GL_DOUBLE_MAT4x2 :								return "double4x2";
			case GL_DOUBLE_MAT4x3 :								return "double4x3";
			case GL_INT :										return "int";
			case GL_INT_VEC2 :									return "int2";
			case GL_INT_VEC3 :									return "int3";
			case GL_INT_VEC4 :									return "int4";
			case GL_UNSIGNED_INT :								return "uint";
			case GL_UNSIGNED_INT_VEC2 :							return "uint2";
			case GL_UNSIGNED_INT_VEC3 :							return "uint3";
			case GL_UNSIGNED_INT_VEC4 :							return "uint4";
			case GL_SAMPLER_1D :								return "sampler1D";
			case GL_SAMPLER_2D :								return "sampler2D";
			case GL_SAMPLER_3D :								return "sampler3D";
			case GL_SAMPLER_CUBE :								return "samplerCube";
			case GL_SAMPLER_1D_SHADOW :							return "sampler1DShadow";
			case GL_SAMPLER_2D_SHADOW :							return "sampler2DShadow";
			case GL_SAMPLER_2D_MULTISAMPLE :					return "sampler2DMS";
			case GL_SAMPLER_2D_MULTISAMPLE_ARRAY :				return "sampler2DMSArray";
			//case GL_SAMPLER_CUBE_MAP_ARRAY_ARB :				return "";
			//case GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW_ARB :		return "";
			case GL_SAMPLER_1D_ARRAY :							return "sampler1DArray";
			case GL_SAMPLER_2D_ARRAY :							return "sampler2DArray";
			case GL_SAMPLER_1D_ARRAY_SHADOW :					return "sampler1DArrayShadow";
			case GL_SAMPLER_2D_ARRAY_SHADOW :					return "sampler2DArrayShadow";
			case GL_SAMPLER_CUBE_SHADOW :						return "samplerCubeShadow";
			case GL_SAMPLER_BUFFER :							return "samplerBuffer";
			case GL_SAMPLER_2D_RECT :							return "sampler2DRect";
			case GL_SAMPLER_2D_RECT_SHADOW :					return "sampler2DRectShadow";
			case GL_INT_SAMPLER_1D :							return "isampler1D";
			case GL_INT_SAMPLER_2D :							return "isampler2D";
			case GL_INT_SAMPLER_3D :							return "isampler3D";
			case GL_INT_SAMPLER_CUBE :							return "isamplerCube";
			case GL_INT_SAMPLER_1D_ARRAY :						return "isampler1DArray";
			case GL_INT_SAMPLER_2D_ARRAY :						return "isampler2DArray";
			case GL_INT_SAMPLER_BUFFER :						return "isamplerBuffer";
			case GL_INT_SAMPLER_2D_RECT :						return "isampler2DRect";
			//case GL_INT_SAMPLER_CUBE_MAP_ARRAY_ARB :			return "";
			case GL_INT_SAMPLER_2D_MULTISAMPLE :				return "isampler2DMS";
			case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY :			return "isampler2DMSArray";
			case GL_UNSIGNED_INT_SAMPLER_1D :					return "usampler1D";
			case GL_UNSIGNED_INT_SAMPLER_2D :					return "usampler2D";
			case GL_UNSIGNED_INT_SAMPLER_3D :					return "usampler3D";
			case GL_UNSIGNED_INT_SAMPLER_CUBE :					return "usamplerCube";
			case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY :				return "usampler1DArray";
			case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY :				return "usampler2DArray";
			case GL_UNSIGNED_INT_SAMPLER_BUFFER :				return "usamplerBuffer";
			case GL_UNSIGNED_INT_SAMPLER_2D_RECT :				return "usampler2DRect";
			//case GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_ARB :	return "";
			case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE :		return "usampler2DMS";
			case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY :	return "usampler2DMSArray";
			case GL_IMAGE_1D :									return "image1D";
			case GL_IMAGE_2D :									return "image2D";
			case GL_IMAGE_3D :									return "image3D";
			case GL_IMAGE_2D_RECT :								return "image2DRect";
			case GL_IMAGE_CUBE :								return "imageCube";
			case GL_IMAGE_BUFFER :								return "imageBuffer";
			case GL_IMAGE_1D_ARRAY :							return "image1DArray";
			case GL_IMAGE_2D_ARRAY :							return "image2DArray";
			case GL_IMAGE_CUBE_MAP_ARRAY :						return "imageCubeArray";
			case GL_IMAGE_2D_MULTISAMPLE :						return "image2DMS";
			case GL_IMAGE_2D_MULTISAMPLE_ARRAY :				return "image2DMSArray";
			case GL_INT_IMAGE_1D :								return "iimage1D";
			case GL_INT_IMAGE_2D :								return "iimage2D";
			case GL_INT_IMAGE_3D :								return "iimage3D";
			case GL_INT_IMAGE_2D_RECT :							return "iimage2DRect";
			case GL_INT_IMAGE_CUBE :							return "iimageCube";
			case GL_INT_IMAGE_BUFFER :							return "iimageBuffer";
			case GL_INT_IMAGE_1D_ARRAY :						return "iimage1DArray";
			case GL_INT_IMAGE_2D_ARRAY :						return "iimage2DArray";
			case GL_INT_IMAGE_CUBE_MAP_ARRAY :					return "iimageCubeArray";
			case GL_INT_IMAGE_2D_MULTISAMPLE :					return "iimage2DMS";
			case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY :			return "iimage2DMSArray";
			case GL_UNSIGNED_INT_IMAGE_1D :						return "uimage1D";
			case GL_UNSIGNED_INT_IMAGE_2D :						return "uimage2D";
			case GL_UNSIGNED_INT_IMAGE_3D :						return "uimage3D";
			case GL_UNSIGNED_INT_IMAGE_2D_RECT :				return "uimage2DRect";
			case GL_UNSIGNED_INT_IMAGE_CUBE :					return "uimageCube";
			case GL_UNSIGNED_INT_IMAGE_BUFFER :					return "uimageBuffer";
			case GL_UNSIGNED_INT_IMAGE_1D_ARRAY :				return "uimage1DArray";
			case GL_UNSIGNED_INT_IMAGE_2D_ARRAY :				return "uimage2DArray";
			case GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY :			return "uimageCubeArray";
			case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE :			return "uimage2DMS";
			case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY :	return "uimage2DMSArray";
		}

		WARNING( "Unknown OpenGL type!" );
		return "<unknown>";
	}
	
/*
=================================================
	_DumpProgramResources
=================================================
*/
	void ShaderCompiler::GLApp::_DumpProgramResources (uint prog) const
	{
		CHECK( prog != 0 );

		String	str;	str.Reserve( 256 );

		str << "Program resources\n---------------\n";

		_DumpUniformBlocksInfo( prog, INOUT str );
		_DumpBufferBlockInfo( prog, INOUT str );
		_DumpUniformsInfo( prog, INOUT str );

		LOG( str.cstr(), ELog::Info | ELog::SpoilerFlag );
	}

/*
=================================================
	_DumpUniformBlocksInfo
=================================================
*/
	void ShaderCompiler::GLApp::_DumpUniformBlocksInfo (uint prog, INOUT String &str) const
	{
		using namespace gl;
		
		static const GLenum ubProperties[] = {
			GL_NUM_ACTIVE_VARIABLES,
			GL_BUFFER_BINDING,
			GL_BUFFER_DATA_SIZE,
		};
		static const GLenum varArrayProps[] = {
			GL_ACTIVE_VARIABLES
		};
		static const GLenum varProps[] = {
			GL_TYPE,
			GL_OFFSET,
			GL_ARRAY_SIZE,
			GL_ARRAY_STRIDE,
			GL_LOCATION
		};

		CHECK( prog != 0 );

		GLint			ub_count		= 0;
		GLint			max_ub_length	= 0;
		GLint			max_var_length	= 0;
		String			buf;
		GLsizei			length			= 0;
		GLint			index			= 0;
		Array<GLint>	var_indices;
		GLint			ub_params[ CountOf(ubProperties) ] = {};
		GLint			var_params[ CountOf(varProps) ]	   = {};

		GL_CALL( glGetProgramInterfaceiv( prog, GL_UNIFORM_BLOCK,  GL_ACTIVE_RESOURCES, &ub_count ) );
		GL_CALL( glGetProgramInterfaceiv( prog, GL_UNIFORM_BLOCK,  GL_MAX_NAME_LENGTH,  &max_ub_length ) );
		GL_CALL( glGetProgramInterfaceiv( prog, GL_UNIFORM,		GL_MAX_NAME_LENGTH,  &max_var_length ) );

		if ( ub_count <= 0 )
			return;

		buf.Reserve( Max( max_ub_length, max_var_length ) );
		
		for (int i = 0; i < ub_count; ++i)
		{
			GLuint	idx = (GLuint) i;

			GL_CALL( glGetProgramResourceName( prog, GL_UNIFORM_BLOCK, idx, max_ub_length, &length, buf.ptr() ) );
			buf.SetLength( length );

			GL_CALL( index = glGetProgramResourceIndex( prog, GL_UNIFORM_BLOCK, buf.cstr() ) );

			GL_CALL( glGetProgramResourceiv( prog, GL_UNIFORM_BLOCK, index, GLsizei(CountOf( ubProperties )),
											 ubProperties, GLsizei(CountOf( ub_params )), null, ub_params ) );

			GLint	ub_binding	= ub_params[1];
			GLint	ub_size		= ub_params[2];

			var_indices.Resize( ub_params[0] );
			GL_CALL( glGetProgramResourceiv( prog, GL_UNIFORM_BLOCK, index, GLsizei(CountOf( varArrayProps )), varArrayProps,
											 GLsizei(var_indices.Count()), null, var_indices.ptr() ) );
			
			str << "UBO     " << buf << ", index: " << index << ", binding: " << ub_binding
				<< ", size: " << ToString( BytesU( ub_size ) ) << '\n';

			FOR( j, var_indices )
			{
				GL_CALL( glGetProgramResourceiv( prog, GL_UNIFORM, var_indices[j], GLsizei(CountOf( varProps )), varProps,
												 GLsizei(CountOf( var_params )), null, var_params ) );

				GL_CALL( glGetProgramResourceName( prog, GL_UNIFORM, var_indices[j], max_var_length, &length, buf.ptr() ) );
				buf.SetLength( length );

				GLenum	var_type		= var_params[0];
				GLint	var_offset		= var_params[1];
				GLint	array_size		= var_params[2];
				//GLint	array_stride	= var_params[3];
				GLint	location		= var_params[4];

				str << "    " << GLVariableTypeToStr( var_type ) << "  " << buf;

				if ( array_size > 1 )
					str << '[' << array_size << ']';

				str << ", offset: " << var_offset << ", location: " << location << "\n";
			}
		}
	}
	
/*
=================================================
	_DumpBufferBlockInfo
=================================================
*/
	void ShaderCompiler::GLApp::_DumpBufferBlockInfo (uint prog, INOUT String &str) const
	{
		using namespace gl;

		static const GLenum ssbProperties[] = {
			GL_NUM_ACTIVE_VARIABLES,
			GL_BUFFER_BINDING,
			GL_BUFFER_DATA_SIZE,
		};
		static const GLenum varArrayProps[] = {
			GL_ACTIVE_VARIABLES
		};
		static const GLenum varProps[] = {
			GL_TYPE,
			GL_OFFSET,
			GL_ARRAY_SIZE,
			GL_ARRAY_STRIDE,
			GL_TOP_LEVEL_ARRAY_SIZE,
			GL_TOP_LEVEL_ARRAY_STRIDE
		};
		
		CHECK( prog != 0 );

		GLint			ssb_count		= 0,
						max_ssb_length	= 0,
						max_var_length	= 0;
		String			buf;
		GLsizei			length			= 0;
		GLint			index			= 0;
		Array<GLint>	var_indices;
		GLint			ssb_params[ CountOf(ssbProperties) ] = {};
		GLint			var_params[ CountOf(varProps) ]		 = {};

		GL_CALL( glGetProgramInterfaceiv( prog, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &ssb_count ) );
		GL_CALL( glGetProgramInterfaceiv( prog, GL_SHADER_STORAGE_BLOCK, GL_MAX_NAME_LENGTH,  &max_ssb_length ) );
		GL_CALL( glGetProgramInterfaceiv( prog, GL_BUFFER_VARIABLE,      GL_MAX_NAME_LENGTH,  &max_var_length ) );

		if ( ssb_count <= 0 )
			return;

		buf.Resize( Max( max_ssb_length, max_var_length ) );

		for (int i = 0; i < ssb_count; ++i)
		{
			GLuint	idx = (GLuint) i;

			GL_CALL( glGetProgramResourceName( prog, GL_SHADER_STORAGE_BLOCK, idx, max_ssb_length, &length, buf.ptr() ) );
			buf.SetLength( length );

			GL_CALL( index = glGetProgramResourceIndex( prog, GL_SHADER_STORAGE_BLOCK, buf.cstr() ) );

			GL_CALL( glGetProgramResourceiv( prog, GL_SHADER_STORAGE_BLOCK, index, GLsizei(CountOf( ssbProperties )),
											 ssbProperties, GLsizei(CountOf( ssb_params )), null, ssb_params ) );
			
			GLint	ssb_binding	= ssb_params[1];
			GLint	ssb_size	= ssb_params[2];

			var_indices.Resize( ssb_params[0] );
			GL_CALL( glGetProgramResourceiv( prog, GL_SHADER_STORAGE_BLOCK, index, GLsizei(CountOf( varArrayProps )), varArrayProps,
											 GLsizei(var_indices.Count()), null, var_indices.ptr() ) );

			str << "SSBO    " << buf << ", index: " << index << ", binding: " << ssb_binding
				<< ", size: " << ToString( BytesU( ssb_size ) ) << '\n';

			FOR( j, var_indices )
			{
				GL_CALL( glGetProgramResourceiv( prog, GL_BUFFER_VARIABLE, var_indices[j], GLsizei(CountOf( varProps )), varProps,
												 GLsizei(CountOf( var_params )), null, var_params ) );

				GL_CALL( glGetProgramResourceName( prog, GL_BUFFER_VARIABLE, var_indices[j], max_var_length, &length, buf.ptr() ) );
				buf.SetLength( length );

				GLenum	var_type		= var_params[0];
				GLint	var_offset		= var_params[1];
				GLint	array_size		= var_params[2];
				//GLint	array_stride	= var_params[3];
				//GLint	tl_arr_size		= var_params[4];
				GLint	tl_arr_stride	= var_params[5];

				str << "    " << GLVariableTypeToStr( var_type ) << "  " << buf;

				if ( array_size > 1 )
					str << '[' << array_size << ']';

				str << ", offset: " << var_offset << ", stride: " << tl_arr_stride << "\n";
			}
		}
	}

/*
=================================================
	_DumpUniformsInfo
=================================================
*/
	void ShaderCompiler::GLApp::_DumpUniformsInfo (uint prog, INOUT String &str) const
	{
		using namespace gl;

		static const GLenum varProps[] = {
			GL_TYPE,
			GL_LOCATION,
			GL_ARRAY_SIZE,
			GL_OFFSET,
			GL_BLOCK_INDEX
		};
		
		CHECK( prog != 0 );
		
		GLint	count			= 0,
				max_name_length = 0,
				length			= 0;
		GLuint	id				= prog;
		String	name;
		GLint	var_params[ CountOf(varProps) ] = {0};

		GL_CALL( glGetProgramInterfaceiv( id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &count ) );
		GL_CALL( glGetProgramInterfaceiv( id, GL_UNIFORM, GL_MAX_NAME_LENGTH, &max_name_length ) );
		
		if ( count <= 0 )
			return;

		name.Resize( max_name_length );
		
		for (int i = 0; i < count; ++i)
		{
			GL_CALL( glGetProgramResourceiv( id, GL_UNIFORM, GLuint(i), GLsizei(CountOf( varProps )), varProps,
											 GLsizei(CountOf( var_params )), &length, var_params ) );

			GL_CALL( glGetProgramResourceName( id, GL_UNIFORM, i, max_name_length, &length, name.ptr() ) );
			name.SetLength( length );

			const GLenum	type		= var_params[0];
			const GLint		location	= var_params[1];
			const GLuint	array_size	= var_params[2];
			//const GLint		offset		= var_params[3];
			const GLint		blockidx	= var_params[4];
			
			if ( blockidx != -1 )
				continue;

			str << "uniform " << GLVariableTypeToStr( type ) << "  " << name;

			if ( array_size > 1 )
				str << '[' << array_size << ']';

			str << ", location: " << location << '\n';
		}
	}

/*
=================================================
	_CompileGLBinary
=================================================
*/
	bool ShaderCompiler::_CompileGLBinary (const _GLSLangCompiler &comp, const Config &cfg, OUT BinaryArray &result) const
	{
		CHECK_ERR( _app.Compile( comp, cfg, OUT result ) );
		return true;
	}

#else
		
/*
=================================================
	_CompileGLSL
=================================================
*/
	bool ShaderCompiler::_CompileGLSL (const Config &cfg, const _ShaderData &shader, OUT String &log, OUT BinaryArray &result) const
	{
		TODO( "_CompileGLSL" );
		return false;
	}
	
/*
=================================================
	_ValidateGLSLSource
=================================================
*/
	bool ShaderCompiler::_ValidateGLSLSource (EShader::type shaderType, StringCRef src) const
	{
		TODO( "_ValidateGLSLBinary" );
		return false;
	}
	
/*
=================================================
	_ValidateGLSLBinary
=================================================
*/
	bool ShaderCompiler::_ValidateGLSLBinary (EShader::type shaderType, BinArrayCRef bin) const
	{
		TODO( "_ValidateGLSLBinary" );
		return false;
	}

#endif

}	// PipelineCompiler
