#pragma once

#include "resource.h"
#include "afxwin.h"
#include "staticlink.h"

class CAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);
	virtual ~CAboutDlg();

	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
    CStatic m_TitleCtrl;
    CStaticLink m_GooglecodeLink;
    CStaticLink m_LinkMareqCom;
    CStaticLink m_MareqSk;

    CFont m_TitleFont;
};
