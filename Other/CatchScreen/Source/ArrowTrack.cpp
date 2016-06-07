#include "stdafx.h"
#include "..\Include\ArrowTrack.h"
#include <math.h>
#include <limits>
const float PI = 3.1415;
CArrowTrack::CArrowTrack(HDC hDC, COLORREF crTrackColor) : CTrack(hDC, crTrackColor)
{
	m_strTrackName = _T("箭头");
}


CArrowTrack::~CArrowTrack(void)
{
}

BOOL CArrowTrack::CreatePos()
{
	if (m_ptTrackStartPoint.y == m_ptTrackEndPoint.y && m_ptTrackStartPoint.x == m_ptTrackEndPoint.x)
	{
		// 水平线或是垂直线
		LineDDA(m_ptTrackStartPoint.x, m_ptTrackStartPoint.y, m_ptTrackEndPoint.x, m_ptTrackEndPoint.y, CTrack::AddPosToList, (LPARAM) this);
	}
	else
	{
		double angle1,angle2,angle3;
		double angle = 20.0;
		double L1,L2,L3;
		double L = 20.0;
		CPoint ptVertex[7];
		ptVertex[0] = m_ptTrackStartPoint;
		ptVertex[3] = m_ptTrackEndPoint;
		int direction = (ptVertex[3].x-ptVertex[0].x>0)?1:-1;
		L1 = sqrt(pow(double(ptVertex[3].x-ptVertex[0].x),2)+pow(double(ptVertex[3].y-ptVertex[0].y),2));
		if (L1 < 20) {
			L1 = 20;
			L = 10;
		}
		double a = L*cos(angle/2*PI/180);
		L2 = sqrt(pow(L1 - L*cos(angle/2*PI/180),2)+pow(L*sin(angle/2*PI/180),2));
		L3 = sqrt(pow(L1 - L*cos(angle/2*PI/180),2)+pow(L*sin(angle/2*PI/180)*2/3,2));
		angle1 = asin((double)(ptVertex[3].y-ptVertex[0].y)/L1)*180/PI;
		angle2 = asin(double(L/2/L2))*180/PI;
		angle3 = asin((double)L/3/L3)*180/PI;

		ptVertex[1].x = direction*L3*cos((angle1+angle3)*PI/180)+ptVertex[0].x;
		ptVertex[1].y = L3*sin((angle1+angle3)*PI/180)+ptVertex[0].y;
		ptVertex[2].x = direction*L2*cos((angle1+angle2)*PI/180)+ptVertex[0].x;
		ptVertex[2].y = L2*sin((angle1+angle2)*PI/180)+ptVertex[0].y;
		ptVertex[4].x = direction*L2*cos((angle1-angle2)*PI/180)+ptVertex[0].x;
		ptVertex[4].y = L2*sin((angle1-angle2)*PI/180)+ptVertex[0].y;
		ptVertex[5].x = direction*L3*cos((angle1-angle3)*PI/180)+ptVertex[0].x;
		ptVertex[5].y = L3*sin((angle1-angle3)*PI/180)+ptVertex[0].y;
		ptVertex[6] = ptVertex[0];
		LineDDA(ptVertex[0].x, ptVertex[0].y, ptVertex[1].x, ptVertex[1].y, CTrack::AddPosToList, (LPARAM) this);
		LineDDA(ptVertex[1].x, ptVertex[1].y, ptVertex[2].x, ptVertex[2].y, CTrack::AddPosToList, (LPARAM) this);
		LineDDA(ptVertex[2].x, ptVertex[2].y, ptVertex[3].x, ptVertex[3].y, CTrack::AddPosToList, (LPARAM) this);
		LineDDA(ptVertex[3].x, ptVertex[3].y, ptVertex[4].x, ptVertex[4].y, CTrack::AddPosToList, (LPARAM) this);
		LineDDA(ptVertex[4].x, ptVertex[4].y, ptVertex[5].x, ptVertex[5].y, CTrack::AddPosToList, (LPARAM) this);
		LineDDA(ptVertex[5].x, ptVertex[5].y, ptVertex[6].x, ptVertex[6].y, CTrack::AddPosToList, (LPARAM) this);
	}
	return TRUE;
}

void CArrowTrack::Draw(const CPoint& ptStart, const CPoint& ptEnd) const
{
	HDC hDC = m_hDC;
	if (ptStart.x == ptEnd.x && ptStart.y == ptEnd.y)
	{
		// 起点和终点相同，就画一个点
		SetPixel(hDC, ptStart.x, ptEnd.y, TRACK_PEN_COLOR);
		return;
	}
	double angle1,angle2,angle3;
	double angle = 20.0;
	double L1,L2,L3;
	double L = 20.0;
	CPoint ptVertex[7];
	ptVertex[0] = ptStart;
	ptVertex[3] = ptEnd;
	int direction = (ptVertex[3].x-ptVertex[0].x>0)?1:-1;

	L1 = sqrt(pow(double(ptVertex[3].x-ptVertex[0].x),2)+pow(double(ptVertex[3].y-ptVertex[0].y),2));
	if (L1 < 20) {
		L1 = 20;
		L = 10;
	}
	L2 = sqrt(pow(L1 - L*cos(angle/2*PI/180),2)+pow(L*sin(angle/2*PI/180),2));
	L3 = sqrt(pow(L1 - L*cos(angle/2*PI/180),2)+pow(L*sin(angle/2*PI/180)*2/3,2));
	angle1 = asin((double)(ptVertex[3].y-ptVertex[0].y)/L1)*180/PI;
	angle2 = asin(double(L/2/L2))*180/PI;
	angle3 = asin((double)L/3/L3)*180/PI;

	ptVertex[1].x = direction*L3*cos((angle1+angle3)*PI/180)+ptVertex[0].x;
	ptVertex[1].y = L3*sin((angle1+angle3)*PI/180)+ptVertex[0].y;
	ptVertex[2].x = direction*L2*cos((angle1+angle2)*PI/180)+ptVertex[0].x;
	ptVertex[2].y = L2*sin((angle1+angle2)*PI/180)+ptVertex[0].y;
	ptVertex[4].x = direction*L2*cos((angle1-angle2)*PI/180)+ptVertex[0].x;
	ptVertex[4].y = L2*sin((angle1-angle2)*PI/180)+ptVertex[0].y;
	ptVertex[5].x = direction*L3*cos((angle1-angle3)*PI/180)+ptVertex[0].x;
	ptVertex[5].y = L3*sin((angle1-angle3)*PI/180)+ptVertex[0].y;
	ptVertex[6] = ptVertex[0];
	Polygon(hDC,ptVertex,7);
}

