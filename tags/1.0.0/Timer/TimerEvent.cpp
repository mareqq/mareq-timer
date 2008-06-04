#include "stdafx.h"
#include "TimerEvent.h"
#include "TimerApp.h"
#include "math.h"

#pragma warning(disable: 4311)

CTimerEvent::CTimerEvent()
:
    m_bEnabled(true),
    m_BaseDateTime(COleDateTime::GetCurrentTime()),
    m_RepeatInterval(0)
{
}

CTimerEvent::~CTimerEvent()
{
}

bool CTimerEvent::Load(int id)
{
    CString strSection;
    strSection.Format(PROFILE_SECTION_EVENT, id);

    m_bEnabled = theApp.GetProfileInt(strSection, PROFILE_EVENT_ENABLED, 1) != 0;
    m_BaseDateTime.ParseDateTime(theApp.GetProfileString(strSection, PROFILE_EVENT_BASE), 0, LANG_ENGLISH);
    m_RepeatInterval = theApp.GetProfileInt(strSection, PROFILE_EVENT_REPEAT, 0);
    m_strMessage = theApp.GetProfileString(strSection, PROFILE_EVENT_MESSAGE);
    m_strAction = theApp.GetProfileString(strSection, PROFILE_EVENT_ACTION);
    m_strActionParams = theApp.GetProfileString(strSection, PROFILE_EVENT_ACTION_PARAMS);

    return IsValid();
}

void CTimerEvent::Save(int id)
{
    CString strSection;
    strSection.Format(PROFILE_SECTION_EVENT, id);

    theApp.WriteProfileInt(strSection, PROFILE_EVENT_ENABLED, m_bEnabled ? 1 : 0);
    theApp.WriteProfileString(strSection, PROFILE_EVENT_BASE, m_BaseDateTime.Format(0, LANG_ENGLISH));
    theApp.WriteProfileInt(strSection, PROFILE_EVENT_REPEAT, m_RepeatInterval);
    theApp.WriteProfileString(strSection, PROFILE_EVENT_MESSAGE, m_strMessage);
    theApp.WriteProfileString(strSection, PROFILE_EVENT_ACTION, m_strAction);
    theApp.WriteProfileString(strSection, PROFILE_EVENT_ACTION_PARAMS, m_strActionParams);
}

bool CTimerEvent::IsValid()
{
    return m_BaseDateTime.GetStatus() == COleDateTime::valid;
}

CString CTimerEvent::GetDescription()
{
    CString strBase;
    strBase.Format(_T("At %s"), m_BaseDateTime.Format());

    CString strInterval;
    if (m_RepeatInterval)
        strInterval.Format(_T(" (and every %d-th second)"), m_RepeatInterval);

    CString strMessage;
    if (!m_strMessage.IsEmpty())
    {
        strMessage.Format(_T(" show \"%s%s\""), m_strMessage.Left(DESCRIPTION_MESSAGE_MAX),
            m_strMessage.GetLength() > DESCRIPTION_MESSAGE_MAX ? _T("...") : _T(""));
    }

    CString strAnd;
    if (!m_strMessage.IsEmpty() && !m_strAction.IsEmpty())
        strAnd = _T(" and");

    CString strAction;
    if (!m_strAction.IsEmpty())
    {
        strAction.Format(_T(" run \"%s%s\""), m_strAction.GetLength() > DESCRIPTION_ACTION_MAX ? _T("...") : _T(""),
            m_strAction.Right(DESCRIPTION_ACTION_MAX));
    }

    return strBase + strInterval + strMessage + strAnd + strAction + _T(".");
}

void CTimerEvent::Process(COleDateTime &lastTime, COleDateTime &currentTime, CTimerWnd &timerWnd, int id)
{
    // Handle repeat interval.
    if (m_RepeatInterval &&
        m_BaseDateTime < lastTime &&
        m_BaseDateTime < currentTime)
    {
        COleDateTimeSpan lastElapsed = lastTime - m_BaseDateTime;
        COleDateTimeSpan currentElapsed = currentTime - m_BaseDateTime;

        double last = (double)lastElapsed.GetTotalSeconds() / (double)m_RepeatInterval;
        double current = (double)currentElapsed.GetTotalSeconds() / (double)m_RepeatInterval;
        if (floor(last) < floor(current))
        {
            Activate(timerWnd, id);
            return;
        }
    }

    // Handle base date time.
    if (m_BaseDateTime > lastTime &&
        m_BaseDateTime <= currentTime)
        Activate(timerWnd, id);
}

void CTimerEvent::Activate(CTimerWnd &timerWnd, int id)
{
    // Show message.
    if (!m_strMessage.IsEmpty())
    {
        timerWnd.ShowBalloon(m_strMessage, id);
    }

    // Run action.
    if (!m_strAction.IsEmpty())
    {
        HINSTANCE hInst = ShellExecute(NULL, _T("open"), m_strAction, m_strActionParams, NULL, 0);
        if ((int)hInst <= 32)
        {
            CString str;
            str.Format(_T("Unable to run \"%s%s\""), m_strAction.GetLength() > DESCRIPTION_ACTION_MAX ? _T("...") : _T(""),
                m_strAction.Right(DESCRIPTION_ACTION_MAX));

            timerWnd.ShowBalloon(str, id);
        }
    }
}

bool CTimerEvent::IsOld()
{
    return !m_RepeatInterval && m_BaseDateTime < COleDateTime::GetCurrentTime();
}

