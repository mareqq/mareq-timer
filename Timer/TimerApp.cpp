#include "stdafx.h"
#include "TimerApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTimerApp theApp;

BEGIN_MESSAGE_MAP(CTimerApp, CWinApp)
END_MESSAGE_MAP()

CTimerApp::CTimerApp()
{
}

BOOL CTimerApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

    // Registry.
	SetRegistryKey(_T("Mareq"));

    // Main window.
    m_TimerWnd.Create();
    m_pMainWnd = &m_TimerWnd;

    return TRUE;
}
