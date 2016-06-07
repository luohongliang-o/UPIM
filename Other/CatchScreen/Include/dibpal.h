/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：dibpal.h
* 版本号  : V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：
*======================================================
* 版本变更记录:
*      v1.0  2002-10-30   20:30   
*
* 
*/

/////////////////////////////////////////////////////////////////////////////
#ifndef DIBPAL_h
#define DIBPAL_h

/*
*类名    : CBmpPalette
*说明    : 位图调色板类
*变更记录:
*
*/

class CBmpPalette : public CPalette
{
public:
    CBmpPalette( CDIBitmap* pBmp );
};

#endif	// DIBPAL_h