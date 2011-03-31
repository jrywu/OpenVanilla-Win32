#ifndef AVKeyCode_h
#define AVKeyCode_h
#include "OpenVanilla.h"
#include <ctype.h>

class AVKeyCode : public OVKeyCode  {
public:
    AVKeyCode (int p = 0)
	{
		chr = toupper(p);
		isShift_ = isCapsLock_ = isCtrl_ = isAlt_ = isNum_ =0;
	}
    virtual int code()			{ return (isShift_||isCapsLock_||isCtrl_||isAlt_) ? chr : tolower(chr); }
    virtual bool isShift()		{ return isShift_; }
    virtual bool isCapslock()	{ return isCapsLock_; }
    virtual bool isCtrl()		{ return isCtrl_; }
    virtual bool isAlt()			{ return isAlt_; }
	virtual bool isNum()		{ return isNum_; }
    
    virtual void setCode(int x)		{ chr = toupper(x); }
    virtual void setShift(bool x)			{ isShift_ = x; }
    virtual void setCapslock(bool x)	{ isCapsLock_ = x; }
    virtual void setCtrl(bool x)			{ isCtrl_ = x; }
    virtual void setAlt(bool x)			{ isAlt_ = x; }
	virtual void setNum(bool x)			{ isNum_ = x; }
protected:
    int chr;
    bool isShift_, isCapsLock_, isCtrl_, isAlt_, isNum_;
};
#endif // AVKeyCode_h
