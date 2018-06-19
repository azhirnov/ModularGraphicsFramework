// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Files/SubFile.h"
#include "Core/STL/Containers/StaticArray.h"

namespace GX_STL
{
namespace GXFile
{

	//
	// Simple File Crypt Algorithm
	//

	template <uint Size>
	struct SimpleFileCryptAlgorithm
	{
	// types
	public:
		typedef StaticArray< ubyte, Size >			password_t;
		typedef SimpleFileCryptAlgorithm< Size >	Self;


	// variables
	private:
		password_t	_password;


	// methods
	public:
		SimpleFileCryptAlgorithm (StringCRef password) : _password(0)
		{
			for (usize i = 0; i < password.Length() and i < _password.Count(); ++i) {
				_password[i] = password[i];
			}
		}

		SimpleFileCryptAlgorithm (BinArrayCRef password) : _password(0)
		{
			for (usize i = 0; i < password.Count() and i < _password.Count(); ++i) {
				_password[i] = password[i];
			}
		}

		void Encrypt (BytesU pos, INOUT ubyte &c) const
		{
			FOR( i, _password )
			{
				c ^= _password[i] + (pos * i);
			}
		}

		void Decrypt (BytesU pos, INOUT ubyte &c) const
		{
			Encrypt( pos, c );
		}
	};



	//
	// Read only Crypted Sub File
	//

	template <typename A>
	class RCryptFile : public SubRFile
	{
	// types
	public:
		typedef A							CryptAlgorithm;
		typedef RCryptFile<A>				Self;
		typedef SharedPointerType<Self>		RCryptFilePtr;


	// variables
	private:
		CryptAlgorithm	_crypt;


	// methods
	public:
		RCryptFile (const RFilePtr &file, const CryptAlgorithm &alg) :
		  SubRFile( file, file->Pos(), file->RemainingSize() ), _crypt(alg)
		{}


		RCryptFile (const RFilePtr &file, BytesU offset, BytesU size, const CryptAlgorithm &alg) :
			SubRFile( file, offset, size ), _crypt(alg)
		{}

			
		ND_ static RCryptFilePtr New (const RFilePtr &file, const CryptAlgorithm &alg)
		{
			return new Self( file, alg );
		}

			
		ND_ static RCryptFilePtr New (const RFilePtr &file, BytesU offset, BytesU size, const CryptAlgorithm &alg)
		{
			return new Self( file, offset, size, alg );
		}

		
		// RFile //
		virtual BytesU ReadBuf (void * buf, BytesU size) noexcept override
		{
			ubyte *	data = Cast<ubyte *>(buf);
			BytesU	pos  = Pos();
			BytesU	r	 = SubRFile::ReadBuf( buf, size );

			if ( r > 0 )
			{
				for (usize i = 0; i < usize(r); ++i) {
					_crypt.Decrypt( pos + i, data[i] );
				}
			}

			return r;
		}

		
		// BaseFile //
		virtual EFile::type GetType () const override { return EFile::Crypted; }


	private:
		static SubRFilePtr New (const RFilePtr &file, BytesU offset, BytesU size) = delete;
	};



	//
	// Write only Crypted Sub File
	//

	template <typename A>
	class WCryptFile : public SubWFile
	{
	// types
	public:
		typedef A							CryptAlgorithm;
		typedef WCryptFile<A>				Self;
		typedef SharedPointerType<Self>		WCryptFilePtr;


	// variables
	private:
		CryptAlgorithm	_crypt;
		bool			_restoreData;


	// methods
	public:
		WCryptFile (const WFilePtr &file, const CryptAlgorithm &alg, bool restoreData = true) :
			SubWFile( file, file->Pos(), file->RemainingSize() ), _crypt(alg), _restoreData(restoreData)
		{}


		WCryptFile (const WFilePtr &file, BytesU offset, BytesU size, const CryptAlgorithm &alg, bool restoreData = true) :
			SubWFile( file, offset, size ), _crypt(alg), _restoreData(restoreData)
		{}
		

		ND_ static WCryptFilePtr New (const WFilePtr &file, const CryptAlgorithm &alg, bool restoreData = true)
		{
			return new Self( file, alg, restoreData );
		}
		

		ND_ static WCryptFilePtr New (const WFilePtr &file, BytesU offset, BytesU size, const CryptAlgorithm &alg, bool restoreData = true)
		{
			return new Self( file, offset, size, alg, restoreData );
		}

			
		// WFile //
		virtual BytesU WriteBuf (const void * buf, BytesU size) noexcept override
		{
			ubyte *	data = Cast<ubyte *>(buf);
			BytesU	pos  = Pos();

			for (usize i = 0; i < size; ++i) {
				_crypt.Encrypt( pos + i, data[i] );
			}

			BytesU	w = SubWFile::WriteBuf( buf, size );

			if ( _restoreData )
			{
				for (usize i = 0; i < size; ++i) {
					_crypt.Decrypt( pos + i, data[i] );
				}
			}
			return w;
		}

		
		// BaseFile //
		virtual EFile::type GetType () const override { return EFile::Crypted; }


	private:
		static SubWFilePtr New (const WFilePtr &file, BytesU offset, BytesU size) = delete;
	};
	

}	// GXFile
}	// GX_STL
