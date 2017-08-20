// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Shaders/Pipeline/GL4PipelineCompiler.h"
#include "Engine/Shaders/Shader/ImageTypes.h"
#include "Engine/Platforms/OpenGL/Impl/gl4.h"

namespace ShaderEditor
{
namespace _ShaderNodesHidden_
{

	struct NodeGraph
	{
		static uint GetPassIndex (const ISrcNodePtr &ptr)	{ return ptr->_graphPassIndex; }
		static uint	GetRefCount (const ISrcNodePtr &ptr)	{ return ptr->_graphReferences; }
		static uint GetNodeIndex (const ISrcNodePtr &ptr)	{ return ptr->_nodeIndex; }

		static ISrcNode::Value_t const&	GetConst (const ISrcNodePtr &ptr)	{ return ptr->_value; }
	};

	extern const char* g_GLSLSharedShaderSource;

}	// _ShaderNodesHidden_


namespace PipelineNodes
{

	static GX_STL::GXTypes::String  ToLowerCase (GX_STL::GXTypes::StringCRef str)
	{
		GX_STL::GXTypes::String		result;
		
		result.Resize( str.Length(), '\0' );

		FOR( i, str ) {
			result[i] = GX_STL::GXTypes::StringUtils::ToLower( str[i] );
		}
		return result;
	}

/*
=================================================
	constructor
=================================================
*/
	GL4PipelineCompiler::GL4PipelineCompiler () :
		_initialized( false )
	{
		Engine::Platforms::RegisterPlatforms();
	}
	
/*
=================================================
	_Initialize
=================================================
*/
	void GL4PipelineCompiler::_Initialize ()
	{
		using namespace Engine;
		using namespace Engine::Base;
		using namespace Engine::Platforms;

		_bufferNodes.Clear();
		_imageNodes.Clear();
		_textureNodes.Clear();
		_funcSources.Clear();

		if ( _initialized )
			return;

		_initialized = true;

		Module *	ms = GetMainSystemInstace();

		ms->AddModule( "win.platform"_GModID, CreateInfo::Platform() );
		ms->AddModule( "input.mngr"_GModID, CreateInfo::InputManager() );
		ms->AddModule( "stream.mngr"_GModID, CreateInfo::StreamManager() );
		ms->AddModule( "gl4.ctx"_GModID, CreateInfo::GpuContext() );

		auto	thread	= ms->GlobalSystems()->Get< ParallelThread >();
	
		thread->AddModule( "win.window"_TModID, CreateInfo::Window() );
		thread->AddModule( "gl4.thrd"_TModID, CreateInfo::GpuThread() );

		auto	window		= thread->GetModule( "win.window"_TModID );
		auto	glthread	= thread->GetModule( "gl4.thrd"_TModID );

		//window->Subscribe( this, &GLApp::_OnWindowClosed );
		//glthread->Subscribe( this, &GLApp::_GLInit );
		//glthread->Subscribe( this, &GLApp::_GLDelete );

		window = null;
		thread = null;
		glthread = null;

		// finish initialization
		ms->Send( Message< ModuleMsg::Link >() );
		ms->Send( Message< ModuleMsg::Compose >() );
	}
	
/*
=================================================
	_Quit
=================================================
*/
	void GL4PipelineCompiler::_Quit ()
	{
	}

/*
=================================================
	Compile
=================================================
*/
	bool GL4PipelineCompiler::Compile (const Array<NodePtr> &allNodes, EShader::type shaderType, OUT ModulePtr &program)
	{
		using NodeGraph		= _ShaderNodesHidden_::NodeGraph;
		using StringMap		= GX_STL::GXTypes::Map< String, String >;

		String	hdr;
		String	src;

		_Initialize();

		CHECK_ERR( _CompilePass0( shaderType, OUT hdr ) );
		CHECK_ERR( _CompilePass1( allNodes, OUT src ) );
		CHECK_ERR( _CompilePass2( allNodes, OUT src ) );
		CHECK_ERR( _CompilePass3( OUT hdr ) );

		FOR( i, _imageNodes ) {
			hdr << _imageNodes[i].second.uniform << "  " << _imageNodes[i].second.name << ";\n";
		}

		FOR( i, _textureNodes ) {
			hdr << _textureNodes[i].second.uniform << "  " << _textureNodes[i].second.name << ";\n";
		}

		FOR( i, _bufferNodes ) {
			hdr << _bufferNodes[i].second.uniform << "  " << _bufferNodes[i].second.name << ";\n";
		}

		hdr << '\n';
		hdr >> src;

		LOG( src.cstr(), GX_STL::ELog::Info | GX_STL::ELog::OpenSpoilerFlag );

		CHECK_ERR( _CompileShaders( src ) );
		return true;
	}
	
/*
=================================================
	_CompilePass0
----
	generate preprocessor
=================================================
*/
	bool GL4PipelineCompiler::_CompilePass0 (EShader::type shaderType, OUT String &src)
	{
		CHECK_ERR( shaderType < EShader::_Count );

		src << "#version 450 core\n\n";

		// extensions
		
		src << "#extension GL_ARB_shading_language_420pack: enable\n";
		src << "#extension GL_ARB_gpu_shader_int64 : require\n";

		if ( shaderType == EShader::Compute )
		{
			src <<	"#extension GL_ARB_compute_variable_group_size : require\n"	// compilation failed if not supported
					"layout (local_size_variable) in;\n";
		}
		src << '\n';
		
		static const char *	perShaderSource[] = {
			"#define SHADER  SH_VERTEX\n",			// Vertex
			"#define SHADER  SH_TESS_CONTROL\n",	// TessControl
			"#define SHADER  SH_TESS_EVALUATION\n",	// TessEvaluation
			"#define SHADER  SH_GEOMETRY\n",		// Geometry
			"#define SHADER  SH_FRAGMENT\n",		// Fragment
			"#define SHADER  SH_COMPUTE\n"			// Compute
		};
		STATIC_ASSERT( GX_STL::GXTypes::CountOf(perShaderSource) == EShader::_Count );

		src << perShaderSource[ shaderType ]
			<< _ShaderNodesHidden_::g_GLSLSharedShaderSource
			<< '\n';
		return true;
	}

/*
=================================================
	_CompilePass1
----
	find images, buffers, textures and set binding indices
=================================================
*/
	bool GL4PipelineCompiler::_CompilePass1 (const Array<NodePtr> &allNodes, OUT String &src)
	{
		using NodeGraph		= _ShaderNodesHidden_::NodeGraph;

		TexturesMap	tex_names;

		FOR( i, allNodes )
		{
			NodePtr		node = allNodes[i];
			
			switch ( node->Type() )
			{
				case ENodeType::Buffer :
					_bufferNodes.Add( node, _bufferNodes.Count() );
					break;

				//case ENodeType::Image :
				//	_imageNodes.Add( node, _imageNodes.Count() );
				//	break;

				case ENodeType::Struct :
				{
					if ( node->Parent() and node->Parent()->Type() == ENodeType::Root )
					{
						NodePtr	image;
						NodePtr	sampler;

						FOR( j, node->Fields() )
						{
							NodePtr		field = node->Fields()[j];

							switch ( field->Type() )
							{
								case ENodeType::Image :		image	= field;	break;
								case ENodeType::Sampler :	sampler	= field;	break;
							}
						}

						if ( image and sampler )
							tex_names.Add({ image, sampler }, NodeInfo(~0u, "", node->Name()));
					}
					break;
				}

				case ENodeType::Function :
				{
					if ( node->TypeName().StartsWith( "GLSL.texture" ) )
					{
						NodePtr	image;
						NodePtr	sampler;

						FOR( j, node->Fields() )
						{
							NodePtr		field = node->Fields()[j];

							switch ( field->Type() )
							{
								case ENodeType::Image :		image	= field;	break;
								case ENodeType::Sampler :	sampler	= field;	break;
							}
						}

						CHECK_ERR( image and sampler );
						_textureNodes.Add({ image, sampler }, _textureNodes.Count() );
					}

					if ( node->TypeName().StartsWith( "GLSL.image" ) )
					{
						NodePtr	image;

						FOR( j, node->Fields() )
						{
							NodePtr		field = node->Fields()[j];

							if ( field->Type() == ENodeType::Image )
								image = field;
						}

						CHECK_ERR( image );
						_imageNodes.Add( image, _imageNodes.Count() );
					}
					break;
				}

				case ENodeType::Scalar :
				{
					if ( node->IsConst() )
					{
						const String	glsl_type = _VecTypeName( node );

						src << "\tconst " << glsl_type << " " << _NodeName( node )
							<< " = " << glsl_type << "(" << _GetConstValue( node ) << ");\n";
					}
					break;
				}
			}
		}

		FOR( i, _imageNodes ) {
			auto&	img = _imageNodes[i];

			img.second.name		= img.first->Name();
			_ImageTypeName( img.first, img.second.bindingIndex, OUT img.second.typeName, OUT img.second.uniform );
		}

		FOR( i, _bufferNodes ) {
			auto&	buf = _bufferNodes[i];

			buf.second.name		= buf.first->Name();
			_BufferTypeName( buf.first, buf.second.bindingIndex, OUT buf.second.typeName, OUT buf.second.uniform );
		}

		FOR( i, _textureNodes ) {
			auto&	tex = _textureNodes[i];

			_TextureTypeName( tex.first.Image(), tex.second.bindingIndex, OUT tex.second.typeName, OUT tex.second.uniform );

			TexturesMap::const_iterator	iter;
			CHECK_ERR( tex_names.Find( tex.first, OUT iter ) );

			tex.second.name	= iter->second.name;

			ASSERT( not tex.second.name.Empty() );
		}

		src << '\n';
		return true;
	}
	
/*
=================================================
	_CompilePass2
----
	generate source from nodes
=================================================
*/
	bool GL4PipelineCompiler::_CompilePass2 (const Array<NodePtr> &allNodes, OUT String &src)
	{
		"void main ()\n{\n" >> src;
		
		FOR_rev( i, allNodes )
		{
			NodePtr		node	= allNodes[i];
			
			// skip uniforms
			if ( node->Parent() and node->Parent()->Type() == ENodeType::Root )
				continue;

			switch ( node->Type() )
			{
				case ENodeType::Scalar :
					if ( node->IsConst() )
						break;

				case ENodeType::Vector :
				case ENodeType::Struct :
				{
					src << "\tconst " << _VecTypeName( node ) << " " << _NodeName( node ) << " = ";

					if ( node->Parent() and node->Parent()->Type() == ENodeType::Function )
					{
						src << _FuncArgs( node );
					}
					else
					{
						WARNING( "???" );
					}

					src << ";\n";
					break;
				}
				case ENodeType::Array :
					src << "\t// TODO: Array\n";
					break;

				//case ENodeType::Struct :
				//	src << "\t// TODO: Struct\n";
				//	break;

				case ENodeType::ShaderOutput :
				{
					src << '\t';

					if ( node->Parent() and node->Parent()->Type() == ENodeType::Function )
					{
						src << _FuncArgs( node );
					}
					else
					{
						WARNING( "???" );
					}

					src << ";\n";
					break;
				}
			}
		}

		src << "}\n\n";

		return true;
	}
	
/*
=================================================
	_CompilePass3
----
	parse functions sources to find dependencies
	TODO: optimize
=================================================
*/
	bool GL4PipelineCompiler::_CompilePass3 (OUT String &src)
	{
		using StringParser = GX_STL::GXTypes::StringParser;

		// find dependencies
		Array< StringCRef >		tokens;

		FOR( i, _funcSources )
		{
			for (usize pos = 0; _funcSources[i].Find( '$', pos, pos ); ++pos)
			{
				usize		p2 = 0;
				StringCRef	str;
				StringParser::ReadLineToEnd( _funcSources[i].SubString( pos ), p2, OUT str );

				StringParser::DivideString_CPP( str, OUT tokens );
				CHECK_ERR( tokens.Count() > 2 and tokens[0] == "$" );

				String	func_src;
				if ( _ShaderNodesHidden_::NodeFunctions::Instance()->GetSource( tokens[1], OUT func_src ) )
				{
					if ( not _funcSources.IsExist( func_src ) )
					{
						_funcSources.Add( func_src );
						i = -1;
						break;
					}
				}
			}
		}

		// write function headers only
		src << "// functions forward declarations\n";

		String	tmp;

		FOR( i, _funcSources )
		{
			usize	pos;
			CHECK_ERR( _funcSources[i].Find( '{', OUT pos ) );

			tmp = _funcSources[i].SubString( 0, pos );
			
			// minimize string
			while (tmp.FindAndDelete( "\n", OUT pos )) {}
			while (tmp.FindAndDelete( "\r", OUT pos )) {}
			while (tmp.FindAndChange( "\t", " ", OUT pos )) {}
			while (tmp.FindAndChange( "  ", " ", OUT pos )) {}
			if ( tmp.Back() == ' ' )	tmp.PopBack();

			src << tmp << ";\n";
		}

		// write full source
		src << "\n// functions sources\n";

		FOR( i, _funcSources )
		{
			// replace signatures
			tmp = _funcSources[i];

			for (usize pos = 0; tmp.Find( '$', pos, pos ); ++pos)
			{
				usize	p1 = ~0u;
				usize	p2 = ~0u;
				usize	p3 = ~0u;
				CHECK_ERR( tmp.Find( '(', OUT p1, pos ) and (tmp.Find( "_in", OUT p2, pos ) or tmp.Find( "_out", OUT p3, pos )) );

				p2 = GX_STL::GXMath::Min( p2, p3 );
				CHECK_ERR( p1 > p2 );

				tmp.Erase( pos, 1 );		// remove '$'
				tmp.Erase( p2-1, p1 - p2 );	// remove in/out args
			}

			src << tmp << "\n\n";
		}
		return true;
	}

/*
=================================================
	_VecTypeName
=================================================
*/
	GL4PipelineCompiler::String  GL4PipelineCompiler::_VecTypeName (const NodePtr &node) const
	{
		//CHECK_ERR( node and (node->Type() == ENodeType::Scalar or node->Type() == ENodeType::Vector) );
		
		return ToLowerCase( node->TypeName() );
	}

/*
=================================================
	_ImageTypeName
=================================================
*/
	bool GL4PipelineCompiler::_ImageTypeName (const NodePtr &imgNode, usize index, OUT String &typeName, OUT String &uniformName) const
	{
		CHECK_ERR( imgNode and imgNode->Type() == ENodeType::Image );

		Array<StringCRef>	tokens;
		GX_STL::GXTypes::StringParser::Tokenize( imgNode->TypeName(), '_', tokens );

		uniformName << "layout (binding=" << index;

		String	fmt = _GetImageFormat( imgNode );
		
		if ( fmt.Empty() )
		{
			LOG( (String("used default image format for '") << _NodeName( imgNode ) << "'").cstr(), GX_STL::ELog::Debug );

			// depends of _TypeName function
			if ( tokens[3].EqualsIC( "float4" ) )	fmt = "rgba32f";	else
			if ( tokens[3].EqualsIC( "float3" ) )	fmt = "rgb32f";		else
			if ( tokens[3].EqualsIC( "float2" ) )	fmt = "rg32f";		else
			if ( tokens[3].EqualsIC( "float" ) )	fmt = "r32f";		else
			if ( tokens[3].EqualsIC( "int4" ) )		fmt = "rgba32i";	else
			if ( tokens[3].EqualsIC( "int3" ) )		fmt = "rgb32i";		else
			if ( tokens[3].EqualsIC( "int2" ) )		fmt = "rg32i";		else
			if ( tokens[3].EqualsIC( "int" ) )		fmt = "r32i";		else
			if ( tokens[3].EqualsIC( "uint4" ) )	fmt = "rgba32ui";	else
			if ( tokens[3].EqualsIC( "uint3" ) )	fmt = "rgb32ui";	else
			if ( tokens[3].EqualsIC( "uint2" ) )	fmt = "rg32ui";		else
			if ( tokens[3].EqualsIC( "uint" ) )		fmt = "r32ui";		else
													RETURN_ERR( "unknown image format" );
		}

		uniformName << ", " << fmt << ") ";
		
		if ( tokens[2] == ShaderNodes::ReadOnly::Name() )				uniformName << "readonly";			else
		if ( tokens[2] == ShaderNodes::WriteOnly::Name() )				uniformName << "writeonly";			else
		if ( tokens[2] == ShaderNodes::Coherent::Name() )				uniformName << "coherent";			else
		if ( tokens[2] == ShaderNodes::Volatile::Name() )				uniformName << "volatile";			else
		if ( tokens[2] == ShaderNodes::Restrict::Name() )				uniformName << "restrict";

		uniformName << " uniform ";
		
		if ( tokens[3].StartsWithIC( "float" ) )						;									else
		if ( tokens[3].StartsWithIC( "int" ) )							uniformName << "i";					else
		if ( tokens[3].StartsWithIC( "uint" ) )							uniformName << "ui";

		if ( tokens[1] == ShaderNodes::ImageType1D::Name() )			uniformName << "image1D";			else
		if ( tokens[1] == ShaderNodes::ImageType1DArray::Name() )		uniformName << "image1DArray";		else
		if ( tokens[1] == ShaderNodes::ImageType2D::Name() )			uniformName << "image2D";			else
		if ( tokens[1] == ShaderNodes::ImageType2DArray::Name() )		uniformName << "image2DArray";		else
		if ( tokens[1] == ShaderNodes::ImageType2DMS::Name() )			uniformName << "image2DMS";			else
		if ( tokens[1] == ShaderNodes::ImageType2DMSArray::Name() )		uniformName << "image2DMSArray";	else
		if ( tokens[1] == ShaderNodes::ImageTypeCube::Name() )			uniformName << "imageCube";			else
		if ( tokens[1] == ShaderNodes::ImageTypeCubeArray::Name() )		uniformName << "imageCubeArray";	else
		if ( tokens[1] == ShaderNodes::ImageType3D::Name() )			uniformName << "image3D";
		
		usize	pos;
		typeName = uniformName;
		typeName.FindAndDelete( String("binding=") << index << ", ", pos );
		typeName.FindAndDelete( "uniform ", pos );

		return true;
	}
	
/*
=================================================
	_GetImageFormat
=================================================
*/
	GL4PipelineCompiler::String  GL4PipelineCompiler::_GetImageFormat (const NodePtr &node) const
	{
		CHECK_ERR( node and node->Type() == ENodeType::Image );

		using ImageUnit		= _ShaderNodesHidden_::ISrcNode::ImageUnit;
		using EPixelFormat	= Engine::Platforms::EPixelFormat;

		String	result;

		auto const&	value = _ShaderNodesHidden_::NodeGraph::GetConst( node );

		if ( not value.IsDefined() )
			return "";

		CHECK_ERR( value.Is< ImageUnit >() );

		const EPixelFormat::type	format = value.Get<ImageUnit>().format;

		CHECK_ERR( not EPixelFormat::IsCompressed( format ) );

		String	fmt_name	= ToLowerCase( EPixelFormat::ToString( format ) );
		usize	pos			= 0;

		CHECK_ERR( not fmt_name.Empty() );
		fmt_name.FindAndDelete( "_unorm", pos );

		if ( format == EPixelFormat::RGB_11_11_10F )
			fmt_name = "r11f_g11f_b10f";
		else
		if ( fmt_name.Back() == 'u' )
			fmt_name << 'i';

		return fmt_name;
	}

/*
=================================================
	_TextureTypeName
----
	TODO: sampler shadow support (depth + comparison)
=================================================
*/
	bool GL4PipelineCompiler::_TextureTypeName (const NodePtr &texNode, usize index, OUT String &typeName, OUT String &uniformName) const
	{
		CHECK_ERR( texNode and texNode->Type() == ENodeType::Image );

		Array<StringCRef>	tokens;
		GX_STL::GXTypes::StringParser::Tokenize( texNode->TypeName(), '_', tokens );

		CHECK_ERR( tokens.Count() == 4 and tokens[0].EqualsIC( "image" ) );
		CHECK_ERR( tokens[2] == ShaderNodes::ReadOnly::Name() );

		if ( tokens[3].StartsWithIC( "float" ) )						;								else
		if ( tokens[3].StartsWithIC( "int" ) )							typeName << "i";				else
		if ( tokens[3].StartsWithIC( "uint" ) )							typeName << "ui";

		if ( tokens[1] == ShaderNodes::ImageType1D::Name() )			typeName << "sampler1D";		else
		if ( tokens[1] == ShaderNodes::ImageType1DArray::Name() )		typeName << "sampler1DArray";	else
		if ( tokens[1] == ShaderNodes::ImageType2D::Name() )			typeName << "sampler2D";		else
		if ( tokens[1] == ShaderNodes::ImageType2DArray::Name() )		typeName << "sampler2DArray";	else
		if ( tokens[1] == ShaderNodes::ImageType2DMS::Name() )			typeName << "sampler2DMS";		else
		if ( tokens[1] == ShaderNodes::ImageType2DMSArray::Name() )		typeName << "sampler2DMSArray";	else
		if ( tokens[1] == ShaderNodes::ImageTypeCube::Name() )			typeName << "samplerCube";		else
		if ( tokens[1] == ShaderNodes::ImageTypeCubeArray::Name() )		typeName << "samplerCubeArray";	else
		if ( tokens[1] == ShaderNodes::ImageType3D::Name() )			typeName << "sampler3D";

		uniformName << "layout (binding=" << index << ")  uniform " << typeName;
		return true;
	}
	
/*
=================================================
	_BufferTypeName
=================================================
*/
	bool GL4PipelineCompiler::_BufferTypeName (const NodePtr &bufNode, usize index, OUT String &typeName, OUT String &uniformName) const
	{
		return "";
	}
	
/*
=================================================
	_NodeName
=================================================
*/
	GL4PipelineCompiler::String  GL4PipelineCompiler::_NodeName (const NodePtr &node) const
	{
		String	str;

		const auto	idx = _ShaderNodesHidden_::NodeGraph::GetNodeIndex( node );
		
		switch ( node->Type() )
		{
			case ENodeType::Scalar :
			case ENodeType::Vector :
			case ENodeType::Struct :
			{
				if ( idx > 0 )
				{
					str << "node_" << idx;
					break;
				}
				else
				if ( node->Parent() and not node->Name().Empty() )
				{
					const auto	idx2 = _ShaderNodesHidden_::NodeGraph::GetNodeIndex( node->Parent() );
					ASSERT( idx2 > 0 );

					str << "node_" << idx2 << '.' << node->Name();
					break;
				}
			}

			case ENodeType::Image :
			{
				NodeInfoMap::const_iterator	iter;
				CHECK_ERR( _imageNodes.Find( node, OUT iter ) );
				str << iter->second.name;
				break;
			}
			
			case ENodeType::Buffer :
			{
				NodeInfoMap::const_iterator	iter;
				CHECK_ERR( _bufferNodes.Find( node, OUT iter ) );
				str << iter->second.name;
				break;
			}

			default :
				RETURN_ERR( "not supported node type" );
		}

		return str;
	}
	
/*
=================================================
	_FuncArgs
=================================================
*/
	GL4PipelineCompiler::String  GL4PipelineCompiler::_FuncArgs (const NodePtr &result)
	{
		CHECK_ERR( result->Parent() and result->Parent()->Type() == ENodeType::Function );

		NodePtr	node	= result->Parent();
		usize	arg		= 0;
		String	str;
		
		if ( node->TypeName().StartsWithIC( "GLSL." ) )
		{
			str << node->TypeName().SubString( StringCRef("GLSL.").Length() ) << "( ";
		}
		else
			str << node->TypeName() << "( ";

		// special cases
		if ( node->TypeName().StartsWithIC( "GLSL.texture" ) )
		{
			NodePtr	image;
			NodePtr	sampler;

			FOR( j, node->Fields() )
			{
				NodePtr		field = node->Fields()[j];

				switch ( field->Type() )
				{
					case ENodeType::Image :		image = field;		break;
					case ENodeType::Sampler :	sampler = field;	break;
				}
			}

			CHECK_ERR( image and sampler );

			TexturesMap::const_iterator	iter;
			CHECK_ERR( _textureNodes.Find( { image, sampler }, OUT iter ) );
			
			FOR( j, node->Fields() )
			{
				NodePtr		field = node->Fields()[j];

				if ( field == result or
					 field == sampler or
					 field->Type() == ENodeType::ShaderOutput )
					continue;

				str << (arg ? ", " : "") << (field == image ? iter->second.name : _NodeName( field ));
				++arg;
			}
		}
		else
		{
			FOR( j, node->Fields() )
			{
				NodePtr		field = node->Fields()[j];

				if ( field == result or
					 field->Type() == ENodeType::ShaderOutput )
					continue;

				str << (arg ? ", " : "") << _NodeName( field );
				++arg;
			}
		}

		str << " )";

		// add function source
		if ( not node->TypeName().StartsWithIC( "GLSL." ) )
		{
			using FuncInfo = _ShaderNodesHidden_::ISrcNode::FuncInfo;

			String		func_src;
			String		signature	= node->TypeName();
			FuncInfo	info		= _ShaderNodesHidden_::NodeGraph::GetConst( node ).Get<FuncInfo>();

			CHECK_ERR( node->Fields().Count() );

			FOR( j, node->Fields() ) {
				signature << (j < info.inArgsCount ? "_in" : "_out") << node->Fields()[j]->TypeName();
			}

			if ( _ShaderNodesHidden_::NodeFunctions::Instance()->GetSource( signature, OUT func_src ) )
			{
				if ( not func_src.Empty() ) {
					_funcSources.Add( func_src );
				}
			}
		}
		return str;
	}

/*
=================================================
	_GetConstValue
=================================================
*/
	struct _UnionValueToString
	{
		using String	= GX_STL::GXTypes::String;

		mutable String	result;

		template <typename T>
		void operator () (const T &val) const
		{
			result = val;
		}

		void operator () (const _ShaderNodesHidden_::ISrcNode::ImageUnit &val) const
		{
			WARNING( "invalid const value" );
		}

		void operator () (const _ShaderNodesHidden_::ISrcNode::SamplerUnit &val) const
		{
			WARNING( "invalid const value" );
		}

		void operator () (const _ShaderNodesHidden_::ISrcNode::BufferUnit &val) const
		{
			WARNING( "invalid const value" );
		}

		void operator () (const GX_STL::GXTypes::String &val) const
		{
			WARNING( "invalid const value" );
		}

		void operator () (const _ShaderNodesHidden_::ISrcNode::FuncInfo &val) const
		{
			WARNING( "invalid const value" );
		}
	};

	GL4PipelineCompiler::String  GL4PipelineCompiler::_GetConstValue (const NodePtr &node) const
	{
		auto const&	value = _ShaderNodesHidden_::NodeGraph::GetConst( node );

		_UnionValueToString		to_str;

		value.Apply( to_str );
		return to_str.result;
	}
	
/*
=================================================
	_CompileGLShader
=================================================
*/
	GL4PipelineCompiler::GLuint  GL4PipelineCompiler::_CompileGLShader (StringCRef shaderSrc) const
	{
		using namespace gl;
		using namespace GX_STL;
		using namespace GX_STL::GXTypes;

		const char *	src[] = { shaderSrc.cstr() };

		GLuint	shader;
		GL_CALL( shader = glCreateShader( GL_COMPUTE_SHADER ) );
		GL_CALL( glShaderSource( shader, 1, src, null ) );
		GL_CALL( glCompileShader( shader ) );
		
		GLint	compile_status = 0;
		GL_CALL( glGetShaderiv( shader, GL_COMPILE_STATUS, &compile_status ) );
		
		GLint	log_size = 0;
		GL_CALL( glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &log_size ) );

		if ( log_size > 5 )
		{
			String	info;
			info.Reserve( log_size+1 );
			
			GL_CALL( glGetShaderInfoLog( shader, log_size, null, info.ptr() ) );
			info.CalculateLength();
			
			_ParseCompilationErrors( shaderSrc, info, (compile_status == GL_TRUE) );
		}
		return shader;
	}
	
/*
=================================================
	LinkGLProgram
=================================================
*/
	static gl::GLuint LinkGLProgram (GX_STL::GXTypes::ArrayCRef< gl::GLuint > shaders)
	{
		using namespace gl;
		using namespace GX_STL;
		using namespace GX_STL::GXTypes;

		GLuint	prog;
		GL_CALL( prog = glCreateProgram() );

		FOR( i, shaders ) {
			GL_CALL( glAttachShader( prog, shaders[i] ) );
		}

		GL_CALL( glLinkProgram( prog ) );

		FOR( i, shaders ) {
			GL_CALL( glDeleteShader( shaders[i] ) );
		}

		GLint	link_status = 0;
		GL_CALL( glGetProgramiv( prog, GL_LINK_STATUS, &link_status ) );
		
		GLint	log_size = 0;
		GL_CALL( glGetProgramiv( prog, GL_INFO_LOG_LENGTH, &log_size ) );

		if ( log_size > 5 )
		{
			String	info;
			info.Reserve( log_size + 1 );

			GL_CALL( glGetProgramInfoLog( prog, log_size, null, info.ptr() ) );
			info.CalculateLength();
			
			LOG( info.cstr(), ((link_status == GL_TRUE) ? ELog::Debug : ELog::Error) | ELog::OpenSpoilerFlag );
		}
		return prog;
	}
	
/*
=================================================
	_CompileShaders
=================================================
*/
	bool GL4PipelineCompiler::_CompileShaders (StringCRef compute)
	{
		LinkGLProgram({ _CompileGLShader( compute ) });


		return true;
	}

/*
=================================================
	_CompileShaders
=================================================
*/
	bool GL4PipelineCompiler::_CompileShaders (StringCRef vertex, StringCRef tessControl, StringCRef tessEval,
												StringCRef geometry, StringCRef fragment)
	{
		return true;
	}
	
/*
=================================================
	_ParseCompilationErrors
=================================================
*/
	void GL4PipelineCompiler::_ParseCompilationErrors (StringCRef source, StringCRef log, bool compiled) const
	{
		using namespace GX_STL;
		using namespace GX_STL::GXTypes;
		using namespace Engine::Platforms;

		// pattern:	<number> ( <line> ) : <error/warning> <code>: <description>

		Array< StringCRef >		lines;
		Array< StringCRef >		tokens;
		String					str;		str.Reserve( log.Length() * 2 );
		usize					prev_line = -1;
		const usize				num_lines	= StringParser::CalculateNumberOfLines( source );

		str << EShader::ToString( /*TODO*/ EShader::Compute ) << " shader compilation "
			<< (compiled ? "message" : "error") << "\n---------------\n";

		StringParser::DivideLines( log, OUT lines );

		FOR( i, lines )
		{
			StringParser::DivideString_CPP( lines[i], OUT tokens );

			if ( tokens.Count() > 8 and
				 tokens[1] == "(" and
				 tokens[3] == ")" and
				 tokens[4] == ":" )
			{
				const usize	line		= (usize) StringUtils::ToInt32( tokens[2] );
				const bool	is_err		= tokens[5].EqualsIC( "error" );
				usize		pos			= 0;
				StringCRef	line_str;

				// unite error in same source lines
				if ( prev_line == line )
				{
					str << lines[i] << "\n";
					continue;
				}

				prev_line = line;

				if ( line >= num_lines )
				{
					str << "(invalid line number)\n";
					continue;
				}

				CHECK( StringParser::MoveToLine( source, INOUT pos, line-1 ) );

				StringParser::ReadLineToEnd( source, INOUT pos, OUT line_str );

				str << "in (" << line << "):\n\"" << line_str << "\"\n" << lines[i] << "\n";
			}
			else
			{
				str << "<unknown message> " << lines[i] << "\n";
			}
		}

		LOG( str.cstr(), compiled ? (ELog::Debug | ELog::SpoilerFlag) : (ELog::Error | ELog::OpenSpoilerFlag) );
	}


}	// PipelineNodes
}	// ShaderEditor
