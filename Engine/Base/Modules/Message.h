// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/BaseObject.h"

namespace Engine
{
namespace Base
{

	//
	// Message
	//

	template <typename T>
	struct Message final
	{
		//friend class Module;
		friend class MessageHandler;

	// types
	public:
		using Sender_t		= BaseObjectPtr;
		using Self			= Message<T>;


	// variables
	private:
		T					_data;
		mutable Sender_t	_sender;
		mutable usize		_numOfSends	= 0;
		mutable bool		_async		= false;


	// methods
	public:
		Message()
		{}
		
		Message (const Message<T> &other) :
			_data( other._data ),
			_sender( other._sender ),
			_numOfSends( 0 )
		{}

		Message (Message<T> &&other) :
			_data( RVREF( other._data ) ),
			_sender( RVREF( other._sender ) ),
			_numOfSends( other._numOfSends )
		{
			other._sender		= null;
			other._numOfSends	= 0;
		}

		template <typename ...ArgTypes>
		Message (ArgTypes&& ...args) : _data{ FW<ArgTypes>( args )... }
		{
			STATIC_ASSERT(( not CompileTime::IsSameTypesWithoutQualifiers< typename CompileTime::TypeListFrom<ArgTypes...>::Front, Self > ));
		}

		// set async flag to disable default checks for thread and write your own synchronization
		Self &	Async (bool isAsync = true)
		{
			_async = isAsync;
			return *this;
		}

		Self &	From (const Sender_t sender)
		{
			_sender = sender;
			return *this;
		}

		Self const&	From (const Sender_t sender) const
		{
			_sender = sender;
			return *this;
		}

		T const&		Data ()				const	{ return _data; }
		bool			IsDiscarded ()		const	{ return _numOfSends == 0; }
		bool			IsAsync ()			const	{ return _async; }
		Sender_t const&	Sender ()			const	{ return _sender; }

		T const *		operator -> ()		const	{ return &_data; }
		T *				operator -> ()				{ return &_data; }

		T const &		operator * ()		const	{ return _data; }
		T &				operator * ()				{ return _data; }
	};

	
	/*
	Examples:

		// Simple message
		struct Data
		{
			int	i;
		};

		Events().Send( Data{ 0 } );


		// Message with output
		struct Data
		{
			int						i;
			Message::Out<float>		f;
			Message::InOut<uint>	u;
		};

		Events().Send( Data{ 0, Uninitialized, 1u } );


		// Message with response
		struct Data
		{
			int						i;
			Message::Response<int>	r;
		};

		Events().Send( Data{ 0, DelegateBuilder( this, &OnResponse ) } );
	*/

}	// Base
}	// Engine
