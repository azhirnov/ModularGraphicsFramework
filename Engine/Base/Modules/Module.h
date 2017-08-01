// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
	// Base Module
	//

	class Module : public BaseObject
	{
	// types
	public:
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

	protected:
		using UntypedID_t			= ModuleMsg::UntypedID_t;
		using ModuleName_t			= ModuleMsg::ModuleName_t;
		using AttachedModules_t		= Array<Pair< ModuleName_t, ModulePtr >>;
		using ParentModules_t		= FixedSizeSet< ModulePtr, 16 >;

		template <typename ...Types>
		using MessageListFrom		= CompileTime::TypeListFrom< Message<Types>... >;

		// this messages available to Send()
		using SupportedMessages_t	= MessageListFrom< 
											ModuleMsg::AttachModule,
											ModuleMsg::DetachModule,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached,
											ModuleMsg::FindModule,
											ModuleMsg::ModulesDeepSearch,
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

		struct ModuleConfig
		{
			UntypedID_t		id;
			uint			maxParents;
		};

	private:
		class AttachModuleToManagerAsyncTask;
		class DetachModuleFromManagerAsyncTask;
		struct _FindModuleWithTypelist_Func;


	// variables
	private:
		MessageHandler					_msgHandler;
		ModulePtr						_manager;
		ParentModules_t					_parents;
		AttachedModules_t				_attachments;
		uint							_attachmentCounter;
		EState							_state;
		const ThreadID					_ownThread;		// TODO: use Atomic<> ?
		const UntypedID_t				_compId;
		const uint						_maxParents;
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

		template <typename CreateInfo>
		bool AddModule (StringCRef name, UntypedID_t id, const CreateInfo &createInfo);

		ModulePtr GetModule (UntypedID_t id);
		ModulePtr GetModule (StringCRef name);
		
		template <typename ...MsgTypes>
		ModulePtr GetModule ();

		Runtime::VirtualTypeList const&		GetSupportedMessages ()		const	{ return _supportedMessages; }
		Runtime::VirtualTypeList const&		GetSupportedEvents ()		const	{ return _supportedEvents; }
		UntypedID_t							GetModuleID ()				const	{ return _compId; }
		EState								GetState ()					const	{ return _state; }
		ThreadID							GetThreadID ()				const	{ return _ownThread; }

		static UntypedID_t					GetStaticID ()						{ return 0; }	// virtual

		
	// hidden methods
	protected:
		Module (const GlobalSystemsRef gs,
				const ModuleConfig &config,
				const Runtime::VirtualTypeList *msgTypes,
				const Runtime::VirtualTypeList *eventTypes);

		~Module ();

		bool _Attach (const ModuleName_t &name, const ModulePtr &unit);
		bool _Detach (const ModulePtr &unit);
		void _DetachAllAttachments ();
		void _DetachSelfFromParent (const ModulePtr &parent);
		void _DetachSelfFromAllParents ();

		bool _AttachSelfToManager (const ModulePtr &mngr, UntypedID_t id = 0, bool wait = true);
		void _DetachSelfFromManager ();

		void _SetManager (const ModulePtr &mngr);
		bool _SetState (EState newState);

		bool _ValidateMsgSubscriptions ();
		bool _ValidateAllSubscriptions ();

		bool _Compose (bool immutable);		// TODO: rename

		void _ClearMessageHandlers ();
		
		template <typename T>			bool _SendMsg (const Message<T> &msg);
		template <typename T>			bool _SendEvent (const Message<T> &msg);
		template <typename T>			bool _SendForEachAttachments (const Message<T> &msg);
		
		template <typename ...Types>	bool _SubscribeOnMsg (Types&& ...args);
		template <typename ...Types>	bool _SubscribeOnEvent (Types&& ...args);

		template <typename Typelist> static bool _FindModuleWithEvents (const AttachedModules_t &modules, OUT ModulePtr &result);
		template <typename Typelist> static bool _FindModuleWithMessages (const AttachedModules_t &modules, OUT ModulePtr &result);

		static bool _IsMutableState (EState state);		// is module editable?
		static bool _IsComposedState (EState state);	// is module completed?
		static bool _IsErrorState (EState state);		// is module initialization failed?

		ParentModules_t const&		_GetParents ()		const	{ return _parents; }
		ModulePtr const&			_GetManager ()		const	{ return _manager; }
		AttachedModules_t const&	_GetAttachments ()	const	{ return _attachments; }


	// message handlers with implementation
	protected:
		bool _OnModuleAttached_Impl (const Message< ModuleMsg::OnModuleAttached > &);
		bool _OnModuleDetached_Impl (const Message< ModuleMsg::OnModuleDetached > &);
		bool _AttachModule_Impl (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule_Impl (const Message< ModuleMsg::DetachModule > &);
		bool _FindModule_Impl (const Message< ModuleMsg::FindModule > &);
		bool _ModulesDeepSearch_Impl (const Message< ModuleMsg::ModulesDeepSearch > &);
		bool _Update_Impl (const Message< ModuleMsg::Update > &);
		bool _Link_Impl (const Message< ModuleMsg::Link > &);
		bool _Compose_Impl (const Message< ModuleMsg::Compose > &);
		bool _Delete_Impl (const Message< ModuleMsg::Delete > &);

	// message handlers without implementation
	protected:
		bool _AttachModule_Empty (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule_Empty (const Message< ModuleMsg::DetachModule > &);
		bool _OnManagerChanged_Empty (const Message< ModuleMsg::OnManagerChanged > &);
		bool _FindModule_Empty (const Message< ModuleMsg::FindModule > &);
		bool _ModulesDeepSearch_Empty (const Message< ModuleMsg::ModulesDeepSearch > &);
		bool _Update_Empty (const Message< ModuleMsg::Update > &);
		bool _Link_Empty (const Message< ModuleMsg::Link > &);
		bool _Compose_Empty (const Message< ModuleMsg::Compose > &);
		bool _Delete_Empty (const Message< ModuleMsg::Delete > &);
	};
	
	

/*
=================================================
	Send
=================================================
*/
	template <typename T>
	forceinline bool Module::Send (const Message<T> &msg)
	{
		// only sync message supported
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		return _msgHandler.Send( msg );
	}
	
/*
=================================================
	_SendMsg
----
	send message and in debug check if this message is supported,
	it is recomended to use this method instead of 'Send'
	when you send message to oneself.
=================================================
*/
	template <typename T>
	forceinline bool Module::_SendMsg (const Message<T> &msg)
	{
		// only sync message supported
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );

		if ( not GetSupportedMessages().HasType( TypeIdOf< Message<T> >() ) )
			RETURN_ERR( "Unsupported message type '" << ToString( TypeIdOf<T>() ) << "'" );

		return _msgHandler.Send( msg );
	}
		
/*
=================================================
	_SendEvent
----
	send message to subscribers (and to oneself if subscribed)
=================================================
*/
	template <typename T>
	forceinline bool Module::_SendEvent (const Message<T> &msg)
	{
		// only sync message supported
		CHECK_ERR( _ownThread == ThreadID::GetCurrent() );
		
		if ( not GetSupportedEvents().HasType( TypeIdOf< Message<T> >() ) )
			RETURN_ERR( "Unsupported event type '" << ToString( TypeIdOf<T>() ) << "'" );

		return _msgHandler.Send( msg.From( this ) );
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
----
	subscribe to messages from oneself.
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
----
	subscribe external handlers for messages from
	self module.
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

		AttachedModules_t	tmp = _attachments;		// TODO: optimize

		FOR( i, tmp )
		{
			tmp[i].second->Send( msg.From( this ) );
		}
		return true;
	}
	
/*
=================================================
	_FindModuleWithTypelist_Func
=================================================
*/
	struct Module::_FindModuleWithTypelist_Func
	{
	// variables
		const Runtime::VirtualTypeList&		_typelist;
		bool								result	= true;

	// methods
		_FindModuleWithTypelist_Func (Runtime::VirtualTypeList const& typelist) :
			_typelist( typelist )
		{}
		
		template <typename T, usize Index>
		void Process ()
		{
			result &= _typelist.HasType< T >();
		}
	};

/*
=================================================
	_FindModuleWithEvents
=================================================
*/
	template <typename Typelist>
	inline bool Module::_FindModuleWithEvents (const AttachedModules_t &modules, OUT ModulePtr &result)
	{
		STATIC_ASSERT( CompileTime::IsTypeList< Typelist > );

		FOR( i, modules )
		{
			_FindModuleWithTypelist_Func	func{ modules[i].second->GetSupportedEvents() };

			Typelist::RuntimeForEach( func );

			if ( func.result )
			{
				result = modules[i].second;
				return true;
			}
		}
		return false;
	}
		
/*
=================================================
	_FindModuleWithMessages
=================================================
*/
	template <typename Typelist>
	inline bool Module::_FindModuleWithMessages (const AttachedModules_t &modules, OUT ModulePtr &result)
	{
		STATIC_ASSERT( CompileTime::IsTypeList< Typelist > );

		FOR( i, modules )
		{
			_FindModuleWithTypelist_Func	func{ modules[i].second->GetSupportedMessages() };

			Typelist::RuntimeForEach( func );

			if ( func.result )
			{
				result = modules[i].second;
				return true;
			}
		}
		return false;
	}
	
/*
=================================================
	GetModule
=================================================
*/
	template <typename ...MsgTypes>
	inline ModulePtr Module::GetModule ()
	{
		ModulePtr	result;
		CHECK_ERR( _FindModuleWithMessages< MessageListFrom< MsgTypes... > >( _GetAttachments(), OUT result ) );
		return result;
	}
	
/*
=================================================
	SendTo
=================================================
*/
	template <typename MsgType>
	forceinline bool BaseObject::SendTo (const ModulePtr &target, const MsgType &msg) const
	{
		return target->Send( msg.From( BaseObjectPtr( const_cast<BaseObject *>(this) ) ) );
	}
	
}	// Base
}	// Engine
