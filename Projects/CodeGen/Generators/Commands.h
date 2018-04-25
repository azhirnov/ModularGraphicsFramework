// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Common.h"

namespace CodeGen
{

	struct EConstantSet
	{
		enum type : uint
		{
			Math,
			Physics,
			MathMostPopularOnly,
			PhysicsMostPopularOnly,

			_Count,
			Unknown = ~0u,
		};
		
		GX_ENUM_BITFIELD( EConstantSet );
	};

	struct ECommandSet
	{
		enum type : uint
		{
			FloatBaseCommands,

			_Count,
			Unknown = ~0u,
		};
		
		GX_ENUM_BITFIELD( ECommandSet );
	};



	//
	// Generator Commands
	//

	class Commands
	{
	// types
	public:
		struct CommandData final
		{
		// types
			using FuncToStr	= void (*) (OUT String &);

		// variables
			FuncToStr	toStr	= null;		// covert function to source code
			FuncToStr	toGPU	= null;		// covert function to GLSL source code
			String		name;
			ushort		ticks	= 0;		// processor ticks for this function
			ubyte		args	= 0;		// number of input arguments

		// methods
			CommandData () {}

			CommandData (CommandData &&) = default;
			CommandData (const CommandData &) = default;

			CommandData (FuncToStr toStr, FuncToStr toGPU, StringCRef name, ubyte args, ushort ticks) :
				toStr(toStr), toGPU(toGPU), name(name), ticks(ticks), args(args)
			{}
		};


	// methods
	public:
		static ArrayCRef<CommandData>  GetCommands (ECommandSet::bits flags);

		//template <typename T>
		//static Array<T>  GetConstants (EConstantSet::bits flags);


	private:
		static ArrayCRef<CommandData>  _GetFloatBaseCommands ();
	};

}	// CodeGen
