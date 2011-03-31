//#define OV_DEBUG
//#define showCompWindow  //Uncomment this to show composition window.
#include "DisplayServer.h"
#include "OVIME.h"
#include "ImmController.h"
#include <windows.h>

DisplayServer::DisplayServer()
{
	hasCompStarted=false;
	isStatusEnabled=false;
	isCompEnabled=false;
	isCandiEnabled=false;	

	dwCandPageSize=5;
	murmur("DisplayServer:DisplayServer()\n");

}

DisplayServer::~DisplayServer()
{
	m_hIMC = NULL;
}

void DisplayServer::setHIMC(HIMC hIMC)
{
	m_hIMC = hIMC;
}

AVDisplayServer *DisplayServer::getStatusPos(LPPOINT lp_pt)
{
	
	//if(//UIGetStatusPos(lp_pt)==-1) {
	//	lp_pt->x=-1;
	//	lp_pt->y=-1;
	//}

	return this;
	 
}
AVDisplayServer *DisplayServer::setBufString(const char *str, int caretX, int markFrom, int markTo)
{
	hideNotify();

	ImmController* controller = ImmController::open();
	
	if(!controller->getCompStartedFlag()) {
		//controller->setCompStartedFlag(true);   
		murmur("\tDSVR: STARTCOMPOSITION");		
		MyGenerateMessage( m_hIMC,WM_IME_STARTCOMPOSITION,0,0);  
#ifdef showCompWindow
		MyGenerateMessage( m_hIMC, WM_IME_NOTIFY,IMN_PRIVATE,WM_IME_STARTCOMPOSITION);
#endif
	}
	
	ImmModel* model = ImmModel::open(m_hIMC);
	
	wchar_t wstr[1024];

	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
 	wcscpy(model->getMyPrivate()->PreEditStr, wstr);

	MakeCompStr(model->getMyPrivate(), model->getCompStr(), caretX, markFrom, markTo);
	wchar_t* word = GETLPCOMPSTR(model->getCompStr());

	murmur("\tDISPLAYSERVER::setbufString str:%s, len,%i, caretX:%i",str, wcslen(wstr), caretX);
	
	ImmModel::close();




	if(wcslen(wstr)) {
		
#ifndef showCompWindow
		MyGenerateMessage( m_hIMC,WM_IME_NOTIFY,IMN_PRIVATE,WM_IME_ENDCOMPOSITION);  
#endif
		murmur("\tCOMPOSITION GCS_COMPSTR"); 

#ifndef showCompWindow
		MyGenerateMessage( m_hIMC, WM_IME_COMPOSITION, 
				*(WORD*)word,
				(GCS_COMPSTR|GCS_COMPATTR
	//			|GCS_COMPREADSTR|GCS_COMPREADATTR
				|GCS_COMPCLAUSE//|GCS_COMPREADCLAUSE
				|GCS_CURSORPOS
				|GCS_DELTASTART				 
				));
#else showCompWindow
		MyGenerateMessage( m_hIMC, WM_IME_NOTIFY,IMN_PRIVATE,WM_IME_STARTCOMPOSITION);
		MyGenerateMessage( m_hIMC,WM_IME_NOTIFY,IMN_PRIVATE,WM_IME_COMPOSITION);  
#endif

	}
	else {
		controller->close();
		controller = NULL;
		murmur("\tDSRV:ENDCOMPOSITION");
		MyGenerateMessage(m_hIMC, WM_IME_ENDCOMPOSITION, 0, 0);
#ifdef showCompWindow
		MyGenerateMessage(m_hIMC, WM_IME_NOTIFY, IMN_PRIVATE, WM_IME_ENDCOMPOSITION);
#endif
	}

				

	
	return this;
}

AVDisplayServer *DisplayServer::sendBuf(const wchar_t *wstr)
{
	
	murmur("\tDISPLAYSERVER::sendbuf in wchar_t len,%i", wcslen(wstr));
	ImmModel* model = ImmModel::open(m_hIMC); 
	wcscpy(model->getMyPrivate()->PreEditStr, L"");	
	MakeCompStr(model->getMyPrivate(), model->getCompStr());

	model->getCompStr()->dwResultStrLen = (int)wcslen(wstr);
	wchar_t* word = GETLPRESULTSTR(model->getCompStr());
	wcscpy(word, wstr);
	
	ImmModel::close();

	//James:把下兩行提前，看能否解決閃爍問題  
	showBuf(false);
	//再hide後再清掉，避免殘像發生 
	//UIClearCompStr();//即時update C# comp string 同步資料
	//showCandi(false);

	murmur("\tCOMPOSITION GCS_RESULTSTR");

	MyGenerateMessage(
		m_hIMC,
		WM_IME_COMPOSITION,
		*(WORD*)word,
		GCS_COMPSTR|GCS_COMPATTR|
	//	GCS_COMPREADSTR|GCS_COMPREADATTR|
	//	GCS_COMPCLAUSE|GCS_COMPREADCLAUSE|
	//	GCS_CURSORPOS|GCS_DELTASTART|
		GCS_RESULTSTR|GCS_RESULTCLAUSE
	//  GCS_RESULTREADSTR|GCS_RESULTREADCLAUSE		
	);

	ImmController* controller = ImmController::open();
	//controller->setCompStartedFlag(false);
	controller->close();
	controller = NULL;

	murmur("\tDSRV:ENDCOMPOSITION");
	MyGenerateMessage(m_hIMC, WM_IME_ENDCOMPOSITION, 0, 0);
#ifdef showCompWindow
	MyGenerateMessage(m_hIMC, WM_IME_NOTIFY, IMN_PRIVATE, WM_IME_ENDCOMPOSITION);
#endif

	return this;
}


AVDisplayServer *DisplayServer::sendBuf(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);

	murmur("\tDISPLAYSERVER::sendbuf str:%s, len,%i",str, wcslen(wstr));
	
	return sendBuf(wstr);
	
}


AVDisplayServer *DisplayServer::setCandiString(const char *str)
{
	murmur("Displayserver::setCandString()\n");
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);

	ImmModel* model = ImmModel::open(m_hIMC);

	//wcscpy(model->getMyPrivate()->CandStr, wstr);
	UpdateCandidate(model->getIMC(), wstr, dwCandPageSize);
	ImmModel::close();
	
	//UISetCandStr(model->getMyPrivate()->CandStr);

	
	

	return this;
}

AVDisplayServer *DisplayServer::showNotify(const char *str)
{

	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
	murmur("Notifypage:%s",wstr);
	//UISetNotifyStr(wstr); 
	////RefreshUI(m_hIMC);
	//UIShowNotifyWindow();
	

	return this;
}

AVDisplayServer *DisplayServer::hideNotify()
{

	//UIHideNotifyWindow();	
	return this;
}

AVDisplayServer *DisplayServer::moveNotify(int x, int y)
{
	//UIMoveNotifyWindow(x, y);
	return this;
}
AVDisplayServer *DisplayServer::moveBuf(
	int x, int y, int fontHeight, const wchar_t* fontName)
{
	murmur("\t move comp to (%d,%d)",x,y);
	//UIMoveCompWindow(x, y, fontHeight, fontName);
	return this;
}

AVDisplayServer *DisplayServer::moveCandi(int x, int y, int compHeight)
{
	murmur("\t move candi to (%d,%d)",x,y);
	//UIMoveCandWindow(x, y, compHeight);
	return this;
}

AVDisplayServer *DisplayServer::SetCompStarted(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetCompStarted=true");
	else
		murmur("\t*DisplayServer::SetCompStarted=false");
	
	hasCompStarted=t;
	return this;
}

AVDisplayServer *DisplayServer::SetStatusEnabled(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetStatusEnabled=true");
	else
		murmur("\t*DisplayServer::SetStatusEnabled=false");
	
	isStatusEnabled=t;
	return this;
}

AVDisplayServer *DisplayServer::SetCompEnabled(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetCompEnabled=true");
	else
		murmur("\t*DisplayServer::SetCompEnabled=false");
	
	isCompEnabled=t;
	return this;
}

AVDisplayServer *DisplayServer::SetCandiEnabled(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetCandiEnabled=true");
	else
		murmur("\t*DisplayServer::SetCandiEnabled=false");
	
	isCandiEnabled=t;
	return this; 
}

AVDisplayServer *DisplayServer::showStatus(bool t)
{	
	murmur("dsvr: show status window: %d", t);
	if(dsvr->isStatusEnabled && !t) 
	{
		//UIHideStatusWindow();
		dsvr->SetStatusEnabled(false);
	}
	if(!dsvr->isStatusEnabled && t)
	{
		//UIShowStatusWindow();
		dsvr->SetStatusEnabled(true);
	}
	
	return this;
}
/*
bool AVDisplayServer::GetisStatusDocked()
{
	return 0; //UIGetIsStatusDocked();
}
void AVDisplayServer::SetisStatusDocked(bool t)
{
	//UISetIsStatusDocked(t);
	
}
*/

AVDisplayServer *DisplayServer::showBuf(bool t)
{
	if(dsvr->isCompEnabled)
	{
		ImmModel* model = ImmModel::open(m_hIMC);

		if(t &&
			model->getMyPrivate()->PreEditStr &&
			wcslen(model->getMyPrivate()->PreEditStr))
		{
			murmur("\t show comp");
			//UIShowCompWindow();
		}
		else
		{
			murmur("\t hide comp");
			//UIHideCompWindow();
		}

		ImmModel::close();
	}
	return this;
}

AVDisplayServer *DisplayServer::showCandi(bool t)
{
	murmur("Displayserver::showCandi()\n");
	if(!dsvr->isCandiEnabled) 
		dsvr->SetCandiEnabled(true);
		
//	{
		ImmModel* model = ImmModel::open(m_hIMC);

		if(t)//&&	model->getMyPrivate()->CandStr && wcslen(model->getMyPrivate()->CandStr))
		{
			// Windows Mobile specific!! Open composition window first to get correct caret position.
			murmur("\t show candi"); 
#ifndef showCompWindow
			MyGenerateMessage(m_hIMC, WM_IME_NOTIFY,  IMN_PRIVATE, WM_IME_STARTCOMPOSITION);
#endif
			MyGenerateMessage(m_hIMC, WM_IME_NOTIFY,  IMN_SETCANDIDATEPOS, 0);
			MyGenerateMessage(m_hIMC, WM_IME_NOTIFY, IMN_OPENCANDIDATE, 0);
			MyGenerateMessage(m_hIMC, WM_IME_NOTIFY, IMN_CHANGECANDIDATE,1);
#ifndef showCompWindow
			MyGenerateMessage(m_hIMC, WM_IME_NOTIFY,  IMN_PRIVATE, WM_IME_ENDCOMPOSITION);
#endif
			//RefreshUI(m_hIMC);
			//UIShowCandWindow();
		}
		else
		{
			murmur("\t hide candi");
			MyGenerateMessage(m_hIMC, WM_IME_NOTIFY, IMN_CLOSECANDIDATE, 0);
			//UIHideCandWindow();
		}

		ImmModel::close();
//	}
	return this;
}



AVDisplayServer *DisplayServer::setMarkFrom(int i)
{
	//UISetMarkFrom(i);
	return this;
}

AVDisplayServer *DisplayServer::setMarkTo(int i)
{
	//UISetMarkTo(i);
	return this;
}

AVDisplayServer *DisplayServer::selectNextCandItem(int i)
{
	murmur("selectNextCandItem:%d\n",i);
	ImmModel* model = ImmModel::open(m_hIMC);
	LPCANDIDATEINFO lpCandInfo=model->getCandInfo();
	LPCANDIDATELIST lpCandList = (LPCANDIDATELIST)((LPBYTE)(lpCandInfo) + lpCandInfo->dwOffset[0]);
	switch (i) {
		case 1: //line down
			lpCandList->dwStyle = 1;
			break;
			
		case -1: //line up
			lpCandList->dwStyle = 2;
			break;
		case 2: // Page down
			lpCandList->dwStyle = 3;
			break;
		case -2: // Page up
			lpCandList->dwStyle = 4;
			break;
		default:
			break;
	}
	ImmModel::close();
	MyGenerateMessage(m_hIMC, WM_IME_NOTIFY, IMN_CHANGECANDIDATE,1);

	
	//UISelectNextCandItem(i);
	return this;
}


int DisplayServer::getCandSelectedItemKey()
{
	AVLoader* loader= AVLoader::open();
	murmur("getCandSelectedItem\n");
	ImmModel* model = ImmModel::open(m_hIMC);
	LPCANDIDATEINFO lpCandInfo=model->getCandInfo();
	LPCANDIDATELIST lpCandList = (LPCANDIDATELIST)((LPBYTE)(lpCandInfo) + lpCandInfo->dwOffset[0]);
	//const wchar_t* lpStr = GETLPCANDSTR(lpCandList, lpCandList->dwSelection);
	DWORD dwSelection = lpCandList->dwSelection;
	ImmModel::close();
	//sendBuf(lpStr);

	AVKeyCode Key;
	Key.setAlt(1); 
	Key.setCtrl(1);
	Key.setShift(1);
	Key.setCode(dwSelection);
	loader->keyEvent(UICurrentInputMethod(), Key);
	


	return 1;//UIGetCandSelectedItemKey();
	
}

bool DisplayServer::getStatusModuleChecked(int index)
{
	return true;//UIGetModuleChecked(index);
}

AVDisplayServer *DisplayServer::setStatusModuleChecked(int index, bool b)
{
	//UISetModuleChecked(index, b);
	return this;
}
