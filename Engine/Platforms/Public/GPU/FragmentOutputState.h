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
			FragOutput (GX_DEFCTOR);
			FragOutput (FragOutIndex index, EFragOutput::type type);

			bool operator == (const FragOutput &right) const;
			bool operator >  (const FragOutput &right) const;
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

		Pair<const Name_t, FragOutput> const&	operator [] (usize index) const;

		Ptr<const FragOutput>					operator () (StringCRef name) const;

		ND_ bool	IsExist (StringCRef name)	const	{ return _outputs.IsExist( name ); }

		ND_ usize	Count ()					const	{ return _outputs.Count(); }

		Self&  Add (StringCRef name, EFragOutput::type type);
		Self&  Add (StringCRef name, EFragOutput::type type, uint index);

		void Clear ();

		bool Merge (const Self &other);

		DEBUG_ONLY( String ToString () const );
	};

	

/*
=================================================
	constructor
=================================================
*/
	inline FragmentOutputState::FragOutput::FragOutput (UninitializedType) :
		index( FragOutIndex_Unknown ),	type( EFragOutput::Unknown )
	{}

	inline FragmentOutputState::FragOutput::FragOutput (FragOutIndex index, EFragOutput::type type) :
		index( index ), type( type )
	{}
	
/*
=================================================
	FragOutput::operator ==
=================================================
*/
	inline bool FragmentOutputState::FragOutput::operator == (const FragOutput &right) const
	{
		return	(type == right.type) and (index == right.index);
	}

	inline bool FragmentOutputState::FragOutput::operator >  (const FragOutput &right) const
	{
		return	type != right.type	?	type	> right.type	:
										index	> right.index;
	}
//-----------------------------------------------------------------------------
	

	
/*
=================================================
	operator []
=================================================
*/
	ND_ inline Pair<const FragmentOutputState::Name_t, FragmentOutputState::FragOutput> const& 
		FragmentOutputState::operator [] (usize index) const
	{
		return _outputs[ index ];
	}
	
/*
=================================================
	operator ()
=================================================
*/
	ND_ inline Ptr<const FragmentOutputState::FragOutput>
		FragmentOutputState::operator () (StringCRef name) const
	{
		FragOutputs_t::const_iterator	iter;
		_outputs.Find( name, OUT iter );
		return iter ? &iter->second : null;
	}
	
/*
=================================================
	Add
=================================================
*/
	inline FragmentOutputState&  FragmentOutputState::Add (StringCRef name, EFragOutput::type type)
	{
		return Add( name, type, uint(_outputs.Count()) );
	}

	inline FragmentOutputState&  FragmentOutputState::Add (StringCRef name, EFragOutput::type type, uint index)
	{
		ASSERT( not IsExist( name ) );
		_outputs.Add( name, FragOutput{ FragOutIndex(index), type } );
		return *this;
	}
	
/*
=================================================
	Clear
=================================================
*/
	inline void FragmentOutputState::Clear ()
	{
		_outputs.Clear();
	}
	
/*
=================================================
	Merge
=================================================
*/
	inline bool FragmentOutputState::Merge (const Self &other)
	{
		FOR( i, other )
		{
			FragOutputs_t::iterator	iter;

			if ( _outputs.Find( other[i].first, OUT iter ) )
			{
				if ( not (iter->second == other[i].second) )
					return false;
			}
			else
			{
				FOR( j, _outputs )
				{
					if ( _outputs[j].second.index == other[i].second.index )
						return false;
				}

				_outputs.Add( other[i].first, other[i].second );
			}
		}
		return true;
	}
	
/*
=================================================
	ToString
=================================================
*/
	DEBUG_ONLY(
	ND_ inline String  FragmentOutputState::ToString () const
	{
		String	str;

		// TODO
		return str;
	})


}	// Platforms
}	// Engine
