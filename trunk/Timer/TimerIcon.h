#pragma once

class CTimerIcon : public CWnd
{
	DECLARE_DYNAMIC(CTimerIcon)

public:
	CTimerIcon();
	virtual ~CTimerIcon();

    bool Create();
    bool ShowBalloon(CString strMessage);

protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnNotifyIcon(UINT message);

    afx_msg void OnAdd();
    afx_msg void OnEdit(UINT nID);
    afx_msg void OnAbout();
    afx_msg void OnExit();
    DECLARE_MESSAGE_MAP()

protected:
    NOTIFYICONDATA m_IconData;
};


