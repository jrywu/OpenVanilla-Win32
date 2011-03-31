#if !defined(AFX_STDAFX_H__D3533DAB_D4AA_4814_B702_A06F8E6B43CC__INCLUDED_)
#define AFX_STDAFX_H__D3533DAB_D4AA_4814_B702_A06F8E6B43CC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef STRICT
#define STRICT
#endif

#if _WIN32_WCE == 201
#error ATL is not supported for Palm-Size PC
#endif

#define WINVER 0x0500
//#define _WIN32_WINNT 0x0400
#define _ATL_FREE_THREADED
#if defined(_WIN32_WCE)    
#undef _WIN32_WINNT        
#endif



#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>


//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D3533DAB_D4AA_4814_B702_A06F8E6B43CC__INCLUDED)
