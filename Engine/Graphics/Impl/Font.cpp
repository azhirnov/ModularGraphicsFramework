// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Graphics/Public/Font.h"
#include "Engine/Graphics/Impl/GraphicsObjectsConstructor.h"

namespace Engine
{
namespace Graphics
{

	//
	// Font
	//

	class Font final : public Module
	{
	// types
	private:
		//using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
		//								> >;

		using SupportedEvents_t		= Module::SupportedEvents_t;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:


	// methods
	public:
		Font (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::Font &ci);
		~Font ();


	// message handlers
	private:
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		//bool _AttachModule (const ModuleMsg::AttachModule &);
		//bool _DetachModule (const ModuleMsg::DetachModule &);

	private:
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	Font::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Font::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Font::Font (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::Font &ci) :
		Module( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "Font" );

		_SubscribeOnMsg( this, &Font::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Font::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Font::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Font::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Font::_FindModule_Impl );
		_SubscribeOnMsg( this, &Font::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Font::_Link );
		_SubscribeOnMsg( this, &Font::_Compose );
		_SubscribeOnMsg( this, &Font::_Delete );
		_SubscribeOnMsg( this, &Font::_OnManagerChanged_Empty );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( null, FontManagerModuleID, true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Font::~Font ()
	{
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Font::_Delete (const ModuleMsg::Delete &msg)
	{
		return _Delete_Impl( msg );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool Font::_Link (const ModuleMsg::Link &msg)
	{
		return true;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Font::_Compose (const ModuleMsg::Compose &msg)
	{
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateFont
=================================================
*/
	ModulePtr GraphicsObjectsConstructor::CreateFont (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::Font &ci)
	{
		return New< Font >( id, gs, ci );
	}

}	// Graphics
}	// Engine
