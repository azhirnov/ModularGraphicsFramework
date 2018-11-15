// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/ThreadSafe/AtomicFlag.h"
#include "Engine/Base/Public/AsyncMessage.h"
#include "Engine/Base/Modules/Module.h"
#include "Engine/Base/Public/TaskModule.h"
#include "Engine/Base/Main/MainSystem.h"

namespace Engine
{
namespace Base
{

	//
	// Async Task
	//

	template <	typename ResultT	= UninitializedType,
				typename ProgressT	= UninitializedType
			 >
	class AsyncTask : public BaseObject
	{
	// types
	public:
		using Result_t		= ResultT;
		using Progress_t	= ProgressT;
		using Self			= AsyncTask< Result_t, Progress_t >;
		using SelfPtr		= SharedPointerType<Self>;


	// variables
	private:
		ModulePtr				_currentThreadModule;		// module in thread where waiting task result and updates progress 
		ModulePtr				_targetThreadModule;		// module in thread where task will be schedule

		mutable SyncEvent		_event;
		mutable AtomicFlag		_isCanceled;
		mutable AtomicFlag		_onCanceledCalled;
		const bool				_isSync;					// if current and target threads are same

		Result_t				_result;


	// methods
	public:
		Self* Execute ()	noexcept;
		void  Cancel ()		noexcept;
		bool  Wait ()		const noexcept;
		bool  IsCanceled ()	const noexcept;
		
		GlobalSystemsRef	GlobalSystems () const	= delete;


	protected:
		AsyncTask (const ModulePtr &currentThreadModule, const ModulePtr &targetThreadModule);
		~AsyncTask ();

		void PublishProgress (Progress_t &&) noexcept;

		ND_ ModulePtr const&	CurrentThreadModule ()		{ return _currentThreadModule; }
		ND_ ModulePtr const&	TargetThreadModule ()		{ return _targetThreadModule; }


	private:
		void _RunSync () noexcept;
		void _RunAsync (GlobalSystemsRef) noexcept;
		void _OnCanceled (GlobalSystemsRef = GlobalSystemsRef(null)) noexcept;
		void _UpdateProgress (GlobalSystemsRef, Progress_t &&) noexcept;
		void _PostExecute (GlobalSystemsRef) noexcept;


	// interface
	protected:
		// in current thread //
		virtual void PreExecute (const ModulePtr &)						{}
		virtual void UpdateProgress (const ModulePtr &, Progress_t &&)	{}
		virtual void PostExecute (const ModulePtr &, Result_t &&)		{}
		virtual void OnCanceled (const ModulePtr &)						{}
		
		// in target thread //
		virtual void ExecuteInBackground (const ModulePtr &, OUT Result_t &) = 0;
	};

	
/*
=================================================
	constructor
=================================================
*/
	template <typename R, typename P>
	inline AsyncTask<R,P>::AsyncTask (const ModulePtr &currentThreadModule, const ModulePtr &targetThreadModule) :
		BaseObject{ currentThreadModule->GlobalSystems() },
		_currentThreadModule{ currentThreadModule },
		_targetThreadModule{ targetThreadModule },
		_event{ SyncEvent::MANUAL_RESET },
		_isCanceled{ false },
		_onCanceledCalled{ false },
		_isSync{ _targetThreadModule->GetThreadID() == _currentThreadModule->GetThreadID() },
		_result{}
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	template <typename R, typename P>
	inline AsyncTask<R,P>::~AsyncTask ()
	{
		_OnCanceled();

		CHECK( Wait() );
	}
	
/*
=================================================
	Wait
=================================================
*/
	template <typename R, typename P>
	inline bool AsyncTask<R,P>::Wait () const noexcept
	{
		// TODO: add profiling
		return _event.Wait( /*TimeL::FromSeconds( 60 )*/ );
	}
		
/*
=================================================
	Execute
=================================================
*/
	template <typename R, typename P>
	inline AsyncTask<R,P>*  AsyncTask<R,P>::Execute () noexcept
	{
		ASSERT( _currentThreadModule->GetThreadID() == ThreadID::GetCurrent() );
		
		if ( IsCanceled() )
		{
			_OnCanceled();
			return this;
		}

		PreExecute( _currentThreadModule );

		if ( IsCanceled() )
		{
			_OnCanceled();
			return this;
		}

		// target thread is current
		if ( _isSync )
		{
			_RunSync();
			return this;
		}

		auto	task_mod = _currentThreadModule->GlobalSystems()->taskModule;
		CHECK_ERR( task_mod, this );

		CHECK( task_mod->SendAsync( ModuleMsg::PushAsyncMessage{
					_targetThreadModule->GetThreadID(),
					&AsyncTask::_RunAsync,
					SelfPtr(this)
				}));
		return this;
	}
	
/*
=================================================
	Cancel
=================================================
*/
	template <typename R, typename P>
	inline void AsyncTask<R,P>::Cancel () noexcept
	{
		_isCanceled = true;
	}
	
/*
=================================================
	IsCanceled
=================================================
*/
	template <typename R, typename P>
	inline bool AsyncTask<R,P>::IsCanceled () const noexcept
	{
		return bool(_isCanceled);
	}

/*
=================================================
	PublishProgress
=================================================
*/
	template <typename R, typename P>
	inline void AsyncTask<R,P>::PublishProgress (Progress_t &&value) noexcept
	{
		ASSERT( _targetThreadModule->GetThreadID() == ThreadID::GetCurrent() );

		// target thread is current
		if ( _isSync )
		{
			UpdateProgress( _currentThreadModule, RVREF( value ) );
			return;
		}
		
		auto	task_mod = _targetThreadModule->GlobalSystems()->taskModule;
		CHECK_ERR( task_mod, );

		CHECK( task_mod->SendAsync( ModuleMsg::PushAsyncMessage{
					_currentThreadModule->GetThreadID(),
					&AsyncTask::_UpdateProgress,
					SelfPtr(this),
					RVREF(value)
				}));
	}
	
/*
=================================================
	_RunSync
=================================================
*/
	template <typename R, typename P>
	inline void AsyncTask<R,P>::_RunSync () noexcept
	{
		if ( IsCanceled() )
		{
			_OnCanceled();
			return;
		}

		ExecuteInBackground( _targetThreadModule, OUT _result );
		
		if ( IsCanceled() )
		{
			_OnCanceled();
			return;
		}

		_event.Signal();

		PostExecute( _currentThreadModule, RVREF( _result ) );
	}
	
/*
=================================================
	_RunAsync
=================================================
*/
	template <typename R, typename P>
	inline void AsyncTask<R,P>::_RunAsync (GlobalSystemsRef) noexcept
	{
		ASSERT( _targetThreadModule->GetThreadID() == ThreadID::GetCurrent() );

		auto	task_mod = _targetThreadModule->GlobalSystems()->taskModule;
		CHECK_ERR( task_mod, void() );

		if ( IsCanceled() )
		{
			CHECK( task_mod->SendAsync( ModuleMsg::PushAsyncMessage{
						_currentThreadModule->GetThreadID(),
						&AsyncTask::_OnCanceled,
						SelfPtr(this)
					}));
			return;
		}
		
		ExecuteInBackground( _targetThreadModule, OUT _result );
		
		if ( IsCanceled() )
		{
			CHECK( task_mod->SendAsync( ModuleMsg::PushAsyncMessage{
						_currentThreadModule->GetThreadID(),
						&AsyncTask::_OnCanceled,
						SelfPtr(this)
					}));
		}
		else
		{
			CHECK( task_mod->SendAsync( ModuleMsg::PushAsyncMessage{
						_currentThreadModule->GetThreadID(),
						&AsyncTask::_PostExecute,
						SelfPtr(this)
					}));
		}

		// unlock '_currentThreadModule' thread 
		_event.Signal();
	}
	
/*
=================================================
	_UpdateProgress
=================================================
*/
	template <typename R, typename P>
	inline void AsyncTask<R,P>::_UpdateProgress (GlobalSystemsRef, Progress_t &&value) noexcept
	{
		ASSERT( _currentThreadModule->GetThreadID() == ThreadID::GetCurrent() );

		UpdateProgress( _currentThreadModule, RVREF( value ) );
	}
		
/*
=================================================
	_PostExecute
=================================================
*/
	template <typename R, typename P>
	inline void AsyncTask<R,P>::_PostExecute (GlobalSystemsRef) noexcept
	{
		ASSERT( _currentThreadModule->GetThreadID() == ThreadID::GetCurrent() );

		PostExecute( _currentThreadModule, RVREF( _result ) );
	}
	
/*
=================================================
	_OnCanceled
=================================================
*/
	template <typename R, typename P>
	inline void AsyncTask<R,P>::_OnCanceled (GlobalSystemsRef) noexcept
	{
		ASSERT( _currentThreadModule->GetThreadID() == ThreadID::GetCurrent() );

		if ( _onCanceledCalled )
			return;

		_onCanceledCalled = true;

		OnCanceled( _currentThreadModule );

		_event.Signal();
	}


}	// Base
}	// Engine
