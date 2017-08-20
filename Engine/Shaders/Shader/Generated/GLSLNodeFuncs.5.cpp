// This is generated file, don't change anything!

#include "GLSLNodeFuncs.Shared.h"

namespace ShaderEditor
{
namespace GLSLShaderNodes
{

	// TextureSample, TextureSampleOffset

/*
=================================================
	TextureSample
=================================================
*/
	struct TextureSample_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		TextureSample_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and
				 in[0].GetValueTypeId() == TypeIdOf<T>() and								// image
				 in[1].GetValueTypeId() == TypeIdOf< Sampler >() and						// sampler
				 in[2].GetValueTypeId() == TypeIdOf< typename T::FpCoord_t >() and			// point
				 out.GetValueTypeId() == TypeIdOf< typename T::Pixel_t >() and				// result
				 not T::Image_t::Multisample and
				 T::Access_t::readAccess )
			{
				FunctionNode	func( "GLSL.texture", 1, in[0].Get<T>(), in[1].Get< Sampler >(), in[2].Get< typename T::FpCoord_t >() );
				Replace( out.Get< typename T::Pixel_t >(), "TextureSample.out", func );
				result = true;
			}
		}
	};

	static bool TextureSample_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 3 and out.Count() == 1 );

		TextureSample_Func	func( in, out.Front() );
		Helpers::ImageTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool TextureSample_Source (StringCRef sig, OUT String &src)
	{
		// no source
		ASSERT( sig.StartsWith( "GLSL.texture" ) );
		return true;
	}

/*
=================================================
	TextureSampleOffset
=================================================
*/
	struct TextureSampleOffset_Func
	{
		ArrayCRef<VariantCRef>	in;
		VariantRef				out;
		bool					result { false };

		TextureSampleOffset_Func (ArrayCRef<VariantCRef> in, VariantRef out) : in(in), out(out)
		{}

		template <typename T, uint Index>
		void Process ()
		{
			if ( not result and
				 in[0].GetValueTypeId() == TypeIdOf<T>() and								// image
				 in[1].GetValueTypeId() == TypeIdOf< Sampler >() and						// sampler
				 in[2].GetValueTypeId() == TypeIdOf< typename T::FpCoord_t >() and			// point
				 in[3].GetValueTypeId() == TypeIdOf< typename T::IntCoord_t >() and			// offset
				 out.GetValueTypeId() == TypeIdOf< typename T::Pixel_t >() and				// result
				 not T::Image_t::Multisample and
				 T::Access_t::readAccess )
			{
				FunctionNode	func( "GLSL.textureOffset", 1, in[0].Get<T>(), in[1].Get< Sampler >(), in[2].Get< typename T::FpCoord_t >(), in[3].Get< typename T::IntCoord_t >() );
				Replace( out.Get< typename T::Pixel_t >(), "TextureSampleOffset.out", func );
				result = true;
			}
		}
	};

	static bool TextureSampleOffset_Node (ArrayCRef<VariantCRef> in, ArrayRef<VariantRef> out)
	{
		CHECK_ERR( in.Count() == 4 and out.Count() == 1 );

		TextureSampleOffset_Func	func( in, out.Front() );
		Helpers::ImageTypes::RuntimeForEach( func );

		if ( func.result )
			return true;

		RETURN_ERR( "unsupported function arguments" );
	}

	static bool TextureSampleOffset_Source (StringCRef sig, OUT String &src)
	{
		// no source
		ASSERT( sig.StartsWith( "GLSL.textureOffset" ) );
		return true;
	}


}	// GLSLShaderNodes

namespace _ShaderNodesHidden_
{
	void RegisterGLSLShaderNodes5 (NodeFunctions *nf)
	{
		nf->Register( "GL4.TextureSample", GLSLShaderNodes::TextureSample_Node, &GLSLShaderNodes::TextureSample_Source );
		nf->Register( "GL4.TextureSampleOffset", GLSLShaderNodes::TextureSampleOffset_Node, &GLSLShaderNodes::TextureSampleOffset_Source );
	}


}	// _ShaderNodesHidden_
}	// ShaderEditor
