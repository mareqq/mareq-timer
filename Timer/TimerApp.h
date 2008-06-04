#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "TimerWnd.h"

#define ELEMS(x) (sizeof(x)/sizeof(*x))

class CTimerApp : public CWinApp
{
public:
	CTimerApp();

protected:
    virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

protected:
    CTimerWnd m_TimerWnd;
};

extern CTimerApp theApp;