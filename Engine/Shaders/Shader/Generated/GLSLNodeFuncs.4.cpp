// This is generated file, don't change anything!

#include "GLSLNodeFuncs.Shared.h"

namespace ShaderEditor
{
namespace GLSLShaderNodes
{

	// ImageLoad, ImageStore

/*
=================================================
	ImageLoad
=================================================
*/
	struct ImageLoad_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		ImageLoad_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and
				 in.Count() >= 2 and
				 in[0].GetValueTypeId() == TypeIdOf<T>() and								// image
				 in[1].GetValueTypeId() == TypeIdOf< typename T::IntCoord_t >() and			// point
				 out.GetValueTypeId() == TypeIdOf< typename T::Pixel_t >() and				// pixel
				 (in.Count() == 2 or
					(in.Count() == 3 and in[2].GetValueTypeId() == TypeIdOf<Int>())) and	// sampler
				 T::Access_t::readAccess )
			{
				if ( in.Count() == 2 )
				{
					FunctionNode	func( "GLSL.imageLoad", 1, in[0].Get<T>(), in[1].Get< typename T::IntCoord_t >() );
					Replace( out.Get< typename T::Pixel_t >(), "ImageLoad.out", func );
					result = true;
				}
				else
				if ( in.Count() == 3 and
					 in[2].GetValueTypeId() == TypeIdOf<Int>() )
				{
					FunctionNode	func( "GLSL.imageLoad", 1, in[0].Get<T>(), in[1].Get< typename T::IntCoord_t >(), in[2].Get<Int>() );
					Replace( out.Get< typename T::Pixel_t >(), "ImageLoad.out", func );
					result = true;
				}
			}
		}
	};

	static bool ImageLoad_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 2 and in.Count() <= 4 and out.Count() == 1 );

		ImageLoad_Func	func( in, out.Front() );
		Helpers::ImageTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool ImageLoad_Source (StringCRef sig, OUT String &src)
	{
		// no source
		ASSERT( sig.StartsWith( "GLSL.imageLoad" ) );
		return true;
	}

/*
=================================================
	ImageStore
=================================================
*/
	struct ImageStore_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		ImageStore_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and
				 in.Count() >= 3 and
				 in[0].GetValueTypeId() == TypeIdOf<T>() and											// image
				 in[1].GetValueTypeId() == TypeIdOf< typename T::IntCoord_t >() and						// point
				 ((in.Count() == 3 and in[2].GetValueTypeId() == TypeIdOf< typename T::Pixel_t >()) or	// data
					(in.Count() == 4 and in[2].GetValueTypeId() == TypeIdOf< Int >() and				// sampler
					 in[3].GetValueTypeId() == TypeIdOf< typename T::Pixel_t >())) and					// data
				 out.GetValueTypeId() == TypeIdOf< ShaderOutputNode >() and								// result
				 T::Access_t::writeAccess )
			{
				if ( in.Count() == 3 )
				{
					FunctionNode	func( "GLSL.imageStore", 1, in[0].Get<T>(), in[1].Get< typename T::IntCoord_t >(), in[2].Get< typename T::Pixel_t >() );
					Replace( out.Get< ShaderOutputNode >(), "ImageStore.out", func );
					result = true;
				}
				else
				if ( in.Count() == 4 and
					 in[2].GetValueTypeId() == TypeIdOf<Int>() )
				{
					FunctionNode	func( "GLSL.imageStore", 1, in[0].Get<T>(), in[1].Get< typename T::IntCoord_t >(), in[2].Get<Int>(), in[3].Get< typename T::Pixel_t >() );
					Replace( out.Get< ShaderOutputNode >(), "ImageStore.out", func );
					result = true;
				}
			}
		}
	};

	static bool ImageStore_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() >= 3 and in.Count() <= 4 and out.Count() == 1 );

		ImageStore_Func	func( in, out.Front() );
		Helpers::ImageTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool ImageStore_Source (StringCRef sig, OUT String &src)
	{
		// no source
		ASSERT( sig.StartsWith( "GLSL.imageStore" ) );
		return true;
	}


}	// GLSLShaderNodes

namespace _ShaderNodesHidden_
{
	void RegisterGLSLShaderNodes4 (NodeFunctions *nf)
	{
		nf->Register( "GL4.ImageLoad", GLSLShaderNodes::ImageLoad_Node, &GLSLShaderNodes::ImageLoad_Source );
		nf->Register( "GL4.ImageStore", GLSLShaderNodes::ImageStore_Node, &GLSLShaderNodes::ImageStore_Source );
	}


}	// _ShaderNodesHidden_
}	// ShaderEditor
