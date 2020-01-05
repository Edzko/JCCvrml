// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// CCortonaCtl wrapper class

class CCortonaCtl : public CWnd
{
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x86a88967, 0x7a20, 0x11d2, { 0x8e, 0xda, 0x0, 0x60, 0x8, 0x18, 0xed, 0xb1 } };
		return clsid;
	}

	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	void SetBackColor(unsigned long newValue);
	unsigned long GetBackColor();
	void SetAppearance(long nNewValue);
	long GetAppearance();
	void SetReadyState(long nNewValue);
	long GetReadyState();
	void SetScene(LPCTSTR lpszNewValue);
	CString GetScene();
	void SetEngine(LPDISPATCH newValue);
	LPDISPATCH GetEngine();
	void SetSlowMode(long nNewValue);
	long GetSlowMode();
	void SetFreeze(BOOL bNewValue);
	BOOL GetFreeze();
	void SetEventProcessed(BOOL bNewValue);
	BOOL GetEventProcessed();
	void SetCollider(BOOL bNewValue);
	BOOL GetCollider();
	void SetHeadLight(BOOL bNewValue);
	BOOL GetHeadLight();
	void SetNavigationMode(short nNewValue);
	short GetNavigationMode();
	void SetNavigationBar(short nNewValue);
	short GetNavigationBar();
	void SetAnimateViewpoints(BOOL bNewValue);
	BOOL GetAnimateViewpoints();
	void SetViewpoint_transition_mode(long nNewValue);
	long GetViewpoint_transition_mode();
	void SetShow_hidden_viewpoints(BOOL bNewValue);
	BOOL GetShow_hidden_viewpoints();
	void SetConsoleMode(short nNewValue);
	short GetConsoleMode();
	void SetTravelSpeed(long nNewValue);
	long GetTravelSpeed();
	void SetBrowser(LPUNKNOWN newValue);
	LPUNKNOWN GetBrowser();
	void SetShowFps(BOOL bNewValue);
	BOOL GetShowFps();
	void SetShowRenderingTime(BOOL bNewValue);
	BOOL GetShowRenderingTime();
	void SetShowProgress(BOOL bNewValue);
	BOOL GetShowProgress();
	void SetWaitForAllResources(BOOL bNewValue);
	BOOL GetWaitForAllResources();
	void SetRendererName(LPCTSTR lpszNewValue);
	CString GetRendererName();
	void SetRendererHints(long nNewValue);
	long GetRendererHints();
	void SetSupportedRendererHints(long nNewValue);
	long GetSupportedRendererHints();
	void SetShowLogo(BOOL bNewValue);
	BOOL GetShowLogo();
	void SetContextMenu(BOOL bNewValue);
	BOOL GetContextMenu();
	long GetAddress();
	void SetBaseURL(LPCTSTR lpszNewValue);
	CString GetBaseURL();
	void SetColliderMode(short nNewValue);
	short GetColliderMode();
	void SetMask(const VARIANT& newValue);
	VARIANT GetMask();
	void SetLoadDroppedScene(BOOL bNewValue);
	BOOL GetLoadDroppedScene();
	void SetMuteSound(short nNewValue);
	short GetMuteSound();
	void SetCpuLoading(short nNewValue);
	short GetCpuLoading();
	void Refresh();
	void Edit();
	void trace(LPCTSTR message);
	void uiAction(LPCTSTR command);
	long GetSupportedRendererOptimization();
	void SetRendererOptimization(long nNewValue);
	long GetRendererOptimization();
	LPDISPATCH GetRenderers();
	void SetRenderer(LPDISPATCH newValue);
	LPDISPATCH GetRenderer();
	LPDISPATCH GetDisplayModes();
	void SetDisplayMode(LPDISPATCH newValue);
	LPDISPATCH GetDisplayMode();
	float GetFrameRate();
	void SetSkin(LPCTSTR lpszNewValue);
	CString GetSkin();
	LPDISPATCH GetPicture();
	CString GetSupportedNavigationStyles();
	void SetNavigationStyle(LPCTSTR lpszNewValue);
	CString GetNavigationStyle();
	CString GetSupportedNavigationTypes();
	void SetNavigationType(LPCTSTR lpszNewValue);
	CString GetNavigationType();
	void SetInputDevices(long nNewValue);
	long GetInputDevices();
};
