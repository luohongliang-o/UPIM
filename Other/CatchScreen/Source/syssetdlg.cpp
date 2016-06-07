/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：SysSetDlg.cpp
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：系统设置对话框模块文件
*======================================================
* 版本变更记录:
*      v1.0  2002-11-18   14：00   
*
* 
*/

/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "..\\INCLUDE\\PaintDlg.h"

//#include "..\\cphdlg.h"
#include "..\\INCLUDE\\SysSetDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//应用程序运行路径,如"c:\cph\"
extern  CString g_strAppPath;        
extern  IniPaintSet g_stuPaintSet;

extern void StopBackMusic();
extern HRESULT PlayBackMusic(CString fileName);

/////////////////////////////////////////////////////////////////////////////
// CSysSetDlg dialog

/*
*--------------------------------------------------------------------------------
*  成员函数名   :CSysSetDlg
*  功能描述     :构造函数
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CSysSetDlg::CSysSetDlg(CWnd* pParent /*=NULL*/)
	: CBitmapDialog(CSysSetDlg::IDD, pParent)
{
   m_pMainWnd = (CCPHDlg *)pParent;
	//{{AFX_DATA_INIT(CSysSetDlg)


	//}}AFX_DATA_INIT
}


void CSysSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CBitmapDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysSetDlg, CBitmapDialog)
	//{{AFX_MSG_MAP(CSysSetDlg)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysSetDlg message handlers
/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnInitDialog()
*  功能描述     :对话框初始化函数
*  输入参数描述	:
*  返回变量说明 :返回TRUE表示初始化成功，FALSE表示初始化失败
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL CSysSetDlg::OnInitDialog() 
{
	CBitmapDialog::OnInitDialog();
	CRect frect;
 	m_pMainWnd->GetWindowRect(&frect);
	MoveWindow(frect.left + 120, frect.top + 200, 400, 300, FALSE);

    CString faceFile = g_strAppPath + "\\UseRes\\face-x.bmp";
	if(!SetBitmap(faceFile))
	    return FALSE;
	//初始化窗体上的按纽
    InitButton();
    
	//从主对话框中得到当前要当前参数值
	 CCPHDlg * pParent = (CCPHDlg *)GetParent();  

   

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   : InitButton()
*  功能描述     : 初始化窗体中的6个按纽 
*  输入参数描述	:
*      
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/

void CSysSetDlg::InitButton()
{
	  m_btnBkMusic.Create("", WS_VISIBLE|WS_TABSTOP, BKMUSICON, this, IDC_BKMUSIC); 
      m_btnBkMusic.SetBitmaps("BtnRes\\x1.bmp", RGB(255, 255, 255), "BtnRes\\x1.bmp", NULL); 

      m_btnMusic.Create("", WS_VISIBLE|WS_TABSTOP,MUSIC0, this, IDC_MUSICINDEX); 
      m_btnMusic.SetBitmaps("BtnRes\\x1.bmp", RGB(255, 255, 255), "BtnRes\\x1.bmp", NULL);   

      m_btnVoice.Create("", WS_VISIBLE|WS_TABSTOP, VOICEON, this, IDC_VOICE); 
      m_btnVoice.SetBitmaps("BtnRes\\x1.bmp", RGB(255, 255, 255), "BtnRes\\x1.bmp", NULL); 

      m_btnTxt.Create("", WS_VISIBLE|WS_TABSTOP, TXTON, this, IDC_TXT); 
      m_btnTxt.SetBitmaps("BtnRes\\x1.bmp", RGB(255, 255, 255), "BtnRes\\x1.bmp", NULL); 

      m_btnOk.Create("", WS_VISIBLE|WS_TABSTOP, SETOK, this, IDC_SETOK); 
      m_btnOk.SetBitmaps("BtnRes\\x4-2.bmp", RGB(255, 0, 255), "BtnRes\\x4-1.bmp", NULL); 

      m_btnCancel.Create("", /*WS_VISIBLE|*/WS_TABSTOP, SETCANCEL, this, IDC_SETCANCEL); 
      m_btnCancel.SetBitmaps("BtnRes\\x3-2.bmp", RGB(255, 0, 255), "BtnRes\\x3-1.bmp", NULL); 

}






