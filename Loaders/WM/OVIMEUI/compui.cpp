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
#define OV_DEBUG

#include <windows.h>
#include <imm.h>
#include <sipapi.h>
#include "uipriv.h"
#ifdef WPC
#include "pimemsg.h"
#endif // WPC

#include "windev.h"
#include "OVutility.h"


typedef bool (WINAPI *PFNSHSipInfo)(UINT,UINT,PVOID,UINT);
bool GetWorkAreaFromPPCShell(LPRECT prect, SIPINFO *pssi)
{
    PFNSHSipInfo   pfnSHSipInfo;
    HMODULE        hMod;
    
//    if (IsAPIReady(SH_SHELL,0))//WAIT_OBJECT_0 != WaitForAPIReady(SH_SHELL, 0))
//        return false;

    hMod = GetModuleHandle(TEXT("aygshell.dll"));
    if (!hMod) 
        return false;
    pfnSHSipInfo = (PFNSHSipInfo) GetProcAddress(hMod, TEXT("SHSipInfo"));
    if (!pfnSHSipInfo)
        return false;

    if ((*pfnSHSipInfo)(SPI_GETSIPINFO, 0, (PVOID) pssi, 0)) {
        RETAILMSG(1,(TEXT("GetWorkAreaFromPPCShell() rcVisibleDesktop = (%d, %d - %d, %d)\n"), 
            pssi->rcVisibleDesktop.left, pssi->rcVisibleDesktop.top, 
            pssi->rcVisibleDesktop.right, pssi->rcVisibleDesktop.bottom));
        RETAILMSG(1,(TEXT("GetWorkAreaFromPPCShell() fdwFlags & SIPF_ON = %d\n"), 
            pssi->fdwFlags & SIPF_ON));
            
        *prect = pssi->rcVisibleDesktop;
        return true;
    }
    return false;
}

/**********************************************************************/
/* GetWorkArea()                                                      */
/**********************************************************************/
void GetWorkArea(LPRECT lprcWorkArea)
{
    SIPINFO  sSipInfo;

    memset(&sSipInfo, 0, sizeof(SIPINFO));
    sSipInfo.cbSize = sizeof(SIPINFO);
    sSipInfo.dwImDataSize = 0;
    sSipInfo.pvImData = NULL;

    if (!GetWorkAreaFromPPCShell(lprcWorkArea, &sSipInfo)) {
        if (SipGetInfo(&sSipInfo)) {
            *lprcWorkArea = sSipInfo.rcVisibleDesktop;
        } else {
            SystemParametersInfo(SPI_GETWORKAREA, 0, lprcWorkArea, 0);
        }
    }
}

/**********************************************************************/
/* SetCompPosition() : set the composition window position            */
/**********************************************************************/
void SetCompPosition(HWND hCompWnd)
{
    LPUIPRIV        lpUIPriv;
    HIMC            hIMC;
    LPINPUTCONTEXT  lpIMC;
    POINT           ptNew;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hCompWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return;
    }

    hIMC = (HIMC)GetWindowLong(hCompWnd, IMMGWL_IMC);
    if (!hIMC) {
        return;
    }

    lpIMC = ImmLockIMC(hIMC);
    if (!lpIMC) {
        return;
    }

    GetWorkArea(&g_sImeUIG.rcWorkArea);

    if (GetCaretPos(&ptNew)){
        ClientToScreen(lpIMC->hWnd, &ptNew);
        if (ptNew.x < g_sImeUIG.rcWorkArea.left) {
            ptNew.x = g_sImeUIG.rcWorkArea.left + CARET_WIDTH;
        } else if (ptNew.x + lpUIPriv->nCompWi > g_sImeUIG.rcWorkArea.right) {
            ptNew.x = g_sImeUIG.rcWorkArea.right - lpUIPriv->nCompWi;
        } else {
            ptNew.x += CARET_WIDTH;
        }
        if (ptNew.y < g_sImeUIG.rcWorkArea.top) {
            ptNew.y = g_sImeUIG.rcWorkArea.top - (g_sImeUIG.cyBorder + COMP_TEXTMARGIN);
        } else if (ptNew.y + lpUIPriv->nCompHi > g_sImeUIG.rcWorkArea.bottom) {
            ptNew.y = g_sImeUIG.rcWorkArea.bottom - lpUIPriv->nCompHi;
        } else {
            ptNew.y -= g_sImeUIG.cyBorder + COMP_TEXTMARGIN;
        }
    } else {
        ptNew.x = g_sImeUIG.rcWorkArea.right - lpUIPriv->nCompWi;
        ptNew.y = g_sImeUIG.rcWorkArea.bottom - lpUIPriv->nCompHi;
    }
    
    if (lpUIPriv->ptComp.x != ptNew.x ||
        lpUIPriv->ptComp.y != ptNew.y) {
        lpUIPriv->ptComp = ptNew;
        SetWindowPos(hCompWnd, NULL, ptNew.x, ptNew.y, 0, 0,
                     SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER);
    }

    ImmUnlockIMC(hIMC);

    return;
}

/**********************************************************************/
/* PaintCompWindow()                                                  */
/**********************************************************************/
void PaintCompWindow(HWND hUIWnd, HDC hDC)
{
    LPCUIPRIV            lpUIPriv;
    HIMC                 hIMC;
    CONST INPUTCONTEXT * lpIMC;
    LPCOMPOSITIONSTRING  lpCompStr;
    LPCTSTR              lpszCompStr;
    DWORD                dwCompStrLen;

    lpUIPriv = (LPCUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return;
    }

    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
    if (!hIMC) {
        return;
    }

    lpIMC = (CONST INPUTCONTEXT *)ImmLockIMC(hIMC);
    if (!lpIMC) {
        return;
    }

    lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
    if (!lpCompStr) {
        ImmUnlockIMC(hIMC);
        return;
    }

#ifdef WPC
    UINT uData;

    uData = IME_ESC_GET_COMPSTR_LEN;
    if (ImmEscape(NULL, hIMC, IME_ESC_QUERY_SUPPORT, (LPVOID)&uData)) {
        dwCompStrLen = ImmEscape(NULL, hIMC, IME_ESC_GET_COMPSTR_LEN, NULL);
    } else {
        dwCompStrLen = 0;
    }
#else
    dwCompStrLen = lpCompStr->dwCompStrLen;
#endif // WPC

    if (0 == dwCompStrLen) {
        goto PaintCompUnlockIMCC;
    }

    lpszCompStr = (LPCTSTR)((LPBYTE)lpCompStr + lpCompStr->dwCompStrOffset);

    SetTextColor(hDC, g_sImeUIG.crWindowText);
    SetBkColor(hDC, g_sImeUIG.crWindow);

    ExtTextOut(hDC, COMP_TEXTMARGIN, COMP_TEXTMARGIN, ETO_OPAQUE, &lpUIPriv->rcCompText,
               lpszCompStr, dwCompStrLen, NULL);

    if (dwCompStrLen <= lpCompStr->dwCursorPos) {
        goto PaintCompUnlockIMCC;
    }

    int  nDx[MAX_COMPKEYS + 1];
    int  nFit;
    SIZE size;

    nDx[0] = 0;
    GetTextExtentExPoint(hDC, lpszCompStr, dwCompStrLen, 32767, &nFit, &nDx[1], &size);

    // Use window background color as text and background.
    // Error text won't show up on the reading window
    SetTextColor(hDC, g_sImeUIG.crWindow);
    SetBkColor(hDC, g_sImeUIG.crWindow);

    ExtTextOut(hDC, COMP_TEXTMARGIN + nDx[lpCompStr->dwCursorPos],
               COMP_TEXTMARGIN, ETO_OPAQUE, NULL,
               lpszCompStr + lpCompStr->dwCursorPos,
               dwCompStrLen - lpCompStr->dwCursorPos,
               NULL);

PaintCompUnlockIMCC:

    ImmUnlockIMCC(lpIMC->hCompStr);

    ImmUnlockIMC(hIMC);

    return;
}

/**********************************************************************/
/* OnCompCreate()                                                     */
/**********************************************************************/
static __inline
LRESULT OnCompCreate(HWND hCompWnd, CONST CREATESTRUCT * lpCS)
{
    HWND                hUIWnd;
    LPUIPRIV            lpUIPriv;
    HIMC                hIMC;
    CONST INPUTCONTEXT * lpIMC;
    LPCOMPOSITIONSTRING lpCompStr;
    LPCTSTR             lpszCompStr;
    TCHAR               szCompChar[2] = {0};
    int                 nMaxKey, i, nCompBytes;
    HDC                 hDC;
    TEXTMETRIC          tm;

    hUIWnd = GetWindow(hCompWnd, GW_OWNER);
    if (!hUIWnd) {
        return -1L;
    }

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return -1L;
    }
    SetWindowLong(hCompWnd, IMMGWL_PRIVATE, (LONG)lpUIPriv);

    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
    if (!lpUIPriv) {
        return -1L;
    }
    SetWindowLong(hCompWnd, IMMGWL_IMC, (LONG)hIMC);

    lpIMC = (CONST INPUTCONTEXT *)ImmLockIMC(hIMC);
    if (!lpIMC) {
        return -1L;
    }

    nMaxKey = ImmEscape(NULL, hIMC, IME_ESC_MAX_KEY, (LPVOID)NULL);
    RETAILMSG(1,(TEXT("IME_ESC_MAX_KEY was called. nMaxKey = %d\n"), nMaxKey));

    lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
    lpszCompStr = (LPCTSTR)((LPBYTE)lpCompStr + lpCompStr->dwCompStrOffset);
    // There is a problem. If the first composition string is '\0', then this calculation will get trouble.
    // Currently, We don't want to waste time on it. So use fix composition window width.
    for (i=0; i < nMaxKey; i++, lpszCompStr++) {
        if (0 == *lpszCompStr) { continue; }

        szCompChar[0] = *lpszCompStr;
    }
    ImmUnlockIMCC(lpIMC->hCompStr);

    if (szCompChar[0])
        nCompBytes = (szCompChar[0] < 0x80 ? 1 : 2);
    else
        nCompBytes = 2;

    hDC = GetDC(NULL);
    GetTextMetrics(hDC, &tm);
    ReleaseDC(NULL, hDC);

    lpUIPriv->ptComp.x = lpCS->x;
    lpUIPriv->ptComp.y = lpCS->y;

    lpUIPriv->rcCompText.left = 0;
    lpUIPriv->rcCompText.top = 0;
#ifdef BYTE_WIDTH
    lpUIPriv->rcCompText.right = (nMaxKey * nCompBytes * BYTE_WIDTH) + COMP_TEXTMARGIN * 2;
#else
    // Heck the width of reading window. Add 10% more in prevent wide character
    lpUIPriv->rcCompText.right = (nMaxKey * nCompBytes * tm.tmAveCharWidth) * 13 / 10 + COMP_TEXTMARGIN * 2;
#endif // FIX_COMPWIDTH
    lpUIPriv->rcCompText.bottom = tm.tmHeight + COMP_TEXTMARGIN * 2;

    lpUIPriv->nCompWi = lpUIPriv->rcCompText.right + g_sImeUIG.cxBorder * 2;
    lpUIPriv->nCompHi = lpUIPriv->rcCompText.bottom + g_sImeUIG.cxBorder * 2;

    return 0L;
}

/**********************************************************************/
/* OnCompDestroy()                                                    */
/**********************************************************************/
static __inline
LRESULT OnCompDestroy(HWND hCompWnd)
{
    LPUIPRIV lpUIPriv;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hCompWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return 0L;
    }

    lpUIPriv->hCompWnd = (HWND)NULL;

    return 0;
}

/**********************************************************************/
/* OnCompPaint()                                                      */
/**********************************************************************/
static __inline
LRESULT OnCompPaint(HWND hCompWnd)
{
    HDC         hDC;
    PAINTSTRUCT ps;

    hDC = BeginPaint(hCompWnd, &ps);
    PaintCompWindow(GetWindow(hCompWnd, GW_OWNER), hDC);
    EndPaint(hCompWnd, &ps);

    return 0L;
}

/**********************************************************************/
/* OnSettingChange()                                                  */
/**********************************************************************/
static __inline
LRESULT OnSettingChange(HWND hCompWnd, WPARAM wFlag)
{
    LPUIPRIV lpUIPriv;
    RECT     rcPrev;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hCompWnd, IMMGWL_PRIVATE);
    rcPrev = g_sImeUIG.rcWorkArea;

    switch(wFlag) {
    case SPI_SETCURRENTIM:
        GetWorkArea(&g_sImeUIG.rcWorkArea);
        break;
    case SPI_SETSIPINFO:
        GetWorkArea(&g_sImeUIG.rcWorkArea);
        break;
    default:
        return 1L;
    }

    if (rcPrev.left == g_sImeUIG.rcWorkArea.left &&
        rcPrev.top == g_sImeUIG.rcWorkArea.top &&
        rcPrev.right == g_sImeUIG.rcWorkArea.right &&
        rcPrev.bottom == g_sImeUIG.rcWorkArea.bottom) {
        return 0L;
    }

    if (lpUIPriv->fdwUIFlags & UI_CAND_ALREADY_OPEN) {
        POINT ptNew;

        ptNew.x = lpUIPriv->ptComp.x;
        ptNew.y = lpUIPriv->ptComp.y + lpUIPriv->nCompHi - g_sImeUIG.cyBorder;

        AdjustCandWnd(lpUIPriv->hCandWnd, ptNew);
    }

    return 0L;
}

/**********************************************************************/
/* CompWndProc()                                                      */
/**********************************************************************/
LRESULT CALLBACK CompWndProc(HWND   hCompWnd,
                             UINT   uMsg,
                             WPARAM wParam,
                             LPARAM lParam)
{
    switch (uMsg) {
    case WM_CREATE:
        return OnCompCreate(hCompWnd, (CONST CREATESTRUCT *)lParam);
    case WM_DESTROY:
        return OnCompDestroy(hCompWnd);
	case WM_IME_NOTIFY:
        if (wParam == IMN_SETCOMPOSITIONWINDOW) {
            SetCompPosition(hCompWnd);
        }
        break;
					   
    case WM_PAINT:
        return OnCompPaint(hCompWnd);
    case WM_SETTINGCHANGE:
        return OnSettingChange(hCompWnd, wParam);
        break;
    default:
        return DefWindowProc(hCompWnd, uMsg, wParam, lParam);
    }

    return 0L;
}


//=====================================================================//
//  Exported API                                                       //
//=====================================================================//

/**********************************************************************/
/* ShowComp() : Show the composition window                           */
/**********************************************************************/
void ShowComp(HWND hUIWnd, int nShowCompCmd)
{
    LPUIPRIV lpUIPriv;
    BOOL     bCompShowed;

    // show or hid the UI window
    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return;
    }

    if (!(lpUIPriv->fdwUIFlags & UI_COMP_ALREADY_START)) {
        return;
    }

    if (!lpUIPriv->hCompWnd) {
        return;
    }

    bCompShowed = IsWindowVisible(lpUIPriv->hCompWnd);

    if (bCompShowed && nShowCompCmd == SW_SHOWNOACTIVATE) {
        return;
    } else if (!bCompShowed && nShowCompCmd == SW_HIDE) {
        return;
    } else {
    }

    if (nShowCompCmd == SW_HIDE) {
        lpUIPriv->fdwSetContext &= ~ISC_SHOWUICOMPOSITIONWINDOW;
    }

    ShowWindow(lpUIPriv->hCompWnd, nShowCompCmd);

    return;
}

/**********************************************************************/
/* StartComp()                                                        */
/**********************************************************************/
void StartComp(HWND hUIWnd)
{
	DEBUGMSG(true,(TEXT("StartComp\n")));
    LPUIPRIV       lpUIPriv;
    HIMC                hIMC;
#ifndef WPC
    LPINPUTCONTEXT      lpIMC;
    LPCOMPOSITIONSTRING lpCompStr;
#endif // WPC
    DWORD               dwCompStrLen;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {     // Oh! Oh!
        return;
    }

    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
    if (!hIMC) {
        return;
    }

#ifndef WPC
    lpIMC = (LPINPUTCONTEXT)ImmLockIMC(hIMC);
    if (!lpIMC) {
        return;
    }

    lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
    if (!lpCompStr) {
        ImmUnlockIMC(hIMC);
        return;
    }

    dwCompStrLen = lpCompStr->dwCompStrLen;

    ImmUnlockIMCC(lpIMC->hCompStr);

    ImmUnlockIMC(hIMC);
#else
    UINT uData;

    uData = IME_ESC_GET_COMPSTR_LEN;
    if (ImmEscape(NULL, hIMC, IME_ESC_QUERY_SUPPORT, (LPVOID)&uData)) {
        dwCompStrLen = ImmEscape(NULL, hIMC, IME_ESC_GET_COMPSTR_LEN, NULL);
    } else {
        dwCompStrLen = 0;
    }
#endif // WPC

    if (0 == dwCompStrLen) { return; }

    if (!lpUIPriv->hCompWnd) {
        lpUIPriv->hCompWnd = CreateWindowEx(
            WS_EX_NOACTIVATE|WS_EX_TOPMOST,
            v_szCompClassName, NULL,
            WS_POPUP|WS_BORDER,
            0, 0, 10, 10,
            hUIWnd, (HMENU)NULL, lpUIPriv->hInst, NULL);

    }

    if (lpUIPriv->hCompWnd) {
        SetWindowPos(lpUIPriv->hCompWnd, HWND_TOPMOST, 0, 0,
                     lpUIPriv->nCompWi, lpUIPriv->nCompHi,
                     SWP_NOACTIVATE|SWP_NOMOVE);//|SWP_NOZORDER);
    }

    lpUIPriv->fdwUIFlags |= UI_COMP_ALREADY_START;

    SetCompPosition(lpUIPriv->hCompWnd);

    ShowComp(hUIWnd, SW_SHOWNOACTIVATE);

    return;
}

/**********************************************************************/
/* OnComp()                                                           */
/**********************************************************************/
void OnComp(HWND hUIWnd, DWORD fdwFlags)
{
    LPUIPRIV lpUIPriv;

    if (!(fdwFlags & (GCS_COMPSTR|GCS_COMPREADSTR|GCS_CURSORPOS))) {
        return;
    }

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return;
    }

    if (!(lpUIPriv->fdwUIFlags & UI_COMP_ALREADY_START)) {
        return;
    }
    
    if (lpUIPriv->hCompWnd) {
        HDC hDC;

        hDC = GetDC(lpUIPriv->hCompWnd);
        PaintCompWindow(hUIWnd, hDC);
        ReleaseDC(lpUIPriv->hCompWnd, hDC);
    }

    return;
}

/**********************************************************************/
/* EndComp()                                                          */
/**********************************************************************/
void EndComp(HWND hUIWnd)
{
    LPUIPRIV lpUIPriv;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return;
    }

    if (!(lpUIPriv->fdwUIFlags & UI_COMP_ALREADY_START)) {
        return;
    }

    lpUIPriv->fdwUIFlags &= ~(UI_COMP_ALREADY_START);

    if (lpUIPriv->hCompWnd && IsWindowVisible(lpUIPriv->hCompWnd)) {
        ShowWindow(lpUIPriv->hCompWnd, SW_HIDE);
    }

    return;
}

