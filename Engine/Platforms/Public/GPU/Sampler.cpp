// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Public/GPU/Sampler.h"
#include "Engine/Platforms/Public/GPU/Enums.ToString.h"

namespace Engine
{
namespace Platforms
{
	
/*
=================================================
	constructor
=================================================
*/
	SamplerDescriptor::SamplerDescriptor (UninitializedType) :
		_addressMode( EAddressMode::Clamp ),
		_filter( EFilter::MinMagMipLinear ),
		_compareOp( ECompareFunc::None )
		//_mipLodBias( 0.0f ),
		//_lodRange( 0.0f )
	{}
	
/*
=================================================
	constructor
=================================================
*/
	SamplerDescriptor::SamplerDescriptor (EAddressMode::type addressMode,
										  EFilter::type filter,
										  ESamplerBorderColor::bits borderColor,
										  //float mipLodBias = 0.0f,
										  //const float2 &lodRange = float2(),
										  ECompareFunc::type compareOp ) :
		_addressMode( addressMode ),	_filter( filter ),
		_borderColor( borderColor ),	_compareOp( compareOp )
		//_mipLodBias( mipLodBias ),	_lodRange( lodRange )
	{}
	
/*
=================================================
	operator ==
=================================================
*/
	bool SamplerDescriptor::operator == (const Self &right) const
	{
		return	this->_hash			== right._hash			and
				All( _addressMode	== right._addressMode )	and
				All( _filter		== right._filter )		and
				_borderColor		== right._borderColor	and
				_compareOp			== right._compareOp;
	}
	
/*
=================================================
	:operator >
=================================================
*/
	bool SamplerDescriptor::operator >  (const Self &right) const
	{
		return
			this->_hash		!= right._hash			?	this->_hash		> right._hash			:
			_addressMode.x	!= right._addressMode.x	?	_addressMode.x	> right._addressMode.x	:
			_addressMode.y	!= right._addressMode.y	?	_addressMode.y	> right._addressMode.y	:
			_addressMode.z	!= right._addressMode.z	?	_addressMode.z	> right._addressMode.z	:
			_filter			!= right._filter		?	_filter			> right._filter			:
			_borderColor	!= right._borderColor	?	_borderColor	> right._borderColor	:
														_compareOp		> right._compareOp;
	}
	
/*
=================================================
	ToString
=================================================
*/
	DEBUG_ONLY(
	String  SamplerDescriptor::ToString () const
	{
		String	str("Sampler {\n");

		str << "\taddressMode: ";

		FOR( i, _addressMode )
		{
			str << (i > 0 ? ", " : "") << EAddressMode::ToString( _addressMode[i] );
		}

		str << "\n";
		str << "\tfilter:      " << EFilter::ToString( _filter ) << "\n";
		str << "\tcompareOp:   " << ECompareFunc::ToString( _compareOp ) << "\n";
		str << "\tborderColor  " << ESamplerBorderColor::ToString( _borderColor ) << "\n";
		str << "}";

		return str;
	})
		

		
/*
=================================================
	SetAddressMode
=================================================
*/
	SamplerDescriptor::Builder&  SamplerDescriptor::Builder::SetAddressMode (EAddressMode::type u, EAddressMode::type v, EAddressMode::type w)
	{
		_state._addressMode.x = u;
		_state._addressMode.y = v;
		_state._addressMode.z = w;
		_changed = true;
		return *this;
	}

	SamplerDescriptor::Builder&  SamplerDescriptor::Builder::SetAddressMode (EAddressMode::type uvw)
	{
		return SetAddressMode( uvw, uvw, uvw );
	}
	
	SamplerDescriptor::Builder&  SamplerDescriptor::Builder::SetAddressMode (uint index, EAddressMode::type value)
	{
		_state._addressMode[index] = value;
		_changed = true;
		return *this;
	}

	SamplerDescriptor::Builder&  SamplerDescriptor::Builder::SetAddressMode (const AddressMode_t &uvw)
	{
		return SetAddressMode( uvw.x, uvw.y, uvw.z );
	}
	
/*
=================================================
	SetFilter
=================================================
*/
	SamplerDescriptor::Builder&  SamplerDescriptor::Builder::SetFilter (EFilter::type value)
	{
		_state._filter = value;
		_changed = true;
		return *this;
	}
	
/*
=================================================
	SetBorderColor
=================================================
*/
	SamplerDescriptor::Builder&  SamplerDescriptor::Builder::SetBorderColor (ESamplerBorderColor::bits value)
	{
		_state._borderColor = value;
		_changed = true;
		return *this;
	}
	
/*
=================================================
	SetCompareOp
=================================================
*/
	SamplerDescriptor::Builder&  SamplerDescriptor::Builder::SetCompareOp (ECompareFunc::type value)
	{
		_state._compareOp = value;
		_changed = true;
		return *this;
	}
		
/*
=================================================
	Finish
=================================================
*/
	SamplerDescriptor const&  SamplerDescriptor::Builder::Finish ()
	{
		if ( _changed )
		{
			_changed = false;

			// TODO: validate
			
			_state._hash = HashOf( BinArrayCRef( &_state, &_state._hash ) );	// TODO
		}
		return _state;
	}


}	// Platforms
}	// Engine
