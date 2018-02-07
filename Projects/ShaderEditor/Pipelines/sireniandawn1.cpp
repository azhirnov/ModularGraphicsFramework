// This is generated file
// Origin file: 'C:\Projects\graphxgenengine\Projects\ShaderEditorTools\SirenianDawn1.cpp'
// Created at: 2018/02/02 - 00:31:59

#include "all_pipelines.h"

namespace Pipelines
{

void Create_sireniandawn1 (PipelineTemplateDescriptor& descr)
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
			.AddTexture( "iChannel1", EImage::Tex2D, EPixelFormatClass::RGBA | EPixelFormatClass::LinearColorSpace | EPixelFormatClass::AnyFloat | EPixelFormatClass::AnyNorm, 1, 1, EShader::bits() | EShader::Fragment )
			.AddUniformBuffer( "", 288_b, 0, 2, EShader::bits() | EShader::Fragment )
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
0x000200F8, 0x00000006, 0x0004003B, 0x0000001E, 0x0000001F, 0x00000007, 0x00040008, 0x00000001, 0x0000006C, 0x00000000, 0x0004003D, 0x0000000F, 
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
                              Line 1 108 0
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

layout(binding=1) uniform sampler2D iChannel1;

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



const vec3 lgt = vec3(-0.523, 0.41, -0.74699998);
const mat2x2 m2 = mat2x2(0.80000001, 0.60000002, -0.60000002, 0.80000001);

//---------------------------------

float curv (in vec3 p, in float w);
vec3 fog (in vec3 ro, in vec3 rd, in vec3 col, in float ds);
float linstep (in float mn, in float mx, in float x);
vec3 stars (in vec3 p);
float march (in vec3 ro, in vec3 rd, out float itrc);
void mainImage (out vec4 fragColor, in vec2 fragCoord);
vec3 normal (in vec3 p, in float ds);
float noise (in vec2 x);
float fbm (in vec2 p);
float bnoise (in vec2 p);
vec3 bump (in vec3 p, in vec3 n, in float ds);
vec3 noised (in vec2 x);
float terrain (in vec2 p);
float map (in vec3 p);
vec3 scatter (in vec3 ro, in vec3 rd);
vec3 hash33 (in vec3 p);

//---------------------------------
void main ()
{
	vec2 coord;
	coord=gl_FragCoord.xy;
	coord.y=(iResolution.xyz.y-coord.y);
	mainImage( out_Color0, coord );
}


vec3 noised (in vec2 x)
{
	vec2 p;
	p=floor( x );
	vec2 f;
	f=fract( x );
	vec2 u;
	u=((f*f)*(float(3.0)-(float(2.0)*f)));
	float a;
	a=textureLod( iChannel0, ((p+vec2(0.5))/float(256.0)), float(0.0) ).x;
	float b;
	b=textureLod( iChannel0, ((p+vec2(1.5, 0.5))/float(256.0)), float(0.0) ).x;
	float c;
	c=textureLod( iChannel0, ((p+vec2(0.5, 1.5))/float(256.0)), float(0.0) ).x;
	float d;
	d=textureLod( iChannel0, ((p+vec2(1.5))/float(256.0)), float(0.0) ).x;
	return vec3( (((a+((b-a)*u.x))+((c-a)*u.y))+(((((a-b)-c)+d)*u.x)*u.y)), (((float(6.0)*f)*(float(1.0)-f))*(vec2( (b-a), (c-a) )+((((a-b)-c)+d)*u.yx))) );
}


float terrain (in vec2 p)
{
	float rz;
	rz=float(0.0);
	float z;
	z=float(1.0);
	vec2 d;
	d=vec2(0.0);
	float scl;
	scl=float(2.95000005);
	float zscl;
	zscl=float(-0.40000001);
	float zz;
	zz=float(5.0);
	{
		int i;
		i=int(0);
		for(; (i<int(5)); ( i )++)
		{
			vec3 n;
			n=noised( p );
			(d+=pow( abs( n.yz ), vec2( zz ) ));
			(d-=smoothstep( float(-0.5), float(1.5), n.yz ));
			(zz-=float(1.0));
			(rz+=((z*n.x)/(dot( d, d )+float(0.85000002))));
			(z*=zscl);
			(zscl*=float(0.80000001));
			p=((m2*p)*scl);
		}
	}
	;
	(rz/=(smoothstep( float(1.5), float(-0.5), rz )+float(0.75)));
	return rz;
}


float map (in vec3 p)
{
	return ((p.y-(terrain( (p.zx*float(0.07)) )*float(2.70000005)))-float(1.0));
}


float march (in vec3 ro, in vec3 rd, out float itrc)
{
	float t;
	t=float(0.0);
	float d;
	d=map( ((rd*t)+ro) );
	float precis;
	precis=float(0.0001);
	{
		int i;
		i=int(0);
		for(; (i<=int(90)); ( i )++)
		{
			if ( ((abs( d )<precis)||(t>float(400.0))))
			{
				break;
			}
			;
			precis=(t*float(0.0001));
			float rl;
			rl=max( (t*float(0.02)), float(1.0) );
			(t+=(d*rl));
			d=(map( ((rd*t)+ro) )*float(0.69999999));
			( itrc )++;
		}
	}
	;
	return t;
}


vec3 normal (in vec3 p, in float ds)
{
	vec2 e;
	e=(vec2(-0.0005, 0.0005)*pow( ds, float(1.0) ));
	return normalize( ((((e.yxx*map( (p+e.yxx) ))+(e.xxy*map( (p+e.xxy) )))+(e.xyx*map( (p+e.xyx) )))+(e.yyy*map( (p+e.yyy) ))) );
}


float noise (in vec2 x)
{
	return texture( iChannel0, (x*float(0.01)) ).x;
}


float fbm (in vec2 p)
{
	float z;
	z=float(0.5);
	float rz;
	rz=float(0.0);
	{
		float i;
		i=float(0.0);
		for(; (i<float(3.0)); ( i )++)
		{
			(rz+=(((sin( (noise( p )*float(5.0)) )*float(0.5))+float(0.5))*z));
			(z*=float(0.5));
			p=(p*float(2.0));
		}
	}
	;
	return rz;
}


float bnoise (in vec2 p)
{
	return fbm( (p*float(3.0)) );
}


vec3 bump (in vec3 p, in vec3 n, in float ds)
{
	vec2 e;
	e=vec2( (float(0.005)*ds), float(0.0) );
	float n0;
	n0=bnoise( p.zx );
	vec3 d;
	d=((vec3( (bnoise( (p.zx+e.xy) )-n0), float(1.0), (bnoise( (p.zx+e.yx) )-n0) )/e.x)*float(0.025));
	(d-=(n*dot( n, d )));
	n=normalize( (n-d) );
	return n;
}


float curv (in vec3 p, in float w)
{
	vec2 e;
	e=(vec2(-1.0, 1.0)*w);
	float t1;
	t1=map( (p+e.yxx) );
	float t2;
	t2=map( (p+e.xxy) );
	float t3;
	t3=map( (p+e.xyx) );
	float t4;
	t4=map( (p+e.yyy) );
	return ((float(0.15000001)/e.y)*((((t1+t2)+t3)+t4)-(float(4.0)*map( p ))));
}


vec3 fog (in vec3 ro, in vec3 rd, in vec3 col, in float ds)
{
	vec3 pos;
	pos=(ro+(rd*ds));
	float mx;
	mx=((fbm( ((pos.zx*float(0.1))-(iTime*float(0.05))) )-float(0.5))*float(0.2));
	float den;
	den=(((float(0.30000001)*exp( (-( ro.y )*float(1.0)) ))*(float(1.0)-exp( ((-( ds )*rd.y)*float(1.0)) )))/rd.y);
	float sdt;
	sdt=max( dot( rd, lgt ), float(0.0) );
	vec3 fogColor;
	fogColor=mix( vec3(0.60000002, 0.23999999, 0.18000001), vec3(1.42999995, 0.77999997, 0.58499998), (pow( sdt, float(2.0) )+(mx*float(0.5))) );
	return mix( col, fogColor, clamp( (den+mx), float(0.0), float(1.0) ) );
}


float linstep (in float mn, in float mx, in float x)
{
	return clamp( ((x-mn)/(mx-mn)), float(0.0), float(1.0) );
}


vec3 scatter (in vec3 ro, in vec3 rd)
{
	float sd;
	sd=max( ((dot( lgt, rd )*float(0.5))+float(0.5)), float(0.0) );
	float dtp;
	dtp=(float(13.0)-((ro+(rd*float(400.0))).y*float(3.5)));
	float hori;
	hori=((linstep( float(-1500.0), float(0.0), dtp )-linstep( float(11.0), float(500.0), dtp ))*float(1.0));
	(hori*=pow( sd, float(0.04) ));
	vec3 col;
	col=vec3(0.0);
	(col+=((pow( hori, float(200.0) )*vec3(1.0, 0.69999999, 0.5))*float(3.0)));
	(col+=((pow( hori, float(25.0) )*vec3(1.0, 0.5, 0.25))*float(0.30000001)));
	(col+=((pow( hori, float(7.0) )*vec3(1.0, 0.40000001, 0.25))*float(0.80000001)));
	return col;
}


vec3 hash33 (in vec3 p)
{
	p=fract( (p*vec3(443.89749146, 397.29730225, 491.18710327)) );
	(p+=dot( p.zxy, (p.yxz+float(19.27000046)) ));
	return fract( vec3( (p.x*p.y), (p.z*p.x), (p.y*p.z) ) );
}


vec3 stars (in vec3 p)
{
	vec3 c;
	c=vec3(0.0);
	float res;
	res=(iResolution.xyz.x*float(0.80000001));
	{
		float i;
		i=float(0.0);
		for(; (i<float(3.0)); ( i )++)
		{
			vec3 q;
			q=(fract( (p*(float(0.15000001)*res)) )-float(0.5));
			vec3 id;
			id=floor( (p*(float(0.15000001)*res)) );
			vec2 rn;
			rn=hash33( id ).xy;
			float c2;
			c2=(float(1.0)-smoothstep( float(0.0), float(0.60000002), length( q ) ));
			(c2*=step( rn.x, (float(0.0005)+((i*i)*float(0.001))) ));
			(c+=(c2*((mix( vec3(1.0, 0.49000001, 0.1), vec3(0.75, 0.89999998, 1.0), rn.y )*float(0.25))+float(0.75))));
			(p*=float(1.39999998));
		}
	}
	;
	return ((c*c)*float(0.69999999));
}


void mainImage (out vec4 fragColor, in vec2 fragCoord)
{
	vec2 q;
	q=(fragCoord.xy/iResolution.xyz.xy);
	vec2 p;
	p=(q-float(0.5));
	(p.x*=(iResolution.xyz.x/iResolution.xyz.y));
	vec2 mo;
	mo=((iMouse.xy/iResolution.xyz.xy)-float(0.5));
	mo=( ((mo==vec2(-0.5))) ? (mo=vec2(-0.2, 0.30000001)) : (mo) );
	(mo.x*=float(1.20000005));
	(mo-=vec2(1.20000005, -0.1));
	(mo.x*=(iResolution.xyz.x/iResolution.xyz.y));
	(mo.x+=(sin( (iTime*float(0.15000001)) )*float(0.2)));
	vec3 ro;
	ro=vec3( float(650.0), ((sin( (iTime*float(0.2)) )*float(0.25))+float(10.0)), -( iTime ) );
	vec3 eye;
	eye=normalize( vec3( cos( mo.x ), (float(-0.5)+mo.y), sin( mo.x ) ) );
	vec3 right;
	right=normalize( vec3( cos( (mo.x+float(1.57079995)) ), float(0.0), sin( (mo.x+float(1.57079995)) ) ) );
	vec3 up;
	up=normalize( cross( right, eye ) );
	vec3 rd;
	rd=normalize( ((((p.x*right)+(p.y*up))*float(1.04999995))+eye) );
	(rd.y+=abs( ((p.x*p.x)*float(0.015)) ));
	rd=normalize( rd );
	float count;
	count=float(0.0);
	float rz;
	rz=march( ro, rd, count );
	vec3 scatt;
	scatt=scatter( ro, rd );
	vec3 bg;
	bg=(stars( rd )*(float(1.0)-clamp( dot( scatt, vec3(1.29999995) ), float(0.0), float(1.0) )));
	vec3 col;
	col=bg;
	vec3 pos;
	pos=(ro+(rz*rd));
	vec3 nor;
	nor=normal( pos, rz );
	if ( (rz<float(400.0)))
	{
		nor=bump( pos, nor, rz );
		float amb;
		amb=clamp( (float(0.5)+(float(0.5)*nor.y)), float(0.0), float(1.0) );
		float dif;
		dif=clamp( dot( nor, lgt ), float(0.0), float(1.0) );
		float bac;
		bac=clamp( dot( nor, vec3(0.57353592, 0.0, 0.81918037) ), float(0.0), float(1.0) );
		float spe;
		spe=pow( clamp( dot( reflect( rd, nor ), lgt ), float(0.0), float(1.0) ), float(500.0) );
		float fre;
		fre=pow( clamp( (float(1.0)+dot( nor, rd )), float(0.0), float(1.0) ), float(2.0) );
		vec3 brdf;
		brdf=((float(1.0)*amb)*vec3(0.1, 0.11, 0.12));
		(brdf+=(bac*vec3(0.15000001, 0.05, 0.04)));
		(brdf+=((float(2.29999995)*dif)*vec3(0.89999998, 0.40000001, 0.25)));
		col=vec3(0.25, 0.25, 0.30000001);
		float crv;
		crv=(curv( pos, float(2.0) )*float(1.0));
		float crv2;
		crv2=(curv( pos, float(0.40000001) )*float(2.5));
		(col+=(clamp( (crv*float(0.89999998)), float(-1.0), float(1.0) )*vec3(0.25, 0.60000002, 0.5)));
		col=(((col*brdf)+((col*spe)*float(0.1)))+((float(0.1)*fre)*col));
		(col*=((crv*float(1.0))+float(1.0)));
		(col*=((crv2*float(1.0))+float(1.0)));
	;
	}
	;
	col=fog( ro, rd, col, rz );
	col=mix( col, bg, smoothstep( float(250.0), float(400.0), rz ) );
	(col+=scatt);
	col=pow( col, vec3(0.93000001, 1.0, 1.0) );
	col=mix( col, smoothstep( float(0.0), float(1.0), col ), float(0.2) );
	(col*=((pow( ((((float(16.0)*q.x)*q.y)*(float(1.0)-q.x))*(float(1.0)-q.y)), float(0.1) )*float(0.89999998))+float(0.1)));
	vec4 past;
	past=texture( iChannel1, q );
	float tOver;
	tOver=clamp( (iTimeDelta-float(0.01666667)), float(0.0), float(1.0) );
	if ( ((count/pow( rz, float(0.64999998) ))>float(3.29999995)))
	{
		col=mix( col, past.xyz, clamp( (float(0.85000002)-(iTimeDelta*float(7.0))), float(0.0), float(1.0) ) );
	}
	;
	fragColor=vec4( col, float(1.0) );
}



)#"_str );
	descr.Fragment().ArraySPIRV({ 
0x07230203, 0x00010000, 0x00080002, 0x000004CE, 0x00000000, 0x00020011, 0x00000001, 0x0006000B, 0x00000002, 0x4C534C47, 0x6474732E, 0x3035342E, 
0x00000000, 0x0003000E, 0x00000000, 0x00000001, 0x0007000F, 0x00000004, 0x00000005, 0x6E69616D, 0x00000000, 0x0000005A, 0x00000070, 0x00030010, 
0x00000005, 0x00000007, 0x00030007, 0x00000001, 0x00000000, 0x002E0003, 0x00000002, 0x000001C2, 0x00000001, 0x4F202F2F, 0x646F4D70, 0x50656C75, 
0x65636F72, 0x64657373, 0x746E6520, 0x702D7972, 0x746E696F, 0x69616D20, 0x2F2F0A6E, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x63206465, 
0x6E65696C, 0x75762074, 0x6E616B6C, 0x0A303031, 0x4F202F2F, 0x646F4D70, 0x50656C75, 0x65636F72, 0x64657373, 0x72617420, 0x2D746567, 0x20766E65, 
0x6B6C7576, 0x2E316E61, 0x2F2F0A30, 0x4D704F20, 0x6C75646F, 0x6F725065, 0x73736563, 0x65206465, 0x7972746E, 0x696F702D, 0x6D20746E, 0x0A6E6961, 
0x6E696C23, 0x0A312065, 0x00000000, 0x00070004, 0x415F4C47, 0x675F4252, 0x735F7570, 0x65646168, 0x6E695F72, 0x00343674, 0x00090004, 0x415F4C47, 
0x735F4252, 0x72617065, 0x5F657461, 0x64616873, 0x6F5F7265, 0x63656A62, 0x00007374, 0x00040005, 0x00000005, 0x6E69616D, 0x00000000, 0x00050005, 
0x0000000D, 0x73696F6E, 0x76286465, 0x003B3266, 0x00030005, 0x0000000C, 0x00000078, 0x00060005, 0x00000011, 0x72726574, 0x286E6961, 0x3B326676, 
0x00000000, 0x00030005, 0x00000010, 0x00000070, 0x00050005, 0x00000016, 0x2870616D, 0x3B336676, 0x00000000, 0x00030005, 0x00000015, 0x00000070, 
0x00070005, 0x0000001D, 0x6372616D, 0x66762868, 0x66763B33, 0x31663B33, 0x0000003B, 0x00030005, 0x0000001A, 0x00006F72, 0x00030005, 0x0000001B, 
0x00006472, 0x00040005, 0x0000001C, 0x63727469, 0x00000000, 0x00060005, 0x00000022, 0x6D726F6E, 0x76286C61, 0x663B3366, 0x00003B31, 0x00030005, 
0x00000020, 0x00000070, 0x00030005, 0x00000021, 0x00007364, 0x00050005, 0x00000025, 0x73696F6E, 0x66762865, 0x00003B32, 0x00030005, 0x00000024, 
0x00000078, 0x00050005, 0x00000028, 0x286D6266, 0x3B326676, 0x00000000, 0x00030005, 0x00000027, 0x00000070, 0x00050005, 0x0000002B, 0x696F6E62, 
0x76286573, 0x003B3266, 0x00030005, 0x0000002A, 0x00000070, 0x00070005, 0x00000031, 0x706D7562, 0x33667628, 0x3366763B, 0x3B31663B, 0x00000000, 
0x00030005, 0x0000002E, 0x00000070, 0x00030005, 0x0000002F, 0x0000006E, 0x00030005, 0x00000030, 0x00007364, 0x00060005, 0x00000036, 0x76727563, 
0x33667628, 0x3B31663B, 0x00000000, 0x00030005, 0x00000034, 0x00000070, 0x00030005, 0x00000035, 0x00000077, 0x00070005, 0x0000003D, 0x28676F66, 
0x3B336676, 0x3B336676, 0x3B336676, 0x003B3166, 0x00030005, 0x00000039, 0x00006F72, 0x00030005, 0x0000003A, 0x00006472, 0x00030005, 0x0000003B, 
0x006C6F63, 0x00030005, 0x0000003C, 0x00007364, 0x00070005, 0x00000043, 0x736E696C, 0x28706574, 0x663B3166, 0x31663B31, 0x0000003B, 0x00030005, 
0x00000040, 0x00006E6D, 0x00030005, 0x00000041, 0x0000786D, 0x00030005, 0x00000042, 0x00000078, 0x00070005, 0x00000048, 0x74616373, 0x28726574, 
0x3B336676, 0x3B336676, 0x00000000, 0x00030005, 0x00000046, 0x00006F72, 0x00030005, 0x00000047, 0x00006472, 0x00050005, 0x0000004C, 0x68736168, 
0x76283333, 0x003B3366, 0x00030005, 0x0000004B, 0x00000070, 0x00050005, 0x0000004F, 0x72617473, 0x66762873, 0x00003B33, 0x00030005, 0x0000004E, 
0x00000070, 0x00070005, 0x00000056, 0x6E69616D, 0x67616D49, 0x66762865, 0x66763B34, 0x00003B32, 0x00050005, 0x00000054, 0x67617266, 0x6F6C6F43, 
0x00000072, 0x00050005, 0x00000055, 0x67617266, 0x726F6F43, 0x00000064, 0x00040005, 0x00000058, 0x726F6F63, 0x00000064, 0x00060005, 0x0000005A, 
0x465F6C67, 0x43676172, 0x64726F6F, 0x00000000, 0x00050005, 0x00000063, 0x64616853, 0x6F747265, 0x00425579, 0x00060006, 0x00000063, 0x00000000, 
0x73655269, 0x74756C6F, 0x006E6F69, 0x00050006, 0x00000063, 0x00000001, 0x6D695469, 0x00000065, 0x00060006, 0x00000063, 0x00000002, 0x6D695469, 
0x6C654465, 0x00006174, 0x00050006, 0x00000063, 0x00000003, 0x61724669, 0x0000656D, 0x00060006, 0x00000063, 0x00000004, 0x6461705F, 0x676E6964, 
0x00000030, 0x00070006, 0x00000063, 0x00000005, 0x61684369, 0x6C656E6E, 0x656D6954, 0x00000000, 0x00080006, 0x00000063, 0x00000006, 0x61684369, 
0x6C656E6E, 0x6F736552, 0x6974756C, 0x00006E6F, 0x00050006, 0x00000063, 0x00000007, 0x756F4D69, 0x00006573, 0x00050006, 0x00000063, 0x00000008, 
0x74614469, 0x00000065, 0x00060006, 0x00000063, 0x00000009, 0x6D615369, 0x52656C70, 0x00657461, 0x00060006, 0x00000063, 0x0000000A, 0x6461705F, 
0x676E6964, 0x00000031, 0x00060006, 0x00000063, 0x0000000B, 0x6461705F, 0x676E6964, 0x00000032, 0x00060006, 0x00000063, 0x0000000C, 0x6461705F, 
0x676E6964, 0x00000033, 0x00070006, 0x00000063, 0x0000000D, 0x6D614369, 0x46617265, 0x74737572, 0x00006D75, 0x00060006, 0x00000063, 0x0000000E, 
0x6D614369, 0x50617265, 0x0000736F, 0x00030005, 0x00000065, 0x00000000, 0x00050005, 0x00000070, 0x5F74756F, 0x6F6C6F43, 0x00003072, 0x00040005, 
0x00000071, 0x61726170, 0x0000006D, 0x00040005, 0x00000072, 0x61726170, 0x0000006D, 0x00030005, 0x00000076, 0x00000070, 0x00030005, 0x00000079, 
0x00000066, 0x00030005, 0x0000007C, 0x00000075, 0x00030005, 0x00000087, 0x00000061, 0x00050005, 0x0000008B, 0x61684369, 0x6C656E6E, 0x00000030, 
0x00030005, 0x00000098, 0x00000062, 0x00030005, 0x000000A2, 0x00000063, 0x00030005, 0x000000AB, 0x00000064, 0x00030005, 0x000000F1, 0x00007A72, 
0x00030005, 0x000000F2, 0x0000007A, 0x00030005, 0x000000F3, 0x00000064, 0x00030005, 0x000000F5, 0x006C6373, 0x00040005, 0x000000F7, 0x6C63737A, 
0x00000000, 0x00030005, 0x000000F9, 0x00007A7A, 0x00030005, 0x000000FC, 0x00000069, 0x00030005, 0x00000106, 0x0000006E, 0x00040005, 0x00000107, 
0x61726170, 0x0000006D, 0x00040005, 0x0000014A, 0x61726170, 0x0000006D, 0x00030005, 0x00000152, 0x00000074, 0x00030005, 0x00000153, 0x00000064, 
0x00040005, 0x00000159, 0x61726170, 0x0000006D, 0x00040005, 0x0000015B, 0x63657270, 0x00007369, 0x00030005, 0x0000015D, 0x00000069, 0x00030005, 
0x00000173, 0x00006C72, 0x00040005, 0x00000182, 0x61726170, 0x0000006D, 0x00030005, 0x0000018D, 0x00000065, 0x00040005, 0x0000019A, 0x61726170, 
0x0000006D, 0x00040005, 0x000001A3, 0x61726170, 0x0000006D, 0x00040005, 0x000001AD, 0x61726170, 0x0000006D, 0x00040005, 0x000001B7, 0x61726170, 
0x0000006D, 0x00030005, 0x000001C6, 0x0000007A, 0x00030005, 0x000001C7, 0x00007A72, 0x00030005, 0x000001C8, 0x00000069, 0x00040005, 0x000001D0, 
0x61726170, 0x0000006D, 0x00040005, 0x000001E6, 0x61726170, 0x0000006D, 0x00030005, 0x000001EA, 0x00000065, 0x00030005, 0x000001EF, 0x0000306E, 
0x00040005, 0x000001F0, 0x61726170, 0x0000006D, 0x00030005, 0x000001F4, 0x00000064, 0x00040005, 0x000001F9, 0x61726170, 0x0000006D, 0x00040005, 
0x00000202, 0x61726170, 0x0000006D, 0x00030005, 0x0000021B, 0x00000065, 0x00030005, 0x00000220, 0x00003174, 0x00040005, 0x00000225, 0x61726170, 
0x0000006D, 0x00030005, 0x00000227, 0x00003274, 0x00040005, 0x0000022C, 0x61726170, 0x0000006D, 0x00030005, 0x0000022E, 0x00003374, 0x00040005, 
0x00000233, 0x61726170, 0x0000006D, 0x00030005, 0x00000235, 0x00003474, 0x00040005, 0x0000023A, 0x61726170, 0x0000006D, 0x00040005, 0x00000248, 
0x61726170, 0x0000006D, 0x00030005, 0x00000250, 0x00736F70, 0x00030005, 0x00000256, 0x0000786D, 0x00040005, 0x00000261, 0x61726170, 0x0000006D, 
0x00030005, 0x00000266, 0x006E6564, 0x00030005, 0x0000027A, 0x00746473, 0x00050005, 0x00000282, 0x43676F66, 0x726F6C6F, 0x00000000, 0x00030005, 
0x000002A5, 0x00006473, 0x00030005, 0x000002AB, 0x00707464, 0x00040005, 0x000002B5, 0x69726F68, 0x00000000, 0x00040005, 0x000002B7, 0x61726170, 
0x0000006D, 0x00040005, 0x000002B8, 0x61726170, 0x0000006D, 0x00040005, 0x000002B9, 0x61726170, 0x0000006D, 0x00040005, 0x000002BE, 0x61726170, 
0x0000006D, 0x00040005, 0x000002BF, 0x61726170, 0x0000006D, 0x00040005, 0x000002C0, 0x61726170, 0x0000006D, 0x00030005, 0x000002CA, 0x006C6F63, 
0x00030005, 0x0000030F, 0x00000063, 0x00030005, 0x00000310, 0x00736572, 0x00030005, 0x00000314, 0x00000069, 0x00030005, 0x0000031C, 0x00000071, 
0x00030005, 0x00000324, 0x00006469, 0x00030005, 0x0000032A, 0x00006E72, 0x00040005, 0x0000032B, 0x61726170, 0x0000006D, 0x00030005, 0x0000032F, 
0x00003263, 0x00030005, 0x00000359, 0x00000071, 0x00030005, 0x00000360, 0x00000070, 0x00030005, 0x0000036D, 0x00006F6D, 0x00030005, 0x000003A0, 
0x00006F72, 0x00030005, 0x000003AD, 0x00657965, 0x00040005, 0x000003B9, 0x68676972, 0x00000074, 0x00030005, 0x000003C5, 0x00007075, 0x00030005, 
0x000003CA, 0x00006472, 0x00040005, 0x000003E7, 0x6E756F63, 0x00000074, 0x00030005, 0x000003E8, 0x00007A72, 0x00040005, 0x000003E9, 0x61726170, 
0x0000006D, 0x00040005, 0x000003EB, 0x61726170, 0x0000006D, 0x00040005, 0x000003ED, 0x61726170, 0x0000006D, 0x00040005, 0x000003F0, 0x74616373, 
0x00000074, 0x00040005, 0x000003F1, 0x61726170, 0x0000006D, 0x00040005, 0x000003F3, 0x61726170, 0x0000006D, 0x00030005, 0x000003F6, 0x00006762, 
0x00040005, 0x000003F7, 0x61726170, 0x0000006D, 0x00030005, 0x00000401, 0x006C6F63, 0x00030005, 0x00000403, 0x00736F70, 0x00030005, 0x00000409, 
0x00726F6E, 0x00040005, 0x0000040A, 0x61726170, 0x0000006D, 0x00040005, 0x0000040C, 0x61726170, 0x0000006D, 0x00040005, 0x00000413, 0x61726170, 
0x0000006D, 0x00040005, 0x00000415, 0x61726170, 0x0000006D, 0x00040005, 0x00000417, 0x61726170, 0x0000006D, 0x00030005, 0x0000041A, 0x00626D61, 
0x00030005, 0x00000420, 0x00666964, 0x00030005, 0x00000424, 0x00636162, 0x00030005, 0x0000042B, 0x00657073, 0x00030005, 0x00000432, 0x00657266, 
0x00040005, 0x00000439, 0x66647262, 0x00000000, 0x00030005, 0x0000044D, 0x00767263, 0x00040005, 0x0000044E, 0x61726170, 0x0000006D, 0x00040005, 
0x00000450, 0x61726170, 0x0000006D, 0x00040005, 0x00000453, 0x32767263, 0x00000000, 0x00040005, 0x00000454, 0x61726170, 0x0000006D, 0x00040005, 
0x00000456, 0x61726170, 0x0000006D, 0x00040005, 0x00000478, 0x61726170, 0x0000006D, 0x00040005, 0x0000047A, 0x61726170, 0x0000006D, 0x00040005, 
0x0000047C, 0x61726170, 0x0000006D, 0x00040005, 0x0000047E, 0x61726170, 0x0000006D, 0x00040005, 0x000004AA, 0x74736170, 0x00000000, 0x00050005, 
0x000004AB, 0x61684369, 0x6C656E6E, 0x00000031, 0x00040005, 0x000004AF, 0x65764F74, 0x00000072, 0x00040047, 0x0000005A, 0x0000000B, 0x0000000F, 
0x00040047, 0x00000060, 0x00000006, 0x00000010, 0x00040047, 0x00000061, 0x00000006, 0x00000010, 0x00040047, 0x00000062, 0x00000006, 0x00000010, 
0x00050048, 0x00000063, 0x00000000, 0x00000023, 0x00000000, 0x00050048, 0x00000063, 0x00000001, 0x00000023, 0x00000010, 0x00050048, 0x00000063, 
0x00000002, 0x00000023, 0x00000014, 0x00050048, 0x00000063, 0x00000003, 0x00000023, 0x00000018, 0x00050048, 0x00000063, 0x00000004, 0x00000023, 
0x0000001C, 0x00050048, 0x00000063, 0x00000005, 0x00000023, 0x00000020, 0x00050048, 0x00000063, 0x00000006, 0x00000023, 0x00000060, 0x00050048, 
0x00000063, 0x00000007, 0x00000023, 0x000000A0, 0x00050048, 0x00000063, 0x00000008, 0x00000023, 0x000000B0, 0x00050048, 0x00000063, 0x00000009, 
0x00000023, 0x000000C0, 0x00050048, 0x00000063, 0x0000000A, 0x00000023, 0x000000C4, 0x00050048, 0x00000063, 0x0000000B, 0x00000023, 0x000000C8, 
0x00050048, 0x00000063, 0x0000000C, 0x00000023, 0x000000CC, 0x00050048, 0x00000063, 0x0000000D, 0x00000023, 0x000000D0, 0x00050048, 0x00000063, 
0x0000000E, 0x00000023, 0x00000110, 0x00030047, 0x00000063, 0x00000002, 0x00040047, 0x00000065, 0x00000022, 0x00000000, 0x00040047, 0x00000065, 
0x00000021, 0x00000002, 0x00040047, 0x00000070, 0x0000001E, 0x00000000, 0x00040047, 0x0000008B, 0x00000022, 0x00000000, 0x00040047, 0x0000008B, 
0x00000021, 0x00000000, 0x00040047, 0x000004AB, 0x00000022, 0x00000000, 0x00040047, 0x000004AB, 0x00000021, 0x00000001, 0x00020013, 0x00000003, 
0x00030021, 0x00000004, 0x00000003, 0x00030016, 0x00000007, 0x00000020, 0x00040017, 0x00000008, 0x00000007, 0x00000002, 0x00040020, 0x00000009, 
0x00000007, 0x00000008, 0x00040017, 0x0000000A, 0x00000007, 0x00000003, 0x00040021, 0x0000000B, 0x0000000A, 0x00000009, 0x00040021, 0x0000000F, 
0x00000007, 0x00000009, 0x00040020, 0x00000013, 0x00000007, 0x0000000A, 0x00040021, 0x00000014, 0x00000007, 0x00000013, 0x00040020, 0x00000018, 
0x00000007, 0x00000007, 0x00060021, 0x00000019, 0x00000007, 0x00000013, 0x00000013, 0x00000018, 0x00050021, 0x0000001F, 0x0000000A, 0x00000013, 
0x00000018, 0x00060021, 0x0000002D, 0x0000000A, 0x00000013, 0x00000013, 0x00000018, 0x00050021, 0x00000033, 0x00000007, 0x00000013, 0x00000018, 
0x00070021, 0x00000038, 0x0000000A, 0x00000013, 0x00000013, 0x00000013, 0x00000018, 0x00060021, 0x0000003F, 0x00000007, 0x00000018, 0x00000018, 
0x00000018, 0x00050021, 0x00000045, 0x0000000A, 0x00000013, 0x00000013, 0x00040021, 0x0000004A, 0x0000000A, 0x00000013, 0x00040017, 0x00000051, 
0x00000007, 0x00000004, 0x00040020, 0x00000052, 0x00000007, 0x00000051, 0x00050021, 0x00000053, 0x00000003, 0x00000052, 0x00000009, 0x00040020, 
0x00000059, 0x00000001, 0x00000051, 0x0004003B, 0x00000059, 0x0000005A, 0x00000001, 0x00040015, 0x0000005D, 0x00000020, 0x00000001, 0x00040015, 
0x0000005E, 0x00000020, 0x00000000, 0x0004002B, 0x0000005E, 0x0000005F, 0x00000004, 0x0004001C, 0x00000060, 0x00000051, 0x0000005F, 0x0004001C, 
0x00000061, 0x00000051, 0x0000005F, 0x0004001C, 0x00000062, 0x00000051, 0x0000005F, 0x0011001E, 0x00000063, 0x00000051, 0x00000007, 0x00000007, 
0x0000005D, 0x00000007, 0x00000060, 0x00000061, 0x00000051, 0x00000051, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000062, 0x00000051, 
0x00040020, 0x00000064, 0x00000002, 0x00000063, 0x0004003B, 0x00000064, 0x00000065, 0x00000002, 0x0004002B, 0x0000005D, 0x00000066, 0x00000000, 
0x0004002B, 0x0000005E, 0x00000067, 0x00000001, 0x00040020, 0x00000068, 0x00000002, 0x00000007, 0x00040020, 0x0000006F, 0x00000003, 0x00000051, 
0x0004003B, 0x0000006F, 0x00000070, 0x00000003, 0x0004002B, 0x00000007, 0x00000080, 0x40400000, 0x0004002B, 0x00000007, 0x00000081, 0x40000000, 
0x00090019, 0x00000088, 0x00000007, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x0003001B, 0x00000089, 0x00000088, 
0x00040020, 0x0000008A, 0x00000000, 0x00000089, 0x0004003B, 0x0000008A, 0x0000008B, 0x00000000, 0x0004002B, 0x00000007, 0x0000008E, 0x3F000000, 
0x0005002C, 0x00000008, 0x0000008F, 0x0000008E, 0x0000008E, 0x0004002B, 0x00000007, 0x00000091, 0x43800000, 0x0004002B, 0x00000007, 0x00000094, 
0x00000000, 0x0004002B, 0x0000005E, 0x00000096, 0x00000000, 0x0004002B, 0x00000007, 0x0000009B, 0x3FC00000, 0x0005002C, 0x00000008, 0x0000009C, 
0x0000009B, 0x0000008E, 0x0005002C, 0x00000008, 0x000000A5, 0x0000008E, 0x0000009B, 0x0005002C, 0x00000008, 0x000000AE, 0x0000009B, 0x0000009B, 
0x0004002B, 0x00000007, 0x000000D1, 0x40C00000, 0x0004002B, 0x00000007, 0x000000D4, 0x3F800000, 0x0005002C, 0x00000008, 0x000000F4, 0x00000094, 
0x00000094, 0x0004002B, 0x00000007, 0x000000F6, 0x403CCCCD, 0x0004002B, 0x00000007, 0x000000F8, 0xBECCCCCD, 0x0004002B, 0x00000007, 0x000000FA, 
0x40A00000, 0x00040020, 0x000000FB, 0x00000007, 0x0000005D, 0x0004002B, 0x0000005D, 0x00000103, 0x00000005, 0x00020014, 0x00000104, 0x0004002B, 
0x00000007, 0x00000112, 0xBF000000, 0x0004002B, 0x00000007, 0x00000123, 0x3F59999A, 0x0004002B, 0x00000007, 0x0000012B, 0x3F4CCCCD, 0x00040018, 
0x0000012E, 0x00000008, 0x00000002, 0x0004002B, 0x00000007, 0x0000012F, 0x3F19999A, 0x0005002C, 0x00000008, 0x00000130, 0x0000012B, 0x0000012F, 
0x0004002B, 0x00000007, 0x00000131, 0xBF19999A, 0x0005002C, 0x00000008, 0x00000132, 0x00000131, 0x0000012B, 0x0005002C, 0x0000012E, 0x00000133, 
0x00000130, 0x00000132, 0x0004002B, 0x0000005D, 0x00000139, 0x00000001, 0x0004002B, 0x00000007, 0x0000013D, 0x3F400000, 0x0004002B, 0x00000007, 
0x00000148, 0x3D8F5C29, 0x0004002B, 0x00000007, 0x0000014C, 0x402CCCCD, 0x0004002B, 0x00000007, 0x0000015C, 0x38D1B717, 0x0004002B, 0x0000005D, 
0x00000164, 0x0000005A, 0x0004002B, 0x00000007, 0x0000016B, 0x43C80000, 0x0004002B, 0x00000007, 0x00000175, 0x3CA3D70A, 0x0004002B, 0x00000007, 
0x00000184, 0x3F333333, 0x0004002B, 0x00000007, 0x0000018E, 0xBA03126F, 0x0004002B, 0x00000007, 0x0000018F, 0x3A03126F, 0x0005002C, 0x00000008, 
0x00000190, 0x0000018E, 0x0000018F, 0x0004002B, 0x00000007, 0x000001C0, 0x3C23D70A, 0x0004002B, 0x00000007, 0x000001EB, 0x3BA3D70A, 0x0004002B, 
0x00000007, 0x0000020B, 0x3CCCCCCD, 0x0004002B, 0x00000007, 0x0000021C, 0xBF800000, 0x0005002C, 0x00000008, 0x0000021D, 0x0000021C, 0x000000D4, 
0x0004002B, 0x00000007, 0x0000023C, 0x3E19999A, 0x0004002B, 0x00000007, 0x00000247, 0x40800000, 0x0004002B, 0x00000007, 0x00000259, 0x3DCCCCCD, 
0x0004002B, 0x00000007, 0x0000025D, 0x3D4CCCCD, 0x0004002B, 0x00000007, 0x00000264, 0x3E4CCCCD, 0x0004002B, 0x00000007, 0x00000267, 0x3E99999A, 
0x0004002B, 0x00000007, 0x0000027C, 0xBF05E354, 0x0004002B, 0x00000007, 0x0000027D, 0x3ED1EB85, 0x0004002B, 0x00000007, 0x0000027E, 0xBF3F3B64, 
0x0006002C, 0x0000000A, 0x0000027F, 0x0000027C, 0x0000027D, 0x0000027E, 0x0004002B, 0x00000007, 0x00000283, 0x3E75C28F, 0x0004002B, 0x00000007, 
0x00000284, 0x3E3851EC, 0x0006002C, 0x0000000A, 0x00000285, 0x0000012F, 0x00000283, 0x00000284, 0x0004002B, 0x00000007, 0x00000286, 0x3FB70A3D, 
0x0004002B, 0x00000007, 0x00000287, 0x3F47AE14, 0x0004002B, 0x00000007, 0x00000288, 0x3F15C28F, 0x0006002C, 0x0000000A, 0x00000289, 0x00000286, 
0x00000287, 0x00000288, 0x0004002B, 0x00000007, 0x000002AC, 0x41500000, 0x0004002B, 0x00000007, 0x000002B2, 0x40600000, 0x0004002B, 0x00000007, 
0x000002B6, 0xC4BB8000, 0x0004002B, 0x00000007, 0x000002BC, 0x41300000, 0x0004002B, 0x00000007, 0x000002BD, 0x43FA0000, 0x0004002B, 0x00000007, 
0x000002C6, 0x3D23D70A, 0x0006002C, 0x0000000A, 0x000002CB, 0x00000094, 0x00000094, 0x00000094, 0x0004002B, 0x00000007, 0x000002CD, 0x43480000, 
0x0006002C, 0x0000000A, 0x000002CF, 0x000000D4, 0x00000184, 0x0000008E, 0x0004002B, 0x00000007, 0x000002D5, 0x41C80000, 0x0004002B, 0x00000007, 
0x000002D7, 0x3E800000, 0x0006002C, 0x0000000A, 0x000002D8, 0x000000D4, 0x0000008E, 0x000002D7, 0x0004002B, 0x00000007, 0x000002DE, 0x40E00000, 
0x0004002B, 0x00000007, 0x000002E0, 0x3ECCCCCD, 0x0006002C, 0x0000000A, 0x000002E1, 0x000000D4, 0x000002E0, 0x000002D7, 0x0004002B, 0x00000007, 
0x000002EA, 0x43DDF2E1, 0x0004002B, 0x00000007, 0x000002EB, 0x43C6A60E, 0x0004002B, 0x00000007, 0x000002EC, 0x43F597F3, 0x0006002C, 0x0000000A, 
0x000002ED, 0x000002EA, 0x000002EB, 0x000002EC, 0x0004002B, 0x00000007, 0x000002F4, 0x419A28F6, 0x0004002B, 0x0000005E, 0x00000300, 0x00000002, 
0x0004002B, 0x00000007, 0x00000339, 0x3A83126F, 0x0004002B, 0x00000007, 0x00000340, 0x3EFAE148, 0x0006002C, 0x0000000A, 0x00000341, 0x000000D4, 
0x00000340, 0x00000259, 0x0004002B, 0x00000007, 0x00000342, 0x3F666666, 0x0006002C, 0x0000000A, 0x00000343, 0x0000013D, 0x00000342, 0x000000D4, 
0x0004002B, 0x00000007, 0x0000034E, 0x3FB33333, 0x00040020, 0x0000035B, 0x00000002, 0x00000051, 0x0004002B, 0x0000005D, 0x0000036E, 0x00000007, 
0x0005002C, 0x00000008, 0x0000037A, 0x00000112, 0x00000112, 0x00040017, 0x0000037B, 0x00000104, 0x00000002, 0x0004002B, 0x00000007, 0x00000380, 
0xBE4CCCCD, 0x0005002C, 0x00000008, 0x00000381, 0x00000380, 0x00000267, 0x0004002B, 0x00000007, 0x00000385, 0x3F99999A, 0x0004002B, 0x00000007, 
0x0000038A, 0xBDCCCCCD, 0x0005002C, 0x00000008, 0x0000038B, 0x00000385, 0x0000038A, 0x0004002B, 0x00000007, 0x000003A1, 0x44228000, 0x0004002B, 
0x00000007, 0x000003A7, 0x41200000, 0x0004002B, 0x00000007, 0x000003BC, 0x3FC90FF9, 0x0004002B, 0x00000007, 0x000003D4, 0x3F866666, 0x0004002B, 
0x00000007, 0x000003DE, 0x3C75C28F, 0x0004002B, 0x00000007, 0x000003FB, 0x3FA66666, 0x0006002C, 0x0000000A, 0x000003FC, 0x000003FB, 0x000003FB, 
0x000003FB, 0x0004002B, 0x00000007, 0x00000426, 0x3F12D340, 0x0004002B, 0x00000007, 0x00000427, 0x3F51B5CE, 0x0006002C, 0x0000000A, 0x00000428, 
0x00000426, 0x00000094, 0x00000427, 0x0004002B, 0x00000007, 0x0000043C, 0x3DE147AE, 0x0004002B, 0x00000007, 0x0000043D, 0x3DF5C28F, 0x0006002C, 
0x0000000A, 0x0000043E, 0x00000259, 0x0000043C, 0x0000043D, 0x0006002C, 0x0000000A, 0x00000441, 0x0000023C, 0x0000025D, 0x000002C6, 0x0004002B, 
0x00000007, 0x00000445, 0x40133333, 0x0006002C, 0x0000000A, 0x00000448, 0x00000342, 0x000002E0, 0x000002D7, 0x0006002C, 0x0000000A, 0x0000044C, 
0x000002D7, 0x000002D7, 0x00000267, 0x0004002B, 0x00000007, 0x00000458, 0x40200000, 0x0006002C, 0x0000000A, 0x0000045D, 0x000002D7, 0x0000012F, 
0x0000008E, 0x0004002B, 0x00000007, 0x00000483, 0x437A0000, 0x0004002B, 0x00000007, 0x0000048C, 0x3F6E147B, 0x0006002C, 0x0000000A, 0x0000048D, 
0x0000048C, 0x000000D4, 0x000000D4, 0x0004002B, 0x00000007, 0x00000496, 0x41800000, 0x0004003B, 0x0000008A, 0x000004AB, 0x00000000, 0x0004002B, 
0x0000005D, 0x000004B0, 0x00000002, 0x0004002B, 0x00000007, 0x000004B3, 0x3C88888A, 0x0004002B, 0x00000007, 0x000004B8, 0x3F266666, 0x0004002B, 
0x00000007, 0x000004BB, 0x40533333, 0x00050036, 0x00000003, 0x00000005, 0x00000000, 0x00000004, 0x000200F8, 0x00000006, 0x0004003B, 0x00000009, 
0x00000058, 0x00000007, 0x0004003B, 0x00000052, 0x00000071, 0x00000007, 0x0004003B, 0x00000009, 0x00000072, 0x00000007, 0x00040008, 0x00000001, 
0x00000086, 0x00000000, 0x0004003D, 0x00000051, 0x0000005B, 0x0000005A, 0x0007004F, 0x00000008, 0x0000005C, 0x0000005B, 0x0000005B, 0x00000000, 
0x00000001, 0x0003003E, 0x00000058, 0x0000005C, 0x00040008, 0x00000001, 0x00000087, 0x00000000, 0x00060041, 0x00000068, 0x00000069, 0x00000065, 
0x00000066, 0x00000067, 0x0004003D, 0x00000007, 0x0000006A, 0x00000069, 0x00050041, 0x00000018, 0x0000006B, 0x00000058, 0x00000067, 0x0004003D, 
0x00000007, 0x0000006C, 0x0000006B, 0x00050083, 0x00000007, 0x0000006D, 0x0000006A, 0x0000006C, 0x00050041, 0x00000018, 0x0000006E, 0x00000058, 
0x00000067, 0x0003003E, 0x0000006E, 0x0000006D, 0x00040008, 0x00000001, 0x00000088, 0x00000000, 0x0004003D, 0x00000008, 0x00000073, 0x00000058, 
0x0003003E, 0x00000072, 0x00000073, 0x00060039, 0x00000003, 0x00000074, 0x00000056, 0x00000071, 0x00000072, 0x0004003D, 0x00000051, 0x00000075, 
0x00000071, 0x0003003E, 0x00000070, 0x00000075, 0x000100FD, 0x00010038, 0x00050036, 0x0000000A, 0x0000000D, 0x00000000, 0x0000000B, 0x00030037, 
0x00000009, 0x0000000C, 0x000200F8, 0x0000000E, 0x0004003B, 0x00000009, 0x00000076, 0x00000007, 0x0004003B, 0x00000009, 0x00000079, 0x00000007, 
0x0004003B, 0x00000009, 0x0000007C, 0x00000007, 0x0004003B, 0x00000018, 0x00000087, 0x00000007, 0x0004003B, 0x00000018, 0x00000098, 0x00000007, 
0x0004003B, 0x00000018, 0x000000A2, 0x00000007, 0x0004003B, 0x00000018, 0x000000AB, 0x00000007, 0x00040008, 0x00000001, 0x0000008F, 0x00000000, 
0x0004003D, 0x00000008, 0x00000077, 0x0000000C, 0x0006000C, 0x00000008, 0x00000078, 0x00000002, 0x00000008, 0x00000077, 0x0003003E, 0x00000076, 
0x00000078, 0x00040008, 0x00000001, 0x00000091, 0x00000000, 0x0004003D, 0x00000008, 0x0000007A, 0x0000000C, 0x0006000C, 0x00000008, 0x0000007B, 
0x00000002, 0x0000000A, 0x0000007A, 0x0003003E, 0x00000079, 0x0000007B, 0x00040008, 0x00000001, 0x00000093, 0x00000000, 0x0004003D, 0x00000008, 
0x0000007D, 0x00000079, 0x0004003D, 0x00000008, 0x0000007E, 0x00000079, 0x00050085, 0x00000008, 0x0000007F, 0x0000007D, 0x0000007E, 0x0004003D, 
0x00000008, 0x00000082, 0x00000079, 0x0005008E, 0x00000008, 0x00000083, 0x00000082, 0x00000081, 0x00050050, 0x00000008, 0x00000084, 0x00000080, 
0x00000080, 0x00050083, 0x00000008, 0x00000085, 0x00000084, 0x00000083, 0x00050085, 0x00000008, 0x00000086, 0x0000007F, 0x00000085, 0x0003003E, 
0x0000007C, 0x00000086, 0x00040008, 0x00000001, 0x00000095, 0x00000000, 0x0004003D, 0x00000089, 0x0000008C, 0x0000008B, 0x0004003D, 0x00000008, 
0x0000008D, 0x00000076, 0x00050081, 0x00000008, 0x00000090, 0x0000008D, 0x0000008F, 0x00050050, 0x00000008, 0x00000092, 0x00000091, 0x00000091, 
0x00050088, 0x00000008, 0x00000093, 0x00000090, 0x00000092, 0x00070058, 0x00000051, 0x00000095, 0x0000008C, 0x00000093, 0x00000002, 0x00000094, 
0x00050051, 0x00000007, 0x00000097, 0x00000095, 0x00000000, 0x0003003E, 0x00000087, 0x00000097, 0x00040008, 0x00000001, 0x00000097, 0x00000000, 
0x0004003D, 0x00000089, 0x00000099, 0x0000008B, 0x0004003D, 0x00000008, 0x0000009A, 0x00000076, 0x00050081, 0x00000008, 0x0000009D, 0x0000009A, 
0x0000009C, 0x00050050, 0x00000008, 0x0000009E, 0x00000091, 0x00000091, 0x00050088, 0x00000008, 0x0000009F, 0x0000009D, 0x0000009E, 0x00070058, 
0x00000051, 0x000000A0, 0x00000099, 0x0000009F, 0x00000002, 0x00000094, 0x00050051, 0x00000007, 0x000000A1, 0x000000A0, 0x00000000, 0x0003003E, 
0x00000098, 0x000000A1, 0x00040008, 0x00000001, 0x00000099, 0x00000000, 0x0004003D, 0x00000089, 0x000000A3, 0x0000008B, 0x0004003D, 0x00000008, 
0x000000A4, 0x00000076, 0x00050081, 0x00000008, 0x000000A6, 0x000000A4, 0x000000A5, 0x00050050, 0x00000008, 0x000000A7, 0x00000091, 0x00000091, 
0x00050088, 0x00000008, 0x000000A8, 0x000000A6, 0x000000A7, 0x00070058, 0x00000051, 0x000000A9, 0x000000A3, 0x000000A8, 0x00000002, 0x00000094, 
0x00050051, 0x00000007, 0x000000AA, 0x000000A9, 0x00000000, 0x0003003E, 0x000000A2, 0x000000AA, 0x00040008, 0x00000001, 0x0000009B, 0x00000000, 
0x0004003D, 0x00000089, 0x000000AC, 0x0000008B, 0x0004003D, 0x00000008, 0x000000AD, 0x00000076, 0x00050081, 0x00000008, 0x000000AF, 0x000000AD, 
0x000000AE, 0x00050050, 0x00000008, 0x000000B0, 0x00000091, 0x00000091, 0x00050088, 0x00000008, 0x000000B1, 0x000000AF, 0x000000B0, 0x00070058, 
0x00000051, 0x000000B2, 0x000000AC, 0x000000B1, 0x00000002, 0x00000094, 0x00050051, 0x00000007, 0x000000B3, 0x000000B2, 0x00000000, 0x0003003E, 
0x000000AB, 0x000000B3, 0x00040008, 0x00000001, 0x0000009C, 0x00000000, 0x0004003D, 0x00000007, 0x000000B4, 0x00000087, 0x0004003D, 0x00000007, 
0x000000B5, 0x00000098, 0x0004003D, 0x00000007, 0x000000B6, 0x00000087, 0x00050083, 0x00000007, 0x000000B7, 0x000000B5, 0x000000B6, 0x00050041, 
0x00000018, 0x000000B8, 0x0000007C, 0x00000096, 0x0004003D, 0x00000007, 0x000000B9, 0x000000B8, 0x00050085, 0x00000007, 0x000000BA, 0x000000B7, 
0x000000B9, 0x00050081, 0x00000007, 0x000000BB, 0x000000B4, 0x000000BA, 0x0004003D, 0x00000007, 0x000000BC, 0x000000A2, 0x0004003D, 0x00000007, 
0x000000BD, 0x00000087, 0x00050083, 0x00000007, 0x000000BE, 0x000000BC, 0x000000BD, 0x00050041, 0x00000018, 0x000000BF, 0x0000007C, 0x00000067, 
0x0004003D, 0x00000007, 0x000000C0, 0x000000BF, 0x00050085, 0x00000007, 0x000000C1, 0x000000BE, 0x000000C0, 0x00050081, 0x00000007, 0x000000C2, 
0x000000BB, 0x000000C1, 0x0004003D, 0x00000007, 0x000000C3, 0x00000087, 0x0004003D, 0x00000007, 0x000000C4, 0x00000098, 0x00050083, 0x00000007, 
0x000000C5, 0x000000C3, 0x000000C4, 0x0004003D, 0x00000007, 0x000000C6, 0x000000A2, 0x00050083, 0x00000007, 0x000000C7, 0x000000C5, 0x000000C6, 
0x0004003D, 0x00000007, 0x000000C8, 0x000000AB, 0x00050081, 0x00000007, 0x000000C9, 0x000000C7, 0x000000C8, 0x00050041, 0x00000018, 0x000000CA, 
0x0000007C, 0x00000096, 0x0004003D, 0x00000007, 0x000000CB, 0x000000CA, 0x00050085, 0x00000007, 0x000000CC, 0x000000C9, 0x000000CB, 0x00050041, 
0x00000018, 0x000000CD, 0x0000007C, 0x00000067, 0x0004003D, 0x00000007, 0x000000CE, 0x000000CD, 0x00050085, 0x00000007, 0x000000CF, 0x000000CC, 
0x000000CE, 0x00050081, 0x00000007, 0x000000D0, 0x000000C2, 0x000000CF, 0x0004003D, 0x00000008, 0x000000D2, 0x00000079, 0x0005008E, 0x00000008, 
0x000000D3, 0x000000D2, 0x000000D1, 0x0004003D, 0x00000008, 0x000000D5, 0x00000079, 0x00050050, 0x00000008, 0x000000D6, 0x000000D4, 0x000000D4, 
0x00050083, 0x00000008, 0x000000D7, 0x000000D6, 0x000000D5, 0x00050085, 0x00000008, 0x000000D8, 0x000000D3, 0x000000D7, 0x0004003D, 0x00000007, 
0x000000D9, 0x00000098, 0x0004003D, 0x00000007, 0x000000DA, 0x00000087, 0x00050083, 0x00000007, 0x000000DB, 0x000000D9, 0x000000DA, 0x0004003D, 
0x00000007, 0x000000DC, 0x000000A2, 0x0004003D, 0x00000007, 0x000000DD, 0x00000087, 0x00050083, 0x00000007, 0x000000DE, 0x000000DC, 0x000000DD, 
0x00050050, 0x00000008, 0x000000DF, 0x000000DB, 0x000000DE, 0x0004003D, 0x00000007, 0x000000E0, 0x00000087, 0x0004003D, 0x00000007, 0x000000E1, 
0x00000098, 0x00050083, 0x00000007, 0x000000E2, 0x000000E0, 0x000000E1, 0x0004003D, 0x00000007, 0x000000E3, 0x000000A2, 0x00050083, 0x00000007, 
0x000000E4, 0x000000E2, 0x000000E3, 0x0004003D, 0x00000007, 0x000000E5, 0x000000AB, 0x00050081, 0x00000007, 0x000000E6, 0x000000E4, 0x000000E5, 
0x0004003D, 0x00000008, 0x000000E7, 0x0000007C, 0x0007004F, 0x00000008, 0x000000E8, 0x000000E7, 0x000000E7, 0x00000001, 0x00000000, 0x0005008E, 
0x00000008, 0x000000E9, 0x000000E8, 0x000000E6, 0x00050081, 0x00000008, 0x000000EA, 0x000000DF, 0x000000E9, 0x00050085, 0x00000008, 0x000000EB, 
0x000000D8, 0x000000EA, 0x00050051, 0x00000007, 0x000000EC, 0x000000EB, 0x00000000, 0x00050051, 0x00000007, 0x000000ED, 0x000000EB, 0x00000001, 
0x00060050, 0x0000000A, 0x000000EE, 0x000000D0, 0x000000EC, 0x000000ED, 0x000200FE, 0x000000EE, 0x00010038, 0x00050036, 0x00000007, 0x00000011, 
0x00000000, 0x0000000F, 0x00030037, 0x00000009, 0x00000010, 0x000200F8, 0x00000012, 0x0004003B, 0x00000018, 0x000000F1, 0x00000007, 0x0004003B, 
0x00000018, 0x000000F2, 0x00000007, 0x0004003B, 0x00000009, 0x000000F3, 0x00000007, 0x0004003B, 0x00000018, 0x000000F5, 0x00000007, 0x0004003B, 
0x00000018, 0x000000F7, 0x00000007, 0x0004003B, 0x00000018, 0x000000F9, 0x00000007, 0x0004003B, 0x000000FB, 0x000000FC, 0x00000007, 0x0004003B, 
0x00000013, 0x00000106, 0x00000007, 0x0004003B, 0x00000009, 0x00000107, 0x00000007, 0x00040008, 0x00000001, 0x000000A3, 0x00000000, 0x0003003E, 
0x000000F1, 0x00000094, 0x00040008, 0x00000001, 0x000000A5, 0x00000000, 0x0003003E, 0x000000F2, 0x000000D4, 0x00040008, 0x00000001, 0x000000A7, 
0x00000000, 0x0003003E, 0x000000F3, 0x000000F4, 0x00040008, 0x00000001, 0x000000A9, 0x00000000, 0x0003003E, 0x000000F5, 0x000000F6, 0x00040008, 
0x00000001, 0x000000AB, 0x00000000, 0x0003003E, 0x000000F7, 0x000000F8, 0x00040008, 0x00000001, 0x000000AD, 0x00000000, 0x0003003E, 0x000000F9, 
0x000000FA, 0x00040008, 0x00000001, 0x000000B0, 0x00000000, 0x0003003E, 0x000000FC, 0x00000066, 0x000200F9, 0x000000FD, 0x00040008, 0x00000001, 
0x000000B1, 0x00000000, 0x000200F8, 0x000000FD, 0x000400F6, 0x000000FF, 0x00000100, 0x00000000, 0x000200F9, 0x00000101, 0x000200F8, 0x00000101, 
0x0004003D, 0x0000005D, 0x00000102, 0x000000FC, 0x000500B1, 0x00000104, 0x00000105, 0x00000102, 0x00000103, 0x000400FA, 0x00000105, 0x000000FE, 
0x000000FF, 0x000200F8, 0x000000FE, 0x00040008, 0x00000001, 0x000000B4, 0x00000000, 0x0004003D, 0x00000008, 0x00000108, 0x00000010, 0x0003003E, 
0x00000107, 0x00000108, 0x00050039, 0x0000000A, 0x00000109, 0x0000000D, 0x00000107, 0x0003003E, 0x00000106, 0x00000109, 0x00040008, 0x00000001, 
0x000000B5, 0x00000000, 0x0004003D, 0x0000000A, 0x0000010A, 0x00000106, 0x0007004F, 0x00000008, 0x0000010B, 0x0000010A, 0x0000010A, 0x00000001, 
0x00000002, 0x0006000C, 0x00000008, 0x0000010C, 0x00000002, 0x00000004, 0x0000010B, 0x0004003D, 0x00000007, 0x0000010D, 0x000000F9, 0x00050050, 
0x00000008, 0x0000010E, 0x0000010D, 0x0000010D, 0x0007000C, 0x00000008, 0x0000010F, 0x00000002, 0x0000001A, 0x0000010C, 0x0000010E, 0x0004003D, 
0x00000008, 0x00000110, 0x000000F3, 0x00050081, 0x00000008, 0x00000111, 0x00000110, 0x0000010F, 0x0003003E, 0x000000F3, 0x00000111, 0x00040008, 
0x00000001, 0x000000B6, 0x00000000, 0x0004003D, 0x0000000A, 0x00000113, 0x00000106, 0x0007004F, 0x00000008, 0x00000114, 0x00000113, 0x00000113, 
0x00000001, 0x00000002, 0x00050050, 0x00000008, 0x00000115, 0x00000112, 0x00000112, 0x00050050, 0x00000008, 0x00000116, 0x0000009B, 0x0000009B, 
0x0008000C, 0x00000008, 0x00000117, 0x00000002, 0x00000031, 0x00000115, 0x00000116, 0x00000114, 0x0004003D, 0x00000008, 0x00000118, 0x000000F3, 
0x00050083, 0x00000008, 0x00000119, 0x00000118, 0x00000117, 0x0003003E, 0x000000F3, 0x00000119, 0x00040008, 0x00000001, 0x000000B7, 0x00000000, 
0x0004003D, 0x00000007, 0x0000011A, 0x000000F9, 0x00050083, 0x00000007, 0x0000011B, 0x0000011A, 0x000000D4, 0x0003003E, 0x000000F9, 0x0000011B, 
0x00040008, 0x00000001, 0x000000B8, 0x00000000, 0x0004003D, 0x00000007, 0x0000011C, 0x000000F2, 0x00050041, 0x00000018, 0x0000011D, 0x00000106, 
0x00000096, 0x0004003D, 0x00000007, 0x0000011E, 0x0000011D, 0x00050085, 0x00000007, 0x0000011F, 0x0000011C, 0x0000011E, 0x0004003D, 0x00000008, 
0x00000120, 0x000000F3, 0x0004003D, 0x00000008, 0x00000121, 0x000000F3, 0x00050094, 0x00000007, 0x00000122, 0x00000120, 0x00000121, 0x00050081, 
0x00000007, 0x00000124, 0x00000122, 0x00000123, 0x00050088, 0x00000007, 0x00000125, 0x0000011F, 0x00000124, 0x0004003D, 0x00000007, 0x00000126, 
0x000000F1, 0x00050081, 0x00000007, 0x00000127, 0x00000126, 0x00000125, 0x0003003E, 0x000000F1, 0x00000127, 0x00040008, 0x00000001, 0x000000B9, 
0x00000000, 0x0004003D, 0x00000007, 0x00000128, 0x000000F7, 0x0004003D, 0x00000007, 0x00000129, 0x000000F2, 0x00050085, 0x00000007, 0x0000012A, 
0x00000129, 0x00000128, 0x0003003E, 0x000000F2, 0x0000012A, 0x00040008, 0x00000001, 0x000000BA, 0x00000000, 0x0004003D, 0x00000007, 0x0000012C, 
0x000000F7, 0x00050085, 0x00000007, 0x0000012D, 0x0000012C, 0x0000012B, 0x0003003E, 0x000000F7, 0x0000012D, 0x00040008, 0x00000001, 0x000000BB, 
0x00000000, 0x0004003D, 0x00000008, 0x00000134, 0x00000010, 0x00050091, 0x00000008, 0x00000135, 0x00000133, 0x00000134, 0x0004003D, 0x00000007, 
0x00000136, 0x000000F5, 0x0005008E, 0x00000008, 0x00000137, 0x00000135, 0x00000136, 0x0003003E, 0x00000010, 0x00000137, 0x000200F9, 0x00000100, 
0x000200F8, 0x00000100, 0x00040008, 0x00000001, 0x000000B1, 0x00000000, 0x0004003D, 0x0000005D, 0x00000138, 0x000000FC, 0x00050080, 0x0000005D, 
0x0000013A, 0x00000138, 0x00000139, 0x0003003E, 0x000000FC, 0x0000013A, 0x000200F9, 0x000000FD, 0x000200F8, 0x000000FF, 0x00040008, 0x00000001, 
0x000000BF, 0x00000000, 0x0004003D, 0x00000007, 0x0000013B, 0x000000F1, 0x0008000C, 0x00000007, 0x0000013C, 0x00000002, 0x00000031, 0x0000009B, 
0x00000112, 0x0000013B, 0x00050081, 0x00000007, 0x0000013E, 0x0000013C, 0x0000013D, 0x0004003D, 0x00000007, 0x0000013F, 0x000000F1, 0x00050088, 
0x00000007, 0x00000140, 0x0000013F, 0x0000013E, 0x0003003E, 0x000000F1, 0x00000140, 0x00040008, 0x00000001, 0x000000C0, 0x00000000, 0x0004003D, 
0x00000007, 0x00000141, 0x000000F1, 0x000200FE, 0x00000141, 0x00010038, 0x00050036, 0x00000007, 0x00000016, 0x00000000, 0x00000014, 0x00030037, 
0x00000013, 0x00000015, 0x000200F8, 0x00000017, 0x0004003B, 0x00000009, 0x0000014A, 0x00000007, 0x00040008, 0x00000001, 0x000000C6, 0x00000000, 
0x00050041, 0x00000018, 0x00000144, 0x00000015, 0x00000067, 0x0004003D, 0x00000007, 0x00000145, 0x00000144, 0x0004003D, 0x0000000A, 0x00000146, 
0x00000015, 0x0007004F, 0x00000008, 0x00000147, 0x00000146, 0x00000146, 0x00000002, 0x00000000, 0x0005008E, 0x00000008, 0x00000149, 0x00000147, 
0x00000148, 0x0003003E, 0x0000014A, 0x00000149, 0x00050039, 0x00000007, 0x0000014B, 0x00000011, 0x0000014A, 0x00050085, 0x00000007, 0x0000014D, 
0x0000014B, 0x0000014C, 0x00050083, 0x00000007, 0x0000014E, 0x00000145, 0x0000014D, 0x00050083, 0x00000007, 0x0000014F, 0x0000014E, 0x000000D4, 
0x000200FE, 0x0000014F, 0x00010038, 0x00050036, 0x00000007, 0x0000001D, 0x00000000, 0x00000019, 0x00030037, 0x00000013, 0x0000001A, 0x00030037, 
0x00000013, 0x0000001B, 0x00030037, 0x00000018, 0x0000001C, 0x000200F8, 0x0000001E, 0x0004003B, 0x00000018, 0x00000152, 0x00000007, 0x0004003B, 
0x00000018, 0x00000153, 0x00000007, 0x0004003B, 0x00000013, 0x00000159, 0x00000007, 0x0004003B, 0x00000018, 0x0000015B, 0x00000007, 0x0004003B, 
0x000000FB, 0x0000015D, 0x00000007, 0x0004003B, 0x00000018, 0x00000173, 0x00000007, 0x0004003B, 0x00000013, 0x00000182, 0x00000007, 0x00040008, 
0x00000001, 0x000000CD, 0x00000000, 0x0003003E, 0x00000152, 0x00000094, 0x00040008, 0x00000001, 0x000000CF, 0x00000000, 0x0004003D, 0x0000000A, 
0x00000154, 0x0000001B, 0x0004003D, 0x00000007, 0x00000155, 0x00000152, 0x0005008E, 0x0000000A, 0x00000156, 0x00000154, 0x00000155, 0x0004003D, 
0x0000000A, 0x00000157, 0x0000001A, 0x00050081, 0x0000000A, 0x00000158, 0x00000156, 0x00000157, 0x0003003E, 0x00000159, 0x00000158, 0x00050039, 
0x00000007, 0x0000015A, 0x00000016, 0x00000159, 0x0003003E, 0x00000153, 0x0000015A, 0x00040008, 0x00000001, 0x000000D1, 0x00000000, 0x0003003E, 
0x0000015B, 0x0000015C, 0x00040008, 0x00000001, 0x000000D4, 0x00000000, 0x0003003E, 0x0000015D, 0x00000066, 0x000200F9, 0x0000015E, 0x00040008, 
0x00000001, 0x000000D5, 0x00000000, 0x000200F8, 0x0000015E, 0x000400F6, 0x00000160, 0x00000161, 0x00000000, 0x000200F9, 0x00000162, 0x000200F8, 
0x00000162, 0x0004003D, 0x0000005D, 0x00000163, 0x0000015D, 0x000500B3, 0x00000104, 0x00000165, 0x00000163, 0x00000164, 0x000400FA, 0x00000165, 
0x0000015F, 0x00000160, 0x000200F8, 0x0000015F, 0x00040008, 0x00000001, 0x000000D7, 0x00000000, 0x0004003D, 0x00000007, 0x00000166, 0x00000153, 
0x0006000C, 0x00000007, 0x00000167, 0x00000002, 0x00000004, 0x00000166, 0x0004003D, 0x00000007, 0x00000168, 0x0000015B, 0x000500B8, 0x00000104, 
0x00000169, 0x00000167, 0x00000168, 0x0004003D, 0x00000007, 0x0000016A, 0x00000152, 0x000500BA, 0x00000104, 0x0000016C, 0x0000016A, 0x0000016B, 
0x000500A6, 0x00000104, 0x0000016D, 0x00000169, 0x0000016C, 0x000300F7, 0x0000016F, 0x00000000, 0x000400FA, 0x0000016D, 0x0000016E, 0x0000016F, 
0x000200F8, 0x0000016E, 0x00040008, 0x00000001, 0x000000D9, 0x00000000, 0x000200F9, 0x00000160, 0x000200F8, 0x0000016F, 0x00040008, 0x00000001, 
0x000000DC, 0x00000000, 0x0004003D, 0x00000007, 0x00000171, 0x00000152, 0x00050085, 0x00000007, 0x00000172, 0x00000171, 0x0000015C, 0x0003003E, 
0x0000015B, 0x00000172, 0x00040008, 0x00000001, 0x000000DE, 0x00000000, 0x0004003D, 0x00000007, 0x00000174, 0x00000152, 0x00050085, 0x00000007, 
0x00000176, 0x00000174, 0x00000175, 0x0007000C, 0x00000007, 0x00000177, 0x00000002, 0x00000028, 0x00000176, 0x000000D4, 0x0003003E, 0x00000173, 
0x00000177, 0x00040008, 0x00000001, 0x000000DF, 0x00000000, 0x0004003D, 0x00000007, 0x00000178, 0x00000153, 0x0004003D, 0x00000007, 0x00000179, 
0x00000173, 0x00050085, 0x00000007, 0x0000017A, 0x00000178, 0x00000179, 0x0004003D, 0x00000007, 0x0000017B, 0x00000152, 0x00050081, 0x00000007, 
0x0000017C, 0x0000017B, 0x0000017A, 0x0003003E, 0x00000152, 0x0000017C, 0x00040008, 0x00000001, 0x000000E0, 0x00000000, 0x0004003D, 0x0000000A, 
0x0000017D, 0x0000001B, 0x0004003D, 0x00000007, 0x0000017E, 0x00000152, 0x0005008E, 0x0000000A, 0x0000017F, 0x0000017D, 0x0000017E, 0x0004003D, 
0x0000000A, 0x00000180, 0x0000001A, 0x00050081, 0x0000000A, 0x00000181, 0x0000017F, 0x00000180, 0x0003003E, 0x00000182, 0x00000181, 0x00050039, 
0x00000007, 0x00000183, 0x00000016, 0x00000182, 0x00050085, 0x00000007, 0x00000185, 0x00000183, 0x00000184, 0x0003003E, 0x00000153, 0x00000185, 
0x00040008, 0x00000001, 0x000000E1, 0x00000000, 0x0004003D, 0x00000007, 0x00000186, 0x0000001C, 0x00050081, 0x00000007, 0x00000187, 0x00000186, 
0x000000D4, 0x0003003E, 0x0000001C, 0x00000187, 0x000200F9, 0x00000161, 0x000200F8, 0x00000161, 0x00040008, 0x00000001, 0x000000D5, 0x00000000, 
0x0004003D, 0x0000005D, 0x00000188, 0x0000015D, 0x00050080, 0x0000005D, 0x00000189, 0x00000188, 0x00000139, 0x0003003E, 0x0000015D, 0x00000189, 
0x000200F9, 0x0000015E, 0x000200F8, 0x00000160, 0x00040008, 0x00000001, 0x000000E5, 0x00000000, 0x0004003D, 0x00000007, 0x0000018A, 0x00000152, 
0x000200FE, 0x0000018A, 0x00010038, 0x00050036, 0x0000000A, 0x00000022, 0x00000000, 0x0000001F, 0x00030037, 0x00000013, 0x00000020, 0x00030037, 
0x00000018, 0x00000021, 0x000200F8, 0x00000023, 0x0004003B, 0x00000009, 0x0000018D, 0x00000007, 0x0004003B, 0x00000013, 0x0000019A, 0x00000007, 
0x0004003B, 0x00000013, 0x000001A3, 0x00000007, 0x0004003B, 0x00000013, 0x000001AD, 0x00000007, 0x0004003B, 0x00000013, 0x000001B7, 0x00000007, 
0x00040008, 0x00000001, 0x000000EC, 0x00000000, 0x0004003D, 0x00000007, 0x00000191, 0x00000021, 0x0007000C, 0x00000007, 0x00000192, 0x00000002, 
0x0000001A, 0x00000191, 0x000000D4, 0x0005008E, 0x00000008, 0x00000193, 0x00000190, 0x00000192, 0x0003003E, 0x0000018D, 0x00000193, 0x00040008, 
0x00000001, 0x000000ED, 0x00000000, 0x0004003D, 0x00000008, 0x00000194, 0x0000018D, 0x0008004F, 0x0000000A, 0x00000195, 0x00000194, 0x00000194, 
0x00000001, 0x00000000, 0x00000000, 0x0004003D, 0x0000000A, 0x00000196, 0x00000020, 0x0004003D, 0x00000008, 0x00000197, 0x0000018D, 0x0008004F, 
0x0000000A, 0x00000198, 0x00000197, 0x00000197, 0x00000001, 0x00000000, 0x00000000, 0x00050081, 0x0000000A, 0x00000199, 0x00000196, 0x00000198, 
0x0003003E, 0x0000019A, 0x00000199, 0x00050039, 0x00000007, 0x0000019B, 0x00000016, 0x0000019A, 0x0005008E, 0x0000000A, 0x0000019C, 0x00000195, 
0x0000019B, 0x0004003D, 0x00000008, 0x0000019D, 0x0000018D, 0x0008004F, 0x0000000A, 0x0000019E, 0x0000019D, 0x0000019D, 0x00000000, 0x00000000, 
0x00000001, 0x0004003D, 0x0000000A, 0x0000019F, 0x00000020, 0x0004003D, 0x00000008, 0x000001A0, 0x0000018D, 0x0008004F, 0x0000000A, 0x000001A1, 
0x000001A0, 0x000001A0, 0x00000000, 0x00000000, 0x00000001, 0x00050081, 0x0000000A, 0x000001A2, 0x0000019F, 0x000001A1, 0x0003003E, 0x000001A3, 
0x000001A2, 0x00050039, 0x00000007, 0x000001A4, 0x00000016, 0x000001A3, 0x0005008E, 0x0000000A, 0x000001A5, 0x0000019E, 0x000001A4, 0x00050081, 
0x0000000A, 0x000001A6, 0x0000019C, 0x000001A5, 0x0004003D, 0x00000008, 0x000001A7, 0x0000018D, 0x0008004F, 0x0000000A, 0x000001A8, 0x000001A7, 
0x000001A7, 0x00000000, 0x00000001, 0x00000000, 0x0004003D, 0x0000000A, 0x000001A9, 0x00000020, 0x0004003D, 0x00000008, 0x000001AA, 0x0000018D, 
0x0008004F, 0x0000000A, 0x000001AB, 0x000001AA, 0x000001AA, 0x00000000, 0x00000001, 0x00000000, 0x00050081, 0x0000000A, 0x000001AC, 0x000001A9, 
0x000001AB, 0x0003003E, 0x000001AD, 0x000001AC, 0x00050039, 0x00000007, 0x000001AE, 0x00000016, 0x000001AD, 0x0005008E, 0x0000000A, 0x000001AF, 
0x000001A8, 0x000001AE, 0x00050081, 0x0000000A, 0x000001B0, 0x000001A6, 0x000001AF, 0x0004003D, 0x00000008, 0x000001B1, 0x0000018D, 0x0008004F, 
0x0000000A, 0x000001B2, 0x000001B1, 0x000001B1, 0x00000001, 0x00000001, 0x00000001, 0x0004003D, 0x0000000A, 0x000001B3, 0x00000020, 0x0004003D, 
0x00000008, 0x000001B4, 0x0000018D, 0x0008004F, 0x0000000A, 0x000001B5, 0x000001B4, 0x000001B4, 0x00000001, 0x00000001, 0x00000001, 0x00050081, 
0x0000000A, 0x000001B6, 0x000001B3, 0x000001B5, 0x0003003E, 0x000001B7, 0x000001B6, 0x00050039, 0x00000007, 0x000001B8, 0x00000016, 0x000001B7, 
0x0005008E, 0x0000000A, 0x000001B9, 0x000001B2, 0x000001B8, 0x00050081, 0x0000000A, 0x000001BA, 0x000001B0, 0x000001B9, 0x0006000C, 0x0000000A, 
0x000001BB, 0x00000002, 0x00000045, 0x000001BA, 0x000200FE, 0x000001BB, 0x00010038, 0x00050036, 0x00000007, 0x00000025, 0x00000000, 0x0000000F, 
0x00030037, 0x00000009, 0x00000024, 0x000200F8, 0x00000026, 0x00040008, 0x00000001, 0x000000F3, 0x00000000, 0x0004003D, 0x00000089, 0x000001BE, 
0x0000008B, 0x0004003D, 0x00000008, 0x000001BF, 0x00000024, 0x0005008E, 0x00000008, 0x000001C1, 0x000001BF, 0x000001C0, 0x00050057, 0x00000051, 
0x000001C2, 0x000001BE, 0x000001C1, 0x00050051, 0x00000007, 0x000001C3, 0x000001C2, 0x00000000, 0x000200FE, 0x000001C3, 0x00010038, 0x00050036, 
0x00000007, 0x00000028, 0x00000000, 0x0000000F, 0x00030037, 0x00000009, 0x00000027, 0x000200F8, 0x00000029, 0x0004003B, 0x00000018, 0x000001C6, 
0x00000007, 0x0004003B, 0x00000018, 0x000001C7, 0x00000007, 0x0004003B, 0x00000018, 0x000001C8, 0x00000007, 0x0004003B, 0x00000009, 0x000001D0, 
0x00000007, 0x00040008, 0x00000001, 0x000000FA, 0x00000000, 0x0003003E, 0x000001C6, 0x0000008E, 0x00040008, 0x00000001, 0x000000FC, 0x00000000, 
0x0003003E, 0x000001C7, 0x00000094, 0x00040008, 0x00000001, 0x000000FF, 0x00000000, 0x0003003E, 0x000001C8, 0x00000094, 0x000200F9, 0x000001C9, 
0x00040008, 0x00000001, 0x00000100, 0x00000000, 0x000200F8, 0x000001C9, 0x000400F6, 0x000001CB, 0x000001CC, 0x00000000, 0x000200F9, 0x000001CD, 
0x000200F8, 0x000001CD, 0x0004003D, 0x00000007, 0x000001CE, 0x000001C8, 0x000500B8, 0x00000104, 0x000001CF, 0x000001CE, 0x00000080, 0x000400FA, 
0x000001CF, 0x000001CA, 0x000001CB, 0x000200F8, 0x000001CA, 0x00040008, 0x00000001, 0x00000102, 0x00000000, 0x0004003D, 0x00000008, 0x000001D1, 
0x00000027, 0x0003003E, 0x000001D0, 0x000001D1, 0x00050039, 0x00000007, 0x000001D2, 0x00000025, 0x000001D0, 0x00050085, 0x00000007, 0x000001D3, 
0x000001D2, 0x000000FA, 0x0006000C, 0x00000007, 0x000001D4, 0x00000002, 0x0000000D, 0x000001D3, 0x00050085, 0x00000007, 0x000001D5, 0x000001D4, 
0x0000008E, 0x00050081, 0x00000007, 0x000001D6, 0x000001D5, 0x0000008E, 0x0004003D, 0x00000007, 0x000001D7, 0x000001C6, 0x00050085, 0x00000007, 
0x000001D8, 0x000001D6, 0x000001D7, 0x0004003D, 0x00000007, 0x000001D9, 0x000001C7, 0x00050081, 0x00000007, 0x000001DA, 0x000001D9, 0x000001D8, 
0x0003003E, 0x000001C7, 0x000001DA, 0x00040008, 0x00000001, 0x00000103, 0x00000000, 0x0004003D, 0x00000007, 0x000001DB, 0x000001C6, 0x00050085, 
0x00000007, 0x000001DC, 0x000001DB, 0x0000008E, 0x0003003E, 0x000001C6, 0x000001DC, 0x00040008, 0x00000001, 0x00000104, 0x00000000, 0x0004003D, 
0x00000008, 0x000001DD, 0x00000027, 0x0005008E, 0x00000008, 0x000001DE, 0x000001DD, 0x00000081, 0x0003003E, 0x00000027, 0x000001DE, 0x000200F9, 
0x000001CC, 0x000200F8, 0x000001CC, 0x00040008, 0x00000001, 0x00000100, 0x00000000, 0x0004003D, 0x00000007, 0x000001DF, 0x000001C8, 0x00050081, 
0x00000007, 0x000001E0, 0x000001DF, 0x000000D4, 0x0003003E, 0x000001C8, 0x000001E0, 0x000200F9, 0x000001C9, 0x000200F8, 0x000001CB, 0x00040008, 
0x00000001, 0x00000108, 0x00000000, 0x0004003D, 0x00000007, 0x000001E1, 0x000001C7, 0x000200FE, 0x000001E1, 0x00010038, 0x00050036, 0x00000007, 
0x0000002B, 0x00000000, 0x0000000F, 0x00030037, 0x00000009, 0x0000002A, 0x000200F8, 0x0000002C, 0x0004003B, 0x00000009, 0x000001E6, 0x00000007, 
0x00040008, 0x00000001, 0x0000010E, 0x00000000, 0x0004003D, 0x00000008, 0x000001E4, 0x0000002A, 0x0005008E, 0x00000008, 0x000001E5, 0x000001E4, 
0x00000080, 0x0003003E, 0x000001E6, 0x000001E5, 0x00050039, 0x00000007, 0x000001E7, 0x00000028, 0x000001E6, 0x000200FE, 0x000001E7, 0x00010038, 
0x00050036, 0x0000000A, 0x00000031, 0x00000000, 0x0000002D, 0x00030037, 0x00000013, 0x0000002E, 0x00030037, 0x00000013, 0x0000002F, 0x00030037, 
0x00000018, 0x00000030, 0x000200F8, 0x00000032, 0x0004003B, 0x00000009, 0x000001EA, 0x00000007, 0x0004003B, 0x00000018, 0x000001EF, 0x00000007, 
0x0004003B, 0x00000009, 0x000001F0, 0x00000007, 0x0004003B, 0x00000013, 0x000001F4, 0x00000007, 0x0004003B, 0x00000009, 0x000001F9, 0x00000007, 
0x0004003B, 0x00000009, 0x00000202, 0x00000007, 0x00040008, 0x00000001, 0x00000115, 0x00000000, 0x0004003D, 0x00000007, 0x000001EC, 0x00000030, 
0x00050085, 0x00000007, 0x000001ED, 0x000001EB, 0x000001EC, 0x00050050, 0x00000008, 0x000001EE, 0x000001ED, 0x00000094, 0x0003003E, 0x000001EA, 
0x000001EE, 0x00040008, 0x00000001, 0x00000117, 0x00000000, 0x0004003D, 0x0000000A, 0x000001F1, 0x0000002E, 0x0007004F, 0x00000008, 0x000001F2, 
0x000001F1, 0x000001F1, 0x00000002, 0x00000000, 0x0003003E, 0x000001F0, 0x000001F2, 0x00050039, 0x00000007, 0x000001F3, 0x0000002B, 0x000001F0, 
0x0003003E, 0x000001EF, 0x000001F3, 0x00040008, 0x00000001, 0x00000119, 0x00000000, 0x0004003D, 0x0000000A, 0x000001F5, 0x0000002E, 0x0007004F, 
0x00000008, 0x000001F6, 0x000001F5, 0x000001F5, 0x00000002, 0x00000000, 0x0004003D, 0x00000008, 0x000001F7, 0x000001EA, 0x00050081, 0x00000008, 
0x000001F8, 0x000001F6, 0x000001F7, 0x0003003E, 0x000001F9, 0x000001F8, 0x00050039, 0x00000007, 0x000001FA, 0x0000002B, 0x000001F9, 0x0004003D, 
0x00000007, 0x000001FB, 0x000001EF, 0x00050083, 0x00000007, 0x000001FC, 0x000001FA, 0x000001FB, 0x0004003D, 0x0000000A, 0x000001FD, 0x0000002E, 
0x0007004F, 0x00000008, 0x000001FE, 0x000001FD, 0x000001FD, 0x00000002, 0x00000000, 0x0004003D, 0x00000008, 0x000001FF, 0x000001EA, 0x0007004F, 
0x00000008, 0x00000200, 0x000001FF, 0x000001FF, 0x00000001, 0x00000000, 0x00050081, 0x00000008, 0x00000201, 0x000001FE, 0x00000200, 0x0003003E, 
0x00000202, 0x00000201, 0x00050039, 0x00000007, 0x00000203, 0x0000002B, 0x00000202, 0x0004003D, 0x00000007, 0x00000204, 0x000001EF, 0x00050083, 
0x00000007, 0x00000205, 0x00000203, 0x00000204, 0x00060050, 0x0000000A, 0x00000206, 0x000001FC, 0x000000D4, 0x00000205, 0x00050041, 0x00000018, 
0x00000207, 0x000001EA, 0x00000096, 0x0004003D, 0x00000007, 0x00000208, 0x00000207, 0x00060050, 0x0000000A, 0x00000209, 0x00000208, 0x00000208, 
0x00000208, 0x00050088, 0x0000000A, 0x0000020A, 0x00000206, 0x00000209, 0x0005008E, 0x0000000A, 0x0000020C, 0x0000020A, 0x0000020B, 0x0003003E, 
0x000001F4, 0x0000020C, 0x00040008, 0x00000001, 0x0000011A, 0x00000000, 0x0004003D, 0x0000000A, 0x0000020D, 0x0000002F, 0x0004003D, 0x0000000A, 
0x0000020E, 0x0000002F, 0x0004003D, 0x0000000A, 0x0000020F, 0x000001F4, 0x00050094, 0x00000007, 0x00000210, 0x0000020E, 0x0000020F, 0x0005008E, 
0x0000000A, 0x00000211, 0x0000020D, 0x00000210, 0x0004003D, 0x0000000A, 0x00000212, 0x000001F4, 0x00050083, 0x0000000A, 0x00000213, 0x00000212, 
0x00000211, 0x0003003E, 0x000001F4, 0x00000213, 0x00040008, 0x00000001, 0x0000011B, 0x00000000, 0x0004003D, 0x0000000A, 0x00000214, 0x0000002F, 
0x0004003D, 0x0000000A, 0x00000215, 0x000001F4, 0x00050083, 0x0000000A, 0x00000216, 0x00000214, 0x00000215, 0x0006000C, 0x0000000A, 0x00000217, 
0x00000002, 0x00000045, 0x00000216, 0x0003003E, 0x0000002F, 0x00000217, 0x00040008, 0x00000001, 0x0000011C, 0x00000000, 0x0004003D, 0x0000000A, 
0x00000218, 0x0000002F, 0x000200FE, 0x00000218, 0x00010038, 0x00050036, 0x00000007, 0x00000036, 0x00000000, 0x00000033, 0x00030037, 0x00000013, 
0x00000034, 0x00030037, 0x00000018, 0x00000035, 0x000200F8, 0x00000037, 0x0004003B, 0x00000009, 0x0000021B, 0x00000007, 0x0004003B, 0x00000018, 
0x00000220, 0x00000007, 0x0004003B, 0x00000013, 0x00000225, 0x00000007, 0x0004003B, 0x00000018, 0x00000227, 0x00000007, 0x0004003B, 0x00000013, 
0x0000022C, 0x00000007, 0x0004003B, 0x00000018, 0x0000022E, 0x00000007, 0x0004003B, 0x00000013, 0x00000233, 0x00000007, 0x0004003B, 0x00000018, 
0x00000235, 0x00000007, 0x0004003B, 0x00000013, 0x0000023A, 0x00000007, 0x0004003B, 0x00000013, 0x00000248, 0x00000007, 0x00040008, 0x00000001, 
0x00000123, 0x00000000, 0x0004003D, 0x00000007, 0x0000021E, 0x00000035, 0x0005008E, 0x00000008, 0x0000021F, 0x0000021D, 0x0000021E, 0x0003003E, 
0x0000021B, 0x0000021F, 0x00040008, 0x00000001, 0x00000125, 0x00000000, 0x0004003D, 0x0000000A, 0x00000221, 0x00000034, 0x0004003D, 0x00000008, 
0x00000222, 0x0000021B, 0x0008004F, 0x0000000A, 0x00000223, 0x00000222, 0x00000222, 0x00000001, 0x00000000, 0x00000000, 0x00050081, 0x0000000A, 
0x00000224, 0x00000221, 0x00000223, 0x0003003E, 0x00000225, 0x00000224, 0x00050039, 0x00000007, 0x00000226, 0x00000016, 0x00000225, 0x0003003E, 
0x00000220, 0x00000226, 0x00040008, 0x00000001, 0x00000127, 0x00000000, 0x0004003D, 0x0000000A, 0x00000228, 0x00000034, 0x0004003D, 0x00000008, 
0x00000229, 0x0000021B, 0x0008004F, 0x0000000A, 0x0000022A, 0x00000229, 0x00000229, 0x00000000, 0x00000000, 0x00000001, 0x00050081, 0x0000000A, 
0x0000022B, 0x00000228, 0x0000022A, 0x0003003E, 0x0000022C, 0x0000022B, 0x00050039, 0x00000007, 0x0000022D, 0x00000016, 0x0000022C, 0x0003003E, 
0x00000227, 0x0000022D, 0x00040008, 0x00000001, 0x00000129, 0x00000000, 0x0004003D, 0x0000000A, 0x0000022F, 0x00000034, 0x0004003D, 0x00000008, 
0x00000230, 0x0000021B, 0x0008004F, 0x0000000A, 0x00000231, 0x00000230, 0x00000230, 0x00000000, 0x00000001, 0x00000000, 0x00050081, 0x0000000A, 
0x00000232, 0x0000022F, 0x00000231, 0x0003003E, 0x00000233, 0x00000232, 0x00050039, 0x00000007, 0x00000234, 0x00000016, 0x00000233, 0x0003003E, 
0x0000022E, 0x00000234, 0x00040008, 0x00000001, 0x0000012B, 0x00000000, 0x0004003D, 0x0000000A, 0x00000236, 0x00000034, 0x0004003D, 0x00000008, 
0x00000237, 0x0000021B, 0x0008004F, 0x0000000A, 0x00000238, 0x00000237, 0x00000237, 0x00000001, 0x00000001, 0x00000001, 0x00050081, 0x0000000A, 
0x00000239, 0x00000236, 0x00000238, 0x0003003E, 0x0000023A, 0x00000239, 0x00050039, 0x00000007, 0x0000023B, 0x00000016, 0x0000023A, 0x0003003E, 
0x00000235, 0x0000023B, 0x00040008, 0x00000001, 0x0000012C, 0x00000000, 0x00050041, 0x00000018, 0x0000023D, 0x0000021B, 0x00000067, 0x0004003D, 
0x00000007, 0x0000023E, 0x0000023D, 0x00050088, 0x00000007, 0x0000023F, 0x0000023C, 0x0000023E, 0x0004003D, 0x00000007, 0x00000240, 0x00000220, 
0x0004003D, 0x00000007, 0x00000241, 0x00000227, 0x00050081, 0x00000007, 0x00000242, 0x00000240, 0x00000241, 0x0004003D, 0x00000007, 0x00000243, 
0x0000022E, 0x00050081, 0x00000007, 0x00000244, 0x00000242, 0x00000243, 0x0004003D, 0x00000007, 0x00000245, 0x00000235, 0x00050081, 0x00000007, 
0x00000246, 0x00000244, 0x00000245, 0x0004003D, 0x0000000A, 0x00000249, 0x00000034, 0x0003003E, 0x00000248, 0x00000249, 0x00050039, 0x00000007, 
0x0000024A, 0x00000016, 0x00000248, 0x00050085, 0x00000007, 0x0000024B, 0x00000247, 0x0000024A, 0x00050083, 0x00000007, 0x0000024C, 0x00000246, 
0x0000024B, 0x00050085, 0x00000007, 0x0000024D, 0x0000023F, 0x0000024C, 0x000200FE, 0x0000024D, 0x00010038, 0x00050036, 0x0000000A, 0x0000003D, 
0x00000000, 0x00000038, 0x00030037, 0x00000013, 0x00000039, 0x00030037, 0x00000013, 0x0000003A, 0x00030037, 0x00000013, 0x0000003B, 0x00030037, 
0x00000018, 0x0000003C, 0x000200F8, 0x0000003E, 0x0004003B, 0x00000013, 0x00000250, 0x00000007, 0x0004003B, 0x00000018, 0x00000256, 0x00000007, 
0x0004003B, 0x00000009, 0x00000261, 0x00000007, 0x0004003B, 0x00000018, 0x00000266, 0x00000007, 0x0004003B, 0x00000018, 0x0000027A, 0x00000007, 
0x0004003B, 0x00000013, 0x00000282, 0x00000007, 0x00040008, 0x00000001, 0x00000133, 0x00000000, 0x0004003D, 0x0000000A, 0x00000251, 0x00000039, 
0x0004003D, 0x0000000A, 0x00000252, 0x0000003A, 0x0004003D, 0x00000007, 0x00000253, 0x0000003C, 0x0005008E, 0x0000000A, 0x00000254, 0x00000252, 
0x00000253, 0x00050081, 0x0000000A, 0x00000255, 0x00000251, 0x00000254, 0x0003003E, 0x00000250, 0x00000255, 0x00040008, 0x00000001, 0x00000135, 
0x00000000, 0x0004003D, 0x0000000A, 0x00000257, 0x00000250, 0x0007004F, 0x00000008, 0x00000258, 0x00000257, 0x00000257, 0x00000002, 0x00000000, 
0x0005008E, 0x00000008, 0x0000025A, 0x00000258, 0x00000259, 0x00050041, 0x00000068, 0x0000025B, 0x00000065, 0x00000139, 0x0004003D, 0x00000007, 
0x0000025C, 0x0000025B, 0x00050085, 0x00000007, 0x0000025E, 0x0000025C, 0x0000025D, 0x00050050, 0x00000008, 0x0000025F, 0x0000025E, 0x0000025E, 
0x00050083, 0x00000008, 0x00000260, 0x0000025A, 0x0000025F, 0x0003003E, 0x00000261, 0x00000260, 0x00050039, 0x00000007, 0x00000262, 0x00000028, 
0x00000261, 0x00050083, 0x00000007, 0x00000263, 0x00000262, 0x0000008E, 0x00050085, 0x00000007, 0x00000265, 0x00000263, 0x00000264, 0x0003003E, 
0x00000256, 0x00000265, 0x00040008, 0x00000001, 0x00000137, 0x00000000, 0x00050041, 0x00000018, 0x00000268, 0x00000039, 0x00000067, 0x0004003D, 
0x00000007, 0x00000269, 0x00000268, 0x0004007F, 0x00000007, 0x0000026A, 0x00000269, 0x00050085, 0x00000007, 0x0000026B, 0x0000026A, 0x000000D4, 
0x0006000C, 0x00000007, 0x0000026C, 0x00000002, 0x0000001B, 0x0000026B, 0x00050085, 0x00000007, 0x0000026D, 0x00000267, 0x0000026C, 0x0004003D, 
0x00000007, 0x0000026E, 0x0000003C, 0x0004007F, 0x00000007, 0x0000026F, 0x0000026E, 0x00050041, 0x00000018, 0x00000270, 0x0000003A, 0x00000067, 
0x0004003D, 0x00000007, 0x00000271, 0x00000270, 0x00050085, 0x00000007, 0x00000272, 0x0000026F, 0x00000271, 0x00050085, 0x00000007, 0x00000273, 
0x00000272, 0x000000D4, 0x0006000C, 0x00000007, 0x00000274, 0x00000002, 0x0000001B, 0x00000273, 0x00050083, 0x00000007, 0x00000275, 0x000000D4, 
0x00000274, 0x00050085, 0x00000007, 0x00000276, 0x0000026D, 0x00000275, 0x00050041, 0x00000018, 0x00000277, 0x0000003A, 0x00000067, 0x0004003D, 
0x00000007, 0x00000278, 0x00000277, 0x00050088, 0x00000007, 0x00000279, 0x00000276, 0x00000278, 0x0003003E, 0x00000266, 0x00000279, 0x00040008, 
0x00000001, 0x00000139, 0x00000000, 0x0004003D, 0x0000000A, 0x0000027B, 0x0000003A, 0x00050094, 0x00000007, 0x00000280, 0x0000027B, 0x0000027F, 
0x0007000C, 0x00000007, 0x00000281, 0x00000002, 0x00000028, 0x00000280, 0x00000094, 0x0003003E, 0x0000027A, 0x00000281, 0x00040008, 0x00000001, 
0x0000013B, 0x00000000, 0x0004003D, 0x00000007, 0x0000028A, 0x0000027A, 0x0007000C, 0x00000007, 0x0000028B, 0x00000002, 0x0000001A, 0x0000028A, 
0x00000081, 0x0004003D, 0x00000007, 0x0000028C, 0x00000256, 0x00050085, 0x00000007, 0x0000028D, 0x0000028C, 0x0000008E, 0x00050081, 0x00000007, 
0x0000028E, 0x0000028B, 0x0000028D, 0x00060050, 0x0000000A, 0x0000028F, 0x0000028E, 0x0000028E, 0x0000028E, 0x0008000C, 0x0000000A, 0x00000290, 
0x00000002, 0x0000002E, 0x00000285, 0x00000289, 0x0000028F, 0x0003003E, 0x00000282, 0x00000290, 0x00040008, 0x00000001, 0x0000013C, 0x00000000, 
0x0004003D, 0x0000000A, 0x00000291, 0x0000003B, 0x0004003D, 0x0000000A, 0x00000292, 0x00000282, 0x0004003D, 0x00000007, 0x00000293, 0x00000266, 
0x0004003D, 0x00000007, 0x00000294, 0x00000256, 0x00050081, 0x00000007, 0x00000295, 0x00000293, 0x00000294, 0x0008000C, 0x00000007, 0x00000296, 
0x00000002, 0x0000002B, 0x00000295, 0x00000094, 0x000000D4, 0x00060050, 0x0000000A, 0x00000297, 0x00000296, 0x00000296, 0x00000296, 0x0008000C, 
0x0000000A, 0x00000298, 0x00000002, 0x0000002E, 0x00000291, 0x00000292, 0x00000297, 0x000200FE, 0x00000298, 0x00010038, 0x00050036, 0x00000007, 
0x00000043, 0x00000000, 0x0000003F, 0x00030037, 0x00000018, 0x00000040, 0x00030037, 0x00000018, 0x00000041, 0x00030037, 0x00000018, 0x00000042, 
0x000200F8, 0x00000044, 0x00040008, 0x00000001, 0x00000142, 0x00000000, 0x0004003D, 0x00000007, 0x0000029B, 0x00000042, 0x0004003D, 0x00000007, 
0x0000029C, 0x00000040, 0x00050083, 0x00000007, 0x0000029D, 0x0000029B, 0x0000029C, 0x0004003D, 0x00000007, 0x0000029E, 0x00000041, 0x0004003D, 
0x00000007, 0x0000029F, 0x00000040, 0x00050083, 0x00000007, 0x000002A0, 0x0000029E, 0x0000029F, 0x00050088, 0x00000007, 0x000002A1, 0x0000029D, 
0x000002A0, 0x0008000C, 0x00000007, 0x000002A2, 0x00000002, 0x0000002B, 0x000002A1, 0x00000094, 0x000000D4, 0x000200FE, 0x000002A2, 0x00010038, 
0x00050036, 0x0000000A, 0x00000048, 0x00000000, 0x00000045, 0x00030037, 0x00000013, 0x00000046, 0x00030037, 0x00000013, 0x00000047, 0x000200F8, 
0x00000049, 0x0004003B, 0x00000018, 0x000002A5, 0x00000007, 0x0004003B, 0x00000018, 0x000002AB, 0x00000007, 0x0004003B, 0x00000018, 0x000002B5, 
0x00000007, 0x0004003B, 0x00000018, 0x000002B7, 0x00000007, 0x0004003B, 0x00000018, 0x000002B8, 0x00000007, 0x0004003B, 0x00000018, 0x000002B9, 
0x00000007, 0x0004003B, 0x00000018, 0x000002BE, 0x00000007, 0x0004003B, 0x00000018, 0x000002BF, 0x00000007, 0x0004003B, 0x00000018, 0x000002C0, 
0x00000007, 0x0004003B, 0x00000013, 0x000002CA, 0x00000007, 0x00040008, 0x00000001, 0x00000149, 0x00000000, 0x0004003D, 0x0000000A, 0x000002A6, 
0x00000047, 0x00050094, 0x00000007, 0x000002A7, 0x0000027F, 0x000002A6, 0x00050085, 0x00000007, 0x000002A8, 0x000002A7, 0x0000008E, 0x00050081, 
0x00000007, 0x000002A9, 0x000002A8, 0x0000008E, 0x0007000C, 0x00000007, 0x000002AA, 0x00000002, 0x00000028, 0x000002A9, 0x00000094, 0x0003003E, 
0x000002A5, 0x000002AA, 0x00040008, 0x00000001, 0x0000014B, 0x00000000, 0x0004003D, 0x0000000A, 0x000002AD, 0x00000046, 0x0004003D, 0x0000000A, 
0x000002AE, 0x00000047, 0x0005008E, 0x0000000A, 0x000002AF, 0x000002AE, 0x0000016B, 0x00050081, 0x0000000A, 0x000002B0, 0x000002AD, 0x000002AF, 
0x00050051, 0x00000007, 0x000002B1, 0x000002B0, 0x00000001, 0x00050085, 0x00000007, 0x000002B3, 0x000002B1, 0x000002B2, 0x00050083, 0x00000007, 
0x000002B4, 0x000002AC, 0x000002B3, 0x0003003E, 0x000002AB, 0x000002B4, 0x00040008, 0x00000001, 0x0000014D, 0x00000000, 0x0003003E, 0x000002B7, 
0x000002B6, 0x0003003E, 0x000002B8, 0x00000094, 0x0004003D, 0x00000007, 0x000002BA, 0x000002AB, 0x0003003E, 0x000002B9, 0x000002BA, 0x00070039, 
0x00000007, 0x000002BB, 0x00000043, 0x000002B7, 0x000002B8, 0x000002B9, 0x0003003E, 0x000002BE, 0x000002BC, 0x0003003E, 0x000002BF, 0x000002BD, 
0x0004003D, 0x00000007, 0x000002C1, 0x000002AB, 0x0003003E, 0x000002C0, 0x000002C1, 0x00070039, 0x00000007, 0x000002C2, 0x00000043, 0x000002BE, 
0x000002BF, 0x000002C0, 0x00050083, 0x00000007, 0x000002C3, 0x000002BB, 0x000002C2, 0x00050085, 0x00000007, 0x000002C4, 0x000002C3, 0x000000D4, 
0x0003003E, 0x000002B5, 0x000002C4, 0x00040008, 0x00000001, 0x0000014E, 0x00000000, 0x0004003D, 0x00000007, 0x000002C5, 0x000002A5, 0x0007000C, 
0x00000007, 0x000002C7, 0x00000002, 0x0000001A, 0x000002C5, 0x000002C6, 0x0004003D, 0x00000007, 0x000002C8, 0x000002B5, 0x00050085, 0x00000007, 
0x000002C9, 0x000002C8, 0x000002C7, 0x0003003E, 0x000002B5, 0x000002C9, 0x00040008, 0x00000001, 0x00000150, 0x00000000, 0x0003003E, 0x000002CA, 
0x000002CB, 0x00040008, 0x00000001, 0x00000151, 0x00000000, 0x0004003D, 0x00000007, 0x000002CC, 0x000002B5, 0x0007000C, 0x00000007, 0x000002CE, 
0x00000002, 0x0000001A, 0x000002CC, 0x000002CD, 0x0005008E, 0x0000000A, 0x000002D0, 0x000002CF, 0x000002CE, 0x0005008E, 0x0000000A, 0x000002D1, 
0x000002D0, 0x00000080, 0x0004003D, 0x0000000A, 0x000002D2, 0x000002CA, 0x00050081, 0x0000000A, 0x000002D3, 0x000002D2, 0x000002D1, 0x0003003E, 
0x000002CA, 0x000002D3, 0x00040008, 0x00000001, 0x00000152, 0x00000000, 0x0004003D, 0x00000007, 0x000002D4, 0x000002B5, 0x0007000C, 0x00000007, 
0x000002D6, 0x00000002, 0x0000001A, 0x000002D4, 0x000002D5, 0x0005008E, 0x0000000A, 0x000002D9, 0x000002D8, 0x000002D6, 0x0005008E, 0x0000000A, 
0x000002DA, 0x000002D9, 0x00000267, 0x0004003D, 0x0000000A, 0x000002DB, 0x000002CA, 0x00050081, 0x0000000A, 0x000002DC, 0x000002DB, 0x000002DA, 
0x0003003E, 0x000002CA, 0x000002DC, 0x00040008, 0x00000001, 0x00000153, 0x00000000, 0x0004003D, 0x00000007, 0x000002DD, 0x000002B5, 0x0007000C, 
0x00000007, 0x000002DF, 0x00000002, 0x0000001A, 0x000002DD, 0x000002DE, 0x0005008E, 0x0000000A, 0x000002E2, 0x000002E1, 0x000002DF, 0x0005008E, 
0x0000000A, 0x000002E3, 0x000002E2, 0x0000012B, 0x0004003D, 0x0000000A, 0x000002E4, 0x000002CA, 0x00050081, 0x0000000A, 0x000002E5, 0x000002E4, 
0x000002E3, 0x0003003E, 0x000002CA, 0x000002E5, 0x00040008, 0x00000001, 0x00000154, 0x00000000, 0x0004003D, 0x0000000A, 0x000002E6, 0x000002CA, 
0x000200FE, 0x000002E6, 0x00010038, 0x00050036, 0x0000000A, 0x0000004C, 0x00000000, 0x0000004A, 0x00030037, 0x00000013, 0x0000004B, 0x000200F8, 
0x0000004D, 0x00040008, 0x00000001, 0x0000015A, 0x00000000, 0x0004003D, 0x0000000A, 0x000002E9, 0x0000004B, 0x00050085, 0x0000000A, 0x000002EE, 
0x000002E9, 0x000002ED, 0x0006000C, 0x0000000A, 0x000002EF, 0x00000002, 0x0000000A, 0x000002EE, 0x0003003E, 0x0000004B, 0x000002EF, 0x00040008, 
0x00000001, 0x0000015B, 0x00000000, 0x0004003D, 0x0000000A, 0x000002F0, 0x0000004B, 0x0008004F, 0x0000000A, 0x000002F1, 0x000002F0, 0x000002F0, 
0x00000002, 0x00000000, 0x00000001, 0x0004003D, 0x0000000A, 0x000002F2, 0x0000004B, 0x0008004F, 0x0000000A, 0x000002F3, 0x000002F2, 0x000002F2, 
0x00000001, 0x00000000, 0x00000002, 0x00060050, 0x0000000A, 0x000002F5, 0x000002F4, 0x000002F4, 0x000002F4, 0x00050081, 0x0000000A, 0x000002F6, 
0x000002F3, 0x000002F5, 0x00050094, 0x00000007, 0x000002F7, 0x000002F1, 0x000002F6, 0x0004003D, 0x0000000A, 0x000002F8, 0x0000004B, 0x00060050, 
0x0000000A, 0x000002F9, 0x000002F7, 0x000002F7, 0x000002F7, 0x00050081, 0x0000000A, 0x000002FA, 0x000002F8, 0x000002F9, 0x0003003E, 0x0000004B, 
0x000002FA, 0x00040008, 0x00000001, 0x0000015C, 0x00000000, 0x00050041, 0x00000018, 0x000002FB, 0x0000004B, 0x00000096, 0x0004003D, 0x00000007, 
0x000002FC, 0x000002FB, 0x00050041, 0x00000018, 0x000002FD, 0x0000004B, 0x00000067, 0x0004003D, 0x00000007, 0x000002FE, 0x000002FD, 0x00050085, 
0x00000007, 0x000002FF, 0x000002FC, 0x000002FE, 0x00050041, 0x00000018, 0x00000301, 0x0000004B, 0x00000300, 0x0004003D, 0x00000007, 0x00000302, 
0x00000301, 0x00050041, 0x00000018, 0x00000303, 0x0000004B, 0x00000096, 0x0004003D, 0x00000007, 0x00000304, 0x00000303, 0x00050085, 0x00000007, 
0x00000305, 0x00000302, 0x00000304, 0x00050041, 0x00000018, 0x00000306, 0x0000004B, 0x00000067, 0x0004003D, 0x00000007, 0x00000307, 0x00000306, 
0x00050041, 0x00000018, 0x00000308, 0x0000004B, 0x00000300, 0x0004003D, 0x00000007, 0x00000309, 0x00000308, 0x00050085, 0x00000007, 0x0000030A, 
0x00000307, 0x00000309, 0x00060050, 0x0000000A, 0x0000030B, 0x000002FF, 0x00000305, 0x0000030A, 0x0006000C, 0x0000000A, 0x0000030C, 0x00000002, 
0x0000000A, 0x0000030B, 0x000200FE, 0x0000030C, 0x00010038, 0x00050036, 0x0000000A, 0x0000004F, 0x00000000, 0x0000004A, 0x00030037, 0x00000013, 
0x0000004E, 0x000200F8, 0x00000050, 0x0004003B, 0x00000013, 0x0000030F, 0x00000007, 0x0004003B, 0x00000018, 0x00000310, 0x00000007, 0x0004003B, 
0x00000018, 0x00000314, 0x00000007, 0x0004003B, 0x00000013, 0x0000031C, 0x00000007, 0x0004003B, 0x00000013, 0x00000324, 0x00000007, 0x0004003B, 
0x00000009, 0x0000032A, 0x00000007, 0x0004003B, 0x00000013, 0x0000032B, 0x00000007, 0x0004003B, 0x00000018, 0x0000032F, 0x00000007, 0x00040008, 
0x00000001, 0x00000163, 0x00000000, 0x0003003E, 0x0000030F, 0x000002CB, 0x00040008, 0x00000001, 0x00000165, 0x00000000, 0x00060041, 0x00000068, 
0x00000311, 0x00000065, 0x00000066, 0x00000096, 0x0004003D, 0x00000007, 0x00000312, 0x00000311, 0x00050085, 0x00000007, 0x00000313, 0x00000312, 
0x0000012B, 0x0003003E, 0x00000310, 0x00000313, 0x00040008, 0x00000001, 0x00000168, 0x00000000, 0x0003003E, 0x00000314, 0x00000094, 0x000200F9, 
0x00000315, 0x00040008, 0x00000001, 0x00000169, 0x00000000, 0x000200F8, 0x00000315, 0x000400F6, 0x00000317, 0x00000318, 0x00000000, 0x000200F9, 
0x00000319, 0x000200F8, 0x00000319, 0x0004003D, 0x00000007, 0x0000031A, 0x00000314, 0x000500B8, 0x00000104, 0x0000031B, 0x0000031A, 0x00000080, 
0x000400FA, 0x0000031B, 0x00000316, 0x00000317, 0x000200F8, 0x00000316, 0x00040008, 0x00000001, 0x0000016C, 0x00000000, 0x0004003D, 0x0000000A, 
0x0000031D, 0x0000004E, 0x0004003D, 0x00000007, 0x0000031E, 0x00000310, 0x00050085, 0x00000007, 0x0000031F, 0x0000023C, 0x0000031E, 0x0005008E, 
0x0000000A, 0x00000320, 0x0000031D, 0x0000031F, 0x0006000C, 0x0000000A, 0x00000321, 0x00000002, 0x0000000A, 0x00000320, 0x00060050, 0x0000000A, 
0x00000322, 0x0000008E, 0x0000008E, 0x0000008E, 0x00050083, 0x0000000A, 0x00000323, 0x00000321, 0x00000322, 0x0003003E, 0x0000031C, 0x00000323, 
0x00040008, 0x00000001, 0x0000016E, 0x00000000, 0x0004003D, 0x0000000A, 0x00000325, 0x0000004E, 0x0004003D, 0x00000007, 0x00000326, 0x00000310, 
0x00050085, 0x00000007, 0x00000327, 0x0000023C, 0x00000326, 0x0005008E, 0x0000000A, 0x00000328, 0x00000325, 0x00000327, 0x0006000C, 0x0000000A, 
0x00000329, 0x00000002, 0x00000008, 0x00000328, 0x0003003E, 0x00000324, 0x00000329, 0x00040008, 0x00000001, 0x00000170, 0x00000000, 0x0004003D, 
0x0000000A, 0x0000032C, 0x00000324, 0x0003003E, 0x0000032B, 0x0000032C, 0x00050039, 0x0000000A, 0x0000032D, 0x0000004C, 0x0000032B, 0x0007004F, 
0x00000008, 0x0000032E, 0x0000032D, 0x0000032D, 0x00000000, 0x00000001, 0x0003003E, 0x0000032A, 0x0000032E, 0x00040008, 0x00000001, 0x00000172, 
0x00000000, 0x0004003D, 0x0000000A, 0x00000330, 0x0000031C, 0x0006000C, 0x00000007, 0x00000331, 0x00000002, 0x00000042, 0x00000330, 0x0008000C, 
0x00000007, 0x00000332, 0x00000002, 0x00000031, 0x00000094, 0x0000012F, 0x00000331, 0x00050083, 0x00000007, 0x00000333, 0x000000D4, 0x00000332, 
0x0003003E, 0x0000032F, 0x00000333, 0x00040008, 0x00000001, 0x00000173, 0x00000000, 0x00050041, 0x00000018, 0x00000334, 0x0000032A, 0x00000096, 
0x0004003D, 0x00000007, 0x00000335, 0x00000334, 0x0004003D, 0x00000007, 0x00000336, 0x00000314, 0x0004003D, 0x00000007, 0x00000337, 0x00000314, 
0x00050085, 0x00000007, 0x00000338, 0x00000336, 0x00000337, 0x00050085, 0x00000007, 0x0000033A, 0x00000338, 0x00000339, 0x00050081, 0x00000007, 
0x0000033B, 0x0000018F, 0x0000033A, 0x0007000C, 0x00000007, 0x0000033C, 0x00000002, 0x00000030, 0x00000335, 0x0000033B, 0x0004003D, 0x00000007, 
0x0000033D, 0x0000032F, 0x00050085, 0x00000007, 0x0000033E, 0x0000033D, 0x0000033C, 0x0003003E, 0x0000032F, 0x0000033E, 0x00040008, 0x00000001, 
0x00000174, 0x00000000, 0x0004003D, 0x00000007, 0x0000033F, 0x0000032F, 0x00050041, 0x00000018, 0x00000344, 0x0000032A, 0x00000067, 0x0004003D, 
0x00000007, 0x00000345, 0x00000344, 0x00060050, 0x0000000A, 0x00000346, 0x00000345, 0x00000345, 0x00000345, 0x0008000C, 0x0000000A, 0x00000347, 
0x00000002, 0x0000002E, 0x00000341, 0x00000343, 0x00000346, 0x0005008E, 0x0000000A, 0x00000348, 0x00000347, 0x000002D7, 0x00060050, 0x0000000A, 
0x00000349, 0x0000013D, 0x0000013D, 0x0000013D, 0x00050081, 0x0000000A, 0x0000034A, 0x00000348, 0x00000349, 0x0005008E, 0x0000000A, 0x0000034B, 
0x0000034A, 0x0000033F, 0x0004003D, 0x0000000A, 0x0000034C, 0x0000030F, 0x00050081, 0x0000000A, 0x0000034D, 0x0000034C, 0x0000034B, 0x0003003E, 
0x0000030F, 0x0000034D, 0x00040008, 0x00000001, 0x00000175, 0x00000000, 0x0004003D, 0x0000000A, 0x0000034F, 0x0000004E, 0x0005008E, 0x0000000A, 
0x00000350, 0x0000034F, 0x0000034E, 0x0003003E, 0x0000004E, 0x00000350, 0x000200F9, 0x00000318, 0x000200F8, 0x00000318, 0x00040008, 0x00000001, 
0x00000169, 0x00000000, 0x0004003D, 0x00000007, 0x00000351, 0x00000314, 0x00050081, 0x00000007, 0x00000352, 0x00000351, 0x000000D4, 0x0003003E, 
0x00000314, 0x00000352, 0x000200F9, 0x00000315, 0x000200F8, 0x00000317, 0x00040008, 0x00000001, 0x00000179, 0x00000000, 0x0004003D, 0x0000000A, 
0x00000353, 0x0000030F, 0x0004003D, 0x0000000A, 0x00000354, 0x0000030F, 0x00050085, 0x0000000A, 0x00000355, 0x00000353, 0x00000354, 0x0005008E, 
0x0000000A, 0x00000356, 0x00000355, 0x00000184, 0x000200FE, 0x00000356, 0x00010038, 0x00050036, 0x00000003, 0x00000056, 0x00000000, 0x00000053, 
0x00030037, 0x00000052, 0x00000054, 0x00030037, 0x00000009, 0x00000055, 0x000200F8, 0x00000057, 0x0004003B, 0x00000009, 0x00000359, 0x00000007, 
0x0004003B, 0x00000009, 0x00000360, 0x00000007, 0x0004003B, 0x00000009, 0x0000036D, 0x00000007, 0x0004003B, 0x00000009, 0x00000378, 0x00000007, 
0x0004003B, 0x00000013, 0x000003A0, 0x00000007, 0x0004003B, 0x00000013, 0x000003AD, 0x00000007, 0x0004003B, 0x00000013, 0x000003B9, 0x00000007, 
0x0004003B, 0x00000013, 0x000003C5, 0x00000007, 0x0004003B, 0x00000013, 0x000003CA, 0x00000007, 0x0004003B, 0x00000018, 0x000003E7, 0x00000007, 
0x0004003B, 0x00000018, 0x000003E8, 0x00000007, 0x0004003B, 0x00000013, 0x000003E9, 0x00000007, 0x0004003B, 0x00000013, 0x000003EB, 0x00000007, 
0x0004003B, 0x00000018, 0x000003ED, 0x00000007, 0x0004003B, 0x00000013, 0x000003F0, 0x00000007, 0x0004003B, 0x00000013, 0x000003F1, 0x00000007, 
0x0004003B, 0x00000013, 0x000003F3, 0x00000007, 0x0004003B, 0x00000013, 0x000003F6, 0x00000007, 0x0004003B, 0x00000013, 0x000003F7, 0x00000007, 
0x0004003B, 0x00000013, 0x00000401, 0x00000007, 0x0004003B, 0x00000013, 0x00000403, 0x00000007, 0x0004003B, 0x00000013, 0x00000409, 0x00000007, 
0x0004003B, 0x00000013, 0x0000040A, 0x00000007, 0x0004003B, 0x00000018, 0x0000040C, 0x00000007, 0x0004003B, 0x00000013, 0x00000413, 0x00000007, 
0x0004003B, 0x00000013, 0x00000415, 0x00000007, 0x0004003B, 0x00000018, 0x00000417, 0x00000007, 0x0004003B, 0x00000018, 0x0000041A, 0x00000007, 
0x0004003B, 0x00000018, 0x00000420, 0x00000007, 0x0004003B, 0x00000018, 0x00000424, 0x00000007, 0x0004003B, 0x00000018, 0x0000042B, 0x00000007, 
0x0004003B, 0x00000018, 0x00000432, 0x00000007, 0x0004003B, 0x00000013, 0x00000439, 0x00000007, 0x0004003B, 0x00000018, 0x0000044D, 0x00000007, 
0x0004003B, 0x00000013, 0x0000044E, 0x00000007, 0x0004003B, 0x00000018, 0x00000450, 0x00000007, 0x0004003B, 0x00000018, 0x00000453, 0x00000007, 
0x0004003B, 0x00000013, 0x00000454, 0x00000007, 0x0004003B, 0x00000018, 0x00000456, 0x00000007, 0x0004003B, 0x00000013, 0x00000478, 0x00000007, 
0x0004003B, 0x00000013, 0x0000047A, 0x00000007, 0x0004003B, 0x00000013, 0x0000047C, 0x00000007, 0x0004003B, 0x00000018, 0x0000047E, 0x00000007, 
0x0004003B, 0x00000052, 0x000004AA, 0x00000007, 0x0004003B, 0x00000018, 0x000004AF, 0x00000007, 0x00040008, 0x00000001, 0x00000180, 0x00000000, 
0x0004003D, 0x00000008, 0x0000035A, 0x00000055, 0x00050041, 0x0000035B, 0x0000035C, 0x00000065, 0x00000066, 0x0004003D, 0x00000051, 0x0000035D, 
0x0000035C, 0x0007004F, 0x00000008, 0x0000035E, 0x0000035D, 0x0000035D, 0x00000000, 0x00000001, 0x00050088, 0x00000008, 0x0000035F, 0x0000035A, 
0x0000035E, 0x0003003E, 0x00000359, 0x0000035F, 0x00040008, 0x00000001, 0x00000182, 0x00000000, 0x0004003D, 0x00000008, 0x00000361, 0x00000359, 
0x00050050, 0x00000008, 0x00000362, 0x0000008E, 0x0000008E, 0x00050083, 0x00000008, 0x00000363, 0x00000361, 0x00000362, 0x0003003E, 0x00000360, 
0x00000363, 0x00040008, 0x00000001, 0x00000183, 0x00000000, 0x00060041, 0x00000068, 0x00000364, 0x00000065, 0x00000066, 0x00000096, 0x0004003D, 
0x00000007, 0x00000365, 0x00000364, 0x00060041, 0x00000068, 0x00000366, 0x00000065, 0x00000066, 0x00000067, 0x0004003D, 0x00000007, 0x00000367, 
0x00000366, 0x00050088, 0x00000007, 0x00000368, 0x00000365, 0x00000367, 0x00050041, 0x00000018, 0x00000369, 0x00000360, 0x00000096, 0x0004003D, 
0x00000007, 0x0000036A, 0x00000369, 0x00050085, 0x00000007, 0x0000036B, 0x0000036A, 0x00000368, 0x00050041, 0x00000018, 0x0000036C, 0x00000360, 
0x00000096, 0x0003003E, 0x0000036C, 0x0000036B, 0x00040008, 0x00000001, 0x00000185, 0x00000000, 0x00050041, 0x0000035B, 0x0000036F, 0x00000065, 
0x0000036E, 0x0004003D, 0x00000051, 0x00000370, 0x0000036F, 0x0007004F, 0x00000008, 0x00000371, 0x00000370, 0x00000370, 0x00000000, 0x00000001, 
0x00050041, 0x0000035B, 0x00000372, 0x00000065, 0x00000066, 0x0004003D, 0x00000051, 0x00000373, 0x00000372, 0x0007004F, 0x00000008, 0x00000374, 
0x00000373, 0x00000373, 0x00000000, 0x00000001, 0x00050088, 0x00000008, 0x00000375, 0x00000371, 0x00000374, 0x00050050, 0x00000008, 0x00000376, 
0x0000008E, 0x0000008E, 0x00050083, 0x00000008, 0x00000377, 0x00000375, 0x00000376, 0x0003003E, 0x0000036D, 0x00000377, 0x00040008, 0x00000001, 
0x00000186, 0x00000000, 0x0004003D, 0x00000008, 0x00000379, 0x0000036D, 0x000500B4, 0x0000037B, 0x0000037C, 0x00000379, 0x0000037A, 0x0004009B, 
0x00000104, 0x0000037D, 0x0000037C, 0x000300F7, 0x0000037F, 0x00000000, 0x000400FA, 0x0000037D, 0x0000037E, 0x00000382, 0x000200F8, 0x0000037E, 
0x0003003E, 0x0000036D, 0x00000381, 0x0003003E, 0x00000378, 0x00000381, 0x000200F9, 0x0000037F, 0x000200F8, 0x00000382, 0x0004003D, 0x00000008, 
0x00000383, 0x0000036D, 0x0003003E, 0x00000378, 0x00000383, 0x000200F9, 0x0000037F, 0x000200F8, 0x0000037F, 0x0004003D, 0x00000008, 0x00000384, 
0x00000378, 0x0003003E, 0x0000036D, 0x00000384, 0x00040008, 0x00000001, 0x00000187, 0x00000000, 0x00050041, 0x00000018, 0x00000386, 0x0000036D, 
0x00000096, 0x0004003D, 0x00000007, 0x00000387, 0x00000386, 0x00050085, 0x00000007, 0x00000388, 0x00000387, 0x00000385, 0x00050041, 0x00000018, 
0x00000389, 0x0000036D, 0x00000096, 0x0003003E, 0x00000389, 0x00000388, 0x00040008, 0x00000001, 0x00000188, 0x00000000, 0x0004003D, 0x00000008, 
0x0000038C, 0x0000036D, 0x00050083, 0x00000008, 0x0000038D, 0x0000038C, 0x0000038B, 0x0003003E, 0x0000036D, 0x0000038D, 0x00040008, 0x00000001, 
0x00000189, 0x00000000, 0x00060041, 0x00000068, 0x0000038E, 0x00000065, 0x00000066, 0x00000096, 0x0004003D, 0x00000007, 0x0000038F, 0x0000038E, 
0x00060041, 0x00000068, 0x00000390, 0x00000065, 0x00000066, 0x00000067, 0x0004003D, 0x00000007, 0x00000391, 0x00000390, 0x00050088, 0x00000007, 
0x00000392, 0x0000038F, 0x00000391, 0x00050041, 0x00000018, 0x00000393, 0x0000036D, 0x00000096, 0x0004003D, 0x00000007, 0x00000394, 0x00000393, 
0x00050085, 0x00000007, 0x00000395, 0x00000394, 0x00000392, 0x00050041, 0x00000018, 0x00000396, 0x0000036D, 0x00000096, 0x0003003E, 0x00000396, 
0x00000395, 0x00040008, 0x00000001, 0x0000018A, 0x00000000, 0x00050041, 0x00000068, 0x00000397, 0x00000065, 0x00000139, 0x0004003D, 0x00000007, 
0x00000398, 0x00000397, 0x00050085, 0x00000007, 0x00000399, 0x00000398, 0x0000023C, 0x0006000C, 0x00000007, 0x0000039A, 0x00000002, 0x0000000D, 
0x00000399, 0x00050085, 0x00000007, 0x0000039B, 0x0000039A, 0x00000264, 0x00050041, 0x00000018, 0x0000039C, 0x0000036D, 0x00000096, 0x0004003D, 
0x00000007, 0x0000039D, 0x0000039C, 0x00050081, 0x00000007, 0x0000039E, 0x0000039D, 0x0000039B, 0x00050041, 0x00000018, 0x0000039F, 0x0000036D, 
0x00000096, 0x0003003E, 0x0000039F, 0x0000039E, 0x00040008, 0x00000001, 0x0000018C, 0x00000000, 0x00050041, 0x00000068, 0x000003A2, 0x00000065, 
0x00000139, 0x0004003D, 0x00000007, 0x000003A3, 0x000003A2, 0x00050085, 0x00000007, 0x000003A4, 0x000003A3, 0x00000264, 0x0006000C, 0x00000007, 
0x000003A5, 0x00000002, 0x0000000D, 0x000003A4, 0x00050085, 0x00000007, 0x000003A6, 0x000003A5, 0x000002D7, 0x00050081, 0x00000007, 0x000003A8, 
0x000003A6, 0x000003A7, 0x00050041, 0x00000068, 0x000003A9, 0x00000065, 0x00000139, 0x0004003D, 0x00000007, 0x000003AA, 0x000003A9, 0x0004007F, 
0x00000007, 0x000003AB, 0x000003AA, 0x00060050, 0x0000000A, 0x000003AC, 0x000003A1, 0x000003A8, 0x000003AB, 0x0003003E, 0x000003A0, 0x000003AC, 
0x00040008, 0x00000001, 0x0000018E, 0x00000000, 0x00050041, 0x00000018, 0x000003AE, 0x0000036D, 0x00000096, 0x0004003D, 0x00000007, 0x000003AF, 
0x000003AE, 0x0006000C, 0x00000007, 0x000003B0, 0x00000002, 0x0000000E, 0x000003AF, 0x00050041, 0x00000018, 0x000003B1, 0x0000036D, 0x00000067, 
0x0004003D, 0x00000007, 0x000003B2, 0x000003B1, 0x00050081, 0x00000007, 0x000003B3, 0x00000112, 0x000003B2, 0x00050041, 0x00000018, 0x000003B4, 
0x0000036D, 0x00000096, 0x0004003D, 0x00000007, 0x000003B5, 0x000003B4, 0x0006000C, 0x00000007, 0x000003B6, 0x00000002, 0x0000000D, 0x000003B5, 
0x00060050, 0x0000000A, 0x000003B7, 0x000003B0, 0x000003B3, 0x000003B6, 0x0006000C, 0x0000000A, 0x000003B8, 0x00000002, 0x00000045, 0x000003B7, 
0x0003003E, 0x000003AD, 0x000003B8, 0x00040008, 0x00000001, 0x00000190, 0x00000000, 0x00050041, 0x00000018, 0x000003BA, 0x0000036D, 0x00000096, 
0x0004003D, 0x00000007, 0x000003BB, 0x000003BA, 0x00050081, 0x00000007, 0x000003BD, 0x000003BB, 0x000003BC, 0x0006000C, 0x00000007, 0x000003BE, 
0x00000002, 0x0000000E, 0x000003BD, 0x00050041, 0x00000018, 0x000003BF, 0x0000036D, 0x00000096, 0x0004003D, 0x00000007, 0x000003C0, 0x000003BF, 
0x00050081, 0x00000007, 0x000003C1, 0x000003C0, 0x000003BC, 0x0006000C, 0x00000007, 0x000003C2, 0x00000002, 0x0000000D, 0x000003C1, 0x00060050, 
0x0000000A, 0x000003C3, 0x000003BE, 0x00000094, 0x000003C2, 0x0006000C, 0x0000000A, 0x000003C4, 0x00000002, 0x00000045, 0x000003C3, 0x0003003E, 
0x000003B9, 0x000003C4, 0x00040008, 0x00000001, 0x00000192, 0x00000000, 0x0004003D, 0x0000000A, 0x000003C6, 0x000003B9, 0x0004003D, 0x0000000A, 
0x000003C7, 0x000003AD, 0x0007000C, 0x0000000A, 0x000003C8, 0x00000002, 0x00000044, 0x000003C6, 0x000003C7, 0x0006000C, 0x0000000A, 0x000003C9, 
0x00000002, 0x00000045, 0x000003C8, 0x0003003E, 0x000003C5, 0x000003C9, 0x00040008, 0x00000001, 0x00000194, 0x00000000, 0x00050041, 0x00000018, 
0x000003CB, 0x00000360, 0x00000096, 0x0004003D, 0x00000007, 0x000003CC, 0x000003CB, 0x0004003D, 0x0000000A, 0x000003CD, 0x000003B9, 0x0005008E, 
0x0000000A, 0x000003CE, 0x000003CD, 0x000003CC, 0x00050041, 0x00000018, 0x000003CF, 0x00000360, 0x00000067, 0x0004003D, 0x00000007, 0x000003D0, 
0x000003CF, 0x0004003D, 0x0000000A, 0x000003D1, 0x000003C5, 0x0005008E, 0x0000000A, 0x000003D2, 0x000003D1, 0x000003D0, 0x00050081, 0x0000000A, 
0x000003D3, 0x000003CE, 0x000003D2, 0x0005008E, 0x0000000A, 0x000003D5, 0x000003D3, 0x000003D4, 0x0004003D, 0x0000000A, 0x000003D6, 0x000003AD, 
0x00050081, 0x0000000A, 0x000003D7, 0x000003D5, 0x000003D6, 0x0006000C, 0x0000000A, 0x000003D8, 0x00000002, 0x00000045, 0x000003D7, 0x0003003E, 
0x000003CA, 0x000003D8, 0x00040008, 0x00000001, 0x00000195, 0x00000000, 0x00050041, 0x00000018, 0x000003D9, 0x00000360, 0x00000096, 0x0004003D, 
0x00000007, 0x000003DA, 0x000003D9, 0x00050041, 0x00000018, 0x000003DB, 0x00000360, 0x00000096, 0x0004003D, 0x00000007, 0x000003DC, 0x000003DB, 
0x00050085, 0x00000007, 0x000003DD, 0x000003DA, 0x000003DC, 0x00050085, 0x00000007, 0x000003DF, 0x000003DD, 0x000003DE, 0x0006000C, 0x00000007, 
0x000003E0, 0x00000002, 0x00000004, 0x000003DF, 0x00050041, 0x00000018, 0x000003E1, 0x000003CA, 0x00000067, 0x0004003D, 0x00000007, 0x000003E2, 
0x000003E1, 0x00050081, 0x00000007, 0x000003E3, 0x000003E2, 0x000003E0, 0x00050041, 0x00000018, 0x000003E4, 0x000003CA, 0x00000067, 0x0003003E, 
0x000003E4, 0x000003E3, 0x00040008, 0x00000001, 0x00000196, 0x00000000, 0x0004003D, 0x0000000A, 0x000003E5, 0x000003CA, 0x0006000C, 0x0000000A, 
0x000003E6, 0x00000002, 0x00000045, 0x000003E5, 0x0003003E, 0x000003CA, 0x000003E6, 0x00040008, 0x00000001, 0x00000198, 0x00000000, 0x0003003E, 
0x000003E7, 0x00000094, 0x00040008, 0x00000001, 0x0000019A, 0x00000000, 0x0004003D, 0x0000000A, 0x000003EA, 0x000003A0, 0x0003003E, 0x000003E9, 
0x000003EA, 0x0004003D, 0x0000000A, 0x000003EC, 0x000003CA, 0x0003003E, 0x000003EB, 0x000003EC, 0x00070039, 0x00000007, 0x000003EE, 0x0000001D, 
0x000003E9, 0x000003EB, 0x000003ED, 0x0004003D, 0x00000007, 0x000003EF, 0x000003ED, 0x0003003E, 0x000003E7, 0x000003EF, 0x0003003E, 0x000003E8, 
0x000003EE, 0x00040008, 0x00000001, 0x0000019C, 0x00000000, 0x0004003D, 0x0000000A, 0x000003F2, 0x000003A0, 0x0003003E, 0x000003F1, 0x000003F2, 
0x0004003D, 0x0000000A, 0x000003F4, 0x000003CA, 0x0003003E, 0x000003F3, 0x000003F4, 0x00060039, 0x0000000A, 0x000003F5, 0x00000048, 0x000003F1, 
0x000003F3, 0x0003003E, 0x000003F0, 0x000003F5, 0x00040008, 0x00000001, 0x0000019E, 0x00000000, 0x0004003D, 0x0000000A, 0x000003F8, 0x000003CA, 
0x0003003E, 0x000003F7, 0x000003F8, 0x00050039, 0x0000000A, 0x000003F9, 0x0000004F, 0x000003F7, 0x0004003D, 0x0000000A, 0x000003FA, 0x000003F0, 
0x00050094, 0x00000007, 0x000003FD, 0x000003FA, 0x000003FC, 0x0008000C, 0x00000007, 0x000003FE, 0x00000002, 0x0000002B, 0x000003FD, 0x00000094, 
0x000000D4, 0x00050083, 0x00000007, 0x000003FF, 0x000000D4, 0x000003FE, 0x0005008E, 0x0000000A, 0x00000400, 0x000003F9, 0x000003FF, 0x0003003E, 
0x000003F6, 0x00000400, 0x00040008, 0x00000001, 0x000001A0, 0x00000000, 0x0004003D, 0x0000000A, 0x00000402, 0x000003F6, 0x0003003E, 0x00000401, 
0x00000402, 0x00040008, 0x00000001, 0x000001A2, 0x00000000, 0x0004003D, 0x0000000A, 0x00000404, 0x000003A0, 0x0004003D, 0x00000007, 0x00000405, 
0x000003E8, 0x0004003D, 0x0000000A, 0x00000406, 0x000003CA, 0x0005008E, 0x0000000A, 0x00000407, 0x00000406, 0x00000405, 0x00050081, 0x0000000A, 
0x00000408, 0x00000404, 0x00000407, 0x0003003E, 0x00000403, 0x00000408, 0x00040008, 0x00000001, 0x000001A4, 0x00000000, 0x0004003D, 0x0000000A, 
0x0000040B, 0x00000403, 0x0003003E, 0x0000040A, 0x0000040B, 0x0004003D, 0x00000007, 0x0000040D, 0x000003E8, 0x0003003E, 0x0000040C, 0x0000040D, 
0x00060039, 0x0000000A, 0x0000040E, 0x00000022, 0x0000040A, 0x0000040C, 0x0003003E, 0x00000409, 0x0000040E, 0x00040008, 0x00000001, 0x000001A5, 
0x00000000, 0x0004003D, 0x00000007, 0x0000040F, 0x000003E8, 0x000500B8, 0x00000104, 0x00000410, 0x0000040F, 0x0000016B, 0x000300F7, 0x00000412, 
0x00000000, 0x000400FA, 0x00000410, 0x00000411, 0x00000412, 0x000200F8, 0x00000411, 0x00040008, 0x00000001, 0x000001A7, 0x00000000, 0x0004003D, 
0x0000000A, 0x00000414, 0x00000403, 0x0003003E, 0x00000413, 0x00000414, 0x0004003D, 0x0000000A, 0x00000416, 0x00000409, 0x0003003E, 0x00000415, 
0x00000416, 0x0004003D, 0x00000007, 0x00000418, 0x000003E8, 0x0003003E, 0x00000417, 0x00000418, 0x00070039, 0x0000000A, 0x00000419, 0x00000031, 
0x00000413, 0x00000415, 0x00000417, 0x0003003E, 0x00000409, 0x00000419, 0x00040008, 0x00000001, 0x000001A9, 0x00000000, 0x00050041, 0x00000018, 
0x0000041B, 0x00000409, 0x00000067, 0x0004003D, 0x00000007, 0x0000041C, 0x0000041B, 0x00050085, 0x00000007, 0x0000041D, 0x0000008E, 0x0000041C, 
0x00050081, 0x00000007, 0x0000041E, 0x0000008E, 0x0000041D, 0x0008000C, 0x00000007, 0x0000041F, 0x00000002, 0x0000002B, 0x0000041E, 0x00000094, 
0x000000D4, 0x0003003E, 0x0000041A, 0x0000041F, 0x00040008, 0x00000001, 0x000001AB, 0x00000000, 0x0004003D, 0x0000000A, 0x00000421, 0x00000409, 
0x00050094, 0x00000007, 0x00000422, 0x00000421, 0x0000027F, 0x0008000C, 0x00000007, 0x00000423, 0x00000002, 0x0000002B, 0x00000422, 0x00000094, 
0x000000D4, 0x0003003E, 0x00000420, 0x00000423, 0x00040008, 0x00000001, 0x000001AD, 0x00000000, 0x0004003D, 0x0000000A, 0x00000425, 0x00000409, 
0x00050094, 0x00000007, 0x00000429, 0x00000425, 0x00000428, 0x0008000C, 0x00000007, 0x0000042A, 0x00000002, 0x0000002B, 0x00000429, 0x00000094, 
0x000000D4, 0x0003003E, 0x00000424, 0x0000042A, 0x00040008, 0x00000001, 0x000001AF, 0x00000000, 0x0004003D, 0x0000000A, 0x0000042C, 0x000003CA, 
0x0004003D, 0x0000000A, 0x0000042D, 0x00000409, 0x0007000C, 0x0000000A, 0x0000042E, 0x00000002, 0x00000047, 0x0000042C, 0x0000042D, 0x00050094, 
0x00000007, 0x0000042F, 0x0000042E, 0x0000027F, 0x0008000C, 0x00000007, 0x00000430, 0x00000002, 0x0000002B, 0x0000042F, 0x00000094, 0x000000D4, 
0x0007000C, 0x00000007, 0x00000431, 0x00000002, 0x0000001A, 0x00000430, 0x000002BD, 0x0003003E, 0x0000042B, 0x00000431, 0x00040008, 0x00000001, 
0x000001B1, 0x00000000, 0x0004003D, 0x0000000A, 0x00000433, 0x00000409, 0x0004003D, 0x0000000A, 0x00000434, 0x000003CA, 0x00050094, 0x00000007, 
0x00000435, 0x00000433, 0x00000434, 0x00050081, 0x00000007, 0x00000436, 0x000000D4, 0x00000435, 0x0008000C, 0x00000007, 0x00000437, 0x00000002, 
0x0000002B, 0x00000436, 0x00000094, 0x000000D4, 0x0007000C, 0x00000007, 0x00000438, 0x00000002, 0x0000001A, 0x00000437, 0x00000081, 0x0003003E, 
0x00000432, 0x00000438, 0x00040008, 0x00000001, 0x000001B3, 0x00000000, 0x0004003D, 0x00000007, 0x0000043A, 0x0000041A, 0x00050085, 0x00000007, 
0x0000043B, 0x000000D4, 0x0000043A, 0x0005008E, 0x0000000A, 0x0000043F, 0x0000043E, 0x0000043B, 0x0003003E, 0x00000439, 0x0000043F, 0x00040008, 
0x00000001, 0x000001B4, 0x00000000, 0x0004003D, 0x00000007, 0x00000440, 0x00000424, 0x0005008E, 0x0000000A, 0x00000442, 0x00000441, 0x00000440, 
0x0004003D, 0x0000000A, 0x00000443, 0x00000439, 0x00050081, 0x0000000A, 0x00000444, 0x00000443, 0x00000442, 0x0003003E, 0x00000439, 0x00000444, 
0x00040008, 0x00000001, 0x000001B5, 0x00000000, 0x0004003D, 0x00000007, 0x00000446, 0x00000420, 0x00050085, 0x00000007, 0x00000447, 0x00000445, 
0x00000446, 0x0005008E, 0x0000000A, 0x00000449, 0x00000448, 0x00000447, 0x0004003D, 0x0000000A, 0x0000044A, 0x00000439, 0x00050081, 0x0000000A, 
0x0000044B, 0x0000044A, 0x00000449, 0x0003003E, 0x00000439, 0x0000044B, 0x00040008, 0x00000001, 0x000001B6, 0x00000000, 0x0003003E, 0x00000401, 
0x0000044C, 0x00040008, 0x00000001, 0x000001B8, 0x00000000, 0x0004003D, 0x0000000A, 0x0000044F, 0x00000403, 0x0003003E, 0x0000044E, 0x0000044F, 
0x0003003E, 0x00000450, 0x00000081, 0x00060039, 0x00000007, 0x00000451, 0x00000036, 0x0000044E, 0x00000450, 0x00050085, 0x00000007, 0x00000452, 
0x00000451, 0x000000D4, 0x0003003E, 0x0000044D, 0x00000452, 0x00040008, 0x00000001, 0x000001BA, 0x00000000, 0x0004003D, 0x0000000A, 0x00000455, 
0x00000403, 0x0003003E, 0x00000454, 0x00000455, 0x0003003E, 0x00000456, 0x000002E0, 0x00060039, 0x00000007, 0x00000457, 0x00000036, 0x00000454, 
0x00000456, 0x00050085, 0x00000007, 0x00000459, 0x00000457, 0x00000458, 0x0003003E, 0x00000453, 0x00000459, 0x00040008, 0x00000001, 0x000001BB, 
0x00000000, 0x0004003D, 0x00000007, 0x0000045A, 0x0000044D, 0x00050085, 0x00000007, 0x0000045B, 0x0000045A, 0x00000342, 0x0008000C, 0x00000007, 
0x0000045C, 0x00000002, 0x0000002B, 0x0000045B, 0x0000021C, 0x000000D4, 0x0005008E, 0x0000000A, 0x0000045E, 0x0000045D, 0x0000045C, 0x0004003D, 
0x0000000A, 0x0000045F, 0x00000401, 0x00050081, 0x0000000A, 0x00000460, 0x0000045F, 0x0000045E, 0x0003003E, 0x00000401, 0x00000460, 0x00040008, 
0x00000001, 0x000001BC, 0x00000000, 0x0004003D, 0x0000000A, 0x00000461, 0x00000401, 0x0004003D, 0x0000000A, 0x00000462, 0x00000439, 0x00050085, 
0x0000000A, 0x00000463, 0x00000461, 0x00000462, 0x0004003D, 0x0000000A, 0x00000464, 0x00000401, 0x0004003D, 0x00000007, 0x00000465, 0x0000042B, 
0x0005008E, 0x0000000A, 0x00000466, 0x00000464, 0x00000465, 0x0005008E, 0x0000000A, 0x00000467, 0x00000466, 0x00000259, 0x00050081, 0x0000000A, 
0x00000468, 0x00000463, 0x00000467, 0x0004003D, 0x00000007, 0x00000469, 0x00000432, 0x00050085, 0x00000007, 0x0000046A, 0x00000259, 0x00000469, 
0x0004003D, 0x0000000A, 0x0000046B, 0x00000401, 0x0005008E, 0x0000000A, 0x0000046C, 0x0000046B, 0x0000046A, 0x00050081, 0x0000000A, 0x0000046D, 
0x00000468, 0x0000046C, 0x0003003E, 0x00000401, 0x0000046D, 0x00040008, 0x00000001, 0x000001BD, 0x00000000, 0x0004003D, 0x00000007, 0x0000046E, 
0x0000044D, 0x00050085, 0x00000007, 0x0000046F, 0x0000046E, 0x000000D4, 0x00050081, 0x00000007, 0x00000470, 0x0000046F, 0x000000D4, 0x0004003D, 
0x0000000A, 0x00000471, 0x00000401, 0x0005008E, 0x0000000A, 0x00000472, 0x00000471, 0x00000470, 0x0003003E, 0x00000401, 0x00000472, 0x00040008, 
0x00000001, 0x000001BE, 0x00000000, 0x0004003D, 0x00000007, 0x00000473, 0x00000453, 0x00050085, 0x00000007, 0x00000474, 0x00000473, 0x000000D4, 
0x00050081, 0x00000007, 0x00000475, 0x00000474, 0x000000D4, 0x0004003D, 0x0000000A, 0x00000476, 0x00000401, 0x0005008E, 0x0000000A, 0x00000477, 
0x00000476, 0x00000475, 0x0003003E, 0x00000401, 0x00000477, 0x000200F9, 0x00000412, 0x000200F8, 0x00000412, 0x00040008, 0x00000001, 0x000001C2, 
0x00000000, 0x0004003D, 0x0000000A, 0x00000479, 0x000003A0, 0x0003003E, 0x00000478, 0x00000479, 0x0004003D, 0x0000000A, 0x0000047B, 0x000003CA, 
0x0003003E, 0x0000047A, 0x0000047B, 0x0004003D, 0x0000000A, 0x0000047D, 0x00000401, 0x0003003E, 0x0000047C, 0x0000047D, 0x0004003D, 0x00000007, 
0x0000047F, 0x000003E8, 0x0003003E, 0x0000047E, 0x0000047F, 0x00080039, 0x0000000A, 0x00000480, 0x0000003D, 0x00000478, 0x0000047A, 0x0000047C, 
0x0000047E, 0x0003003E, 0x00000401, 0x00000480, 0x00040008, 0x00000001, 0x000001C3, 0x00000000, 0x0004003D, 0x0000000A, 0x00000481, 0x00000401, 
0x0004003D, 0x0000000A, 0x00000482, 0x000003F6, 0x0004003D, 0x00000007, 0x00000484, 0x000003E8, 0x0008000C, 0x00000007, 0x00000485, 0x00000002, 
0x00000031, 0x00000483, 0x0000016B, 0x00000484, 0x00060050, 0x0000000A, 0x00000486, 0x00000485, 0x00000485, 0x00000485, 0x0008000C, 0x0000000A, 
0x00000487, 0x00000002, 0x0000002E, 0x00000481, 0x00000482, 0x00000486, 0x0003003E, 0x00000401, 0x00000487, 0x00040008, 0x00000001, 0x000001C4, 
0x00000000, 0x0004003D, 0x0000000A, 0x00000488, 0x000003F0, 0x0004003D, 0x0000000A, 0x00000489, 0x00000401, 0x00050081, 0x0000000A, 0x0000048A, 
0x00000489, 0x00000488, 0x0003003E, 0x00000401, 0x0000048A, 0x00040008, 0x00000001, 0x000001C5, 0x00000000, 0x0004003D, 0x0000000A, 0x0000048B, 
0x00000401, 0x0007000C, 0x0000000A, 0x0000048E, 0x00000002, 0x0000001A, 0x0000048B, 0x0000048D, 0x0003003E, 0x00000401, 0x0000048E, 0x00040008, 
0x00000001, 0x000001C6, 0x00000000, 0x0004003D, 0x0000000A, 0x0000048F, 0x00000401, 0x0004003D, 0x0000000A, 0x00000490, 0x00000401, 0x00060050, 
0x0000000A, 0x00000491, 0x00000094, 0x00000094, 0x00000094, 0x00060050, 0x0000000A, 0x00000492, 0x000000D4, 0x000000D4, 0x000000D4, 0x0008000C, 
0x0000000A, 0x00000493, 0x00000002, 0x00000031, 0x00000491, 0x00000492, 0x00000490, 0x00060050, 0x0000000A, 0x00000494, 0x00000264, 0x00000264, 
0x00000264, 0x0008000C, 0x0000000A, 0x00000495, 0x00000002, 0x0000002E, 0x0000048F, 0x00000493, 0x00000494, 0x0003003E, 0x00000401, 0x00000495, 
0x00040008, 0x00000001, 0x000001C7, 0x00000000, 0x00050041, 0x00000018, 0x00000497, 0x00000359, 0x00000096, 0x0004003D, 0x00000007, 0x00000498, 
0x00000497, 0x00050085, 0x00000007, 0x00000499, 0x00000496, 0x00000498, 0x00050041, 0x00000018, 0x0000049A, 0x00000359, 0x00000067, 0x0004003D, 
0x00000007, 0x0000049B, 0x0000049A, 0x00050085, 0x00000007, 0x0000049C, 0x00000499, 0x0000049B, 0x00050041, 0x00000018, 0x0000049D, 0x00000359, 
0x00000096, 0x0004003D, 0x00000007, 0x0000049E, 0x0000049D, 0x00050083, 0x00000007, 0x0000049F, 0x000000D4, 0x0000049E, 0x00050085, 0x00000007, 
0x000004A0, 0x0000049C, 0x0000049F, 0x00050041, 0x00000018, 0x000004A1, 0x00000359, 0x00000067, 0x0004003D, 0x00000007, 0x000004A2, 0x000004A1, 
0x00050083, 0x00000007, 0x000004A3, 0x000000D4, 0x000004A2, 0x00050085, 0x00000007, 0x000004A4, 0x000004A0, 0x000004A3, 0x0007000C, 0x00000007, 
0x000004A5, 0x00000002, 0x0000001A, 0x000004A4, 0x00000259, 0x00050085, 0x00000007, 0x000004A6, 0x000004A5, 0x00000342, 0x00050081, 0x00000007, 
0x000004A7, 0x000004A6, 0x00000259, 0x0004003D, 0x0000000A, 0x000004A8, 0x00000401, 0x0005008E, 0x0000000A, 0x000004A9, 0x000004A8, 0x000004A7, 
0x0003003E, 0x00000401, 0x000004A9, 0x00040008, 0x00000001, 0x000001C9, 0x00000000, 0x0004003D, 0x00000089, 0x000004AC, 0x000004AB, 0x0004003D, 
0x00000008, 0x000004AD, 0x00000359, 0x00050057, 0x00000051, 0x000004AE, 0x000004AC, 0x000004AD, 0x0003003E, 0x000004AA, 0x000004AE, 0x00040008, 
0x00000001, 0x000001CB, 0x00000000, 0x00050041, 0x00000068, 0x000004B1, 0x00000065, 0x000004B0, 0x0004003D, 0x00000007, 0x000004B2, 0x000004B1, 
0x00050083, 0x00000007, 0x000004B4, 0x000004B2, 0x000004B3, 0x0008000C, 0x00000007, 0x000004B5, 0x00000002, 0x0000002B, 0x000004B4, 0x00000094, 
0x000000D4, 0x0003003E, 0x000004AF, 0x000004B5, 0x00040008, 0x00000001, 0x000001CC, 0x00000000, 0x0004003D, 0x00000007, 0x000004B6, 0x000003E7, 
0x0004003D, 0x00000007, 0x000004B7, 0x000003E8, 0x0007000C, 0x00000007, 0x000004B9, 0x00000002, 0x0000001A, 0x000004B7, 0x000004B8, 0x00050088, 
0x00000007, 0x000004BA, 0x000004B6, 0x000004B9, 0x000500BA, 0x00000104, 0x000004BC, 0x000004BA, 0x000004BB, 0x000300F7, 0x000004BE, 0x00000000, 
0x000400FA, 0x000004BC, 0x000004BD, 0x000004BE, 0x000200F8, 0x000004BD, 0x00040008, 0x00000001, 0x000001CE, 0x00000000, 0x0004003D, 0x0000000A, 
0x000004BF, 0x00000401, 0x0004003D, 0x00000051, 0x000004C0, 0x000004AA, 0x0008004F, 0x0000000A, 0x000004C1, 0x000004C0, 0x000004C0, 0x00000000, 
0x00000001, 0x00000002, 0x00050041, 0x00000068, 0x000004C2, 0x00000065, 0x000004B0, 0x0004003D, 0x00000007, 0x000004C3, 0x000004C2, 0x00050085, 
0x00000007, 0x000004C4, 0x000004C3, 0x000002DE, 0x00050083, 0x00000007, 0x000004C5, 0x00000123, 0x000004C4, 0x0008000C, 0x00000007, 0x000004C6, 
0x00000002, 0x0000002B, 0x000004C5, 0x00000094, 0x000000D4, 0x00060050, 0x0000000A, 0x000004C7, 0x000004C6, 0x000004C6, 0x000004C6, 0x0008000C, 
0x0000000A, 0x000004C8, 0x00000002, 0x0000002E, 0x000004BF, 0x000004C1, 0x000004C7, 0x0003003E, 0x00000401, 0x000004C8, 0x000200F9, 0x000004BE, 
0x000200F8, 0x000004BE, 0x00040008, 0x00000001, 0x000001D1, 0x00000000, 0x0004003D, 0x0000000A, 0x000004C9, 0x00000401, 0x00050051, 0x00000007, 
0x000004CA, 0x000004C9, 0x00000000, 0x00050051, 0x00000007, 0x000004CB, 0x000004C9, 0x00000001, 0x00050051, 0x00000007, 0x000004CC, 0x000004C9, 
0x00000002, 0x00070050, 0x00000051, 0x000004CD, 0x000004CA, 0x000004CB, 0x000004CC, 0x000000D4, 0x0003003E, 0x00000054, 0x000004CD, 0x000100FD, 
0x00010038 });
	descr.Fragment().StringSpirvAsm( 
R"#(// Module Version 10000
// Generated by (magic number): 80002
// Id's are bound by 1230

                              Capability Shader
               2:             ExtInstImport  "GLSL.std.450"
                              MemoryModel Logical GLSL450
                              EntryPoint Fragment 5  "main" 90 112
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
                              Name 13  "noised(vf2;"
                              Name 12  "x"
                              Name 17  "terrain(vf2;"
                              Name 16  "p"
                              Name 22  "map(vf3;"
                              Name 21  "p"
                              Name 29  "march(vf3;vf3;f1;"
                              Name 26  "ro"
                              Name 27  "rd"
                              Name 28  "itrc"
                              Name 34  "normal(vf3;f1;"
                              Name 32  "p"
                              Name 33  "ds"
                              Name 37  "noise(vf2;"
                              Name 36  "x"
                              Name 40  "fbm(vf2;"
                              Name 39  "p"
                              Name 43  "bnoise(vf2;"
                              Name 42  "p"
                              Name 49  "bump(vf3;vf3;f1;"
                              Name 46  "p"
                              Name 47  "n"
                              Name 48  "ds"
                              Name 54  "curv(vf3;f1;"
                              Name 52  "p"
                              Name 53  "w"
                              Name 61  "fog(vf3;vf3;vf3;f1;"
                              Name 57  "ro"
                              Name 58  "rd"
                              Name 59  "col"
                              Name 60  "ds"
                              Name 67  "linstep(f1;f1;f1;"
                              Name 64  "mn"
                              Name 65  "mx"
                              Name 66  "x"
                              Name 72  "scatter(vf3;vf3;"
                              Name 70  "ro"
                              Name 71  "rd"
                              Name 76  "hash33(vf3;"
                              Name 75  "p"
                              Name 79  "stars(vf3;"
                              Name 78  "p"
                              Name 86  "mainImage(vf4;vf2;"
                              Name 84  "fragColor"
                              Name 85  "fragCoord"
                              Name 88  "coord"
                              Name 90  "gl_FragCoord"
                              Name 99  "ShadertoyUB"
                              MemberName 99(ShadertoyUB) 0  "iResolution"
                              MemberName 99(ShadertoyUB) 1  "iTime"
                              MemberName 99(ShadertoyUB) 2  "iTimeDelta"
                              MemberName 99(ShadertoyUB) 3  "iFrame"
                              MemberName 99(ShadertoyUB) 4  "_padding0"
                              MemberName 99(ShadertoyUB) 5  "iChannelTime"
                              MemberName 99(ShadertoyUB) 6  "iChannelResolution"
                              MemberName 99(ShadertoyUB) 7  "iMouse"
                              MemberName 99(ShadertoyUB) 8  "iDate"
                              MemberName 99(ShadertoyUB) 9  "iSampleRate"
                              MemberName 99(ShadertoyUB) 10  "_padding1"
                              MemberName 99(ShadertoyUB) 11  "_padding2"
                              MemberName 99(ShadertoyUB) 12  "_padding3"
                              MemberName 99(ShadertoyUB) 13  "iCameraFrustum"
                              MemberName 99(ShadertoyUB) 14  "iCameraPos"
                              Name 101  ""
                              Name 112  "out_Color0"
                              Name 113  "param"
                              Name 114  "param"
                              Name 118  "p"
                              Name 121  "f"
                              Name 124  "u"
                              Name 135  "a"
                              Name 139  "iChannel0"
                              Name 152  "b"
                              Name 162  "c"
                              Name 171  "d"
                              Name 241  "rz"
                              Name 242  "z"
                              Name 243  "d"
                              Name 245  "scl"
                              Name 247  "zscl"
                              Name 249  "zz"
                              Name 252  "i"
                              Name 262  "n"
                              Name 263  "param"
                              Name 330  "param"
                              Name 338  "t"
                              Name 339  "d"
                              Name 345  "param"
                              Name 347  "precis"
                              Name 349  "i"
                              Name 371  "rl"
                              Name 386  "param"
                              Name 397  "e"
                              Name 410  "param"
                              Name 419  "param"
                              Name 429  "param"
                              Name 439  "param"
                              Name 454  "z"
                              Name 455  "rz"
                              Name 456  "i"
                              Name 464  "param"
                              Name 486  "param"
                              Name 490  "e"
                              Name 495  "n0"
                              Name 496  "param"
                              Name 500  "d"
                              Name 505  "param"
                              Name 514  "param"
                              Name 539  "e"
                              Name 544  "t1"
                              Name 549  "param"
                              Name 551  "t2"
                              Name 556  "param"
                              Name 558  "t3"
                              Name 563  "param"
                              Name 565  "t4"
                              Name 570  "param"
                              Name 584  "param"
                              Name 592  "pos"
                              Name 598  "mx"
                              Name 609  "param"
                              Name 614  "den"
                              Name 634  "sdt"
                              Name 642  "fogColor"
                              Name 677  "sd"
                              Name 683  "dtp"
                              Name 693  "hori"
                              Name 695  "param"
                              Name 696  "param"
                              Name 697  "param"
                              Name 702  "param"
                              Name 703  "param"
                              Name 704  "param"
                              Name 714  "col"
                              Name 783  "c"
                              Name 784  "res"
                              Name 788  "i"
                              Name 796  "q"
                              Name 804  "id"
                              Name 810  "rn"
                              Name 811  "param"
                              Name 815  "c2"
                              Name 857  "q"
                              Name 864  "p"
                              Name 877  "mo"
                              Name 928  "ro"
                              Name 941  "eye"
                              Name 953  "right"
                              Name 965  "up"
                              Name 970  "rd"
                              Name 999  "count"
                              Name 1000  "rz"
                              Name 1001  "param"
                              Name 1003  "param"
                              Name 1005  "param"
                              Name 1008  "scatt"
                              Name 1009  "param"
                              Name 1011  "param"
                              Name 1014  "bg"
                              Name 1015  "param"
                              Name 1025  "col"
                              Name 1027  "pos"
                              Name 1033  "nor"
                              Name 1034  "param"
                              Name 1036  "param"
                              Name 1043  "param"
                              Name 1045  "param"
                              Name 1047  "param"
                              Name 1050  "amb"
                              Name 1056  "dif"
                              Name 1060  "bac"
                              Name 1067  "spe"
                              Name 1074  "fre"
                              Name 1081  "brdf"
                              Name 1101  "crv"
                              Name 1102  "param"
                              Name 1104  "param"
                              Name 1107  "crv2"
                              Name 1108  "param"
                              Name 1110  "param"
                              Name 1144  "param"
                              Name 1146  "param"
                              Name 1148  "param"
                              Name 1150  "param"
                              Name 1194  "past"
                              Name 1195  "iChannel1"
                              Name 1199  "tOver"
                              Decorate 90(gl_FragCoord) BuiltIn FragCoord
                              Decorate 96 ArrayStride 16
                              Decorate 97 ArrayStride 16
                              Decorate 98 ArrayStride 16
                              MemberDecorate 99(ShadertoyUB) 0 Offset 0
                              MemberDecorate 99(ShadertoyUB) 1 Offset 16
                              MemberDecorate 99(ShadertoyUB) 2 Offset 20
                              MemberDecorate 99(ShadertoyUB) 3 Offset 24
                              MemberDecorate 99(ShadertoyUB) 4 Offset 28
                              MemberDecorate 99(ShadertoyUB) 5 Offset 32
                              MemberDecorate 99(ShadertoyUB) 6 Offset 96
                              MemberDecorate 99(ShadertoyUB) 7 Offset 160
                              MemberDecorate 99(ShadertoyUB) 8 Offset 176
                              MemberDecorate 99(ShadertoyUB) 9 Offset 192
                              MemberDecorate 99(ShadertoyUB) 10 Offset 196
                              MemberDecorate 99(ShadertoyUB) 11 Offset 200
                              MemberDecorate 99(ShadertoyUB) 12 Offset 204
                              MemberDecorate 99(ShadertoyUB) 13 Offset 208
                              MemberDecorate 99(ShadertoyUB) 14 Offset 272
                              Decorate 99(ShadertoyUB) Block
                              Decorate 101 DescriptorSet 0
                              Decorate 101 Binding 2
                              Decorate 112(out_Color0) Location 0
                              Decorate 139(iChannel0) DescriptorSet 0
                              Decorate 139(iChannel0) Binding 0
                              Decorate 1195(iChannel1) DescriptorSet 0
                              Decorate 1195(iChannel1) Binding 1
               3:             TypeVoid
               4:             TypeFunction 3
               7:             TypeFloat 32
               8:             TypeVector 7(float) 2
               9:             TypePointer Function 8(fvec2)
              10:             TypeVector 7(float) 3
              11:             TypeFunction 10(fvec3) 9(ptr)
              15:             TypeFunction 7(float) 9(ptr)
              19:             TypePointer Function 10(fvec3)
              20:             TypeFunction 7(float) 19(ptr)
              24:             TypePointer Function 7(float)
              25:             TypeFunction 7(float) 19(ptr) 19(ptr) 24(ptr)
              31:             TypeFunction 10(fvec3) 19(ptr) 24(ptr)
              45:             TypeFunction 10(fvec3) 19(ptr) 19(ptr) 24(ptr)
              51:             TypeFunction 7(float) 19(ptr) 24(ptr)
              56:             TypeFunction 10(fvec3) 19(ptr) 19(ptr) 19(ptr) 24(ptr)
              63:             TypeFunction 7(float) 24(ptr) 24(ptr) 24(ptr)
              69:             TypeFunction 10(fvec3) 19(ptr) 19(ptr)
              74:             TypeFunction 10(fvec3) 19(ptr)
              81:             TypeVector 7(float) 4
              82:             TypePointer Function 81(fvec4)
              83:             TypeFunction 3 82(ptr) 9(ptr)
              89:             TypePointer Input 81(fvec4)
90(gl_FragCoord):     89(ptr) Variable Input
              93:             TypeInt 32 1
              94:             TypeInt 32 0
              95:     94(int) Constant 4
              96:             TypeArray 81(fvec4) 95
              97:             TypeArray 81(fvec4) 95
              98:             TypeArray 81(fvec4) 95
 99(ShadertoyUB):             TypeStruct 81(fvec4) 7(float) 7(float) 93(int) 7(float) 96 97 81(fvec4) 81(fvec4) 7(float) 7(float) 7(float) 7(float) 98 81(fvec4)
             100:             TypePointer Uniform 99(ShadertoyUB)
             101:    100(ptr) Variable Uniform
             102:     93(int) Constant 0
             103:     94(int) Constant 1
             104:             TypePointer Uniform 7(float)
             111:             TypePointer Output 81(fvec4)
 112(out_Color0):    111(ptr) Variable Output
             128:    7(float) Constant 1077936128
             129:    7(float) Constant 1073741824
             136:             TypeImage 7(float) 2D sampled format:Unknown
             137:             TypeSampledImage 136
             138:             TypePointer UniformConstant 137
  139(iChannel0):    138(ptr) Variable UniformConstant
             142:    7(float) Constant 1056964608
             143:    8(fvec2) ConstantComposite 142 142
             145:    7(float) Constant 1132462080
             148:    7(float) Constant 0
             150:     94(int) Constant 0
             155:    7(float) Constant 1069547520
             156:    8(fvec2) ConstantComposite 155 142
             165:    8(fvec2) ConstantComposite 142 155
             174:    8(fvec2) ConstantComposite 155 155
             209:    7(float) Constant 1086324736
             212:    7(float) Constant 1065353216
             244:    8(fvec2) ConstantComposite 148 148
             246:    7(float) Constant 1077726413
             248:    7(float) Constant 3201092813
             250:    7(float) Constant 1084227584
             251:             TypePointer Function 93(int)
             259:     93(int) Constant 5
             260:             TypeBool
             274:    7(float) Constant 3204448256
             291:    7(float) Constant 1062836634
             299:    7(float) Constant 1061997773
             302:             TypeMatrix 8(fvec2) 2
             303:    7(float) Constant 1058642330
             304:    8(fvec2) ConstantComposite 299 303
             305:    7(float) Constant 3206125978
             306:    8(fvec2) ConstantComposite 305 299
             307:         302 ConstantComposite 304 306
             313:     93(int) Constant 1
             317:    7(float) Constant 1061158912)#"_str
+
R"#(
             328:    7(float) Constant 1032805417
             332:    7(float) Constant 1076677837
             348:    7(float) Constant 953267991
             356:     93(int) Constant 90
             363:    7(float) Constant 1137180672
             373:    7(float) Constant 1017370378
             388:    7(float) Constant 1060320051
             398:    7(float) Constant 3120763503
             399:    7(float) Constant 973279855
             400:    8(fvec2) ConstantComposite 398 399
             448:    7(float) Constant 1008981770
             491:    7(float) Constant 1000593162
             523:    7(float) Constant 1020054733
             540:    7(float) Constant 3212836864
             541:    8(fvec2) ConstantComposite 540 212
             572:    7(float) Constant 1041865114
             583:    7(float) Constant 1082130432
             601:    7(float) Constant 1036831949
             605:    7(float) Constant 1028443341
             612:    7(float) Constant 1045220557
             615:    7(float) Constant 1050253722
             636:    7(float) Constant 3204834132
             637:    7(float) Constant 1053944709
             638:    7(float) Constant 3208592228
             639:   10(fvec3) ConstantComposite 636 637 638
             643:    7(float) Constant 1047904911
             644:    7(float) Constant 1043878380
             645:   10(fvec3) ConstantComposite 303 643 644
             646:    7(float) Constant 1068960317
             647:    7(float) Constant 1061662228
             648:    7(float) Constant 1058390671
             649:   10(fvec3) ConstantComposite 646 647 648
             684:    7(float) Constant 1095761920
             690:    7(float) Constant 1080033280
             694:    7(float) Constant 3300622336
             700:    7(float) Constant 1093664768
             701:    7(float) Constant 1140457472
             710:    7(float) Constant 1025758986
             715:   10(fvec3) ConstantComposite 148 148 148
             717:    7(float) Constant 1128792064
             719:   10(fvec3) ConstantComposite 212 388 142
             725:    7(float) Constant 1103626240
             727:    7(float) Constant 1048576000
             728:   10(fvec3) ConstantComposite 212 142 727
             734:    7(float) Constant 1088421888
             736:    7(float) Constant 1053609165
             737:   10(fvec3) ConstantComposite 212 736 727
             746:    7(float) Constant 1138619105
             747:    7(float) Constant 1137092110
             748:    7(float) Constant 1140168691
             749:   10(fvec3) ConstantComposite 746 747 748
             756:    7(float) Constant 1100622070
             768:     94(int) Constant 2
             825:    7(float) Constant 981668463
             832:    7(float) Constant 1056629064
             833:   10(fvec3) ConstantComposite 212 832 601
             834:    7(float) Constant 1063675494
             835:   10(fvec3) ConstantComposite 317 834 212
             846:    7(float) Constant 1068708659
             859:             TypePointer Uniform 81(fvec4)
             878:     93(int) Constant 7
             890:    8(fvec2) ConstantComposite 274 274
             891:             TypeVector 260(bool) 2
             896:    7(float) Constant 3192704205
             897:    8(fvec2) ConstantComposite 896 615
             901:    7(float) Constant 1067030938
             906:    7(float) Constant 3184315597
             907:    8(fvec2) ConstantComposite 901 906
             929:    7(float) Constant 1143111680
             935:    7(float) Constant 1092616192
             956:    7(float) Constant 1070141433
             980:    7(float) Constant 1065772646
             990:    7(float) Constant 1014350479
            1019:    7(float) Constant 1067869798
            1020:   10(fvec3) ConstantComposite 1019 1019 1019
            1062:    7(float) Constant 1058198336
            1063:    7(float) Constant 1062319566
            1064:   10(fvec3) ConstantComposite 1062 148 1063
            1084:    7(float) Constant 1038174126
            1085:    7(float) Constant 1039516303
            1086:   10(fvec3) ConstantComposite 601 1084 1085
            1089:   10(fvec3) ConstantComposite 572 605 710
            1093:    7(float) Constant 1075000115
            1096:   10(fvec3) ConstantComposite 834 736 727
            1100:   10(fvec3) ConstantComposite 727 727 615
            1112:    7(float) Constant 1075838976
            1117:   10(fvec3) ConstantComposite 727 303 142
            1155:    7(float) Constant 1132068864
            1164:    7(float) Constant 1064178811
            1165:   10(fvec3) ConstantComposite 1164 212 212
            1174:    7(float) Constant 1098907648
 1195(iChannel1):    138(ptr) Variable UniformConstant
            1200:     93(int) Constant 2
            1203:    7(float) Constant 1015580810
            1208:    7(float) Constant 1059481190
            1211:    7(float) Constant 1079194419
         5(main):           3 Function None 4
               6:             Label
       88(coord):      9(ptr) Variable Function
      113(param):     82(ptr) Variable Function
      114(param):      9(ptr) Variable Function
                              Line 1 134 0
              91:   81(fvec4) Load 90(gl_FragCoord)
              92:    8(fvec2) VectorShuffle 91 91 0 1
                              Store 88(coord) 92
                              Line 1 135 0
             105:    104(ptr) AccessChain 101 102 103
             106:    7(float) Load 105
             107:     24(ptr) AccessChain 88(coord) 103
             108:    7(float) Load 107
             109:    7(float) FSub 106 108
             110:     24(ptr) AccessChain 88(coord) 103
                              Store 110 109
                              Line 1 136 0
             115:    8(fvec2) Load 88(coord)
                              Store 114(param) 115
             116:           3 FunctionCall 86(mainImage(vf4;vf2;) 113(param) 114(param)
             117:   81(fvec4) Load 113(param)
                              Store 112(out_Color0) 117
                              Return
                              FunctionEnd
 13(noised(vf2;):   10(fvec3) Function None 11
           12(x):      9(ptr) FunctionParameter
              14:             Label
          118(p):      9(ptr) Variable Function
          121(f):      9(ptr) Variable Function
          124(u):      9(ptr) Variable Function
          135(a):     24(ptr) Variable Function
          152(b):     24(ptr) Variable Function
          162(c):     24(ptr) Variable Function
          171(d):     24(ptr) Variable Function
                              Line 1 143 0
             119:    8(fvec2) Load 12(x)
             120:    8(fvec2) ExtInst 2(GLSL.std.450) 8(Floor) 119
                              Store 118(p) 120
                              Line 1 145 0
             122:    8(fvec2) Load 12(x)
             123:    8(fvec2) ExtInst 2(GLSL.std.450) 10(Fract) 122
                              Store 121(f) 123
                              Line 1 147 0
             125:    8(fvec2) Load 121(f)
             126:    8(fvec2) Load 121(f)
             127:    8(fvec2) FMul 125 126
             130:    8(fvec2) Load 121(f)
             131:    8(fvec2) VectorTimesScalar 130 129
             132:    8(fvec2) CompositeConstruct 128 128
             133:    8(fvec2) FSub 132 131
             134:    8(fvec2) FMul 127 133
                              Store 124(u) 134
                              Line 1 149 0
             140:         137 Load 139(iChannel0)
             141:    8(fvec2) Load 118(p)
             144:    8(fvec2) FAdd 141 143
             146:    8(fvec2) CompositeConstruct 145 145
             147:    8(fvec2) FDiv 144 146
             149:   81(fvec4) ImageSampleExplicitLod 140 147 Lod 148
             151:    7(float) CompositeExtract 149 0
                              Store 135(a) 151
                              Line 1 151 0
             153:         137 Load 139(iChannel0)
             154:    8(fvec2) Load 118(p)
             157:    8(fvec2) FAdd 154 156
             158:    8(fvec2) CompositeConstruct 145 145
             159:    8(fvec2) FDiv 157 158
             160:   81(fvec4) ImageSampleExplicitLod 153 159 Lod 148
             161:    7(float) CompositeExtract 160 0
                              Store 152(b) 161
                              Line 1 153 0
             163:         137 Load 139(iChannel0)
             164:    8(fvec2) Load 118(p)
             166:    8(fvec2) FAdd 164 165
             167:    8(fvec2) CompositeConstruct 145 145
             168:    8(fvec2) FDiv 166 167
             169:   81(fvec4) ImageSampleExplicitLod 163 168 Lod 148
             170:    7(float) CompositeExtract 169 0
                              Store 162(c) 170
                              Line 1 155 0
             172:         137 Load 139(iChannel0)
             173:    8(fvec2) Load 118(p)
             175:    8(fvec2) FAdd 173 174
             176:    8(fvec2) CompositeConstruct 145 145
             177:    8(fvec2) FDiv 175 176
             178:   81(fvec4) ImageSampleExplicitLod 172 177 Lod 148
             179:    7(float) CompositeExtract 178 0
                              Store 171(d) 179
                              Line 1 156 0
             180:    7(float) Load 135(a)
             181:    7(float) Load 152(b)
             182:    7(float) Load 135(a)
             183:    7(float) FSub 181 182
             184:     24(ptr) AccessChain 124(u) 150
             185:    7(float) Load 184
             186:    7(float) FMul 183 185
             187:    7(float) FAdd 180 186
             188:    7(float) Load 162(c)
             189:    7(float) Load 135(a)
             190:    7(float) FSub 188 189
             191:     24(ptr) AccessChain 124(u) 103
             192:    7(float) Load 191
             193:    7(float) FMul 190 192
             194:    7(float) FAdd 187 193
             195:    7(float) Load 135(a)
             196:    7(float) Load 152(b)
             197:    7(float) FSub 195 196
             198:    7(float) Load 162(c)
             199:    7(float) FSub 197 198
             200:    7(float) Load 171(d)
             201:    7(float) FAdd 199 200
             202:     24(ptr) AccessChain 124(u) 150
             203:    7(float) Load 202
             204:    7(float) FMul 201 203
             205:     24(ptr) AccessChain 124(u) 103
             206:    7(float) Load 205
             207:    7(float) FMul 204 206
             208:    7(float) FAdd 194 207
             210:    8(fvec2) Load 121(f)
             211:    8(fvec2) VectorTimesScalar 210 209
             213:    8(fvec2) Load 121(f)
             214:    8(fvec2) CompositeConstruct 212 212
             215:    8(fvec2) FSub 214 213
             216:    8(fvec2) FMul 211 215
             217:    7(float) Load 152(b)
             218:    7(float) Load 135(a)
             219:    7(float) FSub 217 218
             220:    7(float) Load 162(c)
             221:    7(float) Load 135(a)
             222:    7(float) FSub 220 221
             223:    8(fvec2) CompositeConstruct 219 222
             224:    7(float) Load 135(a)
             225:    7(float) Load 152(b)
             226:    7(float) FSub 224 225
             227:    7(float) Load 162(c)
             228:    7(float) FSub 226 227
             229:    7(float) Load 171(d)
             230:    7(float) FAdd 228 229
             231:    8(fvec2) Load 124(u)
             232:    8(fvec2) VectorShuffle 231 231 1 0
             233:    8(fvec2) VectorTimesScalar 232 230
             234:    8(fvec2) FAdd 223 233
             235:    8(fvec2) FMul 216 234
             236:    7(float) CompositeExtract 235 0
             237:    7(float) CompositeExtract 235 1
             238:   10(fvec3) CompositeConstruct 208 236 237
                              ReturnValue 238
                              FunctionEnd
17(terrain(vf2;):    7(float) Function None 15
           16(p):      9(ptr) FunctionParameter
              18:             Label
         241(rz):     24(ptr) Variable Function
          242(z):     24(ptr) Variable Function
          243(d):      9(ptr) Variable Function
        245(scl):     24(ptr) Variable Function
       247(zscl):     24(ptr) Variable Function
         249(zz):     24(ptr) Variable Function
          252(i):    251(ptr) Variable Function
          262(n):     19(ptr) Variable Function
      263(param):      9(ptr) Variable Function
                              Line 1 163 0
                              Store 241(rz) 148
                              Line 1 165 0
                              Store 242(z) 212
                              Line 1 167 0
                              Store 243(d) 244
                              Line 1 169 0
                              Store 245(scl) 246
                              Line 1 171 0
                              Store 247(zscl) 248
                              Line 1 173 0
                              Store 249(zz) 250
                              Line 1 176 0
                              Store 252(i) 102
                              Branch 253
                              Line 1 177 0
             253:             Label
                              LoopMerge 255 256 None
                              Branch 257
             257:             Label
             258:     93(int) Load 252(i)
             261:   260(bool) SLessThan 258 259
                              BranchConditional 261 254 255
             254:               Label
                                Line 1 180 0
             264:    8(fvec2)   Load 16(p)
                                Store 263(param) 264
             265:   10(fvec3)   FunctionCall 13(noised(vf2;) 263(param)
                                Store 262(n) 265
                                Line 1 181 0
             266:   10(fvec3)   Load 262(n)
             267:    8(fvec2)   VectorShuffle 266 266 1 2
             268:    8(fvec2)   ExtInst 2(GLSL.std.450) 4(FAbs) 267
             269:    7(float)   Load 249(zz)
             270:    8(fvec2)   CompositeConstruct 269 269
             271:    8(fvec2)   ExtInst 2(GLSL.std.450) 26(Pow) 268 270
             272:    8(fvec2)   Load 243(d)
             273:    8(fvec2)   FAdd 272 271
                                Store 243(d) 273
                                Line 1 182 0
             275:   10(fvec3)   Load 262(n)
             276:    8(fvec2)   VectorShuffle 275 275 1 2
             277:    8(fvec2)   CompositeConstruct 274 274
             278:    8(fvec2)   CompositeConstruct 155 155
             279:    8(fvec2)   ExtInst 2(GLSL.std.450) 49(SmoothStep) 277 278 276
             280:    8(fvec2)   Load 243(d)
             281:    8(fvec2)   FSub 280 279
                                Store 243(d) 281
                                Line 1 183 0
             282:    7(float)   Load 249(zz)
             283:    7(float)   FSub 282 212
                                Store 249(zz) 283
                                Line 1 184 0
             284:    7(float)   Load 242(z)
             285:     24(ptr)   AccessChain 262(n) 150
             286:    7(float)   Load 285
             287:    7(float)   FMul 284 286
             288:    8(fvec2)   Load 243(d)
             289:    8(fvec2)   Load 243(d)
             290:    7(float)   Dot 288 289
             292:    7(float)   FAdd 290 291
             293:    7(float)   FDiv 287 292
             294:    7(float)   Load 241(rz)
             295:    7(float)   FAdd 294 293
                                Store 241(rz) 295
                                Line 1 185 0
             296:    7(float)   Load 247(zscl)
             297:    7(float)   Load 242(z)
             298:    7(float)   FMul 297 296
                                Store 242(z) 298
                                Line 1 186 0)#"_str
+
R"#(
             300:    7(float)   Load 247(zscl)
             301:    7(float)   FMul 300 299
                                Store 247(zscl) 301
                                Line 1 187 0
             308:    8(fvec2)   Load 16(p)
             309:    8(fvec2)   MatrixTimesVector 307 308
             310:    7(float)   Load 245(scl)
             311:    8(fvec2)   VectorTimesScalar 309 310
                                Store 16(p) 311
                                Branch 256
             256:               Label
                                Line 1 177 0
             312:     93(int)   Load 252(i)
             314:     93(int)   IAdd 312 313
                                Store 252(i) 314
                                Branch 253
             255:             Label
                              Line 1 191 0
             315:    7(float) Load 241(rz)
             316:    7(float) ExtInst 2(GLSL.std.450) 49(SmoothStep) 155 274 315
             318:    7(float) FAdd 316 317
             319:    7(float) Load 241(rz)
             320:    7(float) FDiv 319 318
                              Store 241(rz) 320
                              Line 1 192 0
             321:    7(float) Load 241(rz)
                              ReturnValue 321
                              FunctionEnd
    22(map(vf3;):    7(float) Function None 20
           21(p):     19(ptr) FunctionParameter
              23:             Label
      330(param):      9(ptr) Variable Function
                              Line 1 198 0
             324:     24(ptr) AccessChain 21(p) 103
             325:    7(float) Load 324
             326:   10(fvec3) Load 21(p)
             327:    8(fvec2) VectorShuffle 326 326 2 0
             329:    8(fvec2) VectorTimesScalar 327 328
                              Store 330(param) 329
             331:    7(float) FunctionCall 17(terrain(vf2;) 330(param)
             333:    7(float) FMul 331 332
             334:    7(float) FSub 325 333
             335:    7(float) FSub 334 212
                              ReturnValue 335
                              FunctionEnd
29(march(vf3;vf3;f1;):    7(float) Function None 25
          26(ro):     19(ptr) FunctionParameter
          27(rd):     19(ptr) FunctionParameter
        28(itrc):     24(ptr) FunctionParameter
              30:             Label
          338(t):     24(ptr) Variable Function
          339(d):     24(ptr) Variable Function
      345(param):     19(ptr) Variable Function
     347(precis):     24(ptr) Variable Function
          349(i):    251(ptr) Variable Function
         371(rl):     24(ptr) Variable Function
      386(param):     19(ptr) Variable Function
                              Line 1 205 0
                              Store 338(t) 148
                              Line 1 207 0
             340:   10(fvec3) Load 27(rd)
             341:    7(float) Load 338(t)
             342:   10(fvec3) VectorTimesScalar 340 341
             343:   10(fvec3) Load 26(ro)
             344:   10(fvec3) FAdd 342 343
                              Store 345(param) 344
             346:    7(float) FunctionCall 22(map(vf3;) 345(param)
                              Store 339(d) 346
                              Line 1 209 0
                              Store 347(precis) 348
                              Line 1 212 0
                              Store 349(i) 102
                              Branch 350
                              Line 1 213 0
             350:             Label
                              LoopMerge 352 353 None
                              Branch 354
             354:             Label
             355:     93(int) Load 349(i)
             357:   260(bool) SLessThanEqual 355 356
                              BranchConditional 357 351 352
             351:               Label
                                Line 1 215 0
             358:    7(float)   Load 339(d)
             359:    7(float)   ExtInst 2(GLSL.std.450) 4(FAbs) 358
             360:    7(float)   Load 347(precis)
             361:   260(bool)   FOrdLessThan 359 360
             362:    7(float)   Load 338(t)
             364:   260(bool)   FOrdGreaterThan 362 363
             365:   260(bool)   LogicalOr 361 364
                                SelectionMerge 367 None
                                BranchConditional 365 366 367
             366:                 Label
                                  Line 1 217 0
                                  Branch 352
             367:               Label
                                Line 1 220 0
             369:    7(float)   Load 338(t)
             370:    7(float)   FMul 369 348
                                Store 347(precis) 370
                                Line 1 222 0
             372:    7(float)   Load 338(t)
             374:    7(float)   FMul 372 373
             375:    7(float)   ExtInst 2(GLSL.std.450) 40(FMax) 374 212
                                Store 371(rl) 375
                                Line 1 223 0
             376:    7(float)   Load 339(d)
             377:    7(float)   Load 371(rl)
             378:    7(float)   FMul 376 377
             379:    7(float)   Load 338(t)
             380:    7(float)   FAdd 379 378
                                Store 338(t) 380
                                Line 1 224 0
             381:   10(fvec3)   Load 27(rd)
             382:    7(float)   Load 338(t)
             383:   10(fvec3)   VectorTimesScalar 381 382
             384:   10(fvec3)   Load 26(ro)
             385:   10(fvec3)   FAdd 383 384
                                Store 386(param) 385
             387:    7(float)   FunctionCall 22(map(vf3;) 386(param)
             389:    7(float)   FMul 387 388
                                Store 339(d) 389
                                Line 1 225 0
             390:    7(float)   Load 28(itrc)
             391:    7(float)   FAdd 390 212
                                Store 28(itrc) 391
                                Branch 353
             353:               Label
                                Line 1 213 0
             392:     93(int)   Load 349(i)
             393:     93(int)   IAdd 392 313
                                Store 349(i) 393
                                Branch 350
             352:             Label
                              Line 1 229 0
             394:    7(float) Load 338(t)
                              ReturnValue 394
                              FunctionEnd
34(normal(vf3;f1;):   10(fvec3) Function None 31
           32(p):     19(ptr) FunctionParameter
          33(ds):     24(ptr) FunctionParameter
              35:             Label
          397(e):      9(ptr) Variable Function
      410(param):     19(ptr) Variable Function
      419(param):     19(ptr) Variable Function
      429(param):     19(ptr) Variable Function
      439(param):     19(ptr) Variable Function
                              Line 1 236 0
             401:    7(float) Load 33(ds)
             402:    7(float) ExtInst 2(GLSL.std.450) 26(Pow) 401 212
             403:    8(fvec2) VectorTimesScalar 400 402
                              Store 397(e) 403
                              Line 1 237 0
             404:    8(fvec2) Load 397(e)
             405:   10(fvec3) VectorShuffle 404 404 1 0 0
             406:   10(fvec3) Load 32(p)
             407:    8(fvec2) Load 397(e)
             408:   10(fvec3) VectorShuffle 407 407 1 0 0
             409:   10(fvec3) FAdd 406 408
                              Store 410(param) 409
             411:    7(float) FunctionCall 22(map(vf3;) 410(param)
             412:   10(fvec3) VectorTimesScalar 405 411
             413:    8(fvec2) Load 397(e)
             414:   10(fvec3) VectorShuffle 413 413 0 0 1
             415:   10(fvec3) Load 32(p)
             416:    8(fvec2) Load 397(e)
             417:   10(fvec3) VectorShuffle 416 416 0 0 1
             418:   10(fvec3) FAdd 415 417
                              Store 419(param) 418
             420:    7(float) FunctionCall 22(map(vf3;) 419(param)
             421:   10(fvec3) VectorTimesScalar 414 420
             422:   10(fvec3) FAdd 412 421
             423:    8(fvec2) Load 397(e)
             424:   10(fvec3) VectorShuffle 423 423 0 1 0
             425:   10(fvec3) Load 32(p)
             426:    8(fvec2) Load 397(e)
             427:   10(fvec3) VectorShuffle 426 426 0 1 0
             428:   10(fvec3) FAdd 425 427
                              Store 429(param) 428
             430:    7(float) FunctionCall 22(map(vf3;) 429(param)
             431:   10(fvec3) VectorTimesScalar 424 430
             432:   10(fvec3) FAdd 422 431
             433:    8(fvec2) Load 397(e)
             434:   10(fvec3) VectorShuffle 433 433 1 1 1
             435:   10(fvec3) Load 32(p)
             436:    8(fvec2) Load 397(e)
             437:   10(fvec3) VectorShuffle 436 436 1 1 1
             438:   10(fvec3) FAdd 435 437
                              Store 439(param) 438
             440:    7(float) FunctionCall 22(map(vf3;) 439(param)
             441:   10(fvec3) VectorTimesScalar 434 440
             442:   10(fvec3) FAdd 432 441
             443:   10(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 442
                              ReturnValue 443
                              FunctionEnd
  37(noise(vf2;):    7(float) Function None 15
           36(x):      9(ptr) FunctionParameter
              38:             Label
                              Line 1 243 0
             446:         137 Load 139(iChannel0)
             447:    8(fvec2) Load 36(x)
             449:    8(fvec2) VectorTimesScalar 447 448
             450:   81(fvec4) ImageSampleImplicitLod 446 449
             451:    7(float) CompositeExtract 450 0
                              ReturnValue 451
                              FunctionEnd
    40(fbm(vf2;):    7(float) Function None 15
           39(p):      9(ptr) FunctionParameter
              41:             Label
          454(z):     24(ptr) Variable Function
         455(rz):     24(ptr) Variable Function
          456(i):     24(ptr) Variable Function
      464(param):      9(ptr) Variable Function
                              Line 1 250 0
                              Store 454(z) 142
                              Line 1 252 0
                              Store 455(rz) 148
                              Line 1 255 0
                              Store 456(i) 148
                              Branch 457
                              Line 1 256 0
             457:             Label
                              LoopMerge 459 460 None
                              Branch 461
             461:             Label
             462:    7(float) Load 456(i)
             463:   260(bool) FOrdLessThan 462 128
                              BranchConditional 463 458 459
             458:               Label
                                Line 1 258 0
             465:    8(fvec2)   Load 39(p)
                                Store 464(param) 465
             466:    7(float)   FunctionCall 37(noise(vf2;) 464(param)
             467:    7(float)   FMul 466 250
             468:    7(float)   ExtInst 2(GLSL.std.450) 13(Sin) 467
             469:    7(float)   FMul 468 142
             470:    7(float)   FAdd 469 142
             471:    7(float)   Load 454(z)
             472:    7(float)   FMul 470 471
             473:    7(float)   Load 455(rz)
             474:    7(float)   FAdd 473 472
                                Store 455(rz) 474
                                Line 1 259 0
             475:    7(float)   Load 454(z)
             476:    7(float)   FMul 475 142
                                Store 454(z) 476
                                Line 1 260 0
             477:    8(fvec2)   Load 39(p)
             478:    8(fvec2)   VectorTimesScalar 477 129
                                Store 39(p) 478
                                Branch 460
             460:               Label
                                Line 1 256 0
             479:    7(float)   Load 456(i)
             480:    7(float)   FAdd 479 212
                                Store 456(i) 480
                                Branch 457
             459:             Label
                              Line 1 264 0
             481:    7(float) Load 455(rz)
                              ReturnValue 481
                              FunctionEnd
 43(bnoise(vf2;):    7(float) Function None 15
           42(p):      9(ptr) FunctionParameter
              44:             Label
      486(param):      9(ptr) Variable Function
                              Line 1 270 0
             484:    8(fvec2) Load 42(p)
             485:    8(fvec2) VectorTimesScalar 484 128
                              Store 486(param) 485
             487:    7(float) FunctionCall 40(fbm(vf2;) 486(param)
                              ReturnValue 487
                              FunctionEnd
49(bump(vf3;vf3;f1;):   10(fvec3) Function None 45
           46(p):     19(ptr) FunctionParameter
           47(n):     19(ptr) FunctionParameter
          48(ds):     24(ptr) FunctionParameter
              50:             Label
          490(e):      9(ptr) Variable Function
         495(n0):     24(ptr) Variable Function
      496(param):      9(ptr) Variable Function
          500(d):     19(ptr) Variable Function
      505(param):      9(ptr) Variable Function
      514(param):      9(ptr) Variable Function
                              Line 1 277 0
             492:    7(float) Load 48(ds)
             493:    7(float) FMul 491 492
             494:    8(fvec2) CompositeConstruct 493 148
                              Store 490(e) 494
                              Line 1 279 0
             497:   10(fvec3) Load 46(p)
             498:    8(fvec2) VectorShuffle 497 497 2 0
                              Store 496(param) 498
             499:    7(float) FunctionCall 43(bnoise(vf2;) 496(param)
                              Store 495(n0) 499
                              Line 1 281 0
             501:   10(fvec3) Load 46(p)
             502:    8(fvec2) VectorShuffle 501 501 2 0
             503:    8(fvec2) Load 490(e)
             504:    8(fvec2) FAdd 502 503
                              Store 505(param) 504
             506:    7(float) FunctionCall 43(bnoise(vf2;) 505(param)
             507:    7(float) Load 495(n0)
             508:    7(float) FSub 506 507
             509:   10(fvec3) Load 46(p)
             510:    8(fvec2) VectorShuffle 509 509 2 0
             511:    8(fvec2) Load 490(e)
             512:    8(fvec2) VectorShuffle 511 511 1 0
             513:    8(fvec2) FAdd 510 512
                              Store 514(param) 513
             515:    7(float) FunctionCall 43(bnoise(vf2;) 514(param)
             516:    7(float) Load 495(n0)
             517:    7(float) FSub 515 516
             518:   10(fvec3) CompositeConstruct 508 212 517
             519:     24(ptr) AccessChain 490(e) 150
             520:    7(float) Load 519
             521:   10(fvec3) CompositeConstruct 520 520 520
             522:   10(fvec3) FDiv 518 521
             524:   10(fvec3) VectorTimesScalar 522 523
                              Store 500(d) 524
                              Line 1 282 0
             525:   10(fvec3) Load 47(n)
             526:   10(fvec3) Load 47(n)
             527:   10(fvec3) Load 500(d)
             528:    7(float) Dot 526 527
             529:   10(fvec3) VectorTimesScalar 525 528
             530:   10(fvec3) Load 500(d)
             531:   10(fvec3) FSub 530 529
                              Store 500(d) 531
                              Line 1 283 0
             532:   10(fvec3) Load 47(n)
             533:   10(fvec3) Load 500(d)
             534:   10(fvec3) FSub 532 533
             535:   10(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 534
                              Store 47(n) 535
                              Line 1 284 0)#"_str
+
R"#(
             536:   10(fvec3) Load 47(n)
                              ReturnValue 536
                              FunctionEnd
54(curv(vf3;f1;):    7(float) Function None 51
           52(p):     19(ptr) FunctionParameter
           53(w):     24(ptr) FunctionParameter
              55:             Label
          539(e):      9(ptr) Variable Function
         544(t1):     24(ptr) Variable Function
      549(param):     19(ptr) Variable Function
         551(t2):     24(ptr) Variable Function
      556(param):     19(ptr) Variable Function
         558(t3):     24(ptr) Variable Function
      563(param):     19(ptr) Variable Function
         565(t4):     24(ptr) Variable Function
      570(param):     19(ptr) Variable Function
      584(param):     19(ptr) Variable Function
                              Line 1 291 0
             542:    7(float) Load 53(w)
             543:    8(fvec2) VectorTimesScalar 541 542
                              Store 539(e) 543
                              Line 1 293 0
             545:   10(fvec3) Load 52(p)
             546:    8(fvec2) Load 539(e)
             547:   10(fvec3) VectorShuffle 546 546 1 0 0
             548:   10(fvec3) FAdd 545 547
                              Store 549(param) 548
             550:    7(float) FunctionCall 22(map(vf3;) 549(param)
                              Store 544(t1) 550
                              Line 1 295 0
             552:   10(fvec3) Load 52(p)
             553:    8(fvec2) Load 539(e)
             554:   10(fvec3) VectorShuffle 553 553 0 0 1
             555:   10(fvec3) FAdd 552 554
                              Store 556(param) 555
             557:    7(float) FunctionCall 22(map(vf3;) 556(param)
                              Store 551(t2) 557
                              Line 1 297 0
             559:   10(fvec3) Load 52(p)
             560:    8(fvec2) Load 539(e)
             561:   10(fvec3) VectorShuffle 560 560 0 1 0
             562:   10(fvec3) FAdd 559 561
                              Store 563(param) 562
             564:    7(float) FunctionCall 22(map(vf3;) 563(param)
                              Store 558(t3) 564
                              Line 1 299 0
             566:   10(fvec3) Load 52(p)
             567:    8(fvec2) Load 539(e)
             568:   10(fvec3) VectorShuffle 567 567 1 1 1
             569:   10(fvec3) FAdd 566 568
                              Store 570(param) 569
             571:    7(float) FunctionCall 22(map(vf3;) 570(param)
                              Store 565(t4) 571
                              Line 1 300 0
             573:     24(ptr) AccessChain 539(e) 103
             574:    7(float) Load 573
             575:    7(float) FDiv 572 574
             576:    7(float) Load 544(t1)
             577:    7(float) Load 551(t2)
             578:    7(float) FAdd 576 577
             579:    7(float) Load 558(t3)
             580:    7(float) FAdd 578 579
             581:    7(float) Load 565(t4)
             582:    7(float) FAdd 580 581
             585:   10(fvec3) Load 52(p)
                              Store 584(param) 585
             586:    7(float) FunctionCall 22(map(vf3;) 584(param)
             587:    7(float) FMul 583 586
             588:    7(float) FSub 582 587
             589:    7(float) FMul 575 588
                              ReturnValue 589
                              FunctionEnd
61(fog(vf3;vf3;vf3;f1;):   10(fvec3) Function None 56
          57(ro):     19(ptr) FunctionParameter
          58(rd):     19(ptr) FunctionParameter
         59(col):     19(ptr) FunctionParameter
          60(ds):     24(ptr) FunctionParameter
              62:             Label
        592(pos):     19(ptr) Variable Function
         598(mx):     24(ptr) Variable Function
      609(param):      9(ptr) Variable Function
        614(den):     24(ptr) Variable Function
        634(sdt):     24(ptr) Variable Function
   642(fogColor):     19(ptr) Variable Function
                              Line 1 307 0
             593:   10(fvec3) Load 57(ro)
             594:   10(fvec3) Load 58(rd)
             595:    7(float) Load 60(ds)
             596:   10(fvec3) VectorTimesScalar 594 595
             597:   10(fvec3) FAdd 593 596
                              Store 592(pos) 597
                              Line 1 309 0
             599:   10(fvec3) Load 592(pos)
             600:    8(fvec2) VectorShuffle 599 599 2 0
             602:    8(fvec2) VectorTimesScalar 600 601
             603:    104(ptr) AccessChain 101 313
             604:    7(float) Load 603
             606:    7(float) FMul 604 605
             607:    8(fvec2) CompositeConstruct 606 606
             608:    8(fvec2) FSub 602 607
                              Store 609(param) 608
             610:    7(float) FunctionCall 40(fbm(vf2;) 609(param)
             611:    7(float) FSub 610 142
             613:    7(float) FMul 611 612
                              Store 598(mx) 613
                              Line 1 311 0
             616:     24(ptr) AccessChain 57(ro) 103
             617:    7(float) Load 616
             618:    7(float) FNegate 617
             619:    7(float) FMul 618 212
             620:    7(float) ExtInst 2(GLSL.std.450) 27(Exp) 619
             621:    7(float) FMul 615 620
             622:    7(float) Load 60(ds)
             623:    7(float) FNegate 622
             624:     24(ptr) AccessChain 58(rd) 103
             625:    7(float) Load 624
             626:    7(float) FMul 623 625
             627:    7(float) FMul 626 212
             628:    7(float) ExtInst 2(GLSL.std.450) 27(Exp) 627
             629:    7(float) FSub 212 628
             630:    7(float) FMul 621 629
             631:     24(ptr) AccessChain 58(rd) 103
             632:    7(float) Load 631
             633:    7(float) FDiv 630 632
                              Store 614(den) 633
                              Line 1 313 0
             635:   10(fvec3) Load 58(rd)
             640:    7(float) Dot 635 639
             641:    7(float) ExtInst 2(GLSL.std.450) 40(FMax) 640 148
                              Store 634(sdt) 641
                              Line 1 315 0
             650:    7(float) Load 634(sdt)
             651:    7(float) ExtInst 2(GLSL.std.450) 26(Pow) 650 129
             652:    7(float) Load 598(mx)
             653:    7(float) FMul 652 142
             654:    7(float) FAdd 651 653
             655:   10(fvec3) CompositeConstruct 654 654 654
             656:   10(fvec3) ExtInst 2(GLSL.std.450) 46(FMix) 645 649 655
                              Store 642(fogColor) 656
                              Line 1 316 0
             657:   10(fvec3) Load 59(col)
             658:   10(fvec3) Load 642(fogColor)
             659:    7(float) Load 614(den)
             660:    7(float) Load 598(mx)
             661:    7(float) FAdd 659 660
             662:    7(float) ExtInst 2(GLSL.std.450) 43(FClamp) 661 148 212
             663:   10(fvec3) CompositeConstruct 662 662 662
             664:   10(fvec3) ExtInst 2(GLSL.std.450) 46(FMix) 657 658 663
                              ReturnValue 664
                              FunctionEnd
67(linstep(f1;f1;f1;):    7(float) Function None 63
          64(mn):     24(ptr) FunctionParameter
          65(mx):     24(ptr) FunctionParameter
           66(x):     24(ptr) FunctionParameter
              68:             Label
                              Line 1 322 0
             667:    7(float) Load 66(x)
             668:    7(float) Load 64(mn)
             669:    7(float) FSub 667 668
             670:    7(float) Load 65(mx)
             671:    7(float) Load 64(mn)
             672:    7(float) FSub 670 671
             673:    7(float) FDiv 669 672
             674:    7(float) ExtInst 2(GLSL.std.450) 43(FClamp) 673 148 212
                              ReturnValue 674
                              FunctionEnd
72(scatter(vf3;vf3;):   10(fvec3) Function None 69
          70(ro):     19(ptr) FunctionParameter
          71(rd):     19(ptr) FunctionParameter
              73:             Label
         677(sd):     24(ptr) Variable Function
        683(dtp):     24(ptr) Variable Function
       693(hori):     24(ptr) Variable Function
      695(param):     24(ptr) Variable Function
      696(param):     24(ptr) Variable Function
      697(param):     24(ptr) Variable Function
      702(param):     24(ptr) Variable Function
      703(param):     24(ptr) Variable Function
      704(param):     24(ptr) Variable Function
        714(col):     19(ptr) Variable Function
                              Line 1 329 0
             678:   10(fvec3) Load 71(rd)
             679:    7(float) Dot 639 678
             680:    7(float) FMul 679 142
             681:    7(float) FAdd 680 142
             682:    7(float) ExtInst 2(GLSL.std.450) 40(FMax) 681 148
                              Store 677(sd) 682
                              Line 1 331 0
             685:   10(fvec3) Load 70(ro)
             686:   10(fvec3) Load 71(rd)
             687:   10(fvec3) VectorTimesScalar 686 363
             688:   10(fvec3) FAdd 685 687
             689:    7(float) CompositeExtract 688 1
             691:    7(float) FMul 689 690
             692:    7(float) FSub 684 691
                              Store 683(dtp) 692
                              Line 1 333 0
                              Store 695(param) 694
                              Store 696(param) 148
             698:    7(float) Load 683(dtp)
                              Store 697(param) 698
             699:    7(float) FunctionCall 67(linstep(f1;f1;f1;) 695(param) 696(param) 697(param)
                              Store 702(param) 700
                              Store 703(param) 701
             705:    7(float) Load 683(dtp)
                              Store 704(param) 705
             706:    7(float) FunctionCall 67(linstep(f1;f1;f1;) 702(param) 703(param) 704(param)
             707:    7(float) FSub 699 706
             708:    7(float) FMul 707 212
                              Store 693(hori) 708
                              Line 1 334 0
             709:    7(float) Load 677(sd)
             711:    7(float) ExtInst 2(GLSL.std.450) 26(Pow) 709 710
             712:    7(float) Load 693(hori)
             713:    7(float) FMul 712 711
                              Store 693(hori) 713
                              Line 1 336 0
                              Store 714(col) 715
                              Line 1 337 0
             716:    7(float) Load 693(hori)
             718:    7(float) ExtInst 2(GLSL.std.450) 26(Pow) 716 717
             720:   10(fvec3) VectorTimesScalar 719 718
             721:   10(fvec3) VectorTimesScalar 720 128
             722:   10(fvec3) Load 714(col)
             723:   10(fvec3) FAdd 722 721
                              Store 714(col) 723
                              Line 1 338 0
             724:    7(float) Load 693(hori)
             726:    7(float) ExtInst 2(GLSL.std.450) 26(Pow) 724 725
             729:   10(fvec3) VectorTimesScalar 728 726
             730:   10(fvec3) VectorTimesScalar 729 615
             731:   10(fvec3) Load 714(col)
             732:   10(fvec3) FAdd 731 730
                              Store 714(col) 732
                              Line 1 339 0
             733:    7(float) Load 693(hori)
             735:    7(float) ExtInst 2(GLSL.std.450) 26(Pow) 733 734
             738:   10(fvec3) VectorTimesScalar 737 735
             739:   10(fvec3) VectorTimesScalar 738 299
             740:   10(fvec3) Load 714(col)
             741:   10(fvec3) FAdd 740 739
                              Store 714(col) 741
                              Line 1 340 0
             742:   10(fvec3) Load 714(col)
                              ReturnValue 742
                              FunctionEnd
 76(hash33(vf3;):   10(fvec3) Function None 74
           75(p):     19(ptr) FunctionParameter
              77:             Label
                              Line 1 346 0
             745:   10(fvec3) Load 75(p)
             750:   10(fvec3) FMul 745 749
             751:   10(fvec3) ExtInst 2(GLSL.std.450) 10(Fract) 750
                              Store 75(p) 751
                              Line 1 347 0
             752:   10(fvec3) Load 75(p)
             753:   10(fvec3) VectorShuffle 752 752 2 0 1
             754:   10(fvec3) Load 75(p)
             755:   10(fvec3) VectorShuffle 754 754 1 0 2
             757:   10(fvec3) CompositeConstruct 756 756 756
             758:   10(fvec3) FAdd 755 757
             759:    7(float) Dot 753 758
             760:   10(fvec3) Load 75(p)
             761:   10(fvec3) CompositeConstruct 759 759 759
             762:   10(fvec3) FAdd 760 761
                              Store 75(p) 762
                              Line 1 348 0
             763:     24(ptr) AccessChain 75(p) 150
             764:    7(float) Load 763
             765:     24(ptr) AccessChain 75(p) 103
             766:    7(float) Load 765
             767:    7(float) FMul 764 766
             769:     24(ptr) AccessChain 75(p) 768
             770:    7(float) Load 769
             771:     24(ptr) AccessChain 75(p) 150
             772:    7(float) Load 771
             773:    7(float) FMul 770 772
             774:     24(ptr) AccessChain 75(p) 103
             775:    7(float) Load 774
             776:     24(ptr) AccessChain 75(p) 768
             777:    7(float) Load 776
             778:    7(float) FMul 775 777
             779:   10(fvec3) CompositeConstruct 767 773 778
             780:   10(fvec3) ExtInst 2(GLSL.std.450) 10(Fract) 779
                              ReturnValue 780
                              FunctionEnd
  79(stars(vf3;):   10(fvec3) Function None 74
           78(p):     19(ptr) FunctionParameter
              80:             Label
          783(c):     19(ptr) Variable Function
        784(res):     24(ptr) Variable Function
          788(i):     24(ptr) Variable Function
          796(q):     19(ptr) Variable Function
         804(id):     19(ptr) Variable Function
         810(rn):      9(ptr) Variable Function
      811(param):     19(ptr) Variable Function
         815(c2):     24(ptr) Variable Function
                              Line 1 355 0
                              Store 783(c) 715
                              Line 1 357 0
             785:    104(ptr) AccessChain 101 102 150
             786:    7(float) Load 785
             787:    7(float) FMul 786 299
                              Store 784(res) 787
                              Line 1 360 0
                              Store 788(i) 148
                              Branch 789
                              Line 1 361 0
             789:             Label
                              LoopMerge 791 792 None
                              Branch 793
             793:             Label
             794:    7(float) Load 788(i)
             795:   260(bool) FOrdLessThan 794 128
                              BranchConditional 795 790 791
             790:               Label
                                Line 1 364 0
             797:   10(fvec3)   Load 78(p)
             798:    7(float)   Load 784(res)
             799:    7(float)   FMul 572 798
             800:   10(fvec3)   VectorTimesScalar 797 799
             801:   10(fvec3)   ExtInst 2(GLSL.std.450) 10(Fract) 800
             802:   10(fvec3)   CompositeConstruct 142 142 142
             803:   10(fvec3)   FSub 801 802
                                Store 796(q) 803
                                Line 1 366 0
             805:   10(fvec3)   Load 78(p)
             806:    7(float)   Load 784(res)
             807:    7(float)   FMul 572 806
             808:   10(fvec3)   VectorTimesScalar 805 807
             809:   10(fvec3)   ExtInst 2(GLSL.std.450) 8(Floor) 808
                                Store 804(id) 809
                                Line 1 368 0)#"_str
+
R"#(
             812:   10(fvec3)   Load 804(id)
                                Store 811(param) 812
             813:   10(fvec3)   FunctionCall 76(hash33(vf3;) 811(param)
             814:    8(fvec2)   VectorShuffle 813 813 0 1
                                Store 810(rn) 814
                                Line 1 370 0
             816:   10(fvec3)   Load 796(q)
             817:    7(float)   ExtInst 2(GLSL.std.450) 66(Length) 816
             818:    7(float)   ExtInst 2(GLSL.std.450) 49(SmoothStep) 148 303 817
             819:    7(float)   FSub 212 818
                                Store 815(c2) 819
                                Line 1 371 0
             820:     24(ptr)   AccessChain 810(rn) 150
             821:    7(float)   Load 820
             822:    7(float)   Load 788(i)
             823:    7(float)   Load 788(i)
             824:    7(float)   FMul 822 823
             826:    7(float)   FMul 824 825
             827:    7(float)   FAdd 399 826
             828:    7(float)   ExtInst 2(GLSL.std.450) 48(Step) 821 827
             829:    7(float)   Load 815(c2)
             830:    7(float)   FMul 829 828
                                Store 815(c2) 830
                                Line 1 372 0
             831:    7(float)   Load 815(c2)
             836:     24(ptr)   AccessChain 810(rn) 103
             837:    7(float)   Load 836
             838:   10(fvec3)   CompositeConstruct 837 837 837
             839:   10(fvec3)   ExtInst 2(GLSL.std.450) 46(FMix) 833 835 838
             840:   10(fvec3)   VectorTimesScalar 839 727
             841:   10(fvec3)   CompositeConstruct 317 317 317
             842:   10(fvec3)   FAdd 840 841
             843:   10(fvec3)   VectorTimesScalar 842 831
             844:   10(fvec3)   Load 783(c)
             845:   10(fvec3)   FAdd 844 843
                                Store 783(c) 845
                                Line 1 373 0
             847:   10(fvec3)   Load 78(p)
             848:   10(fvec3)   VectorTimesScalar 847 846
                                Store 78(p) 848
                                Branch 792
             792:               Label
                                Line 1 361 0
             849:    7(float)   Load 788(i)
             850:    7(float)   FAdd 849 212
                                Store 788(i) 850
                                Branch 789
             791:             Label
                              Line 1 377 0
             851:   10(fvec3) Load 783(c)
             852:   10(fvec3) Load 783(c)
             853:   10(fvec3) FMul 851 852
             854:   10(fvec3) VectorTimesScalar 853 388
                              ReturnValue 854
                              FunctionEnd
86(mainImage(vf4;vf2;):           3 Function None 83
   84(fragColor):     82(ptr) FunctionParameter
   85(fragCoord):      9(ptr) FunctionParameter
              87:             Label
          857(q):      9(ptr) Variable Function
          864(p):      9(ptr) Variable Function
         877(mo):      9(ptr) Variable Function
             888:      9(ptr) Variable Function
         928(ro):     19(ptr) Variable Function
        941(eye):     19(ptr) Variable Function
      953(right):     19(ptr) Variable Function
         965(up):     19(ptr) Variable Function
         970(rd):     19(ptr) Variable Function
      999(count):     24(ptr) Variable Function
        1000(rz):     24(ptr) Variable Function
     1001(param):     19(ptr) Variable Function
     1003(param):     19(ptr) Variable Function
     1005(param):     24(ptr) Variable Function
     1008(scatt):     19(ptr) Variable Function
     1009(param):     19(ptr) Variable Function
     1011(param):     19(ptr) Variable Function
        1014(bg):     19(ptr) Variable Function
     1015(param):     19(ptr) Variable Function
       1025(col):     19(ptr) Variable Function
       1027(pos):     19(ptr) Variable Function
       1033(nor):     19(ptr) Variable Function
     1034(param):     19(ptr) Variable Function
     1036(param):     24(ptr) Variable Function
     1043(param):     19(ptr) Variable Function
     1045(param):     19(ptr) Variable Function
     1047(param):     24(ptr) Variable Function
       1050(amb):     24(ptr) Variable Function
       1056(dif):     24(ptr) Variable Function
       1060(bac):     24(ptr) Variable Function
       1067(spe):     24(ptr) Variable Function
       1074(fre):     24(ptr) Variable Function
      1081(brdf):     19(ptr) Variable Function
       1101(crv):     24(ptr) Variable Function
     1102(param):     19(ptr) Variable Function
     1104(param):     24(ptr) Variable Function
      1107(crv2):     24(ptr) Variable Function
     1108(param):     19(ptr) Variable Function
     1110(param):     24(ptr) Variable Function
     1144(param):     19(ptr) Variable Function
     1146(param):     19(ptr) Variable Function
     1148(param):     19(ptr) Variable Function
     1150(param):     24(ptr) Variable Function
      1194(past):     82(ptr) Variable Function
     1199(tOver):     24(ptr) Variable Function
                              Line 1 384 0
             858:    8(fvec2) Load 85(fragCoord)
             860:    859(ptr) AccessChain 101 102
             861:   81(fvec4) Load 860
             862:    8(fvec2) VectorShuffle 861 861 0 1
             863:    8(fvec2) FDiv 858 862
                              Store 857(q) 863
                              Line 1 386 0
             865:    8(fvec2) Load 857(q)
             866:    8(fvec2) CompositeConstruct 142 142
             867:    8(fvec2) FSub 865 866
                              Store 864(p) 867
                              Line 1 387 0
             868:    104(ptr) AccessChain 101 102 150
             869:    7(float) Load 868
             870:    104(ptr) AccessChain 101 102 103
             871:    7(float) Load 870
             872:    7(float) FDiv 869 871
             873:     24(ptr) AccessChain 864(p) 150
             874:    7(float) Load 873
             875:    7(float) FMul 874 872
             876:     24(ptr) AccessChain 864(p) 150
                              Store 876 875
                              Line 1 389 0
             879:    859(ptr) AccessChain 101 878
             880:   81(fvec4) Load 879
             881:    8(fvec2) VectorShuffle 880 880 0 1
             882:    859(ptr) AccessChain 101 102
             883:   81(fvec4) Load 882
             884:    8(fvec2) VectorShuffle 883 883 0 1
             885:    8(fvec2) FDiv 881 884
             886:    8(fvec2) CompositeConstruct 142 142
             887:    8(fvec2) FSub 885 886
                              Store 877(mo) 887
                              Line 1 390 0
             889:    8(fvec2) Load 877(mo)
             892:  891(bvec2) FOrdEqual 889 890
             893:   260(bool) All 892
                              SelectionMerge 895 None
                              BranchConditional 893 894 898
             894:               Label
                                Store 877(mo) 897
                                Store 888 897
                                Branch 895
             898:               Label
             899:    8(fvec2)   Load 877(mo)
                                Store 888 899
                                Branch 895
             895:             Label
             900:    8(fvec2) Load 888
                              Store 877(mo) 900
                              Line 1 391 0
             902:     24(ptr) AccessChain 877(mo) 150
             903:    7(float) Load 902
             904:    7(float) FMul 903 901
             905:     24(ptr) AccessChain 877(mo) 150
                              Store 905 904
                              Line 1 392 0
             908:    8(fvec2) Load 877(mo)
             909:    8(fvec2) FSub 908 907
                              Store 877(mo) 909
                              Line 1 393 0
             910:    104(ptr) AccessChain 101 102 150
             911:    7(float) Load 910
             912:    104(ptr) AccessChain 101 102 103
             913:    7(float) Load 912
             914:    7(float) FDiv 911 913
             915:     24(ptr) AccessChain 877(mo) 150
             916:    7(float) Load 915
             917:    7(float) FMul 916 914
             918:     24(ptr) AccessChain 877(mo) 150
                              Store 918 917
                              Line 1 394 0
             919:    104(ptr) AccessChain 101 313
             920:    7(float) Load 919
             921:    7(float) FMul 920 572
             922:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 921
             923:    7(float) FMul 922 612
             924:     24(ptr) AccessChain 877(mo) 150
             925:    7(float) Load 924
             926:    7(float) FAdd 925 923
             927:     24(ptr) AccessChain 877(mo) 150
                              Store 927 926
                              Line 1 396 0
             930:    104(ptr) AccessChain 101 313
             931:    7(float) Load 930
             932:    7(float) FMul 931 612
             933:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 932
             934:    7(float) FMul 933 727
             936:    7(float) FAdd 934 935
             937:    104(ptr) AccessChain 101 313
             938:    7(float) Load 937
             939:    7(float) FNegate 938
             940:   10(fvec3) CompositeConstruct 929 936 939
                              Store 928(ro) 940
                              Line 1 398 0
             942:     24(ptr) AccessChain 877(mo) 150
             943:    7(float) Load 942
             944:    7(float) ExtInst 2(GLSL.std.450) 14(Cos) 943
             945:     24(ptr) AccessChain 877(mo) 103
             946:    7(float) Load 945
             947:    7(float) FAdd 274 946
             948:     24(ptr) AccessChain 877(mo) 150
             949:    7(float) Load 948
             950:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 949
             951:   10(fvec3) CompositeConstruct 944 947 950
             952:   10(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 951
                              Store 941(eye) 952
                              Line 1 400 0
             954:     24(ptr) AccessChain 877(mo) 150
             955:    7(float) Load 954
             957:    7(float) FAdd 955 956
             958:    7(float) ExtInst 2(GLSL.std.450) 14(Cos) 957
             959:     24(ptr) AccessChain 877(mo) 150
             960:    7(float) Load 959
             961:    7(float) FAdd 960 956
             962:    7(float) ExtInst 2(GLSL.std.450) 13(Sin) 961
             963:   10(fvec3) CompositeConstruct 958 148 962
             964:   10(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 963
                              Store 953(right) 964
                              Line 1 402 0
             966:   10(fvec3) Load 953(right)
             967:   10(fvec3) Load 941(eye)
             968:   10(fvec3) ExtInst 2(GLSL.std.450) 68(Cross) 966 967
             969:   10(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 968
                              Store 965(up) 969
                              Line 1 404 0
             971:     24(ptr) AccessChain 864(p) 150
             972:    7(float) Load 971
             973:   10(fvec3) Load 953(right)
             974:   10(fvec3) VectorTimesScalar 973 972
             975:     24(ptr) AccessChain 864(p) 103
             976:    7(float) Load 975
             977:   10(fvec3) Load 965(up)
             978:   10(fvec3) VectorTimesScalar 977 976
             979:   10(fvec3) FAdd 974 978
             981:   10(fvec3) VectorTimesScalar 979 980
             982:   10(fvec3) Load 941(eye)
             983:   10(fvec3) FAdd 981 982
             984:   10(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 983
                              Store 970(rd) 984
                              Line 1 405 0
             985:     24(ptr) AccessChain 864(p) 150
             986:    7(float) Load 985
             987:     24(ptr) AccessChain 864(p) 150
             988:    7(float) Load 987
             989:    7(float) FMul 986 988
             991:    7(float) FMul 989 990
             992:    7(float) ExtInst 2(GLSL.std.450) 4(FAbs) 991
             993:     24(ptr) AccessChain 970(rd) 103
             994:    7(float) Load 993
             995:    7(float) FAdd 994 992
             996:     24(ptr) AccessChain 970(rd) 103
                              Store 996 995
                              Line 1 406 0
             997:   10(fvec3) Load 970(rd)
             998:   10(fvec3) ExtInst 2(GLSL.std.450) 69(Normalize) 997
                              Store 970(rd) 998
                              Line 1 408 0
                              Store 999(count) 148
                              Line 1 410 0
            1002:   10(fvec3) Load 928(ro)
                              Store 1001(param) 1002
            1004:   10(fvec3) Load 970(rd)
                              Store 1003(param) 1004
            1006:    7(float) FunctionCall 29(march(vf3;vf3;f1;) 1001(param) 1003(param) 1005(param)
            1007:    7(float) Load 1005(param)
                              Store 999(count) 1007
                              Store 1000(rz) 1006
                              Line 1 412 0
            1010:   10(fvec3) Load 928(ro)
                              Store 1009(param) 1010
            1012:   10(fvec3) Load 970(rd)
                              Store 1011(param) 1012
            1013:   10(fvec3) FunctionCall 72(scatter(vf3;vf3;) 1009(param) 1011(param)
                              Store 1008(scatt) 1013
                              Line 1 414 0
            1016:   10(fvec3) Load 970(rd)
                              Store 1015(param) 1016
            1017:   10(fvec3) FunctionCall 79(stars(vf3;) 1015(param)
            1018:   10(fvec3) Load 1008(scatt)
            1021:    7(float) Dot 1018 1020
            1022:    7(float) ExtInst 2(GLSL.std.450) 43(FClamp) 1021 148 212
            1023:    7(float) FSub 212 1022
            1024:   10(fvec3) VectorTimesScalar 1017 1023
                              Store 1014(bg) 1024
                              Line 1 416 0
            1026:   10(fvec3) Load 1014(bg)
                              Store 1025(col) 1026
                              Line 1 418 0
            1028:   10(fvec3) Load 928(ro)
            1029:    7(float) Load 1000(rz)
            1030:   10(fvec3) Load 970(rd)
            1031:   10(fvec3) VectorTimesScalar 1030 1029
            1032:   10(fvec3) FAdd 1028 1031
                              Store 1027(pos) 1032
                              Line 1 420 0
            1035:   10(fvec3) Load 1027(pos)
                              Store 1034(param) 1035
            1037:    7(float) Load 1000(rz)
                              Store 1036(param) 1037
            1038:   10(fvec3) FunctionCall 34(normal(vf3;f1;) 1034(param) 1036(param)
                              Store 1033(nor) 1038
                              Line 1 421 0
            1039:    7(float) Load 1000(rz)
            1040:   260(bool) FOrdLessThan 1039 363
                              SelectionMerge 1042 None
                              BranchConditional 1040 1041 1042
            1041:               Label
                                Line 1 423 0
            1044:   10(fvec3)   Load 1027(pos)
                                Store 1043(param) 1044
            1046:   10(fvec3)   Load 1033(nor)
                                Store 1045(param) 1046
            1048:    7(float)   Load 1000(rz)
                                Store 1047(param) 1048
            1049:   10(fvec3)   FunctionCall 49(bump(vf3;vf3;f1;) 1043(param) 1045(param) 1047(param)
                                Store 1033(nor) 1049
                                Line 1 425 0
            1051:     24(ptr)   AccessChain 1033(nor) 103
            1052:    7(float)   Load 1051
            1053:    7(float)   FMul 142 1052)#"_str
+
R"#(
            1054:    7(float)   FAdd 142 1053
            1055:    7(float)   ExtInst 2(GLSL.std.450) 43(FClamp) 1054 148 212
                                Store 1050(amb) 1055
                                Line 1 427 0
            1057:   10(fvec3)   Load 1033(nor)
            1058:    7(float)   Dot 1057 639
            1059:    7(float)   ExtInst 2(GLSL.std.450) 43(FClamp) 1058 148 212
                                Store 1056(dif) 1059
                                Line 1 429 0
            1061:   10(fvec3)   Load 1033(nor)
            1065:    7(float)   Dot 1061 1064
            1066:    7(float)   ExtInst 2(GLSL.std.450) 43(FClamp) 1065 148 212
                                Store 1060(bac) 1066
                                Line 1 431 0
            1068:   10(fvec3)   Load 970(rd)
            1069:   10(fvec3)   Load 1033(nor)
            1070:   10(fvec3)   ExtInst 2(GLSL.std.450) 71(Reflect) 1068 1069
            1071:    7(float)   Dot 1070 639
            1072:    7(float)   ExtInst 2(GLSL.std.450) 43(FClamp) 1071 148 212
            1073:    7(float)   ExtInst 2(GLSL.std.450) 26(Pow) 1072 701
                                Store 1067(spe) 1073
                                Line 1 433 0
            1075:   10(fvec3)   Load 1033(nor)
            1076:   10(fvec3)   Load 970(rd)
            1077:    7(float)   Dot 1075 1076
            1078:    7(float)   FAdd 212 1077
            1079:    7(float)   ExtInst 2(GLSL.std.450) 43(FClamp) 1078 148 212
            1080:    7(float)   ExtInst 2(GLSL.std.450) 26(Pow) 1079 129
                                Store 1074(fre) 1080
                                Line 1 435 0
            1082:    7(float)   Load 1050(amb)
            1083:    7(float)   FMul 212 1082
            1087:   10(fvec3)   VectorTimesScalar 1086 1083
                                Store 1081(brdf) 1087
                                Line 1 436 0
            1088:    7(float)   Load 1060(bac)
            1090:   10(fvec3)   VectorTimesScalar 1089 1088
            1091:   10(fvec3)   Load 1081(brdf)
            1092:   10(fvec3)   FAdd 1091 1090
                                Store 1081(brdf) 1092
                                Line 1 437 0
            1094:    7(float)   Load 1056(dif)
            1095:    7(float)   FMul 1093 1094
            1097:   10(fvec3)   VectorTimesScalar 1096 1095
            1098:   10(fvec3)   Load 1081(brdf)
            1099:   10(fvec3)   FAdd 1098 1097
                                Store 1081(brdf) 1099
                                Line 1 438 0
                                Store 1025(col) 1100
                                Line 1 440 0
            1103:   10(fvec3)   Load 1027(pos)
                                Store 1102(param) 1103
                                Store 1104(param) 129
            1105:    7(float)   FunctionCall 54(curv(vf3;f1;) 1102(param) 1104(param)
            1106:    7(float)   FMul 1105 212
                                Store 1101(crv) 1106
                                Line 1 442 0
            1109:   10(fvec3)   Load 1027(pos)
                                Store 1108(param) 1109
                                Store 1110(param) 736
            1111:    7(float)   FunctionCall 54(curv(vf3;f1;) 1108(param) 1110(param)
            1113:    7(float)   FMul 1111 1112
                                Store 1107(crv2) 1113
                                Line 1 443 0
            1114:    7(float)   Load 1101(crv)
            1115:    7(float)   FMul 1114 834
            1116:    7(float)   ExtInst 2(GLSL.std.450) 43(FClamp) 1115 540 212
            1118:   10(fvec3)   VectorTimesScalar 1117 1116
            1119:   10(fvec3)   Load 1025(col)
            1120:   10(fvec3)   FAdd 1119 1118
                                Store 1025(col) 1120
                                Line 1 444 0
            1121:   10(fvec3)   Load 1025(col)
            1122:   10(fvec3)   Load 1081(brdf)
            1123:   10(fvec3)   FMul 1121 1122
            1124:   10(fvec3)   Load 1025(col)
            1125:    7(float)   Load 1067(spe)
            1126:   10(fvec3)   VectorTimesScalar 1124 1125
            1127:   10(fvec3)   VectorTimesScalar 1126 601
            1128:   10(fvec3)   FAdd 1123 1127
            1129:    7(float)   Load 1074(fre)
            1130:    7(float)   FMul 601 1129
            1131:   10(fvec3)   Load 1025(col)
            1132:   10(fvec3)   VectorTimesScalar 1131 1130
            1133:   10(fvec3)   FAdd 1128 1132
                                Store 1025(col) 1133
                                Line 1 445 0
            1134:    7(float)   Load 1101(crv)
            1135:    7(float)   FMul 1134 212
            1136:    7(float)   FAdd 1135 212
            1137:   10(fvec3)   Load 1025(col)
            1138:   10(fvec3)   VectorTimesScalar 1137 1136
                                Store 1025(col) 1138
                                Line 1 446 0
            1139:    7(float)   Load 1107(crv2)
            1140:    7(float)   FMul 1139 212
            1141:    7(float)   FAdd 1140 212
            1142:   10(fvec3)   Load 1025(col)
            1143:   10(fvec3)   VectorTimesScalar 1142 1141
                                Store 1025(col) 1143
                                Branch 1042
            1042:             Label
                              Line 1 450 0
            1145:   10(fvec3) Load 928(ro)
                              Store 1144(param) 1145
            1147:   10(fvec3) Load 970(rd)
                              Store 1146(param) 1147
            1149:   10(fvec3) Load 1025(col)
                              Store 1148(param) 1149
            1151:    7(float) Load 1000(rz)
                              Store 1150(param) 1151
            1152:   10(fvec3) FunctionCall 61(fog(vf3;vf3;vf3;f1;) 1144(param) 1146(param) 1148(param) 1150(param)
                              Store 1025(col) 1152
                              Line 1 451 0
            1153:   10(fvec3) Load 1025(col)
            1154:   10(fvec3) Load 1014(bg)
            1156:    7(float) Load 1000(rz)
            1157:    7(float) ExtInst 2(GLSL.std.450) 49(SmoothStep) 1155 363 1156
            1158:   10(fvec3) CompositeConstruct 1157 1157 1157
            1159:   10(fvec3) ExtInst 2(GLSL.std.450) 46(FMix) 1153 1154 1158
                              Store 1025(col) 1159
                              Line 1 452 0
            1160:   10(fvec3) Load 1008(scatt)
            1161:   10(fvec3) Load 1025(col)
            1162:   10(fvec3) FAdd 1161 1160
                              Store 1025(col) 1162
                              Line 1 453 0
            1163:   10(fvec3) Load 1025(col)
            1166:   10(fvec3) ExtInst 2(GLSL.std.450) 26(Pow) 1163 1165
                              Store 1025(col) 1166
                              Line 1 454 0
            1167:   10(fvec3) Load 1025(col)
            1168:   10(fvec3) Load 1025(col)
            1169:   10(fvec3) CompositeConstruct 148 148 148
            1170:   10(fvec3) CompositeConstruct 212 212 212
            1171:   10(fvec3) ExtInst 2(GLSL.std.450) 49(SmoothStep) 1169 1170 1168
            1172:   10(fvec3) CompositeConstruct 612 612 612
            1173:   10(fvec3) ExtInst 2(GLSL.std.450) 46(FMix) 1167 1171 1172
                              Store 1025(col) 1173
                              Line 1 455 0
            1175:     24(ptr) AccessChain 857(q) 150
            1176:    7(float) Load 1175
            1177:    7(float) FMul 1174 1176
            1178:     24(ptr) AccessChain 857(q) 103
            1179:    7(float) Load 1178
            1180:    7(float) FMul 1177 1179
            1181:     24(ptr) AccessChain 857(q) 150
            1182:    7(float) Load 1181
            1183:    7(float) FSub 212 1182
            1184:    7(float) FMul 1180 1183
            1185:     24(ptr) AccessChain 857(q) 103
            1186:    7(float) Load 1185
            1187:    7(float) FSub 212 1186
            1188:    7(float) FMul 1184 1187
            1189:    7(float) ExtInst 2(GLSL.std.450) 26(Pow) 1188 601
            1190:    7(float) FMul 1189 834
            1191:    7(float) FAdd 1190 601
            1192:   10(fvec3) Load 1025(col)
            1193:   10(fvec3) VectorTimesScalar 1192 1191
                              Store 1025(col) 1193
                              Line 1 457 0
            1196:         137 Load 1195(iChannel1)
            1197:    8(fvec2) Load 857(q)
            1198:   81(fvec4) ImageSampleImplicitLod 1196 1197
                              Store 1194(past) 1198
                              Line 1 459 0
            1201:    104(ptr) AccessChain 101 1200
            1202:    7(float) Load 1201
            1204:    7(float) FSub 1202 1203
            1205:    7(float) ExtInst 2(GLSL.std.450) 43(FClamp) 1204 148 212
                              Store 1199(tOver) 1205
                              Line 1 460 0
            1206:    7(float) Load 999(count)
            1207:    7(float) Load 1000(rz)
            1209:    7(float) ExtInst 2(GLSL.std.450) 26(Pow) 1207 1208
            1210:    7(float) FDiv 1206 1209
            1212:   260(bool) FOrdGreaterThan 1210 1211
                              SelectionMerge 1214 None
                              BranchConditional 1212 1213 1214
            1213:               Label
                                Line 1 462 0
            1215:   10(fvec3)   Load 1025(col)
            1216:   81(fvec4)   Load 1194(past)
            1217:   10(fvec3)   VectorShuffle 1216 1216 0 1 2
            1218:    104(ptr)   AccessChain 101 1200
            1219:    7(float)   Load 1218
            1220:    7(float)   FMul 1219 734
            1221:    7(float)   FSub 291 1220
            1222:    7(float)   ExtInst 2(GLSL.std.450) 43(FClamp) 1221 148 212
            1223:   10(fvec3)   CompositeConstruct 1222 1222 1222
            1224:   10(fvec3)   ExtInst 2(GLSL.std.450) 46(FMix) 1215 1217 1223
                                Store 1025(col) 1224
                                Branch 1214
            1214:             Label
                              Line 1 465 0
            1225:   10(fvec3) Load 1025(col)
            1226:    7(float) CompositeExtract 1225 0
            1227:    7(float) CompositeExtract 1225 1
            1228:    7(float) CompositeExtract 1225 2
            1229:   81(fvec4) CompositeConstruct 1226 1227 1228 212
                              Store 84(fragColor) 1229
                              Return
                              FunctionEnd
)#"_str );

};
};
