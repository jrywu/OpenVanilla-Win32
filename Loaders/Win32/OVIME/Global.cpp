#include "OVIME.h"
#include <stdio.h>

HINSTANCE hInst;
int CompX;
int CompY;
int CandX;
int CandY;
bool isChinese = true;
bool isTraditional = true;
bool isFullShape = false;

DisplayServer* dsvr = new DisplayServer();

//AVLoader* loader = AVLoader::open();

// For UI on windows CE.
#ifdef WINCE
int CurrentIC=0;

//const WCHAR UICLASSNAME[]=TEXT("OVIMEUI");
//const WCHAR UICOMPCLASSNAME[]=TEXT("OVIMECOMP");
//const WCHAR UICANDCLASSNAME[]=TEXT("OVIMECAND");
//const WCHAR UISTATUSCLASSNAME[]=TEXT("OVIMESTATUS");

#endif
