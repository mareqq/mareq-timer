#pragma once

#include "TimerEvent.h"

#define WM_APP_TIMER WM_APP
#define WM_ICON_NOTIFY WM_APP + 1

#define PROFILE_SECTION _T("")
#define PROFILE_EVENTS_COUNT _T("EventsCount")

#define SMD_NONE -3
#define SMD_ABOUT -2
#define SMD_ADD -1

class CTimerWnd : public CWnd
{
	DECLARE_DYNAMIC(CTimerWnd)

public:
	CTimerWnd();
	virtual ~CTimerWnd();

    bool Create();
    bool ShowBalloon(CString strMessage, int id);
    void CustomizeMenu(CMenu &menu);

protected:
    void LoadTimerEvents();
    void SaveTimerEvents();

    UINT_PTR ShowModalDialog(int id, CDialog &dlg);

    afx_msg void OnAdd();
    afx_msg void OnEdit(UINT nID);
    afx_msg void OnDeleteAllOld();
    afx_msg void OnAbout();
    afx_msg void OnExit();
    afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

protected:
    NOTIFYICONDATA m_NotifyIconData;
	HICON m_hIcon;

    CMap<int, int, CDialog *, CDialog *> m_Dialogs;

    CTimerEvents m_TimerEvents;
    COleDateTime m_LastTime;

    int m_BalloonId;
};


