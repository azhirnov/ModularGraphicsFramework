// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Public/Tools/SamplerUtils.h"

namespace Engine
{
namespace PlatformTools
{

/*
=================================================
	ValidateDescriptor
=================================================
*/
	void SamplerUtils::ValidateDescriptor (INOUT SamplerDescriptor &descr, uint maxAnisotropyLevel)
	{
		SamplerDescriptor::Builder	builder( descr );

		const bool		unnorm_coords = (builder.AddressMode().x == EAddressMode::ClampUnnorm) or
										(builder.AddressMode().y == EAddressMode::ClampUnnorm);

		// validate filtering mode
		{
			// validate anisotropic filtering
			if ( EnumEq( builder.Filter(), EFilter::_ANISOTROPIC ) )
			{
				const bool	support_anisotropy = (maxAnisotropyLevel > 0);

				builder.SetFilter( EFilter::type( (builder.Filter() ^ EFilter::_A_FACTOR_MASK) |
												Clamp(	uint(builder.Filter() & EFilter::_A_FACTOR_MASK),
														uint(support_anisotropy) * 2,
														maxAnisotropyLevel ) ) );
			}

			// validate for unnormalized coords
			if ( unnorm_coords )
			{
				ASSERT( not EFilter::IsMipmapLinear( builder.Filter() ) );
				ASSERT( EFilter::IsMinLinear( builder.Filter() ) == EFilter::IsMagLinear( builder.Filter() ) );
				ASSERT( not EnumEq( builder.Filter(), EFilter::_ANISOTROPIC ) );

				builder.SetFilter( EFilter::_MIP_NEAREST |
									(EFilter::IsMinLinear( builder.Filter() ) or EFilter::IsMagLinear( builder.Filter() ) ?
										EFilter::_MIN_LINEAR | EFilter::_MAG_LINEAR :
										EFilter::_MIN_NEAREST | EFilter::_MAG_NEAREST) );
			}
		}

		// validate addressing mode
		{
			FOR( i, builder.AddressMode() )
			{
				if ( unnorm_coords )
				{
					// unnormalized coords supports only with ClampToEdge addressing mode
					ASSERT( i == 2 or builder.AddressMode()[i] == EAddressMode::ClampUnnorm );

					builder.SetAddressMode( uint(i), EAddressMode::ClampUnnorm );
				}
			}
		}

		// validate compare mode
		{
			if ( unnorm_coords )
			{
				ASSERT( builder.CompareOp() == ECompareFunc::None );
				builder.SetCompareOp( ECompareFunc::None );
			}
		}

		descr = builder.Finish();
	}

}	// PlatformTools
}	// Engine
