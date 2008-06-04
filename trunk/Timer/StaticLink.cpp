#include "stdafx.h"
#include "StaticLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CStaticLink, CStatic)

BEGIN_MESSAGE_MAP(CStaticLink, CStatic)
    ON_WM_CTLCOLOR_REFLECT()
    ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
END_MESSAGE_MAP()

CStaticLink::CStaticLink()
:
    CStatic()
    , m_colorUnvisited(RGB(0,0,255)) // Blue.
    , m_colorVisited(RGB(128,0,128)) // Purple.
    , m_bVisited(false)
{
}

HBRUSH CStaticLink::CtlColor(CDC* pDC, UINT nCtlColor)
{
    ASSERT(nCtlColor == CTLCOLOR_STATIC);

    DWORD dwStyle = GetStyle();
    if (!(dwStyle & SS_NOTIFY))
    {
        // Turn on notify flag to get mouse messages and STN_CLICKED.
        // Otherwise, I will never get any mouse clicks.
        ::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | SS_NOTIFY);
    }

    HBRUSH hbr = NULL;
    if ((dwStyle & 0xFF) <= SS_RIGHT)
    {

        // Font.
        if (!(HFONT)m_Font) {
            LOGFONT lf;
            GetFont()->GetObject(sizeof(lf), &lf);
            lf.lfUnderline = TRUE;
            m_Font.CreateFontIndirect(&lf);
        }

        // Font and color.
        pDC->SelectObject(&m_Font);
        pDC->SetTextColor(m_bVisited ? m_colorVisited : m_colorUnvisited);
        pDC->SetBkMode(TRANSPARENT);

        // Return hollow brush to preserve parent background color.
        hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
    }
    return hbr;
}

void CStaticLink::OnClicked()
{
    if (m_strLink.IsEmpty())
        GetWindowText(m_strLink);

    // Run the file.
    HINSTANCE hInst = ShellExecute(NULL, _T("open"), m_strLink, NULL, NULL, SW_SHOWNORMAL);
    if ((int)hInst > 32)
    {
        m_bVisited = true;
        Invalidate();
    }
    else
    {
        MessageBeep(0);
        TRACE(_T("Unable to execute file %s\n"), m_strLink);
    }
}
