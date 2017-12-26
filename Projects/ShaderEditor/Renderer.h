// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Projects/ShaderEditor/Common.h"
#include "Projects/ShaderEditor/Pipelines/all_pipelines.h"

namespace ShaderEditor
{

	//
	// Renderer
	//
	
	class Renderer final
	{
	// types
	public:

		//
		// Shader Descriptor
		//
		struct ShaderDescr
		{
		// types
			using Channels_t	= FixedSizeArray< Pair<String, uint>, 8 >;
			using PplnCtor_t	= void (*) (OUT PipelineTemplateDescriptor &);

		// variables
			PplnCtor_t		_pplnCtor;
			Channels_t		_channels;

		// methods
			ShaderDescr ();

			ShaderDescr& Pipeline (const PplnCtor_t &value);
			ShaderDescr& InChannel (StringCRef name, uint index);
		};


	private:
		struct ShaderData
		{
			float3	iResolution;			// viewport resolution (in pixels)
			float	iTime;					// shader playback time (in seconds)
			float	iTimeDelta;				// render time (in seconds)
			int		iFrame;					// shader playback frame
			float4	iMouse;					// mouse pixel coords. xy: current (if MLB down), zw: click
			float4	iDate;					// (year, month, day, time in seconds)
			float	iSampleRate;			// sound sample rate (i.e., 44100)
		};


		//
		// Shader
		//
		class Shader : public StaticRefCountedObject
		{
		// types
		private:
			struct PerPass
			{
				ModulePtr	resourceTable;
				ModulePtr	framebuffer;
				ModulePtr	image;
				ModulePtr	ubuffer;
			};

		// variables
		public:
			const ShaderDescr			_descr;
			ModulePtr					_pipelineTemplate;
			ModulePtr					_pipeline;
			StaticArray< PerPass, 2 >	_perPass;

		// methods
		public:
			explicit Shader (const ShaderDescr &descr);

			bool Update (const ModulePtr &builder, const ShaderData &data, uint passIdx);
			bool UpdateUBuffer (const ModulePtr &builder, const ShaderData &data, uint passIdx);
			void Destroy ();
		};

		SHARED_POINTER( Shader );

		using ShadersMap_t	= HashMap< String, ShaderPtr >;


	// variables
	private:
		GraphicsModuleIDs				_ids;
		
		ModulePtr						_cmdBuilder;
		StaticArray< ModulePtr, 2 >		_cmdBuffers;

		ShadersMap_t					_shaders;
		Array< ShaderPtr >				_ordered;

		ModulePtr						_nearestClampSampler;
		ModulePtr						_linearClampSampler;
		ModulePtr						_nearestRepeatSampler;
		ModulePtr						_linearRepeatSampler;

		ModulePtr						_drawTexQuadPipeline;
		StaticArray< ModulePtr, 2 >		_resourceTables;

		const GlobalSystemsRef			_gs;

		uint							_frameCounter;
		TimeProfilerD					_timer;
		TimeD							_lastUpdateTime;
		uint2							_surfaceSize;

		ShaderData						_ubData;

		uint							_passIdx : 1;


	// methods
	public:
		explicit Renderer (GlobalSystemsRef gs);

		bool Inititalize ();

		bool Add (StringCRef name, const ShaderDescr &descr);

		bool Update (const SceneMsg::CameraRequestUpdate &);

	private:
		bool _RecreateAll (const uint2 &newSize);
		bool _CreateShader (const ShaderPtr &shader, const uint2 &newSize);
		bool _CreateCmdBuffers ();
		bool _CreateSamplers ();
		bool _CreateDrawTexQuadPipeline ();
		void _UpdateShaderData ();

		ModulePtr _GetGpuThread () const;
	};

}	// ShaderEditor
