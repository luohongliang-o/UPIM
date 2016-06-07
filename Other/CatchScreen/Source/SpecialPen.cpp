
/**
*  SpecialPen.cpp
*  CPH 1.0 版
*  完成特殊笔的功能（树叶，箭头，栅栏，水泡)
*  Copyright 2002-2003 . All Rights Reserved.
*  实现拾色功能
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13 
*
*  CPH 1.0 2002/11/14  解决了内存泄露问题
*
*/

#include "stdafx.h"
#include "..\Paint\PaintDlg.h"
#include "..\\include\\SpecialPen.h"
#include  <math.h>


#include "..\\include\\includeres.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//应用程序运行路径,如"c:\cph\"
extern  CString g_strAppPath;        
extern  IniPaintSet g_stuPaintSet;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
*--------------------------------------------------------------------------------
*  CSpecialPen()
*  类的构造函数
*  Parameter :
*     无
*  Return value:
*     无
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13  完成数据的初始化
*
*/


CSpecialPen::CSpecialPen(CCPHDlg * pMainWnd/*==NULL*/)
{
	m_pMainWnd = pMainWnd;
	m_psDib = new CDib();
	m_speciapen = LEAF;
	
	m_memDC.CreateCompatibleDC (NULL);

    //设置初始透明度为 30%
	m_iTransparence = g_stuPaintSet.Transparence;


	//设置默认特殊笔
	m_nStyle = 0;
	m_nIndex = 0;
    SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW));
}

CSpecialPen::~CSpecialPen()
{
	if(m_psDib)
	{
		delete m_psDib;
		m_psDib = NULL;
	}
}

/*
*--------------------------------------------------------------------------------
*  GetWidth()
*  取得特殊笔的宽度
*  Parameter :  
*              无
*  Return value:
*              无
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13  取得特殊笔的宽度
*
*/

int CSpecialPen::GetWidth()
{
 //  return m_bitmap.bmWidth; 
   return m_psDib->GetWidth();
}

/*
*--------------------------------------------------------------------------------
*  GetHeight()
*  取得特殊笔的高度
*  Parameter :  
*              无
*  Return value:
*              无
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13  取得特殊笔的高度
*
*/

int CSpecialPen::GetHeight()
{
   return m_psDib->GetHeight();; 
}

/*
*--------------------------------------------------------------------------------
*  GetDibWidth()
*  取得DIB特殊笔的宽度
*  Parameter :  
*              无
*  Return value:
*              无
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/22 罗伟 取得DIB特殊笔的宽度
*
*/

int CSpecialPen::GetDibWidth()
{
   return m_psDib->GetWidth();
}

/*
*--------------------------------------------------------------------------------
*  GetDibHeight()
*  取得DIB特殊笔的高度
*  Parameter :  
*              无
*  Return value:
*              无
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/22 罗伟 取得DIB特殊笔的高度
*
*/

int CSpecialPen::GetDibHeight()
{
   return m_bitmap.bmHeight; 
}



/*
*--------------------------------------------------------------------------------
*  SetTransparence()
*  设置透明度
*  Parameter :  
*              [IN]    iTransparence
*                 透明度(0－100)
*  Return value:
*              无
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13  设置透明度
*
*/

void CSpecialPen::SetTransparence (int iTransparence)
{
    m_iTransparence = iTransparence;
}

/*
*--------------------------------------------------------------------------------
*  chooseSpecialPen()
*  选择特殊画笔 
*  Parameter :  
*              [IN]    nStyle
*                 类型(1.树叶, 2.箭头, 3.栅栏, 4.水泡)
*              [IN]    nIndex
*                 序号
*  Return value:
*              TRUE or FALSE
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13  选择特殊画笔，并把画笔装入内存 
*
*/

BOOL CSpecialPen::chooseSpecialPen (int nStyle, int nIndex)
{

	if (nStyle < 0 || nStyle > 3 || nIndex < 0 || nIndex >= MAX_SPECIALPEN)
		return FALSE;
	m_nStyle = nStyle;
	m_nIndex = nIndex;

	//判断选择的特殊笔句柄是否为空
	if (m_hBitmap[m_nStyle][m_nIndex] == NULL) 
		return FALSE;

	//把指定的特殊笔句柄装入内存
	m_memDC.SelectObject (m_hBitmap[m_nStyle][m_nIndex]);
	::GetObject (m_hBitmap[m_nStyle][m_nIndex], sizeof (m_bitmap), &m_bitmap);

    m_speciapen = SPECIAPEN(nStyle);
    if(nStyle == 2 && nIndex == 1)
		m_speciapen = WOODBAR1;

    return TRUE;
}

/*
*--------------------------------------------------------------------------------
*  Draw()
*  用特殊笔在画板上画图
*  Parameter :  
*              [IN]    hDC
*                 绘画句柄
*              [IN]    point
*                 画板上的位置
*  Return value:
*              无
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13  完成用特殊笔在画板上画图
*
*/

void CSpecialPen::Draw(HDC hDC, CPoint point)
{

	int i, j;
    CPoint currentPoint;

	//如果m_nStyle等于4(即水泡)则设置透明度
    if (m_nStyle == 3)
	{
		//得到中水泡中心点
		int iTemp = m_bitmap.bmWidth;
		currentPoint.x = point.x - iTemp / 2;
		currentPoint.y = point.y - iTemp / 2;

		//用画板上位图遮罩水泡位图中不显的部分
		for (i=0; i < iTemp; i++)
			for (j=0; j<iTemp; j++)
				if (sqrt (pow ((i - iTemp / 2), 2) + pow ((j - iTemp / 2),2)) >= iTemp / 2 -1)
					m_memDC.SetPixel (i, j, GetPixel (hDC, i + currentPoint.x, j+ currentPoint.y));

        //用 Alpah 通道使水泡按一定的透明度显示在画板上
		BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255 * m_iTransparence / 100, 0};
		AlphaBlend (hDC,
					currentPoint.x, currentPoint.y,
					iTemp, iTemp,
					m_memDC.GetSafeHdc(), 
					0, 0,
					iTemp, iTemp,
					blend
					); 


	}
	else
		//显示位图(特殊笔)到画板上
		for (i=0; i < m_bitmap.bmWidth; i++)
			 for (j=0; j < m_bitmap.bmHeight; j++)
				if (GetPixel (m_memDC,i,j) != g_stuPaintSet.tranCol)
					BitBlt (hDC, 
					        point.x - m_bitmap.bmWidth / 2 + i, point.y - m_bitmap.bmHeight / 2 + j,
					        1,1,
					        m_memDC.GetSafeHdc(),i,j,SRCCOPY); 

}


/*
*--------------------------------------------------------------------------------
*  ChangeBitmapSize()
*  用特殊笔在画板上画图
*  Parameter :  
*              [IN]    hBitmap
*                 导入的位图句柄
*              [IN]    ID
                   特殊笔的ID号(0-3)
*              [IN]    Index
*                 导入的位图的序号(0-MAX)
*              [IN]    newRect
*                 位图的新尺寸
*  Return value:
*              经缩放后的位图尺寸
*====================================================
*  版本变更记录
*  CPH 1.1 2002/11/22 罗伟 增加将导入的位图根据设置的尺寸进行缩放后作为当前绘图用的位图
*/
HBITMAP CSpecialPen::ChangeBitmapSize(HBITMAP hBitmap, INT ID, INT index, CRect newRect)
{	
	//从主对话框中得到当前特殊笔的类型
   INT presize = 100;
   
	m_speciapen = m_pMainWnd->GetSpeciaStyle();

    switch(m_speciapen)
	{
		//树叶(最小20*20,最大300*300)
		case LEAF:
			{
                 presize = 20 + INT(double(presize - 1) * 2.8);
                 break;
			}
		//箭头(最小50*50,最大300*300)
		case ARROW:
			{
                 presize = 50 + INT(double(presize- 1 ) * 2.5);
                 break;
			}
		//栅栏(大小暂不变)
		case WOODBAR:
			{
                 presize = -1;
                 break;
			}
		//水泡(大小不变)
		case BLEB:
			{
                 presize = -1;
                 break;
			}
	}//end switch

	m_psDib->Create(CRect(0, 0, 300, 300)); 
    m_psDib->Load(m_bmpFile[ID][index]); 

    LONG windth = m_psDib->GetWidth();
	LONG hight = m_psDib->GetHeight();
	m_psDib->ChangeImageSize(presize, presize);
    windth = m_psDib->GetWidth();
	hight = m_psDib->GetHeight();
	CBitmap * bitmap = m_psDib->GetBitmap();    
	
    hBitmap = (HBITMAP)bitmap;   
	//delete m_psDib;
	//m_psDib = NULL;
	return hBitmap;	
}


/*
*--------------------------------------------------------------------------------
*  DrawDIB()
*  用特殊笔在画板上画图
*  Parameter :  
*              [IN]    pDC
*                 要显示的设备上下文
*              [IN]   point
*                 位图显示的位置
*              [IN]   ID
*                 特殊笔的ID号(0-3)
*              [IN]   Index
*                 文件的索引号(0-MAX)
*              [IN]   size
*                 位图的新尺寸
*              [IN]isChange
                 位图的尺寸是否要变化
*  Return value:
*              
*====================================================
*  版本变更记录
*  CPH 1.1 2002/11/22 罗伟 增加对DIB图形的显示功能
*/

void CSpecialPen::DrawDIB(CDC * pDC, CPoint point, INT ID, INT index, INT size, BOOL isChange)
{
    INT w, h;	
   
 //	m_speciapen = m_pMainWnd->GetSpeciaStyle();  
      
	if(isChange)
	{  
		m_psDib->Load(m_bmpFile[ID][index]); 
  
		 switch(m_speciapen)
		{
			//树叶(最小20*20,最大300*300)
			case LEAF:
				{
					 size = 20 + INT(double(size- 1) * 2.8);
					 break;
				}
			//箭头(最小50*50,最大300*300)
			case ARROW:
				{
					 size = 50 + INT(double(size- 1 ) * 2.5);
					 break;
				}
			//栅栏(大小暂不变)
			case WOODBAR:
			case WOODBAR1:
				{
					 size = 50 + INT(double(size- 1 ) * 2.5);
					 break;
				}
			//水泡(大小不变)
			case BLEB:
				{
					 size = -1;
					 break;
				}
		}//end switch
	}
    if(m_psDib &&  size!= -1)
	{
		//非木栅栏
		if(isChange && m_speciapen !=WOODBAR && m_speciapen !=WOODBAR1)
			m_psDib->ChangeImageSize(size, size);
		//端部木栅栏
		else if(m_speciapen ==WOODBAR)
			m_psDib->ChangeImageSize(size/5, size);
		//中部木栅栏
		else if(m_speciapen ==WOODBAR1)
			m_psDib->ChangeImageSize(size*2/5, size);

		 w = m_psDib->GetWidth();
		 h = m_psDib->GetHeight(); 
	//	 m_psDib->DisplayTransparent(pDC, point.x -w/2 , point.y - h/2, RGB(255, 0, 255)); 
        m_psDib->DisplayTransparent(pDC, point.x -w/2 , point.y - h/2, g_stuPaintSet.tranCol); 


	}
}


//根据当前系统设置的类型索引重新初始化特效笔要导入的文件名
void CSpecialPen::InitBmpFile(INT type)
{
   INT i;
   for(INT k=0;k<4;k++)
	   for(INT n=0;n<MAX_SPECIALPEN;n++)
            m_bmpFile[k][n] =_T("");

   //第一种笔
   for(i = 0 ; i < g_stuPaintSet.Speci1Num && i<MAX_SPECIALPEN; i++)
   {
	   m_bmpFile[0][i].Format("%s\\Picture\\%d_SpecialPen1_%d.bmp", g_strAppPath,g_stuPaintSet.curType,i);
	   m_btn1File[i].Format("%s\\Picture\\%d_SpecialBtn1_%d.bmp", g_strAppPath,g_stuPaintSet.curType,i);
   }
  //第二种笔
   for(i = 0 ; i < g_stuPaintSet.Speci2Num && i<MAX_SPECIALPEN; i++)
	   m_bmpFile[1][i].Format("%s\\Picture\\%d_SpecialPen2_%d.bmp", g_strAppPath,g_stuPaintSet.curType,i);
   
 //第三种笔
   for(i = 0 ; i < g_stuPaintSet.Speci3Num && i<MAX_SPECIALPEN; i++)
	   m_bmpFile[2][i].Format("%s\\Picture\\%d_SpecialPen3_%d.bmp", g_strAppPath,g_stuPaintSet.curType,i);

   //第四种笔
   for(i = 0 ; i < g_stuPaintSet.Speci4Num && i<MAX_SPECIALPEN; i++)
	   m_bmpFile[3][i].Format("%s\\Picture\\%d_SpecialPen4_%d.bmp", g_strAppPath,g_stuPaintSet.curType,i);

	//初始化特殊笔句柄
    for(i=0;i<4;i++)
		for(INT k=0;k<MAX_SPECIALPEN;k++)
		{
			if(m_bmpFile[i][k].IsEmpty())
				continue;
            m_hBitmap[i][k] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
											 m_bmpFile[i][k], IMAGE_BITMAP,
											 0, 0,
											 LR_LOADFROMFILE
											 );
		}

}

//得到第一项特殊笔的索引号为index 的文件名
CString CSpecialPen::GetBtnFile(INT index)
{
    CString fileName;
	if(index >= 0 && index < MAX_SPECIALPEN)
	   fileName = m_btn1File[index];
	return fileName;
}

void CSpecialPen::SetMainWnd(CCPHDlg *pWnd)
{
    m_pMainWnd = pWnd;
}

void CSpecialPen::SetPenType(INT penType)
{    
   m_speciapen = SPECIAPEN(penType);
 
}
