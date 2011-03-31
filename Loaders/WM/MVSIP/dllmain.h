
#ifndef __DLLMAIN_H__
#define __DLLMAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#pragma warning(disable:4355)

/////////////////////////////////////////////////////////////////////////////

    
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <olectl.h>
    
#include "macros.h"


extern HINSTANCE g_hInstDll;
extern DWORD g_dwObjectCount;   

// Initialize the DLL, register the classes etc
 

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv);
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv);
STDAPI DllRegisterServer(void);
STDAPI DllUnregisterServer(void);
//Missing API definition...
HWND GetForegroundKeyboardTarget();

#ifdef __cplusplus
}
#endif

/////////////////////////////////////////////////////////////////////////////

#endif /* __DLLMAIN_H__ */