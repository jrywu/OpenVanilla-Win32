#ifndef __MACROS_H__
#define __MACROS_H__

/////////////////////////////////////////////////////////////////////////////

#ifdef UNDER_CE

#ifndef _T
#ifdef  _UNICODE
#define _T(x)      L ## x
#else
#define _T(x)      x
#endif
#endif

#define RegOpenKey(hkey, lpsz, phk) \
        RegOpenKeyEx((hkey), (lpsz), 0, 0, (phk))

#ifndef MulDiv
#define MulDiv(a,b,c)   (((a)*(b))/(c))
#endif
#endif //UNDER_CE

#ifdef __cplusplus

#define ADDREF(x)    if ((x) != NULL) { (x)->AddRef(); }
#define RELEASE(x)   if ((x) != NULL) { (x)->Release(); (x) = NULL; }

#define METHOD_PROLOGUE(theClass, localClass) \
    theClass* pThis = \
		((theClass *)((BYTE *)this - offsetof(theClass, m_x##localClass))); \

#define DEFINE_INTERFACE_PART(localClass) \
	friend localClass; \
	localClass m_x##localClass; \

#define BEGIN_INTERFACE_PART(localClass, baseClass) \
	class localClass : public baseClass \
	{ \
	public: \
		STDMETHOD_(ULONG, AddRef)(); \
		STDMETHOD_(ULONG, Release)(); \
		STDMETHOD(QueryInterface)(REFIID riid, LPVOID FAR* ppvObj); \

#define END_INTERFACE_PART(localClass) \
	};

#define INTERFACE_PART(localClass)  (&m_x##localClass)
#define CONTROLING_UNKNOWN  (pThis->m_pUnkOuter)


#else

#define ADDREF(x)    if ((x) != NULL) {(x)->lpVtbl->AddRef((x)); }
#define RELEASE(x)   if ((x) != NULL) {(x)->lpVtbl->Release((x)); (x) = NULL;}
#endif // __cplusplus

/////////////////////////////////////////////////////////////////////////////
	
#endif /* __MACROS_H__ */
