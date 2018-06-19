# Pipeline Compiler

Supported input shaders:
- GLSL 4.5
- GXSL

Supported output shaders:
- GLSL 4.5
- GLSL Vulkan 1.1
- OpenCL 1.2 source code and asm
- C++ shaders for software renderer
- HLSL - unstable (WIP)

Features:
- Export buffer types from shader to C++ code.
- Same align for buffer fields in all shaders.


## Pipeline format

```cpp
// Packing: Std140
struct DynamicBuffer_SSBO final
{
	float2  f2;			// offset: 0, align: 8
	float2  _padding0;	// offset: 8, align: 8
	int4    i4;			// offset: 16, align: 16

	// dynamic element
	// Element  arr[];   offset: 32, align: 16
	struct Element final
	{
		int2		i2;			// offset: 0, align: 8
		Bool32		b1;			// offset: 8, align: 4
		float		_padding0;	// offset: 12, align: 4
		float2		f2;			// offset: 16, align: 8
		float2		_padding1;	// offset: 24, align: 8
		int4		i3;			// offset: 32, align: 16
		Bool32_2	b2;			// offset: 48, align: 8
		float2		_padding2;	// offset: 56, align: 8

		Element ()
		{
			// compile-time tests for correct fields offsets
			STATIC_ASSERT( sizeof(Element) == 64 );
			STATIC_ASSERT( (offsetof( Element, i2 ) == 0) and (sizeof( i2 ) == 8) );
			STATIC_ASSERT( (offsetof( Element, b1 ) == 8) and (sizeof( b1 ) == 4) );
			STATIC_ASSERT( (offsetof( Element, _padding0 ) == 12) and (sizeof( _padding0 ) == 4) );
			STATIC_ASSERT( (offsetof( Element, f2 ) == 16) and (sizeof( f2 ) == 8) );
			STATIC_ASSERT( (offsetof( Element, _padding1 ) == 24) and (sizeof( _padding1 ) == 8) );
			STATIC_ASSERT( (offsetof( Element, i3 ) == 32) and (sizeof( i3 ) == 16) );
			STATIC_ASSERT( (offsetof( Element, b2 ) == 48) and (sizeof( b2 ) == 8) );
			STATIC_ASSERT( (offsetof( Element, _padding2 ) == 56) and (sizeof( _padding2 ) == 8) );
		}
	};

	DynamicBuffer_SSBO ()
	{
		// compile-time tests for correct fields offsets
		STATIC_ASSERT( sizeof(DynamicBuffer_SSBO) == 32 );
		STATIC_ASSERT( (offsetof( DynamicBuffer_SSBO, f2 ) == 0) and (sizeof( f2 ) == 8) );
		STATIC_ASSERT( (offsetof( DynamicBuffer_SSBO, _padding0 ) == 8) and (sizeof( _padding0 ) == 8) );
		STATIC_ASSERT( (offsetof( DynamicBuffer_SSBO, i4 ) == 16) and (sizeof( i4 ) == 16) );
	}
};

void Create (PipelineTemplateDescription& descr)
{
	descr = PipelineTemplateDescription();

	// set render state
	descr.renderState = RenderState();

	// set required dynamic states
	descr.dynamicStates = EPipelineDynamicState::Viewport | EPipelineDynamicState::Scissor;

	// set supported shader stages
	descr.supportedShaders = EShader::Vertex | EShader::Fragment;

	// set allowed primitives
	descr.supportedPrimitives = EPrimitive::TriangleStrip | EPrimitive::TriangleList;

	descr.layout = PipelineLayoutDescription::Builder()
			.AddTexture( "un_ColorTexture", EImage::Tex2D, EPixelFormatClass::Any,
						 0u,	// binding index for OpenGL
						 0u,	// unique index for Vulkan, OpenCL, C++
						 EShader::Fragment )
			.AddStorageBuffer( "ssb",
							   288_b,	// size of static part of buffer
							   0_b,		// size of one element in dynamic part of buffer
							   EShaderMemoryModel::Default,	// may be used for pipeline barriers
							   0u, 0u, EShader::Fragment )
			.Finish();

	// set vertex shader source/binary
	descr.Vertex().StringGLSL( ... );
	descr.Vertex().StringHLSL( ... );
	descr.Vertex().ArraySPIRV( ... );
	descr.Vertex().FuncSW( ... );
	
	// set fragment shader output
	descr.fragOutput = FragmentOutputState()
			.Add( "out_Color", EFragOutput::Float4, 0 );
			
	// set fragment shader source/binary
	descr.Fragment().StringGLSL( ... );
	descr.Fragment().ArraySPIRV( ... );
	descr.Fragment().FuncSW( ... );
}
```


## GXSL specification
This is fork of GLSL specification.

- Dynamic arrays in function arguments is allowed.

- Global variables are forbidden.
- Access to external objects (image, texture, buffer, ...) outside of entry point are forbidden.
- Access to builtin shader variables (gl_GlobalInvocationID, ...) outside of entry point is allowed (for HLSL target too).

- Compare operators `< > == != >= <=` are allowed for vector types too.
- Compare operators between vector and scalar types are allowed.
