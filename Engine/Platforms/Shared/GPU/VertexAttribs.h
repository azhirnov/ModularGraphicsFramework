// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/VertexDescr.h"

namespace Engine
{
namespace Platforms
{
	
	//
	// Vertex Attribs
	//

	class VertexAttribs final : public CompileTime::PODStruct		// TODO: copy from StaticArray
	{
	// types
	public:
		enum AttribIndex : uint {};
		enum BindingIndex : uint {};

		using Self	= VertexAttribs;
		

	private:
		//
		// Attrib
		//
		struct Attrib : public CompileTime::PODStruct
		{
		// variables
			EVertexAttribute::type	type;
			AttribIndex				index;
			BytesU					offset;
			BindingIndex			bindingIndex;
			bool					enabled;

		// methods
			Attrib (GX_DEFCTOR) :
				type( EVertexAttribute::Unknown ),
				index( AttribIndex(-1) ),
				offset( 0 ),
				bindingIndex( BindingIndex(0) ),
				enabled( false )
			{}

			Attrib (AttribIndex index, EVertexAttribute::type type, BytesU offset, BindingIndex bindingIndex) :
				type( type ),
				index( index ),
				offset( offset ),
				bindingIndex( bindingIndex ),
				enabled( true )
			{}

			bool IsEnabled () const
			{
				return index != AttribIndex(-1) and enabled;
			}

			/*EVertexAttribute::type ToVSInputType () const
			{
				return EVertexAttribute::ValueType::ToVSInputType( type );
			}*/

			bool operator == (const Attrib &right) const
			{
				return	IsEnabled()		== right.IsEnabled() and (
						IsEnabled()
						? (	type		== right.type		and
							index		== right.index		and
							offset		== right.offset		and
							bindingIndex== right.bindingIndex )
						: true );
			}

			bool operator != (const Attrib &right) const
			{
				return not (*this == right);
			}
		};


		//
		// Buffer Binding
		//
		struct Binding : public CompileTime::PODStruct
		{
		// variables
			BindingIndex			index;
			BytesU					stride;
			EVertexInputRate::type	rate;
			bool					enabled;

		// methods
			Binding (GX_DEFCTOR) :
				index( BindingIndex(-1) ), stride( 0 ), rate( EVertexInputRate::Unknown ), enabled( false )
			{}

			Binding (BindingIndex index, BytesU stride, EVertexInputRate::type rate) :
				index(index), stride(stride), rate(rate), enabled(true)
			{}

			bool IsEnabled () const
			{
				return index != BindingIndex(-1) and enabled;
			}

			bool operator == (const Binding &right) const
			{
				return	IsEnabled()		== right.IsEnabled() and (
						IsEnabled()
						? (	index		== right.index		and
							stride		== right.stride		and
							rate		== right.rate )
						:	true );
			}

			bool operator != (const Binding &right) const
			{
				return not (*this == right);
			}
		};


		using Attribs			= StaticArray< Attrib, GlobalConst::Graphics_MaxAttribs >;
		using BufferBindings	= StaticArray< Binding, GlobalConst::Graphics_MaxAttribs >;
		using PairRef			= Pair< Attrib const&, Binding const& >;


	// variables
	private:
		Attribs				_attribs;
		BufferBindings		_bindings;


	// methods
	public:
		VertexAttribs (GX_DEFCTOR)
		{}
		

		Self & Add (AttribIndex index, EVertexAttribute::type type, BytesU offset, BindingIndex bindingIndex = BindingIndex(0))
		{
			// attrib allready defined
			ASSERT( _attribs[index].index == AttribIndex(-1) );
			ASSERT( bindingIndex < _bindings.Count() );

			_attribs[index] = Attrib( index, type, offset, bindingIndex );
			return *this;
		}
		

		template <typename ClassType, typename ValueType>
		Self & Add (AttribIndex index, ValueType ClassType:: *vertex, BindingIndex bindingIndex = BindingIndex(0))
		{
			// attrib allready defined
			ASSERT( _attribs[index].index == AttribIndex(-1) );
			ASSERT( bindingIndex < _bindings.Count() );

			_attribs[index] = Attrib( index,
									  VertexDescr< ValueType >::attrib,
									  OffsetOf( vertex ),
									  bindingIndex );
			return *this;
		}
		

		template <typename ClassType, typename ValueType>
		Self & Add (AttribIndex index, ValueType ClassType:: *vertex, bool norm, BindingIndex bindingIndex = BindingIndex(0))
		{
			// attrib allready defined
			ASSERT( _attribs[index].index == AttribIndex(-1) );
			ASSERT( bindingIndex < _bindings.Count() );

			_attribs[index] = Attrib( index,
									  EVertexAttribute::SetNormalized( VertexDescr< ValueType >::attrib, norm ),
									  OffsetOf( vertex ),
									  bindingIndex );
			return *this;
		}


		Self & Bind (BindingIndex bindingIndex, BytesU stride, EVertexInputRate::type rate = EVertexInputRate::Vertex)
		{
			ASSERT( _bindings[bindingIndex].index == BindingIndex(-1) );

			_bindings[bindingIndex] = Binding( bindingIndex, stride, rate );
			return *this;
		}


		void Clear ()
		{
			_attribs.Clear();
			_bindings.Clear();
		}


		usize Count () const
		{
			return _attribs.Count();
		}


		PairRef  operator [] (usize i) const
		{
			return PairRef( _attribs[i], _bindings[i] );
		}


		bool operator == (const VertexAttribs &right) const
		{
			FOR( i, _attribs )
			{
				if ( _attribs[i]  != right._attribs[i]  or
					 _bindings[i] != right._bindings[i] )
				{
					return false;
				}
			}
			return true;
		}
	};


}	// Platforms
}	// Engine
