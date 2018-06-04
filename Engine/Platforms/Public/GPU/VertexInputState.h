// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/VertexDescr.h"
#include "Engine/Platforms/Public/GPU/VertexAttribs.h"

namespace Engine
{
namespace Platforms
{
	
	//
	// Vertex Input State
	//

	class VertexInputState : public CompileTime::CopyQualifiers< FixedSizeHashMap<StaticString<32>, int, 1> >
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

			ND_ bool operator == (const Attrib &right) const;
			ND_ bool operator >  (const Attrib &right) const;
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

			ND_ bool operator == (const Binding &right) const;
			ND_ bool operator >  (const Binding &right) const;
		};

		
		using Attribs_t		= FixedSizeHashMap< Name_t, Attrib, GlobalConst::GAPI_MaxAttribs >;
		using Bindings_t	= FixedSizeHashMap< Name_t, Binding, GlobalConst::GAPI_MaxAttribs >;
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
		VertexInputState (const Attribs_t &attribs, const Bindings_t &bindings);

		VertexInputState (Self &&) = default;
		VertexInputState (const Self &) = default;

		Self& operator = (Self &&) = default;
		Self& operator = (const Self &) = default;

		template <typename ClassType, typename ValueType>
		Self & Add (StringCRef name, ValueType ClassType:: *vertex, bool norm = false, StringCRef buffer = StringCRef());
		Self & Add (StringCRef name, EVertexAttribute::type type, BytesU offset, StringCRef buffer = StringCRef());

		Self & Bind (StringCRef name, BytesU stride, uint index = BindingIndex_Auto, EVertexInputRate::type rate = EVertexInputRate::Vertex);

		void  Clear ();

			bool Merge (const VertexAttribs &attribs, OUT VertexInputState &result) const;
		ND_ bool Equals (const VertexInputState &other, bool ignoreNames) const;

		ND_ Attribs_t const&	Attribs () const	{ return _attribs; }
		ND_ Bindings_t const&	Bindings () const	{ return _bindings; }

		ND_ bool operator == (const VertexInputState &right) const;
	};
	
		
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

}	// Platforms
}	// Engine
//-----------------------------------------------------------------------------

namespace GX_STL
{
namespace GXTypes
{
	
	template <>
	struct Hash < Engine::Platforms::VertexInputState::Attrib >
	{
		ND_ HashResult operator () (const Engine::Platforms::VertexInputState::Attrib &x) const noexcept
		{
			return HashOf( x.type ) + HashOf( x.index ) + HashOf( x.offset ) + HashOf( x.bindingIndex );
		}
	};
	
	template <>
	struct Hash < Engine::Platforms::VertexInputState::Binding >
	{
		ND_ HashResult operator () (const Engine::Platforms::VertexInputState::Binding &x) const noexcept
		{
			return HashOf( x.index ) + HashOf( x.stride ) + HashOf( x.rate );
		}
	};
	
	template <>
	struct Hash < Engine::Platforms::VertexInputState >
	{
		ND_ HashResult operator () (const Engine::Platforms::VertexInputState &x) const noexcept;
	};

}	// GXTypes
}	// GX_STL
