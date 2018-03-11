// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Profilers/Public/GpuStatistic.h"
#include "Engine/Platforms/Public/OS/Window.h"
#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/Platforms/Public/GPU/VR.h"
#include "Engine/Profilers/Impl/ProfilerObjectsConstructor.h"
#include "Engine/Platforms/Public/Tools/GPUThreadHelper.h"

namespace Engine
{
namespace Profilers
{

	//
	// FPS Counter
	//

	class FPSCounter : public Module
	{
	// types
	protected:
		using SupportedMessages_t	= MessageListFrom<
											ModuleMsg::AttachModule,
											ModuleMsg::DetachModule,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached,
											ModuleMsg::Link,
											ModuleMsg::Delete
										>;

		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Delete
										>;

		using WindowMsgList_t		= MessageListFrom<
											OSMsg::WindowGetDescriptor,
											OSMsg::WindowSetDescriptor >;

		
	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		const TimeD				_interval;
		TimeProfilerD			_timer;
		uint					_frameCounter;


	// methods
	public:
		FPSCounter (GlobalSystemsRef gs, const CreateInfo::FPSCounter &);
		~FPSCounter ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);

		// event handlers
		bool _OnThreadEndFrame (const Message< GpuMsg::ThreadEndFrame > &);
		bool _OnThreadEndVRFrame (const Message< GpuMsg::ThreadEndVRFrame > &);

		void _Update ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	FPSCounter::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	FPSCounter::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	FPSCounter::FPSCounter (GlobalSystemsRef gs, const CreateInfo::FPSCounter &ci) :
		Module( gs, ModuleConfig{ FPSCounterModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_interval{ ci.interval },	_frameCounter{ 0 }
	{
		SetDebugName( "FPSCounter" );

		_SubscribeOnMsg( this, &FPSCounter::_AttachModule_Impl );
		_SubscribeOnMsg( this, &FPSCounter::_DetachModule_Impl );
		_SubscribeOnMsg( this, &FPSCounter::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &FPSCounter::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &FPSCounter::_Link );
		_SubscribeOnMsg( this, &FPSCounter::_Delete );
	}
	
/*
=================================================
	destructor
=================================================
*/
	FPSCounter::~FPSCounter ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool FPSCounter::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );

		// must be attached to window
		CHECK_LINKING( _GetParents().Count() == 1 );
		CHECK_LINKING( _GetParents().Front()->GetSupportedMessages().HasAllTypes< WindowMsgList_t >() );

		// find gpu thread
		ModulePtr	gthread;
		//if ( not (gthread = GlobalSystems()->parallelThread->GetModuleByMsg< VRThreadMsgList_t >()) )
		if ( not (gthread = PlatformTools::GPUThreadHelper::FindVRThread( GlobalSystems() )) )
		{
			CHECK_ERR(( gthread = PlatformTools::GPUThreadHelper::FindGraphicsThread( GlobalSystems() ) ));
			gthread->Subscribe( this, &FPSCounter::_OnThreadEndFrame );
		}
		else
			gthread->Subscribe( this, &FPSCounter::_OnThreadEndVRFrame );

		_timer.Start();

		return Module::_Link_Impl( msg );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool FPSCounter::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_OnThreadEndFrame
=================================================
*/
	bool FPSCounter::_OnThreadEndFrame (const Message< GpuMsg::ThreadEndFrame > &)
	{
		_Update();
		return true;
	}
	
	bool FPSCounter::_OnThreadEndVRFrame (const Message< GpuMsg::ThreadEndVRFrame > &)
	{
		_Update();
		return true;
	}

/*
=================================================
	_Update
=================================================
*/
	void FPSCounter::_Update ()
	{
		double	dt = _timer.GetTimeDelta().Seconds();

		++_frameCounter;

		if ( dt > _interval.Seconds() )
		{
			ModulePtr	window = _GetParents().Front();

			Message< OSMsg::WindowGetDescriptor >	req_descr;
			window->Send( req_descr );

			String&	str = req_descr->result->caption;
			usize	pos = 0;
			uint	fps	= uint(GXMath::Round(double(_frameCounter) / dt));

			if ( str.FindIC( " [FPS", OUT pos, 0 ) )
			{
				usize	end = 0;
				str.Find( ']', OUT end, pos );

				str.Erase( pos, end - pos + 1 );
			}

			str << " [FPS: " << fps << "]";

			window->Send< OSMsg::WindowSetDescriptor >({ *req_descr->result }); 

			_timer.Start();
			_frameCounter = 0;
		}
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateFPSCounter
=================================================
*/
	ModulePtr ProfilerObjectsConstructor::CreateFPSCounter (GlobalSystemsRef gs, const CreateInfo::FPSCounter &ci)
	{
		return New< FPSCounter >( gs, ci );
	}

}	// Profilers
}	// Engine
