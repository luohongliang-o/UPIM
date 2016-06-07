/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：includeres.h
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：绘画乐园应用程序主界面要使用的外部位图文件名静态数组定义
*======================================================
* 版本变更记录:
*      v1.0  2002-11-8   9：50   罗伟
*
* 
*/

/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include "resource.h"		// 包含资源文件

#ifndef _INCLUDERES
#define _INCLUDERES

struct tagBTNRES
{
	//所在面板索引号
   INT BarIndex;
   //当前面板上的按纽索引号
    INT BtnIndex;  
	//面板按纽对应的文件名
	CString resFileName[2];
	//当前面板上的按纽数目
	INT count;
};



   //定义调色板上的12个颜色控件的ID值，以便创建调色板上的取色控件
	static UINT sel[16]= {IDC_SELCOLOR0,  IDC_SELCOLOR1,  IDC_SELCOLOR2,  IDC_SELCOLOR3, 
	                      IDC_SELCOLOR4,  IDC_SELCOLOR5,  IDC_SELCOLOR6,  IDC_SELCOLOR7, 
				          IDC_SELCOLOR8,  IDC_SELCOLOR9,  IDC_SELCOLOR10,  IDC_SELCOLOR11,
						  IDC_SELCOLOR12,  IDC_SELCOLOR13,  IDC_SELCOLOR14,  IDC_SELCOLOR15,
};



//	static COLORREF col[12] = {RGB(255, 0, 0), RGB(255, 255, 255), RGB(128, 128, 128), RGB(0, 0, 0), 
//		                       RGB(199, 178, 154), RGB(0, 186, 81), RGB(96, 57, 19), RGB(255, 0, 255), 
//		                       RGB(0, 0, 255), RGB(0, 255, 255), RGB(0, 255, 0), RGB(255, 0, 0)
//	};


	struct IniPaintSet
	{
		//[SET]   当前取文件类型
		//缩略图文件名:   类型号_img部位号_序号.bmp
		//全图文件名:     类型号_Pimg部位号_序号.bmp
		INT curType;
        BOOL bCartoonOrgSize; //卡通画导入画布时是否为原图尺寸，为0时其显示大小同主窗口的滑块位置有关
		INT  Transparence;    //特殊笔4在画布上作画时的透明程度(0-100)
        BOOL bAutoSave;       //当画布上的图象更改时,时否即时自动的更新其关联的EXE中的显示图象的内容 

        COLORREF tranCol;   //在画布导入位图时的透明颜色(初始化为粉红色RGB(255,0,255)) 
        CString saveTmpFile;  //画布关闭时,默认保存的临时位图文件名(位于应用程序目录)
    //    COLORREF colTran;     //绘画的透明色



		//[IMAGENUM]  //对应10个区的可用文件数量
		INT img1Num;
		INT img2Num;
		INT img3Num;
		INT img4Num;
		INT img5Num;
		INT img6Num;
		INT img7Num;
		INT img8Num;
		INT img9Num;
		INT img10Num;
        
        //10种类型卡通图象按纽上的提示信息
		CString img1ToolTip;
		CString img2ToolTip;
		CString img3ToolTip;
		CString img4ToolTip;
		CString img5ToolTip;
		CString img6ToolTip;
		CString img7ToolTip;
		CString img8ToolTip;
		CString img9ToolTip;
		CString img10ToolTip;

		//特殊笔数量的定义
		INT Speci1Num;   //第一种类型，滚动条选择类型
		INT Speci2Num;   //第二种类型，鼠标按下移动选择 （上，下，左，右）     
		INT Speci3Num;   //第三种类型，鼠标点击和移动选择
		INT Speci4Num;   //第四种类型，鼠标点击和移动随机选择

	}INIPAINTSET;

	//以下位图位于[安装目录...]\BtnRes\  目录下
	
    static CString btnbmp[]={_T("a1-2.bmp"), _T("a1-1.bmp"), _T("a2-2.bmp"), _T("a2-1.bmp"),     //a部份 [0]---[3]
                               _T("a3-2.bmp"), _T("a3-1.bmp"), _T("a4-2.bmp"), _T("a4-1.bmp"), 	   //      [4]---[7]
	                           _T("a5-2.bmp"), _T("a5-1.bmp"), _T("a6-2.bmp"), _T("a6-1.bmp"), 	   //      [8]---[11]
                               _T("a7-2.bmp"), _T("a7-1.bmp"), _T("a8-2.bmp"), _T("a8-1.bmp"),     //      [12]---[15]
	                           _T("a9-2.bmp"), _T("a9-1.bmp"), _T("a10-2.bmp"), _T("a10-1.bmp"),   //      [16]---[19]

							   _T("b1-2.bmp"), _T("b1-1.bmp"), _T("b2-2.bmp"), _T("b2-1.bmp"),     //b部份 [20]---[23]
                               _T("b3-2.bmp"), _T("b3-1.bmp"), _T("b4-2.bmp"), _T("b4-1.bmp"), 	   //      [24]---[27]
	                           _T("b5-2.bmp"), _T("b5-1.bmp"), _T("b6-2.bmp"), _T("b6-1.bmp"), 	   //      [28]---[31]

							   //C部份为10种卡通按纽上的图象，这些图象在加载时需要根据当前系统指定的画图类型重机关指定文件名
							   _T("0_c1-2.bmp"), _T("0_c1-1.bmp"), _T("0_c2-2.bmp"), _T("0_c2-1.bmp"), 	   //c部份 [32]---[35]
							   _T("0_c3-2.bmp"), _T("0_c3-1.bmp"), _T("0_c4-2.bmp"), _T("0_c4-1.bmp"), 	   //      [36]---[37]
							   _T("0_c5-2.bmp"), _T("0_c5-1.bmp"), _T("0_c6-2.bmp"), _T("0_c6-1.bmp"), 	   //      [40]---[43]		
							   _T("0_c7-2.bmp"), _T("0_c7-1.bmp"), _T("0_c8-2.bmp"), _T("0_c8-1.bmp"), 	   //      [44]---[47]
							   _T("0_c9-2.bmp"), _T("0_c9-1.bmp"), _T("0_c10-2.bmp"), _T("0_c10-1.bmp"),   //      [48]---[51]			
							 
                               _T("d1-2.bmp"), _T("d1-1.bmp"), _T("d2-2.bmp"), _T("d2-1.bmp"), 	   //d部份 [52]---[55]
	                           _T("d3-2.bmp"), _T("d3-1.bmp"), _T("d4-2.bmp"), _T("d4-1.bmp"), 	   //      [56]---[57]
                               _T("d5-2.bmp"), _T("d5-1.bmp"), _T("d6-2.bmp"), _T("d6-1.bmp"), 	   //      [60]---[63]
	                           _T("d7-2.bmp"), _T("d7-1.bmp"), _T("d8-2.bmp"), _T("d8-1.bmp"), 	   //      [64]---[67]

							   _T("e1-2.bmp"), _T("e1-1.bmp"), _T("e2-2.bmp"), _T("e2-1.bmp"),     //e部份 [68]---[71]
		                       _T("e3-2.bmp"), _T("e3-1.bmp"), _T("e4-2.bmp"), _T("e4-1.bmp"), 	   //      [72]---[75]
							   
							   _T("exit-2.bmp"), _T("exit-1.bmp"),	                           //      [76]---[79]
                               _T("d1-3.bmp"), _T("d2-3.bmp"),
	
	};  




static  CString tagBarFile[] = {//形状[0]--[15]
								_T("g19-2.bmp"), _T("g19-1.bmp"),
	                            _T("g18-2.bmp"), _T("g18-1.bmp"), _T("g11-2.bmp"), _T("g11-1.bmp"), 
	                            _T("g12-2.bmp"), _T("g12-1.bmp"), _T("g13-2.bmp"), _T("g13-1.bmp"), 
	                            _T("g14-2.bmp"), _T("g14-1.bmp"), _T("g15-2.bmp"), _T("g15-1.bmp"), 
                                _T("g16-2.bmp"), _T("g16-1.bmp"),  
                               //特殊笔[16] --[23](这些图象可根据设置更改文件名在文件名前加下   序号_)
                                _T("g21-2.bmp"), _T("g21-1.bmp"), _T("g22-2.bmp"), _T("g22-1.bmp"), 
	                            _T("g23-2.bmp"), _T("g23-1.bmp"), _T("g24-2.bmp"), _T("g24-1.bmp"), 
                               //字体[24]-[29]
                                _T("g31-2.bmp"), _T("g31-1.bmp"), _T("g32-2.bmp"), _T("g32-1.bmp"), 
	                            _T("g33-2.bmp"), _T("g33-1.bmp"),                       
                               //树叶的滚动按纽[30]--[33]
                               _T("g51-2.bmp"), _T("g51-1.bmp"), _T("g52-2.bmp"), _T("g52-1.bmp"), /*   _T("g51-3.bmp"), _T("g52-3.bmp"),*/
							   //圈选工具按纽[34]-[43]
							   _T("g41-2.bmp"), _T("g41-1.bmp"), _T("g42-2.bmp"), _T("g42-1.bmp"),
							   _T("g43-2.bmp"), _T("g43-1.bmp"), _T("g44-2.bmp"), _T("g44-1.bmp"),							
                         	   _T("g45-2.bmp"), _T("g45-1.bmp"),

							   //增加形状[44]-[45]
                         	   _T("g17-2.bmp"), _T("g17-1.bmp"),
							    
							   //增加字体类型第三态[46]- [48]
                         	   _T("g31-3.bmp"), _T("g32-3.bmp"), _T("g33-3.bmp")

};


//特殊笔文件及卡通图片的151张图片img[0]---img[150]位于\\picture\\目录下，动态调用（用自加方式，不预定义静态数组）
               


#endif   