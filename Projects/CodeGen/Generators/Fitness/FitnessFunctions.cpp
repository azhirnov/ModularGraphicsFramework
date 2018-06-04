// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "FitnessFunctions.h"

namespace CodeGen
{
	
/*
=================================================
	GetSource
=================================================
*/
	StringCRef FitnessFunctions::GetSource (EFitnessFunction::type func)
	{
		switch ( func )
		{
			case EFitnessFunction::FloatLinear :
				return
					#include "Fitness_FloatLinear.glsl"
					;
			case EFitnessFunction::Bitwise :
				return 
					#include "Fitness_Bitwise.glsl"
					;
		}
		RETURN_ERR( "unsupported fitness function" );
	}
		
}	// CodeGen
