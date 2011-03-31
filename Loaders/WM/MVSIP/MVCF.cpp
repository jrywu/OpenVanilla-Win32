#include "stdafx.h"
#include "dllmain.h"
#include "MVCF.h"
#include "MVSIP.h"



CClassFactory::CClassFactory( REFCLSID rclsid )
{
    g_dwObjectCount++ ;

    m_cRef = 1;
    m_clsid = rclsid;
    
}

CClassFactory::~CClassFactory()
{
    g_dwObjectCount-- ;
}


//
// IUnknown methods.
//

STDMETHODIMP CClassFactory::QueryInterface( REFIID riid, LPVOID FAR* ppobj )
{
    *ppobj = NULL;
    
    if( IID_IUnknown == riid || IID_IClassFactory == riid ) 
	{
        *ppobj = (LPVOID)this;
        AddRef();
        return NOERROR;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CClassFactory::AddRef(VOID)
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CClassFactory::Release(VOID)
{
    if( --m_cRef ) 
	{
        return m_cRef;
    }
    delete this;
    return 0;
}


//
// IClassFactory methods.
//

extern "C" const CLSID CLSID_CMSMVIm;

STDMETHODIMP CClassFactory::CreateInstance(
    IUnknown FAR* pUnkOuter,
    REFIID riid,
    LPVOID FAR *ppunkObject )
{
    HRESULT hr = E_FAIL;


    if( ppunkObject ) 
	{
        *ppunkObject = NULL;
    }


    //
    // We don't aggregate
    //

    if( pUnkOuter ) 
	{
        return CLASS_E_NOAGGREGATION;
    }


    if( pUnkOuter && riid != IID_IUnknown ) 
	{
        return E_INVALIDARG;
    }


    //
    // Create the class object and retrieve the requested interface.
    //

    if( CLSID_CMSMVIm == m_clsid ) 
	{
        CInputMethod *pInputMethod = new CInputMethod( pUnkOuter, g_hInstDll );
        hr = pInputMethod->QueryInterface( riid, ppunkObject );
    }
              
    return hr;
}

STDMETHODIMP CClassFactory::LockServer( BOOL fLock )
{
    if( fLock ) 
	{
        g_dwObjectCount++;
    }
	else
	{
        g_dwObjectCount--;
    }

    return NOERROR;
}
