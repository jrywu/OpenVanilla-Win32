//#define OV_DEBUG
#include "OVIME.h"
#include <imm.h>
#include <sipapi.h>
#include "UIPrivate.h"


#define IMN_PRIVATE_NONIME_TOGGLE_ON       0x000A
#define IMN_PRIVATE_NONIME_TOGGLE_OFF      0x000B
#define IMN_PRIVATE_SETACTIVECONTEXT       0x000C




int UICurrentInputMethod()
{
	return CurrentIC;
}


//
// OnUICreate()                                                       
//
static __inline LRESULT OnUICreate(HWND hUIWnd, LPCREATESTRUCT lpCS)
{
    LPUIPRIV    lpUIPriv;

    lpUIPriv = (LPUIPRIV)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(UIPRIV));
    if (!lpUIPriv) { return -1L; }

    memset(lpUIPriv, 0, sizeof(UIPRIV));


//    lpUIPriv->dwUIMoveOffset = WINDOW_NOT_DRAG;
    lpUIPriv->bOpenStatus = TRUE;

    SetWindowLong(hUIWnd, IMMGWL_PRIVATE, (LONG)lpUIPriv);

    lpUIPriv->hInst = g_hInst; 
    return 0L;
}

//
// OnUIDestroy()                                                        
//
static __inline LRESULT OnUIDestroy(HWND hUIWnd)
{
    LPUIPRIV lpUIPriv;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);

    if(!lpUIPriv) 
        return 0L;

       // candidate window need to be destroyed
    if (lpUIPriv->hCandWnd) {
        DestroyWindow(lpUIPriv->hCandWnd);
        lpUIPriv->hCandWnd = NULL;
    }

    HeapFree(GetProcessHeap(), 0, (LPVOID)lpUIPriv);

    SetWindowLong(hUIWnd, IMMGWL_PRIVATE, (LONG)0);

    return 0L;
}

//
// ShowUI() : show the sub windows                                    
/*/
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
        ShowCand(hUIWnd, nShowCmd);

        return;
    }

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    
    if(!lpUIPriv) 
        return;

   


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
		murmur("IME_NOTIFY: IMN_OPENSTATUSWINDOW");
        break;
	case IMN_CLOSESTATUSWINDOW:{
		murmur("IME_NOTIFY: IMN_CLOSESTATUSWINDOW");
		AVLoader* loader = AVLoader::open();
		loader->unloadCurrentModule();
		CloseCand(hUIWnd);
		murmur("IME_NOTIFY: IMN_CLOSESTATUSWINDOW:Closecand()");
		break;
		}
    case IMN_SETSTATUSWINDOWPOS:
		murmur("IME_NOTIFY: IMN_SETSTATUSWINDOWPOS");
        break;
	case IMN_OPENCANDIDATE:{
		murmur("IME_NOTIFY: IMN_OPENCANDIDATE");
        OpenCand(hUIWnd);
		break;
		}
    case IMN_CHANGECANDIDATE:
		murmur("IME_NOTIFY: IMN_CHANGECANDIDATE");
        ChangeCand(hUIWnd);
        break;
	case IMN_CLOSECANDIDATE:{
		murmur("IME_NOTIFY: IMN_CLOSECANDIDATE");
		CloseCand(hUIWnd);
        break;
		}
    case IMN_SETSENTENCEMODE:
		murmur("IME_NOTIFY: IMN_SETSENTENCEMODE");
        break;
	case IMN_SETOPENSTATUS:{
		murmur("IME_NOTIFY: IMN_SETOPENSTATUS");
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
                //ShowUI(hUIWnd, SW_SHOWNOACTIVATE);
				murmur("ImmGetOpenStatus:TRUE");
            }
            else {
				murmur("ImmGetOpenStatus:FALSE");
                lpUIPriv->bOpenStatus = FALSE;
                //ShowUI(hUIWnd, SW_HIDE);
				AVLoader* loader = AVLoader::open();
				loader->unloadCurrentModule();
				CloseCand(hUIWnd);
            }
            
            ImmUnlockIMC(hIMC);
        break;
		}
    case IMN_SETCONVERSIONMODE:
		murmur("IME_NOTIFY: IMN_SETCONVERSIONMODE");
        /*if (!g_sImeUIG.bNoStatus) {
            HIMC hIMC;
            CONST INPUTCONTEXT * lpIMC;
            hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
            if (!hIMC) 
                break;
            lpIMC = (CONST INPUTCONTEXT *)ImmLockIMC(hIMC);
            if (!lpIMC) 
                break;

            murmur("OnUINotify()::lpIMC->fdwConversion = 0x%x\n", lpIMC->fdwConversion);
            ImmUnlockIMC(hIMC);
            InvalidateRect(lpUIPriv->hStatusWnd, NULL, NULL);
        }*/
        break;

    case IMN_SETCOMPOSITIONFONT:
		murmur("IME_NOTIFY: IMN_SETCOMPOSITIONFONT");
        // we are not going to change font, but an IME can do this if it want
        break;
    case IMN_SETCOMPOSITIONWINDOW:
		murmur("IME_NOTIFY: IMN_SETCOMPOSITIONWINDOW");
        break;
    case IMN_SETCANDIDATEPOS:
		murmur("IME_NOTIFY: IMN_SETCANDIDATEPOS");
        if (!lpUIPriv->hCandWnd) {
            break;
        }
        //GetWorkArea(&g_sImeUIG.rcWorkArea);
        PostMessage(lpUIPriv->hCandWnd, WM_IME_NOTIFY, wParam, lParam);
        break;
    case IMN_GUIDELINE:
		murmur("IME_NOTIFY: IMN_GUIDELINE");
        //ShowGuideLine(hUIWnd);
        break;
    case IMN_PRIVATE:
		murmur("IME_NOTIFY: IMN_PRIVATE");
        switch (lParam) {
        case WM_IME_STARTCOMPOSITION:
			murmur("IME_NOTIFY: IMN_PRIVATE:WM_IME_STARTCOMPOSITION");
            //StartComp(hUIWnd);
            break;
        case WM_IME_COMPOSITION:
			murmur("IME_NOTIFY: IMN_PRIVATE:WM_IME_COMPOSITION");
            //OnComp(hUIWnd, lParam);
            break;
        case WM_IME_ENDCOMPOSITION:
			murmur("IME_NOTIFY: IMN_PRIVATE:WM_IME_ENDCOMPOSITION");
            //EndComp(hUIWnd);
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
				AVLoader* loader = AVLoader::open();
				loader->unloadCurrentModule();
				CloseCand(hUIWnd);
                //ShowUI(hUIWnd, SW_HIDE);
                ImmUnlockIMC(hIMC);
            }
            break;

        case IMN_PRIVATE_NONIME_TOGGLE_ON:
            {
                lpUIPriv->bOpenStatus = TRUE;
                //ShowUI(hUIWnd, SW_SHOWNOACTIVATE);
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
        //ShowUI(hUIWnd, SW_SHOWNOACTIVATE);
    } else {
        //ShowUI(hUIWnd, SW_HIDE);
		CloseCand(hUIWnd);
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
        return (lParam);
    case IMC_SETSTATUSWINDOWPOS:      
        return (0L);

    default:
        return (1L);
    }
}

/**********************************************************************/
/* UIWndProc()                                                        */
/**********************************************************************/
LRESULT APIENTRY UIWndProc(HWND   hUIWnd,  UINT   uMsg, WPARAM wParam,  LPARAM lParam)
{

    switch (uMsg) {
    case WM_CREATE:
		murmur("IMEUIWND:WM_CREATE");
        return OnUICreate(hUIWnd, (LPCREATESTRUCT)lParam);
    case WM_DESTROY:
		murmur("IMEUIWND:WM_Destroy");
        return OnUIDestroy(hUIWnd);
    case WM_IME_STARTCOMPOSITION:
		murmur("IMEUIWND:WM_IME_STARTCOMPOSITION");
        //StartComp(hUIWnd);
        break;
    case WM_IME_COMPOSITION:
		murmur("IMEUIWND:WM_IME_COMPOSITION");
        //OnComp(hUIWnd, lParam);
        break;
    case WM_IME_ENDCOMPOSITION:
		murmur("IMEUIWND:WM_IME_ENDCOMPOSITION");
        //EndComp(hUIWnd);
        break;
    case WM_IME_NOTIFY:
		murmur("IMEUIWND:WM_NOTIFY");
        return OnUINotify(hUIWnd, wParam, lParam);
    case WM_IME_SETCONTEXT:
		murmur("IMEUIWND:WM_IME_SETCONTEXT");
        return OnUISetContext(hUIWnd, (BOOL)wParam, lParam);
    case WM_IME_CONTROL:
		murmur("IMEUIWND:WM_IME_CONTROL");
        return OnUIControl(hUIWnd, wParam, lParam);
    case WM_IME_COMPOSITIONFULL:
		murmur("IMEUIWND:WM_IME_COMPOSITIONFULL");
        return (0L);
    case WM_IME_SELECT:
		murmur("IMEUIWND:WM_IME_SELET");
        return (0L);
    case WM_TIMER:
		murmur("IMEUIWND:WM_TIMER");
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
			murmur("IMEUIWND:WM_SETTINGCHANGE");
			/*
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
			*/
        }
        break;
    default:
        return DefWindowProc(hUIWnd, uMsg, wParam, lParam);
    }

    return (0L);
}

