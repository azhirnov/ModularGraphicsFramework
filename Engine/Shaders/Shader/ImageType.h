// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Shader/NodeTypes.h"

namespace ShaderEditor
{
namespace ShaderNodes
{

	//
	// ReadOnly
	//

	template <typename T>
	struct ReadOnly : _ShaderNodesHidden_::AccessType< true, false >
	{
		static const char*	Name ()		{ return "RO"; }
	};



	//
	// WriteOnly
	//

	template <typename T>
	struct WriteOnly : _ShaderNodesHidden_::AccessType< false, true >
	{
		static const char*	Name ()		{ return "WO"; }
	};



	//
	// Coherent
	//

	template <typename T>
	struct Coherent : _ShaderNodesHidden_::AccessType< true, true >
	{
		static const char*	Name ()		{ return "Coh"; }
	};



	//
	// Volatile
	//

	template <typename T>
	struct Volatile : _ShaderNodesHidden_::AccessType< true, true >
	{
		static const char*	Name ()		{ return "Vol"; }
	};



	//
	// Restrict
	//

	template <typename T>
	struct Restrict : _ShaderNodesHidden_::AccessType< true, true >
	{
		static const char*	Name ()		{ return "Res"; }
	};



	//
	// ImageType1D
	//
	
	template <typename T>
	struct ImageType1D
	{
		using IntCoord_t	= Int;
		using FpCoord_t		= Float;

		static const bool	Multisample	= false;

		static const char*	Name ()		{ return "1D"; }
	};



	//
	// ImageType1DArray
	//
	
	template <typename T>
	struct ImageType1DArray
	{
		using IntCoord_t	= Int2;
		using FpCoord_t		= Float2;

		static const bool	Multisample	= false;

		static const char*	Name ()		{ return "1DA"; }
	};



	//
	// ImageType2D
	//
	
	template <typename T>
	struct ImageType2D
	{
		using IntCoord_t	= Int2;
		using FpCoord_t		= Float2;

		static const bool	Multisample	= false;

		static const char*	Name ()		{ return "2D"; }
	};



	//
	// ImageType2DArray
	//
	
	template <typename T>
	struct ImageType2DArray
	{
		using IntCoord_t	= Int3;
		using FpCoord_t		= Float3;

		static const bool	Multisample	= false;

		static const char*	Name ()		{ return "2DA"; }
	};



	//
	// ImageType2DMS
	//
	
	template <typename T>
	struct ImageType2DMS
	{
		using IntCoord_t	= Int2;
		using FpCoord_t		= Float2;

		static const bool	Multisample	= true;

		static const char*	Name ()		{ return "2DMS"; }
	};



	//
	// ImageType2DMSArray
	//
	
	template <typename T>
	struct ImageType2DMSArray
	{
		using IntCoord_t	= Int3;
		using FpCoord_t		= Float3;

		static const bool	Multisample	= true;

		static const char*	Name ()		{ return "2DMSA"; }
	};



	//
	// ImageTypeCube
	//
	
	template <typename T>
	struct ImageTypeCube
	{
		using IntCoord_t	= Int3;
		using FpCoord_t		= Float3;

		static const bool	Multisample	= false;

		static const char*	Name ()		{ return "Cube"; }
	};



	//
	// ImageTypeCubeArray
	//
	
	template <typename T>
	struct ImageTypeCubeArray
	{
		using IntCoord_t	= Int3;
		using FpCoord_t		= Float3;

		static const bool	Multisample	= false;

		static const char*	Name ()		{ return "CubeA"; }
	};



	//
	// ImageType3D
	//
	
	template <typename T>
	struct ImageType3D
	{
		using IntCoord_t	= Int3;
		using FpCoord_t		= Float3;

		static const bool	Multisample	= false;

		static const char*	Name ()		{ return "3D"; }
	};



	//
	// Pixel Format
	//
	/*
	template <typename T, bool Norm = false>
	struct PixelFormat final : _ShaderNodesHidden_::PixelFormat< T, Norm >
	{
		using Result_t		= GX_STL::CompileTime::SwitchType< IsNormalized or IsFloat, Float4,
								GX_STL::CompileTime::SwitchType< IsSignedInteger, Int4,
									GX_STL::CompileTime::SwitchType< IsUnsignedInteger, UInt4,
																		void > > >;
	};

	using R32F			= PixelFormat< float >;
	using RG32F			= PixelFormat< GX_STL::GXMath::float2 >;
	using RGB32F		= PixelFormat< GX_STL::GXMath::float3 >;
	using RGBA32F		= PixelFormat< GX_STL::GXMath::float4 >;

	using R16F			= PixelFormat< GX_STL::GXMath::half >;
	using RG16F			= PixelFormat< GX_STL::GXMath::half2 >;
	using RGB16F		= PixelFormat< GX_STL::GXMath::half3 >;
	using RGBA16F		= PixelFormat< GX_STL::GXMath::half4 >;

	using R32I			= PixelFormat< int >;
	using RG32I			= PixelFormat< GX_STL::GXMath::int2 >;
	using RGB32I		= PixelFormat< GX_STL::GXMath::int3 >;
	using RGBA32I		= PixelFormat< GX_STL::GXMath::int4 >;

	using R32U			= PixelFormat< GX_STL::GXMath::uint >;
	using RG32U			= PixelFormat< GX_STL::GXMath::uint2 >;
	using RGB32U		= PixelFormat< GX_STL::GXMath::uint3 >;
	using RGBA32U		= PixelFormat< GX_STL::GXMath::uint4 >;

	using R16I			= PixelFormat< short >;
	using RG16I			= PixelFormat< GX_STL::GXMath::short2 >;
	using RGB16I		= PixelFormat< GX_STL::GXMath::short3 >;
	using RGBA16I		= PixelFormat< GX_STL::GXMath::short4 >;

	using R16U			= PixelFormat< GX_STL::GXMath::ushort >;
	using RG16U			= PixelFormat< GX_STL::GXMath::ushort2 >;
	using RGB16U		= PixelFormat< GX_STL::GXMath::ushort3 >;
	using RGBA16U		= PixelFormat< GX_STL::GXMath::ushort4 >;

	using R8I			= PixelFormat< GX_STL::GXMath::byte >;
	using RG8I			= PixelFormat< GX_STL::GXMath::byte2 >;
	using RGB8I			= PixelFormat< GX_STL::GXMath::byte3 >;
	using RGBA8I		= PixelFormat< GX_STL::GXMath::byte4 >;

	using R8U			= PixelFormat< GX_STL::GXMath::ubyte >;
	using RG8U			= PixelFormat< GX_STL::GXMath::ubyte2 >;
	using RGB8U			= PixelFormat< GX_STL::GXMath::ubyte3 >;
	using RGBA8U		= PixelFormat< GX_STL::GXMath::ubyte4 >;

	using R8_UNorm		= PixelFormat< GX_STL::GXMath::ubyte, true >;
	using RG8_UNorm		= PixelFormat< GX_STL::GXMath::ubyte2, true >;
	using RGB8_UNorm	= PixelFormat< GX_STL::GXMath::ubyte3, true >;
	using RGBA8_UNorm	= PixelFormat< GX_STL::GXMath::ubyte4, true >;

	using R8_SNorm		= PixelFormat< GX_STL::GXMath::byte, true >;
	using RG8_SNorm		= PixelFormat< GX_STL::GXMath::byte2, true >;
	using RGB8_SNorm	= PixelFormat< GX_STL::GXMath::byte3, true >;
	using RGBA8_SNorm	= PixelFormat< GX_STL::GXMath::byte4, true >;

	using R16_UNorm		= PixelFormat< GX_STL::GXMath::ushort, true >;
	using RG16_UNorm	= PixelFormat< GX_STL::GXMath::ushort2, true >;
	using RGB16_UNorm	= PixelFormat< GX_STL::GXMath::ushort3, true >;
	using RGBA16_UNorm	= PixelFormat< GX_STL::GXMath::ushort4, true >;

	using R16_SNorm		= PixelFormat< short, true >;
	using RG16_SNorm	= PixelFormat< GX_STL::GXMath::short2, true >;
	using RGB16_SNorm	= PixelFormat< GX_STL::GXMath::short3, true >;
	using RGBA16_SNorm	= PixelFormat< GX_STL::GXMath::short4, true >;
	*/

	

	//
	// Image
	//

	template <	template <typename> class ImageType,
				template <typename> class AccessType,
				typename PixelType
			 >
	struct Image : Node
	{
	// types
	private:
		using Self			= Image< ImageType, AccessType, PixelType >;
		using Access_t		= AccessType<PixelType>;
		using Pixel_t		= PixelType;
		using Image_t		= ImageType<PixelType>;
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

	struct Sampler : Node
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

		static Name_t  _Typename ()		{ return "sampler"; }
	};



	//
	// Texture
	//
	
	template <	template <typename> class ImageType,
				typename PixelType
			 >
	struct Texture : Node
	{
	// types
	private:
		using Self			= Texture< ImageType, PixelType >;
		using Pixel_t		= PixelType;
		using IntCoord_t	= typename ImageType<PixelType>::IntCoord_t;
		using FpCoord_t		= typename ImageType<PixelType>::FpCoord_t;
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

		static Name_t  _Typename ()		{ return "texture"; }
	};



	//
	// Buffer
	//
	
	template <	template <typename> class AccessType,
				typename DataType
			 >
	struct Buffer : Node
	{
	// types
	private:
		using Self		= Buffer< AccessType, DataType >;
		using Data_t	= DataType;
		using ModulePtr	= Engine::Base::ModulePtr;
		

	// methods
	public:
		Buffer (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Buffer )
		{}

		Self &	operator = (const ModulePtr &value);

		DataType const&		Read ()		const;
		DataType &			Write ()	const;

		static Name_t  _Typename ()		{ return "buffer"; }
	};



	//
	// PushConstants
	//

	template <typename T>
	struct PushConstants : Node
	{
	// types
	public:
		using Self	= PushConstants<T>;


	// variables
	private:
		T		_data;


	// methods
	public:
		PushConstants (Node *parent, StringCRef name) :
			Node( parent, name, "push_constants", ENodeType::PushConstants ),
			_data{ this, "data" }
		{}

		T const *	operator -> () const	{ return &_data; }
		T *			operator -> ()			{ return &_data; }
	};


}	// ShaderNodes
}	// ShaderEditor
