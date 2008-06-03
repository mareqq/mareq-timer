#include "stdafx.h"
#include "AboutDlg.h"

IMPLEMENT_DYNAMIC(CAboutDlg, CWnd)

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
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()
