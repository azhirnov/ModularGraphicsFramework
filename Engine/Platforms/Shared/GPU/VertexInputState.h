// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/VertexDescr.h"
#include "Engine/Platforms/Shared/GPU/VertexAttribs.h"

namespace Engine
{
namespace Platforms
{
	
	//
	// Vertex Input State
	//

	class VertexInputState final : public CompileTime::CopyQualifiers< FixedSizeHashMap<StaticString<32>, int, 1> >
	{
	// types
	public:
		//using AttribIndex	= VertexAttribs::AttribIndex;
		using Self			= VertexInputState;
		using Name_t		= VertexAttribs::Name_t;
		
		enum AttribIndex : uint {
			AttribIndex_Unknown	= ~0u,
		};
		enum BindingIndex : uint {
			BindingIndex_Default = 0,
			BindingIndex_Auto	 = ~0u,
		};


		//
		// Attrib
		//
		struct Attrib final : public CompileTime::PODStruct
		{
		// variables
			EVertexAttribute::type	type;	// src type, if src type is normalized short3, then dst type is float3.
			AttribIndex				index;
			BytesU					offset;
			BindingIndex			bindingIndex;

		// methods
			Attrib (GX_DEFCTOR);
			Attrib (AttribIndex index, EVertexAttribute::type type, BytesU offset, BindingIndex bindingIndex);

			EVertexAttribute::type ToDstType () const;

			bool operator == (const Attrib &right) const;
			bool operator >  (const Attrib &right) const;
			bool operator <  (const Attrib &right) const;
		};


		//
		// Buffer Binding
		//
		struct Binding final : public CompileTime::PODStruct
		{
		// variables
			BindingIndex			index;
			BytesU					stride;
			EVertexInputRate::type	rate;

		// methods
			Binding (GX_DEFCTOR);
			Binding (BindingIndex index, BytesU stride, EVertexInputRate::type rate);

			bool operator == (const Binding &right) const;
			bool operator >  (const Binding &right) const;
			bool operator <  (const Binding &right) const;
		};

		
		using Attribs_t		= FixedSizeHashMap< Name_t, Attrib, GlobalConst::Graphics_MaxAttribs >;
		using Bindings_t	= FixedSizeHashMap< Name_t, Binding, GlobalConst::Graphics_MaxAttribs >;
		using PairRef		= Pair< Attrib const&, Binding const& >;
		
		friend struct GX_STL::GXTypes::Hash < VertexInputState::Attrib >;
		friend struct GX_STL::GXTypes::Hash < VertexInputState::Binding >;
		friend struct GX_STL::GXTypes::Hash < VertexInputState >;


	// variables
	private:
		Attribs_t		_attribs;
		Bindings_t		_bindings;


	// methods
	public:
		VertexInputState (GX_DEFCTOR) {}
		
		template <typename ClassType, typename ValueType>
		Self & Add (StringCRef name, ValueType ClassType:: *vertex, bool norm, StringCRef buffer = StringCRef());
		Self & Add (StringCRef name, EVertexAttribute::type type, BytesU offset, StringCRef buffer = StringCRef());

		Self & Bind (StringCRef name, BytesU stride, uint index = BindingIndex_Auto, EVertexInputRate::type rate = EVertexInputRate::Vertex);

		void  Clear ();

		bool Merge (const VertexAttribs &attribs, OUT VertexInputState &result) const;

		Attribs_t const&	Attribs () const	{ return _attribs; }
		Bindings_t const&	Bindings () const	{ return _bindings; }

		bool operator == (const VertexInputState &right) const;
	};
	

	
/*
=================================================
	constructor
=================================================
*/
	inline VertexInputState::Attrib::Attrib (UninitializedType) :
		type( EVertexAttribute::Unknown ),
		index( AttribIndex(~0u) ),
		bindingIndex( BindingIndex_Default )
	{}

	inline VertexInputState::Attrib::Attrib (AttribIndex index, EVertexAttribute::type type, BytesU offset, BindingIndex bindingIndex) :
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
	inline bool VertexInputState::Attrib::operator == (const Attrib &right) const
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
	inline bool VertexInputState::Attrib::operator >  (const Attrib &right) const
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
	inline bool VertexInputState::Attrib::operator <  (const Attrib &right) const
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
	inline VertexInputState::Binding::Binding (UninitializedType) :
		index( BindingIndex(~0u) ), rate( EVertexInputRate::Unknown )
	{}

	inline VertexInputState::Binding::Binding (BindingIndex index, BytesU stride, EVertexInputRate::type rate) :
		index(index), stride(stride), rate(rate)
	{}
	
/*
=================================================
	operator ==
=================================================
*/
	inline bool VertexInputState::Binding::operator == (const Binding &right) const
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
	inline bool VertexInputState::Binding::operator >  (const Binding &right) const
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
	inline bool VertexInputState::Binding::operator <  (const Binding &right) const
	{
		return	index	!= right.index	?	index	< right.index	:
				stride	!= right.stride	?	stride	< right.stride	:
											rate	< right.rate;
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	Add
=================================================
*/
	inline VertexInputState &  VertexInputState::Add (StringCRef name, EVertexAttribute::type type, BytesU offset, StringCRef buffer)
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
	template <typename ClassType, typename ValueType>
	inline VertexInputState &  VertexInputState::Add (StringCRef name, ValueType ClassType:: *vertex, bool norm, StringCRef buffer)
	{
		return Add( name, EVertexAttribute::SetNormalized(VertexDescr< ValueType >::attrib, norm), OffsetOf( vertex ), buffer );
	}
	
/*
=================================================
	Add
=================================================
*/
	inline VertexInputState &  VertexInputState::Bind (StringCRef name, BytesU stride, uint index, EVertexInputRate::type rate)
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
	inline bool VertexInputState::Merge (const VertexAttribs &attribs, OUT VertexInputState &result) const
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
				result._attribs.Add( src.first, Attrib( AttribIndex(src.second.index), src.second.type, iter->second.offset, iter->second.bindingIndex ) );
			}
		}
		return true;
	}
	
/*
=================================================
	Clear
=================================================
*/
	inline void VertexInputState::Clear ()
	{
		_attribs.Clear();
		_bindings.Clear();
	}
	
/*
=================================================
	operator ==
=================================================
*/
	inline bool VertexInputState::operator == (const VertexInputState &right) const
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
	
	template <>
	struct Hash < Engine::Platforms::VertexInputState::Attrib >
	{
		using key_t		= Engine::Platforms::VertexInputState::Attrib;
		using result_t	= HashResult;

		result_t operator () (const key_t &x) const noexcept
		{
			return HashOf( x.type ) + HashOf( x.index ) + HashOf( x.offset ) + HashOf( x.bindingIndex );
		}
	};
	
	template <>
	struct Hash < Engine::Platforms::VertexInputState::Binding >
	{
		using key_t		= Engine::Platforms::VertexInputState::Binding;
		using result_t	= HashResult;

		result_t operator () (const key_t &x) const noexcept
		{
			return HashOf( x.index ) + HashOf( x.stride ) + HashOf( x.rate );
		}
	};
	
	template <>
	struct Hash < Engine::Platforms::VertexInputState >
	{
		using key_t		= Engine::Platforms::VertexInputState;
		using result_t	= HashResult;

		result_t operator () (const key_t &x) const noexcept
		{
			result_t	res;

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
	};

}	// Platforms
}	// Engine
