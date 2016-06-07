
// UPIMServerView.h : CUPIMServerView 类的接口
//

#pragma once

#include "resource.h"
#include "Dlg.h"

class CUPIMServerView : public CFormView
{
protected: // 仅从序列化创建
	CUPIMServerView();
	DECLARE_DYNCREATE(CUPIMServerView)

public:
	enum{ IDD = IDD_UPIMSERVER_FORM };

// 特性
public:
	CUPIMServerDoc* GetDocument() const;

// 操作
public:
	CStatisticsDlg m_DlgStatistics;
protected:
	TabCtrlEx<TabCtrlStyle_base> m_TabCtrl;
	
	BOOL m_IsInitialed;
	map<int,CRect> m_ControlRect;
	int cx;
	int cy;
	
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CUPIMServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void InitChildCtrlPos();
	void MoveWnd(int cx,int cy);

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // UPIMServerView.cpp 中的调试版本
inline CUPIMServerDoc* CUPIMServerView::GetDocument() const
   { return reinterpret_cast<CUPIMServerDoc*>(m_pDocument); }
#endif

