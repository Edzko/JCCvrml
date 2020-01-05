#include "stdafx.h"
#include <math.h>

#include "jccvrml.h"
#include "jccvrmlDlg.h"
#include "jcconfig.h"

#include "CortonaEvents.h"

#define DT 0.02
#define SUBSAMPLE 2
#define PI 3.14159265358979

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "model.h"
/////////////////////////////////////////////////////////////////////////////
// CJccvrmlDlg dialog

CJccvrmlDlg *m_pDlg;
extern double CG_Z, ContainerMass;

static Matrix S(1, 4), oldS(1, 4), dS(1, 4);
static Matrix RA(3, 3), Rf(3, 3);
static Matrix I(3, 1), II(3, 1), vec3(3, 1);
static Matrix CableF(3, 4), CableT(3, 4), ConF(3, 1);
static Matrix EA(3, 1), dEA(3, 1), dEAnew(3, 1), O(3, 1), dO(3, 1), dOnew(3, 1), Onew(3, 1);
static Matrix ddX(3, 1), ddXnew(3, 1), dX(3, 1), dXnew(3, 1), X(3, 1);
static Matrix A(3, 4), C(3, 4), Co(3, 4), Cc(3, 4), D(3, 4), DL(1, 4), CG(3, 1);

CJccvrmlDlg::CJccvrmlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJccvrmlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJccvrmlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJccvrmlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJccvrmlDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	//DDX_Control(pDX, IDC_COSMOPLAYER, m_pCortona);
}

BEGIN_MESSAGE_MAP(CJccvrmlDlg, CDialog)
	//{{AFX_MSG_MAP(CJccvrmlDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_CGZ, OnChangeCgz)
	ON_EN_CHANGE(IDC_MASS, OnChangeMass)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_BN_CLICKED(IDC_CONFIG, OnConfig)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_JCCHELP, &CJccvrmlDlg::OnBnClickedJcchelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJccvrmlDlg message handlers

//HWND JCCDlg;

BOOL CJccvrmlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_CortonaCtl = (CCortonaCtl*)GetDlgItem(ID_CORTONA_CONTROL);
	m_CortonaCtl->GetControlUnknown()->QueryInterface(IID_ICortona, reinterpret_cast<void**>(&m_pCortona));

	m_pCortonaEvents = new CCortonaEvents;
	AfxConnectionAdvise(m_CortonaCtl->GetControlUnknown(), DIID__CortonaEvents,
		m_pCortonaEvents->GetIDispatch(FALSE), FALSE, &m_dwCoockie);
	//CString fname = "c:\\projects\\vrml\\jccvrml\\jcc.wrl";
	//BSTR ctname = fname.AllocSysString();
	//m_pCortona->put_Scene(ctname);
	//SysFreeString(ctname);
	m_CortonaCtl->SetScene("c:\\projects\\vrml\\jccvrml\\jcc.wrl");
	m_CortonaCtl->SetHeadLight(TRUE);
	
	m_CortonaCtl->SetNavigationBar(1);
	//m_CortonaCtl->Edit();
	
	IDispatch* pdisp = m_CortonaCtl->GetEngine();
	if (pdisp != NULL) {
		if (FAILED(pdisp->QueryInterface(IID_IEngine, reinterpret_cast<void**>(&m_pEngine))))
			m_pEngine = NULL;
		IServiceProvider* pServiceProvider;
		if (SUCCEEDED(m_pEngine->QueryInterface(IID_IServiceProvider,
			reinterpret_cast<void**>(&pServiceProvider))))
		{
			pServiceProvider->QueryService(IID_I3DViewService2, IID_I3DViewService2,
				reinterpret_cast<void**>(&m_p3DViewService));
			pServiceProvider->Release();
		}
		pdisp->Release();
	}
	
	A12 = 1.3;
	A23 = 5.78;
	A14 = 5.38;
	C12 = 0.8;
	C23 = 5.55;
	C14 = 5.35;
	CG_Z = 0;
	ContainerMass = 25000;


	// Load Parameters
	MAXACCEL = 0.1*AfxGetApp()->GetProfileInt("Drives","WXACCEL", 15);
	MAXVELOC = 0.1*AfxGetApp()->GetProfileInt("Drives","WXVELOC", 5);
	MAXPOS = 0.1*AfxGetApp()->GetProfileInt("Drives","WXMAX", 5);
	MINPOS = 0.1*(int)AfxGetApp()->GetProfileInt("Drives","WXMIN", -5);
	TXACCEL = 0.1*AfxGetApp()->GetProfileInt("Drives","TXACCEL", 20);
	TXVELOC = 0.1*AfxGetApp()->GetProfileInt("Drives","TXVELOC", 40);
	TXMAX = 0.1*AfxGetApp()->GetProfileInt("Drives","TXMAX", 550);
	TXMIN = 0.1*(int)AfxGetApp()->GetProfileInt("Drives","TXMIN", -550);
	CLACCEL = 0.1*AfxGetApp()->GetProfileInt("Drives","CLACCEL", 20);
	CLVELOC = 0.1*AfxGetApp()->GetProfileInt("Drives","CLVELOC", 20);
	CLMAX = 0.1*AfxGetApp()->GetProfileInt("Drives","CLMAX", 550);
	CLMIN = 0.1*AfxGetApp()->GetProfileInt("Drives","CLMIN", 60);

	// Cabling Geometry
	A12 = 0.001*AfxGetApp()->GetProfileInt("Cabling","A12", 1300);
	A23 = 0.001*AfxGetApp()->GetProfileInt("Cabling","A23", 5780);
	A14 = 0.001*AfxGetApp()->GetProfileInt("Cabling","A14", 5380);
	C12 = 0.001*AfxGetApp()->GetProfileInt("Cabling","C12", 800);
	C23 = 0.001*AfxGetApp()->GetProfileInt("Cabling","C23", 5550);
	C14 = 0.001*AfxGetApp()->GetProfileInt("Cabling","C14", 5350);
	// Cable Dynamics
	KS = (double)AfxGetApp()->GetProfileInt("Cabling","KS", 20000000);
	DS = (double)AfxGetApp()->GetProfileInt("Cabling","DS", 2000000);
	// Controllers
	SKEW_CTRL = AfxGetApp()->GetProfileInt("Controllers","Skew", 0);
	SWAY_CTRL = AfxGetApp()->GetProfileInt("Controllers","Sway", 0);
	SWING_CTRL = AfxGetApp()->GetProfileInt("Controllers","Swing", 0);
	SKEW_INV = AfxGetApp()->GetProfileInt("Controllers","Skewmode", 0);
	SWAY_INV = AfxGetApp()->GetProfileInt("Controllers","Swaymode", 0);
	SWING_INV = AfxGetApp()->GetProfileInt("Controllers","Swingmode", 0);
	

	SetDlgItemText(IDC_CGZ,"0");
	SetDlgItemText(IDC_MASS,"25000");
	m_pOffset = (CSliderCtrl*)GetDlgItem(IDC_OFFSET);
	m_pOffset->SetRange(-10,10,TRUE);
	m_pOffset->SetTicFreq(1);
	m_pOffset->SetLineSize(1);
	Time = 0.0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CJccvrmlDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CJccvrmlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CJccvrmlDlg::OnTimer(UINT nIDEvent) 
{
	char Tc[10];
	double offset;

	// Update Dynamics, subsample
	if (UInput.mResult!=JOYERR_NOERROR) {
		UInput.x=0;
		UInput.y=0;
		UInput.z=0;
	} else {
		UInput.GetJoyInputs();
		//TRACE2("Joystick inputs : %f %f\n",UInput.x,UInput.y);
		if (floor(UInput.z*11.0)!=0.0) m_pOffset->SetPos((int)(UInput.z*11.0));
	}
	offset = (double) m_pOffset->GetPos()/180*PI;
	//TRACE1("Offset : %f\n",offset);
	JCCDynamics(DT,SUBSAMPLE,(float)(UInput.x/6.0),(float)(UInput.y/3.0),UInput.Buttons,offset);

	Time += DT*SUBSAMPLE;

	sprintf(Tc,"%6.2f",Time);
	SetDlgItemText(IDC_POS_TROLLEY, Tc);
	
	// Set the Animation Window
	CDialog::OnTimer(nIDEvent);

	//p_CortonaCtl->
	//m_pCortona->
}

void CJccvrmlDlg::OnChangeCgz() 
{
	CString CharStr;
	GetDlgItemText(IDC_CGZ,CharStr);
	CG_Z = atof(CharStr);
}

void CJccvrmlDlg::OnChangeMass() 
{
	CString CharStr;
	GetDlgItemText(IDC_MASS,CharStr);
	ContainerMass = atof(CharStr);
}

void CJccvrmlDlg::OnReset() 
{
	initDynamics();
}

void CJccvrmlDlg::OnConfig() 
{
	CJCConfig jcccnf(this);
	jcccnf.DoModal();
	ShowWindow(SW_SHOW);
}

void CJccvrmlDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (Time>1.0) 
	{
		RECT Rect;
		CDialog::GetWindowRect(&Rect);
		((CWnd *)GetDlgItem(IDC_COSMOPLAYER))->SetWindowPos(&wndTop,
			120,10,Rect.right-Rect.left-140,Rect.bottom-Rect.top-55,SWP_SHOWWINDOW);
	}
}

HRESULT CJccvrmlDlg::UContainer(float Q[], Matrix X, Matrix CG)
{
	interface IFieldObject *fld;
	BSTR ftype = SysAllocString(L"SFRotation");
	m_pEngine->CreateField(ftype, &fld);
	SysFreeString(ftype);

	IFieldsCollection* pTransformFields;
	HRESULT hr = pContainer->get_Fields(&pTransformFields);

	if (SUCCEEDED(hr)) {

		// Getting translation field by name
		IFieldObject* pTranslationField, *pRotationField;

		VARIANT varName;
		VariantInit(&varName);
		varName.vt = VT_BSTR;

		varName.bstrVal = SysAllocString(L"rotation");
		hr = pTransformFields->get_Item(&varName, &pRotationField);
		SysFreeString(varName.bstrVal);
		if (SUCCEEDED(hr)) {

			// Get ISFRotationObject from IFieldObject
			hr = fld->QueryInterface(IID_ISFRotationObject, (void**)&m_pRotation);
			//hr = pRotationField->QueryInterface(IID_ISFRotationObject, (void**)&m_pRotation);
			if (SUCCEEDED(hr)) {
				m_pRotation->put_X(Q[0]);
				m_pRotation->put_Y(Q[1]);
				m_pRotation->put_Z(Q[2]);
				m_pRotation->put_Angle(Q[3]);
			}
			pRotationField->Assign(fld);
			pRotationField->Release();
		}
		
		varName.bstrVal = SysAllocString(L"translation");
		hr = pTransformFields->get_Item(&varName, &pTranslationField);
		if (SUCCEEDED(hr)) {

			// Get ISFVec3fObject from IFieldObject
			hr = pTranslationField->QueryInterface(IID_ISFVec3fObject, (void**)&m_pTranslation);
			if (SUCCEEDED(hr)) {
				m_pTranslation->put_X((float)(X(0, 0) + CG(0, 0)));
				m_pTranslation->put_Y((float)(X(1, 0) + CG(1, 0)));
				m_pTranslation->put_Z((float)(X(2, 0) + CG(2, 0)));
				m_pTranslation->Release();
			}
			pTranslationField->Release();
		}
		pTransformFields->Release();
		VariantClear(&varName);
	}
	return SUCCEEDED(hr);
}

HRESULT CJccvrmlDlg::UTrolley(float TX, float WX, float Sway2)
{
	_POS pos;
	IFieldsCollection* pTransformFields;
	HRESULT hr = pTrolley->get_Fields(&pTransformFields);

	if (SUCCEEDED(hr)) {
		// Getting translation field by name
		IFieldObject* pTranslationField;

		VARIANT varName;
		VariantInit(&varName);
		varName.vt = VT_BSTR;

		varName.bstrVal = SysAllocString(L"translation");
		hr = pTransformFields->get_Item(&varName, &pTranslationField);
		SysFreeString(varName.bstrVal);
		if (SUCCEEDED(hr)) {
			// Get ISFVec3fObject from IFieldObject
			hr = pTranslationField->QueryInterface(IID_ISFVec3fObject, (void**)&m_pTranslation);
			if (SUCCEEDED(hr)) {
				m_pTranslation->put_X(TX);
				m_pTranslation->put_Y(0);
				m_pTranslation->put_Z(0);
				m_pTranslation->Release();
			}
			pTranslationField->Release();
		}
		pTransformFields->Release();
		VariantClear(&varName);
	}

	/*
	hr = pMaster->get_Fields(&pTransformFields);

	if (SUCCEEDED(hr)) {
		// Getting translation field by name
		IFieldObject* pTranslationField;

		VARIANT varName;
		VariantInit(&varName);
		varName.vt = VT_BSTR;

		varName.bstrVal = SysAllocString(L"translation");
		hr = pTransformFields->get_Item(&varName, &pTranslationField);
		SysFreeString(varName.bstrVal);
		if (SUCCEEDED(hr)) {
			// Get ISFVec3fObject from IFieldObject
			hr = pTranslationField->QueryInterface(IID_ISFVec3fObject, (void**)&m_pTranslation);
			if (SUCCEEDED(hr)) {
				m_pTranslation->put_X(TX);
				m_pTranslation->put_Y(0);
				m_pTranslation->put_Z(100);
				m_pTranslation->Release();
			}
			pTranslationField->Release();
		}
		pTransformFields->Release();
		VariantClear(&varName);
	}
	*/

	LPNODEOBJECT pVP;
	hr = m_pEngine->get_Viewpoint(&pVP);
	BSTR pVPname;
	hr = pVP->get_Name(&pVPname);

	/*
	LPFIELDSCOLLECTION pVPFields;
	hr = pVP->get_Fields(&pVPFields);
	VARIANT varName;
	VariantInit(&varName);
	varName.vt = VT_BSTR;
	IFieldObject* pDescriptionField;
	varName.bstrVal = SysAllocString(L"description");
	hr = pVPFields->get_Item(&varName, &pDescriptionField);
	SysFreeString(varName.bstrVal);
	if (SUCCEEDED(hr)) {
		// Get ISFVec3fObject from IFieldObject
		ISFStringObject *m_pDescription;
		hr = pDescriptionField->QueryInterface(IID_ISFStringObject, (void**)&m_pDescription);
		if (SUCCEEDED(hr)) {
			m_pDescription->get_Value(&pVPname);
		}
	}
	*/
	if (wcscmp(pVPname, L"Master") == 0)
	{
		if (m_p3DViewService != NULL && SUCCEEDED(m_p3DViewService->GetCameraPos(&pos)))
		{
			CString str;
			str.Format("ortx: %.4f %.4f %.4f\norty: %.4f %.4f %.4f\nortz: %.4f %.4f %.4f\norig: %.4f %.4f %.4f\r\n",
				pos.ortx.x, pos.ortx.y, pos.ortx.z,
				pos.orty.x, pos.orty.y, pos.orty.z,
				pos.ortz.x, pos.ortz.y, pos.ortz.z,
				pos.orig.x, pos.orig.z, pos.orig.z);

			TRACE2("Camera(%s): %s", pVPname, str);
		}
		if (m_p3DViewService != NULL && SUCCEEDED(m_p3DViewService->GetCameraPos(&pos)))
		{
			pos.orig.x = 0.8*TX;
			pos.orig.y = 0;
			pos.orig.z = 100;
			VERIFY(SUCCEEDED(m_p3DViewService->SetCameraPos(&pos)));
		}
	}

	if (wcscmp(pVPname, L"Chase") == 0)
	{
		if (m_p3DViewService != NULL && SUCCEEDED(m_p3DViewService->GetCameraPos(&pos)))
		{
			pos.orig.x = TX;
			pos.orig.y = -200;
			pos.orig.z = 30;
			VERIFY(SUCCEEDED(m_p3DViewService->SetCameraPos(&pos)));
		}
	}

	/*
	hr = pChase->get_Fields(&pTransformFields);

	if (SUCCEEDED(hr)) {
		// Getting translation field by name
		IFieldObject* pTranslationField;

		VARIANT varName;
		VariantInit(&varName);
		varName.vt = VT_BSTR;

		varName.bstrVal = SysAllocString(L"translation");
		hr = pTransformFields->get_Item(&varName, &pTranslationField);
		SysFreeString(varName.bstrVal);
		if (SUCCEEDED(hr)) {
			// Get ISFVec3fObject from IFieldObject
			hr = pTranslationField->QueryInterface(IID_ISFVec3fObject, (void**)&m_pTranslation);
			if (SUCCEEDED(hr)) {
				m_pTranslation->put_X(TX);
				m_pTranslation->put_Y(-200);
				m_pTranslation->put_Z(30);
				
				m_pTranslation->Release();
			}
			pTranslationField->Release();
		}
		pTransformFields->Release();
		VariantClear(&varName);
	}
	*/

	hr = pWagon1->get_Fields(&pTransformFields);

	if (SUCCEEDED(hr)) {
		// Getting translation field by name
		IFieldObject* pTranslationField;

		VARIANT varName;
		VariantInit(&varName);
		varName.vt = VT_BSTR;

		varName.bstrVal = SysAllocString(L"translation");
		hr = pTransformFields->get_Item(&varName, &pTranslationField);
		SysFreeString(varName.bstrVal);
		if (SUCCEEDED(hr)) {
			// Get ISFVec3fObject from IFieldObject
			hr = pTranslationField->QueryInterface(IID_ISFVec3fObject, (void**)&m_pTranslation);
			if (SUCCEEDED(hr)) {
				m_pTranslation->put_X(WX);
				m_pTranslation->put_Y(Sway2);
				m_pTranslation->put_Z(0);
				m_pTranslation->Release();
			}
			pTranslationField->Release();
		}
		pTransformFields->Release();
		VariantClear(&varName);
	}

	hr = pWagon2->get_Fields(&pTransformFields);

	if (SUCCEEDED(hr)) {
		// Getting translation field by name
		IFieldObject* pTranslationField;

		VARIANT varName;
		VariantInit(&varName);
		varName.vt = VT_BSTR;

		varName.bstrVal = SysAllocString(L"translation");
		hr = pTransformFields->get_Item(&varName, &pTranslationField);
		SysFreeString(varName.bstrVal);
		if (SUCCEEDED(hr)) {
			// Get ISFVec3fObject from IFieldObject
			hr = pTranslationField->QueryInterface(IID_ISFVec3fObject, (void**)&m_pTranslation);
			if (SUCCEEDED(hr)) {
				m_pTranslation->put_X(-WX);
				m_pTranslation->put_Y(Sway2);
				m_pTranslation->put_Z(0);
				m_pTranslation->Release();
			}
			pTranslationField->Release();
		}
		pTransformFields->Release();
		VariantClear(&varName);
	}
	return SUCCEEDED(hr);
}

HRESULT CJccvrmlDlg::UCables(float TX, float WX, Matrix Co, float Sway2)
{
	interface IFieldObject *fld;
	BSTR ftype = SysAllocString(L"SFRotation");
	m_pEngine->CreateField(ftype, &fld);
	SysFreeString(ftype);

	float value[4], stretch[3], x, y, z;
	x = (float)(-A12 / 2 + WX + TX - Co(0, 0));
	y = (float)(-A14 / 2 - Co(1, 0) + Sway2);
	z = (float)(45.5 - Co(2, 0));
	value[0] = (float)(-y);
	value[1] = (float)(x);
	value[2] = (float)(0);
	value[3] = (float)(atan2(sqrt(x*x + y * y), z));
	stretch[0] = (float)(sqrt(x*x + y * y + z * z) - 0.75);
	stretch[1] = (float)(stretch[0]);
	stretch[2] = (float)(stretch[0]);

	IFieldsCollection* pTransformFields;
	HRESULT hr = pCable1->get_Fields(&pTransformFields);

	if (SUCCEEDED(hr)) {

		// Getting translation field by name
		IFieldObject *pScaleField, *pRotationField;
		
		VARIANT varName;
		VariantInit(&varName);
		varName.vt = VT_BSTR;

		varName.bstrVal = SysAllocString(L"rotation");
		hr = pTransformFields->get_Item(&varName, &pRotationField);
		SysFreeString(varName.bstrVal);
		if (SUCCEEDED(hr)) {

			// Get ISFRotationObject from IFieldObject
			hr = fld->QueryInterface(IID_ISFRotationObject, (void**)&m_pRotation);
			//hr = pRotationField->QueryInterface(IID_ISFRotationObject, (void**)&m_pRotation);
			if (SUCCEEDED(hr)) {
				m_pRotation->put_X(value[0]);
				m_pRotation->put_Y(value[1]);
				m_pRotation->put_Z(value[2]);
				m_pRotation->put_Angle(value[3]);
				pRotationField->Assign(fld);
				m_pRotation->Release();
			}
			pRotationField->Release();
		}

		varName.bstrVal = SysAllocString(L"scale");
		hr = pTransformFields->get_Item(&varName, &pScaleField);
		SysFreeString(varName.bstrVal);

		if (SUCCEEDED(hr)) {

			// Get ISFVec3fObject from IFieldObject
			hr = pScaleField->QueryInterface(IID_ISFVec3fObject, (void**)&m_pTranslation);
			if (SUCCEEDED(hr)) {
				
				m_pTranslation->put_X(stretch[0]);
				m_pTranslation->put_Y(stretch[1]);
				m_pTranslation->put_Z(stretch[2]);
				m_pTranslation->Release();
			}
			pScaleField->Release();
		}
		VariantClear(&varName);
	}
	pTransformFields->Release();

	x = (float)(A12 / 2 + WX + TX - Co(0, 1));
	y = (float)(-A23 / 2 - Co(1, 1) + Sway2);
	z = (float)(45.5 - Co(2, 1));
	value[0] = (float)(-y);
	value[1] = (float)(x);
	value[2] = (float)(0);
	value[3] = (float)(atan2(sqrt(x*x + y * y), z));
	stretch[0] = (float)(sqrt(x*x + y * y + z * z) - 0.75);
	stretch[1] = (float)(stretch[0]);
	stretch[2] = (float)(stretch[0]);
	hr = pCable2->get_Fields(&pTransformFields);

	if (SUCCEEDED(hr)) {

		// Getting translation field by name
		IFieldObject *pScaleField, *pRotationField;


		VARIANT varName;
		VariantInit(&varName);
		varName.vt = VT_BSTR;
		varName.bstrVal = SysAllocString(L"rotation");
		hr = pTransformFields->get_Item(&varName, &pRotationField);
		SysFreeString(varName.bstrVal);
		if (SUCCEEDED(hr)) {

			// Get ISFRotationObject from IFieldObject
			hr = fld->QueryInterface(IID_ISFRotationObject, (void**)&m_pRotation);
			//hr = pRotationField->QueryInterface(IID_ISFRotationObject, (void**)&m_pRotation);
			if (SUCCEEDED(hr)) {
				m_pRotation->put_X(value[0]);
				m_pRotation->put_Y(value[1]);
				m_pRotation->put_Z(value[2]);
				m_pRotation->put_Angle(value[3]);
				pRotationField->Assign(fld);
				m_pRotation->Release();
			}
			pRotationField->Release();
		}

		varName.bstrVal = SysAllocString(L"scale");
		hr = pTransformFields->get_Item(&varName, &pScaleField);
		SysFreeString(varName.bstrVal);

		if (SUCCEEDED(hr)) {

			// Get ISFVec3fObject from IFieldObject
			hr = pScaleField->QueryInterface(IID_ISFVec3fObject, (void**)&m_pTranslation);
			if (SUCCEEDED(hr)) {

				m_pTranslation->put_X(stretch[0]);
				m_pTranslation->put_Y(stretch[1]);
				m_pTranslation->put_Z(stretch[2]);
				m_pTranslation->Release();
			}
			pScaleField->Release();
		}
		VariantClear(&varName);
	}
	pTransformFields->Release();

	x = (float)(A12 / 2 - WX + TX - Co(0, 2));
	y = (float)(A23 / 2 - Co(1, 2) + Sway2);
	z = (float)(45.5 - Co(2, 2));
	value[0] = (float)(-y);
	value[1] = (float)(x);
	value[2] = (float)(0);
	value[3] = (float)(atan2(sqrt(x*x + y * y), z));
	stretch[0] = (float)(sqrt(x*x + y * y + z * z) - 0.75);
	stretch[1] = (float)(stretch[0]);
	stretch[2] = (float)(stretch[0]);
	hr = pCable3->get_Fields(&pTransformFields);

	if (SUCCEEDED(hr)) {

		// Getting translation field by name
		IFieldObject *pScaleField, *pRotationField;

		VARIANT varName;
		VariantInit(&varName);
		varName.vt = VT_BSTR;
		varName.bstrVal = SysAllocString(L"rotation");
		hr = pTransformFields->get_Item(&varName, &pRotationField);
		SysFreeString(varName.bstrVal);
		if (SUCCEEDED(hr)) {

			// Get ISFRotationObject from IFieldObject
			hr = fld->QueryInterface(IID_ISFRotationObject, (void**)&m_pRotation);
			//hr = pRotationField->QueryInterface(IID_ISFRotationObject, (void**)&m_pRotation);
			if (SUCCEEDED(hr)) {
				m_pRotation->put_X(value[0]);
				m_pRotation->put_Y(value[1]);
				m_pRotation->put_Z(value[2]);
				m_pRotation->put_Angle(value[3]);
				pRotationField->Assign(fld);
				m_pRotation->Release();
			}
			pRotationField->Release();
		}

		varName.bstrVal = SysAllocString(L"scale");
		hr = pTransformFields->get_Item(&varName, &pScaleField);
		SysFreeString(varName.bstrVal);

		if (SUCCEEDED(hr)) {

			// Get ISFVec3fObject from IFieldObject
			hr = pScaleField->QueryInterface(IID_ISFVec3fObject, (void**)&m_pTranslation);
			if (SUCCEEDED(hr)) {

				m_pTranslation->put_X(stretch[0]);
				m_pTranslation->put_Y(stretch[1]);
				m_pTranslation->put_Z(stretch[2]);
				m_pTranslation->Release();
			}
			pScaleField->Release();
		}
		VariantClear(&varName);
	}
	pTransformFields->Release();

	x = (float)(-A12 / 2 - WX + TX - Co(0, 3));
	y = (float)(A14 / 2 - Co(1, 3) + Sway2);
	z = (float)(45.5 - Co(2, 3));
	value[0] = (float)(-y);
	value[1] = (float)(x);
	value[2] = (float)(0);
	value[3] = (float)(atan2(sqrt(x*x + y * y), z));
	stretch[0] = (float)(sqrt(x*x + y * y + z * z) - 0.75);
	stretch[1] = (float)(stretch[0]);
	stretch[2] = (float)(stretch[0]);
	hr = pCable4->get_Fields(&pTransformFields);

	if (SUCCEEDED(hr)) {

		// Getting translation field by name
		IFieldObject *pScaleField, *pRotationField;


		VARIANT varName;
		VariantInit(&varName);
		varName.vt = VT_BSTR;
		varName.bstrVal = SysAllocString(L"rotation");
		hr = pTransformFields->get_Item(&varName, &pRotationField);
		SysFreeString(varName.bstrVal);
		if (SUCCEEDED(hr)) {

			// Get ISFRotationObject from IFieldObject
			hr = fld->QueryInterface(IID_ISFRotationObject, (void**)&m_pRotation);
			//hr = pRotationField->QueryInterface(IID_ISFRotationObject, (void**)&m_pRotation);
			if (SUCCEEDED(hr)) {
				m_pRotation->put_X(value[0]);
				m_pRotation->put_Y(value[1]);
				m_pRotation->put_Z(value[2]);
				m_pRotation->put_Angle(value[3]);
				pRotationField->Assign(fld);
				m_pRotation->Release();
			}
			pRotationField->Release();
		}

		varName.bstrVal = SysAllocString(L"scale");
		hr = pTransformFields->get_Item(&varName, &pScaleField);
		SysFreeString(varName.bstrVal);

		if (SUCCEEDED(hr)) {

			// Get ISFVec3fObject from IFieldObject
			hr = pScaleField->QueryInterface(IID_ISFVec3fObject, (void**)&m_pTranslation);
			if (SUCCEEDED(hr)) {

				m_pTranslation->put_X(stretch[0]);
				m_pTranslation->put_Y(stretch[1]);
				m_pTranslation->put_Z(stretch[2]);
				m_pTranslation->Release();
			}
			pScaleField->Release();
		}
		VariantClear(&varName);
	}
	pTransformFields->Release();
	return SUCCEEDED(hr);
}


BEGIN_EVENTSINK_MAP(CJccvrmlDlg, CDialog)
ON_EVENT(CJccvrmlDlg, ID_CORTONA_CONTROL, 6, CJccvrmlDlg::OnSceneLoadedCortonaControl, VTS_BOOL)
END_EVENTSINK_MAP()


void CJccvrmlDlg::OnSceneLoadedCortonaControl(BOOL Success)
{
	INodesCollection* pNodes;
	HRESULT hr = m_pEngine->get_Nodes(&pNodes);
	//INodeObject* pSubsceneRoot;
	long n;
	hr = pNodes->get_Count(&n);
	VARIANT varName;
	VariantInit(&varName);
	varName.vt = VT_BSTR;

	//varName.bstrVal = SysAllocString(L"Crane");
	//hr = pRoots->get_Item(&varName, &pSubsceneRoot);
	//SysFreeString(varName.bstrVal);

	varName.bstrVal = SysAllocString(L"Master");
	hr = pNodes->get_Item(&varName, &pMaster);
	SysFreeString(varName.bstrVal);
	varName.bstrVal = SysAllocString(L"Chase");
	hr = pNodes->get_Item(&varName, &pChase);
	SysFreeString(varName.bstrVal);
	varName.bstrVal = SysAllocString(L"Trolley");
	hr = pNodes->get_Item(&varName, &pTrolley);
	SysFreeString(varName.bstrVal);
	varName.bstrVal = SysAllocString(L"Container");
	hr = pNodes->get_Item(&varName, &pContainer);
	SysFreeString(varName.bstrVal);
	varName.bstrVal = SysAllocString(L"Cable1");
	hr = pNodes->get_Item(&varName, &pCable1);
	SysFreeString(varName.bstrVal);
	varName.bstrVal = SysAllocString(L"Cable2");
	hr = pNodes->get_Item(&varName, &pCable2);
	SysFreeString(varName.bstrVal);
	varName.bstrVal = SysAllocString(L"Cable3");
	hr = pNodes->get_Item(&varName, &pCable3);
	SysFreeString(varName.bstrVal);
	varName.bstrVal = SysAllocString(L"Cable4");
	hr = pNodes->get_Item(&varName, &pCable4);
	SysFreeString(varName.bstrVal);
	varName.bstrVal = SysAllocString(L"Wagon1");
	hr = pNodes->get_Item(&varName, &pWagon1);
	SysFreeString(varName.bstrVal);
	varName.bstrVal = SysAllocString(L"Wagon2");
	hr = pNodes->get_Item(&varName, &pWagon2);
	SysFreeString(varName.bstrVal);

	pNodes->Release();
	VariantClear(&varName);

	initDynamics();
	IDTimer = SetTimer(0, (UINT)(DT * 1000 * SUBSAMPLE), NULL);
}

void CJccvrmlDlg::initDynamics()
{
	Time = 0;

	// Status signals
	Act = 0;  // Activation
	OS = 0;   // Out-of-Sight

	// Inputs
	TrolleyPos = 10.0;// trolley trajectory
	Wagon = 0.0;  // wagon trajectory
	Cable = 25.0;
	CableL[0] = Cable;// cable length (right) in m
	CableL[1] = Cable;// cable length (left) in m
	WindT = 0.0;     // disturbance torque on z-axis of container

	// Time = 0 defined by initialization
	TX = 0.0;        // initial trolley position
	Mass = 20e3;     // Load mass (container + spreader) in kg
	CG(0, 0) = 0.0;	 // CGx in m
	CG(1, 0) = 0.0;	 // CGy in m
	CG(2, 0) = 0.5;   // CGz in m

	// Initial Values
	EA(2, 0) = 5.0*PI / 180.0;   // initial skew in rad
	dEA(2, 0) = 0.0;  // initial skew rate in rad/s
	initSwing = 0.0*PI / 180.0; // initial sway in rad
	WX = 0.0;        // initial wagon position in m
	dWX = 0.0;
	dWXnew = 0.0;    // initial wagon speed in m/s

	// fill the memory of the filter buffer with initial skew angle
	for (i = 0; i < 150; i++) BUF[i] = EA(2, 0);

	TX = TrolleyPos;
	dTX = 0.0;
	WX = Wagon;
	CL[0] = CableL[0];
	CL[1] = CableL[1];
	dTrolleyPos = 0.0;
	dCableL = 0.0;
	// inertia factor for homogeneous weight distribution 40 ft container
	I(0, 0) = 1.3514;
	I(1, 0) = 1.0;
	I(2, 0) = 1.3514;

	//////////////////////////////////////////////
	// INITIALIZE STATES OF THE CRANE MODEL  /////
	//////////////////////////////////////////////

	oldS(0, 0) = 0.475*Mass*GRAVITATION / KS * CL[0];
	oldS(0, 1) = 0.475*Mass*GRAVITATION / KS * CL[0];
	oldS(0, 2) = 0.475*Mass*GRAVITATION / KS * CL[1];
	oldS(0, 3) = 0.475*Mass*GRAVITATION / KS * CL[1];
	dS(0, 0) = 0.0;    dS(0, 1) = 0.0;    dS(0, 2) = 0.0;  dS(0, 3) = 0.0;
	ddX(0, 0) = 0.0;    ddX(1, 0) = 0.0;    ddX(2, 0) = 0.0;
	dX(0, 0) = 0.0;    dX(1, 0) = 0.0;    dX(2, 0) = 0.0;
	dXnew(0, 0) = 0.0; dXnew(1, 0) = 0.0; dXnew(2, 0) = 0.0;
	meanL = (CL[0] + CL[1]) / 2;
	meanS = (oldS(0, 0) + oldS(0, 1) + oldS(0, 2) + oldS(0, 3)) / 4;
	X(0, 0) = TX + (meanL + meanS)*sin(initSwing);
	X(1, 0) = 2.0;   // crane drive swing
	X(2, 0) = HEIGHT - (meanL + meanS)*cos(initSwing) + WHEELR - DIMZ / 2;
	dO(0, 0) = 0.0; dO(1, 0) = 0.0; dO(2, 0) = 0.0;
	O(0, 0) = 0.0; O(1, 0) = 0.0;
	// in approximation ..
	O(2, 0) = dEA(2, 0);
	EA(0, 0) = atan2((CL[0] - CL[1]), 6.0);
	EA(1, 0) = sin(1.3 / 0.8*asin(initSwing)) - initSwing;
	// EA(2,0)  see JCC3D line 98
	dEA(0, 0) = 0.0;
	dEA(1, 0) = 0.0;
	// dEA(2,0)  see JCC3D line 99
	II(0, 0) = Mass * (DIMY*DIMY + DIMZ * DIMZ) / 12.0 / 1.3514*I(0, 0);
	II(1, 0) = Mass * (DIMX*DIMX + DIMZ * DIMZ) / 12.0 / 1.0*I(1, 0);
	II(2, 0) = Mass * (DIMX*DIMX + DIMY * DIMY) / 12.0 / 1.3514*I(2, 0);
	//if (w>1) {dWX=0.0;ddWX=0.0;dWXnew=0.0;}
	// Make Rotation Matrix
	RA = MakeRot(EA);
	// Pulley Locations on the trolley
	A(0, 0) = -A12 / 2 + WX + TX; A(1, 0) = -A14 / 2; A(2, 0) = HEIGHT + WHEELR;
	A(0, 1) = A12 / 2 + WX + TX; A(1, 1) = -A23 / 2; A(2, 1) = HEIGHT + WHEELR;
	A(0, 2) = A12 / 2 - WX + TX; A(1, 2) = A23 / 2; A(2, 2) = HEIGHT + WHEELR;
	A(0, 3) = -A12 / 2 - WX + TX; A(1, 3) = A14 / 2; A(2, 3) = HEIGHT + WHEELR;
	// Pulley locations on the container
	Cc(0, 0) = -C12 / 2 - CG(0, 0); Cc(1, 0) = -C14 / 2 - CG(1, 0); Cc(2, 0) = DIMZ / 2 - CG(2, 0);
	Cc(0, 1) = C12 / 2 - CG(0, 0); Cc(1, 1) = -C23 / 2 - CG(1, 0); Cc(2, 1) = DIMZ / 2 - CG(2, 0);
	Cc(0, 2) = C12 / 2 - CG(0, 0); Cc(1, 2) = C23 / 2 - CG(1, 0); Cc(2, 2) = DIMZ / 2 - CG(2, 0);
	Cc(0, 3) = -C12 / 2 - CG(0, 0); Cc(1, 3) = C14 / 2 - CG(1, 0); Cc(2, 3) = DIMZ / 2 - CG(2, 0);
	C = RA * Cc;
	for (j = 0; j < 4; j++) for (k = 0; k < 3; k++) Co(k, j) = C(k, j) + X(k, 0) + CG(k, 0);
	// Cables in space
	D = A - Co;
	// Actual Cable Length
	for (j = 0; j < 4; j++)
		DL(0, j) = sqrt(D(0, j)*D(0, j) + D(1, j)*D(1, j) + D(2, j)*D(2, j));
	// Stretch in the cables
	S = oldS;
	// Forces in the cables; second order suspension per cable
	for (j = 0; j < 4; j++)
		for (k = 0; k < 3; k++)
			if (S(0, j) <= 0.0)
				CableF(k, j) = 0.0;
			else
				CableF(k, j) =
				0.5774*(KS*S(0, j) + DS * dS(0, j)) / DL(0, j) / DL(0, j)*D(k, j);
	skew = EA(2, 0);  oldskew = EA(2, 0); olddskew = 0;
	Skew0 = 0.0; dSkew0 = 0.0; T0 = 0.0;
	Sway = atan2(45.5 - X(2, 0), TX - X(0, 0)) - PI / 2;; dSway = 0;
	dWXnew = dWX;
	Sway2 = 0.0;
}

void CJccvrmlDlg::JCCDynamics(double dT, int SubSample, float x_in, float y_in,
	int Buttons[], double offset)
{


	// User Trolley Inputs
	if (dTrolleyPos < -y_in) dTrolleyPos += 0.01;
	if (dTrolleyPos > -y_in) dTrolleyPos -= 0.01;
	TrolleyPos += dTrolleyPos;
	if (TrolleyPos > 62.0) TrolleyPos = 62.0;
	if (TrolleyPos < -62.0) TrolleyPos = -62.0;
	if (SWING_CTRL > 0)
		if (Buttons[SWING_CTRL - 1] == SWING_INV) {
			// Apply Sway Control by default
			TrolleyPos += CableL[0] / 20.0*(-0.3*dSway + 0.6*Sway);
		}

	// User Hoisting Inputs
	if (dCableL < x_in) dCableL += 0.01;
	if (dCableL > x_in) dCableL -= 0.01;
	Cable += dCableL;
	if (Cable < 6.0) Cable = 6.0;
	CableL[0] = Cable;
	CableL[1] = Cable;

	// User Skew Inputs
	if (SKEW_CTRL > 0) {
		if (Buttons[SKEW_CTRL - 1] == SKEW_INV)
			Act = 1;
		else
			Act = 0;
	}
	if (SWAY_CTRL > 0) {
		if (Buttons[SWAY_CTRL - 1] == SWAY_INV)
			Sway2 = -dX(1, 0);
		else
			Sway2 = 0.0;
	}

	// Apply low gradient on configuration changes
	if (CG(2, 0) < CG_Z) CG(2, 0) += 0.05;
	if (CG(2, 0) > CG_Z) CG(2, 0) -= 0.05;
	if (Mass < ContainerMass) Mass++;
	if (Mass > ContainerMass) Mass--;

	for (i = 0; i < SubSample; i++) {
		Time += dT;

		// Define inputs:
		dTX = TrolleyPos - TX;
		TX = TrolleyPos;

		CL[0] = CableL[0];  // right hoist length
		CL[1] = CableL[1];  // left hoist length

		// Compute Skew Control
		dWX = CTRL(dT, EA(2, 0), offset, Act, OS, WX, dWX, DL, I, EA, dEA);

		// Integrate Wagon Speed
		WX += dWX * dT;

		////////////////////////////////////////////////////
		// 3D DYNAMICS MODEL OF THE CONTAINER CRANE       //
		////////////////////////////////////////////////////

		// Make Rotation Matrix
		RA = MakeRot(EA);

		// Pulley Locations on the trolley
		A(0, 0) = -A12 / 2 + WX + TX;
		A(0, 1) = A12 / 2 + WX + TX;
		A(0, 2) = A12 / 2 - WX + TX;
		A(0, 3) = -A12 / 2 - WX + TX;
		A(1, 0) = -A14 / 2 + (double)Sway2;
		A(1, 1) = -A23 / 2 + (double)Sway2;
		A(1, 2) = A23 / 2 + (double)Sway2;
		A(1, 3) = A14 / 2 + (double)Sway2;
		C = RA * Cc;
		for (j = 0; j < 4; j++) for (k = 0; k < 3; k++) Co(k, j) = C(k, j) + X(k, 0) + CG(k, 0);
		// Cables in space
		D = A - Co;
		// Actual Cable Length
		for (j = 0; j < 4; j++)
			DL(0, j) = sqrt(D(0, j)*D(0, j) + D(1, j)*D(1, j) + D(2, j)*D(2, j));
		// Stretch in the cables
		S(0, 0) = DL(0, 0) - CL[0]; if (S(0, 0) > MAXFLEX) S(0, 0) = MAXFLEX;
		S(0, 1) = DL(0, 1) - CL[0]; if (S(0, 1) > MAXFLEX) S(0, 1) = MAXFLEX;
		S(0, 2) = DL(0, 2) - CL[1]; if (S(0, 2) > MAXFLEX) S(0, 2) = MAXFLEX;
		S(0, 3) = DL(0, 3) - CL[1]; if (S(0, 3) > MAXFLEX) S(0, 3) = MAXFLEX;
		dS = (S - oldS) / dT;
		oldS = S;
		// Forces in the cables; second order suspension per cable
		for (j = 0; j < 4; j++)
			for (k = 0; k < 3; k++)
				if (S(0, j) <= 0.0) // force is 0 on negative stretch
					CableF(k, j) = 0.0;
				else
					CableF(k, j) =
					0.5774*(KS*S(0, j) + DS * dS(0, j)) / DL(0, j) / DL(0, j)*D(k, j);
		// Torques by the cables on the container
		CableT = Cross(C, CableF);
		CableT(2, 0) += WindT;
		// Torques become rotation accelerations
		// dO = ( RA'*Torque - (O × II*O) ) / II 
		for (j = 0; j < 3; j++) vec3(j, 0) = II(j, 0)*O(j, 0);
		dOnew = (Transpose(RA)*CableT - Cross(O, vec3));
		for (j = 0; j < 3; j++) dOnew(j, 0) /= II(j, 0);
		// Digital Redesign
		dOnew -= O * (15 / (CL[0] + CL[1])*dT);
		O += (dO + dOnew) / 2 * dT;
		// Rotation must be transformed to angular velocity
		dEAnew = MakeRf(EA) * O;
		EA += (dEA + dEAnew) / 2 * dT;
		// Linear forces on the container
		for (j = 0; j < 3; j++)
			ConF(j, 0) = CableF(j, 0) + CableF(j, 1) + CableF(j, 2) + CableF(j, 3);
		// Container acceleration in space
		ddXnew = ConF / Mass;
		ddXnew(2, 0) -= GRAVITATION;
		// Digital redesign
		ddXnew -= (18 / (CL[0] + CL[1])*dT)*dX;
		// Container velocity in space
		dXnew += (ddX + ddXnew) / 2 * dT;
		// Container position in space
		X += (dX + dXnew) / 2 * dT;
		// Update for bilinear integration
		dO = dOnew;			dEA = dEAnew;
		ddX = ddXnew;		dX = dXnew;
		// Update for controller
		olddskew = dEA(2, 0);
		oldskew = EA(2, 0);
	}

	//////////////////////////////////////
	// Generate output data             //
	//////////////////////////////////////

	Quaternion(Q, EA);
	UContainer(Q, X, CG);
	UTrolley((float)TX, (float)WX, (float)Sway2);
	UCables((float)TX, (float)WX, Co, (float)Sway2);

	newSway = atan2(45.5 - X(2, 0), TX - X(0, 0)) - PI / 2;
	dSway = (newSway - Sway) / dT;   // approximation
	Sway = newSway;

	SetDlgItemText(IDC_L, _gcvt((CL[0] + CL[1]) / 2, 5, ValChar));
	SetDlgItemText(IDC_XT, _gcvt(TX, 5, ValChar));
	sprintf(ValChar, "%8.5f", WX);
	SetDlgItemText(IDC_WX, ValChar);
	sprintf(ValChar, "%8.5f", Sway);
	SetDlgItemText(IDC_SWING, ValChar);
	sprintf(ValChar, "%8.5f", EA(2, 0));
	SetDlgItemText(IDC_SKEW, ValChar);
}




void CJccvrmlDlg::Filter(double L, double dT, double SkewSensor) {
	int WIN, OVL, i;
	double a, b, c, da, db, ddy, dtwin;

	WIN = max((int)L, 20);
	OVL = (int)(WIN*OVERLAY);
	/* filter spikes of 0.05 rad/sample */
	if ((SkewSensor - BUF[0]) > 0.05) SkewSensor = BUF[0] + 0.05;
	if ((SkewSensor - BUF[0]) < -0.05) SkewSensor = BUF[0] - 0.05;
	// Update buffer
	for (i = 149; i > 0; i--) BUF[i] = BUF[i - 1];
	BUF[0] = SkewSensor;
	a = 0.0;	b = 0.0;	c = 0.0;
	for (i = 0; i <= WIN; i++) {
		a += BUF[i];
		b += BUF[i + WIN - OVL];
		c += BUF[i + 2 * (WIN - OVL)];
	};
	a /= (double)WIN;
	b /= (double)WIN;
	c /= (double)WIN;
	// time of averaging interval
	dtwin = (WIN - OVL)*dT;
	// compute gradients
	da = (a - b) / dtwin;
	db = (b - c) / dtwin;
	ddy = (da - db) / dtwin;
	// limit acceleration
	if (ddy > MAXDDY) ddy = MAXDDY;
	if (ddy < -MAXDDY) ddy = -MAXDDY;
	// extrapolate sensor value rate
	dSkew = da + ddy * dT*WIN*(1 - OVERLAY / 2.0);
	// extrapolate sensor value
	Skew = a + dSkew * dT*(WIN / 2.0);

	// Limit for safety
	if (Skew > 0.2) Skew = 0.2;
	if (Skew < -0.2) Skew = -0.2;
}

// Skew and Skew rate prediction function
void  CJccvrmlDlg::Prediction(double L, Matrix I, double T0, double Skew0, double dSkew0, int Act) {
	double T, w, decay, lambda0, A;

	// Estimated Skew period time
	T = 2 * PI*sqrt(I(2, 0))*sqrt(L / GRAVITATION);
	// Skew harmonic frequency
	w = 2 * PI / T;
	decay = 5 / L / I(2, 0);
	// find phase of latest measurement
	if (dSkew0 > Skew0) lambda0 = atan2(w*Skew0, dSkew0);
	else lambda0 = PI / 2 - atan2(dSkew0, w*Skew0);
	// find amplitude
	A = Skew0 / sin(lambda0);
	// if controller is active
	if (Act) {
		if (T0 < (1 / decay))
			A *= (1.0 - decay * T0);
		else A = 0.0;
	}
	Skew = A * sin(w*T0 + lambda0);
	if (Act) T0 *= 1.5;
	dSkew = A * w * cos(w*T0 + lambda0);
}

// ControlF returns the force F as input for the wagon actuator
double  CJccvrmlDlg::ControlF(double L, Matrix I,
	double WX, double dWX, double offset) {
	double F, K1, K2, K3, K4;

	//printf("Gains :%f, %f, %f, %f\n",K1,K2,K3,K4);

	K1 = -45000.0;
	K2 = -35000.0 - I(2, 0)*102000.0 - L * 2888.0 / sqrt(I(2, 0));
	K3 = -21000.0 - 8400.0 / I(2, 0) + L * (180 + 69 / sqrt(I(2, 0)));
	K4 = -10000.0;
	if (L < 17) {
		L = 17.0 - L;
		K1 += L * 2000.0 / I(2, 0);
		K2 += L * L*300.0*sqrt(I(2, 0)) + L * 2656.0*sqrt(I(2, 0));
		K3 -= L * 533.0 / I(2, 0);
	}

	if (K2 < -280000.0) K2 = -280000.0;
	if (K2 > -20000.0)  K2 = -20000.0;
	if (K3 < -40000.0)  K3 = -40000.0;

	// Skew dSkew WagonPos WagonSpeed
	F = K1 * (Skew - offset) + K2 * dSkew + K3 * (WX - offset * 2.79) + K4 * dWX;

	return F;
}

double CJccvrmlDlg::CTRL(double dT, double SkewSensor, double offset,
	int Act, int OS, double WX, double dWX,
	Matrix DL, Matrix I, Matrix EA, Matrix dEA)
{
	double L;

	// Average Cable Length
	L = (DL(0, 0) + DL(0, 1) + DL(0, 2) + DL(0, 3)) / 4.0;

	if (OS) {
		T0 += dT;
		Prediction(L, I, T0, Skew0, dSkew0, Act);
	}
	else {
		Filter(L, dT, SkewSensor);
		T0 = 0.0;
		Skew0 = Skew;
		dSkew0 = dSkew;
	}
	if (Act) {
		// Compute the control gains and apply to find control force
		WagonF = ControlF(L, I, WX, dWX, offset);
		ddWXnew = WagonF / MASSWAGON;
		// Limit wagon acceleration
		if (ddWXnew > MAXACCEL) ddWXnew = MAXACCEL;
		if (ddWXnew < -MAXACCEL) ddWXnew = -MAXACCEL;
		// Apply moving average filter on acceleration
		FddWX = ACCFILT * FddWX + (1.0 - ACCFILT)*ddWXnew;
		ddWXnew = FddWX;
		// Integrate acceleration to get velocity
		dWXnew += (ddWX + ddWXnew) / 2 * dT;
	}
	else {
		// Move to neutral
		if (WX >= (offset*2.79)) dWXnew -= MAXACCEL * dT;
		else dWXnew += MAXACCEL * dT;
		if (dWXnew > 0) dWXnew = min(fabs(dWXnew), sqrt(fabs(WX - offset * 2.79) * 2));
		else dWXnew = -min(abs(dWXnew), sqrt(abs(WX - offset * 2.79) * 2));
		ddWXnew = (dWXnew - dWX) / dT;
		// if velocity is small, make it zero
		if (abs(dWXnew) < MAXACCEL*dT) {
			dWXnew = 0.0;
		}
	}

	// Limit velocity
	if (dWXnew > MAXVELOC) { dWXnew = MAXVELOC; ddWXnew = 0.0; }
	if (dWXnew < -MAXVELOC) { dWXnew = -MAXVELOC; ddWXnew = 0.0; }
	//WX += (dWXnew + dWX)/2*dT;

	// Slow down when approaching physical limits
	if ((WX > (MAXPOS*0.8)) & (dWXnew > 0.0)) {
		dWXnew = min(dWXnew, sqrt(max(0.0, MAXPOS - WX))*2.0);
		ddWXnew = (dWXnew - dWX) / dT;
	}
	if ((WX < (-MAXPOS * 0.8)) & (dWXnew < 0.0)) {
		dWXnew = max(dWXnew, sqrt(max(0.0, WX + MAXPOS))*-2.0);
		ddWXnew = (dWXnew - dWX) / dT;
	}

	// limit position
	if (((WX > MAXPOS) & (dWXnew > 0.0)) | ((WX < -MAXPOS) & (dWXnew < 0.0))) {
		dWXnew = 0.0;
		ddWXnew = 0.0;
	}

	// Update for bilinear integration
	ddWX = ddWXnew;
	FddWX = ddWXnew;
	dWX = dWXnew;
	return dWX;
}


void CJccvrmlDlg::OnDestroy()
{

	if (m_CortonaCtl) {
		if (m_pCortonaEvents) {
			AfxConnectionUnadvise(m_CortonaCtl->GetControlUnknown(), DIID__CortonaEvents,
				m_pCortonaEvents->GetIDispatch(FALSE), FALSE, m_dwCoockie);
			m_pCortonaEvents->GetIDispatch(FALSE)->Release();
			m_pCortonaEvents = 0;
		}
		m_pCortona->Release();
		m_pCortona = 0;
	}

	//Free all memory
	if (m_p3DViewService != NULL)
	{
		m_p3DViewService->Release();
		m_p3DViewService = NULL;
	}
	m_pEngine->Release();
	pMaster->Release();
	pTrolley->Release();
	pWagon1->Release();
	pWagon2->Release();
	pChase->Release();
	pCable1->Release();
	pCable2->Release();
	pCable3->Release();
	pCable4->Release();
	pContainer->Release();
	//m_pTranslation->Release();
	//m_pRotation->Release();

	CDialog::OnDestroy();
}


void CJccvrmlDlg::OnBnClickedJcchelp()
{
	::HtmlHelp(m_hWnd, "Jccvrml.chm", HH_DISPLAY_TOC, 0);
}
