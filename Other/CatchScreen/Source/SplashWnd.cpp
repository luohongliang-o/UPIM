/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：Splash.cpp
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：启动Splash封面模块文件
*======================================================
* 版本变更记录:
*      v1.0  2002-11-24   9：00   罗伟
*
* 
*/

/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"  
//#include "..\\resource.h"  
//#include "..\\INCLUDE\\Splash.h" 
#include "..\\resource.h"  
#include "..\\include\\Splashwnd.h"  
#include "..\\CPHDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////


BOOL CSplashWnd::c_bShowSplashWnd;
CSplashWnd* CSplashWnd::c_pSplashWnd;

/*
*--------------------------------------------------------------------------------
*  成员函数名   :CSplashWnd
*  功能描述     :构造函数
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CSplashWnd::CSplashWnd()
{
	//在time(1)中播放的FALSH文件
	m_flashFile1 = _T("");
	//在time(1)中播放的FALSH文件
	m_flashFile2 = _T("");
   //当前要播放的time ID号
	m_timeID = 1;
  //播放FLASH时是否屏蔽鼠标键及键盘键来退出FALSH
	m_bKeyControl = TRUE;

}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :~CSplashWnd
*  功能描述     :析构函数
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CSplashWnd::~CSplashWnd()
{
	// 清除静态窗口指针
	ASSERT(c_pSplashWnd == this);
	c_pSplashWnd = NULL;
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
*--------------------------------------------------------------------------------
*  成员函数名   :EnableSplashScreen
*  功能描述     :使Splash启动屏幕有效
*  输入参数描述	:
*    BOOL bEnable：设置为TRUE表示有效，否则无效
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CSplashWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
{
	c_bShowSplashWnd = bEnable;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :ShowSplashScreen
*  功能描述     :显示Splash启动屏幕窗体
*  输入参数描述	:
*    CString flashfile1: 要播放的flash文件1
*    CString flashfile2: 要播放的flash文件2
*    INT timeID        : 在哪个TIME线程号中打开flash文件1
*    BOOL isCloseMainFrm :播放完FLASH后是否关闭主窗体
*    CWnd* pParentWnd  ：Splash启动窗体的父窗体
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CSplashWnd::ShowSplashScreen(CString flashfile1, CString flashfile2/* = _T("")*/, INT timeID/* = 1*/, BOOL isKeyControl /*= TRUE*/, 
								  BOOL isCloseMainFrm /*=FALSE*/, CWnd* pParentWnd/* = NULL*/)
{
	
	if (!c_bShowSplashWnd || c_pSplashWnd != NULL)
		return;

	// 分配一个新的splash窗口并创建这个窗口.

	c_pSplashWnd = new CSplashWnd;
	c_pSplashWnd->m_flashFile1 = flashfile1;
	c_pSplashWnd->m_flashFile2 = flashfile2;
    c_pSplashWnd->m_timeID = timeID;
    c_pSplashWnd->m_bKeyControl = isKeyControl;
	c_pSplashWnd->m_isCloseMainWnd = isCloseMainFrm;
	if (!c_pSplashWnd->Create(pParentWnd))
		delete c_pSplashWnd;
	else
		c_pSplashWnd->UpdateWindow();
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :PreTranslateAppMessage
*  功能描述     :得到应用程序的消息并处理
*  输入参数描述	:
*   MSG* pMsg：消息指针
*  返回变量说明 :返回TRUE表示消息处理成功，否则失败
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
{
	if (c_pSplashWnd == NULL)
		return FALSE;

	// 若按下下列键或鼠标键后隐藏splash屏幕.
	if(c_pSplashWnd->m_bKeyControl)
	{
		if (pMsg->message == WM_KEYDOWN ||
			pMsg->message == WM_SYSKEYDOWN ||
			pMsg->message == WM_LBUTTONDOWN ||
			pMsg->message == WM_RBUTTONDOWN ||
			pMsg->message == WM_MBUTTONDOWN ||
			pMsg->message == WM_NCLBUTTONDOWN ||
			pMsg->message == WM_NCRBUTTONDOWN ||
			pMsg->message == WM_NCMBUTTONDOWN)
		{
			c_pSplashWnd->HideSplashScreen();
			return TRUE;	
		}
	}
	else 
	{
			if (pMsg->message == WM_RBUTTONDOWN)
			{
				c_pSplashWnd->HideSplashScreen();
				return TRUE;	
			}

	}
	return FALSE;	
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :Create
*  功能描述     :
*  输入参数描述	:
*    CWnd* pParentWnd：Splash启动窗体的父窗体
*  返回变量说明 :返回TRUE表示创建成功，否则表示创建失败
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL CSplashWnd::Create( CWnd* pParentWnd)
{
	return CreateEx(0,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, 800, 600, pParentWnd->GetSafeHwnd(), NULL);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :HideSplashScreen
*  功能描述     :隐藏Splash屏幕
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CSplashWnd::HideSplashScreen()
{
	//消毁这个窗口同时消毁主对话框窗口.
	if(c_pSplashWnd->m_isCloseMainWnd)
	{
		DestroyWindow();
		AfxGetMainWnd()->DestroyWindow();
	}
	//消毁这个窗口并更新主对话框窗口.
	else
	{
		DestroyWindow();
		AfxGetMainWnd()->UpdateWindow();
	}




}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :PostNcDestroy()
*  功能描述     :消毁Splash启动窗体时
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CSplashWnd::PostNcDestroy()
{
	delete this;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnCreate
*  功能描述     :当创建窗体时，修改窗体类型等操作并设置Splash的显示时间
*  输入参数描述	:
*   LPCREATESTRUCT lpCreateStruct：窗体类型结构
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 窗口居中
	CenterWindow();

   //创建Flash控件对象

    m_startflash.Create (NULL,WS_CHILD|WS_VISIBLE,CRect(0, 0, 800, 600),
		                this, 1024);
    
	//控制显示第一副Flash动画
 	SetTimer (1, 0, NULL);
	//控制显示第二副Flash动画(暂不用)
    //SetTimer (2, 12000, NULL);
	//同时启动帧记录
    SetTimer (3, 2000, NULL);
	CRect m_rect;
    GetClientRect (&m_rect );
   
    //使flash窗口显示到视的满屏。    
	m_startflash.MoveWindow (&m_rect);

	return 0;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnPaint()
*  功能描述     :绘制Splash启动窗体
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);

}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnTimer
*  功能描述     :计时器用于控制启动窗体的显示时间
*  输入参数描述	:
*   UINT nIDEvent：计时器的序号
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CSplashWnd::OnTimer(UINT nIDEvent)
{
  CCPHDlg * maindlg = (CCPHDlg *)AfxGetMainWnd();  
  
  char *a=new char [512];
  GetCurrentDirectory (100,a);
  CString  *s=new CString(a);

  //播放第一个flash文件
  if (nIDEvent ==1)
  {	
	  m_startflash.LoadMovie (0, *s+CString("\\") + m_flashFile1);
	  m_startflash.SetLoop(FALSE); 
	  m_startflash.Play();
	  KillTimer (1);
  }
  //播放第二个flash文件(暂不用)
  if (nIDEvent==2)
  {    
	  m_startflash.StopPlay ();
	  m_startflash.LoadMovie(0, *s+CString("\\") + m_flashFile2);
      m_startflash.Play();
  
      KillTimer (2);
 	  HideSplashScreen();
  }
  //隐藏splash窗口
  if(nIDEvent==3)
  {	
	  INT num = m_startflash.CurrentFrame();
	  //假如是关闭窗体FLASH
	  if(m_isCloseMainWnd)
	  {
	    if(num >= 79)
		  {
	        
	         //HideSplashScreen();
             KillTimer (3);
			 delete a;
			 delete s;
			 return;
			 
		  }
	  }
	  //如果是启动FLASH
	 else if(num >= 170)
	 {  
		 KillTimer (3);	
         ShowWindow(SW_HIDE);
	     
	 }


  }
  delete a;
  delete s;
  CWnd ::OnTimer (nIDEvent );

}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :PreCreateWindow
*  功能描述     :创建Splash启动动画窗口以前修改窗口的类型
*  输入参数描述	:
*   CREATESTRUCT& cs：窗口的类型结构体
*  返回变量说明 : 返回TRUE表示修改成功
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL CSplashWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
//		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOWTEXT), NULL);
    ::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOWTEXT), NULL);
	return TRUE;
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetFlashFileName
*  功能描述     :创建Splash启动动画窗口以前修改窗口的类型
*  输入参数描述	:
*   CString filename1:在第一阶段播放的Flash文件名
*   CString filename2:在第二阶段播放的Flash文件名(可以为空)
*  返回变量说明 : 
*  算法描述		:
*  附加说明     :本程序为静态调用，该函数不可用，必须为动态创建splash窗口时方可调用
*--------------------------------------------------------------------------------
*/
void CSplashWnd::SetFlashFileName(CString filename1, CString filename2 /* = NULL*/)
{
   m_flashFile1 = filename1;
   m_flashFile2 = filename2;	
}
