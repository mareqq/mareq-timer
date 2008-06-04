#include "stdafx.h"
#include "AboutDlg.h"

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)

CAboutDlg::CAboutDlg(CWnd* pParent)
    : CDialog(CAboutDlg::IDD, pParent)
{
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_TITLE, m_TitleCtrl);
    DDX_Control(pDX, IDC_STATIC_GOOGLECODE, m_GooglecodeLink);
    DDX_Control(pDX, IDC_STATIC_MAREQ_COM, m_LinkMareqCom);
    DDX_Control(pDX, IDC_STATIC_MAREQ_SK, m_MareqSk);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Title font.
    CFont *pFont = m_TitleCtrl.GetFont();

    LOGFONT lf = {0};
    pFont->GetLogFont(&lf);
    lf.lfWeight = FW_BOLD;

    m_TitleFont.CreateFontIndirect(&lf);
    m_TitleCtrl.SetFont(&m_TitleFont);

    return TRUE;
}
