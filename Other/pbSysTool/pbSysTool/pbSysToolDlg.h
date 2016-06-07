
// pbSysToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
#include "AddDlg.h"
#include "../GridCtrl_src/GridCtrl.h"
// CpbSysToolDlg 对话框

class CpbSysToolDlg : public CDialog
{
// 构造
public:
	CpbSysToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PBSYSTOOL_DIALOG };
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnCbnSelchangeComboSelectTable();
	
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonDelte();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCancel();
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
protected:
	typedef std::vector< std::vector<CString> >  vDataContainer;
	typedef std::vector<CString>                 vGridHead;
	void    UpdateGridData(vDataContainer Data,vGridHead Head);
	LONG    GetDbData(int type,vDataContainer &Data,vGridHead& Head);
	CString GetDlgTitle(int btnop);

	LONG    GetPublicInfo(vDataContainer& vpbInfo);              //公众号信息
	LONG    GetIncode(vDataContainer& vincode);                  //接入码信息
	LONG    GetTpRule(vDataContainer& vtprule);                  //路由规则信息
	LONG    GetPublicTpRelation(vDataContainer& Data);           //公众号对应路由关系表
	LONG    GetTpRelation(vDataContainer& vtprelation);          //路由对应接入码关系表
	LONG    GetDispatchRelation(vDataContainer& vdisrelation);   //接入码对应客服关系表
	LONG    GetCustomerInfo(vDataContainer& vcustomer);

	LONG    DelPulbic(CString pa,int &ErrorNum,CString &ErrorMsg);
	LONG    DelTpRule(CString pa,int &ErrorNum,CString &ErrorMsg);
	LONG    DelIncode(CString pa,int &ErrorNum,CString &ErrorMsg);
	LONG    DelPublicRelation(CString pa1,CString pa2,int &ErrorNum,CString &ErrorMsg);
	LONG    DelTpRelation(CString pa1,CString pa2,int &ErrorNum,CString &ErrorMsg);
	LONG    DelDispatchRelation(CString pa1,CString pa2,int &ErrorNum,CString &ErrorMsg);

	static BOOL CALLBACK GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam);
	
protected:
	CADODatabase*             m_pDB;
	CGridCtrl*                m_Grid;
	vDataContainer            m_vCurTPData;
	vDataContainer            m_vCurIncodeData;
	vDataContainer            m_vCurCustData;
	vDataContainer            m_vCurPublicInfoData;
	int                       m_curComboSel;
	
	std::vector<CString>      m_vModifyHead;
	std::vector<CString>      m_vModifyData;
	CComboBox m_ComboTable;
	int                       m_iCurRow;
};
