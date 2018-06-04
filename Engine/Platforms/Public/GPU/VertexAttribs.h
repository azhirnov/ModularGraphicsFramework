// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/VertexDescr.h"

namespace Engine
{
namespace Platforms
{
	
	//
	// Vertex Attribs
	//

	class VertexAttribs : public CompileTime::CopyQualifiers< FixedSizeHashMap<StaticString<32>, int, 1> >
	{
	// types
	public:
		using Self		= VertexAttribs;
		using Name_t	= StaticString<32>;

		enum AttribIndex : uint {
			AttribIndex_Unknown	= ~0u,
		};


	private:
		//
		// Attrib
		//
		struct Attrib final : public CompileTime::CopyQualifiers< Name_t >
		{
		// variables
			Name_t					buffer;		// buffer name to set buffer binding index to attribute
			AttribIndex				index;
			EVertexAttribute::type	type;		// float|int <1,2,3,4,...> available

		// methods
			Attrib (GX_DEFCTOR);
			explicit Attrib (EVertexAttribute::type type, AttribIndex index, StringCRef buffer = StringCRef());

			bool operator == (const Attrib &right) const;
			bool operator >  (const Attrib &right) const;
		};

		using Attribs	= FixedSizeHashMap< Name_t, Attrib, GlobalConst::GAPI_MaxAttribs >;
		using PairPtr	= Ptr< const Pair<const Name_t, Attrib> >;
		using PairRef	= Pair<const Name_t, Attrib> const&;


	// variables
	private:
		Attribs		_attribs;


	// methods
	public:
		VertexAttribs (GX_DEFCTOR)
		{}
		
		Self & Add (StringCRef name, EVertexAttribute::type type, uint index, StringCRef buffer = StringCRef());

		Self & Add (StringCRef name, EVertexAttribute::type type, StringCRef buffer = StringCRef());
		
		template <typename ValueType>
		Self & Add (StringCRef name, StringCRef buffer = StringCRef());
		
		template <typename ValueType>
		Self & Add (StringCRef name, bool norm, StringCRef buffer = StringCRef());

		ND_ PairPtr	Get (StringCRef name) const;

		ND_ bool	IsExist (StringCRef name) const		{ return _attribs.IsExist( name ); }

			void	Clear ()							{ _attribs.Clear(); }

		ND_ bool	Empty () const						{ return _attribs.Empty(); }

		ND_ usize	Count () const						{ return _attribs.Count(); }

		ND_ PairRef	operator [] (usize index) const		{ return _attribs[index]; }
	};
	

	
/*
=================================================
	constructor
=================================================
*/
	inline VertexAttribs::Attrib::Attrib (UninitializedType) : 
		index( AttribIndex_Unknown ), type( EVertexAttribute::Unknown )
	{}

	inline VertexAttribs::Attrib::Attrib (EVertexAttribute::type type, AttribIndex index, StringCRef buffer) :
		buffer( buffer ), index( index ), type( type )
	{}
	
/*
=================================================
	operator ==
=================================================
*/
	inline bool VertexAttribs::Attrib::operator == (const Attrib &right) const
	{
		return	type	== right.type	and
				index	== right.index	and
				buffer	== right.buffer;
	}
	
/*
=================================================
	operator >
=================================================
*/
	inline bool VertexAttribs::Attrib::operator >  (const Attrib &right) const
	{
		return	type	!= right.type	?	type	> right.type	:
				index	!= right.index	?	index	> right.index	:
											buffer	> right.buffer;
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	Add
=================================================
*/
	inline VertexAttribs &  VertexAttribs::Add (StringCRef name, EVertexAttribute::type type, uint index, StringCRef buffer)
	{
		ASSERT( not _attribs.IsExist( name ) );
		_attribs.Add( name, Attrib{ type, AttribIndex(index), buffer } );
		return *this;
	}

	inline VertexAttribs &  VertexAttribs::Add (StringCRef name, EVertexAttribute::type type, StringCRef buffer)
	{
		return Add( name, type, (uint)_attribs.Count(), buffer );
	}
		
/*
=================================================
	Add
=================================================
*/
	template <typename ValueType>
	inline VertexAttribs &  VertexAttribs::Add (StringCRef name, StringCRef buffer)
	{
		return Add( name, VertexDescr< ValueType >::attrib, buffer );
	}
		
	template <typename ValueType>
	inline VertexAttribs &  VertexAttribs::Add (StringCRef name, bool norm, StringCRef buffer)
	{
		return Add( name, EVertexAttribute::SetNormalized( VertexDescr< ValueType >::attrib, norm ), buffer );
	}
	
/*
=================================================
	Get
=================================================
*/
	inline VertexAttribs::PairPtr  VertexAttribs::Get (StringCRef name) const
	{
		Attribs::const_iterator  iter;
		_attribs.Find( name, OUT iter );
		return iter;
	}
//-----------------------------------------------------------------------------


}	// Platforms
}	// Engine
