// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Wrapper for (member) function pointer.
*/

#pragma once

#include "Engine/STL/CompileTime/TypeTraits.h"
#include "Engine/STL/Memory/MemFunc.h"
#include "Engine/STL/Memory/PlacementNew.h"
#include "Engine/STL/Dimensions/ByteAndBit.h"
#include "Function.h"

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
						UnsafeMem::MemCmp( (void *)this, (void *)right, size ) == 0;
			}
		
			forceinline bool Less (const Interface_t *right) const
			{
				const BytesU	size0 = Size();
				const BytesU	size1 = right->Size();
				const TypeId	type0 = TypeIdOf();
				const TypeId	type1 = right->TypeIdOf();
			
				if ( type0 == type1 ) {
					if ( size0 == size1 )
						return UnsafeMem::MemCmp( (void *)this, (void *)right, size0 ) < 0;
					else
						return size0 < size1;
				}
				return type0 < type1;
			}
		
			forceinline bool Greater (const Interface_t *right) const
			{
				const BytesU	size0 = Size();
				const BytesU	size1 = right->Size();
				const TypeId	type0 = TypeIdOf();
				const TypeId	type1 = right->TypeIdOf();
			
				if ( type0 == type1 ) {
					if ( size0 == size1 )
						return UnsafeMem::MemCmp( (void *)this, (void *)right, size0 ) > 0;
					else
						return size0 > size1;
				}
				return type0 > type1;
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
			BytesU		Size ()					const override	{ return SizeOf( *this ); }
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
			using Ptr_t			= C;
			using Function_t	= Ret (Class:: *) (Args...);

		// variables
		private:
			Ptr_t			_classPtr;
			Function_t		_func;

		// methods
		public:
			MemberDelegate (const Ptr_t &ptr, Function_t fn) noexcept : _classPtr(ptr), _func(fn) {}
			MemberDelegate (Ptr_t &&ptr, Function_t fn) noexcept : _classPtr( RVREF(ptr) ), _func(fn) {}

			bool		IsValid ()				const override	{ return _func != null and _classPtr != null; }
			Ret			Call (Args&&... args)	const override	{ return ((*_classPtr).*_func)( FW<Args>(args)... ); }
			TypeId		TypeIdOf ()				const override	{ return GXTypes::TypeIdOf( *this ); }
			BytesU		Size ()					const override	{ return SizeOf( *this ); }
			void const*	ClassPtr ()				const override	{ return (void const*) &(*_classPtr); }
			
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
			using Ptr_t			= C;
			using Function_t	= Ret (Class:: *) (Args...) const;
			
		// variables
		private:
			Ptr_t			_classPtr;
			Function_t		_func;

		// methods
		public:
			MemberDelegateConst (const Ptr_t &ptr, Function_t fn) noexcept : _classPtr(ptr), _func(fn) {}
			MemberDelegateConst (Ptr_t &&ptr, Function_t fn) noexcept : _classPtr( RVREF(ptr) ), _func(fn) {}

			bool		IsValid ()				const override	{ return _func != null and _classPtr != null; }
			Ret			Call (Args&&... args)	const override	{ return ((*_classPtr).*_func)( FW<Args>(args)... ); }
			TypeId		TypeIdOf ()				const override	{ return GXTypes::TypeIdOf( *this ); }
			BytesU		Size ()					const override	{ return SizeOf( *this ); }
			void const*	ClassPtr ()				const override	{ return (void const*) &(*_classPtr); }
			
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
		_Storage_t	_storage;


	// methods
	public:
		forceinline Delegate (GX_DEFCTOR) noexcept
		{
			_Clear();
		}

		forceinline Delegate (const Self &other) noexcept
		{
			_Copy( other );
		}

		forceinline Delegate (Self &&other) noexcept
		{
			_Move( RVREF( other ) );
		}

		forceinline ~Delegate ()
		{
			_Delete();
		}

		forceinline Self& operator = (const Self &other)
		{
			_DeleteNoClear();
			_Copy( other );
			return *this;
		}

		forceinline Self& operator = (Self &&other)
		{
			_DeleteNoClear();
			_Move( RVREF( other ) );
			return *this;
		}

		template <typename T>
		forceinline bool EqualPointers (const T &ptr)
		{
			return IsValid() and _Internal()->ClassPtr() == (void const *) &(*ptr);
		}

		forceinline operator	bool ()								const	{ return IsValid(); }

		forceinline bool		IsValid ()							const	{ return _IsCreated() DEBUG_ONLY( and _Internal()->IsValid() ); }

		forceinline Result_t	operator () (Args... args)			const	{ ASSERT( IsValid() );  return _Internal()->Call( FW<Args>(args)... ); }
		
		forceinline Result_t	Call (Args... args)					const	{ ASSERT( IsValid() );	return _Internal()->Call( FW<Args>(args)... ); }
		forceinline Result_t	SafeCall (Args... args)				const	{ return IsValid() ? _Internal()->Call( FW<Args>(args)... ) : Result_t(); }

		forceinline TypeId		GetType ()							const	{ return IsValid() ? _Internal()->TypeIdOf() : TypeId(); }

		forceinline bool operator == (const Self &right)			const	{ ASSERT( IsValid() );  return _Internal()->Cmp( right._Internal() );  }
		forceinline bool operator <  (const Self &right)			const	{ ASSERT( IsValid() );  return _Internal()->Less( right._Internal() ); }
		forceinline bool operator >  (const Self &right)			const	{ ASSERT( IsValid() );  return _Internal()->Greater( right._Internal() ); }
		forceinline bool operator != (const Self &right)			const	{ return not (*this == right); }
		forceinline bool operator <= (const Self &right)			const	{ return not (*this >  right); }
		forceinline bool operator >= (const Self &right)			const	{ return not (*this <  right); }


	private:
		forceinline _Interface_t const *	_Internal ()			const	{ return (_Interface_t const*) _storage.buf; }
		forceinline _Interface_t *			_Internal ()					{ return (_Interface_t *) _storage.buf; }
		forceinline BinArrayRef				_Data ()						{ return BinArrayRef( _storage.buf ); }
		forceinline bool					_IsCreated ()			const	{ return _storage.maxAlign != 0; }

		forceinline void _Delete ()
		{
			if ( _IsCreated() )
			{
				_Internal()->~_Interface_t();
				_Clear();
			}
		}

		forceinline void _DeleteNoClear ()
		{
			if ( _IsCreated() )
			{
				_Internal()->~_Interface_t();
			}
		}

		forceinline void _Move (Self &&other)
		{
			if ( other._IsCreated() )
			{
				other._Internal()->MoveTo( _Data() );
				other._Clear();
			}
			else
				_Clear();
		}

		forceinline void _Copy (const Self &other)
		{
			if ( other._IsCreated() )
				other._Internal()->CopyTo( _Data() );
			else
				_Clear();
		}

		forceinline void _Clear ()
		{
			_storage.maxAlign = 0;
		}


	// statics
	public:
		forceinline static Self  Create (Ret (*fn) (Args...)) noexcept
		{
			using DI = _types_hidden_::StaticDelegate< Ret, Args... >;

			Self	del;
			PlacementNew<DI>( del._Data(), DI( fn ) );
			return del;
		}

		template <typename C, typename Class>
		forceinline static Self  Create (const C &classPtr, Ret (Class:: *fn) (Args...)) noexcept
		{
			STATIC_ASSERT(( _types_hidden_::FB_IsSame< C, Class > ));

			using DI = _types_hidden_::MemberDelegate< C, Class, Ret, Args... >;

			Self	del;
			PlacementNew<DI>( del._Data(), DI( classPtr, fn ) );
			return del;
		}

		/*template <typename C, typename Class>
		forceinline static Self  Create (C &&classPtr, Ret (Class:: *fn) (Args...)) noexcept
		{
			STATIC_ASSERT(( _types_hidden_::FB_IsSame< C, Class > ));

			using DI = _types_hidden_::MemberDelegate< C, Class, Ret, Args... >;

			Self	del;
			PlacementNew<DI>( del._Data(), DI( FW<C>(classPtr), fn ) );
			return del;
		}*/

		template <typename C, typename Class>
		forceinline static Self  Create (const C &classPtr, Ret (Class:: *fn) (Args...) const) noexcept
		{
			STATIC_ASSERT(( _types_hidden_::FB_IsSame< C, Class > ));

			using DI = _types_hidden_::MemberDelegateConst< C, Class, Ret, Args... >;

			Self	del;
			PlacementNew<DI>( del._Data(), DI( classPtr, fn ) );
			return del;
		}

		/*template <typename C, typename Class>
		forceinline static Self  Create (C &&classPtr, Ret (Class:: *fn) (Args...) const) noexcept
		{
			STATIC_ASSERT(( _types_hidden_::FB_IsSame< C, Class > ));

			using DI = _types_hidden_::MemberDelegateConst< C, Class, Ret, Args... >;

			Self	del;
			PlacementNew<DI>( del._Data(), DI( FW<C>(classPtr), fn ) );
			return del;
		}*/
	};


/*
=================================================
	DelegateBuilder
=================================================
*/
	template <typename Ret, typename ...Args>
	forceinline Delegate< Ret (Args...) >  DelegateBuilder (Ret (*fn) (Args...)) noexcept
	{
		return Delegate< Ret (Args...) >::Create( fn );
	}

	template <typename C, typename Class, typename Ret, typename ...Args>
	forceinline Delegate< Ret (Args...) >  DelegateBuilder (const C &classPtr, Ret (Class:: *fn) (Args...)) noexcept
	{
		return Delegate< Ret (Args...) >::Create( classPtr, fn );
	}

	/*template <typename C, typename Class, typename Ret, typename ...Args>
	forceinline Delegate< Ret (Args...) >  DelegateBuilder (C &&classPtr, Ret (Class:: *fn) (Args...)) noexcept
	{
		return Delegate< Ret (Args...) >::Create( FW<C>(classPtr), fn );
	}*/

	template <typename C, typename Class, typename Ret, typename ...Args>
	forceinline Delegate< Ret (Args...) >  DelegateBuilder (const C &classPtr, Ret (Class:: *fn) (Args...) const) noexcept
	{
		return Delegate< Ret (Args...) >::Create( classPtr, fn );
	}

	/*template <typename C, typename Class, typename Ret, typename ...Args>
	forceinline Delegate< Ret (Args...) >  DelegateBuilder (C &&classPtr, Ret (Class:: *fn) (Args...) const) noexcept
	{
		return Delegate< Ret (Args...) >::Create( FW<C>(classPtr), fn );
	}*/



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

		forceinline Event (Self &&other) noexcept : _delegates( RVREF(other._delegates) ) {}
		forceinline Event (const Self &other) noexcept : _delegates( other._delegates ) {}

		forceinline Self& operator = (Self &&) = default;
		forceinline Self& operator = (const Self &) = default;


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


		forceinline void Call (Args&&... args)					const		{ FOR( i, _delegates ) { _delegates[i].Call( FW<Args>(args)... ); } }

		forceinline void operator () (Args&&... args)			const		{ FOR( i, _delegates ) { _delegates[i].Call( FW<Args>(args)... ); } }

		forceinline bool Empty ()								const		{ return _delegates.Empty(); }

		forceinline usize Count ()								const		{ return _delegates.Count(); }

		forceinline Delegate_t const&	Get (usize index)		const		{ return _delegates[index]; }


		template <typename T>
		forceinline void RemoveAllFor (const T &ptr)
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
