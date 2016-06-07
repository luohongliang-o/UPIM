/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：Dib.cpp 
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：DIB图形处理类模块文件（调用DibAPI.CPP中的函数）
*======================================================
* 版本变更记录:
*      v1.0  2002-11-11   8:30  罗伟  
*
* 
*/

/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windowsx.h>		

#include "..\Include\Dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// WIDTHBYTES宏执行DWORD扫描行排列.  这个参数"bits"是扫描行(biWidth * biBitCount)位数
// 这个宏返回 DWORD排列这些位句柄需要的位数
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 

IMPLEMENT_SERIAL(CDib, CObject, 0)

////////////////////////////////////////////////////////////////////////////

/*
*--------------------------------------------------------------------------------
*  CDib()
*  构造函数
*  Parameter :  
*                 无
*  Return value:
*                 无
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13 罗伟 完成数据的初始化
*
*/

CDib::CDib()
{
	m_hDib		= NULL;
	m_hBitmap   = NULL;
	m_pPalette  = NULL;
	m_pBitmap   = NULL;
	m_pMemDC	= NULL;
}  
         
/*
*--------------------------------------------------------------------------------
*  ~CDib()
*  析构函数
*  Parameter :  
*                 无
*  Return value:
*                 无
*====================================================
*/
                                      
CDib::~CDib()
{
	Destroy();
	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
	if (m_pPalette != NULL)
	{
		delete m_pPalette;
		m_pPalette = NULL;
	}
	if (m_pPalette != NULL)
	{
		delete m_pPalette;
		m_pPalette = NULL;
	}
	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

}

void CDib::Destroy()
{
	if (m_hDib != NULL)
	{
		DestroyDIB(m_hDib);
		m_hDib = NULL;
	}
}

BOOL CDib::Create(DWORD dwWidth, DWORD dwHeight)
{
	HDIB hDib = CreateDefaultDIB(dwWidth, dwHeight);
	if (! hDib)
		return FALSE;

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Create()
*  创建CDib对象 
*  Parameter :  
*              [IN]    dwWidth
*                 宽度
*              [IN]    dwHeight
*                 高度
*              [IN]    wBitCount
*                 点数
*  Return value:
*              返回TRUE表示创建成功，返回FALSE失败
*====================================================
*
*/

BOOL CDib::Create(DWORD dwWidth, DWORD dwHeight, WORD wBitCount)
{
	HDIB hDib = CreateDIB(dwWidth, dwHeight, wBitCount);
	if (! hDib)
		return FALSE;

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Create()
*  创建CDib对象 
*  Parameter :  
*              [IN]    lpDIB
*                 CDib字节指针
*  Return value:
*              返回TRUE表示创建成功，返回FALSE失败
*===================================================
*
*/

BOOL CDib::Create(LPBYTE lpDIB)
{
	if (lpDIB == NULL)
		return FALSE;
	
	DWORD dwSize = DIBlockSize(lpDIB);

    HDIB hDib  = GlobalAlloc(GHND, dwSize); 
    // 检DIB handle is valid 
    if (! hDib) 
        return FALSE; 
 
    LPBYTE lpbi  = (LPBYTE)GlobalLock(hDib); 
	if (! lpbi)
        return FALSE; 
		
	CopyMemory(lpbi, lpDIB, dwSize);
	GlobalUnlock(hDib);

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Create()
*  创建CDib对象 
*  Parameter :  
*              [IN]    lpDIB
*                 CDib字节指针
*              [IN]    wBitCount
*                 CDib点数
*  Return value:
*              返回TRUE表示创建成功，返回FALSE失败
*====================================================
*
*/

BOOL CDib::Create(LPBYTE lpDIB,
				  WORD  wBitCount)		// bits/pixel 
{
	if (lpDIB == NULL)
		return FALSE;
	if (! Create(lpDIB))
		return FALSE;

	WORD wBits = ((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
	if (wBitCount == wBits)
		return TRUE;

	HDIB hNewDib = ConvertDIBFormat(m_hDib, wBitCount, NULL); 
	if (! hNewDib)
		return FALSE;

	Destroy();
	m_hDib = hNewDib;
	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Create()
*  创建CDib对象 ：DIB 片断
*  Parameter :  
*              [IN]    hBitmap
*                 位图句柄
*  Return value:
*              返回TRUE表示创建成功，返回FALSE失败
*====================================================
*  版本变更记录
*
*/

BOOL CDib::Create(HBITMAP hBitmap)		
{
	if (! hBitmap)
        return FALSE; 

	HDIB hDib = DIBSectionToDIB(hBitmap); 
    if (! hDib) 
        return FALSE; 

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Create()
*  创建CDib对象 
*  Parameter :  
*              [IN]    hBitmap
*                 位图句柄
*              [IN]    wBitCount
*                 CDib像数位数
*  Return value:
*              返回TRUE表示创建成功，返回FALSE失败
*====================================================
*
*/

BOOL CDib::Create(HBITMAP hBitmap,		
				  WORD  wBitCount)		
{
	HDIB hNewDib;

	if (! hBitmap)
        return FALSE; 

	HDIB hDib = DIBSectionToDIB(hBitmap); 
    if (! hDib) 
        return FALSE; 

	DIBSECTION ds;
	GetObject(hBitmap, sizeof(DIBSECTION), &ds);

	if (wBitCount == ds.dsBmih.biBitCount)
		hNewDib = hDib;
	else
	{
		hNewDib = ConvertDIBFormat(hDib, wBitCount, NULL); 
		// cleanup hDib
		GlobalFree(hDib);
	}
	if (! hNewDib)
		return FALSE;

	Destroy();
	m_hDib = hNewDib;
	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Create()
*  创建CDib对象 
*  Parameter :  
*              [IN]    hBitmap
*                 位图句柄
*              [IN]    hPalette
*                 调色板句柄
*  Return value:
*               返回TRUE表示创建成功，返回FALSE失败
*====================================================
*  版本变更记录
*
*
*/

BOOL CDib::Create(HBITMAP hBitmap,		
			      HPALETTE hPalette)	
{
	if (! hBitmap)
        return FALSE; 

	HDIB hDib = BitmapToDIB(hBitmap, hPalette); 
    if (! hDib) 
        return FALSE; 

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Create()
*  创建CDib对象 
*  Parameter :  
*              [IN]    hBitmap
*                 位图句柄
*              [IN]    hPalette
*                 调色板句柄
*              [IN]    wBitCount
*                 CDib对象点数
*  Return value:
*              返回TRUE表示创建成功，返回FALSE失败
*====================================================
*
*/

BOOL CDib::Create(HBITMAP hBitmap,		
			      HPALETTE hPalette,	
				  WORD  wBitCount)		
{
	if (! hBitmap)
        return FALSE; 

	HDIB hDib = BitmapToDIB(hBitmap, hPalette, wBitCount); 
    if (! hDib) 
        return FALSE; 

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Create()
*  创建CDib对象 
*  Parameter :  
*              [IN]    rcScreen
*                 矩形框
*  Return value:
*              返回TRUE表示创建成功，返回FALSE失败
*====================================================
*
*/

BOOL CDib::Create(CRect rcScreen)
{
	HDIB hDib = CopyScreenToDIB(rcScreen); 
    if (! hDib) 
        return FALSE; 

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Create()
*  创建CDib对象 
*  Parameter :  
*              [IN]    hWnd
*                 窗口句柄
*              [IN]    fPrintArea
*                 打印区域
*  Return value:
*              返回TRUE表示创建成功，返回FALSE失败
*====================================================
*
*/

BOOL CDib::Create(HWND hWnd, WORD fPrintArea)
{
	HDIB hDib = CopyWindowToDIB(hWnd, fPrintArea); 
    if (! hDib) 
        return FALSE; 

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Create()
*  创建CDib对象 
*  Parameter :  
*              [IN]    hWnd
*                 窗口句柄
*              [IN]    rcClientArea
*                 客户区域
*  Return value:
*               返回TRUE表示创建成功，返回FALSE失败
*====================================================
*
*/

BOOL CDib::Create(HWND hWnd, CRect rcClientArea)
{
	HDIB hDib = CopyClientRectToDIB(hWnd, rcClientArea); 
    if (! hDib) 
        return FALSE; 

	Destroy();
	m_hDib = hDib;
	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Create()
*  创建CDib对象 
*  Parameter :  
*              [IN]    ar
*                 文件
*  Return value:
*              无
*====================================================
*
*/

void CDib::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	ar.Flush();
	if (ar.IsStoring())
	{
		Write(ar.GetFile());
	}
    else
    {
   		Read(ar.GetFile());
    }
}

/*
*--------------------------------------------------------------------------------
*  Load()
*  创建CDib对象 
*  Parameter :  
*              [IN]    uIDS
*                 Id号
*              [IN]    lpszDibType
*                 Dib类型指针
*  Return value:
*            返回TRUE表示导入成功，返回FALSE失败
*====================================================
*
*/

BOOL CDib::Load(UINT uIDS, LPCSTR lpszDibType)
{                                
	LPCSTR lpszDibRes = MAKEINTRESOURCE(uIDS);

	return Load(lpszDibRes, lpszDibType);
}

/*
*--------------------------------------------------------------------------------
*  Load()
*  创建CDib对象 
*  Parameter :  
*              [lpszDibRes]    uIDS
*                 资源
*              [lpszDibType]    lpszDibType
*                 Dib类型指针
*  Return value:
*            返回TRUE表示导入成功，返回FALSE失败
*====================================================
*
*/
BOOL CDib::Load(LPCSTR lpszDibRes, LPCSTR lpszDibType)
{                                
	HINSTANCE hInst = AfxGetInstanceHandle();
	HRSRC   hRes    = ::FindResource(hInst, lpszDibRes, lpszDibType);
	HGLOBAL hData   = ::LoadResource(hInst, hRes);

	// 若资源存在
	if (hRes == NULL || hData == NULL)
		return FALSE;

	//得到资源缓冲
	LPBYTE lpBuf = (LPBYTE)::LockResource(hData);
	// is DIB ?
	if (((LPBITMAPFILEHEADER)lpBuf)->bfType != DIB_HEADER_MARKER/*"BM"*/)
		return FALSE;

	// 使用这个缓冲区创建 CDib
	LPBYTE lpDIB = lpBuf + sizeof(BITMAPFILEHEADER);
	return Create(lpDIB);
}
/*
*--------------------------------------------------------------------------------
*  Load()
*  创建CDib对象 
*  Parameter :  
*              [IN]    lpszDibFile
*                 文件名
*  Return value:
*              返回TRUE表示导入成功，返回FALSE失败
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13 罗伟  创建CDib对象 
*
*/

BOOL CDib::Load(LPCTSTR lpszDibFile)
{                                
	TRY
	{
		CFile file(lpszDibFile, CFile::modeRead|CFile::shareDenyNone);

		if (! Read(&file))
			return FALSE;
	}
	CATCH (CException, e)
	{
		return FALSE;
	}
	END_CATCH

	return TRUE;
}  
             
/*
*--------------------------------------------------------------------------------
*  Save()
*  保存CDib对象 
*  Parameter :  
*              [IN]    lpszDibFile
*                 文件名
*  Return value:
*             返回TRUE表示保存成功，返回FALSE失败
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13 罗伟  创建CDib对象 
*
*/

BOOL CDib::Save(LPCSTR lpszDibFile)
{
	TRY
	{
		CFile file(lpszDibFile, CFile::modeCreate|CFile::modeWrite);

		if (! Write(&file))
			return FALSE;
	}
	CATCH (CException, e)
	{
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

/*
*--------------------------------------------------------------------------------
*  Read()
*  读CDib对象 
*  Parameter :  
*              [IN]    pFile
*                 文件名指针
*  Return value:
*             返回TRUE表示读入成功，返回FALSE失败
*====================================================
*/

BOOL CDib::Read(CFile *pFile)
{
	WaitCursorBegin();

	LPBITMAPINFOHEADER lpbi;
	DWORD dwSize;
	TRY
	{
		// 读DIB文件头结构
		BITMAPFILEHEADER bmfHdr;
		pFile->Read(&bmfHdr, sizeof(BITMAPFILEHEADER));

		if (bmfHdr.bfType != DIB_HEADER_MARKER/*"BM"*/)
		{
			WaitCursorEnd();
			return FALSE;
		}
		DWORD dwLength = pFile->GetLength();
		if (bmfHdr.bfSize != dwLength)
			bmfHdr.bfSize = dwLength;

		//读 DIB缓冲区
		dwSize = bmfHdr.bfSize - sizeof(BITMAPFILEHEADER);
		lpbi = (LPBITMAPINFOHEADER)GlobalAllocPtr(GHND, dwSize);
		DWORD dwCount = pFile->Read(lpbi, dwSize);
		
		if (dwCount != dwSize)
		{
			GlobalFreePtr(lpbi);
			WaitCursorEnd();
			return FALSE;
		}

		//若不是window DIB 返回 NULL. 
	    if (lpbi->biSize != sizeof(BITMAPINFOHEADER)) 
		{
			GlobalFreePtr(lpbi);
			WaitCursorEnd();
			return FALSE;
		}
		
		//填充颜色表
		int nNumColors = (UINT)lpbi->biClrUsed;
		if (nNumColors == 0) 
		{ 
			//24-bit DIB没有颜色表 
	        if (lpbi->biBitCount != 24) 
		        nNumColors = 1 << lpbi->biBitCount; // standard size table 
		} 
 
		// 若它们为0时填充一些默认值
	    if (lpbi->biClrUsed == 0) 
		    lpbi->biClrUsed = nNumColors; 
		if (lpbi->biSizeImage == 0) 
			lpbi->biSizeImage = ((((lpbi->biWidth * (DWORD)lpbi->biBitCount) + 31) & ~31) >> 3) * lpbi->biHeight; 
 	}
	CATCH (CException, e)
	{
		GlobalFreePtr(lpbi);
		WaitCursorEnd();
		return FALSE;
	}
	END_CATCH

	//使用DIB缓冲区创建CDib 
	BOOL bSuccess = Create((LPBYTE)lpbi);
	GlobalFreePtr(lpbi);
	WaitCursorEnd();

	return bSuccess;
}

/*
*--------------------------------------------------------------------------------
*  Write()
*  写CDib对象 
*  Parameter :  
*              [IN]    pFile
*                 文件名指针
*  Return value:
*             返回TRUE表示写文件成功，返回FALSE失败
*====================================================
*
*/

BOOL CDib::Write(CFile *pFile)
{
	WaitCursorBegin();
     //位图头结构 
    BITMAPFILEHEADER    bmfHdr;
	// 指向一个 DIB 信息头结构指针 
    LPBITMAPINFOHEADER  lpBI;       
    DWORD               dwDIBSize; 

	//得到一个指针指向DIB内存, 开始包含BITMAPINF结构信息 
    lpBI = (LPBITMAPINFOHEADER)GlobalLock(m_hDib); 
    if (!lpBI) 
	{
		GlobalUnlock(m_hDib);
		WaitCursorEnd();
        return FALSE; 
	}
 
    //检查是否是WINDOWS DIB
    if (lpBI->biSize != sizeof(BITMAPINFOHEADER)) 
    { 
        GlobalUnlock(m_hDib); 
		WaitCursorEnd();
        return FALSE; 
    } 
 
    // 填充文件头 
 
    // 填充文件类型 (前两个字符必须是 "BM" 对于BMP文件) 
 
    bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM" 
 
 
    dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPBYTE)lpBI);   
 
    // 计算图形尺寸
 
    if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) 
        dwDIBSize += lpBI->biSizeImage; 
    else 
    { 
         //仅仅为位图
		DWORD dwBmBitsSize;  
 
        // 非压缩格式 
        dwBmBitsSize = WIDTHBYTES1((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * 
                lpBI->biHeight; 
 
        dwDIBSize += dwBmBitsSize; 
 
        lpBI->biSizeImage = dwBmBitsSize; 
    } 
 
 
    // 计算文件尺寸靠增加这个DIB的BITMAPFILEHEADER结构尺寸 
                    
    bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER); 
    bmfHdr.bfReserved1 = 0; 
    bmfHdr.bfReserved2 = 0; 
 
    // 计算位图的偏移量：这个位图文件加上DIB的头尺寸和颜色表尺寸  
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize + 
            PaletteSize((LPBYTE)lpBI); 
 
 	TRY
	{
	    // 写文件头
		pFile->Write(&bmfHdr, sizeof(BITMAPFILEHEADER));
		//写DIB 缓冲区
		pFile->Write(lpBI, dwDIBSize);
	}
	CATCH (CException, e)
	{
        GlobalUnlock(m_hDib); 
		WaitCursorEnd();
		return FALSE;
	}
	END_CATCH

	GlobalUnlock(m_hDib); 
	WaitCursorEnd();
	
	return TRUE;
}

/*
*--------------------------------------------------------------------------------
*  Display()
*  显示CDib对象 
*  Parameter :  
*              [IN]    pDC
*                 DC指针
*              [IN]    xDest
*                 目标X
*              [IN]    yDest
*                 目标Y
*              [IN]    nWidthDest
*                 目标宽度
*              [IN]    nHeightDest
*                 目标高度
*              [IN]    xSrc
*                 源X
*              [IN]    ySrc
*                 源Y
*              [IN]    dwRop
*                 显示方式
*  Return value:
*              返回TRUE表示显示成功，返回FALSE失败
*====================================================
*
*/

BOOL CDib::Display(CDC* pDC, int xDest, int yDest, int nWidthDest, int nHeightDest, 
 				   int xSrc, int ySrc, DWORD dwRop)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	CBitmap* pOldBmp = MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette, TRUE);
    pDC->RealizePalette();

	BOOL bSuccess = pDC->BitBlt( xDest, yDest, 
							nWidthDest, nHeightDest,
						    &MemDC, 
							xSrc, ySrc, 
							dwRop);

	MemDC.SelectObject(pOldBmp);
	pDC->SelectPalette(pOldPal, TRUE);

	return bSuccess;
}

/*
*--------------------------------------------------------------------------------
*  Display()
*  显示CDib对象 
*  Parameter :  
*              [IN]    pDC
*                 DC指针
*              [IN]    xDest
*                 目标X
*              [IN]    yDest
*                 目标Y
*              [IN]    nWidthDest
*                 目标宽度
*              [IN]    nHeightDest
*                 目标高度
*              [IN]    xSrc
*                 源X
*              [IN]    ySrc
*                 源Y
*              [IN]    nWidthDest
*                 源宽度
*              [IN]    nHeightDest
*                 源宽度
*              [IN]    dwRop
*                 显示方式
*  Return value:
*               返回TRUE表示显示成功，返回FALSE失败
*====================================================
*
*/

BOOL CDib::Display(CDC * pDC, int xDest, int yDest, int nWidthDest, int nHeightDest, 
				   int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, DWORD dwRop)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	CBitmap* pOldBmp = MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette, TRUE);
    pDC->RealizePalette();

	BOOL bSuccess = pDC->StretchBlt( xDest, yDest, 
								nWidthDest, nHeightDest,
						        &MemDC, 
								xSrc, ySrc, 
								nWidthSrc, nHeightSrc, 
								dwRop);

	MemDC.SelectObject(pOldBmp);
	pDC->SelectPalette(pOldPal, TRUE);

	return bSuccess;
}

/*
*--------------------------------------------------------------------------------
*  Display()
*  显示CDib对象 
*  Parameter :  
*              [IN]    pDC
*                 DC指针
*              [IN]    x
*                 源X
*              [IN]    y
*                 源Y
*              [IN]    dwRop
*                 显示方式
*  Return value:
*               返回TRUE表示显示成功，返回FALSE失败
*====================================================
*
*/
BOOL CDib::Display(CDC * pDC, int x, int y, DWORD dwRop)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	CBitmap* pOldBmp = MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette, TRUE);
    pDC->RealizePalette();

	BOOL bSuccess = pDC->BitBlt(x, y, 
								GetWidth(), GetHeight(),
								&MemDC, 
								0, 0, 
								dwRop);

	MemDC.SelectObject(pOldBmp);
	pDC->SelectPalette(pOldPal, TRUE);

	return bSuccess;
}

/*
*--------------------------------------------------------------------------------
*  Display()
*  显示CDib对象 
*  Parameter :  
*              [IN]    pDC
*                 DC指针
*              [IN]    rcDest
*                 目标块
*              [IN]    rcSrc
*                 源块
*              [IN]    dwRop
*                 显示方式
*  Return value:
*               返回TRUE表示显示成功，返回FALSE失败
*====================================================
*
*/

BOOL CDib::Display(CDC* pDC, CRect rcDest, CRect rcSrc, DWORD dwRop)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	CBitmap* pOldBmp = MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette, TRUE);
    pDC->RealizePalette();

	BOOL bSuccess = pDC->StretchBlt( rcDest.left, rcDest.top, 
								rcDest.Width(), rcDest.Height(),
						        &MemDC, 
								rcSrc.left, rcSrc.top, 
								rcSrc.Width(), rcSrc.Height(),
								dwRop);

	MemDC.SelectObject(pOldBmp);
	pDC->SelectPalette(pOldPal, TRUE);

	return bSuccess;
}

/*
*--------------------------------------------------------------------------------
*  BuildBitmap()
*  创建CBitmap对象 
*  Parameter :  
               无
*  Return value:
*               返回TRUE表示创建成功，返回FALSE失败
*====================================================
*
*/

BOOL CDib::BuildBitmap()
{
	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
		m_hBitmap = NULL;
	}
	m_hBitmap = DIBToDIBSection(m_hDib);
	if (m_hBitmap == NULL)
		return FALSE;

	if (m_pBitmap)
		delete m_pBitmap;
	m_pBitmap = new CBitmap;
	m_pBitmap->Attach(m_hBitmap);
    

	return TRUE;
}

/*
*--------------------------------------------------------------------------------
*  BuildPalette()
*  创建调色板对象  
*  Parameter :  
               无
*  Return value:
*               返回TRUE表示创建成功，返回FALSE失败
*====================================================
*
*/

BOOL CDib::BuildPalette()
{
	if (m_pPalette != NULL)
	{
		delete m_pPalette;
		m_pPalette = NULL;
	}
	HPALETTE hPalette = CreateDIBPalette(m_hDib);
	if (hPalette == NULL)
		return FALSE;
	m_pPalette = new CPalette;
	m_pPalette->Attach(hPalette);

	return TRUE;
}

/*
*--------------------------------------------------------------------------------
*  UpdateInternal()
*  更新位图及调色板对象 
*  Parameter :  
               无
*  Return value:
*               返回TRUE表示成功，返回FALSE失败
*====================================================
*/

BOOL CDib::UpdateInternal()
{
	BuildPalette();
	return BuildBitmap();
}

/*
*--------------------------------------------------------------------------------
*  GetPalette()
*  创建调色板对象 
*  Parameter :  
               无
*  Return value:
*               
*====================================================
*
*/

CPalette* CDib::GetPalette()
{
	return m_pPalette;
}

/*
*--------------------------------------------------------------------------------
*  GetBitmap()
*  创建CBitmap位图对象 
*  Parameter :  
               无
*  Return value:
*              位图对象
*====================================================
*
*/

CBitmap* CDib::GetBitmap()
{
	return m_pBitmap;
}

/*
*--------------------------------------------------------------------------------
*  IsEmpty()
*  检查CDib对象是否为空
*  Parameter :  
               无
*  Return value:
*               返回TRUE表示为空，返回FALSE不为空
*====================================================
*
*/

BOOL CDib::IsEmpty()
{
	if (m_hDib == NULL)
		return TRUE;

	if (! GlobalLock(m_hDib))
		return TRUE;

	GlobalUnlock(m_hDib);
	return FALSE;
}

/*
*--------------------------------------------------------------------------------
*  GetCompression()
*  压缩CDib对象 
*  Parameter :  
               无
*  Return value:
*              压缩后指针长
*====================================================
*
*/

DWORD CDib::GetCompression()
{
    LPBITMAPINFOHEADER lpBI = (LPBITMAPINFOHEADER)GlobalLock(m_hDib); 
    if (!lpBI) 
	{
		GlobalUnlock(m_hDib);
        return 0; 
	}
 
	DWORD dwCompression = lpBI->biCompression;
	GlobalUnlock(m_hDib);

	return dwCompression;
}

/*
*--------------------------------------------------------------------------------
*  GetBitCount()
*  得到CDib对象的位数
*  Parameter :  
               无
*  Return value:
*           CDib对象的位数 
*====================================================
*
*/

WORD CDib::GetBitCount()
{
    LPBITMAPINFOHEADER lpBI = (LPBITMAPINFOHEADER)GlobalLock(m_hDib); 
    if (!lpBI) 
	{
		GlobalUnlock(m_hDib);
        return 0; 
	}
 
	WORD wBitCount = lpBI->biBitCount;
	GlobalUnlock(m_hDib);

	return wBitCount;
}

/*
*--------------------------------------------------------------------------------
*  GetWidth()
*  得到DIB对象的宽度
*  Parameter :  
               无
*  Return value:
*           DIB对象的宽度   
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13 罗伟  创建CDib对象 
*
*/

LONG CDib::GetWidth()
{
	// 得到 DIB缓冲区的指针
    LPBYTE lpDIB = (LPBYTE)GlobalLock(m_hDib); 
	if (! lpDIB)
	{
		GlobalUnlock(m_hDib);
		return 0;
	}

	LONG lWidth = (LONG)DIBWidth(lpDIB);
	GlobalUnlock(m_hDib);

	return lWidth; 
}

/*
*--------------------------------------------------------------------------------
*  GetHeight()
*  得到DIB对象的高度
*  Parameter :  
               无
*  Return value:
*              DIB对象的高度
*====================================================
*
*/

LONG CDib::GetHeight()
{
	// 得到 DIB缓冲区的指针
    LPBYTE lpDIB = (LPBYTE)GlobalLock(m_hDib); 
	if (! lpDIB)
	{
		GlobalUnlock(m_hDib);
		return 0;
	}

	LONG lHeight = (LONG)DIBHeight(lpDIB);
	GlobalUnlock(m_hDib);

	return lHeight; 
}

/*
*--------------------------------------------------------------------------------
*  GetWidthBytes()
*  得到DIB对象的宽度位数
*  Parameter :  
               无
*  Return value:
*              DIB对象的宽度位数
*====================================================
*
*/

LONG CDib::GetWidthBytes()
{
	return WIDTHBYTES1((GetWidth())*((DWORD)GetBitCount()));
}

/*
*--------------------------------------------------------------------------------
*  GetPixel()
*  得到指定点的像素值
*  Parameter :  
*                [IN] x
*                   点的x坐标
*                [IN] y
*                   点的y坐标
*  Return value: 
*                RGB颜色值
*====================================================
*
*/

COLORREF CDib::GetPixel(LONG x, LONG y)
{
	COLORREF cColor;
	switch (GetBitCount())
	{
		case 1 :	if (1<<(7-x%8) & 
						*(BYTE*)(GetBitsPtr()+GetPixelOffset(x, y)))
						cColor = RGB(255,255,255);
					else
						cColor = RGB(0,0,0);
					break;
		case 4 :	
				{
					PALETTEENTRY PaletteColors[16];
					m_pPalette->GetPaletteEntries(0, 16, PaletteColors);
					int nIndex = (*(BYTE*)(GetBitsPtr()+GetPixelOffset(x, y)) & 
								   (x%2 ? 0x0f : 0xf0)) >> (x%2 ? 0 : 4);
					cColor = RGB(PaletteColors[nIndex].peRed,
								 PaletteColors[nIndex].peGreen,
								 PaletteColors[nIndex].peBlue);
				}
					break;
		case 8 :	
				{
					PALETTEENTRY PaletteColors[256];
					m_pPalette->GetPaletteEntries(0, 256, PaletteColors);
					int nIndex = *(BYTE*)(GetBitsPtr()+GetPixelOffset(x, y));
					cColor = RGB(PaletteColors[nIndex].peRed,
								 PaletteColors[nIndex].peGreen,
								 PaletteColors[nIndex].peBlue);
				}
					break;
		default:	cColor = RGB(*(BYTE*)(GetBitsPtr()+GetPixelOffset(x, y)),
								 *(BYTE*)(GetBitsPtr()+GetPixelOffset(x, y)+1),
								 *(BYTE*)(GetBitsPtr()+GetPixelOffset(x, y)+2));
					break;
	}
	return cColor;
}

/*
*--------------------------------------------------------------------------------
*  GetPixelOffset()
*  得到CDib像素偏移 
*  Parameter :  
*                [IN] x
*                   点的x坐标
*                [IN] y
*                   点的y坐标
*  Return value: 
*               偏移量 
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13 罗伟  创建CDib对象 
*
*/

LONG CDib::GetPixelOffset(LONG x, LONG y)
{
	return (GetHeight()-y-1)*GetWidthBytes()+x/(8/GetBitCount());
}

/*
*--------------------------------------------------------------------------------
*  GetBitsPtr()
*  得到CDib对象位
*  Parameter :  
*                无
*  Return value: 
*                字节指针
*====================================================
*
*/

LPBYTE CDib::GetBitsPtr()
{
    LPBYTE lpDIB = (LPBYTE)GlobalLock(m_hDib); 
    if (! lpDIB) 
	{
		GlobalUnlock(m_hDib);
		return NULL;
	}

	LPBYTE lpData = FindDIBBits(lpDIB);
	GlobalUnlock(m_hDib);

	return lpData;
}

/*
*--------------------------------------------------------------------------------
*  GetHandle()
*  得到CDib对象的句柄
*  Parameter :  
*                无
*  Return value: 
*                对象句柄
*====================================================
*
*/

HANDLE CDib::GetHandle()
{
	return m_hDib;
}

/*
*--------------------------------------------------------------------------------
*  GetColorNumber()
*  得到CDib对象的颜色数 
*  Parameter :  
*                无
*  Return value: 
*                颜色位数
*====================================================
*
*/

WORD CDib::GetColorNumber()
{
    LPBYTE lpBI = (LPBYTE)GlobalLock(m_hDib); 
    if (! lpBI) 
	{
		GlobalUnlock(m_hDib);
		return 0;
	}
 
	WORD wColors = DIBNumColors(lpBI);
	GlobalUnlock(m_hDib);

	return wColors;
}

/*
*--------------------------------------------------------------------------------
*  GetPaletteSize()
*  得到CDib对象的调色板尺寸 
*  Parameter :  
*                无
*  Return value: 
*                调色板位数
*====================================================
*
*/

WORD CDib::GetPaletteSize()
{
    LPBYTE lpBI = (LPBYTE)GlobalLock(m_hDib); 
    if (! lpBI) 
	{
		GlobalUnlock(m_hDib);
		return 0;
	}
 
	WORD wPalSize = PaletteSize(lpBI);
	GlobalUnlock(m_hDib);

	return wPalSize;
}

/*
*--------------------------------------------------------------------------------
*  BeginPaint
*  准备开始对CDib对象的内存CDC进行操作
*  Parameter :  
*                [IN] pDC
*                   要操作的设备上下文指针
*    
*  Return value: 
*                选入对应内存的设备上下文指针
*====================================================
*
*/

CDC* CDib::BeginPaint(CDC *pDC)
{
	if (m_pMemDC)
		delete m_pMemDC;
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(pDC);
	m_pPaletteTmp = m_pMemDC->SelectPalette(m_pPalette, TRUE);
	m_pMemDC->RealizePalette();
	m_pBitmapTmp = (CBitmap *)m_pMemDC->SelectObject(m_pBitmap);
	return m_pMemDC;
}

/*
*--------------------------------------------------------------------------------
*  EndPaint()
*  结束CDib对象的内存CDC进行操作
*  Parameter :  

*  Return value: 
*     
*====================================================
*
*/

void CDib::EndPaint()
{
	m_pMemDC->SelectObject(m_pBitmapTmp);
	m_pMemDC->SelectPalette(m_pPaletteTmp, TRUE);
	delete m_pMemDC;
	m_pMemDC = NULL;

	Create(m_hBitmap);
}

/*
*--------------------------------------------------------------------------------
*  DisplayPalette
*  显示CDib对象 的调色板
*  Parameter :  
*                [IN] pDC
*                   要显示的设备上下文句柄
*                [IN] rc
*                   显示的矩形区
*  Return value: 
*                返回TRUE表示显示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::DisplayPalette(CDC* pDC, CRect rc)
{
	return ::DisplayPalette(pDC->GetSafeHdc(), &rc, (HPALETTE)m_pPalette->GetSafeHandle());
}

/*
*--------------------------------------------------------------------------------
*  DisplayTransparent()
*  透明显示指定的DIB对象 
*  Parameter :  
*
*                [IN]pDC
*                    设备上下文指针
*                [IN] x
*                   点的x坐标
*                [IN] y
*                   点的y坐标
*				 [IN]cTransparentColor	
*					显示时采用的透明色
*  Return value: 
*                返回TRUE表示显示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::DisplayTransparent(CDC * pDC, int x, int y, COLORREF cTransparentColor)
{
	CPalette* pOldPal = pDC->SelectPalette(m_pPalette, TRUE);
    pDC->RealizePalette();

	DrawTransparentBitmap(pDC->GetSafeHdc(), 
						  m_hBitmap, 
						  x, 
						  y, 
						  cTransparentColor);

	pDC->SelectPalette(pOldPal, TRUE);

	return TRUE;
}

#define UPVALUE(x, n)	((x)/((n) ? (n) : 1) + ((x)%((n) ? (n) : 1) ? 1 : 0))

/*
*--------------------------------------------------------------------------------
*  DisplayFadeIn
*  淡入的显示DIB对象（惭显）
*  Parameter :  
*
*                [IN]pDC
*                    设备上下文指针
*                [IN] x
*                   点的x坐标
*                [IN] y
*                   点的y坐标
*				 [IN]nDeta	
*					显示时采用
*				 [IN]dwDelayTime
*					显示时的延迟时间
*  Return value: 
*                返回TRUE表示显示成功，返回FALSE表示失败
*====================================================
*
*/


BOOL CDib::DisplayFadeIn(CDC* pDC, int x, int y, int nDeta, DWORD dwDelayTime)
{
	// 备份 DIB
	HDIB hDib = CopyHandle(m_hDib);

	// 得到颜色数
	WORD wNumColors = GetColorNumber();

	BITMAPINFO *bmi = (BITMAPINFO *)GlobalLock(m_hDib);
	BITMAPINFO *bmi0 = (BITMAPINFO *)GlobalLock(hDib);
	if (! bmi)
		return FALSE;

	BOOL bDone = FALSE;
	int nDetaR, nDetaG, nDetaB;
	// 淡出
	if (wNumColors)		
	{
		// 创建黑色图象
		for (WORD i=0; i<wNumColors; ++i)
		{
			bmi->bmiColors[i].rgbRed = 0 ;
			bmi->bmiColors[i].rgbGreen = 0 ;
			bmi->bmiColors[i].rgbBlue = 0 ;
		}
		UpdateInternal();
		// 开始显示黑色
		Display(pDC, x, y, SRCCOPY);

		while (! bDone)
		{
			bDone = TRUE;
			for (WORD i=0; i<wNumColors; i++)
			{
				nDetaR = UPVALUE(bmi0->bmiColors[i].rgbRed-bmi->bmiColors[i].rgbRed, nDeta);
				nDetaG = UPVALUE(bmi0->bmiColors[i].rgbGreen-bmi->bmiColors[i].rgbGreen, nDeta);
				nDetaB = UPVALUE(bmi0->bmiColors[i].rgbBlue-bmi->bmiColors[i].rgbBlue, nDeta);
				if (! Fade(nDetaR, nDetaG, nDetaB,
						 bmi0->bmiColors[i].rgbRed,
						 bmi0->bmiColors[i].rgbGreen,
						 bmi0->bmiColors[i].rgbBlue,
						 &(bmi->bmiColors[i].rgbRed),
						 &(bmi->bmiColors[i].rgbGreen),
						 &(bmi->bmiColors[i].rgbBlue)))
					 bDone = FALSE;
			}
			UpdateInternal();
			Display(pDC, x, y, SRCCOPY);
			Delay(dwDelayTime);
			nDeta--;
		}
	}
	// 无调色板
	else	
	{
		// 应该增加显示速度
		dwDelayTime /= 10;
		// 位位置
		LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)bmi;
		LPBYTE lpBits = (LPBYTE)lpbi + lpbi->biSize;
		LPBYTE lpBits0 = (LPBYTE)bmi0 + lpbi->biSize;
		int nDelta = WIDTHBYTES1(lpbi->biBitCount*lpbi->biWidth) - lpbi->biWidth*lpbi->biBitCount/8;
		int nx, ny;

		//创建黑色位图
		for (ny=0; ny<lpbi->biHeight; ny++)
		{
			for (nx=0; nx<lpbi->biWidth; nx++)
			{
				*lpBits++ = 0;
				*lpBits++ = 0;
				*lpBits++ = 0;
			}
			lpBits += nDelta; 
		}
		lpBits = (LPBYTE)lpbi + lpbi->biSize;
		UpdateInternal();
		// 开始时显示黑色
		Display(pDC, x, y, SRCCOPY);

		BYTE r, g, b, r0, g0, b0;
		while (! bDone)
		{
			bDone = TRUE;
			for (ny=0; ny<lpbi->biHeight; ny++)
			{
				for (nx=0; nx<lpbi->biWidth; nx++)
				{
					b0 = (BYTE)*lpBits0++;
					g0 = (BYTE)*lpBits0++;
					r0 = (BYTE)*lpBits0++;
					b = (BYTE)*(lpBits);
					g = (BYTE)*(lpBits+1);
					r = (BYTE)*(lpBits+2);
					nDetaR = UPVALUE(r0-r, nDeta);
					nDetaG = UPVALUE(g0-g, nDeta);
					nDetaB = UPVALUE(b0-b, nDeta);
					if (! Fade(nDetaR, nDetaG, nDetaB, r0, g0, b0, &r, &g, &b))
						bDone = FALSE;
					*lpBits++ = b;
					*lpBits++ = g;
					*lpBits++ = r;
				}
				lpBits += nDelta; 
				lpBits0 += nDelta; 
			}
			lpBits = (LPBYTE)lpbi + lpbi->biSize;
			lpBits0 = (LPBYTE)bmi0 + lpbi->biSize;
			UpdateInternal();
			Display(pDC, x, y, SRCCOPY);
			Delay(dwDelayTime);
			nDeta--;
		}
	}

	//清除对象 
	GlobalUnlock(m_hDib);
	GlobalUnlock(hDib);

	//回存备份的 DIB
	m_hDib = CopyHandle(hDib);
	GlobalFree(hDib);

	return TRUE;
}

/*
*--------------------------------------------------------------------------------
*  DisplayFadeOut
*  淡出的显示DIB对象(惭隐)
*  Parameter :  
*
*                [IN]pDC
*                    设备上下文指针
*                [IN] x
*                   点的x坐标
*                [IN] y
*                   点的y坐标
*				 [IN]nDeta	
*					显示时采用
*				 [IN]dwDelayTime
*					显示时的延迟时间
*  Return value: 
*                返回TRUE表示显示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::DisplayFadeOut(CDC* pDC, int x, int y, int nDeta, DWORD dwDelayTime)
{
	//一开始时显示
	Display(pDC, x, y, SRCCOPY);
	
	// 备份DIB
	HDIB hDib = CopyHandle(m_hDib);

	// 得到DIB
	WORD wNumColors = GetColorNumber();

	BITMAPINFO *bmi = (BITMAPINFO *)GlobalLock(m_hDib);
	if (! bmi)
		return FALSE;

	BOOL bDone = FALSE;
	int nDetaR, nDetaG, nDetaB;
	// 淡出（有调色板）
	if (wNumColors)		
	{
		while (! bDone)
		{
			bDone = TRUE;
			for (WORD i=0; i<wNumColors; i++)
			{
				nDetaR = -1*UPVALUE(bmi->bmiColors[i].rgbRed, nDeta);
				nDetaG = -1*UPVALUE(bmi->bmiColors[i].rgbGreen, nDeta);
				nDetaB = -1*UPVALUE(bmi->bmiColors[i].rgbBlue, nDeta);
				if (! Fade(nDetaR, nDetaG, nDetaB,
						 0, 0, 0,
						 &(bmi->bmiColors[i].rgbRed),
						 &(bmi->bmiColors[i].rgbGreen),
						 &(bmi->bmiColors[i].rgbBlue)))
					 bDone = FALSE;
			}
			UpdateInternal();
			Display(pDC, x, y, SRCCOPY);
			Delay(dwDelayTime);
			nDeta--;
		}
	}
	//无调色板
	else		
	{
		// 增加显示速度
		dwDelayTime /= 10;
		//位位置 
		LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)bmi;
		LPBYTE lpBits = (LPBYTE)lpbi + lpbi->biSize;
		int nDelta = WIDTHBYTES1(lpbi->biBitCount*lpbi->biWidth) - lpbi->biWidth*lpbi->biBitCount/8;

		BYTE r, g, b;
		int nx, ny;
		while (! bDone)
		{
			bDone = TRUE;
			for (ny=0; ny<lpbi->biHeight; ny++)
			{
				for (nx=0; nx<lpbi->biWidth; nx++)
				{
					b = (BYTE)*(lpBits);
					g = (BYTE)*(lpBits+1);
					r = (BYTE)*(lpBits+2);
					nDetaR = -1*UPVALUE(r, nDeta);
					nDetaG = -1*UPVALUE(g, nDeta);
					nDetaB = -1*UPVALUE(b, nDeta);
					if (! Fade(nDetaR, nDetaG, nDetaB, 0, 0, 0, &r, &g, &b))
						bDone = FALSE;
					*lpBits++ = b;
					*lpBits++ = g;
					*lpBits++ = r;
				}
				lpBits += nDelta; 
			}
			lpBits = (LPBYTE)lpbi + lpbi->biSize;
			UpdateInternal();
			Display(pDC, x, y, SRCCOPY);
			Delay(dwDelayTime);
			nDeta--;
		}
	}

	//释放
	GlobalUnlock(m_hDib);

	// 恢复先前备份的DIB
	m_hDib = CopyHandle(hDib);
	GlobalFree(hDib);

	return TRUE;
}

/*
*--------------------------------------------------------------------------------
*  DisplayMosaicIn()
*  
*  Parameter :  
*                [IN] pDC
*                     要操作的设备上下文指针
*                [IN] xx
*                   点的x坐标
*                [IN] yy
*                   点的y坐标
*                [IN] nTileSize
*                   颜色表尺寸
*                [IN] dwDelayTime
*                    显示延迟时间
*                [IN] dwRop
*                    
*  Return value: 
*               返回TRUE表示显示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::DisplayMosaicIn(CDC *pDC, int xx, int yy, int nTileSize, DWORD dwDelayTime, DWORD dwRop)
{
    //计算颜色表尺寸
	DWORD dwTileNum=(GetWidth()/nTileSize + ((GetWidth() % nTileSize) ? 1:0))*
					(GetHeight()/nTileSize + ((GetHeight() % nTileSize) ? 1:0));
	//计算表数组
	POINT *point = new POINT[dwTileNum];
	if (point == NULL)
		return FALSE;

	//初始化表数组
	LONG x=0, y=0;
	for (DWORD i=0;i<dwTileNum;++i) 
	{
		point[i].x=x;
		point[i].y=y;

		x+=nTileSize;
		if (x >= GetWidth()) 
		{
			x=0;
            y+=nTileSize;
		}
	}

	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp = MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette, TRUE);
    pDC->RealizePalette();
	
	double fMax = RAND_MAX;
	BOOL  bDone = FALSE;
	while (! bDone)
	{
		DWORD n = (DWORD)((double)dwTileNum*(double)((double)rand()/fMax));
		x = point[n].x;
		y = point[n].y;

		pDC->BitBlt(xx+x,
					yy+y,
					nTileSize,
					nTileSize,
					&MemDC,
					x,
					y,
					dwRop);

		dwTileNum--;
		bDone = !dwTileNum;

		point[n].x = point[dwTileNum].x;
		point[n].y = point[dwTileNum].y;
		Delay(dwDelayTime);
	}

	MemDC.SelectObject(pOldBmp);
	pDC->SelectPalette(pOldPal, TRUE);

	delete[] point;

	return TRUE;
}
/*
*--------------------------------------------------------------------------------
*  DisplayMosaicOut()
*  
*  Parameter :  
*                [IN] pDC
*                     要操作的设备上下文指针
*                [IN] xx
*                   点的x坐标
*                [IN] yy
*                   点的y坐标
*                [IN] nTileSize
*                   尺寸
*                [IN] dwDelayTime
*                    显示延迟时间
*                    
*  Return value: 
*               返回TRUE表示显示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::DisplayMosaicOut(CDC *pDC, int xx, int yy, int nTileSize, DWORD dwDelayTime)
{
	// 开始显示dib
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp = MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette, TRUE);
    pDC->RealizePalette();

	pDC->BitBlt(xx,	yy,
				GetWidth(),	GetHeight(),
				&MemDC,
				0, 0,
				SRCCOPY);

    // 计算表数量
	DWORD dwTileNum=(GetWidth()/nTileSize + ((GetWidth() % nTileSize) ? 1:0))*
					(GetHeight()/nTileSize + ((GetHeight() % nTileSize) ? 1:0));
	// 分配表数组
	POINT *point = new POINT[dwTileNum];
	if (point == NULL)
		return FALSE;

	//初始化表数组
	LONG x=0, y=0;
	for (DWORD i=0;i<dwTileNum;++i) 
	{
		point[i].x=x;
		point[i].y=y;

		x+=nTileSize;
		if (x >= GetWidth()) 
		{
			x=0;
            y+=nTileSize;
		}
	}

	CBrush brush(GetSysColor(COLOR_WINDOW));
	CBrush* oldbrush = pDC->SelectObject(&brush);
	CPen pen(PS_SOLID,1,GetSysColor(COLOR_WINDOW));
    CPen* oldpen = pDC->SelectObject(&pen);
	
	double fMax = RAND_MAX;
	BOOL bDone = FALSE;
	while (! bDone)
	{
		DWORD n = (DWORD)((double)dwTileNum*(double)((double)rand()/fMax));
		x = point[n].x;
		y = point[n].y;

		pDC->Rectangle(	xx+x,
						yy+y,
						xx+x+nTileSize+1,
						yy+y+nTileSize+1);

		dwTileNum--;
		bDone = !dwTileNum;

		point[n].x = point[dwTileNum].x;
		point[n].y = point[dwTileNum].y;
		Delay(dwDelayTime);
	}

	MemDC.SelectObject(pOldBmp);
	pDC->SelectPalette(pOldPal, TRUE);
	pDC->SelectObject(oldbrush);
    pDC->SelectObject(oldpen);

	delete[] point;

	return TRUE;
}

/*
*--------------------------------------------------------------------------------
* Display
* 显示指定的DIB图形
* Parameter :  
*                [IN] pDC
*                     要操作的设备上下文指针
*                [IN] x
*                   点的x坐标
*                [IN] y
*                   点的y坐标
*                [IN] nDeta
*
*                [IN] dwDelayTime
*                    显示延迟时间*                 
*                [IN] dwRop
*                   数据位置

*                    
*  Return value: 
*               返回TRUE表示显示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::Display(int nEffect, CDC* pDC, int x, int y, int nDeta, DWORD dwDelayTime, DWORD dwRop)
{
	if (nEffect == EFFECT_FADE)
		return DisplayFadeIn(pDC, x, y, nDeta, dwDelayTime);
	else if (nEffect == EFFECT_MOSAIC)
		return DisplayMosaicIn(pDC, x, y, nDeta, dwDelayTime, dwRop);

	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp = MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette, TRUE);
    pDC->RealizePalette();

	LONG i = 0;
	BOOL bDone = FALSE;
	while (! bDone)
	{
		switch (nEffect)
		{
		case EFFECT_SCANDOWN:
			if (i > GetHeight())
			{
				i = GetHeight();
				bDone = TRUE;
			}
            pDC->BitBlt(x, 
						y+i,
                        GetWidth(),
						nDeta,
						&MemDC,
                        0,
						i,
						dwRop);
			i += nDeta;
			break;
		case EFFECT_SCANUP:
			if (i > GetHeight())
			{
				i = GetHeight();
				bDone = TRUE;
			}
            pDC->BitBlt(x, 
						y+GetHeight()-i,
                        GetWidth(),
						nDeta,
						&MemDC,
                        0,
						GetHeight()-i,
						dwRop);
			i += nDeta;
			break;
		case EFFECT_SCANRIGHT:
			if (i > GetWidth())
			{
				i = GetWidth();
				bDone = TRUE;
			}
            pDC->BitBlt(x+i, 
						y,
						nDeta,
                        GetHeight(),
						&MemDC,
						i,
                        0,
						dwRop);
			i += nDeta;
			break;
		case EFFECT_SCANLEFT:
			if (i > GetWidth())
			{
				i = GetWidth();
				bDone = TRUE;
			}
            pDC->BitBlt(x+GetWidth()-i, 
						y,
						nDeta,
                        GetHeight(),
						&MemDC,
						GetWidth()-i,
                        0,
						dwRop);
			i += nDeta;
			break;
		case EFFECT_VSPLITSCAN:
			if (i > GetHeight()/2)
			{
				i = GetHeight()/2;
				bDone = TRUE;
			}
			pDC->BitBlt(x,
						y+i,
						GetWidth(),
						nDeta,
						&MemDC,
						0,
						i,
						dwRop);
			pDC->BitBlt(x,
						y+GetHeight()-i,
						GetWidth(),
						i,
						&MemDC,
						0,
						GetHeight()-i,
						dwRop);
			i += nDeta;
			break;
		case EFFECT_HSPLITSCAN:
			if (i > GetWidth()/2)
			{
				i = GetWidth()/2;
				bDone = TRUE;
			}
			pDC->BitBlt(x+i,
						y,
						nDeta,
						GetHeight(),
						&MemDC,
						i,
						0,
						dwRop);
			pDC->BitBlt(x+GetWidth()-i,
						y,
						nDeta,
						GetHeight(),
						&MemDC,
						GetWidth()-i,
						0,
						dwRop);
			i += nDeta;
			break;
		case EFFECT_MOVEDOWN:
			if (i > GetHeight())
			{
				i = GetHeight();
				bDone = TRUE;
			}
			pDC->BitBlt(x,
						y,
						GetWidth(),
						i+nDeta,
						&MemDC,
						0,
						GetHeight()-i,
						dwRop);
			i += nDeta;
			break;
		case EFFECT_MOVEUP:
			if (i > GetHeight())
			{
				i = GetHeight();
				bDone = TRUE;
			}
			pDC->BitBlt(x,
						y+GetHeight()-i,
						GetWidth(),
						i,
						&MemDC,
						0,
						0,
						dwRop);
			i += nDeta;
			break;
		case EFFECT_MOVERIGHT:
			if (i > GetWidth())
			{
				i = GetWidth();
				bDone = TRUE;
			}
			pDC->BitBlt(x,
						y,
						i+nDeta,
						GetHeight(),
						&MemDC,
						GetWidth()-i,
						0,
						dwRop);
			i += nDeta;
			break;
		case EFFECT_MOVELEFT:
			if (i > GetWidth())
			{
				i = GetWidth();
				bDone = TRUE;
			}
			pDC->BitBlt(x+GetWidth()-i,
						y,
						i,
						GetHeight(),
						&MemDC,
						0,
						0,
						dwRop);
			i += nDeta;
			break;
		case EFFECT_VSPLITMOVE:
			if (i > GetHeight()/2)
			{
				i = GetHeight()/2;
				bDone = TRUE;
			}
			pDC->BitBlt(x,
						y,
						GetWidth(),
						i+nDeta,
						&MemDC,
						0,
						GetHeight()/2-i,
						dwRop);
			pDC->BitBlt(x,
						y+GetHeight()-i,
						GetWidth(),
						i,
						&MemDC,
						0,
						GetHeight()/2,
						dwRop);
			i += nDeta;
			break;
		case EFFECT_HSPLITMOVE:
			if (i > GetWidth()/2)
			{
				i = GetWidth()/2;
				bDone = TRUE;
			}
			pDC->BitBlt(x,
						y,
						i+nDeta,
						GetHeight(),
						&MemDC,
						GetWidth()/2-i,
						0,
						dwRop);
			pDC->BitBlt(x+GetWidth()-i,
						y,
						i,
						GetHeight(),
						&MemDC,
						GetWidth()/2,
						0,
						dwRop);
			i += nDeta;
			break;
		case EFFECT_VCROSSMOVE:
			if (i > GetHeight())
			{
				i = GetHeight();
				bDone = TRUE;
			}
			pDC->BitBlt(x,
						y,
						GetWidth()/2,
						i+nDeta,
						&MemDC,
						0,
						GetHeight()-i,
						dwRop);
			pDC->BitBlt(x+GetWidth()/2,
						y+GetHeight()-i,
						GetWidth(),
						i,
						&MemDC,
						GetWidth()/2,
						0,
						dwRop);
			i += nDeta;
			break;
		case EFFECT_HCROSSMOVE:
			if (i > GetWidth())
			{
				i = GetWidth();
				bDone = TRUE;
			}
			pDC->BitBlt(x,
						y,
						i+nDeta,
						GetHeight()/2,
						&MemDC,
						GetWidth()-i,
						0,
						dwRop);
			pDC->BitBlt(x+GetWidth()-i,
						y+GetHeight()/2,
						i,
						GetHeight()/2,
						&MemDC,
						0,
						GetHeight()/2,
						dwRop);
			i += nDeta;
			break;
		case EFFECT_VRASTER:
		{
			if (i > GetHeight())
			{
				i = GetHeight();
				bDone = TRUE;
			}
			LONG j = 0;
			BOOL bQuitLoop = FALSE;
			while (! bQuitLoop)
			{
				if (j > GetWidth())
				{
					j = GetWidth();
					bQuitLoop = TRUE;
				}
				pDC->BitBlt(x+j,
							y,
							nDeta,
							i+1,
							&MemDC,
							j,
							GetHeight()-i,
							dwRop);
				j += nDeta;
				if (j > GetWidth())
				{
					j = GetWidth();
					bQuitLoop = TRUE;
				}
				pDC->BitBlt(x+j,
							y+GetHeight()-i,
							nDeta,
							i,
							&MemDC,
							j,
							0,
							dwRop);
				j += nDeta;
			}
			i++;
			break;
		}
		case EFFECT_HRASTER:
		{
			if (i > GetWidth())
			{
				i = GetWidth();
				bDone = TRUE;
			}
			LONG j = 0;
			BOOL bQuitLoop = FALSE;
			while (! bQuitLoop)
			{
				if (j > GetHeight())
				{
					j = GetHeight();
					bQuitLoop = TRUE;
				}
				pDC->BitBlt(x,
							y+j,
							i+1,
							nDeta,
							&MemDC,
							GetWidth()-i,
							j,
							dwRop);
				j += nDeta;
				if (j > GetHeight())
				{
					j = GetHeight();
					bQuitLoop = TRUE;
				}
				pDC->BitBlt(x+GetWidth()-i,
							y+j,
							i,
							nDeta,
							&MemDC,
							0,
							j,
							dwRop);
				j += nDeta;
			}
			i++;
			break;
		}
		case EFFECT_HBLIND:
		{
			if (i > nDeta)
			{
				i = nDeta;
				bDone = TRUE;
			}
			for (LONG j=i; j<GetHeight(); j+=nDeta)
				pDC->BitBlt(x,
							y+j,
							GetWidth(),
							1,
							&MemDC,
							0,
							j,
							dwRop);
			i++;
			break;
		}
		case EFFECT_VBLIND:
			if (i > nDeta)
			{
				i = nDeta;
				bDone = TRUE;
			}
			for (LONG j=i; j<GetWidth(); j+=nDeta)
				pDC->BitBlt(x+j,
							y,
							1,
							GetHeight(),
							&MemDC,
							j,
							0,
							dwRop);
			i++;
			break;
		}  // end of switch
		Delay(dwDelayTime);
	}	// end of while

	MemDC.SelectObject(pOldBmp);
	pDC->SelectPalette(pOldPal, TRUE);

	return TRUE;
}

/*
*--------------------------------------------------------------------------------
*  Clear()
*  清除创建的DIB对象 
*  Parameter :  
*                [IN] nEffect
*                   点的x坐标
*                [IN] pDC
*                   CDC指针
*                [IN] x
*                   点的x坐标
*                [IN] y
*                   点的y坐标
*                [IN] nDeta
*                   数据位
*                [IN] dwDelayTime
*                   延迟的时间
*  Return value: 
*                返回TRUE表示显示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::Clear(int nEffect, CDC* pDC, int x, int y, int nDeta, DWORD dwDelayTime)
{
	if (nEffect == EFFECT_FADE)
		return DisplayFadeOut(pDC, x, y, nDeta, dwDelayTime);
	else if (nEffect == EFFECT_MOSAIC)
		return DisplayMosaicOut(pDC, x, y, nDeta, dwDelayTime);

	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp = MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette, TRUE);
    pDC->RealizePalette();

	pDC->BitBlt(x,
				y,
				GetWidth(),
				GetHeight(),
				&MemDC,
				0,
				0,
				SRCCOPY);

	CBrush brush(GetSysColor(COLOR_WINDOW));
	CBrush* oldbrush = pDC->SelectObject(&brush);
	CPen pen(PS_SOLID,1,GetSysColor(COLOR_WINDOW));
    CPen* oldpen = pDC->SelectObject(&pen);

	LONG i = 0;
	BOOL bDone = FALSE;
	while (! bDone)
	{
		switch (nEffect)
		{
		case EFFECT_SCANDOWN:
			if (i > GetHeight())
			{
				i = GetHeight();
				bDone = TRUE;
			}
			pDC->Rectangle(x, 
						   y+i,
						   x+GetWidth(), 
						   y+nDeta);
			i += nDeta;
			break;
		case EFFECT_SCANUP:
			if (i > GetHeight())
			{
				i = GetHeight();
				bDone = TRUE;
			}
			pDC->Rectangle(x, 
						   y+GetHeight()-i,
						   x+GetWidth(), 
						   y+GetHeight()-i+nDeta);
			i += nDeta;
			break;
		case EFFECT_SCANRIGHT:
			if (i > GetWidth())
			{
				i = GetWidth();
				bDone = TRUE;
			}
			pDC->Rectangle(x+i,
						   y,
						   x+nDeta, 
						   y+GetHeight());
			i += nDeta;
			break;
		case EFFECT_SCANLEFT:
			if (i > GetWidth())
			{
				i = GetWidth();
				bDone = TRUE;
			}
			pDC->Rectangle(x+GetWidth()-i, 
						   y,
						   x+GetWidth()-i+nDeta, 
						   y+GetHeight());
			i += nDeta;
			break;
		case EFFECT_VSPLITSCAN:
			if (i > GetHeight()/2)
			{
				i = GetHeight()/2;
				bDone = TRUE;
			}
			pDC->Rectangle(x, 
						   y+i,
						   x+GetWidth(), 
						   y+i+nDeta);
			pDC->Rectangle(x, 
						   y+GetHeight()-i,
						   x+GetWidth(), 
						   y+GetHeight());
			i += nDeta;
			break;
		case EFFECT_HSPLITSCAN:
			if (i > GetWidth()/2)
			{
				i = GetWidth()/2;
				bDone = TRUE;
			}
			pDC->Rectangle(x+i, 
						   y,
						   x+i+nDeta, 
						   y+GetHeight());
			pDC->Rectangle(x+GetWidth()-i,
						   y,
						   x+GetWidth(), 
						   y+GetHeight());
			i += nDeta;
			break;
		case EFFECT_MOVEDOWN:
		{
			i += nDeta;
			if (i > GetHeight())
			{
				i = GetHeight();
				bDone = TRUE;
			}
			pDC->BitBlt(x,  		
						y+i,		
      					GetWidth(),		
						GetHeight()-i,	
						&MemDC,
						0,				
						0,		
						SRCCOPY);

			pDC->Rectangle(x, y+i-nDeta,
						   x+GetWidth(), y+i);
			break;
		}
		case EFFECT_MOVEUP:
		{
			i += nDeta;
			if (i > GetHeight())
			{
				i = GetHeight();
				bDone = TRUE;
			}
			pDC->BitBlt(x,  			
						y,	
	      				GetWidth(),		
						GetHeight()-i,	
						&MemDC,
						0,				
						i,		
						SRCCOPY);

			pDC->Rectangle( x,
							y+GetHeight()-i,
							x+GetWidth(),
							y+GetHeight());
			break;
		}
		case EFFECT_MOVERIGHT:
		{
			i += nDeta;
			if (i > GetWidth())
			{
				i = GetWidth();
				bDone = TRUE;
			}
			pDC->BitBlt(x+i,  			
						y,	
      					GetWidth()-i,	
						GetHeight(),	
						&MemDC,
						0,			
						0,		
						SRCCOPY);

			pDC->Rectangle(x+i-nDeta, y,
						   x+i, y+GetHeight());
			break;
		}
		case EFFECT_MOVELEFT:
		{
			i += nDeta;
			if (i > GetWidth())
			{
				i = GetWidth();
				bDone = TRUE;
			}
			pDC->BitBlt(x,  			
						y,		
	      				GetWidth()-i,		
						GetHeight(),	
						&MemDC,
						i,		
						0,			
						SRCCOPY);

			pDC->Rectangle( x+GetWidth()-i,
							y,
							x+GetWidth(),
							y+GetHeight());
			break;
		}
		case EFFECT_VCROSSMOVE:
		{
			i += nDeta;
			if (i > GetHeight())
			{
				i = GetHeight();
				bDone = TRUE;
			}
			pDC->BitBlt(x,  		
						y+i,	
      					GetWidth()/2,	
						GetHeight()-i,
						&MemDC,
						0,			
						0,		
						SRCCOPY);

			pDC->Rectangle(x, y+i-nDeta,
						   x+GetWidth()/2, y+i);

			pDC->BitBlt(x+GetWidth()/2,  			
						y,	
	      				GetWidth(),		
						GetHeight()-i,	
						&MemDC,
						GetWidth()/2,			
						i,		
						SRCCOPY);

			pDC->Rectangle( x+GetWidth()/2,
							y+GetHeight()-i,
							x+GetWidth(),
							y+GetHeight());
			break;
		}
		case EFFECT_HCROSSMOVE:
		{
			i += nDeta;
			if (i > GetWidth())
			{
				i = GetWidth();
				bDone = TRUE;
			}
			pDC->BitBlt(x+i,  			
						y,		
      					GetWidth()-i,	
						GetHeight()/2,
						&MemDC,
						0,			
						0,			
						SRCCOPY);

			pDC->Rectangle(x+i-nDeta, y,
						   x+i, y+GetHeight()/2);
	
			pDC->BitBlt(x,  		
						y+GetHeight()/2,
	      				GetWidth()-i,		
						GetHeight(),
						&MemDC,
						i,			
						GetHeight()/2,
						SRCCOPY);

			pDC->Rectangle( x+GetWidth()-i,
							y+GetHeight()/2,
							x+GetWidth(),
							y+GetHeight());
			break;
		}
		case EFFECT_VSPLITMOVE:
		{
			i += nDeta;
			if (i > GetHeight()/2)
			{
				i = GetHeight()/2;
				bDone = TRUE;
			}
			pDC->BitBlt(x,  		
						y,	
			  			GetWidth(),		
						GetHeight()/2-i,	
						&MemDC,
						0,				
						i,			
						SRCCOPY);

			pDC->BitBlt(x,  		
						y+GetHeight()/2+i,	
      					GetWidth(),		
						GetHeight()/2-i,	
						&MemDC,
						0,			
						GetHeight()/2,		
						SRCCOPY);

			pDC->Rectangle( x,
							y+GetHeight()/2-i,
							x+GetWidth(),
							y+GetHeight()/2+i+1);
			break;
		}
		case EFFECT_HSPLITMOVE:
		{
			i += nDeta;
			if (i > GetWidth()/2)
			{
				i = GetWidth()/2;
				bDone = TRUE;
			}
			pDC->BitBlt(x,  		
						y,	
	      				GetWidth()/2-i,		
						GetHeight(),	
						&MemDC,
						i,			
						0,				
						SRCCOPY);

			pDC->BitBlt(x+GetWidth()/2+i,  			
						y,	
      					GetWidth()/2-i,	
						GetHeight(),
						&MemDC,
						GetWidth()/2,			
						0,		
						SRCCOPY);

			pDC->Rectangle( x+GetWidth()/2-i,
							y,
							x+GetWidth()/2+i+1,
							y+GetHeight());
			break;
		}
		case EFFECT_VRASTER:
		{
			if (i > GetHeight())
			{
				i = GetHeight();
				bDone = TRUE;
			}
			LONG j = 0;
			BOOL bQuitLoop = FALSE;
			while (! bQuitLoop)
			{
				if (j > GetWidth())
				{
					j = GetWidth();
					bQuitLoop = TRUE;
				}
				pDC->BitBlt(x+j,  		
							y+i,	
      						nDeta,	
							GetHeight()-i,
							&MemDC,
							j,			
							0,		
							SRCCOPY);

				pDC->Rectangle(x+j, y+i-1,
							   x+j+nDeta, y+i);
				j += nDeta;
				if (j > GetWidth())
				{
					j = GetWidth();
					bQuitLoop = TRUE;
				}
	
				pDC->BitBlt(x+j,  			
							y,	
				  			nDeta,		
							GetHeight()-i,	
							&MemDC,
							j,		
							i,				
							SRCCOPY);

				pDC->Rectangle( x+j,
								y+GetHeight()-i,
								x+j+nDeta,
								y+GetHeight());
				j += nDeta;
			}
			i++;
			break;
		}
		case EFFECT_HRASTER:
		{
			if (i > GetWidth())
			{
				i = GetWidth();
				bDone = TRUE;
			}
			LONG j = 0;
			BOOL bQuitLoop = FALSE;
			while (! bQuitLoop)
			{
				if (j > GetHeight())
				{
					j = GetHeight();
					bQuitLoop = TRUE;
				}
				pDC->BitBlt(x+i,  			
							y+j,	
      						GetWidth()-i,	
							nDeta,	
							&MemDC,
							0,				
							j,		
							SRCCOPY);
				pDC->Rectangle(x+i-1, y+j,
							   x+i, y+j+nDeta);
				j += nDeta;
				if (j > GetHeight())
				{
					j = GetHeight();
					bQuitLoop = TRUE;
				}
				pDC->BitBlt(x,  			
							y+j,	
				  			GetWidth()-i,		
							nDeta,	
							&MemDC,
							i,		
							j,				
							SRCCOPY);
				pDC->Rectangle( x+GetWidth()-i,
								y+j,
								x+GetWidth(),
								y+j+nDeta);
				j += nDeta;
			}
			i++;
			break;
		}
		case EFFECT_HBLIND:
		{
			if (i > nDeta)
			{
				i = nDeta;
				bDone = TRUE;
			}
			for (LONG j=i; j<GetHeight(); j+=nDeta)
				pDC->Rectangle( x,
								y+j,
								x+GetWidth(),
								y+j+1);
			i++;
			break;
		}
		case EFFECT_VBLIND:
			if (i > nDeta)
			{
				i = nDeta;
				bDone = TRUE;
			}
			for (LONG j=i; j<GetWidth(); j+=nDeta)
				pDC->Rectangle( x+j,
								y,
								x+j+1,
								y+GetHeight());
			i++;
			break;
		}	// end of switch
		Delay(dwDelayTime);
	}	// end of while

	MemDC.SelectObject(pOldBmp);
	pDC->SelectPalette(pOldPal, TRUE);

	return TRUE;
}

/*
*--------------------------------------------------------------------------------
*  CopyRect()
*  复制指定DIB对象的指定区域
*  Parameter :  
*                [IN] rc
*                   矩形区域
*  Return value: 
*                返回TRUE表示成功，返回FALSE表示失败
*====================================================
*
*/

HDIB CDib::CopyRect(CRect rc)
{
	return CropDIB(m_hDib, rc);
}

/*
*--------------------------------------------------------------------------------
*  CutRect()
*  剪切指定DIB对象的指定区域
*  Parameter :  
*                [IN] rc
*                   矩形区域
*  Return value: 
*                返回TRUE表示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::CutRect(CRect rc)
{
	HDIB hNewDib = CutDIB(m_hDib, rc);
	if (! hNewDib)
		return FALSE;

	//释放
	Destroy();
	m_hDib = hNewDib;

	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  MergeDib（）
*  融合指定DIB对象到当前DIB形成一新的DIB
*  Parameter :  
*                [IN] hDib
*                   要融合的DIB对象
*                [IN] ptTopLeft
*                   融合的装置的左上点坐标
*  Return value: 
*                返回TRUE表示成功，返回FALSE表示失败
*====================================================
*
*/
BOOL CDib::MergeDib(HDIB hDib, CPoint ptTopLeft)
{
	POINT pt;
	pt.x = ptTopLeft.x;
	pt.y = ptTopLeft.y;
	HDIB hNewDib = MergeDIB(m_hDib, hDib, pt);
	if (! hNewDib)
		return FALSE;

	Destroy();
	m_hDib = hNewDib;
	
	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Crop()
*  修剪CDib对象 
*  Parameter :  
*                [IN] rc
*                   要修剪的矩形区
*  Return value: 
*                 返回TRUE表示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::	Crop(CRect rc)
{
	HDIB hNewDib = CropDIB(m_hDib, rc);
	if (! hNewDib)
		return FALSE;

	Destroy();
	m_hDib = hNewDib;

	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Rotate()
*  以指定角度旋转DIB对象
*  Parameter :  
*                [IN] fDegrees
*                   角度
*                [IN] clrBack
*                   背景颜色
*  Return value: 
*                 返回TRUE表示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::Rotate(double fDegrees, COLORREF clrBack)
{
	HDIB hNewDib = RotateDIB(m_hDib, fDegrees, clrBack);
	if (! hNewDib)
		return FALSE;

	Destroy();
	m_hDib = hNewDib;

	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Rotate90()
*  旋转90度DIB对象
*  Parameter :  
*                无
*  Return value: 
*                返回TRUE表示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::Rotate90()
{
	HDIB hNewDib = RotateDIB(m_hDib);
	if (! hNewDib)
		return FALSE;

	Destroy();
	m_hDib = hNewDib;

	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Rotate180()
*  旋转180度DIB对象
*  Parameter :  
*                无
*  Return value: 
*                返回TRUE表示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::Rotate180()
{
	HDIB h = RotateDIB(m_hDib);
	if (! h)
		return FALSE;
	HDIB hNewDib = RotateDIB(h);
	DestroyDIB(h);
	if (! hNewDib)
		return FALSE;

	Destroy();
	m_hDib = hNewDib;

	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Rotate270()
*  创建CDib对象 
*  Parameter :  
*                无
*  Return value: 
*                返回TRUE表示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::Rotate270()
{
	HDIB h1 = RotateDIB(m_hDib);
	if (! h1)
		return FALSE;
	HDIB h2 = RotateDIB(h1);
	DestroyDIB(h1);
	if (! h2)
		return FALSE;
	HDIB hNewDib = RotateDIB(h2);
	DestroyDIB(h2);
	if (! hNewDib)
		return FALSE;

	Destroy();
	m_hDib = hNewDib;

	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  FlipHorz()
*  水平镜像
*  Parameter :  
*                无
*  Return value: 
*                返回TRUE表示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::FlipHorz()
{
	HDIB hNewDib = FlipHorzDIB(m_hDib);
	if (! hNewDib)
		return FALSE;
	Destroy();
	m_hDib = hNewDib;

	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  FlipVert()
*  垂直镜像 
*  Parameter :  
*                无
*  Return value: 
*                返回TRUE表示成功，返回FALSE表示失败
*====================================================
*
*/

BOOL CDib::FlipVert()
{
	HDIB hNewDib = FlipVertDIB(m_hDib);
	if (! hNewDib)
		return FALSE;

	Destroy();
	m_hDib = hNewDib;

	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  ChangeImageSize()
*  创建CDib对象 
*  Parameter :  
*                [IN] nWidth
*                   宽度
*                [IN] nHeight
*                   高度
*  Return value: 
*                TRUE OR FALSE
*====================================================
*
*/

BOOL CDib::	ChangeImageSize(int nWidth, int nHeight)
{
	HDIB hNewDib = ChangeDIBSize(m_hDib, nWidth, nHeight);
	if (! hNewDib)
		return FALSE;

	Destroy();
	m_hDib = hNewDib;

	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Zoom()
*  创建CDib对象 
*  Parameter :  
*                [IN] fRatioX
*                   X轴比率
*                [IN] fRatioY
*                   Y轴比率
*  Return value: 
*                TRUE OR FALSE
*==================================================== 
*
*/

BOOL CDib::Zoom(double fRatioX, double fRatioY)
{
	int nWidth = (int)(fRatioX * (double)GetWidth());
	int nHeight = (int)(fRatioY * (double)GetHeight());

	HDIB hNewDib = ChangeDIBSize(m_hDib, nWidth, nHeight);
	if (! hNewDib)
		return FALSE;

	Destroy();
	m_hDib = hNewDib;

	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  ChangeCanvasSize()
*  创建CDib对象 
*  Parameter :  
*                [IN] nWidth
*                   宽度
*                [IN] nHeight
*                   高度
*                [IN] nPosition
*                   位置
*  Return value: 
*                TRUE OR FALSE
*====================================================
*
*/

BOOL CDib::	ChangeCanvasSize(int nWidth, int nHeight, int nPosition)
{
	HDIB hNewDib = ChangeDIBCanvasSize(m_hDib, nWidth, nHeight, nPosition);
	if (! hNewDib)
		return FALSE;

	Destroy();
	m_hDib = hNewDib;

	return UpdateInternal();
}

/*
*--------------------------------------------------------------------------------
*  Clone()
*  创建CDib对象 
*  Parameter :  
*                无
*  Return value: 
*                指向CDib对象的指针
*====================================================
*
*/

CDib * CDib::Clone()
{
	if (m_hDib == NULL)
		return NULL;

	HDIB hDIB = CopyHandle(m_hDib);
	if (hDIB == NULL)
		return NULL;

  	CDib *pDib = new CDib;
	pDib->m_hDib = hDIB;
	pDib->BuildPalette();
	pDib->BuildBitmap();

	return pDib;
}


/*
*--------------------------------------------------------------------------------
*  函数名	: AdjustDibBrightness
*  功能		: 调整DIB位图的亮度
*  参数		: [in] int v	-	调整后的亮度值
*  返回值	: 成功为 TRUE, 否则为 FALSE
*  算法		: 
*===============================================================
*/
BOOL CDib::AdjustDibBrightness(int v)
{
	ASSERT(m_hDib);

	BITMAPINFO * bmi = (BITMAPINFO *) GlobalLock(m_hDib);
	if (!bmi)
		return FALSE;

	WORD wNumColors = DIBNumColors((LPBYTE)bmi);

	if (wNumColors)
	{
		for (WORD i = 0; i < wNumColors; i++)
		{
			ChangeBrightness(v,
							&(bmi->bmiColors[i].rgbRed),
							&(bmi->bmiColors[i].rgbGreen),
							&(bmi->bmiColors[i].rgbBlue));
		}
	}
	else
	{
		LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER) bmi;
		LPBYTE lpBits = (LPBYTE)lpbi + lpbi->biSize;
		int nDelta = WIDTHBYTES(lpbi->biBitCount * lpbi->biWidth)
					- lpbi->biWidth * lpbi->biBitCount / 8;
		BYTE r, g, b;
		for (int ny = 0; ny < lpbi->biHeight; ny++)
		{
			for (int nx = 0; nx < lpbi->biWidth; nx++)
			{
				b = (BYTE) *(lpBits);
				g = (BYTE) *(lpBits + 1);
				r = (BYTE) *(lpBits + 2);
				ChangeBrightness(v, &r, &g, &b);
				*lpBits++ = b;
				*lpBits++ = g;
				*lpBits++ = r;
			}
			lpBits += nDelta;
		}
	}

	GlobalUnlock(m_hDib);

	return TRUE;
}


/*
*--------------------------------------------------------------------------------
*  函数名	: ChangeBrightness
*  功能		: 调整像素点的亮度
*  参数		: [in] int nDelta	-	r,g,b 分量的变化值
*				[in, out] BYTE* r	-	像素点颜色 r 分量值
*				[in, out] BYTE* g	-	像素点颜色 g 分量值
*				[in, out] BYTE* b	-	像素点颜色 b 分量值
*  算法		: 
*===============================================================
*/
void CDib::ChangeBrightness(int nDelta, BYTE *r, BYTE *g, BYTE *b)
{
	int R = * r + nDelta;
	int G = *g + nDelta;
	int B = *b + nDelta;

	*r = (BYTE) BOUND(R, 0, 255);
	*g = (BYTE) BOUND(G, 0, 255);
	*b = (BYTE) BOUND(B, 0, 255);
}

//变化DIB图形的颜色位数
BOOL CDib::ChangeCanvasBit(INT colBit)
{
	CDib newDib;
	newDib.Create(m_hBitmap,colBit);
	if(newDib.GetHandle()==NULL)
	{
		return FALSE;
	}

//	m_hDib = newDib.m_hDib;
//	return UpdateInternal();

    if(!Create(newDib.m_hBitmap, colBit))
		TRACE("更改颜色位数失败\n");
    
	return UpdateInternal();

}

//得到位图句柄 
HBITMAP CDib::GetHBitmap()
{
   return m_hBitmap;
}
