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

				if ( not skipLayouts and is_block )
				{
					str << "layout(offset=" << usize(fl.offset)
						<< ", align=" << usize(fl.align) << ") ";

					if ( fl.memoryModel != EShaderMemoryModel::Default )
						str << ToStringGLSL( fl.memoryModel ) << ' ';
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
					
				str << ';';
				
				if ( skipLayouts or not is_block )
				{
					str << "	// offset: " << usize(fl.offset) << ", align: " << usize(fl.align);
				}
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
			_pp(pp),							_str(str),
			_shaderType(shaderType),			_shaderApi(shaderApi),				
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
	_StructsToString
=================================================
*/
	bool BasePipeline::_StructsToString (const StructTypes &structTypes, OUT String &glslSource)
	{
		using StArray_t	= Array< StructTypes::const_pair_t const *>;

		HashSet<StringCRef>		defined;	defined.Reserve( structTypes.Count() );
		StArray_t				sorted;		sorted.Reserve( structTypes.Count() );
		StArray_t				pending;	pending.Resize( structTypes.Count() );
		
		FOR( i, structTypes ) {
			pending[i] = &structTypes[i];
		}

		// sort by dependencies of other types
		for (bool changed = true; changed;)
		{
			changed = false;

			FOR( i, pending )
			{
				bool	all_deps_defined = true;

				// check fields
				for (auto& fd : Range(pending[i]->second.fields))
				{
					if ( EShaderVariable::IsStruct( fd.type ) )
					{
						all_deps_defined &= defined.IsExist( fd.typeName );
					}
				}

				if ( all_deps_defined )
				{
					changed = true;
					sorted  << pending[i];
					defined << pending[i]->first; 

					pending.Erase( i );
					--i;
				}
			}
		}
		CHECK_ERR( pending.Empty() );

		// serialize
		FOR( i, sorted )
		{
			auto const&	st = *sorted[i];
			
			// keep struct, varying
			if ( st.second.type != EShaderVariable::Struct )
				continue;

			glslSource << "struct " << st.second.typeName << "\n{\n";

			// staticaly sized UB or SSB
			FOR( j, st.second.fields )
			{
				const auto&		fl			= st.second.fields[j];
				const String	type_name	= EShaderVariable::IsStruct( fl.type ) ? fl.typeName : ToStringGLSL( fl.type );
				const uint		array_size	= fl.arraySize;
				const uint		align		= (usize) fl.align;
				const uint		offset		= (usize) fl.offset;

				glslSource	<< "\t" << type_name << "  " << fl.name
							<< ( array_size > 1 ? ("["_str << array_size << "]") : (array_size == 0 ? "[]" : "") )
							<< ";  // offset: " << offset << ", align: " << align << "\n";
			}

			glslSource << "};\n\n";
		}

		return true;
	}
	
	
/*
=================================================
	_SerializeStructs
=================================================
*/
	bool BasePipeline::_SerializeStructs (const StructTypes &structTypes, Ptr<ISerializer> ser, OUT String &serialized)
	{
		using StArray_t	= Array< StructTypes::const_pair_t const *>;

		HashSet<StringCRef>		defined;	defined.Reserve( structTypes.Count() );
		StArray_t				sorted;		sorted.Reserve( structTypes.Count() );
		StArray_t				pending;	pending.Resize( structTypes.Count() );
		String &				str		= serialized;
		
		FOR( i, structTypes ) {
			pending[i] = &structTypes[i];
		}

		// sort by dependencies of other types
		for (bool changed = true; changed;)
		{
			changed = false;

			FOR( i, pending )
			{
				bool	all_deps_defined = true;

				// check fields
				for (auto& fd : Range(pending[i]->second.fields))
				{
					if ( EShaderVariable::IsStruct( fd.type ) )
					{
						all_deps_defined &= defined.IsExist( fd.typeName );
					}
				}

				if ( all_deps_defined )
				{
					changed = true;
					sorted  << pending[i];
					defined << pending[i]->first; 

					pending.Erase( i );
					--i;
				}
			}
		}
		CHECK_ERR( pending.Empty() );

		// serialize
		FOR( i, sorted )
		{
			auto const&	st = *sorted[i];

			// keep struct, buffer, vertex
			if ( st.second.packing[ EVariablePacking::Varying ] )
				continue;
			
			str << ser->Comment( "Packing: "_str << EVariablePacking::ToString(st.second.packing) )
				<< ser->BeginStruct( st.second.typeName, (uint)st.second.stride );

			// staticaly sized UB or SSB
			FOR( j, st.second.fields )
			{
				const auto&		fl			= st.second.fields[j];
				const String	type_name	= EShaderVariable::IsStruct( fl.type ) ? fl.typeName : ser->ToString( fl.type );
				const uint		array_size	= fl.arraySize;
				const uint		align		= (usize) fl.align;
				const uint		offset		= (usize) fl.offset;

				if ( array_size == 0 ) {
					ASSERT( j == st.second.fields.LastIndex() );
				} else {
					str << ser->StructField( fl.name, type_name, array_size, offset, align, uint(fl.stride) * fl.arraySize );
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

			if ( st.second.packing[ EVariablePacking::VertexAttrib ] ) {
				str << ser->StructCtorForInitializerList();
			}
			
			str << ser->EndStruct() << '\n';
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
			layout.AddImage( src.name, src.imageType, src.format, src.memoryModel, src.location.index, src.location.uniqueIndex, src.shaderUsage );
		}

		void operator () (const UniformBuffer &src) const
		{
			layout.AddUniformBuffer( src.name, src.size, src.location.index, src.location.uniqueIndex, src.shaderUsage );
		}

		void operator () (const StorageBuffer &src) const
		{
			layout.AddStorageBuffer( src.name, src.staticSize, src.arrayStride, src.memoryModel, src.location.index, src.location.uniqueIndex, src.shaderUsage );
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

			// skip varyings and vertices
			if ( var.packing == EVariablePacking::Varying or var.packing == EVariablePacking::VertexAttrib )
				continue;

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

		FOR( i, structTypes )
		{
			auto&	var = structTypes[i].second;
				
			if ( var.packing == EVariablePacking::Varying or var.packing == EVariablePacking::VertexAttrib )
				continue;

			BytesU	offset;
			BytesU	align	= var.align;

			CHECK_ERR( helper.RecursiveProcess( var.typeName, INOUT var.fields, OUT offset, INOUT align ) );

			var.align	= Max( var.align, align );
			var.stride	= Max( var.align, var.stride );
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
		EShaderSrcFormat::type	src_fmt	=	//shaderFormat == EShaderSrcFormat::GXSL_Vulkan ?	EShaderSrcFormat::GLSL_Vulkan : 
											//shaderFormat == EShaderSrcFormat::GXSL ?		EShaderSrcFormat::GLSL :
											shaderFormat;

		if ( convCfg.searchForSharedTypes and convCfg.addPaddingToStructs )
		{
			// replace types to aligned types and padding
			CHECK_ERR( _AddPaddingToStructs( _structTypes ) );

			// update offsets by packing
			CHECK_ERR( _CalculateOffsets( _structTypes ) );
		}

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
		
		String	glsl_types;
		CHECK_ERR( _StructsToString( _structTypes, OUT glsl_types ) );

		String	varyings;
		_VaryingsToString( shader._io, OUT varyings );


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

			str << varyings << '\n';
			_BindingsToString( shader.type, EShaderType::GLSL, false, OUT str );

			source	<< version
					<< _GetDefaultHeaderGLSL()
					<< _GetPerShaderHeaderGLSL( shader.type )
					<< glsl_types
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

				if ( not ShaderCompiler::Instance()->Translate( shader.type, source, "main", cfg2, OUT log, OUT compiled.glslBinary ) )
				{
					CHECK_ERR( _OnCompilationFailed( shader.type, cfg2.source, source, log ) );
				}
			}
		}


		// compile for Vulkan
		if ( convCfg.target[ EShaderDstFormat::SPIRV_Binary ] )
		{
			source.Clear();
			str.Clear();

			str << varyings << '\n';
			_BindingsToString( shader.type, EShaderType::SPIRV, false, OUT str );

			source	<< version
					<< _GetDefaultHeaderGLSL()
					<< _GetPerShaderHeaderGLSL( shader.type )
					<< glsl_types
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

				if ( not ShaderCompiler::Instance()->Translate( shader.type, source, "main", cfg, OUT log, OUT compiled.spirvAsm ) )
				{
					CHECK_ERR( _OnCompilationFailed( shader.type, cfg.source, source, log ) );
				}
			}
		}


		// compile for C++
		if ( shader.type == EShader::Compute and convCfg.target[ EShaderDstFormat::CPP_Module ] )
		{
			source.Clear();
			str.Clear();

			str << varyings << '\n';
			_BindingsToString( shader.type, EShaderType::Software, false, OUT str );
			
			source	<< version
					<< _GetDefaultHeaderGLSL()
					<< _GetPerShaderHeaderGLSL( shader.type )
					<< glsl_types
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
		if ( shader.type == EShader::Compute and convCfg.target[ EShaderDstFormat::CL_Source ] )
		{
			source.Clear();
			str.Clear();

			str << varyings << '\n';
			_BindingsToString( shader.type, EShaderType::CL, false, OUT str );
			
			source	<< version
					<< _GetDefaultHeaderGLSL()
					<< _GetPerShaderHeaderGLSL( shader.type )
					<< glsl_types
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

				if ( not ShaderCompiler::Instance()->Translate( shader.type, source, "main", cfg2, OUT log, OUT compiled.clAsm ) )
				{
					CHECK_ERR( _OnCompilationFailed( shader.type, cfg2.source, source, log ) );
				}
			}
		}

		// not supported
		ASSERT( not convCfg.target[ EShaderDstFormat::HLSL_Source ] );
		ASSERT( not convCfg.target[ EShaderDstFormat::HLSL_Binary ] );
		
		return true;
	}
	
/*
=================================================
	_ValidateShader
----
	pass 2
=================================================
*/
	bool BasePipeline::_ValidateShader (EShader::type shaderType, const CompiledShader &compiled)
	{
		if ( not compiled.glsl.Empty() ) {
			CHECK_ERR( ShaderCompiler::Instance()->Validate( EShaderDstFormat::GLSL_Source, shaderType, compiled.glsl ) );
		}
		
		if ( not compiled.glslBinary.Empty() ) {
			CHECK_ERR( ShaderCompiler::Instance()->Validate( EShaderDstFormat::GLSL_Binary, shaderType, compiled.glslBinary ) );
		}
		
		if ( not compiled.spirv.Empty() ) {
			CHECK_ERR( ShaderCompiler::Instance()->Validate( EShaderDstFormat::SPIRV_Binary, shaderType, compiled.spirv ) );
		}
		
		if ( not compiled.cl.Empty() ) {
			CHECK_ERR( ShaderCompiler::Instance()->Validate( EShaderDstFormat::CL_Source, shaderType, compiled.cl ) );
		}

		if ( not compiled.clAsm.Empty() ) {
			CHECK_ERR( ShaderCompiler::Instance()->Validate( EShaderDstFormat::CL_Binary, shaderType, compiled.clAsm ) );
		}
		return true;
	}

}	// PipelineCompiler
