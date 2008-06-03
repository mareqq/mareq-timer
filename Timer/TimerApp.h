#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "TimerIcon.h"
#include "TimerEvent.h"

#define WM_APP_TIMER WM_APP
#define WM_ICON_NOTIFY WM_APP + 1

#define PROFILE_SECTION _T("")
#define PROFILE_EVENTS_COUNT _T("EventsCount")

class CTimerApp : public CWinApp
{
public:
	CTimerApp();

    HICON GetIcon() { return m_hIcon; }
    CTimerIcon &GetTimerIcon() { return m_TimerIcon; }
    void CustomizeMenu(CMenu &menu);

protected:
    void LoadTimerEvents();
    void SaveTimerEvents();

    virtual BOOL InitInstance();
    virtual int ExitInstance();

    void AddTimerEvent();
    void EditTimerEvent(int id);
    void AboutApplication();

    void OnTimer(WPARAM wParam, LPARAM lParam);
    void OnNotifyIcon(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	HICON m_hIcon;
    CTimerIcon m_TimerIcon;

    CTimerEvents m_TimerEvents;
    COleDateTime m_LastTime;

    UINT_PTR m_Timer;
};

extern CTimerApp theApp;