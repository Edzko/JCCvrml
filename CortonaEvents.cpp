// CortonaEvents.cpp : implementation file
//

#include "stdafx.h"

#include "CortonaEvents.h"

#include "CortonaControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCortonaEvents

IMPLEMENT_DYNCREATE(CCortonaEvents, CCmdTarget)

CCortonaEvents::CCortonaEvents()
{
	EnableAutomation();
}

CCortonaEvents::~CCortonaEvents()
{
}

void CCortonaEvents::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////
// Cortona Control Events handlers

void CCortonaEvents::OnClick()
{
}

void CCortonaEvents::OnDblClick()
{
}

void CCortonaEvents::OnMouseDown(short button, short shift, long x, long y)
{
}

void CCortonaEvents::OnMouseMove(short button, short shift, long x, long y)
{
}

void CCortonaEvents::OnMouseUp(short button, short shift, long x, long y)
{
}

void CCortonaEvents::OnOnAnchor(LPCTSTR Url, LPCTSTR Parameter, LPCTSTR Description, LPCTSTR Name)
{
    ShellExecute( NULL, "open", Url, NULL, "", 0 );
}

void CCortonaEvents::OnOnEventOut(LPDISPATCH Value, LPCTSTR NodeName, LPCTSTR EventOutName, double TimeStamp)
{
}

void CCortonaEvents::OnOnMouseEnter()
{
}

void CCortonaEvents::OnOnMouseOut()
{
}

void CCortonaEvents::OnOnSceneLoaded(BOOL Success)
{
    ((CFrameWnd*)AfxGetMainWnd())->GetActiveView()->SendMessage( wmSceneLoaded, (WPARAM)Success );
}

void CCortonaEvents::OnOnSceneUnloaded()
{
    ((CFrameWnd*)AfxGetMainWnd())->GetActiveView()->SendMessage( wmSceneUnloaded );
}

BEGIN_MESSAGE_MAP(CCortonaEvents, CCmdTarget)
	//{{AFX_MSG_MAP(CCortonaEvents)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CCortonaEvents, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CCortonaEvents)
    //DISP_FUNCTION_ID(CCortonaEvents,"Click",DISPID_CLICK,OnClick,VT_EMPTY,VTS_NONE)
    //DISP_FUNCTION_ID(CCortonaEvents,"DblClick",DISPID_DBLCLICK,OnClick,VT_EMPTY,VTS_NONE)
    //DISP_FUNCTION_ID(CCortonaEvents,"MouseMove",DISPID_MOUSEMOVE,OnMouseMove,VT_EMPTY,VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS)
    //DISP_FUNCTION_ID(CCortonaEvents,"MouseDown",DISPID_MOUSEDOWN,OnMouseDown,VT_EMPTY,VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS)
    //DISP_FUNCTION_ID(CCortonaEvents,"MouseUp",DISPID_MOUSEUP,OnMouseUp,VT_EMPTY,VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS)
    //DISP_FUNCTION_ID(CCortonaEvents,"OnMouseEnter",1,OnOnMouseEnter,VT_EMPTY,VTS_NONE)
    //DISP_FUNCTION_ID(CCortonaEvents,"OnMouseOut",2,OnOnMouseOut,VT_EMPTY,VTS_NONE)
    //DISP_FUNCTION_ID(CCortonaEvents,"OnEventOut",5,OnOnEventOut,VT_EMPTY,VTS_DISPATCH VTS_BSTR VTS_BSTR VTS_R8)
    //DISP_FUNCTION_ID(CCortonaEvents,"OnSceneLoaded",6,OnOnSceneLoaded,VT_EMPTY, VTS_BOOL )
    //DISP_FUNCTION_ID(CCortonaEvents,"OnAnchor", 7,OnOnAnchor,VT_EMPTY,VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
    //DISP_FUNCTION_ID(CCortonaEvents,"OnSceneUnloaded",8,OnOnSceneUnloaded,VT_EMPTY,VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CCortonaEvents, CCmdTarget)
	INTERFACE_PART(CCortonaEvents,DIID__CortonaEvents,Dispatch)
END_INTERFACE_MAP()

