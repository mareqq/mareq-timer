#include "stdafx.h"
#include "resource.h"
#include "TimerIcon.h"
#include "TimerApp.h"

#define ID_NOTIFYICON 1
#define WM_NOTIFYICON WM_APP

IMPLEMENT_DYNAMIC(CTimerIcon, CWnd)

CTimerIcon::CTimerIcon()
{
    memset(&m_IconData, 0, sizeof(m_IconData));
    m_IconData.cbSize = sizeof(m_IconData);
}

CTimerIcon::~CTimerIcon()
{
    if (m_hWnd)
    {
        Shell_NotifyIcon(NIM_DELETE, &m_IconData);
    }
}

BEGIN_MESSAGE_MAP(CTimerIcon, CWnd)
    ON_COMMAND(ID_ADD, &CTimerIcon::OnAdd)
    ON_COMMAND_RANGE(1, ID_ADD - 1, &CTimerIcon::OnEdit)
    ON_COMMAND(ID_ABOUT, &CTimerIcon::OnAbout)
    ON_COMMAND(ID_EXIT, &CTimerIcon::OnExit)
END_MESSAGE_MAP()

bool CTimerIcon::Create()
{
    // Create as invisible window.
    CWnd::CreateEx(0, AfxRegisterWndClass(0), _T(""), WS_POPUP, 0, 0, 0, 0, NULL, 0);

    // Create system tray icon.
    if (m_hWnd)
    {
        m_IconData.hWnd = m_hWnd;
        m_IconData.uID = ID_NOTIFYICON;
        m_IconData.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
        m_IconData.uCallbackMessage = WM_NOTIFYICON;
        m_IconData.hIcon = theApp.GetIcon();
        _tcscpy(m_IconData.szTip, _T("Mareq Timer"));
        m_IconData.uVersion = NOTIFYICON_VERSION;

        Shell_NotifyIcon(NIM_ADD, &m_IconData);
    }

    return m_hWnd != NULL;
}

bool CTimerIcon::ShowBalloon(CString strMessage)
{
    m_IconData.uFlags = NIF_INFO;
    _tcsncpy(m_IconData.szInfo, strMessage, sizeof(m_IconData.szInfo));
    m_IconData.uTimeout = 10000;
    _tcscpy(m_IconData.szInfoTitle, _T("Mareq Timer"));
    m_IconData.dwInfoFlags = NIIF_INFO;

    return Shell_NotifyIcon(NIM_MODIFY, &m_IconData) == TRUE;
}

LRESULT CTimerIcon::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_NOTIFYICON && wParam == ID_NOTIFYICON)
        return OnNotifyIcon(LOWORD(lParam));

    return CWnd::WindowProc(message, wParam, lParam);
}

LRESULT CTimerIcon::OnNotifyIcon(UINT message)
{
    if (message == WM_RBUTTONUP)
    {
        // Setup menu.
        CMenu menu;
        if (!menu.LoadMenu(IDR_MENU))
            return 0;

        CMenu *pSubMenu = menu.GetSubMenu(0);
        if (!pSubMenu)
            return 0;

        theApp.CustomizeMenu(*pSubMenu);
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
        theApp.PostThreadMessage(WM_ICON_NOTIFY, WM_COMMAND, ID_ADD);
    }

    return 1;
}

void CTimerIcon::OnAdd()
{
    theApp.PostThreadMessage(WM_ICON_NOTIFY, WM_COMMAND, ID_ADD);
}

void CTimerIcon::OnEdit(UINT nID)
{
    theApp.PostThreadMessage(WM_ICON_NOTIFY, WM_COMMAND, nID);
}
void CTimerIcon::OnAbout()
{
    theApp.PostThreadMessage(WM_ICON_NOTIFY, WM_COMMAND, ID_ABOUT);
}

void CTimerIcon::OnExit()
{
    theApp.PostThreadMessage(WM_ICON_NOTIFY, WM_COMMAND, ID_EXIT);
}
