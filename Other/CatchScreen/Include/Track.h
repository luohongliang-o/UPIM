/**
*   Track.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002. All Rights Reserved.
*   定义各种轨迹的抽象基类，抽取取公共接口
*======================================================
*   版本
*   Revision 1.0	2002/11/11    	 建立 CTrack 类
*/

#if !defined(AFX_Track_H__6EF81C80_A150_483E_976E_8528CB0D24A9__INCLUDED_)
#define AFX_Track_H__6EF81C80_A150_483E_976E_8528CB0D24A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

// 定义轨迹笔色常量
const COLORREF TRACK_PEN_COLOR = RGB(0, 0, 0);


/*
*类名: CTrack
*说明：定义各种轨迹的抽象基类，抽取取公共接口
*版本
*	Revision 1.0	2002/11/11     	 建立 CTrack 类
*/
class CTrack
{
protected:
	CString m_strTrackName;
	HDC m_hDC;
	CPoint m_ptTrackEndPoint;
	CPoint m_ptTrackStartPoint;
	CList<CPoint, CPoint&> m_listTrackPoints;


public:
	CTrack(HDC hDC, COLORREF crTrackColor);
	virtual ~CTrack();

	// 往轨迹点列表中加入一个点，处理回调
	static void CALLBACK  AddPosToList(int x, int y, LPARAM lpData);

public:
	void SetTrackColor(COLORREF crTrackColor);
	// 遍历轨迹点集合函数
	POSITION GetHeadPosition();
	CPoint& GetNextPoint(POSITION& rPos);
	// 真正加入轨迹点函数
	void AddPoint(int x, int y);
	void BeginTrack(const CPoint& ptPoint);
	void DrawTrack(const CPoint& ptPoint);
	void EndTrack(const CPoint& ptPoint);


protected:
	// 对一些特殊情况先作出处理再绘图
	void PreDraw(const CPoint& ptStart, const CPoint& ptEnd) const;
	// 对一些特殊情况先作出处理再计算正常轨迹的轨迹点集合
	BOOL PreCreatePos();

	// 根据轨迹创建点的集合
	virtual BOOL CreatePos() = 0;

	// 画轨迹函数
	virtual void Draw(const CPoint& ptStart, const CPoint& ptEnd) const = 0;


protected:
	COLORREF m_crTrackColor;

	// 以下函数皆为辅助作图函数

	// 计算直线方程的函数
	BOOL CalLineEquation(const CPoint & ptBegin, const CPoint & ptEnd, double * k, double * b) const;

	// 求两条直线交点座标函数
	void GetIntersectPoint(const CPoint& pt1Begin, const CPoint& pt1End, const CPoint &pt2Begin, const CPoint &pt2End, CPoint& ptIntersect) const;

	// 等分圆
	BOOL SplitCircle(const CPoint& ptCenter, const CPoint& ptBegin, int nCount, CPoint * ptVertex) const;

	// 得到弧终点座标
	CPoint GetArcEndPoint(const CPoint& ptCenter, double dwRadius, double eStartAngle, double eSweepAngle) const;
};

#endif // !defined(AFX_Track_H__6EF81C80_A150_483E_976E_8528CB0D24A9__INCLUDED_)
