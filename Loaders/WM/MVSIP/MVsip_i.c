/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun May 24 03:30:49 2009
 */
/* Compiler settings for .\MVsip.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID LIBID_MVSIPLib = {0x8CF074A6,0xA7A0,0x4666,{0xAA,0x49,0xAA,0xFA,0x23,0xD7,0xD2,0xCD}};


const IID IID_IIMCallback = {0x42429669,0xae04,0x11d0,{0xa4,0xf8,0x00,0xaa,0x00,0xa7,0x49,0xb9}};


const IID IID_IIMCallbackEx = {0x9DDB3920,0x3606,0x11d2,{0xA2,0xEB,0x00,0x00,0xF8,0x75,0x72,0x70}};


const IID IID_IInputMethod = {0x42429666,0xae04,0x11d0,{0xa4,0xf8,0x00,0xaa,0x00,0xa7,0x49,0xb9}};


const IID IID_IInputMethodEx = {0x9ddb3921,0x3606,0x11d2,{0xa2,0xeb,0x00,0x00,0xf8,0x75,0x72,0x70}};


const CLSID CLSID_CMSMVIm = {0xe80d7526,0x58df,0x4142,{0xa9,0x58,0x72,0x49,0x0a,0x69,0x1b,0x46}};


#ifdef __cplusplus
}
#endif

