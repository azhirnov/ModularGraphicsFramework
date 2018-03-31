
#ifdef ANGELSCRIPT
void main ()
{
	GraphicsPipeline ppln = GraphicsPipeline();
	
	ppln.shaders.vertex.LoadSelf();
	ppln.shaders.fragment.LoadSelf();
	
	ppln.shaderFormat = EShaderSrcFormat_GLSL;

	ppln.renderState.inputAssembly.topology	= EPrimitive_TriangleStrip;

	ppln.dynamicStates |= EPipelineDynamicState_Viewport;
	ppln.dynamicStates |= EPipelineDynamicState_Scissor;
}
#endif	// ANGELSCRIPT
//-----------------------------------------------------------------------------


#ifdef SHADER
#if SHADER & SH_VERTEX

const float2	g_Positions[] = {
	{ -1.0,  1.0 },
	{ -1.0, -1.0 },
	{  1.0,  1.0 },
	{  1.0, -1.0 }
};

void main() {
	gl_Position	= float4( g_Positions[gl_VertexIndex], 0.0, 1.0 );
}
#endif
//-----------------------------------------------------------------------------


#if SHADER & SH_FRAGMENT

layout(std140) uniform ShadertoyUB
{
	float3	iResolution;			// viewport resolution (in pixels)
	float	iTime;					// shader playback time (in seconds)
	float	iTimeDelta;				// render time (in seconds)
	int		iFrame;					// shader playback frame
	float	iChannelTime[4];		// channel playback time (in seconds)
	float3	iChannelResolution[4];	// channel resolution (in pixels)
	float4	iMouse;					// mouse pixel coords. xy: current (if MLB down), zw: click
	float4	iDate;					// (year, month, day, time in seconds)
	float	iSampleRate;			// sound sample rate (i.e., 44100)
	float3	iCameraFrustum[4];		// frustum rays (left bottom, right bottom, left top, right top)
	float3	iCameraPos;				// camera position in world space
};

// input channel, TODO: 2D and Cube support
//uniform sampler2D	iChannel0;
//uniform sampler2D	iChannel1;
//uniform sampler2D	iChannel2;
//uniform sampler2D	iChannel3;

//layout(origin_upper_left) in float4 gl_FragCoord;

layout(location=0) out float4	out_Color0;

void mainImage (out float4 fragColor, in float2 fragCoord);

void main ()
{
	vec2 coord = gl_FragCoord.xy;
	//coord.y = iResolution.y - coord.y;

	mainImage( out_Color0, coord );
}
#endif
#endif	// SHADER
