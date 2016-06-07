/**
*   SelectTool.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   定义选取工具的抽象基类
*======================================================
*   版本
*   Revision 1.0	2002/11/14    	  定义选取工具的抽象基类
*/

#if !defined(AFX_SELECTTOOL_H__7C6DD163_F7C8_11D6_95DB_000795CFF65D__INCLUDED_)
#define AFX_SELECTTOOL_H__7C6DD163_F7C8_11D6_95DB_000795CFF65D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCartoon;
class CFloatDibWnd;


/*
*类名: CSelectTool
*说明：定义各种选取工具的抽象基类，抽取取公共接口
*版本
*	Revision 1.0	2002/11/14     	 完成 CSelectTool 的基本功能
*/
class CSelectTool  
{
protected:
	HWND m_hWnd;
	HDC m_hDC;
	CPoint m_ptSelectStartPoint;
	CPoint m_ptSelectEndPoint;


public:
	CSelectTool(HWND hWnd);
	virtual ~CSelectTool();


public:
	void BeginSelect(const CPoint& ptPoint);
	void ChangeSelect(const CPoint& ptPoint);
	CFloatDibWnd * EndSelect(const CPoint& ptPoint);


protected:
	virtual void DarkUnselectedRgn();
	virtual void DrawRubber(const CPoint& ptPoint) const;
	virtual void DrawTrack(const CPoint& ptStart, const CPoint& ptEnd) const = 0;
};

#endif // !defined(AFX_SELECTTOOL_H__7C6DD163_F7C8_11D6_95DB_000795CFF65D__INCLUDED_)
