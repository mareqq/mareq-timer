#pragma once

class CStaticLink : public CStatic
{
    DECLARE_DYNAMIC(CStaticLink)

public:
    CStaticLink();

    COLORREF m_colorUnvisited;
    COLORREF m_colorVisited;
    bool m_bVisited;

    CString m_strLink; // If empty, caption's URL will be used.

protected:
    CFont m_Font;

    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg void OnClicked();
    DECLARE_MESSAGE_MAP()
};
