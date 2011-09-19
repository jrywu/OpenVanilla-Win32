// OVIMGeneric.cpp: Generic Input Method, reads .cin
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

//#define OV_DEBUG
#define wildcardAutoCompose

#include "OVIMGenericSQ.h"
#include "OVOFReverseLookupSQ.h"
#include "CIN-Defaults.h"


using namespace std;

OVCINSQList *cinlist=NULL;
//SQLite3 *db=NULL;

extern "C" unsigned int OVGetLibraryVersion() {
    return OV_VERSION;
}

extern "C" int OVInitializeLibrary(OVService *s, const char *libpath) {
    if (cinlist) {
        // already initializde
        return false;
    }

    const char *pathsep=s->pathSeparator();
 
    // will be something like this on OS X:
    //     ~/Library/OpenVanilla/version/UserSpace/OVIMGeneric/
    string userpath=s->userSpacePath("OVIMGeneric");

	murmur(" userSpacePath %s", userpath.c_str());
    // will be something like this on OS X:
    //     /Library/OpenVanilla/version/Modules/OVIMGeneric/
	// will be something like this on Windows Vista/7+
	//     c:\users\[username]\appdata\roadming\Openmanilla\OVIMGeneric
    string datapath=string(libpath) + string(pathsep) + string("OVIMGeneric");

	murmur(" dataPath %s", datapath.c_str());
 
    string dbfile = userpath + string("imdb.db");
	string userdbfile = userpath + string("userdb.db");
	cinlist = new OVCINSQList(pathsep, dbfile.c_str(), userdbfile.c_str());
    if (!cinlist) return false;

	
	//Watch watch;
    int loaded=0;

    //watch.start(); 
    loaded += cinlist->load(userpath.c_str(), ".cin");
    //watch.stop();

    //murmur("Loaded modules from %s in %1.3f",userpath.c_str(), watch.getSec());

    //watch.start(); 
    loaded += cinlist->load(datapath.c_str(), ".cin");
    //watch.stop();

    //murmur("Loaded modules from %s in %1.3f", datapath.c_str(), watch.getSec());

	if (!loaded) 		loaded = cinlist->loadfromdb();
	if (!loaded)
	{
	        murmur ("OVIMGeneric: nothing loaded, init failed");
		    return false;
	}
    
    return true;
}
extern "C" OVModule *OVGetModuleFromLibrary(int x) {
	murmur("OVGetModuleFromLibrary:%d; cinlist->count:%d", x , cinlist->count());
	
	if(cinlist->count() == 0 ) return NULL; 

		
	if ((size_t)x < ( cinlist->count() ))    // IM Modules
	{

		murmur("OVGetModuleFromLibrary:%s", cinlist->cinInfo((size_t)x).shortfilename.c_str());
	
		//if(cinlist->getAssocCinInfo().shortfilename =="assoc.cin") {
			return new OVIMGeneric(cinlist->cinInfo((size_t)x), cinlist->getAssocCinInfo(), cinlist->getdb());
		//}
		//else
		//	return new OVIMGeneric(cinlist->cinInfo((size_t)x), cinlist->getdb());
	}
#ifndef WINCE 
	else if ((size_t)x < 2* cinlist->count())  // Reverse lookup
	{
		size_t y = x - (cinlist->count()) ;
		murmur("OVGetModuleFromLibrary:%s", cinlist->cinInfo(y).shortfilename.c_str());
		return new OVOFReverseLookup(cinlist->cinInfo(y), cinlist->getdb());

	}
#endif
	else
		return NULL;

 
}

GenericKeySequence::GenericKeySequence(OVCINSQ* cintab) {
    cinTable=cintab;
}
    
bool GenericKeySequence::valid(char c) {
	string inKey;
	inKey.push_back(c);
    if (!cinTable->isValidKey(inKey)) return false;
    return true;
}
    
bool GenericKeySequence::add(char c) {
    if (valid(c) == 0) return false;
    return OVKeySequenceSimple::add(c);
}
    
string *GenericKeySequence::compose(string *s)
{
    for (int i=0; i<len; i++)
    {
		string inKey;
		inKey.push_back(seq[i]);
		vector<string> outStringVectorRef;
        if (cinTable->getCharVectorByKey(inKey, outStringVectorRef) > 0)
			s->append(outStringVectorRef[0]);
    }
    return s;
}

OVIMGeneric::OVIMGeneric(const OVCINSQInfo& ci, SQLite3 *dbo) : cininfo(ci), cintab(NULL), assoctab(NULL), db(dbo) {
    idstr = "OVIMGeneric-" + cininfo.shortfilename;
}

OVIMGeneric::OVIMGeneric(const OVCINSQInfo& ci, const OVCINSQInfo& aci, SQLite3 *dbo) : cininfo(ci), associnfo(aci), cintab(NULL), assoctab(NULL), db(dbo) {
    idstr = "OVIMGeneric-" + cininfo.shortfilename;
}

OVIMGeneric::~OVIMGeneric() {
    if (cintab) delete cintab;
}

const char* OVIMGeneric::identifier()
{
    return idstr.c_str();
}

const char* OVIMGeneric::localizedName(const char* locale)
{
    if (!strcasecmp(locale, "zh_TW")) return cininfo.tcname.c_str();
    if (!strcasecmp(locale, "zh_CN")) return cininfo.scname.c_str();
    return cininfo.ename.c_str();
}

int OVIMGeneric::initialize(OVDictionary* global, OVService* srv, const char*)
{
	srv->notify("Creating table index... Please wait..."); 
    //Watch watch;
    //watch.start();
    if (!cintab) {
        cintab=new OVCINSQ(cininfo,db); 
     }
	if(associnfo.shortfilename == string("assoc.cin") && !assoctab)	{
		assoctab = new OVCINSQ(associnfo,db);
	}

    //watch.stop();
	
    //murmur("OVIMGeneric: initializing %s in %1.3f sec", identifier(), watch.getSec());
    update(global, srv);
    return 1;
}

void OVIMGeneric::update(OVDictionary* global, OVService*)
{
    CINSetDefaults(cininfo.shortfilename.c_str(), global);
    
    cfgMaxSeqLen=global->getInteger(CIN_MAXSEQLEN);
    cfgBeep=global->getInteger(CIN_WARNINGBEEP);
    cfgAutoCompose=global->getInteger(CIN_AUTOCOMPOSE);
    cfgHitMaxAndCompose=global->getInteger(CIN_HITMAX);

	if(!global->getInteger(CIN_SHIFTSELECTIONKEY))
	   doShiftSelKey = false;
	else
	   doShiftSelKey = true;
		
	const char *cfgstr;
	cfgstr = global->getStringWithDefault(CIN_MATCHONECHAR, "?");
	cfgMatchOneChar = cfgstr[0];
	
	cfgstr = global->getStringWithDefault(CIN_MATCHZEROORMORECHAR, "*");
	cfgMatchZeroOrMoreChar = cfgstr[0];

	cfgAssociatedPhrase=global->getIntegerWithDefault(CIN_ASSOCIATEDPHRASE, 1) == 0 ? false : true;
	cfgLearnAssociatedPhrase=global->getIntegerWithDefault(CIN_LEARNASSOCIATEDPHRASE, 1) == 0 ? false : true;

#ifdef WINCE
	cfgOrderWordsByFreq=global->getIntegerWithDefault(CIN_ORDERWORDSBYFREQ, 1) == 0 ? false : true;
#else
	cfgOrderWordsByFreq=global->getIntegerWithDefault(CIN_ORDERWORDSBYFREQ, 0) == 0 ? false : true;
#endif

	
}

OVInputMethodContext *OVIMGeneric::newContext()
{
    return new OVGenericContext(this, cintab);
}

void OVGenericContext::updateDisplay(OVBuffer *buf)
{
    buf->clear();
    // murmur("UpdateDisplay");
    if (keyseq.length())
    {
        string *ms= new string;
        keyseq.compose(ms);
        buf->append(ms->c_str());
        delete ms;
    }
    buf->update();
}
void OVGenericContext::start(OVBuffer*, OVCandidate*, OVService*) {

}
void OVGenericContext::end() {
	

}

void OVGenericContext::clear() {
    keyseq.clear();
    autocomposing=false;
    candi.cancel();
}



int OVGenericContext::keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar, OVService *srv) {
	//murmur("OVGenericContext::Keyevent:%c; assoconduty:%d",key->code(), assoconduty);
    if (candi.onDuty())
    {
		string output;

#ifdef WINCE  // select candi from UI : ctrl+alt+shift : key.code()=dwSelection.
		
		if(candi.uiselect(key, output)) 
		{
			murmur("select candi from UI: dwselection %i", key->code());

            buf->clear()->append(output.c_str())->update()->send();

            keyseq.clear();
            cancelAutoCompose(textbar);
			updateFreqFetchAssociatedPhrase(output,textbar);
/*-------------------------------
			if(assoconduty) cintab->updatePhraseUserFrequency((lastPhrase+output).c_str(), (parent->doLearnAssociatedPhrase(),true); // update user freq.
			else	cintab->updatePhraseUserFrequency(output.c_str(), parent->doLearnAssociatedPhrase(), false); // update user freq.
			lastPhrase = output;

			if(assoconduty) assoconduty = false;
			
			if(parent->doAssociatedPhrase() &&
				cintab->getAssociatedPhrases(output.c_str(), candidateStringVector)
				) {
			//Fetch associated phrase... 
			candi.prepare(&candidateStringVector, "!@#$%^&*()", textbar); 
			assoconduty = true;
			
			}
*///..........................
            return 1;
		}
#endif  
		
        if (!autocomposing && !assoconduty) return candidateEvent(key, buf, textbar, srv);

		if (key->code() == ovkDown ||
            key->code() == ovkLeft ||
            key->code() == ovkUp ||
            key->code() == ovkRight 
			)	//|| (!candi.onePage() && key->code()==ovkSpace && !assoconduty))
			return candidateEvent(key, buf, textbar, srv);
        
		
        if (candi.select(key->code(), output))
        {
			murmur("candi.select:%c", key->code());
            buf->clear()->append(output.c_str())->update()->send();
            keyseq.clear();
            cancelAutoCompose(textbar);			

			updateFreqFetchAssociatedPhrase(output, textbar);
/*/==========
			if(assoconduty) cintab->updatePhraseUserFrequency((lastPhrase+output).c_str(), parent->doLearnAssociatedPhrase(), true); // update user freq.
			else	cintab->updatePhraseUserFrequency(output.c_str(), parent->doLearnAssociatedPhrase(), false); // update user freq.
			lastPhrase = output;




			if(assoconduty) assoconduty = false;
			
			if(parent->doAssociatedPhrase() &&
				cintab->getAssociatedPhrases(output.c_str(), candidateStringVector, parent->doLearnAssociatedPhrase())
				) {
			//Fetch associated phrase... 
#if defined(WIN32) && !defined(WINCE)
			candi.prepare(&candidateStringVector, "!@#$%^&*()", textbar, "1234567890", string("Shift+#\xE3\x80\x80"));
#else
			candi.prepare(&candidateStringVector, "!@#$%^&*()", textbar); 
#endif
			assoconduty = true;
			}
*///..........................
			

            return 1;
        }
    }

	if(assoconduty) {
		murmur("reset candi & assoconduty");
		assoconduty = false;
		textbar->hide()->clear();
		candi.cancel();
	}

    if (!keyseq.length() && !isprint(key->code())) return 0;

    if (key->code()==ovkEsc)
    {
        cancelAutoCompose(textbar);
        buf->clear()->update();
        keyseq.clear();
        return 1;
    }

    if (key->code() == ovkDelete || key->code() == ovkBackspace)
    {
        keyseq.remove();
        updateDisplay(buf);
        if (!keyseq.length() && autocomposing) cancelAutoCompose(textbar);
        
        // if autocomposing is on
        if (keyseq.length() && parent->isAutoCompose())
        {
						
			if (cintab->getWordVectorByCharWithWildcardSupport(
#ifdef wildcardAutoCompose
				keyseq.getSeq(), candidateStringVector, parent->matchOneChar(), parent->matchZeroOrMoreChar(), parent->doOrderWordsByFreq()), true)
#else
				keyseq.getSeq(), candidateStringVector, parent->matchOneChar(), parent->matchZeroOrMoreChar(), parent->doOrderWordsByFreq()))
#endif
            {
                autocomposing=true;
#ifdef wildcardAutoCompose
                compose(buf, textbar, srv, true);
#else
				compose(buf, textbar, srv);
#endif
            }
            else if (candi.onDuty()) cancelAutoCompose(textbar);
        }

        return 1;
    }

    if (keyseq.length() && key->code()==ovkSpace)
    {
        // there the candiate window has only one page, space key
        // will send out the first candidate
        if (autocomposing && candi.onDuty())
        {            
			keyseq.clear();
            autocomposing=false;
            cancelAutoCompose(textbar);
            return candidateEvent(key, buf, textbar, srv);
        }

        autocomposing=false;        
        return compose(buf, textbar, srv);
    }
    
    // we send back any CTRL/OPT/CMD key combination 
    // <comment author='lukhnos'>In OV 0.7 this part will be processed by
    // pre-IM key filters</comment>
    if (key->isOpt() || key->isCommand() || key->isCtrl())
    {
        if (!buf->isEmpty()) {
            cancelAutoCompose(textbar);
            buf->clear()->update();
            keyseq.clear();
        }
        return 0;
    }
    
    
    // shift and capslock processing
	// <comment author='b6s'>Shift processing is disabled.</comment>
    if (isprint(key->code()) && (key->isCapslock() /*|| key->isShift()*/))
    {
        if (key->isCapslock())
        {
            char cbuf[2];
            if (key->isShift())
                sprintf(cbuf, "%c", toupper(key->code()));
            else
                sprintf(cbuf, "%c", tolower(key->code()));

            buf->append(cbuf);
        }
        //else if (key->isShift()) buf->appendChar(key->lower());
        cancelAutoCompose(textbar);
        keyseq.clear();
        buf->update()->send();
        return 1;
    }

    if (isprint(key->code()) && keyseq.valid(static_cast<char>(key->code())) &&
		/*!key->isShift() &&*/ !key->isCapslock())
    {
    	if (keyseq.length() > 0 &&
    		// prevent to the exception of parent->maxSeqLen() == 0
    		keyseq.length() == parent->maxSeqLen())
    	{
            updateDisplay(buf);
			srv->notify("\xE6\x8C\x89\xE9\x8D\xB5\xE6\x9C\x89\xE8\xAA\xA4 1"); //«öÁä¦³»~
            if (parent->isBeep())
				srv->beep();
            return 1;
    	}
    	
        keyseq.add(key->code());
		// murmur("add %d", key->code());
        if (keyseq.length() == parent->maxSeqLen() &&
        parent->isHitMaxAndCompose())
        {
            autocomposing=false;
            cancelAutoCompose(textbar);
            return compose(buf, textbar, srv);
        }
		
        updateDisplay(buf);
        if (cintab->isEndKey(static_cast<char>(key->code())))
        {
            autocomposing=false;
            cancelAutoCompose(textbar);
            return compose(buf, textbar, srv);
        }
        
        // if autocomposing is on
        if (parent->isAutoCompose())
        {
            if (cintab->getWordVectorByChar(keyseq.getSeq(),
#ifdef wildcardAutoCompose
				candidateStringVector, parent->doOrderWordsByFreq(), true ))
#else
				candidateStringVector, parent->doOrderWordsByFreq()))
#endif
            {
                autocomposing=1;
#ifdef wildcardAutoCompose
                compose(buf, textbar, srv, true);
#else
				compose(buf, textbar, srv);
#endif
            }
            else if (candi.onDuty()) cancelAutoCompose(textbar);
        }
		        
        return 1;
    }
    
    if (!buf->isEmpty()) {
		srv->notify("\xE6\x8C\x89\xE9\x8D\xB5\xE6\x9C\x89\xE8\xAA\xA4 2"); //«öÁä¦³»~
        if (parent->isBeep())
			srv->beep();
        return 1;
    }
    
    if (isprint(key->code())) {
        char sb[2];
        sprintf(sb, "%c", key->code());
        buf->append(sb)->update()->send();
        return 1;
    }
    
    return 0;
}

void OVGenericContext::cancelAutoCompose(OVCandidate *textbar)
{
    autocomposing=false;
    candi.cancel();
    textbar->hide()->clear();
}
    
        
int OVGenericContext::compose(OVBuffer *buf, OVCandidate *textbar, OVService *srv, bool doAC)
{
	murmur("OVGenericContext::compose()\n");
	size_t size;

    if (!keyseq.length()) return 0;
	size =
		cintab->getWordVectorByCharWithWildcardSupport( 
				keyseq.getSeq(), candidateStringVector, parent->matchOneChar(), parent->matchZeroOrMoreChar(), parent->doOrderWordsByFreq(), doAC);
	
    if (size == 0)
    {
		srv->notify("\xE6\x8C\x89\xE9\x8D\xB5\xE6\x9C\x89\xE8\xAA\xA4 3"); //«öÁä¦³»~
        if (parent->isBeep())
			srv->beep();
        return 1;
    }

    if (size ==1 && !autocomposing)
    {
        buf->clear()->append(candidateStringVector[0].c_str())->update()->send();

        keyseq.clear();
		updateFreqFetchAssociatedPhrase(candidateStringVector[0],textbar);
		
/*//---------------------------------				
		if(assoconduty) cintab->updatePhraseUserFrequency((lastPhrase+candidateStringVector[0]).c_str(),parent->doLearnAssociatedPhrase(),true); // update user freq.
		else	cintab->updatePhraseUserFrequency(candidateStringVector[0].c_str(), parent->doLearnAssociatedPhrase(), false); // update user freq.
		lastPhrase = candidateStringVector[0];


		if(parent->doAssociatedPhrase() &&
			cintab->getAssociatedPhrases(candidateStringVector[0].c_str(), candidateStringVector, parent->doLearnAssociatedPhrase())
		) {
		// Fetch associated phrases.
		
#if defined(WIN32) && !defined(WINCE)
			candi.prepare(&candidateStringVector, "!@#$%^&*()", textbar, "1234567890", string("Shift+#\xE3\x80\x80"));
#else
			candi.prepare(&candidateStringVector, "!@#$%^&*()", textbar); 
#endif
			assoconduty = true;
		
		}
*/// .........................
        return 1;
    }

    if (!autocomposing)
    {    
        buf->clear()->append(candidateStringVector[0].c_str())->update(); 
        keyseq.clear();
    }
	
	string currentSelKey = cintab->getSelKey();

	if(parent->isShiftSelKey())
		currentSelKey = " " + currentSelKey;
	
	murmur("\tcandi.prepare\n");
    candi.prepare(&candidateStringVector,
				  const_cast<char*>(currentSelKey.c_str()), textbar);
    return 1;
}

int OVGenericContext::candidateEvent(OVKeyCode *key, OVBuffer *buf, 
    OVCandidate *textbar, OVService *srv)
{
	
    if (key->code() == ovkEsc || key->code() == ovkBackspace)
    {
        textbar->hide()->clear();
        candi.cancel();
        buf->clear()->update();
        return 1;
    }

    if (key->code() == ovkDown || key->code() == ovkRight )
//       || (!candi.onePage() && key->code()==ovkSpace))
    {
        candi.pageDown()->update(textbar);
        return 1;
    }

    if (key->code() == ovkUp || key->code() == ovkLeft)
    {
        candi.pageUp()->update(textbar);
        return 1;
    }

    // enter == first candidate
    // space (when candidate list has only one page) == first candidate
    char c=key->code();
    if (key->code() == ovkReturn || 
        (candi.onePage() && key->code()==ovkSpace) ) c=candi.getSelKey()[0];

    string output;
    if (candi.select(c, output)) {
		murmur("candi.select: %c, %s\n",c, output);
        buf->clear()->append(output.c_str())->update()->send();

        candi.cancel();
        textbar->hide()->clear();
		
		updateFreqFetchAssociatedPhrase(output,textbar);
/*/---------------
		if(assoconduty) cintab->updatePhraseUserFrequency((lastPhrase+output).c_str(), parent->doLearnAssociatedPhrase(), true); // update user freq.
		else	cintab->updatePhraseUserFrequency(output.c_str(), parent->doLearnAssociatedPhrase(),false); // update user freq.
		lastPhrase = output;
//----------------


		if(parent->doAssociatedPhrase() &&
			cintab->getAssociatedPhrases(output.c_str(), candidateStringVector, parent->doLearnAssociatedPhrase())	
		) {
		// Fetch associated phrases.
		
#if defined(WIN32) && !defined(WINCE)
			candi.prepare(&candidateStringVector, "!@#$%^&*()", textbar, "1234567890", string("Shift+#\xE3\x80\x80"));
#else
			candi.prepare(&candidateStringVector, "!@#$%^&*()", textbar); 
#endif		
			assoconduty = true;
		
		}
*///----------------------------
        return 1;
    }

	string inKey;
	inKey.push_back(c);
    if (cintab->isValidKey(inKey) || cintab->isEndKey(c)) {

        string output;

		candi.select(candi.getSelKey()[0], output);
		buf->clear()->append(output.c_str())->update()->send();
		// update user freq only. no associated phrases because endkey is used to end the composition.
		cintab->updatePhraseUserFrequency(output.c_str(), parent->doLearnAssociatedPhrase(),false); 

		keyseq.add(c);
		updateDisplay(buf);
		candi.cancel();
		textbar->hide()->clear();

		if(cintab->isEndKey(c))
			compose(buf, textbar, srv);
		
		return 1;			
    }
	
	srv->notify("\xE6\x8C\x89\xE9\x8D\xB5\xE6\x9C\x89\xE8\xAA\xA4 4"); //«öÁä¦³»~
	if (parent->isBeep())
		srv->beep();

		return 1;
}


void OVGenericContext::updateFreqFetchAssociatedPhrase(const string& phrase, OVCandidate *textbar ){

	if(assoconduty) cintab->updatePhraseUserFrequency((lastPhrase + phrase).c_str(), parent->doLearnAssociatedPhrase(), true); // update user freq.
	else	cintab->updatePhraseUserFrequency(phrase.c_str(), parent->doLearnAssociatedPhrase(), false); // update user freq.
			lastPhrase = phrase;

			if(assoconduty) assoconduty = false;
			
			if( parent->doAssociatedPhrase() &&
				cintab->getAssociatedPhrases(phrase, candidateStringVector, parent->doLearnAssociatedPhrase())
				) {
			//Fetch associated phrase... 
#if defined(WIN32) && !defined(WINCE)
			candi.prepare(&candidateStringVector, "!@#$%^&*()", textbar, "1234567890", string("Shift+#\xE3\x80\x80"));
#else
			candi.prepare(&candidateStringVector, "!@#$%^&*()", textbar); 
#endif
			assoconduty = true;
			}

}