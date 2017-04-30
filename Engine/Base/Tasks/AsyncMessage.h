// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	AsyncMessage - lightweight async operation.
*/

#pragma once

#include "Engine/Base/Common/ModuleMessages.h"
#include "Engine/Base/Modules/Message.h"

namespace Engine
{
namespace Base
{

	//
	// Async Message
	//

	class AsyncMessage final
	{
	// types
	public:
		using Self		= AsyncMessage;
		using Func_t	= std::function< void (const TaskModulePtr &) >;


	// variables
	private:
		Func_t		_func;


	// methods
	public:
		AsyncMessage (GX_DEFCTOR) noexcept : _func()
		{}

		explicit AsyncMessage (Func_t &&func) noexcept : _func( RVREF(func) )
		{}
		
		template <typename FN>
		explicit AsyncMessage (FN value) noexcept : _func(value)
		{}

		template <typename FN, typename ...Args>
		explicit AsyncMessage (FN func, Args ...args) noexcept :
			_func(std::bind( func, FW<Args>(args)..., std::placeholders::_1 ))
		{}
		
		AsyncMessage (Self &&)			= default;
		AsyncMessage (const Self &)		= default;

		Self& operator = (Self &&)		= default;
		Self& operator = (const Self &)	= default;


		void Process (const TaskModulePtr &thread) const noexcept
		{
			ASSERT( bool(_func) );
			return _func( thread );
		}
	};

	
}	// Base

namespace ModuleMsg
{

	//
	// Push Async Message
	//
	struct PushAsyncMessage
	{
	// variables
		SingleRead< AsyncMessage >	asyncMsg;
		ThreadID					target;
		ThreadID					altTarget;

	// methods
		PushAsyncMessage (AsyncMessage &&msg, ThreadID target, ThreadID altTarget = ThreadID()) :
			asyncMsg( RVREF(msg) ), target(target), altTarget(altTarget)
		{}

		template <typename T>
		PushAsyncMessage (const ModulePtr &target, Message<T> &&msg) :
			asyncMsg( std::bind( &_Call<T>, target, RVREF( msg ), std::placeholders::_3 ) ),
			target(target->GetThreadID()),
			altTarget()
		{}


	private:
		template <typename T>
		static void _Call (const ModulePtr &target, const Message<T> &msg, const TaskModulePtr &) { target->Send<T>( msg ); }
	};

}	// ModuleMsg
}	// Engine
