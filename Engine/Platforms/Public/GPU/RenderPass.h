// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/RenderPassEnums.h"
#include "Engine/Platforms/Public/GPU/ImageEnums.h"
#include "Engine/Platforms/Public/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Public/GPU/MultiSamples.h"
#include "Engine/Platforms/Public/GPU/IDs.h"

namespace Engine
{
namespace Platforms
{
	
	//
	// Render Pass Descriptor
	//
	
	struct RenderPassDescriptor final : CompileTime::FastCopyable
	{
		friend struct RenderPassDescrBuilder;

	// types
	public:
		using Self		= RenderPassDescriptor;
		using Name_t	= StaticString<64>;

		static const uint	MAX_COLOR_ATTACHMENTS	= GlobalConst::GAPI_MaxColorBuffers;
		static const uint	MAX_SUBPASSES			= GlobalConst::GAPI_MaxRenderPassSubpasses;
		static const uint	MAX_DEPENDENCIES		= MAX_SUBPASSES * 2;


		//
		// Color Attachment
		//
		struct ColorAttachment_t : CompileTime::PODStruct
		{
		// variables
			Name_t						name;
			EPixelFormat::type			format;
			MultiSamples				samples;
			EAttachmentLoadOp::type		loadOp;			// for color and depth
			EAttachmentStoreOp::type	storeOp;		// for color and depth
			EImageLayout::type			initialLayout;
			EImageLayout::type			finalLayout;

		// methods
			ColorAttachment_t (GX_DEFCTOR) :
				format( EPixelFormat::Unknown ),			samples(),
				loadOp( EAttachmentLoadOp::Unknown ),		storeOp( EAttachmentStoreOp::Unknown ),
				initialLayout( EImageLayout::Undefined ),	finalLayout( EImageLayout::Undefined )
			{}
		};


		//
		// Depth Stencil Attachment
		//
		struct DepthStencilAttachment_t : ColorAttachment_t
		{
		// variables
			EAttachmentLoadOp::type		stencilLoadOp;
			EAttachmentStoreOp::type	stencilStoreOp;
			
		// methods
			DepthStencilAttachment_t (GX_DEFCTOR) :
				ColorAttachment_t(),
				stencilLoadOp( EAttachmentLoadOp::Unknown ),	stencilStoreOp( EAttachmentStoreOp::Unknown )
			{}

			bool IsEnabled () const		{ return format != EPixelFormat::Unknown; }
		};


		//
		// Attachment Reference
		//
		struct AttachmentRef_t : CompileTime::PODStruct
		{
		// variables
			Name_t				name;
			EImageLayout::type	layout;


		// methods
			AttachmentRef_t (GX_DEFCTOR) :
				layout( EImageLayout::Undefined )
			{}

			bool IsEnabled () const		{ return not name.Empty(); }
		};

		using AttachmentsRef_t		= FixedSizeArray< AttachmentRef_t, MAX_COLOR_ATTACHMENTS >;
		using PreserveAttachments_t	= FixedSizeArray< Name_t, MAX_COLOR_ATTACHMENTS >;


		//
		// Subpass
		//
		struct Subpass_t : CompileTime::PODStruct
		{
		// variables
			Name_t					name;
			AttachmentsRef_t		colors;
			AttachmentRef_t			depthStencil;
			AttachmentsRef_t		inputs;
			AttachmentRef_t			resolve;
			PreserveAttachments_t	preserves;
			
		// methods
			Subpass_t (GX_DEFCTOR)
			{}

			explicit Subpass_t (StringCRef name) : name(name)
			{}
		};


		//
		// Subpass Dependency
		//
		struct SubpassDependency_t : CompileTime::PODStruct
		{
		// variables
			Name_t						srcPass;
			EPipelineStage::bits		srcStage;
			EPipelineAccess::bits		srcAccess;

			Name_t						dstPass;
			EPipelineStage::bits		dstStage;
			EPipelineAccess::bits		dstAccess;

			ESubpassDependency::bits	dependency;

		// methods
			SubpassDependency_t (GX_DEFCTOR)
			{}
		};

		using ColorAttachments_t	= FixedSizeArray< ColorAttachment_t, MAX_COLOR_ATTACHMENTS >;
		using Subpasses_t			= Array< Subpass_t >;
		using Dependencies_t		= FixedSizeArray< SubpassDependency_t, MAX_DEPENDENCIES >;


	// variables
	private:
		ColorAttachments_t			_colorAttachmens;
		DepthStencilAttachment_t	_depthStencilAttachment;
		Subpasses_t					_subpasses;
		Dependencies_t				_dependencies;
		HashResult					_hash;


	// methods
	public:
		RenderPassDescriptor (GX_DEFCTOR) {}

		ArrayCRef< ColorAttachment_t >		ColorAttachments ()			const	{ return _colorAttachmens; }
		ArrayCRef< Subpass_t >				Subpasses ()				const	{ return _subpasses; }
		ArrayCRef< SubpassDependency_t >	Dependencies ()				const	{ return _dependencies; }
		DepthStencilAttachment_t const&		DepthStencilAttachment ()	const	{ return _depthStencilAttachment; }

		bool operator == (const Self &right) const;
		bool operator >  (const Self &right) const;
		bool operator <  (const Self &right) const;
		
		bool operator != (const Self &right) const		{ return not (*this == right); }
		bool operator >= (const Self &right) const		{ return not (*this <  right); }
		bool operator <= (const Self &right) const		{ return not (*this >  right); }
	};



	//
	// Render Pass Descriptor Builder
	//

	struct RenderPassDescrBuilder final
	{
	// types
	public:
		using Self						= RenderPassDescrBuilder;
		using Name_t					= RenderPassDescriptor::Name_t;
		using Subpass_t					= RenderPassDescriptor::Subpass_t;
		using AttachmentRef_t			= RenderPassDescriptor::AttachmentRef_t;
		using ColorAttachment_t			= RenderPassDescriptor::ColorAttachment_t;
		using DepthStencilAttachment_t	= RenderPassDescriptor::DepthStencilAttachment_t;
		using SubpassDependency_t		= RenderPassDescriptor::SubpassDependency_t;

		struct SimpleBuilder;

		struct SubpassBuilder final
		{
		// variables
		private:
			Subpass_t &		_valueRef;

		// methods
		public:
			SubpassBuilder (Subpass_t &value) : _valueRef(value) {}
			
			SubpassBuilder& AddColorAttachment (const AttachmentRef_t &value);
			SubpassBuilder& AddColorAttachment (StringCRef			   name,
												EImageLayout::type	   layout);
			
			SubpassBuilder& SetDepthStencilAttachment (const AttachmentRef_t &value);
			SubpassBuilder& SetDepthStencilAttachment (StringCRef			  name,
													   EImageLayout::type	  layout);

			SubpassBuilder& AddInputAttachment (const AttachmentRef_t &value);
			SubpassBuilder& AddInputAttachment (StringCRef			   name,
												EImageLayout::type	   layout);

			SubpassBuilder& SetResolveAttachment (const AttachmentRef_t &value);
			SubpassBuilder& SetResolveAttachment (StringCRef			 name,
												  EImageLayout::type	 layout);

			SubpassBuilder& AddPreserveAttachment (StringCRef name);
		};


	// variables
	private:
		RenderPassDescriptor	_state;
		bool					_changed;

		
	// methods
	public:
		RenderPassDescrBuilder () : _state(), _changed(true)
		{}

		explicit
		RenderPassDescrBuilder (const RenderPassDescriptor &state) : _state(state), _changed(true)
		{}

		Self& AddColorAttachment (const ColorAttachment_t &value);
		Self& AddColorAttachment (StringCRef				name,
								  EPixelFormat::type		format,
								  MultiSamples				samples,
								  EAttachmentLoadOp::type	loadOp,
								  EAttachmentStoreOp::type	storeOp,
								  EImageLayout::type		initialLayout,
								  EImageLayout::type		finalLayout);

		Self& SetDepthStencilAttachment (const DepthStencilAttachment_t &value);
		Self& SetDepthStencilAttachment (StringCRef					name,
										 EPixelFormat::type			format,
										 MultiSamples				samples,
										 EAttachmentLoadOp::type	depthLoadOp,
										 EAttachmentStoreOp::type	depthStoreOp,
										 EAttachmentLoadOp::type	stencilLoadOp,
										 EAttachmentStoreOp::type	stencilStoreOp,
										 EImageLayout::type			initialLayout,
										 EImageLayout::type			finalLayout);

		Self& AddSubpass (const Subpass_t &value);
		SubpassBuilder AddSubpass (StringCRef name);

		Self& AddDependency (const SubpassDependency_t &value);
		Self& AddDependency (StringCRef					srcPass,
							 EPipelineStage::bits		srcStage,
							 EPipelineAccess::bits		srcAccess,
							 StringCRef					dstPass,
							 EPipelineStage::bits		dstStage,
							 EPipelineAccess::bits		dstAccess,
							 ESubpassDependency::bits	dependency);

		// validate, calculate hash and return
		RenderPassDescriptor const& Finish ();

		// default render pass for presenting to screen
		static RenderPassDescriptor CreateForSurface (EPixelFormat::type colorFmt, EPixelFormat::type depthStencilFmt, EImageLayout::type finalLayout = EImageLayout::PresentSrc);

		// simplified builder
		static SimpleBuilder CreateForFramebuffer ();
	};


	//
	// Simple Builder
	//
	struct RenderPassDescrBuilder::SimpleBuilder final
	{
		friend struct RenderPassDescrBuilder;

	// types
	private:
		struct DefaultAttachment : CompileTime::FastCopyable
		{
		// variables
			Name_t				name;
			EPixelFormat::type	format	= EPixelFormat::Unknown;
			MultiSamples		samples;
			
		// methods
			DefaultAttachment () {}
			DefaultAttachment (StringCRef name, EPixelFormat::type fmt) : name(name), format(fmt) {}
			DefaultAttachment (StringCRef name, EPixelFormat::type fmt, uint samp) : name(name), format(fmt), samples(samp) {}
		};
		using DefAttachments_t	= FixedSizeArray< DefaultAttachment, GlobalConst::GAPI_MaxColorBuffers+1 >;
		using Self				= SimpleBuilder;


	// variables
	private:
		RenderPassDescrBuilder	_builder;
		DefAttachments_t		_attachments;
		bool					_hasDepth	= false;
		bool					_hasStencil	= false;


	// methods
	private:
		SimpleBuilder () {}

	public:
		SimpleBuilder (Self &&) = default;
		SimpleBuilder (const Self &) = default;

		Self&	Add (StringCRef name, EPixelFormat::type fmt, uint samp = 1)		{ return Add( name, fmt, MultiSamples(samp) ); }

		Self&	Add (StringCRef name, EPixelFormat::type fmt, MultiSamples samp = MultiSamples());

		RenderPassDescriptor const& Finish ();
	};

	inline RenderPassDescrBuilder::SimpleBuilder  RenderPassDescrBuilder::CreateForFramebuffer () {
		return SimpleBuilder();
	}


}	// Platforms


namespace CreateInfo
{

	//
	// GPU Render Pass Create Info
	//

	struct GpuRenderPass
	{
		ModulePtr							gpuThread;			// can be null
		Platforms::RenderPassDescriptor		descr;
	};

}	// CreateInfo


namespace GpuMsg
{

	//
	// Get Render Pass Descriptor
	//
	struct GetRenderPassDescriptor
	{
		Out< Platforms::RenderPassDescriptor >	result;
	};


}	// GpuMsg
}	// Engine
