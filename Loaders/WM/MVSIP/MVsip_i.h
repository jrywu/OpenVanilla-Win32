/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun May 24 03:30:49 2009
 */
/* Compiler settings for .\MVsip.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __MVsip_i_h__
#define __MVsip_i_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IIMCallback_FWD_DEFINED__
#define __IIMCallback_FWD_DEFINED__
typedef interface IIMCallback IIMCallback;
#endif 	/* __IIMCallback_FWD_DEFINED__ */


#ifndef __IIMCallbackEx_FWD_DEFINED__
#define __IIMCallbackEx_FWD_DEFINED__
typedef interface IIMCallbackEx IIMCallbackEx;
#endif 	/* __IIMCallbackEx_FWD_DEFINED__ */


#ifndef __IInputMethod_FWD_DEFINED__
#define __IInputMethod_FWD_DEFINED__
typedef interface IInputMethod IInputMethod;
#endif 	/* __IInputMethod_FWD_DEFINED__ */


#ifndef __IInputMethodEx_FWD_DEFINED__
#define __IInputMethodEx_FWD_DEFINED__
typedef interface IInputMethodEx IInputMethodEx;
#endif 	/* __IInputMethodEx_FWD_DEFINED__ */


#ifndef __CMSMVIm_FWD_DEFINED__
#define __CMSMVIm_FWD_DEFINED__

#ifdef __cplusplus
typedef class CMSMVIm CMSMVIm;
#else
typedef struct CMSMVIm CMSMVIm;
#endif /* __cplusplus */

#endif 	/* __CMSMVIm_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __MVSIPLib_LIBRARY_DEFINED__
#define __MVSIPLib_LIBRARY_DEFINED__

/* library MVSIPLib */
/* [helpstring][version][uuid] */ 

typedef struct  _tagImInfo
    {
    DWORD cbSize;
    HANDLE hImageNarrow;
    HANDLE hImageWide;
    int iNarrow;
    int iWide;
    DWORD fdwFlags;
    RECT rcSipRect;
    }	IMINFO;


EXTERN_C const IID LIBID_MVSIPLib;

#ifndef __IIMCallback_INTERFACE_DEFINED__
#define __IIMCallback_INTERFACE_DEFINED__

/* interface IIMCallback */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID_IIMCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("42429669-ae04-11d0-a4f8-00aa00a749b9")
    IIMCallback : public IUnknown
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetImInfo( 
            IMINFO __RPC_FAR *pimi) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendVirtualKey( 
            BYTE bVK,
            DWORD dwFlags) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendCharEvents( 
            UINT uVK,
            UINT uKeyFlags,
            UINT uChars,
            UINT __RPC_FAR *puShift,
            UINT __RPC_FAR *puChars) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendString( 
            BSTR ptszStr,
            DWORD dwChars) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IIMCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IIMCallback __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IIMCallback __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IIMCallback __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetImInfo )( 
            IIMCallback __RPC_FAR * This,
            IMINFO __RPC_FAR *pimi);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendVirtualKey )( 
            IIMCallback __RPC_FAR * This,
            BYTE bVK,
            DWORD dwFlags);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendCharEvents )( 
            IIMCallback __RPC_FAR * This,
            UINT uVK,
            UINT uKeyFlags,
            UINT uChars,
            UINT __RPC_FAR *puShift,
            UINT __RPC_FAR *puChars);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendString )( 
            IIMCallback __RPC_FAR * This,
            BSTR ptszStr,
            DWORD dwChars);
        
        END_INTERFACE
    } IIMCallbackVtbl;

    interface IIMCallback
    {
        CONST_VTBL struct IIMCallbackVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIMCallback_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IIMCallback_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IIMCallback_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IIMCallback_SetImInfo(This,pimi)	\
    (This)->lpVtbl -> SetImInfo(This,pimi)

#define IIMCallback_SendVirtualKey(This,bVK,dwFlags)	\
    (This)->lpVtbl -> SendVirtualKey(This,bVK,dwFlags)

#define IIMCallback_SendCharEvents(This,uVK,uKeyFlags,uChars,puShift,puChars)	\
    (This)->lpVtbl -> SendCharEvents(This,uVK,uKeyFlags,uChars,puShift,puChars)

#define IIMCallback_SendString(This,ptszStr,dwChars)	\
    (This)->lpVtbl -> SendString(This,ptszStr,dwChars)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IIMCallback_SetImInfo_Proxy( 
    IIMCallback __RPC_FAR * This,
    IMINFO __RPC_FAR *pimi);


void __RPC_STUB IIMCallback_SetImInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IIMCallback_SendVirtualKey_Proxy( 
    IIMCallback __RPC_FAR * This,
    BYTE bVK,
    DWORD dwFlags);


void __RPC_STUB IIMCallback_SendVirtualKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IIMCallback_SendCharEvents_Proxy( 
    IIMCallback __RPC_FAR * This,
    UINT uVK,
    UINT uKeyFlags,
    UINT uChars,
    UINT __RPC_FAR *puShift,
    UINT __RPC_FAR *puChars);


void __RPC_STUB IIMCallback_SendCharEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IIMCallback_SendString_Proxy( 
    IIMCallback __RPC_FAR * This,
    BSTR ptszStr,
    DWORD dwChars);


void __RPC_STUB IIMCallback_SendString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IIMCallback_INTERFACE_DEFINED__ */


#ifndef __IIMCallbackEx_INTERFACE_DEFINED__
#define __IIMCallbackEx_INTERFACE_DEFINED__

/* interface IIMCallbackEx */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID_IIMCallbackEx;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9DDB3920-3606-11d2-A2EB-0000F8757270")
    IIMCallbackEx : public IIMCallback
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetImInfo( 
            long __RPC_FAR *pimi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SendVirtualKey( 
            BYTE bVK,
            DWORD dwFlags) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SendCharEvents( 
            UINT uVK,
            UINT uKeyFlags,
            UINT uChars,
            UINT __RPC_FAR *puShift,
            UINT __RPC_FAR *puChars) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SendString( 
            BSTR ptszStr,
            DWORD dwChars) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SendAlternatives( 
            long __RPC_FAR *plmd) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IIMCallbackExVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IIMCallbackEx __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IIMCallbackEx __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IIMCallbackEx __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetImInfo )( 
            IIMCallbackEx __RPC_FAR * This,
            IMINFO __RPC_FAR *pimi);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendVirtualKey )( 
            IIMCallbackEx __RPC_FAR * This,
            BYTE bVK,
            DWORD dwFlags);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendCharEvents )( 
            IIMCallbackEx __RPC_FAR * This,
            UINT uVK,
            UINT uKeyFlags,
            UINT uChars,
            UINT __RPC_FAR *puShift,
            UINT __RPC_FAR *puChars);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendString )( 
            IIMCallbackEx __RPC_FAR * This,
            BSTR ptszStr,
            DWORD dwChars);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetImInfo )( 
            IIMCallbackEx __RPC_FAR * This,
            long __RPC_FAR *pimi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendVirtualKey )( 
            IIMCallbackEx __RPC_FAR * This,
            BYTE bVK,
            DWORD dwFlags);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendCharEvents )( 
            IIMCallbackEx __RPC_FAR * This,
            UINT uVK,
            UINT uKeyFlags,
            UINT uChars,
            UINT __RPC_FAR *puShift,
            UINT __RPC_FAR *puChars);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendString )( 
            IIMCallbackEx __RPC_FAR * This,
            BSTR ptszStr,
            DWORD dwChars);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendAlternatives )( 
            IIMCallbackEx __RPC_FAR * This,
            long __RPC_FAR *plmd);
        
        END_INTERFACE
    } IIMCallbackExVtbl;

    interface IIMCallbackEx
    {
        CONST_VTBL struct IIMCallbackExVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIMCallbackEx_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IIMCallbackEx_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IIMCallbackEx_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IIMCallbackEx_SetImInfo(This,pimi)	\
    (This)->lpVtbl -> SetImInfo(This,pimi)

#define IIMCallbackEx_SendVirtualKey(This,bVK,dwFlags)	\
    (This)->lpVtbl -> SendVirtualKey(This,bVK,dwFlags)

#define IIMCallbackEx_SendCharEvents(This,uVK,uKeyFlags,uChars,puShift,puChars)	\
    (This)->lpVtbl -> SendCharEvents(This,uVK,uKeyFlags,uChars,puShift,puChars)

#define IIMCallbackEx_SendString(This,ptszStr,dwChars)	\
    (This)->lpVtbl -> SendString(This,ptszStr,dwChars)


#define IIMCallbackEx_SetImInfo(This,pimi)	\
    (This)->lpVtbl -> SetImInfo(This,pimi)

#define IIMCallbackEx_SendVirtualKey(This,bVK,dwFlags)	\
    (This)->lpVtbl -> SendVirtualKey(This,bVK,dwFlags)

#define IIMCallbackEx_SendCharEvents(This,uVK,uKeyFlags,uChars,puShift,puChars)	\
    (This)->lpVtbl -> SendCharEvents(This,uVK,uKeyFlags,uChars,puShift,puChars)

#define IIMCallbackEx_SendString(This,ptszStr,dwChars)	\
    (This)->lpVtbl -> SendString(This,ptszStr,dwChars)

#define IIMCallbackEx_SendAlternatives(This,plmd)	\
    (This)->lpVtbl -> SendAlternatives(This,plmd)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IIMCallbackEx_SetImInfo_Proxy( 
    IIMCallbackEx __RPC_FAR * This,
    long __RPC_FAR *pimi);


void __RPC_STUB IIMCallbackEx_SetImInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IIMCallbackEx_SendVirtualKey_Proxy( 
    IIMCallbackEx __RPC_FAR * This,
    BYTE bVK,
    DWORD dwFlags);


void __RPC_STUB IIMCallbackEx_SendVirtualKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IIMCallbackEx_SendCharEvents_Proxy( 
    IIMCallbackEx __RPC_FAR * This,
    UINT uVK,
    UINT uKeyFlags,
    UINT uChars,
    UINT __RPC_FAR *puShift,
    UINT __RPC_FAR *puChars);


void __RPC_STUB IIMCallbackEx_SendCharEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IIMCallbackEx_SendString_Proxy( 
    IIMCallbackEx __RPC_FAR * This,
    BSTR ptszStr,
    DWORD dwChars);


void __RPC_STUB IIMCallbackEx_SendString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IIMCallbackEx_SendAlternatives_Proxy( 
    IIMCallbackEx __RPC_FAR * This,
    long __RPC_FAR *plmd);


void __RPC_STUB IIMCallbackEx_SendAlternatives_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IIMCallbackEx_INTERFACE_DEFINED__ */


#ifndef __IInputMethod_INTERFACE_DEFINED__
#define __IInputMethod_INTERFACE_DEFINED__

/* interface IInputMethod */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID_IInputMethod;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("42429666-ae04-11d0-a4f8-00aa00a749b9")
    IInputMethod : public IUnknown
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Select( 
            /* [in] */ HWND hwndSip) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Deselect( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Showing( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Hiding( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetInfo( 
            /* [out] */ IMINFO __RPC_FAR *pimi) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ReceiveSipInfo( 
            /* [in] */ SIPINFO __RPC_FAR *psi) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RegisterCallback( 
            /* [in] */ IIMCallback __RPC_FAR *lpIMCallback) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetImData( 
            /* [in] */ DWORD dwSize,
            /* [out] */ LPVOID pvImData) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetImData( 
            /* [in] */ DWORD dwSize,
            /* [in] */ LPVOID pvImData) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UserOptionsDlg( 
            /* [in] */ HWND hwndParent) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IInputMethodVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IInputMethod __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IInputMethod __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IInputMethod __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Select )( 
            IInputMethod __RPC_FAR * This,
            /* [in] */ HWND hwndSip);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Deselect )( 
            IInputMethod __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Showing )( 
            IInputMethod __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Hiding )( 
            IInputMethod __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInfo )( 
            IInputMethod __RPC_FAR * This,
            /* [out] */ IMINFO __RPC_FAR *pimi);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReceiveSipInfo )( 
            IInputMethod __RPC_FAR * This,
            /* [in] */ SIPINFO __RPC_FAR *psi);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterCallback )( 
            IInputMethod __RPC_FAR * This,
            /* [in] */ IIMCallback __RPC_FAR *lpIMCallback);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetImData )( 
            IInputMethod __RPC_FAR * This,
            /* [in] */ DWORD dwSize,
            /* [out] */ LPVOID pvImData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetImData )( 
            IInputMethod __RPC_FAR * This,
            /* [in] */ DWORD dwSize,
            /* [in] */ LPVOID pvImData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UserOptionsDlg )( 
            IInputMethod __RPC_FAR * This,
            /* [in] */ HWND hwndParent);
        
        END_INTERFACE
    } IInputMethodVtbl;

    interface IInputMethod
    {
        CONST_VTBL struct IInputMethodVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInputMethod_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IInputMethod_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IInputMethod_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IInputMethod_Select(This,hwndSip)	\
    (This)->lpVtbl -> Select(This,hwndSip)

#define IInputMethod_Deselect(This)	\
    (This)->lpVtbl -> Deselect(This)

#define IInputMethod_Showing(This)	\
    (This)->lpVtbl -> Showing(This)

#define IInputMethod_Hiding(This)	\
    (This)->lpVtbl -> Hiding(This)

#define IInputMethod_GetInfo(This,pimi)	\
    (This)->lpVtbl -> GetInfo(This,pimi)

#define IInputMethod_ReceiveSipInfo(This,psi)	\
    (This)->lpVtbl -> ReceiveSipInfo(This,psi)

#define IInputMethod_RegisterCallback(This,lpIMCallback)	\
    (This)->lpVtbl -> RegisterCallback(This,lpIMCallback)

#define IInputMethod_GetImData(This,dwSize,pvImData)	\
    (This)->lpVtbl -> GetImData(This,dwSize,pvImData)

#define IInputMethod_SetImData(This,dwSize,pvImData)	\
    (This)->lpVtbl -> SetImData(This,dwSize,pvImData)

#define IInputMethod_UserOptionsDlg(This,hwndParent)	\
    (This)->lpVtbl -> UserOptionsDlg(This,hwndParent)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IInputMethod_Select_Proxy( 
    IInputMethod __RPC_FAR * This,
    /* [in] */ HWND hwndSip);


void __RPC_STUB IInputMethod_Select_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IInputMethod_Deselect_Proxy( 
    IInputMethod __RPC_FAR * This);


void __RPC_STUB IInputMethod_Deselect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IInputMethod_Showing_Proxy( 
    IInputMethod __RPC_FAR * This);


void __RPC_STUB IInputMethod_Showing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IInputMethod_Hiding_Proxy( 
    IInputMethod __RPC_FAR * This);


void __RPC_STUB IInputMethod_Hiding_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IInputMethod_GetInfo_Proxy( 
    IInputMethod __RPC_FAR * This,
    /* [out] */ IMINFO __RPC_FAR *pimi);


void __RPC_STUB IInputMethod_GetInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IInputMethod_ReceiveSipInfo_Proxy( 
    IInputMethod __RPC_FAR * This,
    /* [in] */ SIPINFO __RPC_FAR *psi);


void __RPC_STUB IInputMethod_ReceiveSipInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IInputMethod_RegisterCallback_Proxy( 
    IInputMethod __RPC_FAR * This,
    /* [in] */ IIMCallback __RPC_FAR *lpIMCallback);


void __RPC_STUB IInputMethod_RegisterCallback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IInputMethod_GetImData_Proxy( 
    IInputMethod __RPC_FAR * This,
    /* [in] */ DWORD dwSize,
    /* [out] */ LPVOID pvImData);


void __RPC_STUB IInputMethod_GetImData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IInputMethod_SetImData_Proxy( 
    IInputMethod __RPC_FAR * This,
    /* [in] */ DWORD dwSize,
    /* [in] */ LPVOID pvImData);


void __RPC_STUB IInputMethod_SetImData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IInputMethod_UserOptionsDlg_Proxy( 
    IInputMethod __RPC_FAR * This,
    /* [in] */ HWND hwndParent);


void __RPC_STUB IInputMethod_UserOptionsDlg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IInputMethod_INTERFACE_DEFINED__ */


#ifndef __IInputMethodEx_INTERFACE_DEFINED__
#define __IInputMethodEx_INTERFACE_DEFINED__

/* interface IInputMethodEx */
/* [object][helpstring][uuid] */ 


EXTERN_C const IID IID_IInputMethodEx;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9ddb3921-3606-11d2-a2eb-0000f8757270")
    IInputMethodEx : public IInputMethod
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Select( 
            /* [in] */ HWND hwndSip) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Deselect( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Showing( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Hiding( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetInfo( 
            /* [out] */ long __RPC_FAR *pimi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReceiveSipInfo( 
            /* [in] */ long __RPC_FAR *psi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RegisterCallback( 
            /* [in] */ IIMCallback __RPC_FAR *lpIMCallback) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetImData( 
            /* [in] */ DWORD dwSize,
            /* [out] */ long __RPC_FAR *pvImData) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetImData( 
            /* [in] */ DWORD dwSize,
            /* [in] */ long __RPC_FAR *pvImData) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UserOptionsDlg( 
            /* [in] */ HWND hwndParent) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetIMMActiveContext( 
            /* [in] */ HWND hwnd,
            /* [in] */ BOOL bOpen,
            /* [in] */ DWORD dwConversion,
            /* [in] */ DWORD dwSentence,
            /* [in] */ DWORD hkl) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RegisterCallbackEx( 
            /* [in] */ IIMCallbackEx __RPC_FAR *lpIMCallback) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IInputMethodExVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IInputMethodEx __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IInputMethodEx __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Select )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ HWND hwndSip);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Deselect )( 
            IInputMethodEx __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Showing )( 
            IInputMethodEx __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Hiding )( 
            IInputMethodEx __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInfo )( 
            IInputMethodEx __RPC_FAR * This,
            /* [out] */ IMINFO __RPC_FAR *pimi);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReceiveSipInfo )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ SIPINFO __RPC_FAR *psi);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterCallback )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ IIMCallback __RPC_FAR *lpIMCallback);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetImData )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ DWORD dwSize,
            /* [out] */ LPVOID pvImData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetImData )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ DWORD dwSize,
            /* [in] */ LPVOID pvImData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UserOptionsDlg )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ HWND hwndParent);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Select )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ HWND hwndSip);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Deselect )( 
            IInputMethodEx __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Showing )( 
            IInputMethodEx __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Hiding )( 
            IInputMethodEx __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInfo )( 
            IInputMethodEx __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pimi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReceiveSipInfo )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ long __RPC_FAR *psi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterCallback )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ IIMCallback __RPC_FAR *lpIMCallback);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetImData )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ DWORD dwSize,
            /* [out] */ long __RPC_FAR *pvImData);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetImData )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ DWORD dwSize,
            /* [in] */ long __RPC_FAR *pvImData);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UserOptionsDlg )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ HWND hwndParent);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetIMMActiveContext )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ HWND hwnd,
            /* [in] */ BOOL bOpen,
            /* [in] */ DWORD dwConversion,
            /* [in] */ DWORD dwSentence,
            /* [in] */ DWORD hkl);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterCallbackEx )( 
            IInputMethodEx __RPC_FAR * This,
            /* [in] */ IIMCallbackEx __RPC_FAR *lpIMCallback);
        
        END_INTERFACE
    } IInputMethodExVtbl;

    interface IInputMethodEx
    {
        CONST_VTBL struct IInputMethodExVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInputMethodEx_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IInputMethodEx_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IInputMethodEx_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IInputMethodEx_Select(This,hwndSip)	\
    (This)->lpVtbl -> Select(This,hwndSip)

#define IInputMethodEx_Deselect(This)	\
    (This)->lpVtbl -> Deselect(This)

#define IInputMethodEx_Showing(This)	\
    (This)->lpVtbl -> Showing(This)

#define IInputMethodEx_Hiding(This)	\
    (This)->lpVtbl -> Hiding(This)

#define IInputMethodEx_GetInfo(This,pimi)	\
    (This)->lpVtbl -> GetInfo(This,pimi)

#define IInputMethodEx_ReceiveSipInfo(This,psi)	\
    (This)->lpVtbl -> ReceiveSipInfo(This,psi)

#define IInputMethodEx_RegisterCallback(This,lpIMCallback)	\
    (This)->lpVtbl -> RegisterCallback(This,lpIMCallback)

#define IInputMethodEx_GetImData(This,dwSize,pvImData)	\
    (This)->lpVtbl -> GetImData(This,dwSize,pvImData)

#define IInputMethodEx_SetImData(This,dwSize,pvImData)	\
    (This)->lpVtbl -> SetImData(This,dwSize,pvImData)

#define IInputMethodEx_UserOptionsDlg(This,hwndParent)	\
    (This)->lpVtbl -> UserOptionsDlg(This,hwndParent)


#define IInputMethodEx_Select(This,hwndSip)	\
    (This)->lpVtbl -> Select(This,hwndSip)

#define IInputMethodEx_Deselect(This)	\
    (This)->lpVtbl -> Deselect(This)

#define IInputMethodEx_Showing(This)	\
    (This)->lpVtbl -> Showing(This)

#define IInputMethodEx_Hiding(This)	\
    (This)->lpVtbl -> Hiding(This)

#define IInputMethodEx_GetInfo(This,pimi)	\
    (This)->lpVtbl -> GetInfo(This,pimi)

#define IInputMethodEx_ReceiveSipInfo(This,psi)	\
    (This)->lpVtbl -> ReceiveSipInfo(This,psi)

#define IInputMethodEx_RegisterCallback(This,lpIMCallback)	\
    (This)->lpVtbl -> RegisterCallback(This,lpIMCallback)

#define IInputMethodEx_GetImData(This,dwSize,pvImData)	\
    (This)->lpVtbl -> GetImData(This,dwSize,pvImData)

#define IInputMethodEx_SetImData(This,dwSize,pvImData)	\
    (This)->lpVtbl -> SetImData(This,dwSize,pvImData)

#define IInputMethodEx_UserOptionsDlg(This,hwndParent)	\
    (This)->lpVtbl -> UserOptionsDlg(This,hwndParent)

#define IInputMethodEx_SetIMMActiveContext(This,hwnd,bOpen,dwConversion,dwSentence,hkl)	\
    (This)->lpVtbl -> SetIMMActiveContext(This,hwnd,bOpen,dwConversion,dwSentence,hkl)

#define IInputMethodEx_RegisterCallbackEx(This,lpIMCallback)	\
    (This)->lpVtbl -> RegisterCallbackEx(This,lpIMCallback)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInputMethodEx_Select_Proxy( 
    IInputMethodEx __RPC_FAR * This,
    /* [in] */ HWND hwndSip);


void __RPC_STUB IInputMethodEx_Select_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInputMethodEx_Deselect_Proxy( 
    IInputMethodEx __RPC_FAR * This);


void __RPC_STUB IInputMethodEx_Deselect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInputMethodEx_Showing_Proxy( 
    IInputMethodEx __RPC_FAR * This);


void __RPC_STUB IInputMethodEx_Showing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInputMethodEx_Hiding_Proxy( 
    IInputMethodEx __RPC_FAR * This);


void __RPC_STUB IInputMethodEx_Hiding_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInputMethodEx_GetInfo_Proxy( 
    IInputMethodEx __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pimi);


void __RPC_STUB IInputMethodEx_GetInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInputMethodEx_ReceiveSipInfo_Proxy( 
    IInputMethodEx __RPC_FAR * This,
    /* [in] */ long __RPC_FAR *psi);


void __RPC_STUB IInputMethodEx_ReceiveSipInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInputMethodEx_RegisterCallback_Proxy( 
    IInputMethodEx __RPC_FAR * This,
    /* [in] */ IIMCallback __RPC_FAR *lpIMCallback);


void __RPC_STUB IInputMethodEx_RegisterCallback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInputMethodEx_GetImData_Proxy( 
    IInputMethodEx __RPC_FAR * This,
    /* [in] */ DWORD dwSize,
    /* [out] */ long __RPC_FAR *pvImData);


void __RPC_STUB IInputMethodEx_GetImData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInputMethodEx_SetImData_Proxy( 
    IInputMethodEx __RPC_FAR * This,
    /* [in] */ DWORD dwSize,
    /* [in] */ long __RPC_FAR *pvImData);


void __RPC_STUB IInputMethodEx_SetImData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInputMethodEx_UserOptionsDlg_Proxy( 
    IInputMethodEx __RPC_FAR * This,
    /* [in] */ HWND hwndParent);


void __RPC_STUB IInputMethodEx_UserOptionsDlg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInputMethodEx_SetIMMActiveContext_Proxy( 
    IInputMethodEx __RPC_FAR * This,
    /* [in] */ HWND hwnd,
    /* [in] */ BOOL bOpen,
    /* [in] */ DWORD dwConversion,
    /* [in] */ DWORD dwSentence,
    /* [in] */ DWORD hkl);


void __RPC_STUB IInputMethodEx_SetIMMActiveContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInputMethodEx_RegisterCallbackEx_Proxy( 
    IInputMethodEx __RPC_FAR * This,
    /* [in] */ IIMCallbackEx __RPC_FAR *lpIMCallback);


void __RPC_STUB IInputMethodEx_RegisterCallbackEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IInputMethodEx_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CMSMVIm;

#ifdef __cplusplus

class DECLSPEC_UUID("e80d7526-58df-4142-a958-72490a691b46")
CMSMVIm;
#endif
#endif /* __MVSIPLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
