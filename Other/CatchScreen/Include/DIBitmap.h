/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：DIBitmap.h
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：DIB图形处理的类的头文件
*======================================================
* 版本变更记录:
*      v1.0  2002-10-30   20:30  
*
* 
*/

/////////////////////////////////////////////////////////////////////////////

#ifndef DIB256_h
#define DIB256_h

////////////////////////////////////////////////////////////////////////////
/*
*类名    : CDIBitmap
*说明    : DIB图形操作类
*变更记录:
*
*/

class  CDIBitmap
{
	friend class CBmpPalette;

	BITMAPINFO *	m_pInfo;
	BYTE *			m_pPixels;
	CBmpPalette *	m_pPal;
	BOOL			m_bIsPadded;

	public:		//constructors
		CDIBitmap();
		virtual ~CDIBitmap();

	private:
		CDIBitmap( const CDIBitmap& dbmp );

	public:		// attributes
		BITMAPINFO *	GetHeaderPtr() const;
		BYTE *			GetPixelPtr() const;
		RGBQUAD *		GetColorTablePtr() const;
		int				GetWidth() const;
		int				GetHeight() const;
		CBmpPalette *	GetPalette() { return m_pPal; }

	public:		// operations
		BOOL			CreatePalette();	// 创建调色板，在Load()和CreateFromBitmap()中自动导入
		void			ClearPalette();		// 消毁当前位图使用的调色板
		BOOL			CreateFromBitmap( CDC *, CBitmap * );
		BOOL			LoadResource(LPCTSTR ID);
		BOOL            LoadResource(UINT ID);
		BOOL            LoadBitmap(LPCTSTR  lpsID);
		BOOL            LoadBitmap(UINT ID);
		void			DestroyBitmap();
		BOOL			DeleteObject() { DestroyBitmap(); return TRUE; }
		
	public:		// overridables


			// 在指定的位置画位图
		virtual void	DrawDIB( CDC * pDC, int x=0, int y=0 );

			// 画指定的位图和使用拉伸/压缩它到指定的尺寸
		virtual void	DrawDIB( CDC * pDC, int x, int y, int width, int height );

			// 画DIB图形的部份到DC给定的面积
		virtual int		DrawDIB( CDC * pDC, CRect & rectDC, CRect & rectDIB );

			// 从外部文件中导入位图
		virtual BOOL	Load( CFile * pFile );
		virtual BOOL	Load( const CString & );

			//保存DIB位图到磁盘
		virtual BOOL	Save( CFile * pFile );
		virtual BOOL	Save( const CString & );

	protected:
		int				GetPalEntries() const;
		int				GetPalEntries( BITMAPINFOHEADER& infoHeader ) const;
		DWORD			GetBitsPerPixel() const;
		DWORD			LastByte( DWORD BitsPerPixel, DWORD PixelCount ) const;
		DWORD			GetBytesPerLine( DWORD BitsPerPixel, DWORD Width ) const;
		BOOL			PadBits();
		BOOL			UnPadBits();
		WORD			GetColorCount() const;
};

#include "dibpal.h"

#endif	// DIB256_h