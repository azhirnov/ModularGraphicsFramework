// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/Base/Modules/ModulesFactory.h"

namespace Engine
{
namespace Base
{
	
/*
=================================================
	constructor
=================================================
*/
	ModulesFactory::ModulesFactory (const SubSystemsRef gs) :
		BaseObject( gs )
	{
		SetDebugName( "ModulesFactory" );

		GlobalSystems()->GetSetter< ModulesFactory >().Set( this );

		_constructors.Reserve( 128 );
		_families.Reserve( EModuleGroup::_Count );
	}
	
/*
=================================================
	destructor
=================================================
*/
	ModulesFactory::~ModulesFactory ()
	{
		GlobalSystems()->GetSetter< ModulesFactory >().Set( null );
	}
	
/*
=================================================
	Create
=================================================
*/
	bool ModulesFactory::Create (UntypedID_t id, SubSystemsRef gs, VariantCRef msg, OUT ModulePtr &result)
	{
		SCOPELOCK( _lock );

		CompConstructors_t::const_iterator	iter;

		if ( _constructors.Find( id, OUT iter ) and
			 iter->second->IsValid( msg.GetValueTypeId() ) )
		{
			result = iter->second->Call( gs, msg );

			return result.IsNotNull();
		}

		RETURN_ERR( "module '" << ToString( GModID::type( id ) ) << "' is not registered" );
	}
	
/*
=================================================
	_Register
=================================================
*/
	bool ModulesFactory::_Register (UntypedID_t id, TypeId typeId, Constructor_t &&ctor)
	{
		SCOPELOCK( _lock );

		const EModuleGroup::type	family	= EModuleGroup::type( id & EModuleGroup::_Mask );
		const ModulePtr			owner	= ctor->_system;

		CHECK_ERR( family < EModuleGroup::_Count );


		// check if family already registered with different type
		CompFamilies_t::const_iterator	iter;

		if ( _families.Find( family, OUT iter ) )
		{
			CHECK_ERR( iter->second == typeId );
		}
		else
			_families.Add( family, typeId );


		// add constructors
		CHECK_ERR( not _constructors.IsExist( id ) );

		_constructors.Add( RVREF(id), RVREF(ctor) );


		// send message
		if ( owner )
		{
			owner->Send( Message< ModuleMsg::OnRegistered >{ this, this } );
		}
		return true;
	}
	
/*
=================================================
	Unregister
=================================================
*/
	bool ModulesFactory::Unregister (UntypedID_t id)
	{
		SCOPELOCK( _lock );

		CompConstructors_t::iterator	iter;

		if ( _constructors.Find( id, OUT iter ) )
		{
			if ( iter->second.IsCreated() and iter->second->_system )
			{
				iter->second->_system->Send( Message< ModuleMsg::OnUnregistered >{ this, this } );
			}

			_constructors.EraseFromIter( iter );
			return true;
		}

		return false;
	}
	
/*
=================================================
	IsRegistered
=================================================
*/
	bool ModulesFactory::IsRegistered (UntypedID_t id)
	{
		SCOPELOCK( _lock );

		return _constructors.IsExist( id );
	}


}	// Base
}	// Engine
