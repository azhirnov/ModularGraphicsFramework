// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Graphics/Shared/GraphicsContext.h"


namespace Engine
{
namespace Graphics
{

	static ModulePtr CreateDefaultGraphicsContext (GlobalSystemsRef, const CreateInfo::GraphicsContext &);
	static ModulePtr CreateDefaultGraphicsThread (GlobalSystemsRef, const CreateInfo::GraphicsThread &);

/*
=================================================
	RegisterGraphics
=================================================
*/
	void RegisterGraphics ()
	{
		auto	ms	= GetMainSystemInstace();

		GraphicsContext::Register( ms->GlobalSystems() );
		
		auto	factory = ms->GlobalSystems()->Get< ModulesFactory >();

		factory->Register( 0, &CreateDefaultGraphicsContext );
		factory->Register( 0, &CreateDefaultGraphicsThread );
	}
	
/*
=================================================
	UnregisterGraphics
=================================================
*/
	void UnregisterGraphics ()
	{
		auto	ms	= GetMainSystemInstace();
		
		GraphicsContext::Unregister( ms->GlobalSystems() );
	}
	
/*
=================================================
	CreateDefaultGraphicsContext
=================================================
*/
	static ModulePtr CreateDefaultGraphicsContext (GlobalSystemsRef gs, const CreateInfo::GraphicsContext &ci)
	{
		auto	factory = gs->Get< ModulesFactory >();

		Array< ModuleMsg::UntypedID_t >	ids;
		factory->Search<decltype(ci)>( "", OUT ids );
		
		FOR( i, ids ) {
			if ( ids[i] != GModID::type(0) ) {
				ModulePtr	mod;
				CHECK_ERR( factory->Create( ids[i], gs, ci, OUT mod ) );
				return mod;
			}
		}
		return null;
	}
	
/*
=================================================
	CreateDefaultGraphicsThread
=================================================
*/
	static ModulePtr CreateDefaultGraphicsThread (GlobalSystemsRef gs, const CreateInfo::GraphicsThread &ci)
	{
		auto	factory = gs->Get< ModulesFactory >();

		Array< ModuleMsg::UntypedID_t >	ids;
		factory->Search<decltype(ci)>( "", OUT ids );
		
		FOR( i, ids ) {
			if ( ids[i] != GModID::type(0) ) {
				ModulePtr	mod;
				CHECK_ERR( factory->Create( ids[i], gs, ci, OUT mod ) );
				return mod;
			}
		}
		return null;
	}

}	// Graphics
}	// Engine
