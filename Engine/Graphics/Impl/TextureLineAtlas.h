// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Graphics/Common.h"

namespace Engine
{
namespace Graphics
{

	//
	// Texture Lines Atlas
	//

	class TextureLineAtlas
	{
	// types
	public:

		//
		// Sub Texture Identifer
		//
		struct SubTextureId
		{
			friend class TextureLineAtlas;

		// variables
		private:
			uint	_id;
		
		// methods
		private:
			SubTextureId (uint x, uint y) : _id( ((y & 0xFFFF) << 16) | (x & 0xFFFF) )	{}

		public:
			SubTextureId () : _id(UMax) {}

			bool operator >  (const SubTextureId &right) const		{ return _id >  right._id; }
			bool operator >= (const SubTextureId &right) const		{ return _id >= right._id; }
			bool operator <  (const SubTextureId &right) const		{ return _id <  right._id; }
			bool operator == (const SubTextureId &right) const		{ return _id == right._id; }

			uint X () const			{ return _id & 0xFFFF; }
			uint Y () const			{ return _id >> 16; }

			bool IsValid ()	const	{ return _id != -1; }
		};


	private:

		//
		// Texture Line
		//
		struct TexLine
		{
			ushort		height;
			ushort		width;
			ushort		offsetY;	// top coord of this line
			ushort		offsetX;	// left coord of this line
			ushort		localX;		// left coord of last added texture
			///
			TexLine () : height(0), width(0), offsetX(0), offsetY(0), localX(0) {}
		};

		using TexLines_t	= Map< SubTextureId, TexLine >;


	// variables
	private:
		uint2			_texDim;
		TexLines_t		_lines;
		TexLines_t		_freeLines;
		uint			_height;
		uint2			_align;


	// methods
	private:
		bool _GetTexLine (const SubTextureId &id, OUT TexLine *&line);

	public:
		TextureLineAtlas ();
		~TextureLineAtlas ();

		void Setup (uint alignX = 128, uint alignY = 4);

		bool Create (const uint2 &dim);
		void Destroy ();

		bool Empty () const;

		bool AllocLine (OUT SubTextureId &id, const uint2 &dim);
		bool DeallocLine (INOUT SubTextureId &id);
		void GetFreeSpace (OUT uint2 &notAllocated, OUT uint &freeLines) const;

		bool AddTexture (const SubTextureId &id, const uint2 &dim, OUT uint2 &offset);
		bool GetLineFreeSize (const SubTextureId &id, OUT uint2 &dim);

		bool FindLine (OUT SubTextureId &id, uint lineHeight, uint requestFreeWidth = 0) const;

		uint2 const &	Dimension () const	{ return _texDim; }
	};


	
/*
=================================================
	constructor
=================================================
*/
	inline TextureLineAtlas::TextureLineAtlas () : _height(0), _align(128,4)
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	inline TextureLineAtlas::~TextureLineAtlas ()
	{
		Destroy();
	}
	
/*
=================================================
	Setup
=================================================
*/
	inline void TextureLineAtlas::Setup (uint alignX, uint alignY)
	{
		_align	= uint2( alignX, alignY );

		if ( IsNotZero( _texDim ) )
		{
			if ( _texDim.x % alignX != 0 )
			{
				WARNING( "set default width align" );
				_align.x = _texDim.x >> 3;
			}
		}
	}
	
/*
=================================================
	Create
=================================================
*/
	inline bool TextureLineAtlas::Create (const uint2 &dim)
	{
		Destroy();

		_texDim	 = dim;

		Setup( _align.x, _align.y );

		return true;
	}
	
/*
=================================================
	Destroy
=================================================
*/
	inline void TextureLineAtlas::Destroy ()
	{
		_lines.Clear();
		_freeLines.Clear();
		_height  = 0;
		_align	 = uint2(128, 4);
		_texDim  = uint2();
	}
	
/*
=================================================
	Empty
=================================================
*/
	inline bool TextureLineAtlas::Empty () const
	{
		return _lines.Empty();
	}

/*
=================================================
	AllocLine
=================================================
*/
	inline bool TextureLineAtlas::AllocLine (OUT SubTextureId &id, const uint2 &dim)
	{
		const uint	aligned_height		= Align( dim.y, _align.y );
		const uint	aligned_width		= Align( dim.x, _align.x );
		const uint	free_height			= _texDim.y - _height;
		const bool	use_any_free_line	= aligned_height > free_height;

		TexLine	line;

		CHECK_ERR( aligned_width  <= _texDim.x );


		// find in free lines
		bool	used_free_line	= false;
		usize	near_fl_index	= -1;
		float	near_fl_diff	= 0.0f;

		FOR( i, _freeLines )
		{
			TexLine &	tl = _freeLines[i].second;

			if ( tl.height >= aligned_height and
				 tl.height <= aligned_height + _align.y and
				 tl.width  >= aligned_width )
			{
				line			= tl;
				used_free_line	= true;
				_freeLines.EraseByIndex( i );
				break;
			}

			if ( use_any_free_line )
			{
				if ( tl.height >= aligned_height and
					 tl.width  >= _align.x )
				{
					vec2	match;
					
					match.y = (float) aligned_height / tl.height;
					match.x = tl.width > aligned_width ?
								(float) aligned_width / tl.width :
								(float) tl.width / aligned_width;

					float	diff = (1.0f - match).Sum();

					if ( diff > near_fl_diff )
					{
						near_fl_diff  = diff;
						near_fl_index = i;
					}
				}
			}
		}


		// create new line
		if ( not used_free_line )
		{
			if ( aligned_height > free_height )
				return false;

			line.height		= aligned_height;
			line.width		= _texDim.x;
			line.localX		= 0;
			line.offsetX	= 0;
			line.offsetY	= _height;

			_height += aligned_height;
		}


		// add free line
		if ( line.width > aligned_width and
			 line.width < aligned_width + _align.x )
		{
			TexLine	free_line;

			free_line.height	= line.height;
			free_line.localX	= 0;
			free_line.offsetX	= line.offsetX + aligned_width;
			free_line.offsetY	= line.offsetY;
			free_line.width		= line.width - aligned_width;
			line.width			= aligned_width;

			_freeLines.Add( SubTextureId( free_line.offsetX, free_line.offsetY ), free_line );
		}


		id = SubTextureId( line.offsetX, line.offsetY );

		_lines.Add( id, line );
		return true;
	}
	
/*
=================================================
	DeallocLine
=================================================
*/
	inline bool TextureLineAtlas::DeallocLine (INOUT SubTextureId &id)
	{
		CHECK_ERR( id.IsValid() );

		usize	index = -1;
		
		CHECK_ERR( _lines.FindIndex( id, index ) );

		TexLine	line = _lines[ index ].second;

		_lines.EraseByIndex( index );

		// if line in bottom then change _height
		if ( line.height + line.offsetY == _height )
		{
			FOR_rev( i, _lines )
			{
				uint y = _lines[i].first.Y();

				if ( y >= line.offsetY )
				{
					_height -= line.height;
					break;
				}
			}
		}

		id = SubTextureId();
		return true;
	}
	
/*
=================================================
	AddTexture
----
	'offset' - left top coord of sub texture (in pixels).
	returns true if 'offset' calculated successfully.
=================================================
*/
	inline bool TextureLineAtlas::AddTexture (const SubTextureId &id, const uint2 &dim, OUT uint2 &offset)
	{
		CHECK_ERR( id.IsValid() );

		TexLine *	line = null;
		
		CHECK_ERR( _GetTexLine( id, line ) );
		CHECK_ERR( dim.y <= line->height );

		if ( (int) dim.x > line->width - line->localX )
			return false;
		
		offset = uint2( line->localX, line->offsetY );

		line->localX += dim.x;
		return true;
	}
	
/*
=================================================
	GetLineFreeSize
=================================================
*/
	inline bool TextureLineAtlas::GetLineFreeSize (const SubTextureId &id, OUT uint2 &dim)
	{
		CHECK_ERR( id.IsValid() );

		TexLine *	line = null;

		CHECK_ERR( _GetTexLine( id, line ) );

		dim = uint2( line->width, line->height );
		return true;
	}
	
/*
=================================================
	_GetTexLine
=================================================
*/
	inline bool TextureLineAtlas::_GetTexLine (const SubTextureId &id, TexLine *&line)
	{
		CHECK_ERR( id.IsValid() );

		TexLines_t::iterator	res;

		if ( _lines.Find( id, res ) )
		{
			line = &res->second;
			return true;
		}
		return false;
	}
	
/*
=================================================
	GetFreeSpace
=================================================
*/
	inline void TextureLineAtlas::GetFreeSpace (OUT uint2 &notAllocated, OUT uint &freeLines) const
	{
		freeLines	 = _freeLines.Count();
		notAllocated = uint2( _texDim.x, _texDim.y - _height );
	}
	
/*
=================================================
	FindLine
=================================================
*/
	inline bool TextureLineAtlas::FindLine (OUT SubTextureId &id, uint lineHeight, uint requestFreeWidth) const
	{
		FOR( i, _lines )
		{
			TexLine const &	tl = _lines[i].second;

			if ( tl.height == lineHeight and tl.width - tl.localX >= (int)requestFreeWidth )
			{
				id = _lines[i].first;
				return true;
			}
		}
		id = SubTextureId();
		return false;
	}


}	// Graphics
}	// Engine
