// RegMV.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>


typedef HRESULT (*typeRegMVSIP)();
typedef HRESULT (*typeUnregMVSIP)();

int _tmain(int argc, _TCHAR* argv[])
{
	HINSTANCE hInst = LoadLibrary(TEXT("MVSIP.DLL"));
	typeRegMVSIP regMVSIP = (typeRegMVSIP)GetProcAddressW( hInst, TEXT("DllRegisterServer"));
	typeUnregMVSIP unregMVSIP = (typeUnregMVSIP)GetProcAddressW( hInst, TEXT("DllUnregisterServer"));


	if(argc && !_wcsicmp(argv[0],TEXT("/u")))
	{	//Unregister server
		if(unregMVSIP() ==  S_OK)
			MessageBox(NULL,TEXT("Unregister MobileVanilla successed."), TEXT("RegMV"), MB_OK);
		else
			MessageBox(NULL,TEXT("Unregister MobileVanilla failed."), TEXT("RegMV"), MB_OK);

	}
	else
		//Register server
	{
		if(regMVSIP()== S_OK)
			MessageBox(NULL,TEXT("Register MobileVanilla successed."), TEXT("RegMV"), MB_OK);
		else
			MessageBox(NULL,TEXT("Register MobileVanilla failed."), TEXT("RegMV"), MB_OK);

	}
	
	return 0;
}

