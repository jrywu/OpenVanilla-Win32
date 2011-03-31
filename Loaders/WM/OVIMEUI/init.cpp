//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//

#include <windows.h>
#include <imm.h>
#include "uipriv.h"

PPOINT pptStatusPos;
CRITICAL_SECTION csStatusPos;

const TCHAR v_szImeUIClassName[]=TEXT("OVChnImeUI");
const TCHAR v_szCompClassName[]=TEXT("ChnCompUI");
const TCHAR v_szCandClassName[]=TEXT("ChnCandUI");
const TCHAR v_szStatusClassName[]=TEXT("ChnStatusUI");

const TCHAR g_szChineseIMEUI[]=TEXT("ControlPanel\\Chinese IME UI\\Settings");
const TCHAR g_szShowState[]=TEXT("ShowState");
const TCHAR g_szAutoCandAccel[]=TEXT("AutoCandAccel");

HINSTANCE g_hInst=NULL;
IMEUIG g_sImeUIG={0};

/**********************************************************************/
/* ImeRegisterUIClass()                                               */
/**********************************************************************/
void ImeRegisterUIClass(HINSTANCE hInstance)
{
    WNDCLASS   wcWndCls;

    wcWndCls.cbClsExtra    = 0;
    wcWndCls.cbWndExtra    = sizeof(LONG) * 2;
    wcWndCls.hIcon         = NULL;
    wcWndCls.hInstance     = hInstance;
    wcWndCls.hCursor       = NULL;
    wcWndCls.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wcWndCls.lpszMenuName  = (LPCTSTR)NULL;
    wcWndCls.style         = CS_IME;

    // IME UI class
    if (!GetClassInfo(hInstance, v_szImeUIClassName, &wcWndCls)) {
        wcWndCls.lpfnWndProc   = ImeUIWndProc;
        wcWndCls.lpszClassName = v_szImeUIClassName;

        RegisterClass(&wcWndCls);
    }

    // IME Comp Class
    if (!GetClassInfo(hInstance, v_szCompClassName, &wcWndCls)) {
        wcWndCls.lpfnWndProc   = CompWndProc;
        wcWndCls.lpszClassName = v_szCompClassName;

        RegisterClass(&wcWndCls);
    }

    // IME Cand class
    if (!GetClassInfo(hInstance, v_szCandClassName, &wcWndCls)) {
        wcWndCls.lpfnWndProc   = CandWndProc;
        wcWndCls.lpszClassName = v_szCandClassName;

        RegisterClass(&wcWndCls);
    }

    if (!GetClassInfo(hInstance, v_szStatusClassName, &wcWndCls)) {
        wcWndCls.lpfnWndProc   = StatusWndProc;
        wcWndCls.lpszClassName = v_szStatusClassName;

        RegisterClass(&wcWndCls);
    }
    return;
}

/**********************************************************************/
/* ImeUnregisterUIClass()                                             */
/**********************************************************************/
void ImeUnregisterUIClass(HINSTANCE hInstance)
{
    WNDCLASS   wcWndCls;

    if (GetClassInfo(hInstance, v_szImeUIClassName, &wcWndCls)) {
        UnregisterClass(v_szImeUIClassName, hInstance);
    }

    if (GetClassInfo(hInstance, v_szCompClassName, &wcWndCls)) {
        UnregisterClass(v_szCompClassName, hInstance);
    }

    if (GetClassInfo(hInstance, v_szCandClassName, &wcWndCls)) {
        UnregisterClass(v_szCandClassName, hInstance);
    }

    if (GetClassInfo(hInstance, v_szStatusClassName, &wcWndCls)) {
        UnregisterClass(v_szStatusClassName, hInstance);
    }
    return;
}


/**********************************************************************/
/* InitUIGlobalData()                                                 */
/**********************************************************************/
void InitUIGlobalData(void)
{
    HDC     hDC;
    HFONT   hSysFont, hOldFont;
    LOGFONT lfFont;
    SIZE    lSize;
    TCHAR   szCandChar[2] = {0x4E2D,0};
    TCHAR   szFraction[6] = {'9','9','/','9','9',0};
    int     i;
/* Read reg. settings
    HKEY    hkey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, g_szChineseIMEUI, 0, 0, &hkey) == ERROR_SUCCESS) {
        DWORD   cb = sizeof(DWORD);
        DWORD   dw = 1;
        RegQueryValueEx(hkey, g_szShowState, NULL, NULL, (LPBYTE)&dw, &cb);
        g_sImeUIG.bNoStatus = ( dw == 0 );
        cb = sizeof(DWORD);
        dw = 0;
        RegQueryValueEx(hkey, g_szAutoCandAccel, NULL, NULL, (LPBYTE)&dw, &cb);
        g_sImeUIG.bAutoCandAccel = ( dw != 0 );
    }
    else {
        g_sImeUIG.bNoStatus = g_sImeUIG.bAutoCandAccel = FALSE;
    }
*/
    g_sImeUIG.cxBorder = GetSystemMetrics(SM_CXBORDER);
    g_sImeUIG.cyBorder = GetSystemMetrics(SM_CYBORDER);

    g_sImeUIG.cxVScroll = GetSystemMetrics(SM_CXVSCROLL);

    g_sImeUIG.crWindow = GetSysColor(COLOR_WINDOW);
    g_sImeUIG.crWindowText = GetSysColor(COLOR_WINDOWTEXT);
    g_sImeUIG.crHighlight = GetSysColor(COLOR_HIGHLIGHT);
    g_sImeUIG.crHighlightText = GetSysColor(COLOR_HIGHLIGHTTEXT);
    g_sImeUIG.crBtnFace = GetSysColor(COLOR_BTNFACE);
    g_sImeUIG.crBtnText = GetSysColor(COLOR_BTNTEXT);

// Get candtext width and height
    hDC = GetDC(NULL);
    GetTextExtentPoint(hDC, szCandChar, lstrlen(szCandChar), &lSize);
    g_sImeUIG.nCandCharWi = lSize.cx;
    g_sImeUIG.nCandLineWi = lSize.cx*2 + CAND_TEXTMARGIN * 2;
    g_sImeUIG.nCandLineHi = lSize.cy + CAND_LINEGAP;

    g_sImeUIG.nCandDx = CAND_TEXTMARGIN;
    for (i=0; i <= MAX_PAGESIZE; i++) {
        g_sImeUIG.nCandDy[i] = g_sImeUIG.nCandLineHi * i + CAND_LINEGAP;
    }

    hSysFont = (HFONT)GetStockObject(SYSTEM_FONT);
    GetObject(hSysFont, sizeof(LOGFONT), &lfFont);

    double scaleY;
    #define SCALEY(argY) ((int) ((argY) * scaleY))

    HDC screen = GetDC(0);
    scaleY = GetDeviceCaps(screen, LOGPIXELSY) / 96.0;
    ReleaseDC(0, screen);

    lfFont.lfWidth = 0;
    lfFont.lfHeight = SCALEY(11);
    lfFont.lfWeight = FW_EXTRALIGHT;

// Get frac text width and height
	g_sImeUIG.hFracFont = CreateFontIndirect(&lfFont);
    hOldFont = (HFONT)SelectObject(hDC, g_sImeUIG.hFracFont);
    GetTextExtentPoint(hDC, szFraction, lstrlen(szFraction), &lSize);
    g_sImeUIG.nCandFracWi = lSize.cx + CAND_LINEGAP * 2;
    g_sImeUIG.nCandFracHi = lSize.cy + CAND_LINEGAP * 2;
    SelectObject(hDC, hOldFont);
    ReleaseDC(NULL, hDC);

    return;
}

/**********************************************************************/
/* DestroyUIGlobalData()                                              */
/**********************************************************************/
void DestroyUIGlobalData(void)
{
    if (g_sImeUIG.hFracFont) {
        DeleteObject(g_sImeUIG.hFracFont);
        g_sImeUIG.hFracFont = NULL;
    }

    return;
}

/**********************************************************************/
/* DllMain()                                                          */
/* Return Value:                                                      */
/*      TRUE - successful                                             */
/*      FALSE - failure                                               */
/**********************************************************************/
BOOL WINAPI DllMain(HANDLE hinstDLL,        // instance handle of this library
                    DWORD  fdwReason,       // reason called
                    LPVOID lpvReserved)     // reserve pointer
{
    static  HANDLE hMapFile1 = NULL;
    BOOL    fInit;
    
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        g_hInst = (HINSTANCE)hinstDLL;

        DisableThreadLibraryCalls((HMODULE)hinstDLL);
        ImeRegisterUIClass((HINSTANCE)hinstDLL);
        InitUIGlobalData();

        InitializeCriticalSection( &csStatusPos);
        EnterCriticalSection(&csStatusPos);
        
        hMapFile1 = CreateFileMapping((HANDLE)-1, (LPSECURITY_ATTRIBUTES) NULL, PAGE_READWRITE, (DWORD) 0, (DWORD) sizeof(POINT), TEXT("ImeStatusPos") );
        pptStatusPos = (PPOINT)MapViewOfFile( hMapFile1, FILE_MAP_WRITE, (DWORD) 0, (DWORD) 0, (DWORD) sizeof(POINT));
        
        fInit = (GetLastError() != ERROR_ALREADY_EXISTS);
        if (fInit)
            pptStatusPos->x = pptStatusPos->y = -1;
            
        LeaveCriticalSection(&csStatusPos);

        break;
    case DLL_PROCESS_DETACH:
        ImeUnregisterUIClass((HINSTANCE)hinstDLL);
        DestroyUIGlobalData();

        EnterCriticalSection(&csStatusPos);

        UnmapViewOfFile( (LPCVOID) pptStatusPos);
        CloseHandle(hMapFile1);

        LeaveCriticalSection(&csStatusPos);

        break;
    default:
        break;
    }

    return (TRUE);
}


//=====================================================================//
//  Exported API                                                       //
//=====================================================================//

/**********************************************************************/
/* ImeGetUIClassName()                                                */
/**********************************************************************/
extern "C"
void CALLBACK ImeGetUIClassName(LPTSTR lpszImeUIClassName)
{
    lstrcpy(lpszImeUIClassName, v_szImeUIClassName);

    return;
}

