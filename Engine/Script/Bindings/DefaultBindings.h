// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Script/Impl/ScriptEngine.h"
#include "Engine/Script/Impl/ScriptTypes.h"

#include "Engine/STL/Math/Vec.h"
#include "Engine/STL/Math/Color/Color.h"

namespace GX_STL
{
namespace GXScript
{

	//
	// Default Bindings
	//

	struct DefaultBindings final : Noninstancable
	{
		static void BindScalarMath (ScriptEngine *se);
		static void BindVectorMath (ScriptEngine *se);
		static void BindString (ScriptEngine *se);
		static void BindLog (ScriptEngine *se);
	};


	GX_DECL_SCRIPT_TYPE( GXMath::bool2,		"bool2" );
	GX_DECL_SCRIPT_TYPE( GXMath::int2,		"int2" );
	GX_DECL_SCRIPT_TYPE( GXMath::uint2,		"uint2" );
	GX_DECL_SCRIPT_TYPE( GXMath::ilong2,	"ilong2" );
	GX_DECL_SCRIPT_TYPE( GXMath::ulong2,	"ulong2" );
	GX_DECL_SCRIPT_TYPE( GXMath::float2,	"float2"  );
	GX_DECL_SCRIPT_TYPE( GXMath::double2,	"double2" );
	
	GX_DECL_SCRIPT_TYPE( GXMath::bool3,		"bool3" );
	GX_DECL_SCRIPT_TYPE( GXMath::int3,		"int3" );
	GX_DECL_SCRIPT_TYPE( GXMath::uint3,		"uint3" );
	GX_DECL_SCRIPT_TYPE( GXMath::ilong3,	"ilong3" );
	GX_DECL_SCRIPT_TYPE( GXMath::ulong3,	"ulong3" );
	GX_DECL_SCRIPT_TYPE( GXMath::float3,	"float3"  );
	GX_DECL_SCRIPT_TYPE( GXMath::double3,	"double3" );
	
	GX_DECL_SCRIPT_TYPE( GXMath::bool4,		"bool4" );
	GX_DECL_SCRIPT_TYPE( GXMath::int4,		"int4" );
	GX_DECL_SCRIPT_TYPE( GXMath::uint4,		"uint4" );
	GX_DECL_SCRIPT_TYPE( GXMath::ilong4,	"ilong4" );
	GX_DECL_SCRIPT_TYPE( GXMath::ulong4,	"ulong4" );
	GX_DECL_SCRIPT_TYPE( GXMath::float4,	"float4"  );
	GX_DECL_SCRIPT_TYPE( GXMath::double4,	"double4" );
	
	GX_DECL_SCRIPT_TYPE( GXMath::color4u,	"color4u" );
	GX_DECL_SCRIPT_TYPE( GXMath::color4f,	"color4f" );
	
	GX_DECL_SCRIPT_TYPE( GXTypes::String,	"String" );


}	// GXScript
}	// GX_STL
