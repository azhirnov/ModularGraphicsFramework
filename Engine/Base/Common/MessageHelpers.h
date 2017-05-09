// Copyright �  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/Common.h"

namespace Engine
{
namespace ModuleMsg
{
	
	//
	// Message Output
	//
	template <typename T>
	struct Out
	{
	// variables
	protected:
		mutable Optional<T>		_value;


	// methods
	protected:
		explicit Out (const T& val) : _value(val) {}
		explicit Out (T &&val) : _value( FW<T>(val) ) {}

	public:
		Out (GX_DEFCTOR) : _value() {}
		Out (Out &&) = default;
		Out (const Out &) = default;

		void Set (const T &val)	const	{ _value = val; }
		void Set (T &&val)		const	{ _value = FW<T>(val); }

		// available only for sender
		Optional<T>&	Get ()			{ return _value; }
	};


	//
	// Message Input/Output
	//
	template <typename T>
	struct InOut : Out<T>
	{
	// methods
	public:
		InOut (const T& val) : Out(val) {}
		InOut (T &&val) : Out( FW<T>(val) ) {}

		InOut () : Out() {}
		InOut (InOut &&) = default;
		InOut (const InOut &) = default;

		Optional<T> const&	Get ()	const	{ return _value; }
	};


	//
	// Message Response
	//
	template <typename T>
	struct Response
	{
	// types
	public:
		using Callback = Delegate< void (const T&) >;


	// variables
	private:
		mutable Callback	_cb;


	// methods
	public:
		explicit Response (const Callback &cb) : _cb(cb) {}
		explicit Response (Callback &&cb) : _cb(RVREF(cb)) {}

		Response (const Response &) = default;
		Response (Response &&) = default;
		Response () = delete;

		void Call (const T &data) const		{ return _cb.SafeCall( data ); }
	};


	//
	// Message Input with single read op (move-ctor available)
	//
	template <typename T>
	struct SingleRead
	{
	// variables
	private:
		mutable T		_value;
		mutable bool	_isDefined = false;

	// methods
	public:
		SingleRead (T &&value) : _value(RVREF(value)), _isDefined(true) {}
		SingleRead (SingleRead &&other) : _value(RVREF(other._value)), _isDefined(other._isDefined) { other._isDefined = false; }
		SingleRead (const SingleRead &other) : _value(RVREF(other._value)), _isDefined(other._isDefined) { other._isDefined = false; }

		T& Get () const
		{
			ASSERT( _isDefined );
			_isDefined = false;
			return _value;
		}
	};


	//
	// Message Output with single write op
	//
	template <typename T>
	struct SingleWrite
	{
	// variables
	private:
		mutable Optional<T>		_value;

	// methods
	public:
		SingleWrite () : _value() {}

		void Set (T &&value) const
		{
			ASSERT( not _value.IsDefined() );

			if ( not _value.IsDefined() )
				_value = RVREF( value );
		}

		Optional<T>& Get ()
		{
			return _value;
		}
	};


	//
	// Flags (TODO)
	//
	/*struct EMessageFlags
	{
		enum type : uint
		{
			NoAsync = 0,	// 
			NoResend,
			NoDiscard,

			_Count,
			Unknown	= uint(-1)
		};

		using bits = EnumBitfield< EMessageFlags >;
	};*/


}	// ModuleMsg
}	// Engine