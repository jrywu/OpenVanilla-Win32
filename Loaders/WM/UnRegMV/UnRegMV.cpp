// UnRegMV.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>

#define IMClsidKey			TEXT("CLSID\\{E80D7526-58DF-4142-A958-72490A691B46}")
#define IMClsid				TEXT("{E80D7526-58DF-4142-A958-72490A691B46}")
#define MVHotKey			TEXT("00000110")

typedef HRESULT (*typeRegMVSIP)();
typedef HRESULT (*typeUnregMVSIP)();

int _tmain(int argc, _TCHAR* argv[])
{
	
	HINSTANCE hInst = LoadLibrary(TEXT("MVSIP.DLL"));
	typeRegMVSIP regMVSIP = (typeRegMVSIP)GetProcAddressW( hInst, TEXT("DllRegisterServer"));
	typeUnregMVSIP unregMVSIP = (typeUnregMVSIP)GetProcAddressW( hInst, TEXT("DllUnregisterServer"));


	if(argc && !_wcsicmp(argv[0],TEXT("/")))
	{//Register server
		if(regMVSIP()== S_OK)
			MessageBox(NULL,TEXT("Register MobileVanilla successed."), TEXT("RegMV"), MB_OK);
		else
			MessageBox(NULL,TEXT("Register MobileVanilla failed."), TEXT("RegMV"), MB_OK);

	}
	else
	{	//Unregister server
/*		
	HRESULT hr=S_OK;

	wchar_t wbuf[256];


	swprintf(wbuf, TEXT("%s\\InprocServer32"), IMClsidKey);
	if (ERROR_SUCCESS != RegDeleteKey(HKEY_CLASSES_ROOT, wbuf))
           hr = E_FAIL;

	swprintf(wbuf, TEXT("%s\\DefaultIcon"), IMClsidKey);
	if (ERROR_SUCCESS != RegDeleteKey(HKEY_CLASSES_ROOT, wbuf))
           hr = E_FAIL;

	swprintf(wbuf, TEXT("%s\\IsSIPInputMethod"), IMClsidKey);
	if (ERROR_SUCCESS != RegDeleteKey(HKEY_CLASSES_ROOT, wbuf))
           hr = E_FAIL;

	if (ERROR_SUCCESS != RegDeleteKey(HKEY_CLASSES_ROOT, IMClsidKey))
           hr = E_FAIL;
	

	swprintf(wbuf, TEXT("Control Panel\\Input Method\\Hot Keys\\%s"), MVHotKey);
	if (ERROR_SUCCESS != RegDeleteKey(HKEY_CURRENT_USER, wbuf))
           hr = E_FAIL;

*/
		if(unregMVSIP() ==  S_OK)
//	if(hr == S_OK)
			MessageBox(NULL,TEXT("Unregister MobileVanilla successed."), TEXT("UnRegMV"), MB_OK);
		else
			MessageBox(NULL,TEXT("Unregister MobileVanilla failed."), TEXT("UnRegMV"), MB_OK);

	}
	
		


	
	return 0;
}
