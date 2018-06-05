// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/PixelFormatEnums.h"

namespace Engine
{
namespace Platforms
{
	
	//
	// Fragment Output State
	//

	struct FragmentOutputState : public CompileTime::CopyQualifiers< FixedSizeHashSet<StaticString<32>, 1> >
	{
	// types
	public:
		using Self	= FragmentOutputState;

		enum FragOutIndex : uint {
			FragOutIndex_Unknown = ~0u
		};


	private:
		struct FragOutput final : public CompileTime::FastCopyable
		{
		// variables
			FragOutIndex		index;
			EFragOutput::type	type;

		// methods
			FragOutput (GX_DEFCTOR) :
				index( FragOutIndex_Unknown ),	type( EFragOutput::Unknown )
			{}

			FragOutput (FragOutIndex index, EFragOutput::type type) :
				index( index ), type( type )
			{}

			bool operator == (const FragOutput &right) const
			{
				return	(type == right.type) and (index == right.index);
			}

			bool operator >  (const FragOutput &right) const
			{
				return	type != right.type	?	type	> right.type	:
												index	> right.index;
			}
		};

		using Name_t			= StaticString<32>;
		using FragOutputs_t		= FixedSizeHashMap< Name_t, FragOutput, GlobalConst::GAPI_MaxColorBuffers >;


	// variables
	private:
		FragOutputs_t	_outputs;


	// methods
	public:
		FragmentOutputState (GX_DEFCTOR)
		{}

		ND_ Pair<const Name_t, FragOutput> const&  operator [] (usize index) const {
			return _outputs[ index ];
		}

		ND_ Ptr<const FragOutput> operator () (StringCRef name) const
		{
			FragOutputs_t::const_iterator	iter;
			_outputs.Find( name, OUT iter );
			return iter ? &iter->second : null;
		}

		Self& Add (StringCRef name, EFragOutput::type type) {
			return Add( name, type, uint(_outputs.Count()) );
		}

		Self& Add (StringCRef name, EFragOutput::type type, uint index)
		{
			ASSERT( not IsExist( name ) );
			_outputs.Add( name, FragOutput{ FragOutIndex(index), type } );
			return *this;
		}

		ND_ bool IsExist (StringCRef name) const {
			return _outputs.IsExist( name );
		}

		ND_ usize Count () const {
			return _outputs.Count();
		}

		void Clear () {
			_outputs.Clear();
		}
	};


}	// Platforms
}	// Engine
