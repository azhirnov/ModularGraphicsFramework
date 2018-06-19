// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Public/CreateInfo.h"
#include "Engine/Base/Public/ModuleMessages.h"
#include "Engine/Base/Public/ProfilingMessages.h"
#include "Engine/Base/Common/BaseObject.h"
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
			LinkingFailed,
			ComposingFailed,
			IncompleteAttachment,	// current module can't find necessary modules.
		};

	protected:
		using UntypedID_t			= ModuleMsg::UntypedID_t;
		using ModuleName_t			= ModuleMsg::ModuleName_t;
		using AttachedModules_t		= Array<Pair< ModuleName_t, ModulePtr >>;
		using ParentModules_t		= Set< ModulePtr >;	//FixedSizeSet< ModulePtr, 16 >;
		using EHandlerPriority		= MessageHandler::EPriority;

		template <typename ...Types>
		using MessageListFrom		= CompileTime::TypeListFrom< Types... >;

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
											ModuleMsg::AfterLink,
											ModuleMsg::Compose,
											ModuleMsg::AfterCompose,
											ModuleMsg::Update,
											ModuleMsg::Delete,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached
										>;

		struct ModuleConfig
		{
		// variables
			UntypedID_t		id				= 0;
			uint			maxParents		= 1;
			bool			multiAttachment	= false;

		// methods
			ModuleConfig (UntypedID_t id, uint maxParents, bool multiAttachment = false) :
				id{id}, maxParents{maxParents}, multiAttachment{multiAttachment} {}
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
		const ModuleConfig		_moduleCfg;
		const TypeIdList&		_supportedMessages;
		const TypeIdList&		_supportedEvents;


	// methods
	public:
		template <typename MsgT>
		bool Send (const MsgT &msg) noexcept;
		
		template <typename MsgT>
		bool SendAsync (const MsgT &msg) noexcept;

		//template <typename MsgT>
		//auto Request (MsgT &msg) noexcept;
		
		template <typename MsgT>
		auto Request (MsgT &&msg) noexcept;

		template <typename Class, typename Func>
		bool Subscribe (const Class &obj, Func func, EHandlerPriority prior = EHandlerPriority::Auto);

		template <typename MsgList>
		bool ReceiveEvents (const ModulePtr &other);

		GX_PROFILE_MSG(
			template <typename Class, typename Func>
			bool _SubscribeDbg (const Class &obj, Func func);
		)
			
		template <typename Class, typename Func>
		void Unsubscribe (const Class &obj, Func func);
		
		template <typename Class>
		void UnsubscribeAll (const Class &unit);

		template <typename CreateInfo>
		bool AddModule (UntypedID_t id, const CreateInfo &createInfo);

		template <typename CreateInfo>
		bool AddModule (StringCRef name, UntypedID_t id, const CreateInfo &createInfo);

		ND_ ModulePtr  GetModuleByID (UntypedID_t id);	// TODO: remove
		ND_ ModulePtr  GetModuleByName (StringCRef name);
		
		template <typename Typelist>				ND_ ModulePtr  GetModuleByMsg ();
		template <typename Typelist>				ND_ ModulePtr  GetModuleByEvent ();
		template <typename Msg, typename Events>	ND_ ModulePtr  GetModuleByMsgEvent ();
		
		template <typename Typelist>				ND_ ModulePtr  GetParentByMsg ();
		template <typename Typelist>				ND_ ModulePtr  GetParentByEvent ();
		template <typename Msg, typename Events>	ND_ ModulePtr  GetParentByMsgEvent ();

		ND_ TypeIdList const&	GetSupportedMessages ()		const	{ return _supportedMessages; }
		ND_ TypeIdList const&	GetSupportedEvents ()		const	{ return _supportedEvents; }
		ND_ UntypedID_t			GetModuleID ()				const	{ return _moduleCfg.id; }
		ND_ EState				GetState ()					const	{ return _state; }
		ND_ ThreadID			GetThreadID ()				const	{ return _ownThread; }

		
	// hidden methods
	protected:
		Module (const GlobalSystemsRef gs,
				const ModuleConfig &config,
				const TypeIdList *msgTypes,
				const TypeIdList *eventTypes);

		~Module ();

		bool _Attach (const ModuleName_t &name, const ModulePtr &unit, bool mustBeUniqueID = false);
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
		
		template <typename MsgList>		bool _CopySubscriptions (const ModulePtr &other, bool removeUnsupported = false);
		template <typename MsgList>		bool _CopySubscriptions (const ModulePtr &other, bool removeUnsupported, EHandlerPriority priority);

		template <typename MsgT>		bool _SendMsg (const MsgT &msg);
		template <typename MsgT>		bool _SendEvent (const MsgT &msg);
		template <typename MsgT>		bool _SendUncheckedEvent (const MsgT &msg);
		template <typename MsgT>		bool _SendForEachAttachments (const MsgT &msg);
		
		template <typename Class, typename Func>	bool _SubscribeOnMsg (const Class &obj, Func func, EHandlerPriority prior = EHandlerPriority::Auto);
		template <typename Class, typename Func>	bool _SubscribeOnEvent (const Class &obj, Func func, EHandlerPriority prior = EHandlerPriority::Auto);
		
		bool _FindAttachment (ArrayCRef<TypeId> messages, ArrayCRef<TypeId> events, OUT ModulePtr &result) const;
		bool _FindParent (ArrayCRef<TypeId> messages, ArrayCRef<TypeId> events, OUT ModulePtr &result) const;
		
		bool _FindAllAttachments (ArrayCRef<TypeId> messages, ArrayCRef<TypeId> events, OUT Array<ModulePtr> &result) const;
		bool _FindAllParents (ArrayCRef<TypeId> messages, ArrayCRef<TypeId> events, OUT Array<ModulePtr> &result) const;

		ND_ static bool _IsMutableState (EState state);				// is module editable?
		ND_ static bool _IsComposedState (EState state);			// is module completed?
		ND_ static bool _IsErrorState (EState state);				// is module initialization failed?
		ND_ static bool _IsComposedOrLinkedState (EState state);
		ND_ static bool _IsInitialState (EState state);				// is module in initial or failed state?

		ND_ ParentModules_t const&		_GetParents ()		const	{ return _parents; }
		ND_ ModulePtr const&			_GetManager ()		const	{ return _manager; }
		ND_ AttachedModules_t const&	_GetAttachments ()	const	{ return _attachments; }


	private:
		void _Release (RefCounter_t &) override final;
		
		bool _DetachSingle (const ModulePtr &unit);
		bool _DetachMulti (const ModulePtr &unit);


	// message handlers with implementation
	protected:
		bool _OnModuleAttached_Impl (const ModuleMsg::OnModuleAttached &);
		bool _OnModuleDetached_Impl (const ModuleMsg::OnModuleDetached &);
		bool _AttachModule_Impl (const ModuleMsg::AttachModule &);
		bool _DetachModule_Impl (const ModuleMsg::DetachModule &);
		bool _ReplaceModule_Impl (const ModuleMsg::ReplaceModule &);
		bool _FindModule_Impl (const ModuleMsg::FindModule &);
		bool _ModulesDeepSearch_Impl (const ModuleMsg::ModulesDeepSearch &);
		bool _Update_Impl (const ModuleMsg::Update &);
		bool _Link_Impl (const ModuleMsg::Link &);
		bool _Compose_Impl (const ModuleMsg::Compose &);
		bool _Delete_Impl (const ModuleMsg::Delete &);


	// message handlers without implementation
	protected:
		bool _AttachModule_Empty (const ModuleMsg::AttachModule &);
		bool _DetachModule_Empty (const ModuleMsg::DetachModule &);
		bool _ReplaceModule_Empty (const ModuleMsg::ReplaceModule &);
		bool _OnManagerChanged_Empty (const ModuleMsg::OnManagerChanged &);
		bool _FindModule_Empty (const ModuleMsg::FindModule &);
		bool _ModulesDeepSearch_Empty (const ModuleMsg::ModulesDeepSearch &);
		bool _Update_Empty (const ModuleMsg::Update &);
		bool _Link_Empty (const ModuleMsg::Link &);
		bool _Compose_Empty (const ModuleMsg::Compose &);
		bool _Delete_Empty (const ModuleMsg::Delete &);
	};
	
	
}	// Base
}	// Engine


#include "Module.inl.h"
