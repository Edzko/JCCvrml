// JCCJoy.h: interface for the CJoyStick class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JCCJOY_H__89B52CE4_E684_11D3_8661_B18E6F37F658__INCLUDED_)
#define AFX_JCCJOY_H__89B52CE4_E684_11D3_8661_B18E6F37F658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>

class CJoyStick  
{
public:
	int JOYID;
	CJoyStick();
	virtual ~CJoyStick();

	double x, y, z;
	int Buttons[4];
	MMRESULT mResult;

	void GetJoyInputs();

private:
	JOYINFO Joystick;
	double x_0, y_0, z_0;
};

#endif // !defined(AFX_JCCJOY_H__89B52CE4_E684_11D3_8661_B18E6F37F658__INCLUDED_)
