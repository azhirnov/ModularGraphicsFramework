// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Public/GPU/Framebuffer.h"
#include "Core/STL/Algorithms/StringParser.h"

namespace Engine
{
namespace Platforms
{

/*
=================================================
	ColorImage
=================================================
*/
	FramebufferDescription&  FramebufferDescription::ColorImage (StringCRef name, EImage::type imageType)
	{
		colorAttachments.PushBack({ name, imageType, false });
		return *this;
	}
	
/*
=================================================
	ColorRenderbuffer
=================================================
*/
	FramebufferDescription&  FramebufferDescription::ColorRenderbuffer (StringCRef name, EImage::type imageType)
	{
		colorAttachments.PushBack({ name, imageType, true });
		return *this;
	}
	
/*
=================================================
	DepthStencilImage
=================================================
*/
	FramebufferDescription&  FramebufferDescription::DepthStencilImage (StringCRef name, EImage::type imageType)
	{
		depthStencilAttachment = { name, imageType, false };
		return *this;
	}
	
/*
=================================================
	DepthStencilRenderbuffer
=================================================
*/
	FramebufferDescription&  FramebufferDescription::DepthStencilRenderbuffer (StringCRef name, EImage::type imageType)
	{
		depthStencilAttachment = { name, imageType, true };
		return *this;
	}
	
/*
=================================================
	ToString
=================================================
*/
	DEBUG_ONLY(
	String  FramebufferDescription::ToString () const
	{
		String	str;
		str << "Framebuffer {"
			<< "\n	size:       " << size.x << 'x' << size.y
			<< "\n	layers:     " << layers
			<< "\n	samples:    " << samples.Get();

		if ( colorAttachments.Empty() )
		{
			str << "\n	color attachments = {\n";

			for (auto& col : colorAttachments) {
				str << col.ToString() << '\n';
			}
			str << "\n}";
		}

		if ( depthStencilAttachment.IsEnabled() )
		{
			str << "\n	depth-stencil attachment = \n" << depthStencilAttachment.ToString();
		}

		str << "\n}\n";
		return str;
	})
		
/*
=================================================
	AttachmentInfo::ToString
=================================================
*/
	DEBUG_ONLY(
	String  FramebufferDescription::AttachmentInfo::ToString () const
	{
		String	str;

		str << "\tAttachment {"
			<< "\t\n	name:       " << name
			<< "\t\n	imageType:  " << EImage::ToString( imageType )
			<< "\t\n}";

		return str;
	})


}	// Platforms
}	// Engine
