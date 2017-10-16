// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "VariantCRef.h"
#include "VariantRef.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Static Variant
	//

	template <usize DataSize>
	struct StaticVariant final
	{
	// types
	public:
		using Self	= StaticVariant< DataSize >;


	private:
		union _Storage_t {
			ulong	maxAlign;
			ubyte	buf[ DataSize ];
		};


	// variables
	private:
		_Storage_t		_storage;
		TypeId			_typeId;


	// methods
	public:
		StaticVariant (GX_DEFCTOR);
		
		StaticVariant (const Self &other);
		StaticVariant (Self &&other);
		
		template <typename T>
		explicit StaticVariant (const T &value);
		
		template <typename T>
		explicit StaticVariant (T &&value);

		~StaticVariant ();


		//template <typename T>
		//Optional<T>		GetOptional ()	const;

		VariantCRef		Get () const;

		VariantRef		Get ();

		template <typename T>
		T const&		Get () const;

		template <typename T>
		T &				Get ();
		
		template <typename T>
		bool			Is () const				{ return _typeId == TypeIdOf<T>(); }


		bool			IsDefined () const		{ return _typeId != TypeId(); }

		TypeId			GetTypeId () const		{ return _typeId; }


	private:
		BinArrayRef		_Data ()				{ return BinArrayRef( _storage.buf ); }

		void			_Clear ()				{ _typeId = TypeId();	ZeroMem( _Data() ); }
	};

	

	inline StaticVariant::StaticVariant (UninitializedType)
	{
		_Clear();
	}
		
	inline StaticVariant (const Self &other)
	{
	}

	inline StaticVariant (Self &&other)
	{
	}
		
	template <typename T>
	explicit StaticVariant (const T &value);
		
	template <typename T>
	explicit StaticVariant (T &&value);

	~StaticVariant ();


	
	template <usize DataSize>
	struct Hash< StaticVariant<DataSize> > : private Hash<T>
	{
		typedef StaticVariant<DataSize>		key_t;
		typedef Hash<T>						base_t;
		typedef typename base_t::result_t	result_t;

		result_t operator () (const key_t &x) const noexcept
		{
			// TODO
		}
	};


}	// GXTypes
}	// GX_STL
