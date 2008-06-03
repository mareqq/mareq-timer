#include "stdafx.h"
#include "TimerApp.h"
#include "TimerEventDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTimerEventDlg::CTimerEventDlg(CWnd* pParent)
	: CDialog(CTimerEventDlg::IDD, pParent)
    , m_bNew(true)
    , m_BaseDate(COleDateTime::GetCurrentTime())
    , m_BaseTime(m_BaseDate)
    , m_IntervalTime(m_BaseDate.GetYear(), m_BaseDate.GetMonth(), m_BaseDate.GetDay(), 0, 0, 0)
    , m_bEnabled(TRUE)
{
}

void CTimerEventDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_MonthCalCtrl(pDX, IDC_MONTHCALENDAR, m_BaseDate);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TIME, m_BaseTime);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_REPEAT, m_IntervalTime);
    DDX_Control(pDX, ID_DELETE, m_DeleteCtrl);
    DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strMessage);
    DDX_Text(pDX, IDC_EDIT_ACTION, m_strAction);
    DDX_Control(pDX, IDOK, m_OkCtrl);
    DDX_Check(pDX, IDC_CHECK1, m_bEnabled);
}

BEGIN_MESSAGE_MAP(CTimerEventDlg, CDialog)
    ON_BN_CLICKED(ID_DELETE, &CTimerEventDlg::OnBnClickedDelete)
END_MESSAGE_MAP()

BOOL CTimerEventDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(theApp.GetIcon(), TRUE);
	SetIcon(theApp.GetIcon(), FALSE);

    if (m_bNew)
    {
        SetWindowText(_T("New Mareq Timer Event"));
        m_OkCtrl.SetWindowText(_T("&Add"));
        m_DeleteCtrl.ShowWindow(SW_HIDE);
    }
    else
    {
        SetWindowText(_T("Edit Mareq Timer Event"));
        m_OkCtrl.SetWindowText(_T("&OK"));
    }

	return TRUE;
}

void CTimerEventDlg::OnBnClickedDelete()
{
    EndDialog(ID_DELETE);
}

void CTimerEventDlg::SetTimerEvent(CTimerEvent timerEvent)
{
    m_bNew = false;

    m_bEnabled = timerEvent.GetEnabled();
    m_BaseDate = timerEvent.GetBaseDateTime();
    m_BaseTime = timerEvent.GetBaseDateTime();

    UINT interval = timerEvent.GetRepeatInterval();
    if (interval)
    {
        int hours = (int)(interval / 3600);
        int minutes = (int)((interval - (hours * 3600)) / 60);
        int seconds = (int)(interval - (hours * 3600) - (minutes * 60));

        m_IntervalTime.SetTime(hours, minutes, seconds);
    }

    m_strMessage = timerEvent.GetMessage();
    m_strAction = timerEvent.GetAction();
}

CTimerEvent CTimerEventDlg::GetTimerEvent()
{
    CTimerEvent timerEvent;
    timerEvent.SetEnabled(m_bEnabled == TRUE);
    timerEvent.SetBaseDateTime(COleDateTime(m_BaseDate.GetYear(), m_BaseDate.GetMonth(), m_BaseDate.GetDay(),
        m_BaseTime.GetHour(), m_BaseTime.GetMinute(), m_BaseTime.GetSecond()));

    int interval = (m_IntervalTime.GetHour() * 3600) + (m_IntervalTime.GetMinute() * 60) + m_IntervalTime.GetSecond();
    timerEvent.SetRepeatInterval(interval);

    timerEvent.SetMessage(m_strMessage);
    timerEvent.SetAction(m_strAction);
    return timerEvent;
}
