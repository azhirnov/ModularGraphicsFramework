// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/ShaderLang/SWLangCommon.h"

#ifdef GRAPHICS_API_SOFT

namespace SWShaderLang
{
namespace Impl
{

	//
	// Base Image
	//
	struct BaseImage
	{
	// types
	protected:
		using EPixelFormat	= Engine::Platforms::EPixelFormat;
		using MemLayout_t	= Engine::GpuMsg::GetSWImageMemoryLayout::ImgLayers3D;
		using LoadFunc_t	= void (*) (const MemLayout_t &memLayout, const int3 &coord, OUT void *texel);
		using StoreFunc_t	= void (*) (MemLayout_t &memLayout, const int3 &coord, const void* texel);

		enum class EOutputFormat
		{
			Float4,
			Int4,
			UInt4,
		};


	// variables
	private:
		LoadFunc_t		_load		= null;
		StoreFunc_t		_store		= null;
		MemLayout_t		_memLayout;


	// methods
	protected:
		template <typename T>
		BaseImage (MemLayout_t &&memLayout, UninitializedT<T>);
		BaseImage (MemLayout_t &&memLayout, EOutputFormat outFmt);
		
		BaseImage () {}

		GX_DEFCOPYCTOR_ASSIGN( BaseImage );

		void _Load (const int3 &coord, OUT void *texel) const;
		void _Store (const int3 &coord, const void* texel);

	private:
		bool _Init (EOutputFormat outFmt);

		template <typename SrcColor, typename DstColor>
		static void _LoadTexel (const MemLayout_t &memLayout, const int3 &coord, OUT void *texel);
		
		template <typename DstColor, typename SrcColor>
		static void _StoreTexel (MemLayout_t &memLayout, const int3 &coord, const void* texel);

	public:
		int3	Size () const;
		int		Samples () const;
	};


	template <>	inline BaseImage::BaseImage (MemLayout_t &&memLayout, UninitializedT<glm::vec4>)  : BaseImage{ RVREF(memLayout), EOutputFormat::Float4 } {}
	template <>	inline BaseImage::BaseImage (MemLayout_t &&memLayout, UninitializedT<glm::ivec4>) : BaseImage{ RVREF(memLayout), EOutputFormat::Int4 }   {}
	template <>	inline BaseImage::BaseImage (MemLayout_t &&memLayout, UninitializedT<glm::uvec4>) : BaseImage{ RVREF(memLayout), EOutputFormat::UInt4 }  {}
	
	inline void BaseImage::_Load (const int3 &coord, OUT void *texel) const	{ ASSERT( _load );  return _load( _memLayout, coord, OUT texel ); }
	inline void BaseImage::_Store (const int3 &coord, const void* texel)	{ ASSERT( _store ); return _store( _memLayout, coord, texel ); }
//-----------------------------------------------------------------------------



	//
	// Image 2D
	//
	template <	typename T,
				EStorageAccess::type Access
			 >
	struct Image2D final : public BaseImage
	{
		friend class SWShaderHelper;

	// methods
	protected:
		explicit Image2D (MemLayout_t &&memLayout) : BaseImage{ RVREF(memLayout), UninitializedT<T>() } {}
		
		Image2D& operator = (const Image2D &) = default;
		Image2D& operator = (Image2D &&) = default;

	public:
		Image2D () {}

		ND_  T	Load (const int2 &point) const;
		void	Store (const int2 &point, const T &value);

		// TODO: atomics
	};
	

	template <typename T, EStorageAccess::type A>
	inline T  Image2D<T,A>::Load (const int2 &point) const
	{
		STATIC_ASSERT( EStorageAccess::HasReadAccess(A) );
		T res;
		_Load( point.xyo(), OUT &res );
		return res;
	}
		
	template <typename T, EStorageAccess::type A>
	inline void  Image2D<T,A>::Store (const int2 &point, const T &value)
	{
		STATIC_ASSERT( EStorageAccess::HasWriteAccess(A) );
		_Store( point.xyo(), &value );
	}
//-----------------------------------------------------------------------------



	//
	// Image 2D Array
	//
	template <	typename T,
				EStorageAccess::type Access
			 >
	struct Image2DArray final : public BaseImage
	{
		friend class SWShaderHelper;
		
	// methods
	protected:
		explicit Image2DArray (MemLayout_t &&memLayout) : BaseImage{ RVREF(memLayout), UninitializedT<T>() } {}
		
		Image2DArray& operator = (const Image2DArray &) = default;
		Image2DArray& operator = (Image2DArray &&) = default;

	public:
		Image2DArray () {}

		ND_ T	Load (const int3 &point) const;
		void	Store (const int3 &point, const T &value);

		// TODO: atomics
	};
	

	template <typename T, EStorageAccess::type A>
	inline T  Image2DArray<T,A>::Load (const int3 &point) const
	{
		STATIC_ASSERT( EStorageAccess::HasReadAccess(A) );
		T res;
		_Load( point, OUT &res );
		return res;
	}
		
	template <typename T, EStorageAccess::type A>
	inline void  Image2DArray<T,A>::Store (const int3 &point, const T &value)
	{
		STATIC_ASSERT( EStorageAccess::HasWriteAccess(A) );
		_Store( point, &value );
	}
//-----------------------------------------------------------------------------



	//
	// Image 3D
	//
	template <	typename T,
				EStorageAccess::type Access
			 >
	struct Image3D final : public BaseImage
	{
		friend class SWShaderHelper;
		
	// methods
	protected:
		explicit Image3D (MemLayout_t &&memLayout) : BaseImage{ RVREF(memLayout), UninitializedT<T>() } {}
		
		Image3D& operator = (const Image3D &) = default;
		Image3D& operator = (Image3D &&) = default;

	public:
		Image3D () {}

		ND_ T	Load (const int3 &point) const;
		void	Store (const int3 &point, const T &value);

		// TODO: atomics
	};
	

	template <typename T, EStorageAccess::type A>
	inline T  Image3D<T,A>::Load (const int3 &point) const
	{
		STATIC_ASSERT( EStorageAccess::HasReadAccess(A) );
		T res;
		_Load( point, OUT &res );
		return res;
	}
		
	template <typename T, EStorageAccess::type A>
	inline void  Image3D<T,A>::Store (const int3 &point, const T &value)
	{
		STATIC_ASSERT( EStorageAccess::HasWriteAccess(A) );
		_Store( point, &value );
	}
//-----------------------------------------------------------------------------


}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
