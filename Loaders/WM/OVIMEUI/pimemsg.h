//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:

pimemsg.h

Abstract:

Private ime message header file

Notes:


--*/


#ifndef _PRIV_IMEMSG_
#define _PRIV_IMEMSG_

#ifdef __cplusplus
extern "C"  {
#endif

#define IMN_PRIVATE_SETIMKL_DEFAULT     0xFFFF0001
#define IMN_PRIVATE_SETIMKL_PHON        0xFFFF0002
#define IMN_PRIVATE_SETIMKL_CHAJEI      0xFFFF0003
#define IMN_PRIVATE_SETIMKL_HWX         0xFFFF0004
#define IMN_PRIVATE_SETIMKL_SYMBOL      0xFFFF0005
#define IMN_PRIVATE_SETIMKL_SP          0xFFFF0006

#define IME_ESC_GET_COMPSTR_LEN         0x1010

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* _PRIV_IMEMSG_ */
