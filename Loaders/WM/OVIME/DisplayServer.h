#ifndef DISPLAYSERVER_H
#define DISPLAYSERVER_H

#include <windows.h>
#include <imm.h>
#include "ExtraStructs.h"
#include "AVDisplayServer.h"

#include "ImmModel.h"

extern bool isChinese;
extern bool isFullShape;

class DisplayServer : public AVDisplayServer
{
public:
	DisplayServer();
	~DisplayServer();

	void setHIMC(HIMC hIMC);
	virtual AVDisplayServer * getStatusPos(LPPOINT);
	bool getisChineseFlag() {return isChinese;}
	bool getisFullShapeFlag() {return isFullShape;}

	//virtual AVDisplayServer *setBufString(const char *str);
	virtual AVDisplayServer *setBufString(const char *str,int caretX=0, int markFrom=-1, int markTo=-1); 
	virtual AVDisplayServer *sendBuf(const char *str);
	virtual AVDisplayServer *sendBuf(const wchar_t *wstr);
	virtual AVDisplayServer *setCandiString(const char *str);
	virtual AVDisplayServer *showBuf(bool t);
	virtual AVDisplayServer *showNotify(const char *str);
	virtual AVDisplayServer *hideNotify();
	virtual AVDisplayServer *moveNotify(int x, int y);
	virtual AVDisplayServer *showCandi(bool t);
	//virtual AVDisplayServer *setCursorPos(int i);
	virtual AVDisplayServer *setMarkFrom(int i);
	virtual AVDisplayServer *setMarkTo(int i);
	virtual AVDisplayServer *selectNextCandItem(int i);
	virtual int				getCandSelectedItemKey();
	virtual bool			getStatusModuleChecked(int index);
	virtual	AVDisplayServer * setStatusModuleChecked(int index, bool b);


	virtual DisplayServer *showStatus(bool t);
	virtual DisplayServer *moveBuf(
		int x, int y, int fontHeight, const wchar_t* fontName);
	virtual DisplayServer *moveCandi(int x, int y, int compHeight);

	virtual DisplayServer *SetStatusEnabled(bool t);
	virtual DisplayServer *SetCompEnabled(bool t);
	virtual DisplayServer *SetCandiEnabled(bool t);
	virtual DisplayServer *SetCandPageSize(DWORD n) { dwCandPageSize = n; return this;}

	virtual DisplayServer *SetCompStarted(bool t);

	virtual bool			GetisStatusDocked();
	virtual void			SetisStatusDocked(bool t);

public:	//James: 為了在Utils.cpp, IME.cpp 使用，暫改成public
	bool hasCompStarted; 
	bool isStatusEnabled; 
	bool isCompEnabled; 
	bool isCandiEnabled;
	

private:
	HIMC m_hIMC;

	DWORD dwCandPageSize;

};
#endif //DISPLAYSERVER_H

