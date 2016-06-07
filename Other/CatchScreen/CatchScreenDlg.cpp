// CatchScreenDlg.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "CatchScreen.h"
#include "CatchScreenDlg.h"
#include "atlimage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Canvas      *m_DrawRect;
HWND g_QQWnd;
CString GetMoudulePath()
{
	CString strRetun = _T("");

#ifdef _UNICODE
	TCHAR szBuff[MAX_PATH];
	HMODULE module = GetModuleHandle(0); 
	GetModuleFileName(module, szBuff, sizeof(szBuff)); 
	strRetun.Format(_T("%s"),szBuff);

#else
	HMODULE module = GetModuleHandle(0); 
	CHAR szBuff[MAX_PATH]; 
	GetModuleFileName(module, szBuff, sizeof(szBuff)); 
	strRetun.Format(_T("%s"),szBuff);
#endif 

	int pos = strRetun.ReverseFind(_T('\\'));

	if(pos != -1)
	{
		strRetun = strRetun.Left(pos);
	}
	return strRetun;
}

void WriteDebugMessage
	(
	LPCSTR p_lpcszFileNamePrifix,	// [in]文件名前缀(填空则默认为"DB"--Debug)
	LPCSTR p_lpcszFormat,			// [in]输出格式
	...							// [in]输出内容
	)
{
	char	szValue[100*1024] = {0}, szMessage[100*1024] = {0}, szFileName[MAX_PATH] = {0};
	va_list args;
	va_start( args, p_lpcszFormat );
	int nReturnCode = _vsnprintf( szValue, sizeof(szValue), p_lpcszFormat, args );
	if ( (nReturnCode < 0) || (nReturnCode == sizeof(szValue)) )
	{
		szValue[sizeof(szValue)-1] = '\0';
		nReturnCode = sizeof(szValue) - 1;
	}
	va_end( args );
	SYSTEMTIME	stNow;
	GetLocalTime( &stNow );
	char	szPrefix[32] = "DB";
	if (  p_lpcszFileNamePrifix[0] != '\0' )
	{
		sprintf_s( szPrefix, sizeof(szPrefix), "%s", p_lpcszFileNamePrifix );
	}
	sprintf_s( szFileName, sizeof(szFileName), "%s\\%s%04d%02d%02d.log", GetMoudulePath(), szPrefix, stNow.wYear, stNow.wMonth, stNow.wDay );
	FILE	*pFile = fopen( szFileName, "ab" );
	if ( pFile != NULL )
	{
		DWORD	dwThreadID = GetCurrentThreadId();
		int	iRet = sprintf_s( szMessage, sizeof(szMessage)-2, "%04d-%02d-%02d/%02d:%02d:%02d.%03d[%08X]--%s",
			stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour, stNow.wMinute, stNow.wSecond, stNow.wMilliseconds,
			dwThreadID, szValue );
		szMessage[iRet] = '\r';
		szMessage[iRet+1] = '\n';
		szMessage[iRet+2] = '\0';
		fwrite( szMessage, 1, strlen(szMessage), pFile );
		fclose( pFile );
		pFile = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCatchScreenDlg dialog
char strOld[1024]={0};
CCatchScreenDlg::CCatchScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCatchScreenDlg::IDD, pParent)
	
{
	//{{AFX_DATA_INIT(CCatchScreenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_pngAction.LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_JIETU));
	m_pngMask.LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_SC_MASK));
	m_pngDot.LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_SC_DOT));

    m_hCursor=AfxGetApp()->LoadCursor(IDC_CURSOR1);  
	m_iActionMask = 0;
	m_xScreen = GetSystemMetrics(SM_CXSCREEN);
	m_yScreen = GetSystemMetrics(SM_CYSCREEN);
	CRect rect(0, 0,m_xScreen,m_yScreen);
	m_pBitmap=CBitmap::FromHandle(CopyScreenToBitmap(&rect));
    m_DrawRect = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCatchScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatchScreenDlg)
	//DDX_Control(pDX, IDC_EDIT1, m_tipEdit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCatchScreenDlg, CDialog)
	//{{AFX_MSG_MAP(CCatchScreenDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONUP()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatchScreenDlg message handlers

BOOL CCatchScreenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowPos(&wndTopMost,0,0,m_xScreen,m_yScreen,SWP_SHOWWINDOW);
   	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_curSel[0]=32646;//IDC_SIZEALL;
	m_curSel[1]=32642;//IDC_SIZENWSE;
	m_curSel[2]=32645;//IDC_SIZENS;
	m_curSel[3]=32643;//IDC_SIZENWSE;
	m_curSel[4]=32644;//IDC_SIZEWE;
	m_curSel[5]=32644;//IDC_SIZEWE;
	m_curSel[6]=32643;//IDC_SIZENWSE;
	m_curSel[7]=32645;//IDC_SIZENS;
	m_curSel[8]=32642;//IDC_SIZENWSE;

	m_bSelect=FALSE;
	m_bSelected=FALSE;
	m_bLeftDown=FALSE;
	m_iCapture=0;
	m_bLButtonUp = FALSE;
	m_bTopDownFlag = FALSE;//下面
	m_bPanitCanvas = FALSE;
	

	for(int i=0;i<9;i++)
	{
		m_rcSel[i].left=0;
		m_rcSel[i].top=0;
		m_rcSel[i].right=0;
		m_rcSel[i].bottom=0;
	}
	
	for (int j=0;j<5;j++)
	{
		m_PaintDlg[j].SetDlgType(j);
		m_PaintDlg[j].Create(IDD_PAINT_DIALOG,this);
		m_PaintDlg[j].ShowWindow(SW_HIDE);
	}
	m_DlgShowIndex = 0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCatchScreenDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		DrawImage();
		CDialog::OnPaint();
	}
}

HCURSOR CCatchScreenDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCatchScreenDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void CCatchScreenDlg::OnCancel() 
{
	if (m_DrawRect)
	{
		m_DrawRect->DestroyWindow();
		delete m_DrawRect;
		m_DrawRect = NULL;
	}
	for (int i =0;i<5;i++)
	{
		m_PaintDlg[i].DestroyWindow();
	}
	CDialog::OnCancel();
}

void CCatchScreenDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bSelect&&!m_bPanitCanvas)//正在画
	{
		m_rcSel[0].right=point.x;
		m_rcSel[0].bottom=point.y;
		DrawImage();
	}
	if(m_bSelected)//画完
	{
		m_hCursor=AfxGetApp()->LoadCursor(IDC_CURSOR1);      
		if(m_iCapture==0){
			if (!m_bPanitCanvas)
			{
				for(int i=8;i>=0;i--){
					if(PtInRect(m_rcSel[i],point)){
						m_hCursor = LoadCursor(NULL,MAKEINTRESOURCE(m_curSel[i]));
						break;
					}
				}
			}
			LONG64 iOldMask=m_iActionMask;
			m_iActionMask = 0;
			if (PtInRect(m_rcAction[0],point)) m_iActionMask = 0x000000001;
			if (PtInRect(m_rcAction[1],point)) m_iActionMask = 0x000000010;
			if (PtInRect(m_rcAction[2],point)) m_iActionMask = 0x000000100;
			if (PtInRect(m_rcAction[3],point)) m_iActionMask = 0x000001000;
			if (PtInRect(m_rcAction[4],point)) m_iActionMask = 0x000010000;
			if (PtInRect(m_rcAction[5],point)) m_iActionMask = 0x000100000;
			if (PtInRect(m_rcAction[6],point)) m_iActionMask = 0x001000000;
			if (PtInRect(m_rcAction[7],point)) m_iActionMask = 0x010000000;
			if (PtInRect(m_rcAction[8],point)) m_iActionMask = 0x100000000;
			if(iOldMask!=m_iActionMask) DrawImage();
		}
		else {
			if (!m_bPanitCanvas)
			{
				for(int i=8;i>=0;i--){
					CRect tempRect;
					tempRect.CopyRect(m_rcSel[i]);
					tempRect.InflateRect(30,30);
					if(PtInRect(tempRect,point)){
						m_hCursor = LoadCursor(NULL,MAKEINTRESOURCE(m_curSel[i]));
						break;
					}
				}
			}
			int x=point.x-m_ptLButtonDown.x;
			int y=point.y-m_ptLButtonDown.y;
			switch(m_iCapture)
			{
			case 1:
				m_rcSel[0].left+=x;
				m_rcSel[0].top+=y;
				break;
			case 2:
				m_rcSel[0].top+=y;
				break;
			case 3:
				m_rcSel[0].top+=y;
				m_rcSel[0].right+=x;
				break;
			case 4:
				m_rcSel[0].left+=x;
				break;
			case 5:
				m_rcSel[0].right+=x;
				break;
			case 6:
				m_rcSel[0].left+=x;
				m_rcSel[0].bottom+=y;
				break;
			case 7:
				m_rcSel[0].bottom+=y;
				break;
			case 8:
				m_rcSel[0].right+=x;
				m_rcSel[0].bottom+=y;
				break;
			case 9:
				m_rcSel[0].left+=x;
				m_rcSel[0].right+=x;
				m_rcSel[0].top+=y;
				m_rcSel[0].bottom+=y;
				break;
			}
			m_ptLButtonDown=point;
			DrawImage();
		}	
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CCatchScreenDlg::saveToFile(CString strSaveFile)
{
	CString strExt=".png";
	if(strSaveFile=="")
	{
		CFileDialog fd(FALSE,"PNG","未命名",4|2,"PNG 图片格式 (*.png)|*.png|BMP 图片格式 (*.bmp)|*.bmp|JPG 图片格式 (*.jpg)|*.jpg||");
		if(fd.DoModal()==IDCANCEL)
			return;
		strExt=fd.GetFileExt();
		strExt.Trim();strExt.MakeLower();
		strSaveFile=fd.GetPathName();
	}
	//////////////////////////////////////////////////////////////////////////
	CDC *dc=GetDC();
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(dc);
	CBitmap BufferBmp;
	BufferBmp.CreateCompatibleBitmap(dc,m_rcSel[0].Width(),m_rcSel[0].Height());
	BufferDC.SelectObject(&BufferBmp);
	CDC pdc;
	pdc.CreateCompatibleDC(&BufferDC);
	if (m_DrawRect)
	{
		m_pBitmap = m_DrawRect->m_pDib->GetBitmap();
	}
	pdc.SelectObject(m_pBitmap);
	BufferDC.BitBlt(0,0,m_rcSel[0].Width(),m_rcSel[0].Height(),&pdc,0,0,SRCCOPY);
	pdc.DeleteDC();

	CImage image;
	image.Attach(BufferBmp);
	if(strExt=="bmp")
		image.Save(strSaveFile, Gdiplus::ImageFormatBMP);
	else if(strExt=="jpg")
		image.Save(strSaveFile, Gdiplus::ImageFormatJPEG);
	else
		image.Save(strSaveFile, Gdiplus::ImageFormatPNG);

	BufferDC.DeleteDC();
	BufferBmp.DeleteObject();
	ReleaseDC(dc);
	//////////////////////////////////////////////////////////////////////////

	m_rcSel[0].right=m_rcSel[0].left;
	m_rcSel[0].bottom=m_rcSel[0].top;
	m_bSelected=false;

	OnCancel();
}

void CCatchScreenDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_ptLButtonDown=point;
	m_bLeftDown=true;
	if(m_bSelected){
// 		if(PtInRect(m_rcAction[0],point)){//设置为背景
// 			CopyScreenToBitmap(&m_rcSel[0],TRUE);
// 			char szCurDir[MAX_PATH]={0};
// 			GetCurrentDirectory(MAX_PATH,szCurDir);
// 			strcat(szCurDir,"\\WallPaper.bmp");
// 			/saveToFile(szCurDir);
// 			SystemParametersInfo(SPI_SETDESKWALLPAPER,0,szCurDir,0);
// 			m_bLeftDown=false;
// 		}
		CRect rc,CanvasRc;
		rc.left = m_ToolBarRect.left;
		rc.top = !m_bTopDownFlag?(m_ToolBarRect.bottom+2):(m_ToolBarRect.top - 2 - 37);
		rc.bottom = rc.top+37;rc.right = rc.left+247;
		CanvasRc.CopyRect(m_rcSel[0]);
		CanvasRc.InflateRect(-2,-2);
		if(PtInRect(m_rcAction[0],point))
		{
			m_DlgShowIndex = 0;
			m_bPanitCanvas = TRUE;
			m_PaintDlg[0].ShowWindow(SW_SHOWNORMAL|SW_SHOWNOACTIVATE);
			m_PaintDlg[0].MoveWindow(rc);
			if (m_DrawRect==NULL)
			{
				m_DrawRect = new Canvas;
				m_DrawRect->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|CS_DBLCLKS, CanvasRc, this, IDC_DRAWRECT, NULL);
			}
			m_DrawRect->SetPenType(PTP_PENCIL);
			m_DrawRect->SetPenSize(2);
			m_DrawRect->SetPenTrack((PEN_TRACK)PT_RECTANGLE);
			m_bLeftDown = false;
		}
		if(PtInRect(m_rcAction[1],point))
		{
			m_DlgShowIndex = 1;
			m_bPanitCanvas = TRUE;
			m_PaintDlg[1].ShowWindow(SW_SHOWNORMAL|SW_SHOWNOACTIVATE);
			m_PaintDlg[1].MoveWindow(rc);
			if (m_DrawRect==NULL)
			{
				m_DrawRect = new Canvas;
				m_DrawRect->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|CS_DBLCLKS, CanvasRc, this, IDC_DRAWRECT, NULL);
			}
			m_DrawRect->SetPenType(PTP_PENCIL);
			m_DrawRect->SetPenSize(2);
			m_DrawRect->SetPenTrack((PEN_TRACK)PT_ELLIPSE);
		}
		if(PtInRect(m_rcAction[2],point))
		{
			m_DlgShowIndex = 2;
			m_bPanitCanvas = TRUE;
			m_PaintDlg[2].ShowWindow(SW_SHOWNORMAL|SW_SHOWNOACTIVATE);
			m_PaintDlg[2].MoveWindow(rc);
			if (m_DrawRect==NULL)
			{
				m_DrawRect = new Canvas;
				m_DrawRect->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|CS_DBLCLKS, CanvasRc, this, IDC_DRAWRECT, NULL);
			}
			m_DrawRect->SetPenType(PTP_PENCIL);
			m_DrawRect->SetPenSize(5);
			m_DrawRect->SetPenTrack((PEN_TRACK)PT_ARROW);
		}
		if(PtInRect(m_rcAction[3],point))
		{
			m_DlgShowIndex = 3;
			m_bPanitCanvas = TRUE;
			m_PaintDlg[3].ShowWindow(SW_SHOWNORMAL|SW_SHOWNOACTIVATE);
			m_PaintDlg[3].MoveWindow(rc);
			if (m_DrawRect==NULL)
			{
				m_DrawRect = new Canvas;
				m_DrawRect->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|CS_DBLCLKS, CanvasRc, this, IDC_DRAWRECT, NULL);
			}
			m_DrawRect->SetPenType(PTP_PENCIL);
			m_DrawRect->SetPenSize(10);
			m_DrawRect->SetPenTrack((PEN_TRACK)PT_RANDOM);
		}
		if(PtInRect(m_rcAction[4],point))
		{
			m_DlgShowIndex = 4;
			m_bPanitCanvas = TRUE;
			m_PaintDlg[4].ShowWindow(SW_SHOWNORMAL|SW_SHOWNOACTIVATE);
			m_PaintDlg[4].MoveWindow(rc);		
			if (m_DrawRect==NULL)
			{
				m_DrawRect = new Canvas;
				m_DrawRect->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|CS_DBLCLKS, CanvasRc, this, IDC_DRAWRECT, NULL);
			}
			m_DrawRect->SetToolBarStyle(PBASEPEN);
			m_DrawRect->SetBasePenStyle(TEXT);
		}
		
		for(int i = 0;i<5;i++){
			if (i!=m_DlgShowIndex){
				m_PaintDlg[i].ShowWindow(SW_HIDE);
			}
		}

		if(PtInRect(m_rcAction[5],point))
		{
			if(m_DrawRect && !m_DrawRect->UnNDo())
				return OnCancel();
		}

		if(PtInRect(m_rcAction[6],point))
		{
			saveToFile();
			m_bLeftDown=false;
		}
		if (PtInRect(m_rcAction[7],point))
		{
			OnCancel();
		}
		if (PtInRect(m_rcAction[8],point))
		{
			CopyScreenToBitmap(&m_rcSel[0],TRUE);
			::PostMessage(g_QQWnd,UM_FEEDBACKMSG,3,0);
			OnCancel();
		}
		if (!m_bPanitCanvas)
		{
			m_hCursor=AfxGetApp()->LoadCursor(IDC_CURSOR1);      
			for(int i=8;i>=0;i--)
			{
				if(PtInRect(m_rcSel[i],point))
				{
					if(i==0)
						m_iCapture=9;
					else
						m_iCapture=i;
					m_hCursor = LoadCursor(NULL,MAKEINTRESOURCE(m_curSel[i]));
					break;
				}
			}
		}
		
	}
	if(!m_bPanitCanvas && !m_bSelected)
	{
		m_bSelect=true;
		m_rcSel[0].left=point.x;
		m_rcSel[0].top=point.y;
		m_rcSel[0].right=point.x;
		m_rcSel[0].bottom=point.y;
		DrawImage();
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CCatchScreenDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(!m_bLeftDown)
	{
		CDialog::OnLButtonUp(nFlags, point);
		return;
	}
	if (!m_bPanitCanvas)
	{
		m_bLButtonUp = TRUE;
		m_bLeftDown=false;
		m_bSelect=false;
		m_bSelected=true;
		m_iCapture=0;

		int x=m_rcSel[0].left;
		int y=m_rcSel[0].top;
		int w=m_rcSel[0].Width();
		int h=m_rcSel[0].Height();
		if(x>m_rcSel[0].right)x=m_rcSel[0].right;
		if(y>m_rcSel[0].bottom)y=m_rcSel[0].bottom;
		if(w<0)w=-w;
		if(h<0)h=-h;
		m_rcSel[0].left=x;
		m_rcSel[0].top=y;
		m_rcSel[0].right=x+w;
		m_rcSel[0].bottom=y+h;

		DrawImage();
		m_hCursor=AfxGetApp()->LoadCursor(IDC_CURSOR1);      
		if(PtInRect(m_rcSel[0],point))
			m_hCursor = LoadCursor(NULL,IDC_SIZEALL);


	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CCatchScreenDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(PtInRect(m_rcSel[0],point))
	{
		CopyScreenToBitmap(&m_rcSel[0],TRUE);
		::PostMessage(g_QQWnd,UM_FEEDBACKMSG,3,0);
		OnCancel();

	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CCatchScreenDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	EndDialog(IDCANCEL);
	CDialog::OnRButtonUp(nFlags, point);
}

HBRUSH CCatchScreenDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

BOOL CCatchScreenDlg::OnEraseBkgnd(CDC* pDC) 
{
	BITMAP bmp;
	m_pBitmap->GetBitmap(&bmp);

	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);

	dcCompatible.SelectObject(m_pBitmap);

	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,SRCCOPY);
	
	return TRUE;
}

BOOL CCatchScreenDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	SetCursor(m_hCursor);
	return TRUE;

}

HBITMAP CCatchScreenDlg::CopyScreenToBitmap(LPRECT lpRect,BOOL bSave)
{
	HDC       hScrDC, hMemDC;      
	HBITMAP    hBitmap, hOldBitmap;   
	int       nX, nY, nX2, nY2;      
	int       nWidth, nHeight;
	if (IsRectEmpty(lpRect)) return NULL;
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	hMemDC = CreateCompatibleDC(hScrDC);
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;
	if (nX < 0) nX = 0;
	if (nY < 0) nY = 0;
	if (nX2 > m_xScreen) nX2 = m_xScreen;
	if (nY2 > m_yScreen) nY2 = m_yScreen;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	if(bSave){
		CDC dcCompatible;
		dcCompatible.CreateCompatibleDC(CDC::FromHandle(hMemDC));
		dcCompatible.SelectObject(m_pBitmap);
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,dcCompatible, nX, nY, SRCCOPY);
	}
	else{
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,hScrDC, nX, nY, SRCCOPY);
	}
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	if(bSave){
		if (OpenClipboard()) {
			EmptyClipboard();
			if (m_bPanitCanvas)
			{
				if (!m_DrawRect->m_pDib->IsEmpty())
				{
					HANDLE hdib = m_DrawRect->m_pDib->GetHandle();
					SetClipboardData(CF_DIB, hdib);
				}
			}
			else
				SetClipboardData(CF_BITMAP, hBitmap);
			CloseClipboard();
      }
	}
	return hBitmap;
}

void CCatchScreenDlg::DrawImage()
{
	CDC *dc=GetDC();

	//////////////////////////////////////////////////////////////////////////
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(dc);
	CBitmap BufferBmp;
	BufferBmp.CreateCompatibleBitmap(dc,m_xScreen,m_yScreen);
	BufferDC.SelectObject(&BufferBmp);

	//////////////////////////////////////////////////////////////////////////
	CDC pdc;
	pdc.CreateCompatibleDC(&BufferDC);
	pdc.SelectObject(m_pBitmap);
	BufferDC.BitBlt(0,0,m_xScreen,m_yScreen,&pdc,0,0,SRCCOPY);
	pdc.DeleteDC();
	//////////////////////////////////////////////////////////////////////////
	if(m_rcSel[0].Width()==0 || m_rcSel[0].Height()==0)
		m_pngMask.DrawImage(&BufferDC,0,0,m_xScreen,m_yScreen,0,0,8,8);
	else
	{
		SetCorrectRect(m_rcSel[0]);
		int x=m_rcSel[0].left;
		int y=m_rcSel[0].top;
		int w=m_rcSel[0].Width();
		int h=m_rcSel[0].Height();

		m_pngMask.DrawImage(&BufferDC,0,0,m_xScreen,y,0,0,8,8);
		m_pngMask.DrawImage(&BufferDC,0,y+h,m_xScreen,m_yScreen-y-h,0,0,8,8);
		m_pngMask.DrawImage(&BufferDC,0,y,x,h,0,0,8,8);
		m_pngMask.DrawImage(&BufferDC,x+w,y,m_xScreen-x-w,h,0,0,8,8);

		BufferDC.Draw3dRect(x,y,w,h,RGB(0,0,0),RGB(0,0,0));
		//////////////////////////////////////////////////////////////////////////
		if (m_bLButtonUp)
		{
			DrawToolBar(BufferDC,CRect(x,y,x+w,y+h));
		}
		//////////////////////////////////////////////////////////////////////////
		CRect rc; rc.left=x+2;rc.top=y-24;rc.right=x+268;rc.bottom=y-2;
		if(rc.top<0){rc.top=y+2;rc.bottom=y+24;}
		if (rc.right > m_xScreen)
		{
			rc.right = m_xScreen - 2;
			rc.left = m_xScreen - 268 - 2;
		}
		if (m_bTopDownFlag)
		{
			rc.bottom-=25;
			rc.top-=25;
		}
		m_pngMask.DrawImage(&BufferDC,rc.left,rc.top,rc.Width(),rc.Height(),0,0,8,8);
		CFont font;
		font.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("宋体"));
		BufferDC.SelectObject((HFONT)font);
		BufferDC.SetBkMode(TRANSPARENT);
		BufferDC.SetTextColor(RGB(255,255,255));
		CString stxt;stxt.Format(" 起始位置: %4d.%4d  区域大小: %4d * %4d",x,y,w,h);
		BufferDC.DrawText(stxt,rc,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
		//////////////////////////////////////////////////////////////////////////
		DrawDot(BufferDC);
	}

	//////////////////////////////////////////////////////////////////////////
	//绘画界面
	if (m_bPanitCanvas) 
		dc->BitBlt(m_ToolBarRect.left,m_ToolBarRect.top,m_ToolBarRect.Width(),\
		m_ToolBarRect.Height(),&BufferDC,m_ToolBarRect.left,m_ToolBarRect.top,SRCCOPY);
	else dc->BitBlt(0,0,m_xScreen,m_yScreen,&BufferDC,0,0,SRCCOPY);
	
	//////////////////////////////////////////////////////////////////////////
	//清理资源
	BufferDC.DeleteDC();
	BufferBmp.DeleteObject();
	ReleaseDC(dc);

}

void CCatchScreenDlg::SetCorrectRect(CRect &rc)
{
	int x=rc.left;
	int y=rc.top;
	int w=rc.Width();
	int h=rc.Height();
	if(x>rc.right)x=rc.right;
	if(y>rc.bottom)y=rc.bottom;
	if(w<0)w=-w;
	if(h<0)h=-h;

	if (x+w>=m_xScreen)
	{
		x =  m_xScreen-w;
		rc.left = x;
		rc.right = m_xScreen;
	}
	if (rc.right - w < 0)
	{
		x=0;
		rc.left = 0;
		rc.right = w;
	}
	if (rc.top+h>m_yScreen)
	{
		y = m_yScreen - h;
		rc.bottom = m_yScreen;
		rc.top = y;
	}
	if (rc.bottom-h<0)
	{
		y = 0;
		rc.top = y;
		rc.bottom = y+h;
	}
}

void CCatchScreenDlg::DrawToolBar(CDC &BufferDC,CRect rc)
{
	m_rcAction[0].left = rc.left+rc.Width() - 290 + 5;
	if (m_rcAction[0].left<0)
	{
		m_rcAction[0].left = rc.left+5;
	}
	m_rcAction[0].top = rc.top+rc.Height()+2;
	m_rcAction[0].right = m_rcAction[0].left + 23;
	m_rcAction[0].bottom = m_rcAction[0].top+25;
	m_bTopDownFlag = FALSE;
	if (m_rcAction[0].bottom > m_yScreen)
	{
		m_rcAction[0].top = rc.top - 2 - 25;
		m_rcAction[0].bottom = rc.top + 2;
		m_bTopDownFlag = TRUE;
	}
	m_pngAction.DrawImage(&BufferDC,m_rcAction[0].left-5,m_rcAction[0].top,29,25,0,(m_iActionMask & 0x00000000F)*25,29,25);

	m_rcAction[1].left = m_rcAction[0].right+1+2;
	m_rcAction[1].top = m_rcAction[0].top;
	m_rcAction[1].right = m_rcAction[1].left + 23;
	m_rcAction[1].bottom = m_rcAction[1].top+25;
	m_pngAction.DrawImage(&BufferDC,m_rcAction[1].left-2,m_rcAction[1].top,26,25,29 ,((m_iActionMask & 0x0000000F0)==16)*25,26,25);

	m_rcAction[2].left = m_rcAction[1].right+1+2;
	m_rcAction[2].top = m_rcAction[1].top;
	m_rcAction[2].right = m_rcAction[2].left + 22;
	m_rcAction[2].bottom = m_rcAction[2].top+25; 
	m_pngAction.DrawImage(&BufferDC,m_rcAction[2].left-2,m_rcAction[2].top,25,25,55,((m_iActionMask & 0x000000F00)==256)*25,25,25);

	m_rcAction[3].left = m_rcAction[2].right+1+3;
	m_rcAction[3].top = m_rcAction[2].top;
	m_rcAction[3].right = m_rcAction[3].left + 23;
	m_rcAction[3].bottom = m_rcAction[3].top+25;
	m_pngAction.DrawImage(&BufferDC,m_rcAction[3].left-3,m_rcAction[3].top,27,25,80,((m_iActionMask & 0x00000F000)==256*16)*25,27,25);

	m_rcAction[4].left = m_rcAction[3].right+1+2;
	m_rcAction[4].top = m_rcAction[3].top;
	m_rcAction[4].right = m_rcAction[4].left + 23;
	m_rcAction[4].bottom = m_rcAction[4].top+25;
	m_pngAction.DrawImage(&BufferDC,m_rcAction[4].left-2,m_rcAction[4].top,26,25,107,((m_iActionMask & 0x0000F0000)==256*16*16)*25,26,25);

	m_rcAction[5].left = m_rcAction[4].right+1+10;
	m_rcAction[5].top = m_rcAction[4].top;
	m_rcAction[5].right = m_rcAction[5].left + 23;
	m_rcAction[5].bottom = m_rcAction[5].top+25;
	m_pngAction.DrawImage(&BufferDC,m_rcAction[5].left-10,m_rcAction[5].top,34,25,133 ,((m_iActionMask & 0x000F00000)==256*16*16*16)*25,34,25);

	m_rcAction[6].left = m_rcAction[5].right+1+2;
	m_rcAction[6].top = m_rcAction[5].top;
	m_rcAction[6].right = m_rcAction[6].left + 23;
	m_rcAction[6].bottom = m_rcAction[6].top+25;
	m_pngAction.DrawImage(&BufferDC,m_rcAction[6].left-2,m_rcAction[6].top,26,25,167 ,((m_iActionMask & 0x00F000000)==256*16*16*16*16)*25,26,25);

	m_rcAction[7].left = m_rcAction[6].right+1+10;
	m_rcAction[7].top = m_rcAction[6].top;
	m_rcAction[7].right = m_rcAction[7].left + 22;
	m_rcAction[7].bottom = m_rcAction[7].top+25;
	m_pngAction.DrawImage(&BufferDC,m_rcAction[7].left-10,m_rcAction[7].top,33,25,193,((m_iActionMask & 0x0F0000000)==256*16*16*16*16*16)*25,33,25);

	m_rcAction[8].left = m_rcAction[7].right+1+4;
	m_rcAction[8].top = m_rcAction[7].top;
	m_rcAction[8].right = m_rcAction[8].left + 59;
	m_rcAction[8].bottom = m_rcAction[8].top+25;
	m_pngAction.DrawImage(&BufferDC,m_rcAction[8].left-4,m_rcAction[8].top,64,25,226,((m_iActionMask & 0xF00000000)>=256*16*16*16*16*16)*25,64,25);

	m_ToolBarRect = CRect(m_rcAction[0].left-5,m_rcAction[0].top-2,m_rcAction[0].left+m_pngAction->GetWidth(),m_rcAction[0].top+m_pngAction->GetHeight()/2);
}
void CCatchScreenDlg::DrawDot(CDC &BufferDC)
{
	int x=m_rcSel[0].left;
	int y=m_rcSel[0].top;
	int w=m_rcSel[0].Width();
	int h=m_rcSel[0].Height();

	m_pngDot.DrawImage(&BufferDC,x-2,y-2);
	m_rcSel[1].left=x-2;m_rcSel[1].top=y-2;

	m_pngDot.DrawImage(&BufferDC,x+w/2-2,y-2);
	m_rcSel[2].left=x+w/2-2;m_rcSel[2].top=y-2;

	m_pngDot.DrawImage(&BufferDC,x+w-3,y-2);
	m_rcSel[3].left=x+w-3;m_rcSel[3].top=y-2;

	m_pngDot.DrawImage(&BufferDC,x-2,y+h/2-2);
	m_rcSel[4].left=x-2;m_rcSel[4].top=y+h/2-2;

	m_pngDot.DrawImage(&BufferDC,x+w-3,y+h/2-2);
	m_rcSel[5].left=x+w-3;m_rcSel[5].top=y+h/2-2;

	m_pngDot.DrawImage(&BufferDC,x-2,y+h-3);
	m_rcSel[6].left=x-2;m_rcSel[6].top=y+h-3;

	m_pngDot.DrawImage(&BufferDC,x+w/2-2,y+h-3);
	m_rcSel[7].left=x+w/2-2;m_rcSel[7].top=y+h-3;

	m_pngDot.DrawImage(&BufferDC,x+w-3,y+h-3);
	m_rcSel[8].left=x+w-3;m_rcSel[8].top=y+h-3;

	for(int i=1;i<9;i++)
	{
		m_rcSel[i].right=m_rcSel[i].left+5;
		m_rcSel[i].bottom=m_rcSel[i].top+5;
	}
}

void CCatchScreenDlg::OnClose()
{
	for (int i =0;i<5;i++)
	{
		m_PaintDlg[i].DestroyWindow();
	}
	if (m_DrawRect)
	{
		m_DrawRect->DestroyWindow();
		delete m_DrawRect;
		m_DrawRect = NULL;
	}
	
	CDialog::OnClose();
}

