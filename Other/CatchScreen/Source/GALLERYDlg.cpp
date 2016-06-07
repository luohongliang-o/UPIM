/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：GALLERYDlg.cpp
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



#include "stdafx.h"
#include <math.h>
#include "..\Paint\PaintDlg.h"
#include "..\\CPHDlg.h"
#include "..\\include\\GALLERYDlg.h"

#include "winuser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//应用程序运行路径,如"c:\cph\"
extern  CString g_strAppPath;        
extern  IniPaintSet g_stuPaintSet;
/////////////////////////////////////////////////////////////////////////////
// CGALLERYDlg dialog
/*--------------------------------------------------------------------------------
*  成员函数名   :CGALLERYDlg
*  功能描述     :构造函数
*  输入参数描述	:
*    CWnd* pParent：父窗体句柄
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/

CGALLERYDlg::CGALLERYDlg(CWnd* pParent /*=NULL*/)
	: CBitmapDialog(CGALLERYDlg::IDD, pParent)
{
	m_pMainWnd = (CCPHDlg *)pParent;
	//{{AFX_DATA_INIT(CGALLERYDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_bZoom = FALSE;
	CurrentPage = 0;
	m_pagenum = 0;
	//}}AFX_DATA_INIT
}

/*--------------------------------------------------------------------------------
*  成员函数名   :DoDataExchange
*  功能描述     :对话框数据关联宏
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/
void CGALLERYDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGALLERYDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGALLERYDlg, CBitmapDialog)
	//{{AFX_MSG_MAP(CGALLERYDlg)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_GAL_EXIT, OnExitClick)
	ON_BN_CLICKED(IDC_GAL_FORWARD, OnForwardClick)
	ON_BN_CLICKED(IDC_GAL_BACK, OnBackClick)
	ON_BN_CLICKED(IDC_GAL_ZOOM, OnZoomClick)
	ON_BN_CLICKED(IDC_GAL_EDIT, OnEditClick)

    ON_MESSAGE(UWM_GETPREPICTURE, OnGetWndMsg)  //++自定义消息
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGALLERYDlg message handlers
////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------------
*  成员函数名   :OnInitDialog()
*  功能描述     :对话框初始化
*  输入参数描述	:
*  返回变量说明 :返回TRUE表示初始化成功，否则返回FALSE表示初始化失败
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/
BOOL CGALLERYDlg::OnInitDialog() 
{

    CRect rect;
    GetClientRect(&rect);
	CBitmapDialog::OnInitDialog();
    //SetWindowPos(NULL,0,0,800,600,SWP_NOMOVE);
    
	CRect frect;
	m_pMainWnd->GetWindowRect(&frect);
	MoveWindow(frect.left, frect.top, frect.Width(), frect.Height(), FALSE);	
   

	 //DestroyWindow();

    //对话框窗体居中
    //CenterWindow(); 
	//设置对话框窗口底图

    CString faceFile = g_strAppPath + "\\UseRes\\Face_manage.bmp";
	if(!SetBitmap(faceFile))
	    return FALSE; 
    //初始化对话框上的按纽
	InitButton();
	//初始化对话框上的9个图象窗口
	InitGallery();
	m_viewWnd.CreateEx(WS_EX_TOPMOST, NULL, "", WS_CHILD | WS_TABSTOP ,0, 70, 592, 458, m_hWnd, NULL);
	
	CreateBTNST();
	PicNumber = btnbmp.GetSize(); 
	if(PicNumber > 9)
		m_pagenum = 9;
	else
		m_pagenum = PicNumber%9;
	m_GALForward.ShowWindow(SW_HIDE);
	TotalPage = PicNumber/9 ;
	if(PicNumber<=9)
	{
		
		for(int i=0;i<PicNumber;i++)
			ShowBitmap(CurrentPage*9+i,i);
		m_GALBack.ShowWindow(SW_HIDE);
	}
	else
	{
		for(int i=0;i<9;i++)
			ShowBitmap(CurrentPage*9+i,i);
      //m_GALBack.ShowWindow(SW_HIDE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/*--------------------------------------------------------------------------------
*  成员函数名   :OnCreate
*  功能描述     :当创建对话框时改变对话框窗体的类型
*  输入参数描述	:
*   LPCREATESTRUCT lpCreateStruct：窗体类型结构参数
*  返回变量说明 :返回0表示成功创建窗体，返回-1表示窗体被消毁
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/
int CGALLERYDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBitmapDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
// 	CRect frect;
//  CCPHDlg * maindlg = (CCPHDlg *)AfxGetMainWnd(); 
//	maindlg->GetWindowRect(&frect);
//	SetWindowPos(&wndTop, frect.left, frect.top, frect.Width(), frect.Height(), SWP_SHOWWINDOW);
    CenterWindow();

	return 0;
}

/*--------------------------------------------------------------------------------
*  成员函数名   :OnPaint()
*  功能描述     :当对话框接到WM_PAINT消息时重新绘制窗体
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/
void CGALLERYDlg::OnPaint() 
{
	CPaintDC dc(this); 
    if (IsIconic())
	{
	    SendMessage(WM_ICONERASEBKGND,  (WPARAM) dc.GetSafeHdc(), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
       //Invalidate();
	}
	else
	{
			CBitmapDialog::OnPaint();
	}
//	m_viewWnd.Invalidate();
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   : GetButtonDefaultRect
*  功能描述     : 得到在主界面中要动态创建的图标按纽的默认矩形区
*  输入参数描述	:
*      INT btnID:要动态创建的按纽或其它控件的ID号
*  返回变量说明 :返回要创建控件的指定的矩形
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
CRect CGALLERYDlg::GetButtonDefaultRect(INT btnID)
{   
   	CRect rect;

    switch(btnID)
	{	
		//前翻页按纽
		case IDC_GAL_FORWARD:
			{ 
				rect.SetRect(28, 528, 28 + 70, 528 + 26); 
				break;  
			}  
		//后翻页按纽
		case IDC_GAL_BACK:                                  
			  {
				  rect.SetRect(490, 528, 490 + 70, 528 + 26);
				  break; 
			  }
        //缩放按纽
		case IDC_GAL_ZOOM:                      
			  {
				  rect.SetRect(659, 89, 659 + 123, 89 + 141);  
				  break;
			  }
        //编辑按纽
		case IDC_GAL_EDIT:                            
			  { 
				  rect.SetRect(659, 249, 659 + 123, 249 + 141);
				  break; 
			  }
        //退出按纽
		case IDC_GAL_EXIT:                  
			  { 
				  rect.SetRect(658, 410, 658 + 142, 410 + 190);
				  break; 
			  }
		//第一个图片窗口
		case IDC_GAL_SHOW0:                  
			  { 
				  rect.SetRect(29, 94, 29 + 160, 94 + 120);
				  break; 
			  }
		//第二个图片窗口
		case IDC_GAL_SHOW1:                  
			  { 
				  rect.SetRect(210, 94, 219 + 160, 94 + 120);
				  break; 
			  }
		//第三个图片窗口
		case IDC_GAL_SHOW2:                  
			  { 
				  rect.SetRect(399, 94, 399 + 160, 94 + 120);
				  break; 
			  }
		//第四个图片窗口
		case IDC_GAL_SHOW3:                  
			  { 
				  rect.SetRect(29, 239, 29 + 160, 239 + 120);
				  break; 
			  }
		//第五个图片窗口
		case IDC_GAL_SHOW4:                  
			  { 
				  rect.SetRect(210, 239, 219 + 160, 239 + 120);
				  break; 
			  }
		//第六个图片窗口
		case IDC_GAL_SHOW5:                  
			  { 
				  rect.SetRect(399, 239, 399 + 160, 239 + 120);
				  break; 
			  }
		//第七个图片窗口
		case IDC_GAL_SHOW6:                  
			  { 
				  rect.SetRect(29, 384, 29 + 160, 384 + 120);
				  break; 
			  }
		//第八个图片窗口
		case IDC_GAL_SHOW7:                  
			  { 
				  rect.SetRect(210, 384, 219 + 160, 384 + 120);
				  break; 
			  }
		//第九个图片窗口
		case IDC_GAL_SHOW8:                  
			  { 
				  rect.SetRect(399, 384, 399 + 160, 384 + 120);
				  break; 
			  }
		//放大显示时的窗口
		case IDC_GAL_SHOWBIG:                  
			  { 
				  rect.SetRect(36, 106, 36 + 520, 106 + 390);
				  break; 
			  }
		default:

			{  rect.SetRect(0, 0, 0, 0); }
		}//end switch

	return rect;
}


/*--------------------------------------------------------------------------------
*  成员函数名   :GetResPath
*  功能描述     :得到资源文件所在的目录路径
*  输入参数描述	:
*   UINT pathID: 要取得的路径的ID号
*  返回变量说明 :文件所在的绝对路径
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/
CString CGALLERYDlg::GetResPath(UINT pathID)
{
	CString strpath=_T("");
	switch(pathID)
	{
	case 0:
		strpath=g_strAppPath + _T("\\BtnRes\\");
		break;
    case 1:
		strpath=g_strAppPath + _T("\\MyPicture\\*.bmp");
        break;
	case 2:
		strpath=g_strAppPath + _T("\\MyPicture\\");
		break;
	default:
		break;
	}
   return strpath;
}


/*--------------------------------------------------------------------------------
*  成员函数名   :InitButton()
*  功能描述     :初始化对话框上的全部按纽
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/
void CGALLERYDlg::InitButton()
{  
	CString strBtnBmpPath=_T("");
	strBtnBmpPath = GetResPath(0);

	//以下位图位于[安装目录...]\BtnRes\  目录下
    static CString btnbmp[]={_T("h1-2.bmp"), _T("h1-1.bmp"), _T("h2-2.bmp"), _T("h2-1.bmp"),     //h部份 [0]---[3]
                               _T("h3-2.bmp"), _T("h3-1.bmp"), _T("h32-2.bmp"), _T("h32-1.bmp"), 	   //      [4]---[7]
	                           _T("h4-2.bmp"), _T("h4-1.bmp"), _T("h5-2.bmp"), _T("h5-1.bmp"), 	   //      [8]---[11]	
							};  
     
 	  //初始化向前按纽
	  m_GALForward.Create("", WS_VISIBLE|WS_TABSTOP, GetButtonDefaultRect(IDC_GAL_FORWARD), 
		            this, IDC_GAL_FORWARD); 
      m_GALForward.SetBitmaps(strBtnBmpPath + btnbmp[0], RGB(100, 0, 100), 
		                strBtnBmpPath + btnbmp[1], NULL); 
	  m_GALForward.SetTooltipText(_T("上一页"));

      //初始化向后按纽(a1)
  	  m_GALBack.Create("", WS_VISIBLE|WS_TABSTOP, GetButtonDefaultRect(IDC_GAL_BACK), this, IDC_GAL_BACK); 
      m_GALBack.SetBitmaps(strBtnBmpPath + btnbmp[2], RGB(100, 0, 100), strBtnBmpPath + btnbmp[3], NULL); 
  	  m_GALBack.SetTooltipText(_T("下一页"));
   
	  //初始化缩放按纽(a2)
	  m_GALZoom.Create("", WS_VISIBLE|WS_TABSTOP, GetButtonDefaultRect(IDC_GAL_ZOOM), this, IDC_GAL_ZOOM); 
      m_GALZoom.SetBitmaps(strBtnBmpPath + btnbmp[4], RGB(100, 0, 100), strBtnBmpPath + btnbmp[5], NULL); 
  	  m_GALZoom.SetTooltipText(_T("放大"));
	  m_bZoom = FALSE;

	 //初始化编辑按纽(a3)
	  m_GALEdit.Create("", WS_VISIBLE|WS_TABSTOP, GetButtonDefaultRect(IDC_GAL_EDIT), this, IDC_GAL_EDIT); 
      m_GALEdit.SetBitmaps(strBtnBmpPath + btnbmp[8], RGB(100, 0, 100), strBtnBmpPath + btnbmp[9], NULL); 
  	  m_GALEdit.SetTooltipText(_T("编辑"));

      //初始化退出按纽(a4)
	  m_GALExit.Create("", WS_VISIBLE|WS_TABSTOP, GetButtonDefaultRect(IDC_GAL_EXIT), this, IDC_GAL_EXIT); 
      m_GALExit.SetBitmaps(strBtnBmpPath + btnbmp[10], RGB(100, 0, 100), strBtnBmpPath + btnbmp[11], NULL); 
  	  m_GALExit.SetTooltipText(_T("退出"));

	 

}

/*
*--------------------------------------------------------------------------------
*  成员函数名   : OnExit()
*  功能描述     : 当单击[退出]按纽时的退出应用程序的消息函数
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void CGALLERYDlg::OnExitClick() 
{
	EndDialog(0);
	SetCurrentDirectory(g_strAppPath);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnForwardClick() 
*  功能描述     :向前翻页
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void CGALLERYDlg::OnForwardClick() 
{
	int PicYU = PicNumber % 9;
	m_pagenum = 9;
	m_GALBack.ShowWindow(SW_SHOW);
	for(int i=0;i<9;i++)
		m_ViewShow[i].ShowWindow(SW_SHOW);
	if(CurrentPage>0)
	{
		CurrentPage--;
		for(int i=0;i<9;i++)
			ShowBitmap(CurrentPage*9+i,i);
		if(CurrentPage==0)
			m_GALForward.ShowWindow(SW_HIDE);
	}	
}

/*--------------------------------------------------------------------------------
*  成员函数名   :OnBackClick() 
*  功能描述     :向后翻页
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/
void CGALLERYDlg::OnBackClick() 
{
	//得到当前页的余数：及当前页共有的图形数量
	int PicYU = PicNumber % 9;
	m_pagenum = PicYU;
	m_GALForward.ShowWindow(SW_SHOW);

	//显示当前页
	if (CurrentPage<TotalPage-1)
	{
		CurrentPage++;
		for(int i=0;i<9;i++)
			ShowBitmap(CurrentPage*9+i,i);
		if((CurrentPage==TotalPage-1)&&(PicYU==0))
			m_GALBack.ShowWindow(SW_HIDE);
	}

	//最后一页
	else if((CurrentPage==TotalPage-1)&&(PicYU!=0))
	{
		CurrentPage++;
		for(int i=0;i<PicYU;i++)
			ShowBitmap(CurrentPage*9+i,i);
		for(int j=PicYU;j<9;j++)
			m_ViewShow[j].ShowWindow(SW_HIDE);
		m_GALBack.ShowWindow(SW_HIDE);
	}
}


/*--------------------------------------------------------------------------------
*  成员函数名   :OnEditClick()
*  功能描述     :编辑当前选中的图象
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:在绘画区中打开选中图象并关闭画册对话框
*-----------------------------------------------------------------------------------
*/

void CGALLERYDlg::OnEditClick() 
{
	if(PicNumber == 0)
		return;
		 for(int i = 0; i < 9; i++)
		{	
			if(m_ViewShow[i].GetPreFocus())
		    break;
		}
	 //DestroyWindow();
     m_pMainWnd->EditMyPicture(m_ViewShow[i].m_srcDib); 
     OnExitClick();

     
}


/*--------------------------------------------------------------------------------
*  成员函数名   :InitGallery()
*  功能描述     :初始化对话框上的9个用于显示图形的子窗口
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/
void CGALLERYDlg::InitGallery()
{
	CString strBtnBmpPath=_T("");
	strBtnBmpPath = GetResPath(1);

	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	bool bFind = true;
	//查找\\MyPicture\\    目录下的全部文件
	hFind = FindFirstFile(strBtnBmpPath, &wfd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		
		FindClose(hFind);
	}
	else
	{
		btnbmp.Add(wfd.cFileName);
		int i=1;
		
		while(bFind)
		{	
			if(FindNextFile(hFind, &wfd))
				bFind = TRUE;
			else
				bFind = FALSE;
			if(bFind)
				btnbmp.Add(wfd.cFileName);
			i++;
		}
		FindClose(hFind);
	}
}		

/*--------------------------------------------------------------------------------
*  成员函数名   :OnZoomClick()
*  功能描述     :缩放当前选中的图片
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/
void CGALLERYDlg::OnZoomClick()
{	
	if(PicNumber == 0)
		return;
	CRect rect[9];
	rect[0] = CRectRECT_IMG0;
 	rect[1] = CRectRECT_IMG1;  
	rect[2] = CRectRECT_IMG2;
 	rect[3] = CRectRECT_IMG3;  
	rect[4] = CRectRECT_IMG4;
 	rect[5] = CRectRECT_IMG5;  
	rect[6] = CRectRECT_IMG6;
 	rect[7] = CRectRECT_IMG7;  
 	rect[8] = CRectRECT_IMG8;  

	CString strBtnBmpPath1=_T("");
	strBtnBmpPath1= GetResPath(0);
    INT focus = 0;
	//以下位图位于[安装目录...]\BtnRes\  目录下
    static CString btnbmp1[]={_T("h1-2.bmp"), _T("h1-1.bmp"), 
		                      _T("h2-2.bmp"), _T("h2-1.bmp"),     //h部份 [0]---[3]
                              _T("h3-2.bmp"), _T("h3-1.bmp"),
							  _T("h32-2.bmp"), _T("h32-1.bmp")   //[4]---[7]
							}; 
//是缩小
	if(m_bZoom)
	{
        for(focus = 0; focus< 9; focus++)
		{	if(m_ViewShow[focus].GetPreFocus())
		    break;
		}


		m_ViewShow[focus].SetWindowPos(&wndTop, rect[focus].left, rect[focus].top,
				                                    rect[focus].Width(), rect[focus].Height(),
													SWP_SHOWWINDOW); 

        for(int k = 0; k < focus; k++)
		{
				m_ViewShow[k].SetWindowPos(&wndTop, rect[k].left, rect[k].top,
				                                    rect[k].Width(), rect[k].Height(),
													SWP_SHOWWINDOW);   
		}

		for(int m = focus + 1; m < m_pagenum ; m++)
				m_ViewShow[m].SetWindowPos(&wndTop, rect[m].left, rect[m].top,
				                                    rect[m].Width(), rect[m].Height(),
													SWP_SHOWWINDOW);      
     
	    m_ViewShow[focus].SetBig(TRUE); 
	    m_ViewShow[focus].DrawSmall(); 

         m_bZoom = !m_bZoom;
	
		m_GALZoom.SetBitmaps(strBtnBmpPath1 + btnbmp1[4], g_stuPaintSet.tranCol, strBtnBmpPath1 + btnbmp1[5], NULL); 
  		m_GALZoom.SetTooltipText(_T("放大"));
	}
	//是放大
	else
	{
	  for(focus = 0; focus< 9; focus++)
		{	if(m_ViewShow[focus].GetPreFocus())
		    break;
		}

       for(int k = 0; k < focus; k++)
				m_ViewShow[k].SetWindowPos(&wndBottom, rect[k].left, rect[k].top,
				                                      rect[k].Width(), rect[k].Height(),
													  SWP_SHOWWINDOW);      

		for(int m = focus + 1; m < 9; m++)
				m_ViewShow[m].SetWindowPos(&wndBottom, rect[m].left, rect[m].top,
				                                    rect[m].Width(), rect[m].Height(),
													SWP_SHOWWINDOW);    

	   m_ViewShow[focus].SetBig(FALSE); 
	   //放大焦点图
       m_ViewShow[focus].DrawBig(); 

       m_bZoom = !m_bZoom;

	   m_GALZoom.SetBitmaps(strBtnBmpPath1 + btnbmp1[6], g_stuPaintSet.tranCol, strBtnBmpPath1 + btnbmp1[7], NULL); 
  	   m_GALZoom.SetTooltipText(_T("缩小"));    

	}

}

/*--------------------------------------------------------------------------------
*  成员函数名   :ShowBitmap
*  功能描述     :在对话框中的9个视中显示出对应的图形
*  输入参数描述	:
*    long PicNum:枚举出的图象序数
*    long number:当前页面的图形窗口位置序号
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/
void CGALLERYDlg::ShowBitmap(long PicNum,long number)
{
	CString strBtnBmpPath1=_T("");
	strBtnBmpPath1= GetResPath(2);
	m_ViewShow[number].SetPrePos(number);  
	switch(number)
	{
	case 0:	
		m_ViewShow[0].SetViewBitmap(strBtnBmpPath1 + btnbmp[PicNum]); 
		m_ViewShow[0].ShowWindow(SW_SHOW);
		m_ViewShow[0].SetPreFocus(TRUE); 
        m_ViewShow[0].Invalidate(TRUE); 

		break;
	case 1:
		m_ViewShow[1].SetViewBitmap(strBtnBmpPath1 + btnbmp[PicNum]); 
		m_ViewShow[1].ShowWindow(SW_SHOW);
	    m_ViewShow[1].Invalidate(TRUE); 
		break;
	case 2:
		m_ViewShow[2].SetViewBitmap(strBtnBmpPath1 + btnbmp[PicNum]); 
		m_ViewShow[2].ShowWindow(SW_SHOW);

        m_ViewShow[2].Invalidate(TRUE); 
		break;
	case 3:
		m_ViewShow[3].SetViewBitmap(strBtnBmpPath1 + btnbmp[PicNum]); 
		m_ViewShow[3].ShowWindow(SW_SHOW);
        m_ViewShow[3].Invalidate(TRUE); 
		break;
	case 4:
		m_ViewShow[4].SetViewBitmap(strBtnBmpPath1 + btnbmp[PicNum]); 
		m_ViewShow[4].ShowWindow(SW_SHOW);
		m_ViewShow[4].Invalidate(TRUE); 
		break;
	case 5:
		m_ViewShow[5].SetViewBitmap(strBtnBmpPath1 + btnbmp[PicNum]); 
		m_ViewShow[5].ShowWindow(SW_SHOW);
		m_ViewShow[5].Invalidate(TRUE);
		break;
	case 6:
		m_ViewShow[6].SetViewBitmap(strBtnBmpPath1 + btnbmp[PicNum]); 
		m_ViewShow[6].ShowWindow(SW_SHOW);
		m_ViewShow[6].Invalidate(TRUE);
		break;
	case 7:
		m_ViewShow[7].SetViewBitmap(strBtnBmpPath1 + btnbmp[PicNum]); 
		m_ViewShow[7].ShowWindow(SW_SHOW);
		m_ViewShow[7].Invalidate(TRUE);
		break;
	case 8:
		m_ViewShow[8].SetViewBitmap(strBtnBmpPath1 + btnbmp[PicNum]); 
		m_ViewShow[8].ShowWindow(SW_SHOW);
		m_ViewShow[8].Invalidate(TRUE);
		break;
	default:
		break;
	}
}


/*--------------------------------------------------------------------------------
*  成员函数名   :CreateBTNST()
*  功能描述     :在对话框上创建9个用于显示图形的窗体对象
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/
void CGALLERYDlg::CreateBTNST()
{
	m_ViewShow[0].CreateEx(WS_EX_TOPMOST, NULL, "",WS_CHILD | WS_TABSTOP ,29, 94, 160, 120, m_hWnd, NULL);
	m_ViewShow[0].SetPrePos(0); 
	m_ViewShow[1].CreateEx(WS_EX_TOPMOST, NULL, "",WS_CHILD | WS_TABSTOP ,214, 94, 160, 120, m_hWnd, NULL);
	m_ViewShow[1].SetPrePos(1); 
	m_ViewShow[2].CreateEx(WS_EX_TOPMOST, NULL, "",WS_CHILD | WS_TABSTOP ,399, 94, 160, 120, m_hWnd, NULL);
	m_ViewShow[2].SetPrePos(2); 
	m_ViewShow[3].CreateEx(WS_EX_TOPMOST, NULL, "",WS_CHILD | WS_TABSTOP ,29, 239, 160, 120, m_hWnd, NULL);
	m_ViewShow[3].SetPrePos(3); 
	m_ViewShow[4].CreateEx(WS_EX_TOPMOST, NULL, "",WS_CHILD | WS_TABSTOP ,214, 239, 160, 120, m_hWnd, NULL);
	m_ViewShow[4].SetPrePos(4); 
	m_ViewShow[5].CreateEx(WS_EX_TOPMOST, NULL, "",WS_CHILD | WS_TABSTOP ,399, 239, 160, 120, m_hWnd, NULL);
	m_ViewShow[5].SetPrePos(5); 
	m_ViewShow[6].CreateEx(WS_EX_TOPMOST, NULL, "",WS_CHILD | WS_TABSTOP ,29, 384, 160, 120, m_hWnd, NULL);
	m_ViewShow[6].SetPrePos(6); 
	m_ViewShow[7].CreateEx(WS_EX_TOPMOST, NULL, "",WS_CHILD | WS_TABSTOP ,214, 384, 160, 120, m_hWnd, NULL);
	m_ViewShow[7].SetPrePos(7); 
	m_ViewShow[8].CreateEx(WS_EX_TOPMOST, NULL, "",WS_CHILD | WS_TABSTOP ,399, 384, 160, 120, m_hWnd, NULL);
	m_ViewShow[8].SetPrePos(8); 

}



/*--------------------------------------------------------------------------------
*  成员函数名   :OnShowClick
*  功能描述     :当显示选中的图片时
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/
void CGALLERYDlg::OnShowClick(long Number)
{
	CString strBtnBmpPath1=_T("");
	strBtnBmpPath1= GetResPath(0);

	//以下位图位于[安装目录...]\BtnRes\  目录下
    static CString btnbmp1[]={_T("h1-2.bmp"), _T("h1-1.bmp"), _T("h2-2.bmp"), _T("h2-1.bmp"),     //h部份 [0]---[3]
                               _T("h3-2.bmp"), _T("h3-1.bmp"), _T("h32-2.bmp"), _T("h32-1.bmp"), 	   //      [4]---[7]
							};

	m_GALZoom.SetBitmaps(strBtnBmpPath1 + btnbmp1[5], RGB(100, 0, 100), strBtnBmpPath1 + btnbmp1[6], NULL); 
  	m_GALZoom.SetTooltipText(_T("缩小"));

	SendMessage(WM_PAINT);
	int PicNum = CurrentPage*9 + Number;

}



/*--------------------------------------------------------------------------------
*  成员函数名   :OnGetWndMsg
*  功能描述     :得到当前图形视的索引号并画当前视边框，同时将最大化当前图形，隐藏若它图形
*  输入参数描述	:
*    LPARAM wParam：当前图形窗体的句柄
*    WPARAM nevent：消息标识号
*  返回变量说明 :
*  算法描述		:
*  附加说明:
//参数   LPARAM=0  表示当前图形窗口画边框，其它窗口若有边框则取消边框同时取消其它窗体的焦点状态
//       LPARAM=1  表示当前图形窗口要恢复，其它窗口同时显示
//       LPARAM=2  表示当前图形窗口要最大化，其它窗口需隐藏
//       LPARAM=3  暂没使用*
*-----------------------------------------------------------------------------------
*/
LRESULT CGALLERYDlg::OnGetWndMsg(LPARAM wParam, WPARAM nevent)
{
     CViewBig * pCurrentWnd= (CViewBig*) wParam ;
     INT index = pCurrentWnd->GetPrePos(); 
     m_ViewShow[index].SetPreFocus(TRUE); 
   

	 switch(nevent)
	 {
		 case 0:
			 {  
				 m_ViewShow[index].DrawFrame(TRUE);
                 for(int i = 0; i < index; i++)
				 {
                    m_ViewShow[i].DrawFrame(FALSE);
                    m_ViewShow[i].SetPreFocus(FALSE);  
				 }
                 for(int k = index + 1; k < 9; k++)
				 {
					 m_ViewShow[k].DrawFrame(FALSE);	
                     m_ViewShow[k].SetPreFocus(FALSE);  					 
				 }
				 break;
			 
			 }
		 case 1:
			 { 
				 for(int i = 0; i < index; i++)
						m_ViewShow[i].ShowWindow(SW_SHOW);
				 for(int k = index + 1; k < 9; k++)
						m_ViewShow[k].ShowWindow(SW_SHOW);				 
				 break;
			 }
		 
		 case 2:
			 {
				 for(int i = 0; i < index; i++)
						m_ViewShow[i].ShowWindow(SW_HIDE);
				 for(int k = index + 1; k < 9; k++)
						m_ViewShow[k].ShowWindow(SW_HIDE);				 
				 break;
			 
			 }
			 case 3:
			 {
                 switch(index)
				 {
					 case 0:
						 { 
							m_ViewShow[0].MoveWindow(CRect(29, 94, 160, 120),TRUE); 		 
							break;
						 } 
					 case 1:
						 {
							m_ViewShow[1].MoveWindow(CRect(214, 94, 160, 120),TRUE); 	
							break;
						 }
					 case 2:
						 {
							m_ViewShow[2].MoveWindow(CRect(399, 94, 160, 120),TRUE); 	
							break;
						 }
					 case 3:
						 {
							m_ViewShow[3].MoveWindow(CRect(29, 239, 160, 120),TRUE); 	
							break;
						 }
					 case 4:
						 {
							m_ViewShow[4].MoveWindow(CRect(214, 239, 160, 120),TRUE); 
							break;
						 }					
					 case 5:
						 {
							m_ViewShow[5].MoveWindow(CRect(399, 239, 160, 120),TRUE); 
							break;
						 }
					 case 6:
						 {
							m_ViewShow[6].MoveWindow(CRect(29, 384, 160, 120),TRUE); 
							break;
						 }					
					 case 7:
						 {
							m_ViewShow[7].MoveWindow(CRect(214, 384, 160, 120),TRUE); 
							break;
						 }
		
					 case 8:
						 {
							m_ViewShow[8].MoveWindow(CRect(399, 384, 160, 120),TRUE); 
							break;
						 }
					 default:
						 break;
				 }//end switch
			 }

		 default:
			 break;
	 }//END switch

     return 0;
}

/*--------------------------------------------------------------------------------
*  成员函数名   :GetPageNum()
*  功能描述     :得到当前选择的图形序号
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/
//返回当前页上的可用图形数量
INT CGALLERYDlg::GetPageNum()
{
	return m_pagenum;

}


/*--------------------------------------------------------------------------------
*  成员函数名   : OnOK
*  功能描述     : 重载基类虚函数，防止敲回车键退出程序
*-----------------------------------------------------------------------------------
*/
void CGALLERYDlg::OnOK() 
{
	//EditMyPicture(m_DrawRect.m_pDib);
}


/*--------------------------------------------------------------------------------
*  成员函数名   : OnCancel
*  功能描述     : 重载基类虚函数，防止敲Escasp键退出程序
*-----------------------------------------------------------------------------------
*/
void CGALLERYDlg::OnCancel() 
{
}

void CGALLERYDlg::SetCphDlgWnd(CCPHDlg *pWnd)
{
   m_pMainWnd = pWnd;
}
