// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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

		//
		// Contructor interface
		//
		struct IContructor
		{
			IContructor () {}
			virtual ~IContructor () {}

			virtual bool	  IsValid (TypeId id) const = 0;
			virtual ModulePtr Call (GlobalSystemsRef gs, VariantCRef) const = 0;
		};


		//
		// Contructor implementation
		//
		template <typename CtorMsg>
		struct ContructorImpl final : IContructor
		{
		// types
			using CtorMsg_t	= ModulePtr (*) (GlobalSystemsRef gs, const CtorMsg &);

		// variables
			CtorMsg_t	_ctorMsg;

		// methods
			explicit ContructorImpl (CtorMsg_t ctor) : _ctorMsg(ctor)
			{}
			
			bool IsValid (TypeId id) const override
			{
				return _ctorMsg and id == TypeIdOf<CtorMsg>();
			}

			ModulePtr Call (GlobalSystemsRef gs, VariantCRef msg) const override
			{
				if ( _ctorMsg and msg.IsType<CtorMsg>() )
					return _ctorMsg( gs, msg.Get<CtorMsg>() );

				return null;
			}
		};

		using Constructor_t			= VariantInterface< IContructor, sizeof(ContructorImpl<int>) >;
		using UntypedID_t			= ModuleMsg::UntypedID_t;


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
			bool operator <  (const ConstructorID &right) const;
		};


		using ModGroups_t			= Map< EModuleGroup::type, TypeId >;
		using ModConstructors_t		= Map< ConstructorID, Constructor_t >;


	// variables
	private:
		ModConstructors_t		_constructors;
		ModGroups_t				_groups;
		mutable OS::Mutex		_lock;


	// methods
	public:
		explicit
		ModulesFactory (const GlobalSystemsRef gs);
		~ModulesFactory ();


		bool Create (UntypedID_t id, GlobalSystemsRef gs, VariantCRef msg, OUT ModulePtr &result);
		
		template <typename CtorMsg>
		bool Create (UntypedID_t id, GlobalSystemsRef gs, const CtorMsg &msg, OUT ModulePtr &result);


		template <typename ModIdType, typename CtorMsg>
		bool Register (ModIdType id, ModulePtr (*ctor) (GlobalSystemsRef gs, const CtorMsg &));
		
		template <typename Class, typename CtorMsg>
		bool Unregister ();
		
		template <typename Class>
		bool UnregisterAll ();
		
		template <typename Class, typename CtorMsg>
		bool IsRegistered () const;


	private:
		bool _Register (UntypedID_t id, TypeId ctorMsgType, TypeId moduleIdType, Constructor_t &&ctor);
		bool _Unregister (UntypedID_t id, TypeId ctorMsgType);
		bool _UnregisterAll (UntypedID_t id);
		bool _IsRegistered (UntypedID_t id, TypeId ctorMsgType) const;
	};
	
	

/*
=================================================
	Create
=================================================
*/
	template <typename CtorMsg>
	inline bool ModulesFactory::Create (UntypedID_t id, GlobalSystemsRef gs, const CtorMsg &msg, OUT ModulePtr &result)
	{
		return Create( id, gs, VariantCRef::FromConst(msg), OUT result );
	}

/*
=================================================
	Register
=================================================
*/
	template <typename ModIdType, typename CtorMsg>
	inline bool ModulesFactory::Register (ModIdType id, ModulePtr (*ctor) (GlobalSystemsRef gs, const CtorMsg &))
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
	template <typename Class, typename CtorMsg>
	inline bool ModulesFactory::Unregister ()
	{
		return _Unregister( Class::GetStaticID(), TypeIdOf<CtorMsg>() );
	}
	
/*
=================================================
	UnregisterAll
=================================================
*/
	template <typename Class>
	inline bool ModulesFactory::UnregisterAll ()
	{
		return _UnregisterAll( Class::GetStaticID() );
	}

/*
=================================================
	IsRegistered
=================================================
*/
	template <typename Class, typename CtorMsg>
	inline bool ModulesFactory::IsRegistered () const
	{
		return _IsRegistered( Class::GetStaticID(), TypeIdOf<CtorMsg>() );
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
	ConstructorID::operator <
=================================================
*/
	inline bool ModulesFactory::ConstructorID::operator <  (const ConstructorID &right) const
	{
		return	moduleID != right.moduleID ?
					moduleID < right.moduleID :
					createInfoType != TypeId() and right.createInfoType != TypeId() ?
						createInfoType < right.createInfoType :
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
		ModulePtr	unit;
		CHECK_ERR( GlobalSystems()->Get< ModulesFactory >()->Create( id, GlobalSystems(), createInfo, OUT unit ) );

		_SendMsg( Message< ModuleMsg::AttachModule >{ this, unit } );
		return true;
	}

	
}	// Base
}	// Engine
