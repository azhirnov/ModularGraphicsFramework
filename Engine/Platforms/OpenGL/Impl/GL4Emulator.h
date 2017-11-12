// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenGL/Impl/gl4.h"

#if defined( GRAPHICS_API_OPENGL )

namespace gl
{
	
# if GRAPHICS_API_OPENGL >= 450

	inline void glBindMultiTexture (GLuint texunit, GLenum target, GLuint texture)
	{
		GX_UNUSED( target );
		return glBindTextureUnit( texunit, texture );
	}

	inline void glGenerateTextureMipmaps (GLenum target, GLuint texture)
	{
		GX_UNUSED( target );
		return glGenerateTextureMipmap( texture );
	}

	inline void glTextureParameterInt (GLuint texture, GLenum target, GLenum pname, GLint param)
	{
		GX_UNUSED( target );
		return glTextureParameteri( texture, pname, param );
	}

	inline void glGetImageFromTexture (GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels)
	{
		GX_UNUSED( target );
		return glGetTextureImage( texture, level, format, type, bufSize, pixels );
	}

	/*inline void glGetSubImageFromTexture (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset,
										  GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type,
  										  GLsizei bufSize, void *pixels)
	{
		GX_UNUSED( target );
		return glGetTextureSubImage( texture, level, xoffset, yoffset, zoffset,
									 width, height, depth, format, type, bufSize, pixels );
	}*/

# endif

# if GRAPHICS_API_OPENGL < 450

	// using extension https://www.khronos.org/registry/OpenGL/extensions/EXT/EXT_direct_state_access.txt
	
	// Buffer
	inline void glNamedBufferData (GLuint buffer, GLsizei size, const void *data, GLenum usage)
	{
		return glNamedBufferDataEXT( buffer, size, data, usage );
	}

	inline void glNamedBufferSubData (GLuint buffer, GLintptr offset, GLsizei size, const void *data)
	{
		return glNamedBufferSubDataEXT( buffer, offset, size, data );
	}
	
	inline void glGetNamedBufferSubData (GLuint buffer, GLintptr offset, GLsizei size, void *data)
	{
		return glGetNamedBufferSubDataEXT( buffer, offset, size, data );
	}

	// Vertex Attribs
	inline void glDisableVertexArrayAttrib (GLuint vaobj, GLuint index)
	{
		return glDisableVertexArrayAttribEXT( vaobj, index );
	}
	
	inline void glEnableVertexArrayAttrib (GLuint vaobj, GLuint index)
	{
		return glEnableVertexArrayAttribEXT( vaobj, index );
	}

	inline void glVertexArrayAttribBinding (GLuint vaobj, GLuint attribindex, GLuint bindingindex)
	{
		return glVertexArrayVertexAttribBindingEXT( vaobj, attribindex, bindingindex );
	}
	
	inline void glVertexArrayAttribFormat (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
	{
		return glVertexArrayVertexAttribFormatEXT( vaobj, attribindex, size, type, normalized, relativeoffset );
	}

	inline void glVertexArrayAttribIFormat (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
	{
		return glVertexArrayVertexAttribIFormatEXT( vaobj, attribindex, size, type, relativeoffset );
	}

	inline void glVertexArrayAttribLFormat (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
	{
		return glVertexArrayVertexAttribLFormatEXT( vaobj, attribindex, size, type, relativeoffset );
	}

	inline void glVertexArrayVertexBuffer (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
	{
		return glVertexArrayBindVertexBufferEXT( vaobj, bindingindex, buffer, offset, stride );
	}

	inline void glVertexArrayBindingDivisor (GLuint vaobj, GLuint bindingindex, GLuint divisor)
	{
		return glVertexArrayVertexBindingDivisorEXT( vaobj, bindingindex, divisor );
	}
	
	inline void glVertexArrayElementBuffer (GLuint vaobj, GLuint buffer)
	{
		TODO( "" );
		glBindVertexArray( vaobj );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer );	// TODO
	}
	
	// Framebuffer
	inline void glNamedFramebufferTexture (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)
	{
		return glNamedFramebufferTextureEXT( framebuffer, attachment, texture, level );
	}

	inline void glNamedFramebufferTextureLayer (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer)
	{
		return glNamedFramebufferTextureLayerEXT( framebuffer, attachment, texture, level, layer );
	}

	inline GLenum glCheckNamedFramebufferStatus (GLuint framebuffer, GLenum target)
	{
		return glCheckNamedFramebufferStatusEXT( framebuffer, target );
	}

	inline void glNamedFramebufferDrawBuffers (GLuint framebuffer, GLsizei n, const GLenum *bufs)
	{
		return glFramebufferDrawBuffersEXT( framebuffer, n, bufs );
	}

	/*inline void glNamedFramebufferReadBuffer (GLuint framebuffer, GLenum mode)
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, framebuffer );
		glReadBuffer( mode );
	}*/
	
	inline void glReadnPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data)
	{
		GX_UNUSED( bufSize );
		return glReadPixels( x, y, width, height, format, type, data );
	}
	
	// Texture
	inline void glBindMultiTexture (GLuint unit, GLenum target, GLuint texture)
	{
		return glBindMultiTextureEXT( unit + GL_TEXTURE0, target, texture );
	}
	
	inline void glGenerateTextureMipmaps (GLenum target, GLuint texture)
	{
		return glGenerateTextureMipmapEXT( target, texture );
	}
	
	inline void glTextureParameterInt (GLuint texture, GLenum target, GLenum pname, GLint param)
	{
		return glTextureParameteriEXT( texture, target, pname, param );
	}

	inline void glGetImageFromTexture (GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels)
	{
		GX_UNUSED( bufSize );
		return glGetTextureImageEXT( texture, target, level, format, type, pixels );
	}

# endif

}	// gl

#endif	// GRAPHICS_API_OPENGL
