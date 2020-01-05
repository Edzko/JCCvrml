// JCCJoy.cpp: implementation of the CJoyStick class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "jccvrml.h"
#include "JCCJoy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJoyStick::~CJoyStick()
{

}

CJoyStick::CJoyStick() 
{
	JOYID = (UINT)JOYSTICKID1;
	mResult=joyGetPos(JOYID,(LPJOYINFO)&Joystick);
	// Try second joystick
	if(mResult!=JOYERR_NOERROR) JOYID = (UINT)JOYSTICKID2;
	mResult=joyGetPos(JOYID,(LPJOYINFO)&Joystick);
	if(mResult!=JOYERR_NOERROR)
	{
		if (MessageBox(NULL,"Jostick Problem, Continue?","JCC Animation",MB_ICONQUESTION | MB_YESNO)==IDNO)
			exit(0);
		if (mResult==MMSYSERR_NODRIVER) 
			TRACE0("Error encountered, DRIVER problem.");
		else 
			TRACE("JoyStick Error encountered.");
		return;
	}

	x_0 = (float)Joystick.wXpos*2/65535-1.0;
	y_0 = (float)Joystick.wYpos*2/65535-1.0;
	z_0 = (float)Joystick.wZpos*2/65535-1.0;
}

void CJoyStick::GetJoyInputs()
{
	mResult=joyGetPos(JOYID,(LPJOYINFO)&Joystick);
	if(mResult!=JOYERR_NOERROR)
	{
		if(mResult==MMSYSERR_NODRIVER) 
			TRACE0("Error encountered, DRIVER problem.");
		else 
			TRACE0("JoyStick Error encountered.");
		return;
	}
	//Scaling position(0...65535 -> -1...1)
	x = (float)Joystick.wXpos*2/65535-1.0-x_0;
	y = (float)Joystick.wYpos*2/65535-1.0-y_0;
	z = (float)Joystick.wZpos*2/65535-1.0-z_0;
#define DZ 0.5
	if (x>DZ) x-=DZ; else if (x<-DZ) x+=DZ; else x=0.0;
	if (y>DZ) y-=DZ; else if (y<-DZ) y+=DZ; else y=0.0;
	if (z>DZ) z-=DZ; else if (z<-DZ) z+=DZ; else z=0.0;
	if(Joystick.wButtons&0x1) Buttons[0]=1; else Buttons[0]=0;
	if(Joystick.wButtons&0x2) Buttons[1]=1; else Buttons[1]=0;
	if(Joystick.wButtons&0x4) Buttons[2]=1; else Buttons[2]=0;
	if(Joystick.wButtons&0x8) Buttons[3]=1; else Buttons[3]=0;
}
