// Copyright © 2014-2015  Zhirnov Andrey. All rights reserved.
/*
#pragma once

#include "../Containers/String.h"

namespace UX_STL
{
namespace UXTypes
{

	//
	// Extended String
	//

	template <	typename T,
				typename S = typename TAutoCopyStrategy<T>::type,
				typename A = TAllocator<T>
			 >
	struct TExtString
	{
	// types
	public:
		enum EChunkType
		{
			UNKNOWN	= 0,
			COLOR,					// #[color:RRGGBBAA]
			SCALE,					// #[scale:1.0]
			SPACING,				// #[spacing:1.0]
			OFFSET_X,				// #[offsetX:0]
			OFFSET_Y,				// #[offsetY:0]
			ALIGN,					// #[align:left|top]	(left, centerX, right), (bottom, centerY, top), center
			// for reader state only
			END		= 0xEE,
			STRING	= 0xFF,
		};

		struct EAlign
		{
			enum type
			{
				LEFT		= 0x01,
				CENTER_X	= 0x02,
				RIGHT		= 0x04,
				TOP			= 0x10,
				CENTER_Y	= 0x20,
				BOTTOM		= 0x40,
				CENTER		= CENTER_X | CENTER_Y,
			};
		};

		struct TChunk
		{
			EChunkType	eType;
			union {
				float	fValue;		// scale, spacing
				int	iValue;		// color
			};
			///
			TChunk (): eType(UNKNOWN), iValue(0) {}
			TChunk (EChunkType t, int i) : eType(t), iValue(i) {}
			TChunk (EChunkType t, float f) : eType(t), fValue(f) {}
		};
		

		struct TReader;

		typedef TExtString< T, S, A >									Self;
		typedef typename NearInt::UnsignedSize< sizeof(T)*2 >::type		chunk_code_t;
		typedef Array<T,S,A>											array_t;
		typedef typename EAlign::type									align_t;
		typedef TStringRef<const T>										cstr_t;


	private:
		struct _ECode {
			enum type
			{
				END			= 0,
				LENGTH		= 0x0001,	// uint + T[]
				EMPTY		= 0x0002,	// uint + 0
				_MAX_LENGTH	= 0x00E0,
				SCALE		= 0x00E1,	// float
				SPACING		= 0x00E2,	// float
				COLOR		= 0x00E3,	// uint == ubyte[4] == color4u
				OFFSET_X	= 0x00E4,	// float
				OFFSET_Y	= 0x00E5,	// float
				ALIGN		= 0x00E6,
			};
		};

	// values
	private:
		array_t		_aString;
		bool		_bEmpty;


	// methods
	private:
		static EChunkType				_s_CodeToChunk (typename _ECode::type code);
		static typename _ECode::type	_s_ChunkToCode (EChunkType chunk);

		static typename EAlign::type	_s_ParseAlign (TStringRef<const T> str);
		static void						_s_AddAlign (int align, TString<T> &str);


	public:
		TExtString () {}

		explicit
		TExtString (const cstr_t &s);

		~TExtString () {}


		Self &	Append (const cstr_t &s);
		Self &	Append (const cstr_t &s0, const cstr_t &s1);
		Self &	Append (const Self &s);
		Self &	Append (const TChunk &c);
		Self &	Append (EChunkType t, int i)				{ return Append( TChunk( t, i ) ); }
		Self &	Append (EChunkType t, float f)				{ return Append( TChunk( t, f ) ); }
		Self &	AppendChunk (TReader &reader);

		// chunks
		Self &	AddColor (uint color)						{ return Append( TChunk( COLOR, (int)color ) ); }
		Self &	AddScale (float scale)						{ return Append( TChunk( SCALE, scale ) ); }
		Self &	AddSpacing (float spacing)					{ return Append( TChunk( SPACING, spacing ) ); }
		Self &	AddOffsetX (float offset)					{ return Append( TChunk( OFFSET_X, offset ) ); }
		Self &	AddOffsetY (float offset)					{ return Append( TChunk( OFFSET_Y, offset ) ); }
		Self &	AddAlign (align_t align)					{ return Append( TChunk( ALIGN, (int)align ) ); }

		void Insert (TReader &pos, const TChunk &c);
		void Insert (TReader &pos, TStringRef<T> s);
		void InsertColor (TReader &pos, uint color)		{ Insert( pos, TChunk( COLOR, (int)color ) ); }
		void InsertScale (TReader &pos, float scale)		{ Insert( pos, TChunk( SCALE, scale ) ); }
		void InsertSpacing (TReader &pos, float spacing)	{ Insert( pos, TChunk( SPACING, spacing ) ); }
		void InsertOffsetX (TReader &pos, float offset)		{ Insert( pos, TChunk( OFFSET_X, offset ) ); }
		void InsertOffsetY (TReader &pos, float offset)		{ Insert( pos, TChunk( OFFSET_Y, offset ) ); }
		void InsertAlign (TReader &pos, align_t align)		{ Insert( pos, TChunk( ALIGN, align ) ); }

		void Erase (TReader &pos);

		void Optimize ();
		bool DivideString (TReader &pos, usize strPos);

		void RemoveChunks (EChunkType type);
		void Reserve (usize uSize)							{ _aString.Reserve( uSize ); }

		usize	Count () const								{ return _aString.Count(); }
		usize	Size ()	 const								{ return _aString.Size(); }
		bool	Empty () const								{ return _bEmpty; }

		void Copy (const cstr_t &s);
		void Copy (const Self &s);

		void Clear ()										{ _aString.Clear();		_bEmpty = true; }

		//void SwapMem (array_t &arr)							{ _aString.SwapMem( arr ); }
		void SetMem (array_t &arr)							{ _aString.SetMem( arr ); }
		void SetMem (TString<T> &str)						{ MemMover::Swap( str, _aString );  str.Clear();  _bEmpty = false; }

		void Parse (const cstr_t &s);
		void UnParse (TString<T> &str);

		void ExtractString (TString<T> &str) const;

		TReader	GetReader () const;
	};



	//
	// Extended String Reader
	//

	template <typename T, typename S, typename A>
	struct TExtString<T,S,A>::TReader
	{
		template <typename T2, typename S2, typename A2>
		friend struct TExtString;

	// types
	private:
		typedef TExtString<T,S,A>				estr_t;
		typedef typename estr_t::chunk_code_t	chunk_code_t;
		typedef typename estr_t::EChunkType		EChunkType;
		typedef TReader							Self;
		typedef TStringRef<const T>				cstr_t;

		union UCharsToCode
		{
			chunk_code_t	uCode;
			T				aChars[2];
			///
			UCharsToCode (chunk_code_t c): uCode(c) {}
			UCharsToCode (T c0, T c1) { aChars[0] = c0; aChars[1] = c1; }
		};

		template <typename B>
		union UCharsToType
		{
			enum {
				CHARS_COUNT	= sizeof(B) / sizeof(T)
			};

			B		tType;
			T		aChars[ CHARS_COUNT ];
			///
			UCharsToType (const T *p) { MemCopy( aChars, p, sizeof(aChars) ); }
		};


	// values
	private:
		const Array<T,S,A> &	_aString;
		usize					_uPos;
		EChunkType				_eState;
		bool					_bChunkReaded;


	// methods
	public:
		TReader (const TExtString<T,S,A> &s);
		TReader (const Self &reader);

		Self &			operator = (const Self &reader);

		EChunkType		NextChunk ();
		EChunkType		GetState ()		const		{ return _eState; }

		bool			Move (uint offset);
		bool			MoveFromStart (uint offset);
		bool			MoveFromEnd (uint offset);
		void			MoveToStart ();
		void			MoveToEnd ();
		usize			NumChunks ()	const;

		bool			Find (EChunkType type);

		float			GetChunkFloat ();
		int			GetChunkInt ();
		const cstr_t	GetString ();
	};

	

	typedef TExtString< char  >		ext_string;
	typedef	TExtString< wchar >		ext_wstring;


	
	template <typename T, typename S, typename A>
	struct TAutoCopyStrategy< TExtString<T,S,A> >
	{
		typedef TCopyStrategy< TExtString<T,S,A> >		type;
	};
	

	
	template <typename T, typename S, typename A>
	inline TExtString<T,S,A>::TExtString (const TStringRef<const T> &s) : _bEmpty(true)
	{
		Parse( s );
	}
		
	
	template <typename T, typename S, typename A>
	inline typename TExtString<T,S,A>::EChunkType  TExtString<T,S,A>::_s_CodeToChunk (typename _ECode::type code)
	{
		switch ( code )
		{
			case _ECode::END		: return END;
			case _ECode::LENGTH		: return STRING;
			case _ECode::SCALE		: return SCALE;
			case _ECode::SPACING	: return SPACING;
			case _ECode::COLOR		: return COLOR;
			case _ECode::OFFSET_X	: return OFFSET_X;
			case _ECode::OFFSET_Y	: return OFFSET_Y;
			case _ECode::ALIGN		: return ALIGN;
		};

		return UNKNOWN;
	}

	
	template <typename T, typename S, typename A>
	inline typename TExtString<T,S,A>::_ECode::type  TExtString<T,S,A>::_s_ChunkToCode (EChunkType chunk)
	{
		switch ( chunk )
		{
			case END		: return _ECode::END;
			case STRING		: return _ECode::LENGTH;
			case SCALE		: return _ECode::SCALE;
			case SPACING	: return _ECode::SPACING;
			case COLOR		: return _ECode::COLOR;
			case OFFSET_X	: return _ECode::OFFSET_X;
			case OFFSET_Y	: return _ECode::OFFSET_Y;
			case ALIGN		: return _ECode::ALIGN;
		};

		return _ECode::EMPTY;
	}
	
	
	template <typename T, typename S, typename A>
	inline typename TExtString<T,S,A>::EAlign::type  TExtString<T,S,A>::_s_ParseAlign (const TStringRef<const T> &str)
	{
		struct Util {
			static bool IsWord (const T &c)
			{
				return  ( c >= T('a') and c <= T('z') ) or
						( c >= T('A') and c <= T('Z') );
			}
		};

		typedef const TStaticString< T, 10 >	st_str_t;
		typedef const TStringRef< const T >		unistr_t;

		static st_str_t		s_sAlignLeft   ( "left"    );
		static st_str_t		s_sAlignRight  ( "right"   );
		static st_str_t		s_sAlignTop    ( "top"     );
		static st_str_t		s_sAlignBottom ( "bottom"  );
		static st_str_t		s_sAlignCenterX( "centerX" );
		static st_str_t		s_sAlignCenterY( "centerY" );
		static st_str_t		s_sAlignCenter ( "center"  );

		unistr_t	s_first,
					s_second;
		usize		u_word_begin = -1;
		int			result = 0;

		for (usize i = 0; i < str.Count(); ++i)
		{
			if ( Util::IsWord( str[i] ) )
			{
				if ( u_word_begin == -1 )
					u_word_begin = i;
			}
			else
			{
				const unistr_t	s_word( &str[u_word_begin], i - u_word_begin );

				if ( not s_word.Empty() )
				{
					if ( s_word == s_sAlignLeft    )	result |= EAlign::LEFT;		else
					if ( s_word == s_sAlignRight   )	result |= EAlign::RIGHT;	else
					if ( s_word == s_sAlignCenterX )	result |= EAlign::CENTER_X;	else
					if ( s_word == s_sAlignTop     )	result |= EAlign::TOP;		else
					if ( s_word == s_sAlignBottom  )	result |= EAlign::BOTTOM;	else
					if ( s_word == s_sAlignCenterY )	result |= EAlign::CENTER_Y;	else
					if ( s_word == s_sAlignCenter  )	result |= EAlign::CENTER;	else
						WARNING( (TString<T>("unknown align parameter \"") << s_word << "\"").cstr() );
				}

				u_word_begin = -1;
			}
		}

		return (typename EAlign::type) result;
	}
	

	template <typename T, typename S, typename A>
	inline void  TExtString<T,S,A>::_s_AddAlign (int align, TString<T> &str)
	{
		if ( align == EAlign::CENTER )
		{
			str << "center";
			return;
		}

		const int	mask_x = (int)EAlign::LEFT | (int)EAlign::CENTER_X | (int)EAlign::RIGHT;
		const int	mask_y = (int)EAlign::TOP  | (int)EAlign::CENTER_Y | (int)EAlign::BOTTOM;

		usize	len = str.Length();

		switch ( align & mask_x )
		{
			case EAlign::LEFT :		str << "left";		break;
			case EAlign::CENTER_X :	str << "centerX";	break;
			case EAlign::RIGHT :	str << "right";		break;
		}

		if ( len != str.Length() )
			str << ',';

		len = str.Length();

		switch ( align & mask_y )
		{
			case EAlign::TOP :		str << "top";		break;
			case EAlign::CENTER_Y :	str << "centerY";	break;
			case EAlign::BOTTOM :	str << "bottom";	break;
		}

		if ( len == str.Length() )
			str.PopBack();
	}

	
	template <typename T, typename S, typename A>
	inline TExtString<T,S,A> &  TExtString<T,S,A>::Append (const TStringRef<const T> &s)
	{
		const chunk_code_t	u_chunk		= _ECode::LENGTH;
		const usize			u_length	= s.Length()+1;

		_aString.Append( Buffer<const T>( (const T *)&u_chunk,  sizeof(u_chunk)/sizeof(T) ) );
		_aString.Append( Buffer<const T>( (const T *)&u_length, sizeof(u_length)/sizeof(T) ) );
		_aString.Append( Buffer<const T>(  s.cstr(), s.Length() ) );
		_aString.PushBack( T(0) );

		_bEmpty = false;
		return *this;
	}
	

	template <typename T, typename S, typename A>
	inline TExtString<T,S,A> &  TExtString<T,S,A>::Append (const TStringRef<const T> &s0, const TStringRef<const T> &s1)
	{
		const chunk_code_t	u_chunk		= _ECode::LENGTH;
		const usize			u_length	= s0.Length() + s1.Length() + 1;

		_aString.Append( Buffer<const T>( (const T *)&u_chunk,  sizeof(u_chunk)/sizeof(T) ) );
		_aString.Append( Buffer<const T>( (const T *)&u_length, sizeof(u_length)/sizeof(T) ) );
		_aString.Append( Buffer<const T>( s0.cstr(), s0.Length() ) );
		_aString.Append( Buffer<const T>( s1.cstr(), s1.Length() ) );
		_aString.PushBack( T(0) );
		
		_bEmpty = false;
		return *this;
	}

	
	template <typename T, typename S, typename A>
	inline TExtString<T,S,A> &  TExtString<T,S,A>::Append (const Self &s)
	{
		_aString.Append( s._aString );
		_bEmpty = false;
		return *this;
	}

	
	template <typename T, typename S, typename A>
	inline TExtString<T,S,A> &  TExtString<T,S,A>::Append (const TChunk &c)
	{
		const chunk_code_t	u_code = _s_ChunkToCode( c.eType );

		_aString.Append( Buffer<const T>( (const T *) &u_code,	sizeof(u_code)   / sizeof(T) ) );
		_aString.Append( Buffer<const T>( (const T *) &c.iValue, sizeof(c.iValue) / sizeof(T) ) );
		
		_bEmpty = false;
		return *this;
	}
	
	
	template <typename T, typename S, typename A>
	inline TExtString<T,S,A> &  TExtString<T,S,A>::AppendChunk (TReader &reader)
	{
		if ( reader._bChunkReaded )
		{
			WARNING( "reader must be in start of chunk!" );
			return *this;
		}

		EChunkType	type = reader.GetState();

		switch ( type )
		{
			case COLOR :
			case ALIGN :
				Append( TChunk( type, reader.GetChunkInt() ) );
				break;
			
			case SCALE :
			case SPACING :
			case OFFSET_X :
			case OFFSET_Y :
				Append( TChunk( type, reader.GetChunkFloat() ) );
				break;

			case STRING	:
				Append( reader.GetString() );
				break;

			default :
				WARNING( "unknown chunk!" );
		}

		_bEmpty = false;
		return *this;
	}

	
	template <typename T, typename S, typename A>
	inline void TExtString<T,S,A>::Copy (const TStringRef<const T> &s)
	{
		Clear();
		Append( s );
	}

	
	template <typename T, typename S, typename A>
	inline void TExtString<T,S,A>::Copy (const Self &s)
	{
		_aString.Copy( s._aString );
		_bEmpty = s.Empty();
	}
	

	template <typename T, typename S, typename A>
	inline void TExtString<T,S,A>::Insert (TReader &reader, const TChunk &c)
	{
		typedef typename TReader::UCharsToCode	UCharsToCode;

		usize	pos = reader._uPos;
		
		if ( reader._uPos >= sizeof(UCharsToCode) and not reader._bChunkReaded )
		{
			pos -= sizeof(UCharsToCode);
		}

		const chunk_code_t	u_code = _s_ChunkToCode( c.eType );

		_aString.Insert( Buffer<const T>( (const T *) &u_code,	sizeof(u_code)   / sizeof(T) ), pos );	pos += sizeof(u_code);
		_aString.Insert( Buffer<const T>( (const T *) &c.iValue, sizeof(c.iValue) / sizeof(T) ), pos );	pos += sizeof(c.iValue);

		reader._uPos = pos;
	}
	

	template <typename T, typename S, typename A>
	inline void TExtString<T,S,A>::Insert (TReader &reader, const TStringRef<T> &s)
	{
		typedef typename TReader::UCharsToCode	UCharsToCode;
	
		usize	pos = reader._uPos;
		
		if ( reader._uPos >= sizeof(UCharsToCode) and not reader._bChunkReaded )
		{
			pos -= sizeof(UCharsToCode);
		}

		const chunk_code_t	u_chunk		= _ECode::LENGTH;
		const usize			u_length	= s.Length()+1;

		ASSERT( u_length < _ECode::_MAX_LENGTH );

		_aString.Insert( Buffer<const T>( (const T *)&u_chunk,  sizeof(u_chunk)  / sizeof(T) ), pos );	pos += sizeof(u_chunk);
		_aString.Insert( Buffer<const T>( (const T *)&u_length, sizeof(u_length) / sizeof(T) ), pos );	pos += sizeof(u_length);
		_aString.Insert( Buffer<const T>( s.cstr(), s.Length() ), pos );									pos += s.Length();
		_aString.Insert( T(0), pos );																		pos += 1;

		reader._uPos = pos;
	}

	
	template <typename T, typename S, typename A>
	inline void TExtString<T,S,A>::Erase (TReader &reader)
	{
		typedef typename TReader::UCharsToCode					UCharsToCode;
		typedef typename TReader::template UCharsToType<uint>	UCharsToUint;
	
		if ( reader._bChunkReaded )
		{
			WARNING( "can't erase readed chunk!" );
			return;
		}

		usize	len = 0;

		if ( reader._uPos >= sizeof(UCharsToCode) )
		{
			len			 += sizeof(UCharsToCode);
			reader._uPos -= sizeof(UCharsToCode);
		}

		switch ( reader._eState )
		{
			case COLOR :
			case ALIGN :
				len += sizeof(int);
				break;
			
			case SCALE :
			case SPACING :
			case OFFSET_X :
			case OFFSET_Y :
				len += sizeof(float);
				break;

			case STRING	: {
				const UCharsToUint	s_length( _aString.ptr() + reader._uPos + len );
				len += s_length.CHARS_COUNT;
				len += s_length.tType;
				break;
			}

			default :
				WARNING( "unknown chunk!" );
		}

		_aString.Erase( reader._uPos, len );

		reader._bChunkReaded = true;
		reader._eState = UNKNOWN;
	}
	

	template <typename T, typename S, typename A>
	inline bool  TExtString<T,S,A>::DivideString (TReader &reader, usize strPos)
	{
		if ( reader.GetState() != STRING )
		{
			WARNING( "can't divide non-string chunk!" );
			return false;
		}

		if ( reader._bChunkReaded )
		{
			WARNING( "current position in end of chunk, move to start of chunk!" );
			return false;
		}

		typedef typename TReader::UCharsToCode					UCharsToCode;
		typedef typename TReader::template UCharsToType<uint>	UCharsToUint;
		
		const UCharsToUint	s_length( _aString.ptr() + reader._uPos );

		const usize	len = s_length.tType;

		if ( strPos >= len )
		{
			WARNING( "divide position is too big!" );
			return false;
		}

		usize	new_len = strPos + 1;
		TODO( "fix it!!!" );
		//_aString.Replace( (const T *) &new_len, sizeof(new_len) / sizeof(T), reader._uPos );
		
		reader._uPos += s_length.CHARS_COUNT;
		reader._uPos += strPos;


		usize					i			= 0;
		const chunk_code_t		u_chunk		= _ECode::LENGTH;

		typedef TStaticString< T, (sizeof(u_chunk) + sizeof(new_len)) / sizeof(T) + 2 >		st_str_t;

		st_str_t				tmp_str;

		tmp_str.SetLength( tmp_str.Capacity()-1 );

		new_len = len - strPos;

		tmp_str[i++] = 0;
		MemCopy( &tmp_str[i], &u_chunk, sizeof(u_chunk) );		i += sizeof(u_chunk) / sizeof(T);
		MemCopy( &tmp_str[i], &new_len, sizeof(new_len) );		i += sizeof(new_len) / sizeof(T);
		ASSERT( i == tmp_str.Length() );

		_aString.Insert( (cstr_t) tmp_str, reader._uPos );

		reader._uPos += sizeof(u_chunk) / sizeof(T) + 1;
		return true;
	}


	template <typename T, typename S, typename A>
	inline void TExtString<T,S,A>::Optimize ()
	{
		TReader				reader		 = GetReader();
		int					prev_color	 = 0;
		int					prev_align	 = 0;
		float				prev_scale	 = 0.0f;
		float				prev_spacing = 0.0f;
		float				prev_offsetx = 0.0f;
		float				prev_offsety = 0.0f;


		while ( reader.NextChunk() != END )
		{
			TReader		tmp( reader );
			EChunkType	type = tmp.GetState();

			switch ( type )
			{
				case COLOR :
				{
					int	value = tmp.GetChunkInt();

					if ( prev_color == value )
						Erase( reader );
					
					prev_color = value;
					break;
				}

				case ALIGN :
				{
					int	value = tmp.GetChunkInt();

					if (prev_align == value )
						Erase( reader );

					prev_align = value;
					break;
				}
			
				case SCALE :
				{
					float	value = tmp.GetChunkFloat();

					if ( UXMath::Equals( prev_scale, value ) )
						Erase( reader );

					prev_scale = value;
					break;
				}

				case SPACING :
				{
					float	value = tmp.GetChunkFloat();

					if ( UXMath::Equals( prev_spacing, value ) )
						Erase( reader );

					prev_spacing = value;
					break;
				}

				case OFFSET_X :
				{
					float	value = tmp.GetChunkFloat();

					if ( UXMath::Equals( prev_offsetx, value ) )
						Erase( reader );

					prev_offsetx = value;
					break;
				}

				case OFFSET_Y :
				{
					float	value = tmp.GetChunkFloat();

					if ( UXMath::Equals( prev_offsety, value ) )
						Erase( reader );

					prev_offsety = value;
					break;
				}

				case STRING :
					break;
		
				default :
					WARNING( "unknown chunk!" );
			}
		}
		

		// pass2: unite strings
		reader = GetReader();

		TStringRef<const T>		prev_str;
		EChunkType				prev_chunk = UNKNOWN;

		while ( reader.NextChunk() != END )
		{
			TReader		tmp( reader );
			EChunkType	type = tmp.GetState();

			if ( type == STRING )
			{
				TStringRef<const T>	value = tmp.GetString();

				if ( prev_chunk == type )
				{
					TString<T>	str = prev_str;
					str += value;

					Erase( reader );
					Insert( reader, str );
						
					prev_str = TStringRef<const T>();
				}
				else
					prev_str = value;

				break;
			}
			prev_chunk = type;
		}
	}
	

	template <typename T, typename S, typename A>
	inline void  TExtString<T,S,A>::RemoveChunks (EChunkType type)
	{
		TReader	reader = GetReader();

		while ( reader.NextChunk() != END )
		{
			if ( type == reader.GetState() )
			{
				Erase( reader );
			}
		}
	}


	template <typename T, typename S, typename A>
	inline typename TExtString<T,S,A>::TReader  TExtString<T,S,A>::GetReader () const
	{
		return TReader( *this );
	}


	template <typename T, typename S, typename A>
	inline void  TExtString<T,S,A>::Parse (const TStringRef<const T> &s)
	{
		typedef const TStaticString< T, 10 >	st_str_t;
		typedef const TStringRef< const T >		unistr_t;

		static st_str_t		s_sColor  ( "color"   );
		static st_str_t		s_sScale  ( "scale"   );
		static st_str_t		s_sAlign  ( "align"   );
		static st_str_t		s_sSpacing( "spacing" );
		static st_str_t		s_sOffsetX( "offsetX" );
		static st_str_t		s_sOffsetY( "offsetY" );


		Clear();
		
		EChunkType	e_mode	= STRING;
		usize		u_last	= 0;
		usize		i		= 0;

		for (; i < s.Length();)
		{
			// find key
			if ( s[i] != '#' || s[i+1] != '[' ) {
				++i;
				continue;
			}

			if ( u_last != i )
				Append( TString<T,S,A>( (T*)s.ptr() + u_last, i - u_last ) );

			i += 2;
			u_last = i;

			if ( i >= s.Length() ) break;

			while ( s[i] != ':' and s[i] != ']' and s[i] != 0 ) { ++i; }

			if ( s[i] != ':' ) {
				WARNING( "wrong char" );
				continue;
			}

			unistr_t	s_chunk( (T*)s.ptr() + u_last, i - u_last );
			u_last = ++i;

			while ( s[i] != ']' and s[i] != 0 ) { ++i; }

			unistr_t	s_value( (T*)s.ptr() + u_last, i - u_last );
			u_last = ++i;

			if ( s_chunk.EqualsInRange( (unistr_t) s_sColor, 0, s_chunk.Length() ) ) {
				AddColor( (uint) StringUtils::FromStr16ToInt( s_value ) );
			}
			else
			if ( s_chunk.EqualsInRange( (unistr_t) s_sScale, 0, s_chunk.Length() ) ) {
				AddScale( (float) StringUtils::FromStrToFloat( s_value ) );
			}
			else
			if ( s_chunk.EqualsInRange( (unistr_t) s_sSpacing, 0, s_chunk.Length() ) ) {
				AddSpacing( (float) StringUtils::FromStrToFloat( s_value ) );
			}
			else
			if ( s_chunk.EqualsInRange( (unistr_t) s_sOffsetX, 0, s_chunk.Length() ) ) {
				AddOffsetX( (float) StringUtils::FromStrToFloat( s_value ) );
			}
			else
			if ( s_chunk.EqualsInRange( (unistr_t) s_sOffsetY, 0, s_chunk.Length() ) ) {
				AddOffsetY( (float) StringUtils::FromStrToFloat( s_value ) );
			}
			else
			if ( s_chunk.EqualsInRange( (unistr_t) s_sAlign, 0, s_chunk.Length() ) ) {
				AddAlign( _s_ParseAlign( s_value ) );
			}
			else
				WARNING( "unknonw chunk type" );
		}

		if ( u_last != i )
			Append( TString<T,S,A>( (T*)s.ptr() + u_last, i - u_last ) );

		_bEmpty = s.Empty();
	}
	
	
	template <typename T, typename S, typename A>
	inline void  TExtString<T,S,A>::UnParse (TString<T> &str)
	{
		typedef const TStaticString< T, 10 >	st_str_t;

		static st_str_t		s_sColor  ( "color"   );
		static st_str_t		s_sScale  ( "scale"   );
		static st_str_t		s_sAlign  ( "align"   );
		static st_str_t		s_sSpacing( "spacing" );
		static st_str_t		s_sOffsetX( "offsetX" );
		static st_str_t		s_sOffsetY( "offsetY" );

		TReader	reader = GetReader();

		str.Clear();

		while ( reader.NextChunk() != UNKNOWN )
		{
			switch ( reader.GetState() )
			{
				case COLOR :	str << "#[color:" << String().FormatI( (uint)reader.GetChunkInt(), 16 ) << ']';	break;
				case SCALE :	str << "#[scale:" << reader.GetChunkFloat() << ']';								break;
				case SPACING :	str << "#[spacing:" << reader.GetChunkFloat() << ']';							break;
				case OFFSET_X :	str << "#[offsetX:" << reader.GetChunkFloat() << ']';							break;
				case OFFSET_Y :	str << "#[offsetY:" << reader.GetChunkFloat() << ']';							break;
				case ALIGN :	str << "#[align:";  _s_AddAlign( reader.GetChunkInt(), str );  str << ']';		break;
				case STRING	:	str << /*for debug:*//* "#[]" << reader.GetString();								break;
				case END :		return;
				default :		WARNING( "unknown chunk!" );
			}
		}
	}
	

	template <typename T, typename S, typename A>
	inline void  TExtString<T,S,A>::ExtractString (TString<T> &str) const
	{
		TReader	reader = GetReader();

		str.Clear();

		while ( reader.NextChunk() != UNKNOWN )
		{
			switch ( reader.GetState() )
			{
				case STRING	:	str << reader.GetString();
				case END :		return;
			}
		}
	}

	

	template <typename T, typename S, typename A>
	inline TExtString<T,S,A>::TReader::TReader (const TExtString<T,S,A> &s) :
		_aString(s._aString), _uPos(0), _eState(estr_t::UNKNOWN), _bChunkReaded(false)
	{}
		
	
	template <typename T, typename S, typename A>
	inline TExtString<T,S,A>::TReader::TReader (const Self &reader) :
		_aString(reader._aString), _uPos(reader._uPos), _eState(reader._eState), _bChunkReaded(reader._bChunkReaded)
	{}
	
		
	template <typename T, typename S, typename A>
	inline typename TExtString<T,S,A>::TReader &  TExtString<T,S,A>::TReader::operator = (const Self &reader)
	{
		if ( _aString.ptr() != reader._aString.ptr() )
		{
			WARNING( "!!!" );
			return *this;
		}
		_uPos			= reader._uPos;
		_eState			= reader._eState;
		_bChunkReaded	= reader._bChunkReaded;
		return *this;
	}


	template <typename T, typename S, typename A>
	inline typename TExtString<T,S,A>::EChunkType  TExtString<T,S,A>::TReader::NextChunk ()
	{
		if ( not _bChunkReaded )
		{
			switch ( _eState )
			{
				case COLOR :
				case ALIGN :	GetChunkInt();		break;

				case SCALE :
				case SPACING :
				case OFFSET_X :
				case OFFSET_Y :	GetChunkFloat();	break;

				case STRING	:	GetString();		break;

				case UNKNOWN :	break;

				default :		WARNING( "unknown chunk!" );
			}
		}

		if ( _uPos + sizeof(UCharsToCode) >= _aString.Count() )
		{
			return ( _eState = estr_t::END );
		}

		const UCharsToCode	s_code( _aString[_uPos], _aString[_uPos+1] );

		_uPos += sizeof(UCharsToCode);
		_bChunkReaded = false;
		return _eState = estr_t::_s_CodeToChunk( (typename estr_t::_ECode::type)s_code.uCode );
	}
	

	template <typename T, typename S, typename A>
	inline bool  TExtString<T,S,A>::TReader::Move (uint offset)
	{
		if ( _eState == estr_t::UNKNOWN )
			NextChunk();

		while ( offset != 0 and NextChunk() != END )
		{
			--offset;
		}

		return offset == 0;
	}
	

	template <typename T, typename S, typename A>
	inline bool  TExtString<T,S,A>::TReader::MoveFromStart (uint offset)
	{
		MoveToStart();
		return Move( offset );
	}


	template <typename T, typename S, typename A>
	inline bool  TExtString<T,S,A>::TReader::MoveFromEnd (uint offset)
	{
		MoveToStart();
		usize len = NumChunks();

		if ( offset > len )
		{
			WARNING( "offset is too big!" );
			return false;
		}

		return Move( len - offset );
	}
	

	template <typename T, typename S, typename A>
	inline void  TExtString<T,S,A>::TReader::MoveToStart ()
	{
		_uPos			= 0;
		_eState			= estr_t::UNKNOWN;
		_bChunkReaded	= false;
	}
		

	template <typename T, typename S, typename A>
	inline void  TExtString<T,S,A>::TReader::MoveToEnd ()
	{
		//while ( NextChunk() != END ) {}

		// fast
		_uPos			= _aString.Count();
		_eState			= estr_t::END;
		_bChunkReaded	= true;
	}


	template <typename T, typename S, typename A>
	inline usize  TExtString<T,S,A>::TReader::NumChunks () const
	{
		TReader	reader( *this );

		usize	chunks = 0;

		reader.MoveToStart();

		while ( reader.NextChunk() != END )
		{
			chunks++;
		}
		return chunks;
	}
	

	template <typename T, typename S, typename A>
	inline bool  TExtString<T,S,A>::TReader::Find (EChunkType type)
	{
		while ( NextChunk() != END )
		{
			if ( GetState() == type )
				return true;
		}
		return false;
	}

	
	template <typename T, typename S, typename A>
	inline float  TExtString<T,S,A>::TReader::GetChunkFloat ()
	{
		if ( _uPos >= _aString.Count() )
			return 0.0f;

		ASSERT( _eState == estr_t::SCALE or
				_eState == estr_t::SPACING or
				_eState == estr_t::OFFSET_X or
				_eState == estr_t::OFFSET_Y	);
		ASSERT( not _bChunkReaded );

		const UCharsToType<float>	s_value( _aString.ptr() + _uPos );

		_uPos  += s_value.CHARS_COUNT;
		_eState = estr_t::UNKNOWN;
		_bChunkReaded = true;

		return s_value.tType;
	}


	
	template <typename T, typename S, typename A>
	inline int  TExtString<T,S,A>::TReader::GetChunkInt ()
	{
		if ( _uPos >= _aString.Count() )
			return 0;

		ASSERT( _eState == estr_t::COLOR or
				_eState == estr_t::ALIGN );
		ASSERT( not _bChunkReaded );

		const UCharsToType<int>	s_value( _aString.ptr() + _uPos );

		_uPos  += s_value.CHARS_COUNT;
		_eState = estr_t::UNKNOWN;
		_bChunkReaded = true;

		return s_value.tType;
	}


	
	template <typename T, typename S, typename A>
	inline const TStringRef<const T>  TExtString<T,S,A>::TReader::GetString ()
	{
		if ( _uPos >= _aString.Count() )
			return TStringRef<const T>();

		ASSERT( _eState == estr_t::STRING );
		ASSERT( not _bChunkReaded );

		const UCharsToType<uint>		s_length( _aString.ptr() + _uPos );

		_uPos += s_length.CHARS_COUNT;

		const T *	p_begin = _aString.ptr() + _uPos;
		const T *	p_end	= _aString.ptr() + _uPos + s_length.tType;

		_uPos  += s_length.tType;
		_eState = estr_t::UNKNOWN;
		_bChunkReaded = true;

		return TStringRef<T>( (void *)p_begin, (void *)p_end );
	}
	

}	// UXTypes
}	// UX_STL
*/
