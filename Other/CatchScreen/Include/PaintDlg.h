
// PaintDlg.h : 头文件
//

#pragma once
#include "BitmapDialog.h"
#include "colorpen.h"
#include "ColorStatic.h"
#include "FontCombo.h"
#include "MyBitmapStatic.h"
// CPaintDlg 对话框
class CPaintDlg : public CBitmapDialog
{
// 构造
public:
	CPaintDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PAINT_DIALOG };
	
public:
	void SetDlgRect(CRect rc);
	BOOL GetFontIsBold();
	void SetPenCol(COLORREF pencolor);
	void SetCanvansStyle();
	void SetDlgType(int type);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	

// 实现
protected:
	//HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd,UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCbnSelchangeComboFont();
	DECLARE_MESSAGE_MAP()
protected:
	CBrush     m_hbrush;   
	CRect      m_rctDlg;
	CRect      m_rctColor[16];
	CColorPen  m_SelColor[16];
	CRect      m_CurSelColor;
	
	afx_msg LRESULT OnUwmShowpencolor1(WPARAM wParam, LPARAM lParam);
private:
	BOOL       m_bFontBold;
public:
	CMyBitmapStatic m_pen[3];
	CRect           m_penrect[3];
	CColorStatic    m_preSelColor;
//	PRETOOLBAR      m_toolbarstyle;   //当前工具栏
	COLORREF        m_pencolor;       //当前画笔颜色
//	BASEPEN         m_basepen;        //当前基本画笔工具
//	PEN_TRACK       m_shapepen;       //当前画笔工具轨迹
	CComboBox       m_fontCombo;      //字体枚举框
//	BOOL            m_bCanvas;        //是否是在画布上取色
	int             m_DlgType;
};
