// jccvrml.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "jccvrml.h"
#include "jccvrmlDlg.h"

#define PASSWD "ECT-Q4849-TEC"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern CJccvrmlDlg *m_pDlg;


/////////////////////////////////////////////////////////////////////////////
// CJccvrmlApp

BEGIN_MESSAGE_MAP(CJccvrmlApp, CWinApp)
	//{{AFX_MSG_MAP(CJccvrmlApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJccvrmlApp construction

CJccvrmlApp::CJccvrmlApp(){}

/////////////////////////////////////////////////////////////////////////////
// The one and only CJccvrmlApp object

CJccvrmlApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CJccvrmlApp initialization

char Password[20];

BOOL CALLBACK PasswdDialogProc(
  HWND hwndDlg,  // handle to dialog box
  UINT uMsg,     // message
  WPARAM wParam, // first message parameter
  LPARAM lParam  // second message parameter
)
{
	CEdit *m_pPasswd;
	switch (uMsg) {
	case WM_INITDIALOG : 
		TRACE("Initialize\n");
		return TRUE;
	case WM_COMMAND : 
		TRACE("Control\n");
		switch (LOWORD(wParam)) {
		case IDOK : 
			DestroyWindow(hwndDlg);
			TRACE("OK\n");
			break;
		case IDC_PASSWD : 
			m_pPasswd = (CEdit*)GetDlgItem(hwndDlg,IDC_PASSWD);
			GetDlgItemText(hwndDlg,IDC_PASSWD,Password,20);
			break;
		case IDCANCEL : 
			strcat(Password,"");
			DestroyWindow(hwndDlg);
			TRACE("Cancel\n");
			break;
		}
		return TRUE;
	default : return FALSE;
	}
}
 
BOOL CJccvrmlApp::InitInstance()
{
	CString pw, val;
	CWinApp *WinApp = AfxGetApp();
	CTime ToDay = CTime::GetCurrentTime();
	CTime Expires(2020,12,31,0,0,0); 

	AfxEnableControlContainer();

	SetRegistryKey("TEC");

	//TRACE("%s - %s\n",m_pszProfileName,m_pszRegistryKey);
	pw = Expires.Format("%B %d, %Y");
	pw = GetProfileString("","Code");
	if (ToDay>Expires) return FALSE;
	if (pw.Compare(PASSWD)!=0) {
		DialogBox(AfxGetInstanceHandle(),
			MAKEINTRESOURCE(IDD_PASSWD),
			NULL,&PasswdDialogProc); //AfxGetMainWnd()->m_hWnd
		pw = Password;
		if (pw.Compare(PASSWD)!=0) return FALSE;
		WriteProfileString("","Code",PASSWD);
		WriteProfileString("","Date",ToDay.Format("%B %d, %Y"));
	}
	//free((void*)m_pszRegistryKey);  // to save to win.ini
	
	CJccvrmlDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

	return FALSE;
}
