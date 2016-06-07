#include "stdafx.h"
#include "CalendarDlg.h"

CCalendarDlg::CCalendarDlg()
{
	m_pCurrChooseYearLabel = NULL ; 
	m_pMonthLabel  = NULL ;
	m_strDateTime =" " ; 
}

CCalendarDlg::~CCalendarDlg()
{

}
LPCTSTR CCalendarDlg::GetWindowClassName() const
{
	return _T("UPGuiFoundation");
}


void CCalendarDlg::Notify(TNotifyUI& msg)
{
	if( _tcsicmp(msg.sType , _T("windowinit")) == 0)
	{
		OnPrepare(msg) ; 
	}
	else if( _tcsicmp(msg.sType , _T("click")) == 0)
	{
		CControlUI *pOne = static_cast<CControlUI*>(m_PaintManager.FindControl(msg.ptMouse)) ;

		if( msg.pSender == m_pBtnNext)
		{
			OnBtnNextYear(msg) ;
		}
		else if( msg.pSender == m_pBtnPrev)
		{
			OnBtnPrevYear(msg) ; 
		}
		else if( msg.pSender == m_pBtnPrevMonth)
		{
			OnBtnPrevMonth(msg) ; 
		}
		else if( msg.pSender == m_pBtnNextMonth )
		{
			OnBtnNextMonth(msg) ; 
		}
		else
		{
			int i = 5 ; 
			for( ; i <= 5 + 41 ; i++)
			{
				CString strNum  ; 
				strNum.Format("%d" ,i) ; 
				CString strName = "ButtonUI" + strNum ; 
				if(  _tcsicmp(pOne->GetName() ,strName) == 0  )
				{

					OnBtnRiliClick(msg ,strName) ;
					::PostMessage(m_hWnd, WM_CLOSE, NULL, NULL);
					break; 
				}
 			}
		}
	}	
}


CDuiString CCalendarDlg::GetSkinFile()
{
	return _T("dlg_rili.xml");
}

CDuiString CCalendarDlg::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

CControlUI* CCalendarDlg::CreateControl(LPCTSTR pstrClass)
{
	//if (_tcsicmp(pstrClass, _T("FaceCtrl")) == 0)
	//	return new CFaceCtrl;
	return NULL;
}

LRESULT CCalendarDlg::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void CCalendarDlg::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
}

LRESULT CCalendarDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_ACTIVATE)
	{
		if (WA_INACTIVE == (UINT)LOWORD(wParam))
			::PostMessage(m_hWnd, WM_CLOSE, NULL, NULL);
	}
	else if (uMsg == WM_CLOSE)
	{
		//::PostMessage(pParenthWnd, FACE_CTRL_SEL, NULL, NULL);
		::DestroyWindow(m_hWnd);
		return 0;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CCalendarDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

void CCalendarDlg::SetParentWnd(HWND hWnd)
{
	pParenthWnd = hWnd;
}

void CCalendarDlg::InitWindow()
{
	m_pMonthLabel = static_cast<CTextUI*>( m_PaintManager.FindControl(_T("TXT_MONTH"))) ; 

	m_pCurrChooseYearLabel = static_cast<CTextUI*>( m_PaintManager.FindControl(_T("TXT_YEAR"))) ; 

	m_pBtnNext = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("BTN_DOWN_YEAR"))) ; 

	m_pBtnPrev = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("BTN_UP_YEAR"))) ;

	m_pBtnPrevMonth = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("BTN_UP_MONTH"))) ;

	m_pBtnNextMonth = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("BTN_DOWN_MONTH"))) ;

}


void CCalendarDlg::OnPrepare(TNotifyUI &msg )
{
	// 获取系统标准时间 
	::GetLocalTime(&m_sysTime) ; 

	// 设置日历背景 
	DrawCalendar(m_sysTime) ;

}

// 根据sysTime设置日期和对应月份的日历  
void CCalendarDlg::DrawCalendar(SYSTEMTIME sysTime)
{
	// 设置日期显示界面 
	CDuiString strYear ,strMonth; 
	strYear.Format("%d" ,sysTime.wYear) ; 
	m_pCurrChooseYearLabel->SetText(strYear) ;
	strMonth.Format("%d",sysTime.wMonth) ; 
	SetMonth(sysTime.wMonth) ; 
	// 设置日期界面结束 

	int iDay = 0;
	char cDay[3] = {0};
	//int iStartDay = m_sysTime.wDayOfWeek;
	sysTime.wDay = 1;
	int iStartDay = GetDayOfWeek(sysTime);
	//上月天数
	int iLastMonthStartDays = 31 - iStartDay;
	if(sysTime.wMonth>1)
		iLastMonthStartDays = GetMonthDays(sysTime.wYear,sysTime.wMonth - 1) - iStartDay;
	//本月天数
	int iMonthDays = GetMonthDays(sysTime.wYear,sysTime.wMonth);
	//下月天数
	int iNextMonthDays = 0;



	CControlUI* pButtonUI;
	CContainerUI* pControl = static_cast<CContainerUI*>(m_PaintManager.FindControl(_T("ContainerUI3")));
	// 创建6 * 7 行 日历日期详细显示数据 
	for(int i = 0;i < 42;i++)
	{
		pButtonUI = m_PaintManager.FindSubControlByClass(pControl,_T("ButtonUI"),i);
		//上月
		if(i<iStartDay)
		{
			iLastMonthStartDays ++;
			sprintf(cDay,"%d",iLastMonthStartDays);
			pButtonUI->SetText(cDay);
			DrawBtnBackImage(pButtonUI,false);
		}
		else if(i>iStartDay-1&&iDay<iMonthDays)
		{
			iDay++;
			sprintf(cDay,"%d",iDay);
			pButtonUI->SetText(cDay);
			DrawBtnBackImage(pButtonUI,true);
		} 
		else 
		{
			iNextMonthDays ++;
			sprintf(cDay,"%d",iNextMonthDays);
			pButtonUI->SetText(cDay);
			DrawBtnBackImage(pButtonUI,false);
		}
	}
}
// 计算当前时间是一个星期的第几天 
int CCalendarDlg::GetDayOfWeek(SYSTEMTIME m_sysTime)
{
	cTime.SetDate(m_sysTime.wYear,m_sysTime.wMonth,m_sysTime.wDay);
	return cTime.GetDayOfWeek()-1;
}



int CCalendarDlg::GetMonthDays(int iY,int iM) 
{
	int iTotalDay = 31;        
	if(iM == 2) 
	{
		//闰年可以被4或者400整除 但是不能被100整除
		if( (iY % 4 ==0 && iY % 100 !=0) || iY % 400 ==0) 
			iTotalDay= 29;
		else
			iTotalDay=28;
	}
	else if(iM==4 || iM==6 || iM==9||iM==11)
		iTotalDay = 30;
	return iTotalDay;
}


void CCalendarDlg::OnBtnNextYear(TNotifyUI& msg)
{
	if(m_pCurrChooseYearLabel)
	{
		CDuiString str = m_pCurrChooseYearLabel->GetText() ; 
		int iYear = atoi(str) + 1 ;
		// 当前年  
		int iCurrYear = m_sysTime.wYear ;

		// 因为查询的是历史消息，年份最大值是当前年  
		if( iYear > iCurrYear )
		{
			iYear = iCurrYear ; 
		}
		str.Format("%d",iYear) ; 
		m_pCurrChooseYearLabel->SetText(str) ; 

		CDuiString strMonth = m_pMonthLabel->GetText() ; 
		int iMonth = GetMonth(strMonth) ; 

		// 构造一个系统时间结构体    
		SYSTEMTIME sysTime ; 
		memset(&sysTime ,0,sizeof(SYSTEMTIME)) ; 
		sysTime.wYear  = (WORD)iYear ;
		sysTime.wMonth = (WORD)iMonth ; 
		sysTime.wDay   = m_sysTime.wDay ;

		DrawCalendar(sysTime) ;
	}
}

void CCalendarDlg::OnBtnPrevYear( TNotifyUI& msg)
{
	if(m_pCurrChooseYearLabel)
	{
		CDuiString str = m_pCurrChooseYearLabel->GetText() ; 
		int iYear = atoi(str) - 1 ; 
		str.Format("%d",iYear) ; 
		m_pCurrChooseYearLabel->SetText(str) ; 

		CDuiString strMonth = m_pMonthLabel->GetText() ; 
		int iMonth = GetMonth(strMonth) ; 

		// 构造一个系统时间结构体    
		SYSTEMTIME sysTime ; 
		memset(&sysTime ,0,sizeof(SYSTEMTIME)) ; 
		sysTime.wYear  = (WORD)iYear ;
		sysTime.wMonth = (WORD)iMonth ; 
		sysTime.wDay   = m_sysTime.wDay ;
		
		// 更新日历 
		DrawCalendar(sysTime) ;
	}
}

void CCalendarDlg::OnBtnPrevMonth(TNotifyUI& msg) 
{
	if (!m_pMonthLabel)
		return ; 

	CDuiString strMonth = m_pMonthLabel->GetText() ;
	//m_pMonthLabel->SetText("一月二月") ; 
	int iMonth = 1 ; 
	if(strMonth == "一月")
	{
		iMonth = 0 ; 
	}
	else if( strMonth == "二月")
	{
		 iMonth = 1 ; 
	}
	else if( strMonth == "三月")
	{
		iMonth = 2 ; 
	}
	else if( strMonth == "四月")
	{
		iMonth = 3 ; 
	}
	else if( strMonth == "五月")
	{
		iMonth = 4 ; 
	}
	else if( strMonth == "六月")
	{
		iMonth = 5 ; 
	}
	else if( strMonth == "七月")
	{
		iMonth = 6 ; 
	}
	else if( strMonth == "八月")
	{
		iMonth = 7 ; 
	}
	else if( strMonth == "九月")
	{
		iMonth = 8 ; 
	}
	else if( strMonth == "十月")
	{
		iMonth = 9 ; 
	}
	else if( strMonth == "十一月")
	{
		iMonth = 10 ; 
	}
	else if( strMonth == "十二月")
	{
		iMonth = 11 ; 
	}

	int iYear = 0 ; 

	// 刷新界面  
	if ( iMonth == 0)
	{
		SetMonth(12) ;
		// 从12往前，变成1月份，年份-1 
		iMonth = 12 ; 
		if( m_pCurrChooseYearLabel)
		{
			CDuiString strYear = m_pCurrChooseYearLabel->GetText() ; 
			iYear = atoi(strYear) - 1 ; 
			CDuiString strPrevYear ; 
			strPrevYear.Format("%d" ,iYear) ; 
			m_pCurrChooseYearLabel->SetText(strPrevYear) ; 
		}
	}
	else
	{
		SetMonth(iMonth) ; 
	}
	// 刷新界面  
	SYSTEMTIME sysTime ; 
	memset(&sysTime ,0,sizeof(SYSTEMTIME)) ; 
	sysTime.wYear  = (WORD)(atoi(m_pCurrChooseYearLabel->GetText())) ; 
	sysTime.wMonth = (WORD)iMonth ; 
	sysTime.wDay   = m_sysTime.wDay ; 
	DrawCalendar(sysTime) ; 
}

void CCalendarDlg::OnBtnNextMonth(TNotifyUI &msg) 
{
	if( !m_pMonthLabel )
		return ; 
	CDuiString strMonth  = m_pMonthLabel->GetText(); 
	int iMonth = GetMonth(strMonth) ;

	int iNextMonth = iMonth + 1 ; 
	

	if( iNextMonth == 13 )
	{
		iNextMonth = 1 ; 
		SetMonth(iMonth) ;
		if(m_pCurrChooseYearLabel)
		{
			CDuiString strYear = m_pCurrChooseYearLabel->GetText() ;
			int iNextYear = atoi(strYear) + 1 ; 
			
			if( iNextYear > m_sysTime.wYear)
			{
				iNextYear = m_sysTime.wYear ; 
			}
			CDuiString strNextYear ;
			strNextYear.Format("%d",iNextYear) ;

			m_pCurrChooseYearLabel->SetText(strNextYear) ; 
			
		}
		 
	}
	else
	{
		SetMonth(iNextMonth) ; 
	}

	int iYear = atoi(m_pCurrChooseYearLabel->GetText()) ; 

	// 刷新日历界面 
	SYSTEMTIME sysTime ; 
	memset(&sysTime,0,sizeof(SYSTEMTIME)) ; 
	sysTime.wYear  = iYear ; 
	sysTime.wMonth = iNextMonth ; 
	sysTime.wDay   = m_sysTime.wDay ;

	DrawCalendar(sysTime) ; 
}


// 选择日期 
void CCalendarDlg::OnBtnRiliClick(TNotifyUI &msg ,CString strControlName)
{
	CControlUI* pButtonUI;
	CContainerUI* pControl = static_cast<CContainerUI*>(m_PaintManager.FindControl(_T("ContainerUI3")));
	
	//pButtonUI = m_PaintManager.FindSubControlByClass(pControl,_T("ButtonUI"),i);

	pButtonUI = m_PaintManager.FindSubControlByName(pControl,strControlName) ; 

	if ( pButtonUI )
	{
		CDuiString strText = pButtonUI->GetText() ; 
		int iDay = atoi(strText) ; 
		
		CString strDay ,strMonth ,strYear ; 
		strDay.Format("%02d",iDay) ; 

		int iMonth ,iYear ; 

		if( m_pMonthLabel )
		{
			iMonth = GetMonth(m_pMonthLabel->GetText()) ; 
			strMonth.Format("%02d",iMonth) ;
		}

		if( m_pCurrChooseYearLabel)
		{
			iYear = atoi( m_pCurrChooseYearLabel->GetText()) ; 
			strYear = m_pCurrChooseYearLabel->GetText().GetData() ; 
		}

		CString strDateTime = strYear + strMonth + strDay ; 

		//SetDateTime(strDateTime) ; 
		SYSTEMTIME dateTime ; 
		memset(&dateTime ,0,sizeof(SYSTEMTIME)) ; 
		dateTime.wYear  = iYear ; 
		dateTime.wMonth = iMonth ; 
		dateTime.wDay   = iDay ;
		SetDateTime(dateTime) ; 

		::SendMessage(pParenthWnd ,WM_CHOOSEDATETIMEINHISTORYRECORD,0,0) ; 

	}
}
// 绘制具体每天日期 
void CCalendarDlg::DrawBtnBackImage(CControlUI* pButtonUI,bool isEnable) 
{
	if(isEnable)
	{
		pButtonUI->SetEnabled(true);
		pButtonUI->SetVisible(true) ; 
		pButtonUI->SetBkColor(RGB(0,255,255)) ; 		
	}
	else
	{
		pButtonUI->SetBkColor(RGB(255,255,255)) ; 
		
		// 置灰  
		pButtonUI->SetEnabled(FALSE) ;
		pButtonUI->SetVisible(false) ; 
	}
}

void CCalendarDlg::SetMonth(int iMonth)
{
	CDuiString strMonth ; 

	switch( iMonth )
	{
	case 1:strMonth = "一月"; break;
	case 2:strMonth = "二月"; break;
	case 3:strMonth = "三月"; break;
	case 4:strMonth = "四月"; break;
	case 5:strMonth = "五月"; break;
	case 6:strMonth = "六月"; break;
	case 7:strMonth = "七月"; break;
	case 8:strMonth = "八月"; break;
	case 9:strMonth = "九月"; break;
	case 10:strMonth = "十月"; break;
	case 11:strMonth = "十一月"; break;
	case 12:strMonth = "十二月"; break;
	//default: strMonth = "十二月" ;break ;  
	}

	if( m_pMonthLabel)
	{
		m_pMonthLabel->SetText(_T(" ")) ;  
		m_pMonthLabel->SetText(strMonth) ; 
	}
}

int CCalendarDlg::GetMonth(CDuiString strMonth) 
{
	
	int iMonth = 0  ; 
	if(strMonth == "一月") iMonth=1 ; 
	else if(strMonth== "二月") iMonth=2; 
	else if(strMonth== "三月") iMonth=3; 
	else if(strMonth== "四月") iMonth=4; 
	else if(strMonth== "五月") iMonth=5; 
	else if(strMonth== "六月") iMonth=6; 
	else if(strMonth== "七月") iMonth=7; 
	else if(strMonth== "八月") iMonth=8; 
	else if(strMonth== "九月") iMonth=9; 
	else if(strMonth== "十月") iMonth=10;
	else if(strMonth== "十一月") iMonth=11;
	else if(strMonth== "十二月") iMonth=12; 
	
	return iMonth ; 
}

void CCalendarDlg::SetDateTime(SYSTEMTIME sysDateTime) 
{
	m_sysChooseDate = sysDateTime ;
}

SYSTEMTIME& CCalendarDlg::GetDateTime() 
{
	return m_sysChooseDate ; 
}