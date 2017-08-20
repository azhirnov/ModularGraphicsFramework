// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Shader/VecTypes.h"
#include "Engine/Shaders/Shader/ImageTypes.h"
#include "Engine/Shaders/Shader/BufferTypes.h"
#include "Engine/Shaders/Shader/OtherTypes.h"
#include "Engine/Shaders/Shader/Generated/AllNodeFuncs.h"

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
		explicit Shader (struct PipelineNodes::Pipeline *pipeline, EShader::type shaderType);

		virtual void  Main () const = 0;

		virtual bool _Compile ();

	public:
		bool AddOutputNode (const Node &node);
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
		explicit ComputeShader (PipelineNodes::Pipeline *pipeline) :
			Shader( pipeline, EShader::Compute ),
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


	// methods
	public:
		explicit VertexShader (PipelineNodes::Pipeline *pipeline) :
			Shader( pipeline, EShader::Vertex ),
			vs_instanceID( VS_GetInstanceID() ),	vs_vertexID( VS_GetVertexID() )
		{}

		void SetPosition (const Float4 &value) {
			return VS_SetPosition( *this, value );
		}
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


	// methods
	public:
		explicit TessControlShader (PipelineNodes::Pipeline *pipeline) :
			Shader( pipeline, EShader::TessControl ),
			tc_invocationID( TCS_GetInvocationID() ),	tc_patchVerticesIn( TCS_GetPatchVerticesIn() ),
			tc_primitiveID( TCS_GetPrimitiveID() )
		{}

		void SetPosition (const Float4 &value) {
			return TCS_SetPosition( *this, value );
		}

		void SetTessLevelInner (const Float2 &value) {
			return TCS_SetTessLevelInner( *this, value );
		}

		void SetTessLevelOuter (const Float4 &value) {
			return TCS_SetTessLevelOuter( *this, value );
		}
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


	// methods
	public:
		explicit TessEvaluationShader (PipelineNodes::Pipeline *pipeline) :
			Shader( pipeline, EShader::TessEvaluation ),
			te_patchVerticesIn( TES_GetPatchVerticesIn() ),		te_primitiveID( TES_GetPrimitiveID() ),
			te_tessCoord( TES_GetTessCoord() ),					te_tessLevelInner( TES_GetTessLevelInner() ),
			te_tessLevelOuter( TES_GetTessLevelOuter() )
		{}

		void SetPointSize (const Float &value) {
			return TES_SetPointSize( *this, value );
		}

		void SetPosition (const Float4 &value) {
			return TES_SetPosition( *this, value );
		}
	};



	//
	// Geometry Shader
	//

	struct GeometryShader : Shader
	{
	// variables
	protected:
		const Int	gs_primitiveIDIn;


	// methods
	public:
		explicit GeometryShader (PipelineNodes::Pipeline *pipeline) :
			Shader( pipeline, EShader::Geometry ),
			gs_primitiveIDIn( GS_GetPrimitiveID() )
		{}

		void SetLayer (const Int &value) {
			return GS_SetLayer( *this, value );
		}

		void SetPointSize (const Float &value) {
			return GS_SetPointSize( *this, value );
		}

		void SetPosition (const Float4 &value) {
			return GS_SetPosition( *this, value );
		}

		void SetPrimitiveID (const Int &value) {
			return GS_SetPrimitiveID( *this, value );
		}

		void SetViewportIndex (const Int &value) {
			return GS_SetViewportIndex( *this, value );
		}
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

		
	// methods
	public:
		explicit FragmentShader (PipelineNodes::Pipeline *pipeline) :
			Shader( pipeline, EShader::Fragment ),
			fs_screenResolution( FS_GetScreenResolution() ),	fs_fragCoord( FS_GetFragCoord() ),
			fs_frontFacing( FS_GetFrontFacing() ),				fs_helperInvocation( FS_GetHelperInvocation() ),
			fs_layer( FS_GetLayerIndex() ),						fs_numSamples( FS_GetNumSamples() ),
			fs_pointCoord( FS_GetPointCoord() ),				fs_primitiveID( FS_GetPrimitiveID() ),
			fs_sampleID( FS_GetSampleID() ),					fs_viewportIndex( FS_GetViewportIndex() )
		{}

		void SetFragmentDepth (const Float &value) {
			return FS_SetFragmentDepth( *this, value );
		}
	};


}	// ShaderNodes
}	// ShaderEditor
