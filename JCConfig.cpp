// JCConfig.cpp : implementation file
//

#include "stdafx.h"
#include "jccvrml.h"
#include "jccvrmlDlg.h"
#include "JCConfig.h"
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
#define MAXACCEL ((CJccvrmlDlg*)m_pParentWnd)->MAXACCEL
#define MAXVELOC ((CJccvrmlDlg*)m_pParentWnd)->MAXVELOC
#define MAXPOS ((CJccvrmlDlg*)m_pParentWnd)->MAXPOS
#define MINPOS ((CJccvrmlDlg*)m_pParentWnd)->MINPOS
#define TXACCEL ((CJccvrmlDlg*)m_pParentWnd)->TXACCEL
#define TXVELOC ((CJccvrmlDlg*)m_pParentWnd)->TXVELOC
#define TXMIN ((CJccvrmlDlg*)m_pParentWnd)->TXMIN
#define TXMAX ((CJccvrmlDlg*)m_pParentWnd)->TXMAX
#define CLACCEL ((CJccvrmlDlg*)m_pParentWnd)->CLACCEL
#define CLVELOC ((CJccvrmlDlg*)m_pParentWnd)->CLVELOC
#define CLMIN ((CJccvrmlDlg*)m_pParentWnd)->CLMIN
#define CLMAX ((CJccvrmlDlg*)m_pParentWnd)->CLMAX
#define SKEW_CTRL ((CJccvrmlDlg*)m_pParentWnd)->SKEW_CTRL
#define SWAY_CTRL ((CJccvrmlDlg*)m_pParentWnd)->SWAY_CTRL
#define SWING_CTRL ((CJccvrmlDlg*)m_pParentWnd)->SWING_CTRL
#define SKEW_INV ((CJccvrmlDlg*)m_pParentWnd)->SKEW_INV
#define SWAY_INV ((CJccvrmlDlg*)m_pParentWnd)->SWAY_INV
#define SWING_INV ((CJccvrmlDlg*)m_pParentWnd)->SWING_INV


/////////////////////////////////////////////////////////////////////////////
// CJCConfig dialog

CJCConfig::CJCConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CJCConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJCConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentWnd = pParent;
}

void CJCConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJCConfig)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CJCConfig, CDialog)
	//{{AFX_MSG_MAP(CJCConfig)
	ON_BN_CLICKED(IDC_GEOM, OnGeom)
	ON_EN_CHANGE(IDC_WXACC, OnChangeWxacc)
	ON_EN_CHANGE(IDC_WXVELOC, OnChangeWxveloc)
	ON_EN_CHANGE(IDC_WXMAX, OnChangeWxpos)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_EN_CHANGE(IDC_CLACC, OnChangeClacc)
	ON_EN_CHANGE(IDC_CLVELOC, OnChangeClveloc)
	ON_EN_CHANGE(IDC_CLMAX, OnChangeClmax)
	ON_EN_CHANGE(IDC_CLMIN, OnChangeClmin)
	ON_EN_CHANGE(IDC_TXACC, OnChangeTxacc)
	ON_EN_CHANGE(IDC_TXVELOC, OnChangeTxveloc)
	ON_EN_CHANGE(IDC_TXMAX, OnChangeTxmax)
	ON_EN_CHANGE(IDC_TXMIN, OnChangeTxmin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJCConfig message handlers

BOOL CJCConfig::OnInitDialog() 
{
	char buf[20];
	CDialog::OnInitDialog();
	ShowWindow(SW_SHOW);
	double val = TXACCEL;
	sprintf(buf,"%-5.2f",TXACCEL);SetDlgItemText(IDC_TXACC,buf);
	sprintf(buf,"%-5.2f",TXVELOC);SetDlgItemText(IDC_TXVELOC,buf);
	sprintf(buf,"%-5.2f",TXMIN);SetDlgItemText(IDC_TXMIN,buf);
	sprintf(buf,"%-5.2f",TXMAX);SetDlgItemText(IDC_TXMAX,buf);

	sprintf(buf,"%-5.2f",CLACCEL);SetDlgItemText(IDC_CLACC,buf);
	sprintf(buf,"%-5.2f",CLVELOC);SetDlgItemText(IDC_CLVELOC,buf);
	sprintf(buf,"%-5.2f",CLMIN);SetDlgItemText(IDC_CLMIN,buf);
	sprintf(buf,"%-5.2f",CLMAX);SetDlgItemText(IDC_CLMAX,buf);

	sprintf(buf,"%-5.2f",MAXACCEL);SetDlgItemText(IDC_WXACC,buf);
	sprintf(buf,"%-5.2f",MAXVELOC);SetDlgItemText(IDC_WXVELOC,buf);
	sprintf(buf,"%-5.2f",MAXPOS);SetDlgItemText(IDC_WXMAX,buf);
	sprintf(buf,"%-5.2f",MINPOS);SetDlgItemText(IDC_WXMIN,buf);

	m_pSkewCtrl = (CComboBox*)GetDlgItem(IDC_SKEWCTRL);
	m_pSkewCtrl->SetCurSel(SKEW_CTRL);
	m_pSwayCtrl = (CComboBox*)GetDlgItem(IDC_SWAYCTRL);
	m_pSwayCtrl->SetCurSel(SWAY_CTRL);
	m_pSwingCtrl = (CComboBox*)GetDlgItem(IDC_SWINGCTRL);
	m_pSwingCtrl->SetCurSel(SWING_CTRL);
	m_pSkewInv = (CButton*)GetDlgItem(IDC_SKEWINV);
	m_pSkewInv->SetCheck(SKEW_INV);
	m_pSwayInv = (CButton*)GetDlgItem(IDC_SWAYINV);
	m_pSwayInv->SetCheck(SWAY_INV);
	m_pSwingInv = (CButton*)GetDlgItem(IDC_SWINGINV);
	m_pSwingInv->SetCheck(SWING_INV);

	DoStore = 0;
	return TRUE;
}

CJCConfig::~CJCConfig() 
{
	CWinApp *WinApp = AfxGetApp();
	if (DoStore) {
		// Store skew drive parameters
		AfxGetApp()->WriteProfileInt("Drives","WXACCEL",(int)(10.0*MAXACCEL));	
		AfxGetApp()->WriteProfileInt("Drives","WXVELOC",(int)(10.0*MAXVELOC));	
		AfxGetApp()->WriteProfileInt("Drives","WXMAX",(int)(10.0*MAXPOS));	
		AfxGetApp()->WriteProfileInt("Drives","WXMIN",(int)(10.0*MAXPOS));	
		// Store Trolley Drive parameters
		AfxGetApp()->WriteProfileInt("Drives","TXACCEL",(int)(10.0*TXACCEL));	
		AfxGetApp()->WriteProfileInt("Drives","TXVELOC",(int)(10.0*TXVELOC));	
		AfxGetApp()->WriteProfileInt("Drives","TXMAX",(int)(10.0*TXMAX));	
		AfxGetApp()->WriteProfileInt("Drives","TXMIN",(int)(10.0*TXMIN));	
		// Store Hoist Drive parameters
		AfxGetApp()->WriteProfileInt("Drives","CLACCEL",(int)(10.0*CLACCEL));	
		AfxGetApp()->WriteProfileInt("Drives","CLVELOC",(int)(10.0*CLVELOC));	
		AfxGetApp()->WriteProfileInt("Drives","CLMAX",(int)(10.0*CLMAX));	
		AfxGetApp()->WriteProfileInt("Drives","CLMIN",(int)(10.0*CLMIN));	
		// Store Cabling Geometry parameters
		WinApp->WriteProfileInt("Cabling","A12",(int)(A12*1000.0));
		WinApp->WriteProfileInt("Cabling","A23",(int)(A23*1000.0));
		WinApp->WriteProfileInt("Cabling","A14",(int)(A14*1000.0));
		WinApp->WriteProfileInt("Cabling","C12",(int)(C12*1000.0));
		WinApp->WriteProfileInt("Cabling","C23",(int)(C23*1000.0));
		WinApp->WriteProfileInt("Cabling","C14",(int)(C14*1000.0));
		// Store Controllers
		WinApp->WriteProfileInt("Controllers","Skew",SKEW_CTRL);
		WinApp->WriteProfileInt("Controllers","Sway",SWAY_CTRL);
		WinApp->WriteProfileInt("Controllers","Swing",SWING_CTRL);
		WinApp->WriteProfileInt("Controllers","Skewmode",SKEW_INV);
		WinApp->WriteProfileInt("Controllers","Swaymode",SWAY_INV);
		WinApp->WriteProfileInt("Controllers","Swingmode",SWING_INV);
		// Store Cable Dynamic Characteristics
		AfxGetApp()->WriteProfileInt("Cabling","KS",(int)(KS));
		AfxGetApp()->WriteProfileInt("Cabling","DS",(int)(DS));
	}
	WinApp->GetMainWnd()->RedrawWindow();
}

void CJCConfig::OnGeom() 
{
	CCableGeom jccgeom(m_pParentWnd);
	jccgeom.DoModal();
}


void CJCConfig::OnChangeWxacc()   {GetDlgItemText(IDC_WXACC,val);  MAXACCEL = atof(val.GetBuffer(20));}
void CJCConfig::OnChangeWxveloc() {GetDlgItemText(IDC_WXVELOC,val);MAXVELOC = atof(val.GetBuffer(20));}
void CJCConfig::OnChangeWxpos()   {GetDlgItemText(IDC_WXMAX,val);  MAXPOS = atof(val.GetBuffer(20));  }
void CJCConfig::OnChangeClacc()   {GetDlgItemText(IDC_CLACC,val);  CLACCEL = atof(val.GetBuffer(20)); }
void CJCConfig::OnChangeClveloc() {GetDlgItemText(IDC_CLVELOC,val);CLVELOC = atof(val.GetBuffer(20)); }
void CJCConfig::OnChangeClmax()   {GetDlgItemText(IDC_CLMAX,val);  CLMAX = atof(val.GetBuffer(20));   }
void CJCConfig::OnChangeClmin()   {GetDlgItemText(IDC_CLMIN,val);  CLMIN = atof(val.GetBuffer(20));   }
void CJCConfig::OnChangeTxacc()   {GetDlgItemText(IDC_TXACC,val);  TXACCEL = atof(val.GetBuffer(20)); }
void CJCConfig::OnChangeTxveloc() {GetDlgItemText(IDC_TXVELOC,val);TXVELOC = atof(val.GetBuffer(20)); }
void CJCConfig::OnChangeTxmax()   {GetDlgItemText(IDC_TXMAX,val);  TXMAX = atof(val.GetBuffer(20));   }
void CJCConfig::OnChangeTxmin()   {GetDlgItemText(IDC_TXMIN,val);  TXMIN = atof(val.GetBuffer(20));   }

void CJCConfig::OnSave() 
{
	CButton *m_pStore;
	m_pStore = (CButton*)GetDlgItem(IDC_SAVE);
	DoStore = m_pStore->GetCheck();
}

void CJCConfig::OnOK() 
{
	// Store Controllers
	SWAY_CTRL  = m_pSwayCtrl->GetCurSel();
	SKEW_CTRL  = m_pSkewCtrl->GetCurSel();
	SWING_CTRL = m_pSwingCtrl->GetCurSel();
	SWAY_INV  = m_pSwayInv->GetCheck();
	SKEW_INV  = m_pSkewInv->GetCheck();
	SWING_INV = m_pSwingInv->GetCheck();
	
	CDialog::OnOK();
}
