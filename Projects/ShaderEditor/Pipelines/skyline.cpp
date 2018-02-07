// This is generated file
// Origin file: 'C:\Projects\graphxgenengine\Projects\ShaderEditorTools\Skyline.cpp'
// Created at: 2018/02/02 - 00:31:59

#include "all_pipelines.h"

namespace Pipelines
{

void Create_skyline (PipelineTemplateDescriptor& descr)
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
			.AddUniformBuffer( "", 288_b, 0, 0, EShader::bits() | EShader::Fragment )
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
0x000200F8, 0x00000006, 0x0004003B, 0x0000001E, 0x0000001F, 0x00000007, 0x00040008, 0x00000001, 0x0000006A, 0x00000000, 0x0004003D, 0x0000000F, 
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
                              Line 1 106 0
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



struct Ray{
	vec3 origin;
	vec3 pos;
	vec3 dir;
};


//---------------------------------

const float frameToRenderHQ = float(50.0);
const float antialiasingSamples = float(16.0);
float localTime;
float seed;
float fade;
vec3 sunDir;
vec3 sunCol;
float exposure;
vec3 skyCol;
vec3 horizonCol;
float marchCount;
const vec2 zeroOne = vec2(0.0, 1.0);
const float PI = float(3.14159274);
float voxelPad;

//---------------------------------

Ray Ray_From (const vec3 leftBottom, const vec3 rightBottom, const vec3 leftTop, const vec3 rightTop, const vec3 origin, const float nearPlane, const vec2 unormCoord);
float Hash11 (in float a);
vec2 Hash22 (in vec2 uv);
float Hash21 (in vec2 uv);
float Hash2d (in vec2 uv);
float Hash3d (in vec3 uv);
float mixP (in float f0, in float f1, in float a);
float noise2d (in vec2 uv);
float noise (in vec3 uv);
vec3 saturate (in vec3 a);
float saturate (in float a);
vec3 GetSunColorSmall (in vec3 rayDir, in vec3 sunDir);
vec3 GetEnvMap (in vec3 rayDir, in vec3 sunDir);
vec2 matmin (in vec2 a, in vec2 b);
vec3 GetEnvMapSkyline (in vec3 rayDir, in vec3 sunDir, in float height);
float sdBox (in vec3 p, in vec3 radius);
float Repeat (in float a, in float len);
float cylCap (in vec3 p, in float r, in float lenRad);
float smin (in float a, in float b, in float k);
vec2 Car (in vec3 baseCenter, in float unique);
vec2 CityBlock (in vec3 p, in vec2 pint);
vec2 DistanceToObject (in vec3 p);
void CalcWindows (in vec2 block, in vec3 pos, inout vec3 texColor, inout float windowRef, inout vec3 normal);
vec3 RayTrace (const vec2 fragCoord, const Ray ray);
void mainImage (out vec4 fragColor, in vec2 fragCoord);

//---------------------------------
void main ()
{
	vec2 coord;
	coord=gl_FragCoord.xy;
	coord.y=(iResolution.xyz.y-coord.y);
	mainImage( out_Color0, coord );
}


Ray Ray_From (const vec3 leftBottom, const vec3 rightBottom, const vec3 leftTop, const vec3 rightTop, const vec3 origin, const float nearPlane, const vec2 unormCoord)
{
	const vec2 coord=unormCoord;
	const vec3 vec=mix( mix( leftBottom, rightBottom, coord.x ), mix( leftTop, rightTop, coord.x ), coord.y );
	Ray ray;
	ray.origin=origin;
	ray.dir=normalize( vec );
	ray.pos=(ray.origin+(ray.dir*nearPlane));
	return ray;
}


float Hash11 (in float a)
{
	return fract( (sin( a )*float(10403.90039063)) );
}


float Hash21 (in vec2 uv)
{
	float f;
	f=(uv.x+(uv.y*float(37.0)));
	return fract( (sin( f )*float(104003.8984375)) );
}


vec2 Hash22 (in vec2 uv)
{
	float f;
	f=(uv.x+(uv.y*float(37.0)));
	return fract( (cos( f )*vec2(10003.57910156, 37049.69921875)) );
}


float Hash2d (in vec2 uv)
{
	float f;
	f=(uv.x+(uv.y*float(37.0)));
	return fract( (sin( f )*float(104003.8984375)) );
}


float Hash3d (in vec3 uv)
{
	float f;
	f=((uv.x+(uv.y*float(37.0)))+(uv.z*float(521.0)));
	return fract( (sin( f )*float(110003.8984375)) );
}


float mixP (in float f0, in float f1, in float a)
{
	return mix( f0, f1, ((a*a)*(float(3.0)-(float(2.0)*a))) );
}


float noise2d (in vec2 uv)
{
	vec2 fr;
	fr=fract( uv.xy );
	vec2 fl;
	fl=floor( uv.xy );
	float h00;
	h00=Hash2d( fl );
	float h10;
	h10=Hash2d( (fl+vec2(1.0, 0.0)) );
	float h01;
	h01=Hash2d( (fl+zeroOne) );
	float h11;
	h11=Hash2d( (fl+vec2(1.0)) );
	return mixP( mixP( h00, h10, fr.x ), mixP( h01, h11, fr.x ), fr.y );
}


float noise (in vec3 uv)
{
	vec3 fr;
	fr=fract( uv.xyz );
	vec3 fl;
	fl=floor( uv.xyz );
	float h000;
	h000=Hash3d( fl );
	float h100;
	h100=Hash3d( (fl+vec3(1.0, 0.0, 0.0)) );
	float h010;
	h010=Hash3d( (fl+vec3(0.0, 1.0, 0.0)) );
	float h110;
	h110=Hash3d( (fl+vec3(1.0, 1.0, 0.0)) );
	float h001;
	h001=Hash3d( (fl+vec3(0.0, 0.0, 1.0)) );
	float h101;
	h101=Hash3d( (fl+vec3(1.0, 0.0, 1.0)) );
	float h011;
	h011=Hash3d( (fl+vec3(0.0, 1.0, 1.0)) );
	float h111;
	h111=Hash3d( (fl+vec3(1.0)) );
	return mixP( mixP( mixP( h000, h100, fr.x ), mixP( h010, h110, fr.x ), fr.y ), mixP( mixP( h001, h101, fr.x ), mixP( h011, h111, fr.x ), fr.y ), fr.z );
}


vec3 saturate (in vec3 a)
{
	return clamp( a, float(0.0), float(1.0) );
}


float saturate (in float a)
{
	return clamp( a, float(0.0), float(1.0) );
}


vec3 GetSunColorSmall (in vec3 rayDir, in vec3 sunDir)
{
	vec3 localRay;
	localRay=normalize( rayDir );
	float dist;
	dist=(float(1.0)-((dot( localRay, sunDir )*float(0.5))+float(0.5)));
	float sunIntensity;
	sunIntensity=(float(0.05)/dist);
	(sunIntensity+=(exp( (-( dist )*float(150.0)) )*float(7000.0)));
	sunIntensity=min( sunIntensity, float(40000.0) );
	return ((sunCol*sunIntensity)*float(0.025));
}


vec3 GetEnvMap (in vec3 rayDir, in vec3 sunDir)
{
	vec3 finalColor;
	finalColor=(mix( horizonCol, skyCol, pow( saturate( rayDir.y ), float(0.47) ) )*float(0.94999999));
	float n;
	n=noise2d( ((rayDir.xz/rayDir.y)*float(1.0)) );
	(n+=(noise2d( ((rayDir.xz/rayDir.y)*float(2.0)) )*float(0.5)));
	(n+=(noise2d( ((rayDir.xz/rayDir.y)*float(4.0)) )*float(0.25)));
	(n+=(noise2d( ((rayDir.xz/rayDir.y)*float(8.0)) )*float(0.125)));
	n=pow( abs( n ), float(3.0) );
	n=mix( (n*float(0.2)), n, saturate( abs( (rayDir.y*float(8.0)) ) ) );
	finalColor=mix( finalColor, (((vec3(1.0)+(sunCol*float(10.0)))*float(0.75))*saturate( ((rayDir.y+float(0.2))*float(5.0)) )), saturate( (n*float(0.125)) ) );
	(finalColor+=GetSunColorSmall( rayDir, sunDir ));
	return finalColor;
}


vec3 GetEnvMapSkyline (in vec3 rayDir, in vec3 sunDir, in float height)
{
	vec3 finalColor;
	finalColor=GetEnvMap( rayDir, sunDir );
	float radial;
	radial=(atan( rayDir.z, rayDir.x )*float(4.0));
	float skyline;
	skyline=floor( (((sin( (float(5.34560013)*radial) )+sin( (float(1.23399997)*radial) ))+sin( (float(2.17700005)*radial) ))*float(0.60000002)) );
	(radial*=float(4.0));
	(skyline+=(floor( (((sin( (float(5.0)*radial) )+sin( (float(1.23399997)*radial) ))+sin( (float(2.17700005)*radial) ))*float(0.60000002)) )*float(0.1)));
	float mask;
	mask=saturate( (((((rayDir.y*float(8.0))-skyline)-float(2.5))+height)*float(24.0)) );
	float vert;
	vert=((sign( sin( (radial*float(32.0)) ) )*float(0.5))+float(0.5));
	float hor;
	hor=((sign( sin( (rayDir.y*float(256.0)) ) )*float(0.5))+float(0.5));
	mask=saturate( (mask+((float(1.0)-(hor*vert))*float(0.05))) );
	finalColor=mix( (finalColor*vec3(0.1, 0.07, 0.05)), finalColor, mask );
	return finalColor;
}


vec2 matmin (in vec2 a, in vec2 b)
{
	if ( (a.x<b.x))
	{
		return a;
	}
	else
	{
		return b;
	}
	;
}


float sdBox (in vec3 p, in vec3 radius)
{
	vec3 dist;
	dist=(abs( p )-radius);
	return (min( max( dist.x, max( dist.y, dist.z ) ), float(0.0) )+length( max( dist, float(0.0) ) ));
}


float cylCap (in vec3 p, in float r, in float lenRad)
{
	float a;
	a=(length( p.xy )-r);
	a=max( a, (abs( p.z )-lenRad) );
	return a;
}


float smin (in float a, in float b, in float k)
{
	return (log2( (exp2( (k*a) )+exp2( (k*b) )) )/k);
}


float Repeat (in float a, in float len)
{
	return (mod(a, len)-(float(0.5)*len));
}


vec2 Car (in vec3 baseCenter, in float unique)
{
	float car;
	car=sdBox( (baseCenter+vec3(0.0, -0.008, 0.001)), vec3(0.01, 0.00225, 0.0275) );
	car=smin( car, sdBox( (baseCenter+vec3(0.0, -0.016, 0.008)), vec3(0.005, 0.0005, 0.01) ), float(-160.0) );
	vec3 wMirror;
	wMirror=(baseCenter+vec3(0.0, -0.005, 0.0));
	wMirror.z=(abs( wMirror.z )-float(0.02));
	float wheels;
	wheels=cylCap( wMirror.zyx, float(0.004), float(0.0135) );
	vec2 distAndMat;
	distAndMat=vec2( wheels, float(3.0) );
	distAndMat=matmin( distAndMat, vec2( car, (float(100000.0)+unique) ) );
	return distAndMat;
}


vec2 CityBlock (in vec3 p, in vec2 pint)
{
	vec4 rand;
	rand.xy=Hash22( pint );
	rand.zw=Hash22( rand.xy );
	vec2 rand2;
	rand2=Hash22( rand.zw );
	float baseRad;
	baseRad=(float(0.2)+(rand.x*float(0.1)));
	baseRad=(floor( ((baseRad*float(20.0))+float(0.5)) )/float(20.0));
	vec3 baseCenter;
	baseCenter=(p-vec3(0.5, 0.0, 0.5));
	float height;
	height=((rand.w*rand.z)+float(0.1));
	float downtown;
	downtown=saturate( (float(4.0)/length( pint.xy )) );
	(height*=downtown);
	(height*=(float(1.5)+((baseRad-float(0.15000001))*float(20.0))));
	(height+=float(0.1));
	height=(floor( (height*float(20.0)) )*float(0.05));
	float d;
	d=sdBox( baseCenter, vec3( baseRad, height, baseRad ) );
	d=min( d, p.y );
	float height2;
	height2=(max( float(0.0), ((rand.y*float(2.0))-float(1.0)) )*downtown);
	height2=(floor( (height2*float(20.0)) )*float(0.05));
	rand2=(floor( (rand2*float(20.0)) )*float(0.05));
	d=min( d, sdBox( (baseCenter-vec3( float(0.0), height, float(0.0) )), vec3( baseRad, (height2-rand2.y), (baseRad*float(0.40000001)) ) ) );
	d=min( d, sdBox( (baseCenter-vec3( float(0.0), height, float(0.0) )), vec3( (baseRad*float(0.40000001)), (height2-rand2.x), baseRad ) ) );
	if ( (rand2.y>float(0.25)))
	{
		d=min( d, sdBox( (baseCenter-vec3( float(0.0), height, float(0.0) )), vec3( (baseRad*float(0.80000001)), height2, (baseRad*float(0.80000001)) ) ) );
		float topWidth;
		topWidth=baseRad;
		if ( (height2>float(0.0)))
		{
			topWidth=(baseRad*float(0.80000001));
		}
		;
		d=max( d, -( sdBox( (baseCenter-vec3( float(0.0), (height+height2), float(0.0) )), vec3( (topWidth-float(0.0125)), float(0.015), (topWidth-float(0.0125)) ) ) ) );
	;
	}
	else
	{
		if ( (height2>float(0.0)))
		{
			d=min( d, cylCap( (baseCenter-vec3( float(0.0), height, float(0.0) )).xzy, (baseRad*float(0.80000001)), height2 ) );
		}
		;
	;
	}
	;
	d=min( d, sdBox( (baseCenter-vec3( ((rand.x-float(0.5))*baseRad), (height+height2), ((rand.y-float(0.5))*baseRad) )), vec3( ((baseRad*float(0.30000001))*rand.z), (float(0.1)*rand2.y), (((baseRad*float(0.30000001))*rand2.x)+float(0.025)) ) ) );
	vec3 boxPos;
	boxPos=(baseCenter-vec3( ((rand2.x-float(0.5))*baseRad), (height+height2), ((rand2.y-float(0.5))*baseRad) ));
	float big;
	big=sign( boxPos.x );
	boxPos.x=((abs( boxPos.x )-float(0.02))-((baseRad*float(0.30000001))*rand.w));
	d=min( d, sdBox( boxPos, vec3( ((baseRad*float(0.30000001))*rand.w), (float(0.07)*rand.y), (((baseRad*float(0.2))*rand.x)+(big*float(0.025))) ) ) );
	if ( (rand.y<float(0.04)))
	{
		d=min( d, (length( (baseCenter-vec3( float(0.0), height, float(0.0) )) )-(baseRad*float(0.80000001))) );
	;
	}
	;
	vec2 distAndMat;
	distAndMat=vec2( d, float(0.0) );
	distAndMat=matmin( distAndMat, vec2( sdBox( baseCenter, vec3(0.34999999, 0.005, 0.34999999) ), float(1.0) ) );
	return distAndMat;
}


vec2 DistanceToObject (in vec3 p)
{
	vec3 rep;
	rep=p;
	rep.xz=fract( p.xz );
	vec2 distAndMat;
	distAndMat=CityBlock( rep, floor( p.xz ) );
	vec3 p2;
	p2=p;
	rep.xyz=p2;
	float carTime;
	carTime=(localTime*float(0.2));
	float crossStreet;
	crossStreet=float(1.0);
	float repeatDist;
	repeatDist=float(0.25);
	if ( (abs( (fract( rep.x )-float(0.5)) )<float(0.34999999)))
	{
		(p2.x+=float(0.05));
		p2.xz=(p2.zx*vec2(-1.0, 1.0));
		rep.xz=p2.xz;
		crossStreet=float(0.0);
		repeatDist=float(0.1);
	;
	}
	;
	(rep.z+=floor( p2.x ));
	rep.x=Repeat( (p2.x-float(0.5)), float(1.0) );
	rep.z=(rep.z*sign( rep.x ));
	rep.x=((rep.x*sign( rep.x ))-float(0.09));
	(rep.z-=(carTime*crossStreet));
	float uniqueID;
	uniqueID=floor( (rep.z/repeatDist) );
	rep.z=Repeat( rep.z, repeatDist );
	(rep.x+=((Hash11( uniqueID )*float(0.075))-float(0.01)));
	float frontBack;
	frontBack=((Hash11( (uniqueID*float(0.98699999)) )*float(0.18000001))-float(0.09));
	(frontBack*=sin( ((localTime*float(2.0))+uniqueID) ));
	(rep.z+=(frontBack*crossStreet));
	vec2 carDist;
	carDist=Car( rep, uniqueID );
	distAndMat=matmin( distAndMat, carDist );
	return distAndMat;
}


void CalcWindows (in vec2 block, in vec3 pos, inout vec3 texColor, inout float windowRef, inout vec3 normal)
{
	vec3 hue;
	hue=vec3( (Hash21( block )*float(0.80000001)), (Hash21( (block*float(7.88999987)) )*float(0.40000001)), (Hash21( (block*float(37.88999939)) )*float(0.5)) );
	(texColor+=(hue*float(0.40000001)));
	(texColor*=float(0.75));
	float window;
	window=float(0.0);
	window=max( window, mix( float(0.2), float(1.0), floor( ((fract( ((pos.y*float(20.0))-float(0.34999999)) )*float(2.0))+float(0.1)) ) ) );
	if ( (pos.y<float(0.05)))
	{
		window=float(1.0);
	}
	;
	float winWidth;
	winWidth=(Hash21( (block*float(4.3210001)) )*float(2.0));
	if ( ((winWidth<float(1.29999995))&&(winWidth>=float(1.0))))
	{
		winWidth=float(1.29999995);
	}
	;
	window=max( window, mix( float(0.2), float(1.0), floor( (fract( ((pos.x*float(40.0))+float(0.05)) )*winWidth) ) ) );
	window=max( window, mix( float(0.2), float(1.0), floor( (fract( ((pos.z*float(40.0))+float(0.05)) )*winWidth) ) ) );
	if ( (window<float(0.5)))
	{
		(windowRef+=float(1.0));
	;
	}
	;
	(window*=Hash21( (block*float(1.12300003)) ));
	(texColor*=window);
	float wave;
	wave=(floor( ((sin( (((pos.y*float(40.0))-float(0.1))*PI) )*float(0.505))-float(0.5)) )+float(1.0));
	(normal.y-=max( float(-1.0), min( float(1.0), (-( wave )*float(0.5)) ) ));
	float pits;
	pits=(min( float(1.0), (abs( sin( ((pos.z*float(80.0))*PI) ) )*float(4.0)) )-float(1.0));
	(normal.z+=(pits*float(0.25)));
	pits=(min( float(1.0), (abs( sin( ((pos.x*float(80.0))*PI) ) )*float(4.0)) )-float(1.0));
	(normal.x+=(pits*float(0.25)));
}

)#"_str
+
R"#(
vec3 RayTrace (const vec2 fragCoord, const Ray ray)
{
	marchCount=float(0.0);
	sunCol=vec3(0.07257384, 0.0697609, 0.05625879);
	sunDir=vec3(0.54945004, 0.59080648, 0.59080648);
	horizonCol=vec3(0.89999998, 0.85500002, 0.76499999);
	skyCol=vec3(0.30000001, 0.5, 0.94999999);
	exposure=float(1.0);
	fade=float(1.0);
	vec3 camPos;
	camPos=vec3(0.0);
	vec3 camUp;
	camUp=vec3(0.0);
	vec3 camLookat;
	camLookat=vec3(0.0);
	vec2 uv;
	uv=(((fragCoord.xy/iResolution.xyz.xy)*float(2.0))-float(1.0));
	(uv/=float(2.0));
	localTime=(fract( (localTime/float(58.0)) )*float(58.0));
	if ( (localTime<float(8.0)))
	{
		float time;
		time=(localTime-float(0.0));
		float alpha;
		alpha=(time/float(8.0));
		fade=saturate( time );
		(fade*=saturate( (float(8.0)-localTime) ));
		camPos=vec3(13.0, 3.29999995, -3.5);
		(camPos.x-=(smoothstep( float(0.0), float(1.0), alpha )*float(4.80000019)));
		camUp=vec3(0.0, 1.0, 0.0);
		camLookat=vec3(0.0, 1.5, 1.5);
	;
	}
	else
	{
		if ( (localTime<float(14.0)))
		{
			float time;
			time=(localTime-float(8.0));
			float alpha;
			alpha=(time/float(6.0));
			fade=saturate( time );
			(fade*=saturate( (float(14.0)-localTime) ));
			camPos=vec3( float(26.0), (float(0.05)+(smoothstep( float(0.0), float(1.0), alpha )*float(0.40000001))), float(2.0) );
			(camPos.z-=(alpha*float(2.79999995)));
			camUp=vec3(0.0, 1.0, 0.0);
			camLookat=vec3( (camPos.x-float(0.30000001)), float(-8.14999962), float(-40.0) );
			sunDir=vec3(0.63158137, 0.39889351, 0.66482252);
			sunCol=vec3(0.07257384, 0.0697609, 0.04500703);
			(exposure*=float(0.69999999));
			(skyCol*=float(1.5));
		;
		}
		else
		{
			if ( (localTime<float(24.0)))
			{
				float time;
				time=(localTime-float(14.0));
				float alpha;
				alpha=(time/float(10.0));
				fade=saturate( time );
				(fade*=saturate( (float(24.0)-localTime) ));
				camPos=vec3(12.0, 6.30000019, -0.5);
				(camPos.y-=(alpha*float(5.5)));
				camPos.x=(cos( (alpha*float(1.0)) )*float(5.19999981));
				camPos.z=(sin( (alpha*float(1.0)) )*float(5.19999981));
				camUp=normalize( vec3( float(0.0), float(1.0), (float(-0.5)+(alpha*float(0.5))) ) );
				camLookat=vec3(0.0, 1.0, -0.5);
			;
			}
			else
			{
				if ( (localTime<float(38.0)))
				{
					float time;
					time=(localTime-float(24.0));
					float alpha;
					alpha=(time/float(14.0));
					fade=saturate( time );
					(fade*=saturate( (float(38.0)-localTime) ));
					camPos=vec3( (float(2.1500001)-(alpha*float(0.5))), float(0.02), (float(-1.0)-(alpha*float(0.2))) );
					(camPos.y+=(smoothstep( float(0.0), float(1.0), (alpha*alpha) )*float(3.4000001)));
					camUp=vec3(0.0, 1.0, 0.0);
					camLookat=vec3( float(0.0), (float(0.5)+alpha), (alpha*float(5.0)) );
				;
				}
				else
				{
					if ( (localTime<float(56.0)))
					{
						float time;
						time=(localTime-float(38.0));
						float alpha;
						alpha=(time/float(18.0));
						fade=saturate( time );
						(fade*=saturate( (float(56.0)-localTime) ));
						camPos=vec3( float(-2.0), (float(1.29999995)-(alpha*float(1.20000005))), (float(-10.5)-(alpha*float(0.5))) );
						camUp=vec3(0.0, 1.0, 0.0);
						camLookat=vec3( float(-2.0), (float(0.30000001)+alpha), float(-0.0) );
						sunDir=normalize( vec3( (float(0.5)-(alpha*float(0.60000002))), (float(0.30000001)-(alpha*float(0.30000001))), float(1.0) ) );
						sunCol=vec3(0.07257384, 0.0416315, 0.01687764);
						(localTime*=antialiasingSamples);
						(exposure*=float(0.40000001));
						horizonCol=vec3(2.0, 1.0, 0.69999999);
						skyCol=vec3(0.75, 0.5, 0.94999999);
					;
					}
					else
					{
						if ( (localTime<float(58.0)))
						{
							fade=float(0.0);
							camPos=vec3(26.0, 100.0, 2.0);
							camUp=vec3(0.0, 1.0, 0.0);
							camLookat=vec3(0.30000001, 0.15000001, 0.0);
						;
						}
					;
					}
				;
				}
			;
			}
		;
		}
	;
	}
	;
	vec3 camVec;
	camVec=normalize( (camLookat-camPos) );
	vec3 sideNorm;
	sideNorm=normalize( cross( camUp, camVec ) );
	vec3 upNorm;
	upNorm=cross( camVec, sideNorm );
	vec3 worldFacing;
	worldFacing=(camPos+camVec);
	vec3 worldPix;
	worldPix=((worldFacing+((uv.x*sideNorm)*(iResolution.xyz.x/iResolution.xyz.y)))+(uv.y*upNorm));
	vec3 rayVec;
	rayVec=ray.dir;
	vec2 distAndMat;
	distAndMat=vec2(0.0);
	float t;
	t=float(0.05);
	vec3 pos;
	pos=vec3(0.0);
	{
		int i;
		i=int(0);
		for(; (i<int(250)); ( i )++)
		{
			(marchCount+=float(1.0));
			pos=(camPos+(rayVec*t));
			distAndMat=DistanceToObject( pos );
			float walk;
			walk=distAndMat.x;
			float dx;
			dx=-( fract( pos.x ) );
			if ( (rayVec.x>float(0.0)))
			{
				dx=fract( -( pos.x ) );
			}
			;
			float dz;
			dz=-( fract( pos.z ) );
			if ( (rayVec.z>float(0.0)))
			{
				dz=fract( -( pos.z ) );
			}
			;
			float nearestVoxel;
			nearestVoxel=(min( fract( (dx/rayVec.x) ), fract( (dz/rayVec.z) ) )+voxelPad);
			nearestVoxel=max( voxelPad, nearestVoxel );
			walk=min( walk, nearestVoxel );
			(t+=walk);
			if ( ((t>float(45.0))||(abs( distAndMat.x )<float(0.000625))))
			{
				break;
			}
			;
		}
	}
	;
	float alpha;
	alpha=(-( camPos.y )/rayVec.y);
	if ( ((t>float(45.0))&&(rayVec.y<float(-0.0))))
	{
		pos.xz=(camPos.xz+(rayVec.xz*alpha));
		pos.y=float(-0.0);
		t=alpha;
		distAndMat.y=float(0.0);
		distAndMat.x=float(0.0);
	;
	}
	;
	vec3 finalColor;
	finalColor=vec3(0.0);
	if ( ((t<=float(45.0))||(t==alpha)))
	{
		float dist;
		dist=distAndMat.x;
		vec3 smallVec;
		smallVec=vec3(0.000625, 0.0, 0.0);
		vec3 normalU;
		normalU=vec3( (dist-DistanceToObject( (pos-smallVec.xyy) ).x), (dist-DistanceToObject( (pos-smallVec.yxy) ).x), (dist-DistanceToObject( (pos-smallVec.yyx) ).x) );
		vec3 normal;
		normal=normalize( normalU );
		float ambientS;
		ambientS=float(1.0);
		(ambientS*=saturate( (DistanceToObject( (pos+(normal*float(0.0125))) ).x*float(80.0)) ));
		(ambientS*=saturate( (DistanceToObject( (pos+(normal*float(0.025))) ).x*float(40.0)) ));
		(ambientS*=saturate( (DistanceToObject( (pos+(normal*float(0.05))) ).x*float(20.0)) ));
		(ambientS*=saturate( (DistanceToObject( (pos+(normal*float(0.1))) ).x*float(10.0)) ));
		(ambientS*=saturate( (DistanceToObject( (pos+(normal*float(0.2))) ).x*float(5.0)) ));
		(ambientS*=saturate( (DistanceToObject( (pos+(normal*float(0.40000001))) ).x*float(2.5)) ));
		float ambient;
		ambient=ambientS;
		ambient=max( float(0.025), pow( ambient, float(0.5) ) );
		ambient=saturate( ambient );
		vec3 ref;
		ref=reflect( rayVec, normal );
		float sunShadow;
		sunShadow=float(1.0);
		float iter;
		iter=float(0.01);
		vec3 nudgePos;
		nudgePos=(pos+(normal*float(0.002)));
		{
			int i;
			i=int(0);
			for(; (i<int(40)); ( i )++)
			{
				vec3 shadowPos;
				shadowPos=(nudgePos+(sunDir*iter));
				float tempDist;
				tempDist=DistanceToObject( shadowPos ).x;
				(sunShadow*=saturate( (tempDist*float(150.0)) ));
				if ( (tempDist<=float(0.0)))
				{
					break;
				}
				;
				float walk;
				walk=tempDist;
				float dx;
				dx=-( fract( shadowPos.x ) );
				if ( (sunDir.x>float(0.0)))
				{
					dx=fract( -( shadowPos.x ) );
				}
				;
				float dz;
				dz=-( fract( shadowPos.z ) );
				if ( (sunDir.z>float(0.0)))
				{
					dz=fract( -( shadowPos.z ) );
				}
				;
				float nearestVoxel;
				nearestVoxel=(min( fract( (dx/sunDir.x) ), fract( (dz/sunDir.z) ) )+float(0.000625));
				nearestVoxel=max( float(0.2), nearestVoxel );
				walk=min( walk, nearestVoxel );
				(iter+=max( float(0.01), walk ));
				if ( (iter>float(4.5)))
				{
					break;
				}
				;
			}
		}
		;
		sunShadow=saturate( sunShadow );
		float n;
		n=float(0.0);
		(n+=noise( (pos*float(32.0)) ));
		(n+=noise( (pos*float(64.0)) ));
		(n+=noise( (pos*float(128.0)) ));
		(n+=noise( (pos*float(256.0)) ));
		(n+=noise( (pos*float(512.0)) ));
		n=mix( float(0.69999999), float(0.94999999), n );
		vec2 block;
		block=floor( pos.xz );
		vec3 texColor;
		texColor=vec3(0.94999999, 1.0, 1.0);
		(texColor*=float(0.80000001));
		float windowRef;
		windowRef=float(0.0);
		if ( ((normal.y<float(0.1))&&(distAndMat.y==float(0.0))))
		{
			vec3 posdx;
			posdx=dFdx( pos );
			vec3 posdy;
			posdy=dFdy( pos );
			vec3 posGrad;
			posGrad=((posdx*Hash21( uv ))+(posdy*Hash21( (uv*float(7.65430021)) )));
			vec3 colTotal;
			colTotal=vec3(0.0);
			vec3 colTemp;
			colTemp=texColor;
			vec3 nTemp;
			nTemp=vec3(0.0);
			CalcWindows( block, pos, colTemp, windowRef, nTemp );
			colTotal=colTemp;
			colTemp=texColor;
			CalcWindows( block, (pos+(posdx*float(0.66600001))), colTemp, windowRef, nTemp );
			(colTotal+=colTemp);
			colTemp=texColor;
			CalcWindows( block, ((pos+(posdx*float(0.66600001)))+(posdy*float(0.66600001))), colTemp, windowRef, nTemp );
			(colTotal+=colTemp);
			colTemp=texColor;
			CalcWindows( block, (pos+(posdy*float(0.66600001))), colTemp, windowRef, nTemp );
			(colTotal+=colTemp);
			colTemp=texColor;
			CalcWindows( block, ((pos+(posdx*float(0.333)))+(posdy*float(0.333))), colTemp, windowRef, nTemp );
			(colTotal+=colTemp);
			texColor=(colTotal*float(0.2));
			(windowRef*=float(0.2));
			normal=normalize( (normal+(nTemp*float(0.2))) );
		;
		}
		else
		{
			float xroad;
			xroad=abs( (fract( (pos.x+float(0.5)) )-float(0.5)) );
			float zroad;
			zroad=abs( (fract( (pos.z+float(0.5)) )-float(0.5)) );
			float road;
			road=saturate( ((min( xroad, zroad )-float(0.14300001))*float(480.0)) );
			(texColor*=(float(1.0)-(((normal.y*float(0.94999999))*Hash21( (block*float(9.86999989)) ))*road)));
			(texColor*=mix( float(0.1), float(1.0), road ));
			float yellowLine;
			yellowLine=saturate( (float(1.0)-((min( xroad, zroad )-float(0.002))*float(480.0))) );
			(yellowLine*=saturate( ((min( xroad, zroad )-float(0.0005))*float(480.0)) ));
			(yellowLine*=saturate( ((((xroad*xroad)+(zroad*zroad))-float(0.05))*float(880.0)) ));
			texColor=mix( texColor, vec3(1.0, 0.80000001, 0.30000001), yellowLine );
			float whiteLine;
			whiteLine=saturate( (float(1.0)-((min( xroad, zroad )-float(0.06))*float(480.0))) );
			(whiteLine*=saturate( ((min( xroad, zroad )-float(0.056))*float(480.0)) ));
			(whiteLine*=saturate( ((((xroad*xroad)+(zroad*zroad))-float(0.05))*float(880.0)) ));
			(whiteLine*=saturate( (float(1.0)-((fract( (zroad*float(8.0)) )-float(0.5))*float(280.0))) ));
			(whiteLine*=saturate( (float(1.0)-((fract( (xroad*float(8.0)) )-float(0.5))*float(280.0))) ));
			texColor=mix( texColor, vec3(0.5), whiteLine );
			whiteLine=saturate( (float(1.0)-((min( xroad, zroad )-float(0.11))*float(480.0))) );
			(whiteLine*=saturate( ((min( xroad, zroad )-float(0.106))*float(480.0)) ));
			(whiteLine*=saturate( ((((xroad*xroad)+(zroad*zroad))-float(0.06))*float(880.0)) ));
			texColor=mix( texColor, vec3(0.5), whiteLine );
			float crossWalk;
			crossWalk=saturate( (float(1.0)-((fract( (xroad*float(40.0)) )-float(0.5))*float(280.0))) );
			(crossWalk*=saturate( ((zroad-float(0.15000001))*float(880.0)) ));
			(crossWalk*=(saturate( ((-( zroad )+float(0.20999999))*float(880.0)) )*(float(1.0)-road)));
			(crossWalk*=(n*n));
			texColor=mix( texColor, vec3(0.25), crossWalk );
			crossWalk=saturate( (float(1.0)-((fract( (zroad*float(40.0)) )-float(0.5))*float(280.0))) );
			(crossWalk*=saturate( ((xroad-float(0.15000001))*float(880.0)) ));
			(crossWalk*=(saturate( ((-( xroad )+float(0.20999999))*float(880.0)) )*(float(1.0)-road)));
			(crossWalk*=(n*n));
			texColor=mix( texColor, vec3(0.25), crossWalk );
			float sidewalk;
			sidewalk=float(1.0);
			vec2 blockSize;
			blockSize=vec2(100.0);
			if ( (pos.y>float(0.1)))
			{
				blockSize=vec2(10.0, 50.0);
			}
			;
			(sidewalk*=saturate( abs( ((sin( (pos.z*blockSize.x) )*float(800.0))/blockSize.x) ) ));
			(sidewalk*=saturate( abs( ((sin( (pos.x*blockSize.y) )*float(800.0))/blockSize.y) ) ));
			sidewalk=saturate( mix( float(0.69999999), float(1.0), sidewalk ) );
			sidewalk=saturate( ((float(1.0)-road)+sidewalk) );
			(texColor*=sidewalk);
		;
		}
		;
		if ( (distAndMat.y==float(3.0)))
		{
			texColor=vec3(0.05);
		;
		}
		;
		(texColor*=((vec3(1.0)*n)*float(0.05)));
		(texColor*=float(0.69999999));
		texColor=saturate( texColor );
		float windowMask;
		windowMask=float(0.0);
		if ( (distAndMat.y>=float(100.0)))
		{
			texColor=(vec3( (Hash11( distAndMat.y )*float(1.0)), Hash11( (distAndMat.y*float(8.76500034)) ), Hash11( (distAndMat.y*float(17.7310009)) ) )*float(0.1));
			texColor=pow( abs( texColor ), vec3(0.2) );
			texColor=max( vec3(0.25), texColor );
			texColor.z=min( texColor.y, texColor.z );
			(texColor*=(Hash11( (distAndMat.y*float(0.78899997)) )*float(0.15000001)));
			windowMask=saturate( (max( float(0.0), (abs( (pos.y-float(0.0175)) )*float(3800.0)) )-float(10.0)) );
			vec2 dirNorm;
			dirNorm=abs( normalize( normal.xz ) );
			float pillars;
			pillars=saturate( (float(1.0)-max( dirNorm.x, dirNorm.y )) );
			pillars=pow( max( float(0.0), (pillars-float(0.15000001)) ), float(0.125) );
			windowMask=max( windowMask, pillars );
			(texColor*=windowMask);
		;
		}
		;
		vec3 lightColor;
		lightColor=(((vec3(100.0)*sunCol)*saturate( dot( sunDir, normal ) ))*sunShadow);
		float ambientAvg;
		ambientAvg=(((ambient*float(3.0))+ambientS)*float(0.25));
		(lightColor+=(((skyCol*saturate( ((normal.y*float(0.5))+float(0.5)) ))*pow( ambientAvg, float(0.34999999) ))*float(2.5)));
		(lightColor*=float(4.0));
		finalColor=(texColor*lightColor);
		if ( (distAndMat.y>=float(100.0)))
		{
			float yfade;
			yfade=max( float(0.01), min( float(1.0), (ref.y*float(100.0)) ) );
			(yfade*=saturate( (float(1.0)-(abs( (dFdx( windowMask )*dFdy( windowMask )) )*float(250.99499512))) ));
			(finalColor+=(((GetEnvMapSkyline( ref, sunDir, (pos.y-float(1.5)) )*float(0.30000001))*yfade)*max( float(0.40000001), sunShadow )));
		;
		}
		;
		if ( (windowRef!=float(0.0)))
		{
			(finalColor*=mix( float(1.0), float(0.60000002), windowRef ));
			float yfade;
			yfade=max( float(0.01), min( float(1.0), (ref.y*float(100.0)) ) );
			(finalColor+=((((GetEnvMapSkyline( ref, sunDir, (pos.y-float(0.5)) )*float(0.60000002))*yfade)*max( float(0.60000002), sunShadow ))*windowRef));
		;
		}
		;
		(finalColor*=float(0.89999998));
		vec3 rv2;
		rv2=rayVec;
		(rv2.y*=saturate( sign( rv2.y ) ));
		vec3 fogColor;
		fogColor=GetEnvMap( rv2, sunDir );
		fogColor=min( vec3(9.0), fogColor );
		finalColor=mix( fogColor, finalColor, exp( (-( t )*float(0.02)) ) );
	;
	}
	else
	{
		finalColor=GetEnvMap( rayVec, sunDir );
	;
	}
	;
	(finalColor*=(vec3(1.0)*saturate( (float(1.0)-length( (uv/float(2.5)) )) )));
	(finalColor*=(float(1.29999995)*exposure));
	return vec3( (clamp( finalColor, float(0.0), float(1.0) )*saturate( (fade+float(0.2)) )) );
}


void mainImage (out vec4 fragColor, in vec2 fragCoord)
{
	Ray ray;
	ray=Ray_From( (iCameraFrustum[int(0)]).xyz, (iCameraFrustum[int(1)]).xyz, (iCameraFrustum[int(2)]).xyz, (iCameraFrustum[int(3)]).xyz, iCameraPos.xyz, float(10.0), (fragCoord/iResolution.xyz.xy) );
	localTime=float(0.0);
	seed=float(1.0);
	fade=float(1.0);
	exposure=float(1.0);
	marchCount=float(0.0);
	voxelPad=float(0.2);
	vec3 finalColor;
	finalColor=vec3(0.0);
	localTime=iTime;
	finalColor=RayTrace( fragCoord, ray );
	fragColor=vec4( sqrt( clamp( finalColor, float(0.0), float(1.0) ) ), float(1.0) );
}



)#"_str );
	descr.Fragment().ArraySPIRV({ 
0x07230203, 0x00010000, 0x00080002, 0x00000B78, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0007000F, 0x00000004, 0x00000005, 0x6E69616D, 0x00000000, 0x0000008B, 0x000000A1, 0x00030010, 
0x00000005, 0x00000007, 0x00030007, 0x00000001, 0x00000000, 0x002E0003, 0x00000002, 0x000001C2, 0x00000001, 0x4F202F2F, 0x646F4D70, 0x50656C75, 
0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x63206465, 
0x6E65696C, 0x75762074, 0x6E616B6C, 0x0A303031, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x72617420, 0x2D746567, 0x20766E65, 
0x6B6C7576, 0x2E316E61, 0x2F2F0A30, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x65206465, 0x7972746E, 0x696F702D, 0x6D20746E, 0x0A6E6961, 
0x6E696C23, 0x0A312065, 0x00000000, 0x00070004, 0x415F4C47, 0x675F4252, 0x735F7570, 0x65646168, 0x6E695F72, 0x00343674, 0x00090004, 0x415F4C47, 
0x735F4252, 0x72617065, 0x5F657461, 0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00040005, 0x00000005, 0x6E69616D, 0x00000000, 0x00030005, 
0x0000000A, 0x00796152, 0x00050006, 0x0000000A, 0x00000000, 0x6769726F, 0x00006E69, 0x00040006, 0x0000000A, 0x00000001, 0x00736F70, 0x00040006, 
0x0000000A, 0x00000002, 0x00726964, 0x000C0005, 0x00000013, 0x5F796152, 0x6D6F7246, 0x33667628, 0x3366763B, 0x3366763B, 0x3366763B, 0x3366763B, 
0x3B31663B, 0x3B326676, 0x00000000, 0x00050005, 0x0000000C, 0x7466656C, 0x74746F42, 0x00006D6F, 0x00050005, 0x0000000D, 0x68676972, 0x746F4274, 
0x006D6F74, 0x00040005, 0x0000000E, 0x7466656C, 0x00706F54, 0x00050005, 0x0000000F, 0x68676972, 0x706F5474, 0x00000000, 0x00040005, 0x00000010, 
0x6769726F, 0x00006E69, 0x00050005, 0x00000011, 0x7261656E, 0x6E616C50, 0x00000065, 0x00050005, 0x00000012, 0x726F6E75, 0x6F6F436D, 0x00006472, 
0x00050005, 0x00000018, 0x68736148, 0x66283131, 0x00003B31, 0x00030005, 0x00000017, 0x00000061, 0x00050005, 0x0000001D, 0x68736148, 0x76283132, 
0x003B3266, 0x00030005, 0x0000001C, 0x00007675, 0x00050005, 0x00000021, 0x68736148, 0x76283232, 0x003B3266, 0x00030005, 0x00000020, 0x00007675, 
0x00050005, 0x00000024, 0x68736148, 0x76286432, 0x003B3266, 0x00030005, 0x00000023, 0x00007675, 0x00050005, 0x00000029, 0x68736148, 0x76286433, 
0x003B3366, 0x00030005, 0x00000028, 0x00007675, 0x00060005, 0x0000002F, 0x5078696D, 0x3B316628, 0x663B3166, 0x00003B31, 0x00030005, 0x0000002C, 
0x00003066, 0x00030005, 0x0000002D, 0x00003166, 0x00030005, 0x0000002E, 0x00000061, 0x00060005, 0x00000032, 0x73696F6E, 0x28643265, 0x3B326676, 
0x00000000, 0x00030005, 0x00000031, 0x00007675, 0x00050005, 0x00000035, 0x73696F6E, 0x66762865, 0x00003B33, 0x00030005, 0x00000034, 0x00007675, 
0x00060005, 0x00000039, 0x75746173, 0x65746172, 0x33667628, 0x0000003B, 0x00030005, 0x00000038, 0x00000061, 0x00060005, 0x0000003C, 0x75746173, 
0x65746172, 0x3B316628, 0x00000000, 0x00030005, 0x0000003B, 0x00000061, 0x00090005, 0x00000041, 0x53746547, 0x6F436E75, 0x53726F6C, 0x6C6C616D, 
0x33667628, 0x3366763B, 0x0000003B, 0x00040005, 0x0000003F, 0x44796172, 0x00007269, 0x00040005, 0x00000040, 0x446E7573, 0x00007269, 0x00070005, 
0x00000045, 0x45746547, 0x614D766E, 0x66762870, 0x66763B33, 0x00003B33, 0x00040005, 0x00000043, 0x44796172, 0x00007269, 0x00040005, 0x00000044, 
0x446E7573, 0x00007269, 0x000A0005, 0x0000004B, 0x45746547, 0x614D766E, 0x796B5370, 0x656E696C, 0x33667628, 0x3366763B, 0x3B31663B, 0x00000000, 
0x00040005, 0x00000048, 0x44796172, 0x00007269, 0x00040005, 0x00000049, 0x446E7573, 0x00007269, 0x00040005, 0x0000004A, 0x67696568, 0x00007468, 
0x00060005, 0x00000050, 0x6D74616D, 0x76286E69, 0x763B3266, 0x003B3266, 0x00030005, 0x0000004E, 0x00000061, 0x00030005, 0x0000004F, 0x00000062, 
0x00060005, 0x00000055, 0x6F426473, 0x66762878, 0x66763B33, 0x00003B33, 0x00030005, 0x00000053, 0x00000070, 0x00040005, 0x00000054, 0x69646172, 
0x00007375, 0x00070005, 0x0000005B, 0x436C7963, 0x76287061, 0x663B3366, 0x31663B31, 0x0000003B, 0x00030005, 0x00000058, 0x00000070, 0x00030005, 
0x00000059, 0x00000072, 0x00040005, 0x0000005A, 0x526E656C, 0x00006461, 0x00060005, 0x00000060, 0x6E696D73, 0x3B316628, 0x663B3166, 0x00003B31, 
0x00030005, 0x0000005D, 0x00000061, 0x00030005, 0x0000005E, 0x00000062, 0x00030005, 0x0000005F, 0x0000006B, 0x00060005, 0x00000065, 0x65706552, 
0x66287461, 0x31663B31, 0x0000003B, 0x00030005, 0x00000063, 0x00000061, 0x00030005, 0x00000064, 0x006E656C, 0x00050005, 0x0000006A, 0x28726143, 
0x3B336676, 0x003B3166, 0x00050005, 0x00000068, 0x65736162, 0x746E6543, 0x00007265, 0x00040005, 0x00000069, 0x71696E75, 0x00006575, 0x00070005, 
0x0000006F, 0x79746943, 0x636F6C42, 0x6676286B, 0x66763B33, 0x00003B32, 0x00030005, 0x0000006D, 0x00000070, 0x00040005, 0x0000006E, 0x746E6970, 
0x00000000, 0x00080005, 0x00000073, 0x74736944, 0x65636E61, 0x624F6F54, 0x7463656A, 0x33667628, 0x0000003B, 0x00030005, 0x00000072, 0x00000070, 
0x000A0005, 0x0000007B, 0x636C6143, 0x646E6957, 0x2873776F, 0x3B326676, 0x3B336676, 0x3B336676, 0x763B3166, 0x003B3366, 0x00040005, 0x00000076, 
0x636F6C62, 0x0000006B, 0x00030005, 0x00000077, 0x00736F70, 0x00050005, 0x00000078, 0x43786574, 0x726F6C6F, 0x00000000, 0x00050005, 0x00000079, 
0x646E6977, 0x6552776F, 0x00000066, 0x00040005, 0x0000007A, 0x6D726F6E, 0x00006C61, 0x000C0005, 0x00000080, 0x54796152, 0x65636172, 0x32667628, 
0x7274733B, 0x2D746375, 0x2D796152, 0x2D336676, 0x2D336676, 0x31336676, 0x0000003B, 0x00050005, 0x0000007E, 0x67617266, 0x726F6F43, 0x00000064, 
0x00030005, 0x0000007F, 0x00796172, 0x00070005, 0x00000087, 0x6E69616D, 0x67616D49, 0x66762865, 0x66763B34, 0x00003B32, 0x00050005, 0x00000085, 
0x67617266, 0x6F6C6F43, 0x00000072, 0x00050005, 0x00000086, 0x67617266, 0x726F6F43, 0x00000064, 0x00040005, 0x00000089, 0x726F6F63, 0x00000064, 
0x00060005, 0x0000008B, 0x465F6C67, 0x43676172, 0x64726F6F, 0x00000000, 0x00050005, 0x00000094, 0x64616853, 0x6F747265, 0x00425579, 0x00060006, 
0x00000094, 0x00000000, 0x73655269, 0x74756C6F, 0x006E6F69, 0x00050006, 0x00000094, 0x00000001, 0x6D695469, 0x00000065, 0x00060006, 0x00000094, 
0x00000002, 0x6D695469, 0x6C654465, 0x00006174, 0x00050006, 0x00000094, 0x00000003, 0x61724669, 0x0000656D, 0x00060006, 0x00000094, 0x00000004, 
0x6461705F, 0x676E6964, 0x00000030, 0x00070006, 0x00000094, 0x00000005, 0x61684369, 0x6C656E6E, 0x656D6954, 0x00000000, 0x00080006, 0x00000094, 
0x00000006, 0x61684369, 0x6C656E6E, 0x6F736552, 0x6974756C, 0x00006E6F, 0x00050006, 0x00000094, 0x00000007, 0x756F4D69, 0x00006573, 0x00050006, 
0x00000094, 0x00000008, 0x74614469, 0x00000065, 0x00060006, 0x00000094, 0x00000009, 0x6D615369, 0x52656C70, 0x00657461, 0x00060006, 0x00000094, 
0x0000000A, 0x6461705F, 0x676E6964, 0x00000031, 0x00060006, 0x00000094, 0x0000000B, 0x6461705F, 0x676E6964, 0x00000032, 0x00060006, 0x00000094, 
0x0000000C, 0x6461705F, 0x676E6964, 0x00000033, 0x00070006, 0x00000094, 0x0000000D, 0x6D614369, 0x46617265, 0x74737572, 0x00006D75, 0x00060006, 
0x00000094, 0x0000000E, 0x6D614369, 0x50617265, 0x0000736F, 0x00030005, 0x00000096, 0x00000000, 0x00050005, 0x000000A1, 0x5F74756F, 0x6F6C6F43, 
0x00003072, 0x00040005, 0x000000A2, 0x61726170, 0x0000006D, 0x00040005, 0x000000A3, 0x61726170, 0x0000006D, 0x00040005, 0x000000A7, 0x726F6F63, 
0x00000064, 0x00030005, 0x000000A8, 0x00636576, 0x00030005, 0x000000B7, 0x00796172, 0x00030005, 0x000000CF, 0x00000066, 0x00030005, 0x000000DE, 
0x00000066, 0x00030005, 0x000000EE, 0x00000066, 0x00030005, 0x000000FB, 0x00000066, 0x00030005, 0x0000011D, 0x00007266, 0x00030005, 0x00000120, 
0x00006C66, 0x00030005, 0x00000123, 0x00303068, 0x00040005, 0x00000124, 0x61726170, 0x0000006D, 0x00030005, 0x00000127, 0x00303168, 0x00040005, 
0x0000012D, 0x61726170, 0x0000006D, 0x00030005, 0x0000012F, 0x00313068, 0x00040005, 0x00000133, 0x61726170, 0x0000006D, 0x00030005, 0x00000135, 
0x00313168, 0x00040005, 0x00000139, 0x61726170, 0x0000006D, 0x00040005, 0x0000013B, 0x61726170, 0x0000006D, 0x00040005, 0x0000013D, 0x61726170, 
0x0000006D, 0x00040005, 0x0000013F, 0x61726170, 0x0000006D, 0x00040005, 0x00000143, 0x61726170, 0x0000006D, 0x00040005, 0x00000145, 0x61726170, 
0x0000006D, 0x00040005, 0x00000147, 0x61726170, 0x0000006D, 0x00040005, 0x0000014B, 0x61726170, 0x0000006D, 0x00040005, 0x0000014C, 0x61726170, 
0x0000006D, 0x00040005, 0x0000014D, 0x61726170, 0x0000006D, 0x00030005, 0x00000153, 0x00007266, 0x00030005, 0x00000156, 0x00006C66, 0x00040005, 
0x00000159, 0x30303068, 0x00000000, 0x00040005, 0x0000015A, 0x61726170, 0x0000006D, 0x00040005, 0x0000015D, 0x30303168, 0x00000000, 0x00040005, 
0x00000161, 0x61726170, 0x0000006D, 0x00040005, 0x00000163, 0x30313068, 0x00000000, 0x00040005, 0x00000167, 0x61726170, 0x0000006D, 0x00040005, 
0x00000169, 0x30313168, 0x00000000, 0x00040005, 0x0000016D, 0x61726170, 0x0000006D, 0x00040005, 0x0000016F, 0x31303068, 0x00000000, 0x00040005, 
0x00000173, 0x61726170, 0x0000006D, 0x00040005, 0x00000175, 0x31303168, 0x00000000, 0x00040005, 0x00000179, 0x61726170, 0x0000006D, 0x00040005, 
0x0000017B, 0x31313068, 0x00000000, 0x00040005, 0x0000017F, 0x61726170, 0x0000006D, 0x00040005, 0x00000181, 0x31313168, 0x00000000, 0x00040005, 
0x00000185, 0x61726170, 0x0000006D, 0x00040005, 0x00000187, 0x61726170, 0x0000006D, 0x00040005, 0x00000189, 0x61726170, 0x0000006D, 0x00040005, 
0x0000018B, 0x61726170, 0x0000006D, 0x00040005, 0x0000018F, 0x61726170, 0x0000006D, 0x00040005, 0x00000191, 0x61726170, 0x0000006D, 0x00040005, 
0x00000193, 0x61726170, 0x0000006D, 0x00040005, 0x00000197, 0x61726170, 0x0000006D, 0x00040005, 0x00000198, 0x61726170, 0x0000006D, 0x00040005, 
0x00000199, 0x61726170, 0x0000006D, 0x00040005, 0x0000019D, 0x61726170, 0x0000006D, 0x00040005, 0x0000019F, 0x61726170, 0x0000006D, 0x00040005, 
0x000001A1, 0x61726170, 0x0000006D, 0x00040005, 0x000001A5, 0x61726170, 0x0000006D, 0x00040005, 0x000001A7, 0x61726170, 0x0000006D, 0x00040005, 
0x000001A9, 0x61726170, 0x0000006D, 0x00040005, 0x000001AD, 0x61726170, 0x0000006D, 0x00040005, 0x000001AE, 0x61726170, 0x0000006D, 0x00040005, 
0x000001AF, 0x61726170, 0x0000006D, 0x00040005, 0x000001B3, 0x61726170, 0x0000006D, 0x00040005, 0x000001B4, 0x61726170, 0x0000006D, 0x00040005, 
0x000001B5, 0x61726170, 0x0000006D, 0x00050005, 0x000001C5, 0x61636F6C, 0x7961526C, 0x00000000, 0x00040005, 0x000001C8, 0x74736964, 0x00000000, 
0x00060005, 0x000001D0, 0x496E7573, 0x6E65746E, 0x79746973, 0x00000000, 0x00040005, 0x000001E1, 0x436E7573, 0x00006C6F, 0x00050005, 0x000001E9, 
0x616E6966, 0x6C6F436C, 0x0000726F, 0x00050005, 0x000001EA, 0x69726F68, 0x436E6F7A, 0x00006C6F, 0x00040005, 0x000001EC, 0x43796B73, 0x00006C6F, 
0x00040005, 0x000001EE, 0x61726170, 0x0000006D, 0x00030005, 0x000001F8, 0x0000006E, 0x00040005, 0x00000200, 0x61726170, 0x0000006D, 0x00040005, 
0x00000209, 0x61726170, 0x0000006D, 0x00040005, 0x00000216, 0x61726170, 0x0000006D, 0x00040005, 0x00000224, 0x61726170, 0x0000006D, 0x00040005, 
0x00000235, 0x61726170, 0x0000006D, 0x00040005, 0x00000244, 0x61726170, 0x0000006D, 0x00040005, 0x00000249, 0x61726170, 0x0000006D, 0x00040005, 
0x0000024D, 0x61726170, 0x0000006D, 0x00040005, 0x0000024F, 0x61726170, 0x0000006D, 0x00050005, 0x00000257, 0x616E6966, 0x6C6F436C, 0x0000726F, 
0x00040005, 0x00000258, 0x61726170, 0x0000006D, 0x00040005, 0x0000025A, 0x61726170, 0x0000006D, 0x00040005, 0x0000025D, 0x69646172, 0x00006C61, 
0x00040005, 0x00000264, 0x6C796B73, 0x00656E69, 0x00040005, 0x00000289, 0x6B73616D, 0x00000000, 0x00040005, 0x00000295, 0x61726170, 0x0000006D, 
0x00040005, 0x00000297, 0x74726576, 0x00000000, 0x00030005, 0x0000029F, 0x00726F68, 0x00040005, 0x000002AF, 0x61726170, 0x0000006D, 0x00040005, 
0x000002CA, 0x74736964, 0x00000000, 0x00030005, 0x000002DF, 0x00000061, 0x00030005, 0x00000305, 0x00726163, 0x00040005, 0x0000030F, 0x61726170, 
0x0000006D, 0x00040005, 0x00000310, 0x61726170, 0x0000006D, 0x00040005, 0x0000031A, 0x61726170, 0x0000006D, 0x00040005, 0x0000031B, 0x61726170, 
0x0000006D, 0x00040005, 0x0000031E, 0x61726170, 0x0000006D, 0x00040005, 0x00000320, 0x61726170, 0x0000006D, 0x00040005, 0x00000321, 0x61726170, 
0x0000006D, 0x00040005, 0x00000323, 0x72694D77, 0x00726F72, 0x00040005, 0x0000032E, 0x65656877, 0x0000736C, 0x00040005, 0x00000331, 0x61726170, 
0x0000006D, 0x00040005, 0x00000334, 0x61726170, 0x0000006D, 0x00040005, 0x00000335, 0x61726170, 0x0000006D, 0x00050005, 0x00000337, 0x74736964, 
0x4D646E41, 0x00007461, 0x00040005, 0x0000033F, 0x61726170, 0x0000006D, 0x00040005, 0x00000341, 0x61726170, 0x0000006D, 0x00040005, 0x00000346, 
0x646E6172, 0x00000000, 0x00040005, 0x00000347, 0x61726170, 0x0000006D, 0x00040005, 0x0000034C, 0x61726170, 0x0000006D, 0x00040005, 0x00000352, 
0x646E6172, 0x00000032, 0x00040005, 0x00000353, 0x61726170, 0x0000006D, 0x00040005, 0x00000357, 0x65736162, 0x00646152, 0x00050005, 0x00000362, 
0x65736162, 0x746E6543, 0x00007265, 0x00040005, 0x00000366, 0x67696568, 0x00007468, 0x00050005, 0x0000036E, 0x6E776F64, 0x6E776F74, 0x00000000, 
0x00040005, 0x00000372, 0x61726170, 0x0000006D, 0x00030005, 0x00000385, 0x00000064, 0x00040005, 0x0000038A, 0x61726170, 0x0000006D, 0x00040005, 
0x0000038C, 0x61726170, 0x0000006D, 0x00040005, 0x00000392, 0x67696568, 0x00327468, 0x00040005, 0x000003B0, 0x61726170, 0x0000006D, 0x00040005, 
0x000003B1, 0x61726170, 0x0000006D, 0x00040005, 0x000003C1, 0x61726170, 0x0000006D, 0x00040005, 0x000003C2, 0x61726170, 0x0000006D, 0x00040005, 
0x000003D6, 0x61726170, 0x0000006D, 0x00040005, 0x000003D7, 0x61726170, 0x0000006D, 0x00050005, 0x000003DA, 0x57706F74, 0x68746469, 0x00000000, 
0x00040005, 0x000003F0, 0x61726170, 0x0000006D, 0x00040005, 0x000003F1, 0x61726170, 0x0000006D, 0x00040005, 0x00000401, 0x61726170, 0x0000006D, 
0x00040005, 0x00000403, 0x61726170, 0x0000006D, 0x00040005, 0x00000404, 0x61726170, 0x0000006D, 0x00040005, 0x00000429, 0x61726170, 0x0000006D, 
0x00040005, 0x0000042A, 0x61726170, 0x0000006D, 0x00040005, 0x0000042D, 0x50786F62, 0x0000736F, 0x00030005, 0x0000043E, 0x00676962, 0x00040005, 
0x0000045F, 0x61726170, 0x0000006D, 0x00040005, 0x00000461, 0x61726170, 0x0000006D, 0x00050005, 0x00000474, 0x74736964, 0x4D646E41, 0x00007461, 
0x00040005, 0x00000479, 0x61726170, 0x0000006D, 0x00040005, 0x0000047B, 0x61726170, 0x0000006D, 0x00040005, 0x0000047E, 0x61726170, 0x0000006D, 
0x00040005, 0x00000480, 0x61726170, 0x0000006D, 0x00030005, 0x00000485, 0x00706572, 0x00050005, 0x0000048C, 0x74736964, 0x4D646E41, 0x00007461, 
0x00040005, 0x00000490, 0x61726170, 0x0000006D, 0x00040005, 0x00000492, 0x61726170, 0x0000006D, 0x00030005, 0x00000494, 0x00003270, 0x00040005, 
0x00000497, 0x54726163, 0x00656D69, 0x00050005, 0x00000499, 0x61636F6C, 0x6D69546C, 0x00000065, 0x00050005, 0x0000049C, 0x736F7263, 0x72745373, 
0x00746565, 0x00050005, 0x0000049D, 0x65706572, 0x69447461, 0x00007473, 0x00040005, 0x000004BF, 0x61726170, 0x0000006D, 0x00040005, 0x000004C0, 
0x61726170, 0x0000006D, 0x00050005, 0x000004DA, 0x71696E75, 0x44496575, 0x00000000, 0x00040005, 0x000004E0, 0x61726170, 0x0000006D, 0x00040005, 
0x000004E3, 0x61726170, 0x0000006D, 0x00040005, 0x000004E7, 0x61726170, 0x0000006D, 0x00050005, 0x000004F1, 0x6E6F7266, 0x63614274, 0x0000006B, 
0x00040005, 0x000004F5, 0x61726170, 0x0000006D, 0x00040005, 0x00000508, 0x44726163, 0x00747369, 0x00040005, 0x00000509, 0x61726170, 0x0000006D, 
0x00040005, 0x0000050B, 0x61726170, 0x0000006D, 0x00040005, 0x0000050E, 0x61726170, 0x0000006D, 0x00040005, 0x00000510, 0x61726170, 0x0000006D, 
0x00030005, 0x00000516, 0x00657568, 0x00040005, 0x00000517, 0x61726170, 0x0000006D, 0x00040005, 0x0000051E, 0x61726170, 0x0000006D, 0x00040005, 
0x00000524, 0x61726170, 0x0000006D, 0x00040005, 0x0000052E, 0x646E6977, 0x0000776F, 0x00050005, 0x0000053F, 0x576E6977, 0x68746469, 0x00000000, 
0x00040005, 0x00000543, 0x61726170, 0x0000006D, 0x00040005, 0x0000056E, 0x61726170, 0x0000006D, 0x00040005, 0x00000575, 0x65766177, 0x00000000, 
0x00040005, 0x0000058B, 0x73746970, 0x00000000, 0x00050005, 0x000005AB, 0x6372616D, 0x756F4368, 0x0000746E, 0x00040005, 0x000005B0, 0x446E7573, 
0x00007269, 0x00050005, 0x000005B9, 0x6F707865, 0x65727573, 0x00000000, 0x00040005, 0x000005BA, 0x65646166, 0x00000000, 0x00040005, 0x000005BB, 
0x506D6163, 0x0000736F, 0x00040005, 0x000005BD, 0x556D6163, 0x00000070, 0x00050005, 0x000005BE, 0x4C6D6163, 0x616B6F6F, 0x00000074, 0x00030005, 
0x000005BF, 0x00007675, 0x00040005, 0x000005D4, 0x656D6974, 0x00000000, 0x00040005, 0x000005D7, 0x68706C61, 0x00000061, 0x00040005, 0x000005DA, 
0x61726170, 0x0000006D, 0x00040005, 0x000005DF, 0x61726170, 0x0000006D, 0x00040005, 0x000005F6, 0x656D6974, 0x00000000, 0x00040005, 0x000005F9, 
0x68706C61, 0x00000061, 0x00040005, 0x000005FD, 0x61726170, 0x0000006D, 0x00040005, 0x00000602, 0x61726170, 0x0000006D, 0x00040005, 0x00000629, 
0x656D6974, 0x00000000, 0x00040005, 0x0000062C, 0x68706C61, 0x00000061, 0x00040005, 0x0000062F, 0x61726170, 0x0000006D, 0x00040005, 0x00000634, 
0x61726170, 0x0000006D, 0x00040005, 0x0000065A, 0x656D6974, 0x00000000, 0x00040005, 0x0000065D, 0x68706C61, 0x00000061, 0x00040005, 0x00000660, 
0x61726170, 0x0000006D, 0x00040005, 0x00000665, 0x61726170, 0x0000006D, 0x00040005, 0x00000686, 0x656D6974, 0x00000000, 0x00040005, 0x00000689, 
0x68706C61, 0x00000061, 0x00040005, 0x0000068D, 0x61726170, 0x0000006D, 0x00040005, 0x00000692, 0x61726170, 0x0000006D, 0x00040005, 0x000006BE, 
0x566D6163, 0x00006365, 0x00050005, 0x000006C3, 0x65646973, 0x6D726F4E, 0x00000000, 0x00040005, 0x000006C8, 0x6F4E7075, 0x00006D72, 0x00050005, 
0x000006CC, 0x6C726F77, 0x63614664, 0x00676E69, 0x00050005, 0x000006D0, 0x6C726F77, 0x78695064, 0x00000000, 0x00040005, 0x000006E2, 0x56796172, 
0x00006365, 0x00050005, 0x000006E4, 0x74736964, 0x4D646E41, 0x00007461, 0x00030005, 0x000006E6, 0x00000074, 0x00030005, 0x000006E7, 0x00736F70, 
0x00030005, 0x000006E9, 0x00000069, 0x00040005, 0x000006F9, 0x61726170, 0x0000006D, 0x00040005, 0x000006FC, 0x6B6C6177, 0x00000000, 0x00030005, 
0x000006FF, 0x00007864, 0x00030005, 0x0000070D, 0x00007A64, 0x00060005, 0x0000071B, 0x7261656E, 0x56747365, 0x6C65786F, 0x00000000, 0x00050005, 
0x00000727, 0x65786F76, 0x6461506C, 0x00000000, 0x00040005, 0x00000744, 0x68706C61, 0x00000061, 0x00050005, 0x00000762, 0x616E6966, 0x6C6F436C, 
0x0000726F, 0x00040005, 0x0000076B, 0x74736964, 0x00000000, 0x00050005, 0x0000076E, 0x6C616D73, 0x6365566C, 0x00000000, 0x00040005, 0x00000770, 
0x6D726F6E, 0x00556C61, 0x00040005, 0x00000776, 0x61726170, 0x0000006D, 0x00040005, 0x0000077F, 0x61726170, 0x0000006D, 0x00040005, 0x00000788, 
0x61726170, 0x0000006D, 0x00040005, 0x0000078D, 0x6D726F6E, 0x00006C61, 0x00050005, 0x00000790, 0x69626D61, 0x53746E65, 0x00000000, 0x00040005, 
0x00000795, 0x61726170, 0x0000006D, 0x00040005, 0x00000799, 0x61726170, 0x0000006D, 0x00040005, 0x000007A1, 0x61726170, 0x0000006D, 0x00040005, 
0x000007A5, 0x61726170, 0x0000006D, 0x00040005, 0x000007AD, 0x61726170, 0x0000006D, 0x00040005, 0x000007B1, 0x61726170, 0x0000006D, 0x00040005, 
0x000007B9, 0x61726170, 0x0000006D, 0x00040005, 0x000007BD, 0x61726170, 0x0000006D, 0x00040005, 0x000007C5, 0x61726170, 0x0000006D, 0x00040005, 
0x000007C9, 0x61726170, 0x0000006D, 0x00040005, 0x000007D1, 0x61726170, 0x0000006D, 0x00040005, 0x000007D5, 0x61726170, 0x0000006D, 0x00040005, 
0x000007D9, 0x69626D61, 0x00746E65, 0x00040005, 0x000007DE, 0x61726170, 0x0000006D, 0x00030005, 0x000007E1, 0x00666572, 0x00050005, 0x000007E5, 
0x536E7573, 0x6F646168, 0x00000077, 0x00040005, 0x000007E6, 0x72657469, 0x00000000, 0x00050005, 0x000007E7, 0x6764756E, 0x736F5065, 0x00000000, 
0x00030005, 0x000007ED, 0x00000069, 0x00050005, 0x000007F6, 0x64616873, 0x6F50776F, 0x00000073, 0x00050005, 0x000007FC, 0x706D6574, 0x74736944, 
0x00000000, 0x00040005, 0x000007FD, 0x61726170, 0x0000006D, 0x00040005, 0x00000803, 0x61726170, 0x0000006D, 0x00040005, 0x0000080C, 0x6B6C6177, 
0x00000000, 0x00030005, 0x0000080E, 0x00007864, 0x00030005, 0x0000081C, 0x00007A64, 0x00060005, 0x0000082A, 0x7261656E, 0x56747365, 0x6C65786F, 
0x00000000, 0x00040005, 0x00000848, 0x61726170, 0x0000006D, 0x00030005, 0x0000084B, 0x0000006E, 0x00040005, 0x0000084E, 0x61726170, 0x0000006D, 
0x00040005, 0x00000855, 0x61726170, 0x0000006D, 0x00040005, 0x0000085C, 0x61726170, 0x0000006D, 0x00040005, 0x00000862, 0x61726170, 0x0000006D, 
0x00040005, 0x00000869, 0x61726170, 0x0000006D, 0x00040005, 0x0000086F, 0x636F6C62, 0x0000006B, 0x00050005, 0x00000873, 0x43786574, 0x726F6C6F, 
0x00000000, 0x00050005, 0x00000877, 0x646E6977, 0x6552776F, 0x00000066, 0x00040005, 0x00000883, 0x64736F70, 0x00000078, 0x00040005, 0x00000886, 
0x64736F70, 0x00000079, 0x00040005, 0x00000889, 0x47736F70, 0x00646172, 0x00040005, 0x0000088B, 0x61726170, 0x0000006D, 0x00040005, 0x00000893, 
0x61726170, 0x0000006D, 0x00050005, 0x00000897, 0x546C6F63, 0x6C61746F, 0x00000000, 0x00040005, 0x00000898, 0x546C6F63, 0x00706D65, 0x00040005, 
0x0000089A, 0x6D65546E, 0x00000070, 0x00040005, 0x0000089B, 0x61726170, 0x0000006D, 0x00040005, 0x0000089D, 0x61726170, 0x0000006D, 0x00040005, 
0x0000089F, 0x61726170, 0x0000006D, 0x00040005, 0x000008A1, 0x61726170, 0x0000006D, 0x00040005, 0x000008A3, 0x61726170, 0x0000006D, 0x00040005, 
0x000008B0, 0x61726170, 0x0000006D, 0x00040005, 0x000008B2, 0x61726170, 0x0000006D, 0x00040005, 0x000008B3, 0x61726170, 0x0000006D, 0x00040005, 
0x000008B5, 0x61726170, 0x0000006D, 0x00040005, 0x000008B7, 0x61726170, 0x0000006D, 0x00040005, 0x000008C8, 0x61726170, 0x0000006D, 0x00040005, 
0x000008CA, 0x61726170, 0x0000006D, 0x00040005, 0x000008CB, 0x61726170, 0x0000006D, 0x00040005, 0x000008CD, 0x61726170, 0x0000006D, 0x00040005, 
0x000008CF, 0x61726170, 0x0000006D, 0x00040005, 0x000008DD, 0x61726170, 0x0000006D, 0x00040005, 0x000008DF, 0x61726170, 0x0000006D, 0x00040005, 
0x000008E0, 0x61726170, 0x0000006D, 0x00040005, 0x000008E2, 0x61726170, 0x0000006D, 0x00040005, 0x000008E4, 0x61726170, 0x0000006D, 0x00040005, 
0x000008F6, 0x61726170, 0x0000006D, 0x00040005, 0x000008F8, 0x61726170, 0x0000006D, 0x00040005, 0x000008F9, 0x61726170, 0x0000006D, 0x00040005, 
0x000008FB, 0x61726170, 0x0000006D, 0x00040005, 0x000008FD, 0x61726170, 0x0000006D, 0x00040005, 0x00000910, 0x616F7278, 0x00000064, 0x00040005, 
0x00000917, 0x616F727A, 0x00000064, 0x00040005, 0x0000091E, 0x64616F72, 0x00000000, 0x00040005, 0x00000926, 0x61726170, 0x0000006D, 0x00040005, 
0x0000092E, 0x61726170, 0x0000006D, 0x00050005, 0x0000093A, 0x6C6C6579, 0x694C776F, 0x0000656E, 0x00040005, 0x00000941, 0x61726170, 0x0000006D, 
0x00040005, 0x00000948, 0x61726170, 0x0000006D, 0x00040005, 0x00000956, 0x61726170, 0x0000006D, 0x00050005, 0x0000095F, 0x74696877, 0x6E694C65, 
0x00000065, 0x00040005, 0x00000967, 0x61726170, 0x0000006D, 0x00040005, 0x0000096F, 0x61726170, 0x0000006D, 0x00040005, 0x0000097C, 0x61726170, 
0x0000006D, 0x00040005, 0x00000987, 0x61726170, 0x0000006D, 0x00040005, 0x00000991, 0x61726170, 0x0000006D, 0x00040005, 0x000009A1, 0x61726170, 
0x0000006D, 0x00040005, 0x000009A9, 0x61726170, 0x0000006D, 0x00040005, 0x000009B6, 0x61726170, 0x0000006D, 0x00050005, 0x000009BE, 0x736F7263, 
0x6C615773, 0x0000006B, 0x00040005, 0x000009C5, 0x61726170, 0x0000006D, 0x00040005, 0x000009CA, 0x61726170, 0x0000006D, 0x00040005, 0x000009D3, 
0x61726170, 0x0000006D, 0x00040005, 0x000009EA, 0x61726170, 0x0000006D, 0x00040005, 0x000009EF, 0x61726170, 0x0000006D, 0x00040005, 0x000009F7, 
0x61726170, 0x0000006D, 0x00050005, 0x00000A07, 0x65646973, 0x6B6C6177, 0x00000000, 0x00050005, 0x00000A08, 0x636F6C62, 0x7A69536B, 0x00000065, 
0x00040005, 0x00000A1D, 0x61726170, 0x0000006D, 0x00040005, 0x00000A2C, 0x61726170, 0x0000006D, 0x00040005, 0x00000A32, 0x61726170, 0x0000006D, 
0x00040005, 0x00000A38, 0x61726170, 0x0000006D, 0x00040005, 0x00000A4A, 0x61726170, 0x0000006D, 0x00050005, 0x00000A4D, 0x646E6977, 0x614D776F, 
0x00006B73, 0x00040005, 0x00000A53, 0x61726170, 0x0000006D, 0x00040005, 0x00000A5C, 0x61726170, 0x0000006D, 0x00040005, 0x00000A62, 0x61726170, 
0x0000006D, 0x00040005, 0x00000A76, 0x61726170, 0x0000006D, 0x00040005, 0x00000A84, 0x61726170, 0x0000006D, 0x00040005, 0x00000A86, 0x4E726964, 
0x006D726F, 0x00040005, 0x00000A8B, 0x6C6C6970, 0x00737261, 0x00040005, 0x00000A92, 0x61726170, 0x0000006D, 0x00050005, 0x00000A9E, 0x6867696C, 
0x6C6F4374, 0x0000726F, 0x00040005, 0x00000AA5, 0x61726170, 0x0000006D, 0x00050005, 0x00000AAA, 0x69626D61, 0x41746E65, 0x00006776, 0x00040005, 
0x00000AB5, 0x61726170, 0x0000006D, 0x00040005, 0x00000AC8, 0x64616679, 0x00000065, 0x00040005, 0x00000AD7, 0x61726170, 0x0000006D, 0x00040005, 
0x00000ADE, 0x61726170, 0x0000006D, 0x00040005, 0x00000AE0, 0x61726170, 0x0000006D, 0x00040005, 0x00000AE2, 0x61726170, 0x0000006D, 0x00040005, 
0x00000AF4, 0x64616679, 0x00000065, 0x00040005, 0x00000AFD, 0x61726170, 0x0000006D, 0x00040005, 0x00000AFF, 0x61726170, 0x0000006D, 0x00040005, 
0x00000B01, 0x61726170, 0x0000006D, 0x00030005, 0x00000B0F, 0x00327672, 0x00040005, 0x00000B14, 0x61726170, 0x0000006D, 0x00050005, 0x00000B1A, 
0x43676F66, 0x726F6C6F, 0x00000000, 0x00040005, 0x00000B1B, 0x61726170, 0x0000006D, 0x00040005, 0x00000B1D, 0x61726170, 0x0000006D, 0x00040005, 
0x00000B2D, 0x61726170, 0x0000006D, 0x00040005, 0x00000B2F, 0x61726170, 0x0000006D, 0x00040005, 0x00000B37, 0x61726170, 0x0000006D, 0x00040005, 
0x00000B46, 0x61726170, 0x0000006D, 0x00030005, 0x00000B4F, 0x00796172, 0x00040005, 0x00000B68, 0x64656573, 0x00000000, 0x00050005, 0x00000B69, 
0x616E6966, 0x6C6F436C, 0x0000726F, 0x00040047, 0x0000008B, 0x0000000B, 0x0000000F, 0x00040047, 0x00000091, 0x00000006, 0x00000010, 0x00040047, 
0x00000092, 0x00000006, 0x00000010, 0x00040047, 0x00000093, 0x00000006, 0x00000010, 0x00050048, 0x00000094, 0x00000000, 0x00000023, 0x00000000, 
0x00050048, 0x00000094, 0x00000001, 0x00000023, 0x00000010, 0x00050048, 0x00000094, 0x00000002, 0x00000023, 0x00000014, 0x00050048, 0x00000094, 
0x00000003, 0x00000023, 0x00000018, 0x00050048, 0x00000094, 0x00000004, 0x00000023, 0x0000001C, 0x00050048, 0x00000094, 0x00000005, 0x00000023, 
0x00000020, 0x00050048, 0x00000094, 0x00000006, 0x00000023, 0x00000060, 0x00050048, 0x00000094, 0x00000007, 0x00000023, 0x000000A0, 0x00050048, 
0x00000094, 0x00000008, 0x00000023, 0x000000B0, 0x00050048, 0x00000094, 0x00000009, 0x00000023, 0x000000C0, 0x00050048, 0x00000094, 0x0000000A, 
0x00000023, 0x000000C4, 0x00050048, 0x00000094, 0x0000000B, 0x00000023, 0x000000C8, 0x00050048, 0x00000094, 0x0000000C, 0x00000023, 0x000000CC, 
0x00050048, 0x00000094, 0x0000000D, 0x00000023, 0x000000D0, 0x00050048, 0x00000094, 0x0000000E, 0x00000023, 0x00000110, 0x00030047, 0x00000094, 
0x00000002, 0x00040047, 0x00000096, 0x00000022, 0x00000000, 0x00040047, 0x00000096, 0x00000021, 0x00000000, 0x00040047, 0x000000A1, 0x0000001E, 
0x00000000, 0x00020013, 0x00000003, 0x00030021, 0x00000004, 0x00000003, 0x00030016, 0x00000007, 0x00000020, 0x00040017, 0x00000008, 0x00000007, 
0x00000003, 0x00040017, 0x00000009, 0x00000007, 0x00000002, 0x0005001E, 0x0000000A, 0x00000008, 0x00000008, 0x00000008, 0x000A0021, 0x0000000B, 
0x0000000A, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000008, 0x00000007, 0x00000009, 0x00040020, 0x00000015, 0x00000007, 0x00000007, 
0x00040021, 0x00000016, 0x00000007, 0x00000015, 0x00040020, 0x0000001A, 0x00000007, 0x00000009, 0x00040021, 0x0000001B, 0x00000007, 0x0000001A, 
0x00040021, 0x0000001F, 0x00000009, 0x0000001A, 0x00040020, 0x00000026, 0x00000007, 0x00000008, 0x00040021, 0x00000027, 0x00000007, 0x00000026, 
0x00060021, 0x0000002B, 0x00000007, 0x00000015, 0x00000015, 0x00000015, 0x00040021, 0x00000037, 0x00000008, 0x00000026, 0x00050021, 0x0000003E, 
0x00000008, 0x00000026, 0x00000026, 0x00060021, 0x00000047, 0x00000008, 0x00000026, 0x00000026, 0x00000015, 0x00050021, 0x0000004D, 0x00000009, 
0x0000001A, 0x0000001A, 0x00050021, 0x00000052, 0x00000007, 0x00000026, 0x00000026, 0x00060021, 0x00000057, 0x00000007, 0x00000026, 0x00000015, 
0x00000015, 0x00050021, 0x00000062, 0x00000007, 0x00000015, 0x00000015, 0x00050021, 0x00000067, 0x00000009, 0x00000026, 0x00000015, 0x00050021, 
0x0000006C, 0x00000009, 0x00000026, 0x0000001A, 0x00040021, 0x00000071, 0x00000009, 0x00000026, 0x00080021, 0x00000075, 0x00000003, 0x0000001A, 
0x00000026, 0x00000026, 0x00000015, 0x00000026, 0x00050021, 0x0000007D, 0x00000008, 0x00000009, 0x0000000A, 0x00040017, 0x00000082, 0x00000007, 
0x00000004, 0x00040020, 0x00000083, 0x00000007, 0x00000082, 0x00050021, 0x00000084, 0x00000003, 0x00000083, 0x0000001A, 0x00040020, 0x0000008A, 
0x00000001, 0x00000082, 0x0004003B, 0x0000008A, 0x0000008B, 0x00000001, 0x00040015, 0x0000008E, 0x00000020, 0x00000001, 0x00040015, 0x0000008F, 
0x00000020, 0x00000000, 0x0004002B, 0x0000008F, 0x00000090, 0x00000004, 0x0004001C, 0x00000091, 0x00000082, 0x00000090, 0x0004001C, 0x00000092, 
0x00000082, 0x00000090, 0x0004001C, 0x00000093, 0x00000082, 0x00000090, 0x0011001E, 0x00000094, 0x00000082, 0x00000007, 0x00000007, 0x0000008E, 
0x00000007, 0x00000091, 0x00000092, 0x00000082, 0x00000082, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000093, 0x00000082, 0x00040020, 
0x00000095, 0x00000002, 0x00000094, 0x0004003B, 0x00000095, 0x00000096, 0x00000002, 0x0004002B, 0x0000008E, 0x00000097, 0x00000000, 0x0004002B, 
0x0000008F, 0x00000098, 0x00000001, 0x00040020, 0x00000099, 0x00000002, 0x00000007, 0x00040020, 0x000000A0, 0x00000003, 0x00000082, 0x0004003B, 
0x000000A0, 0x000000A1, 0x00000003, 0x0004002B, 0x0000008F, 0x000000A9, 0x00000000, 0x00040020, 0x000000B6, 0x00000007, 0x0000000A, 0x0004002B, 
0x0000008E, 0x000000B9, 0x00000002, 0x0004002B, 0x0000008E, 0x000000BD, 0x00000001, 0x0004002B, 0x00000007, 0x000000CA, 0x46228F9A, 0x0004002B, 
0x00000007, 0x000000D4, 0x42140000, 0x0004002B, 0x00000007, 0x000000D9, 0x47CB21F3, 0x0004002B, 0x00000007, 0x000000E7, 0x461C4E51, 0x0004002B, 
0x00000007, 0x000000E8, 0x4710B9B3, 0x0005002C, 0x00000009, 0x000000E9, 0x000000E7, 0x000000E8, 0x0004002B, 0x0000008F, 0x00000102, 0x00000002, 
0x0004002B, 0x00000007, 0x00000105, 0x44024000, 0x0004002B, 0x00000007, 0x0000010A, 0x47D6D9F3, 0x0004002B, 0x00000007, 0x00000114, 0x40400000, 
0x0004002B, 0x00000007, 0x00000115, 0x40000000, 0x0004002B, 0x00000007, 0x00000129, 0x3F800000, 0x0004002B, 0x00000007, 0x0000012A, 0x00000000, 
0x0005002C, 0x00000009, 0x0000012B, 0x00000129, 0x0000012A, 0x0005002C, 0x00000009, 0x00000131, 0x0000012A, 0x00000129, 0x0005002C, 0x00000009, 
0x00000137, 0x00000129, 0x00000129, 0x0006002C, 0x00000008, 0x0000015F, 0x00000129, 0x0000012A, 0x0000012A, 0x0006002C, 0x00000008, 0x00000165, 
0x0000012A, 0x00000129, 0x0000012A, 0x0006002C, 0x00000008, 0x0000016B, 0x00000129, 0x00000129, 0x0000012A, 0x0006002C, 0x00000008, 0x00000171, 
0x0000012A, 0x0000012A, 0x00000129, 0x0006002C, 0x00000008, 0x00000177, 0x00000129, 0x0000012A, 0x00000129, 0x0006002C, 0x00000008, 0x0000017D, 
0x0000012A, 0x00000129, 0x00000129, 0x0006002C, 0x00000008, 0x00000183, 0x00000129, 0x00000129, 0x00000129, 0x0004002B, 0x00000007, 0x000001CC, 
0x3F000000, 0x0004002B, 0x00000007, 0x000001D1, 0x3D4CCCCD, 0x0004002B, 0x00000007, 0x000001D6, 0x43160000, 0x0004002B, 0x00000007, 0x000001D9, 
0x45DAC000, 0x0004002B, 0x00000007, 0x000001DE, 0x471C4000, 0x00040020, 0x000001E0, 0x00000006, 0x00000008, 0x0004003B, 0x000001E0, 0x000001E1, 
0x00000006, 0x0004002B, 0x00000007, 0x000001E5, 0x3CCCCCCD, 0x0004003B, 0x000001E0, 0x000001EA, 0x00000006, 0x0004003B, 0x000001E0, 0x000001EC, 
0x00000006, 0x0004002B, 0x00000007, 0x000001F2, 0x3EF0A3D7, 0x0004002B, 0x00000007, 0x000001F6, 0x3F733333, 0x0004002B, 0x00000007, 0x00000214, 
0x40800000, 0x0004002B, 0x00000007, 0x00000218, 0x3E800000, 0x0004002B, 0x00000007, 0x00000222, 0x41000000, 0x0004002B, 0x00000007, 0x00000226, 
0x3E000000, 0x0004002B, 0x00000007, 0x0000022E, 0x3E4CCCCD, 0x0004002B, 0x00000007, 0x0000023A, 0x41200000, 0x0004002B, 0x00000007, 0x0000023D, 
0x3F400000, 0x0004002B, 0x00000007, 0x00000242, 0x40A00000, 0x0004002B, 0x00000007, 0x00000265, 0x40AB0F28, 0x0004002B, 0x00000007, 0x00000269, 
0x3F9DF3B6, 0x0004002B, 0x00000007, 0x0000026E, 0x400B53F8, 0x0004002B, 0x00000007, 0x00000273, 0x3F19999A, 0x0004002B, 0x00000007, 0x00000285, 
0x3DCCCCCD, 0x0004002B, 0x00000007, 0x0000028F, 0x40200000, 0x0004002B, 0x00000007, 0x00000293, 0x41C00000, 0x0004002B, 0x00000007, 0x00000299, 
0x42000000, 0x0004002B, 0x00000007, 0x000002A2, 0x43800000, 0x0004002B, 0x00000007, 0x000002B2, 0x3D8F5C29, 0x0006002C, 0x00000008, 0x000002B3, 
0x00000285, 0x000002B2, 0x000001D1, 0x00020014, 0x000002C0, 0x0004002B, 0x00000007, 0x00000307, 0xBC03126F, 0x0004002B, 0x00000007, 0x00000308, 
0x3A83126F, 0x0006002C, 0x00000008, 0x00000309, 0x0000012A, 0x00000307, 0x00000308, 0x0004002B, 0x00000007, 0x0000030B, 0x3C23D70A, 0x0004002B, 
0x00000007, 0x0000030C, 0x3B1374BC, 0x0004002B, 0x00000007, 0x0000030D, 0x3CE147AE, 0x0006002C, 0x00000008, 0x0000030E, 0x0000030B, 0x0000030C, 
0x0000030D, 0x0004002B, 0x00000007, 0x00000313, 0xBC83126F, 0x0004002B, 0x00000007, 0x00000314, 0x3C03126F, 0x0006002C, 0x00000008, 0x00000315, 
0x0000012A, 0x00000313, 0x00000314, 0x0004002B, 0x00000007, 0x00000317, 0x3BA3D70A, 0x0004002B, 0x00000007, 0x00000318, 0x3A03126F, 0x0006002C, 
0x00000008, 0x00000319, 0x00000317, 0x00000318, 0x0000030B, 0x0004002B, 0x00000007, 0x0000031D, 0xC3200000, 0x0004002B, 0x00000007, 0x00000325, 
0xBBA3D70A, 0x0006002C, 0x00000008, 0x00000326, 0x0000012A, 0x00000325, 0x0000012A, 0x0004002B, 0x00000007, 0x0000032B, 0x3CA3D70A, 0x0004002B, 
0x00000007, 0x0000032F, 0x3B83126F, 0x0004002B, 0x00000007, 0x00000330, 0x3C5D2F1B, 0x0004002B, 0x00000007, 0x0000033B, 0x47C35000, 0x0004002B, 
0x00000007, 0x0000035D, 0x41A00000, 0x0006002C, 0x00000008, 0x00000364, 0x000001CC, 0x0000012A, 0x000001CC, 0x0004002B, 0x0000008F, 0x00000367, 
0x00000003, 0x0004002B, 0x00000007, 0x00000377, 0x3FC00000, 0x0004002B, 0x00000007, 0x00000379, 0x3E19999A, 0x0004002B, 0x00000007, 0x000003AD, 
0x3ECCCCCD, 0x0004002B, 0x00000007, 0x000003D0, 0x3F4CCCCD, 0x0004002B, 0x00000007, 0x000003EA, 0x3C4CCCCD, 0x0004002B, 0x00000007, 0x000003EC, 
0x3C75C28F, 0x0004002B, 0x00000007, 0x0000041A, 0x3E99999A, 0x0004002B, 0x00000007, 0x00000466, 0x3D23D70A, 0x0004002B, 0x00000007, 0x00000477, 
0x3EB33333, 0x0006002C, 0x00000008, 0x00000478, 0x00000477, 0x00000317, 0x00000477, 0x00040020, 0x00000498, 0x00000006, 0x00000007, 0x0004003B, 
0x00000498, 0x00000499, 0x00000006, 0x0004002B, 0x00000007, 0x000004AC, 0xBF800000, 0x0005002C, 0x00000009, 0x000004AD, 0x000004AC, 0x00000129, 
0x0004002B, 0x00000007, 0x000004D0, 0x3DB851EC, 0x0004002B, 0x00000007, 0x000004EA, 0x3D99999A, 0x0004002B, 0x00000007, 0x000004F3, 0x3F7CAC08, 
0x0004002B, 0x00000007, 0x000004F7, 0x3E3851EC, 0x0004002B, 0x00000007, 0x0000051C, 0x40FC7AE1, 0x0004002B, 0x00000007, 0x00000522, 0x42178F5C, 
0x0004002B, 0x00000007, 0x00000541, 0x408A45A2, 0x0004002B, 0x00000007, 0x00000547, 0x3FA66666, 0x0004002B, 0x00000007, 0x00000551, 0x42200000, 
0x0004002B, 0x00000007, 0x0000056C, 0x3F8FBE77, 0x0004002B, 0x00000007, 0x0000057A, 0x40490FDB, 0x0004002B, 0x00000007, 0x0000057D, 0x3F0147AE, 
0x0004002B, 0x00000007, 0x0000058E, 0x42A00000, 0x0004003B, 0x00000498, 0x000005AB, 0x00000006, 0x0004002B, 0x00000007, 0x000005AC, 0x3D94A198, 
0x0004002B, 0x00000007, 0x000005AD, 0x3D8EDECE, 0x0004002B, 0x00000007, 0x000005AE, 0x3D666F9E, 0x0006002C, 0x00000008, 0x000005AF, 0x000005AC, 
0x000005AD, 0x000005AE, 0x0004003B, 0x000001E0, 0x000005B0, 0x00000006, 0x0004002B, 0x00000007, 0x000005B1, 0x3F0CA8C2, 0x0004002B, 0x00000007, 
0x000005B2, 0x3F173F18, 0x0006002C, 0x00000008, 0x000005B3, 0x000005B1, 0x000005B2, 0x000005B2, 0x0004002B, 0x00000007, 0x000005B4, 0x3F666666, 
0x0004002B, 0x00000007, 0x000005B5, 0x3F5AE148, 0x0004002B, 0x00000007, 0x000005B6, 0x3F43D70A, 0x0006002C, 0x00000008, 0x000005B7, 0x000005B4, 
0x000005B5, 0x000005B6, 0x0006002C, 0x00000008, 0x000005B8, 0x0000041A, 0x000001CC, 0x000001F6, 0x0004003B, 0x00000498, 0x000005B9, 0x00000006, 
0x0004003B, 0x00000498, 0x000005BA, 0x00000006, 0x0006002C, 0x00000008, 0x000005BC, 0x0000012A, 0x0000012A, 0x0000012A, 0x00040020, 0x000005C0, 
0x00000002, 0x00000082, 0x0004002B, 0x00000007, 0x000005CC, 0x42680000, 0x0004002B, 0x00000007, 0x000005E3, 0x41500000, 0x0004002B, 0x00000007, 
0x000005E4, 0x40533333, 0x0004002B, 0x00000007, 0x000005E5, 0xC0600000, 0x0006002C, 0x00000008, 0x000005E6, 0x000005E3, 0x000005E4, 0x000005E5, 
0x0004002B, 0x00000007, 0x000005E9, 0x4099999A, 0x0006002C, 0x00000008, 0x000005EF, 0x0000012A, 0x00000377, 0x00000377, 0x0004002B, 0x00000007, 
0x000005F2, 0x41600000, 0x0004002B, 0x00000007, 0x000005FB, 0x40C00000, 0x0004002B, 0x00000007, 0x00000606, 0x41D00000, 0x0004002B, 0x00000007, 
0x0000060D, 0x40333333, 0x0004002B, 0x00000007, 0x00000616, 0xC1026666, 0x0004002B, 0x00000007, 0x00000617, 0xC2200000, 0x0004002B, 0x00000007, 
0x00000619, 0x3F21AF51, 0x0004002B, 0x00000007, 0x0000061A, 0x3ECC3BC5, 0x0004002B, 0x00000007, 0x0000061B, 0x3F2A31CF, 0x0006002C, 0x00000008, 
0x0000061C, 0x00000619, 0x0000061A, 0x0000061B, 0x0004002B, 0x00000007, 0x0000061D, 0x3D38594B, 0x0006002C, 0x00000008, 0x0000061E, 0x000005AC, 
0x000005AD, 0x0000061D, 0x0004002B, 0x00000007, 0x0000061F, 0x3F333333, 0x0004002B, 0x00000007, 0x00000638, 0x41400000, 0x0004002B, 0x00000007, 
0x00000639, 0x40C9999A, 0x0004002B, 0x00000007, 0x0000063A, 0xBF000000, 0x0006002C, 0x00000008, 0x0000063B, 0x00000638, 0x00000639, 0x0000063A, 
0x0004002B, 0x00000007, 0x0000063D, 0x40B00000, 0x0004002B, 0x00000007, 0x00000646, 0x40A66666, 0x0006002C, 0x00000008, 0x00000653, 0x0000012A, 
0x00000129, 0x0000063A, 0x0004002B, 0x00000007, 0x00000656, 0x42180000, 0x0004002B, 0x00000007, 0x00000669, 0x4009999A, 0x0004002B, 0x00000007, 
0x00000675, 0x4059999A, 0x0004002B, 0x00000007, 0x00000682, 0x42600000, 0x0004002B, 0x00000007, 0x0000068B, 0x41900000, 0x0004002B, 0x00000007, 
0x00000696, 0xC0000000, 0x0004002B, 0x00000007, 0x00000698, 0x3F99999A, 0x0004002B, 0x00000007, 0x0000069B, 0xC1280000, 0x0004002B, 0x00000007, 
0x000006A2, 0x80000000, 0x0004002B, 0x00000007, 0x000006AC, 0x3D2A85CB, 0x0004002B, 0x00000007, 0x000006AD, 0x3C8A42FA, 0x0006002C, 0x00000008, 
0x000006AE, 0x000005AC, 0x000006AC, 0x000006AD, 0x0004002B, 0x00000007, 0x000006AF, 0x41800000, 0x0006002C, 0x00000008, 0x000006B4, 0x00000115, 
0x00000129, 0x0000061F, 0x0006002C, 0x00000008, 0x000006B5, 0x0000023D, 0x000001CC, 0x000001F6, 0x0004002B, 0x00000007, 0x000006BB, 0x42C80000, 
0x0006002C, 0x00000008, 0x000006BC, 0x00000606, 0x000006BB, 0x00000115, 0x0006002C, 0x00000008, 0x000006BD, 0x0000041A, 0x00000379, 0x0000012A, 
0x0005002C, 0x00000009, 0x000006E5, 0x0000012A, 0x0000012A, 0x00040020, 0x000006E8, 0x00000007, 0x0000008E, 0x0004002B, 0x0000008E, 0x000006F0, 
0x000000FA, 0x0004003B, 0x00000498, 0x00000727, 0x00000006, 0x0004002B, 0x00000007, 0x00000734, 0x42340000, 0x0004002B, 0x00000007, 0x0000073C, 
0x3A23D70A, 0x0006002C, 0x00000008, 0x0000076F, 0x0000073C, 0x0000012A, 0x0000012A, 0x0004002B, 0x00000007, 0x000007EA, 0x3B03126F, 0x0004002B, 
0x0000008E, 0x000007F4, 0x00000028, 0x0004002B, 0x00000007, 0x00000841, 0x40900000, 0x0004002B, 0x00000007, 0x00000853, 0x42800000, 0x0004002B, 
0x00000007, 0x0000085A, 0x43000000, 0x0004002B, 0x00000007, 0x00000867, 0x44000000, 0x0006002C, 0x00000008, 0x00000874, 0x000001F6, 0x00000129, 
0x00000129, 0x0004002B, 0x00000007, 0x00000891, 0x40F4F007, 0x0004002B, 0x00000007, 0x000008AD, 0x3F2A7EFA, 0x0004002B, 0x00000007, 0x000008F0, 
0x3EAA7EFA, 0x0004002B, 0x00000007, 0x00000922, 0x3E126E98, 0x0004002B, 0x00000007, 0x00000924, 0x43F00000, 0x0004002B, 0x00000007, 0x0000092C, 
0x411DEB85, 0x0004002B, 0x00000007, 0x00000954, 0x445C0000, 0x0006002C, 0x00000008, 0x0000095B, 0x00000129, 0x000003D0, 0x0000041A, 0x0004002B, 
0x00000007, 0x00000963, 0x3D75C28F, 0x0004002B, 0x00000007, 0x0000096C, 0x3D656042, 0x0004002B, 0x00000007, 0x00000984, 0x438C0000, 0x0006002C, 
0x00000008, 0x00000996, 0x000001CC, 0x000001CC, 0x000001CC, 0x0004002B, 0x00000007, 0x0000099D, 0x3DE147AE, 0x0004002B, 0x00000007, 0x000009A6, 
0x3DD91687, 0x0004002B, 0x00000007, 0x000009D0, 0x3E570A3D, 0x0006002C, 0x00000008, 0x000009E0, 0x00000218, 0x00000218, 0x00000218, 0x0005002C, 
0x00000009, 0x00000A09, 0x000006BB, 0x000006BB, 0x0004002B, 0x00000007, 0x00000A0F, 0x42480000, 0x0005002C, 0x00000009, 0x00000A10, 0x0000023A, 
0x00000A0F, 0x0004002B, 0x00000007, 0x00000A17, 0x44480000, 0x0006002C, 0x00000008, 0x00000A42, 0x000001D1, 0x000001D1, 0x000001D1, 0x0004002B, 
0x00000007, 0x00000A5A, 0x410C3D71, 0x0004002B, 0x00000007, 0x00000A60, 0x418DD917, 0x0006002C, 0x00000008, 0x00000A68, 0x0000022E, 0x0000022E, 
0x0000022E, 0x0004002B, 0x00000007, 0x00000A74, 0x3F49FBE7, 0x0004002B, 0x00000007, 0x00000A7D, 0x3C8F5C29, 0x0004002B, 0x00000007, 0x00000A80, 
0x456D8000, 0x0006002C, 0x00000008, 0x00000A9F, 0x000006BB, 0x000006BB, 0x000006BB, 0x0004002B, 0x00000007, 0x00000AD4, 0x437AFEB8, 0x0004002B, 
0x00000007, 0x00000B20, 0x41100000, 0x0006002C, 0x00000008, 0x00000B21, 0x00000B20, 0x00000B20, 0x00000B20, 0x0004002B, 0x0000008E, 0x00000B50, 
0x0000000D, 0x0004002B, 0x0000008E, 0x00000B5A, 0x00000003, 0x0004002B, 0x0000008E, 0x00000B5E, 0x0000000E, 0x0004003B, 0x00000498, 0x00000B68, 
0x00000006, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 0x000200F8, 0x00000006, 0x0004003B, 0x0000001A, 0x00000089, 0x00000007, 
0x0004003B, 0x00000083, 0x000000A2, 0x00000007, 0x0004003B, 0x0000001A, 0x000000A3, 0x00000007, 0x00040008, 0x00000001, 0x000000A0, 0x00000000, 
0x0004003D, 0x00000082, 0x0000008C, 0x0000008B, 0x0007004F, 0x00000009, 0x0000008D, 0x0000008C, 0x0000008C, 0x00000000, 0x00000001, 0x0003003E, 
0x00000089, 0x0000008D, 0x00040008, 0x00000001, 0x000000A1, 0x00000000, 0x00060041, 0x00000099, 0x0000009A, 0x00000096, 0x00000097, 0x00000098, 
0x0004003D, 0x00000007, 0x0000009B, 0x0000009A, 0x00050041, 0x00000015, 0x0000009C, 0x00000089, 0x00000098, 0x0004003D, 0x00000007, 0x0000009D, 
0x0000009C, 0x00050083, 0x00000007, 0x0000009E, 0x0000009B, 0x0000009D, 0x00050041, 0x00000015, 0x0000009F, 0x00000089, 0x00000098, 0x0003003E, 
0x0000009F, 0x0000009E, 0x00040008, 0x00000001, 0x000000A2, 0x00000000, 0x0004003D, 0x00000009, 0x000000A4, 0x00000089, 0x0003003E, 0x000000A3, 
0x000000A4, 0x00060039, 0x00000003, 0x000000A5, 0x00000087, 0x000000A2, 0x000000A3, 0x0004003D, 0x00000082, 0x000000A6, 0x000000A2, 0x0003003E, 
0x000000A1, 0x000000A6, 0x000100FD, 0x00010038, 0x00050036, 0x0000000A, 0x00000013, 0x00000000, 0x0000000B, 0x00030037, 0x00000008, 0x0000000C, 
0x00030037, 0x00000008, 0x0000000D, 0x00030037, 0x00000008, 0x0000000E, 0x00030037, 0x00000008, 0x0000000F, 0x00030037, 0x00000008, 0x00000010, 
0x00030037, 0x00000007, 0x00000011, 0x00030037, 0x00000009, 0x00000012, 0x000200F8, 0x00000014, 0x0004003B, 0x0000001A, 0x000000A7, 0x00000007, 
0x0004003B, 0x00000026, 0x000000A8, 0x00000007, 0x0004003B, 0x000000B6, 0x000000B7, 0x00000007, 0x00040008, 0x00000001, 0x000000A8, 0x00000000, 
0x0003003E, 0x000000A7, 0x00000012, 0x00040008, 0x00000001, 0x000000A9, 0x00000000, 0x00050041, 0x00000015, 0x000000AA, 0x000000A7, 0x000000A9, 
0x0004003D, 0x00000007, 0x000000AB, 0x000000AA, 0x00060050, 0x00000008, 0x000000AC, 0x000000AB, 0x000000AB, 0x000000AB, 0x0008000C, 0x00000008, 
0x000000AD, 0x00000002, 0x0000002E, 0x0000000C, 0x0000000D, 0x000000AC, 0x00050041, 0x00000015, 0x000000AE, 0x000000A7, 0x000000A9, 0x0004003D, 
0x00000007, 0x000000AF, 0x000000AE, 0x00060050, 0x00000008, 0x000000B0, 0x000000AF, 0x000000AF, 0x000000AF, 0x0008000C, 0x00000008, 0x000000B1, 
0x00000002, 0x0000002E, 0x0000000E, 0x0000000F, 0x000000B0, 0x00050041, 0x00000015, 0x000000B2, 0x000000A7, 0x00000098, 0x0004003D, 0x00000007, 
0x000000B3, 0x000000B2, 0x00060050, 0x00000008, 0x000000B4, 0x000000B3, 0x000000B3, 0x000000B3, 0x0008000C, 0x00000008, 0x000000B5, 0x00000002, 
0x0000002E, 0x000000AD, 0x000000B1, 0x000000B4, 0x0003003E, 0x000000A8, 0x000000B5, 0x00040008, 0x00000001, 0x000000AB, 0x00000000, 0x00050041, 
0x00000026, 0x000000B8, 0x000000B7, 0x00000097, 0x0003003E, 0x000000B8, 0x00000010, 0x00040008, 0x00000001, 0x000000AC, 0x00000000, 0x0004003D, 
0x00000008, 0x000000BA, 0x000000A8, 0x0006000C, 0x00000008, 0x000000BB, 0x00000002, 0x00000045, 0x000000BA, 0x00050041, 0x00000026, 0x000000BC, 
0x000000B7, 0x000000B9, 0x0003003E, 0x000000BC, 0x000000BB, 0x00040008, 0x00000001, 0x000000AD, 0x00000000, 0x00050041, 0x00000026, 0x000000BE, 
0x000000B7, 0x00000097, 0x0004003D, 0x00000008, 0x000000BF, 0x000000BE, 0x00050041, 0x00000026, 0x000000C0, 0x000000B7, 0x000000B9, 0x0004003D, 
0x00000008, 0x000000C1, 0x000000C0, 0x0005008E, 0x00000008, 0x000000C2, 0x000000C1, 0x00000011, 0x00050081, 0x00000008, 0x000000C3, 0x000000BF, 
0x000000C2, 0x00050041, 0x00000026, 0x000000C4, 0x000000B7, 0x000000BD, 0x0003003E, 0x000000C4, 0x000000C3, 0x00040008, 0x00000001, 0x000000AE, 
0x00000000, 0x0004003D, 0x0000000A, 0x000000C5, 0x000000B7, 0x000200FE, 0x000000C5, 0x00010038, 0x00050036, 0x00000007, 0x00000018, 0x00000000, 
0x00000016, 0x00030037, 0x00000015, 0x00000017, 0x000200F8, 0x00000019, 0x00040008, 0x00000001, 0x000000B4, 0x00000000, 0x0004003D, 0x00000007, 
0x000000C8, 0x00000017, 0x0006000C, 0x00000007, 0x000000C9, 0x00000002, 0x0000000D, 0x000000C8, 0x00050085, 0x00000007, 0x000000CB, 0x000000C9, 
0x000000CA, 0x0006000C, 0x00000007, 0x000000CC, 0x00000002, 0x0000000A, 0x000000CB, 0x000200FE, 0x000000CC, 0x00010038, 0x00050036, 0x00000007, 
0x0000001D, 0x00000000, 0x0000001B, 0x00030037, 0x0000001A, 0x0000001C, 0x000200F8, 0x0000001E, 0x0004003B, 0x00000015, 0x000000CF, 0x00000007, 
0x00040008, 0x00000001, 0x000000BB, 0x00000000, 0x00050041, 0x00000015, 0x000000D0, 0x0000001C, 0x000000A9, 0x0004003D, 0x00000007, 0x000000D1, 
0x000000D0, 0x00050041, 0x00000015, 0x000000D2, 0x0000001C, 0x00000098, 0x0004003D, 0x00000007, 0x000000D3, 0x000000D2, 0x00050085, 0x00000007, 
0x000000D5, 0x000000D3, 0x000000D4, 0x00050081, 0x00000007, 0x000000D6, 0x000000D1, 0x000000D5, 0x0003003E, 0x000000CF, 0x000000D6, 0x00040008, 
0x00000001, 0x000000BC, 0x00000000, 0x0004003D, 0x00000007, 0x000000D7, 0x000000CF, 0x0006000C, 0x00000007, 0x000000D8, 0x00000002, 0x0000000D, 
0x000000D7, 0x00050085, 0x00000007, 0x000000DA, 0x000000D8, 0x000000D9, 0x0006000C, 0x00000007, 0x000000DB, 0x00000002, 0x0000000A, 0x000000DA, 
0x000200FE, 0x000000DB, 0x00010038, 0x00050036, 0x00000009, 0x00000021, 0x00000000, 0x0000001F, 0x00030037, 0x0000001A, 0x00000020, 0x000200F8, 
0x00000022, 0x0004003B, 0x00000015, 0x000000DE, 0x00000007, 0x00040008, 0x00000001, 0x000000C3, 0x00000000, 0x00050041, 0x00000015, 0x000000DF, 
0x00000020, 0x000000A9, 0x0004003D, 0x00000007, 0x000000E0, 0x000000DF, 0x00050041, 0x00000015, 0x000000E1, 0x00000020, 0x00000098, 0x0004003D, 
0x00000007, 0x000000E2, 0x000000E1, 0x00050085, 0x00000007, 0x000000E3, 0x000000E2, 0x000000D4, 0x00050081, 0x00000007, 0x000000E4, 0x000000E0, 
0x000000E3, 0x0003003E, 0x000000DE, 0x000000E4, 0x00040008, 0x00000001, 0x000000C4, 0x00000000, 0x0004003D, 0x00000007, 0x000000E5, 0x000000DE, 
0x0006000C, 0x00000007, 0x000000E6, 0x00000002, 0x0000000E, 0x000000E5, 0x0005008E, 0x00000009, 0x000000EA, 0x000000E9, 0x000000E6, 0x0006000C, 
0x00000009, 0x000000EB, 0x00000002, 0x0000000A, 0x000000EA, 0x000200FE, 0x000000EB, 0x00010038, 0x00050036, 0x00000007, 0x00000024, 0x00000000, 
0x0000001B, 0x00030037, 0x0000001A, 0x00000023, 0x000200F8, 0x00000025, 0x0004003B, 0x00000015, 0x000000EE, 0x00000007, 0x00040008, 0x00000001, 
0x000000CB, 0x00000000, 0x00050041, 0x00000015, 0x000000EF, 0x00000023, 0x000000A9, 0x0004003D, 0x00000007, 0x000000F0, 0x000000EF, 0x00050041, 
0x00000015, 0x000000F1, 0x00000023, 0x00000098, 0x0004003D, 0x00000007, 0x000000F2, 0x000000F1, 0x00050085, 0x00000007, 0x000000F3, 0x000000F2, 
0x000000D4, 0x00050081, 0x00000007, 0x000000F4, 0x000000F0, 0x000000F3, 0x0003003E, 0x000000EE, 0x000000F4, 0x00040008, 0x00000001, 0x000000CC, 
0x00000000, 0x0004003D, 0x00000007, 0x000000F5, 0x000000EE, 0x0006000C, 0x00000007, 0x000000F6, 0x00000002, 0x0000000D, 0x000000F5, 0x00050085, 
0x00000007, 0x000000F7, 0x000000F6, 0x000000D9, 0x0006000C, 0x00000007, 0x000000F8, 0x00000002, 0x0000000A, 0x000000F7, 0x000200FE, 0x000000F8, 
0x00010038, 0x00050036, 0x00000007, 0x00000029, 0x00000000, 0x00000027, 0x00030037, 0x00000026, 0x00000028, 0x000200F8, 0x0000002A, 0x0004003B, 
0x00000015, 0x000000FB, 0x00000007, 0x00040008, 0x00000001, 0x000000D3, 0x00000000, 0x00050041, 0x00000015, 0x000000FC, 0x00000028, 0x000000A9, 
0x0004003D, 0x00000007, 0x000000FD, 0x000000FC, 0x00050041, 0x00000015, 0x000000FE, 0x00000028, 0x00000098, 0x0004003D, 0x00000007, 0x000000FF, 
0x000000FE, 0x00050085, 0x00000007, 0x00000100, 0x000000FF, 0x000000D4, 0x00050081, 0x00000007, 0x00000101, 0x000000FD, 0x00000100, 0x00050041, 
0x00000015, 0x00000103, 0x00000028, 0x00000102, 0x0004003D, 0x00000007, 0x00000104, 0x00000103, 0x00050085, 0x00000007, 0x00000106, 0x00000104, 
0x00000105, 0x00050081, 0x00000007, 0x00000107, 0x00000101, 0x00000106, 0x0003003E, 0x000000FB, 0x00000107, 0x00040008, 0x00000001, 0x000000D4, 
0x00000000, 0x0004003D, 0x00000007, 0x00000108, 0x000000FB, 0x0006000C, 0x00000007, 0x00000109, 0x00000002, 0x0000000D, 0x00000108, 0x00050085, 
0x00000007, 0x0000010B, 0x00000109, 0x0000010A, 0x0006000C, 0x00000007, 0x0000010C, 0x00000002, 0x0000000A, 0x0000010B, 0x000200FE, 0x0000010C, 
0x00010038, 0x00050036, 0x00000007, 0x0000002F, 0x00000000, 0x0000002B, 0x00030037, 0x00000015, 0x0000002C, 0x00030037, 0x00000015, 0x0000002D, 
0x00030037, 0x00000015, 0x0000002E, 0x000200F8, 0x00000030, 0x00040008, 0x00000001, 0x000000DA, 0x00000000, 0x0004003D, 0x00000007, 0x0000010F, 
0x0000002C, 0x0004003D, 0x00000007, 0x00000110, 0x0000002D, 0x0004003D, 0x00000007, 0x00000111, 0x0000002E, 0x0004003D, 0x00000007, 0x00000112, 
0x0000002E, 0x00050085, 0x00000007, 0x00000113, 0x00000111, 0x00000112, 0x0004003D, 0x00000007, 0x00000116, 0x0000002E, 0x00050085, 0x00000007, 
0x00000117, 0x00000115, 0x00000116, 0x00050083, 0x00000007, 0x00000118, 0x00000114, 0x00000117, 0x00050085, 0x00000007, 0x00000119, 0x00000113, 
0x00000118, 0x0008000C, 0x00000007, 0x0000011A, 0x00000002, 0x0000002E, 0x0000010F, 0x00000110, 0x00000119, 0x000200FE, 0x0000011A, 0x00010038, 
0x00050036, 0x00000007, 0x00000032, 0x00000000, 0x0000001B, 0x00030037, 0x0000001A, 0x00000031, 0x000200F8, 0x00000033, 0x0004003B, 0x0000001A, 
0x0000011D, 0x00000007, 0x0004003B, 0x0000001A, 0x00000120, 0x00000007, 0x0004003B, 0x00000015, 0x00000123, 0x00000007, 0x0004003B, 0x0000001A, 
0x00000124, 0x00000007, 0x0004003B, 0x00000015, 0x00000127, 0x00000007, 0x0004003B, 0x0000001A, 0x0000012D, 0x00000007, 0x0004003B, 0x00000015, 
0x0000012F, 0x00000007, 0x0004003B, 0x0000001A, 0x00000133, 0x00000007, 0x0004003B, 0x00000015, 0x00000135, 0x00000007, 0x0004003B, 0x0000001A, 
0x00000139, 0x00000007, 0x0004003B, 0x00000015, 0x0000013B, 0x00000007, 0x0004003B, 0x00000015, 0x0000013D, 0x00000007, 0x0004003B, 0x00000015, 
0x0000013F, 0x00000007, 0x0004003B, 0x00000015, 0x00000143, 0x00000007, 0x0004003B, 0x00000015, 0x00000145, 0x00000007, 0x0004003B, 0x00000015, 
0x00000147, 0x00000007, 0x0004003B, 0x00000015, 0x0000014B, 0x00000007, 0x0004003B, 0x00000015, 0x0000014C, 0x00000007, 0x0004003B, 0x00000015, 
0x0000014D, 0x00000007, 0x00040008, 0x00000001, 0x000000E1, 0x00000000, 0x0004003D, 0x00000009, 0x0000011E, 0x00000031, 0x0006000C, 0x00000009, 
0x0000011F, 0x00000002, 0x0000000A, 0x0000011E, 0x0003003E, 0x0000011D, 0x0000011F, 0x00040008, 0x00000001, 0x000000E3, 0x00000000, 0x0004003D, 
0x00000009, 0x00000121, 0x00000031, 0x0006000C, 0x00000009, 0x00000122, 0x00000002, 0x00000008, 0x00000121, 0x0003003E, 0x00000120, 0x00000122, 
0x00040008, 0x00000001, 0x000000E5, 0x00000000, 0x0004003D, 0x00000009, 0x00000125, 0x00000120, 0x0003003E, 0x00000124, 0x00000125, 0x00050039, 
0x00000007, 0x00000126, 0x00000024, 0x00000124, 0x0003003E, 0x00000123, 0x00000126, 0x00040008, 0x00000001, 0x000000E7, 0x00000000, 0x0004003D, 
0x00000009, 0x00000128, 0x00000120, 0x00050081, 0x00000009, 0x0000012C, 0x00000128, 0x0000012B, 0x0003003E, 0x0000012D, 0x0000012C, 0x00050039, 
0x00000007, 0x0000012E, 0x00000024, 0x0000012D, 0x0003003E, 0x00000127, 0x0000012E, 0x00040008, 0x00000001, 0x000000E9, 0x00000000, 0x0004003D, 
0x00000009, 0x00000130, 0x00000120, 0x00050081, 0x00000009, 0x00000132, 0x00000130, 0x00000131, 0x0003003E, 0x00000133, 0x00000132, 0x00050039, 
0x00000007, 0x00000134, 0x00000024, 0x00000133, 0x0003003E, 0x0000012F, 0x00000134, 0x00040008, 0x00000001, 0x000000EB, 0x00000000, 0x0004003D, 
0x00000009, 0x00000136, 0x00000120, 0x00050081, 0x00000009, 0x00000138, 0x00000136, 0x00000137, 0x0003003E, 0x00000139, 0x00000138, 0x00050039, 
0x00000007, 0x0000013A, 0x00000024, 0x00000139, 0x0003003E, 0x00000135, 0x0000013A, 0x00040008, 0x00000001, 0x000000EC, 0x00000000, 0x0004003D, 
0x00000007, 0x0000013C, 0x00000123, 0x0003003E, 0x0000013B, 0x0000013C, 0x0004003D, 0x00000007, 0x0000013E, 0x00000127, 0x0003003E, 0x0000013D, 
0x0000013E, 0x00050041, 0x00000015, 0x00000140, 0x0000011D, 0x000000A9, 0x0004003D, 0x00000007, 0x00000141, 0x00000140, 0x0003003E, 0x0000013F, 
0x00000141, 0x00070039, 0x00000007, 0x00000142, 0x0000002F, 0x0000013B, 0x0000013D, 0x0000013F, 0x0004003D, 0x00000007, 0x00000144, 0x0000012F, 
0x0003003E, 0x00000143, 0x00000144, 0x0004003D, 0x00000007, 0x00000146, 0x00000135, 0x0003003E, 0x00000145, 0x00000146, 0x00050041, 0x00000015, 
0x00000148, 0x0000011D, 0x000000A9, 0x0004003D, 0x00000007, 0x00000149, 0x00000148, 0x0003003E, 0x00000147, 0x00000149, 0x00070039, 0x00000007, 
0x0000014A, 0x0000002F, 0x00000143, 0x00000145, 0x00000147, 0x0003003E, 0x0000014B, 0x00000142, 0x0003003E, 0x0000014C, 0x0000014A, 0x00050041, 
0x00000015, 0x0000014E, 0x0000011D, 0x00000098, 0x0004003D, 0x00000007, 0x0000014F, 0x0000014E, 0x0003003E, 0x0000014D, 0x0000014F, 0x00070039, 
0x00000007, 0x00000150, 0x0000002F, 0x0000014B, 0x0000014C, 0x0000014D, 0x000200FE, 0x00000150, 0x00010038, 0x00050036, 0x00000007, 0x00000035, 
0x00000000, 0x00000027, 0x00030037, 0x00000026, 0x00000034, 0x000200F8, 0x00000036, 0x0004003B, 0x00000026, 0x00000153, 0x00000007, 0x0004003B, 
0x00000026, 0x00000156, 0x00000007, 0x0004003B, 0x00000015, 0x00000159, 0x00000007, 0x0004003B, 0x00000026, 0x0000015A, 0x00000007, 0x0004003B, 
0x00000015, 0x0000015D, 0x00000007, 0x0004003B, 0x00000026, 0x00000161, 0x00000007, 0x0004003B, 0x00000015, 0x00000163, 0x00000007, 0x0004003B, 
0x00000026, 0x00000167, 0x00000007, 0x0004003B, 0x00000015, 0x00000169, 0x00000007, 0x0004003B, 0x00000026, 0x0000016D, 0x00000007, 0x0004003B, 
0x00000015, 0x0000016F, 0x00000007, 0x0004003B, 0x00000026, 0x00000173, 0x00000007, 0x0004003B, 0x00000015, 0x00000175, 0x00000007, 0x0004003B, 
0x00000026, 0x00000179, 0x00000007, 0x0004003B, 0x00000015, 0x0000017B, 0x00000007, 0x0004003B, 0x00000026, 0x0000017F, 0x00000007, 0x0004003B, 
0x00000015, 0x00000181, 0x00000007, 0x0004003B, 0x00000026, 0x00000185, 0x00000007, 0x0004003B, 0x00000015, 0x00000187, 0x00000007, 0x0004003B, 
0x00000015, 0x00000189, 0x00000007, 0x0004003B, 0x00000015, 0x0000018B, 0x00000007, 0x0004003B, 0x00000015, 0x0000018F, 0x00000007, 0x0004003B, 
0x00000015, 0x00000191, 0x00000007, 0x0004003B, 0x00000015, 0x00000193, 0x00000007, 0x0004003B, 0x00000015, 0x00000197, 0x00000007, 0x0004003B, 
0x00000015, 0x00000198, 0x00000007, 0x0004003B, 0x00000015, 0x00000199, 0x00000007, 0x0004003B, 0x00000015, 0x0000019D, 0x00000007, 0x0004003B, 
0x00000015, 0x0000019F, 0x00000007, 0x0004003B, 0x00000015, 0x000001A1, 0x00000007, 0x0004003B, 0x00000015, 0x000001A5, 0x00000007, 0x0004003B, 
0x00000015, 0x000001A7, 0x00000007, 0x0004003B, 0x00000015, 0x000001A9, 0x00000007, 0x0004003B, 0x00000015, 0x000001AD, 0x00000007, 0x0004003B, 
0x00000015, 0x000001AE, 0x00000007, 0x0004003B, 0x00000015, 0x000001AF, 0x00000007, 0x0004003B, 0x00000015, 0x000001B3, 0x00000007, 0x0004003B, 
0x00000015, 0x000001B4, 0x00000007, 0x0004003B, 0x00000015, 0x000001B5, 0x00000007, 0x00040008, 0x00000001, 0x000000F3, 0x00000000, 0x0004003D, 
0x00000008, 0x00000154, 0x00000034, 0x0006000C, 0x00000008, 0x00000155, 0x00000002, 0x0000000A, 0x00000154, 0x0003003E, 0x00000153, 0x00000155, 
0x00040008, 0x00000001, 0x000000F5, 0x00000000, 0x0004003D, 0x00000008, 0x00000157, 0x00000034, 0x0006000C, 0x00000008, 0x00000158, 0x00000002, 
0x00000008, 0x00000157, 0x0003003E, 0x00000156, 0x00000158, 0x00040008, 0x00000001, 0x000000F7, 0x00000000, 0x0004003D, 0x00000008, 0x0000015B, 
0x00000156, 0x0003003E, 0x0000015A, 0x0000015B, 0x00050039, 0x00000007, 0x0000015C, 0x00000029, 0x0000015A, 0x0003003E, 0x00000159, 0x0000015C, 
0x00040008, 0x00000001, 0x000000F9, 0x00000000, 0x0004003D, 0x00000008, 0x0000015E, 0x00000156, 0x00050081, 0x00000008, 0x00000160, 0x0000015E, 
0x0000015F, 0x0003003E, 0x00000161, 0x00000160, 0x00050039, 0x00000007, 0x00000162, 0x00000029, 0x00000161, 0x0003003E, 0x0000015D, 0x00000162, 
0x00040008, 0x00000001, 0x000000FB, 0x00000000, 0x0004003D, 0x00000008, 0x00000164, 0x00000156, 0x00050081, 0x00000008, 0x00000166, 0x00000164, 
0x00000165, 0x0003003E, 0x00000167, 0x00000166, 0x00050039, 0x00000007, 0x00000168, 0x00000029, 0x00000167, 0x0003003E, 0x00000163, 0x00000168, 
0x00040008, 0x00000001, 0x000000FD, 0x00000000, 0x0004003D, 0x00000008, 0x0000016A, 0x00000156, 0x00050081, 0x00000008, 0x0000016C, 0x0000016A, 
0x0000016B, 0x0003003E, 0x0000016D, 0x0000016C, 0x00050039, 0x00000007, 0x0000016E, 0x00000029, 0x0000016D, 0x0003003E, 0x00000169, 0x0000016E, 
0x00040008, 0x00000001, 0x000000FF, 0x00000000, 0x0004003D, 0x00000008, 0x00000170, 0x00000156, 0x00050081, 0x00000008, 0x00000172, 0x00000170, 
0x00000171, 0x0003003E, 0x00000173, 0x00000172, 0x00050039, 0x00000007, 0x00000174, 0x00000029, 0x00000173, 0x0003003E, 0x0000016F, 0x00000174, 
0x00040008, 0x00000001, 0x00000101, 0x00000000, 0x0004003D, 0x00000008, 0x00000176, 0x00000156, 0x00050081, 0x00000008, 0x00000178, 0x00000176, 
0x00000177, 0x0003003E, 0x00000179, 0x00000178, 0x00050039, 0x00000007, 0x0000017A, 0x00000029, 0x00000179, 0x0003003E, 0x00000175, 0x0000017A, 
0x00040008, 0x00000001, 0x00000103, 0x00000000, 0x0004003D, 0x00000008, 0x0000017C, 0x00000156, 0x00050081, 0x00000008, 0x0000017E, 0x0000017C, 
0x0000017D, 0x0003003E, 0x0000017F, 0x0000017E, 0x00050039, 0x00000007, 0x00000180, 0x00000029, 0x0000017F, 0x0003003E, 0x0000017B, 0x00000180, 
0x00040008, 0x00000001, 0x00000105, 0x00000000, 0x0004003D, 0x00000008, 0x00000182, 0x00000156, 0x00050081, 0x00000008, 0x00000184, 0x00000182, 
0x00000183, 0x0003003E, 0x00000185, 0x00000184, 0x00050039, 0x00000007, 0x00000186, 0x00000029, 0x00000185, 0x0003003E, 0x00000181, 0x00000186, 
0x00040008, 0x00000001, 0x00000106, 0x00000000, 0x0004003D, 0x00000007, 0x00000188, 0x00000159, 0x0003003E, 0x00000187, 0x00000188, 0x0004003D, 
0x00000007, 0x0000018A, 0x0000015D, 0x0003003E, 0x00000189, 0x0000018A, 0x00050041, 0x00000015, 0x0000018C, 0x00000153, 0x000000A9, 0x0004003D, 
0x00000007, 0x0000018D, 0x0000018C, 0x0003003E, 0x0000018B, 0x0000018D, 0x00070039, 0x00000007, 0x0000018E, 0x0000002F, 0x00000187, 0x00000189, 
0x0000018B, 0x0004003D, 0x00000007, 0x00000190, 0x00000163, 0x0003003E, 0x0000018F, 0x00000190, 0x0004003D, 0x00000007, 0x00000192, 0x00000169, 
0x0003003E, 0x00000191, 0x00000192, 0x00050041, 0x00000015, 0x00000194, 0x00000153, 0x000000A9, 0x0004003D, 0x00000007, 0x00000195, 0x00000194, 
0x0003003E, 0x00000193, 0x00000195, 0x00070039, 0x00000007, 0x00000196, 0x0000002F, 0x0000018F, 0x00000191, 0x00000193, 0x0003003E, 0x00000197, 
0x0000018E, 0x0003003E, 0x00000198, 0x00000196, 0x00050041, 0x00000015, 0x0000019A, 0x00000153, 0x00000098, 0x0004003D, 0x00000007, 0x0000019B, 
0x0000019A, 0x0003003E, 0x00000199, 0x0000019B, 0x00070039, 0x00000007, 0x0000019C, 0x0000002F, 0x00000197, 0x00000198, 0x00000199, 0x0004003D, 
0x00000007, 0x0000019E, 0x0000016F, 0x0003003E, 0x0000019D, 0x0000019E, 0x0004003D, 0x00000007, 0x000001A0, 0x00000175, 0x0003003E, 0x0000019F, 
0x000001A0, 0x00050041, 0x00000015, 0x000001A2, 0x00000153, 0x000000A9, 0x0004003D, 0x00000007, 0x000001A3, 0x000001A2, 0x0003003E, 0x000001A1, 
0x000001A3, 0x00070039, 0x00000007, 0x000001A4, 0x0000002F, 0x0000019D, 0x0000019F, 0x000001A1, 0x0004003D, 0x00000007, 0x000001A6, 0x0000017B, 
0x0003003E, 0x000001A5, 0x000001A6, 0x0004003D, 0x00000007, 0x000001A8, 0x00000181, 0x0003003E, 0x000001A7, 0x000001A8, 0x00050041, 0x00000015, 
0x000001AA, 0x00000153, 0x000000A9, 0x0004003D, 0x00000007, 0x000001AB, 0x000001AA, 0x0003003E, 0x000001A9, 0x000001AB, 0x00070039, 0x00000007, 
0x000001AC, 0x0000002F, 0x000001A5, 0x000001A7, 0x000001A9, 0x0003003E, 0x000001AD, 0x000001A4, 0x0003003E, 0x000001AE, 0x000001AC, 0x00050041, 
0x00000015, 0x000001B0, 0x00000153, 0x00000098, 0x0004003D, 0x00000007, 0x000001B1, 0x000001B0, 0x0003003E, 0x000001AF, 0x000001B1, 0x00070039, 
0x00000007, 0x000001B2, 0x0000002F, 0x000001AD, 0x000001AE, 0x000001AF, 0x0003003E, 0x000001B3, 0x0000019C, 0x0003003E, 0x000001B4, 0x000001B2, 
0x00050041, 0x00000015, 0x000001B6, 0x00000153, 0x00000102, 0x0004003D, 0x00000007, 0x000001B7, 0x000001B6, 0x0003003E, 0x000001B5, 0x000001B7, 
0x00070039, 0x00000007, 0x000001B8, 0x0000002F, 0x000001B3, 0x000001B4, 0x000001B5, 0x000200FE, 0x000001B8, 0x00010038, 0x00050036, 0x00000008, 
0x00000039, 0x00000000, 0x00000037, 0x00030037, 0x00000026, 0x00000038, 0x000200F8, 0x0000003A, 0x00040008, 0x00000001, 0x0000010C, 0x00000000, 
0x0004003D, 0x00000008, 0x000001BB, 0x00000038, 0x00060050, 0x00000008, 0x000001BC, 0x0000012A, 0x0000012A, 0x0000012A, 0x00060050, 0x00000008, 
0x000001BD, 0x00000129, 0x00000129, 0x00000129, 0x0008000C, 0x00000008, 0x000001BE, 0x00000002, 0x0000002B, 0x000001BB, 0x000001BC, 0x000001BD, 
0x000200FE, 0x000001BE, 0x00010038, 0x00050036, 0x00000007, 0x0000003C, 0x00000000, 0x00000016, 0x00030037, 0x00000015, 0x0000003B, 0x000200F8, 
0x0000003D, 0x00040008, 0x00000001, 0x00000112, 0x00000000, 0x0004003D, 0x00000007, 0x000001C1, 0x0000003B, 0x0008000C, 0x00000007, 0x000001C2, 
0x00000002, 0x0000002B, 0x000001C1, 0x0000012A, 0x00000129, 0x000200FE, 0x000001C2, 0x00010038, 0x00050036, 0x00000008, 0x00000041, 0x00000000, 
0x0000003E, 0x00030037, 0x00000026, 0x0000003F, 0x00030037, 0x00000026, 0x00000040, 0x000200F8, 0x00000042, 0x0004003B, 0x00000026, 0x000001C5, 
0x00000007, 0x0004003B, 0x00000015, 0x000001C8, 0x00000007, 0x0004003B, 0x00000015, 0x000001D0, 0x00000007, 0x00040008, 0x00000001, 0x00000119, 
0x00000000, 0x0004003D, 0x00000008, 0x000001C6, 0x0000003F, 0x0006000C, 0x00000008, 0x000001C7, 0x00000002, 0x00000045, 0x000001C6, 0x0003003E, 
0x000001C5, 0x000001C7, 0x00040008, 0x00000001, 0x0000011B, 0x00000000, 0x0004003D, 0x00000008, 0x000001C9, 0x000001C5, 0x0004003D, 0x00000008, 
0x000001CA, 0x00000040, 0x00050094, 0x00000007, 0x000001CB, 0x000001C9, 0x000001CA, 0x00050085, 0x00000007, 0x000001CD, 0x000001CB, 0x000001CC, 
0x00050081, 0x00000007, 0x000001CE, 0x000001CD, 0x000001CC, 0x00050083, 0x00000007, 0x000001CF, 0x00000129, 0x000001CE, 0x0003003E, 0x000001C8, 
0x000001CF, 0x00040008, 0x00000001, 0x0000011D, 0x00000000, 0x0004003D, 0x00000007, 0x000001D2, 0x000001C8, 0x00050088, 0x00000007, 0x000001D3, 
0x000001D1, 0x000001D2, 0x0003003E, 0x000001D0, 0x000001D3, 0x00040008, 0x00000001, 0x0000011E, 0x00000000, 0x0004003D, 0x00000007, 0x000001D4, 
0x000001C8, 0x0004007F, 0x00000007, 0x000001D5, 0x000001D4, 0x00050085, 0x00000007, 0x000001D7, 0x000001D5, 0x000001D6, 0x0006000C, 0x00000007, 
0x000001D8, 0x00000002, 0x0000001B, 0x000001D7, 0x00050085, 0x00000007, 0x000001DA, 0x000001D8, 0x000001D9, 0x0004003D, 0x00000007, 0x000001DB, 
0x000001D0, 0x00050081, 0x00000007, 0x000001DC, 0x000001DB, 0x000001DA, 0x0003003E, 0x000001D0, 0x000001DC, 0x00040008, 0x00000001, 0x0000011F, 
0x00000000, 0x0004003D, 0x00000007, 0x000001DD, 0x000001D0, 0x0007000C, 0x00000007, 0x000001DF, 0x00000002, 0x00000025, 0x000001DD, 0x000001DE, 
0x0003003E, 0x000001D0, 0x000001DF, 0x00040008, 0x00000001, 0x00000120, 0x00000000, 0x0004003D, 0x00000008, 0x000001E2, 0x000001E1, 0x0004003D, 
0x00000007, 0x000001E3, 0x000001D0, 0x0005008E, 0x00000008, 0x000001E4, 0x000001E2, 0x000001E3, 0x0005008E, 0x00000008, 0x000001E6, 0x000001E4, 
0x000001E5, 0x000200FE, 0x000001E6, 0x00010038, 0x00050036, 0x00000008, 0x00000045, 0x00000000, 0x0000003E, 0x00030037, 0x00000026, 0x00000043, 
0x00030037, 0x00000026, 0x00000044, 0x000200F8, 0x00000046, 0x0004003B, 0x00000026, 0x000001E9, 0x00000007, 0x0004003B, 0x00000015, 0x000001EE, 
0x00000007, 0x0004003B, 0x00000015, 0x000001F8, 0x00000007, 0x0004003B, 0x0000001A, 0x00000200, 0x00000007, 0x0004003B, 0x0000001A, 0x00000209, 
0x00000007, 0x0004003B, 0x0000001A, 0x00000216, 0x00000007, 0x0004003B, 0x0000001A, 0x00000224, 0x00000007, 0x0004003B, 0x00000015, 0x00000235, 
0x00000007, 0x0004003B, 0x00000015, 0x00000244, 0x00000007, 0x0004003B, 0x00000015, 0x00000249, 0x00000007, 0x0004003B, 0x00000026, 0x0000024D, 
0x00000007, 0x0004003B, 0x00000026, 0x0000024F, 0x00000007, 0x00040008, 0x00000001, 0x00000127, 0x00000000, 0x0004003D, 0x00000008, 0x000001EB, 
0x000001EA, 0x0004003D, 0x00000008, 0x000001ED, 0x000001EC, 0x00050041, 0x00000015, 0x000001EF, 0x00000043, 0x00000098, 0x0004003D, 0x00000007, 
0x000001F0, 0x000001EF, 0x0003003E, 0x000001EE, 0x000001F0, 0x00050039, 0x00000007, 0x000001F1, 0x0000003C, 0x000001EE, 0x0007000C, 0x00000007, 
0x000001F3, 0x00000002, 0x0000001A, 0x000001F1, 0x000001F2, 0x00060050, 0x00000008, 0x000001F4, 0x000001F3, 0x000001F3, 0x000001F3, 0x0008000C, 
0x00000008, 0x000001F5, 0x00000002, 0x0000002E, 0x000001EB, 0x000001ED, 0x000001F4, 0x0005008E, 0x00000008, 0x000001F7, 0x000001F5, 0x000001F6, 
0x0003003E, 0x000001E9, 0x000001F7, 0x00040008, 0x00000001, 0x00000129, 0x00000000, 0x0004003D, 0x00000008, 0x000001F9, 0x00000043, 0x0007004F, 
0x00000009, 0x000001FA, 0x000001F9, 0x000001F9, 0x00000000, 0x00000002, 0x00050041, 0x00000015, 0x000001FB, 0x00000043, 0x00000098, 0x0004003D, 
0x00000007, 0x000001FC, 0x000001FB, 0x00050050, 0x00000009, 0x000001FD, 0x000001FC, 0x000001FC, 0x00050088, 0x00000009, 0x000001FE, 0x000001FA, 
0x000001FD, 0x0005008E, 0x00000009, 0x000001FF, 0x000001FE, 0x00000129, 0x0003003E, 0x00000200, 0x000001FF, 0x00050039, 0x00000007, 0x00000201, 
0x00000032, 0x00000200, 0x0003003E, 0x000001F8, 0x00000201, 0x00040008, 0x00000001, 0x0000012A, 0x00000000, 0x0004003D, 0x00000008, 0x00000202, 
0x00000043, 0x0007004F, 0x00000009, 0x00000203, 0x00000202, 0x00000202, 0x00000000, 0x00000002, 0x00050041, 0x00000015, 0x00000204, 0x00000043, 
0x00000098, 0x0004003D, 0x00000007, 0x00000205, 0x00000204, 0x00050050, 0x00000009, 0x00000206, 0x00000205, 0x00000205, 0x00050088, 0x00000009, 
0x00000207, 0x00000203, 0x00000206, 0x0005008E, 0x00000009, 0x00000208, 0x00000207, 0x00000115, 0x0003003E, 0x00000209, 0x00000208, 0x00050039, 
0x00000007, 0x0000020A, 0x00000032, 0x00000209, 0x00050085, 0x00000007, 0x0000020B, 0x0000020A, 0x000001CC, 0x0004003D, 0x00000007, 0x0000020C, 
0x000001F8, 0x00050081, 0x00000007, 0x0000020D, 0x0000020C, 0x0000020B, 0x0003003E, 0x000001F8, 0x0000020D, 0x00040008, 0x00000001, 0x0000012B, 
0x00000000, 0x0004003D, 0x00000008, 0x0000020E, 0x00000043, 0x0007004F, 0x00000009, 0x0000020F, 0x0000020E, 0x0000020E, 0x00000000, 0x00000002, 
0x00050041, 0x00000015, 0x00000210, 0x00000043, 0x00000098, 0x0004003D, 0x00000007, 0x00000211, 0x00000210, 0x00050050, 0x00000009, 0x00000212, 
0x00000211, 0x00000211, 0x00050088, 0x00000009, 0x00000213, 0x0000020F, 0x00000212, 0x0005008E, 0x00000009, 0x00000215, 0x00000213, 0x00000214, 
0x0003003E, 0x00000216, 0x00000215, 0x00050039, 0x00000007, 0x00000217, 0x00000032, 0x00000216, 0x00050085, 0x00000007, 0x00000219, 0x00000217, 
0x00000218, 0x0004003D, 0x00000007, 0x0000021A, 0x000001F8, 0x00050081, 0x00000007, 0x0000021B, 0x0000021A, 0x00000219, 0x0003003E, 0x000001F8, 
0x0000021B, 0x00040008, 0x00000001, 0x0000012C, 0x00000000, 0x0004003D, 0x00000008, 0x0000021C, 0x00000043, 0x0007004F, 0x00000009, 0x0000021D, 
0x0000021C, 0x0000021C, 0x00000000, 0x00000002, 0x00050041, 0x00000015, 0x0000021E, 0x00000043, 0x00000098, 0x0004003D, 0x00000007, 0x0000021F, 
0x0000021E, 0x00050050, 0x00000009, 0x00000220, 0x0000021F, 0x0000021F, 0x00050088, 0x00000009, 0x00000221, 0x0000021D, 0x00000220, 0x0005008E, 
0x00000009, 0x00000223, 0x00000221, 0x00000222, 0x0003003E, 0x00000224, 0x00000223, 0x00050039, 0x00000007, 0x00000225, 0x00000032, 0x00000224, 
0x00050085, 0x00000007, 0x00000227, 0x00000225, 0x00000226, 0x0004003D, 0x00000007, 0x00000228, 0x000001F8, 0x00050081, 0x00000007, 0x00000229, 
0x00000228, 0x00000227, 0x0003003E, 0x000001F8, 0x00000229, 0x00040008, 0x00000001, 0x0000012D, 0x00000000, 0x0004003D, 0x00000007, 0x0000022A, 
0x000001F8, 0x0006000C, 0x00000007, 0x0000022B, 0x00000002, 0x00000004, 0x0000022A, 0x0007000C, 0x00000007, 0x0000022C, 0x00000002, 0x0000001A, 
0x0000022B, 0x00000114, 0x0003003E, 0x000001F8, 0x0000022C, 0x00040008, 0x00000001, 0x0000012E, 0x00000000, 0x0004003D, 0x00000007, 0x0000022D, 
0x000001F8, 0x00050085, 0x00000007, 0x0000022F, 0x0000022D, 0x0000022E, 0x0004003D, 0x00000007, 0x00000230, 0x000001F8, 0x00050041, 0x00000015, 
0x00000231, 0x00000043, 0x00000098, 0x0004003D, 0x00000007, 0x00000232, 0x00000231, 0x00050085, 0x00000007, 0x00000233, 0x00000232, 0x00000222, 
0x0006000C, 0x00000007, 0x00000234, 0x00000002, 0x00000004, 0x00000233, 0x0003003E, 0x00000235, 0x00000234, 0x00050039, 0x00000007, 0x00000236, 
0x0000003C, 0x00000235, 0x0008000C, 0x00000007, 0x00000237, 0x00000002, 0x0000002E, 0x0000022F, 0x00000230, 0x00000236, 0x0003003E, 0x000001F8, 
0x00000237, 0x00040008, 0x00000001, 0x0000012F, 0x00000000, 0x0004003D, 0x00000008, 0x00000238, 0x000001E9, 0x0004003D, 0x00000008, 0x00000239, 
0x000001E1, 0x0005008E, 0x00000008, 0x0000023B, 0x00000239, 0x0000023A, 0x00050081, 0x00000008, 0x0000023C, 0x00000183, 0x0000023B, 0x0005008E, 
0x00000008, 0x0000023E, 0x0000023C, 0x0000023D, 0x00050041, 0x00000015, 0x0000023F, 0x00000043, 0x00000098, 0x0004003D, 0x00000007, 0x00000240, 
0x0000023F, 0x00050081, 0x00000007, 0x00000241, 0x00000240, 0x0000022E, 0x00050085, 0x00000007, 0x00000243, 0x00000241, 0x00000242, 0x0003003E, 
0x00000244, 0x00000243, 0x00050039, 0x00000007, 0x00000245, 0x0000003C, 0x00000244, 0x0005008E, 0x00000008, 0x00000246, 0x0000023E, 0x00000245, 
0x0004003D, 0x00000007, 0x00000247, 0x000001F8, 0x00050085, 0x00000007, 0x00000248, 0x00000247, 0x00000226, 0x0003003E, 0x00000249, 0x00000248, 
0x00050039, 0x00000007, 0x0000024A, 0x0000003C, 0x00000249, 0x00060050, 0x00000008, 0x0000024B, 0x0000024A, 0x0000024A, 0x0000024A, 0x0008000C, 
0x00000008, 0x0000024C, 0x00000002, 0x0000002E, 0x00000238, 0x00000246, 0x0000024B, 0x0003003E, 0x000001E9, 0x0000024C, 0x00040008, 0x00000001, 
0x00000130, 0x00000000, 0x0004003D, 0x00000008, 0x0000024E, 0x00000043, 0x0003003E, 0x0000024D, 0x0000024E, 0x0004003D, 0x00000008, 0x00000250, 
0x00000044, 0x0003003E, 0x0000024F, 0x00000250, 0x00060039, 0x00000008, 0x00000251, 0x00000041, 0x0000024D, 0x0000024F, 0x0004003D, 0x00000008, 
0x00000252, 0x000001E9, 0x00050081, 0x00000008, 0x00000253, 0x00000252, 0x00000251, 0x0003003E, 0x000001E9, 0x00000253, 0x00040008, 0x00000001, 
0x00000131, 0x00000000, 0x0004003D, 0x00000008, 0x00000254, 0x000001E9, 0x000200FE, 0x00000254, 0x00010038, 0x00050036, 0x00000008, 0x0000004B, 
0x00000000, 0x00000047, 0x00030037, 0x00000026, 0x00000048, 0x00030037, 0x00000026, 0x00000049, 0x00030037, 0x00000015, 0x0000004A, 0x000200F8, 
0x0000004C, 0x0004003B, 0x00000026, 0x00000257, 0x00000007, 0x0004003B, 0x00000026, 0x00000258, 0x00000007, 0x0004003B, 0x00000026, 0x0000025A, 
0x00000007, 0x0004003B, 0x00000015, 0x0000025D, 0x00000007, 0x0004003B, 0x00000015, 0x00000264, 0x00000007, 0x0004003B, 0x00000015, 0x00000289, 
0x00000007, 0x0004003B, 0x00000015, 0x00000295, 0x00000007, 0x0004003B, 0x00000015, 0x00000297, 0x00000007, 0x0004003B, 0x00000015, 0x0000029F, 
0x00000007, 0x0004003B, 0x00000015, 0x000002AF, 0x00000007, 0x00040008, 0x00000001, 0x00000138, 0x00000000, 0x0004003D, 0x00000008, 0x00000259, 
0x00000048, 0x0003003E, 0x00000258, 0x00000259, 0x0004003D, 0x00000008, 0x0000025B, 0x00000049, 0x0003003E, 0x0000025A, 0x0000025B, 0x00060039, 
0x00000008, 0x0000025C, 0x00000045, 0x00000258, 0x0000025A, 0x0003003E, 0x00000257, 0x0000025C, 0x00040008, 0x00000001, 0x0000013A, 0x00000000, 
0x00050041, 0x00000015, 0x0000025E, 0x00000048, 0x00000102, 0x0004003D, 0x00000007, 0x0000025F, 0x0000025E, 0x00050041, 0x00000015, 0x00000260, 
0x00000048, 0x000000A9, 0x0004003D, 0x00000007, 0x00000261, 0x00000260, 0x0007000C, 0x00000007, 0x00000262, 0x00000002, 0x00000019, 0x0000025F, 
0x00000261, 0x00050085, 0x00000007, 0x00000263, 0x00000262, 0x00000214, 0x0003003E, 0x0000025D, 0x00000263, 0x00040008, 0x00000001, 0x0000013C, 
0x00000000, 0x0004003D, 0x00000007, 0x00000266, 0x0000025D, 0x00050085, 0x00000007, 0x00000267, 0x00000265, 0x00000266, 0x0006000C, 0x00000007, 
0x00000268, 0x00000002, 0x0000000D, 0x00000267, 0x0004003D, 0x00000007, 0x0000026A, 0x0000025D, 0x00050085, 0x00000007, 0x0000026B, 0x00000269, 
0x0000026A, 0x0006000C, 0x00000007, 0x0000026C, 0x00000002, 0x0000000D, 0x0000026B, 0x00050081, 0x00000007, 0x0000026D, 0x00000268, 0x0000026C, 
0x0004003D, 0x00000007, 0x0000026F, 0x0000025D, 0x00050085, 0x00000007, 0x00000270, 0x0000026E, 0x0000026F, 0x0006000C, 0x00000007, 0x00000271, 
0x00000002, 0x0000000D, 0x00000270, 0x00050081, 0x00000007, 0x00000272, 0x0000026D, 0x00000271, 0x00050085, 0x00000007, 0x00000274, 0x00000272, 
0x00000273, 0x0006000C, 0x00000007, 0x00000275, 0x00000002, 0x00000008, 0x00000274, 0x0003003E, 0x00000264, 0x00000275, 0x00040008, 0x00000001, 
0x0000013D, 0x00000000, 0x0004003D, 0x00000007, 0x00000276, 0x0000025D, 0x00050085, 0x00000007, 0x00000277, 0x00000276, 0x00000214, 0x0003003E, 
0x0000025D, 0x00000277, 0x00040008, 0x00000001, 0x0000013E, 0x00000000, 0x0004003D, 0x00000007, 0x00000278, 0x0000025D, 0x00050085, 0x00000007, 
0x00000279, 0x00000242, 0x00000278, 0x0006000C, 0x00000007, 0x0000027A, 0x00000002, 0x0000000D, 0x00000279, 0x0004003D, 0x00000007, 0x0000027B, 
0x0000025D, 0x00050085, 0x00000007, 0x0000027C, 0x00000269, 0x0000027B, 0x0006000C, 0x00000007, 0x0000027D, 0x00000002, 0x0000000D, 0x0000027C, 
0x00050081, 0x00000007, 0x0000027E, 0x0000027A, 0x0000027D, 0x0004003D, 0x00000007, 0x0000027F, 0x0000025D, 0x00050085, 0x00000007, 0x00000280, 
0x0000026E, 0x0000027F, 0x0006000C, 0x00000007, 0x00000281, 0x00000002, 0x0000000D, 0x00000280, 0x00050081, 0x00000007, 0x00000282, 0x0000027E, 
0x00000281, 0x00050085, 0x00000007, 0x00000283, 0x00000282, 0x00000273, 0x0006000C, 0x00000007, 0x00000284, 0x00000002, 0x00000008, 0x00000283, 
0x00050085, 0x00000007, 0x00000286, 0x00000284, 0x00000285, 0x0004003D, 0x00000007, 0x00000287, 0x00000264, 0x00050081, 0x00000007, 0x00000288, 
0x00000287, 0x00000286, 0x0003003E, 0x00000264, 0x00000288, 0x00040008, 0x00000001, 0x00000140, 0x00000000, 0x00050041, 0x00000015, 0x0000028A, 
0x00000048, 0x00000098, 0x0004003D, 0x00000007, 0x0000028B, 0x0000028A, 0x00050085, 0x00000007, 0x0000028C, 0x0000028B, 0x00000222, 0x0004003D, 
0x00000007, 0x0000028D, 0x00000264, 0x00050083, 0x00000007, 0x0000028E, 0x0000028C, 0x0000028D, 0x00050083, 0x00000007, 0x00000290, 0x0000028E, 
0x0000028F, 0x0004003D, 0x00000007, 0x00000291, 0x0000004A, 0x00050081, 0x00000007, 0x00000292, 0x00000290, 0x00000291, 0x00050085, 0x00000007, 
0x00000294, 0x00000292, 0x00000293, 0x0003003E, 0x00000295, 0x00000294, 0x00050039, 0x00000007, 0x00000296, 0x0000003C, 0x00000295, 0x0003003E, 
0x00000289, 0x00000296, 0x00040008, 0x00000001, 0x00000142, 0x00000000, 0x0004003D, 0x00000007, 0x00000298, 0x0000025D, 0x00050085, 0x00000007, 
0x0000029A, 0x00000298, 0x00000299, 0x0006000C, 0x00000007, 0x0000029B, 0x00000002, 0x0000000D, 0x0000029A, 0x0006000C, 0x00000007, 0x0000029C, 
0x00000002, 0x00000006, 0x0000029B, 0x00050085, 0x00000007, 0x0000029D, 0x0000029C, 0x000001CC, 0x00050081, 0x00000007, 0x0000029E, 0x0000029D, 
0x000001CC, 0x0003003E, 0x00000297, 0x0000029E, 0x00040008, 0x00000001, 0x00000144, 0x00000000, 0x00050041, 0x00000015, 0x000002A0, 0x00000048, 
0x00000098, 0x0004003D, 0x00000007, 0x000002A1, 0x000002A0, 0x00050085, 0x00000007, 0x000002A3, 0x000002A1, 0x000002A2, 0x0006000C, 0x00000007, 
0x000002A4, 0x00000002, 0x0000000D, 0x000002A3, 0x0006000C, 0x00000007, 0x000002A5, 0x00000002, 0x00000006, 0x000002A4, 0x00050085, 0x00000007, 
0x000002A6, 0x000002A5, 0x000001CC, 0x00050081, 0x00000007, 0x000002A7, 0x000002A6, 0x000001CC, 0x0003003E, 0x0000029F, 0x000002A7, 0x00040008, 
0x00000001, 0x00000145, 0x00000000, 0x0004003D, 0x00000007, 0x000002A8, 0x00000289, 0x0004003D, 0x00000007, 0x000002A9, 0x0000029F, 0x0004003D, 
0x00000007, 0x000002AA, 0x00000297, 0x00050085, 0x00000007, 0x000002AB, 0x000002A9, 0x000002AA, 0x00050083, 0x00000007, 0x000002AC, 0x00000129, 
0x000002AB, 0x00050085, 0x00000007, 0x000002AD, 0x000002AC, 0x000001D1, 0x00050081, 0x00000007, 0x000002AE, 0x000002A8, 0x000002AD, 0x0003003E, 
0x000002AF, 0x000002AE, 0x00050039, 0x00000007, 0x000002B0, 0x0000003C, 0x000002AF, 0x0003003E, 0x00000289, 0x000002B0, 0x00040008, 0x00000001, 
0x00000146, 0x00000000, 0x0004003D, 0x00000008, 0x000002B1, 0x00000257, 0x00050085, 0x00000008, 0x000002B4, 0x000002B1, 0x000002B3, 0x0004003D, 
0x00000008, 0x000002B5, 0x00000257, 0x0004003D, 0x00000007, 0x000002B6, 0x00000289, 0x00060050, 0x00000008, 0x000002B7, 0x000002B6, 0x000002B6, 
0x000002B6, 0x0008000C, 0x00000008, 0x000002B8, 0x00000002, 0x0000002E, 0x000002B4, 0x000002B5, 0x000002B7, 0x0003003E, 0x00000257, 0x000002B8, 
0x00040008, 0x00000001, 0x00000147, 0x00000000, 0x0004003D, 0x00000008, 0x000002B9, 0x00000257, 0x000200FE, 0x000002B9, 0x00010038, 0x00050036, 
0x00000009, 0x00000050, 0x00000000, 0x0000004D, 0x00030037, 0x0000001A, 0x0000004E, 0x00030037, 0x0000001A, 0x0000004F, 0x000200F8, 0x00000051, 
0x00040008, 0x00000001, 0x0000014D, 0x00000000, 0x00050041, 0x00000015, 0x000002BC, 0x0000004E, 0x000000A9, 0x0004003D, 0x00000007, 0x000002BD, 
0x000002BC, 0x00050041, 0x00000015, 0x000002BE, 0x0000004F, 0x000000A9, 0x0004003D, 0x00000007, 0x000002BF, 0x000002BE, 0x000500B8, 0x000002C0, 
0x000002C1, 0x000002BD, 0x000002BF, 0x000300F7, 0x000002C3, 0x00000000, 0x000400FA, 0x000002C1, 0x000002C2, 0x000002C6, 0x000200F8, 0x000002C2, 
0x00040008, 0x00000001, 0x0000014F, 0x00000000, 0x0004003D, 0x00000009, 0x000002C4, 0x0000004E, 0x000200FE, 0x000002C4, 0x000200F8, 0x000002C6, 
0x00040008, 0x00000001, 0x00000153, 0x00000000, 0x0004003D, 0x00000009, 0x000002C7, 0x0000004F, 0x000200FE, 0x000002C7, 0x000200F8, 0x000002C3, 
0x00030001, 0x00000009, 0x000002C9, 0x000200FE, 0x000002C9, 0x00010038, 0x00050036, 0x00000007, 0x00000055, 0x00000000, 0x00000052, 0x00030037, 
0x00000026, 0x00000053, 0x00030037, 0x00000026, 0x00000054, 0x000200F8, 0x00000056, 0x0004003B, 0x00000026, 0x000002CA, 0x00000007, 0x00040008, 
0x00000001, 0x0000015C, 0x00000000, 0x0004003D, 0x00000008, 0x000002CB, 0x00000053, 0x0006000C, 0x00000008, 0x000002CC, 0x00000002, 0x00000004, 
0x000002CB, 0x0004003D, 0x00000008, 0x000002CD, 0x00000054, 0x00050083, 0x00000008, 0x000002CE, 0x000002CC, 0x000002CD, 0x0003003E, 0x000002CA, 
0x000002CE, 0x00040008, 0x00000001, 0x0000015D, 0x00000000, 0x00050041, 0x00000015, 0x000002CF, 0x000002CA, 0x000000A9, 0x0004003D, 0x00000007, 
0x000002D0, 0x000002CF, 0x00050041, 0x00000015, 0x000002D1, 0x000002CA, 0x00000098, 0x0004003D, 0x00000007, 0x000002D2, 0x000002D1, 0x00050041, 
0x00000015, 0x000002D3, 0x000002CA, 0x00000102, 0x0004003D, 0x00000007, 0x000002D4, 0x000002D3, 0x0007000C, 0x00000007, 0x000002D5, 0x00000002, 
0x00000028, 0x000002D2, 0x000002D4, 0x0007000C, 0x00000007, 0x000002D6, 0x00000002, 0x00000028, 0x000002D0, 0x000002D5, 0x0007000C, 0x00000007, 
0x000002D7, 0x00000002, 0x00000025, 0x000002D6, 0x0000012A, 0x0004003D, 0x00000008, 0x000002D8, 0x000002CA, 0x00060050, 0x00000008, 0x000002D9, 
0x0000012A, 0x0000012A, 0x0000012A, 0x0007000C, 0x00000008, 0x000002DA, 0x00000002, 0x00000028, 0x000002D8, 0x000002D9, 0x0006000C, 0x00000007, 
0x000002DB, 0x00000002, 0x00000042, 0x000002DA, 0x00050081, 0x00000007, 0x000002DC, 0x000002D7, 0x000002DB, 0x000200FE, 0x000002DC, 0x00010038, 
0x00050036, 0x00000007, 0x0000005B, 0x00000000, 0x00000057, 0x00030037, 0x00000026, 0x00000058, 0x00030037, 0x00000015, 0x00000059, 0x00030037, 
0x00000015, 0x0000005A, 0x000200F8, 0x0000005C, 0x0004003B, 0x00000015, 0x000002DF, 0x00000007, 0x00040008, 0x00000001, 0x00000164, 0x00000000, 
0x0004003D, 0x00000008, 0x000002E0, 0x00000058, 0x0007004F, 0x00000009, 0x000002E1, 0x000002E0, 0x000002E0, 0x00000000, 0x00000001, 0x0006000C, 
0x00000007, 0x000002E2, 0x00000002, 0x00000042, 0x000002E1, 0x0004003D, 0x00000007, 0x000002E3, 0x00000059, 0x00050083, 0x00000007, 0x000002E4, 
0x000002E2, 0x000002E3, 0x0003003E, 0x000002DF, 0x000002E4, 0x00040008, 0x00000001, 0x00000165, 0x00000000, 0x0004003D, 0x00000007, 0x000002E5, 
0x000002DF, 0x00050041, 0x00000015, 0x000002E6, 0x00000058, 0x00000102, 0x0004003D, 0x00000007, 0x000002E7, 0x000002E6, 0x0006000C, 0x00000007, 
0x000002E8, 0x00000002, 0x00000004, 0x000002E7, 0x0004003D, 0x00000007, 0x000002E9, 0x0000005A, 0x00050083, 0x00000007, 0x000002EA, 0x000002E8, 
0x000002E9, 0x0007000C, 0x00000007, 0x000002EB, 0x00000002, 0x00000028, 0x000002E5, 0x000002EA, 0x0003003E, 0x000002DF, 0x000002EB, 0x00040008, 
0x00000001, 0x00000166, 0x00000000, 0x0004003D, 0x00000007, 0x000002EC, 0x000002DF, 0x000200FE, 0x000002EC, 0x00010038, 0x00050036, 0x00000007, 
0x00000060, 0x00000000, 0x0000002B, 0x00030037, 0x00000015, 0x0000005D, 0x00030037, 0x00000015, 0x0000005E, 0x00030037, 0x00000015, 0x0000005F, 
0x000200F8, 0x00000061, 0x00040008, 0x00000001, 0x0000016C, 0x00000000, 0x0004003D, 0x00000007, 0x000002EF, 0x0000005F, 0x0004003D, 0x00000007, 
0x000002F0, 0x0000005D, 0x00050085, 0x00000007, 0x000002F1, 0x000002EF, 0x000002F0, 0x0006000C, 0x00000007, 0x000002F2, 0x00000002, 0x0000001D, 
0x000002F1, 0x0004003D, 0x00000007, 0x000002F3, 0x0000005F, 0x0004003D, 0x00000007, 0x000002F4, 0x0000005E, 0x00050085, 0x00000007, 0x000002F5, 
0x000002F3, 0x000002F4, 0x0006000C, 0x00000007, 0x000002F6, 0x00000002, 0x0000001D, 0x000002F5, 0x00050081, 0x00000007, 0x000002F7, 0x000002F2, 
0x000002F6, 0x0006000C, 0x00000007, 0x000002F8, 0x00000002, 0x0000001E, 0x000002F7, 0x0004003D, 0x00000007, 0x000002F9, 0x0000005F, 0x00050088, 
0x00000007, 0x000002FA, 0x000002F8, 0x000002F9, 0x000200FE, 0x000002FA, 0x00010038, 0x00050036, 0x00000007, 0x00000065, 0x00000000, 0x00000062, 
0x00030037, 0x00000015, 0x00000063, 0x00030037, 0x00000015, 0x00000064, 0x000200F8, 0x00000066, 0x00040008, 0x00000001, 0x00000172, 0x00000000, 
0x0004003D, 0x00000007, 0x000002FD, 0x00000063, 0x0004003D, 0x00000007, 0x000002FE, 0x00000064, 0x0005008D, 0x00000007, 0x000002FF, 0x000002FD, 
0x000002FE, 0x0004003D, 0x00000007, 0x00000300, 0x00000064, 0x00050085, 0x00000007, 0x00000301, 0x000001CC, 0x00000300, 0x00050083, 0x00000007, 
0x00000302, 0x000002FF, 0x00000301, 0x000200FE, 0x00000302, 0x00010038, 0x00050036, 0x00000009, 0x0000006A, 0x00000000, 0x00000067, 0x00030037, 
0x00000026, 0x00000068, 0x00030037, 0x00000015, 0x00000069, 0x000200F8, 0x0000006B, 0x0004003B, 0x00000015, 0x00000305, 0x00000007, 0x0004003B, 
0x00000026, 0x0000030F, 0x00000007, 0x0004003B, 0x00000026, 0x00000310, 0x00000007, 0x0004003B, 0x00000026, 0x0000031A, 0x00000007, 0x0004003B, 
0x00000026, 0x0000031B, 0x00000007, 0x0004003B, 0x00000015, 0x0000031E, 0x00000007, 0x0004003B, 0x00000015, 0x00000320, 0x00000007, 0x0004003B, 
0x00000015, 0x00000321, 0x00000007, 0x0004003B, 0x00000026, 0x00000323, 0x00000007, 0x0004003B, 0x00000015, 0x0000032E, 0x00000007, 0x0004003B, 
0x00000026, 0x00000331, 0x00000007, 0x0004003B, 0x00000015, 0x00000334, 0x00000007, 0x0004003B, 0x00000015, 0x00000335, 0x00000007, 0x0004003B, 
0x0000001A, 0x00000337, 0x00000007, 0x0004003B, 0x0000001A, 0x0000033F, 0x00000007, 0x0004003B, 0x0000001A, 0x00000341, 0x00000007, 0x00040008, 
0x00000001, 0x00000179, 0x00000000, 0x0004003D, 0x00000008, 0x00000306, 0x00000068, 0x00050081, 0x00000008, 0x0000030A, 0x00000306, 0x00000309, 
0x0003003E, 0x0000030F, 0x0000030A, 0x0003003E, 0x00000310, 0x0000030E, 0x00060039, 0x00000007, 0x00000311, 0x00000055, 0x0000030F, 0x00000310, 
0x0003003E, 0x00000305, 0x00000311, 0x00040008, 0x00000001, 0x0000017A, 0x00000000, 0x0004003D, 0x00000008, 0x00000312, 0x00000068, 0x00050081, 
0x00000008, 0x00000316, 0x00000312, 0x00000315, 0x0003003E, 0x0000031A, 0x00000316, 0x0003003E, 0x0000031B, 0x00000319, 0x00060039, 0x00000007, 
0x0000031C, 0x00000055, 0x0000031A, 0x0000031B, 0x0004003D, 0x00000007, 0x0000031F, 0x00000305, 0x0003003E, 0x0000031E, 0x0000031F, 0x0003003E, 
0x00000320, 0x0000031C, 0x0003003E, 0x00000321, 0x0000031D, 0x00070039, 0x00000007, 0x00000322, 0x00000060, 0x0000031E, 0x00000320, 0x00000321, 
0x0003003E, 0x00000305, 0x00000322, 0x00040008, 0x00000001, 0x0000017C, 0x00000000, 0x0004003D, 0x00000008, 0x00000324, 0x00000068, 0x00050081, 
0x00000008, 0x00000327, 0x00000324, 0x00000326, 0x0003003E, 0x00000323, 0x00000327, 0x00040008, 0x00000001, 0x0000017D, 0x00000000, 0x00050041, 
0x00000015, 0x00000328, 0x00000323, 0x00000102, 0x0004003D, 0x00000007, 0x00000329, 0x00000328, 0x0006000C, 0x00000007, 0x0000032A, 0x00000002, 
0x00000004, 0x00000329, 0x00050083, 0x00000007, 0x0000032C, 0x0000032A, 0x0000032B, 0x00050041, 0x00000015, 0x0000032D, 0x00000323, 0x00000102, 
0x0003003E, 0x0000032D, 0x0000032C, 0x00040008, 0x00000001, 0x0000017F, 0x00000000, 0x0004003D, 0x00000008, 0x00000332, 0x00000323, 0x0008004F, 
0x00000008, 0x00000333, 0x00000332, 0x00000332, 0x00000002, 0x00000001, 0x00000000, 0x0003003E, 0x00000331, 0x00000333, 0x0003003E, 0x00000334, 
0x0000032F, 0x0003003E, 0x00000335, 0x00000330, 0x00070039, 0x00000007, 0x00000336, 0x0000005B, 0x00000331, 0x00000334, 0x00000335, 0x0003003E, 
0x0000032E, 0x00000336, 0x00040008, 0x00000001, 0x00000181, 0x00000000, 0x0004003D, 0x00000007, 0x00000338, 0x0000032E, 0x00050050, 0x00000009, 
0x00000339, 0x00000338, 0x00000114, 0x0003003E, 0x00000337, 0x00000339, 0x00040008, 0x00000001, 0x00000182, 0x00000000, 0x0004003D, 0x00000007, 
0x0000033A, 0x00000305, 0x0004003D, 0x00000007, 0x0000033C, 0x00000069, 0x00050081, 0x00000007, 0x0000033D, 0x0000033B, 0x0000033C, 0x00050050, 
0x00000009, 0x0000033E, 0x0000033A, 0x0000033D, 0x0004003D, 0x00000009, 0x00000340, 0x00000337, 0x0003003E, 0x0000033F, 0x00000340, 0x0003003E, 
0x00000341, 0x0000033E, 0x00060039, 0x00000009, 0x00000342, 0x00000050, 0x0000033F, 0x00000341, 0x0003003E, 0x00000337, 0x00000342, 0x00040008, 
0x00000001, 0x00000183, 0x00000000, 0x0004003D, 0x00000009, 0x00000343, 0x00000337, 0x000200FE, 0x00000343, 0x00010038, 0x00050036, 0x00000009, 
0x0000006F, 0x00000000, 0x0000006C, 0x00030037, 0x00000026, 0x0000006D, 0x00030037, 0x0000001A, 0x0000006E, 0x000200F8, 0x00000070, 0x0004003B, 
0x00000083, 0x00000346, 0x00000007, 0x0004003B, 0x0000001A, 0x00000347, 0x00000007, 0x0004003B, 0x0000001A, 0x0000034C, 0x00000007, 0x0004003B, 
0x0000001A, 0x00000352, 0x00000007, 0x0004003B, 0x0000001A, 0x00000353, 0x00000007, 0x0004003B, 0x00000015, 0x00000357, 0x00000007, 0x0004003B, 
0x00000026, 0x00000362, 0x00000007, 0x0004003B, 0x00000015, 0x00000366, 0x00000007, 0x0004003B, 0x00000015, 0x0000036E, 0x00000007, 0x0004003B, 
0x00000015, 0x00000372, 0x00000007, 0x0004003B, 0x00000015, 0x00000385, 0x00000007, 0x0004003B, 0x00000026, 0x0000038A, 0x00000007, 0x0004003B, 
0x00000026, 0x0000038C, 0x00000007, 0x0004003B, 0x00000015, 0x00000392, 0x00000007, 0x0004003B, 0x00000026, 0x000003B0, 0x00000007, 0x0004003B, 
0x00000026, 0x000003B1, 0x00000007, 0x0004003B, 0x00000026, 0x000003C1, 0x00000007, 0x0004003B, 0x00000026, 0x000003C2, 0x00000007, 0x0004003B, 
0x00000026, 0x000003D6, 0x00000007, 0x0004003B, 0x00000026, 0x000003D7, 0x00000007, 0x0004003B, 0x00000015, 0x000003DA, 0x00000007, 0x0004003B, 
0x00000026, 0x000003F0, 0x00000007, 0x0004003B, 0x00000026, 0x000003F1, 0x00000007, 0x0004003B, 0x00000026, 0x00000401, 0x00000007, 0x0004003B, 
0x00000015, 0x00000403, 0x00000007, 0x0004003B, 0x00000015, 0x00000404, 0x00000007, 0x0004003B, 0x00000026, 0x00000429, 0x00000007, 0x0004003B, 
0x00000026, 0x0000042A, 0x00000007, 0x0004003B, 0x00000026, 0x0000042D, 0x00000007, 0x0004003B, 0x00000015, 0x0000043E, 0x00000007, 0x0004003B, 
0x00000026, 0x0000045F, 0x00000007, 0x0004003B, 0x00000026, 0x00000461, 0x00000007, 0x0004003B, 0x0000001A, 0x00000474, 0x00000007, 0x0004003B, 
0x00000026, 0x00000479, 0x00000007, 0x0004003B, 0x00000026, 0x0000047B, 0x00000007, 0x0004003B, 0x0000001A, 0x0000047E, 0x00000007, 0x0004003B, 
0x0000001A, 0x00000480, 0x00000007, 0x00040008, 0x00000001, 0x0000018A, 0x00000000, 0x0004003D, 0x00000009, 0x00000348, 0x0000006E, 0x0003003E, 
0x00000347, 0x00000348, 0x00050039, 0x00000009, 0x00000349, 0x00000021, 0x00000347, 0x0004003D, 0x00000082, 0x0000034A, 0x00000346, 0x0009004F, 
0x00000082, 0x0000034B, 0x0000034A, 0x00000349, 0x00000004, 0x00000005, 0x00000002, 0x00000003, 0x0003003E, 0x00000346, 0x0000034B, 0x00040008, 
0x00000001, 0x0000018B, 0x00000000, 0x0004003D, 0x00000082, 0x0000034D, 0x00000346, 0x0007004F, 0x00000009, 0x0000034E, 0x0000034D, 0x0000034D, 
0x00000000, 0x00000001, 0x0003003E, 0x0000034C, 0x0000034E, 0x00050039, 0x00000009, 0x0000034F, 0x00000021, 0x0000034C, 0x0004003D, 0x00000082, 
0x00000350, 0x00000346, 0x0009004F, 0x00000082, 0x00000351, 0x00000350, 0x0000034F, 0x00000000, 0x00000001, 0x00000004, 0x00000005, 0x0003003E, 
0x00000346, 0x00000351, 0x00040008, 0x00000001, 0x0000018D, 0x00000000, 0x0004003D, 0x00000082, 0x00000354, 0x00000346, 0x0007004F, 0x00000009, 
0x00000355, 0x00000354, 0x00000354, 0x00000002, 0x00000003, 0x0003003E, 0x00000353, 0x00000355, 0x00050039, 0x00000009, 0x00000356, 0x00000021, 
0x00000353, 0x0003003E, 0x00000352, 0x00000356, 0x00040008, 0x00000001, 0x0000018F, 0x00000000, 0x00050041, 0x00000015, 0x00000358, 0x00000346, 
0x000000A9, 0x0004003D, 0x00000007, 0x00000359, 0x00000358, 0x00050085, 0x00000007, 0x0000035A, 0x00000359, 0x00000285, 0x00050081, 0x00000007, 
0x0000035B, 0x0000022E, 0x0000035A, 0x0003003E, 0x00000357, 0x0000035B, 0x00040008, 0x00000001, 0x00000190, 0x00000000, 0x0004003D, 0x00000007, 
0x0000035C, 0x00000357, 0x00050085, 0x00000007, 0x0000035E, 0x0000035C, 0x0000035D, 0x00050081, 0x00000007, 0x0000035F, 0x0000035E, 0x000001CC, 
0x0006000C, 0x00000007, 0x00000360, 0x00000002, 0x00000008, 0x0000035F, 0x00050088, 0x00000007, 0x00000361, 0x00000360, 0x0000035D, 0x0003003E, 
0x00000357, 0x00000361, 0x00040008, 0x00000001, 0x00000192, 0x00000000, 0x0004003D, 0x00000008, 0x00000363, 0x0000006D, 0x00050083, 0x00000008, 
0x00000365, 0x00000363, 0x00000364, 0x0003003E, 0x00000362, 0x00000365, 0x00040008, 0x00000001, 0x00000194, 0x00000000, 0x00050041, 0x00000015, 
0x00000368, 0x00000346, 0x00000367, 0x0004003D, 0x00000007, 0x00000369, 0x00000368, 0x00050041, 0x00000015, 0x0000036A, 0x00000346, 0x00000102, 
0x0004003D, 0x00000007, 0x0000036B, 0x0000036A, 0x00050085, 0x00000007, 0x0000036C, 0x00000369, 0x0000036B, 0x00050081, 0x00000007, 0x0000036D, 
0x0000036C, 0x00000285, 0x0003003E, 0x00000366, 0x0000036D, 0x00040008, 0x00000001, 0x00000196, 0x00000000, 0x0004003D, 0x00000009, 0x0000036F, 
0x0000006E, 0x0006000C, 0x00000007, 0x00000370, 0x00000002, 0x00000042, 0x0000036F, 0x00050088, 0x00000007, 0x00000371, 0x00000214, 0x00000370, 
0x0003003E, 0x00000372, 0x00000371, 0x00050039, 0x00000007, 0x00000373, 0x0000003C, 0x00000372, 0x0003003E, 0x0000036E, 0x00000373, 0x00040008, 
0x00000001, 0x00000197, 0x00000000, 0x0004003D, 0x00000007, 0x00000374, 0x0000036E, 0x0004003D, 0x00000007, 0x00000375, 0x00000366, 0x00050085, 
0x00000007, 0x00000376, 0x00000375, 0x00000374, 0x0003003E, 0x00000366, 0x00000376, 0x00040008, 0x00000001, 0x00000198, 0x00000000, 0x0004003D, 
0x00000007, 0x00000378, 0x00000357, 0x00050083, 0x00000007, 0x0000037A, 0x00000378, 0x00000379, 0x00050085, 0x00000007, 0x0000037B, 0x0000037A, 
0x0000035D, 0x00050081, 0x00000007, 0x0000037C, 0x00000377, 0x0000037B, 0x0004003D, 0x00000007, 0x0000037D, 0x00000366, 0x00050085, 0x00000007, 
0x0000037E, 0x0000037D, 0x0000037C, 0x0003003E, 0x00000366, 0x0000037E, 0x00040008, 0x00000001, 0x00000199, 0x00000000, 0x0004003D, 0x00000007, 
0x0000037F, 0x00000366, 0x00050081, 0x00000007, 0x00000380, 0x0000037F, 0x00000285, 0x0003003E, 0x00000366, 0x00000380, 0x00040008, 0x00000001, 
0x0000019A, 0x00000000, 0x0004003D, 0x00000007, 0x00000381, 0x00000366, 0x00050085, 0x00000007, 0x00000382, 0x00000381, 0x0000035D, 0x0006000C, 
0x00000007, 0x00000383, 0x00000002, 0x00000008, 0x00000382, 0x00050085, 0x00000007, 0x00000384, 0x00000383, 0x000001D1, 0x0003003E, 0x00000366, 
0x00000384, 0x00040008, 0x00000001, 0x0000019C, 0x00000000, 0x0004003D, 0x00000007, 0x00000386, 0x00000357, 0x0004003D, 0x00000007, 0x00000387, 
0x00000366, 0x0004003D, 0x00000007, 0x00000388, 0x00000357, 0x00060050, 0x00000008, 0x00000389, 0x00000386, 0x00000387, 0x00000388, 0x0004003D, 
0x00000008, 0x0000038B, 0x00000362, 0x0003003E, 0x0000038A, 0x0000038B, 0x0003003E, 0x0000038C, 0x00000389, 0x00060039, 0x00000007, 0x0000038D, 
0x00000055, 0x0000038A, 0x0000038C, 0x0003003E, 0x00000385, 0x0000038D, 0x00040008, 0x00000001, 0x0000019D, 0x00000000, 0x0004003D, 0x00000007, 
0x0000038E, 0x00000385, 0x00050041, 0x00000015, 0x0000038F, 0x0000006D, 0x00000098, 0x0004003D, 0x00000007, 0x00000390, 0x0000038F, 0x0007000C, 
0x00000007, 0x00000391, 0x00000002, 0x00000025, 0x0000038E, 0x00000390, 0x0003003E, 0x00000385, 0x00000391, 0x00040008, 0x00000001, 0x0000019F, 
0x00000000, 0x00050041, 0x00000015, 0x00000393, 0x00000346, 0x00000098, 0x0004003D, 0x00000007, 0x00000394, 0x00000393, 0x00050085, 0x00000007, 
0x00000395, 0x00000394, 0x00000115, 0x00050083, 0x00000007, 0x00000396, 0x00000395, 0x00000129, 0x0007000C, 0x00000007, 0x00000397, 0x00000002, 
0x00000028, 0x0000012A, 0x00000396, 0x0004003D, 0x00000007, 0x00000398, 0x0000036E, 0x00050085, 0x00000007, 0x00000399, 0x00000397, 0x00000398, 
0x0003003E, 0x00000392, 0x00000399, 0x00040008, 0x00000001, 0x000001A0, 0x00000000, 0x0004003D, 0x00000007, 0x0000039A, 0x00000392, 0x00050085, 
0x00000007, 0x0000039B, 0x0000039A, 0x0000035D, 0x0006000C, 0x00000007, 0x0000039C, 0x00000002, 0x00000008, 0x0000039B, 0x00050085, 0x00000007, 
0x0000039D, 0x0000039C, 0x000001D1, 0x0003003E, 0x00000392, 0x0000039D, 0x00040008, 0x00000001, 0x000001A1, 0x00000000, 0x0004003D, 0x00000009, 
0x0000039E, 0x00000352, 0x0005008E, 0x00000009, 0x0000039F, 0x0000039E, 0x0000035D, 0x0006000C, 0x00000009, 0x000003A0, 0x00000002, 0x00000008, 
0x0000039F, 0x0005008E, 0x00000009, 0x000003A1, 0x000003A0, 0x000001D1, 0x0003003E, 0x00000352, 0x000003A1, 0x00040008, 0x00000001, 0x000001A2, 
0x00000000, 0x0004003D, 0x00000007, 0x000003A2, 0x00000385, 0x0004003D, 0x00000008, 0x000003A3, 0x00000362, 0x0004003D, 0x00000007, 0x000003A4, 
0x00000366, 0x00060050, 0x00000008, 0x000003A5, 0x0000012A, 0x000003A4, 0x0000012A, 0x00050083, 0x00000008, 0x000003A6, 0x000003A3, 0x000003A5, 
0x0004003D, 0x00000007, 0x000003A7, 0x00000357, 0x0004003D, 0x00000007, 0x000003A8, 0x00000392, 0x00050041, 0x00000015, 0x000003A9, 0x00000352, 
0x00000098, 0x0004003D, 0x00000007, 0x000003AA, 0x000003A9, 0x00050083, 0x00000007, 0x000003AB, 0x000003A8, 0x000003AA, 0x0004003D, 0x00000007, 
0x000003AC, 0x00000357, 0x00050085, 0x00000007, 0x000003AE, 0x000003AC, 0x000003AD, 0x00060050, 0x00000008, 0x000003AF, 0x000003A7, 0x000003AB, 
0x000003AE, 0x0003003E, 0x000003B0, 0x000003A6, 0x0003003E, 0x000003B1, 0x000003AF, 0x00060039, 0x00000007, 0x000003B2, 0x00000055, 0x000003B0, 
0x000003B1, 0x0007000C, 0x00000007, 0x000003B3, 0x00000002, 0x00000025, 0x000003A2, 0x000003B2, 0x0003003E, 0x00000385, 0x000003B3, 0x00040008, 
0x00000001, 0x000001A3, 0x00000000, 0x0004003D, 0x00000007, 0x000003B4, 0x00000385, 0x0004003D, 0x00000008, 0x000003B5, 0x00000362, 0x0004003D, 
0x00000007, 0x000003B6, 0x00000366, 0x00060050, 0x00000008, 0x000003B7, 0x0000012A, 0x000003B6, 0x0000012A, 0x00050083, 0x00000008, 0x000003B8, 
0x000003B5, 0x000003B7, 0x0004003D, 0x00000007, 0x000003B9, 0x00000357, 0x00050085, 0x00000007, 0x000003BA, 0x000003B9, 0x000003AD, 0x0004003D, 
0x00000007, 0x000003BB, 0x00000392, 0x00050041, 0x00000015, 0x000003BC, 0x00000352, 0x000000A9, 0x0004003D, 0x00000007, 0x000003BD, 0x000003BC, 
0x00050083, 0x00000007, 0x000003BE, 0x000003BB, 0x000003BD, 0x0004003D, 0x00000007, 0x000003BF, 0x00000357, 0x00060050, 0x00000008, 0x000003C0, 
0x000003BA, 0x000003BE, 0x000003BF, 0x0003003E, 0x000003C1, 0x000003B8, 0x0003003E, 0x000003C2, 0x000003C0, 0x00060039, 0x00000007, 0x000003C3, 
0x00000055, 0x000003C1, 0x000003C2, 0x0007000C, 0x00000007, 0x000003C4, 0x00000002, 0x00000025, 0x000003B4, 0x000003C3, 0x0003003E, 0x00000385, 
0x000003C4, 0x00040008, 0x00000001, 0x000001A4, 0x00000000, 0x00050041, 0x00000015, 0x000003C5, 0x00000352, 0x00000098, 0x0004003D, 0x00000007, 
0x000003C6, 0x000003C5, 0x000500BA, 0x000002C0, 0x000003C7, 0x000003C6, 0x00000218, 0x000300F7, 0x000003C9, 0x00000000, 0x000400FA, 0x000003C7, 
0x000003C8, 0x000003F5, 0x000200F8, 0x000003C8, 0x00040008, 0x00000001, 0x000001A6, 0x00000000, 0x0004003D, 0x00000007, 0x000003CA, 0x00000385, 
0x0004003D, 0x00000008, 0x000003CB, 0x00000362, 0x0004003D, 0x00000007, 0x000003CC, 0x00000366, 0x00060050, 0x00000008, 0x000003CD, 0x0000012A, 
0x000003CC, 0x0000012A, 0x00050083, 0x00000008, 0x000003CE, 0x000003CB, 0x000003CD, 0x0004003D, 0x00000007, 0x000003CF, 0x00000357, 0x00050085, 
0x00000007, 0x000003D1, 0x000003CF, 0x000003D0, 0x0004003D, 0x00000007, 0x000003D2, 0x00000392, 0x0004003D, 0x00000007, 0x000003D3, 0x00000357, 
0x00050085, 0x00000007, 0x000003D4, 0x000003D3, 0x000003D0, 0x00060050, 0x00000008, 0x000003D5, 0x000003D1, 0x000003D2, 0x000003D4, 0x0003003E, 
0x000003D6, 0x000003CE, 0x0003003E, 0x000003D7, 0x000003D5, 0x00060039, 0x00000007, 0x000003D8, 0x00000055, 0x000003D6, 0x000003D7, 0x0007000C, 
0x00000007, 0x000003D9, 0x00000002, 0x00000025, 0x000003CA, 0x000003D8, 0x0003003E, 0x00000385, 0x000003D9, 0x00040008, 0x00000001, 0x000001A8, 
0x00000000, 0x0004003D, 0x00000007, 0x000003DB, 0x00000357, 0x0003003E, 0x000003DA, 0x000003DB, 0x00040008, 0x00000001, 0x000001A9, 0x00000000, 
0x0004003D, 0x00000007, 0x000003DC, 0x00000392, 0x000500BA, 0x000002C0, 0x000003DD, 0x000003DC, 0x0000012A, 0x000300F7, 0x000003DF, 0x00000000, 
0x000400FA, 0x000003DD, 0x000003DE, 0x000003DF, 0x000200F8, 0x000003DE, 0x00040008, 0x00000001, 0x000001AB, 0x00000000, 0x0004003D, 0x00000007, 
0x000003E0, 0x00000357, 0x00050085, 0x00000007, 0x000003E1, 0x000003E0, 0x000003D0, 0x0003003E, 0x000003DA, 0x000003E1, 0x000200F9, 0x000003DF, 
0x000200F8, 0x000003DF, 0x00040008, 0x00000001, 0x000001AE, 0x00000000, 0x0004003D, 0x00000007, 0x000003E2, 0x00000385, 0x0004003D, 0x00000008, 
0x000003E3, 0x00000362, 0x0004003D, 0x00000007, 0x000003E4, 0x00000366, 0x0004003D, 0x00000007, 0x000003E5, 0x00000392, 0x00050081, 0x00000007, 
0x000003E6, 0x000003E4, 0x000003E5, 0x00060050, 0x00000008, 0x000003E7, 0x0000012A, 0x000003E6, 0x0000012A, 0x00050083, 0x00000008, 0x000003E8, 
0x000003E3, 0x000003E7, 0x0004003D, 0x00000007, 0x000003E9, 0x000003DA, 0x00050083, 0x00000007, 0x000003EB, 0x000003E9, 0x000003EA, 0x0004003D, 
0x00000007, 0x000003ED, 0x000003DA, 0x00050083, 0x00000007, 0x000003EE, 0x000003ED, 0x000003EA, 0x00060050, 0x00000008, 0x000003EF, 0x000003EB, 
0x000003EC, 0x000003EE, 0x0003003E, 0x000003F0, 0x000003E8, 0x0003003E, 0x000003F1, 0x000003EF, 0x00060039, 0x00000007, 0x000003F2, 0x00000055, 
0x000003F0, 0x000003F1, 0x0004007F, 0x00000007, 0x000003F3, 0x000003F2, 0x0007000C, 0x00000007, 0x000003F4, 0x00000002, 0x00000028, 0x000003E2, 
0x000003F3, 0x0003003E, 0x00000385, 0x000003F4, 0x000200F9, 0x000003C9, 0x000200F8, 0x000003F5, 0x00040008, 0x00000001, 0x000001B3, 0x00000000, 
0x0004003D, 0x00000007, 0x000003F6, 0x00000392, 0x000500BA, 0x000002C0, 0x000003F7, 0x000003F6, 0x0000012A, 0x000300F7, 0x000003F9, 0x00000000, 
0x000400FA, 0x000003F7, 0x000003F8, 0x000003F9, 0x000200F8, 0x000003F8, 0x00040008, 0x00000001, 0x000001B5, 0x00000000, 0x0004003D, 0x00000007, 
0x000003FA, 0x00000385, 0x0004003D, 0x00000008, 0x000003FB, 0x00000362, 0x0004003D, 0x00000007, 0x000003FC, 0x00000366, 0x00060050, 0x00000008, 
0x000003FD, 0x0000012A, 0x000003FC, 0x0000012A, 0x00050083, 0x00000008, 0x000003FE, 0x000003FB, 0x000003FD, 0x0004003D, 0x00000007, 0x000003FF, 
0x00000357, 0x00050085, 0x00000007, 0x00000400, 0x000003FF, 0x000003D0, 0x0008004F, 0x00000008, 0x00000402, 0x000003FE, 0x000003FE, 0x00000000, 
0x00000002, 0x00000001, 0x0003003E, 0x00000401, 0x00000402, 0x0003003E, 0x00000403, 0x00000400, 0x0004003D, 0x00000007, 0x00000405, 0x00000392, 
0x0003003E, 0x00000404, 0x00000405, 0x00070039, 0x00000007, 0x00000406, 0x0000005B, 0x00000401, 0x00000403, 0x00000404, 0x0007000C, 0x00000007, 
0x00000407, 0x00000002, 0x00000025, 0x000003FA, 0x00000406, 0x0003003E, 0x00000385, 0x00000407, 0x000200F9, 0x000003F9, 0x000200F8, 0x000003F9, 
0x000200F9, 0x000003C9, 0x000200F8, 0x000003C9, 0x00040008, 0x00000001, 0x000001BB, 0x00000000, 0x0004003D, 0x00000007, 0x00000408, 0x00000385, 
0x0004003D, 0x00000008, 0x00000409, 0x00000362, 0x00050041, 0x00000015, 0x0000040A, 0x00000346, 0x000000A9, 0x0004003D, 0x00000007, 0x0000040B, 
0x0000040A, 0x00050083, 0x00000007, 0x0000040C, 0x0000040B, 0x000001CC, 0x0004003D, 0x00000007, 0x0000040D, 0x00000357, 0x00050085, 0x00000007, 
0x0000040E, 0x0000040C, 0x0000040D, 0x0004003D, 0x00000007, 0x0000040F, 0x00000366, 0x0004003D, 0x00000007, 0x00000410, 0x00000392, 0x00050081, 
0x00000007, 0x00000411, 0x0000040F, 0x00000410, 0x00050041, 0x00000015, 0x00000412, 0x00000346, 0x00000098, 0x0004003D, 0x00000007, 0x00000413, 
0x00000412, 0x00050083, 0x00000007, 0x00000414, 0x00000413, 0x000001CC, 0x0004003D, 0x00000007, 0x00000415, 0x00000357, 0x00050085, 0x00000007, 
0x00000416, 0x00000414, 0x00000415, 0x00060050, 0x00000008, 0x00000417, 0x0000040E, 0x00000411, 0x00000416, 0x00050083, 0x00000008, 0x00000418, 
0x00000409, 0x00000417, 0x0004003D, 0x00000007, 0x00000419, 0x00000357, 0x00050085, 0x00000007, 0x0000041B, 0x00000419, 0x0000041A, 0x00050041, 
0x00000015, 0x0000041C, 0x00000346, 0x00000102, 0x0004003D, 0x00000007, 0x0000041D, 0x0000041C, 0x00050085, 0x00000007, 0x0000041E, 0x0000041B, 
0x0000041D, 0x00050041, 0x00000015, 0x0000041F, 0x00000352, 0x00000098, 0x0004003D, 0x00000007, 0x00000420, 0x0000041F, 0x00050085, 0x00000007, 
0x00000421, 0x00000285, 0x00000420, 0x0004003D, 0x00000007, 0x00000422, 0x00000357, 0x00050085, 0x00000007, 0x00000423, 0x00000422, 0x0000041A, 
0x00050041, 0x00000015, 0x00000424, 0x00000352, 0x000000A9, 0x0004003D, 0x00000007, 0x00000425, 0x00000424, 0x00050085, 0x00000007, 0x00000426, 
0x00000423, 0x00000425, 0x00050081, 0x00000007, 0x00000427, 0x00000426, 0x000001E5, 0x00060050, 0x00000008, 0x00000428, 0x0000041E, 0x00000421, 
0x00000427, 0x0003003E, 0x00000429, 0x00000418, 0x0003003E, 0x0000042A, 0x00000428, 0x00060039, 0x00000007, 0x0000042B, 0x00000055, 0x00000429, 
0x0000042A, 0x0007000C, 0x00000007, 0x0000042C, 0x00000002, 0x00000025, 0x00000408, 0x0000042B, 0x0003003E, 0x00000385, 0x0000042C, 0x00040008, 
0x00000001, 0x000001BD, 0x00000000, 0x0004003D, 0x00000008, 0x0000042E, 0x00000362, 0x00050041, 0x00000015, 0x0000042F, 0x00000352, 0x000000A9, 
0x0004003D, 0x00000007, 0x00000430, 0x0000042F, 0x00050083, 0x00000007, 0x00000431, 0x00000430, 0x000001CC, 0x0004003D, 0x00000007, 0x00000432, 
0x00000357, 0x00050085, 0x00000007, 0x00000433, 0x00000431, 0x00000432, 0x0004003D, 0x00000007, 0x00000434, 0x00000366, 0x0004003D, 0x00000007, 
0x00000435, 0x00000392, 0x00050081, 0x00000007, 0x00000436, 0x00000434, 0x00000435, 0x00050041, 0x00000015, 0x00000437, 0x00000352, 0x00000098, 
0x0004003D, 0x00000007, 0x00000438, 0x00000437, 0x00050083, 0x00000007, 0x00000439, 0x00000438, 0x000001CC, 0x0004003D, 0x00000007, 0x0000043A, 
0x00000357, 0x00050085, 0x00000007, 0x0000043B, 0x00000439, 0x0000043A, 0x00060050, 0x00000008, 0x0000043C, 0x00000433, 0x00000436, 0x0000043B, 
0x00050083, 0x00000008, 0x0000043D, 0x0000042E, 0x0000043C, 0x0003003E, 0x0000042D, 0x0000043D, 0x00040008, 0x00000001, 0x000001BF, 0x00000000, 
0x00050041, 0x00000015, 0x0000043F, 0x0000042D, 0x000000A9, 0x0004003D, 0x00000007, 0x00000440, 0x0000043F, 0x0006000C, 0x00000007, 0x00000441, 
0x00000002, 0x00000006, 0x00000440, 0x0003003E, 0x0000043E, 0x00000441, 0x00040008, 0x00000001, 0x000001C0, 0x00000000, 0x00050041, 0x00000015, 
0x00000442, 0x0000042D, 0x000000A9, 0x0004003D, 0x00000007, 0x00000443, 0x00000442, 0x0006000C, 0x00000007, 0x00000444, 0x00000002, 0x00000004, 
0x00000443, 0x00050083, 0x00000007, 0x00000445, 0x00000444, 0x0000032B, 0x0004003D, 0x00000007, 0x00000446, 0x00000357, 0x00050085, 0x00000007, 
0x00000447, 0x00000446, 0x0000041A, 0x00050041, 0x00000015, 0x00000448, 0x00000346, 0x00000367, 0x0004003D, 0x00000007, 0x00000449, 0x00000448, 
0x00050085, 0x00000007, 0x0000044A, 0x00000447, 0x00000449, 0x00050083, 0x00000007, 0x0000044B, 0x00000445, 0x0000044A, 0x00050041, 0x00000015, 
0x0000044C, 0x0000042D, 0x000000A9, 0x0003003E, 0x0000044C, 0x0000044B, 0x00040008, 0x00000001, 0x000001C1, 0x00000000, 0x0004003D, 0x00000007, 
0x0000044D, 0x00000385, 0x0004003D, 0x00000007, 0x0000044E, 0x00000357, 0x00050085, 0x00000007, 0x0000044F, 0x0000044E, 0x0000041A, 0x00050041, 
0x00000015, 0x00000450, 0x00000346, 0x00000367, 0x0004003D, 0x00000007, 0x00000451, 0x00000450, 0x00050085, 0x00000007, 0x00000452, 0x0000044F, 
0x00000451, 0x00050041, 0x00000015, 0x00000453, 0x00000346, 0x00000098, 0x0004003D, 0x00000007, 0x00000454, 0x00000453, 0x00050085, 0x00000007, 
0x00000455, 0x000002B2, 0x00000454, 0x0004003D, 0x00000007, 0x00000456, 0x00000357, 0x00050085, 0x00000007, 0x00000457, 0x00000456, 0x0000022E, 
0x00050041, 0x00000015, 0x00000458, 0x00000346, 0x000000A9, 0x0004003D, 0x00000007, 0x00000459, 0x00000458, 0x00050085, 0x00000007, 0x0000045A, 
0x00000457, 0x00000459, 0x0004003D, 0x00000007, 0x0000045B, 0x0000043E, 0x00050085, 0x00000007, 0x0000045C, 0x0000045B, 0x000001E5, 0x00050081, 
0x00000007, 0x0000045D, 0x0000045A, 0x0000045C, 0x00060050, 0x00000008, 0x0000045E, 0x00000452, 0x00000455, 0x0000045D, 0x0004003D, 0x00000008, 
0x00000460, 0x0000042D, 0x0003003E, 0x0000045F, 0x00000460, 0x0003003E, 0x00000461, 0x0000045E, 0x00060039, 0x00000007, 0x00000462, 0x00000055, 
0x0000045F, 0x00000461, 0x0007000C, 0x00000007, 0x00000463, 0x00000002, 0x00000025, 0x0000044D, 0x00000462, 0x0003003E, 0x00000385, 0x00000463, 
0x00040008, 0x00000001, 0x000001C2, 0x00000000, 0x00050041, 0x00000015, 0x00000464, 0x00000346, 0x00000098, 0x0004003D, 0x00000007, 0x00000465, 
0x00000464, 0x000500B8, 0x000002C0, 0x00000467, 0x00000465, 0x00000466, 0x000300F7, 0x00000469, 0x00000000, 0x000400FA, 0x00000467, 0x00000468, 
0x00000469, 0x000200F8, 0x00000468, 0x00040008, 0x00000001, 0x000001C4, 0x00000000, 0x0004003D, 0x00000007, 0x0000046A, 0x00000385, 0x0004003D, 
0x00000008, 0x0000046B, 0x00000362, 0x0004003D, 0x00000007, 0x0000046C, 0x00000366, 0x00060050, 0x00000008, 0x0000046D, 0x0000012A, 0x0000046C, 
0x0000012A, 0x00050083, 0x00000008, 0x0000046E, 0x0000046B, 0x0000046D, 0x0006000C, 0x00000007, 0x0000046F, 0x00000002, 0x00000042, 0x0000046E, 
0x0004003D, 0x00000007, 0x00000470, 0x00000357, 0x00050085, 0x00000007, 0x00000471, 0x00000470, 0x000003D0, 0x00050083, 0x00000007, 0x00000472, 
0x0000046F, 0x00000471, 0x0007000C, 0x00000007, 0x00000473, 0x00000002, 0x00000025, 0x0000046A, 0x00000472, 0x0003003E, 0x00000385, 0x00000473, 
0x000200F9, 0x00000469, 0x000200F8, 0x00000469, 0x00040008, 0x00000001, 0x000001C9, 0x00000000, 0x0004003D, 0x00000007, 0x00000475, 0x00000385, 
0x00050050, 0x00000009, 0x00000476, 0x00000475, 0x0000012A, 0x0003003E, 0x00000474, 0x00000476, 0x00040008, 0x00000001, 0x000001CA, 0x00000000, 
0x0004003D, 0x00000008, 0x0000047A, 0x00000362, 0x0003003E, 0x00000479, 0x0000047A, 0x0003003E, 0x0000047B, 0x00000478, 0x00060039, 0x00000007, 
0x0000047C, 0x00000055, 0x00000479, 0x0000047B, 0x00050050, 0x00000009, 0x0000047D, 0x0000047C, 0x00000129, 0x0004003D, 0x00000009, 0x0000047F, 
0x00000474, 0x0003003E, 0x0000047E, 0x0000047F, 0x0003003E, 0x00000480, 0x0000047D, 0x00060039, 0x00000009, 0x00000481, 0x00000050, 0x0000047E, 
0x00000480, 0x0003003E, 0x00000474, 0x00000481, 0x00040008, 0x00000001, 0x000001CB, 0x00000000, 0x0004003D, 0x00000009, 0x00000482, 0x00000474, 
0x000200FE, 0x00000482, 0x00010038, 0x00050036, 0x00000009, 0x00000073, 0x00000000, 0x00000071, 0x00030037, 0x00000026, 0x00000072, 0x000200F8, 
0x00000074, 0x0004003B, 0x00000026, 0x00000485, 0x00000007, 0x0004003B, 0x0000001A, 0x0000048C, 0x00000007, 0x0004003B, 0x00000026, 0x00000490, 
0x00000007, 0x0004003B, 0x0000001A, 0x00000492, 0x00000007, 0x0004003B, 0x00000026, 0x00000494, 0x00000007, 0x0004003B, 0x00000015, 0x00000497, 
0x00000007, 0x0004003B, 0x00000015, 0x0000049C, 0x00000007, 0x0004003B, 0x00000015, 0x0000049D, 0x00000007, 0x0004003B, 0x00000015, 0x000004BF, 
0x00000007, 0x0004003B, 0x00000015, 0x000004C0, 0x00000007, 0x0004003B, 0x00000015, 0x000004DA, 0x00000007, 0x0004003B, 0x00000015, 0x000004E0, 
0x00000007, 0x0004003B, 0x00000015, 0x000004E3, 0x00000007, 0x0004003B, 0x00000015, 0x000004E7, 0x00000007, 0x0004003B, 0x00000015, 0x000004F1, 
0x00000007, 0x0004003B, 0x00000015, 0x000004F5, 0x00000007, 0x0004003B, 0x0000001A, 0x00000508, 0x00000007, 0x0004003B, 0x00000026, 0x00000509, 
0x00000007, 0x0004003B, 0x00000015, 0x0000050B, 0x00000007, 0x0004003B, 0x0000001A, 0x0000050E, 0x00000007, 0x0004003B, 0x0000001A, 0x00000510, 
0x00000007, 0x00040008, 0x00000001, 0x000001D2, 0x00000000, 0x0004003D, 0x00000008, 0x00000486, 0x00000072, 0x0003003E, 0x00000485, 0x00000486, 
0x00040008, 0x00000001, 0x000001D3, 0x00000000, 0x0004003D, 0x00000008, 0x00000487, 0x00000072, 0x0007004F, 0x00000009, 0x00000488, 0x00000487, 
0x00000487, 0x00000000, 0x00000002, 0x0006000C, 0x00000009, 0x00000489, 0x00000002, 0x0000000A, 0x00000488, 0x0004003D, 0x00000008, 0x0000048A, 
0x00000485, 0x0008004F, 0x00000008, 0x0000048B, 0x0000048A, 0x00000489, 0x00000003, 0x00000001, 0x00000004, 0x0003003E, 0x00000485, 0x0000048B, 
0x00040008, 0x00000001, 0x000001D5, 0x00000000, 0x0004003D, 0x00000008, 0x0000048D, 0x00000072, 0x0007004F, 0x00000009, 0x0000048E, 0x0000048D, 
0x0000048D, 0x00000000, 0x00000002, 0x0006000C, 0x00000009, 0x0000048F, 0x00000002, 0x00000008, 0x0000048E, 0x0004003D, 0x00000008, 0x00000491, 
0x00000485, 0x0003003E, 0x00000490, 0x00000491, 0x0003003E, 0x00000492, 0x0000048F, 0x00060039, 0x00000009, 0x00000493, 0x0000006F, 0x00000490, 
0x00000492, 0x0003003E, 0x0000048C, 0x00000493, 0x00040008, 0x00000001, 0x000001D7, 0x00000000, 0x0004003D, 0x00000008, 0x00000495, 0x00000072, 
0x0003003E, 0x00000494, 0x00000495, 0x00040008, 0x00000001, 0x000001D8, 0x00000000, 0x0004003D, 0x00000008, 0x00000496, 0x00000494, 0x0003003E, 
0x00000485, 0x00000496, 0x00040008, 0x00000001, 0x000001DA, 0x00000000, 0x0004003D, 0x00000007, 0x0000049A, 0x00000499, 0x00050085, 0x00000007, 
0x0000049B, 0x0000049A, 0x0000022E, 0x0003003E, 0x00000497, 0x0000049B, 0x00040008, 0x00000001, 0x000001DC, 0x00000000, 0x0003003E, 0x0000049C, 
0x00000129, 0x00040008, 0x00000001, 0x000001DE, 0x00000000, 0x0003003E, 0x0000049D, 0x00000218, 0x00040008, 0x00000001, 0x000001DF, 0x00000000, 
0x00050041, 0x00000015, 0x0000049E, 0x00000485, 0x000000A9, 0x0004003D, 0x00000007, 0x0000049F, 0x0000049E, 0x0006000C, 0x00000007, 0x000004A0, 
0x00000002, 0x0000000A, 0x0000049F, 0x00050083, 0x00000007, 0x000004A1, 0x000004A0, 0x000001CC, 0x0006000C, 0x00000007, 0x000004A2, 0x00000002, 
0x00000004, 0x000004A1, 0x000500B8, 0x000002C0, 0x000004A3, 0x000004A2, 0x00000477, 0x000300F7, 0x000004A5, 0x00000000, 0x000400FA, 0x000004A3, 
0x000004A4, 0x000004A5, 0x000200F8, 0x000004A4, 0x00040008, 0x00000001, 0x000001E1, 0x00000000, 0x00050041, 0x00000015, 0x000004A6, 0x00000494, 
0x000000A9, 0x0004003D, 0x00000007, 0x000004A7, 0x000004A6, 0x00050081, 0x00000007, 0x000004A8, 0x000004A7, 0x000001D1, 0x00050041, 0x00000015, 
0x000004A9, 0x00000494, 0x000000A9, 0x0003003E, 0x000004A9, 0x000004A8, 0x00040008, 0x00000001, 0x000001E2, 0x00000000, 0x0004003D, 0x00000008, 
0x000004AA, 0x00000494, 0x0007004F, 0x00000009, 0x000004AB, 0x000004AA, 0x000004AA, 0x00000002, 0x00000000, 0x00050085, 0x00000009, 0x000004AE, 
0x000004AB, 0x000004AD, 0x0004003D, 0x00000008, 0x000004AF, 0x00000494, 0x0008004F, 0x00000008, 0x000004B0, 0x000004AF, 0x000004AE, 0x00000003, 
0x00000001, 0x00000004, 0x0003003E, 0x00000494, 0x000004B0, 0x00040008, 0x00000001, 0x000001E3, 0x00000000, 0x0004003D, 0x00000008, 0x000004B1, 
0x00000494, 0x0007004F, 0x00000009, 0x000004B2, 0x000004B1, 0x000004B1, 0x00000000, 0x00000002, 0x0004003D, 0x00000008, 0x000004B3, 0x00000485, 
0x0008004F, 0x00000008, 0x000004B4, 0x000004B3, 0x000004B2, 0x00000003, 0x00000001, 0x00000004, 0x0003003E, 0x00000485, 0x000004B4, 0x00040008, 
0x00000001, 0x000001E4, 0x00000000, 0x0003003E, 0x0000049C, 0x0000012A, 0x00040008, 0x00000001, 0x000001E5, 0x00000000, 0x0003003E, 0x0000049D, 
0x00000285, 0x000200F9, 0x000004A5, 0x000200F8, 0x000004A5, 0x00040008, 0x00000001, 0x000001E9, 0x00000000, 0x00050041, 0x00000015, 0x000004B5, 
0x00000494, 0x000000A9, 0x0004003D, 0x00000007, 0x000004B6, 0x000004B5, 0x0006000C, 0x00000007, 0x000004B7, 0x00000002, 0x00000008, 0x000004B6, 
0x00050041, 0x00000015, 0x000004B8, 0x00000485, 0x00000102, 0x0004003D, 0x00000007, 0x000004B9, 0x000004B8, 0x00050081, 0x00000007, 0x000004BA, 
0x000004B9, 0x000004B7, 0x00050041, 0x00000015, 0x000004BB, 0x00000485, 0x00000102, 0x0003003E, 0x000004BB, 0x000004BA, 0x00040008, 0x00000001, 
0x000001EA, 0x00000000, 0x00050041, 0x00000015, 0x000004BC, 0x00000494, 0x000000A9, 0x0004003D, 0x00000007, 0x000004BD, 0x000004BC, 0x00050083, 
0x00000007, 0x000004BE, 0x000004BD, 0x000001CC, 0x0003003E, 0x000004BF, 0x000004BE, 0x0003003E, 0x000004C0, 0x00000129, 0x00060039, 0x00000007, 
0x000004C1, 0x00000065, 0x000004BF, 0x000004C0, 0x00050041, 0x00000015, 0x000004C2, 0x00000485, 0x000000A9, 0x0003003E, 0x000004C2, 0x000004C1, 
0x00040008, 0x00000001, 0x000001EB, 0x00000000, 0x00050041, 0x00000015, 0x000004C3, 0x00000485, 0x00000102, 0x0004003D, 0x00000007, 0x000004C4, 
0x000004C3, 0x00050041, 0x00000015, 0x000004C5, 0x00000485, 0x000000A9, 0x0004003D, 0x00000007, 0x000004C6, 0x000004C5, 0x0006000C, 0x00000007, 
0x000004C7, 0x00000002, 0x00000006, 0x000004C6, 0x00050085, 0x00000007, 0x000004C8, 0x000004C4, 0x000004C7, 0x00050041, 0x00000015, 0x000004C9, 
0x00000485, 0x00000102, 0x0003003E, 0x000004C9, 0x000004C8, 0x00040008, 0x00000001, 0x000001EC, 0x00000000, 0x00050041, 0x00000015, 0x000004CA, 
0x00000485, 0x000000A9, 0x0004003D, 0x00000007, 0x000004CB, 0x000004CA, 0x00050041, 0x00000015, 0x000004CC, 0x00000485, 0x000000A9, 0x0004003D, 
0x00000007, 0x000004CD, 0x000004CC, 0x0006000C, 0x00000007, 0x000004CE, 0x00000002, 0x00000006, 0x000004CD, 0x00050085, 0x00000007, 0x000004CF, 
0x000004CB, 0x000004CE, 0x00050083, 0x00000007, 0x000004D1, 0x000004CF, 0x000004D0, 0x00050041, 0x00000015, 0x000004D2, 0x00000485, 0x000000A9, 
0x0003003E, 0x000004D2, 0x000004D1, 0x00040008, 0x00000001, 0x000001ED, 0x00000000, 0x0004003D, 0x00000007, 0x000004D3, 0x00000497, 0x0004003D, 
0x00000007, 0x000004D4, 0x0000049C, 0x00050085, 0x00000007, 0x000004D5, 0x000004D3, 0x000004D4, 0x00050041, 0x00000015, 0x000004D6, 0x00000485, 
0x00000102, 0x0004003D, 0x00000007, 0x000004D7, 0x000004D6, 0x00050083, 0x00000007, 0x000004D8, 0x000004D7, 0x000004D5, 0x00050041, 0x00000015, 
0x000004D9, 0x00000485, 0x00000102, 0x0003003E, 0x000004D9, 0x000004D8, 0x00040008, 0x00000001, 0x000001EF, 0x00000000, 0x00050041, 0x00000015, 
0x000004DB, 0x00000485, 0x00000102, 0x0004003D, 0x00000007, 0x000004DC, 0x000004DB, 0x0004003D, 0x00000007, 0x000004DD, 0x0000049D, 0x00050088, 
0x00000007, 0x000004DE, 0x000004DC, 0x000004DD, 0x0006000C, 0x00000007, 0x000004DF, 0x00000002, 0x00000008, 0x000004DE, 0x0003003E, 0x000004DA, 
0x000004DF, 0x00040008, 0x00000001, 0x000001F0, 0x00000000, 0x00050041, 0x00000015, 0x000004E1, 0x00000485, 0x00000102, 0x0004003D, 0x00000007, 
0x000004E2, 0x000004E1, 0x0003003E, 0x000004E0, 0x000004E2, 0x0004003D, 0x00000007, 0x000004E4, 0x0000049D, 0x0003003E, 0x000004E3, 0x000004E4, 
0x00060039, 0x00000007, 0x000004E5, 0x00000065, 0x000004E0, 0x000004E3, 0x00050041, 0x00000015, 0x000004E6, 0x00000485, 0x00000102, 0x0003003E, 
0x000004E6, 0x000004E5, 0x00040008, 0x00000001, 0x000001F1, 0x00000000, 0x0004003D, 0x00000007, 0x000004E8, 0x000004DA, 0x0003003E, 0x000004E7, 
0x000004E8, 0x00050039, 0x00000007, 0x000004E9, 0x00000018, 0x000004E7, 0x00050085, 0x00000007, 0x000004EB, 0x000004E9, 0x000004EA, 0x00050083, 
0x00000007, 0x000004EC, 0x000004EB, 0x0000030B, 0x00050041, 0x00000015, 0x000004ED, 0x00000485, 0x000000A9, 0x0004003D, 0x00000007, 0x000004EE, 
0x000004ED, 0x00050081, 0x00000007, 0x000004EF, 0x000004EE, 0x000004EC, 0x00050041, 0x00000015, 0x000004F0, 0x00000485, 0x000000A9, 0x0003003E, 
0x000004F0, 0x000004EF, 0x00040008, 0x00000001, 0x000001F3, 0x00000000, 0x0004003D, 0x00000007, 0x000004F2, 0x000004DA, 0x00050085, 0x00000007, 
0x000004F4, 0x000004F2, 0x000004F3, 0x0003003E, 0x000004F5, 0x000004F4, 0x00050039, 0x00000007, 0x000004F6, 0x00000018, 0x000004F5, 0x00050085, 
0x00000007, 0x000004F8, 0x000004F6, 0x000004F7, 0x00050083, 0x00000007, 0x000004F9, 0x000004F8, 0x000004D0, 0x0003003E, 0x000004F1, 0x000004F9, 
0x00040008, 0x00000001, 0x000001F4, 0x00000000, 0x0004003D, 0x00000007, 0x000004FA, 0x00000499, 0x00050085, 0x00000007, 0x000004FB, 0x000004FA, 
0x00000115, 0x0004003D, 0x00000007, 0x000004FC, 0x000004DA, 0x00050081, 0x00000007, 0x000004FD, 0x000004FB, 0x000004FC, 0x0006000C, 0x00000007, 
0x000004FE, 0x00000002, 0x0000000D, 0x000004FD, 0x0004003D, 0x00000007, 0x000004FF, 0x000004F1, 0x00050085, 0x00000007, 0x00000500, 0x000004FF, 
0x000004FE, 0x0003003E, 0x000004F1, 0x00000500, 0x00040008, 0x00000001, 0x000001F5, 0x00000000, 0x0004003D, 0x00000007, 0x00000501, 0x000004F1, 
0x0004003D, 0x00000007, 0x00000502, 0x0000049C, 0x00050085, 0x00000007, 0x00000503, 0x00000501, 0x00000502, 0x00050041, 0x00000015, 0x00000504, 
0x00000485, 0x00000102, 0x0004003D, 0x00000007, 0x00000505, 0x00000504, 0x00050081, 0x00000007, 0x00000506, 0x00000505, 0x00000503, 0x00050041, 
0x00000015, 0x00000507, 0x00000485, 0x00000102, 0x0003003E, 0x00000507, 0x00000506, 0x00040008, 0x00000001, 0x000001F7, 0x00000000, 0x0004003D, 
0x00000008, 0x0000050A, 0x00000485, 0x0003003E, 0x00000509, 0x0000050A, 0x0004003D, 0x00000007, 0x0000050C, 0x000004DA, 0x0003003E, 0x0000050B, 
0x0000050C, 0x00060039, 0x00000009, 0x0000050D, 0x0000006A, 0x00000509, 0x0000050B, 0x0003003E, 0x00000508, 0x0000050D, 0x00040008, 0x00000001, 
0x000001F8, 0x00000000, 0x0004003D, 0x00000009, 0x0000050F, 0x0000048C, 0x0003003E, 0x0000050E, 0x0000050F, 0x0004003D, 0x00000009, 0x00000511, 
0x00000508, 0x0003003E, 0x00000510, 0x00000511, 0x00060039, 0x00000009, 0x00000512, 0x00000050, 0x0000050E, 0x00000510, 0x0003003E, 0x0000048C, 
0x00000512, 0x00040008, 0x00000001, 0x000001F9, 0x00000000, 0x0004003D, 0x00000009, 0x00000513, 0x0000048C, 0x000200FE, 0x00000513, 0x00010038, 
0x00050036, 0x00000003, 0x0000007B, 0x00000000, 0x00000075, 0x00030037, 0x0000001A, 0x00000076, 0x00030037, 0x00000026, 0x00000077, 0x00030037, 
0x00000026, 0x00000078, 0x00030037, 0x00000015, 0x00000079, 0x00030037, 0x00000026, 0x0000007A, 0x000200F8, 0x0000007C, 0x0004003B, 0x00000026, 
0x00000516, 0x00000007, 0x0004003B, 0x0000001A, 0x00000517, 0x00000007, 0x0004003B, 0x0000001A, 0x0000051E, 0x00000007, 0x0004003B, 0x0000001A, 
0x00000524, 0x00000007, 0x0004003B, 0x00000015, 0x0000052E, 0x00000007, 0x0004003B, 0x00000015, 0x0000053F, 0x00000007, 0x0004003B, 0x0000001A, 
0x00000543, 0x00000007, 0x0004003B, 0x0000001A, 0x0000056E, 0x00000007, 0x0004003B, 0x00000015, 0x00000575, 0x00000007, 0x0004003B, 0x00000015, 
0x0000058B, 0x00000007, 0x00040008, 0x00000001, 0x00000200, 0x00000000, 0x0004003D, 0x00000009, 0x00000518, 0x00000076, 0x0003003E, 0x00000517, 
0x00000518, 0x00050039, 0x00000007, 0x00000519, 0x0000001D, 0x00000517, 0x00050085, 0x00000007, 0x0000051A, 0x00000519, 0x000003D0, 0x0004003D, 
0x00000009, 0x0000051B, 0x00000076, 0x0005008E, 0x00000009, 0x0000051D, 0x0000051B, 0x0000051C, 0x0003003E, 0x0000051E, 0x0000051D, 0x00050039, 
0x00000007, 0x0000051F, 0x0000001D, 0x0000051E, 0x00050085, 0x00000007, 0x00000520, 0x0000051F, 0x000003AD, 0x0004003D, 0x00000009, 0x00000521, 
0x00000076, 0x0005008E, 0x00000009, 0x00000523, 0x00000521, 0x00000522, 0x0003003E, 0x00000524, 0x00000523, 0x00050039, 0x00000007, 0x00000525, 
0x0000001D, 0x00000524, 0x00050085, 0x00000007, 0x00000526, 0x00000525, 0x000001CC, 0x00060050, 0x00000008, 0x00000527, 0x0000051A, 0x00000520, 
0x00000526, 0x0003003E, 0x00000516, 0x00000527, 0x00040008, 0x00000001, 0x00000201, 0x00000000, 0x0004003D, 0x00000008, 0x00000528, 0x00000516, 
0x0005008E, 0x00000008, 0x00000529, 0x00000528, 0x000003AD, 0x0004003D, 0x00000008, 0x0000052A, 0x00000078, 0x00050081, 0x00000008, 0x0000052B, 
0x0000052A, 0x00000529, 0x0003003E, 0x00000078, 0x0000052B, 0x00040008, 0x00000001, 0x00000202, 0x00000000, 0x0004003D, 0x00000008, 0x0000052C, 
0x00000078, 0x0005008E, 0x00000008, 0x0000052D, 0x0000052C, 0x0000023D, 0x0003003E, 0x00000078, 0x0000052D, 0x00040008, 0x00000001, 0x00000204, 
0x00000000, 0x0003003E, 0x0000052E, 0x0000012A, 0x00040008, 0x00000001, 0x00000205, 0x00000000, 0x0004003D, 0x00000007, 0x0000052F, 0x0000052E, 
0x00050041, 0x00000015, 0x00000530, 0x00000077, 0x00000098, 0x0004003D, 0x00000007, 0x00000531, 0x00000530, 0x00050085, 0x00000007, 0x00000532, 
0x00000531, 0x0000035D, 0x00050083, 0x00000007, 0x00000533, 0x00000532, 0x00000477, 0x0006000C, 0x00000007, 0x00000534, 0x00000002, 0x0000000A, 
0x00000533, 0x00050085, 0x00000007, 0x00000535, 0x00000534, 0x00000115, 0x00050081, 0x00000007, 0x00000536, 0x00000535, 0x00000285, 0x0006000C, 
0x00000007, 0x00000537, 0x00000002, 0x00000008, 0x00000536, 0x0008000C, 0x00000007, 0x00000538, 0x00000002, 0x0000002E, 0x0000022E, 0x00000129, 
0x00000537, 0x0007000C, 0x00000007, 0x00000539, 0x00000002, 0x00000028, 0x0000052F, 0x00000538, 0x0003003E, 0x0000052E, 0x00000539, 0x00040008, 
0x00000001, 0x00000206, 0x00000000, 0x00050041, 0x00000015, 0x0000053A, 0x00000077, 0x00000098, 0x0004003D, 0x00000007, 0x0000053B, 0x0000053A, 
0x000500B8, 0x000002C0, 0x0000053C, 0x0000053B, 0x000001D1, 0x000300F7, 0x0000053E, 0x00000000, 0x000400FA, 0x0000053C, 0x0000053D, 0x0000053E, 
0x000200F8, 0x0000053D, 0x00040008, 0x00000001, 0x00000208, 0x00000000, 0x0003003E, 0x0000052E, 0x00000129, 0x000200F9, 0x0000053E, 0x000200F8, 
0x0000053E, 0x00040008, 0x00000001, 0x0000020C, 0x00000000, 0x0004003D, 0x00000009, 0x00000540, 0x00000076, 0x0005008E, 0x00000009, 0x00000542, 
0x00000540, 0x00000541, 0x0003003E, 0x00000543, 0x00000542, 0x00050039, 0x00000007, 0x00000544, 0x0000001D, 0x00000543, 0x00050085, 0x00000007, 
0x00000545, 0x00000544, 0x00000115, 0x0003003E, 0x0000053F, 0x00000545, 0x00040008, 0x00000001, 0x0000020D, 0x00000000, 0x0004003D, 0x00000007, 
0x00000546, 0x0000053F, 0x000500B8, 0x000002C0, 0x00000548, 0x00000546, 0x00000547, 0x0004003D, 0x00000007, 0x00000549, 0x0000053F, 0x000500BE, 
0x000002C0, 0x0000054A, 0x00000549, 0x00000129, 0x000500A7, 0x000002C0, 0x0000054B, 0x00000548, 0x0000054A, 0x000300F7, 0x0000054D, 0x00000000, 
0x000400FA, 0x0000054B, 0x0000054C, 0x0000054D, 0x000200F8, 0x0000054C, 0x00040008, 0x00000001, 0x0000020F, 0x00000000, 0x0003003E, 0x0000053F, 
0x00000547, 0x000200F9, 0x0000054D, 0x000200F8, 0x0000054D, 0x00040008, 0x00000001, 0x00000212, 0x00000000, 0x0004003D, 0x00000007, 0x0000054E, 
0x0000052E, 0x00050041, 0x00000015, 0x0000054F, 0x00000077, 0x000000A9, 0x0004003D, 0x00000007, 0x00000550, 0x0000054F, 0x00050085, 0x00000007, 
0x00000552, 0x00000550, 0x00000551, 0x00050081, 0x00000007, 0x00000553, 0x00000552, 0x000001D1, 0x0006000C, 0x00000007, 0x00000554, 0x00000002, 
0x0000000A, 0x00000553, 0x0004003D, 0x00000007, 0x00000555, 0x0000053F, 0x00050085, 0x00000007, 0x00000556, 0x00000554, 0x00000555, 0x0006000C, 
0x00000007, 0x00000557, 0x00000002, 0x00000008, 0x00000556, 0x0008000C, 0x00000007, 0x00000558, 0x00000002, 0x0000002E, 0x0000022E, 0x00000129, 
0x00000557, 0x0007000C, 0x00000007, 0x00000559, 0x00000002, 0x00000028, 0x0000054E, 0x00000558, 0x0003003E, 0x0000052E, 0x00000559, 0x00040008, 
0x00000001, 0x00000213, 0x00000000, 0x0004003D, 0x00000007, 0x0000055A, 0x0000052E, 0x00050041, 0x00000015, 0x0000055B, 0x00000077, 0x00000102, 
0x0004003D, 0x00000007, 0x0000055C, 0x0000055B, 0x00050085, 0x00000007, 0x0000055D, 0x0000055C, 0x00000551, 0x00050081, 0x00000007, 0x0000055E, 
0x0000055D, 0x000001D1, 0x0006000C, 0x00000007, 0x0000055F, 0x00000002, 0x0000000A, 0x0000055E, 0x0004003D, 0x00000007, 0x00000560, 0x0000053F, 
0x00050085, 0x00000007, 0x00000561, 0x0000055F, 0x00000560, 0x0006000C, 0x00000007, 0x00000562, 0x00000002, 0x00000008, 0x00000561, 0x0008000C, 
0x00000007, 0x00000563, 0x00000002, 0x0000002E, 0x0000022E, 0x00000129, 0x00000562, 0x0007000C, 0x00000007, 0x00000564, 0x00000002, 0x00000028, 
0x0000055A, 0x00000563, 0x0003003E, 0x0000052E, 0x00000564, 0x00040008, 0x00000001, 0x00000214, 0x00000000, 0x0004003D, 0x00000007, 0x00000565, 
0x0000052E, 0x000500B8, 0x000002C0, 0x00000566, 0x00000565, 0x000001CC, 0x000300F7, 0x00000568, 0x00000000, 0x000400FA, 0x00000566, 0x00000567, 
0x00000568, 0x000200F8, 0x00000567, 0x00040008, 0x00000001, 0x00000216, 0x00000000, 0x0004003D, 0x00000007, 0x00000569, 0x00000079, 0x00050081, 
0x00000007, 0x0000056A, 0x00000569, 0x00000129, 0x0003003E, 0x00000079, 0x0000056A, 0x000200F9, 0x00000568, 0x000200F8, 0x00000568, 0x00040008, 
0x00000001, 0x0000021A, 0x00000000, 0x0004003D, 0x00000009, 0x0000056B, 0x00000076, 0x0005008E, 0x00000009, 0x0000056D, 0x0000056B, 0x0000056C, 
0x0003003E, 0x0000056E, 0x0000056D, 0x00050039, 0x00000007, 0x0000056F, 0x0000001D, 0x0000056E, 0x0004003D, 0x00000007, 0x00000570, 0x0000052E, 
0x00050085, 0x00000007, 0x00000571, 0x00000570, 0x0000056F, 0x0003003E, 0x0000052E, 0x00000571, 0x00040008, 0x00000001, 0x0000021B, 0x00000000, 
0x0004003D, 0x00000007, 0x00000572, 0x0000052E, 0x0004003D, 0x00000008, 0x00000573, 0x00000078, 0x0005008E, 0x00000008, 0x00000574, 0x00000573, 
0x00000572, 0x0003003E, 0x00000078, 0x00000574, 0x00040008, 0x00000001, 0x0000021D, 0x00000000, 0x00050041, 0x00000015, 0x00000576, 0x00000077, 
0x00000098, 0x0004003D, 0x00000007, 0x00000577, 0x00000576, 0x00050085, 0x00000007, 0x00000578, 0x00000577, 0x00000551, 0x00050083, 0x00000007, 
0x00000579, 0x00000578, 0x00000285, 0x00050085, 0x00000007, 0x0000057B, 0x00000579, 0x0000057A, 0x0006000C, 0x00000007, 0x0000057C, 0x00000002, 
0x0000000D, 0x0000057B, 0x00050085, 0x00000007, 0x0000057E, 0x0000057C, 0x0000057D, 0x00050083, 0x00000007, 0x0000057F, 0x0000057E, 0x000001CC, 
0x0006000C, 0x00000007, 0x00000580, 0x00000002, 0x00000008, 0x0000057F, 0x00050081, 0x00000007, 0x00000581, 0x00000580, 0x00000129, 0x0003003E, 
0x00000575, 0x00000581, 0x00040008, 0x00000001, 0x0000021E, 0x00000000, 0x0004003D, 0x00000007, 0x00000582, 0x00000575, 0x0004007F, 0x00000007, 
0x00000583, 0x00000582, 0x00050085, 0x00000007, 0x00000584, 0x00000583, 0x000001CC, 0x0007000C, 0x00000007, 0x00000585, 0x00000002, 0x00000025, 
0x00000129, 0x00000584, 0x0007000C, 0x00000007, 0x00000586, 0x00000002, 0x00000028, 0x000004AC, 0x00000585, 0x00050041, 0x00000015, 0x00000587, 
0x0000007A, 0x00000098, 0x0004003D, 0x00000007, 0x00000588, 0x00000587, 0x00050083, 0x00000007, 0x00000589, 0x00000588, 0x00000586, 0x00050041, 
0x00000015, 0x0000058A, 0x0000007A, 0x00000098, 0x0003003E, 0x0000058A, 0x00000589, 0x00040008, 0x00000001, 0x00000220, 0x00000000, 0x00050041, 
0x00000015, 0x0000058C, 0x00000077, 0x00000102, 0x0004003D, 0x00000007, 0x0000058D, 0x0000058C, 0x00050085, 0x00000007, 0x0000058F, 0x0000058D, 
0x0000058E, 0x00050085, 0x00000007, 0x00000590, 0x0000058F, 0x0000057A, 0x0006000C, 0x00000007, 0x00000591, 0x00000002, 0x0000000D, 0x00000590, 
0x0006000C, 0x00000007, 0x00000592, 0x00000002, 0x00000004, 0x00000591, 0x00050085, 0x00000007, 0x00000593, 0x00000592, 0x00000214, 0x0007000C, 
0x00000007, 0x00000594, 0x00000002, 0x00000025, 0x00000129, 0x00000593, 0x00050083, 0x00000007, 0x00000595, 0x00000594, 0x00000129, 0x0003003E, 
0x0000058B, 0x00000595, 0x00040008, 0x00000001, 0x00000221, 0x00000000, 0x0004003D, 0x00000007, 0x00000596, 0x0000058B, 0x00050085, 0x00000007, 
0x00000597, 0x00000596, 0x00000218, 0x00050041, 0x00000015, 0x00000598, 0x0000007A, 0x00000102, 0x0004003D, 0x00000007, 0x00000599, 0x00000598, 
0x00050081, 0x00000007, 0x0000059A, 0x00000599, 0x00000597, 0x00050041, 0x00000015, 0x0000059B, 0x0000007A, 0x00000102, 0x0003003E, 0x0000059B, 
0x0000059A, 0x00040008, 0x00000001, 0x00000222, 0x00000000, 0x00050041, 0x00000015, 0x0000059C, 0x00000077, 0x000000A9, 0x0004003D, 0x00000007, 
0x0000059D, 0x0000059C, 0x00050085, 0x00000007, 0x0000059E, 0x0000059D, 0x0000058E, 0x00050085, 0x00000007, 0x0000059F, 0x0000059E, 0x0000057A, 
0x0006000C, 0x00000007, 0x000005A0, 0x00000002, 0x0000000D, 0x0000059F, 0x0006000C, 0x00000007, 0x000005A1, 0x00000002, 0x00000004, 0x000005A0, 
0x00050085, 0x00000007, 0x000005A2, 0x000005A1, 0x00000214, 0x0007000C, 0x00000007, 0x000005A3, 0x00000002, 0x00000025, 0x00000129, 0x000005A2, 
0x00050083, 0x00000007, 0x000005A4, 0x000005A3, 0x00000129, 0x0003003E, 0x0000058B, 0x000005A4, 0x00040008, 0x00000001, 0x00000223, 0x00000000, 
0x0004003D, 0x00000007, 0x000005A5, 0x0000058B, 0x00050085, 0x00000007, 0x000005A6, 0x000005A5, 0x00000218, 0x00050041, 0x00000015, 0x000005A7, 
0x0000007A, 0x000000A9, 0x0004003D, 0x00000007, 0x000005A8, 0x000005A7, 0x00050081, 0x00000007, 0x000005A9, 0x000005A8, 0x000005A6, 0x00050041, 
0x00000015, 0x000005AA, 0x0000007A, 0x000000A9, 0x0003003E, 0x000005AA, 0x000005A9, 0x000100FD, 0x00010038, 0x00050036, 0x00000008, 0x00000080, 
0x00000000, 0x0000007D, 0x00030037, 0x00000009, 0x0000007E, 0x00030037, 0x0000000A, 0x0000007F, 0x000200F8, 0x00000081, 0x0004003B, 0x00000026, 
0x000005BB, 0x00000007, 0x0004003B, 0x00000026, 0x000005BD, 0x00000007, 0x0004003B, 0x00000026, 0x000005BE, 0x00000007, 0x0004003B, 0x0000001A, 
0x000005BF, 0x00000007, 0x0004003B, 0x00000015, 0x000005D4, 0x00000007, 0x0004003B, 0x00000015, 0x000005D7, 0x00000007, 0x0004003B, 0x00000015, 
0x000005DA, 0x00000007, 0x0004003B, 0x00000015, 0x000005DF, 0x00000007, 0x0004003B, 0x00000015, 0x000005F6, 0x00000007, 0x0004003B, 0x00000015, 
0x000005F9, 0x00000007, 0x0004003B, 0x00000015, 0x000005FD, 0x00000007, 0x0004003B, 0x00000015, 0x00000602, 0x00000007, 0x0004003B, 0x00000015, 
0x00000629, 0x00000007, 0x0004003B, 0x00000015, 0x0000062C, 0x00000007, 0x0004003B, 0x00000015, 0x0000062F, 0x00000007, 0x0004003B, 0x00000015, 
0x00000634, 0x00000007, 0x0004003B, 0x00000015, 0x0000065A, 0x00000007, 0x0004003B, 0x00000015, 0x0000065D, 0x00000007, 0x0004003B, 0x00000015, 
0x00000660, 0x00000007, 0x0004003B, 0x00000015, 0x00000665, 0x00000007, 0x0004003B, 0x00000015, 0x00000686, 0x00000007, 0x0004003B, 0x00000015, 
0x00000689, 0x00000007, 0x0004003B, 0x00000015, 0x0000068D, 0x00000007, 0x0004003B, 0x00000015, 0x00000692, 0x00000007, 0x0004003B, 0x00000026, 
0x000006BE, 0x00000007, 0x0004003B, 0x00000026, 0x000006C3, 0x00000007, 0x0004003B, 0x00000026, 0x000006C8, 0x00000007, 0x0004003B, 0x00000026, 
0x000006CC, 0x00000007, 0x0004003B, 0x00000026, 0x000006D0, 0x00000007, 0x0004003B, 0x00000026, 0x000006E2, 0x00000007, 0x0004003B, 0x0000001A, 
0x000006E4, 0x00000007, 0x0004003B, 0x00000015, 0x000006E6, 0x00000007, 0x0004003B, 0x00000026, 0x000006E7, 0x00000007, 0x0004003B, 0x000006E8, 
0x000006E9, 0x00000007, 0x0004003B, 0x00000026, 0x000006F9, 0x00000007, 0x0004003B, 0x00000015, 0x000006FC, 0x00000007, 0x0004003B, 0x00000015, 
0x000006FF, 0x00000007, 0x0004003B, 0x00000015, 0x0000070D, 0x00000007, 0x0004003B, 0x00000015, 0x0000071B, 0x00000007, 0x0004003B, 0x00000015, 
0x00000744, 0x00000007, 0x0004003B, 0x00000026, 0x00000762, 0x00000007, 0x0004003B, 0x00000015, 0x0000076B, 0x00000007, 0x0004003B, 0x00000026, 
0x0000076E, 0x00000007, 0x0004003B, 0x00000026, 0x00000770, 0x00000007, 0x0004003B, 0x00000026, 0x00000776, 0x00000007, 0x0004003B, 0x00000026, 
0x0000077F, 0x00000007, 0x0004003B, 0x00000026, 0x00000788, 0x00000007, 0x0004003B, 0x00000026, 0x0000078D, 0x00000007, 0x0004003B, 0x00000015, 
0x00000790, 0x00000007, 0x0004003B, 0x00000026, 0x00000795, 0x00000007, 0x0004003B, 0x00000015, 0x00000799, 0x00000007, 0x0004003B, 0x00000026, 
0x000007A1, 0x00000007, 0x0004003B, 0x00000015, 0x000007A5, 0x00000007, 0x0004003B, 0x00000026, 0x000007AD, 0x00000007, 0x0004003B, 0x00000015, 
0x000007B1, 0x00000007, 0x0004003B, 0x00000026, 0x000007B9, 0x00000007, 0x0004003B, 0x00000015, 0x000007BD, 0x00000007, 0x0004003B, 0x00000026, 
0x000007C5, 0x00000007, 0x0004003B, 0x00000015, 0x000007C9, 0x00000007, 0x0004003B, 0x00000026, 0x000007D1, 0x00000007, 0x0004003B, 0x00000015, 
0x000007D5, 0x00000007, 0x0004003B, 0x00000015, 0x000007D9, 0x00000007, 0x0004003B, 0x00000015, 0x000007DE, 0x00000007, 0x0004003B, 0x00000026, 
0x000007E1, 0x00000007, 0x0004003B, 0x00000015, 0x000007E5, 0x00000007, 0x0004003B, 0x00000015, 0x000007E6, 0x00000007, 0x0004003B, 0x00000026, 
0x000007E7, 0x00000007, 0x0004003B, 0x000006E8, 0x000007ED, 0x00000007, 0x0004003B, 0x00000026, 0x000007F6, 0x00000007, 0x0004003B, 0x00000015, 
0x000007FC, 0x00000007, 0x0004003B, 0x00000026, 0x000007FD, 0x00000007, 0x0004003B, 0x00000015, 0x00000803, 0x00000007, 0x0004003B, 0x00000015, 
0x0000080C, 0x00000007, 0x0004003B, 0x00000015, 0x0000080E, 0x00000007, 0x0004003B, 0x00000015, 0x0000081C, 0x00000007, 0x0004003B, 0x00000015, 
0x0000082A, 0x00000007, 0x0004003B, 0x00000015, 0x00000848, 0x00000007, 0x0004003B, 0x00000015, 0x0000084B, 0x00000007, 0x0004003B, 0x00000026, 
0x0000084E, 0x00000007, 0x0004003B, 0x00000026, 0x00000855, 0x00000007, 0x0004003B, 0x00000026, 0x0000085C, 0x00000007, 0x0004003B, 0x00000026, 
0x00000862, 0x00000007, 0x0004003B, 0x00000026, 0x00000869, 0x00000007, 0x0004003B, 0x0000001A, 0x0000086F, 0x00000007, 0x0004003B, 0x00000026, 
0x00000873, 0x00000007, 0x0004003B, 0x00000015, 0x00000877, 0x00000007, 0x0004003B, 0x00000026, 0x00000883, 0x00000007, 0x0004003B, 0x00000026, 
0x00000886, 0x00000007, 0x0004003B, 0x00000026, 0x00000889, 0x00000007, 0x0004003B, 0x0000001A, 0x0000088B, 0x00000007, 0x0004003B, 0x0000001A, 
0x00000893, 0x00000007, 0x0004003B, 0x00000026, 0x00000897, 0x00000007, 0x0004003B, 0x00000026, 0x00000898, 0x00000007, 0x0004003B, 0x00000026, 
0x0000089A, 0x00000007, 0x0004003B, 0x0000001A, 0x0000089B, 0x00000007, 0x0004003B, 0x00000026, 0x0000089D, 0x00000007, 0x0004003B, 0x00000026, 
0x0000089F, 0x00000007, 0x0004003B, 0x00000015, 0x000008A1, 0x00000007, 0x0004003B, 0x00000026, 0x000008A3, 0x00000007, 0x0004003B, 0x0000001A, 
0x000008B0, 0x00000007, 0x0004003B, 0x00000026, 0x000008B2, 0x00000007, 0x0004003B, 0x00000026, 0x000008B3, 0x00000007, 0x0004003B, 0x00000015, 
0x000008B5, 0x00000007, 0x0004003B, 0x00000026, 0x000008B7, 0x00000007, 0x0004003B, 0x0000001A, 0x000008C8, 0x00000007, 0x0004003B, 0x00000026, 
0x000008CA, 0x00000007, 0x0004003B, 0x00000026, 0x000008CB, 0x00000007, 0x0004003B, 0x00000015, 0x000008CD, 0x00000007, 0x0004003B, 0x00000026, 
0x000008CF, 0x00000007, 0x0004003B, 0x0000001A, 0x000008DD, 0x00000007, 0x0004003B, 0x00000026, 0x000008DF, 0x00000007, 0x0004003B, 0x00000026, 
0x000008E0, 0x00000007, 0x0004003B, 0x00000015, 0x000008E2, 0x00000007, 0x0004003B, 0x00000026, 0x000008E4, 0x00000007, 0x0004003B, 0x0000001A, 
0x000008F6, 0x00000007, 0x0004003B, 0x00000026, 0x000008F8, 0x00000007, 0x0004003B, 0x00000026, 0x000008F9, 0x00000007, 0x0004003B, 0x00000015, 
0x000008FB, 0x00000007, 0x0004003B, 0x00000026, 0x000008FD, 0x00000007, 0x0004003B, 0x00000015, 0x00000910, 0x00000007, 0x0004003B, 0x00000015, 
0x00000917, 0x00000007, 0x0004003B, 0x00000015, 0x0000091E, 0x00000007, 0x0004003B, 0x00000015, 0x00000926, 0x00000007, 0x0004003B, 0x0000001A, 
0x0000092E, 0x00000007, 0x0004003B, 0x00000015, 0x0000093A, 0x00000007, 0x0004003B, 0x00000015, 0x00000941, 0x00000007, 0x0004003B, 0x00000015, 
0x00000948, 0x00000007, 0x0004003B, 0x00000015, 0x00000956, 0x00000007, 0x0004003B, 0x00000015, 0x0000095F, 0x00000007, 0x0004003B, 0x00000015, 
0x00000967, 0x00000007, 0x0004003B, 0x00000015, 0x0000096F, 0x00000007, 0x0004003B, 0x00000015, 0x0000097C, 0x00000007, 0x0004003B, 0x00000015, 
0x00000987, 0x00000007, 0x0004003B, 0x00000015, 0x00000991, 0x00000007, 0x0004003B, 0x00000015, 0x000009A1, 0x00000007, 0x0004003B, 0x00000015, 
0x000009A9, 0x00000007, 0x0004003B, 0x00000015, 0x000009B6, 0x00000007, 0x0004003B, 0x00000015, 0x000009BE, 0x00000007, 0x0004003B, 0x00000015, 
0x000009C5, 0x00000007, 0x0004003B, 0x00000015, 0x000009CA, 0x00000007, 0x0004003B, 0x00000015, 0x000009D3, 0x00000007, 0x0004003B, 0x00000015, 
0x000009EA, 0x00000007, 0x0004003B, 0x00000015, 0x000009EF, 0x00000007, 0x0004003B, 0x00000015, 0x000009F7, 0x00000007, 0x0004003B, 0x00000015, 
0x00000A07, 0x00000007, 0x0004003B, 0x0000001A, 0x00000A08, 0x00000007, 0x0004003B, 0x00000015, 0x00000A1D, 0x00000007, 0x0004003B, 0x00000015, 
0x00000A2C, 0x00000007, 0x0004003B, 0x00000015, 0x00000A32, 0x00000007, 0x0004003B, 0x00000015, 0x00000A38, 0x00000007, 0x0004003B, 0x00000026, 
0x00000A4A, 0x00000007, 0x0004003B, 0x00000015, 0x00000A4D, 0x00000007, 0x0004003B, 0x00000015, 0x00000A53, 0x00000007, 0x0004003B, 0x00000015, 
0x00000A5C, 0x00000007, 0x0004003B, 0x00000015, 0x00000A62, 0x00000007, 0x0004003B, 0x00000015, 0x00000A76, 0x00000007, 0x0004003B, 0x00000015, 
0x00000A84, 0x00000007, 0x0004003B, 0x0000001A, 0x00000A86, 0x00000007, 0x0004003B, 0x00000015, 0x00000A8B, 0x00000007, 0x0004003B, 0x00000015, 
0x00000A92, 0x00000007, 0x0004003B, 0x00000026, 0x00000A9E, 0x00000007, 0x0004003B, 0x00000015, 0x00000AA5, 0x00000007, 0x0004003B, 0x00000015, 
0x00000AAA, 0x00000007, 0x0004003B, 0x00000015, 0x00000AB5, 0x00000007, 0x0004003B, 0x00000015, 0x00000AC8, 0x00000007, 0x0004003B, 0x00000015, 
0x00000AD7, 0x00000007, 0x0004003B, 0x00000026, 0x00000ADE, 0x00000007, 0x0004003B, 0x00000026, 0x00000AE0, 0x00000007, 0x0004003B, 0x00000015, 
0x00000AE2, 0x00000007, 0x0004003B, 0x00000015, 0x00000AF4, 0x00000007, 0x0004003B, 0x00000026, 0x00000AFD, 0x00000007, 0x0004003B, 0x00000026, 
0x00000AFF, 0x00000007, 0x0004003B, 0x00000015, 0x00000B01, 0x00000007, 0x0004003B, 0x00000026, 0x00000B0F, 0x00000007, 0x0004003B, 0x00000015, 
0x00000B14, 0x00000007, 0x0004003B, 0x00000026, 0x00000B1A, 0x00000007, 0x0004003B, 0x00000026, 0x00000B1B, 0x00000007, 0x0004003B, 0x00000026, 
0x00000B1D, 0x00000007, 0x0004003B, 0x00000026, 0x00000B2D, 0x00000007, 0x0004003B, 0x00000026, 0x00000B2F, 0x00000007, 0x0004003B, 0x00000015, 
0x00000B37, 0x00000007, 0x0004003B, 0x00000015, 0x00000B46, 0x00000007, 0x00040008, 0x00000001, 0x00000229, 0x00000000, 0x0003003E, 0x000005AB, 
0x0000012A, 0x00040008, 0x00000001, 0x0000022A, 0x00000000, 0x0003003E, 0x000001E1, 0x000005AF, 0x00040008, 0x00000001, 0x0000022B, 0x00000000, 
0x0003003E, 0x000005B0, 0x000005B3, 0x00040008, 0x00000001, 0x0000022C, 0x00000000, 0x0003003E, 0x000001EA, 0x000005B7, 0x00040008, 0x00000001, 
0x0000022D, 0x00000000, 0x0003003E, 0x000001EC, 0x000005B8, 0x00040008, 0x00000001, 0x0000022E, 0x00000000, 0x0003003E, 0x000005B9, 0x00000129, 
0x00040008, 0x00000001, 0x0000022F, 0x00000000, 0x0003003E, 0x000005BA, 0x00000129, 0x00040008, 0x00000001, 0x00000231, 0x00000000, 0x0003003E, 
0x000005BB, 0x000005BC, 0x00040008, 0x00000001, 0x00000233, 0x00000000, 0x0003003E, 0x000005BD, 0x000005BC, 0x00040008, 0x00000001, 0x00000235, 
0x00000000, 0x0003003E, 0x000005BE, 0x000005BC, 0x00040008, 0x00000001, 0x00000237, 0x00000000, 0x00050041, 0x000005C0, 0x000005C1, 0x00000096, 
0x00000097, 0x0004003D, 0x00000082, 0x000005C2, 0x000005C1, 0x0007004F, 0x00000009, 0x000005C3, 0x000005C2, 0x000005C2, 0x00000000, 0x00000001, 
0x00050088, 0x00000009, 0x000005C4, 0x0000007E, 0x000005C3, 0x0005008E, 0x00000009, 0x000005C5, 0x000005C4, 0x00000115, 0x00050050, 0x00000009, 
0x000005C6, 0x00000129, 0x00000129, 0x00050083, 0x00000009, 0x000005C7, 0x000005C5, 0x000005C6, 0x0003003E, 0x000005BF, 0x000005C7, 0x00040008, 
0x00000001, 0x00000238, 0x00000000, 0x0004003D, 0x00000009, 0x000005C8, 0x000005BF, 0x00050050, 0x00000009, 0x000005C9, 0x00000115, 0x00000115, 
0x00050088, 0x00000009, 0x000005CA, 0x000005C8, 0x000005C9, 0x0003003E, 0x000005BF, 0x000005CA, 0x00040008, 0x00000001, 0x00000239, 0x00000000, 
0x0004003D, 0x00000007, 0x000005CB, 0x00000499, 0x00050088, 0x00000007, 0x000005CD, 0x000005CB, 0x000005CC, 0x0006000C, 0x00000007, 0x000005CE, 
0x00000002, 0x0000000A, 0x000005CD, 0x00050085, 0x00000007, 0x000005CF, 0x000005CE, 0x000005CC, 0x0003003E, 0x00000499, 0x000005CF, 0x00040008, 
0x00000001, 0x0000023A, 0x00000000, 0x0004003D, 0x00000007, 0x000005D0, 0x00000499, 0x000500B8, 0x000002C0, 0x000005D1, 0x000005D0, 0x00000222, 
0x000300F7, 0x000005D3, 0x00000000, 0x000400FA, 0x000005D1, 0x000005D2, 0x000005F0, 0x000200F8, 0x000005D2, 0x00040008, 0x00000001, 0x0000023D, 
0x00000000, 0x0004003D, 0x00000007, 0x000005D5, 0x00000499, 0x00050083, 0x00000007, 0x000005D6, 0x000005D5, 0x0000012A, 0x0003003E, 0x000005D4, 
0x000005D6, 0x00040008, 0x00000001, 0x0000023F, 0x00000000, 0x0004003D, 0x00000007, 0x000005D8, 0x000005D4, 0x00050088, 0x00000007, 0x000005D9, 
0x000005D8, 0x00000222, 0x0003003E, 0x000005D7, 0x000005D9, 0x00040008, 0x00000001, 0x00000240, 0x00000000, 0x0004003D, 0x00000007, 0x000005DB, 
0x000005D4, 0x0003003E, 0x000005DA, 0x000005DB, 0x00050039, 0x00000007, 0x000005DC, 0x0000003C, 0x000005DA, 0x0003003E, 0x000005BA, 0x000005DC, 
0x00040008, 0x00000001, 0x00000241, 0x00000000, 0x0004003D, 0x00000007, 0x000005DD, 0x00000499, 0x00050083, 0x00000007, 0x000005DE, 0x00000222, 
0x000005DD, 0x0003003E, 0x000005DF, 0x000005DE, 0x00050039, 0x00000007, 0x000005E0, 0x0000003C, 0x000005DF, 0x0004003D, 0x00000007, 0x000005E1, 
0x000005BA, 0x00050085, 0x00000007, 0x000005E2, 0x000005E1, 0x000005E0, 0x0003003E, 0x000005BA, 0x000005E2, 0x00040008, 0x00000001, 0x00000242, 
0x00000000, 0x0003003E, 0x000005BB, 0x000005E6, 0x00040008, 0x00000001, 0x00000243, 0x00000000, 0x0004003D, 0x00000007, 0x000005E7, 0x000005D7, 
0x0008000C, 0x00000007, 0x000005E8, 0x00000002, 0x00000031, 0x0000012A, 0x00000129, 0x000005E7, 0x00050085, 0x00000007, 0x000005EA, 0x000005E8, 
0x000005E9, 0x00050041, 0x00000015, 0x000005EB, 0x000005BB, 0x000000A9, 0x0004003D, 0x00000007, 0x000005EC, 0x000005EB, 0x00050083, 0x00000007, 
0x000005ED, 0x000005EC, 0x000005EA, 0x00050041, 0x00000015, 0x000005EE, 0x000005BB, 0x000000A9, 0x0003003E, 0x000005EE, 0x000005ED, 0x00040008, 
0x00000001, 0x00000244, 0x00000000, 0x0003003E, 0x000005BD, 0x00000165, 0x00040008, 0x00000001, 0x00000245, 0x00000000, 0x0003003E, 0x000005BE, 
0x000005EF, 0x000200F9, 0x000005D3, 0x000200F8, 0x000005F0, 0x00040008, 0x00000001, 0x0000024A, 0x00000000, 0x0004003D, 0x00000007, 0x000005F1, 
0x00000499, 0x000500B8, 0x000002C0, 0x000005F3, 0x000005F1, 0x000005F2, 0x000300F7, 0x000005F5, 0x00000000, 0x000400FA, 0x000005F3, 0x000005F4, 
0x00000624, 0x000200F8, 0x000005F4, 0x00040008, 0x00000001, 0x0000024D, 0x00000000, 0x0004003D, 0x00000007, 0x000005F7, 0x00000499, 0x00050083, 
0x00000007, 0x000005F8, 0x000005F7, 0x00000222, 0x0003003E, 0x000005F6, 0x000005F8, 0x00040008, 0x00000001, 0x0000024F, 0x00000000, 0x0004003D, 
0x00000007, 0x000005FA, 0x000005F6, 0x00050088, 0x00000007, 0x000005FC, 0x000005FA, 0x000005FB, 0x0003003E, 0x000005F9, 0x000005FC, 0x00040008, 
0x00000001, 0x00000250, 0x00000000, 0x0004003D, 0x00000007, 0x000005FE, 0x000005F6, 0x0003003E, 0x000005FD, 0x000005FE, 0x00050039, 0x00000007, 
0x000005FF, 0x0000003C, 0x000005FD, 0x0003003E, 0x000005BA, 0x000005FF, 0x00040008, 0x00000001, 0x00000251, 0x00000000, 0x0004003D, 0x00000007, 
0x00000600, 0x00000499, 0x00050083, 0x00000007, 0x00000601, 0x000005F2, 0x00000600, 0x0003003E, 0x00000602, 0x00000601, 0x00050039, 0x00000007, 
0x00000603, 0x0000003C, 0x00000602, 0x0004003D, 0x00000007, 0x00000604, 0x000005BA, 0x00050085, 0x00000007, 0x00000605, 0x00000604, 0x00000603, 
0x0003003E, 0x000005BA, 0x00000605, 0x00040008, 0x00000001, 0x00000252, 0x00000000, 0x0004003D, 0x00000007, 0x00000607, 0x000005F9, 0x0008000C, 
0x00000007, 0x00000608, 0x00000002, 0x00000031, 0x0000012A, 0x00000129, 0x00000607, 0x00050085, 0x00000007, 0x00000609, 0x00000608, 0x000003AD, 
0x00050081, 0x00000007, 0x0000060A, 0x000001D1, 0x00000609, 0x00060050, 0x00000008, 0x0000060B, 0x00000606, 0x0000060A, 0x00000115, 0x0003003E, 
0x000005BB, 0x0000060B, 0x00040008, 0x00000001, 0x00000253, 0x00000000, 0x0004003D, 0x00000007, 0x0000060C, 0x000005F9, 0x00050085, 0x00000007, 
0x0000060E, 0x0000060C, 0x0000060D, 0x00050041, 0x00000015, 0x0000060F, 0x000005BB, 0x00000102, 0x0004003D, 0x00000007, 0x00000610, 0x0000060F, 
0x00050083, 0x00000007, 0x00000611, 0x00000610, 0x0000060E, 0x00050041, 0x00000015, 0x00000612, 0x000005BB, 0x00000102, 0x0003003E, 0x00000612, 
0x00000611, 0x00040008, 0x00000001, 0x00000254, 0x00000000, 0x0003003E, 0x000005BD, 0x00000165, 0x00040008, 0x00000001, 0x00000255, 0x00000000, 
0x00050041, 0x00000015, 0x00000613, 0x000005BB, 0x000000A9, 0x0004003D, 0x00000007, 0x00000614, 0x00000613, 0x00050083, 0x00000007, 0x00000615, 
0x00000614, 0x0000041A, 0x00060050, 0x00000008, 0x00000618, 0x00000615, 0x00000616, 0x00000617, 0x0003003E, 0x000005BE, 0x00000618, 0x00040008, 
0x00000001, 0x00000256, 0x00000000, 0x0003003E, 0x000005B0, 0x0000061C, 0x00040008, 0x00000001, 0x00000257, 0x00000000, 0x0003003E, 0x000001E1, 
0x0000061E, 0x00040008, 0x00000001, 0x00000258, 0x00000000, 0x0004003D, 0x00000007, 0x00000620, 0x000005B9, 0x00050085, 0x00000007, 0x00000621, 
0x00000620, 0x0000061F, 0x0003003E, 0x000005B9, 0x00000621, 0x00040008, 0x00000001, 0x00000259, 0x00000000, 0x0004003D, 0x00000008, 0x00000622, 
0x000001EC, 0x0005008E, 0x00000008, 0x00000623, 0x00000622, 0x00000377, 0x0003003E, 0x000001EC, 0x00000623, 0x000200F9, 0x000005F5, 0x000200F8, 
0x00000624, 0x00040008, 0x00000001, 0x0000025E, 0x00000000, 0x0004003D, 0x00000007, 0x00000625, 0x00000499, 0x000500B8, 0x000002C0, 0x00000626, 
0x00000625, 0x00000293, 0x000300F7, 0x00000628, 0x00000000, 0x000400FA, 0x00000626, 0x00000627, 0x00000654, 0x000200F8, 0x00000627, 0x00040008, 
0x00000001, 0x00000261, 0x00000000, 0x0004003D, 0x00000007, 0x0000062A, 0x00000499, 0x00050083, 0x00000007, 0x0000062B, 0x0000062A, 0x000005F2, 
0x0003003E, 0x00000629, 0x0000062B, 0x00040008, 0x00000001, 0x00000263, 0x00000000, 0x0004003D, 0x00000007, 0x0000062D, 0x00000629, 0x00050088, 
0x00000007, 0x0000062E, 0x0000062D, 0x0000023A, 0x0003003E, 0x0000062C, 0x0000062E, 0x00040008, 0x00000001, 0x00000264, 0x00000000, 0x0004003D, 
0x00000007, 0x00000630, 0x00000629, 0x0003003E, 0x0000062F, 0x00000630, 0x00050039, 0x00000007, 0x00000631, 0x0000003C, 0x0000062F, 0x0003003E, 
0x000005BA, 0x00000631, 0x00040008, 0x00000001, 0x00000265, 0x00000000, 0x0004003D, 0x00000007, 0x00000632, 0x00000499, 0x00050083, 0x00000007, 
0x00000633, 0x00000293, 0x00000632, 0x0003003E, 0x00000634, 0x00000633, 0x00050039, 0x00000007, 0x00000635, 0x0000003C, 0x00000634, 0x0004003D, 
0x00000007, 0x00000636, 0x000005BA, 0x00050085, 0x00000007, 0x00000637, 0x00000636, 0x00000635, 0x0003003E, 0x000005BA, 0x00000637, 0x00040008, 
0x00000001, 0x00000266, 0x00000000, 0x0003003E, 0x000005BB, 0x0000063B, 0x00040008, 0x00000001, 0x00000267, 0x00000000, 0x0004003D, 0x00000007, 
0x0000063C, 0x0000062C, 0x00050085, 0x00000007, 0x0000063E, 0x0000063C, 0x0000063D, 0x00050041, 0x00000015, 0x0000063F, 0x000005BB, 0x00000098, 
0x0004003D, 0x00000007, 0x00000640, 0x0000063F, 0x00050083, 0x00000007, 0x00000641, 0x00000640, 0x0000063E, 0x00050041, 0x00000015, 0x00000642, 
0x000005BB, 0x00000098, 0x0003003E, 0x00000642, 0x00000641, 0x00040008, 0x00000001, 0x00000268, 0x00000000, 0x0004003D, 0x00000007, 0x00000643, 
0x0000062C, 0x00050085, 0x00000007, 0x00000644, 0x00000643, 0x00000129, 0x0006000C, 0x00000007, 0x00000645, 0x00000002, 0x0000000E, 0x00000644, 
0x00050085, 0x00000007, 0x00000647, 0x00000645, 0x00000646, 0x00050041, 0x00000015, 0x00000648, 0x000005BB, 0x000000A9, 0x0003003E, 0x00000648, 
0x00000647, 0x00040008, 0x00000001, 0x00000269, 0x00000000, 0x0004003D, 0x00000007, 0x00000649, 0x0000062C, 0x00050085, 0x00000007, 0x0000064A, 
0x00000649, 0x00000129, 0x0006000C, 0x00000007, 0x0000064B, 0x00000002, 0x0000000D, 0x0000064A, 0x00050085, 0x00000007, 0x0000064C, 0x0000064B, 
0x00000646, 0x00050041, 0x00000015, 0x0000064D, 0x000005BB, 0x00000102, 0x0003003E, 0x0000064D, 0x0000064C, 0x00040008, 0x00000001, 0x0000026A, 
0x00000000, 0x0004003D, 0x00000007, 0x0000064E, 0x0000062C, 0x00050085, 0x00000007, 0x0000064F, 0x0000064E, 0x000001CC, 0x00050081, 0x00000007, 
0x00000650, 0x0000063A, 0x0000064F, 0x00060050, 0x00000008, 0x00000651, 0x0000012A, 0x00000129, 0x00000650, 0x0006000C, 0x00000008, 0x00000652, 
0x00000002, 0x00000045, 0x00000651, 0x0003003E, 0x000005BD, 0x00000652, 0x00040008, 0x00000001, 0x0000026B, 0x00000000, 0x0003003E, 0x000005BE, 
0x00000653, 0x000200F9, 0x00000628, 0x000200F8, 0x00000654, 0x00040008, 0x00000001, 0x00000270, 0x00000000, 0x0004003D, 0x00000007, 0x00000655, 
0x00000499, 0x000500B8, 0x000002C0, 0x00000657, 0x00000655, 0x00000656, 0x000300F7, 0x00000659, 0x00000000, 0x000400FA, 0x00000657, 0x00000658, 
0x00000680, 0x000200F8, 0x00000658, 0x00040008, 0x00000001, 0x00000273, 0x00000000, 0x0004003D, 0x00000007, 0x0000065B, 0x00000499, 0x00050083, 
0x00000007, 0x0000065C, 0x0000065B, 0x00000293, 0x0003003E, 0x0000065A, 0x0000065C, 0x00040008, 0x00000001, 0x00000275, 0x00000000, 0x0004003D, 
0x00000007, 0x0000065E, 0x0000065A, 0x00050088, 0x00000007, 0x0000065F, 0x0000065E, 0x000005F2, 0x0003003E, 0x0000065D, 0x0000065F, 0x00040008, 
0x00000001, 0x00000276, 0x00000000, 0x0004003D, 0x00000007, 0x00000661, 0x0000065A, 0x0003003E, 0x00000660, 0x00000661, 0x00050039, 0x00000007, 
0x00000662, 0x0000003C, 0x00000660, 0x0003003E, 0x000005BA, 0x00000662, 0x00040008, 0x00000001, 0x00000277, 0x00000000, 0x0004003D, 0x00000007, 
0x00000663, 0x00000499, 0x00050083, 0x00000007, 0x00000664, 0x00000656, 0x00000663, 0x0003003E, 0x00000665, 0x00000664, 0x00050039, 0x00000007, 
0x00000666, 0x0000003C, 0x00000665, 0x0004003D, 0x00000007, 0x00000667, 0x000005BA, 0x00050085, 0x00000007, 0x00000668, 0x00000667, 0x00000666, 
0x0003003E, 0x000005BA, 0x00000668, 0x00040008, 0x00000001, 0x00000278, 0x00000000, 0x0004003D, 0x00000007, 0x0000066A, 0x0000065D, 0x00050085, 
0x00000007, 0x0000066B, 0x0000066A, 0x000001CC, 0x00050083, 0x00000007, 0x0000066C, 0x00000669, 0x0000066B, 0x0004003D, 0x00000007, 0x0000066D, 
0x0000065D, 0x00050085, 0x00000007, 0x0000066E, 0x0000066D, 0x0000022E, 0x00050083, 0x00000007, 0x0000066F, 0x000004AC, 0x0000066E, 0x00060050, 
0x00000008, 0x00000670, 0x0000066C, 0x0000032B, 0x0000066F, 0x0003003E, 0x000005BB, 0x00000670, 0x00040008, 0x00000001, 0x00000279, 0x00000000, 
0x0004003D, 0x00000007, 0x00000671, 0x0000065D, 0x0004003D, 0x00000007, 0x00000672, 0x0000065D, 0x00050085, 0x00000007, 0x00000673, 0x00000671, 
0x00000672, 0x0008000C, 0x00000007, 0x00000674, 0x00000002, 0x00000031, 0x0000012A, 0x00000129, 0x00000673, 0x00050085, 0x00000007, 0x00000676, 
0x00000674, 0x00000675, 0x00050041, 0x00000015, 0x00000677, 0x000005BB, 0x00000098, 0x0004003D, 0x00000007, 0x00000678, 0x00000677, 0x00050081, 
0x00000007, 0x00000679, 0x00000678, 0x00000676, 0x00050041, 0x00000015, 0x0000067A, 0x000005BB, 0x00000098, 0x0003003E, 0x0000067A, 0x00000679, 
0x00040008, 0x00000001, 0x0000027A, 0x00000000, 0x0003003E, 0x000005BD, 0x00000165, 0x00040008, 0x00000001, 0x0000027B, 0x00000000, 0x0004003D, 
0x00000007, 0x0000067B, 0x0000065D, 0x00050081, 0x00000007, 0x0000067C, 0x000001CC, 0x0000067B, 0x0004003D, 0x00000007, 0x0000067D, 0x0000065D, 
0x00050085, 0x00000007, 0x0000067E, 0x0000067D, 0x00000242, 0x00060050, 0x00000008, 0x0000067F, 0x0000012A, 0x0000067C, 0x0000067E, 0x0003003E, 
0x000005BE, 0x0000067F, 0x000200F9, 0x00000659, 0x000200F8, 0x00000680, 0x00040008, 0x00000001, 0x00000280, 0x00000000, 0x0004003D, 0x00000007, 
0x00000681, 0x00000499, 0x000500B8, 0x000002C0, 0x00000683, 0x00000681, 0x00000682, 0x000300F7, 0x00000685, 0x00000000, 0x000400FA, 0x00000683, 
0x00000684, 0x000006B6, 0x000200F8, 0x00000684, 0x00040008, 0x00000001, 0x00000283, 0x00000000, 0x0004003D, 0x00000007, 0x00000687, 0x00000499, 
0x00050083, 0x00000007, 0x00000688, 0x00000687, 0x00000656, 0x0003003E, 0x00000686, 0x00000688, 0x00040008, 0x00000001, 0x00000285, 0x00000000, 
0x0004003D, 0x00000007, 0x0000068A, 0x00000686, 0x00050088, 0x00000007, 0x0000068C, 0x0000068A, 0x0000068B, 0x0003003E, 0x00000689, 0x0000068C, 
0x00040008, 0x00000001, 0x00000286, 0x00000000, 0x0004003D, 0x00000007, 0x0000068E, 0x00000686, 0x0003003E, 0x0000068D, 0x0000068E, 0x00050039, 
0x00000007, 0x0000068F, 0x0000003C, 0x0000068D, 0x0003003E, 0x000005BA, 0x0000068F, 0x00040008, 0x00000001, 0x00000287, 0x00000000, 0x0004003D, 
0x00000007, 0x00000690, 0x00000499, 0x00050083, 0x00000007, 0x00000691, 0x00000682, 0x00000690, 0x0003003E, 0x00000692, 0x00000691, 0x00050039, 
0x00000007, 0x00000693, 0x0000003C, 0x00000692, 0x0004003D, 0x00000007, 0x00000694, 0x000005BA, 0x00050085, 0x00000007, 0x00000695, 0x00000694, 
0x00000693, 0x0003003E, 0x000005BA, 0x00000695, 0x00040008, 0x00000001, 0x00000288, 0x00000000, 0x0004003D, 0x00000007, 0x00000697, 0x00000689, 
0x00050085, 0x00000007, 0x00000699, 0x00000697, 0x00000698, 0x00050083, 0x00000007, 0x0000069A, 0x00000547, 0x00000699, 0x0004003D, 0x00000007, 
0x0000069C, 0x00000689, 0x00050085, 0x00000007, 0x0000069D, 0x0000069C, 0x000001CC, 0x00050083, 0x00000007, 0x0000069E, 0x0000069B, 0x0000069D, 
0x00060050, 0x00000008, 0x0000069F, 0x00000696, 0x0000069A, 0x0000069E, 0x0003003E, 0x000005BB, 0x0000069F, 0x00040008, 0x00000001, 0x00000289, 
0x00000000, 0x0003003E, 0x000005BD, 0x00000165, 0x00040008, 0x00000001, 0x0000028A, 0x00000000, 0x0004003D, 0x00000007, 0x000006A0, 0x00000689, 
0x00050081, 0x00000007, 0x000006A1, 0x0000041A, 0x000006A0, 0x00060050, 0x00000008, 0x000006A3, 0x00000696, 0x000006A1, 0x000006A2, 0x0003003E, 
0x000005BE, 0x000006A3, 0x00040008, 0x00000001, 0x0000028B, 0x00000000, 0x0004003D, 0x00000007, 0x000006A4, 0x00000689, 0x00050085, 0x00000007, 
0x000006A5, 0x000006A4, 0x00000273, 0x00050083, 0x00000007, 0x000006A6, 0x000001CC, 0x000006A5, 0x0004003D, 0x00000007, 0x000006A7, 0x00000689, 
0x00050085, 0x00000007, 0x000006A8, 0x000006A7, 0x0000041A, 0x00050083, 0x00000007, 0x000006A9, 0x0000041A, 0x000006A8, 0x00060050, 0x00000008, 
0x000006AA, 0x000006A6, 0x000006A9, 0x00000129, 0x0006000C, 0x00000008, 0x000006AB, 0x00000002, 0x00000045, 0x000006AA, 0x0003003E, 0x000005B0, 
0x000006AB, 0x00040008, 0x00000001, 0x0000028C, 0x00000000, 0x0003003E, 0x000001E1, 0x000006AE, 0x00040008, 0x00000001, 0x0000028D, 0x00000000, 
0x0004003D, 0x00000007, 0x000006B0, 0x00000499, 0x00050085, 0x00000007, 0x000006B1, 0x000006B0, 0x000006AF, 0x0003003E, 0x00000499, 0x000006B1, 
0x00040008, 0x00000001, 0x0000028E, 0x00000000, 0x0004003D, 0x00000007, 0x000006B2, 0x000005B9, 0x00050085, 0x00000007, 0x000006B3, 0x000006B2, 
0x000003AD, 0x0003003E, 0x000005B9, 0x000006B3, 0x00040008, 0x00000001, 0x0000028F, 0x00000000, 0x0003003E, 0x000001EA, 0x000006B4, 0x00040008, 
0x00000001, 0x00000290, 0x00000000, 0x0003003E, 0x000001EC, 0x000006B5, 0x000200F9, 0x00000685, 0x000200F8, 0x000006B6, 0x00040008, 0x00000001, 
0x00000295, 0x00000000, 0x0004003D, 0x00000007, 0x000006B7, 0x00000499, 0x000500B8, 0x000002C0, 0x000006B8, 0x000006B7, 0x000005CC, 0x000300F7, 
0x000006BA, 0x00000000, 0x000400FA, 0x000006B8, 0x000006B9, 0x000006BA, 0x000200F8, 0x000006B9, 0x00040008, 0x00000001, 0x00000297, 0x00000000, 
0x0003003E, 0x000005BA, 0x0000012A, 0x00040008, 0x00000001, 0x00000298, 0x00000000, 0x0003003E, 0x000005BB, 0x000006BC, 0x00040008, 0x00000001, 
0x00000299, 0x00000000, 0x0003003E, 0x000005BD, 0x00000165, 0x00040008, 0x00000001, 0x0000029A, 0x00000000, 0x0003003E, 0x000005BE, 0x000006BD, 
0x000200F9, 0x000006BA, 0x000200F8, 0x000006BA, 0x000200F9, 0x00000685, 0x000200F8, 0x00000685, 0x000200F9, 0x00000659, 0x000200F8, 0x00000659, 
0x000200F9, 0x00000628, 0x000200F8, 0x00000628, 0x000200F9, 0x000005F5, 0x000200F8, 0x000005F5, 0x000200F9, 0x000005D3, 0x000200F8, 0x000005D3, 
0x00040008, 0x00000001, 0x000002A9, 0x00000000, 0x0004003D, 0x00000008, 0x000006BF, 0x000005BE, 0x0004003D, 0x00000008, 0x000006C0, 0x000005BB, 
0x00050083, 0x00000008, 0x000006C1, 0x000006BF, 0x000006C0, 0x0006000C, 0x00000008, 0x000006C2, 0x00000002, 0x00000045, 0x000006C1, 0x0003003E, 
0x000006BE, 0x000006C2, 0x00040008, 0x00000001, 0x000002AB, 0x00000000, 0x0004003D, 0x00000008, 0x000006C4, 0x000005BD, 0x0004003D, 0x00000008, 
0x000006C5, 0x000006BE, 0x0007000C, 0x00000008, 0x000006C6, 0x00000002, 0x00000044, 0x000006C4, 0x000006C5, 0x0006000C, 0x00000008, 0x000006C7, 
0x00000002, 0x00000045, 0x000006C6, 0x0003003E, 0x000006C3, 0x000006C7, 0x00040008, 0x00000001, 0x000002AD, 0x00000000, 0x0004003D, 0x00000008, 
0x000006C9, 0x000006BE, 0x0004003D, 0x00000008, 0x000006CA, 0x000006C3, 0x0007000C, 0x00000008, 0x000006CB, 0x00000002, 0x00000044, 0x000006C9, 
0x000006CA, 0x0003003E, 0x000006C8, 0x000006CB, 0x00040008, 0x00000001, 0x000002AF, 0x00000000, 0x0004003D, 0x00000008, 0x000006CD, 0x000005BB, 
0x0004003D, 0x00000008, 0x000006CE, 0x000006BE, 0x00050081, 0x00000008, 0x000006CF, 0x000006CD, 0x000006CE, 0x0003003E, 0x000006CC, 0x000006CF, 
0x00040008, 0x00000001, 0x000002B1, 0x00000000, 0x0004003D, 0x00000008, 0x000006D1, 0x000006CC, 0x00050041, 0x00000015, 0x000006D2, 0x000005BF, 
0x000000A9, 0x0004003D, 0x00000007, 0x000006D3, 0x000006D2, 0x0004003D, 0x00000008, 0x000006D4, 0x000006C3, 0x0005008E, 0x00000008, 0x000006D5, 
0x000006D4, 0x000006D3, 0x00060041, 0x00000099, 0x000006D6, 0x00000096, 0x00000097, 0x000000A9, 0x0004003D, 0x00000007, 0x000006D7, 0x000006D6, 
0x00060041, 0x00000099, 0x000006D8, 0x00000096, 0x00000097, 0x00000098, 0x0004003D, 0x00000007, 0x000006D9, 0x000006D8, 0x00050088, 0x00000007, 
0x000006DA, 0x000006D7, 0x000006D9, 0x0005008E, 0x00000008, 0x000006DB, 0x000006D5, 0x000006DA, 0x00050081, 0x00000008, 0x000006DC, 0x000006D1, 
0x000006DB, 0x00050041, 0x00000015, 0x000006DD, 0x000005BF, 0x00000098, 0x0004003D, 0x00000007, 0x000006DE, 0x000006DD, 0x0004003D, 0x00000008, 
0x000006DF, 0x000006C8, 0x0005008E, 0x00000008, 0x000006E0, 0x000006DF, 0x000006DE, 0x00050081, 0x00000008, 0x000006E1, 0x000006DC, 0x000006E0, 
0x0003003E, 0x000006D0, 0x000006E1, 0x00040008, 0x00000001, 0x000002B3, 0x00000000, 0x00050051, 0x00000008, 0x000006E3, 0x0000007F, 0x00000002, 
0x0003003E, 0x000006E2, 0x000006E3, 0x00040008, 0x00000001, 0x000002B5, 0x00000000, 0x0003003E, 0x000006E4, 0x000006E5, 0x00040008, 0x00000001, 
0x000002B7, 0x00000000, 0x0003003E, 0x000006E6, 0x000001D1, 0x00040008, 0x00000001, 0x000002B9, 0x00000000, 0x0003003E, 0x000006E7, 0x000005BC, 
0x00040008, 0x00000001, 0x000002BC, 0x00000000, 0x0003003E, 0x000006E9, 0x00000097, 0x000200F9, 0x000006EA, 0x00040008, 0x00000001, 0x000002BD, 
0x00000000, 0x000200F8, 0x000006EA, 0x000400F6, 0x000006EC, 0x000006ED, 0x00000000, 0x000200F9, 0x000006EE, 0x000200F8, 0x000006EE, 0x0004003D, 
0x0000008E, 0x000006EF, 0x000006E9, 0x000500B1, 0x000002C0, 0x000006F1, 0x000006EF, 0x000006F0, 0x000400FA, 0x000006F1, 0x000006EB, 0x000006EC, 
0x000200F8, 0x000006EB, 0x00040008, 0x00000001, 0x000002BF, 0x00000000, 0x0004003D, 0x00000007, 0x000006F2, 0x000005AB, 0x00050081, 0x00000007, 
0x000006F3, 0x000006F2, 0x00000129, 0x0003003E, 0x000005AB, 0x000006F3, 0x00040008, 0x00000001, 0x000002C0, 0x00000000, 0x0004003D, 0x00000008, 
0x000006F4, 0x000005BB, 0x0004003D, 0x00000008, 0x000006F5, 0x000006E2, 0x0004003D, 0x00000007, 0x000006F6, 0x000006E6, 0x0005008E, 0x00000008, 
0x000006F7, 0x000006F5, 0x000006F6, 0x00050081, 0x00000008, 0x000006F8, 0x000006F4, 0x000006F7, 0x0003003E, 0x000006E7, 0x000006F8, 0x00040008, 
0x00000001, 0x000002C1, 0x00000000, 0x0004003D, 0x00000008, 0x000006FA, 0x000006E7, 0x0003003E, 0x000006F9, 0x000006FA, 0x00050039, 0x00000009, 
0x000006FB, 0x00000073, 0x000006F9, 0x0003003E, 0x000006E4, 0x000006FB, 0x00040008, 0x00000001, 0x000002C3, 0x00000000, 0x00050041, 0x00000015, 
0x000006FD, 0x000006E4, 0x000000A9, 0x0004003D, 0x00000007, 0x000006FE, 0x000006FD, 0x0003003E, 0x000006FC, 0x000006FE, 0x00040008, 0x00000001, 
0x000002C5, 0x00000000, 0x00050041, 0x00000015, 0x00000700, 0x000006E7, 0x000000A9, 0x0004003D, 0x00000007, 0x00000701, 0x00000700, 0x0006000C, 
0x00000007, 0x00000702, 0x00000002, 0x0000000A, 0x00000701, 0x0004007F, 0x00000007, 0x00000703, 0x00000702, 0x0003003E, 0x000006FF, 0x00000703, 
0x00040008, 0x00000001, 0x000002C6, 0x00000000, 0x00050041, 0x00000015, 0x00000704, 0x000006E2, 0x000000A9, 0x0004003D, 0x00000007, 0x00000705, 
0x00000704, 0x000500BA, 0x000002C0, 0x00000706, 0x00000705, 0x0000012A, 0x000300F7, 0x00000708, 0x00000000, 0x000400FA, 0x00000706, 0x00000707, 
0x00000708, 0x000200F8, 0x00000707, 0x00040008, 0x00000001, 0x000002C8, 0x00000000, 0x00050041, 0x00000015, 0x00000709, 0x000006E7, 0x000000A9, 
0x0004003D, 0x00000007, 0x0000070A, 0x00000709, 0x0004007F, 0x00000007, 0x0000070B, 0x0000070A, 0x0006000C, 0x00000007, 0x0000070C, 0x00000002, 
0x0000000A, 0x0000070B, 0x0003003E, 0x000006FF, 0x0000070C, 0x000200F9, 0x00000708, 0x000200F8, 0x00000708, 0x00040008, 0x00000001, 0x000002CC, 
0x00000000, 0x00050041, 0x00000015, 0x0000070E, 0x000006E7, 0x00000102, 0x0004003D, 0x00000007, 0x0000070F, 0x0000070E, 0x0006000C, 0x00000007, 
0x00000710, 0x00000002, 0x0000000A, 0x0000070F, 0x0004007F, 0x00000007, 0x00000711, 0x00000710, 0x0003003E, 0x0000070D, 0x00000711, 0x00040008, 
0x00000001, 0x000002CD, 0x00000000, 0x00050041, 0x00000015, 0x00000712, 0x000006E2, 0x00000102, 0x0004003D, 0x00000007, 0x00000713, 0x00000712, 
0x000500BA, 0x000002C0, 0x00000714, 0x00000713, 0x0000012A, 0x000300F7, 0x00000716, 0x00000000, 0x000400FA, 0x00000714, 0x00000715, 0x00000716, 
0x000200F8, 0x00000715, 0x00040008, 0x00000001, 0x000002CF, 0x00000000, 0x00050041, 0x00000015, 0x00000717, 0x000006E7, 0x00000102, 0x0004003D, 
0x00000007, 0x00000718, 0x00000717, 0x0004007F, 0x00000007, 0x00000719, 0x00000718, 0x0006000C, 0x00000007, 0x0000071A, 0x00000002, 0x0000000A, 
0x00000719, 0x0003003E, 0x0000070D, 0x0000071A, 0x000200F9, 0x00000716, 0x000200F8, 0x00000716, 0x00040008, 0x00000001, 0x000002D3, 0x00000000, 
0x0004003D, 0x00000007, 0x0000071C, 0x000006FF, 0x00050041, 0x00000015, 0x0000071D, 0x000006E2, 0x000000A9, 0x0004003D, 0x00000007, 0x0000071E, 
0x0000071D, 0x00050088, 0x00000007, 0x0000071F, 0x0000071C, 0x0000071E, 0x0006000C, 0x00000007, 0x00000720, 0x00000002, 0x0000000A, 0x0000071F, 
0x0004003D, 0x00000007, 0x00000721, 0x0000070D, 0x00050041, 0x00000015, 0x00000722, 0x000006E2, 0x00000102, 0x0004003D, 0x00000007, 0x00000723, 
0x00000722, 0x00050088, 0x00000007, 0x00000724, 0x00000721, 0x00000723, 0x0006000C, 0x00000007, 0x00000725, 0x00000002, 0x0000000A, 0x00000724, 
0x0007000C, 0x00000007, 0x00000726, 0x00000002, 0x00000025, 0x00000720, 0x00000725, 0x0004003D, 0x00000007, 0x00000728, 0x00000727, 0x00050081, 
0x00000007, 0x00000729, 0x00000726, 0x00000728, 0x0003003E, 0x0000071B, 0x00000729, 0x00040008, 0x00000001, 0x000002D4, 0x00000000, 0x0004003D, 
0x00000007, 0x0000072A, 0x00000727, 0x0004003D, 0x00000007, 0x0000072B, 0x0000071B, 0x0007000C, 0x00000007, 0x0000072C, 0x00000002, 0x00000028, 
0x0000072A, 0x0000072B, 0x0003003E, 0x0000071B, 0x0000072C, 0x00040008, 0x00000001, 0x000002D5, 0x00000000, 0x0004003D, 0x00000007, 0x0000072D, 
0x000006FC, 0x0004003D, 0x00000007, 0x0000072E, 0x0000071B, 0x0007000C, 0x00000007, 0x0000072F, 0x00000002, 0x00000025, 0x0000072D, 0x0000072E, 
0x0003003E, 0x000006FC, 0x0000072F, 0x00040008, 0x00000001, 0x000002D6, 0x00000000, 0x0004003D, 0x00000007, 0x00000730, 0x000006FC, 0x0004003D, 
0x00000007, 0x00000731, 0x000006E6, 0x00050081, 0x00000007, 0x00000732, 0x00000731, 0x00000730, 0x0003003E, 0x000006E6, 0x00000732, 0x00040008, 
0x00000001, 0x000002D7, 0x00000000, 0x0004003D, 0x00000007, 0x00000733, 0x000006E6, 0x000500BA, 0x000002C0, 0x00000735, 0x00000733, 0x00000734, 
0x000400A8, 0x000002C0, 0x00000736, 0x00000735, 0x000300F7, 0x00000738, 0x00000000, 0x000400FA, 0x00000736, 0x00000737, 0x00000738, 0x000200F8, 
0x00000737, 0x00050041, 0x00000015, 0x00000739, 0x000006E4, 0x000000A9, 0x0004003D, 0x00000007, 0x0000073A, 0x00000739, 0x0006000C, 0x00000007, 
0x0000073B, 0x00000002, 0x00000004, 0x0000073A, 0x000500B8, 0x000002C0, 0x0000073D, 0x0000073B, 0x0000073C, 0x000200F9, 0x00000738, 0x000200F8, 
0x00000738, 0x000700F5, 0x000002C0, 0x0000073E, 0x00000735, 0x00000716, 0x0000073D, 0x00000737, 0x000300F7, 0x00000740, 0x00000000, 0x000400FA, 
0x0000073E, 0x0000073F, 0x00000740, 0x000200F8, 0x0000073F, 0x00040008, 0x00000001, 0x000002D9, 0x00000000, 0x000200F9, 0x000006EC, 0x000200F8, 
0x00000740, 0x000200F9, 0x000006ED, 0x000200F8, 0x000006ED, 0x00040008, 0x00000001, 0x000002BD, 0x00000000, 0x0004003D, 0x0000008E, 0x00000742, 
0x000006E9, 0x00050080, 0x0000008E, 0x00000743, 0x00000742, 0x000000BD, 0x0003003E, 0x000006E9, 0x00000743, 0x000200F9, 0x000006EA, 0x000200F8, 
0x000006EC, 0x00040008, 0x00000001, 0x000002E0, 0x00000000, 0x00050041, 0x00000015, 0x00000745, 0x000005BB, 0x00000098, 0x0004003D, 0x00000007, 
0x00000746, 0x00000745, 0x0004007F, 0x00000007, 0x00000747, 0x00000746, 0x00050041, 0x00000015, 0x00000748, 0x000006E2, 0x00000098, 0x0004003D, 
0x00000007, 0x00000749, 0x00000748, 0x00050088, 0x00000007, 0x0000074A, 0x00000747, 0x00000749, 0x0003003E, 0x00000744, 0x0000074A, 0x00040008, 
0x00000001, 0x000002E1, 0x00000000, 0x0004003D, 0x00000007, 0x0000074B, 0x000006E6, 0x000500BA, 0x000002C0, 0x0000074C, 0x0000074B, 0x00000734, 
0x000300F7, 0x0000074E, 0x00000000, 0x000400FA, 0x0000074C, 0x0000074D, 0x0000074E, 0x000200F8, 0x0000074D, 0x00050041, 0x00000015, 0x0000074F, 
0x000006E2, 0x00000098, 0x0004003D, 0x00000007, 0x00000750, 0x0000074F, 0x000500B8, 0x000002C0, 0x00000751, 0x00000750, 0x000006A2, 0x000200F9, 
0x0000074E, 0x000200F8, 0x0000074E, 0x000700F5, 0x000002C0, 0x00000752, 0x0000074C, 0x000006EC, 0x00000751, 0x0000074D, 0x000300F7, 0x00000754, 
0x00000000, 0x000400FA, 0x00000752, 0x00000753, 0x00000754, 0x000200F8, 0x00000753, 0x00040008, 0x00000001, 0x000002E3, 0x00000000, 0x0004003D, 
0x00000008, 0x00000755, 0x000005BB, 0x0007004F, 0x00000009, 0x00000756, 0x00000755, 0x00000755, 0x00000000, 0x00000002, 0x0004003D, 0x00000008, 
0x00000757, 0x000006E2, 0x0007004F, 0x00000009, 0x00000758, 0x00000757, 0x00000757, 0x00000000, 0x00000002, 0x0004003D, 0x00000007, 0x00000759, 
0x00000744, 0x0005008E, 0x00000009, 0x0000075A, 0x00000758, 0x00000759, 0x00050081, 0x00000009, 0x0000075B, 0x00000756, 0x0000075A, 0x0004003D, 
0x00000008, 0x0000075C, 0x000006E7, 0x0008004F, 0x00000008, 0x0000075D, 0x0000075C, 0x0000075B, 0x00000003, 0x00000001, 0x00000004, 0x0003003E, 
0x000006E7, 0x0000075D, 0x00040008, 0x00000001, 0x000002E4, 0x00000000, 0x00050041, 0x00000015, 0x0000075E, 0x000006E7, 0x00000098, 0x0003003E, 
0x0000075E, 0x000006A2, 0x00040008, 0x00000001, 0x000002E5, 0x00000000, 0x0004003D, 0x00000007, 0x0000075F, 0x00000744, 0x0003003E, 0x000006E6, 
0x0000075F, 0x00040008, 0x00000001, 0x000002E6, 0x00000000, 0x00050041, 0x00000015, 0x00000760, 0x000006E4, 0x00000098, 0x0003003E, 0x00000760, 
0x0000012A, 0x00040008, 0x00000001, 0x000002E7, 0x00000000, 0x00050041, 0x00000015, 0x00000761, 0x000006E4, 0x000000A9, 0x0003003E, 0x00000761, 
0x0000012A, 0x000200F9, 0x00000754, 0x000200F8, 0x00000754, 0x00040008, 0x00000001, 0x000002EC, 0x00000000, 0x0003003E, 0x00000762, 0x000005BC, 
0x00040008, 0x00000001, 0x000002ED, 0x00000000, 0x0004003D, 0x00000007, 0x00000763, 0x000006E6, 0x000500BC, 0x000002C0, 0x00000764, 0x00000763, 
0x00000734, 0x0004003D, 0x00000007, 0x00000765, 0x000006E6, 0x0004003D, 0x00000007, 0x00000766, 0x00000744, 0x000500B4, 0x000002C0, 0x00000767, 
0x00000765, 0x00000766, 0x000500A6, 0x000002C0, 0x00000768, 0x00000764, 0x00000767, 0x000300F7, 0x0000076A, 0x00000000, 0x000400FA, 0x00000768, 
0x00000769, 0x00000B2C, 0x000200F8, 0x00000769, 0x00040008, 0x00000001, 0x000002F0, 0x00000000, 0x00050041, 0x00000015, 0x0000076C, 0x000006E4, 
0x000000A9, 0x0004003D, 0x00000007, 0x0000076D, 0x0000076C, 0x0003003E, 0x0000076B, 0x0000076D, 0x00040008, 0x00000001, 0x000002F2, 0x00000000, 
0x0003003E, 0x0000076E, 0x0000076F, 0x00040008, 0x00000001, 0x000002F4, 0x00000000, 0x0004003D, 0x00000007, 0x00000771, 0x0000076B, 0x0004003D, 
0x00000008, 0x00000772, 0x000006E7, 0x0004003D, 0x00000008, 0x00000773, 0x0000076E, 0x0008004F, 0x00000008, 0x00000774, 0x00000773, 0x00000773, 
0x00000000, 0x00000001, 0x00000001, 0x00050083, 0x00000008, 0x00000775, 0x00000772, 0x00000774, 0x0003003E, 0x00000776, 0x00000775, 0x00050039, 
0x00000009, 0x00000777, 0x00000073, 0x00000776, 0x00050051, 0x00000007, 0x00000778, 0x00000777, 0x00000000, 0x00050083, 0x00000007, 0x00000779, 
0x00000771, 0x00000778, 0x0004003D, 0x00000007, 0x0000077A, 0x0000076B, 0x0004003D, 0x00000008, 0x0000077B, 0x000006E7, 0x0004003D, 0x00000008, 
0x0000077C, 0x0000076E, 0x0008004F, 0x00000008, 0x0000077D, 0x0000077C, 0x0000077C, 0x00000001, 0x00000000, 0x00000001, 0x00050083, 0x00000008, 
0x0000077E, 0x0000077B, 0x0000077D, 0x0003003E, 0x0000077F, 0x0000077E, 0x00050039, 0x00000009, 0x00000780, 0x00000073, 0x0000077F, 0x00050051, 
0x00000007, 0x00000781, 0x00000780, 0x00000000, 0x00050083, 0x00000007, 0x00000782, 0x0000077A, 0x00000781, 0x0004003D, 0x00000007, 0x00000783, 
0x0000076B, 0x0004003D, 0x00000008, 0x00000784, 0x000006E7, 0x0004003D, 0x00000008, 0x00000785, 0x0000076E, 0x0008004F, 0x00000008, 0x00000786, 
0x00000785, 0x00000785, 0x00000001, 0x00000001, 0x00000000, 0x00050083, 0x00000008, 0x00000787, 0x00000784, 0x00000786, 0x0003003E, 0x00000788, 
0x00000787, 0x00050039, 0x00000009, 0x00000789, 0x00000073, 0x00000788, 0x00050051, 0x00000007, 0x0000078A, 0x00000789, 0x00000000, 0x00050083, 
0x00000007, 0x0000078B, 0x00000783, 0x0000078A, 0x00060050, 0x00000008, 0x0000078C, 0x00000779, 0x00000782, 0x0000078B, 0x0003003E, 0x00000770, 
0x0000078C, 0x00040008, 0x00000001, 0x000002F6, 0x00000000, 0x0004003D, 0x00000008, 0x0000078E, 0x00000770, 0x0006000C, 0x00000008, 0x0000078F, 
0x00000002, 0x00000045, 0x0000078E, 0x0003003E, 0x0000078D, 0x0000078F, 0x00040008, 0x00000001, 0x000002F8, 0x00000000, 0x0003003E, 0x00000790, 
0x00000129, 0x00040008, 0x00000001, 0x000002F9, 0x00000000, 0x0004003D, 0x00000008, 0x00000791, 0x000006E7, 0x0004003D, 0x00000008, 0x00000792, 
0x0000078D, 0x0005008E, 0x00000008, 0x00000793, 0x00000792, 0x000003EA, 0x00050081, 0x00000008, 0x00000794, 0x00000791, 0x00000793, 0x0003003E, 
0x00000795, 0x00000794, 0x00050039, 0x00000009, 0x00000796, 0x00000073, 0x00000795, 0x00050051, 0x00000007, 0x00000797, 0x00000796, 0x00000000, 
0x00050085, 0x00000007, 0x00000798, 0x00000797, 0x0000058E, 0x0003003E, 0x00000799, 0x00000798, 0x00050039, 0x00000007, 0x0000079A, 0x0000003C, 
0x00000799, 0x0004003D, 0x00000007, 0x0000079B, 0x00000790, 0x00050085, 0x00000007, 0x0000079C, 0x0000079B, 0x0000079A, 0x0003003E, 0x00000790, 
0x0000079C, 0x00040008, 0x00000001, 0x000002FA, 0x00000000, 0x0004003D, 0x00000008, 0x0000079D, 0x000006E7, 0x0004003D, 0x00000008, 0x0000079E, 
0x0000078D, 0x0005008E, 0x00000008, 0x0000079F, 0x0000079E, 0x000001E5, 0x00050081, 0x00000008, 0x000007A0, 0x0000079D, 0x0000079F, 0x0003003E, 
0x000007A1, 0x000007A0, 0x00050039, 0x00000009, 0x000007A2, 0x00000073, 0x000007A1, 0x00050051, 0x00000007, 0x000007A3, 0x000007A2, 0x00000000, 
0x00050085, 0x00000007, 0x000007A4, 0x000007A3, 0x00000551, 0x0003003E, 0x000007A5, 0x000007A4, 0x00050039, 0x00000007, 0x000007A6, 0x0000003C, 
0x000007A5, 0x0004003D, 0x00000007, 0x000007A7, 0x00000790, 0x00050085, 0x00000007, 0x000007A8, 0x000007A7, 0x000007A6, 0x0003003E, 0x00000790, 
0x000007A8, 0x00040008, 0x00000001, 0x000002FB, 0x00000000, 0x0004003D, 0x00000008, 0x000007A9, 0x000006E7, 0x0004003D, 0x00000008, 0x000007AA, 
0x0000078D, 0x0005008E, 0x00000008, 0x000007AB, 0x000007AA, 0x000001D1, 0x00050081, 0x00000008, 0x000007AC, 0x000007A9, 0x000007AB, 0x0003003E, 
0x000007AD, 0x000007AC, 0x00050039, 0x00000009, 0x000007AE, 0x00000073, 0x000007AD, 0x00050051, 0x00000007, 0x000007AF, 0x000007AE, 0x00000000, 
0x00050085, 0x00000007, 0x000007B0, 0x000007AF, 0x0000035D, 0x0003003E, 0x000007B1, 0x000007B0, 0x00050039, 0x00000007, 0x000007B2, 0x0000003C, 
0x000007B1, 0x0004003D, 0x00000007, 0x000007B3, 0x00000790, 0x00050085, 0x00000007, 0x000007B4, 0x000007B3, 0x000007B2, 0x0003003E, 0x00000790, 
0x000007B4, 0x00040008, 0x00000001, 0x000002FC, 0x00000000, 0x0004003D, 0x00000008, 0x000007B5, 0x000006E7, 0x0004003D, 0x00000008, 0x000007B6, 
0x0000078D, 0x0005008E, 0x00000008, 0x000007B7, 0x000007B6, 0x00000285, 0x00050081, 0x00000008, 0x000007B8, 0x000007B5, 0x000007B7, 0x0003003E, 
0x000007B9, 0x000007B8, 0x00050039, 0x00000009, 0x000007BA, 0x00000073, 0x000007B9, 0x00050051, 0x00000007, 0x000007BB, 0x000007BA, 0x00000000, 
0x00050085, 0x00000007, 0x000007BC, 0x000007BB, 0x0000023A, 0x0003003E, 0x000007BD, 0x000007BC, 0x00050039, 0x00000007, 0x000007BE, 0x0000003C, 
0x000007BD, 0x0004003D, 0x00000007, 0x000007BF, 0x00000790, 0x00050085, 0x00000007, 0x000007C0, 0x000007BF, 0x000007BE, 0x0003003E, 0x00000790, 
0x000007C0, 0x00040008, 0x00000001, 0x000002FD, 0x00000000, 0x0004003D, 0x00000008, 0x000007C1, 0x000006E7, 0x0004003D, 0x00000008, 0x000007C2, 
0x0000078D, 0x0005008E, 0x00000008, 0x000007C3, 0x000007C2, 0x0000022E, 0x00050081, 0x00000008, 0x000007C4, 0x000007C1, 0x000007C3, 0x0003003E, 
0x000007C5, 0x000007C4, 0x00050039, 0x00000009, 0x000007C6, 0x00000073, 0x000007C5, 0x00050051, 0x00000007, 0x000007C7, 0x000007C6, 0x00000000, 
0x00050085, 0x00000007, 0x000007C8, 0x000007C7, 0x00000242, 0x0003003E, 0x000007C9, 0x000007C8, 0x00050039, 0x00000007, 0x000007CA, 0x0000003C, 
0x000007C9, 0x0004003D, 0x00000007, 0x000007CB, 0x00000790, 0x00050085, 0x00000007, 0x000007CC, 0x000007CB, 0x000007CA, 0x0003003E, 0x00000790, 
0x000007CC, 0x00040008, 0x00000001, 0x000002FE, 0x00000000, 0x0004003D, 0x00000008, 0x000007CD, 0x000006E7, 0x0004003D, 0x00000008, 0x000007CE, 
0x0000078D, 0x0005008E, 0x00000008, 0x000007CF, 0x000007CE, 0x000003AD, 0x00050081, 0x00000008, 0x000007D0, 0x000007CD, 0x000007CF, 0x0003003E, 
0x000007D1, 0x000007D0, 0x00050039, 0x00000009, 0x000007D2, 0x00000073, 0x000007D1, 0x00050051, 0x00000007, 0x000007D3, 0x000007D2, 0x00000000, 
0x00050085, 0x00000007, 0x000007D4, 0x000007D3, 0x0000028F, 0x0003003E, 0x000007D5, 0x000007D4, 0x00050039, 0x00000007, 0x000007D6, 0x0000003C, 
0x000007D5, 0x0004003D, 0x00000007, 0x000007D7, 0x00000790, 0x00050085, 0x00000007, 0x000007D8, 0x000007D7, 0x000007D6, 0x0003003E, 0x00000790, 
0x000007D8, 0x00040008, 0x00000001, 0x00000300, 0x00000000, 0x0004003D, 0x00000007, 0x000007DA, 0x00000790, 0x0003003E, 0x000007D9, 0x000007DA, 
0x00040008, 0x00000001, 0x00000301, 0x00000000, 0x0004003D, 0x00000007, 0x000007DB, 0x000007D9, 0x0007000C, 0x00000007, 0x000007DC, 0x00000002, 
0x0000001A, 0x000007DB, 0x000001CC, 0x0007000C, 0x00000007, 0x000007DD, 0x00000002, 0x00000028, 0x000001E5, 0x000007DC, 0x0003003E, 0x000007D9, 
0x000007DD, 0x00040008, 0x00000001, 0x00000302, 0x00000000, 0x0004003D, 0x00000007, 0x000007DF, 0x000007D9, 0x0003003E, 0x000007DE, 0x000007DF, 
0x00050039, 0x00000007, 0x000007E0, 0x0000003C, 0x000007DE, 0x0003003E, 0x000007D9, 0x000007E0, 0x00040008, 0x00000001, 0x00000304, 0x00000000, 
0x0004003D, 0x00000008, 0x000007E2, 0x000006E2, 0x0004003D, 0x00000008, 0x000007E3, 0x0000078D, 0x0007000C, 0x00000008, 0x000007E4, 0x00000002, 
0x00000047, 0x000007E2, 0x000007E3, 0x0003003E, 0x000007E1, 0x000007E4, 0x00040008, 0x00000001, 0x00000306, 0x00000000, 0x0003003E, 0x000007E5, 
0x00000129, 0x00040008, 0x00000001, 0x00000308, 0x00000000, 0x0003003E, 0x000007E6, 0x0000030B, 0x00040008, 0x00000001, 0x0000030A, 0x00000000, 
0x0004003D, 0x00000008, 0x000007E8, 0x000006E7, 0x0004003D, 0x00000008, 0x000007E9, 0x0000078D, 0x0005008E, 0x00000008, 0x000007EB, 0x000007E9, 
0x000007EA, 0x00050081, 0x00000008, 0x000007EC, 0x000007E8, 0x000007EB, 0x0003003E, 0x000007E7, 0x000007EC, 0x00040008, 0x00000001, 0x0000030D, 
0x00000000, 0x0003003E, 0x000007ED, 0x00000097, 0x000200F9, 0x000007EE, 0x00040008, 0x00000001, 0x0000030E, 0x00000000, 0x000200F8, 0x000007EE, 
0x000400F6, 0x000007F0, 0x000007F1, 0x00000000, 0x000200F9, 0x000007F2, 0x000200F8, 0x000007F2, 0x0004003D, 0x0000008E, 0x000007F3, 0x000007ED, 
0x000500B1, 0x000002C0, 0x000007F5, 0x000007F3, 0x000007F4, 0x000400FA, 0x000007F5, 0x000007EF, 0x000007F0, 0x000200F8, 0x000007EF, 0x00040008, 
0x00000001, 0x00000311, 0x00000000, 0x0004003D, 0x00000008, 0x000007F7, 0x000007E7, 0x0004003D, 0x00000008, 0x000007F8, 0x000005B0, 0x0004003D, 
0x00000007, 0x000007F9, 0x000007E6, 0x0005008E, 0x00000008, 0x000007FA, 0x000007F8, 0x000007F9, 0x00050081, 0x00000008, 0x000007FB, 0x000007F7, 
0x000007FA, 0x0003003E, 0x000007F6, 0x000007FB, 0x00040008, 0x00000001, 0x00000313, 0x00000000, 0x0004003D, 0x00000008, 0x000007FE, 0x000007F6, 
0x0003003E, 0x000007FD, 0x000007FE, 0x00050039, 0x00000009, 0x000007FF, 0x00000073, 0x000007FD, 0x00050051, 0x00000007, 0x00000800, 0x000007FF, 
0x00000000, 0x0003003E, 0x000007FC, 0x00000800, 0x00040008, 0x00000001, 0x00000314, 0x00000000, 0x0004003D, 0x00000007, 0x00000801, 0x000007FC, 
0x00050085, 0x00000007, 0x00000802, 0x00000801, 0x000001D6, 0x0003003E, 0x00000803, 0x00000802, 0x00050039, 0x00000007, 0x00000804, 0x0000003C, 
0x00000803, 0x0004003D, 0x00000007, 0x00000805, 0x000007E5, 0x00050085, 0x00000007, 0x00000806, 0x00000805, 0x00000804, 0x0003003E, 0x000007E5, 
0x00000806, 0x00040008, 0x00000001, 0x00000315, 0x00000000, 0x0004003D, 0x00000007, 0x00000807, 0x000007FC, 0x000500BC, 0x000002C0, 0x00000808, 
0x00000807, 0x0000012A, 0x000300F7, 0x0000080A, 0x00000000, 0x000400FA, 0x00000808, 0x00000809, 0x0000080A, 0x000200F8, 0x00000809, 0x00040008, 
0x00000001, 0x00000317, 0x00000000, 0x000200F9, 0x000007F0, 0x000200F8, 0x0000080A, 0x00040008, 0x00000001, 0x0000031B, 0x00000000, 0x0004003D, 
0x00000007, 0x0000080D, 0x000007FC, 0x0003003E, 0x0000080C, 0x0000080D, 0x00040008, 0x00000001, 0x0000031D, 0x00000000, 0x00050041, 0x00000015, 
0x0000080F, 0x000007F6, 0x000000A9, 0x0004003D, 0x00000007, 0x00000810, 0x0000080F, 0x0006000C, 0x00000007, 0x00000811, 0x00000002, 0x0000000A, 
0x00000810, 0x0004007F, 0x00000007, 0x00000812, 0x00000811, 0x0003003E, 0x0000080E, 0x00000812, 0x00040008, 0x00000001, 0x0000031E, 0x00000000, 
0x00050041, 0x00000498, 0x00000813, 0x000005B0, 0x000000A9, 0x0004003D, 0x00000007, 0x00000814, 0x00000813, 0x000500BA, 0x000002C0, 0x00000815, 
0x00000814, 0x0000012A, 0x000300F7, 0x00000817, 0x00000000, 0x000400FA, 0x00000815, 0x00000816, 0x00000817, 0x000200F8, 0x00000816, 0x00040008, 
0x00000001, 0x00000320, 0x00000000, 0x00050041, 0x00000015, 0x00000818, 0x000007F6, 0x000000A9, 0x0004003D, 0x00000007, 0x00000819, 0x00000818, 
0x0004007F, 0x00000007, 0x0000081A, 0x00000819, 0x0006000C, 0x00000007, 0x0000081B, 0x00000002, 0x0000000A, 0x0000081A, 0x0003003E, 0x0000080E, 
0x0000081B, 0x000200F9, 0x00000817, 0x000200F8, 0x00000817, 0x00040008, 0x00000001, 0x00000324, 0x00000000, 0x00050041, 0x00000015, 0x0000081D, 
0x000007F6, 0x00000102, 0x0004003D, 0x00000007, 0x0000081E, 0x0000081D, 0x0006000C, 0x00000007, 0x0000081F, 0x00000002, 0x0000000A, 0x0000081E, 
0x0004007F, 0x00000007, 0x00000820, 0x0000081F, 0x0003003E, 0x0000081C, 0x00000820, 0x00040008, 0x00000001, 0x00000325, 0x00000000, 0x00050041, 
0x00000498, 0x00000821, 0x000005B0, 0x00000102, 0x0004003D, 0x00000007, 0x00000822, 0x00000821, 0x000500BA, 0x000002C0, 0x00000823, 0x00000822, 
0x0000012A, 0x000300F7, 0x00000825, 0x00000000, 0x000400FA, 0x00000823, 0x00000824, 0x00000825, 0x000200F8, 0x00000824, 0x00040008, 0x00000001, 
0x00000327, 0x00000000, 0x00050041, 0x00000015, 0x00000826, 0x000007F6, 0x00000102, 0x0004003D, 0x00000007, 0x00000827, 0x00000826, 0x0004007F, 
0x00000007, 0x00000828, 0x00000827, 0x0006000C, 0x00000007, 0x00000829, 0x00000002, 0x0000000A, 0x00000828, 0x0003003E, 0x0000081C, 0x00000829, 
0x000200F9, 0x00000825, 0x000200F8, 0x00000825, 0x00040008, 0x00000001, 0x0000032B, 0x00000000, 0x0004003D, 0x00000007, 0x0000082B, 0x0000080E, 
0x00050041, 0x00000498, 0x0000082C, 0x000005B0, 0x000000A9, 0x0004003D, 0x00000007, 0x0000082D, 0x0000082C, 0x00050088, 0x00000007, 0x0000082E, 
0x0000082B, 0x0000082D, 0x0006000C, 0x00000007, 0x0000082F, 0x00000002, 0x0000000A, 0x0000082E, 0x0004003D, 0x00000007, 0x00000830, 0x0000081C, 
0x00050041, 0x00000498, 0x00000831, 0x000005B0, 0x00000102, 0x0004003D, 0x00000007, 0x00000832, 0x00000831, 0x00050088, 0x00000007, 0x00000833, 
0x00000830, 0x00000832, 0x0006000C, 0x00000007, 0x00000834, 0x00000002, 0x0000000A, 0x00000833, 0x0007000C, 0x00000007, 0x00000835, 0x00000002, 
0x00000025, 0x0000082F, 0x00000834, 0x00050081, 0x00000007, 0x00000836, 0x00000835, 0x0000073C, 0x0003003E, 0x0000082A, 0x00000836, 0x00040008, 
0x00000001, 0x0000032C, 0x00000000, 0x0004003D, 0x00000007, 0x00000837, 0x0000082A, 0x0007000C, 0x00000007, 0x00000838, 0x00000002, 0x00000028, 
0x0000022E, 0x00000837, 0x0003003E, 0x0000082A, 0x00000838, 0x00040008, 0x00000001, 0x0000032D, 0x00000000, 0x0004003D, 0x00000007, 0x00000839, 
0x0000080C, 0x0004003D, 0x00000007, 0x0000083A, 0x0000082A, 0x0007000C, 0x00000007, 0x0000083B, 0x00000002, 0x00000025, 0x00000839, 0x0000083A, 
0x0003003E, 0x0000080C, 0x0000083B, 0x00040008, 0x00000001, 0x0000032E, 0x00000000, 0x0004003D, 0x00000007, 0x0000083C, 0x0000080C, 0x0007000C, 
0x00000007, 0x0000083D, 0x00000002, 0x00000028, 0x0000030B, 0x0000083C, 0x0004003D, 0x00000007, 0x0000083E, 0x000007E6, 0x00050081, 0x00000007, 
0x0000083F, 0x0000083E, 0x0000083D, 0x0003003E, 0x000007E6, 0x0000083F, 0x00040008, 0x00000001, 0x0000032F, 0x00000000, 0x0004003D, 0x00000007, 
0x00000840, 0x000007E6, 0x000500BA, 0x000002C0, 0x00000842, 0x00000840, 0x00000841, 0x000300F7, 0x00000844, 0x00000000, 0x000400FA, 0x00000842, 
0x00000843, 0x00000844, 0x000200F8, 0x00000843, 0x00040008, 0x00000001, 0x00000331, 0x00000000, 0x000200F9, 0x000007F0, 0x000200F8, 0x00000844, 
0x000200F9, 0x000007F1, 0x000200F8, 0x000007F1, 0x00040008, 0x00000001, 0x0000030E, 0x00000000, 0x0004003D, 0x0000008E, 0x00000846, 0x000007ED, 
0x00050080, 0x0000008E, 0x00000847, 0x00000846, 0x000000BD, 0x0003003E, 0x000007ED, 0x00000847, 0x000200F9, 0x000007EE, 0x000200F8, 0x000007F0, 
0x00040008, 0x00000001, 0x00000337, 0x00000000, 0x0004003D, 0x00000007, 0x00000849, 0x000007E5, 0x0003003E, 0x00000848, 0x00000849, 0x00050039, 
0x00000007, 0x0000084A, 0x0000003C, 0x00000848, 0x0003003E, 0x000007E5, 0x0000084A, 0x00040008, 0x00000001, 0x00000339, 0x00000000, 0x0003003E, 
0x0000084B, 0x0000012A, 0x00040008, 0x00000001, 0x0000033A, 0x00000000, 0x0004003D, 0x00000008, 0x0000084C, 0x000006E7, 0x0005008E, 0x00000008, 
0x0000084D, 0x0000084C, 0x00000299, 0x0003003E, 0x0000084E, 0x0000084D, 0x00050039, 0x00000007, 0x0000084F, 0x00000035, 0x0000084E, 0x0004003D, 
0x00000007, 0x00000850, 0x0000084B, 0x00050081, 0x00000007, 0x00000851, 0x00000850, 0x0000084F, 0x0003003E, 0x0000084B, 0x00000851, 0x00040008, 
0x00000001, 0x0000033B, 0x00000000, 0x0004003D, 0x00000008, 0x00000852, 0x000006E7, 0x0005008E, 0x00000008, 0x00000854, 0x00000852, 0x00000853, 
0x0003003E, 0x00000855, 0x00000854, 0x00050039, 0x00000007, 0x00000856, 0x00000035, 0x00000855, 0x0004003D, 0x00000007, 0x00000857, 0x0000084B, 
0x00050081, 0x00000007, 0x00000858, 0x00000857, 0x00000856, 0x0003003E, 0x0000084B, 0x00000858, 0x00040008, 0x00000001, 0x0000033C, 0x00000000, 
0x0004003D, 0x00000008, 0x00000859, 0x000006E7, 0x0005008E, 0x00000008, 0x0000085B, 0x00000859, 0x0000085A, 0x0003003E, 0x0000085C, 0x0000085B, 
0x00050039, 0x00000007, 0x0000085D, 0x00000035, 0x0000085C, 0x0004003D, 0x00000007, 0x0000085E, 0x0000084B, 0x00050081, 0x00000007, 0x0000085F, 
0x0000085E, 0x0000085D, 0x0003003E, 0x0000084B, 0x0000085F, 0x00040008, 0x00000001, 0x0000033D, 0x00000000, 0x0004003D, 0x00000008, 0x00000860, 
0x000006E7, 0x0005008E, 0x00000008, 0x00000861, 0x00000860, 0x000002A2, 0x0003003E, 0x00000862, 0x00000861, 0x00050039, 0x00000007, 0x00000863, 
0x00000035, 0x00000862, 0x0004003D, 0x00000007, 0x00000864, 0x0000084B, 0x00050081, 0x00000007, 0x00000865, 0x00000864, 0x00000863, 0x0003003E, 
0x0000084B, 0x00000865, 0x00040008, 0x00000001, 0x0000033E, 0x00000000, 0x0004003D, 0x00000008, 0x00000866, 0x000006E7, 0x0005008E, 0x00000008, 
0x00000868, 0x00000866, 0x00000867, 0x0003003E, 0x00000869, 0x00000868, 0x00050039, 0x00000007, 0x0000086A, 0x00000035, 0x00000869, 0x0004003D, 
0x00000007, 0x0000086B, 0x0000084B, 0x00050081, 0x00000007, 0x0000086C, 0x0000086B, 0x0000086A, 0x0003003E, 0x0000084B, 0x0000086C, 0x00040008, 
0x00000001, 0x0000033F, 0x00000000, 0x0004003D, 0x00000007, 0x0000086D, 0x0000084B, 0x0008000C, 0x00000007, 0x0000086E, 0x00000002, 0x0000002E, 
0x0000061F, 0x000001F6, 0x0000086D, 0x0003003E, 0x0000084B, 0x0000086E, 0x00040008, 0x00000001, 0x00000341, 0x00000000, 0x0004003D, 0x00000008, 
0x00000870, 0x000006E7, 0x0007004F, 0x00000009, 0x00000871, 0x00000870, 0x00000870, 0x00000000, 0x00000002, 0x0006000C, 0x00000009, 0x00000872, 
0x00000002, 0x00000008, 0x00000871, 0x0003003E, 0x0000086F, 0x00000872, 0x00040008, 0x00000001, 0x00000343, 0x00000000, 0x0003003E, 0x00000873, 
0x00000874, 0x00040008, 0x00000001, 0x00000344, 0x00000000, 0x0004003D, 0x00000008, 0x00000875, 0x00000873, 0x0005008E, 0x00000008, 0x00000876, 
0x00000875, 0x000003D0, 0x0003003E, 0x00000873, 0x00000876, 0x00040008, 0x00000001, 0x00000346, 0x00000000, 0x0003003E, 0x00000877, 0x0000012A, 
0x00040008, 0x00000001, 0x00000347, 0x00000000, 0x00050041, 0x00000015, 0x00000878, 0x0000078D, 0x00000098, 0x0004003D, 0x00000007, 0x00000879, 
0x00000878, 0x000500B8, 0x000002C0, 0x0000087A, 0x00000879, 0x00000285, 0x000300F7, 0x0000087C, 0x00000000, 0x000400FA, 0x0000087A, 0x0000087B, 
0x0000087C, 0x000200F8, 0x0000087B, 0x00050041, 0x00000015, 0x0000087D, 0x000006E4, 0x00000098, 0x0004003D, 0x00000007, 0x0000087E, 0x0000087D, 
0x000500B4, 0x000002C0, 0x0000087F, 0x0000087E, 0x0000012A, 0x000200F9, 0x0000087C, 0x000200F8, 0x0000087C, 0x000700F5, 0x000002C0, 0x00000880, 
0x0000087A, 0x000007F0, 0x0000087F, 0x0000087B, 0x000300F7, 0x00000882, 0x00000000, 0x000400FA, 0x00000880, 0x00000881, 0x0000090F, 0x000200F8, 
0x00000881, 0x00040008, 0x00000001, 0x0000034A, 0x00000000, 0x0004003D, 0x00000008, 0x00000884, 0x000006E7, 0x000400CF, 0x00000008, 0x00000885, 
0x00000884, 0x0003003E, 0x00000883, 0x00000885, 0x00040008, 0x00000001, 0x0000034C, 0x00000000, 0x0004003D, 0x00000008, 0x00000887, 0x000006E7, 
0x000400D0, 0x00000008, 0x00000888, 0x00000887, 0x0003003E, 0x00000886, 0x00000888, 0x00040008, 0x00000001, 0x0000034E, 0x00000000, 0x0004003D, 
0x00000008, 0x0000088A, 0x00000883, 0x0004003D, 0x00000009, 0x0000088C, 0x000005BF, 0x0003003E, 0x0000088B, 0x0000088C, 0x00050039, 0x00000007, 
0x0000088D, 0x0000001D, 0x0000088B, 0x0005008E, 0x00000008, 0x0000088E, 0x0000088A, 0x0000088D, 0x0004003D, 0x00000008, 0x0000088F, 0x00000886, 
0x0004003D, 0x00000009, 0x00000890, 0x000005BF, 0x0005008E, 0x00000009, 0x00000892, 0x00000890, 0x00000891, 0x0003003E, 0x00000893, 0x00000892, 
0x00050039, 0x00000007, 0x00000894, 0x0000001D, 0x00000893, 0x0005008E, 0x00000008, 0x00000895, 0x0000088F, 0x00000894, 0x00050081, 0x00000008, 
0x00000896, 0x0000088E, 0x00000895, 0x0003003E, 0x00000889, 0x00000896, 0x00040008, 0x00000001, 0x00000350, 0x00000000, 0x0003003E, 0x00000897, 
0x000005BC, 0x00040008, 0x00000001, 0x00000352, 0x00000000, 0x0004003D, 0x00000008, 0x00000899, 0x00000873, 0x0003003E, 0x00000898, 0x00000899, 
0x00040008, 0x00000001, 0x00000354, 0x00000000, 0x0003003E, 0x0000089A, 0x000005BC, 0x00040008, 0x00000001, 0x00000355, 0x00000000, 0x0004003D, 
0x00000009, 0x0000089C, 0x0000086F, 0x0003003E, 0x0000089B, 0x0000089C, 0x0004003D, 0x00000008, 0x0000089E, 0x000006E7, 0x0003003E, 0x0000089D, 
0x0000089E, 0x0004003D, 0x00000008, 0x000008A0, 0x00000898, 0x0003003E, 0x0000089F, 0x000008A0, 0x0004003D, 0x00000007, 0x000008A2, 0x00000877, 
0x0003003E, 0x000008A1, 0x000008A2, 0x0004003D, 0x00000008, 0x000008A4, 0x0000089A, 0x0003003E, 0x000008A3, 0x000008A4, 0x00090039, 0x00000003, 
0x000008A5, 0x0000007B, 0x0000089B, 0x0000089D, 0x0000089F, 0x000008A1, 0x000008A3, 0x0004003D, 0x00000008, 0x000008A6, 0x0000089F, 0x0003003E, 
0x00000898, 0x000008A6, 0x0004003D, 0x00000007, 0x000008A7, 0x000008A1, 0x0003003E, 0x00000877, 0x000008A7, 0x0004003D, 0x00000008, 0x000008A8, 
0x000008A3, 0x0003003E, 0x0000089A, 0x000008A8, 0x00040008, 0x00000001, 0x00000356, 0x00000000, 0x0004003D, 0x00000008, 0x000008A9, 0x00000898, 
0x0003003E, 0x00000897, 0x000008A9, 0x00040008, 0x00000001, 0x00000357, 0x00000000, 0x0004003D, 0x00000008, 0x000008AA, 0x00000873, 0x0003003E, 
0x00000898, 0x000008AA, 0x00040008, 0x00000001, 0x00000358, 0x00000000, 0x0004003D, 0x00000008, 0x000008AB, 0x000006E7, 0x0004003D, 0x00000008, 
0x000008AC, 0x00000883, 0x0005008E, 0x00000008, 0x000008AE, 0x000008AC, 0x000008AD, 0x00050081, 0x00000008, 0x000008AF, 0x000008AB, 0x000008AE, 
0x0004003D, 0x00000009, 0x000008B1, 0x0000086F, 0x0003003E, 0x000008B0, 0x000008B1, 0x0003003E, 0x000008B2, 0x000008AF, 0x0004003D, 0x00000008, 
0x000008B4, 0x00000898, 0x0003003E, 0x000008B3, 0x000008B4, 0x0004003D, 0x00000007, 0x000008B6, 0x00000877, 0x0003003E, 0x000008B5, 0x000008B6, 
0x0004003D, 0x00000008, 0x000008B8, 0x0000089A, 0x0003003E, 0x000008B7, 0x000008B8, 0x00090039, 0x00000003, 0x000008B9, 0x0000007B, 0x000008B0, 
0x000008B2, 0x000008B3, 0x000008B5, 0x000008B7, 0x0004003D, 0x00000008, 0x000008BA, 0x000008B3, 0x0003003E, 0x00000898, 0x000008BA, 0x0004003D, 
0x00000007, 0x000008BB, 0x000008B5, 0x0003003E, 0x00000877, 0x000008BB, 0x0004003D, 0x00000008, 0x000008BC, 0x000008B7, 0x0003003E, 0x0000089A, 
0x000008BC, 0x00040008, 0x00000001, 0x00000359, 0x00000000, 0x0004003D, 0x00000008, 0x000008BD, 0x00000898, 0x0004003D, 0x00000008, 0x000008BE, 
0x00000897, 0x00050081, 0x00000008, 0x000008BF, 0x000008BE, 0x000008BD, 0x0003003E, 0x00000897, 0x000008BF, 0x00040008, 0x00000001, 0x0000035A, 
0x00000000, 0x0004003D, 0x00000008, 0x000008C0, 0x00000873, 0x0003003E, 0x00000898, 0x000008C0, 0x00040008, 0x00000001, 0x0000035B, 0x00000000, 
0x0004003D, 0x00000008, 0x000008C1, 0x000006E7, 0x0004003D, 0x00000008, 0x000008C2, 0x00000883, 0x0005008E, 0x00000008, 0x000008C3, 0x000008C2, 
0x000008AD, 0x00050081, 0x00000008, 0x000008C4, 0x000008C1, 0x000008C3, 0x0004003D, 0x00000008, 0x000008C5, 0x00000886, 0x0005008E, 0x00000008, 
0x000008C6, 0x000008C5, 0x000008AD, 0x00050081, 0x00000008, 0x000008C7, 0x000008C4, 0x000008C6, 0x0004003D, 0x00000009, 0x000008C9, 0x0000086F, 
0x0003003E, 0x000008C8, 0x000008C9, 0x0003003E, 0x000008CA, 0x000008C7, 0x0004003D, 0x00000008, 0x000008CC, 0x00000898, 0x0003003E, 0x000008CB, 
0x000008CC, 0x0004003D, 0x00000007, 0x000008CE, 0x00000877, 0x0003003E, 0x000008CD, 0x000008CE, 0x0004003D, 0x00000008, 0x000008D0, 0x0000089A, 
0x0003003E, 0x000008CF, 0x000008D0, 0x00090039, 0x00000003, 0x000008D1, 0x0000007B, 0x000008C8, 0x000008CA, 0x000008CB, 0x000008CD, 0x000008CF, 
0x0004003D, 0x00000008, 0x000008D2, 0x000008CB, 0x0003003E, 0x00000898, 0x000008D2, 0x0004003D, 0x00000007, 0x000008D3, 0x000008CD, 0x0003003E, 
0x00000877, 0x000008D3, 0x0004003D, 0x00000008, 0x000008D4, 0x000008CF, 0x0003003E, 0x0000089A, 0x000008D4, 0x00040008, 0x00000001, 0x0000035C, 
0x00000000, 0x0004003D, 0x00000008, 0x000008D5, 0x00000898, 0x0004003D, 0x00000008, 0x000008D6, 0x00000897, 0x00050081, 0x00000008, 0x000008D7, 
0x000008D6, 0x000008D5, 0x0003003E, 0x00000897, 0x000008D7, 0x00040008, 0x00000001, 0x0000035D, 0x00000000, 0x0004003D, 0x00000008, 0x000008D8, 
0x00000873, 0x0003003E, 0x00000898, 0x000008D8, 0x00040008, 0x00000001, 0x0000035E, 0x00000000, 0x0004003D, 0x00000008, 0x000008D9, 0x000006E7, 
0x0004003D, 0x00000008, 0x000008DA, 0x00000886, 0x0005008E, 0x00000008, 0x000008DB, 0x000008DA, 0x000008AD, 0x00050081, 0x00000008, 0x000008DC, 
0x000008D9, 0x000008DB, 0x0004003D, 0x00000009, 0x000008DE, 0x0000086F, 0x0003003E, 0x000008DD, 0x000008DE, 0x0003003E, 0x000008DF, 0x000008DC, 
0x0004003D, 0x00000008, 0x000008E1, 0x00000898, 0x0003003E, 0x000008E0, 0x000008E1, 0x0004003D, 0x00000007, 0x000008E3, 0x00000877, 0x0003003E, 
0x000008E2, 0x000008E3, 0x0004003D, 0x00000008, 0x000008E5, 0x0000089A, 0x0003003E, 0x000008E4, 0x000008E5, 0x00090039, 0x00000003, 0x000008E6, 
0x0000007B, 0x000008DD, 0x000008DF, 0x000008E0, 0x000008E2, 0x000008E4, 0x0004003D, 0x00000008, 0x000008E7, 0x000008E0, 0x0003003E, 0x00000898, 
0x000008E7, 0x0004003D, 0x00000007, 0x000008E8, 0x000008E2, 0x0003003E, 0x00000877, 0x000008E8, 0x0004003D, 0x00000008, 0x000008E9, 0x000008E4, 
0x0003003E, 0x0000089A, 0x000008E9, 0x00040008, 0x00000001, 0x0000035F, 0x00000000, 0x0004003D, 0x00000008, 0x000008EA, 0x00000898, 0x0004003D, 
0x00000008, 0x000008EB, 0x00000897, 0x00050081, 0x00000008, 0x000008EC, 0x000008EB, 0x000008EA, 0x0003003E, 0x00000897, 0x000008EC, 0x00040008, 
0x00000001, 0x00000360, 0x00000000, 0x0004003D, 0x00000008, 0x000008ED, 0x00000873, 0x0003003E, 0x00000898, 0x000008ED, 0x00040008, 0x00000001, 
0x00000361, 0x00000000, 0x0004003D, 0x00000008, 0x000008EE, 0x000006E7, 0x0004003D, 0x00000008, 0x000008EF, 0x00000883, 0x0005008E, 0x00000008, 
0x000008F1, 0x000008EF, 0x000008F0, 0x00050081, 0x00000008, 0x000008F2, 0x000008EE, 0x000008F1, 0x0004003D, 0x00000008, 0x000008F3, 0x00000886, 
0x0005008E, 0x00000008, 0x000008F4, 0x000008F3, 0x000008F0, 0x00050081, 0x00000008, 0x000008F5, 0x000008F2, 0x000008F4, 0x0004003D, 0x00000009, 
0x000008F7, 0x0000086F, 0x0003003E, 0x000008F6, 0x000008F7, 0x0003003E, 0x000008F8, 0x000008F5, 0x0004003D, 0x00000008, 0x000008FA, 0x00000898, 
0x0003003E, 0x000008F9, 0x000008FA, 0x0004003D, 0x00000007, 0x000008FC, 0x00000877, 0x0003003E, 0x000008FB, 0x000008FC, 0x0004003D, 0x00000008, 
0x000008FE, 0x0000089A, 0x0003003E, 0x000008FD, 0x000008FE, 0x00090039, 0x00000003, 0x000008FF, 0x0000007B, 0x000008F6, 0x000008F8, 0x000008F9, 
0x000008FB, 0x000008FD, 0x0004003D, 0x00000008, 0x00000900, 0x000008F9, 0x0003003E, 0x00000898, 0x00000900, 0x0004003D, 0x00000007, 0x00000901, 
0x000008FB, 0x0003003E, 0x00000877, 0x00000901, 0x0004003D, 0x00000008, 0x00000902, 0x000008FD, 0x0003003E, 0x0000089A, 0x00000902, 0x00040008, 
0x00000001, 0x00000362, 0x00000000, 0x0004003D, 0x00000008, 0x00000903, 0x00000898, 0x0004003D, 0x00000008, 0x00000904, 0x00000897, 0x00050081, 
0x00000008, 0x00000905, 0x00000904, 0x00000903, 0x0003003E, 0x00000897, 0x00000905, 0x00040008, 0x00000001, 0x00000363, 0x00000000, 0x0004003D, 
0x00000008, 0x00000906, 0x00000897, 0x0005008E, 0x00000008, 0x00000907, 0x00000906, 0x0000022E, 0x0003003E, 0x00000873, 0x00000907, 0x00040008, 
0x00000001, 0x00000364, 0x00000000, 0x0004003D, 0x00000007, 0x00000908, 0x00000877, 0x00050085, 0x00000007, 0x00000909, 0x00000908, 0x0000022E, 
0x0003003E, 0x00000877, 0x00000909, 0x00040008, 0x00000001, 0x00000365, 0x00000000, 0x0004003D, 0x00000008, 0x0000090A, 0x0000078D, 0x0004003D, 
0x00000008, 0x0000090B, 0x0000089A, 0x0005008E, 0x00000008, 0x0000090C, 0x0000090B, 0x0000022E, 0x00050081, 0x00000008, 0x0000090D, 0x0000090A, 
0x0000090C, 0x0006000C, 0x00000008, 0x0000090E, 0x00000002, 0x00000045, 0x0000090D, 0x0003003E, 0x0000078D, 0x0000090E, 0x000200F9, 0x00000882, 
0x000200F8, 0x0000090F, 0x00040008, 0x00000001, 0x0000036B, 0x00000000, 0x00050041, 0x00000015, 0x00000911, 0x000006E7, 0x000000A9, 0x0004003D, 
0x00000007, 0x00000912, 0x00000911, 0x00050081, 0x00000007, 0x00000913, 0x00000912, 0x000001CC, 0x0006000C, 0x00000007, 0x00000914, 0x00000002, 
0x0000000A, 0x00000913, 0x00050083, 0x00000007, 0x00000915, 0x00000914, 0x000001CC, 0x0006000C, 0x00000007, 0x00000916, 0x00000002, 0x00000004, 
0x00000915, 0x0003003E, 0x00000910, 0x00000916, 0x00040008, 0x00000001, 0x0000036D, 0x00000000, 0x00050041, 0x00000015, 0x00000918, 0x000006E7, 
0x00000102, 0x0004003D, 0x00000007, 0x00000919, 0x00000918, 0x00050081, 0x00000007, 0x0000091A, 0x00000919, 0x000001CC, 0x0006000C, 0x00000007, 
0x0000091B, 0x00000002, 0x0000000A, 0x0000091A, 0x00050083, 0x00000007, 0x0000091C, 0x0000091B, 0x000001CC, 0x0006000C, 0x00000007, 0x0000091D, 
0x00000002, 0x00000004, 0x0000091C, 0x0003003E, 0x00000917, 0x0000091D, 0x00040008, 0x00000001, 0x0000036F, 0x00000000, 0x0004003D, 0x00000007, 
0x0000091F, 0x00000910, 0x0004003D, 0x00000007, 0x00000920, 0x00000917, 0x0007000C, 0x00000007, 0x00000921, 0x00000002, 0x00000025, 0x0000091F, 
0x00000920, 0x00050083, 0x00000007, 0x00000923, 0x00000921, 0x00000922, 0x00050085, 0x00000007, 0x00000925, 0x00000923, 0x00000924, 0x0003003E, 
0x00000926, 0x00000925, 0x00050039, 0x00000007, 0x00000927, 0x0000003C, 0x00000926, 0x0003003E, 0x0000091E, 0x00000927, 0x00040008, 0x00000001, 
0x00000370, 0x00000000, 0x00050041, 0x00000015, 0x00000928, 0x0000078D, 0x00000098, 0x0004003D, 0x00000007, 0x00000929, 0x00000928, 0x00050085, 
0x00000007, 0x0000092A, 0x00000929, 0x000001F6, 0x0004003D, 0x00000009, 0x0000092B, 0x0000086F, 0x0005008E, 0x00000009, 0x0000092D, 0x0000092B, 
0x0000092C, 0x0003003E, 0x0000092E, 0x0000092D, 0x00050039, 0x00000007, 0x0000092F, 0x0000001D, 0x0000092E, 0x00050085, 0x00000007, 0x00000930, 
0x0000092A, 0x0000092F, 0x0004003D, 0x00000007, 0x00000931, 0x0000091E, 0x00050085, 0x00000007, 0x00000932, 0x00000930, 0x00000931, 0x00050083, 
0x00000007, 0x00000933, 0x00000129, 0x00000932, 0x0004003D, 0x00000008, 0x00000934, 0x00000873, 0x0005008E, 0x00000008, 0x00000935, 0x00000934, 
0x00000933, 0x0003003E, 0x00000873, 0x00000935, 0x00040008, 0x00000001, 0x00000371, 0x00000000, 0x0004003D, 0x00000007, 0x00000936, 0x0000091E, 
0x0008000C, 0x00000007, 0x00000937, 0x00000002, 0x0000002E, 0x00000285, 0x00000129, 0x00000936, 0x0004003D, 0x00000008, 0x00000938, 0x00000873, 
0x0005008E, 0x00000008, 0x00000939, 0x00000938, 0x00000937, 0x0003003E, 0x00000873, 0x00000939, 0x00040008, 0x00000001, 0x00000373, 0x00000000, 
0x0004003D, 0x00000007, 0x0000093B, 0x00000910, 0x0004003D, 0x00000007, 0x0000093C, 0x00000917, 0x0007000C, 0x00000007, 0x0000093D, 0x00000002, 
0x00000025, 0x0000093B, 0x0000093C, 0x00050083, 0x00000007, 0x0000093E, 0x0000093D, 0x000007EA, 0x00050085, 0x00000007, 0x0000093F, 0x0000093E, 
0x00000924, 0x00050083, 0x00000007, 0x00000940, 0x00000129, 0x0000093F, 0x0003003E, 0x00000941, 0x00000940, 0x00050039, 0x00000007, 0x00000942, 
0x0000003C, 0x00000941, 0x0003003E, 0x0000093A, 0x00000942, 0x00040008, 0x00000001, 0x00000374, 0x00000000, 0x0004003D, 0x00000007, 0x00000943, 
0x00000910, 0x0004003D, 0x00000007, 0x00000944, 0x00000917, 0x0007000C, 0x00000007, 0x00000945, 0x00000002, 0x00000025, 0x00000943, 0x00000944, 
0x00050083, 0x00000007, 0x00000946, 0x00000945, 0x00000318, 0x00050085, 0x00000007, 0x00000947, 0x00000946, 0x00000924, 0x0003003E, 0x00000948, 
0x00000947, 0x00050039, 0x00000007, 0x00000949, 0x0000003C, 0x00000948, 0x0004003D, 0x00000007, 0x0000094A, 0x0000093A, 0x00050085, 0x00000007, 
0x0000094B, 0x0000094A, 0x00000949, 0x0003003E, 0x0000093A, 0x0000094B, 0x00040008, 0x00000001, 0x00000375, 0x00000000, 0x0004003D, 0x00000007, 
0x0000094C, 0x00000910, 0x0004003D, 0x00000007, 0x0000094D, 0x00000910, 0x00050085, 0x00000007, 0x0000094E, 0x0000094C, 0x0000094D, 0x0004003D, 
0x00000007, 0x0000094F, 0x00000917, 0x0004003D, 0x00000007, 0x00000950, 0x00000917, 0x00050085, 0x00000007, 0x00000951, 0x0000094F, 0x00000950, 
0x00050081, 0x00000007, 0x00000952, 0x0000094E, 0x00000951, 0x00050083, 0x00000007, 0x00000953, 0x00000952, 0x000001D1, 0x00050085, 0x00000007, 
0x00000955, 0x00000953, 0x00000954, 0x0003003E, 0x00000956, 0x00000955, 0x00050039, 0x00000007, 0x00000957, 0x0000003C, 0x00000956, 0x0004003D, 
0x00000007, 0x00000958, 0x0000093A, 0x00050085, 0x00000007, 0x00000959, 0x00000958, 0x00000957, 0x0003003E, 0x0000093A, 0x00000959, 0x00040008, 
0x00000001, 0x00000376, 0x00000000, 0x0004003D, 0x00000008, 0x0000095A, 0x00000873, 0x0004003D, 0x00000007, 0x0000095C, 0x0000093A, 0x00060050, 
0x00000008, 0x0000095D, 0x0000095C, 0x0000095C, 0x0000095C, 0x0008000C, 0x00000008, 0x0000095E, 0x00000002, 0x0000002E, 0x0000095A, 0x0000095B, 
0x0000095D, 0x0003003E, 0x00000873, 0x0000095E, 0x00040008, 0x00000001, 0x00000378, 0x00000000, 0x0004003D, 0x00000007, 0x00000960, 0x00000910, 
0x0004003D, 0x00000007, 0x00000961, 0x00000917, 0x0007000C, 0x00000007, 0x00000962, 0x00000002, 0x00000025, 0x00000960, 0x00000961, 0x00050083, 
0x00000007, 0x00000964, 0x00000962, 0x00000963, 0x00050085, 0x00000007, 0x00000965, 0x00000964, 0x00000924, 0x00050083, 0x00000007, 0x00000966, 
0x00000129, 0x00000965, 0x0003003E, 0x00000967, 0x00000966, 0x00050039, 0x00000007, 0x00000968, 0x0000003C, 0x00000967, 0x0003003E, 0x0000095F, 
0x00000968, 0x00040008, 0x00000001, 0x00000379, 0x00000000, 0x0004003D, 0x00000007, 0x00000969, 0x00000910, 0x0004003D, 0x00000007, 0x0000096A, 
0x00000917, 0x0007000C, 0x00000007, 0x0000096B, 0x00000002, 0x00000025, 0x00000969, 0x0000096A, 0x00050083, 0x00000007, 0x0000096D, 0x0000096B, 
0x0000096C, 0x00050085, 0x00000007, 0x0000096E, 0x0000096D, 0x00000924, 0x0003003E, 0x0000096F, 0x0000096E, 0x00050039, 0x00000007, 0x00000970, 
0x0000003C, 0x0000096F, 0x0004003D, 0x00000007, 0x00000971, 0x0000095F, 0x00050085, 0x00000007, 0x00000972, 0x00000971, 0x00000970, 0x0003003E, 
0x0000095F, 0x00000972, 0x00040008, 0x00000001, 0x0000037A, 0x00000000, 0x0004003D, 0x00000007, 0x00000973, 0x00000910, 0x0004003D, 0x00000007, 
0x00000974, 0x00000910, 0x00050085, 0x00000007, 0x00000975, 0x00000973, 0x00000974, 0x0004003D, 0x00000007, 0x00000976, 0x00000917, 0x0004003D, 
0x00000007, 0x00000977, 0x00000917, 0x00050085, 0x00000007, 0x00000978, 0x00000976, 0x00000977, 0x00050081, 0x00000007, 0x00000979, 0x00000975, 
0x00000978, 0x00050083, 0x00000007, 0x0000097A, 0x00000979, 0x000001D1, 0x00050085, 0x00000007, 0x0000097B, 0x0000097A, 0x00000954, 0x0003003E, 
0x0000097C, 0x0000097B, 0x00050039, 0x00000007, 0x0000097D, 0x0000003C, 0x0000097C, 0x0004003D, 0x00000007, 0x0000097E, 0x0000095F, 0x00050085, 
0x00000007, 0x0000097F, 0x0000097E, 0x0000097D, 0x0003003E, 0x0000095F, 0x0000097F, 0x00040008, 0x00000001, 0x0000037B, 0x00000000, 0x0004003D, 
0x00000007, 0x00000980, 0x00000917, 0x00050085, 0x00000007, 0x00000981, 0x00000980, 0x00000222, 0x0006000C, 0x00000007, 0x00000982, 0x00000002, 
0x0000000A, 0x00000981, 0x00050083, 0x00000007, 0x00000983, 0x00000982, 0x000001CC, 0x00050085, 0x00000007, 0x00000985, 0x00000983, 0x00000984, 
0x00050083, 0x00000007, 0x00000986, 0x00000129, 0x00000985, 0x0003003E, 0x00000987, 0x00000986, 0x00050039, 0x00000007, 0x00000988, 0x0000003C, 
0x00000987, 0x0004003D, 0x00000007, 0x00000989, 0x0000095F, 0x00050085, 0x00000007, 0x0000098A, 0x00000989, 0x00000988, 0x0003003E, 0x0000095F, 
0x0000098A, 0x00040008, 0x00000001, 0x0000037C, 0x00000000, 0x0004003D, 0x00000007, 0x0000098B, 0x00000910, 0x00050085, 0x00000007, 0x0000098C, 
0x0000098B, 0x00000222, 0x0006000C, 0x00000007, 0x0000098D, 0x00000002, 0x0000000A, 0x0000098C, 0x00050083, 0x00000007, 0x0000098E, 0x0000098D, 
0x000001CC, 0x00050085, 0x00000007, 0x0000098F, 0x0000098E, 0x00000984, 0x00050083, 0x00000007, 0x00000990, 0x00000129, 0x0000098F, 0x0003003E, 
0x00000991, 0x00000990, 0x00050039, 0x00000007, 0x00000992, 0x0000003C, 0x00000991, 0x0004003D, 0x00000007, 0x00000993, 0x0000095F, 0x00050085, 
0x00000007, 0x00000994, 0x00000993, 0x00000992, 0x0003003E, 0x0000095F, 0x00000994, 0x00040008, 0x00000001, 0x0000037D, 0x00000000, 0x0004003D, 
0x00000008, 0x00000995, 0x00000873, 0x0004003D, 0x00000007, 0x00000997, 0x0000095F, 0x00060050, 0x00000008, 0x00000998, 0x00000997, 0x00000997, 
0x00000997, 0x0008000C, 0x00000008, 0x00000999, 0x00000002, 0x0000002E, 0x00000995, 0x00000996, 0x00000998, 0x0003003E, 0x00000873, 0x00000999, 
0x00040008, 0x00000001, 0x0000037E, 0x00000000, 0x0004003D, 0x00000007, 0x0000099A, 0x00000910, 0x0004003D, 0x00000007, 0x0000099B, 0x00000917, 
0x0007000C, 0x00000007, 0x0000099C, 0x00000002, 0x00000025, 0x0000099A, 0x0000099B, 0x00050083, 0x00000007, 0x0000099E, 0x0000099C, 0x0000099D, 
0x00050085, 0x00000007, 0x0000099F, 0x0000099E, 0x00000924, 0x00050083, 0x00000007, 0x000009A0, 0x00000129, 0x0000099F, 0x0003003E, 0x000009A1, 
0x000009A0, 0x00050039, 0x00000007, 0x000009A2, 0x0000003C, 0x000009A1, 0x0003003E, 0x0000095F, 0x000009A2, 0x00040008, 0x00000001, 0x0000037F, 
0x00000000, 0x0004003D, 0x00000007, 0x000009A3, 0x00000910, 0x0004003D, 0x00000007, 0x000009A4, 0x00000917, 0x0007000C, 0x00000007, 0x000009A5, 
0x00000002, 0x00000025, 0x000009A3, 0x000009A4, 0x00050083, 0x00000007, 0x000009A7, 0x000009A5, 0x000009A6, 0x00050085, 0x00000007, 0x000009A8, 
0x000009A7, 0x00000924, 0x0003003E, 0x000009A9, 0x000009A8, 0x00050039, 0x00000007, 0x000009AA, 0x0000003C, 0x000009A9, 0x0004003D, 0x00000007, 
0x000009AB, 0x0000095F, 0x00050085, 0x00000007, 0x000009AC, 0x000009AB, 0x000009AA, 0x0003003E, 0x0000095F, 0x000009AC, 0x00040008, 0x00000001, 
0x00000380, 0x00000000, 0x0004003D, 0x00000007, 0x000009AD, 0x00000910, 0x0004003D, 0x00000007, 0x000009AE, 0x00000910, 0x00050085, 0x00000007, 
0x000009AF, 0x000009AD, 0x000009AE, 0x0004003D, 0x00000007, 0x000009B0, 0x00000917, 0x0004003D, 0x00000007, 0x000009B1, 0x00000917, 0x00050085, 
0x00000007, 0x000009B2, 0x000009B0, 0x000009B1, 0x00050081, 0x00000007, 0x000009B3, 0x000009AF, 0x000009B2, 0x00050083, 0x00000007, 0x000009B4, 
0x000009B3, 0x00000963, 0x00050085, 0x00000007, 0x000009B5, 0x000009B4, 0x00000954, 0x0003003E, 0x000009B6, 0x000009B5, 0x00050039, 0x00000007, 
0x000009B7, 0x0000003C, 0x000009B6, 0x0004003D, 0x00000007, 0x000009B8, 0x0000095F, 0x00050085, 0x00000007, 0x000009B9, 0x000009B8, 0x000009B7, 
0x0003003E, 0x0000095F, 0x000009B9, 0x00040008, 0x00000001, 0x00000381, 0x00000000, 0x0004003D, 0x00000008, 0x000009BA, 0x00000873, 0x0004003D, 
0x00000007, 0x000009BB, 0x0000095F, 0x00060050, 0x00000008, 0x000009BC, 0x000009BB, 0x000009BB, 0x000009BB, 0x0008000C, 0x00000008, 0x000009BD, 
0x00000002, 0x0000002E, 0x000009BA, 0x00000996, 0x000009BC, 0x0003003E, 0x00000873, 0x000009BD, 0x00040008, 0x00000001, 0x00000383, 0x00000000, 
0x0004003D, 0x00000007, 0x000009BF, 0x00000910, 0x00050085, 0x00000007, 0x000009C0, 0x000009BF, 0x00000551, 0x0006000C, 0x00000007, 0x000009C1, 
0x00000002, 0x0000000A, 0x000009C0, 0x00050083, 0x00000007, 0x000009C2, 0x000009C1, 0x000001CC, 0x00050085, 0x00000007, 0x000009C3, 0x000009C2, 
0x00000984, 0x00050083, 0x00000007, 0x000009C4, 0x00000129, 0x000009C3, 0x0003003E, 0x000009C5, 0x000009C4, 0x00050039, 0x00000007, 0x000009C6, 
0x0000003C, 0x000009C5, 0x0003003E, 0x000009BE, 0x000009C6, 0x00040008, 0x00000001, 0x00000384, 0x00000000, 0x0004003D, 0x00000007, 0x000009C7, 
0x00000917, 0x00050083, 0x00000007, 0x000009C8, 0x000009C7, 0x00000379, 0x00050085, 0x00000007, 0x000009C9, 0x000009C8, 0x00000954, 0x0003003E, 
0x000009CA, 0x000009C9, 0x00050039, 0x00000007, 0x000009CB, 0x0000003C, 0x000009CA, 0x0004003D, 0x00000007, 0x000009CC, 0x000009BE, 0x00050085, 
0x00000007, 0x000009CD, 0x000009CC, 0x000009CB, 0x0003003E, 0x000009BE, 0x000009CD, 0x00040008, 0x00000001, 0x00000385, 0x00000000, 0x0004003D, 
0x00000007, 0x000009CE, 0x00000917, 0x0004007F, 0x00000007, 0x000009CF, 0x000009CE, 0x00050081, 0x00000007, 0x000009D1, 0x000009CF, 0x000009D0, 
0x00050085, 0x00000007, 0x000009D2, 0x000009D1, 0x00000954, 0x0003003E, 0x000009D3, 0x000009D2, 0x00050039, 0x00000007, 0x000009D4, 0x0000003C, 
0x000009D3, 0x0004003D, 0x00000007, 0x000009D5, 0x0000091E, 0x00050083, 0x00000007, 0x000009D6, 0x00000129, 0x000009D5, 0x00050085, 0x00000007, 
0x000009D7, 0x000009D4, 0x000009D6, 0x0004003D, 0x00000007, 0x000009D8, 0x000009BE, 0x00050085, 0x00000007, 0x000009D9, 0x000009D8, 0x000009D7, 
0x0003003E, 0x000009BE, 0x000009D9, 0x00040008, 0x00000001, 0x00000386, 0x00000000, 0x0004003D, 0x00000007, 0x000009DA, 0x0000084B, 0x0004003D, 
0x00000007, 0x000009DB, 0x0000084B, 0x00050085, 0x00000007, 0x000009DC, 0x000009DA, 0x000009DB, 0x0004003D, 0x00000007, 0x000009DD, 0x000009BE, 
0x00050085, 0x00000007, 0x000009DE, 0x000009DD, 0x000009DC, 0x0003003E, 0x000009BE, 0x000009DE, 0x00040008, 0x00000001, 0x00000387, 0x00000000, 
0x0004003D, 0x00000008, 0x000009DF, 0x00000873, 0x0004003D, 0x00000007, 0x000009E1, 0x000009BE, 0x00060050, 0x00000008, 0x000009E2, 0x000009E1, 
0x000009E1, 0x000009E1, 0x0008000C, 0x00000008, 0x000009E3, 0x00000002, 0x0000002E, 0x000009DF, 0x000009E0, 0x000009E2, 0x0003003E, 0x00000873, 
0x000009E3, 0x00040008, 0x00000001, 0x00000388, 0x00000000, 0x0004003D, 0x00000007, 0x000009E4, 0x00000917, 0x00050085, 0x00000007, 0x000009E5, 
0x000009E4, 0x00000551, 0x0006000C, 0x00000007, 0x000009E6, 0x00000002, 0x0000000A, 0x000009E5, 0x00050083, 0x00000007, 0x000009E7, 0x000009E6, 
0x000001CC, 0x00050085, 0x00000007, 0x000009E8, 0x000009E7, 0x00000984, 0x00050083, 0x00000007, 0x000009E9, 0x00000129, 0x000009E8, 0x0003003E, 
0x000009EA, 0x000009E9, 0x00050039, 0x00000007, 0x000009EB, 0x0000003C, 0x000009EA, 0x0003003E, 0x000009BE, 0x000009EB, 0x00040008, 0x00000001, 
0x00000389, 0x00000000, 0x0004003D, 0x00000007, 0x000009EC, 0x00000910, 0x00050083, 0x00000007, 0x000009ED, 0x000009EC, 0x00000379, 0x00050085, 
0x00000007, 0x000009EE, 0x000009ED, 0x00000954, 0x0003003E, 0x000009EF, 0x000009EE, 0x00050039, 0x00000007, 0x000009F0, 0x0000003C, 0x000009EF, 
0x0004003D, 0x00000007, 0x000009F1, 0x000009BE, 0x00050085, 0x00000007, 0x000009F2, 0x000009F1, 0x000009F0, 0x0003003E, 0x000009BE, 0x000009F2, 
0x00040008, 0x00000001, 0x0000038A, 0x00000000, 0x0004003D, 0x00000007, 0x000009F3, 0x00000910, 0x0004007F, 0x00000007, 0x000009F4, 0x000009F3, 
0x00050081, 0x00000007, 0x000009F5, 0x000009F4, 0x000009D0, 0x00050085, 0x00000007, 0x000009F6, 0x000009F5, 0x00000954, 0x0003003E, 0x000009F7, 
0x000009F6, 0x00050039, 0x00000007, 0x000009F8, 0x0000003C, 0x000009F7, 0x0004003D, 0x00000007, 0x000009F9, 0x0000091E, 0x00050083, 0x00000007, 
0x000009FA, 0x00000129, 0x000009F9, 0x00050085, 0x00000007, 0x000009FB, 0x000009F8, 0x000009FA, 0x0004003D, 0x00000007, 0x000009FC, 0x000009BE, 
0x00050085, 0x00000007, 0x000009FD, 0x000009FC, 0x000009FB, 0x0003003E, 0x000009BE, 0x000009FD, 0x00040008, 0x00000001, 0x0000038B, 0x00000000, 
0x0004003D, 0x00000007, 0x000009FE, 0x0000084B, 0x0004003D, 0x00000007, 0x000009FF, 0x0000084B, 0x00050085, 0x00000007, 0x00000A00, 0x000009FE, 
0x000009FF, 0x0004003D, 0x00000007, 0x00000A01, 0x000009BE, 0x00050085, 0x00000007, 0x00000A02, 0x00000A01, 0x00000A00, 0x0003003E, 0x000009BE, 
0x00000A02, 0x00040008, 0x00000001, 0x0000038C, 0x00000000, 0x0004003D, 0x00000008, 0x00000A03, 0x00000873, 0x0004003D, 0x00000007, 0x00000A04, 
0x000009BE, 0x00060050, 0x00000008, 0x00000A05, 0x00000A04, 0x00000A04, 0x00000A04, 0x0008000C, 0x00000008, 0x00000A06, 0x00000002, 0x0000002E, 
0x00000A03, 0x000009E0, 0x00000A05, 0x0003003E, 0x00000873, 0x00000A06, 0x00040008, 0x00000001, 0x0000038E, 0x00000000, 0x0003003E, 0x00000A07, 
0x00000129, 0x00040008, 0x00000001, 0x00000390, 0x00000000, 0x0003003E, 0x00000A08, 0x00000A09, 0x00040008, 0x00000001, 0x00000391, 0x00000000, 
0x00050041, 0x00000015, 0x00000A0A, 0x000006E7, 0x00000098, 0x0004003D, 0x00000007, 0x00000A0B, 0x00000A0A, 0x000500BA, 0x000002C0, 0x00000A0C, 
0x00000A0B, 0x00000285, 0x000300F7, 0x00000A0E, 0x00000000, 0x000400FA, 0x00000A0C, 0x00000A0D, 0x00000A0E, 0x000200F8, 0x00000A0D, 0x00040008, 
0x00000001, 0x00000393, 0x00000000, 0x0003003E, 0x00000A08, 0x00000A10, 0x000200F9, 0x00000A0E, 0x000200F8, 0x00000A0E, 0x00040008, 0x00000001, 
0x00000396, 0x00000000, 0x00050041, 0x00000015, 0x00000A11, 0x000006E7, 0x00000102, 0x0004003D, 0x00000007, 0x00000A12, 0x00000A11, 0x00050041, 
0x00000015, 0x00000A13, 0x00000A08, 0x000000A9, 0x0004003D, 0x00000007, 0x00000A14, 0x00000A13, 0x00050085, 0x00000007, 0x00000A15, 0x00000A12, 
0x00000A14, 0x0006000C, 0x00000007, 0x00000A16, 0x00000002, 0x0000000D, 0x00000A15, 0x00050085, 0x00000007, 0x00000A18, 0x00000A16, 0x00000A17, 
0x00050041, 0x00000015, 0x00000A19, 0x00000A08, 0x000000A9, 0x0004003D, 0x00000007, 0x00000A1A, 0x00000A19, 0x00050088, 0x00000007, 0x00000A1B, 
0x00000A18, 0x00000A1A, 0x0006000C, 0x00000007, 0x00000A1C, 0x00000002, 0x00000004, 0x00000A1B, 0x0003003E, 0x00000A1D, 0x00000A1C, 0x00050039, 
0x00000007, 0x00000A1E, 0x0000003C, 0x00000A1D, 0x0004003D, 0x00000007, 0x00000A1F, 0x00000A07, 0x00050085, 0x00000007, 0x00000A20, 0x00000A1F, 
0x00000A1E, 0x0003003E, 0x00000A07, 0x00000A20, 0x00040008, 0x00000001, 0x00000397, 0x00000000, 0x00050041, 0x00000015, 0x00000A21, 0x000006E7, 
0x000000A9, 0x0004003D, 0x00000007, 0x00000A22, 0x00000A21, 0x00050041, 0x00000015, 0x00000A23, 0x00000A08, 0x00000098, 0x0004003D, 0x00000007, 
0x00000A24, 0x00000A23, 0x00050085, 0x00000007, 0x00000A25, 0x00000A22, 0x00000A24, 0x0006000C, 0x00000007, 0x00000A26, 0x00000002, 0x0000000D, 
0x00000A25, 0x00050085, 0x00000007, 0x00000A27, 0x00000A26, 0x00000A17, 0x00050041, 0x00000015, 0x00000A28, 0x00000A08, 0x00000098, 0x0004003D, 
0x00000007, 0x00000A29, 0x00000A28, 0x00050088, 0x00000007, 0x00000A2A, 0x00000A27, 0x00000A29, 0x0006000C, 0x00000007, 0x00000A2B, 0x00000002, 
0x00000004, 0x00000A2A, 0x0003003E, 0x00000A2C, 0x00000A2B, 0x00050039, 0x00000007, 0x00000A2D, 0x0000003C, 0x00000A2C, 0x0004003D, 0x00000007, 
0x00000A2E, 0x00000A07, 0x00050085, 0x00000007, 0x00000A2F, 0x00000A2E, 0x00000A2D, 0x0003003E, 0x00000A07, 0x00000A2F, 0x00040008, 0x00000001, 
0x00000398, 0x00000000, 0x0004003D, 0x00000007, 0x00000A30, 0x00000A07, 0x0008000C, 0x00000007, 0x00000A31, 0x00000002, 0x0000002E, 0x0000061F, 
0x00000129, 0x00000A30, 0x0003003E, 0x00000A32, 0x00000A31, 0x00050039, 0x00000007, 0x00000A33, 0x0000003C, 0x00000A32, 0x0003003E, 0x00000A07, 
0x00000A33, 0x00040008, 0x00000001, 0x00000399, 0x00000000, 0x0004003D, 0x00000007, 0x00000A34, 0x0000091E, 0x00050083, 0x00000007, 0x00000A35, 
0x00000129, 0x00000A34, 0x0004003D, 0x00000007, 0x00000A36, 0x00000A07, 0x00050081, 0x00000007, 0x00000A37, 0x00000A35, 0x00000A36, 0x0003003E, 
0x00000A38, 0x00000A37, 0x00050039, 0x00000007, 0x00000A39, 0x0000003C, 0x00000A38, 0x0003003E, 0x00000A07, 0x00000A39, 0x00040008, 0x00000001, 
0x0000039A, 0x00000000, 0x0004003D, 0x00000007, 0x00000A3A, 0x00000A07, 0x0004003D, 0x00000008, 0x00000A3B, 0x00000873, 0x0005008E, 0x00000008, 
0x00000A3C, 0x00000A3B, 0x00000A3A, 0x0003003E, 0x00000873, 0x00000A3C, 0x000200F9, 0x00000882, 0x000200F8, 0x00000882, 0x00040008, 0x00000001, 
0x0000039E, 0x00000000, 0x00050041, 0x00000015, 0x00000A3D, 0x000006E4, 0x00000098, 0x0004003D, 0x00000007, 0x00000A3E, 0x00000A3D, 0x000500B4, 
0x000002C0, 0x00000A3F, 0x00000A3E, 0x00000114, 0x000300F7, 0x00000A41, 0x00000000, 0x000400FA, 0x00000A3F, 0x00000A40, 0x00000A41, 0x000200F8, 
0x00000A40, 0x00040008, 0x00000001, 0x000003A0, 0x00000000, 0x0003003E, 0x00000873, 0x00000A42, 0x000200F9, 0x00000A41, 0x000200F8, 0x00000A41, 
0x00040008, 0x00000001, 0x000003A4, 0x00000000, 0x0004003D, 0x00000007, 0x00000A43, 0x0000084B, 0x0005008E, 0x00000008, 0x00000A44, 0x00000183, 
0x00000A43, 0x0005008E, 0x00000008, 0x00000A45, 0x00000A44, 0x000001D1, 0x0004003D, 0x00000008, 0x00000A46, 0x00000873, 0x00050085, 0x00000008, 
0x00000A47, 0x00000A46, 0x00000A45, 0x0003003E, 0x00000873, 0x00000A47, 0x00040008, 0x00000001, 0x000003A5, 0x00000000, 0x0004003D, 0x00000008, 
0x00000A48, 0x00000873, 0x0005008E, 0x00000008, 0x00000A49, 0x00000A48, 0x0000061F, 0x0003003E, 0x00000873, 0x00000A49, 0x00040008, 0x00000001, 
0x000003A6, 0x00000000, 0x0004003D, 0x00000008, 0x00000A4B, 0x00000873, 0x0003003E, 0x00000A4A, 0x00000A4B, 0x00050039, 0x00000008, 0x00000A4C, 
0x00000039, 0x00000A4A, 0x0003003E, 0x00000873, 0x00000A4C, 0x00040008, 0x00000001, 0x000003A8, 0x00000000, 0x0003003E, 0x00000A4D, 0x0000012A, 
0x00040008, 0x00000001, 0x000003A9, 0x00000000, 0x00050041, 0x00000015, 0x00000A4E, 0x000006E4, 0x00000098, 0x0004003D, 0x00000007, 0x00000A4F, 
0x00000A4E, 0x000500BE, 0x000002C0, 0x00000A50, 0x00000A4F, 0x000006BB, 0x000300F7, 0x00000A52, 0x00000000, 0x000400FA, 0x00000A50, 0x00000A51, 
0x00000A52, 0x000200F8, 0x00000A51, 0x00040008, 0x00000001, 0x000003AB, 0x00000000, 0x00050041, 0x00000015, 0x00000A54, 0x000006E4, 0x00000098, 
0x0004003D, 0x00000007, 0x00000A55, 0x00000A54, 0x0003003E, 0x00000A53, 0x00000A55, 0x00050039, 0x00000007, 0x00000A56, 0x00000018, 0x00000A53, 
0x00050085, 0x00000007, 0x00000A57, 0x00000A56, 0x00000129, 0x00050041, 0x00000015, 0x00000A58, 0x000006E4, 0x00000098, 0x0004003D, 0x00000007, 
0x00000A59, 0x00000A58, 0x00050085, 0x00000007, 0x00000A5B, 0x00000A59, 0x00000A5A, 0x0003003E, 0x00000A5C, 0x00000A5B, 0x00050039, 0x00000007, 
0x00000A5D, 0x00000018, 0x00000A5C, 0x00050041, 0x00000015, 0x00000A5E, 0x000006E4, 0x00000098, 0x0004003D, 0x00000007, 0x00000A5F, 0x00000A5E, 
0x00050085, 0x00000007, 0x00000A61, 0x00000A5F, 0x00000A60, 0x0003003E, 0x00000A62, 0x00000A61, 0x00050039, 0x00000007, 0x00000A63, 0x00000018, 
0x00000A62, 0x00060050, 0x00000008, 0x00000A64, 0x00000A57, 0x00000A5D, 0x00000A63, 0x0005008E, 0x00000008, 0x00000A65, 0x00000A64, 0x00000285, 
0x0003003E, 0x00000873, 0x00000A65, 0x00040008, 0x00000001, 0x000003AC, 0x00000000, 0x0004003D, 0x00000008, 0x00000A66, 0x00000873, 0x0006000C, 
0x00000008, 0x00000A67, 0x00000002, 0x00000004, 0x00000A66, 0x0007000C, 0x00000008, 0x00000A69, 0x00000002, 0x0000001A, 0x00000A67, 0x00000A68, 
0x0003003E, 0x00000873, 0x00000A69, 0x00040008, 0x00000001, 0x000003AD, 0x00000000, 0x0004003D, 0x00000008, 0x00000A6A, 0x00000873, 0x0007000C, 
0x00000008, 0x00000A6B, 0x00000002, 0x00000028, 0x000009E0, 0x00000A6A, 0x0003003E, 0x00000873, 0x00000A6B, 0x00040008, 0x00000001, 0x000003AE, 
0x00000000, 0x00050041, 0x00000015, 0x00000A6C, 0x00000873, 0x00000098, 0x0004003D, 0x00000007, 0x00000A6D, 0x00000A6C, 0x00050041, 0x00000015, 
0x00000A6E, 0x00000873, 0x00000102, 0x0004003D, 0x00000007, 0x00000A6F, 0x00000A6E, 0x0007000C, 0x00000007, 0x00000A70, 0x00000002, 0x00000025, 
0x00000A6D, 0x00000A6F, 0x00050041, 0x00000015, 0x00000A71, 0x00000873, 0x00000102, 0x0003003E, 0x00000A71, 0x00000A70, 0x00040008, 0x00000001, 
0x000003AF, 0x00000000, 0x00050041, 0x00000015, 0x00000A72, 0x000006E4, 0x00000098, 0x0004003D, 0x00000007, 0x00000A73, 0x00000A72, 0x00050085, 
0x00000007, 0x00000A75, 0x00000A73, 0x00000A74, 0x0003003E, 0x00000A76, 0x00000A75, 0x00050039, 0x00000007, 0x00000A77, 0x00000018, 0x00000A76, 
0x00050085, 0x00000007, 0x00000A78, 0x00000A77, 0x00000379, 0x0004003D, 0x00000008, 0x00000A79, 0x00000873, 0x0005008E, 0x00000008, 0x00000A7A, 
0x00000A79, 0x00000A78, 0x0003003E, 0x00000873, 0x00000A7A, 0x00040008, 0x00000001, 0x000003B0, 0x00000000, 0x00050041, 0x00000015, 0x00000A7B, 
0x000006E7, 0x00000098, 0x0004003D, 0x00000007, 0x00000A7C, 0x00000A7B, 0x00050083, 0x00000007, 0x00000A7E, 0x00000A7C, 0x00000A7D, 0x0006000C, 
0x00000007, 0x00000A7F, 0x00000002, 0x00000004, 0x00000A7E, 0x00050085, 0x00000007, 0x00000A81, 0x00000A7F, 0x00000A80, 0x0007000C, 0x00000007, 
0x00000A82, 0x00000002, 0x00000028, 0x0000012A, 0x00000A81, 0x00050083, 0x00000007, 0x00000A83, 0x00000A82, 0x0000023A, 0x0003003E, 0x00000A84, 
0x00000A83, 0x00050039, 0x00000007, 0x00000A85, 0x0000003C, 0x00000A84, 0x0003003E, 0x00000A4D, 0x00000A85, 0x00040008, 0x00000001, 0x000003B2, 
0x00000000, 0x0004003D, 0x00000008, 0x00000A87, 0x0000078D, 0x0007004F, 0x00000009, 0x00000A88, 0x00000A87, 0x00000A87, 0x00000000, 0x00000002, 
0x0006000C, 0x00000009, 0x00000A89, 0x00000002, 0x00000045, 0x00000A88, 0x0006000C, 0x00000009, 0x00000A8A, 0x00000002, 0x00000004, 0x00000A89, 
0x0003003E, 0x00000A86, 0x00000A8A, 0x00040008, 0x00000001, 0x000003B4, 0x00000000, 0x00050041, 0x00000015, 0x00000A8C, 0x00000A86, 0x000000A9, 
0x0004003D, 0x00000007, 0x00000A8D, 0x00000A8C, 0x00050041, 0x00000015, 0x00000A8E, 0x00000A86, 0x00000098, 0x0004003D, 0x00000007, 0x00000A8F, 
0x00000A8E, 0x0007000C, 0x00000007, 0x00000A90, 0x00000002, 0x00000028, 0x00000A8D, 0x00000A8F, 0x00050083, 0x00000007, 0x00000A91, 0x00000129, 
0x00000A90, 0x0003003E, 0x00000A92, 0x00000A91, 0x00050039, 0x00000007, 0x00000A93, 0x0000003C, 0x00000A92, 0x0003003E, 0x00000A8B, 0x00000A93, 
0x00040008, 0x00000001, 0x000003B5, 0x00000000, 0x0004003D, 0x00000007, 0x00000A94, 0x00000A8B, 0x00050083, 0x00000007, 0x00000A95, 0x00000A94, 
0x00000379, 0x0007000C, 0x00000007, 0x00000A96, 0x00000002, 0x00000028, 0x0000012A, 0x00000A95, 0x0007000C, 0x00000007, 0x00000A97, 0x00000002, 
0x0000001A, 0x00000A96, 0x00000226, 0x0003003E, 0x00000A8B, 0x00000A97, 0x00040008, 0x00000001, 0x000003B6, 0x00000000, 0x0004003D, 0x00000007, 
0x00000A98, 0x00000A4D, 0x0004003D, 0x00000007, 0x00000A99, 0x00000A8B, 0x0007000C, 0x00000007, 0x00000A9A, 0x00000002, 0x00000028, 0x00000A98, 
0x00000A99, 0x0003003E, 0x00000A4D, 0x00000A9A, 0x00040008, 0x00000001, 0x000003B7, 0x00000000, 0x0004003D, 0x00000007, 0x00000A9B, 0x00000A4D, 
0x0004003D, 0x00000008, 0x00000A9C, 0x00000873, 0x0005008E, 0x00000008, 0x00000A9D, 0x00000A9C, 0x00000A9B, 0x0003003E, 0x00000873, 0x00000A9D, 
0x000200F9, 0x00000A52, 0x000200F8, 0x00000A52, 0x00040008, 0x00000001, 0x000003BC, 0x00000000, 0x0004003D, 0x00000008, 0x00000AA0, 0x000001E1, 
0x00050085, 0x00000008, 0x00000AA1, 0x00000A9F, 0x00000AA0, 0x0004003D, 0x00000008, 0x00000AA2, 0x000005B0, 0x0004003D, 0x00000008, 0x00000AA3, 
0x0000078D, 0x00050094, 0x00000007, 0x00000AA4, 0x00000AA2, 0x00000AA3, 0x0003003E, 0x00000AA5, 0x00000AA4, 0x00050039, 0x00000007, 0x00000AA6, 
0x0000003C, 0x00000AA5, 0x0005008E, 0x00000008, 0x00000AA7, 0x00000AA1, 0x00000AA6, 0x0004003D, 0x00000007, 0x00000AA8, 0x000007E5, 0x0005008E, 
0x00000008, 0x00000AA9, 0x00000AA7, 0x00000AA8, 0x0003003E, 0x00000A9E, 0x00000AA9, 0x00040008, 0x00000001, 0x000003BE, 0x00000000, 0x0004003D, 
0x00000007, 0x00000AAB, 0x000007D9, 0x00050085, 0x00000007, 0x00000AAC, 0x00000AAB, 0x00000114, 0x0004003D, 0x00000007, 0x00000AAD, 0x00000790, 
0x00050081, 0x00000007, 0x00000AAE, 0x00000AAC, 0x00000AAD, 0x00050085, 0x00000007, 0x00000AAF, 0x00000AAE, 0x00000218, 0x0003003E, 0x00000AAA, 
0x00000AAF, 0x00040008, 0x00000001, 0x000003BF, 0x00000000, 0x0004003D, 0x00000008, 0x00000AB0, 0x000001EC, 0x00050041, 0x00000015, 0x00000AB1, 
0x0000078D, 0x00000098, 0x0004003D, 0x00000007, 0x00000AB2, 0x00000AB1, 0x00050085, 0x00000007, 0x00000AB3, 0x00000AB2, 0x000001CC, 0x00050081, 
0x00000007, 0x00000AB4, 0x00000AB3, 0x000001CC, 0x0003003E, 0x00000AB5, 0x00000AB4, 0x00050039, 0x00000007, 0x00000AB6, 0x0000003C, 0x00000AB5, 
0x0005008E, 0x00000008, 0x00000AB7, 0x00000AB0, 0x00000AB6, 0x0004003D, 0x00000007, 0x00000AB8, 0x00000AAA, 0x0007000C, 0x00000007, 0x00000AB9, 
0x00000002, 0x0000001A, 0x00000AB8, 0x00000477, 0x0005008E, 0x00000008, 0x00000ABA, 0x00000AB7, 0x00000AB9, 0x0005008E, 0x00000008, 0x00000ABB, 
0x00000ABA, 0x0000028F, 0x0004003D, 0x00000008, 0x00000ABC, 0x00000A9E, 0x00050081, 0x00000008, 0x00000ABD, 0x00000ABC, 0x00000ABB, 0x0003003E, 
0x00000A9E, 0x00000ABD, 0x00040008, 0x00000001, 0x000003C0, 0x00000000, 0x0004003D, 0x00000008, 0x00000ABE, 0x00000A9E, 0x0005008E, 0x00000008, 
0x00000ABF, 0x00000ABE, 0x00000214, 0x0003003E, 0x00000A9E, 0x00000ABF, 0x00040008, 0x00000001, 0x000003C1, 0x00000000, 0x0004003D, 0x00000008, 
0x00000AC0, 0x00000873, 0x0004003D, 0x00000008, 0x00000AC1, 0x00000A9E, 0x00050085, 0x00000008, 0x00000AC2, 0x00000AC0, 0x00000AC1, 0x0003003E, 
0x00000762, 0x00000AC2, 0x00040008, 0x00000001, 0x000003C2, 0x00000000, 0x00050041, 0x00000015, 0x00000AC3, 0x000006E4, 0x00000098, 0x0004003D, 
0x00000007, 0x00000AC4, 0x00000AC3, 0x000500BE, 0x000002C0, 0x00000AC5, 0x00000AC4, 0x000006BB, 0x000300F7, 0x00000AC7, 0x00000000, 0x000400FA, 
0x00000AC5, 0x00000AC6, 0x00000AC7, 0x000200F8, 0x00000AC6, 0x00040008, 0x00000001, 0x000003C5, 0x00000000, 0x00050041, 0x00000015, 0x00000AC9, 
0x000007E1, 0x00000098, 0x0004003D, 0x00000007, 0x00000ACA, 0x00000AC9, 0x00050085, 0x00000007, 0x00000ACB, 0x00000ACA, 0x000006BB, 0x0007000C, 
0x00000007, 0x00000ACC, 0x00000002, 0x00000025, 0x00000129, 0x00000ACB, 0x0007000C, 0x00000007, 0x00000ACD, 0x00000002, 0x00000028, 0x0000030B, 
0x00000ACC, 0x0003003E, 0x00000AC8, 0x00000ACD, 0x00040008, 0x00000001, 0x000003C6, 0x00000000, 0x0004003D, 0x00000007, 0x00000ACE, 0x00000A4D, 
0x000400CF, 0x00000007, 0x00000ACF, 0x00000ACE, 0x0004003D, 0x00000007, 0x00000AD0, 0x00000A4D, 0x000400D0, 0x00000007, 0x00000AD1, 0x00000AD0, 
0x00050085, 0x00000007, 0x00000AD2, 0x00000ACF, 0x00000AD1, 0x0006000C, 0x00000007, 0x00000AD3, 0x00000002, 0x00000004, 0x00000AD2, 0x00050085, 
0x00000007, 0x00000AD5, 0x00000AD3, 0x00000AD4, 0x00050083, 0x00000007, 0x00000AD6, 0x00000129, 0x00000AD5, 0x0003003E, 0x00000AD7, 0x00000AD6, 
0x00050039, 0x00000007, 0x00000AD8, 0x0000003C, 0x00000AD7, 0x0004003D, 0x00000007, 0x00000AD9, 0x00000AC8, 0x00050085, 0x00000007, 0x00000ADA, 
0x00000AD9, 0x00000AD8, 0x0003003E, 0x00000AC8, 0x00000ADA, 0x00040008, 0x00000001, 0x000003C7, 0x00000000, 0x00050041, 0x00000015, 0x00000ADB, 
0x000006E7, 0x00000098, 0x0004003D, 0x00000007, 0x00000ADC, 0x00000ADB, 0x00050083, 0x00000007, 0x00000ADD, 0x00000ADC, 0x00000377, 0x0004003D, 
0x00000008, 0x00000ADF, 0x000007E1, 0x0003003E, 0x00000ADE, 0x00000ADF, 0x0004003D, 0x00000008, 0x00000AE1, 0x000005B0, 0x0003003E, 0x00000AE0, 
0x00000AE1, 0x0003003E, 0x00000AE2, 0x00000ADD, 0x00070039, 0x00000008, 0x00000AE3, 0x0000004B, 0x00000ADE, 0x00000AE0, 0x00000AE2, 0x0005008E, 
0x00000008, 0x00000AE4, 0x00000AE3, 0x0000041A, 0x0004003D, 0x00000007, 0x00000AE5, 0x00000AC8, 0x0005008E, 0x00000008, 0x00000AE6, 0x00000AE4, 
0x00000AE5, 0x0004003D, 0x00000007, 0x00000AE7, 0x000007E5, 0x0007000C, 0x00000007, 0x00000AE8, 0x00000002, 0x00000028, 0x000003AD, 0x00000AE7, 
0x0005008E, 0x00000008, 0x00000AE9, 0x00000AE6, 0x00000AE8, 0x0004003D, 0x00000008, 0x00000AEA, 0x00000762, 0x00050081, 0x00000008, 0x00000AEB, 
0x00000AEA, 0x00000AE9, 0x0003003E, 0x00000762, 0x00000AEB, 0x000200F9, 0x00000AC7, 0x000200F8, 0x00000AC7, 0x00040008, 0x00000001, 0x000003CB, 
0x00000000, 0x0004003D, 0x00000007, 0x00000AEC, 0x00000877, 0x000500B6, 0x000002C0, 0x00000AED, 0x00000AEC, 0x0000012A, 0x000300F7, 0x00000AEF, 
0x00000000, 0x000400FA, 0x00000AED, 0x00000AEE, 0x00000AEF, 0x000200F8, 0x00000AEE, 0x00040008, 0x00000001, 0x000003CD, 0x00000000, 0x0004003D, 
0x00000007, 0x00000AF0, 0x00000877, 0x0008000C, 0x00000007, 0x00000AF1, 0x00000002, 0x0000002E, 0x00000129, 0x00000273, 0x00000AF0, 0x0004003D, 
0x00000008, 0x00000AF2, 0x00000762, 0x0005008E, 0x00000008, 0x00000AF3, 0x00000AF2, 0x00000AF1, 0x0003003E, 0x00000762, 0x00000AF3, 0x00040008, 
0x00000001, 0x000003CF, 0x00000000, 0x00050041, 0x00000015, 0x00000AF5, 0x000007E1, 0x00000098, 0x0004003D, 0x00000007, 0x00000AF6, 0x00000AF5, 
0x00050085, 0x00000007, 0x00000AF7, 0x00000AF6, 0x000006BB, 0x0007000C, 0x00000007, 0x00000AF8, 0x00000002, 0x00000025, 0x00000129, 0x00000AF7, 
0x0007000C, 0x00000007, 0x00000AF9, 0x00000002, 0x00000028, 0x0000030B, 0x00000AF8, 0x0003003E, 0x00000AF4, 0x00000AF9, 0x00040008, 0x00000001, 
0x000003D0, 0x00000000, 0x00050041, 0x00000015, 0x00000AFA, 0x000006E7, 0x00000098, 0x0004003D, 0x00000007, 0x00000AFB, 0x00000AFA, 0x00050083, 
0x00000007, 0x00000AFC, 0x00000AFB, 0x000001CC, 0x0004003D, 0x00000008, 0x00000AFE, 0x000007E1, 0x0003003E, 0x00000AFD, 0x00000AFE, 0x0004003D, 
0x00000008, 0x00000B00, 0x000005B0, 0x0003003E, 0x00000AFF, 0x00000B00, 0x0003003E, 0x00000B01, 0x00000AFC, 0x00070039, 0x00000008, 0x00000B02, 
0x0000004B, 0x00000AFD, 0x00000AFF, 0x00000B01, 0x0005008E, 0x00000008, 0x00000B03, 0x00000B02, 0x00000273, 0x0004003D, 0x00000007, 0x00000B04, 
0x00000AF4, 0x0005008E, 0x00000008, 0x00000B05, 0x00000B03, 0x00000B04, 0x0004003D, 0x00000007, 0x00000B06, 0x000007E5, 0x0007000C, 0x00000007, 
0x00000B07, 0x00000002, 0x00000028, 0x00000273, 0x00000B06, 0x0005008E, 0x00000008, 0x00000B08, 0x00000B05, 0x00000B07, 0x0004003D, 0x00000007, 
0x00000B09, 0x00000877, 0x0005008E, 0x00000008, 0x00000B0A, 0x00000B08, 0x00000B09, 0x0004003D, 0x00000008, 0x00000B0B, 0x00000762, 0x00050081, 
0x00000008, 0x00000B0C, 0x00000B0B, 0x00000B0A, 0x0003003E, 0x00000762, 0x00000B0C, 0x000200F9, 0x00000AEF, 0x000200F8, 0x00000AEF, 0x00040008, 
0x00000001, 0x000003D4, 0x00000000, 0x0004003D, 0x00000008, 0x00000B0D, 0x00000762, 0x0005008E, 0x00000008, 0x00000B0E, 0x00000B0D, 0x000005B4, 
0x0003003E, 0x00000762, 0x00000B0E, 0x00040008, 0x00000001, 0x000003D6, 0x00000000, 0x0004003D, 0x00000008, 0x00000B10, 0x000006E2, 0x0003003E, 
0x00000B0F, 0x00000B10, 0x00040008, 0x00000001, 0x000003D7, 0x00000000, 0x00050041, 0x00000015, 0x00000B11, 0x00000B0F, 0x00000098, 0x0004003D, 
0x00000007, 0x00000B12, 0x00000B11, 0x0006000C, 0x00000007, 0x00000B13, 0x00000002, 0x00000006, 0x00000B12, 0x0003003E, 0x00000B14, 0x00000B13, 
0x00050039, 0x00000007, 0x00000B15, 0x0000003C, 0x00000B14, 0x00050041, 0x00000015, 0x00000B16, 0x00000B0F, 0x00000098, 0x0004003D, 0x00000007, 
0x00000B17, 0x00000B16, 0x00050085, 0x00000007, 0x00000B18, 0x00000B17, 0x00000B15, 0x00050041, 0x00000015, 0x00000B19, 0x00000B0F, 0x00000098, 
0x0003003E, 0x00000B19, 0x00000B18, 0x00040008, 0x00000001, 0x000003D9, 0x00000000, 0x0004003D, 0x00000008, 0x00000B1C, 0x00000B0F, 0x0003003E, 
0x00000B1B, 0x00000B1C, 0x0004003D, 0x00000008, 0x00000B1E, 0x000005B0, 0x0003003E, 0x00000B1D, 0x00000B1E, 0x00060039, 0x00000008, 0x00000B1F, 
0x00000045, 0x00000B1B, 0x00000B1D, 0x0003003E, 0x00000B1A, 0x00000B1F, 0x00040008, 0x00000001, 0x000003DA, 0x00000000, 0x0004003D, 0x00000008, 
0x00000B22, 0x00000B1A, 0x0007000C, 0x00000008, 0x00000B23, 0x00000002, 0x00000025, 0x00000B21, 0x00000B22, 0x0003003E, 0x00000B1A, 0x00000B23, 
0x00040008, 0x00000001, 0x000003DB, 0x00000000, 0x0004003D, 0x00000008, 0x00000B24, 0x00000B1A, 0x0004003D, 0x00000008, 0x00000B25, 0x00000762, 
0x0004003D, 0x00000007, 0x00000B26, 0x000006E6, 0x0004007F, 0x00000007, 0x00000B27, 0x00000B26, 0x00050085, 0x00000007, 0x00000B28, 0x00000B27, 
0x0000032B, 0x0006000C, 0x00000007, 0x00000B29, 0x00000002, 0x0000001B, 0x00000B28, 0x00060050, 0x00000008, 0x00000B2A, 0x00000B29, 0x00000B29, 
0x00000B29, 0x0008000C, 0x00000008, 0x00000B2B, 0x00000002, 0x0000002E, 0x00000B24, 0x00000B25, 0x00000B2A, 0x0003003E, 0x00000762, 0x00000B2B, 
0x000200F9, 0x0000076A, 0x000200F8, 0x00000B2C, 0x00040008, 0x00000001, 0x000003E0, 0x00000000, 0x0004003D, 0x00000008, 0x00000B2E, 0x000006E2, 
0x0003003E, 0x00000B2D, 0x00000B2E, 0x0004003D, 0x00000008, 0x00000B30, 0x000005B0, 0x0003003E, 0x00000B2F, 0x00000B30, 0x00060039, 0x00000008, 
0x00000B31, 0x00000045, 0x00000B2D, 0x00000B2F, 0x0003003E, 0x00000762, 0x00000B31, 0x000200F9, 0x0000076A, 0x000200F8, 0x0000076A, 0x00040008, 
0x00000001, 0x000003E4, 0x00000000, 0x0004003D, 0x00000009, 0x00000B32, 0x000005BF, 0x00050050, 0x00000009, 0x00000B33, 0x0000028F, 0x0000028F, 
0x00050088, 0x00000009, 0x00000B34, 0x00000B32, 0x00000B33, 0x0006000C, 0x00000007, 0x00000B35, 0x00000002, 0x00000042, 0x00000B34, 0x00050083, 
0x00000007, 0x00000B36, 0x00000129, 0x00000B35, 0x0003003E, 0x00000B37, 0x00000B36, 0x00050039, 0x00000007, 0x00000B38, 0x0000003C, 0x00000B37, 
0x0005008E, 0x00000008, 0x00000B39, 0x00000183, 0x00000B38, 0x0004003D, 0x00000008, 0x00000B3A, 0x00000762, 0x00050085, 0x00000008, 0x00000B3B, 
0x00000B3A, 0x00000B39, 0x0003003E, 0x00000762, 0x00000B3B, 0x00040008, 0x00000001, 0x000003E5, 0x00000000, 0x0004003D, 0x00000007, 0x00000B3C, 
0x000005B9, 0x00050085, 0x00000007, 0x00000B3D, 0x00000547, 0x00000B3C, 0x0004003D, 0x00000008, 0x00000B3E, 0x00000762, 0x0005008E, 0x00000008, 
0x00000B3F, 0x00000B3E, 0x00000B3D, 0x0003003E, 0x00000762, 0x00000B3F, 0x00040008, 0x00000001, 0x000003E6, 0x00000000, 0x0004003D, 0x00000008, 
0x00000B40, 0x00000762, 0x00060050, 0x00000008, 0x00000B41, 0x0000012A, 0x0000012A, 0x0000012A, 0x00060050, 0x00000008, 0x00000B42, 0x00000129, 
0x00000129, 0x00000129, 0x0008000C, 0x00000008, 0x00000B43, 0x00000002, 0x0000002B, 0x00000B40, 0x00000B41, 0x00000B42, 0x0004003D, 0x00000007, 
0x00000B44, 0x000005BA, 0x00050081, 0x00000007, 0x00000B45, 0x00000B44, 0x0000022E, 0x0003003E, 0x00000B46, 0x00000B45, 0x00050039, 0x00000007, 
0x00000B47, 0x0000003C, 0x00000B46, 0x0005008E, 0x00000008, 0x00000B48, 0x00000B43, 0x00000B47, 0x00050051, 0x00000007, 0x00000B49, 0x00000B48, 
0x00000000, 0x00050051, 0x00000007, 0x00000B4A, 0x00000B48, 0x00000001, 0x00050051, 0x00000007, 0x00000B4B, 0x00000B48, 0x00000002, 0x00060050, 
0x00000008, 0x00000B4C, 0x00000B49, 0x00000B4A, 0x00000B4B, 0x000200FE, 0x00000B4C, 0x00010038, 0x00050036, 0x00000003, 0x00000087, 0x00000000, 
0x00000084, 0x00030037, 0x00000083, 0x00000085, 0x00030037, 0x0000001A, 0x00000086, 0x000200F8, 0x00000088, 0x0004003B, 0x000000B6, 0x00000B4F, 
0x00000007, 0x0004003B, 0x00000026, 0x00000B69, 0x00000007, 0x00040008, 0x00000001, 0x000003ED, 0x00000000, 0x00060041, 0x000005C0, 0x00000B51, 
0x00000096, 0x00000B50, 0x00000097, 0x0004003D, 0x00000082, 0x00000B52, 0x00000B51, 0x0008004F, 0x00000008, 0x00000B53, 0x00000B52, 0x00000B52, 
0x00000000, 0x00000001, 0x00000002, 0x00060041, 0x000005C0, 0x00000B54, 0x00000096, 0x00000B50, 0x000000BD, 0x0004003D, 0x00000082, 0x00000B55, 
0x00000B54, 0x0008004F, 0x00000008, 0x00000B56, 0x00000B55, 0x00000B55, 0x00000000, 0x00000001, 0x00000002, 0x00060041, 0x000005C0, 0x00000B57, 
0x00000096, 0x00000B50, 0x000000B9, 0x0004003D, 0x00000082, 0x00000B58, 0x00000B57, 0x0008004F, 0x00000008, 0x00000B59, 0x00000B58, 0x00000B58, 
0x00000000, 0x00000001, 0x00000002, 0x00060041, 0x000005C0, 0x00000B5B, 0x00000096, 0x00000B50, 0x00000B5A, 0x0004003D, 0x00000082, 0x00000B5C, 
0x00000B5B, 0x0008004F, 0x00000008, 0x00000B5D, 0x00000B5C, 0x00000B5C, 0x00000000, 0x00000001, 0x00000002, 0x00050041, 0x000005C0, 0x00000B5F, 
0x00000096, 0x00000B5E, 0x0004003D, 0x00000082, 0x00000B60, 0x00000B5F, 0x0008004F, 0x00000008, 0x00000B61, 0x00000B60, 0x00000B60, 0x00000000, 
0x00000001, 0x00000002, 0x0004003D, 0x00000009, 0x00000B62, 0x00000086, 0x00050041, 0x000005C0, 0x00000B63, 0x00000096, 0x00000097, 0x0004003D, 
0x00000082, 0x00000B64, 0x00000B63, 0x0007004F, 0x00000009, 0x00000B65, 0x00000B64, 0x00000B64, 0x00000000, 0x00000001, 0x00050088, 0x00000009, 
0x00000B66, 0x00000B62, 0x00000B65, 0x000B0039, 0x0000000A, 0x00000B67, 0x00000013, 0x00000B53, 0x00000B56, 0x00000B59, 0x00000B5D, 0x00000B61, 
0x0000023A, 0x00000B66, 0x0003003E, 0x00000B4F, 0x00000B67, 0x00040008, 0x00000001, 0x000003EE, 0x00000000, 0x0003003E, 0x00000499, 0x0000012A, 
0x00040008, 0x00000001, 0x000003EF, 0x00000000, 0x0003003E, 0x00000B68, 0x00000129, 0x00040008, 0x00000001, 0x000003F0, 0x00000000, 0x0003003E, 
0x000005BA, 0x00000129, 0x00040008, 0x00000001, 0x000003F1, 0x00000000, 0x0003003E, 0x000005B9, 0x00000129, 0x00040008, 0x00000001, 0x000003F2, 
0x00000000, 0x0003003E, 0x000005AB, 0x0000012A, 0x00040008, 0x00000001, 0x000003F3, 0x00000000, 0x0003003E, 0x00000727, 0x0000022E, 0x00040008, 
0x00000001, 0x000003F5, 0x00000000, 0x0003003E, 0x00000B69, 0x000005BC, 0x00040008, 0x00000001, 0x000003F6, 0x00000000, 0x00050041, 0x00000099, 
0x00000B6A, 0x00000096, 0x000000BD, 0x0004003D, 0x00000007, 0x00000B6B, 0x00000B6A, 0x0003003E, 0x00000499, 0x00000B6B, 0x00040008, 0x00000001, 
0x000003F7, 0x00000000, 0x0004003D, 0x00000009, 0x00000B6C, 0x00000086, 0x0004003D, 0x0000000A, 0x00000B6D, 0x00000B4F, 0x00060039, 0x00000008, 
0x00000B6E, 0x00000080, 0x00000B6C, 0x00000B6D, 0x0003003E, 0x00000B69, 0x00000B6E, 0x00040008, 0x00000001, 0x000003F8, 0x00000000, 0x0004003D, 
0x00000008, 0x00000B6F, 0x00000B69, 0x00060050, 0x00000008, 0x00000B70, 0x0000012A, 0x0000012A, 0x0000012A, 0x00060050, 0x00000008, 0x00000B71, 
0x00000129, 0x00000129, 0x00000129, 0x0008000C, 0x00000008, 0x00000B72, 0x00000002, 0x0000002B, 0x00000B6F, 0x00000B70, 0x00000B71, 0x0006000C, 
0x00000008, 0x00000B73, 0x00000002, 0x0000001F, 0x00000B72, 0x00050051, 0x00000007, 0x00000B74, 0x00000B73, 0x00000000, 0x00050051, 0x00000007, 
0x00000B75, 0x00000B73, 0x00000001, 0x00050051, 0x00000007, 0x00000B76, 0x00000B73, 0x00000002, 0x00070050, 0x00000082, 0x00000B77, 0x00000B74, 
0x00000B75, 0x00000B76, 0x00000129, 0x0003003E, 0x00000085, 0x00000B77, 0x000100FD, 0x00010038 });
	descr.Fragment().StringSpirvAsm( 
R"#(// Module Version 10000
// Generated by (magic number): 80002
// Id's are bound by 2936

                              Capability Shader
               2:             ExtInstImport  "GLSL.std.450"
                              MemoryModel Logical GLSL450
                              EntryPoint Fragment 5  "main" 139 161
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
                              Name 10  "Ray"
                              MemberName 10(Ray) 0  "origin"
                              MemberName 10(Ray) 1  "pos"
                              MemberName 10(Ray) 2  "dir"
                              Name 19  "Ray_From(vf3;vf3;vf3;vf3;vf3;f1;vf2;"
                              Name 12  "leftBottom"
                              Name 13  "rightBottom"
                              Name 14  "leftTop"
                              Name 15  "rightTop"
                              Name 16  "origin"
                              Name 17  "nearPlane"
                              Name 18  "unormCoord"
                              Name 24  "Hash11(f1;"
                              Name 23  "a"
                              Name 29  "Hash21(vf2;"
                              Name 28  "uv"
                              Name 33  "Hash22(vf2;"
                              Name 32  "uv"
                              Name 36  "Hash2d(vf2;"
                              Name 35  "uv"
                              Name 41  "Hash3d(vf3;"
                              Name 40  "uv"
                              Name 47  "mixP(f1;f1;f1;"
                              Name 44  "f0"
                              Name 45  "f1"
                              Name 46  "a"
                              Name 50  "noise2d(vf2;"
                              Name 49  "uv"
                              Name 53  "noise(vf3;"
                              Name 52  "uv"
                              Name 57  "saturate(vf3;"
                              Name 56  "a"
                              Name 60  "saturate(f1;"
                              Name 59  "a"
                              Name 65  "GetSunColorSmall(vf3;vf3;"
                              Name 63  "rayDir"
                              Name 64  "sunDir"
                              Name 69  "GetEnvMap(vf3;vf3;"
                              Name 67  "rayDir"
                              Name 68  "sunDir"
                              Name 75  "GetEnvMapSkyline(vf3;vf3;f1;"
                              Name 72  "rayDir"
                              Name 73  "sunDir"
                              Name 74  "height"
                              Name 80  "matmin(vf2;vf2;"
                              Name 78  "a"
                              Name 79  "b"
                              Name 85  "sdBox(vf3;vf3;"
                              Name 83  "p"
                              Name 84  "radius"
                              Name 91  "cylCap(vf3;f1;f1;"
                              Name 88  "p"
                              Name 89  "r"
                              Name 90  "lenRad"
                              Name 96  "smin(f1;f1;f1;"
                              Name 93  "a"
                              Name 94  "b"
                              Name 95  "k"
                              Name 101  "Repeat(f1;f1;"
                              Name 99  "a"
                              Name 100  "len"
                              Name 106  "Car(vf3;f1;"
                              Name 104  "baseCenter"
                              Name 105  "unique"
                              Name 111  "CityBlock(vf3;vf2;"
                              Name 109  "p"
                              Name 110  "pint"
                              Name 115  "DistanceToObject(vf3;"
                              Name 114  "p"
                              Name 123  "CalcWindows(vf2;vf3;vf3;f1;vf3;"
                              Name 118  "block"
                              Name 119  "pos"
                              Name 120  "texColor"
                              Name 121  "windowRef"
                              Name 122  "normal"
                              Name 128  "RayTrace(vf2;struct-Ray-vf3-vf3-vf31;"
                              Name 126  "fragCoord"
                              Name 127  "ray"
                              Name 135  "mainImage(vf4;vf2;"
                              Name 133  "fragColor"
                              Name 134  "fragCoord"
                              Name 137  "coord"
                              Name 139  "gl_FragCoord"
                              Name 148  "ShadertoyUB"
                              MemberName 148(ShadertoyUB) 0  "iResolution"
                              MemberName 148(ShadertoyUB) 1  "iTime"
                              MemberName 148(ShadertoyUB) 2  "iTimeDelta"
                              MemberName 148(ShadertoyUB) 3  "iFrame"
                              MemberName 148(ShadertoyUB) 4  "_padding0"
                              MemberName 148(ShadertoyUB) 5  "iChannelTime"
                              MemberName 148(ShadertoyUB) 6  "iChannelResolution"
                              MemberName 148(ShadertoyUB) 7  "iMouse"
                              MemberName 148(ShadertoyUB) 8  "iDate"
                              MemberName 148(ShadertoyUB) 9  "iSampleRate"
                              MemberName 148(ShadertoyUB) 10  "_padding1"
                              MemberName 148(ShadertoyUB) 11  "_padding2"
                              MemberName 148(ShadertoyUB) 12  "_padding3"
                              MemberName 148(ShadertoyUB) 13  "iCameraFrustum"
                              MemberName 148(ShadertoyUB) 14  "iCameraPos"
                              Name 150  ""
                              Name 161  "out_Color0"
                              Name 162  "param"
                              Name 163  "param"
                              Name 167  "coord"
                              Name 168  "vec"
                              Name 183  "ray"
                              Name 207  "f"
                              Name 222  "f"
                              Name 238  "f"
                              Name 251  "f"
                              Name 285  "fr"
                              Name 288  "fl"
                              Name 291  "h00"
                              Name 292  "param"
                              Name 295  "h10"
                              Name 301  "param"
                              Name 303  "h01"
                              Name 307  "param"
                              Name 309  "h11"
                              Name 313  "param"
                              Name 315  "param"
                              Name 317  "param"
                              Name 319  "param"
                              Name 323  "param"
                              Name 325  "param"
                              Name 327  "param"
                              Name 331  "param"
                              Name 332  "param"
                              Name 333  "param"
                              Name 339  "fr"
                              Name 342  "fl"
                              Name 345  "h000"
                              Name 346  "param"
                              Name 349  "h100"
                              Name 353  "param"
                              Name 355  "h010"
                              Name 359  "param"
                              Name 361  "h110"
                              Name 365  "param"
                              Name 367  "h001"
                              Name 371  "param"
                              Name 373  "h101"
                              Name 377  "param"
                              Name 379  "h011"
                              Name 383  "param"
                              Name 385  "h111"
                              Name 389  "param"
                              Name 391  "param"
                              Name 393  "param"
                              Name 395  "param"
                              Name 399  "param"
                              Name 401  "param"
                              Name 403  "param"
                              Name 407  "param"
                              Name 408  "param"
                              Name 409  "param"
                              Name 413  "param"
                              Name 415  "param"
                              Name 417  "param"
                              Name 421  "param"
                              Name 423  "param"
                              Name 425  "param"
                              Name 429  "param"
                              Name 430  "param"
                              Name 431  "param"
                              Name 435  "param"
                              Name 436  "param"
                              Name 437  "param"
                              Name 453  "localRay"
                              Name 456  "dist"
                              Name 464  "sunIntensity"
                              Name 481  "sunCol"
                              Name 489  "finalColor"
                              Name 490  "horizonCol"
                              Name 492  "skyCol"
                              Name 494  "param"
                              Name 504  "n"
                              Name 512  "param"
                              Name 521  "param"
                              Name 534  "param"
                              Name 548  "param"
                              Name 565  "param"
                              Name 580  "param"
                              Name 585  "param"
                              Name 589  "param"
                              Name 591  "param"
                              Name 599  "finalColor"
                              Name 600  "param"
                              Name 602  "param"
                              Name 605  "radial"
                              Name 612  "skyline"
                              Name 649  "mask"
                              Name 661  "param"
                              Name 663  "vert"
                              Name 671  "hor"
                              Name 687  "param"
                              Name 714  "dist"
                              Name 735  "a"
                              Name 773  "car"
                              Name 783  "param"
                              Name 784  "param"
                              Name 794  "param"
                              Name 795  "param"
                              Name 798  "param"
                              Name 800  "param"
                              Name 801  "param"
                              Name 803  "wMirror"
                              Name 814  "wheels"
                              Name 817  "param"
                              Name 820  "param"
                              Name 821  "param"
                              Name 823  "distAndMat"
                              Name 831  "param"
                              Name 833  "param"
                              Name 838  "rand"
                              Name 839  "param"
                              Name 844  "param"
                              Name 850  "rand2"
                              Name 851  "param"
                              Name 855  "baseRad"
                              Name 866  "baseCenter"
                              Name 870  "height"
                              Name 878  "downtown"
                              Name 882  "param"
                              Name 901  "d"
                              Name 906  "param"
                              Name 908  "param"
                              Name 914  "height2"
                              Name 944  "param"
                              Name 945  "param"
                              Name 961  "param"
                              Name 962  "param"
                              Name 982  "param"
                              Name 983  "param"
                              Name 986  "topWidth"
                              Name 1008  "param"
                              Name 1009  "param"
                              Name 1025  "param"
                              Name 1027  "param"
                              Name 1028  "param"
                              Name 1065  "param"
                              Name 1066  "param"
                              Name 1069  "boxPos"
                              Name 1086  "big"
                              Name 1119  "param"
                              Name 1121  "param"
                              Name 1140  "distAndMat"
                              Name 1145  "param"
                              Name 1147  "param"
                              Name 1150  "param"
                              Name 1152  "param"
                              Name 1157  "rep"
                              Name 1164  "distAndMat"
                              Name 1168  "param"
                              Name 1170  "param"
                              Name 1172  "p2"
                              Name 1175  "carTime"
                              Name 1177  "localTime"
                              Name 1180  "crossStreet"
                              Name 1181  "repeatDist"
                              Name 1215  "param"
                              Name 1216  "param"
                              Name 1242  "uniqueID"
                              Name 1248  "param"
                              Name 1251  "param"
                              Name 1255  "param"
                              Name 1265  "frontBack"
                              Name 1269  "param"
                              Name 1288  "carDist"
                              Name 1289  "param"
                              Name 1291  "param"
                              Name 1294  "param"
                              Name 1296  "param"
                              Name 1302  "hue"
                              Name 1303  "param"
                              Name 1310  "param"
                              Name 1316  "param"
                              Name 1326  "window"
                              Name 1343  "winWidth"
                              Name 1347  "param"
                              Name 1390  "param"
                              Name 1397  "wave"
                              Name 1419  "pits"
                              Name 1451  "marchCount"
                              Name 1456  "sunDir"
                              Name 1465  "exposure"
                              Name 1466  "fade"
                              Name 1467  "camPos"
                              Name 1469  "camUp"
                              Name 1470  "camLookat"
                              Name 1471  "uv"
                              Name 1492  "time"
                              Name 1495  "alpha"
                              Name 1498  "param"
                              Name 1503  "param"
                              Name 1526  "time"
                              Name 1529  "alpha"
                              Name 1533  "param"
                              Name 1538  "param"
                              Name 1577  "time")#"_str
+
R"#(
                              Name 1580  "alpha"
                              Name 1583  "param"
                              Name 1588  "param"
                              Name 1626  "time"
                              Name 1629  "alpha"
                              Name 1632  "param"
                              Name 1637  "param"
                              Name 1670  "time"
                              Name 1673  "alpha"
                              Name 1677  "param"
                              Name 1682  "param"
                              Name 1726  "camVec"
                              Name 1731  "sideNorm"
                              Name 1736  "upNorm"
                              Name 1740  "worldFacing"
                              Name 1744  "worldPix"
                              Name 1762  "rayVec"
                              Name 1764  "distAndMat"
                              Name 1766  "t"
                              Name 1767  "pos"
                              Name 1769  "i"
                              Name 1785  "param"
                              Name 1788  "walk"
                              Name 1791  "dx"
                              Name 1805  "dz"
                              Name 1819  "nearestVoxel"
                              Name 1831  "voxelPad"
                              Name 1860  "alpha"
                              Name 1890  "finalColor"
                              Name 1899  "dist"
                              Name 1902  "smallVec"
                              Name 1904  "normalU"
                              Name 1910  "param"
                              Name 1919  "param"
                              Name 1928  "param"
                              Name 1933  "normal"
                              Name 1936  "ambientS"
                              Name 1941  "param"
                              Name 1945  "param"
                              Name 1953  "param"
                              Name 1957  "param"
                              Name 1965  "param"
                              Name 1969  "param"
                              Name 1977  "param"
                              Name 1981  "param"
                              Name 1989  "param"
                              Name 1993  "param"
                              Name 2001  "param"
                              Name 2005  "param"
                              Name 2009  "ambient"
                              Name 2014  "param"
                              Name 2017  "ref"
                              Name 2021  "sunShadow"
                              Name 2022  "iter"
                              Name 2023  "nudgePos"
                              Name 2029  "i"
                              Name 2038  "shadowPos"
                              Name 2044  "tempDist"
                              Name 2045  "param"
                              Name 2051  "param"
                              Name 2060  "walk"
                              Name 2062  "dx"
                              Name 2076  "dz"
                              Name 2090  "nearestVoxel"
                              Name 2120  "param"
                              Name 2123  "n"
                              Name 2126  "param"
                              Name 2133  "param"
                              Name 2140  "param"
                              Name 2146  "param"
                              Name 2153  "param"
                              Name 2159  "block"
                              Name 2163  "texColor"
                              Name 2167  "windowRef"
                              Name 2179  "posdx"
                              Name 2182  "posdy"
                              Name 2185  "posGrad"
                              Name 2187  "param"
                              Name 2195  "param"
                              Name 2199  "colTotal"
                              Name 2200  "colTemp"
                              Name 2202  "nTemp"
                              Name 2203  "param"
                              Name 2205  "param"
                              Name 2207  "param"
                              Name 2209  "param"
                              Name 2211  "param"
                              Name 2224  "param"
                              Name 2226  "param"
                              Name 2227  "param"
                              Name 2229  "param"
                              Name 2231  "param"
                              Name 2248  "param"
                              Name 2250  "param"
                              Name 2251  "param"
                              Name 2253  "param"
                              Name 2255  "param"
                              Name 2269  "param"
                              Name 2271  "param"
                              Name 2272  "param"
                              Name 2274  "param"
                              Name 2276  "param"
                              Name 2294  "param"
                              Name 2296  "param"
                              Name 2297  "param"
                              Name 2299  "param"
                              Name 2301  "param"
                              Name 2320  "xroad"
                              Name 2327  "zroad"
                              Name 2334  "road"
                              Name 2342  "param"
                              Name 2350  "param"
                              Name 2362  "yellowLine"
                              Name 2369  "param"
                              Name 2376  "param"
                              Name 2390  "param"
                              Name 2399  "whiteLine"
                              Name 2407  "param"
                              Name 2415  "param"
                              Name 2428  "param"
                              Name 2439  "param"
                              Name 2449  "param"
                              Name 2465  "param"
                              Name 2473  "param"
                              Name 2486  "param"
                              Name 2494  "crossWalk"
                              Name 2501  "param"
                              Name 2506  "param"
                              Name 2515  "param"
                              Name 2538  "param"
                              Name 2543  "param"
                              Name 2551  "param"
                              Name 2567  "sidewalk"
                              Name 2568  "blockSize"
                              Name 2589  "param"
                              Name 2604  "param"
                              Name 2610  "param"
                              Name 2616  "param"
                              Name 2634  "param"
                              Name 2637  "windowMask"
                              Name 2643  "param"
                              Name 2652  "param"
                              Name 2658  "param"
                              Name 2678  "param"
                              Name 2692  "param"
                              Name 2694  "dirNorm"
                              Name 2699  "pillars"
                              Name 2706  "param"
                              Name 2718  "lightColor"
                              Name 2725  "param"
                              Name 2730  "ambientAvg"
                              Name 2741  "param"
                              Name 2760  "yfade"
                              Name 2775  "param"
                              Name 2782  "param"
                              Name 2784  "param"
                              Name 2786  "param"
                              Name 2804  "yfade"
                              Name 2813  "param"
                              Name 2815  "param"
                              Name 2817  "param"
                              Name 2831  "rv2"
                              Name 2836  "param"
                              Name 2842  "fogColor"
                              Name 2843  "param"
                              Name 2845  "param"
                              Name 2861  "param"
                              Name 2863  "param"
                              Name 2871  "param"
                              Name 2886  "param"
                              Name 2895  "ray"
                              Name 2920  "seed"
                              Name 2921  "finalColor"
                              Decorate 139(gl_FragCoord) BuiltIn FragCoord
                              Decorate 145 ArrayStride 16
                              Decorate 146 ArrayStride 16
                              Decorate 147 ArrayStride 16
                              MemberDecorate 148(ShadertoyUB) 0 Offset 0
                              MemberDecorate 148(ShadertoyUB) 1 Offset 16
                              MemberDecorate 148(ShadertoyUB) 2 Offset 20
                              MemberDecorate 148(ShadertoyUB) 3 Offset 24
                              MemberDecorate 148(ShadertoyUB) 4 Offset 28
                              MemberDecorate 148(ShadertoyUB) 5 Offset 32
                              MemberDecorate 148(ShadertoyUB) 6 Offset 96
                              MemberDecorate 148(ShadertoyUB) 7 Offset 160
                              MemberDecorate 148(ShadertoyUB) 8 Offset 176
                              MemberDecorate 148(ShadertoyUB) 9 Offset 192
                              MemberDecorate 148(ShadertoyUB) 10 Offset 196
                              MemberDecorate 148(ShadertoyUB) 11 Offset 200
                              MemberDecorate 148(ShadertoyUB) 12 Offset 204
                              MemberDecorate 148(ShadertoyUB) 13 Offset 208
                              MemberDecorate 148(ShadertoyUB) 14 Offset 272
                              Decorate 148(ShadertoyUB) Block
                              Decorate 150 DescriptorSet 0
                              Decorate 150 Binding 0
                              Decorate 161(out_Color0) Location 0
               3:             TypeVoid
               4:             TypeFunction 3
               7:             TypeFloat 32
               8:             TypeVector 7(float) 3
               9:             TypeVector 7(float) 2
         10(Ray):             TypeStruct 8(fvec3) 8(fvec3) 8(fvec3)
              11:             TypeFunction 10(Ray) 8(fvec3) 8(fvec3) 8(fvec3) 8(fvec3) 8(fvec3) 7(float) 9(fvec2)
              21:             TypePointer Function 7(float)
              22:             TypeFunction 7(float) 21(ptr)
              26:             TypePointer Function 9(fvec2)
              27:             TypeFunction 7(float) 26(ptr)
              31:             TypeFunction 9(fvec2) 26(ptr)
              38:             TypePointer Function 8(fvec3)
              39:             TypeFunction 7(float) 38(ptr)
              43:             TypeFunction 7(float) 21(ptr) 21(ptr) 21(ptr)
              55:             TypeFunction 8(fvec3) 38(ptr)
              62:             TypeFunction 8(fvec3) 38(ptr) 38(ptr)
              71:             TypeFunction 8(fvec3) 38(ptr) 38(ptr) 21(ptr)
              77:             TypeFunction 9(fvec2) 26(ptr) 26(ptr)
              82:             TypeFunction 7(float) 38(ptr) 38(ptr)
              87:             TypeFunction 7(float) 38(ptr) 21(ptr) 21(ptr)
              98:             TypeFunction 7(float) 21(ptr) 21(ptr)
             103:             TypeFunction 9(fvec2) 38(ptr) 21(ptr)
             108:             TypeFunction 9(fvec2) 38(ptr) 26(ptr)
             113:             TypeFunction 9(fvec2) 38(ptr)
             117:             TypeFunction 3 26(ptr) 38(ptr) 38(ptr) 21(ptr) 38(ptr)
             125:             TypeFunction 8(fvec3) 9(fvec2) 10(Ray)
             130:             TypeVector 7(float) 4
             131:             TypePointer Function 130(fvec4)
             132:             TypeFunction 3 131(ptr) 26(ptr)
             138:             TypePointer Input 130(fvec4)
139(gl_FragCoord):    138(ptr) Variable Input
             142:             TypeInt 32 1
             143:             TypeInt 32 0
             144:    143(int) Constant 4
             145:             TypeArray 130(fvec4) 144
             146:             TypeArray 130(fvec4) 144
             147:             TypeArray 130(fvec4) 144
148(ShadertoyUB):             TypeStruct 130(fvec4) 7(float) 7(float) 142(int) 7(float) 145 146 130(fvec4) 130(fvec4) 7(float) 7(float) 7(float) 7(float) 147 130(fvec4)
             149:             TypePointer Uniform 148(ShadertoyUB)
             150:    149(ptr) Variable Uniform
             151:    142(int) Constant 0
             152:    143(int) Constant 1
             153:             TypePointer Uniform 7(float)
             160:             TypePointer Output 130(fvec4)
 161(out_Color0):    160(ptr) Variable Output
             169:    143(int) Constant 0
             182:             TypePointer Function 10(Ray)
             185:    142(int) Constant 2
             189:    142(int) Constant 1
             202:    7(float) Constant 1176670106
             212:    7(float) Constant 1108606976
             217:    7(float) Constant 1204494835
             231:    7(float) Constant 1176260177
             232:    7(float) Constant 1192278451
             233:    9(fvec2) ConstantComposite 231 232
             258:    143(int) Constant 2
             261:    7(float) Constant 1140998144
             266:    7(float) Constant 1205262835
             276:    7(float) Constant 1077936128
             277:    7(float) Constant 1073741824
             297:    7(float) Constant 1065353216
             298:    7(float) Constant 0
             299:    9(fvec2) ConstantComposite 297 298
             305:    9(fvec2) ConstantComposite 298 297
             311:    9(fvec2) ConstantComposite 297 297
             351:    8(fvec3) ConstantComposite 297 298 298
             357:    8(fvec3) ConstantComposite 298 297 298
             363:    8(fvec3) ConstantComposite 297 297 298
             369:    8(fvec3) ConstantComposite 298 298 297
             375:    8(fvec3) ConstantComposite 297 298 297
             381:    8(fvec3) ConstantComposite 298 297 297
             387:    8(fvec3) ConstantComposite 297 297 297
             460:    7(float) Constant 1056964608
             465:    7(float) Constant 1028443341
             470:    7(float) Constant 1125515264
             473:    7(float) Constant 1171963904
             478:    7(float) Constant 1193033728
             480:             TypePointer Private 8(fvec3)
     481(sunCol):    480(ptr) Variable Private
             485:    7(float) Constant 1020054733
 490(horizonCol):    480(ptr) Variable Private
     492(skyCol):    480(ptr) Variable Private
             498:    7(float) Constant 1055957975
             502:    7(float) Constant 1064514355
             532:    7(float) Constant 1082130432
             536:    7(float) Constant 1048576000
             546:    7(float) Constant 1090519040
             550:    7(float) Constant 1040187392
             558:    7(float) Constant 1045220557
             570:    7(float) Constant 1092616192
             573:    7(float) Constant 1061158912
             578:    7(float) Constant 1084227584
             613:    7(float) Constant 1084952360
             617:    7(float) Constant 1067316150
             622:    7(float) Constant 1074484216
             627:    7(float) Constant 1058642330
             645:    7(float) Constant 1036831949
             655:    7(float) Constant 1075838976
             659:    7(float) Constant 1103101952
             665:    7(float) Constant 1107296256
             674:    7(float) Constant 1132462080
             690:    7(float) Constant 1032805417
             691:    8(fvec3) ConstantComposite 645 690 465)#"_str
+
R"#(
             704:             TypeBool
             775:    7(float) Constant 3154317935
             776:    7(float) Constant 981668463
             777:    8(fvec3) ConstantComposite 298 775 776
             779:    7(float) Constant 1008981770
             780:    7(float) Constant 991130812
             781:    7(float) Constant 1021396910
             782:    8(fvec3) ConstantComposite 779 780 781
             787:    7(float) Constant 3162706543
             788:    7(float) Constant 1006834287
             789:    8(fvec3) ConstantComposite 298 787 788
             791:    7(float) Constant 1000593162
             792:    7(float) Constant 973279855
             793:    8(fvec3) ConstantComposite 791 792 779
             797:    7(float) Constant 3273654272
             805:    7(float) Constant 3148076810
             806:    8(fvec3) ConstantComposite 298 805 298
             811:    7(float) Constant 1017370378
             815:    7(float) Constant 998445679
             816:    7(float) Constant 1012739867
             827:    7(float) Constant 1203982336
             861:    7(float) Constant 1101004800
             868:    8(fvec3) ConstantComposite 460 298 460
             871:    143(int) Constant 3
             887:    7(float) Constant 1069547520
             889:    7(float) Constant 1041865114
             941:    7(float) Constant 1053609165
             976:    7(float) Constant 1061997773
            1002:    7(float) Constant 1011666125
            1004:    7(float) Constant 1014350479
            1050:    7(float) Constant 1050253722
            1126:    7(float) Constant 1025758986
            1143:    7(float) Constant 1051931443
            1144:    8(fvec3) ConstantComposite 1143 791 1143
            1176:             TypePointer Private 7(float)
 1177(localTime):   1176(ptr) Variable Private
            1196:    7(float) Constant 3212836864
            1197:    9(fvec2) ConstantComposite 1196 297
            1232:    7(float) Constant 1035489772
            1258:    7(float) Constant 1033476506
            1267:    7(float) Constant 1065135112
            1271:    7(float) Constant 1043878380
            1308:    7(float) Constant 1090288353
            1314:    7(float) Constant 1108840284
            1345:    7(float) Constant 1082803618
            1351:    7(float) Constant 1067869798
            1361:    7(float) Constant 1109393408
            1388:    7(float) Constant 1066385015
            1402:    7(float) Constant 1078530011
            1405:    7(float) Constant 1057048494
            1422:    7(float) Constant 1117782016
1451(marchCount):   1176(ptr) Variable Private
            1452:    7(float) Constant 1033150872
            1453:    7(float) Constant 1032773326
            1454:    7(float) Constant 1030123422
            1455:    8(fvec3) ConstantComposite 1452 1453 1454
    1456(sunDir):    480(ptr) Variable Private
            1457:    7(float) Constant 1057794242
            1458:    7(float) Constant 1058488088
            1459:    8(fvec3) ConstantComposite 1457 1458 1458
            1460:    7(float) Constant 1063675494
            1461:    7(float) Constant 1062920520
            1462:    7(float) Constant 1061410570
            1463:    8(fvec3) ConstantComposite 1460 1461 1462
            1464:    8(fvec3) ConstantComposite 1050 460 502
  1465(exposure):   1176(ptr) Variable Private
      1466(fade):   1176(ptr) Variable Private
            1468:    8(fvec3) ConstantComposite 298 298 298
            1472:             TypePointer Uniform 130(fvec4)
            1484:    7(float) Constant 1114112000
            1507:    7(float) Constant 1095761920
            1508:    7(float) Constant 1079194419
            1509:    7(float) Constant 3227516928
            1510:    8(fvec3) ConstantComposite 1507 1508 1509
            1513:    7(float) Constant 1083808154
            1519:    8(fvec3) ConstantComposite 298 887 887
            1522:    7(float) Constant 1096810496
            1531:    7(float) Constant 1086324736
            1542:    7(float) Constant 1104150528
            1549:    7(float) Constant 1077097267
            1558:    7(float) Constant 3238159974
            1559:    7(float) Constant 3256877056
            1561:    7(float) Constant 1059172177
            1562:    7(float) Constant 1053572037
            1563:    7(float) Constant 1059729871
            1564:    8(fvec3) ConstantComposite 1561 1562 1563
            1565:    7(float) Constant 1027103051
            1566:    8(fvec3) ConstantComposite 1452 1453 1565
            1567:    7(float) Constant 1060320051
            1592:    7(float) Constant 1094713344
            1593:    7(float) Constant 1086953882
            1594:    7(float) Constant 3204448256
            1595:    8(fvec3) ConstantComposite 1592 1593 1594
            1597:    7(float) Constant 1085276160
            1606:    7(float) Constant 1084647014
            1619:    8(fvec3) ConstantComposite 298 297 1594
            1622:    7(float) Constant 1108869120
            1641:    7(float) Constant 1074370970
            1653:    7(float) Constant 1079613850
            1666:    7(float) Constant 1113587712
            1675:    7(float) Constant 1099956224
            1686:    7(float) Constant 3221225472
            1688:    7(float) Constant 1067030938
            1691:    7(float) Constant 3240624128
            1698:    7(float) Constant 2147483648
            1708:    7(float) Constant 1026196939
            1709:    7(float) Constant 1015694074
            1710:    8(fvec3) ConstantComposite 1452 1708 1709
            1711:    7(float) Constant 1098907648
            1716:    8(fvec3) ConstantComposite 277 297 1567
            1717:    8(fvec3) ConstantComposite 573 460 502
            1723:    7(float) Constant 1120403456
            1724:    8(fvec3) ConstantComposite 1542 1723 277
            1725:    8(fvec3) ConstantComposite 1050 889 298
            1765:    9(fvec2) ConstantComposite 298 298
            1768:             TypePointer Function 142(int)
            1776:    142(int) Constant 250
  1831(voxelPad):   1176(ptr) Variable Private
            1844:    7(float) Constant 1110704128
            1852:    7(float) Constant 975427338
            1903:    8(fvec3) ConstantComposite 1852 298 298
            2026:    7(float) Constant 990057071
            2036:    142(int) Constant 40
            2113:    7(float) Constant 1083179008
            2131:    7(float) Constant 1115684864
            2138:    7(float) Constant 1124073472
            2151:    7(float) Constant 1140850688
            2164:    8(fvec3) ConstantComposite 502 297 297
            2193:    7(float) Constant 1089794055
            2221:    7(float) Constant 1059749626
            2288:    7(float) Constant 1051361018
            2338:    7(float) Constant 1041395352
            2340:    7(float) Constant 1139802112
            2348:    7(float) Constant 1092479877
            2388:    7(float) Constant 1146880000
            2395:    8(fvec3) ConstantComposite 297 976 1050
            2403:    7(float) Constant 1031127695
            2412:    7(float) Constant 1030053954
            2436:    7(float) Constant 1133248512
            2454:    8(fvec3) ConstantComposite 460 460 460
            2461:    7(float) Constant 1038174126
            2470:    7(float) Constant 1037637255
            2512:    7(float) Constant 1045891645
            2528:    8(fvec3) ConstantComposite 536 536 536
            2569:    9(fvec2) ConstantComposite 1723 1723
            2575:    7(float) Constant 1112014848
            2576:    9(fvec2) ConstantComposite 570 2575
            2583:    7(float) Constant 1145569280
            2626:    8(fvec3) ConstantComposite 465 465 465
            2650:    7(float) Constant 1091321201
            2656:    7(float) Constant 1099815191
            2664:    8(fvec3) ConstantComposite 558 558 558
            2676:    7(float) Constant 1061813223
            2685:    7(float) Constant 1016028201
            2688:    7(float) Constant 1164804096
            2719:    8(fvec3) ConstantComposite 1723 1723 1723
            2772:    7(float) Constant 1132134072
            2848:    7(float) Constant 1091567616
            2849:    8(fvec3) ConstantComposite 2848 2848 2848
            2896:    142(int) Constant 13
            2906:    142(int) Constant 3
            2910:    142(int) Constant 14
      2920(seed):   1176(ptr) Variable Private
         5(main):           3 Function None 4
               6:             Label
      137(coord):     26(ptr) Variable Function
      162(param):    131(ptr) Variable Function
      163(param):     26(ptr) Variable Function
                              Line 1 160 0
             140:  130(fvec4) Load 139(gl_FragCoord)
             141:    9(fvec2) VectorShuffle 140 140 0 1
                              Store 137(coord) 141
                              Line 1 161 0
             154:    153(ptr) AccessChain 150 151 152
             155:    7(float) Load 154
             156:     21(ptr) AccessChain 137(coord) 152
             157:    7(float) Load 156
             158:    7(float) FSub 155 157
             159:     21(ptr) AccessChain 137(coord) 152
                              Store 159 158
                              Line 1 162 0
             164:    9(fvec2) Load 137(coord)
                              Store 163(param) 164
             165:           3 FunctionCall 135(mainImage(vf4;vf2;) 162(param) 163(param)
             166:  130(fvec4) Load 162(param)
                              Store 161(out_Color0) 166
                              Return
                              FunctionEnd
19(Ray_From(vf3;vf3;vf3;vf3;vf3;f1;vf2;):     10(Ray) Function None 11
  12(leftBottom):    8(fvec3) FunctionParameter
 13(rightBottom):    8(fvec3) FunctionParameter
     14(leftTop):    8(fvec3) FunctionParameter
    15(rightTop):    8(fvec3) FunctionParameter
      16(origin):    8(fvec3) FunctionParameter
   17(nearPlane):    7(float) FunctionParameter
  18(unormCoord):    9(fvec2) FunctionParameter
              20:             Label
      167(coord):     26(ptr) Variable Function
        168(vec):     38(ptr) Variable Function
        183(ray):    182(ptr) Variable Function
                              Line 1 168 0
                              Store 167(coord) 18(unormCoord)
                              Line 1 169 0
             170:     21(ptr) AccessChain 167(coord) 169
             171:    7(float) Load 170
             172:    8(fvec3) CompositeConstruct 171 171 171
             173:    8(fvec3) ExtInst 2(GLSL.std.450) 46(FMix) 12(leftBottom) 13(rightBottom) 172
             174:     21(ptr) AccessChain 167(coord) 169
             175:    7(float) Load 174
             176:    8(fvec3) CompositeConstruct 175 175 175
             177:    8(fvec3) ExtInst 2(GLSL.std.450) 46(FMix) 14(leftTop) 15(rightTop) 176
             178:     21(ptr) AccessChain 167(coord) 152
             179:    7(float) Load 178
             180:    8(fvec3) CompositeConstruct 179 179 179
             181:    8(fvec3) ExtInst 2(GLSL.std.450) 46(FMix) 173 177 180
                              Store 168(vec) 181
                              Line 1 171 0
             184:     38(ptr) AccessChain 183(ray) 151
                              Store 184 16(origin)
                              Line 1 172 0
             186:    8(fvec3) Load 168(vec)
             187:    8(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 186
             188:     38(ptr) AccessChain 183(ray) 185
                              Store 188 187
                              Line 1 173 0
             190:     38(ptr) AccessChain 183(ray) 151
             191:    8(fvec3) Load 190
             192:     38(ptr) AccessChain 183(ray) 185
             193:    8(fvec3) Load 192
             194:    8(fvec3) VectorTimesScalar 193 17(nearPlane)
             195:    8(fvec3) FAdd 191 194
             196:     38(ptr) AccessChain 183(ray) 189
                              Store 196 195
                              Line 1 174 0
             197:     10(Ray) Load 183(ray)
                              ReturnValue 197
                              FunctionEnd
  24(Hash11(f1;):    7(float) Function None 22
           23(a):     21(ptr) FunctionParameter
              25:             Label
                              Line 1 180 0
             200:    7(float) Load 23(a)
             201:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 200
             203:    7(float) FMul 201 202
             204:    7(float) ExtInst 2(GLSL.std.450) 10(Fract) 203
                              ReturnValue 204
                              FunctionEnd
 29(Hash21(vf2;):    7(float) Function None 27
          28(uv):     26(ptr) FunctionParameter
              30:             Label
          207(f):     21(ptr) Variable Function
                              Line 1 187 0
             208:     21(ptr) AccessChain 28(uv) 169
             209:    7(float) Load 208
             210:     21(ptr) AccessChain 28(uv) 152
             211:    7(float) Load 210
             213:    7(float) FMul 211 212
             214:    7(float) FAdd 209 213
                              Store 207(f) 214
                              Line 1 188 0
             215:    7(float) Load 207(f)
             216:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 215
             218:    7(float) FMul 216 217
             219:    7(float) ExtInst 2(GLSL.std.450) 10(Fract) 218
                              ReturnValue 219
                              FunctionEnd
 33(Hash22(vf2;):    9(fvec2) Function None 31
          32(uv):     26(ptr) FunctionParameter
              34:             Label
          222(f):     21(ptr) Variable Function
                              Line 1 195 0
             223:     21(ptr) AccessChain 32(uv) 169
             224:    7(float) Load 223
             225:     21(ptr) AccessChain 32(uv) 152
             226:    7(float) Load 225
             227:    7(float) FMul 226 212
             228:    7(float) FAdd 224 227
                              Store 222(f) 228
                              Line 1 196 0
             229:    7(float) Load 222(f)
             230:    7(float) ExtInst 2(GLSL.std.450) 14(Cos) 229
             234:    9(fvec2) VectorTimesScalar 233 230
             235:    9(fvec2) ExtInst 2(GLSL.std.450) 10(Fract) 234
                              ReturnValue 235
                              FunctionEnd
 36(Hash2d(vf2;):    7(float) Function None 27
          35(uv):     26(ptr) FunctionParameter
              37:             Label
          238(f):     21(ptr) Variable Function
                              Line 1 203 0
             239:     21(ptr) AccessChain 35(uv) 169
             240:    7(float) Load 239
             241:     21(ptr) AccessChain 35(uv) 152
             242:    7(float) Load 241
             243:    7(float) FMul 242 212
             244:    7(float) FAdd 240 243
                              Store 238(f) 244
                              Line 1 204 0
             245:    7(float) Load 238(f)
             246:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 245
             247:    7(float) FMul 246 217
             248:    7(float) ExtInst 2(GLSL.std.450) 10(Fract) 247
                              ReturnValue 248
                              FunctionEnd
 41(Hash3d(vf3;):    7(float) Function None 39
          40(uv):     38(ptr) FunctionParameter
              42:             Label
          251(f):     21(ptr) Variable Function
                              Line 1 211 0
             252:     21(ptr) AccessChain 40(uv) 169
             253:    7(float) Load 252
             254:     21(ptr) AccessChain 40(uv) 152
             255:    7(float) Load 254
             256:    7(float) FMul 255 212
             257:    7(float) FAdd 253 256
             259:     21(ptr) AccessChain 40(uv) 258
             260:    7(float) Load 259)#"_str
+
R"#(
             262:    7(float) FMul 260 261
             263:    7(float) FAdd 257 262
                              Store 251(f) 263
                              Line 1 212 0
             264:    7(float) Load 251(f)
             265:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 264
             267:    7(float) FMul 265 266
             268:    7(float) ExtInst 2(GLSL.std.450) 10(Fract) 267
                              ReturnValue 268
                              FunctionEnd
47(mixP(f1;f1;f1;):    7(float) Function None 43
          44(f0):     21(ptr) FunctionParameter
          45(f1):     21(ptr) FunctionParameter
           46(a):     21(ptr) FunctionParameter
              48:             Label
                              Line 1 218 0
             271:    7(float) Load 44(f0)
             272:    7(float) Load 45(f1)
             273:    7(float) Load 46(a)
             274:    7(float) Load 46(a)
             275:    7(float) FMul 273 274
             278:    7(float) Load 46(a)
             279:    7(float) FMul 277 278
             280:    7(float) FSub 276 279
             281:    7(float) FMul 275 280
             282:    7(float) ExtInst 2(GLSL.std.450) 46(FMix) 271 272 281
                              ReturnValue 282
                              FunctionEnd
50(noise2d(vf2;):    7(float) Function None 27
          49(uv):     26(ptr) FunctionParameter
              51:             Label
         285(fr):     26(ptr) Variable Function
         288(fl):     26(ptr) Variable Function
        291(h00):     21(ptr) Variable Function
      292(param):     26(ptr) Variable Function
        295(h10):     21(ptr) Variable Function
      301(param):     26(ptr) Variable Function
        303(h01):     21(ptr) Variable Function
      307(param):     26(ptr) Variable Function
        309(h11):     21(ptr) Variable Function
      313(param):     26(ptr) Variable Function
      315(param):     21(ptr) Variable Function
      317(param):     21(ptr) Variable Function
      319(param):     21(ptr) Variable Function
      323(param):     21(ptr) Variable Function
      325(param):     21(ptr) Variable Function
      327(param):     21(ptr) Variable Function
      331(param):     21(ptr) Variable Function
      332(param):     21(ptr) Variable Function
      333(param):     21(ptr) Variable Function
                              Line 1 225 0
             286:    9(fvec2) Load 49(uv)
             287:    9(fvec2) ExtInst 2(GLSL.std.450) 10(Fract) 286
                              Store 285(fr) 287
                              Line 1 227 0
             289:    9(fvec2) Load 49(uv)
             290:    9(fvec2) ExtInst 2(GLSL.std.450) 8(Floor) 289
                              Store 288(fl) 290
                              Line 1 229 0
             293:    9(fvec2) Load 288(fl)
                              Store 292(param) 293
             294:    7(float) FunctionCall 36(Hash2d(vf2;) 292(param)
                              Store 291(h00) 294
                              Line 1 231 0
             296:    9(fvec2) Load 288(fl)
             300:    9(fvec2) FAdd 296 299
                              Store 301(param) 300
             302:    7(float) FunctionCall 36(Hash2d(vf2;) 301(param)
                              Store 295(h10) 302
                              Line 1 233 0
             304:    9(fvec2) Load 288(fl)
             306:    9(fvec2) FAdd 304 305
                              Store 307(param) 306
             308:    7(float) FunctionCall 36(Hash2d(vf2;) 307(param)
                              Store 303(h01) 308
                              Line 1 235 0
             310:    9(fvec2) Load 288(fl)
             312:    9(fvec2) FAdd 310 311
                              Store 313(param) 312
             314:    7(float) FunctionCall 36(Hash2d(vf2;) 313(param)
                              Store 309(h11) 314
                              Line 1 236 0
             316:    7(float) Load 291(h00)
                              Store 315(param) 316
             318:    7(float) Load 295(h10)
                              Store 317(param) 318
             320:     21(ptr) AccessChain 285(fr) 169
             321:    7(float) Load 320
                              Store 319(param) 321
             322:    7(float) FunctionCall 47(mixP(f1;f1;f1;) 315(param) 317(param) 319(param)
             324:    7(float) Load 303(h01)
                              Store 323(param) 324
             326:    7(float) Load 309(h11)
                              Store 325(param) 326
             328:     21(ptr) AccessChain 285(fr) 169
             329:    7(float) Load 328
                              Store 327(param) 329
             330:    7(float) FunctionCall 47(mixP(f1;f1;f1;) 323(param) 325(param) 327(param)
                              Store 331(param) 322
                              Store 332(param) 330
             334:     21(ptr) AccessChain 285(fr) 152
             335:    7(float) Load 334
                              Store 333(param) 335
             336:    7(float) FunctionCall 47(mixP(f1;f1;f1;) 331(param) 332(param) 333(param)
                              ReturnValue 336
                              FunctionEnd
  53(noise(vf3;):    7(float) Function None 39
          52(uv):     38(ptr) FunctionParameter
              54:             Label
         339(fr):     38(ptr) Variable Function
         342(fl):     38(ptr) Variable Function
       345(h000):     21(ptr) Variable Function
      346(param):     38(ptr) Variable Function
       349(h100):     21(ptr) Variable Function
      353(param):     38(ptr) Variable Function
       355(h010):     21(ptr) Variable Function
      359(param):     38(ptr) Variable Function
       361(h110):     21(ptr) Variable Function
      365(param):     38(ptr) Variable Function
       367(h001):     21(ptr) Variable Function
      371(param):     38(ptr) Variable Function
       373(h101):     21(ptr) Variable Function
      377(param):     38(ptr) Variable Function
       379(h011):     21(ptr) Variable Function
      383(param):     38(ptr) Variable Function
       385(h111):     21(ptr) Variable Function
      389(param):     38(ptr) Variable Function
      391(param):     21(ptr) Variable Function
      393(param):     21(ptr) Variable Function
      395(param):     21(ptr) Variable Function
      399(param):     21(ptr) Variable Function
      401(param):     21(ptr) Variable Function
      403(param):     21(ptr) Variable Function
      407(param):     21(ptr) Variable Function
      408(param):     21(ptr) Variable Function
      409(param):     21(ptr) Variable Function
      413(param):     21(ptr) Variable Function
      415(param):     21(ptr) Variable Function
      417(param):     21(ptr) Variable Function
      421(param):     21(ptr) Variable Function
      423(param):     21(ptr) Variable Function
      425(param):     21(ptr) Variable Function
      429(param):     21(ptr) Variable Function
      430(param):     21(ptr) Variable Function
      431(param):     21(ptr) Variable Function
      435(param):     21(ptr) Variable Function
      436(param):     21(ptr) Variable Function
      437(param):     21(ptr) Variable Function
                              Line 1 243 0
             340:    8(fvec3) Load 52(uv)
             341:    8(fvec3) ExtInst 2(GLSL.std.450) 10(Fract) 340
                              Store 339(fr) 341
                              Line 1 245 0
             343:    8(fvec3) Load 52(uv)
             344:    8(fvec3) ExtInst 2(GLSL.std.450) 8(Floor) 343
                              Store 342(fl) 344
                              Line 1 247 0
             347:    8(fvec3) Load 342(fl)
                              Store 346(param) 347
             348:    7(float) FunctionCall 41(Hash3d(vf3;) 346(param)
                              Store 345(h000) 348
                              Line 1 249 0
             350:    8(fvec3) Load 342(fl)
             352:    8(fvec3) FAdd 350 351
                              Store 353(param) 352
             354:    7(float) FunctionCall 41(Hash3d(vf3;) 353(param)
                              Store 349(h100) 354
                              Line 1 251 0
             356:    8(fvec3) Load 342(fl)
             358:    8(fvec3) FAdd 356 357
                              Store 359(param) 358
             360:    7(float) FunctionCall 41(Hash3d(vf3;) 359(param)
                              Store 355(h010) 360
                              Line 1 253 0
             362:    8(fvec3) Load 342(fl)
             364:    8(fvec3) FAdd 362 363
                              Store 365(param) 364
             366:    7(float) FunctionCall 41(Hash3d(vf3;) 365(param)
                              Store 361(h110) 366
                              Line 1 255 0
             368:    8(fvec3) Load 342(fl)
             370:    8(fvec3) FAdd 368 369
                              Store 371(param) 370
             372:    7(float) FunctionCall 41(Hash3d(vf3;) 371(param)
                              Store 367(h001) 372
                              Line 1 257 0
             374:    8(fvec3) Load 342(fl)
             376:    8(fvec3) FAdd 374 375
                              Store 377(param) 376
             378:    7(float) FunctionCall 41(Hash3d(vf3;) 377(param)
                              Store 373(h101) 378
                              Line 1 259 0
             380:    8(fvec3) Load 342(fl)
             382:    8(fvec3) FAdd 380 381
                              Store 383(param) 382
             384:    7(float) FunctionCall 41(Hash3d(vf3;) 383(param)
                              Store 379(h011) 384
                              Line 1 261 0
             386:    8(fvec3) Load 342(fl)
             388:    8(fvec3) FAdd 386 387
                              Store 389(param) 388
             390:    7(float) FunctionCall 41(Hash3d(vf3;) 389(param)
                              Store 385(h111) 390
                              Line 1 262 0
             392:    7(float) Load 345(h000)
                              Store 391(param) 392
             394:    7(float) Load 349(h100)
                              Store 393(param) 394
             396:     21(ptr) AccessChain 339(fr) 169
             397:    7(float) Load 396
                              Store 395(param) 397
             398:    7(float) FunctionCall 47(mixP(f1;f1;f1;) 391(param) 393(param) 395(param)
             400:    7(float) Load 355(h010)
                              Store 399(param) 400
             402:    7(float) Load 361(h110)
                              Store 401(param) 402
             404:     21(ptr) AccessChain 339(fr) 169
             405:    7(float) Load 404
                              Store 403(param) 405
             406:    7(float) FunctionCall 47(mixP(f1;f1;f1;) 399(param) 401(param) 403(param)
                              Store 407(param) 398
                              Store 408(param) 406
             410:     21(ptr) AccessChain 339(fr) 152
             411:    7(float) Load 410
                              Store 409(param) 411
             412:    7(float) FunctionCall 47(mixP(f1;f1;f1;) 407(param) 408(param) 409(param)
             414:    7(float) Load 367(h001)
                              Store 413(param) 414
             416:    7(float) Load 373(h101)
                              Store 415(param) 416
             418:     21(ptr) AccessChain 339(fr) 169
             419:    7(float) Load 418
                              Store 417(param) 419
             420:    7(float) FunctionCall 47(mixP(f1;f1;f1;) 413(param) 415(param) 417(param)
             422:    7(float) Load 379(h011)
                              Store 421(param) 422
             424:    7(float) Load 385(h111)
                              Store 423(param) 424
             426:     21(ptr) AccessChain 339(fr) 169
             427:    7(float) Load 426
                              Store 425(param) 427
             428:    7(float) FunctionCall 47(mixP(f1;f1;f1;) 421(param) 423(param) 425(param)
                              Store 429(param) 420
                              Store 430(param) 428
             432:     21(ptr) AccessChain 339(fr) 152
             433:    7(float) Load 432
                              Store 431(param) 433
             434:    7(float) FunctionCall 47(mixP(f1;f1;f1;) 429(param) 430(param) 431(param)
                              Store 435(param) 412
                              Store 436(param) 434
             438:     21(ptr) AccessChain 339(fr) 258
             439:    7(float) Load 438
                              Store 437(param) 439
             440:    7(float) FunctionCall 47(mixP(f1;f1;f1;) 435(param) 436(param) 437(param)
                              ReturnValue 440
                              FunctionEnd
57(saturate(vf3;):    8(fvec3) Function None 55
           56(a):     38(ptr) FunctionParameter
              58:             Label
                              Line 1 268 0
             443:    8(fvec3) Load 56(a)
             444:    8(fvec3) CompositeConstruct 298 298 298
             445:    8(fvec3) CompositeConstruct 297 297 297
             446:    8(fvec3) ExtInst 2(GLSL.std.450) 43(FClamp) 443 444 445
                              ReturnValue 446
                              FunctionEnd
60(saturate(f1;):    7(float) Function None 22
           59(a):     21(ptr) FunctionParameter
              61:             Label
                              Line 1 274 0
             449:    7(float) Load 59(a)
             450:    7(float) ExtInst 2(GLSL.std.450) 43(FClamp) 449 298 297
                              ReturnValue 450
                              FunctionEnd
65(GetSunColorSmall(vf3;vf3;):    8(fvec3) Function None 62
      63(rayDir):     38(ptr) FunctionParameter
      64(sunDir):     38(ptr) FunctionParameter
              66:             Label
   453(localRay):     38(ptr) Variable Function
       456(dist):     21(ptr) Variable Function
464(sunIntensity):     21(ptr) Variable Function
                              Line 1 281 0
             454:    8(fvec3) Load 63(rayDir)
             455:    8(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 454
                              Store 453(localRay) 455
                              Line 1 283 0
             457:    8(fvec3) Load 453(localRay)
             458:    8(fvec3) Load 64(sunDir)
             459:    7(float) Dot 457 458
             461:    7(float) FMul 459 460
             462:    7(float) FAdd 461 460
             463:    7(float) FSub 297 462
                              Store 456(dist) 463
                              Line 1 285 0
             466:    7(float) Load 456(dist)
             467:    7(float) FDiv 465 466
                              Store 464(sunIntensity) 467
                              Line 1 286 0
             468:    7(float) Load 456(dist)
             469:    7(float) FNegate 468
             471:    7(float) FMul 469 470
             472:    7(float) ExtInst 2(GLSL.std.450) 27(Exp) 471
             474:    7(float) FMul 472 473
             475:    7(float) Load 464(sunIntensity)
             476:    7(float) FAdd 475 474
                              Store 464(sunIntensity) 476
                              Line 1 287 0
             477:    7(float) Load 464(sunIntensity)
             479:    7(float) ExtInst 2(GLSL.std.450) 37(FMin) 477 478
                              Store 464(sunIntensity) 479
                              Line 1 288 0
             482:    8(fvec3) Load 481(sunCol)
             483:    7(float) Load 464(sunIntensity)
             484:    8(fvec3) VectorTimesScalar 482 483
             486:    8(fvec3) VectorTimesScalar 484 485
                              ReturnValue 486
                              FunctionEnd
69(GetEnvMap(vf3;vf3;):    8(fvec3) Function None 62
      67(rayDir):     38(ptr) FunctionParameter
      68(sunDir):     38(ptr) FunctionParameter)#"_str
+
R"#(
              70:             Label
 489(finalColor):     38(ptr) Variable Function
      494(param):     21(ptr) Variable Function
          504(n):     21(ptr) Variable Function
      512(param):     26(ptr) Variable Function
      521(param):     26(ptr) Variable Function
      534(param):     26(ptr) Variable Function
      548(param):     26(ptr) Variable Function
      565(param):     21(ptr) Variable Function
      580(param):     21(ptr) Variable Function
      585(param):     21(ptr) Variable Function
      589(param):     38(ptr) Variable Function
      591(param):     38(ptr) Variable Function
                              Line 1 295 0
             491:    8(fvec3) Load 490(horizonCol)
             493:    8(fvec3) Load 492(skyCol)
             495:     21(ptr) AccessChain 67(rayDir) 152
             496:    7(float) Load 495
                              Store 494(param) 496
             497:    7(float) FunctionCall 60(saturate(f1;) 494(param)
             499:    7(float) ExtInst 2(GLSL.std.450) 26(Pow) 497 498
             500:    8(fvec3) CompositeConstruct 499 499 499
             501:    8(fvec3) ExtInst 2(GLSL.std.450) 46(FMix) 491 493 500
             503:    8(fvec3) VectorTimesScalar 501 502
                              Store 489(finalColor) 503
                              Line 1 297 0
             505:    8(fvec3) Load 67(rayDir)
             506:    9(fvec2) VectorShuffle 505 505 0 2
             507:     21(ptr) AccessChain 67(rayDir) 152
             508:    7(float) Load 507
             509:    9(fvec2) CompositeConstruct 508 508
             510:    9(fvec2) FDiv 506 509
             511:    9(fvec2) VectorTimesScalar 510 297
                              Store 512(param) 511
             513:    7(float) FunctionCall 50(noise2d(vf2;) 512(param)
                              Store 504(n) 513
                              Line 1 298 0
             514:    8(fvec3) Load 67(rayDir)
             515:    9(fvec2) VectorShuffle 514 514 0 2
             516:     21(ptr) AccessChain 67(rayDir) 152
             517:    7(float) Load 516
             518:    9(fvec2) CompositeConstruct 517 517
             519:    9(fvec2) FDiv 515 518
             520:    9(fvec2) VectorTimesScalar 519 277
                              Store 521(param) 520
             522:    7(float) FunctionCall 50(noise2d(vf2;) 521(param)
             523:    7(float) FMul 522 460
             524:    7(float) Load 504(n)
             525:    7(float) FAdd 524 523
                              Store 504(n) 525
                              Line 1 299 0
             526:    8(fvec3) Load 67(rayDir)
             527:    9(fvec2) VectorShuffle 526 526 0 2
             528:     21(ptr) AccessChain 67(rayDir) 152
             529:    7(float) Load 528
             530:    9(fvec2) CompositeConstruct 529 529
             531:    9(fvec2) FDiv 527 530
             533:    9(fvec2) VectorTimesScalar 531 532
                              Store 534(param) 533
             535:    7(float) FunctionCall 50(noise2d(vf2;) 534(param)
             537:    7(float) FMul 535 536
             538:    7(float) Load 504(n)
             539:    7(float) FAdd 538 537
                              Store 504(n) 539
                              Line 1 300 0
             540:    8(fvec3) Load 67(rayDir)
             541:    9(fvec2) VectorShuffle 540 540 0 2
             542:     21(ptr) AccessChain 67(rayDir) 152
             543:    7(float) Load 542
             544:    9(fvec2) CompositeConstruct 543 543
             545:    9(fvec2) FDiv 541 544
             547:    9(fvec2) VectorTimesScalar 545 546
                              Store 548(param) 547
             549:    7(float) FunctionCall 50(noise2d(vf2;) 548(param)
             551:    7(float) FMul 549 550
             552:    7(float) Load 504(n)
             553:    7(float) FAdd 552 551
                              Store 504(n) 553
                              Line 1 301 0
             554:    7(float) Load 504(n)
             555:    7(float) ExtInst 2(GLSL.std.450) 4(FAbs) 554
             556:    7(float) ExtInst 2(GLSL.std.450) 26(Pow) 555 276
                              Store 504(n) 556
                              Line 1 302 0
             557:    7(float) Load 504(n)
             559:    7(float) FMul 557 558
             560:    7(float) Load 504(n)
             561:     21(ptr) AccessChain 67(rayDir) 152
             562:    7(float) Load 561
             563:    7(float) FMul 562 546
             564:    7(float) ExtInst 2(GLSL.std.450) 4(FAbs) 563
                              Store 565(param) 564
             566:    7(float) FunctionCall 60(saturate(f1;) 565(param)
             567:    7(float) ExtInst 2(GLSL.std.450) 46(FMix) 559 560 566
                              Store 504(n) 567
                              Line 1 303 0
             568:    8(fvec3) Load 489(finalColor)
             569:    8(fvec3) Load 481(sunCol)
             571:    8(fvec3) VectorTimesScalar 569 570
             572:    8(fvec3) FAdd 387 571
             574:    8(fvec3) VectorTimesScalar 572 573
             575:     21(ptr) AccessChain 67(rayDir) 152
             576:    7(float) Load 575
             577:    7(float) FAdd 576 558
             579:    7(float) FMul 577 578
                              Store 580(param) 579
             581:    7(float) FunctionCall 60(saturate(f1;) 580(param)
             582:    8(fvec3) VectorTimesScalar 574 581
             583:    7(float) Load 504(n)
             584:    7(float) FMul 583 550
                              Store 585(param) 584
             586:    7(float) FunctionCall 60(saturate(f1;) 585(param)
             587:    8(fvec3) CompositeConstruct 586 586 586
             588:    8(fvec3) ExtInst 2(GLSL.std.450) 46(FMix) 568 582 587
                              Store 489(finalColor) 588
                              Line 1 304 0
             590:    8(fvec3) Load 67(rayDir)
                              Store 589(param) 590
             592:    8(fvec3) Load 68(sunDir)
                              Store 591(param) 592
             593:    8(fvec3) FunctionCall 65(GetSunColorSmall(vf3;vf3;) 589(param) 591(param)
             594:    8(fvec3) Load 489(finalColor)
             595:    8(fvec3) FAdd 594 593
                              Store 489(finalColor) 595
                              Line 1 305 0
             596:    8(fvec3) Load 489(finalColor)
                              ReturnValue 596
                              FunctionEnd
75(GetEnvMapSkyline(vf3;vf3;f1;):    8(fvec3) Function None 71
      72(rayDir):     38(ptr) FunctionParameter
      73(sunDir):     38(ptr) FunctionParameter
      74(height):     21(ptr) FunctionParameter
              76:             Label
 599(finalColor):     38(ptr) Variable Function
      600(param):     38(ptr) Variable Function
      602(param):     38(ptr) Variable Function
     605(radial):     21(ptr) Variable Function
    612(skyline):     21(ptr) Variable Function
       649(mask):     21(ptr) Variable Function
      661(param):     21(ptr) Variable Function
       663(vert):     21(ptr) Variable Function
        671(hor):     21(ptr) Variable Function
      687(param):     21(ptr) Variable Function
                              Line 1 312 0
             601:    8(fvec3) Load 72(rayDir)
                              Store 600(param) 601
             603:    8(fvec3) Load 73(sunDir)
                              Store 602(param) 603
             604:    8(fvec3) FunctionCall 69(GetEnvMap(vf3;vf3;) 600(param) 602(param)
                              Store 599(finalColor) 604
                              Line 1 314 0
             606:     21(ptr) AccessChain 72(rayDir) 258
             607:    7(float) Load 606
             608:     21(ptr) AccessChain 72(rayDir) 169
             609:    7(float) Load 608
             610:    7(float) ExtInst 2(GLSL.std.450) 25(Atan2) 607 609
             611:    7(float) FMul 610 532
                              Store 605(radial) 611
                              Line 1 316 0
             614:    7(float) Load 605(radial)
             615:    7(float) FMul 613 614
             616:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 615
             618:    7(float) Load 605(radial)
             619:    7(float) FMul 617 618
             620:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 619
             621:    7(float) FAdd 616 620
             623:    7(float) Load 605(radial)
             624:    7(float) FMul 622 623
             625:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 624
             626:    7(float) FAdd 621 625
             628:    7(float) FMul 626 627
             629:    7(float) ExtInst 2(GLSL.std.450) 8(Floor) 628
                              Store 612(skyline) 629
                              Line 1 317 0
             630:    7(float) Load 605(radial)
             631:    7(float) FMul 630 532
                              Store 605(radial) 631
                              Line 1 318 0
             632:    7(float) Load 605(radial)
             633:    7(float) FMul 578 632
             634:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 633
             635:    7(float) Load 605(radial)
             636:    7(float) FMul 617 635
             637:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 636
             638:    7(float) FAdd 634 637
             639:    7(float) Load 605(radial)
             640:    7(float) FMul 622 639
             641:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 640
             642:    7(float) FAdd 638 641
             643:    7(float) FMul 642 627
             644:    7(float) ExtInst 2(GLSL.std.450) 8(Floor) 643
             646:    7(float) FMul 644 645
             647:    7(float) Load 612(skyline)
             648:    7(float) FAdd 647 646
                              Store 612(skyline) 648
                              Line 1 320 0
             650:     21(ptr) AccessChain 72(rayDir) 152
             651:    7(float) Load 650
             652:    7(float) FMul 651 546
             653:    7(float) Load 612(skyline)
             654:    7(float) FSub 652 653
             656:    7(float) FSub 654 655
             657:    7(float) Load 74(height)
             658:    7(float) FAdd 656 657
             660:    7(float) FMul 658 659
                              Store 661(param) 660
             662:    7(float) FunctionCall 60(saturate(f1;) 661(param)
                              Store 649(mask) 662
                              Line 1 322 0
             664:    7(float) Load 605(radial)
             666:    7(float) FMul 664 665
             667:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 666
             668:    7(float) ExtInst 2(GLSL.std.450) 6(FSign) 667
             669:    7(float) FMul 668 460
             670:    7(float) FAdd 669 460
                              Store 663(vert) 670
                              Line 1 324 0
             672:     21(ptr) AccessChain 72(rayDir) 152
             673:    7(float) Load 672
             675:    7(float) FMul 673 674
             676:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 675
             677:    7(float) ExtInst 2(GLSL.std.450) 6(FSign) 676
             678:    7(float) FMul 677 460
             679:    7(float) FAdd 678 460
                              Store 671(hor) 679
                              Line 1 325 0
             680:    7(float) Load 649(mask)
             681:    7(float) Load 671(hor)
             682:    7(float) Load 663(vert)
             683:    7(float) FMul 681 682
             684:    7(float) FSub 297 683
             685:    7(float) FMul 684 465
             686:    7(float) FAdd 680 685
                              Store 687(param) 686
             688:    7(float) FunctionCall 60(saturate(f1;) 687(param)
                              Store 649(mask) 688
                              Line 1 326 0
             689:    8(fvec3) Load 599(finalColor)
             692:    8(fvec3) FMul 689 691
             693:    8(fvec3) Load 599(finalColor)
             694:    7(float) Load 649(mask)
             695:    8(fvec3) CompositeConstruct 694 694 694
             696:    8(fvec3) ExtInst 2(GLSL.std.450) 46(FMix) 692 693 695
                              Store 599(finalColor) 696
                              Line 1 327 0
             697:    8(fvec3) Load 599(finalColor)
                              ReturnValue 697
                              FunctionEnd
80(matmin(vf2;vf2;):    9(fvec2) Function None 77
           78(a):     26(ptr) FunctionParameter
           79(b):     26(ptr) FunctionParameter
              81:             Label
                              Line 1 333 0
             700:     21(ptr) AccessChain 78(a) 169
             701:    7(float) Load 700
             702:     21(ptr) AccessChain 79(b) 169
             703:    7(float) Load 702
             705:   704(bool) FOrdLessThan 701 703
                              SelectionMerge 707 None
                              BranchConditional 705 706 710
             706:               Label
                                Line 1 335 0
             708:    9(fvec2)   Load 78(a)
                                ReturnValue 708
             710:               Label
                                Line 1 339 0
             711:    9(fvec2)   Load 79(b)
                                ReturnValue 711
             707:             Label
             713:    9(fvec2) Undef
                              ReturnValue 713
                              FunctionEnd
85(sdBox(vf3;vf3;):    7(float) Function None 82
           83(p):     38(ptr) FunctionParameter
      84(radius):     38(ptr) FunctionParameter
              86:             Label
       714(dist):     38(ptr) Variable Function
                              Line 1 348 0
             715:    8(fvec3) Load 83(p)
             716:    8(fvec3) ExtInst 2(GLSL.std.450) 4(FAbs) 715
             717:    8(fvec3) Load 84(radius)
             718:    8(fvec3) FSub 716 717
                              Store 714(dist) 718
                              Line 1 349 0
             719:     21(ptr) AccessChain 714(dist) 169
             720:    7(float) Load 719
             721:     21(ptr) AccessChain 714(dist) 152
             722:    7(float) Load 721
             723:     21(ptr) AccessChain 714(dist) 258
             724:    7(float) Load 723
             725:    7(float) ExtInst 2(GLSL.std.450) 40(FMax) 722 724
             726:    7(float) ExtInst 2(GLSL.std.450) 40(FMax) 720 725
             727:    7(float) ExtInst 2(GLSL.std.450) 37(FMin) 726 298
             728:    8(fvec3) Load 714(dist)
             729:    8(fvec3) CompositeConstruct 298 298 298
             730:    8(fvec3) ExtInst 2(GLSL.std.450) 40(FMax) 728 729
             731:    7(float) ExtInst 2(GLSL.std.450) 66(Length) 730
             732:    7(float) FAdd 727 731
                              ReturnValue 732
                              FunctionEnd
91(cylCap(vf3;f1;f1;):    7(float) Function None 87
           88(p):     38(ptr) FunctionParameter
           89(r):     21(ptr) FunctionParameter
      90(lenRad):     21(ptr) FunctionParameter
              92:             Label
          735(a):     21(ptr) Variable Function
                              Line 1 356 0
             736:    8(fvec3) Load 88(p)
             737:    9(fvec2) VectorShuffle 736 736 0 1
             738:    7(float) ExtInst 2(GLSL.std.450) 66(Length) 737
             739:    7(float) Load 89(r)
             740:    7(float) FSub 738 739
                              Store 735(a) 740
                              Line 1 357 0
             741:    7(float) Load 735(a)
             742:     21(ptr) AccessChain 88(p) 258
             743:    7(float) Load 742
             744:    7(float) ExtInst 2(GLSL.std.450) 4(FAbs) 743
             745:    7(float) Load 90(lenRad)
             746:    7(float) FSub 744 745)#"_str
+
R"#(
             747:    7(float) ExtInst 2(GLSL.std.450) 40(FMax) 741 746
                              Store 735(a) 747
                              Line 1 358 0
             748:    7(float) Load 735(a)
                              ReturnValue 748
                              FunctionEnd
96(smin(f1;f1;f1;):    7(float) Function None 43
           93(a):     21(ptr) FunctionParameter
           94(b):     21(ptr) FunctionParameter
           95(k):     21(ptr) FunctionParameter
              97:             Label
                              Line 1 364 0
             751:    7(float) Load 95(k)
             752:    7(float) Load 93(a)
             753:    7(float) FMul 751 752
             754:    7(float) ExtInst 2(GLSL.std.450) 29(Exp2) 753
             755:    7(float) Load 95(k)
             756:    7(float) Load 94(b)
             757:    7(float) FMul 755 756
             758:    7(float) ExtInst 2(GLSL.std.450) 29(Exp2) 757
             759:    7(float) FAdd 754 758
             760:    7(float) ExtInst 2(GLSL.std.450) 30(Log2) 759
             761:    7(float) Load 95(k)
             762:    7(float) FDiv 760 761
                              ReturnValue 762
                              FunctionEnd
101(Repeat(f1;f1;):    7(float) Function None 98
           99(a):     21(ptr) FunctionParameter
        100(len):     21(ptr) FunctionParameter
             102:             Label
                              Line 1 370 0
             765:    7(float) Load 99(a)
             766:    7(float) Load 100(len)
             767:    7(float) FMod 765 766
             768:    7(float) Load 100(len)
             769:    7(float) FMul 460 768
             770:    7(float) FSub 767 769
                              ReturnValue 770
                              FunctionEnd
106(Car(vf3;f1;):    9(fvec2) Function None 103
 104(baseCenter):     38(ptr) FunctionParameter
     105(unique):     21(ptr) FunctionParameter
             107:             Label
        773(car):     21(ptr) Variable Function
      783(param):     38(ptr) Variable Function
      784(param):     38(ptr) Variable Function
      794(param):     38(ptr) Variable Function
      795(param):     38(ptr) Variable Function
      798(param):     21(ptr) Variable Function
      800(param):     21(ptr) Variable Function
      801(param):     21(ptr) Variable Function
    803(wMirror):     38(ptr) Variable Function
     814(wheels):     21(ptr) Variable Function
      817(param):     38(ptr) Variable Function
      820(param):     21(ptr) Variable Function
      821(param):     21(ptr) Variable Function
 823(distAndMat):     26(ptr) Variable Function
      831(param):     26(ptr) Variable Function
      833(param):     26(ptr) Variable Function
                              Line 1 377 0
             774:    8(fvec3) Load 104(baseCenter)
             778:    8(fvec3) FAdd 774 777
                              Store 783(param) 778
                              Store 784(param) 782
             785:    7(float) FunctionCall 85(sdBox(vf3;vf3;) 783(param) 784(param)
                              Store 773(car) 785
                              Line 1 378 0
             786:    8(fvec3) Load 104(baseCenter)
             790:    8(fvec3) FAdd 786 789
                              Store 794(param) 790
                              Store 795(param) 793
             796:    7(float) FunctionCall 85(sdBox(vf3;vf3;) 794(param) 795(param)
             799:    7(float) Load 773(car)
                              Store 798(param) 799
                              Store 800(param) 796
                              Store 801(param) 797
             802:    7(float) FunctionCall 96(smin(f1;f1;f1;) 798(param) 800(param) 801(param)
                              Store 773(car) 802
                              Line 1 380 0
             804:    8(fvec3) Load 104(baseCenter)
             807:    8(fvec3) FAdd 804 806
                              Store 803(wMirror) 807
                              Line 1 381 0
             808:     21(ptr) AccessChain 803(wMirror) 258
             809:    7(float) Load 808
             810:    7(float) ExtInst 2(GLSL.std.450) 4(FAbs) 809
             812:    7(float) FSub 810 811
             813:     21(ptr) AccessChain 803(wMirror) 258
                              Store 813 812
                              Line 1 383 0
             818:    8(fvec3) Load 803(wMirror)
             819:    8(fvec3) VectorShuffle 818 818 2 1 0
                              Store 817(param) 819
                              Store 820(param) 815
                              Store 821(param) 816
             822:    7(float) FunctionCall 91(cylCap(vf3;f1;f1;) 817(param) 820(param) 821(param)
                              Store 814(wheels) 822
                              Line 1 385 0
             824:    7(float) Load 814(wheels)
             825:    9(fvec2) CompositeConstruct 824 276
                              Store 823(distAndMat) 825
                              Line 1 386 0
             826:    7(float) Load 773(car)
             828:    7(float) Load 105(unique)
             829:    7(float) FAdd 827 828
             830:    9(fvec2) CompositeConstruct 826 829
             832:    9(fvec2) Load 823(distAndMat)
                              Store 831(param) 832
                              Store 833(param) 830
             834:    9(fvec2) FunctionCall 80(matmin(vf2;vf2;) 831(param) 833(param)
                              Store 823(distAndMat) 834
                              Line 1 387 0
             835:    9(fvec2) Load 823(distAndMat)
                              ReturnValue 835
                              FunctionEnd
111(CityBlock(vf3;vf2;):    9(fvec2) Function None 108
          109(p):     38(ptr) FunctionParameter
       110(pint):     26(ptr) FunctionParameter
             112:             Label
       838(rand):    131(ptr) Variable Function
      839(param):     26(ptr) Variable Function
      844(param):     26(ptr) Variable Function
      850(rand2):     26(ptr) Variable Function
      851(param):     26(ptr) Variable Function
    855(baseRad):     21(ptr) Variable Function
 866(baseCenter):     38(ptr) Variable Function
     870(height):     21(ptr) Variable Function
   878(downtown):     21(ptr) Variable Function
      882(param):     21(ptr) Variable Function
          901(d):     21(ptr) Variable Function
      906(param):     38(ptr) Variable Function
      908(param):     38(ptr) Variable Function
    914(height2):     21(ptr) Variable Function
      944(param):     38(ptr) Variable Function
      945(param):     38(ptr) Variable Function
      961(param):     38(ptr) Variable Function
      962(param):     38(ptr) Variable Function
      982(param):     38(ptr) Variable Function
      983(param):     38(ptr) Variable Function
   986(topWidth):     21(ptr) Variable Function
     1008(param):     38(ptr) Variable Function
     1009(param):     38(ptr) Variable Function
     1025(param):     38(ptr) Variable Function
     1027(param):     21(ptr) Variable Function
     1028(param):     21(ptr) Variable Function
     1065(param):     38(ptr) Variable Function
     1066(param):     38(ptr) Variable Function
    1069(boxPos):     38(ptr) Variable Function
       1086(big):     21(ptr) Variable Function
     1119(param):     38(ptr) Variable Function
     1121(param):     38(ptr) Variable Function
1140(distAndMat):     26(ptr) Variable Function
     1145(param):     38(ptr) Variable Function
     1147(param):     38(ptr) Variable Function
     1150(param):     26(ptr) Variable Function
     1152(param):     26(ptr) Variable Function
                              Line 1 394 0
             840:    9(fvec2) Load 110(pint)
                              Store 839(param) 840
             841:    9(fvec2) FunctionCall 33(Hash22(vf2;) 839(param)
             842:  130(fvec4) Load 838(rand)
             843:  130(fvec4) VectorShuffle 842 841 4 5 2 3
                              Store 838(rand) 843
                              Line 1 395 0
             845:  130(fvec4) Load 838(rand)
             846:    9(fvec2) VectorShuffle 845 845 0 1
                              Store 844(param) 846
             847:    9(fvec2) FunctionCall 33(Hash22(vf2;) 844(param)
             848:  130(fvec4) Load 838(rand)
             849:  130(fvec4) VectorShuffle 848 847 0 1 4 5
                              Store 838(rand) 849
                              Line 1 397 0
             852:  130(fvec4) Load 838(rand)
             853:    9(fvec2) VectorShuffle 852 852 2 3
                              Store 851(param) 853
             854:    9(fvec2) FunctionCall 33(Hash22(vf2;) 851(param)
                              Store 850(rand2) 854
                              Line 1 399 0
             856:     21(ptr) AccessChain 838(rand) 169
             857:    7(float) Load 856
             858:    7(float) FMul 857 645
             859:    7(float) FAdd 558 858
                              Store 855(baseRad) 859
                              Line 1 400 0
             860:    7(float) Load 855(baseRad)
             862:    7(float) FMul 860 861
             863:    7(float) FAdd 862 460
             864:    7(float) ExtInst 2(GLSL.std.450) 8(Floor) 863
             865:    7(float) FDiv 864 861
                              Store 855(baseRad) 865
                              Line 1 402 0
             867:    8(fvec3) Load 109(p)
             869:    8(fvec3) FSub 867 868
                              Store 866(baseCenter) 869
                              Line 1 404 0
             872:     21(ptr) AccessChain 838(rand) 871
             873:    7(float) Load 872
             874:     21(ptr) AccessChain 838(rand) 258
             875:    7(float) Load 874
             876:    7(float) FMul 873 875
             877:    7(float) FAdd 876 645
                              Store 870(height) 877
                              Line 1 406 0
             879:    9(fvec2) Load 110(pint)
             880:    7(float) ExtInst 2(GLSL.std.450) 66(Length) 879
             881:    7(float) FDiv 532 880
                              Store 882(param) 881
             883:    7(float) FunctionCall 60(saturate(f1;) 882(param)
                              Store 878(downtown) 883
                              Line 1 407 0
             884:    7(float) Load 878(downtown)
             885:    7(float) Load 870(height)
             886:    7(float) FMul 885 884
                              Store 870(height) 886
                              Line 1 408 0
             888:    7(float) Load 855(baseRad)
             890:    7(float) FSub 888 889
             891:    7(float) FMul 890 861
             892:    7(float) FAdd 887 891
             893:    7(float) Load 870(height)
             894:    7(float) FMul 893 892
                              Store 870(height) 894
                              Line 1 409 0
             895:    7(float) Load 870(height)
             896:    7(float) FAdd 895 645
                              Store 870(height) 896
                              Line 1 410 0
             897:    7(float) Load 870(height)
             898:    7(float) FMul 897 861
             899:    7(float) ExtInst 2(GLSL.std.450) 8(Floor) 898
             900:    7(float) FMul 899 465
                              Store 870(height) 900
                              Line 1 412 0
             902:    7(float) Load 855(baseRad)
             903:    7(float) Load 870(height)
             904:    7(float) Load 855(baseRad)
             905:    8(fvec3) CompositeConstruct 902 903 904
             907:    8(fvec3) Load 866(baseCenter)
                              Store 906(param) 907
                              Store 908(param) 905
             909:    7(float) FunctionCall 85(sdBox(vf3;vf3;) 906(param) 908(param)
                              Store 901(d) 909
                              Line 1 413 0
             910:    7(float) Load 901(d)
             911:     21(ptr) AccessChain 109(p) 152
             912:    7(float) Load 911
             913:    7(float) ExtInst 2(GLSL.std.450) 37(FMin) 910 912
                              Store 901(d) 913
                              Line 1 415 0
             915:     21(ptr) AccessChain 838(rand) 152
             916:    7(float) Load 915
             917:    7(float) FMul 916 277
             918:    7(float) FSub 917 297
             919:    7(float) ExtInst 2(GLSL.std.450) 40(FMax) 298 918
             920:    7(float) Load 878(downtown)
             921:    7(float) FMul 919 920
                              Store 914(height2) 921
                              Line 1 416 0
             922:    7(float) Load 914(height2)
             923:    7(float) FMul 922 861
             924:    7(float) ExtInst 2(GLSL.std.450) 8(Floor) 923
             925:    7(float) FMul 924 465
                              Store 914(height2) 925
                              Line 1 417 0
             926:    9(fvec2) Load 850(rand2)
             927:    9(fvec2) VectorTimesScalar 926 861
             928:    9(fvec2) ExtInst 2(GLSL.std.450) 8(Floor) 927
             929:    9(fvec2) VectorTimesScalar 928 465
                              Store 850(rand2) 929
                              Line 1 418 0
             930:    7(float) Load 901(d)
             931:    8(fvec3) Load 866(baseCenter)
             932:    7(float) Load 870(height)
             933:    8(fvec3) CompositeConstruct 298 932 298
             934:    8(fvec3) FSub 931 933
             935:    7(float) Load 855(baseRad)
             936:    7(float) Load 914(height2)
             937:     21(ptr) AccessChain 850(rand2) 152
             938:    7(float) Load 937
             939:    7(float) FSub 936 938
             940:    7(float) Load 855(baseRad)
             942:    7(float) FMul 940 941
             943:    8(fvec3) CompositeConstruct 935 939 942
                              Store 944(param) 934
                              Store 945(param) 943
             946:    7(float) FunctionCall 85(sdBox(vf3;vf3;) 944(param) 945(param)
             947:    7(float) ExtInst 2(GLSL.std.450) 37(FMin) 930 946
                              Store 901(d) 947
                              Line 1 419 0
             948:    7(float) Load 901(d)
             949:    8(fvec3) Load 866(baseCenter)
             950:    7(float) Load 870(height)
             951:    8(fvec3) CompositeConstruct 298 950 298
             952:    8(fvec3) FSub 949 951
             953:    7(float) Load 855(baseRad)
             954:    7(float) FMul 953 941
             955:    7(float) Load 914(height2)
             956:     21(ptr) AccessChain 850(rand2) 169
             957:    7(float) Load 956
             958:    7(float) FSub 955 957
             959:    7(float) Load 855(baseRad)
             960:    8(fvec3) CompositeConstruct 954 958 959
                              Store 961(param) 952
                              Store 962(param) 960
             963:    7(float) FunctionCall 85(sdBox(vf3;vf3;) 961(param) 962(param)
             964:    7(float) ExtInst 2(GLSL.std.450) 37(FMin) 948 963
                              Store 901(d) 964
                              Line 1 420 0
             965:     21(ptr) AccessChain 850(rand2) 152
             966:    7(float) Load 965
             967:   704(bool) FOrdGreaterThan 966 536
                              SelectionMerge 969 None
                              BranchConditional 967 968 1013
             968:               Label
                                Line 1 422 0
             970:    7(float)   Load 901(d)
             971:    8(fvec3)   Load 866(baseCenter)
             972:    7(float)   Load 870(height)
             973:    8(fvec3)   CompositeConstruct 298 972 298
             974:    8(fvec3)   FSub 971 973
             975:    7(float)   Load 855(baseRad)
             977:    7(float)   FMul 975 976)#"_str
+
R"#(
             978:    7(float)   Load 914(height2)
             979:    7(float)   Load 855(baseRad)
             980:    7(float)   FMul 979 976
             981:    8(fvec3)   CompositeConstruct 977 978 980
                                Store 982(param) 974
                                Store 983(param) 981
             984:    7(float)   FunctionCall 85(sdBox(vf3;vf3;) 982(param) 983(param)
             985:    7(float)   ExtInst 2(GLSL.std.450) 37(FMin) 970 984
                                Store 901(d) 985
                                Line 1 424 0
             987:    7(float)   Load 855(baseRad)
                                Store 986(topWidth) 987
                                Line 1 425 0
             988:    7(float)   Load 914(height2)
             989:   704(bool)   FOrdGreaterThan 988 298
                                SelectionMerge 991 None
                                BranchConditional 989 990 991
             990:                 Label
                                  Line 1 427 0
             992:    7(float)     Load 855(baseRad)
             993:    7(float)     FMul 992 976
                                  Store 986(topWidth) 993
                                  Branch 991
             991:               Label
                                Line 1 430 0
             994:    7(float)   Load 901(d)
             995:    8(fvec3)   Load 866(baseCenter)
             996:    7(float)   Load 870(height)
             997:    7(float)   Load 914(height2)
             998:    7(float)   FAdd 996 997
             999:    8(fvec3)   CompositeConstruct 298 998 298
            1000:    8(fvec3)   FSub 995 999
            1001:    7(float)   Load 986(topWidth)
            1003:    7(float)   FSub 1001 1002
            1005:    7(float)   Load 986(topWidth)
            1006:    7(float)   FSub 1005 1002
            1007:    8(fvec3)   CompositeConstruct 1003 1004 1006
                                Store 1008(param) 1000
                                Store 1009(param) 1007
            1010:    7(float)   FunctionCall 85(sdBox(vf3;vf3;) 1008(param) 1009(param)
            1011:    7(float)   FNegate 1010
            1012:    7(float)   ExtInst 2(GLSL.std.450) 40(FMax) 994 1011
                                Store 901(d) 1012
                                Branch 969
            1013:               Label
                                Line 1 435 0
            1014:    7(float)   Load 914(height2)
            1015:   704(bool)   FOrdGreaterThan 1014 298
                                SelectionMerge 1017 None
                                BranchConditional 1015 1016 1017
            1016:                 Label
                                  Line 1 437 0
            1018:    7(float)     Load 901(d)
            1019:    8(fvec3)     Load 866(baseCenter)
            1020:    7(float)     Load 870(height)
            1021:    8(fvec3)     CompositeConstruct 298 1020 298
            1022:    8(fvec3)     FSub 1019 1021
            1023:    7(float)     Load 855(baseRad)
            1024:    7(float)     FMul 1023 976
            1026:    8(fvec3)     VectorShuffle 1022 1022 0 2 1
                                  Store 1025(param) 1026
                                  Store 1027(param) 1024
            1029:    7(float)     Load 914(height2)
                                  Store 1028(param) 1029
            1030:    7(float)     FunctionCall 91(cylCap(vf3;f1;f1;) 1025(param) 1027(param) 1028(param)
            1031:    7(float)     ExtInst 2(GLSL.std.450) 37(FMin) 1018 1030
                                  Store 901(d) 1031
                                  Branch 1017
            1017:               Label
                                Branch 969
             969:             Label
                              Line 1 443 0
            1032:    7(float) Load 901(d)
            1033:    8(fvec3) Load 866(baseCenter)
            1034:     21(ptr) AccessChain 838(rand) 169
            1035:    7(float) Load 1034
            1036:    7(float) FSub 1035 460
            1037:    7(float) Load 855(baseRad)
            1038:    7(float) FMul 1036 1037
            1039:    7(float) Load 870(height)
            1040:    7(float) Load 914(height2)
            1041:    7(float) FAdd 1039 1040
            1042:     21(ptr) AccessChain 838(rand) 152
            1043:    7(float) Load 1042
            1044:    7(float) FSub 1043 460
            1045:    7(float) Load 855(baseRad)
            1046:    7(float) FMul 1044 1045
            1047:    8(fvec3) CompositeConstruct 1038 1041 1046
            1048:    8(fvec3) FSub 1033 1047
            1049:    7(float) Load 855(baseRad)
            1051:    7(float) FMul 1049 1050
            1052:     21(ptr) AccessChain 838(rand) 258
            1053:    7(float) Load 1052
            1054:    7(float) FMul 1051 1053
            1055:     21(ptr) AccessChain 850(rand2) 152
            1056:    7(float) Load 1055
            1057:    7(float) FMul 645 1056
            1058:    7(float) Load 855(baseRad)
            1059:    7(float) FMul 1058 1050
            1060:     21(ptr) AccessChain 850(rand2) 169
            1061:    7(float) Load 1060
            1062:    7(float) FMul 1059 1061
            1063:    7(float) FAdd 1062 485
            1064:    8(fvec3) CompositeConstruct 1054 1057 1063
                              Store 1065(param) 1048
                              Store 1066(param) 1064
            1067:    7(float) FunctionCall 85(sdBox(vf3;vf3;) 1065(param) 1066(param)
            1068:    7(float) ExtInst 2(GLSL.std.450) 37(FMin) 1032 1067
                              Store 901(d) 1068
                              Line 1 445 0
            1070:    8(fvec3) Load 866(baseCenter)
            1071:     21(ptr) AccessChain 850(rand2) 169
            1072:    7(float) Load 1071
            1073:    7(float) FSub 1072 460
            1074:    7(float) Load 855(baseRad)
            1075:    7(float) FMul 1073 1074
            1076:    7(float) Load 870(height)
            1077:    7(float) Load 914(height2)
            1078:    7(float) FAdd 1076 1077
            1079:     21(ptr) AccessChain 850(rand2) 152
            1080:    7(float) Load 1079
            1081:    7(float) FSub 1080 460
            1082:    7(float) Load 855(baseRad)
            1083:    7(float) FMul 1081 1082
            1084:    8(fvec3) CompositeConstruct 1075 1078 1083
            1085:    8(fvec3) FSub 1070 1084
                              Store 1069(boxPos) 1085
                              Line 1 447 0
            1087:     21(ptr) AccessChain 1069(boxPos) 169
            1088:    7(float) Load 1087
            1089:    7(float) ExtInst 2(GLSL.std.450) 6(FSign) 1088
                              Store 1086(big) 1089
                              Line 1 448 0
            1090:     21(ptr) AccessChain 1069(boxPos) 169
            1091:    7(float) Load 1090
            1092:    7(float) ExtInst 2(GLSL.std.450) 4(FAbs) 1091
            1093:    7(float) FSub 1092 811
            1094:    7(float) Load 855(baseRad)
            1095:    7(float) FMul 1094 1050
            1096:     21(ptr) AccessChain 838(rand) 871
            1097:    7(float) Load 1096
            1098:    7(float) FMul 1095 1097
            1099:    7(float) FSub 1093 1098
            1100:     21(ptr) AccessChain 1069(boxPos) 169
                              Store 1100 1099
                              Line 1 449 0
            1101:    7(float) Load 901(d)
            1102:    7(float) Load 855(baseRad)
            1103:    7(float) FMul 1102 1050
            1104:     21(ptr) AccessChain 838(rand) 871
            1105:    7(float) Load 1104
            1106:    7(float) FMul 1103 1105
            1107:     21(ptr) AccessChain 838(rand) 152
            1108:    7(float) Load 1107
            1109:    7(float) FMul 690 1108
            1110:    7(float) Load 855(baseRad)
            1111:    7(float) FMul 1110 558
            1112:     21(ptr) AccessChain 838(rand) 169
            1113:    7(float) Load 1112
            1114:    7(float) FMul 1111 1113
            1115:    7(float) Load 1086(big)
            1116:    7(float) FMul 1115 485
            1117:    7(float) FAdd 1114 1116
            1118:    8(fvec3) CompositeConstruct 1106 1109 1117
            1120:    8(fvec3) Load 1069(boxPos)
                              Store 1119(param) 1120
                              Store 1121(param) 1118
            1122:    7(float) FunctionCall 85(sdBox(vf3;vf3;) 1119(param) 1121(param)
            1123:    7(float) ExtInst 2(GLSL.std.450) 37(FMin) 1101 1122
                              Store 901(d) 1123
                              Line 1 450 0
            1124:     21(ptr) AccessChain 838(rand) 152
            1125:    7(float) Load 1124
            1127:   704(bool) FOrdLessThan 1125 1126
                              SelectionMerge 1129 None
                              BranchConditional 1127 1128 1129
            1128:               Label
                                Line 1 452 0
            1130:    7(float)   Load 901(d)
            1131:    8(fvec3)   Load 866(baseCenter)
            1132:    7(float)   Load 870(height)
            1133:    8(fvec3)   CompositeConstruct 298 1132 298
            1134:    8(fvec3)   FSub 1131 1133
            1135:    7(float)   ExtInst 2(GLSL.std.450) 66(Length) 1134
            1136:    7(float)   Load 855(baseRad)
            1137:    7(float)   FMul 1136 976
            1138:    7(float)   FSub 1135 1137
            1139:    7(float)   ExtInst 2(GLSL.std.450) 37(FMin) 1130 1138
                                Store 901(d) 1139
                                Branch 1129
            1129:             Label
                              Line 1 457 0
            1141:    7(float) Load 901(d)
            1142:    9(fvec2) CompositeConstruct 1141 298
                              Store 1140(distAndMat) 1142
                              Line 1 458 0
            1146:    8(fvec3) Load 866(baseCenter)
                              Store 1145(param) 1146
                              Store 1147(param) 1144
            1148:    7(float) FunctionCall 85(sdBox(vf3;vf3;) 1145(param) 1147(param)
            1149:    9(fvec2) CompositeConstruct 1148 297
            1151:    9(fvec2) Load 1140(distAndMat)
                              Store 1150(param) 1151
                              Store 1152(param) 1149
            1153:    9(fvec2) FunctionCall 80(matmin(vf2;vf2;) 1150(param) 1152(param)
                              Store 1140(distAndMat) 1153
                              Line 1 459 0
            1154:    9(fvec2) Load 1140(distAndMat)
                              ReturnValue 1154
                              FunctionEnd
115(DistanceToObject(vf3;):    9(fvec2) Function None 113
          114(p):     38(ptr) FunctionParameter
             116:             Label
       1157(rep):     38(ptr) Variable Function
1164(distAndMat):     26(ptr) Variable Function
     1168(param):     38(ptr) Variable Function
     1170(param):     26(ptr) Variable Function
        1172(p2):     38(ptr) Variable Function
   1175(carTime):     21(ptr) Variable Function
1180(crossStreet):     21(ptr) Variable Function
1181(repeatDist):     21(ptr) Variable Function
     1215(param):     21(ptr) Variable Function
     1216(param):     21(ptr) Variable Function
  1242(uniqueID):     21(ptr) Variable Function
     1248(param):     21(ptr) Variable Function
     1251(param):     21(ptr) Variable Function
     1255(param):     21(ptr) Variable Function
 1265(frontBack):     21(ptr) Variable Function
     1269(param):     21(ptr) Variable Function
   1288(carDist):     26(ptr) Variable Function
     1289(param):     38(ptr) Variable Function
     1291(param):     21(ptr) Variable Function
     1294(param):     26(ptr) Variable Function
     1296(param):     26(ptr) Variable Function
                              Line 1 466 0
            1158:    8(fvec3) Load 114(p)
                              Store 1157(rep) 1158
                              Line 1 467 0
            1159:    8(fvec3) Load 114(p)
            1160:    9(fvec2) VectorShuffle 1159 1159 0 2
            1161:    9(fvec2) ExtInst 2(GLSL.std.450) 10(Fract) 1160
            1162:    8(fvec3) Load 1157(rep)
            1163:    8(fvec3) VectorShuffle 1162 1161 3 1 4
                              Store 1157(rep) 1163
                              Line 1 469 0
            1165:    8(fvec3) Load 114(p)
            1166:    9(fvec2) VectorShuffle 1165 1165 0 2
            1167:    9(fvec2) ExtInst 2(GLSL.std.450) 8(Floor) 1166
            1169:    8(fvec3) Load 1157(rep)
                              Store 1168(param) 1169
                              Store 1170(param) 1167
            1171:    9(fvec2) FunctionCall 111(CityBlock(vf3;vf2;) 1168(param) 1170(param)
                              Store 1164(distAndMat) 1171
                              Line 1 471 0
            1173:    8(fvec3) Load 114(p)
                              Store 1172(p2) 1173
                              Line 1 472 0
            1174:    8(fvec3) Load 1172(p2)
                              Store 1157(rep) 1174
                              Line 1 474 0
            1178:    7(float) Load 1177(localTime)
            1179:    7(float) FMul 1178 558
                              Store 1175(carTime) 1179
                              Line 1 476 0
                              Store 1180(crossStreet) 297
                              Line 1 478 0
                              Store 1181(repeatDist) 536
                              Line 1 479 0
            1182:     21(ptr) AccessChain 1157(rep) 169
            1183:    7(float) Load 1182
            1184:    7(float) ExtInst 2(GLSL.std.450) 10(Fract) 1183
            1185:    7(float) FSub 1184 460
            1186:    7(float) ExtInst 2(GLSL.std.450) 4(FAbs) 1185
            1187:   704(bool) FOrdLessThan 1186 1143
                              SelectionMerge 1189 None
                              BranchConditional 1187 1188 1189
            1188:               Label
                                Line 1 481 0
            1190:     21(ptr)   AccessChain 1172(p2) 169
            1191:    7(float)   Load 1190
            1192:    7(float)   FAdd 1191 465
            1193:     21(ptr)   AccessChain 1172(p2) 169
                                Store 1193 1192
                                Line 1 482 0
            1194:    8(fvec3)   Load 1172(p2)
            1195:    9(fvec2)   VectorShuffle 1194 1194 2 0
            1198:    9(fvec2)   FMul 1195 1197
            1199:    8(fvec3)   Load 1172(p2)
            1200:    8(fvec3)   VectorShuffle 1199 1198 3 1 4
                                Store 1172(p2) 1200
                                Line 1 483 0
            1201:    8(fvec3)   Load 1172(p2)
            1202:    9(fvec2)   VectorShuffle 1201 1201 0 2
            1203:    8(fvec3)   Load 1157(rep)
            1204:    8(fvec3)   VectorShuffle 1203 1202 3 1 4
                                Store 1157(rep) 1204
                                Line 1 484 0
                                Store 1180(crossStreet) 298
                                Line 1 485 0
                                Store 1181(repeatDist) 645
                                Branch 1189
            1189:             Label
                              Line 1 489 0
            1205:     21(ptr) AccessChain 1172(p2) 169
            1206:    7(float) Load 1205
            1207:    7(float) ExtInst 2(GLSL.std.450) 8(Floor) 1206
            1208:     21(ptr) AccessChain 1157(rep) 258
            1209:    7(float) Load 1208
            1210:    7(float) FAdd 1209 1207
            1211:     21(ptr) AccessChain 1157(rep) 258
                              Store 1211 1210
                              Line 1 490 0
            1212:     21(ptr) AccessChain 1172(p2) 169
            1213:    7(float) Load 1212)#"_str
+
R"#(
            1214:    7(float) FSub 1213 460
                              Store 1215(param) 1214
                              Store 1216(param) 297
            1217:    7(float) FunctionCall 101(Repeat(f1;f1;) 1215(param) 1216(param)
            1218:     21(ptr) AccessChain 1157(rep) 169
                              Store 1218 1217
                              Line 1 491 0
            1219:     21(ptr) AccessChain 1157(rep) 258
            1220:    7(float) Load 1219
            1221:     21(ptr) AccessChain 1157(rep) 169
            1222:    7(float) Load 1221
            1223:    7(float) ExtInst 2(GLSL.std.450) 6(FSign) 1222
            1224:    7(float) FMul 1220 1223
            1225:     21(ptr) AccessChain 1157(rep) 258
                              Store 1225 1224
                              Line 1 492 0
            1226:     21(ptr) AccessChain 1157(rep) 169
            1227:    7(float) Load 1226
            1228:     21(ptr) AccessChain 1157(rep) 169
            1229:    7(float) Load 1228
            1230:    7(float) ExtInst 2(GLSL.std.450) 6(FSign) 1229
            1231:    7(float) FMul 1227 1230
            1233:    7(float) FSub 1231 1232
            1234:     21(ptr) AccessChain 1157(rep) 169
                              Store 1234 1233
                              Line 1 493 0
            1235:    7(float) Load 1175(carTime)
            1236:    7(float) Load 1180(crossStreet)
            1237:    7(float) FMul 1235 1236
            1238:     21(ptr) AccessChain 1157(rep) 258
            1239:    7(float) Load 1238
            1240:    7(float) FSub 1239 1237
            1241:     21(ptr) AccessChain 1157(rep) 258
                              Store 1241 1240
                              Line 1 495 0
            1243:     21(ptr) AccessChain 1157(rep) 258
            1244:    7(float) Load 1243
            1245:    7(float) Load 1181(repeatDist)
            1246:    7(float) FDiv 1244 1245
            1247:    7(float) ExtInst 2(GLSL.std.450) 8(Floor) 1246
                              Store 1242(uniqueID) 1247
                              Line 1 496 0
            1249:     21(ptr) AccessChain 1157(rep) 258
            1250:    7(float) Load 1249
                              Store 1248(param) 1250
            1252:    7(float) Load 1181(repeatDist)
                              Store 1251(param) 1252
            1253:    7(float) FunctionCall 101(Repeat(f1;f1;) 1248(param) 1251(param)
            1254:     21(ptr) AccessChain 1157(rep) 258
                              Store 1254 1253
                              Line 1 497 0
            1256:    7(float) Load 1242(uniqueID)
                              Store 1255(param) 1256
            1257:    7(float) FunctionCall 24(Hash11(f1;) 1255(param)
            1259:    7(float) FMul 1257 1258
            1260:    7(float) FSub 1259 779
            1261:     21(ptr) AccessChain 1157(rep) 169
            1262:    7(float) Load 1261
            1263:    7(float) FAdd 1262 1260
            1264:     21(ptr) AccessChain 1157(rep) 169
                              Store 1264 1263
                              Line 1 499 0
            1266:    7(float) Load 1242(uniqueID)
            1268:    7(float) FMul 1266 1267
                              Store 1269(param) 1268
            1270:    7(float) FunctionCall 24(Hash11(f1;) 1269(param)
            1272:    7(float) FMul 1270 1271
            1273:    7(float) FSub 1272 1232
                              Store 1265(frontBack) 1273
                              Line 1 500 0
            1274:    7(float) Load 1177(localTime)
            1275:    7(float) FMul 1274 277
            1276:    7(float) Load 1242(uniqueID)
            1277:    7(float) FAdd 1275 1276
            1278:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 1277
            1279:    7(float) Load 1265(frontBack)
            1280:    7(float) FMul 1279 1278
                              Store 1265(frontBack) 1280
                              Line 1 501 0
            1281:    7(float) Load 1265(frontBack)
            1282:    7(float) Load 1180(crossStreet)
            1283:    7(float) FMul 1281 1282
            1284:     21(ptr) AccessChain 1157(rep) 258
            1285:    7(float) Load 1284
            1286:    7(float) FAdd 1285 1283
            1287:     21(ptr) AccessChain 1157(rep) 258
                              Store 1287 1286
                              Line 1 503 0
            1290:    8(fvec3) Load 1157(rep)
                              Store 1289(param) 1290
            1292:    7(float) Load 1242(uniqueID)
                              Store 1291(param) 1292
            1293:    9(fvec2) FunctionCall 106(Car(vf3;f1;) 1289(param) 1291(param)
                              Store 1288(carDist) 1293
                              Line 1 504 0
            1295:    9(fvec2) Load 1164(distAndMat)
                              Store 1294(param) 1295
            1297:    9(fvec2) Load 1288(carDist)
                              Store 1296(param) 1297
            1298:    9(fvec2) FunctionCall 80(matmin(vf2;vf2;) 1294(param) 1296(param)
                              Store 1164(distAndMat) 1298
                              Line 1 505 0
            1299:    9(fvec2) Load 1164(distAndMat)
                              ReturnValue 1299
                              FunctionEnd
123(CalcWindows(vf2;vf3;vf3;f1;vf3;):           3 Function None 117
      118(block):     26(ptr) FunctionParameter
        119(pos):     38(ptr) FunctionParameter
   120(texColor):     38(ptr) FunctionParameter
  121(windowRef):     21(ptr) FunctionParameter
     122(normal):     38(ptr) FunctionParameter
             124:             Label
       1302(hue):     38(ptr) Variable Function
     1303(param):     26(ptr) Variable Function
     1310(param):     26(ptr) Variable Function
     1316(param):     26(ptr) Variable Function
    1326(window):     21(ptr) Variable Function
  1343(winWidth):     21(ptr) Variable Function
     1347(param):     26(ptr) Variable Function
     1390(param):     26(ptr) Variable Function
      1397(wave):     21(ptr) Variable Function
      1419(pits):     21(ptr) Variable Function
                              Line 1 512 0
            1304:    9(fvec2) Load 118(block)
                              Store 1303(param) 1304
            1305:    7(float) FunctionCall 29(Hash21(vf2;) 1303(param)
            1306:    7(float) FMul 1305 976
            1307:    9(fvec2) Load 118(block)
            1309:    9(fvec2) VectorTimesScalar 1307 1308
                              Store 1310(param) 1309
            1311:    7(float) FunctionCall 29(Hash21(vf2;) 1310(param)
            1312:    7(float) FMul 1311 941
            1313:    9(fvec2) Load 118(block)
            1315:    9(fvec2) VectorTimesScalar 1313 1314
                              Store 1316(param) 1315
            1317:    7(float) FunctionCall 29(Hash21(vf2;) 1316(param)
            1318:    7(float) FMul 1317 460
            1319:    8(fvec3) CompositeConstruct 1306 1312 1318
                              Store 1302(hue) 1319
                              Line 1 513 0
            1320:    8(fvec3) Load 1302(hue)
            1321:    8(fvec3) VectorTimesScalar 1320 941
            1322:    8(fvec3) Load 120(texColor)
            1323:    8(fvec3) FAdd 1322 1321
                              Store 120(texColor) 1323
                              Line 1 514 0
            1324:    8(fvec3) Load 120(texColor)
            1325:    8(fvec3) VectorTimesScalar 1324 573
                              Store 120(texColor) 1325
                              Line 1 516 0
                              Store 1326(window) 298
                              Line 1 517 0
            1327:    7(float) Load 1326(window)
            1328:     21(ptr) AccessChain 119(pos) 152
            1329:    7(float) Load 1328
            1330:    7(float) FMul 1329 861
            1331:    7(float) FSub 1330 1143
            1332:    7(float) ExtInst 2(GLSL.std.450) 10(Fract) 1331
            1333:    7(float) FMul 1332 277
            1334:    7(float) FAdd 1333 645
            1335:    7(float) ExtInst 2(GLSL.std.450) 8(Floor) 1334
            1336:    7(float) ExtInst 2(GLSL.std.450) 46(FMix) 558 297 1335
            1337:    7(float) ExtInst 2(GLSL.std.450) 40(FMax) 1327 1336
                              Store 1326(window) 1337
                              Line 1 518 0
            1338:     21(ptr) AccessChain 119(pos) 152
            1339:    7(float) Load 1338
            1340:   704(bool) FOrdLessThan 1339 465
                              SelectionMerge 1342 None
                              BranchConditional 1340 1341 1342
            1341:               Label
                                Line 1 520 0
                                Store 1326(window) 297
                                Branch 1342
            1342:             Label
                              Line 1 524 0
            1344:    9(fvec2) Load 118(block)
            1346:    9(fvec2) VectorTimesScalar 1344 1345
                              Store 1347(param) 1346
            1348:    7(float) FunctionCall 29(Hash21(vf2;) 1347(param)
            1349:    7(float) FMul 1348 277
                              Store 1343(winWidth) 1349
                              Line 1 525 0
            1350:    7(float) Load 1343(winWidth)
            1352:   704(bool) FOrdLessThan 1350 1351
            1353:    7(float) Load 1343(winWidth)
            1354:   704(bool) FOrdGreaterThanEqual 1353 297
            1355:   704(bool) LogicalAnd 1352 1354
                              SelectionMerge 1357 None
                              BranchConditional 1355 1356 1357
            1356:               Label
                                Line 1 527 0
                                Store 1343(winWidth) 1351
                                Branch 1357
            1357:             Label
                              Line 1 530 0
            1358:    7(float) Load 1326(window)
            1359:     21(ptr) AccessChain 119(pos) 169
            1360:    7(float) Load 1359
            1362:    7(float) FMul 1360 1361
            1363:    7(float) FAdd 1362 465
            1364:    7(float) ExtInst 2(GLSL.std.450) 10(Fract) 1363
            1365:    7(float) Load 1343(winWidth)
            1366:    7(float) FMul 1364 1365
            1367:    7(float) ExtInst 2(GLSL.std.450) 8(Floor) 1366
            1368:    7(float) ExtInst 2(GLSL.std.450) 46(FMix) 558 297 1367
            1369:    7(float) ExtInst 2(GLSL.std.450) 40(FMax) 1358 1368
                              Store 1326(window) 1369
                              Line 1 531 0
            1370:    7(float) Load 1326(window)
            1371:     21(ptr) AccessChain 119(pos) 258
            1372:    7(float) Load 1371
            1373:    7(float) FMul 1372 1361
            1374:    7(float) FAdd 1373 465
            1375:    7(float) ExtInst 2(GLSL.std.450) 10(Fract) 1374
            1376:    7(float) Load 1343(winWidth)
            1377:    7(float) FMul 1375 1376
            1378:    7(float) ExtInst 2(GLSL.std.450) 8(Floor) 1377
            1379:    7(float) ExtInst 2(GLSL.std.450) 46(FMix) 558 297 1378
            1380:    7(float) ExtInst 2(GLSL.std.450) 40(FMax) 1370 1379
                              Store 1326(window) 1380
                              Line 1 532 0
            1381:    7(float) Load 1326(window)
            1382:   704(bool) FOrdLessThan 1381 460
                              SelectionMerge 1384 None
                              BranchConditional 1382 1383 1384
            1383:               Label
                                Line 1 534 0
            1385:    7(float)   Load 121(windowRef)
            1386:    7(float)   FAdd 1385 297
                                Store 121(windowRef) 1386
                                Branch 1384
            1384:             Label
                              Line 1 538 0
            1387:    9(fvec2) Load 118(block)
            1389:    9(fvec2) VectorTimesScalar 1387 1388
                              Store 1390(param) 1389
            1391:    7(float) FunctionCall 29(Hash21(vf2;) 1390(param)
            1392:    7(float) Load 1326(window)
            1393:    7(float) FMul 1392 1391
                              Store 1326(window) 1393
                              Line 1 539 0
            1394:    7(float) Load 1326(window)
            1395:    8(fvec3) Load 120(texColor)
            1396:    8(fvec3) VectorTimesScalar 1395 1394
                              Store 120(texColor) 1396
                              Line 1 541 0
            1398:     21(ptr) AccessChain 119(pos) 152
            1399:    7(float) Load 1398
            1400:    7(float) FMul 1399 1361
            1401:    7(float) FSub 1400 645
            1403:    7(float) FMul 1401 1402
            1404:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 1403
            1406:    7(float) FMul 1404 1405
            1407:    7(float) FSub 1406 460
            1408:    7(float) ExtInst 2(GLSL.std.450) 8(Floor) 1407
            1409:    7(float) FAdd 1408 297
                              Store 1397(wave) 1409
                              Line 1 542 0
            1410:    7(float) Load 1397(wave)
            1411:    7(float) FNegate 1410
            1412:    7(float) FMul 1411 460
            1413:    7(float) ExtInst 2(GLSL.std.450) 37(FMin) 297 1412
            1414:    7(float) ExtInst 2(GLSL.std.450) 40(FMax) 1196 1413
            1415:     21(ptr) AccessChain 122(normal) 152
            1416:    7(float) Load 1415
            1417:    7(float) FSub 1416 1414
            1418:     21(ptr) AccessChain 122(normal) 152
                              Store 1418 1417
                              Line 1 544 0
            1420:     21(ptr) AccessChain 119(pos) 258
            1421:    7(float) Load 1420
            1423:    7(float) FMul 1421 1422
            1424:    7(float) FMul 1423 1402
            1425:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 1424
            1426:    7(float) ExtInst 2(GLSL.std.450) 4(FAbs) 1425
            1427:    7(float) FMul 1426 532
            1428:    7(float) ExtInst 2(GLSL.std.450) 37(FMin) 297 1427
            1429:    7(float) FSub 1428 297
                              Store 1419(pits) 1429
                              Line 1 545 0
            1430:    7(float) Load 1419(pits)
            1431:    7(float) FMul 1430 536
            1432:     21(ptr) AccessChain 122(normal) 258
            1433:    7(float) Load 1432
            1434:    7(float) FAdd 1433 1431
            1435:     21(ptr) AccessChain 122(normal) 258
                              Store 1435 1434
                              Line 1 546 0
            1436:     21(ptr) AccessChain 119(pos) 169
            1437:    7(float) Load 1436
            1438:    7(float) FMul 1437 1422
            1439:    7(float) FMul 1438 1402
            1440:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 1439
            1441:    7(float) ExtInst 2(GLSL.std.450) 4(FAbs) 1440
            1442:    7(float) FMul 1441 532
            1443:    7(float) ExtInst 2(GLSL.std.450) 37(FMin) 297 1442
            1444:    7(float) FSub 1443 297
                              Store 1419(pits) 1444
                              Line 1 547 0
            1445:    7(float) Load 1419(pits)
            1446:    7(float) FMul 1445 536
            1447:     21(ptr) AccessChain 122(normal) 169
            1448:    7(float) Load 1447
            1449:    7(float) FAdd 1448 1446
            1450:     21(ptr) AccessChain 122(normal) 169
                              Store 1450 1449
                              Return
                              FunctionEnd
128(RayTrace(vf2;struct-Ray-vf3-vf3-vf31;):    8(fvec3) Function None 125
  126(fragCoord):    9(fvec2) FunctionParameter
        127(ray):     10(Ray) FunctionParameter
             129:             Label)#"_str
+
R"#(
    1467(camPos):     38(ptr) Variable Function
     1469(camUp):     38(ptr) Variable Function
 1470(camLookat):     38(ptr) Variable Function
        1471(uv):     26(ptr) Variable Function
      1492(time):     21(ptr) Variable Function
     1495(alpha):     21(ptr) Variable Function
     1498(param):     21(ptr) Variable Function
     1503(param):     21(ptr) Variable Function
      1526(time):     21(ptr) Variable Function
     1529(alpha):     21(ptr) Variable Function
     1533(param):     21(ptr) Variable Function
     1538(param):     21(ptr) Variable Function
      1577(time):     21(ptr) Variable Function
     1580(alpha):     21(ptr) Variable Function
     1583(param):     21(ptr) Variable Function
     1588(param):     21(ptr) Variable Function
      1626(time):     21(ptr) Variable Function
     1629(alpha):     21(ptr) Variable Function
     1632(param):     21(ptr) Variable Function
     1637(param):     21(ptr) Variable Function
      1670(time):     21(ptr) Variable Function
     1673(alpha):     21(ptr) Variable Function
     1677(param):     21(ptr) Variable Function
     1682(param):     21(ptr) Variable Function
    1726(camVec):     38(ptr) Variable Function
  1731(sideNorm):     38(ptr) Variable Function
    1736(upNorm):     38(ptr) Variable Function
1740(worldFacing):     38(ptr) Variable Function
  1744(worldPix):     38(ptr) Variable Function
    1762(rayVec):     38(ptr) Variable Function
1764(distAndMat):     26(ptr) Variable Function
         1766(t):     21(ptr) Variable Function
       1767(pos):     38(ptr) Variable Function
         1769(i):   1768(ptr) Variable Function
     1785(param):     38(ptr) Variable Function
      1788(walk):     21(ptr) Variable Function
        1791(dx):     21(ptr) Variable Function
        1805(dz):     21(ptr) Variable Function
1819(nearestVoxel):     21(ptr) Variable Function
     1860(alpha):     21(ptr) Variable Function
1890(finalColor):     38(ptr) Variable Function
      1899(dist):     21(ptr) Variable Function
  1902(smallVec):     38(ptr) Variable Function
   1904(normalU):     38(ptr) Variable Function
     1910(param):     38(ptr) Variable Function
     1919(param):     38(ptr) Variable Function
     1928(param):     38(ptr) Variable Function
    1933(normal):     38(ptr) Variable Function
  1936(ambientS):     21(ptr) Variable Function
     1941(param):     38(ptr) Variable Function
     1945(param):     21(ptr) Variable Function
     1953(param):     38(ptr) Variable Function
     1957(param):     21(ptr) Variable Function
     1965(param):     38(ptr) Variable Function
     1969(param):     21(ptr) Variable Function
     1977(param):     38(ptr) Variable Function
     1981(param):     21(ptr) Variable Function
     1989(param):     38(ptr) Variable Function
     1993(param):     21(ptr) Variable Function
     2001(param):     38(ptr) Variable Function
     2005(param):     21(ptr) Variable Function
   2009(ambient):     21(ptr) Variable Function
     2014(param):     21(ptr) Variable Function
       2017(ref):     38(ptr) Variable Function
 2021(sunShadow):     21(ptr) Variable Function
      2022(iter):     21(ptr) Variable Function
  2023(nudgePos):     38(ptr) Variable Function
         2029(i):   1768(ptr) Variable Function
 2038(shadowPos):     38(ptr) Variable Function
  2044(tempDist):     21(ptr) Variable Function
     2045(param):     38(ptr) Variable Function
     2051(param):     21(ptr) Variable Function
      2060(walk):     21(ptr) Variable Function
        2062(dx):     21(ptr) Variable Function
        2076(dz):     21(ptr) Variable Function
2090(nearestVoxel):     21(ptr) Variable Function
     2120(param):     21(ptr) Variable Function
         2123(n):     21(ptr) Variable Function
     2126(param):     38(ptr) Variable Function
     2133(param):     38(ptr) Variable Function
     2140(param):     38(ptr) Variable Function
     2146(param):     38(ptr) Variable Function
     2153(param):     38(ptr) Variable Function
     2159(block):     26(ptr) Variable Function
  2163(texColor):     38(ptr) Variable Function
 2167(windowRef):     21(ptr) Variable Function
     2179(posdx):     38(ptr) Variable Function
     2182(posdy):     38(ptr) Variable Function
   2185(posGrad):     38(ptr) Variable Function
     2187(param):     26(ptr) Variable Function
     2195(param):     26(ptr) Variable Function
  2199(colTotal):     38(ptr) Variable Function
   2200(colTemp):     38(ptr) Variable Function
     2202(nTemp):     38(ptr) Variable Function
     2203(param):     26(ptr) Variable Function
     2205(param):     38(ptr) Variable Function
     2207(param):     38(ptr) Variable Function
     2209(param):     21(ptr) Variable Function
     2211(param):     38(ptr) Variable Function
     2224(param):     26(ptr) Variable Function
     2226(param):     38(ptr) Variable Function
     2227(param):     38(ptr) Variable Function
     2229(param):     21(ptr) Variable Function
     2231(param):     38(ptr) Variable Function
     2248(param):     26(ptr) Variable Function
     2250(param):     38(ptr) Variable Function
     2251(param):     38(ptr) Variable Function
     2253(param):     21(ptr) Variable Function
     2255(param):     38(ptr) Variable Function
     2269(param):     26(ptr) Variable Function
     2271(param):     38(ptr) Variable Function
     2272(param):     38(ptr) Variable Function
     2274(param):     21(ptr) Variable Function
     2276(param):     38(ptr) Variable Function
     2294(param):     26(ptr) Variable Function
     2296(param):     38(ptr) Variable Function
     2297(param):     38(ptr) Variable Function
     2299(param):     21(ptr) Variable Function
     2301(param):     38(ptr) Variable Function
     2320(xroad):     21(ptr) Variable Function
     2327(zroad):     21(ptr) Variable Function
      2334(road):     21(ptr) Variable Function
     2342(param):     21(ptr) Variable Function
     2350(param):     26(ptr) Variable Function
2362(yellowLine):     21(ptr) Variable Function
     2369(param):     21(ptr) Variable Function
     2376(param):     21(ptr) Variable Function
     2390(param):     21(ptr) Variable Function
 2399(whiteLine):     21(ptr) Variable Function
     2407(param):     21(ptr) Variable Function
     2415(param):     21(ptr) Variable Function
     2428(param):     21(ptr) Variable Function
     2439(param):     21(ptr) Variable Function
     2449(param):     21(ptr) Variable Function
     2465(param):     21(ptr) Variable Function
     2473(param):     21(ptr) Variable Function
     2486(param):     21(ptr) Variable Function
 2494(crossWalk):     21(ptr) Variable Function
     2501(param):     21(ptr) Variable Function
     2506(param):     21(ptr) Variable Function
     2515(param):     21(ptr) Variable Function
     2538(param):     21(ptr) Variable Function
     2543(param):     21(ptr) Variable Function
     2551(param):     21(ptr) Variable Function
  2567(sidewalk):     21(ptr) Variable Function
 2568(blockSize):     26(ptr) Variable Function
     2589(param):     21(ptr) Variable Function
     2604(param):     21(ptr) Variable Function
     2610(param):     21(ptr) Variable Function
     2616(param):     21(ptr) Variable Function
     2634(param):     38(ptr) Variable Function
2637(windowMask):     21(ptr) Variable Function
     2643(param):     21(ptr) Variable Function
     2652(param):     21(ptr) Variable Function
     2658(param):     21(ptr) Variable Function
     2678(param):     21(ptr) Variable Function
     2692(param):     21(ptr) Variable Function
   2694(dirNorm):     26(ptr) Variable Function
   2699(pillars):     21(ptr) Variable Function
     2706(param):     21(ptr) Variable Function
2718(lightColor):     38(ptr) Variable Function
     2725(param):     21(ptr) Variable Function
2730(ambientAvg):     21(ptr) Variable Function
     2741(param):     21(ptr) Variable Function
     2760(yfade):     21(ptr) Variable Function
     2775(param):     21(ptr) Variable Function
     2782(param):     38(ptr) Variable Function
     2784(param):     38(ptr) Variable Function
     2786(param):     21(ptr) Variable Function
     2804(yfade):     21(ptr) Variable Function
     2813(param):     38(ptr) Variable Function
     2815(param):     38(ptr) Variable Function
     2817(param):     21(ptr) Variable Function
       2831(rv2):     38(ptr) Variable Function
     2836(param):     21(ptr) Variable Function
  2842(fogColor):     38(ptr) Variable Function
     2843(param):     38(ptr) Variable Function
     2845(param):     38(ptr) Variable Function
     2861(param):     38(ptr) Variable Function
     2863(param):     38(ptr) Variable Function
     2871(param):     21(ptr) Variable Function
     2886(param):     21(ptr) Variable Function
                              Line 1 553 0
                              Store 1451(marchCount) 298
                              Line 1 554 0
                              Store 481(sunCol) 1455
                              Line 1 555 0
                              Store 1456(sunDir) 1459
                              Line 1 556 0
                              Store 490(horizonCol) 1463
                              Line 1 557 0
                              Store 492(skyCol) 1464
                              Line 1 558 0
                              Store 1465(exposure) 297
                              Line 1 559 0
                              Store 1466(fade) 297
                              Line 1 561 0
                              Store 1467(camPos) 1468
                              Line 1 563 0
                              Store 1469(camUp) 1468
                              Line 1 565 0
                              Store 1470(camLookat) 1468
                              Line 1 567 0
            1473:   1472(ptr) AccessChain 150 151
            1474:  130(fvec4) Load 1473
            1475:    9(fvec2) VectorShuffle 1474 1474 0 1
            1476:    9(fvec2) FDiv 126(fragCoord) 1475
            1477:    9(fvec2) VectorTimesScalar 1476 277
            1478:    9(fvec2) CompositeConstruct 297 297
            1479:    9(fvec2) FSub 1477 1478
                              Store 1471(uv) 1479
                              Line 1 568 0
            1480:    9(fvec2) Load 1471(uv)
            1481:    9(fvec2) CompositeConstruct 277 277
            1482:    9(fvec2) FDiv 1480 1481
                              Store 1471(uv) 1482
                              Line 1 569 0
            1483:    7(float) Load 1177(localTime)
            1485:    7(float) FDiv 1483 1484
            1486:    7(float) ExtInst 2(GLSL.std.450) 10(Fract) 1485
            1487:    7(float) FMul 1486 1484
                              Store 1177(localTime) 1487
                              Line 1 570 0
            1488:    7(float) Load 1177(localTime)
            1489:   704(bool) FOrdLessThan 1488 546
                              SelectionMerge 1491 None
                              BranchConditional 1489 1490 1520
            1490:               Label
                                Line 1 573 0
            1493:    7(float)   Load 1177(localTime)
            1494:    7(float)   FSub 1493 298
                                Store 1492(time) 1494
                                Line 1 575 0
            1496:    7(float)   Load 1492(time)
            1497:    7(float)   FDiv 1496 546
                                Store 1495(alpha) 1497
                                Line 1 576 0
            1499:    7(float)   Load 1492(time)
                                Store 1498(param) 1499
            1500:    7(float)   FunctionCall 60(saturate(f1;) 1498(param)
                                Store 1466(fade) 1500
                                Line 1 577 0
            1501:    7(float)   Load 1177(localTime)
            1502:    7(float)   FSub 546 1501
                                Store 1503(param) 1502
            1504:    7(float)   FunctionCall 60(saturate(f1;) 1503(param)
            1505:    7(float)   Load 1466(fade)
            1506:    7(float)   FMul 1505 1504
                                Store 1466(fade) 1506
                                Line 1 578 0
                                Store 1467(camPos) 1510
                                Line 1 579 0
            1511:    7(float)   Load 1495(alpha)
            1512:    7(float)   ExtInst 2(GLSL.std.450) 49(SmoothStep) 298 297 1511
            1514:    7(float)   FMul 1512 1513
            1515:     21(ptr)   AccessChain 1467(camPos) 169
            1516:    7(float)   Load 1515
            1517:    7(float)   FSub 1516 1514
            1518:     21(ptr)   AccessChain 1467(camPos) 169
                                Store 1518 1517
                                Line 1 580 0
                                Store 1469(camUp) 357
                                Line 1 581 0
                                Store 1470(camLookat) 1519
                                Branch 1491
            1520:               Label
                                Line 1 586 0
            1521:    7(float)   Load 1177(localTime)
            1523:   704(bool)   FOrdLessThan 1521 1522
                                SelectionMerge 1525 None
                                BranchConditional 1523 1524 1572
            1524:                 Label
                                  Line 1 589 0
            1527:    7(float)     Load 1177(localTime)
            1528:    7(float)     FSub 1527 546
                                  Store 1526(time) 1528
                                  Line 1 591 0
            1530:    7(float)     Load 1526(time)
            1532:    7(float)     FDiv 1530 1531
                                  Store 1529(alpha) 1532
                                  Line 1 592 0
            1534:    7(float)     Load 1526(time)
                                  Store 1533(param) 1534
            1535:    7(float)     FunctionCall 60(saturate(f1;) 1533(param)
                                  Store 1466(fade) 1535
                                  Line 1 593 0
            1536:    7(float)     Load 1177(localTime)
            1537:    7(float)     FSub 1522 1536
                                  Store 1538(param) 1537
            1539:    7(float)     FunctionCall 60(saturate(f1;) 1538(param)
            1540:    7(float)     Load 1466(fade)
            1541:    7(float)     FMul 1540 1539
                                  Store 1466(fade) 1541
                                  Line 1 594 0
            1543:    7(float)     Load 1529(alpha)
            1544:    7(float)     ExtInst 2(GLSL.std.450) 49(SmoothStep) 298 297 1543
            1545:    7(float)     FMul 1544 941
            1546:    7(float)     FAdd 465 1545
            1547:    8(fvec3)     CompositeConstruct 1542 1546 277
                                  Store 1467(camPos) 1547
                                  Line 1 595 0
            1548:    7(float)     Load 1529(alpha)
            1550:    7(float)     FMul 1548 1549
            1551:     21(ptr)     AccessChain 1467(camPos) 258
            1552:    7(float)     Load 1551
            1553:    7(float)     FSub 1552 1550
            1554:     21(ptr)     AccessChain 1467(camPos) 258
                                  Store 1554 1553
                                  Line 1 596 0
                                  Store 1469(camUp) 357
                                  Line 1 597 0
            1555:     21(ptr)     AccessChain 1467(camPos) 169
            1556:    7(float)     Load 1555
            1557:    7(float)     FSub 1556 1050
            1560:    8(fvec3)     CompositeConstruct 1557 1558 1559
                                  Store 1470(camLookat) 1560
                                  Line 1 598 0
                                  Store 1456(sunDir) 1564
                                  Line 1 599 0
                                  Store 481(sunCol) 1566
                                  Line 1 600 0
            1568:    7(float)     Load 1465(exposure))#"_str
+
R"#(
            1569:    7(float)     FMul 1568 1567
                                  Store 1465(exposure) 1569
                                  Line 1 601 0
            1570:    8(fvec3)     Load 492(skyCol)
            1571:    8(fvec3)     VectorTimesScalar 1570 887
                                  Store 492(skyCol) 1571
                                  Branch 1525
            1572:                 Label
                                  Line 1 606 0
            1573:    7(float)     Load 1177(localTime)
            1574:   704(bool)     FOrdLessThan 1573 659
                                  SelectionMerge 1576 None
                                  BranchConditional 1574 1575 1620
            1575:                   Label
                                    Line 1 609 0
            1578:    7(float)       Load 1177(localTime)
            1579:    7(float)       FSub 1578 1522
                                    Store 1577(time) 1579
                                    Line 1 611 0
            1581:    7(float)       Load 1577(time)
            1582:    7(float)       FDiv 1581 570
                                    Store 1580(alpha) 1582
                                    Line 1 612 0
            1584:    7(float)       Load 1577(time)
                                    Store 1583(param) 1584
            1585:    7(float)       FunctionCall 60(saturate(f1;) 1583(param)
                                    Store 1466(fade) 1585
                                    Line 1 613 0
            1586:    7(float)       Load 1177(localTime)
            1587:    7(float)       FSub 659 1586
                                    Store 1588(param) 1587
            1589:    7(float)       FunctionCall 60(saturate(f1;) 1588(param)
            1590:    7(float)       Load 1466(fade)
            1591:    7(float)       FMul 1590 1589
                                    Store 1466(fade) 1591
                                    Line 1 614 0
                                    Store 1467(camPos) 1595
                                    Line 1 615 0
            1596:    7(float)       Load 1580(alpha)
            1598:    7(float)       FMul 1596 1597
            1599:     21(ptr)       AccessChain 1467(camPos) 152
            1600:    7(float)       Load 1599
            1601:    7(float)       FSub 1600 1598
            1602:     21(ptr)       AccessChain 1467(camPos) 152
                                    Store 1602 1601
                                    Line 1 616 0
            1603:    7(float)       Load 1580(alpha)
            1604:    7(float)       FMul 1603 297
            1605:    7(float)       ExtInst 2(GLSL.std.450) 14(Cos) 1604
            1607:    7(float)       FMul 1605 1606
            1608:     21(ptr)       AccessChain 1467(camPos) 169
                                    Store 1608 1607
                                    Line 1 617 0
            1609:    7(float)       Load 1580(alpha)
            1610:    7(float)       FMul 1609 297
            1611:    7(float)       ExtInst 2(GLSL.std.450) 13(Sin) 1610
            1612:    7(float)       FMul 1611 1606
            1613:     21(ptr)       AccessChain 1467(camPos) 258
                                    Store 1613 1612
                                    Line 1 618 0
            1614:    7(float)       Load 1580(alpha)
            1615:    7(float)       FMul 1614 460
            1616:    7(float)       FAdd 1594 1615
            1617:    8(fvec3)       CompositeConstruct 298 297 1616
            1618:    8(fvec3)       ExtInst 2(GLSL.std.450) 69(Normalize) 1617
                                    Store 1469(camUp) 1618
                                    Line 1 619 0
                                    Store 1470(camLookat) 1619
                                    Branch 1576
            1620:                   Label
                                    Line 1 624 0
            1621:    7(float)       Load 1177(localTime)
            1623:   704(bool)       FOrdLessThan 1621 1622
                                    SelectionMerge 1625 None
                                    BranchConditional 1623 1624 1664
            1624:                     Label
                                      Line 1 627 0
            1627:    7(float)         Load 1177(localTime)
            1628:    7(float)         FSub 1627 659
                                      Store 1626(time) 1628
                                      Line 1 629 0
            1630:    7(float)         Load 1626(time)
            1631:    7(float)         FDiv 1630 1522
                                      Store 1629(alpha) 1631
                                      Line 1 630 0
            1633:    7(float)         Load 1626(time)
                                      Store 1632(param) 1633
            1634:    7(float)         FunctionCall 60(saturate(f1;) 1632(param)
                                      Store 1466(fade) 1634
                                      Line 1 631 0
            1635:    7(float)         Load 1177(localTime)
            1636:    7(float)         FSub 1622 1635
                                      Store 1637(param) 1636
            1638:    7(float)         FunctionCall 60(saturate(f1;) 1637(param)
            1639:    7(float)         Load 1466(fade)
            1640:    7(float)         FMul 1639 1638
                                      Store 1466(fade) 1640
                                      Line 1 632 0
            1642:    7(float)         Load 1629(alpha)
            1643:    7(float)         FMul 1642 460
            1644:    7(float)         FSub 1641 1643
            1645:    7(float)         Load 1629(alpha)
            1646:    7(float)         FMul 1645 558
            1647:    7(float)         FSub 1196 1646
            1648:    8(fvec3)         CompositeConstruct 1644 811 1647
                                      Store 1467(camPos) 1648
                                      Line 1 633 0
            1649:    7(float)         Load 1629(alpha)
            1650:    7(float)         Load 1629(alpha)
            1651:    7(float)         FMul 1649 1650
            1652:    7(float)         ExtInst 2(GLSL.std.450) 49(SmoothStep) 298 297 1651
            1654:    7(float)         FMul 1652 1653
            1655:     21(ptr)         AccessChain 1467(camPos) 152
            1656:    7(float)         Load 1655
            1657:    7(float)         FAdd 1656 1654
            1658:     21(ptr)         AccessChain 1467(camPos) 152
                                      Store 1658 1657
                                      Line 1 634 0
                                      Store 1469(camUp) 357
                                      Line 1 635 0
            1659:    7(float)         Load 1629(alpha)
            1660:    7(float)         FAdd 460 1659
            1661:    7(float)         Load 1629(alpha)
            1662:    7(float)         FMul 1661 578
            1663:    8(fvec3)         CompositeConstruct 298 1660 1662
                                      Store 1470(camLookat) 1663
                                      Branch 1625
            1664:                     Label
                                      Line 1 640 0
            1665:    7(float)         Load 1177(localTime)
            1667:   704(bool)         FOrdLessThan 1665 1666
                                      SelectionMerge 1669 None
                                      BranchConditional 1667 1668 1718
            1668:                       Label
                                        Line 1 643 0
            1671:    7(float)           Load 1177(localTime)
            1672:    7(float)           FSub 1671 1622
                                        Store 1670(time) 1672
                                        Line 1 645 0
            1674:    7(float)           Load 1670(time)
            1676:    7(float)           FDiv 1674 1675
                                        Store 1673(alpha) 1676
                                        Line 1 646 0
            1678:    7(float)           Load 1670(time)
                                        Store 1677(param) 1678
            1679:    7(float)           FunctionCall 60(saturate(f1;) 1677(param)
                                        Store 1466(fade) 1679
                                        Line 1 647 0
            1680:    7(float)           Load 1177(localTime)
            1681:    7(float)           FSub 1666 1680
                                        Store 1682(param) 1681
            1683:    7(float)           FunctionCall 60(saturate(f1;) 1682(param)
            1684:    7(float)           Load 1466(fade)
            1685:    7(float)           FMul 1684 1683
                                        Store 1466(fade) 1685
                                        Line 1 648 0
            1687:    7(float)           Load 1673(alpha)
            1689:    7(float)           FMul 1687 1688
            1690:    7(float)           FSub 1351 1689
            1692:    7(float)           Load 1673(alpha)
            1693:    7(float)           FMul 1692 460
            1694:    7(float)           FSub 1691 1693
            1695:    8(fvec3)           CompositeConstruct 1686 1690 1694
                                        Store 1467(camPos) 1695
                                        Line 1 649 0
                                        Store 1469(camUp) 357
                                        Line 1 650 0
            1696:    7(float)           Load 1673(alpha)
            1697:    7(float)           FAdd 1050 1696
            1699:    8(fvec3)           CompositeConstruct 1686 1697 1698
                                        Store 1470(camLookat) 1699
                                        Line 1 651 0
            1700:    7(float)           Load 1673(alpha)
            1701:    7(float)           FMul 1700 627
            1702:    7(float)           FSub 460 1701
            1703:    7(float)           Load 1673(alpha)
            1704:    7(float)           FMul 1703 1050
            1705:    7(float)           FSub 1050 1704
            1706:    8(fvec3)           CompositeConstruct 1702 1705 297
            1707:    8(fvec3)           ExtInst 2(GLSL.std.450) 69(Normalize) 1706
                                        Store 1456(sunDir) 1707
                                        Line 1 652 0
                                        Store 481(sunCol) 1710
                                        Line 1 653 0
            1712:    7(float)           Load 1177(localTime)
            1713:    7(float)           FMul 1712 1711
                                        Store 1177(localTime) 1713
                                        Line 1 654 0
            1714:    7(float)           Load 1465(exposure)
            1715:    7(float)           FMul 1714 941
                                        Store 1465(exposure) 1715
                                        Line 1 655 0
                                        Store 490(horizonCol) 1716
                                        Line 1 656 0
                                        Store 492(skyCol) 1717
                                        Branch 1669
            1718:                       Label
                                        Line 1 661 0
            1719:    7(float)           Load 1177(localTime)
            1720:   704(bool)           FOrdLessThan 1719 1484
                                        SelectionMerge 1722 None
                                        BranchConditional 1720 1721 1722
            1721:                         Label
                                          Line 1 663 0
                                          Store 1466(fade) 298
                                          Line 1 664 0
                                          Store 1467(camPos) 1724
                                          Line 1 665 0
                                          Store 1469(camUp) 357
                                          Line 1 666 0
                                          Store 1470(camLookat) 1725
                                          Branch 1722
            1722:                       Label
                                        Branch 1669
            1669:                     Label
                                      Branch 1625
            1625:                   Label
                                    Branch 1576
            1576:                 Label
                                  Branch 1525
            1525:               Label
                                Branch 1491
            1491:             Label
                              Line 1 681 0
            1727:    8(fvec3) Load 1470(camLookat)
            1728:    8(fvec3) Load 1467(camPos)
            1729:    8(fvec3) FSub 1727 1728
            1730:    8(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 1729
                              Store 1726(camVec) 1730
                              Line 1 683 0
            1732:    8(fvec3) Load 1469(camUp)
            1733:    8(fvec3) Load 1726(camVec)
            1734:    8(fvec3) ExtInst 2(GLSL.std.450) 68(Cross) 1732 1733
            1735:    8(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 1734
                              Store 1731(sideNorm) 1735
                              Line 1 685 0
            1737:    8(fvec3) Load 1726(camVec)
            1738:    8(fvec3) Load 1731(sideNorm)
            1739:    8(fvec3) ExtInst 2(GLSL.std.450) 68(Cross) 1737 1738
                              Store 1736(upNorm) 1739
                              Line 1 687 0
            1741:    8(fvec3) Load 1467(camPos)
            1742:    8(fvec3) Load 1726(camVec)
            1743:    8(fvec3) FAdd 1741 1742
                              Store 1740(worldFacing) 1743
                              Line 1 689 0
            1745:    8(fvec3) Load 1740(worldFacing)
            1746:     21(ptr) AccessChain 1471(uv) 169
            1747:    7(float) Load 1746
            1748:    8(fvec3) Load 1731(sideNorm)
            1749:    8(fvec3) VectorTimesScalar 1748 1747
            1750:    153(ptr) AccessChain 150 151 169
            1751:    7(float) Load 1750
            1752:    153(ptr) AccessChain 150 151 152
            1753:    7(float) Load 1752
            1754:    7(float) FDiv 1751 1753
            1755:    8(fvec3) VectorTimesScalar 1749 1754
            1756:    8(fvec3) FAdd 1745 1755
            1757:     21(ptr) AccessChain 1471(uv) 152
            1758:    7(float) Load 1757
            1759:    8(fvec3) Load 1736(upNorm)
            1760:    8(fvec3) VectorTimesScalar 1759 1758
            1761:    8(fvec3) FAdd 1756 1760
                              Store 1744(worldPix) 1761
                              Line 1 691 0
            1763:    8(fvec3) CompositeExtract 127(ray) 2
                              Store 1762(rayVec) 1763
                              Line 1 693 0
                              Store 1764(distAndMat) 1765
                              Line 1 695 0
                              Store 1766(t) 465
                              Line 1 697 0
                              Store 1767(pos) 1468
                              Line 1 700 0
                              Store 1769(i) 151
                              Branch 1770
                              Line 1 701 0
            1770:             Label
                              LoopMerge 1772 1773 None
                              Branch 1774
            1774:             Label
            1775:    142(int) Load 1769(i)
            1777:   704(bool) SLessThan 1775 1776
                              BranchConditional 1777 1771 1772
            1771:               Label
                                Line 1 703 0
            1778:    7(float)   Load 1451(marchCount)
            1779:    7(float)   FAdd 1778 297
                                Store 1451(marchCount) 1779
                                Line 1 704 0)#"_str
+
R"#(
            1780:    8(fvec3)   Load 1467(camPos)
            1781:    8(fvec3)   Load 1762(rayVec)
            1782:    7(float)   Load 1766(t)
            1783:    8(fvec3)   VectorTimesScalar 1781 1782
            1784:    8(fvec3)   FAdd 1780 1783
                                Store 1767(pos) 1784
                                Line 1 705 0
            1786:    8(fvec3)   Load 1767(pos)
                                Store 1785(param) 1786
            1787:    9(fvec2)   FunctionCall 115(DistanceToObject(vf3;) 1785(param)
                                Store 1764(distAndMat) 1787
                                Line 1 707 0
            1789:     21(ptr)   AccessChain 1764(distAndMat) 169
            1790:    7(float)   Load 1789
                                Store 1788(walk) 1790
                                Line 1 709 0
            1792:     21(ptr)   AccessChain 1767(pos) 169
            1793:    7(float)   Load 1792
            1794:    7(float)   ExtInst 2(GLSL.std.450) 10(Fract) 1793
            1795:    7(float)   FNegate 1794
                                Store 1791(dx) 1795
                                Line 1 710 0
            1796:     21(ptr)   AccessChain 1762(rayVec) 169
            1797:    7(float)   Load 1796
            1798:   704(bool)   FOrdGreaterThan 1797 298
                                SelectionMerge 1800 None
                                BranchConditional 1798 1799 1800
            1799:                 Label
                                  Line 1 712 0
            1801:     21(ptr)     AccessChain 1767(pos) 169
            1802:    7(float)     Load 1801
            1803:    7(float)     FNegate 1802
            1804:    7(float)     ExtInst 2(GLSL.std.450) 10(Fract) 1803
                                  Store 1791(dx) 1804
                                  Branch 1800
            1800:               Label
                                Line 1 716 0
            1806:     21(ptr)   AccessChain 1767(pos) 258
            1807:    7(float)   Load 1806
            1808:    7(float)   ExtInst 2(GLSL.std.450) 10(Fract) 1807
            1809:    7(float)   FNegate 1808
                                Store 1805(dz) 1809
                                Line 1 717 0
            1810:     21(ptr)   AccessChain 1762(rayVec) 258
            1811:    7(float)   Load 1810
            1812:   704(bool)   FOrdGreaterThan 1811 298
                                SelectionMerge 1814 None
                                BranchConditional 1812 1813 1814
            1813:                 Label
                                  Line 1 719 0
            1815:     21(ptr)     AccessChain 1767(pos) 258
            1816:    7(float)     Load 1815
            1817:    7(float)     FNegate 1816
            1818:    7(float)     ExtInst 2(GLSL.std.450) 10(Fract) 1817
                                  Store 1805(dz) 1818
                                  Branch 1814
            1814:               Label
                                Line 1 723 0
            1820:    7(float)   Load 1791(dx)
            1821:     21(ptr)   AccessChain 1762(rayVec) 169
            1822:    7(float)   Load 1821
            1823:    7(float)   FDiv 1820 1822
            1824:    7(float)   ExtInst 2(GLSL.std.450) 10(Fract) 1823
            1825:    7(float)   Load 1805(dz)
            1826:     21(ptr)   AccessChain 1762(rayVec) 258
            1827:    7(float)   Load 1826
            1828:    7(float)   FDiv 1825 1827
            1829:    7(float)   ExtInst 2(GLSL.std.450) 10(Fract) 1828
            1830:    7(float)   ExtInst 2(GLSL.std.450) 37(FMin) 1824 1829
            1832:    7(float)   Load 1831(voxelPad)
            1833:    7(float)   FAdd 1830 1832
                                Store 1819(nearestVoxel) 1833
                                Line 1 724 0
            1834:    7(float)   Load 1831(voxelPad)
            1835:    7(float)   Load 1819(nearestVoxel)
            1836:    7(float)   ExtInst 2(GLSL.std.450) 40(FMax) 1834 1835
                                Store 1819(nearestVoxel) 1836
                                Line 1 725 0
            1837:    7(float)   Load 1788(walk)
            1838:    7(float)   Load 1819(nearestVoxel)
            1839:    7(float)   ExtInst 2(GLSL.std.450) 37(FMin) 1837 1838
                                Store 1788(walk) 1839
                                Line 1 726 0
            1840:    7(float)   Load 1788(walk)
            1841:    7(float)   Load 1766(t)
            1842:    7(float)   FAdd 1841 1840
                                Store 1766(t) 1842
                                Line 1 727 0
            1843:    7(float)   Load 1766(t)
            1845:   704(bool)   FOrdGreaterThan 1843 1844
            1846:   704(bool)   LogicalNot 1845
                                SelectionMerge 1848 None
                                BranchConditional 1846 1847 1848
            1847:                 Label
            1849:     21(ptr)     AccessChain 1764(distAndMat) 169
            1850:    7(float)     Load 1849
            1851:    7(float)     ExtInst 2(GLSL.std.450) 4(FAbs) 1850
            1853:   704(bool)     FOrdLessThan 1851 1852
                                  Branch 1848
            1848:               Label
            1854:   704(bool)   Phi 1845 1814 1853 1847
                                SelectionMerge 1856 None
                                BranchConditional 1854 1855 1856
            1855:                 Label
                                  Line 1 729 0
                                  Branch 1772
            1856:               Label
                                Branch 1773
            1773:               Label
                                Line 1 701 0
            1858:    142(int)   Load 1769(i)
            1859:    142(int)   IAdd 1858 189
                                Store 1769(i) 1859
                                Branch 1770
            1772:             Label
                              Line 1 736 0
            1861:     21(ptr) AccessChain 1467(camPos) 152
            1862:    7(float) Load 1861
            1863:    7(float) FNegate 1862
            1864:     21(ptr) AccessChain 1762(rayVec) 152
            1865:    7(float) Load 1864
            1866:    7(float) FDiv 1863 1865
                              Store 1860(alpha) 1866
                              Line 1 737 0
            1867:    7(float) Load 1766(t)
            1868:   704(bool) FOrdGreaterThan 1867 1844
                              SelectionMerge 1870 None
                              BranchConditional 1868 1869 1870
            1869:               Label
            1871:     21(ptr)   AccessChain 1762(rayVec) 152
            1872:    7(float)   Load 1871
            1873:   704(bool)   FOrdLessThan 1872 1698
                                Branch 1870
            1870:             Label
            1874:   704(bool) Phi 1868 1772 1873 1869
                              SelectionMerge 1876 None
                              BranchConditional 1874 1875 1876
            1875:               Label
                                Line 1 739 0
            1877:    8(fvec3)   Load 1467(camPos)
            1878:    9(fvec2)   VectorShuffle 1877 1877 0 2
            1879:    8(fvec3)   Load 1762(rayVec)
            1880:    9(fvec2)   VectorShuffle 1879 1879 0 2
            1881:    7(float)   Load 1860(alpha)
            1882:    9(fvec2)   VectorTimesScalar 1880 1881
            1883:    9(fvec2)   FAdd 1878 1882
            1884:    8(fvec3)   Load 1767(pos)
            1885:    8(fvec3)   VectorShuffle 1884 1883 3 1 4
                                Store 1767(pos) 1885
                                Line 1 740 0
            1886:     21(ptr)   AccessChain 1767(pos) 152
                                Store 1886 1698
                                Line 1 741 0
            1887:    7(float)   Load 1860(alpha)
                                Store 1766(t) 1887
                                Line 1 742 0
            1888:     21(ptr)   AccessChain 1764(distAndMat) 152
                                Store 1888 298
                                Line 1 743 0
            1889:     21(ptr)   AccessChain 1764(distAndMat) 169
                                Store 1889 298
                                Branch 1876
            1876:             Label
                              Line 1 748 0
                              Store 1890(finalColor) 1468
                              Line 1 749 0
            1891:    7(float) Load 1766(t)
            1892:   704(bool) FOrdLessThanEqual 1891 1844
            1893:    7(float) Load 1766(t)
            1894:    7(float) Load 1860(alpha)
            1895:   704(bool) FOrdEqual 1893 1894
            1896:   704(bool) LogicalOr 1892 1895
                              SelectionMerge 1898 None
                              BranchConditional 1896 1897 2860
            1897:               Label
                                Line 1 752 0
            1900:     21(ptr)   AccessChain 1764(distAndMat) 169
            1901:    7(float)   Load 1900
                                Store 1899(dist) 1901
                                Line 1 754 0
                                Store 1902(smallVec) 1903
                                Line 1 756 0
            1905:    7(float)   Load 1899(dist)
            1906:    8(fvec3)   Load 1767(pos)
            1907:    8(fvec3)   Load 1902(smallVec)
            1908:    8(fvec3)   VectorShuffle 1907 1907 0 1 1
            1909:    8(fvec3)   FSub 1906 1908
                                Store 1910(param) 1909
            1911:    9(fvec2)   FunctionCall 115(DistanceToObject(vf3;) 1910(param)
            1912:    7(float)   CompositeExtract 1911 0
            1913:    7(float)   FSub 1905 1912
            1914:    7(float)   Load 1899(dist)
            1915:    8(fvec3)   Load 1767(pos)
            1916:    8(fvec3)   Load 1902(smallVec)
            1917:    8(fvec3)   VectorShuffle 1916 1916 1 0 1
            1918:    8(fvec3)   FSub 1915 1917
                                Store 1919(param) 1918
            1920:    9(fvec2)   FunctionCall 115(DistanceToObject(vf3;) 1919(param)
            1921:    7(float)   CompositeExtract 1920 0
            1922:    7(float)   FSub 1914 1921
            1923:    7(float)   Load 1899(dist)
            1924:    8(fvec3)   Load 1767(pos)
            1925:    8(fvec3)   Load 1902(smallVec)
            1926:    8(fvec3)   VectorShuffle 1925 1925 1 1 0
            1927:    8(fvec3)   FSub 1924 1926
                                Store 1928(param) 1927
            1929:    9(fvec2)   FunctionCall 115(DistanceToObject(vf3;) 1928(param)
            1930:    7(float)   CompositeExtract 1929 0
            1931:    7(float)   FSub 1923 1930
            1932:    8(fvec3)   CompositeConstruct 1913 1922 1931
                                Store 1904(normalU) 1932
                                Line 1 758 0
            1934:    8(fvec3)   Load 1904(normalU)
            1935:    8(fvec3)   ExtInst 2(GLSL.std.450) 69(Normalize) 1934
                                Store 1933(normal) 1935
                                Line 1 760 0
                                Store 1936(ambientS) 297
                                Line 1 761 0
            1937:    8(fvec3)   Load 1767(pos)
            1938:    8(fvec3)   Load 1933(normal)
            1939:    8(fvec3)   VectorTimesScalar 1938 1002
            1940:    8(fvec3)   FAdd 1937 1939
                                Store 1941(param) 1940
            1942:    9(fvec2)   FunctionCall 115(DistanceToObject(vf3;) 1941(param)
            1943:    7(float)   CompositeExtract 1942 0
            1944:    7(float)   FMul 1943 1422
                                Store 1945(param) 1944
            1946:    7(float)   FunctionCall 60(saturate(f1;) 1945(param)
            1947:    7(float)   Load 1936(ambientS)
            1948:    7(float)   FMul 1947 1946
                                Store 1936(ambientS) 1948
                                Line 1 762 0
            1949:    8(fvec3)   Load 1767(pos)
            1950:    8(fvec3)   Load 1933(normal)
            1951:    8(fvec3)   VectorTimesScalar 1950 485
            1952:    8(fvec3)   FAdd 1949 1951
                                Store 1953(param) 1952
            1954:    9(fvec2)   FunctionCall 115(DistanceToObject(vf3;) 1953(param)
            1955:    7(float)   CompositeExtract 1954 0
            1956:    7(float)   FMul 1955 1361
                                Store 1957(param) 1956
            1958:    7(float)   FunctionCall 60(saturate(f1;) 1957(param)
            1959:    7(float)   Load 1936(ambientS)
            1960:    7(float)   FMul 1959 1958
                                Store 1936(ambientS) 1960
                                Line 1 763 0
            1961:    8(fvec3)   Load 1767(pos)
            1962:    8(fvec3)   Load 1933(normal)
            1963:    8(fvec3)   VectorTimesScalar 1962 465
            1964:    8(fvec3)   FAdd 1961 1963
                                Store 1965(param) 1964
            1966:    9(fvec2)   FunctionCall 115(DistanceToObject(vf3;) 1965(param)
            1967:    7(float)   CompositeExtract 1966 0
            1968:    7(float)   FMul 1967 861
                                Store 1969(param) 1968
            1970:    7(float)   FunctionCall 60(saturate(f1;) 1969(param)
            1971:    7(float)   Load 1936(ambientS)
            1972:    7(float)   FMul 1971 1970
                                Store 1936(ambientS) 1972
                                Line 1 764 0
            1973:    8(fvec3)   Load 1767(pos)
            1974:    8(fvec3)   Load 1933(normal)
            1975:    8(fvec3)   VectorTimesScalar 1974 645
            1976:    8(fvec3)   FAdd 1973 1975
                                Store 1977(param) 1976
            1978:    9(fvec2)   FunctionCall 115(DistanceToObject(vf3;) 1977(param)
            1979:    7(float)   CompositeExtract 1978 0
            1980:    7(float)   FMul 1979 570
                                Store 1981(param) 1980
            1982:    7(float)   FunctionCall 60(saturate(f1;) 1981(param)
            1983:    7(float)   Load 1936(ambientS)
            1984:    7(float)   FMul 1983 1982
                                Store 1936(ambientS) 1984
                                Line 1 765 0
            1985:    8(fvec3)   Load 1767(pos)
            1986:    8(fvec3)   Load 1933(normal)
            1987:    8(fvec3)   VectorTimesScalar 1986 558
            1988:    8(fvec3)   FAdd 1985 1987
                                Store 1989(param) 1988
            1990:    9(fvec2)   FunctionCall 115(DistanceToObject(vf3;) 1989(param)
            1991:    7(float)   CompositeExtract 1990 0
            1992:    7(float)   FMul 1991 578
                                Store 1993(param) 1992
            1994:    7(float)   FunctionCall 60(saturate(f1;) 1993(param)
            1995:    7(float)   Load 1936(ambientS)
            1996:    7(float)   FMul 1995 1994
                                Store 1936(ambientS) 1996
                                Line 1 766 0
            1997:    8(fvec3)   Load 1767(pos)
            1998:    8(fvec3)   Load 1933(normal)
            1999:    8(fvec3)   VectorTimesScalar 1998 941
            2000:    8(fvec3)   FAdd 1997 1999
                                Store 2001(param) 2000
            2002:    9(fvec2)   FunctionCall 115(DistanceToObject(vf3;) 2001(param)
            2003:    7(float)   CompositeExtract 2002 0
            2004:    7(float)   FMul 2003 655
                                Store 2005(param) 2004
            2006:    7(float)   FunctionCall 60(saturate(f1;) 2005(param)
            2007:    7(float)   Load 1936(ambientS)
            2008:    7(float)   FMul 2007 2006
                                Store 1936(ambientS) 2008
                                Line 1 768 0)#"_str
+
R"#(
            2010:    7(float)   Load 1936(ambientS)
                                Store 2009(ambient) 2010
                                Line 1 769 0
            2011:    7(float)   Load 2009(ambient)
            2012:    7(float)   ExtInst 2(GLSL.std.450) 26(Pow) 2011 460
            2013:    7(float)   ExtInst 2(GLSL.std.450) 40(FMax) 485 2012
                                Store 2009(ambient) 2013
                                Line 1 770 0
            2015:    7(float)   Load 2009(ambient)
                                Store 2014(param) 2015
            2016:    7(float)   FunctionCall 60(saturate(f1;) 2014(param)
                                Store 2009(ambient) 2016
                                Line 1 772 0
            2018:    8(fvec3)   Load 1762(rayVec)
            2019:    8(fvec3)   Load 1933(normal)
            2020:    8(fvec3)   ExtInst 2(GLSL.std.450) 71(Reflect) 2018 2019
                                Store 2017(ref) 2020
                                Line 1 774 0
                                Store 2021(sunShadow) 297
                                Line 1 776 0
                                Store 2022(iter) 779
                                Line 1 778 0
            2024:    8(fvec3)   Load 1767(pos)
            2025:    8(fvec3)   Load 1933(normal)
            2027:    8(fvec3)   VectorTimesScalar 2025 2026
            2028:    8(fvec3)   FAdd 2024 2027
                                Store 2023(nudgePos) 2028
                                Line 1 781 0
                                Store 2029(i) 151
                                Branch 2030
                                Line 1 782 0
            2030:               Label
                                LoopMerge 2032 2033 None
                                Branch 2034
            2034:               Label
            2035:    142(int)   Load 2029(i)
            2037:   704(bool)   SLessThan 2035 2036
                                BranchConditional 2037 2031 2032
            2031:                 Label
                                  Line 1 785 0
            2039:    8(fvec3)     Load 2023(nudgePos)
            2040:    8(fvec3)     Load 1456(sunDir)
            2041:    7(float)     Load 2022(iter)
            2042:    8(fvec3)     VectorTimesScalar 2040 2041
            2043:    8(fvec3)     FAdd 2039 2042
                                  Store 2038(shadowPos) 2043
                                  Line 1 787 0
            2046:    8(fvec3)     Load 2038(shadowPos)
                                  Store 2045(param) 2046
            2047:    9(fvec2)     FunctionCall 115(DistanceToObject(vf3;) 2045(param)
            2048:    7(float)     CompositeExtract 2047 0
                                  Store 2044(tempDist) 2048
                                  Line 1 788 0
            2049:    7(float)     Load 2044(tempDist)
            2050:    7(float)     FMul 2049 470
                                  Store 2051(param) 2050
            2052:    7(float)     FunctionCall 60(saturate(f1;) 2051(param)
            2053:    7(float)     Load 2021(sunShadow)
            2054:    7(float)     FMul 2053 2052
                                  Store 2021(sunShadow) 2054
                                  Line 1 789 0
            2055:    7(float)     Load 2044(tempDist)
            2056:   704(bool)     FOrdLessThanEqual 2055 298
                                  SelectionMerge 2058 None
                                  BranchConditional 2056 2057 2058
            2057:                   Label
                                    Line 1 791 0
                                    Branch 2032
            2058:                 Label
                                  Line 1 795 0
            2061:    7(float)     Load 2044(tempDist)
                                  Store 2060(walk) 2061
                                  Line 1 797 0
            2063:     21(ptr)     AccessChain 2038(shadowPos) 169
            2064:    7(float)     Load 2063
            2065:    7(float)     ExtInst 2(GLSL.std.450) 10(Fract) 2064
            2066:    7(float)     FNegate 2065
                                  Store 2062(dx) 2066
                                  Line 1 798 0
            2067:   1176(ptr)     AccessChain 1456(sunDir) 169
            2068:    7(float)     Load 2067
            2069:   704(bool)     FOrdGreaterThan 2068 298
                                  SelectionMerge 2071 None
                                  BranchConditional 2069 2070 2071
            2070:                   Label
                                    Line 1 800 0
            2072:     21(ptr)       AccessChain 2038(shadowPos) 169
            2073:    7(float)       Load 2072
            2074:    7(float)       FNegate 2073
            2075:    7(float)       ExtInst 2(GLSL.std.450) 10(Fract) 2074
                                    Store 2062(dx) 2075
                                    Branch 2071
            2071:                 Label
                                  Line 1 804 0
            2077:     21(ptr)     AccessChain 2038(shadowPos) 258
            2078:    7(float)     Load 2077
            2079:    7(float)     ExtInst 2(GLSL.std.450) 10(Fract) 2078
            2080:    7(float)     FNegate 2079
                                  Store 2076(dz) 2080
                                  Line 1 805 0
            2081:   1176(ptr)     AccessChain 1456(sunDir) 258
            2082:    7(float)     Load 2081
            2083:   704(bool)     FOrdGreaterThan 2082 298
                                  SelectionMerge 2085 None
                                  BranchConditional 2083 2084 2085
            2084:                   Label
                                    Line 1 807 0
            2086:     21(ptr)       AccessChain 2038(shadowPos) 258
            2087:    7(float)       Load 2086
            2088:    7(float)       FNegate 2087
            2089:    7(float)       ExtInst 2(GLSL.std.450) 10(Fract) 2088
                                    Store 2076(dz) 2089
                                    Branch 2085
            2085:                 Label
                                  Line 1 811 0
            2091:    7(float)     Load 2062(dx)
            2092:   1176(ptr)     AccessChain 1456(sunDir) 169
            2093:    7(float)     Load 2092
            2094:    7(float)     FDiv 2091 2093
            2095:    7(float)     ExtInst 2(GLSL.std.450) 10(Fract) 2094
            2096:    7(float)     Load 2076(dz)
            2097:   1176(ptr)     AccessChain 1456(sunDir) 258
            2098:    7(float)     Load 2097
            2099:    7(float)     FDiv 2096 2098
            2100:    7(float)     ExtInst 2(GLSL.std.450) 10(Fract) 2099
            2101:    7(float)     ExtInst 2(GLSL.std.450) 37(FMin) 2095 2100
            2102:    7(float)     FAdd 2101 1852
                                  Store 2090(nearestVoxel) 2102
                                  Line 1 812 0
            2103:    7(float)     Load 2090(nearestVoxel)
            2104:    7(float)     ExtInst 2(GLSL.std.450) 40(FMax) 558 2103
                                  Store 2090(nearestVoxel) 2104
                                  Line 1 813 0
            2105:    7(float)     Load 2060(walk)
            2106:    7(float)     Load 2090(nearestVoxel)
            2107:    7(float)     ExtInst 2(GLSL.std.450) 37(FMin) 2105 2106
                                  Store 2060(walk) 2107
                                  Line 1 814 0
            2108:    7(float)     Load 2060(walk)
            2109:    7(float)     ExtInst 2(GLSL.std.450) 40(FMax) 779 2108
            2110:    7(float)     Load 2022(iter)
            2111:    7(float)     FAdd 2110 2109
                                  Store 2022(iter) 2111
                                  Line 1 815 0
            2112:    7(float)     Load 2022(iter)
            2114:   704(bool)     FOrdGreaterThan 2112 2113
                                  SelectionMerge 2116 None
                                  BranchConditional 2114 2115 2116
            2115:                   Label
                                    Line 1 817 0
                                    Branch 2032
            2116:                 Label
                                  Branch 2033
            2033:                 Label
                                  Line 1 782 0
            2118:    142(int)     Load 2029(i)
            2119:    142(int)     IAdd 2118 189
                                  Store 2029(i) 2119
                                  Branch 2030
            2032:               Label
                                Line 1 823 0
            2121:    7(float)   Load 2021(sunShadow)
                                Store 2120(param) 2121
            2122:    7(float)   FunctionCall 60(saturate(f1;) 2120(param)
                                Store 2021(sunShadow) 2122
                                Line 1 825 0
                                Store 2123(n) 298
                                Line 1 826 0
            2124:    8(fvec3)   Load 1767(pos)
            2125:    8(fvec3)   VectorTimesScalar 2124 665
                                Store 2126(param) 2125
            2127:    7(float)   FunctionCall 53(noise(vf3;) 2126(param)
            2128:    7(float)   Load 2123(n)
            2129:    7(float)   FAdd 2128 2127
                                Store 2123(n) 2129
                                Line 1 827 0
            2130:    8(fvec3)   Load 1767(pos)
            2132:    8(fvec3)   VectorTimesScalar 2130 2131
                                Store 2133(param) 2132
            2134:    7(float)   FunctionCall 53(noise(vf3;) 2133(param)
            2135:    7(float)   Load 2123(n)
            2136:    7(float)   FAdd 2135 2134
                                Store 2123(n) 2136
                                Line 1 828 0
            2137:    8(fvec3)   Load 1767(pos)
            2139:    8(fvec3)   VectorTimesScalar 2137 2138
                                Store 2140(param) 2139
            2141:    7(float)   FunctionCall 53(noise(vf3;) 2140(param)
            2142:    7(float)   Load 2123(n)
            2143:    7(float)   FAdd 2142 2141
                                Store 2123(n) 2143
                                Line 1 829 0
            2144:    8(fvec3)   Load 1767(pos)
            2145:    8(fvec3)   VectorTimesScalar 2144 674
                                Store 2146(param) 2145
            2147:    7(float)   FunctionCall 53(noise(vf3;) 2146(param)
            2148:    7(float)   Load 2123(n)
            2149:    7(float)   FAdd 2148 2147
                                Store 2123(n) 2149
                                Line 1 830 0
            2150:    8(fvec3)   Load 1767(pos)
            2152:    8(fvec3)   VectorTimesScalar 2150 2151
                                Store 2153(param) 2152
            2154:    7(float)   FunctionCall 53(noise(vf3;) 2153(param)
            2155:    7(float)   Load 2123(n)
            2156:    7(float)   FAdd 2155 2154
                                Store 2123(n) 2156
                                Line 1 831 0
            2157:    7(float)   Load 2123(n)
            2158:    7(float)   ExtInst 2(GLSL.std.450) 46(FMix) 1567 502 2157
                                Store 2123(n) 2158
                                Line 1 833 0
            2160:    8(fvec3)   Load 1767(pos)
            2161:    9(fvec2)   VectorShuffle 2160 2160 0 2
            2162:    9(fvec2)   ExtInst 2(GLSL.std.450) 8(Floor) 2161
                                Store 2159(block) 2162
                                Line 1 835 0
                                Store 2163(texColor) 2164
                                Line 1 836 0
            2165:    8(fvec3)   Load 2163(texColor)
            2166:    8(fvec3)   VectorTimesScalar 2165 976
                                Store 2163(texColor) 2166
                                Line 1 838 0
                                Store 2167(windowRef) 298
                                Line 1 839 0
            2168:     21(ptr)   AccessChain 1933(normal) 152
            2169:    7(float)   Load 2168
            2170:   704(bool)   FOrdLessThan 2169 645
                                SelectionMerge 2172 None
                                BranchConditional 2170 2171 2172
            2171:                 Label
            2173:     21(ptr)     AccessChain 1764(distAndMat) 152
            2174:    7(float)     Load 2173
            2175:   704(bool)     FOrdEqual 2174 298
                                  Branch 2172
            2172:               Label
            2176:   704(bool)   Phi 2170 2032 2175 2171
                                SelectionMerge 2178 None
                                BranchConditional 2176 2177 2319
            2177:                 Label
                                  Line 1 842 0
            2180:    8(fvec3)     Load 1767(pos)
            2181:    8(fvec3)     DPdx 2180
                                  Store 2179(posdx) 2181
                                  Line 1 844 0
            2183:    8(fvec3)     Load 1767(pos)
            2184:    8(fvec3)     DPdy 2183
                                  Store 2182(posdy) 2184
                                  Line 1 846 0
            2186:    8(fvec3)     Load 2179(posdx)
            2188:    9(fvec2)     Load 1471(uv)
                                  Store 2187(param) 2188
            2189:    7(float)     FunctionCall 29(Hash21(vf2;) 2187(param)
            2190:    8(fvec3)     VectorTimesScalar 2186 2189
            2191:    8(fvec3)     Load 2182(posdy)
            2192:    9(fvec2)     Load 1471(uv)
            2194:    9(fvec2)     VectorTimesScalar 2192 2193
                                  Store 2195(param) 2194
            2196:    7(float)     FunctionCall 29(Hash21(vf2;) 2195(param)
            2197:    8(fvec3)     VectorTimesScalar 2191 2196
            2198:    8(fvec3)     FAdd 2190 2197
                                  Store 2185(posGrad) 2198
                                  Line 1 848 0
                                  Store 2199(colTotal) 1468
                                  Line 1 850 0
            2201:    8(fvec3)     Load 2163(texColor)
                                  Store 2200(colTemp) 2201
                                  Line 1 852 0
                                  Store 2202(nTemp) 1468
                                  Line 1 853 0
            2204:    9(fvec2)     Load 2159(block)
                                  Store 2203(param) 2204
            2206:    8(fvec3)     Load 1767(pos)
                                  Store 2205(param) 2206
            2208:    8(fvec3)     Load 2200(colTemp)
                                  Store 2207(param) 2208
            2210:    7(float)     Load 2167(windowRef)
                                  Store 2209(param) 2210
            2212:    8(fvec3)     Load 2202(nTemp)
                                  Store 2211(param) 2212
            2213:           3     FunctionCall 123(CalcWindows(vf2;vf3;vf3;f1;vf3;) 2203(param) 2205(param) 2207(param) 2209(param) 2211(param)
            2214:    8(fvec3)     Load 2207(param)
                                  Store 2200(colTemp) 2214
            2215:    7(float)     Load 2209(param)
                                  Store 2167(windowRef) 2215
            2216:    8(fvec3)     Load 2211(param)
                                  Store 2202(nTemp) 2216
                                  Line 1 854 0
            2217:    8(fvec3)     Load 2200(colTemp)
                                  Store 2199(colTotal) 2217
                                  Line 1 855 0
            2218:    8(fvec3)     Load 2163(texColor)
                                  Store 2200(colTemp) 2218
                                  Line 1 856 0
            2219:    8(fvec3)     Load 1767(pos)
            2220:    8(fvec3)     Load 2179(posdx))#"_str
+
R"#(
            2222:    8(fvec3)     VectorTimesScalar 2220 2221
            2223:    8(fvec3)     FAdd 2219 2222
            2225:    9(fvec2)     Load 2159(block)
                                  Store 2224(param) 2225
                                  Store 2226(param) 2223
            2228:    8(fvec3)     Load 2200(colTemp)
                                  Store 2227(param) 2228
            2230:    7(float)     Load 2167(windowRef)
                                  Store 2229(param) 2230
            2232:    8(fvec3)     Load 2202(nTemp)
                                  Store 2231(param) 2232
            2233:           3     FunctionCall 123(CalcWindows(vf2;vf3;vf3;f1;vf3;) 2224(param) 2226(param) 2227(param) 2229(param) 2231(param)
            2234:    8(fvec3)     Load 2227(param)
                                  Store 2200(colTemp) 2234
            2235:    7(float)     Load 2229(param)
                                  Store 2167(windowRef) 2235
            2236:    8(fvec3)     Load 2231(param)
                                  Store 2202(nTemp) 2236
                                  Line 1 857 0
            2237:    8(fvec3)     Load 2200(colTemp)
            2238:    8(fvec3)     Load 2199(colTotal)
            2239:    8(fvec3)     FAdd 2238 2237
                                  Store 2199(colTotal) 2239
                                  Line 1 858 0
            2240:    8(fvec3)     Load 2163(texColor)
                                  Store 2200(colTemp) 2240
                                  Line 1 859 0
            2241:    8(fvec3)     Load 1767(pos)
            2242:    8(fvec3)     Load 2179(posdx)
            2243:    8(fvec3)     VectorTimesScalar 2242 2221
            2244:    8(fvec3)     FAdd 2241 2243
            2245:    8(fvec3)     Load 2182(posdy)
            2246:    8(fvec3)     VectorTimesScalar 2245 2221
            2247:    8(fvec3)     FAdd 2244 2246
            2249:    9(fvec2)     Load 2159(block)
                                  Store 2248(param) 2249
                                  Store 2250(param) 2247
            2252:    8(fvec3)     Load 2200(colTemp)
                                  Store 2251(param) 2252
            2254:    7(float)     Load 2167(windowRef)
                                  Store 2253(param) 2254
            2256:    8(fvec3)     Load 2202(nTemp)
                                  Store 2255(param) 2256
            2257:           3     FunctionCall 123(CalcWindows(vf2;vf3;vf3;f1;vf3;) 2248(param) 2250(param) 2251(param) 2253(param) 2255(param)
            2258:    8(fvec3)     Load 2251(param)
                                  Store 2200(colTemp) 2258
            2259:    7(float)     Load 2253(param)
                                  Store 2167(windowRef) 2259
            2260:    8(fvec3)     Load 2255(param)
                                  Store 2202(nTemp) 2260
                                  Line 1 860 0
            2261:    8(fvec3)     Load 2200(colTemp)
            2262:    8(fvec3)     Load 2199(colTotal)
            2263:    8(fvec3)     FAdd 2262 2261
                                  Store 2199(colTotal) 2263
                                  Line 1 861 0
            2264:    8(fvec3)     Load 2163(texColor)
                                  Store 2200(colTemp) 2264
                                  Line 1 862 0
            2265:    8(fvec3)     Load 1767(pos)
            2266:    8(fvec3)     Load 2182(posdy)
            2267:    8(fvec3)     VectorTimesScalar 2266 2221
            2268:    8(fvec3)     FAdd 2265 2267
            2270:    9(fvec2)     Load 2159(block)
                                  Store 2269(param) 2270
                                  Store 2271(param) 2268
            2273:    8(fvec3)     Load 2200(colTemp)
                                  Store 2272(param) 2273
            2275:    7(float)     Load 2167(windowRef)
                                  Store 2274(param) 2275
            2277:    8(fvec3)     Load 2202(nTemp)
                                  Store 2276(param) 2277
            2278:           3     FunctionCall 123(CalcWindows(vf2;vf3;vf3;f1;vf3;) 2269(param) 2271(param) 2272(param) 2274(param) 2276(param)
            2279:    8(fvec3)     Load 2272(param)
                                  Store 2200(colTemp) 2279
            2280:    7(float)     Load 2274(param)
                                  Store 2167(windowRef) 2280
            2281:    8(fvec3)     Load 2276(param)
                                  Store 2202(nTemp) 2281
                                  Line 1 863 0
            2282:    8(fvec3)     Load 2200(colTemp)
            2283:    8(fvec3)     Load 2199(colTotal)
            2284:    8(fvec3)     FAdd 2283 2282
                                  Store 2199(colTotal) 2284
                                  Line 1 864 0
            2285:    8(fvec3)     Load 2163(texColor)
                                  Store 2200(colTemp) 2285
                                  Line 1 865 0
            2286:    8(fvec3)     Load 1767(pos)
            2287:    8(fvec3)     Load 2179(posdx)
            2289:    8(fvec3)     VectorTimesScalar 2287 2288
            2290:    8(fvec3)     FAdd 2286 2289
            2291:    8(fvec3)     Load 2182(posdy)
            2292:    8(fvec3)     VectorTimesScalar 2291 2288
            2293:    8(fvec3)     FAdd 2290 2292
            2295:    9(fvec2)     Load 2159(block)
                                  Store 2294(param) 2295
                                  Store 2296(param) 2293
            2298:    8(fvec3)     Load 2200(colTemp)
                                  Store 2297(param) 2298
            2300:    7(float)     Load 2167(windowRef)
                                  Store 2299(param) 2300
            2302:    8(fvec3)     Load 2202(nTemp)
                                  Store 2301(param) 2302
            2303:           3     FunctionCall 123(CalcWindows(vf2;vf3;vf3;f1;vf3;) 2294(param) 2296(param) 2297(param) 2299(param) 2301(param)
            2304:    8(fvec3)     Load 2297(param)
                                  Store 2200(colTemp) 2304
            2305:    7(float)     Load 2299(param)
                                  Store 2167(windowRef) 2305
            2306:    8(fvec3)     Load 2301(param)
                                  Store 2202(nTemp) 2306
                                  Line 1 866 0
            2307:    8(fvec3)     Load 2200(colTemp)
            2308:    8(fvec3)     Load 2199(colTotal)
            2309:    8(fvec3)     FAdd 2308 2307
                                  Store 2199(colTotal) 2309
                                  Line 1 867 0
            2310:    8(fvec3)     Load 2199(colTotal)
            2311:    8(fvec3)     VectorTimesScalar 2310 558
                                  Store 2163(texColor) 2311
                                  Line 1 868 0
            2312:    7(float)     Load 2167(windowRef)
            2313:    7(float)     FMul 2312 558
                                  Store 2167(windowRef) 2313
                                  Line 1 869 0
            2314:    8(fvec3)     Load 1933(normal)
            2315:    8(fvec3)     Load 2202(nTemp)
            2316:    8(fvec3)     VectorTimesScalar 2315 558
            2317:    8(fvec3)     FAdd 2314 2316
            2318:    8(fvec3)     ExtInst 2(GLSL.std.450) 69(Normalize) 2317
                                  Store 1933(normal) 2318
                                  Branch 2178
            2319:                 Label
                                  Line 1 875 0
            2321:     21(ptr)     AccessChain 1767(pos) 169
            2322:    7(float)     Load 2321
            2323:    7(float)     FAdd 2322 460
            2324:    7(float)     ExtInst 2(GLSL.std.450) 10(Fract) 2323
            2325:    7(float)     FSub 2324 460
            2326:    7(float)     ExtInst 2(GLSL.std.450) 4(FAbs) 2325
                                  Store 2320(xroad) 2326
                                  Line 1 877 0
            2328:     21(ptr)     AccessChain 1767(pos) 258
            2329:    7(float)     Load 2328
            2330:    7(float)     FAdd 2329 460
            2331:    7(float)     ExtInst 2(GLSL.std.450) 10(Fract) 2330
            2332:    7(float)     FSub 2331 460
            2333:    7(float)     ExtInst 2(GLSL.std.450) 4(FAbs) 2332
                                  Store 2327(zroad) 2333
                                  Line 1 879 0
            2335:    7(float)     Load 2320(xroad)
            2336:    7(float)     Load 2327(zroad)
            2337:    7(float)     ExtInst 2(GLSL.std.450) 37(FMin) 2335 2336
            2339:    7(float)     FSub 2337 2338
            2341:    7(float)     FMul 2339 2340
                                  Store 2342(param) 2341
            2343:    7(float)     FunctionCall 60(saturate(f1;) 2342(param)
                                  Store 2334(road) 2343
                                  Line 1 880 0
            2344:     21(ptr)     AccessChain 1933(normal) 152
            2345:    7(float)     Load 2344
            2346:    7(float)     FMul 2345 502
            2347:    9(fvec2)     Load 2159(block)
            2349:    9(fvec2)     VectorTimesScalar 2347 2348
                                  Store 2350(param) 2349
            2351:    7(float)     FunctionCall 29(Hash21(vf2;) 2350(param)
            2352:    7(float)     FMul 2346 2351
            2353:    7(float)     Load 2334(road)
            2354:    7(float)     FMul 2352 2353
            2355:    7(float)     FSub 297 2354
            2356:    8(fvec3)     Load 2163(texColor)
            2357:    8(fvec3)     VectorTimesScalar 2356 2355
                                  Store 2163(texColor) 2357
                                  Line 1 881 0
            2358:    7(float)     Load 2334(road)
            2359:    7(float)     ExtInst 2(GLSL.std.450) 46(FMix) 645 297 2358
            2360:    8(fvec3)     Load 2163(texColor)
            2361:    8(fvec3)     VectorTimesScalar 2360 2359
                                  Store 2163(texColor) 2361
                                  Line 1 883 0
            2363:    7(float)     Load 2320(xroad)
            2364:    7(float)     Load 2327(zroad)
            2365:    7(float)     ExtInst 2(GLSL.std.450) 37(FMin) 2363 2364
            2366:    7(float)     FSub 2365 2026
            2367:    7(float)     FMul 2366 2340
            2368:    7(float)     FSub 297 2367
                                  Store 2369(param) 2368
            2370:    7(float)     FunctionCall 60(saturate(f1;) 2369(param)
                                  Store 2362(yellowLine) 2370
                                  Line 1 884 0
            2371:    7(float)     Load 2320(xroad)
            2372:    7(float)     Load 2327(zroad)
            2373:    7(float)     ExtInst 2(GLSL.std.450) 37(FMin) 2371 2372
            2374:    7(float)     FSub 2373 792
            2375:    7(float)     FMul 2374 2340
                                  Store 2376(param) 2375
            2377:    7(float)     FunctionCall 60(saturate(f1;) 2376(param)
            2378:    7(float)     Load 2362(yellowLine)
            2379:    7(float)     FMul 2378 2377
                                  Store 2362(yellowLine) 2379
                                  Line 1 885 0
            2380:    7(float)     Load 2320(xroad)
            2381:    7(float)     Load 2320(xroad)
            2382:    7(float)     FMul 2380 2381
            2383:    7(float)     Load 2327(zroad)
            2384:    7(float)     Load 2327(zroad)
            2385:    7(float)     FMul 2383 2384
            2386:    7(float)     FAdd 2382 2385
            2387:    7(float)     FSub 2386 465
            2389:    7(float)     FMul 2387 2388
                                  Store 2390(param) 2389
            2391:    7(float)     FunctionCall 60(saturate(f1;) 2390(param)
            2392:    7(float)     Load 2362(yellowLine)
            2393:    7(float)     FMul 2392 2391
                                  Store 2362(yellowLine) 2393
                                  Line 1 886 0
            2394:    8(fvec3)     Load 2163(texColor)
            2396:    7(float)     Load 2362(yellowLine)
            2397:    8(fvec3)     CompositeConstruct 2396 2396 2396
            2398:    8(fvec3)     ExtInst 2(GLSL.std.450) 46(FMix) 2394 2395 2397
                                  Store 2163(texColor) 2398
                                  Line 1 888 0
            2400:    7(float)     Load 2320(xroad)
            2401:    7(float)     Load 2327(zroad)
            2402:    7(float)     ExtInst 2(GLSL.std.450) 37(FMin) 2400 2401
            2404:    7(float)     FSub 2402 2403
            2405:    7(float)     FMul 2404 2340
            2406:    7(float)     FSub 297 2405
                                  Store 2407(param) 2406
            2408:    7(float)     FunctionCall 60(saturate(f1;) 2407(param)
                                  Store 2399(whiteLine) 2408
                                  Line 1 889 0
            2409:    7(float)     Load 2320(xroad)
            2410:    7(float)     Load 2327(zroad)
            2411:    7(float)     ExtInst 2(GLSL.std.450) 37(FMin) 2409 2410
            2413:    7(float)     FSub 2411 2412
            2414:    7(float)     FMul 2413 2340
                                  Store 2415(param) 2414
            2416:    7(float)     FunctionCall 60(saturate(f1;) 2415(param)
            2417:    7(float)     Load 2399(whiteLine)
            2418:    7(float)     FMul 2417 2416
                                  Store 2399(whiteLine) 2418
                                  Line 1 890 0
            2419:    7(float)     Load 2320(xroad)
            2420:    7(float)     Load 2320(xroad)
            2421:    7(float)     FMul 2419 2420
            2422:    7(float)     Load 2327(zroad)
            2423:    7(float)     Load 2327(zroad)
            2424:    7(float)     FMul 2422 2423
            2425:    7(float)     FAdd 2421 2424
            2426:    7(float)     FSub 2425 465
            2427:    7(float)     FMul 2426 2388
                                  Store 2428(param) 2427
            2429:    7(float)     FunctionCall 60(saturate(f1;) 2428(param)
            2430:    7(float)     Load 2399(whiteLine)
            2431:    7(float)     FMul 2430 2429
                                  Store 2399(whiteLine) 2431
                                  Line 1 891 0
            2432:    7(float)     Load 2327(zroad)
            2433:    7(float)     FMul 2432 546
            2434:    7(float)     ExtInst 2(GLSL.std.450) 10(Fract) 2433
            2435:    7(float)     FSub 2434 460
            2437:    7(float)     FMul 2435 2436
            2438:    7(float)     FSub 297 2437
                                  Store 2439(param) 2438
            2440:    7(float)     FunctionCall 60(saturate(f1;) 2439(param)
            2441:    7(float)     Load 2399(whiteLine)
            2442:    7(float)     FMul 2441 2440
                                  Store 2399(whiteLine) 2442
                                  Line 1 892 0
            2443:    7(float)     Load 2320(xroad)
            2444:    7(float)     FMul 2443 546
            2445:    7(float)     ExtInst 2(GLSL.std.450) 10(Fract) 2444
            2446:    7(float)     FSub 2445 460
            2447:    7(float)     FMul 2446 2436
            2448:    7(float)     FSub 297 2447
                                  Store 2449(param) 2448
            2450:    7(float)     FunctionCall 60(saturate(f1;) 2449(param)
            2451:    7(float)     Load 2399(whiteLine)
            2452:    7(float)     FMul 2451 2450
                                  Store 2399(whiteLine) 2452
                                  Line 1 893 0
            2453:    8(fvec3)     Load 2163(texColor)
            2455:    7(float)     Load 2399(whiteLine)
            2456:    8(fvec3)     CompositeConstruct 2455 2455 2455)#"_str
+
R"#(
            2457:    8(fvec3)     ExtInst 2(GLSL.std.450) 46(FMix) 2453 2454 2456
                                  Store 2163(texColor) 2457
                                  Line 1 894 0
            2458:    7(float)     Load 2320(xroad)
            2459:    7(float)     Load 2327(zroad)
            2460:    7(float)     ExtInst 2(GLSL.std.450) 37(FMin) 2458 2459
            2462:    7(float)     FSub 2460 2461
            2463:    7(float)     FMul 2462 2340
            2464:    7(float)     FSub 297 2463
                                  Store 2465(param) 2464
            2466:    7(float)     FunctionCall 60(saturate(f1;) 2465(param)
                                  Store 2399(whiteLine) 2466
                                  Line 1 895 0
            2467:    7(float)     Load 2320(xroad)
            2468:    7(float)     Load 2327(zroad)
            2469:    7(float)     ExtInst 2(GLSL.std.450) 37(FMin) 2467 2468
            2471:    7(float)     FSub 2469 2470
            2472:    7(float)     FMul 2471 2340
                                  Store 2473(param) 2472
            2474:    7(float)     FunctionCall 60(saturate(f1;) 2473(param)
            2475:    7(float)     Load 2399(whiteLine)
            2476:    7(float)     FMul 2475 2474
                                  Store 2399(whiteLine) 2476
                                  Line 1 896 0
            2477:    7(float)     Load 2320(xroad)
            2478:    7(float)     Load 2320(xroad)
            2479:    7(float)     FMul 2477 2478
            2480:    7(float)     Load 2327(zroad)
            2481:    7(float)     Load 2327(zroad)
            2482:    7(float)     FMul 2480 2481
            2483:    7(float)     FAdd 2479 2482
            2484:    7(float)     FSub 2483 2403
            2485:    7(float)     FMul 2484 2388
                                  Store 2486(param) 2485
            2487:    7(float)     FunctionCall 60(saturate(f1;) 2486(param)
            2488:    7(float)     Load 2399(whiteLine)
            2489:    7(float)     FMul 2488 2487
                                  Store 2399(whiteLine) 2489
                                  Line 1 897 0
            2490:    8(fvec3)     Load 2163(texColor)
            2491:    7(float)     Load 2399(whiteLine)
            2492:    8(fvec3)     CompositeConstruct 2491 2491 2491
            2493:    8(fvec3)     ExtInst 2(GLSL.std.450) 46(FMix) 2490 2454 2492
                                  Store 2163(texColor) 2493
                                  Line 1 899 0
            2495:    7(float)     Load 2320(xroad)
            2496:    7(float)     FMul 2495 1361
            2497:    7(float)     ExtInst 2(GLSL.std.450) 10(Fract) 2496
            2498:    7(float)     FSub 2497 460
            2499:    7(float)     FMul 2498 2436
            2500:    7(float)     FSub 297 2499
                                  Store 2501(param) 2500
            2502:    7(float)     FunctionCall 60(saturate(f1;) 2501(param)
                                  Store 2494(crossWalk) 2502
                                  Line 1 900 0
            2503:    7(float)     Load 2327(zroad)
            2504:    7(float)     FSub 2503 889
            2505:    7(float)     FMul 2504 2388
                                  Store 2506(param) 2505
            2507:    7(float)     FunctionCall 60(saturate(f1;) 2506(param)
            2508:    7(float)     Load 2494(crossWalk)
            2509:    7(float)     FMul 2508 2507
                                  Store 2494(crossWalk) 2509
                                  Line 1 901 0
            2510:    7(float)     Load 2327(zroad)
            2511:    7(float)     FNegate 2510
            2513:    7(float)     FAdd 2511 2512
            2514:    7(float)     FMul 2513 2388
                                  Store 2515(param) 2514
            2516:    7(float)     FunctionCall 60(saturate(f1;) 2515(param)
            2517:    7(float)     Load 2334(road)
            2518:    7(float)     FSub 297 2517
            2519:    7(float)     FMul 2516 2518
            2520:    7(float)     Load 2494(crossWalk)
            2521:    7(float)     FMul 2520 2519
                                  Store 2494(crossWalk) 2521
                                  Line 1 902 0
            2522:    7(float)     Load 2123(n)
            2523:    7(float)     Load 2123(n)
            2524:    7(float)     FMul 2522 2523
            2525:    7(float)     Load 2494(crossWalk)
            2526:    7(float)     FMul 2525 2524
                                  Store 2494(crossWalk) 2526
                                  Line 1 903 0
            2527:    8(fvec3)     Load 2163(texColor)
            2529:    7(float)     Load 2494(crossWalk)
            2530:    8(fvec3)     CompositeConstruct 2529 2529 2529
            2531:    8(fvec3)     ExtInst 2(GLSL.std.450) 46(FMix) 2527 2528 2530
                                  Store 2163(texColor) 2531
                                  Line 1 904 0
            2532:    7(float)     Load 2327(zroad)
            2533:    7(float)     FMul 2532 1361
            2534:    7(float)     ExtInst 2(GLSL.std.450) 10(Fract) 2533
            2535:    7(float)     FSub 2534 460
            2536:    7(float)     FMul 2535 2436
            2537:    7(float)     FSub 297 2536
                                  Store 2538(param) 2537
            2539:    7(float)     FunctionCall 60(saturate(f1;) 2538(param)
                                  Store 2494(crossWalk) 2539
                                  Line 1 905 0
            2540:    7(float)     Load 2320(xroad)
            2541:    7(float)     FSub 2540 889
            2542:    7(float)     FMul 2541 2388
                                  Store 2543(param) 2542
            2544:    7(float)     FunctionCall 60(saturate(f1;) 2543(param)
            2545:    7(float)     Load 2494(crossWalk)
            2546:    7(float)     FMul 2545 2544
                                  Store 2494(crossWalk) 2546
                                  Line 1 906 0
            2547:    7(float)     Load 2320(xroad)
            2548:    7(float)     FNegate 2547
            2549:    7(float)     FAdd 2548 2512
            2550:    7(float)     FMul 2549 2388
                                  Store 2551(param) 2550
            2552:    7(float)     FunctionCall 60(saturate(f1;) 2551(param)
            2553:    7(float)     Load 2334(road)
            2554:    7(float)     FSub 297 2553
            2555:    7(float)     FMul 2552 2554
            2556:    7(float)     Load 2494(crossWalk)
            2557:    7(float)     FMul 2556 2555
                                  Store 2494(crossWalk) 2557
                                  Line 1 907 0
            2558:    7(float)     Load 2123(n)
            2559:    7(float)     Load 2123(n)
            2560:    7(float)     FMul 2558 2559
            2561:    7(float)     Load 2494(crossWalk)
            2562:    7(float)     FMul 2561 2560
                                  Store 2494(crossWalk) 2562
                                  Line 1 908 0
            2563:    8(fvec3)     Load 2163(texColor)
            2564:    7(float)     Load 2494(crossWalk)
            2565:    8(fvec3)     CompositeConstruct 2564 2564 2564
            2566:    8(fvec3)     ExtInst 2(GLSL.std.450) 46(FMix) 2563 2528 2565
                                  Store 2163(texColor) 2566
                                  Line 1 910 0
                                  Store 2567(sidewalk) 297
                                  Line 1 912 0
                                  Store 2568(blockSize) 2569
                                  Line 1 913 0
            2570:     21(ptr)     AccessChain 1767(pos) 152
            2571:    7(float)     Load 2570
            2572:   704(bool)     FOrdGreaterThan 2571 645
                                  SelectionMerge 2574 None
                                  BranchConditional 2572 2573 2574
            2573:                   Label
                                    Line 1 915 0
                                    Store 2568(blockSize) 2576
                                    Branch 2574
            2574:                 Label
                                  Line 1 918 0
            2577:     21(ptr)     AccessChain 1767(pos) 258
            2578:    7(float)     Load 2577
            2579:     21(ptr)     AccessChain 2568(blockSize) 169
            2580:    7(float)     Load 2579
            2581:    7(float)     FMul 2578 2580
            2582:    7(float)     ExtInst 2(GLSL.std.450) 13(Sin) 2581
            2584:    7(float)     FMul 2582 2583
            2585:     21(ptr)     AccessChain 2568(blockSize) 169
            2586:    7(float)     Load 2585
            2587:    7(float)     FDiv 2584 2586
            2588:    7(float)     ExtInst 2(GLSL.std.450) 4(FAbs) 2587
                                  Store 2589(param) 2588
            2590:    7(float)     FunctionCall 60(saturate(f1;) 2589(param)
            2591:    7(float)     Load 2567(sidewalk)
            2592:    7(float)     FMul 2591 2590
                                  Store 2567(sidewalk) 2592
                                  Line 1 919 0
            2593:     21(ptr)     AccessChain 1767(pos) 169
            2594:    7(float)     Load 2593
            2595:     21(ptr)     AccessChain 2568(blockSize) 152
            2596:    7(float)     Load 2595
            2597:    7(float)     FMul 2594 2596
            2598:    7(float)     ExtInst 2(GLSL.std.450) 13(Sin) 2597
            2599:    7(float)     FMul 2598 2583
            2600:     21(ptr)     AccessChain 2568(blockSize) 152
            2601:    7(float)     Load 2600
            2602:    7(float)     FDiv 2599 2601
            2603:    7(float)     ExtInst 2(GLSL.std.450) 4(FAbs) 2602
                                  Store 2604(param) 2603
            2605:    7(float)     FunctionCall 60(saturate(f1;) 2604(param)
            2606:    7(float)     Load 2567(sidewalk)
            2607:    7(float)     FMul 2606 2605
                                  Store 2567(sidewalk) 2607
                                  Line 1 920 0
            2608:    7(float)     Load 2567(sidewalk)
            2609:    7(float)     ExtInst 2(GLSL.std.450) 46(FMix) 1567 297 2608
                                  Store 2610(param) 2609
            2611:    7(float)     FunctionCall 60(saturate(f1;) 2610(param)
                                  Store 2567(sidewalk) 2611
                                  Line 1 921 0
            2612:    7(float)     Load 2334(road)
            2613:    7(float)     FSub 297 2612
            2614:    7(float)     Load 2567(sidewalk)
            2615:    7(float)     FAdd 2613 2614
                                  Store 2616(param) 2615
            2617:    7(float)     FunctionCall 60(saturate(f1;) 2616(param)
                                  Store 2567(sidewalk) 2617
                                  Line 1 922 0
            2618:    7(float)     Load 2567(sidewalk)
            2619:    8(fvec3)     Load 2163(texColor)
            2620:    8(fvec3)     VectorTimesScalar 2619 2618
                                  Store 2163(texColor) 2620
                                  Branch 2178
            2178:               Label
                                Line 1 926 0
            2621:     21(ptr)   AccessChain 1764(distAndMat) 152
            2622:    7(float)   Load 2621
            2623:   704(bool)   FOrdEqual 2622 276
                                SelectionMerge 2625 None
                                BranchConditional 2623 2624 2625
            2624:                 Label
                                  Line 1 928 0
                                  Store 2163(texColor) 2626
                                  Branch 2625
            2625:               Label
                                Line 1 932 0
            2627:    7(float)   Load 2123(n)
            2628:    8(fvec3)   VectorTimesScalar 387 2627
            2629:    8(fvec3)   VectorTimesScalar 2628 465
            2630:    8(fvec3)   Load 2163(texColor)
            2631:    8(fvec3)   FMul 2630 2629
                                Store 2163(texColor) 2631
                                Line 1 933 0
            2632:    8(fvec3)   Load 2163(texColor)
            2633:    8(fvec3)   VectorTimesScalar 2632 1567
                                Store 2163(texColor) 2633
                                Line 1 934 0
            2635:    8(fvec3)   Load 2163(texColor)
                                Store 2634(param) 2635
            2636:    8(fvec3)   FunctionCall 57(saturate(vf3;) 2634(param)
                                Store 2163(texColor) 2636
                                Line 1 936 0
                                Store 2637(windowMask) 298
                                Line 1 937 0
            2638:     21(ptr)   AccessChain 1764(distAndMat) 152
            2639:    7(float)   Load 2638
            2640:   704(bool)   FOrdGreaterThanEqual 2639 1723
                                SelectionMerge 2642 None
                                BranchConditional 2640 2641 2642
            2641:                 Label
                                  Line 1 939 0
            2644:     21(ptr)     AccessChain 1764(distAndMat) 152
            2645:    7(float)     Load 2644
                                  Store 2643(param) 2645
            2646:    7(float)     FunctionCall 24(Hash11(f1;) 2643(param)
            2647:    7(float)     FMul 2646 297
            2648:     21(ptr)     AccessChain 1764(distAndMat) 152
            2649:    7(float)     Load 2648
            2651:    7(float)     FMul 2649 2650
                                  Store 2652(param) 2651
            2653:    7(float)     FunctionCall 24(Hash11(f1;) 2652(param)
            2654:     21(ptr)     AccessChain 1764(distAndMat) 152
            2655:    7(float)     Load 2654
            2657:    7(float)     FMul 2655 2656
                                  Store 2658(param) 2657
            2659:    7(float)     FunctionCall 24(Hash11(f1;) 2658(param)
            2660:    8(fvec3)     CompositeConstruct 2647 2653 2659
            2661:    8(fvec3)     VectorTimesScalar 2660 645
                                  Store 2163(texColor) 2661
                                  Line 1 940 0
            2662:    8(fvec3)     Load 2163(texColor)
            2663:    8(fvec3)     ExtInst 2(GLSL.std.450) 4(FAbs) 2662
            2665:    8(fvec3)     ExtInst 2(GLSL.std.450) 26(Pow) 2663 2664
                                  Store 2163(texColor) 2665
                                  Line 1 941 0
            2666:    8(fvec3)     Load 2163(texColor)
            2667:    8(fvec3)     ExtInst 2(GLSL.std.450) 40(FMax) 2528 2666
                                  Store 2163(texColor) 2667
                                  Line 1 942 0
            2668:     21(ptr)     AccessChain 2163(texColor) 152
            2669:    7(float)     Load 2668
            2670:     21(ptr)     AccessChain 2163(texColor) 258
            2671:    7(float)     Load 2670
            2672:    7(float)     ExtInst 2(GLSL.std.450) 37(FMin) 2669 2671
            2673:     21(ptr)     AccessChain 2163(texColor) 258
                                  Store 2673 2672
                                  Line 1 943 0
            2674:     21(ptr)     AccessChain 1764(distAndMat) 152
            2675:    7(float)     Load 2674
            2677:    7(float)     FMul 2675 2676
                                  Store 2678(param) 2677
            2679:    7(float)     FunctionCall 24(Hash11(f1;) 2678(param)
            2680:    7(float)     FMul 2679 889
            2681:    8(fvec3)     Load 2163(texColor)
            2682:    8(fvec3)     VectorTimesScalar 2681 2680
                                  Store 2163(texColor) 2682
                                  Line 1 944 0
            2683:     21(ptr)     AccessChain 1767(pos) 152)#"_str
+
R"#(
            2684:    7(float)     Load 2683
            2686:    7(float)     FSub 2684 2685
            2687:    7(float)     ExtInst 2(GLSL.std.450) 4(FAbs) 2686
            2689:    7(float)     FMul 2687 2688
            2690:    7(float)     ExtInst 2(GLSL.std.450) 40(FMax) 298 2689
            2691:    7(float)     FSub 2690 570
                                  Store 2692(param) 2691
            2693:    7(float)     FunctionCall 60(saturate(f1;) 2692(param)
                                  Store 2637(windowMask) 2693
                                  Line 1 946 0
            2695:    8(fvec3)     Load 1933(normal)
            2696:    9(fvec2)     VectorShuffle 2695 2695 0 2
            2697:    9(fvec2)     ExtInst 2(GLSL.std.450) 69(Normalize) 2696
            2698:    9(fvec2)     ExtInst 2(GLSL.std.450) 4(FAbs) 2697
                                  Store 2694(dirNorm) 2698
                                  Line 1 948 0
            2700:     21(ptr)     AccessChain 2694(dirNorm) 169
            2701:    7(float)     Load 2700
            2702:     21(ptr)     AccessChain 2694(dirNorm) 152
            2703:    7(float)     Load 2702
            2704:    7(float)     ExtInst 2(GLSL.std.450) 40(FMax) 2701 2703
            2705:    7(float)     FSub 297 2704
                                  Store 2706(param) 2705
            2707:    7(float)     FunctionCall 60(saturate(f1;) 2706(param)
                                  Store 2699(pillars) 2707
                                  Line 1 949 0
            2708:    7(float)     Load 2699(pillars)
            2709:    7(float)     FSub 2708 889
            2710:    7(float)     ExtInst 2(GLSL.std.450) 40(FMax) 298 2709
            2711:    7(float)     ExtInst 2(GLSL.std.450) 26(Pow) 2710 550
                                  Store 2699(pillars) 2711
                                  Line 1 950 0
            2712:    7(float)     Load 2637(windowMask)
            2713:    7(float)     Load 2699(pillars)
            2714:    7(float)     ExtInst 2(GLSL.std.450) 40(FMax) 2712 2713
                                  Store 2637(windowMask) 2714
                                  Line 1 951 0
            2715:    7(float)     Load 2637(windowMask)
            2716:    8(fvec3)     Load 2163(texColor)
            2717:    8(fvec3)     VectorTimesScalar 2716 2715
                                  Store 2163(texColor) 2717
                                  Branch 2642
            2642:               Label
                                Line 1 956 0
            2720:    8(fvec3)   Load 481(sunCol)
            2721:    8(fvec3)   FMul 2719 2720
            2722:    8(fvec3)   Load 1456(sunDir)
            2723:    8(fvec3)   Load 1933(normal)
            2724:    7(float)   Dot 2722 2723
                                Store 2725(param) 2724
            2726:    7(float)   FunctionCall 60(saturate(f1;) 2725(param)
            2727:    8(fvec3)   VectorTimesScalar 2721 2726
            2728:    7(float)   Load 2021(sunShadow)
            2729:    8(fvec3)   VectorTimesScalar 2727 2728
                                Store 2718(lightColor) 2729
                                Line 1 958 0
            2731:    7(float)   Load 2009(ambient)
            2732:    7(float)   FMul 2731 276
            2733:    7(float)   Load 1936(ambientS)
            2734:    7(float)   FAdd 2732 2733
            2735:    7(float)   FMul 2734 536
                                Store 2730(ambientAvg) 2735
                                Line 1 959 0
            2736:    8(fvec3)   Load 492(skyCol)
            2737:     21(ptr)   AccessChain 1933(normal) 152
            2738:    7(float)   Load 2737
            2739:    7(float)   FMul 2738 460
            2740:    7(float)   FAdd 2739 460
                                Store 2741(param) 2740
            2742:    7(float)   FunctionCall 60(saturate(f1;) 2741(param)
            2743:    8(fvec3)   VectorTimesScalar 2736 2742
            2744:    7(float)   Load 2730(ambientAvg)
            2745:    7(float)   ExtInst 2(GLSL.std.450) 26(Pow) 2744 1143
            2746:    8(fvec3)   VectorTimesScalar 2743 2745
            2747:    8(fvec3)   VectorTimesScalar 2746 655
            2748:    8(fvec3)   Load 2718(lightColor)
            2749:    8(fvec3)   FAdd 2748 2747
                                Store 2718(lightColor) 2749
                                Line 1 960 0
            2750:    8(fvec3)   Load 2718(lightColor)
            2751:    8(fvec3)   VectorTimesScalar 2750 532
                                Store 2718(lightColor) 2751
                                Line 1 961 0
            2752:    8(fvec3)   Load 2163(texColor)
            2753:    8(fvec3)   Load 2718(lightColor)
            2754:    8(fvec3)   FMul 2752 2753
                                Store 1890(finalColor) 2754
                                Line 1 962 0
            2755:     21(ptr)   AccessChain 1764(distAndMat) 152
            2756:    7(float)   Load 2755
            2757:   704(bool)   FOrdGreaterThanEqual 2756 1723
                                SelectionMerge 2759 None
                                BranchConditional 2757 2758 2759
            2758:                 Label
                                  Line 1 965 0
            2761:     21(ptr)     AccessChain 2017(ref) 152
            2762:    7(float)     Load 2761
            2763:    7(float)     FMul 2762 1723
            2764:    7(float)     ExtInst 2(GLSL.std.450) 37(FMin) 297 2763
            2765:    7(float)     ExtInst 2(GLSL.std.450) 40(FMax) 779 2764
                                  Store 2760(yfade) 2765
                                  Line 1 966 0
            2766:    7(float)     Load 2637(windowMask)
            2767:    7(float)     DPdx 2766
            2768:    7(float)     Load 2637(windowMask)
            2769:    7(float)     DPdy 2768
            2770:    7(float)     FMul 2767 2769
            2771:    7(float)     ExtInst 2(GLSL.std.450) 4(FAbs) 2770
            2773:    7(float)     FMul 2771 2772
            2774:    7(float)     FSub 297 2773
                                  Store 2775(param) 2774
            2776:    7(float)     FunctionCall 60(saturate(f1;) 2775(param)
            2777:    7(float)     Load 2760(yfade)
            2778:    7(float)     FMul 2777 2776
                                  Store 2760(yfade) 2778
                                  Line 1 967 0
            2779:     21(ptr)     AccessChain 1767(pos) 152
            2780:    7(float)     Load 2779
            2781:    7(float)     FSub 2780 887
            2783:    8(fvec3)     Load 2017(ref)
                                  Store 2782(param) 2783
            2785:    8(fvec3)     Load 1456(sunDir)
                                  Store 2784(param) 2785
                                  Store 2786(param) 2781
            2787:    8(fvec3)     FunctionCall 75(GetEnvMapSkyline(vf3;vf3;f1;) 2782(param) 2784(param) 2786(param)
            2788:    8(fvec3)     VectorTimesScalar 2787 1050
            2789:    7(float)     Load 2760(yfade)
            2790:    8(fvec3)     VectorTimesScalar 2788 2789
            2791:    7(float)     Load 2021(sunShadow)
            2792:    7(float)     ExtInst 2(GLSL.std.450) 40(FMax) 941 2791
            2793:    8(fvec3)     VectorTimesScalar 2790 2792
            2794:    8(fvec3)     Load 1890(finalColor)
            2795:    8(fvec3)     FAdd 2794 2793
                                  Store 1890(finalColor) 2795
                                  Branch 2759
            2759:               Label
                                Line 1 971 0
            2796:    7(float)   Load 2167(windowRef)
            2797:   704(bool)   FOrdNotEqual 2796 298
                                SelectionMerge 2799 None
                                BranchConditional 2797 2798 2799
            2798:                 Label
                                  Line 1 973 0
            2800:    7(float)     Load 2167(windowRef)
            2801:    7(float)     ExtInst 2(GLSL.std.450) 46(FMix) 297 627 2800
            2802:    8(fvec3)     Load 1890(finalColor)
            2803:    8(fvec3)     VectorTimesScalar 2802 2801
                                  Store 1890(finalColor) 2803
                                  Line 1 975 0
            2805:     21(ptr)     AccessChain 2017(ref) 152
            2806:    7(float)     Load 2805
            2807:    7(float)     FMul 2806 1723
            2808:    7(float)     ExtInst 2(GLSL.std.450) 37(FMin) 297 2807
            2809:    7(float)     ExtInst 2(GLSL.std.450) 40(FMax) 779 2808
                                  Store 2804(yfade) 2809
                                  Line 1 976 0
            2810:     21(ptr)     AccessChain 1767(pos) 152
            2811:    7(float)     Load 2810
            2812:    7(float)     FSub 2811 460
            2814:    8(fvec3)     Load 2017(ref)
                                  Store 2813(param) 2814
            2816:    8(fvec3)     Load 1456(sunDir)
                                  Store 2815(param) 2816
                                  Store 2817(param) 2812
            2818:    8(fvec3)     FunctionCall 75(GetEnvMapSkyline(vf3;vf3;f1;) 2813(param) 2815(param) 2817(param)
            2819:    8(fvec3)     VectorTimesScalar 2818 627
            2820:    7(float)     Load 2804(yfade)
            2821:    8(fvec3)     VectorTimesScalar 2819 2820
            2822:    7(float)     Load 2021(sunShadow)
            2823:    7(float)     ExtInst 2(GLSL.std.450) 40(FMax) 627 2822
            2824:    8(fvec3)     VectorTimesScalar 2821 2823
            2825:    7(float)     Load 2167(windowRef)
            2826:    8(fvec3)     VectorTimesScalar 2824 2825
            2827:    8(fvec3)     Load 1890(finalColor)
            2828:    8(fvec3)     FAdd 2827 2826
                                  Store 1890(finalColor) 2828
                                  Branch 2799
            2799:               Label
                                Line 1 980 0
            2829:    8(fvec3)   Load 1890(finalColor)
            2830:    8(fvec3)   VectorTimesScalar 2829 1460
                                Store 1890(finalColor) 2830
                                Line 1 982 0
            2832:    8(fvec3)   Load 1762(rayVec)
                                Store 2831(rv2) 2832
                                Line 1 983 0
            2833:     21(ptr)   AccessChain 2831(rv2) 152
            2834:    7(float)   Load 2833
            2835:    7(float)   ExtInst 2(GLSL.std.450) 6(FSign) 2834
                                Store 2836(param) 2835
            2837:    7(float)   FunctionCall 60(saturate(f1;) 2836(param)
            2838:     21(ptr)   AccessChain 2831(rv2) 152
            2839:    7(float)   Load 2838
            2840:    7(float)   FMul 2839 2837
            2841:     21(ptr)   AccessChain 2831(rv2) 152
                                Store 2841 2840
                                Line 1 985 0
            2844:    8(fvec3)   Load 2831(rv2)
                                Store 2843(param) 2844
            2846:    8(fvec3)   Load 1456(sunDir)
                                Store 2845(param) 2846
            2847:    8(fvec3)   FunctionCall 69(GetEnvMap(vf3;vf3;) 2843(param) 2845(param)
                                Store 2842(fogColor) 2847
                                Line 1 986 0
            2850:    8(fvec3)   Load 2842(fogColor)
            2851:    8(fvec3)   ExtInst 2(GLSL.std.450) 37(FMin) 2849 2850
                                Store 2842(fogColor) 2851
                                Line 1 987 0
            2852:    8(fvec3)   Load 2842(fogColor)
            2853:    8(fvec3)   Load 1890(finalColor)
            2854:    7(float)   Load 1766(t)
            2855:    7(float)   FNegate 2854
            2856:    7(float)   FMul 2855 811
            2857:    7(float)   ExtInst 2(GLSL.std.450) 27(Exp) 2856
            2858:    8(fvec3)   CompositeConstruct 2857 2857 2857
            2859:    8(fvec3)   ExtInst 2(GLSL.std.450) 46(FMix) 2852 2853 2858
                                Store 1890(finalColor) 2859
                                Branch 1898
            2860:               Label
                                Line 1 992 0
            2862:    8(fvec3)   Load 1762(rayVec)
                                Store 2861(param) 2862
            2864:    8(fvec3)   Load 1456(sunDir)
                                Store 2863(param) 2864
            2865:    8(fvec3)   FunctionCall 69(GetEnvMap(vf3;vf3;) 2861(param) 2863(param)
                                Store 1890(finalColor) 2865
                                Branch 1898
            1898:             Label
                              Line 1 996 0
            2866:    9(fvec2) Load 1471(uv)
            2867:    9(fvec2) CompositeConstruct 655 655
            2868:    9(fvec2) FDiv 2866 2867
            2869:    7(float) ExtInst 2(GLSL.std.450) 66(Length) 2868
            2870:    7(float) FSub 297 2869
                              Store 2871(param) 2870
            2872:    7(float) FunctionCall 60(saturate(f1;) 2871(param)
            2873:    8(fvec3) VectorTimesScalar 387 2872
            2874:    8(fvec3) Load 1890(finalColor)
            2875:    8(fvec3) FMul 2874 2873
                              Store 1890(finalColor) 2875
                              Line 1 997 0
            2876:    7(float) Load 1465(exposure)
            2877:    7(float) FMul 1351 2876
            2878:    8(fvec3) Load 1890(finalColor)
            2879:    8(fvec3) VectorTimesScalar 2878 2877
                              Store 1890(finalColor) 2879
                              Line 1 998 0
            2880:    8(fvec3) Load 1890(finalColor)
            2881:    8(fvec3) CompositeConstruct 298 298 298
            2882:    8(fvec3) CompositeConstruct 297 297 297
            2883:    8(fvec3) ExtInst 2(GLSL.std.450) 43(FClamp) 2880 2881 2882
            2884:    7(float) Load 1466(fade)
            2885:    7(float) FAdd 2884 558
                              Store 2886(param) 2885
            2887:    7(float) FunctionCall 60(saturate(f1;) 2886(param)
            2888:    8(fvec3) VectorTimesScalar 2883 2887
            2889:    7(float) CompositeExtract 2888 0
            2890:    7(float) CompositeExtract 2888 1
            2891:    7(float) CompositeExtract 2888 2
            2892:    8(fvec3) CompositeConstruct 2889 2890 2891
                              ReturnValue 2892
                              FunctionEnd
135(mainImage(vf4;vf2;):           3 Function None 132
  133(fragColor):    131(ptr) FunctionParameter
  134(fragCoord):     26(ptr) FunctionParameter
             136:             Label
       2895(ray):    182(ptr) Variable Function
2921(finalColor):     38(ptr) Variable Function
                              Line 1 1005 0
            2897:   1472(ptr) AccessChain 150 2896 151
            2898:  130(fvec4) Load 2897
            2899:    8(fvec3) VectorShuffle 2898 2898 0 1 2
            2900:   1472(ptr) AccessChain 150 2896 189
            2901:  130(fvec4) Load 2900
            2902:    8(fvec3) VectorShuffle 2901 2901 0 1 2
            2903:   1472(ptr) AccessChain 150 2896 185
            2904:  130(fvec4) Load 2903
            2905:    8(fvec3) VectorShuffle 2904 2904 0 1 2
            2907:   1472(ptr) AccessChain 150 2896 2906
            2908:  130(fvec4) Load 2907
            2909:    8(fvec3) VectorShuffle 2908 2908 0 1 2
            2911:   1472(ptr) AccessChain 150 2910
            2912:  130(fvec4) Load 2911
            2913:    8(fvec3) VectorShuffle 2912 2912 0 1 2
            2914:    9(fvec2) Load 134(fragCoord)
            2915:   1472(ptr) AccessChain 150 151
            2916:  130(fvec4) Load 2915
            2917:    9(fvec2) VectorShuffle 2916 2916 0 1
            2918:    9(fvec2) FDiv 2914 2917)#"_str
+
R"#(
            2919:     10(Ray) FunctionCall 19(Ray_From(vf3;vf3;vf3;vf3;vf3;f1;vf2;) 2899 2902 2905 2909 2913 570 2918
                              Store 2895(ray) 2919
                              Line 1 1006 0
                              Store 1177(localTime) 298
                              Line 1 1007 0
                              Store 2920(seed) 297
                              Line 1 1008 0
                              Store 1466(fade) 297
                              Line 1 1009 0
                              Store 1465(exposure) 297
                              Line 1 1010 0
                              Store 1451(marchCount) 298
                              Line 1 1011 0
                              Store 1831(voxelPad) 558
                              Line 1 1013 0
                              Store 2921(finalColor) 1468
                              Line 1 1014 0
            2922:    153(ptr) AccessChain 150 189
            2923:    7(float) Load 2922
                              Store 1177(localTime) 2923
                              Line 1 1015 0
            2924:    9(fvec2) Load 134(fragCoord)
            2925:     10(Ray) Load 2895(ray)
            2926:    8(fvec3) FunctionCall 128(RayTrace(vf2;struct-Ray-vf3-vf3-vf31;) 2924 2925
                              Store 2921(finalColor) 2926
                              Line 1 1016 0
            2927:    8(fvec3) Load 2921(finalColor)
            2928:    8(fvec3) CompositeConstruct 298 298 298
            2929:    8(fvec3) CompositeConstruct 297 297 297
            2930:    8(fvec3) ExtInst 2(GLSL.std.450) 43(FClamp) 2927 2928 2929
            2931:    8(fvec3) ExtInst 2(GLSL.std.450) 31(Sqrt) 2930
            2932:    7(float) CompositeExtract 2931 0
            2933:    7(float) CompositeExtract 2931 1
            2934:    7(float) CompositeExtract 2931 2
            2935:  130(fvec4) CompositeConstruct 2932 2933 2934 297
                              Store 133(fragColor) 2935
                              Return
                              FunctionEnd
)#"_str );

};
};
