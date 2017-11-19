// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
	ModulesFactory::ModulesFactory (GlobalSystemsRef gs) :
		BaseObject( gs )
	{
		SetDebugName( "ModulesFactory" );

		GlobalSystems()->GetSetter< ModulesFactory >().Set( this );

		_constructors.Reserve( 128 );
		_groups.Reserve( EModuleGroup::_Count );
	}
	
/*
=================================================
	destructor
=================================================
*/
	ModulesFactory::~ModulesFactory ()
	{
		ASSERT( _constructors.Empty() );	// some constructors is not unregistered

		Clear();

		GlobalSystems()->GetSetter< ModulesFactory >().Set( null );
	}
	
/*
=================================================
	Clear
=================================================
*/
	void ModulesFactory::Clear ()
	{
		_constructors.Clear();
		_groups.Clear();
	}

/*
=================================================
	Create
=================================================
*/
	bool ModulesFactory::Create (UntypedID_t id, GlobalSystemsRef gs, VariantCRef msg, OUT ModulePtr &result) noexcept
	{
		SCOPELOCK( _lock );

		ModConstructors_t::const_iterator	iter;

		if ( _constructors.Find( ConstructorID( id, msg.GetValueTypeId() ), OUT iter ) and
			 iter->second->IsValid( msg.GetValueTypeId() ) )
		{
			result = iter->second->Call( gs, msg );

			return result.IsNotNull();
		}

		RETURN_ERR( "module '" << ToString( GModID::type( id ) ) << "' with '" <<
					ToString( msg.GetValueTypeId() ) << "' create info is not registered" );
	}
	
/*
=================================================
	_Register
=================================================
*/
	bool ModulesFactory::_Register (UntypedID_t id, TypeId ctorMsgType, TypeId moduleIdType, Constructor_t &&ctor)
	{
		SCOPELOCK( _lock );

		const EModuleGroup::type	group	= EModuleGroup::type( id & EModuleGroup::_Mask );

		CHECK_ERR( group < EModuleGroup::_Count );
		CHECK_ERR( ctorMsgType != TypeId() );


		// check if group already registered with different type
		ModGroups_t::const_iterator	iter;

		if ( _groups.Find( group, OUT iter ) )
		{
			CHECK_ERR( iter->second == moduleIdType );
		}
		else
			_groups.Add( group, moduleIdType );


		// add constructors
		CHECK_ERR( not _constructors.IsExist( ConstructorID( id, ctorMsgType ) ) );

		_constructors.Add( RVREF(ConstructorID( id, ctorMsgType )), RVREF(ctor) );
		return true;
	}
	
/*
=================================================
	_Unregister
=================================================
*/
	bool ModulesFactory::_Unregister (UntypedID_t id, TypeId ctorMsgType)
	{
		SCOPELOCK( _lock );

		ModConstructors_t::iterator	iter;

		if ( _constructors.Find( ConstructorID( id, ctorMsgType ), OUT iter ) )
		{
			_constructors.EraseByIter( iter );
			return true;
		}

		return false;
	}
	
/*
=================================================
	_UnregisterAll
=================================================
*/
	bool ModulesFactory::_UnregisterAll (UntypedID_t id)
	{
		SCOPELOCK( _lock );

		usize	idx = 0;

		if ( _constructors.FindIndex( ConstructorID( id, TypeId() ), OUT idx ) )
		{
			for (usize i = idx; i < _constructors.Count() and _constructors[i].first.moduleID == id;)
			{
				_constructors.EraseByIndex( i );
			}
			return true;
		}

		return false;
	}

/*
=================================================
	_IsRegistered
=================================================
*/
	bool ModulesFactory::_IsRegistered (UntypedID_t id, TypeId ctorMsgType) const
	{
		SCOPELOCK( _lock );

		return _constructors.IsExist( ConstructorID( id, ctorMsgType ) );
	}
	
/*
=================================================
	_Search
=================================================
*/
	bool ModulesFactory::_Search (TypeId ctorMsgType, StringCRef startsWith, OUT Array<UntypedID_t> &result) const
	{
		SCOPELOCK( _lock );

		result.Clear();

		FOR( i, _constructors )
		{
			if ( _constructors[i].first.createInfoType == ctorMsgType )
			{
				if ( startsWith.Empty() or
					 ToString(GModID::type( _constructors[i].first.moduleID )).StartsWithIC( startsWith ) )
				{
					result.PushBack( _constructors[i].first.moduleID );
				}
			}
		}
		return true;
	}


}	// Base
}	// Engine
