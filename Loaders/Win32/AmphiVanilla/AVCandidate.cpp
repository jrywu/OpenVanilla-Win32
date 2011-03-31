#include "AVCandidate.h"

AVCandidate::AVCandidate() : candiOnscreen(0) {}
AVCandidate::AVCandidate(AVDisplayServer* svr) : dsvr(svr), candiOnscreen(0) {}

void AVCandidate::setDisplayServer(AVDisplayServer *svr)
{
	dsvr = svr;
}
OVCandidate* AVCandidate::clear() {
	candistr = "";
	return this;
}
OVCandidate* AVCandidate::append(const char *s) {
	candistr += s;
	return this;
}
OVCandidate* AVCandidate::hide() {
	if (candiOnscreen) { candiOnscreen=0; dsvr->showCandi(false);}
	return this;
}
OVCandidate* AVCandidate::show() {
#ifndef WINCE
	if (!candiOnscreen)  //fix for autocompsing candi update
	{	
#endif
		if(candistr.length()>0)
		{
			candiOnscreen=1; 
			dsvr->showCandi(true);
		}
#ifndef WINCE
	}
#endif
	return this;
}
OVCandidate* AVCandidate::update() {
	dsvr->setCandiString(candistr.c_str());
	return this; 
}
bool AVCandidate::onScreen() {
	return candiOnscreen;
}
