//#define OV_DEBUG
// OVUtility.h: Inline utilities
//
// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
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

#ifndef __OVUtility_h
#define __OVUtility_h

#include <ctime>

//<comment author='b6s'>
//The following three headers are for WIN32 specific debug log.
#if (defined(WIN32)||defined(WINCE))&& defined(OV_DEBUG) 
#include <windows.h>
#ifndef WINCE
#include <shlobj.h>
#include <io.h>
#endif
#endif
//</comment>

using namespace std;

class Watch
{
public:
	void start()
	{
		tick_ = 0L;
		sec_ = 0.0;
#ifdef OV_DEBUG
#ifndef WINCE
		from_ = clock();
#else
		from_=GetTickCount();
#endif
#else
		from_ = 0L;
#endif
	}

	void stop()
	{
#ifdef OV_DEBUG
#ifndef WINCE
		tick_ = clock() - from_;
		sec_ = static_cast<double>(tick_) / CLOCKS_PER_SEC;
#else
		tick_ = GetTickCount() - from_;
		sec_ = static_cast<double>(tick_) / CLOCKS_PER_SEC;

#endif
#endif
	}

	clock_t getTick() { return tick_; }
	double getSec() { return sec_; }

private:
	clock_t from_;
	clock_t tick_;
	double sec_;
};



// debug routine
#ifdef OV_DEBUG 
    #include <stdarg.h>
    #include <stdio.h>

    // Thanks to modern C++ compiler, we can write this in the header file. 
    // No extra compiling/library making/linking is even needed!

    inline static void murmur(const char* format,...) {
#if defined(WIN32)||defined(WINCE)
	
		//@warning OV-specific paths here!
		//<comment author='b6s'>
		//The following codes of paths are redundant to AVConfig.cpp,
		//however, they're necessary...		
		char OV_BASEDIR_PRE[MAX_PATH];
		char OV_USERDIR_PRE[MAX_PATH];

#ifndef WINCE
		//GetWindowsDirectoryA(OV_BASEDIR_PRE, MAX_PATH - 14);
		//sprintf(OV_BASEDIR_PRE, "%s\\%s", OV_BASEDIR_PRE, "OpenVanilla\\");
		strcpy(OV_BASEDIR_PRE, "C:\\OpenVanilla\\");

		if (SHGetFolderPathA(
				NULL,
				CSIDL_APPDATA|CSIDL_FLAG_CREATE,
				NULL,
				0,
				OV_USERDIR_PRE) >= 0) {
			sprintf(OV_USERDIR_PRE, "%s\\%s", OV_USERDIR_PRE, "OpenVanilla\\");
			if (_access (OV_USERDIR_PRE, 0))
				CreateDirectoryA(OV_USERDIR_PRE, NULL);
		} else
			sprintf(OV_USERDIR_PRE, "%s\\%s", OV_BASEDIR_PRE, "User\\");
#else	//WINCE
		//WCHAR wbuf[MAX_PATH];
		//GetModuleFileNameW(GetModuleHandleW(L"OVIME.dll"), wbuf, MAX_PATH);
		//GetModuleFileNameW(NULL, wbuf, MAX_PATH);
		//wcstombs(OV_BASEDIR_PRE,wbuf, MAX_PATH);
		//WideCharToMultiByte(CP_UTF8, 0, wbuf, (int)(wcslen(wbuf)+1), OV_BASEDIR_PRE, MAX_PATH,NULL, NULL);
		//char* lb= strrchr(OV_BASEDIR_PRE,'\\');
		//OV_BASEDIR_PRE[((DWORD)lb-(DWORD)OV_BASEDIR_PRE)/sizeof(char) +1]=0; //stripped the filename
		strcpy(OV_BASEDIR_PRE,"\\Program files\\Mobilevanilla\\");
		strcpy(OV_USERDIR_PRE, OV_BASEDIR_PRE);
		
#endif

		char OV_WIN32_DEBUG_LOG_PATH[MAX_PATH];
		sprintf(OV_WIN32_DEBUG_LOG_PATH, "%s\\%s", OV_USERDIR_PRE, "orz.txt");
		//</comment>
		
		FILE *fp;
		if (fp=fopen(OV_WIN32_DEBUG_LOG_PATH, "a")) {
			va_list args;
			va_start (args, format);
			vfprintf (fp, format, args);
#if defined(WINCE) //|| defined(WIN32)
			WCHAR wbuf[256];
			char buf[256];
			sprintf(buf, format, args);
			MultiByteToWideChar(CP_UTF8, 0, buf, (int)strlen(buf)+1, wbuf, 256);
			OutputDebugStringW(wbuf);
#endif
			va_end (args);
			fprintf (fp, "\n");
			fclose(fp);
		}
#else
		va_list args;
		va_start (args, format);
		vfprintf (stderr, format, args);
		va_end (args);
		fprintf (stderr, "\n");
#endif
    }
#else
    inline static void murmur(const char* format,...) {}
#endif

#endif
