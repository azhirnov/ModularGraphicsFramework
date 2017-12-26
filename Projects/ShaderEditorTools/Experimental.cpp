
#ifdef __cplusplus
#include "Projects/ShaderEditorTools/ShadertoyTemplate.h"

DECL_PIPELINE( Experimental, ShadertoyTemplate,
{
	shaders.fragment.Load( __FILE__ );
})
#endif


#ifdef SHADER
#if SHADER & SH_FRAGMENT

void mainImage (out vec4 fragColor, in vec2 fragCoord)
{
	vec2 uv = fragCoord.xy / iResolution.xy;
	fragColor = vec4(uv,0.5+0.5*sin(iTime),1.0);
}

#endif
#endif	// SHADER
