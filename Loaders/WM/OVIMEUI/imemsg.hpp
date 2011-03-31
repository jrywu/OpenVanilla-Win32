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
#ifndef __SHOWIMEMSG_HPP_
#define __SHOWIMEMSG_HPP_


/**********************************************************************/
/* ShowImeMessage()                                                   */
/**********************************************************************/
__inline
void ShowImeMessage(HWND hUIWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
    case WM_CREATE:
        DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_CREATE, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
        break;
    case WM_DESTROY:
        DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_DESTROY, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
        break;
    case WM_IME_STARTCOMPOSITION:
        DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_STARTCOMPOSITION, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
        break;
    case WM_IME_ENDCOMPOSITION:
        DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_ENDCOMPOSITION, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
        break;
    case WM_IME_COMPOSITION:
        DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_COMPOSITION, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
        break;
    case WM_IME_SETCONTEXT:
        DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_SETCONTEXT, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
        break;
    case WM_IME_NOTIFY:
        switch(wParam) {
        case IMN_CLOSESTATUSWINDOW:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_CLOSESTATUSWINDOW, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMN_OPENSTATUSWINDOW:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_OPENSTATUSWINDOW, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMN_SETCONVERSIONMODE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_SETCONVERSIONMODE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMN_SETCOMPOSITIONFONT:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_SETCOMPOSITIONFONT, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMN_SETOPENSTATUS:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_SETOPENSTATUS, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMN_OPENCANDIDATE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_OPENCANDIDATE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMN_CHANGECANDIDATE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_CHANGECANDIDATE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMN_CLOSECANDIDATE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_CLOSECANDIDATE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMN_GUIDELINE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_GUIDELINE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMN_SETCANDIDATEPOS:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_SETCANDIDATEPOS, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMN_SETCOMPOSITIONWINDOW:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_SETCOMPOSITIONWINDOW, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMN_SETSENTENCEMODE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_SETSENTENCEMODE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMN_SETSTATUSWINDOWPOS:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_SETSTATUSWINDOWPOS, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMN_PRIVATE:
            switch(lParam) {
            case WM_IME_STARTCOMPOSITION:
                DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_PRIVATE, WM_IME_STARTCOMPOSITION\r\n"), hUIWnd));
                break;
            case WM_IME_COMPOSITION:
                DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_PRIVATE, WM_IME_COMPOSITION\r\n"), hUIWnd));
                break;
            case WM_IME_ENDCOMPOSITION:
                DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_PRIVATE, WM_IME_ENDCOMPOSITION\r\n"), hUIWnd));
                break;
            default:
                DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_NOTIFY, IMN_PRIVATE, lParam=%x\r\n"), hUIWnd, lParam));
                break;
            }
            break;
        default:
            break;
        }
        break;
    case WM_IME_CONTROL:
        switch(wParam) {
        case IMC_GETCONVERSIONMODE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_CONTROL, IMC_GETCONVERSIONMODE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMC_SETCONVERSIONMODE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_CONTROL, IMC_SETCONVERSIONMODE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMC_GETSENTENCEMODE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_CONTROL, IMC_GETSENTENCEMODE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMC_SETSENTENCEMODE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_CONTROL, IMC_SETSENTENCEMODE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMC_GETOPENSTATUS:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_CONTROL, IMC_GETOPENSTATUS, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMC_SETOPENSTATUS:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_CONTROL, IMC_SETOPENSTATUS, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMC_GETSOFTKBDFONT:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_CONTROL, IMC_GETSOFTKBDFONT, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMC_SETSOFTKBDFONT:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_CONTROL, IMC_SETSOFTKBDFONT, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMC_GETSOFTKBDPOS:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_CONTROL, IMC_GETSOFTKBDPOS, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMC_SETSOFTKBDPOS:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_CONTROL, IMC_SETSOFTKBDPOS, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMC_GETSOFTKBDSUBTYPE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_CONTROL, IMC_GETSOFTKBDSUBTYPE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMC_SETSOFTKBDSUBTYPE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_CONTROL, IMC_SETSOFTKBDSUBTYPE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMC_SETSOFTKBDDATA:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_CONTROL, IMC_SETSOFTKBDDATA, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        default:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_CONTROL, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
            break;
        }
        break;
    case WM_IME_COMPOSITIONFULL:
        DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, IME_COMPOSITIONFULL, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
        break;
    case WM_IME_SELECT:
        DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SELECT, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
        break;
    case WM_IME_CHAR:
        DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_CHAR, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
        break;
    case WM_IME_SYSTEM:
        switch(wParam){
        case IMS_DESTROYWINDOW:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_DESTROYWINDOW, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_IME31COMPATIBLE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_IME31COMPATIBLE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_SETOPENSTATUS:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_SETOPENSTATUS, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_SETACTIVECONTEXT:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_SETACTIVECONTEXT, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_CHANGE_SHOWSTAT:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_CHANGE_SHOWSTAT, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_WINDOWPOS:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_WINDOWPOS, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_SENDIMEMSG:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_SENDIMEMSG, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_SENDIMEMSGEX:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_SENDIMEMSGEX, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_SETCANDIDATEPOS:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_SETCANDIDATEPOS, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_SETCOMPOSITIONFONT:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_SETCOMPOSITIONFONT, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_SETCOMPOSITIONWINDOW:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_SETCOMPOSITIONWINDOW, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_CHECKENABLE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_CHECKENABLE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_CONFIGUREIME:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_CONFIGUREIME, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_CONTROLIMEMSG:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_CONTROLIMEMSG, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_SETOPENCLOSE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_SETOPENCLOSE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_ISACTIVATED:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_ISACTIVATED, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_UNLOADTHREADLAYOUT:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_UNLOADTHREADLAYOUT, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_LCHGREQUEST:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_LCHGREQUEST, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_SETSOFTKBDONOFF:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_SETSOFTKBDONOFF, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_GETCONVERSIONMODE:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_GETCONVERSIONMODE, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_IMEHELP:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_IMEHELP, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_IMENT35SENDAPPMSG:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_IMENT35SENDAPPMSG, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_ACTIVATECONTEXT:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_ACTIVATECONTEXT, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_DEACTIVATECONTEXT:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_DEACTIVATECONTEXT, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_ACTIVATETHREADLAYOUT:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_ACTIVATETHREADLAYOUT, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_CLOSEPROPERTYWINDOW:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_CLOSEPROPERTYWINDOW, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_OPENPROPERTYWINDOW:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_OPENPROPERTYWINDOW, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_GETIMEMENU:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_GETIMEMENU, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_ENDIMEMENU:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_ENDIMEMENU, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMS_SETCONVERSIONSTATUS:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, IMS_SETCONVERSIONSTATUS, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        default:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_SYSTEM, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
            break;
        }
        break;
    case WM_IME_REQUEST:
        switch(wParam) {
        case IMR_COMPOSITIONWINDOW:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_REQUEST, IMR_COMPOSITIONWINDOW, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMR_CANDIDATEWINDOW:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_REQUEST, IMR_CANDIDATEWINDOW, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMR_COMPOSITIONFONT:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_REQUEST, IMR_COMPOSITIONFONT, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMR_RECONVERTSTRING:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_REQUEST, IMR_RECONVERTSTRING, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMR_CONFIRMRECONVERTSTRING:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_REQUEST, IMR_CONFIRMRECONVERTSTRING, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMR_QUERYCHARPOSITION:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_REQUEST, IMR_QUERYCHARPOSITION, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        case IMR_DOCUMENTFEED:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_REQUEST, IMR_DOCUMENTFEED, lParam=%x\r\n"), hUIWnd, lParam));
            break;
        default:
            DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_REQUEST, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
            break;
        }
        break;
    case WM_IME_KEYDOWN:
        DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_KEYDOWN, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
        break;
    case WM_IME_KEYUP:
        DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_IME_KEYUP, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
        break;
    case WM_SETTINGCHANGE:
        DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_SETTINGCHANGE, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
        break;
    case WM_HELP:
        DEBUGMSG(TRUE, (TEXT("ImeMsg: hUIWnd=%x, WM_HELP, wParam=%x, lParam=%x\r\n"), hUIWnd, wParam, lParam));
        break;
    default:
        break;
    }
}


#endif // __SHOWIMEMSG_HPP_
