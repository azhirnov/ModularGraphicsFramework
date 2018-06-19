// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Wrapper for (member) function pointer.
*/

#pragma once

#include "Core/STL/CompileTime/TypeTraits.h"
#include "Core/STL/Memory/MemFunc.h"
#include "Core/STL/Memory/PlacementNew.h"
#include "Core/STL/Dimensions/ByteAndBit.h"
#include "Core/STL/Types/Function.h"
#include "Core/STL/Types/SharedPointer.h"
#include "Core/STL/Types/WeakPointer.h"

namespace GX_STL
{
namespace GXTypes
{

	template <typename Func, usize BufSize = GlobalConst::STL_DelegateBufferSize>
	struct Delegate;
	
	template <typename Func, usize BufSize = GlobalConst::STL_DelegateBufferSize>
	struct Event {};


	namespace _types_hidden_
	{

		//
		// Pointer Wrapper
		//
		template <typename T>
		struct PointerWrapper
		{
		// variables
		private:
			mutable T	_ptr;

		// methods
		public:
			PointerWrapper (const T &ptr) : _ptr(ptr) {}

			bool IsValid () const			{ return _ptr != null; }

			decltype(auto) Ref () const		{ return *_ptr; }
			void const*    Ptr () const		{ return Cast<void const*>(&(*_ptr)); }
		};
		

		template <typename T, typename B, typename WS, typename SPS>
		struct PointerWrapper< WeakPointer<T,B,WS,SPS> >
		{
		// types
			using WP = WeakPointer<T,B,WS,SPS>;

		// variables
		private:
			mutable WP		_ptr;

		// methods
		public:
			PointerWrapper (const WP &ptr) : _ptr(ptr) {}
			PointerWrapper (WP &&ptr) : _ptr(RVREF(ptr)) {}

			bool IsValid () const		{ return _ptr.Lock().IsNotNull(); }

			T &			Ref () const	{ return *_ptr.Lock(); }
			void const*	Ptr () const	{ return Cast<void const *>(_ptr.RawPtr()); }
		};


		struct DelegateBase {};

		template <typename Func>
		struct DelegateInterface;


		//
		// Delegate interface
		//
		template <typename Ret, typename ...Args>
		struct DelegateInterface< Ret (Args...) >
		{
		// types
			using Interface_t	= DelegateInterface< Ret (Args...) >;


		// methods
			virtual ~DelegateInterface ()				{}
			virtual bool		IsValid ()				const = 0;
			virtual Ret			Call (Args&&...)		const = 0;
			//virtual void		Swap (Interface_t *)	= 0;
			virtual void		MoveTo (BinArrayRef)	= 0;
			virtual void		CopyTo (BinArrayRef)	const = 0;
			virtual TypeId		TypeIdOf ()				const = 0;
			virtual BytesU		Size ()					const = 0;
			virtual void const*	ClassPtr ()				const = 0;
		
			forceinline bool Cmp (const Interface_t *right) const
			{
				const BytesU size = Size();
				return	( size == right->Size() ) and
						( TypeIdOf() == right->TypeIdOf() ) and
						UnsafeMem::MemCmp( Cast<const void *>(this), Cast<const void *>(right), size ) == 0;
			}
		
			forceinline bool Less (const Interface_t *right) const
			{
				const BytesU	size0 = Size();
				const BytesU	size1 = right->Size();
				const TypeId	type0 = TypeIdOf();
				const TypeId	type1 = right->TypeIdOf();
			
				if ( type0 == type1 ) {
					if ( size0 == size1 )
						return UnsafeMem::MemCmp( Cast<const void *>(this), Cast<const void *>(right), size0 ) < 0;
					else
						return size0 < size1;
				}
				return type0 < type1;
			}
		};


		//
		// Static Delegate
		//
		template <typename Ret, typename ...Args>
		struct StaticDelegate final : DelegateInterface< Ret (Args...) >
		{
		// types
		public:
			using Self			= StaticDelegate< Ret, Args... >;
			using Function_t	= Ret (*) (Args...);

		// variables
		private:
			Function_t	_func;

		// methods
		public:
			explicit StaticDelegate (Function_t fn) noexcept : _func(fn) {}

			bool		IsValid ()				const override	{ return _func != null; }
			Ret			Call (Args&&... args)	const override	{ return _func( FW<Args>(args)... ); }
			TypeId		TypeIdOf ()				const override	{ return GXTypes::TypeIdOf( *this ); }
			BytesU		Size ()					const override	{ return BytesU::SizeOf( *this ); }
			void const*	ClassPtr ()				const override	{ return null; }
			
			void MoveTo (BinArrayRef buf) override
			{
				PlacementNew< Self >( buf, RVREF( *this ) );
			}

			void CopyTo (BinArrayRef buf) const override
			{
				PlacementNew< Self >( buf, *this );
			}
		};
		

		//
		// Member Delegate
		//
		template <typename C, typename Class, typename Ret, typename ...Args>
		struct MemberDelegate final : DelegateInterface< Ret (Args...) >
		{
		// types
		public:
			using Self			= MemberDelegate< C, Class, Ret, Args... >;
			using Ptr_t			= PointerWrapper< C >;
			using Function_t	= Ret (Class:: *) (Args...);

		// variables
		private:
			Ptr_t			_classPtr;
			Function_t		_func;

		// methods
		public:
			MemberDelegate (const C &ptr, Function_t fn) noexcept : _classPtr(ptr), _func(fn) {}
			MemberDelegate (C &&ptr, Function_t fn) noexcept : _classPtr( RVREF(ptr) ), _func(fn) {}

			bool		IsValid ()				const override	{ return _func != null and _classPtr.IsValid(); }
			Ret			Call (Args&&... args)	const override	{ return (_classPtr.Ref().*_func)( FW<Args>(args)... ); }
			TypeId		TypeIdOf ()				const override	{ return GXTypes::TypeIdOf( *this ); }
			BytesU		Size ()					const override	{ return BytesU::SizeOf( *this ); }
			void const*	ClassPtr ()				const override	{ return _classPtr.Ptr(); }
			
			void MoveTo (BinArrayRef buf) override
			{
				PlacementNew< Self >( buf, RVREF( *this ) );
			}

			void CopyTo (BinArrayRef buf) const override
			{
				PlacementNew< Self >( buf, *this );
			}
		};
		

		//
		// Member Delegate (const)
		//
		template <typename C, typename Class, typename Ret, typename ...Args>
		struct MemberDelegateConst final : DelegateInterface< Ret (Args...) >
		{
		// types
		public:
			using Self			= MemberDelegateConst< C, Class, Ret, Args... >;
			using Ptr_t			= PointerWrapper< C >;
			using Function_t	= Ret (Class:: *) (Args...) const;
			
		// variables
		private:
			Ptr_t			_classPtr;
			Function_t		_func;

		// methods
		public:
			MemberDelegateConst (const C &ptr, Function_t fn) noexcept : _classPtr(ptr), _func(fn) {}
			MemberDelegateConst (C &&ptr, Function_t fn) noexcept : _classPtr( RVREF(ptr) ), _func(fn) {}

			bool		IsValid ()				const override	{ return _func != null and _classPtr.IsValid(); }
			Ret			Call (Args&&... args)	const override	{ return (_classPtr.Ref().*_func)( FW<Args>(args)... ); }
			TypeId		TypeIdOf ()				const override	{ return GXTypes::TypeIdOf( *this ); }
			BytesU		Size ()					const override	{ return BytesU::SizeOf( *this ); }
			void const*	ClassPtr ()				const override	{ return _classPtr.Ptr(); }
			
			void MoveTo (BinArrayRef buf) override
			{
				PlacementNew< Self >( buf, RVREF( *this ) );
			}

			void CopyTo (BinArrayRef buf) const override
			{
				PlacementNew< Self >( buf, *this );
			}
		};

	}	// _types_hidden_


	
	//
	// Delegate
	//

	template <typename Ret, typename ...Args, usize BufSize>
	struct alignas(ulong) Delegate< Ret (Args...), BufSize > : _types_hidden_::DelegateBase
	{
	// types
	public:
		using Self			= Delegate< Ret (Args...), BufSize >;
		using Result_t		= Ret;
		using Args_t		= CompileTime::TypeListFrom< Args..., CompileTime::TypeListEnd >;
		using Function_t	= Ret (Args...);

		// use references for heavy objects!!!
		//STATIC_ASSERT( Args_t::MaxSizeOf() <= sizeof(ulong) );

	private:
		using _Interface_t	= _types_hidden_::DelegateInterface< Ret (Args...) >;
		
		union _Storage_t {
			ulong	maxAlign;
			ubyte	buf[ BufSize ];
		};


	// variables
	private:
		_Storage_t				_storage;
		DEBUG_ONLY( TypeId		_dbgType );


	// methods
	public:
		forceinline Delegate (GX_DEFCTOR)
		{
			_Clear();
		}

		forceinline Delegate (const Self &other)
		{
			_Copy( other );
		}

		forceinline Delegate (Self &&other)
		{
			_Move( RVREF( other ) );
		}

		forceinline ~Delegate ()
		{
			_Delete();
		}

		forceinline Self& operator = (const Self &other)
		{
			_Delete();
			_Copy( other );
			return *this;
		}

		forceinline Self& operator = (Self &&other)
		{
			_Delete();
			_Move( RVREF( other ) );
			return *this;
		}

		ND_ forceinline bool EqualPointers (const void *ptr) const
		{
			return _IsCreated() and _Internal()->ClassPtr() == ptr;
		}

		ND_ forceinline explicit operator	bool ()				const	{ return IsValid(); }

		ND_ forceinline bool		IsValid ()					const	{ return _IsCreated() and _Internal()->IsValid(); }

		forceinline Result_t		operator () (Args... args)	const	{ ASSERT( IsValid() );  return _Internal()->Call( FW<Args>(args)... ); }
		
		forceinline Result_t		Call (Args... args)			const	{ ASSERT( IsValid() );	return _Internal()->Call( FW<Args>(args)... ); }
		forceinline Result_t		SafeCall (Args... args)		const	{ return IsValid() ? _Internal()->Call( FW<Args>(args)... ) : Result_t(); }

		ND_ forceinline TypeId		GetType ()					const	{ return IsValid() ? _Internal()->TypeIdOf() : TypeId(); }

		ND_ forceinline bool operator == (const Self &right)	const	{ ASSERT( IsValid() );  return _Internal()->Cmp( right._Internal() );  }
		ND_ forceinline bool operator <  (const Self &right)	const	{ ASSERT( IsValid() );  return _Internal()->Less( right._Internal() ); }
		ND_ forceinline bool operator >  (const Self &right)	const	{ ASSERT( IsValid() );  return right._Internal()->Less( _Internal() ); }
		ND_ forceinline bool operator != (const Self &right)	const	{ return not (*this == right); }
		ND_ forceinline bool operator <= (const Self &right)	const	{ return not (*this >  right); }
		ND_ forceinline bool operator >= (const Self &right)	const	{ return not (*this <  right); }


	private:
		forceinline _Interface_t const *	_Internal ()		const	{ return Cast<_Interface_t const*>(_storage.buf); }
		forceinline _Interface_t *			_Internal ()				{ return Cast<_Interface_t *>(_storage.buf); }
		forceinline BinArrayRef				_Data ()					{ return BinArrayRef( _storage.buf ); }
		forceinline bool					_IsCreated ()		const	{ return _storage.maxAlign != 0; }

		forceinline void _Delete () noexcept
		{
			if ( _IsCreated() )
			{
				_Internal()->~_Interface_t();
				_Clear();
			}
		}

		forceinline void _Move (Self &&other) noexcept
		{
			_Clear();

			if ( other._IsCreated() )
			{
				DEBUG_ONLY( _dbgType = other._dbgType );
				other._Internal()->MoveTo( _Data() );
				other._Clear();
			}
		}

		forceinline void _Copy (const Self &other) noexcept
		{
			_Clear();

			if ( other._IsCreated() )
			{
				DEBUG_ONLY( _dbgType = other._dbgType );
				other._Internal()->CopyTo( _Data() );
			}
		}

		forceinline void _Clear ()
		{
			_storage.maxAlign = 0;
			DEBUG_ONLY( _dbgType = TypeId() );
		}


	// statics
	public:
		ND_ forceinline static Self  Create (Ret (*fn) (Args...)) noexcept
		{
			using DI = _types_hidden_::StaticDelegate< Ret, Args... >;

			Self	del;
			PlacementNew<DI>( del._Data(), DI( fn ) );
			DEBUG_ONLY( del._dbgType = TypeIdOf( fn ) );
			return del;
		}

		template <typename C, typename Class>
		ND_ forceinline static Self  Create (const C &classPtr, Ret (Class:: *fn) (Args...)) noexcept
		{
			STATIC_ASSERT(( _types_hidden_::FB_IsSame< C, Class > ));

			using DI = _types_hidden_::MemberDelegate< C, Class, Ret, Args... >;

			Self	del;
			PlacementNew<DI>( del._Data(), DI( classPtr, fn ) );
			DEBUG_ONLY( del._dbgType = TypeIdOf( fn ) );
			return del;
		}

		template <typename C, typename Class>
		ND_ forceinline static Self  Create (const C &classPtr, Ret (Class:: *fn) (Args...) const) noexcept
		{
			STATIC_ASSERT(( _types_hidden_::FB_IsSame< C, Class > ));

			using DI = _types_hidden_::MemberDelegateConst< C, Class, Ret, Args... >;

			Self	del;
			PlacementNew<DI>( del._Data(), DI( classPtr, fn ) );
			DEBUG_ONLY( del._dbgType = TypeIdOf( fn ) );
			return del;
		}
	};


/*
=================================================
	DelegateBuilder
=================================================
*/
	template <typename Ret, typename ...Args>
	ND_ forceinline Delegate< Ret (Args...) >  DelegateBuilder (Ret (*fn) (Args...)) noexcept
	{
		return Delegate< Ret (Args...) >::Create( fn );
	}

	template <typename C, typename Class, typename Ret, typename ...Args>
	ND_ forceinline Delegate< Ret (Args...) >  DelegateBuilder (const C &classPtr, Ret (Class:: *fn) (Args...)) noexcept
	{
		return Delegate< Ret (Args...) >::Create( classPtr, fn );
	}

	template <typename C, typename Class, typename Ret, typename ...Args>
	ND_ forceinline Delegate< Ret (Args...) >  DelegateBuilder (const C &classPtr, Ret (Class:: *fn) (Args...) const) noexcept
	{
		return Delegate< Ret (Args...) >::Create( classPtr, fn );
	}



	//
	// Event
	//
	
	template <typename Ret, typename ...Args, usize BufSize>
	struct Event< Ret (Args...), BufSize > : public CompileTime::FastCopyable
	{
	// types
	public:
		using Function_t		= Ret (Args...);
		using Delegate_t		= Delegate< Ret (Args...), BufSize >;
		using Self				= Event< Ret (Args...), BufSize >;
		using DelegateArray_t	= Array< Delegate_t >;
		using Args_t			= CompileTime::TypeListFrom< Args..., CompileTime::TypeListEnd >;
		using Result_t			= Ret;


	// variables
	private:
		DelegateArray_t		_delegates;


	// methods
	public:
		forceinline Event (GX_DEFCTOR) noexcept {}
		
		GX_DEFCOPYCTOR_ASSIGN( Event );

		forceinline void Add (const Delegate_t &del)
		{
			ASSERT( del.IsValid() );

			Remove( del );
			_delegates.PushBack( del );
		}

		forceinline void Add (Ret (*fn) (Args...))									{ Add( Delegate_t::Create( fn ) ); }

		template <typename C, typename Class>
		forceinline void Add (const C &ptr, Ret (Class:: *fn) (Args...))			{ Add( Delegate_t::Create( ptr, fn ) ); }
		
		template <typename C, typename Class>
		forceinline void Add (const C &ptr, Ret (Class:: *fn) (Args...) const)		{ Add( Delegate_t::Create( ptr, fn ) ); }

		forceinline void Remove (const Delegate_t &del)								{ _delegates.FindAndErase( del ); }
		forceinline void Remove (Ret (*fn) (Args...))								{ Remove( Delegate_t::Create( fn ) ); }

		template <typename C, typename Class>
		forceinline void Remove (const C &ptr, Ret (Class:: *fn) (Args...))			{ Remove( Delegate_t::Create( ptr, fn ) ); }
		
		template <typename C, typename Class>
		forceinline void Remove (const C &ptr, Ret (Class:: *fn) (Args...) const)	{ Remove( Delegate_t::Create( ptr, fn ) ); }

		forceinline void Reserve (usize count)										{ _delegates.Reserve( count ); }
		forceinline void Clear ()													{ _delegates.Clear(); }


		forceinline void Call (Args... args)						const			{ FOR( i, _delegates ) { _delegates[i].Call( FW<Args>(args)... ); } }

		forceinline void operator () (Args... args)					const			{ FOR( i, _delegates ) { _delegates[i].Call( FW<Args>(args)... ); } }

		ND_ forceinline bool Empty ()								const			{ return _delegates.Empty(); }

		ND_ forceinline usize Count ()								const			{ return _delegates.Count(); }

		ND_ forceinline Delegate_t const&	Get (usize index)		const			{ return _delegates[index]; }


		forceinline void RemoveAllFor (const void *ptr)
		{
			FOR( i, _delegates )
			{
				if ( _delegates[i].EqualPointers( ptr ) )
				{
					_delegates.Erase( i );
					--i;
				}
			}
		}
		
		template <typename T>
		forceinline void RemoveAllFor (const T &ptr)
		{
			return RemoveAllFor( Cast<const void *>(&(*ptr)) );
		}

		forceinline friend void SwapValues (Self &left, Self &right)
		{
			SwapValues( left._delegates, right._delegates );
		}
	};

	
	template <typename Ret, typename ...Args, usize BufSize>
	struct Event< Delegate< Ret (Args...), BufSize > > : Event< Ret (Args...), BufSize >
	{};


}	// GXTypes

namespace CompileTime
{
	template <typename T>
	static constexpr bool	IsDelegate = IsBaseOf< GXTypes::_types_hidden_::DelegateBase, T >;

}	// CompileTime
}	// GX_STL
