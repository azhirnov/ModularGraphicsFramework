// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/DeserializedShader.h"
#include "Engine/PipelineCompiler/Common/ToGLSL.h"
#include "Engine/PipelineCompiler/Common/Packing.h"
#include "Core/STL/Algorithms/StringParser.h"

namespace PipelineCompiler
{
	
/*
=================================================
	IncIndent
=================================================
*/
	inline String IncIndent (String &&str, StringCRef indent = "  ")
	{
		StringParser::IncreaceIndent( str, indent );
		return str;
	}

/*
=================================================
	TextureUniform::ToString
=================================================
*/
	String DeserializedShader::TextureUniform::ToString () const
	{
		String	str;
		str << "TextureUniform {"
			<< "\n  name:          " << name
			<< "\n  unit:          " << unit
			<< "\n  descriptorSet: " << descriptorSet
			<< "\n  imageType:     " << EImage::ToString( imageType )
			<< "\n  format:        " << String().FormatI( format, 16 )
			<< "\n}";
		return str;
	}
	
/*
=================================================
	ImageUniform::ToString
=================================================
*/
	String DeserializedShader::ImageUniform::ToString () const
	{
		String	str;
		str << "ImageUniform {"
			<< "\n  name:          " << name
			<< "\n  unit:          " << unit
			<< "\n  descriptorSet: " << descriptorSet
			<< "\n  imageType:     " << EImage::ToString( imageType )
			<< "\n  memoryModel:   " << EShaderMemoryModel::ToString( memoryModel )
			<< "\n  format:        " << (format == EPixelFormat::Unknown ? "none" : EPixelFormat::ToString( format ))
			<< "\n}";
		return str;
	}
	
/*
=================================================
	_Variable::_ToString
=================================================
*/
	String DeserializedShader::_Variable::_ToString () const
	{
		String	str;
		str << "\n  name:        " << name
			<< "\n  type:        " << /*EShaderVariable::ToString*/ (EShaderVariable::IsStruct( type ) ? StringCRef(typeName) : ToStringGLSL( type ))
			//<< "\n  typeName:    " << typeName
			<< "\n  precision:   " << EPrecision::ToString( precision )
			<< "\n  qualifier:   " << EVariableQualifier::ToString( qualifier )
			<< "\n  memoryModel: " << EShaderMemoryModel::ToString( memoryModel )
			<< "\n  format:      " << (format == EPixelFormat::Unknown ? "none" : EPixelFormat::ToString( format ))
			<< "\n  arraySize:   " << (arraySize.IsDynamicArray() ? "dynamic"_str : arraySize.IsNotArray() ? ""_str : String().FormatI( arraySize.Size(), 10 ));
		return str;
	}
	
/*
=================================================
	Uniform::ToString
=================================================
*/
	String DeserializedShader::Uniform::ToString () const
	{
		String	str;
		str << "Uniform {"
			<< _ToString()
			<< "\n  location:    " << location
			<< "\n}";
		return str;
	}

/*
=================================================
	BufferVariable::ToString
=================================================
*/
	String DeserializedShader::BufferVariable::ToString () const
	{
		String	str;
		str << "BufferVariable {"
			<< _ToString()
			<< "\n  offset:      " << GX_STL::GXTypes::ToString( offset )
			<< "\n  align:       " << GX_STL::GXTypes::ToString( align )
			<< "\n  stride:      " << GX_STL::GXTypes::ToString( stride );

		if ( not fields.Empty() )
		{
			str << "\n  fields = {";
			FOR( i, fields ) {
				str << '\n' << IncIndent( fields[i].ToString(), "    " );
			}
			str << "\n  }";
		}
		str << "\n}";
		return str;
	}
	
/*
=================================================
	UniformBuffer::ToString
=================================================
*/
	String DeserializedShader::UniformBuffer::ToString () const
	{
		String	str;
		str << "UniformBuffer {"
			<< "\n  name:          " << name
			<< "\n  typeName:      " << typeName
			<< "\n  bindingIndex:  " << bindingIndex
			<< "\n  descriptorSet: " << descriptorSet
			<< "\n  packing:       " << (packing != EVariablePacking::Unknown ? EVariablePacking::ToString( packing ) : "")
			<< "\n  size:          " << GX_STL::GXTypes::ToString( size );

		if ( not fields.Empty() )
		{
			str << "\n  fields = {";
			FOR( i, fields ) {
				str << "\n" << IncIndent( fields[i].ToString(), "    " );
			}
			str << "\n  }";
		}
		str << "\n}";
		return str;
	}
	
/*
=================================================
	StorageBuffer::ToString
=================================================
*/
	String DeserializedShader::StorageBuffer::ToString () const
	{
		String	str;
		str << "StorageBuffer {"
			<< "\n  name:          " << name
			<< "\n  typeName:      " << typeName
			<< "\n  bindingIndex:  " << bindingIndex
			<< "\n  descriptorSet: " << descriptorSet
			<< "\n  memoryModel:   " << EShaderMemoryModel::ToString( memoryModel )
			<< "\n  packing:       " << (packing != EVariablePacking::Unknown ? EVariablePacking::ToString( packing ) : "")
			<< "\n  size:          " << GX_STL::GXTypes::ToString( size );

		if ( not fields.Empty() )
		{
			str << "\n  fields = {";
			FOR( i, fields ) {
				str << "\n" << IncIndent( fields[i].ToString(), "    " );
			}
			str << "\n  }";
		}
		str << "\n}";
		return str;
	}
	
/*
=================================================
	PushConstant::ToString
=================================================
*/
	String DeserializedShader::PushConstant::ToString () const
	{
		String	str;
		str << "PushConstant {"
			<< "\n}";
		return str;
	}
	
/*
=================================================
	IOVariable::ToString
=================================================
*/
	String DeserializedShader::IOVariable::ToString () const
	{
		String	str;
		str << "IOVariable {"
			<< _ToString()
			<< "\n  location:    " << location;

		if ( not fields.Empty() )
		{
			str << "\n  fields = {";
			FOR( i, fields ) {
				str << "\n" << IncIndent( fields[i].ToString(), "    " );
			}
			str << "\n  }";
		}
		str << "\n}";
		return str;
	}
	
/*
=================================================
	Constant_ToString_Func
=================================================
*/
	struct Constant_ToString_Func
	{
		String &	str;

		Constant_ToString_Func (String &str) : str(str)
		{}

		template <typename T>
		void operator () (const T &value) const
		{
			str << "\t\t{ " << ToString( value ) << "}";
		}
	};
	
/*
=================================================
	Constant::ToString
=================================================
*/
	String DeserializedShader::Constant::ToString () const
	{
		String	str;
		str << "Constant {"
			<< _ToString()
			<< "\n  values:    ";

		Constant_ToString_Func	func( str );
		FOR( i, values )
		{
			str << (i ? ",\n" : "");
			values[i].Apply( func );
		}
		str << "\n}";
		return str;
	}
	
/*
=================================================
	FunctionArgument::ToString
=================================================
*/
	String DeserializedShader::FunctionArgument::ToString () const
	{
		String	str;
		str << "FunctionArgument {"
			<< _ToString();

		if ( not fields.Empty() )
		{
			str << "\n  fields = {";
			FOR( i, fields ) {
				str << "\n" << IncIndent( fields[i].ToString(), "    " );
			}
			str << "\n  }";
		}
		str << "\n}";
		return str;
	}
	
/*
=================================================
	Function::ToString
=================================================
*/
	String DeserializedShader::Function::ToString () const
	{
		String	str;
		str << "Function {"
			<< "\n  name:       " << name
			<< "\n  signature:  " << signature
			<< "\n  result = \n" << IncIndent( result.ToString(), "    " )
			<< "\n  args = {";
		FOR( i, args ) {
			str << "\n" << IncIndent( args[i].ToString(), "    " );
		}
		str << "\n  }"
			<< "\n}";
		return str;
	}
	
/*
=================================================
	VertexShaderInfo::ToString
=================================================
*/
	String DeserializedShader::VertexShaderInfo::ToString () const
	{
		String	str;
		return str;
	}
	
/*
=================================================
	TessControlShaderInfo::ToString
=================================================
*/
	String DeserializedShader::TessControlShaderInfo::ToString () const
	{
		String	str;
		str << "\n  patchSize:   " << patchSize;
		return str;
	}
	
/*
=================================================
	TessEvaluationShaderInfo::ToString
=================================================
*/
	String DeserializedShader::TessEvaluationShaderInfo::ToString () const
	{
		String	str;
		str << "\n  spacing:   " << ETessellationSpacing::ToString( spacing )
			<< "\n  primitive: " << ETessellationInputPrimitive::ToString( primitive )
			<< "\n  ccw:       " << ccw;
		return str;
	}
	
/*
=================================================
	GeometryShaderInfo::ToString
=================================================
*/
	String DeserializedShader::GeometryShaderInfo::ToString () const
	{
		String	str;
		str << "\n  inputPrimitive:    " << EGeometryInputPrimitive::ToString( inputPrimitive )
			<< "\n  outputPrimitive:   " << EGeometryOutputPrimitive::ToString( outputPrimitive )
			<< "\n  invocations:       " << invocations
			<< "\n  maxOutputVertices: " << maxOutputVertices;
		return str;
	}
	
/*
=================================================
	FragmentShaderInfo::ToString
=================================================
*/
	String DeserializedShader::FragmentShaderInfo::ToString () const
	{
		String	str;

		//if ( flags & EFragmentShaderParams::_ConservativeDepth_Mask )
		//{
		//}

		//if ( EnumEq( flags, EFragmentShaderParams::EarlyFragmentTests ) )	str << "\n  EarlyFragmentTests";

		//str << "\n  inputPrimitive:    "
		return str;
	}
	
/*
=================================================
	ComputeShaderInfo::ToString
=================================================
*/
	String DeserializedShader::ComputeShaderInfo::ToString () const
	{
		String	str;
		str << "\n  localSize:    " << GX_STL::GXTypes::ToString( localSize );
		return str;
	}
	
/*
=================================================
	ToString
=================================================
*/
	String DeserializedShader::ToString () const
	{
		String	str;
		str << "Deserialized " << EShader::ToString( _shaderType ) << " shader {\n";

		if ( not _textures.Empty() )
		{
			str << "\n  textures = {";
			FOR( i, _textures ) {
				str << "\n" << IncIndent( _textures[i].ToString(), "    " );
			}
			str << "\n  }";
		}

		if ( not _images.Empty() )
		{
			str << "\n  images = {";
			FOR( i, _images ) {
				str << "\n" << IncIndent( _images[i].ToString(), "    " );
			}
			str << "\n  }";
		}

		if ( not _uniforms.Empty() )
		{
			str << "\n  uniforms = {";
			FOR( i, _uniforms ) {
				str << "\n" << IncIndent( _uniforms[i].ToString(), "    " );
			}
			str << "\n  }";
		}

		if ( not _uniformBuffers.Empty() )
		{
			str << "\n  uniform_buffers = {";
			FOR( i, _uniformBuffers ) {
				str << "\n" << IncIndent( _uniformBuffers[i].ToString(), "    " );
			}
			str << "\n  }";
		}

		if ( not _storageBuffers.Empty() )
		{
			str << "\n  storage_buffers = {";
			FOR( i, _storageBuffers ) {
				str << "\n" << IncIndent( _storageBuffers[i].ToString(), "    " );
			}
			str << "\n  }";
		}

		if ( not _pushConstants.Empty() )
		{
			str << "\n  push_constants = {";
			FOR( i, _pushConstants ) {
				str << "\n" << IncIndent( _pushConstants[i].ToString(), "    " );
			}
			str << "\n  }";
		}

		if ( not _variables.Empty() )
		{
			str << "\n  io_variables = {";
			FOR( i, _variables ) {
				str << "\n" << IncIndent( _variables[i].ToString(), "    " );
			}
			str << "\n  }";
		}

		if ( not _constants.Empty() )
		{
			str << "\n  constants = {";
			FOR( i, _constants ) {
				str << "\n" << IncIndent( _constants[i].ToString(), "    " );
			}
			str << "\n  }";
		}

		if ( not _functions.Empty() )
		{
			str << "\n  functions = {";
			FOR( i, _functions ) {
				str << "\n" << IncIndent( _functions[i].ToString(), "    " );
			}
			str << "\n  }";
		}

		if ( not _extensions.Empty() )
		{
			str << "\n  extensions = {";
			FOR( i, _extensions ) {
				str << (i ? "," : "") << "\n\t\t\"" << _extensions[i] << '"';
			}
			str << "\n  }";
		}

		switch ( _shaderType )
		{
			case EShader::Vertex : {
				str << IncIndent( _vertex.ToString(), "  " );
				break;
			}
			case EShader::TessControl : {
				str << IncIndent( _tessControl.ToString(), "  " );
				break;
			}
			case EShader::TessEvaluation : {
				str << IncIndent( _tessEvaluation.ToString(), "  " );
				break;
			}
			case EShader::Geometry : {
				str << IncIndent( _geometry.ToString(), "  " );
				break;
			}
			case EShader::Fragment : {
				str << IncIndent( _fragment.ToString(), "  " );
				break;
			}
			case EShader::Compute : {
				str << IncIndent( _compute.ToString(), "  " );
				break;
			}
		}
		
		str << "\n}";
		return str;
	}
	
/*
=================================================
	CalculateOffsets
=================================================
*/
	bool DeserializedShader::CalculateOffsets ()
	{
		CalculateOffsets_Helper<BufferVariable>		helper;

		const BytesU	vec4_align = 16_b;

		FOR( i, _uniformBuffers )
		{
			auto&	buf		= _uniformBuffers[i];
			BytesU	offset;
			BytesU	align;

			helper.RecursiveProcess( INOUT buf.fields, INOUT offset, INOUT align, buf.packing );

			buf.size			= AlignToLarge( offset, vec4_align );
			buf.bindingIndex	= uint(i);
		}
		
		FOR( i, _storageBuffers )
		{
			auto&	buf		= _storageBuffers[i];
			BytesU	offset;
			BytesU	align;
			
			helper.RecursiveProcess( INOUT buf.fields, INOUT offset, INOUT align, buf.packing );

			buf.size			= offset;
			buf.bindingIndex	= uint(i);
		}
		return true;
	}
	
/*
=================================================
	CalculateVaryingLocations_Helper
=================================================
*/
	struct CalculateVaryingLocations_Helper
	{
	// types
		using IOVariable	= DeserializedShader::IOVariable;

		struct Sorter
		{
			bool operator () (const IOVariable &left, const IOVariable &right) const
			{
				const bool		l_struct= EShaderVariable::IsStruct( left.type );
				const bool		r_struct= EShaderVariable::IsStruct( right.type );
				const BytesU	l_size	= (l_struct ? ~0_b : EShaderVariable::SizeOf( left.type, 0_b ));
				const BytesU	r_size	= (r_struct ? ~0_b : EShaderVariable::SizeOf( right.type, 0_b ));

				return	l_struct and r_struct ?
							(left.typeName	!= right.typeName	?	left.typeName > right.typeName	:
																	left.name	  > right.name)	:
							(l_size			!= r_size			?	l_size		  < r_size			:
																	left.name	  > right.name);
			}
		};

	// variables
		Array<IOVariable>	_tempInput;
		Array<IOVariable>	_tempOutput;
		uint				_inLocation		= 0;
		uint				_outLocation	= 0;

	// methods
		bool RecursiveProcess (INOUT Array<IOVariable> &fields)
		{
			_tempInput.Clear();
			_tempOutput.Clear();

			for (const auto& fld : fields)
			{
				if ( fld.qualifier[ EVariableQualifier::BuiltIn ] )
					{}	// skip
				else
				if ( fld.qualifier[ EVariableQualifier::In ] )
					_tempInput << fld;
				else
				if ( fld.qualifier[ EVariableQualifier::Out ] )
					_tempOutput << fld;
				else
					RETURN_ERR( "unknown qualifier type!" );
			}

			Sort( _tempInput, Sorter() );
			Sort( _tempOutput, Sorter() );

			fields.Clear();
			fields << _tempInput << _tempOutput;

			for (auto& fld : fields)
			{
				if ( fld.type == EShaderVariable::VaryingsBlock )
					{}	// skip
				else
				if ( fld.qualifier[ EVariableQualifier::In ] )
					fld.location = _inLocation++;
				else
				if ( fld.qualifier[ EVariableQualifier::Out ] )
					fld.location = _outLocation++;

				if ( not fld.fields.Empty() ) {
					CHECK_ERR( RecursiveProcess( INOUT fld.fields ) );
				}
			}
			return true;
		}
	};
	
/*
=================================================
	CalculateLocations
=================================================
*/
	bool DeserializedShader::CalculateLocations ()
	{
		// varyings
		{
			CalculateVaryingLocations_Helper	helper;
			helper.RecursiveProcess( INOUT _variables );
		}

		// textures
		{
			struct TexSorting {
				bool operator () (const TextureUniform &left, const TextureUniform &right) const {
					return	left.name > right.name;
				}
			};
			Sort( _textures, TexSorting() );

			FOR( i, _textures ) {
				_textures[i].unit = uint(i);
			}
		}

		// images
		{
			struct ImgSorting {
				bool operator () (const ImageUniform &left, const ImageUniform &right) const {
					return	left.name > right.name;
				}
			};
			Sort( _images, ImgSorting() );

			FOR( i, _images ) {
				_images[i].unit = uint(i);
			}
		}
		return true;
	}
	
/*
=================================================
	ReplaceTypesInStructs
=================================================
*/
	bool DeserializedShader::ReplaceTypesInStructs ()
	{
		ReplaceTypes_Helper<BufferVariable>		helper;
		
		const BytesU	vec4_align = 16_b;

		FOR( i, _uniformBuffers )
		{
			auto&	buf		= _uniformBuffers[i];
			BytesU	offset;
			BytesU	max_align;

			CHECK_ERR( helper.RecursiveProcess( buf.typeName, INOUT buf.fields, OUT offset, OUT max_align ) );

			ASSERT( buf.size == AlignToLarge( offset, vec4_align ) );
		}
		
		FOR( i, _storageBuffers )
		{
			auto&	buf		= _storageBuffers[i];
			BytesU	offset;
			BytesU	max_align;

			CHECK_ERR( helper.RecursiveProcess( buf.typeName, INOUT buf.fields, OUT offset, OUT max_align ) );
			
			ASSERT( buf.size == offset );
		}
		return true;
	}


}	// PipelineCompiler
