#include "stdafx.h"
#include "resource.h"
#include "TimerWnd.h"
#include "TimerApp.h"
#include "EventDlg.h"
#include "AboutDlg.h"

#define ID_NOTIFY_ICON 1
#define WM_NOTIFY_ICON WM_APP

IMPLEMENT_DYNAMIC(CTimerWnd, CWnd)

CTimerWnd::CTimerWnd()
:
    m_hIcon(NULL),
    m_BalloonId(-1)
{
    memset(&m_NotifyIconData, 0, sizeof(m_NotifyIconData));
    m_NotifyIconData.cbSize = sizeof(m_NotifyIconData);
}

CTimerWnd::~CTimerWnd()
{
    if (m_hIcon)
        DeleteObject(m_hIcon);

    if (m_hWnd)
        Shell_NotifyIcon(NIM_DELETE, &m_NotifyIconData);
}

BEGIN_MESSAGE_MAP(CTimerWnd, CWnd)
    ON_MESSAGE(WM_NOTIFY_ICON, &CTimerWnd::OnNotifyIcon)
    ON_WM_TIMER()
    ON_COMMAND(ID_ADD, &CTimerWnd::OnAdd)
    ON_COMMAND_RANGE(1, ID_ADD - 1, &CTimerWnd::OnEdit)
    ON_COMMAND(ID_DELETE_ALL_OLD, &CTimerWnd::OnDeleteAllOld)
    ON_COMMAND(ID_ABOUT, &CTimerWnd::OnAbout)
    ON_COMMAND(ID_EXIT, &CTimerWnd::OnExit)
END_MESSAGE_MAP()

bool CTimerWnd::Create()
{
    m_hIcon = theApp.LoadIcon(IDR_ICON);

    // Create as invisible window.
    if (!CWnd::CreateEx(0, AfxRegisterWndClass(0), _T(""), WS_POPUP, 0, 0, 0, 0, NULL, 0))
        return false;

    // Create system tray icon.
    m_NotifyIconData.hWnd = m_hWnd;
    m_NotifyIconData.uID = ID_NOTIFY_ICON;
    m_NotifyIconData.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
    m_NotifyIconData.uCallbackMessage = WM_NOTIFY_ICON;
    m_NotifyIconData.hIcon = m_hIcon;
    _tcsncpy_s(m_NotifyIconData.szTip, ELEMS(m_NotifyIconData.szTip), _T("Mareq Timer"), ELEMS(m_NotifyIconData.szTip) - 1);
    m_NotifyIconData.uVersion = NOTIFYICON_VERSION;

    if (!Shell_NotifyIcon(NIM_ADD, &m_NotifyIconData))
        return false;

    // Load events.
    LoadTimerEvents();

    // Set timer.
    if (!SetTimer(WM_APP_TIMER, 1000, NULL))
        return false;

    return true;
}

void CTimerWnd::LoadTimerEvents()
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

void CTimerWnd::SaveTimerEvents()
{
    int count = (int)m_TimerEvents.GetCount();
    theApp.WriteProfileInt(PROFILE_SECTION, PROFILE_EVENTS_COUNT, count);

    for (int i = 0; i < count; i++)
        m_TimerEvents[i].Save(i);
}

LRESULT CTimerWnd::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
    if (wParam != ID_NOTIFY_ICON)
        return 0;

    UINT message = LOWORD(lParam);
    if (message == WM_RBUTTONUP)
    {
        // Setup menu.
        CMenu menu;
        if (!menu.LoadMenu(IDR_MENU))
            return 0;

        CMenu *pSubMenu = menu.GetSubMenu(0);
        if (!pSubMenu)
            return 0;

        CustomizeMenu(*pSubMenu);
        pSubMenu->SetDefaultItem(ID_ADD, FALSE);

        // Display menu.
        CPoint point;
        GetCursorPos(&point);

        SetForegroundWindow();
        pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
        PostMessage(WM_NULL);
    }
    else if (message == WM_LBUTTONDBLCLK)
    {
        PostMessage(WM_COMMAND, ID_ADD);
    }
    else if (message == NIN_BALLOONUSERCLICK)
    {
        if (m_BalloonId >= 0)
            OnEdit(m_BalloonId + 1);
    }

    return 1;
}

void CTimerWnd::OnAdd()
{
    CTimerEvent timerEvent;

    CEventDlg dlg(*this);
    dlg.SetAdd(m_hIcon);

    SetForegroundWindow();
    if (ShowModalDialog(SMD_ADD, dlg) == IDOK)
    {
        m_TimerEvents.Add(dlg.GetTimerEvent());
        SaveTimerEvents();
    }
}

void CTimerWnd::OnEdit(UINT nID)
{
    int id = nID - 1;
    if (id < 0 || id >= m_TimerEvents.GetCount())
        return;

    CEventDlg dlg(*this);
    dlg.SetEdit(m_hIcon, m_TimerEvents[id]);

    SetForegroundWindow();
    INT_PTR retVal = ShowModalDialog(id, dlg);
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

void CTimerWnd::OnDeleteAllOld()
{
    for (int i = 0; i < m_TimerEvents.GetCount(); i++)
    {
        if (m_TimerEvents[i].IsOld())
        {
            m_TimerEvents.RemoveAt(i--);
            continue;
        }
    }
    SaveTimerEvents();
}

void CTimerWnd::OnAbout()
{
    CAboutDlg dlg;

    SetForegroundWindow();
    ShowModalDialog(SMD_ABOUT, dlg);
}

void CTimerWnd::OnExit()
{
    PostMessage(WM_QUIT);
}

void CTimerWnd::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == WM_APP_TIMER)
    {
        COleDateTime currentTime = COleDateTime::GetCurrentTime();

        if (m_LastTime != COleDateTime())
        {
            for (int i = 0; i < m_TimerEvents.GetCount(); i++)
                m_TimerEvents[i].Process(m_LastTime, currentTime, *this, i);
        }

        m_LastTime = currentTime;
    }

    CWnd::OnTimer(nIDEvent);
}

bool CTimerWnd::ShowBalloon(CString strMessage, int id)
{
    m_BalloonId = id;

    m_NotifyIconData.uFlags = NIF_INFO;
    _tcsncpy_s(m_NotifyIconData.szInfo, ELEMS(m_NotifyIconData.szInfo), strMessage, ELEMS(m_NotifyIconData.szInfo) - 1);
    m_NotifyIconData.uTimeout = 10000;
    _tcsncpy_s(m_NotifyIconData.szInfoTitle, ELEMS(m_NotifyIconData.szInfoTitle), _T("Mareq Timer"), ELEMS(m_NotifyIconData.szInfoTitle) - 1);
    m_NotifyIconData.dwInfoFlags = NIIF_INFO;

    return Shell_NotifyIcon(NIM_MODIFY, &m_NotifyIconData) == TRUE;
}

void CTimerWnd::CustomizeMenu(CMenu &menu)
{
    // Events.
    for (int i = (int)m_TimerEvents.GetCount() - 1; i >= 0; i--)
    {
        CTimerEvent &timerEvent = m_TimerEvents[i];

        // Caption.
        CString strCaption;
        if (i < 10)
            strCaption.Format(_T("&%d %s"), i < 9 ? i + 1 : 0, timerEvent.GetDescription());
        else
            strCaption.Format(_T("   %s"), timerEvent.GetDescription());
        menu.InsertMenu(1, MF_BYPOSITION|MF_STRING, i + 1, strCaption);

        // Check.
        menu.CheckMenuItem(1, MF_BYPOSITION|(timerEvent.GetEnabled() ? MF_CHECKED : MF_UNCHECKED));
    }

    // Enable delete all old.
    UINT nEnable = MF_DISABLED|MF_GRAYED;
    for (int i = 0; i < m_TimerEvents.GetCount(); i++)
    {
        if (m_TimerEvents[i].IsOld())
        {
            nEnable = MF_ENABLED;
            break;
        }
    }

    menu.EnableMenuItem(ID_DELETE_ALL_OLD, MF_BYCOMMAND|nEnable);
}

UINT_PTR CTimerWnd::ShowModalDialog(int id, CDialog &dlg)
{
    CDialog *pDlg = NULL;
    if (m_Dialogs.Lookup(id, pDlg))
    {
        pDlg->SetForegroundWindow();
        return IDCANCEL;
    }
    else
    {
        m_Dialogs[id] = &dlg;
        UINT_PTR retVal = dlg.DoModal();
        m_Dialogs.RemoveKey(id);
        return retVal;
    }
}
