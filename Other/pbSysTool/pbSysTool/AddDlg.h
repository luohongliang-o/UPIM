#pragma once
#include "afxwin.h"
#include <vector>
#include "../GridCtrl_src/GridCtrl.h"
// CAddDlg 对话框

class CAddDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddDlg)

public:
	CAddDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	typedef std::vector<std::vector<CString>> vContainerData;
	void SetTableType(int tabletype){m_tableType = tabletype;}
	void SetDlgTitle(CString title){m_TitleStr = title;}
	void CreateCtrl();
	void DeleteAllCtrl();
	LONG AddProc(CADODatabase* pDb,int tabletype,int &ErrorNum,CString &ErrorMsg);
	void SetTPRule(vContainerData Data);
	void SetIncode(vContainerData Data);
	void SetCustomer(vContainerData Data);
	void SetPublicInfo(vContainerData Data);

protected:
	void PublicAddCtrl();
	void InCodeAddCtrl();
	void TpAddCtrl();
	void PublicRelationAddCtrl();
	void TpIncodeAddCtrl();
	void InCodeUserAddCtrl();

	void IsShowTimeTP(BOOL bFlag);
	void IsShowProblemTP(BOOL bFlag);
	void IsShowInterfaceTP(BOOL bFlag);
	void IsShowDirectCustomerTP(BOOL bFlag);
	void IsShowExTP(BOOL bFlag);

	
	LONG AddPublicProc(CADODatabase* pDb,CString publicname,CString createname,CString comment,CString welcomcomment,CString& curPubID,
		               int &ErrorNum,CString &ErrorMsg);
	LONG SetTpToPublic(CADODatabase* pDb,char* publicid,char* strtpid,int &ErrorNum,CString &ErrorMsg);

	LONG AddIncode(CADODatabase* pDb,CString name,int disRule,int &ErrorNum,CString &ErrorMsg);

	LONG AddTPRule(CADODatabase* pDb,CString tptype,CString tpname,CString starttime,
				  CString timeincre,CString probtype,CString iname,CString csser,int &ErrorNum,CString &ErrorMsg);
	BOOL ReSetTPRuleParam();

	LONG AddTpRelation(CADODatabase* pDb,CString tpid,CString incodeid,int &ErrorNum,CString &ErrorMsg);
	LONG AddDisRelation(CADODatabase* pDb,CString incodeid,CString id,CString loginname,CString priority,int &ErrorNum,CString &ErrorMsg);
public:
	afx_msg void OnBnClickedGetTpRule();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnComboxSelChange();
	afx_msg void OnBnClickedButtonGetIncode();
	afx_msg void OnBnClickedButtonGetCustomer();
	afx_msg void OnGridClick1(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridClick2(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridClick3(NMHDR *pNotifyStruct, LRESULT* pResult);

private:
	int           m_tableType;
	CString       m_TitleStr;
	CButton       m_BtnTpRule;
	typedef std::vector<CStatic*>       vCStatic;
	typedef std::vector<CEdit*>         vCEdit;
	typedef std::vector<CListCtrl*>     vCListCtrl;
	typedef std::vector<CComboBox*>     vCComboBox;
	typedef std::vector<CDateTimeCtrl*> vCDateTimeCtrl;
	typedef std::vector<CGridCtrl*>     vGridCtrl;
	vCStatic                            m_StaticObject;
	vCEdit                              m_EidtObject;
	vCComboBox                          m_ComboObject;
	vCDateTimeCtrl                      m_DatetimeObject;
	vGridCtrl                           m_GridObject;
	
public:
	CButton m_BtnIncode;
	CButton m_BtnCustomer;
	CButton m_BtnPblicInfo;

	vContainerData m_vtprule;
	vContainerData m_vIncode;
	vContainerData m_vCustomer;
	vContainerData m_vPublicInfo;
	
	afx_msg void OnBnClickedButtonGetPublic();
};
