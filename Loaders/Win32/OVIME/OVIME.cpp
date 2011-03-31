//#define OV_DEBUG
#include "OVIME.h"
#include "OVUtility.h"
/**********************************************************************/
// For wince														  */
#ifdef WINCE
#include "uiprivate.h"

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
    if (!GetClassInfo(hInstance, UICLASSNAME, &wcWndCls)) {
        wcWndCls.lpfnWndProc   = UIWndProc;
		wcWndCls.lpszClassName = UICLASSNAME;//v_szImeUIClassName;

        RegisterClass(&wcWndCls);
    }

    // IME Cand class
    if (!GetClassInfo(hInstance, UICANDCLASSNAME, &wcWndCls)) {
        wcWndCls.lpfnWndProc   = CandWndProc;
        wcWndCls.lpszClassName = UICANDCLASSNAME;

        RegisterClass(&wcWndCls);
    }

    // IME Comp Class
    //if (!GetClassInfo(hInstance, UICOMPCLASSNAME, &wcWndCls)) {
    //    wcWndCls.lpfnWndProc   = CompWndProc;
    //    wcWndCls.lpszClassName = UICOMPCLASSNAME;

    //    RegisterClass(&wcWndCls);
    //}

    //if (!GetClassInfo(hInstance, UISTATUSCLASSNAME, &wcWndCls)) {
    //    wcWndCls.lpfnWndProc   = StatusWndProc;
    //    wcWndCls.lpszClassName = UISTATUSCLASSNAME;

    //    RegisterClass(&wcWndCls);
    //}
    return;
}

/**********************************************************************/
/* ImeUnregisterUIClass()                                             */
/**********************************************************************/
void ImeUnregisterUIClass(HINSTANCE hInstance)
{
    WNDCLASS   wcWndCls;

    if (GetClassInfo(hInstance, UICLASSNAME, &wcWndCls)) {
        UnregisterClass(UICLASSNAME, hInstance);
    }

//    if (GetClassInfo(hInstance, UICOMPCLASSNAME, &wcWndCls)) {
//        UnregisterClass(UICOMPCLASSNAME, hInstance);
//    }

    if (GetClassInfo(hInstance, UICANDCLASSNAME, &wcWndCls)) {
        UnregisterClass(UICANDCLASSNAME, hInstance);
    }

//    if (GetClassInfo(hInstance, UISTATUSCLASSNAME, &wcWndCls)) {
//        UnregisterClass(UISTATUSCLASSNAME, hInstance);
//    }
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
    int     i;


/*
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

    g_sImeUIG.crWindow = 0x0;//GetSysColor(COLOR_WINDOW);
    g_sImeUIG.crWindowText = 0xffffff;//GetSysColor(COLOR_WINDOWTEXT);
    g_sImeUIG.crHighlight = 0xffffff;//GetSysColor(COLOR_HIGHLIGHT);
    g_sImeUIG.crHighlightText = 0x0;//GetSysColor(COLOR_HIGHLIGHTTEXT);
    g_sImeUIG.crBtnFace = GetSysColor(COLOR_BTNFACE);
    g_sImeUIG.crBtnText = GetSysColor(COLOR_BTNTEXT);

    hDC = GetDC(NULL);

	hSysFont = (HFONT)GetStockObject(SYSTEM_FONT);
    GetObject(hSysFont, sizeof(LOGFONT), &lfFont);

    HDC screen = GetDC(NULL);  // GetDC of entire screen
    double scaleY = GetDeviceCaps(screen, LOGPIXELSY) / 96.0;
    ReleaseDC(NULL, screen);

    lfFont.lfWidth = 0;
    lfFont.lfHeight = (int) ((18) * scaleY);//SCALEY(12);
    lfFont.lfWeight = FW_NORMAL;

    g_sImeUIG.hFont  =	 CreateFontIndirect(&lfFont);

    hOldFont = (HFONT)SelectObject(hDC, g_sImeUIG.hFont);
	GetTextExtentPoint(hDC, szCandChar, lstrlen(szCandChar), &lSize);

    g_sImeUIG.nCandCharWi = lSize.cx;

    //g_sImeUIG.nCandLineWi = lSize.cx*2 + CAND_TEXTMARGIN * 2;
    g_sImeUIG.nCandLineHi = (int)( lSize.cy *1.2 );

    g_sImeUIG.nCandDy = g_sImeUIG.nCandLineHi;//CAND_TEXTMARGIN;
    for (i=0; i <= MAX_PAGESIZE; i++) {
		g_sImeUIG.nCandDx[i] = g_sImeUIG.nCandCharWi * 2 * i ;
    }


    SelectObject(hDC, hOldFont);
    ReleaseDC(NULL, hDC);

    return;
}

/**********************************************************************/
/* DestroyUIGlobalData()                                              */
/**********************************************************************/
void DestroyUIGlobalData(void)
{
    if (g_sImeUIG.hFont) {
        DeleteObject(g_sImeUIG.hFont);
        g_sImeUIG.hFont = NULL;
    }

    return;
}

#endif

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  dwReason, 
                       LPVOID lpReserved
					 )
{	


#ifdef WINCE
	static  HANDLE hMapFile1 = NULL;
//	BOOL    fInit;
#endif

	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
		hInst = (HINSTANCE)hModule;

		AVLoader* loader = AVLoader::open();
		loader->connectDisplayServer(dsvr);

		

#ifdef WINCE
		// UI init.
		g_hInst = (HINSTANCE)hModule;
        DisableThreadLibraryCalls((HMODULE)hModule);
        ImeRegisterUIClass((HINSTANCE)hModule);
        InitUIGlobalData();

#else
		WNDCLASSEX wc;
		wc.cbSize			= sizeof(WNDCLASSEX);
		wc.style			= CS_OVIME | CS_IME;
		wc.lpfnWndProc		= UIWndProc;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 2 * sizeof(LONG);
		wc.hInstance		= (HINSTANCE)hModule;
		wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
		wc.hIcon			= NULL;
		wc.lpszMenuName		= (LPTSTR)NULL;
		wc.lpszClassName	= UICLASSNAME;
		wc.hbrBackground	= NULL;
		wc.hIconSm			= NULL;

		//IMEUIRegisterClass( (HINSTANCE)hModule );


		if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		  return FALSE;

#endif
		
		break;
		}
	case DLL_PROCESS_DETACH:
		// free UI library
		AVLoader::close();
#ifdef WINCE
		ImeUnregisterUIClass((HINSTANCE)hModule);
        DestroyUIGlobalData();


#else
		UnregisterClass(UICLASSNAME, (HINSTANCE)hModule);
#endif
		
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
    return TRUE;
}
