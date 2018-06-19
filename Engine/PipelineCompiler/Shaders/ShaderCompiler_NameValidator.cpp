// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler_NameValidator.h"

namespace PipelineCompiler
{

/*
=================================================
	ValidateLocalVar
=================================================
*/
	bool ShaderVarNameValidator::ValidateLocalVar (INOUT String &name, uint id, uint fnScopeId)
	{
		if ( _lastFnScopeId != fnScopeId )
		{
			_uniqueLocals.Clear();
			_usedLocalNames.Clear();
			_lastFnScopeId = fnScopeId;
		}

		UniqueNames_t::iterator	iter;

		if ( _uniqueLocals.Find( SymbolID(id), OUT iter ) )
		{
			name = iter->second;
			return true;
		}
		
		_ReplaceKeyword( INOUT name );

		if ( not _usedLocalNames.IsExist( name ) and not _usedGlobalNames.IsExist( name ) )
		{
			_uniqueLocals.Add( SymbolID(id), name );
			_usedLocalNames.Add( name );
			return true;
		}


		// check special cases
		SpecialCases_t::const_iterator	sc_iter;
		
		if ( _GetSpecialCases().Find( name, OUT sc_iter ) )
		{
			for (auto& candidate : sc_iter->second)
			{
				if ( not _usedLocalNames.IsExist( candidate ) and not _usedGlobalNames.IsExist( candidate ) )
				{
					name = candidate;
					_uniqueLocals.Add( SymbolID(id), name );
					_usedLocalNames.Add( name );
					return true;
				}
			}
		}


		// make unique
		//name << '_';
		const usize	len = name.Length();

		for (uint i = 1; i < 100; ++i)
		{
			name.SetLength( len );
			name << i;

			if ( not _usedLocalNames.IsExist( name ) and not _usedGlobalNames.IsExist( name ) )
				break;
		}

		_uniqueLocals.Add( SymbolID(id), name );
		_usedLocalNames.Add( name );
		return true;
	}
	
/*
=================================================
	ValidateGlobalVar
=================================================
*/
	bool ShaderVarNameValidator::ValidateGlobalVar (INOUT String &name, uint id)
	{
		UniqueNames_t::iterator	iter;

		if ( _uniqueGlobals.Find( SymbolID(id), OUT iter ) )
		{
			name = iter->second;
			return true;
		}

		_ReplaceKeyword( INOUT name );

		if ( not _usedGlobalNames.IsExist( name ) )
		{
			_uniqueGlobals.Add( SymbolID(id), name );
			_usedGlobalNames.Add( name );
			return true;
		}

		// make unique
		//name << '_';
		const usize	len = name.Length();

		for (uint i = 1; i < 100; ++i)
		{
			name.SetLength( len );
			name << i;

			if ( not _usedGlobalNames.IsExist( name ) )
				break;
		}

		_uniqueGlobals.Add( SymbolID(id), name );
		_usedGlobalNames.Add( name );
		return true;
	}
	
/*
=================================================
	ValidateField
=================================================
*/
	bool ShaderVarNameValidator::ValidateField (INOUT String &name)
	{
		_ReplaceKeyword( INOUT name );
		return true;
	}
	
/*
=================================================
	ValidateFunction
=================================================
*/
	bool ShaderVarNameValidator::ValidateFunction (StringCRef signature, OUT String &name)
	{
		name = signature;

		usize	pos  = 0;

		if ( name.Find( '(', OUT pos ) )
		{
			UniqueFuncs_t::iterator	iter;

			if ( _definedFuncs.Find( signature, OUT iter ) )
			{
				name = iter->second;
				return true;
			}
			
			name.SetLength( pos );


			// make unique name
			if ( not _funcOverloadingSupported )
			{
				for (usize i = 0; i < 1000; ++i)
				{
					name.SetLength( pos );

					if ( i > 0 )
						name << '_' << i;

					if ( not _existingFuncs.IsExist( name ) )
						break;
				}
			}
		}

		_ReplaceKeyword( INOUT name );
		_ReplaceBuiltinFunctions( INOUT name );
		
		_definedFuncs.Add( signature, name );
		_existingFuncs.Add( name );

		return true;
	}

/*
=================================================
	_ReplaceKeyword
=================================================
*/
	void ShaderVarNameValidator::_ReplaceKeyword (INOUT String &name) const
	{
		switch ( _dstShader )
		{
			case EShaderDstFormat::GLSL_Source :
				return _ReplaceGLSLKeyword( INOUT name );

			case EShaderDstFormat::CL_Source :
				return _ReplaceCLKeyword( INOUT name );

			case EShaderDstFormat::CPP_Module :
				return _ReplaceCPPKeyword( INOUT name );

			case EShaderDstFormat::HLSL_Source :
				return _ReplaceHLSLKeyword( INOUT name );
		}

		RETURN_ERR( "unsupported shader format", void() );
	}
	
/*
=================================================
	_ReplaceBuiltinFunctions
=================================================
*/
	void ShaderVarNameValidator::_ReplaceBuiltinFunctions (INOUT String &name) const
	{
		switch ( _dstShader )
		{
			case EShaderDstFormat::GLSL_Source :
				return _ReplaceGLSLBuiltinFn( INOUT name );

			case EShaderDstFormat::CL_Source :
				return _ReplaceCLBuiltinFn( INOUT name );

			case EShaderDstFormat::CPP_Module :
				return _ReplaceCPPBuiltinFn( INOUT name );

			case EShaderDstFormat::HLSL_Source :
				return _ReplaceHLSLBuiltinFn( INOUT name );
		}

		RETURN_ERR( "unsupported shader format", void() );
	}

/*
=================================================
	_ReplaceGLSLKeyword
=================================================
*/
	void ShaderVarNameValidator::_ReplaceGLSLKeyword (INOUT String &name)
	{
		if ( name.StartsWithIC( "gl_" ) )
		{
			name.Erase( 0, 3 );
			"gx_" >> name;
		}

		static const HashSet<String> reserved = {
			"const", "uniform", "layout", "centroid", "flat", "smooth", "break", "continue", "do", "for", "while",
			"switch", "case", "default", "if", "else", "in", "out", "inout", "float", "int", "void", "bool", "true",
			"false", "invariant", "discard", "return", "mat2", "mat3", "mat4", "mat2x2", "mat2x3", "mat2x4", "mat3x2",
			"mat3x3", "mat3x4", "mat4x2", "mat4x3", "mat4x4", "vec2", "vec3", "vec4", "ivec2", "ivec3", "ivec4",
			"bvec2", "bvec3", "bvec4", "uint", "uvec2", "uvec3", "uvec4", "lowp", "mediump", "highp", "precision",
			"sampler2D", "sampler3D", "samplerCube", "sampler2DShadow", "samplerCubeShadow", "sampler2DArray",
			"sampler2DArrayShadow", "isampler2D", "isampler3D", "isamplerCube", "isampler2DArray", "usampler2D",
			"usampler3D", "usamplerCube", "usampler2DArray", "struct", "attribute", "varying", "coherent", "volatile",
			"restrict", "readonly", "writeonly", "resource", "atomic_uint", "noperspective", "patch", "sample",
			"subroutine", "common", "partition", "active", "asm", "class", "union", "enum", "typedef", "template", "this",
			"goto", "inline", "noinline", "volatile", "public", "static", "extern", "external", "interface", "long",
			"short", "double", "half", "fixed", "unsigned", "superp", "input", "output", "hvec2", "hvec3", "hvec4",
			"dvec2", "dvec3", "dvec4", "fvec2", "fvec3", "fvec4", "sampler3DRect", "filter", "image1D", "image2D", "image3D",
			"imageCube", "iimage1D", "iimage2D", "iimage3D", "iimageCube", "uimage1D", "uimage2D", "uimage3D", "uimageCube",
			"image1DArray", "image2DArray", "iimage1DArray", "iimage2DArray", "uimage1DArray", "uimage2DArray", "imageBuffer",
			"iimageBuffer", "uimageBuffer", "sampler1D", "sampler1DShadow", "sampler1DArray", "sampler1DArrayShadow",
			"isampler1D", "isampler1DArray", "usampler1D", "usampler1DArray", "sampler2DRect", "sampler2DRectShadow",
			"isampler2DRect", "usampler2DRect", "samplerBuffer", "isamplerBuffer", "usamplerBuffer", "sampler2DMS",
			"isampler2DMS", "usampler2DMS", "sampler2DMSArray", "isampler2DMSArray", "usampler2DMSArray", "sizeof", "cast",
			"namespace", "using", "precise", "dmat2", "dmat3", "dmat4", "dmat2x2", "dmat2x3", "dmat2x4", "dmat3x2", "dmat3x3",
			"dmat3x4", "dmat4x2", "dmat4x3", "dmat4x4", "samplerCubeArray", "samplerCubeArrayShadow", "isamplerCubeArray",
			"usamplerCubeArray", "imageCubeArray", "iimageCubeArray", "uimageCubeArray"
		};

		if ( reserved.IsExist( name ) )
		{
			"rsvd_" >> name;
		}
	}
	
/*
=================================================
	_ReplaceGLSLBuiltinFn
=================================================
*/
	void ShaderVarNameValidator::_ReplaceGLSLBuiltinFn (INOUT String &name)
	{
	}

/*
=================================================
	_ReplaceCLKeyword
=================================================
*/
	void ShaderVarNameValidator::_ReplaceCLKeyword (INOUT String &name)
	{
		if ( name.StartsWithIC( "cl_" ) )
		{
			name.Erase( 0, 3 );
			"gx_" >> name;
		}
		
		static const HashSet<String> reserved = {
			"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern",
			"float", "for", "goto", "if", "inline", "int", "long", "register", "restrict", "return", "short", "signed",
			"sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "_Alignas",
			"_Alignof", "_Atomic", "_Bool", "_Complex", "_Generic", "_Imaginary", "_Noreturn", "_Static_assert","_Thread_local",
			"alignas", "alignof", "atomic_bool", "atomic_int", "bool", "complex", "imaginary", "noreturn", "static_assert",
			"thread_local", "asm", "fortran",
			"__global", "global", "__local", "local", "__constant", "constant", "__private", "private", "__kernel", "kernel",
			"__read_only", "read_only", "__write_only", "write_only", "__read_write", "read_write",
			"image2d_t", "image3d_t", "image2d_array_t", "image1d_t", "image1d_buffer_t", "image1d_array_t", "sampler_t", "event_t"
		};
		
		if ( reserved.IsExist( name ) )
		{
			"rsvd_" >> name;
			return;
		}

		static const Array<String> vector_types = {
			"char", "uchar", "short", "ushort", "int", "uint", "long", "ulong", "float", "double", "bool", "half", "quad"
		};

		const auto			IsNumber =	LAMBDA() (const char c) { return c >= '0' and c <= '9'; };
		Array<StringCRef>	tokens;

		for (auto& vtype : vector_types)
		{
			if ( name == vtype )
			{
				name = "type";
				return;
			}
			else
			if ( name.StartsWith( vtype ) )
			{
				// separate to words and numbers
				bool	is_number	= false;
				usize	pos			= vtype.Length();

				tokens << name.SubString( 0, vtype.Length() );

				for (usize i = pos; i < name.Length(); ++i)
				{
					const char	c		= vtype[i];
					const bool	prev	= is_number;

					is_number = IsNumber( c );

					if ( prev == is_number or pos == i )
						continue;

					tokens << name.SubString( pos, i - pos );
					pos = i;
				}
				
				tokens << name.SubString( pos, name.Length() - pos );

				// pattern 'typeN'
				if ( tokens.Count() == 2 and tokens[1].Length() > 0 and IsNumber( tokens[1].Front() ) )
				{
					name = "typeN";
					return;
				}

				// pattern 'typeNxM'
				if ( tokens.Count() == 4 and
					 tokens[1].Length() > 0 and IsNumber( tokens[1].Front() ) and
					 tokens[2] == "x" and
					 tokens[3].Length() > 0 and IsNumber( tokens[3].Front() ) )
				{
					name = "typeNxM";
					return;
				}
			}
		}
	}
	
/*
=================================================
	_ReplaceCLBuiltinFn
=================================================
*/
	void ShaderVarNameValidator::_ReplaceCLBuiltinFn (INOUT String &name)
	{
		if ( name == "main" )
			name = "Main";
	}

/*
=================================================
	_ReplaceCPPKeyword
=================================================
*/
	void ShaderVarNameValidator::_ReplaceCPPKeyword (INOUT String &name)
	{
		static const HashSet<String> reserved = {
			"alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit", "atomic_noexcept", "auto", "bitand",
			"bitor", "bool", "break", "case", "catch", "char", "char16_t", "char32_t", "class", "compl", "concept", "const",
			"constexpr", "const_cast", "continue", "co_await", "co_return", "co_yield", "decltype", "default", "delete", "do",
			"double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto",
			"if", "import", "inline", "int", "long", "module", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr",
			"operator", "or", "or_eq", "private", "protected", "public", "register", "reinterpret_cast", "requires", "return", "short",
			"signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch", "synchronized", "template", "this",
			"thread_local", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void",
			"volatile", "wchar_t", "while", "xor", "xor_eq", "override", "final", "transaction_safe", "transaction_safe_dynamic"
		};

		if ( reserved.IsExist( name ) )
		{
			"rsvd_" >> name;
		}
	}
	
/*
=================================================
	_ReplaceCPPBuiltinFn
=================================================
*/
	void ShaderVarNameValidator::_ReplaceCPPBuiltinFn (INOUT String &name)
	{
	}

/*
=================================================
	_ReplaceHLSLKeyword
=================================================
*/
	void ShaderVarNameValidator::_ReplaceHLSLKeyword (INOUT String &name)
	{
		static const HashSet<String> reserved = {
			"AppendStructuredBuffer", "asm", "asm_fragment", "BlendState", "bool", "bool1", "bool2", "bool3", "bool4", "bool1x1", "bool1x2",
			"bool1x3", "bool1x4", "bool2x1", "bool2x2", "bool2x3", "bool2x4", "bool3x1", "bool3x2", "bool3x3", "bool3x4", "bool4x1", "bool4x2",
			"bool4x3", "bool4x4", "break", "Buffer", "ByteAddressBuffer", "case", "cbuffer", "centroid", "class", "column_major", "compile",
			"compile_fragment", "CompileShader", "const", "continue", "ComputeShader", "ConsumeStructuredBuffer", "default", "DepthStencilState",
			"DepthStencilView", "discard", "do", "double", "DomainShader", "dword", "else", "export", "extern", "false", "float", "float1", "float2",
			"float3", "float4", "float1x1", "float1x2", "float1x3", "float1x4", "float2x1", "float2x2", "float2x3", "float2x4", "float3x1", "float3x2",
			"float3x3", "float3x4", "float4x1", "float4x2", "float4x3", "float4x4", "for", "fxgroup", "GeometryShader", "groupshared", "half", "Hullshader",
			"if", "in", "inline", "inout", "InputPatch", "int", "int1", "int2", "int3", "int4", "int1x1", "int1x2", "int1x3", "int1x4", "int2x1", "int2x2",
			"int2x3", "int2x4", "int3x1", "int3x2", "int3x3", "int3x4", "int4x1", "int4x2", "int4x3", "int4x4", "interface", "line", "lineadj", "linear",
			"LineStream", "matrix", "min16float", "min10float", "min10float1", "min10float2", "min10float3", "min10float4", "min10float1x1", "min10float1x2",
			"min10float1x3", "min10float1x4", "min10float2x1", "min10float2x2", "min10float2x3", "min10float2x4", "min10float3x1", "min10float3x2",
			"min10float3x3", "min10float3x4", "min10float4x1", "min10float4x2", "min10float4x3", "min10float4x4", "min16int", "min16int1", "min16int2",
			"min16int3", "min16int4", "min16int1x1", "min16int1x2", "min16int1x3", "min16int1x4", "min16int2x1", "min16int2x2", "min16int2x3", "min16int2x4",
			"min16int3x1", "min16int3x2", "min16int3x3", "min16int3x4", "min16int4x1", "min16int4x2", "min16int4x3", "min16int4x4", "min12int", "min12int1",
			"min12int2", "min12int3", "min12int4", "min12int1x1", "min12int1x2", "min12int1x3", "min12int1x4", "min12int2x1", "min12int2x2", "min12int2x3",
			"min12int2x4", "min12int3x1", "min12int3x2", "min12int3x3", "min12int3x4", "min12int4x1", "min12int4x2", "min12int4x3", "min12int4x4", "min16uint",
			"min16uint1", "min16uint2", "min16uint3", "min16uint4", "min16uint1x1", "min16uint1x2", "min16uint1x3", "min16uint1x4", "min16uint2x1", "min16uint2x2",
			"min16uint2x3", "min16uint2x4", "min16uint3x1", "min16uint3x2", "min16uint3x3", "min16uint3x4", "min16uint4x1", "min16uint4x2", "min16uint4x3",
			"min16uint4x4", "namespace", "nointerpolation", "noperspective", "NULL", "out", "OutputPatch", "packoffset", "pass", "pixelfragment", "PixelShader",
			"point", "PointStream", "precise", "RasterizerState", "RenderTargetView", "return", "register", "row_major", "RWBuffer", "RWByteAddressBuffer",
			"RWStructuredBuffer", "RWTexture1D", "RWTexture1DArray", "RWTexture2D", "RWTexture2DArray", "RWTexture3D", "sample", "sampler", "SamplerState",
			"SamplerComparisonState", "shared", "snorm", "stateblock", "stateblock_state", "static", "string", "struct", "switch", "StructuredBuffer",
			"tbuffer", "technique", "technique10", "technique11", "texture", "Texture1D", "Texture1DArray", "Texture2D", "Texture2DArray", "Texture2DMS",
			"Texture2DMSArray", "Texture3D", "TextureCube", "TextureCubeArray", "true", "typedef", "triangle", "triangleadj", "TriangleStream", "uint",
			"uint1", "uint2", "uint3", "uint4", "uint1x1", "uint1x2", "uint1x3", "uint1x4", "uint2x1", "uint2x2", "uint2x3", "uint2x4", "uint3x1", "uint3x2",
			"uint3x3", "uint3x4", "uint4x1", "uint4x2", "uint4x3", "uint4x4", "uniform", "unorm", "unsigned", "vector", "vertexfragment", "VertexShader",
			"void", "volatile", "while"
		};

		if ( reserved.IsExist( name ) )
		{
			"rsvd_" >> name;
		}
		else
		// prefix '_' is reserved, replace by '__' prefix
		if ( name.Length() > 1	and
			 name[0] == '_'		and
			 name[1] != '_' )
		{
			'_' >> name;
		}
	}
	
/*
=================================================
	_ReplaceHLSLBuiltinFn
=================================================
*/
	void ShaderVarNameValidator::_ReplaceHLSLBuiltinFn (INOUT String &name)
	{
	}

/*
=================================================
	_GetSpecialCases
=================================================
*/
	ShaderVarNameValidator::SpecialCases_t const&  ShaderVarNameValidator::_GetSpecialCases ()
	{
		static const SpecialCases_t		map = {
			{ "i", {"c", "h", "k", "m"}},
			{ "j", {"t", "u", "l", "d"}}
		};
		return map;
	}

}	// PipelineCompiler
