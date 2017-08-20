// This is generated file, don't change anything!

#include "GLSLNodeFuncs.Shared.h"


namespace ShaderEditor
{
namespace _ShaderNodesHidden_
{
/*
=================================================
	_RegisterDefaultGLSLFunctions
=================================================
*/
	void NodeFunctions::_RegisterDefaultGLSLFunctions ()
	{
		RegisterGLSLShaderNodes0 ( this );
		RegisterGLSLShaderNodes1 ( this );
		RegisterGLSLShaderNodes2 ( this );
		RegisterGLSLShaderNodes3 ( this );
		RegisterGLSLShaderNodes4 ( this );
		RegisterGLSLShaderNodes5 ( this );
		RegisterGLSLShaderNodes6 ( this );
	}

	const char* g_GLSLSharedShaderSource = R"##(
#define and			&&
#define or			||
#define not			!

#define bool2		bvec2
#define bool3		bvec3
#define bool4		bvec4

#define int2		ivec2
#define int3		ivec3
#define int4		ivec4

#define uint2		uvec2
#define uint3		uvec3
#define uint4		uvec4

#ifdef GL_ARB_gpu_shader_int64

# define ilong		int64_t
# define ilong2		i64vec2
# define ilong3		i64vec3
# define ilong4		i64vec4

# define ulong		uint64_t
# define ulong2		u64vec2
# define ulong3		u64vec3
# define ulong4		u64vec4

#endif // GL_ARB_gpu_shader_int64

#define float2		vec2
#define float3		vec3
#define float4		vec4
			
#define float2x2	mat2
#define float3x3	mat3
#define float4x4	mat4

#define double2		dvec2
#define double3		dvec3
#define double4		dvec4
			
#define double2x2	dmat2
#define double3x3	dmat3
#define double4x4	dmat4

#define SH_VERTEX           0
#define SH_TESS_CONTROL     1
#define SH_TESS_EVALUATION  2
#define SH_GEOMETRY         3
#define SH_FRAGMENT         4
#define SH_COMPUTE          5
)##";

}	// _ShaderNodesHidden_
}	// ShaderEditor
