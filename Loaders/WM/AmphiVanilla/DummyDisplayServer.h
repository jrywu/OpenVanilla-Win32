#ifndef DummyDISPLAYSERVER_H
#define DummyDISPLAYSERVER_H

#include "MVDisplayServer.h"

class DummyDisplayServer : public MVDisplayServer
{
public:
	virtual bool getisChineseFlag();
	virtual bool getisFullShapeFlag();
	virtual MVDisplayServer *setBufString(const char *str,int caretX=-1, int markFrom=-1, int markTo=-1);
	virtual MVDisplayServer *setBufString(const char *str);
	virtual MVDisplayServer *sendBuf(const char *str);
	virtual MVDisplayServer *setCandiString(const char *str);
	virtual MVDisplayServer *showBuf(bool t);
	virtual MVDisplayServer *showCandi(bool t);
	virtual MVDisplayServer *showNotify(const char *str);
	virtual MVDisplayServer *hideNotify();
	virtual MVDisplayServer *setCursorPos(int i);
	virtual MVDisplayServer *setMarkFrom(int i);
	virtual MVDisplayServer *setMarkTo(int i);
	virtual int getCandSelectedItemKey();
	virtual bool getStatusModuleChecked(int index);
	virtual	MVDisplayServer * setStatusModuleChecked(int index, bool b);
};
#endif //DummyDISPLAYSERVER_H
