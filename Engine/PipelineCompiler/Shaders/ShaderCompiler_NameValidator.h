// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Common/Common.h"
#include "Engine/PipelineCompiler/Common/ShaderEnums.h"

namespace PipelineCompiler
{

	//
	// Shader Variables Name Validator
	//

	class ShaderVarNameValidator final
	{
	// types
	private:
		enum SymbolID : uint {};
		using UniqueNames_t		= Map< SymbolID, String >;
		using UsedNames_t		= HashSet< String >;
		using SpecialCases_t	= HashMap< String, Array<String> >;
		using UniqueFuncs_t		= HashMap< String, String >;	// { signature, unique name }


	// variables
	private:
		// local variables and function arguments
		UniqueNames_t		_uniqueLocals;
		UsedNames_t			_usedLocalNames;
		uint				_lastFnScopeId	= 0;
		
		// global variables and external objects
		UniqueNames_t		_uniqueGlobals;
		UsedNames_t			_usedGlobalNames;

		// functions
		UniqueFuncs_t		_definedFuncs;
		UsedNames_t			_existingFuncs;
		const bool			_funcOverloadingSupported;

		const EShaderFormat::type	_dstShader;


	// methods
	public:
		explicit ShaderVarNameValidator (EShaderFormat::type dstShader, bool funcOverloading = true) :
			_funcOverloadingSupported{funcOverloading}, _dstShader{dstShader}
		{}

		bool ValidateLocalVar (INOUT String &name, uint id, uint fnScopeId);
		bool ValidateGlobalVar (INOUT String &name, uint id);
		bool ValidateFunction (StringCRef signature, OUT String &name);
		bool ValidateField (INOUT String &name);

	private:
		void _ReplaceKeyword (INOUT String &name) const;
		void _ReplaceBuiltinFunctions (INOUT String &name) const;
		
		static void _ReplaceGLSLKeyword (INOUT String &name);
		static void _ReplaceCLKeyword (INOUT String &name);
		static void _ReplaceCPPKeyword (INOUT String &name);
		static void _ReplaceHLSLKeyword (INOUT String &name);
		
		static void _ReplaceGLSLBuiltinFn (INOUT String &name);
		static void _ReplaceCLBuiltinFn (INOUT String &name);
		static void _ReplaceCPPBuiltinFn (INOUT String &name);
		static void _ReplaceHLSLBuiltinFn (INOUT String &name);

		static SpecialCases_t const&  _GetSpecialCases ();
	};

}	// PipelineCompiler
