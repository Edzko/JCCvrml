// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__EDBEF3BD_E621_11D3_8661_00E098032DB1__INCLUDED_)
#define AFX_STDAFX_H__EDBEF3BD_E621_11D3_8661_00E098032DB1__INCLUDED_

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#define _WIN32_DCOM

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxctl.h>			// AfxConnectionAdvise()


#include "cortona_support.h"
#include "CortonaControl.h"
#include "CortonaDispatch.h"
#include "Shelley.h"

//#import "c:\windows\syswow64\cp211_axucp.dll" no_namespace

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

enum {
	wmUser = WM_USER,
	wmSceneLoaded,
	wmSceneUnloaded,
	wmTrace
};

#endif // !defined(AFX_STDAFX_H__EDBEF3BD_E621_11D3_8661_00E098032DB1__INCLUDED_)

