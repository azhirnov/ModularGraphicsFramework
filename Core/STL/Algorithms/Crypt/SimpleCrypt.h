// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Containers/ArrayRef.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Simple Crypt Algorithm (XOR)
	//

	struct SimpleCryptAlgorithm
	{
		// source to encrypted
		template <typename T, typename B>
		static bool Encrypt (INOUT ArrayRef<T> data, ArrayCRef<B> password)
		{
			FOR( i, data )
			{
				FOR( j, password )
				{
					data[i] ^= ( password[j] + i*j );
				}
			}
			return true;
		}

		// encrypted to source
		template <typename T, typename B>
		static bool Decrypt (INOUT ArrayRef<T> data, ArrayCRef<B> password)
		{
			return Encrypt( data, password );
		}
	};



	//
	// Crypt
	//
	/*
	template <typename T, typename CryptAlgorithmType = SimpleCryptAlgorithm >
	struct Crypt
	{
	// types
	public:
		typedef CryptAlgorithmType	Algorithm;


	// methods
	public:
	};
	*/


}	// GXTypes
}	// GX_STL
