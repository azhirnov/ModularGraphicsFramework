// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

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
			ModulePtr	_system;

			explicit IContructor (const ModulePtr &unit) : _system(unit) {}
			virtual ~IContructor () {}

			virtual bool	  IsValid (TypeId id) const = 0;
			virtual ModulePtr Call (SubSystemsRef gs, VariantCRef) const = 0;
		};

		//
		// Contructor implementation
		//
		template <typename CtorMsg>
		struct ContructorImpl final : IContructor
		{
		// types
			using CtorMsg_t	= ModulePtr (*) (SubSystemsRef gs, const CtorMsg &);

		// variables
			CtorMsg_t	_ctorMsg;

		// methods
			ContructorImpl (const ModulePtr &unit, CtorMsg_t ctorMsg) :
				IContructor(unit), _ctorMsg(ctorMsg)
			{}
			
			bool IsValid (TypeId id) const override
			{
				return _ctorMsg and id == TypeIdOf<CtorMsg>();
			}

			ModulePtr Call (SubSystemsRef gs, VariantCRef msg) const override
			{
				if ( _ctorMsg and msg.IsType<CtorMsg>() )
					return _ctorMsg( gs, msg.Get<CtorMsg>() );

				return null;
			}
		};


		using Constructor_t			= VariantInterface< IContructor, sizeof(ContructorImpl<int>) >;
		using UntypedID_t			= ModuleMsg::UntypedID_t;
		using CompFamilies_t		= Map< EModuleGroup::type, TypeId >;
		using CompConstructors_t	= Map< UntypedID_t, Constructor_t >;


	// variables
	private:
		CompConstructors_t		_constructors;
		CompFamilies_t			_families;
		OS::Mutex				_lock;


	// methods
	public:
		explicit
		ModulesFactory (const SubSystemsRef gs);
		~ModulesFactory ();


		bool Create (UntypedID_t id, SubSystemsRef gs, VariantCRef msg, OUT ModulePtr &result);
		
		template <typename CtorMsg>
		bool Create (UntypedID_t id, SubSystemsRef gs, const CtorMsg &msg, OUT ModulePtr &result);


		template <typename ModIdType, typename CtorMsg>
		bool Register (ModIdType id,
					   const ModulePtr &unit,
					   ModulePtr (*ctorMsg) (SubSystemsRef gs, const CtorMsg &));
		
		template <typename Class>
		bool Unregister ();
		bool Unregister (UntypedID_t id);
		
		bool IsRegistered (UntypedID_t id);


	private:
		bool _Register (UntypedID_t id, TypeId typeId, Constructor_t &&ctor);
	};
	
	

/*
=================================================
	Create
=================================================
*/
	template <typename CtorMsg>
	inline bool ModulesFactory::Create (UntypedID_t id, SubSystemsRef gs, const CtorMsg &msg, OUT ModulePtr &result)
	{
		return Create( id, gs, VariantCRef::FromConst(msg), OUT result );
	}

/*
=================================================
	Register
=================================================
*/
	template <typename ModIdType, typename CtorMsg>
	inline bool ModulesFactory::Register (ModIdType id,
										  const ModulePtr &unit,
										  ModulePtr (*ctorMsg) (SubSystemsRef gs, const CtorMsg &))
	{
		STATIC_ASSERT(( not CompileTime::IsSameTypes< ModIdType, UntypedID_t > ));
		CHECK_ERR( ctorMsg != null );

		return _Register( id,
						  TypeIdOf<ModIdType>(),
						  Constructor_t( ContructorImpl<CtorMsg>( unit, ctorMsg ) ) );
	}

/*
=================================================
	Unregister
=================================================
*/
	template <typename Class>
	inline bool ModulesFactory::Unregister ()
	{
		return Unregister( Class::GetStaticID() );
	}
	
/*
=================================================
	AddModule
=================================================
*/
	template <typename CreateInfo>
	inline bool Module::AddModule (UntypedID_t id, const CreateInfo &createInfo)
	{
		ModulePtr	unit;
		CHECK_ERR( GlobalSystems()->Get< ModulesFactory >()->Create( id, GlobalSystems(), createInfo, OUT unit ) );

		Send( Message< ModuleMsg::AttachModule >{ this, unit } );
		return true;
	}

	
}	// Base
}	// Engine
