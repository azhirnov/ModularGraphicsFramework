// This is generated file, don't change anything!

#include "AllNodeFuncs.Shared.h"

namespace ShaderEditor
{
namespace ShaderNodes
{


	// vertex shader input/output

	Int  VS_GetInstanceID () {
		return Helpers::BuildNode< Int >( "VS_GetInstanceID" );
	}

	Int  VS_GetVertexID () {
		return Helpers::BuildNode< Int >( "VS_GetVertexID" );
	}

	void   VS_SetPosition (const Shader &vs, const Float4 &value) {
		Helpers::NodeGetter::Get( vs )->Root()->AddField( Helpers::BuildNode< ShaderOutputNode >( "VS_SetPosition", value ) );
	}
		
	// tess control shader input/output

	Int  TCS_GetInvocationID () {
		return Helpers::BuildNode< Int >( "TCS_GetInvocationID" );
	}

	Int  TCS_GetPatchVerticesIn () {
		return Helpers::BuildNode< Int >( "TCS_GetPatchVerticesIn" );
	}

	Int  TCS_GetPrimitiveID () {
		return Helpers::BuildNode< Int >( "TCS_GetPrimitiveID" );
	}

	void   TCS_SetPosition (const Shader &tcs, const Float4 &value) {
		Helpers::NodeGetter::Get( tcs )->Root()->AddField( Helpers::BuildNode< ShaderOutputNode >( "TCS_SetPosition", value ) );
	}

	void   TCS_SetTessLevelInner (const Shader &tcs, const Float2 &value) {
		Helpers::NodeGetter::Get( tcs )->Root()->AddField( Helpers::BuildNode< ShaderOutputNode >( "TCS_SetTessLevelInner", value ) );
	}

	void   TCS_SetTessLevelOuter (const Shader &tcs, const Float4 &value) {
		Helpers::NodeGetter::Get( tcs )->Root()->AddField( Helpers::BuildNode< ShaderOutputNode >( "TCS_SetTessLevelOuter", value ) );
	}
		
	// tess evaluation shader input/output

	Int    TES_GetPatchVerticesIn () {
		return Helpers::BuildNode< Int >( "TES_GetPatchVerticesIn" );
	}

	Int    TES_GetPrimitiveID () {
		return Helpers::BuildNode< Int >( "TES_GetPrimitiveID" );
	}

	Float3 TES_GetTessCoord () {
		return Helpers::BuildNode< Float3 >( "TES_GetTessCoord" );
	}

	Float2 TES_GetTessLevelInner () {
		return Helpers::BuildNode< Float2 >( "TES_GetTessLevelInner" );
	}

	Float4 TES_GetTessLevelOuter () {
		return Helpers::BuildNode< Float4 >( "TES_GetTessLevelOuter" );
	}

	void   TES_SetPointSize (const Shader &tes, const Float &value) {
		Helpers::NodeGetter::Get( tes )->Root()->AddField( Helpers::BuildNode< ShaderOutputNode >( "TES_SetPointSize", value ) );
	}

	void   TES_SetPosition (const Shader &tes, const Float4 &value) {
		Helpers::NodeGetter::Get( tes )->Root()->AddField( Helpers::BuildNode< ShaderOutputNode >( "TES_SetPosition", value ) );
	}
		
	// geometry shader input/output

	Int    GS_GetPrimitiveID () {
		return Helpers::BuildNode< Int >( "GS_GetPrimitiveID" );
	}

	void   GS_SetLayer (const Shader &gs, const Int &value) {
		Helpers::NodeGetter::Get( gs )->Root()->AddField( Helpers::BuildNode< ShaderOutputNode >( "GS_SetLayer", value ) );
	}

	void   GS_SetPointSize (const Shader &gs, const Float &value) {
		Helpers::NodeGetter::Get( gs )->Root()->AddField( Helpers::BuildNode< ShaderOutputNode >( "GS_SetPointSize", value ) );
	}

	void   GS_SetPosition (const Shader &gs, const Float4 &value) {
		Helpers::NodeGetter::Get( gs )->Root()->AddField( Helpers::BuildNode< ShaderOutputNode >( "GS_SetPosition", value ) );
	}

	void   GS_SetPrimitiveID (const Shader &gs, const Int &value) {
		Helpers::NodeGetter::Get( gs )->Root()->AddField( Helpers::BuildNode< ShaderOutputNode >( "GS_SetPrimitiveID", value ) );
	}

	void   GS_SetViewportIndex (const Shader &gs, const Int &value) {
		Helpers::NodeGetter::Get( gs )->Root()->AddField( Helpers::BuildNode< ShaderOutputNode >( "GS_SetViewportIndex", value ) );
	}
		
	// fragment shader input/output

	Int2   FS_GetScreenResolution () {
		return Helpers::BuildNode< Int2 >( "FS_GetScreenResolution" );
	}

	Float4 FS_GetFragCoord () {
		return Helpers::BuildNode< Float4 >( "FS_GetFragCoord" );
	}

	Bool   FS_GetFrontFacing () {
		return Helpers::BuildNode< Bool >( "FS_GetFrontFacing" );
	}

	Bool   FS_GetHelperInvocation () {
		return Helpers::BuildNode< Bool >( "FS_GetHelperInvocation" );
	}

	Int    FS_GetLayerIndex () {
		return Helpers::BuildNode< Int >( "FS_GetLayerIndex" );
	}

	Int    FS_GetNumSamples () {
		return Helpers::BuildNode< Int >( "FS_GetNumSamples" );
	}

	Float2 FS_GetPointCoord () {
		return Helpers::BuildNode< Float2 >( "FS_GetPointCoord" );
	}

	Int    FS_GetPrimitiveID () {
		return Helpers::BuildNode< Int >( "FS_GetPrimitiveID" );
	}

	Int    FS_GetSampleID () {
		return Helpers::BuildNode< Int >( "FS_GetSampleID" );
	}

	Int    FS_GetViewportIndex () {
		return Helpers::BuildNode< Int >( "FS_GetViewportIndex" );
	}

	void   FS_SetFragmentDepth (const Shader &fs, const Float &value) {
		Helpers::NodeGetter::Get( fs )->Root()->AddField( Helpers::BuildNode< ShaderOutputNode >( "FS_SetFragmentDepth", value ) );
	}
		
	// compute shader input/output

	// global linear index
	Int   CS_GetGlobalIndexSize () {
		return Helpers::BuildNode< Int >( "CS_GetGlobalIndexSize" );
	}

	Int   CS_GetGlobalIndex () {
		return Helpers::BuildNode< Int >( "CS_GetGlobalIndex" );
	}

	// local linear index
	Int   CS_GetLocalIndexSize () {
		return Helpers::BuildNode< Int >( "CS_GetLocalIndexSize" );
	}

	Int   CS_GetLocalIndex () {
		return Helpers::BuildNode< Int >( "CS_GetLocalIndex" );
	}

	// group linear index
	Int   CS_GetGroupIndexSize () {
		return Helpers::BuildNode< Int >( "CS_GetGroupIndexSize" );
	}

	Int   CS_GetGroupIndex () {
		return Helpers::BuildNode< Int >( "CS_GetGroupIndex" );
	}

	// global coordinate in 3D
	Int3   CS_GetGlobalCoordSize () {
		return Helpers::BuildNode< Int3 >( "CS_GetGlobalCoordSize" );
	}

	Int3   CS_GetGlobalCoord () {
		return Helpers::BuildNode< Int3 >( "CS_GetGlobalCoord" );
	}

	// local coordinate in 3D
	Int3   CS_GetLocalCoordSize () {
		return Helpers::BuildNode< Int3 >( "CS_GetLocalCoordSize" );
	}

	Int3   CS_GetLocalCoord () {
		return Helpers::BuildNode< Int3 >( "CS_GetLocalCoord" );
	}

	// group coordinate in 3D
	Int3   CS_GetGroupCoordSize () {
		return Helpers::BuildNode< Int3 >( "CS_GetGroupCoordSize" );
	}

	Int3   CS_GetGroupCoord () {
		return Helpers::BuildNode< Int3 >( "CS_GetGroupCoord" );
	}
		

}	// ShaderNodes
}	// ShaderEditor
