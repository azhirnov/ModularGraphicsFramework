// Copyright © 2014-2016  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Types/Ptr.h"
#include "Engine/STL/CompileTime/TypeTraits.h"
#include "Engine/STL/Types/Optional.h"

namespace GX_STL
{
namespace GXTypes
{
	/*
	//
	// List
	//

	template <typename T>
	struct List
	{
	// types
	private:
		struct ListNode;
		typedef Ptr< ListNode >		ListNodePtr;

		struct ListNode
		{
		// variables
			ListNodePtr		prev;
			ListNodePtr		next;
			T				value;
			
		// methods
			ListNode (const T &value, ListNodePtr prev, ListNodePtr next) :
				value( value ), prev( prev ), next( next )
			{
				ASSERT( Check() );
			}
			
			ListNode (T &&value, ListNodePtr prev, ListNodePtr next) :
				value( RVREF( value ) ), prev( prev ), next( next )
			{
				ASSERT( Check() );
			}

			~ListNode ()
			{
				if ( HasPrev() )	prev->next = next;
				if ( HasNext() )	next->prev = prev;
			}

			bool Check () const
			{
				if ( isFirstOrLast )
				{
					if ( not HasPrev() and next->prev == this )
						return true;	// begin
					
					if ( not HasNext() and prev->next == this )
						return true;	// end
				}
				return prev->next == this and next->prev == this;
			}

			bool HasPrev () const	{ return prev != null; }
			bool HasNext () const	{ return next != null; }
		};

	public:
		struct  ListNodeIterator;
		typedef ListNodeIterator	iterator;

		typedef List<T>		Self;


	// variables
	private:
		ListNodePtr		_begin;
		ListNodePtr		_end;

		//
		// Begin -> Node <-> ... <-> Node <- End
		//


	// methods
	public:
		List ();
		List (Self &&other);
		List (const Self &other);

		~List ();

		void Clear ();

		bool Empty ()	const	{ return _begin->next == _end; }

		bool IsBegin (iterator iter) const;
		bool IsEnd (iterator iter) const;

		iterator	Begin ();
		iterator	End ();

		iterator	PushBack (T &&value);
		iterator	PushBack (const T &value);

		iterator	PushFront (T &&value);
		iterator	PushFront (const T &value);

		void PopFront ();
		void PopBack ();

		usize CalcSize () const;

		Self &	operator = (Self &&right);
		Self &	operator = (const Self &right);

		bool	operator == (const Self &right) const;
		bool	operator != (const Self &right) const;
	};


	
	//
	// List Node Iterator
	//

	template <typename T>
	struct List<T>::ListNodeIterator
	{
	// variables
	private:
		Ptr< ListNode >		_node;

	// methods
	public:
		ListNodeIterator () {}
		ListNodeIterator (ListNodePtr node) : _node( node ) {}
		ListNodeIterator (const ListNodeIterator &iter) : _node( iter._node ) {}

		ListNodeIterator&	operator ++ ()
		{
			ASSERT( IsValid() );

			if ( _node.IsNotNull() )
				_node = _node->next;

			return *this;
		}

		ListNodeIterator	operator ++ (int)
		{
			ListNodeIterator	iter( *this );
			++(*this);
			return iter;
		}

		ListNodeIterator&	operator -- ()
		{
			ASSERT( IsValid() );

			if ( _node.IsNotNull() )
				_node = _node->prev;

			return *this;
		}

		ListNodeIterator	operator -- (int)
		{
			ListNodeIterator	iter( *this );
			--(*this);
			return iter;
		}

		ListNodeIterator&	operator = (const ListNodeIterator &right)
		{
			_node = right._node;
			return *this;
		}

		ListNodeIterator&	operator += (const isize value)
		{
			ASSERT( IsValid() );

			if ( value > 0 )
			{
				for (isize i = 0; i < value and IsValid(); ++i) {
					_node = _node->next;
				}
			}
			else
			{
				for (isize i = 0; i > value and IsValid(); --i) {
					_node = _node->prev;
				}
			}
			return *this;
		}

		ListNodeIterator&	operator -= (const isize value)
		{
			return *this += -value;
		}

		ListNodeIterator&	MoveToFirst ()
		{
			for (; _node->HasPrev();) {
				_node = _node->prev;
			}
			return *this;
		}

		ListNodeIterator&	MoveToLast ()
		{
			for (; _node->HasNext();) {
				_node = _node->next;
			}
			return *this;
		}

		ListNodeIterator	PushBack (const T &value)
		{
			ASSERT( IsValid() );
			return ListNodeIterator( new ListNode( value, _node, _node->next ) );
		}

		ListNodeIterator	PushBack (T &&value)
		{
			ASSERT( IsValid() );
			return ListNodeIterator( new ListNode( RVREF( value ), _node, _node->next ) );
		}
		
		ListNodeIterator	PushFront (const T &value)
		{
			ASSERT( IsValid() );
			return ListNodeIterator( new ListNode( value, _node->prev, _node ) );
		}
		
		ListNodeIterator	PushFront (T &&value)
		{
			ASSERT( IsValid() );
			return ListNodeIterator( new ListNode( RVREF( value ), _node->prev, _node ) );
		}

		bool				Remove ()
		{
			if ( not IsValid() )
				return false;

			delete _node;
			_node = null;
			return true;
		}

		ListNodeIterator	operator + (const isize value) const	{ return ListNodeIterator( *this ) += value; }
		ListNodeIterator	operator - (const isize value) const	{ return ListNodeIterator( *this ) -= value; }
		
		ListNodeIterator	operator >> (const isize value) const	{ return ListNodeIterator( *this ) += value; }
		ListNodeIterator	operator << (const isize value) const	{ return ListNodeIterator( *this ) -= value; }

		ListNodeIterator	operator >>= (const isize value)		{ return *this += value; }
		ListNodeIterator	operator <<= (const isize value)		{ return *this -= value; }

		T &					operator * ()							{ ASSERT( IsValid() );  return _node->value; }
		T const &			operator * () const						{ ASSERT( IsValid() );  return _node->value; }

		T *					operator -> ()							{ ASSERT( IsValid() );  return &_node->value; }
		T const *			operator -> () const					{ ASSERT( IsValid() );  return &_node->value; }

		bool	operator == (const ListNodeIterator &right) const	{ return _node == right._node; }
		bool	operator != (const ListNodeIterator &right) const	{ return _node == right._node; }

		bool	IsValid () const									{ return _node.IsNotNull(); }

		operator bool () const										{ return IsValid(); }
	};

	

	template <typename T>
	inline List<T>::List ()
	{
	}
	

	template <typename T>
	inline List<T>::List (List<T> &&other)
	{
		*this = RVREF( other );
	}


	template <typename T>
	inline List<T>::List (const List<T> &other)
	{
		*this = other;
	}

	
	template <typename T>
	inline List<T>::~List ()
	{
		Clear();
	}

	
	template <typename T>
	inline void List<T>::Clear ()
	{
		for (; not Empty();)
		{
			PopFront();
		}
	}
	

	template <typename T>
	inline typename List<T>::iterator  List<T>::PushBack (T &&value)
	{
		return iterator( _end ).PushFront( RVREF( value ) );
	}


	template <typename T>
	inline typename List<T>::iterator  List<T>::PushBack (const T &value)
	{
		return iterator( _end ).PushFront( value );
	}

	
	template <typename T>
	inline typename List<T>::iterator  List<T>::PushFront (T &&value)
	{
		return iterator( _begin ).PushBack( RVREF( value ) );
	}


	template <typename T>
	inline typename List<T>::iterator  List<T>::PushFront (const T &value)
	{
		return iterator( _begin ).PushBack( value );
	}

	
	template <typename T>
	inline void List<T>::PopFront ()
	{
		if ( _begin->HasNext() )
			delete _begin->next;
	}


	template <typename T>
	inline void List<T>::PopBack ()
	{
		if ( _end->HasPrev() )
			delete _end->prev;
	}
	

	template <typename T>
	inline bool List<T>::IsBegin (iterator iter) const
	{
		return iter == iterator( _begin );
	}
	

	template <typename T>
	inline bool List<T>::IsEnd (iterator iter) const
	{
		return iter == iterator( _end );
	}
	

	template <typename T>
	inline typename List<T>::iterator List<T>::Begin ()
	{
		return iterator( _begin );
	}
	

	template <typename T>
	inline typename List<T>::iterator List<T>::End ()
	{
		return iterator( _end );
	}
	

	template <typename T>
	inline usize CalcSize () const
	{
		usize		count = 0;
		iterator	iter  = Begin();

		for (; iter; ++count) {
			++iter;
		}
		return count;
	}
		

	template <typename T>
	inline List<T> &  List<T>::operator = (Self &&right)
	{
	}


	template <typename T>
	inline List<T> &  List<T>::operator = (const Self &right)
	{
	}
		

	template <typename T>
	inline bool	 List<T>::operator == (const Self &right) const
	{
	}


	template <typename T>
	inline bool  List<T>::operator != (const Self &right) const
	{
	}
	*/

}	// GXTypes
}	// GX_STL
