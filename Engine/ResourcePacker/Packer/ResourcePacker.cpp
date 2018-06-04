// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/ResourcePacker/Packer/ResourcePacker.h"

#include "Engine/ResourcePacker/Pipelines/ScriptGraphicsPipeline.h"
#include "Engine/ResourcePacker/Pipelines/ScriptComputePipeline.h"

#include "Engine/ResourcePacker/FilePacker/CppFileSystemPacker.h"
#include "Engine/ResourcePacker/Images/ImageConverter.h"

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
	SetExecutable
=================================================
*/
	void ResourcePacker::SetExecutable (StringCRef name)
	{
		if ( OS::FileSystem::IsAbsolutePath( name ) )
		{
			_exeName = name;
		}
		else
		{
			String	dir;
			OS::FileSystem::GetCurrentDirectory( OUT dir );

			_exeName = FileAddress::BuildPath( dir, name );
		}

		ASSERT( OS::FileSystem::IsFileExist( _exeName ) );
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
	_PackFile
=================================================
*
	bool ResourcePacker::_PackFile (IFilePackerPtr &&file, StringCRef input, StringCRef output)
	{
		CHECK_ERR( file->Load( input ) );
		CHECK_ERR( file->Save( output ) );

		return true;
	}

/*
=================================================
	ScriptResourcePacker
=================================================
*/
	class ResourcePacker::ScriptResourcePacker final
	{
	private:
		Ptr< PipelineConverter >	_pplnConv;

	public:
		ScriptResourcePacker () : _pplnConv{ &Instance()->_pplnConverter } {}
		ScriptResourcePacker (const ScriptResourcePacker &) = default;
		~ScriptResourcePacker () {}

		void SetPipelineConfig (const PipelineConverterConfig &cfg) {
			_pplnConv->SetConfig( cfg );
		}

		void AddPipeline (const String &filename) {
			_pplnConv->LoadPipeline( filename );
			_pplnConv->AddDependency( ScriptHelper::CurrentFileName() );
			_pplnConv->AddDependency( Instance()->GetExecutableName() );
		}

		void AddAllPipelines (const String &folder) {
			_pplnConv->LoadAllPipelines( folder, false );
			_pplnConv->AddDependency( ScriptHelper::CurrentFileName() );
			_pplnConv->AddDependency( Instance()->GetExecutableName() );
		}
		
		void RecursiveAddAllPipelines (const String &folder) {
			_pplnConv->LoadAllPipelines( folder, true );
			_pplnConv->AddDependency( ScriptHelper::CurrentFileName() );
			_pplnConv->AddDependency( Instance()->GetExecutableName() );
		}

		void AddPipelineTemplate (const String &filename, const String &funcName) {
			_pplnConv->LoadPipelineTemplate( filename, funcName );
			_pplnConv->AddDependency( ScriptHelper::CurrentFileName() );
			_pplnConv->AddDependency( Instance()->GetExecutableName() );
		}

		void ConvertPipelines (const String &output) {
			_pplnConv->ConvertPipelines( output );
		}

		void PackTextFile (const String &input, const String &output) {
			//Instance()->_PackFile( IFilePackerPtr{new TextFilePacker()}, input, output );
		}

		void PackBinaryFile (const String &input, const String &output) {
			//Instance()->_PackFile( IFilePackerPtr{new BinaryFilePacker()}, input, output );
		}
	};
	
/*
=================================================
	_Bind
=================================================
*/
	void ResourcePacker::_Bind (GXScript::ScriptEnginePtr se)
	{
		using namespace GX_STL::GXScript;

		DefaultBindings::BindScalarMath( se.ptr() );
		DefaultBindings::BindVectorMath( se.ptr() );
		DefaultBindings::BindString( se.ptr() );
		DefaultBindings::BindLog( se.ptr() );

		ScriptPipeline::BindAll( se );
		ScriptGraphicsPipeline::Bind( se );
		ScriptComputePipeline::Bind( se );

		ImageConverter::BindAll( se );

		using Self = ScriptResourcePacker;

		ClassBinder<Self>		binder{ se, "ResourcePacker" };

		binder.CreateClassValue();
		binder.AddMethod( &Self::SetPipelineConfig,			"SetConfig" );
		binder.AddMethod( &Self::AddPipeline,				"AddPipeline" );
		binder.AddMethod( &Self::AddAllPipelines,			"AddAllPipelines" );
		binder.AddMethod( &Self::RecursiveAddAllPipelines,	"RecursiveAddPipelines" );
		binder.AddMethod( &Self::AddPipelineTemplate,		"AddPipelineTemplate" );
		binder.AddMethod( &Self::ConvertPipelines,			"ConvertPipelines" );
		binder.AddMethod( &Self::PackTextFile,				"PackTextFile" );
		binder.AddMethod( &Self::PackBinaryFile,			"PackBinaryFile" );
	}

}	// ResPack
