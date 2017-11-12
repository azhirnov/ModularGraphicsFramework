// Copyright �  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "../Common.h"

class CLApp final : public StaticRefCountedObject
{
// variables
public:
	Ptr< Module >		ms;

	ModulePtr			buffer;
	ModulePtr			image;

	ModulePtr			cpipeline;
	ModulePtr			pipelineTemplate;
	ModulePtr			resourceTable;
	
	Array<ModulePtr>	cmdBuffers;
	ModulePtr			cmdBuilder;
	uint				cmdBufIndex	= 0;

private:
	bool				looping	= true;


// methods
public:
	CLApp ();

	void Initialize ();
	void Quit ();
	bool Update ();

private:
	bool _Draw (const Message< ModuleMsg::Update > &msg);
	bool _CLInit (const Message< GpuMsg::DeviceCreated > &msg);
	bool _CLDelete (const Message< GpuMsg::DeviceBeforeDestroy > &msg);
};


CLApp::CLApp ()
{
	ms = GetMainSystemInstace();

	Platforms::RegisterPlatforms();
}


void CLApp::Initialize ()
{
	ms->AddModule( StreamManagerModuleID, CreateInfo::StreamManager() );
	ms->AddModule( CLContextModuleID, CreateInfo::GpuContext() );

	auto	thread	= ms->GlobalSystems()->Get< ParallelThread >();
	
	thread->AddModule( CLThreadModuleID, CreateInfo::GpuThread{ null, null, CreateInfo::GpuContext{ "CL 1.2"_GAPI } } );

	auto	clthread	= thread->GetModuleByID( CLThreadModuleID );

	clthread->Subscribe( this, &CLApp::_Draw );
	clthread->Subscribe( this, &CLApp::_CLInit );
	clthread->Subscribe( this, &CLApp::_CLDelete );

	// finish initialization
	ModuleUtils::Initialize({ ms });
}


void CLApp::Quit ()
{
	Message< ModuleMsg::Delete >	del_msg;

	if ( pipelineTemplate )
		pipelineTemplate->Send( del_msg );

	looping = false;
}


bool CLApp::Update ()
{
	if ( not looping )
		return false;

	ms->Send< ModuleMsg::Update >({});
	return true;
}


bool CLApp::_Draw (const Message< ModuleMsg::Update > &)
{
	if ( not looping )
		return false;
	
	auto	clthread	= ms->GlobalSystems()->Get< ParallelThread >()->GetModuleByID( CLThreadModuleID );

	cmdBuilder->Send< GpuMsg::CmdBegin >({ cmdBuffers[cmdBufIndex % cmdBuffers.Count()] });
	cmdBuilder->Send< GpuMsg::CmdBindComputePipeline >({ cpipeline });
	cmdBuilder->Send< GpuMsg::CmdBindComputeResourceTable >({ resourceTable });
	cmdBuilder->Send< GpuMsg::CmdDispatch >({ uint3(8, 8, 1) });

	Message< GpuMsg::CmdEnd >	cmd_end;
	cmdBuilder->Send( cmd_end );
	clthread->Send< GpuMsg::SubmitComputeQueueCommands >({ cmd_end->cmdBuffer.Get() });
	return true;
}


void Create_compute (PipelineTemplateDescriptor& descr)
{
	descr = PipelineTemplateDescriptor{};

	descr.supportedShaders |= EShader::Compute;

	descr.localGroupSize = uint3(1,1,1);

	descr.layout = PipelineLayoutDescriptor::Builder{}
		.AddImage( "outImage", EImage::Tex2D, EPixelFormat::Unknown, true, false, 0, 0, EShader::bits() | EShader::Compute )
		.Finish();

	descr.Compute().StringCL(R"#(
kernel void main (write_only image2d_t outImage)
{
	double a = 4.81729392;
	double d = sin( a );

	write_imagef(outImage, (int2)(0,0), convert_float4(d));
}
	)#");
}


bool CLApp::_CLInit (const Message< GpuMsg::DeviceCreated > &)
{
	auto	clthread = ms->GlobalSystems()->Get< ParallelThread >()->GetModuleByID( CLThreadModuleID );
	auto	factory = ms->GlobalSystems()->Get< ModulesFactory >();

	CHECK_ERR( factory->Create(
					CLImageModuleID,
					clthread->GlobalSystems(),
					CreateInfo::GpuImage{ null,
						ImageDescriptor{
							EImage::Tex2D,
							uint4( 128, 128, 0, 0 ),
							EPixelFormat::RGBA32F,
							EImageUsage::bits() | EImageUsage::Storage
						}},
					OUT image ) );

	CHECK_ERR( image->AddModule( CLMemoryModuleID, CreateInfo::GpuMemory{ null, EGpuMemory::bits() | EGpuMemory::CoherentWithCPU, EMemoryAccess::All } ) );

	CHECK_ERR( factory->Create(
					CLBufferModuleID,
					clthread->GlobalSystems(),
					CreateInfo::GpuBuffer{ null,
						BufferDescriptor{
							32_Kb,
							EBufferUsage::bits() | EBufferUsage::Storage
						}},
					OUT buffer ) );
	
	CHECK_ERR( buffer->AddModule( CLMemoryModuleID, CreateInfo::GpuMemory{ null, EGpuMemory::bits() | EGpuMemory::CoherentWithCPU, EMemoryAccess::All } ) );

	PipelineTemplateDescriptor	pp_temp_descr;
	Create_compute( OUT pp_temp_descr );

	CHECK_ERR( factory->Create(
					PipelineTemplateModuleID,
					clthread->GlobalSystems(),
					CreateInfo::PipelineTemplate{ pp_temp_descr },
					OUT pipelineTemplate ) );

	ModuleUtils::Initialize({ image, buffer, pipelineTemplate });
	
	Message< GpuMsg::CreateComputePipeline >	cppl_ctor{ CLComputePipelineModuleID };
	pipelineTemplate->Send( cppl_ctor );

	cpipeline = cppl_ctor->result.Get(null);
	
	CHECK_ERR( factory->Create(
					CLPipelineResourceTableModuleID,
					clthread->GlobalSystems(),
					CreateInfo::PipelineResourceTable{},
					OUT resourceTable )
	);
	resourceTable->Send< ModuleMsg::AttachModule >({ "pipeline", cpipeline });
	resourceTable->Send< ModuleMsg::AttachModule >({ "outImage", image });

	CHECK_ERR( factory->Create(
					CLCommandBuilderModuleID,
					clthread->GlobalSystems(),
					CreateInfo::GpuCommandBuilder{},
					OUT cmdBuilder )
	);
	
	cmdBufIndex = 0;
	cmdBuffers.Resize( 2 );

	FOR( i, cmdBuffers )
	{
		CHECK_ERR( factory->Create(
						CLCommandBufferModuleID,
						clthread->GlobalSystems(),
						CreateInfo::GpuCommandBuffer{},
						OUT cmdBuffers[i] )
		);
		cmdBuilder->Send< ModuleMsg::AttachModule >({ ""_str << i, cmdBuffers[i] });
	}

	ModuleUtils::Initialize({ cpipeline, resourceTable, cmdBuilder });
	return true;
}


bool CLApp::_CLDelete (const Message< GpuMsg::DeviceBeforeDestroy > &)
{
	looping = false;
	return true;
}


	
extern void Test_CLCompute ()
{
	{
		CLApp	app;
	
		CHECK( app.ms->GlobalSystems()->Get< FileManager >()->FindAndSetCurrentDir( "Tests/Engine.Base" ) );

		app.Initialize();

		// main loop
		for (uint i = 0; i < 10 and app.Update(); ++i) {}

		app.Quit();
	}
	GetMainSystemInstace()->Send< ModuleMsg::Delete >({});
	
	//WARNING( "OpenCL Window test succeeded!" );
}