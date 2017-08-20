// This is generated file, don't change anything!

#include "GLSLNodeFuncs.Shared.h"

namespace ShaderEditor
{
namespace GLSLShaderNodes
{

	// vertex shader input/output

	static bool VS_GetInstanceID_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "VS_GetInstanceID", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "instanceID", func );
		return true;
	}

	static bool VS_GetInstanceID_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "VS_GetInstanceID" ) );
		src << "int VS_GetInstanceID () {\n\treturn gl_InstanceID;\n}";
		return true;
	}

	static bool VS_GetVertexID_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "VS_GetVertexID", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "vertexID", func );
		return true;
	}

	static bool VS_GetVertexID_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "VS_GetVertexID" ) );
		src << "int VS_GetVertexID () {\n\treturn gl_VertexID;\n}";
		return true;
	}

	static bool VS_SetPosition_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == TypeIdOf< Float4 >() );
		CHECK_ERR( out.Front().GetValueTypeId() == TypeIdOf< ShaderOutputNode >() );

		FunctionNode	func( "VS_SetPosition", out.Count(), in.Front().Get<Float4>() );
		Replace( out.Front().Get<ShaderOutputNode>(), "Sync.out", func );
		return true;
	}

	static bool VS_SetPosition_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "VS_SetPosition" ) );
		src << "void VS_SetPosition (const float4 value) {\n\tgl_Position = value;\n}";
		return true;
	}
	
	// tess control shader input/output

	static bool TCS_GetInvocationID_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "TCS_GetInvocationID", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "invocationID", func );
		return true;
	}

	static bool TCS_GetInvocationID_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "TCS_GetInvocationID" ) );
		src << "int TCS_GetInvocationID () {\n\treturn gl_InvocationID;\n}";
		return true;
	}

	static bool TCS_GetPatchVerticesIn_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "TCS_GetPatchVerticesIn", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "patchVertices", func );
		return true;
	}

	static bool TCS_GetPatchVerticesIn_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "TCS_GetPatchVerticesIn" ) );
		src << "int TCS_GetPatchVerticesIn () {\n\treturn gl_PatchVerticesIn;\n}";
		return true;
	}

	static bool TCS_GetPrimitiveID_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "TCS_GetPrimitiveID", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "primitiveID", func );
		return true;
	}

	static bool TCS_GetPrimitiveID_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "TCS_GetPrimitiveID" ) );
		src << "int TCS_GetPrimitiveID () {\n\treturn gl_PrimitiveID;\n}";
		return true;
	}

	static bool TCS_SetPosition_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == TypeIdOf< Float4 >() );
		CHECK_ERR( out.Front().GetValueTypeId() == TypeIdOf< ShaderOutputNode >() );

		FunctionNode	func( "TCS_SetPosition", out.Count(), in.Front().Get<Float4>() );
		Replace( out.Front().Get<ShaderOutputNode>(), "Sync.out", func );
		return true;
	}

	static bool TCS_SetPosition_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "TCS_SetPosition" ) );
		src << "void TCS_SetPosition (const float4 value) {\n\tgl_Position = value;\n}";
		return true;
	}

	static bool TCS_SetTessLevelInner_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == TypeIdOf< Float2 >() );
		CHECK_ERR( out.Front().GetValueTypeId() == TypeIdOf< ShaderOutputNode >() );

		FunctionNode	func( "TCS_SetTessLevelInner", out.Count(), in.Front().Get<Float2>() );
		Replace( out.Front().Get<ShaderOutputNode>(), "Sync.out", func );
		return true;
	}

	static bool TCS_SetTessLevelInner_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "TCS_SetTessLevelInner" ) );
		src << "void TCS_SetTessLevelInner (const float2 value) {\n\tgl_TessLevelInner = value;\n}";
		return true;
	}

	static bool TCS_SetTessLevelOuter_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == TypeIdOf< Float4 >() );
		CHECK_ERR( out.Front().GetValueTypeId() == TypeIdOf< ShaderOutputNode >() );

		FunctionNode	func( "TCS_SetTessLevelOuter", out.Count(), in.Front().Get<Float4>() );
		Replace( out.Front().Get<ShaderOutputNode>(), "Sync.out", func );
		return true;
	}

	static bool TCS_SetTessLevelOuter_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "TCS_SetTessLevelOuter" ) );
		src << "void TCS_SetTessLevelOuter (const int value) {\n\tgl_TessLevelOuter = value;\n}";
		return true;
	}
	
	// tess evaluation shader input/output

	static bool TES_GetPatchVerticesIn_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "TES_GetPatchVerticesIn", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "patchVertices", func );
		return true;
	}

	static bool TES_GetPatchVerticesIn_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "TES_GetPatchVerticesIn" ) );
		src << "int TES_GetPatchVerticesIn () {\n\treturn gl_PatchVerticesIn;\n}";
		return true;
	}

	static bool TES_GetPrimitiveID_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "TES_GetPrimitiveID", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "primitiveID", func );
		return true;
	}

	static bool TES_GetPrimitiveID_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "TES_GetPrimitiveID" ) );
		src << "int TES_GetPrimitiveID () {\n\treturn gl_PrimitiveID;\n}";
		return true;
	}

	static bool TES_GetTessCoord_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Float3>() );

		FunctionNode	func( "TES_GetTessCoord", in.Count(), out.Count() );
		Replace( out.Front().Get<Float3>(), "tessCoord", func );
		return true;
	}

	static bool TES_GetTessCoord_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "TES_GetTessCoord" ) );
		src << "float3 TES_GetTessCoord () {\n\treturn gl_TessCoord;\n}";
		return true;
	}

	static bool TES_GetTessLevelInner_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Float2>() );

		FunctionNode	func( "TES_GetTessLevelInner", in.Count(), out.Count() );
		Replace( out.Front().Get<Float2>(), "tessLevelInner", func );
		return true;
	}

	static bool TES_GetTessLevelInner_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "TES_GetTessLevelInner" ) );
		src << "float2 TES_GetTessLevelInner () {\n\treturn float2(gl_TessLevelInner[0], gl_TessLevelInner[1]);\n}";
		return true;
	}

	static bool TES_GetTessLevelOuter_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Float4>() );

		FunctionNode	func( "TES_GetTessLevelOuter", in.Count(), out.Count() );
		Replace( out.Front().Get<Float4>(), "tessLevelOuter", func );
		return true;
	}

	static bool TES_GetTessLevelOuter_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "TES_GetTessLevelOuter" ) );
		src << "float4 TES_GetTessLevelOuter () {\n\treturn float4(gl_TessLevelOuter[0], gl_TessLevelOuter[1], gl_TessLevelOuter[2], gl_TessLevelOuter[3]);\n}";
		return true;
	}

	static bool TES_SetPointSize_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == TypeIdOf< Int >() );
		CHECK_ERR( out.Front().GetValueTypeId() == TypeIdOf< ShaderOutputNode >() );

		FunctionNode	func( "TES_SetPointSize", out.Count(), in.Front().Get<Int>() );
		Replace( out.Front().Get<ShaderOutputNode>(), "Sync.out", func );
		return true;
	}

	static bool TES_SetPointSize_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "TES_SetPointSize" ) );
		src << "void TES_SetPointSize (const int value) {\n\tgl_PointSize = value;\n}";
		return true;
	}

	static bool TES_SetPosition_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == TypeIdOf< Float4 >() );
		CHECK_ERR( out.Front().GetValueTypeId() == TypeIdOf< ShaderOutputNode >() );

		FunctionNode	func( "TES_SetPosition", out.Count(), in.Front().Get<Float4>() );
		Replace( out.Front().Get<ShaderOutputNode>(), "Sync.out", func );
		return true;
	}

	static bool TES_SetPosition_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "TES_SetPosition" ) );
		src << "void TES_SetPosition (const float4 value) {\n\tgl_Position = value;\n}";
		return true;
	}
	
	// geometry shader input/output

	static bool GS_GetPrimitiveID_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "GS_GetPrimitiveID", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "primitiveID", func );
		return true;
	}

	static bool GS_GetPrimitiveID_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "GS_GetPrimitiveID" ) );
		src << "int GS_GetPrimitiveID () {\n\treturn gl_PrimitiveIDIn;\n}";
		return true;
	}

	static bool GS_SetLayer_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == TypeIdOf< Int >() );
		CHECK_ERR( out.Front().GetValueTypeId() == TypeIdOf< ShaderOutputNode >() );

		FunctionNode	func( "GS_SetLayer", out.Count(), in.Front().Get<Int>() );
		Replace( out.Front().Get<ShaderOutputNode>(), "Sync.out", func );
		return true;
	}

	static bool GS_SetLayer_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "GS_SetLayer" ) );
		src << "void GS_SetLayer (const int value) {\n\tgl_Layer = value;\n}";
		return true;
	}

	static bool GS_SetPointSize_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == TypeIdOf< Float >() );
		CHECK_ERR( out.Front().GetValueTypeId() == TypeIdOf< ShaderOutputNode >() );

		FunctionNode	func( "GS_SetPointSize", out.Count(), in.Front().Get<Float>() );
		Replace( out.Front().Get<ShaderOutputNode>(), "Sync.out", func );
		return true;
	}

	static bool GS_SetPointSize_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "GS_SetPointSize" ) );
		src << "void GS_SetPointSize (const float value) {\n\tgl_PointSize = value;\n}";
		return true;
	}

	static bool GS_SetPosition_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == TypeIdOf< Float4 >() );
		CHECK_ERR( out.Front().GetValueTypeId() == TypeIdOf< ShaderOutputNode >() );

		FunctionNode	func( "GS_SetPosition", out.Count(), in.Front().Get<Float4>() );
		Replace( out.Front().Get<ShaderOutputNode>(), "Sync.out", func );
		return true;
	}

	static bool GS_SetPosition_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "GS_SetPosition" ) );
		src << "void GS_SetPosition (const float4 value) {\n\tgl_Position = value;\n}";
		return true;
	}

	static bool GS_SetPrimitiveID_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == TypeIdOf< Int >() );
		CHECK_ERR( out.Front().GetValueTypeId() == TypeIdOf< ShaderOutputNode >() );

		FunctionNode	func( "GS_SetPrimitiveID", out.Count(), in.Front().Get<Int>() );
		Replace( out.Front().Get<ShaderOutputNode>(), "Sync.out", func );
		return true;
	}

	static bool GS_SetPrimitiveID_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "GS_SetPrimitiveID" ) );
		src << "void GS_SetPrimitiveID (const int value) {\n\tgl_PrimitiveID = value;\n}";
		return true;
	}

	static bool GS_SetViewportIndex_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == TypeIdOf< Int >() );
		CHECK_ERR( out.Front().GetValueTypeId() == TypeIdOf< ShaderOutputNode >() );

		FunctionNode	func( "GS_SetViewportIndex", out.Count(), in.Front().Get<Int>() );
		Replace( out.Front().Get<ShaderOutputNode>(), "Sync.out", func );
		return true;
	}

	static bool GS_SetViewportIndex_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "GS_SetViewportIndex" ) );
		src << "void GS_SetViewportIndex (const int value) {\n\tgl_ViewportIndex = value;\n}";
		return true;
	}
	
	// fragment shader input/output

	static bool FS_GetScreenResolution_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int2>() );

		FunctionNode	func( "FS_GetScreenResolution", in.Count(), out.Count() );
		Replace( out.Front().Get<Int2>(), "screenRes", func );
		return true;
	}

	static bool FS_GetScreenResolution_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "FS_GetScreenResolution" ) );
		src << "int2 FS_GetScreenResolution () {\n\treturn unScreenResolution;\n}";
		return true;
	}

	static bool FS_GetFragCoord_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Float4>() );

		FunctionNode	func( "FS_GetFragCoord", in.Count(), out.Count() );
		Replace( out.Front().Get<Float4>(), "fragCoord", func );
		return true;
	}

	static bool FS_GetFragCoord_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "FS_GetFragCoord" ) );
		src << "float4 FS_GetFragCoord () {\n\treturn gl_FragCoord;\n}";
		return true;
	}

	static bool FS_GetFrontFacing_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Bool>() );

		FunctionNode	func( "FS_GetFrontFacing", in.Count(), out.Count() );
		Replace( out.Front().Get<Bool>(), "frontFacing", func );
		return true;
	}

	static bool FS_GetFrontFacing_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "FS_GetFrontFacing" ) );
		src << "bool FS_GetFrontFacing () {\n\treturn gl_FrontFacing;\n}";
		return true;
	}

	static bool FS_GetHelperInvocation_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Bool>() );

		FunctionNode	func( "FS_GetHelperInvocation", in.Count(), out.Count() );
		Replace( out.Front().Get<Bool>(), "helperInvocation", func );
		return true;
	}

	static bool FS_GetHelperInvocation_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "FS_GetHelperInvocation" ) );
		src << "bool FS_GetHelperInvocation () {\n\treturn gl_HelperInvocation;\n}";
		return true;
	}

	static bool FS_GetLayerIndex_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "FS_GetLayerIndex", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "layer", func );
		return true;
	}

	static bool FS_GetLayerIndex_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "FS_GetLayerIndex" ) );
		src << "int FS_GetLayerIndex () {\n\treturn gl_Layer;\n}";
		return true;
	}

	static bool FS_GetNumSamples_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "FS_GetNumSamples", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "numSamples", func );
		return true;
	}

	static bool FS_GetNumSamples_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "FS_GetNumSamples" ) );
		src << "int FS_GetNumSamples () {\n\treturn gl_NumSamples;\n}";
		return true;
	}

	static bool FS_GetPointCoord_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Float2>() );

		FunctionNode	func( "FS_GetPointCoord", in.Count(), out.Count() );
		Replace( out.Front().Get<Float2>(), "pointCoord", func );
		return true;
	}

	static bool FS_GetPointCoord_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "FS_GetPointCoord" ) );
		src << "float2 FS_GetPointCoord () {\n\treturn gl_PointCoord;\n}";
		return true;
	}

	static bool FS_GetPrimitiveID_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "FS_GetPrimitiveID", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "primitiveID", func );
		return true;
	}

	static bool FS_GetPrimitiveID_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "FS_GetPrimitiveID" ) );
		src << "int FS_GetPrimitiveID () {\n\treturn gl_PrimitiveID;\n}";
		return true;
	}

	static bool FS_GetSampleID_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "FS_GetSampleID", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "sampleID", func );
		return true;
	}

	static bool FS_GetSampleID_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "FS_GetSampleID" ) );
		src << "int FS_GetSampleID () {\n\treturn gl_SampleID;\n}";
		return true;
	}

	static bool FS_GetViewportIndex_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "FS_GetViewportIndex", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "viewportIndex", func );
		return true;
	}

	static bool FS_GetViewportIndex_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "FS_GetViewportIndex" ) );
		src << "int FS_GetViewportIndex () {\n\treturn gl_ViewportIndex;\n}";
		return true;
	}

	static bool FS_SetFragmentDepth_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 1 and out.Count() == 1 );
		CHECK_ERR( in.Front().GetValueTypeId() == TypeIdOf< Int >() );
		CHECK_ERR( out.Front().GetValueTypeId() == TypeIdOf< ShaderOutputNode >() );

		FunctionNode	func( "FS_SetFragmentDepth", out.Count(), in.Front().Get<Int>() );
		Replace( out.Front().Get<ShaderOutputNode>(), "Sync.out", func );
		return true;
	}

	static bool FS_SetFragmentDepth_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "FS_SetFragmentDepth" ) );
		src << "void FS_SetFragmentDepth (float value) {\n\tgl_FragDepth = value;\n}";
		return true;
	}
	
	// compute shader input/output

	static bool CS_GetGlobalIndexSize_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "CS_GetGlobalIndexSize", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "globalIndexSize", func );
		return true;
	}

	static bool CS_GetGlobalIndexSize_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "CS_GetGlobalIndexSize_Source" ) );
		src << "int CS_GetGlobalIndexSize () {\n\tint3 size  = $CS_GetGlobalCoordSize_outInt3();\n\treturn size.x * size.y * size.z;\n}";
		return true;
	}

	static bool CS_GetGlobalIndex_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "CS_GetGlobalIndex", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "globalIndex", func );
		return true;
	}

	static bool CS_GetGlobalIndex_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "CS_GetGlobalIndex" ) );
		src << "int CS_GetGlobalIndex () {\n\tint3 coord = $CS_GetGlobalCoord_outInt3();\n\tint3 size  = $CS_GetGlobalCoordSize_outInt3();\n\treturn coord.x + (coord.y * size.x) + (coord.z * size.x * size.y);\n}";
		return true;
	}

	static bool CS_GetLocalIndexSize_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "CS_GetLocalIndexSize", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "localIndexSize", func );
		return true;
	}

	static bool CS_GetLocalIndexSize_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "CS_GetLocalIndexSize" ) );
		src << "int CS_GetLocalIndexSize () {\n\tint3 size  = $CS_GetLocalCoordSize_outInt3();\n\treturn size.x * size.y * size.z;\n}";
		return true;
	}

	static bool CS_GetLocalIndex_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "CS_GetLocalIndex", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "localIndex", func );
		return true;
	}

	static bool CS_GetLocalIndex_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "CS_GetLocalIndex" ) );
		src << "int CS_GetLocalIndex () {\n\treturn int( gl_LocalInvocationIndex );\n}";
		return true;
	}

	static bool CS_GetGroupIndexSize_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "CS_GetGroupIndexSize", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "groupIndexSize", func );
		return true;
	}

	static bool CS_GetGroupIndexSize_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "CS_GetGroupIndexSize" ) );
		src << "int CS_GetGroupIndexSize () {\n\tint3 size  = $CS_GetGroupCoordSize_outInt3();\n\treturn size.x * size.y * size.z;\n}";
		return true;
	}

	static bool CS_GetGroupIndex_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int>() );

		FunctionNode	func( "CS_GetGroupIndex", in.Count(), out.Count() );
		Replace( out.Front().Get<Int>(), "groupIndex", func );
		return true;
	}

	static bool CS_GetGroupIndex_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "CS_GetGroupIndex" ) );
		src << "int CS_GetGroupIndex () {\n\tint3 coord = $CS_GetGroupCoord_outInt3();\n\tint3 size  = $CS_GetGroupCoordSize_outInt3();\n\treturn coord.x + (coord.y * size.x) + (coord.z * size.x * size.y);\n}";
		return true;
	}

	static bool CS_GetGlobalCoordSize_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int3>() );

		FunctionNode	func( "CS_GetGlobalCoordSize", in.Count(), out.Count() );
		Replace( out.Front().Get<Int3>(), "globalCoordSize", func );
		return true;
	}

	static bool CS_GetGlobalCoordSize_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "CS_GetGlobalCoordSize" ) );
		src << "int3 CS_GetGlobalCoordSize () {\n\treturn $CS_GetGroupCoordSize_outInt3() * $CS_GetLocalCoordSize_outInt3();\n}";
		return true;
	}

	static bool CS_GetGlobalCoord_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int3>() );

		FunctionNode	func( "CS_GetGlobalCoord", in.Count(), out.Count() );
		Replace( out.Front().Get<Int3>(), "globalCoord", func );
		return true;
	}

	static bool CS_GetGlobalCoord_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "CS_GetGlobalCoord" ) );
		src << "int3 CS_GetGlobalCoord () {\n\treturn int3( gl_GlobalInvocationID );\n}";
		return true;
	}

	static bool CS_GetLocalCoordSize_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int3>() );

		FunctionNode	func( "CS_GetLocalCoordSize", in.Count(), out.Count() );
		Replace( out.Front().Get<Int3>(), "localCoordSize", func );
		return true;
	}

	static bool CS_GetLocalCoordSize_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "CS_GetLocalCoordSize" ) );
		src << "int3 CS_GetLocalCoordSize () {\n#ifdef GL_ARB_compute_variable_group_size\n\treturn int3( gl_LocalGroupSizeARB );\n#else\n\treturn int3( gl_WorkGroupSize );\n#endif\n}";
		return true;
	}

	static bool CS_GetLocalCoord_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int3>() );

		FunctionNode	func( "CS_GetLocalCoord", in.Count(), out.Count() );
		Replace( out.Front().Get<Int3>(), "localCoord", func );
		return true;
	}

	static bool CS_GetLocalCoord_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "CS_GetLocalCoord" ) );
		src << "int3 CS_GetLocalCoord () {\n\treturn int3( gl_LocalInvocationID );\n}";
		return true;
	}

	static bool CS_GetGroupCoordSize_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int3>() );

		FunctionNode	func( "CS_GetGroupCoordSize", in.Count(), out.Count() );
		Replace( out.Front().Get<Int3>(), "groupCoordSize", func );
		return true;
	}

	static bool CS_GetGroupCoordSize_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "CS_GetGroupCoordSize" ) );
		src << "int3 CS_GetGroupCoordSize () {\n\treturn int3( gl_NumWorkGroups );\n}";
		return true;
	}

	static bool CS_GetGroupCoord_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 0 and out.Count() == 1 and out.Front().GetValueTypeId() == TypeIdOf<Int3>() );

		FunctionNode	func( "CS_GetGroupCoord", in.Count(), out.Count() );
		Replace( out.Front().Get<Int3>(), "groupCoord", func );
		return true;
	}

	static bool CS_GetGroupCoord_Source (StringCRef sig, OUT String &src)
	{
		ASSERT( sig.StartsWith( "CS_GetGroupCoord" ) );
		src << "int3 CS_GetGroupCoord () {\n\treturn int3( gl_WorkGroupID );\n}";
		return true;
	}
	

}	// GLSLShaderNodes

namespace _ShaderNodesHidden_
{
	void RegisterGLSLShaderNodes6 (NodeFunctions *nf)
	{

		nf->Register( "GL4.VS_GetInstanceID", &GLSLShaderNodes::VS_GetInstanceID_Node, &GLSLShaderNodes::VS_GetInstanceID_Source );
		nf->Register( "GL4.VS_GetVertexID", &GLSLShaderNodes::VS_GetVertexID_Node, &GLSLShaderNodes::VS_GetVertexID_Source );
		nf->Register( "GL4.VS_SetPosition", &GLSLShaderNodes::VS_SetPosition_Node, &GLSLShaderNodes::VS_SetPosition_Source );
	
		nf->Register( "GL4.TCS_GetInvocationID", &GLSLShaderNodes::TCS_GetInvocationID_Node, &GLSLShaderNodes::TCS_GetInvocationID_Source );
		nf->Register( "GL4.TCS_GetPatchVerticesIn", &GLSLShaderNodes::TCS_GetPatchVerticesIn_Node, &GLSLShaderNodes::TCS_GetPatchVerticesIn_Source );
		nf->Register( "GL4.TCS_GetPrimitiveID", &GLSLShaderNodes::TCS_GetPrimitiveID_Node, &GLSLShaderNodes::TCS_GetPrimitiveID_Source );
		nf->Register( "GL4.TCS_SetPosition", &GLSLShaderNodes::TCS_SetPosition_Node, &GLSLShaderNodes::TCS_SetPosition_Source );
		nf->Register( "GL4.TCS_SetTessLevelInner", &GLSLShaderNodes::TCS_SetTessLevelInner_Node, &GLSLShaderNodes::TCS_SetTessLevelInner_Source );
		nf->Register( "GL4.TCS_SetTessLevelOuter", &GLSLShaderNodes::TCS_SetTessLevelOuter_Node, &GLSLShaderNodes::TCS_SetTessLevelOuter_Source );
	
		nf->Register( "GL4.TES_GetPatchVerticesIn", &GLSLShaderNodes::TES_GetPatchVerticesIn_Node, &GLSLShaderNodes::TES_GetPatchVerticesIn_Source );
		nf->Register( "GL4.TES_GetPrimitiveID", &GLSLShaderNodes::TES_GetPrimitiveID_Node, &GLSLShaderNodes::TES_GetPrimitiveID_Source );
		nf->Register( "GL4.TES_GetTessCoord", &GLSLShaderNodes::TES_GetTessCoord_Node, &GLSLShaderNodes::TES_GetTessCoord_Source );
		nf->Register( "GL4.TES_GetTessLevelInner", &GLSLShaderNodes::TES_GetTessLevelInner_Node, &GLSLShaderNodes::TES_GetTessLevelInner_Source );
		nf->Register( "GL4.TES_GetTessLevelOuter", &GLSLShaderNodes::TES_GetTessLevelOuter_Node, &GLSLShaderNodes::TES_GetTessLevelOuter_Source );
		nf->Register( "GL4.TES_SetPointSize", &GLSLShaderNodes::TES_SetPointSize_Node, &GLSLShaderNodes::TES_SetPointSize_Source );
		nf->Register( "GL4.TES_SetPosition", &GLSLShaderNodes::TES_SetPosition_Node, &GLSLShaderNodes::TES_SetPosition_Source );
	
		nf->Register( "GL4.GS_GetPrimitiveID", &GLSLShaderNodes::GS_GetPrimitiveID_Node, &GLSLShaderNodes::GS_GetPrimitiveID_Source );
		nf->Register( "GL4.GS_SetLayer", &GLSLShaderNodes::GS_SetLayer_Node, &GLSLShaderNodes::GS_SetLayer_Source );
		nf->Register( "GL4.GS_SetPointSize", &GLSLShaderNodes::GS_SetPointSize_Node, &GLSLShaderNodes::GS_SetPointSize_Source );
		nf->Register( "GL4.GS_SetPosition", &GLSLShaderNodes::GS_SetPosition_Node, &GLSLShaderNodes::GS_SetPosition_Source );
		nf->Register( "GL4.GS_SetPrimitiveID", &GLSLShaderNodes::GS_SetPrimitiveID_Node, &GLSLShaderNodes::GS_SetPrimitiveID_Source );
		nf->Register( "GL4.GS_SetViewportIndex", &GLSLShaderNodes::GS_SetViewportIndex_Node, &GLSLShaderNodes::GS_SetViewportIndex_Source );
	
		nf->Register( "GL4.FS_GetScreenResolution", &GLSLShaderNodes::FS_GetScreenResolution_Node, &GLSLShaderNodes::FS_GetScreenResolution_Source );
		nf->Register( "GL4.FS_GetFragCoord", &GLSLShaderNodes::FS_GetFragCoord_Node, &GLSLShaderNodes::FS_GetFragCoord_Source );
		nf->Register( "GL4.FS_GetFrontFacing", &GLSLShaderNodes::FS_GetFrontFacing_Node, &GLSLShaderNodes::FS_GetFrontFacing_Source );
		nf->Register( "GL4.FS_GetHelperInvocation", &GLSLShaderNodes::FS_GetHelperInvocation_Node, &GLSLShaderNodes::FS_GetHelperInvocation_Source );
		nf->Register( "GL4.FS_GetLayerIndex", &GLSLShaderNodes::FS_GetLayerIndex_Node, &GLSLShaderNodes::FS_GetLayerIndex_Source );
		nf->Register( "GL4.FS_GetNumSamples", &GLSLShaderNodes::FS_GetNumSamples_Node, &GLSLShaderNodes::FS_GetNumSamples_Source );
		nf->Register( "GL4.FS_GetPointCoord", &GLSLShaderNodes::FS_GetPointCoord_Node, &GLSLShaderNodes::FS_GetPointCoord_Source );
		nf->Register( "GL4.FS_GetPrimitiveID", &GLSLShaderNodes::FS_GetPrimitiveID_Node, &GLSLShaderNodes::FS_GetPrimitiveID_Source );
		nf->Register( "GL4.FS_GetSampleID", &GLSLShaderNodes::FS_GetSampleID_Node, &GLSLShaderNodes::FS_GetSampleID_Source );
		nf->Register( "GL4.FS_GetViewportIndex", &GLSLShaderNodes::FS_GetViewportIndex_Node, &GLSLShaderNodes::FS_GetViewportIndex_Source );
		nf->Register( "GL4.FS_SetFragmentDepth", &GLSLShaderNodes::FS_SetFragmentDepth_Node, &GLSLShaderNodes::FS_SetFragmentDepth_Source );
	
		nf->Register( "GL4.CS_GetGlobalIndexSize", &GLSLShaderNodes::CS_GetGlobalIndexSize_Node, &GLSLShaderNodes::CS_GetGlobalIndexSize_Source );
		nf->Register( "GL4.CS_GetGlobalIndex", &GLSLShaderNodes::CS_GetGlobalIndex_Node, &GLSLShaderNodes::CS_GetGlobalIndex_Source );
		nf->Register( "GL4.CS_GetLocalIndexSize", &GLSLShaderNodes::CS_GetLocalIndexSize_Node, &GLSLShaderNodes::CS_GetLocalIndexSize_Source );
		nf->Register( "GL4.CS_GetLocalIndex", &GLSLShaderNodes::CS_GetLocalIndex_Node, &GLSLShaderNodes::CS_GetLocalIndex_Source );
		nf->Register( "GL4.CS_GetGroupIndexSize", &GLSLShaderNodes::CS_GetGroupIndexSize_Node, &GLSLShaderNodes::CS_GetGroupIndexSize_Source );
		nf->Register( "GL4.CS_GetGroupIndex", &GLSLShaderNodes::CS_GetGroupIndex_Node, &GLSLShaderNodes::CS_GetGroupIndex_Source );
		nf->Register( "GL4.CS_GetGlobalCoordSize", &GLSLShaderNodes::CS_GetGlobalCoordSize_Node, &GLSLShaderNodes::CS_GetGlobalCoordSize_Source );
		nf->Register( "GL4.CS_GetGlobalCoord", &GLSLShaderNodes::CS_GetGlobalCoord_Node, &GLSLShaderNodes::CS_GetGlobalCoord_Source );
		nf->Register( "GL4.CS_GetLocalCoordSize", &GLSLShaderNodes::CS_GetLocalCoordSize_Node, &GLSLShaderNodes::CS_GetLocalCoordSize_Source );
		nf->Register( "GL4.CS_GetLocalCoord", &GLSLShaderNodes::CS_GetLocalCoord_Node, &GLSLShaderNodes::CS_GetLocalCoord_Source );
		nf->Register( "GL4.CS_GetGroupCoordSize", &GLSLShaderNodes::CS_GetGroupCoordSize_Node, &GLSLShaderNodes::CS_GetGroupCoordSize_Source );
		nf->Register( "GL4.CS_GetGroupCoord", &GLSLShaderNodes::CS_GetGroupCoord_Node, &GLSLShaderNodes::CS_GetGroupCoord_Source );
		}


}	// _ShaderNodesHidden_
}	// ShaderEditor
