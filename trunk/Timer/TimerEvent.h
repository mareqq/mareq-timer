#pragma once

#define PROFILE_SECTION_EVENT _T("Event %d")
#define PROFILE_EVENT_ENABLED _T("Enabled")
#define PROFILE_EVENT_BASE _T("BaseDateTime")
#define PROFILE_EVENT_REPEAT _T("RepeatInterval")
#define PROFILE_EVENT_MESSAGE _T("Message")
#define PROFILE_EVENT_ACTION _T("Action")

#define DESCRIPTION_MESSAGE_MAX 30
#define DESCRIPTION_ACTION_MAX 30

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

    bool GetEnabled() { return m_bEnabled; }
    COleDateTime GetBaseDateTime() { return m_BaseDateTime; }
    UINT GetRepeatInterval() { return m_RepeatInterval; }
    CString GetMessage() { return m_strMessage; }
    CString GetAction() { return m_strAction; }

    bool Load(int id);
    void Save(int id);
    bool IsValid();

    CString GetDescription();
    void Process(COleDateTime &lastTime, COleDateTime &currentTime, CTimerWnd &timerWnd, int id);
    void Activate(CTimerWnd &timerWnd, int id);

protected:
    bool m_bEnabled;

    COleDateTime m_BaseDateTime;
    UINT m_RepeatInterval;

    CString m_strMessage;
    CString m_strAction;

    friend class CArray<CTimerEvent>;
};

typedef CArray<CTimerEvent> CTimerEvents;

