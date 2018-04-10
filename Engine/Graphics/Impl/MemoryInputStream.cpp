// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Graphics/Public/MemStream.h"
#include "Engine/Graphics/Impl/GraphicsBaseModule.h"
#include "Engine/Graphics/Impl/GraphicsObjectsConstructor.h"

namespace Engine
{
namespace Graphics
{
	
	
	//
	// Memory Input Stream Module
	//

	class MemoryInputStream final : public GraphicsBaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GraphicsBaseModule::SupportedMessages_t::Append< MessageListFrom< 
											GraphicsMsg::StreamReadBuffer
										> >;

		using SupportedEvents_t		= GraphicsBaseModule::SupportedEvents_t;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		File::RFilePtr	_file;
		BinaryArray		_cache;


	// methods
	public:
		MemoryInputStream (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuMemoryInStream &ci);
		~MemoryInputStream ();


	// message handlers
	private:

	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	MemoryInputStream::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	MemoryInputStream::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	MemoryInputStream::MemoryInputStream (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuMemoryInStream &ci) :
		GraphicsBaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes )
	{
		_SubscribeOnMsg( this, &MemoryInputStream::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &MemoryInputStream::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &MemoryInputStream::_AttachModule_Empty );
		_SubscribeOnMsg( this, &MemoryInputStream::_DetachModule_Empty );
		_SubscribeOnMsg( this, &MemoryInputStream::_FindModule_Empty );
		_SubscribeOnMsg( this, &MemoryInputStream::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &MemoryInputStream::_Delete_Impl );

		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	MemoryInputStream::~MemoryInputStream ()
	{
	}
	
//-----------------------------------------------------------------------------
	
	
/*
=================================================
	CreateMemoryInputStream
=================================================
*/
	ModulePtr GraphicsObjectsConstructor::CreateMemoryInputStream (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuMemoryInStream &ci)
	{
		return New< MemoryInputStream >( id, gs, ci );
	}

}	// Graphics
}	// Engine
