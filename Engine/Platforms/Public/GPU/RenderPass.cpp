// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Public/GPU/RenderPass.h"
#include "Core/STL/Algorithms/StringParser.h"
#include "Engine/Platforms/Public/GPU/Enums.ToString.h"

namespace Engine
{
namespace Platforms
{
	
/*
=================================================
	ColorAttachment_t::ToString
=================================================
*/
	DEBUG_ONLY(
	String  RenderPassDescription::ColorAttachment_t::ToString () const
	{
		String	str;
		str << "ColorAttachment {";

		if ( IsEnabled() ) {
			str << "\n	name          = " << name
				<< "\n	format        = " << EPixelFormat::ToString( format )
				<< "\n	samples       = " << samples.Get()
				<< "\n	loadOp        = " << EAttachmentLoadOp::ToString( loadOp )
				<< "\n	storeOp       = " << EAttachmentStoreOp::ToString( storeOp )
				<< "\n	initialLayout = " << EImageLayout::ToString( initialLayout )
				<< "\n	finalLayout   = " << EImageLayout::ToString( finalLayout );
		}

		str << "\n}";
		return str;
	})
//-----------------------------------------------------------------------------
	

/*
=================================================
	DepthStencilAttachment_t::ToString
=================================================
*/
	DEBUG_ONLY(
	String  RenderPassDescription::DepthStencilAttachment_t::ToString () const
	{
		String	str;
		str << "DepthStencilAttachment {";

		if ( IsEnabled() ) {
			str << "\n	name           = " << name
				<< "\n	format         = " << EPixelFormat::ToString( format )
				<< "\n	samples        = " << samples.Get()
				<< "\n	loadOp         = " << EAttachmentLoadOp::ToString( loadOp )
				<< "\n	storeOp        = " << EAttachmentStoreOp::ToString( storeOp )
				<< "\n	stencilLoadOp  = " << EAttachmentLoadOp::ToString( stencilLoadOp )
				<< "\n	stencilStoreOp = " << EAttachmentStoreOp::ToString( stencilStoreOp )
				<< "\n	initialLayout  = " << EImageLayout::ToString( initialLayout )
				<< "\n	finalLayout    = " << EImageLayout::ToString( finalLayout );
		}

		str << "\n}";
		return str;
	})
//-----------------------------------------------------------------------------
	

/*
=================================================
	AttachmentRef_t::ToString
=================================================
*/
	DEBUG_ONLY(
	String  RenderPassDescription::AttachmentRef_t::ToString () const
	{
		String	str;
		str << "AttachmentRef {";

		if ( IsEnabled() ) {
			str << "\n	name    = " << name
				<< "\n	layout  = " << EImageLayout::ToString( layout );
		}

		str	<< "\n}";
		return str;
	})
//-----------------------------------------------------------------------------
	

/*
=================================================
	Subpass_t::ToString
=================================================
*/
	DEBUG_ONLY(
	String  RenderPassDescription::Subpass_t::ToString () const
	{
		const auto	AttachmentsRefs_ToString = LAMBDA() (const AttachmentsRef_t &arr)
		{{
			String	str;
			FOR( i, arr ) {
				str << (i ? ",\n" : "") << arr[i].ToString();
			}
			StringParser::IncreaceIndent( INOUT str, "\t\t" );
			return str;
		}};

		String	str;
		str << "Subpass {"
			<< "\n	name             = " << name;

		if ( not colors.Empty() ) {
			str << "\n	colorAttachments = {\n" << AttachmentsRefs_ToString( colors ) << "\n\t}";
		}

		if ( depthStencil.IsEnabled() ) {
			str << "\n	depthStencil     = " << depthStencil.ToString();
		}

		if ( not inputs.Empty() ) {
			str << "\n	inputs           = " << AttachmentsRefs_ToString( inputs ) << "\n\t}";
		}

		if ( resolve.IsEnabled() ) {
			str << "\n	resolve          = " << resolve.ToString();
		}

		if ( not preserves.Empty() ) {
			str << "\n	preserves        = { ";
			FOR( i, preserves ) {
				str << (i ? ", " : "") << preserves[i];
			}
			str << " }";
		}

		str << "\n}";
		return str;
	})
//-----------------------------------------------------------------------------


/*
=================================================
	SubpassDependency_t::ToString
=================================================
*/
	DEBUG_ONLY(
	String  RenderPassDescription::SubpassDependency_t::ToString () const
	{
		String	str;
		str << "SubpassDependency {"
			<< "\n	srcPass    = " << srcPass
			<< "\n	srcStage   = " << EPipelineStage::ToString( srcStage )
			<< "\n	srcAccess  = " << EPipelineAccess::ToString( srcAccess )
			<< "\n	dstPass    = " << dstPass
			<< "\n	dstStage   = " << EPipelineStage::ToString( dstStage )
			<< "\n	dstAccess  = " << EPipelineAccess::ToString( dstAccess )
			<< "\n	dependency = " << ESubpassDependency::ToString( dependency )
			<< "\n}";
		return str;
	})
//-----------------------------------------------------------------------------


/*
=================================================
	operator ==
=================================================
*/
	bool RenderPassDescription::operator == (const Self &right) const
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
	bool RenderPassDescription::operator >  (const Self &right) const
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
	ToString
=================================================
*/
	DEBUG_ONLY(
	String  RenderPassDescription::ToString () const
	{
		String	str = "RenderPass {\n";

		if ( not _colorAttachmens.Empty() )
		{
			str << "\tcolorAttachmens = {\n";
			FOR( i, _colorAttachmens ) {
				str << (i ? ",\n" : "") << _colorAttachmens[i].ToString();
			}
			str << "\t}\n";
		}

		if ( _depthStencilAttachment.IsEnabled() )
		{
			str << "\tdepthStencilAttachment = "
				<< _depthStencilAttachment.ToString();
		}

		if ( not _subpasses.Empty() )
		{
			str << "\tsubpasses = {\n";
			FOR( i, _subpasses ) {
				str << (i ? ",\n" : "") << _subpasses[i].ToString();
			}
			str << "\t}\n";
		}

		if ( not _dependencies.Empty() )
		{
			str << "\tdependencies = {\n";
			FOR( i, _dependencies ) {
				str << (i ? ",\n" : "") << _dependencies[i].ToString();
			}
			str << "\t}\n";
		}

		return str;
	})
//-----------------------------------------------------------------------------



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
		RenderPassDescrBuilder::SubpassBuilder::AddColorAttachment (StringCRef			name,
																	EImageLayout::type	layout)
	{
		AttachmentRef_t		tmp;
		tmp.name	= name;
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
		RenderPassDescrBuilder::SubpassBuilder::SetDepthStencilAttachment (StringCRef			name,
																		   EImageLayout::type	layout)
	{
		AttachmentRef_t		tmp;
		tmp.name	= name;
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
		RenderPassDescrBuilder::SubpassBuilder::AddInputAttachment (StringCRef			name,
																	EImageLayout::type	layout)
	{
		AttachmentRef_t		tmp;
		tmp.name	= name;
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
		RenderPassDescrBuilder::SubpassBuilder::SetResolveAttachment (StringCRef			name,
																	  EImageLayout::type	layout)
	{
		AttachmentRef_t		tmp;
		tmp.name	= name;
		tmp.layout	= layout;

		return SetResolveAttachment( tmp );
	}
	
/*
=================================================
	AddPreserveAttachment
=================================================
*/
	RenderPassDescrBuilder::SubpassBuilder& 
		RenderPassDescrBuilder::SubpassBuilder::AddPreserveAttachment (StringCRef name)
	{
		_valueRef.preserves.PushBack( name );
		return *this;
	}
//-----------------------------------------------------------------------------
	


/*
=================================================
	AddColorAttachment
=================================================
*/
	RenderPassDescrBuilder&
		RenderPassDescrBuilder::AddColorAttachment (const ColorAttachment_t &value)
	{
		CHECK_ERR( value.finalLayout != EImageLayout::Preinitialized and value.finalLayout != EImageLayout::Undefined, *this );

		_changed = true;
		_state._colorAttachmens.PushBack( value );
		return *this;
	}

	RenderPassDescrBuilder&
		RenderPassDescrBuilder::AddColorAttachment (StringCRef					name,
													EPixelFormat::type			format,
													MultiSamples				samples,
													EAttachmentLoadOp::type		loadOp,
													EAttachmentStoreOp::type	storeOp,
													EImageLayout::type			initialLayout,
													EImageLayout::type			finalLayout)
	{
		ColorAttachment_t	tmp;
		tmp.name			= name;
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
		RenderPassDescrBuilder::SetDepthStencilAttachment (StringCRef				name,
														   EPixelFormat::type		format,
														   MultiSamples				samples,
														   EAttachmentLoadOp::type	depthLoadOp,
														   EAttachmentStoreOp::type depthStoreOp,
														   EAttachmentLoadOp::type	stencilLoadOp,
														   EAttachmentStoreOp::type stencilStoreOp,
														   EImageLayout::type		initialLayout,
														   EImageLayout::type		finalLayout)
	{
		DepthStencilAttachment_t	tmp;
		tmp.name			= name;
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

	RenderPassDescrBuilder::SubpassBuilder  RenderPassDescrBuilder::AddSubpass (StringCRef name)
	{
		_changed = true;
		_state._subpasses.PushBack( Subpass_t{ name } );
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
		RenderPassDescrBuilder::AddDependency (StringCRef				srcPass,
											   EPipelineStage::bits		srcStage,
											   EPipelineAccess::bits	srcAccess,
											   StringCRef				dstPass,
											   EPipelineStage::bits		dstStage,
											   EPipelineAccess::bits	dstAccess,
											   ESubpassDependency::bits	dependency)
	{
		SubpassDependency_t		tmp;
		tmp.srcPass		= srcPass;
		tmp.srcStage	= srcStage;
		tmp.srcAccess	= srcAccess;
		tmp.dstPass		= dstPass;
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
	RenderPassDescription const&  RenderPassDescrBuilder::Finish ()
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
	
/*
=================================================
	CreateForSurface
=================================================
*/
	RenderPassDescription  RenderPassDescrBuilder::CreateForSurface (EPixelFormat::type colorFmt, EPixelFormat::type depthStencilFmt, EImageLayout::type finalLayout)
	{
		Self		builder;
		auto		subpass	= builder.AddSubpass("pass");

		// depth attachment
		if ( depthStencilFmt != EPixelFormat::Unknown )
		{
			DepthStencilAttachment_t	attach;
			AttachmentRef_t				attach_ref;
			
			attach.name				= "depth-stencil";
			attach.format			= depthStencilFmt;
			attach.loadOp			= EAttachmentLoadOp::Clear;
			attach.storeOp			= EAttachmentStoreOp::Store;
			attach.stencilLoadOp	= EAttachmentLoadOp::Invalidate;
			attach.stencilStoreOp	= EAttachmentStoreOp::Invalidate;
			attach.initialLayout	= EImageLayout::Undefined;
			attach.finalLayout		= EImageLayout::DepthStencilAttachmentOptimal;
			builder.SetDepthStencilAttachment( attach );

			attach_ref.name			= attach.name;
			attach_ref.layout		= EImageLayout::DepthStencilAttachmentOptimal;
			subpass.SetDepthStencilAttachment( attach_ref );
		}

		// color attachment
		{
			ColorAttachment_t		attach;
			AttachmentRef_t			attach_ref;
			
			attach.name				= "Color0";
			attach.format			= colorFmt;
			attach.loadOp			= EAttachmentLoadOp::Clear;
			attach.storeOp			= EAttachmentStoreOp::Store;
			attach.initialLayout	= EImageLayout::Undefined;
			attach.finalLayout		= finalLayout;
			builder.AddColorAttachment( attach );

			attach_ref.name			= attach.name;
			attach_ref.layout		= EImageLayout::ColorAttachmentOptimal;
			subpass.AddColorAttachment( attach_ref );
		}

		// dependency
		{
			SubpassDependency_t	dep;

			dep.srcPass		= "external";
			dep.srcStage	= EPipelineStage::BottomOfPipe;
			dep.srcAccess	= EPipelineAccess::MemoryRead;
			dep.dstPass		= "pass";
			dep.dstStage	= EPipelineStage::ColorAttachmentOutput;
			dep.dstAccess	= EPipelineAccess::ColorAttachmentRead | EPipelineAccess::ColorAttachmentWrite;
			dep.dependency	= ESubpassDependency::ByRegion;
			builder.AddDependency( dep );

			dep.srcPass		= "pass";
			dep.srcStage	= EPipelineStage::ColorAttachmentOutput;
			dep.srcAccess	= EPipelineAccess::ColorAttachmentRead | EPipelineAccess::ColorAttachmentWrite;
			dep.dstPass		= "external";
			dep.dstStage	= EPipelineStage::BottomOfPipe;
			dep.dstAccess	= EPipelineAccess::MemoryRead;
			dep.dependency	= ESubpassDependency::ByRegion;
			builder.AddDependency( dep );
		}

		return builder.Finish();
	}
//-----------------------------------------------------------------------------

	

/*
=================================================
	SimpleBuilder::Add
=================================================
*/
	RenderPassDescrBuilder::SimpleBuilder&
		RenderPassDescrBuilder::SimpleBuilder::Add (StringCRef name, EPixelFormat::type format, MultiSamples samples)
	{
		if ( _builder._state.Subpasses().Empty() )
			_builder.AddSubpass("pass-1");

		SubpassBuilder	subpass{ _builder._state._subpasses.Back() };

		if ( EPixelFormat::HasDepthOrStencil( format ) )
		{
			CHECK_ERR( not _hasDepth and not _hasStencil, *this );

			_hasDepth	= EPixelFormat::HasDepth( format );
			_hasStencil = EPixelFormat::HasStencil( format );

			subpass.SetDepthStencilAttachment( name, EImageLayout::DepthStencilAttachmentOptimal );

			_builder.SetDepthStencilAttachment(	name,
												format,
												samples,
												_hasDepth ? EAttachmentLoadOp::Clear : EAttachmentLoadOp::Invalidate,
												_hasDepth ? EAttachmentStoreOp::Store : EAttachmentStoreOp::Invalidate,
												_hasStencil ? EAttachmentLoadOp::Clear : EAttachmentLoadOp::Invalidate,
												_hasStencil ? EAttachmentStoreOp::Store : EAttachmentStoreOp::Invalidate,
												EImageLayout::Undefined,
												EImageLayout::DepthStencilAttachmentOptimal );
		}
		else
		{
			subpass.AddColorAttachment( name, EImageLayout::ColorAttachmentOptimal );

			_builder.AddColorAttachment( name,
										 format,
										 samples,
										 EAttachmentLoadOp::Clear,
										 EAttachmentStoreOp::Store,
										 EImageLayout::Undefined,
										 EImageLayout::ShaderReadOnlyOptimal );
		}
		return *this;
	}
	
/*
=================================================
	SimpleBuilder::Finish
=================================================
*/
	RenderPassDescription const&  RenderPassDescrBuilder::SimpleBuilder::Finish ()
	{
		CHECK( not _builder._state.Subpasses().Empty() );

		// add dependency
		if ( not _builder._state.Subpasses().Empty() and
			 _builder._state.Dependencies().Empty() )
		{
			SubpassDependency_t	dep;

			dep.srcPass		= "external";
			dep.srcStage	= EPipelineStage::BottomOfPipe;
			dep.srcAccess	= EPipelineAccess::MemoryRead;
			dep.dstPass		= _builder._state._subpasses.Front().name;
			dep.dstStage	= EPipelineStage::ColorAttachmentOutput;
			dep.dstAccess	= EPipelineAccess::ColorAttachmentRead | EPipelineAccess::ColorAttachmentWrite;
			dep.dependency	= ESubpassDependency::ByRegion;
			_builder.AddDependency( dep );
		
			dep.srcPass		= _builder._state._subpasses.Front().name;
			dep.srcStage	= EPipelineStage::ColorAttachmentOutput;
			dep.srcAccess	= EPipelineAccess::ColorAttachmentRead | EPipelineAccess::ColorAttachmentWrite;
			dep.dstPass		= "external";
			dep.dstStage	= EPipelineStage::BottomOfPipe;
			dep.dstAccess	= EPipelineAccess::MemoryRead;
			dep.dependency	= ESubpassDependency::ByRegion;
			_builder.AddDependency( dep );
		}

		return _builder.Finish();
	}

}	// Platforms
}	// Engine
