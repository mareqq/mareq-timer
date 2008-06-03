#include "stdafx.h"
#include "TimerApp.h"
#include "TimerEventDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTimerApp theApp;

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    theApp.PostThreadMessage(WM_APP_TIMER, 0, 0);
}

BEGIN_MESSAGE_MAP(CTimerApp, CWinApp)
    ON_THREAD_MESSAGE(WM_APP_TIMER, &CTimerApp::OnTimer)
    ON_THREAD_MESSAGE(WM_ICON_NOTIFY, &CTimerApp::OnNotifyIcon)
END_MESSAGE_MAP()

CTimerApp::CTimerApp()
:
    m_hIcon(NULL),
    m_Timer(0)
{
}

BOOL CTimerApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

    // Registry
	SetRegistryKey(_T("Mareq"));

    // System tray icon
    m_hIcon = LoadIcon(IDR_ICON);
    m_TimerIcon.Create();

    // Timer
    LoadTimerEvents();
    if ((m_Timer = SetTimer(NULL, NULL, 1000, TimerProc)) == 0)
        throw _T("Timer creation failed");

    m_pMainWnd = &m_TimerIcon;
    return TRUE;
}

int CTimerApp::ExitInstance()
{
    if (m_Timer)
        KillTimer(NULL, m_Timer);

    return CWinApp::ExitInstance();
}

void CTimerApp::LoadTimerEvents()
{
    int count = theApp.GetProfileInt(PROFILE_SECTION, PROFILE_EVENTS_COUNT, 0);

    m_TimerEvents.RemoveAll();
    for (int i = 0; i < count; i++)
    {
        CTimerEvent timerEvent;
        if (timerEvent.Load(i))
            m_TimerEvents.Add(timerEvent);
    }
}

void CTimerApp::SaveTimerEvents()
{
    int count = (int)m_TimerEvents.GetCount();
    theApp.WriteProfileInt(PROFILE_SECTION, PROFILE_EVENTS_COUNT, count);

    for (int i = 0; i < count; i++)
        m_TimerEvents[i].Save(i);
}

void CTimerApp::OnTimer(WPARAM wParam, LPARAM lParam)
{
    COleDateTime currentTime = COleDateTime::GetCurrentTime();

    if (m_LastTime != COleDateTime())
    {
        for (int i = 0; i < m_TimerEvents.GetCount(); i++)
            m_TimerEvents[i].Process(m_LastTime, currentTime);
    }

    m_LastTime = currentTime;
}

void CTimerApp::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
    if (wParam == WM_COMMAND)
    {
        switch (lParam)
        {
        case ID_ADD:
            AddTimerEvent();
            break;
        case ID_ABOUT:
            AboutApplication();
            break;
        case ID_EXIT:
            PostThreadMessage(WM_QUIT, 0, 0);
            break;
        default:
            EditTimerEvent((int)lParam - 1);
            break;
        }
    }
}

void CTimerApp::AddTimerEvent()
{
    CTimerEvent timerEvent;

    CTimerEventDlg dlg;
    if (dlg.DoModal() == IDOK)
    {
        m_TimerEvents.Add(dlg.GetTimerEvent());
        SaveTimerEvents();
    }
}

void CTimerApp::EditTimerEvent(int id)
{
    if (id < 0 || id >= m_TimerEvents.GetCount())
        return;

    CTimerEventDlg dlg;
    dlg.SetTimerEvent(m_TimerEvents[id]);
    INT_PTR retVal = dlg.DoModal();
    if (retVal == IDOK)
    {
        m_TimerEvents[id] = dlg.GetTimerEvent();
        SaveTimerEvents();
    }
    else if (retVal == ID_DELETE)
    {
        m_TimerEvents.RemoveAt(id);
        SaveTimerEvents();
    }
}

void CTimerApp::AboutApplication()
{
    CAboutDlg dlg;
    dlg.DoModal();
}

void CTimerApp::CustomizeMenu(CMenu &menu)
{
    for (int i = (int)m_TimerEvents.GetCount() - 1; i >= 0; i--)
    {
        CTimerEvent &timerEvent = m_TimerEvents[i];

        CString strBase = timerEvent.GetBaseDateTime().Format();

        CString strInterval;
        if (timerEvent.GetRepeatInterval())
            strInterval.Format(_T(" %d"), timerEvent.GetRepeatInterval());

        CString strMessage;
        if (timerEvent.GetMessage())
            strMessage.Format(_T(" %s"), timerEvent.GetMessage());

        CString strAction;
        if (timerEvent.GetAction())
            strAction.Format(_T(" %s"), timerEvent.GetAction());

        CString strName = strBase + strInterval + strMessage + strAction;
        menu.InsertMenu(1, MF_BYPOSITION|MF_STRING, i + 1, strName);
        menu.CheckMenuItem(1, MF_BYPOSITION|(timerEvent.GetEnabled() ? MF_CHECKED : MF_UNCHECKED));
    }
}
