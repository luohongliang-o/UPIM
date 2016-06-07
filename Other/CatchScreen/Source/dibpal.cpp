/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：dibpal.cpp
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003. All Rights Reserved.
* 模块简单描述：
*======================================================
* 版本变更记录:
*      v1.0  2002-10-30   20:30    根据一些参考资料完成本类
*
* 
*/

/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Include\DIBitmap.h"
#include "..\Include\dibpal.h"

/*
*--------------------------------------------------------------------------------
*  成员函数名   :CBmpPalette
*  功能描述     :构造彩色对话框的调色板
*  输入参数描述	:
*   CDIBitmap* pBmp：DIB位图指针
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
CBmpPalette::CBmpPalette( CDIBitmap* pBmp )
{
    ASSERT( pBmp );
    int cPaletteEntries = pBmp->GetPalEntries();
    int cPalette = sizeof(LOGPALETTE) +
                   sizeof(PALETTEENTRY) * cPaletteEntries;

    // 因为这个LOGPALETTE 结构体structure是可以更改的,你必
    // 动态分配它, 可以使用一个堆栈

    LOGPALETTE* pPal = (LOGPALETTE*)new BYTE[cPalette];
    RGBQUAD*    pColorTab = pBmp->GetColorTablePtr();
    pPal->palVersion = 0x300;
    pPal->palNumEntries = cPaletteEntries;
    // 循环通过颜色表,增加每种颜色到逻辑调色板中 

    for( int ndx = 0; ndx < cPaletteEntries; ndx++ )
    {
        pPal->palPalEntry[ndx].peRed   = pColorTab[ndx].rgbRed;
        pPal->palPalEntry[ndx].peGreen = pColorTab[ndx].rgbGreen;
        pPal->palPalEntry[ndx].peBlue  = pColorTab[ndx].rgbBlue;
        pPal->palPalEntry[ndx].peFlags = NULL;
    }
    VERIFY( CreatePalette( pPal ) );
    delete [] (BYTE*)pPal;
}
