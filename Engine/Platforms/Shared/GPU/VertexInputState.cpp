// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/VertexInputState.h"

namespace Engine
{
namespace Platforms
{

/*
=================================================
	constructor
=================================================
*/
	VertexInputState::Attrib::Attrib (UninitializedType) :
		type( EVertexAttribute::Unknown ),
		index( AttribIndex(~0u) ),
		bindingIndex( BindingIndex_Default )
	{}

	VertexInputState::Attrib::Attrib (AttribIndex index, EVertexAttribute::type type, BytesU offset, BindingIndex bindingIndex) :
		type( type ),
		index( index ),
		offset( offset ),
		bindingIndex( bindingIndex )
	{}

/*
=================================================
	ToDstType
=================================================
*/
	inline EVertexAttribute::type  VertexInputState::Attrib::ToDstType () const
	{
		return EVertexAttribute::ToDstType( type );
	}

/*
=================================================
	operator ==
=================================================
*/
	bool VertexInputState::Attrib::operator == (const Attrib &right) const
	{
		return	type		== right.type		and
				index		== right.index		and
				offset		== right.offset		and
				bindingIndex== right.bindingIndex;
	}
	
/*
=================================================
	operator >
=================================================
*/
	bool VertexInputState::Attrib::operator >  (const Attrib &right) const
	{
		return	type	!= right.type	?	type			> right.type	:
				index	!= right.index	?	index			> right.index	:
				offset	!= right.offset	?	offset			> right.offset	:
											bindingIndex	> right.bindingIndex;
	}
	
/*
=================================================
	operator <
=================================================
*/
	bool VertexInputState::Attrib::operator <  (const Attrib &right) const
	{
		return	type	!= right.type	?	type			< right.type	:
				index	!= right.index	?	index			< right.index	:
				offset	!= right.offset	?	offset			< right.offset	:
											bindingIndex	< right.bindingIndex;
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	VertexInputState::Binding::Binding (UninitializedType) :
		index( BindingIndex(~0u) ), rate( EVertexInputRate::Unknown )
	{}

	VertexInputState::Binding::Binding (BindingIndex index, BytesU stride, EVertexInputRate::type rate) :
		index(index), stride(stride), rate(rate)
	{}
	
/*
=================================================
	operator ==
=================================================
*/
	bool VertexInputState::Binding::operator == (const Binding &right) const
	{
		return	index	== right.index	and
				stride	== right.stride	and
				rate	== right.rate;
	}
		
/*
=================================================
	operator >
=================================================
*/	
	bool VertexInputState::Binding::operator >  (const Binding &right) const
	{
		return	index	!= right.index	?	index	> right.index	:
				stride	!= right.stride	?	stride	> right.stride	:
											rate	> right.rate;
	}
		
/*
=================================================
	operator <
=================================================
*/	
	bool VertexInputState::Binding::operator <  (const Binding &right) const
	{
		return	index	!= right.index	?	index	< right.index	:
				stride	!= right.stride	?	stride	< right.stride	:
											rate	< right.rate;
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	VertexInputState::VertexInputState (const Attribs_t &attribs, const Bindings_t &bindings) :
		_attribs( attribs ), _bindings( bindings )
	{}

/*
=================================================
	Add
=================================================
*/
	VertexInputState &  VertexInputState::Add (StringCRef name, EVertexAttribute::type type, BytesU offset, StringCRef buffer)
	{
		ASSERT( not _attribs.IsExist( name ) );

		BindingIndex	binding	= BindingIndex_Default;

		if ( not buffer.Empty() )
		{
			Bindings_t::iterator	iter;
			CHECK_ERR( _bindings.Find( buffer, OUT iter ), *this );
			binding = iter->second.index;
		}

		_attribs.Add( name, Attrib{ AttribIndex(_attribs.Count()), type, offset, binding } );
		return *this;
	}
	
/*
=================================================
	Add
=================================================
*/
	VertexInputState &  VertexInputState::Bind (StringCRef name, BytesU stride, uint index, EVertexInputRate::type rate)
	{
		ASSERT( not _bindings.IsExist( name ) );

		if ( index == BindingIndex_Auto )
			index = (uint) _bindings.Count();

		_bindings.Add( name, Binding( BindingIndex(index), stride, rate ) );
		return *this;
	}
	
/*
=================================================
	Merge
=================================================
*/
	bool VertexInputState::Merge (const VertexAttribs &attribs, OUT VertexInputState &result) const
	{
		result.Clear();
		result._bindings = this->_bindings;

		FOR( i, attribs )
		{
			const auto&					src = attribs[i];
			Attribs_t::const_iterator	iter;

			if ( _attribs.Find( src.first, OUT iter ) )
			{
				CHECK_ERR( src.second.type == iter->second.ToDstType() );

				// TODO: check binding index
				result._attribs.Add( src.first, Attrib( AttribIndex(src.second.index), iter->second.type, iter->second.offset, iter->second.bindingIndex ) );
			}
		}
		return true;
	}
	
/*
=================================================
	Equals
=================================================
*/
	bool VertexInputState::Equals (const VertexInputState &other, bool ignoreNames) const
	{
		if ( _attribs.Count()  != other._attribs.Count() or
			 _bindings.Count() != other._bindings.Count() )
		{
			return false;
		}

		FOR( i, _attribs )
		{
			const auto&		left	= _attribs[i];
			const auto&		right	= other._attribs[i];

			if ( left.second.bindingIndex	!= right.second.bindingIndex	or
				 left.second.index			!= right.second.index			or
				 left.second.offset			!= right.second.offset			or
				 left.second.type			!= right.second.type			or
				 (not ignoreNames and left.first != right.first) )
			{
				return false;
			}
		}

		FOR( i, _bindings )
		{
			const auto&		left	= _bindings[i];
			const auto&		right	= other._bindings[i];

			if ( left.second.index	!= right.second.index	or
				 left.second.rate	!= right.second.rate	or
				 left.second.stride	!= right.second.stride	or
				 (not ignoreNames and left.first != right.first) )
			{
				return false;
			}
		}

		return true;
	}

/*
=================================================
	Clear
=================================================
*/
	void VertexInputState::Clear ()
	{
		_attribs.Clear();
		_bindings.Clear();
	}
	
/*
=================================================
	operator ==
=================================================
*/
	bool VertexInputState::operator == (const VertexInputState &right) const
	{
		return	_attribs	== right._attribs	and
				_bindings	== right._bindings;
	}
//-----------------------------------------------------------------------------

}	// Platforms
}	// Engine


namespace GX_STL
{
namespace GXTypes
{


	HashResult  Hash < Engine::Platforms::VertexInputState >::operator () (const Key_t &x) const noexcept
	{
		Result_t	res;

		res += HashOf( x._attribs.Count() );
		res += HashOf( x._bindings.Count() );

		FOR( i, x._attribs ) {
			res += HashOf( x._attribs[i].first );
			res += HashOf( x._attribs[i].second );
		}
		FOR( i, x._bindings ) {
			res += HashOf( x._bindings[i].first );
			res += HashOf( x._bindings[i].second );
		}
		return res;
	}

}	// GXTypes
}	// GX_STL
