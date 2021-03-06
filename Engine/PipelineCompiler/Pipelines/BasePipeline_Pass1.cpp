// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
	_AddBindings (TextureUniform)
=================================================
*/
	void BasePipeline::_AddBindings (INOUT Array<TextureUniform> &textures)
	{
		_MergeTextures_Func	func( INOUT textures );

		for (auto& un : bindings.uniforms) {
			un.Accept( func );
		}
		
		for (auto& tex : textures) {
			bindings.uniforms.PushBack( Bindings::_Uniform{ tex } );
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
	_AddBindings (ImageUniform)
=================================================
*/
	void BasePipeline::_AddBindings (INOUT Array<ImageUniform> &images)
	{
		_MergeImages_Func	func( INOUT images );

		for (auto& un : bindings.uniforms) {
			un.Accept( func );
		}

		for (auto& img : images) {
			bindings.uniforms.PushBack( Bindings::_Uniform{ img } );
		}
	}
	
/*
=================================================
	_MergeSubpasses_Func
=================================================
*/
	struct BasePipeline::_MergeSubpasses_Func
	{
	// variables
		Array<SubpassInput>&	images;
		
	// methods
		_MergeSubpasses_Func (INOUT Array<SubpassInput> &images) : images(images)
		{}

		void operator () (SubpassInput &spi) const
		{
			FOR( i, images )
			{
				const auto&	other = images[i];

				if ( other.name == spi.name )
				{
					CHECK( other.attachmentIndex == spi.attachmentIndex );
					CHECK( other.isMultisample == spi.isMultisample );
					
					spi.shaderUsage |= other.shaderUsage;

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
	_AddBindings (SubpassInput)
=================================================
*/
	void BasePipeline::_AddBindings (INOUT Array<SubpassInput> &subpasses)
	{
		_MergeSubpasses_Func	func( INOUT subpasses );

		for (auto& un : bindings.uniforms) {
			un.Accept( func );
		}

		for (auto& subpass : subpasses) {
			bindings.uniforms.PushBack( Bindings::_Uniform{ subpass } );
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
	_AddBindings (UniformBuffer)
=================================================
*/
	void BasePipeline::_AddBindings (INOUT Array<UniformBuffer> &buffers)
	{
		_MergeUniformBuffers_Func	func( INOUT buffers );

		for (auto& un : bindings.uniforms) {
			un.Accept( func );
		}

		for (auto& buf : buffers) {
			bindings.uniforms.PushBack( Bindings::_Uniform{ buf } );
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
	_AddBindings (StorageBuffer)
=================================================
*/
	void BasePipeline::_AddBindings (INOUT Array<StorageBuffer> &buffers)
	{
		_MergeStorageBuffers_Func	func( INOUT buffers );

		for (auto& un : bindings.uniforms) {
			un.Accept( func );
		}

		for (auto& buf : buffers) {
			bindings.uniforms.PushBack( Bindings::_Uniform{ buf } );
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

		for (const auto& src : shader.Textures())
		{
			TextureUniform	dst;

			dst.name		= src.name;
			dst.format		= src.format;
			dst.imageType	= src.imageType;
			dst.shaderUsage	|= shader.Type();

			if ( src.isShadow ) {
				// default shadow sampler
				dst.defaultSampler = SamplerDescription( EAddressMode::ClampToEdge, EFilter::MinMagMipLinear,
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

		for (const auto& src : shader.Images())
		{
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
	_ExtractSubpasses
=================================================
*/
	bool BasePipeline::_ExtractSubpasses (const DeserializedShader &shader, OUT Array<SubpassInput> &result)
	{
		result.Reserve( shader.SubpassInputs().Count() );

		for (const auto& src : shader.SubpassInputs())
		{
			SubpassInput	dst;

			dst.name			= src.name;
			dst.attachmentIndex	= src.attachmentIndex;
			dst.isMultisample	= src.isMultisample;
			dst.shaderUsage		|= shader.Type();

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

		for (const auto& src : shader.UniformBuffers())
		{
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

		for (const auto& src : shader.StorageBuffers())
		{
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

		for (auto& fld : fields) {
			min_offset = Min( fld.offset, min_offset );
		}

		for (auto& var : fields)
		{
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

			CHECK_ERR( var.offset != ~0_b );		// 'offset' and 'align' must be correct

			curr.fields.PushBack( RVREF(info) );
		}
		return true;
	}
	
/*
=================================================
	_RecursiveProcessVarying
=================================================
*/
	bool BasePipeline::_RecursiveProcessVarying (const Array<DeserializedShader::IOVariable> &varyings, StringCRef,
												 INOUT Array<Varying> &input, INOUT Array<Varying> &output)
	{
		for (auto& src : varyings)
		{
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

			for (auto& src_fld : src.fields)
			{
				_VaryingField	dst_fld;
				
				dst_fld.name		= src_fld.name;
				dst_fld.typeName	= src_fld.typeName;
				dst_fld.type		= src_fld.type;
				dst_fld.precision	= src_fld.precision;
				dst_fld.location	= src_fld.location;
				dst_fld.arraySize	= src_fld.arraySize;
				dst_fld.qualifier	= src_fld.qualifier;
				dst_fld.qualifier.Reset( EVariableQualifier::BuiltIn );

				CHECK_ERR( src_fld.fields.Empty() );	// block in block is not supported

				dst.fields.PushBack( RVREF(dst_fld) );
			}

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
	_ExtractAttribs
=================================================
*/
	bool BasePipeline::_ExtractAttribs (const Array<Varying> &input, OUT VertexAttribs &attribs)
	{
		const VertexAttribs	old = attribs;

		attribs.Clear();

		for (auto& in : input)
		{
			auto	iter = old.Get( in.name );

			attribs.Add( in.name, EShaderVariable::ToAttrib( in.type ), iter ? iter->second.buffer : "" );
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

		for (auto& out : output)
		{
			fragOutput.Add( out.name, EShaderVariable::ToFragOutput( out.type ) );
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

		for (auto& fld : fields) {
			min_location = Min( fld.location, min_location );
		}

		for (auto& var : fields)
		{
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

			CHECK_ERR( var.location != UMax );		// 'offset' and 'align' must be correct

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
		for (auto& src : shader.UniformBuffers())
		{
			_StructField	dst;
			
			dst.type		= EShaderVariable::UniformBlock;
			dst.typeName	= src.typeName;
			dst.packing		|= src.packing;

			CHECK_ERR( _RecursiveProcessBuffer( src.fields, src.packing, OUT dst, INOUT result ) );
			result.PushBack( RVREF(dst) );
		}

		for (auto& src : shader.StorageBuffers())
		{
			_StructField	dst;

			dst.type		= EShaderVariable::StorageBlock;
			dst.typeName	= src.typeName;
			dst.packing		|= src.packing;

			CHECK_ERR( _RecursiveProcessBuffer( src.fields, src.packing, OUT dst, INOUT result ) );
			result.PushBack( RVREF(dst) );
		}

		for (auto& src : shader.Variables())
		{
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
		for (auto& st : newTypes)
		{
			CHECK_ERR( _AddStructType( st, INOUT currTypes ) );
		}
		return true;
	}
	
/*
=================================================
	_AttribsToStructTypes
=================================================
*/
	bool BasePipeline::_AttribsToStructTypes (StringCRef name, const VertexAttribs &attribs, INOUT StructTypes &types)
	{
		if ( attribs.Empty() )
			return true;

		_StructField	st;
		st.typeName		= "NativeVertex_"_str << name;
		st.type			= EShaderVariable::VertexAttribs;
		st.align		= 1_b;
		st.packing		= EVariablePacking::VertexAttrib;

		FOR( i, attribs )
		{
			auto const&	attr = attribs[i];

			_StructField	field;
			field.name		= attr.first;
			field.offset	= BytesU(attr.second.index);	// it is OK, write index to offset, sort, then calculate correct offset
			field.type		= EShaderVariable::type(attr.second.type);
			field.packing	= st.packing;
			field.stride	= EShaderVariable::SizeOf( field.type );
			field.align		= field.stride % 4 == 0 ? 4_b : 1_b;

			st.fields.PushBack( RVREF(field) );
		}

		Sort( st.fields, LAMBDA() (const auto& lhs, const auto &rhs) { return lhs.offset > rhs.offset; } );

		BytesU	offset;

		for (auto& field : st.fields)
		{
			offset = AlignToLarge( offset, field.align );

			field.offset	= offset;
			st.align		= Max( field.align, st.align );

			offset += EShaderVariable::SizeOf( field.type ) * field.arraySize.Size();
		}

		st.stride = offset;

		types.Add( st.typeName, st );
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


		void operator () (SubpassInput &src)
		{
			src.location.uniqueIndex	= _index++;
			src.location.index			= _textureCounter++;	// in OpenGL subpass is sampler2D
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

		for (auto& un : bindings.uniforms) {
			un.Accept( func );
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

				if ( iter->second.fields.Back().arraySize.IsDynamicArray() )
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

		for (auto& un : bindings.uniforms) {
			un.Accept( func );
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
		bool				changeDescriptions	= false;


	// methods
		_UpdateDescriptorSets_Func ()
		{}

		void StartPass2 ()
		{
			changeDescriptions = true;

			FOR( i, _descriptors ) {
				_descriptors[i].second = uint(i);
			}
		}


		template <typename T>
		void operator () (T &src)
		{
			if ( changeDescriptions )
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

		for (auto& un : bindings.uniforms) {
			un.Accept( func );
		}

		func.StartPass2();
		
		for (auto& un : bindings.uniforms) {
			un.Accept( func );
		}
		return true;
	}
	
/*
=================================================
	_DisasembleShader
=================================================
*/
	bool BasePipeline::_DisasembleShader (const ConverterConfig &, INOUT ShaderModule &shader, OUT ShaderDisasembly &disasm)
	{
		String				str;
		String				log;
		BinaryArray			optimized;
		DeserializedShader	deserialized;
		Array<StringCRef>	source;
		String				shader_entry	= shader.entry;
		const String		version			= _GetVersionGLSL( shaderFormat );
		
		LOG( "Disasemble "_str << EShaderFormat::ToString( shaderFormat ), ELog::Info );

		CHECK_ERR(	EShaderFormat::GetApiFormat( shaderFormat ) == EShaderFormat::GXSL	or
					EShaderFormat::GetApiFormat( shaderFormat ) == EShaderFormat::GLSL	or
					EShaderFormat::GetApiFormat( shaderFormat ) == EShaderFormat::VKSL );

		if ( shader.type == EShader::Vertex )
		{
			str << ToStringGLSL( attribs );
		}

		if ( shader.type == EShader::Fragment )
		{
			str << ToStringGLSL( fragOutput );
		}

		if ( shader.type == EShader::Compute )
		{
			str << _LocalGroupSizeToStringGLSL( localGroupSize );
		}

		// build source
		{
			source	<< version
					<< _GetDefaultHeaderGLSL()
					<< _GetTypeRedefinitionGLSL()
					<< _GetPerShaderHeaderGLSL( shader.type )
					<< str;

			for (auto& src : shader._source) {
				source << StringCRef(src);
			}
		}

		// deserialize
		{
			if ( not ShaderCompiler::Instance()->Deserialize( shaderFormat, shader.type, source, shader_entry, _path, OUT log, OUT deserialized ) )
			{
				RETURN_ERR( log );
			}

			CHECK_ERR( deserialized.CalculateOffsets() );
			CHECK_ERR( deserialized.CalculateLocations() );
			//LOG( deserialized.ToString(), ELog::Debug );

			CHECK_ERR( _ExtractTextures( deserialized, OUT disasm.textures ) );
			CHECK_ERR( _ExtractImages( deserialized, OUT disasm.images ) );
			CHECK_ERR( _ExtractSubpasses( deserialized, OUT disasm.subpasses ) );
			CHECK_ERR( _ExtractUniformBuffers( deserialized, OUT disasm.uniformBuffers ) );
			CHECK_ERR( _ExtractStorageBuffers( deserialized, OUT disasm.storageBuffers ) );
			CHECK_ERR( _ExtractTypes( deserialized, OUT disasm.structTypes ) );
			CHECK_ERR( _ExtractVaryings( deserialized, OUT disasm.input, OUT disasm.output ) );
			
			if ( shader.type == EShader::Vertex )
				CHECK_ERR( _ExtractAttribs( disasm.input, OUT attribs ) );
			
			if ( shader.type == EShader::Fragment )
			{
				CHECK_ERR( _ExtractFragOutput( disasm.output, OUT fragOutput ) );

				earlyFragmentTests = EnumEq( deserialized.FragmentShader().flags, EFragmentShaderParams::EarlyFragmentTests ) or
									 not EnumEq( deserialized.FragmentShader().flags, EFragmentShaderParams::DepthExport );
			}

			if ( shader.type == EShader::TessControl )
				patchControlPoints = deserialized.TessControlShader().patchSize;

			if ( shader.type == EShader::Compute )
				localGroupSize = deserialized.ComputeShader().localSize;
		}

		// add bindings
		{
			_AddBindings( INOUT disasm.textures );
			_AddBindings( INOUT disasm.images );
			_AddBindings( INOUT disasm.subpasses );
			_AddBindings( INOUT disasm.uniformBuffers );
			_AddBindings( INOUT disasm.storageBuffers );
		}

		// add varyings
		{
			shader._io.Clear();

			for (auto& in : disasm.input)
			{
				if ( in.qualifier[ EVariableQualifier::BuiltIn ] )
					continue;

				shader._io << in;
			}
		
			for (auto& out : disasm.output)
			{
				if ( out.qualifier[ EVariableQualifier::BuiltIn ] )
					continue;

				shader._io << out;
			}
		}
		return true;
	}


}	// PipelineCompiler
