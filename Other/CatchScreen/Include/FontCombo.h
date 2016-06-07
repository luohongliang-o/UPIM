/**
*	File Name			:	FontCombo.h
*	Version				:	1.0
*	Project Information	:	
*	Copyright			:	Copyright 2002 . All Rights Reserved.
*	Brief Description   :	显示字体列表的下拉列表框类
*====================================================================================
*   版本
*   Revision 1.0	2002/11/11	建立CFontCombo类
*/


/*
*类名：CFontCombo
*说明：用于枚举所有已安装字体并用下拉列表框显示
*版本：
*Revision     1.0     2002/11/06       枚举所有已安装字体并用下拉列表框显示
*/

class CFontCombo : public CComboBox
{
// Attribute
public:
	// 字体家族集
	BYTE required_charset;
private:
	CString m_sSelection;


protected:

	
// Construction
public:
	CFontCombo();
	virtual ~CFontCombo();

	// 枚举现有操作系统所安装的所有字体
	static int CALLBACK EnumFontFamProc(ENUMLOGFONT *lpelf, 
					NEWTEXTMETRIC *lpntm,
					int nFontType,
					LPARAM lParam);
	// 获得字体
	void ProcessFonts(void);	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontCombo)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetFontType();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CFontCombo)
	afx_msg void OnSelchange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
