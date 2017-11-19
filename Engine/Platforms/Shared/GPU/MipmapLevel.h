// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

namespace Engine
{
namespace Platforms
{

	//
	// Mipmap Level
	//
	
	struct MipmapLevel : CompileTime::PODStruct
	{
	// types
	private:
		using Self	= MipmapLevel;


	// variables
	private:
		uint		_value;


	// methods
	public:
		MipmapLevel (GX_DEFCTOR) : _value(0)
		{}

		explicit
		MipmapLevel (uint value) : _value(value)
		{}

		uint Get () const		{ return _value; }

		_GX_DIM_CMP_OPERATORS_SELF( _value )
	};



	//
	// Mipmap Levels Range
	//
	/*
	struct MipmapLevelsRange
	{
	// types
	private:
		using Self	= MipmapLevelsRange;


	// variables
	private:
		uint2		_value;


	// methods
	public:
		MipmapLevelsRange (GX_DEFCTOR) : _value(0)
		{}

		explicit
		MipmapLevelsRange (uint2 value) : _value(value)
		{}

		uint2 Get () const		{ return _value; }
	};*/


}	// Platforms
}	// Engine

namespace GX_STL
{
namespace GXTypes
{
	template <>
	struct Hash< Engine::Platforms::MipmapLevel >
	{
		using Result_t	= HashResult;

		Result_t operator () (const Engine::Platforms::MipmapLevel &value) const
		{
			return HashOf( value.Get() );
		}
	};

}	// GXTypes
}	// GX_STL
