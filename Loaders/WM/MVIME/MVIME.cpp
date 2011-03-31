// MVIME.cpp
//
//#define OV_DEBUG

#include "MVIME.h"


typedef void (CALLBACK *TypeImeGetUIClassname)(LPTSTR lpszImeUIClassName);

HINSTANCE hUIInst;
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  dwReason, 
                       LPVOID lpReserved
					 )
{	
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
		
	//	if(!(hUIInst = GetModuleHandleW(_T("Msimeuic.dll"))))
		hUIInst = LoadLibrary(_T("Msimeuic.dll"));
		
		TypeImeGetUIClassname ImeGetUIClassName;
		ImeGetUIClassName= (TypeImeGetUIClassname) GetProcAddress( hUIInst,_T("ImeGetUIClassName") );

		if(ImeGetUIClassName) {
			ImeGetUIClassName(UICLASSNAME);
		char buf[32];
		wcstombs(buf, UICLASSNAME,32);
		murmur("UIClassName:%s\n", buf);
		}
		else
			murmur("Bind UI dll failed\n");

		hInst = (HINSTANCE)hModule;
		// load UI library
		AVLoader* loader = AVLoader::open();
		loader->connectDisplayServer(dsvr);

		HIMC hImc;

		hImc = (HIMC)GetWindowLong(FindWindow(UICLASSNAME,NULL), IMMGWL_IMC);
		dsvr->setHIMC(hImc);
		murmur("hImc:%x",hImc);

		/*
		//<comment author='b6s'>Test for a replacement of UI's DllMain()
		//UIConstruct();
		//</comment>

		WNDCLASS wc;
		//wc.cbSize			= sizeof(WNDCLASS);
		wc.style			= CS_OVIME | CS_IME;
		wc.lpfnWndProc		= UIWndProc;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 2 * sizeof(LONG);
		wc.hInstance		= (HINSTANCE)hModule;
		wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
		wc.hIcon			= NULL;
		wc.lpszMenuName		= (LPTSTR)NULL;
		wc.lpszClassName	= UICLASSNAME;
		wc.hbrBackground	= NULL;
		//wc.hIconSm			= NULL;

		//IMEUIRegisterClass( (HINSTANCE)hModule );
		*/
		

		//if( !RegisterClass( (LPWNDCLASS)&wc ) )
		//  return FALSE;

		break;
		}
	case DLL_PROCESS_DETACH:
		// free UI library
		AVLoader::close();
		UnregisterClass(UICLASSNAME, (HINSTANCE)hModule);
		//IMEUIUnRegisterClass( (HINSTANCE)hModule );
		//<comment author='b6s'>Test for a replacement of UI's DllMain()
		//UIDispose();
		//</comment>
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
    return TRUE;
}
