#include <stdio.h>

#include "MVLoader.h"
#include "MVKeyCode.h"
#include "MVDisplayServer.h"
#include "DummyDisplayServer.h"

int main(int argc, char *argv[])
{
	char str[1024];
	char c;
	MVLoader *loader = MVLoader::open();
	MVLoader::close();
	loader = MVLoader::open();
	MVDisplayServer *dsvr = new DummyDisplayServer();
	loader->connectDisplayServer(dsvr);
	
	murmur("All inited");
	while( c = getchar())
	{
		if(c == EOF)
			break;
		if(c == '\n')
			continue;
		MVKeyCode kc(c);
		loader->keyEvent(0, kc);
	}
	
	MVLoader::close();
	murmur("shutdown");
	delete dsvr;
	return 0;
}
