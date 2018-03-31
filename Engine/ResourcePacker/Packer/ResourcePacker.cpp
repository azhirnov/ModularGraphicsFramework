// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/ResourcePacker/Packer/ResourcePacker.h"
#include "Engine/ResourcePacker/Pipelines/ScriptGraphicsPipeline.h"
#include "Engine/ResourcePacker/Pipelines/ScriptComputePipeline.h"
#include "Engine/Script/Bindings/DefaultBindings.h"

namespace ResPack
{

/*
=================================================
	constructor
=================================================
*/
	ResourcePacker::ResourcePacker ()
	{
		GXMath::InitializeSTLMath();
		Logger::GetInstance()->Open( "log", false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	ResourcePacker::~ResourcePacker ()
	{
		Logger::GetInstance()->Close();
	}
	
/*
=================================================
	Instance
=================================================
*/
	Ptr<ResourcePacker> ResourcePacker::Instance ()
	{
		static ResourcePacker	inst;
		return &inst;
	}

/*
=================================================
	Initialize
=================================================
*/
	bool ResourcePacker::Initialize ()
	{
		_Bind( ScriptHelper::GetScriptEngine() );

		return true;
	}
	
/*
=================================================
	AddPipeline
=================================================
*/
	void ResourcePacker::AddPipeline (const PipelinePtr &ppln)
	{
		_pplnConverter.AddPipeline( ppln );
	}
	
/*
=================================================
	Run
=================================================
*/
	bool ResourcePacker::Run (StringCRef res)
	{
		const String	path = FileAddress::GetPath( res );

		CHECK_ERR( OS::FileSystem::SetCurrentDirectory( path ) );

		CHECK_ERR( ScriptHelper::RunScript( res ) );
		return true;
	}

/*
=================================================
	ScriptResourcePacker
=================================================
*/
	class ResourcePacker::ScriptResourcePacker final
	{
	public:
		ScriptResourcePacker () {}
		ScriptResourcePacker (const ScriptResourcePacker &) = default;
		~ScriptResourcePacker () {}

		void SetPipelineConfig (const PipelineConverterConfig &cfg) {
			Instance()->_pplnConverter.SetConfig( cfg );
		}

		void AddPipeline (const String &folder) {
			Instance()->_pplnConverter.LoadPipeline( folder );
		}

		void AddAllPipelines (const String &folder) {
			Instance()->_pplnConverter.LoadAllPipelines( folder );
		}

		void AddPipelineTemplate (const String &filename, const String &funcName) {
			Instance()->_pplnConverter.LoadPipelineTemplate( filename, funcName );
		}

		void ConvertPipelines (const String &output) {
			Instance()->_pplnConverter.ConvertPipelines( output );
		}
	};
	
/*
=================================================
	_Bind
=================================================
*/
	void ResourcePacker::_Bind (GXScript::ScriptEnginePtr se)
	{
		using namespace GXScript;

		DefaultBindings::BindScalarMath( se.ptr() );
		DefaultBindings::BindVectorMath( se.ptr() );
		DefaultBindings::BindString( se.ptr() );
		DefaultBindings::BindLog( se.ptr() );

		ScriptPipeline::BindAll( se );
		ScriptGraphicsPipeline::Bind( se );
		ScriptComputePipeline::Bind( se );

		using Self = ScriptResourcePacker;

		ClassBinder<Self>		binder{ se, "ResourcePacker" };

		binder.CreateClassValue();
		binder.AddMethod( &Self::SetPipelineConfig,		"SetConfig" );
		binder.AddMethod( &Self::AddPipeline,			"AddPipeline" );
		binder.AddMethod( &Self::AddAllPipelines,		"AddAllPipelines" );
		binder.AddMethod( &Self::AddPipelineTemplate,	"AddPipelineTemplate" );
		binder.AddMethod( &Self::ConvertPipelines,		"ConvertPipelines" );
	}

}	// ResPack
