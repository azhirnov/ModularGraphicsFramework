// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Common.h"

namespace CodeGen
{
	class IGenerator;


	//
	// Code Generator Application
	//

	class CodeGenApp final : public StaticRefCountedObject
	{
	// types
	private:
		using CmdQueue_t			= Queue< Function<void()> >;
		using Generator_t			= UniquePtr< IGenerator >;
		using GeneratorCallback_t	= Function<void (IGenerator*)>;


	// variables
	private:
		ModulePtr				_gpuThread;
		
		GXScript::ScriptEngine	_scriptEngine;

		CmdQueue_t				_cmdQueue;

		Generator_t				_currGenerator;
		GeneratorCallback_t		_onGenerationCompleted;

		bool					_looping		= true;


	// methods
	public:
		CodeGenApp ();
		~CodeGenApp ();
		
		bool Initialize (GAPI::type api, StringCRef device);
		void Quit ();
		bool Update ();

		void Run (Function<void()> &&func);
		void SetGenerator (Generator_t &&gen, GeneratorCallback_t &&cb);

		bool RunScript (StringCRef fname);

		static Ptr<CodeGenApp> Instance ();

	private:
		bool _OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &);
		bool _Draw (const Message< ModuleMsg::Update > &);
		bool _GInit (const Message< GpuMsg::DeviceCreated > &);
		bool _GDelete (const Message< GpuMsg::DeviceBeforeDestroy > &);

		void _RunTests ();
		void _BindToScript ();
	};

}	// CodeGen
