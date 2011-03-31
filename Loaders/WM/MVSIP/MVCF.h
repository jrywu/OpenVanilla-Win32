#ifndef __MVCF_H__
#define __MVCF_H__



class CClassFactory : public IClassFactory
{
public:

    //
    // Ctor, Dtor
    //

    CClassFactory(REFCLSID rclsid);
    virtual ~CClassFactory();


    //
    // IUnknown methods
    //

    STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
    STDMETHOD_(ULONG,AddRef) (VOID);
    STDMETHOD_(ULONG,Release) (VOID);


    //
    // IClassFactory methods
    //

    STDMETHOD(CreateInstance) (IUnknown FAR* pUnkOuter, REFIID riid,
                               LPVOID FAR *ppunkObject);
    STDMETHOD(LockServer) (BOOL fLock);


protected:

    ULONG m_cRef;
    CLSID m_clsid;

};



#endif // __MVCF_H__
