#define OV_DEBUG
#include "DummyDisplayServer.h"
#include <iostream>
#include "OVUtility.h"

using namespace std;

MVDisplayServer *DummyDisplayServer::setBufString(const char *str)
{
	murmur("setbufstring:%s",str);
	cout << "setBufString " << str << endl;
	return this;
}
MVDisplayServer *DummyDisplayServer::setBufString(const char *str,int caretX, int markFrom, int markTo)
{
	murmur("setbufstring:%s",str);
	cout << "setBufString " << str << endl;
	return this;

}
MVDisplayServer *DummyDisplayServer::sendBuf(const char *str)
{
	murmur("sendbuf: %s",str);
	cout << "sendBufString " << str << endl;
	return this;
}
MVDisplayServer *DummyDisplayServer::setCandiString(const char *str)
{
	murmur("setcandistr: %s",str);
	cout << "setCandiString " << str << endl;
	return this;
}
MVDisplayServer *DummyDisplayServer::showBuf(bool t)
{
	cout << "showBuf " << t << endl;
	return this;
}
MVDisplayServer *DummyDisplayServer::showCandi(bool t)
{
	cout << "showCandi " << t << endl;
	return this;
}
MVDisplayServer *DummyDisplayServer::showNotify(const char *str)
{
	cout << "showNotify " << str << endl;
	return this;
}
MVDisplayServer *DummyDisplayServer::hideNotify()
{
	cout << "hideNotify" << endl;
	return this;
}
MVDisplayServer *DummyDisplayServer::setCursorPos(int i)
{
	cout << "setCursorPos " << i << endl;
	return this;
}
MVDisplayServer *DummyDisplayServer::setMarkFrom(int i)
{
	cout << "setMarkFrom " << i << endl;
	return this;
}
MVDisplayServer *DummyDisplayServer::setMarkTo(int i)
{
	cout << "setMarkTo " << i << endl;
	return this;
}


bool DummyDisplayServer::getisChineseFlag()
{
	cout << "getisChineseFlag" << endl;
	return true;
}

bool DummyDisplayServer::getisFullShapeFlag()
{
	cout << "getisFullFlag" << endl;
	return true;
}

int DummyDisplayServer::getCandSelectedItemKey()
{
	cout << "getCandSelectedItemKey" << endl;
	return 0;
}

bool DummyDisplayServer::getStatusModuleChecked(int index)
{
	cout << "getStatusModuleChecked " << index << endl;
	return true;
}

MVDisplayServer * DummyDisplayServer::setStatusModuleChecked(int index, bool b)
{
	cout << "setStatusModuleChecked " << index <<":"<< b << endl;
	return this;
}
