// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Static variant class for types with base interface.
*/

#pragma once

#include "Engine/STL/Types/VariantRef.h"
#include "Engine/STL/Types/VariantCRef.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Variant Interface
	//

	template <typename BaseClass, usize BufSize>
	struct alignas(ulong) VariantInterface : public CompileTime::FastCopyable
	{
	// types
	public:
		using Self			= VariantInterface< BaseClass, BufSize >;
		using Interface_t	= BaseClass *;


	private:
		static const usize	_BUF_SIZE = BufSize + sizeof(void *);	// + size of virtual functions in '_Impl'


		//
		// Interface
		//
		struct _Interface
		{
			virtual ~_Interface () {}
			virtual void		MoveTo (BinArrayRef)	= 0;
			virtual void		CopyTo (BinArrayRef)	const = 0;
			virtual TypeId		TypeIdOf ()				const = 0;
		};


		//
		// Implementation
		//
		template <typename Class>
		struct _Impl final : _Interface
		{
			using Self = _Impl<Class>;

			Class	value;

			_Impl (const Class &val) : value(val) {}
			_Impl (Class &&val) : value( FW<Class>(val) ) {}

			_Impl (const Self &other) : value(other.value) {}
			_Impl (Self &&other) : value( RVREF(other.value) ) {}

			void		MoveTo (BinArrayRef buf) override		{ PlacementNew< Self >( buf, RVREF( *this ) ); }
			void		CopyTo (BinArrayRef buf) const override	{ PlacementNew< Self >( buf, *this ); }
			TypeId		TypeIdOf ()				 const override	{ return GXTypes::TypeIdOf( value ); }
		};


		//
		// Storage
		//
		union _Storage_t {
			ulong	maxAlign;
			ubyte	buf[ _BUF_SIZE ];
		};


	// variables
	private:
		mutable _Storage_t		_storage;


	// methods
	public:
		VariantInterface (GX_DEFCTOR)
		{
			_Clear();
		}

		VariantInterface (const Self &other)
		{
			_Copy( other );
		}

		VariantInterface (Self &&other)
		{
			_Move( FW<Self>( other ) );
		}

		template <typename Class>
		explicit VariantInterface (Class &&obj)
		{
			STATIC_ASSERT( TypeTraits::IsRValueReference<Class &&> );
			STATIC_ASSERT(( CompileTime::IsBaseOf< BaseClass, Class > ));
			STATIC_ASSERT( _BUF_SIZE >= sizeof(_Impl<Class>) );
			STATIC_ASSERT( sizeof(_Impl<Class>) == (sizeof(void*) + sizeof(Class)) );

			PlacementNew< _Impl<Class> >( _Data(), RVREF(_Impl<Class>( FW<Class>(obj) )) );
		}

		template <typename Class>
		explicit VariantInterface (const Class &obj)
		{
			STATIC_ASSERT(( CompileTime::IsBaseOf< BaseClass, Class > ));
			STATIC_ASSERT( _BUF_SIZE >= sizeof(_Impl<Class>) );
			STATIC_ASSERT( sizeof(_Impl<Class>) == (sizeof(void*) + sizeof(Class)) );

			PlacementNew< _Impl<Class> >( _Data(), RVREF(_Impl<Class>( FW<Class>(obj) )) );
		}

		~VariantInterface ()
		{
			_Delete();
		}

		Self& operator = (const Self &other)
		{
			_DeleteNoClear();
			_Copy( other );
			return *this;
		}

		Self& operator = (Self &&other)
		{
			_DeleteNoClear();
			_Move( RVREF( other ) );
			return *this;
		}
		

		bool			IsCreated () const
		{
			return _storage.maxAlign != 0;
		}


		Ptr<BaseClass>	operator -> () const
		{
			return GetInterface();
		}

		Ptr<BaseClass>	GetInterface () const
		{
			return IsCreated() ? _GetBase() : null;
		}


		template <typename T>
		Ptr<T>			Get () const
		{
			return IsCreated() and TypeIdOf<T>() == GetTypeId() ? (T *) _GetBase() : null;
		}


		TypeId		GetTypeId ()						const	{ return IsCreated() ? _Internal()->TypeIdOf() : TypeId(); }


	private:
		forceinline BaseClass *			_GetBase ()		const	{ return (BaseClass *) &_storage.buf[sizeof(void *)]; }
		forceinline _Interface const*	_Internal ()	const	{ return (_Interface const*) _storage.buf; }
		forceinline _Interface *		_Internal ()			{ return (_Interface *) _storage.buf; }
		forceinline BinArrayRef			_Data ()				{ return BinArrayRef( _storage.buf ); }

		forceinline void _Delete ()
		{
			if ( IsCreated() )
			{
				_Internal()->~_Interface();
				_Clear();
			}
		}

		forceinline void _DeleteNoClear ()
		{
			if ( IsCreated() )
			{
				_Internal()->~_Interface();
			}
		}

		forceinline void _Move (Self &&other)
		{
			if ( other.IsCreated() )
			{
				other._Internal()->MoveTo( _Data() );
				other._Clear();
			}
			else
				_Clear();
		}

		forceinline void _Copy (const Self &other)
		{
			if ( other.IsCreated() )
				other._Internal()->CopyTo( _Data() );
			else
				_Clear();
		}

		forceinline void _Clear ()
		{
			_storage.maxAlign = 0;
		}
	};


}	// GXTypes
}	// GX_STL
