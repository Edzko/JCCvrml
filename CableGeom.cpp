// CableGeom.cpp : implementation file
//

#include "stdafx.h"
#include "jccvrml.h"
#include "jccvrmlDlg.h"
#include "CableGeom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define A12 ((CJccvrmlDlg*)m_pParentWnd)->A12
#define A23 ((CJccvrmlDlg*)m_pParentWnd)->A23
#define A14 ((CJccvrmlDlg*)m_pParentWnd)->A14
#define C12 ((CJccvrmlDlg*)m_pParentWnd)->C12
#define C23 ((CJccvrmlDlg*)m_pParentWnd)->C23
#define C14 ((CJccvrmlDlg*)m_pParentWnd)->C14
#define KS ((CJccvrmlDlg*)m_pParentWnd)->KS
#define DS ((CJccvrmlDlg*)m_pParentWnd)->DS

/////////////////////////////////////////////////////////////////////////////
// CCableGeom dialog

CCableGeom::CCableGeom(CWnd* pParent /*=NULL*/)
	: CDialog(CCableGeom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCableGeom)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

void CCableGeom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCableGeom)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCableGeom, CDialog)
	//{{AFX_MSG_MAP(CCableGeom)
	ON_EN_CHANGE(IDC_KS, OnChangeKs)
	ON_EN_CHANGE(IDC_KD, OnChangeKd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCableGeom message handlers

BOOL CCableGeom::OnInitDialog() 
{
	char buf[100];
	CDialog::OnInitDialog();
	ShowWindow(SW_SHOW);
	
	SetDlgItemText(IDC_A12,_gcvt(A12,3,buf));
	SetDlgItemText(IDC_A23,_gcvt(A23,3,buf));
	SetDlgItemText(IDC_A14,_gcvt(A14,3,buf));
	SetDlgItemText(IDC_C12,_gcvt(C12,3,buf));
	SetDlgItemText(IDC_C23,_gcvt(C23,3,buf));
	SetDlgItemText(IDC_C14,_gcvt(C14,3,buf));

	SetDlgItemText(IDC_KS,_gcvt(KS,10,buf));
	SetDlgItemText(IDC_KD,_gcvt(DS,10,buf));

	return TRUE;
}

void CCableGeom::OnChangeKs() 
{
	char val[100];
	GetDlgItemText(IDC_KS,val,20);
	KS = atof(val);
}

void CCableGeom::OnChangeKd() 
{
	char val[100];
	GetDlgItemText(IDC_KD,val,20);
	DS = atof(val);
}
