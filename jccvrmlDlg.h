// jccvrmlDlg.h : header file
//

#if !defined(AFX_JCCVRMLDLG_H__EDBEF3BB_E621_11D3_8661_00E098032DB1__INCLUDED_)
#define AFX_JCCVRMLDLG_H__EDBEF3BB_E621_11D3_8661_00E098032DB1__INCLUDED_

//#include "EAISampleManager.h"
#include "JCCJoy.h"
#include "resource.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CortonaCtl.h"

#define _NO_TEMPLATE
//#define _NO_NAMESPACE
#include "matrix.h"

#ifndef _NO_NAMESPACE
using namespace std;
using namespace math;
#define STD std
#else
#define STD
#endif
#ifndef _NO_TEMPLATE
typedef matrix<double> Matrix;
#else
typedef matrix Matrix;
#endif;

#define GRAVITATION 9.81
#define HEIGHT 45.0
#define WHEELR 0.5
#define DIMX 2.44
#define DIMY 12.76
#define DIMZ 2.44
#define DT 0.02
#define MASSWAGON 1500
#define MAXFLEX 0.5  // maximum flex in the cable, to limit force
#define PI 3.14159265358979

// Filter constants
#define OVERLAY 0.4
#define MAXDDY 0.02
// Wagon Acceleration Moving Average filter
#define ACCFILT 0.6



/////////////////////////////////////////////////////////////////////////////
// CJccvrmlDlg dialog

extern void JCCDynamics(double dT,int SubSample, float x_in, float y_in, int Buttons[], double offset);
//extern CEAISampleManager m_EAIManager;

class CJccvrmlDlg : public CDialog
{
// Construction
public:
	CJccvrmlDlg(CWnd* pParent = NULL);	// standard constructor
	~CJccvrmlDlg() //standard destructor
	{
		//m_EAIManager.shutdownCosmoPlayer();
	}
	double Time;
// Dialog Data
	//{{AFX_DATA(CJccvrmlDlg)
	enum { IDD = IDD_JCCVRML_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJccvrmlDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CJccvrmlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeCgz();
	afx_msg void OnChangeMass();
	afx_msg void OnReset();
	afx_msg void OnConfig();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	// Pulley locations geometry
	double A12,A23,A14,C12,C23,C14;

	// Skew Wagon Actuator limitations
	double MAXACCEL,MAXVELOC,MAXPOS,MINPOS;  // not used!

	// Trolley Actuator limitations
	double TXACCEL,TXVELOC,TXMIN,TXMAX;

	// Hoisting Actuator limitations
	double CLACCEL,CLVELOC,CLMIN,CLMAX;

	// Cable stiffness and damping
	double KS, DS;

	// Automatic Controllers
	int SKEW_CTRL, SWAY_CTRL, SWING_CTRL;
	int SKEW_INV, SWAY_INV, SWING_INV;

private:
	int Act, OS;
	double TrolleyPos, Wagon, CableL[2];
	int i, j, k;
	double T0, skew, oldskew, olddskew;
	double TX, dTX, WX, dWX, dWXnew, CL[2];
	double Mass, initSwing;
	double WindT, meanL, meanS, Sway, dSway, newSway;
	double dTrolleyPos, dCableL, Cable;
	double Sway2;

	float Q[4];
	char ValChar[20];

	double Skew, dSkew;
	double FddWX;
	double WagonF, ddWXnew, ddWX;
	// for prediction filter
	double Skew0, dSkew0;

	// allocate memory for filter recent measurements
	// assume 3*window (max cable length = 50)
	double BUF[150];
	double CG_Z, ContainerMass;

	UINT IDTimer;
	CJoyStick UInput;
	CSliderCtrl *m_pOffset;
	CWnd m_ControlWrapper;

	CCortonaCtl *m_CortonaCtl;
	I3DViewService2* m_p3DViewService;
	IEngine* m_pEngine;
	interface ICortona* m_pCortona;
	INodeObject* pMaster, *pTrolley, *pWagon1, *pWagon2, *pChase;
	INodeObject* pCable1, *pCable2, *pCable3, *pCable4, *pContainer;
	ISFVec3fObject* m_pTranslation;
	ISFRotationObject* m_pRotation;

	class CCortonaEvents* m_pCortonaEvents;
	DWORD m_dwCoockie;

	UINT m_SelectedRenderer;
	void initDynamics();
	void JCCDynamics(double dT, int SubSample, float x_in, float y_in,
		int Buttons[], double offset);
	double CTRL(double dT, double SkewSensor, double offset,
		int Act, int OS, double WX, double dWX,
		Matrix DL, Matrix I, Matrix EA, Matrix dEA);
	double ControlF(double L, Matrix I,
		double WX, double dWX, double offset);
	void Prediction(double L, Matrix I, double T0, double Skew0, double dSkew0, int Act);
	void Filter(double L, double dT, double SkewSensor);

public:
	HRESULT UCables(float TX, float WX, Matrix Co, float Sway2);
	HRESULT UTrolley(float TX, float WX, float Sway2);
	HRESULT UContainer(float Q[], Matrix X, Matrix CG);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_EVENTSINK_MAP()
	void OnSceneLoadedCortonaControl(BOOL Success);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedJcchelp();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JCCVRMLDLG_H__EDBEF3BB_E621_11D3_8661_00E098032DB1__INCLUDED_)
