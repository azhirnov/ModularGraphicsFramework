// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Pipelines/BasePipeline.h"
#include "Engine/PipelineCompiler/Common/ToGLSL.h"
#include "Engine/PipelineCompiler/Common/Packing.h"

namespace PipelineCompiler
{
	
/*
=================================================
	_StructToString
=================================================
*/
	String  BasePipeline::_StructToString (const StructTypes &types, StringCRef typeName, bool skipLayouts)
	{
		String						str;
		StructTypes::const_iterator	iter;

		if ( types.Find( typeName, OUT iter ) )
		{
			const bool	is_block	= iter->second.type == EShaderVariable::UniformBlock or
									  iter->second.type == EShaderVariable::StorageBlock;

			FOR( i, iter->second.fields )
			{
				const auto&		fl = iter->second.fields[i];
			
				str << "\t";

				if ( not skipLayouts and is_block ) {
					str << "layout(offset=" << usize(fl.offset)
						<< ", align=" << usize(fl.align) << ") ";
				}

				if ( EShaderVariable::IsStruct( fl.type ) )
					str << fl.typeName;
				else
					str << ToStringGLSL( fl.type );

				str << "  " << fl.name;
					
				if ( fl.arraySize > 1 )
					str << "[" << fl.arraySize << "]";
				else
				if ( fl.arraySize == 0 )
					str << "[]";
					
				str << ";    // offset: " << usize(fl.offset) << ", align: " << usize(fl.align);
				str << "\n";
			}
		}
		return str;
	}

/*
=================================================
	_BindingsToString_Func
=================================================
*/
	struct BasePipeline::_BindingsToString_Func
	{
	// variables
		BasePipeline const *	_pp;
		String	&				_str;
		EShader::type			_shaderType;
		EShaderType				_shaderApi;
		bool					_useOriginTypes;
		bool					_skipBufferLayouts;


	// methods
		_BindingsToString_Func (BasePipeline const *pp, EShader::type shaderType, EShaderType shaderApi, bool useOriginTypes, OUT String &str) :
			_pp(pp),							_shaderType(shaderType),
			_shaderApi(shaderApi),				_str(str),
			_useOriginTypes(useOriginTypes),	_skipBufferLayouts(shaderApi == EShaderType::SPIRV)
		{}


		void operator () (const TextureUniform &tex) const
		{
			if ( not tex.shaderUsage[ _shaderType ] )
				return;

			_str << tex.ToStringGLSL( _shaderApi );
		}


		void operator () (const ImageUniform &img) const
		{
			if ( not img.shaderUsage[ _shaderType ] )
				return;

			_str << img.ToStringGLSL( _shaderApi );
		}


		void operator () (const UniformBuffer &ub) const
		{
			if ( not ub.shaderUsage[ _shaderType ] )
				return;

			_str << ub.ToStringGLSL( _StructToString( ub.typeName ), _shaderApi );
		}


		void operator () (const StorageBuffer &ssb) const
		{
			if ( not ssb.shaderUsage[ _shaderType ] )
				return;

			_str << ssb.ToStringGLSL( _StructToString( ssb.typeName ), _shaderApi );
		}

		String _StructToString (StringCRef typeName) const
		{
			return BasePipeline::_StructToString( (_useOriginTypes ? _pp->_originTypes : _pp->_structTypes), typeName, _skipBufferLayouts );
		}
	};
	
/*
=================================================
	_BindingsToString
=================================================
*/
	void BasePipeline::_BindingsToString (EShader::type shaderType, EShaderType shaderApi, bool useOriginTypes, OUT String &str) const
	{
		_BindingsToString_Func	func( this, shaderType, shaderApi, useOriginTypes, OUT str );

		//TODO("");
		FOR( i, bindings.uniforms )
		{
			const auto&		un = bindings.uniforms[i];

			un.Apply( func );
			str << '\n';
		}
	}

/*
=================================================
	_MergeStructTypes
=================================================
*/
	bool BasePipeline::_MergeStructTypes (const StructTypes &newTypes, INOUT StructTypes &currTypes)
	{
		FOR( i, newTypes )
		{
			const auto&		st = newTypes[i];

			CHECK_ERR( _AddStructType( st.second, INOUT currTypes ) );
		}
		return true;
	}
	
/*
=================================================
	_AllStructsToString
=================================================
*/
	bool BasePipeline::_AllStructsToString (const StructTypes &structTypes, Ptr<ISerializer> ser, OUT String &serialized, OUT String &glslSource)
	{
		using Dependencies_t = HashMap<StringCRef, HashSet<StringCRef>>;

		String &			str		= serialized;
		Dependencies_t		depends;

		
		// build dependencies pass 1
		FOR( i, structTypes ) {
			depends.Add( structTypes[i].first, Uninitialized );
		}
		
		// build dependencies pass 2
		FOR( i, structTypes )
		{
			const auto&					st	= structTypes[i];
			Dependencies_t::iterator	iter;

			if ( depends.Find( st.first, OUT iter ) )
			{
				FOR( j, st.second.fields )
				{
					const auto&		fl = st.second.fields[j];

					if ( EShaderVariable::IsStruct( fl.type ) and
						 depends.IsExist( fl.typeName ) )
					{
						iter->second.Add( fl.typeName );
					}
				}
			}
		}
		
		// build dependencies pass 3
		for (bool changed = true; changed;)
		{
			changed = false;

			FOR( i, depends )
			{
				auto&			curr		= depends[i];
				const usize		deps_count	= curr.second.Count();

				FOR( j, curr.second )
				{
					Dependencies_t::iterator	iter;

					if ( depends.Find( curr.second[j], OUT iter ) )
					{
						// add dependencies of dependent type
						FOR( k, iter->second ) {
							curr.second.Add( iter->second[k] );
						}

						// this condition depends of HashSet implementation,
						// in other implementations you must restart loop if 'Add' method used
						if ( deps_count != curr.second.Count() )
							break;
					}
				}

				if ( deps_count != curr.second.Count() )
				{
					changed = true;
					break;
				}
			}
		}


		// sort by dependent types
		Array<usize>	indices;	indices.Resize( depends.Count() );

		FOR( i, indices ) { indices[i] = i; }

		Sort( indices, LAMBDA( &depends ) (auto left, auto right)
						{
							const auto&	l = depends[left];
							const auto&	r = depends[right];
							FOR( i, l.second ) {
								if ( l.second[i] == r.first )
									return true;	// left is dependent of right
							}
							FOR( i, r.second ) {
								if ( r.second[i] == l.first )
									return false;	// right is dependent of left
							}
							return l.second.Count() > r.second.Count();	// left is not dependent of right, just for sorting
						}
			);


		// convert to target
		FOR( i, indices )
		{
			const auto&		st			= structTypes[indices[i]];
			const bool		glsl_keep	= st.second.type == EShaderVariable::Struct;			// keep struct, varying
			const bool		ser_keep	= not st.second.packing[ EVariablePacking::Varying ];	// keep struct, buffer
			const bool		is_dynamic	= st.second.fields.Back().arraySize == 0;

			if ( ser_keep ) {
				str << ser->Comment( "Packing: "_str << EVariablePacking::ToString(st.second.packing) )
					<< ser->BeginStruct( st.second.typeName, (uint)st.second.stride );
			}
			if ( glsl_keep ) {
				glslSource << "struct " << st.second.typeName << "\n{\n";
			}

			// staticaly sized UB or SSB
			FOR( j, st.second.fields )
			{
				const auto&		fl			= st.second.fields[j];
				const String	type_name	= EShaderVariable::IsStruct( fl.type ) ? fl.typeName : ser->ToString( fl.type );
				const uint		array_size	= fl.arraySize;
				const uint		align		= (usize) fl.align;
				const uint		offset		= (usize) fl.offset;

				if ( ser_keep and array_size == 0 ) {
					ASSERT( j == st.second.fields.LastIndex() );
				}
				else
				if ( ser_keep ) {
					str << ser->StructField( fl.name, type_name, array_size, offset, align, uint(fl.stride) * fl.arraySize );
				}
				if ( glsl_keep ) {
					glslSource	<< "\t" << type_name << "  " << fl.name
								<< ( array_size > 1 ? ("["_str << array_size << "]") : (array_size == 0 ? "[]" : "") )
								<< ";  // offset: " << offset << ", align: " << align << "\n";
				}
			}
				
			// SSB with dynamic array
			if ( st.second.fields.Back().arraySize == 0 )
			{
				const auto&		arr			= st.second.fields.Back();
				const String	type_name	= EShaderVariable::IsStruct( arr.type ) ? arr.typeName : ser->ToString( arr.type );
				ASSERT( arr.arraySize == 0 and arr.stride > 0_b );

				str << "\n\t";
				str << ser->Comment( "Element  "_str << arr.name << "[];   offset: " << uint(arr.offset) << ", align: " << uint(arr.align) );
				str << ser->BeginStruct( "Element", uint(arr.stride) );
				str << ser->StructField( arr.name, type_name, 1, 0, uint(arr.align), uint(arr.stride) );
				str << ser->EndStruct();
			}

			if ( glsl_keep ) {
				glslSource << "};\n\n";
			}
			if ( ser_keep ) {
				str << ser->EndStruct() << '\n';
			}

		}
		return true;
	}

/*
=================================================
	_BindingsToLayout_Func
=================================================
*/
	struct BasePipeline::_BindingsToLayout_Func
	{
	// types
		using Uniform	= PipelineLayoutDescriptor::Uniform_t;
		

	// variables
		PipelineLayoutDescriptor::Builder&	layout;
		

	// methods
		explicit _BindingsToLayout_Func (PipelineLayoutDescriptor::Builder &layout) :
			layout(layout)
		{}

		void operator () (const TextureUniform &src) const
		{
			layout.AddTexture( src.name, src.imageType, src.format, src.location.index, src.location.uniqueIndex, src.shaderUsage );
		}

		void operator () (const ImageUniform &src) const
		{
			layout.AddImage( src.name, src.imageType, src.format, EGpuMemoryModel::HasWriteAccess( src.memoryModel ),
							 EGpuMemoryModel::HasReadAccess( src.memoryModel ), src.location.index, src.location.uniqueIndex, src.shaderUsage );
		}

		void operator () (const UniformBuffer &src) const
		{
			layout.AddUniformBuffer( src.name, src.size, src.location.index, src.location.uniqueIndex, src.shaderUsage );
		}

		void operator () (const StorageBuffer &src) const
		{
			layout.AddStorageBuffer( src.name, src.staticSize, src.arrayStride, EGpuMemoryModel::HasWriteAccess( src.memoryModel ),
									 EGpuMemoryModel::HasReadAccess( src.memoryModel ), src.location.index, src.location.uniqueIndex, src.shaderUsage );
		}

		// TODO:
		//	PipelineLayoutDescriptor::SamplerUniform
		//	PipelineLayoutDescriptor::PushConstants
		//	PipelineLayoutDescriptor::SubpassInput
	};
	
/*
=================================================
	_ConvertLayout
=================================================
*/
	bool BasePipeline::_ConvertLayout (StringCRef name, INOUT String &src, Ptr<ISerializer> ser) const
	{
		PipelineLayoutDescriptor::Builder	builder;

		_BindingsToLayout_Func		func( OUT builder );

		FOR( i, bindings.uniforms ) {
			bindings.uniforms[i].Apply( func );
		}

		src << ser->ToString( name, builder.Finish() ) << '\n';
		return true;
	}
	
/*
=================================================
	_TypeReplacer
=================================================
*/
	bool BasePipeline::_TypeReplacer (StringCRef typeName, INOUT ShaderCompiler::FieldTypeInfo &field) const
	{
		StructTypes::iterator	iter;

		if ( _structTypes.Find( typeName, OUT iter ) )
		{
			FOR( i, iter->second.fields )
			{
				const auto&		fl = iter->second.fields[i];

				if ( fl.name == field.name )
				{
					//field.index	= i;	// not needed, index will be replaced later
					field.type	= fl.type;
					return true;
				}
			}
		}
		return false;
	}
	
/*
=================================================
	_CalculateOffsets_Helper
=================================================
*/
	struct BasePipeline::_CalculateOffsets_Helper final : CalculateOffsets_Helper<BasePipeline::_StructField>
	{
	// variables
		StructTypes&	structTypes;


	// methods
		explicit _CalculateOffsets_Helper (StructTypes& structTypes) :
			structTypes(structTypes)
		{}


		bool ProcessStruct (INOUT BytesU &offset, INOUT Variable &var, EVariablePacking::type packing) override
		{
			if ( var.fields.Empty() )
			{
				StructTypes::iterator	iter;
				CHECK_ERR( structTypes.Find( var.typeName, OUT iter ) );
				
				Variable				st_type	= iter->second;
				BytesU					off		= offset;

				CHECK_ERR( Self::ProcessStruct( INOUT off, INOUT st_type, packing ) );

				var.offset	= st_type.offset;
				var.align	= st_type.align;
				var.stride	= st_type.stride;
				offset		= var.offset + var.stride * var.arraySize;
				return true;
			}

			CHECK_ERR( Self::ProcessStruct( INOUT offset, INOUT var, packing ) );
			return true;
		}
	};

/*
=================================================
	_CalculateOffsets
=================================================
*/
	bool BasePipeline::_CalculateOffsets (INOUT StructTypes &structTypes)
	{
		_CalculateOffsets_Helper	helper( structTypes );

		FOR( i, structTypes )
		{
			auto&	var = structTypes[i].second;
			BytesU	offset;
			CHECK_ERR( helper.ProcessStruct( INOUT offset, INOUT var, EVariablePacking::GetMaxPacking( var.packing ) ) );
		}
		return true;
	}
	
/*
=================================================
	_ReplaceTypes_Helper
=================================================
*/
	struct BasePipeline::_ReplaceTypes_Helper final : ReplaceTypes_Helper<BasePipeline::_StructField>
	{
	// variables
		StructTypes&	structTypes;
	
	// methods
		explicit _ReplaceTypes_Helper (StructTypes& structTypes) :
			structTypes(structTypes)
		{}

		void ReplaceVector (INOUT Array<Variable> &fields, INOUT usize &i, BytesU sizeOf) override
		{
			Self::ReplaceVector( INOUT fields, INOUT i, sizeOf );
		}
		
		void ReplaceMatrix (INOUT Array<Variable> &fields, INOUT usize &i, BytesU sizeOf) override
		{
			Self::ReplaceMatrix( INOUT fields, INOUT i, sizeOf );
		}
		
		void ReplaceArray (INOUT Array<Variable> &fields, INOUT usize &i, BytesU sizeOf) override
		{
			Self::ReplaceArray( INOUT fields, INOUT i, sizeOf );
		}
		
		bool ProcessType (StringCRef typeName) override
		{
			StructTypes::iterator	iter;
			CHECK_ERR( structTypes.Find( typeName, OUT iter ) );
			
			BytesU	offset, align;
			CHECK_ERR( RecursiveProcess( iter->second.typeName, INOUT iter->second.fields, OUT offset, OUT align ) );
			return true;
		}
	};
	
/*
=================================================
	_AddPaddingToStructs
----
	pass 2
=================================================
*/
	bool BasePipeline::_AddPaddingToStructs (INOUT StructTypes &structTypes)
	{
		_ReplaceTypes_Helper	helper( structTypes );
		EVariablePacking::bits	mask;	mask |= EVariablePacking::Varying;

		FOR( i, structTypes )
		{
			auto&	var = structTypes[i].second;
				
			if ( var.packing == mask )
				continue;

			BytesU	offset, align;
			CHECK_ERR( helper.RecursiveProcess( var.typeName, INOUT var.fields, OUT offset, OUT align ) );
		}
		return true;
	}
	
/*
=================================================
	_CompileShader
----
	pass 2
=================================================
*/
	bool BasePipeline::_CompileShader (const ShaderModule &shader, const ConverterConfig &convCfg, OUT CompiledShader &compiled) const
	{
		Array<StringCRef>		source;
		String					log;
		String					str;
		const String			version	=	_GetVersionGLSL();
		EShaderSrcFormat::type	src_fmt	=	shaderFormat == EShaderSrcFormat::GXSL_Vulkan ?	EShaderSrcFormat::GLSL_Vulkan : 
											shaderFormat == EShaderSrcFormat::GXSL ?		EShaderSrcFormat::GLSL :
											shaderFormat;

		// replace types
		BinaryArray		glsl_source;
		{
			ShaderCompiler::Config	cfg;
			cfg.filterInactive	= false;
			cfg.obfuscate		= false;
			cfg.skipExternals	= true;
			cfg.optimize		= false;
			cfg.source			= shaderFormat;
			cfg.target			= EShaderDstFormat::GLSL_Source;
			cfg.typeReplacer	= DelegateBuilder( this, &BasePipeline::_TypeReplacer );

			source	<< version
					<< _GetDefaultHeaderGLSL()
					<< _GetPerShaderHeaderGLSL( shader.type );

			FOR( i, shader._source ) {
				source << StringCRef(shader._source[i]);
			}

			if ( not ShaderCompiler::Instance()->Translate( shader.type, source, shader.entry, cfg, OUT log, OUT glsl_source ) )
			{
				CHECK_ERR( _OnCompilationFailed( shader.type, cfg.source, source, log ) );
			}
		}


		ShaderCompiler::Config	cfg;
		cfg.filterInactive	= true;
		cfg.obfuscate		= convCfg.obfuscate;
		cfg.optimize		= convCfg.optimizeSource;
		cfg.skipExternals	= false;

		// compile (optimize) for OpenGL
		if ( convCfg.target[ EShaderDstFormat::GLSL_Source ] )
		{
			source.Clear();
			str.Clear();

			str << _VaryingsToString( shader._io ) << '\n';
			_BindingsToString( shader.type, EShaderType::GLSL, false, OUT str );

			source	<< version
					<< _GetDefaultHeaderGLSL()
					<< _GetPerShaderHeaderGLSL( shader.type )
					<< convCfg._glslTypes
					<< str
					<< (const char*) glsl_source.ptr();
				
			cfg.source = src_fmt;
			cfg.target = EShaderDstFormat::GLSL_Source;
			
			if ( not ShaderCompiler::Instance()->Translate( shader.type, source, "main", cfg, OUT log, OUT compiled.glsl ) )
			{
				CHECK_ERR( _OnCompilationFailed( shader.type, cfg.source, source, log ) );
			}


			// compile to binary GLSL for OpenGL (machine dependent)
			if ( convCfg.target[ EShaderDstFormat::GLSL_Binary ] )
			{
				source.Clear();
				source << (const char*)compiled.glsl.ptr();
				
				ShaderCompiler::Config	cfg2;
				cfg2.source	= EShaderSrcFormat::GLSL;
				cfg2.target = EShaderDstFormat::GLSL_Binary;

				if ( not ShaderCompiler::Instance()->Translate( shader.type, source, "main", cfg, OUT log, OUT compiled.glslBinary ) )
				{
					CHECK_ERR( _OnCompilationFailed( shader.type, cfg.source, source, log ) );
				}
			}
		}


		// compile for Vulkan
		if ( convCfg.target[ EShaderDstFormat::SPIRV_Binary ] )
		{
			source.Clear();
			str.Clear();

			str << _VaryingsToString( shader._io ) << '\n';
			_BindingsToString( shader.type, EShaderType::SPIRV, false, OUT str );

			source	<< version
					<< _GetDefaultHeaderGLSL()
					<< _GetPerShaderHeaderGLSL( shader.type )
					<< convCfg._glslTypes
					<< str
					<< (const char*) glsl_source.ptr();
				
			cfg.source = src_fmt;
			cfg.target = EShaderDstFormat::SPIRV_Binary;
			
			if ( not ShaderCompiler::Instance()->Translate( shader.type, source, "main", cfg, OUT log, OUT compiled.spirv ) )
			{
				CHECK_ERR( _OnCompilationFailed( shader.type, cfg.source, source, log ) );
			}

			// disassamble spirv
			if ( convCfg.target[ EShaderDstFormat::SPIRV_Source ] )
			{
				cfg.target = EShaderDstFormat::SPIRV_Source;

				if ( not ShaderCompiler::Instance()->Translate( shader.type, source, "main", cfg, OUT log, OUT compiled.spirvSource ) )
				{
					CHECK_ERR( _OnCompilationFailed( shader.type, cfg.source, source, log ) );
				}
			}
		}


		// compile for C++
		if ( convCfg.target[ EShaderDstFormat::CPP_Module ] )
		{
			source.Clear();
			str.Clear();

			str << _VaryingsToString( shader._io ) << '\n';
			_BindingsToString( shader.type, EShaderType::Software, false, OUT str );
			
			source	<< version
					<< _GetDefaultHeaderGLSL()
					<< _GetPerShaderHeaderGLSL( shader.type )
					<< convCfg._glslTypes
					<< str
					<< (const char*) glsl_source.ptr();
			
			cfg.source = src_fmt;
			cfg.target = EShaderDstFormat::CPP_Module;
			
			if ( not ShaderCompiler::Instance()->Translate( shader.type, source, "main", cfg, OUT log, OUT compiled.cpp ) )
			{
				CHECK_ERR( _OnCompilationFailed( shader.type, cfg.source, source, log ) );
			}
		}


		// compile for CL
		if ( convCfg.target[ EShaderDstFormat::CL_Source ] )
		{
			source.Clear();
			str.Clear();

			str << _VaryingsToString( shader._io ) << '\n';
			_BindingsToString( shader.type, EShaderType::CL, false, OUT str );
			
			source	<< version
					<< _GetDefaultHeaderGLSL()
					<< _GetPerShaderHeaderGLSL( shader.type )
					<< convCfg._glslTypes						// TODO: convert to CL ?
					<< str
					<< (const char*) glsl_source.ptr();
			
			cfg.source = src_fmt;
			cfg.target = EShaderDstFormat::CL_Source;
			
			if ( not ShaderCompiler::Instance()->Translate( shader.type, source, "main", cfg, OUT log, OUT compiled.cl ) )
			{
				CHECK_ERR( _OnCompilationFailed( shader.type, cfg.source, source, log ) );
			}


			// compile CL source to binary
			if ( convCfg.target[ EShaderDstFormat::CL_Binary ] )
			{
				source.Clear();
				source << (const char*)compiled.cl.ptr();
				
				ShaderCompiler::Config	cfg2;
				cfg2.source	= EShaderSrcFormat::CL;
				cfg2.target = EShaderDstFormat::CL_Binary;

				if ( not ShaderCompiler::Instance()->Translate( shader.type, source, "main", cfg, OUT log, OUT compiled.clBinary ) )
				{
					CHECK_ERR( _OnCompilationFailed( shader.type, cfg.source, source, log ) );
				}
			}
		}

		// not supported
		ASSERT( not convCfg.target[ EShaderDstFormat::HLSL_Source ] );
		ASSERT( not convCfg.target[ EShaderDstFormat::HLSL_Binary ] );
		
		return true;
	}

}	// PipelineCompiler
