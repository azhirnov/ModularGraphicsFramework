// This is generated file
// Origin file: 'C:\Projects\graphxgenengine\Projects\ShaderEditorTools\Glowballs.cpp'
// Created at: 2018/02/02 - 00:31:58

#include "all_pipelines.h"

namespace Pipelines
{

void Create_glowballs (PipelineTemplateDescriptor& descr)
{
	descr = PipelineTemplateDescriptor();
	// RenderState
	descr.renderState = RenderState();
	// InputAssemblyState
	descr.renderState.inputAssembly.topology         = EPrimitive::TriangleStrip;
	descr.renderState.inputAssembly.primitiveRestart = false;

	descr.dynamicStates = EPipelineDynamicState::bits() | EPipelineDynamicState::Viewport | EPipelineDynamicState::Scissor;
	descr.supportedShaders = EShader::bits() | EShader::Vertex | EShader::Fragment;
	descr.supportedPrimitives = EPrimitive::bits() | EPrimitive::TriangleStrip;

	descr.layout = PipelineLayoutDescriptor::Builder()
			.AddTexture( "iChannel0", EImage::Tex2D, EPixelFormatClass::RGBA | EPixelFormatClass::LinearColorSpace | EPixelFormatClass::AnyFloat | EPixelFormatClass::AnyNorm, 2, 0, EShader::bits() | EShader::Fragment )
			.AddUniformBuffer( "", 288_b, 0, 1, EShader::bits() | EShader::Fragment )
			.Finish();

	descr.Vertex().StringGLSL( 
R"#(#version 450 core


#define SH_VERTEX           (1<<0)
#define SH_TESS_CONTROL     (1<<1)
#define SH_TESS_EVALUATION  (1<<2)
#define SH_GEOMETRY         (1<<3)
#define SH_FRAGMENT         (1<<4)
#define SH_COMPUTE          (1<<5)

#ifdef GL_ARB_gpu_shader_int64
#extension GL_ARB_gpu_shader_int64 : require
#define ARB_gpu_shader_int64_enabled  1
#endif

// for vulkan compatibility
#ifdef GL_ARB_separate_shader_objects
#extension GL_ARB_separate_shader_objects : enable
#define ARB_separate_shader_objects_enabled  1
#endif

#define bool2		bvec2
#define bool3		bvec3
#define bool4		bvec4

#define int2		ivec2
#define int3		ivec3
#define int4		ivec4

#define uint2		uvec2
#define uint3		uvec3
#define uint4		uvec4

#ifdef ARB_gpu_shader_int64_enabled
# define ilong		int64_t
# define ilong2		i64vec2
# define ilong3		i64vec3
# define ilong4		i64vec4
# define ulong		uint64_t
# define ulong2		u64vec2
# define ulong3		u64vec3
# define ulong4		u64vec4
#endif // ARB_gpu_shader_int64_enabled

#define float2		vec2
#define float3		vec3
#define float4		vec4
			
#define float2x2	mat2x2
#define float2x3	mat2x3
#define float2x4	mat2x4
#define float3x2	mat3x2
#define float3x3	mat3x3
#define float3x4	mat3x4
#define float4x2	mat4x2
#define float4x3	mat4x3
#define float4x4	mat4x4

#define double2		dvec2
#define double3		dvec3
#define double4		dvec4
			
#define double2x2	dmat2x2
#define double2x3	dmat2x3
#define double2x4	dmat2x4
#define double3x2	dmat3x2
#define double3x3	dmat3x3
#define double3x4	dmat3x4
#define double4x2	dmat4x2
#define double4x3	dmat4x3
#define double4x4	dmat4x4

#ifdef VULKAN
#define PUSH_CONSTANT( _name_ )	layout (std140, push_constant) uniform _name_
#else
#define PUSH_CONSTANT( _name_ )	layout (std140) uniform pushConst_##_name_
#endif
		
#define SHADER	SH_VERTEX
#ifdef VULKAN
# define gl_VertexID    gl_VertexIndex
# define gl_InstanceID  gl_InstanceIndex
#else
# define gl_VertexIndex    gl_VertexID
# define gl_InstanceIndex  gl_InstanceID
#endif
out gl_PerVertex {
	vec4 gl_Position;
	float gl_PointSize;
	float gl_ClipDistance[];
	float gl_CullDistance[];
};






const vec2 g_Positions[4] = { vec2(-1.0, 1.0), vec2(-1.0), vec2(1.0), vec2(1.0, -1.0) };

//---------------------------------


//---------------------------------
void main ()
{
	gl_Position=vec4( g_Positions[gl_VertexID], float(0.0), float(1.0) );
}



)#"_str );
	descr.Vertex().ArraySPIRV({ 
0x07230203, 0x00010000, 0x00080002, 0x00000029, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0007000F, 0x00000000, 0x00000005, 0x6E69616D, 0x00000000, 0x0000000E, 0x0000001C, 0x00030007, 
0x00000001, 0x00000000, 0x002E0003, 0x00000002, 0x000001C2, 0x00000001, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x746E6520, 
0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x63206465, 0x6E65696C, 0x75762074, 0x6E616B6C, 
0x0A303031, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x72617420, 0x2D746567, 0x20766E65, 0x6B6C7576, 0x2E316E61, 0x2F2F0A30, 
0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x65206465, 0x7972746E, 0x696F702D, 0x6D20746E, 0x0A6E6961, 0x6E696C23, 0x0A312065, 0x00000000, 
0x00070004, 0x415F4C47, 0x675F4252, 0x735F7570, 0x65646168, 0x6E695F72, 0x00343674, 0x00090004, 0x415F4C47, 0x735F4252, 0x72617065, 0x5F657461, 
0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00040005, 0x00000005, 0x6E69616D, 0x00000000, 0x00060005, 0x0000000C, 0x505F6C67, 0x65567265, 
0x78657472, 0x00000000, 0x00060006, 0x0000000C, 0x00000000, 0x505F6C67, 0x7469736F, 0x006E6F69, 0x00070006, 0x0000000C, 0x00000001, 0x505F6C67, 
0x746E696F, 0x657A6953, 0x00000000, 0x00070006, 0x0000000C, 0x00000002, 0x435F6C67, 0x4470696C, 0x61747369, 0x0065636E, 0x00070006, 0x0000000C, 
0x00000003, 0x435F6C67, 0x446C6C75, 0x61747369, 0x0065636E, 0x00030005, 0x0000000E, 0x00000000, 0x00060005, 0x0000001C, 0x565F6C67, 0x65747265, 
0x646E4978, 0x00007865, 0x00050005, 0x0000001F, 0x65646E69, 0x6C626178, 0x00000065, 0x00050048, 0x0000000C, 0x00000000, 0x0000000B, 0x00000000, 
0x00050048, 0x0000000C, 0x00000001, 0x0000000B, 0x00000001, 0x00050048, 0x0000000C, 0x00000002, 0x0000000B, 0x00000003, 0x00050048, 0x0000000C, 
0x00000003, 0x0000000B, 0x00000004, 0x00030047, 0x0000000C, 0x00000002, 0x00040047, 0x0000001C, 0x0000000B, 0x0000002A, 0x00020013, 0x00000003, 
0x00030021, 0x00000004, 0x00000003, 0x00030016, 0x00000007, 0x00000020, 0x00040017, 0x00000008, 0x00000007, 0x00000004, 0x00040015, 0x00000009, 
0x00000020, 0x00000000, 0x0004002B, 0x00000009, 0x0000000A, 0x00000001, 0x0004001C, 0x0000000B, 0x00000007, 0x0000000A, 0x0006001E, 0x0000000C, 
0x00000008, 0x00000007, 0x0000000B, 0x0000000B, 0x00040020, 0x0000000D, 0x00000003, 0x0000000C, 0x0004003B, 0x0000000D, 0x0000000E, 0x00000003, 
0x00040015, 0x0000000F, 0x00000020, 0x00000001, 0x0004002B, 0x0000000F, 0x00000010, 0x00000000, 0x00040017, 0x00000011, 0x00000007, 0x00000002, 
0x0004002B, 0x00000009, 0x00000012, 0x00000004, 0x0004001C, 0x00000013, 0x00000011, 0x00000012, 0x0004002B, 0x00000007, 0x00000014, 0xBF800000, 
0x0004002B, 0x00000007, 0x00000015, 0x3F800000, 0x0005002C, 0x00000011, 0x00000016, 0x00000014, 0x00000015, 0x0005002C, 0x00000011, 0x00000017, 
0x00000014, 0x00000014, 0x0005002C, 0x00000011, 0x00000018, 0x00000015, 0x00000015, 0x0005002C, 0x00000011, 0x00000019, 0x00000015, 0x00000014, 
0x0007002C, 0x00000013, 0x0000001A, 0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x00040020, 0x0000001B, 0x00000001, 0x0000000F, 0x0004003B, 
0x0000001B, 0x0000001C, 0x00000001, 0x00040020, 0x0000001E, 0x00000007, 0x00000013, 0x00040020, 0x00000020, 0x00000007, 0x00000011, 0x0004002B, 
0x00000007, 0x00000023, 0x00000000, 0x00040020, 0x00000027, 0x00000003, 0x00000008, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 
0x000200F8, 0x00000006, 0x0004003B, 0x0000001E, 0x0000001F, 0x00000007, 0x00040008, 0x00000001, 0x0000006B, 0x00000000, 0x0004003D, 0x0000000F, 
0x0000001D, 0x0000001C, 0x0003003E, 0x0000001F, 0x0000001A, 0x00050041, 0x00000020, 0x00000021, 0x0000001F, 0x0000001D, 0x0004003D, 0x00000011, 
0x00000022, 0x00000021, 0x00050051, 0x00000007, 0x00000024, 0x00000022, 0x00000000, 0x00050051, 0x00000007, 0x00000025, 0x00000022, 0x00000001, 
0x00070050, 0x00000008, 0x00000026, 0x00000024, 0x00000025, 0x00000023, 0x00000015, 0x00050041, 0x00000027, 0x00000028, 0x0000000E, 0x00000010, 
0x0003003E, 0x00000028, 0x00000026, 0x000100FD, 0x00010038 });
	descr.Vertex().StringSpirvAsm( 
R"#(// Module Version 10000
// Generated by (magic number): 80002
// Id's are bound by 41

                              Capability Shader
               2:             ExtInstImport  "GLSL.std.450"
                              MemoryModel Logical GLSL450
                              EntryPoint Vertex 5  "main" 14 28
               1:             String  ""
                              Source GLSL 450 1  "// OpModuleProcessed entry-point main
// OpModuleProcessed client vulkan100
// OpModuleProcessed target-env vulkan1.0
// OpModuleProcessed entry-point main
#line 1
"
                              SourceExtension  "GL_ARB_gpu_shader_int64"
                              SourceExtension  "GL_ARB_separate_shader_objects"
                              Name 5  "main"
                              Name 12  "gl_PerVertex"
                              MemberName 12(gl_PerVertex) 0  "gl_Position"
                              MemberName 12(gl_PerVertex) 1  "gl_PointSize"
                              MemberName 12(gl_PerVertex) 2  "gl_ClipDistance"
                              MemberName 12(gl_PerVertex) 3  "gl_CullDistance"
                              Name 14  ""
                              Name 28  "gl_VertexIndex"
                              Name 31  "indexable"
                              MemberDecorate 12(gl_PerVertex) 0 BuiltIn Position
                              MemberDecorate 12(gl_PerVertex) 1 BuiltIn PointSize
                              MemberDecorate 12(gl_PerVertex) 2 BuiltIn ClipDistance
                              MemberDecorate 12(gl_PerVertex) 3 BuiltIn CullDistance
                              Decorate 12(gl_PerVertex) Block
                              Decorate 28(gl_VertexIndex) BuiltIn VertexIndex
               3:             TypeVoid
               4:             TypeFunction 3
               7:             TypeFloat 32
               8:             TypeVector 7(float) 4
               9:             TypeInt 32 0
              10:      9(int) Constant 1
              11:             TypeArray 7(float) 10
12(gl_PerVertex):             TypeStruct 8(fvec4) 7(float) 11 11
              13:             TypePointer Output 12(gl_PerVertex)
              14:     13(ptr) Variable Output
              15:             TypeInt 32 1
              16:     15(int) Constant 0
              17:             TypeVector 7(float) 2
              18:      9(int) Constant 4
              19:             TypeArray 17(fvec2) 18
              20:    7(float) Constant 3212836864
              21:    7(float) Constant 1065353216
              22:   17(fvec2) ConstantComposite 20 21
              23:   17(fvec2) ConstantComposite 20 20
              24:   17(fvec2) ConstantComposite 21 21
              25:   17(fvec2) ConstantComposite 21 20
              26:          19 ConstantComposite 22 23 24 25
              27:             TypePointer Input 15(int)
28(gl_VertexIndex):     27(ptr) Variable Input
              30:             TypePointer Function 19
              32:             TypePointer Function 17(fvec2)
              35:    7(float) Constant 0
              39:             TypePointer Output 8(fvec4)
         5(main):           3 Function None 4
               6:             Label
   31(indexable):     30(ptr) Variable Function
                              Line 1 107 0
              29:     15(int) Load 28(gl_VertexIndex)
                              Store 31(indexable) 26
              33:     32(ptr) AccessChain 31(indexable) 29
              34:   17(fvec2) Load 33
              36:    7(float) CompositeExtract 34 0
              37:    7(float) CompositeExtract 34 1
              38:    8(fvec4) CompositeConstruct 36 37 35 21
              40:     39(ptr) AccessChain 14 16
                              Store 40 38
                              Return
                              FunctionEnd
)#"_str );

	descr.fragOutput = FragmentOutputState()
			.Add( "out_Color0", EFragOutput::Float4, 0 );

	descr.Fragment().StringGLSL( 
R"#(#version 450 core


#define SH_VERTEX           (1<<0)
#define SH_TESS_CONTROL     (1<<1)
#define SH_TESS_EVALUATION  (1<<2)
#define SH_GEOMETRY         (1<<3)
#define SH_FRAGMENT         (1<<4)
#define SH_COMPUTE          (1<<5)

#ifdef GL_ARB_gpu_shader_int64
#extension GL_ARB_gpu_shader_int64 : require
#define ARB_gpu_shader_int64_enabled  1
#endif

// for vulkan compatibility
#ifdef GL_ARB_separate_shader_objects
#extension GL_ARB_separate_shader_objects : enable
#define ARB_separate_shader_objects_enabled  1
#endif

#define bool2		bvec2
#define bool3		bvec3
#define bool4		bvec4

#define int2		ivec2
#define int3		ivec3
#define int4		ivec4

#define uint2		uvec2
#define uint3		uvec3
#define uint4		uvec4

#ifdef ARB_gpu_shader_int64_enabled
# define ilong		int64_t
# define ilong2		i64vec2
# define ilong3		i64vec3
# define ilong4		i64vec4
# define ulong		uint64_t
# define ulong2		u64vec2
# define ulong3		u64vec3
# define ulong4		u64vec4
#endif // ARB_gpu_shader_int64_enabled

#define float2		vec2
#define float3		vec3
#define float4		vec4
			
#define float2x2	mat2x2
#define float2x3	mat2x3
#define float2x4	mat2x4
#define float3x2	mat3x2
#define float3x3	mat3x3
#define float3x4	mat3x4
#define float4x2	mat4x2
#define float4x3	mat4x3
#define float4x4	mat4x4

#define double2		dvec2
#define double3		dvec3
#define double4		dvec4
			
#define double2x2	dmat2x2
#define double2x3	dmat2x3
#define double2x4	dmat2x4
#define double3x2	dmat3x2
#define double3x3	dmat3x3
#define double3x4	dmat3x4
#define double4x2	dmat4x2
#define double4x3	dmat4x3
#define double4x4	dmat4x4

#ifdef VULKAN
#define PUSH_CONSTANT( _name_ )	layout (std140, push_constant) uniform _name_
#else
#define PUSH_CONSTANT( _name_ )	layout (std140) uniform pushConst_##_name_
#endif
		
#define SHADER	SH_FRAGMENT

layout(location=0) out   vec4 out_Color0;

layout(binding=2) uniform sampler2D iChannel0;

layout(binding=0) layout(std140) uniform ShadertoyUB {
	layout(offset=0, align=16) vec4  iResolution;    // offset: 0, align: 16
	layout(offset=16, align=4) float  iTime;    // offset: 16, align: 4
	layout(offset=20, align=4) float  iTimeDelta;    // offset: 20, align: 4
	layout(offset=24, align=4) int  iFrame;    // offset: 24, align: 4
	layout(offset=28, align=4) float  _padding0;    // offset: 28, align: 4
	layout(offset=32, align=16) vec4  iChannelTime[4];    // offset: 32, align: 16
	layout(offset=96, align=16) vec4  iChannelResolution[4];    // offset: 96, align: 16
	layout(offset=160, align=16) vec4  iMouse;    // offset: 160, align: 16
	layout(offset=176, align=16) vec4  iDate;    // offset: 176, align: 16
	layout(offset=192, align=4) float  iSampleRate;    // offset: 192, align: 4
	layout(offset=196, align=4) float  _padding1;    // offset: 196, align: 4
	layout(offset=200, align=4) float  _padding2;    // offset: 200, align: 4
	layout(offset=204, align=4) float  _padding3;    // offset: 204, align: 4
	layout(offset=208, align=16) vec4  iCameraFrustum[4];    // offset: 208, align: 16
	layout(offset=272, align=16) vec4  iCameraPos;    // offset: 272, align: 16

} ;



struct Box{
	vec3 pos;
	vec3 size;
};

struct Ray{
	vec3 origin;
	vec3 dir;
};

struct Camera{
	vec3 pos;
	Ray ray;
};

struct Sphere{
	vec3 pos;
	float radius;
};

struct HitTest{
	bool hit;
	float dist;
	vec3 normal;
	vec4 col;
	float ref;
};


//---------------------------------

float t;
float divergence;

//---------------------------------

HitTest minT (in HitTest a, in HitTest b);
HitTest minT (in HitTest a, in HitTest b, in HitTest c);
vec4 traceScene (in Camera cam, in vec2 seed, in float lastB);
HitTest minT (in HitTest a, in HitTest b, in HitTest c, in HitTest d, in HitTest e);
HitTest intersectBox (in Ray r, in Box b);
float nrand (in vec2 n);
HitTest intersectSphere (in Ray r, in Sphere s);
Camera setupCam (in vec3 pos, in vec3 target, in float fov, in vec2 uv);
void mainImage (out vec4 fragColor, in vec2 fragCoord);

//---------------------------------
void main ()
{
	vec2 coord;
	coord=gl_FragCoord.xy;
	coord.y=(iResolution.xyz.y-coord.y);
	mainImage( out_Color0, coord );
}


HitTest minT (in HitTest a, in HitTest b)
{
	if ( (a.dist<b.dist))
	{
		return a;
	;
	}
	else
	{
		return b;
	;
	}
	;
}


HitTest minT (in HitTest a, in HitTest b, in HitTest c)
{
	return minT( a, minT( b, c ) );
}


HitTest minT (in HitTest a, in HitTest b, in HitTest c, in HitTest d, in HitTest e)
{
	return minT( minT( a, b ), minT( c, d, e ) );
}


HitTest intersectBox (in Ray r, in Box b)
{
	(b.size*=float(0.5));
	vec3 ba;
	ba=(b.pos-b.size);
	vec3 bb;
	bb=(b.pos+b.size);
	HitTest h;
	h=HitTest( bool(false), float(100000.0), vec3(0.0), vec4(0.0), float(0.0) );
	float d;
	d=float(100000.0);
	vec3 dA;
	dA=((r.origin-ba)/-( r.dir ));
	vec3 dB;
	dB=((r.origin-bb)/-( r.dir ));
	dA.x=( ((dA.x<=float(0.0))) ? (float(100000.0)) : (dA.x) );
	dA.y=( ((dA.y<=float(0.0))) ? (float(100000.0)) : (dA.y) );
	dA.z=( ((dA.z<=float(0.0))) ? (float(100000.0)) : (dA.z) );
	dB.x=( ((dB.x<=float(0.0))) ? (float(100000.0)) : (dB.x) );
	dB.y=( ((dB.y<=float(0.0))) ? (float(100000.0)) : (dB.y) );
	dB.z=( ((dB.z<=float(0.0))) ? (float(100000.0)) : (dB.z) );
	float d1;
	d1=min( dA.x, min( dA.y, dA.z ) );
	float d2;
	d2=min( dB.x, min( dB.y, dB.z ) );
	d=min( d1, d2 );
	vec3 endPoint;
	endPoint=(r.origin+(r.dir*d));
	(endPoint-=b.pos);
	if ( (d!=float(100000.0)))
	{
		h.hit=bool(true);
		h.dist=d;
		h.ref=float(0.0);
		if ( (abs( (abs( endPoint.x )-b.size.x) )<float(0.01)))
		{
			bool l;
			l=(endPoint.x<float(0.0));
			h.normal=vec3( float( ( (l) ? (int(1)) : (int(-1)) ) ), float(0.0), float(0.0) );
			h.col=( (l) ? (vec4(0.89999998, 0.5, 0.5, 0.0)) : (vec4(0.5, 0.5, 0.89999998, 0.0)) );
			return h;
		;
		}
		;
		if ( (abs( (abs( endPoint.z )-b.size.z) )<float(0.01)))
		{
			h.normal=vec3( float(0.0), float(0.0), -( sign( endPoint.z ) ) );
			h.col=vec4(0.89999998, 0.89999998, 0.89999998, 0.0);
			return h;
		;
		}
		;
		h.normal=vec3( float(0.0), -( sign( endPoint.y ) ), float(0.0) );
		h.col=vec4( float(1.0), float(1.0), float(1.0), (sign( endPoint.y )*clamp( (sin( (t*float(0.242)) )+float(0.80000001)), float(0.2), float(1.20000005) )) );
		return h;
	;
	}
	;
	return h;
}


HitTest intersectSphere (in Ray r, in Sphere s)
{
	vec3 o;
	o=(r.origin-s.pos);
	float v;
	v=dot( o, r.dir );
	if ( (v>float(0.0)))
	{
		return HitTest( bool(false), float(100000.0), vec3(0.0), vec4(0.0), float(0.0) );
	}
	;
	float disc;
	disc=((s.radius*s.radius)-(dot( o, o )-(v*v)));
	if ( (disc<float(0.0)))
	{
		return HitTest( bool(false), float(100000.0), vec3(0.0), vec4(0.0), float(0.0) );
	}
	;
	float dist;
	dist=(length( o )-sqrt( disc ));
	return HitTest( bool(true), dist, normalize( ((r.origin+(r.dir*dist))-s.pos) ), vec4(0.0), ((sin( (iTime*float(0.25)) )*float(0.5))+float(0.5)) );
}


float nrand (in vec2 n)
{
	return fract( (sin( dot( n.xy, vec2(12.98980045, 78.23300171) ) )*float(43758.546875)) );
}


vec4 traceScene (in Camera cam, in vec2 seed, in float lastB)
{
	vec3 startPos;
	startPos=cam.pos;
	vec4 result;
	result=vec4(0.0);
	int maxI;
	maxI=int( (float(30.0)*lastB) );
	{
		int i;
		i=int(0);
		for(; (i<int(30)); ( i )++)
		{
			if ( (i==maxI))
			{
				break;
			}
			;
			Ray r;
			r=cam.ray;
			(r.dir.x+=(((nrand( seed )*float(2.0))-float(1.0))*divergence));
			(r.dir.y+=(((nrand( seed.yx )*float(2.0))-float(1.0))*divergence));
			(r.dir.z+=(((nrand( seed.xx )*float(2.0))-float(1.0))*divergence));
			r.dir=normalize( r.dir );
			vec4 impact;
			impact=vec4(1.0);
			( seed )++;
			int maxJ;
			maxJ=int( (float(10.0)*lastB) );
			{
				int j;
				j=int(0);
				for(; (j<int(10)); ( j )++)
				{
					HitTest t0;
					t0=intersectBox( r, Box( vec3(-5.0, 10.0, -5.0), vec3(30.0, 20.0, 25.0) ) );
					HitTest t1;
					t1=intersectSphere( r, Sphere( vec3(-1.0, 2.0, 0.0), float(2.0) ) );
					t1.col=vec4(0.5, 0.60000002, 0.89999998, 0.0);
					HitTest t2;
					t2=intersectSphere( r, Sphere( vec3(4.0, 5.0, 4.0), float(5.0) ) );
					t2.col=vec4(0.89999998, 0.89999998, 0.0, 0.0);
					t2.ref=float(0.80000001);
					HitTest t3;
					t3=intersectSphere( r, Sphere( vec3(-5.0, 4.0, 4.0), float(4.0) ) );
					t3.col=vec4( float(0.30000001), float(0.89999998), float(0.60000002), (sin( (t*float(0.60000002)) )*float(3.0)) );
					t3.ref=float(0.0);
					HitTest t4;
					t4=intersectSphere( r, Sphere( vec3(4.0, 2.5, -2.0), float(2.5) ) );
					t4.col=vec4( float(1.0), float(0.5), float(0.2), (sin( (t*float(0.69999999)) )*float(3.0)) );
					t4.ref=float(0.0);
					HitTest test;
					test=minT( t0, t1, t2, t3, t4 );
					if ( test.hit)
					{
						(impact*=test.col);
						if ( (test.col.w>float(0.0)))
						{
							(result+=((test.col*impact)*test.col.w));
						;
						}
						;
						(r.origin+=(r.dir*test.dist));
						(r.origin+=(test.normal*float(0.01)));
						vec3 random;
						random=((vec3( nrand( (r.origin.xy+seed) ), nrand( (r.origin.yz+seed) ), nrand( (r.origin.zx+seed) ) )*float(2.0))-float(1.0));
						if ( ((test!=t0)&&(test!=t2)))
						{
							vec3 refl;
							refl=reflect( r.dir, test.normal );
							vec3 matte;
							matte=normalize( (test.normal+random) );
							float s;
							s=max( float(0.0), -( dot( test.normal, r.dir ) ) );
							s=step( pow( s, float(0.5) ), nrand( seed ) );
							r.dir=((refl*s)+(matte*(float(1.0)-s)));
						;
						}
						else
						{
							r.dir=normalize( mix( (test.normal+random), reflect( r.dir, test.normal ), test.ref ) );
						;
						}
						;
					;
					}
					else
					{
						break;
					;
					}
					;
				}
			}
			;
		}
	}
	;
	return (result/float(30.0));
}


Camera setupCam (in vec3 pos, in vec3 target, in float fov, in vec2 uv)
{
	vec2 mouse;
	mouse=(iMouse.xy/iResolution.xyz.xy);
	mouse=((mouse*float(2.0))-float(0.5));
	(pos-=target);
	pos.xz=((cos( (mouse.x*float(0.5)) )*pos.xz)+(sin( (mouse.x*float(0.5)) )*vec2( -( pos.xz.y ), pos.xz.x )));
	pos.xy=((cos( (mouse.y*float(0.5)) )*pos.xy)+(sin( (mouse.y*float(0.5)) )*vec2( -( pos.xy.y ), pos.xy.x )));
	(pos+=target);
	Camera cam;
	cam.pos=pos;
	Ray ray;
	ray.origin=pos;
	(uv*=fov);
	vec3 cw;
	cw=normalize( (target-pos) );
	vec3 cp;
	cp=vec3(0.0, 1.0, 0.0);
	vec3 cu;
	cu=normalize( cross( cw, cp ) );
	vec3 cv;
	cv=normalize( cross( cu, cw ) );
	ray.dir=normalize( (((uv.x*cu)+(uv.y*cv))+(float(0.5)*cw)) );
	cam.ray=ray;
	divergence=(fov/iResolution.xyz.x);
	divergence=(divergence+(length( uv )*float(0.01)));
	return cam;
}


void mainImage (out vec4 fragColor, in vec2 fragCoord)
{
	t=iTime;
	vec2 uv;
	uv=(fragCoord.xy/iResolution.xyz.xy);
	vec4 l;
	l=texture( iChannel0, uv );
	uv=((uv*float(2.0))-float(1.0));
	(uv.y/=(iResolution.xyz.x/iResolution.xyz.y));
	Camera cam;
	cam=setupCam( vec3(0.0, 3.0, -8.0), vec3(4.0, 5.0, 4.0), float(1.0), uv );
	float lastB;
	lastB=max( l.x, max( l.y, l.z ) );
	lastB=pow( lastB, float(0.25) );
	vec4 c;
	c=traceScene( cam, (uv+iTime), max( float(0.30000001), (float(1.0)-lastB) ) );
	fragColor=mix( c, l, ( (((iMouse.z>float(0.5))||(iFrame==int(0)))) ? (float(0.5)) : (float(0.98000002)) ) );
}



)#"_str );
	descr.Fragment().ArraySPIRV({ 
0x07230203, 0x00010000, 0x00080002, 0x000003E8, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0007000F, 0x00000004, 0x00000005, 0x6E69616D, 0x00000000, 0x0000004F, 0x00000065, 0x00030010, 
0x00000005, 0x00000007, 0x00030007, 0x00000001, 0x00000000, 0x002E0003, 0x00000002, 0x000001C2, 0x00000001, 0x4F202F2F, 0x646F4D70, 0x50656C75, 
0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x63206465, 
0x6E65696C, 0x75762074, 0x6E616B6C, 0x0A303031, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x72617420, 0x2D746567, 0x20766E65, 
0x6B6C7576, 0x2E316E61, 0x2F2F0A30, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x65206465, 0x7972746E, 0x696F702D, 0x6D20746E, 0x0A6E6961, 
0x6E696C23, 0x0A312065, 0x00000000, 0x00070004, 0x415F4C47, 0x675F4252, 0x735F7570, 0x65646168, 0x6E695F72, 0x00343674, 0x00090004, 0x415F4C47, 
0x735F4252, 0x72617065, 0x5F657461, 0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00040005, 0x00000005, 0x6E69616D, 0x00000000, 0x00040005, 
0x0000000B, 0x54746948, 0x00747365, 0x00040006, 0x0000000B, 0x00000000, 0x00746968, 0x00050006, 0x0000000B, 0x00000001, 0x74736964, 0x00000000, 
0x00050006, 0x0000000B, 0x00000002, 0x6D726F6E, 0x00006C61, 0x00040006, 0x0000000B, 0x00000003, 0x006C6F63, 0x00040006, 0x0000000B, 0x00000004, 
0x00666572, 0x00140005, 0x00000010, 0x546E696D, 0x72747328, 0x2D746375, 0x54746948, 0x2D747365, 0x662D3162, 0x66762D31, 0x66762D33, 0x31662D34, 
0x74733B31, 0x74637572, 0x7469482D, 0x74736554, 0x2D31622D, 0x762D3166, 0x762D3366, 0x662D3466, 0x003B3131, 0x00030005, 0x0000000E, 0x00000061, 
0x00030005, 0x0000000F, 0x00000062, 0x001D0005, 0x00000016, 0x546E696D, 0x72747328, 0x2D746375, 0x54746948, 0x2D747365, 0x662D3162, 0x66762D31, 
0x66762D33, 0x31662D34, 0x74733B31, 0x74637572, 0x7469482D, 0x74736554, 0x2D31622D, 0x762D3166, 0x762D3366, 0x662D3466, 0x733B3131, 0x63757274, 
0x69482D74, 0x73655474, 0x31622D74, 0x2D31662D, 0x2D336676, 0x2D346676, 0x3B313166, 0x00000000, 0x00030005, 0x00000013, 0x00000061, 0x00030005, 
0x00000014, 0x00000062, 0x00030005, 0x00000015, 0x00000063, 0x002D0005, 0x0000001E, 0x546E696D, 0x72747328, 0x2D746375, 0x54746948, 0x2D747365, 
0x662D3162, 0x66762D31, 0x66762D33, 0x31662D34, 0x74733B31, 0x74637572, 0x7469482D, 0x74736554, 0x2D31622D, 0x762D3166, 0x762D3366, 0x662D3466, 
0x733B3131, 0x63757274, 0x69482D74, 0x73655474, 0x31622D74, 0x2D31662D, 0x2D336676, 0x2D346676, 0x3B313166, 0x75727473, 0x482D7463, 0x65547469, 
0x622D7473, 0x31662D31, 0x3366762D, 0x3466762D, 0x3131662D, 0x7274733B, 0x2D746375, 0x54746948, 0x2D747365, 0x662D3162, 0x66762D31, 0x66762D33, 
0x31662D34, 0x00003B31, 0x00030005, 0x00000019, 0x00000061, 0x00030005, 0x0000001A, 0x00000062, 0x00030005, 0x0000001B, 0x00000063, 0x00030005, 
0x0000001C, 0x00000064, 0x00030005, 0x0000001D, 0x00000065, 0x00030005, 0x00000020, 0x00796152, 0x00050006, 0x00000020, 0x00000000, 0x6769726F, 
0x00006E69, 0x00040006, 0x00000020, 0x00000001, 0x00726964, 0x00030005, 0x00000022, 0x00786F42, 0x00040006, 0x00000022, 0x00000000, 0x00736F70, 
0x00050006, 0x00000022, 0x00000001, 0x657A6973, 0x00000000, 0x00100005, 0x00000027, 0x65746E69, 0x63657372, 0x786F4274, 0x72747328, 0x2D746375, 
0x2D796152, 0x2D336676, 0x31336676, 0x7274733B, 0x2D746375, 0x2D786F42, 0x2D336676, 0x31336676, 0x0000003B, 0x00030005, 0x00000025, 0x00000072, 
0x00030005, 0x00000026, 0x00000062, 0x00040005, 0x00000029, 0x65687053, 0x00006572, 0x00040006, 0x00000029, 0x00000000, 0x00736F70, 0x00050006, 
0x00000029, 0x00000001, 0x69646172, 0x00007375, 0x00110005, 0x0000002E, 0x65746E69, 0x63657372, 0x68705374, 0x28657265, 0x75727473, 0x522D7463, 
0x762D7961, 0x762D3366, 0x3B313366, 0x75727473, 0x532D7463, 0x72656870, 0x66762D65, 0x31662D33, 0x00003B31, 0x00030005, 0x0000002C, 0x00000072, 
0x00030005, 0x0000002D, 0x00000073, 0x00050005, 0x00000034, 0x6E61726E, 0x66762864, 0x00003B32, 0x00030005, 0x00000033, 0x0000006E, 0x00040005, 
0x00000036, 0x656D6143, 0x00006172, 0x00040006, 0x00000036, 0x00000000, 0x00736F70, 0x00040006, 0x00000036, 0x00000001, 0x00796172, 0x00110005, 
0x0000003D, 0x63617274, 0x65635365, 0x7328656E, 0x63757274, 0x61432D74, 0x6172656D, 0x3366762D, 0x7274732D, 0x2D746375, 0x2D796152, 0x2D336676, 
0x31336676, 0x66763B31, 0x31663B32, 0x0000003B, 0x00030005, 0x0000003A, 0x006D6163, 0x00040005, 0x0000003B, 0x64656573, 0x00000000, 0x00040005, 
0x0000003C, 0x7473616C, 0x00000042, 0x00090005, 0x00000045, 0x75746573, 0x6D614370, 0x33667628, 0x3366763B, 0x3B31663B, 0x3B326676, 0x00000000, 
0x00030005, 0x00000041, 0x00736F70, 0x00040005, 0x00000042, 0x67726174, 0x00007465, 0x00030005, 0x00000043, 0x00766F66, 0x00030005, 0x00000044, 
0x00007675, 0x00070005, 0x0000004B, 0x6E69616D, 0x67616D49, 0x66762865, 0x66763B34, 0x00003B32, 0x00050005, 0x00000049, 0x67617266, 0x6F6C6F43, 
0x00000072, 0x00050005, 0x0000004A, 0x67617266, 0x726F6F43, 0x00000064, 0x00040005, 0x0000004D, 0x726F6F63, 0x00000064, 0x00060005, 0x0000004F, 
0x465F6C67, 0x43676172, 0x64726F6F, 0x00000000, 0x00050005, 0x00000058, 0x64616853, 0x6F747265, 0x00425579, 0x00060006, 0x00000058, 0x00000000, 
0x73655269, 0x74756C6F, 0x006E6F69, 0x00050006, 0x00000058, 0x00000001, 0x6D695469, 0x00000065, 0x00060006, 0x00000058, 0x00000002, 0x6D695469, 
0x6C654465, 0x00006174, 0x00050006, 0x00000058, 0x00000003, 0x61724669, 0x0000656D, 0x00060006, 0x00000058, 0x00000004, 0x6461705F, 0x676E6964, 
0x00000030, 0x00070006, 0x00000058, 0x00000005, 0x61684369, 0x6C656E6E, 0x656D6954, 0x00000000, 0x00080006, 0x00000058, 0x00000006, 0x61684369, 
0x6C656E6E, 0x6F736552, 0x6974756C, 0x00006E6F, 0x00050006, 0x00000058, 0x00000007, 0x756F4D69, 0x00006573, 0x00050006, 0x00000058, 0x00000008, 
0x74614469, 0x00000065, 0x00060006, 0x00000058, 0x00000009, 0x6D615369, 0x52656C70, 0x00657461, 0x00060006, 0x00000058, 0x0000000A, 0x6461705F, 
0x676E6964, 0x00000031, 0x00060006, 0x00000058, 0x0000000B, 0x6461705F, 0x676E6964, 0x00000032, 0x00060006, 0x00000058, 0x0000000C, 0x6461705F, 
0x676E6964, 0x00000033, 0x00070006, 0x00000058, 0x0000000D, 0x6D614369, 0x46617265, 0x74737572, 0x00006D75, 0x00060006, 0x00000058, 0x0000000E, 
0x6D614369, 0x50617265, 0x0000736F, 0x00030005, 0x0000005A, 0x00000000, 0x00050005, 0x00000065, 0x5F74756F, 0x6F6C6F43, 0x00003072, 0x00040005, 
0x00000066, 0x61726170, 0x0000006D, 0x00040005, 0x00000067, 0x61726170, 0x0000006D, 0x00040005, 0x00000079, 0x61726170, 0x0000006D, 0x00040005, 
0x0000007B, 0x61726170, 0x0000006D, 0x00040005, 0x0000007E, 0x61726170, 0x0000006D, 0x00040005, 0x00000080, 0x61726170, 0x0000006D, 0x00040005, 
0x00000084, 0x61726170, 0x0000006D, 0x00040005, 0x00000086, 0x61726170, 0x0000006D, 0x00040005, 0x00000089, 0x61726170, 0x0000006D, 0x00040005, 
0x0000008B, 0x61726170, 0x0000006D, 0x00040005, 0x0000008D, 0x61726170, 0x0000006D, 0x00040005, 0x00000090, 0x61726170, 0x0000006D, 0x00040005, 
0x00000091, 0x61726170, 0x0000006D, 0x00030005, 0x0000009A, 0x00006162, 0x00030005, 0x000000A0, 0x00006262, 0x00030005, 0x000000A6, 0x00000068, 
0x00030005, 0x000000AD, 0x00000064, 0x00030005, 0x000000AE, 0x00004164, 0x00030005, 0x000000B7, 0x00004264, 0x00030005, 0x00000104, 0x00003164, 
0x00030005, 0x0000010D, 0x00003264, 0x00050005, 0x00000119, 0x50646E65, 0x746E696F, 0x00000000, 0x00030005, 0x0000013B, 0x0000006C, 0x00030005, 
0x00000170, 0x00000074, 0x00030005, 0x00000182, 0x0000006F, 0x00030005, 0x00000188, 0x00000076, 0x00040005, 0x00000192, 0x63736964, 0x00000000, 
0x00040005, 0x000001A5, 0x74736964, 0x00000000, 0x00050005, 0x000001CC, 0x72617473, 0x736F5074, 0x00000000, 0x00040005, 0x000001CF, 0x75736572, 
0x0000746C, 0x00040005, 0x000001D1, 0x4978616D, 0x00000000, 0x00030005, 0x000001D6, 0x00000069, 0x00030005, 0x000001E5, 0x00000072, 0x00040005, 
0x000001E8, 0x61726170, 0x0000006D, 0x00050005, 0x000001EE, 0x65766964, 0x6E656772, 0x00006563, 0x00040005, 0x000001F5, 0x61726170, 0x0000006D, 
0x00040005, 0x00000201, 0x61726170, 0x0000006D, 0x00040005, 0x00000211, 0x61706D69, 0x00007463, 0x00040005, 0x00000216, 0x4A78616D, 0x00000000, 
0x00030005, 0x0000021B, 0x0000006A, 0x00030005, 0x00000224, 0x00003074, 0x00040005, 0x0000022B, 0x61726170, 0x0000006D, 0x00040005, 0x0000022D, 
0x61726170, 0x0000006D, 0x00030005, 0x0000022F, 0x00003174, 0x00040005, 0x00000233, 0x61726170, 0x0000006D, 0x00040005, 0x00000235, 0x61726170, 
0x0000006D, 0x00030005, 0x0000023A, 0x00003274, 0x00040005, 0x0000023F, 0x61726170, 0x0000006D, 0x00040005, 0x00000241, 0x61726170, 0x0000006D, 
0x00030005, 0x00000246, 0x00003374, 0x00040005, 0x00000249, 0x61726170, 0x0000006D, 0x00040005, 0x0000024B, 0x61726170, 0x0000006D, 0x00030005, 
0x00000256, 0x00003474, 0x00040005, 0x0000025B, 0x61726170, 0x0000006D, 0x00040005, 0x0000025D, 0x61726170, 0x0000006D, 0x00040005, 0x00000267, 
0x74736574, 0x00000000, 0x00040005, 0x00000268, 0x61726170, 0x0000006D, 0x00040005, 0x0000026A, 0x61726170, 0x0000006D, 0x00040005, 0x0000026C, 
0x61726170, 0x0000006D, 0x00040005, 0x0000026E, 0x61726170, 0x0000006D, 0x00040005, 0x00000270, 0x61726170, 0x0000006D, 0x00040005, 0x0000029A, 
0x646E6172, 0x00006D6F, 0x00040005, 0x000002A0, 0x61726170, 0x0000006D, 0x00040005, 0x000002A7, 0x61726170, 0x0000006D, 0x00040005, 0x000002AE, 
0x61726170, 0x0000006D, 0x00040005, 0x000002E6, 0x6C666572, 0x00000000, 0x00040005, 0x000002EC, 0x7474616D, 0x00000065, 0x00030005, 0x000002F2, 
0x00000073, 0x00040005, 0x000002FC, 0x61726170, 0x0000006D, 0x00040005, 0x00000324, 0x73756F6D, 0x00000065, 0x00030005, 0x00000362, 0x006D6163, 
0x00030005, 0x00000365, 0x00796172, 0x00030005, 0x0000036B, 0x00007763, 0x00030005, 0x00000370, 0x00007063, 0x00030005, 0x00000372, 0x00007563, 
0x00030005, 0x00000377, 0x00007663, 0x00030005, 0x0000039A, 0x00007675, 0x00030005, 0x000003A0, 0x0000006C, 0x00050005, 0x000003A4, 0x61684369, 
0x6C656E6E, 0x00000030, 0x00030005, 0x000003B5, 0x006D6163, 0x00040005, 0x000003B8, 0x61726170, 0x0000006D, 0x00040005, 0x000003B9, 0x61726170, 
0x0000006D, 0x00040005, 0x000003BA, 0x61726170, 0x0000006D, 0x00040005, 0x000003BB, 0x61726170, 0x0000006D, 0x00040005, 0x000003BE, 0x7473616C, 
0x00000042, 0x00030005, 0x000003C9, 0x00000063, 0x00040005, 0x000003D2, 0x61726170, 0x0000006D, 0x00040005, 0x000003D4, 0x61726170, 0x0000006D, 
0x00040005, 0x000003D5, 0x61726170, 0x0000006D, 0x00040047, 0x0000004F, 0x0000000B, 0x0000000F, 0x00040047, 0x00000055, 0x00000006, 0x00000010, 
0x00040047, 0x00000056, 0x00000006, 0x00000010, 0x00040047, 0x00000057, 0x00000006, 0x00000010, 0x00050048, 0x00000058, 0x00000000, 0x00000023, 
0x00000000, 0x00050048, 0x00000058, 0x00000001, 0x00000023, 0x00000010, 0x00050048, 0x00000058, 0x00000002, 0x00000023, 0x00000014, 0x00050048, 
0x00000058, 0x00000003, 0x00000023, 0x00000018, 0x00050048, 0x00000058, 0x00000004, 0x00000023, 0x0000001C, 0x00050048, 0x00000058, 0x00000005, 
0x00000023, 0x00000020, 0x00050048, 0x00000058, 0x00000006, 0x00000023, 0x00000060, 0x00050048, 0x00000058, 0x00000007, 0x00000023, 0x000000A0, 
0x00050048, 0x00000058, 0x00000008, 0x00000023, 0x000000B0, 0x00050048, 0x00000058, 0x00000009, 0x00000023, 0x000000C0, 0x00050048, 0x00000058, 
0x0000000A, 0x00000023, 0x000000C4, 0x00050048, 0x00000058, 0x0000000B, 0x00000023, 0x000000C8, 0x00050048, 0x00000058, 0x0000000C, 0x00000023, 
0x000000CC, 0x00050048, 0x00000058, 0x0000000D, 0x00000023, 0x000000D0, 0x00050048, 0x00000058, 0x0000000E, 0x00000023, 0x00000110, 0x00030047, 
0x00000058, 0x00000002, 0x00040047, 0x0000005A, 0x00000022, 0x00000000, 0x00040047, 0x0000005A, 0x00000021, 0x00000001, 0x00040047, 0x00000065, 
0x0000001E, 0x00000000, 0x00040047, 0x000003A4, 0x00000022, 0x00000000, 0x00040047, 0x000003A4, 0x00000021, 0x00000000, 0x00020013, 0x00000003, 
0x00030021, 0x00000004, 0x00000003, 0x00020014, 0x00000007, 0x00030016, 0x00000008, 0x00000020, 0x00040017, 0x00000009, 0x00000008, 0x00000003, 
0x00040017, 0x0000000A, 0x00000008, 0x00000004, 0x0007001E, 0x0000000B, 0x00000007, 0x00000008, 0x00000009, 0x0000000A, 0x00000008, 0x00040020, 
0x0000000C, 0x00000007, 0x0000000B, 0x00050021, 0x0000000D, 0x0000000B, 0x0000000C, 0x0000000C, 0x00060021, 0x00000012, 0x0000000B, 0x0000000C, 
0x0000000C, 0x0000000C, 0x00080021, 0x00000018, 0x0000000B, 0x0000000C, 0x0000000C, 0x0000000C, 0x0000000C, 0x0000000C, 0x0004001E, 0x00000020, 
0x00000009, 0x00000009, 0x00040020, 0x00000021, 0x00000007, 0x00000020, 0x0004001E, 0x00000022, 0x00000009, 0x00000009, 0x00040020, 0x00000023, 
0x00000007, 0x00000022, 0x00050021, 0x00000024, 0x0000000B, 0x00000021, 0x00000023, 0x0004001E, 0x00000029, 0x00000009, 0x00000008, 0x00040020, 
0x0000002A, 0x00000007, 0x00000029, 0x00050021, 0x0000002B, 0x0000000B, 0x00000021, 0x0000002A, 0x00040017, 0x00000030, 0x00000008, 0x00000002, 
0x00040020, 0x00000031, 0x00000007, 0x00000030, 0x00040021, 0x00000032, 0x00000008, 0x00000031, 0x0004001E, 0x00000036, 0x00000009, 0x00000020, 
0x00040020, 0x00000037, 0x00000007, 0x00000036, 0x00040020, 0x00000038, 0x00000007, 0x00000008, 0x00060021, 0x00000039, 0x0000000A, 0x00000037, 
0x00000031, 0x00000038, 0x00040020, 0x0000003F, 0x00000007, 0x00000009, 0x00070021, 0x00000040, 0x00000036, 0x0000003F, 0x0000003F, 0x00000038, 
0x00000031, 0x00040020, 0x00000047, 0x00000007, 0x0000000A, 0x00050021, 0x00000048, 0x00000003, 0x00000047, 0x00000031, 0x00040020, 0x0000004E, 
0x00000001, 0x0000000A, 0x0004003B, 0x0000004E, 0x0000004F, 0x00000001, 0x00040015, 0x00000052, 0x00000020, 0x00000001, 0x00040015, 0x00000053, 
0x00000020, 0x00000000, 0x0004002B, 0x00000053, 0x00000054, 0x00000004, 0x0004001C, 0x00000055, 0x0000000A, 0x00000054, 0x0004001C, 0x00000056, 
0x0000000A, 0x00000054, 0x0004001C, 0x00000057, 0x0000000A, 0x00000054, 0x0011001E, 0x00000058, 0x0000000A, 0x00000008, 0x00000008, 0x00000052, 
0x00000008, 0x00000055, 0x00000056, 0x0000000A, 0x0000000A, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000057, 0x0000000A, 0x00040020, 
0x00000059, 0x00000002, 0x00000058, 0x0004003B, 0x00000059, 0x0000005A, 0x00000002, 0x0004002B, 0x00000052, 0x0000005B, 0x00000000, 0x0004002B, 
0x00000053, 0x0000005C, 0x00000001, 0x00040020, 0x0000005D, 0x00000002, 0x00000008, 0x00040020, 0x00000064, 0x00000003, 0x0000000A, 0x0004003B, 
0x00000064, 0x00000065, 0x00000003, 0x0004002B, 0x00000052, 0x0000006B, 0x00000001, 0x0004002B, 0x00000008, 0x00000095, 0x3F000000, 0x0003002A, 
0x00000007, 0x000000A7, 0x0004002B, 0x00000008, 0x000000A8, 0x47C35000, 0x0004002B, 0x00000008, 0x000000A9, 0x00000000, 0x0006002C, 0x00000009, 
0x000000AA, 0x000000A9, 0x000000A9, 0x000000A9, 0x0007002C, 0x0000000A, 0x000000AB, 0x000000A9, 0x000000A9, 0x000000A9, 0x000000A9, 0x0008002C, 
0x0000000B, 0x000000AC, 0x000000A7, 0x000000A8, 0x000000AA, 0x000000AB, 0x000000A9, 0x0004002B, 0x00000053, 0x000000C1, 0x00000000, 0x0004002B, 
0x00000053, 0x000000D8, 0x00000002, 0x00030029, 0x00000007, 0x00000129, 0x00040020, 0x0000012A, 0x00000007, 0x00000007, 0x0004002B, 0x00000052, 
0x0000012E, 0x00000004, 0x0004002B, 0x00000008, 0x00000137, 0x3C23D70A, 0x0004002B, 0x00000052, 0x0000013F, 0x00000002, 0x0004002B, 0x00000052, 
0x00000141, 0xFFFFFFFF, 0x0004002B, 0x00000052, 0x00000146, 0x00000003, 0x0004002B, 0x00000008, 0x00000148, 0x3F666666, 0x0007002C, 0x0000000A, 
0x00000149, 0x00000148, 0x00000095, 0x00000095, 0x000000A9, 0x0007002C, 0x0000000A, 0x0000014A, 0x00000095, 0x00000095, 0x00000148, 0x000000A9, 
0x00040017, 0x0000014B, 0x00000007, 0x00000004, 0x0007002C, 0x0000000A, 0x00000161, 0x00000148, 0x00000148, 0x00000148, 0x000000A9, 0x0004002B, 
0x00000008, 0x0000016B, 0x3F800000, 0x00040020, 0x0000016F, 0x00000006, 0x00000008, 0x0004003B, 0x0000016F, 0x00000170, 0x00000006, 0x0004002B, 
0x00000008, 0x00000172, 0x3E77CED9, 0x0004002B, 0x00000008, 0x00000175, 0x3F4CCCCD, 0x0004002B, 0x00000008, 0x00000177, 0x3E4CCCCD, 0x0004002B, 
0x00000008, 0x00000178, 0x3F99999A, 0x0004002B, 0x00000008, 0x000001B9, 0x3E800000, 0x0004002B, 0x00000008, 0x000001C2, 0x414FD639, 0x0004002B, 
0x00000008, 0x000001C3, 0x429C774C, 0x0005002C, 0x00000030, 0x000001C4, 0x000001C2, 0x000001C3, 0x0004002B, 0x00000008, 0x000001C7, 0x472AEE8C, 
0x00040020, 0x000001D0, 0x00000007, 0x00000052, 0x0004002B, 0x00000008, 0x000001D2, 0x41F00000, 0x0004002B, 0x00000052, 0x000001DD, 0x0000001E, 
0x0004002B, 0x00000008, 0x000001EB, 0x40000000, 0x0004003B, 0x0000016F, 0x000001EE, 0x00000006, 0x0007002C, 0x0000000A, 0x00000212, 0x0000016B, 
0x0000016B, 0x0000016B, 0x0000016B, 0x0004002B, 0x00000008, 0x00000217, 0x41200000, 0x0004002B, 0x00000052, 0x00000222, 0x0000000A, 0x0004002B, 
0x00000008, 0x00000225, 0xC0A00000, 0x0006002C, 0x00000009, 0x00000226, 0x00000225, 0x00000217, 0x00000225, 0x0004002B, 0x00000008, 0x00000227, 
0x41A00000, 0x0004002B, 0x00000008, 0x00000228, 0x41C80000, 0x0006002C, 0x00000009, 0x00000229, 0x000001D2, 0x00000227, 0x00000228, 0x0005002C, 
0x00000022, 0x0000022A, 0x00000226, 0x00000229, 0x0004002B, 0x00000008, 0x00000230, 0xBF800000, 0x0006002C, 0x00000009, 0x00000231, 0x00000230, 
0x000001EB, 0x000000A9, 0x0005002C, 0x00000029, 0x00000232, 0x00000231, 0x000001EB, 0x0004002B, 0x00000008, 0x00000237, 0x3F19999A, 0x0007002C, 
0x0000000A, 0x00000238, 0x00000095, 0x00000237, 0x00000148, 0x000000A9, 0x0004002B, 0x00000008, 0x0000023B, 0x40800000, 0x0004002B, 0x00000008, 
0x0000023C, 0x40A00000, 0x0006002C, 0x00000009, 0x0000023D, 0x0000023B, 0x0000023C, 0x0000023B, 0x0005002C, 0x00000029, 0x0000023E, 0x0000023D, 
0x0000023C, 0x0007002C, 0x0000000A, 0x00000243, 0x00000148, 0x00000148, 0x000000A9, 0x000000A9, 0x0006002C, 0x00000009, 0x00000247, 0x00000225, 
0x0000023B, 0x0000023B, 0x0005002C, 0x00000029, 0x00000248, 0x00000247, 0x0000023B, 0x0004002B, 0x00000008, 0x0000024D, 0x3E99999A, 0x0004002B, 
0x00000008, 0x00000251, 0x40400000, 0x0004002B, 0x00000008, 0x00000257, 0x40200000, 0x0004002B, 0x00000008, 0x00000258, 0xC0000000, 0x0006002C, 
0x00000009, 0x00000259, 0x0000023B, 0x00000257, 0x00000258, 0x0005002C, 0x00000029, 0x0000025A, 0x00000259, 0x00000257, 0x0004002B, 0x00000008, 
0x00000260, 0x3F333333, 0x0004002B, 0x00000053, 0x0000027B, 0x00000003, 0x00040017, 0x000002BF, 0x00000007, 0x00000003, 0x0004002B, 0x00000052, 
0x00000325, 0x00000007, 0x00040020, 0x00000326, 0x00000002, 0x0000000A, 0x0006002C, 0x00000009, 0x00000371, 0x000000A9, 0x0000016B, 0x000000A9, 
0x00090019, 0x000003A1, 0x00000008, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x0003001B, 0x000003A2, 0x000003A1, 
0x00040020, 0x000003A3, 0x00000000, 0x000003A2, 0x0004003B, 0x000003A3, 0x000003A4, 0x00000000, 0x0004002B, 0x00000008, 0x000003B6, 0xC1000000, 
0x0006002C, 0x00000009, 0x000003B7, 0x000000A9, 0x00000251, 0x000003B6, 0x00040020, 0x000003DF, 0x00000002, 0x00000052, 0x0004002B, 0x00000008, 
0x000003E4, 0x3F7AE148, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 0x000200F8, 0x00000006, 0x0004003B, 0x00000031, 0x0000004D, 
0x00000007, 0x0004003B, 0x00000047, 0x00000066, 0x00000007, 0x0004003B, 0x00000031, 0x00000067, 0x00000007, 0x00040008, 0x00000001, 0x0000009C, 
0x00000000, 0x0004003D, 0x0000000A, 0x00000050, 0x0000004F, 0x0007004F, 0x00000030, 0x00000051, 0x00000050, 0x00000050, 0x00000000, 0x00000001, 
0x0003003E, 0x0000004D, 0x00000051, 0x00040008, 0x00000001, 0x0000009D, 0x00000000, 0x00060041, 0x0000005D, 0x0000005E, 0x0000005A, 0x0000005B, 
0x0000005C, 0x0004003D, 0x00000008, 0x0000005F, 0x0000005E, 0x00050041, 0x00000038, 0x00000060, 0x0000004D, 0x0000005C, 0x0004003D, 0x00000008, 
0x00000061, 0x00000060, 0x00050083, 0x00000008, 0x00000062, 0x0000005F, 0x00000061, 0x00050041, 0x00000038, 0x00000063, 0x0000004D, 0x0000005C, 
0x0003003E, 0x00000063, 0x00000062, 0x00040008, 0x00000001, 0x0000009E, 0x00000000, 0x0004003D, 0x00000030, 0x00000068, 0x0000004D, 0x0003003E, 
0x00000067, 0x00000068, 0x00060039, 0x00000003, 0x00000069, 0x0000004B, 0x00000066, 0x00000067, 0x0004003D, 0x0000000A, 0x0000006A, 0x00000066, 
0x0003003E, 0x00000065, 0x0000006A, 0x000100FD, 0x00010038, 0x00050036, 0x0000000B, 0x00000010, 0x00000000, 0x0000000D, 0x00030037, 0x0000000C, 
0x0000000E, 0x00030037, 0x0000000C, 0x0000000F, 0x000200F8, 0x00000011, 0x00040008, 0x00000001, 0x000000A4, 0x00000000, 0x00050041, 0x00000038, 
0x0000006C, 0x0000000E, 0x0000006B, 0x0004003D, 0x00000008, 0x0000006D, 0x0000006C, 0x00050041, 0x00000038, 0x0000006E, 0x0000000F, 0x0000006B, 
0x0004003D, 0x00000008, 0x0000006F, 0x0000006E, 0x000500B8, 0x00000007, 0x00000070, 0x0000006D, 0x0000006F, 0x000300F7, 0x00000072, 0x00000000, 
0x000400FA, 0x00000070, 0x00000071, 0x00000075, 0x000200F8, 0x00000071, 0x00040008, 0x00000001, 0x000000A6, 0x00000000, 0x0004003D, 0x0000000B, 
0x00000073, 0x0000000E, 0x000200FE, 0x00000073, 0x000200F8, 0x00000075, 0x00040008, 0x00000001, 0x000000AB, 0x00000000, 0x0004003D, 0x0000000B, 
0x00000076, 0x0000000F, 0x000200FE, 0x00000076, 0x000200F8, 0x00000072, 0x00030001, 0x0000000B, 0x00000078, 0x000200FE, 0x00000078, 0x00010038, 
0x00050036, 0x0000000B, 0x00000016, 0x00000000, 0x00000012, 0x00030037, 0x0000000C, 0x00000013, 0x00030037, 0x0000000C, 0x00000014, 0x00030037, 
0x0000000C, 0x00000015, 0x000200F8, 0x00000017, 0x0004003B, 0x0000000C, 0x00000079, 0x00000007, 0x0004003B, 0x0000000C, 0x0000007B, 0x00000007, 
0x0004003B, 0x0000000C, 0x0000007E, 0x00000007, 0x0004003B, 0x0000000C, 0x00000080, 0x00000007, 0x00040008, 0x00000001, 0x000000B4, 0x00000000, 
0x0004003D, 0x0000000B, 0x0000007A, 0x00000014, 0x0003003E, 0x00000079, 0x0000007A, 0x0004003D, 0x0000000B, 0x0000007C, 0x00000015, 0x0003003E, 
0x0000007B, 0x0000007C, 0x00060039, 0x0000000B, 0x0000007D, 0x00000010, 0x00000079, 0x0000007B, 0x0004003D, 0x0000000B, 0x0000007F, 0x00000013, 
0x0003003E, 0x0000007E, 0x0000007F, 0x0003003E, 0x00000080, 0x0000007D, 0x00060039, 0x0000000B, 0x00000081, 0x00000010, 0x0000007E, 0x00000080, 
0x000200FE, 0x00000081, 0x00010038, 0x00050036, 0x0000000B, 0x0000001E, 0x00000000, 0x00000018, 0x00030037, 0x0000000C, 0x00000019, 0x00030037, 
0x0000000C, 0x0000001A, 0x00030037, 0x0000000C, 0x0000001B, 0x00030037, 0x0000000C, 0x0000001C, 0x00030037, 0x0000000C, 0x0000001D, 0x000200F8, 
0x0000001F, 0x0004003B, 0x0000000C, 0x00000084, 0x00000007, 0x0004003B, 0x0000000C, 0x00000086, 0x00000007, 0x0004003B, 0x0000000C, 0x00000089, 
0x00000007, 0x0004003B, 0x0000000C, 0x0000008B, 0x00000007, 0x0004003B, 0x0000000C, 0x0000008D, 0x00000007, 0x0004003B, 0x0000000C, 0x00000090, 
0x00000007, 0x0004003B, 0x0000000C, 0x00000091, 0x00000007, 0x00040008, 0x00000001, 0x000000BA, 0x00000000, 0x0004003D, 0x0000000B, 0x00000085, 
0x00000019, 0x0003003E, 0x00000084, 0x00000085, 0x0004003D, 0x0000000B, 0x00000087, 0x0000001A, 0x0003003E, 0x00000086, 0x00000087, 0x00060039, 
0x0000000B, 0x00000088, 0x00000010, 0x00000084, 0x00000086, 0x0004003D, 0x0000000B, 0x0000008A, 0x0000001B, 0x0003003E, 0x00000089, 0x0000008A, 
0x0004003D, 0x0000000B, 0x0000008C, 0x0000001C, 0x0003003E, 0x0000008B, 0x0000008C, 0x0004003D, 0x0000000B, 0x0000008E, 0x0000001D, 0x0003003E, 
0x0000008D, 0x0000008E, 0x00070039, 0x0000000B, 0x0000008F, 0x00000016, 0x00000089, 0x0000008B, 0x0000008D, 0x0003003E, 0x00000090, 0x00000088, 
0x0003003E, 0x00000091, 0x0000008F, 0x00060039, 0x0000000B, 0x00000092, 0x00000010, 0x00000090, 0x00000091, 0x000200FE, 0x00000092, 0x00010038, 
0x00050036, 0x0000000B, 0x00000027, 0x00000000, 0x00000024, 0x00030037, 0x00000021, 0x00000025, 0x00030037, 0x00000023, 0x00000026, 0x000200F8, 
0x00000028, 0x0004003B, 0x0000003F, 0x0000009A, 0x00000007, 0x0004003B, 0x0000003F, 0x000000A0, 0x00000007, 0x0004003B, 0x0000000C, 0x000000A6, 
0x00000007, 0x0004003B, 0x00000038, 0x000000AD, 0x00000007, 0x0004003B, 0x0000003F, 0x000000AE, 0x00000007, 0x0004003B, 0x0000003F, 0x000000B7, 
0x00000007, 0x0004003B, 0x00000038, 0x000000C0, 0x00000007, 0x0004003B, 0x00000038, 0x000000CC, 0x00000007, 0x0004003B, 0x00000038, 0x000000D7, 
0x00000007, 0x0004003B, 0x00000038, 0x000000E3, 0x00000007, 0x0004003B, 0x00000038, 0x000000EE, 0x00000007, 0x0004003B, 0x00000038, 0x000000F9, 
0x00000007, 0x0004003B, 0x00000038, 0x00000104, 0x00000007, 0x0004003B, 0x00000038, 0x0000010D, 0x00000007, 0x0004003B, 0x0000003F, 0x00000119, 
0x00000007, 0x0004003B, 0x0000012A, 0x0000013B, 0x00000007, 0x00040008, 0x00000001, 0x000000C0, 0x00000000, 0x00050041, 0x0000003F, 0x00000096, 
0x00000026, 0x0000006B, 0x0004003D, 0x00000009, 0x00000097, 0x00000096, 0x0005008E, 0x00000009, 0x00000098, 0x00000097, 0x00000095, 0x00050041, 
0x0000003F, 0x00000099, 0x00000026, 0x0000006B, 0x0003003E, 0x00000099, 0x00000098, 0x00040008, 0x00000001, 0x000000C2, 0x00000000, 0x00050041, 
0x0000003F, 0x0000009B, 0x00000026, 0x0000005B, 0x0004003D, 0x00000009, 0x0000009C, 0x0000009B, 0x00050041, 0x0000003F, 0x0000009D, 0x00000026, 
0x0000006B, 0x0004003D, 0x00000009, 0x0000009E, 0x0000009D, 0x00050083, 0x00000009, 0x0000009F, 0x0000009C, 0x0000009E, 0x0003003E, 0x0000009A, 
0x0000009F, 0x00040008, 0x00000001, 0x000000C4, 0x00000000, 0x00050041, 0x0000003F, 0x000000A1, 0x00000026, 0x0000005B, 0x0004003D, 0x00000009, 
0x000000A2, 0x000000A1, 0x00050041, 0x0000003F, 0x000000A3, 0x00000026, 0x0000006B, 0x0004003D, 0x00000009, 0x000000A4, 0x000000A3, 0x00050081, 
0x00000009, 0x000000A5, 0x000000A2, 0x000000A4, 0x0003003E, 0x000000A0, 0x000000A5, 0x00040008, 0x00000001, 0x000000C6, 0x00000000, 0x0003003E, 
0x000000A6, 0x000000AC, 0x00040008, 0x00000001, 0x000000C8, 0x00000000, 0x0003003E, 0x000000AD, 0x000000A8, 0x00040008, 0x00000001, 0x000000CA, 
0x00000000, 0x00050041, 0x0000003F, 0x000000AF, 0x00000025, 0x0000005B, 0x0004003D, 0x00000009, 0x000000B0, 0x000000AF, 0x0004003D, 0x00000009, 
0x000000B1, 0x0000009A, 0x00050083, 0x00000009, 0x000000B2, 0x000000B0, 0x000000B1, 0x00050041, 0x0000003F, 0x000000B3, 0x00000025, 0x0000006B, 
0x0004003D, 0x00000009, 0x000000B4, 0x000000B3, 0x0004007F, 0x00000009, 0x000000B5, 0x000000B4, 0x00050088, 0x00000009, 0x000000B6, 0x000000B2, 
0x000000B5, 0x0003003E, 0x000000AE, 0x000000B6, 0x00040008, 0x00000001, 0x000000CC, 0x00000000, 0x00050041, 0x0000003F, 0x000000B8, 0x00000025, 
0x0000005B, 0x0004003D, 0x00000009, 0x000000B9, 0x000000B8, 0x0004003D, 0x00000009, 0x000000BA, 0x000000A0, 0x00050083, 0x00000009, 0x000000BB, 
0x000000B9, 0x000000BA, 0x00050041, 0x0000003F, 0x000000BC, 0x00000025, 0x0000006B, 0x0004003D, 0x00000009, 0x000000BD, 0x000000BC, 0x0004007F, 
0x00000009, 0x000000BE, 0x000000BD, 0x00050088, 0x00000009, 0x000000BF, 0x000000BB, 0x000000BE, 0x0003003E, 0x000000B7, 0x000000BF, 0x00040008, 
0x00000001, 0x000000CD, 0x00000000, 0x00050041, 0x00000038, 0x000000C2, 0x000000AE, 0x000000C1, 0x0004003D, 0x00000008, 0x000000C3, 0x000000C2, 
0x000500BC, 0x00000007, 0x000000C4, 0x000000C3, 0x000000A9, 0x000300F7, 0x000000C6, 0x00000000, 0x000400FA, 0x000000C4, 0x000000C5, 0x000000C7, 
0x000200F8, 0x000000C5, 0x0003003E, 0x000000C0, 0x000000A8, 0x000200F9, 0x000000C6, 0x000200F8, 0x000000C7, 0x00050041, 0x00000038, 0x000000C8, 
0x000000AE, 0x000000C1, 0x0004003D, 0x00000008, 0x000000C9, 0x000000C8, 0x0003003E, 0x000000C0, 0x000000C9, 0x000200F9, 0x000000C6, 0x000200F8, 
0x000000C6, 0x0004003D, 0x00000008, 0x000000CA, 0x000000C0, 0x00050041, 0x00000038, 0x000000CB, 0x000000AE, 0x000000C1, 0x0003003E, 0x000000CB, 
0x000000CA, 0x00040008, 0x00000001, 0x000000CE, 0x00000000, 0x00050041, 0x00000038, 0x000000CD, 0x000000AE, 0x0000005C, 0x0004003D, 0x00000008, 
0x000000CE, 0x000000CD, 0x000500BC, 0x00000007, 0x000000CF, 0x000000CE, 0x000000A9, 0x000300F7, 0x000000D1, 0x00000000, 0x000400FA, 0x000000CF, 
0x000000D0, 0x000000D2, 0x000200F8, 0x000000D0, 0x0003003E, 0x000000CC, 0x000000A8, 0x000200F9, 0x000000D1, 0x000200F8, 0x000000D2, 0x00050041, 
0x00000038, 0x000000D3, 0x000000AE, 0x0000005C, 0x0004003D, 0x00000008, 0x000000D4, 0x000000D3, 0x0003003E, 0x000000CC, 0x000000D4, 0x000200F9, 
0x000000D1, 0x000200F8, 0x000000D1, 0x0004003D, 0x00000008, 0x000000D5, 0x000000CC, 0x00050041, 0x00000038, 0x000000D6, 0x000000AE, 0x0000005C, 
0x0003003E, 0x000000D6, 0x000000D5, 0x00040008, 0x00000001, 0x000000CF, 0x00000000, 0x00050041, 0x00000038, 0x000000D9, 0x000000AE, 0x000000D8, 
0x0004003D, 0x00000008, 0x000000DA, 0x000000D9, 0x000500BC, 0x00000007, 0x000000DB, 0x000000DA, 0x000000A9, 0x000300F7, 0x000000DD, 0x00000000, 
0x000400FA, 0x000000DB, 0x000000DC, 0x000000DE, 0x000200F8, 0x000000DC, 0x0003003E, 0x000000D7, 0x000000A8, 0x000200F9, 0x000000DD, 0x000200F8, 
0x000000DE, 0x00050041, 0x00000038, 0x000000DF, 0x000000AE, 0x000000D8, 0x0004003D, 0x00000008, 0x000000E0, 0x000000DF, 0x0003003E, 0x000000D7, 
0x000000E0, 0x000200F9, 0x000000DD, 0x000200F8, 0x000000DD, 0x0004003D, 0x00000008, 0x000000E1, 0x000000D7, 0x00050041, 0x00000038, 0x000000E2, 
0x000000AE, 0x000000D8, 0x0003003E, 0x000000E2, 0x000000E1, 0x00040008, 0x00000001, 0x000000D0, 0x00000000, 0x00050041, 0x00000038, 0x000000E4, 
0x000000B7, 0x000000C1, 0x0004003D, 0x00000008, 0x000000E5, 0x000000E4, 0x000500BC, 0x00000007, 0x000000E6, 0x000000E5, 0x000000A9, 0x000300F7, 
0x000000E8, 0x00000000, 0x000400FA, 0x000000E6, 0x000000E7, 0x000000E9, 0x000200F8, 0x000000E7, 0x0003003E, 0x000000E3, 0x000000A8, 0x000200F9, 
0x000000E8, 0x000200F8, 0x000000E9, 0x00050041, 0x00000038, 0x000000EA, 0x000000B7, 0x000000C1, 0x0004003D, 0x00000008, 0x000000EB, 0x000000EA, 
0x0003003E, 0x000000E3, 0x000000EB, 0x000200F9, 0x000000E8, 0x000200F8, 0x000000E8, 0x0004003D, 0x00000008, 0x000000EC, 0x000000E3, 0x00050041, 
0x00000038, 0x000000ED, 0x000000B7, 0x000000C1, 0x0003003E, 0x000000ED, 0x000000EC, 0x00040008, 0x00000001, 0x000000D1, 0x00000000, 0x00050041, 
0x00000038, 0x000000EF, 0x000000B7, 0x0000005C, 0x0004003D, 0x00000008, 0x000000F0, 0x000000EF, 0x000500BC, 0x00000007, 0x000000F1, 0x000000F0, 
0x000000A9, 0x000300F7, 0x000000F3, 0x00000000, 0x000400FA, 0x000000F1, 0x000000F2, 0x000000F4, 0x000200F8, 0x000000F2, 0x0003003E, 0x000000EE, 
0x000000A8, 0x000200F9, 0x000000F3, 0x000200F8, 0x000000F4, 0x00050041, 0x00000038, 0x000000F5, 0x000000B7, 0x0000005C, 0x0004003D, 0x00000008, 
0x000000F6, 0x000000F5, 0x0003003E, 0x000000EE, 0x000000F6, 0x000200F9, 0x000000F3, 0x000200F8, 0x000000F3, 0x0004003D, 0x00000008, 0x000000F7, 
0x000000EE, 0x00050041, 0x00000038, 0x000000F8, 0x000000B7, 0x0000005C, 0x0003003E, 0x000000F8, 0x000000F7, 0x00040008, 0x00000001, 0x000000D2, 
0x00000000, 0x00050041, 0x00000038, 0x000000FA, 0x000000B7, 0x000000D8, 0x0004003D, 0x00000008, 0x000000FB, 0x000000FA, 0x000500BC, 0x00000007, 
0x000000FC, 0x000000FB, 0x000000A9, 0x000300F7, 0x000000FE, 0x00000000, 0x000400FA, 0x000000FC, 0x000000FD, 0x000000FF, 0x000200F8, 0x000000FD, 
0x0003003E, 0x000000F9, 0x000000A8, 0x000200F9, 0x000000FE, 0x000200F8, 0x000000FF, 0x00050041, 0x00000038, 0x00000100, 0x000000B7, 0x000000D8, 
0x0004003D, 0x00000008, 0x00000101, 0x00000100, 0x0003003E, 0x000000F9, 0x00000101, 0x000200F9, 0x000000FE, 0x000200F8, 0x000000FE, 0x0004003D, 
0x00000008, 0x00000102, 0x000000F9, 0x00050041, 0x00000038, 0x00000103, 0x000000B7, 0x000000D8, 0x0003003E, 0x00000103, 0x00000102, 0x00040008, 
0x00000001, 0x000000D4, 0x00000000, 0x00050041, 0x00000038, 0x00000105, 0x000000AE, 0x000000C1, 0x0004003D, 0x00000008, 0x00000106, 0x00000105, 
0x00050041, 0x00000038, 0x00000107, 0x000000AE, 0x0000005C, 0x0004003D, 0x00000008, 0x00000108, 0x00000107, 0x00050041, 0x00000038, 0x00000109, 
0x000000AE, 0x000000D8, 0x0004003D, 0x00000008, 0x0000010A, 0x00000109, 0x0007000C, 0x00000008, 0x0000010B, 0x00000002, 0x00000025, 0x00000108, 
0x0000010A, 0x0007000C, 0x00000008, 0x0000010C, 0x00000002, 0x00000025, 0x00000106, 0x0000010B, 0x0003003E, 0x00000104, 0x0000010C, 0x00040008, 
0x00000001, 0x000000D6, 0x00000000, 0x00050041, 0x00000038, 0x0000010E, 0x000000B7, 0x000000C1, 0x0004003D, 0x00000008, 0x0000010F, 0x0000010E, 
0x00050041, 0x00000038, 0x00000110, 0x000000B7, 0x0000005C, 0x0004003D, 0x00000008, 0x00000111, 0x00000110, 0x00050041, 0x00000038, 0x00000112, 
0x000000B7, 0x000000D8, 0x0004003D, 0x00000008, 0x00000113, 0x00000112, 0x0007000C, 0x00000008, 0x00000114, 0x00000002, 0x00000025, 0x00000111, 
0x00000113, 0x0007000C, 0x00000008, 0x00000115, 0x00000002, 0x00000025, 0x0000010F, 0x00000114, 0x0003003E, 0x0000010D, 0x00000115, 0x00040008, 
0x00000001, 0x000000D7, 0x00000000, 0x0004003D, 0x00000008, 0x00000116, 0x00000104, 0x0004003D, 0x00000008, 0x00000117, 0x0000010D, 0x0007000C, 
0x00000008, 0x00000118, 0x00000002, 0x00000025, 0x00000116, 0x00000117, 0x0003003E, 0x000000AD, 0x00000118, 0x00040008, 0x00000001, 0x000000D9, 
0x00000000, 0x00050041, 0x0000003F, 0x0000011A, 0x00000025, 0x0000005B, 0x0004003D, 0x00000009, 0x0000011B, 0x0000011A, 0x00050041, 0x0000003F, 
0x0000011C, 0x00000025, 0x0000006B, 0x0004003D, 0x00000009, 0x0000011D, 0x0000011C, 0x0004003D, 0x00000008, 0x0000011E, 0x000000AD, 0x0005008E, 
0x00000009, 0x0000011F, 0x0000011D, 0x0000011E, 0x00050081, 0x00000009, 0x00000120, 0x0000011B, 0x0000011F, 0x0003003E, 0x00000119, 0x00000120, 
0x00040008, 0x00000001, 0x000000DA, 0x00000000, 0x00050041, 0x0000003F, 0x00000121, 0x00000026, 0x0000005B, 0x0004003D, 0x00000009, 0x00000122, 
0x00000121, 0x0004003D, 0x00000009, 0x00000123, 0x00000119, 0x00050083, 0x00000009, 0x00000124, 0x00000123, 0x00000122, 0x0003003E, 0x00000119, 
0x00000124, 0x00040008, 0x00000001, 0x000000DB, 0x00000000, 0x0004003D, 0x00000008, 0x00000125, 0x000000AD, 0x000500B6, 0x00000007, 0x00000126, 
0x00000125, 0x000000A8, 0x000300F7, 0x00000128, 0x00000000, 0x000400FA, 0x00000126, 0x00000127, 0x00000128, 0x000200F8, 0x00000127, 0x00040008, 
0x00000001, 0x000000DD, 0x00000000, 0x00050041, 0x0000012A, 0x0000012B, 0x000000A6, 0x0000005B, 0x0003003E, 0x0000012B, 0x00000129, 0x00040008, 
0x00000001, 0x000000DE, 0x00000000, 0x0004003D, 0x00000008, 0x0000012C, 0x000000AD, 0x00050041, 0x00000038, 0x0000012D, 0x000000A6, 0x0000006B, 
0x0003003E, 0x0000012D, 0x0000012C, 0x00040008, 0x00000001, 0x000000DF, 0x00000000, 0x00050041, 0x00000038, 0x0000012F, 0x000000A6, 0x0000012E, 
0x0003003E, 0x0000012F, 0x000000A9, 0x00040008, 0x00000001, 0x000000E0, 0x00000000, 0x00050041, 0x00000038, 0x00000130, 0x00000119, 0x000000C1, 
0x0004003D, 0x00000008, 0x00000131, 0x00000130, 0x0006000C, 0x00000008, 0x00000132, 0x00000002, 0x00000004, 0x00000131, 0x00060041, 0x00000038, 
0x00000133, 0x00000026, 0x0000006B, 0x000000C1, 0x0004003D, 0x00000008, 0x00000134, 0x00000133, 0x00050083, 0x00000008, 0x00000135, 0x00000132, 
0x00000134, 0x0006000C, 0x00000008, 0x00000136, 0x00000002, 0x00000004, 0x00000135, 0x000500B8, 0x00000007, 0x00000138, 0x00000136, 0x00000137, 
0x000300F7, 0x0000013A, 0x00000000, 0x000400FA, 0x00000138, 0x00000139, 0x0000013A, 0x000200F8, 0x00000139, 0x00040008, 0x00000001, 0x000000E3, 
0x00000000, 0x00050041, 0x00000038, 0x0000013C, 0x00000119, 0x000000C1, 0x0004003D, 0x00000008, 0x0000013D, 0x0000013C, 0x000500B8, 0x00000007, 
0x0000013E, 0x0000013D, 0x000000A9, 0x0003003E, 0x0000013B, 0x0000013E, 0x00040008, 0x00000001, 0x000000E4, 0x00000000, 0x0004003D, 0x00000007, 
0x00000140, 0x0000013B, 0x000600A9, 0x00000052, 0x00000142, 0x00000140, 0x0000006B, 0x00000141, 0x0004006F, 0x00000008, 0x00000143, 0x00000142, 
0x00060050, 0x00000009, 0x00000144, 0x00000143, 0x000000A9, 0x000000A9, 0x00050041, 0x0000003F, 0x00000145, 0x000000A6, 0x0000013F, 0x0003003E, 
0x00000145, 0x00000144, 0x00040008, 0x00000001, 0x000000E5, 0x00000000, 0x0004003D, 0x00000007, 0x00000147, 0x0000013B, 0x00070050, 0x0000014B, 
0x0000014C, 0x00000147, 0x00000147, 0x00000147, 0x00000147, 0x000600A9, 0x0000000A, 0x0000014D, 0x0000014C, 0x00000149, 0x0000014A, 0x00050041, 
0x00000047, 0x0000014E, 0x000000A6, 0x00000146, 0x0003003E, 0x0000014E, 0x0000014D, 0x00040008, 0x00000001, 0x000000E6, 0x00000000, 0x0004003D, 
0x0000000B, 0x0000014F, 0x000000A6, 0x000200FE, 0x0000014F, 0x000200F8, 0x0000013A, 0x00040008, 0x00000001, 0x000000EA, 0x00000000, 0x00050041, 
0x00000038, 0x00000151, 0x00000119, 0x000000D8, 0x0004003D, 0x00000008, 0x00000152, 0x00000151, 0x0006000C, 0x00000008, 0x00000153, 0x00000002, 
0x00000004, 0x00000152, 0x00060041, 0x00000038, 0x00000154, 0x00000026, 0x0000006B, 0x000000D8, 0x0004003D, 0x00000008, 0x00000155, 0x00000154, 
0x00050083, 0x00000008, 0x00000156, 0x00000153, 0x00000155, 0x0006000C, 0x00000008, 0x00000157, 0x00000002, 0x00000004, 0x00000156, 0x000500B8, 
0x00000007, 0x00000158, 0x00000157, 0x00000137, 0x000300F7, 0x0000015A, 0x00000000, 0x000400FA, 0x00000158, 0x00000159, 0x0000015A, 0x000200F8, 
0x00000159, 0x00040008, 0x00000001, 0x000000EC, 0x00000000, 0x00050041, 0x00000038, 0x0000015B, 0x00000119, 0x000000D8, 0x0004003D, 0x00000008, 
0x0000015C, 0x0000015B, 0x0006000C, 0x00000008, 0x0000015D, 0x00000002, 0x00000006, 0x0000015C, 0x0004007F, 0x00000008, 0x0000015E, 0x0000015D, 
0x00060050, 0x00000009, 0x0000015F, 0x000000A9, 0x000000A9, 0x0000015E, 0x00050041, 0x0000003F, 0x00000160, 0x000000A6, 0x0000013F, 0x0003003E, 
0x00000160, 0x0000015F, 0x00040008, 0x00000001, 0x000000ED, 0x00000000, 0x00050041, 0x00000047, 0x00000162, 0x000000A6, 0x00000146, 0x0003003E, 
0x00000162, 0x00000161, 0x00040008, 0x00000001, 0x000000EE, 0x00000000, 0x0004003D, 0x0000000B, 0x00000163, 0x000000A6, 0x000200FE, 0x00000163, 
0x000200F8, 0x0000015A, 0x00040008, 0x00000001, 0x000000F2, 0x00000000, 0x00050041, 0x00000038, 0x00000165, 0x00000119, 0x0000005C, 0x0004003D, 
0x00000008, 0x00000166, 0x00000165, 0x0006000C, 0x00000008, 0x00000167, 0x00000002, 0x00000006, 0x00000166, 0x0004007F, 0x00000008, 0x00000168, 
0x00000167, 0x00060050, 0x00000009, 0x00000169, 0x000000A9, 0x00000168, 0x000000A9, 0x00050041, 0x0000003F, 0x0000016A, 0x000000A6, 0x0000013F, 
0x0003003E, 0x0000016A, 0x00000169, 0x00040008, 0x00000001, 0x000000F3, 0x00000000, 0x00050041, 0x00000038, 0x0000016C, 0x00000119, 0x0000005C, 
0x0004003D, 0x00000008, 0x0000016D, 0x0000016C, 0x0006000C, 0x00000008, 0x0000016E, 0x00000002, 0x00000006, 0x0000016D, 0x0004003D, 0x00000008, 
0x00000171, 0x00000170, 0x00050085, 0x00000008, 0x00000173, 0x00000171, 0x00000172, 0x0006000C, 0x00000008, 0x00000174, 0x00000002, 0x0000000D, 
0x00000173, 0x00050081, 0x00000008, 0x00000176, 0x00000174, 0x00000175, 0x0008000C, 0x00000008, 0x00000179, 0x00000002, 0x0000002B, 0x00000176, 
0x00000177, 0x00000178, 0x00050085, 0x00000008, 0x0000017A, 0x0000016E, 0x00000179, 0x00070050, 0x0000000A, 0x0000017B, 0x0000016B, 0x0000016B, 
0x0000016B, 0x0000017A, 0x00050041, 0x00000047, 0x0000017C, 0x000000A6, 0x00000146, 0x0003003E, 0x0000017C, 0x0000017B, 0x00040008, 0x00000001, 
0x000000F4, 0x00000000, 0x0004003D, 0x0000000B, 0x0000017D, 0x000000A6, 0x000200FE, 0x0000017D, 0x000200F8, 0x00000128, 0x00040008, 0x00000001, 
0x000000F8, 0x00000000, 0x0004003D, 0x0000000B, 0x0000017F, 0x000000A6, 0x000200FE, 0x0000017F, 0x00010038, 0x00050036, 0x0000000B, 0x0000002E, 
0x00000000, 0x0000002B, 0x00030037, 0x00000021, 0x0000002C, 0x00030037, 0x0000002A, 0x0000002D, 0x000200F8, 0x0000002F, 0x0004003B, 0x0000003F, 
0x00000182, 0x00000007, 0x0004003B, 0x00000038, 0x00000188, 0x00000007, 0x0004003B, 0x00000038, 0x00000192, 0x00000007, 0x0004003B, 0x00000038, 
0x000001A5, 0x00000007, 0x00040008, 0x00000001, 0x000000FF, 0x00000000, 0x00050041, 0x0000003F, 0x00000183, 0x0000002C, 0x0000005B, 0x0004003D, 
0x00000009, 0x00000184, 0x00000183, 0x00050041, 0x0000003F, 0x00000185, 0x0000002D, 0x0000005B, 0x0004003D, 0x00000009, 0x00000186, 0x00000185, 
0x00050083, 0x00000009, 0x00000187, 0x00000184, 0x00000186, 0x0003003E, 0x00000182, 0x00000187, 0x00040008, 0x00000001, 0x00000101, 0x00000000, 
0x0004003D, 0x00000009, 0x00000189, 0x00000182, 0x00050041, 0x0000003F, 0x0000018A, 0x0000002C, 0x0000006B, 0x0004003D, 0x00000009, 0x0000018B, 
0x0000018A, 0x00050094, 0x00000008, 0x0000018C, 0x00000189, 0x0000018B, 0x0003003E, 0x00000188, 0x0000018C, 0x00040008, 0x00000001, 0x00000102, 
0x00000000, 0x0004003D, 0x00000008, 0x0000018D, 0x00000188, 0x000500BA, 0x00000007, 0x0000018E, 0x0000018D, 0x000000A9, 0x000300F7, 0x00000190, 
0x00000000, 0x000400FA, 0x0000018E, 0x0000018F, 0x00000190, 0x000200F8, 0x0000018F, 0x00040008, 0x00000001, 0x00000104, 0x00000000, 0x000200FE, 
0x000000AC, 0x000200F8, 0x00000190, 0x00040008, 0x00000001, 0x00000108, 0x00000000, 0x00050041, 0x00000038, 0x00000193, 0x0000002D, 0x0000006B, 
0x0004003D, 0x00000008, 0x00000194, 0x00000193, 0x00050041, 0x00000038, 0x00000195, 0x0000002D, 0x0000006B, 0x0004003D, 0x00000008, 0x00000196, 
0x00000195, 0x00050085, 0x00000008, 0x00000197, 0x00000194, 0x00000196, 0x0004003D, 0x00000009, 0x00000198, 0x00000182, 0x0004003D, 0x00000009, 
0x00000199, 0x00000182, 0x00050094, 0x00000008, 0x0000019A, 0x00000198, 0x00000199, 0x0004003D, 0x00000008, 0x0000019B, 0x00000188, 0x0004003D, 
0x00000008, 0x0000019C, 0x00000188, 0x00050085, 0x00000008, 0x0000019D, 0x0000019B, 0x0000019C, 0x00050083, 0x00000008, 0x0000019E, 0x0000019A, 
0x0000019D, 0x00050083, 0x00000008, 0x0000019F, 0x00000197, 0x0000019E, 0x0003003E, 0x00000192, 0x0000019F, 0x00040008, 0x00000001, 0x00000109, 
0x00000000, 0x0004003D, 0x00000008, 0x000001A0, 0x00000192, 0x000500B8, 0x00000007, 0x000001A1, 0x000001A0, 0x000000A9, 0x000300F7, 0x000001A3, 
0x00000000, 0x000400FA, 0x000001A1, 0x000001A2, 0x000001A3, 0x000200F8, 0x000001A2, 0x00040008, 0x00000001, 0x0000010B, 0x00000000, 0x000200FE, 
0x000000AC, 0x000200F8, 0x000001A3, 0x00040008, 0x00000001, 0x0000010F, 0x00000000, 0x0004003D, 0x00000009, 0x000001A6, 0x00000182, 0x0006000C, 
0x00000008, 0x000001A7, 0x00000002, 0x00000042, 0x000001A6, 0x0004003D, 0x00000008, 0x000001A8, 0x00000192, 0x0006000C, 0x00000008, 0x000001A9, 
0x00000002, 0x0000001F, 0x000001A8, 0x00050083, 0x00000008, 0x000001AA, 0x000001A7, 0x000001A9, 0x0003003E, 0x000001A5, 0x000001AA, 0x00040008, 
0x00000001, 0x00000110, 0x00000000, 0x0004003D, 0x00000008, 0x000001AB, 0x000001A5, 0x00050041, 0x0000003F, 0x000001AC, 0x0000002C, 0x0000005B, 
0x0004003D, 0x00000009, 0x000001AD, 0x000001AC, 0x00050041, 0x0000003F, 0x000001AE, 0x0000002C, 0x0000006B, 0x0004003D, 0x00000009, 0x000001AF, 
0x000001AE, 0x0004003D, 0x00000008, 0x000001B0, 0x000001A5, 0x0005008E, 0x00000009, 0x000001B1, 0x000001AF, 0x000001B0, 0x00050081, 0x00000009, 
0x000001B2, 0x000001AD, 0x000001B1, 0x00050041, 0x0000003F, 0x000001B3, 0x0000002D, 0x0000005B, 0x0004003D, 0x00000009, 0x000001B4, 0x000001B3, 
0x00050083, 0x00000009, 0x000001B5, 0x000001B2, 0x000001B4, 0x0006000C, 0x00000009, 0x000001B6, 0x00000002, 0x00000045, 0x000001B5, 0x00050041, 
0x0000005D, 0x000001B7, 0x0000005A, 0x0000006B, 0x0004003D, 0x00000008, 0x000001B8, 0x000001B7, 0x00050085, 0x00000008, 0x000001BA, 0x000001B8, 
0x000001B9, 0x0006000C, 0x00000008, 0x000001BB, 0x00000002, 0x0000000D, 0x000001BA, 0x00050085, 0x00000008, 0x000001BC, 0x000001BB, 0x00000095, 
0x00050081, 0x00000008, 0x000001BD, 0x000001BC, 0x00000095, 0x00080050, 0x0000000B, 0x000001BE, 0x00000129, 0x000001AB, 0x000001B6, 0x000000AB, 
0x000001BD, 0x000200FE, 0x000001BE, 0x00010038, 0x00050036, 0x00000008, 0x00000034, 0x00000000, 0x00000032, 0x00030037, 0x00000031, 0x00000033, 
0x000200F8, 0x00000035, 0x00040008, 0x00000001, 0x00000116, 0x00000000, 0x0004003D, 0x00000030, 0x000001C1, 0x00000033, 0x00050094, 0x00000008, 
0x000001C5, 0x000001C1, 0x000001C4, 0x0006000C, 0x00000008, 0x000001C6, 0x00000002, 0x0000000D, 0x000001C5, 0x00050085, 0x00000008, 0x000001C8, 
0x000001C6, 0x000001C7, 0x0006000C, 0x00000008, 0x000001C9, 0x00000002, 0x0000000A, 0x000001C8, 0x000200FE, 0x000001C9, 0x00010038, 0x00050036, 
0x0000000A, 0x0000003D, 0x00000000, 0x00000039, 0x00030037, 0x00000037, 0x0000003A, 0x00030037, 0x00000031, 0x0000003B, 0x00030037, 0x00000038, 
0x0000003C, 0x000200F8, 0x0000003E, 0x0004003B, 0x0000003F, 0x000001CC, 0x00000007, 0x0004003B, 0x00000047, 0x000001CF, 0x00000007, 0x0004003B, 
0x000001D0, 0x000001D1, 0x00000007, 0x0004003B, 0x000001D0, 0x000001D6, 0x00000007, 0x0004003B, 0x00000021, 0x000001E5, 0x00000007, 0x0004003B, 
0x00000031, 0x000001E8, 0x00000007, 0x0004003B, 0x00000031, 0x000001F5, 0x00000007, 0x0004003B, 0x00000031, 0x00000201, 0x00000007, 0x0004003B, 
0x00000047, 0x00000211, 0x00000007, 0x0004003B, 0x000001D0, 0x00000216, 0x00000007, 0x0004003B, 0x000001D0, 0x0000021B, 0x00000007, 0x0004003B, 
0x0000000C, 0x00000224, 0x00000007, 0x0004003B, 0x00000021, 0x0000022B, 0x00000007, 0x0004003B, 0x00000023, 0x0000022D, 0x00000007, 0x0004003B, 
0x0000000C, 0x0000022F, 0x00000007, 0x0004003B, 0x00000021, 0x00000233, 0x00000007, 0x0004003B, 0x0000002A, 0x00000235, 0x00000007, 0x0004003B, 
0x0000000C, 0x0000023A, 0x00000007, 0x0004003B, 0x00000021, 0x0000023F, 0x00000007, 0x0004003B, 0x0000002A, 0x00000241, 0x00000007, 0x0004003B, 
0x0000000C, 0x00000246, 0x00000007, 0x0004003B, 0x00000021, 0x00000249, 0x00000007, 0x0004003B, 0x0000002A, 0x0000024B, 0x00000007, 0x0004003B, 
0x0000000C, 0x00000256, 0x00000007, 0x0004003B, 0x00000021, 0x0000025B, 0x00000007, 0x0004003B, 0x0000002A, 0x0000025D, 0x00000007, 0x0004003B, 
0x0000000C, 0x00000267, 0x00000007, 0x0004003B, 0x0000000C, 0x00000268, 0x00000007, 0x0004003B, 0x0000000C, 0x0000026A, 0x00000007, 0x0004003B, 
0x0000000C, 0x0000026C, 0x00000007, 0x0004003B, 0x0000000C, 0x0000026E, 0x00000007, 0x0004003B, 0x0000000C, 0x00000270, 0x00000007, 0x0004003B, 
0x0000003F, 0x0000029A, 0x00000007, 0x0004003B, 0x00000031, 0x000002A0, 0x00000007, 0x0004003B, 0x00000031, 0x000002A7, 0x00000007, 0x0004003B, 
0x00000031, 0x000002AE, 0x00000007, 0x0004003B, 0x0000003F, 0x000002E6, 0x00000007, 0x0004003B, 0x0000003F, 0x000002EC, 0x00000007, 0x0004003B, 
0x00000038, 0x000002F2, 0x00000007, 0x0004003B, 0x00000031, 0x000002FC, 0x00000007, 0x00040008, 0x00000001, 0x0000011D, 0x00000000, 0x00050041, 
0x0000003F, 0x000001CD, 0x0000003A, 0x0000005B, 0x0004003D, 0x00000009, 0x000001CE, 0x000001CD, 0x0003003E, 0x000001CC, 0x000001CE, 0x00040008, 
0x00000001, 0x0000011F, 0x00000000, 0x0003003E, 0x000001CF, 0x000000AB, 0x00040008, 0x00000001, 0x00000121, 0x00000000, 0x0004003D, 0x00000008, 
0x000001D3, 0x0000003C, 0x00050085, 0x00000008, 0x000001D4, 0x000001D2, 0x000001D3, 0x0004006E, 0x00000052, 0x000001D5, 0x000001D4, 0x0003003E, 
0x000001D1, 0x000001D5, 0x00040008, 0x00000001, 0x00000124, 0x00000000, 0x0003003E, 0x000001D6, 0x0000005B, 0x000200F9, 0x000001D7, 0x00040008, 
0x00000001, 0x00000125, 0x00000000, 0x000200F8, 0x000001D7, 0x000400F6, 0x000001D9, 0x000001DA, 0x00000000, 0x000200F9, 0x000001DB, 0x000200F8, 
0x000001DB, 0x0004003D, 0x00000052, 0x000001DC, 0x000001D6, 0x000500B1, 0x00000007, 0x000001DE, 0x000001DC, 0x000001DD, 0x000400FA, 0x000001DE, 
0x000001D8, 0x000001D9, 0x000200F8, 0x000001D8, 0x00040008, 0x00000001, 0x00000127, 0x00000000, 0x0004003D, 0x00000052, 0x000001DF, 0x000001D6, 
0x0004003D, 0x00000052, 0x000001E0, 0x000001D1, 0x000500AA, 0x00000007, 0x000001E1, 0x000001DF, 0x000001E0, 0x000300F7, 0x000001E3, 0x00000000, 
0x000400FA, 0x000001E1, 0x000001E2, 0x000001E3, 0x000200F8, 0x000001E2, 0x00040008, 0x00000001, 0x00000129, 0x00000000, 0x000200F9, 0x000001D9, 
0x000200F8, 0x000001E3, 0x00040008, 0x00000001, 0x0000012D, 0x00000000, 0x00050041, 0x00000021, 0x000001E6, 0x0000003A, 0x0000006B, 0x0004003D, 
0x00000020, 0x000001E7, 0x000001E6, 0x0003003E, 0x000001E5, 0x000001E7, 0x00040008, 0x00000001, 0x0000012E, 0x00000000, 0x0004003D, 0x00000030, 
0x000001E9, 0x0000003B, 0x0003003E, 0x000001E8, 0x000001E9, 0x00050039, 0x00000008, 0x000001EA, 0x00000034, 0x000001E8, 0x00050085, 0x00000008, 
0x000001EC, 0x000001EA, 0x000001EB, 0x00050083, 0x00000008, 0x000001ED, 0x000001EC, 0x0000016B, 0x0004003D, 0x00000008, 0x000001EF, 0x000001EE, 
0x00050085, 0x00000008, 0x000001F0, 0x000001ED, 0x000001EF, 0x00060041, 0x00000038, 0x000001F1, 0x000001E5, 0x0000006B, 0x000000C1, 0x0004003D, 
0x00000008, 0x000001F2, 0x000001F1, 0x00050081, 0x00000008, 0x000001F3, 0x000001F2, 0x000001F0, 0x00060041, 0x00000038, 0x000001F4, 0x000001E5, 
0x0000006B, 0x000000C1, 0x0003003E, 0x000001F4, 0x000001F3, 0x00040008, 0x00000001, 0x0000012F, 0x00000000, 0x0004003D, 0x00000030, 0x000001F6, 
0x0000003B, 0x0007004F, 0x00000030, 0x000001F7, 0x000001F6, 0x000001F6, 0x00000001, 0x00000000, 0x0003003E, 0x000001F5, 0x000001F7, 0x00050039, 
0x00000008, 0x000001F8, 0x00000034, 0x000001F5, 0x00050085, 0x00000008, 0x000001F9, 0x000001F8, 0x000001EB, 0x00050083, 0x00000008, 0x000001FA, 
0x000001F9, 0x0000016B, 0x0004003D, 0x00000008, 0x000001FB, 0x000001EE, 0x00050085, 0x00000008, 0x000001FC, 0x000001FA, 0x000001FB, 0x00060041, 
0x00000038, 0x000001FD, 0x000001E5, 0x0000006B, 0x0000005C, 0x0004003D, 0x00000008, 0x000001FE, 0x000001FD, 0x00050081, 0x00000008, 0x000001FF, 
0x000001FE, 0x000001FC, 0x00060041, 0x00000038, 0x00000200, 0x000001E5, 0x0000006B, 0x0000005C, 0x0003003E, 0x00000200, 0x000001FF, 0x00040008, 
0x00000001, 0x00000130, 0x00000000, 0x0004003D, 0x00000030, 0x00000202, 0x0000003B, 0x0007004F, 0x00000030, 0x00000203, 0x00000202, 0x00000202, 
0x00000000, 0x00000000, 0x0003003E, 0x00000201, 0x00000203, 0x00050039, 0x00000008, 0x00000204, 0x00000034, 0x00000201, 0x00050085, 0x00000008, 
0x00000205, 0x00000204, 0x000001EB, 0x00050083, 0x00000008, 0x00000206, 0x00000205, 0x0000016B, 0x0004003D, 0x00000008, 0x00000207, 0x000001EE, 
0x00050085, 0x00000008, 0x00000208, 0x00000206, 0x00000207, 0x00060041, 0x00000038, 0x00000209, 0x000001E5, 0x0000006B, 0x000000D8, 0x0004003D, 
0x00000008, 0x0000020A, 0x00000209, 0x00050081, 0x00000008, 0x0000020B, 0x0000020A, 0x00000208, 0x00060041, 0x00000038, 0x0000020C, 0x000001E5, 
0x0000006B, 0x000000D8, 0x0003003E, 0x0000020C, 0x0000020B, 0x00040008, 0x00000001, 0x00000131, 0x00000000, 0x00050041, 0x0000003F, 0x0000020D, 
0x000001E5, 0x0000006B, 0x0004003D, 0x00000009, 0x0000020E, 0x0000020D, 0x0006000C, 0x00000009, 0x0000020F, 0x00000002, 0x00000045, 0x0000020E, 
0x00050041, 0x0000003F, 0x00000210, 0x000001E5, 0x0000006B, 0x0003003E, 0x00000210, 0x0000020F, 0x00040008, 0x00000001, 0x00000133, 0x00000000, 
0x0003003E, 0x00000211, 0x00000212, 0x00040008, 0x00000001, 0x00000134, 0x00000000, 0x0004003D, 0x00000030, 0x00000213, 0x0000003B, 0x00050050, 
0x00000030, 0x00000214, 0x0000016B, 0x0000016B, 0x00050081, 0x00000030, 0x00000215, 0x00000213, 0x00000214, 0x0003003E, 0x0000003B, 0x00000215, 
0x00040008, 0x00000001, 0x00000136, 0x00000000, 0x0004003D, 0x00000008, 0x00000218, 0x0000003C, 0x00050085, 0x00000008, 0x00000219, 0x00000217, 
0x00000218, 0x0004006E, 0x00000052, 0x0000021A, 0x00000219, 0x0003003E, 0x00000216, 0x0000021A, 0x00040008, 0x00000001, 0x00000139, 0x00000000, 
0x0003003E, 0x0000021B, 0x0000005B, 0x000200F9, 0x0000021C, 0x00040008, 0x00000001, 0x0000013A, 0x00000000, 0x000200F8, 0x0000021C, 0x000400F6, 
0x0000021E, 0x0000021F, 0x00000000, 0x000200F9, 0x00000220, 0x000200F8, 0x00000220, 0x0004003D, 0x00000052, 0x00000221, 0x0000021B, 0x000500B1, 
0x00000007, 0x00000223, 0x00000221, 0x00000222, 0x000400FA, 0x00000223, 0x0000021D, 0x0000021E, 0x000200F8, 0x0000021D, 0x00040008, 0x00000001, 
0x0000013D, 0x00000000, 0x0004003D, 0x00000020, 0x0000022C, 0x000001E5, 0x0003003E, 0x0000022B, 0x0000022C, 0x0003003E, 0x0000022D, 0x0000022A, 
0x00060039, 0x0000000B, 0x0000022E, 0x00000027, 0x0000022B, 0x0000022D, 0x0003003E, 0x00000224, 0x0000022E, 0x00040008, 0x00000001, 0x0000013F, 
0x00000000, 0x0004003D, 0x00000020, 0x00000234, 0x000001E5, 0x0003003E, 0x00000233, 0x00000234, 0x0003003E, 0x00000235, 0x00000232, 0x00060039, 
0x0000000B, 0x00000236, 0x0000002E, 0x00000233, 0x00000235, 0x0003003E, 0x0000022F, 0x00000236, 0x00040008, 0x00000001, 0x00000140, 0x00000000, 
0x00050041, 0x00000047, 0x00000239, 0x0000022F, 0x00000146, 0x0003003E, 0x00000239, 0x00000238, 0x00040008, 0x00000001, 0x00000142, 0x00000000, 
0x0004003D, 0x00000020, 0x00000240, 0x000001E5, 0x0003003E, 0x0000023F, 0x00000240, 0x0003003E, 0x00000241, 0x0000023E, 0x00060039, 0x0000000B, 
0x00000242, 0x0000002E, 0x0000023F, 0x00000241, 0x0003003E, 0x0000023A, 0x00000242, 0x00040008, 0x00000001, 0x00000143, 0x00000000, 0x00050041, 
0x00000047, 0x00000244, 0x0000023A, 0x00000146, 0x0003003E, 0x00000244, 0x00000243, 0x00040008, 0x00000001, 0x00000144, 0x00000000, 0x00050041, 
0x00000038, 0x00000245, 0x0000023A, 0x0000012E, 0x0003003E, 0x00000245, 0x00000175, 0x00040008, 0x00000001, 0x00000146, 0x00000000, 0x0004003D, 
0x00000020, 0x0000024A, 0x000001E5, 0x0003003E, 0x00000249, 0x0000024A, 0x0003003E, 0x0000024B, 0x00000248, 0x00060039, 0x0000000B, 0x0000024C, 
0x0000002E, 0x00000249, 0x0000024B, 0x0003003E, 0x00000246, 0x0000024C, 0x00040008, 0x00000001, 0x00000147, 0x00000000, 0x0004003D, 0x00000008, 
0x0000024E, 0x00000170, 0x00050085, 0x00000008, 0x0000024F, 0x0000024E, 0x00000237, 0x0006000C, 0x00000008, 0x00000250, 0x00000002, 0x0000000D, 
0x0000024F, 0x00050085, 0x00000008, 0x00000252, 0x00000250, 0x00000251, 0x00070050, 0x0000000A, 0x00000253, 0x0000024D, 0x00000148, 0x00000237, 
0x00000252, 0x00050041, 0x00000047, 0x00000254, 0x00000246, 0x00000146, 0x0003003E, 0x00000254, 0x00000253, 0x00040008, 0x00000001, 0x00000148, 
0x00000000, 0x00050041, 0x00000038, 0x00000255, 0x00000246, 0x0000012E, 0x0003003E, 0x00000255, 0x000000A9, 0x00040008, 0x00000001, 0x0000014A, 
0x00000000, 0x0004003D, 0x00000020, 0x0000025C, 0x000001E5, 0x0003003E, 0x0000025B, 0x0000025C, 0x0003003E, 0x0000025D, 0x0000025A, 0x00060039, 
0x0000000B, 0x0000025E, 0x0000002E, 0x0000025B, 0x0000025D, 0x0003003E, 0x00000256, 0x0000025E, 0x00040008, 0x00000001, 0x0000014B, 0x00000000, 
0x0004003D, 0x00000008, 0x0000025F, 0x00000170, 0x00050085, 0x00000008, 0x00000261, 0x0000025F, 0x00000260, 0x0006000C, 0x00000008, 0x00000262, 
0x00000002, 0x0000000D, 0x00000261, 0x00050085, 0x00000008, 0x00000263, 0x00000262, 0x00000251, 0x00070050, 0x0000000A, 0x00000264, 0x0000016B, 
0x00000095, 0x00000177, 0x00000263, 0x00050041, 0x00000047, 0x00000265, 0x00000256, 0x00000146, 0x0003003E, 0x00000265, 0x00000264, 0x00040008, 
0x00000001, 0x0000014C, 0x00000000, 0x00050041, 0x00000038, 0x00000266, 0x00000256, 0x0000012E, 0x0003003E, 0x00000266, 0x000000A9, 0x00040008, 
0x00000001, 0x0000014E, 0x00000000, 0x0004003D, 0x0000000B, 0x00000269, 0x00000224, 0x0003003E, 0x00000268, 0x00000269, 0x0004003D, 0x0000000B, 
0x0000026B, 0x0000022F, 0x0003003E, 0x0000026A, 0x0000026B, 0x0004003D, 0x0000000B, 0x0000026D, 0x0000023A, 0x0003003E, 0x0000026C, 0x0000026D, 
0x0004003D, 0x0000000B, 0x0000026F, 0x00000246, 0x0003003E, 0x0000026E, 0x0000026F, 0x0004003D, 0x0000000B, 0x00000271, 0x00000256, 0x0003003E, 
0x00000270, 0x00000271, 0x00090039, 0x0000000B, 0x00000272, 0x0000001E, 0x00000268, 0x0000026A, 0x0000026C, 0x0000026E, 0x00000270, 0x0003003E, 
0x00000267, 0x00000272, 0x00040008, 0x00000001, 0x0000014F, 0x00000000, 0x00050041, 0x0000012A, 0x00000273, 0x00000267, 0x0000005B, 0x0004003D, 
0x00000007, 0x00000274, 0x00000273, 0x000300F7, 0x00000276, 0x00000000, 0x000400FA, 0x00000274, 0x00000275, 0x00000319, 0x000200F8, 0x00000275, 
0x00040008, 0x00000001, 0x00000151, 0x00000000, 0x00050041, 0x00000047, 0x00000277, 0x00000267, 0x00000146, 0x0004003D, 0x0000000A, 0x00000278, 
0x00000277, 0x0004003D, 0x0000000A, 0x00000279, 0x00000211, 0x00050085, 0x0000000A, 0x0000027A, 0x00000279, 0x00000278, 0x0003003E, 0x00000211, 
0x0000027A, 0x00040008, 0x00000001, 0x00000152, 0x00000000, 0x00060041, 0x00000038, 0x0000027C, 0x00000267, 0x00000146, 0x0000027B, 0x0004003D, 
0x00000008, 0x0000027D, 0x0000027C, 0x000500BA, 0x00000007, 0x0000027E, 0x0000027D, 0x000000A9, 0x000300F7, 0x00000280, 0x00000000, 0x000400FA, 
0x0000027E, 0x0000027F, 0x00000280, 0x000200F8, 0x0000027F, 0x00040008, 0x00000001, 0x00000154, 0x00000000, 0x00050041, 0x00000047, 0x00000281, 
0x00000267, 0x00000146, 0x0004003D, 0x0000000A, 0x00000282, 0x00000281, 0x0004003D, 0x0000000A, 0x00000283, 0x00000211, 0x00050085, 0x0000000A, 
0x00000284, 0x00000282, 0x00000283, 0x00060041, 0x00000038, 0x00000285, 0x00000267, 0x00000146, 0x0000027B, 0x0004003D, 0x00000008, 0x00000286, 
0x00000285, 0x0005008E, 0x0000000A, 0x00000287, 0x00000284, 0x00000286, 0x0004003D, 0x0000000A, 0x00000288, 0x000001CF, 0x00050081, 0x0000000A, 
0x00000289, 0x00000288, 0x00000287, 0x0003003E, 0x000001CF, 0x00000289, 0x000200F9, 0x00000280, 0x000200F8, 0x00000280, 0x00040008, 0x00000001, 
0x00000158, 0x00000000, 0x00050041, 0x0000003F, 0x0000028A, 0x000001E5, 0x0000006B, 0x0004003D, 0x00000009, 0x0000028B, 0x0000028A, 0x00050041, 
0x00000038, 0x0000028C, 0x00000267, 0x0000006B, 0x0004003D, 0x00000008, 0x0000028D, 0x0000028C, 0x0005008E, 0x00000009, 0x0000028E, 0x0000028B, 
0x0000028D, 0x00050041, 0x0000003F, 0x0000028F, 0x000001E5, 0x0000005B, 0x0004003D, 0x00000009, 0x00000290, 0x0000028F, 0x00050081, 0x00000009, 
0x00000291, 0x00000290, 0x0000028E, 0x00050041, 0x0000003F, 0x00000292, 0x000001E5, 0x0000005B, 0x0003003E, 0x00000292, 0x00000291, 0x00040008, 
0x00000001, 0x00000159, 0x00000000, 0x00050041, 0x0000003F, 0x00000293, 0x00000267, 0x0000013F, 0x0004003D, 0x00000009, 0x00000294, 0x00000293, 
0x0005008E, 0x00000009, 0x00000295, 0x00000294, 0x00000137, 0x00050041, 0x0000003F, 0x00000296, 0x000001E5, 0x0000005B, 0x0004003D, 0x00000009, 
0x00000297, 0x00000296, 0x00050081, 0x00000009, 0x00000298, 0x00000297, 0x00000295, 0x00050041, 0x0000003F, 0x00000299, 0x000001E5, 0x0000005B, 
0x0003003E, 0x00000299, 0x00000298, 0x00040008, 0x00000001, 0x0000015B, 0x00000000, 0x00050041, 0x0000003F, 0x0000029B, 0x000001E5, 0x0000005B, 
0x0004003D, 0x00000009, 0x0000029C, 0x0000029B, 0x0007004F, 0x00000030, 0x0000029D, 0x0000029C, 0x0000029C, 0x00000000, 0x00000001, 0x0004003D, 
0x00000030, 0x0000029E, 0x0000003B, 0x00050081, 0x00000030, 0x0000029F, 0x0000029D, 0x0000029E, 0x0003003E, 0x000002A0, 0x0000029F, 0x00050039, 
0x00000008, 0x000002A1, 0x00000034, 0x000002A0, 0x00050041, 0x0000003F, 0x000002A2, 0x000001E5, 0x0000005B, 0x0004003D, 0x00000009, 0x000002A3, 
0x000002A2, 0x0007004F, 0x00000030, 0x000002A4, 0x000002A3, 0x000002A3, 0x00000001, 0x00000002, 0x0004003D, 0x00000030, 0x000002A5, 0x0000003B, 
0x00050081, 0x00000030, 0x000002A6, 0x000002A4, 0x000002A5, 0x0003003E, 0x000002A7, 0x000002A6, 0x00050039, 0x00000008, 0x000002A8, 0x00000034, 
0x000002A7, 0x00050041, 0x0000003F, 0x000002A9, 0x000001E5, 0x0000005B, 0x0004003D, 0x00000009, 0x000002AA, 0x000002A9, 0x0007004F, 0x00000030, 
0x000002AB, 0x000002AA, 0x000002AA, 0x00000002, 0x00000000, 0x0004003D, 0x00000030, 0x000002AC, 0x0000003B, 0x00050081, 0x00000030, 0x000002AD, 
0x000002AB, 0x000002AC, 0x0003003E, 0x000002AE, 0x000002AD, 0x00050039, 0x00000008, 0x000002AF, 0x00000034, 0x000002AE, 0x00060050, 0x00000009, 
0x000002B0, 0x000002A1, 0x000002A8, 0x000002AF, 0x0005008E, 0x00000009, 0x000002B1, 0x000002B0, 0x000001EB, 0x00060050, 0x00000009, 0x000002B2, 
0x0000016B, 0x0000016B, 0x0000016B, 0x00050083, 0x00000009, 0x000002B3, 0x000002B1, 0x000002B2, 0x0003003E, 0x0000029A, 0x000002B3, 0x00040008, 
0x00000001, 0x0000015C, 0x00000000, 0x0004003D, 0x0000000B, 0x000002B4, 0x00000267, 0x0004003D, 0x0000000B, 0x000002B5, 0x00000224, 0x00050051, 
0x00000007, 0x000002B6, 0x000002B4, 0x00000000, 0x00050051, 0x00000007, 0x000002B7, 0x000002B5, 0x00000000, 0x000500A5, 0x00000007, 0x000002B8, 
0x000002B6, 0x000002B7, 0x00050051, 0x00000008, 0x000002B9, 0x000002B4, 0x00000001, 0x00050051, 0x00000008, 0x000002BA, 0x000002B5, 0x00000001, 
0x000500B6, 0x00000007, 0x000002BB, 0x000002B9, 0x000002BA, 0x000500A6, 0x00000007, 0x000002BC, 0x000002B8, 0x000002BB, 0x00050051, 0x00000009, 
0x000002BD, 0x000002B4, 0x00000002, 0x00050051, 0x00000009, 0x000002BE, 0x000002B5, 0x00000002, 0x000500B6, 0x000002BF, 0x000002C0, 0x000002BD, 
0x000002BE, 0x0004009A, 0x00000007, 0x000002C1, 0x000002C0, 0x000500A6, 0x00000007, 0x000002C2, 0x000002BC, 0x000002C1, 0x00050051, 0x0000000A, 
0x000002C3, 0x000002B4, 0x00000003, 0x00050051, 0x0000000A, 0x000002C4, 0x000002B5, 0x00000003, 0x000500B6, 0x0000014B, 0x000002C5, 0x000002C3, 
0x000002C4, 0x0004009A, 0x00000007, 0x000002C6, 0x000002C5, 0x000500A6, 0x00000007, 0x000002C7, 0x000002C2, 0x000002C6, 0x00050051, 0x00000008, 
0x000002C8, 0x000002B4, 0x00000004, 0x00050051, 0x00000008, 0x000002C9, 0x000002B5, 0x00000004, 0x000500B6, 0x00000007, 0x000002CA, 0x000002C8, 
0x000002C9, 0x000500A6, 0x00000007, 0x000002CB, 0x000002C7, 0x000002CA, 0x0004003D, 0x0000000B, 0x000002CC, 0x00000267, 0x0004003D, 0x0000000B, 
0x000002CD, 0x0000023A, 0x00050051, 0x00000007, 0x000002CE, 0x000002CC, 0x00000000, 0x00050051, 0x00000007, 0x000002CF, 0x000002CD, 0x00000000, 
0x000500A5, 0x00000007, 0x000002D0, 0x000002CE, 0x000002CF, 0x00050051, 0x00000008, 0x000002D1, 0x000002CC, 0x00000001, 0x00050051, 0x00000008, 
0x000002D2, 0x000002CD, 0x00000001, 0x000500B6, 0x00000007, 0x000002D3, 0x000002D1, 0x000002D2, 0x000500A6, 0x00000007, 0x000002D4, 0x000002D0, 
0x000002D3, 0x00050051, 0x00000009, 0x000002D5, 0x000002CC, 0x00000002, 0x00050051, 0x00000009, 0x000002D6, 0x000002CD, 0x00000002, 0x000500B6, 
0x000002BF, 0x000002D7, 0x000002D5, 0x000002D6, 0x0004009A, 0x00000007, 0x000002D8, 0x000002D7, 0x000500A6, 0x00000007, 0x000002D9, 0x000002D4, 
0x000002D8, 0x00050051, 0x0000000A, 0x000002DA, 0x000002CC, 0x00000003, 0x00050051, 0x0000000A, 0x000002DB, 0x000002CD, 0x00000003, 0x000500B6, 
0x0000014B, 0x000002DC, 0x000002DA, 0x000002DB, 0x0004009A, 0x00000007, 0x000002DD, 0x000002DC, 0x000500A6, 0x00000007, 0x000002DE, 0x000002D9, 
0x000002DD, 0x00050051, 0x00000008, 0x000002DF, 0x000002CC, 0x00000004, 0x00050051, 0x00000008, 0x000002E0, 0x000002CD, 0x00000004, 0x000500B6, 
0x00000007, 0x000002E1, 0x000002DF, 0x000002E0, 0x000500A6, 0x00000007, 0x000002E2, 0x000002DE, 0x000002E1, 0x000500A7, 0x00000007, 0x000002E3, 
0x000002CB, 0x000002E2, 0x000300F7, 0x000002E5, 0x00000000, 0x000400FA, 0x000002E3, 0x000002E4, 0x00000309, 0x000200F8, 0x000002E4, 0x00040008, 
0x00000001, 0x0000015F, 0x00000000, 0x00050041, 0x0000003F, 0x000002E7, 0x000001E5, 0x0000006B, 0x0004003D, 0x00000009, 0x000002E8, 0x000002E7, 
0x00050041, 0x0000003F, 0x000002E9, 0x00000267, 0x0000013F, 0x0004003D, 0x00000009, 0x000002EA, 0x000002E9, 0x0007000C, 0x00000009, 0x000002EB, 
0x00000002, 0x00000047, 0x000002E8, 0x000002EA, 0x0003003E, 0x000002E6, 0x000002EB, 0x00040008, 0x00000001, 0x00000161, 0x00000000, 0x00050041, 
0x0000003F, 0x000002ED, 0x00000267, 0x0000013F, 0x0004003D, 0x00000009, 0x000002EE, 0x000002ED, 0x0004003D, 0x00000009, 0x000002EF, 0x0000029A, 
0x00050081, 0x00000009, 0x000002F0, 0x000002EE, 0x000002EF, 0x0006000C, 0x00000009, 0x000002F1, 0x00000002, 0x00000045, 0x000002F0, 0x0003003E, 
0x000002EC, 0x000002F1, 0x00040008, 0x00000001, 0x00000163, 0x00000000, 0x00050041, 0x0000003F, 0x000002F3, 0x00000267, 0x0000013F, 0x0004003D, 
0x00000009, 0x000002F4, 0x000002F3, 0x00050041, 0x0000003F, 0x000002F5, 0x000001E5, 0x0000006B, 0x0004003D, 0x00000009, 0x000002F6, 0x000002F5, 
0x00050094, 0x00000008, 0x000002F7, 0x000002F4, 0x000002F6, 0x0004007F, 0x00000008, 0x000002F8, 0x000002F7, 0x0007000C, 0x00000008, 0x000002F9, 
0x00000002, 0x00000028, 0x000000A9, 0x000002F8, 0x0003003E, 0x000002F2, 0x000002F9, 0x00040008, 0x00000001, 0x00000164, 0x00000000, 0x0004003D, 
0x00000008, 0x000002FA, 0x000002F2, 0x0007000C, 0x00000008, 0x000002FB, 0x00000002, 0x0000001A, 0x000002FA, 0x00000095, 0x0004003D, 0x00000030, 
0x000002FD, 0x0000003B, 0x0003003E, 0x000002FC, 0x000002FD, 0x00050039, 0x00000008, 0x000002FE, 0x00000034, 0x000002FC, 0x0007000C, 0x00000008, 
0x000002FF, 0x00000002, 0x00000030, 0x000002FB, 0x000002FE, 0x0003003E, 0x000002F2, 0x000002FF, 0x00040008, 0x00000001, 0x00000165, 0x00000000, 
0x0004003D, 0x00000009, 0x00000300, 0x000002E6, 0x0004003D, 0x00000008, 0x00000301, 0x000002F2, 0x0005008E, 0x00000009, 0x00000302, 0x00000300, 
0x00000301, 0x0004003D, 0x00000009, 0x00000303, 0x000002EC, 0x0004003D, 0x00000008, 0x00000304, 0x000002F2, 0x00050083, 0x00000008, 0x00000305, 
0x0000016B, 0x00000304, 0x0005008E, 0x00000009, 0x00000306, 0x00000303, 0x00000305, 0x00050081, 0x00000009, 0x00000307, 0x00000302, 0x00000306, 
0x00050041, 0x0000003F, 0x00000308, 0x000001E5, 0x0000006B, 0x0003003E, 0x00000308, 0x00000307, 0x000200F9, 0x000002E5, 0x000200F8, 0x00000309, 
0x00040008, 0x00000001, 0x0000016A, 0x00000000, 0x00050041, 0x0000003F, 0x0000030A, 0x00000267, 0x0000013F, 0x0004003D, 0x00000009, 0x0000030B, 
0x0000030A, 0x0004003D, 0x00000009, 0x0000030C, 0x0000029A, 0x00050081, 0x00000009, 0x0000030D, 0x0000030B, 0x0000030C, 0x00050041, 0x0000003F, 
0x0000030E, 0x000001E5, 0x0000006B, 0x0004003D, 0x00000009, 0x0000030F, 0x0000030E, 0x00050041, 0x0000003F, 0x00000310, 0x00000267, 0x0000013F, 
0x0004003D, 0x00000009, 0x00000311, 0x00000310, 0x0007000C, 0x00000009, 0x00000312, 0x00000002, 0x00000047, 0x0000030F, 0x00000311, 0x00050041, 
0x00000038, 0x00000313, 0x00000267, 0x0000012E, 0x0004003D, 0x00000008, 0x00000314, 0x00000313, 0x00060050, 0x00000009, 0x00000315, 0x00000314, 
0x00000314, 0x00000314, 0x0008000C, 0x00000009, 0x00000316, 0x00000002, 0x0000002E, 0x0000030D, 0x00000312, 0x00000315, 0x0006000C, 0x00000009, 
0x00000317, 0x00000002, 0x00000045, 0x00000316, 0x00050041, 0x0000003F, 0x00000318, 0x000001E5, 0x0000006B, 0x0003003E, 0x00000318, 0x00000317, 
0x000200F9, 0x000002E5, 0x000200F8, 0x000002E5, 0x000200F9, 0x00000276, 0x000200F8, 0x00000319, 0x00040008, 0x00000001, 0x00000172, 0x00000000, 
0x000200F9, 0x0000021E, 0x000200F8, 0x00000276, 0x000200F9, 0x0000021F, 0x000200F8, 0x0000021F, 0x00040008, 0x00000001, 0x0000013A, 0x00000000, 
0x0004003D, 0x00000052, 0x0000031B, 0x0000021B, 0x00050080, 0x00000052, 0x0000031C, 0x0000031B, 0x0000006B, 0x0003003E, 0x0000021B, 0x0000031C, 
0x000200F9, 0x0000021C, 0x000200F8, 0x0000021E, 0x000200F9, 0x000001DA, 0x000200F8, 0x000001DA, 0x00040008, 0x00000001, 0x00000125, 0x00000000, 
0x0004003D, 0x00000052, 0x0000031D, 0x000001D6, 0x00050080, 0x00000052, 0x0000031E, 0x0000031D, 0x0000006B, 0x0003003E, 0x000001D6, 0x0000031E, 
0x000200F9, 0x000001D7, 0x000200F8, 0x000001D9, 0x00040008, 0x00000001, 0x0000017C, 0x00000000, 0x0004003D, 0x0000000A, 0x0000031F, 0x000001CF, 
0x00070050, 0x0000000A, 0x00000320, 0x000001D2, 0x000001D2, 0x000001D2, 0x000001D2, 0x00050088, 0x0000000A, 0x00000321, 0x0000031F, 0x00000320, 
0x000200FE, 0x00000321, 0x00010038, 0x00050036, 0x00000036, 0x00000045, 0x00000000, 0x00000040, 0x00030037, 0x0000003F, 0x00000041, 0x00030037, 
0x0000003F, 0x00000042, 0x00030037, 0x00000038, 0x00000043, 0x00030037, 0x00000031, 0x00000044, 0x000200F8, 0x00000046, 0x0004003B, 0x00000031, 
0x00000324, 0x00000007, 0x0004003B, 0x00000037, 0x00000362, 0x00000007, 0x0004003B, 0x00000021, 0x00000365, 0x00000007, 0x0004003B, 0x0000003F, 
0x0000036B, 0x00000007, 0x0004003B, 0x0000003F, 0x00000370, 0x00000007, 0x0004003B, 0x0000003F, 0x00000372, 0x00000007, 0x0004003B, 0x0000003F, 
0x00000377, 0x00000007, 0x00040008, 0x00000001, 0x00000183, 0x00000000, 0x00050041, 0x00000326, 0x00000327, 0x0000005A, 0x00000325, 0x0004003D, 
0x0000000A, 0x00000328, 0x00000327, 0x0007004F, 0x00000030, 0x00000329, 0x00000328, 0x00000328, 0x00000000, 0x00000001, 0x00050041, 0x00000326, 
0x0000032A, 0x0000005A, 0x0000005B, 0x0004003D, 0x0000000A, 0x0000032B, 0x0000032A, 0x0007004F, 0x00000030, 0x0000032C, 0x0000032B, 0x0000032B, 
0x00000000, 0x00000001, 0x00050088, 0x00000030, 0x0000032D, 0x00000329, 0x0000032C, 0x0003003E, 0x00000324, 0x0000032D, 0x00040008, 0x00000001, 
0x00000184, 0x00000000, 0x0004003D, 0x00000030, 0x0000032E, 0x00000324, 0x0005008E, 0x00000030, 0x0000032F, 0x0000032E, 0x000001EB, 0x00050050, 
0x00000030, 0x00000330, 0x00000095, 0x00000095, 0x00050083, 0x00000030, 0x00000331, 0x0000032F, 0x00000330, 0x0003003E, 0x00000324, 0x00000331, 
0x00040008, 0x00000001, 0x00000185, 0x00000000, 0x0004003D, 0x00000009, 0x00000332, 0x00000042, 0x0004003D, 0x00000009, 0x00000333, 0x00000041, 
0x00050083, 0x00000009, 0x00000334, 0x00000333, 0x00000332, 0x0003003E, 0x00000041, 0x00000334, 0x00040008, 0x00000001, 0x00000186, 0x00000000, 
0x00050041, 0x00000038, 0x00000335, 0x00000324, 0x000000C1, 0x0004003D, 0x00000008, 0x00000336, 0x00000335, 0x00050085, 0x00000008, 0x00000337, 
0x00000336, 0x00000095, 0x0006000C, 0x00000008, 0x00000338, 0x00000002, 0x0000000E, 0x00000337, 0x0004003D, 0x00000009, 0x00000339, 0x00000041, 
0x0007004F, 0x00000030, 0x0000033A, 0x00000339, 0x00000339, 0x00000000, 0x00000002, 0x0005008E, 0x00000030, 0x0000033B, 0x0000033A, 0x00000338, 
0x00050041, 0x00000038, 0x0000033C, 0x00000324, 0x000000C1, 0x0004003D, 0x00000008, 0x0000033D, 0x0000033C, 0x00050085, 0x00000008, 0x0000033E, 
0x0000033D, 0x00000095, 0x0006000C, 0x00000008, 0x0000033F, 0x00000002, 0x0000000D, 0x0000033E, 0x00050041, 0x00000038, 0x00000340, 0x00000041, 
0x000000D8, 0x0004003D, 0x00000008, 0x00000341, 0x00000340, 0x0004007F, 0x00000008, 0x00000342, 0x00000341, 0x00050041, 0x00000038, 0x00000343, 
0x00000041, 0x000000C1, 0x0004003D, 0x00000008, 0x00000344, 0x00000343, 0x00050050, 0x00000030, 0x00000345, 0x00000342, 0x00000344, 0x0005008E, 
0x00000030, 0x00000346, 0x00000345, 0x0000033F, 0x00050081, 0x00000030, 0x00000347, 0x0000033B, 0x00000346, 0x0004003D, 0x00000009, 0x00000348, 
0x00000041, 0x0008004F, 0x00000009, 0x00000349, 0x00000348, 0x00000347, 0x00000003, 0x00000001, 0x00000004, 0x0003003E, 0x00000041, 0x00000349, 
0x00040008, 0x00000001, 0x00000187, 0x00000000, 0x00050041, 0x00000038, 0x0000034A, 0x00000324, 0x0000005C, 0x0004003D, 0x00000008, 0x0000034B, 
0x0000034A, 0x00050085, 0x00000008, 0x0000034C, 0x0000034B, 0x00000095, 0x0006000C, 0x00000008, 0x0000034D, 0x00000002, 0x0000000E, 0x0000034C, 
0x0004003D, 0x00000009, 0x0000034E, 0x00000041, 0x0007004F, 0x00000030, 0x0000034F, 0x0000034E, 0x0000034E, 0x00000000, 0x00000001, 0x0005008E, 
0x00000030, 0x00000350, 0x0000034F, 0x0000034D, 0x00050041, 0x00000038, 0x00000351, 0x00000324, 0x0000005C, 0x0004003D, 0x00000008, 0x00000352, 
0x00000351, 0x00050085, 0x00000008, 0x00000353, 0x00000352, 0x00000095, 0x0006000C, 0x00000008, 0x00000354, 0x00000002, 0x0000000D, 0x00000353, 
0x00050041, 0x00000038, 0x00000355, 0x00000041, 0x0000005C, 0x0004003D, 0x00000008, 0x00000356, 0x00000355, 0x0004007F, 0x00000008, 0x00000357, 
0x00000356, 0x00050041, 0x00000038, 0x00000358, 0x00000041, 0x000000C1, 0x0004003D, 0x00000008, 0x00000359, 0x00000358, 0x00050050, 0x00000030, 
0x0000035A, 0x00000357, 0x00000359, 0x0005008E, 0x00000030, 0x0000035B, 0x0000035A, 0x00000354, 0x00050081, 0x00000030, 0x0000035C, 0x00000350, 
0x0000035B, 0x0004003D, 0x00000009, 0x0000035D, 0x00000041, 0x0008004F, 0x00000009, 0x0000035E, 0x0000035D, 0x0000035C, 0x00000003, 0x00000004, 
0x00000002, 0x0003003E, 0x00000041, 0x0000035E, 0x00040008, 0x00000001, 0x00000188, 0x00000000, 0x0004003D, 0x00000009, 0x0000035F, 0x00000042, 
0x0004003D, 0x00000009, 0x00000360, 0x00000041, 0x00050081, 0x00000009, 0x00000361, 0x00000360, 0x0000035F, 0x0003003E, 0x00000041, 0x00000361, 
0x00040008, 0x00000001, 0x0000018A, 0x00000000, 0x0004003D, 0x00000009, 0x00000363, 0x00000041, 0x00050041, 0x0000003F, 0x00000364, 0x00000362, 
0x0000005B, 0x0003003E, 0x00000364, 0x00000363, 0x00040008, 0x00000001, 0x0000018C, 0x00000000, 0x0004003D, 0x00000009, 0x00000366, 0x00000041, 
0x00050041, 0x0000003F, 0x00000367, 0x00000365, 0x0000005B, 0x0003003E, 0x00000367, 0x00000366, 0x00040008, 0x00000001, 0x0000018D, 0x00000000, 
0x0004003D, 0x00000008, 0x00000368, 0x00000043, 0x0004003D, 0x00000030, 0x00000369, 0x00000044, 0x0005008E, 0x00000030, 0x0000036A, 0x00000369, 
0x00000368, 0x0003003E, 0x00000044, 0x0000036A, 0x00040008, 0x00000001, 0x0000018F, 0x00000000, 0x0004003D, 0x00000009, 0x0000036C, 0x00000042, 
0x0004003D, 0x00000009, 0x0000036D, 0x00000041, 0x00050083, 0x00000009, 0x0000036E, 0x0000036C, 0x0000036D, 0x0006000C, 0x00000009, 0x0000036F, 
0x00000002, 0x00000045, 0x0000036E, 0x0003003E, 0x0000036B, 0x0000036F, 0x00040008, 0x00000001, 0x00000191, 0x00000000, 0x0003003E, 0x00000370, 
0x00000371, 0x00040008, 0x00000001, 0x00000193, 0x00000000, 0x0004003D, 0x00000009, 0x00000373, 0x0000036B, 0x0004003D, 0x00000009, 0x00000374, 
0x00000370, 0x0007000C, 0x00000009, 0x00000375, 0x00000002, 0x00000044, 0x00000373, 0x00000374, 0x0006000C, 0x00000009, 0x00000376, 0x00000002, 
0x00000045, 0x00000375, 0x0003003E, 0x00000372, 0x00000376, 0x00040008, 0x00000001, 0x00000195, 0x00000000, 0x0004003D, 0x00000009, 0x00000378, 
0x00000372, 0x0004003D, 0x00000009, 0x00000379, 0x0000036B, 0x0007000C, 0x00000009, 0x0000037A, 0x00000002, 0x00000044, 0x00000378, 0x00000379, 
0x0006000C, 0x00000009, 0x0000037B, 0x00000002, 0x00000045, 0x0000037A, 0x0003003E, 0x00000377, 0x0000037B, 0x00040008, 0x00000001, 0x00000196, 
0x00000000, 0x00050041, 0x00000038, 0x0000037C, 0x00000044, 0x000000C1, 0x0004003D, 0x00000008, 0x0000037D, 0x0000037C, 0x0004003D, 0x00000009, 
0x0000037E, 0x00000372, 0x0005008E, 0x00000009, 0x0000037F, 0x0000037E, 0x0000037D, 0x00050041, 0x00000038, 0x00000380, 0x00000044, 0x0000005C, 
0x0004003D, 0x00000008, 0x00000381, 0x00000380, 0x0004003D, 0x00000009, 0x00000382, 0x00000377, 0x0005008E, 0x00000009, 0x00000383, 0x00000382, 
0x00000381, 0x00050081, 0x00000009, 0x00000384, 0x0000037F, 0x00000383, 0x0004003D, 0x00000009, 0x00000385, 0x0000036B, 0x0005008E, 0x00000009, 
0x00000386, 0x00000385, 0x00000095, 0x00050081, 0x00000009, 0x00000387, 0x00000384, 0x00000386, 0x0006000C, 0x00000009, 0x00000388, 0x00000002, 
0x00000045, 0x00000387, 0x00050041, 0x0000003F, 0x00000389, 0x00000365, 0x0000006B, 0x0003003E, 0x00000389, 0x00000388, 0x00040008, 0x00000001, 
0x00000197, 0x00000000, 0x0004003D, 0x00000020, 0x0000038A, 0x00000365, 0x00050041, 0x00000021, 0x0000038B, 0x00000362, 0x0000006B, 0x0003003E, 
0x0000038B, 0x0000038A, 0x00040008, 0x00000001, 0x00000198, 0x00000000, 0x0004003D, 0x00000008, 0x0000038C, 0x00000043, 0x00060041, 0x0000005D, 
0x0000038D, 0x0000005A, 0x0000005B, 0x000000C1, 0x0004003D, 0x00000008, 0x0000038E, 0x0000038D, 0x00050088, 0x00000008, 0x0000038F, 0x0000038C, 
0x0000038E, 0x0003003E, 0x000001EE, 0x0000038F, 0x00040008, 0x00000001, 0x00000199, 0x00000000, 0x0004003D, 0x00000008, 0x00000390, 0x000001EE, 
0x0004003D, 0x00000030, 0x00000391, 0x00000044, 0x0006000C, 0x00000008, 0x00000392, 0x00000002, 0x00000042, 0x00000391, 0x00050085, 0x00000008, 
0x00000393, 0x00000392, 0x00000137, 0x00050081, 0x00000008, 0x00000394, 0x00000390, 0x00000393, 0x0003003E, 0x000001EE, 0x00000394, 0x00040008, 
0x00000001, 0x0000019A, 0x00000000, 0x0004003D, 0x00000036, 0x00000395, 0x00000362, 0x000200FE, 0x00000395, 0x00010038, 0x00050036, 0x00000003, 
0x0000004B, 0x00000000, 0x00000048, 0x00030037, 0x00000047, 0x00000049, 0x00030037, 0x00000031, 0x0000004A, 0x000200F8, 0x0000004C, 0x0004003B, 
0x00000031, 0x0000039A, 0x00000007, 0x0004003B, 0x00000047, 0x000003A0, 0x00000007, 0x0004003B, 0x00000037, 0x000003B5, 0x00000007, 0x0004003B, 
0x0000003F, 0x000003B8, 0x00000007, 0x0004003B, 0x0000003F, 0x000003B9, 0x00000007, 0x0004003B, 0x00000038, 0x000003BA, 0x00000007, 0x0004003B, 
0x00000031, 0x000003BB, 0x00000007, 0x0004003B, 0x00000038, 0x000003BE, 0x00000007, 0x0004003B, 0x00000047, 0x000003C9, 0x00000007, 0x0004003B, 
0x00000037, 0x000003D2, 0x00000007, 0x0004003B, 0x00000031, 0x000003D4, 0x00000007, 0x0004003B, 0x00000038, 0x000003D5, 0x00000007, 0x00040008, 
0x00000001, 0x000001A0, 0x00000000, 0x00050041, 0x0000005D, 0x00000398, 0x0000005A, 0x0000006B, 0x0004003D, 0x00000008, 0x00000399, 0x00000398, 
0x0003003E, 0x00000170, 0x00000399, 0x00040008, 0x00000001, 0x000001A2, 0x00000000, 0x0004003D, 0x00000030, 0x0000039B, 0x0000004A, 0x00050041, 
0x00000326, 0x0000039C, 0x0000005A, 0x0000005B, 0x0004003D, 0x0000000A, 0x0000039D, 0x0000039C, 0x0007004F, 0x00000030, 0x0000039E, 0x0000039D, 
0x0000039D, 0x00000000, 0x00000001, 0x00050088, 0x00000030, 0x0000039F, 0x0000039B, 0x0000039E, 0x0003003E, 0x0000039A, 0x0000039F, 0x00040008, 
0x00000001, 0x000001A4, 0x00000000, 0x0004003D, 0x000003A2, 0x000003A5, 0x000003A4, 0x0004003D, 0x00000030, 0x000003A6, 0x0000039A, 0x00050057, 
0x0000000A, 0x000003A7, 0x000003A5, 0x000003A6, 0x0003003E, 0x000003A0, 0x000003A7, 0x00040008, 0x00000001, 0x000001A5, 0x00000000, 0x0004003D, 
0x00000030, 0x000003A8, 0x0000039A, 0x0005008E, 0x00000030, 0x000003A9, 0x000003A8, 0x000001EB, 0x00050050, 0x00000030, 0x000003AA, 0x0000016B, 
0x0000016B, 0x00050083, 0x00000030, 0x000003AB, 0x000003A9, 0x000003AA, 0x0003003E, 0x0000039A, 0x000003AB, 0x00040008, 0x00000001, 0x000001A6, 
0x00000000, 0x00060041, 0x0000005D, 0x000003AC, 0x0000005A, 0x0000005B, 0x000000C1, 0x0004003D, 0x00000008, 0x000003AD, 0x000003AC, 0x00060041, 
0x0000005D, 0x000003AE, 0x0000005A, 0x0000005B, 0x0000005C, 0x0004003D, 0x00000008, 0x000003AF, 0x000003AE, 0x00050088, 0x00000008, 0x000003B0, 
0x000003AD, 0x000003AF, 0x00050041, 0x00000038, 0x000003B1, 0x0000039A, 0x0000005C, 0x0004003D, 0x00000008, 0x000003B2, 0x000003B1, 0x00050088, 
0x00000008, 0x000003B3, 0x000003B2, 0x000003B0, 0x00050041, 0x00000038, 0x000003B4, 0x0000039A, 0x0000005C, 0x0003003E, 0x000003B4, 0x000003B3, 
0x00040008, 0x00000001, 0x000001A8, 0x00000000, 0x0003003E, 0x000003B8, 0x000003B7, 0x0003003E, 0x000003B9, 0x0000023D, 0x0003003E, 0x000003BA, 
0x0000016B, 0x0004003D, 0x00000030, 0x000003BC, 0x0000039A, 0x0003003E, 0x000003BB, 0x000003BC, 0x00080039, 0x00000036, 0x000003BD, 0x00000045, 
0x000003B8, 0x000003B9, 0x000003BA, 0x000003BB, 0x0003003E, 0x000003B5, 0x000003BD, 0x00040008, 0x00000001, 0x000001AA, 0x00000000, 0x00050041, 
0x00000038, 0x000003BF, 0x000003A0, 0x000000C1, 0x0004003D, 0x00000008, 0x000003C0, 0x000003BF, 0x00050041, 0x00000038, 0x000003C1, 0x000003A0, 
0x0000005C, 0x0004003D, 0x00000008, 0x000003C2, 0x000003C1, 0x00050041, 0x00000038, 0x000003C3, 0x000003A0, 0x000000D8, 0x0004003D, 0x00000008, 
0x000003C4, 0x000003C3, 0x0007000C, 0x00000008, 0x000003C5, 0x00000002, 0x00000028, 0x000003C2, 0x000003C4, 0x0007000C, 0x00000008, 0x000003C6, 
0x00000002, 0x00000028, 0x000003C0, 0x000003C5, 0x0003003E, 0x000003BE, 0x000003C6, 0x00040008, 0x00000001, 0x000001AB, 0x00000000, 0x0004003D, 
0x00000008, 0x000003C7, 0x000003BE, 0x0007000C, 0x00000008, 0x000003C8, 0x00000002, 0x0000001A, 0x000003C7, 0x000001B9, 0x0003003E, 0x000003BE, 
0x000003C8, 0x00040008, 0x00000001, 0x000001AD, 0x00000000, 0x0004003D, 0x00000030, 0x000003CA, 0x0000039A, 0x00050041, 0x0000005D, 0x000003CB, 
0x0000005A, 0x0000006B, 0x0004003D, 0x00000008, 0x000003CC, 0x000003CB, 0x00050050, 0x00000030, 0x000003CD, 0x000003CC, 0x000003CC, 0x00050081, 
0x00000030, 0x000003CE, 0x000003CA, 0x000003CD, 0x0004003D, 0x00000008, 0x000003CF, 0x000003BE, 0x00050083, 0x00000008, 0x000003D0, 0x0000016B, 
0x000003CF, 0x0007000C, 0x00000008, 0x000003D1, 0x00000002, 0x00000028, 0x0000024D, 0x000003D0, 0x0004003D, 0x00000036, 0x000003D3, 0x000003B5, 
0x0003003E, 0x000003D2, 0x000003D3, 0x0003003E, 0x000003D4, 0x000003CE, 0x0003003E, 0x000003D5, 0x000003D1, 0x00070039, 0x0000000A, 0x000003D6, 
0x0000003D, 0x000003D2, 0x000003D4, 0x000003D5, 0x0003003E, 0x000003C9, 0x000003D6, 0x00040008, 0x00000001, 0x000001AE, 0x00000000, 0x0004003D, 
0x0000000A, 0x000003D7, 0x000003C9, 0x0004003D, 0x0000000A, 0x000003D8, 0x000003A0, 0x00060041, 0x0000005D, 0x000003D9, 0x0000005A, 0x00000325, 
0x000000D8, 0x0004003D, 0x00000008, 0x000003DA, 0x000003D9, 0x000500BA, 0x00000007, 0x000003DB, 0x000003DA, 0x00000095, 0x000400A8, 0x00000007, 
0x000003DC, 0x000003DB, 0x000300F7, 0x000003DE, 0x00000000, 0x000400FA, 0x000003DC, 0x000003DD, 0x000003DE, 0x000200F8, 0x000003DD, 0x00050041, 
0x000003DF, 0x000003E0, 0x0000005A, 0x00000146, 0x0004003D, 0x00000052, 0x000003E1, 0x000003E0, 0x000500AA, 0x00000007, 0x000003E2, 0x000003E1, 
0x0000005B, 0x000200F9, 0x000003DE, 0x000200F8, 0x000003DE, 0x000700F5, 0x00000007, 0x000003E3, 0x000003DB, 0x0000004C, 0x000003E2, 0x000003DD, 
0x000600A9, 0x00000008, 0x000003E5, 0x000003E3, 0x00000095, 0x000003E4, 0x00070050, 0x0000000A, 0x000003E6, 0x000003E5, 0x000003E5, 0x000003E5, 
0x000003E5, 0x0008000C, 0x0000000A, 0x000003E7, 0x00000002, 0x0000002E, 0x000003D7, 0x000003D8, 0x000003E6, 0x0003003E, 0x00000049, 0x000003E7, 
0x000100FD, 0x00010038 });
	descr.Fragment().StringSpirvAsm( 
R"#(// Module Version 10000
// Generated by (magic number): 80002
// Id's are bound by 1000

                              Capability Shader
               2:             ExtInstImport  "GLSL.std.450"
                              MemoryModel Logical GLSL450
                              EntryPoint Fragment 5  "main" 79 101
                              ExecutionMode 5 OriginUpperLeft
               1:             String  ""
                              Source GLSL 450 1  "// OpModuleProcessed entry-point main
// OpModuleProcessed client vulkan100
// OpModuleProcessed target-env vulkan1.0
// OpModuleProcessed entry-point main
#line 1
"
                              SourceExtension  "GL_ARB_gpu_shader_int64"
                              SourceExtension  "GL_ARB_separate_shader_objects"
                              Name 5  "main"
                              Name 11  "HitTest"
                              MemberName 11(HitTest) 0  "hit"
                              MemberName 11(HitTest) 1  "dist"
                              MemberName 11(HitTest) 2  "normal"
                              MemberName 11(HitTest) 3  "col"
                              MemberName 11(HitTest) 4  "ref"
                              Name 16  "minT(struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;"
                              Name 14  "a"
                              Name 15  "b"
                              Name 22  "minT(struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;"
                              Name 19  "a"
                              Name 20  "b"
                              Name 21  "c"
                              Name 30  "minT(struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;"
                              Name 25  "a"
                              Name 26  "b"
                              Name 27  "c"
                              Name 28  "d"
                              Name 29  "e"
                              Name 32  "Ray"
                              MemberName 32(Ray) 0  "origin"
                              MemberName 32(Ray) 1  "dir"
                              Name 34  "Box"
                              MemberName 34(Box) 0  "pos"
                              MemberName 34(Box) 1  "size"
                              Name 39  "intersectBox(struct-Ray-vf3-vf31;struct-Box-vf3-vf31;"
                              Name 37  "r"
                              Name 38  "b"
                              Name 41  "Sphere"
                              MemberName 41(Sphere) 0  "pos"
                              MemberName 41(Sphere) 1  "radius"
                              Name 46  "intersectSphere(struct-Ray-vf3-vf31;struct-Sphere-vf3-f11;"
                              Name 44  "r"
                              Name 45  "s"
                              Name 52  "nrand(vf2;"
                              Name 51  "n"
                              Name 54  "Camera"
                              MemberName 54(Camera) 0  "pos"
                              MemberName 54(Camera) 1  "ray"
                              Name 61  "traceScene(struct-Camera-vf3-struct-Ray-vf3-vf311;vf2;f1;"
                              Name 58  "cam"
                              Name 59  "seed"
                              Name 60  "lastB"
                              Name 69  "setupCam(vf3;vf3;f1;vf2;"
                              Name 65  "pos"
                              Name 66  "target"
                              Name 67  "fov"
                              Name 68  "uv"
                              Name 75  "mainImage(vf4;vf2;"
                              Name 73  "fragColor"
                              Name 74  "fragCoord"
                              Name 77  "coord"
                              Name 79  "gl_FragCoord"
                              Name 88  "ShadertoyUB"
                              MemberName 88(ShadertoyUB) 0  "iResolution"
                              MemberName 88(ShadertoyUB) 1  "iTime"
                              MemberName 88(ShadertoyUB) 2  "iTimeDelta"
                              MemberName 88(ShadertoyUB) 3  "iFrame"
                              MemberName 88(ShadertoyUB) 4  "_padding0"
                              MemberName 88(ShadertoyUB) 5  "iChannelTime"
                              MemberName 88(ShadertoyUB) 6  "iChannelResolution"
                              MemberName 88(ShadertoyUB) 7  "iMouse"
                              MemberName 88(ShadertoyUB) 8  "iDate"
                              MemberName 88(ShadertoyUB) 9  "iSampleRate"
                              MemberName 88(ShadertoyUB) 10  "_padding1"
                              MemberName 88(ShadertoyUB) 11  "_padding2"
                              MemberName 88(ShadertoyUB) 12  "_padding3"
                              MemberName 88(ShadertoyUB) 13  "iCameraFrustum"
                              MemberName 88(ShadertoyUB) 14  "iCameraPos"
                              Name 90  ""
                              Name 101  "out_Color0"
                              Name 102  "param"
                              Name 103  "param"
                              Name 121  "param"
                              Name 123  "param"
                              Name 126  "param"
                              Name 128  "param"
                              Name 132  "param"
                              Name 134  "param"
                              Name 137  "param"
                              Name 139  "param"
                              Name 141  "param"
                              Name 144  "param"
                              Name 145  "param"
                              Name 154  "ba"
                              Name 160  "bb"
                              Name 166  "h"
                              Name 173  "d"
                              Name 174  "dA"
                              Name 183  "dB"
                              Name 260  "d1"
                              Name 269  "d2"
                              Name 281  "endPoint"
                              Name 315  "l"
                              Name 368  "t"
                              Name 386  "o"
                              Name 392  "v"
                              Name 402  "disc"
                              Name 421  "dist"
                              Name 460  "startPos"
                              Name 463  "result"
                              Name 465  "maxI"
                              Name 470  "i"
                              Name 485  "r"
                              Name 488  "param"
                              Name 494  "divergence"
                              Name 501  "param"
                              Name 513  "param"
                              Name 529  "impact"
                              Name 534  "maxJ"
                              Name 539  "j"
                              Name 548  "t0"
                              Name 555  "param"
                              Name 557  "param"
                              Name 559  "t1"
                              Name 563  "param"
                              Name 565  "param"
                              Name 570  "t2"
                              Name 575  "param"
                              Name 577  "param"
                              Name 582  "t3"
                              Name 585  "param"
                              Name 587  "param"
                              Name 598  "t4"
                              Name 603  "param"
                              Name 605  "param"
                              Name 615  "test"
                              Name 616  "param"
                              Name 618  "param"
                              Name 620  "param"
                              Name 622  "param"
                              Name 624  "param"
                              Name 666  "random"
                              Name 672  "param"
                              Name 679  "param"
                              Name 686  "param"
                              Name 742  "refl"
                              Name 748  "matte"
                              Name 754  "s"
                              Name 764  "param"
                              Name 804  "mouse"
                              Name 866  "cam"
                              Name 869  "ray"
                              Name 875  "cw"
                              Name 880  "cp"
                              Name 882  "cu"
                              Name 887  "cv"
                              Name 922  "uv"
                              Name 928  "l"
                              Name 932  "iChannel0"
                              Name 949  "cam"
                              Name 952  "param"
                              Name 953  "param"
                              Name 954  "param"
                              Name 955  "param"
                              Name 958  "lastB"
                              Name 969  "c"
                              Name 978  "param"
                              Name 980  "param"
                              Name 981  "param"
                              Decorate 79(gl_FragCoord) BuiltIn FragCoord
                              Decorate 85 ArrayStride 16
                              Decorate 86 ArrayStride 16
                              Decorate 87 ArrayStride 16
                              MemberDecorate 88(ShadertoyUB) 0 Offset 0
                              MemberDecorate 88(ShadertoyUB) 1 Offset 16
                              MemberDecorate 88(ShadertoyUB) 2 Offset 20
                              MemberDecorate 88(ShadertoyUB) 3 Offset 24
                              MemberDecorate 88(ShadertoyUB) 4 Offset 28
                              MemberDecorate 88(ShadertoyUB) 5 Offset 32
                              MemberDecorate 88(ShadertoyUB) 6 Offset 96
                              MemberDecorate 88(ShadertoyUB) 7 Offset 160
                              MemberDecorate 88(ShadertoyUB) 8 Offset 176
                              MemberDecorate 88(ShadertoyUB) 9 Offset 192
                              MemberDecorate 88(ShadertoyUB) 10 Offset 196
                              MemberDecorate 88(ShadertoyUB) 11 Offset 200
                              MemberDecorate 88(ShadertoyUB) 12 Offset 204
                              MemberDecorate 88(ShadertoyUB) 13 Offset 208
                              MemberDecorate 88(ShadertoyUB) 14 Offset 272
                              Decorate 88(ShadertoyUB) Block
                              Decorate 90 DescriptorSet 0
                              Decorate 90 Binding 1
                              Decorate 101(out_Color0) Location 0
                              Decorate 932(iChannel0) DescriptorSet 0
                              Decorate 932(iChannel0) Binding 0
               3:             TypeVoid
               4:             TypeFunction 3
               7:             TypeBool
               8:             TypeFloat 32
               9:             TypeVector 8(float) 3
              10:             TypeVector 8(float) 4
     11(HitTest):             TypeStruct 7(bool) 8(float) 9(fvec3) 10(fvec4) 8(float)
              12:             TypePointer Function 11(HitTest)
              13:             TypeFunction 11(HitTest) 12(ptr) 12(ptr)
              18:             TypeFunction 11(HitTest) 12(ptr) 12(ptr) 12(ptr)
              24:             TypeFunction 11(HitTest) 12(ptr) 12(ptr) 12(ptr) 12(ptr) 12(ptr)
         32(Ray):             TypeStruct 9(fvec3) 9(fvec3)
              33:             TypePointer Function 32(Ray)
         34(Box):             TypeStruct 9(fvec3) 9(fvec3)
              35:             TypePointer Function 34(Box)
              36:             TypeFunction 11(HitTest) 33(ptr) 35(ptr)
      41(Sphere):             TypeStruct 9(fvec3) 8(float)
              42:             TypePointer Function 41(Sphere)
              43:             TypeFunction 11(HitTest) 33(ptr) 42(ptr)
              48:             TypeVector 8(float) 2
              49:             TypePointer Function 48(fvec2)
              50:             TypeFunction 8(float) 49(ptr)
      54(Camera):             TypeStruct 9(fvec3) 32(Ray)
              55:             TypePointer Function 54(Camera)
              56:             TypePointer Function 8(float)
              57:             TypeFunction 10(fvec4) 55(ptr) 49(ptr) 56(ptr)
              63:             TypePointer Function 9(fvec3)
              64:             TypeFunction 54(Camera) 63(ptr) 63(ptr) 56(ptr) 49(ptr)
              71:             TypePointer Function 10(fvec4)
              72:             TypeFunction 3 71(ptr) 49(ptr)
              78:             TypePointer Input 10(fvec4)
79(gl_FragCoord):     78(ptr) Variable Input
              82:             TypeInt 32 1
              83:             TypeInt 32 0
              84:     83(int) Constant 4
              85:             TypeArray 10(fvec4) 84
              86:             TypeArray 10(fvec4) 84
              87:             TypeArray 10(fvec4) 84
 88(ShadertoyUB):             TypeStruct 10(fvec4) 8(float) 8(float) 82(int) 8(float) 85 86 10(fvec4) 10(fvec4) 8(float) 8(float) 8(float) 8(float) 87 10(fvec4)
              89:             TypePointer Uniform 88(ShadertoyUB)
              90:     89(ptr) Variable Uniform
              91:     82(int) Constant 0
              92:     83(int) Constant 1
              93:             TypePointer Uniform 8(float)
             100:             TypePointer Output 10(fvec4)
 101(out_Color0):    100(ptr) Variable Output
             107:     82(int) Constant 1
             149:    8(float) Constant 1056964608
             167:     7(bool) ConstantFalse
             168:    8(float) Constant 1203982336
             169:    8(float) Constant 0
             170:    9(fvec3) ConstantComposite 169 169 169
             171:   10(fvec4) ConstantComposite 169 169 169 169
             172: 11(HitTest) ConstantComposite 167 168 170 171 169
             193:     83(int) Constant 0
             216:     83(int) Constant 2
             297:     7(bool) ConstantTrue
             298:             TypePointer Function 7(bool)
             302:     82(int) Constant 4
             311:    8(float) Constant 1008981770
             319:     82(int) Constant 2
             321:     82(int) Constant 4294967295
             326:     82(int) Constant 3
             328:    8(float) Constant 1063675494
             329:   10(fvec4) ConstantComposite 328 149 149 169
             330:   10(fvec4) ConstantComposite 149 149 328 169
             331:             TypeVector 7(bool) 4
             353:   10(fvec4) ConstantComposite 328 328 328 169
             363:    8(float) Constant 1065353216
             367:             TypePointer Private 8(float)
          368(t):    367(ptr) Variable Private
             370:    8(float) Constant 1048039129
             373:    8(float) Constant 1061997773
             375:    8(float) Constant 1045220557
             376:    8(float) Constant 1067030938
             441:    8(float) Constant 1048576000
             450:    8(float) Constant 1095751225
             451:    8(float) Constant 1117550412
             452:   48(fvec2) ConstantComposite 450 451
             455:    8(float) Constant 1193995916
             464:             TypePointer Function 82(int)
             466:    8(float) Constant 1106247680
             477:     82(int) Constant 30
             491:    8(float) Constant 1073741824
 494(divergence):    367(ptr) Variable Private
             530:   10(fvec4) ConstantComposite 363 363 363 363
             535:    8(float) Constant 1092616192)#"_str
+
R"#(
             546:     82(int) Constant 10
             549:    8(float) Constant 3231711232
             550:    9(fvec3) ConstantComposite 549 535 549
             551:    8(float) Constant 1101004800
             552:    8(float) Constant 1103626240
             553:    9(fvec3) ConstantComposite 466 551 552
             554:     34(Box) ConstantComposite 550 553
             560:    8(float) Constant 3212836864
             561:    9(fvec3) ConstantComposite 560 491 169
             562:  41(Sphere) ConstantComposite 561 491
             567:    8(float) Constant 1058642330
             568:   10(fvec4) ConstantComposite 149 567 328 169
             571:    8(float) Constant 1082130432
             572:    8(float) Constant 1084227584
             573:    9(fvec3) ConstantComposite 571 572 571
             574:  41(Sphere) ConstantComposite 573 572
             579:   10(fvec4) ConstantComposite 328 328 169 169
             583:    9(fvec3) ConstantComposite 549 571 571
             584:  41(Sphere) ConstantComposite 583 571
             589:    8(float) Constant 1050253722
             593:    8(float) Constant 1077936128
             599:    8(float) Constant 1075838976
             600:    8(float) Constant 3221225472
             601:    9(fvec3) ConstantComposite 571 599 600
             602:  41(Sphere) ConstantComposite 601 599
             608:    8(float) Constant 1060320051
             635:     83(int) Constant 3
             703:             TypeVector 7(bool) 3
             805:     82(int) Constant 7
             806:             TypePointer Uniform 10(fvec4)
             881:    9(fvec3) ConstantComposite 169 363 169
             929:             TypeImage 8(float) 2D sampled format:Unknown
             930:             TypeSampledImage 929
             931:             TypePointer UniformConstant 930
  932(iChannel0):    931(ptr) Variable UniformConstant
             950:    8(float) Constant 3238002688
             951:    9(fvec3) ConstantComposite 169 593 950
             991:             TypePointer Uniform 82(int)
             996:    8(float) Constant 1065017672
         5(main):           3 Function None 4
               6:             Label
       77(coord):     49(ptr) Variable Function
      102(param):     71(ptr) Variable Function
      103(param):     49(ptr) Variable Function
                              Line 1 156 0
              80:   10(fvec4) Load 79(gl_FragCoord)
              81:   48(fvec2) VectorShuffle 80 80 0 1
                              Store 77(coord) 81
                              Line 1 157 0
              94:     93(ptr) AccessChain 90 91 92
              95:    8(float) Load 94
              96:     56(ptr) AccessChain 77(coord) 92
              97:    8(float) Load 96
              98:    8(float) FSub 95 97
              99:     56(ptr) AccessChain 77(coord) 92
                              Store 99 98
                              Line 1 158 0
             104:   48(fvec2) Load 77(coord)
                              Store 103(param) 104
             105:           3 FunctionCall 75(mainImage(vf4;vf2;) 102(param) 103(param)
             106:   10(fvec4) Load 102(param)
                              Store 101(out_Color0) 106
                              Return
                              FunctionEnd
16(minT(struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;): 11(HitTest) Function None 13
           14(a):     12(ptr) FunctionParameter
           15(b):     12(ptr) FunctionParameter
              17:             Label
                              Line 1 164 0
             108:     56(ptr) AccessChain 14(a) 107
             109:    8(float) Load 108
             110:     56(ptr) AccessChain 15(b) 107
             111:    8(float) Load 110
             112:     7(bool) FOrdLessThan 109 111
                              SelectionMerge 114 None
                              BranchConditional 112 113 117
             113:               Label
                                Line 1 166 0
             115: 11(HitTest)   Load 14(a)
                                ReturnValue 115
             117:               Label
                                Line 1 171 0
             118: 11(HitTest)   Load 15(b)
                                ReturnValue 118
             114:             Label
             120: 11(HitTest) Undef
                              ReturnValue 120
                              FunctionEnd
22(minT(struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;): 11(HitTest) Function None 18
           19(a):     12(ptr) FunctionParameter
           20(b):     12(ptr) FunctionParameter
           21(c):     12(ptr) FunctionParameter
              23:             Label
      121(param):     12(ptr) Variable Function
      123(param):     12(ptr) Variable Function
      126(param):     12(ptr) Variable Function
      128(param):     12(ptr) Variable Function
                              Line 1 180 0
             122: 11(HitTest) Load 20(b)
                              Store 121(param) 122
             124: 11(HitTest) Load 21(c)
                              Store 123(param) 124
             125: 11(HitTest) FunctionCall 16(minT(struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;) 121(param) 123(param)
             127: 11(HitTest) Load 19(a)
                              Store 126(param) 127
                              Store 128(param) 125
             129: 11(HitTest) FunctionCall 16(minT(struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;) 126(param) 128(param)
                              ReturnValue 129
                              FunctionEnd
30(minT(struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;): 11(HitTest) Function None 24
           25(a):     12(ptr) FunctionParameter
           26(b):     12(ptr) FunctionParameter
           27(c):     12(ptr) FunctionParameter
           28(d):     12(ptr) FunctionParameter
           29(e):     12(ptr) FunctionParameter
              31:             Label
      132(param):     12(ptr) Variable Function
      134(param):     12(ptr) Variable Function
      137(param):     12(ptr) Variable Function
      139(param):     12(ptr) Variable Function
      141(param):     12(ptr) Variable Function
      144(param):     12(ptr) Variable Function
      145(param):     12(ptr) Variable Function
                              Line 1 186 0
             133: 11(HitTest) Load 25(a)
                              Store 132(param) 133
             135: 11(HitTest) Load 26(b)
                              Store 134(param) 135
             136: 11(HitTest) FunctionCall 16(minT(struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;) 132(param) 134(param)
             138: 11(HitTest) Load 27(c)
                              Store 137(param) 138
             140: 11(HitTest) Load 28(d)
                              Store 139(param) 140
             142: 11(HitTest) Load 29(e)
                              Store 141(param) 142
             143: 11(HitTest) FunctionCall 22(minT(struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;) 137(param) 139(param) 141(param)
                              Store 144(param) 136
                              Store 145(param) 143
             146: 11(HitTest) FunctionCall 16(minT(struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;) 144(param) 145(param)
                              ReturnValue 146
                              FunctionEnd
39(intersectBox(struct-Ray-vf3-vf31;struct-Box-vf3-vf31;): 11(HitTest) Function None 36
           37(r):     33(ptr) FunctionParameter
           38(b):     35(ptr) FunctionParameter
              40:             Label
         154(ba):     63(ptr) Variable Function
         160(bb):     63(ptr) Variable Function
          166(h):     12(ptr) Variable Function
          173(d):     56(ptr) Variable Function
         174(dA):     63(ptr) Variable Function
         183(dB):     63(ptr) Variable Function
             192:     56(ptr) Variable Function
             204:     56(ptr) Variable Function
             215:     56(ptr) Variable Function
             227:     56(ptr) Variable Function
             238:     56(ptr) Variable Function
             249:     56(ptr) Variable Function
         260(d1):     56(ptr) Variable Function
         269(d2):     56(ptr) Variable Function
   281(endPoint):     63(ptr) Variable Function
          315(l):    298(ptr) Variable Function
                              Line 1 192 0
             150:     63(ptr) AccessChain 38(b) 107
             151:    9(fvec3) Load 150
             152:    9(fvec3) VectorTimesScalar 151 149
             153:     63(ptr) AccessChain 38(b) 107
                              Store 153 152
                              Line 1 194 0
             155:     63(ptr) AccessChain 38(b) 91
             156:    9(fvec3) Load 155
             157:     63(ptr) AccessChain 38(b) 107
             158:    9(fvec3) Load 157
             159:    9(fvec3) FSub 156 158
                              Store 154(ba) 159
                              Line 1 196 0
             161:     63(ptr) AccessChain 38(b) 91
             162:    9(fvec3) Load 161
             163:     63(ptr) AccessChain 38(b) 107
             164:    9(fvec3) Load 163
             165:    9(fvec3) FAdd 162 164
                              Store 160(bb) 165
                              Line 1 198 0
                              Store 166(h) 172
                              Line 1 200 0
                              Store 173(d) 168
                              Line 1 202 0
             175:     63(ptr) AccessChain 37(r) 91
             176:    9(fvec3) Load 175
             177:    9(fvec3) Load 154(ba)
             178:    9(fvec3) FSub 176 177
             179:     63(ptr) AccessChain 37(r) 107
             180:    9(fvec3) Load 179
             181:    9(fvec3) FNegate 180
             182:    9(fvec3) FDiv 178 181
                              Store 174(dA) 182
                              Line 1 204 0
             184:     63(ptr) AccessChain 37(r) 91
             185:    9(fvec3) Load 184
             186:    9(fvec3) Load 160(bb)
             187:    9(fvec3) FSub 185 186
             188:     63(ptr) AccessChain 37(r) 107
             189:    9(fvec3) Load 188
             190:    9(fvec3) FNegate 189
             191:    9(fvec3) FDiv 187 190
                              Store 183(dB) 191
                              Line 1 205 0
             194:     56(ptr) AccessChain 174(dA) 193
             195:    8(float) Load 194
             196:     7(bool) FOrdLessThanEqual 195 169
                              SelectionMerge 198 None
                              BranchConditional 196 197 199
             197:               Label
                                Store 192 168
                                Branch 198
             199:               Label
             200:     56(ptr)   AccessChain 174(dA) 193
             201:    8(float)   Load 200
                                Store 192 201
                                Branch 198
             198:             Label
             202:    8(float) Load 192
             203:     56(ptr) AccessChain 174(dA) 193
                              Store 203 202
                              Line 1 206 0
             205:     56(ptr) AccessChain 174(dA) 92
             206:    8(float) Load 205
             207:     7(bool) FOrdLessThanEqual 206 169
                              SelectionMerge 209 None
                              BranchConditional 207 208 210
             208:               Label
                                Store 204 168
                                Branch 209
             210:               Label
             211:     56(ptr)   AccessChain 174(dA) 92
             212:    8(float)   Load 211
                                Store 204 212
                                Branch 209
             209:             Label
             213:    8(float) Load 204
             214:     56(ptr) AccessChain 174(dA) 92
                              Store 214 213
                              Line 1 207 0
             217:     56(ptr) AccessChain 174(dA) 216
             218:    8(float) Load 217
             219:     7(bool) FOrdLessThanEqual 218 169
                              SelectionMerge 221 None
                              BranchConditional 219 220 222
             220:               Label
                                Store 215 168
                                Branch 221
             222:               Label
             223:     56(ptr)   AccessChain 174(dA) 216
             224:    8(float)   Load 223
                                Store 215 224
                                Branch 221
             221:             Label
             225:    8(float) Load 215
             226:     56(ptr) AccessChain 174(dA) 216
                              Store 226 225
                              Line 1 208 0
             228:     56(ptr) AccessChain 183(dB) 193
             229:    8(float) Load 228
             230:     7(bool) FOrdLessThanEqual 229 169
                              SelectionMerge 232 None
                              BranchConditional 230 231 233
             231:               Label
                                Store 227 168
                                Branch 232
             233:               Label
             234:     56(ptr)   AccessChain 183(dB) 193
             235:    8(float)   Load 234
                                Store 227 235
                                Branch 232
             232:             Label
             236:    8(float) Load 227
             237:     56(ptr) AccessChain 183(dB) 193
                              Store 237 236
                              Line 1 209 0
             239:     56(ptr) AccessChain 183(dB) 92
             240:    8(float) Load 239
             241:     7(bool) FOrdLessThanEqual 240 169
                              SelectionMerge 243 None
                              BranchConditional 241 242 244
             242:               Label
                                Store 238 168
                                Branch 243
             244:               Label
             245:     56(ptr)   AccessChain 183(dB) 92
             246:    8(float)   Load 245
                                Store 238 246
                                Branch 243
             243:             Label
             247:    8(float) Load 238
             248:     56(ptr) AccessChain 183(dB) 92
                              Store 248 247
                              Line 1 210 0
             250:     56(ptr) AccessChain 183(dB) 216
             251:    8(float) Load 250
             252:     7(bool) FOrdLessThanEqual 251 169
                              SelectionMerge 254 None
                              BranchConditional 252 253 255
             253:               Label
                                Store 249 168
                                Branch 254
             255:               Label
             256:     56(ptr)   AccessChain 183(dB) 216
             257:    8(float)   Load 256
                                Store 249 257
                                Branch 254
             254:             Label
             258:    8(float) Load 249
             259:     56(ptr) AccessChain 183(dB) 216
                              Store 259 258
                              Line 1 212 0
             261:     56(ptr) AccessChain 174(dA) 193
             262:    8(float) Load 261
             263:     56(ptr) AccessChain 174(dA) 92
             264:    8(float) Load 263
             265:     56(ptr) AccessChain 174(dA) 216
             266:    8(float) Load 265)#"_str
+
R"#(
             267:    8(float) ExtInst 2(GLSL.std.450) 37(FMin) 264 266
             268:    8(float) ExtInst 2(GLSL.std.450) 37(FMin) 262 267
                              Store 260(d1) 268
                              Line 1 214 0
             270:     56(ptr) AccessChain 183(dB) 193
             271:    8(float) Load 270
             272:     56(ptr) AccessChain 183(dB) 92
             273:    8(float) Load 272
             274:     56(ptr) AccessChain 183(dB) 216
             275:    8(float) Load 274
             276:    8(float) ExtInst 2(GLSL.std.450) 37(FMin) 273 275
             277:    8(float) ExtInst 2(GLSL.std.450) 37(FMin) 271 276
                              Store 269(d2) 277
                              Line 1 215 0
             278:    8(float) Load 260(d1)
             279:    8(float) Load 269(d2)
             280:    8(float) ExtInst 2(GLSL.std.450) 37(FMin) 278 279
                              Store 173(d) 280
                              Line 1 217 0
             282:     63(ptr) AccessChain 37(r) 91
             283:    9(fvec3) Load 282
             284:     63(ptr) AccessChain 37(r) 107
             285:    9(fvec3) Load 284
             286:    8(float) Load 173(d)
             287:    9(fvec3) VectorTimesScalar 285 286
             288:    9(fvec3) FAdd 283 287
                              Store 281(endPoint) 288
                              Line 1 218 0
             289:     63(ptr) AccessChain 38(b) 91
             290:    9(fvec3) Load 289
             291:    9(fvec3) Load 281(endPoint)
             292:    9(fvec3) FSub 291 290
                              Store 281(endPoint) 292
                              Line 1 219 0
             293:    8(float) Load 173(d)
             294:     7(bool) FOrdNotEqual 293 168
                              SelectionMerge 296 None
                              BranchConditional 294 295 296
             295:               Label
                                Line 1 221 0
             299:    298(ptr)   AccessChain 166(h) 91
                                Store 299 297
                                Line 1 222 0
             300:    8(float)   Load 173(d)
             301:     56(ptr)   AccessChain 166(h) 107
                                Store 301 300
                                Line 1 223 0
             303:     56(ptr)   AccessChain 166(h) 302
                                Store 303 169
                                Line 1 224 0
             304:     56(ptr)   AccessChain 281(endPoint) 193
             305:    8(float)   Load 304
             306:    8(float)   ExtInst 2(GLSL.std.450) 4(FAbs) 305
             307:     56(ptr)   AccessChain 38(b) 107 193
             308:    8(float)   Load 307
             309:    8(float)   FSub 306 308
             310:    8(float)   ExtInst 2(GLSL.std.450) 4(FAbs) 309
             312:     7(bool)   FOrdLessThan 310 311
                                SelectionMerge 314 None
                                BranchConditional 312 313 314
             313:                 Label
                                  Line 1 227 0
             316:     56(ptr)     AccessChain 281(endPoint) 193
             317:    8(float)     Load 316
             318:     7(bool)     FOrdLessThan 317 169
                                  Store 315(l) 318
                                  Line 1 228 0
             320:     7(bool)     Load 315(l)
             322:     82(int)     Select 320 107 321
             323:    8(float)     ConvertSToF 322
             324:    9(fvec3)     CompositeConstruct 323 169 169
             325:     63(ptr)     AccessChain 166(h) 319
                                  Store 325 324
                                  Line 1 229 0
             327:     7(bool)     Load 315(l)
             332:  331(bvec4)     CompositeConstruct 327 327 327 327
             333:   10(fvec4)     Select 332 329 330
             334:     71(ptr)     AccessChain 166(h) 326
                                  Store 334 333
                                  Line 1 230 0
             335: 11(HitTest)     Load 166(h)
                                  ReturnValue 335
             314:               Label
                                Line 1 234 0
             337:     56(ptr)   AccessChain 281(endPoint) 216
             338:    8(float)   Load 337
             339:    8(float)   ExtInst 2(GLSL.std.450) 4(FAbs) 338
             340:     56(ptr)   AccessChain 38(b) 107 216
             341:    8(float)   Load 340
             342:    8(float)   FSub 339 341
             343:    8(float)   ExtInst 2(GLSL.std.450) 4(FAbs) 342
             344:     7(bool)   FOrdLessThan 343 311
                                SelectionMerge 346 None
                                BranchConditional 344 345 346
             345:                 Label
                                  Line 1 236 0
             347:     56(ptr)     AccessChain 281(endPoint) 216
             348:    8(float)     Load 347
             349:    8(float)     ExtInst 2(GLSL.std.450) 6(FSign) 348
             350:    8(float)     FNegate 349
             351:    9(fvec3)     CompositeConstruct 169 169 350
             352:     63(ptr)     AccessChain 166(h) 319
                                  Store 352 351
                                  Line 1 237 0
             354:     71(ptr)     AccessChain 166(h) 326
                                  Store 354 353
                                  Line 1 238 0
             355: 11(HitTest)     Load 166(h)
                                  ReturnValue 355
             346:               Label
                                Line 1 242 0
             357:     56(ptr)   AccessChain 281(endPoint) 92
             358:    8(float)   Load 357
             359:    8(float)   ExtInst 2(GLSL.std.450) 6(FSign) 358
             360:    8(float)   FNegate 359
             361:    9(fvec3)   CompositeConstruct 169 360 169
             362:     63(ptr)   AccessChain 166(h) 319
                                Store 362 361
                                Line 1 243 0
             364:     56(ptr)   AccessChain 281(endPoint) 92
             365:    8(float)   Load 364
             366:    8(float)   ExtInst 2(GLSL.std.450) 6(FSign) 365
             369:    8(float)   Load 368(t)
             371:    8(float)   FMul 369 370
             372:    8(float)   ExtInst 2(GLSL.std.450) 13(Sin) 371
             374:    8(float)   FAdd 372 373
             377:    8(float)   ExtInst 2(GLSL.std.450) 43(FClamp) 374 375 376
             378:    8(float)   FMul 366 377
             379:   10(fvec4)   CompositeConstruct 363 363 363 378
             380:     71(ptr)   AccessChain 166(h) 326
                                Store 380 379
                                Line 1 244 0
             381: 11(HitTest)   Load 166(h)
                                ReturnValue 381
             296:             Label
                              Line 1 248 0
             383: 11(HitTest) Load 166(h)
                              ReturnValue 383
                              FunctionEnd
46(intersectSphere(struct-Ray-vf3-vf31;struct-Sphere-vf3-f11;): 11(HitTest) Function None 43
           44(r):     33(ptr) FunctionParameter
           45(s):     42(ptr) FunctionParameter
              47:             Label
          386(o):     63(ptr) Variable Function
          392(v):     56(ptr) Variable Function
       402(disc):     56(ptr) Variable Function
       421(dist):     56(ptr) Variable Function
                              Line 1 255 0
             387:     63(ptr) AccessChain 44(r) 91
             388:    9(fvec3) Load 387
             389:     63(ptr) AccessChain 45(s) 91
             390:    9(fvec3) Load 389
             391:    9(fvec3) FSub 388 390
                              Store 386(o) 391
                              Line 1 257 0
             393:    9(fvec3) Load 386(o)
             394:     63(ptr) AccessChain 44(r) 107
             395:    9(fvec3) Load 394
             396:    8(float) Dot 393 395
                              Store 392(v) 396
                              Line 1 258 0
             397:    8(float) Load 392(v)
             398:     7(bool) FOrdGreaterThan 397 169
                              SelectionMerge 400 None
                              BranchConditional 398 399 400
             399:               Label
                                Line 1 260 0
                                ReturnValue 172
             400:             Label
                              Line 1 264 0
             403:     56(ptr) AccessChain 45(s) 107
             404:    8(float) Load 403
             405:     56(ptr) AccessChain 45(s) 107
             406:    8(float) Load 405
             407:    8(float) FMul 404 406
             408:    9(fvec3) Load 386(o)
             409:    9(fvec3) Load 386(o)
             410:    8(float) Dot 408 409
             411:    8(float) Load 392(v)
             412:    8(float) Load 392(v)
             413:    8(float) FMul 411 412
             414:    8(float) FSub 410 413
             415:    8(float) FSub 407 414
                              Store 402(disc) 415
                              Line 1 265 0
             416:    8(float) Load 402(disc)
             417:     7(bool) FOrdLessThan 416 169
                              SelectionMerge 419 None
                              BranchConditional 417 418 419
             418:               Label
                                Line 1 267 0
                                ReturnValue 172
             419:             Label
                              Line 1 271 0
             422:    9(fvec3) Load 386(o)
             423:    8(float) ExtInst 2(GLSL.std.450) 66(Length) 422
             424:    8(float) Load 402(disc)
             425:    8(float) ExtInst 2(GLSL.std.450) 31(Sqrt) 424
             426:    8(float) FSub 423 425
                              Store 421(dist) 426
                              Line 1 272 0
             427:    8(float) Load 421(dist)
             428:     63(ptr) AccessChain 44(r) 91
             429:    9(fvec3) Load 428
             430:     63(ptr) AccessChain 44(r) 107
             431:    9(fvec3) Load 430
             432:    8(float) Load 421(dist)
             433:    9(fvec3) VectorTimesScalar 431 432
             434:    9(fvec3) FAdd 429 433
             435:     63(ptr) AccessChain 45(s) 91
             436:    9(fvec3) Load 435
             437:    9(fvec3) FSub 434 436
             438:    9(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 437
             439:     93(ptr) AccessChain 90 107
             440:    8(float) Load 439
             442:    8(float) FMul 440 441
             443:    8(float) ExtInst 2(GLSL.std.450) 13(Sin) 442
             444:    8(float) FMul 443 149
             445:    8(float) FAdd 444 149
             446: 11(HitTest) CompositeConstruct 297 427 438 171 445
                              ReturnValue 446
                              FunctionEnd
  52(nrand(vf2;):    8(float) Function None 50
           51(n):     49(ptr) FunctionParameter
              53:             Label
                              Line 1 278 0
             449:   48(fvec2) Load 51(n)
             453:    8(float) Dot 449 452
             454:    8(float) ExtInst 2(GLSL.std.450) 13(Sin) 453
             456:    8(float) FMul 454 455
             457:    8(float) ExtInst 2(GLSL.std.450) 10(Fract) 456
                              ReturnValue 457
                              FunctionEnd
61(traceScene(struct-Camera-vf3-struct-Ray-vf3-vf311;vf2;f1;):   10(fvec4) Function None 57
         58(cam):     55(ptr) FunctionParameter
        59(seed):     49(ptr) FunctionParameter
       60(lastB):     56(ptr) FunctionParameter
              62:             Label
   460(startPos):     63(ptr) Variable Function
     463(result):     71(ptr) Variable Function
       465(maxI):    464(ptr) Variable Function
          470(i):    464(ptr) Variable Function
          485(r):     33(ptr) Variable Function
      488(param):     49(ptr) Variable Function
      501(param):     49(ptr) Variable Function
      513(param):     49(ptr) Variable Function
     529(impact):     71(ptr) Variable Function
       534(maxJ):    464(ptr) Variable Function
          539(j):    464(ptr) Variable Function
         548(t0):     12(ptr) Variable Function
      555(param):     33(ptr) Variable Function
      557(param):     35(ptr) Variable Function
         559(t1):     12(ptr) Variable Function
      563(param):     33(ptr) Variable Function
      565(param):     42(ptr) Variable Function
         570(t2):     12(ptr) Variable Function
      575(param):     33(ptr) Variable Function
      577(param):     42(ptr) Variable Function
         582(t3):     12(ptr) Variable Function
      585(param):     33(ptr) Variable Function
      587(param):     42(ptr) Variable Function
         598(t4):     12(ptr) Variable Function
      603(param):     33(ptr) Variable Function
      605(param):     42(ptr) Variable Function
       615(test):     12(ptr) Variable Function
      616(param):     12(ptr) Variable Function
      618(param):     12(ptr) Variable Function
      620(param):     12(ptr) Variable Function
      622(param):     12(ptr) Variable Function
      624(param):     12(ptr) Variable Function
     666(random):     63(ptr) Variable Function
      672(param):     49(ptr) Variable Function
      679(param):     49(ptr) Variable Function
      686(param):     49(ptr) Variable Function
       742(refl):     63(ptr) Variable Function
      748(matte):     63(ptr) Variable Function
          754(s):     56(ptr) Variable Function
      764(param):     49(ptr) Variable Function
                              Line 1 285 0
             461:     63(ptr) AccessChain 58(cam) 91
             462:    9(fvec3) Load 461
                              Store 460(startPos) 462
                              Line 1 287 0
                              Store 463(result) 171
                              Line 1 289 0
             467:    8(float) Load 60(lastB)
             468:    8(float) FMul 466 467
             469:     82(int) ConvertFToS 468
                              Store 465(maxI) 469
                              Line 1 292 0
                              Store 470(i) 91
                              Branch 471
                              Line 1 293 0
             471:             Label
                              LoopMerge 473 474 None
                              Branch 475
             475:             Label
             476:     82(int) Load 470(i)
             478:     7(bool) SLessThan 476 477
                              BranchConditional 478 472 473
             472:               Label
                                Line 1 295 0
             479:     82(int)   Load 470(i)
             480:     82(int)   Load 465(maxI)
             481:     7(bool)   IEqual 479 480
                                SelectionMerge 483 None
                                BranchConditional 481 482 483
             482:                 Label
                                  Line 1 297 0
                                  Branch 473
             483:               Label
                                Line 1 301 0
             486:     33(ptr)   AccessChain 58(cam) 107
             487:     32(Ray)   Load 486
                                Store 485(r) 487
                                Line 1 302 0
             489:   48(fvec2)   Load 59(seed)
                                Store 488(param) 489
             490:    8(float)   FunctionCall 52(nrand(vf2;) 488(param)
             492:    8(float)   FMul 490 491
             493:    8(float)   FSub 492 363
             495:    8(float)   Load 494(divergence)
             496:    8(float)   FMul 493 495
             497:     56(ptr)   AccessChain 485(r) 107 193
             498:    8(float)   Load 497
             499:    8(float)   FAdd 498 496)#"_str
+
R"#(
             500:     56(ptr)   AccessChain 485(r) 107 193
                                Store 500 499
                                Line 1 303 0
             502:   48(fvec2)   Load 59(seed)
             503:   48(fvec2)   VectorShuffle 502 502 1 0
                                Store 501(param) 503
             504:    8(float)   FunctionCall 52(nrand(vf2;) 501(param)
             505:    8(float)   FMul 504 491
             506:    8(float)   FSub 505 363
             507:    8(float)   Load 494(divergence)
             508:    8(float)   FMul 506 507
             509:     56(ptr)   AccessChain 485(r) 107 92
             510:    8(float)   Load 509
             511:    8(float)   FAdd 510 508
             512:     56(ptr)   AccessChain 485(r) 107 92
                                Store 512 511
                                Line 1 304 0
             514:   48(fvec2)   Load 59(seed)
             515:   48(fvec2)   VectorShuffle 514 514 0 0
                                Store 513(param) 515
             516:    8(float)   FunctionCall 52(nrand(vf2;) 513(param)
             517:    8(float)   FMul 516 491
             518:    8(float)   FSub 517 363
             519:    8(float)   Load 494(divergence)
             520:    8(float)   FMul 518 519
             521:     56(ptr)   AccessChain 485(r) 107 216
             522:    8(float)   Load 521
             523:    8(float)   FAdd 522 520
             524:     56(ptr)   AccessChain 485(r) 107 216
                                Store 524 523
                                Line 1 305 0
             525:     63(ptr)   AccessChain 485(r) 107
             526:    9(fvec3)   Load 525
             527:    9(fvec3)   ExtInst 2(GLSL.std.450) 69(Normalize) 526
             528:     63(ptr)   AccessChain 485(r) 107
                                Store 528 527
                                Line 1 307 0
                                Store 529(impact) 530
                                Line 1 308 0
             531:   48(fvec2)   Load 59(seed)
             532:   48(fvec2)   CompositeConstruct 363 363
             533:   48(fvec2)   FAdd 531 532
                                Store 59(seed) 533
                                Line 1 310 0
             536:    8(float)   Load 60(lastB)
             537:    8(float)   FMul 535 536
             538:     82(int)   ConvertFToS 537
                                Store 534(maxJ) 538
                                Line 1 313 0
                                Store 539(j) 91
                                Branch 540
                                Line 1 314 0
             540:               Label
                                LoopMerge 542 543 None
                                Branch 544
             544:               Label
             545:     82(int)   Load 539(j)
             547:     7(bool)   SLessThan 545 546
                                BranchConditional 547 541 542
             541:                 Label
                                  Line 1 317 0
             556:     32(Ray)     Load 485(r)
                                  Store 555(param) 556
                                  Store 557(param) 554
             558: 11(HitTest)     FunctionCall 39(intersectBox(struct-Ray-vf3-vf31;struct-Box-vf3-vf31;) 555(param) 557(param)
                                  Store 548(t0) 558
                                  Line 1 319 0
             564:     32(Ray)     Load 485(r)
                                  Store 563(param) 564
                                  Store 565(param) 562
             566: 11(HitTest)     FunctionCall 46(intersectSphere(struct-Ray-vf3-vf31;struct-Sphere-vf3-f11;) 563(param) 565(param)
                                  Store 559(t1) 566
                                  Line 1 320 0
             569:     71(ptr)     AccessChain 559(t1) 326
                                  Store 569 568
                                  Line 1 322 0
             576:     32(Ray)     Load 485(r)
                                  Store 575(param) 576
                                  Store 577(param) 574
             578: 11(HitTest)     FunctionCall 46(intersectSphere(struct-Ray-vf3-vf31;struct-Sphere-vf3-f11;) 575(param) 577(param)
                                  Store 570(t2) 578
                                  Line 1 323 0
             580:     71(ptr)     AccessChain 570(t2) 326
                                  Store 580 579
                                  Line 1 324 0
             581:     56(ptr)     AccessChain 570(t2) 302
                                  Store 581 373
                                  Line 1 326 0
             586:     32(Ray)     Load 485(r)
                                  Store 585(param) 586
                                  Store 587(param) 584
             588: 11(HitTest)     FunctionCall 46(intersectSphere(struct-Ray-vf3-vf31;struct-Sphere-vf3-f11;) 585(param) 587(param)
                                  Store 582(t3) 588
                                  Line 1 327 0
             590:    8(float)     Load 368(t)
             591:    8(float)     FMul 590 567
             592:    8(float)     ExtInst 2(GLSL.std.450) 13(Sin) 591
             594:    8(float)     FMul 592 593
             595:   10(fvec4)     CompositeConstruct 589 328 567 594
             596:     71(ptr)     AccessChain 582(t3) 326
                                  Store 596 595
                                  Line 1 328 0
             597:     56(ptr)     AccessChain 582(t3) 302
                                  Store 597 169
                                  Line 1 330 0
             604:     32(Ray)     Load 485(r)
                                  Store 603(param) 604
                                  Store 605(param) 602
             606: 11(HitTest)     FunctionCall 46(intersectSphere(struct-Ray-vf3-vf31;struct-Sphere-vf3-f11;) 603(param) 605(param)
                                  Store 598(t4) 606
                                  Line 1 331 0
             607:    8(float)     Load 368(t)
             609:    8(float)     FMul 607 608
             610:    8(float)     ExtInst 2(GLSL.std.450) 13(Sin) 609
             611:    8(float)     FMul 610 593
             612:   10(fvec4)     CompositeConstruct 363 149 375 611
             613:     71(ptr)     AccessChain 598(t4) 326
                                  Store 613 612
                                  Line 1 332 0
             614:     56(ptr)     AccessChain 598(t4) 302
                                  Store 614 169
                                  Line 1 334 0
             617: 11(HitTest)     Load 548(t0)
                                  Store 616(param) 617
             619: 11(HitTest)     Load 559(t1)
                                  Store 618(param) 619
             621: 11(HitTest)     Load 570(t2)
                                  Store 620(param) 621
             623: 11(HitTest)     Load 582(t3)
                                  Store 622(param) 623
             625: 11(HitTest)     Load 598(t4)
                                  Store 624(param) 625
             626: 11(HitTest)     FunctionCall 30(minT(struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;struct-HitTest-b1-f1-vf3-vf4-f11;) 616(param) 618(param) 620(param) 622(param) 624(param)
                                  Store 615(test) 626
                                  Line 1 335 0
             627:    298(ptr)     AccessChain 615(test) 91
             628:     7(bool)     Load 627
                                  SelectionMerge 630 None
                                  BranchConditional 628 629 793
             629:                   Label
                                    Line 1 337 0
             631:     71(ptr)       AccessChain 615(test) 326
             632:   10(fvec4)       Load 631
             633:   10(fvec4)       Load 529(impact)
             634:   10(fvec4)       FMul 633 632
                                    Store 529(impact) 634
                                    Line 1 338 0
             636:     56(ptr)       AccessChain 615(test) 326 635
             637:    8(float)       Load 636
             638:     7(bool)       FOrdGreaterThan 637 169
                                    SelectionMerge 640 None
                                    BranchConditional 638 639 640
             639:                     Label
                                      Line 1 340 0
             641:     71(ptr)         AccessChain 615(test) 326
             642:   10(fvec4)         Load 641
             643:   10(fvec4)         Load 529(impact)
             644:   10(fvec4)         FMul 642 643
             645:     56(ptr)         AccessChain 615(test) 326 635
             646:    8(float)         Load 645
             647:   10(fvec4)         VectorTimesScalar 644 646
             648:   10(fvec4)         Load 463(result)
             649:   10(fvec4)         FAdd 648 647
                                      Store 463(result) 649
                                      Branch 640
             640:                   Label
                                    Line 1 344 0
             650:     63(ptr)       AccessChain 485(r) 107
             651:    9(fvec3)       Load 650
             652:     56(ptr)       AccessChain 615(test) 107
             653:    8(float)       Load 652
             654:    9(fvec3)       VectorTimesScalar 651 653
             655:     63(ptr)       AccessChain 485(r) 91
             656:    9(fvec3)       Load 655
             657:    9(fvec3)       FAdd 656 654
             658:     63(ptr)       AccessChain 485(r) 91
                                    Store 658 657
                                    Line 1 345 0
             659:     63(ptr)       AccessChain 615(test) 319
             660:    9(fvec3)       Load 659
             661:    9(fvec3)       VectorTimesScalar 660 311
             662:     63(ptr)       AccessChain 485(r) 91
             663:    9(fvec3)       Load 662
             664:    9(fvec3)       FAdd 663 661
             665:     63(ptr)       AccessChain 485(r) 91
                                    Store 665 664
                                    Line 1 347 0
             667:     63(ptr)       AccessChain 485(r) 91
             668:    9(fvec3)       Load 667
             669:   48(fvec2)       VectorShuffle 668 668 0 1
             670:   48(fvec2)       Load 59(seed)
             671:   48(fvec2)       FAdd 669 670
                                    Store 672(param) 671
             673:    8(float)       FunctionCall 52(nrand(vf2;) 672(param)
             674:     63(ptr)       AccessChain 485(r) 91
             675:    9(fvec3)       Load 674
             676:   48(fvec2)       VectorShuffle 675 675 1 2
             677:   48(fvec2)       Load 59(seed)
             678:   48(fvec2)       FAdd 676 677
                                    Store 679(param) 678
             680:    8(float)       FunctionCall 52(nrand(vf2;) 679(param)
             681:     63(ptr)       AccessChain 485(r) 91
             682:    9(fvec3)       Load 681
             683:   48(fvec2)       VectorShuffle 682 682 2 0
             684:   48(fvec2)       Load 59(seed)
             685:   48(fvec2)       FAdd 683 684
                                    Store 686(param) 685
             687:    8(float)       FunctionCall 52(nrand(vf2;) 686(param)
             688:    9(fvec3)       CompositeConstruct 673 680 687
             689:    9(fvec3)       VectorTimesScalar 688 491
             690:    9(fvec3)       CompositeConstruct 363 363 363
             691:    9(fvec3)       FSub 689 690
                                    Store 666(random) 691
                                    Line 1 348 0
             692: 11(HitTest)       Load 615(test)
             693: 11(HitTest)       Load 548(t0)
             694:     7(bool)       CompositeExtract 692 0
             695:     7(bool)       CompositeExtract 693 0
             696:     7(bool)       LogicalNotEqual 694 695
             697:    8(float)       CompositeExtract 692 1
             698:    8(float)       CompositeExtract 693 1
             699:     7(bool)       FOrdNotEqual 697 698
             700:     7(bool)       LogicalOr 696 699
             701:    9(fvec3)       CompositeExtract 692 2
             702:    9(fvec3)       CompositeExtract 693 2
             704:  703(bvec3)       FOrdNotEqual 701 702
             705:     7(bool)       Any 704
             706:     7(bool)       LogicalOr 700 705
             707:   10(fvec4)       CompositeExtract 692 3
             708:   10(fvec4)       CompositeExtract 693 3
             709:  331(bvec4)       FOrdNotEqual 707 708
             710:     7(bool)       Any 709
             711:     7(bool)       LogicalOr 706 710
             712:    8(float)       CompositeExtract 692 4
             713:    8(float)       CompositeExtract 693 4
             714:     7(bool)       FOrdNotEqual 712 713
             715:     7(bool)       LogicalOr 711 714
             716: 11(HitTest)       Load 615(test)
             717: 11(HitTest)       Load 570(t2)
             718:     7(bool)       CompositeExtract 716 0
             719:     7(bool)       CompositeExtract 717 0
             720:     7(bool)       LogicalNotEqual 718 719
             721:    8(float)       CompositeExtract 716 1
             722:    8(float)       CompositeExtract 717 1
             723:     7(bool)       FOrdNotEqual 721 722
             724:     7(bool)       LogicalOr 720 723
             725:    9(fvec3)       CompositeExtract 716 2
             726:    9(fvec3)       CompositeExtract 717 2
             727:  703(bvec3)       FOrdNotEqual 725 726
             728:     7(bool)       Any 727
             729:     7(bool)       LogicalOr 724 728
             730:   10(fvec4)       CompositeExtract 716 3
             731:   10(fvec4)       CompositeExtract 717 3
             732:  331(bvec4)       FOrdNotEqual 730 731
             733:     7(bool)       Any 732
             734:     7(bool)       LogicalOr 729 733
             735:    8(float)       CompositeExtract 716 4
             736:    8(float)       CompositeExtract 717 4
             737:     7(bool)       FOrdNotEqual 735 736
             738:     7(bool)       LogicalOr 734 737
             739:     7(bool)       LogicalAnd 715 738
                                    SelectionMerge 741 None
                                    BranchConditional 739 740 777
             740:                     Label
                                      Line 1 351 0
             743:     63(ptr)         AccessChain 485(r) 107
             744:    9(fvec3)         Load 743
             745:     63(ptr)         AccessChain 615(test) 319
             746:    9(fvec3)         Load 745
             747:    9(fvec3)         ExtInst 2(GLSL.std.450) 71(Reflect) 744 746
                                      Store 742(refl) 747
                                      Line 1 353 0
             749:     63(ptr)         AccessChain 615(test) 319
             750:    9(fvec3)         Load 749
             751:    9(fvec3)         Load 666(random)
             752:    9(fvec3)         FAdd 750 751
             753:    9(fvec3)         ExtInst 2(GLSL.std.450) 69(Normalize) 752
                                      Store 748(matte) 753
                                      Line 1 355 0
             755:     63(ptr)         AccessChain 615(test) 319
             756:    9(fvec3)         Load 755
             757:     63(ptr)         AccessChain 485(r) 107
             758:    9(fvec3)         Load 757
             759:    8(float)         Dot 756 758
             760:    8(float)         FNegate 759
             761:    8(float)         ExtInst 2(GLSL.std.450) 40(FMax) 169 760
                                      Store 754(s) 761
                                      Line 1 356 0)#"_str
+
R"#(
             762:    8(float)         Load 754(s)
             763:    8(float)         ExtInst 2(GLSL.std.450) 26(Pow) 762 149
             765:   48(fvec2)         Load 59(seed)
                                      Store 764(param) 765
             766:    8(float)         FunctionCall 52(nrand(vf2;) 764(param)
             767:    8(float)         ExtInst 2(GLSL.std.450) 48(Step) 763 766
                                      Store 754(s) 767
                                      Line 1 357 0
             768:    9(fvec3)         Load 742(refl)
             769:    8(float)         Load 754(s)
             770:    9(fvec3)         VectorTimesScalar 768 769
             771:    9(fvec3)         Load 748(matte)
             772:    8(float)         Load 754(s)
             773:    8(float)         FSub 363 772
             774:    9(fvec3)         VectorTimesScalar 771 773
             775:    9(fvec3)         FAdd 770 774
             776:     63(ptr)         AccessChain 485(r) 107
                                      Store 776 775
                                      Branch 741
             777:                     Label
                                      Line 1 362 0
             778:     63(ptr)         AccessChain 615(test) 319
             779:    9(fvec3)         Load 778
             780:    9(fvec3)         Load 666(random)
             781:    9(fvec3)         FAdd 779 780
             782:     63(ptr)         AccessChain 485(r) 107
             783:    9(fvec3)         Load 782
             784:     63(ptr)         AccessChain 615(test) 319
             785:    9(fvec3)         Load 784
             786:    9(fvec3)         ExtInst 2(GLSL.std.450) 71(Reflect) 783 785
             787:     56(ptr)         AccessChain 615(test) 302
             788:    8(float)         Load 787
             789:    9(fvec3)         CompositeConstruct 788 788 788
             790:    9(fvec3)         ExtInst 2(GLSL.std.450) 46(FMix) 781 786 789
             791:    9(fvec3)         ExtInst 2(GLSL.std.450) 69(Normalize) 790
             792:     63(ptr)         AccessChain 485(r) 107
                                      Store 792 791
                                      Branch 741
             741:                   Label
                                    Branch 630
             793:                   Label
                                    Line 1 370 0
                                    Branch 542
             630:                 Label
                                  Branch 543
             543:                 Label
                                  Line 1 314 0
             795:     82(int)     Load 539(j)
             796:     82(int)     IAdd 795 107
                                  Store 539(j) 796
                                  Branch 540
             542:               Label
                                Branch 474
             474:               Label
                                Line 1 293 0
             797:     82(int)   Load 470(i)
             798:     82(int)   IAdd 797 107
                                Store 470(i) 798
                                Branch 471
             473:             Label
                              Line 1 380 0
             799:   10(fvec4) Load 463(result)
             800:   10(fvec4) CompositeConstruct 466 466 466 466
             801:   10(fvec4) FDiv 799 800
                              ReturnValue 801
                              FunctionEnd
69(setupCam(vf3;vf3;f1;vf2;):  54(Camera) Function None 64
         65(pos):     63(ptr) FunctionParameter
      66(target):     63(ptr) FunctionParameter
         67(fov):     56(ptr) FunctionParameter
          68(uv):     49(ptr) FunctionParameter
              70:             Label
      804(mouse):     49(ptr) Variable Function
        866(cam):     55(ptr) Variable Function
        869(ray):     33(ptr) Variable Function
         875(cw):     63(ptr) Variable Function
         880(cp):     63(ptr) Variable Function
         882(cu):     63(ptr) Variable Function
         887(cv):     63(ptr) Variable Function
                              Line 1 387 0
             807:    806(ptr) AccessChain 90 805
             808:   10(fvec4) Load 807
             809:   48(fvec2) VectorShuffle 808 808 0 1
             810:    806(ptr) AccessChain 90 91
             811:   10(fvec4) Load 810
             812:   48(fvec2) VectorShuffle 811 811 0 1
             813:   48(fvec2) FDiv 809 812
                              Store 804(mouse) 813
                              Line 1 388 0
             814:   48(fvec2) Load 804(mouse)
             815:   48(fvec2) VectorTimesScalar 814 491
             816:   48(fvec2) CompositeConstruct 149 149
             817:   48(fvec2) FSub 815 816
                              Store 804(mouse) 817
                              Line 1 389 0
             818:    9(fvec3) Load 66(target)
             819:    9(fvec3) Load 65(pos)
             820:    9(fvec3) FSub 819 818
                              Store 65(pos) 820
                              Line 1 390 0
             821:     56(ptr) AccessChain 804(mouse) 193
             822:    8(float) Load 821
             823:    8(float) FMul 822 149
             824:    8(float) ExtInst 2(GLSL.std.450) 14(Cos) 823
             825:    9(fvec3) Load 65(pos)
             826:   48(fvec2) VectorShuffle 825 825 0 2
             827:   48(fvec2) VectorTimesScalar 826 824
             828:     56(ptr) AccessChain 804(mouse) 193
             829:    8(float) Load 828
             830:    8(float) FMul 829 149
             831:    8(float) ExtInst 2(GLSL.std.450) 13(Sin) 830
             832:     56(ptr) AccessChain 65(pos) 216
             833:    8(float) Load 832
             834:    8(float) FNegate 833
             835:     56(ptr) AccessChain 65(pos) 193
             836:    8(float) Load 835
             837:   48(fvec2) CompositeConstruct 834 836
             838:   48(fvec2) VectorTimesScalar 837 831
             839:   48(fvec2) FAdd 827 838
             840:    9(fvec3) Load 65(pos)
             841:    9(fvec3) VectorShuffle 840 839 3 1 4
                              Store 65(pos) 841
                              Line 1 391 0
             842:     56(ptr) AccessChain 804(mouse) 92
             843:    8(float) Load 842
             844:    8(float) FMul 843 149
             845:    8(float) ExtInst 2(GLSL.std.450) 14(Cos) 844
             846:    9(fvec3) Load 65(pos)
             847:   48(fvec2) VectorShuffle 846 846 0 1
             848:   48(fvec2) VectorTimesScalar 847 845
             849:     56(ptr) AccessChain 804(mouse) 92
             850:    8(float) Load 849
             851:    8(float) FMul 850 149
             852:    8(float) ExtInst 2(GLSL.std.450) 13(Sin) 851
             853:     56(ptr) AccessChain 65(pos) 92
             854:    8(float) Load 853
             855:    8(float) FNegate 854
             856:     56(ptr) AccessChain 65(pos) 193
             857:    8(float) Load 856
             858:   48(fvec2) CompositeConstruct 855 857
             859:   48(fvec2) VectorTimesScalar 858 852
             860:   48(fvec2) FAdd 848 859
             861:    9(fvec3) Load 65(pos)
             862:    9(fvec3) VectorShuffle 861 860 3 4 2
                              Store 65(pos) 862
                              Line 1 392 0
             863:    9(fvec3) Load 66(target)
             864:    9(fvec3) Load 65(pos)
             865:    9(fvec3) FAdd 864 863
                              Store 65(pos) 865
                              Line 1 394 0
             867:    9(fvec3) Load 65(pos)
             868:     63(ptr) AccessChain 866(cam) 91
                              Store 868 867
                              Line 1 396 0
             870:    9(fvec3) Load 65(pos)
             871:     63(ptr) AccessChain 869(ray) 91
                              Store 871 870
                              Line 1 397 0
             872:    8(float) Load 67(fov)
             873:   48(fvec2) Load 68(uv)
             874:   48(fvec2) VectorTimesScalar 873 872
                              Store 68(uv) 874
                              Line 1 399 0
             876:    9(fvec3) Load 66(target)
             877:    9(fvec3) Load 65(pos)
             878:    9(fvec3) FSub 876 877
             879:    9(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 878
                              Store 875(cw) 879
                              Line 1 401 0
                              Store 880(cp) 881
                              Line 1 403 0
             883:    9(fvec3) Load 875(cw)
             884:    9(fvec3) Load 880(cp)
             885:    9(fvec3) ExtInst 2(GLSL.std.450) 68(Cross) 883 884
             886:    9(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 885
                              Store 882(cu) 886
                              Line 1 405 0
             888:    9(fvec3) Load 882(cu)
             889:    9(fvec3) Load 875(cw)
             890:    9(fvec3) ExtInst 2(GLSL.std.450) 68(Cross) 888 889
             891:    9(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 890
                              Store 887(cv) 891
                              Line 1 406 0
             892:     56(ptr) AccessChain 68(uv) 193
             893:    8(float) Load 892
             894:    9(fvec3) Load 882(cu)
             895:    9(fvec3) VectorTimesScalar 894 893
             896:     56(ptr) AccessChain 68(uv) 92
             897:    8(float) Load 896
             898:    9(fvec3) Load 887(cv)
             899:    9(fvec3) VectorTimesScalar 898 897
             900:    9(fvec3) FAdd 895 899
             901:    9(fvec3) Load 875(cw)
             902:    9(fvec3) VectorTimesScalar 901 149
             903:    9(fvec3) FAdd 900 902
             904:    9(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 903
             905:     63(ptr) AccessChain 869(ray) 107
                              Store 905 904
                              Line 1 407 0
             906:     32(Ray) Load 869(ray)
             907:     33(ptr) AccessChain 866(cam) 107
                              Store 907 906
                              Line 1 408 0
             908:    8(float) Load 67(fov)
             909:     93(ptr) AccessChain 90 91 193
             910:    8(float) Load 909
             911:    8(float) FDiv 908 910
                              Store 494(divergence) 911
                              Line 1 409 0
             912:    8(float) Load 494(divergence)
             913:   48(fvec2) Load 68(uv)
             914:    8(float) ExtInst 2(GLSL.std.450) 66(Length) 913
             915:    8(float) FMul 914 311
             916:    8(float) FAdd 912 915
                              Store 494(divergence) 916
                              Line 1 410 0
             917:  54(Camera) Load 866(cam)
                              ReturnValue 917
                              FunctionEnd
75(mainImage(vf4;vf2;):           3 Function None 72
   73(fragColor):     71(ptr) FunctionParameter
   74(fragCoord):     49(ptr) FunctionParameter
              76:             Label
         922(uv):     49(ptr) Variable Function
          928(l):     71(ptr) Variable Function
        949(cam):     55(ptr) Variable Function
      952(param):     63(ptr) Variable Function
      953(param):     63(ptr) Variable Function
      954(param):     56(ptr) Variable Function
      955(param):     49(ptr) Variable Function
      958(lastB):     56(ptr) Variable Function
          969(c):     71(ptr) Variable Function
      978(param):     55(ptr) Variable Function
      980(param):     49(ptr) Variable Function
      981(param):     56(ptr) Variable Function
                              Line 1 416 0
             920:     93(ptr) AccessChain 90 107
             921:    8(float) Load 920
                              Store 368(t) 921
                              Line 1 418 0
             923:   48(fvec2) Load 74(fragCoord)
             924:    806(ptr) AccessChain 90 91
             925:   10(fvec4) Load 924
             926:   48(fvec2) VectorShuffle 925 925 0 1
             927:   48(fvec2) FDiv 923 926
                              Store 922(uv) 927
                              Line 1 420 0
             933:         930 Load 932(iChannel0)
             934:   48(fvec2) Load 922(uv)
             935:   10(fvec4) ImageSampleImplicitLod 933 934
                              Store 928(l) 935
                              Line 1 421 0
             936:   48(fvec2) Load 922(uv)
             937:   48(fvec2) VectorTimesScalar 936 491
             938:   48(fvec2) CompositeConstruct 363 363
             939:   48(fvec2) FSub 937 938
                              Store 922(uv) 939
                              Line 1 422 0
             940:     93(ptr) AccessChain 90 91 193
             941:    8(float) Load 940
             942:     93(ptr) AccessChain 90 91 92
             943:    8(float) Load 942
             944:    8(float) FDiv 941 943
             945:     56(ptr) AccessChain 922(uv) 92
             946:    8(float) Load 945
             947:    8(float) FDiv 946 944
             948:     56(ptr) AccessChain 922(uv) 92
                              Store 948 947
                              Line 1 424 0
                              Store 952(param) 951
                              Store 953(param) 573
                              Store 954(param) 363
             956:   48(fvec2) Load 922(uv)
                              Store 955(param) 956
             957:  54(Camera) FunctionCall 69(setupCam(vf3;vf3;f1;vf2;) 952(param) 953(param) 954(param) 955(param)
                              Store 949(cam) 957
                              Line 1 426 0
             959:     56(ptr) AccessChain 928(l) 193
             960:    8(float) Load 959
             961:     56(ptr) AccessChain 928(l) 92
             962:    8(float) Load 961
             963:     56(ptr) AccessChain 928(l) 216
             964:    8(float) Load 963
             965:    8(float) ExtInst 2(GLSL.std.450) 40(FMax) 962 964
             966:    8(float) ExtInst 2(GLSL.std.450) 40(FMax) 960 965
                              Store 958(lastB) 966
                              Line 1 427 0
             967:    8(float) Load 958(lastB)
             968:    8(float) ExtInst 2(GLSL.std.450) 26(Pow) 967 441
                              Store 958(lastB) 968
                              Line 1 429 0
             970:   48(fvec2) Load 922(uv)
             971:     93(ptr) AccessChain 90 107
             972:    8(float) Load 971
             973:   48(fvec2) CompositeConstruct 972 972
             974:   48(fvec2) FAdd 970 973
             975:    8(float) Load 958(lastB)
             976:    8(float) FSub 363 975
             977:    8(float) ExtInst 2(GLSL.std.450) 40(FMax) 589 976
             979:  54(Camera) Load 949(cam)
                              Store 978(param) 979
                              Store 980(param) 974
                              Store 981(param) 977
             982:   10(fvec4) FunctionCall 61(traceScene(struct-Camera-vf3-struct-Ray-vf3-vf311;vf2;f1;) 978(param) 980(param) 981(param)
                              Store 969(c) 982
                              Line 1 430 0
             983:   10(fvec4) Load 969(c)
             984:   10(fvec4) Load 928(l)
             985:     93(ptr) AccessChain 90 805 216
             986:    8(float) Load 985
             987:     7(bool) FOrdGreaterThan 986 149
             988:     7(bool) LogicalNot 987
                              SelectionMerge 990 None
                              BranchConditional 988 989 990
             989:               Label
             992:    991(ptr)   AccessChain 90 326
             993:     82(int)   Load 992
             994:     7(bool)   IEqual 993 91
                                Branch 990
             990:             Label
             995:     7(bool) Phi 987 76 994 989)#"_str
+
R"#(
             997:    8(float) Select 995 149 996
             998:   10(fvec4) CompositeConstruct 997 997 997 997
             999:   10(fvec4) ExtInst 2(GLSL.std.450) 46(FMix) 983 984 998
                              Store 73(fragColor) 999
                              Return
                              FunctionEnd
)#"_str );

};
};
