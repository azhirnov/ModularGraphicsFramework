// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Thread.h"

namespace Engine
{
namespace PlatformTools
{
	using namespace Platforms;


	//
	// Async Commands Emulator
	//

	class AsyncCommandsEmulator final
	{
	// types
	private:
		using SubmitedCommand_t		= GpuMsg::SubmitGraphicsQueueCommands;
		using PendingCommands_t		= Array< SubmitedCommand_t >;
		using SemaphoreSet_t		= Set< GpuSemaphoreId >;
		using SubmitQueue_t			= Delegate< bool (const SubmitedCommand_t &cmd) >;


	// variables
	private:
		PendingCommands_t	_pendingCommands;
		bool				_locked			= false;		// check for recursion


	// methods
	public:
		AsyncCommandsEmulator () {}

		void Add (const SubmitedCommand_t &cmd);

		bool Empty () const		{ return _pendingCommands.Empty(); }

		bool Run (GpuFenceId fenceId, const SubmitQueue_t &submitQueueFunc);

		bool RunAll (const SubmitQueue_t &submitQueueFunc);

	private:
		bool _RunAll (INOUT PendingCommands_t &commands, const SubmitQueue_t &submitQueueFunc, GpuFenceId breakOnFence = Uninitialized) const;
	};

	
/*
=================================================
	Add
=================================================
*/
	inline void AsyncCommandsEmulator::Add (const SubmitedCommand_t &cmd)
	{
		ModuleUtils::Send( cmd.commands, Message<GpuMsg::SetCommandBufferState>{ GpuMsg::SetCommandBufferState::EState::Pending });

		_pendingCommands.PushBack( cmd );
	}
	
/*
=================================================
	Run
=================================================
*/
	inline bool AsyncCommandsEmulator::Run (GpuFenceId fenceId, const SubmitQueue_t &submitQueueFunc)
	{
		CHECK_ERR( fenceId != GpuFenceId::Unknown );
		CHECK_ERR( not _locked );
		SCOPE_SETTER( _locked = true, false );

		return _RunAll( _pendingCommands, submitQueueFunc, fenceId );
	}
	
/*
=================================================
	RunAll
=================================================
*/
	inline bool AsyncCommandsEmulator::RunAll (const SubmitQueue_t &submitQueueFunc)
	{
		CHECK_ERR( not _locked );
		SCOPE_SETTER( _locked = true, false );

		return _RunAll( _pendingCommands, submitQueueFunc );
	}

	inline bool AsyncCommandsEmulator::_RunAll (INOUT PendingCommands_t &pendingCommands, const SubmitQueue_t &submitQueueFunc, GpuFenceId breakOnFence) const
	{
		// TODO: optimize
		SemaphoreSet_t	signaled_semaphores;
		SemaphoreSet_t	all_signal_semaphores;	
		SemaphoreSet_t	all_wait_semaphores;
		
		// get all signal & wait semaphores
		FOR( i, pendingCommands )
		{
			for (auto& wait_sem : Range(pendingCommands[i].waitSemaphores)) {
				ASSERT( wait_sem.second == EPipelineStage::AllCommands );
				all_wait_semaphores.Add( wait_sem.first );
			}
			all_signal_semaphores.AddArray( pendingCommands[i].signalSemaphores );
		}

		// sort & run
		for (usize iter = 0; iter < 10 and not pendingCommands.Empty(); ++iter)
		{
			FOR( i, pendingCommands )
			{
				auto&	cmd			= pendingCommands[i];
				bool	ready		= true;
				bool	maybe_ready	= true;

				// check wait semaphores
				for (auto& wait_sem : Range(cmd.waitSemaphores))
				{
					if ( not signaled_semaphores.IsExist( wait_sem.first ) )
					{
						ready = false;

						// waiting semaphore from other command
						if ( all_signal_semaphores.IsExist( wait_sem.first ) )
							maybe_ready = false;
					}
				}

				if ( ready						or
					 (maybe_ready and iter > 0)	or
					 iter > 2 )
				{
					signaled_semaphores.AddArray( cmd.signalSemaphores );
					CHECK_ERR( submitQueueFunc( cmd ) );

					bool	exit = (breakOnFence != GpuFenceId::Unknown and cmd.fence == breakOnFence);

					pendingCommands.Erase( i );
					--i;

					if ( exit )
						return true;
				}
			}
		}

		ASSERT( pendingCommands.Empty() );
		//pendingCommands.Clear();

		return true;
	}

}	// PlatformTools
}	// Engine
