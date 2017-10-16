// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Logger -	log class for write html logs with color highlighting.
				message template: "date - time - [thread ID] - message"
*/

#pragma once

#include "Engine/STL/Files/HDDFile.h"
#include "Engine/STL/Containers/Map.h"
#include "Engine/STL/Math/Color/Color.h"
#include "ELog.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Logger
	//

	class Logger
	{
	// types
	private:
		typedef Map< usize, GXMath::uint2 >		ThreadUniqueColor_t;	// key: threadID, data:{ text_color, background_color }


	// variables
	private:
		File::WFilePtr			_logFile;
		OS::Mutex				_lockLog;
		String					_crashFileName;
		String					_buffer;
		String					_projectFolder;		// project folder name for cuting full file path to short internal path
		ThreadUniqueColor_t		_threadColors;
		OS::PerfomanceTimer		_timer;
		TimeD					_skipErrorMoument;
		bool					_lockRecursion;

		// cache
		String					_lastSrcFile;
		usize					_lastThreadId;
		int						_lastLine;
		ELog::type				_lastType;
		bool					_cached;

		// config
		static const bool		_perThreadColors;
		static const bool		_allowSkipErrors;
		static const bool		_allowCaching;
		static const uint		_minSizeForAutoSpolier;


	// methods
	public:
		Logger ();
		~Logger ();

		void SetProjectFolderName (StringCRef folder);


		// Log File //
		bool Open (StringCRef filename, bool unique);
		void Close ();
		bool IsOpened () const;

		void Write (StringCRef msg, ELog::type type, StringCRef file, int line);


		// Crash File //
		bool CreateCrashFile (StringCRef filename);	// on start application
		bool CloseCrashFile ();						// on stop application

		bool ReadCrashFile (StringCRef name, OUT bool &wasCrashed, OUT OS::Date &time);


		// Utils //
		void SendAnEmail ();

		static Ptr<Logger> GetInstance ();


	private:
		void _ClearCache ();
		bool _CmpWithCache (ELog::type type, StringCRef file, usize threadId, int line);
		
		friend int ::GX_STL::__write_to_log (const char *msg, ELog::type type, const char *file, int line);
		int  _Write (StringCRef msg, ELog::type type, StringCRef file, int line);

		void _AddThreadColor (INOUT String &str, usize threadId);

		static void _GetDate (INOUT String &str);
		static void _GetDateForFName (INOUT String &str);
		static void _AddColor (INOUT String &str, ELog::type type);
		static char _GetPrefix (ELog::type type);

		static String& _ConvertToHtml (String &buf, StringCRef msg);

		static StringCRef _ToShortPath (StringCRef path, StringCRef folder);

		static TimeD _MinTimeDeltaToSkipError ();
	};
	

}	// GXTypes
}	// GX_STL
