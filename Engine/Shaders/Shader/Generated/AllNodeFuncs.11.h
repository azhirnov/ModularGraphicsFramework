// This is generated file, don't change anything!

#pragma once

#include "Engine/Shaders/Shader/Helpers.h"

namespace ShaderEditor
{
namespace ShaderNodes
{


	// vertex shader input/output

	Int    VS_GetInstanceID ();
	Int    VS_GetVertexID ();

	void   VS_SetPosition (const Shader &vs, const Float4 &value);
		
	// tess control shader input/output

	Int    TCS_GetInvocationID ();
	Int    TCS_GetPatchVerticesIn ();
	Int    TCS_GetPrimitiveID ();

	void   TCS_SetPosition (const Shader &tcs, const Float4 &value);
	void   TCS_SetTessLevelInner (const Shader &tcs, const Float2 &value);
	void   TCS_SetTessLevelOuter (const Shader &tcs, const Float4 &value);
		
	// tess evaluation shader input/output

	Int    TES_GetPatchVerticesIn ();
	Int    TES_GetPrimitiveID ();
	Float3 TES_GetTessCoord ();
	Float2 TES_GetTessLevelInner ();
	Float4 TES_GetTessLevelOuter ();

	void   TES_SetPointSize (const Shader &tes, const Float &value);
	void   TES_SetPosition (const Shader &tes, const Float4 &value);
		
	// geometry shader input/output

	Int    GS_GetPrimitiveID ();

	void   GS_SetLayer (const Shader &gs, const Int &value);
	void   GS_SetPointSize (const Shader &gs, const Float &value);
	void   GS_SetPosition (const Shader &gs, const Float4 &value);
	void   GS_SetPrimitiveID (const Shader &gs, const Int &value);
	void   GS_SetViewportIndex (const Shader &gs, const Int &value);
		
	// fragment shader input/output

	Int2   FS_GetScreenResolution ();
	Float4 FS_GetFragCoord ();
	Bool   FS_GetFrontFacing ();
	Bool   FS_GetHelperInvocation ();
	Int    FS_GetLayerIndex ();
	Int    FS_GetNumSamples ();
	Float2 FS_GetPointCoord ();
	Int    FS_GetPrimitiveID ();
	Int    FS_GetSampleID ();
	Int    FS_GetViewportIndex ();

	void   FS_SetFragmentDepth (const Shader &fs, const Float &value);
		
	// compute shader input/output

	// global linear index
	Int   CS_GetGlobalIndexSize ();
	Int   CS_GetGlobalIndex ();

	// local linear index
	Int   CS_GetLocalIndexSize ();
	Int   CS_GetLocalIndex ();

	// group linear index
	Int   CS_GetGroupIndexSize ();
	Int   CS_GetGroupIndex ();

	// global coordinate in 3D
	Int3   CS_GetGlobalCoordSize ();
	Int3   CS_GetGlobalCoord ();

	// local coordinate in 3D
	Int3   CS_GetLocalCoordSize ();
	Int3   CS_GetLocalCoord ();

	// group coordinate in 3D
	Int3   CS_GetGroupCoordSize ();
	Int3   CS_GetGroupCoord ();
		

}	// ShaderNodes
}	// ShaderEditor
