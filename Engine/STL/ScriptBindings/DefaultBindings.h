// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "ScriptBinder.h"

#ifdef GX_ANGELSCRIPT_ENABLED

namespace GX_STL
{
namespace GXScript
{
	

	//
	// Vector
	//
	GX_DECL_SCRIPT_TYPE( GXMath::bool2,		"bool2" );
	GX_DECL_SCRIPT_TYPE( GXMath::int2,		"int2" );
	//GX_DECL_SCRIPT_TYPE( GXMath::uint2,	"uint2" );
	GX_DECL_SCRIPT_TYPE( GXMath::ilong2,	"ilong2" );
	//GX_DECL_SCRIPT_TYPE( GXMath::ulong2,	"ulong2" );
	GX_DECL_SCRIPT_TYPE( GXMath::float2,	"float2"  );
	GX_DECL_SCRIPT_TYPE( GXMath::double2,	"double2" );
	
	GX_DECL_SCRIPT_TYPE( GXMath::bool3,		"bool3" );
	GX_DECL_SCRIPT_TYPE( GXMath::int3,		"int3" );
	//GX_DECL_SCRIPT_TYPE( GXMath::uint3,	"uint3" );
	GX_DECL_SCRIPT_TYPE( GXMath::ilong3,	"ilong3" );
	//GX_DECL_SCRIPT_TYPE( GXMath::ulong3,	"ulong3" );
	GX_DECL_SCRIPT_TYPE( GXMath::float3,	"float3"  );
	GX_DECL_SCRIPT_TYPE( GXMath::double3,	"double3" );
	
	GX_DECL_SCRIPT_TYPE( GXMath::bool4,		"bool4" );
	GX_DECL_SCRIPT_TYPE( GXMath::int4,		"int4" );
	//GX_DECL_SCRIPT_TYPE( GXMath::uint4,	"uint4" );
	GX_DECL_SCRIPT_TYPE( GXMath::ilong4,	"ilong4" );
	//GX_DECL_SCRIPT_TYPE( GXMath::ulong4,	"ulong4" );
	GX_DECL_SCRIPT_TYPE( GXMath::float4,	"float4"  );
	GX_DECL_SCRIPT_TYPE( GXMath::double4,	"double4" );


	//
	// Default Bindings
	//

	struct DefaultBindings
	{
		static void BindMath (AngelScript::asIScriptEngine *se);

		static void BindVector (AngelScript::asIScriptEngine *se);
		static void BindVectorMath (AngelScript::asIScriptEngine *se);
	};
	

}	// GXScript
}	// GX_STL

#endif	// GX_ANGELSCRIPT_ENABLED
