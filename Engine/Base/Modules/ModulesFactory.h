// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Modules/Module.h"

namespace Engine
{
namespace Base
{

	//
	// Modules Factory
	//

	class ModulesFactory final : public BaseObject
	{
	// types
	private:
		using UntypedID_t	= ModuleMsg::UntypedID_t;


		//
		// Contructor interface
		//
		struct IContructor
		{
			IContructor () {}
			virtual ~IContructor () {}

			virtual bool	  IsValid (TypeId id) const = 0;
			virtual ModulePtr Call (UntypedID_t, GlobalSystemsRef, VariantCRef) const = 0;
		};


		//
		// Contructor implementation
		//
		template <typename CtorMsg>
		struct ContructorImpl final : IContructor
		{
		// types
			using CtorMsg_t	= ModulePtr (*) (UntypedID_t, GlobalSystemsRef, const CtorMsg &);

		// variables
			CtorMsg_t	_ctorMsg;

		// methods
			explicit ContructorImpl (CtorMsg_t ctor) : _ctorMsg(ctor)
			{}
			
			bool IsValid (TypeId id) const override
			{
				return _ctorMsg and id == TypeIdOf<CtorMsg>();
			}

			ModulePtr Call (UntypedID_t id, GlobalSystemsRef gs, VariantCRef msg) const override
			{
				if ( _ctorMsg and msg.IsType<CtorMsg>() )
					return _ctorMsg( id, gs, msg.Get<CtorMsg>() );

				return null;
			}
		};

		using Constructor_t	= VariantInterface< IContructor, sizeof(ContructorImpl<int>) >;


		//
		// Constructor ID
		//
		struct ConstructorID
		{
		// variables
			UntypedID_t		moduleID;
			TypeId			createInfoType;

		// methods
			ConstructorID () {}
			ConstructorID (UntypedID_t moduleID, TypeId createInfoType) : moduleID(moduleID), createInfoType(createInfoType) {}

			ConstructorID (ConstructorID &&) = default;
			ConstructorID (const ConstructorID &) = default;

			ConstructorID& operator = (ConstructorID &&) = default;
			ConstructorID& operator = (const ConstructorID &) = default;

			bool operator == (const ConstructorID &right) const;
			bool operator >  (const ConstructorID &right) const;
		};


		using ModGroups_t			= Map< EModuleGroup::type, TypeId >;
		using ModConstructors_t		= Map< ConstructorID, Constructor_t >;


	// variables
	private:
		ModConstructors_t		_constructors;
		ModGroups_t				_groups;
		mutable OS::Mutex		_lock;		// TODO: read-write sync


	// methods
	public:
		explicit ModulesFactory (GlobalSystemsRef gs);
		~ModulesFactory ();

		bool Create (UntypedID_t id, GlobalSystemsRef gs, VariantCRef msg, OUT ModulePtr &result) noexcept;
		
		template <typename CtorMsg>
		bool Create (UntypedID_t id, GlobalSystemsRef gs, const CtorMsg &msg, OUT ModulePtr &result) noexcept;

		template <typename CtorMsg>
		bool Search (StringCRef startsWith, OUT Array<UntypedID_t> &result) const;

		template <typename ModIdType, typename CtorMsg>
		bool Register (ModIdType id, ModulePtr (*ctor) (UntypedID_t, GlobalSystemsRef, const CtorMsg &));
		
		template <typename CtorMsg, typename ModIdType>
		bool Unregister (ModIdType id);
		
		template <typename ModIdType>
		bool UnregisterAll (ModIdType id);
		
		template <typename CtorMsg, typename ModIdType>
		bool IsRegistered (ModIdType id) const;

		void Clear ();

	private:
		bool _Register (UntypedID_t id, TypeId ctorMsgType, TypeId moduleIdType, Constructor_t &&ctor);
		bool _Unregister (UntypedID_t id, TypeId ctorMsgType);
		bool _UnregisterAll (UntypedID_t id);
		bool _IsRegistered (UntypedID_t id, TypeId ctorMsgType) const;
		bool _Search (TypeId ctorMsgType, StringCRef startsWith, OUT Array<UntypedID_t> &result) const;
	};
	
	

/*
=================================================
	Create
=================================================
*/
	template <typename CtorMsg>
	inline bool ModulesFactory::Create (UntypedID_t id, GlobalSystemsRef gs, const CtorMsg &msg, OUT ModulePtr &result) noexcept
	{
		return Create( id, gs, VariantCRef::FromConst(msg), OUT result );
	}

/*
=================================================
	Register
=================================================
*/
	template <typename ModIdType, typename CtorMsg>
	inline bool ModulesFactory::Register (ModIdType id, ModulePtr (*ctor) (UntypedID_t, GlobalSystemsRef, const CtorMsg &))
	{
		STATIC_ASSERT(( not CompileTime::IsSameTypes< ModIdType, UntypedID_t > ));
		CHECK_ERR( ctor != null );

		return _Register( id,
						  TypeIdOf<CtorMsg>(),
						  TypeIdOf<ModIdType>(),
						  Constructor_t( ContructorImpl<CtorMsg>( ctor ) ) );
	}

/*
=================================================
	Unregister
=================================================
*/
	template <typename CtorMsg, typename ModIdType>
	inline bool ModulesFactory::Unregister (ModIdType id)
	{
		return _Unregister( id, TypeIdOf<CtorMsg>() );
	}
	
/*
=================================================
	UnregisterAll
=================================================
*/
	template <typename ModIdType>
	inline bool ModulesFactory::UnregisterAll (ModIdType id)
	{
		return _UnregisterAll( id );
	}

/*
=================================================
	IsRegistered
=================================================
*/
	template <typename CtorMsg, typename ModIdType>
	inline bool ModulesFactory::IsRegistered (ModIdType id) const
	{
		return _IsRegistered( id, TypeIdOf<CtorMsg>() );
	}
	
/*
=================================================
	Search
=================================================
*/
	template <typename CtorMsg>
	inline bool ModulesFactory::Search (StringCRef startsWith, OUT Array<UntypedID_t> &result) const
	{
		return _Search( TypeIdOf<CtorMsg>(), startsWith, OUT result );
	}

/*
=================================================
	ConstructorID::operator ==
=================================================
*/
	inline bool ModulesFactory::ConstructorID::operator == (const ConstructorID &right) const
	{
		return	moduleID == right.moduleID and
				(createInfoType == TypeId() or right.createInfoType == TypeId() or createInfoType == right.createInfoType);
	}
	
/*
=================================================
	ConstructorID::operator >
=================================================
*/
	inline bool ModulesFactory::ConstructorID::operator >  (const ConstructorID &right) const 
	{
		return	moduleID != right.moduleID ?
					moduleID > right.moduleID :
					createInfoType != TypeId() and right.createInfoType != TypeId() ?
						createInfoType > right.createInfoType :
						false;			// types are equal
	}

/*
=================================================
	Module::AddModule
=================================================
*/
	template <typename CreateInfo>
	inline bool Module::AddModule (UntypedID_t id, const CreateInfo &createInfo)
	{
		return AddModule( StringCRef(), id, createInfo );
	}

	template <typename CreateInfo>
	inline bool Module::AddModule (StringCRef name, UntypedID_t id, const CreateInfo &createInfo)
	{
		ModulePtr	unit;
		CHECK_ERR( GlobalSystems()->modulesFactory->Create( id, GlobalSystems(), createInfo, OUT unit ) );

		CHECK_ERR( _SendMsg< ModuleMsg::AttachModule >({ name, unit }) );
		return true;
	}

	
}	// Base
}	// Engine
