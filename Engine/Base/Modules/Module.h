// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/Base/Common/CreateInfo.h"
#include "Engine/Base/Common/ModuleMessages.h"
#include "Engine/Base/Modules/Message.h"
#include "Engine/Base/Modules/MessageHandler.h"

namespace Engine
{
namespace Base
{

	//
	// Module interface
	//

	class Module : public BaseObject
	{
	// types
	protected:
		enum class EState
		{
			// it is recording state, add modules to build object.
			Initial = 0,

			// after call Link modules creates links for each other.
			Linked,

			// modules are linked and completely initialized, but can be modified.
			ComposedMutable,

			// modules are licked and completely initialized, all initial state commands will fail.
			ComposedImmutable,

			// object will be deleted, modules must be destroyed or detached.
			Deleting,

			// module initialized with errors
			LinkageFailed,
			ComposingFailed,
			IncompleteAttachment,	// current module can't find necessary modules.
		};


		using UntypedID_t			= ulong;
		using Modules_t				= Map< UntypedID_t, ModulePtr >;		// TODO: fixed map ?
		using ModulesRef_t			= ArrayCRef< Pair<UntypedID_t, ModulePtr> >;

		template <typename ...Types>
		using MessageListFrom		= CompileTime::TypeListFrom< Message<Types>... >;

		// this messages available to Send()
		using SupportedMessages_t	= MessageListFrom< 
											ModuleMsg::AttachModule,
											ModuleMsg::DetachModule,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached,
											ModuleMsg::FindModule,
											ModuleMsg::Link,
											ModuleMsg::Compose,
											ModuleMsg::Update,
											ModuleMsg::Delete
										>;

		// this messages available to Subscribe()
		using SupportedEvents_t		= MessageListFrom< 
											ModuleMsg::Link,
											ModuleMsg::Compose,
											ModuleMsg::Update,
											ModuleMsg::Delete
										>;
		
		class AttachModuleToManagerAsyncTask;
		class DetachModuleFromManagerAsyncTask;


	// variables
	private:
		MessageHandler					_msgHandler;
		ModulePtr						_manager;
		ModulePtr						_parent;
		Modules_t						_attachments;
		//bool							_lockAttachments;
		EState							_state;
		const ThreadID					_ownThread;		// TODO: use Atomic<> ?
		const UntypedID_t				_compId;
		const Runtime::VirtualTypeList&	_supportedMessages;
		const Runtime::VirtualTypeList&	_supportedEvents;


	// methods
	public:
		template <typename T>
		bool Send (const Message<T> &msg);

		template <typename ...Types>
		bool Subscribe (Types&& ...args);
		
		template <typename ...Types>
		bool Unsubscribe (Types&& ...args);
		
		void UnsubscribeAll (const ModulePtr &unit);

		template <typename CreateInfo>
		bool AddModule (UntypedID_t id, const CreateInfo &createInfo);

		ModulePtr GetModule (UntypedID_t id);
		

		Runtime::VirtualTypeList const&		GetSupportedMessages ()		const	{ return _supportedMessages; }
		Runtime::VirtualTypeList const&		GetSupportedEvents ()		const	{ return _supportedEvents; }
		UntypedID_t							GetModuleID ()				const	{ return _compId; }
		EState								GetState ()					const	{ return _state; }
		ThreadID							GetThreadID ()				const	{ return _ownThread; }

		static UntypedID_t					GetStaticID ()						{ return 0; }	// virtual

		
	// hidden methods
	protected:
		Module (const SubSystemsRef gs, UntypedID_t id,
				 const Runtime::VirtualTypeList *msgTypes,
				 const Runtime::VirtualTypeList *eventTypes);

		~Module ();

		bool _Attach (const ModulePtr &unit);
		bool _Detach (const ModulePtr &unit);
		void _DetachAllAttachments ();
		void _SelfDetach ();
		void _DeferredDelete (const TaskModulePtr &);

		void _SetManager (const ModulePtr &mngr);
		bool _SetState (EState newState);

		bool _ValidateMsgSubscriptions ();
		bool _ValidateAllSubscriptions ();
		
		template <typename T>			bool _SendMsg (const Message<T> &msg);
		template <typename T>			bool _SendEvent (const Message<T> &msg);
		template <typename T>			bool _SendForEachAttachments (const Message<T> &msg);
		
		template <typename ...Types>	bool _SubscribeOnMsg (Types&& ...args);
		template <typename ...Types>	bool _SubscribeOnEvent (Types&& ...args);

		static bool _IsMutableState (EState state);		// is module editable?
		static bool _IsComposedState (EState state);	// is module completed?
		static bool _IsErrorState (EState state);		// is module initialization failed?

		ModulePtr const&	_GetParent ()		const	{ return _parent; }
		ModulePtr const&	_GetManager ()		const	{ return _manager; }
		Modules_t const&	_GetAttachments ()	const	{ return _attachments; }


	// message handlers with implementation
	protected:
		void _OnModuleAttached (const Message< ModuleMsg::OnModuleAttached > &);
		void _OnModuleDetached (const Message< ModuleMsg::OnModuleDetached > &);
		void _AttachModule_Impl (const Message< ModuleMsg::AttachModule > &);
		void _DetachModule_Impl (const Message< ModuleMsg::DetachModule > &);
		void _FindModule_Impl (const Message< ModuleMsg::FindModule > &);
		void _Update_Impl (const Message< ModuleMsg::Update > &);
		void _Link_Impl (const Message< ModuleMsg::Link > &);
		void _Compose_Impl (const Message< ModuleMsg::Compose > &);
		void _Delete_Impl (const Message< ModuleMsg::Delete > &);

	// message handlers without implementation
	protected:
		void _AttachModule_Empty (const Message< ModuleMsg::AttachModule > &);
		void _DetachModule_Empty (const Message< ModuleMsg::DetachModule > &);
		void _FindModule_Empty (const Message< ModuleMsg::FindModule > &);
		void _Update_Empty (const Message< ModuleMsg::Update > &);
		void _Link_Empty (const Message< ModuleMsg::Link > &);
		void _Compose_Empty (const Message< ModuleMsg::Compose > &);
		void _Delete_Empty (const Message< ModuleMsg::Delete > &);
	};
	
	

/*
=================================================
	Send
=================================================
*/
	template <typename T>
	forceinline bool Module::Send (const Message<T> &msg)
	{
		return _SendMsg( msg );
	}
	
/*
=================================================
	_SendMsg
=================================================
*/
	template <typename T>
	forceinline bool Module::_SendMsg (const Message<T> &msg)
	{
		// only sync message supported
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );
		//ASSERT( GetSupportedMessages().HasType( TypeIdOf< Message<T> >() ) );

		return _msgHandler.Send( msg );
	}
		
/*
=================================================
	_SendEvent
=================================================
*/
	template <typename T>
	forceinline bool Module::_SendEvent (const Message<T> &msg)
	{
		// only sync message supported
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );
		ASSERT( GetSupportedEvents().HasType( TypeIdOf< Message<T> >() ) );

		return _msgHandler.Send( msg );
	}

/*
=================================================
	Subscribe
=================================================
*/
	template <typename ...Types>
	forceinline bool Module::Subscribe (Types&& ...args)
	{
		return _SubscribeOnEvent( FW<Types>( args )... );
	}

/*
=================================================
	Unsubscribe
=================================================
*/
	template <typename ...Types>
	forceinline bool Module::Unsubscribe (Types&& ...args)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		_msgHandler.Unsubscribe( FW<Types>( args )... );
		return true;
	}
	
/*
=================================================
	_SubscribeOnMsg
=================================================
*/
	template <typename ...Types>
	forceinline bool Module::_SubscribeOnMsg (Types&& ...args)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		_msgHandler.Subscribe( GetSupportedMessages(), FW<Types>( args )... );
		return true;
	}
		
/*
=================================================
	_SubscribeOnEvent
=================================================
*/
	template <typename ...Types>
	forceinline bool Module::_SubscribeOnEvent (Types&& ...args)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		_msgHandler.Subscribe( GetSupportedEvents(), FW<Types>( args )... );
		return true;
	}

/*
=================================================
	_SendForEachAttachments
=================================================
*/
	template <typename DataType>
	forceinline bool Module::_SendForEachAttachments (const Message<DataType> &msg)
	{
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );
		//CHECK_ERR( not _lockAttachments );
		//SCOPE_SETTER( _lockAttachments = true, false );

		Modules_t	tmp = _attachments;

		FOR( i, tmp )
		{
			tmp[i].second->Send( msg );
		}
		return true;
	}
	
}	// Base
}	// Engine
