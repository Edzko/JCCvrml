// jccvrml.h : main header file for the JCCVRML application
//

#if !defined(AFX_JCCVRML_H__EDBEF3B9_E621_11D3_8661_00E098032DB1__INCLUDED_)
#define AFX_JCCVRML_H__EDBEF3B9_E621_11D3_8661_00E098032DB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CJccvrmlApp:
// See jccvrml.cpp for the implementation of this class
//

class CJccvrmlApp : public CWinApp
{
public:
	CJccvrmlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJccvrmlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CJccvrmlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JCCVRML_H__EDBEF3B9_E621_11D3_8661_00E098032DB1__INCLUDED_)
