#pragma once

#include "../GridCtrl_src/GridCtrl.h"
// CModifyDlg 对话框

class CModifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CModifyDlg)

public:
	CModifyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModifyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnComboxSelChange();

	typedef std::vector<CString>  vString;
	void SetTableType(int tabletype){m_tableType = tabletype;}
	void SetDlgTitle(CString title){m_TitleStr = title;}
	void CreateCtrl();
	void DeleteAllCtrl();
	void SetCtrlData(vString Head,vString Data);
	void SetTPRuleCtrl();
	void IsShowCtrl();
	

protected:
	void IsShowTimeTP(BOOL bFlag);
	void IsShowProblemTP(BOOL bFlag);
	void IsShowInterfaceTP(BOOL bFlag);
	void IsShowDirectCustomerTP(BOOL bFlag);

	BOOL ReSetTPRuleParam();

	LONG ModifyProc(CADODatabase* pDb,int tabletype,int &ErrorNum,CString &ErrorMsg);
	LONG ModifyPublic(CADODatabase* pDb,CString id,CString name,CString comment,
							CString welcontent,int &ErrorNum,CString &ErrorMsg);
	LONG ModifyTPRule(CADODatabase* pDb,CString id,CString tpname,CString tptype,
		CString starttime, CString timeincre,CString probtype,CString iname,CString csser
		,int &ErrorNum,CString &ErrorMsg);
	LONG ModifyIncode(CADODatabase* pDb,CString id,CString name,CString disRule,int &ErrorNum,CString &ErrorMsg);
private:
	int m_tableType;
	CString m_TitleStr;
	CGridCtrl m_Grid;
	vString m_vHead;
	vString m_vData;
	std::vector<CStatic*>       m_StaticObject;
	std::vector<CEdit*>         m_EidtObject;
	std::vector<CComboBox*>     m_ComboObject;
	std::vector<CDateTimeCtrl*> m_DatetimeObject;
public:
	afx_msg void OnBnClickedOk();
};
