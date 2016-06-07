
/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：CqOctree.CPP
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：执行颜色量子化的二叉树运算，包括两个全局函数
*     HPALETTE CreateOctreePalette (HBITMAP hImage, UINT nMaxColors, UINT nColorBits)
	  HPALETTE CreateOctreePalette (LPSTR lpDIB, UINT nMaxColors, UINT nColorBits)
*为使用方便把它们定义在DIBAPI.H中
*======================================================
* 版本变更记录:
*      v1.0  2002-11-11   
*
* 
*/

/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "..\Include\dibapi.h"

// 内在结构
typedef struct _NODE 
{
    // 确信结点无子结点
	BOOL bIsLeaf;               
     // 叶结点像素的数量
    UINT nPixelCount; 
	// 红色成份值
    UINT nRedSum; 
	// 绿色成份值       
    UINT nGreenSum;
	// 蓝色成份值
    UINT nBlueSum;              
    // 子节点指针
    struct _NODE* pChild[8];    
     // 指向下一个结点
    struct _NODE* pNext;       

} NODE;

// 函数原型定义在dibapi.h中作为全局函数
//HPALETTE CreateOctreePalette (HDIB hDIB, UINT nMaxColors, UINT nColorBits)
//HPALETTE CreateOctreePalette (LPSTR lpDIB, UINT nMaxColors, UINT nColorBits)

//仅仅本地使用
HPALETTE BuildOctreePalette(HANDLE hImage, UINT nMaxColors, UINT nColorBits);
void AddColor (NODE**, BYTE, BYTE, BYTE, UINT, UINT, UINT*, NODE**);
NODE* CreateNode (UINT, UINT, UINT*, NODE**);
void ReduceTree (UINT, UINT*, NODE**);
void DeleteTree (NODE**);
void GetPaletteColors (NODE*, PALETTEENTRY*, UINT*);
int GetRightShiftCount (DWORD);
int GetLeftShiftCount (DWORD);

// 函数体
/*
*--------------------------------------------------------------------------------
*  成员函数名   :CreateOctreePalette
*  功能描述     :创建Octree树调色板
*  输入参数描述	:
*  HDIB hDIB：DIB句柄
*  UINT nMaxColors：最大颜色数
*  UINT nColorBits：颜色位数
*  返回变量说明 :返回创建的调色板句柄
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
HPALETTE CreateOctreePalette(HDIB hDIB, UINT nMaxColors, UINT nColorBits)
{
	HANDLE hImage;

	hImage = DIBToDIBSection(hDIB);
	if (! hImage)
		return NULL;
	return BuildOctreePalette(hImage, nMaxColors, nColorBits);
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :CreateOctreePalette
*  功能描述     :创建Octree树调色板
*  输入参数描述	:
*  LPBYTE lpDIB：DIB指针
*  UINT nMaxColors：最大颜色数
*  UINT nColorBits：颜色位数
*  返回变量说明 :返回创建的调色板句柄
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
HPALETTE CreateOctreePalette(LPBYTE lpDIB, UINT nMaxColors, UINT nColorBits)
{
	HANDLE hImage;

	hImage = DIBToDIBSection(lpDIB);
	if (! hImage)
		return NULL;
	return BuildOctreePalette(hImage, nMaxColors, nColorBits);
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :BuildOctreePalette
*  功能描述     :创建Octree树调色板
*  输入参数描述	:
*  HANDLE hImage:位图句柄
*  UINT nMaxColors：最大颜色数
*  UINT nColorBits：颜色位数
*  返回变量说明 :返回创建的调色板句柄
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
HPALETTE BuildOctreePalette(HANDLE hImage, UINT nMaxColors, UINT nColorBits)
{
    DIBSECTION ds;
    int i, j, nPad;
    BYTE* pbBits;
    WORD* pwBits;
    DWORD* pdwBits;
    DWORD rmask, gmask, bmask;
    int rright, gright, bright;
    int rleft, gleft, bleft;
    BYTE r, g, b;
    WORD wColor;
    DWORD dwColor, dwSize;
    LOGPALETTE* plp;
    HPALETTE hPalette;
    NODE* pTree;
    UINT nLeafCount, nIndex;
    NODE* pReducibleNodes[9];
    HDC hdc;    
	BYTE* pBuffer;    
	BITMAPINFO bmi;

    // 初始化 octree树变量
    pTree = NULL;
    nLeafCount = 0;
    if (nColorBits > 8) 
        return NULL;
    for (i=0; i<=(int) nColorBits; i++)
        pReducibleNodes[i] = NULL;

    // 扫描DIB并创建octree树
    GetObject (hImage, sizeof (ds), &ds);
    nPad = ds.dsBm.bmWidthBytes - (((ds.dsBmih.biWidth *
        ds.dsBmih.biBitCount) + 7) / 8);

    switch (ds.dsBmih.biBitCount) {

    case 1: // 1-bit DIB    
	case 4: // 4-bit DIB    
	case 8: // 8-bit DIB
    
        hdc = GetDC (NULL);        
		pBuffer = new BYTE[ds.dsBmih.biWidth * 3];
        ZeroMemory (&bmi, sizeof (bmi));
        bmi.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = ds.dsBmih.biWidth;
        bmi.bmiHeader.biHeight = ds.dsBmih.biHeight;
        bmi.bmiHeader.biPlanes = 1;        
		bmi.bmiHeader.biBitCount = 24;
        bmi.bmiHeader.biCompression = BI_RGB;
        for (i=0; i<ds.dsBmih.biHeight; i++) 
		{
            GetDIBits (hdc, (HBITMAP) hImage, i, 1, pBuffer, &bmi,
                DIB_RGB_COLORS);            
			pbBits = pBuffer;
            for (j=0; j<ds.dsBmih.biWidth; j++) 
			{                
				b = *pbBits++;
                g = *pbBits++;                
				r = *pbBits++;
                AddColor (&pTree, r, g, b, nColorBits, 0, &nLeafCount,
                          pReducibleNodes);
                while (nLeafCount > nMaxColors)
                    ReduceTree (nColorBits, &nLeafCount, pReducibleNodes);            
			}        
		}
        delete pBuffer;        
		ReleaseDC (NULL, hdc);        
		break;
		
	//  16-bit DIBs
	case 16: 
        if (ds.dsBmih.biCompression == BI_BITFIELDS) {
            rmask = ds.dsBitfields[0];
            gmask = ds.dsBitfields[1];
            bmask = ds.dsBitfields[2];
        }
        else {
            rmask = 0x7C00;
            gmask = 0x03E0;
            bmask = 0x001F;
        }

        rright = GetRightShiftCount (rmask);
        gright = GetRightShiftCount (gmask);
        bright = GetRightShiftCount (bmask);

        rleft = GetLeftShiftCount (rmask);
        gleft = GetLeftShiftCount (gmask);
        bleft = GetLeftShiftCount (bmask);

        pwBits = (WORD*) ds.dsBm.bmBits;
        for (i=0; i<ds.dsBmih.biHeight; i++) {
            for (j=0; j<ds.dsBmih.biWidth; j++) {
                wColor = *pwBits++;
                b = (BYTE) (((wColor & (WORD) bmask) >> bright) << bleft);
                g = (BYTE) (((wColor & (WORD) gmask) >> gright) << gleft);
                r = (BYTE) (((wColor & (WORD) rmask) >> rright) << rleft);
                AddColor (&pTree, r, g, b, nColorBits, 0, &nLeafCount,
                          pReducibleNodes);
                while (nLeafCount > nMaxColors)
                    ReduceTree (nColorBits, &nLeafCount, pReducibleNodes);
            }
            pwBits = (WORD*) (((BYTE*) pwBits) + nPad);
        }
        break;

    // 24-bit DIBs
    case 24: 
        pbBits = (BYTE*) ds.dsBm.bmBits;
        for (i=0; i<ds.dsBmih.biHeight; i++) {
            for (j=0; j<ds.dsBmih.biWidth; j++) {
                b = *pbBits++;
                g = *pbBits++;
                r = *pbBits++;
                AddColor (&pTree, r, g, b, nColorBits, 0, &nLeafCount,
                          pReducibleNodes);
                while (nLeafCount > nMaxColors)
                    ReduceTree (nColorBits, &nLeafCount, pReducibleNodes);
            }
            pbBits += nPad;
        }
        break;

     // 32-bit DIBs
    case 32: 
        if (ds.dsBmih.biCompression == BI_BITFIELDS) {
            rmask = ds.dsBitfields[0];
            gmask = ds.dsBitfields[1];
            bmask = ds.dsBitfields[2];
        }
        else {
            rmask = 0x00FF0000;
            gmask = 0x0000FF00;
            bmask = 0x000000FF;
        }

        rright = GetRightShiftCount (rmask);
        gright = GetRightShiftCount (gmask);
        bright = GetRightShiftCount (bmask);

        pdwBits = (DWORD*) ds.dsBm.bmBits;
        for (i=0; i<ds.dsBmih.biHeight; i++) {
            for (j=0; j<ds.dsBmih.biWidth; j++) {
                dwColor = *pdwBits++;
                b = (BYTE) ((dwColor & bmask) >> bright);
                g = (BYTE) ((dwColor & gmask) >> gright);
                r = (BYTE) ((dwColor & rmask) >> rright);
                AddColor (&pTree, r, g, b, nColorBits, 0, &nLeafCount,
                          pReducibleNodes);
                while (nLeafCount > nMaxColors)
                    ReduceTree (nColorBits, &nLeafCount, pReducibleNodes);
            }
            pdwBits = (DWORD*) (((BYTE*) pdwBits) + nPad);
        }
        break;

    // DIB 必须是16, 24, 或 32-bit!
    default: 
        return NULL;
    }

    if (nLeafCount > nMaxColors) { 
        DeleteTree (&pTree);
        return NULL;
    }

    // 创建逻辑调色板从octree树中的颜色
    dwSize = sizeof (LOGPALETTE) + ((nLeafCount - 1) * sizeof (PALETTEENTRY));
    if ((plp = (LOGPALETTE*) HeapAlloc (GetProcessHeap (), 0,
        dwSize)) == NULL) {
        DeleteTree (&pTree);
        return NULL;
    }

    plp->palVersion = 0x300;
    plp->palNumEntries = (WORD) nLeafCount;
    nIndex = 0;
    GetPaletteColors (pTree, plp->palPalEntry, &nIndex);
    hPalette = CreatePalette (plp);

    HeapFree (GetProcessHeap (), 0, plp);
    DeleteTree (&pTree);
    return hPalette;
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :AddColor
*  功能描述     :增加颜色值到链表中
*  输入参数描述	:
*   NODE** ppNode:链表结构指针
*   BYTE r:红色位数
*   BYTE g:绿色位数
*   BYTE b:蓝色位数
*   UINT nColorBits:颜色位数
*   UINT nLevel: 树层次
*   UINT* pLeafCount:叶结点数量
*   NODE** pReducibleNodes:链表结构指针
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void AddColor(NODE** ppNode, BYTE r, BYTE g, BYTE b, UINT nColorBits,
    UINT nLevel, UINT* pLeafCount, NODE** pReducibleNodes)
{
    int nIndex, shift;
    static BYTE mask[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

    // 假若链表不存在则创建它
    if (*ppNode == NULL)
        *ppNode = CreateNode (nLevel, nColorBits, pLeafCount,
                              pReducibleNodes);

    // 假若是叶结点更新颜色信息表头
    if ((*ppNode)->bIsLeaf) {
        (*ppNode)->nPixelCount++;
        (*ppNode)->nRedSum += r;
        (*ppNode)->nGreenSum += g;
        (*ppNode)->nBlueSum += b;
    }

    // 不是叶结点
    else {
        shift = 7 - nLevel;
        nIndex = (((r & mask[nLevel]) >> shift) << 2) |
            (((g & mask[nLevel]) >> shift) << 1) |
            ((b & mask[nLevel]) >> shift);
        AddColor (&((*ppNode)->pChild[nIndex]), r, g, b, nColorBits,
                  nLevel + 1, pLeafCount, pReducibleNodes);
    }
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :CreateNode
*  功能描述     :创建链表树
*  输入参数描述	:
*   UINT nLevel:层次
*   UINT nColorBits:颜色位图
*   UINT* pLeafCount:叶指针
*   NODE** pReducibleNodes :链表结构指针
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
NODE* CreateNode (UINT nLevel, UINT nColorBits, UINT* pLeafCount,
                  NODE** pReducibleNodes)
{
    NODE* pNode;

    if ((pNode = (NODE*) HeapAlloc (GetProcessHeap (), HEAP_ZERO_MEMORY,
        sizeof (NODE))) == NULL)
        return NULL;

    pNode->bIsLeaf = (nLevel == nColorBits) ? TRUE : FALSE;
    if (pNode->bIsLeaf)
        (*pLeafCount)++;
    //为本级增加到列表中
    else { 
        pNode->pNext = pReducibleNodes[nLevel];
        pReducibleNodes[nLevel] = pNode;
    }
    return pNode;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :ReduceTree
*  功能描述     :还原树结构
*  输入参数描述	:
*   UINT nColorBits:颜色位数
*   UINT* pLeafCount:叶指针
*   NODE** pReducibleNode:链表结构指针
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void ReduceTree (UINT nColorBits, UINT* pLeafCount, NODE** pReducibleNodes)
{
    int i;
    NODE* pNode;
    UINT nRedSum, nGreenSum, nBlueSum, nChildren;

    // 查打下面的叶结点以简化树结构
    for (i=nColorBits - 1; (i>0) && (pReducibleNodes[i] == NULL); i--);

    // 在层次i中简化结点增加结点到列表
    pNode = pReducibleNodes[i];
    pReducibleNodes[i] = pNode->pNext;

    nRedSum = nGreenSum = nBlueSum = nChildren = 0;
    for (i=0; i<8; i++) {
        if (pNode->pChild[i] != NULL) {
            nRedSum += pNode->pChild[i]->nRedSum;
            nGreenSum += pNode->pChild[i]->nGreenSum;
            nBlueSum += pNode->pChild[i]->nBlueSum;
            pNode->nPixelCount += pNode->pChild[i]->nPixelCount;
            HeapFree (GetProcessHeap (), 0, pNode->pChild[i]);
            pNode->pChild[i] = NULL;
            nChildren++;
        }
    }

    pNode->bIsLeaf = TRUE;
    pNode->nRedSum = nRedSum;
    pNode->nGreenSum = nGreenSum;
    pNode->nBlueSum = nBlueSum;
    *pLeafCount -= (nChildren - 1);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :DeleteTree
*  功能描述     :删除树结构
*  输入参数描述	:
*  NODE** ppNode：链表结构指针
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void DeleteTree(NODE** ppNode)
{
    int i;

    for (i=0; i<8; i++) {
        if ((*ppNode)->pChild[i] != NULL)
            DeleteTree (&((*ppNode)->pChild[i]));
    }
    HeapFree (GetProcessHeap (), 0, *ppNode);
    *ppNode = NULL;
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetPaletteColors
*  功能描述     :得到调色板的颜色
*  输入参数描述	:
*  NODE* pTree:  树结构指针
*  PALETTEENTRY* pPalEntries:调色板指针
*  UINT* pIndex：颜色索引指针
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void GetPaletteColors (NODE* pTree, PALETTEENTRY* pPalEntries, UINT* pIndex)
{
    int i;

    if (pTree->bIsLeaf) {
        pPalEntries[*pIndex].peRed =
            (BYTE) ((pTree->nRedSum) / (pTree->nPixelCount));
        pPalEntries[*pIndex].peGreen =
            (BYTE) ((pTree->nGreenSum) / (pTree->nPixelCount));
        pPalEntries[*pIndex].peBlue =
            (BYTE) ((pTree->nBlueSum) / (pTree->nPixelCount));
        (*pIndex)++;
    }
    else {
        for (i=0; i<8; i++) {
            if (pTree->pChild[i] != NULL)
                GetPaletteColors (pTree->pChild[i], pPalEntries, pIndex);
        }
    }
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetRightShiftCount
*  功能描述     :得到右子树数量
*  输入参数描述	:
*  DWORD dwVal:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
int GetRightShiftCount(DWORD dwVal)
{
    int i;

    for (i=0; i<sizeof (DWORD) * 8; i++) {
        if (dwVal & 1)
            return i;
        dwVal >>= 1;
    }
    return -1;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetLeftShiftCount
*  功能描述     :得到左子树数量
*  输入参数描述	:
*  DWORD dwVal:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
int GetLeftShiftCount (DWORD dwVal)
{
    int nCount, i;

    nCount = 0;
    for (i=0; i<sizeof (DWORD) * 8; i++) {
        if (dwVal & 1)
            nCount++;
        dwVal >>= 1;
    }
    return (8 - nCount);
}
