// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/ResourcePacker/Pipelines/PipelineConverter.h"
#include "Engine/PipelineCompiler/Serializers/CppSerializer.h"

namespace ResPack
{
	using namespace PipelineCompiler;

/*
=================================================
	constructor
=================================================
*/
	PipelineConverter::PipelineConverter ()
	{}
	
/*
=================================================
	destructor
=================================================
*/
	PipelineConverter::~PipelineConverter ()
	{}
	
/*
=================================================
	AddPipeline
=================================================
*/
	void PipelineConverter::AddPipeline (const PipelinePtr &ppln)
	{
		_pipelines.Add( ppln );
	}
	
/*
=================================================
	AddDependency
=================================================
*/
	void PipelineConverter::AddDependency (StringCRef fname)
	{
		_dependencies.Add( fname );
	}

/*
=================================================
	SetConfig
=================================================
*/
	void PipelineConverter::SetConfig (const ConverterConfig &cfg)
	{
		_pplnConfig = cfg;
	}
	
/*
=================================================
	LoadPipeline
=================================================
*/
	bool PipelineConverter::LoadPipeline (StringCRef fname)
	{
		CHECK_ERR( ScriptHelper::RunScript( fname ) );
		return true;
	}
	
/*
=================================================
	LoadAllPipelines
=================================================
*/
	bool PipelineConverter::LoadAllPipelines (StringCRef folder, bool searchInSubfolders)
	{
		const auto IsPipelineExt = LAMBDA() (StringCRef ext)
		{{
			const char* script_ext[] = {
				"ppln", "ppln.c", "ppln.cpp"
			};
			for (auto e : script_ext) {
				if ( ext.EqualsIC( e ) )
					return true;
			}
			return false;
		}};

		CHECK_ERR( OS::FileSystem::IsDirectoryExist( folder ) );

		Queue<String> folder_stack;
		{
			String	dir;
			OS::FileSystem::GetCurrentDirectory( OUT dir );
			dir = FileAddress::BuildPath( dir, folder );

			folder_stack.PushBack( RVREF(dir) );
		}

		for (; not folder_stack.Empty();)
		{
			Array<String>	files;
			CHECK_ERR( OS::FileSystem::GetAllFilesInPath( folder_stack.Front(), OUT files ) );

			for (auto& file : files)
			{
				if ( not IsPipelineExt( FileAddress::GetExtensions( file ) ) )
					continue;

				CHECK_ERR( ScriptHelper::RunScript( FileAddress::BuildPath( folder_stack.Front(), file ) ) );
			}

			if ( not searchInSubfolders )
				break;

			files.Clear();
			CHECK_ERR( OS::FileSystem::GetAllDirsInPath( folder_stack.Front(), OUT files ) );

			for (auto& fname : files) {
				folder_stack.PushBack( FileAddress::BuildPath( folder_stack.Front(), fname ) );
			}
			
			folder_stack.PopFront();
		}
		return true;
	}
	
/*
=================================================
	LoadPipelineTemplate
=================================================
*/
	bool PipelineConverter::LoadPipelineTemplate (StringCRef fname, StringCRef funcName)
	{
		PipelinesSet	saved_pplns = RVREF( _pipelines );

		_dependencies.Add( fname );
		_pipelines.Clear();

		CHECK_ERR( ScriptHelper::RunScript( fname ) );
		CHECK_ERR( _pipelines.Count() == 1 );

		auto iter = _templates.Add( funcName, _pipelines.Front() );

		_pipelines = RVREF( saved_pplns );

		PipelineManager::Instance()->Remove( iter->second.ptr() );

		if ( DynCast< PipelineCompiler::ComputePipeline *>( iter->second ) )
		{
			String	signature;
			GXScript::GlobalFunction< ScriptComputePipeline (*) () >::GetDescriptor( OUT signature, funcName );

			AS_CALL( ScriptHelper::GetScriptEngine()->Get()->RegisterGlobalFunction(
							signature.cstr(),
							AngelScript::asFUNCTION( _GetComputePipelineTemplate ),
							AngelScript::asCALL_GENERIC,
							iter->second.ptr()
			) );
		}
		else
		if ( DynCast< PipelineCompiler::GraphicsPipeline *>( iter->second ) )
		{
			String	signature;
			GXScript::GlobalFunction< ScriptGraphicsPipeline (*) () >::GetDescriptor( OUT signature, funcName );

			AS_CALL( ScriptHelper::GetScriptEngine()->Get()->RegisterGlobalFunction(
							signature.cstr(),
							AngelScript::asFUNCTION( _GetGraphicsPipelineTemplate ),
							AngelScript::asCALL_GENERIC,
							iter->second.ptr()
			) );
		}
		else
			RETURN_ERR( "unsupported pipeline type!" );

		return true;
	}
	
/*
=================================================
	_GetGraphicsPipelineTemplate
=================================================
*/
	void PipelineConverter::_GetGraphicsPipelineTemplate (AngelScript::asIScriptGeneric *gen)
	{
		ASSERT( gen->GetArgCount() == 0 );
		
		void *	dst = gen->GetAddressOfReturnLocation();
		auto *	src = Cast<PipelineCompiler::GraphicsPipeline const *>(gen->GetAuxiliary());

		UnsafeMem::PlacementNew< ScriptGraphicsPipeline >( dst, src );
	}
	
/*
=================================================
	_GetComputePipelineTemplate
=================================================
*/
	void PipelineConverter::_GetComputePipelineTemplate (AngelScript::asIScriptGeneric *gen)
	{
		ASSERT( gen->GetArgCount() == 0 );
		
		void *	dst = gen->GetAddressOfReturnLocation();
		auto *	src = Cast<PipelineCompiler::ComputePipeline const *>(gen->GetAuxiliary());
		
		UnsafeMem::PlacementNew< ScriptComputePipeline >( dst, src );
	}

/*
=================================================
	ConvertPipelines
=================================================
*/
	bool PipelineConverter::ConvertPipelines (StringCRef outFolder)
	{
		CHECK_ERR( OS::FileSystem::IsDirectoryExist( outFolder ) );

		// add dependency to resource packer executable
		{
			for (auto& ppln : _pipelines)
			{
				for (auto& dep : _dependencies) {
					ppln->Depends( dep );
				}
			}
		}

		// convert pipelines
		CHECK_ERR( PipelineManager::Instance()->Convert(
						_pipelines,
						FileAddress::BuildPath( outFolder, "all_pipelines" ),
						new CppSerializer(),
						_pplnConfig ) );

		ShaderCompiler::Instance()->DestroyContext();

		_pipelines.Clear();
		_dependencies.Clear();
		_templates.Clear();

		return true;
	}
		
}	// ResPack
