// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "vulkan1.h"

#if defined( GRAPHICS_API_VULKAN )

#include "Engine/Platforms/Shared/GPU/RenderStateEnums.h"
#include "Engine/Platforms/Shared/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Shared/GPU/TextureEnums.h"
#include "Engine/Platforms/Shared/GPU/MultiSamples.h"

namespace Engine
{
namespace Platforms
{
	
/*
=================================================
	LogicOp
=================================================
*/
	inline vk::VkLogicOp  Vk1Enum (ELogicOp::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case ELogicOp::Clear		: return VK_LOGIC_OP_CLEAR;
			case ELogicOp::Set			: return VK_LOGIC_OP_SET;
			case ELogicOp::Copy			: return VK_LOGIC_OP_COPY;
			case ELogicOp::CopyInverted	: return VK_LOGIC_OP_COPY_INVERTED;
			case ELogicOp::Noop			: return VK_LOGIC_OP_NO_OP;
			case ELogicOp::Invert		: return VK_LOGIC_OP_INVERT;
			case ELogicOp::And			: return VK_LOGIC_OP_AND;
			case ELogicOp::NotAnd		: return VK_LOGIC_OP_NAND;
			case ELogicOp::Or			: return VK_LOGIC_OP_OR;
			case ELogicOp::NotOr		: return VK_LOGIC_OP_NOR;
			case ELogicOp::Xor			: return VK_LOGIC_OP_XOR;
			case ELogicOp::Equiv		: return VK_LOGIC_OP_EQUIVALENT;
			case ELogicOp::AndReverse	: return VK_LOGIC_OP_AND_REVERSE;
			case ELogicOp::AndInverted	: return VK_LOGIC_OP_AND_INVERTED;
			case ELogicOp::OrReverse	: return VK_LOGIC_OP_OR_REVERSE;
			case ELogicOp::OrInverted	: return VK_LOGIC_OP_OR_INVERTED;
		}
		RETURN_ERR( "invalid logical op", VK_LOGIC_OP_MAX_ENUM );
	}
	
/*
=================================================
	BlendFunc
=================================================
*/
	inline vk::VkBlendFactor  Vk1Enum (EBlendFunc::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EBlendFunc::Zero				: return VK_BLEND_FACTOR_ZERO;
			case EBlendFunc::One				: return VK_BLEND_FACTOR_ONE;
			case EBlendFunc::SrcColor			: return VK_BLEND_FACTOR_SRC_COLOR;
			case EBlendFunc::OneMinusSrcColor	: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			case EBlendFunc::DstColor			: return VK_BLEND_FACTOR_DST_COLOR;
			case EBlendFunc::OneMinusDstColor	: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			case EBlendFunc::SrcAlpha			: return VK_BLEND_FACTOR_SRC_ALPHA;
			case EBlendFunc::OneMinusSrcAlpha	: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case EBlendFunc::DstAlpha			: return VK_BLEND_FACTOR_DST_ALPHA;
			case EBlendFunc::OneMinusDstAlpha	: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case EBlendFunc::ConstColor			: return VK_BLEND_FACTOR_CONSTANT_COLOR;
			case EBlendFunc::OneMinusConstColor	: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
			case EBlendFunc::ConstAlpha			: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
			case EBlendFunc::OneMinusConstAlpha	: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
			case EBlendFunc::SrcAlphaSaturate	: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			// TODO
			//	VK_BLEND_FACTOR_SRC1_COLOR
			//	VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR
			//	VK_BLEND_FACTOR_SRC1_ALPHA
			//	VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA
		}
		RETURN_ERR( "invalid blend func", VK_BLEND_FACTOR_MAX_ENUM );
	}
	
/*
=================================================
	BlendEq
=================================================
*/
	inline vk::VkBlendOp  Vk1Enum (EBlendEq::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EBlendEq::Add		: return VK_BLEND_OP_ADD;
			case EBlendEq::Sub		: return VK_BLEND_OP_SUBTRACT;
			case EBlendEq::RevSub	: return VK_BLEND_OP_REVERSE_SUBTRACT;
			case EBlendEq::Min		: return VK_BLEND_OP_MIN;
			case EBlendEq::Max		: return VK_BLEND_OP_MAX;
		}
		RETURN_ERR( "invalid blend equation", VK_BLEND_OP_MAX_ENUM );
	}
	
/*
=================================================
	CompareFunc
=================================================
*/
	inline vk::VkCompareOp  Vk1Enum (ECompareFunc::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case ECompareFunc::Never	: return VK_COMPARE_OP_NEVER;
			case ECompareFunc::Less		: return VK_COMPARE_OP_LESS;
			case ECompareFunc::Equal	: return VK_COMPARE_OP_EQUAL;
			case ECompareFunc::LEqual	: return VK_COMPARE_OP_LESS_OR_EQUAL;
			case ECompareFunc::Greater	: return VK_COMPARE_OP_GREATER;
			case ECompareFunc::NotEqual	: return VK_COMPARE_OP_NOT_EQUAL;
			case ECompareFunc::GEqual	: return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case ECompareFunc::Always	: return VK_COMPARE_OP_ALWAYS;
		}
		RETURN_ERR( "invalid compare func", VK_COMPARE_OP_MAX_ENUM );
	}
	
/*
=================================================
	StencilOp
=================================================
*/
	inline vk::VkStencilOp  Vk1Enum (EStencilOp::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EStencilOp::Keep		: return VK_STENCIL_OP_KEEP;
			case EStencilOp::Zero		: return VK_STENCIL_OP_ZERO;
			case EStencilOp::Replace	: return VK_STENCIL_OP_REPLACE;
			case EStencilOp::Incr		: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			case EStencilOp::IncrWrap	: return VK_STENCIL_OP_INCREMENT_AND_WRAP;
			case EStencilOp::Decr		: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			case EStencilOp::DecrWrap	: return VK_STENCIL_OP_DECREMENT_AND_WRAP;
			case EStencilOp::Invert		: return VK_STENCIL_OP_INVERT;
		}
		RETURN_ERR( "invalid stencil op", VK_STENCIL_OP_MAX_ENUM );
	}
	
/*
=================================================
	PolygonMode
=================================================
*/
	inline vk::VkPolygonMode  Vk1Enum (EPolygonMode::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EPolygonMode::Point	: return VK_POLYGON_MODE_POINT;
			case EPolygonMode::Line		: return VK_POLYGON_MODE_LINE;
			case EPolygonMode::Fill		: return VK_POLYGON_MODE_FILL;
		}
		RETURN_ERR( "invalid polygon mode", VK_POLYGON_MODE_MAX_ENUM );
	}
	
/*
=================================================
	CullMode
=================================================
*/
	inline bool Vk1Enum (EPolygonFace::type value, OUT vk::VkCullModeFlagBits &cullMode)
	{
		using namespace vk;
		CHECK_ERR( value != EPolygonFace::Unknown );

		cullMode = VK_CULL_MODE_NONE;

		if ( EnumEq( value, EPolygonFace::Front ) )
			cullMode = VkCullModeFlagBits( cullMode | VK_CULL_MODE_FRONT_BIT );
		
		if ( EnumEq( value, EPolygonFace::Back ) )
			cullMode = VkCullModeFlagBits( cullMode | VK_CULL_MODE_BACK_BIT );

		return true;
	}
	
/*
=================================================
	StencilFace
=================================================
*/
	inline bool Vk1Enum (EPolygonFace::type value, OUT vk::VkStencilFaceFlagBits &stencilFaceMask)
	{
		using namespace vk;
		CHECK_ERR( value != EPolygonFace::Unknown );

		stencilFaceMask = VkStencilFaceFlagBits(0);

		if ( EnumEq( value, EPolygonFace::Front ) )
			stencilFaceMask = VkStencilFaceFlagBits( stencilFaceMask | VK_STENCIL_FACE_FRONT_BIT );
		
		if ( EnumEq( value, EPolygonFace::Back ) )
			stencilFaceMask = VkStencilFaceFlagBits( stencilFaceMask | VK_STENCIL_FACE_BACK_BIT );

		return true;
	}

/*
=================================================
	Primitive
=================================================
*/
	inline vk::VkPrimitiveTopology  Vk1Enum (EPrimitive::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EPrimitive::Point			:	return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case EPrimitive::LineList		:	return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case EPrimitive::LineStrip		:	return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case EPrimitive::TriangleList	:	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case EPrimitive::TriangleStrip	:	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			// TODO
			//	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN
			//	VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY
			//	VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY
			//	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY
			//	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY
			//	VK_PRIMITIVE_TOPOLOGY_PATCH_LIST
		}
		RETURN_ERR( "invalid primitive type", VK_PRIMITIVE_TOPOLOGY_MAX_ENUM );
	}
	
/*
=================================================
	BorderColor
=================================================
*
	inline vk::VkBorderColor  Vk1Enum (ESamplerBorderColor::type value)
	{
		using namespace vk;

		if ( EnumEq( value, ESamplerBorderColor::Int ) )
		{
			// int
			if ( EnumEq( value, ESamplerBorderColor::Black ) )
				return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			
			if ( EnumEq( value, ESamplerBorderColor::White ) )
				return VK_BORDER_COLOR_INT_OPAQUE_WHITE;

			//if ( EnumEq( value, ESamplerBorderColor::Transparent ) )
				return VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
		}
		else
		{
			// float
			if ( EnumEq( value, ESamplerBorderColor::Black ) )
				return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
			
			if ( EnumEq( value, ESamplerBorderColor::White ) )
				return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

			//if ( EnumEq( value, ESamplerBorderColor::Transparent ) )
				return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		}
	}
	
/*
=================================================
	AddressMode
=================================================
*/
	inline vk::VkSamplerAddressMode  Vk1Enum (EAddressMode::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EAddressMode::ClampUnnorm		:
			case EAddressMode::ClampToEdge		:	return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			case EAddressMode::ClampToBorder	:	return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
			case EAddressMode::Repeat			:	return VK_SAMPLER_ADDRESS_MODE_REPEAT;
			case EAddressMode::MirroredRepeat	:	return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			case EAddressMode::MirroredClamp	:	return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
		}

		RETURN_ERR( "invalid address mode", VK_SAMPLER_ADDRESS_MODE_MAX_ENUM );
	}
	
/*
=================================================
	PipelineDynamicState
=================================================
*/
	inline vk::VkDynamicState  Vk1Enum (EPipelineDynamicState::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EPipelineDynamicState::Viewport			: return VK_DYNAMIC_STATE_VIEWPORT;
			case EPipelineDynamicState::Scissor				: return VK_DYNAMIC_STATE_SCISSOR;
			case EPipelineDynamicState::LineWidth			: return VK_DYNAMIC_STATE_LINE_WIDTH;
			case EPipelineDynamicState::DepthBias			: return VK_DYNAMIC_STATE_DEPTH_BIAS;
			case EPipelineDynamicState::BlendConstants		: return VK_DYNAMIC_STATE_BLEND_CONSTANTS;
			case EPipelineDynamicState::DepthBounds			: return VK_DYNAMIC_STATE_DEPTH_BOUNDS;
			case EPipelineDynamicState::StencilCompareMask	: return VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;
			case EPipelineDynamicState::StencilWriteMask	: return VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;
			case EPipelineDynamicState::StencilReference	: return VK_DYNAMIC_STATE_STENCIL_REFERENCE;
		}

		RETURN_ERR( "invalid dynamic state type", VK_DYNAMIC_STATE_MAX_ENUM );
	}
	
/*
=================================================
	BufferUsage
=================================================
*
	inline vk::VkBufferUsageFlags  Vk1Enum (EBufferUsage::bits values)
	{
		using namespace vk;

		VkBufferUsageFlags	result = 0;

		FOR( i, values )
		{
			auto t = EBufferUsage::type(i);

			if ( not values.Get( t ) )
				continue;

			switch ( t )
			{
				case EBufferUsage::TransferSrc	:	result |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;			break;
				case EBufferUsage::TransferDst	:	result |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;			break;
				case EBufferUsage::UniformTexel	:	result |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;	break;
				case EBufferUsage::StorageTexel	:	result |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;	break;
				case EBufferUsage::Uniform		:	result |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;		break;
				case EBufferUsage::Storage		:	result |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;		break;
				case EBufferUsage::Index		:	result |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;			break;
				case EBufferUsage::Vertex		:	result |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;		break;
				case EBufferUsage::Indirect		:	result |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;		break;
				default							:	RETURN_ERR( "invalid buffer usage", 0 );
			}
		}

		return result;
	}
	
/*
=================================================
	BufferSparse
=================================================
*
	inline bool  Vk1Enum (OUT vk::VkBufferCreateFlagBits flags, ESparseMemory::bits values)
	{
		using namespace vk;

		flags = VkBufferCreateFlagBits(0);
		VkBufferCreateFlags result = 0;

		FOR( i, values )
		{
			auto t = ESparseMemory::type(i);

			if ( not values.Get( t ) )
				continue;

			switch ( t )
			{
				case ESparseMemory::None		:	break;
				case ESparseMemory::Binding		:	result |= VK_BUFFER_CREATE_SPARSE_BINDING_BIT;	break;
				case ESparseMemory::Residency	:	result |= VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT;	break;
				case ESparseMemory::Aliased		:	result |= VK_BUFFER_CREATE_SPARSE_ALIASED_BIT;	break;
				default							:	RETURN_ERR( "invalid buffer sparse type" );
			}
		}

		flags = (VkBufferCreateFlagBits) result;
		return true;
	}
	
/*
=================================================
	ImageSparse
=================================================
*
	inline bool  Vk1Enum (OUT vk::VkImageCreateFlagBits flags, ESparseMemory::bits values)
	{
		using namespace vk;
		
		flags = VkImageCreateFlagBits(0);
		VkImageCreateFlags result = 0;

		FOR( i, values )
		{
			auto t = ESparseMemory::type(i);

			if ( not values.Get( t ) )
				continue;

			switch ( t )
			{
				case ESparseMemory::None		:	break;
				case ESparseMemory::Binding		:	result |= VK_IMAGE_CREATE_SPARSE_BINDING_BIT;	break;
				case ESparseMemory::Residency	:	result |= VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT;	break;
				case ESparseMemory::Aliased		:	result |= VK_IMAGE_CREATE_SPARSE_ALIASED_BIT;	break;
				default							:	RETURN_ERR( "invalid buffer sparse type" );
			}
		}
		
		flags = (VkImageCreateFlagBits) result;
		return true;
	}
	
/*
=================================================
	IndexType
=================================================
*/
	inline vk::VkIndexType  Vk1Enum (EIndex::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EIndex::UShort	:	return VK_INDEX_TYPE_UINT16;
			case EIndex::UInt	:	return VK_INDEX_TYPE_UINT32;
		}

		RETURN_ERR( "invalid index type", VK_INDEX_TYPE_MAX_ENUM );
	}
	
/*
=================================================
	ShaderStage
=================================================
*
	inline vk::VkShaderStageFlagBits  Vk1Enum (EShader::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EShader::Vertex			: return VK_SHADER_STAGE_VERTEX_BIT;
			case EShader::TessControl		: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			case EShader::TessEvaluation	: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			case EShader::Geometry			: return VK_SHADER_STAGE_GEOMETRY_BIT;
			case EShader::Fragment			: return VK_SHADER_STAGE_FRAGMENT_BIT;
			case EShader::Compute			: return VK_SHADER_STAGE_COMPUTE_BIT;
		}

		RETURN_ERR( "invalid shader type", VkShaderStageFlagBits(0) );
	}
	
/*
=================================================
	VertexInputRate
=================================================
*/
	inline vk::VkVertexInputRate  Vk1Enum (EVertexInputRate::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EVertexInputRate::Vertex	: return VK_VERTEX_INPUT_RATE_VERTEX;
			case EVertexInputRate::Instance	: return VK_VERTEX_INPUT_RATE_INSTANCE;
		}

		RETURN_ERR( "invalid vertex input rate", VK_VERTEX_INPUT_RATE_MAX_ENUM );
	}
	
/*
=================================================
	MultiSamples
=================================================
*/
	inline vk::VkSampleCountFlagBits  Vk1Enum (MultiSamples value)
	{
		using namespace vk;

		return Clamp( (VkSampleCountFlagBits)value.Get(), VK_SAMPLE_COUNT_1_BIT, VK_SAMPLE_COUNT_64_BIT );
	}
	
/*
=================================================
	AttachmentLoadOp
=================================================
*
	inline vk::VkAttachmentLoadOp  Vk1Enum (EAttachmentLoadOp::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EAttachmentLoadOp::None	: return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			case EAttachmentLoadOp::Load	: return VK_ATTACHMENT_LOAD_OP_LOAD;
			case EAttachmentLoadOp::Clear	: return VK_ATTACHMENT_LOAD_OP_CLEAR;
		}

		RETURN_ERR( "invalid load op type", VK_ATTACHMENT_LOAD_OP_MAX_ENUM );
	}
	
/*
=================================================
	AttachmentStoreOp
=================================================
*
	inline vk::VkAttachmentStoreOp  Vk1Enum (EAttachmentStoreOp::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EAttachmentStoreOp::None	: return VK_ATTACHMENT_STORE_OP_DONT_CARE;
			case EAttachmentStoreOp::Store	: return VK_ATTACHMENT_STORE_OP_STORE;
		}

		RETURN_ERR( "invalid store op type", VK_ATTACHMENT_STORE_OP_MAX_ENUM );
	}
	
/*
=================================================
	ImageLayout
=================================================
*
	inline vk::VkImageLayout  Vk1Enum (EImageLayout::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EImageLayout::Undefined						: return VK_IMAGE_LAYOUT_UNDEFINED;
			case EImageLayout::General							: return VK_IMAGE_LAYOUT_GENERAL;
			case EImageLayout::ColorAttachmentOptimal			: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			case EImageLayout::DepthStencilAttachmentOptimal	: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			case EImageLayout::DepthStencilReadOnlyOptimal		: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			case EImageLayout::ShaderReadOnlyOptimal			: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			case EImageLayout::TransferSrcOptimal				: return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			case EImageLayout::TransferDstOptimal				: return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			case EImageLayout::Preinitialized					: return VK_IMAGE_LAYOUT_PREINITIALIZED;
			case EImageLayout::PresentSrc						: return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		}

		RETURN_ERR( "invalid image layout type", VK_IMAGE_LAYOUT_MAX_ENUM );
	}
	
/*
=================================================
	SubpassDependency
=================================================
*
	inline vk::VkDependencyFlags  Vk1Enum (ESubpassDependency::bits values)
	{
		using namespace vk;

		VkDependencyFlags	flags = 0;

		FOR( i, values )
		{
			auto t = ESubpassDependency::type(i);

			if ( not values.Get( t ) )
				continue;

			switch ( t )
			{
				case ESubpassDependency::ByRegion		: flags |= VK_DEPENDENCY_BY_REGION_BIT;			break;
				case ESubpassDependency::ViewLocal		: flags |= VK_DEPENDENCY_VIEW_LOCAL_BIT_KHX;	break;
				case ESubpassDependency::DeviceGroup	: flags |= VK_DEPENDENCY_DEVICE_GROUP_BIT_KHX;	break;
				default									: RETURN_ERR( "invalid dependency type", VK_DEPENDENCY_FLAG_BITS_MAX_ENUM );
			}
		}
		return flags;
	}
	
/*
=================================================
	PipelineStage
=================================================
*
	inline vk::VkPipelineStageFlags  Vk1Enum (EPipelineStage::bits values)
	{
		using namespace vk;

		VkPipelineStageFlags	flags = 0;

		FOR( i, values )
		{
			auto t = EPipelineStage::type(i);

			if ( not values.Get( t ) )
				continue;

			switch ( t )
			{
				case EPipelineStage::TopOfPipe				: flags |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;						break;
				case EPipelineStage::DrawIndirect			: flags |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;						break;
				case EPipelineStage::VertexInput			: flags |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;						break;
				case EPipelineStage::VertexShader			: flags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;						break;
				case EPipelineStage::TessControlShader		: flags |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;		break;
				case EPipelineStage::TessEvaluationShader	: flags |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;	break;
				case EPipelineStage::GeometryShader			: flags |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;					break;
				case EPipelineStage::FragmentShader			: flags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;					break;
				case EPipelineStage::EarlyFragmentTests		: flags |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;				break;
				case EPipelineStage::LateFragmentTests		: flags |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;				break;
				case EPipelineStage::ColorAttachments		: flags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;			break;
				case EPipelineStage::ComputeShader			: flags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;					break;
				case EPipelineStage::Transfer				: flags |= VK_PIPELINE_STAGE_TRANSFER_BIT;							break;
				case EPipelineStage::BottomOfPipe			: flags |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;					break;
				case EPipelineStage::Host					: flags |= VK_PIPELINE_STAGE_HOST_BIT;								break;
				default										: RETURN_ERR( "invalid pipeline state", VK_PIPELINE_STAGE_FLAG_BITS_MAX_ENUM );
			}
		}
		return flags;
	}
		
/*
=================================================
	SubpassAccess
=================================================
*
	inline vk::VkAccessFlags  Vk1Enum (ESubpassAccess::bits values)
	{
		using namespace vk;

		VkAccessFlags	flags = 0;

		FOR( i, values )
		{
			auto t = ESubpassAccess::type(i);

			if ( not values.Get( t ) )
				continue;

			switch ( t )
			{
				case ESubpassAccess::IndirectCommandRead			: flags |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;				break;
				case ESubpassAccess::IndexRead						: flags |= VK_ACCESS_INDEX_READ_BIT;						break;
				case ESubpassAccess::VertexAttributeRead			: flags |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;				break;
				case ESubpassAccess::UniformRead					: flags |= VK_ACCESS_UNIFORM_READ_BIT;						break;
				case ESubpassAccess::InputAttachmentRead			: flags |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;				break;
				case ESubpassAccess::ShaderRead						: flags |= VK_ACCESS_SHADER_READ_BIT;						break;
				case ESubpassAccess::ShaderWrite					: flags |= VK_ACCESS_SHADER_WRITE_BIT;						break;
				case ESubpassAccess::ColorAttachmentRead			: flags |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;				break;
				case ESubpassAccess::ColorAttachmentWrite			: flags |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;			break;
				case ESubpassAccess::DepthStencilAttachmentRead		: flags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;		break;
				case ESubpassAccess::DepthStencilAttachmentWrite	: flags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;	break;
				case ESubpassAccess::TransferRead					: flags |= VK_ACCESS_TRANSFER_READ_BIT;						break;
				case ESubpassAccess::TransferWrite					: flags |= VK_ACCESS_TRANSFER_WRITE_BIT;					break;
				case ESubpassAccess::HostRead						: flags |= VK_ACCESS_HOST_READ_BIT;							break;
				case ESubpassAccess::HostWrite						: flags |= VK_ACCESS_HOST_WRITE_BIT;						break;
				case ESubpassAccess::MemoryRead						: flags |= VK_ACCESS_MEMORY_READ_BIT;						break;
				case ESubpassAccess::MemoryWrite					: flags |= VK_ACCESS_MEMORY_WRITE_BIT;						break;
				default												: RETURN_ERR( "invalid access type", VK_ACCESS_FLAG_BITS_MAX_ENUM );
			}
		}
		return flags;
	}
		
/*
=================================================
	ImageUsage
=================================================
*
	inline vk::VkImageUsageFlags  Vk1Enum (EImageUsage::bits values)
	{
		using namespace vk;

		VkImageUsageFlags	flags = 0;

		FOR( i, values )
		{
			auto t = EImageUsage::type(i);
			
			if ( not values.Get( t ) )
				continue;

			switch ( t )
			{
				case EImageUsage::TransferSrc				: flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;				break;
				case EImageUsage::TransferDst				: flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;				break;
				case EImageUsage::Sampled					: flags |= VK_IMAGE_USAGE_SAMPLED_BIT;					break;
				case EImageUsage::Storage					: flags |= VK_IMAGE_USAGE_STORAGE_BIT;					break;
				case EImageUsage::ColorAttachment			: flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;			break;
				case EImageUsage::DepthStencilAttachment	: flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;	break;
				case EImageUsage::TransientAttachment		: flags |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;		break;
				case EImageUsage::InputAttachment			: flags |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;			break;
				default										: RETURN_ERR( "invalid image usage type", VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM );
			}
		}
		return flags;
	}

/*
=================================================
	Format
=================================================
*
	inline vk::VkFormat  Vk1Enum (EAttribute::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EAttribute::Byte			: return VK_FORMAT_R8_SINT;
			case EAttribute::Byte2			: return VK_FORMAT_R8G8_SINT;
			case EAttribute::Byte3			: return VK_FORMAT_R8G8B8_SINT;
			case EAttribute::Byte4			: return VK_FORMAT_R8G8B8A8_SINT;
			case EAttribute::Byte_Norm		: return VK_FORMAT_R8_SNORM;
			case EAttribute::Byte2_Norm		: return VK_FORMAT_R8G8_SNORM;
			case EAttribute::Byte3_Norm		: return VK_FORMAT_R8G8B8_SNORM;
			case EAttribute::Byte4_Norm		: return VK_FORMAT_R8G8B8A8_SNORM;
			case EAttribute::UByte			: return VK_FORMAT_R8_UINT;
			case EAttribute::UByte2			: return VK_FORMAT_R8G8_UINT;
			case EAttribute::UByte3			: return VK_FORMAT_R8G8B8_UINT;
			case EAttribute::UByte4			: return VK_FORMAT_R8G8B8A8_UINT;
			case EAttribute::UByte_Norm		: return VK_FORMAT_R8_UNORM;
			case EAttribute::UByte2_Norm	: return VK_FORMAT_R8G8_UNORM;
			case EAttribute::UByte3_Norm	: return VK_FORMAT_R8G8B8_UNORM;
			case EAttribute::UByte4_Norm	: return VK_FORMAT_R8G8B8A8_UNORM;
			case EAttribute::Short			: return VK_FORMAT_R16_SINT;
			case EAttribute::Short2			: return VK_FORMAT_R16G16_SINT;
			case EAttribute::Short3			: return VK_FORMAT_R16G16B16_SINT;
			case EAttribute::Short4			: return VK_FORMAT_R16G16B16A16_SINT;
			case EAttribute::Short_Norm		: return VK_FORMAT_R16_SNORM;
			case EAttribute::Short2_Norm	: return VK_FORMAT_R16G16_SNORM;
			case EAttribute::Short3_Norm	: return VK_FORMAT_R16G16B16_SNORM;
			case EAttribute::Short4_Norm	: return VK_FORMAT_R16G16B16A16_SNORM;
			case EAttribute::UShort			: return VK_FORMAT_R16_UINT;
			case EAttribute::UShort2		: return VK_FORMAT_R16G16_UINT;
			case EAttribute::UShort3		: return VK_FORMAT_R16G16B16_UINT;
			case EAttribute::UShort4		: return VK_FORMAT_R16G16B16A16_UINT;
			case EAttribute::UShort_Norm	: return VK_FORMAT_R16_UNORM;
			case EAttribute::UShort2_Norm	: return VK_FORMAT_R16G16_UNORM;
			case EAttribute::UShort3_Norm	: return VK_FORMAT_R16G16B16_UNORM;
			case EAttribute::UShort4_Norm	: return VK_FORMAT_R16G16B16A16_UNORM;
			case EAttribute::Int			: return VK_FORMAT_R32_SINT;
			case EAttribute::Int2			: return VK_FORMAT_R32G32_SINT;
			case EAttribute::Int3			: return VK_FORMAT_R32G32B32_SINT;
			case EAttribute::Int4			: return VK_FORMAT_R32G32B32A32_SINT;
			case EAttribute::UInt			: return VK_FORMAT_R32_UINT;
			case EAttribute::UInt2			: return VK_FORMAT_R32G32_UINT;
			case EAttribute::UInt3			: return VK_FORMAT_R32G32B32_UINT;
			case EAttribute::UInt4			: return VK_FORMAT_R32G32B32A32_UINT;
			case EAttribute::Long			: return VK_FORMAT_R64_SINT;
			case EAttribute::Long2			: return VK_FORMAT_R64G64_SINT;
			case EAttribute::Long3			: return VK_FORMAT_R64G64B64_SINT;
			case EAttribute::Long4			: return VK_FORMAT_R64G64B64A64_SINT;
			case EAttribute::ULong			: return VK_FORMAT_R64_UINT;
			case EAttribute::ULong2			: return VK_FORMAT_R64G64_UINT;
			case EAttribute::ULong3			: return VK_FORMAT_R64G64B64_UINT;
			case EAttribute::ULong4			: return VK_FORMAT_R64G64B64A64_UINT;
			case EAttribute::Float			: return VK_FORMAT_R32_SFLOAT;
			case EAttribute::Float2			: return VK_FORMAT_R32G32_SFLOAT;
			case EAttribute::Float3			: return VK_FORMAT_R32G32B32_SFLOAT;
			case EAttribute::Float4			: return VK_FORMAT_R32G32B32A32_SFLOAT;
			case EAttribute::Double			: return VK_FORMAT_R64_SFLOAT;
			case EAttribute::Double2		: return VK_FORMAT_R64G64_SFLOAT;
			case EAttribute::Double3		: return VK_FORMAT_R64G64B64_SFLOAT;
			case EAttribute::Double4		: return VK_FORMAT_R64G64B64A64_SFLOAT;

		}
		RETURN_ERR( "invalid attrib format", VK_FORMAT_MAX_ENUM );
	}

/*
=================================================
	Format
=================================================
*/
	inline vk::VkFormat  Vk1Enum (EPixelFormat::type value)
	{
		using namespace vk;

		switch ( value )
		{
			case EPixelFormat::Unknown				: return VK_FORMAT_UNDEFINED;
			case EPixelFormat::RGBA16_SNorm			: return VK_FORMAT_R16G16B16A16_SNORM;
			case EPixelFormat::RGBA8_SNorm			: return VK_FORMAT_R8G8B8A8_SNORM;
			case EPixelFormat::RGB16_SNorm			: return VK_FORMAT_R16G16B16_SNORM;
			case EPixelFormat::RGB8_SNorm			: return VK_FORMAT_R8G8B8_SNORM;
			case EPixelFormat::RG16_SNorm			: return VK_FORMAT_R16G16_SNORM;
			case EPixelFormat::RG8_SNorm			: return VK_FORMAT_R8G8_SNORM;
			case EPixelFormat::R16_SNorm			: return VK_FORMAT_R16_SNORM;
			case EPixelFormat::R8_SNorm				: return VK_FORMAT_R8_SNORM;
			case EPixelFormat::RGBA16_UNorm			: return VK_FORMAT_R16G16B16A16_UNORM;
			case EPixelFormat::RGBA8_UNorm			: return VK_FORMAT_R8G8B8A8_UNORM;
			case EPixelFormat::RGB16_UNorm			: return VK_FORMAT_R16G16B16_UNORM;
			case EPixelFormat::RGB8_UNorm			: return VK_FORMAT_R8G8B8_UNORM;
			case EPixelFormat::RG16_UNorm			: return VK_FORMAT_R16G16_UNORM;
			case EPixelFormat::RG8_UNorm			: return VK_FORMAT_R8G8_UNORM;
			case EPixelFormat::R16_UNorm			: return VK_FORMAT_R16_UNORM;
			case EPixelFormat::R8_UNorm				: return VK_FORMAT_R8_UNORM;
			case EPixelFormat::RGB10_A2_UNorm		: return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
			case EPixelFormat::RGBA4_UNorm			: return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
			case EPixelFormat::RGB5_A1_UNorm		: return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
			case EPixelFormat::RGB_5_6_5_UNorm		: return VK_FORMAT_R5G6B5_UNORM_PACK16;
			case EPixelFormat::BGR8_UNorm			: return VK_FORMAT_B8G8R8_UNORM;
			case EPixelFormat::BGRA8_UNorm			: return VK_FORMAT_B8G8R8A8_UNORM;
			case EPixelFormat::sRGB8				: return VK_FORMAT_R8G8B8_SRGB;
			case EPixelFormat::sRGB8_A8				: return VK_FORMAT_R8G8B8A8_SRGB;
			case EPixelFormat::R8I					: return VK_FORMAT_R8_SINT;
			case EPixelFormat::RG8I					: return VK_FORMAT_R8G8_SINT;
			case EPixelFormat::RGB8I				: return VK_FORMAT_R8G8B8_SINT;
			case EPixelFormat::RGBA8I				: return VK_FORMAT_R8G8B8A8_SINT;
			case EPixelFormat::R16I					: return VK_FORMAT_R16_SINT;
			case EPixelFormat::RG16I				: return VK_FORMAT_R16G16_SINT;
			case EPixelFormat::RGB16I				: return VK_FORMAT_R16G16B16_SINT;
			case EPixelFormat::RGBA16I				: return VK_FORMAT_R16G16B16A16_SINT;
			case EPixelFormat::R32I					: return VK_FORMAT_R32_SINT;
			case EPixelFormat::RG32I				: return VK_FORMAT_R32G32_SINT;
			case EPixelFormat::RGB32I				: return VK_FORMAT_R32G32B32_SINT;
			case EPixelFormat::RGBA32I				: return VK_FORMAT_R32G32B32A32_UINT;
			case EPixelFormat::R8U					: return VK_FORMAT_R8_UINT;
			case EPixelFormat::RG8U					: return VK_FORMAT_R8G8_UINT;
			case EPixelFormat::RGB8U				: return VK_FORMAT_R8G8B8_UINT;
			case EPixelFormat::RGBA8U				: return VK_FORMAT_R8G8B8A8_UINT;
			case EPixelFormat::R16U					: return VK_FORMAT_R16_UINT;
			case EPixelFormat::RG16U				: return VK_FORMAT_R16G16_UINT;
			case EPixelFormat::RGB16U				: return VK_FORMAT_R16G16B16_UINT;
			case EPixelFormat::RGBA16U				: return VK_FORMAT_R16G16B16A16_UINT;
			case EPixelFormat::R32U					: return VK_FORMAT_R32_UINT;
			case EPixelFormat::RG32U				: return VK_FORMAT_R32G32_UINT;
			case EPixelFormat::RGB32U				: return VK_FORMAT_R32G32B32_UINT;
			case EPixelFormat::RGBA32U				: return VK_FORMAT_R32G32B32A32_SINT;
			case EPixelFormat::RGB10_A2U			: return VK_FORMAT_A2B10G10R10_UINT_PACK32;
			case EPixelFormat::R16F					: return VK_FORMAT_R16_SFLOAT;
			case EPixelFormat::RG16F				: return VK_FORMAT_R16G16_SFLOAT;
			case EPixelFormat::RGB16F				: return VK_FORMAT_R16G16B16_SFLOAT;
			case EPixelFormat::RGBA16F				: return VK_FORMAT_R16G16B16A16_SFLOAT;
			case EPixelFormat::R32F					: return VK_FORMAT_R32_SFLOAT;
			case EPixelFormat::RG32F				: return VK_FORMAT_R32G32_SFLOAT;
			case EPixelFormat::RGB32F				: return VK_FORMAT_R32G32B32_SFLOAT;
			case EPixelFormat::RGBA32F				: return VK_FORMAT_R32G32B32A32_SFLOAT;
			case EPixelFormat::RGB_11_11_10F		: return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
			case EPixelFormat::Depth16				: return VK_FORMAT_D16_UNORM;
			case EPixelFormat::Depth24				: return VK_FORMAT_X8_D24_UNORM_PACK32;
			case EPixelFormat::Depth32				:	// TODO: remove?
			case EPixelFormat::Depth32F				: return VK_FORMAT_D32_SFLOAT;
			case EPixelFormat::Depth16_Stencil8		: return VK_FORMAT_D16_UNORM_S8_UINT;
			case EPixelFormat::Depth24_Stencil8		: return VK_FORMAT_D24_UNORM_S8_UINT;
			case EPixelFormat::Depth32F_Stencil8	: return VK_FORMAT_D32_SFLOAT_S8_UINT;
			case EPixelFormat::BC1_RGB8_UNorm		: return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
			case EPixelFormat::BC1_RGB8_A1_UNorm	: return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
			case EPixelFormat::BC2_RGBA8_UNorm		: return VK_FORMAT_BC2_UNORM_BLOCK;
			case EPixelFormat::BC3_RGBA8_UNorm		: return VK_FORMAT_BC3_UNORM_BLOCK;
			case EPixelFormat::BC4_RED8_SNorm		: return VK_FORMAT_BC4_SNORM_BLOCK;
			case EPixelFormat::BC4_RED8_UNorm		: return VK_FORMAT_BC4_UNORM_BLOCK;
			case EPixelFormat::BC5_RG8_SNorm		: return VK_FORMAT_BC5_SNORM_BLOCK;
			case EPixelFormat::BC5_RG8_UNorm		: return VK_FORMAT_BC5_UNORM_BLOCK;
			case EPixelFormat::BC7_RGBA8_UNorm		: return VK_FORMAT_BC7_UNORM_BLOCK;
			case EPixelFormat::BC7_SRGB8_A8_UNorm	: return VK_FORMAT_BC7_SRGB_BLOCK;
			case EPixelFormat::BC6H_RGB16F			: return VK_FORMAT_BC6H_SFLOAT_BLOCK;
			case EPixelFormat::BC6H_RGB16F_Unsigned	: return VK_FORMAT_BC6H_UFLOAT_BLOCK;
			case EPixelFormat::ETC2_RGB8_UNorm		: return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
			case EPixelFormat::ECT2_SRGB8_UNorm		: return VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
			case EPixelFormat::ETC2_RGB8_A1_UNorm	: return VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
			case EPixelFormat::ETC2_SRGB8_A1_UNorm	: return VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
			case EPixelFormat::ETC2_RGBA8_UNorm		: return VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
			case EPixelFormat::ETC2_SRGB8_A8_UNorm	: return VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
			case EPixelFormat::EAC_R11_SNorm		: return VK_FORMAT_EAC_R11_SNORM_BLOCK;
			case EPixelFormat::EAC_R11_UNorm		: return VK_FORMAT_EAC_R11_UNORM_BLOCK;
			case EPixelFormat::EAC_RG11_SNorm		: return VK_FORMAT_EAC_R11G11_SNORM_BLOCK;
			case EPixelFormat::EAC_RG11_UNorm		: return VK_FORMAT_EAC_R11G11_UNORM_BLOCK;
			case EPixelFormat::ASTC_RGBA_4x4		: return VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
			case EPixelFormat::ASTC_RGBA_5x4		: return VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
			case EPixelFormat::ASTC_RGBA_5x5		: return VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
			case EPixelFormat::ASTC_RGBA_6x5		: return VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
			case EPixelFormat::ASTC_RGBA_6x6		: return VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
			case EPixelFormat::ASTC_RGBA_8x5		: return VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
			case EPixelFormat::ASTC_RGBA_8x6		: return VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
			case EPixelFormat::ASTC_RGBA_8x8		: return VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
			case EPixelFormat::ASTC_RGBA_10x5		: return VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
			case EPixelFormat::ASTC_RGBA_10x6		: return VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
			case EPixelFormat::ASTC_RGBA_10x8		: return VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
			case EPixelFormat::ASTC_RGBA_10x10		: return VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
			case EPixelFormat::ASTC_RGBA_12x10		: return VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
			case EPixelFormat::ASTC_RGBA_12x12		: return VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
			case EPixelFormat::ASTC_SRGB8_A8_4x4	: return VK_FORMAT_ASTC_4x4_SRGB_BLOCK;
			case EPixelFormat::ASTC_SRGB8_A8_5x4	: return VK_FORMAT_ASTC_5x4_SRGB_BLOCK;
			case EPixelFormat::ASTC_SRGB8_A8_5x5	: return VK_FORMAT_ASTC_5x5_SRGB_BLOCK;
			case EPixelFormat::ASTC_SRGB8_A8_6x5	: return VK_FORMAT_ASTC_6x5_SRGB_BLOCK;
			case EPixelFormat::ASTC_SRGB8_A8_6x6	: return VK_FORMAT_ASTC_6x6_SRGB_BLOCK;
			case EPixelFormat::ASTC_SRGB8_A8_8x5	: return VK_FORMAT_ASTC_8x5_SRGB_BLOCK;
			case EPixelFormat::ASTC_SRGB8_A8_8x6	: return VK_FORMAT_ASTC_8x6_SRGB_BLOCK;
			case EPixelFormat::ASTC_SRGB8_A8_8x8	: return VK_FORMAT_ASTC_8x8_SRGB_BLOCK;
			case EPixelFormat::ASTC_SRGB8_A8_10x5	: return VK_FORMAT_ASTC_10x5_SRGB_BLOCK;
			case EPixelFormat::ASTC_SRGB8_A8_10x6	: return VK_FORMAT_ASTC_10x6_SRGB_BLOCK;
			case EPixelFormat::ASTC_SRGB8_A8_10x8	: return VK_FORMAT_ASTC_10x8_SRGB_BLOCK;
			case EPixelFormat::ASTC_SRGB8_A8_10x10	: return VK_FORMAT_ASTC_10x10_SRGB_BLOCK;
			case EPixelFormat::ASTC_SRGB8_A8_12x10	: return VK_FORMAT_ASTC_12x10_SRGB_BLOCK;
			case EPixelFormat::ASTC_SRGB8_A8_12x12	: return VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
		}

		RETURN_ERR( "invalid pixel format", VK_FORMAT_MAX_ENUM );
	}


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
