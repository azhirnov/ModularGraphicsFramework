// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/RenderPass.h"

namespace Engine
{
namespace Platforms
{
	
/*
=================================================
	operator ==
=================================================
*/
	bool RenderPassDescriptor::operator == (const Self &right) const
	{
		return	this->_hash == right._hash																			and
				BinArrayCRef::From( _colorAttachmens ).BinEquals( BinArrayCRef::From( right._colorAttachmens ) )	and
				BinArrayCRef::From( _dependencies ).BinEquals( BinArrayCRef::From( right._dependencies ) )			and
				BinArrayCRef::From( _subpasses ).BinEquals( BinArrayCRef::From( right._subpasses ) )				and
				MemCmp( _depthStencilAttachment, right._depthStencilAttachment ) == 0;
	}
	
/*
=================================================
	operator >
=================================================
*/
	bool RenderPassDescriptor::operator >  (const Self &right) const
	{
		return
			this->_hash				 != right._hash						?	this->_hash				 > right._hash						:
			_colorAttachmens.Count() != right._colorAttachmens.Count()	?	_colorAttachmens.Count() > right._colorAttachmens.Count()	:
			_dependencies.Count()	 != right._dependencies.Count()		?	_dependencies.Count()	 > right._dependencies.Count()		:
			_subpasses.Count()		 != right._subpasses.Count()		?	_subpasses.Count()		 > right._subpasses.Count()			:
																	MemCmp( _depthStencilAttachment, right._depthStencilAttachment ) > 0;
	}
	
/*
=================================================
	operator <
=================================================
*/
	bool RenderPassDescriptor::operator <  (const Self &right) const
	{
		return	
			this->_hash				 != right._hash						?	this->_hash				 < right._hash						:
			_colorAttachmens.Count() != right._colorAttachmens.Count()	?	_colorAttachmens.Count() < right._colorAttachmens.Count()	:
			_dependencies.Count()	 != right._dependencies.Count()		?	_dependencies.Count()	 < right._dependencies.Count()		:
			_subpasses.Count()		 != right._subpasses.Count()		?	_subpasses.Count()		 < right._subpasses.Count()			:
																	MemCmp( _depthStencilAttachment, right._depthStencilAttachment ) < 0;
	}


/*
=================================================
	AddColorAttachment
=================================================
*/
	RenderPassDescrBuilder::SubpassBuilder&
		RenderPassDescrBuilder::SubpassBuilder::AddColorAttachment (const AttachmentRef_t &value)
	{
		_valueRef.colors.PushBack( value );
		return *this;
	}

	RenderPassDescrBuilder::SubpassBuilder&
		RenderPassDescrBuilder::SubpassBuilder::AddColorAttachment (uint				index,
																	EImageLayout::type	layout)
	{
		AttachmentRef_t		tmp;
		tmp.index	= RenderPassDescriptor::AttachmentIndex(index);
		tmp.layout	= layout;

		return AddColorAttachment( tmp );
	}
		
/*
=================================================
	SetDepthStencilAttachment
=================================================
*/	
	RenderPassDescrBuilder::SubpassBuilder& 
		RenderPassDescrBuilder::SubpassBuilder::SetDepthStencilAttachment (const AttachmentRef_t &value)
	{
		_valueRef.depthStencil = value;
		return *this;
	}

	RenderPassDescrBuilder::SubpassBuilder& 
		RenderPassDescrBuilder::SubpassBuilder::SetDepthStencilAttachment (uint					index,
																		   EImageLayout::type	layout)
	{
		AttachmentRef_t		tmp;
		tmp.index	= RenderPassDescriptor::AttachmentIndex(index);
		tmp.layout	= layout;

		return SetDepthStencilAttachment( tmp );
	}
	
/*
=================================================
	AddInputAttachment
=================================================
*/
	RenderPassDescrBuilder::SubpassBuilder& 
		RenderPassDescrBuilder::SubpassBuilder::AddInputAttachment (const AttachmentRef_t &value)
	{
		_valueRef.inputs.PushBack( value );
		return *this;
	}

	RenderPassDescrBuilder::SubpassBuilder& 
		RenderPassDescrBuilder::SubpassBuilder::AddInputAttachment (uint				index,
																	EImageLayout::type	layout)
	{
		AttachmentRef_t		tmp;
		tmp.index	= RenderPassDescriptor::AttachmentIndex(index);
		tmp.layout	= layout;

		return AddInputAttachment( tmp );
	}
	
/*
=================================================
	SetResolveAttachment
=================================================
*/
	RenderPassDescrBuilder::SubpassBuilder& 
		RenderPassDescrBuilder::SubpassBuilder::SetResolveAttachment (const AttachmentRef_t &value)
	{
		_valueRef.resolve = value;
		return *this;
	}

	RenderPassDescrBuilder::SubpassBuilder& 
		RenderPassDescrBuilder::SubpassBuilder::SetResolveAttachment (uint					index,
																	  EImageLayout::type	layout)
	{
		AttachmentRef_t		tmp;
		tmp.index	= RenderPassDescriptor::AttachmentIndex(index);
		tmp.layout	= layout;

		return SetResolveAttachment( tmp );
	}
	
/*
=================================================
	AddPreserveAttachment
=================================================
*/
	RenderPassDescrBuilder::SubpassBuilder& 
		RenderPassDescrBuilder::SubpassBuilder::AddPreserveAttachment (uint index)
	{
		_valueRef.preserves.PushBack( RenderPassDescriptor::AttachmentIndex( index ) );
		return *this;
	}
	

/*
=================================================
	AddColorAttachment
=================================================
*/
	RenderPassDescrBuilder&
		RenderPassDescrBuilder::AddColorAttachment (const ColorAttachment_t &value)
	{
		_changed = true;
		_state._colorAttachmens.PushBack( value );
		return *this;
	}

	RenderPassDescrBuilder&
		RenderPassDescrBuilder::AddColorAttachment (EPixelFormat::type			format,
													MultiSamples				samples,
													EAttachmentLoadOp::type		loadOp,
													EAttachmentStoreOp::type	storeOp,
													EImageLayout::type			initialLayout,
													EImageLayout::type			finalLayout)
	{
		ColorAttachment_t	tmp;
		tmp.format			= format;
		tmp.samples			= samples;
		tmp.loadOp			= loadOp;
		tmp.storeOp			= storeOp;
		tmp.initialLayout	= initialLayout;
		tmp.finalLayout		= finalLayout;

		return AddColorAttachment( tmp );
	}
	
/*
=================================================
	SetDepthStencilAttachment
=================================================
*/
	RenderPassDescrBuilder&
		RenderPassDescrBuilder::SetDepthStencilAttachment (const DepthStencilAttachment_t &value)
	{
		_changed = true;
		_state._depthStencilAttachment = value;
		return *this;
	}

	RenderPassDescrBuilder&
		RenderPassDescrBuilder::SetDepthStencilAttachment (EPixelFormat::type		format,
														   MultiSamples				samples,
														   EAttachmentLoadOp::type	depthLoadOp,
														   EAttachmentStoreOp::type depthStoreOp,
														   EAttachmentLoadOp::type	stencilLoadOp,
														   EAttachmentStoreOp::type stencilStoreOp,
														   EImageLayout::type		initialLayout,
														   EImageLayout::type		finalLayout)
	{
		DepthStencilAttachment_t	tmp;
		tmp.format			= format;
		tmp.samples			= samples;
		tmp.loadOp			= depthLoadOp;
		tmp.storeOp			= depthStoreOp;
		tmp.stencilLoadOp	= stencilLoadOp;
		tmp.stencilStoreOp	= stencilStoreOp;
		tmp.initialLayout	= initialLayout;
		tmp.finalLayout		= finalLayout;

		return SetDepthStencilAttachment( tmp );
	}
	
/*
=================================================
	AddSubpass
=================================================
*/
	RenderPassDescrBuilder&  RenderPassDescrBuilder::AddSubpass (const Subpass_t &value)
	{
		_changed = true;
		_state._subpasses.PushBack( value );
		return *this;
	}

	RenderPassDescrBuilder::SubpassBuilder  RenderPassDescrBuilder::AddSubpass ()
	{
		_changed = true;
		_state._subpasses.PushBack( Uninitialized );
		return SubpassBuilder( _state._subpasses.Back() );
	}
	
/*
=================================================
	AddDependency
=================================================
*/
	RenderPassDescrBuilder&  RenderPassDescrBuilder::AddDependency (const SubpassDependency_t &value)
	{
		_changed = true;
		_state._dependencies.PushBack( value );
		return *this;
	}

	RenderPassDescrBuilder&
		RenderPassDescrBuilder::AddDependency (uint						srcPass,
											   EPipelineStage::bits		srcStage,
											   ESubpassAccess::bits		srcAccess,
											   uint						dstPass,
											   EPipelineStage::bits		dstStage,
											   ESubpassAccess::bits		dstAccess,
											   ESubpassDependency::bits	dependency)
	{
		SubpassDependency_t		tmp;
		tmp.srcPass		= RenderPassDescriptor::SubpassIndex( srcPass );
		tmp.srcStage	= srcStage;
		tmp.srcAccess	= srcAccess;
		tmp.dstPass		= RenderPassDescriptor::SubpassIndex( dstPass );
		tmp.dstStage	= dstStage;
		tmp.dstAccess	= dstAccess;
		tmp.dependency	= dependency;

		return AddDependency( tmp );
	}
	
/*
=================================================
	Finish
=================================================
*/
	RenderPassDescriptor const&  RenderPassDescrBuilder::Finish ()
	{
		if ( _changed )
		{
			_changed = false;

			HashResult	hash;
			
			hash += HashOf( BinArrayCRef::From( _state._colorAttachmens ) );
			hash += HashOf( BinArrayCRef::FromValue( _state._depthStencilAttachment ) );
			hash += HashOf( BinArrayCRef::From( _state._dependencies ) );
			hash += HashOf( BinArrayCRef::From( _state._subpasses ) );

			_state._hash = hash;
		}

		return _state;
	}


}	// Platforms
}	// Engine
