// App.cpp : Defines the entry point for the application.
//
#pragma  once 
#include "stdafx.h"
class CCalendarDlg : public WindowImplBase
{
public :
	CCalendarDlg() ; 
	~CCalendarDlg() ; 

	LPCTSTR GetWindowClassName() const;	
	virtual void InitWindow();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void OnFinalMessage(HWND hWnd);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public :
	// 设置父窗口  
	void SetParentWnd( HWND hWnd ) ; 
	// 绘制日历
	void DrawCalendar(SYSTEMTIME m_sysTime) ; 

	// 返回系统时间m_sysTime在当前月份中第几个星期 
	int GetDayOfWeek(SYSTEMTIME m_sysTime) ; 

	// 计算年Y中M月的天数 
	int GetMonthDays(int iY,int iM) ; 

	
	// 绘制具体每天日期 
	void DrawBtnBackImage(CControlUI* pButtonUI,bool isEnable) ; 

	void SetMonth(int iMonth) ; 

	int GetMonth(CDuiString strMonth) ; 

	void SetDateTime(SYSTEMTIME sysDateTime) ;

	SYSTEMTIME& GetDateTime() ; 

protected:
	void Notify(TNotifyUI& msg) ; 
	void OnPrepare(TNotifyUI &msg ) ; 

	// 下一年 
	void OnBtnNextYear(TNotifyUI& msg) ; 
	// 上一年 
	void OnBtnPrevYear( TNotifyUI& msg ) ; 

	void OnBtnPrevMonth(TNotifyUI& msg) ; 

	// 下一个月 
	void OnBtnNextMonth(TNotifyUI &msg) ;

	// 选择日期 
	void OnBtnRiliClick(TNotifyUI &msg ,CString strControlName) ; 


private:
	// 父窗口句柄 
	HWND pParenthWnd;
	// 系统时间 
	SYSTEMTIME m_sysTime; 
	// 
	COleDateTime cTime;

	CString m_strDateTime ; 

	// 通过日历选择的日期 
	SYSTEMTIME m_sysChooseDate ;

	// 月份 
	//CLabelUI *m_pMonthLabel ; 
	CTextUI *m_pMonthLabel ; 

	// 当前选择的年份 
	CTextUI* m_pCurrChooseYearLabel ; 

	// 下一年 
	CButtonUI *m_pBtnNext ;

	// 上一年 
	CButtonUI *m_pBtnPrev ; 

	// 上一个月 
	CButtonUI *m_pBtnPrevMonth ;

	// 下一个月 
	CButtonUI *m_pBtnNextMonth ; 

};