// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Log/Logger.h"
#include "Engine/STL/ThreadSafe/Singleton.h"
#include "Engine/STL/Algorithms/StringParser.h"
#include "Engine/STL/Math/Rand/Pseudorandom.h"
#include "Engine/STL/Math/MathTypeCast.h"
#include "Engine/STL/OS/OSLowLevel.h"
#include "Engine/STL/Types/ScopeSetter.h"

namespace GX_STL
{
	using namespace GXTypes;

/*
=================================================
	__debug_console_write
=================================================
*/
	void __debug_console_write (const char *msg, const char *file, int line)
	{
		OS::PlatformUtils::IDEConsoleMessage( msg, file, line );
	}
	
/*
=================================================
	__show_assert
=================================================
*/
	forceinline int __show_assert (StringCRef msg, ELog::type type, StringCRef file, int line)
	{
#		ifdef __GX_LOG_SHOW_WARNING_ENABLED__
		if ( ELog::IsError( type ) )
		{
			OS::PlatformUtils::Dialog::EResult res =
				OS::PlatformUtils::Dialog::ShowAssertion( "Error message", msg, file, line );

			switch ( res )
			{
				case OS::PlatformUtils::Dialog::EResult::Ignore :	return 1;
				case OS::PlatformUtils::Dialog::EResult::Skip :		return 2;
				case OS::PlatformUtils::Dialog::EResult::Retry :	return -1;
				case OS::PlatformUtils::Dialog::EResult::Abort :	return -2;
			}
		}
#		endif
		return 0;
	}

/*
=================================================
	__write_to_log
=================================================
*/
	int __write_to_log (const char *msg, ELog::type type, const char *file, int line)
	{
		__debug_console_write( msg, file, line );

		Ptr<Logger>	log = Logger::GetInstance();
		int			res	= 0;

		if ( log and log->IsOpened() and
			 (res = log->_Write( msg, type, file, line )) != 0 )
		{
			return res;
		}

		return __show_assert( msg, type, file, line );
	}


namespace GXTypes
{

	struct EDefaultLogColors
	{
		enum type
		{
			BLUE		= 0x0000FF,
			NAVY		= 0x0000AB,
			VIOLET		= 0x8A2BE2,

			LIME		= 0x32CD32,
			GREEN		= 0x688E23,
			DARCK_GREEN	= 0x007F00,

			ORANGE		= 0xFF7F00,
			RED			= 0xFF0000,
			//RED			= 0xAA0000,
			DARCK_RED	= 0xA00000,

			SILVER		= 0x909090,
		};
	};
	
/*
=================================================
	Logger config for fast rebuild
=================================================
*/
	const bool		Logger::_perThreadColors		= true;
	const bool		Logger::_allowSkipErrors		= true;
	const bool		Logger::_allowCaching			= false;
	const uint		Logger::_minSizeForAutoSpolier	= 100;
	
	TimeD			Logger::_MinTimeDeltaToSkipError ()		{ return TimeD::FromSeconds( 5.0 ); }

/*
=================================================
	constructor
=================================================
*/
	Logger::Logger () :
		_lockRecursion( false )
	{
		_ClearCache();

		_buffer.Reserve( 256 );

		using namespace GXMath;

		_threadColors.Add(
			OS::Thread::GetCurrentThreadId(),
			uint2( EDefaultLogColors::SILVER, color4u::Html::White().ToIntRGB() )
		);
	}
	
/*
=================================================
	destructor
=================================================
*/
	Logger::~Logger ()
	{
		CHECK( not IsOpened() );			// Log file must be closed!
		CHECK( _crashFileName.Empty() );	// Crash file must be closed!

		Close();
		CloseCrashFile();
	}
	
/*
=================================================
	GetInstance
=================================================
*/
	Ptr<Logger> Logger::GetInstance ()
	{
		return SingletonMultiThread::Instance< Logger >();
	}

/*
=================================================
	SetProjectFolderName
=================================================
*/
	void Logger::SetProjectFolderName (StringCRef folder)
	{
		_projectFolder = folder;
	}

/*
=================================================
	Open
=================================================
*/
	bool Logger::Open (StringCRef name, bool unique)
	{
		Close();


		String	log_name;

		log_name.Reserve( 256 );

		// create directory
		StringCRef	log_path = FileAddress::GetPath( name );

		if ( not OS::FileSystem::IsDirectoryExist( log_path ) )
		{
			CHECK_ERR( OS::FileSystem::CreateDirectories( log_path ) );
		}

		if ( unique )
		{
			for (uint i = 0; i < 100; ++i)
			{
				log_name.Clear();

				log_name << name << "_(";

				_GetDateForFName( log_name );

				log_name << ')';
				
				if ( i != 0 )
					log_name << '_' << i;

				log_name << ".html";

				if ( not OS::FileSystem::IsFileExist( log_name ) )
					break;
			}
		}
		else
		{
			log_name.Clear();
			log_name << name << ".html";
		}
		

		SCOPELOCK( _lockLog );
		
		CHECK_ERR( (_logFile = File::HddWFile::New( log_name )) );

		static const char	header[] =	"<html> <head> <title> log </title> </head> <body BGCOLOR=\"#ffffff\">"
										"<p><PRE><font face=\"Lucida Console, Times New Roman\""
										"size=\"2\" color=\"#000000\">\n";

		_logFile->Write( StringCRef( header ) );
		_logFile->Flush();
		return true;
	}
	
/*
=================================================
	Close
=================================================
*/
	void Logger::Close ()
	{
		SCOPELOCK( _lockLog );

		if ( not _logFile )
			return;
		
		if ( _cached )
			_logFile->Write( StringCRef( "</details>" ) );

		_logFile->Write( StringCRef( "</font></PRE> </p> </body> </html>\n" ) );

		_logFile->Close();
		_logFile = null;

		_ClearCache();
	}
	
/*
=================================================
	IsOpened
=================================================
*/
	bool Logger::IsOpened () const
	{
		return _logFile and _logFile->IsOpened();
	}

/*
=================================================
	CreateCrashFile
=================================================
*/
	bool Logger::CreateCrashFile (StringCRef filename)
	{
		CloseCrashFile();
		
		SCOPELOCK( _lockLog );

		String	crash_file;

		crash_file.Reserve( 256 );
		crash_file = FileAddress::GetPath( filename );

		if ( not OS::FileSystem::IsDirectoryExist( crash_file ) )
		{
			CHECK_ERR( OS::FileSystem::CreateDirectories( crash_file ) );
		}

		crash_file.Clear();
		crash_file << filename << ".crash";

		File::WFilePtr	file;
		CHECK_ERR( (file = File::HddWFile::New( crash_file )) );

		String	msg;

		msg << "Started at ";	_GetDate( msg );
		msg << " - ";

		CHECK_ERR( file->Write( msg.cstr(), msg.LengthInBytes() ) );

		_crashFileName = RVREF( crash_file );
		return true;
	}
	
/*
=================================================
	CloseCrashFile
=================================================
*/
	bool Logger::CloseCrashFile ()
	{
		SCOPELOCK( _lockLog );

		if ( _crashFileName.Empty() )
			return true;
		
		File::WFilePtr	file;
		CHECK_ERR( (file = File::HddWFile::New( _crashFileName, File::HddWFile::EOpenFlags::Append )) );

		String	msg;

		msg << "Closed at ";	_GetDate( msg );
		msg << "\n";

		CHECK_ERR( file->Write( msg.cstr(), msg.LengthInBytes() ) );

		_crashFileName.Clear();
		return true;
	}
	
/*
=================================================
	ReadCrashFile
=================================================
*/
	bool Logger::ReadCrashFile (StringCRef filename, OUT bool &wasCrashed, OUT OS::Date &time)
	{
		SCOPELOCK( _lockLog );

		wasCrashed	= false;
		time		= OS::Date();

		String	fname = filename;

		if ( FileAddress::GetExtension( fname ).Empty() ) {
			fname << ".crash";
		}

		if ( not _crashFileName.Empty() ) {
			CHECK_ERR( _crashFileName != fname );
		}
		

		File::RFilePtr	file;

		if ( not (file = File::HddRFile::New( fname )) )
			return false;

		// read file
		String	data;
		BytesU	size = file->Size();

		CHECK_ALLOC( data.Reserve( (usize)size + 1 ) );

		CHECK_ERR( file->Read( data.ptr(), size ) );
		data.SetLength( (usize)size );

		// is started and closed?
		usize	pos = 0;
		bool	start_found = false;

		while ( data.Find( "Started", pos, pos ) )	{ start_found = true; ++pos; }

		if ( start_found and
			 not data.Find( "Closed", pos, pos ) )
		{
			wasCrashed = true;
		}

		return true;
	}
	
/*
=================================================
	SendAnEmail
=================================================
*/
	void Logger::SendAnEmail ()
	{
		TODO( "SendEmail" );
		//ESS()->GetApplication()->GetPlatform()->SendEmail();
	}
	
/*
=================================================
	Write
=================================================
*/
	void Logger::Write (StringCRef msg, ELog::type type, StringCRef file, int line)
	{
		_Write( msg, type, file, line );
	}

	int Logger::_Write (StringCRef msg, ELog::type type, StringCRef file, int line)
	{
		SCOPELOCK( _lockLog );
		
		if ( msg.Empty() or not IsOpened() )
			return 0;

		// recursive write to '_buffer' is denied
		if ( _lockRecursion )
		{
			GX_BREAK_POINT();
			return 0;
		}
		SCOPE_SETTER( _lockRecursion = true, false );

		const usize	thread_id	 = OS::Thread::GetCurrentThreadId();
		const bool	with_spoiler = EnumEq( type, ELog::SpoilerFlag ) or msg.Length() > _minSizeForAutoSpolier;
		const bool	prev_cached	 = _cached;
		const bool	is_cached	 = _CmpWithCache( type, file, thread_id, line );

		String &	str			 = _buffer;		str.Clear();

		// remove 'new line'
		if ( msg.Back() == '\n' )
			msg = msg.SubString( 0, msg.Length()-1 );

		if ( prev_cached and is_cached )
		{
			str << '\n' << msg;
		}
		else
		if ( is_cached )
		{
			// append to previous message
			_AddColor( str, type );

			str << "<details><summary>...</summary>" << msg;
		}
		else
		{
			if ( prev_cached )
				str << "</details>";

			// message type prefix
			_AddColor( str, type );
			str << _GetPrefix( type );

			// date and time
			_AddColor( str, ELog::_SrcFile );
			str << " - ";
			_GetDate( str );
			str << " - ";

			// current thread
			_AddThreadColor( str, thread_id );
			str << "[" << String().FormatAlignedI( thread_id, 8, '0', 16 ) << "]";
	
			// source file and line number
			_AddColor( str, ELog::_SrcFile );
			str << " - (file: " << _ToShortPath( file, _projectFolder ) << ", line: " << line << ")\n";
		
			// message
			_AddColor( str, type );

			if ( with_spoiler )
			{
				StringCRef	summary;
				usize		pos = 0;
				StringParser::ReadLineToEnd( msg, OUT pos, OUT summary );

				if ( EnumEq( type, ELog::OpenSpoilerFlag ) or ELog::IsError( type ) )
					str << "<details open>";
				else
					str << "<details>";

				str << "<summary>" << summary << "</summary>";
				_ConvertToHtml( str, msg.SubString(pos) ) << "</details>";
			}
			else
				_ConvertToHtml( str, msg ) << '\n';
		}

		_logFile->Write( StringCRef( str ) );
		_logFile->Flush();

		// show dialog
		int res = 1000;

		if ( not _allowSkipErrors or
			 _skipErrorMoument < _timer.GetTime() )
		{
			res = __show_assert( msg, type, file, line );

			// if 'ignore'
			if ( res == 1 )
			{
				// get time when user selected answer
				_skipErrorMoument = _timer.GetTime() + _MinTimeDeltaToSkipError();
			}
		}

		return res;
	}
	
/*
=================================================
	_ClearCache
=================================================
*/
	void Logger::_ClearCache ()
	{
		_cached			= false;
		_lastType		= ELog::type(-1);
		_lastLine		= 0;
		_lastThreadId	= 0;
		_lastSrcFile.Clear();
	}
	
/*
=================================================
	_CmpWithCache
=================================================
*/
	bool Logger::_CmpWithCache (ELog::type type, StringCRef file, usize threadId, int line)
	{
		if ( _allowCaching							and
			 _lastType == type						and
			 not EnumEq( type, ELog::SpoilerFlag )	and
			 //not ELog::IsError( type )			and
			 _lastSrcFile == file					and
			 _lastThreadId == threadId				and
			 _lastLine == line )
		{
			_cached = true;
			return true;
		}

		_cached			= false;
		_lastType		= type;
		_lastLine		= line;
		_lastThreadId	= threadId;
		_lastSrcFile	= file;

		return false;
	}
	
/*
=================================================
	_AddThreadColor
=================================================
*/
	inline void Logger::_AddThreadColor (INOUT String &str, usize threadId)
	{
		using namespace GXMath;

		if ( not _perThreadColors )
			return;
		
		uint2	color;

		ThreadUniqueColor_t::const_iterator	iter;

		if ( _threadColors.Find( threadId, OUT iter ) )
		{
			color = iter->second;
		}
		else
		{
			float	value	= TypeCast::ToUNorm( Pseudorandom::Float( threadId ) ) * 0.7f;
			//float	value	= Random::UNorm<float>();
			color4f	fcolor	= ColorUtils::RainbowRGB( value ).xyzo();
			float	lum		= ColorUtils::Luminance( fcolor );

			color.x	= (lum > 0.6f ? color4u::Html::Black() : color4u::Html::White()).ToIntRGB();
			color.y	= color4u( fcolor ).ToIntRGB();

			_threadColors.Add( threadId, color );
		}

		str << "</font><font color=\"#" << String().FormatAlignedI( color.x, 6, '0', 16 )
			<< "\"; style=\"background-color: #" << String().FormatAlignedI( color.y, 6, '0', 16 ) << "\">";
	}
	
/*
=================================================
	_AddColor
=================================================
*/
	inline void Logger::_AddColor (INOUT String &str, ELog::type type)
	{
		uint	color = 0;

		switch ( type & ELog::_FlagsMask )
		{
			case ELog::Debug	:	color = EDefaultLogColors::NAVY;			break;
			case ELog::Info		:	color = EDefaultLogColors::DARCK_GREEN;		break;
			case ELog::Warning	:	color = EDefaultLogColors::ORANGE;			break;
			case ELog::Error	:	color = EDefaultLogColors::RED;				break;
			case ELog::Fatal	:	color = EDefaultLogColors::DARCK_RED;		break;
			case ELog::_SrcFile	:	color = EDefaultLogColors::SILVER;			break;
		}
		
		str << "</font><font color=\"#" << String().FormatAlignedI( color, 6, '0', 16 ) << "\">";
	}
	
/*
=================================================
	_GetPrefix
=================================================
*/
	inline char Logger::_GetPrefix (ELog::type type)
	{
		switch ( type & ELog::_FlagsMask )
		{
			case ELog::Debug	:	return 'D';
			case ELog::Info		:	return 'I';
			case ELog::Warning	:	return 'W';
			case ELog::Error	:	return 'E';
			case ELog::Fatal	:	return 'F';
		}
		return ' ';
	}

/*
=================================================
	_GetDate
----
	add date to string, format: "yyyy/mm/dm - hh:mm:ss"
=================================================
*/
	inline void Logger::_GetDate (INOUT String &str)
	{
		str << OS::Date().Now().ToString( "yyyy/mm/dm - hh:mi:ss" );
	}
	
/*
=================================================
	_GetDateForFName
----
	add date to string, format: "yyyy.mm.dm-hh.mm.ss"
=================================================
*/
	inline void Logger::_GetDateForFName (INOUT String &str)
	{
		str << OS::Date().Now().ToString( "yyyy.mm.dm-hh.mi.ss" );
	}
	
/*
=================================================
	_ToShortPath
=================================================
*/
	inline StringCRef Logger::_ToShortPath (StringCRef path, StringCRef folder)
	{
		usize	pos;

		if ( path.Find( folder, OUT pos, 0 ) )
		{
			return path.SubString( pos + folder.Length() );
		}
		return FileAddress::ToShortPath( path, 3 );
	}
	
/*
=================================================
	_ConvertToHtml
=================================================
*/
	inline String& Logger::_ConvertToHtml (String &buf, StringCRef msg)
	{
		static const StringCRef		angle_left	= "&lt;";
		static const StringCRef		angle_right	= "&gt;";
		static const StringCRef		ampersand	= "&amp;";

		usize j = buf.Length();

		buf.Resize( buf.Length() + msg.Length() + 16, '\0' );

		FOR( i, msg )
		{
			switch ( msg[i] )
			{
				case '<' :
					buf.Insert( angle_left, j );
					j += angle_left.Length();
					break;

				case '>' :
					buf.Insert( angle_right, j );
					j += angle_right.Length();
					break;

				case '&' :
					buf.Insert( ampersand, j );
					j += ampersand.Length();
					break;

				default :
					buf[j++] = msg[i];
			}
		}

		buf.SetLength( j );
		return buf;
	}


}	// GXTypes
}	// GX_STL
