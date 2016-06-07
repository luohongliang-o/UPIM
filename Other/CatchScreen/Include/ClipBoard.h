// ClipBoard.h: interface for the CClipBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIPBOARD_H__206A16A3_782E_4C31_8DA7_6A940C32833E__INCLUDED_)
#define AFX_CLIPBOARD_H__206A16A3_782E_4C31_8DA7_6A940C32833E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDib;
//class CFloatDibWnd;

class CClipBoard  
{
protected:
	// ¼ôÌù°åÖÐ±£´æµÄÍ¼Æ¬
	CDib * m_pDib;


public:
	BOOL IsEmpty();
	CDib* GetDibFromClipBoard();
	BOOL PutDibToClipBoard(CDib * pDib);
	CClipBoard();
	virtual ~CClipBoard();


protected:
// 	CFloatDibWnd * CenterPaste(HWND hWnd);
// 	CFloatDibWnd * Paste(HWND hWnd, int x = 0, int y = 0, int flag = 0);
};

#endif // !defined(AFX_CLIPBOARD_H__206A16A3_782E_4C31_8DA7_6A940C32833E__INCLUDED_)
