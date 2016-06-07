/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：DIBAPI.CPP
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：DIB图形处理API函数库模块文件（）
*======================================================
* 版本变更记录:
*      v1.0  2002-11-11  
*
* 
*/

 //使严格检测有效
   
 
#include "stdafx.h"
#include <string.h> 
#include <stdio.h> 
#include <math.h> 
#include <io.h> 
#include <direct.h> 
#include <stdlib.h> 
#include <assert.h> 
#include <windows.h> 
#include <windowsx.h> 
#include <mmsystem.h> 
#include "..\Include\dibapi.h" 

 
/************************************************************************* 
 * 函数名: CreateDIB() 
 * 
 * 参数: 
 * 
 * DWORD dwWidth    - 新位图的宽度(像素) 
 * DWORD dwHeight   - 新位图的高度(像素)  
 * WORD  wBitCount  - 新DIB的位数 (1, 4, 8, 或 24位) 
 * 
 * 返回值: 
 * 
 * HDIB             -新建DIB 的句柄
 * 
 * 描述: 
 * 
 * 这个函数初使化一个新的DIB并分配内存靠填充DIB的位图信息头结构体
 * BITMAPINFOHEADER, 为颜色表分配内存并为这个bitmap位分配内存. 
 * 这个头信息、颜色表和位所有都位于邻近的内存块中. 这个函数同
 * DDB的CreateBitmap() Windows API函数类似。 
 * 这个颜色表和位图位没有被初始化，即为0 
 * 
 ************************************************************************/ 

HDIB CreateDIB(DWORD dwWidth, DWORD dwHeight, WORD wBitCount) 
{ 
    // DIB位图文件并头结构 
    BITMAPINFOHEADER    bi;    
    // DIB位图信息头结构 
    LPBITMAPINFOHEADER  lpbi;      
    // 内存块大小
    DWORD               dwLen; 

    HDIB                hDIB; 
    //每扫描行字节数
    DWORD               dwBytesPerLine;
 
 
    //确信每像素位是有效的
 
    if (wBitCount <= 1) 
        wBitCount = 1; 
    else if (wBitCount <= 4) 
        wBitCount = 4; 
    else if (wBitCount <= 8) 
        wBitCount = 8; 
    else if (wBitCount <= 24) 
        wBitCount = 24; 
    else 
        // 设置默认的值是4位（16色）假若参数是无效的
        wBitCount = 4;
 
    // 初始化INFOHEADER 
     //本结构大小的字节 数
    bi.biSize = sizeof(BITMAPINFOHEADER);   
	// 位图的宽度（参数传入） 
    bi.biWidth = dwWidth;				
	// 位图的高度（参数传入）
    bi.biHeight = dwHeight;			
	//必须是1 
    bi.biPlanes = 1;				
	//像素的位数(参数传入) 
    bi.biBitCount = wBitCount;			
    //压缩方式
    bi.biCompression = BI_RGB;				
	//位图数据字节数(0为默认数)
    bi.biSizeImage = 0;					
	//水平分辨率（像素点/米）
    bi.biXPelsPerMeter = 0;				
	//垂直分辨率（像素点/米）
    bi.biYPelsPerMeter = 0;				
    //图像中使用的颜色数
    bi.biClrUsed = 0;						
    //图像中重要的颜面色数
    bi.biClrImportant = 0;					
 
    // 计算内存块的大小必须保存这个DIB.这个块应当是足够大的  This 
    dwBytesPerLine = WIDTHBYTES1(wBitCount * dwWidth); 
    dwLen = bi.biSize + PaletteSize((LPBYTE)&bi) + (dwBytesPerLine * dwHeight); 
 
    // 分配内存块和保存位图
     hDIB = GlobalAlloc(GHND, dwLen); 
     if (!hDIB) 
        return NULL; 
 
    // 锁定内存块和得到它的指针
 
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 
 
    //使用上面的位图信息结构 BITMAPINFOHEADER 填充DIB的第一部份 BITMAPINFOHEADER 
 
    *lpbi = bi; 
 
    // 因为我们不知道这个色彩表和位将包含, 
    // 正好移除这些空白.  解锁DIB内存块和返回HDIB名柄. 
 
    GlobalUnlock(hDIB); 
 
    return hDIB; 
} 

 
/************************************************************************* 
 * 函数名:CreateDefaultDIB() 
 * 
 * 参数: 
 * 
 * DWORD dwWidth    - 新位图的宽度(像素)  
 * DWORD dwHeight   - 新位图的高度(像素)   
 * 
 * 返回值: 
 * 
 * HDIB             - 新DIB的句柄 
 * 
 * 描述: 
 * 
 *本函数根据指事实上的参数创建一个新的DIB位图，类似创建DDB的CreateBitmap()  API函数
 *DIB的颜面色表将根据系统调色板来创建，位图的位数据未被初始化即为0
 * 
 ************************************************************************/ 
HDIB CreateDefaultDIB(DWORD dwWidth, DWORD dwHeight) 
{ 
	// 得到 DC
    HDC hDC = GetDC(NULL); 
    if (!hDC) 
        return NULL; 

	// DC bts/像素
	int nDeviceBitsPixel = GetDeviceCaps(hDC, BITSPIXEL);

	//  依造DC创建DIB 
	HDIB hDIB = CreateDIB(dwWidth, dwHeight, nDeviceBitsPixel);

	// DIB缓冲器
	LPBITMAPINFO lpbmi = (LPBITMAPINFO)GlobalLock(hDIB);
    LPBYTE lpDIBBits = FindDIBBits((LPBYTE)lpbmi); 
    DWORD dwBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 

	// 设置 DIB颜色为白色
	for (DWORD l=0; l<dwBitsSize; ++l)
		lpDIBBits[l] = 0xff;

	// 假如没有调色板, 返回DIB句柄
	if (nDeviceBitsPixel > 8)
	{
		GlobalUnlock(hDIB);
	    ReleaseDC(NULL, hDC); 
		return hDIB;
	}

	// 若是调色板,设置系统调色板到DIB

	// 系统调色板中的颜色
    int nColors = PalEntriesOnDevice(hDC);   // 色彩数
  
    // 复制系统调色板到我们的逻辑调色板
	PALETTEENTRY pe[256];
    GetSystemPaletteEntries(hDC, 0, nColors, pe); 

	//设置颜色表
	for (int i=0; i<nColors; ++i)
	{
		lpbmi->bmiColors[i].rgbRed = pe[i].peRed;
		lpbmi->bmiColors[i].rgbGreen = pe[i].peGreen;
		lpbmi->bmiColors[i].rgbBlue = pe[i].peBlue;
		lpbmi->bmiColors[i].rgbReserved = 0;
	}

    // 清除
	GlobalUnlock(hDIB);
    ReleaseDC(NULL, hDC); 

	return hDIB;
}

/************************************************************************* 
 * 函数：DestroyDIB () 
 * 参数：无
 * 描述：释放为DIB分配的内存 
 * 返回值：无 
 * 
 *************************************************************************/ 
void DestroyDIB(HDIB hDib) 
{ 
    GlobalFree(hDib); 
} 


/************************************************************************* 
 * 函数:  ReadDIBFile () 
 * 参数:
 * HANDLE hFile -指向DIB数据缓冲区的句柄
 * 
 * 描述: 
 * 读指定的DIB文件到内存中
 * 
 * 返回值:  
 * 成功返回一个hDIB句柄,否则返回NULL 
 * 
 * 注意: 
 * 这个函数不支持OS/2的DIB
 * 
 *************************************************************************/ 
HANDLE ReadDIBFile(HANDLE hFile) 
{ 
    BITMAPFILEHEADER    bmfHeader; 
    DWORD               dwBitsSize; 
     // 颜色表项数量    
    UINT             nNumColors;  
    HANDLE              hDIB;     
    // 使用为 GlobalRealloc() //MPB 
    HANDLE              hDIBtmp; 
    LPBITMAPINFOHEADER  lpbi; 
    DWORD               offBits; 
    DWORD               dwRead; 
 
    //当读文件时得到DIB的长度字节 
 
    dwBitsSize = GetFileSize(hFile, NULL); 
 
    // 为头 & 颜色表分配内存.并放大到内存到我们需要的

    hDIB = GlobalAlloc(GMEM_MOVEABLE, (DWORD)(sizeof(BITMAPINFOHEADER) + 
            256 * sizeof(RGBQUAD))); 
 
    if (!hDIB) 
        return NULL; 
 
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 
 
    if (!lpbi)  
    { 
        GlobalFree(hDIB); 
        return NULL; 
    } 
 
    // 从文件中读出 BITMAPFILEHEADER 头结构 
 
    if (!ReadFile(hFile, (LPBYTE)&bmfHeader, sizeof (BITMAPFILEHEADER), 
            &dwRead, NULL)) 
        goto ErrExit; 
 
    if (sizeof (BITMAPFILEHEADER) != dwRead) 
        goto ErrExit; 
    // 'BM' 
    if (bmfHeader.bfType != 0x4d42)  
        goto ErrExit; 
 
    //读出BITMAPINFOHEADER 结构
 
    if (!ReadFile(hFile, (LPBYTE)lpbi, sizeof(BITMAPINFOHEADER), &dwRead, 
            NULL)) 
        goto ErrExit; 
 
    if (sizeof(BITMAPINFOHEADER) != dwRead) 
        goto ErrExit; 
 
    // 检查是否是Windows DIB --  OS/2 DIB 的文件头和头表结构是不同的
    // 若不是Windows DIB (e.g. 如结构体大小是错误的),返回NULL
 
    if (lpbi->biSize == sizeof(BITMAPCOREHEADER)) 
        goto ErrExit; 
 
    // 现在决定颜色表的尺寸和读出它. 
    // 因为这个文件靠fOffBits偏移量, 需要作一些特别的处理以保证这些位
    // 直接跟随颜色表
 
    if (!(nNumColors = (UINT)lpbi->biClrUsed)) 
    { 
        // 24-bit真彩色没有颜色表,其它为默认尺寸 
         //(lpbi->biBitCount != 24) // 标准大小表 
        if (lpbi->biBitCount > 8)	
            nNumColors = 1 << lpbi->biBitCount; 
    } 
 
    // 若为0或空填充一些默认值 
 
    if (lpbi->biClrUsed == 0) 
        lpbi->biClrUsed = nNumColors; 
 
    if (lpbi->biSizeImage == 0) 
    { 
        lpbi->biSizeImage = ((((lpbi->biWidth * (DWORD)lpbi->biBitCount) + 
                31) & ~31) >> 3) * lpbi->biHeight; 
    } 
 
    //为头得到适当大小的缓冲区,颜色表和数据位 
 
    GlobalUnlock(hDIB); 
    hDIBtmp = GlobalReAlloc(hDIB, lpbi->biSize + nNumColors * 
            sizeof(RGBQUAD) + lpbi->biSizeImage, 0); 

    // 导入时不能调整缓冲区的大小
    if (!hDIBtmp) 
        goto ErrExitNoUnlock; //MPB 
    else 
        hDIB = hDIBtmp; 
 
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 
 
    // 读出颜色表
 
    ReadFile (hFile, (LPBYTE)(lpbi) + lpbi->biSize, 
            nNumColors * sizeof(RGBQUAD), &dwRead, NULL); 
 
    // 从开始到DIB头偏移数据位 
 
    offBits = lpbi->biSize + nNumColors * sizeof(RGBQUAD); 
 
    // 若 bfOffBits域非空, 那时数据位可能直接跟随在颜色表后
    // 使用查找这些数据位
 
    if (bmfHeader.bfOffBits != 0L) 
        SetFilePointer(hFile, bmfHeader.bfOffBits, NULL, FILE_BEGIN); 
 
    if (ReadFile(hFile, (LPBYTE)lpbi + offBits, lpbi->biSizeImage, &dwRead, 
            NULL)) 
        goto OKExit; 
 
ErrExit: 
    GlobalUnlock(hDIB);     
 
ErrExitNoUnlock:     
    GlobalFree(hDIB); 
    return NULL; 
 
OKExit: 
    GlobalUnlock(hDIB); 
    return hDIB; 
} 


/************************************************************************* 
 * 函数名：LoadDIB() 
 * 参数：
 * LPCTSTR lpFileName - 指定要装载的DIB源文件 
 *
 * 描述：
 * 导入这个指定的DIB源文件,并为它分配内存,从磁盘文件装载读到内存中 
 *
 * 返回值:
 * HANDLE - 指向DIB数据缓冲区的句柄,或若不成功返回NULL. 
 * 
 * 注意：本函数不支持OS/2 的DIB;
 * 
 *************************************************************************/ 
HDIB LoadDIB(LPCTSTR lpFileName) 
{ 
    HDIB        hDIB; 
    HANDLE      hFile; 
 
    // 设置等待沙漏光标以便导入文件 
 //   SetCursor(LoadCursor(NULL, IDC_WAIT)); 
     SetCursor(AfxGetApp()->LoadCursor(IDC_WAIT)); 
    if ((hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, 
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 
            NULL)) != INVALID_HANDLE_VALUE) 
    { 
        hDIB = ReadDIBFile(hFile); 
        CloseHandle(hFile); 
     //   SetCursor(LoadCursor(NULL, IDC_ARROW)); 
        SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW)); 
        return hDIB; 
    } 
    else 
    { 
       // SetCursor(LoadCursor(NULL, IDC_ARROW)); 
         SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW)); 
        return NULL; 
    } 
} 

 
 
/************************************************************************* 
 * 函数:SaveDIB() 
 * 参数: 
 * HDIB hDib - 要保存的DIB的内存句柄 
 * LPCTSTR lpFileName - 存储DIB的文件的完整路径名 
 *
 * 描述:
 * 以指定的文件名保存这个指定的DIB到磁盘,若文件已经存在则覆盖它  
 * 
 * 返回值:
 * TRUE-保存成功
 * FALSE-保存失败
 * 
 *************************************************************************/ 

BOOL SaveDIB(HDIB hDib, LPCTSTR lpFileName) 
{ 
    BITMAPFILEHEADER    bmfHdr;     
    LPBITMAPINFOHEADER  lpBI;       
    HANDLE              fh;         
    DWORD               dwDIBSize; 
    DWORD               dwWritten; 
 
    if (!hDib) 
        return FALSE; 
 
    fh = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 
 
    if (fh == INVALID_HANDLE_VALUE) 
        return FALSE; 
 
    //得到一个指向DIB内存的指针,开始时包含BITMAPINFO结构信息 
    lpBI = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
    if (!lpBI) 
    { 
        CloseHandle(fh); 
        return FALSE; 
    } 
 
    // 检查是否使用 OS/2 DIB.   
    if (lpBI->biSize != sizeof(BITMAPINFOHEADER)) 
    { 
        GlobalUnlock(hDib); 
        CloseHandle(fh); 
        return FALSE; 
    } 
 
    // 填充文件头的域(对位图开始两个字节必须为"BM" ) 
 
    bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM" 
 
    //计算DIB的尺寸 
 
    // 局部计算
     dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPBYTE)lpBI);   
 
    //计算位图数据尺寸 （REL类型位图）
    if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) 
        dwDIBSize += lpBI->biSizeImage; 
    else 
    { 
        DWORD dwBmBitsSize;  
 
        //非RLE位图,尺寸为宽*高 
 
        dwBmBitsSize = WIDTHBYTES1((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * 
                lpBI->biHeight; 
 
        dwDIBSize += dwBmBitsSize; 
        lpBI->biSizeImage = dwBmBitsSize; 
    } 
 
 
    //计算文件的尺寸靠加入DIB尺寸到(BITMAPFILEHEADER) 
                    
    bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER); 
    bmfHdr.bfReserved1 = 0; 
    bmfHdr.bfReserved2 = 0; 
 
    // 计算位图位的偏移： 位图文件头加上DIB头加上颜色表头尺寸. 
     
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize + 
            PaletteSize((LPBYTE)lpBI); 
 
    //写文件头 
     WriteFile(fh, (LPBYTE)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 
 
    // 写DIB头和位 --使用逻辑坐标，因此可写入的数据可以超过32767字节     
    WriteFile(fh, (LPBYTE)lpBI, dwDIBSize, &dwWritten, NULL); 
 
    GlobalUnlock(hDib); 
    CloseHandle(fh); 
 
    if (dwWritten == 0) 
        return FALSE; 
    else 
        return TRUE;  
} 
 
/************************************************************************* 
 * 函数:FindDIBBits() 
 * 
 * 参数: 
 * LPBYTE lpDIB      - DIB 内存块指针
 * 
 * 返回值:
 * LPBYTE            - DIB数据位指针 
 *
 * 描述: 
 * 这个函数计算DIB数据位的地址和返加数据位的指针
 * 
 ************************************************************************/ 
LPBYTE FindDIBBits(LPBYTE lpDIB) 
{ 
   return (lpDIB + *(LPDWORD)lpDIB + PaletteSize(lpDIB)); 
} 

 
/************************************************************************* 
 * 函数:DIBWidth() 
 * 
 * 参数: 
 * LPBYTE lpDIB      - DIB内存块指针 
 * 
 * 返回值: 
 * DWORD            - DIB 的宽度
 * 
 * 描述: 
 * 这个函数从BITMAPINFOHEADER结构宽度项返回DIB的宽度 (Windows 3.0类型) 
 * 或从BITMAPCOREHEADER结构宽度项返回DIB宽度(OS/2类型). 
 * 
 ************************************************************************/ 
DWORD DIBWidth(LPBYTE lpDIB) 
{ 
	// Win 3.0-类型 DIB 
    LPBITMAPINFOHEADER   lpbmi;  
     // OS/2-类型DIB 
    LPBITMAPCOREHEADER   lpbmc;  
 
    // 指向头 (Win 3.0或OS/2) 
     lpbmi = (LPBITMAPINFOHEADER)lpDIB; 
     lpbmc = (LPBITMAPCOREHEADER)lpDIB; 
 
    // 返回DIB宽度(Win 3.0 DIB) 
     if (lpbmi->biSize == sizeof(BITMAPINFOHEADER)) 
        return lpbmi->biWidth; 
    // 返回DIB宽度( OS/2 DIB) 
    else  
        return (DWORD)lpbmc->bcWidth; 
} 


DWORD DIBWidth(HDIB hDIB) 
{ 
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	DWORD dw = DIBWidth(lpDIB);
	GlobalUnlock(hDIB);
	return dw;
} 
 
/************************************************************************* 
 * 函数:DIBHeight() 
 * 
 * 参数:
 * 
 * LPBYTE lpDIB      - DIB内存块指针
 * 
 * 返回值
 * DWORD            - DIB高度
 * 
 * 描述:
 * 这个函数从BITMAPINFOHEADER结构宽度项返回DIB的高度 (Windows 3.0类型) 
 * 或从BITMAPCOREHEADER结构宽度项返回DIB高度(OS/2类型). 
 * 
 ************************************************************************/ 
DWORD DIBHeight(LPBYTE lpDIB) 
{ 

   //指向Win3.0-类型DIB 
   LPBITMAPINFOHEADER   lpbmi;  
    //指向OS/2-类型DIB 
   LPBITMAPCOREHEADER   lpbmc;  
 
   //指向头 (OS/2或 Win 3.0 )
    lpbmi = (LPBITMAPINFOHEADER)lpDIB; 
   lpbmc = (LPBITMAPCOREHEADER)lpDIB; 
 
    // 返回 DIB高度(Win 3.0 DIB )
    if (lpbmi->biSize == sizeof(BITMAPINFOHEADER)) 
        return lpbmi->biHeight; 
    else  //返回DIB高度(OS/2 DIB) 
        return (DWORD)lpbmc->bcHeight; 
} 
 
DWORD DIBHeight(HDIB hDIB) 
{ 
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	DWORD dw = DIBHeight(lpDIB);
	GlobalUnlock(hDIB);
	return dw;
} 

 
/************************************************************************* 
 * 函数:PaletteSize() 
 * 
 * 参数: 
 * LPBYTE lpDIB      - DIB内存块指针
 * 
 * 返回值: 
 * WORD             - DIB调色板的尺寸
 * 
 * 描述:  
 * 这个函数可以得到需要保存的DIB的调色板的尺寸 =
 * RGBQUAD乘以颜色数(Windows 3.0-类型的DIB)
 * 或乘这个RGBTRIPLE的尺寸 (OS/2-类型的DIB). 
 * 
 ************************************************************************/ 
WORD PaletteSize(LPBYTE lpDIB) 
{ 
    // 计算被调色板需要的尺寸
    if (IS_WIN30_DIB (lpDIB)) 
        return (DIBNumColors(lpDIB) * sizeof(RGBQUAD)); 
    else 
        return (DIBNumColors(lpDIB) * sizeof(RGBTRIPLE)); 
} 
 
WORD PaletteSize(HDIB hDIB) 
{ 
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	WORD wSize = PaletteSize(lpDIB);
	GlobalUnlock(hDIB);
	return wSize;
} 

 
/************************************************************************* 
 * 
 * 函数名:DIBNumColors() 
 * 
 * 参数: 
 * 
 * LPBYTE lpDIB      - 指向一个充满DIB内存的块 
 * 
 * Return Value: 
 * 
 * WORD             - 颜色表中颜色的数量
 * 
 * 描述: 
 * 
 * 这个函数计算颜色表中的颜色数量靠这个DIB的每一像素（1: colors=2,  4: colors=16, if 8: colors=256, 
 *  对24位位图无颜色表. 
 * 
 ************************************************************************/ 
WORD DIBNumColors(LPBYTE lpDIB) 
{ 
    // DIB位数量 
	WORD wBitCount;  
 
    // 如果是 Windows类型DIB, 颜色数在颜色表中的数量少于允许的数量  
 
    if (IS_WIN30_DIB(lpDIB)) 
    { 
        DWORD dwClrUsed; 
 
        dwClrUsed = ((LPBITMAPINFOHEADER)lpDIB)->biClrUsed; 
        if (dwClrUsed) 
 
        return (WORD)dwClrUsed; 
    } 
 
    // 计算基于每像素的颜色数量     
    if (IS_WIN30_DIB(lpDIB)) 
        wBitCount = ((LPBITMAPINFOHEADER)lpDIB)->biBitCount; 
    else 
        wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount; 
 
    switch (wBitCount) 
    { 
        case 1: 
            return 2; 
 
        case 4: 
            return 16; 
 
        case 8: 
            return 256; 
 
        default: 
            return 0; 
    } 
} 

/************************************************************************* 
 * 
 * 函数名:DIBNumColors() 
 *
 * 描述: 计算DIB颜色数量
 * 参数: 
 * HDIB hDIB- DIB句柄
 *    
 * 返回值：DIB颜色数量 
 ************************************************************************/ 
WORD DIBNumColors(HDIB hDIB) 
{ 
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	WORD wSize = DIBNumColors(lpDIB);
	GlobalUnlock(hDIB);
	return wSize;
} 


/************************************************************************* 
 * 
 * 函数名:DIBBitCount() 
 *
 * 描述: 计算DIB图形位数
 * 参数: 
 * LPBYTE lpDIB- 指向BITMAPINFOHEADER 结构体指针
 *    
 * 返回值：DIB颜色数量 
 ************************************************************************/ 
WORD DIBBitCount(LPBYTE lpDIB) 
{ 
    if (IS_WIN30_DIB(lpDIB)) 
        return ((LPBITMAPINFOHEADER)lpDIB)->biBitCount; 
    else 
        return ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount; 
} 


/************************************************************************* 
 * 
 * 函数名:DIBBitCount() 
 *
 * 描述: 计算DIB位数
 * 参数: 
 * HDIB hDIB- DIB句柄
 *    
 * 返回值：DIB颜色数量 
 ************************************************************************/ 
WORD DIBBitCount(HDIB hDIB) 
{ 
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	WORD wSize = DIBBitCount(lpDIB);
	GlobalUnlock(hDIB);
	return wSize;
} 
 
/************************************************************************* 
 * 
 * 函数名:BytesPerLine() 
 *
 * 描述: 计算每扫描行的字节位数
 * 参数: 
 * LPBYTE lpDIB - 指向BITMAPINFOHEADER 结构体指针
 *    
 * 返回值：每扫描行的字节数量 
 ************************************************************************/  
DWORD BytesPerLine(LPBYTE lpDIB) 
{ 
    return WIDTHBYTES1(((LPBITMAPINFOHEADER)lpDIB)->biWidth * ((LPBITMAPINFOHEADER)lpDIB)->biPlanes * ((LPBITMAPINFOHEADER)lpDIB)->biBitCount); 
} 

/************************************************************************* 
 * 
 * 函数名:BytesPerLine() 
 *
 * 描述: 计算每扫描行的字节位数
 * 参数: 
 * HDIB hDIB - DIB名柄
 *    
 * 返回值：每扫描行的字节数量 
 ************************************************************************/ 
DWORD BytesPerLine(HDIB hDIB) 
{ 
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	DWORD dw = BytesPerLine(lpDIB);
	GlobalUnlock(hDIB);
	return dw;
} 

/************************************************************************* 
 * 
 * 函数名:DIBlockSize() 
 *
 * 描述: 计算DIB块每扫描行的字节位数
 * 参数: 
 * HDIB hDIB - DIB名柄
 *    
 * 返回值：每扫描行的字节数量 
 ************************************************************************/ 
DWORD DIBlockSize(LPBYTE lpDIB)
{
	if (((LPBITMAPINFOHEADER)lpDIB)->biSizeImage == 0)
		((LPBITMAPINFOHEADER)lpDIB)->biSizeImage = BytesPerLine(lpDIB) * ((LPBITMAPINFOHEADER)lpDIB)->biHeight;
	return ((LPBITMAPINFOHEADER)lpDIB)->biSize + PaletteSize(lpDIB) + ((LPBITMAPINFOHEADER)lpDIB)->biSizeImage;
}


/************************************************************************* 
 * 
 * 函数名:DIBlockSize() 
 *
 * 描述: 计算DIB块每扫描行的字节位数
 * 参数: 
 * LPBYTE lpDIB - 指向BITMAPINFOHEADER 结构体指针
 *    
 * 返回值：每扫描行的字节数量 
 ************************************************************************/
DWORD DIBlockSize(HDIB hDIB) 
{ 
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	DWORD dw = DIBlockSize(lpDIB);
	GlobalUnlock(hDIB);
	return dw;
} 


 /************************************************************************* 
 * 
 * 函数名:CreateDIBPalette()  
 *
 * 描述: 创建DIB调色板
 * 参数: 
 * 
 * LPBYTE lpbi       - 指定的DIB 
 *    
 * 返回值：创建的调色板
 ************************************************************************/
HPALETTE CreateDIBPalette(LPBYTE lpbi) 
{ 
    // 指向一个逻辑调色面板
    LPLOGPALETTE        lpPal;  
	// 逻辑调色面板句柄
    HANDLE              hLogPal;        
    HPALETTE            hPal = NULL;    
    // 颜色表中的颜色数量
    int                 i, wNumColors;  
	// 指向BITMAPINFO结构(Win3.0) 
    LPBITMAPINFO        lpbmi;  
	 // 指向BITMAPCOREINFO结构(OS/2) 
    LPBITMAPCOREINFO    lpbmc;         
    BOOL                bWinStyleDIB;  
 
    //假若句柄有错返回NULL 
 
    if (! lpbi) 
        return NULL; 
 
    //得到BITMAPINFO指针 (Win 3.0) 
     lpbmi = (LPBITMAPINFO)lpbi; 
 
    //得到BITMAPCOREINFO (OS/2 1.x)指针 
     lpbmc = (LPBITMAPCOREINFO)lpbi; 
 
    // 得到DIB颜色数量  
     wNumColors = DIBNumColors(lpbi); 
 
    // 是 Win 3.0 DIB? 
     bWinStyleDIB = IS_WIN30_DIB(lpbi); 
    if (wNumColors) 
    { 
        // 为逻辑调色板分配内存
         hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + 
                sizeof(PALETTEENTRY) * wNumColors); 
 
        // 若无足够内存 
        if (!hLogPal) 
            return NULL; 
 
        //锁定内存和指向它
         lpPal = (LPLOGPALETTE)GlobalLock(hLogPal); 
 
        //设置调色板的数量和版本
         lpPal->palVersion = PALVERSION; 
        lpPal->palNumEntries = wNumColors; 
 
        // 保存RGB(Win 3.0 DIB)或 RGB quads (if OS/2 DIB)      
        for (i = 0; i < wNumColors; i++) 
        { 
            if (bWinStyleDIB) 
            { 
                lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed; 
                lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen; 
                lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue; 
                lpPal->palPalEntry[i].peFlags = 0; 
            } 
            else 
            { 
                lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed; 
                lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen; 
                lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue; 
                lpPal->palPalEntry[i].peFlags = 0; 
            } 
        } 
 
        // 创建调色板和指向它 
        hPal = CreatePalette(lpPal); 
        if (!hPal) 
        { 
            GlobalUnlock(hLogPal); 
            GlobalFree(hLogPal); 
            return NULL; 
        } 
    } 
 
    return hPal; 
} 
 

 /************************************************************************* 
 * 
 * 函数名:CreateDIBPalette()  
 *
 * 描述: 创建DIB调色板
 * 参数: 
 * 
 *  HDIB hDIB        - 指定的DIB句柄
 *    
 * 返回值：创建的调色板
 ************************************************************************/
HPALETTE CreateDIBPalette(HDIB hDIB) 
{ 
    //调色板句柄
    HPALETTE            hPal = NULL;
    //指向packed-DIB 
    LPBYTE               lpbi;      
 
    if (!hDIB) 
        return NULL; 
 
    // 锁定DIB内存并指它 
    lpbi = (LPBYTE)GlobalLock(hDIB); 
 
	hPal = CreateDIBPalette(lpbi);

	// 解除锁定
    GlobalUnlock(hDIB); 
 
    return hPal; 
} 
 

 /************************************************************************* 
 * 
 * 函数名:DIBToBitmap() 
 *
 * 描述: DIB转换为Bitmap
 * 参数: 
 * 
 * HDIB hDIB        - 指定的要转换的DIB
 * 
 * HPALETTE hPal    - 指定Bitmap要使用的调色板
 *    
 * 返回值：转换后的Bitmap句柄
 ************************************************************************/

HBITMAP DIBToBitmap(HDIB hDIB, HPALETTE hPal) 
{ 
  
	LPBYTE       lpDIBHdr, lpDIBBits;  
    HBITMAP     hBitmap;           
    HDC         hDC;                
    HPALETTE    hOldPal = NULL;    

    if (!hDIB) 
        return NULL; 
 
    lpDIBHdr = (LPBYTE)GlobalLock(hDIB); 
 
    //得到DIB位指针 
    lpDIBBits = FindDIBBits(lpDIBHdr); 
 
    //得到DC  
     hDC = GetDC(NULL); 
    if (!hDC) 
    { 
        GlobalUnlock(hDIB); 
        return NULL; 
    } 
 
    //选择和初始化调色板 
    if (hPal) 
	{
        hOldPal = SelectPalette(hDC, hPal, FALSE); 
	    RealizePalette(hDC); 
	}
 
    // 从DIB头信息和位数据创建位图 
    hBitmap = CreateDIBitmap(hDC, 
							(LPBITMAPINFOHEADER)lpDIBHdr, 
							CBM_INIT, 
							lpDIBBits, 
							(LPBITMAPINFO)lpDIBHdr, 
							DIB_RGB_COLORS);
 
    //备份以前的调色板 
    if (hOldPal) 
        SelectPalette(hDC, hOldPal, FALSE); 

    ReleaseDC(NULL, hDC); 
    GlobalUnlock(hDIB); 
 
    return hBitmap; 
} 
 
 /************************************************************************* 
 * 
 * 函数名:BitmapToDIB() 
 *
 * 描述:Bitmap转换为 DIB
 * 参数: 
 * 
 * HBITMAP hBitmap       - 指定的要转换的Bitmap
 * 
 * HPALETTE hPal    - 指定Bitmap要使用的调色板
 *    
 * 返回值：转换后的DIB句柄
 ************************************************************************/

HDIB BitmapToDIB(HBITMAP hBitmap, HPALETTE hPal) 
{ 
	// bitmap 结构
    BITMAP              bm;    
	 // bitmap 头
    BITMAPINFOHEADER    bi;   
	//BITMAPINFOHEADER 指针
    LPBITMAPINFOHEADER  lpbi;  
	 //内存块的尺寸 
    DWORD               dwLen;     
    HANDLE              hDIB, h;    
    HDC                 hDC; 
	//像素位
    WORD                biBits;    
 
    if (!hBitmap) 
        return NULL; 
 
    //填充BITMAP结构 
    if (!GetObject(hBitmap, sizeof(bm), (LPBYTE)&bm)) 
        return NULL; 
 
    // 若没有指定调色板,使用默认调色板 
     if (hPal == NULL) 
        hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE); 
 
    // 计算每像素的位数
    biBits = bm.bmPlanes * bm.bmBitsPixel; 
 
    // 确信每像素都是有效的 
    if (biBits <= 1) 
        biBits = 1; 
    else if (biBits <= 4) 
        biBits = 4; 
    else if (biBits <= 8) 
        biBits = 8; 
    else 
        biBits = 24; 
 
    //初始化 BITMAPINFOHEADER 
    bi.biSize = sizeof(BITMAPINFOHEADER); 
    bi.biWidth = bm.bmWidth; 
    bi.biHeight = bm.bmHeight; 
    bi.biPlanes = 1; 
    bi.biBitCount = biBits; 
    bi.biCompression = BI_RGB; 
    bi.biSizeImage = 0; 
    bi.biXPelsPerMeter = 0; 
    bi.biYPelsPerMeter = 0; 
    bi.biClrUsed = 0; 
    bi.biClrImportant = 0; 
 
    //计算内存块的尺寸必须保存 BITMAPINFO 
 
    dwLen = bi.biSize + PaletteSize((LPBYTE)&bi); 
 
    // 得到一个DC 
     hDC = GetDC(NULL); 
 
    // 选择调色板
     hPal = SelectPalette(hDC, hPal, FALSE); 
    RealizePalette(hDC); 
 
    //分配内存以保存位图
     hDIB = GlobalAlloc(GHND, dwLen); 
 
    if (!hDIB) 
    { 
      SelectPalette(hDC, hPal, TRUE); 
      RealizePalette(hDC); 
      ReleaseDC(NULL, hDC); 
      return NULL; 
    } 
 
    //锁定内存和锁定它
     lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 
 
    *lpbi = bi; 
 
    GetDIBits(hDC, hBitmap, 0, (UINT)bi.biHeight, NULL, (LPBITMAPINFO)lpbi, 
        DIB_RGB_COLORS); 
 
    bi = *lpbi; 
    GlobalUnlock(hDIB); 
 
    if (bi.biSizeImage == 0) 
        bi.biSizeImage = WIDTHBYTES1((DWORD)bm.bmWidth * biBits) * bm.bmHeight; 
 
    //重新分配内存以便容纳所有的位
 
    dwLen = bi.biSize + PaletteSize((LPBYTE)&bi) + bi.biSizeImage; 
 
    if (h = GlobalReAlloc(hDIB, dwLen, 0)) 
        hDIB = h; 
    else 
    {
		 GlobalFree(hDIB); 
        hDIB = NULL; 
        SelectPalette(hDC, hPal, TRUE); 
        RealizePalette(hDC); 
        ReleaseDC(NULL, hDC); 
        return NULL; 
    }
 
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 
 
    if (GetDIBits(hDC, hBitmap, 0, (UINT)bi.biHeight, (LPBYTE)lpbi + 
            (WORD)lpbi->biSize + PaletteSize((LPBYTE)lpbi), (LPBITMAPINFO)lpbi, 
            DIB_RGB_COLORS) == 0) 
    { 
        GlobalUnlock(hDIB); 
        hDIB = NULL; 
        SelectPalette(hDC, hPal, TRUE); 
        RealizePalette(hDC); 
        ReleaseDC(NULL, hDC); 
        return NULL; 
    } 
 
    bi = *lpbi; 

    GlobalUnlock(hDIB); 
    SelectPalette(hDC, hPal, TRUE); 
    RealizePalette(hDC); 
    ReleaseDC(NULL, hDC); 

    return hDIB; 
} 
 

 /************************************************************************* 
 * 
 *函数名: BitmapToDIB() 
 *
 * 描述:Bitmap转换为 DIB
 * 参数: 
 * 
 * HBITMAP hBitmap       - 指定的要转换的Bitmap
 * 
 * HPALETTE hPal    - 指定Bitmap要使用的调色板
 *
 * WORD wBitCount   - 指定转换后的DIB颜色位
 *    
 * 返回值：转换后的DIB句柄
 ************************************************************************/
HDIB BitmapToDIB(HBITMAP hBitmap, HPALETTE hPalette, WORD wBitCount)
{ 
	HDIB hNewDib;

	if (! hBitmap)
        return NULL; 

	//得到 bitmap 信息
	BITMAP  bm;        
	GetObject(hBitmap, sizeof(bm), (LPBYTE)&bm); 
	int biBits = bm.bmPlanes * bm.bmBitsPixel;

	if (biBits <= 1) 
		biBits = 1; 
	else if (biBits <= 4) 
		biBits = 4; 
	else if (biBits <= 8) 
		biBits = 8; 
	else 
		biBits = 24; 

	//从当前DIB句柄中得到当前bitmap
	HDIB hDib = BitmapToDIB(hBitmap, hPalette);
	if (! hDib)
        return NULL; 

	if (wBitCount == biBits)
		hNewDib = hDib;
	else
	{
		hNewDib = ConvertDIBFormat((HANDLE)hDib, (UINT)wBitCount, hPalette);

		GlobalFree(hDib);
	}

	return hNewDib;
} 

 /************************************************************************* 
 * 
 * 函数名:ChangeBitmapFormat()
 *
 * 描述:转换Bitmap格式到其它位格式
 * 参数: 
 * 
 * HBITMAP          - bitmap句柄 
 * 
 * WORD             - 每像素要转换的位数
 * 
 * DWORD            - 压缩格式类型
 * 
 * HPALETTE         - 调色板句柄 
 *    
 * 返回值：转换后的DIB句柄
 ************************************************************************/
HDIB ChangeBitmapFormat (HBITMAP hbm, WORD biBits, DWORD biCompression, HPALETTE hpal)
{
    BITMAP               bm;
    BITMAPINFOHEADER     bi;
    LPBITMAPINFOHEADER   lpbi;
    DWORD                dwLen;
    HANDLE               hdib;
    HANDLE               h;
    HDC                  hdc;

    if (!hbm)
        return NULL;

    if (hpal == NULL)
        hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

    GetObject(hbm,sizeof(bm),(LPBYTE)&bm);

    if (biBits == 0)
	{
        biBits =  bm.bmPlanes * bm.bmBitsPixel;

	    if (biBits <= 1) 
		    biBits = 1; 
	    else if (biBits <= 4) 
		    biBits = 4; 
	    else if (biBits <= 8) 
		    biBits = 8; 
		else 
			biBits = 24; 
	}

    bi.biSize               = sizeof(BITMAPINFOHEADER);
    bi.biWidth              = bm.bmWidth;
    bi.biHeight             = bm.bmHeight;
    bi.biPlanes             = 1;
    bi.biBitCount           = biBits;
    bi.biCompression        = biCompression;
    bi.biSizeImage          = 0;
    bi.biXPelsPerMeter      = 0;
    bi.biYPelsPerMeter      = 0;
    bi.biClrUsed            = 0;
    bi.biClrImportant       = 0;

    dwLen  = bi.biSize + PaletteSize((LPBYTE)&bi);

    hdc = GetDC(NULL);
    HPALETTE hpalT = SelectPalette(hdc,hpal,FALSE);
    RealizePalette(hdc);

    hdib = GlobalAlloc(GHND,dwLen);

    if (!hdib)
	{
        SelectPalette(hdc,hpalT,FALSE);
        ReleaseDC(NULL,hdc);
        return NULL;
    }

    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);

    *lpbi = bi;


    GetDIBits(hdc, hbm, 0L, (DWORD)bi.biHeight,
        (LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

    bi = *lpbi;
    GlobalUnlock(hdib);

    if (bi.biSizeImage == 0)
	{
        bi.biSizeImage = WIDTHBYTES1((DWORD)bm.bmWidth * biBits) * bm.bmHeight;

        if (biCompression != BI_RGB)
            bi.biSizeImage = (bi.biSizeImage * 3) / 2;
    }

    ///分配足够的内存
    dwLen = bi.biSize + PaletteSize((LPBYTE)&bi) + bi.biSizeImage;
    if (h = GlobalReAlloc(hdib,dwLen,0))
        hdib = h;
    else
	{
        GlobalFree(hdib);
        hdib = NULL;

        SelectPalette(hdc,hpalT,FALSE);
        ReleaseDC(NULL,hdc);
        return hdib;
    }

    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);

    if (! GetDIBits(hdc,
                    hbm,
                    0L,
                    (DWORD)bi.biHeight,
                    (LPBYTE)lpbi + (WORD)lpbi->biSize + PaletteSize((LPBYTE)lpbi),
                    (LPBITMAPINFO)lpbi, 
					(DWORD)DIB_RGB_COLORS))
	{
         GlobalUnlock(hdib);
         hdib = NULL;
         SelectPalette(hdc,hpalT,FALSE);
         ReleaseDC(NULL,hdc);
         return NULL;
    }
    bi = *lpbi;
   
	GlobalUnlock(hdib);
    SelectPalette(hdc,hpalT,FALSE);
    ReleaseDC(NULL,hdc);
    return hdib;
}

 /************************************************************************* 
 * 
 * 函数名:ChangeDIBFormat() 
 *
 * 描述:转换DIB格式到其它位格式
 * 参数: 
 * 
 * HDIB              - DIB句柄 
 * 
 * WORD             - 每像素要转换的位数
 * 
 * DWORD            - 压缩格式类型
 * 
 * HPALETTE         - 调色板句柄 
 *    
 * 返回值：转换后的DIB句柄
 ************************************************************************/
HDIB ChangeDIBFormat(HDIB hDIB, WORD wBitCount, DWORD dwCompression) 
{ 

    HBITMAP            hBitmap;         
    //新DIB句柄
    HDIB               hNewDIB = NULL;  
    HPALETTE           hPal;			

    //检查DIB是否有效
     if (!hDIB) 
        return NULL; 
 
    // 保存旧DIB调色板
     hPal = CreateDIBPalette(hDIB); 
    if (hPal == NULL)
        hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
 
    // 转换旧DIB到bitmap 
     hBitmap = DIBToBitmap(hDIB, hPal); 
    if (!hBitmap) 
    { 
        DeleteObject(hPal); 
        return NULL; 
    } 

	// 改变Bitmap格式
	hNewDIB = ChangeBitmapFormat(hBitmap, wBitCount, dwCompression, hPal);
 
    DeleteObject(hBitmap); 
    DeleteObject(hPal); 
 
    return hNewDIB; 
} 
 

 /************************************************************************* 
 * 
 * 函数名:ConvertDIBFormat () 
 *
 * 描述:转换DIB格式到其它位格式
 * 参数: 
 * 
 * LPBITMAPINFO lpSrcDIB - 资源CF_DIB 
 * UINT         nWidth   - 新DIB的宽度 
 * UINT         nHeight  - 新DIB的高度 
 * UINT         nbpp     - 新DIB的类型 
 * BOOL         bStretch - TRUE拉伸资源，FALSE图形到左上角 
 * HPALETTE	 hPalSrc     - 新资源使用的调色板 
 * DWORD		 dwSize  - 新DIB 的指针
 *    
 * 返回值：转换后的DIB句柄
 ************************************************************************/
HDIB ConvertDIBFormat(LPBYTE lpDIB, UINT nWidth, UINT nHeight, UINT nbpp, BOOL bStretch, HPALETTE hPalSrc) 
{ 
	LPBITMAPINFO lpSrcDIB =  (LPBITMAPINFO)lpDIB;
    LPBITMAPINFO lpbmi = NULL; 
    LPBYTE       lpSourceBits, lpTargetBits, lpResult; 
    HDC			 hDC = NULL, hSourceDC, hTargetDC; 
    HBITMAP      hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize, dwTargetBitsSize, dwTargetHeaderSize, dwColorNum; 
	HDIB		 hDib;
 
    // 分配和填充BITMAPINFO结构为这个新DIB 
    if (nbpp <= 8) 
		dwColorNum = 1 << nbpp;
	else
		dwColorNum = 0;
    dwTargetHeaderSize = sizeof( BITMAPINFO ) + ( dwColorNum * sizeof( RGBQUAD ) ); 
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize ); 
    lpbmi->bmiHeader.biSize = sizeof( BITMAPINFOHEADER ); 
    lpbmi->bmiHeader.biWidth = nWidth; 
    lpbmi->bmiHeader.biHeight = nHeight; 
    lpbmi->bmiHeader.biPlanes = 1; 
    lpbmi->bmiHeader.biBitCount = nbpp; 
    lpbmi->bmiHeader.biCompression = BI_RGB; 
    lpbmi->bmiHeader.biSizeImage = 0; 
    lpbmi->bmiHeader.biXPelsPerMeter = 0; 
    lpbmi->bmiHeader.biYPelsPerMeter = 0; 
    lpbmi->bmiHeader.biClrUsed = 0; 
    lpbmi->bmiHeader.biClrImportant = 0; 
    // 填充颜色表
    if( ! CopyColorTable( lpbmi, (LPBITMAPINFO)lpSrcDIB, hPalSrc ) ) 
    { 
        free( lpbmi ); 
        return NULL; 
    } 
 
	hDC = GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC ); 
 
    // 运算 DIBSection到相配的 DIB 
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPBYTE)lpSrcDIB), dwSourceBitsSize ); 
    lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize; 
 
    //选DIBSections到DCs中 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap ); 
 
    //为DIBSections设置颜色表 
    if( lpSrcDIB->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hSourceDC, 0, 1 << lpSrcDIB->bmiHeader.biBitCount, lpSrcDIB->bmiColors ); 
    if( lpbmi->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hTargetDC, 0, 1 << lpbmi->bmiHeader.biBitCount, lpbmi->bmiColors ); 
 
    // 若要一个真拷贝 
    if( (lpSrcDIB->bmiHeader.biWidth==lpbmi->bmiHeader.biWidth) && (lpSrcDIB->bmiHeader.biHeight==lpbmi->bmiHeader.biHeight) ) 
    { 
        BitBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, SRCCOPY ); 
    } 
    else 
    { 
        // 否则拉伸它
        if( bStretch ) 
        { 
            SetStretchBltMode( hTargetDC, COLORONCOLOR ); 
            StretchBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, lpSrcDIB->bmiHeader.biWidth, lpSrcDIB->bmiHeader.biHeight, SRCCOPY ); 
        } 
        else 
        { 
            // 或在资源的左上角
            BitBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, SRCCOPY ); 
        } 
    } 
 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    SelectObject( hTargetDC, hOldTargetBitmap ); 
    DeleteDC( hSourceDC ); 
    DeleteDC( hTargetDC ); 
    ReleaseDC( NULL, hDC ); 
 
    GdiFlush(); 
 
    // 分配足够的内存款为新DIB,并拷贝数据位 
    hDib = GlobalAlloc(GHND, dwTargetHeaderSize + dwTargetBitsSize ); 
    lpResult = (LPBYTE)GlobalLock(hDib); 
    memcpy( lpResult, lpbmi, dwTargetHeaderSize ); 
    memcpy( FindDIBBits( (LPBYTE)lpResult ), lpTargetBits, dwTargetBitsSize ); 

    DeleteObject( hTargetBitmap ); 
    DeleteObject( hSourceBitmap ); 
	free( lpbmi ); 
    GlobalUnlock(hDib);
 
    return hDib; 
} 

 /************************************************************************* 
 * 
 * 函数名:ConvertDIBFormat () 
 *
 * 描述:转换DIB格式到其它位格式
 * 参数: 
 * 
 * LPBITMAPINFO lpSrcDIB - 资源CF_DIB 
 * UINT         nWidth   - 新DIB的宽度 
 * UINT         nHeight  - 新DIB的高度 
 * UINT         nbpp     - 新DIB的类型 
 * BOOL         bStretch - TRUE拉伸资源，FALSE图形到左上角 
 * HPALETTE	 hPalSrc     - 新资源使用的调色板 
 * DWORD		 dwSize  - 新DIB 的指针
 *    
 * 返回值：转换后的DIB句柄
 ************************************************************************/
HDIB ConvertDIBFormat(HDIB hDIB, UINT nWidth, UINT nHeight, UINT nbpp, BOOL bStretch, HPALETTE hPalSrc)
{
    LPBITMAPINFO lpbmi = NULL; 
    LPBYTE       lpSourceBits, lpTargetBits, lpResult; 
    HDC			 hDC = NULL, hSourceDC, hTargetDC; 
    HBITMAP      hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize, dwTargetBitsSize, dwTargetHeaderSize, dwColorNum; 
	HDIB		 hNewDIB;
	DWORD		 dwSize;

	// 得到DIB指针
	if (! hDIB)
		return NULL;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDIB);
	if (! lpSrcDIB)
		return NULL;

    // 分配和填充BITMAPINFO结构为这个新DIB 
    if (nbpp <= 8) 
		dwColorNum = 1 << nbpp;
	else
		dwColorNum = 0;
    dwTargetHeaderSize = sizeof( BITMAPINFO ) + ( dwColorNum * sizeof( RGBQUAD ) ); 
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize ); 
    lpbmi->bmiHeader.biSize = sizeof( BITMAPINFOHEADER ); 
    lpbmi->bmiHeader.biWidth = nWidth; 
    lpbmi->bmiHeader.biHeight = nHeight; 
    lpbmi->bmiHeader.biPlanes = 1; 
    lpbmi->bmiHeader.biBitCount = nbpp; 
    lpbmi->bmiHeader.biCompression = BI_RGB; 
    lpbmi->bmiHeader.biSizeImage = 0; 
    lpbmi->bmiHeader.biXPelsPerMeter = 0; 
    lpbmi->bmiHeader.biYPelsPerMeter = 0; 
    lpbmi->bmiHeader.biClrUsed = 0; 
    lpbmi->bmiHeader.biClrImportant = 0; 
    //填充颜色表
    if( ! CopyColorTable( lpbmi, (LPBITMAPINFO)lpSrcDIB, hPalSrc ) ) 
    { 
        free( lpbmi ); 
        return NULL; 
    } 
 
	hDC = GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC ); 
 
    //运算资源 DIB片段以匹配资源DIB 
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPBYTE)lpSrcDIB), dwSourceBitsSize ); 
    lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize; 
 
    //选择DIB片段到 DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap ); 
 
    // 为DIB片段选择颜色表 
    if( lpSrcDIB->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hSourceDC, 0, 1 << lpSrcDIB->bmiHeader.biBitCount, lpSrcDIB->bmiColors ); 
    if( lpbmi->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hTargetDC, 0, 1 << lpbmi->bmiHeader.biBitCount, lpbmi->bmiColors ); 
 
    if( (lpSrcDIB->bmiHeader.biWidth==lpbmi->bmiHeader.biWidth) && (lpSrcDIB->bmiHeader.biHeight==lpbmi->bmiHeader.biHeight) ) 
    { 
        BitBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, SRCCOPY ); 
    } 
    else 
    { 
        if( bStretch ) 
        { 
            SetStretchBltMode( hTargetDC, COLORONCOLOR ); 
            StretchBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, lpSrcDIB->bmiHeader.biWidth, lpSrcDIB->bmiHeader.biHeight, SRCCOPY ); 
        } 
        else 
        { 
            BitBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, SRCCOPY ); 
        } 
    } 
 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    SelectObject( hTargetDC, hOldTargetBitmap ); 
    DeleteDC( hSourceDC ); 
    DeleteDC( hTargetDC ); 
    ReleaseDC( NULL, hDC ); 
 
    GdiFlush(); 
 
    //分配足够的内存为新DIB
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND, dwSize);
    lpResult = (LPBYTE)GlobalLock(hNewDIB);
    memcpy( lpResult, lpbmi, dwTargetHeaderSize ); 
    memcpy( FindDIBBits( (LPBYTE)lpResult ), lpTargetBits, dwTargetBitsSize ); 

    DeleteObject( hTargetBitmap ); 
    DeleteObject( hSourceBitmap ); 
    free( lpbmi ); 
	GlobalUnlock(hDIB);
	GlobalUnlock(hNewDIB);
 
    return hNewDIB;
}


/************************************************************************* 
 *
 * 函数名:ConvertDIBFormat () 
 *
 * 描述:转换DIB格式到其它位格式
 * 参数: 
 * 
 * LPBYTE	lpDIB    - 资源CF_DIB
 * UINT         nbpp     - 新DIB的类型
 * HPALETTE	hPalSrc  - 新资源使用的调色板 
 *    
 * 返回值：转换后的DIB句柄
 ************************************************************************/
HDIB ConvertDIBFormat(LPBYTE lpDIB, UINT nbpp, HPALETTE hPalSrc)
{
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)lpDIB;
    LPBITMAPINFO lpbmi = NULL; 
    LPBYTE       lpSourceBits, lpTargetBits, lpResult; 
    HDC			 hDC = NULL, hSourceDC, hTargetDC; 
    HBITMAP      hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize, dwTargetBitsSize, dwTargetHeaderSize, dwColorNum; 
	HDIB		 hNewDIB;
	DWORD		 dwSize;
	int			 nWidth, nHeight;

	nWidth = lpSrcDIB->bmiHeader.biWidth;
	nHeight = lpSrcDIB->bmiHeader.biHeight;

    //分配足够的内存为新DIB 
    if (nbpp <= 8) 
		dwColorNum = 1 << nbpp;
	else
		dwColorNum = 0;
    dwTargetHeaderSize = sizeof( BITMAPINFO ) + ( dwColorNum * sizeof( RGBQUAD ) ); 
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize ); 
    lpbmi->bmiHeader.biSize = sizeof( BITMAPINFOHEADER ); 
    lpbmi->bmiHeader.biWidth = nWidth; 
    lpbmi->bmiHeader.biHeight = nHeight; 
    lpbmi->bmiHeader.biPlanes = 1; 
    lpbmi->bmiHeader.biBitCount = nbpp; 
    lpbmi->bmiHeader.biCompression = BI_RGB; 
    lpbmi->bmiHeader.biSizeImage = 0; 
    lpbmi->bmiHeader.biXPelsPerMeter = 0; 
    lpbmi->bmiHeader.biYPelsPerMeter = 0; 
    lpbmi->bmiHeader.biClrUsed = 0; 
    lpbmi->bmiHeader.biClrImportant = 0; 
    //填充颜色表
    if( ! CopyColorTable( lpbmi, (LPBITMAPINFO)lpSrcDIB, hPalSrc ) ) 
    { 
        free( lpbmi ); 
        return NULL; 
    } 

	hDC = GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC ); 
 
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPBYTE)lpSrcDIB), dwSourceBitsSize ); 
    lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize; 
 

    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap ); 
 
    // 为DIB片段选取颜色表 
    if( lpSrcDIB->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hSourceDC, 0, 1 << lpSrcDIB->bmiHeader.biBitCount, lpSrcDIB->bmiColors ); 
    if( lpbmi->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hTargetDC, 0, 1 << lpbmi->bmiHeader.biBitCount, lpbmi->bmiColors ); 
 
    BitBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, SRCCOPY ); 
 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    SelectObject( hTargetDC, hOldTargetBitmap ); 
    DeleteDC( hSourceDC ); 
    DeleteDC( hTargetDC ); 
    ReleaseDC( NULL, hDC ); 
 
    GdiFlush(); 
 
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND, dwSize);
    lpResult = (LPBYTE)GlobalLock(hNewDIB);
    memcpy( lpResult, lpbmi, dwTargetHeaderSize ); 
    memcpy( FindDIBBits( (LPBYTE)lpResult ), lpTargetBits, dwTargetBitsSize ); 
 
    DeleteObject( hTargetBitmap ); 
    DeleteObject( hSourceBitmap ); 
    free( lpbmi ); 
	GlobalUnlock(hNewDIB);
 
    return hNewDIB;
}




/**************************************************************************** 
*  
* 函数名:ConvertDIBFormat () 
*
* 描述:转换DIB格式到其它位格式
* 参数: 
*  HDIB	       hDIB      - 新资源的DIB句柄 
*  UINT         nbpp     - 新DIB的类型
*  HPALETTE	hPalSrc  - 新资源使用的调色板 
* 
* 返回值：转换后的DIB句柄
* 
\****************************************************************************/ 

HDIB ConvertDIBFormat(HDIB hDIB, UINT nbpp, HPALETTE hPalSrc)
{
    LPBITMAPINFO lpbmi = NULL; 
    LPBYTE       lpSourceBits, lpTargetBits, lpResult; 
    HDC			 hDC = NULL, hSourceDC, hTargetDC; 
    HBITMAP      hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize, dwTargetBitsSize, dwTargetHeaderSize, dwColorNum; 
	HANDLE		 hNewDIB;
	DWORD		 dwSize;
	int			 nWidth, nHeight;

	//得到DIB指针
	if (! hDIB)
		return NULL;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDIB);
	if (! lpSrcDIB)
		return NULL;
	nWidth = lpSrcDIB->bmiHeader.biWidth;
	nHeight = lpSrcDIB->bmiHeader.biHeight;

    //分配足够的内存为新DIB
    if (nbpp <= 8) 
		dwColorNum = 1 << nbpp;
	else
		dwColorNum = 0;
    dwTargetHeaderSize = sizeof( BITMAPINFO ) + ( dwColorNum * sizeof( RGBQUAD ) ); 
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize ); 
    lpbmi->bmiHeader.biSize = sizeof( BITMAPINFOHEADER ); 
    lpbmi->bmiHeader.biWidth = nWidth; 
    lpbmi->bmiHeader.biHeight = nHeight; 
    lpbmi->bmiHeader.biPlanes = 1; 
    lpbmi->bmiHeader.biBitCount = nbpp; 
    lpbmi->bmiHeader.biCompression = BI_RGB; 
    lpbmi->bmiHeader.biSizeImage = 0; 
    lpbmi->bmiHeader.biXPelsPerMeter = 0; 
    lpbmi->bmiHeader.biYPelsPerMeter = 0; 
    lpbmi->bmiHeader.biClrUsed = 0; 
    lpbmi->bmiHeader.biClrImportant = 0; 
    //填充颜色表
    if( ! CopyColorTable( lpbmi, (LPBITMAPINFO)lpSrcDIB, hPalSrc ) ) 
    { 
        free( lpbmi ); 
        return NULL; 
    } 
 
	hDC = GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC ); 

    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPBYTE)lpSrcDIB), dwSourceBitsSize ); 
    lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize; 
 
    //选取DIB片段到DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap ); 
 
    //为DIB片段设置颜色表
    if( lpSrcDIB->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hSourceDC, 0, 1 << lpSrcDIB->bmiHeader.biBitCount, lpSrcDIB->bmiColors ); 
    if( lpbmi->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hTargetDC, 0, 1 << lpbmi->bmiHeader.biBitCount, lpbmi->bmiColors ); 
 
    BitBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, SRCCOPY ); 
 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    SelectObject( hTargetDC, hOldTargetBitmap ); 
    DeleteDC( hSourceDC ); 
    DeleteDC( hTargetDC ); 
    ReleaseDC( NULL, hDC ); 

    GdiFlush(); 

	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND, dwSize);
    lpResult = (LPBYTE)GlobalLock(hNewDIB);
    memcpy( lpResult, lpbmi, dwTargetHeaderSize ); 
    memcpy( FindDIBBits( (LPBYTE)lpResult ), lpTargetBits, dwTargetBitsSize ); 
 
    DeleteObject( hTargetBitmap ); 
    DeleteObject( hSourceBitmap ); 
    free( lpbmi ); 
	GlobalUnlock(hDIB);
	GlobalUnlock(hNewDIB);
 
    return hNewDIB;
}

 

/**************************************************************************** 
* 
* 函数名:CopyColorTable()
* 
*  描述:  拷贝颜色表从一个DIB到其它的DIB. 
* 
*  参数:   LPBITMAPINFO lpTarget - 目的 DIB指针 
*          LPBITMAPINFO lpSource - 资源 DIB指针 
* 
*  返回值:  返回TRUE表示成功,否则表示失败. 
* 
\****************************************************************************/ 
BOOL CopyColorTable( LPBITMAPINFO lpTarget, LPBITMAPINFO lpSource, HPALETTE hPalSrc ) 
{ 
    //使用目标颜色深度
    switch( lpTarget->bmiHeader.biBitCount ) 
    { 
        // 8bpp - 需要256色颜色表
        case 8: 
			if (hPalSrc)
			{ 
				PALETTEENTRY    pe[256]; 
				UINT            i; 

				GetPaletteEntries( hPalSrc, 0, 256, pe ); 
				for(i=0;i<256;i++) 
				{ 
					lpTarget->bmiColors[i].rgbRed = pe[i].peRed; 
					lpTarget->bmiColors[i].rgbGreen = pe[i].peGreen; 
					lpTarget->bmiColors[i].rgbBlue = pe[i].peBlue; 
					lpTarget->bmiColors[i].rgbReserved = 0; 
				} 
			}
			else
			{
				if( lpSource->bmiHeader.biBitCount == 8 ) 
				{ 
					memcpy( lpTarget->bmiColors, lpSource->bmiColors, 256*sizeof(RGBQUAD) ); 
				} 
				else 
				{ 
					HPALETTE        hPal; 
					HDC            hDC = GetDC( NULL ); 
					PALETTEENTRY    pe[256]; 
					UINT            i; 
 
					hPal = CreateOctreePalette((LPBYTE)lpSource, 236, 8);
					if (! hPal)	               
						hPal = CreateHalftonePalette( hDC ); 
					ReleaseDC( NULL, hDC ); 

					GetPaletteEntries( hPal, 0, 256, pe ); 
					DeleteObject( hPal ); 
					for(i=0;i<256;i++) 
					{ 
						lpTarget->bmiColors[i].rgbRed = pe[i].peRed; 
						lpTarget->bmiColors[i].rgbGreen = pe[i].peGreen; 
						lpTarget->bmiColors[i].rgbBlue = pe[i].peBlue; 
						lpTarget->bmiColors[i].rgbReserved = pe[i].peFlags; 
					} 
				}
			}
			break; // end 8bpp 
 
        // 4bpp - 需要16色颜色表
        case 4: 
			if (hPalSrc)
			{ 
				PALETTEENTRY    pe[16]; 
				UINT            i; 

				GetPaletteEntries( hPalSrc, 0, 16, pe ); 
				for(i=0;i<16;i++) 
				{ 
					lpTarget->bmiColors[i].rgbRed = pe[i].peRed; 
					lpTarget->bmiColors[i].rgbGreen = pe[i].peGreen; 
					lpTarget->bmiColors[i].rgbBlue = pe[i].peBlue; 
					lpTarget->bmiColors[i].rgbReserved = 0; 
				} 
			}
			else
			{ 
				if( lpSource->bmiHeader.biBitCount == 4 ) 
				{ 
					memcpy( lpTarget->bmiColors, lpSource->bmiColors, 16*sizeof(RGBQUAD) ); 
				} 
				else 
				{ // != 4bpp, 使用系统调色板 
					HPALETTE        hPal; 
					PALETTEENTRY    pe[256]; 
					UINT            i; 
 
					hPal = (HPALETTE)GetStockObject( DEFAULT_PALETTE ); 
					GetPaletteEntries( hPal, 0, 16, pe ); 
					for(i=0;i<16;i++) 
					{ 
						lpTarget->bmiColors[i].rgbRed = pe[i].peRed; 
						lpTarget->bmiColors[i].rgbGreen = pe[i].peGreen; 
						lpTarget->bmiColors[i].rgbBlue = pe[i].peBlue; 
						lpTarget->bmiColors[i].rgbReserved = pe[i].peFlags; 
					}
                } 
			}
			break; // end 4bpp 
 
        // 1bpp - 需要2位 mono色颜色表 
        case 1: 
            lpTarget->bmiColors[0].rgbRed = 0; 
            lpTarget->bmiColors[0].rgbGreen = 0; 
            lpTarget->bmiColors[0].rgbBlue = 0; 
            lpTarget->bmiColors[0].rgbReserved = 0; 
            lpTarget->bmiColors[1].rgbRed = 255; 
            lpTarget->bmiColors[1].rgbGreen = 255; 
            lpTarget->bmiColors[1].rgbBlue = 255; 
            lpTarget->bmiColors[1].rgbReserved = 0; 
			break; // end 1bpp 
 
        // > 8bpp 模式
        case 32: 
        case 24: 
        case 16: 
        default: 
	        break; 
    } 
    return TRUE; 
} 



/**************************************************************************** 
* 
* 函数名:DIBToDIBSection() 
* 
* 描述：创建DIB片段从DIB
* 
* 参数:   LPBYTE lpDIB - DIB数据缓冲区指针
* 
* 返回值:  HBITMAP -DIB片断句柄, 为空表示失败 
* 
\****************************************************************************/ 
HBITMAP DIBToDIBSection(LPBYTE lpDIB) 
{
    LPBYTE       lpSourceBits; 
    HDC			 hDC = NULL, hSourceDC; 
    HBITMAP      hSourceBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize; 

	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)lpDIB;
	if (! lpSrcDIB)
		return NULL;

	hDC = GetDC( NULL ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
 
    // 填充资源 DIB片段到匹配的资源DIB 
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPBYTE)lpSrcDIB), dwSourceBitsSize ); 
 
    //资源 DIB片段到DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
 
    //为资源 DIB片段设置颜色表
    if( lpSrcDIB->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hSourceDC, 0, 1 << lpSrcDIB->bmiHeader.biBitCount, lpSrcDIB->bmiColors ); 

    // Clean up and delete the DCs 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    DeleteDC( hSourceDC ); 
    ReleaseDC( NULL, hDC ); 
 
    // Flush the GDI batch, so we can play with the bits 
    GdiFlush(); 
 
    return hSourceBitmap;
} 

/**************************************************************************** 
* 
* 函数名:DIBToDIBSection() 
* 
* 描述：创建DIB片段从DIB
* 
* 参数:   HIDB DIB - DIB句柄
* 
* 返回值:  HBITMAP -DIB片断句柄, 为空表示失败 
* 
\****************************************************************************/ 
HBITMAP DIBToDIBSection(HDIB hDIB) 
{
    HBITMAP      hSourceBitmap; 

	//得到DIB指针
	if (! hDIB)
		return NULL;
	LPBYTE lpSrcDIB = (LPBYTE)GlobalLock(hDIB);
	if (! lpSrcDIB)
		return NULL;

	hSourceBitmap = DIBToDIBSection(lpSrcDIB);

	GlobalUnlock(hDIB);
 
    return hSourceBitmap;
} 

/**************************************************************************** 
* 
* 函数名: DIBSectionToDIB() 
* 
* 描述:  从DIB片断创建DIB
* 
* 参数:  HBITMAP - DIB片断指针,为NULL表示错误 
* 
* 返回值:  HBITMAP hBitmap - DIB 句柄
* 
\****************************************************************************/ 
HDIB DIBSectionToDIB(HBITMAP hBitmap)
{
    HDC			 hDC = NULL, hSourceDC; 
	HBITMAP		 hOldSourceBitmap;
	HANDLE		 hNewDIB;
    LPBITMAPINFO lpbmi = NULL; 
	DWORD		 dwSize;
	DIBSECTION   ds;
    DWORD		 dwColorNum; 

	//得到DIB信息
	GetObject(hBitmap, sizeof(DIBSECTION), &ds);
    dwColorNum = ds.dsBmih.biClrUsed; 
    if (dwColorNum == 0 && ds.dsBmih.biBitCount <= 8) 
		dwColorNum = 1 << ds.dsBmih.biBitCount;

    //按BITMAPINFO 结构分配内存给新DIB
	dwSize = sizeof(BITMAPINFOHEADER) + (dwColorNum * sizeof(RGBQUAD)) + ds.dsBmih.biSizeImage;
	hNewDIB = GlobalAlloc(GHND, dwSize);
	if (! hNewDIB)
		return NULL;
	lpbmi = (LPBITMAPINFO)GlobalLock(hNewDIB);
	if (! lpbmi)
		return NULL;
    lpbmi->bmiHeader.biSize = sizeof( BITMAPINFOHEADER ); 
    lpbmi->bmiHeader.biWidth = ds.dsBm.bmWidth; 
    lpbmi->bmiHeader.biHeight = ds.dsBm.bmHeight; 
    lpbmi->bmiHeader.biPlanes = 1; 
    lpbmi->bmiHeader.biBitCount = ds.dsBmih.biBitCount; 
    lpbmi->bmiHeader.biCompression = ds.dsBmih.biCompression; 
    lpbmi->bmiHeader.biSizeImage = ds.dsBmih.biSizeImage; 
    lpbmi->bmiHeader.biXPelsPerMeter = ds.dsBmih.biXPelsPerMeter; 
    lpbmi->bmiHeader.biYPelsPerMeter = ds.dsBmih.biYPelsPerMeter; 
    lpbmi->bmiHeader.biClrUsed = ds.dsBmih.biClrUsed; 
    lpbmi->bmiHeader.biClrImportant = ds.dsBmih.biClrImportant; 

	// 为操作得到DC
	hDC = GetDC( NULL ); 
 
	//得到DIB位
    if (! GetDIBits(hDC,
                    hBitmap,
                    0L,
                    (DWORD)ds.dsBm.bmHeight,
                    (LPBYTE)lpbmi + (WORD)lpbmi->bmiHeader.biSize + (dwColorNum*sizeof(RGBQUAD)),
                    (LPBITMAPINFO)lpbmi, 
					(DWORD)DIB_RGB_COLORS))
	{
         GlobalUnlock(hNewDIB);
         ReleaseDC(NULL,hDC);
         return NULL;
    }

	// 内存DC
    hSourceDC = CreateCompatibleDC( hDC ); 
    // 选取DIB片断到DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hBitmap ); 
 
    // 从DIB片断中填充颜色表
    if( lpbmi->bmiHeader.biBitCount <= 8 ) 
        GetDIBColorTable( hSourceDC, 0, 1 << lpbmi->bmiHeader.biBitCount, lpbmi->bmiColors ); 
 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    DeleteDC( hSourceDC ); 
    ReleaseDC( NULL, hDC ); 
	GlobalUnlock(hNewDIB);
 
    return hNewDIB; 
}


/************************************************************************* 
 * 
 * 函数名:CopyScreenToBitmap() 
 * 
 * 描述:拷贝屏幕到Bitmap
 *  
 * 参数: 
 * 
 * LPRECT lpRect    - 指定的窗口区域
 * 
 * Return Value: 
 * 
 * HDIB             - 依赖设备的bitmap句柄 
 * 
 ************************************************************************/

HBITMAP CopyScreenToBitmap(LPRECT lpRect) 
{ 
   // 屏幕DC和内存DC 
    HDC         hScrDC, hMemDC;        
   //依赖设备的bitmaps 
    HBITMAP     hBitmap, hOldBitmap;    

    int         nX, nY, nX2, nY2;       
    // DIB 宽度和高度
    int         nWidth, nHeight;       
   // 屏幕点
    int         xScrn, yScrn;         
 
 
    if (IsRectEmpty(lpRect)) 
      return NULL; 
 
     
    hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL); 
    hMemDC = CreateCompatibleDC(hScrDC); 
 
 
    nX = lpRect->left; 
    nY = lpRect->top; 
    nX2 = lpRect->right; 
    nY2 = lpRect->bottom; 
 
    //得到屏幕模式
    xScrn = GetDeviceCaps(hScrDC, HORZRES); 
    yScrn = GetDeviceCaps(hScrDC, VERTRES); 
 
    //确信位图矩形区是可见的
 
    if (nX < 0) 
        nX = 0; 
    if (nY < 0) 
        nY = 0; 
    if (nX2 > xScrn) 
        nX2 = xScrn; 
    if (nY2 > yScrn) 
        nY2 = yScrn; 
 
    nWidth = nX2 - nX; 
    nHeight = nY2 - nY; 
 
    // 创建一个bitmap使用屏幕兼容模式 
    hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight); 
 
    // 选取新位图到内存DC 
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); 
 
    BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY); 
 
     hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap); 
 
    DeleteDC(hScrDC); 
    DeleteDC(hMemDC); 
 
    return hBitmap; 
} 


/************************************************************************* 
 * 
 * 函数名:CopyWindowToBitmap() 
 * 
 * 参数: 
 * 
 * HWND hWnd        - 指定的窗口句柄 
 * 
 * WORD fPrintArea  - 指定的窗口面积 
 * 
 *  返回值:  
 * 
 * HDIB          - 依赖设备的bitmap句柄 
 * 
 * 描述: 拷贝指定窗口到Bitmap
 * 
 * 
 ************************************************************************/  
HBITMAP CopyWindowToBitmap(HWND hWnd, WORD fPrintArea) 
{ 
    
	HBITMAP     hBitmap = NULL; 
 
    //检查窗口句柄
     if (!hWnd) 
        return NULL; 
 
    switch (fPrintArea) 
    { 
    // 复制整个窗口尺寸
	case PW_WINDOW:  
        { 
            RECT    rectWnd; 
 
            //得以窗口矩形区
 
            GetWindowRect(hWnd, &rectWnd); 
 
        
            hBitmap = CopyScreenToBitmap(&rectWnd); 
            break; 
        } 
         //拷贝客户区 
        case PW_CLIENT: 
        { 
            RECT rectClient; 
            POINT pt1, pt2; 
 
            // 得到客户区尺寸
             GetClientRect(hWnd, &rectClient); 
 
            //转变客户区到屏幕区 
             pt1.x = rectClient.left; 
            pt1.y = rectClient.top; 
            pt2.x = rectClient.right; 
            pt2.y = rectClient.bottom; 
            ClientToScreen(hWnd, &pt1); 
            ClientToScreen(hWnd, &pt2); 
            rectClient.left = pt1.x; 
            rectClient.top = pt1.y; 
            rectClient.right = pt2.x; 
            rectClient.bottom = pt2.y; 
 
            // 得到客户区的位图             
            hBitmap = CopyScreenToBitmap(&rectClient); 
            break; 
        } 
     // 初始化打印面积
        default:    
            return NULL; 
    } 
 
   return hBitmap; 
} 
 
 
/************************************************************************* 
 * 
 * 函数名:CopyClientRectToBitmap() 
 * 
 * 参数: 
 * 
 * HWND hWnd        - 指定的窗口句柄
 * 
 * LPRECT lpRect    - 指定要拷贝的客户区面积
 * 
 *  返回值: 
 * 
 * HDIB             - 依赖设备的bitmap句柄 
 * 
 * 描述: 
 * 
 * 拷贝窗口听客户区部份到bitmap. 
 * 
 ************************************************************************/ 
HBITMAP CopyClientRectToBitmap(HWND hWnd, LPRECT lpRect) 
{ 
   HBITMAP     hBitmap = NULL;  
 
    if (!hWnd) 
        return NULL; 
 
    POINT   pt1, pt2; 

    pt1.x = lpRect->left; 
    pt1.y = lpRect->top; 
    pt2.x = lpRect->right; 
    pt2.y = lpRect->bottom; 
    ClientToScreen(hWnd, &pt1); 
    ClientToScreen(hWnd, &pt2); 
    lpRect->left = pt1.x; 
    lpRect->top = pt1.y; 
    lpRect->right = pt2.x; 
    lpRect->bottom = pt2.y; 

    // 得到客户区的DIB
    hBitmap = CopyScreenToBitmap(lpRect); 
 
	//保存lpRect
    pt1.x = lpRect->left; 
    pt1.y = lpRect->top; 
    pt2.x = lpRect->right; 
    pt2.y = lpRect->bottom; 
    ScreenToClient(hWnd, &pt1); 
    ScreenToClient(hWnd, &pt2); 
    lpRect->left = pt1.x; 
    lpRect->top = pt1.y; 
    lpRect->right = pt2.x; 
    lpRect->bottom = pt2.y; 
 
   return hBitmap; 
} 
 
/************************************************************************* 
 * 
 * 函数名:CopyScreenToDIB() 
 * 
 * 参数:
 * 
 * LPRECT lpRect    - 指定的窗口区域
 * 
 *  返回值:  
 * 
 * HDIB             - 依赖设备的bitmap句柄 
 * 
 * 描述:
 * 
 * 拷贝屏幕指定的区域到bitmap. 
 * 
 ************************************************************************/ 
HDIB CopyScreenToDIB(LPRECT lpRect) 
{ 
    HBITMAP     hBitmap;       
    HPALETTE    hPalette;       
    HDIB        hDIB = NULL;   
 
    //得到设备相关DDB
     hBitmap = CopyScreenToBitmap(lpRect); 
 
    if (!hBitmap) 
      return NULL; 
 
    //得到当前调色板 
    hPalette = GetSystemPalette(); 
 
    // 转换DDB到DIB
     hDIB = BitmapToDIB(hBitmap, hPalette); 
 
    DeleteObject(hPalette); 
    DeleteObject(hBitmap); 
 
    return hDIB; 
} 
 
/************************************************************************* 
 * 
 * 函数名:CopyWindowToDIB() 
 * 
 * 参数:
 * 
 * HWND hWnd        - 指定的窗口句柄 
 * 
 * WORD fPrintArea  - 指定的窗口面积
 * 
 * 返回值:  
 * 
 * HDIB             - 依赖设备的bitmap句柄 
 * 
 * 描述:
 * 
 * 拷贝窗口指定的部份到bitmap
 * 
 ************************************************************************/ 
HDIB CopyWindowToDIB(HWND hWnd, WORD fPrintArea) 
{ 
   HDIB     hDIB = NULL;  
 
    if (!hWnd) 
        return NULL; 
 
    switch (fPrintArea) 
    { 
    // 拷贝整个屏幕
	case PW_WINDOW:
        { 
            RECT    rectWnd; 
 
            //提到窗口尺寸
             GetWindowRect(hWnd, &rectWnd); 
 
            //得到窗口的DIB
            hDIB = CopyScreenToDIB(&rectWnd); 
            break; 
        } 
        // 拷贝客户区
        case PW_CLIENT:
        { 
            RECT    rectClient; 
            POINT   pt1, pt2; 
 
            // 得到客户区面积
             GetClientRect(hWnd, &rectClient); 
 
            pt1.x = rectClient.left; 
            pt1.y = rectClient.top; 
            pt2.x = rectClient.right; 
            pt2.y = rectClient.bottom; 
            ClientToScreen(hWnd, &pt1); 
            ClientToScreen(hWnd, &pt2); 
            rectClient.left = pt1.x; 
            rectClient.top = pt1.y; 
            rectClient.right = pt2.x; 
            rectClient.bottom = pt2.y; 
 
            // 从客户区得到DIB 
            hDIB = CopyScreenToDIB(&rectClient); 
            break; 
        } 
       
        default:   
            return NULL; 
    } 
 
  
   return hDIB; 
} 
 
/************************************************************************* 
 * 
 * 函数名:CopyClientRectToDIB() 
 * 
 * 参数:
 * 
 * HWND hWnd        - 指定的窗口句柄 
 * 
 * LPRECT lpRect    - 指定客户区的区域
 * 
 * 返回值: 
 * 
 * HDIB             - 依赖设备的bitmap句柄 
 * 
 * 描述:
 * 
 *  拷贝客户区的指定部份到bitmap
 * 
 ************************************************************************/ 
HDIB CopyClientRectToDIB(HWND hWnd, LPRECT lpRect) 
{ 
   HDIB     hDIB = NULL;  // handle to DIB 
 
    if (!hWnd) 
        return NULL; 
 
    POINT   pt1, pt2; 

    pt1.x = lpRect->left; 
    pt1.y = lpRect->top; 
    pt2.x = lpRect->right; 
    pt2.y = lpRect->bottom; 
    ClientToScreen(hWnd, &pt1); 
    ClientToScreen(hWnd, &pt2); 


	// 如果超出窗口矩形范围，则剪切掉
	CRect rcWnd;
	GetWindowRect(hWnd, &rcWnd);
	pt1.x = pt1.x >= rcWnd.left ? pt1.x : rcWnd.left;
	pt1.x = pt1.x >= rcWnd.right ? rcWnd.right : pt1.x;
	pt1.y = pt1.y >= rcWnd.top ? pt1.y : rcWnd.top;
	pt1.y = pt1.y >= rcWnd.bottom ? rcWnd.bottom : pt1.y;
	pt2.x = pt2.x >= rcWnd.left ? pt2.x : rcWnd.left;
	pt2.x = pt2.x >= rcWnd.right ? rcWnd.right : pt2.x;
	pt2.y = pt2.y >= rcWnd.top ? pt2.y : rcWnd.top;
	pt2.y = pt2.y >= rcWnd.bottom ? rcWnd.bottom : pt2.y;


    lpRect->left = pt1.x; 
    lpRect->top = pt1.y; 
    lpRect->right = pt2.x; 
    lpRect->bottom = pt2.y; 

    //得到客户区DIB
    hDIB = CopyScreenToDIB(lpRect); 
 
	//保存区域
    pt1.x = lpRect->left; 
    pt1.y = lpRect->top; 
    pt2.x = lpRect->right; 
    pt2.y = lpRect->bottom; 
    ScreenToClient(hWnd, &pt1); 
    ScreenToClient(hWnd, &pt2); 
    lpRect->left = pt1.x; 
    lpRect->top = pt1.y; 
    lpRect->right = pt2.x; 
    lpRect->bottom = pt2.y; 
 
   return hDIB; 
} 

 
/************************************************************************* 
 * 
 * 函数名:PaintDIB() 
 * 
 * 参数:
 * 
 * HDC hDC          - 输出的HDC句柄
 * 
 * LPRECT lpDCRect  - DC输出矩形
 * 
 * HDIB hDIB        - 内存HDIB句柄
 * 
 * LPRECT lpDIBRect - DIB矩形到输出lpDCRect 
 *
 * HPALETTE hPal    - 显示DIB要使用的调色板
 *
 * DWORD dwRop      - 显示DIB用ROP模式
 * 
 * 返回值: 
 * 
 * BOOL             - 返回TRUE表示DIB被正确画 
 * 
 * 描述: 
 *  在相关DC上画DIB.  
 * 
 * 
 ************************************************************************/ 
BOOL PaintDIB(HDC hDC, LPRECT lpDCRect, HDIB hDIB, LPRECT lpDIBRect, 
        HPALETTE hPal, DWORD dwRop)
{ 
   //BITMAPINFOHEADER 结构指针
    LPBYTE        lpDIBHdr;          
    //DIB数据位指针
    LPBYTE        lpDIBBits;      
    // 成功标记
    BOOL         bSuccess=FALSE;   
    // 旧调色板
    HPALETTE     hOldPal=NULL;   
 
    // 检查DIB句柄
    if (!hDIB) 
        return FALSE; 
    lpDIBHdr  = (LPBYTE)GlobalLock(hDIB); 
    lpDIBBits = FindDIBBits(lpDIBHdr); 
 
	// 若无调色板, 从DIB中创建一个
    if (! hPal) 
		hPal = CreateDIBPalette(lpDIBHdr);

    // 选取调色板 
    if (hPal) 
    { 
        hOldPal = SelectPalette(hDC, hPal, TRUE); 
        RealizePalette(hDC); 
    } 
  
    SetStretchBltMode(hDC, COLORONCOLOR); 
 
	bSuccess = StretchDIBits(hDC, 
							 lpDCRect->left, 
							 lpDCRect->top, 
							 RECTWIDTH(lpDCRect), 
							 RECTHEIGHT(lpDCRect), 
							 lpDIBRect->left, 
				             (int)DIBHeight(lpDIBHdr) - lpDIBRect->top - RECTHEIGHT(lpDIBRect), 
							 RECTWIDTH(lpDIBRect), 
							 RECTHEIGHT(lpDIBRect), 
							 lpDIBBits, 
							 (LPBITMAPINFO)lpDIBHdr, 
							 DIB_RGB_COLORS, 
							 SRCCOPY); 
 

    GlobalUnlock(hDIB); 
 
    if (hOldPal) 
		SelectPalette(hDC, hOldPal, FALSE); 
    return bSuccess; 
} 




/************************************************************************* 
 * 
 * 函数名:PaintBitmap() 
 * 
 * 参数:
 * 
 * HDC hDC          - 输出的HDC句柄
 * 
 * LPRECT lpDCRect  - DC输出矩形
 * 
 * HBITMAP hDDB     - 依赖设备的bitmap (DDB) 
 * 
* LPRECT lpDIBRect - DDB矩形到输出lpDCRect 
 *
 * HPALETTE hPal    - 显示DDB要使用的调色板
 * 
 * 返回值:  
 * 
 * BOOL             - 返回TRUE表示DDB被正确画 
 * 
 * 描述:
 * 
 *在相关DC上画DDB.  
 * 
 ************************************************************************/ 
BOOL PaintBitmap(HDC hDC, LPRECT lpDCRect, HBITMAP hDDB,  LPRECT lpDDBRect,  
        HPALETTE hPal, DWORD dwRop)
{ 
    // 内存DC 
    HDC         hMemDC;           
    //先前的bitmap 
    HBITMAP     hOldBitmap;       
   //先前的调色板1 
    HPALETTE    hOldPal1 = NULL;   
   // 先前的调色板2
    HPALETTE    hOldPal2 = NULL; 

    BOOL        bSuccess = FALSE;  
 
    //创建内存DC 
    hMemDC = CreateCompatibleDC (hDC); 
 
    if (!hMemDC) 
        return FALSE; 
 
    // 若存在调色板,选取并初始化它 
     if (hPal) 
    { 
        hOldPal1 = SelectPalette(hMemDC, hPal, TRUE); 
        hOldPal2 = SelectPalette(hDC, hPal, TRUE); 
        RealizePalette(hDC); 
    } 
 
    // 选位图到内存DC中
     hOldBitmap = (HBITMAP)SelectObject (hMemDC, hDDB); 
 
    SetStretchBltMode (hDC, COLORONCOLOR); 
    if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDDBRect)) && 
            (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDDBRect))) 
        bSuccess = BitBlt(hDC, lpDCRect->left, lpDCRect->top, 
                lpDCRect->right - lpDCRect->left, 
                lpDCRect->bottom - lpDCRect->top, hMemDC, lpDDBRect->left, 
                lpDDBRect->top, dwRop); 
    else 
        bSuccess = StretchBlt(hDC, lpDCRect->left,  lpDCRect->top,  
                lpDCRect->right - lpDCRect->left, 
                lpDCRect->bottom - lpDCRect->top, hMemDC, lpDDBRect->left,  
                lpDDBRect->top,  lpDDBRect->right - lpDDBRect->left, 
                lpDDBRect->bottom - lpDDBRect->top, dwRop); 
 
 
    SelectObject(hMemDC, hOldBitmap); 
 
    if (hOldPal1) 
        SelectPalette (hMemDC, hOldPal1, FALSE); 
 
    if (hOldPal2) 
        SelectPalette (hDC, hOldPal2, FALSE); 
 
    DeleteDC (hMemDC); 
  
    return bSuccess; 
} 

/************************************************************************* 
 * 
 * 函数名:DisplayPalette () 
 * 
 * 参数: 
 * 
 * HDC hDC          - 显示调色板的设备上下文
 * LPRECT lpRect      -显示调色板的区域
 * HPALETTE hPal     - 要显示的调色板句柄
 * 
 * 返回值: 
 * 
 * BOOL            - 返回TRUE表示成功，返回 FALSE表示失败
 * 
 * 描述: 
 * 
 * 显示调色板在设备上下文中指定的区域 
 * 
 ************************************************************************/ 
BOOL DisplayPalette(HDC hDC, LPRECT lpRect, HPALETTE hPal)
{
	if (! hPal)
		return FALSE;

	int nEntries;
	PALETTEENTRY pe[256];
	nEntries = GetPaletteEntries(hPal, 0, 256, pe);
	int nSqr = (int)sqrt((double)nEntries);

	int nWidth = (lpRect->right-lpRect->left)/nSqr;
	int nHeight = (lpRect->bottom-lpRect->top)/nSqr;
	lpRect->right = lpRect->left + nWidth*nSqr;
	lpRect->bottom = lpRect->top + nHeight*nSqr;

	HPALETTE hOldPal = (HPALETTE)SelectPalette(hDC, hPal, FALSE);
	RealizePalette(hDC);

	HBRUSH hBrush, hOldBrush;
	int x, y;
	for (int i=0; i<nEntries; ++i)
	{
		x = i%nSqr;
		y = i/nSqr;
		hBrush = CreateSolidBrush(RGB(pe[i].peRed, pe[i].peGreen, pe[i].peBlue));
		hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Rectangle(hDC, 
			      lpRect->left + x*nWidth, 
				  lpRect->top + y*nHeight, 
				  lpRect->left + (x+1)*nWidth, 
				  lpRect->top + (y+1) *nHeight);
		SelectObject(hDC, hOldBrush);
		DeleteObject(hBrush);
	}
	
	SelectPalette(hDC, hOldPal, FALSE);

	return TRUE;
}

/************************************************************************* 
 * 
 * 函数名:CopyPalette () 
 * 
 * 参数:
 * 
 * HPALETTE hPalSrc    - 资源调色板句柄
 * 
 *返回值: 
 * 
 * HPALETTE          - 目的调色板句柄
 * 
 * 描述:
 * 
 * 拷贝调色板到另一调色板
 * 
 ************************************************************************/ 
HPALETTE CopyPalette(HPALETTE hPalSrc)
{
    PLOGPALETTE     plogPal;
    int             iNumEntries=0;
    HPALETTE        hPal;
	HANDLE			h;

	iNumEntries = GetPaletteEntries(hPalSrc, 0, iNumEntries, NULL);
    if (iNumEntries == 0) 
        return (HPALETTE) NULL;

	h = GlobalAlloc(GHND, sizeof(DWORD) + sizeof(PALETTEENTRY)*iNumEntries);
	if (! h)
        return (HPALETTE) NULL;
	plogPal = (PLOGPALETTE)GlobalLock(h);
    if (! plogPal) 
        return (HPALETTE) NULL;

    plogPal->palVersion = 0x300;
    plogPal->palNumEntries = (WORD) iNumEntries;
    GetPaletteEntries(hPalSrc, 0, iNumEntries, plogPal->palPalEntry);
    hPal = CreatePalette(plogPal);

    GlobalUnlock(h);
    GlobalFree(h);

    return hPal;
}
 

/************************************************************************* 
 * 
 * 函数名:PalEntriesOnDevice() 
 * 
 * 参数:
 * 
 * HDC hDC          - 设备上下文
 * 
 * 返回值: 
 * 
 * int              - 设备调色板的数量
 * 
 * 描述:
 * 
 * 得到系统设备调色板的数量
 * 
 ************************************************************************/ 
int PalEntriesOnDevice(HDC hDC) 
{ 
	//颜色数
    int nColors; 
 
    // 从设备中发现颜色数     
    nColors = (1 << (GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES))); 
 
    assert(nColors); 
    return nColors; 
} 
 
 
/************************************************************************* 
 * 
 * 函数名:GetSystemPalette() 
 * 
 * 参数: 
 * 
 * 
 * 返回值:  
 * 
 * HPALETTE         - 要复制的当前系统调色板 
 * 
 * 描述:
 * 
 *得到当前系统调色板
 * 
 ************************************************************************/ 
HPALETTE GetSystemPalette(void) 
{ 
    HDC hDC;                
    static HPALETTE hPal = NULL;   
    HANDLE hLogPal;        
    LPLOGPALETTE lpLogPal;  
    int nColors;            

     hDC = GetDC(NULL); 
 
    if (!hDC) 
        return NULL; 
    // 调色板数量
    nColors = PalEntriesOnDevice(hDC);   
 
    //分配内存给调色板并锁定它
     hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + nColors * 
            sizeof(PALETTEENTRY)); 
 
    if (!hLogPal) 
        return NULL; 
 
    //得到逻辑调色板的指针 
    lpLogPal = (LPLOGPALETTE)GlobalLock(hLogPal); 
 
     lpLogPal->palVersion = PALVERSION; 
    lpLogPal->palNumEntries = nColors; 
 
    // 拷贝系统调色板到逻辑调色板中
     GetSystemPaletteEntries(hDC, 0, nColors, 
            (LPPALETTEENTRY)(lpLogPal->palPalEntry)); 
 
    hPal = CreatePalette(lpLogPal); 
 
    GlobalUnlock(hLogPal); 
    GlobalFree(hLogPal); 
    ReleaseDC(NULL, hDC); 
 
    return hPal; 
} 
 
/************************************************************************* 
 * 
 * 函数名:CreateIdentifyPalette () 
 * 
 * 参数: 
 * 
 * HPALETTE hPalSrc    - 资源调色板句柄
 * 
 * 返回值: 
 * 
 * HPALETTE          - 目的调色板句柄 
 * 
 * 描述: 
 * 
 * 从资源调色板中创建一个目的调色板 
 * 
 ************************************************************************/ 
HPALETTE CreateIdentifyPalette(HPALETTE hPalSrc)
{
    BOOL bResult = FALSE;
    int i, iSysColors, iPalEntries;
    HPALETTE hPalette, hpalOld;

	if (! hPalSrc)
		return NULL;

	//创建一个新调色板
	hPalette = CopyPalette(hPalSrc);

    //得到屏幕DC
    HDC hdcScreen = GetDC(NULL);
    ASSERT(hdcScreen);

    if (!(GetDeviceCaps(hdcScreen, RASTERCAPS) & RC_PALETTE)) 
	{
        TRACE("Not a palettized device");
        goto abort;
    }

    // 得到系统颜色数和调色板数
     iSysColors = GetDeviceCaps(hdcScreen, NUMCOLORS);
    iPalEntries = GetDeviceCaps(hdcScreen, SIZEPALETTE);

    //或超过256色
    if (iSysColors > 256) goto abort;
    SetSystemPaletteUse(hdcScreen, SYSPAL_NOSTATIC);
    SetSystemPaletteUse(hdcScreen, SYSPAL_STATIC);

    hpalOld = ::SelectPalette(hdcScreen,
                              hPalette, 
                              FALSE);
    ::RealizePalette(hdcScreen);
    // 代替旧的调色板
    ::SelectPalette(hdcScreen, hpalOld, FALSE);

    PALETTEENTRY pe[256];
    GetSystemPaletteEntries(hdcScreen, 
                            0,
                            iPalEntries,
                            pe);


    for (i = 0; i < iSysColors/2; i++) {
        pe[i].peFlags = 0;
    }
    for (; i < iPalEntries-iSysColors/2; i++) {
        pe[i].peFlags = PC_NOCOLLAPSE;
    }
    for (; i < iPalEntries; i++) {
        pe[i].peFlags = 0;
    }

    ResizePalette(hPalette, iPalEntries);

    // 更新调色板
    SetPaletteEntries(hPalette, 0, iPalEntries, pe);
    bResult = TRUE;

abort:
    ::ReleaseDC(NULL, hdcScreen);
    return bResult ? hPalette : NULL;
}

/************************************************************************* 
 * 
 * 函数名:MapDIBColorsToPalette () 
 * 
 * 参数: 
 * 
 * HDIB hDIB		    - DIB 句柄
 * HPALETTE hPalette    - 调色板句柄
 * 
 * 返回值: 
 * 
 * HPALETTE          - 目的调色板句柄
 * 
 * 描述: 
 * 
 * 用给定的调色板画颜色 
 * 
 ************************************************************************/ 
BOOL MapDIBColorsToPalette(HDIB hDIB, HPALETTE hPalette)
{
    if (! hDIB)
        return FALSE;
    if (! hPalette)
        return FALSE;

    LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
    if (! lpbi)
        return FALSE;
    LPRGBQUAD pctThis = (LPRGBQUAD)((LPBYTE)lpbi + lpbi->biSize);

    // 创建一个索引转换表到DIB颜色
    BYTE imap[256];
    for (int i = 0; i < 256; i++) 
	{
        imap[i] = (BYTE)GetNearestPaletteIndex(hPalette, 
									RGB(pctThis->rgbRed,
										pctThis->rgbGreen,
										pctThis->rgbBlue));
        pctThis++;
    }
    // 无DIB位
    LPBYTE pBits = (LPBYTE)lpbi + lpbi->biSize + PaletteSize((LPBYTE)lpbi);
    int iSize = WIDTHBYTES1(lpbi->biBitCount*lpbi->biWidth) * lpbi->biHeight;
    while (iSize--) 
	{
        *pBits = imap[*pBits];
        pBits++;
    }

    //再协调DIB颜色表
    PALETTEENTRY pe[256];
    GetPaletteEntries(hPalette, 0, 256, pe);
    pctThis = (LPRGBQUAD)((LPBYTE)lpbi + lpbi->biSize);
    for (int i = 0; i < 256; i++) 
	{
        pctThis->rgbRed = pe[i].peRed;    
        pctThis->rgbGreen = pe[i].peGreen;    
        pctThis->rgbBlue = pe[i].peBlue;
        pctThis++;    
    }
	
	GlobalUnlock(hDIB);
    return TRUE;
}

/************************************************************************* 
 * 
 * 函数名:DrawTransparentBitmap () 
 * 
 * 参数: 
 * 
 * HDC hDC          - 输出用CDC句柄
 * 
 * HBITMAP hBitmap    - 画透明位图的句柄
 * 
 * LONG xStart, yStart   - 开始画点的坐标
 *
 * COLORREF cTransparentColor - 透明颜色的RGB值 
 * 返回值: 
 * 
 * 描述: 
 *		 画一透明位图
 * 
 ************************************************************************/ 
void DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, 
						   LONG xStart, LONG yStart, 
						   COLORREF cTransparentColor)
{
   BITMAP     bm;
   COLORREF   cColor;
   HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
   HBITMAP    bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
   HDC        hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
   POINT      ptSize;

   hdcTemp = CreateCompatibleDC(hdc);
  // 选入bitmap
   SelectObject(hdcTemp, hBitmap); 

   GetObject(hBitmap, sizeof(BITMAP), (LPBYTE)&bm);
   ptSize.x = bm.bmWidth;            
   ptSize.y = bm.bmHeight;           
   DPtoLP(hdcTemp, &ptSize, 1);     
                                    
   // 创建一些 DCs到临时数据.
   hdcBack   = CreateCompatibleDC(hdc);
   hdcObject = CreateCompatibleDC(hdc);
   hdcMem    = CreateCompatibleDC(hdc);
   hdcSave   = CreateCompatibleDC(hdc);

   // 为每一个DC创建bitmap， DCs是必须的为GDI操作
   // 单色 DC
   bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
   bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

   // 每个DC必须选取一个bitmap对象到保存的pixel数据
   bmBackOld   = (HBITMAP)SelectObject(hdcBack, bmAndBack);
   bmObjectOld = (HBITMAP)SelectObject(hdcObject, bmAndObject);
   bmMemOld    = (HBITMAP)SelectObject(hdcMem, bmAndMem);
   bmSaveOld   = (HBITMAP)SelectObject(hdcSave, bmSave);

   //设置映射模式
   SetMapMode(hdcTemp, GetMapMode(hdc));

   //保存位图.
   BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

   // 设置背景色
   cColor = SetBkColor(hdcTemp, cTransparentColor);
   BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,
          SRCCOPY);
   SetBkColor(hdcTemp, cColor);

   //创建反转对象
   BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0,
          NOTSRCCOPY);

   //拷贝主DC的背景到目的区.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart,
          SRCCOPY);

   //面具位图将被代替
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);

   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);

   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);

   //拷贝目的位图到屏幕
   BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,
          SRCCOPY);

   //代替起源位图
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

   //删除内存DC
   DeleteObject(SelectObject(hdcBack, bmBackOld));
   DeleteObject(SelectObject(hdcObject, bmObjectOld));
   DeleteObject(SelectObject(hdcMem, bmMemOld));
   DeleteObject(SelectObject(hdcSave, bmSaveOld));

   //删除内存 DCs.
   DeleteDC(hdcMem);
   DeleteDC(hdcBack);
   DeleteDC(hdcObject);
   DeleteDC(hdcSave);
   DeleteDC(hdcTemp);
}

/************************************************************************* 
 * 
 * 函数名:Fade() 
 * 
 * 参数: 
 * 
 * int nDeltaR   -  红色的变化值
 * int nDeltaG   -  绿色的变化值
 * int nDeltaB   -  蓝色的变化值
 * BYTE rm		 -  红色的最大值
 * BYTE gm		 -  绿色的最大值
 * BYTE bm		 -  蓝色的最大值
 * BYTE *r		 -  红色值变化后的值指针
 * BYTE *g		 -  绿色值变化后的值指针
 * BYTE *b		 -  蓝色值变化后的值指针
 * 
 * 返回值: 
 *
 * BOOL          -  返回TRUE表示成功，返回FALSE表示失败
 * 
 * 描述: 
 * 
 * 变化 R, G, B的值
 * 
 ************************************************************************/
BOOL Fade(int nDeltaR, int nDeltaG, int nDeltaB, 
		  BYTE rm, BYTE gm, BYTE bm,
		  BYTE *r, BYTE *g, BYTE *b)
{
	int R = *r + nDeltaR;
	int G = *g + nDeltaG;
	int B = *b + nDeltaB;

	if (nDeltaR < 0)
		*r = (BYTE)BOUND(R, rm, 255);
	else
		*r = (BYTE)BOUND(R, 0, rm);
	if (nDeltaG < 0)
		*g = (BYTE)BOUND(G, gm, 255);
	else
		*g = (BYTE)BOUND(G, 0, gm);
	if (nDeltaB < 0)
		*b = (BYTE)BOUND(B, bm, 255);
	else
		*b = (BYTE)BOUND(B, 0, bm);

	if (*r==rm && *g==gm && *b==bm)
		return TRUE;
	else
		return FALSE;
}


/************************************************************************* 
 * 
 * 函数名:Delay() 
 * 
 * 参数: 
 * 
 * DWORD dwDelayTime   - 延迟的毫秒
 * 
 * 返回值: none
 * 
 * 描述: 
 * 
 * 用于延迟指定的毫秒以进行下一个指令操作 
 * 本函数有问题暂不用
 ************************************************************************/ 
void Delay(DWORD dwDelayTime)
{
  // DWORD dwTimeBegin, dwTimeEnd;
  //  dwTimeBegin = timeGetTime();
  //  do
//	{
  //      dwTimeEnd = timeGetTime();
//	} while (dwTimeEnd - dwTimeBegin < dwDelayTime);
}

/************************************************************************* 
 * 
 * 函数名:CopyHandle() 
 * 
 * 参数: 
 * 
 * HANDLE h         - 资源句柄
 * 
 * 返回值: 
 * 
 * HANDLE           - 目标句柄
 * 
 * 描述: 
 * 
 * 拷贝句柄到另一个句柄
 ************************************************************************/ 
HANDLE CopyHandle(HANDLE h)
{
	if (h == NULL)
		return NULL;

	DWORD  dwLen = ::GlobalSize((HGLOBAL)h);
	HANDLE hCopy = ::GlobalAlloc(GHND, dwLen);
	if (hCopy == NULL)
		return NULL;

	void* lpCopy = ::GlobalLock((HGLOBAL) hCopy);
	void* lp     = ::GlobalLock((HGLOBAL) h);
	::CopyMemory(lpCopy, lp, dwLen);
	::GlobalUnlock(hCopy);
	::GlobalUnlock(h);

	return hCopy;
}

/************************************************************************* 
 * 
 * 函数名:CropBitmap() 
 * 
 * 参数: 
 * 
 * HBITMAP hBitmap - 被裁剪的DDB句柄
 * LPRECT lpRect   - 要裁剪的矩形区
 * 
 * 返回值: 
 * 
 * HDIB             - 新位图句柄
 * 
 * 描述: 
 * 
 * 裁剪指定的DDB矩形区, 返回一个已被裁剪后的DIB句柄
 * 
 ************************************************************************/ 
HBITMAP CropBitmap(HBITMAP hBitmap, LPRECT lpRect)
{
	WaitCursorBegin();

	//得到当前DC
	HDC hDC = GetDC(NULL);
	// create bitmap
	HBITMAP hNewBmp = CreateCompatibleBitmap(hDC, RECTWIDTH(lpRect), RECTHEIGHT(lpRect));
	if (hNewBmp == NULL)
	{
		ReleaseDC(NULL, hDC);
		WaitCursorEnd();
		return NULL;
	}
	//创建内存DC
	HDC hMemDC = CreateCompatibleDC(hDC);
	if (hMemDC == NULL)
	{
		ReleaseDC(NULL, hDC);
		DeleteObject(hNewBmp);
		WaitCursorEnd();
		return NULL;
	}
	HDC hSrcMemDC = CreateCompatibleDC(hDC);
	if (hSrcMemDC == NULL)
	{
		DeleteDC(hMemDC);
		ReleaseDC(NULL, hDC);
		DeleteObject(hNewBmp);
		WaitCursorEnd();
		return NULL;
	}

	//选取新位图到内存DC
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hNewBmp);
	//选取旧位图到屏幕DCSrcMemDC
	HBITMAP hSrcOldBitmap = (HBITMAP)SelectObject(hSrcMemDC, hBitmap);

	//在新位图中画旧位图
	if (! BitBlt(hMemDC, 
				 0, 
				 0,
				 RECTWIDTH(lpRect), 
				 RECTHEIGHT(lpRect),
				 hSrcMemDC,
				 lpRect->left,
				 lpRect->top,
				 SRCCOPY))
	{
		SelectObject(hMemDC, hOldBitmap);
		SelectObject(hSrcMemDC, hSrcOldBitmap);
		DeleteDC(hMemDC);
		DeleteDC(hSrcMemDC);
		ReleaseDC(NULL, hDC);
		DeleteObject(hNewBmp);
		WaitCursorEnd();
		return NULL;
	}
	
	//恢复
	SelectObject(hMemDC, hOldBitmap);
	SelectObject(hSrcMemDC, hSrcOldBitmap);
	DeleteDC(hMemDC);
	DeleteDC(hSrcMemDC);
	ReleaseDC(NULL, hDC);
	WaitCursorEnd();

	return hNewBmp;
}

/************************************************************************* 
 * 
 * 函数名:CropDIB() 
 * 
 * 参数: 
 * 
 * HDIB hDIB       - 要被裁剪的DIB句柄
 * LPRECT lpRect   - 指定要裁剪的区域
 * 
 * 返回值: 
 * 
 * HDIB             - 新DIB句柄 
 * 
 * 描述: 
 * 
 * 裁剪指定的DIB的指定区域并返回经裁剪后的DIB，源DIB保持不变
 * 
 ************************************************************************/ 
HDIB CropDIB(HDIB hDIB, LPRECT lpRect)
{
    LPBITMAPINFO lpbmi = NULL; 
    LPBYTE       lpSourceBits, lpTargetBits, lpResult; 
    HDC			 hDC = NULL, hSourceDC, hTargetDC; 
    HBITMAP      hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize, dwTargetBitsSize, dwTargetHeaderSize; 
	int			 nWidth, nHeight;
	HDIB		 hNewDIB;
	DWORD		 dwSize;

	WaitCursorBegin();

	//得到DIB指针
	if (! hDIB)
	{
		WaitCursorEnd();
		return NULL;
	}
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDIB);
	if (! lpSrcDIB)
	{
		WaitCursorEnd();
		return NULL;
	}

    // 分配和填充BITMAPINFO结构为新DIB 
    dwTargetHeaderSize = sizeof( BITMAPINFOHEADER ) + PaletteSize(lpSrcDIB); 
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize ); 
	memcpy(lpbmi, lpSrcDIB, dwTargetHeaderSize);
	nWidth = RECTWIDTH(lpRect);
	nHeight = RECTHEIGHT(lpRect);
    lpbmi->bmiHeader.biWidth = nWidth; 
    lpbmi->bmiHeader.biHeight = nHeight; 
 
	hDC = GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC ); 
 
    //运算资源DIB片段到匹配的资源DIB 
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPBYTE)lpSrcDIB), dwSourceBitsSize ); 
    lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize; 
 
    //选取资源DIB片段到DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap ); 
 
	//将旧位图放入新位图中 
    BitBlt(hTargetDC, 0, 0, nWidth, nHeight, hSourceDC, lpRect->left, lpRect->top, SRCCOPY); 

    SelectObject( hSourceDC, hOldSourceBitmap ); 
    SelectObject( hTargetDC, hOldTargetBitmap ); 
    DeleteDC( hSourceDC ); 
    DeleteDC( hTargetDC ); 
    ReleaseDC( NULL, hDC ); 

    GdiFlush(); 
 
    // 为新DIB分配足够内存 
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND, dwSize);
    lpResult = (LPBYTE)GlobalLock(hNewDIB); 
    memcpy( lpResult, lpbmi, dwTargetHeaderSize ); 
    memcpy( FindDIBBits( (LPBYTE)lpResult ), lpTargetBits, dwTargetBitsSize ); 
 
    DeleteObject( hTargetBitmap ); 
    DeleteObject( hSourceBitmap ); 
    free( lpbmi ); 
	GlobalUnlock(hDIB);
	GlobalUnlock(hNewDIB);
	WaitCursorEnd();
 
    return hNewDIB;
}

/************************************************************************* 
 * 
 * 函数名:CutDIB() 
 * 
 * 参数: 
 * 
 * HDIB hDIB       - 要被剪切的DIB句柄
 * LPRECT lpRect   - 指定剪裁的区域
 * 
 * 返回值: 
 * 
 * HDIB             - 新DIB句柄
 * 
 * 描述: 
 * 剪切DIB指定的矩形区域，源DIB不变
 * 
 * 
 ************************************************************************/ 
HDIB CutDIB(HDIB hDIB, LPRECT lpRect)
{
    LPBITMAPINFO lpbmi = NULL; 
    LPBYTE       lpTargetBits; 
    HDC			 hDC = NULL, hTargetDC; 
    HBITMAP      hTargetBitmap, hOldTargetBitmap; 
    DWORD        dwTargetBitsSize; 
	HDIB		 hNewDIB;

	WaitCursorBegin();

	// 得到新DIB句柄
	if (! hDIB)
	{
		WaitCursorEnd();
		return NULL;
	}
	hNewDIB = CopyHandle(hDIB);
	if (! hNewDIB)
	{
		WaitCursorEnd();
		return NULL;
	}

	hDC = GetDC( NULL ); 
    lpbmi = (LPBITMAPINFO)GlobalLock(hNewDIB); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hTargetDC = CreateCompatibleDC( hDC ); 
 
    //运算资源DIB片段到匹配的资源DIB 
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 
    memcpy( lpTargetBits, FindDIBBits((LPBYTE)lpbmi), dwTargetBitsSize ); 

    //选取资源DIB片段到 DCs 
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap ); 
 
	//清除指定区域
	HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255,255,255));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hTargetDC, hBrush);
	HPEN hOldPen = (HPEN)SelectObject(hTargetDC, hPen);
	Rectangle(hTargetDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
	SelectObject(hTargetDC, hOldBrush);
	SelectObject(hTargetDC, hOldPen);

    SelectObject( hTargetDC, hOldTargetBitmap ); 
    DeleteDC( hTargetDC ); 
    ReleaseDC( NULL, hDC ); 
 
    GdiFlush(); 
 
    //复制位
    memcpy( FindDIBBits( (LPBYTE)lpbmi ), lpTargetBits, dwTargetBitsSize ); 

    DeleteObject( hTargetBitmap ); 
	GlobalUnlock(hNewDIB);
	WaitCursorEnd();
 
    return hNewDIB;
}

/************************************************************************* 
 * 
 * 函数名:MergeDIB() 
 * 
 * 参数: 
 * 
 * HDIB hDIB1       - 合并目标的DIB句柄
 * HDIB hDIB2       - 要合并的DIB句柄
 * POINT ptTopLeft  - 在hDIB1中开始合并的位置(左上角坐标)
 * 
 * 返回值: 
 * 
 * HDIB             - 合并后新DIB的句柄 
 * 
 * 描述: 
 * 
 * 合并第二个DIB到第一个DIB中形成一个新的DIB，源DIB不变
 * 
 ************************************************************************/ 
HDIB MergeDIB(HDIB hDIB1, HDIB hDIB2, POINT ptTopLeft)
{
    LPBITMAPINFO lpbmi = NULL; 
    LPBYTE       lpSourceBits, lpTargetBits; 
    HDC			 hDC = NULL, hSourceDC, hTargetDC; 
    HBITMAP      hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap; 
    DWORD        dwTargetBitsSize, dwSourceBitsSize; 
	HDIB		 hNewDIB;
	int			 nWidth, nHeight;

	WaitCursorBegin();

	//得到DIB指针
	if (! hDIB1 || ! hDIB2)
	{
		WaitCursorEnd();
		return NULL;
	}

	//新DIB
	hNewDIB = CopyHandle(hDIB1);
	if (! hNewDIB)
	{
		WaitCursorEnd();
		return NULL;
	}
    //新DIB指针
    lpbmi = (LPBITMAPINFO)GlobalLock(hNewDIB);
	if (! lpbmi)
	{
		WaitCursorBegin();
		return NULL;
	}
	// DIB2 缓冲区
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDIB2);
	
	if (! lpSrcDIB)
	{
		GlobalUnlock(hNewDIB);
		DestroyDIB(hNewDIB);
		WaitCursorBegin();
		return NULL;
	}
	nWidth = DIBWidth(lpSrcDIB);
	nHeight = DIBHeight(lpSrcDIB);

	hDC = GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC ); 
 
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPBYTE)lpSrcDIB), dwSourceBitsSize ); 
    memcpy( lpTargetBits, FindDIBBits((LPBYTE)lpbmi), dwTargetBitsSize ); 

    // 选取资源DIB片段到 DCs  
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap ); 
 
	// 合并
    BitBlt(hTargetDC, ptTopLeft.x, ptTopLeft.y, nWidth, nHeight, hSourceDC, 0, 0, SRCCOPY); 
 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    SelectObject( hTargetDC, hOldTargetBitmap ); 
    DeleteDC( hSourceDC ); 
    DeleteDC( hTargetDC ); 
    ReleaseDC( NULL, hDC ); 
 
 
    GdiFlush(); 
 
    // 拷贝位
    memcpy( FindDIBBits((LPBYTE)lpbmi), lpTargetBits, dwTargetBitsSize ); 
 
    DeleteObject( hTargetBitmap ); 
    DeleteObject( hSourceBitmap ); 
	GlobalUnlock(hDIB2);
	GlobalUnlock(hNewDIB);
	WaitCursorEnd();
 
    return hNewDIB;
}


/************************************************************************* 
 * 
 * 函数名:RotateBitmapNT() 
 * 
 * 参数: 
 * 
 * hBitmap		- 要旋转的Bitmap
 * fDegrees		- 旋转的角度
 * clrBack		- 定义的颜色在旋转时不覆盖资源DIB
 * 
 * 返回值: 
 * 
 * HBITMAP            - 返回新的已经旋转的bitmap句柄
 * 
 * 描述: 
 * 
 * 旋转位图
 * 
 ************************************************************************/ 
HBITMAP RotateBitmapNT(HBITMAP hBitmap, double fDegrees, COLORREF clrBack)
{
	//创建内存DC用于显示 
	CDC sourceDC, destDC;
	sourceDC.CreateCompatibleDC( NULL );
	destDC.CreateCompatibleDC( NULL );

	//得到逻辑相配值
	BITMAP bm;
	::GetObject( hBitmap, sizeof( bm ), &bm );
	
	// 转换角度为弧度
	#define PI		3.1415926535798932
	double radians = (fDegrees/90.0)*(PI/2);

	float cosine = (float)cos(radians);
	float sine = (float)sin(radians);

	//计算旋转后的位图尺寸
	int x1 = (int)(bm.bmHeight * sine);
	int y1 = (int)(bm.bmHeight * cosine);
	int x2 = (int)(bm.bmWidth * cosine + bm.bmHeight * sine);
	int y2 = (int)(bm.bmHeight * cosine - bm.bmWidth * sine);
	int x3 = (int)(bm.bmWidth * cosine);
	int y3 = (int)(-bm.bmWidth * sine);

	int minx = min(0,min(x1, min(x2,x3)));
	int miny = min(0,min(y1, min(y2,y3)));
	int maxx = max(0,max(x1, max(x2,x3)));
	int maxy = max(0,max(y1, max(y2,y3)));

	int w = maxx - minx;
	int h = maxy - miny;

	// 创建位图
	HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), w, h);

	HBITMAP hbmOldSource = (HBITMAP)::SelectObject( sourceDC.m_hDC, hBitmap );
	HBITMAP hbmOldDest = (HBITMAP)::SelectObject( destDC.m_hDC, hbmResult );

	//当变化映射模式时
	HBRUSH hbrBack = CreateSolidBrush( clrBack );
	HBRUSH hbrOld = (HBRUSH)::SelectObject( destDC.m_hDC, hbrBack );
	destDC.PatBlt( 0, 0, w, h, PATCOPY );
	::DeleteObject( ::SelectObject( destDC.m_hDC, hbrOld ) );

	SetGraphicsMode(destDC.m_hDC, GM_ADVANCED);
	XFORM xform;
	xform.eM11 = cosine;
	xform.eM12 = -sine;
	xform.eM21 = sine;
	xform.eM22 = cosine;
	xform.eDx = (float)-minx;
	xform.eDy = (float)-miny;

	SetWorldTransform( destDC.m_hDC, &xform );

	destDC.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &sourceDC, 0, 0, SRCCOPY );

	::SelectObject( sourceDC.m_hDC, hbmOldSource );
	::SelectObject( destDC.m_hDC, hbmOldDest );

	return hbmResult;
}

/************************************************************************* 
 * 
 * 函数名:RotateBitmap() 
 * 
 * 参数: 
 * 
 * hBitmap		- 要旋转的Bitmap
 * fDegrees		- 旋转的角度
 * clrBack		- 定义的颜色在旋转时不覆盖资源DIB
 * 
 * 返回值: 
 * 
 * HBITMAP      - 返回新的已经旋转的bitmap句柄
 * 
 * 描述: 
 * 
 * 旋转位图
 * 
 ************************************************************************/ 
HBITMAP RotateBitmap(HBITMAP hBitmap, double fDegrees, COLORREF clrBack)
{
	//创建一个内存DC用于显示
	CDC sourceDC, destDC;
	sourceDC.CreateCompatibleDC( NULL );
	destDC.CreateCompatibleDC( NULL );

	BITMAP bm;
	::GetObject( hBitmap, sizeof( bm ), &bm );

	double radians = (fDegrees/90.0)*(PI/2);

	float cosine = (float)cos(radians);
	float sine = (float)sin(radians);


	int x1 = (int)(-bm.bmHeight * sine);
	int y1 = (int)(bm.bmHeight * cosine);
	int x2 = (int)(bm.bmWidth * cosine - bm.bmHeight * sine);
	int y2 = (int)(bm.bmHeight * cosine + bm.bmWidth * sine);
	int x3 = (int)(bm.bmWidth * cosine);
	int y3 = (int)(bm.bmWidth * sine);

	int minx = min(0,min(x1, min(x2,x3)));
	int miny = min(0,min(y1, min(y2,y3)));
	int maxx = max(x1, max(x2,x3));
	int maxy = max(y1, max(y2,y3));

	int w = maxx - minx;
	int h = maxy - miny;

	HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), w, h);

	HBITMAP hbmOldSource = (HBITMAP)::SelectObject( sourceDC.m_hDC, hBitmap );
	HBITMAP hbmOldDest = (HBITMAP)::SelectObject( destDC.m_hDC, hbmResult );

	HBRUSH hbrBack = CreateSolidBrush( clrBack );
	HBRUSH hbrOld = (HBRUSH)::SelectObject( destDC.m_hDC, hbrBack );
	destDC.PatBlt( 0, 0, w, h, PATCOPY );
	::DeleteObject( ::SelectObject( destDC.m_hDC, hbrOld ) );

	sourceDC.SetMapMode(MM_ISOTROPIC);
	sourceDC.SetWindowExt(1,1);
	sourceDC.SetViewportExt(1,-1);
	sourceDC.SetViewportOrg(0, bm.bmHeight-1);

	destDC.SetMapMode(MM_ISOTROPIC);
	destDC.SetWindowExt(1,1);
	destDC.SetViewportExt(1,-1);
	destDC.SetWindowOrg(minx, maxy);

	for( int y = miny; y < maxy; y++ )
	{
		for( int x = minx; x < maxx; x++ )
		{
			int sourcex = (int)(x*cosine + y*sine);
			int sourcey = (int)(y*cosine - x*sine);
			if( sourcex >= 0 && sourcex < bm.bmWidth && sourcey >= 0 
					&& sourcey < bm.bmHeight )
				destDC.SetPixel(x,y,sourceDC.GetPixel(sourcex,sourcey));
		}
	}

	::SelectObject( sourceDC.m_hDC, hbmOldSource );
	::SelectObject( destDC.m_hDC, hbmOldDest );

	return hbmResult;
}


/************************************************************************* 
 * 
 * 函数名:RotateDIB() 
 * 
 * 参数: 
 * 
 * hDIB		     - 要旋转的DIB
 * fDegrees		- 旋转的角度
 * clrBack		- 定义的颜色在旋转时不覆盖资源DIB
 * 
 * 返回值: 
 * 
 * HDIB      - 返回新的已经旋转的DIB句柄
 * 
 * 描述: 
 * 
 * 旋转DIB位图
 * 
 ************************************************************************/ 
HDIB RotateDIB(HDIB hDIB, double fDegrees, COLORREF clrBack)
{
	WaitCursorBegin();

	//得到资源bitmap信息
	LPBITMAPINFO lpBmInfo = (LPBITMAPINFO)GlobalLock(hDIB);
	int bpp = lpBmInfo->bmiHeader.biBitCount;		
	
	int nColors = lpBmInfo->bmiHeader.biClrUsed ? lpBmInfo->bmiHeader.biClrUsed : 
					1 << bpp;
	int nWidth = lpBmInfo->bmiHeader.biWidth;
	int nHeight = lpBmInfo->bmiHeader.biHeight;
	int nRowBytes = ((((nWidth * bpp) + 31) & ~31) / 8);

	DWORD compression = lpBmInfo->bmiHeader.biCompression;
	if( nHeight < 0 || (compression!=BI_RGB))
	{
		GlobalUnlock(hDIB);
		WaitCursorEnd();
		return NULL;
	}

	LPBYTE lpDIBBits = FindDIBBits((LPBYTE)lpBmInfo);

	double radians = (fDegrees/90.0)*(PI/2);

	float cosine = (float)cos(radians);
	float sine = (float)sin(radians);

	int x1 = (int)(-nHeight * sine);
	int y1 = (int)(nHeight * cosine);
	int x2 = (int)(nWidth * cosine - nHeight * sine);
	int y2 = (int)(nHeight * cosine + nWidth * sine);
	int x3 = (int)(nWidth * cosine);
	int y3 = (int)(nWidth * sine);

	int minx = min(0,min(x1, min(x2,x3)));
	int miny = min(0,min(y1, min(y2,y3)));
	int maxx = max(x1, max(x2,x3));
	int maxy = max(y1, max(y2,y3));

	int w = maxx - minx;
	int h = maxy - miny;

	int nResultRowBytes = ((((w * bpp) + 31) & ~31) / 8);
	long len = nResultRowBytes * h;
	int nHeaderSize = ((LPBYTE)lpDIBBits-(LPBYTE)lpBmInfo) ;
	HANDLE hDIBResult = GlobalAlloc(GHND,len+nHeaderSize);
	//初始化头信息
	LPBITMAPINFO lpBmInfoResult = (LPBITMAPINFO)GlobalLock(hDIBResult);
	memcpy( (void*)lpBmInfoResult, (void*)lpBmInfo, nHeaderSize);
	lpBmInfoResult->bmiHeader.biWidth = w;
	lpBmInfoResult->bmiHeader.biHeight = h;
	lpBmInfoResult->bmiHeader.biSizeImage = len;

	LPBYTE lpDIBBitsResult = FindDIBBits((LPBYTE)lpBmInfoResult);

	// 得到背景色值(索引号)
	ZeroMemory( lpDIBBitsResult, len );
	DWORD dwBackColor;
	switch(bpp)
	{
	//单色
	case 1:	
		if( clrBack == RGB(255,255,255) )
			memset( lpDIBBitsResult, 0xff, len );
		break;
    //查找颜色表
	case 4:
	case 8:	
		int i;
		for(i = 0; i < nColors; i++ )
		{
			if( lpBmInfo->bmiColors[i].rgbBlue ==  GetBValue(clrBack)
				&& lpBmInfo->bmiColors[i].rgbGreen ==  GetGValue(clrBack)
				&& lpBmInfo->bmiColors[i].rgbRed ==  GetRValue(clrBack) )
			{
				if(bpp==4) i = i | i<<4;
				memset( lpDIBBitsResult, i, len );
				break;
			}
		}
		// 若没有发现设为黑色
		break;
	case 16:
		if( *((DWORD*)lpBmInfo->bmiColors) == 0x7c00 )
		{
			dwBackColor = ((GetRValue(clrBack)>>3) << 10) + 
					((GetRValue(clrBack)>>3) << 5) +
					(GetBValue(clrBack)>>3) ;
		}
		else
		{
			dwBackColor = ((GetRValue(clrBack)>>3) << 11) + 
					((GetRValue(clrBack)>>2) << 5) +
					(GetBValue(clrBack)>>3) ;
		}
		break;
	case 24:
	case 32:
		dwBackColor = (((DWORD)GetRValue(clrBack)) << 16) | 
				(((DWORD)GetGValue(clrBack)) << 8) |
				(((DWORD)GetBValue(clrBack)));
		break;
	}

	for( int y = 0; y < h; y++ )
	{
		for( int x = 0; x < w; x++ )
		{
			int sourcex = (int)((x+minx)*cosine + (y+miny)*sine);
			int sourcey = (int)((y+miny)*cosine - (x+minx)*sine);
			if( sourcex >= 0 && sourcex < nWidth && sourcey >= 0 
				&& sourcey < nHeight )
			{
				//设置目标DIB像素
				switch(bpp)
				{
					BYTE mask;
				case 1:	
					mask = *((LPBYTE)lpDIBBits + nRowBytes*sourcey + 
						sourcex/8) & (0x80 >> sourcex%8);
					//调整目标掩饰位图
					mask = mask ? (0x80 >> x%8) : 0;
					*((LPBYTE)lpDIBBitsResult + nResultRowBytes*(y) + 
								(x/8)) &= ~(0x80 >> x%8);
					*((LPBYTE)lpDIBBitsResult + nResultRowBytes*(y) + 
								(x/8)) |= mask;
					break;
				case 4:
					mask = *((LPBYTE)lpDIBBits + nRowBytes*sourcey + 
						sourcex/2) & ((sourcex&1) ? 0x0f : 0xf0);
						//调整目标掩饰位图
					if( (sourcex&1) != (x&1) )
						mask = (mask&0xf0) ? (mask>>4) : (mask<<4);
					*((LPBYTE)lpDIBBitsResult + nResultRowBytes*(y) + 
							(x/2)) &= ~((x&1) ? 0x0f : 0xf0);
					*((LPBYTE)lpDIBBitsResult + nResultRowBytes*(y) + 
							(x/2)) |= mask;
					break;
				case 8:
					BYTE pixel ;
					pixel = *((LPBYTE)lpDIBBits + nRowBytes*sourcey + 
							sourcex);
					*((LPBYTE)lpDIBBitsResult + nResultRowBytes*(y) + 
							(x)) = pixel;
					break;
				case 16:
					DWORD dwPixel;
					dwPixel = *((LPWORD)((LPBYTE)lpDIBBits + 
							nRowBytes*sourcey + sourcex*2));
					*((LPWORD)((LPBYTE)lpDIBBitsResult + 
						nResultRowBytes*y + x*2)) = (WORD)dwPixel;
					break;
				case 24:
					dwPixel = *((LPDWORD)((LPBYTE)lpDIBBits + 
						nRowBytes*sourcey + sourcex*3)) & 0xffffff;
					*((LPDWORD)((LPBYTE)lpDIBBitsResult + 
						nResultRowBytes*y + x*3)) |= dwPixel;
					break;
				case 32:
					dwPixel = *((LPDWORD)((LPBYTE)lpDIBBits + 
						nRowBytes*sourcey + sourcex*4));
					*((LPDWORD)((LPBYTE)lpDIBBitsResult + 
						nResultRowBytes*y + x*4)) = dwPixel;
				}
			}
			else 
			{
				// 画背景色.
				switch(bpp)
				{
				case 16:
					*((LPWORD)((LPBYTE)lpDIBBitsResult + 
						nResultRowBytes*y + x*2)) = 
						(WORD)dwBackColor;
					break;
				case 24:
					*((LPDWORD)((LPBYTE)lpDIBBitsResult + 
						nResultRowBytes*y + x*3)) |= dwBackColor;
					break;
				case 32:
					*((LPDWORD)((LPBYTE)lpDIBBitsResult + 
						nResultRowBytes*y + x*4)) = dwBackColor;
					break;
				}
			}
		}
	}

	GlobalUnlock(hDIB);
	GlobalUnlock(hDIBResult);
	WaitCursorEnd();

	return hDIBResult;
}

/************************************************************************* 
 * 
 * 函数名:RotateDIB() 
 * 
 * 参数: 
 * 
 * hDIB		     - 要旋转的DIB
 * 
 * 返回值: 
 * 
 * HDIB      - 返回新的已经旋转的DIB句柄
 * 
 * 描述: 
 * 
 * 旋转DIB位图
 * 
 ************************************************************************/ 
HDIB RotateDIB(HDIB hDib)
{
	WaitCursorBegin();

	// 旧DIB
	LPBYTE lpDIBSrc = (LPBYTE)GlobalLock(hDib);

	DWORD lSrcWidth = DIBWidth(lpDIBSrc);
	DWORD lSrcHeight = DIBHeight(lpDIBSrc);
	WORD wBitCount = ((LPBITMAPINFOHEADER)lpDIBSrc)->biBitCount;
	//位位置
    LPBYTE lpOldBits = FindDIBBits(lpDIBSrc);

	//得到新DIB的 bytes/pixel, bytes/行
	double fColorBytes = (double)((double)wBitCount/8.0);
	DWORD lSrcRowBytes = WIDTHBYTES1(lSrcWidth*((DWORD)wBitCount));
	DWORD lDestRowBytes = WIDTHBYTES1(lSrcHeight*((DWORD)wBitCount));

	//调整新DIB尺寸
	DWORD dwDataLength = GlobalSize(hDib);
	dwDataLength += lDestRowBytes*(lSrcWidth-1)+(DWORD)((lSrcHeight-1)*fColorBytes) - 
				  lSrcRowBytes*(lSrcHeight-1)+(DWORD)((lSrcWidth-1)*fColorBytes);
	HDIB hNewDib = GlobalAlloc(GHND, dwDataLength);
	if (! hNewDib)
	{
		WaitCursorEnd();
		return NULL;
	}
	//新DIB缓冲区
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	// 从旧DIB拷贝LPBITMAPINFO到新DIB
	memcpy(lpDIB, lpDIBSrc, sizeof(BITMAPINFOHEADER)+PaletteSize(lpDIBSrc));
	// 交换宽度和高度
	((LPBITMAPINFOHEADER)lpDIB)->biHeight = lSrcWidth;
	((LPBITMAPINFOHEADER)lpDIB)->biWidth = lSrcHeight;
	// 新数据位的位置
	LPBYTE lpData = FindDIBBits(lpDIB);

	DWORD i, j;
	switch (wBitCount)
	{
	case 1:
		for (i=0; i<lSrcHeight; ++i)
		{
			for (j=0; j<lSrcWidth; ++j)
			{
				*(lpData+(lDestRowBytes*j+(lSrcHeight-i-1)/8)) &= ~(1<<(7-((lSrcHeight-i-1)%8)));
				*(lpData+(lDestRowBytes*j+(lSrcHeight-i-1)/8)) |= 
					((*(lpOldBits+(lSrcRowBytes*i+j/8))<<(j%8))>>7)<<(7-((lSrcHeight-i-1)%8));
			}
		}
		break;
	case 4:
		for (i=0; i<lSrcHeight; ++i)
		{
			for (j=0; j<lSrcWidth; ++j)
			{
				*(lpData+(lDestRowBytes*j+(lSrcHeight-i-1)/2)) &= ((lSrcHeight-i-1)%2) ? 0xf0 : 0x0f;
				*(lpData+(lDestRowBytes*j+(lSrcHeight-i-1)/2)) |= 
					((*(lpOldBits+(lSrcRowBytes*i+j/2))<<(j%2 ? 4 : 0))>>4)<<(((lSrcHeight-i-1)%2) ? 0 : 4);
			}
		}
		break;
	case 8:
		for (i=0; i<lSrcHeight; ++i)
		{
			for (j=0; j<lSrcWidth; ++j)
			{
				*(lpData+(lDestRowBytes*j+lSrcHeight-i-1))
					= *(lpOldBits+(lSrcRowBytes*i+j));
			}
		}
		break;
	case 24:
		for (i=0; i<lSrcHeight; ++i)
		{
			for (j=0; j<lSrcWidth; j++)
			{
				*(lpData+(lDestRowBytes*j+(lSrcHeight-i-1)*3))
					= *(lpOldBits+(lSrcRowBytes*i+j*3));
				*(lpData+(lDestRowBytes*j+(lSrcHeight-i-1)*3)+1)
					= *(lpOldBits+(lSrcRowBytes*i+j*3)+1);
				*(lpData+(lDestRowBytes*j+(lSrcHeight-i-1)*3)+2)
					= *(lpOldBits+(lSrcRowBytes*i+j*3)+2);
			}
		}
		break;
	}

	GlobalUnlock(hDib);
	GlobalUnlock(hNewDib);
	WaitCursorEnd();
	
	return hNewDib;
}

/************************************************************************* 
 * 
 * 函数名:FlipHorzDIB() 
 * 
 * 参数: 
 * 
 * HDIB hDIB		 - 要进行水平镜像的DIB句柄
 * 
 * 返回值: 
 * 
 * HDIB             - 已经水平镜像的DIB句柄 
 * 
 * 描述: 
 * 
 * 函数返回经水平镜像的DIB图形句柄 ,资源DIB不变
 * 
 ************************************************************************/ 
HDIB FlipHorzDIB(HDIB hDib)
{
	WaitCursorBegin();

	// 创建新DIB
	DWORD dwDataLength = GlobalSize(hDib);
	HDIB hNewDib = GlobalAlloc(GHND, dwDataLength);
	if (! hNewDib)
	{
		WaitCursorEnd();
		return NULL;
	}
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	if (lpDIB == NULL)
	{
		WaitCursorEnd();
		return FALSE;
	}
	// 旧DIB 缓冲区
	LPBYTE lpDIBSrc = (LPBYTE)GlobalLock(hDib);

	//从旧DIB拷贝LPBITMAPINFO到新DIB
	memcpy(lpDIB, lpDIBSrc, sizeof(BITMAPINFOHEADER)+PaletteSize(lpDIBSrc));
	
	DWORD lSrcWidth = DIBWidth(lpDIBSrc);
	DWORD lSrcHeight = DIBHeight(lpDIBSrc);
	WORD wBitCount = ((LPBITMAPINFOHEADER)lpDIBSrc)->biBitCount;

	//得到 bytes/pixel, bytes/行  
	double fColorBytes = (double)((double)wBitCount/8.0);
	DWORD lSrcRowBytes = WIDTHBYTES1(lSrcWidth*((DWORD)wBitCount));

	//位位置
    LPBYTE lpOldBits = FindDIBBits(lpDIBSrc);
	LPBYTE lpData = FindDIBBits(lpDIB);
	DWORD i, j;
	switch (wBitCount)
	{
	case 1:
		for (i=0; i<lSrcHeight; ++i)
			for (j=0; j<lSrcWidth; ++j)
			{
				*(lpData+(lSrcRowBytes*i+(lSrcWidth-j-1)/8)) &= ~(1<<(7-((lSrcWidth-j-1)%8)));
				*(lpData+(lSrcRowBytes*i+(lSrcWidth-j-1)/8)) |= 
					((*(lpOldBits+(lSrcRowBytes*i+j/8))<<(j%8))>>7)<<(7-(lSrcWidth-j-1)%8);
			}
		break;
	case 4:
		for (i=0; i<lSrcHeight; ++i)
		{
			for (j=0; j<lSrcWidth; ++j)
			{
				*(lpData+(lSrcRowBytes*i+(lSrcWidth-j-1)/2)) &= ((lSrcWidth-j-1)%2) ? 0xf0 : 0x0f;
				*(lpData+(lSrcRowBytes*i+(lSrcWidth-j-1)/2)) |= 
					((*(lpOldBits+(lSrcRowBytes*i+j/2))<<(j%2 ? 4 : 0))>>4)<<(((lSrcWidth-j-1)%2) ? 0 : 4);
			}
		}
		break;
	case 8:
		for (i=0; i<lSrcHeight; ++i)
			for (j=0; j<lSrcWidth; ++j)
				*(lpData+(lSrcRowBytes*i+lSrcWidth-j-1))
					= *(lpOldBits+(lSrcRowBytes*i+j));
		break;
	case 24:
		for (i=0; i<lSrcHeight; ++i)
			for (j=0; j<lSrcWidth; j++)
			{
				*(lpData+(lSrcRowBytes*i+(lSrcWidth-j-1)*3))
					= *(lpOldBits+(lSrcRowBytes*i+j*3));
				*(lpData+(lSrcRowBytes*i+(lSrcWidth-j-1)*3)+1)
					= *(lpOldBits+(lSrcRowBytes*i+j*3)+1);
				*(lpData+(lSrcRowBytes*i+(lSrcWidth-j-1)*3)+2)
					= *(lpOldBits+(lSrcRowBytes*i+j*3)+2);
			}
		break;
	}

	GlobalUnlock(hDib);
	GlobalUnlock(hNewDib);
	WaitCursorEnd();

	return hNewDib;
}

/************************************************************************* 
 * 
 * 函数名:FlipVertDIB() 
 * 
 * 参数: 
 * 
 * HDIB hDIB		 - 要进行垂直镜像的DIB句柄
 * 
 * 返回值: 
 * 
 * HDIB             - 已经垂直镜像的DIB句柄 
 * 
 * 描述: 
 * 
 * 函数返回经垂直镜像的DIB图形句柄 ,资源DIB不变
 * 
 ************************************************************************/ 
HDIB FlipVertDIB(HDIB hDib)
{
	WaitCursorBegin();

	// 创建新DIB
	DWORD dwDataLength = GlobalSize(hDib);
	HDIB hNewDib = GlobalAlloc(GHND, dwDataLength);
	if (! hNewDib)
	{
		WaitCursorEnd();
		return NULL;
	}
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hNewDib);
	if (lpDIB == NULL)
	{
		WaitCursorEnd();
		return FALSE;
	}
	// 旧DIB缓冲区
	LPBYTE lpDIBSrc = (LPBYTE)GlobalLock(hDib);

	// 从旧DIB拷贝 LPBITMAPINFO 到新DIB 
	memcpy(lpDIB, lpDIBSrc, sizeof(BITMAPINFOHEADER)+PaletteSize(lpDIBSrc));
	
	DWORD lSrcWidth = DIBWidth(lpDIBSrc);
	DWORD lSrcHeight = DIBHeight(lpDIBSrc);
	WORD wBitCount = ((LPBITMAPINFOHEADER)lpDIBSrc)->biBitCount;

	//得到字节位bytes/pixel, bytes/row  
	double fColorBytes = (double)((double)wBitCount/8.0);
	DWORD lSrcRowBytes = WIDTHBYTES1(lSrcWidth*((DWORD)wBitCount));

	// bits 位置
    LPBYTE lpOldBits = FindDIBBits(lpDIBSrc);
	LPBYTE lpData = FindDIBBits(lpDIB);

	for (DWORD i=0; i<lSrcHeight; ++i)
		for (DWORD j=0; j<lSrcRowBytes; ++j)
			*(lpData+(lSrcRowBytes*(lSrcHeight-i-1)+j))
				= *(lpOldBits+(lSrcRowBytes*i+j));

	GlobalUnlock(hDib);
	GlobalUnlock(hNewDib);
	WaitCursorEnd();

	return hNewDib;
}

/************************************************************************* 
 * 
 * 函数名:ChangeDIBSize() 
 * 
 * 参数: 
 * 
 * HDIB hDIB				 - 要进行缩放的DIB句柄
 * int nWidth, int nHeight   - 缩放后的DIB尺寸
 * 
 * 返回值: 
 * 
 * HDIB             - 经缩入后的DIB句柄 
 * 
 * 描述: 
 * 
 * 用于缩放指定的DIB，返回缩放后的DIB图形句柄 
 * 
 ************************************************************************/ 
HDIB ChangeDIBSize(HDIB hDIB, int nWidth, int nHeight)
{
    LPBITMAPINFO lpbmi = NULL; 
    LPBYTE       lpSourceBits, lpTargetBits, lpResult; 
    HDC			 hDC = NULL, hSourceDC, hTargetDC; 
    HBITMAP      hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize, dwTargetBitsSize, dwTargetHeaderSize; 
	HDIB		 hNewDIB;
	DWORD		 dwSize;

	WaitCursorBegin();

	//得到DIB指针
	if (! hDIB)
	{
		WaitCursorEnd();
		return NULL;
	}
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDIB);
	if (! lpSrcDIB)
	{
		WaitCursorEnd();
		return NULL;
	}

    //为新DIB分配BITMAPINFO结构 
    dwTargetHeaderSize = sizeof( BITMAPINFOHEADER ) + PaletteSize(lpSrcDIB); 
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize ); 
	memcpy(lpbmi, lpSrcDIB, dwTargetHeaderSize);
    lpbmi->bmiHeader.biWidth = nWidth; 
    lpbmi->bmiHeader.biHeight = nHeight; 
 
    //使用DIB片段和BitBlt()到转化
	hDC = GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC ); 
 
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPBYTE)lpSrcDIB), dwSourceBitsSize ); 
    lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize; 
 
    //选取DIB片段 到DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap ); 
 
	//将旧位图加入到新位图中 
    SetStretchBltMode( hTargetDC, COLORONCOLOR ); 
    StretchBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, lpSrcDIB->bmiHeader.biWidth, lpSrcDIB->bmiHeader.biHeight, SRCCOPY ); 

    SelectObject( hSourceDC, hOldSourceBitmap ); 
    SelectObject( hTargetDC, hOldTargetBitmap ); 
    DeleteDC( hSourceDC ); 
    DeleteDC( hTargetDC ); 
    ReleaseDC( NULL, hDC ); 
 

    GdiFlush(); 

	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND, dwSize);
    lpResult = (LPBYTE)GlobalLock(hNewDIB);
    memcpy( lpResult, lpbmi, dwTargetHeaderSize ); 
    memcpy( FindDIBBits( (LPBYTE)lpResult ), lpTargetBits, dwTargetBitsSize ); 
 
    DeleteObject( hTargetBitmap ); 
    DeleteObject( hSourceBitmap ); 
    free( lpbmi ); 
	GlobalUnlock(hDIB);
	GlobalUnlock(hNewDIB);
	WaitCursorEnd();
 
    return hNewDIB;
}

/************************************************************************* 
 * 
 * 函数名:ChangeDIBCanvasSize() 
 * 
 * 参数: 
 * 
 * HDIB hDIB				 - 要变化的DIB句柄
 * int nWidth, int nHeight   - DIB的新画布尺寸
 * int nPosition			 - 源DIB在新画布DIB中的位置
 * 
 * 返回值: 
 * 
 * HDIB             - 新DIB句柄
 * 
 * 描述: 
 * 
 * 用于改变画布尺寸，不改变DIB图形的尺寸，但可以设定DIB在新画布上的位置
 * 
 ************************************************************************/ 
HDIB ChangeDIBCanvasSize(HDIB hDIB, int nWidth, int nHeight, int nPosition)
{
    LPBITMAPINFO lpbmi = NULL; 
    LPBYTE       lpSourceBits, lpTargetBits, lpResult; 
    HDC			 hDC = NULL, hSourceDC, hTargetDC; 
    HBITMAP      hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize, dwTargetBitsSize, dwTargetHeaderSize; 
	HDIB		 hNewDIB;
	DWORD		 dwSize;

	WaitCursorBegin();

	//得到DIB指针
	if (! hDIB)
	{
		WaitCursorEnd();
		return NULL;
	}
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDIB);
	if (! lpSrcDIB)
	{
		WaitCursorEnd();
		return NULL;
	}

    //为新DIB分配BITMAPINFO结构 
    dwTargetHeaderSize = sizeof( BITMAPINFOHEADER ) + PaletteSize(lpSrcDIB); 
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize ); 
	memcpy(lpbmi, lpSrcDIB, dwTargetHeaderSize);
    lpbmi->bmiHeader.biWidth = nWidth; 
    lpbmi->bmiHeader.biHeight = nHeight; 
 
	hDC = GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC ); 
 
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPBYTE)lpSrcDIB), dwSourceBitsSize ); 
    lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize; 
 
    //选取DIB片段到DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap ); 
 
	//旧位图在新位图中的位置
	int x = 0;
	int y = 0;
	int nOldWidth = lpSrcDIB->bmiHeader.biWidth;
	int nOldHeight = lpSrcDIB->bmiHeader.biHeight; 
 	switch (nPosition)
	{
	case LEFT_UP:
		x = 0;
		y = 0;
		break;
	case CENTER_UP:
		x = (nWidth-nOldWidth)/2;
		y = 0;
		break;
	case RIGHT_UP:
		x = nWidth-nOldWidth;
		y = 0;
		break;
	case LEFT_CENTER:
		x = 0;
		y = (nHeight-nOldHeight)/2;
		break;
	case CENTER_CENTER:
		x = (nWidth-nOldWidth)/2;
		y = (nHeight-nOldHeight)/2;
		break;
	case CENTER_RIGHT:
		x = nWidth-nOldWidth;
		y = (nHeight-nOldHeight)/2;
		break;
	case LEFT_DOWN:
		x = 0;
		y = nHeight-nOldHeight;
		break;
	case CENTER_DOWN:
		x = (nWidth-nOldWidth)/2;
		y = nHeight-nOldHeight;
		break;
	case RIGHT_DOWN:
		x = nWidth-nOldWidth;
		y = nHeight-nOldHeight;
		break;
	}

	// 开始用白色清除旧位图
	HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255,255,255));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hTargetDC, hBrush);
	HPEN hOldPen = (HPEN)SelectObject(hTargetDC, hPen);
	Rectangle(hTargetDC, 0, 0, nWidth, nHeight);
	SelectObject(hTargetDC, hOldBrush);
	SelectObject(hTargetDC, hOldPen);
	//放旧位图到新位图当中
    BitBlt(hTargetDC, x, y, nOldWidth, nOldHeight, hSourceDC, 0, 0, SRCCOPY); 
 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    SelectObject( hTargetDC, hOldTargetBitmap ); 
    DeleteDC( hSourceDC ); 
    DeleteDC( hTargetDC ); 
    ReleaseDC( NULL, hDC ); 
 

    GdiFlush(); 
 
    //为新位图分配足够内存并拷贝位 
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND, dwSize);
    lpResult = (LPBYTE)GlobalLock(hNewDIB);
    memcpy( lpResult, lpbmi, dwTargetHeaderSize ); 
    memcpy( FindDIBBits( (LPBYTE)lpResult ), lpTargetBits, dwTargetBitsSize ); 
 
    DeleteObject( hTargetBitmap ); 
    DeleteObject( hSourceBitmap ); 
    free( lpbmi ); 
	GlobalUnlock(hDIB);
	GlobalUnlock(hNewDIB);
	WaitCursorEnd();
 
    return hNewDIB;
}

