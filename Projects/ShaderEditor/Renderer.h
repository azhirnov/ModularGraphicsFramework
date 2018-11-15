// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Common.h"
#include "Pipelines/all_pipelines.h"

namespace ShaderEditor
{

	//
	// Renderer
	//
	
	class Renderer final : public StaticRefCountedObject
	{
	// types
	public:

		//
		// Shader Description
		//
		struct ShaderDescr
		{
		// types
			using Channels_t	= FixedSizeArray< Pair<String, uint>, 8 >;
			using PplnCtor_t	= void (*) (OUT PipelineTemplateDescription &);

		// variables
			PplnCtor_t		_pplnCtor	= null;
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
			float3	iCameraFrustumRay0;		// left bottom - frustum rays
			float3	iCameraFrustumRay1;		// right bottom
			float3	iCameraFrustumRay2;		// left top
			float3	iCameraFrustumRay3;		// right top
			float3	iCameraPos;				// camera position in world space
		};

		static constexpr uint	MAX_PASSES	= 4;	// 2 passes for 2 framebuffers (VR)


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
				uint2		viewport;
			};

			using PerPass_t	= StaticArray< PerPass, MAX_PASSES >;

		// variables
		public:
			const ShaderDescr	_descr;
			ModulePtr			_pipelineTemplate;
			ModulePtr			_pipeline;
			PerPass_t			_perPass;
			bool				_isCompute	= false;

		// methods
		public:
			explicit Shader (const ShaderDescr &descr);

			bool Update (const ModulePtr &builder, const ShaderData &data, uint passIdx);
			void Destroy ();
			
		private:
			bool _UpdateUBuffer (const ModulePtr &builder, const ShaderData &data, uint passIdx);
			bool _UpdateGraphics (const ModulePtr &builder, const ShaderData &data, uint passIdx);
			bool _UpdateCompute (const ModulePtr &builder, const ShaderData &data, uint passIdx);
		};

		SHARED_POINTER( Shader );

		using ShadersMap_t		= HashMap< String, ShaderPtr >;
		using ResTables_t		= StaticArray< ModulePtr, MAX_PASSES >;

		using LoadedImages_t	= HashMap< String, ModulePtr >;


	// variables
	private:
		GraphicsModuleIDs				_ids;
		ComputeModuleIDs				_computeIDs;

		ShadersMap_t					_shaders;
		Array< ShaderPtr >				_ordered;

		LoadedImages_t					_loadedImages;
		ModulePtr						_asyncCmdBuilder;
		ModulePtr						_builtinFileProvider;

		ModulePtr						_nearestClampSampler;
		ModulePtr						_linearClampSampler;
		ModulePtr						_nearestRepeatSampler;
		ModulePtr						_linearRepeatSampler;

		ModulePtr						_drawTexQuadPipeline;
		ResTables_t						_resourceTables;

		const GlobalSystemsRef			_gs;

		uint							_frameCounter;
		TimeProfilerD					_timer;
		TimeD							_lastUpdateTime;

		uint2							_surfaceSize;
		uint2							_scaledSurfaceSize;
		float							_sufaceScale	= 1.0f;

		ShaderData						_ubData;

		float2							_mousePos;
		bool							_mousePressed	= false;

		uint							_passIdx : 1;
		bool							_vrMode		= false;


	// methods
	public:
		explicit Renderer (GlobalSystemsRef gs);

		bool Inititalize (float surfaceScale);

		bool Add (StringCRef name, const ShaderDescr &descr);

		void Reset ();

		bool Update (const SceneMsg::CameraRequestUpdate &);

	private:
		bool _RecreateAll (const uint2 &newSize);
		bool _CreateShader (const ShaderPtr &shader, const uint2 &newSize);
		bool _LoadImage (StringCRef filename, OUT ModulePtr &mod);
		
		bool _CreateSamplers ();
		bool _CreateCmdBuffer (const ModulePtr &cmdBuilder);
		bool _CreateDrawTexQuadPipeline ();
		
		void _UpdateShaderData (const SceneMsg::CameraGetState::State &);
		
		void _OnMouseX (const ModuleMsg::InputMotion &);
		void _OnMouseY (const ModuleMsg::InputMotion &);

		void _OnMouseLeftButtonDown (const ModuleMsg::InputKey &);
		void _OnMouseLeftButtonUp (const ModuleMsg::InputKey &);
	};

}	// ShaderEditor
