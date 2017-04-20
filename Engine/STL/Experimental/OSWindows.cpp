// Copyright © 2014-2016  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Common/Init.h"

#ifdef PLATFORM_WINDOWS

#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"

namespace GX_STL
{
namespace OS
{
	
/*
=================================================
	_CreateLinkWin32
=================================================
*/
	bool _CreateLinkWin32 (const char *linkFilename, const char *link, bool isFolder)
	{
		HRESULT hres;
		IShellLinkA* psl;
 
		hres = CoInitialize( 0 );
		
		if ( FAILED(hres) )
			return false;

		hres = CoCreateInstance( isFolder ? CLSID_FolderShortcut : CLSID_ShellLink,
								 NULL,
								 CLSCTX_INPROC_SERVER,
								 IID_IShellLink,
								 (LPVOID*)&psl );

		if ( FAILED(hres) )
			return false;
 
		psl->SetPath( link );
		//psl->SetDescription( "" );
		
		IPersistFile* ppf;
 
		hres = psl->QueryInterface( IID_IPersistFile, (LPVOID*)&ppf );
 
		if ( SUCCEEDED(hres) )
		{
			WCHAR wsz[MAX_PATH]; 
 
			MultiByteToWideChar( CP_ACP, 0, linkFilename, -1, wsz, MAX_PATH ); 
            
			hres = ppf->Save( wsz, TRUE ); 
			ppf->Release(); 
		}

		psl->Release();
		CoUninitialize();

		if ( FAILED(hres) )
			return false;

		return true;
	}
	
/*
=================================================
	_GetkWinApiError
=================================================
*/
	bool _GetkWinApiError (const char *buf, int &bufSize)
	{
		DWORD	dw = ::GetLastError();

		if ( dw == 0 )
			return true;

		DWORD	dw_count = ::FormatMessageA(
									FORMAT_MESSAGE_FROM_SYSTEM |
									FORMAT_MESSAGE_IGNORE_INSERTS,
									0,
									dw,
									MAKELANGID( LANG_ENGLISH, SUBLANG_DEFAULT ),
									(LPTSTR) buf,
									bufSize,
									0 );

		bufSize = dw_count;
		return false;
	}


}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS