#include "MVIME.h"
#include <stdio.h>

HINSTANCE hInst;
WCHAR UICLASSNAME[32];

int CompX;
int CompY;
int CandX;
int CandY;
bool isChinese = true;
bool isTraditional = true;
bool isFullShape = false;

DisplayServer* dsvr = new DisplayServer();

//AVLoader* loader = AVLoader::open();
