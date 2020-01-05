#if !defined(AFX_JCCONFIG_H__881387E4_2CAE_11D4_8662_00E098032DB1__INCLUDED_)
#define AFX_JCCONFIG_H__881387E4_2CAE_11D4_8662_00E098032DB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JCConfig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJCConfig dialog

class CJCConfig : public CDialog
{
// Construction
public:
	CComboBox *m_pSkewCtrl, *m_pSwayCtrl, *m_pSwingCtrl;
	CButton *m_pSkewInv, *m_pSwayInv, *m_pSwingInv;
	int DoStore;

	CJCConfig(CWnd* pParent = NULL);   // standard constructor
	~CJCConfig();
	CString val;

// Dialog Data
	//{{AFX_DATA(CJCConfig)
	enum { IDD = IDD_CONFIG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJCConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJCConfig)
	virtual BOOL OnInitDialog();
	afx_msg void OnGeom();
	afx_msg void OnChangeWxacc();
	afx_msg void OnChangeWxveloc();
	afx_msg void OnChangeWxpos();
	afx_msg void OnSave();
	afx_msg void OnChangeClacc();
	afx_msg void OnChangeClveloc();
	afx_msg void OnChangeClmax();
	afx_msg void OnChangeClmin();
	afx_msg void OnChangeTxacc();
	afx_msg void OnChangeTxveloc();
	afx_msg void OnChangeTxmax();
	afx_msg void OnChangeTxmin();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JCCONFIG_H__881387E4_2CAE_11D4_8662_00E098032DB1__INCLUDED_)
