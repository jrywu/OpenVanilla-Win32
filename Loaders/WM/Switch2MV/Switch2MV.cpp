// Switch2MV.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>


int _tmain(int argc, _TCHAR* argv[])
{
	CLSID Clsid_MV = {0xE80D7526, 0x58DF, 0x4142, {0xA9, 0x58, 0x72, 0x49, 0x0A, 0x69, 0x1B, 0x46}};
	//{E80D7526-58DF-4142-A958-72490A691B46};
	SipSetCurrentIM(&Clsid_MV);
	SipShowIM(SIPF_OFF);
	return 0;
}

