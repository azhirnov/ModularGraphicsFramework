// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Public/GPU/Enums.ToString.h"
#include "Core/STL/Algorithms/StringParser.h"

namespace Engine
{
namespace Platforms
{
	
/*
=================================================
	constructor
=================================================
*/
	GraphicsPipelineDescription::GraphicsPipelineDescription (UninitializedType) :
		patchControlPoints{0}, subpass{0}, earlyFragmentTests{true}
	{}
	
	GraphicsPipelineDescription::GraphicsPipelineDescription  (const VertexInputState &		vertexInput,
																const RenderState &			renderState,
																EPipelineDynamicState::bits	dynamicStates,
																const FragmentOutputState &	fragOutput,
																bool						earlyFragmentTests,
																uint						patchControlPoints,
																uint						subpass) :
		vertexInput(vertexInput),					renderState(renderState),
		dynamicStates(dynamicStates),				fragOutput(fragOutput),
		patchControlPoints(patchControlPoints),		subpass(subpass),
		earlyFragmentTests{earlyFragmentTests}
	{}
	
/*
=================================================
	ToString
=================================================
*/
	DEBUG_ONLY(
	String GraphicsPipelineDescription::ToString () const
	{
		String	str;
		String	temp;
		
		temp = vertexInput.ToString();
		StringParser::IncreaceIndent( INOUT temp, "\t\t\t" );

		str << "GraphicsPipeline {"
			<< "\n	vertexInput:\n"<< temp;
		
		temp = renderState.ToString();
		StringParser::IncreaceIndent( INOUT temp, "\t\t\t" );

		str	<< "\n	renderState:\n" << temp;
		
		temp = fragOutput.ToString();
		StringParser::IncreaceIndent( INOUT temp, "\t\t\t" );

		str << "\n	fragOutput:\n" << temp;

		str	<< "\n	dynamicStates:  " << EPipelineDynamicState::ToString( dynamicStates )
			<< ""
			<< "\n}";
		return str;
	})
//-----------------------------------------------------------------------------
	


/*
=================================================
	constructor
=================================================
*/
	ComputePipelineDescription::ComputePipelineDescription (UninitializedType)
	{}

	ComputePipelineDescription::ComputePipelineDescription (const uint3 &localGroupSize) :
		localGroupSize(localGroupSize)
	{}
//-----------------------------------------------------------------------------
	


/*
=================================================
	constructor
=================================================
*/
	PipelineTemplateDescription::PipelineTemplateDescription (UninitializedType) :
		patchControlPoints(0), subpass(0)
	{
	}
	
/*
=================================================
	AddSource
=================================================
*/
	void PipelineTemplateDescription::ShaderSource::AddSource (EShaderLangFormat::type fmt, String &&src)
	{
		ASSERT( EShaderLangFormat::GetStorage( fmt ) == EShaderLangFormat::Source );
		ASSERT( not _sources.IsExist( fmt ) );

		_sources.Add( fmt, Data_t{RVREF( src )} );
	}
	
/*
=================================================
	AddBinary
=================================================
*/
	void PipelineTemplateDescription::ShaderSource::AddBinary (EShaderLangFormat::type fmt, BinArrayCRef bin)
	{
		ASSERT( EShaderLangFormat::GetStorage( fmt ) == EShaderLangFormat::Binary );
		ASSERT( not _sources.IsExist( fmt ) );

		_sources.Add( fmt, Data_t{BinaryArray( bin )} );
	}
	
/*
=================================================
	AddSpirv
=================================================
*/
	void PipelineTemplateDescription::ShaderSource::AddSpirv (EShaderLangFormat::type fmt, ArrayCRef<uint> bin)
	{
		ASSERT( EShaderLangFormat::GetStorage( fmt ) == EShaderLangFormat::Binary );
		ASSERT( not _sources.IsExist( fmt ) );

		_sources.Add( fmt, Data_t{Array<uint>( bin )} );
	}
	
/*
=================================================
	AddInvocable
=================================================
*/
	void PipelineTemplateDescription::ShaderSource::AddInvocable (EShaderLangFormat::type fmt, SWInvoke_t ptr)
	{
		ASSERT( EShaderLangFormat::GetStorage( fmt ) == EShaderLangFormat::Executable );
		ASSERT( not _sources.IsExist( fmt ) );

		_sources.Add( fmt, Data_t{ ptr });
	}
	
/*
=================================================
	AddSource
=================================================
*/
	bool PipelineTemplateDescription::ShaderSource::AddSource (EShaderLangFormat::type fmt, const RFilePtr &file)
	{
		ASSERT( EShaderLangFormat::GetStorage( fmt ) == EShaderLangFormat::Source );
		ASSERT( not _sources.IsExist( fmt ) );
		CHECK_ERR( file );
		
		const usize	len		= usize(file->RemainingSize());
		String		str;	str.Resize( len );

		CHECK_ERR( file->Read( str.ptr(), str.LengthInBytes() ) );
		
		_sources.Add( fmt, Data_t{RVREF( str )} );
		return true;
	}
	
/*
=================================================
	AddBinary
=================================================
*/
	bool PipelineTemplateDescription::ShaderSource::AddBinary (EShaderLangFormat::type fmt, const RFilePtr &file)
	{
		ASSERT( EShaderLangFormat::GetStorage( fmt ) == EShaderLangFormat::Binary );
		ASSERT( not _sources.IsExist( fmt ) );
		CHECK_ERR( file );

		usize		len		= usize(file->RemainingSize());
		BinaryArray	arr;	arr.Resize( len );

		CHECK_ERR( file->Read(BinArrayRef( arr )) );

		_sources.Add( fmt, Data_t{RVREF( arr )} );
		return true;
	}
	
/*
=================================================
	AddSpirv
=================================================
*/
	bool PipelineTemplateDescription::ShaderSource::AddSpirv (EShaderLangFormat::type fmt, const RFilePtr &file)
	{
		ASSERT( EShaderLangFormat::GetStorage( fmt ) == EShaderLangFormat::Binary );
		ASSERT( not _sources.IsExist( fmt ) );
		CHECK_ERR( file );

		usize		len		= usize(file->RemainingSize());
		Array<uint>	arr;	arr.Resize( len / sizeof(uint) );

		CHECK_ERR( file->Read(ArrayRef<uint>( arr )) );

		_sources.Add( fmt, Data_t{RVREF( arr )} );
		return true;
	}

/*
=================================================
	GetString
=================================================
*/
	StringCRef  PipelineTemplateDescription::ShaderSource::GetString (EShaderLangFormat::type fmt) const
	{
		Sources_t::const_iterator	iter;
		if ( not _sources.Find( fmt, OUT iter ) )
			return {};

		if ( not iter->second.Is<String>() )
			return {};

		return iter->second.Get<String>();
	}
	
/*
=================================================
	GetBinary
=================================================
*/
	BinArrayCRef  PipelineTemplateDescription::ShaderSource::GetBinary (EShaderLangFormat::type fmt) const
	{
		Sources_t::const_iterator	iter;
		if ( not _sources.Find( fmt, OUT iter ) )
			return {};

		if ( not iter->second.Is<BinaryArray>() )
			return {};

		return iter->second.Get<BinaryArray>();
	}
	
/*
=================================================
	GetSpirv
=================================================
*/
	ArrayCRef<uint>  PipelineTemplateDescription::ShaderSource::GetSpirv (EShaderLangFormat::type fmt) const
	{
		Sources_t::const_iterator	iter;
		if ( not _sources.Find( fmt, OUT iter ) )
			return {};

		if ( not iter->second.Is<Array<uint>>() )
			return {};

		return iter->second.Get<Array<uint>>();
	}
	
/*
=================================================
	GetInvocable
=================================================
*/
	PipelineTemplateDescription::ShaderSource::SWInvoke_t
		PipelineTemplateDescription::ShaderSource::GetInvocable (EShaderLangFormat::type fmt) const
	{
		Sources_t::const_iterator	iter;
		if ( not _sources.Find( fmt, OUT iter ) )
			return null;

		if ( not iter->second.Is<SWInvoke_t>() )
			return null;

		return iter->second.Get<SWInvoke_t>();
	}
	
/*
=================================================
	IsExists
=================================================
*/
	bool PipelineTemplateDescription::ShaderSource::IsExists (EShaderLangFormat::type fmt) const
	{
		return _sources.IsExist( fmt );
	}


}	// Platforms
}	// Engine
