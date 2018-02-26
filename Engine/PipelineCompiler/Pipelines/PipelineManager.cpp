// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Pipelines/PipelineManager.h"

namespace PipelineCompiler
{
	
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

					CHECK_ERR(	dst.name		== tex.name			and
								dst.imageType	== tex.imageType	and
								EPixelFormatClass::StrongComparison( dst.format, tex.format ) );

					dst.shaderUsage |= tex.shaderUsage;
					found = true;
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
					
					CHECK_ERR(	dst.name		== img.name			and
								dst.imageType	== img.imageType	and
								dst.format		== img.format );

					dst.shaderUsage |= img.shaderUsage;
					found = true;
				}
			}

			if ( not found ) {
				map.Add( img.name, Uniform(img) );
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
	Convert
----
	convert all pipelines to c++ code with glsl shaders,
	spirv binary and other.
=================================================
*/
	bool PipelineManager::Convert (StringCRef filename, Ptr<ISerializer> ser, const ConverterConfig &constCfg) const
	{
		ConverterConfig		cfg			= constCfg;
		const String		path		= FileAddress::GetPath( filename );
		const String		inc_name	= FileAddress::BuildPath( path, FileAddress::GetName( filename ), ser->GetHeaderFileExt() );
		
		if ( cfg.errorIfFileExist ) {
			CHECK_ERR( not OS::FileSystem::IsFileExist( inc_name ) );
		}
		CHECK_ERR( OS::FileSystem::IsDirectoryExist( path ) );

		_structTypes.Clear();
		_bindings.Clear();
		

		// prepare
		FOR( i, _pipelines )
		{
			const auto&		pp = _pipelines[i];

			CHECK_ERR( pp->Prepare( cfg ) );

			if ( cfg.searchForSharedTypes ) {
				CHECK_ERR( BasePipeline::_MergeStructTypes( pp->_structTypes, INOUT _structTypes ) );
			}

			if ( cfg.optimizeBindings )
			{
				_AddBinding_Func	func( _bindings );

				FOR( j, pp->bindings.uniforms ) {
					pp->bindings.uniforms[j].Apply( func );
				}
			}
		}

		CHECK_ERR( _ProcessSharedTypes( path, ser, INOUT cfg ) );


		String	includes;
		includes << ser->Comment( "This is generated file" );
		includes << ser->Comment( "Created at: "_str << ToString( OS::Date().Now() ) ) << '\n';
		includes << ser->BeginFile( true ) << '\n';

		FOR( i, cfg.includings ) {
			includes << ser->Include( cfg.includings[i] );
		}
		includes << '\n';
		includes << ser->DeclNamespace( cfg.nameSpace );
		includes << ser->BeginScope();

		cfg.includings.Clear();
		cfg.includings.PushFront( FileAddress::GetNameAndExt( inc_name ) );


		// convert
		FOR( i, _pipelines )
		{
			const auto&		pp		= _pipelines[i];
			const String	fname	= FileAddress::BuildPath( path, pp->Name(), ser->GetSourceFileExt() );

			if ( cfg.errorIfFileExist ) {
				CHECK_ERR( not OS::FileSystem::IsFileExist( fname ) );
			}

			WFilePtr		file	= File::HddWFile::New( fname );
			CHECK_ERR( file );

			String			str;
			CHECK_ERR( pp->Convert( OUT str, ser, cfg ) );

			CHECK_ERR( file->Write( StringCRef(str) ) );

			includes << ser->Comment( "From file '"_str << FileAddress::GetNameAndExt( fname ) << "'" );
			includes << ser->DeclFunction( "void", "Create_"_str << pp->Name(),
							{{"PipelineTemplateDescriptor&", "descr"}}, true ) << '\n';
		}

		includes << ser->EndScope();	// namespace
		includes << ser->EndFile( true );


		// save
		{
			WFilePtr	file = File::HddWFile::New( inc_name );
			CHECK_ERR( file );

			CHECK_ERR( file->Write( StringCRef(includes) ) );
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
	bool PipelineManager::_ProcessSharedTypes (StringCRef path, Ptr<ISerializer> ser, INOUT ConverterConfig &cfg) const
	{
		// update offsets by packing
		CHECK_ERR( BasePipeline::_CalculateOffsets( _structTypes ) );
		
		if ( cfg.addPaddingToStructs )
		{
			// replace types to aligned types and padding
			CHECK_ERR( BasePipeline::_AddPaddingToStructs( _structTypes ) );

			// update offsets by packing
			CHECK_ERR( BasePipeline::_CalculateOffsets( _structTypes ) );
		}

		// update location and bindings
		{
			auto&	textures	= _bindings[ BindableTypes::IndexOf<TextureUniform> ];
			FOR( i, textures ) {
				textures[i].second.Get<TextureUniform>().location.index = i;
			}

			auto&	images		= _bindings[ BindableTypes::IndexOf<ImageUniform> ];
			FOR( i, images ) {
				images[i].second.Get<ImageUniform>().location.index = i;
			}

			auto&	uniform_buffers	= _bindings[ BindableTypes::IndexOf<UniformBuffer> ];
			FOR( i, uniform_buffers ) {
				uniform_buffers[i].second.Get<UniformBuffer>().location.index = i;
			}

			auto&	storage_buffers = _bindings[ BindableTypes::IndexOf<StorageBuffer> ];
			FOR( i, storage_buffers ) {
				storage_buffers[i].second.Get<StorageBuffer>().location.index = i;
			}
		}

		// replace per-shader types with shared types
		{
			usize	replaced	= 0;
			usize	skiped		= 0;

			FOR( i, _pipelines )
			{
				auto&	pp = _pipelines[i];

				// replace struct types
				FOR( j, pp->_structTypes )
				{
					auto&					st	= pp->_structTypes[j];
					StructTypes::iterator	iter;

					if ( _structTypes.Find( st.first, OUT iter ) )
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
					FOR( j, pp->bindings.uniforms )
					{
						auto&					bind	= pp->bindings.uniforms[j];
						auto&					map		= _bindings[ bind.GetCurrentIndex() ];
						_ReplaceBinding_Func	func( map );

						bind.Apply( func );
					}
				}

				CHECK_ERR( pp->_UpdateBufferSizes() );
			}

			LOG( ("Replaced shader types: "_str << replaced << ", skiped: " << skiped).cstr(), ELog::Debug );
		}
		

		// save shared types
		if ( cfg.searchForSharedTypes /*and not _structTypes.Empty()*/ )
		{
			cfg.includings.PushBack( String() );
			CHECK_ERR( _SaveSharedTypes( path, ser, cfg.nameSpace, OUT cfg._glslTypes, OUT cfg.includings.Back() ) );
		}
		return true;
	}

/*
=================================================
	_SaveSharedTypes
=================================================
*/
	bool PipelineManager::_SaveSharedTypes (StringCRef path, Ptr<ISerializer> ser, StringCRef nameSpace, OUT String &glslSource, OUT String &filename) const
	{
		filename = FileAddress::BuildPath( "", "shared_types", ser->GetHeaderFileExt() );

		const String	fname		= FileAddress::BuildPath( path, filename );
		String			str;
		
		str << ser->Comment( "This is generated file" );
		str << ser->Comment( "Created at: "_str << ToString( OS::Date().Now() ) ) << '\n';
		str << ser->BeginFile( true ) << '\n';
		str << ser->DeclNamespace( nameSpace );
		str << ser->BeginScope();

		CHECK_ERR( BasePipeline::_AllStructsToString( _structTypes, ser, OUT str, OUT glslSource ) );
		
		str << ser->EndScope();	// namespace
		str << ser->EndFile( true );


		// save c++ file
		WFilePtr		file	= File::HddWFile::New( fname );
		CHECK_ERR( file );
		
		CHECK_ERR( file->Write( StringCRef(str) ) );
		return true;
	}


}	// PipelineCompiler
