// OVKPAssociatedPhrases.h: Generic tool for associated phrases
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

#ifndef __OVKPAssociatedPhrase_h
#define __OVKPAssociatedPhrase_h

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <string>
#ifndef WIN32
	#include <OpenVanilla/OpenVanilla.h>
	#include <OpenVanilla/OVLibrary.h>
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OpenVanilla.h"
	#include "OVLibrary.h"
	#include "OVUtility.h"
#endif
using namespace std;

class OVKPAssociatedPhrasePrompterContext : public OVInputMethodContext
{
public:
    virtual void start(OVBuffer*, OVCandidate*, OVService*);
    virtual void clear();
    virtual void end();
	virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*);
};

class OVKPAssociatedPhrasePrompter : public OVInputMethod {
public:
    virtual const char* moduleType() 
	{
		return "OVKeyPreprocessor";
	}
    virtual const char* identifier()
	{
		return "OVKPAssociatedPhrasePrompter";
		
	}
    virtual const char* localizedName(const char* locale)
	{
		if (!stricmp(locale, "zh_TW")) return "\xE8\x81\xAF\xE6\x83\xB3\xE8\xA9\x9E\xE8\xBC\xB8\xE5\x85\xA5"; //聯想詞輸入
		
		return "Associated Phrase Prompter";
		
	}
	
    virtual int initialize(OVDictionary*, OVService*, const char*)
	{
		return 1;
	}
	
    virtual void update(OVDictionary*, OVService*)
	{	
	}
	
    virtual OVInputMethodContext* newContext()
	{
		return new OVKPAssociatedPhrasePrompterContext;
	}
    
protected:
	friend class OVKPAssociatedPhrasePrompterContext;
};

class OVOFAssociatedPhraseReceptor : public OVOutputFilter
{
public:
    virtual int initialize(OVDictionary*, OVService*, const char*);

    virtual const char* identifier()
	{
		return "OVOFAssociatedPhraseReceptor";
	}
	
    virtual const char* localizedName(const char* locale)
	{
		if (!stricmp(locale, "zh_TW")) return "\xE8\x81\xAF\xE6\x83\xB3\xE8\xA9\x9E\xE8\xBC\xB8\xE5\x85\xA5\xE6\x8F\x90\xE7\xA4\xBA"; //聯想詞輸入提示
		
		return "Associated Phrase Receptor";
	}
	
    virtual const char* process(const char*, OVService*);
};

#endif
