#ifndef __IM_H__
#define __IM_H__


#include <aygshell.h>
#include "macros.h"

#include "MVsip.h"


BEGIN_INTERFACE_PART(CInputMethod, IInputMethod)

public:

    //
    // Ctor, Dtor
    //

    CInputMethod( IUnknown *pUnkOuter, HINSTANCE hInstance );
    ~CInputMethod();


    //
    // IInputMethod methods
    //

    STDMETHOD(Select) (HWND hwndSip);
    STDMETHOD(Deselect) (void);
    STDMETHOD(Showing) (void);
    STDMETHOD(Hiding) (void);
    STDMETHOD(GetInfo) (IMINFO *pimi);
    STDMETHOD(ReceiveSipInfo) (SIPINFO *psi);
    STDMETHOD(RegisterCallback) (IIMCallback *pIMCallback);
    STDMETHOD(GetImData) (DWORD dwSize, void *pvImData);
    STDMETHOD(SetImData) (DWORD dwSize, void *pvImData);
    STDMETHOD(UserOptionsDlg) (HWND hwndParent);

private:

    int m_cRef;
    LPUNKNOWN m_pUnkOuter;

END_INTERFACE_PART(CInputMethod)

//Some defines
//
// Flags for fdwFlags member of KEYENTRY.
//
#define F_VK                0x00000001      // Use SendVirtualKey
#define F_STK               0x00000002      // Key is 'sticky'
#define F_DOWN              0x00000004      // Key is down
#define F_REDRAW            0x00000008      // Redraw window when state changes
#define F_SHARED            0x00000010      // When switching KB's, new KB will assume state of key from prev. KB

#define F_STK_FRIEND        0x00000040      // When pressed, will NOT unstick a F_STK button.
#define F_PAINTED_DOWN      0x00000080
#define F_CAPTURED          0x00000100
#define F_ASCIIMODE         0x00000200
#define F_NUMERICMODE       0x00000400
#define F_INTLMODE          0x00000800
#define F_ASCII_LS_MODE     0x00001000
#define F_INTL_LS_MODE      0x00002000
#define F_ASCII_SMALL_MODE  0x00004000
#define F_INTL_SMALL_MODE   0x00008000
#define F_ASCII_SS_MODE     0x00010000
#define F_INTL_SS_MODE      0x00020000
#define F_NOUPSWIPE         0x00040000
#define F_COMPLEXDRAW       0x00080000


#define NUM_KEYBDS          9
#define NUM_ROWS_LARGEKB    4
#define NUM_ROWS_SMALLKB    5
#define MAX_NUM_ROWS        NUM_ROWS_SMALLKB
#define MAX_KEYS_IN_ROW     14
#define BITMAP_HEIGHT       80
#define BITMAP_WIDTH        240
#define SENTINEL            VK_END

// Private VK values.
#define VVK_NUMERIC         VK_MBUTTON
#define VVK_INTL            VK_RBUTTON
#define VVK_INTL_LS         VK_PAUSE
#define NO_CTL              0xFFFFFFFF

//Keyboard bitmap offsets into our mondo bitmap
#define ASCII_BM_OFFSET                   0
#define ASCII_SHIFTED_BM_OFFSET          80
#define NUMERIC_BM_OFFSET               160
#define INTL_BM_OFFSET                  240
#define INTL_SHIFTED_BM_OFFSET          320
#define SWIPE_OFFSET                    400
#define SWIPE_INTL_OFFSET               480
#define ASCII_SMALL_BM_OFFSET           560
#define ASCII_SMALL_SHIFTED_BM_OFFSET   640
#define INTL_SMALL_BM_OFFSET            720
#define INTL_SMALL_SHIFTED_BM_OFFSET    800
#define SWIPE_SMALL_OFFSET              880
#define SWIPE_SMALL_SHLOCK_OFFSET       960
#define SWIPE_SMALL_INTL_OFFSET        1040
#define SWIPE_SMALL_INTL_SHLOCK_OFFSET 1120

#define SWIPE_LENGTH		    20			// For stroke capability 
#define SWIPE_SPACE         0x0001
#define SWIPE_BACKSPACE     0x0002
#define SWIPE_UPCASE        0x0004
#define SWIPE_ENTER         0x0008

//These are the indexes into g_keys for each keyboard
#define ASCIIKB                  0
#define NUMERICKB                1
#define INTLKB                   2
//Large swipe keyboards
#define ASCIIKB_LS               3
#define INTLKB_LS                4

#define ASCIIKB_SMALL            5
#define INTLKB_SMALL             6
#define ASCIIKB_SS               7
#define INTLKB_SS                8

#define LK_BACKKEYCOL           11
#define LK_BACKKEYROW            0
#define LK_ENTERKEYCOL          10
#define LK_ENTERKEYROW           2
#define SK_ENTERKEYCOL          11
#define SK_ENTERKEYROW           3
    
//Function prototypes
UINT IM_ReadRegistry(void);
BOOL IM_OptionsDlg(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);


#ifndef ZeroMemory
#define ZeroMemory(dest,length) memset(dest,0,length)
#endif


#endif __IM_H__