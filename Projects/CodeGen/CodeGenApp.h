// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Common.h"

namespace CodeGen
{

	//
	// Code Generator Application
	//

	class CodeGenApp final : public StaticRefCountedObject
	{
	// types
	private:
		using CmdQueue	= Queue< Function<void()> >;


	// variables
	private:
		ModulePtr				_gpuThread;
		
		GXScript::ScriptEngine	_scriptEngine;

		CmdQueue				_cmdQueue;

		bool					_looping		= true;


	// methods
	public:
		CodeGenApp ();
		~CodeGenApp ();
		
		bool Initialize (GAPI::type api);
		void Quit ();
		bool Update ();

		void Run (Function<void()> &&func);
		bool RunScript (StringCRef fname);
		
		void Exit ();

	private:
		bool _OnWindowClosed (const Message<OSMsg::WindowAfterDestroy> &);
		bool _Draw (const Message< ModuleMsg::Update > &);
		bool _GInit (const Message< GpuMsg::DeviceCreated > &);
		bool _GDelete (const Message< GpuMsg::DeviceBeforeDestroy > &);

		void _RunTests ();
		void _BindToScript ();
	};

}	// CodeGen
