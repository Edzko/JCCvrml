#if !defined(AFX_CORTONAEVENTS_H__ED41CD95_A0E6_11D3_B7AB_00A0C94C6AE0__INCLUDED_)
#define AFX_CORTONAEVENTS_H__ED41CD95_A0E6_11D3_B7AB_00A0C94C6AE0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CortonaEvents.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CCortonaEvents command target

class CCortonaEvents : public CCmdTarget
{
	DECLARE_DYNCREATE(CCortonaEvents)

	CCortonaEvents();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCortonaEvents)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCortonaEvents();

	// Generated message map functions
	//{{AFX_MSG(CCortonaEvents)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CCortonaEvents)
        afx_msg void OnClick();
        afx_msg void OnDblClick();
        afx_msg void OnMouseDown(short button, short shift, long x, long y);
        afx_msg void OnMouseMove(short button, short shift, long x, long y);
        afx_msg void OnMouseUp(short button, short shift, long x, long y);
        afx_msg void OnOnAnchor(LPCTSTR Url, LPCTSTR Parameter, LPCTSTR Description, LPCTSTR Name);
        afx_msg void OnOnEventOut(LPDISPATCH Value, LPCTSTR NodeName, LPCTSTR EventOutName, double TimeStamp);
        afx_msg void OnOnMouseEnter();
        afx_msg void OnOnMouseOut();
        afx_msg void OnOnSceneLoaded(BOOL Success);
        afx_msg void OnOnSceneUnloaded();
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CORTONAEVENTS_H__ED41CD95_A0E6_11D3_B7AB_00A0C94C6AE0__INCLUDED_)
