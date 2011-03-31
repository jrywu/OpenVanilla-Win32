
// Registry key definition
const static TCHAR g_pszRegInternational[]=TEXT("SOFTWARE\\Microsoft\\International");
const static TCHAR g_pszRegLCID[]=TEXT("LCID");
const static TCHAR g_pszRegKeybd[]=TEXT("ControlPanel\\Keybd");
const static TCHAR g_pszRegInitialDelay[]=TEXT("InitialDelay");
const static TCHAR g_pszRegRepeatRate[]=TEXT("RepeatRate");
const static TCHAR g_pszRegKBMode[]=TEXT("KBMode");
const static TCHAR g_pszIMClsidKey[]=TEXT("CLSID\\{E80D7526-58DF-4142-A958-72490A691B46}");
const static TCHAR g_ptszClassName[]=TEXT("MVIMWnd");

//
// Globals.
//
WCHAR UICLASSNAME[32];
BOOL    g_bNullImc = FALSE;


static IIMCallback* g_pIMCallback = NULL;
static IIMCallbackEx* g_pIMCallbackEx = NULL;


//UINT				g_uLCID;

// GDI resources
static HBITMAP		g_hKeybdBmp[NUM_KEYBDS], g_hOldKeybdBmp;
static HIMAGELIST   g_hImagelistWide, g_hImagelistNarrow;
static HDC			g_hKeybdDC;
// Globals- internal
static int		g_nDownKeys = 0, g_nDown;
static bool		g_bHoldKeySent = false;
static DWORD    g_dwInitialDelay = 500, g_dwRepeatRate = 20;
static DWORD    g_dwKBMode = 0;
static UINT     g_uModifiers = 0, g_uVirtKey = 0;
static HWND     g_hwndMain;
static const KEYENTRY *g_pRepeatKey = NULL;
wchar_t szDllPath[MAX_PATH] = TEXT("");


// Misc definition
#define TIMERID_REPEAT      500


