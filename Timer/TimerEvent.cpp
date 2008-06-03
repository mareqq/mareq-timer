#include "stdafx.h"
#include "TimerEvent.h"
#include "TimerApp.h"
#include "math.h"

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
}

bool CTimerEvent::IsValid()
{
    return m_BaseDateTime.GetStatus() == COleDateTime::valid
        || m_BaseDateTime.GetStatus() == COleDateTime::null;
}

void CTimerEvent::Process(COleDateTime &lastTime, COleDateTime &currentTime)
{
    if (m_BaseDateTime > lastTime ||
        m_BaseDateTime > currentTime)
        return;

    COleDateTimeSpan lastElapsed = lastTime - m_BaseDateTime;
    COleDateTimeSpan currentElapsed = currentTime - m_BaseDateTime;

    double last = (double)lastElapsed.GetTotalSeconds() / (double)m_RepeatInterval;
    double current = (double)currentElapsed.GetTotalSeconds() / (double)m_RepeatInterval;
    if (floor(last) < floor(current))
        Activate();
}

void CTimerEvent::Activate()
{
    if (!m_strMessage.IsEmpty())
    {
        theApp.GetTimerIcon().ShowBalloon(m_strMessage);
    }

    if (!m_strAction.IsEmpty())
    {
    }
}
