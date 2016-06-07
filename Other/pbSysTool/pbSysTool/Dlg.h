#pragma once
#include "afxdialogex.h"
class CDlg :
	public CDialogEx
{
	DECLARE_DYNAMIC(CDlg)
public:
	CDlg(CWnd* pParent = NULL);
	~CDlg(void);
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	virtual void SetTableType(int tabletype){m_tableType = tabletype;}
	virtual void SetDlgTitle(CString title){m_TitleStr = title;}
private:
	int m_tableType;
	CString m_TitleStr;
};

