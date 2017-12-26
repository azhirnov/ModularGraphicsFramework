// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Pipelines/BasePipeline.h"
#include "Engine/PipelineCompiler/Common/ToGLSL.h"

namespace PipelineCompiler
{

	static EVariableQualifier::bits		structTypeQualifierMask		= ~(EVariableQualifier::BuiltIn | EVariableQualifier::In |
																		EVariableQualifier::Out | EVariableQualifier::Uniform);

/*
=================================================
	_MergeTextures_Func
=================================================
*/
	struct BasePipeline::_MergeTextures_Func
	{
	// variables
		Array<TextureUniform>&	textures;
		
	// methods
		_MergeTextures_Func (INOUT Array<TextureUniform> &textures) : textures(textures)
		{}

		void operator () (TextureUniform &tex) const
		{
			FOR( i, textures )
			{
				const auto&	other = textures[i];

				if ( other.name == tex.name )
				{
					CHECK( other.format == tex.format );
					CHECK( other.imageType == tex.imageType );
					//CHECK( other.defaultSampler == tex.defaultSampler );	// TODO
					
					tex.shaderUsage |= other.shaderUsage;

					textures.Erase( i );
					--i;
				}
			}
		}

		template <typename T>
		void operator () (const T &) const
		{}
	};
	
/*
=================================================
	_AddBindings
=================================================
*/
	void BasePipeline::_AddBindings (INOUT Array<TextureUniform> &textures)
	{
		_MergeTextures_Func	func( INOUT textures );

		FOR( i, bindings.uniforms ) {
			bindings.uniforms[i].Apply( func );
		}

		FOR( i, textures ) {
			bindings.uniforms.PushBack( Bindings::_Uniform{ textures[i] } );
		}
	}
	
/*
=================================================
	_MergeImages_Func
=================================================
*/
	struct BasePipeline::_MergeImages_Func
	{
	// variables
		Array<ImageUniform>&	images;
		
	// methods
		_MergeImages_Func (INOUT Array<ImageUniform> &images) : images(images)
		{}

		void operator () (ImageUniform &img) const
		{
			FOR( i, images )
			{
				const auto&	other = images[i];

				if ( other.name == img.name )
				{
					CHECK( other.format == img.format );
					CHECK( other.imageType == img.imageType );
					CHECK( other.memoryModel == img.memoryModel );
					
					img.shaderUsage |= other.shaderUsage;

					images.Erase( i );
					--i;
				}
			}
		}

		template <typename T>
		void operator () (const T &) const
		{}
	};
	
/*
=================================================
	_AddBindings
=================================================
*/
	void BasePipeline::_AddBindings (INOUT Array<ImageUniform> &images)
	{
		_MergeImages_Func	func( INOUT images );

		FOR( i, bindings.uniforms ) {
			bindings.uniforms[i].Apply( func );
		}

		FOR( i, images ) {
			bindings.uniforms.PushBack( Bindings::_Uniform{ images[i] } );
		}
	}
	
/*
=================================================
	_MergeUniformBuffers_Func
=================================================
*/
	struct BasePipeline::_MergeUniformBuffers_Func
	{
	// variables
		Array<UniformBuffer>&	buffers;
		
	// methods
		_MergeUniformBuffers_Func (INOUT Array<UniformBuffer> &buffers) : buffers(buffers)
		{}
		
		void operator () (UniformBuffer &buf) const
		{
			FOR( i, buffers )
			{
				const auto&	other = buffers[i];

				if ( other.name == buf.name )
				{
					CHECK( other.typeName == buf.typeName );

					buf.shaderUsage |= other.shaderUsage;

					buffers.Erase( i );
					--i;
				}
			}
		}

		template <typename T>
		void operator () (const T &) const
		{}
	};
	
/*
=================================================
	_AddBindings
=================================================
*/
	void BasePipeline::_AddBindings (INOUT Array<UniformBuffer> &buffers)
	{
		_MergeUniformBuffers_Func	func( INOUT buffers );

		FOR( i, bindings.uniforms ) {
			bindings.uniforms[i].Apply( func );
		}

		FOR( i, buffers ) {
			bindings.uniforms.PushBack( Bindings::_Uniform{ buffers[i] } );
		}
	}
	
/*
=================================================
	_MergeStorageBuffers_Func
=================================================
*/
	struct BasePipeline::_MergeStorageBuffers_Func
	{
	// variables
		Array<StorageBuffer>&	buffers;
		
	// methods
		_MergeStorageBuffers_Func (INOUT Array<StorageBuffer> &buffers) : buffers(buffers)
		{}
		
		void operator () (StorageBuffer &buf) const
		{
			FOR( i, buffers )
			{
				const auto&	other = buffers[i];

				if ( other.name == buf.name )
				{
					CHECK( other.typeName == buf.typeName );
					CHECK( other.memoryModel == buf.memoryModel );
					
					buf.shaderUsage |= other.shaderUsage;

					buffers.Erase( i );
					--i;
				}
			}
		}

		template <typename T>
		void operator () (const T &) const
		{}
	};
	
/*
=================================================
	_AddBindings
=================================================
*/
	void BasePipeline::_AddBindings (INOUT Array<StorageBuffer> &buffers)
	{
		_MergeStorageBuffers_Func	func( INOUT buffers );

		FOR( i, bindings.uniforms ) {
			bindings.uniforms[i].Apply( func );
		}

		FOR( i, buffers ) {
			bindings.uniforms.PushBack( Bindings::_Uniform{ buffers[i] } );
		}
	}
	
/*
=================================================
	_ExtractTextures
=================================================
*/
	bool BasePipeline::_ExtractTextures (const DeserializedShader &shader, OUT Array<TextureUniform> &result)
	{
		result.Reserve( shader.Textures().Count() );

		FOR( i, shader.Textures() )
		{
			const auto&		src	= shader.Textures()[i];
			TextureUniform	dst;

			dst.name		= src.name;
			dst.format		= src.format;
			dst.imageType	= src.imageType;
			dst.shaderUsage	|= shader.Type();

			if ( src.isShadow ) {
				// default shadow sampler
				dst.defaultSampler = SamplerDescriptor( EAddressMode::ClampToEdge, EFilter::MinMagMipLinear,
														Uninitialized, ECompareFunc::LEqual );
			}

			result.PushBack( RVREF(dst) );
		}
		return true;
	}

/*
=================================================
	_ExtractImages
=================================================
*/
	bool BasePipeline::_ExtractImages (const DeserializedShader &shader, OUT Array<ImageUniform> &result)
	{
		result.Reserve( shader.Images().Count() );

		FOR( i, shader.Images() )
		{
			const auto&		src = shader.Images()[i];
			ImageUniform	dst;

			dst.name		= src.name;
			dst.memoryModel	= src.memoryModel;
			dst.format		= src.format;
			dst.imageType	= src.imageType;
			dst.shaderUsage	|= shader.Type();

			result.PushBack( RVREF(dst) );
		}
		return true;
	}

/*
=================================================
	_ExtractUniformBuffers
=================================================
*/
	bool BasePipeline::_ExtractUniformBuffers (const DeserializedShader &shader, OUT Array<UniformBuffer> &result)
	{
		result.Reserve( shader.UniformBuffers().Count() );

		FOR( i, shader.UniformBuffers() )
		{
			const auto&		src = shader.UniformBuffers()[i];
			UniformBuffer	dst;

			dst.name		= src.name;
			dst.typeName	= src.typeName;
			dst.shaderUsage	|= shader.Type();

			result.PushBack( RVREF(dst) );
		}
		return true;
	}

/*
=================================================
	_ExtractStorageBuffers
=================================================
*/
	bool BasePipeline::_ExtractStorageBuffers (const DeserializedShader &shader, OUT Array<StorageBuffer> &result)
	{
		result.Reserve( shader.StorageBuffers().Count() );

		FOR( i, shader.StorageBuffers() )
		{
			const auto&		src = shader.StorageBuffers()[i];
			StorageBuffer	dst;

			dst.name		= src.name;
			dst.typeName	= src.typeName;
			dst.memoryModel	= src.memoryModel;
			dst.shaderUsage	|= shader.Type();

			result.PushBack( RVREF(dst) );
		}
		return true;
	}

/*
=================================================
	_RecursiveProcessBuffer
=================================================
*/
	bool BasePipeline::_RecursiveProcessBuffer (const Array<DeserializedShader::BufferVariable> &fields, EVariablePacking::type packing,
												OUT _StructField &curr, INOUT Array<_StructField> &result)
	{
		BytesU	min_offset	= ~0_b;

		FOR( i, fields ) {
			min_offset = Min( fields[i].offset, min_offset );
		}

		FOR( i, fields )
		{
			const auto&		var = fields[i];
			_StructField	info;
			
			info.name			= var.name;
			info.type			= var.type;
			info.typeName		= var.typeName;
			info.arraySize		= var.arraySize;
			info.offset			= var.offset - min_offset;
			info.align			= var.align;
			info.stride			= var.stride;
			info.qualifier		= var.qualifier & structTypeQualifierMask;
			info.memoryModel	= var.memoryModel;
			//info.packing		|= packing;

			if ( EShaderVariable::IsStruct( var.type ) )
			{
				_StructField	dst;
				dst.typeName	= var.typeName;
				dst.type		= var.type;
				dst.qualifier	= var.qualifier & structTypeQualifierMask;
				dst.memoryModel	= var.memoryModel;
				dst.packing		|= packing;
				// ignore: name, align, offset, stride

				CHECK_ERR( _RecursiveProcessBuffer( var.fields, packing, OUT dst, INOUT result ) );
				result.PushBack( RVREF(dst) );
			}

			CHECK_ERR( var.offset != BytesU(~0u) );		// 'offset' and 'align' must be correct

			curr.fields.PushBack( RVREF(info) );
		}
		return true;
	}
	
/*
=================================================
	_RecursiveProcessVarying
=================================================
*/
	bool BasePipeline::_RecursiveProcessVarying (const Array<DeserializedShader::IOVariable> &fields, StringCRef parent,
												 INOUT Array<Varying> &input, INOUT Array<Varying> &output)
	{
		FOR( i, fields )
		{
			const auto&		src = fields[i];
			Varying			dst;

			dst.name		= src.name;
			dst.typeName	= src.typeName;
			dst.type		= src.type;
			dst.precision	= src.precision;
			dst.location	= src.location;
			dst.arraySize	= src.arraySize;
			dst.qualifier	= src.qualifier;
			dst.qualifier.Reset( EVariableQualifier::BuiltIn );
						 //.Reset( EVariableQualifier::BindlessSampler )
						 //.Reset( EVariableQualifier::BindlessImage );

			if ( src.qualifier[ EVariableQualifier::BuiltIn ] )
			{}	// skip
			else
			if ( src.qualifier[ EVariableQualifier::In ] )
				input.PushBack( RVREF(dst) );
			else
			if ( src.qualifier[ EVariableQualifier::Out ] )
				output.PushBack( RVREF(dst) );
			else
				RETURN_ERR( "unknown varying type!" );
		}
		return true;
	}

/*
=================================================
	_ExtractVaryings
=================================================
*/
	bool BasePipeline::_ExtractVaryings (const DeserializedShader &shader, OUT Array<Varying> &input, OUT Array<Varying> &output)
	{
		input.Clear();
		output.Clear();

		CHECK_ERR( _RecursiveProcessVarying( shader.Variables(), "", INOUT input, INOUT output ) );
		return true;
	}
	
/*
=================================================
	_ExtractConstants
=================================================
*/
	bool BasePipeline::_ExtractConstants (const DeserializedShader &shader, OUT Array<Constant> &result)
	{
		result.Clear();
		result.Reserve( shader.Constants().Count() );

		FOR( i, shader.Constants() )
		{
			const auto&		src	= shader.Constants()[i];
			Constant		dst;

			dst.name		= src.name;
			dst.typeName	= src.typeName;
			dst.type		= src.type;
			dst.arraySize	= src.arraySize;
			dst.location	= UMax;	// unused
			dst.precision	= src.precision;
			dst.qualifier	= src.qualifier;
			dst.values		= src.values;

			result.PushBack( RVREF(dst) );
		}
		return true;
	}
	
/*
=================================================
	_ExtractAttribs
=================================================
*/
	bool BasePipeline::_ExtractAttribs (const Array<Varying> &input, OUT VertexAttribs &attribs)
	{
		const VertexAttribs	old = attribs;

		attribs.Clear();

		FOR( i, input )
		{
			auto	iter = old.Get( input[i].name );

			attribs.Add( input[i].name, EShaderVariable::ToAttrib( input[i].type ), iter ? iter->second.buffer : "" );
		}
		return true;
	}
	
/*
=================================================
	_ExtractFragOutput
=================================================
*/
	bool BasePipeline::_ExtractFragOutput (const Array<Varying> &output, OUT FragmentOutputState &fragOutput)
	{
		fragOutput.Clear();

		FOR( i, output )
		{
			fragOutput.Add( output[i].name, EShaderVariable::ToFragOutput( output[i].type ) );
		}
		return true;
	}
	
/*
=================================================
	_RecursiveProcessVarying
=================================================
*/
	bool BasePipeline::_RecursiveProcessVarying (const Array<DeserializedShader::IOVariable> &fields,
												 INOUT _StructField *curr, INOUT Array<_StructField> &result)
	{
		uint	min_location	= 0;
		BytesU	align			= 16_b;

		FOR( i, fields ) {
			min_location = Min( fields[i].location, min_location );
		}

		FOR( i, fields )
		{
			const auto&		var = fields[i];
			_StructField	info;
			
			// temp
			ASSERT( not var.qualifier[ EVariableQualifier::BuiltIn ] );
			ASSERT( not var.name.StartsWith("gl_") );
			ASSERT( not var.typeName.StartsWith("gl_") );

			info.name		= var.name;
			info.type		= var.type;
			info.typeName	= var.typeName;
			info.arraySize	= var.arraySize;
			info.offset		= (var.location - min_location) * align;
			info.align		= align;
			info.stride		= align;
			info.qualifier	= var.qualifier & structTypeQualifierMask;
			info.packing	|= EVariablePacking::Varying;

			if ( EShaderVariable::IsStruct( var.type ) )
			{
				_StructField	dst;
				CHECK_ERR( _RecursiveProcessVarying( var.fields, INOUT &dst, INOUT result ) );
				result.PushBack( RVREF(dst) );
			}

			CHECK_ERR( var.location != BytesU(~0u) );		// 'offset' and 'align' must be correct

			if ( curr ) {
				curr->fields.PushBack( RVREF(info) );
			}
		}
		return true;
	}

/*
=================================================
	_ExtractTypes
=================================================
*/
	bool BasePipeline::_ExtractTypes (const DeserializedShader &shader, OUT Array<_StructField> &result)
	{
		FOR( i, shader.UniformBuffers() )
		{
			const auto&		src = shader.UniformBuffers()[i];
			_StructField	dst;
			
			dst.type		= EShaderVariable::UniformBlock;
			dst.typeName	= src.typeName;
			dst.packing		|= src.packing;

			CHECK_ERR( _RecursiveProcessBuffer( src.fields, src.packing, OUT dst, INOUT result ) );
			result.PushBack( RVREF(dst) );
		}

		FOR( i, shader.StorageBuffers() )
		{
			const auto&		src = shader.StorageBuffers()[i];
			_StructField	dst;

			dst.type		= EShaderVariable::StorageBlock;
			dst.typeName	= src.typeName;
			dst.packing		|= src.packing;

			CHECK_ERR( _RecursiveProcessBuffer( src.fields, src.packing, OUT dst, INOUT result ) );
			result.PushBack( RVREF(dst) );
		}

		FOR( i, shader.Variables() )
		{
			const auto&		src = shader.Variables()[i];
			_StructField	dst;

			if ( src.qualifier[ EVariableQualifier::BuiltIn ] or not EShaderVariable::IsStruct( src.type ) )
				continue;
			
			// temp
			ASSERT( not src.name.StartsWith("gl_") );
			ASSERT( not src.typeName.StartsWith("gl_") );

			dst.type		= EShaderVariable::Struct;
			dst.typeName	= src.typeName;
			dst.packing		|= EVariablePacking::Varying;

			CHECK_ERR( _RecursiveProcessVarying( src.fields, INOUT &dst, INOUT result ) );
			result.PushBack( RVREF(dst) );
		}

		// TODO: is local struct types needed outside of shader?
		/*FOR( i, shader.Constants() )
		{
			const auto&		src = shader.Constants()[i];
			_StructField	dst;

			if ( not EShaderVariable::IsStruct( src.type ) )
				continue;

			TODO( "" );
		}*/
		return true;
	}

/*
=================================================
	_MergeStructTypes
=================================================
*/
	bool BasePipeline::_MergeStructTypes (const Array<_StructField> &newTypes, INOUT StructTypes &currTypes)
	{
		FOR( i, newTypes )
		{
			const auto&		st = newTypes[i];

			CHECK_ERR( _AddStructType( st, INOUT currTypes ) );
		}
		return true;
	}
	
/*
=================================================
	_UpdateBindingIndices_Func
=================================================
*/
	struct BasePipeline::_UpdateBindingIndices_Func
	{
	// variables
		uint		_textureCounter			= 0;
		uint		_imageCounter			= 0;
		uint		_uniformBlockCounter	= 0;
		uint		_storageBlockCounter	= 0;
		uint		_index					= 0;


	// methods
		_UpdateBindingIndices_Func ()
		{}


		void operator () (TextureUniform &src)
		{
			src.location.uniqueIndex	= _index++;
			src.location.index			= _textureCounter++;
		}


		void operator () (ImageUniform &src)
		{
			src.location.uniqueIndex	= _index++;
			src.location.index			= _imageCounter++;
		}


		void operator () (UniformBuffer &src)
		{
			src.location.uniqueIndex	= _index++;
			src.location.index			= _uniformBlockCounter++;
		}


		void operator () (StorageBuffer &src)
		{
			src.location.uniqueIndex	= _index++;
			src.location.index			= _storageBlockCounter++;
		}
	};
	
/*
=================================================
	_UpdateBindings
=================================================
*/
	bool BasePipeline::_UpdateBindings ()
	{
		_UpdateBindingIndices_Func	func;

		FOR( i, bindings.uniforms ) {
			bindings.uniforms[i].Apply( func );
		}
		return true;
	}
	
/*
=================================================
	_UpdateBufferSizes_Func
=================================================
*/
	struct BasePipeline::_UpdateBufferSizes_Func
	{
	// variables
		StructTypes const&		structTypes;


	// methods
		explicit
		_UpdateBufferSizes_Func (StructTypes const& structTypes) :
			structTypes( structTypes )
		{}

		template <typename T>
		void operator () (T &)
		{}


		void operator () (UniformBuffer &src)
		{
			StructTypes::const_iterator	iter;

			if ( structTypes.Find( src.typeName, OUT iter ) ) {
				src.size = iter->second.stride;
			}
		}


		void operator () (StorageBuffer &src)
		{
			StructTypes::const_iterator	iter;

			if ( structTypes.Find( src.typeName, OUT iter ) )
			{
				src.staticSize  = iter->second.stride;
				src.arrayStride = 0_b;

				if ( iter->second.fields.Back().arraySize == 0 )
					src.arrayStride = iter->second.fields.Back().stride;
			}
		}
	};
	
/*
=================================================
	_UpdateBufferSizes
=================================================
*/
	bool BasePipeline::_UpdateBufferSizes ()
	{
		_UpdateBufferSizes_Func	func( _structTypes );

		FOR( i, bindings.uniforms ) {
			bindings.uniforms[i].Apply( func );
		}
		return true;
	}

/*
=================================================
	_UpdateDescriptorSets_Func
=================================================
*/
	struct BasePipeline::_UpdateDescriptorSets_Func
	{
	// variables
		Map<uint, uint>		_descriptors;
		bool				changeDescriptors	= false;


	// methods
		_UpdateDescriptorSets_Func ()
		{}

		void StartPass2 ()
		{
			changeDescriptors = true;

			FOR( i, _descriptors ) {
				_descriptors[i].second = i;
			}
		}


		template <typename T>
		void operator () (T &src)
		{
			if ( changeDescriptors )
			{
				Map<uint, uint>::const_iterator	iter;

				if ( _descriptors.Find( src.location.descriptorSet, OUT iter ) )
					src.location.descriptorSet = iter->second;
				else
					src.location.descriptorSet = _descriptors.Back().second;
			}
			else
				_descriptors.Add( src.location.descriptorSet, src.location.descriptorSet );
		}
	};
	
/*
=================================================
	_UpdateDescriptorSets
=================================================
*/
	bool BasePipeline::_UpdateDescriptorSets ()
	{
		_UpdateDescriptorSets_Func	func;

		FOR( i, bindings.uniforms ) {
			bindings.uniforms[i].Apply( func );
		}

		func.StartPass2();
		
		FOR( i, bindings.uniforms ) {
			bindings.uniforms[i].Apply( func );
		}
		return true;
	}
	
/*
=================================================
	_DisasembleShader
=================================================
*/
	bool BasePipeline::_DisasembleShader (const ConverterConfig &convCfg, INOUT ShaderModule &shader, OUT ShaderDisasembly &disasm)
	{
		String				str;
		String				log;
		BinaryArray			optimized;
		DeserializedShader	deserialized;
		Array<StringCRef>	source;
		String				shader_entry	= shader.entry;
		const String		version			= _GetVersionGLSL();
		
		CHECK_ERR(	shaderFormat == EShaderSrcFormat::GXSL			or
					shaderFormat == EShaderSrcFormat::GXSL_Vulkan	or
					shaderFormat == EShaderSrcFormat::GLSL			or
					shaderFormat == EShaderSrcFormat::GLSL_Vulkan );

		shader._sourceOnly.Clear();

		if ( shader.type == EShader::Vertex )
		{
			str << ToStringGLSL( attribs );
		}

		if ( shader.type == EShader::Fragment )
		{
			str << ToStringGLSL( fragOutput );
		}

		// build source
		{
			CHECK_ERR( shader._constants.Empty() );

			str << _VaryingsToString( shader._io ) << '\n';
			_BindingsToString( shader.type, EShaderType::None, OUT str );

			source	<< version
					<< _GetDefaultHeaderGLSL()
					<< _GetPerShaderHeaderGLSL( shader.type )
					<< str;

			FOR( i, shader._source ) {
				source << StringCRef(shader._source[i]);
			}
		}

		// optimize before deserializing
		if ( convCfg.optimizeSource )
		{
			ShaderCompiler::Config	cfg;
			cfg.filterInactive	= true;
			cfg.obfuscate		= false;
			cfg.skipExternals	= false;
			cfg.optimize		= true;
			cfg.source			= shaderFormat;
			cfg.target			= EShaderDstFormat::GLSL_Source;

			if ( not ShaderCompiler::Instance()->Translate( shader.type, source, shader_entry, cfg, OUT log, OUT optimized ) )
			{
				CHECK_ERR( _OnCompilationFailed( shader.type, cfg.source, source, log ) );
			}

			StringCRef	temp = (const char*) optimized.ptr();
			
			source.Clear();
			source		 << temp;
			shader_entry = "main";

			usize	pos;
			CHECK_ERR( temp.FindIC( "main(", OUT pos ) );
			StringParser::ToBeginOfLine( temp, INOUT pos );

			shader._sourceOnly = BinArrayCRef::From( temp.SubString( pos ) );
		}
		else
		// GXSL to GLSL
		if ( shaderFormat == EShaderSrcFormat::GXSL or
			 shaderFormat == EShaderSrcFormat::GLSL or
			 shaderFormat == EShaderSrcFormat::GXSL_Vulkan or
			 shaderFormat == EShaderSrcFormat::GLSL_Vulkan )
		{
			ShaderCompiler::Config	cfg;
			cfg.filterInactive	= false;
			cfg.obfuscate		= false;
			cfg.skipExternals	= true;
			cfg.optimize		= false;
			cfg.source			= shaderFormat;
			cfg.target			= EShaderDstFormat::GLSL_Source;

			if ( not ShaderCompiler::Instance()->Translate( shader.type, source, shader.entry, cfg, OUT log, OUT shader._sourceOnly ) )
			{
				CHECK_ERR( _OnCompilationFailed( shader.type, cfg.source, source, log ) );
			}
		}
		else
		{
			// remove externals
			TODO( "not supported yet" );
		}

		// deserialize
		{
			CHECK_ERR( ShaderCompiler::Instance()->Deserialize( shaderFormat, shader.type, source, shader_entry, OUT log, OUT deserialized ) );
			CHECK_ERR( deserialized.CalculateOffsets() );
			CHECK_ERR( deserialized.CalculateLocations() );
			//LOG( deserialized.ToString().cstr(), ELog::Debug );

			CHECK_ERR( _ExtractTextures( deserialized, OUT disasm.textures ) );
			CHECK_ERR( _ExtractImages( deserialized, OUT disasm.images ) );
			CHECK_ERR( _ExtractUniformBuffers( deserialized, OUT disasm.uniformBuffers ) );
			CHECK_ERR( _ExtractStorageBuffers( deserialized, OUT disasm.storageBuffers ) );
			CHECK_ERR( _ExtractTypes( deserialized, OUT disasm.structTypes ) );
			CHECK_ERR( _ExtractVaryings( deserialized, OUT disasm.input, OUT disasm.output ) );
			CHECK_ERR( _ExtractConstants( deserialized, OUT disasm.constants ) );
			
			if ( shader.type == EShader::Vertex )
				CHECK_ERR( _ExtractAttribs( disasm.input, OUT attribs ) );
			
			if ( shader.type == EShader::Fragment )
				CHECK_ERR( _ExtractFragOutput( disasm.output, OUT fragOutput ) );

			if ( shader.type == EShader::TessControl )
				patchControlPoints = deserialized.TessControlShader().patchSize;

			if ( shader.type == EShader::Compute )
				localGroupSize = deserialized.ComputeShader().localSize;
		}

		// add bindings
		{
			_AddBindings( INOUT disasm.textures );
			_AddBindings( INOUT disasm.images );
			_AddBindings( INOUT disasm.uniformBuffers );
			_AddBindings( INOUT disasm.storageBuffers );
		}

		// add varyings
		{
			shader._io.Clear();

			FOR( i, disasm.input )
			{
				if ( disasm.input[i].qualifier[ EVariableQualifier::BuiltIn ] )
					continue;

				shader._io << disasm.input[i];
			}
		
			FOR( i, disasm.output )
			{
				if ( disasm.output[i].qualifier[ EVariableQualifier::BuiltIn ] )
					continue;

				shader._io << disasm.output[i];
			}
		}

		shader._constants = disasm.constants;
		return true;
	}


}	// PipelineCompiler
