// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Graphics/Shared/BatchRenderer.h"
#include "Engine/Graphics/Shared/Commands.h"
#include "Engine/Graphics/Impl/GraphicsObjectsConstructor.h"
#include "Engine/Graphics/Impl/GraphicsBaseModule.h"

namespace Engine
{
namespace Graphics
{
	using namespace Engine::Platforms;



	//
	// Batch Renderer
	//

	class BatchRenderer final : public GraphicsBaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GraphicsBaseModule::SupportedMessages_t::Append< MessageListFrom<
											GraphicsMsg::BatchRendererSetMaterial,
											GraphicsMsg::BatchRendererSetCustomMaterial,
											GraphicsMsg::AddBatch,
											GraphicsMsg::BeginBatchRenderer,
											GraphicsMsg::FlushBatchRenderer
										> >;

		using SupportedEvents_t		= GraphicsBaseModule::SupportedEvents_t;
		
		using DeviceMsgList_t		= MessageListFrom< GpuMsg::GetDeviceInfo >;
		using DeviceEventList_t		= MessageListFrom< GpuMsg::DeviceCreated, GpuMsg::DeviceBeforeDestroy >;

		using GThreadMsgList_t		= MessageListFrom< GpuMsg::ThreadBeginFrame, GpuMsg::ThreadEndFrame, GpuMsg::GetGraphicsModules >;

		using LayerName_t			= GraphicsMsg::BatchRendererSetMaterial::LayerName_t;


		struct Material
		{
		// variables
			ModulePtr			pipeline;
			ModulePtr			resourceTable;
			LayerName_t			layer;
			bool				userDefined		= false;

		// methods
			Material (GX_DEFCTOR) {}

			bool operator == (const Material &right) const	{
				return layer == right.layer and pipeline == right.pipeline and resourceTable == right.resourceTable;
			}
		};


		struct Batch
		{
		// variables
			Array<uint>			indices;
			VertexInputState	attribs;
			EPrimitive::type	primitive;
			Material			material;

		// methods
			Batch (const VertexInputState &attribs, const Material &mtr, EPrimitive::type primitive) :
				attribs(attribs), primitive(primitive), material( mtr )
			{}
		};


		struct CmdBuffer
		{
		// variables
			Array<ModulePtr>	resourceTables;
		};

		using Batches_t				= Array< Batch >;	// TODO: use HashMap

		using DefMaterial_t			= Optional< GraphicsMsg::BatchRendererSetMaterial >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		CreateInfo::BatchRenderer	_descr;

		// resources
		GraphicsModuleIDs			_moduleIDs;

		// current state
		Batches_t					_batches;
		BinaryArray					_vertices;
		Material					_currMaterial;
		DefMaterial_t				_defMaterial;
		ModulePtr					_currRenderPass;

		// statistic
		// TODO


	// methods
	public:
		BatchRenderer (GlobalSystemsRef gs, const CreateInfo::BatchRenderer &ci);
		~BatchRenderer ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);

		bool _AddBatch (const Message< GraphicsMsg::AddBatch > &);
		bool _BatchRendererSetMaterial (const Message< GraphicsMsg::BatchRendererSetMaterial > &);
		bool _BatchRendererSetCustomMaterial (const Message< GraphicsMsg::BatchRendererSetCustomMaterial > &);
		bool _BeginBatchRenderer (const Message< GraphicsMsg::BeginBatchRenderer > &);
		bool _FlushBatchRenderer (const Message< GraphicsMsg::FlushBatchRenderer > &);

	// events
		void _OnFrameCompleted (uint index);


	private:
		bool _CreatePipeline (const GraphicsMsg::BatchRendererSetMaterial &data, const VertexInputState &attribs, EPrimitive::type primitive, OUT ModulePtr &pipeline);
		bool _CreatePipelineResources (const GraphicsMsg::BatchRendererSetMaterial &data, const ModulePtr &pipeline, OUT ModulePtr &resourceTable);
		bool _CreateCurrentMaterial (const VertexInputState &attribs, EPrimitive::type primitive);

		void _ClearCurrent ();

		bool _CreateBatch (const GraphicsMsg::AddBatch &data, OUT Ptr<Batch> &batch);
		void _AlignVertices (Ptr<Batch> batch);
		bool _CopyVertices (Ptr<Batch> batch, const GraphicsMsg::AddBatch &data);
		bool _CopyIndices (Ptr<Batch> batch, const GraphicsMsg::AddBatch &data);
		bool _GenIndexSequence (Ptr<Batch> batch, const GraphicsMsg::AddBatch &data);

		template <typename T>
		bool _CopyIndicesT (Ptr<Batch> batch, const GraphicsMsg::AddBatch &data);
		bool _CheckIndicesCount (usize count, EPrimitive::type primitive) const;
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	BatchRenderer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	BatchRenderer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	BatchRenderer::BatchRenderer (GlobalSystemsRef gs, const CreateInfo::BatchRenderer &ci) :
		GraphicsBaseModule( gs, ModuleConfig{ BatchRendererModuleID, ~0u }, &_msgTypes, &_eventTypes ),
		_descr( ci )
	{
		SetDebugName( "BatchRenderer" );

		_SubscribeOnMsg( this, &BatchRenderer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &BatchRenderer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &BatchRenderer::_AttachModule_Impl );
		_SubscribeOnMsg( this, &BatchRenderer::_DetachModule_Impl );
		_SubscribeOnMsg( this, &BatchRenderer::_FindModule_Impl );
		_SubscribeOnMsg( this, &BatchRenderer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &BatchRenderer::_Link );
		_SubscribeOnMsg( this, &BatchRenderer::_Compose );
		_SubscribeOnMsg( this, &BatchRenderer::_Delete );
		_SubscribeOnMsg( this, &BatchRenderer::_OnManagerChanged );
		_SubscribeOnMsg( this, &BatchRenderer::_AddBatch );
		_SubscribeOnMsg( this, &BatchRenderer::_BatchRendererSetMaterial );
		_SubscribeOnMsg( this, &BatchRenderer::_BatchRendererSetCustomMaterial );
		_SubscribeOnMsg( this, &BatchRenderer::_BeginBatchRenderer );
		_SubscribeOnMsg( this, &BatchRenderer::_FlushBatchRenderer );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGpuThread( ci.gpuThread ), UntypedID_t(0), true );

		// validate descriptor
		_descr.vertexAlign	= Clamp( _descr.vertexAlign, 4_b, 256_b );
		_descr.blockSize	= Clamp( _descr.blockSize, 1_Kb, 128_Mb );
	}
	
/*
=================================================
	destructor
=================================================
*/
	BatchRenderer::~BatchRenderer ()
	{
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool BatchRenderer::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_descr = Uninitialized;

		_ClearCurrent();

		// TODO: delete all resources

		return _Delete_Impl( msg );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool BatchRenderer::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial );

		// find graphics thread
		ModulePtr	gthread;
		CHECK_LINKING( gthread = GlobalSystems()->parallelThread->GetModuleByMsg< GThreadMsgList_t >() );

		Message< GpuMsg::GetGraphicsModules >	req_ids;
		gthread->Send( req_ids );

		_moduleIDs << req_ids->graphics;

		return Module::_Link_Impl( msg );
	}

/*
=================================================
	_Compose
=================================================
*/
	bool BatchRenderer::_Compose (const Message< ModuleMsg::Compose > &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		_batches.Reserve( 16 );
		_vertices.Reserve( 1024 );

		return _DefCompose( false );
	}
	
/*
=================================================
	PrimitiveStripToList
=================================================
*/
	inline EPrimitive::type PrimitiveStripToList (EPrimitive::type value)
	{
		switch ( value )
		{
			case EPrimitive::LineList :
			case EPrimitive::LineStrip :
				return EPrimitive::LineList;

			case EPrimitive::TriangleList :
			case EPrimitive::TriangleStrip :
				return EPrimitive::TriangleList;
		}
		return value;
	}
	
/*
=================================================
	_CreateBatch
=================================================
*/
	bool BatchRenderer::_CreateBatch (const GraphicsMsg::AddBatch &data, OUT Ptr<Batch> &batch)
	{
		// change primitive type
		EPrimitive::type	new_primitive;

		switch ( data.primitive )
		{
			case EPrimitive::Point :
				new_primitive = EPrimitive::Point;
				break;

			case EPrimitive::LineList :
			case EPrimitive::LineStrip :
				new_primitive = EPrimitive::LineList;
				break;

			case EPrimitive::TriangleList :
			case EPrimitive::TriangleStrip :
				new_primitive = EPrimitive::TriangleList;
				break;

			//case EPrimitive::Patch :				// will be supported later
			//	new_primitive = EPrimitive::Patch;
			//	break;

			default :
				WARNING( "unknown primitive" );
				new_primitive = data.primitive;
		}

		// change attribs
		CHECK_ERR( data.attribs.Bindings().Count() == 1 );

		VertexInputState	aligned_attribs{ data.attribs.Attribs(), Uninitialized };

		aligned_attribs.Bind( "", AlignToLarge( data.attribs.Bindings().Front().second.stride, _descr.vertexAlign ) );

		// check material
		if ( _currMaterial.userDefined )
		{
			Message< GpuMsg::GetGraphicsPipelineDescriptor >	req_descr;
			_currMaterial.pipeline->Send( req_descr );
			CHECK_ERR( req_descr->result );

			// user defined pipeline must be compatible with current vertex input state,
			// names from batch attribs will be ignored
			CHECK_ERR( req_descr->result->vertexInput.Equals( aligned_attribs, true ) );
		}
		else
		{
			CHECK_ERR( _CreateCurrentMaterial( aligned_attribs, new_primitive ) );
		}

		// create
		_batches.PushBack( Batch{ aligned_attribs, _currMaterial, new_primitive } );

		batch = &_batches.Back();
		return true;
	}
	
/*
=================================================
	_AlignVertices
=================================================
*/
	void BatchRenderer::_AlignVertices (Ptr<Batch> batch)
	{
		const BytesU	stride	= batch->attribs.Bindings().Front().second.stride;
		const BytesU	aligned	= AlignToLarge( _vertices.Size(), stride );

		_vertices.Resize( (usize)aligned );
	}

/*
=================================================
	_CopyVertices
=================================================
*/
	bool BatchRenderer::_CopyVertices (Ptr<Batch> batch, const GraphicsMsg::AddBatch &data)
	{
		// add vertices
		const usize		src_stride	= (usize) data.attribs.Bindings().Front().second.stride;
		const usize		dst_stride	= (usize) batch->attribs.Bindings().Front().second.stride;
		const usize		count		= (usize) data.vertices.Size() / src_stride;
		const usize		offset		= (usize) _vertices.Size();
		
		ASSERT( offset % dst_stride == 0 );

		_vertices.Resize( offset + dst_stride * count );

		for (usize i = 0; i < count; ++i)
		{
			MemCopy( _vertices.SubArray( offset + dst_stride * i, src_stride ),
					 data.vertices.SubArray( src_stride * i, src_stride ) );
		}
		return true;
	}
	
/*
=================================================
	_CheckIndicesCount
=================================================
*/
	inline bool BatchRenderer::_CheckIndicesCount (usize count, EPrimitive::type primitive) const
	{
		switch ( primitive )
		{
			case EPrimitive::Point :			return true;
			case EPrimitive::LineList :			return count % 2;
			case EPrimitive::LineStrip :		return count >= 2;
			case EPrimitive::TriangleList :		return count % 3;
			case EPrimitive::TriangleStrip :	return count >= 3;
		}
		return true;
	}

/*
=================================================
	_CopyIndicesT
=================================================
*/
	template <typename T>
	inline bool BatchRenderer::_CopyIndicesT (Ptr<Batch> batch, const GraphicsMsg::AddBatch &data)
	{
		// add indices
		const usize		offset	= (usize) _vertices.Size();
		const usize		stride	= (usize) batch->attribs.Bindings().Front().second.stride;
		const usize		first	= offset / stride;
		const usize		count	= (usize) data.indices.Size() / sizeof(T);
		const T *		indices	= PointerCast< const T >( data.indices.ptr() );

		CHECK_ERR( _CheckIndicesCount( count, data.primitive ) );

		switch ( data.primitive )
		{
			case EPrimitive::Point :
			case EPrimitive::LineList :
			case EPrimitive::TriangleList :
			{
				batch->indices.Reserve( batch->indices.Count() + count );

				for (usize i = 0; i < count; ++i) {
					batch->indices << uint(first + indices[i]);
				}
				break;
			}

			case EPrimitive::LineStrip :
			{
				batch->indices.Reserve( batch->indices.Count() + (count-1)*2 );
				batch->indices << uint(first + indices[0]);
				batch->indices << uint(first + indices[1]);
				
				for (usize i = 2; i < count; ++i)
				{
					batch->indices << uint(first + indices[i-1]);
					batch->indices << uint(first + indices[i]);
				}
				break;
			}

			case EPrimitive::TriangleStrip :
			{
				batch->indices.Reserve( batch->indices.Count() + (count-2)*3 );
				batch->indices << uint(first + indices[0]);
				batch->indices << uint(first + indices[1]);
				batch->indices << uint(first + indices[2]);
				
				for (usize i = 3; i < count; ++i)
				{
					batch->indices << uint(first + indices[i-2]);
					batch->indices << uint(first + indices[i-1]);
					batch->indices << uint(first + indices[i]);
				}
				break;
			}
		}
		return true;
	}
	
/*
=================================================
	_GenIndexSequence
=================================================
*/
	bool BatchRenderer::_GenIndexSequence (Ptr<Batch> batch, const GraphicsMsg::AddBatch &data)
	{
		const usize		offset	= (usize) _vertices.Size();
		const usize		stride	= (usize) batch->attribs.Bindings().Front().second.stride;
		const usize		count	= (usize) data.vertices.Size() / stride;
		const usize		first	= offset / stride;

		ASSERT( offset % stride == 0 );
		CHECK_ERR( _CheckIndicesCount( count, data.primitive ) );

		for (usize i = 0; i < count; ++i) {
			batch->indices << uint(first + i);
		}
		return true;
	}

/*
=================================================
	_CopyIndices
=================================================
*/
	bool BatchRenderer::_CopyIndices (Ptr<Batch> batch, const GraphicsMsg::AddBatch &data)
	{
		if ( data.indices.Empty() )
			return _GenIndexSequence( batch, data );

		switch ( data.indexType )
		{
			case EIndex::UShort	:	return _CopyIndicesT<ushort>( batch, data );
			case EIndex::UInt :		return _CopyIndicesT<uint>( batch, data );
			default :				RETURN_ERR( "unsupported index type!" );
		}
	}

/*
=================================================
	_AddBatch
=================================================
*/
	bool BatchRenderer::_AddBatch (const Message< GraphicsMsg::AddBatch > &msg)
	{
		Ptr<Batch>				curr;
		const EPrimitive::type	primitive	= PrimitiveStripToList( msg->primitive );

		// search in existing batches
		FOR( i, _batches )
		{
			auto&	batch = _batches[i];

			if ( batch.primitive			== primitive			and
				 batch.attribs.Attribs()	== msg->attribs.Attribs() )
			{
				if ( batch.material == _currMaterial )
				{
					curr = &batch;
					break;
				}

				if ( not batch.material.userDefined and _defMaterial )
				{
					CHECK_ERR( _CreateCurrentMaterial( batch.attribs, batch.primitive ) );
					
					if ( batch.material == _currMaterial )
					{
						curr = &batch;
						break;
					}
				}
			}
		}

		// create new batch
		if ( not curr ) {
			CHECK_ERR( _CreateBatch( *msg, OUT curr ) );
		}

		_AlignVertices( curr );
		CHECK_ERR( _CopyIndices( curr, *msg ) );
		CHECK_ERR( _CopyVertices( curr, *msg ) );

		return true;
	}
	
/*
=================================================
	_CreatePipeline
=================================================
*/
	bool BatchRenderer::_CreatePipeline (const GraphicsMsg::BatchRendererSetMaterial &data, const VertexInputState &attribs,
										 EPrimitive::type primitive, OUT ModulePtr &pipeline)
	{
		Message< GpuMsg::CreateGraphicsPipeline >	create_gpp;

		create_gpp->topology	= primitive;
		create_gpp->moduleID	= _moduleIDs.pipeline;
		create_gpp->vertexInput	= attribs;
		create_gpp->renderPass	= _currRenderPass;

		pipeline = null;

		// multitextured
		if ( data.image0 and data.image1 )
		{
			CHECK_ERR( _descr.multitexturedShader );
			CHECK_ERR( _descr.multitexturedShader->Send( create_gpp ) );
			pipeline << create_gpp->result;
			return true;
		}

		// single texture
		if ( data.image0 )
		{
			CHECK_ERR( _descr.texturedShader );
			CHECK_ERR( _descr.texturedShader->Send( create_gpp ) );
			pipeline << create_gpp->result;
			return true;
		}

		// color only
		{
			CHECK_ERR( _descr.coloredShader );
			CHECK_ERR( _descr.coloredShader->Send( create_gpp ) );
			pipeline << create_gpp->result;
			return true;
		}
	}
	
/*
=================================================
	_CreatePipelineResources
=================================================
*/
	bool BatchRenderer::_CreatePipelineResources (const GraphicsMsg::BatchRendererSetMaterial &data, const ModulePtr &pipeline, OUT ModulePtr &resourceTable)
	{
		auto	factory = GlobalSystems()->modulesFactory;

		resourceTable = null;

		// multitextured
		if ( data.image0 and data.image1 )
		{
			CHECK_ERR( factory->Create(
							_moduleIDs.resourceTable,
							GlobalSystems(),
							CreateInfo::PipelineResourceTable{},
							OUT resourceTable )
			);
			resourceTable->Send< ModuleMsg::AttachModule >({ "pipeline", pipeline });
			resourceTable->Send< ModuleMsg::AttachModule >({ "image0", data.image0 });
			resourceTable->Send< ModuleMsg::AttachModule >({ "image1", data.image1 });

			if ( data.sampler0 )
				resourceTable->Send< ModuleMsg::AttachModule >({ "image0.sampler", data.sampler0 });

			if ( data.sampler1 )
				resourceTable->Send< ModuleMsg::AttachModule >({ "image1.sampler", data.sampler1 });

			ModuleUtils::Initialize({ resourceTable });
			return true;
		}

		// single texture
		if ( data.image0 )
		{
			CHECK_ERR( factory->Create(
							_moduleIDs.resourceTable,
							GlobalSystems(),
							CreateInfo::PipelineResourceTable{},
							OUT resourceTable )
			);
			resourceTable->Send< ModuleMsg::AttachModule >({ "pipeline", pipeline });
			resourceTable->Send< ModuleMsg::AttachModule >({ "image0", data.image0 });

			if ( data.sampler0 )
				resourceTable->Send< ModuleMsg::AttachModule >({ "image0.sampler", data.sampler0 });

			ModuleUtils::Initialize({ resourceTable });
		}

		// color only
		return true;	// no resources
	}
	
/*
=================================================
	_CreateCurrentMaterial
=================================================
*/
	bool BatchRenderer::_CreateCurrentMaterial (const VertexInputState &attribs, EPrimitive::type primitive)
	{
		if ( _currMaterial.pipeline and _currMaterial.resourceTable )
			return true;	// has current material

		// default material must be defined
		CHECK_ERR( _defMaterial );
		
		_currMaterial = Uninitialized;
		_currMaterial.layer			= _defMaterial->layer;
		_currMaterial.userDefined	= false;

		CHECK_ERR( _CreatePipeline( *_defMaterial, attribs, primitive, OUT _currMaterial.pipeline ) );
		CHECK_ERR( _CreatePipelineResources( *_defMaterial, _currMaterial.pipeline, OUT _currMaterial.resourceTable ) );
		return true;
	}

/*
=================================================
	_BatchRendererSetMaterial
=================================================
*/
	bool BatchRenderer::_BatchRendererSetMaterial (const Message< GraphicsMsg::BatchRendererSetMaterial > &msg)
	{
		_currMaterial	= Uninitialized;
		_defMaterial	= *msg;

		return true;
	}
	
/*
=================================================
	_BatchRendererSetCustomMaterial
=================================================
*/
	bool BatchRenderer::_BatchRendererSetCustomMaterial (const Message< GraphicsMsg::BatchRendererSetCustomMaterial > &msg)
	{
		CHECK_ERR( msg->pipeline and msg->resourceTable );

		_defMaterial = Uninitialized;

		_currMaterial.layer			= msg->layer;
		_currMaterial.pipeline		= msg->pipeline;
		_currMaterial.resourceTable	= msg->resourceTable;
		_currMaterial.userDefined	= true;

		return true;
	}
	
/*
=================================================
	_BeginBatchRenderer
=================================================
*/
	bool BatchRenderer::_BeginBatchRenderer (const Message< GraphicsMsg::BeginBatchRenderer > &msg)
	{
		_currRenderPass = msg->renderPass;

		// get default render pass from device
		if ( not _currRenderPass )
		{
			ModulePtr	dev;
			CHECK_ERR(( dev = GlobalSystems()->parallelThread->GetModuleByMsgEvent< DeviceMsgList_t, DeviceEventList_t >() ));

			Message< GpuMsg::GetDeviceInfo >	req_info;
			dev->Send( req_info );
			CHECK_ERR( req_info->result );

			_currRenderPass = req_info->result->renderPass;
		}
		return true;
	}

/*
=================================================
	_FlushBatchRenderer
=================================================
*/
	bool BatchRenderer::_FlushBatchRenderer (const Message< GraphicsMsg::FlushBatchRenderer > &msg)
	{
		CHECK_ERR( msg->framebuffer and msg->cmdBuilder );

		using EMappingFlags = GpuMsg::MapMemoryToCpu::EMappingFlags;

		auto		factory = GlobalSystems()->modulesFactory;

		ModulePtr	vbuffer;
		ModulePtr	ibuffer;

		BytesU		indices_size;

		FOR( i, _batches ) {
			indices_size += _batches[i].indices.Size();
		}

		// TODO: check render pass compatibility
		// TODO: staging buffers ?

		// create vertex buffer
		CHECK_ERR( factory->Create(
						_moduleIDs.buffer,
						GlobalSystems(),
						CreateInfo::GpuBuffer{
							BufferDescriptor{
								_vertices.Size(),
								EBufferUsage::bits() | EBufferUsage::Vertex
							},
							EGpuMemory::bits() | EGpuMemory::CoherentWithCPU,
							EMemoryAccess::All },
						OUT vbuffer ) );

		// create indix buffer
		CHECK_ERR( factory->Create(
						_moduleIDs.buffer,
						GlobalSystems(),
						CreateInfo::GpuBuffer{
							BufferDescriptor{
								indices_size,
								EBufferUsage::bits() | EBufferUsage::Index
							},
							EGpuMemory::bits() | EGpuMemory::CoherentWithCPU,
							EMemoryAccess::All },
						OUT ibuffer ) );

		ModuleUtils::Initialize({ vbuffer, ibuffer });

		// copy vertices
		{
			vbuffer->Send< GpuMsg::MapMemoryToCpu >({ EMappingFlags::WriteDiscard, _vertices.Size() });
			vbuffer->Send< GpuMsg::WriteToStream >({ _vertices });
			vbuffer->Send< GpuMsg::UnmapMemory >({});
			//vbuffer->Send< GpuMsg::WriteToGpuMemory >({ _vertices });
		}

		// copy indices
		{
			BytesU	offset;
			ibuffer->Send< GpuMsg::MapMemoryToCpu >({ EMappingFlags::WriteDiscard, indices_size });

			FOR( i, _batches ) {
				ibuffer->Send< GpuMsg::WriteToStream >({ ArrayCRef<uint>( _batches[i].indices ), offset });
				offset += _batches[i].indices.Size();
			}
			ibuffer->Send< GpuMsg::UnmapMemory >({});
			
			/*FOR( i, _batches ) {
				ibuffer->Send< GpuMsg::WriteToGpuMemory >({ ArrayCRef<uint>( _batches[i].indices ), offset });
				offset += _batches[i].indices.Size();
			}*/
		}

		// build commands
		{
			auto		builder				= msg->cmdBuilder;
			uint		idx_offset			= 0;

			builder->Send< GpuMsg::CmdBeginRenderPass >({ _currRenderPass, msg->framebuffer, msg->viewport });
			
			builder->Send< GpuMsg::CmdBindVertexBuffers >({ vbuffer });
			builder->Send< GpuMsg::CmdBindIndexBuffer >({ ibuffer, EIndex::UInt });

			FOR( i, _batches )
			{
				auto const&	batch	= _batches[i];
				
				builder->Send< GpuMsg::CmdBindGraphicsPipeline >({ batch.material.pipeline });
				builder->Send< GpuMsg::CmdBindGraphicsResourceTable >({ batch.material.resourceTable });
				builder->Send< GpuMsg::CmdSetViewport >({ msg->viewport, float2(0.0f, 1.0f) });
				builder->Send< GpuMsg::CmdSetScissor >({ msg->viewport });
				builder->Send< GpuMsg::CmdDrawIndexed >({ uint(batch.indices.Count()), 1u, idx_offset });

				idx_offset += uint(batch.indices.Count());
			}

			builder->Send< GpuMsg::CmdEndRenderPass >({});

		}
		
		auto	on_completed =	LAMBDA( vbuffer, ibuffer ) (uint)
								{
									Message< ModuleMsg::Delete >	del_msg;
									vbuffer->Send( del_msg );
									ibuffer->Send( del_msg );
								};

		CHECK( msg->cmdBuilder->Send< GraphicsMsg::SubscribeOnFrameCompleted >( RVREF(on_completed) ) );

		_ClearCurrent();
		return true;
	}
	
/*
=================================================
	_ClearCurrent
=================================================
*/
	void BatchRenderer::_ClearCurrent ()
	{
		_currMaterial	= Uninitialized;
		_currRenderPass	= null;

		_vertices.Clear();
		_batches.Clear();
	}
//-----------------------------------------------------------------------------

	
/*
=================================================
	CreateBatchRenderer
=================================================
*/
	ModulePtr GraphicsObjectsConstructor::CreateBatchRenderer (GlobalSystemsRef gs, const CreateInfo::BatchRenderer &ci)
	{
		return New< BatchRenderer >( gs, ci );
	}

}	// Graphics
}	// Engine
