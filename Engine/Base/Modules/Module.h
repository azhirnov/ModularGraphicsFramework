// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Modules/CreateInfo.h"
#include "Engine/Base/Modules/ModuleMessages.h"
#include "Engine/Base/Modules/Message.h"
#include "Engine/Base/Modules/MessageHandler.h"
#include "Engine/Base/Modules/ProfilingMessages.h"

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
			LinkingFailed,
			ComposingFailed,
			IncompleteAttachment,	// current module can't find necessary modules.
		};

	protected:
		using UntypedID_t			= ModuleMsg::UntypedID_t;
		using ModuleName_t			= ModuleMsg::ModuleName_t;
		using AttachedModules_t		= Array<Pair< ModuleName_t, ModulePtr >>;
		using ParentModules_t		= Set< ModulePtr >;	//FixedSizeSet< ModulePtr, 16 >;

		template <typename ...Types>
		using MessageListFrom		= CompileTime::TypeListFrom< Message<Types>... >;

		// this messages available to Send()
		using SupportedMessages_t	= MessageListFrom< 
											ModuleMsg::AttachModule,
											ModuleMsg::DetachModule,
											//ModuleMsg::ReplaceModule,
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
											//GX_PROFILE_MSG( ProfilingMsg::OnSendMsg, )
											ModuleMsg::Link,
											ModuleMsg::Compose,
											ModuleMsg::Update,
											ModuleMsg::Delete,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached
										>;

		struct ModuleConfig
		{
			UntypedID_t		id;
			uint			maxParents;
		};
		

	private:
		class AttachModuleToManagerAsyncTask;
		struct _SubscribeReceiver_Func;


	// variables
	private:
		MessageHandler			_msgHandler;
		ModulePtr				_manager;
		ParentModules_t			_parents;
		AttachedModules_t		_attachments;
		EState					_state;
		const ThreadID			_ownThread;		// TODO: use Atomic<> ?
		const UntypedID_t		_compId;
		const uint				_maxParents;
		const TypeIdList&		_supportedMessages;
		const TypeIdList&		_supportedEvents;


	// methods
	public:
		template <typename T>
		bool Send (const Message<T> &msg) noexcept;

		template <typename ...Types>
		bool Subscribe (Types&& ...args);

		template <typename MsgList>
		bool ReceiveEvents (const ModulePtr &other);

		GX_PROFILE_MSG(
			template <typename ...Types>
			bool _SubscribeDbg (Types&& ...args);
		)

		//template <typename ...Types>
		//bool Unsubscribe (Types&& ...args);
		
		template <typename T>
		void UnsubscribeAll (const T &unit);

		template <typename CreateInfo>
		bool AddModule (UntypedID_t id, const CreateInfo &createInfo);

		template <typename CreateInfo>
		bool AddModule (StringCRef name, UntypedID_t id, const CreateInfo &createInfo);

		ModulePtr GetModuleByID (UntypedID_t id);	// TODO: remove
		ModulePtr GetModuleByName (StringCRef name);
		
		template <typename Typelist>				CHECKRES(ModulePtr) GetModuleByMsg ();
		template <typename Typelist>				CHECKRES(ModulePtr) GetModuleByEvent ();
		template <typename Msg, typename Events>	CHECKRES(ModulePtr) GetModuleByMsgEvent ();
		
		template <typename Typelist>				CHECKRES(ModulePtr) GetParentByMsg ();
		template <typename Typelist>				CHECKRES(ModulePtr) GetParentByEvent ();
		template <typename Msg, typename Events>	CHECKRES(ModulePtr) GetParentByMsgEvent ();

		TypeIdList const&	GetSupportedMessages ()		const	{ return _supportedMessages; }
		TypeIdList const&	GetSupportedEvents ()		const	{ return _supportedEvents; }
		UntypedID_t			GetModuleID ()				const	{ return _compId; }
		EState				GetState ()					const	{ return _state; }
		ThreadID			GetThreadID ()				const	{ return _ownThread; }

		
	// hidden methods
	protected:
		Module (const GlobalSystemsRef gs,
				const ModuleConfig &config,
				const TypeIdList *msgTypes,
				const TypeIdList *eventTypes);

		~Module ();

		bool _Attach (const ModuleName_t &name, const ModulePtr &unit, bool mustBeUniqueID);
		bool _Detach (const ModulePtr &unit);

		// helpers
		void _DetachAllAttachments ();
		void _DetachSelfFromParent (const ModulePtr &parent);
		void _DetachSelfFromAllParents ();

		bool _OnAttachedToParent (const ModulePtr &parent);
		bool _OnDetachedFromParent (const ModulePtr &parent);

		bool _AttachSelfToManager (const ModulePtr &mngr, UntypedID_t id = 0, bool wait = true);
		void _DetachSelfFromManager ();

		void _SetManager (const ModulePtr &mngr);
		bool _SetState (EState newState);

		bool _ValidateMsgSubscriptions ();
		bool _ValidateAllSubscriptions ();

		bool _DefCompose (bool immutable);

		void _ClearMessageHandlers ();
		
		template <typename MsgList>		bool _CopySubscriptions (const ModulePtr &other);

		template <typename T>			bool _SendMsg (const Message<T> &msg);
		template <typename T>			bool _SendEvent (const Message<T> &msg);
		template <typename T>			bool _SendUncheckedEvent (const Message<T> &msg);
		template <typename T>			bool _SendForEachAttachments (const Message<T> &msg);
		
		template <typename ...Types>	bool _SubscribeOnMsg (Types&& ...args);
		template <typename ...Types>	bool _SubscribeOnEvent (Types&& ...args);
		
		bool _FindAttachment (ArrayCRef<TypeId> messages, ArrayCRef<TypeId> events, OUT ModulePtr &result) const;
		bool _FindParent (ArrayCRef<TypeId> messages, ArrayCRef<TypeId> events, OUT ModulePtr &result) const;
		
		bool _FindAllAttachments (ArrayCRef<TypeId> messages, ArrayCRef<TypeId> events, OUT Array<ModulePtr> &result) const;
		bool _FindAllParents (ArrayCRef<TypeId> messages, ArrayCRef<TypeId> events, OUT Array<ModulePtr> &result) const;

		static bool _IsMutableState (EState state);			// is module editable?
		static bool _IsComposedState (EState state);		// is module completed?
		static bool _IsErrorState (EState state);			// is module initialization failed?
		static bool _IsComposedOrLinkedState (EState state);

		ParentModules_t const&		_GetParents ()		const	{ return _parents; }
		ModulePtr const&			_GetManager ()		const	{ return _manager; }
		AttachedModules_t const&	_GetAttachments ()	const	{ return _attachments; }

	private:
		void _Release (RefCounter_t &) override final;


	// message handlers with implementation
	protected:
		bool _OnModuleAttached_Impl (const Message< ModuleMsg::OnModuleAttached > &);
		bool _OnModuleDetached_Impl (const Message< ModuleMsg::OnModuleDetached > &);
		bool _AttachModule_Impl (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule_Impl (const Message< ModuleMsg::DetachModule > &);
		bool _ReplaceModule_Impl (const Message< ModuleMsg::ReplaceModule > &);
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
		bool _ReplaceModule_Empty (const Message< ModuleMsg::ReplaceModule > &);
		bool _OnManagerChanged_Empty (const Message< ModuleMsg::OnManagerChanged > &);
		bool _FindModule_Empty (const Message< ModuleMsg::FindModule > &);
		bool _ModulesDeepSearch_Empty (const Message< ModuleMsg::ModulesDeepSearch > &);
		bool _Update_Empty (const Message< ModuleMsg::Update > &);
		bool _Link_Empty (const Message< ModuleMsg::Link > &);
		bool _Compose_Empty (const Message< ModuleMsg::Compose > &);
		bool _Delete_Empty (const Message< ModuleMsg::Delete > &);
	};
	
	
}	// Base
}	// Engine


#include "Module.inl.h"
