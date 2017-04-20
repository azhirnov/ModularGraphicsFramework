// Copyright © 2014-2016  Zhirnov Andrey. All rights reserved.

#pragma once

#include "TypeId.h"

namespace GX_STL
{
namespace GXTypes
{

	/*
	//
	// Variant
	//

	struct TVariant : public CompileTime::FastCopyable
	{
	// types
	private:
		struct IValue
		{
		private:
			TypeId_t const	_id;
			
		public:
			IValue (TypeId_t id): _id(id) {}
			virtual ~IValue () {}
			TypeId_t				Id ()		const	{ return _id; }
			virtual IValue *		Copy ()		const = 0;
			virtual const void *	Ptr ()		const = 0;
			virtual void *			Ptr () = 0;
			virtual usize			SizeOf ()	const = 0;
		};


		template <typename T>
		struct TValue : IValue
		{
		private:
			T		_value;
	
		public:
			TValue () : _value(), IValue( TypeId<T>() ) {}
			TValue (const T& v) : _value(v), IValue( TypeId<T>() ) {}

			IValue *		Copy ()		const	override	{ return new TValue<T>( _value ); }
			const void *	Ptr ()		const	override	{ return &_value; }
			void *			Ptr ()	 			override	{ return &_value; }
			usize			SizeOf ()	const	override	{ return sizeof(_value); }
		};


		struct TPODValue : IValue
		{
		private:
			UniquePtr<ubyte>	_data;
			usize			_size;

		public:
			TPODValue () : _data(null), _size(0), IValue(TypeId_t()) {}
			TPODValue (usize size, TypeId_t id) : _data(null), _size(size), IValue(id)
			{
				if ( _size != 0 )
				{
					_data = new ubyte[size];
					ZeroMem( _data.ptr(), _size );
				}
			}
			
			TPODValue (const TPODValue &v) : _data(null), _size(v._size), IValue(TypeId_t())
			{
				if ( v._data != null )
				{
					_data = new ubyte[_size];
					MemCopy(_data.ptr(), v._data.ptr(), _size);
				}
			}
			
			IValue *		Copy ()		const	override	{ return new TPODValue( *this ); }
			const void *	Ptr ()		const	override	{ return _data.ptr(); }
			void *			Ptr ()				override	{ return _data.ptr(); }
			usize			SizeOf ()	const	override	{ return _size; }
		};


	// variables
	private:
		UniquePtr< IValue >	_pValue;


	// methods
	private:
		template <typename T>
		void _Set(const T& value)
		{
			_pValue = new TValue<T>( value );
		}

	public:
		TVariant ()
		{}

		TVariant (const TVariant &sVar)
		{
			if ( sVar.IsDefined() )
				_pValue = sVar._pValue->Copy();
		}


		template <typename T>
		explicit
		TVariant (const T& value)
		{
			_Set( value );
		}


		template <typename T>
		TVariant & operator = (const T& value)
		{
			_Set( value );
			return *this;
		}


		void CreatePOD (usize size, TypeId_t id = TypeId_t())
		{
			_pValue = new TPODValue( size, id );
		}


		template <typename T>
		const T & Get () const
		{
			if ( TypeId<T>() == _pValue->Id() )
				return *(const T *)_pValue->Ptr();
			
			WARNING( "incorrect requirement type" );
			return *(const T *) null;
		}

		template <typename T>
		T & Get ()
		{
			if ( TypeId<T>() == _pValue->Id() )
				return *(T *)_pValue->Ptr();
			
			WARNING( "incorrect requirement type" );
			return *(T *) null;
		}


		void * GetPtr ()
		{
			return IsDefined() ? _pValue->Ptr() : null;
		}


		void const * GetPtr () const
		{
			return IsDefined() ? _pValue->Ptr() : null;
		}


		usize GetSizeOf () const
		{
			return IsDefined() ? _pValue->SizeOf() : 0;
		}


		template <typename T>
		void CopyTo (OUT T &value) const
		{
			value = Get<T>();
		}


		TypeId_t GetValueTypeId () const
		{
			return IsDefined() ? _pValue->Id() : TypeId_t();
		}

		bool IsDefined () const
		{
			return _pValue != null;
		}
	};
	*/

}	// GXTypes
}	// GX_STL
