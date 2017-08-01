// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Shaders/Engine.Shaders.h"

using namespace ShaderEditor::ShaderNodes;
using namespace ShaderEditor::PipelineNodes;
	
	
template <template <typename> class AccessType, typename PixelType>
using Image2D	= Image< ImageType2D, AccessType, PixelType >;

template <typename PixelType>
using Texture2D	= Texture< ImageType2D, PixelType >;


#define DECL_STRUCT( _typeName_ ) \
		_typeName_ (Node *parent, StringCRef name) : \
			Node( parent, name, #_typeName_, ENodeType::Struct ) \
		{}


struct MyPipeline : ComputePipeline
{
public:
	struct Constants : Node
	{
		Float2		scale		{ this, "scale" };
		Float2		translate	{ this, "translate" };
		
		DECL_STRUCT( Constants )
	};

	struct Shared : Node
	{
		Array< Float2, 16 >		elements	{ this, "elements" };

		DECL_STRUCT( Shared )
	};

	struct MyComputeShader : ComputeShader
	{
	public:
		Texture2D< Float4 >				diffuseMap	{ this, "diffuseMap" };

		Image2D< ReadOnly, Float4 >		image0		{ this, "image0" };
		Image2D< Coherent, Float4 >		outImage1	{ this, "outImage1" };
		Buffer< Coherent, Float >		outBuf		{ this, "outBuf" };

		//PushConstants< Constants >		constants	{ this, "constants" };

		MyComputeShader ()
		{}

	private:
		LocalShared< Shared >			shared		{ this, "shared" };

		void Main () const override
		{
			Float	constColor{ 1.0f };

			Float4	color0 = diffuseMap.Sample( Float2Ctor( Float3Ctor(cs_localCoord) / Float3Ctor(cs_localCoordSize) ) );
			Float4	color1 = image0.Load( Int2Ctor(cs_localCoord) );
			//Float4	color2 = color0;

			//constants->scale = color0.xy;

			//outBuf.Write( index, color0.x );
			outImage1.Store( Int2Ctor(cs_localCoord),
							 Float4Ctor( Float2Ctor(color0).xy + Float2Ctor(color1).xy, color0.z + color1.z, constColor ) );
		}
	};

	
public:
	MyPipeline ()
	{
		compute._Compile();
	}


	MyComputeShader	compute;
};



void main ()
{
	using namespace GX_STL::GXTypes;
	using namespace GX_STL::GXMath;
	using namespace Engine::Platforms;

	InitializeSTLMath();

	Logger::GetInstance()->Open( "log", false );
	ShaderEditor::_ShaderNodesHidden_::NodeFunctions::Instance()->BindAPI( "GL4" );

	{
		ModulePtr	diffuseTex;
		ModulePtr	linearSampler;
		ModulePtr	img1;
		ModulePtr	img2;
		ModulePtr	buf1;

		MyPipeline	prog;

		prog.compute.diffuseMap.image	= diffuseTex;
		prog.compute.diffuseMap.sampler	= linearSampler;

		prog.compute.image0		= img1;
		prog.compute.outImage1	= img2;
		prog.compute.outBuf		= buf1;

		//prog.compute.constants->scale	= float2(1.0f);

		prog.Run( uint3(1024, 1024, 0) );
	}

	ShaderEditor::_ShaderNodesHidden_::NodeFunctions::Instance()->UnregisterAll();
	ShaderEditor::_ShaderNodesHidden_::NodeFunctions::Instance()->UnregisterCompilers();
	
	WARNING( "Tests Finished!" );

	Logger::GetInstance()->Close();
	
	DEBUG_ONLY( RefCountedObject::s_ChenckNotReleasedObjects() );
}
