// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/Common.h"

namespace Engine
{
namespace _BaseMessages_
{
	
	//
	// Message Output
	//
	template <typename T, bool IsOptional>
	struct _Out
	{
	// variables
	protected:
		mutable Optional<T>			_value;
		DEBUG_ONLY( mutable bool	_wasUsed = false );


	// methods
	protected:
		explicit _Out (const T& val) : _value(val) {}
		explicit _Out (T &&val) : _value( FW<T>(val) ) {}

	public:
		_Out () {}
		_Out (_Out &&) = default;
		_Out (const _Out &) = default;

		~_Out ()								{ if_constexpr( not IsOptional ) { ASSERT( not _value.IsDefined() or _wasUsed ); }}

		void Set (const T &val)		const		{ DEBUG_ONLY( _wasUsed = false );  _value = val; }
		void Set (T &&val)			const		{ DEBUG_ONLY( _wasUsed = false );  _value = FW<T>(val); }

		// available only for sender
		ND_ bool			IsDefined ()		{ return _value.IsDefined(); }
		ND_ Optional<T> &	GetOptional ()		{ DEBUG_ONLY( _wasUsed = true );  return _value; }
		ND_ T				Get (const T& def)	{ DEBUG_ONLY( _wasUsed = true );  return _value.Get( def ); }

		ND_ T *				operator -> ()		{ DEBUG_ONLY( _wasUsed = true );  return _value.GetPtr(); }
		ND_ T &				operator * ()		{ DEBUG_ONLY( _wasUsed = true );  return _value.Get(); }

		ND_ explicit operator bool ()			{ return IsDefined(); }

		bool MoveTo (OUT T &dst)
		{
			if ( _value.IsDefined() )
			{
				dst = RVREF( _value.Get() );
				_value.Undefine();
				return true;
			}
			return false;
		}

		bool MoveTo (OUT Optional<T> &dst)
		{
			dst.Undefine();

			if ( _value.IsDefined() )
			{
				dst = RVREF( _value );
				return true;
			}
			return false;
		}
	};

	template <typename T>
	using Out		= _Out< T, false >;

	template <typename T>
	using Out_opt	= _Out< T, true >;



	//
	// Message Input/Output
	//
	template <typename T>
	struct InOut : Out<T>
	{
	// methods
	public:
		InOut (const T& val) : Out<T>(val) {}
		InOut (T &&val) : Out<T>( FW<T>(val) ) {}

		InOut () : Out<T>() {}
		InOut (InOut &&) = default;
		InOut (const InOut &) = default;
		
		// available for sender and receiver
		ND_ bool			IsDefined ()	const	{ return this->_value.IsDefined(); }
		ND_ Optional<T> &	GetOptional ()	const	{ return this->_value; }
		ND_ T &				Get ()			const	{ return this->_value.Get(); }
		
		ND_ T *				operator -> ()	const	{ return this->_value.GetPtr(); }
		ND_ T &				operator * ()	const	{ return this->_value.Get(); }

		ND_ explicit operator bool ()		const	{ return IsDefined(); }
	};


	//
	// Message Response
	//
	template <typename ...Args>
	struct Response
	{
	// types
	public:
		using Callback = Delegate< void (Args&&...) >;


	// variables
	private:
		mutable Callback	_cb;


	// methods
	public:
		explicit Response (const Callback &cb) : _cb(cb) {}
		explicit Response (Callback &&cb) : _cb(RVREF(cb)) {}

		GX_DEFCOPYCTOR_ONLY( Response );

		void Call (Args&& ...args) const	{ return _cb.SafeCall( FW<Args>( args )... ); }
	};


	//
	// Message Input with single read op (move-ctor available)
	//
	template <typename T>
	struct ReadOnce
	{
	// variables
	private:
		mutable T		_value;
		mutable bool	_isDefined = false;

	// methods
	public:
		ReadOnce (T &&value) : _value(RVREF(value)), _isDefined(true) {}
		ReadOnce (ReadOnce &&other) : _value(RVREF(other._value)), _isDefined(other._isDefined) { other._isDefined = false; }
		ReadOnce (const ReadOnce &other) : _value(RVREF(other._value)), _isDefined(other._isDefined) { other._isDefined = false; }

		ND_ T &  Get () const
		{
			ASSERT( _isDefined );
			_isDefined = false;
			return _value;
		}
	};


	//
	// Message Output with single write op (move-ctor available)
	//
	template <typename T>
	struct WriteOnce
	{
	// variables
	private:
		mutable Optional<T>		_value;

	// methods
	public:
		WriteOnce () : _value() {}

		void Set (T &&value) const
		{
			ASSERT( not _value.IsDefined() );

			if ( not _value.IsDefined() )
				_value = RVREF( value );
		}

		ND_ Optional<T> &  Get ()
		{
			return _value;
		}
	};


	//
	// Editable message input/output
	//
	template <typename T>
	struct Editable
	{
	// types
	public:
		using Value_t	= T;
		using Self		= Editable<T>;

	// variables
	private:
		mutable T	_value;

	// methods
	public:
		Editable (GX_DEFCTOR) : _value{} {}

		explicit Editable (const T &value) : _value{value} {}
		explicit Editable (T &&value) : _value(RVREF(value)) {}

		void operator = (const T &value) const	{ _value = value; }
		void operator = (T &&value) const		{ _value = RVREF(value); }
		
		_GX_DIM_CMP_OPERATORS_TYPE( _value, T, );

		ND_ T *	operator -> () const			{ return &_value; }
		ND_ T &	operator * () const				{ return _value; }
	};


}	// _BaseMessages_
}	// Engine
