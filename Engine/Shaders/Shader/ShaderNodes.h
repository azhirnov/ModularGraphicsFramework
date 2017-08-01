// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Shader/NodeFuncs.h"

namespace ShaderEditor
{
namespace ShaderNodes
{

	//
	// Shader
	//

	struct Shader : Node
	{
		friend struct PipelineNodes::Pipeline;

	// types
	protected:
		using EShader	= Engine::Platforms::EShader;

		struct ShaderCompiler;


	// variables
	private:
		ShaderCompiler *		_currCompiler;
		const EShader::type		_shaderType;


	// methods
	protected:
		explicit Shader (EShader::type shaderType);

		virtual void  Main () const = 0;

	public:
		bool AddOutputNode (const Node &node);

		virtual bool _Compile ();
	};



	//
	// Compute Shader
	//

	struct ComputeShader : Shader
	{
	// variables
	protected:
		// global linear index
		const Int		cs_globalIndexSize;
		const Int		cs_globalIndex;
		
		// local linear index
		const Int		cs_localIndexSize;
		const Int		cs_localIndex;
		
		// group linear index
		const Int		cs_groupIndexSize;
		const Int		cs_groupIndex;
		
		// global coordinate in 3D
		const Int3		cs_globalCoordSize;
		const Int3		cs_globalCoord;

		// local coordinate in 3D
		const Int3		cs_localCoordSize;
		const Int3		cs_localCoord;

		// group coordinate in 3D
		const Int3		cs_groupCoordSize;
		const Int3		cs_groupCoord;


	// methods
	public:
		ComputeShader () :
			Shader( EShader::Compute ),
			cs_globalIndexSize{ CS_GetGlobalIndexSize() },		cs_globalIndex{ CS_GetGlobalIndex() },
			cs_localIndexSize{ CS_GetLocalIndexSize() },		cs_localIndex{ CS_GetLocalIndex() },
			cs_groupIndexSize{ CS_GetGroupIndexSize() },		cs_groupIndex{ CS_GetGroupIndex() },
			cs_globalCoordSize{ CS_GetGlobalCoordSize() },		cs_globalCoord{ CS_GetGlobalCoord() },
			cs_localCoordSize{ CS_GetLocalCoordSize() },		cs_localCoord{ CS_GetLocalCoord() },
			cs_groupCoordSize{ CS_GetGroupCoordSize() },		cs_groupCoord{ CS_GetGroupCoord() }
		{}
	};



	//
	// Vertex Shader
	//

	struct VertexShader : Shader
	{
	// variables
	protected:
		const Int		vs_instanceID;
		const Int		vs_vertexID;

		Float4			vs_position	{ this, "vs_position" };


	// methods
	public:
		VertexShader () :
			Shader( EShader::Vertex ),
			vs_instanceID( VS_GetInstanceID() ),	vs_vertexID( VS_GetVertexID() )
		{}
	};



	//
	// Tessellation Control Shader
	//

	struct TessControlShader : Shader
	{
	// variables
	protected:
		const Int		tc_invocationID;
		const Int		tc_patchVerticesIn;
		const Int		tc_primitiveID;
		
		Float4			tc_position			{ this, "tc_position" };
		Float2			tc_tessLevelInner	{ this, "tc_tessLevelInner" };
		Float4			tc_tessLevelOuter	{ this, "tc_tessLevelOuter" };


	// methods
	public:
		TessControlShader () :
			Shader( EShader::TessControl ),
			tc_invocationID( TCS_GetInvocationID() ),	tc_patchVerticesIn( TCS_GetPatchVerticesIn() ),
			tc_primitiveID( TCS_GetPrimitiveID() )
		{}
	};



	//
	// Tessellation Evaluation Shader
	//

	struct TessEvaluationShader : Shader
	{
	// variables
	protected:
		const Int		te_patchVerticesIn;
		const Int		te_primitiveID;
		const Float3	te_tessCoord;
		const Float2	te_tessLevelInner;
		const Float4	te_tessLevelOuter;

		Float			te_pointSize	{ this, "te_pointSize" };
		Float4			te_position		{ this, "te_position" };


	// methods
	public:
		TessEvaluationShader () :
			Shader( EShader::TessEvaluation ),
			te_patchVerticesIn( TES_GetPatchVerticesIn() ),		te_primitiveID( TES_GetPrimitiveID() ),
			te_tessCoord( TES_GetTessCoord() ),					te_tessLevelInner( TES_GetTessLevelInner() ),
			te_tessLevelOuter( TES_GetTessLevelOuter() )
		{}
	};



	//
	// Geometry Shader
	//

	struct GeometryShader : Shader
	{
	// variables
	protected:
		const Int	gs_primitiveIDIn;

		Int			gs_layer			{ this, "gs_layer" };
		Float		gs_pointSize		{ this, "gs_pointSize" };
		Float4		gs_position			{ this, "gs_position" };
		Int			gs_primitiveID		{ this, "gs_primitiveID" };
		Int			gs_viewportIndex	{ this, "gs_viewportIndex" };


	// methods
	public:
		GeometryShader () :
			Shader( EShader::Geometry ),
			gs_primitiveIDIn( GS_GetPrimitiveIDIn() )
		{}
	};



	//
	// Fragment Shader
	//

	struct FragmentShader : Shader
	{
	// variables
	protected:
		const Int2		fs_screenResolution;
		const Float4	fs_fragCoord;
		const Bool		fs_frontFacing;
		const Bool		fs_helperInvocation;
		const Int		fs_layer;
		const Int		fs_numSamples;
		const Float2	fs_pointCoord;
		const Int		fs_primitiveID;
		const Int		fs_sampleID;
		//const Array<Int, 8>	sampleMask;
		//const Array<Int, 8>	sampleMaskIn;
		//const Float2			samplePosition;
		const Int		fs_viewportIndex;

		Float			fs_fragDepth	{ this, "fs_fragDepth" };

		
	// methods
	public:
		FragmentShader () :
			Shader( EShader::Fragment ),
			fs_screenResolution( FS_GetScreenResolution() ),	fs_fragCoord( FS_GetFragCoord() ),
			fs_frontFacing( FS_GetFrontFacing() ),				fs_helperInvocation( FS_GetHelperInvocation() ),
			fs_layer( FS_GetLayerIndex() ),						fs_numSamples( FS_GetNumSamples() ),
			fs_pointCoord( FS_GetPointCoord() ),				fs_primitiveID( FS_GetPrimitiveID() ),
			fs_sampleID( FS_GetSampleID() ),					fs_viewportIndex( FS_GetViewportIndex() )
		{}
	};


}	// ShaderNodes
}	// ShaderEditor
