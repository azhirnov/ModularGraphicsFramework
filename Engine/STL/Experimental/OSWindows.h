// Copyright © 2014-2016  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/OS/Base/Common.h"

#ifdef PLATFORM_WINDOWS

#if defined( PLATFORM_SDL )
	// disable WinAPI Shells
#	undef PLATFORM_WINDOWS_SHELL
#	include "Engine/STL/OS/SDL/OS_SDL.h"
#else
	// enable WinAPI Shells
#	define PLATFORM_WINDOWS_SHELL
#endif


#define _WINDOWS_		// disable <Windows.h>
/*
#define _WINDEF_		// disable <windef.h>
#define _WINBASE_		// disable <winbase.h>
#define MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS	0
#define _WINGDI_		// disable <wingdi.h>
#define _INC_PROCESS	// disable <process.h>
#define _WINUSER_		// disable <winuser.h>
#define _WINNLS_		// disable <winnls.h>
#define _WINCON_		// disable <wincon.h>
#define VER_H			// disable <winver.h>
#define _WINREG_		// disable <winreg.h>
#define _WINNETWK_		// disable <winnetwk.h>
#define __STRALIGN_H_	// disable <stralign.h>
#define _WINSVC_		// disable <winsvc.h>
#define _IMM_			// disable <imm.h>
#define _BASETSD_H_		// disable <basetsd.h>
#define _WINERROR_		// disable <winerror.h>
#define __wtypes_h__	// disable <wtypes.h>
#define __unknwn_h__	// disable <unknown.h>
#define __objidl_h__	// disable <objidl.h>
#define _OBJBASE_H_		// disable <objbase.h>
#define __oaidl_h__		// disable <oaidl.h>
#define _OLEAUTO_H_		// dsiable <olaauto.h>
#define __oleidl_h__	// disable <oleidl.h>
#define _OLE2_H_		// disable <ole2.h>
#define __wbemcli_h__	// disable <wbemcli.h>
#define _INC_COMUTIL	// disable <comutil.h>
*/

namespace GX_STL
{
namespace winapi
{
	using namespace GXTypes;
	
#	undef __in_opt
#	undef __in
#	undef __out_opt
#	undef __out
#	undef __inout
#	undef __inout_opt
#	undef __reserved
#	undef __deref_out
#	undef __deref
#	undef CONST
#	undef WINAPI
#	undef WINBASEAPI
#	undef WINUSERAPI
#	undef WINGDIAPI
#	undef WINADVAPI
#	undef APIENTRY
#	undef DECLARE_HANDLE
	
#	define __in_opt
#	define __in
#	define __out_opt
#	define __out
#	define __inout
#	define __inout_opt
#	define __reserved
#	define __deref_out
#	define __deref
#	define CONST				const
#	define WINAPI				CALLBACK
#	define WINBASEAPI			GX_DLL_IMPORT
#	define WINUSERAPI			GX_DLL_IMPORT
#	define WINGDIAPI			GX_DLL_IMPORT
#	define WINADVAPI			GX_DLL_IMPORT
#	define APIENTRY				WINAPI
#	define DECLARE_HANDLE(name)	struct name##__{int unused;}; typedef struct name##__ *name
	
#	ifndef SHSTDAPI
#	 if !defined(_SHELL32_)
#		define SHSTDAPI          extern "C" GX_DLL_IMPORT HRESULT CALLBACK
#		define SHSTDAPI_(type)   extern "C" GX_DLL_IMPORT type CALLBACK
#	 else
#		define SHSTDAPI          STDAPI
#		define SHSTDAPI_(type)   STDAPI_(type)
#	 endif
#	endif // SHSTDAPI

	typedef void			VOID;
	typedef ubyte			BYTE, *	LPBYTE, *PBYTE, BOOLEAN;
	typedef char			CHAR;
	typedef wchar			WCHAR;
	typedef ushort			WORD, USHORT, ATOM;
	typedef short			SHORT;
	typedef int				BOOL, *PBOOL, LONG, HRESULT, *LPLONG;
	typedef uint			UINT, *PUINT, ULONG, DWORD, *LPDWORD;
	typedef isize			LRESULT, LONG_PTR, INT_PTR, *PINT_PTR, *PLONG_PTR, LPARAM;
	typedef usize			SIZE_T, ULONG_PTR, UINT_PTR, *PUINT_PTR, *PULONG_PTR, WPARAM;
	typedef ilong			LONGLONG;
	typedef char *			LPSTR;
	typedef const char *	LPCSTR;
	typedef const wchar *	LPCWSTR;
	typedef wchar *			LPWSTR;
	typedef void			*HANDLE, *PVOID, *LPVOID;
	typedef HANDLE			HLOCAL, *LPHANDLE, HGLOBAL, GLOBALHANDLE, LOCALHANDLE, *PHANDLE;
	typedef WORD   *		LPWORD;
	typedef ulong			ULONGLONG, DWORDLONG, *PDWORDLONG;
	typedef DWORD			COLORREF, *LPCOLORREF;
	typedef ULONG_PTR		DWORD_PTR, *PDWORD_PTR;
	typedef DWORD			ACCESS_MASK;
	typedef ACCESS_MASK		REGSAM;
	typedef float			FLOAT;
	typedef double			DOUBLE;
	typedef int			INT32, INT;
	typedef ilong			INT64;
	typedef ulong			UINT64;

	DECLARE_HANDLE( HWND );
	DECLARE_HANDLE( HINSTANCE );
	DECLARE_HANDLE( HRSRC );
	DECLARE_HANDLE( HGLRC );
	DECLARE_HANDLE( HICON );
	DECLARE_HANDLE( HBRUSH );
	DECLARE_HANDLE( HDC );
	DECLARE_HANDLE( HKL );
	DECLARE_HANDLE( HMENU );
	DECLARE_HANDLE( HRGN );
	DECLARE_HANDLE( HKEY );
	DECLARE_HANDLE( HFONT );
	DECLARE_HANDLE( HRAWINPUT );
	DECLARE_HANDLE( HBITMAP );

	typedef HKEY *			PHKEY;
	typedef HINSTANCE		HMODULE;
	typedef HICON			HCURSOR;
	
	inline LPWSTR MAKEINTRESOURCEW(int i);
	inline LPSTR MAKEINTRESOURCEA(int i);
	
	typedef LONG	LSTATUS;
	
	typedef VOID (CALLBACK* TIMERPROC)(HWND, UINT, UINT_PTR, DWORD);
	typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
	typedef int (WINAPI *PROC)();
	

	typedef struct _LUID {
		DWORD LowPart;
		LONG HighPart;
	} LUID, *PLUID;

	struct LARGE_INTEGER {
		LONGLONG	QuadPart;
	};
	typedef LARGE_INTEGER *	PLARGE_INTEGER;

	struct ULARGE_INTEGER {
		ULONGLONG	QuadPart;
	};
	typedef ULARGE_INTEGER *	PULARGE_INTEGER;
	
	typedef struct tagPOINT
	{
		LONG  x;
		LONG  y;
	} POINT, *PPOINT, *NPPOINT, *LPPOINT;
	
	typedef struct tagMSG {
		HWND        hwnd;
		UINT        message;
		WPARAM      wParam;
		LPARAM      lParam;
		DWORD       time;
		POINT       pt;
	#ifdef _MAC
		DWORD       lPrivate;
	#endif
	} MSG, *PMSG, *NPMSG, *LPMSG;

	typedef struct tagRECT
	{
		LONG    left;
		LONG    top;
		LONG    right;
		LONG    bottom;
	} RECT, *PRECT, *LPRECT;

	typedef struct _SECURITY_ATTRIBUTES {
		DWORD		nLength;
		LPVOID		lpSecurityDescriptor;
		BOOL		bInheritHandle;
	} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;
	
	typedef struct _SYSTEMTIME {
		WORD wYear;
		WORD wMonth;
		WORD wDayOfWeek;
		WORD wDay;
		WORD wHour;
		WORD wMinute;
		WORD wSecond;
		WORD wMilliseconds;
	} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

	typedef struct _RTL_SRWLOCK {
			PVOID	Ptr;
	} RTL_SRWLOCK, *PRTL_SRWLOCK, SRWLOCK, *PSRWLOCK;
	   
	typedef struct _RTL_CONDITION_VARIABLE {
			PVOID	Ptr;      
	} RTL_CONDITION_VARIABLE, *PRTL_CONDITION_VARIABLE, CONDITION_VARIABLE, *PCONDITION_VARIABLE;

	typedef isize (CALLBACK *FARPROC)();
	
	typedef struct _LIST_ENTRY {
	   struct _LIST_ENTRY *Flink;
	   struct _LIST_ENTRY *Blink;
	} LIST_ENTRY, *PLIST_ENTRY, *PRLIST_ENTRY;

	typedef struct _RTL_CRITICAL_SECTION_DEBUG {
		WORD   Type;
		WORD   CreatorBackTraceIndex;
		struct _RTL_CRITICAL_SECTION *CriticalSection;
		LIST_ENTRY ProcessLocksList;
		DWORD EntryCount;
		DWORD ContentionCount;
		DWORD Flags;
		WORD   CreatorBackTraceIndexHigh;
		WORD   SpareWORD;
	} RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG, RTL_RESOURCE_DEBUG, *PRTL_RESOURCE_DEBUG;

#	pragma pack(push, 8)
	typedef struct _RTL_CRITICAL_SECTION {
		PRTL_CRITICAL_SECTION_DEBUG		DebugInfo;

		//
		//  The following three fields control entering and exiting the critical
		//  section for the resource
		//

		LONG		LockCount;
		LONG		RecursionCount;
		HANDLE		OwningThread;        // from the thread's ClientId->UniqueThread
		HANDLE		LockSemaphore;
		ULONG_PTR	SpinCount;        // force size on 64-bit systems when packed
	} CRITICAL_SECTION, *PCRITICAL_SECTION, *LPCRITICAL_SECTION;
#	pragma pack(pop)
	
	typedef struct _FILETIME {
		DWORD dwLowDateTime;
		DWORD dwHighDateTime;
	} FILETIME, *PFILETIME, *LPFILETIME;

	typedef struct _BY_HANDLE_FILE_INFORMATION {
		DWORD dwFileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		DWORD dwVolumeSerialNumber;
		DWORD nFileSizeHigh;
		DWORD nFileSizeLow;
		DWORD nNumberOfLinks;
		DWORD nFileIndexHigh;
		DWORD nFileIndexLow;
	} BY_HANDLE_FILE_INFORMATION, *PBY_HANDLE_FILE_INFORMATION, *LPBY_HANDLE_FILE_INFORMATION;
	

	typedef struct tagWNDCLASSA {
		UINT        style;
		WNDPROC     lpfnWndProc;
		int         cbClsExtra;
		int         cbWndExtra;
		HINSTANCE   hInstance;
		HICON       hIcon;
		HCURSOR     hCursor;
		HBRUSH      hbrBackground;
		LPCSTR      lpszMenuName;
		LPCSTR      lpszClassName;
	} WNDCLASSA, *PWNDCLASSA, *NPWNDCLASSA, *LPWNDCLASSA;

	typedef struct tagWNDCLASSW {
		UINT        style;
		WNDPROC     lpfnWndProc;
		int         cbClsExtra;
		int         cbWndExtra;
		HINSTANCE   hInstance;
		HICON       hIcon;
		HCURSOR     hCursor;
		HBRUSH      hbrBackground;
		LPCWSTR     lpszMenuName;
		LPCWSTR     lpszClassName;
	} WNDCLASSW, *PWNDCLASSW, *NPWNDCLASSW, *LPWNDCLASSW;

	typedef struct tagWNDCLASSEXA {
		UINT        cbSize;
		/* Win 3.x */
		UINT        style;
		WNDPROC     lpfnWndProc;
		int         cbClsExtra;
		int         cbWndExtra;
		HINSTANCE   hInstance;
		HICON       hIcon;
		HCURSOR     hCursor;
		HBRUSH      hbrBackground;
		LPCSTR      lpszMenuName;
		LPCSTR      lpszClassName;
		/* Win 4.0 */
		HICON       hIconSm;
	} WNDCLASSEXA, *PWNDCLASSEXA, *NPWNDCLASSEXA, *LPWNDCLASSEXA;

	typedef struct tagWNDCLASSEXW {
		UINT        cbSize;
		/* Win 3.x */
		UINT        style;
		WNDPROC     lpfnWndProc;
		int         cbClsExtra;
		int         cbWndExtra;
		HINSTANCE   hInstance;
		HICON       hIcon;
		HCURSOR     hCursor;
		HBRUSH      hbrBackground;
		LPCWSTR     lpszMenuName;
		LPCWSTR     lpszClassName;
		/* Win 4.0 */
		HICON       hIconSm;
	} WNDCLASSEXW, *PWNDCLASSEXW, *NPWNDCLASSEXW, *LPWNDCLASSEXW;

	typedef struct _WGLSWAP
	{
		HDC hdc;
		UINT uiFlags;
	} WGLSWAP;
	
	typedef struct _MEMORYSTATUS {
		DWORD dwLength;
		DWORD dwMemoryLoad;
		SIZE_T dwTotalPhys;
		SIZE_T dwAvailPhys;
		SIZE_T dwTotalPageFile;
		SIZE_T dwAvailPageFile;
		SIZE_T dwTotalVirtual;
		SIZE_T dwAvailVirtual;
	} MEMORYSTATUS, *LPMEMORYSTATUS;

	typedef struct _MEMORYSTATUSEX {
		DWORD dwLength;
		DWORD dwMemoryLoad;
		DWORDLONG ullTotalPhys;
		DWORDLONG ullAvailPhys;
		DWORDLONG ullTotalPageFile;
		DWORDLONG ullAvailPageFile;
		DWORDLONG ullTotalVirtual;
		DWORDLONG ullAvailVirtual;
		DWORDLONG ullAvailExtendedVirtual;
	} MEMORYSTATUSEX, *LPMEMORYSTATUSEX;
	
	/* Layer plane descriptor */
	typedef struct tagLAYERPLANEDESCRIPTOR { // lpd
		WORD  nSize;
		WORD  nVersion;
		DWORD dwFlags;
		BYTE  iPixelType;
		BYTE  cColorBits;
		BYTE  cRedBits;
		BYTE  cRedShift;
		BYTE  cGreenBits;
		BYTE  cGreenShift;
		BYTE  cBlueBits;
		BYTE  cBlueShift;
		BYTE  cAlphaBits;
		BYTE  cAlphaShift;
		BYTE  cAccumBits;
		BYTE  cAccumRedBits;
		BYTE  cAccumGreenBits;
		BYTE  cAccumBlueBits;
		BYTE  cAccumAlphaBits;
		BYTE  cDepthBits;
		BYTE  cStencilBits;
		BYTE  cAuxBuffers;
		BYTE  iLayerPlane;
		BYTE  bReserved;
		COLORREF crTransparent;
	} LAYERPLANEDESCRIPTOR, *PLAYERPLANEDESCRIPTOR, *LPLAYERPLANEDESCRIPTOR;
	
	typedef struct _POINTL      /* ptl  */
	{
		LONG  x;
		LONG  y;
	} POINTL, *PPOINTL;

	const uint	CCHDEVICENAME	= 32;
	const uint	CCHFORMNAME		= 32;

	typedef struct _devicemodeA {
		BYTE   dmDeviceName[CCHDEVICENAME];
		WORD dmSpecVersion;
		WORD dmDriverVersion;
		WORD dmSize;
		WORD dmDriverExtra;
		DWORD dmFields;
		union {
		  /* printer only fields */
		  struct {
			short dmOrientation;
			short dmPaperSize;
			short dmPaperLength;
			short dmPaperWidth;
			short dmScale;
			short dmCopies;
			short dmDefaultSource;
			short dmPrintQuality;
		  };
		  /* display only fields */
		  struct {
			POINTL dmPosition;
			DWORD  dmDisplayOrientation;
			DWORD  dmDisplayFixedOutput;
		  };
		};
		short dmColor;
		short dmDuplex;
		short dmYResolution;
		short dmTTOption;
		short dmCollate;
		BYTE   dmFormName[CCHFORMNAME];
		WORD   dmLogPixels;
		DWORD  dmBitsPerPel;
		DWORD  dmPelsWidth;
		DWORD  dmPelsHeight;
		union {
			DWORD  dmDisplayFlags;
			DWORD  dmNup;
		};
		DWORD  dmDisplayFrequency;
		DWORD  dmICMMethod;
		DWORD  dmICMIntent;
		DWORD  dmMediaType;
		DWORD  dmDitherType;
		DWORD  dmReserved1;
		DWORD  dmReserved2;
		DWORD  dmPanningWidth;
		DWORD  dmPanningHeight;
	} DEVMODEA, *PDEVMODEA, *NPDEVMODEA, *LPDEVMODEA;

	typedef struct _devicemodeW {
		WCHAR  dmDeviceName[CCHDEVICENAME];
		WORD dmSpecVersion;
		WORD dmDriverVersion;
		WORD dmSize;
		WORD dmDriverExtra;
		DWORD dmFields;
		union {
		  /* printer only fields */
		  struct {
			short dmOrientation;
			short dmPaperSize;
			short dmPaperLength;
			short dmPaperWidth;
			short dmScale;
			short dmCopies;
			short dmDefaultSource;
			short dmPrintQuality;
		  };
		  /* display only fields */
		  struct {
			POINTL dmPosition;
			DWORD  dmDisplayOrientation;
			DWORD  dmDisplayFixedOutput;
		  };
		};
		short dmColor;
		short dmDuplex;
		short dmYResolution;
		short dmTTOption;
		short dmCollate;
		WCHAR  dmFormName[CCHFORMNAME];
		WORD   dmLogPixels;
		DWORD  dmBitsPerPel;
		DWORD  dmPelsWidth;
		DWORD  dmPelsHeight;
		union {
			DWORD  dmDisplayFlags;
			DWORD  dmNup;
		};
		DWORD  dmDisplayFrequency;
		DWORD  dmICMMethod;
		DWORD  dmICMIntent;
		DWORD  dmMediaType;
		DWORD  dmDitherType;
		DWORD  dmReserved1;
		DWORD  dmReserved2;
		DWORD  dmPanningWidth;
		DWORD  dmPanningHeight;
	} DEVMODEW, *PDEVMODEW, *NPDEVMODEW, *LPDEVMODEW;

	typedef struct _OSVERSIONINFOEXA {
		DWORD dwOSVersionInfoSize;
		DWORD dwMajorVersion;
		DWORD dwMinorVersion;
		DWORD dwBuildNumber;
		DWORD dwPlatformId;
		CHAR   szCSDVersion[ 128 ];     // Maintenance string for PSS usage
		WORD   wServicePackMajor;
		WORD   wServicePackMinor;
		WORD   wSuiteMask;
		BYTE  wProductType;
		BYTE  wReserved;
	} OSVERSIONINFOEXA, *POSVERSIONINFOEXA, *LPOSVERSIONINFOEXA;

	typedef struct _OSVERSIONINFOEXW {
		DWORD dwOSVersionInfoSize;
		DWORD dwMajorVersion;
		DWORD dwMinorVersion;
		DWORD dwBuildNumber;
		DWORD dwPlatformId;
		WCHAR  szCSDVersion[ 128 ];     // Maintenance string for PSS usage
		WORD   wServicePackMajor;
		WORD   wServicePackMinor;
		WORD   wSuiteMask;
		BYTE  wProductType;
		BYTE  wReserved;
	} OSVERSIONINFOEXW, *POSVERSIONINFOEXW, *LPOSVERSIONINFOEXW, RTL_OSVERSIONINFOEXW, *PRTL_OSVERSIONINFOEXW;
	
	typedef struct _OSVERSIONINFOA {
		DWORD dwOSVersionInfoSize;
		DWORD dwMajorVersion;
		DWORD dwMinorVersion;
		DWORD dwBuildNumber;
		DWORD dwPlatformId;
		CHAR   szCSDVersion[ 128 ];     // Maintenance string for PSS usage
	} OSVERSIONINFOA, *POSVERSIONINFOA, *LPOSVERSIONINFOA;

	typedef struct _OSVERSIONINFOW {
		DWORD dwOSVersionInfoSize;
		DWORD dwMajorVersion;
		DWORD dwMinorVersion;
		DWORD dwBuildNumber;
		DWORD dwPlatformId;
		WCHAR  szCSDVersion[ 128 ];     // Maintenance string for PSS usage
	} OSVERSIONINFOW, *POSVERSIONINFOW, *LPOSVERSIONINFOW, RTL_OSVERSIONINFOW, *PRTL_OSVERSIONINFOW;
	
	typedef struct _SYSTEM_INFO {
		union {
			DWORD dwOemId;          // Obsolete field...do not use
			struct {
				WORD wProcessorArchitecture;
				WORD wReserved;
			} DUMMYSTRUCTNAME;
		} DUMMYUNIONNAME;
		DWORD dwPageSize;
		LPVOID lpMinimumApplicationAddress;
		LPVOID lpMaximumApplicationAddress;
		DWORD_PTR dwActiveProcessorMask;
		DWORD dwNumberOfProcessors;
		DWORD dwProcessorType;
		DWORD dwAllocationGranularity;
		WORD wProcessorLevel;
		WORD wProcessorRevision;
	} SYSTEM_INFO, *LPSYSTEM_INFO;
	
	typedef struct _DISPLAY_DEVICEA {
		DWORD  cb;
		CHAR   DeviceName[32];
		CHAR   DeviceString[128];
		DWORD  StateFlags;
		CHAR   DeviceID[128];
		CHAR   DeviceKey[128];
	} DISPLAY_DEVICEA, *PDISPLAY_DEVICEA, *LPDISPLAY_DEVICEA;

	typedef struct _DISPLAY_DEVICEW {
		DWORD  cb;
		WCHAR  DeviceName[32];
		WCHAR  DeviceString[128];
		DWORD  StateFlags;
		WCHAR  DeviceID[128];
		WCHAR  DeviceKey[128];
	} DISPLAY_DEVICEW, *PDISPLAY_DEVICEW, *LPDISPLAY_DEVICEW;
	
	typedef struct tagPIXELFORMATDESCRIPTOR
	{
		WORD  nSize;
		WORD  nVersion;
		DWORD dwFlags;
		BYTE  iPixelType;
		BYTE  cColorBits;
		BYTE  cRedBits;
		BYTE  cRedShift;
		BYTE  cGreenBits;
		BYTE  cGreenShift;
		BYTE  cBlueBits;
		BYTE  cBlueShift;
		BYTE  cAlphaBits;
		BYTE  cAlphaShift;
		BYTE  cAccumBits;
		BYTE  cAccumRedBits;
		BYTE  cAccumGreenBits;
		BYTE  cAccumBlueBits;
		BYTE  cAccumAlphaBits;
		BYTE  cDepthBits;
		BYTE  cStencilBits;
		BYTE  cAuxBuffers;
		BYTE  iLayerType;
		BYTE  bReserved;
		DWORD dwLayerMask;
		DWORD dwVisibleMask;
		DWORD dwDamageMask;
	} PIXELFORMATDESCRIPTOR, *PPIXELFORMATDESCRIPTOR, *LPPIXELFORMATDESCRIPTOR;

	typedef struct tagRAWINPUTDEVICE {
		USHORT usUsagePage; // Toplevel collection UsagePage
		USHORT usUsage;     // Toplevel collection Usage
		DWORD dwFlags;
		HWND hwndTarget;    // Target hwnd. null = follows keyboard focus
	} RAWINPUTDEVICE, *PRAWINPUTDEVICE, *LPRAWINPUTDEVICE;
	
	typedef CONST RAWINPUTDEVICE* PCRAWINPUTDEVICE;
	
	typedef struct tagRAWINPUTHEADER {
		DWORD dwType;
		DWORD dwSize;
		HANDLE hDevice;
		WPARAM wParam;
	} RAWINPUTHEADER, *PRAWINPUTHEADER, *LPRAWINPUTHEADER;
	
	typedef struct tagRAWMOUSE {
		/*
		 * Indicator flags.
		 */
		USHORT usFlags;

		/*
		 * The transition state of the mouse buttons.
		 */
		union {
			ULONG ulButtons;
			struct  {
				USHORT  usButtonFlags;
				USHORT  usButtonData;
			};
		};


		/*
		 * The raw state of the mouse buttons.
		 */
		ULONG ulRawButtons;

		/*
		 * The signed relative or absolute motion in the X direction.
		 */
		LONG lLastX;

		/*
		 * The signed relative or absolute motion in the Y direction.
		 */
		LONG lLastY;

		/*
		 * Device-specific additional information for the event.
		 */
		ULONG ulExtraInformation;

	} RAWMOUSE, *PRAWMOUSE, *LPRAWMOUSE;
	
	typedef struct tagRAWKEYBOARD {
		/*
		 * The "make" scan code (key depression).
		 */
		USHORT MakeCode;

		/*
		 * The flags field indicates a "break" (key release) and other
		 * miscellaneous scan code information defined in ntddkbd.h.
		 */
		USHORT Flags;

		USHORT Reserved;

		/*
		 * Windows message compatible information
		 */
		USHORT VKey;
		UINT   Message;

		/*
		 * Device-specific additional information for the event.
		 */
		ULONG ExtraInformation;

	} RAWKEYBOARD, *PRAWKEYBOARD, *LPRAWKEYBOARD;
	
	typedef struct tagRAWHID {
		DWORD dwSizeHid;    // byte size of each report
		DWORD dwCount;      // number of input packed
		BYTE bRawData[1];
	} RAWHID, *PRAWHID, *LPRAWHID;

	typedef struct tagRAWINPUT {
		RAWINPUTHEADER header;
		union {
			RAWMOUSE    mouse;
			RAWKEYBOARD keyboard;
			RAWHID      hid;
		} data;
	} RAWINPUT, *PRAWINPUT, *LPRAWINPUT;
	

	typedef struct tagMOUSEINPUT {
		LONG    dx;
		LONG    dy;
		DWORD   mouseData;
		DWORD   dwFlags;
		DWORD   time;
		ULONG_PTR dwExtraInfo;
	} MOUSEINPUT, *PMOUSEINPUT, * LPMOUSEINPUT;
		
	typedef struct tagKEYBDINPUT {
		WORD    wVk;
		WORD    wScan;
		DWORD   dwFlags;
		DWORD   time;
		ULONG_PTR dwExtraInfo;
	} KEYBDINPUT, *PKEYBDINPUT, * LPKEYBDINPUT;

	typedef struct tagHARDWAREINPUT {
		DWORD   uMsg;
		WORD    wParamL;
		WORD    wParamH;
	} HARDWAREINPUT, *PHARDWAREINPUT, * LPHARDWAREINPUT;

	typedef struct tagINPUT {
		DWORD   type;

		union
		{
			MOUSEINPUT      mi;
			KEYBDINPUT      ki;
			HARDWAREINPUT   hi;
		};
	} INPUT, *PINPUT, * LPINPUT;
	
	typedef struct _OVERLAPPED {
		ULONG_PTR Internal;
		ULONG_PTR InternalHigh;
		union {
			struct {
				DWORD Offset;
				DWORD OffsetHigh;
			} DUMMYSTRUCTNAME;
			PVOID Pointer;
		} DUMMYUNIONNAME;

		HANDLE  hEvent;
	} OVERLAPPED, *LPOVERLAPPED;
	
	typedef struct _RECTL       /* rcl */
	{
		LONG    left;
		LONG    top;
		LONG    right;
		LONG    bottom;
	} RECTL, *PRECTL, *LPRECTL;
	
	
	typedef ULONG_PTR HCRYPTPROV;
	typedef ULONG_PTR HCRYPTKEY;
	typedef ULONG_PTR HCRYPTHASH;



	//
	// Win Enums
	//

	enum EWinEnum
	{
		// BOOL
		FALSE						= 0,
		TRUE						= 1,

		//
		MAX_PATH					= 260,

		// Class styles
		CS_VREDRAW					= 0x0001,
		CS_HREDRAW					= 0x0002,
		CS_DBLCLKS					= 0x0008,
		CS_OWNDC					= 0x0020,
		CS_CLASSDC					= 0x0040,
		CS_PARENTDC					= 0x0080,
		CS_NOCLOSE					= 0x0200,
		CS_SAVEBITS					= 0x0800,
		CS_BYTEALIGNCLIENT			= 0x1000,
		CS_BYTEALIGNWINDOW			= 0x2000,
		CS_GLOBALCLASS				= 0x4000,

		// MessageBox() flags
		MB_OK						= 0x00000000L,
		MB_OKCANCEL					= 0x00000001L,
		MB_ABORTRETRYIGNORE			= 0x00000002L,
		MB_YESNOCANCEL				= 0x00000003L,
		MB_YESNO					= 0x00000004L,
		MB_RETRYCANCEL				= 0x00000005L,
		MB_CANCELTRYCONTINUE		= 0x00000006L,
		MB_ICONHAND					= 0x00000010L,
		MB_ICONQUESTION				= 0x00000020L,
		MB_ICONEXCLAMATION			= 0x00000030L,
		MB_ICONASTERISK				= 0x00000040L,
		MB_USERICON					= 0x00000080L,
		MB_ICONWARNING				= MB_ICONEXCLAMATION,
		MB_ICONERROR				= MB_ICONHAND,
		MB_ICONINFORMATION			= MB_ICONASTERISK,
		MB_ICONSTOP					= MB_ICONHAND,
		MB_DEFBUTTON1				= 0x00000000L,
		MB_DEFBUTTON2				= 0x00000100L,
		MB_DEFBUTTON3				= 0x00000200L,
		MB_DEFBUTTON4				= 0x00000300L,
		MB_APPLMODAL				= 0x00000000L,
		MB_SYSTEMMODAL				= 0x00001000L,
		MB_TASKMODAL				= 0x00002000L,
		MB_HELP						= 0x00004000L, // Help Button
		MB_NOFOCUS					= 0x00008000L,
		MB_SETFOREGROUND			= 0x00010000L,
		MB_DEFAULT_DESKTOP_ONLY		= 0x00020000L,
		MB_TOPMOST					= 0x00040000L,
		MB_RIGHT					= 0x00080000L,
		MB_RTLREADING				= 0x00100000L,
		MB_TYPEMASK					= 0x0000000FL,
		MB_ICONMASK					= 0x000000F0L,
		MB_DEFMASK					= 0x00000F00L,
		MB_MODEMASK					= 0x00003000L,
		MB_MISCMASK					= 0x0000C000L,

		// 
		INPUT_MOUSE					= 0,
		INPUT_KEYBOARD				= 1,
		INPUT_HARDWARE				= 2,
	
		// Dialog Box Command IDs
		IDOK						= 1,
		IDCANCEL					= 2,
		IDABORT						= 3,
		IDRETRY						= 4,
		IDIGNORE					= 5,
		IDYES						= 6,
		IDNO						= 7,
		IDCLOSE						= 8,
		IDHELP						= 9,
		IDTRYAGAIN					= 10,
		IDCONTINUE					= 11,
		IDTIMEOUT					= 32000,

		//
		TLS_OUT_OF_INDEXES			= 0xFFFFFFFFL,
		
		// Local Memory Flags
		LMEM_FIXED					= 0x0000,
		LMEM_MOVEABLE				= 0x0002,
		LMEM_NOCOMPACT				= 0x0010,
		LMEM_NODISCARD				= 0x0020,
		LMEM_ZEROINIT				= 0x0040,
		LMEM_MODIFY					= 0x0080,
		LMEM_DISCARDABLE			= 0x0F00,
		LMEM_VALID_FLAGS			= 0x0F72,
		LMEM_INVALID_HANDLE			= 0x8000,
		LHND						= LMEM_MOVEABLE | LMEM_ZEROINIT,
		LPTR						= LMEM_FIXED | LMEM_ZEROINIT,

		// Process dwCreationFlag values
		DEBUG_PROCESS					= 0x00000001,
		DEBUG_ONLY_THIS_PROCESS			= 0x00000002,
		CREATE_SUSPENDED				= 0x00000004,
		DETACHED_PROCESS				= 0x00000008,
		CREATE_NEW_CONSOLE				= 0x00000010,
		NORMAL_PRIORITY_CLASS			= 0x00000020,
		IDLE_PRIORITY_CLASS				= 0x00000040,
		HIGH_PRIORITY_CLASS				= 0x00000080,
		REALTIME_PRIORITY_CLASS			= 0x00000100,
		CREATE_NEW_PROCESS_GROUP		= 0x00000200,
		CREATE_UNICODE_ENVIRONMENT		= 0x00000400,
		CREATE_SEPARATE_WOW_VDM			= 0x00000800,
		CREATE_SHARED_WOW_VDM			= 0x00001000,
		CREATE_FORCEDOS					= 0x00002000,
		BELOW_NORMAL_PRIORITY_CLASS		= 0x00004000,
		ABOVE_NORMAL_PRIORITY_CLASS		= 0x00008000,
		INHERIT_PARENT_AFFINITY			= 0x00010000,
		CREATE_PROTECTED_PROCESS		= 0x00040000,
		EXTENDED_STARTUPINFO_PRESENT	= 0x00080000,
		PROCESS_MODE_BACKGROUND_BEGIN	= 0x00100000,
		PROCESS_MODE_BACKGROUND_END		= 0x00200000,
		CREATE_BREAKAWAY_FROM_JOB		= 0x01000000,
		CREATE_PRESERVE_CODE_AUTHZ_LEVEL= 0x02000000,
		CREATE_DEFAULT_ERROR_MODE		= 0x04000000,
		CREATE_NO_WINDOW				= 0x08000000,
		PROFILE_USER					= 0x10000000,
		PROFILE_KERNEL					= 0x20000000,
		PROFILE_SERVER					= 0x40000000,
		CREATE_IGNORE_SYSTEM_DEFAULT	= 0x80000000,

		// 
		WAIT_OBJECT_0				= 0L,
		WAIT_FAILED					= 0xFFFFFFFFL,
		WAIT_ABANDONED				= 0x00000080L,

		//
		DUPLICATE_CLOSE_SOURCE		= 0x00000001,
		DUPLICATE_SAME_ACCESS		= 0x00000002,

		//
		CONDITION_VARIABLE_LOCKMODE_SHARED	= 0x1,


		// Access type
		DELETE						= 0x00010000L,
		READ_CONTROL				= 0x00020000L,
		WRITE_DAC					= 0x00040000L,
		WRITE_OWNER					= 0x00080000L,
		SYNCHRONIZE					= 0x00100000L,
		STANDARD_RIGHTS_REQUIRED	= 0x000F0000L,
		STANDARD_RIGHTS_READ		= READ_CONTROL,
		STANDARD_RIGHTS_WRITE		= READ_CONTROL,
		STANDARD_RIGHTS_EXECUTE		= READ_CONTROL,
		STANDARD_RIGHTS_ALL			= 0x001F0000L,
		SPECIFIC_RIGHTS_ALL			= 0x0000FFFFL,

		// File
		FILE_READ_DATA				= 0x0001,		// file & pipe
		FILE_LIST_DIRECTORY			= 0x0001,		// directory
		FILE_WRITE_DATA				= 0x0002,		// file & pipe
		FILE_ADD_FILE				= 0x0002,		// directory
		FILE_APPEND_DATA			= 0x0004,		// file
		FILE_ADD_SUBDIRECTORY		= 0x0004,		// directory
		FILE_CREATE_PIPE_INSTANCE	= 0x0004,		// named pipe
		FILE_READ_EA				= 0x0008,		// file & directory
		FILE_WRITE_EA				= 0x0010,		// file & directory
		FILE_EXECUTE				= 0x0020,		// file
		FILE_TRAVERSE				= 0x0020,		// directory
		FILE_DELETE_CHILD			= 0x0040,		// directory
		FILE_READ_ATTRIBUTES		= 0x0080,		// all
		FILE_WRITE_ATTRIBUTES		= 0x0100,		// all

		FILE_ALL_ACCESS				= STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x1FF,
		FILE_GENERIC_READ			= STANDARD_RIGHTS_READ | FILE_READ_DATA | FILE_READ_ATTRIBUTES | FILE_READ_EA | SYNCHRONIZE,
		FILE_GENERIC_WRITE			= STANDARD_RIGHTS_WRITE | FILE_WRITE_DATA | FILE_WRITE_ATTRIBUTES | FILE_WRITE_EA | FILE_APPEND_DATA | SYNCHRONIZE,
		FILE_GENERIC_EXECUTE		= STANDARD_RIGHTS_EXECUTE | FILE_READ_ATTRIBUTES | FILE_EXECUTE | SYNCHRONIZE,

		FILE_SHARE_READ						= 0x00000001,
		FILE_SHARE_WRITE					= 0x00000002,
		FILE_SHARE_DELETE					= 0x00000004,
		FILE_ATTRIBUTE_READONLY				= 0x00000001,
		FILE_ATTRIBUTE_HIDDEN				= 0x00000002,
		FILE_ATTRIBUTE_SYSTEM				= 0x00000004,
		FILE_ATTRIBUTE_DIRECTORY			= 0x00000010,
		FILE_ATTRIBUTE_ARCHIVE				= 0x00000020,
		FILE_ATTRIBUTE_DEVICE				= 0x00000040,
		FILE_ATTRIBUTE_NORMAL				= 0x00000080,
		FILE_ATTRIBUTE_TEMPORARY			= 0x00000100,
		FILE_ATTRIBUTE_SPARSE_FILE			= 0x00000200,
		FILE_ATTRIBUTE_REPARSE_POINT		= 0x00000400,
		FILE_ATTRIBUTE_COMPRESSED			= 0x00000800,
		FILE_ATTRIBUTE_OFFLINE				= 0x00001000,
		FILE_ATTRIBUTE_NOT_CONTENT_INDEXED	= 0x00002000,
		FILE_ATTRIBUTE_ENCRYPTED			= 0x00004000,
		FILE_ATTRIBUTE_VIRTUAL				= 0x00010000,
		FILE_NOTIFY_CHANGE_FILE_NAME		= 0x00000001,
		FILE_NOTIFY_CHANGE_DIR_NAME			= 0x00000002,
		FILE_NOTIFY_CHANGE_ATTRIBUTES		= 0x00000004,
		FILE_NOTIFY_CHANGE_SIZE				= 0x00000008,
		FILE_NOTIFY_CHANGE_LAST_WRITE		= 0x00000010,
		FILE_NOTIFY_CHANGE_LAST_ACCESS		= 0x00000020,
		FILE_NOTIFY_CHANGE_CREATION			= 0x00000040,
		FILE_NOTIFY_CHANGE_SECURITY			= 0x00000100,
		FILE_ACTION_ADDED					= 0x00000001,
		FILE_ACTION_REMOVED					= 0x00000002,
		FILE_ACTION_MODIFIED				= 0x00000003,
		FILE_ACTION_RENAMED_OLD_NAME		= 0x00000004,
		FILE_ACTION_RENAMED_NEW_NAME		= 0x00000005,
		MAILSLOT_NO_MESSAGE					= (DWORD)-1, 
		MAILSLOT_WAIT_FOREVER				= (DWORD)-1, 
		FILE_CASE_SENSITIVE_SEARCH			= 0x00000001,
		FILE_CASE_PRESERVED_NAMES			= 0x00000002,
		FILE_UNICODE_ON_DISK				= 0x00000004,
		FILE_PERSISTENT_ACLS				= 0x00000008,
		FILE_FILE_COMPRESSION				= 0x00000010,
		FILE_VOLUME_QUOTAS					= 0x00000020,
		FILE_SUPPORTS_SPARSE_FILES			= 0x00000040,
		FILE_SUPPORTS_REPARSE_POINTS		= 0x00000080,
		FILE_SUPPORTS_REMOTE_STORAGE		= 0x00000100,
		FILE_VOLUME_IS_COMPRESSED			= 0x00008000,
		FILE_SUPPORTS_OBJECT_IDS			= 0x00010000,
		FILE_SUPPORTS_ENCRYPTION			= 0x00020000,
		FILE_NAMED_STREAMS					= 0x00040000,
		FILE_READ_ONLY_VOLUME				= 0x00080000,
		FILE_SEQUENTIAL_WRITE_ONCE			= 0x00100000,
		FILE_SUPPORTS_TRANSACTIONS			= 0x00200000,
		FILE_SUPPORTS_HARD_LINKS			= 0x00400000,
		FILE_SUPPORTS_EXTENDED_ATTRIBUTES	= 0x00800000,
		FILE_SUPPORTS_OPEN_BY_FILE_ID		= 0x01000000,
		FILE_SUPPORTS_USN_JOURNAL			= 0x02000000,

		//
		INVALID_FILE_SIZE					= (DWORD)0xFFFFFFFF,
		INVALID_SET_FILE_POINTER			= (DWORD)-1,
		INVALID_FILE_ATTRIBUTES				= (DWORD)-1,
		

		// Color Types
		CTLCOLOR_MSGBOX						= 0,
		CTLCOLOR_EDIT						= 1,
		CTLCOLOR_LISTBOX					= 2,
		CTLCOLOR_BTN						= 3,
		CTLCOLOR_DLG						= 4,
		CTLCOLOR_SCROLLBAR					= 5,
		CTLCOLOR_STATIC						= 6,
		CTLCOLOR_MAX						= 7,
		COLOR_SCROLLBAR						= 0,
		COLOR_BACKGROUND					= 1,
		COLOR_ACTIVECAPTION					= 2,
		COLOR_INACTIVECAPTION				= 3,
		COLOR_MENU							= 4,
		COLOR_WINDOW						= 5,
		COLOR_WINDOWFRAME					= 6,
		COLOR_MENUTEXT						= 7,
		COLOR_WINDOWTEXT					= 8,
		COLOR_CAPTIONTEXT					= 9,
		COLOR_ACTIVEBORDER					= 10,
		COLOR_INACTIVEBORDER				= 11,
		COLOR_APPWORKSPACE					= 12,
		COLOR_HIGHLIGHT						= 13,
		COLOR_HIGHLIGHTTEXT					= 14,
		COLOR_BTNFACE						= 15,
		COLOR_BTNSHADOW						= 16,
		COLOR_GRAYTEXT						= 17,
		COLOR_BTNTEXT						= 18,
		COLOR_INACTIVECAPTIONTEXT			= 19,
		COLOR_BTNHIGHLIGHT					= 20,
	
		
		// Window Styles
		WS_OVERLAPPED						= 0x00000000L,
		WS_POPUP							= 0x80000000L,
		WS_CHILD							= 0x40000000L,
		WS_MINIMIZE							= 0x20000000L,
		WS_VISIBLE							= 0x10000000L,
		WS_DISABLED							= 0x08000000L,
		WS_CLIPSIBLINGS						= 0x04000000L,
		WS_CLIPCHILDREN						= 0x02000000L,
		WS_MAXIMIZE							= 0x01000000L,
		WS_CAPTION							= 0x00C00000L,     /* WS_BORDER | WS_DLGFRAME  */
		WS_BORDER							= 0x00800000L,
		WS_DLGFRAME							= 0x00400000L,
		WS_VSCROLL							= 0x00200000L,
		WS_HSCROLL							= 0x00100000L,
		WS_SYSMENU							= 0x00080000L,
		WS_THICKFRAME						= 0x00040000L,
		WS_GROUP							= 0x00020000L,
		WS_TABSTOP							= 0x00010000L,
		WS_MINIMIZEBOX						= 0x00020000L,
		WS_MAXIMIZEBOX						= 0x00010000L,
		WS_TILED							= WS_OVERLAPPED,
		WS_ICONIC							= WS_MINIMIZE,
		WS_SIZEBOX							= WS_THICKFRAME,
		WS_OVERLAPPEDWINDOW					= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		WS_POPUPWINDOW						= WS_POPUP | WS_BORDER | WS_SYSMENU,
		WS_CHILDWINDOW						= WS_CHILD,
		WS_TILEDWINDOW						= WS_OVERLAPPEDWINDOW,
		
		SS_ICON								= 0x00000003L,
		SS_BITMAP							= 0x0000000EL,
		
		STM_SETICON							= 0x0170,
		STM_GETICON							= 0x0171,
		STM_SETIMAGE						= 0x0172,
		STM_GETIMAGE						= 0x0173,

		IMAGE_BITMAP						= 0,
		IMAGE_ICON							= 1,
		IMAGE_CURSOR						= 2,

		// Extended Window Styles
		WS_EX_DLGMODALFRAME					= 0x00000001L,
		WS_EX_NOPARENTNOTIFY				= 0x00000004L,
		WS_EX_TOPMOST						= 0x00000008L,
		WS_EX_ACCEPTFILES					= 0x00000010L,
		WS_EX_TRANSPARENT					= 0x00000020L,
		WS_EX_MDICHILD						= 0x00000040L,
		WS_EX_TOOLWINDOW					= 0x00000080L,
		WS_EX_WINDOWEDGE					= 0x00000100L,
		WS_EX_CLIENTEDGE					= 0x00000200L,
		WS_EX_CONTEXTHELP					= 0x00000400L,
		WS_EX_RIGHT							= 0x00001000L,
		WS_EX_LEFT							= 0x00000000L,
		WS_EX_RTLREADING					= 0x00002000L,
		WS_EX_LTRREADING					= 0x00000000L,
		WS_EX_LEFTSCROLLBAR					= 0x00004000L,
		WS_EX_RIGHTSCROLLBAR				= 0x00000000L,
		WS_EX_CONTROLPARENT					= 0x00010000L,
		WS_EX_STATICEDGE					= 0x00020000L,
		WS_EX_APPWINDOW						= 0x00040000L,
		WS_EX_OVERLAPPEDWINDOW				= WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE,
		WS_EX_PALETTEWINDOW					= WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
		WS_EX_LAYERED						= 0x00080000,
		WS_EX_NOINHERITLAYOUT				= 0x00100000L, // Disable inheritence of mirroring by children
		WS_EX_LAYOUTRTL						= 0x00400000L, // Right to left mirroring
		WS_EX_COMPOSITED					= 0x02000000L,
		WS_EX_NOACTIVATE					= 0x08000000L,

	
		// Window field offsets for GetWindowLong()
		GWL_WNDPROC							= -4,
		GWL_HINSTANCE						= -6,
		GWL_HWNDPARENT						= -8,
		GWL_STYLE							= -16,
		GWL_EXSTYLE							= -20,
		GWL_USERDATA						= -21,
		GWL_ID              				= -12,
		
		// Window field offsets for GetWindowLongPtr()
		GWLP_WNDPROC						= -4,
		GWLP_HINSTANCE						= -6,
		GWLP_HWNDPARENT						= -8,
		GWLP_USERDATA						= -21,
		GWLP_ID								= -12,
		
		
		// ShowWindow() Commands
		SW_HIDE								= 0,
		SW_SHOWNORMAL						= 1,
		SW_NORMAL							= 1,
		SW_SHOWMINIMIZED					= 2,
		SW_SHOWMAXIMIZED					= 3,
		SW_MAXIMIZE							= 3,
		SW_SHOWNOACTIVATE					= 4,
		SW_SHOW								= 5,
		SW_MINIMIZE							= 6,
		SW_SHOWMINNOACTIVE					= 7,
		SW_SHOWNA							= 8,
		SW_RESTORE							= 9,
		SW_SHOWDEFAULT						= 10,
		SW_FORCEMINIMIZE					= 11,
		SW_MAX								= 11,

		// Device Mode
		DM_ORIENTATION						= 0x00000001L,
		DM_PAPERSIZE						= 0x00000002L,
		DM_PAPERLENGTH						= 0x00000004L,
		DM_PAPERWIDTH						= 0x00000008L,
		DM_SCALE							= 0x00000010L,
		DM_POSITION							= 0x00000020L,
		DM_NUP								= 0x00000040L,
		DM_DISPLAYORIENTATION				= 0x00000080L,
		DM_COPIES							= 0x00000100L,
		DM_DEFAULTSOURCE					= 0x00000200L,
		DM_PRINTQUALITY						= 0x00000400L,
		DM_COLOR							= 0x00000800L,
		DM_DUPLEX							= 0x00001000L,
		DM_YRESOLUTION						= 0x00002000L,
		DM_TTOPTION							= 0x00004000L,
		DM_COLLATE							= 0x00008000L,
		DM_FORMNAME							= 0x00010000L,
		DM_LOGPIXELS						= 0x00020000L,
		DM_BITSPERPEL						= 0x00040000L,
		DM_PELSWIDTH						= 0x00080000L,
		DM_PELSHEIGHT						= 0x00100000L,
		DM_DISPLAYFLAGS						= 0x00200000L,
		DM_DISPLAYFREQUENCY					= 0x00400000L,
		DM_ICMMETHOD						= 0x00800000L,
		DM_ICMINTENT						= 0x01000000L,
		DM_MEDIATYPE						= 0x02000000L,
		DM_DITHERTYPE						= 0x04000000L,
		DM_PANNINGWIDTH						= 0x08000000L,
		DM_PANNINGHEIGHT					= 0x10000000L,
		DM_DISPLAYFIXEDOUTPUT				= 0x20000000L,
		

		// Flags for ChangeDisplaySettings
		CDS_UPDATEREGISTRY					= 0x00000001,
		CDS_TEST							= 0x00000002,
		CDS_FULLSCREEN						= 0x00000004,
		CDS_GLOBAL							= 0x00000008,
		CDS_SET_PRIMARY						= 0x00000010,
		CDS_VIDEOPARAMETERS					= 0x00000020,
		CDS_ENABLE_UNSAFE_MODES				= 0x00000100,
		CDS_DISABLE_UNSAFE_MODES			= 0x00000200,
		CDS_RESET							= 0x40000000,
		CDS_RESET_EX						= 0x20000000,
		CDS_NORESET							= 0x10000000,
		
		// Return values for ChangeDisplaySettings
		DISP_CHANGE_SUCCESSFUL				= 0,
		DISP_CHANGE_RESTART					= 1,
		DISP_CHANGE_FAILED					= -1,
		DISP_CHANGE_BADMODE					= -2,
		DISP_CHANGE_NOTUPDATED				= -3,
		DISP_CHANGE_BADFLAGS				= -4,
		DISP_CHANGE_BADPARAM				= -5,
		DISP_CHANGE_BADDUALVIEW				= -6,
	

		// Queue status flags for GetQueueStatus() and MsgWaitForMultipleObjects()
		QS_KEY								= 0x0001,
		QS_MOUSEMOVE						= 0x0002,
		QS_MOUSEBUTTON						= 0x0004,
		QS_POSTMESSAGE						= 0x0008,
		QS_TIMER							= 0x0010,
		QS_PAINT							= 0x0020,
		QS_SENDMESSAGE						= 0x0040,
		QS_HOTKEY							= 0x0080,
		QS_ALLPOSTMESSAGE					= 0x0100,
		QS_RAWINPUT							= 0x0400,
		QS_MOUSE							= QS_MOUSEMOVE | QS_MOUSEBUTTON,
		QS_INPUT							= QS_MOUSE | QS_KEY | QS_RAWINPUT,
		QS_ALLEVENTS						= QS_INPUT | QS_POSTMESSAGE | QS_TIMER | QS_PAINT | QS_HOTKEY,
		QS_ALLINPUT							= QS_INPUT | QS_POSTMESSAGE | QS_TIMER | QS_PAINT | QS_HOTKEY | QS_SENDMESSAGE,


		// PeekMessage() Options
		PM_NOREMOVE							= 0x0000,
		PM_REMOVE							= 0x0001,
		PM_NOYIELD							= 0x0002,
		PM_QS_INPUT							= QS_INPUT << 16,
		PM_QS_POSTMESSAGE					= (QS_POSTMESSAGE | QS_HOTKEY | QS_TIMER) << 16,
		PM_QS_PAINT							= QS_PAINT << 16,
		PM_QS_SENDMESSAGE					= QS_SENDMESSAGE << 16,


		// Window Messages
		WM_NULL								= 0x0000,
		WM_CREATE							= 0x0001,
		WM_DESTROY							= 0x0002,
		WM_MOVE								= 0x0003,
		WM_SIZE								= 0x0005,
		WM_ACTIVATE							= 0x0006,
		// WM_ACTIVATE state values :
			WA_INACTIVE						= 0,
			WA_ACTIVE						= 1,
			WA_CLICKACTIVE					= 2,
		WM_SETFOCUS							= 0x0007,
		WM_KILLFOCUS						= 0x0008,
		WM_ENABLE							= 0x000A,
		WM_SETREDRAW						= 0x000B,
		WM_SETTEXT							= 0x000C,
		WM_GETTEXT							= 0x000D,
		WM_GETTEXTLENGTH					= 0x000E,
		WM_PAINT							= 0x000F,
		WM_CLOSE							= 0x0010,
		WM_QUIT								= 0x0012,
		WM_ERASEBKGND						= 0x0014,
		WM_SYSCOLORCHANGE					= 0x0015,
		WM_SHOWWINDOW						= 0x0018,
		WM_WININICHANGE						= 0x001A,
		WM_SETTINGCHANGE					= WM_WININICHANGE,
		WM_DEVMODECHANGE					= 0x001B,
		WM_ACTIVATEAPP						= 0x001C,
		WM_FONTCHANGE						= 0x001D,
		WM_TIMECHANGE						= 0x001E,
		WM_CANCELMODE						= 0x001F,
		WM_SETCURSOR						= 0x0020,
		WM_MOUSEACTIVATE					= 0x0021,
		WM_CHILDACTIVATE					= 0x0022,
		WM_QUEUESYNC						= 0x0023,
		WM_GETMINMAXINFO					= 0x0024,
		WM_SYSCOMMAND						= 0x0112,
		WM_TIMER							= 0x0113,
		WM_INPUT							= 0x00FF,
		WM_MOUSEMOVE						= 0x0200,
		WM_USER								= 0x0400,
		WM_INPUTLANGCHANGE					= 0x0051,
		WM_QUERYENDSESSION					= 0x0011,
		WM_QUERYOPEN						= 0x0013,
		WM_ENDSESSION						= 0x0016,
		WM_MOUSEFIRST						= 0x0200,
		WM_LBUTTONDOWN						= 0x0201,
		WM_LBUTTONUP						= 0x0202,
		WM_LBUTTONDBLCLK					= 0x0203,
		WM_RBUTTONDOWN						= 0x0204,
		WM_RBUTTONUP						= 0x0205,
		WM_RBUTTONDBLCLK					= 0x0206,
		WM_MBUTTONDOWN						= 0x0207,
		WM_MBUTTONUP						= 0x0208,
		WM_MBUTTONDBLCLK					= 0x0209,
		WM_XBUTTONDOWN						= 0x020B,
		WM_XBUTTONUP						= 0x020C,
		WM_XBUTTONDBLCLK					= 0x020D,
		WM_MOUSEHWHEEL						= 0x020E,
		
		// Raw Input Device Flags
		RID_INPUT							= 0x10000003,
		RID_HEADER							= 0x10000005,
		RIM_TYPEMOUSE						= 0,
		RIM_TYPEKEYBOARD					= 1,
		RIM_TYPEHID							= 2,
		RI_KEY_MAKE							= 0,
		RI_KEY_BREAK						= 1,
		RI_KEY_E0							= 2,
		RI_KEY_E1							= 4,
		RI_KEY_TERMSRV_SET_LED				= 8,
		RI_KEY_TERMSRV_SHADOW				= 0x10,

		// raw input mouse flags
		RI_MOUSE_LEFT_BUTTON_DOWN			= 0x0001,  // Left Button changed to down.
		RI_MOUSE_LEFT_BUTTON_UP				= 0x0002,  // Left Button changed to up.
		RI_MOUSE_RIGHT_BUTTON_DOWN			= 0x0004,  // Right Button changed to down.
		RI_MOUSE_RIGHT_BUTTON_UP			= 0x0008,  // Right Button changed to up.
		RI_MOUSE_MIDDLE_BUTTON_DOWN			= 0x0010,  // Middle Button changed to down.
		RI_MOUSE_MIDDLE_BUTTON_UP			= 0x0020,  // Middle Button changed to up.
		RI_MOUSE_BUTTON_1_DOWN				= RI_MOUSE_LEFT_BUTTON_DOWN,
		RI_MOUSE_BUTTON_1_UP				= RI_MOUSE_LEFT_BUTTON_UP,
		RI_MOUSE_BUTTON_2_DOWN				= RI_MOUSE_RIGHT_BUTTON_DOWN,
		RI_MOUSE_BUTTON_2_UP				= RI_MOUSE_RIGHT_BUTTON_UP,
		RI_MOUSE_BUTTON_3_DOWN				= RI_MOUSE_MIDDLE_BUTTON_DOWN,
		RI_MOUSE_BUTTON_3_UP				= RI_MOUSE_MIDDLE_BUTTON_UP,
		RI_MOUSE_BUTTON_4_DOWN				= 0x0040,
		RI_MOUSE_BUTTON_4_UP				= 0x0080,
		RI_MOUSE_BUTTON_5_DOWN				= 0x0100,
		RI_MOUSE_BUTTON_5_UP				= 0x0200,
		RI_MOUSE_WHEEL						= 0x0400,
		WHEEL_DELTA							= 120,

		// WM_SIZE message wParam values
		SIZE_RESTORED						= 0,
		SIZE_MINIMIZED						= 1,
		SIZE_MAXIMIZED						= 2,
		SIZE_MAXSHOW						= 3,
		SIZE_MAXHIDE						= 4,
	
		// System Menu Command Values
		SC_SIZE								= 0xF000,
		SC_MOVE								= 0xF010,
		SC_MINIMIZE							= 0xF020,
		SC_MAXIMIZE							= 0xF030,
		SC_NEXTWINDOW						= 0xF040,
		SC_PREVWINDOW						= 0xF050,
		SC_CLOSE							= 0xF060,
		SC_VSCROLL							= 0xF070,
		SC_HSCROLL							= 0xF080,
		SC_MOUSEMENU						= 0xF090,
		SC_KEYMENU							= 0xF100,
		SC_ARRANGE							= 0xF110,
		SC_RESTORE							= 0xF120,
		SC_TASKLIST							= 0xF130,
		SC_SCREENSAVE						= 0xF140,
		SC_HOTKEY							= 0xF150,
		SC_DEFAULT							= 0xF160,
		SC_MONITORPOWER						= 0xF170,
		SC_CONTEXTHELP						= 0xF180,
		SC_SEPARATOR						= 0xF00F,
	
		// SetWindowPos Flags
		SWP_NOSIZE							= 0x0001,
		SWP_NOMOVE							= 0x0002,
		SWP_NOZORDER						= 0x0004,
		SWP_NOREDRAW						= 0x0008,
		SWP_NOACTIVATE						= 0x0010,
		SWP_FRAMECHANGED					= 0x0020,  /* The frame changed: send WM_NCCALCSIZE */
		SWP_SHOWWINDOW						= 0x0040,
		SWP_HIDEWINDOW						= 0x0080,
		SWP_NOCOPYBITS						= 0x0100,
		SWP_NOOWNERZORDER					= 0x0200,  /* Don't do owner Z ordering */
		SWP_NOSENDCHANGING					= 0x0400,  /* Don't send WM_WINDOWPOSCHANGING */
		
		ERROR_SUCCESS						= 0L,
	
		// Registry Specific Access Rights.
		KEY_QUERY_VALUE						= 0x0001,
		KEY_SET_VALUE						= 0x0002,
		KEY_CREATE_SUB_KEY					= 0x0004,
		KEY_ENUMERATE_SUB_KEYS				= 0x0008,
		KEY_NOTIFY							= 0x0010,
		KEY_CREATE_LINK						= 0x0020,
		KEY_WOW64_32KEY						= 0x0200,
		KEY_WOW64_64KEY						= 0x0100,
		KEY_WOW64_RES						= 0x0300,
		KEY_READ							= (STANDARD_RIGHTS_READ | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY) & (~SYNCHRONIZE),
		KEY_WRITE							= (STANDARD_RIGHTS_WRITE | KEY_SET_VALUE | KEY_CREATE_SUB_KEY) & (~SYNCHRONIZE),
		KEY_EXECUTE							= KEY_READ & (~SYNCHRONIZE),
		KEY_ALL_ACCESS						= STANDARD_RIGHTS_ALL | KEY_QUERY_VALUE | KEY_SET_VALUE | KEY_CREATE_SUB_KEY | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY | KEY_CREATE_LINK & (~SYNCHRONIZE),

		// PixelFormatDescriptor flags
		PFD_TYPE_RGBA						= 0,
		PFD_TYPE_COLORINDEX					= 1,
		PFD_MAIN_PLANE						= 0,
		PFD_DOUBLEBUFFER					= 0x00000001,
		PFD_STEREO							= 0x00000002,
		PFD_DRAW_TO_WINDOW					= 0x00000004,
		PFD_DRAW_TO_BITMAP					= 0x00000008,
		PFD_SUPPORT_GDI						= 0x00000010,
		PFD_SUPPORT_OPENGL					= 0x00000020,
		PFD_GENERIC_FORMAT					= 0x00000040,
		PFD_NEED_PALETTE					= 0x00000080,
		PFD_NEED_SYSTEM_PALETTE				= 0x00000100,
		PFD_SWAP_EXCHANGE					= 0x00000200,
		PFD_SWAP_COPY						= 0x00000400,
		PFD_SWAP_LAYER_BUFFERS				= 0x00000800,
		PFD_GENERIC_ACCELERATED				= 0x00001000,
		PFD_SUPPORT_DIRECTDRAW				= 0x00002000,
		PFD_DIRECT3D_ACCELERATED			= 0x00004000,
		PFD_SUPPORT_COMPOSITION				= 0x00008000,

		// Map Virtual Key Param
		MAPVK_VK_TO_VSC						= 0,
		MAPVK_VSC_TO_VK						= 1,
		MAPVK_VK_TO_CHAR					= 2,
		MAPVK_VSC_TO_VK_EX					= 3,

		// Class Field for SetClassLong
		GCLP_MENUNAME						= -8,
		GCLP_HBRBACKGROUND					= -10,
		GCLP_HCURSOR						= -12,
		GCLP_HICON							= -14,
		GCLP_HMODULE						= -16,
		GCLP_WNDPROC						= -24,
		GCLP_HICONSM						= -34,

		// 
		MOUSEEVENTF_MOVE					= 0x0001,
		MOUSEEVENTF_LEFTDOWN				= 0x0002,
		MOUSEEVENTF_LEFTUP					= 0x0004,
		MOUSEEVENTF_RIGHTDOWN				= 0x0008,
		MOUSEEVENTF_RIGHTUP					= 0x0010,
		MOUSEEVENTF_MIDDLEDOWN				= 0x0020,
		MOUSEEVENTF_MIDDLEUP				= 0x0040,
		MOUSEEVENTF_XDOWN					= 0x0080,
		MOUSEEVENTF_XUP						= 0x0100,
		MOUSEEVENTF_WHEEL					= 0x0800,
		MOUSEEVENTF_HWHEEL					= 0x01000,
		MOUSEEVENTF_MOVE_NOCOALESCE			= 0x2000,
		MOUSEEVENTF_VIRTUALDESK				= 0x4000,
		MOUSEEVENTF_ABSOLUTE				= 0x8000,
	

		// 
		KEYEVENTF_EXTENDEDKEY				= 0x0001,
		KEYEVENTF_KEYUP						= 0x0002,
		KEYEVENTF_UNICODE					= 0x0004,
		KEYEVENTF_SCANCODE					= 0x0008,


		// for VirtuaAlloc
		PAGE_NOACCESS						= 0x01,
		PAGE_READONLY						= 0x02,
		PAGE_READWRITE						= 0x04,
		PAGE_WRITECOPY						= 0x08,
		PAGE_EXECUTE						= 0x10,
		PAGE_EXECUTE_READ					= 0x20,
		PAGE_EXECUTE_READWRITE				= 0x40,
		PAGE_EXECUTE_WRITECOPY				= 0x80,
		PAGE_GUARD							= 0x100,
		PAGE_NOCACHE						= 0x200,
		PAGE_WRITECOMBINE					= 0x400,
		MEM_COMMIT							= 0x1000,
		MEM_RESERVE							= 0x2000,
		MEM_DECOMMIT						= 0x4000,
		MEM_RELEASE							= 0x8000,
		MEM_FREE							= 0x10000,
		MEM_PRIVATE							= 0x20000,
		MEM_MAPPED							= 0x40000,
		MEM_RESET							= 0x80000,
		MEM_TOP_DOWN						= 0x100000,
		MEM_WRITE_WATCH						= 0x200000,
		MEM_PHYSICAL						= 0x400000,
		MEM_ROTATE							= 0x800000,
		MEM_LARGE_PAGES						= 0x20000000,
		MEM_4MB_PAGES						= 0x80000000,
		SEC_FILE							= 0x800000,
		SEC_IMAGE							= 0x1000000,
		SEC_PROTECTED_IMAGE					= 0x2000000,
		SEC_RESERVE							= 0x4000000,
		SEC_COMMIT							= 0x8000000,
		SEC_NOCACHE							= 0x10000000,
		SEC_WRITECOMBINE					= 0x40000000,
		SEC_LARGE_PAGES						= 0x80000000,
		MEM_IMAGE							= SEC_IMAGE,
		WRITE_WATCH_FLAG_RESET				= 0x01,

		// Reason flags (in DllMain)
		DLL_PROCESS_ATTACH					= 1,
		DLL_THREAD_ATTACH					= 2,
		DLL_THREAD_DETACH					= 3,
		DLL_PROCESS_DETACH					= 0,
		
		MK_LBUTTON							= 0x0001,
		MK_RBUTTON							= 0x0002,
		MK_SHIFT							= 0x0004,
		MK_CONTROL							= 0x0008,
		MK_MBUTTON							= 0x0010,
		MK_XBUTTON1							= 0x0020,
		MK_XBUTTON2							= 0x0040,
		
		// Shell File Operations
		FO_MOVE								= 0x0001,
		FO_COPY								= 0x0002,
		FO_DELETE							= 0x0003,
		FO_RENAME							= 0x0004,
		
		// SHFILEOPSTRUCT.fFlags and IFileOperation::SetOperationFlags() flag values
		FOF_MULTIDESTFILES					= 0x0001,
		FOF_CONFIRMMOUSE					= 0x0002,
		FOF_SILENT							= 0x0004,
		FOF_RENAMEONCOLLISION				= 0x0008,
		FOF_NOCONFIRMATION					= 0x0010,
		FOF_WANTMAPPINGHANDLE				= 0x0020,
		FOF_FILESONLY						= 0x0080,
		FOF_SIMPLEPROGRESS					= 0x0100,
		FOF_NOCONFIRMMKDIR					= 0x0200,
		FOF_NOERRORUI						= 0x0400,
		FOF_NOCOPYSECURITYATTRIBS			= 0x0800,
		FOF_NORECURSION						= 0x1000,
		FOF_NO_CONNECTED_ELEMENTS			= 0x2000,
		FOF_WANTNUKEWARNING					= 0x4000,
		FOF_NO_UI							= (FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR),
		PO_DELETE							= 0x0013,
		PO_RENAME							= 0x0014,
		PO_PORTCHANGE						= 0x0020,
		PO_REN_PORT							= 0x0034,
		
		// generic
		GENERIC_READ						= 0x80000000L,
		GENERIC_WRITE						= 0x40000000L,
		GENERIC_EXECUTE						= 0x20000000L,
		GENERIC_ALL							= 0x10000000L,
		
		// 
		CREATE_NEW							= 1,
		CREATE_ALWAYS						= 2,
		OPEN_EXISTING						= 3,
		OPEN_ALWAYS							= 4,
		TRUNCATE_EXISTING					= 5,
		
		IGNORE								= 0,       // Ignore signal
		INFINITE							= 0xFFFFFFFF,  // Infinite timeout
		
		EXCEPTION_EXECUTE_HANDLER			= 1,

		
		// dwFlags definitions for CryptAcquireContext
		CRYPT_VERIFYCONTEXT					= 0xF0000000,
		CRYPT_NEWKEYSET						= 0x00000008,
		CRYPT_DELETEKEYSET					= 0x00000010,
		CRYPT_MACHINE_KEYSET				= 0x00000020,
		CRYPT_SILENT						= 0x00000040,

		// dwFlag definitions for CryptGenKey
		CRYPT_EXPORTABLE					= 0x00000001,
		CRYPT_USER_PROTECTED				= 0x00000002,
		CRYPT_CREATE_SALT					= 0x00000004,
		CRYPT_UPDATE_KEY					= 0x00000008,
		CRYPT_NO_SALT						= 0x00000010,
		CRYPT_PREGEN						= 0x00000040,
		CRYPT_RECIPIENT						= 0x00000010,
		CRYPT_INITIATOR						= 0x00000040,
		CRYPT_ONLINE						= 0x00000080,
		CRYPT_SF							= 0x00000100,
		CRYPT_CREATE_IV						= 0x00000200,
		CRYPT_KEK							= 0x00000400,
		CRYPT_DATA_KEY						= 0x00000800,
		CRYPT_VOLATILE						= 0x00001000,
		CRYPT_SGCKEY						= 0x00002000,

		RSA1024BIT_KEY						= 0x04000000,

		// dwFlags definitions for CryptDeriveKey
		CRYPT_SERVER						= 0x00000400,

		KEY_LENGTH_MASK						= 0xFFFF0000,

		// dwFlag definitions for CryptExportKey
		CRYPT_Y_ONLY						= 0x00000001,
		CRYPT_SSL2_FALLBACK					= 0x00000002,
		CRYPT_DESTROYKEY					= 0x00000004,
		CRYPT_OAEP							= 0x00000040,	// used with RSA encryptions/decryptions
															// CryptExportKey, CryptImportKey,
															// CryptEncrypt and CryptDecrypt

		CRYPT_BLOB_VER3						= 0x00000080,  // export version 3 of a blob type
		
		// dwFlags definitions for CryptCreateHash
		CRYPT_SECRETDIGEST					= 0x00000001,

		// dwFlags definitions for CryptHashSessionKey
		CRYPT_LITTLE_ENDIAN					= 0x00000001,

		// dwFlags definitions for CryptSignHash and CryptVerifySignature
		CRYPT_NOHASHOID						= 0x00000001,
		CRYPT_TYPE2_FORMAT					= 0x00000002,
		CRYPT_X931_FORMAT					= 0x00000004,

		// dwFlag definitions for CryptSetProviderEx and CryptGetDefaultProvider
		CRYPT_MACHINE_DEFAULT				= 0x00000001,
		CRYPT_USER_DEFAULT					= 0x00000002,
		CRYPT_DELETE_DEFAULT				= 0x00000004,
		
		// certenrolld_begin -- PROV_RSA_*
		PROV_RSA_FULL						= 1,
		PROV_RSA_SIG						= 2,
		PROV_DSS							= 3,
		PROV_FORTEZZA						= 4,
		PROV_MS_EXCHANGE					= 5,
		PROV_SSL							= 6,
		PROV_RSA_SCHANNEL					= 12,
		PROV_DSS_DH							= 13,
		PROV_EC_ECDSA_SIG					= 14,
		PROV_EC_ECNRA_SIG					= 15,
		PROV_EC_ECDSA_FULL					= 16,
		PROV_EC_ECNRA_FULL					= 17,
		PROV_DH_SCHANNEL					= 18,
		PROV_SPYRUS_LYNKS					= 20,
		PROV_RNG							= 21,
		PROV_INTEL_SEC						= 22,
		PROV_REPLACE_OWF					= 23,
		PROV_RSA_AES						= 24,
		
		// raw input mouse state
		MOUSE_MOVE_RELATIVE					= 0,
		MOUSE_MOVE_ABSOLUTE					= 1,
		MOUSE_VIRTUAL_DESKTOP				= 0x02,
		MOUSE_ATTRIBUTES_CHANGED			= 0x04,
		MOUSE_MOVE_NOCOALESCE				= 0x08,
			
		// move file flags
		MOVEFILE_REPLACE_EXISTING			= 0x00000001,
		MOVEFILE_COPY_ALLOWED				= 0x00000002,
		MOVEFILE_DELAY_UNTIL_REBOOT			= 0x00000004,
		MOVEFILE_WRITE_THROUGH				= 0x00000008,
		MOVEFILE_CREATE_HARDLINK			= 0x00000010,
		MOVEFILE_FAIL_IF_NOT_TRACKABLE		= 0x00000020,
	};
	


	// Standard Cursor IDs
	static const LPSTR	IDC_ARROW			= MAKEINTRESOURCEA( 32512 );
	static const LPSTR	IDC_IBEAM			= MAKEINTRESOURCEA( 32513 );
	static const LPSTR	IDC_WAIT			= MAKEINTRESOURCEA( 32514 );
	static const LPSTR	IDC_CROSS			= MAKEINTRESOURCEA( 32515 );
	static const LPSTR	IDC_UPARROW			= MAKEINTRESOURCEA( 32516 );
	static const LPSTR	IDC_SIZE			= MAKEINTRESOURCEA( 32640 );  /* OBSOLETE: use IDC_SIZEALL */
	static const LPSTR	IDC_ICON			= MAKEINTRESOURCEA( 32641 );  /* OBSOLETE: use IDC_ARROW */
	static const LPSTR	IDC_SIZENWSE		= MAKEINTRESOURCEA( 32642 );
	static const LPSTR	IDC_SIZENESW		= MAKEINTRESOURCEA( 32643 );
	static const LPSTR	IDC_SIZEWE			= MAKEINTRESOURCEA( 32644 );
	static const LPSTR	IDC_SIZENS			= MAKEINTRESOURCEA( 32645 );
	static const LPSTR	IDC_SIZEALL			= MAKEINTRESOURCEA( 32646 );
	static const LPSTR	IDC_NO				= MAKEINTRESOURCEA( 32648 ); /*not in win3.1 */
	static const LPSTR	IDC_HAND			= MAKEINTRESOURCEA( 32649 );
	static const LPSTR	IDC_APPSTARTING		= MAKEINTRESOURCEA( 32650 ); /*not in win3.1 */
	static const LPSTR	IDC_HELP			= MAKEINTRESOURCEA( 32651 );

	static const HANDLE	INVALID_HANDLE_VALUE	= HANDLE( LONG_PTR(-1) );
	static const HWND	HWND_TOP				= (HWND) 0;
	static const HWND	HWND_BOTTOM				= (HWND) 1;
	static const HWND	HWND_TOPMOST			= (HWND)-1;
	static const HWND	HWND_NOTOPMOST			= (HWND)-2;
	static const HKEY	HKEY_LOCAL_MACHINE		= (HKEY)(ULONG_PTR)((LONG)0x80000002);
	
	static const DWORD	STD_INPUT_HANDLE		= DWORD(-10);
	static const DWORD	STD_OUTPUT_HANDLE		= DWORD(-11);
	static const DWORD	STD_ERROR_HANDLE		= DWORD(-12);

	
	typedef struct _WIN32_FIND_DATAA {
		DWORD dwFileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		DWORD nFileSizeHigh;
		DWORD nFileSizeLow;
		DWORD dwReserved0;
		DWORD dwReserved1;
		CHAR   cFileName[ MAX_PATH ];
		CHAR   cAlternateFileName[ 14 ];
	#ifdef _MAC
		DWORD dwFileType;
		DWORD dwCreatorType;
		WORD  wFinderFlags;
	#endif
	} WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;

	typedef struct _WIN32_FIND_DATAW {
		DWORD dwFileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		DWORD nFileSizeHigh;
		DWORD nFileSizeLow;
		DWORD dwReserved0;
		DWORD dwReserved1;
		WCHAR  cFileName[ MAX_PATH ];
		WCHAR  cAlternateFileName[ 14 ];
	#ifdef _MAC
		DWORD dwFileType;
		DWORD dwCreatorType;
		WORD  wFinderFlags;
	#endif
	} WIN32_FIND_DATAW, *PWIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;
	
	typedef WORD FILEOP_FLAGS;

	typedef struct _SHFILEOPSTRUCTA
	{
		HWND            hwnd;
		UINT            wFunc;
		LPCSTR          pFrom;
		LPCSTR          pTo;
		FILEOP_FLAGS    fFlags;
		BOOL            fAnyOperationsAborted;
		LPVOID          hNameMappings;
		LPCSTR          lpszProgressTitle; // only used if FOF_SIMPLEPROGRESS
	} SHFILEOPSTRUCTA, *LPSHFILEOPSTRUCTA;
	typedef struct _SHFILEOPSTRUCTW
	{
		HWND            hwnd;
		UINT            wFunc;
		LPCWSTR         pFrom;
		LPCWSTR         pTo;
		FILEOP_FLAGS    fFlags;
		BOOL            fAnyOperationsAborted;
		LPVOID          hNameMappings;
		LPCWSTR         lpszProgressTitle; // only used if FOF_SIMPLEPROGRESS
	} SHFILEOPSTRUCTW, *LPSHFILEOPSTRUCTW;


extern "C" {


	// ANSI and UNICODE versions
	namespace _winapi_hidden_
	{
		WINBASEAPI
		int
		WINAPI
		MessageBoxA(
			__in_opt HWND hWnd,
			__in_opt LPCSTR lpText,
			__in_opt LPCSTR lpCaption,
			__in UINT uType);

		WINBASEAPI
		int
		WINAPI
		MessageBoxW(
			__in_opt HWND hWnd,
			__in_opt LPCWSTR lpText,
			__in_opt LPCWSTR lpCaption,
			__in UINT uType);


		WINBASEAPI
		__out_opt
		HMODULE
		WINAPI
		LoadLibraryA(
			__in LPCSTR lpLibFileName
			);

		WINBASEAPI
		__out_opt
		HMODULE
		WINAPI
		LoadLibraryW(
			__in LPCWSTR lpLibFileName
			);

		
		WINBASEAPI
		__out_opt
		HANDLE
		WINAPI
		CreateMutexA(
			__in_opt LPSECURITY_ATTRIBUTES lpMutexAttributes,
			__in     BOOL bInitialOwner,
			__in_opt LPCSTR lpName
			);

		WINBASEAPI
		__out_opt
		HANDLE
		WINAPI
		CreateMutexW(
			__in_opt LPSECURITY_ATTRIBUTES lpMutexAttributes,
			__in     BOOL bInitialOwner,
			__in_opt LPCWSTR lpName
			);

		
		WINBASEAPI
		__out_opt
		HANDLE
		WINAPI
		CreateEventA(
			__in_opt LPSECURITY_ATTRIBUTES lpEventAttributes,
			__in     BOOL bManualReset,
			__in     BOOL bInitialState,
			__in_opt LPCSTR lpName
			);

		WINBASEAPI
		__out_opt
		HANDLE
		WINAPI
		CreateEventW(
			__in_opt LPSECURITY_ATTRIBUTES lpEventAttributes,
			__in     BOOL bManualReset,
			__in     BOOL bInitialState,
			__in_opt LPCWSTR lpName
			);

		
		WINBASEAPI
		__out_opt
		HANDLE
		WINAPI
		CreateSemaphoreA(
			__in_opt LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
			__in     LONG lInitialCount,
			__in     LONG lMaximumCount,
			__in_opt LPCSTR lpName
			);

		WINBASEAPI
		__out_opt
		HANDLE
		WINAPI
		CreateSemaphoreW(
			__in_opt LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
			__in     LONG lInitialCount,
			__in     LONG lMaximumCount,
			__in_opt LPCWSTR lpName
			);
		

		WINBASEAPI
		__out_opt
		HRSRC
		WINAPI
		FindResourceA(
			__in_opt HMODULE hModule,
			__in     LPCSTR lpName,
			__in     LPCSTR lpType
			);

		WINBASEAPI
		__out_opt
		HRSRC
		WINAPI
		FindResourceW(
			__in_opt HMODULE hModule,
			__in     LPCWSTR lpName,
			__in     LPCWSTR lpType
			);

		WINBASEAPI
		DWORD
		WINAPI
		GetFileAttributesA(
			__in LPCSTR lpFileName
			);

		WINBASEAPI
		DWORD
		WINAPI
		GetFileAttributesW(
			__in LPCWSTR lpFileName
			);


		WINBASEAPI
		BOOL
		WINAPI
		SetCurrentDirectoryA(
			__in LPCSTR lpPathName
			);

		WINBASEAPI
		BOOL
		WINAPI
		SetCurrentDirectoryW(
			__in LPCWSTR lpPathName
			);

		WINBASEAPI
		DWORD
		WINAPI
		GetCurrentDirectoryA(
			__in DWORD nBufferLength,
			LPSTR lpBuffer
			);

		WINBASEAPI
		DWORD
		WINAPI
		GetCurrentDirectoryW(
			__in DWORD nBufferLength,
			LPWSTR lpBuffer
			);

		WINBASEAPI
		BOOL
		WINAPI
		CreateDirectoryA(
			__in     LPCSTR lpPathName,
			__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes
			);

		WINBASEAPI
		BOOL
		WINAPI
		CreateDirectoryW(
			__in     LPCWSTR lpPathName,
			__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes
			);
		

		WINBASEAPI
		__out
		HANDLE
		WINAPI
		FindFirstFileA(
			__in  LPCSTR lpFileName,
			__out LPWIN32_FIND_DATAA lpFindFileData
			);

		WINBASEAPI
		__out
		HANDLE
		WINAPI
		FindFirstFileW(
			__in  LPCWSTR lpFileName,
			__out LPWIN32_FIND_DATAW lpFindFileData
			);

		WINBASEAPI
		BOOL
		WINAPI
		FindNextFileA(
			__in  HANDLE hFindFile,
			__out LPWIN32_FIND_DATAA lpFindFileData
			);

		WINBASEAPI
		BOOL
		WINAPI
		FindNextFileW(
			__in  HANDLE hFindFile,
			__out LPWIN32_FIND_DATAW lpFindFileData
			);

		
		WINUSERAPI
		BOOL
		WINAPI
		GetClassInfoA(
			__in_opt HINSTANCE hInstance,
			__in LPCSTR lpClassName,
			__out LPWNDCLASSA lpWndClass);

		WINUSERAPI
		BOOL
		WINAPI
		GetClassInfoW(
			__in_opt HINSTANCE hInstance,
			__in LPCWSTR lpClassName,
			__out LPWNDCLASSW lpWndClass);

		WINUSERAPI
		HCURSOR
		WINAPI
		LoadCursorA(
			__in_opt HINSTANCE hInstance,
			__in LPCSTR lpCursorName);

		WINUSERAPI
		HCURSOR
		WINAPI
		LoadCursorW(
			__in_opt HINSTANCE hInstance,
			__in LPCWSTR lpCursorName);

		
		WINUSERAPI
		ATOM
		WINAPI
		RegisterClassExA(
			__in CONST WNDCLASSEXA *);

		WINUSERAPI
		ATOM
		WINAPI
		RegisterClassExW(
			__in CONST WNDCLASSEXW *);
		
		WINUSERAPI
		BOOL
		WINAPI
		UnregisterClassA(
			__in LPCSTR lpClassName,
			__in_opt HINSTANCE hInstance);

		WINUSERAPI
		BOOL
		WINAPI
		UnregisterClassW(
			__in LPCWSTR lpClassName,
			__in_opt HINSTANCE hInstance);

		
		WINUSERAPI
		BOOL
		WINAPI
		SetWindowTextA(
			__in HWND hWnd,
			__in_opt LPCSTR lpString);

		WINUSERAPI
		BOOL
		WINAPI
		SetWindowTextW(
			__in HWND hWnd,
			__in_opt LPCWSTR lpString);


		WINUSERAPI
		int
		WINAPI
		GetWindowTextA(
			__in HWND hWnd,
			__out LPSTR lpString,
			__in int nMaxCount);

		WINUSERAPI
		int
		WINAPI
		GetWindowTextW(
			__in HWND hWnd,
			__out LPWSTR lpString,
			__in int nMaxCount);


		WINUSERAPI
		HWND
		WINAPI
		CreateWindowExA(
			__in DWORD dwExStyle,
			__in_opt LPCSTR lpClassName,
			__in_opt LPCSTR lpWindowName,
			__in DWORD dwStyle,
			__in int X,
			__in int Y,
			__in int nWidth,
			__in int nHeight,
			__in_opt HWND hWndParent,
			__in_opt HMENU hMenu,
			__in_opt HINSTANCE hInstance,
			__in_opt LPVOID lpParam);

		WINUSERAPI
		HWND
		WINAPI
		CreateWindowExW(
			__in DWORD dwExStyle,
			__in_opt LPCWSTR lpClassName,
			__in_opt LPCWSTR lpWindowName,
			__in DWORD dwStyle,
			__in int X,
			__in int Y,
			__in int nWidth,
			__in int nHeight,
			__in_opt HWND hWndParent,
			__in_opt HMENU hMenu,
			__in_opt HINSTANCE hInstance,
			__in_opt LPVOID lpParam);

	
		WINUSERAPI
		LONG
		WINAPI
		ChangeDisplaySettingsA(
			__in_opt DEVMODEA* lpDevMode,
			__in DWORD dwFlags);

		WINUSERAPI
		LONG
		WINAPI
		ChangeDisplaySettingsW(
			__in_opt DEVMODEW* lpDevMode,
			__in DWORD dwFlags);
		

		WINBASEAPI
		__out_opt
		HMODULE
		WINAPI
		GetModuleHandleA(
			__in_opt LPCSTR lpModuleName
			);

		WINBASEAPI
		__out_opt
		HMODULE
		WINAPI
		GetModuleHandleW(
			__in_opt LPCWSTR lpModuleName
			);

	
		WINUSERAPI
		BOOL
		WINAPI
		EnumDisplaySettingsA(
			__in_opt LPCSTR lpszDeviceName,
			__in DWORD iModeNum,
			__inout DEVMODEA* lpDevMode);

		WINUSERAPI
		BOOL
		WINAPI
		EnumDisplaySettingsW(
			__in_opt LPCWSTR lpszDeviceName,
			__in DWORD iModeNum,
			__inout DEVMODEW* lpDevMode);
		
		WINUSERAPI
		BOOL
		WINAPI
		EnumDisplayDevicesA(
			__in_opt LPCSTR lpDevice,
			__in DWORD iDevNum,
			__inout PDISPLAY_DEVICEA lpDisplayDevice,
			__in DWORD dwFlags);

		WINUSERAPI
		BOOL
		WINAPI
		EnumDisplayDevicesW(
			__in_opt LPCWSTR lpDevice,
			__in DWORD iDevNum,
			__inout PDISPLAY_DEVICEW lpDisplayDevice,
			__in DWORD dwFlags);

	
		WINBASEAPI
		BOOL
		WINAPI
		GetVersionExA(
			__inout LPOSVERSIONINFOA lpVersionInformation
			);
		WINBASEAPI
		BOOL
		WINAPI
		GetVersionExW(
			__inout LPOSVERSIONINFOW lpVersionInformation
			);
		
		WINADVAPI
		LSTATUS
		APIENTRY
		RegOpenKeyExA (
			__in HKEY hKey,
			__in_opt LPCSTR lpSubKey,
			__in_opt DWORD ulOptions,
			__in REGSAM samDesired,
			__out PHKEY phkResult
			);

		WINADVAPI
		LSTATUS
		APIENTRY
		RegOpenKeyExW (
			__in HKEY hKey,
			__in_opt LPCWSTR lpSubKey,
			__in_opt DWORD ulOptions,
			__in REGSAM samDesired,
			__out PHKEY phkResult
			);
		
		WINADVAPI
		LSTATUS
		APIENTRY
		RegQueryValueExA (
			__in HKEY hKey,
			__in_opt LPCSTR lpValueName,
			__reserved LPDWORD lpReserved,
			__out_opt LPDWORD lpType,
			__out_opt LPBYTE lpData,
			__inout_opt LPDWORD lpcbData
			);

		WINADVAPI
		LSTATUS
		APIENTRY
		RegQueryValueExW (
			__in HKEY hKey,
			__in_opt LPCWSTR lpValueName,
			__reserved LPDWORD lpReserved,
			__out_opt LPDWORD lpType,
			__out_opt LPBYTE lpData,
			__inout_opt LPDWORD lpcbData
			);

		
		WINUSERAPI
		HICON
		WINAPI
		LoadIconA(
			__in_opt HINSTANCE hInstance,
			__in LPCSTR lpIconName);

		WINUSERAPI
		HICON
		WINAPI
		LoadIconW(
			__in_opt HINSTANCE hInstance,
			__in LPCWSTR lpIconName);

		
		WINUSERAPI
		HBITMAP
		WINAPI
		LoadBitmapA(
			__in_opt HINSTANCE hInstance,
			__in LPCSTR lpBitmapName);

		WINUSERAPI
		HBITMAP
		WINAPI
		LoadBitmapW(
			__in_opt HINSTANCE hInstance,
			__in LPCWSTR lpBitmapName);

		
		WINBASEAPI
		VOID
		WINAPI
		OutputDebugStringA(
			__in_opt LPCSTR lpOutputString
			);

		WINBASEAPI
		VOID
		WINAPI
		OutputDebugStringW(
			__in_opt LPCWSTR lpOutputString
			);

	
		WINBASEAPI
		DWORD
		WINAPI
		GetModuleFileNameA(
			__in_opt HMODULE hModule,
			__out LPSTR lpFilename,
			__in     DWORD nSize
			);

		WINBASEAPI
		DWORD
		WINAPI
		GetModuleFileNameW(
			__in_opt HMODULE hModule,
			__out LPWSTR lpFilename,
			__in     DWORD nSize
			);


		WINBASEAPI
		UINT
		WINAPI
		GetSystemDirectoryA(
			__out LPSTR lpBuffer,
			__in UINT uSize
			);

		WINBASEAPI
		UINT
		WINAPI
		GetSystemDirectoryW(
			__out LPWSTR lpBuffer,
			__in UINT uSize
			);

		
		WINBASEAPI
		BOOL
		WINAPI
		DeleteFileA(
			__in LPCSTR lpFileName
			);

		WINBASEAPI
		BOOL
		WINAPI
		DeleteFileW(
			__in LPCWSTR lpFileName
			);

		
		WINBASEAPI
		BOOL
		WINAPI
		RemoveDirectoryA(
			__in LPCSTR lpPathName
			);

		WINBASEAPI
		BOOL
		WINAPI
		RemoveDirectoryW(
			__in LPCWSTR lpPathName
			);
		
		SHSTDAPI_(int) SHFileOperationA(__inout LPSHFILEOPSTRUCTA lpFileOp);
		SHSTDAPI_(int) SHFileOperationW(__inout LPSHFILEOPSTRUCTW lpFileOp);

	
		WINBASEAPI
		BOOL
		WINAPI
		CopyFileA(
			__in LPCSTR lpExistingFileName,
			__in LPCSTR lpNewFileName,
			__in BOOL bFailIfExists
			);

		WINBASEAPI
		BOOL
		WINAPI
		CopyFileW(
			__in LPCWSTR lpExistingFileName,
			__in LPCWSTR lpNewFileName,
			__in BOOL bFailIfExists
			);
		
		WINBASEAPI
		BOOL
		WINAPI
		MoveFileExA(
			_In_     LPCSTR lpExistingFileName,
			_In_opt_ LPCSTR lpNewFileName,
			_In_     DWORD    dwFlags
			);

		WINBASEAPI
		BOOL
		WINAPI
		MoveFileExW(
			_In_     LPCWSTR lpExistingFileName,
			_In_opt_ LPCWSTR lpNewFileName,
			_In_     DWORD    dwFlags
			);

		SHSTDAPI_(HINSTANCE) ShellExecuteA(__in_opt HWND hwnd, __in_opt LPCSTR lpOperation, __in LPCSTR lpFile, __in_opt LPCSTR lpParameters,
			__in_opt LPCSTR lpDirectory, __in INT nShowCmd);

		SHSTDAPI_(HINSTANCE) ShellExecuteW(__in_opt HWND hwnd, __in_opt LPCWSTR lpOperation, __in LPCWSTR lpFile, __in_opt LPCWSTR lpParameters,
			__in_opt LPCWSTR lpDirectory, __in INT nShowCmd);
		
		WINBASEAPI
		__out
		HANDLE
		WINAPI
		CreateFileA(
			__in     LPCSTR lpFileName,
			__in     DWORD dwDesiredAccess,
			__in     DWORD dwShareMode,
			__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			__in     DWORD dwCreationDisposition,
			__in     DWORD dwFlagsAndAttributes,
			__in_opt HANDLE hTemplateFile
			);

		WINBASEAPI
		__out
		HANDLE
		WINAPI
		CreateFileW(
			__in     LPCWSTR lpFileName,
			__in     DWORD dwDesiredAccess,
			__in     DWORD dwShareMode,
			__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			__in     DWORD dwCreationDisposition,
			__in     DWORD dwFlagsAndAttributes,
			__in_opt HANDLE hTemplateFile
			);
		
		WINADVAPI
		BOOL
		WINAPI
		CryptEnumProvidersA(
			__in        DWORD   dwIndex,
			__reserved  DWORD   *pdwReserved,
			__in        DWORD   dwFlags,
			__out       DWORD   *pdwProvType,
			__out       LPSTR szProvName,
			__inout     DWORD   *pcbProvName
			);
		WINADVAPI
		BOOL
		WINAPI
		CryptEnumProvidersW(
			__in        DWORD   dwIndex,
			__reserved  DWORD   *pdwReserved,
			__in        DWORD   dwFlags,
			__out       DWORD   *pdwProvType,
			__out       LPWSTR szProvName,
			__inout     DWORD   *pcbProvName
			);
		
		WINADVAPI
		BOOL
		WINAPI
		CryptAcquireContextA(
			__out       HCRYPTPROV  *phProv,
			__in_opt    LPCSTR    szContainer,
			__in_opt    LPCSTR    szProvider,
			__in        DWORD       dwProvType,
			__in        DWORD       dwFlags
			);
		WINADVAPI
		BOOL
		WINAPI
		CryptAcquireContextW(
			__out       HCRYPTPROV  *phProv,
			__in_opt    LPCWSTR    szContainer,
			__in_opt    LPCWSTR    szProvider,
			__in        DWORD       dwProvType,
			__in        DWORD       dwFlags
			);

	}	// _winapi_hidden_
	
}	// extern "C"

	

// other WINAPI functions
extern "C" {

	WINBASEAPI
	BOOL
	WINAPI
	QueryPerformanceCounter(
		__out LARGE_INTEGER *lpPerformanceCount
		);

	WINBASEAPI
	BOOL
	WINAPI
	QueryPerformanceFrequency(
		__out LARGE_INTEGER *lpFrequency
		);

	WINBASEAPI
	BOOL
	WINAPI
	FreeLibrary (
		__in HMODULE hLibModule
		);

	WINBASEAPI
	FARPROC
	WINAPI
	GetProcAddress (
		__in HMODULE hModule,
		__in LPCSTR lpProcName
		);

	
	WINBASEAPI
	LPVOID
	WINAPI
	TlsGetValue(
		__in DWORD dwTlsIndex
		);

	WINBASEAPI
	BOOL
	WINAPI
	TlsFree(
		__in DWORD dwTlsIndex
		);

	WINBASEAPI
	HLOCAL
	WINAPI
	LocalFree(
		HLOCAL hMem
		);

	WINBASEAPI
	BOOL
	WINAPI
	TlsSetValue(
		__in     DWORD dwTlsIndex,
		__in_opt LPVOID lpTlsValue
		);

	WINBASEAPI
	HLOCAL
	WINAPI
	LocalAlloc(
		__in UINT uFlags,
		__in SIZE_T uBytes
		);

	WINBASEAPI
	DWORD
	WINAPI
	TlsAlloc(
		VOID
		);
	

	WINBASEAPI
	__out
	HANDLE
	WINAPI
	GetCurrentThread(
		VOID
		);

	WINBASEAPI
	DWORD
	WINAPI
	GetCurrentThreadId(
		VOID
		);

	WINBASEAPI
	DWORD
	WINAPI
	SetThreadIdealProcessor(
		__in HANDLE hThread,
		__in DWORD dwIdealProcessor
		);

	WINBASEAPI
	BOOL
	WINAPI
	SetThreadPriority(
		__in HANDLE hThread,
		__in int nPriority
		);

	WINBASEAPI
	DWORD
	WINAPI
	SuspendThread(
		__in HANDLE hThread
		);

	WINBASEAPI
	DWORD
	WINAPI
	ResumeThread(
		__in HANDLE hThread
		);

	WINBASEAPI
	BOOL
	WINAPI
	GetExitCodeThread(
		__in  HANDLE hThread,
		__out LPDWORD lpExitCode
		);

	WINBASEAPI
	__out
	HANDLE
	WINAPI
	GetCurrentProcess(
		VOID
		);

	WINBASEAPI
	BOOL
	WINAPI
	TerminateThread(
		__in HANDLE hThread,
		__in DWORD dwExitCode
		);


	UINT_PTR __cdecl _beginthread (void (__cdecl * _StartAddress) (void *),
			unsigned _StackSize, void * _ArgList);

	void __cdecl _endthread(void);

	UINT_PTR __cdecl _beginthreadex(void * _Security, unsigned _StackSize,
			unsigned (__stdcall * _StartAddress) (void *), void * _ArgList, 
			unsigned _InitFlag, unsigned * _ThrdAddr);

	void __cdecl _endthreadex(unsigned _Retval);


	WINBASEAPI
	BOOL
	WINAPI
	CloseHandle(
		__in HANDLE hObject
		);

	WINBASEAPI
	BOOL
	WINAPI
	DuplicateHandle(
		__in        HANDLE hSourceProcessHandle,
		__in        HANDLE hSourceHandle,
		__in        HANDLE hTargetProcessHandle,
		__deref_out LPHANDLE lpTargetHandle,
		__in        DWORD dwDesiredAccess,
		__in        BOOL bInheritHandle,
		__in        DWORD dwOptions
		);

	WINBASEAPI
	DWORD
	WINAPI
	WaitForSingleObject(
		__in HANDLE hHandle,
		__in DWORD dwMilliseconds
		);
	
	WINBASEAPI
	DWORD
	WINAPI
	WaitForMultipleObjects(
		__in DWORD nCount,
		CONST HANDLE *lpHandles,
		__in BOOL bWaitAll,
		__in DWORD dwMilliseconds
		);

	WINBASEAPI
	BOOL
	WINAPI
	ReleaseMutex(
		__in HANDLE hMutex
		);


	WINBASEAPI
	BOOL
	WINAPI
	SetEvent(
		__in HANDLE hEvent
		);

	WINBASEAPI
	BOOL
	WINAPI
	ResetEvent(
		__in HANDLE hEvent
		);

	WINBASEAPI
	BOOL
	WINAPI
	PulseEvent(
		__in HANDLE hEvent
		);


	WINBASEAPI
	VOID
	WINAPI
	InitializeCriticalSection(
		__out LPCRITICAL_SECTION lpCriticalSection
		);

	WINBASEAPI
	VOID
	WINAPI
	EnterCriticalSection(
		__inout LPCRITICAL_SECTION lpCriticalSection
		);

	WINBASEAPI
	VOID
	WINAPI
	LeaveCriticalSection(
		__inout LPCRITICAL_SECTION lpCriticalSection
		);
	
	WINBASEAPI
	BOOL
	WINAPI
	TryEnterCriticalSection(
		__inout LPCRITICAL_SECTION lpCriticalSection
		);

	WINBASEAPI
	VOID
	WINAPI
	DeleteCriticalSection(
		__inout LPCRITICAL_SECTION lpCriticalSection
		);

	
	WINBASEAPI
	BOOL
	WINAPI
	ReleaseSemaphore(
		__in      HANDLE hSemaphore,
		__in      LONG lReleaseCount,
		__out_opt LPLONG lpPreviousCount
		);


	
	WINBASEAPI
	LONGLONG
	WINAPI
	InterlockedCompareExchange64 (
		__inout LONGLONG volatile *Destination,
		__in    LONGLONG Exchange,
		__in    LONGLONG Comperand
		);

		
	WINBASEAPI
	VOID
	WINAPI
	Sleep(
		__in DWORD dwMilliseconds
		);

	WINBASEAPI
	__out_opt
	HGLOBAL
	WINAPI
	LoadResource(
		__in_opt HMODULE hModule,
		__in HRSRC hResInfo
		);

	WINBASEAPI
	DWORD
	WINAPI
	SizeofResource(
		__in_opt HMODULE hModule,
		__in HRSRC hResInfo
		);

	WINBASEAPI
	BOOL
	WINAPI
	FreeResource(
		__in HGLOBAL hResData
		);

	WINBASEAPI
	LPVOID
	WINAPI
	LockResource(
		__in HGLOBAL hResData
		);

	
	WINUSERAPI
	BOOL
	WINAPI
	DestroyIcon(
		__in HICON hIcon);


	WINBASEAPI
	BOOL
	WINAPI
	FindClose(
		__inout HANDLE hFindFile
		);

		
	WINUSERAPI
	LRESULT
	WINAPI
	DefWindowProcA(
		__in HWND hWnd,
		__in UINT Msg,
		__in WPARAM wParam,
		__in LPARAM lParam);

	WINUSERAPI
	LRESULT
	WINAPI
	DefWindowProcW(
		__in HWND hWnd,
		__in UINT Msg,
		__in WPARAM wParam,
		__in LPARAM lParam);

	
	WINUSERAPI
	BOOL
	WINAPI
	GetClientRect(
		__in HWND hWnd,
		__out LPRECT lpRect);

	WINUSERAPI
	BOOL
	WINAPI
	GetWindowRect(
		__in HWND hWnd,
		__out LPRECT lpRect);

	WINUSERAPI
	BOOL
	WINAPI
	AdjustWindowRect(
		__inout LPRECT lpRect,
		__in DWORD dwStyle,
		__in BOOL bMenu);

	WINUSERAPI
	BOOL
	WINAPI
	AdjustWindowRectEx(
		__inout LPRECT lpRect,
		__in DWORD dwStyle,
		__in BOOL bMenu,
		__in DWORD dwExStyle);
	

	WINUSERAPI
	LONG
	WINAPI
	GetWindowLongA(
		__in HWND hWnd,
		__in int nIndex);

	WINUSERAPI
	LONG
	WINAPI
	GetWindowLongW(
		__in HWND hWnd,
		__in int nIndex);
	
	WINUSERAPI
	BOOL
	WINAPI
	SetForegroundWindow(
		__in HWND hWnd);
	
	WINUSERAPI
	BOOL
	WINAPI
	DestroyWindow(
		__in HWND hWnd);

	WINUSERAPI
	BOOL
	WINAPI
	ShowWindow(
		__in HWND hWnd,
		__in int nCmdShow);
	
	
	WINUSERAPI
	HWND
	WINAPI
	SetFocus(
		__in_opt HWND hWnd);

	WINUSERAPI
	HWND
	WINAPI
	GetActiveWindow(
		VOID);

	WINUSERAPI
	HWND
	WINAPI
	GetFocus(
		VOID);
	
	WINUSERAPI
	int
	WINAPI
	ToAscii(
		__in UINT uVirtKey,
		__in UINT uScanCode,
		__in_opt CONST BYTE *lpKeyState,
		__out LPWORD lpChar,
		__in UINT uFlags);

	WINUSERAPI
	int
	WINAPI
	ToAsciiEx(
		__in UINT uVirtKey,
		__in UINT uScanCode,
		__in_opt CONST BYTE *lpKeyState,
		__out LPWORD lpChar,
		__in UINT uFlags,
		__in_opt HKL dwhkl);

	WINUSERAPI
	int
	WINAPI
	ToUnicode(
		__in UINT wVirtKey,
		__in UINT wScanCode,
		__in_opt CONST BYTE *lpKeyState,
		__out LPWSTR pwszBuff,
		__in int cchBuff,
		__in UINT wFlags);
	
	WINUSERAPI
	SHORT
	WINAPI
	GetAsyncKeyState(
		__in int vKey);

	
	WINUSERAPI
	int
	WINAPI
	ShowCursor(
		__in BOOL bShow);

	WINUSERAPI
	BOOL
	WINAPI
	SetCursorPos(
		__in int X,
		__in int Y);


	WINUSERAPI
	BOOL
	WINAPI
	SetPhysicalCursorPos(
		__in int X,
		__in int Y);

	WINUSERAPI
	HCURSOR
	WINAPI
	SetCursor(
		__in_opt HCURSOR hCursor);

	WINUSERAPI
	BOOL
	WINAPI
	GetCursorPos(
		__out LPPOINT lpPoint);

	WINUSERAPI
	BOOL
	WINAPI
	GetPhysicalCursorPos(
		__out LPPOINT lpPoint);

	WINUSERAPI
	BOOL
	WINAPI
	ClipCursor(
		__in_opt CONST RECT *lpRect);

	WINUSERAPI
	BOOL
	WINAPI
	GetClipCursor(
		__out LPRECT lpRect);

	WINUSERAPI
	HCURSOR
	WINAPI
	GetCursor(
		VOID);
	

	WINBASEAPI
	VOID
	WINAPI
	GlobalMemoryStatus(
		__out LPMEMORYSTATUS lpBuffer
		);

	WINBASEAPI
	BOOL
	WINAPI
	GlobalMemoryStatusEx(
		__out LPMEMORYSTATUSEX lpBuffer
		);

	
	WINUSERAPI
	UINT_PTR
	WINAPI
	SetTimer(
		__in_opt HWND hWnd,
		__in UINT_PTR nIDEvent,
		__in UINT uElapse,
		__in_opt TIMERPROC lpTimerFunc);

	WINUSERAPI
	BOOL
	WINAPI
	KillTimer(
		__in_opt HWND hWnd,
		__in UINT_PTR uIDEvent);
	
	WINUSERAPI
	HDC
	WINAPI
	GetDC(
		__in_opt HWND hWnd);

	WINUSERAPI
	HDC
	WINAPI
	GetDCEx(
		__in_opt HWND hWnd,
		__in_opt HRGN hrgnClip,
		__in DWORD flags);
	
	WINUSERAPI
	HDC
	WINAPI
	GetWindowDC(
		__in_opt HWND hWnd);

	WINUSERAPI
	int
	WINAPI
	ReleaseDC(
		__in_opt HWND hWnd,
		__in HDC hDC);
	

	WINUSERAPI
	int
	WINAPI
	GetSystemMetrics(
		__in int nIndex);

	WINGDIAPI
	int
	WINAPI
	GetDeviceCaps( __in_opt HDC hdc, __in int index);
	

	WINUSERAPI
	LONG
	WINAPI
	SetWindowLongA(
		__in HWND hWnd,
		__in int nIndex,
		__in LONG dwNewLong);

	WINUSERAPI
	LONG
	WINAPI
	SetWindowLongW(
		__in HWND hWnd,
		__in int nIndex,
		__in LONG dwNewLong);

	WINUSERAPI
	LONG
	WINAPI
	GetWindowLongA(
		__in HWND hWnd,
		__in int nIndex);

	WINUSERAPI
	LONG
	WINAPI
	GetWindowLongW(
		__in HWND hWnd,
		__in int nIndex);

#if PLATFORM_BITS == 64

	WINUSERAPI
	LONG_PTR
	WINAPI
	SetWindowLongPtrA(
		__in HWND hWnd,
		__in int nIndex,
		__in LONG_PTR dwNewLong);

	WINUSERAPI
	LONG_PTR
	WINAPI
	SetWindowLongPtrW(
		__in HWND hWnd,
		__in int nIndex,
		__in LONG_PTR dwNewLong);

	WINUSERAPI
	LONG_PTR
	WINAPI
	GetWindowLongPtrA(
		__in HWND hWnd,
		__in int nIndex);

	WINUSERAPI
	LONG_PTR
	WINAPI
	GetWindowLongPtrW(
		__in HWND hWnd,
		__in int nIndex);

#else

	inline
	LONG_PTR
	WINAPI
	SetWindowLongPtrA(
		__in HWND hWnd,
		__in int nIndex,
		__in LONG_PTR dwNewLong)
	{
		return SetWindowLongA( hWnd, nIndex, (LONG)dwNewLong );
	}

	inline
	LONG_PTR
	WINAPI
	SetWindowLongPtrW(
		__in HWND hWnd,
		__in int nIndex,
		__in LONG_PTR dwNewLong)
	{
		return SetWindowLongW( hWnd, nIndex, (LONG)dwNewLong );
	}
	
	inline
	LONG_PTR
	WINAPI
	GetWindowLongPtrA(
		__in HWND hWnd,
		__in int nIndex)
	{
		return (LONG_PTR)GetWindowLongA( hWnd, nIndex );
	}

	inline
	LONG_PTR
	WINAPI
	GetWindowLongPtrW(
		__in HWND hWnd,
		__in int nIndex)
	{
		return (LONG_PTR)GetWindowLongW( hWnd, nIndex );
	}

#endif

	
	WINUSERAPI
	BOOL
	WINAPI
	PeekMessageA(
		__out LPMSG lpMsg,
		__in_opt HWND hWnd,
		__in UINT wMsgFilterMin,
		__in UINT wMsgFilterMax,
		__in UINT wRemoveMsg);

	WINUSERAPI
	BOOL
	WINAPI
	PeekMessageW(
		__out LPMSG lpMsg,
		__in_opt HWND hWnd,
		__in UINT wMsgFilterMin,
		__in UINT wMsgFilterMax,
		__in UINT wRemoveMsg);
	
	WINUSERAPI
	BOOL
	WINAPI
	PostMessageA(
		__in_opt HWND hWnd,
		__in UINT Msg,
		__in WPARAM wParam,
		__in LPARAM lParam);

	WINUSERAPI
	BOOL
	WINAPI
	PostMessageW(
		__in_opt HWND hWnd,
		__in UINT Msg,
		__in WPARAM wParam,
		__in LPARAM lParam);
	

	WINUSERAPI
	BOOL
	WINAPI
	TranslateMessage(
		__in CONST MSG *lpMsg);

	WINUSERAPI
	LRESULT
	WINAPI
	DispatchMessageA(
		__in CONST MSG *lpMsg);

	WINUSERAPI
	LRESULT
	WINAPI
	DispatchMessageW(
		__in CONST MSG *lpMsg);
	

	WINBASEAPI
	VOID
	WINAPI
	GetLocalTime(
		__out LPSYSTEMTIME lpSystemTime
		);


	WINUSERAPI
	BOOL
	WINAPI
	CloseWindow(
		__in  HWND hWnd);

	WINUSERAPI
	BOOL
	WINAPI
	MoveWindow(
		__in HWND hWnd,
		__in int X,
		__in int Y,
		__in int nWidth,
		__in int nHeight,
		__in BOOL bRepaint);

	WINUSERAPI
	BOOL
	WINAPI
	SetWindowPos(
		__in HWND hWnd,
		__in_opt HWND hWndInsertAfter,
		__in int X,
		__in int Y,
		__in int cx,
		__in int cy,
		__in UINT uFlags);
	
	WINBASEAPI
	VOID
	WINAPI
	GetSystemInfo(
		__out LPSYSTEM_INFO lpSystemInfo
		);
	

	WINADVAPI
	LSTATUS
	APIENTRY
	RegCloseKey (
		__in HKEY hKey
		);
	
	WINUSERAPI
	BOOL
	WINAPI
	RegisterRawInputDevices(
		__in PCRAWINPUTDEVICE pRawInputDevices,
		__in UINT uiNumDevices,
		__in UINT cbSize);

	WINUSERAPI
	UINT
	WINAPI
	GetRawInputData(
		__in HRAWINPUT hRawInput,
		__in UINT uiCommand,
		__out_opt LPVOID pData,
		__inout PUINT pcbSize,
		__in UINT cbSizeHeader);
	
	WINUSERAPI
	BOOL
	WINAPI
	GetKeyboardState(
		__out PBYTE lpKeyState);
	
	WINUSERAPI
	UINT
	WINAPI
	MapVirtualKeyA(
		__in UINT uCode,
		__in UINT uMapType);

	WINUSERAPI
	UINT
	WINAPI
	MapVirtualKeyW(
		__in UINT uCode,
		__in UINT uMapType);

	WINUSERAPI
	int
	WINAPI
	ToUnicodeEx(
		__in UINT wVirtKey,
		__in UINT wScanCode,
		__in CONST BYTE *lpKeyState,
		__out LPWSTR pwszBuff,
		__in int cchBuff,
		__in UINT wFlags,
		__in_opt HKL dwhkl);
	
	WINUSERAPI
	HKL
	WINAPI
	GetKeyboardLayout(
		__in DWORD idThread);
	
	WINUSERAPI
	BOOL
	WINAPI
	DestroyCursor(
		__in HCURSOR hCursor);
	
	WINUSERAPI
	HCURSOR
	WINAPI
	CreateCursor(
		__in_opt HINSTANCE hInst,
		__in int xHotSpot,
		__in int yHotSpot,
		__in int nWidth,
		__in int nHeight,
		__in CONST VOID *pvANDPlane,
		__in CONST VOID *pvXORPlane);
	
	WINUSERAPI
	BOOL
	WINAPI
	ClipCursor(
		__in_opt CONST RECT *lpRect);
												
	WINUSERAPI
	DWORD
	WINAPI
	SetClassLongA(
		__in HWND hWnd,
		__in int nIndex,
		__in LONG dwNewLong);

	WINUSERAPI
	DWORD
	WINAPI
	SetClassLongW(
		__in HWND hWnd,
		__in int nIndex,
		__in LONG dwNewLong);

	WINUSERAPI
	UINT
	WINAPI
	SendInput(
		__in UINT cInputs,                     // number of input in the array
		__in LPINPUT pInputs,  // array of inputs
		__in int cbSize);
	

	WINUSERAPI
	BOOL
	WINAPI
	UpdateWindow(
		__in HWND hWnd);

	
	WINUSERAPI
	LRESULT
	WINAPI
	SendMessageA(
		__in HWND hWnd,
		__in UINT Msg,
		__in WPARAM wParam,
		__in LPARAM lParam);

	WINUSERAPI
	LRESULT
	WINAPI
	SendMessageW(
		__in HWND hWnd,
		__in UINT Msg,
		__in WPARAM wParam,
		__in LPARAM lParam);

	
	WINBASEAPI
	HANDLE
	WINAPI
	GetStdHandle(
		__in DWORD nStdHandle
		);

	WINBASEAPI
	BOOL
	WINAPI
	SetStdHandle(
		__in DWORD nStdHandle,
		__in HANDLE hHandle
		);

	WINBASEAPI
	BOOL
	WINAPI
	SetStdHandleEx(
		__in      DWORD nStdHandle,
		__in      HANDLE hHandle,
		__out_opt PHANDLE phPrevValue
		);
	

	WINBASEAPI
	LPVOID
	WINAPI
	VirtualAlloc(
		__in_opt LPVOID lpAddress,
		__in     SIZE_T dwSize,
		__in     DWORD flAllocationType,
		__in     DWORD flProtect
		);

	WINBASEAPI
	BOOL
	WINAPI
	VirtualFree(
		__in LPVOID lpAddress,
		__in SIZE_T dwSize,
		__in DWORD dwFreeType
		);


	WINBASEAPI
	DWORD
	WINAPI
	GetTickCount(
		VOID
		);
	
	WINBASEAPI
	ULONGLONG
	WINAPI
	GetTickCount64(
		VOID
		);


	WINBASEAPI
	VOID
	WINAPI
	InitializeSRWLock (
		 __out PSRWLOCK SRWLock
		 );

	WINBASEAPI
	VOID
	WINAPI
	ReleaseSRWLockExclusive (
		 __inout PSRWLOCK SRWLock
		 );

	WINBASEAPI
	VOID
	WINAPI
	ReleaseSRWLockShared (
		 __inout PSRWLOCK SRWLock
		 );

	WINBASEAPI
	VOID
	WINAPI
	AcquireSRWLockExclusive (
		 __inout PSRWLOCK SRWLock
		 );

	WINBASEAPI
	VOID
	WINAPI
	AcquireSRWLockShared (
		 __inout PSRWLOCK SRWLock
		 );

	WINBASEAPI
	BOOLEAN
	WINAPI
	TryAcquireSRWLockExclusive (
		__inout PSRWLOCK SRWLock
		);

	WINBASEAPI
	BOOLEAN
	WINAPI
	TryAcquireSRWLockShared (
		__inout PSRWLOCK SRWLock
		);



	WINBASEAPI
	VOID
	WINAPI
	InitializeConditionVariable (
		__out PCONDITION_VARIABLE ConditionVariable
		);

	WINBASEAPI
	VOID
	WINAPI
	WakeConditionVariable (
		__inout PCONDITION_VARIABLE ConditionVariable
		);

	WINBASEAPI
	VOID
	WINAPI
	WakeAllConditionVariable (
		__inout PCONDITION_VARIABLE ConditionVariable
		);

	WINBASEAPI
	BOOL
	WINAPI
	SleepConditionVariableCS (
		__inout PCONDITION_VARIABLE ConditionVariable,
		__inout PCRITICAL_SECTION CriticalSection,
		__in DWORD dwMilliseconds
		);

	WINBASEAPI
	BOOL
	WINAPI
	SleepConditionVariableSRW (
		__inout PCONDITION_VARIABLE ConditionVariable,
		__inout PSRWLOCK SRWLock,
		__in DWORD dwMilliseconds,
		__in ULONG Flags
		);
	
	WINBASEAPI
	BOOL
	WINAPI
	GetFileTime(
		__in      HANDLE hFile,
		__out_opt LPFILETIME lpCreationTime,
		__out_opt LPFILETIME lpLastAccessTime,
		__out_opt LPFILETIME lpLastWriteTime
		);

	WINBASEAPI
	BOOL
	WINAPI
	SetFileTime(
		__in     HANDLE hFile,
		__in_opt CONST FILETIME *lpCreationTime,
		__in_opt CONST FILETIME *lpLastAccessTime,
		__in_opt CONST FILETIME *lpLastWriteTime
		);
	
	WINBASEAPI
	BOOL
	WINAPI
	LocalFileTimeToFileTime(
		__in  CONST FILETIME *lpLocalFileTime,
		__out LPFILETIME lpFileTime
		);

	WINBASEAPI
	BOOL
	WINAPI
	DosDateTimeToFileTime(
		__in  WORD wFatDate,
		__in  WORD wFatTime,
		__out LPFILETIME lpFileTime
		);
	
	WINBASEAPI
	DWORD
	WINAPI
	GetLastError(
		VOID
		);
	
	WINADVAPI
	BOOL
	WINAPI
	CryptReleaseContext(
		__in    HCRYPTPROV  hProv,
		__in    DWORD       dwFlags
		);
	
	WINADVAPI
	BOOL
	WINAPI
	CryptGenRandom(
		__in      HCRYPTPROV  hProv,
		__in      DWORD   dwLen,
		__inout   BYTE    *pbBuffer
		);
	
	WINBASEAPI
	BOOL
	WINAPI
	GetFileSizeEx(
		__in  HANDLE hFile,
		__out PLARGE_INTEGER lpFileSize
		);

	WINGDIAPI int   WINAPI ChoosePixelFormat( __in HDC hdc, __in CONST PIXELFORMATDESCRIPTOR *ppfd);
	WINGDIAPI int  WINAPI DescribePixelFormat(  __in HDC hdc,
												__in int iPixelFormat,
												__in UINT nBytes,
												__out_opt LPPIXELFORMATDESCRIPTOR ppfd);
	WINGDIAPI BOOL  WINAPI SetPixelFormat(__in HDC hdc, __in int format, __in CONST PIXELFORMATDESCRIPTOR * ppfd);

#if 1 
	WINGDIAPI BOOL  WINAPI wglCopyContext(HGLRC, HGLRC, UINT);
	WINGDIAPI HGLRC WINAPI wglCreateContext(HDC);
	WINGDIAPI HGLRC WINAPI wglCreateLayerContext(HDC, int);
	WINGDIAPI BOOL  WINAPI wglDeleteContext(HGLRC);
	WINGDIAPI HGLRC WINAPI wglGetCurrentContext(VOID);
	WINGDIAPI HDC   WINAPI wglGetCurrentDC(VOID);
	WINGDIAPI PROC  WINAPI wglGetProcAddress(LPCSTR);
	WINGDIAPI BOOL  WINAPI wglMakeCurrent(HDC, HGLRC);
	WINGDIAPI BOOL  WINAPI wglShareLists(HGLRC, HGLRC);
	WINGDIAPI BOOL  WINAPI SwapBuffers(HDC);
	WINGDIAPI BOOL  WINAPI wglDescribeLayerPlane(HDC, int, int, UINT, LPLAYERPLANEDESCRIPTOR);
	WINGDIAPI BOOL  WINAPI wglRealizeLayerPalette(HDC, int, BOOL);
	WINGDIAPI BOOL  WINAPI wglSwapLayerBuffers(HDC, UINT);
	WINGDIAPI DWORD WINAPI wglSwapMultipleBuffers(UINT, CONST WGLSWAP *);
#endif

}	// extern "C"


	// override for ANSI and UNICODE versions
	inline
	int
	WINAPI
	MessageBox(
		__in_opt HWND hWnd,
		__in_opt LPCSTR lpText,
		__in_opt LPCSTR lpCaption,
		__in UINT uType)
	{
		return _winapi_hidden_::MessageBoxA( hWnd, lpText, lpCaption, uType );
	}

	inline
	int
	WINAPI
	MessageBox(
		__in_opt HWND hWnd,
		__in_opt LPCWSTR lpText,
		__in_opt LPCWSTR lpCaption,
		__in UINT uType)
	{
		return _winapi_hidden_::MessageBoxW( hWnd, lpText, lpCaption, uType );
	}


	inline
	HMODULE
	WINAPI
	LoadLibrary(
		__in LPCSTR lpLibFileName
		)
	{
		return _winapi_hidden_::LoadLibraryA( lpLibFileName );
	}

	inline
	HMODULE
	WINAPI
	LoadLibrary(
		__in LPCWSTR lpLibFileName
		)
	{
		return _winapi_hidden_::LoadLibraryW( lpLibFileName );
	}
	


	inline
	HANDLE
	WINAPI
	CreateMutex(
		__in_opt LPSECURITY_ATTRIBUTES lpMutexAttributes,
		__in     BOOL bInitialOwner,
		__in_opt LPCSTR lpName
		)
	{
		return _winapi_hidden_::CreateMutexA( lpMutexAttributes, bInitialOwner, lpName );
	}

	inline
	HANDLE
	WINAPI
	CreateMutex(
		__in_opt LPSECURITY_ATTRIBUTES lpMutexAttributes,
		__in     BOOL bInitialOwner,
		__in_opt LPCWSTR lpName
		)
	{
		return _winapi_hidden_::CreateMutexW( lpMutexAttributes, bInitialOwner, lpName );
	}


	inline
	HANDLE
	WINAPI
	CreateEvent(
		__in_opt LPSECURITY_ATTRIBUTES lpEventAttributes,
		__in     BOOL bManualReset,
		__in     BOOL bInitialState,
		__in_opt LPCSTR lpName
		)
	{
		return _winapi_hidden_::CreateEventA( lpEventAttributes, bManualReset, bInitialState, lpName );
	}

	inline
	HANDLE
	WINAPI
	CreateEvent(
		__in_opt LPSECURITY_ATTRIBUTES lpEventAttributes,
		__in     BOOL bManualReset,
		__in     BOOL bInitialState,
		__in_opt LPCWSTR lpName
		)
	{
		return _winapi_hidden_::CreateEventW( lpEventAttributes, bManualReset, bInitialState, lpName );
	}
	

	inline
	HANDLE
	WINAPI
	CreateSemaphore(
		__in_opt LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
		__in     LONG lInitialCount,
		__in     LONG lMaximumCount,
		__in_opt LPCSTR lpName
		)
	{
		return _winapi_hidden_::CreateSemaphoreA( lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName );
	}

	inline
	HANDLE
	WINAPI
	CreateSemaphore(
		__in_opt LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
		__in     LONG lInitialCount,
		__in     LONG lMaximumCount,
		__in_opt LPCWSTR lpName
		)
	{
		return _winapi_hidden_::CreateSemaphoreW( lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName );
	}
	

	inline
	LONGLONG
	InterlockedAnd64 (
		__inout LONGLONG volatile *Destination,
		__in    LONGLONG Value
		)
	{
		LONGLONG Old;

		do {
			Old = *Destination;
		} while (InterlockedCompareExchange64(Destination,
											  Old & Value,
											  Old) != Old);

		return Old;
	}

	inline
	LONGLONG
	InterlockedOr64 (
		__inout LONGLONG volatile *Destination,
		__in    LONGLONG Value
		)
	{
		LONGLONG Old;

		do {
			Old = *Destination;
		} while (InterlockedCompareExchange64(Destination,
											  Old | Value,
											  Old) != Old);

		return Old;
	}

	inline
	LONGLONG
	InterlockedXor64 (
		__inout LONGLONG volatile *Destination,
		__in    LONGLONG Value
		)
	{
		LONGLONG Old;

		do {
			Old = *Destination;
		} while (InterlockedCompareExchange64(Destination,
											  Old ^ Value,
											  Old) != Old);

		return Old;
	}

	inline
	LONGLONG
	InterlockedIncrement64 (
		__inout LONGLONG volatile *Addend
		)
	{
		LONGLONG Old;

		do {
			Old = *Addend;
		} while (InterlockedCompareExchange64(Addend,
											  Old + 1,
											  Old) != Old);

		return Old + 1;
	}

	inline
	LONGLONG
	InterlockedDecrement64 (
		__inout LONGLONG volatile *Addend
		)
	{
		LONGLONG Old;

		do {
			Old = *Addend;
		} while (InterlockedCompareExchange64(Addend,
											  Old - 1,
											  Old) != Old);

		return Old - 1;
	}

	inline
	LONGLONG
	InterlockedExchange64 (
		__inout LONGLONG volatile *Target,
		__in    LONGLONG Value
		)
	{
		LONGLONG Old;

		do {
			Old = *Target;
		} while (InterlockedCompareExchange64(Target,
											  Value,
											  Old) != Old);

		return Old;
	}

	inline
	LONGLONG
	InterlockedExchangeAdd64(
		__inout LONGLONG volatile *Addend,
		__in    LONGLONG Value
		)
	{
		LONGLONG Old;

		do {
			Old = *Addend;
		} while (InterlockedCompareExchange64(Addend,
											  Old + Value,
											  Old) != Old);

		return Old;
	}


	inline
	HRSRC
	WINAPI
	FindResource(
		__in_opt HMODULE hModule,
		__in     LPCSTR lpName,
		__in     LPCSTR lpType
		)
	{
		return _winapi_hidden_::FindResourceA( hModule, lpName, lpType );
	}

	inline
	HRSRC
	WINAPI
	FindResource(
		__in_opt HMODULE hModule,
		__in     LPCWSTR lpName,
		__in     LPCWSTR lpType
		)
	{
		return _winapi_hidden_::FindResourceW( hModule, lpName, lpType );
	}

	
	inline
	ULONG_PTR
	IS_INTRESOURCE(__in HGLOBAL _r)
	{
		return (((ULONG_PTR)(_r)) >> 16) == 0;
	}

	inline
	LPSTR
	MAKEINTRESOURCEA(int i)
	{
		return LPSTR( ULONG_PTR( WORD(i) ) );
	}

	inline
	LPWSTR
	MAKEINTRESOURCEW(int i)
	{
		return LPWSTR( ULONG_PTR( WORD(i) ) );
	}
	

	inline
	DWORD
	WINAPI
	GetFileAttributes(
		__in LPCSTR lpFileName
		)
	{
		return _winapi_hidden_::GetFileAttributesA( lpFileName );
	}

	inline
	DWORD
	WINAPI
	GetFileAttributes(
		__in LPCWSTR lpFileName
		)
	{
		return _winapi_hidden_::GetFileAttributesW( lpFileName );
	}


	inline
	BOOL
	WINAPI
	SetCurrentDirectory(
		__in LPCSTR lpPathName
		)
	{
		return _winapi_hidden_::SetCurrentDirectoryA( lpPathName );
	}

	inline
	BOOL
	WINAPI
	SetCurrentDirectory(
		__in LPCWSTR lpPathName
		)
	{
		return _winapi_hidden_::SetCurrentDirectoryW( lpPathName );
	}

	inline
	DWORD
	WINAPI
	GetCurrentDirectory(
		__in DWORD nBufferLength,
		__out LPSTR lpBuffer
		)
	{
		return _winapi_hidden_::GetCurrentDirectoryA( nBufferLength, lpBuffer );
	}

	inline
	DWORD
	WINAPI
	GetCurrentDirectory(
		__in DWORD nBufferLength,
		__out LPWSTR lpBuffer
		)
	{
		return _winapi_hidden_::GetCurrentDirectoryW( nBufferLength, lpBuffer );
	}

	inline
	BOOL
	WINAPI
	CreateDirectory(
		__in     LPCSTR lpPathName,
		__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes
		)
	{
		return _winapi_hidden_::CreateDirectoryA( lpPathName, lpSecurityAttributes );
	}

	inline
	BOOL
	WINAPI
	CreateDirectory(
		__in     LPCWSTR lpPathName,
		__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes
		)
	{
		return _winapi_hidden_::CreateDirectoryW( lpPathName, lpSecurityAttributes );
	}

	
	inline
	HANDLE
	WINAPI
	FindFirstFile(
		__in  LPCSTR lpFileName,
		__out LPWIN32_FIND_DATAA lpFindFileData
		)
	{
		return _winapi_hidden_::FindFirstFileA( lpFileName, lpFindFileData );
	}

	inline
	HANDLE
	WINAPI
	FindFirstFile(
		__in  LPCWSTR lpFileName,
		__out LPWIN32_FIND_DATAW lpFindFileData
		)
	{
		return _winapi_hidden_::FindFirstFileW( lpFileName, lpFindFileData );
	}

	inline
	BOOL
	WINAPI
	FindNextFile(
		__in  HANDLE hFindFile,
		__out LPWIN32_FIND_DATAA lpFindFileData
		)
	{
		return _winapi_hidden_::FindNextFileA( hFindFile, lpFindFileData );
	}

	inline
	BOOL
	WINAPI
	FindNextFile(
		__in  HANDLE hFindFile,
		__out LPWIN32_FIND_DATAW lpFindFileData
		)
	{
		return _winapi_hidden_::FindNextFileW( hFindFile, lpFindFileData );
	}


	
	inline
	BOOL
	WINAPI
	GetClassInfo(
		__in_opt HINSTANCE hInstance,
		__in LPCSTR lpClassName,
		__out LPWNDCLASSA lpWndClass)
	{
		return _winapi_hidden_::GetClassInfoA( hInstance, lpClassName, lpWndClass );
	}

	inline
	BOOL
	WINAPI
	GetClassInfo(
		__in_opt HINSTANCE hInstance,
		__in LPCWSTR lpClassName,
		__out LPWNDCLASSW lpWndClass)
	{
		return _winapi_hidden_::GetClassInfoW( hInstance, lpClassName, lpWndClass );
	}

	
	inline
	HCURSOR
	WINAPI
	LoadCursor(
		__in_opt HINSTANCE hInstance,
		__in LPCSTR lpCursorName)
	{
		return _winapi_hidden_::LoadCursorA( hInstance, lpCursorName );
	}

	inline
	HCURSOR
	WINAPI
	LoadCursor(
		__in_opt HINSTANCE hInstance,
		__in LPCWSTR lpCursorName)
	{
		return _winapi_hidden_::LoadCursorW( hInstance, lpCursorName );
	}


	inline
	ATOM
	WINAPI
	RegisterClassEx(
		__in CONST WNDCLASSEXA *pWC)
	{
		return _winapi_hidden_::RegisterClassExA( pWC );
	}

	inline
	ATOM
	WINAPI
	RegisterClassEx(
		__in CONST WNDCLASSEXW *pWC)
	{
		return _winapi_hidden_::RegisterClassExW( pWC );
	}

	
	inline
	BOOL
	WINAPI
	SetWindowText(
		__in HWND hWnd,
		__in_opt LPCSTR lpString)
	{
		return _winapi_hidden_::SetWindowTextA( hWnd, lpString );
	}

	inline
	BOOL
	WINAPI
	SetWindowText(
		__in HWND hWnd,
		__in_opt LPCWSTR lpString)
	{
		return _winapi_hidden_::SetWindowTextW( hWnd, lpString );
	}
	
	inline
	int
	WINAPI
	GetWindowText(
		__in HWND hWnd,
		__out LPSTR lpString,
		__in int nMaxCount)
	{
		return _winapi_hidden_::GetWindowTextA( hWnd, lpString, nMaxCount );
	}

	inline
	int
	WINAPI
	GetWindowText(
		__in HWND hWnd,
		__out LPWSTR lpString,
		__in int nMaxCount)
	{
		return _winapi_hidden_::GetWindowTextW( hWnd, lpString, nMaxCount );
	}

	
	inline
	HWND
	WINAPI
	CreateWindowEx(
		__in DWORD dwExStyle,
		__in_opt LPCSTR lpClassName,
		__in_opt LPCSTR lpWindowName,
		__in DWORD dwStyle,
		__in int X,
		__in int Y,
		__in int nWidth,
		__in int nHeight,
		__in_opt HWND hWndParent,
		__in_opt HMENU hMenu,
		__in_opt HINSTANCE hInstance,
		__in_opt LPVOID lpParam)
	{
		return _winapi_hidden_::CreateWindowExA( dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y,
												 nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam );
	}

	inline
	HWND
	WINAPI
	CreateWindowEx(
		__in DWORD dwExStyle,
		__in_opt LPCWSTR lpClassName,
		__in_opt LPCWSTR lpWindowName,
		__in DWORD dwStyle,
		__in int X,
		__in int Y,
		__in int nWidth,
		__in int nHeight,
		__in_opt HWND hWndParent,
		__in_opt HMENU hMenu,
		__in_opt HINSTANCE hInstance,
		__in_opt LPVOID lpParam)
	{
		return _winapi_hidden_::CreateWindowExW( dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y,
												 nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam );
	}

	inline
	HWND
	WINAPI
	CreateWindow(
		__in_opt LPCSTR lpClassName,
		__in_opt LPCSTR lpWindowName,
		__in DWORD dwStyle,
		__in int X,
		__in int Y,
		__in int nWidth,
		__in int nHeight,
		__in_opt HWND hWndParent,
		__in_opt HMENU hMenu,
		__in_opt HINSTANCE hInstance,
		__in_opt LPVOID lpParam)
	{
		return _winapi_hidden_::CreateWindowExA( 0L, lpClassName, lpWindowName, dwStyle, X, Y,
												 nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam );
	}

	inline
	HWND
	WINAPI
	CreateWindow(
		__in_opt LPCWSTR lpClassName,
		__in_opt LPCWSTR lpWindowName,
		__in DWORD dwStyle,
		__in int X,
		__in int Y,
		__in int nWidth,
		__in int nHeight,
		__in_opt HWND hWndParent,
		__in_opt HMENU hMenu,
		__in_opt HINSTANCE hInstance,
		__in_opt LPVOID lpParam)
	{
		return _winapi_hidden_::CreateWindowExW( 0L, lpClassName, lpWindowName, dwStyle, X, Y,
												 nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam );
	}

	
	inline
	LONG
	WINAPI
	ChangeDisplaySettings(
		__in_opt DEVMODEA* lpDevMode,
		__in DWORD dwFlags)
	{
		return _winapi_hidden_::ChangeDisplaySettingsA( lpDevMode, dwFlags );
	}

	inline
	LONG
	WINAPI
	ChangeDisplaySettings(
		__in_opt DEVMODEW* lpDevMode,
		__in DWORD dwFlags)
	{
		return _winapi_hidden_::ChangeDisplaySettingsW( lpDevMode, dwFlags );
	}
	

	inline
	BOOL
	WINAPI
	UnregisterClass(
		__in LPCSTR lpClassName,
		__in_opt HINSTANCE hInstance)
	{
		return _winapi_hidden_::UnregisterClassA( lpClassName, hInstance );
	}

	inline
	BOOL
	WINAPI
	UnregisterClassW(
		__in LPCWSTR lpClassName,
		__in_opt HINSTANCE hInstance)
	{
		return _winapi_hidden_::UnregisterClassW( lpClassName, hInstance );
	}
	

	inline
	__out_opt
	HMODULE
	WINAPI
	GetModuleHandle(
		__in_opt LPCSTR lpModuleName
		)
	{
		return _winapi_hidden_::GetModuleHandleA( lpModuleName );
	}


	inline
	__out_opt
	HMODULE
	WINAPI
	GetModuleHandle(
		__in_opt LPCWSTR lpModuleName
		)
	{
		return _winapi_hidden_::GetModuleHandleW( lpModuleName );
	}


	inline
	BOOL
	WINAPI
	EnumDisplaySettings(
		__in_opt LPCSTR lpszDeviceName,
		__in DWORD iModeNum,
		__inout DEVMODEA* lpDevMode)
	{
		return _winapi_hidden_::EnumDisplaySettingsA( lpszDeviceName, iModeNum, lpDevMode );
	}

	inline
	BOOL
	WINAPI
	EnumDisplaySettings(
		__in_opt LPCWSTR lpszDeviceName,
		__in DWORD iModeNum,
		__inout DEVMODEW* lpDevMode)
	{
		return _winapi_hidden_::EnumDisplaySettingsW( lpszDeviceName, iModeNum, lpDevMode );
	}

	inline
	BOOL
	WINAPI
	EnumDisplayDevices(
		__in_opt LPCSTR lpDevice,
		__in DWORD iDevNum,
		__inout PDISPLAY_DEVICEA lpDisplayDevice,
		__in DWORD dwFlags)
	{
		return _winapi_hidden_::EnumDisplayDevicesA( lpDevice, iDevNum, lpDisplayDevice, dwFlags );
	}

	inline
	BOOL
	WINAPI
	EnumDisplayDevices(
		__in_opt LPCWSTR lpDevice,
		__in DWORD iDevNum,
		__inout PDISPLAY_DEVICEW lpDisplayDevice,
		__in DWORD dwFlags)
	{
		return _winapi_hidden_::EnumDisplayDevicesW( lpDevice, iDevNum, lpDisplayDevice, dwFlags );
	}

	
	inline
	BOOL
	WINAPI
	GetVersionEx(
		__inout LPOSVERSIONINFOA lpVersionInformation
		)
	{
		return _winapi_hidden_::GetVersionExA( lpVersionInformation );
	}

	inline
	BOOL
	WINAPI
	GetVersionEx(
		__inout LPOSVERSIONINFOW lpVersionInformation
		)
	{
		return _winapi_hidden_::GetVersionExW( lpVersionInformation );
	}


	inline
	LSTATUS
	APIENTRY
	RegOpenKeyEx (
		__in HKEY hKey,
		__in_opt LPCSTR lpSubKey,
		__in_opt DWORD ulOptions,
		__in REGSAM samDesired,
		__out PHKEY phkResult
		)
	{
		return _winapi_hidden_::RegOpenKeyExA( hKey, lpSubKey, ulOptions, samDesired, phkResult );
	}

	inline
	LSTATUS
	APIENTRY
	RegOpenKeyEx (
		__in HKEY hKey,
		__in_opt LPCWSTR lpSubKey,
		__in_opt DWORD ulOptions,
		__in REGSAM samDesired,
		__out PHKEY phkResult
		)
	{
		return _winapi_hidden_::RegOpenKeyExW( hKey, lpSubKey, ulOptions, samDesired, phkResult );
	}
		
	inline
	LSTATUS
	APIENTRY
	RegQueryValueEx (
		__in HKEY hKey,
		__in_opt LPCSTR lpValueName,
		__reserved LPDWORD lpReserved,
		__out_opt LPDWORD lpType,
		__out_opt LPBYTE lpData,
		__inout_opt LPDWORD lpcbData
		)
	{
		return _winapi_hidden_::RegQueryValueExA( hKey, lpValueName, lpReserved, lpType, lpData, lpcbData );
	}

	inline
	LSTATUS
	APIENTRY
	RegQueryValueExW (
		__in HKEY hKey,
		__in_opt LPCWSTR lpValueName,
		__reserved LPDWORD lpReserved,
		__out_opt LPDWORD lpType,
		__out_opt LPBYTE lpData,
		__inout_opt LPDWORD lpcbData
		)
	{
		return _winapi_hidden_::RegQueryValueExW( hKey, lpValueName, lpReserved, lpType, lpData, lpcbData );
	}


	inline
	HICON
	WINAPI
	LoadIcon(
		__in_opt HINSTANCE hInstance,
		__in LPCSTR lpIconName)
	{
		return _winapi_hidden_::LoadIconA( hInstance, lpIconName );
	}

	inline
	HICON
	WINAPI
	LoadIcon(
		__in_opt HINSTANCE hInstance,
		__in LPCWSTR lpIconName)
	{
		return _winapi_hidden_::LoadIconW( hInstance, lpIconName );
	}
	

	inline
	HBITMAP
	WINAPI
	LoadBitmap(
		__in_opt HINSTANCE hInstance,
		__in LPCSTR lpBitmapName)
	{
		return _winapi_hidden_::LoadBitmapA( hInstance, lpBitmapName );
	}

	inline
	HBITMAP
	WINAPI
	LoadBitmap(
		__in_opt HINSTANCE hInstance,
		__in LPCWSTR lpBitmapName)
	{
		return _winapi_hidden_::LoadBitmapW( hInstance, lpBitmapName );
	}
	

	inline
	VOID
	WINAPI
	OutputDebugString(
		__in_opt LPCSTR lpOutputString
		)
	{
		return _winapi_hidden_::OutputDebugStringA( lpOutputString );
	}

	inline
	VOID
	WINAPI
	OutputDebugString(
		__in_opt LPCWSTR lpOutputString
		)
	{
		return _winapi_hidden_::OutputDebugStringW( lpOutputString );
	}
	

	inline
	DWORD
	WINAPI
	GetModuleFileName(
		__in_opt HMODULE hModule,
		__out LPSTR lpFilename,
		__in     DWORD nSize
		)
	{
		return _winapi_hidden_::GetModuleFileNameA( hModule, lpFilename, nSize );
	}

	inline
	DWORD
	WINAPI
	GetModuleFileName(
		__in_opt HMODULE hModule,
		__out LPWSTR lpFilename,
		__in     DWORD nSize
		)
	{
		return _winapi_hidden_::GetModuleFileNameW( hModule, lpFilename, nSize );
	}


	inline
	UINT
	WINAPI
	GetSystemDirectory(
		__out_opt LPSTR lpBuffer,
		__in UINT uSize
		)
	{
		return _winapi_hidden_::GetSystemDirectoryA( lpBuffer, uSize );
	}

	inline
	UINT
	WINAPI
	GetSystemDirectory(
		__out_opt LPWSTR lpBuffer,
		__in UINT uSize
		)
	{
		return _winapi_hidden_::GetSystemDirectoryW( lpBuffer, uSize );
	}
	

	inline
	BOOL
	WINAPI
	DeleteFile(
		__in LPCSTR lpFileName
		)
	{
		return _winapi_hidden_::DeleteFileA( lpFileName );
	}

	inline
	BOOL
	WINAPI
	DeleteFile(
		__in LPCWSTR lpFileName
		)
	{
		return _winapi_hidden_::DeleteFileW( lpFileName );
	}

	
	inline
	BOOL
	WINAPI
	RemoveDirectory(
		__in LPCSTR lpPathName
		)
	{
		return _winapi_hidden_::RemoveDirectoryA( lpPathName );
	}
	
	inline
	BOOL
	WINAPI
	RemoveDirectory(
		__in LPCWSTR lpPathName
		)
	{
		return _winapi_hidden_::RemoveDirectoryW( lpPathName );
	}

	inline
	HANDLE
	WINAPI
	CreateFile(
		__in     LPCSTR lpFileName,
		__in     DWORD dwDesiredAccess,
		__in     DWORD dwShareMode,
		__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		__in     DWORD dwCreationDisposition,
		__in     DWORD dwFlagsAndAttributes,
		__in_opt HANDLE hTemplateFile
		)
	{
		return _winapi_hidden_::CreateFileA( lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
											 dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );
	}

	inline
	HANDLE
	WINAPI
	CreateFile(
		__in     LPCWSTR lpFileName,
		__in     DWORD dwDesiredAccess,
		__in     DWORD dwShareMode,
		__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		__in     DWORD dwCreationDisposition,
		__in     DWORD dwFlagsAndAttributes,
		__in_opt HANDLE hTemplateFile
		)
	{
		return _winapi_hidden_::CreateFileW( lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
											 dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );
	}
	
	inline int SHFileOperation(__inout LPSHFILEOPSTRUCTA lpFileOp)	{ return _winapi_hidden_::SHFileOperationA( lpFileOp ); }
	
	inline int SHFileOperation(__inout LPSHFILEOPSTRUCTW lpFileOp)	{ return _winapi_hidden_::SHFileOperationW( lpFileOp ); }
	
	inline
	BOOL
	WINAPI
	CopyFile(
		__in LPCSTR lpExistingFileName,
		__in LPCSTR lpNewFileName,
		__in BOOL bFailIfExists
		)
	{
		return _winapi_hidden_::CopyFileA( lpExistingFileName, lpNewFileName, bFailIfExists );
	}

	inline
	BOOL
	WINAPI
	CopyFile(
		__in LPCWSTR lpExistingFileName,
		__in LPCWSTR lpNewFileName,
		__in BOOL bFailIfExists
		)
	{
		return _winapi_hidden_::CopyFileW( lpExistingFileName, lpNewFileName, bFailIfExists );
	}
		
	inline
	BOOL
	WINAPI
	MoveFileEx(
		_In_     LPCSTR lpExistingFileName,
		_In_opt_ LPCSTR lpNewFileName,
		_In_     DWORD    dwFlags
		)
	{
		return _winapi_hidden_::MoveFileExA( lpExistingFileName, lpNewFileName, dwFlags );
	}

	inline
	BOOL
	WINAPI
	MoveFileEx(
		_In_     LPCWSTR lpExistingFileName,
		_In_opt_ LPCWSTR lpNewFileName,
		_In_     DWORD    dwFlags
		)
	{
		return _winapi_hidden_::MoveFileExW( lpExistingFileName, lpNewFileName, dwFlags );
	}

	inline
	BOOL
	WINAPI
	CryptEnumProviders(
		__in       DWORD   dwIndex,
		__reserved DWORD   *pdwReserved,
		__in       DWORD   dwFlags,
		__out      DWORD   *pdwProvType,
		__out      LPSTR   szProvName,
		__inout    DWORD   *pcbProvName
		)
	{
		return _winapi_hidden_::CryptEnumProvidersA( dwIndex, pdwReserved, dwFlags, pdwProvType, szProvName, pcbProvName );
	}

	inline
	BOOL
	WINAPI
	CryptEnumProviders(
		__in        DWORD   dwIndex,
		__reserved  DWORD   *pdwReserved,
		__in        DWORD   dwFlags,
		__out       DWORD   *pdwProvType,
		__out       LPWSTR szProvName,
		__inout     DWORD   *pcbProvName
		)
	{
		return _winapi_hidden_::CryptEnumProvidersW( dwIndex, pdwReserved, dwFlags, pdwProvType, szProvName, pcbProvName );
	}
		
	inline
	BOOL
	WINAPI
	CryptAcquireContext(
		__out       HCRYPTPROV  *phProv,
		__in_opt    LPCSTR    szContainer,
		__in_opt    LPCSTR    szProvider,
		__in        DWORD       dwProvType,
		__in        DWORD       dwFlags
		)
	{
		return _winapi_hidden_::CryptAcquireContextA( phProv, szContainer, szProvider, dwProvType, dwFlags );
	}

	inline
	BOOL
	WINAPI
	CryptAcquireContext(
		__out       HCRYPTPROV  *phProv,
		__in_opt    LPCWSTR    szContainer,
		__in_opt    LPCWSTR    szProvider,
		__in        DWORD       dwProvType,
		__in        DWORD       dwFlags
		)
	{
		return _winapi_hidden_::CryptAcquireContextW( phProv, szContainer, szProvider, dwProvType, dwFlags );
	}

	
	inline
	HINSTANCE ShellExecute(__in_opt HWND hwnd, __in_opt LPCSTR lpOperation, __in LPCSTR lpFile, __in_opt LPCSTR lpParameters,
		__in_opt LPCSTR lpDirectory, __in INT nShowCmd)
	{
		return _winapi_hidden_::ShellExecuteA( hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd );
	}

	inline
	HINSTANCE ShellExecute(__in_opt HWND hwnd, __in_opt LPCWSTR lpOperation, __in LPCWSTR lpFile, __in_opt LPCWSTR lpParameters,
		__in_opt LPCWSTR lpDirectory, __in INT nShowCmd)
	{
		return _winapi_hidden_::ShellExecuteW( hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd );
	}


	template <typename A, typename B>
	inline WORD MAKEWORD(A a, B b)	{ return (WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8); }
	
	template <typename A, typename B>
	inline LONG MAKELONG(A a, B b)	{ return (LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16); }
	
	template <typename T>
	inline WORD LOWORD(T l)			{ return (WORD)(((DWORD_PTR)(l)) & 0xffff); }

	template <typename T>
	inline WORD HIWORD(T l)			{ return (WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff); }

	template <typename T>
	inline BYTE LOBYTE(T w)			{ return (BYTE)(((DWORD_PTR)(w)) & 0xff); }

	template <typename T>
	inline BYTE HIBYTE(T w)			{ return (BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff); }

	
	inline bool SUCCEEDED(HRESULT hr)	{ return (hr >= 0); }
	inline bool FAILED(HRESULT hr)		{ return (hr < 0); }



#	undef __in_opt
#	undef __in
#	undef __out_opt
#	undef __out
#	undef __inout
#	undef __inout_opt
#	undef __reserved
#	undef __deref_out
#	undef __deref
#	undef CONST
//#	undef WINAPI
#	undef WINBASEAPI
#	undef WINUSERAPI
#	undef WINGDIAPI
#	undef WINADVAPI
#	undef APIENTRY
//#	undef DECLARE_HANDLE
#	undef SHSTDAPI
#	undef SHSTDAPI_

//-------------------------------------------------------------------

}	// winapi
}	// GX_STL

#endif	// PLATFORM_WINDOWS