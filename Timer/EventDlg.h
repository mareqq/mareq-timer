#pragma once

#include "resource.h"
#include "TimerEvent.h"
#include "afxdtctl.h"
#include "afxwin.h"

class CEventDlg : public CDialog
{
public:
	CEventDlg(CTimerWnd &timerWnd, CWnd* pParent = NULL);

	enum { IDD = IDD_EVENT };

    void SetAdd(HICON hIcon);
    void SetEdit(HICON hIcon, CTimerEvent timerEvent);
    CTimerEvent GetTimerEvent();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

    afx_msg void OnBnClickedTest();
    afx_msg void OnBnClickedDelete();
    afx_msg void OnEnChangeEditAction();
    afx_msg void OnBnClickedButtonFile();
	DECLARE_MESSAGE_MAP()

protected:
    CDateTimeCtrl m_IntervalTimeCtrl;
    CEdit m_ActionCtrl;
    CEdit m_ActionParamsCtrl;
    CButton m_DeleteCtrl;
    CButton m_OkCtrl;
    CComboBox m_RingTone;

    bool m_bNew;
    HICON m_hIcon;
    CTimerWnd &m_TimerWnd;

    BOOL m_bEnabled;
    COleDateTime m_BaseDate;
    COleDateTime m_BaseTime;
    COleDateTime m_IntervalTime;
    CString m_strMessage;
    CString m_strAction;
    CString m_strActionParams;
    CString m_strRingTone;
};
