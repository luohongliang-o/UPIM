/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：Canvas.h 
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：绘画区窗体类头文件
*======================================================
* 版本变更记录:
*      v1.0  2002-11-4   8:30  罗伟  
*
* 
*/

/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_Canvas_H__7FD19197_7DEB_43A7_B4E8_D67A67BEC917__INCLUDED_)
#define AFX_Canvas_H__7FD19197_7DEB_43A7_B4E8_D67A67BEC917__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include  "Dib.h"
#include "ENUM.h"
//#include "SpecialPen.h"
#include "ClipBoard.h"	// Added by ClassView
#include <list>
/////////////////////////////////////////////////////////////////////////////
/*
*类名    : Canvas
*说明    : 绘画区画布类
*变更记录:
*
*/

class CAbstractPen;
//class CSelectTool;
//class CFloatDibWnd;


class Canvas : public CWnd
{
// Construction
public:
	Canvas();
    
// Attributes
    
private:

	// 当前笔的大小，是经滚动条大小转换过后的实际笔大小
	int m_nPenSize;  //1-40
	int m_orgPenSize; //0-110  未转换的笔大小
	PEN_TRACK m_PenTrack;
	PEN_STYLE m_PenStyle;     

    //SPECIAPEN    m_speciapenType;

     //当前画笔颜色
	COLORREF      m_pencolor;      
    //当前画刷填充色(暂没有使用)
	COLORREF      m_crFillColor;   
    //当前工具栏(详枚举)
	PRETOOLBAR    m_toolbarstyle;   
    //当前基本画笔工具(详枚举)
	BASEPEN       m_basepen;       
   //当前字体类型(详枚举)
	FONTSTYLE     m_fontstyle;
    //当前几何画笔工具(详枚举)
 //   PEN_TRACK     m_shapepen;       
    //当前图片类别(详枚举)
	PICTURES      m_picture;        

  
    //当前画笔尺寸(1-100)
	INT           m_presize;        
    //鼠标左键按下情况
	BOOL          m_bLdown;
	// 剪贴板
	CClipBoard m_ClipBoard;
	// 判断是否在画虚框
	BOOL m_bDrawingRubber;
	// 虚框矩形大小
	CRect m_rcRubber;
	// 字体
	CFont* m_pFont;
	// 文字编辑控件
	CEdit m_EditText;
	// 虚框开始点
	CPoint m_ptStart;
	// 字体大小
	int m_nFontSize;
	// 粗字体
	BOOL m_bBold;
	// 斜体字
	BOOL m_bItalic;
	// 下划线字
	BOOL m_bUnderline;
	// 字体名
	CString m_stSelection;
	// 鼠标点暂存
	CPoint m_pTemp;

public:    //本部份为其它类要使用的成员变量
   //DIB对象
    CDib *        m_pDib;  	   
	//操作DIB之前备份的一个DIB副本
	CDib *        m_tempDib;        
    //背景DIB(暂不用)
 	CDib *        m_pDibBk;         
    //卡通窗体
//    CCartoon *    m_cartoon;       


protected:
	// 浮动窗口，用于选择、帖图
	//CFloatDibWnd * m_pFloatDibWnd;
	// 选取工具指针
	//CSelectTool * m_pSelectTool;
	// 笔对象指针
	CAbstractPen * m_pPen;

public:
	//定义一个特殊笔
	//CSpecialPen m_SpeciaPen;

private:   //本部份供测试用
	CPoint m_strPoint;
	CPoint pointOld;
	CPoint m_endPoint;
    CDC *  m_paintDC;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Canvas)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	enum {		
		BITMAP_STRETCH=0,		// 位图以拉伸方式填充对话框
		BITMAP_TILE,		    // 位图以自身尺寸填满对话框
		BITMAP_CENTER			//位图居中显示在对话框中
	};
public:
	void LoadFile(CString filename);
	CRect rcClient;
	int scrollbar_width;
	int x_offset;
	int y_offset;
	CSize image_size;
	HBITMAP hBitmap;           
		
	void DrawBitmap();
	void SetBitmap(HBITMAP hBmp);
	void ShowFillColor();
	void ShowPenColor();

    //吸管光标
	//HCURSOR m_hCursorPicker;  
	//是否采用背景图片
    BOOL m_bSetBkBitmap;       

	INT m_nSelectColorMode;
	virtual ~	Canvas();
protected:
    //背景DIB图片显示方式
	int			m_nType;	
	//DIB画刷
	CBrush		m_HollowBrush;     
	BOOL m_bUnDo;
	BOOL m_bReDo;
	// Generated message map functions
protected:
	//{{AFX_MSG(Canvas)
	afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//afx_msg	void OnCutCanvasDibRect(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL GetModify();
	void SetModify(BOOL isChanged = TRUE);
	INT m_leafIndex;   //树叶索引号(0-5)
	void SetLeafIndex(INT index);
	BOOL SelectToolAct(SELECT_TOOL st);
	void SetPenType(PEN_TYPE PenType);
	BOOL Paste();
	BOOL Copy();
	BOOL Cut();
    //得到作画用各参数值
	//SPECIAPEN GetSpeciaPenStyle();
	INT GetPenSize();
	PICTURES GetPictureStyle();
	FONTSTYLE GetFontStyle();
	BASEPEN GetBasePenStyle();
	PRETOOLBAR GetToolBarStyle();
	COLORREF GetFillColor();
	COLORREF GetPenColor();

	//设置绘画用各参数值
	void SetSpeciaPenStyle(SPECIAPEN);
	void SetPenSize(INT);
	void SetPictureStyle(PICTURES);
	void SetPenTrack(PEN_TRACK );
	void SetPenStyle(PEN_STYLE);
	void SetFontStyle(FONTSTYLE);
	void SetBasePenStyle(BASEPEN );
	void SetToolBarStyle(PRETOOLBAR);
	void SetFillColor(COLORREF);
	void SetPenColor(COLORREF);

	void CreatNewCanvas(CString strfile = _T(""), INT width=0,INT height=0 /*= CRect(0, 0, 520, 390)*/);
	
	void UnDo();
	BOOL UnNDo();
	void ReDo();
	void SaveBak();
	void SaveNBak();

	BOOL CreateNewCartoon(CString strFile);


protected:  
	BOOL CopyClientRectToDib();

//	CString m_cartoonfilename;

	afx_msg LRESULT OnReDraw(WPARAM,LPARAM);

	INT m_DrawPenStyle;
	CDC memDC;
	CBitmap* pBitmap;
	CBitmap* pOldBitmap;

private:

	// 处理文字的相关相关函数
	void DoDrawRubber(CDC* pDC, CRect rc);
	void StopDrawRubber();
	void StartDrawRubber(CPoint point);
	BOOL MergeText();
	void DrawRubber(CPoint point);
	BOOL PointInDib(CPoint point);
    BOOL m_bchanged;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
private:
    //+++++++++++++++++++++++++++++++++++++++++++++++++++
    CRect  m_wndRect;  //画布矩形区大小(.left!=0 .top!=0)
    CRect  m_clientRect; //画布客户区的矩形区大小(.left=0 .top=0)

	//+++++++++++++++++++++++++++++++++++++++++++++++++++
public:
	void SetFloatWndShowBtn(BOOL bCreateBtn);
	void UpdateMainWndImage();
	void InitImage(HBITMAP hBitmap);
	void InitImage(CString fileName);
	void SetNewWndRect(CRect wndRect);
	HBITMAP GetHbitmap();
	INT GetOrgPenSize();
	void MoveCanvas();
    CRect GetCanvasWndRect();
    CRect GetCanvasClientRect();

private:
	CString m_curTmpFile;
	std::list<CDib*>  m_vDib;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Canvas_H__7FD19197_7DEB_43A7_B4E8_D67A67BEC917__INCLUDED_)
