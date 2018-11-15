// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Graphics/Public/Font.h"
#include "Engine/Graphics/Impl/GraphicsObjectsConstructor.h"
#include "Engine/Graphics/Impl/GraphicsBaseModule.h"

namespace Engine
{
namespace Graphics
{

	//
	// Font Manager
	//

	class FontManager final : public GraphicsBaseModule
	{
	// types
	private:
		using SupportedEvents_t		= Module::SupportedEvents_t;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:


	// methods
	public:
		FontManager (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::FontManager &ci);
		~FontManager ();


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


	
	const TypeIdList	FontManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	FontManager::FontManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::FontManager &ci) :
		GraphicsBaseModule( gs, ModuleConfig{ id, UMax }, &_eventTypes )
	{
		SetDebugName( "FontManager" );

		_SubscribeOnMsg( this, &FontManager::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &FontManager::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &FontManager::_AttachModule_Impl );
		_SubscribeOnMsg( this, &FontManager::_DetachModule_Impl );
		_SubscribeOnMsg( this, &FontManager::_FindModule_Impl );
		_SubscribeOnMsg( this, &FontManager::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &FontManager::_Link );
		_SubscribeOnMsg( this, &FontManager::_Compose );
		_SubscribeOnMsg( this, &FontManager::_Delete );
		_SubscribeOnMsg( this, &FontManager::_OnManagerChanged_Empty );

		_AttachSelfToManager( _GetGpuThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	FontManager::~FontManager ()
	{
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool FontManager::_Delete (const ModuleMsg::Delete &msg)
	{
		return _Delete_Impl( msg );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool FontManager::_Link (const ModuleMsg::Link &)
	{
		return true;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool FontManager::_Compose (const ModuleMsg::Compose &)
	{
		return true;
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateFontManager
=================================================
*/
	ModulePtr GraphicsObjectsConstructor::CreateFontManager (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::FontManager &ci)
	{
		return New< FontManager >( id, gs, ci );
	}

}	// Graphics
}	// Engine
