#pragma once

#include "SystemTray.h"

class CTimerDlg : public CDialog
{
public:
	CTimerDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_TIMER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;
    CSystemTray m_SystemTray;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

    afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSettings();
    afx_msg void OnExit();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

    CTime m_BaseDate;
    CTime m_BaseTime;
    CTime m_IntervalTime;

    LONGLONG m_LastElapsedSecs;
public:
    CString m_Message;
    afx_msg void OnAbout();
};
