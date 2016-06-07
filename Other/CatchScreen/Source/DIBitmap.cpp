/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：DIBitmap.cpp
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：DIB图形处理的类的模块文件
*======================================================
* 版本变更记录:
*      v1.0  2002-10-30   20:30  罗伟 
*
* 
*/

/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Include\DIBitmap.h"
#include "..\Include\dibpal.h"


#define PADWIDTH(x)	(((x)*8 + 31)  & (~31))/8

/*
*--------------------------------------------------------------------------------
*  成员函数名   :CDIBitmap()
*  功能描述     :构造函数
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
CDIBitmap :: CDIBitmap()
	: m_pInfo(0)
	, m_pPixels(0)
	, m_pPal(0)
	, m_bIsPadded(FALSE)
{
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :~CDIBitmap()
*  功能描述     :析构函数
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
CDIBitmap :: ~CDIBitmap() {
    delete [] (BYTE*)m_pInfo;
    delete [] m_pPixels;
	delete m_pPal;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :DestroyBitmap()
*  功能描述     :释放位图句柄
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void CDIBitmap :: DestroyBitmap() {
    delete [] (BYTE*)m_pInfo;
    delete [] m_pPixels;
	delete m_pPal;
	m_pInfo = 0;
	m_pPixels = 0;
	m_pPal = 0;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :CreateFromBitmap
*  功能描述     :从位图对象指针中创建DIB对象
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
BOOL CDIBitmap :: CreateFromBitmap( CDC * pDC, CBitmap * pSrcBitmap ) {
	ASSERT_VALID(pSrcBitmap);
	ASSERT_VALID(pDC);

	try {
		BITMAP bmHdr;

		// 得到位数据信息
		pSrcBitmap->GetObject(sizeof(BITMAP), &bmHdr);

		// 为位图数据重新分配内存
		if( m_pPixels ) {
			delete [] m_pPixels;
			m_pPixels = 0;
		}

		DWORD dwWidth;
		if (bmHdr.bmBitsPixel > 8)
			dwWidth = PADWIDTH(bmHdr.bmWidth * 3);
		else
			dwWidth = PADWIDTH(bmHdr.bmWidth);

		m_pPixels = new BYTE[dwWidth*bmHdr.bmHeight];
		if( !m_pPixels )
			throw TEXT("不能分配存储区\n");

		// 设置适当的颜色数为基于BITMAP结构信息
		WORD wColors;
		switch( bmHdr.bmBitsPixel ) {
			case 1 : 
				wColors = 2;
				break;
			case 4 :
				wColors = 16;
				break;
			case 8 :
				wColors = 256;
				break;
			default :
				wColors = 0;
				break;
		}

		// 再分配并组装BITMAPINFO结构
		if( m_pInfo ) {
			delete [] (BYTE*)m_pInfo;
			m_pInfo = 0;
		}

		m_pInfo = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFOHEADER) + wColors*sizeof(RGBQUAD)];
		if( !m_pInfo )
			throw TEXT("could not allocate BITMAPINFO struct\n");

		// 组装BITMAPINFO头信息
		m_pInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		m_pInfo->bmiHeader.biWidth = bmHdr.bmWidth;
		m_pInfo->bmiHeader.biHeight = bmHdr.bmHeight;
		m_pInfo->bmiHeader.biPlanes = bmHdr.bmPlanes;
		
		
		if( bmHdr.bmBitsPixel > 8 )
			m_pInfo->bmiHeader.biBitCount = 24;
		else
			m_pInfo->bmiHeader.biBitCount = bmHdr.bmBitsPixel;

		m_pInfo->bmiHeader.biCompression = BI_RGB;
		m_pInfo->bmiHeader.biSizeImage = ((((bmHdr.bmWidth * bmHdr.bmBitsPixel) + 31) & ~31) >> 3) * bmHdr.bmHeight;
		m_pInfo->bmiHeader.biXPelsPerMeter = 0;
		m_pInfo->bmiHeader.biYPelsPerMeter = 0;
		m_pInfo->bmiHeader.biClrUsed = 0;
		m_pInfo->bmiHeader.biClrImportant = 0;

		// 得到实际的位
		int test = ::GetDIBits(pDC->GetSafeHdc(), (HBITMAP)pSrcBitmap->GetSafeHandle(),
	 		0, (WORD)bmHdr.bmHeight, m_pPixels, m_pInfo, DIB_RGB_COLORS);

		// 检查要扫描的位图行的数量
		if( test != (int)bmHdr.bmHeight )
			throw TEXT("call to GetDIBits did not return full number of requested scan lines\n");
		
		CreatePalette();
		m_bIsPadded = FALSE;
#ifdef _DEBUG
	} catch( TCHAR * psz ) {
		TRACE1("CDIBitmap::CreateFromBitmap(): %s\n", psz);
#else
	} catch( TCHAR * ) {
#endif
		if( m_pPixels ) {
			delete [] m_pPixels;
			m_pPixels = 0;
		}
		if( m_pInfo ) {
			delete [] (BYTE*) m_pInfo;
			m_pInfo = 0;
		}
		return FALSE;
	}

	return TRUE;
}
/*
*--------------------------------------------------------------------------------
*  成员函数名   :LoadBitmap
*  功能描述     :从位图资源ID导入DIB
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/

BOOL CDIBitmap::LoadBitmap(UINT ID)
{
  return LoadResource(ID); 
}

BOOL CDIBitmap::LoadBitmap(LPCTSTR lpsID)
{
HBITMAP hBmp = (HBITMAP)::LoadImage(
						AfxGetInstanceHandle(), 
						lpsID, IMAGE_BITMAP,
						0,0, LR_LOADFROMFILE
					);

	if( hBmp == 0 ) 
		return FALSE;

	CBitmap bmp;
	bmp.Attach(hBmp);
	CClientDC cdc( CWnd::GetDesktopWindow() );
	BOOL bRet = CreateFromBitmap( &cdc, &bmp );
	bmp.DeleteObject();
	return bRet;
}

BOOL CDIBitmap::LoadResource(UINT ID)
{
 return LoadResource(MAKEINTRESOURCE(ID)); 
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :LoadResource
*  功能描述     :从外部文件导入DIB
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/

BOOL CDIBitmap :: LoadResource(LPCTSTR pszID) {
	HBITMAP hBmp = (HBITMAP)::LoadImage(
						AfxGetInstanceHandle(), 
						pszID, IMAGE_BITMAP,
						0,0, LR_CREATEDIBSECTION
					);

	if( hBmp == 0 ) 
		return FALSE;

	CBitmap bmp;
	bmp.Attach(hBmp);
	CClientDC cdc( CWnd::GetDesktopWindow() );
	BOOL bRet = CreateFromBitmap( &cdc, &bmp );
	bmp.DeleteObject();
	return bRet;
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :Load
*  功能描述     :从文件指针导入DIB
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
BOOL CDIBitmap :: Load( CFile* pFile ) {
    ASSERT( pFile );
    BOOL fReturn = TRUE;
    try {
        delete [] (BYTE*)m_pInfo;
        delete [] m_pPixels;
        m_pInfo = 0;
        m_pPixels = 0;
        DWORD       dwStart = pFile->GetPosition();
        //
        // 检查确信已有一个位图.头两个字节必须为'B' 和 'M'.
        BITMAPFILEHEADER fileHeader;
        pFile->Read(&fileHeader, sizeof(fileHeader));
        if( fileHeader.bfType != 0x4D42 )
            throw TEXT("Error:Unexpected file type, not a DIB\n");

        BITMAPINFOHEADER infoHeader;
        pFile->Read( &infoHeader, sizeof(infoHeader) );
        if( infoHeader.biSize != sizeof(infoHeader) )
            throw TEXT("Error:OS2 PM BMP Format not supported\n");

        //存储DIB结构的尺寸
        int cPaletteEntries = GetPalEntries( infoHeader );
        int cColorTable = 256 * sizeof(RGBQUAD);
        int cInfo = sizeof(BITMAPINFOHEADER) + cColorTable;
        int cPixels = fileHeader.bfSize - fileHeader.bfOffBits;
        //
        //分配位置为一个新位图信息头, 并复制这个头，读这个文件并保存结果到颜色表中
         m_pInfo = (BITMAPINFO*)new BYTE[cInfo];
        memcpy( m_pInfo, &infoHeader, sizeof(BITMAPINFOHEADER) );
        pFile->Read( ((BYTE*)m_pInfo) + sizeof(BITMAPINFOHEADER),
                     cColorTable );
        //
        // 为像素区域分配内存空间， 并从文件中导入像素数据
        m_pPixels = new BYTE[cPixels];
        pFile->Seek(dwStart + fileHeader.bfOffBits, CFile::begin);
        pFile->Read( m_pPixels, cPixels );
		CreatePalette();
		m_bIsPadded = TRUE;
#ifdef _DEBUG
    } catch( TCHAR * psz ) {
		TRACE( psz );
#else
    } catch( TCHAR * ) {
#endif
        fReturn = FALSE;
    }
    return fReturn;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :Load
*  功能描述     :从外部文件中导入DIB
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
BOOL CDIBitmap :: Load( const CString & strFilename ) {
	CFile file;
	if( file.Open( strFilename, CFile::modeRead ) )
		return Load( &file );
	return FALSE;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :Save
*  功能描述     :保存DIB到文件
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
BOOL CDIBitmap :: Save( const CString & strFileName ) {
    ASSERT(! strFileName.IsEmpty());

    CFile File;

	if( !File.Open(strFileName, CFile::modeCreate|CFile::modeWrite) ) {
	    TRACE1("CDIBitmap::Save(): 打开文件出错,当与入文件 %s时.\n", strFileName);
	    return FALSE;
	}

    return Save( &File );
}      


/*
*--------------------------------------------------------------------------------
*  成员函数名   : Save
*  功能描述     :保存DIB到文件
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
// 不能打开或关闭文件pFile.  假定调用它.
BOOL CDIBitmap :: Save( CFile * pFile ) {
	ASSERT_VALID( pFile );
    ASSERT( m_pInfo );
    ASSERT( m_pPixels );
    
    BITMAPFILEHEADER bmfHdr;
    
	DWORD dwPadWidth = PADWIDTH(GetWidth());

	// 确信位图数据已经被真补到格式中
	PadBits();
    
    bmfHdr.bfType = 0x4D42;
    // 初始化BitmapInfo尺寸
    DWORD dwImageSize= m_pInfo->bmiHeader.biSize;
	// Add in palette size
    WORD wColors = GetColorCount();
    WORD wPaletteSize = (WORD)(wColors*sizeof(RGBQUAD));
    dwImageSize += wPaletteSize;
    
    // 加入尺寸值到真正的位对列中
    dwImageSize += PADWIDTH((GetWidth()) * DWORD(m_pInfo->bmiHeader.biBitCount)/8) * GetHeight();
    m_pInfo->bmiHeader.biSizeImage = 0;
    bmfHdr.bfSize = dwImageSize + sizeof(BITMAPFILEHEADER);
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + m_pInfo->bmiHeader.biSize + wPaletteSize;
    pFile->Write(&bmfHdr, sizeof(BITMAPFILEHEADER));
    
    pFile->Write(m_pInfo, sizeof(BITMAPINFO) + (wColors-1)*sizeof(RGBQUAD));
    pFile->Write(m_pPixels,
		DWORD((dwPadWidth*(DWORD)m_pInfo->bmiHeader.biBitCount*GetHeight())/8) );
  
	return TRUE;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :CreatePalette()
*  功能描述     :创建调色板
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
BOOL CDIBitmap :: CreatePalette() {
	if( m_pPal )
		delete m_pPal;
	m_pPal = 0;
	ASSERT( m_pInfo );
	//假若颜色数 <= 256色，仅仅需要调色板。否则应当扩大内存
	if( m_pInfo->bmiHeader.biBitCount <= 8 )
		m_pPal = new CBmpPalette(this);
	return m_pPal ? TRUE : FALSE;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :ClearPalette()
*  功能描述     :清除调色板
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void CDIBitmap :: ClearPalette() {
	if( m_pPal )
		delete m_pPal;
	m_pPal = 0;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   : DrawDIB
*  功能描述     :画DIB
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void CDIBitmap :: DrawDIB( CDC* pDC, int x, int y )
 {
	DrawDIB( pDC, x, y, GetWidth(), GetHeight() );
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :DrawDIB
*  功能描述     :画DIB
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
// DrawDib uses StretchDIBits to display the bitmap.
void CDIBitmap :: DrawDIB( CDC* pDC, int x, int y, int width, int height ) {
    ASSERT( pDC );
    HDC     hdc = pDC->GetSafeHdc();

	CPalette * pOldPal = 0;

	if( m_pPal ) {
		pOldPal = pDC->SelectPalette( m_pPal, FALSE );
		pDC->RealizePalette();
		// 确信使用拉伸模式是最好的为要拉伸的彩色图片
		pDC->SetStretchBltMode(COLORONCOLOR);
	}

    if( m_pInfo )
        StretchDIBits( hdc,
                       x,
                       y,
                       width,
                       height,
                       0,
                       0,
                       GetWidth(),
                       GetHeight(),
                       GetPixelPtr(),
                       GetHeaderPtr(),
                       DIB_RGB_COLORS,
                       SRCCOPY );
	
	if( m_pPal )
		pDC->SelectPalette( pOldPal, FALSE );
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :DrawDIB
*  功能描述     :画DIB
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
int CDIBitmap :: DrawDIB( CDC * pDC, CRect & rectDC, CRect & rectDIB ) {
    ASSERT( pDC );
    HDC     hdc = pDC->GetSafeHdc();

	CPalette * pOldPal = 0;

	if( m_pPal ) {
		pOldPal = pDC->SelectPalette( m_pPal, FALSE );
		pDC->RealizePalette();
		//  确信使用拉伸模式是最好的为要拉伸的彩色图片
		pDC->SetStretchBltMode(COLORONCOLOR);
	}

	int nRet = 0;

    if( m_pInfo )
		nRet =	SetDIBitsToDevice(
					hdc,					// device
					rectDC.left,			// DestX
					rectDC.top,				// DestY
					rectDC.Width(),			// DestWidth
					rectDC.Height(),		// DestHeight
					rectDIB.left,			// SrcX
					GetHeight() -
						rectDIB.top -
						rectDIB.Height(),	// SrcY
					0,						// StartScan
					GetHeight(),			// NumScans
					GetPixelPtr(),			// color data
					GetHeaderPtr(),			// header data
					DIB_RGB_COLORS			// color usage
				);
	
	if( m_pPal )
		pDC->SelectPalette( pOldPal, FALSE );

	return nRet;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetHeaderPtr()
*  功能描述     :得以DIB头信息结构
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
BITMAPINFO * CDIBitmap :: GetHeaderPtr() const {
    ASSERT( m_pInfo );
    ASSERT( m_pPixels );
    return m_pInfo;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetColorTablePtr
*  功能描述     :得以当前调色板
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
RGBQUAD * CDIBitmap :: GetColorTablePtr() const {
    ASSERT( m_pInfo );
    ASSERT( m_pPixels );
    RGBQUAD* pColorTable = 0;
    if( m_pInfo != 0 ) {
        int cOffset = sizeof(BITMAPINFOHEADER);
        pColorTable = (RGBQUAD*)(((BYTE*)(m_pInfo)) + cOffset);
    }
    return pColorTable;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetPixelPtr()
*  功能描述     :得到当前象素
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
BYTE * CDIBitmap :: GetPixelPtr() const {
//     ASSERT( m_pInfo );
//     ASSERT( m_pPixels );
    return m_pPixels;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetWidth()
*  功能描述     :得到图形宽度
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
int CDIBitmap :: GetWidth() const {
    ASSERT( m_pInfo );
    return m_pInfo->bmiHeader.biWidth;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetHeight()
*  功能描述     :得到图形高度
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
int CDIBitmap :: GetHeight() const {
    ASSERT( m_pInfo );
    return m_pInfo->bmiHeader.biHeight;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetColorCount()
*  功能描述     :得到图形颜色色深位数
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
WORD CDIBitmap :: GetColorCount() const {
	ASSERT( m_pInfo );

	switch( m_pInfo->bmiHeader.biBitCount )	{
		case 1:		return 2;
		case 4:		return 16;
		case 8:		return 256;
		default:	return 0;
	}
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetPalEntries()
*  功能描述     :得到调色板数
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
int CDIBitmap :: GetPalEntries() const {
    ASSERT( m_pInfo );
    return GetPalEntries( *(BITMAPINFOHEADER*)m_pInfo );
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetPalEntries
*  功能描述     :得到调色板数
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
int CDIBitmap :: GetPalEntries( BITMAPINFOHEADER& infoHeader ) const {
	int nReturn;
	if( infoHeader.biClrUsed == 0 )
		nReturn = ( 1 << infoHeader.biBitCount );
	else
		nReturn = infoHeader.biClrUsed;

	return nReturn;
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetBitsPerPixel()
*  功能描述     :得到每个像素的位数
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/

DWORD CDIBitmap :: GetBitsPerPixel() const {
	ASSERT( m_pInfo );
	return m_pInfo->bmiHeader.biBitCount;
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :LastByte
*  功能描述     :得到最后位
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
DWORD CDIBitmap :: LastByte( DWORD dwBitsPerPixel, DWORD dwPixels ) const {
	register DWORD dwBits = dwBitsPerPixel * dwPixels;
	register DWORD numBytes  = dwBits / 8;
	register DWORD extraBits = dwBits - numBytes * 8;
    return (extraBits % 8) ? numBytes+1 : numBytes;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetBytesPerLine
*  功能描述     :得到每扫描行位数
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
DWORD CDIBitmap :: GetBytesPerLine( DWORD dwBitsPerPixel, DWORD dwWidth ) const {
	DWORD dwBits = dwBitsPerPixel * dwWidth;

	if( (dwBits % 32) == 0 )
         //已经 DWORD排列, 不需要填充
		return (dwBits/8);  //已经 DWORD aligned, 不需要填充
	
	DWORD dwPadBits = 32 - (dwBits % 32);
	return (dwBits/8 + dwPadBits/8 + (((dwPadBits % 8) > 0) ? 1 : 0));
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :PadBits()
*  功能描述     :增加位
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
BOOL CDIBitmap :: PadBits() {
	if( m_bIsPadded )
		return TRUE;

    // 当每像素位伸展超过1字节时
    DWORD dwAdjust = 1, dwOffset = 0, dwPadOffset=0;
	BOOL bIsOdd = FALSE;
    
	dwPadOffset = GetBytesPerLine(GetBitsPerPixel(), GetWidth());
	dwOffset = LastByte(GetBitsPerPixel(), GetWidth());

	if( dwPadOffset == dwOffset )
		return TRUE;

    BYTE * pTemp = new BYTE [GetWidth()*dwAdjust];
    if( !pTemp ) {
		TRACE1("CDIBitmap::PadBits(): 不能分配行宽度 %d.\n", GetWidth());
		return FALSE;
	}
    
    //已经为位队列分配了主够的内存空间包括填充料,因此必须需要移位.

    for( DWORD row = GetHeight()-1 ; row>0 ; --row ) {
	    CopyMemory((void *)pTemp, (const void *)(m_pPixels + (row*dwOffset)), dwOffset );
	    CopyMemory((void *)(m_pPixels + (row*dwPadOffset)), (const void *)pTemp, dwOffset);
	}
    delete [] pTemp;
    
    return TRUE;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :UnPadBits()
*  功能描述     :
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
BOOL CDIBitmap::UnPadBits() {
	if( ! m_bIsPadded )
		return TRUE;

	DWORD dwAdjust = 1;
	BOOL bIsOdd = FALSE;

	DWORD dwPadOffset = GetBytesPerLine(GetBitsPerPixel(), GetWidth());
	DWORD dwOffset = LastByte(GetBitsPerPixel(), GetWidth());

    BYTE * pTemp = new BYTE [dwOffset];
    if( !pTemp ) {
		TRACE1("CDIBitmap::UnPadBits() could not allocate row of width %d.\n", GetWidth());
		return FALSE;
	}

   //已经为位队列分配了主够的内存空间包括填充料,因此必须需要移位.
    for( DWORD row=1 ; row < DWORD(GetHeight()); ++row ) {
		CopyMemory((void *)pTemp, (const void *)(m_pPixels + row*(dwPadOffset)), dwOffset);
	    CopyMemory((void *)(m_pPixels + (row*dwOffset)), (const void *)pTemp, dwOffset);
	}

    delete [] pTemp;
    
    return TRUE;
}

