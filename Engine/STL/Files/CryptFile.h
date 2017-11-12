// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "SubFile.h"
#include "Engine/STL/Containers/StaticArray.h"
#include "Engine/STL/Experimental/Unroll.h"

namespace GX_STL
{
namespace File
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
		
	private:
		struct _RecursiveXor
		{
		private:
			const password_t &	pw;
			usize				pos;
			ubyte	&			c;

		public:
			_RecursiveXor (const password_t &pw, usize p, ubyte &c) : pw(pw), pos(p), c(c)
			{}

			void operator () (usize i)
			{
				c ^= pw[i] + (pos * i);
			}
		};


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
			_RecursiveXor	rx( _password, (usize)pos, c );
			UnrollLoop< password_t::STATIC_COUNT >::Run( rx );
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
		RCryptFile (const File::RFilePtr &file, const CryptAlgorithm &alg) :
		  SubRFile( file, file->Pos(), file->RemainingSize() ), _crypt(alg)
		{}


		RCryptFile (const File::RFilePtr &file, BytesU offset, BytesU size, const CryptAlgorithm &alg) :
			SubRFile( file, offset, size ), _crypt(alg)
		{}

			
		static RCryptFilePtr New (const File::RFilePtr &file, const CryptAlgorithm &alg)
		{
			return new Self( file, alg );
		}

			
		static RCryptFilePtr New (const File::RFilePtr &file, BytesU offset, BytesU size, const CryptAlgorithm &alg)
		{
			return new Self( file, offset, size, alg );
		}

		
		// RFile //
		virtual BytesU ReadBuf (void * buf, BytesU size) override
		{
			ubyte *	data = (ubyte *) buf;
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
		static SubRFilePtr New (const File::RFilePtr &file, BytesU offset, BytesU size) = delete;
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
		WCryptFile (const File::WFilePtr &file, const CryptAlgorithm &alg, bool restoreData = true) :
			SubWFile( file, file->Pos(), file->RemainingSize() ), _crypt(alg), _restoreData(restoreData)
		{}


		WCryptFile (const File::WFilePtr &file, BytesU offset, BytesU size, const CryptAlgorithm &alg, bool restoreData = true) :
			SubWFile( file, offset, size ), _crypt(alg), _restoreData(restoreData)
		{}
		

		static WCryptFilePtr New (const File::WFilePtr &file, const CryptAlgorithm &alg, bool restoreData = true)
		{
			return new Self( file, alg, restoreData );
		}
		

		static WCryptFilePtr New (const File::WFilePtr &file, BytesU offset, BytesU size, const CryptAlgorithm &alg, bool restoreData = true)
		{
			return new Self( file, offset, size, alg, restoreData );
		}

			
		// WFile //
		virtual BytesU WriteBuf (const void * buf, BytesU size) override
		{
			ubyte *	data = (ubyte *) buf;
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
		static SubWFilePtr New (const File::WFilePtr &file, BytesU offset, BytesU size) = delete;
	};
	

}	// File
}	// GX_STL
