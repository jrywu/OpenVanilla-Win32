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
#include <string.h>
#include "uipriv.h"

#define SPACING                    3

extern PPOINT pptStatusPos;
extern void GetWorkArea(LPRECT lprcWorkArea);

void PASCAL DrawDragBorder(HWND hWnd, LONG lCursorPos, LONG lCursorOffset, CONST RECT& rcWorkArea)
{
    HDC  hDC;
    int  cxBorder, cyBorder;
    int  x, y;
    RECT rcWnd;

    cxBorder = GetSystemMetrics(SM_CXBORDER);   // width of border
    cyBorder = GetSystemMetrics(SM_CYBORDER);   // height of border

    // get cursor position
    x = (*(LPPOINTS)&lCursorPos).x;
    y = (*(LPPOINTS)&lCursorPos).y;

    // calculate the org by the offset
    x -= (*(LPPOINTS)&lCursorOffset).x;
    y -= (*(LPPOINTS)&lCursorOffset).y;

    // check for the min boundary of the display
    if (x < rcWorkArea.left) {
        x = rcWorkArea.left;
    }

    if (y < rcWorkArea.top) {
        y = rcWorkArea.top;
    }

    // check for the max boundary of the display
    GetWindowRect(hWnd, &rcWnd);

    if (x + rcWnd.right - rcWnd.left > rcWorkArea.right)
        x = rcWorkArea.right - (rcWnd.right - rcWnd.left);

    if (y + rcWnd.bottom - rcWnd.top > rcWorkArea.bottom)
        y = rcWorkArea.bottom - (rcWnd.bottom - rcWnd.top);

    // draw the moving track
    hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
    SelectObject(hDC, GetStockObject(GRAY_BRUSH));

    // ->
    PatBlt(hDC, x, y, rcWnd.right - rcWnd.left - cxBorder, cyBorder,
        PATINVERT);
    // v
    PatBlt(hDC, x, y + cyBorder, cxBorder, rcWnd.bottom - rcWnd.top -
        cyBorder, PATINVERT);
    // _>
    PatBlt(hDC, x + cxBorder, y + rcWnd.bottom - rcWnd.top,
        rcWnd.right - rcWnd.left - cxBorder, -cyBorder, PATINVERT);
    //  v
    PatBlt(hDC, x + rcWnd.right - rcWnd.left, y,
        - cxBorder, rcWnd.bottom - rcWnd.top - cyBorder, PATINVERT);

    DeleteDC(hDC);
    return;
}

/**********************************************************************/
/* GetStatusWnd                                                       */
/* Return Value :                                                     */
/*      window handle of status window                                */
/**********************************************************************/
HWND PASCAL GetStatusWnd(HWND hUIWnd)                // UI window
{
    HLOCAL   hUIPrivate;
    LPUIPRIV lpUIPrivate;
    HWND     hStatusWnd;

    hUIPrivate = (HLOCAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!hUIPrivate) 
        return (HWND)NULL;

    lpUIPrivate = (LPUIPRIV)LocalLock(hUIPrivate);
    if (!lpUIPrivate) 
        return (HWND)NULL;

    hStatusWnd = lpUIPrivate->hStatusWnd;

    LocalUnlock(hUIPrivate);

    return (hStatusWnd);
}

/**********************************************************************/
/* AdjustStatusBoundary()                                             */
/**********************************************************************/
void PASCAL AdjustStatusBoundary(LPPOINTS lppt, HWND hUIWnd)
{
    LPCUIPRIV lpUIPrivate;

    lpUIPrivate = (LPCUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (lpUIPrivate->hStatusWnd == NULL)
        return;
    // display boundary check
    if (lppt->x < g_sImeUIG.rcWorkArea.left) {
        lppt->x = (short)g_sImeUIG.rcWorkArea.left;
    } 
    else if (lppt->x + lpUIPrivate->nStatusWidth > g_sImeUIG.rcWorkArea.right) {
        lppt->x = (short)(g_sImeUIG.rcWorkArea.right - lpUIPrivate->nStatusWidth);
    }

    if (lppt->y < g_sImeUIG.rcWorkArea.top) {
        lppt->y = (short)g_sImeUIG.rcWorkArea.top;
    } 
    else if (lppt->y + lpUIPrivate->nStatusHeight > g_sImeUIG.rcWorkArea.bottom) {
        lppt->y = (short)(g_sImeUIG.rcWorkArea.bottom - lpUIPrivate->nStatusHeight);
    }
    return;
}

/**********************************************************************/
/* SetStatusWindowPos()                                               */
/**********************************************************************/
LRESULT PASCAL SetStatusWindowPos(HWND hStatusWnd)
{
    HWND           hUIWnd;
    HIMC           hIMC;
    CONST INPUTCONTEXT * lpIMC;
    POINTS         ptPos;

    hUIWnd = GetWindow(hStatusWnd, GW_OWNER);

    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
    if (!hIMC)
        return (1L);

    lpIMC = (CONST INPUTCONTEXT *)ImmLockIMC(hIMC);
    if (!lpIMC) 
        return (1L);


    ptPos.x = (short)lpIMC->ptStatusWndPos.x;
    ptPos.y = (short)lpIMC->ptStatusWndPos.y;

    // display boundary adjust
    AdjustStatusBoundary(&ptPos, hUIWnd);

    pptStatusPos->x = ptPos.x;
    pptStatusPos->y = ptPos.y;
    SetWindowPos(hStatusWnd, NULL,
                 ptPos.x, ptPos.y,
                 0, 0, SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER);

    ImmUnlockIMC(hIMC);

    return (0L);
}

/**********************************************************************/
/* ShowStatus()                                                       */
/**********************************************************************/
void PASCAL ShowStatus( HWND hUIWnd, int  nShowStatusCmd)
{
    HLOCAL   hUIPrivate;
    LPUIPRIV lpUIPrivate;
    HIMC           hIMC;
    LPINPUTCONTEXT lpIMC;

    hUIPrivate = (HLOCAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!hUIPrivate)
        return;

    lpUIPrivate = (LPUIPRIV)LocalLock(hUIPrivate);
    if (!lpUIPrivate) 
        return;
        
    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
    if (!hIMC) {
        LocalUnlock(hUIPrivate);
        return;
    }
    
    lpIMC = (LPINPUTCONTEXT)ImmLockIMC(hIMC);

    if (lpUIPrivate->hStatusWnd) {
        if (lpUIPrivate->nShowStatusCmd != nShowStatusCmd) {
            GetWorkArea(&g_sImeUIG.rcWorkArea);
            if (lpIMC) {
                if (pptStatusPos->x >= 0 && pptStatusPos->y >= 0) {
                    lpIMC->ptStatusWndPos.x = pptStatusPos->x;
                    lpIMC->ptStatusWndPos.y = pptStatusPos->y;
                }
                else {
                    pptStatusPos->x = lpIMC->ptStatusWndPos.x;
                    pptStatusPos->y = lpIMC->ptStatusWndPos.y;
                }
                ImmUnlockIMC(hIMC);
            }
            
            SetStatusWindowPos(lpUIPrivate->hStatusWnd);
            ShowWindow(lpUIPrivate->hStatusWnd, nShowStatusCmd);
            lpUIPrivate->nShowStatusCmd = nShowStatusCmd;
        }
    }

    LocalUnlock(hUIPrivate);

    return;
}

/**********************************************************************/
/* OpenStatus()                                                       */
/**********************************************************************/
void PASCAL OpenStatus( HWND hUIWnd)
{
    HLOCAL         hUIPrivate;
    LPUIPRIV       lpUIPrivate;
    HIMC           hIMC;
    CONST INPUTCONTEXT * lpIMC;
    POINT          ptPos;
    int            nShowStatusCmd;
    BOOL           fInit = FALSE;

    hUIPrivate = (HLOCAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!hUIPrivate) 
        return;

    lpUIPrivate = (LPUIPRIV)LocalLock(hUIPrivate);
    if (!lpUIPrivate) 
        return;

    if (!lpUIPrivate->hStatusWnd) {
        lpUIPrivate->hStatusWnd = CreateWindowEx(WS_EX_NOACTIVATE|WS_EX_TOPMOST|WS_EX_TOOLWINDOW,v_szStatusClassName, NULL,WS_POPUP|WS_BORDER|WS_NOTIFY|CLS_FRACTION, 0, 0, 50, 27, hUIWnd, (HMENU)NULL, lpUIPrivate->hInst, NULL);

        if (!lpUIPrivate->hStatusWnd) {
            LocalUnlock(hUIPrivate);
            return;
        }

        fInit = TRUE;
    }
    
    GetWorkArea(&g_sImeUIG.rcWorkArea);
    
    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
    if (!hIMC) {
        ptPos.x = g_sImeUIG.rcWorkArea.right - lpUIPrivate->nStatusWidth;
        ptPos.y = g_sImeUIG.rcWorkArea.bottom - lpUIPrivate->nStatusHeight;
        nShowStatusCmd = SW_HIDE;
    }
    else if (lpIMC = (CONST INPUTCONTEXT *)ImmLockIMC(hIMC)) {
        if (fInit)
            ptPos.x = g_sImeUIG.rcWorkArea.right - lpUIPrivate->nStatusWidth;
        else if (lpIMC->ptStatusWndPos.x < g_sImeUIG.rcWorkArea.left)
            ptPos.x = g_sImeUIG.rcWorkArea.right - lpUIPrivate->nStatusWidth;
        else if (lpIMC->ptStatusWndPos.x + lpUIPrivate->nStatusWidth > g_sImeUIG.rcWorkArea.right)
            ptPos.x = g_sImeUIG.rcWorkArea.right - lpUIPrivate->nStatusWidth;
        else
            ptPos.x = lpIMC->ptStatusWndPos.x;

        if (fInit)    
            ptPos.y = g_sImeUIG.rcWorkArea.bottom - lpUIPrivate->nStatusHeight;
        else if (lpIMC->ptStatusWndPos.y < g_sImeUIG.rcWorkArea.top)
            ptPos.y = g_sImeUIG.rcWorkArea.bottom - lpUIPrivate->nStatusHeight;
        else if (lpIMC->ptStatusWndPos.y + lpUIPrivate->nStatusHeight > g_sImeUIG.rcWorkArea.right)
            ptPos.y = g_sImeUIG.rcWorkArea.bottom - lpUIPrivate->nStatusHeight;
        else
            ptPos.y = lpIMC->ptStatusWndPos.y;
        
        ImmUnlockIMC(hIMC);
        nShowStatusCmd = SW_SHOWNOACTIVATE;
    } 
    else {
        ptPos.x = g_sImeUIG.rcWorkArea.right - lpUIPrivate->nStatusWidth;
        ptPos.y = g_sImeUIG.rcWorkArea.bottom - lpUIPrivate->nStatusHeight;
        nShowStatusCmd = SW_HIDE;
    }

    if (pptStatusPos->x < 0 && pptStatusPos->y < 0)
        *pptStatusPos = ptPos;
    else
        ptPos = *pptStatusPos;

    if (lpUIPrivate->hStatusWnd)
        SetWindowPos(lpUIPrivate->hStatusWnd, HWND_TOPMOST, ptPos.x, ptPos.y, 0, 0, SWP_NOACTIVATE|SWP_NOSIZE);
    
//    lpUIPrivate->fdwSetContext |= ISC_SHOWUIALL;

    if (hIMC)
        ShowStatus(hUIWnd, SW_SHOWNOACTIVATE);

    LocalUnlock(hUIPrivate);

    return;
}

/**********************************************************************/
/* DestroyStatusWindow()                                              */
/**********************************************************************/
void PASCAL DestroyStatusWindow( HWND hStatusWnd)
{
    HWND     hUIWnd;
    HLOCAL   hUIPrivate;
    LPUIPRIV lpUIPrivate;

    hUIWnd = GetWindow(hStatusWnd, GW_OWNER);

    hUIPrivate = (HLOCAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!hUIPrivate) 
        return;

    lpUIPrivate = (LPUIPRIV)LocalLock(hUIPrivate);
    if (!lpUIPrivate) 
        return;

    lpUIPrivate->nShowStatusCmd = SW_HIDE;

    lpUIPrivate->hStatusWnd = (HWND)NULL;

    LocalUnlock(hUIPrivate);

    return;
}

/**********************************************************************/
/* SetStatus                                                          */
/**********************************************************************/
void PASCAL SetStatus(HWND hStatusWnd, CONST POINT * lpptCursor)
{
    HWND           hUIWnd;
    HIMC           hIMC;
    CONST INPUTCONTEXT * lpIMC;
    
    HLOCAL    hUIPrivate;
    LPCUIPRIV lpUIPrivate;
    DWORD fdwConversion;

    hUIWnd = GetWindow(hStatusWnd, GW_OWNER);
    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
    if (!hIMC)
        return;

    lpIMC = (CONST INPUTCONTEXT *)ImmLockIMC(hIMC);
    if (!lpIMC)
        return;

    hUIPrivate = (HLOCAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!hUIPrivate)
        return;

    lpUIPrivate = (LPCUIPRIV)LocalLock(hUIPrivate);
    if (!lpUIPrivate) 
        return;

	if (!lpIMC->fOpen){
        ImmSetOpenStatus(hIMC, FALSE);
		SendMessage(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
	}
    else if (PtInRect(&lpUIPrivate->rcStatusButton, *lpptCursor)) {

        if (lpIMC->fdwConversion & IME_CMODE_NATIVE)
        // change to alphanumeric mode
            fdwConversion = lpIMC->fdwConversion & ~(IME_CMODE_CHARCODE | IME_CMODE_NATIVE | IME_CMODE_EUDC);
        else
            fdwConversion = (lpIMC->fdwConversion | IME_CMODE_NATIVE) &    ~(IME_CMODE_CHARCODE | IME_CMODE_EUDC);

        ImmSetConversionStatus(hIMC, fdwConversion, lpIMC->fdwSentence);
    } 
    else if (PtInRect(&lpUIPrivate->rcFullButton, *lpptCursor)) {
        if (lpIMC->fdwConversion & IME_CMODE_FULLSHAPE)
            fdwConversion = lpIMC->fdwConversion & ~IME_CMODE_FULLSHAPE;
        else
            fdwConversion = lpIMC->fdwConversion | IME_CMODE_FULLSHAPE;
        ImmSetConversionStatus(hIMC, fdwConversion, lpIMC->fdwSentence);
    }
    else
        MessageBeep((UINT)-1);

    LocalUnlock(hUIPrivate);
    ImmUnlockIMC(hIMC);

    return;
}

/**********************************************************************/
/* PaintStatusWindow()                                                */
/**********************************************************************/
void PASCAL PaintStatusWindow(HDC  hDC, HWND hStatusWnd)
{
    HWND           hUIWnd;
    HIMC           hIMC;
    CONST INPUTCONTEXT * lpIMC;
    HANDLE         hUIPrivate;
    LPUIPRIV       lpUIPrivate;
    HBRUSH         hbr, hbrold;

    hUIWnd = GetWindow(hStatusWnd, GW_OWNER);
    
    hUIPrivate = (HLOCAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!hUIPrivate)
        return;

    lpUIPrivate = (LPUIPRIV)LocalLock(hUIPrivate);
    if (!lpUIPrivate) 
        return;
    

    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
    if (!hIMC) {
        LocalUnlock(hUIPrivate);
        MessageBeep((UINT)-1);
        return;
    }

    if (!(lpIMC = (CONST INPUTCONTEXT *)ImmLockIMC(hIMC))) {
        LocalUnlock(hUIPrivate);
        MessageBeep((UINT)-1);
        return;
    }

    if (lpIMC->fOpen)
        SetTextColor(hDC, RGB(0x00, 0x00, 0x00));
    else
        SetTextColor(hDC, RGB(0x80, 0x80, 0x80));

    SetBkColor(hDC, RGB(0xC0, 0xC0, 0xC0));
    hbr = CreateSolidBrush(RGB(0xC0, 0xC0, 0xC0));
    hbrold = (HBRUSH) SelectObject(hDC, hbr);
    FillRect(hDC, &lpUIPrivate->rcStatusWnd, hbr);
    Rectangle(hDC, lpUIPrivate->rcStatusButton.left, lpUIPrivate->rcStatusButton.top, lpUIPrivate->rcStatusButton.right, lpUIPrivate->rcStatusButton.bottom);
    Rectangle(hDC, lpUIPrivate->rcFullButton.left, lpUIPrivate->rcFullButton.top, lpUIPrivate->rcFullButton.right, lpUIPrivate->rcFullButton.bottom);
    
    DrawText(hDC, lpIMC->fdwConversion & IME_CMODE_NATIVE ? TEXT("\x4e2d") : TEXT("A"), 1, &lpUIPrivate->rcStatusButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    DrawText(hDC, lpIMC->fdwConversion & IME_CMODE_FULLSHAPE ? TEXT("\x5168") : TEXT("\x534a"), 1, &lpUIPrivate->rcFullButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hDC, hbrold);
    DeleteObject(hbr);
    ImmUnlockIMC(hIMC);
    LocalUnlock(hUIPrivate);
    return;
}

static __inline LRESULT OnStatusCreate(HWND hStatusWnd, CONST CREATESTRUCT * lpCS)
{
    HWND     hUIWnd;
    LPUIPRIV lpUIPriv;
    HIMC     hIMC;
    HDC      hDC;
    SIZE     size;

    hUIWnd = GetWindow(hStatusWnd, GW_OWNER);
    if (!hUIWnd) {
        return -1L;
    }

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return -1L;
    }
    SetWindowLong(hStatusWnd, IMMGWL_PRIVATE, (LONG)lpUIPriv);

    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
    if (!lpUIPriv) {
        return -1L;
    }
    SetWindowLong(hStatusWnd, IMMGWL_IMC, (LONG)hIMC);

    hDC = GetDC(NULL);

    GetTextExtentPoint(hDC, TEXT("\x4e2d"), 1, &size);

    ReleaseDC(NULL, hDC);

    lpUIPriv->nStatusWidth = lpCS->cx;
    lpUIPriv->nStatusHeight = lpCS->cy;


    lpUIPriv->rcStatusWnd.left = lpUIPriv->rcStatusWnd.top = 0;
    lpUIPriv->rcStatusWnd.right = lpCS->cx;
    lpUIPriv->rcStatusWnd.bottom = lpCS->cy;
    
    lpUIPriv->rcStatusButton.left = lpUIPriv->rcStatusButton.top = SPACING;
    lpUIPriv->rcStatusButton.right = lpUIPriv->rcStatusButton.left + 20;
    lpUIPriv->rcStatusButton.bottom = lpUIPriv->rcStatusButton.top + 20;

    lpUIPriv->rcFullButton.left = lpUIPriv->rcStatusButton.right + SPACING;
    lpUIPriv->rcFullButton.top = SPACING;
    lpUIPriv->rcFullButton.right = lpUIPriv->rcFullButton.left + 20;
    lpUIPriv->rcFullButton.bottom = lpUIPriv->rcFullButton.top + 20;
    return 0L;
}


/**********************************************************************/
/* StatusWndProc()                                                    */
/**********************************************************************/
LRESULT CALLBACK StatusWndProc(HWND hStatusWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND           hUIWnd;
    HANDLE         hUIPrivate;
    LPUIPRIV       lpUIPrivate;

    switch (uMsg) {
    case WM_CREATE:
        return OnStatusCreate(hStatusWnd, (CONST CREATESTRUCT *)lParam);
    case WM_DESTROY:
        DestroyStatusWindow(hStatusWnd);
        break;
            
    case WM_MOUSEMOVE:
        {
            hUIWnd = GetWindow(hStatusWnd, GW_OWNER);
            
            hUIPrivate = (HLOCAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
            if (!hUIPrivate)
                return DefWindowProc(hStatusWnd, uMsg, wParam, lParam);
        
            lpUIPrivate = (LPUIPRIV)LocalLock(hUIPrivate);
            if (!lpUIPrivate) 
                return DefWindowProc(hStatusWnd, uMsg, wParam, lParam);
                
            if (lpUIPrivate->dwUIMoveOffset != WINDOW_NOT_DRAG) {
                POINT ptCursor;
        
                DrawDragBorder(hStatusWnd, lpUIPrivate->dwUIMoveXY, lpUIPrivate->dwUIMoveOffset, g_sImeUIG.rcWorkArea);
                ptCursor.x = LOWORD(lParam);
                ptCursor.y = HIWORD(lParam);
                lpUIPrivate->dwUIMoveXY = MAKELONG(ptCursor.x, ptCursor.y);
                DrawDragBorder(hStatusWnd, lpUIPrivate->dwUIMoveXY, lpUIPrivate->dwUIMoveOffset, g_sImeUIG.rcWorkArea);
            } 
            else {
                LocalUnlock(hUIPrivate);
                return DefWindowProc(hStatusWnd, uMsg, wParam, lParam);
            }
                    
               LocalUnlock(hUIPrivate);
        }
        
        break;

    case WM_LBUTTONDOWN:
        {
            POINT ptCursor, ptSavCursor;
            RECT  rcWnd;

            hUIWnd = GetWindow(hStatusWnd, GW_OWNER);
            
            hUIPrivate = (HLOCAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
            if (!hUIPrivate)
                return DefWindowProc(hStatusWnd, uMsg, wParam, lParam);
        
            lpUIPrivate = (LPUIPRIV)LocalLock(hUIPrivate);
            if (!lpUIPrivate) 
                return DefWindowProc(hStatusWnd, uMsg, wParam, lParam);
        

            ptCursor.x = LOWORD(lParam); 
            ptCursor.y = HIWORD(lParam); 
            ptSavCursor = ptCursor;
        
    
            if (PtInRect(&lpUIPrivate->rcStatusButton, ptCursor) || PtInRect(&lpUIPrivate->rcFullButton, ptCursor)) {
                SetStatus(hStatusWnd, &ptCursor);
                break;
            } 
            else {
                GetWorkArea(&g_sImeUIG.rcWorkArea);
            }
        
            SetCapture(hStatusWnd);
            lpUIPrivate->dwUIMoveXY = MAKELONG(ptSavCursor.x, ptSavCursor.y);
            GetWindowRect(hStatusWnd, &rcWnd);
            lpUIPrivate->dwUIMoveOffset = MAKELONG(ptSavCursor.x - rcWnd.left, ptSavCursor.y - rcWnd.top);
        
            DrawDragBorder(hStatusWnd, lpUIPrivate->dwUIMoveXY, lpUIPrivate->dwUIMoveOffset, g_sImeUIG.rcWorkArea);
        
            LocalUnlock(hUIPrivate);
        }
        break;
    case WM_LBUTTONUP:
        {
            hUIWnd = GetWindow(hStatusWnd, GW_OWNER);
            hUIPrivate = (HLOCAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
            if (!hUIPrivate)
                return DefWindowProc(hStatusWnd, uMsg, wParam, lParam);
        
            lpUIPrivate = (LPUIPRIV)LocalLock(hUIPrivate);
            if (!lpUIPrivate) 
                return DefWindowProc(hStatusWnd, uMsg, wParam, lParam);

            if (lpUIPrivate->dwUIMoveOffset != WINDOW_NOT_DRAG) {
                LPARAM lTmpCursor, lTmpOffset;
        
                lTmpCursor = lpUIPrivate->dwUIMoveXY;
        
                // calculate the org by the offset
                lTmpOffset = lpUIPrivate->dwUIMoveOffset;
        
                DrawDragBorder(hStatusWnd, lTmpCursor, lTmpOffset, g_sImeUIG.rcWorkArea);
        
                (*(LPPOINTS)&lTmpCursor).x -= (*(LPPOINTS)&lTmpOffset).x;
                (*(LPPOINTS)&lTmpCursor).y -= (*(LPPOINTS)&lTmpOffset).y;
        
                lpUIPrivate->dwUIMoveOffset = WINDOW_NOT_DRAG;
                ReleaseCapture();
        
                AdjustStatusBoundary((LPPOINTS)&lTmpCursor, GetWindow(hStatusWnd, GW_OWNER));
        
                SendMessage(GetWindow(hStatusWnd, GW_OWNER), WM_IME_CONTROL, IMC_SETSTATUSWINDOWPOS, lTmpCursor);
            } 
            else {
                LocalUnlock(hUIPrivate);
                return DefWindowProc(hStatusWnd, uMsg, wParam, lParam);
            }
               LocalUnlock(hUIPrivate);
        }
            
        break;

    case WM_IME_NOTIFY:
        // get work area for changing
        {
            hUIWnd = GetWindow(hStatusWnd, GW_OWNER);
            
            hUIPrivate = (HLOCAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
            if (!hUIPrivate)
                return DefWindowProc(hStatusWnd, uMsg, wParam, lParam);
        
            lpUIPrivate = (LPUIPRIV)LocalLock(hUIPrivate);
            if (!lpUIPrivate) 
                return DefWindowProc(hStatusWnd, uMsg, wParam, lParam);
            GetWorkArea(&g_sImeUIG.rcWorkArea);
            LocalUnlock(hUIPrivate);
        }
        

        if (wParam == IMN_SETSTATUSWINDOWPOS) {
            SetStatusWindowPos(hStatusWnd);
        }
        break;
    case WM_PAINT:
        {
            HDC         hDC;
            PAINTSTRUCT ps;
    
            hDC = BeginPaint(hStatusWnd, &ps);
            PaintStatusWindow(hDC, hStatusWnd);
            EndPaint(hStatusWnd, &ps);
        }
        break;
    default:
        return DefWindowProc(hStatusWnd, uMsg, wParam, lParam);
    }

    return (0L);
}

