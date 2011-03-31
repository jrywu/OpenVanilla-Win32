//
// KB Macros
//
//#ifndef ISCHNKBD
#define ISCHNKBD()          (g_nCurKB == 0)
//#endif
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
//#define CONTROLED()         (g_FnKeyStates[FN_CONTROL] & STATE_SET)
#define ISCONTROLLOCKED()   (g_FnKeyStates[FN_CONTROL] & STATE_LOCKED)
#define RESETCONTROL()      (g_FnKeyStates[FN_CONTROL] &= ~(STATE_SET|STATE_LOCKED))
#else
#define CONTROLED()         FALSE
#define ISCONTROLLOCKED()   FALSE
#define RESETCONTROL()      TRUE
#endif // FN_CONTROL

#define SYMBOLED()         (g_FnKeyStates[FN_SYMBOL] & STATE_SET)
#define ISSYMBOLLOCKED()   (g_FnKeyStates[FN_SYMBOL] & STATE_LOCKED)
#define RESETSYMBOL()		(g_FnKeyStates[FN_SYMBOL] &= ~(STATE_SET|STATE_LOCKED))

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

//#ifndef NULLIMC_KBD
//#define NULLIMC_KBD         0
//#endif // NULLIMC_KBD

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


