// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Pipelines/PipelineManager.h"
#include "Engine/PipelineCompiler/Pipelines/GraphicsPipeline.h"
#include "Core/STL/ThreadSafe/Singleton.h"

namespace PipelineCompiler
{
	using WFilePtr = GXFile::WFilePtr;
	
/*
=================================================
	constructor
=================================================
*/
	PipelineManager::PipelineManager ()
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	PipelineManager::~PipelineManager ()
	{
	}

/*
=================================================
	Add
=================================================
*/
	void PipelineManager::Add (Ptr<BasePipeline> pp)
	{
		_pipelines.Add( pp );
	}
	
/*
=================================================
	Remove
=================================================
*/
	void PipelineManager::Remove (Ptr<BasePipeline> pp)
	{
		_pipelines.Erase( pp );
	}

/*
=================================================
	Instance
=================================================
*/
	Ptr<PipelineManager>  PipelineManager::Instance ()
	{
		return SingletonMultiThread::Instance<PipelineManager>();
	}
	
/*
=================================================
	_AddBinding_Func
=================================================
*/
	struct PipelineManager::_AddBinding_Func
	{
	// types
		using Uniform	= BasePipeline::Bindings::_Uniform;

	// variables
		SharedTypes &		bindings;

	// methods
		_AddBinding_Func (SharedTypes &bindings) : bindings(bindings)
		{}

		template <typename T>
		static constexpr usize Id (const T&)
		{
			return BindableTypes::IndexOf<T>;
		}
		
/*
=================================================
	operator (TextureUniform)
=================================================
*/
		bool operator () (const TextureUniform &tex)
		{
			NamedTypes&		map = bindings[ Id(tex) ];
			usize			idx;
			bool			found = false;
			
			if ( map.FindFirstIndex( tex.name, OUT idx ) )
			{
				for (; idx < map.Count() and map[idx].first == tex.name; ++idx)
				{
					auto&	dst = map[idx].second.Get<TextureUniform>();

					if ( dst.name		== tex.name			and
						 dst.imageType	== tex.imageType	and
						 EPixelFormatClass::StrongComparison( dst.format, tex.format ) )
					{
						dst.shaderUsage |= tex.shaderUsage;
						found = true;
						break;
					}
				}
			}

			if ( not found ) {
				map.Add( tex.name, Uniform(tex) );
			}
			return true;
		}
		
/*
=================================================
	operator (ImageUniform)
=================================================
*/
		bool operator () (const ImageUniform &img)
		{
			NamedTypes&		map = bindings[ Id(img) ];
			usize			idx;
			bool			found = false;
			
			if ( map.FindFirstIndex( img.name, OUT idx ) )
			{
				for (; idx < map.Count() and map[idx].first == img.name; ++idx)
				{
					auto&	dst = map[idx].second.Get<ImageUniform>();
					
					if ( dst.name		== img.name			and
						 dst.imageType	== img.imageType	and
						 dst.format		== img.format )
					{
						dst.shaderUsage |= img.shaderUsage;
						found = true;
						break;
					}
				}
			}

			if ( not found ) {
				map.Add( img.name, Uniform(img) );
			}
			return true;
		}

/*
=================================================
	operator (SubpassInput)
=================================================
*/
		bool operator () (const SubpassInput &spi)
		{
			NamedTypes&		map = bindings[ Id(spi) ];
			usize			idx;
			bool			found = false;
			
			if ( map.FindFirstIndex( spi.name, OUT idx ) )
			{
				for (; idx < map.Count() and map[idx].first == spi.name; ++idx)
				{
					auto&	dst = map[idx].second.Get<SubpassInput>();
					
					if ( dst.name				== spi.name				and
						 dst.attachmentIndex	== spi.attachmentIndex	and
						 dst.isMultisample		== spi.isMultisample )
					{
						dst.shaderUsage |= spi.shaderUsage;
						found = true;
						break;
					}
				}
			}

			if ( not found ) {
				map.Add( spi.name, Uniform(spi) );
			}
			return true;
		}
		
/*
=================================================
	operator (UniformBuffer)
=================================================
*/
		bool operator () (const UniformBuffer &buf)
		{
			NamedTypes&		map = bindings[ Id(buf) ];
			usize			idx;
			bool			found = false;
			
			if ( map.FindFirstIndex( buf.name, OUT idx ) )
			{
				for (; idx < map.Count() and map[idx].first == buf.name; ++idx)
				{
					auto&	dst = map[idx].second.Get<UniformBuffer>();
					
					if ( dst.typeName == buf.typeName )
					{
						CHECK_ERR(	dst.name		== buf.name		and
									dst.typeName	== buf.typeName	and
									dst.packing		== buf.packing );

						dst.shaderUsage |= buf.shaderUsage;
						found = true;
						break;
					}
				}
			}
			
			if ( not found ) {
				map.Add( buf.name, Uniform(buf) );
			}
			return true;
		}
		
/*
=================================================
	operator (StorageBuffer)
=================================================
*/
		bool operator () (const StorageBuffer &buf)
		{
			NamedTypes&		map = bindings[ Id(buf) ];
			usize			idx;
			bool			found = false;

			if ( map.FindFirstIndex( buf.name, OUT idx ) )
			{
				for (; idx < map.Count() and map[idx].first == buf.name; ++idx)
				{
					auto&	dst = map[idx].second.Get<StorageBuffer>();
				
					if ( dst.typeName == buf.typeName )
					{
						CHECK_ERR(	dst.name		== buf.name		and
									dst.typeName	== buf.typeName	and
									dst.packing		== buf.packing );

						dst.shaderUsage |= buf.shaderUsage;
						found = true;
					}
				}
			}

			if ( not found ) {
				map.Add( buf.name, Uniform(buf) );
			}
			return true;
		}
	};

/*
=================================================
	ConvertAll
----
	convert all pipelines to c++ code with glsl shaders,
	spirv binary and other.
=================================================
*/
	bool PipelineManager::ConvertAll (StringCRef filename, Ptr<ISerializer> ser, const ConverterConfig &cfg) const
	{
		return Convert( _pipelines, filename, ser, cfg );
	}
	
/*
=================================================
	Convert
=================================================
*/
	bool PipelineManager::Convert (ArrayCRef<BasePipelinePtr> pipelines, StringCRef filename, Ptr<ISerializer> ser, const ConverterConfig &cfg) const
	{
		return _Convert( pipelines, filename, ser, cfg );
	}
	
	bool PipelineManager::Convert (ArrayCRef<BasePipeline *> pipelines, StringCRef filename, Ptr<ISerializer> ser, const ConverterConfig &cfg) const
	{
		return _Convert( pipelines, filename, ser, cfg );
	}
	
/*
=================================================
	_Convert
=================================================
*/
	template <typename PplnCollection>
	bool PipelineManager::_Convert (const PplnCollection &pipelines, StringCRef filename, Ptr<ISerializer> ser, const ConverterConfig &constCfg) const
	{
		ConverterConfig		cfg			= constCfg;
		const String		path		= FileAddress::GetPath( filename );
		const String		inc_name	= FileAddress::BuildPath( path, FileAddress::GetName( filename ), ser->GetHeaderFileExt() );
		const String		types_fname	= FileAddress::BuildPath( path, "shared_types", ser->GetHeaderFileExt() );
		
		if ( cfg.errorIfFileExist ) {
			CHECK_ERR( not OS::FileSystem::IsFileExist( inc_name ) );
		}
		CHECK_ERR( OS::FileSystem::IsDirectoryExist( path ) );

		_sharedStructTypes.Clear();
		_sharedBindings.Clear();
		_sharedVertexInput.Clear();
		_sharedFragmentOutput.Clear();
		

		// prepare
		for (auto& pp : pipelines)
		{
			CHECK_ERR( pp->Prepare( cfg ) );

			if ( cfg.searchForSharedTypes ) {
				CHECK_ERR( BasePipeline::_MergeStructTypes( pp->_structTypes, INOUT _sharedStructTypes ) );
			}

			// extract resource bindings
			if ( cfg.optimizeBindings )
			{
				_AddBinding_Func	func( INOUT _sharedBindings );

				FOR( j, pp->bindings.uniforms ) {
					pp->bindings.uniforms[j].Accept( func );
				}
			}

			// extract attribs
			if ( cfg.optimizeVertexInput )
			{
				/*if ( auto* gppln = DynCast<GraphicsPipeline *>(pp) )
				{
					CHECK( _MergeVertexInput( gppln->shaders.vertex, INOUT _sharedAttribs ) );
				}*/
			}

			// extract fragment output
			if ( cfg.optimizeFragmentOutput )
			{
				// TODO
			}
		}

		String	shared_types_src;
		CHECK_ERR( _ProcessSharedTypes( pipelines, ser, OUT shared_types_src, INOUT cfg ) );


		String	includes;
		includes << ser->Comment( "This is generated file" );
		//includes << ser->Comment( "Created at: "_str << ToString( Date().Now() ) ) << '\n';
		includes << ser->BeginFile( true );

		if ( ser->GetSourceFileExt().EqualsIC( "cpp" ) )
		{
			includes << R"#(
#include "Engine/Platforms/Engine.Platforms.h"

#ifdef GRAPHICS_API_SOFT
#include "Engine/Platforms/Soft/ShaderLang/SWLang.h"
#endif

namespace Pipelines
{
	using namespace GX_STL;
	using namespace GX_STL::GXTypes;
	using namespace GX_STL::GXMath;

	using namespace Engine::Platforms;
}

)#";
		}

		FOR( i, cfg.includings ) {
			includes << ser->Include( cfg.includings[i] );
		}
		
		if ( cfg.searchForSharedTypes )
			includes << ser->Include( FileAddress::BuildPath( "", "shared_types", ser->GetHeaderFileExt() ) );

		includes << '\n';
		includes << ser->BeginNamespace( cfg.nameSpace );

		cfg.includings.Clear();
		cfg.includings.PushFront( FileAddress::GetNameAndExt( inc_name ) );


		// convert
		bool	any_file_builded = false;

		FOR( i, pipelines )
		{
			const auto&		pp		= pipelines[i];
			const String	fname	= FileAddress::BuildPath( path, pp->Name(), ser->GetSourceFileExt() );
			bool			rebuild	= true;

			if ( OS::FileSystem::IsFileExist( fname ) )
			{
				const TimeL	time = OS::FileSystem::GetFileLastModificationTime( fname ).ToTime();

				rebuild = (pp->LastEditTime() > time or not constCfg.minimalRebuild);

				if ( cfg.errorIfFileExist )
					RETURN_ERR( "output file already exist!" );
			}

			if ( rebuild )
			{
				WFilePtr		file	= GXFile::HddWFile::New( fname );
				CHECK_ERR( file );

				String			str;
				CHECK_ERR( pp->Convert( OUT str, ser, cfg ) );

				CHECK_ERR( file->Write( StringCRef(str) ) );

				LOG( "Converted pipeline '"_str << fname << "'", ELog::Debug );
			}
			else
			{
				LOG( "Pipeline '"_str << fname << "' skiped, no changes detected", ELog::Debug );
			}

			any_file_builded |= rebuild;

			includes << ser->Comment( "From file '"_str << FileAddress::GetNameAndExt( fname ) << "'" );
			includes << ser->DeclFunction( "void", "Create_"_str << pp->Name(),
							{{"PipelineTemplateDescription&", "descr"}}, true ) << '\n';
		}

		includes << ser->EndNamespace();
		includes << ser->EndFile( true );


		// save 'all_pipelines'
		if ( any_file_builded or
			 not OS::FileSystem::IsFileExist( inc_name ) )
		{
			WFilePtr	file = GXFile::HddWFile::New( inc_name );
			CHECK_ERR( file );

			CHECK_ERR( file->Write( StringCRef(includes) ) );
		}
		
		// save 'shared_types'
		if ( cfg.searchForSharedTypes and
			 (any_file_builded or not OS::FileSystem::IsFileExist( types_fname )) )
		{
			WFilePtr	file = GXFile::HddWFile::New( types_fname );
			CHECK_ERR( file );
		
			CHECK_ERR( file->Write( StringCRef(shared_types_src) ) );
		}
		return true;
	}
	
/*
=================================================
	_ReplaceBinding_Func
=================================================
*/
	struct PipelineManager::_ReplaceBinding_Func
	{
		NamedTypes&				bindings;
		NamedTypes::iterator	iter;

		_ReplaceBinding_Func (NamedTypes &bindings) : bindings(bindings)
		{}


		void operator () (TextureUniform &tex)
		{
			if ( bindings.Find( tex.name, OUT iter ) )
			{
				tex.location.index = iter->second.Get<TextureUniform>().location.index;
			}
			else
				tex.location.index = UMax;	// invalidate to refresh binding indices
		}


		void operator () (ImageUniform &img)
		{
			if ( bindings.Find( img.name, OUT iter ) )
			{
				img.location.index = iter->second.Get<ImageUniform>().location.index;
			}
			else
				img.location.index = UMax;	// invalidate to refresh binding indices
		}


		void operator () (SubpassInput &img)
		{
			if ( bindings.Find( img.name, OUT iter ) )
			{
				img.location.index = iter->second.Get<SubpassInput>().location.index;
			}
			else
				img.location.index = UMax;	// invalidate to refresh binding indices
		}


		void operator () (UniformBuffer &buf)
		{
			if ( bindings.Find( buf.name, OUT iter ) )
			{
				buf.location.index = iter->second.Get<UniformBuffer>().location.index;
			}
			else
				buf.location.index = UMax;	// invalidate to refresh binding indices
		}


		void operator () (StorageBuffer &buf)
		{
			if ( bindings.Find( buf.name, OUT iter ) )
			{
				buf.location.index = iter->second.Get<StorageBuffer>().location.index;
			}
			else
				buf.location.index = UMax;	// invalidate to refresh binding indices
		}
	};

/*
=================================================
	_ProcessSharedTypes
=================================================
*/
	template <typename PplnCollection>
	bool PipelineManager::_ProcessSharedTypes (const PplnCollection &pipelines, Ptr<ISerializer> ser, OUT String &fileSource, INOUT ConverterConfig &cfg) const
	{
		if ( cfg.addPaddingToStructs )
		{
			// replace types to aligned types and padding
			CHECK_ERR( BasePipeline::_AddPaddingToStructs( _sharedStructTypes ) );

			// update offsets by packing
			CHECK_ERR( BasePipeline::_CalculateOffsets( _sharedStructTypes ) );
		}

		// update location and bindings
		{
			auto&	textures	= _sharedBindings[ BindableTypes::IndexOf<TextureUniform> ];
			FOR( i, textures ) {
				textures[i].second.template Get<TextureUniform>().location.index = uint(i);
			}

			auto&	images		= _sharedBindings[ BindableTypes::IndexOf<ImageUniform> ];
			FOR( i, images ) {
				images[i].second.template Get<ImageUniform>().location.index = uint(i);
			}

			auto&	uniform_buffers	= _sharedBindings[ BindableTypes::IndexOf<UniformBuffer> ];
			FOR( i, uniform_buffers ) {
				uniform_buffers[i].second.template Get<UniformBuffer>().location.index = uint(i);
			}

			auto&	storage_buffers = _sharedBindings[ BindableTypes::IndexOf<StorageBuffer> ];
			FOR( i, storage_buffers ) {
				storage_buffers[i].second.template Get<StorageBuffer>().location.index = uint(i);
			}
		}

		// update vertex input
		{
			// TODO
		}

		// replace per-shader types with shared types
		{
			usize	replaced	= 0;
			usize	skiped		= 0;

			for (auto& pp : pipelines)
			{
				// replace struct types
				for (auto& st : pp->_structTypes)
				{
					StructTypes::iterator	iter;

					if ( _sharedStructTypes.Find( st.first, OUT iter ) )
					{
						st.second = iter->second;
						++replaced;
					}
					else
						++skiped;
				}

				// replace bindings
				if ( cfg.optimizeBindings )
				{
					for (auto& bind : pp->bindings.uniforms)
					{
						auto&					map		= _sharedBindings[ bind.GetCurrentIndex() ];
						_ReplaceBinding_Func	func( map );

						bind.Accept( func );
					}
				}

				CHECK_ERR( pp->_UpdateBufferSizes() );

				// replace vertex input
				if ( cfg.optimizeVertexInput )
				{
				}

				// replace fragment output
				if ( cfg.optimizeFragmentOutput )
				{
					// TODO
				}
			}

			LOG( "Replaced shader types: "_str << replaced << ", skiped: " << skiped, ELog::Debug );
		}
		

		// save shared types
		if ( cfg.searchForSharedTypes )
		{
			CHECK_ERR( _SaveSharedTypes( ser, cfg.nameSpace, OUT fileSource ) );
		}
		return true;
	}

/*
=================================================
	_SaveSharedTypes
=================================================
*/
	bool PipelineManager::_SaveSharedTypes (Ptr<ISerializer> ser, StringCRef nameSpace, OUT String &fileSource) const
	{
		String&	str = fileSource;
		
		str << ser->Comment( "This is generated file" );
		//str << ser->Comment( "Created at: "_str << ToString( Date().Now() ) ) << '\n';
		str << ser->BeginFile( true ) << '\n';
		str << ser->BeginNamespace( nameSpace );

		CHECK_ERR( BasePipeline::_SerializeStructs( _sharedStructTypes, ser, OUT str ) );
		
		str << ser->EndNamespace();
		str << ser->EndFile( true );
		return true;
	}
	
/*
=================================================
	_MergeVertexInput
=================================================
*/
	bool PipelineManager::_MergeVertexInput (const ArrayCRef<Varying> &input, INOUT Array<Varying> &output)
	{
		for (const auto& left : input)
		{
			if ( not left.qualifier[ EVariableQualifier::In ] )
				continue;

			bool	found = false;

			for (const auto& right : output)
			{
				if ( right.name == left.name )
				{
					CHECK_ERR( left.type		== right.type );
					CHECK_ERR( left.arraySize	== right.arraySize );
					CHECK_ERR( left.location	== right.location );
					CHECK_ERR( left.fields		== right.fields );

					found = true;
					break;
				}
			}

			if ( not found )
			{
				output.PushBack( left );
			}
		}
		return true;
	}
	
/*
=================================================
	_MergeFragmentOutput
=================================================
*/
	bool PipelineManager::_MergeFragmentOutput (const ArrayCRef<Varying> &input, INOUT Array<Varying> &output)
	{
		TODO( "" );
		return false;
	}

}	// PipelineCompiler
