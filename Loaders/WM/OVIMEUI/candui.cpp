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
#include "sipapi.h"

#include "windev.h"
#include <kfuncs.h>
#include "imemsg.hpp"
extern void GetWorkArea(LPRECT lprcWorkArea);

/**********************************************************************/
/* SetScrBarInfo()                                                    */
/**********************************************************************/
void SetScrBarInfo(HWND hScrBar, int nMin, int nMax, UINT nPage, int nPos)
{
    SCROLLINFO si;

    si.cbSize= sizeof(SCROLLINFO);
    si.fMask= SIF_PAGE | SIF_POS | SIF_RANGE;
    si.nMin = nMin;
    si.nMax = nMax;
    si.nPage= nPage;
    si.nPos = nPos;

    SetScrollInfo(hScrBar, SB_CTL, &si, TRUE);

    return;
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

/**********************************************************************/
/* CalcCandSize()                                                     */
/**********************************************************************/
void CalcCandSize(HWND hCandWnd)
{
    LPUIPRIV    lpUIPriv;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hCandWnd, IMMGWL_PRIVATE);
    if (!lpUIPriv) {
        return;
    }
    if (!lpUIPriv->lpCandList) {
        return;
    }


    if (g_sImeUIG.bAutoCandAccel && IsSIPOn()) {
        g_sImeUIG.nCandLineWi = g_sImeUIG.nCandCharWi * 3 + CAND_TEXTMARGIN * 2;
        lpUIPriv->rcCandText.left = 0;
    }
    else {
        g_sImeUIG.nCandLineWi = g_sImeUIG.nCandCharWi * 4 + CAND_TEXTMARGIN * 2;
        lpUIPriv->rcCandText.left = g_sImeUIG.nCandCharWi;
    }

    
    lpUIPriv->rcCandText.bottom = g_sImeUIG.nCandDy[lpUIPriv->lpCandList->dwPageSize];

    lpUIPriv->rcFraction.top = lpUIPriv->rcCandText.bottom;
    lpUIPriv->rcFraction.bottom = lpUIPriv->rcFraction.top + lpUIPriv->nFracHi;

    if (lpUIPriv->lpCandList->dwCount > lpUIPriv->lpCandList->dwPageSize) {
        // This means we need scroll bar in candidate window
        lpUIPriv->fdwUIFlags |= UI_CAND_DO_SCROLL;

        SetScrBarInfo(lpUIPriv->hScrBar, 0, lpUIPriv->lpCandList->dwCount - 1,
                      lpUIPriv->lpCandList->dwPageSize,
                      lpUIPriv->lpCandList->dwPageStart);

        if (g_sImeUIG.nCandLineWi + g_sImeUIG.cxVScroll < lpUIPriv->nFracWi) {
            lpUIPriv->rcCandText.right = lpUIPriv->nFracWi - g_sImeUIG.cxVScroll;
            g_sImeUIG.nCandDx = (lpUIPriv->rcCandText.right - lpUIPriv->rcCandText.left -
                                 - g_sImeUIG.nCandCharWi) / 2 + ((g_sImeUIG.bAutoCandAccel && IsSIPOn()) ? 0 : g_sImeUIG.nCandCharWi);
            lpUIPriv->rcFraction.right = lpUIPriv->nFracWi;
        } else {
            lpUIPriv->rcCandText.right = g_sImeUIG.nCandLineWi;
            g_sImeUIG.nCandDx = CAND_TEXTMARGIN + ((g_sImeUIG.bAutoCandAccel && IsSIPOn()) ? 0 : g_sImeUIG.nCandCharWi);
            lpUIPriv->rcFraction.right = g_sImeUIG.nCandLineWi + g_sImeUIG.cxVScroll;
        }

        lpUIPriv->nCandWi = lpUIPriv->rcFraction.right + g_sImeUIG.cxBorder * 2;
    } else {
        // We don't need scroll bar here
        lpUIPriv->fdwUIFlags &= ~(UI_CAND_DO_SCROLL);

        if (g_sImeUIG.nCandLineWi < lpUIPriv->nFracWi) {
            lpUIPriv->rcCandText.right = lpUIPriv->nFracWi;
            g_sImeUIG.nCandDx = (lpUIPriv->nFracWi-g_sImeUIG.nCandCharWi) / 2 + ((g_sImeUIG.bAutoCandAccel && IsSIPOn()) ? 0 : g_sImeUIG.nCandCharWi);
            lpUIPriv->rcFraction.right = lpUIPriv->nFracWi;
        } else {
            lpUIPriv->rcCandText.right = g_sImeUIG.nCandLineWi;
            g_sImeUIG.nCandDx = CAND_TEXTMARGIN + ((g_sImeUIG.bAutoCandAccel && IsSIPOn()) ? 0 : g_sImeUIG.nCandCharWi);
            lpUIPriv->rcFraction.right = g_sImeUIG.nCandLineWi;
        }

        lpUIPriv->nCandWi = lpUIPriv->rcFraction.right + g_sImeUIG.cxBorder * 2;
    }
    lpUIPriv->nCandHi = lpUIPriv->rcFraction.bottom + g_sImeUIG.cyBorder * 2;

    return;
}

// to avoid SIP region
typedef bool (WINAPI *PFNSHSipInfo)(UINT,UINT,PVOID,UINT);

BOOL CheckRectForSIP(CONST RECT * lprcRect, int& SipTop)
{
    SIPINFO  sSipInfo;
    RECT     rcScr;
    RECT     rcTmp1, rcTmp2;

    PFNSHSipInfo   pfnSHSipInfo;
    HMODULE        hMod;
    
    memset(&sSipInfo, 0, sizeof(SIPINFO));
    sSipInfo.cbSize = sizeof(SIPINFO);
    sSipInfo.dwImDataSize = 0;
    sSipInfo.pvImData = NULL;

//    if (IsAPIReady(SH_SHELL,0))//WAIT_OBJECT_0 != WaitForAPIReady(SH_SHELL, 0))
 //       goto GeneralSipCheck;

    hMod = GetModuleHandle(TEXT("aygshell.dll"));
    if (!hMod) 
        goto GeneralSipCheck;
    pfnSHSipInfo = (PFNSHSipInfo) GetProcAddress(hMod, TEXT("SHSipInfo"));
    if (!pfnSHSipInfo)
        goto GeneralSipCheck;
        
    if ((*pfnSHSipInfo)(SPI_GETSIPINFO, 0, (PVOID) &sSipInfo, 0)) {
        if (!SystemParametersInfo(SPI_GETWORKAREA, 0, &rcScr, 0))
            return FALSE;
        SipTop = sSipInfo.fdwFlags & SIPF_ON ? sSipInfo.rcSipRect.top : rcScr.bottom;
        if ((sSipInfo.fdwFlags & SIPF_ON) == 0)
            return false;

        // SIP can have minus coordinate. so let's get real display position
        if (!IntersectRect(&rcTmp1, &sSipInfo.rcSipRect, &rcScr)) 
            return FALSE;
        // now check the given RECT is overlapped with SIP
        if (!IntersectRect(&rcTmp2, lprcRect, &rcTmp1))
            return FALSE;
        // now they are overlapped.
        // but need to check that SIP is higher than the given rect. 
        // if it is, there is no meaning to change candidate list position.
        if (rcTmp2.top > sSipInfo.rcSipRect.top)
            return FALSE;
        return TRUE;
    }
    
GeneralSipCheck:    
    if (SipGetInfo(&sSipInfo)) {
        if (!SystemParametersInfo(SPI_GETWORKAREA, 0, &rcScr, 0))
            return FALSE;
        SipTop = sSipInfo.fdwFlags & SIPF_ON ? sSipInfo.rcSipRect.top : rcScr.bottom;

        if ((sSipInfo.fdwFlags & SIPF_ON) == 0)
            return false;
        // SIP can have minus coordinate. so let's get real display position
        if (!IntersectRect(&rcTmp1, &sSipInfo.rcSipRect, &rcScr))
            return FALSE;
        // now check the given RECT is overlapped with SIP
        if (!IntersectRect(&rcTmp2, lprcRect, &rcTmp1))
            return FALSE;
        // now they are overlapped.
        // but need to check that SIP is higher than the given rect. 
        // if it is, there is no meaning to change candidate list position.
        if (rcTmp2.top > sSipInfo.rcSipRect.top)
            return FALSE;
        return TRUE;
    }
    SipTop = g_sImeUIG.rcWorkArea.bottom;
    return FALSE;
}

/**********************************************************************/
/* AdjustCandWnd()                                                    */
/**********************************************************************/
void AdjustCandWnd(HWND hCandWnd, POINT ptNew)
{
    LPUIPRIV        lpUIPriv;
    int             nAvailTextHi;
    DWORD           dwPageSize;  // Available page size

    RECT    rect;
    BOOL    bSipOverlapped=FALSE;
    int nSipTop;
    
    lpUIPriv = (LPUIPRIV)GetWindowLong(hCandWnd, IMMGWL_PRIVATE);

    if (!lpUIPriv->lpCandList) {
        return;
    }

    ptNew.x = ptNew.x < 0 ? 0 : ptNew.x;
    ptNew.y = ptNew.y < 0 ? 0 : ptNew.y;

    nAvailTextHi = g_sImeUIG.rcWorkArea.bottom - ptNew.y
                    - lpUIPriv->nFracHi - g_sImeUIG.cyBorder * 2;
    nAvailTextHi = nAvailTextHi < 0 ? 0 : nAvailTextHi;

    rect.left = ptNew.x;
    rect.top  = ptNew.y;
    rect.right = ptNew.x + g_sImeUIG.nCandLineWi;
    rect.bottom = ptNew.y + g_sImeUIG.nCandLineHi*lpUIPriv->dwPageSize + lpUIPriv->nFracHi + g_sImeUIG.cyBorder * 2;
    bSipOverlapped = CheckRectForSIP(&rect, nSipTop);

    dwPageSize = (DWORD)nAvailTextHi / g_sImeUIG.nCandLineHi;

    if ((dwPageSize >= lpUIPriv->dwPageSize) && !bSipOverlapped) {
        dwPageSize = lpUIPriv->dwPageSize;
    } else if ((dwPageSize >= MIN_PAGESIZE) && !bSipOverlapped) {
        dwPageSize = MIN_PAGESIZE;
    } else {
#if 1   // Use new way to calculate candidate window position.
        // We will calculate the space above composition window and
        //   try to meet MIN_PAGESIZE.
        // If there is still not enough space, candidate window will
        //   locate above the composition window.

        nAvailTextHi = ptNew.y - lpUIPriv->nCompHi - g_sImeUIG.rcWorkArea.top -
                       lpUIPriv->nFracHi - g_sImeUIG.cyBorder;
        nAvailTextHi = nAvailTextHi < 0 ? 0 : nAvailTextHi;

        dwPageSize = (DWORD)nAvailTextHi / g_sImeUIG.nCandLineHi;
        if (dwPageSize >= lpUIPriv->dwPageSize ) {
            dwPageSize = lpUIPriv->dwPageSize;
        } else if (dwPageSize < MIN_PAGESIZE) {
            dwPageSize = MIN_PAGESIZE;
        } else {
        }

        ptNew.y = lpUIPriv->ptComp.y - g_sImeUIG.nCandDy[dwPageSize] -
                  lpUIPriv->nFracHi - g_sImeUIG.cyBorder;
                  
        // if there is no enough space at the above and the below of the composition window,
        murmur("AdjustCandWnd() bSipOverlapped = %d, ptNew.y = %d, g_sImeUIG.rcWorkArea.top = %d\n", bSipOverlapped, ptNew.y, g_sImeUIG.rcWorkArea.top);
        if (ptNew.y < g_sImeUIG.rcWorkArea.top) {
            ptNew.y = nSipTop - lpUIPriv->nCandHi;
            dwPageSize = lpUIPriv->dwPageSize;
            if ((lpUIPriv->ptComp.x + lpUIPriv->nCompWi + lpUIPriv->nCandWi) < g_sImeUIG.rcWorkArea.right)
                ptNew.x = lpUIPriv->ptComp.x + lpUIPriv->nCompWi;
            else if (lpUIPriv->ptComp.x > lpUIPriv->nCandWi)
                ptNew.x = lpUIPriv->ptComp.x - lpUIPriv->nCandWi;
        }

#else   // Still keep the old way for reference and backup (in case).
        int nyCand;

        nyCand = lpUIPriv->ptComp.y - g_sImeUIG.nCandDy[lpUIPriv->dwPageSize] -
                 lpUIPriv->nFracHi - g_sImeUIG.cyBorder;
        if (nyCand >= g_sImeUIG.rcWorkArea.top) {
            ptNew.y = nyCand;
            dwPageSize = lpUIPriv->dwPageSize;
        } else {
            dwPageSize = MIN_PAGESIZE;
        }
#endif
    }

    lpUIPriv->lpCandList->dwPageSize = dwPageSize;
    CalcCandSize(hCandWnd);

    SetWindowPos(hCandWnd, NULL, 0, 0,
                 lpUIPriv->nCandWi, lpUIPriv->nCandHi,
                 SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
    SetWindowPos(lpUIPriv->hScrBar, NULL,
                 lpUIPriv->rcCandText.right, lpUIPriv->rcCandText.top,
                 g_sImeUIG.cxVScroll, (lpUIPriv->rcCandText.bottom - lpUIPriv->rcCandText.top),
                 SWP_NOACTIVATE|SWP_NOZORDER);

    InvalidateRect(hCandWnd, NULL, FALSE);

    if (ptNew.x + lpUIPriv->nCandWi > g_sImeUIG.rcWorkArea.right) {
        ptNew.x = g_sImeUIG.rcWorkArea.right - lpUIPriv->nCandWi;
    }

    if (ptNew.x != lpUIPriv->ptCand.x ||
        ptNew.y != lpUIPriv->ptCand.y) {
        lpUIPriv->ptCand = ptNew;
        SetWindowPos(hCandWnd, NULL, ptNew.x, ptNew.y, 0, 0,
                     SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER);
    }

    return;
}

/**********************************************************************/
/* OnCandCreate()                                                     */
/**********************************************************************/
static __inline
LRESULT OnCandCreate(HWND hCandWnd, CONST CREATESTRUCT * lpCS)
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

    if (lpCS->style & CLS_FRACTION) {
        lpUIPriv->fdwUIFlags |= UI_CAND_FRACTION;

        lpUIPriv->nFracWi = g_sImeUIG.nCandFracWi;
        lpUIPriv->nFracHi = g_sImeUIG.nCandFracHi;
    } else {
        lpUIPriv->fdwUIFlags &= ~(UI_CAND_FRACTION);

        lpUIPriv->nFracWi = lpUIPriv->nFracHi = 0;
    }
    
    lpUIPriv->ptCand.x = lpCS->x;
    lpUIPriv->ptCand.y = lpCS->y;

    lpUIPriv->nCandWi = lpCS->cx;
    lpUIPriv->nCandHi = lpCS->cy;

    lpUIPriv->rcCandText.left = g_sImeUIG.nCandCharWi;
    lpUIPriv->rcCandText.top = 0;
    lpUIPriv->rcCandText.right = lpCS->cx + g_sImeUIG.nCandCharWi;
    lpUIPriv->rcCandText.bottom = lpCS->cy;

    lpUIPriv->rcFraction.left = 0;
    lpUIPriv->rcFraction.right = lpCS->cx;
    lpUIPriv->rcFraction.top = lpUIPriv->rcFraction.bottom = lpCS->cy;

    if (!lpUIPriv->hScrBar) {
        lpUIPriv->hScrBar = CreateWindow(
            TEXT("scrollbar"), NULL, WS_CHILD|SBS_VERT,
            lpUIPriv->rcCandText.right, lpUIPriv->rcCandText.top,
            g_sImeUIG.cxVScroll, lpCS->cy,
            hCandWnd, (HMENU)1, lpUIPriv->hInst, NULL);
    }

    return 0L;
}

/**********************************************************************/
/* OnCandDestroy()                                                    */
/**********************************************************************/
static __inline
LRESULT OnCandDestroy(HWND hCandWnd)
{
    LPUIPRIV lpUIPriv;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hCandWnd, IMMGWL_PRIVATE);

    lpUIPriv->hCandWnd = (HWND)NULL;

    if (lpUIPriv->hScrBar) {
        DestroyWindow(lpUIPriv->hScrBar);
        lpUIPriv->hScrBar = (HWND)NULL;
    }

    if (g_sImeUIG.hFracFont) {
        DeleteObject(g_sImeUIG.hFracFont);
        g_sImeUIG.hFracFont = NULL;
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
    TCHAR           szCandStr[2] = {0};

    RECT            rcAccel;
    TCHAR           szAccelStr[2] = {0};

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

    rcCandLine.left = lpUIPriv->rcCandText.left;
    rcCandLine.right = lpUIPriv->rcCandText.right;

    rcAccel.top = rcCandLine.top = g_sImeUIG.nCandDy[i] - CAND_LINEGAP;
    rcAccel.bottom = rcCandLine.bottom = g_sImeUIG.nCandDy[i + 1];

    rcAccel.left = 0; rcAccel.right = rcCandLine.left;

    szCandStr[0] = *(LPCTSTR)((LPBYTE)lpCandList + lpCandList->dwOffset[dwIndex]);

    ExtTextOut(hDC, g_sImeUIG.nCandDx, g_sImeUIG.nCandDy[i],
               ETO_OPAQUE, &rcCandLine, szCandStr, 1, NULL);

    if (!g_sImeUIG.bAutoCandAccel || !IsSIPOn()) {
        szAccelStr[0] = (TCHAR)(TEXT('1') + i);
        ExtTextOut(hDC, 3, g_sImeUIG.nCandDy[i], ETO_OPAQUE, &rcAccel, szAccelStr, 1, NULL);
    }

    return;
}

/**********************************************************************/
/* DrawFraction()                                                     */
/**********************************************************************/
void DrawFraction(HDC hDC, LPCUIPRIV lpUIPriv)
{
    HFONT hOldFont;
    TCHAR szFraction[10];

    hOldFont = (HFONT)SelectObject(hDC, g_sImeUIG.hFracFont);

    SetBkColor(hDC, g_sImeUIG.crBtnFace);
    SetTextColor(hDC, g_sImeUIG.crBtnText);

    StringCchPrintf(szFraction, sizeof(szFraction)/sizeof(szFraction[0]),
            TEXT("%d/%d"), lpUIPriv->lpCandList->dwSelection + 1,
            lpUIPriv->lpCandList->dwCount);
    
    ExtTextOut(hDC, lpUIPriv->rcFraction.left + CAND_LINEGAP,
            lpUIPriv->rcFraction.top + CAND_LINEGAP, ETO_OPAQUE,
            &lpUIPriv->rcFraction, szFraction, lstrlen(szFraction), NULL);

    SelectObject(hDC, hOldFont);

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
    TCHAR           szCandStr[2] = {0};
    
    RECT            rcAccel;
    TCHAR           szAccelStr[2] = {0};

    dwStart = lpCandList->dwPageStart;
    dwEnd = dwStart + lpCandList->dwPageSize;
    dwEnd = dwEnd > lpCandList->dwCount ? lpCandList->dwCount : dwEnd;
    dwSelect = lpCandList->dwSelection;


    SetBkColor(hDC, g_sImeUIG.crWindow);
    SetTextColor(hDC, g_sImeUIG.crWindowText);

	lpUIPriv->rcCandText.right = g_sImeUIG.rcWorkArea.right - g_sImeUIG.nCandCharWi*2;
    rcCandLine.left = 0;
    rcCandLine.right = lpUIPriv->rcCandText.right;

    rcAccel.top = rcCandLine.top = lpUIPriv->rcCandText.top;
    rcAccel.bottom = rcCandLine.bottom = g_sImeUIG.nCandDy[0];

    ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rcCandLine, NULL, 0, NULL);

    rcCandLine.left = lpUIPriv->rcCandText.left;
    
    rcAccel.left = 0;
    rcAccel.right = rcCandLine.left;

    for (i = 0; dwStart < dwEnd; dwStart++, i++) {
        if (dwSelect == dwStart) {
            rcAccel.top = rcCandLine.top = g_sImeUIG.nCandDy[i] - CAND_LINEGAP;
            rcAccel.bottom = rcCandLine.bottom = g_sImeUIG.nCandDy[i + 1];

            SetBkColor(hDC, g_sImeUIG.crHighlight);
            SetTextColor(hDC, g_sImeUIG.crHighlightText);
        } else {
            rcAccel.top = rcCandLine.top = g_sImeUIG.nCandDy[i];
            rcAccel.bottom = rcCandLine.bottom = g_sImeUIG.nCandDy[i + 1];
        }

        szCandStr[0] = *(LPCTSTR)((LPBYTE)lpCandList + lpCandList->dwOffset[dwStart]);

        ExtTextOut(hDC, g_sImeUIG.nCandDx, g_sImeUIG.nCandDy[i],
                   ETO_OPAQUE, &rcCandLine, szCandStr, 1, NULL);

        if (!g_sImeUIG.bAutoCandAccel || !IsSIPOn()) {
            szAccelStr[0] = (TCHAR)(TEXT('1')+i);
            ExtTextOut(hDC, 3, g_sImeUIG.nCandDy[i], ETO_OPAQUE, &rcAccel, szAccelStr, 1, NULL);
        }

        if (dwSelect == dwStart) {
            SetBkColor(hDC, g_sImeUIG.crWindow);
            SetTextColor(hDC, g_sImeUIG.crWindowText);
        }
    }

    if (i < (int)lpCandList->dwPageSize) {
        rcCandLine.top = g_sImeUIG.nCandDy[i];
        rcCandLine.bottom = lpUIPriv->rcCandText.bottom;
        rcCandLine.left = rcAccel.left;

        ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rcCandLine, NULL, 0, NULL);
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
    if (lpUIPriv->fdwUIFlags & UI_CAND_FRACTION) {
        DrawFraction(hDC, lpUIPriv);
    }
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
    DrawCandText(hDC, lpUIPriv); // HIGHLIGHT_CURSEL
    if (lpUIPriv->fdwUIFlags & UI_CAND_FRACTION) {
        DrawFraction(hDC, lpUIPriv);
    }
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
static __inline
LRESULT OnCandSetCursor(HWND hCandWnd, UINT uMsg, LPARAM lParam)
{
    LPUIPRIV        lpUIPriv;
    LPCANDIDATELIST lpCandList;
    POINT            ptCursor;
    DWORD           dwSelection;
    HDC             hDC;

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


    dwSelection = lpCandList->dwPageStart + 
        (ptCursor.y - lpUIPriv->rcCandText.top) / g_sImeUIG.nCandLineHi;
    dwSelection = dwSelection >= (lpCandList->dwPageStart + lpCandList->dwPageSize) ?
        lpCandList->dwPageStart + lpCandList->dwPageSize - 1 : dwSelection;

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
            if (lpUIPriv->fdwUIFlags & UI_CAND_FRACTION) {
                DrawFraction(hDC, lpUIPriv);
            }
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
            if (lpUIPriv->fdwUIFlags & UI_CAND_FRACTION) {
                DrawFraction(hDC, lpUIPriv);
            }
        }
        break;
    case WM_LBUTTONUP:
        if (hCandWnd == GetCapture()) {
            ReleaseCapture();
        }

        if (!PtInRect(&lpUIPriv->rcCandText, ptCursor)) {
            break;
        } else if (dwSelection != lpCandList->dwSelection) {
            break;
        } else {
            HIMC hIMC;

            hIMC = (HIMC)GetWindowLong(hCandWnd, IMMGWL_IMC);
            ImmNotifyIME(hIMC, NI_SELECTCANDIDATESTR, 0L, lpCandList->dwSelection);
			ImmNotifyIME(hIMC, 0x12, 0L, lpCandList->dwSelection);
			ImmNotifyIME(hIMC, 2, 0L, lpCandList->dwSelection);
        }
        break;
    default:
        break;
    }

    ReleaseDC(hCandWnd, hDC);

    return 0L;
}

/**********************************************************************/
/* OnCandScroll()                                                     */
/**********************************************************************/
static __inline
LRESULT OnCandScroll(HWND hCandWnd, WORD wScrollCode, WORD wPos)
{
    LPUIPRIV        lpUIPriv;
    LPCANDIDATELIST lpCandList;
    DWORD           dwMax;

    lpUIPriv = (LPUIPRIV)GetWindowLong(hCandWnd, IMMGWL_PRIVATE);

    if (!lpUIPriv->lpCandList) {
        return -1L;
    }

    lpCandList = lpUIPriv->lpCandList;

    switch(wScrollCode) {
    case SB_LINEDOWN:
        if (lpCandList->dwPageStart <
            (lpCandList->dwCount - lpCandList->dwPageSize)) {
            lpCandList->dwPageStart++;
            lpCandList->dwSelection = lpCandList->dwPageStart;
            DrawCandWnd(hCandWnd);
            SetScrollPos(lpUIPriv->hScrBar, SB_CTL, (int)lpCandList->dwPageStart, TRUE);
        }
        break;
    case SB_LINEUP:
        if (lpUIPriv->lpCandList->dwPageStart > 0){
            lpUIPriv->lpCandList->dwPageStart--;
            lpCandList->dwSelection = lpCandList->dwPageStart;
            DrawCandWnd(hCandWnd);
            SetScrollPos(lpUIPriv->hScrBar, SB_CTL, (int)lpCandList->dwPageStart, TRUE);
        }
        break;
    case SB_PAGEDOWN:
        if (lpCandList->dwPageStart <
            (lpCandList->dwCount - lpCandList->dwPageSize)) {
            lpCandList->dwPageStart += lpCandList->dwPageSize;
            dwMax = lpCandList->dwCount - lpCandList->dwPageSize;
            lpCandList->dwPageStart = lpCandList->dwPageStart < dwMax ?
                lpCandList->dwPageStart : dwMax;
            lpCandList->dwSelection = lpCandList->dwPageStart;
            DrawCandWnd(hCandWnd);
            SetScrollPos(lpUIPriv->hScrBar, SB_CTL, (int)lpCandList->dwPageStart, TRUE);
        }
        break;
    case SB_PAGEUP:
        if (lpCandList->dwPageStart > 0) {
            if (lpCandList->dwPageStart <= lpCandList->dwPageSize) {
                lpCandList->dwPageStart = 0;
            } else {
                lpCandList->dwPageStart -= lpCandList->dwPageSize;
            }
            lpCandList->dwSelection = lpCandList->dwPageStart;
            DrawCandWnd(hCandWnd);
            SetScrollPos(lpUIPriv->hScrBar, SB_CTL, (int)lpCandList->dwPageStart, TRUE);
        }
        break;
    case SB_THUMBTRACK:
        dwMax = lpUIPriv->lpCandList->dwCount - lpUIPriv->lpCandList->dwPageSize;
        dwMax = wPos > dwMax ? dwMax : wPos;
        if (wPos != lpCandList->dwPageStart && dwMax != lpCandList->dwPageStart) {
            lpCandList->dwPageStart = dwMax;
            lpCandList->dwSelection = lpCandList->dwPageStart;
            DrawCandWnd(hCandWnd);
        }
        break;
    case SB_THUMBPOSITION:
        dwMax = lpUIPriv->lpCandList->dwCount - lpUIPriv->lpCandList->dwPageSize;
        dwMax = wPos > dwMax ? dwMax : wPos;
        if (wPos != lpCandList->dwPageStart && dwMax != lpCandList->dwPageStart) {
            lpCandList->dwPageStart = dwMax;
            lpCandList->dwSelection = lpCandList->dwPageStart;
            DrawCandWnd(hCandWnd);
        }
        SetScrollPos(lpUIPriv->hScrBar, SB_CTL, (int)lpCandList->dwPageStart, TRUE);
        break;
    default:
        break;
    }

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
    POINT          ptNew;

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
    GetWorkArea(&g_sImeUIG.rcWorkArea);

    ImmUnlockIMC(hIMC);

    AdjustCandWnd(hCandWnd, ptNew);

    return;
}



/**********************************************************************/
/* CandUIWndProc()                                                    */
/**********************************************************************/
LRESULT CALLBACK CandWndProc(HWND   hCandWnd,
                             UINT   uMsg,
                             WPARAM wParam,
                             LPARAM lParam)
{
	ShowImeMessage(hCandWnd, uMsg, wParam, lParam);
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
        return OnCandScroll(hCandWnd, LOWORD(wParam), HIWORD(wParam));
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


//=====================================================================//
//  Exported API                                                       //
//=====================================================================//

/**********************************************************************/
/* ShowCand() : Show the candidate window                             */
/**********************************************************************/
void ShowCand(HWND hUIWnd, int nShowCandCmd)
{
    LPUIPRIV lpUIPriv;
    BOOL     bCandShowed;

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

    bCandShowed = IsWindowVisible(lpUIPriv->hCandWnd);

    if (nShowCandCmd == SW_SHOWNOACTIVATE) {
        if (!IsWindowVisible(lpUIPriv->hCandWnd)) {
            ShowWindow(lpUIPriv->hCandWnd, nShowCandCmd);
        }
        if (lpUIPriv->fdwUIFlags & UI_CAND_DO_SCROLL &&
            !IsWindowVisible(lpUIPriv->hScrBar)) {
            ShowWindow(lpUIPriv->hScrBar, nShowCandCmd);
        } else if (!(lpUIPriv->fdwUIFlags & UI_CAND_DO_SCROLL) &&
                   IsWindowVisible(lpUIPriv->hScrBar)) {
            ShowWindow(lpUIPriv->hScrBar, SW_HIDE);
        }
    } else if (nShowCandCmd == SW_HIDE) {
        if (IsWindowVisible(lpUIPriv->hCandWnd)) {
            ShowWindow(lpUIPriv->hCandWnd, SW_HIDE);
        }
        if (IsWindowVisible(lpUIPriv->hScrBar)) {
            ShowWindow(lpUIPriv->hScrBar, SW_HIDE);
        }
    } else {
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
    int             nCandWi, nCandHi;
    POINT           ptNew;

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
        lpUIPriv->hCandWnd = CreateWindowEx(
            WS_EX_NOACTIVATE|WS_EX_TOPMOST,
            v_szCandClassName, NULL,
            WS_POPUP|WS_BORDER|WS_NOTIFY|CLS_FRACTION,
            0, 0, 50, 50,
            hUIWnd, (HMENU)NULL, lpUIPriv->hInst, NULL);

        if (!lpUIPriv->hCandWnd) {
            lpUIPriv->lpCandList = NULL;
            ImmUnlockIMCC(lpIMC->hCandInfo);
            goto OpenCandUnlockIMCC;
        }

    }

    lpUIPriv->fdwUIFlags |= UI_CAND_ALREADY_OPEN;

    nCandWi = lpUIPriv->nCandWi;
    nCandHi = lpUIPriv->nCandHi;

    CalcCandSize(lpUIPriv->hCandWnd);

    if (nCandWi != lpUIPriv->nCandWi ||
        nCandHi != lpUIPriv->nCandHi) {
        SetWindowPos(lpUIPriv->hCandWnd, HWND_TOPMOST, 0, 0,
                     lpUIPriv->nCandWi, lpUIPriv->nCandHi,
                     SWP_NOACTIVATE|SWP_NOMOVE); //|SWP_NOZORDER);
        SetWindowPos(lpUIPriv->hScrBar, NULL,
                     lpUIPriv->rcCandText.right, lpUIPriv->rcCandText.top,
                     g_sImeUIG.cxVScroll, (lpUIPriv->rcCandText.bottom - lpUIPriv->rcCandText.top),
                     SWP_NOACTIVATE|SWP_NOZORDER);
    }

    if (IsWindowVisible(lpUIPriv->hCompWnd)) {
        ptNew.x = lpUIPriv->ptComp.x;
        ptNew.y = lpUIPriv->ptComp.y + lpUIPriv->nCompHi - g_sImeUIG.cyBorder;
    } else {
        ptNew = lpIMC->cfCandForm[0].ptCurrentPos;
        ClientToScreen(lpIMC->hWnd, &ptNew);
    }

    //AdjustCandWnd(lpUIPriv->hCandWnd, ptNew);
	SetCandPosition(lpUIPriv->hCandWnd);

    ShowCand(hUIWnd, SW_SHOWNOACTIVATE);

OpenCandUnlockIMCC:
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

    if (lpUIPriv->hScrBar && IsWindowVisible(lpUIPriv->hScrBar)) {
        ShowWindow(lpUIPriv->hScrBar, SW_HIDE);
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
        return;
    }

    lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo);
    if (!lpCandInfo) {
        goto ChangeCandUnlockIMCC;
    }

    lpCandList = (LPCANDIDATELIST)((LPBYTE)lpCandInfo + lpCandInfo->dwOffset[0]);

    lpUIPriv->lpCandList = lpCandList;

    nCandWi = lpUIPriv->nCandWi;
    nCandHi = lpUIPriv->nCandHi;

    CalcCandSize(lpUIPriv->hCandWnd);

    if (nCandWi != lpUIPriv->nCandWi ||
        nCandHi != lpUIPriv->nCandHi) {
        SetWindowPos(lpUIPriv->hCandWnd, NULL, 0, 0,
                     lpUIPriv->nCandWi, lpUIPriv->nCandHi,
                     SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
        SetWindowPos(lpUIPriv->hScrBar, NULL,
                     lpUIPriv->rcCandText.right, lpUIPriv->rcCandText.top,
                     g_sImeUIG.cxVScroll, (lpUIPriv->rcCandText.bottom - lpUIPriv->rcCandText.top),
                     SWP_NOACTIVATE|SWP_NOZORDER);
    }

    InvalidateRect(lpUIPriv->hCandWnd, NULL, TRUE);
    ShowCand(hUIWnd, SW_SHOWNOACTIVATE);

ChangeCandUnlockIMCC:
    ImmUnlockIMCC(lpIMC->hCandInfo);

    ImmUnlockIMC(hIMC);

    return;
}

