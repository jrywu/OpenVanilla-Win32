#ifndef __MVSIP_H__
#define __MVSIP_H__
#include <aygshell.h>
#include "macros.h"
#include "MVSIP_i.h"

BEGIN_INTERFACE_PART(CInputMethod, IInputMethod)

public:

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

public: // IInputMethodEx methods
    STDMETHOD(SetIMMActiveContext) (HWND,BOOL bOpen,DWORD dwConversion,DWORD dwSentence,DWORD hkl);
    STDMETHOD(RegisterCallbackEx) (IIMCallbackEx *pIMCallbackEx);

private:

    int m_cRef;
    LPUNKNOWN m_pUnkOuter;

END_INTERFACE_PART(CInputMethod)



#endif //__MVSIP_H__