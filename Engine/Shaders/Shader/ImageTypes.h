// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Shader/VecTypes.h"

namespace ShaderEditor
{
namespace ShaderNodes
{

	//
	// ReadOnly
	//

	struct ReadOnly final : _ShaderNodesHidden_::AccessType< true, false >
	{
		static const char*	Name ()		{ return "RO"; }
	};



	//
	// WriteOnly
	//

	struct WriteOnly final : _ShaderNodesHidden_::AccessType< false, true >
	{
		static const char*	Name ()		{ return "WO"; }
	};



	//
	// Coherent
	//

	struct Coherent final : _ShaderNodesHidden_::AccessType< true, true >
	{
		static const char*	Name ()		{ return "Coh"; }
	};



	//
	// Volatile
	//

	struct Volatile final : _ShaderNodesHidden_::AccessType< true, true >
	{
		static const char*	Name ()		{ return "Vol"; }
	};



	//
	// Restrict
	//

	struct Restrict final : _ShaderNodesHidden_::AccessType< true, true >
	{
		static const char*	Name ()		{ return "Res"; }
	};



	//
	// ImageType1D
	//
	
	struct ImageType1D final
	{
		using IntCoord_t	= Int;
		using FpCoord_t		= Float;

		static const uint	Dimension	= 1;
		static const bool	IsCubeMap	= false;
		static const bool	IsArray		= false;
		static const bool	Multisample	= false;

		static const char*	Name ()		{ return "1D"; }
	};



	//
	// ImageType1DArray
	//
	
	struct ImageType1DArray final
	{
		using IntCoord_t	= Int2;
		using FpCoord_t		= Float2;
		
		static const uint	Dimension	= 1;
		static const bool	IsCubeMap	= false;
		static const bool	IsArray		= true;
		static const bool	Multisample	= false;

		static const char*	Name ()		{ return "1DA"; }
	};



	//
	// ImageType2D
	//
	
	struct ImageType2D final
	{
		using IntCoord_t	= Int2;
		using FpCoord_t		= Float2;
		
		static const uint	Dimension	= 2;
		static const bool	IsCubeMap	= false;
		static const bool	IsArray		= false;
		static const bool	Multisample	= false;

		static const char*	Name ()		{ return "2D"; }
	};



	//
	// ImageType2DArray
	//
	
	struct ImageType2DArray final
	{
		using IntCoord_t	= Int3;
		using FpCoord_t		= Float3;
		
		static const uint	Dimension	= 2;
		static const bool	IsCubeMap	= false;
		static const bool	IsArray		= true;
		static const bool	Multisample	= false;

		static const char*	Name ()		{ return "2DA"; }
	};



	//
	// ImageType2DMS
	//
	
	struct ImageType2DMS final
	{
		using IntCoord_t	= Int2;
		using FpCoord_t		= Float2;
		
		static const uint	Dimension	= 2;
		static const bool	IsCubeMap	= false;
		static const bool	IsArray		= false;
		static const bool	Multisample	= true;

		static const char*	Name ()		{ return "2DMS"; }
	};



	//
	// ImageType2DMSArray
	//
	
	struct ImageType2DMSArray final
	{
		using IntCoord_t	= Int3;
		using FpCoord_t		= Float3;
		
		static const uint	Dimension	= 2;
		static const bool	IsCubeMap	= false;
		static const bool	IsArray		= true;
		static const bool	Multisample	= true;

		static const char*	Name ()		{ return "2DMSA"; }
	};



	//
	// ImageTypeCube
	//
	
	struct ImageTypeCube final
	{
		using IntCoord_t	= Int3;
		using FpCoord_t		= Float3;
		
		static const uint	Dimension	= 2;
		static const bool	IsCubeMap	= true;
		static const bool	IsArray		= false;
		static const bool	Multisample	= false;

		static const char*	Name ()		{ return "Cube"; }
	};



	//
	// ImageTypeCubeArray
	//
	
	struct ImageTypeCubeArray final
	{
		using IntCoord_t	= Int3;
		using FpCoord_t		= Float3;
		
		static const uint	Dimension	= 2;
		static const bool	IsCubeMap	= true;
		static const bool	IsArray		= true;
		static const bool	Multisample	= false;

		static const char*	Name ()		{ return "CubeA"; }
	};



	//
	// ImageType3D
	//
	
	struct ImageType3D final
	{
		using IntCoord_t	= Int3;
		using FpCoord_t		= Float3;
		
		static const uint	Dimension	= 3;
		static const bool	IsCubeMap	= false;
		static const bool	IsArray		= false;
		static const bool	Multisample	= false;

		static const char*	Name ()		{ return "3D"; }
	};

	

	//
	// Image
	//

	template <typename ImageType, typename AccessType, typename PixelType>
	struct Image final : Node
	{
	// types
	public:
		using Self			= Image< ImageType, AccessType, PixelType >;
		using Access_t		= AccessType;
		using Pixel_t		= PixelType;
		using Image_t		= ImageType;
		using FpCoord_t		= typename Image_t::FpCoord_t;
		using IntCoord_t	= typename Image_t::IntCoord_t;
		using ModulePtr		= Engine::Base::ModulePtr;
		using Data_t		= UInt;
		

	// methods
	public:
		Image (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Image )
		{}

		Self &	operator = (const ModulePtr &value);

		Pixel_t	Load (const IntCoord_t &coord) const;
		void	Store (const IntCoord_t &coord, const Pixel_t &value) const;

		void AtomicAdd (const IntCoord_t &coord, const Data_t &data) const;
		void AtomicExchange (const IntCoord_t &coord, const Data_t &data) const;

		void AtomicMax (const IntCoord_t &coord, const Data_t &data) const;
		void AtomicMin (const IntCoord_t &coord, const Data_t &data) const;

		void AtomicAnd (const IntCoord_t &coord, const Data_t &data) const;
		void AtomicOr (const IntCoord_t &coord, const Data_t &data) const;
		void AtomicXor (const IntCoord_t &coord, const Data_t &data) const;

		Int			Samples () const;
		IntCoord_t	Size () const;

		static Name_t  _Typename ();
	};



	//
	// Sampler
	//

	struct Sampler final : Node
	{
	// types
	private:
		using Self			= Sampler;
		using ModulePtr		= Engine::Base::ModulePtr;


	// methods
	public:
		Sampler (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Sampler )
		{}

		Self &	operator = (const ModulePtr &value);

		static Name_t  _Typename ()		{ return "Sampler"; }
	};



	//
	// Texture
	//
	
	template <typename ImageType, typename PixelType>
	struct Texture final : Node
	{
	// types
	private:
		using Self			= Texture< ImageType, PixelType >;
		using Pixel_t		= PixelType;
		using IntCoord_t	= typename ImageType::IntCoord_t;
		using FpCoord_t		= typename ImageType::FpCoord_t;
		using Image_t		= Image< ImageType, ReadOnly, PixelType >;


	// variables
	public:
		Image_t		image	{ this, "image" };
		Sampler		sampler	{ this, "sampler" };


	// methods
	public:
		Texture (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Struct )
		{}

		Pixel_t	Sample (const FpCoord_t &point) const;
		Pixel_t SampleOffset (const FpCoord_t &point, const IntCoord_t &offset) const;
		Pixel_t SampleLod (const FpCoord_t &point, const Float &lod) const;
		Pixel_t SampleLodOffset (const FpCoord_t &point, const Float &lod, const IntCoord_t &offset) const;
		Pixel_t SampleGrad (const FpCoord_t &point, const FpCoord_t &dPdx, const FpCoord_t &dPdy) const;
		Pixel_t SampleGradOffset (const FpCoord_t &point, const FpCoord_t &dPdx, const FpCoord_t &dPdy, const IntCoord_t &offset) const;
		Pixel_t SampleGather (const FpCoord_t &point) const;
		Pixel_t SampleGatherOffset (const FpCoord_t &point, const IntCoord_t &offset) const;

		Pixel_t SampleProj (const Float4 &point) const;
		Pixel_t SampleProjOffset (const Float4 &point, const IntCoord_t &offset) const;
		Pixel_t SampleProjLod (const Float4 &point, const Float &lod) const;
		Pixel_t SampleProjLodOffset (const Float4 &point, const Float &lod, const IntCoord_t &offset) const;
		Pixel_t SampleProjGrad (const Float4 &point, const FpCoord_t &dPdx, const FpCoord_t &dPdy) const;
		Pixel_t SampleProjGradOffset (const Float4 &point, const FpCoord_t &dPdx, const FpCoord_t &dPdy, const IntCoord_t &offset) const;

		Int			Samples () const;
		IntCoord_t	Size (const Int &lod) const;
		FpCoord_t	QueryLod (const FpCoord_t &point) const;
		Int			QueryLevels () const;

		static Name_t  _Typename ()		{ return "Texture"; }
	};



	//
	// Sample
	//

	struct Sample
	{
	// variables
		Int		value;

	// methods
		explicit Sample (Int &&val) : value(RVREF(val))
		{}
	};


}	// ShaderNodes
}	// ShaderEditor
