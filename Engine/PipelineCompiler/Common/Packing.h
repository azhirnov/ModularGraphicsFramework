// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Common/Common.h"

namespace PipelineCompiler
{
	
	//
	// CalculateOffsets_Helper
	//

	template <typename T>
	struct CalculateOffsets_Helper
	{
	// types
		using Variable	= T;
		using Self		= CalculateOffsets_Helper<T>;


	// constants
		static constexpr BytesU		vec4_align { 16_b };


	// methods
		virtual void CalcOffset (INOUT BytesU &offset, BytesU sizeOf, INOUT Variable &var)
		{
			offset		= AlignToLarge( offset, var.align );
			var.offset	= offset;
			offset		+= sizeOf;
		}


		virtual bool ProcessVec (INOUT BytesU &offset, INOUT Variable &var, EVariablePacking::type packing)
		{
			BytesU		size_of	= 0_b;

			switch ( packing )
			{
				case EVariablePacking::Varying :
				case EVariablePacking::Std140 :
				{
					// see https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_uniform_buffer_object.txt

					const uint		vec_size = EShaderVariable::VecSize( var.type );
					const uint2		mat_size = EShaderVariable::MatSize( var.type );
					const auto		scalar	 = EShaderVariable::ToScalar( var.type );

					// scalar or vector
					if ( vec_size > 0 )
					{
						// rule 4
						if ( var.arraySize == 1 )
						{
							// rule 1, 2, 3
							var.align =	vec_size == 1 ?	EShaderVariable::SizeOf( var.type, 0_b ) :
										vec_size == 2 ?	EShaderVariable::SizeOf( var.type, 0_b ) :
										/* 3 or 4 */	EShaderVariable::SizeOf( scalar, 0_b ) * 4;
						}
						else
							var.align = EShaderVariable::SizeOf( var.type, vec4_align );

						var.stride	= var.align;
						size_of		= var.align * var.arraySize;
					}
					else
					// matrix
					if ( All( mat_size > 0 ) )
					{
						// rules 5..8 equivalent of rule 4
						var.align	= EShaderVariable::SizeOf( EShaderVariable::ToVec( scalar, mat_size.x ), vec4_align );
						var.stride	= EShaderVariable::SizeOf( var.type, var.align );
						size_of		= var.stride * var.arraySize;
					}
					else
					// sampler or struct or array of samplers or structs
					{
						// rule 9, 10
						TODO( "" );
					}
					break;
				}
				
				case EVariablePacking::Default :
				case EVariablePacking::Std430 :
				{
					// see https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_shader_storage_buffer_object.txt
						
					const uint		vec_size = EShaderVariable::VecSize( var.type );
					const uint2		mat_size = EShaderVariable::MatSize( var.type );
					const auto		scalar	 = EShaderVariable::ToScalar( var.type );

					// scalar or vector
					if ( vec_size > 0 )
					{
						// rule 1, 2, 3
						var.align =	vec_size == 1 ?	EShaderVariable::SizeOf( var.type, 0_b ) :
									vec_size == 2 ?	EShaderVariable::SizeOf( var.type, 0_b ) :
									/* 3 or 4 */	EShaderVariable::SizeOf( scalar, 0_b ) * 4;

						var.stride	= var.align;
						size_of		= var.align * var.arraySize;
					}
					else
					// matrix
					if ( All( mat_size > 0 ) )
					{
						// rules 5..8 equivalent of rule 4
						var.align	= EShaderVariable::SizeOf( EShaderVariable::ToVec( scalar, mat_size.x ), vec4_align );
						var.stride	= EShaderVariable::SizeOf( var.type, var.align );
						size_of		= var.stride * var.arraySize;
					}
					else
					// sampler or struct or array of samplers or structs
					{
						// rule 9, 10
						TODO( "" );
					}
					break;
				}

				case EVariablePacking::Shared :
				case EVariablePacking::Packed :
				{
					var.align	= 1_b;
					var.stride	= EShaderVariable::SizeOf( var.type, 0_b );
					size_of		= var.stride * var.arraySize;

					WARNING( "implementation definied packing!" );
					break;
				}

				default :
					RETURN_ERR( "unsupported packing!" );
			}

			CalcOffset( INOUT offset, size_of, INOUT var );
			return true;
		}


		virtual bool ProcessStruct (INOUT BytesU &offset, INOUT Variable &var, EVariablePacking::type packing)
		{
			ASSERT( EShaderVariable::IsStruct( var.type ) );
			
			// process fields
			BytesU	max_align	= 0_b;
			BytesU	size		= 0_b;
			RecursiveProcess( INOUT var.fields, INOUT size, INOUT max_align, packing );

			// calc align of structure
			switch ( packing )
			{
				case EVariablePacking::Varying :
					var.align = vec4_align;
					break;

				case EVariablePacking::Std140 :
					// rule 9
					var.align = Max( max_align, vec4_align );
					break;

				case EVariablePacking::Default :
				case EVariablePacking::Std430 :
					// rule 9
					var.align = max_align;
					break;

				case EVariablePacking::Shared :
				case EVariablePacking::Packed :
					WARNING( "implementation definied packing!" );
					break;

				default :
					RETURN_ERR( "unsupported packing!" );
			}

			size = AlignToLarge( size, var.align );


			// process fields
			max_align = 0_b;
			CalcOffset( INOUT offset, 0_b, INOUT var );
			RecursiveProcess( INOUT var.fields, INOUT offset, INOUT max_align, packing );
			
			offset		= AlignToLarge( offset, var.align );
			var.stride	= size;

			ASSERT( max_align <= var.align );
			ASSERT( offset - var.offset == size );
			ASSERT( (offset + size * (var.arraySize - 1)) == (var.offset + var.stride * var.arraySize) );

			offset		= var.offset + var.stride * var.arraySize;
			return true;
		}


		virtual bool RecursiveProcess (INOUT Array<Variable> &fields, INOUT BytesU &offset, INOUT BytesU &maxAlign,
										EVariablePacking::type packing)
		{
			FOR( i, fields )
			{
				auto&	var = fields[i];

				// if dynamic array
				if ( var.arraySize == 0 ) {
					ASSERT( i == fields.LastIndex() and "dynamic array must be the last element of buffer!" );
				}

				if ( EShaderVariable::IsStruct( var.type ) ) {
					// struct
					CHECK_ERR( ProcessStruct( INOUT offset, INOUT var, packing ) );
				}
				else {
					// scalar, vector, matrix
					CHECK_ERR( ProcessVec( INOUT offset, INOUT var, packing ) );
				}
					
				maxAlign = Max( maxAlign, var.align );
			}

			return true;
		}
	};



	//
	// ReplaceTypes_Helper
	//

	template <typename T>
	struct ReplaceTypes_Helper
	{
	// types
		using Variable	= T;
		using Self		= ReplaceTypes_Helper<T>;
		using NameSet_t	= HashSet< String >;


	// constants
		static constexpr BytesU		vec4_align { 16_b };


	// variables
		NameSet_t	_processedTypes;
		uint		_paddingCount	= 0;
		

	// methods
		ReplaceTypes_Helper ()
		{}


		void _AddPadding (INOUT Array<Variable> &fields, INOUT usize i, const BytesU freeSpace, const BytesU align)
		{
			// calculate padding
			usize	padding_vec = usize(freeSpace / 4_b);

			if ( padding_vec % 4 == 0 )	padding_vec = 4;	else
			if ( padding_vec % 2 == 0 )	padding_vec = 2;	else
										padding_vec = 1;

			// add padding
			const usize		padding_count	= usize(freeSpace / 4_b) / padding_vec;
			const auto		var2			= fields[i];
			BytesU			stride			= 0_b;

			for (usize j = 0; j < padding_count; ++j)
			{
				Variable		padding;
				padding.name		= "_padding"_str << (_paddingCount++);
				padding.type		= EShaderVariable::ToVec( EShaderVariable::Float, padding_vec );
				padding.arraySize	= 1;
				padding.offset		= var2.offset + var2.stride + stride;
				padding.stride		= EShaderVariable::SizeOf( padding.type, 4_b );
				padding.align		= padding.stride;
				padding.precision	= var2.precision;
				padding.qualifier	= var2.qualifier;
				padding.memoryModel	= var2.memoryModel;

				stride += padding.stride;

				fields.Insert( padding, ++i );
			}

			ASSERT( freeSpace == stride );
			//ASSERT( var2.stride + stride == sizeOf );
		}


		virtual void ReplaceVector (INOUT Array<Variable> &fields, INOUT usize &i, const BytesU sizeOf)
		{
			auto&			var			= fields[i];
			const uint		vec_size	= EShaderVariable::VecSize( var.type );
			BytesU			free_space	= sizeOf;
			
			// replace to aligned vector
			if ( vec_size > 2 )
			{
				var.type	= EShaderVariable::ToVec( var.type, 4 );
				var.align	= EShaderVariable::SizeOf( var.type, 4_b );
				var.stride	= var.align;

				if ( var.stride == sizeOf )
					return;

				free_space -= var.stride;
			}
			else
			{
				var.align	= 4_b;
				var.stride	= EShaderVariable::SizeOf( var.type, var.align );
				free_space	-= var.stride;
			}

			_AddPadding( INOUT fields, INOUT i, free_space, 0_b );
		}


		virtual void ReplaceMatrix (INOUT Array<Variable> &fields, INOUT usize &i, BytesU sizeOf)
		{
			auto&			var			= fields[i];
			const uint2		mat_size	= EShaderVariable::MatSize( var.type );
			
			// convert to aligned matrix
			var.type		= EShaderVariable::ToMat( var.type, mat_size.x, 4 );
			var.stride		= EShaderVariable::SizeOf( var.type, var.align );

			ASSERT( sizeOf == var.stride );
		}


		virtual void ReplaceArray (INOUT Array<Variable> &fields, INOUT usize &i, BytesU sizeOf)
		{
			auto&			var			= fields[i];
			const BytesU	elem_size	= (i < fields.LastIndex() ? (fields[i+1].offset - var.offset) / var.arraySize : var.stride);
			const uint		vec_size	= EShaderVariable::VecSize( var.type );
			const uint2		mat_size	= EShaderVariable::MatSize( var.type );
			const BytesU	scalar_size	= EShaderVariable::SizeOf( EShaderVariable::ToScalar( var.type ), 0_b );

			// change type
			if ( All( mat_size > 0u ) )
			{
				// convert to aligned matrix
				var.type		= EShaderVariable::ToMat( var.type, mat_size.x, 4 );
				var.stride		= EShaderVariable::SizeOf( var.type, var.align );
			}
			else
			if ( vec_size > 0u )
			{
				const usize		new_vsize	= usize(var.stride / scalar_size);
				ASSERT( new_vsize == 1 or new_vsize == 2 or new_vsize == 4 );

				// convert to aligned vector
				var.type		= EShaderVariable::ToVec( var.type, new_vsize );
				var.stride		= EShaderVariable::SizeOf( var.type, var.align );
			}
					
			// add padding
			const BytesU	unaligned		= EShaderVariable::SizeOf( var.type, 0_b );
			const BytesU	padding_size	= sizeOf - unaligned * var.arraySize;

			if ( padding_size != 0_b )
			{
				const usize		new_vsize	= usize(padding_size / 4_b);
				Variable		padding;

				padding.name		= "_padding"_str << (_paddingCount++);
				padding.type		= EShaderVariable::ToVec( EShaderVariable::Float, new_vsize );
				padding.arraySize	= 1;
				padding.offset		= var.offset + unaligned * var.arraySize;
				padding.stride		= EShaderVariable::SizeOf( padding.type, 4_b );
				padding.align		= padding.stride;
				padding.precision	= var.precision;
				padding.qualifier	= var.qualifier;
				padding.memoryModel	= var.memoryModel;

				ASSERT( padding.stride == EShaderVariable::SizeOf( padding.type, padding.align ) );
				ASSERT( padding_size == padding.stride );

				fields.Insert( padding, i+1 );
			}

			//ASSERT( elem_size == var.stride );
		}


		virtual bool ProcessType (StringCRef typeName)
		{
			// override this method to process other types
			return false;
		}
		

		virtual bool RecursiveProcess (StringCRef typeName, INOUT Array<Variable> &fields, OUT BytesU &offset, OUT BytesU &maxAlign)
		{
			if ( _processedTypes.IsExist( typeName ) )
				return true;

			_processedTypes.Add( typeName );

			// prepare
			BytesU	min_offset		= ~0_b;
			BytesU	max_offset		= 0_b;
			
			FOR( i, fields )
			{
				const auto&		var = fields[i];

				if ( EShaderVariable::IsStruct( var.type ) and var.fields.Empty() )
				{
					if ( not _processedTypes.IsExist( var.typeName ) ) {
						CHECK_ERR( ProcessType( var.typeName ) );
					}
				}

				min_offset = Min( var.offset, min_offset );
				max_offset = Max( var.offset, max_offset );
			}


			// check padding
			_paddingCount	= 0;
			offset			= 0_b;
			maxAlign		= 0_b;

			FOR( i, fields )
			{
				if ( EShaderVariable::IsStruct( fields[i].type ) )
				{
					auto&		var = fields[i];

					if ( not var.fields.Empty() )
					{
						BytesU	off, align;
						CHECK_ERR( RecursiveProcess( var.typeName, INOUT var.fields, OUT off, OUT align ) );

						CHECK_ERR( off % align == 0_b );
					}
				}
				else
				{
					const auto&		var			= fields[i];
					const BytesU	unaligned	= EShaderVariable::SizeOf( var.type, 0_b ) * var.arraySize;
					const BytesU	size_of		= (i < fields.LastIndex() ? fields[i+1].offset - var.offset : var.stride * var.arraySize);
					const uint2		mat_size	= EShaderVariable::MatSize( var.type );
					
					if ( unaligned == size_of )
						{}	// without padding
					else
					if ( var.arraySize > 1 )
						ReplaceArray( INOUT fields, INOUT i, size_of );
					else
					if ( All( mat_size > 0u ) )
						ReplaceMatrix( INOUT fields, INOUT i, size_of );
					else
						ReplaceVector( INOUT fields, INOUT i, size_of );
				}

				{
					auto&	var	= fields[i];

					var.offset -= min_offset;

					offset	 = var.offset + var.stride * var.arraySize;
					maxAlign = Max( var.align, maxAlign );
				}
			}

			const BytesU	free_space	= maxAlign - (offset % maxAlign);
			usize			i			= fields.LastIndex();
			
			if ( free_space != 0_b ) {
				_AddPadding( INOUT fields, INOUT i, free_space, maxAlign );
			}
			return true;
		}
	};


}	// PipelineCompiler
