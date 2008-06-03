#include "stdafx.h"
#include "TimerApp.h"
#include "TimerDlg.h"
#include "AboutDlg.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_ICON_NOTIFY  WM_APP + 1
#define ID_TIMER 1

CTimerDlg::CTimerDlg(CWnd* pParent)
	: CDialog(CTimerDlg::IDD, pParent)
    , m_BaseDate(CTime::GetCurrentTime())
    , m_BaseTime(CTime::GetCurrentTime())
    , m_IntervalTime(m_BaseDate.GetYear(), m_BaseDate.GetMonth(), m_BaseDate.GetDay(), 1, 0, 0)
    , m_Message("Interval has elapsed...")
    , m_LastElapsedSecs(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTimerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TIME, m_BaseTime);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_TIME2, m_IntervalTime);
    DDX_MonthCalCtrl(pDX, IDC_MONTHCALENDAR, m_BaseDate);
    DDX_Text(pDX, IDC_EDIT_MESSAGE, m_Message);
}

BEGIN_MESSAGE_MAP(CTimerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_COMMAND(ID_SETTINGS, &CTimerDlg::OnSettings)
    ON_COMMAND(ID_EXIT, &CTimerDlg::OnExit)
    ON_COMMAND(ID_ABOUT, &CTimerDlg::OnAbout)
END_MESSAGE_MAP()

BOOL CTimerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

    m_SystemTray.Create(NULL, WM_ICON_NOTIFY, "Mareq Timer", m_hIcon, IDR_MENU);
    m_SystemTray.RemoveTaskbarIcon(this);
    m_SystemTray.SetMenuDefaultItem(ID_SETTINGS, FALSE);

    tm time;
    m_BaseDate = theApp.GetProfileInt("", "BaseDate", (int)mktime(m_BaseDate.GetLocalTm(&time)));
    m_BaseTime = theApp.GetProfileInt("", "BaseTime", (int)mktime(m_BaseTime.GetLocalTm(&time)));
    m_IntervalTime = theApp.GetProfileInt("", "IntervalTime", (int)mktime(m_IntervalTime.GetLocalTm(&time)));
    m_Message = theApp.GetProfileString("", "Message", m_Message);
    UpdateData(FALSE);

    SetTimer(ID_TIMER, 1000, NULL);
	return TRUE;
}

void CTimerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CTimerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTimerDlg::OnSettings()
{
    m_SystemTray.MaximiseFromTray(this);
}

void CTimerDlg::OnExit()
{
    EndDialog(IDCANCEL);
}

void CTimerDlg::OnOK()
{
    UpdateData(TRUE);
    m_SystemTray.MinimiseToTray(this);

    tm time;
    theApp.WriteProfileInt("", "BaseDate", (int)mktime(m_BaseDate.GetLocalTm(&time)));
    theApp.WriteProfileInt("", "BaseTime", (int)mktime(m_BaseTime.GetLocalTm(&time)));
    theApp.WriteProfileInt("", "IntervalTime", (int)mktime(m_IntervalTime.GetLocalTm(&time)));
    theApp.WriteProfileString("", "Message", m_Message);

    m_LastElapsedSecs = 0;
}

void CTimerDlg::OnCancel()
{
    UpdateData(FALSE);
    m_SystemTray.MinimiseToTray(this);
}

void CTimerDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == ID_TIMER)
    {
        CTimeSpan elapsed = CTime::GetCurrentTime() - CTime(m_BaseDate.GetYear(), m_BaseDate.GetMonth(),
            m_BaseDate.GetDay(), m_BaseTime.GetHour(), m_BaseTime.GetMinute(), m_BaseTime.GetSecond());

        LONGLONG elapsedSecs = elapsed.GetTotalSeconds();

        if (m_LastElapsedSecs)
        {
            LONGLONG intervalSecs = m_IntervalTime.GetHour() * 3600 + m_IntervalTime.GetMinute() * 60
                + m_IntervalTime.GetSecond();

            double last = (double)m_LastElapsedSecs / (double)intervalSecs;
            double current = (double)elapsedSecs / (double)intervalSecs;
            if (floor(last) < floor(current))
            {
                m_SystemTray.ShowBalloon(m_Message, "Mareq Timer", NIIF_INFO);
            }
        }

        m_LastElapsedSecs = elapsedSecs;
    }
}

void CTimerDlg::OnAbout()
{
    CAboutDlg dlg(this);
    dlg.DoModal();
}
