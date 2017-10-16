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
		using Func_t	= std::function< void (const TaskModulePtr &) >;	// TODO: change to ModulePtr


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
		explicit AsyncMessage (FN func, Args&& ...args) noexcept :
			_func(std::bind( func, FW<Args>(args)..., std::placeholders::_1 ))
		{}
		
		AsyncMessage (Self &&)			= default;
		AsyncMessage (const Self &)		= default;

		Self& operator = (Self &&)		= default;
		Self& operator = (const Self &)	= default;


		void Process (const TaskModulePtr &task) const noexcept
		{
			ASSERT( bool(_func) );
			return _func( task );
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
		SingleRead< Base::AsyncMessage >	asyncMsg;
		Base::ThreadID						target;
		Base::ThreadID						altTarget;

	// methods
		PushAsyncMessage (Base::AsyncMessage &&msg, Base::ThreadID target) :
			asyncMsg( RVREF(msg) ), target(target), altTarget(target)
		{}

		PushAsyncMessage (Base::AsyncMessage &&msg, Base::ThreadID target, Base::ThreadID altTarget) :
			asyncMsg( RVREF(msg) ), target(target), altTarget(altTarget)
		{}

		template <typename T>
		PushAsyncMessage (const ModulePtr &target, Base::Message<T> &&msg) :
			asyncMsg( std::bind( &_Call<T>, target, RVREF( msg ), std::placeholders::_1 ) ),
			target(target->GetThreadID()),
			altTarget(target->GetThreadID())
		{}


	private:
		template <typename T>
		static void _Call (const ModulePtr &target, const Base::Message<T> &msg, const Base::TaskModulePtr &)
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

		SingleRead< Func_t >	asyncPushMsg;

		AddTaskSchedulerToManager (const ModulePtr &mod, Func_t &&func) :
			AddToManager{ mod }, asyncPushMsg( RVREF(func) )
		{}
	};

}	// ModuleMsg
}	// Engine
