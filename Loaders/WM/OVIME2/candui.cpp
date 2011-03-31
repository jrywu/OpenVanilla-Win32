//#define OV_DEBUG
#include "ovime.h"

#include "uiprivate.h"
#include <sipapi.h>

//#include "windev.h"
#include <kfuncs.h>


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
        murmur("GetWorkAreaFromPPCShell() rcVisibleDesktop = (%d, %d - %d, %d)\n", 
            pssi->rcVisibleDesktop.left, pssi->rcVisibleDesktop.top, 
            pssi->rcVisibleDesktop.right, pssi->rcVisibleDesktop.bottom);
        murmur("GetWorkAreaFromPPCShell() fdwFlags & SIPF_ON = %d\n", 
            pssi->fdwFlags & SIPF_ON);
            
		if(!(pssi->fdwFlags & SIPF_ON)) pssi->rcVisibleDesktop.bottom-= pssi->rcVisibleDesktop.top;
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
		murmur("rcWorkArea.top=%d; psi->rcWorkArea.bottom=%d\n",lprcWorkArea->top, lprcWorkArea->bottom);
		murmur("rcWorkArea.right=%d; psi->rcWorkArea.left=%d\n",lprcWorkArea->right, lprcWorkArea->left);

}


BOOL IsSIPOn()
{
    SIPINFO  sSipInfo;
    sSipInfo.cbSize = sizeof(SIPINFO);
    sSipInfo.dwImDataSize = 0;
    sSipInfo.pvImData = NULL;
    if (SipGetInfo(&sSipInfo) && (sSipInfo.fdwFlags & SIPF_ON))
        return TRUE;
    return FALSE;
}

//
// AdjustCandWnd()                                                    
//
void AdjustCandWnd(HWND hCandWnd,  LPUIPRIV        lpUIPriv,  LPINPUTCONTEXT  lpIMC)
{
    //LPUIPRIV        lpUIPriv;
	POINT ptNew;
    

	GetWorkArea(&g_sImeUIG.rcWorkArea);
	
    if (GetCaretPos(&ptNew)){
        ClientToScreen(lpIMC->hWnd, &ptNew);
        murmur("AdjustCandWnd() ptNew.x = %d, ptNew.y = %d, rcWorkArea.bottom = %d, nCandHi=%d\n", ptNew.x, ptNew.y, g_sImeUIG.rcWorkArea.bottom, lpUIPriv->nCandHi);
        if(  ( g_sImeUIG.rcWorkArea.bottom - ptNew.y < 2*lpUIPriv->nCandHi ) && 
			 ( g_sImeUIG.rcWorkArea.bottom - ptNew.y >0 ) &&
				ptNew.y < g_sImeUIG.rcWorkArea.bottom ){
             ptNew.y -= lpUIPriv->nCandHi;
			
        } else 
        ptNew.y = g_sImeUIG.rcWorkArea.bottom - lpUIPriv->nCandHi;
    } else 
		ptNew.y = g_sImeUIG.rcWorkArea.bottom - lpUIPriv->nCandHi;
        
    
    ptNew.x = 0;

    murmur("AdjustCandWnd() ptNew.x = %d, ptNew.y = %d\n", ptNew.x, ptNew.y);
        
   

    InvalidateRect(hCandWnd, NULL, FALSE);

	lpUIPriv->rcCandText.right = g_sImeUIG.rcWorkArea.right -g_sImeUIG.nCandCharWi *2;
	
    if (ptNew.x != lpUIPriv->ptCand.x ||
        ptNew.y != lpUIPriv->ptCand.y ||
		g_sImeUIG.rcWorkArea.right != lpUIPriv->nCandWi
		) {
        lpUIPriv->ptCand = ptNew;
		lpUIPriv->nCandWi = g_sImeUIG.rcWorkArea.right;
		SetWindowPos(hCandWnd, NULL, ptNew.x, ptNew.y, g_sImeUIG.rcWorkArea.right, g_sImeUIG.nCandLineHi,
				SWP_NOACTIVATE|SWP_NOZORDER);
    }

    return;
}

/**********************************************************************/
/* OnCandCreate()                                                     */
/**********************************************************************/
static __inline LRESULT OnCandCreate(HWND hCandWnd, CONST CREATESTRUCT * lpCS)
{
    HWND     hUIWnd;
    LPUIPRIV lpUIPriv;
    HIMC     hIMC;

    hUIWnd = GetWindow(hCandWnd, GW_OWNER);
    if (!hUIWnd) {
        return -1L;
    }

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return -1L;
    }
    SetWindowLong(hCandWnd, IMMGWL_PRIVATE, (LONG)lpUIPriv);

    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
    if (!lpUIPriv) {
        return -1L;
    }
    SetWindowLong(hCandWnd, IMMGWL_IMC, (LONG)hIMC);

	GetWorkArea(&g_sImeUIG.rcWorkArea);

   
    
    lpUIPriv->ptCand.x = 0;// lpCS->x;
    lpUIPriv->ptCand.y = 0;//lpCS->y;

	lpUIPriv->nCandWi = g_sImeUIG.rcWorkArea.right; //lpCS->cx;
    lpUIPriv->nCandHi = g_sImeUIG.nCandLineHi ;//lpCS->cy;

    lpUIPriv->rcCandText.left = g_sImeUIG.nCandCharWi*2; //space for left arrow
    lpUIPriv->rcCandText.top =  0;//g_sImeUIG.rcWorkArea.bottom - g_sImeUIG.nCandLineHi;
    lpUIPriv->rcCandText.right =  g_sImeUIG.rcWorkArea.right - 2* g_sImeUIG.nCandCharWi;// space for right arrow
    lpUIPriv->rcCandText.bottom = g_sImeUIG.nCandLineHi;//g_sImeUIG.rcWorkArea.bottom;


    return 0L;
}

/**********************************************************************/
/* OnCandDestroy()                                                    */
/**********************************************************************/
static __inline LRESULT OnCandDestroy(HWND hCandWnd)
{
    LPUIPRIV lpUIPriv;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hCandWnd, IMMGWL_PRIVATE);

    lpUIPriv->hCandWnd = (HWND)NULL;

    if (g_sImeUIG.hFont) {
        DeleteObject(g_sImeUIG.hFont);
        g_sImeUIG.hFont = NULL;
    }

    return 0L;
}

/**********************************************************************/
/* DrawCandLine()                                                     */
/**********************************************************************/
void DrawCandLine(HDC hDC, LPUIPRIV lpUIPriv, DWORD dwIndex, BOOL fSelected)
{
    LPCANDIDATELIST lpCandList = lpUIPriv->lpCandList;
    DWORD           dwStart, dwEnd;
    int             i;
    RECT            rcCandLine;
	LPCTSTR			szCandStr;


    dwStart = lpCandList->dwPageStart;

    dwEnd = dwStart + lpCandList->dwPageSize;
    dwEnd = dwEnd > lpCandList->dwCount ? lpCandList->dwCount : dwEnd;

    if (dwIndex > dwEnd) {
        return;
    }

    i = dwIndex - dwStart;
        
    if (fSelected) {
        SetBkColor(hDC, g_sImeUIG.crHighlight);
        SetTextColor(hDC, g_sImeUIG.crHighlightText);
    } else {
        SetBkColor(hDC, g_sImeUIG.crWindow);
        SetTextColor(hDC, g_sImeUIG.crWindowText);
    }
   szCandStr = (LPCTSTR)((LPBYTE)lpCandList + lpCandList->dwOffset[dwIndex]);

	rcCandLine.left = g_sImeUIG.nCandDx[i]; //lpUIPriv->rcCandText.left;
	rcCandLine.right = g_sImeUIG.nCandDx[i] + wcslen(szCandStr) * g_sImeUIG.nCandCharWi ; ;//lpUIPriv->rcCandText.right;


    ExtTextOut(hDC, g_sImeUIG.nCandDx[i], g_sImeUIG.nCandDy,
               ETO_OPAQUE, &rcCandLine, szCandStr, 1, NULL);

    

    return;
}

/**********************************************************************/
/* DrawCandText()                                                     */
/**********************************************************************/
void DrawCandText(HDC hDC, LPCUIPRIV lpUIPriv)
{
    LPCANDIDATELIST lpCandList = lpUIPriv->lpCandList;
    DWORD           dwStart, dwEnd, dwSelect;
    int             i;
    RECT            rcCandLine;
	LPCWSTR         szCandStr;
	LPCWSTR			szRScroll=_T(">>"), szLScroll=_T("<<");
	   

    dwStart = lpCandList->dwPageStart;

    dwEnd = dwStart +  32; //MAX_PAGE =32//lpCandList->dwPageSize;
    dwEnd = dwEnd > lpCandList->dwCount ? lpCandList->dwCount : dwEnd;

    dwSelect = lpCandList->dwSelection;


    SetBkColor(hDC, g_sImeUIG.crWindow);
    SetTextColor(hDC, g_sImeUIG.crWindowText);

	
	rcCandLine.top = 0;
	rcCandLine.bottom = g_sImeUIG.nCandLineHi;
	rcCandLine.left = 0;
	rcCandLine.right = g_sImeUIG.rcWorkArea.right;
	g_sImeUIG.nCandDx[0] = lpUIPriv->rcCandText.left; // Reserved space for left arrow
    
    
	
	if(lpCandList->dwPageStart !=0)
		ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rcCandLine, szLScroll, 2, NULL);  // Draw left scroll symbol and fill background
	else 
		ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rcCandLine, NULL, 0, NULL);  // Fill background color of candtext area.

  
    for (i = 0; dwStart < dwEnd; dwStart++, i++) {

		szCandStr =  GETLPCANDSTR(lpCandList, dwStart);// *(LPCTSTR)((LPBYTE)lpCandList + lpCandList->dwOffset[dwStart]);
		int rlen = wcslen(szCandStr);
		g_sImeUIG.nCandDx[i+1] = g_sImeUIG.nCandDx[i] + (rlen + 1)  * g_sImeUIG.nCandCharWi;

		if(g_sImeUIG.nCandDx[i+1] > g_sImeUIG.rcWorkArea.right - g_sImeUIG.nCandCharWi*2) 
			break;
		
	
        if (dwSelect == dwStart) {
			rcCandLine.left = g_sImeUIG.nCandDx[i];
            rcCandLine.right =  g_sImeUIG.nCandDx[i] + rlen  * g_sImeUIG.nCandCharWi;
            SetBkColor(hDC, g_sImeUIG.crHighlight);
            SetTextColor(hDC, g_sImeUIG.crHighlightText);
        } else {
			rcCandLine.left = g_sImeUIG.nCandDx[i]; 
			rcCandLine.right =  g_sImeUIG.nCandDx[i] + rlen  * g_sImeUIG.nCandCharWi;
        }
		
        ExtTextOut(hDC, g_sImeUIG.nCandDx[i],  0,
                  ETO_OPAQUE, &rcCandLine, szCandStr, rlen, NULL);
 
        if (dwSelect == dwStart) {
            SetBkColor(hDC, g_sImeUIG.crWindow);
            SetTextColor(hDC, g_sImeUIG.crWindowText);
        }
    }
	
	lpCandList->dwPageSize = i;

	if(lpCandList->dwPageStart + lpCandList->dwPageSize != lpCandList->dwCount) {
		rcCandLine.left =  g_sImeUIG.rcWorkArea.right -  g_sImeUIG.nCandCharWi*2;
		rcCandLine.right =  g_sImeUIG.rcWorkArea.right;
		ExtTextOut(hDC, rcCandLine.left, 0, ETO_OPAQUE, &rcCandLine, szRScroll, 2, NULL);  // Fill background color of candtext area.
	}


    return;
}

/**********************************************************************/
/* DrawCandWnd()                                                      */
/**********************************************************************/
void DrawCandWnd(HWND hCandWnd)
{
    LPCUIPRIV lpUIPriv;
    HDC       hDC;

    lpUIPriv = (LPCUIPRIV)GetWindowLong(hCandWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return;
    }
    if (!lpUIPriv->lpCandList) {
        return;
    }

    hDC = GetDC(hCandWnd);
    DrawCandText(hDC, lpUIPriv);
   
    ReleaseDC(hCandWnd, hDC);
}

/**********************************************************************/
/* OnCandPaint()                                                      */
/**********************************************************************/
static __inline
LRESULT OnCandPaint(HWND hCandWnd)
{
    LPCUIPRIV       lpUIPriv;
    HDC             hDC;
    PAINTSTRUCT     ps;

    lpUIPriv = (LPCUIPRIV)GetWindowLong(hCandWnd, IMMGWL_PRIVATE);
    
    if (!lpUIPriv->lpCandList) {
        return -1L;
    }

	hDC = BeginPaint(hCandWnd, (LPPAINTSTRUCT)&ps);
    HFONT hOldFont = (HFONT)SelectObject(hDC, g_sImeUIG.hFont);
    DrawCandText(hDC, lpUIPriv); 
	SelectObject(hDC, hOldFont);
    EndPaint(hCandWnd, (LPPAINTSTRUCT)&ps);

    return 0L;
}

/**********************************************************************/
/* CandNotifyIME()                                                    */
/**********************************************************************/
void CandNotifyIME(HWND hCandWnd, WORD wCode)
{
    LPCUIPRIV lpUIPriv;
    HIMC      hIMC;

    lpUIPriv = (LPCUIPRIV)GetWindowLong(hCandWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return;
    }
    if (!lpUIPriv->lpCandList) {
        return;
    }
    
    hIMC = (HIMC)GetWindowLong(hCandWnd, IMMGWL_IMC);
    if (!hIMC) {
        return;
    }

    switch(wCode) {
    case CLN_SELECTION:
        ImmNotifyIME(hIMC, NI_SELECTCANDIDATESTR, 0L,
                     lpUIPriv->lpCandList->dwSelection);
        break;
    case CLN_DETERMINE:
        ImmNotifyIME(hIMC, NI_SELECTCANDIDATESTR, 0L,
                     lpUIPriv->lpCandList->dwSelection);
        ImmEscape((HKL)NULL, hIMC, IME_ESC_PRIVATE_FIRST, NULL);
        break;
    case CLN_PAGESTART:
        ImmNotifyIME(hIMC, NI_SETCANDIDATE_PAGESTART, 0L,
                     lpUIPriv->lpCandList->dwPageStart);
        break;
    default:
        break;
    }

    return;
}

/**********************************************************************/
/* OnCandSetCursor()                                                  */
/**********************************************************************/
static __inline LRESULT OnCandSetCursor(HWND hCandWnd, UINT uMsg, LPARAM lParam)
{
    LPUIPRIV        lpUIPriv;
    LPCANDIDATELIST lpCandList;
    POINT            ptCursor;
    DWORD           dwSelection;
    HDC             hDC;
	int				i;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hCandWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return -1L;
    }
    
    lpCandList = lpUIPriv->lpCandList;
    if (!lpCandList) {
        return -1L;
    }
    
    ptCursor.x = LOWORD(lParam);
    ptCursor.y = HIWORD(lParam);

	//Determine selected item acoording to mouse cursor  coordinate
 
	if(ptCursor.x < g_sImeUIG.nCandDx[0]){
		// page-up
		lpCandList->dwStyle = 4;
	}
	else if(ptCursor.x > lpUIPriv->rcCandText.right ) {
		// page-down
		lpCandList->dwStyle = 3;
	}
	else{
		for(i=0; i< 32; i++){
			if(ptCursor.x > g_sImeUIG.nCandDx[i] && ptCursor.x < g_sImeUIG.nCandDx[i+1])
				break;
		}
	}
	dwSelection = lpCandList->dwPageStart + i;

    if (dwSelection > lpCandList->dwCount -1)
        dwSelection = lpCandList->dwCount -1;

    hDC = GetDC(hCandWnd);

    switch (uMsg) {
    case WM_MOUSEMOVE:
        if (!PtInRect(&lpUIPriv->rcCandText, ptCursor)) {
            break;
        }

        if (dwSelection != lpCandList->dwSelection) {
            DrawCandLine(hDC, lpUIPriv, lpCandList->dwSelection, FALSE);
            lpCandList->dwSelection = dwSelection;
            DrawCandLine(hDC, lpUIPriv, lpCandList->dwSelection, TRUE);

        }
        break;
    case WM_LBUTTONDOWN:
        if (!PtInRect(&lpUIPriv->rcCandText, ptCursor)) {
            break;
        }

        SetCapture(hCandWnd);
        if (dwSelection != lpCandList->dwSelection) {
            DrawCandLine(hDC, lpUIPriv, lpCandList->dwSelection, FALSE);
            lpCandList->dwSelection = dwSelection;
            DrawCandLine(hDC, lpUIPriv, lpCandList->dwSelection, TRUE);
//            if (lpUIPriv->fdwUIFlags & UI_CAND_FRACTION) {
//                DrawFraction(hDC, lpUIPriv);
//            }
        }
        break;
    case WM_LBUTTONUP:
        if (hCandWnd == GetCapture()) {
            ReleaseCapture();
        }
	
        
		if(lpCandList->dwStyle !=0){
			ReleaseDC(hCandWnd, hDC);
			ChangeCand(hCandWnd);
			return 0L;
		} else if (!PtInRect(&lpUIPriv->rcCandText, ptCursor)) {
            break;
		} else if (dwSelection != lpCandList->dwSelection) {
            break;
        } else {
            HIMC hIMC;

            hIMC = (HIMC)GetWindowLong(hCandWnd, IMMGWL_IMC);
            ImmNotifyIME(hIMC, NI_SELECTCANDIDATESTR, 0L, lpCandList->dwSelection);
			//ImmNotifyIME(hIMC, 0x12, 0L, lpCandList->dwSelection);
			//ImmNotifyIME(hIMC, 2, 0L, lpCandList->dwSelection);
        }
        break;
    default:
        break;
    }

    ReleaseDC(hCandWnd, hDC);

    return 0L;
}

/**********************************************************************/
/* SetCandPosition()                                                  */
/**********************************************************************/
void SetCandPosition(HWND hCandWnd)
{
    LPUIPRIV       lpUIPriv;
    HIMC           hIMC;
    LPINPUTCONTEXT lpIMC;                 
//    POINT          ptNew;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hCandWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return;
    }

    if (!lpUIPriv->lpCandList) {
        return;
    }

    hIMC = (HIMC)GetWindowLong(hCandWnd, IMMGWL_IMC);
    if (!hIMC) {
        return;
    }

    lpIMC = (LPINPUTCONTEXT)ImmLockIMC(hIMC);
    if (!lpIMC) {
        ImmUnlockIMC(hIMC);
        return;
    }
  //  GetWorkArea(&g_sImeUIG.rcWorkArea);
    
	
	//ptNew.y = g_sImeUIG.rcWorkArea.bottom - g_sImeUIG.nCandLineHi;
	AdjustCandWnd(hCandWnd, lpUIPriv, lpIMC);

    ImmUnlockIMC(hIMC);


    return;
}



/**********************************************************************/
/* CandUIWndProc()                                                    */
/**********************************************************************/
LRESULT CALLBACK CandWndProc(HWND   hCandWnd, UINT   uMsg, WPARAM wParam, LPARAM lParam)
{

    LPCUIPRIV lpUIPriv;

    switch(uMsg) {
    case WM_CREATE:
        return OnCandCreate(hCandWnd, (CONST CREATESTRUCT *)lParam);
    case WM_DESTROY:
        return OnCandDestroy(hCandWnd);
    case WM_PAINT:    
        return OnCandPaint(hCandWnd);
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        return OnCandSetCursor(hCandWnd, uMsg, lParam);
    case WM_VSCROLL:
		break;
    case WM_IME_NOTIFY:
        lpUIPriv = (LPCUIPRIV)GetWindowLong(hCandWnd, IMMGWL_PRIVATE);
        if (!lpUIPriv->lpCandList) {
            break;
        }
        if (wParam == IMN_SETCANDIDATEPOS) {
            SetCandPosition(hCandWnd);
        }
        break;
    default:
        return DefWindowProc(hCandWnd, uMsg, wParam, lParam);
    }

    return 0L;
}

/**********************************************************************/
/* ShowCand() : Show the candidate window                             */
/**********************************************************************/
void ShowCand(HWND hUIWnd, int nShowCandCmd)
{
    LPUIPRIV lpUIPriv;

    // show or hid the UI window
    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return;
    }

    if (!(lpUIPriv->fdwUIFlags & UI_CAND_ALREADY_OPEN)) {
        return;
    }

    if (!lpUIPriv->hCandWnd) {
        return;
    }

   
    if (nShowCandCmd == SW_SHOWNOACTIVATE) {
        if (!IsWindowVisible(lpUIPriv->hCandWnd)) {
            ShowWindow(lpUIPriv->hCandWnd, nShowCandCmd);
        }
        
    } else if (nShowCandCmd == SW_HIDE) {
        if (IsWindowVisible(lpUIPriv->hCandWnd)) {
            ShowWindow(lpUIPriv->hCandWnd, SW_HIDE);
        }
      
    } 

    return;
}

/**********************************************************************/
/* OpenCand()                                                         */
/**********************************************************************/
void OpenCand(HWND hUIWnd)
{
    LPUIPRIV        lpUIPriv;
    HIMC            hIMC;
    LPINPUTCONTEXT  lpIMC;
    LPCANDIDATEINFO lpCandInfo;

//    POINT           ptNew;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) { return; }

    lpUIPriv->lpCandList = NULL;

    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
    if (!hIMC) {
        return;
    }

    lpIMC = (LPINPUTCONTEXT)ImmLockIMC(hIMC);
    if (!lpIMC) {
        return;
    }

    if (!lpIMC->hCandInfo) {
        ImmUnlockIMC(hIMC);
        return;
    }

    lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
    if (!lpCandInfo) {
        ImmUnlockIMC(hIMC);
        return;
    }

    lpUIPriv->lpCandList = (LPCANDIDATELIST)((LPBYTE)lpCandInfo +
                                             lpCandInfo->dwOffset[0]);

    if (lpUIPriv->lpCandList->dwPageSize > MAX_PAGESIZE) {
        lpUIPriv->lpCandList->dwPageSize = MAX_PAGESIZE;
    }

    lpUIPriv->dwPageSize = lpUIPriv->lpCandList->dwCount > lpUIPriv->lpCandList->dwPageSize ?
                           lpUIPriv->lpCandList->dwPageSize : lpUIPriv->lpCandList->dwCount;
    lpUIPriv->lpCandList->dwPageSize = lpUIPriv->dwPageSize;


    if (!lpUIPriv->hCandWnd) {
		GetWorkArea(&g_sImeUIG.rcWorkArea);
        lpUIPriv->hCandWnd = CreateWindowEx(
            WS_EX_NOACTIVATE|WS_EX_TOPMOST,
            UICANDCLASSNAME, NULL,
            WS_POPUP|WS_BORDER|WS_NOTIFY|CLS_FRACTION,
            0, 0, //50, 50,
			g_sImeUIG.rcWorkArea.right, g_sImeUIG.nCandLineHi,
            hUIWnd, (HMENU)NULL, lpUIPriv->hInst, NULL);

        if (!lpUIPriv->hCandWnd) {
            lpUIPriv->lpCandList = NULL;
            ImmUnlockIMCC(lpIMC->hCandInfo);
			ImmUnlockIMC(hIMC);
			return;
        }

    }

    lpUIPriv->fdwUIFlags |= UI_CAND_ALREADY_OPEN;


		
   
	//ptNew.x =0;
	//ptNew.y = g_sImeUIG.rcWorkArea.bottom - g_sImeUIG.nCandLineHi;
  

    AdjustCandWnd(lpUIPriv->hCandWnd, lpUIPriv, lpIMC);

    ShowCand(hUIWnd, SW_SHOWNOACTIVATE);


    ImmUnlockIMCC(lpIMC->hCandInfo);

    ImmUnlockIMC(hIMC);

    return;
}

/**********************************************************************/
/* CloseCand()                                                        */
/**********************************************************************/
void CloseCand(HWND hUIWnd)
{
    LPUIPRIV lpUIPriv;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);

    if (!(lpUIPriv->fdwUIFlags & UI_CAND_ALREADY_OPEN)) {
        return;
    }

    lpUIPriv->fdwUIFlags &= ~(UI_CAND_ALREADY_OPEN);

    if (lpUIPriv->hCandWnd && IsWindowVisible(lpUIPriv->hCandWnd)) {
        ShowWindow(lpUIPriv->hCandWnd, SW_HIDE);
    }

    
    if (lpUIPriv->lpCandList) {
        lpUIPriv->lpCandList = NULL;
    }

    return;
}

/**********************************************************************/
/* ChangeCand()                                                       */
/**********************************************************************/
void ChangeCand(HWND hUIWnd)
{
    LPUIPRIV        lpUIPriv;
    HIMC            hIMC;
    CONST INPUTCONTEXT * lpIMC;
    LPCANDIDATEINFO lpCandInfo;
    LPCANDIDATELIST lpCandList;
    int             nCandWi, nCandHi;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return;
    }

    if (!(lpUIPriv->fdwUIFlags & UI_CAND_ALREADY_OPEN)) {
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

    if (!lpIMC->hCandInfo) {
        ImmUnlockIMC(hIMC);
        ImmUnlockIMCC(lpIMC->hCandInfo);
	    return;
    }

    lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
    if (!lpCandInfo) {
        ImmUnlockIMCC(lpIMC->hCandInfo);
		ImmUnlockIMC(hIMC);
	    return;
    }

    lpCandList = (LPCANDIDATELIST)((LPBYTE)lpCandInfo + lpCandInfo->dwOffset[0]);

    lpUIPriv->lpCandList = lpCandList;

    nCandWi = lpUIPriv->nCandWi;
    nCandHi = lpUIPriv->nCandHi;

	GetWorkArea(&g_sImeUIG.rcWorkArea);
       
    if (nCandWi != lpUIPriv->nCandWi ||
        nCandHi != lpUIPriv->nCandHi) {
			lpUIPriv->nCandWi = g_sImeUIG.rcWorkArea.right;
			lpUIPriv->nCandHi = g_sImeUIG.nCandLineHi;
        SetWindowPos(lpUIPriv->hCandWnd, NULL, 0, 0,
                     g_sImeUIG.rcWorkArea.right, g_sImeUIG.nCandLineHi,
                     SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
    }

	// Processeing page/line up/down
	if(lpCandList->dwStyle == 1) {//line down
		if(lpCandList->dwSelection < lpCandList->dwCount-1 ) 
			lpCandList->dwSelection++;
		if(lpCandList->dwSelection > (lpCandList->dwPageStart + lpCandList->dwPageSize-1))
			lpCandList->dwStyle = 3; // do page down later
		
			
	}else if(lpCandList->dwStyle == 2) { // line up
		if (lpCandList->dwSelection > 0) 
			lpCandList->dwSelection--;
		if (lpCandList->dwSelection < lpCandList->dwPageStart) 
			lpCandList->dwStyle = 4; // do page up later
	
	}
	if(lpCandList->dwStyle == 3) {//page down
		if (lpCandList->dwPageStart < (lpCandList->dwCount - lpCandList->dwPageSize)) {
			lpCandList->dwPageStart += lpCandList->dwPageSize;
			lpCandList->dwSelection = lpCandList->dwPageStart;
			
		}
	}else if(lpCandList->dwStyle == 4) {//page up
		int i,nCharCnt=0;
		nCandWi = g_sImeUIG.rcWorkArea.right - g_sImeUIG.nCandCharWi *2;
		for( i=0;i<32; i++){
			
			if(lpCandList->dwPageStart > 0) 
				lpCandList->dwPageStart--;
			else
				break;
			nCharCnt += wcslen(GETLPCANDSTR(lpCandList, lpCandList->dwPageStart) )+ 1;
			if(nCandWi - nCharCnt * g_sImeUIG.nCandCharWi < g_sImeUIG.nCandCharWi*2){
				lpCandList->dwPageStart++;
				break;
			}
			
			
		}
		
		if(lpCandList->dwSelection > lpCandList->dwPageStart +i-1 )
			lpCandList->dwSelection = lpCandList->dwPageStart;
		
	}

	lpCandList->dwStyle = 0; // reset lpCandList->dwStyle
    InvalidateRect(lpUIPriv->hCandWnd, NULL, TRUE);
    ShowCand(hUIWnd, SW_SHOWNOACTIVATE);

    ImmUnlockIMCC(lpIMC->hCandInfo);
    ImmUnlockIMC(hIMC);

    return;
}

