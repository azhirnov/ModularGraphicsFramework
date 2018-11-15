// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/Query.h"
#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

namespace Engine
{
namespace PlatformGL
{
	
	//
	// OpenGL Query Pool
	//

	class GL4QueryPool final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetQueryPoolDescription,
											GpuMsg::GetGLQueryID,
											GpuMsg::GetGLPipelineStatisticQueries
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;

		using GLQueryIDs_t			= MixedSizeArray< gl::GLuint, 16 >;
		using QueryTargets_t		= MixedSizeArray< gl::GLenum, GLQueryIDs_t::MemoryContainer_t::FIXED_SIZE >;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		QueryPoolDescription	_descr;

		GLQueryIDs_t			_queryIds;
		QueryTargets_t			_targets;


	// methods
	public:
		GL4QueryPool (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuQueryPool &ci);
		~GL4QueryPool ();


	// message handlers
	private:
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);

		bool _GetGLQueryID (const GpuMsg::GetGLQueryID &);
		bool _GetGLPipelineStatisticQueries (const GpuMsg::GetGLPipelineStatisticQueries &);
		bool _GetQueryPoolDescription (const GpuMsg::GetQueryPoolDescription &);


	private:
		bool _IsCreated () const;
		bool _CreateQueryPool ();
		void _DestroyQueryPool ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	GL4QueryPool::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4QueryPool::GL4QueryPool (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuQueryPool &ci) :
		GL4BaseModule( gs, ModuleConfig{ id, UMax }, &_eventTypes ),
		_descr( ci.descr )
	{
		SetDebugName( "GL4QueryPool" );

		_SubscribeOnMsg( this, &GL4QueryPool::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4QueryPool::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4QueryPool::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GL4QueryPool::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GL4QueryPool::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4QueryPool::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4QueryPool::_Link_Impl );
		_SubscribeOnMsg( this, &GL4QueryPool::_Compose );
		_SubscribeOnMsg( this, &GL4QueryPool::_Delete );
		_SubscribeOnMsg( this, &GL4QueryPool::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4QueryPool::_GetGLQueryID );
		_SubscribeOnMsg( this, &GL4QueryPool::_GetGLPipelineStatisticQueries );
		_SubscribeOnMsg( this, &GL4QueryPool::_GetQueryPoolDescription );
		_SubscribeOnMsg( this, &GL4QueryPool::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4QueryPool::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4QueryPool::_GetGLPrivateClasses );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );

		ASSERT( _descr.count > 0 );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4QueryPool::~GL4QueryPool ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool GL4QueryPool::_Compose (const ModuleMsg::Compose &)
	{
		if ( _IsComposedState( GetState() ) or _IsCreated() )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateQueryPool() );
		
		return _DefCompose( false );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool GL4QueryPool::_Delete (const ModuleMsg::Delete &msg)
	{
		_DestroyQueryPool();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetGLQueryID
=================================================
*/
	bool GL4QueryPool::_GetGLQueryID (const GpuMsg::GetGLQueryID &msg)
	{
		CHECK_ERR( _descr.queryType == EQuery::Occlusion or _descr.queryType == EQuery::Timestamp );
		CHECK_ERR( msg.index < _queryIds.Count() );
		CHECK_ERR(	(_targets[ msg.index ] == 0 and msg.target != 0) or
					(_targets[ msg.index ] != 0 and msg.target == 0) or
					(_targets[ msg.index ] == msg.target) );

		if ( msg.target != 0 )
			_targets[ msg.index ] = msg.target;

		msg.result.Set({ _queryIds[ msg.index ], _targets[ msg.index ] });
		return true;
	}
	
/*
=================================================
	_GetGLPipelineStatisticQueries
=================================================
*/
	bool GL4QueryPool::_GetGLPipelineStatisticQueries (const GpuMsg::GetGLPipelineStatisticQueries &msg)
	{
		CHECK_ERR( _descr.queryType == EQuery::PipelineStatistic );
		CHECK_ERR( msg.index < _descr.count );

		GpuMsg::GetGLPipelineStatisticQueries::Queries_t	results;
		const usize											stat_count = _queryIds.Count() / _descr.count;

		for (usize i = 0; i < stat_count; ++i)
		{
			usize j = i + msg.index * stat_count;

			results[i].id		= _queryIds[j];
			results[i].target	= _targets[j];
		}

		msg.result.Set( results );
		return true;
	}

/*
=================================================
	_GetQueryPoolDescription
=================================================
*/
	bool GL4QueryPool::_GetQueryPoolDescription (const GpuMsg::GetQueryPoolDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool GL4QueryPool::_IsCreated () const
	{
		return not _queryIds.Empty();
	}
	
/*
=================================================
	_CreateQueryPool
=================================================
*/
	bool GL4QueryPool::_CreateQueryPool ()
	{
		using namespace gl;

		if ( _descr.queryType == EQuery::PipelineStatistic )
		{
			ASSERT( GetDevice()->IsExtensionSupported( "GL_ARB_pipeline_statistics_query" ) );

			FixedSizeArray< GLenum, EPipelineStatistic::_Count >	stat_targets;

			FOR( i, _descr.statisticFlags )
			{
				const auto	t = EPipelineStatistic::type(i);

				if ( _descr.statisticFlags[t] )
					stat_targets.PushBack( GL4Enum( t ) );
			}
			CHECK_ERR( not stat_targets.Empty() );

			_queryIds.Resize( _descr.count * stat_targets.Count(), false );
			_targets.Resize( _queryIds.Count(), false );

			FOR( i, _targets ) {
				_targets[i] = stat_targets[ i % stat_targets.Count() ];
			}
		}
		else
		{
			_queryIds.Resize( _descr.count, false );
			_targets.Resize( _queryIds.Count(), false );
		}

		GL_CHECK( glGenQueries( GLsizei(_queryIds.Count()), OUT _queryIds.RawPtr() ));

		ZeroMem( ArrayRef<gl::GLenum>(_targets) );
		return true;
	}
	
/*
=================================================
	_DestroyQueryPool
=================================================
*/
	void GL4QueryPool::_DestroyQueryPool ()
	{
		using namespace gl;

		if ( not _queryIds.Empty() )
		{
			GL_CALL( glDeleteQueries( GLsizei(_queryIds.Count()), _queryIds.RawPtr() ) );
		}

		_queryIds.Clear();
		_targets.Clear();

		_descr = Uninitialized;
	}


}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4QueryPool (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuQueryPool &ci)
	{
		return New< PlatformGL::GL4QueryPool >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
