
// TestMDIView.h : CTestMDIView 类的接口
//

#pragma once


class CTestMDIView : public CView
{
protected: // 仅从序列化创建
	CTestMDIView();
	DECLARE_DYNCREATE(CTestMDIView)

// 特性
public:
	CTestMDIDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CTestMDIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // TestMDIView.cpp 中的调试版本
inline CTestMDIDoc* CTestMDIView::GetDocument() const
   { return reinterpret_cast<CTestMDIDoc*>(m_pDocument); }
#endif

