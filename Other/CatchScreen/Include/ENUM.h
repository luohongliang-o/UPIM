/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：ENUM.h
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：绘画乐园应用程序中所有的枚举定义
*======================================================
* 版本变更记录:
*      v1.0  2002-11-4   14:30    
*
* 
*/

/////////////////////////////////////////////////////////////////////////////

// Copyright (C) 1991 - 1999 Rational Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_SHAPEPEN_3DBDD86701F6_INCLUDED
#define _INC_SHAPEPEN_3DBDD86701F6_INCLUDED
//////////////////////////////////////////////////////////////////////

const COLORREF COLOR_BLACK = RGB(0, 0, 0);
const COLORREF COLOR_WHITE = RGB(255, 255, 255);
const COLORREF COLOR_PINK = RGB(255, 0, 255);

const  INT COLOR_NUMBER	= 256;
const COLORREF PP_FILL_COLOR = COLOR_BLACK;         //填充颜色
const INT PP_PEN_COLOR = 1;                        //画笔颜色
const INT SHOWSCROLL =  0;                         //不显示滚动条


//定义当前工具栏的类型
typedef enum PRETOOLBAR
{
	PBASEPEN,    //基本笔
	PSPECIAPEN,  //其它笔(暂没有用到)
	PPICTURES   //图片
};

//定义画笔工具类型的枚举

typedef enum BASEPEN 
{
	SELRGN,		 //圈选工具 
	PEN,			//包括: 轨迹、铅笔、画刷、形状、喷枪、油漆桶、橡皮
	SUCKERPEN,		//吸管
	SPECIA,			//特殊笔
	TEXT			//文字
};

/*
*枚举名: PEN_TYPE
*说明  : 定义笔的类型的枚举
*版本
*	Revision 1.0	2002/11/21     	创建初始定义
*/
typedef enum enuPEN_TYPE
{
	PTP_PENCIL,			//铅笔
	PTP_RUBBER,			//橡皮擦
	PTP_PAINTBRUSH,		//画刷
	PTP_AIRPEN,			//喷枪
	PTP_PAINTTUB		//油漆桶
}PEN_TYPE;

//////////////////////////////////////////////////////////////////
/*
*枚举名: PEN_TRACK
*说明：定义笔的轨迹常量，共有五种轨迹
*版本
*	Revision 1.0	2002/11/12     	创建初始定义
*/
typedef enum enuPEN_TRACK
{
	PT_RANDOM,				// 任意轨迹
	PT_LINE,				// 直线
	PT_ELLIPSE,				// 椭圆
	PT_TRIANGLE,			// 三角形
	PT_RECTANGLE,			// 矩形
	PT_TETRAGON,			// 四角星
	PT_PENTAGON,			// 五角星
	PT_FIVESIDE,			// 五边形
	PT_POLYGON,				// 多边形
	PT_ARROW                // 箭头
} PEN_TRACK;

//////////////////////////////////////////////////////////////////
//定义特殊笔枚举
enum SPECIAPEN 
{
	LEAF=0,	   //树叶
	ARROW,	   //箭头
	WOODBAR,   //木栅栏(端部)
    BLEB,      //水泡
	WOODBAR1  //木栅栏(中部)
};
///////////////////////////////////////////////////////////////////
//定义图片笔刷工具枚举
typedef enum PICTURES 
{/*
	NATURE,        //自然场景
	PEOPLE,	       //人物
	ANIMAL,	       //动物
	HEXABOD,	   //昆虫
	SEAANIMAL,	    //海洋动物
	COSTERMONGER,	//水果
	PLANT,	        //植物
	TRAFFIC,	    //交通工具
	BUILDING,	    //建筑物
	FIRMAMENT	    //太空
*/
	IMAGE_1,        //自然场景
	IMAGE_2,	       //人物
	IMAGE_3,	       //动物
	IMAGE_4,	   //昆虫
	IMAGE_5,	    //海洋动物
	IMAGE_6,	//水果
	IMAGE_7,	        //植物
	IMAGE_8,	    //交通工具
	IMAGE_9,	    //建筑物
	IMAGE_10	    //太空
};

//////////////////////////////////////////////////////////////////////
//定义笔触类型枚举
/*
*枚举名: PEN_STYLE
*说明：定义笔尖形状的枚举常量，共有五种笔尖形状
*版本
*	Revision 1.0	2002/11/1     	创建初始定义
*/
typedef enum enuPEN_STYLE
{
	PS_CIRCLE,				// 圆形笔尖
	PS_VELLIPSE,			// 垂直的椭圆笔尖
	PS_HELLIPSE,			// 水平的椭圆笔尖
	PS_RELLIPSE,			// 向右倾斜的椭圆笔尖
	PS_LELLIPSE		    	// 向左倾斜的椭圆笔尖	
} PEN_STYLE;

//////////////////////////////////////////////////////////////////////
//定义字体类型枚举
typedef enum FONTSTYLE
{
   BOLD,        //加粗
   INCLINE,     //倾斜
   UNDERLINE    //下划线
 
};


/*
*枚举名: SELECT_TOOL
*说明：定义选择工具的几个子工具
*版本
*	Revision 1.0	2002/11/21    	创建初始定义
*/
typedef enum enuSELECT_TOOL
{
	ST_ALL,					// 全部选择
	ST_RECTANGLE,			// 矩形框选择
	ST_CUT,					// 剪切
	ST_COPY,				// 复制
	ST_PASTE				// 粘贴
}SELECT_TOOL;




#endif /* _INC_SHAPEPEN_3DBDD86701F6_INCLUDED */
