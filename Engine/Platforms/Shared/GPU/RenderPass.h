// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/RenderPassEnums.h"
#include "Engine/Platforms/Shared/GPU/TextureEnums.h"
#include "Engine/Platforms/Shared/GPU/PixelFormatEnums.h"
#include "Engine/Platforms/Shared/GPU/MultiSamples.h"

namespace Engine
{
namespace Platforms
{
	
	//
	// Render Pass Descriptor
	//
	
	struct RenderPassDescriptor : CompileTime::FastCopyable
	{
		friend struct RenderPassDescrBuilder;

	// types
	public:
		using Self	= RenderPassDescriptor;

		enum AttachmentIndex : uint {};
		enum SubpassIndex : uint { SubpassIndexExternal = uint(1u << 31) };

		static const uint	MAX_COLOR_ATTACHMENTS	= GlobalConst::Graphics_MaxColorBuffers;
		static const uint	MAX_SUBPASSES			= GlobalConst::Graphics_MaxRenderPassSubpasses;
		static const uint	MAX_DEPENDENCIES		= MAX_SUBPASSES * 2;


		//
		// Color Attachment
		//
		struct ColorAttachment_t : CompileTime::PODStruct
		{
		// variables
			EPixelFormat::type			format;
			MultiSamples				samples;
			EAttachmentLoadOp::type		loadOp;			// for color and depth
			EAttachmentStoreOp::type	storeOp;		// for color and depth
			EImageLayout::type			initialLayout;
			EImageLayout::type			finalLayout;

		// methods
			ColorAttachment_t (GX_DEFCTOR) :
				format( EPixelFormat::Unknown ),			samples(),
				loadOp( EAttachmentLoadOp::None ),			storeOp( EAttachmentStoreOp::None ),
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
				stencilLoadOp( EAttachmentLoadOp::None ),	stencilStoreOp( EAttachmentStoreOp::None )
			{}

			bool IsEnabled () const		{ return format != EPixelFormat::Unknown; }
		};


		//
		// Attachment Reference
		//
		struct AttachmentRef_t : CompileTime::PODStruct
		{
		// variables
			AttachmentIndex		index;
			EImageLayout::type	layout;


		// methods
			AttachmentRef_t (GX_DEFCTOR) :
				index( AttachmentIndex(-1) ),	layout( EImageLayout::Undefined )
			{}

			bool IsEnabled () const		{ return index != AttachmentIndex(-1); }
		};

		using AttachmentsRef_t		= FixedSizeArray< AttachmentRef_t, MAX_COLOR_ATTACHMENTS >;
		using PreserveAttachments_t	= FixedSizeArray< AttachmentIndex, MAX_COLOR_ATTACHMENTS >;


		//
		// Subpass
		//
		struct Subpass_t : CompileTime::PODStruct
		{
		// variables
			AttachmentsRef_t		colors;
			AttachmentRef_t			depthStencil;
			AttachmentsRef_t		inputs;
			AttachmentRef_t			resolve;
			PreserveAttachments_t	preserves;
			
		// methods
			Subpass_t (GX_DEFCTOR)
			{}
		};


		//
		// Subpass Dependency
		//
		struct SubpassDependency_t : CompileTime::PODStruct
		{
		// variables
			SubpassIndex				srcPass;
			EPipelineStage::bits		srcStage;
			ESubpassAccess::bits		srcAccess;

			SubpassIndex				dstPass;
			EPipelineStage::bits		dstStage;
			ESubpassAccess::bits		dstAccess;

			ESubpassDependency::bits	dependency;

		// methods
			SubpassDependency_t (GX_DEFCTOR) :
				srcPass( SubpassIndex(-1) ),	dstPass( SubpassIndex(-1) )
			{}
		};

		using ColorAttachments_t	= FixedSizeArray< ColorAttachment_t, MAX_COLOR_ATTACHMENTS >;
		using Subpasses_t			= FixedSizeArray< Subpass_t, MAX_SUBPASSES >;
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
		//HashResult						GetHash ()					const	{ return _hash; }

		// compare without hash
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

	struct RenderPassDescrBuilder
	{
	// types
	public:
		using Self						= RenderPassDescrBuilder;
		using Subpass_t					= RenderPassDescriptor::Subpass_t;
		using AttachmentRef_t			= RenderPassDescriptor::AttachmentRef_t;
		using ColorAttachment_t			= RenderPassDescriptor::ColorAttachment_t;
		using DepthStencilAttachment_t	= RenderPassDescriptor::DepthStencilAttachment_t;
		using SubpassDependency_t		= RenderPassDescriptor::SubpassDependency_t;
		using SubpassIndex				= RenderPassDescriptor::SubpassIndex;
		using AttachmentIndex			= RenderPassDescriptor::AttachmentIndex;

		static const SubpassIndex	SubpassIndexExternal	= RenderPassDescriptor::SubpassIndexExternal;


		struct SubpassBuilder
		{
		// variables
		private:
			Subpass_t &	_valueRef;

		// methods
		public:
			SubpassBuilder (Subpass_t &value) : _valueRef(value) {}
			
			SubpassBuilder& AddColorAttachment (const AttachmentRef_t &value);
			SubpassBuilder& AddColorAttachment (uint				   index,
												EImageLayout::type	   layout);
			
			SubpassBuilder& SetDepthStencilAttachment (const AttachmentRef_t &value);
			SubpassBuilder& SetDepthStencilAttachment (uint					  index,
													   EImageLayout::type	  layout);

			SubpassBuilder& AddInputAttachment (const AttachmentRef_t &value);
			SubpassBuilder& AddInputAttachment (uint				   index,
												EImageLayout::type	   layout);

			SubpassBuilder& SetResolveAttachment (const AttachmentRef_t &value);
			SubpassBuilder& SetResolveAttachment (uint					 index,
												  EImageLayout::type	 layout);

			SubpassBuilder& AddPreserveAttachment (uint value);
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
		Self& AddColorAttachment (EPixelFormat::type		format,
								  MultiSamples				samples,
								  EAttachmentLoadOp::type	loadOp,
								  EAttachmentStoreOp::type	storeOp,
								  EImageLayout::type		initialLayout,
								  EImageLayout::type		finalLayout);

		Self& SetDepthStencilAttachment (const DepthStencilAttachment_t &value);
		Self& SetDepthStencilAttachment (EPixelFormat::type			format,
										 MultiSamples				samples,
										 EAttachmentLoadOp::type	depthLoadOp,
										 EAttachmentStoreOp::type	depthStoreOp,
										 EAttachmentLoadOp::type	stencilLoadOp,
										 EAttachmentStoreOp::type	stencilStoreOp,
										 EImageLayout::type			initialLayout,
										 EImageLayout::type			finalLayout);

		Self& AddSubpass (const Subpass_t &value);
		SubpassBuilder AddSubpass ();

		Self& AddDependency (const SubpassDependency_t &value);
		Self& AddDependency (uint						srcPass,
							 EPipelineStage::bits		srcStage,
							 ESubpassAccess::bits		srcAccess,
							 uint						dstPass,
							 EPipelineStage::bits		dstStage,
							 ESubpassAccess::bits		dstAccess,
							 ESubpassDependency::bits	dependency);

		// validate, calculate hash and return
		RenderPassDescriptor const& Finish ();
	};

}	// Platforms


namespace CreateInfo
{

	//
	// GPU Render Pass Create Info
	//

	struct GpuRenderPass
	{
		ModulePtr							gpuThread;
		Platforms::RenderPassDescriptor		descr;
	};

}	// CreateInfo


namespace ModuleMsg
{

	//
	// Get GPU Render Pass Descriptor
	//
	struct GetGpuRenderPassDescriptor
	{
		Out< Platforms::RenderPassDescriptor >	result;
	};
	

	// platform-dependent
	struct GetGpuRenderPassID;


}	// ModuleMsg
}	// Engine
