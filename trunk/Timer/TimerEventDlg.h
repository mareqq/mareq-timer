#pragma once

#include "resource.h"
#include "TimerEvent.h"
#include "afxdtctl.h"
#include "afxwin.h"

class CTimerEventDlg : public CDialog
{
public:
	CTimerEventDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_TIMER_EVENT };

    void SetTimerEvent(CTimerEvent timerEvent);
    CTimerEvent GetTimerEvent();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

    afx_msg void OnBnClickedDelete();
	DECLARE_MESSAGE_MAP()

protected:
    CButton m_DeleteCtrl;
    CButton m_OkCtrl;

    bool m_bNew;

    BOOL m_bEnabled;
    COleDateTime m_BaseDate;
    COleDateTime m_BaseTime;
    COleDateTime m_IntervalTime;
    CString m_strMessage;
    CString m_strAction;
};
