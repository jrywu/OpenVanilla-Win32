//#define OV_DEBUG
#include "OVIME.h"
#include "AVKeyCode.h"
#include "ExtraStructs.h"
#include <commctrl.h>

#include "ImmController.h"

#include <vector>
#include <string>
#include <iostream> 
#include <sstream>
#include <fstream>


using namespace std;

void
MyGenerateMessage(HIMC hIMC, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LPINPUTCONTEXT lpIMC;



	if((lpIMC = ImmLockIMC(hIMC)) == NULL)
		return;

    if (IsWindow(lpIMC->hWnd))
    {
        LPTRANSMSG lpTransMsg;
        if (!(lpIMC->hMsgBuf = ImmReSizeIMCC(lpIMC->hMsgBuf,
			(lpIMC->dwNumMsgBuf + 1) * sizeof(TRANSMSG))))
            return;
		
        if (!(lpTransMsg = (LPTRANSMSG)ImmLockIMCC(lpIMC->hMsgBuf)))
            return;
		
        lpTransMsg += (lpIMC->dwNumMsgBuf);
        //lpTransMsg->uMsg = msg;
		lpTransMsg->uMsg = msg;
		lpTransMsg->wParam=wParam;
		lpTransMsg->lParam=lParam;

        ImmUnlockIMCC(lpIMC->hMsgBuf);
        lpIMC->dwNumMsgBuf++;

        ImmGenerateMessage(hIMC);
    }
	ImmUnlockIMC(hIMC);
}

BOOL
MyGenerateMessageToTransKey(LPDWORD lpdwTransKey, UINT *uNumTranMsgs,
							UINT msg, WPARAM wParam, LPARAM lParam) 
{
	// This code is from FreePY. 
	// It seems that the first byte is the number of bytes.
	LPDWORD lpdwTemp;
	
	if (((*uNumTranMsgs) + 1) >= (UINT)*lpdwTransKey)
        return FALSE;
	
	lpdwTemp = (LPDWORD)lpdwTransKey+1+(*uNumTranMsgs)*3;
	*(lpdwTemp++) = msg;
	*(lpdwTemp++) = (DWORD)wParam;
	*(lpdwTemp++) = (DWORD)lParam;

	(*uNumTranMsgs)++;

    return TRUE;
}

BOOL APIENTRY 
ImeInquire(LPIMEINFO lpIMEInfo, LPTSTR lpszUIClass, LPCTSTR lpszOption)
{
    lpIMEInfo->dwPrivateDataSize = sizeof(MYPRIVATE);

    lpIMEInfo->fdwProperty = IME_PROP_AT_CARET|IME_PROP_KBD_CHAR_FIRST |IME_PROP_UNICODE;
                             //IME_PROP_SPECIAL_UI;
							 //IME_PROP_UNICODE

    lpIMEInfo->fdwConversionCaps = IME_CMODE_NOCONVERSION | IME_CMODE_FULLSHAPE |
								IME_CMODE_NATIVE;

    lpIMEInfo->fdwSentenceCaps = IME_SMODE_NONE;
    lpIMEInfo->fdwUICaps = UI_CAP_2700;

	lpIMEInfo->fdwSCSCaps = 0;

    lpIMEInfo->fdwSelectCaps = SELECT_CAP_CONVERSION;

    _tcscpy(lpszUIClass, UICLASSNAME);
	

    return TRUE;
}

BOOL APIENTRY 
ImeConfigure(HKL hKL,HWND hWnd, DWORD dwMode, LPVOID lpData)
{

	if( IME_CONFIG_GENERAL == dwMode )
	{
#ifdef WINCE
#else
		//TCHAR buf[MAX_PATH];
		//GetWindowsDirectory( buf, MAX_PATH );
		//_tcscat( buf, _T("\\OpenVanilla\\OVPreferences.exe")); 

		//@warning an OV-specific path here!
		// Is it better to use OVService to get it? 
		
		//AVLoader* loader=AVLoader::open();
		AVEmbeddedLoader *em = AVEmbeddedLoader::getInstance();
		
		murmur("ImeConfigure. Base dir from loader:%s", em->cfg()->getBaseDir()); //loader->getBaseDirectory());
		
		//LPCTSTR buf = _T("C:\\OpenVanilla\\OVPreferences.exe"); 
						
		TCHAR dir[MAX_PATH], file[MAX_PATH];
				
		mbstowcs(dir,em->cfg()->getBaseDir(),MAX_PATH);

		em->removeInstance();   // delete it.

		_tcscpy(file,dir);
		_tcscat(file,_T("OVPreferences.exe"));
				
		char buff[MAX_PATH];
		wcstombs(buff,file, MAX_PATH);
		murmur("OVpreference:%s",buff);

		//if(PathFileExists(buf)){ 
		if(!_waccess(file,0)){
			//ShellExecute( hWnd, _T("open"), buf, NULL, NULL, SW_SHOWNORMAL );
			SHELLEXECUTEINFO ShExecInfo = {0};
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = NULL;
			ShExecInfo.lpFile = file;		
			ShExecInfo.lpParameters = _T("");	
			ShExecInfo.lpDirectory = dir;
			ShExecInfo.nShow = SW_SHOWNORMAL;
			ShExecInfo.hInstApp = NULL;	
			ShellExecuteEx(&ShExecInfo);
			WaitForSingleObject(ShExecInfo.hProcess,INFINITE);

			murmur("send WM_IME_RELOADCONFIG to all windows. My Hwnd:%x", hWnd);
			EnumChildWindows( GetDesktopWindow(), ReloadIME, NULL);
		}
#endif
	}

	InvalidateRect(hWnd,NULL,FALSE);
	return TRUE;
}

DWORD APIENTRY 
ImeConversionList(HIMC hIMC,LPCTSTR lpSource,LPCANDIDATELIST lpCandList,DWORD dwBufLen,UINT uFlag)
{
    return 0;
}

BOOL APIENTRY 
ImeDestroy(UINT uForce)
{
    return FALSE;  
} 

LRESULT APIENTRY 
ImeEscape(HIMC hIMC,UINT uSubFunc,LPVOID lpData)
{

#define IME_ESC_GET_COMPSTR_LEN         0x1010
	murmur("ImeEscape: subfunc: %x\n", uSubFunc);

	LRESULT lRet;
	switch (uSubFunc)
	{
	case IME_ESC_QUERY_SUPPORT: //0x03
		murmur("ImeEscape: subfunc: %x, lpdata, %x\n", uSubFunc, *(LPUINT)lpData);
		if ( lpData == NULL )
			return FALSE;
		switch (*(LPUINT)lpData)
		{
		case IME_ESC_QUERY_SUPPORT:
		case IME_ESC_IME_NAME:
		case IME_ESC_GET_COMPSTR_LEN:
		case IME_ESC_MAX_KEY:
			return TRUE;
		default:
			return FALSE;
		}
		break;
	case IME_ESC_MAX_KEY: //0x1005
			return DWORD(4);
	case IME_ESC_GET_COMPSTR_LEN: //0x1010
	{
		
		CONST INPUTCONTEXT * lpIMC = (CONST INPUTCONTEXT *)ImmLockIMC(hIMC);
		if (!lpIMC) {
			return false;
		}
		LPCOMPOSITIONSTRING lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
		if (!lpCompStr) {
			ImmUnlockIMC(hIMC);
			return false;
		}
		DWORD dwCompStrLen = lpCompStr->dwCompStrLen;
		ImmUnlockIMCC(lpIMC->hCompStr);
		ImmUnlockIMC(hIMC);
		murmur("ImeEscape: IME_ESC_GET_COMPSTR_LEN: %u\n", dwCompStrLen);
		return dwCompStrLen?(LRESULT)dwCompStrLen:1;
		
		//return 1;
		}
	default:
		return FALSE;
	}
	return lRet;
}


BOOL APIENTRY 
ImeProcessKey(HIMC hIMC, UINT uVKey, LPARAM lKeyData, CONST LPBYTE lpbKeyState)
{
	static bool hIMCset=false;
	BOOL isProcessed = FALSE;
	murmur("ImeProcessKey: %u, hex%x, lKeyData:%x \n", uVKey, uVKey, lKeyData);
	if (!hIMC) return isProcessed;

	//if(!hIMCset) {
		dsvr->setHIMC(hIMC);
	//	hIMCset = true;
	//}

	/*DWORD conv, sentence;
	ImmGetConversionStatus(hIMC, &conv, &sentence);
	//Alphanumeric mode
	if(!(conv & IME_CMODE_NATIVE)) return isProcessed;*/

	ImmController* controller = ImmController::open();
	int controlState =
		controller->onControlEvent(hIMC, uVKey, lKeyData, lpbKeyState);
	switch(controlState) {
		case 0: // CTRL/ALT/SHIFT, passed
			isProcessed = FALSE;   // IME_CMODE_NATIVE: native english mode.
			break;
		case 1:	// CTRL/ALT/SHIFT, processed
			isProcessed = TRUE;
			break;
		case 2:	// normal
#ifdef WINCE
		if(0xEB==uVKey)    // HTC Raphel(touch pro) chn/eng toggle key
		{
			if(ImmGetOpenStatus(hIMC))
				ImmSetOpenStatus(NULL, FALSE);
			else
				ImmSetOpenStatus(NULL, TRUE);
			isProcessed = TRUE;
			break;
		}
		else if(0xEC==uVKey)   // HTC Raphel fn key. Do nothing here.	
		{
			isProcessed = TRUE;
			break;				
		}
		else if(VK_DBE_ALPHANUMERIC==uVKey)	
		{
			isProcessed = TRUE;
			break;				
		}
		else if( 0==ImmGetOpenStatus(hIMC) )	
			break;
		else
	
#endif
			isProcessed =
				controller->onTypingEvent(hIMC, uVKey, lKeyData, lpbKeyState);
			break;
	}
	
	//<comment author='b6s'>
	// NOT close ImmController because of counting for SHIFT pressed time.
	//ImmController::close();
	//</comment>

	return isProcessed;
}

BOOL APIENTRY 
ImeSelect(HIMC hIMC, BOOL fSelect)
{
    LPINPUTCONTEXT lpIMC;

    if (!hIMC)
        return TRUE;

	if (!fSelect)
		return TRUE; 

	DWORD conv, sentence;
	ImmGetConversionStatus( hIMC, &conv, &sentence);
	if( isChinese )
		conv |= IME_CMODE_NATIVE;
	else
		conv &= ~IME_CMODE_NATIVE;
	if( isFullShape )
		conv |= IME_CMODE_FULLSHAPE;
	else
		conv &= ~IME_CMODE_FULLSHAPE;
	murmur("IsChinese:%i; IsFullShape:%i\n", isChinese, isFullShape);
	ImmSetConversionStatus( hIMC, conv, sentence);

    if (lpIMC = ImmLockIMC(hIMC)) {		
		LPCOMPOSITIONSTRING lpCompStr;
		LPCANDIDATEINFO lpCandInfo;
		LPMYPRIVATE lpMyPrivate;

		lpIMC->fOpen = TRUE;

		// Resize of compositing string of IMC
		lpIMC->hCompStr = ImmReSizeIMCC(lpIMC->hCompStr,sizeof(MYCOMPSTR));
		if (lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr))
		{
			InitCompStr(lpCompStr);
			ImmUnlockIMCC(lpIMC->hCompStr);
		}
		lpIMC->hCandInfo = ImmReSizeIMCC(lpIMC->hCandInfo,sizeof(MYCAND));
		if (lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo))
		{
			InitCandInfo(lpCandInfo);
			ImmUnlockIMCC(lpIMC->hCandInfo);
		}
		
		if ((lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate)) != NULL) {
			for(int i = 0; i < MAXSTRSIZE; i++)
				lpMyPrivate->PreEditStr[i]='\0';
			for(int i = 0; i < MAXSTRSIZE; i++)
				lpMyPrivate->CandStr[i]='\0';
			ImmUnlockIMCC(lpIMC->hPrivate);	
		}
		ImmUnlockIMC(hIMC);
    }
	return TRUE;
}

BOOL APIENTRY 
ImeSetActiveContext(HIMC hIMC,BOOL fFlag)
{
    return TRUE;
}

UINT APIENTRY 
ImeToAsciiEx (UINT uVKey, UINT uScanCode,
			  CONST LPBYTE lpbKeyState,
			  LPDWORD lpdwTransKey, UINT fuState, HIMC hIMC)
{
	//murmur("ImeToAsciiEx");
	return 0;
}

BOOL APIENTRY
NotifyIME(HIMC hIMC,DWORD dwAction,DWORD dwIndex,DWORD dwValue)
{
	murmur("NotifyIME: dwAction:%x; dwIndex:%x; dwValue:%x", dwAction, dwIndex, dwValue);
    BOOL bRet = FALSE;
	//LPINPUTCONTEXT lpIMC;
	
    switch(dwAction)
    {
	case NI_OPENCANDIDATE: //0x10
		break;
	case NI_CLOSECANDIDATE: //0x11
		{
		dsvr->showCandi(false);
		break;
		}
	case NI_SELECTCANDIDATESTR://0x12
		{
		dsvr->getCandSelectedItemKey();
		
		break;
		}
	case NI_CHANGECANDIDATELIST://0x13
		break;
	case NI_SETCANDIDATE_PAGESTART://0x16
		break;
	case NI_SETCANDIDATE_PAGESIZE: //0x17
		break;
	case NI_CONTEXTUPDATED: //0x03
		switch (dwValue)
		{
		case IMC_SETCOMPOSITIONWINDOW:
			break;
		case IMC_SETCONVERSIONMODE:
			break;
		case IMC_SETSENTENCEMODE:
			break;
		case IMC_SETCANDIDATEPOS:
			break;
		case IMC_SETCOMPOSITIONFONT:
			break;
		default:
			break;
		}
		break;
		
	case NI_COMPOSITIONSTR: //0x15
		switch (dwIndex)
		{
		case CPS_COMPLETE: //0x1
			break;
		case CPS_CONVERT:
			break;
		case CPS_REVERT:
			break;
		case CPS_CANCEL:
			break;
		default:
			break;
		}
		break;
			
	default:
		break;
    }
    return bRet;
}

BOOL APIENTRY
ImeRegisterWord(LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr)
{
    return FALSE;
}

BOOL APIENTRY
ImeUnregisterWord(LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr)
{
    return FALSE;
}

UINT APIENTRY
ImeGetRegisterWordStyle(UINT nItem, LPSTYLEBUF lp)
{
	return 0;
}

UINT APIENTRY
ImeEnumRegisterWord(REGISTERWORDENUMPROC lpfn,
					LPCTSTR lpRead, DWORD dw,
					LPCTSTR lpStr, LPVOID lpData)
{
	return 0;
}

BOOL APIENTRY
ImeSetCompositionString(HIMC hIMC, DWORD dwIndex,
						LPCVOID lpComp, DWORD dwComp,
						LPCVOID lpRead, DWORD dwRead)
{
    return FALSE;
}
