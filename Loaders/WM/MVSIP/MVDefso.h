
typedef struct {
    BYTE    bVk;
    UINT    uChar;
    UINT    uFullChar;
    UINT    uCtrl;
    DWORD   fdwFlags;
    int     nLeft;
} KEYENTRY;

//


// Flags for fdwFlags member of KEYENTRY.
#define F_VK                0x00010000      // VK key
#define F_VVK               0x00020000      // vitual VK key
#define F_STK               0x00040000      // sticky Key
#define F_TOG               0x00080000      // toggle key
#define F_MODE              0x00100000      // mode changing key
#define F_NOSHIFT           0x00200000      // VK key don't want SHIFT key down
#define F_NOCTRL            0x00400000      // The key wo't effect under CONTROL mode
#define F_NOLOCK            0x00800000      // This key can not be locked
#define F_POSTSK            0x01000000      // post send key
#define F_REPEAT            0x10000000      // repeat key
#define F_REDRAW            0x20000000      // redraw window when state changes
#define F_COMPLEX           0x40000000      // The complex key
#define F_DISABLE           0x80000000      // disabled key

#define KEYINDEX_MASK       0x000000FF      // key index (last byte)


// Misc definition
#define TIMERID_REPEAT      500

// Keyboard information
#define NUM_KEYBDS          4
#define NUM_ROWS            5
#define MAX_KEYS_IN_ROW     14
#define NUM_ROWS_LARGEKB    4
#define NUM_ROWS_SMALLKB    5
#define MAX_NUM_ROWS        NUM_ROWS_SMALLKB
#define MAX_KEYS_IN_ROW     14
#define NUM_MODEKEYS        3
#define NUM_FUNKEYS         3

#define NUM_COMPLEXRECTS    2

#define KEYBD_HEIGHT        160//80
#define KEYBD_WIDTH         480//240


// Flags for KEYSTATE
#define STATE_DOWN          0x01
#define STATE_SET           0x02
#define STATE_LOCKED        0x04
#define STATE_DISABLED      0x08
#define STATE_PREVSET       0x10

// Private Virtual VK key definition
#define VVK_CHINESE         VK_KANJI
#define VVK_ENGLISH         VK_DBE_ALPHANUMERIC
#define VVK_SYMBOL          VK_MBUTTON
#define VVK_SHAPE           VK_CONVERT
#define VVK_SHIFT           VK_SHIFT
#define VVK_CAPITAL         VK_CAPITAL
#define VVK_CONTROL         VK_CONTROL
#define VVK_ALPHANUM        VK_DBE_ALPHANUMERIC
#define NO_CTL              0xFFFF
#define SENTINEL            VK_END

#define VK_PACKET		0xE7

// Index of Function Key
#define FN_SHAPE            0x01
#define FN_SHIFT            0x02
#define FN_CONTROL          0x03

// Index of Mode
#define MODE_CHINESE        0x01
#define MODE_ENGLISH        0x02
#define MODE_SYMBOL         0x03

#define NULLIMC_KBD         MODE_ENGLISH




//
// Prototypes.
//
//static BOOL WINAPI IM_PressKey( BOOL fPress, KEYENTRY *pKey, RECT *prcKey );


//
// Macros
//
#ifndef ISCHNKBD
#define ISCHNKBD()          (g_nCurKB == 0)
#endif
#define KBDBMOFFSET()       (g_nCurKB * KEYBD_HEIGHT)

#define KEYSTATE(p)         (((BYTE *) g_KeyStates)[(p - g_keys[0][0])])
#define FNKEYSTATE(p)       (g_FnKeyStates[p->fdwFlags & KEYINDEX_MASK])
#define FNKEYSTATE2(index)  (g_FnKeyStates[index])

#define GETKEYENTRY(r,c)    (g_keys[g_nCurKB][r] + c)
#define GETKEYINDEX(p)      (p->fdwFlags & KEYINDEX_MASK)

#define ISNORMALKEY(p)      (!(p->fdwFlags & (F_STK|F_TOG|F_MODE|F_VVK)))
#define ISFNKEY(p)          (p->fdwFlags & (F_STK|F_TOG))
#define ISKEYLOCKABLE(p)    (!(p->fdwFlags & F_NOLOCK))

#define ISKEYSET(p)         (g_FnKeyStates[p->fdwFlags & KEYINDEX_MASK] & STATE_SET)
#define ISKEYSET2(index)    (g_FnKeyStates[index] & STATE_SET)
#define ISKEYLOCKED(p)      (g_FnKeyStates[p->fdwFlags & KEYINDEX_MASK] & STATE_LOCKED)

#ifdef NUM_MODEKEYS
#define SETMODE(p)          {memset(g_ModeKeyStates, 0, sizeof(g_ModeKeyStates)); g_ModeKeyStates[p->fdwFlags & KEYINDEX_MASK] |= STATE_SET;}
#define SETMODE2(index)     {memset(g_ModeKeyStates, 0, sizeof(g_ModeKeyStates)); g_ModeKeyStates[index] |= STATE_SET;}
#define SETCHNMODE()        {memset(g_ModeKeyStates, 0, sizeof(g_ModeKeyStates)); g_ModeKeyStates[0] |= STATE_SET;}
#else
#define SETMODE(p)          (TRUE)
#define SETMODE2(index)     (TRUE)
#define SETCHNMODE()        (TRUE)
#endif // NUM_MODEKEYS

#ifdef NUM_COMPLEXRECTS
extern const RECT           g_rcComplex[NUM_COMPLEXRECTS];

#define GETCOMPLEXRECT(n)   (g_rcComplex + n)
#else
#define GETCOMPLEXRECT(n)   NULL
#endif // g_rcComplex

#ifdef FN_SHAPE
#define SHAPED()            (g_FnKeyStates[FN_SHAPE] & STATE_SET)
#else
#define SHAPED()            FALSE
#endif // FN_SHAPE

#ifdef FN_CONTROL
#define CONTROLED()         (g_FnKeyStates[FN_CONTROL] & STATE_SET)
#define ISCONTROLLOCKED()   (g_FnKeyStates[FN_CONTROL] & STATE_LOCKED)
#define RESETCONTROL()      (g_FnKeyStates[FN_CONTROL] &= ~(STATE_SET|STATE_LOCKED))
#else
#define CONTROLED()         FALSE
#define ISCONTROLLOCKED()   FALSE
#define RESETCONTROL()      TRUE
#endif // FN_CONTROL

#ifdef FN_SHIFT
#define SHIFTED()           (g_FnKeyStates[FN_SHIFT] & STATE_SET)
#define ISSHIFTLOCKED()     (g_FnKeyStates[FN_SHIFT] & STATE_LOCKED)
#define RESETSHIFT()        (g_FnKeyStates[FN_SHIFT] &= ~(STATE_SET|STATE_LOCKED))
#else
#define SHIFTED()           FALSE
#define ISSHIFTLOCKED()     FALSE
#define RESETSHIFT()        TRUE
#endif // FN_SHFIT

#ifndef DISABLEDKEYNUM
#define DISABLEDKEYNUM      0
#endif // DISABLEDKEYNUM

#ifndef DISABLEDBMPOFFSET
#define DISABLEDBMPOFFSET   0
#endif // DISABLEDBMPOFFSET

#ifndef NULLIMC_KBD
#define NULLIMC_KBD         0
#endif // NULLIMC_KBD

#ifndef GETKBMODE
#define GETKBMODE()         ((DWORD)g_nCurKB)
#endif // GETKBMODE()

//
// HIDPI macros.
//

int g_HIDPI_LogPixelsX;
int g_HIDPI_LogPixelsY;

#define SCALEX(argX) (((argX) * g_HIDPI_LogPixelsX)/192)
#define SCALEY(argY) (((argY) * g_HIDPI_LogPixelsY)/192)
#define UNSCALEX(argX) (((argX) * 192)/g_HIDPI_LogPixelsX)
#define UNSCALEY(argY) (((argY) * 192)/g_HIDPI_LogPixelsY)


// Registry key definition
const static TCHAR g_pszRegInternational[]=TEXT("SOFTWARE\\Microsoft\\International");
const static TCHAR g_pszRegLCID[]=TEXT("LCID");
const static TCHAR g_pszRegKeybd[]=TEXT("ControlPanel\\Keybd");
const static TCHAR g_pszRegInitialDelay[]=TEXT("InitialDelay");
const static TCHAR g_pszRegRepeatRate[]=TEXT("RepeatRate");
const static TCHAR g_pszRegKBMode[]=TEXT("KBMode");
const static TCHAR g_pszIMClsidKey[]=TEXT("CLSID\\{E80D7526-58DF-4142-A958-72490A691B46}");
const static TCHAR g_ptszClassName[]=TEXT("MVIMWnd");
