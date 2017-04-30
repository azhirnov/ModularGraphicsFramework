// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Ptr.h"

namespace GX_STL
{
namespace GXTypes
{
	
	//
	// Function Output Argument Helper
	//

	template <typename T>
	struct Out
	{
	// types
	public:
		typedef Out<T>		Self;
		typedef T			value_t;

		
	// variables
	private:
		T *		_ref;
		bool	_changed;

		
	// methods
	public:
		Out (T &ref) : _ref(&ref), _changed(false)
		{}

		Out (Out<T> &&other) : _ref(other._ref), _changed(other._changed)
		{}

		Out (Out<T> &other) : _ref(other._ref), _changed(other._changed)
		{}

		~Out ()
		{
			// result value must be initialized
			ASSERT( _changed );
		}

		operator const T& () const
		{
			return *_ref;
		}

		operator T& ()
		{
			_changed = true;
			return *_ref;
		}

		Self& operator = (const T &value)
		{
			_changed = true;
			*_ref	 = value;
			return *this;
		}
		
		T & operator * ()
		{
			_changed = true;
			return *_ptr;
		}

		T * operator -> ()
		{
			_changed = true;
			return _ref;
		}
	};



	//
	// Function Optional Output Argument Helper
	//
	
	template <typename T>
	struct OutOp
	{
	// types
	public:
		typedef OutOp<T>	Self;
		typedef T			value_t;

		
	// variables
	private:
		T *		_ref;
		bool	_changed;
		bool	_defined;

		
	// methods
	public:
		OutOp (GX_DEFCTOR) : _ref(null), _changed(false), _defined(false)
		{}

		OutOp (T &ref) : _ref(&ref), _changed(false), _defined(true)
		{}

		OutOp (Out<T> &&other) : _ref(other._ref), _changed(other._changed), _defined(other._defined)
		{}

		OutOp (Out<T> &other) : _ref(other._ref), _changed(other._changed), _defined(other._defined)
		{}

		~OutOp ()
		{
			// result value must be initialized
			ASSERT( not _defined or _changed );
		}

		bool IsDefined () const
		{
			return _defined;
		}

		operator const T& () const
		{
			ASSERT( _defined );
			return *_ref;
		}

		operator T& ()
		{
			ASSERT( _defined );

			_changed = true;
			return *_ref;
		}

		Self& operator = (const T &value)
		{
			ASSERT( _defined );

			_changed = true;
			*_ref	 = value;
			return *this;
		}
		
		T & operator * ()
		{
			ASSERT( _defined );

			_changed = true;
			return *_ptr;
		}

		T * operator -> ()
		{
			ASSERT( _defined );

			_changed = true;
			return _ref;
		}
	};



	//
	// Function Input/Output Argument Helper
	//

	template <typename T>
	struct InOut
	{
	// types
	public:
		typedef InOut<T>	Self;
		typedef T			value_t;

		
	// variables
	private:
		T *		_ref;

		
	// methods
	public:
		InOut (T &ref) : _ref(&ref)
		{}

		InOut (Out<T> &&other) : _ref(other._ref)
		{}

		InOut (Out<T> &other) : _ref(other._ref)
		{}
		
		operator const T& () const
		{
			return *_ref;
		}

		operator T& ()
		{
			return *_ref;
		}

		Self& operator = (const T &value)
		{
			*_ref = value;
			return *this;
		}
		
		T & operator * ()
		{
			return *_ptr;
		}

		T * operator -> ()
		{
			return _ref;
		}
	};




}	// GXTypes
}	// GX_STL
