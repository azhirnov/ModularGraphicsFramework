// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	AsyncMessage - lightweight async operation.
*/

#pragma once

#include "Engine/Base/Public/ModuleMessages.h"
#include "Engine/Base/Common/BaseObject.h"

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
		
		GX_DEFCOPYCTOR_ASSIGN( AsyncMessage );

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
	struct PushAsyncMessage : _MessageBase_
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
		PushAsyncMessage (const ModulePtr &target, T &&msg) :
			asyncMsg{LAMBDA( targ = target, m = RVREF(msg) ) (GlobalSystemsRef) { _Call<T>( targ, m ); }},
			target{ target->GetThreadID() },
			altTarget{ target->GetThreadID() }
		{}


	private:
		template <typename T>
		static void _Call (const ModulePtr &target, const T &msg)
		{
			target->Send<T>( msg );
		}
	};
	

	//
	// Add Task Scheduler to Manager
	//
	struct AddTaskSchedulerToManager final : AddToManager
	{
	// types
		using Func_t	= Delegate< usize (Base::AsyncMessage &&) >;		// must be internally synchronized function
		
	// variables
		ReadOnce< Func_t >		asyncPushMsg;
		
	// methods
		AddTaskSchedulerToManager (const ModulePtr &mod, Func_t &&func) :
			AddToManager{ mod }, asyncPushMsg( RVREF(func) )
		{}
	};

}	// ModuleMsg
}	// Engine
