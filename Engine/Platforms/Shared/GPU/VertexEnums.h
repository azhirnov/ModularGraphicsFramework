// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/PixelFormatEnums.h"

namespace Engine
{
namespace Platforms
{
	
	struct EVertexAttribute
	{
	private:
		using _vtypeinfo	= _platforms_hidden_::EValueTypeInfo;

	public:
		enum type : uint
		{
			NormalizedFlag	= _vtypeinfo::_NORM,

			Byte			= _vtypeinfo::Byte,
			Byte2			= _vtypeinfo::Byte2,
			Byte3			= _vtypeinfo::Byte3,
			Byte4			= _vtypeinfo::Byte4,
			
			Byte_Norm		= Byte  | NormalizedFlag,
			Byte2_Norm		= Byte2 | NormalizedFlag,
			Byte3_Norm		= Byte3 | NormalizedFlag,
			Byte4_Norm		= Byte4 | NormalizedFlag,

			UByte			= _vtypeinfo::UByte,
			UByte2			= _vtypeinfo::UByte2,
			UByte3			= _vtypeinfo::UByte3,
			UByte4			= _vtypeinfo::UByte4,
			
			UByte_Norm		= UByte  | NormalizedFlag,
			UByte2_Norm		= UByte2 | NormalizedFlag,
			UByte3_Norm		= UByte3 | NormalizedFlag,
			UByte4_Norm		= UByte4 | NormalizedFlag,
			
			Short			= _vtypeinfo::Short,
			Short2			= _vtypeinfo::Short2,
			Short3			= _vtypeinfo::Short3,
			Short4			= _vtypeinfo::Short4,
			
			Short_Norm		= Short  | NormalizedFlag,
			Short2_Norm		= Short2 | NormalizedFlag,
			Short3_Norm		= Short3 | NormalizedFlag,
			Short4_Norm		= Short4 | NormalizedFlag,
			
			UShort			= _vtypeinfo::UShort,
			UShort2			= _vtypeinfo::UShort2,
			UShort3			= _vtypeinfo::UShort3,
			UShort4			= _vtypeinfo::UShort4,
			
			UShort_Norm		= UShort   | NormalizedFlag,
			UShort2_Norm	= UShort2 | NormalizedFlag,
			UShort3_Norm	= UShort3 | NormalizedFlag,
			UShort4_Norm	= UShort4 | NormalizedFlag,
			
			Int				= _vtypeinfo::Int,
			Int2			= _vtypeinfo::Int2,
			Int3			= _vtypeinfo::Int3,
			Int4			= _vtypeinfo::Int4,
			
			Int_Norm		= Int  | NormalizedFlag,
			Int2_Norm		= Int2 | NormalizedFlag,
			Int3_Norm		= Int3 | NormalizedFlag,
			Int4_Norm		= Int4 | NormalizedFlag,
			
			UInt			= _vtypeinfo::UInt,
			UInt2			= _vtypeinfo::UInt2,
			UInt3			= _vtypeinfo::UInt3,
			UInt4			= _vtypeinfo::UInt4,
			
			UInt_Norm		= UInt  | NormalizedFlag,
			UInt2_Norm		= UInt2 | NormalizedFlag,
			UInt3_Norm		= UInt3 | NormalizedFlag,
			UInt4_Norm		= UInt4 | NormalizedFlag,
			
			Long			= _vtypeinfo::Long,
			Long2			= _vtypeinfo::Long2,
			Long3			= _vtypeinfo::Long3,
			Long4			= _vtypeinfo::Long4,
			
			Long_Norm		= Long  | NormalizedFlag,
			Long2_Norm		= Long2 | NormalizedFlag,
			Long3_Norm		= Long3 | NormalizedFlag,
			Long4_Norm		= Long4 | NormalizedFlag,
			
			ULong			= _vtypeinfo::ULong,
			ULong2			= _vtypeinfo::ULong2,
			ULong3			= _vtypeinfo::ULong3,
			ULong4			= _vtypeinfo::ULong4,
			
			ULong_Norm		= ULong  | NormalizedFlag,
			ULong2_Norm		= ULong2 | NormalizedFlag,
			ULong3_Norm		= ULong3 | NormalizedFlag,
			ULong4_Norm		= ULong4 | NormalizedFlag,
			
			Half			= _vtypeinfo::Half,
			Half2			= _vtypeinfo::Half2,
			Half3			= _vtypeinfo::Half3,
			Half4			= _vtypeinfo::Half4,

			Float			= _vtypeinfo::Float,
			Float2			= _vtypeinfo::Float2,
			Float3			= _vtypeinfo::Float3,
			Float4			= _vtypeinfo::Float4,
			
			Double			= _vtypeinfo::Double,
			Double2			= _vtypeinfo::Double2,
			Double3			= _vtypeinfo::Double3,
			Double4			= _vtypeinfo::Double4,

			Unknown			= ~0u,
		};

		static type  ToDstType (type src);

		static type  SetNormalized (type value, bool norm);

		struct ValueType
		{
			static bool IsInteger(type value);
			static bool IsFloat  (type value);
			static bool IsDouble (type value);
		};
	};
	

	struct EIndex
	{
		enum type : uint
		{
			UShort	= 0,
			UInt,

			_Count,
			Unknown		= ~0u,
		};

		template <typename T>	static constexpr type From ();

		static BytesU	Sizeof (type value);
	};
	

	struct EVertexInputRate
	{
		enum type : uint
		{
			Vertex	= 0,
			Instance,

			_Count,
			Unknown		= ~0u,
		};
	};


	
//-----------------------------------------------------------------------------//
// EVertexAttribute

	 inline EVertexAttribute::type  EVertexAttribute::ToDstType (type value)
	 {
		const type	count	= type( value & _vtypeinfo::_COL_MASK );
		const bool	norm	= EnumEq( value, _vtypeinfo::_NORM );
		const type	ftype	= type( _vtypeinfo::_FLOAT | count );

		ASSERT( count >= _vtypeinfo::_COL1 and count <= _vtypeinfo::_COL4 );

		switch ( value & (_vtypeinfo::_TYPE_MASK | _vtypeinfo::_UNSIGNED) )
		{
			case _vtypeinfo::_BYTE	:
			case _vtypeinfo::_SHORT	:
			case _vtypeinfo::_INT	:
				return norm ? ftype : type( _vtypeinfo::_INT | count );

			case _vtypeinfo::_UBYTE	:
			case _vtypeinfo::_USHORT	:
			case _vtypeinfo::_UINT	:
				return norm ? ftype : type( _vtypeinfo::_UINT | count );

			case _vtypeinfo::_HALF	:
			case _vtypeinfo::_FLOAT	:
				return type( _vtypeinfo::_FLOAT | count );

			case _vtypeinfo::_DOUBLE :
				return type( _vtypeinfo::_DOUBLE | count );
		}
			
		RETURN_ERR( "invalid attrib type", ftype );
	 }
	 
	inline EVertexAttribute::type  EVertexAttribute::SetNormalized (type value, bool norm)
	{
		return type( (value & ~NormalizedFlag) | (norm ? NormalizedFlag : 0) );
	}
	
	inline bool EVertexAttribute::ValueType::IsInteger (type value)
	{
		return	EnumEqMask( ToDstType( value ), _vtypeinfo::_INT,  _vtypeinfo::_TYPE_MASK ) or
				EnumEqMask( ToDstType( value ), _vtypeinfo::_UINT, _vtypeinfo::_TYPE_MASK );
	}

	inline bool EVertexAttribute::ValueType::IsFloat (type value)
	{
		return EnumEqMask( ToDstType( value ), _vtypeinfo::_FLOAT, _vtypeinfo::_TYPE_MASK );
	}

	inline bool EVertexAttribute::ValueType::IsDouble (type value)
	{
		return EnumEqMask( ToDstType( value ), _vtypeinfo::_DOUBLE, _vtypeinfo::_TYPE_MASK );
	}
		

//-----------------------------------------------------------------------------//
// EIndex

	template <>	inline constexpr EIndex::type  EIndex::From< ushort > ()	{ return UShort; }
	template <>	inline constexpr EIndex::type  EIndex::From< uint > ()		{ return UInt; }
	
	inline BytesU EIndex::Sizeof (type value)
	{
		switch ( value )
		{
			case EIndex::UShort :	return BytesU::SizeOf<ushort>();
			case EIndex::UInt :		return BytesU::SizeOf<uint>();
		}
		RETURN_ERR( "unknown index type!" );
	}

}	// Platforms
}	// Engine
