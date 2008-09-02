#include "stdafx.h"
#include "TimerApp.h"
#include "EventDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEventDlg::CEventDlg(CTimerWnd &timerWnd, CWnd* pParent)
	: CDialog(CEventDlg::IDD, pParent)
    , m_bNew(true)
    , m_hIcon(NULL)
    , m_TimerWnd(timerWnd)
    , m_bEnabled(TRUE)
    , m_BaseDate(COleDateTime::GetCurrentTime())
    , m_BaseTime(m_BaseDate)
    , m_IntervalTime(m_BaseDate)
    , m_strMessage(_T("Timer event raised!\r\nDelete this message text to show no message."))
{
    m_IntervalTime.SetTime(0, 0, 0);
}

void CEventDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_MonthCalCtrl(pDX, IDC_MONTHCALENDAR, m_BaseDate);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TIME, m_BaseTime);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_REPEAT, m_IntervalTime);
    DDX_Check(pDX, IDC_CHECK_ENABLED, m_bEnabled);
    DDX_Control(pDX, ID_DELETE, m_DeleteCtrl);
    DDX_Control(pDX, IDOK, m_OkCtrl);
    DDX_Control(pDX, IDC_DATETIMEPICKER_REPEAT, m_IntervalTimeCtrl);
    DDX_Control(pDX, IDC_EDIT_ACTION, m_ActionCtrl);
    DDX_Control(pDX, IDC_EDIT_ACTION_PARAMS, m_ActionParamsCtrl);
    DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strMessage);
    DDX_Text(pDX, IDC_EDIT_ACTION, m_strAction);
    DDX_Text(pDX, IDC_EDIT_ACTION_PARAMS, m_strActionParams);
    DDX_CBString(pDX, IDC_COMBO_RING_TONE, m_strRingTone);
    DDX_Control(pDX, IDC_COMBO_RING_TONE, m_RingTone);
}

BEGIN_MESSAGE_MAP(CEventDlg, CDialog)
    ON_BN_CLICKED(ID_DELETE, &CEventDlg::OnBnClickedDelete)
    ON_BN_CLICKED(ID_TEST, &CEventDlg::OnBnClickedTest)
    ON_EN_CHANGE(IDC_EDIT_ACTION, &CEventDlg::OnEnChangeEditAction)
    ON_BN_CLICKED(IDC_BUTTON_FILE, &CEventDlg::OnBnClickedButtonFile)
END_MESSAGE_MAP()

BOOL CEventDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

    m_RingTone.AddString(RINGTONE_DEFAULT);
    m_RingTone.AddString(RINGTONE_BEEP);
    m_RingTone.AddString(RINGTONE_ASTERISK);
    m_RingTone.AddString(RINGTONE_EXCLAMATION);
    m_RingTone.AddString(RINGTONE_HAND);
    m_RingTone.AddString(RINGTONE_QUESTION);

    if (m_bNew)
    {
        SetWindowText(_T("New Mareq Timer Event"));
        m_OkCtrl.SetWindowText(_T("&Add"));
        m_DeleteCtrl.ShowWindow(SW_HIDE);

        // To force 00:00:00 time it the control.
        m_IntervalTimeCtrl.SetTime(m_IntervalTime);
        m_IntervalTime.SetStatus(COleDateTime::null);
        m_IntervalTimeCtrl.SetTime(m_IntervalTime);
    }
    else
    {
        SetWindowText(_T("Edit Mareq Timer Event"));
        m_OkCtrl.SetWindowText(_T("&OK"));
    }

    OnEnChangeEditAction();
	return TRUE;
}

void CEventDlg::OnBnClickedTest()
{
    if (!UpdateData(TRUE))
        return;

    GetTimerEvent().Activate(m_TimerWnd, SMD_NONE);
}


void CEventDlg::OnBnClickedDelete()
{
    EndDialog(ID_DELETE);
}

void CEventDlg::SetAdd(HICON hIcon)
{
    m_hIcon = hIcon;
    m_strRingTone = RINGTONE_DEFAULT;
}

void CEventDlg::SetEdit(HICON hIcon, CTimerEvent timerEvent)
{
    m_bNew = false;
    m_hIcon = hIcon;

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
    m_strActionParams = timerEvent.GetActionParams();
    m_strRingTone = timerEvent.GetRingTone();
}

CTimerEvent CEventDlg::GetTimerEvent()
{
    CTimerEvent timerEvent;
    timerEvent.SetEnabled(m_bEnabled == TRUE);
    timerEvent.SetBaseDateTime(COleDateTime(m_BaseDate.GetYear(), m_BaseDate.GetMonth(), m_BaseDate.GetDay(),
        m_BaseTime.GetHour(), m_BaseTime.GetMinute(), m_BaseTime.GetSecond()));

    int interval = m_IntervalTime.GetStatus() == COleDateTime::valid ?
        (m_IntervalTime.GetHour() * 3600) + (m_IntervalTime.GetMinute() * 60) + m_IntervalTime.GetSecond() : 0;
    timerEvent.SetRepeatInterval(interval);

    timerEvent.SetMessage(m_strMessage);
    timerEvent.SetAction(m_strAction);
    timerEvent.SetActionParams(m_strActionParams);
    timerEvent.SetRingTone(m_strRingTone);
    return timerEvent;
}

void CEventDlg::OnEnChangeEditAction()
{
    CString str;
    m_ActionCtrl.GetWindowText(str);
    m_ActionParamsCtrl.EnableWindow(!str.IsEmpty());
}

void CEventDlg::OnBnClickedButtonFile()
{
    CFileDialog dlg(TRUE);
    if (dlg.DoModal() == IDOK)
    {
        m_strRingTone = dlg.GetPathName();
        UpdateData(FALSE);
    }
}
