#pragma once
#include "track.h"
class CArrowTrack :
	public CTrack
{
public:
	CArrowTrack(HDC hDC, COLORREF crTrackColor) ;
	~CArrowTrack(void);
protected:
	// 根据轨迹创建点的集合
	virtual BOOL CreatePos();

	// 画矩形
	virtual void Draw(const CPoint& ptStart, const CPoint& ptEnd) const;

};

