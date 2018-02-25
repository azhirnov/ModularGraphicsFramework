// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/PipelineLayout.h"

namespace Engine
{
namespace Platforms
{

/*
=================================================
	TextureUniform::operator ==
=================================================
*/
	bool PipelineLayoutDescriptor::TextureUniform::operator == (const TextureUniform &right) const
	{
		return	stageFlags		== right.stageFlags		and
				binding			== right.binding		and
				uniqueIndex		== right.uniqueIndex	and
				textureType		== right.textureType	and
				format			== right.format			and
				name			== right.name;
	}
	
/*
=================================================
	TextureUniform::operator >
=================================================
*/
	bool PipelineLayoutDescriptor::TextureUniform::operator >  (const TextureUniform &right) const
	{
		return	stageFlags		!= right.stageFlags		?	stageFlags		> right.stageFlags		:
				binding			!= right.binding		?	binding			> right.binding			:
				uniqueIndex		!= right.uniqueIndex	?	uniqueIndex		< right.uniqueIndex		:
				textureType		!= right.textureType	?	textureType		> right.textureType		:
				format			!= right.format			?	format			> right.format			:
															name			> right.name;
	}
	
/*
=================================================
	TextureUniform::operator <
=================================================
*/
	bool PipelineLayoutDescriptor::TextureUniform::operator <  (const TextureUniform &right) const
	{
		return	stageFlags		!= right.stageFlags		?	stageFlags		< right.stageFlags		:
				binding			!= right.binding		?	binding			< right.binding			:
				uniqueIndex		!= right.uniqueIndex	?	uniqueIndex		< right.uniqueIndex		:
				textureType		!= right.textureType	?	textureType		< right.textureType		:
				format			!= right.format			?	format			< right.format			:
															name			< right.name;
	}
//-----------------------------------------------------------------------------
		


/*
=================================================
	SamplerUniform::operator ==
=================================================
*/
	bool PipelineLayoutDescriptor::SamplerUniform::operator == (const SamplerUniform &right) const
	{
		return	stageFlags		== right.stageFlags		and
				binding			== right.binding		and
				uniqueIndex		== right.uniqueIndex	and
				name			== right.name;
	}
	
/*
=================================================
	SamplerUniform::operator >
=================================================
*/
	bool PipelineLayoutDescriptor::SamplerUniform::operator >  (const SamplerUniform &right) const
	{
		return	stageFlags		!= right.stageFlags		?	stageFlags		> right.stageFlags		:
				binding			!= right.binding		?	binding			> right.binding			:
				uniqueIndex		!= right.uniqueIndex	?	uniqueIndex		< right.uniqueIndex		:
															name			> right.name;
	}
	
/*
=================================================
	SamplerUniform::operator <
=================================================
*/
	bool PipelineLayoutDescriptor::SamplerUniform::operator <  (const SamplerUniform &right) const
	{
		return	stageFlags		!= right.stageFlags		?	stageFlags		< right.stageFlags		:
				binding			!= right.binding		?	binding			< right.binding			:
				uniqueIndex		!= right.uniqueIndex	?	uniqueIndex		< right.uniqueIndex		:
															name			< right.name;
	}
//-----------------------------------------------------------------------------
			


/*
=================================================
	SubpassInput::operator ==
=================================================
*/
	bool PipelineLayoutDescriptor::SubpassInput::operator == (const SubpassInput &right) const
	{
		return	stageFlags		== right.stageFlags		and
				binding			== right.binding		and
				uniqueIndex		== right.uniqueIndex	and
				name			== right.name;
	}
	
/*
=================================================
	SubpassInput::operator >
=================================================
*/
	bool PipelineLayoutDescriptor::SubpassInput::operator >  (const SubpassInput &right) const
	{
		return	stageFlags		!= right.stageFlags		?	stageFlags		> right.stageFlags		:
				binding			!= right.binding		?	binding			> right.binding			:
				uniqueIndex		!= right.uniqueIndex	?	uniqueIndex		< right.uniqueIndex		:
															name			> right.name;
	}
	
/*
=================================================
	SubpassInput::operator <
=================================================
*/
	bool PipelineLayoutDescriptor::SubpassInput::operator <  (const SubpassInput &right) const
	{
		return	stageFlags		!= right.stageFlags		?	stageFlags		< right.stageFlags		:
				binding			!= right.binding		?	binding			< right.binding			:
				uniqueIndex		!= right.uniqueIndex	?	uniqueIndex		< right.uniqueIndex		:
															name			< right.name;
	}
//-----------------------------------------------------------------------------
			


/*
=================================================
	ImageUniform::operator ==
=================================================
*/
	bool PipelineLayoutDescriptor::ImageUniform::operator == (const ImageUniform &right) const
	{
		return	stageFlags		== right.stageFlags		and
				binding			== right.binding		and
				uniqueIndex		== right.uniqueIndex	and
				imageType		== right.imageType		and
				format			== right.format			and
				writeAccess		== right.writeAccess	and
				readAccess		== right.readAccess		and
				name			== right.name;
	}
	
/*
=================================================
	ImageUniform::operator >
=================================================
*/
	bool PipelineLayoutDescriptor::ImageUniform::operator >  (const ImageUniform &right) const
	{
		return	stageFlags		!= right.stageFlags		?	stageFlags		> right.stageFlags		:
				binding			!= right.binding		?	binding			> right.binding			:
				uniqueIndex		!= right.uniqueIndex	?	uniqueIndex		< right.uniqueIndex		:
				imageType		!= right.imageType		?	imageType		> right.imageType		:
				format			!= right.format			?	format			> right.format			:
				writeAccess		!= right.writeAccess	?	writeAccess		> right.writeAccess		:
				readAccess		!= right.readAccess		?	readAccess		> right.readAccess		:
															name			> right.name;
	}
	
/*
=================================================
	ImageUniform::operator <
=================================================
*/
	bool PipelineLayoutDescriptor::ImageUniform::operator <  (const ImageUniform &right) const
	{
		return	stageFlags		!= right.stageFlags		?	stageFlags		< right.stageFlags		:
				binding			!= right.binding		?	binding			< right.binding			:
				uniqueIndex		!= right.uniqueIndex	?	uniqueIndex		< right.uniqueIndex		:
				imageType		!= right.imageType		?	imageType		< right.imageType		:
				format			!= right.format			?	format			< right.format			:
				writeAccess		!= right.writeAccess	?	writeAccess		< right.writeAccess		:
				readAccess		!= right.readAccess		?	readAccess		< right.readAccess		:
															name			< right.name;
	}
//-----------------------------------------------------------------------------
			


/*
=================================================
	UniformBuffer::operator ==
=================================================
*/
	bool PipelineLayoutDescriptor::UniformBuffer::operator == (const UniformBuffer &right) const
	{
		return	stageFlags		== right.stageFlags		and
				binding			== right.binding		and
				uniqueIndex		== right.uniqueIndex	and
				size			== right.size			and
				name			== right.name;
	}
	
/*
=================================================
	UniformBuffer::operator >
=================================================
*/
	bool PipelineLayoutDescriptor::UniformBuffer::operator >  (const UniformBuffer &right) const
	{
		return	stageFlags		!= right.stageFlags		?	stageFlags		> right.stageFlags		:
				binding			!= right.binding		?	binding			> right.binding			:
				uniqueIndex		!= right.uniqueIndex	?	uniqueIndex		< right.uniqueIndex		:
				size			!= right.size			?	size			> right.size			:
															name			> right.name;
	}
	
/*
=================================================
	UniformBuffer::operator <
=================================================
*/
	bool PipelineLayoutDescriptor::UniformBuffer::operator <  (const UniformBuffer &right) const
	{
		return	stageFlags		!= right.stageFlags		?	stageFlags		< right.stageFlags		:
				binding			!= right.binding		?	binding			< right.binding			:
				uniqueIndex		!= right.uniqueIndex	?	uniqueIndex		< right.uniqueIndex		:
				size			!= right.size			?	size			< right.size			:
															name			< right.name;
	}
//-----------------------------------------------------------------------------
			


/*
=================================================
	StorageBuffer::operator ==
=================================================
*/
	bool PipelineLayoutDescriptor::StorageBuffer::operator == (const StorageBuffer &right) const
	{
		return	stageFlags		== right.stageFlags		and
				binding			== right.binding		and
				uniqueIndex		== right.uniqueIndex	and
				staticSize		== right.staticSize		and
				arrayStride		== right.arrayStride	and
				writeAccess		== right.writeAccess	and
				readAccess		== right.readAccess		and
				name			== right.name;
	}
	
/*
=================================================
	StorageBuffer::operator >
=================================================
*/
	bool PipelineLayoutDescriptor::StorageBuffer::operator >  (const StorageBuffer &right) const
	{
		return	stageFlags		!= right.stageFlags		?	stageFlags		> right.stageFlags		:
				binding			!= right.binding		?	binding			> right.binding			:
				uniqueIndex		!= right.uniqueIndex	?	uniqueIndex		< right.uniqueIndex		:
				staticSize		!= right.staticSize		?	staticSize		> right.staticSize		:
				arrayStride		!= right.arrayStride	?	arrayStride		> right.arrayStride		:
				writeAccess		!= right.writeAccess	?	writeAccess		> right.writeAccess		:
				readAccess		!= right.readAccess		?	readAccess		> right.readAccess		:
															name			> right.name;
	}
	
/*
=================================================
	StorageBuffer::operator <
=================================================
*/
	bool PipelineLayoutDescriptor::StorageBuffer::operator <  (const StorageBuffer &right) const
	{
		return	stageFlags		!= right.stageFlags		?	stageFlags		< right.stageFlags		:
				binding			!= right.binding		?	binding			< right.binding			:
				uniqueIndex		!= right.uniqueIndex	?	uniqueIndex		< right.uniqueIndex		:
				staticSize		!= right.staticSize		?	staticSize		< right.staticSize		:
				arrayStride		!= right.arrayStride	?	arrayStride		< right.arrayStride		:
				writeAccess		!= right.writeAccess	?	writeAccess		< right.writeAccess		:
				readAccess		!= right.readAccess		?	readAccess		< right.readAccess		:
															name			< right.name;
	}
//-----------------------------------------------------------------------------
			


/*
=================================================
	PushConstant::operator ==
=================================================
*/
	bool PipelineLayoutDescriptor::PushConstant::operator == (const PushConstant &right) const
	{
		return	stageFlags	== right.stageFlags	and
				offset		== right.offset		and
				size		== right.size		and
				name		== right.name;
	}
	
/*
=================================================
	PushConstant::operator >
=================================================
*/
	bool PipelineLayoutDescriptor::PushConstant::operator >  (const PushConstant &right) const
	{
		return	stageFlags	!= right.stageFlags	?	stageFlags	> right.stageFlags	:
				offset		!= right.offset		?	offset		> right.offset		:
				size		!= right.size		?	size		> right.size		:
													name		> right.name;
	}
	
/*
=================================================
	PushConstant::operator <
=================================================
*/
	bool PipelineLayoutDescriptor::PushConstant::operator <  (const PushConstant &right) const
	{
		return	stageFlags	!= right.stageFlags	?	stageFlags	< right.stageFlags	:
				offset		!= right.offset		?	offset		< right.offset		:
				size		!= right.size		?	size		< right.size		:
													name		< right.name;
	}
//-----------------------------------------------------------------------------
	


/*
=================================================
	Uniform::operator <
=================================================
*
	bool PipelineLayoutDescriptor::Uniform::operator == (const Uniform &right) const
	{
		return	stageFlags		== right.stageFlags		and
				binding			== right.binding		and
				//uniqueIndex		== right.uniqueIndex	and
				valueType		== right.valueType		and
				arraySize		== right.arraySize		and
				name			== right.name;
	}

/*
=================================================
	Uniform::operator <
=================================================
*
	bool PipelineLayoutDescriptor::Uniform::operator >  (const Uniform &right) const
	{
		return	stageFlags		!= right.stageFlags		?	stageFlags		> right.stageFlags		:
				binding			!= right.binding		?	binding			> right.binding			:
				//uniqueIndex		!= right.uniqueIndex	?	uniqueIndex		< right.uniqueIndex		:
				valueType		!= right.valueType		?	valueType		> right.valueType		:
				arraySize		!= right.arraySize		?	arraySize		> right.arraySize		:
															name			> right.name;
	}

/*
=================================================
	Uniform::operator <
=================================================
*
	bool PipelineLayoutDescriptor::Uniform::operator <  (const Uniform &right) const
	{
		return	stageFlags		!= right.stageFlags		?	stageFlags		< right.stageFlags		:
				binding			!= right.binding		?	binding			< right.binding			:
				//uniqueIndex		!= right.uniqueIndex	?	uniqueIndex		< right.uniqueIndex		:
				valueType		!= right.valueType		?	valueType		< right.valueType		:
				arraySize		!= right.arraySize		?	arraySize		< right.arraySize		:
															name			< right.name;
	}
//-----------------------------------------------------------------------------



/*
=================================================
	AddTexture
=================================================
*/
	PipelineLayoutDescriptor::Builder&
		PipelineLayoutDescriptor::Builder::AddTexture (StringCRef name, EImage::type textureType, EPixelFormatClass::type format,
														uint binding, uint uniqueIndex, EShader::bits stageFlags)
	{
		TextureUniform	tex;
		tex.name			= name;
		tex.textureType		= textureType;
		tex.format			= format;
		tex.binding			= binding;
		tex.uniqueIndex		= uniqueIndex;
		tex.stageFlags		= stageFlags;

		_descr._uniforms.PushBack(Uniform_t( RVREF(tex) ));
		_changed = true;
		return *this;
	}
	
/*
=================================================
	AddImage
=================================================
*/
	PipelineLayoutDescriptor::Builder&
		PipelineLayoutDescriptor::Builder::AddImage (StringCRef name, EImage::type imageType, EPixelFormat::type format,
													 bool writeAccess, bool readAccess, uint binding, uint uniqueIndex, EShader::bits stageFlags)
	{
		ImageUniform	img;
		img.name			= name;
		img.imageType		= imageType;
		img.format			= format;
		img.writeAccess		= writeAccess;
		img.readAccess		= readAccess;
		img.binding			= binding;
		img.uniqueIndex		= uniqueIndex;
		img.stageFlags		= stageFlags;

		_descr._uniforms.PushBack(Uniform_t( RVREF(img) ));
		_changed = true;
		return *this;
	}
	
/*
=================================================
	AddUniformBuffer
=================================================
*/
	PipelineLayoutDescriptor::Builder&
		PipelineLayoutDescriptor::Builder::AddUniformBuffer (StringCRef name, BytesU size, uint binding, uint uniqueIndex, EShader::bits stageFlags)
	{
		UniformBuffer	buf;
		buf.name			= name;
		buf.size			= size;
		buf.binding			= binding;
		buf.uniqueIndex		= uniqueIndex;
		buf.stageFlags		= stageFlags;

		_descr._uniforms.PushBack(Uniform_t( RVREF(buf) ));
		_changed = true;
		return *this;
	}
		
/*
=================================================
	AddStorageBuffer
=================================================
*/
	PipelineLayoutDescriptor::Builder&
		PipelineLayoutDescriptor::Builder::AddStorageBuffer (StringCRef name, BytesU size, BytesU stride, bool writeAccess, bool readAccess,
															 uint binding, uint uniqueIndex, EShader::bits stageFlags)
	{
		StorageBuffer	buf;
		buf.name			= name;
		buf.staticSize		= BytesUL(size);
		buf.arrayStride		= BytesUL(stride);
		buf.writeAccess		= writeAccess;
		buf.readAccess		= readAccess;
		buf.binding			= binding;
		buf.uniqueIndex		= uniqueIndex;
		buf.stageFlags		= stageFlags;

		_descr._uniforms.PushBack(Uniform_t( RVREF(buf) ));
		_changed = true;
		return *this;
	}
	
/*
=================================================
	AddPushConstant
=================================================
*/
	PipelineLayoutDescriptor::Builder&
		PipelineLayoutDescriptor::Builder::AddPushConstant (StringCRef name, BytesU offset, BytesU size, EShader::bits stageFlags)
	{
		PushConstant	pc;
		pc.name			= name;
		pc.offset		= offset;
		pc.size			= size;
		pc.stageFlags	= stageFlags;

		_descr._uniforms.PushBack(Uniform_t( RVREF(pc) ));
		_changed = true;
		return *this;
	}
	
/*
=================================================
	AddPushConstantsBuffer
=================================================
*/
	PipelineLayoutDescriptor::Builder&
		PipelineLayoutDescriptor::Builder::AddPushConstantsBuffer (StringCRef name, BytesU size, uint binding, uint uniqueIndex, EShader::bits stageFlags)
	{
		PushConstantsBuffer	pcb;
		pcb.name		= name;
		pcb.binding		= binding;
		pcb.uniqueIndex	= uniqueIndex;
		pcb.size		= size;
		pcb.stageFlags	= stageFlags;

		_descr._uniforms.PushBack(Uniform_t( RVREF(pcb) ));
		_changed = true;
		return *this;
	}

/*
=================================================
	Finish
=================================================
*/
	PipelineLayoutDescriptor const&
		PipelineLayoutDescriptor::Builder::Finish ()
	{
		if ( _changed )
		{
			_changed	 = true;
			_descr._hash = HashOf( _descr );
		}
		return _descr;
	}
//-----------------------------------------------------------------------------
	


/*
=================================================
	operator ==
=================================================
*/
	bool PipelineLayoutDescriptor::operator == (const Self &right) const
	{
		return	_hash		== right._hash		and
				_uniforms	== right._uniforms;
	}
	
/*
=================================================
	operator >
=================================================
*/
	bool PipelineLayoutDescriptor::operator >  (const Self &right) const
	{
		return	_hash != right._hash	?	_hash		> right._hash	:
											_uniforms	> right._uniforms;
	}
	
/*
=================================================
	operator <
=================================================
*/
	bool PipelineLayoutDescriptor::operator <  (const Self &right) const
	{
		return	_hash != right._hash	?	_hash		< right._hash	:
											_uniforms	< right._uniforms;
	}
//-----------------------------------------------------------------------------
	
}	// Platforms
}	// Engine
