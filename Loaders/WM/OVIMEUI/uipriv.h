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

#ifndef __UIPRIV_H_
#define __UIPRIV_H_


#define CAND_LINEGAP                1
#define CAND_TEXTMARGIN             5
#define MAX_PAGESIZE                9
#define MIN_PAGESIZE                4

#define CARET_WIDTH                 2
#define COMP_TEXTMARGIN             1
#define MAX_COMPKEYS                10

#define TIMER_UICHANGE              (WM_USER + 0x0501)

#define UI_COMP_ALREADY_START       ISC_SHOWUICOMPOSITIONWINDOW
#define UI_CAND_ALREADY_OPEN        ISC_SHOWUICANDIDATEWINDOW

#define UI_CAND_FRACTION            0x00000010      // is candidate window default style
#define UI_CAND_DO_SCROLL           0x00000020      // candidate window do scrolling
#define UI_CAND_BTN_PUSHED          0x00000040      // button pushed state
#define UI_CAND_DBL_TAPPED          0x00000080      // double clicked
#define UI_DIFF_SYS_CHARSET         0x00100000      // the system charset is not NATIVE_CHARSET
#define UI_DEFAULT_MODE             0x00200000      // is candidate window default style

#define UI_COMPONENT_STATE          (UI_COMP_ALREADY_START|UI_CAND_ALREADY_OPEN)
#define UI_PRIVATE_FLAGS            (UI_DIFF_SYS_CHARSET|UI_DEFAULT_MODE)
#define UI_CAND_FLAGS               (UI_CAND_FRACTION|UI_CAND_DO_SCROLL|UI_CAND_BTN_PUSHED|UI_CAND_DBL_TAPPED)

#if (UI_COMPONENT_STATE & (UI_PRIVATE_FLAGS|UI_CAND_FLAGS))
#error bit confliction
#endif

#define WINDOW_NOT_DRAG         0xFFFFFFFF

typedef struct tagIMEUIG {              // IME UI global data
    // For UI common info
    RECT            rcWorkArea;         // the working area of system
    int             cxBorder;           // the width of border
    int             cyBorder;           // the height of border
    int             cxVScroll;          // the width of VScroll bar
    DWORD           crWindow;           // the window background color
    DWORD           crWindowText;       // the text in window color
    DWORD           crHighlight;        // selected item color
    DWORD           crHighlightText;    // text of selected item color
    DWORD           crBtnFace;          // button face color
    DWORD           crBtnText;          // button text color
    // For candidate window
    int             nCandCharWi;        // the width of candidate character
    int             nCandLineWi;        // the default candidate line width
    int             nCandLineHi;        // the default candidate line height
    int             nCandDx;            // the dx of candidate text
    int             nCandDy[MAX_PAGESIZE+1];// the dy of candidate text
    HFONT           hFracFont;          // the specified font for Fraction
    int             nCandFracWi;        // the candidate fraction width
    int             nCandFracHi;        // the candidate fraction height
    
    BOOL			bNoStatus;
    BOOL			bAutoCandAccel;
} IMEUIG;

typedef IMEUIG      *PIMEUIG;
typedef IMEUIG NEAR *NPIMEUIG;
typedef IMEUIG FAR  *LPIMEUIG;

extern HINSTANCE g_hInst;
extern IMEUIG g_sImeUIG;

typedef struct tagUIPRIV {              // IME private UI data
    HINSTANCE       hInst;              // the process instance
    DWORD           fdwSetContext;      // the actions to take at set context time
    DWORD           fdwUIFlags;         // the flags for UI
    HIMC            hCacheIMC;          // the recent selected hIMC
    // For composition window
    HWND            hCompWnd;           // composition window handle
    POINT           ptComp;             // the position of composition window
    int             nCompWi;            // the width of composition window
    int             nCompHi;            // the height of composition window
    RECT            rcCompText;         // the rect of composition text
    int             nCompCharWi;        // the width of composition char
    // For candidate window
    HWND            hCandWnd;           // candidate window handle
    HWND            hScrBar;            // scroll bar window handle
    POINT           ptCand;             // the candidate window position
    int             nCandWi;            // the width of candidate window
    int             nCandHi;            // the height of candidate window
    int             nFracWi;            // the candidate fraction width
    int             nFracHi;            // the candidate fraction height
    RECT            rcCandText;         // the rect of candidate text area
    RECT            rcFraction;         // the rect of fraction
    DWORD           dwPageSize;         // orginal candidate page size
    LPCANDIDATELIST lpCandList;         // pointer of Candidate List
    // For status window
    HWND			hStatusWnd;
    int     		nShowStatusCmd;
    int         	nStatusWidth;      // width of status window
    int         	nStatusHeight;      // high of status window
    RECT        	rcStatusWnd;   
    RECT        	rcStatusButton;
    RECT        	rcFullButton;
    DWORD			dwUIMoveOffset;
    DWORD			dwUIMoveXY;
    
    BOOL			bOpenStatus;
} UIPRIV;

typedef UIPRIV      *PUIPRIV;
typedef UIPRIV NEAR *NPUIPRIV;
typedef UIPRIV FAR  *LPUIPRIV;

typedef CONST UIPRIV FAR *LPCUIPRIV;


extern const TCHAR v_szImeUIClassName[];
extern const TCHAR v_szCompClassName[];
extern const TCHAR v_szCandClassName[];
extern const TCHAR v_szStatusClassName[];


void SetCompPosition(HWND hCompWnd);                            // compui.cpp
void WINAPI ShowComp(HWND hUIWnd, int nShowCandCmd);            // compui.cpp
void PASCAL StartComp(HWND hUIWnd);                             // compui.cpp
void PASCAL OnComp(HWND hUIWnd, DWORD fdwFlags);                // compui.cpp
void PASCAL EndComp(HWND hUIWnd);                               // compui.cpp

void AdjustCandWnd(HWND hCandWnd, POINT ptNew);                 // candui.cpp
void WINAPI ShowCand(HWND hUIWnd, int nShowCandCmd);            // candui.cpp
void WINAPI OpenCand(HWND hUIWnd);                              // candui.cpp
void WINAPI CloseCand(HWND hUIWnd);                             // candui.cpp
void WINAPI ChangeCand(HWND hUIWnd);                            // candui.cpp


void WINAPI ShowStatus(HWND hUIWnd, int nShowCandCmd);            
void WINAPI OpenStatus(HWND hUIWnd);                              
void WINAPI CloseStatus(HWND hUIWnd);                             


LRESULT CALLBACK ImeUIWndProc(HWND, UINT, WPARAM, LPARAM);      // ui.cpp

LRESULT CALLBACK CompWndProc(HWND, UINT, WPARAM, LPARAM);       // candui.cpp

LRESULT CALLBACK CandWndProc(HWND, UINT, WPARAM, LPARAM);       // candui.cpp

LRESULT CALLBACK StatusWndProc(HWND, UINT, WPARAM, LPARAM);

#endif __UIPRIV_H_
