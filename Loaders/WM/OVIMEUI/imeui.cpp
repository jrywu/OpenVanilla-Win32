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
#include "OVutility.h"

#include "imemsg.hpp"
#define IMN_PRIVATE_NONIME_TOGGLE_ON       0x000A
#define IMN_PRIVATE_NONIME_TOGGLE_OFF      0x000B
#define IMN_PRIVATE_SETACTIVECONTEXT       0x000C

extern LRESULT PASCAL SetStatusWindowPos(HWND hStatusWnd);
extern HWND PASCAL GetStatusWnd(HWND hUIWnd);
extern void GetWorkArea(LPRECT lprcWorkArea);
extern void CalcCandSize(HWND hCandWnd);
extern void AdjustCandWnd(HWND hCandWnd, POINT ptNew);

#define IME_CHOTKEY_DISMISS            (IME_CHOTKEY_FIRST+4)

typedef struct _HOTKEY {
    UINT uVKey;
    UINT uMod;
} HOTKEY;

HOTKEY CandDismiss;

void PressHotKey(HOTKEY hk)
{
    UINT uAlt, uCtrl, uShift, uRL;
    if (hk.uMod & MOD_IGNORE_ALL_MODIFIER) {
        keybd_event(hk.uVKey, 0, 0, 0);
        keybd_event(hk.uVKey, 0, KEYEVENTF_KEYUP, 0);
        return;
    }
    uCtrl = hk.uMod & MOD_CONTROL;
    uAlt  = hk.uMod & MOD_ALT;
    uShift = hk.uMod & MOD_SHIFT;
    uRL = hk.uMod & (MOD_LEFT | MOD_RIGHT);
    
    if (uRL) {
        if (uRL & MOD_RIGHT) {
            if (uCtrl)
                keybd_event(VK_RCONTROL, 0, 0, 0);
            if (uAlt)
                keybd_event(VK_RMENU, 0, 0, 0);
            if (uShift)
                keybd_event(VK_RSHIFT, 0, 0, 0);
            keybd_event(hk.uVKey, 0, 0, 0);
            keybd_event(hk.uVKey, 0, KEYEVENTF_KEYUP, 0);
            if (uShift)
                keybd_event(VK_RSHIFT, 0, KEYEVENTF_KEYUP, 0);
            if (uAlt)
                keybd_event(VK_RMENU, 0, KEYEVENTF_KEYUP, 0);
            if (uCtrl)
                keybd_event(VK_RCONTROL, 0, KEYEVENTF_KEYUP, 0);
        }
        else {
            if (uCtrl)
                keybd_event(VK_LCONTROL, 0, 0, 0);
            if (uAlt)
                keybd_event(VK_LMENU, 0, 0, 0);
            if (uShift)
                keybd_event(VK_LSHIFT, 0, 0, 0);
            keybd_event(hk.uVKey, 0, 0, 0);
            keybd_event(hk.uVKey, 0, KEYEVENTF_KEYUP, 0);
            if (uShift)
                keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
            if (uAlt)
                keybd_event(VK_LMENU, 0, KEYEVENTF_KEYUP, 0);
            if (uCtrl)
                keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);
        }
    }
}

/**********************************************************************/
/* OnUICreate()                                                       */
/**********************************************************************/
static __inline
LRESULT OnUICreate(HWND hUIWnd, LPCREATESTRUCT lpCS)
{
    LPUIPRIV    lpUIPriv;

    lpUIPriv = (LPUIPRIV)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(UIPRIV));
    if (!lpUIPriv) { return -1L; }

    memset(lpUIPriv, 0, sizeof(UIPRIV));

#ifdef WPC
    // This is because Rapier. Rapier SIP just send the notify messages to
    // top level and visible window while the SIP is showed or hided.
    // So ImeUI has to set to TOPMOST and VISIBLE.

    // Move to ChangeImeUIStyle(). We can't change the IME UI window style
    //   at this meanwhile. GWE will active IME UI window if UI's style is
    //   visible and it's real visible.
    // This is in winceos\coreos\winmgr\wmbase\wmbase.cpp - line 705.
    // We have to set UI's style late.

    // *** Important ***
    // Set IME UI as visible caused a lot of problems. We have to turn it off.
    //   So let composition window handle WM_SETTINGCHANGE.
#endif // WPC

    lpUIPriv->dwUIMoveOffset = WINDOW_NOT_DRAG;
    lpUIPriv->bOpenStatus = TRUE;

    SetWindowLong(hUIWnd, IMMGWL_PRIVATE, (LONG)lpUIPriv);

    lpUIPriv->hInst = g_hInst; //lpCS->hInstance;

    return 0L;
}

/**********************************************************************/
/* OnUIDestroy()                                                        */
/**********************************************************************/
static __inline
LRESULT OnUIDestroy(HWND hUIWnd)
{
    LPUIPRIV lpUIPriv;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);

    if(!lpUIPriv) 
        return 0L;

    if (lpUIPriv->hStatusWnd) {
        DestroyWindow(lpUIPriv->hStatusWnd);
        lpUIPriv->hStatusWnd = NULL;
    }

    // composition window need to be destroyed
    if (lpUIPriv->hCompWnd) {
        DestroyWindow(lpUIPriv->hCompWnd);
        lpUIPriv->hCompWnd = NULL;
    }

    // candidate window need to be destroyed
    if (lpUIPriv->hCandWnd) {
        DestroyWindow(lpUIPriv->hCandWnd);
        lpUIPriv->hCandWnd = NULL;
    }

    HeapFree(GetProcessHeap(), 0, (LPVOID)lpUIPriv);

    SetWindowLong(hUIWnd, IMMGWL_PRIVATE, (LONG)0);

    return 0L;
}

/**********************************************************************/
/* ShowUI() : show the sub windows                                    */
/**********************************************************************/
void ShowUI(HWND hUIWnd, int nShowCmd)
{
    HIMC           hIMC;
    CONST INPUTCONTEXT * lpIMC;
    LPUIPRIV       lpUIPriv;

    if (nShowCmd == SW_HIDE) {
    } else if ( !(hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC)) ) {
        nShowCmd = SW_HIDE;
    } else if (!(lpIMC = (CONST INPUTCONTEXT *)ImmLockIMC(hIMC))) {
        nShowCmd = SW_HIDE;
    } else {
    }

    if (nShowCmd == SW_HIDE) {
        if (!g_sImeUIG.bNoStatus)
            ShowStatus(hUIWnd, nShowCmd);
        ShowComp(hUIWnd, nShowCmd);
        ShowCand(hUIWnd, nShowCmd);

        return;
    }

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    
    if(!lpUIPriv) 
        return;

    if (lpUIPriv->bOpenStatus && nShowCmd == SW_HIDE)
        return;
    if (!lpUIPriv->bOpenStatus && nShowCmd != SW_HIDE)
        return;

    if (lpUIPriv->hStatusWnd && !g_sImeUIG.bNoStatus)
        {
        if (IsWindowVisible(lpUIPriv->hStatusWnd))
            {
            // some time the WM_ERASEBKGND is eaten by the app
            InvalidateRect(lpUIPriv->hStatusWnd, NULL, TRUE);
            }

        SendMessage(lpUIPriv->hStatusWnd, WM_IME_NOTIFY, IMN_OPENSTATUSWINDOW, 0);

        if (!IsWindowVisible(lpUIPriv->hStatusWnd))
            {
            ShowStatus(hUIWnd, SW_SHOWNOACTIVATE);
            }
        }


    if ((lpUIPriv->fdwSetContext & ISC_SHOWUICOMPOSITIONWINDOW) &&
        (lpUIPriv->fdwUIFlags & UI_COMP_ALREADY_START)) {
        if (lpUIPriv->hCompWnd) {
            if (IsWindowVisible(lpUIPriv->hCompWnd)) {
                // some time the WM_ERASEBKGND is eaten by the app
                InvalidateRect(lpUIPriv->hCompWnd, NULL, TRUE);
            }

            SendMessage(lpUIPriv->hCompWnd, WM_IME_NOTIFY, IMN_SETCOMPOSITIONWINDOW, 0);

            if (!IsWindowVisible(lpUIPriv->hCompWnd)) {
                ShowComp(hUIWnd, SW_SHOWNOACTIVATE);
            }
        } else {
            lpUIPriv->fdwSetContext |= ISC_SHOWUICOMPOSITIONWINDOW;
            StartComp(hUIWnd);
        }
    } else if (!IsWindowVisible(lpUIPriv->hCompWnd)) {
    } else {
        ShowComp(hUIWnd, SW_HIDE);
    }

    if ((lpUIPriv->fdwSetContext & ISC_SHOWUICANDIDATEWINDOW) &&
        (lpUIPriv->fdwUIFlags & UI_CAND_ALREADY_OPEN)) {
        if (lpUIPriv->hCandWnd)    {
            if (IsWindowVisible(lpUIPriv->hCandWnd)) { 
                // some time the WM_ERASEBKGND is eaten by the app
                InvalidateRect(lpUIPriv->hCandWnd, NULL, TRUE);
            }

            SendMessage(lpUIPriv->hCandWnd, WM_IME_NOTIFY, IMN_SETCANDIDATEPOS, 0x0001);

            if (!IsWindowVisible(lpUIPriv->hCandWnd)) {
                ShowCand(hUIWnd, SW_SHOWNOACTIVATE);
            }
        } else {
            lpUIPriv->fdwSetContext |= ISC_SHOWUICANDIDATEWINDOW;
            OpenCand(hUIWnd);
        }
    } else if (!IsWindowVisible(lpUIPriv->hCandWnd)) {
    } else {
        ShowCand(hUIWnd, SW_HIDE);
    }

    // we switch to this hIMC
    lpUIPriv->hCacheIMC = hIMC;

    if ( lpIMC ) {
        ImmUnlockIMC(hIMC);
    }

    return;
}

/**********************************************************************/
/* ShowGuideLine                                                      */
/**********************************************************************/
void ShowGuideLine(HWND hUIWnd)
{
    HIMC           hIMC;
    CONST INPUTCONTEXT * lpIMC;
    CONST GUIDELINE * lpGuideLine;

    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
    if (!hIMC) { return; }

    lpIMC = (CONST INPUTCONTEXT *)ImmLockIMC(hIMC);
    if (!lpIMC) { return; }

    lpGuideLine = (CONST GUIDELINE *)ImmLockIMCC(lpIMC->hGuideLine);

    if (!lpGuideLine) {
    } else if (lpGuideLine->dwLevel == GL_LEVEL_ERROR) {
        MessageBeep((UINT)-1);
        MessageBeep((UINT)-1);
    } else if (lpGuideLine->dwLevel == GL_LEVEL_WARNING) {
        MessageBeep((UINT)-1);
    } else {
    }

    ImmUnlockIMCC(lpIMC->hGuideLine);
    ImmUnlockIMC(hIMC);

    return;
}

/**********************************************************************/
/* OnUINotify()                                                       */
/**********************************************************************/
static __inline
LRESULT OnUINotify(HWND hUIWnd, WPARAM wParam, LPARAM lParam)
{
    LPUIPRIV lpUIPriv;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    
    if (!lpUIPriv) 
        return -1L;

    switch (wParam) {
    case IMN_OPENSTATUSWINDOW:
		////murmur("IME_NOTIFY: IMN_OPENSTATUSWINDOW");
        if (!g_sImeUIG.bNoStatus)
            OpenStatus(hUIWnd);
        break;
    case IMN_CLOSESTATUSWINDOW:
		////murmur("IME_NOTIFY: IMN_CLOSESTATUSWINDOW");
        break;
    case IMN_SETSTATUSWINDOWPOS:
		////murmur("IME_NOTIFY: IMN_SETSTATUSWINDOWPOS");
        if (!g_sImeUIG.bNoStatus)
            SetStatusWindowPos(lpUIPriv->hStatusWnd);
        break;
    case IMN_OPENCANDIDATE:
		////murmur("IME_NOTIFY: IMN_OPENCANDIDATE");
        OpenCand(hUIWnd);
        break;
    case IMN_CHANGECANDIDATE:
		////murmur("IME_NOTIFY: IMN_CHANGECANDIDATE");
        ChangeCand(hUIWnd);
        break;
    case IMN_CLOSECANDIDATE:
		////murmur("IME_NOTIFY: IMN_CLOSECANDIDATE");
        CloseCand(hUIWnd);
        break;
    case IMN_SETSENTENCEMODE:
		////murmur("IME_NOTIFY: IMN_SETSENTENCEMODE");
        break;
    case IMN_SETOPENSTATUS:
		////murmur("IME_NOTIFY: IMN_SETOPENSTATUS");
        if (!g_sImeUIG.bNoStatus) {
            HIMC hIMC;
            CONST INPUTCONTEXT * lpIMC;
            hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
            if (!hIMC) 
                break;
            lpIMC = (CONST INPUTCONTEXT *)ImmLockIMC(hIMC);
            if (!lpIMC) 
                break;

            if (ImmGetOpenStatus(hIMC)) {
                lpUIPriv->bOpenStatus = TRUE;
                ShowUI(hUIWnd, SW_SHOWNOACTIVATE);
            }
            else {
                lpUIPriv->bOpenStatus = FALSE;
                ShowUI(hUIWnd, SW_HIDE);
            }
            
            ImmUnlockIMC(hIMC);
            InvalidateRect(lpUIPriv->hStatusWnd, NULL, NULL);
        }
        break;
    case IMN_SETCONVERSIONMODE:
		////murmur("IME_NOTIFY: IMN_SETCONVERSIONMODE");
        if (!g_sImeUIG.bNoStatus) {
            HIMC hIMC;
            CONST INPUTCONTEXT * lpIMC;
            hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
            if (!hIMC) 
                break;
            lpIMC = (CONST INPUTCONTEXT *)ImmLockIMC(hIMC);
            if (!lpIMC) 
                break;

            RETAILMSG(1,(TEXT("OnUINotify()::lpIMC->fdwConversion = 0x%x\n"), lpIMC->fdwConversion));
            ImmUnlockIMC(hIMC);
            InvalidateRect(lpUIPriv->hStatusWnd, NULL, NULL);
        }
        break;

    case IMN_SETCOMPOSITIONFONT:
		////murmur("IME_NOTIFY: IMN_SETCOMPOSITIONFONT");
        // we are not going to change font, but an IME can do this if it want
        break;
    case IMN_SETCOMPOSITIONWINDOW:
		////murmur("IME_NOTIFY: IMN_SETCOMPOSITIONWINDOW");
        break;
    case IMN_SETCANDIDATEPOS:
		////murmur("IME_NOTIFY: IMN_SETCANDIDATEPOS");
        if (!lpUIPriv->hCandWnd) {
            break;
        }
        GetWorkArea(&g_sImeUIG.rcWorkArea);

        PostMessage(lpUIPriv->hCandWnd, WM_IME_NOTIFY, wParam, lParam);
        break;
    case IMN_GUIDELINE:
		////murmur("IME_NOTIFY: IMN_GUIDELINE");
        ShowGuideLine(hUIWnd);
        break;
    case IMN_PRIVATE:
		////murmur("IME_NOTIFY: IMN_PRIVATE");
        switch (lParam) {
        case WM_IME_STARTCOMPOSITION:
			////murmur("IME_NOTIFY: IMN_PRIVATE:WM_IME_STARTCOMPOSITION");
            StartComp(hUIWnd);
            break;
        case WM_IME_COMPOSITION:
			////murmur("IME_NOTIFY: IMN_PRIVATE:WM_IME_COMPOSITION");
            OnComp(hUIWnd, lParam);
            break;
        case WM_IME_ENDCOMPOSITION:
			////murmur("IME_NOTIFY: IMN_PRIVATE:WM_IME_ENDCOMPOSITION");
            EndComp(hUIWnd);
            break;
        case IMN_PRIVATE_NONIME_TOGGLE_OFF:
            {
                HIMC hIMC;
                CONST INPUTCONTEXT * lpIMC;
                hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
                if (!hIMC) 
                    break;
                lpIMC = (CONST INPUTCONTEXT *)ImmLockIMC(hIMC);
                if (!lpIMC) 
                    break;

                lpUIPriv->bOpenStatus = FALSE;
                ImmSetOpenStatus(hIMC, FALSE);
                ShowUI(hUIWnd, SW_HIDE);
                ImmUnlockIMC(hIMC);
            }
            break;

        case IMN_PRIVATE_NONIME_TOGGLE_ON:
            {
                lpUIPriv->bOpenStatus = TRUE;
                ShowUI(hUIWnd, SW_SHOWNOACTIVATE);
            }
            break;

        case IMN_PRIVATE_SETACTIVECONTEXT:
            {
                HIMC hIMC;
                hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
                if (!hIMC) 
                    break;
                if (ImmGetOpenStatus(hIMC) && !lpUIPriv->bOpenStatus)
                    PostMessage(hUIWnd, WM_IME_NOTIFY, IMN_PRIVATE, IMN_PRIVATE_NONIME_TOGGLE_OFF);
            }
            break; 
        default:
            break;
        }
        break;
    case IMN_SOFTKBDDESTROYED:
        // WinCE implement soft keyboard in different way.
        break;
    default:
        break;
    }

    return 0L;
}

/**********************************************************************/
/* UIChange()                                                       */
/**********************************************************************/
LRESULT UIChange(HWND hUIWnd)
{
    LPCUIPRIV lpUIPriv;

    lpUIPriv = (LPCUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);

    if(!lpUIPriv) 
        return (0L);      

    if (lpUIPriv->fdwSetContext & ISC_SHOWUIALL) {
        ShowUI(hUIWnd, SW_SHOWNOACTIVATE);
    } else {
        ShowUI(hUIWnd, SW_HIDE);
    }

    return (0L);
}

/**********************************************************************/
/* OnUISetContext()                                                   */
/**********************************************************************/
static __inline
LRESULT OnUISetContext(HWND hUIWnd, BOOL fOn, LPARAM lShowUI)
{
    LPUIPRIV lpUIPriv;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);

    if(!lpUIPriv)
        return 0L;

    if (fOn) {
        lpUIPriv->fdwSetContext |= ISC_SHOWUIALL;
        SetTimer(hUIWnd, TIMER_UICHANGE, 200, NULL);
    } else {
        KillTimer(hUIWnd, TIMER_UICHANGE);
        lpUIPriv->fdwSetContext &= ~ISC_SHOWUIALL;
        UIChange(hUIWnd);
    }

    return 0L;
}

/**********************************************************************/
/* GetCandPos()                                                       */
/**********************************************************************/
LRESULT GetCandPos(HWND hUIWnd, LPCANDIDATEFORM lpCandForm)
{
    LPUIPRIV       lpUIPriv;
    RECT           rcCandWnd;
    HIMC           hIMC;
    LPINPUTCONTEXT lpIMC;

    if (lpCandForm->dwIndex != 0) {
        return (1L);
    }

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);

    if(!lpUIPriv)
        return (1L);      

    if (!lpUIPriv->hCandWnd) {
        return (1L);
    }

    if (!GetWindowRect(lpUIPriv->hCandWnd, &rcCandWnd)) {
        return (1L);
    }

    lpCandForm->dwStyle = CFS_CANDIDATEPOS;
    lpCandForm->ptCurrentPos = *(LPPOINT)&rcCandWnd;
    lpCandForm->rcArea = rcCandWnd;

    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);

    if (!hIMC) {
        return (1L);
    }

    lpIMC = (LPINPUTCONTEXT)ImmLockIMC(hIMC);

    if (!lpIMC) {
        return (1L);
    }

    ScreenToClient(lpIMC->hWnd, &lpCandForm->ptCurrentPos);

    lpCandForm->rcArea.right += lpCandForm->ptCurrentPos.x -
        lpCandForm->rcArea.left;

    lpCandForm->rcArea.bottom += lpCandForm->ptCurrentPos.y -
        lpCandForm->rcArea.top;

    *(LPPOINT)&lpCandForm->rcArea = lpCandForm->ptCurrentPos;

    ImmUnlockIMC(hIMC);

    return (0L);
}

/**********************************************************************/
/* OnUIControl()                                                      */
/**********************************************************************/
static __inline
LRESULT OnUIControl(HWND hUIWnd, WPARAM wParam, LPARAM lParam)
{
    switch (wParam) {
    case IMC_GETCANDIDATEPOS:
        return GetCandPos(hUIWnd, (LPCANDIDATEFORM)lParam);
    case IMC_GETCOMPOSITIONWINDOW:
    case IMC_GETSOFTKBDPOS:
    case IMC_SETSOFTKBDPOS:
        return (0L);
    case IMC_GETSTATUSWINDOWPOS:
        {
            HWND   hStatusWnd;
            RECT   rcStatusWnd;
    
            hStatusWnd = GetStatusWnd(hUIWnd);
            if (!hStatusWnd) {
                return (0L); 
            }
    
            if (!GetWindowRect(hStatusWnd, &rcStatusWnd)) {
                 return (0L);    
            }
    
            lParam = MAKELRESULT(rcStatusWnd.left, rcStatusWnd.top);
        }
        return (lParam);

    case IMC_SETSTATUSWINDOWPOS:
        {
            HIMC            hIMC;
            POINT           ptPos;

            ptPos.x = ((LPPOINTS)&lParam)->x;
            ptPos.y = ((LPPOINTS)&lParam)->y;

            hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
            if (!hIMC) {
                return (1L);
            }

            if(!ImmSetStatusWindowPos(hIMC, &ptPos)) {
                return (1L);
            }

        }
        return (0L);

    default:
        return (1L);
    }
}

/**********************************************************************/
/* UIWndProc()                                                        */
/**********************************************************************/
LRESULT CALLBACK ImeUIWndProc(HWND   hUIWnd,
                              UINT   uMsg,
                              WPARAM wParam,
                              LPARAM lParam)
{
//#ifdef __SHOWIMEMSG_HPP_
    ShowImeMessage(hUIWnd, uMsg, wParam, lParam);
//#endif
    switch (uMsg) {
    case WM_CREATE:
		////murmur("IMEUIWND:WM_CREATE");
        return OnUICreate(hUIWnd, (LPCREATESTRUCT)lParam);
    case WM_DESTROY:
		////murmur("IMEUIWND:WM_Destroy");
        return OnUIDestroy(hUIWnd);
    case WM_IME_STARTCOMPOSITION:
		////murmur("IMEUIWND:WM_IME_STARTCOMPOSITION");
        StartComp(hUIWnd);
        break;
    case WM_IME_COMPOSITION:
		////murmur("IMEUIWND:WM_IME_COMPOSITION");
        OnComp(hUIWnd, lParam);
        break;
    case WM_IME_ENDCOMPOSITION:
		////murmur("IMEUIWND:WM_IME_ENDCOMPOSITION");
        EndComp(hUIWnd);
        break;
    case WM_IME_NOTIFY:
		////murmur("IMEUIWND:WM_NOTIFY");
        return OnUINotify(hUIWnd, wParam, lParam);
    case WM_IME_SETCONTEXT:
		////murmur("IMEUIWND:WM_IME_SETCONTEXT");
        return OnUISetContext(hUIWnd, (BOOL)wParam, lParam);
    case WM_IME_CONTROL:
		////murmur("IMEUIWND:WM_IME_CONTROL");
        return OnUIControl(hUIWnd, wParam, lParam);
    case WM_IME_COMPOSITIONFULL:
		////murmur("IMEUIWND:WM_IME_COMPOSITIONFULL");
        return (0L);
    case WM_IME_SELECT:
		////murmur("IMEUIWND:WM_IME_SELET");
        return (0L);
    case WM_TIMER:
		////murmur("IMEUIWND:WM_TIMER");
        switch(wParam) {
        case TIMER_UICHANGE:
            KillTimer(hUIWnd, wParam);
            return UIChange(hUIWnd);
        default:
            break;
        }
        return (1L);
    case WM_SETTINGCHANGE:
        {
			////murmur("IMEUIWND:WM_SETTINGCHANGE");
            LPUIPRIV    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
            if (!lpUIPriv)
                break;
    
            if (wParam == SETTINGCHANGE_RESET) {
                if (lpUIPriv->hCandWnd)    {
                    if ((lpUIPriv->fdwSetContext & ISC_SHOWUICANDIDATEWINDOW) &&
                        (lpUIPriv->fdwUIFlags & UI_CAND_ALREADY_OPEN)) {
                        if (IsWindowVisible(lpUIPriv->hCandWnd)) { 
                            HKL hKL;
                            if (!ImmGetHotKey( IME_CHOTKEY_DISMISS, &CandDismiss.uMod, &CandDismiss.uVKey, &hKL)) {
                                CandDismiss.uVKey = VK_ESCAPE;
                                CandDismiss.uMod = 0x0400;
                            }
                            PressHotKey(CandDismiss);
                        }
                    }
                }
            }
        }
        break;
    default:
        return DefWindowProc(hUIWnd, uMsg, wParam, lParam);
    }

    return (0L);
}

