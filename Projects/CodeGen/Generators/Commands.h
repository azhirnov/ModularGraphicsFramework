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
			PrimeNumbers,
			Values0to255,

			_Count,
			Unknown = ~0u,
		};
		
		GX_ENUM_BITFIELD( EConstantSet );
	};

	struct ECommandSet
	{
		enum type : uint
		{
			FloatArithmetic,
			IntArithmetic,
			IntBitwise,

			_Count,
			Unknown = ~0u,
		};
		
		GX_ENUM_BITFIELD( ECommandSet );
	};



	//
	// Generator Commands
	//

	class Commands final : public Noninstancable
	{
	// types
	public:
		struct CommandData final
		{
		// types
			using FuncToGLSL	= void (*) (OUT String &);
			using FuncToSrc		= void (*) (ArrayCRef<StringCRef>, OUT String &);

		// variables
			FuncToSrc	toStr	= null;		// covert function to source code
			FuncToGLSL	toGPU	= null;		// covert function to GLSL source code
			String		name;
			ushort		ticks	= 0;		// processor ticks for this function
			ubyte		args	= 0;		// number of input arguments

		// methods
			CommandData () {}

			CommandData (CommandData &&) = default;
			CommandData (const CommandData &) = default;

			CommandData (FuncToSrc toStr, FuncToGLSL toGPU, StringCRef name, ubyte args, ushort ticks) :
				toStr(toStr), toGPU(toGPU), name(name), ticks(ticks), args(args)
			{}
		};


	// methods
	public:
		static ArrayCRef<CommandData>  GetCommands (ECommandSet::bits flags);

		template <typename T>
		static Set<T>  GetConstants (EConstantSet::bits flags);


	private:
		static ArrayCRef<CommandData>  _GetFloatArithmetic ();
		static ArrayCRef<CommandData>  _GetIntArithmetic ();
		static ArrayCRef<CommandData>  _GetIntBitwise ();

		template <typename T>  static Array<T>  _GetMathConstantsF ();
		template <typename T>  static Array<T>  _GetPhysicsConstantsF ();
		template <typename T>  static Array<T>  _GetMathMostPopularOnlyConstantsF ();
		template <typename T>  static Array<T>  _GetPhysicsMostPopularOnlyConstantsF ();

		template <typename T>  static Array<T>  _GetPrimeNumbersConstants ();
		template <typename T>  static Array<T>  _GetValues0to255Constants ();
	};

}	// CodeGen
