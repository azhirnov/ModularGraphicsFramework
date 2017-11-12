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
		using Func_t	= std::function< void (GlobalSystemsRef) >;


	// variables
	private:
		Func_t		_func;


	// methods
	public:
		AsyncMessage (GX_DEFCTOR) : _func{}
		{}

		explicit AsyncMessage (Func_t &&func) : _func{ RVREF(func) }
		{}
		
		template <typename FN>
		explicit AsyncMessage (FN value) : _func{value}
		{}

		template <typename FN, typename ...Args>
		explicit AsyncMessage (FN func, Args&& ...args) :
			_func{std::bind( func, FW<Args>(args)..., std::placeholders::_1 )}
		{}
		
		AsyncMessage (Self &&)			= default;
		AsyncMessage (const Self &)		= default;

		Self& operator = (Self &&)		= default;
		Self& operator = (const Self &)	= default;


		void Process (GlobalSystemsRef gs) const noexcept
		{
			ASSERT( bool(_func) );
			return _func( gs );
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
		ReadOnce< Base::AsyncMessage >		asyncMsg;
		Base::ThreadID						target;
		Base::ThreadID						altTarget;

	// methods
		PushAsyncMessage (Base::AsyncMessage &&msg, Base::ThreadID target) :
			asyncMsg{ RVREF(msg) }, target{ target }, altTarget{ target }
		{}

		PushAsyncMessage (Base::AsyncMessage &&msg, Base::ThreadID target, Base::ThreadID altTarget) :
			asyncMsg{ RVREF(msg) }, target{ target }, altTarget{ altTarget }
		{}

		template <typename T>
		PushAsyncMessage (const ModulePtr &target, Base::Message<T> &&msg) :
			asyncMsg{LAMBDA( targ = target, m = RVREF(msg) ) (GlobalSystemsRef) { _Call<T>( targ, m ); }},
			target{ target->GetThreadID() },
			altTarget{ target->GetThreadID() }
		{}


	private:
		template <typename T>
		static void _Call (const ModulePtr &target, const Base::Message<T> &msg)
		{
			target->Send<T>( msg );
		}
	};
	

	//
	// Add Task Scheduler to Manager
	//
	struct AddTaskSchedulerToManager : AddToManager
	{
		using Func_t			= Delegate< usize (Base::AsyncMessage &&) >;		// must be internally synchronized function

		ReadOnce< Func_t >		asyncPushMsg;

		AddTaskSchedulerToManager (const ModulePtr &mod, Func_t &&func) :
			AddToManager{ mod }, asyncPushMsg( RVREF(func) )
		{}
	};

}	// ModuleMsg
}	// Engine
