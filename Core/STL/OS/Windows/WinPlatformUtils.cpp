// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/STL.Config.h"

#ifdef PLATFORM_WINDOWS

#include "Core/STL/OS/Windows/WinPlatformUtils.h"
#include "Core/STL/OS/Windows/WinFileSystem.h"
#include "Core/STL/Math/Interpolations.h"
#include "Core/STL/OS/Base/BaseFileSystem.h"
#include "Core/STL/OS/Windows/WinHeader.h"
#include "Core/STL/Log/ToString.h"

#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"

//#pragma comment (lib, "Comctl32.lib")

namespace GX_STL
{
namespace OS
{
	static bool _CreateLinkWin32 (const char *linkFilename, const char *link, bool isFolder);
	static bool _GetWinApiError (INOUT char *buf, INOUT uint &bufSize);
	
/*
=================================================
	Run
=================================================
*/
	bool PlatformUtils::Run (StringCRef commands, TimeL timeout)
	{
		char	buf[MAX_PATH] = {};
		::GetSystemDirectoryA( buf, UINT(CountOf(buf)) );
		
		String		command_line;
		command_line << '"' << buf << "\\cmd.exe\" /C " << commands;

		STARTUPINFOA			startup_info = {};
		PROCESS_INFORMATION		proc_info	 = {};
		
		bool process_created = ::CreateProcessA(
			NULL,
			command_line.ptr(),
			NULL,
			NULL,
			FALSE,
			CREATE_NO_WINDOW,
			NULL,
			NULL,
			OUT &startup_info,
			OUT &proc_info
		);

		if ( not process_created )
			return false;

		if ( ::WaitForSingleObject( proc_info.hThread, uint(timeout.MilliSeconds()) ) != WAIT_OBJECT_0 )
			return false;
		
		DWORD process_exit;
		::GetExitCodeProcess( proc_info.hProcess, OUT &process_exit );

		return true;
	}

/*
=================================================
	OpenURL
=================================================
*/
	bool PlatformUtils::OpenURL (StringCRef url)
	{
		return usize(::ShellExecuteA( null, "open", url.cstr(), null, null, SW_SHOWNORMAL )) > 32;
	}
	
/*
=================================================
	OpenFile
=================================================
*/
	bool PlatformUtils::OpenFile (StringCRef filename)
	{
		return usize(::ShellExecuteA( null, null, filename.cstr(), null, null, SW_SHOWNORMAL )) > 32;
	}

/*
=================================================
	CreateLink
=================================================
*/
	bool PlatformUtils::CreateLink (StringCRef linkFilename, StringCRef link)
	{
		return _CreateLinkWin32( linkFilename.cstr(), link.cstr(), not FileSystem::IsFileExist( link ) );
	}
	
/*
=================================================
	IDEConsoleMessage
=================================================
*/
	void PlatformUtils::IDEConsoleMessage (StringCRef message, StringCRef file, int line)
	{
		String	str;
		str << file << '(' << line << "): " << message << '\n';

		::OutputDebugStringA( str.cstr() );
	}
	
/*
=================================================
	ValidateHeap
=================================================
*/
	bool PlatformUtils::ValidateHeap ()
	{
		const DWORD	count		= ::GetProcessHeaps( 0, null );
		HANDLE *	ph_heaps	= new HANDLE[count]; //(HANDLE *)_alloca( sizeof(HANDLE) * count );
		bool		result		= true;

		::GetProcessHeaps( count, ph_heaps );

		for (uint index = 0; index < count; ++index)
		{
			if( not ::HeapValidate( ph_heaps[ index ], 0, null ) )
			{
				result = false;
				//assert(false);
				//::FatalAppExitA( 1, "heaps validation failed" );
			}
		}

		delete[] ph_heaps;

		return result;
	}

/*
=================================================
	ShowAssertion
=================================================
*/
	PlatformUtils::Dialog::EResult  PlatformUtils::Dialog::ShowAssertion (StringCRef caption, StringCRef msg, StringCRef file, int line)
	{
		if ( msg.Length() > 1024 )
			msg = msg.SubString( 0, 1024 );

		// keep it for WinXP, 7
	#if 1
		String	str;
		str << "File: " << file
			<< "\nLine: " << line
			<< "\n\nMessage:\n" << msg
			<< "\n\n(Yes - debug application,\n No - skip message,\n Cancel - mute messages)";

		int	result = ::MessageBoxExA( null, str.cstr(), caption.cstr(),
									  MB_YESNOCANCEL | MB_ICONERROR | MB_SETFOREGROUND | MB_TOPMOST | MB_DEFBUTTON2,
									  MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ) );

		switch ( result )
		{
			case IDYES :		return EResult::Retry;		// debug app
			case IDNO :			return EResult::Skip;		// skip current message
			case IDCANCEL :		return EResult::Ignore;		// skip some messages
			default :			return EResult::Unknown;
		};
		
	#else

		String	str;
		str << "File: " << file
			<< "\nLine: " << line
			<< "\n\nMessage:\n" << msg
			<< "\n\n(Press Retry to debug application)";

		StaticArray< TASKDIALOG_BUTTON, 4 >	buttons {
			{ (int)EResult::Skip,	L"Skip" },
			{ (int)EResult::Ignore,	L"Ignore" },
			{ (int)EResult::Retry,	L"Retry" },
			{ (int)EResult::Abort,	L"Abort" }
		};

		TASKDIALOGCONFIG	config	= {};
		config.cbSize				= sizeof(config);
		config.hwndParent			= null;
		config.hInstance			= GetModuleHandle( null );
		config.dwFlags				= 0;
		config.pszWindowTitle		= L"caption";
		//config.pszMainIcon			= TD_WARNING_ICON;
		config.pszMainInstruction	= L"instruction";
		config.pszContent			= L"content";
		config.cButtons				= buttons.Count();
		config.pButtons				= buttons.ptr();
		config.nDefaultButton		= 0;

		int		result_btn = (int)EResult::Unknown;

		TaskDialogIndirect( &config, &result_btn, null, null );

		return (EResult) result_btn;
	#endif
	}

/*
=================================================
	_CheckError
=================================================
*/
	bool PlatformUtils::_CheckError (StringCRef file, int line)
	{
		char	buf[128] = {};
		uint	size	 = uint(CountOf(buf));

		if ( _GetWinApiError( buf, size ) )
			return true;

		String	str("WinAPI error: ");

		str << StringCRef( buf, size-2 );

		LOG( str, ELog::Warning, file.cstr(), line );
		return false;
	}
	
/*
=================================================
	_CreateLinkWin32
=================================================
*/
	static bool _CreateLinkWin32 (const char *linkFilename, const char *link, bool isFolder)
	{
		HRESULT hres;
		IShellLinkA* psl;
 
		hres = ::CoInitialize( null );
		
		if ( FAILED(hres) )
			return false;

		hres = ::CoCreateInstance( isFolder ? CLSID_FolderShortcut : CLSID_ShellLink,
									null,
									CLSCTX_INPROC_SERVER,
									IID_IShellLink,
									Cast<LPVOID*>(&psl) );

		if ( FAILED(hres) )
			return false;
 
		psl->SetPath( link );
		//psl->SetDescription( "" );
		
		IPersistFile* ppf;
 
		hres = psl->QueryInterface( IID_IPersistFile, Cast<LPVOID*>(&ppf) );
 
		if ( SUCCEEDED(hres) )
		{
			WCHAR wsz[MAX_PATH]; 
 
			::MultiByteToWideChar( CP_ACP, 0, linkFilename, -1, wsz, MAX_PATH ); 
            
			hres = ppf->Save( wsz, TRUE ); 
			ppf->Release(); 
		}

		psl->Release();
		::CoUninitialize();

		if ( FAILED(hres) )
			return false;

		return true;
	}
	
/*
=================================================
	_GetWinApiError
=================================================
*/
	static bool _GetWinApiError (INOUT char *buf, INOUT uint &bufSize)
	{
		DWORD	dw = ::GetLastError();

		if ( dw == 0 )
			return true;

		DWORD	dw_count = ::FormatMessageA(
									FORMAT_MESSAGE_FROM_SYSTEM |
									FORMAT_MESSAGE_IGNORE_INSERTS,
									null,
									dw,
									MAKELANGID( LANG_ENGLISH, SUBLANG_DEFAULT ),
									LPTSTR(buf),
									bufSize,
									null );

		bufSize = dw_count;
		return false;
	}


}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS
