/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：GALLERYDlg.h
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：绘画乐园[我的画册]对话框头文件
*======================================================
* 版本变更记录:
*      v1.0  2002-11-22   16：50  
*
* 
*/

/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_GALLERYDLG_H__9C7C2161_F6E6_11D6_95DA_000795D28546__INCLUDED_)
#define AFX_GALLERYDLG_H__9C7C2161_F6E6_11D6_95DA_000795D28546__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "BitmapDialog.h"
#include "ButtonST.h"
#include "ENUM.h"
#include "DefineMessage.h"
#include <afxtempl.h>
#include "viewbig.h"


//#include "FileInfo.h"
/////////////////////////////////////////////////////////////////////////////
/*
*类名    : CGALLERYDlg
*说明    : 主对话框类
*变更记录:
*
*/
class CCPHDlg;
class CGALLERYDlg : public CBitmapDialog
{
// Construction
public:
	CGALLERYDlg(CWnd* pParent = NULL);   // standard constructor
public:
	void SetCphDlgWnd(CCPHDlg * pWnd);
	INT GetPageNum();

	CString GetResPath(UINT btnID);
	void InitButton();
	CViewBig  m_viewWnd;
	CViewBig  m_ViewShow[9];
// Dialog Data
	//{{AFX_DATA(CGALLERYDlg)
	enum { IDD = IDD_CPH_GALLERY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGALLERYDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGALLERYDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg LRESULT OnGetWndMsg(LPARAM, WPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CRect GetButtonDefaultRect(INT btnID);
	void OnExitClick();
	void OnForwardClick();
	void OnBackClick();
	void OnZoomClick();
	void OnEditClick();
	BOOL m_bZoom;
	void ShowBitmap(long PicNum,long x);
	void InitGallery();
	void CreateBTNST();
	void OnShowClick(long Number);
	
	//当前页上的图形窗口序号
	INT m_pagenum;
    //前翻工具按纽
	CButtonST   m_GALForward;       
     //后翻工具按纽 
    CButtonST   m_GALBack;      
	 //缩放工具按纽
    CButtonST   m_GALZoom;	
   
	CButtonST   m_GALFace;
     //编辑工具按纽 
    CButtonST   m_GALEdit;     
   //退出工具按纽 
    CButtonST   m_GALExit;        

	//图片的数量
	int PicNumber;     
	//图片的当前页数
	int CurrentPage;	
	//图片的名称
	int TotalPage;
	CArray<CString,CString> btnbmp;       


private:
   CCPHDlg * m_pMainWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GALLERYDLG_H__9C7C2161_F6E6_11D6_95DA_000795D28546__INCLUDED_)
