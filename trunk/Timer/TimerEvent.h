#pragma once

#define PROFILE_SECTION_EVENT _T("Event %d")
#define PROFILE_EVENT_ENABLED _T("Enabled")
#define PROFILE_EVENT_BASE _T("BaseDateTime")
#define PROFILE_EVENT_REPEAT _T("RepeatInterval")
#define PROFILE_EVENT_MESSAGE _T("Message")
#define PROFILE_EVENT_ACTION _T("Action")
#define PROFILE_EVENT_ACTION_PARAMS _T("ActionParams")
#define PROFILE_EVENT_RING_TONE _T("RingTone")

#define DESCRIPTION_MESSAGE_MAX 30
#define DESCRIPTION_ACTION_MAX 30

#define RINGTONE_DEFAULT _T("Default")
#define RINGTONE_BEEP _T("System Beep")
#define RINGTONE_ASTERISK _T("System Asterisk")
#define RINGTONE_EXCLAMATION _T("System Exclamation")
#define RINGTONE_HAND _T("System Hand")
#define RINGTONE_QUESTION _T("System Question")

class CTimerWnd;

class CTimerEvent
{
public:
    CTimerEvent();
    virtual ~CTimerEvent();

    void SetEnabled(bool bEnabled) { m_bEnabled = bEnabled; }
    void SetBaseDateTime(COleDateTime dateTime) { m_BaseDateTime = dateTime; }
    void SetRepeatInterval(UINT interval) { m_RepeatInterval = interval; }
    void SetMessage(CString strMessage) { m_strMessage = strMessage; }
    void SetAction(CString strAction) { m_strAction = strAction; }
    void SetActionParams(CString strActionParams) { m_strActionParams = strActionParams; }
    void SetRingTone(CString strRingTone) { m_strRingTone = strRingTone; }

    bool GetEnabled() { return m_bEnabled; }
    COleDateTime GetBaseDateTime() { return m_BaseDateTime; }
    UINT GetRepeatInterval() { return m_RepeatInterval; }
    CString GetMessage() { return m_strMessage; }
    CString GetAction() { return m_strAction; }
    CString GetActionParams() { return m_strActionParams; }
    CString GetRingTone() { return m_strRingTone; }

    bool Load(int id);
    void Save(int id);
    bool IsValid();
    bool IsOld();

    CString GetDescription();
    void Process(COleDateTime &lastTime, COleDateTime &currentTime, CTimerWnd &timerWnd, int id);
    void Activate(CTimerWnd &timerWnd, int id);

protected:
    bool m_bEnabled;

    COleDateTime m_BaseDateTime;
    UINT m_RepeatInterval;

    CString m_strMessage;
    CString m_strAction;
    CString m_strActionParams;
    CString m_strRingTone;

    friend class CArray<CTimerEvent>;
};

typedef CArray<CTimerEvent> CTimerEvents;

