/////////////////////////////////////////////////////////////////////////////
/**
* �ļ���  ��Dibapi.h 
* �汾��  ��V1.0
* ��Ŀ���ƣ�CPH
* ��Ȩ������Copyright 2002-2003 . All Rights Reserved.
* ģ���������DIBͼ�δ�����ͷ�ļ�(�ṩ����DIBͼ�εĺ�����)
*======================================================
* �汾�����¼:
*      v1.0  2002-11-11  
*
* 
*/

/////////////////////////////////////////////////////////////////////////////

#ifndef __DIBAPI_H_
#define __DIBAPI_H_
//////////////////////////////////////////////////////////

//����һ��DIB���
#define HDIB				HANDLE 

// Dib ͷ��� - ����дһ��DIB���ļ��� 
#define DIB_HEADER_MARKER	((WORD) ('M' << 8) | 'B') 

//DIB ����
#define PALVERSION			0x300 

// ����DIBʱ�õĳ���  
#define PW_WINDOW			1 
#define PW_CLIENT			2

// ��ɫ���ӻ����㷨��
#define CQ_HISTOGRAM		1
#define CQ_OCTREE			2

//ͼ���ڻ����ϵ�λ�ó���
#define LEFT_UP				1
#define CENTER_UP			2
#define RIGHT_UP			3
#define LEFT_CENTER			4
#define CENTER_CENTER		5
#define CENTER_RIGHT        6
#define LEFT_DOWN			7
#define CENTER_DOWN			8
#define RIGHT_DOWN			9

// DIB �� 
#define WIDTHBYTES1(bits)    (((bits) + 31) / 32 * 4) 
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER)) 
#define RECTWIDTH(lpRect)   ((lpRect)->right - (lpRect)->left) 
#define RECTHEIGHT(lpRect)  ((lpRect)->bottom - (lpRect)->top) 
#define BOUND(x, mn, mx)	((x) < (mn) ? (mn) : ((x) > (mx) ? (mx) : (x)))
#define WaitCursorBegin()	HCURSOR hcURSOR = SetCursor(LoadCursor(NULL, IDC_WAIT))
#define WaitCursorEnd()		SetCursor(hcURSOR) 
 
//����ԭ�� 
// DIB��ʼ��
HDIB CreateDIB(DWORD, DWORD, WORD); 
HDIB CreateDefaultDIB(DWORD dwWidth, DWORD dwHeight);
void DestroyDIB(HDIB); 
HDIB LoadDIB (LPTSTR); 
BOOL SaveDIB (HDIB, LPTSTR); 
HDIB ReadDIBFile(HANDLE); 
// DIB ����
DWORD BytesPerLine(LPBYTE lpDIB);
DWORD BytesPerLine(HDIB hDIB);
DWORD DIBlockSize(HDIB hDIB);
DWORD DIBlockSize(LPBYTE lpDIB);
DWORD DIBHeight (LPBYTE lpDIB); 
DWORD DIBHeight (HDIB hDIB); 
DWORD DIBWidth (LPBYTE lpDIB); 
DWORD DIBWidth (HDIB hDIB); 
WORD DIBNumColors (LPBYTE lpDIB); 
WORD DIBNumColors (HDIB hDIB); 
WORD DIBBitCount (LPBYTE lpDIB); 
WORD DIBBitCount (HDIB hDIB); 
LPBYTE FindDIBBits (LPBYTE lpDIB); 
WORD PaletteSize (LPBYTE lpDIB); 
WORD PaletteSize (HDIB hDIB); 
// DIB��ʾ
BOOL PaintBitmap(HDC, LPRECT, HBITMAP, LPRECT, HPALETTE, DWORD);
BOOL PaintDIB(HDC, LPRECT, HDIB, LPRECT, HPALETTE, DWORD);
// DIB ����
HBITMAP DIBToDIBSection(LPBYTE lpDIB);
HBITMAP DIBToDIBSection(HDIB hDIB);
HDIB DIBSectionToDIB(HBITMAP hBitmap);
HDIB ConvertDIBFormat(LPBYTE lpSrcDIB, UINT nWidth, UINT nHeight, UINT nbpp, BOOL bStretch, HPALETTE hPalSrc);
HDIB ConvertDIBFormat(HDIB hDIB, UINT nWidth, UINT nHeight, UINT nbpp, BOOL bStretch, HPALETTE hPalSrc);
HDIB ConvertDIBFormat(LPBYTE lpSrcDIB, UINT nbpp, HPALETTE hPalSrc);
HDIB ConvertDIBFormat(HDIB hDIB, UINT nbpp, HPALETTE hPalSrc);
HDIB BitmapToDIB (HBITMAP, HPALETTE); 
HDIB BitmapToDIB (HBITMAP, HPALETTE, WORD);
HDIB ChangeBitmapFormat (HBITMAP, WORD, DWORD, HPALETTE); 
HBITMAP DIBToBitmap (HDIB hDIB, HPALETTE hPal); 
HDIB ChangeDIBFormat (HDIB, WORD, DWORD); 
BOOL CopyColorTable( LPBITMAPINFO lpTarget, LPBITMAPINFO lpSource, HPALETTE hPalSrc );
// DIB ��ɫ��
HPALETTE CreateDIBPalette(LPBYTE lpDIB);
HPALETTE CreateDIBPalette(HDIB hDIB); 
BOOL DisplayPalette(HDC hDC, LPRECT lpRect, HPALETTE hPal);
HPALETTE CopyPalette(HPALETTE hPalSrc);
HPALETTE GetSystemPalette(void); 
int PalEntriesOnDevice (HDC hDC); 
HPALETTE CreateIdentifyPalette(HPALETTE hPalSrc);
BOOL MapDIBColorsToPalette(HDIB hDIB, HPALETTE hPalette);
// DIB ����
HBITMAP CopyScreenToBitmap (LPRECT); 
HBITMAP CopyWindowToBitmap (HWND, WORD); 
HBITMAP CopyClientRectToBitmap(HWND hWnd, LPRECT lpRect);
HDIB CopyScreenToDIB (LPRECT); 
HDIB CopyWindowToDIB (HWND, WORD); 
HDIB CopyClientRectToDIB(HWND hWnd, LPRECT lpRect); 
//Ч����ʾ
void DrawTransparentBitmap(HDC, HBITMAP, LONG, LONG, COLORREF);
BOOL Fade(int nDeltaR, int nDeltaG, int nDeltaB, 
		  BYTE rm, BYTE gm, BYTE bm,
		  BYTE *r, BYTE *g, BYTE *b);
//��ɫ���ӻ�
HPALETTE CreateOctreePalette(HDIB hDIB, UINT nMaxColors, UINT nColorBits);
HPALETTE CreateOctreePalette(LPBYTE lpDIB, UINT nMaxColors, UINT nColorBits);
// ��������
HANDLE CopyHandle(HANDLE h);
void Delay(DWORD dwDelayTime);
// DIBת��
HBITMAP	CropBitmap(HBITMAP, LPRECT);
HDIB CropDIB(HDIB, LPRECT);
HDIB CutDIB(HDIB, LPRECT);
HDIB MergeDIB(HDIB hDib1, HDIB hDib2, POINT ptTopLeft);
HBITMAP RotateBitmapNT(HBITMAP hBitmap, double fDegrees, COLORREF clrBack);
HBITMAP RotateBitmap(HBITMAP hBitmap, double fDegrees, COLORREF clrBack);
HDIB RotateDIB(HDIB hDIB, double fDegrees, COLORREF clrBack);
HDIB RotateDIB(HDIB hDib);
HDIB FlipHorzDIB(HDIB hDib);
HDIB FlipVertDIB(HDIB hDib);
HDIB ChangeDIBSize(HDIB hDIB, int nWidth, int nHeight);
HDIB ChangeDIBCanvasSize(HDIB hDIB, int nWidth, int nHeight, int nPosition);

//////////////////////////////////////////////////////////
#endif	//__DIBAPI_H_