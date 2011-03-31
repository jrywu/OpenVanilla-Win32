//#define OV_DEBUG
#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include <sipapi.h>
#include <keybd.h>
#include "dllmain.h"
#include "MVSIP.h"
#include "MVSIP_i.h"
#include "KeyMap.h"
#include "kbmacros.h"
#include "MVdefs.h"
// for OV_DEBUG...
#include "OVUtility.h"

//LRESULT APIENTRY UIWndProc(HWND   hUIWnd,  UINT   uMsg, WPARAM wParam,  LPARAM lParam);

__inline void HIDPI_InitScaling()
{
    HDC screen;
    
    if( g_HIDPI_LogPixelsX )
        return;
    
    screen = GetDC(NULL);
    g_HIDPI_LogPixelsX = GetDeviceCaps(screen, LOGPIXELSX);
    g_HIDPI_LogPixelsY = GetDeviceCaps(screen, LOGPIXELSY);
    ReleaseDC(NULL, screen);
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

// Start the key repeat timer for the specified key.
__inline static void StartRepeat(const KEYENTRY *pKey)
{
    if (pKey) {
		g_bHoldKeySent = false;
        g_pRepeatKey = pKey;
        SetTimer(g_hwndMain, TIMERID_REPEAT, g_dwInitialDelay, NULL);
    }
}

// Stop the key repeat timer.
__inline static void StopRepeat(void)
{
    if (g_pRepeatKey) {
        g_pRepeatKey = NULL;
        KillTimer(g_hwndMain, TIMERID_REPEAT);
    }
}

// Reset function key states
static BOOL ResetFnKeyState(BOOL fForced)
{
    BOOL fReset;

    fReset = FALSE;

    if (SHIFTED() && (fForced || !ISSHIFTLOCKED())) {
        RESETSHIFT();
        // Unpress SHIFT key
        if (GetKeyState(VK_SHIFT) & 0x80) {
            g_pIMCallback->SendVirtualKey(VK_SHIFT, KEYEVENTF_KEYUP|KEYEVENTF_SILENT);
        }

        fReset = TRUE;
    }

    if (CONTROLED() && (fForced || !ISCONTROLLOCKED())) {
        RESETCONTROL();
        // Unpress CONTROL key
        if (GetKeyState(VK_CONTROL) & 0x80) {
            g_pIMCallback->SendVirtualKey(VK_CONTROL, KEYEVENTF_KEYUP|KEYEVENTF_SILENT);
        }

        fReset = TRUE;
    }
	
	if (SYMBOLED() && (fForced || !ISSYMBOLLOCKED())) {
		RESETSYMBOL();
		fReset = TRUE;
	}


    return fReset;
}

static void SetKeybd(int nKeybd)
{
	g_nCurKB = nKeybd;
    switch(nKeybd) {
    case 0:
	case 3:
        g_pRowCoord = g_5RowCoord; //chn and sym keyboards are 4 rows
        break;
    case 1:
    case 2:
        g_pRowCoord = g_4RowCoord; // eng keyboard is 3 rows
        break;
    default:
        break;
    }
}

BOOL SwitchKeybd(void)
{
    int nKeybd;

    if (g_FnKeyStates[FN_SYMBOL] & STATE_SET) {
        nKeybd = 3;
    } else if (g_ModeKeyStates[MODE_ENGLISH] & STATE_SET) {
        nKeybd = 1;
        if (g_FnKeyStates[FN_SHIFT] & STATE_SET) nKeybd++;
    } else {
        nKeybd = 0;   // default keyboard
		//if (g_FnKeyStates[FN_SHIFT] & STATE_SET) nKeybd++;
    }

    if (nKeybd == g_nCurKB)  
		return FALSE;

    SetKeybd(nKeybd);

    return TRUE;
}



void InitKeybd(int nKeybd)
{
    memset(g_KeyStates, 0, sizeof(g_KeyStates));
    memset(g_ModeKeyStates, 0, sizeof(g_ModeKeyStates));
    memset(g_FnKeyStates, 0, sizeof(g_FnKeyStates));


    switch(nKeybd) {
	case 0:
		g_ModeKeyStates[MODE_CHINESE] |= STATE_SET;
    case 1:
    case 2:
        g_ModeKeyStates[MODE_ENGLISH] |= STATE_SET;
        break;
    case 3:
        g_FnKeyStates[FN_SYMBOL] |= STATE_SET;
        break;
    default:
        g_ModeKeyStates[MODE_CHINESE] |= STATE_SET;
        break;
    }

    SetKeybd(nKeybd);

    return;
}

//void DestroyKeybd(void)
//{
//    return;
//}

//
// Paint an area of the keyboard.
//
static void WINAPI IM_DrawArea( HDC hdc, RECT *prcUpdate )
{
	murmur("IM_DrawArea");
    const KEYENTRY*   pKey;
    RECT        rc;
    RECT        rcDummy;
    int         nRow        = 0;
    int         nDrawn      = 0;
	int         nKeys = 0;

    //
    // Draw the normal bitmap
    //
	SelectObject(g_hKeybdDC, g_hKeybdBmp[g_nCurKB]);
    StretchBlt(
        hdc,
        prcUpdate->left,
        prcUpdate->top,
        prcUpdate->right - prcUpdate->left,
        prcUpdate->bottom - prcUpdate->top,
        g_hKeybdDC,
        UNSCALEX(prcUpdate->left),
        UNSCALEY(prcUpdate->top),// +  KBDBMOFFSET(),
        UNSCALEX(prcUpdate->right - prcUpdate->left),
        UNSCALEY(prcUpdate->bottom - prcUpdate->top),
        SRCCOPY );


    //
    // Now draw all keys that are down.
    //

    if( nDrawn < g_nDown ) 
    {
        for( nRow = 0; nRow < MAX_NUM_ROWS; nRow++ ) 
        {
            pKey = GETKEYENTRY(nRow, 0);     // g_keys[nRow];
            while( nDrawn < g_nDown && pKey->bVk != SENTINEL ) 
            {
                if(KEYSTATE(pKey) & (STATE_DOWN|STATE_DISABLED))
                {
                    nDrawn++;
                    rc.left   = pKey->nLeft;
                    rc.right  = (pKey + 1)->nLeft;
                    rc.top    = g_pRowCoord[nRow];
                    rc.bottom = g_pRowCoord[nRow + 1];
                    InflateRect( &rc, -1, -1 );
                    if( IntersectRect( &rcDummy, prcUpdate, &rc ) ) 
                    {
						SelectObject(g_hKeybdDC, g_hKeybdBmp[g_nCurKB]);	
						StretchBlt(
							hdc,
							SCALEX(rc.left),
                            SCALEY(rc.top),
                            SCALEX(rc.right - rc.left + 1),
                            SCALEY(rc.bottom - rc.top + 1),
							g_hKeybdDC,
							rc.left - 2,
							rc.top - 2,// + KBDBMOFFSET() ,
							rc.right - rc.left + 2,
							rc.bottom - rc.top + 2,
							SRCCOPY);
                    }
                }
                pKey++;
            }
        }
    }

    return;
}


// Paint an area of the keyboard.
static void IM_DrawArea(void)
{
    HDC  hdc;
    RECT rc;

    GetClientRect(g_hwndMain, &rc);
    hdc = GetDC(g_hwndMain);
    IM_DrawArea(hdc, &rc);
    ReleaseDC(g_hwndMain, hdc);
}

//
// Determine what key contains the point specified in rcKey.left,rcKey.top.
// Fill in the full coordinates for the key and return the keyentry.
//
// Assumptions: we will not get a down event outside of our window, and every
// point in the window maps to a key.
//
__inline const KEYENTRY* WINAPI IM_GetKeyFromCoord( RECT *prcKey )
{


    int nRow = 0;
    //int nKey = 0;
	
	const KEYENTRY *pKey;
    //
    // Row
    //

    while( prcKey->top > SCALEY(g_pRowCoord[nRow + 1]) ) 
            nRow++;
    
    //
    // Key
    //
	pKey = GETKEYENTRY(nRow, 1);
    while( prcKey->left > SCALEX(pKey->nLeft)) //g_keys[nRow][nKey + 1].nLeft) ) 
            pKey++;//nKey++;

    //
    // Set up rect and return the key.
    //

    prcKey->left   = SCALEX((pKey - 1)->nLeft);//g_keys[nRow][nKey].nLeft);
    prcKey->right  = SCALEX(pKey->nLeft);//g_keys[nRow][nKey + 1].nLeft);
    prcKey->top    = SCALEY(g_pRowCoord[nRow]);
    prcKey->bottom = SCALEY(g_pRowCoord[nRow + 1]);

    return (pKey - 1);  //return g_keys[nRow] + nKey;
}


// Paint a key of the keyboard.
__inline static void IM_DrawKey(const KEYENTRY *pKey, RECT *prcKey, BOOL fPress)
{
	HDC		hdc;
    RECT	rc;

	int keyPressedOffset;
	//
    // Draw key in new state.
    //
    // Set up values specific to press/unpress.
    if (fPress) 
	{
		keyPressedOffset = 2;
        InflateRect(&rc, -1, -1);
    } 
	else 
	{
		keyPressedOffset = 0;
	}

	rc = *prcKey;
    hdc = GetDC( g_hwndMain );
	SelectObject(g_hKeybdDC, g_hKeybdBmp[g_nCurKB]);
    StretchBlt(
            hdc,
            rc.left,
            rc.top,
            rc.right - rc.left + SCALEX(2),
            rc.bottom - rc.top + SCALEY(2),
            g_hKeybdDC,
            UNSCALEX(rc.left - keyPressedOffset),
            UNSCALEY(rc.top - keyPressedOffset),// + SCALEY(KBDBMOFFSET()) ),
            UNSCALEX(rc.right - rc.left) + 2,
            UNSCALEY(rc.bottom - rc.top) + 2,
            SRCCOPY);


	ReleaseDC( g_hwndMain, hdc );
    return;
}


// Actually send the key to the system.
__inline HRESULT IM_SendKey(const KEYENTRY *pKey, BOOL fPress) {
    HRESULT         hRes;
    UINT            uChar, VkeyToSend;
    DWORD           dwVkFlags;
    KEY_STATE_FLAGS nShiftState;

    if (fPress) {
        dwVkFlags = KEYEVENTF_SILENT;
        nShiftState = KeyStateDownFlag;  // POST WM_KEYDOWN
    } else {
        dwVkFlags = KEYEVENTF_KEYUP | KEYEVENTF_SILENT;
        nShiftState = KeyStatePrevDownFlag | KeyShiftNoCharacterFlag;
    }

    // Send the appropriate key message.
    // Don't send anything if it is a VVK code
    if (pKey->fdwFlags & F_VVK) {
        if (fPress && (SHIFTED() || ISSHIFTLOCKED())) {
            g_pIMCallback->SendVirtualKey(VK_SHIFT, KEYEVENTF_KEYUP|KEYEVENTF_SILENT);
        	RESETSHIFT();
        }
        hRes = S_OK;
    } else if (pKey->fdwFlags & F_VK) {
        BOOL bShifted = FALSE;

        bShifted = GetKeyState(VK_SHIFT) & 0x80;
        if (pKey->fdwFlags & F_NOSHIFT && bShifted) {
            g_pIMCallback->SendVirtualKey(VK_SHIFT, KEYEVENTF_KEYUP|KEYEVENTF_SILENT);
        }

        hRes = g_pIMCallback->SendVirtualKey(pKey->bVk, dwVkFlags);

        if (pKey->fdwFlags & F_NOSHIFT && bShifted) {
            g_pIMCallback->SendVirtualKey(VK_SHIFT, KEYEVENTF_SILENT);
        }
	} else {
		
		uChar = (SHAPED() && (!ISCHNKBD()) )? pKey->uFullChar : pKey->uChar;			
		VkeyToSend = ( uChar < 128 ) ? pKey->bVk : VK_PACKET;
			
		if(!fPress && (pKey->fdwFlags & F_HOLD) && !g_bHoldKeySent) { 
			// send WM_KEYDOWN on key up if hold is aborted 
			dwVkFlags = KEYEVENTF_SILENT;
			nShiftState = KeyStateDownFlag;  // POST WM_KEYDOWN
			hRes = g_pIMCallback->SendCharEvents(VkeyToSend,
				                                 nShiftState,
					                             uChar ? 1 : 0,
						                         &nShiftState,
							                     &uChar);
			dwVkFlags = KEYEVENTF_KEYUP | KEYEVENTF_SILENT;
			nShiftState = KeyStatePrevDownFlag | KeyShiftNoCharacterFlag;

		}
			

		if((pKey->fdwFlags & F_HOLD) && g_bHoldKeySent) {
			// send hold key
			uChar = SHAPED() ? pKey->uHoldFullCHar : pKey->uHoldChar ;
			VkeyToSend = ( uChar < 128 ) ? pKey->bHoldVk : VK_PACKET;
		}
		// do not post WM_KEYDOWN for F_HOLD keys immediately
		if(!(fPress && (pKey->fdwFlags & F_HOLD) && !g_bHoldKeySent ) ) 
			hRes = g_pIMCallback->SendCharEvents(VkeyToSend,
				                                 nShiftState,
					                             uChar ? 1 : 0,
						                         &nShiftState,
							                     &uChar);
	} 
    
    return hRes;
}


//
// Press or unpress a key.
// fPress is TRUE to press the key, FALSE to unpress it.
//
static BOOL WINAPI IM_PressKey( BOOL fPress, const KEYENTRY *pKey, RECT *prcKey )
{
	
    HRESULT         hRes;
    //
    // Set up values specific to press/unpress.
	//
	
    if( fPress ) 
    {
        KEYSTATE(pKey) |= STATE_DOWN;
		if (pKey->fdwFlags & (F_REPEAT | F_HOLD))  
			StartRepeat(pKey); 
    }
    else
    {
		StopRepeat();
        KEYSTATE(pKey) &= ~STATE_DOWN;
    }


    //
    // Adjust down count (an optimization for redraw and searching).
    //

    g_nDown += fPress ? 1 : -1;
	
    if (!(pKey->fdwFlags & F_MODE && !fPress)) {
        IM_DrawKey(pKey, prcKey, fPress);
    }

    // Send out the key.
    hRes = IM_SendKey(pKey, fPress);

    return SUCCEEDED(hRes);

	
}



// Handle WM_CREATE message.
__inline static LRESULT WINAPI IM_OnCreate(void)
{
	murmur("WM_CREATE: OnCreate() \n");
    DWORD  dwType = REG_DWORD, dwSize;

	GetModuleFileName(GetModuleHandle(L"MVSIP.DLL"), szDllPath, MAX_PATH);
	TCHAR* lb= wcsrchr(szDllPath,'\\');
	szDllPath[((DWORD)lb-(DWORD)szDllPath)/sizeof(TCHAR) +1]=0; //stripped the filename

/*
	
    dwSize = sizeof(DWORD);
    if (RegQueryValueEx(HKEY_LOCAL_MACHINE,
                        g_pszRegLCID,
                        (DWORD *)g_pszRegInternational,
                        &dwType,
                        (BYTE *)&g_uLCID,
                        &dwSize) != ERROR_SUCCESS) {
        g_uLCID = 0; // NULL LCID
    }
*/
    dwSize = sizeof(DWORD);
    RegQueryValueEx(HKEY_CURRENT_USER,
                    g_pszRegInitialDelay,
                    (LPDWORD)g_pszRegKeybd,
                    &dwType,
                    (LPBYTE)&g_dwInitialDelay,
                    &dwSize);

    dwSize = sizeof(DWORD);
    RegQueryValueEx(HKEY_CURRENT_USER,
                    g_pszRegRepeatRate,
                    (LPDWORD)g_pszRegKeybd,
                    &dwType,
                    (LPBYTE)&g_dwRepeatRate,
                    &dwSize);
    if (!g_dwRepeatRate) {
        g_dwRepeatRate = 1;
    }
    g_dwRepeatRate = 1000 / g_dwRepeatRate;

    dwSize = sizeof(DWORD);
    if (RegQueryValueEx(HKEY_CLASSES_ROOT,
                        g_pszRegKBMode,
                        (LPDWORD)g_pszIMClsidKey,
                        &dwType,
                        (LPBYTE)&g_dwKBMode,
                        &dwSize) != ERROR_SUCCESS) {
        g_dwKBMode = 0;
    }

	murmur("g_dwKBMode:%d\n", g_dwKBMode);

    InitKeybd((int)g_dwKBMode);

	murmur("g_nCurKB:%d\n", g_nCurKB);

	
	
    g_nDownKeys = 0;


    g_pRepeatKey = NULL;

	// Load sip images
	wchar_t szBmpPath[MAX_PATH];
	swprintf(szBmpPath, TEXT("%schn24.bmp"), szDllPath);
	g_hKeybdBmp[0]= SHLoadDIBitmap(szBmpPath);

	swprintf(szBmpPath, TEXT("%seng24.bmp"), szDllPath);
	g_hKeybdBmp[1] = SHLoadDIBitmap(szBmpPath);

	swprintf(szBmpPath, TEXT("%sengshift24.bmp"), szDllPath);
	g_hKeybdBmp[2] = SHLoadDIBitmap(szBmpPath);

	swprintf(szBmpPath, TEXT("%ssym24.bmp"), szDllPath);
	g_hKeybdBmp[3] = SHLoadDIBitmap(szBmpPath);
    
	g_hKeybdDC = CreateCompatibleDC(NULL);
    g_hOldKeybdBmp   = (HBITMAP)SelectObject(g_hKeybdDC, g_hKeybdBmp[g_nCurKB]);

// Get UI Class name
	_tcscpy(UICLASSNAME, _T("OVIMEUI"));
    return 0;
}

// Handle WM_DESTROY message.
__inline static LRESULT WINAPI IM_OnDestroy(void)
{
    StopRepeat();

    // Unpress SHIFT key
    if (GetKeyState(VK_SHIFT) & 0x80) {
        g_pIMCallback->SendVirtualKey(VK_SHIFT, KEYEVENTF_KEYUP|KEYEVENTF_SILENT);
    }

    // Unpress CONTROL key
    if (GetKeyState(VK_CONTROL) & 0x80) {
        g_pIMCallback->SendVirtualKey(VK_CONTROL, KEYEVENTF_KEYUP|KEYEVENTF_SILENT);
    }

    // Unpress CAPITAL key
    if (GetKeyState(VK_CAPITAL) & 0x01) {
        g_pIMCallback->SendVirtualKey(VK_CAPITAL, KEYEVENTF_KEYUP|KEYEVENTF_SILENT);
    }
    
    SelectObject(g_hKeybdDC, g_hOldKeybdBmp);
	for(int i=0;i<NUM_KEYBDS;i++)
		DeleteObject(g_hKeybdBmp[i]);
    DeleteDC(g_hKeybdDC);

    if (GETKBMODE() != g_dwKBMode) {
        HKEY  hKey;
        
        g_dwKBMode = GETKBMODE();
        if (RegOpenKeyEx(HKEY_CLASSES_ROOT, g_pszIMClsidKey, 0, 0, &hKey) == ERROR_SUCCESS) {
            DWORD dwType = REG_DWORD, dwSize = sizeof(DWORD);

            RegSetValueEx(hKey, g_pszRegKBMode, 0,
                          dwType,
                          (const BYTE *)&g_dwKBMode,
                          dwSize);

            RegCloseKey(hKey);
        }
    }
//	DestroyKeybd();

    return 0;
}

__inline static void WINAPI IM_SetDownKeys( void )
{
    const KEYENTRY*   pKey;
    int         nRow;
	// Reset the down key numbers
    g_nDown = 0;

    for( nRow = 0; nRow < g_nRows; nRow++ ) 
    {
        pKey = GETKEYENTRY(nRow, 0);// g_keys[nRow];
        while( pKey->bVk != SENTINEL ) 
        {
			if (ISFNKEY(pKey)) 
			{
                if (ISKEYSET(pKey)) 
				{
                    KEYSTATE(pKey) |= STATE_DOWN;
                    g_nDownKeys++;
                } 
				else 
					KEYSTATE(pKey) &= ~STATE_DOWN;
            }
            pKey++;
        }
    }

    return;
    
}


void IM_ProceedKey(const KEYENTRY *pKey)
{
	murmur("IM_Proceedkey::key->bvK:%x\n", pKey->bVk);
    BOOL            fRedraw = FALSE;
    int             nRow;
    const KEYENTRY *pTemp;

    if (pKey->fdwFlags & F_REDRAW) 
	        fRedraw = TRUE;
    else 
	        fRedraw = FALSE;

    // If the key unpressed is not sticky, unpress all currently stuck keys
    // except for locked.
    if (g_nDownKeys && ISNORMALKEY(pKey)) {
        RECT rc;

        for (nRow = 0; nRow < g_nRows; nRow++) 
		{
            pTemp = GETKEYENTRY(nRow, 0);
            while (g_nDownKeys && pTemp->bVk != SENTINEL) 
			{
                if (pTemp->fdwFlags & F_STK && ISKEYSET(pTemp) && !ISKEYLOCKED(pTemp)) 
				{
                    rc.left   = pTemp->nLeft;
                    rc.right  = (pTemp + 1)->nLeft;
                    rc.top    = g_pRowCoord[nRow];
                    rc.bottom = g_pRowCoord[nRow + 1];
                    FNKEYSTATE(pTemp) &= ~STATE_SET;
                    IM_PressKey(FALSE, pTemp, &rc);
                    if (pTemp->fdwFlags & F_REDRAW) 
                        fRedraw = TRUE;
                    
                }
                pTemp++;
            }
        }
    }

    if (!fRedraw) 
		return; 

    if (!SwitchKeybd()) 
		return;


    if (ISCHNKBD()) 
	{
		murmur("IM_Proceedkey::SetOpenstatus: true\n");
        ImmSetOpenStatus(NULL, TRUE);
	}
    else 
	{
		murmur("IM_Proceedkey::SetOpenstatus: false\n");
        ImmSetOpenStatus(NULL, FALSE);
		SendMessage(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
		//UIWndProc(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
		
	
	}
    
    // Reset the STK/TOG down key states.
    IM_SetDownKeys();

    // Redraw IM keyboard key
    IM_DrawArea();

    return;
}

//
// Handle mouse events.
//
__inline static LRESULT WINAPI IM_OnMouseEvent( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static RECT         rcKey={0};
	RECT rcTemp;
    static const KEYENTRY*    pKey    = NULL;
    static const KEYENTRY *pStkKey = NULL;

    const KEYENTRY *pTemp = NULL;

    switch( msg ) 
    {

    //
    // On a keydown, either press the button, or if it is sticky, toggle it.
    //
    case WM_LBUTTONDOWN:

        rcKey.left = LOWORD(lParam);
        rcKey.top = HIWORD(lParam);
		
        if( pKey = IM_GetKeyFromCoord( &rcKey ) ) 
        {
			
			if (pKey->fdwFlags & F_DISABLE || KEYSTATE(pKey) & STATE_DISABLED) 
			{
				pKey = NULL;
				break;
	        }
			
			pStkKey = NULL;
			
			if (pKey->fdwFlags & F_STK) 
			{
				FNKEYSTATE(pKey) ^= STATE_SET;

				if (ISKEYSET(pKey)) 
					pStkKey = pKey;
				//else 
				if (ISKEYLOCKED(pKey)) 
				{
					FNKEYSTATE(pKey) &= ~STATE_LOCKED;
					FNKEYSTATE(pKey) &= ~STATE_SET;
				}

				if (ISKEYSET(pKey)) 
					IM_PressKey(TRUE, pKey, &rcKey); 
			}
			else if (pKey->fdwFlags & F_TOG) 
			{
				if(pKey->fdwFlags & F_LOCK) 
						g_FnKeyStates[pKey->bVk] ^= (STATE_LOCKED);
								
				FNKEYSTATE(pKey) ^= STATE_SET;

				if (ISKEYSET(pKey))  
					IM_PressKey(TRUE, pKey, &rcKey); 
			} 
			else
				IM_PressKey(TRUE, pKey, &rcKey);      
            SetCapture( hwnd );
        }
        
		break;  


    //
    // on a keyup, unpress a non-sticky button.
    //
    case WM_LBUTTONUP:
        if (!pKey) 
			break;

        if (pKey->fdwFlags & F_DISABLE || KEYSTATE(pKey) & STATE_DISABLED) 
		{
            pKey = NULL;
            break;
        }

        if (pKey->fdwFlags & (F_STK|F_TOG)) 
		{
            if (!ISKEYSET(pKey)) 
				IM_PressKey(FALSE, pKey, &rcKey);
        } 
		else if (pKey->fdwFlags & F_MODE)
		{
            IM_PressKey(FALSE, pKey, &rcKey);
            SETMODE(pKey);
            ResetFnKeyState(TRUE);


        } 
		else 
		   IM_PressKey(FALSE, pKey, &rcKey);
        
        ReleaseCapture();
        IM_ProceedKey(pKey);
        pKey = NULL;
        break;
	 case WM_LBUTTONDBLCLK:
        rcTemp.left = (short)LOWORD(lParam);
        rcTemp.top  = (short)HIWORD(lParam);
        pTemp = IM_GetKeyFromCoord(&rcTemp);
		// check that it is same key or not. 
		// Cannot compare the pointers because in case of Shift, 
		// the poisition is the same but the pointer is different.
    	if (pStkKey && pTemp && EqualRect(&rcKey, &rcTemp)) 
		{
            if (ISKEYLOCKABLE(pStkKey)) 
				FNKEYSTATE(pStkKey) |= STATE_LOCKED;
            pStkKey = NULL;
        } 
		else 
            IM_OnMouseEvent(hwnd, WM_LBUTTONDOWN, wParam, lParam);
  
        break;

    default:
        break;
	}





    return 0;
}

// Handle the WM_TIMER message.
__inline static LRESULT WINAPI IM_OnTimer(HWND hWnd, WPARAM wId)
{
    switch(wId) {
    case TIMERID_REPEAT:
        KillTimer(hWnd, TIMERID_REPEAT);
        if (g_pRepeatKey) {
            //sndPlaySound(NULL, 0x40000000);
			if(g_pRepeatKey->fdwFlags & F_HOLD)
				g_bHoldKeySent = true;
            else if(g_pRepeatKey->fdwFlags & F_REPEAT)
				SetTimer(g_hwndMain, TIMERID_REPEAT, g_dwInitialDelay, NULL);
			if(g_pRepeatKey->fdwFlags & (F_HOLD |F_REPEAT))
				IM_SendKey(g_pRepeatKey, TRUE);
			murmur("IM_OnTimer:IM_SendKey.\n");
        }
        break;

    default:
        break;
    }

    return 0;
}

// Handle the WM_Onpaint message.
__inline static LRESULT WINAPI IM_OnPaint(HWND hWnd)
{
    PAINTSTRUCT ps;

    BeginPaint(hWnd, &ps);
    IM_DrawArea( ps.hdc, &ps.rcPaint );
    EndPaint(hWnd, &ps);

    return 0;
}
//
// Main window procedure.
//
LRESULT WINAPI ImWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	murmur("InWndProc:msg:%x, wparam:%x, lparam:%x",msg, wParam, lParam);
    HIDPI_InitScaling();
    switch( msg )
    {
	case WM_CREATE:{
		murmur("InWndProc:WM_CREATE");
        return IM_OnCreate();
		}
	case WM_DESTROY:{
		murmur("InWndProc:WM_DESTROY");
        return IM_OnDestroy();
					}
	case WM_PAINT:{
		murmur("InWndProc:WM_PAINT");
		return IM_OnPaint(hwnd);
				  }
  	case WM_WINDOWPOSCHANGED:
		murmur("InWndProc:WM_WINDOWPOSCHANGED");
		break;
	case WM_MOVE:
		murmur("InWndProc:WM_MOVE");
		break;
	case WM_SIZE:{
		murmur("InWndProc:WM_SIZE");
		if(!IsSIPOn()){ 
			murmur("IsSIPON:false, IMN_CLOSESTATUSWINDOW\n");
            ImmSetOpenStatus(NULL, FALSE);
			SendMessage(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
			//UIWndProc(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
		}
		break;
				 }
	case WM_SETCURSOR:
		murmur("InWndProc:WM_SETCURSOR");
		break;
	case WM_CAPTURECHANGED:
		murmur("InWndProc:WM_CAPTURECHANGED");
		break;
	case WM_LBUTTONDOWN:{
		murmur("InWndProc:WM_LBUTTONDOWN");
		return IM_OnMouseEvent( hwnd, msg, wParam, lParam );
						}
	case WM_LBUTTONUP:{
		murmur("InWndProc:WM_LBUTTONUP");
		return IM_OnMouseEvent( hwnd, msg, wParam, lParam );
					  }
	case WM_MOUSEMOVE:{
		murmur("InWndProc:WM_MOUSEMOVE");
		return IM_OnMouseEvent( hwnd, msg, wParam, lParam );
					  }
	case WM_LBUTTONDBLCLK:{
		murmur("InWndProc:WM_LBUTTONBLCK");
        return IM_OnMouseEvent( hwnd, msg, wParam, lParam );
						  }
	case WM_TIMER:{
		murmur("InWndProc:WM_TIMER");
        return IM_OnTimer(hwnd, wParam);
				  }


            
    } // switch( message )

    return DefWindowProc( hwnd, msg, wParam, lParam );
}


	
// Switch IME
__inline static void SwitchIME(void)
{
	

	murmur(" SwitchIME: modifier: %x; uVkey: %x\n", g_uModifiers, g_uVirtKey);
    // Press keys
    if (g_uModifiers & MOD_ALT) {
        keybd_event(VK_LMENU, 0, KEYEVENTF_SILENT, 0);
    }
    if (g_uModifiers & MOD_CONTROL) {
        keybd_event(VK_LCONTROL, 0, KEYEVENTF_SILENT, 0);
    }
    if (g_uModifiers & MOD_SHIFT) {
        keybd_event(VK_LSHIFT, 0, KEYEVENTF_SILENT, 0);
    }
    if (g_uVirtKey) {
        keybd_event((BYTE)g_uVirtKey, 0, KEYEVENTF_SILENT, 0);
    }

    // Release keys
    if (g_uVirtKey) {
        keybd_event((BYTE)g_uVirtKey, 0, KEYEVENTF_KEYUP|KEYEVENTF_SILENT, 0);
    }
    if (g_uModifiers & MOD_ALT) {
        keybd_event(VK_LMENU, 0, KEYEVENTF_KEYUP|KEYEVENTF_SILENT, 0);
    }
    if (g_uModifiers & MOD_CONTROL) {
        keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP|KEYEVENTF_SILENT, 0);
    }
    if (g_uModifiers & MOD_SHIFT) {
        keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP|KEYEVENTF_SILENT, 0);
    }


}





//
// IInputMethod implementation.
//

CInputMethod::CInputMethod( IUnknown *pUnkOuter, HINSTANCE hInstance )
{
	murmur("CInputMehod::CInputMethod");
    m_cRef = 0;
    g_dwObjectCount++;

    if( !pUnkOuter ) 
    {
        m_pUnkOuter = this;
    }
    else 
    {
        m_pUnkOuter = pUnkOuter;
    }



    return;
}

CInputMethod::~CInputMethod()
{
	murmur("CInputMehod::~CInputMethod");
    g_dwObjectCount--;

    return;
}


//
// IInputMethod methods.
//

STDMETHODIMP CInputMethod::Select( HWND hwndSip )
{
	murmur("CInputMehod::Select");

	
    WNDCLASS wc;

    ZeroMemory( &wc, sizeof(wc) );
    wc.lpfnWndProc = ImWndProc;
    wc.hInstance = g_hInstDll;
    wc.hbrBackground = NULL;
    wc.lpszClassName = g_ptszClassName;

    if( !RegisterClass( &wc ) ) 
    {
        return E_FAIL;
    }

    IM_SetDownKeys();

    g_hwndMain = CreateWindow(
                        g_ptszClassName,
                        TEXT(""),
                        WS_CHILD,
                        0,
                        0,
                        10,
                        10,
                        hwndSip,
                        (HMENU)NULL,
                        g_hInstDll,
                        NULL );
    g_bNullImc = (0 == ImmGetContext(GetForegroundKeyboardTarget()));
	//g_bNullImc = false;// (0 == ImmGetContext(hwndSip));

    HKL hImeKL= (HKL)IME_KBDLAYOUT;

    ImmGetHotKey(IME_SW_HOTKEY, &g_uModifiers, &g_uVirtKey, &hImeKL);
	murmur("HOTKEY modifier:%x, uVKey:%x\n",g_uModifiers, g_uVirtKey);
    SwitchIME();

    if (ISCHNKBD()) 
	{
		murmur("CInputMethod:Select : ImmSetopenStatus (true)");
        ImmSetOpenStatus(NULL, TRUE);
    }
	else 
	{
		murmur("CInputMethod:Select : ImmSetopenStatus (false)");
        ImmSetOpenStatus(NULL, FALSE);
		SendMessage(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
		//UIWndProc(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
    }


    ShowWindow( g_hwndMain, SW_SHOWNOACTIVATE );

    return NOERROR;
}


STDMETHODIMP CInputMethod::Deselect( void )
{
	murmur("CInputMehod::Deselect");
	
    ImageList_Destroy( g_hImagelistWide );
    ImageList_Destroy( g_hImagelistNarrow );
    g_hImagelistWide = g_hImagelistNarrow = NULL;
	
    DestroyWindow( g_hwndMain );
    UnregisterClass( g_ptszClassName, g_hInstDll );

    if (ISCHNKBD()) {
        ImmSetOpenStatus(NULL, FALSE);
		SendMessage(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
		//UIWndProc(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
		ImmSetOpenStatus(NULL, TRUE);
    }


    return NOERROR;
}
 

STDMETHODIMP CInputMethod::Showing( void )
{
	murmur("CInputMethod::Showing()\n");

    if (g_hwndMain)
    {
        if (ISCHNKBD())
		{
			murmur("CInputMethod::Showing:SetOpenstatus: true\n");
            ImmSetOpenStatus(NULL, TRUE);
		}
        else
		{
			murmur("CInputMethod::Showing:SetOpenstatus: false\n");

            ImmSetOpenStatus(NULL, FALSE);
			SendMessage(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
			//UIWndProc(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
			
		}
    }


    return NOERROR;
}


STDMETHODIMP CInputMethod::Hiding( void )
{
if (ResetFnKeyState(FALSE)) 
	{
        SwitchKeybd();
        IM_SetDownKeys();
    }
	
    //if (g_hImWnd && 	if(ISCHNKBD())
    //    ImmSetOpenStatus(NULL, FALSE);
	

    return NOERROR;
}


STDMETHODIMP CInputMethod::GetInfo( IMINFO *pimi )
{
	murmur("CInputMehod::Getinfo");
    HBITMAP hbm;


    if( !g_hImagelistWide ) 
    {

        g_hImagelistWide = ImageList_Create(
                            32,
                            16,
                            ILC_COLOR | ILC_MASK,
                            1,
                            1 );
        g_hImagelistNarrow = ImageList_Create(
                            16,
                            16,
                            ILC_COLOR | ILC_MASK,
                            1,
                            1 );


		wchar_t szBmpPath[MAX_PATH];
		swprintf(szBmpPath, TEXT("%siconl.bmp"), szDllPath);
		
		char buf [MAX_PATH];
		WideCharToMultiByte(CP_UTF8, 0, szBmpPath, (int)(wcslen(szBmpPath)+1), buf, MAX_PATH,NULL, NULL);
		murmur("Load dibbmp iconl.dmp:%s\n", buf);

		if( hbm = SHLoadDIBitmap(szBmpPath)) 
        {
			murmur("iconl.bmp loaded\n");
            ImageList_AddMasked( g_hImagelistWide, hbm, RGB(255,255,255) );
            DeleteObject( hbm );
        }

		swprintf(szBmpPath, TEXT("%sicons.bmp"), szDllPath);
		WideCharToMultiByte(CP_UTF8, 0, szBmpPath, (int)(wcslen(szBmpPath)+1), buf, MAX_PATH,NULL, NULL);
		murmur("Load dibbmp icons.dmp:%s\n", buf);

		if( hbm = SHLoadDIBitmap(szBmpPath) )
        {
			murmur("icons.bmp loaded\n");
            ImageList_AddMasked( g_hImagelistNarrow, hbm, RGB(255,255,255) );
            DeleteObject( hbm );
        }
    }

	RECT rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);

	int KeybdWidth = SCALEX(KEYBD_WIDTH);
	if(  rcWorkArea.right - rcWorkArea.left  != KeybdWidth) 
					 KeybdWidth= rcWorkArea.right - rcWorkArea.left;

    pimi->fdwFlags = SIPF_DOCKED;
    pimi->hImageNarrow = (HANDLE)g_hImagelistNarrow;
    pimi->hImageWide = (HANDLE)g_hImagelistWide;
	pimi->iNarrow = pimi->iWide = 0;
    pimi->rcSipRect.left = pimi->rcSipRect.top = 0;
    pimi->rcSipRect.right = KeybdWidth;// SCALEX(KEYBD_WIDTH);
    pimi->rcSipRect.bottom = SCALEY(KEYBD_HEIGHT);
	murmur("pimi->rc.right=%d; pimi->rc.bottom=%d\n",pimi->rcSipRect.right, pimi->rcSipRect.bottom);

    return NOERROR;
}



STDMETHODIMP CInputMethod::ReceiveSipInfo( SIPINFO *psi )
{

	murmur("CInputMehod::ReceiveSipInfo, SIPON:%d", IsSIPOn());
	if(psi->rcSipRect.left != 0){
		psi->rcSipRect.right+= psi->rcSipRect.left;
		psi->rcSipRect.left=0;
	}

	
	MoveWindow(
        g_hwndMain,
        0,
        0,
        psi->rcSipRect.right - psi->rcSipRect.left,
        psi->rcSipRect.bottom - psi->rcSipRect.top,
        FALSE );
	murmur("psi->rc.top=%d; psi->rc.bottom=%d\n",psi->rcSipRect.top, psi->rcSipRect.bottom);
	murmur("psi->rc.right=%d; psi->rc.left=%d\n",psi->rcSipRect.right, psi->rcSipRect.left);


// When SIP window closed by user, this is the only function invoked. Hide IME UI here.  
// Moved to WM_SIZE
//	if(!IsSIPOn()) 
//		SendMessage(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
	
	RECT rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
	int KeybdWidth = SCALEX(KEYBD_WIDTH);
	if (psi->rcSipRect.bottom - psi->rcSipRect.top < SCALEY(KEYBD_HEIGHT)
		|| psi->rcSipRect.right - psi->rcSipRect.left != rcWorkArea.right - rcWorkArea.left 
		|| rcWorkArea.right - rcWorkArea.left != KeybdWidth )
	{
		murmur("reset sip area\n");
		murmur("rcWorkArea.top=%d; psi->rcWorkArea.bottom=%d\n",rcWorkArea.top, rcWorkArea.bottom);
		murmur("rcWorkArea.right=%d; psi->rcWorkArea.left=%d\n",rcWorkArea.right, rcWorkArea.left);

		
		KeybdWidth= rcWorkArea.right - rcWorkArea.left;
		g_HIDPI_LogPixelsX =int( (float)KeybdWidth / (float(KEYBD_WIDTH)/192));
		

		IMINFO Info;	
		Info.cbSize = sizeof(Info);
		Info.fdwFlags = SIPF_DOCKED | SIPF_ON; 
		Info.hImageNarrow = (HANDLE)g_hImagelistNarrow;
		Info.hImageWide = (HANDLE)g_hImagelistWide;
		Info.iNarrow = Info.iWide = 0;
		Info.rcSipRect.left = 0;
		
		Info.rcSipRect.right = KeybdWidth;//SCALEX(KEYBD_WIDTH);
		Info.rcSipRect.bottom = rcWorkArea.bottom-rcWorkArea.top;
		Info.rcSipRect.top = Info.rcSipRect.bottom - SCALEY(KEYBD_HEIGHT);
		g_pIMCallback->SetImInfo(&Info);
		InvalidateRect(g_hwndMain, NULL, TRUE);
		
		
	}

    return NOERROR;
}


STDMETHODIMP CInputMethod::RegisterCallback( IIMCallback *pIMCallback )
{
	murmur("CInputMehod::RegisterCallback");
    g_pIMCallback = pIMCallback;
    return NOERROR;
}


STDMETHODIMP CInputMethod::GetImData( DWORD dwSize, void *pvImData )
{
    return E_NOTIMPL;
}


STDMETHODIMP CInputMethod::SetImData( DWORD dwSize, void *pvImData )
{
    return E_NOTIMPL;

}


STDMETHODIMP CInputMethod::UserOptionsDlg( HWND hwndParent )
{
    return E_NOTIMPL;
}

//
// IInputMethodEx methods
//

STDMETHODIMP CInputMethod::SetIMMActiveContext(HWND hWnd, BOOL bOpen,
                           DWORD dwConversion,DWORD dwSentence,DWORD hkl)
{
    //g_bNullImc = false;  (0 == ImmGetContext(hWnd));
    g_bNullImc = (0 == ImmGetContext(GetForegroundKeyboardTarget()));

	murmur("CInputMethod::SetIMMActiveContext. hWnd=%x,bOpen=%x, hkl:%x, IME_KBDLAYOUT=%x\n",hWnd,bOpen, hkl ,IME_KBDLAYOUT);
		
	if ((hkl != IME_KBDLAYOUT ))// && !g_bNullImc)
        SwitchIME();		

    if (SwitchKeybd())  IM_DrawArea(); 

    if (!g_bNullImc) {
        if (ISCHNKBD()) 
		{
			murmur("CInputMethod::SetIMMActiveContext:SetOpenstatus:true \n");
            ImmSetOpenStatus(NULL, TRUE);
        } 
		else 
		{
			murmur("CInputMethod::SetIMMActiveContext:SetOpenstatus: false\n");
            ImmSetOpenStatus(NULL, FALSE);
			SendMessage(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
			//UIWndProc(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
        }
    } 
	else 
	{
		murmur("CInputMethod::SetIMMActiveContext; g_bNullImc:SetOpenstatus:false.\n",false);
        ImmSetOpenStatus(NULL, FALSE);
		SendMessage(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
		//UIWndProc(FindWindow(UICLASSNAME, NULL),WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW,0);
    }


    return NOERROR;
}

STDMETHODIMP CInputMethod::RegisterCallbackEx(IIMCallbackEx *pIMCallbackEx)
{
    

    g_pIMCallbackEx = pIMCallbackEx;

    return NOERROR;
}


//
// IUnknown methods.
//

STDMETHODIMP CInputMethod::QueryInterface( REFIID riid, LPVOID FAR* ppobj )
{
    if( IID_IUnknown == riid || IID_IInputMethod == riid || IID_IInputMethodEx == riid) 
    {
        *ppobj = this;
        AddRef();
        return NOERROR;
    } 
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CInputMethod::AddRef( void )
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CInputMethod::Release( void )
{
    if( --m_cRef ) 
    {
        return m_cRef;
    }
    delete this;
    return 0;
}
