// This is generated file, don't change anything!

#include "GLSLNodeFuncs.Shared.h"

namespace ShaderEditor
{
namespace GLSLShaderNodes
{

	// Type casts

/*
=================================================
	TypeCtor
=================================================
*/
	struct TypeCtor_Func
	{
		FunctionNode&			func;
		VariantCRef				in;
		bool					found { false };

		TypeCtor_Func (FunctionNode &func, VariantCRef in) : func(func), in(in)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not found and in.GetValueTypeId() == TypeIdOf<T>() )
			{
				func.AddField( in.Get<T>() );
				found = true;
			}
		}
	};

	static bool BoolCtor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.bool", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Bool>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Bool>(), "BoolCtor.out", func );
		return true;
	}

	static bool BoolCtor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool Bool2Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.bool2", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Bool2>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Bool2>(), "Bool2Ctor.out", func );
		return true;
	}

	static bool Bool2Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool Bool3Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.bool3", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Bool3>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Bool3>(), "Bool3Ctor.out", func );
		return true;
	}

	static bool Bool3Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool Bool4Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.bool4", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Bool4>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Bool4>(), "Bool4Ctor.out", func );
		return true;
	}

	static bool Bool4Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool IntCtor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.int", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Int>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Int>(), "IntCtor.out", func );
		return true;
	}

	static bool IntCtor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool Int2Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.int2", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Int2>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Int2>(), "Int2Ctor.out", func );
		return true;
	}

	static bool Int2Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool Int3Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.int3", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Int3>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Int3>(), "Int3Ctor.out", func );
		return true;
	}

	static bool Int3Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool Int4Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.int4", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Int4>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Int4>(), "Int4Ctor.out", func );
		return true;
	}

	static bool Int4Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool UIntCtor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.uint", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<UInt>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<UInt>(), "UIntCtor.out", func );
		return true;
	}

	static bool UIntCtor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool UInt2Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.uint2", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<UInt2>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<UInt2>(), "UInt2Ctor.out", func );
		return true;
	}

	static bool UInt2Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool UInt3Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.uint3", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<UInt3>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<UInt3>(), "UInt3Ctor.out", func );
		return true;
	}

	static bool UInt3Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool UInt4Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.uint4", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<UInt4>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<UInt4>(), "UInt4Ctor.out", func );
		return true;
	}

	static bool UInt4Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool ILongCtor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.ilong", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<ILong>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<ILong>(), "ILongCtor.out", func );
		return true;
	}

	static bool ILongCtor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool ILong2Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.ilong2", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<ILong2>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<ILong2>(), "ILong2Ctor.out", func );
		return true;
	}

	static bool ILong2Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool ILong3Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.ilong3", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<ILong3>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<ILong3>(), "ILong3Ctor.out", func );
		return true;
	}

	static bool ILong3Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool ILong4Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.ilong4", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<ILong4>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<ILong4>(), "ILong4Ctor.out", func );
		return true;
	}

	static bool ILong4Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool ULongCtor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.ulong", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<ULong>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<ULong>(), "ULongCtor.out", func );
		return true;
	}

	static bool ULongCtor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool ULong2Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.ulong2", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<ULong2>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<ULong2>(), "ULong2Ctor.out", func );
		return true;
	}

	static bool ULong2Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool ULong3Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.ulong3", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<ULong3>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<ULong3>(), "ULong3Ctor.out", func );
		return true;
	}

	static bool ULong3Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool ULong4Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.ulong4", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<ULong4>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<ULong4>(), "ULong4Ctor.out", func );
		return true;
	}

	static bool ULong4Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool FloatCtor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.float", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Float>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Float>(), "FloatCtor.out", func );
		return true;
	}

	static bool FloatCtor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool Float2Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.float2", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Float2>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Float2>(), "Float2Ctor.out", func );
		return true;
	}

	static bool Float2Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool Float3Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.float3", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Float3>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Float3>(), "Float3Ctor.out", func );
		return true;
	}

	static bool Float3Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool Float4Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.float4", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Float4>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Float4>(), "Float4Ctor.out", func );
		return true;
	}

	static bool Float4Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool DoubleCtor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.double", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Double>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Double>(), "DoubleCtor.out", func );
		return true;
	}

	static bool DoubleCtor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool Double2Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.double2", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Double2>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Double2>(), "Double2Ctor.out", func );
		return true;
	}

	static bool Double2Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool Double3Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.double3", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Double3>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Double3>(), "Double3Ctor.out", func );
		return true;
	}

	static bool Double3Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}

	static bool Double4Ctor_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 1 and in.Count() <= 4 and out.Count() == 1 );
		
		FunctionNode	func( "GLSL.double4", in.Count(), out.Count() );
		CHECK_ERR( TypeIdOf<Double4>() == out.Front().GetValueTypeId() );
		
		FOR( i, in ) {
			TypeCtor_Func	tlist_func( func, in[i] );
			Helpers::AllTypes::RuntimeForEach( tlist_func );
			CHECK_ERR( tlist_func.found );
		}

		Replace( out.Front().Get<Double4>(), "Double4Ctor.out", func );
		return true;
	}

	static bool Double4Ctor_Source (StringCRef sig, OUT String &src)
	{
		// no source
		return true;
	}


}	// GLSLShaderNodes

namespace _ShaderNodesHidden_
{
	void RegisterGLSLShaderNodes1 (NodeFunctions *nf)
	{
		nf->Register( "GL4.BoolCtor", GLSLShaderNodes::BoolCtor_Node, &GLSLShaderNodes::BoolCtor_Source );
		nf->Register( "GL4.Bool2Ctor", GLSLShaderNodes::Bool2Ctor_Node, &GLSLShaderNodes::Bool2Ctor_Source );
		nf->Register( "GL4.Bool3Ctor", GLSLShaderNodes::Bool3Ctor_Node, &GLSLShaderNodes::Bool3Ctor_Source );
		nf->Register( "GL4.Bool4Ctor", GLSLShaderNodes::Bool4Ctor_Node, &GLSLShaderNodes::Bool4Ctor_Source );
		nf->Register( "GL4.IntCtor", GLSLShaderNodes::IntCtor_Node, &GLSLShaderNodes::IntCtor_Source );
		nf->Register( "GL4.Int2Ctor", GLSLShaderNodes::Int2Ctor_Node, &GLSLShaderNodes::Int2Ctor_Source );
		nf->Register( "GL4.Int3Ctor", GLSLShaderNodes::Int3Ctor_Node, &GLSLShaderNodes::Int3Ctor_Source );
		nf->Register( "GL4.Int4Ctor", GLSLShaderNodes::Int4Ctor_Node, &GLSLShaderNodes::Int4Ctor_Source );
		nf->Register( "GL4.UIntCtor", GLSLShaderNodes::UIntCtor_Node, &GLSLShaderNodes::UIntCtor_Source );
		nf->Register( "GL4.UInt2Ctor", GLSLShaderNodes::UInt2Ctor_Node, &GLSLShaderNodes::UInt2Ctor_Source );
		nf->Register( "GL4.UInt3Ctor", GLSLShaderNodes::UInt3Ctor_Node, &GLSLShaderNodes::UInt3Ctor_Source );
		nf->Register( "GL4.UInt4Ctor", GLSLShaderNodes::UInt4Ctor_Node, &GLSLShaderNodes::UInt4Ctor_Source );
		nf->Register( "GL4.ILongCtor", GLSLShaderNodes::ILongCtor_Node, &GLSLShaderNodes::ILongCtor_Source );
		nf->Register( "GL4.ILong2Ctor", GLSLShaderNodes::ILong2Ctor_Node, &GLSLShaderNodes::ILong2Ctor_Source );
		nf->Register( "GL4.ILong3Ctor", GLSLShaderNodes::ILong3Ctor_Node, &GLSLShaderNodes::ILong3Ctor_Source );
		nf->Register( "GL4.ILong4Ctor", GLSLShaderNodes::ILong4Ctor_Node, &GLSLShaderNodes::ILong4Ctor_Source );
		nf->Register( "GL4.ULongCtor", GLSLShaderNodes::ULongCtor_Node, &GLSLShaderNodes::ULongCtor_Source );
		nf->Register( "GL4.ULong2Ctor", GLSLShaderNodes::ULong2Ctor_Node, &GLSLShaderNodes::ULong2Ctor_Source );
		nf->Register( "GL4.ULong3Ctor", GLSLShaderNodes::ULong3Ctor_Node, &GLSLShaderNodes::ULong3Ctor_Source );
		nf->Register( "GL4.ULong4Ctor", GLSLShaderNodes::ULong4Ctor_Node, &GLSLShaderNodes::ULong4Ctor_Source );
		nf->Register( "GL4.FloatCtor", GLSLShaderNodes::FloatCtor_Node, &GLSLShaderNodes::FloatCtor_Source );
		nf->Register( "GL4.Float2Ctor", GLSLShaderNodes::Float2Ctor_Node, &GLSLShaderNodes::Float2Ctor_Source );
		nf->Register( "GL4.Float3Ctor", GLSLShaderNodes::Float3Ctor_Node, &GLSLShaderNodes::Float3Ctor_Source );
		nf->Register( "GL4.Float4Ctor", GLSLShaderNodes::Float4Ctor_Node, &GLSLShaderNodes::Float4Ctor_Source );
		nf->Register( "GL4.DoubleCtor", GLSLShaderNodes::DoubleCtor_Node, &GLSLShaderNodes::DoubleCtor_Source );
		nf->Register( "GL4.Double2Ctor", GLSLShaderNodes::Double2Ctor_Node, &GLSLShaderNodes::Double2Ctor_Source );
		nf->Register( "GL4.Double3Ctor", GLSLShaderNodes::Double3Ctor_Node, &GLSLShaderNodes::Double3Ctor_Source );
		nf->Register( "GL4.Double4Ctor", GLSLShaderNodes::Double4Ctor_Node, &GLSLShaderNodes::Double4Ctor_Source );
	}


}	// _ShaderNodesHidden_
}	// ShaderEditor
