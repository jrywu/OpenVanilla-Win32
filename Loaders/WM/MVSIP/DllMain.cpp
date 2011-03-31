#include "stdafx.h"
#include "initguid.h"
#include <commctrl.h>
#include <aygshell.h>
#include <sipapi.h>
#include "dllmain.h"
#include "MVCF.h"
#include "MVsip_i.c"



// constant strings that we use for registering the server
#define IMClsidKey			TEXT("CLSID\\{E80D7526-58DF-4142-A958-72490A691B46}")
#define IMClsid				TEXT("{E80D7526-58DF-4142-A958-72490A691B46}")
#define SIPNAME				TEXT("MobileVannilla")
#define MVHotKey			TEXT("00000110")
#define IME_KBDLAYOUT		0xE0110404
#define szIME_KBDLAYOUT		TEXT("e0110404")
#define IME_SW_HOTKEY		0x00000110
#define IME_SW_HOTKEY_KM	0x00c002
#define IME_SW_HOTKEY_VK	0x7C


DWORD		g_dwObjectCount;
HINSTANCE	g_hInstDll;

CComModule	_Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C" BOOL WINAPI DllMain(HANDLE hInstance, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hInstDll = (HINSTANCE)hInstance;
		_Module.Init(ObjectMap, (HINSTANCE)hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		_Module.Term();
	}
	return TRUE;    
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	if( !g_dwObjectCount )
	{
        return NOERROR;
    }

    return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	HRESULT hr;
    
    CClassFactory *pClassFactory = new CClassFactory( rclsid );
    
    if( pClassFactory ) 
	{
        hr = pClassFactory->QueryInterface( riid, ppv );
        RELEASE( pClassFactory );

        return hr;
    }

    return E_OUTOFMEMORY;
}

//
// add all the registry entries needed to make this CLSID into a SIP
//
HRESULT RegisterAClsid(LPTSTR pszClsid)
{
	LONG		lRC;
	HKEY		hClsidKey;
	HKEY		hGuidKey;
	HKEY		hRegKey;
	HKEY		hKey;

	DWORD		dwDisposition;

 	lRC = RegOpenKey(HKEY_CLASSES_ROOT, TEXT("CLSID"), &hClsidKey);
	if (ERROR_SUCCESS != lRC)
	{
		OutputDebugString(TEXT("Failed to create the CLSID key"));
		return E_FAIL;
	}

	// create the base key for the OLE object under clsid
	lRC = RegCreateKeyEx(hClsidKey, pszClsid,
		                 0, TEXT("REG_SZ"), 0, KEY_WRITE, NULL, &hGuidKey, &dwDisposition);
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hClsidKey);
		return E_FAIL;
	}

	// now set the named value under this key
	lRC = RegSetValueEx(hGuidKey, NULL, 0, REG_SZ, (BYTE*) SIPNAME, 
		                sizeof(TCHAR)*(1+_tcslen(SIPNAME)));
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hGuidKey);
		RegCloseKey(hClsidKey);
		return E_FAIL;
	}
	// set the KBMode key
	/*
	DWORD dwKBmode=0;
	 lRC = RegSetValueEx(hGuidKey, TEXT("KBMode"), 0, REG_DWORD,(const BYTE *) &dwKBmode,   sizeof(DWORD));
	
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hGuidKey);
		RegCloseKey(hClsidKey);
		return E_FAIL;
	}
	*/
	// now, create the standard OLE server key
	lRC = RegCreateKeyEx(hGuidKey, TEXT("InprocServer32"),
		                 0, TEXT("REG_SZ"), 0, KEY_WRITE, NULL, &hRegKey, &dwDisposition);
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hGuidKey);
		RegCloseKey(hClsidKey);
		return E_FAIL;
	}
	wchar_t szDllPath[MAX_PATH];
	GetModuleFileName(GetModuleHandle(L"MVSIP.DLL"), szDllPath, MAX_PATH);
	// now set the named value under this key
	lRC = RegSetValueEx(hRegKey, NULL, 0, REG_SZ,  (BYTE*) szDllPath, //EXEPATH, 
		                sizeof(TCHAR)*(1+_tcslen(szDllPath)));
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hRegKey);
		RegCloseKey(hGuidKey);
		RegCloseKey(hClsidKey);
		return E_FAIL;
	}
	RegCloseKey(hRegKey);

	// this is the *only* thing which marks this as a sip
	lRC = RegCreateKeyEx(hGuidKey, TEXT("IsSIPInputMethod"),
		                 0, TEXT("REG_SZ"), 0, KEY_WRITE, NULL, &hRegKey, &dwDisposition);
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hGuidKey);
		RegCloseKey(hClsidKey);
		return E_FAIL;
	}

	// now set the named value under this key
	lRC = RegSetValueEx(hRegKey, NULL, 0, REG_SZ,  (BYTE*) L"1", 
		                sizeof(TCHAR)*(1+_tcslen(L"1")));
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hRegKey);
		RegCloseKey(hGuidKey);
		RegCloseKey(hClsidKey);
		return E_FAIL;
	}
	RegCloseKey(hRegKey);

	// this controls the icon we see in the sip menu
	lRC = RegCreateKeyEx(hGuidKey, TEXT("DefaultIcon"),
		                 0, TEXT("REG_SZ"), 0, KEY_WRITE, NULL, &hRegKey, &dwDisposition);
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hGuidKey);
		RegCloseKey(hClsidKey);
		return E_FAIL;
	}
	
	// now set the named value under this key
	lRC = RegSetValueEx(hRegKey, NULL, 0, REG_SZ,  (BYTE*) TEXT("1"), 
		                sizeof(TCHAR)*(1+_tcslen(TEXT("1"))));
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hRegKey);
		RegCloseKey(hGuidKey);
		RegCloseKey(hClsidKey);
		return E_FAIL;
	}
	RegCloseKey(hRegKey);
	RegCloseKey(hGuidKey);
	RegCloseKey(hClsidKey);
	//-------------------------------------------------------------------------------------------
	// The following key settings are for OVIME.dll
	// Open the hotkey at HK_C_USER\Control Panel\Input Method\HotKeys
	lRC = RegOpenKey(HKEY_CURRENT_USER, TEXT("Control Panel\\Input Method\\Hot Keys"), &hKey);
	if (ERROR_SUCCESS != lRC)
	{
		OutputDebugString(TEXT("Failed to open hotkey key"));
		return E_FAIL;
	}
	
	//
	// create the base key for the new MV IM 
	lRC = RegCreateKeyEx(hKey, MVHotKey,
		                 0, TEXT("REG_SZ"), 0, KEY_WRITE, NULL, &hRegKey, &dwDisposition);
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hRegKey);
		RegCloseKey(hKey);
		return E_FAIL;
	}
	//
	// Now set IM/KM/VK values
	DWORD dwValue = IME_KBDLAYOUT;
	lRC = RegSetValueEx(hRegKey, TEXT("Ime"), 0, REG_DWORD,(const BYTE *) &dwValue,  
		 sizeof(DWORD));
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hRegKey);
		RegCloseKey(hKey);
		return E_FAIL;
	}
	// Key modifier (KM)
	dwValue = IME_SW_HOTKEY_KM;// Control
	lRC = RegSetValueEx(hRegKey, TEXT("KM"), 0, REG_DWORD,(const BYTE *) &dwValue,  
		 sizeof(DWORD));
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hRegKey);
		RegCloseKey(hKey);
		return E_FAIL;
	}
	
	// Virtual key code (VK)
	dwValue = IME_SW_HOTKEY_VK;// F9
	lRC = RegSetValueEx(hRegKey, TEXT("VK"), 0, REG_DWORD,(const BYTE *) &dwValue,  
		 sizeof(DWORD));
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hRegKey);
		RegCloseKey(hKey);
		return E_FAIL;
	}
	RegCloseKey(hRegKey);
	RegCloseKey(hKey);
	//
	// Open the Im preload key
	//
	bool preloadkeycreated=false;
	lRC = RegOpenKey(HKEY_CURRENT_USER, TEXT("keyboard layout\\Preload"), &hKey);
	if (ERROR_SUCCESS != lRC)
	{
		OutputDebugString(TEXT("Failed to open preload key"));
		return E_FAIL;
	}

	DWORD dwSubKeys, dwBufsize, dwType = REG_SZ, dwSize=32;
	TCHAR keyname[16],  szPreloadkeypath[64], szValue[32];
	// Query the number of subkeys
	lRC=RegQueryInfoKey(hKey, NULL,NULL,NULL, &dwSubKeys, NULL, NULL,NULL,NULL,NULL,NULL,NULL);
	if (ERROR_SUCCESS != lRC)
	{
		OutputDebugString(TEXT("Failed to open preload key"));
		return E_FAIL;
	}
	//
	// enumkey to delete previuos set preload key
	for(DWORD i=0; i<dwSubKeys; i++)
	{
		dwBufsize=16;
		if(RegEnumKeyExW(hKey, i, keyname, &dwBufsize, NULL,NULL,NULL,NULL)== ERROR_SUCCESS)
		{
			wcscpy(szPreloadkeypath,TEXT("keyboard layout\\Preload\\"));
			wcscat(szPreloadkeypath, keyname);
			if(RegQueryValueExW(HKEY_CURRENT_USER, TEXT("Default"),
                        (LPDWORD) szPreloadkeypath,
                        &dwType,
                        (LPBYTE)szValue,
                        &dwSize) == ERROR_SUCCESS)
			{
				if(!wcscmp(szValue,szIME_KBDLAYOUT))
						preloadkeycreated=true;//RegDeleteKey(HKEY_CURRENT_USER, szPreloadkeypath);
			}
		}
	}
	if(!preloadkeycreated)
	{
		/*
		// Query the subkeys again
		lRC=RegQueryInfoKey(hKey, NULL,NULL,NULL, &dwSubKeys, NULL, NULL,NULL,NULL,NULL,NULL,NULL);
		if (ERROR_SUCCESS != lRC)
		{
			OutputDebugString(TEXT("Failed to open preload key"));
			return E_FAIL;
		}
		*/
		// Set the key name as subkeys +1
		swprintf(keyname,TEXT("%i"), dwSubKeys+1);
		// create the base key for the new MV IM 
		lRC = RegCreateKeyEx(hKey, keyname,
				0, TEXT("REG_SZ"), 0, KEY_WRITE, NULL, &hRegKey, &dwDisposition);
		if (ERROR_SUCCESS != lRC)
		{
			RegCloseKey(hRegKey);
			RegCloseKey(hKey);
			return E_FAIL;
		}
		// Now set default value to be keyboard layout name
		lRC = RegSetValueEx(hRegKey, TEXT("Default"), 0, REG_SZ,(const BYTE *) szIME_KBDLAYOUT,  
			 sizeof(TCHAR)*(1+_tcslen(szIME_KBDLAYOUT)));
		if (ERROR_SUCCESS != lRC)
		{
			RegCloseKey(hRegKey);
			RegCloseKey(hKey);
			return E_FAIL;
		}
		RegCloseKey(hRegKey);
	}

	RegCloseKey(hKey);
	
	//
	// Open the keyboard layout key.
	lRC = RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("System\\CurrentControlSet\\Control\\Layouts"), &hKey);
	if (ERROR_SUCCESS != lRC)
	{
		OutputDebugString(TEXT("Failed to open layout key"));
		return E_FAIL;
	}
	//
	// Create MV layout
	lRC = RegCreateKeyEx(hKey, szIME_KBDLAYOUT,
		                 0, TEXT("REG_SZ"), 0, KEY_WRITE, NULL, &hRegKey, &dwDisposition);
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hRegKey);
		RegCloseKey(hKey);
		return E_FAIL;
	}
	//
	// Now set MV OVIME.dll filepath
	WCHAR* lb= wcsrchr(szDllPath,'\\');
	szDllPath[((DWORD)lb-(DWORD)szDllPath)/sizeof(TCHAR) +1]=0; //stripped the filename
	wcscat(szDllPath, TEXT("OVIME.DLL"));
	//wcscpy(szDllPath, TEXT("OVIME.DLL"));
	lRC = RegSetValueEx(hRegKey, TEXT("Ime File"), 0, REG_SZ,(const BYTE *) szDllPath,  
		 sizeof(TCHAR)*(1+_tcslen(szDllPath)));
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hRegKey);
		RegCloseKey(hKey);
		return E_FAIL;
	}
	// Set layout to be English keyboard
	lRC = RegSetValueEx(hRegKey, TEXT("Keyboard Layout"), 0, REG_SZ,(const BYTE *) TEXT("00000409"),  
		 sizeof(TCHAR)*(1+_tcslen(TEXT("00000409"))));
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hRegKey);
		RegCloseKey(hKey);
		return E_FAIL;
	}
	// set the layout text
	lRC = RegSetValueEx(hRegKey, TEXT("Layout Text"), 0, REG_SZ,(const BYTE *) SIPNAME,  
		 sizeof(TCHAR)*(1+_tcslen(SIPNAME)));
	if (ERROR_SUCCESS != lRC)
	{
		RegCloseKey(hRegKey);
		RegCloseKey(hKey);
		return E_FAIL;
	}

	RegCloseKey(hRegKey);
	RegCloseKey(hKey);
	

	return S_OK;
}

//
// DllRegisterServer - Adds entries to the system registry
//
STDAPI DllRegisterServer(void)
{
	HRESULT	hr = S_OK;
	
	hr = RegisterAClsid(IMClsid);

	return hr;

}

//
// DllUnregisterServer - Removes entries from the system registry
//
STDAPI DllUnregisterServer(void)
{

	
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
	
	HKEY hKey;
	DWORD dwSubKeys, dwBufsize, dwType = REG_SZ, dwSize=32;
	TCHAR keyname[16],  szPreloadkeypath[64], szValue[32];

	LONG lRC = RegOpenKey(HKEY_CURRENT_USER, TEXT("keyboard layout\\Preload"), &hKey);
	if (ERROR_SUCCESS != lRC)
	{
		OutputDebugString(TEXT("Failed to open hotkey key"));
		return E_FAIL;
	}

	lRC=RegQueryInfoKey(hKey, NULL,NULL,NULL, &dwSubKeys, NULL, NULL,NULL,NULL,NULL,NULL,NULL);
	if (ERROR_SUCCESS != lRC)
	{
		OutputDebugString(TEXT("Failed to open hotkey key"));
		return E_FAIL;
	}
	
	
	for(DWORD i=0; i<dwSubKeys; i++)
	{
		dwBufsize=16;
		if(RegEnumKeyExW(hKey, i, keyname, &dwBufsize, NULL,NULL,NULL,NULL)== ERROR_SUCCESS)
		{
			wcscpy(szPreloadkeypath,TEXT("keyboard layout\\Preload\\"));
			wcscat(szPreloadkeypath, keyname);
			if(RegQueryValueExW(HKEY_CURRENT_USER, TEXT("Default"),
                        (LPDWORD) szPreloadkeypath,
                        &dwType,
                        (LPBYTE)szValue,
                        &dwSize) == ERROR_SUCCESS)
			{
				if(!wcscmp(szValue,szIME_KBDLAYOUT))
						RegDeleteKey(HKEY_CURRENT_USER, szPreloadkeypath);
			}
		}
	}
	RegCloseKey(hKey);


	swprintf(wbuf, TEXT("Control Panel\\Input Method\\Hot Keys\\%s"), MVHotKey);
	if (ERROR_SUCCESS != RegDeleteKey(HKEY_CURRENT_USER, wbuf))
           hr = E_FAIL;



	swprintf(wbuf, TEXT("System\\CurrentControlSet\\Control\\Layouts\\%s"), szIME_KBDLAYOUT);
	if (ERROR_SUCCESS != RegDeleteKey(HKEY_LOCAL_MACHINE, wbuf))
           hr = E_FAIL;
		   
	return hr; 
  
}


