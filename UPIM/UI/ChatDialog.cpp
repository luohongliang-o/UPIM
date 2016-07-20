#include "stdafx.h"
#include <windows.h>
#include <shellapi.h>

#include "ChatDialog.h"
#include "../globalvar.h"
#include "../../UiLib/Control/UIRichEdit.h"
#include "../LoadDllFunc.h"
#include "../MainPort.h"
#include "AddMember.h"
#include "UIMenu.h"

#pragma  comment (lib,"oledlg.lib")
CFontInfo g_buddyFontInfo;

//主面板
const TCHAR* const kTitleControlName			= _T("apptitle");
const TCHAR* const kBackgroundControlName		= _T("chatbox_bg");
//联系人
const TCHAR* const kLogoButtonControlName		= _T("chatbox_logo");
const TCHAR* const kNickNameControlName			= _T("chatbox_nickname");
const TCHAR* const kDescriptionControlName		= _T("chatbox_description");

const TCHAR* const kFontbarControlName			= _T("chatbox_fontbar");
const TCHAR* const kFontTypeControlName			= _T("chatbox_font_type");
const TCHAR* const kFontSizeControlName			= _T("chatbox_font_size");
const TCHAR* const kBoldButtonControlName		= _T("chatbox_boldbtn");
const TCHAR* const kItalicButtonControlName		= _T("chatbox_italicbtn");
const TCHAR* const KUnderlineButtonControlName	= _T("chatbox_underlinebtn");
const TCHAR* const kColorButtonControlName		= _T("chatbox_colorbtn");
const TCHAR* const kInputRichEditControlName	= _T("chatbox_input_richedit");
const TCHAR* const kViewRichEditControlName		= _T("chatbox_view_richedit");

// wuchao add at 20160408
const TCHAR* const kBYTSButtonControlName       = _T("chatbox_biaoyangtousubtn") ;
const TCHAR* const kSCFXSButtonControlName       = _T("chatbox_shoucangbtn") ;

const int kEmotionRefreshTimerId = 1001;
const int kEmotionRefreshInterval = 150;

ChatDialog::ChatDialog(const CDuiString& bgimage, DWORD bkcolor, const FriendListItemInfo& myselft_info, const FriendListItemInfo& friend_info, EDIALOGTYPE eDialogType_/* =eDIALOG_Single */)
	: bgimage_(bgimage)
	, bkcolor_(bkcolor)
	, myselft_(myselft_info)
	, friend_(friend_info)
	, emotion_timer_start_(false)
	, text_color_(0xFF000000)
	, bold_(false)
	, italic_(false)
	, underline_(false)
	, font_size_(12)
	, font_face_name_(STR_DEFAULT_FONT)
{
	eDialogType = eDialogType_;

	m_pBackground = NULL;
	m_pMinSysBtn = m_pMaxSysBtn = m_pRestoreSysBtn = m_pCloseSysBtn = NULL;
	m_pFontBtn = m_pFaceBtn = m_pImageBtn = m_pScreenshotsBtn = m_pShakeBtn = NULL;
	m_pPinZhongBtn = m_pWenZhangBtn = m_pHisRecordBtn = NULL;	//IM2.0
	m_pRight_search = m_pRight_close = m_pRight_rili = m_pRight_begin = m_pRight_up = m_pRight_down = m_pRight_end = m_pRight_searchok = NULL; //IM2.0 
	m_pFontNameCombo = m_pFontSizeCombo = NULL;
	m_pBoldBtn = m_pItalicBtn = m_pUnderLineBtn = NULL;
	m_pColorBtn = NULL;
	m_pCloseBtn = m_pSendBtn = NULL;
	m_pSendEdit = m_pRecvEdit = NULL;
	m_pControlSwitch = NULL;
	m_pHisRecordEdit = NULL;
	m_pSearchTimeCombo = NULL;
	m_pActiveX_Web = NULL;
	m_pSearchContent = NULL;
	pWebHandle = NULL;

	//right_part
	m_pRoom_note = m_pRoom_member = NULL;
	m_pRoomNoteEdit = NULL;
	pGroupsList = NULL;
	bIsInitWindow = false;

	// tg
	for (int nIndex = 0; nIndex < TG_LAOSHINUM; nIndex++)
		m_pTgHead[nIndex]  = NULL;

	memset(&m_ptRBtnDown, 0, sizeof(m_ptRBtnDown));
	m_strCheckMenu = _T("Menu_ZoomRatio_100");
	m_pLastImageOle = NULL;
	m_cxPicBarDlg = m_cyPicBarDlg = 0;

	SELECTNODE m_selectNode = {0};
	m_PublicSveID = _T("");
	m_pIncodeID = _T("");
	nRouteID = -1;
	nPublicID = -1;
	bsendPubconClose = FALSE;
	pMsg = NULL;
	bControlImg = FALSE;
	bControlImg2 = FALSE;
	bFristReturn = FALSE;
	m_roomid = -1;
	m_analystid = -1;
	m_bRefreshing = FALSE;
	nRightSwitchType = 0;

	// wuchao add 
	// 聊天记录显示的起始和结束位置
	m_nBeginDisplay = 0 ; 
	m_nEndDisplay   = 0 ; 
	m_pBYTSBtn      = NULL ; 
	m_pShouCangBtn  = NULL ; 

	memset(&m_vtInfo ,0 ,sizeof(AnalystInfo)) ; 
}

ChatDialog::~ChatDialog()
{
}

LPCTSTR ChatDialog::GetWindowClassName() const
{
	return _T("UPGuiFoundation");
}

// 不同版本中工具条不一致，但是沿用名称必须一致
// chatbox_fontbtn : 字体
// chatbox_emotionbtn：表情
// chatbox_imagebtn： 图片
// chatbox_shakebtn： 窗口抖动
// chatbox_screenshotsbtn：屏幕截图
// chatbox_pingzhong：品种
// chatbox_wenzhang：文章
// chatbox_hisrecord：聊天记录

void ChatDialog::InitWindow()
{
	m_pBackground = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("chatbox_bg")));
	m_pMinSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_minbtn")));
	m_pMaxSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_maxbtn")));
	m_pRestoreSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_restorebtn")));
	m_pCloseSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_closebtn")));
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_closebtn2")));

	m_pFontBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_fontbtn")));
	m_pFaceBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_emotionbtn")));
	m_pImageBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_imagebtn")));
	m_pShakeBtn  = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_shakebtn")));
	m_pScreenshotsBtn  = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_screenshotsbtn")));

	m_pFontNameCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("chatbox_font_type")));
	m_pFontSizeCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("chatbox_font_size")));
	m_pBoldBtn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("chatbox_boldbtn")));
	m_pItalicBtn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("chatbox_italicbtn")));
	m_pUnderLineBtn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("chatbox_underlinebtn")));
	m_pColorBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_colorbtn")));

	m_pSendBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_sendbtn")));
	m_pSendEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("chatbox_input_richedit")));
	m_pRecvEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("chatbox_view_richedit")));

	//right_part
	m_pRoom_note = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("chatbox_room_note")));
	m_pRoom_member = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("chatbox_room_member")));
	m_pRoomNoteEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("chatbox_note_richedit")));
	pGroupsList = static_cast<CGroupsUI*>(m_PaintManager.FindControl("chatbox_room_memberlist"));

	// IM2.0
	m_pPinZhongBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_pingzhong")));
	m_pWenZhangBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_wenzhang")));
	m_pHisRecordBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_hisrecord")));
	m_pControlSwitch = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("chat_right_switch")));
	m_pHisRecordEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("chatbox_hisrecord_richedit")));

	m_pRight_close = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_right_close")));
	m_pRight_search = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_right_search")));
	m_pRight_rili = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_right_rili")));
	m_pRight_begin = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_right_begin")));
	m_pRight_up = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_right_up")));
	m_pRight_down = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_right_down")));
	m_pRight_end = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_right_end")));
	m_pRight_searchok = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("chatbox_right_searchok")));
	m_pSearchTimeCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("chatbox_right_fanweitime")));
	m_pActiveX_Web =  static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("chatbox_ie")));
	m_pSearchContent =  static_cast<CEditUI*>(m_PaintManager.FindControl(_T("chatbox_right_searchcontent")));
	
	m_pDateTimeLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("chatbox_right_dateTime"))) ; 

	m_pBYTSBtn      = static_cast<CButtonUI*>(m_PaintManager.FindControl(kBYTSButtonControlName));
	m_pShouCangBtn  = static_cast<CButtonUI*>(m_PaintManager.FindControl(kSCFXSButtonControlName));


	// tg
	for (int nIndex = 0; nIndex < TG_LAOSHINUM; nIndex++)
	{
		CString strOptionName = "";
		strOptionName.Format("tg_%d", nIndex);
		m_pTgHead[nIndex]  = static_cast<COptionUI*>(m_PaintManager.FindControl(strOptionName));
	}
	
	//////////////////////////////////////////////////////////////////////////
	SetRichEditCallback(m_pRecvEdit);
	SetRichEditCallback(m_pSendEdit);
	SetRichEditCallback(m_pHisRecordEdit);

	// 初始化字体
	ITextServices * pTextServices = m_pSendEdit->GetTextServices();
	RichEdit_SetDefFont(pTextServices, g_userconfig.m_fontInfo.m_strName.c_str(),
		g_userconfig.m_fontInfo.m_nSize, g_userconfig.m_fontInfo.m_clrText, g_userconfig.m_fontInfo.m_bBold,
		g_userconfig.m_fontInfo.m_bItalic, g_userconfig.m_fontInfo.m_bUnderLine, FALSE);
	pTextServices->Release();

	//////////////////////////////////////////////////////////////////////////
	CControlUI* nick_name = m_PaintManager.FindControl(kNickNameControlName);
	if (nick_name != NULL)
	{
		CString strNameAll = "", strName = "";
		strName.Format("%s", friend_.nick_name.GetData());
		strNameAll.Format("%s(%s)", friend_.nick_name.GetData(), friend_.numid.GetData());

		// 个人聊天才显示"昵称(ID)"
		if (eDialogType == eDIALOG_Single)
			nick_name->SetText(strNameAll);
		else if (eDialogType == eDIALOG_Analys)
		{
#ifdef NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
			CString strUser = ConvertIdToNoPhonrNum(strName);
			nick_name->SetText(strUser);
#else
			nick_name->SetText(strName);
#endif // NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
		}
		else
			nick_name->SetText(strName);
	}

	CControlUI* desciption = m_PaintManager.FindControl(kDescriptionControlName);
	if (desciption != NULL)
	{
		CDuiString strDesciption = _T("");
		if (eDialogType == eDIALOG_GroupSend)
			strDesciption = _T("[此处发送的消息会批量发送至此分组的每一个好友]");
		else if (eDialogType == eDIALOG_Public)
		{
			if (g_MyClient.IsClientLoginIn())
			{
				strDesciption = friend_.description;
				nPublicID = atoi(friend_.numid.GetData());
			}
			else
			{
				CString strTmp = _T("");
				strTmp.Format("*与客户 [%s] 通过公众号对话中", friend_.numid.GetData());
				strDesciption = strTmp;
			}
		}
		else
			strDesciption = friend_.description;
		desciption->SetText(strDesciption);

		// 修改desciption控件的长度，让他不要覆盖
		int nLen = strDesciption.GetLength();
		if (nLen == 0)
			desciption->SetFixedWidth(1);
		else
			desciption->SetFixedWidth(nLen * WLEN_TO_PIX);
	}

	// 隐藏字体栏
	CContainerUI* pFontbar = static_cast<CContainerUI*>(m_PaintManager.FindControl(kFontbarControlName));
	if (pFontbar != NULL)
		pFontbar->SetVisible(!pFontbar->IsVisible());

	//此处加载表情列表配置文件
	m_FaceList.LoadConfigFile(g_config.szFaceConfig);

	// 个人模式
	if (eDialogType == eDIALOG_Single) 
	{

	}
	// 群组模式
	else if (eDialogType == eDIALOG_Group)
	{
		m_pRoomNoteEdit->SetText(friend_.groupnote.GetData());
		//群组聊天禁止窗口抖动
		m_pShakeBtn->SetVisible(false);
	}
	else if (eDialogType == eDIALOG_Public)
	{
		//公众号聊天禁止窗口抖动
		m_pShakeBtn->SetVisible(false);

		CActiveXUI* pActiveXUI = static_cast<CActiveXUI*>(m_PaintManager.FindControl(_T("ie")));
		if( pActiveXUI ) 
		{
			IWebBrowser2* pWebBrowser = NULL;
			pActiveXUI->GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);
			if( pWebBrowser != NULL ) 
			{
				//pWebBrowser->Navigate((BSTR)(wstrFilePath.c_str()),NULL,NULL,NULL,NULL); 
				//pWebBrowser->Navigate(L"http://www.baidu.com",NULL,NULL,NULL,NULL); 
				CString strPath = _T("");
				strPath.Format("%s\\display\\display.html", g_config.szHomePath);
				BSTR bsName = strPath.AllocSysString();
				pWebBrowser->Navigate(bsName, NULL, NULL, NULL, NULL); 
				pWebBrowser->Release();
				SysFreeString(bsName);
			}
		}
	}
	else if (eDialogType == eDIALOG_GroupSend)
	{
		if (m_vtUPIMID.size() > 0)
		{
			LoadTeamMember();
		}
	}
	else if (eDialogType == eDIALOG_ChatTG)
	{
		CActiveXUI* pActiveXUI = static_cast<CActiveXUI*>(m_PaintManager.FindControl(_T("ie2")));
		if( pActiveXUI ) 
		{
			IWebBrowser2* pWebBrowser = NULL;
			pActiveXUI->GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);
			if( pWebBrowser != NULL ) 
			{
				CString strPath = _T("");
				strPath.Format("%s\\display\\1.html", g_config.szHomePath);
				BSTR bsName = strPath.AllocSysString();
				pWebBrowser->Navigate(bsName, NULL, NULL, NULL, NULL); 
				pWebBrowser->Release();
				SysFreeString(bsName);
			}
		}
	}
	else if (eDialogType == eDIALOG_Analys)
	{
		// 设置右侧介绍页网址，版本不同页面url不一样
		if( m_pActiveX_Web ) 
		{
			m_pActiveX_Web->SetDelayCreate(false);
			pWebHandle = new CCustomWebEventHandler;
			m_pActiveX_Web->SetWebBrowserEventHandler(pWebHandle);
			m_pActiveX_Web->Navigate2("about:blank");    //这行代码，如果注释掉，就不会去掉边框，IE有bug，第二次加载网页才会让事件处理器有效

			CString strURL = _T("");
			// 如果是分析师版本的聊天界面，看见的是客户的持仓或者交易信息
			if (VER_UPIM_NUMS == 3)
			{
				//http://upia.upchinafund.com/Home/im/userinfo.html?userName=15932043841
				strURL.Format("http://upia.upchinafund.com/Home/im/userinfo.html?userName=%s",friend_.numid);
			}
			// 如果是客户版本的聊天界面，看见的是分析师的行业
			else if (VER_UPIM_NUMS == 4)
			{
				//http://upia.upchinafund.com/Home/im/introduces.html?analystId=1150
				strURL.Format("http://upia.upchinafund.com/Home/im/introduces.html?analystId=%s",friend_.numid);
			}
			m_pActiveX_Web->Navigate2(strURL);
		}

		// 设置头像
		if (VER_UPIM_NUMS == 3)
		{
			// 看客户是否下载了头像，如果下载了，就显示客户自己的头像
			//FriendList_ITER findIt ; 
			CButtonUI* log_button = static_cast<CButtonUI*>(m_PaintManager.FindControl(kLogoButtonControlName));
			if (log_button != NULL)
			{
				CDuiString numid = friend_.numid ; 
				CString strHead = "";		// 在线的正常头像
				CString strHead2 = "";		// 离线时的灰化头像
				strHead.Format("%s\\%s_n.jpg", g_config.szUserHead, numid);
				strHead2.Format("%s\\%s_g.jpg", g_config.szUserHead, numid);

				if( friend_.nstate)
				{
					log_button->SetNormalImage(strHead);
				}
				else
				{
					log_button->SetNormalImage(strHead2);
				}
			}
		}
		else if (VER_UPIM_NUMS == 4)
		{
			// 显示分析师的头像
			CButtonUI* log_button = static_cast<CButtonUI*>(m_PaintManager.FindControl(kLogoButtonControlName));
			if (log_button != NULL)
			{
				CString strRoomId = friend_.Parentid ;
				int nRoomID       = atoi( strRoomId.GetBuffer()) ; 
				ALLCLIENT_ROOM_INFO pAnalysNode ; 
				FindAnalysInRoom(nRoomID,pAnalysNode) ;

				int nAnalysID = atoi(friend_.numid.GetData());
				CString strHead = "";		// 在线的正常头像
				CString strHead2 = "";		// 离线时的灰化头像
				strHead.Format("%s\\%d_n.jpg", g_config.szUserHead, nAnalysID);
				strHead2.Format("%s\\%d_g.jpg", g_config.szUserHead, nAnalysID);

				int iSize = pAnalysNode.pVtAnalys->size() ; 
				ROOMANALYS_INFO info ; 

				for( int i = 0 ; i < iSize ;i++)
				{
					bool is1 = (*pAnalysNode.pVtAnalys)[i].m_bIsOnLine ; 
					if( strcmp((*pAnalysNode.pVtAnalys)[i].m_szAnalysName ,friend_.nick_name.GetData()) == 0 )
					{
						if( (*pAnalysNode.pVtAnalys)[i].m_bIsOnLine)
						{
							log_button->SetNormalImage(strHead);
						}
						else
						{
							log_button->SetNormalImage(strHead2);
						}
						break; 
					}
				}
			}

			// 设置擅长品种
			// wuchao modify at 2016-03-28  
			CDuiString strDesciption = _T("");
			strDesciption = friend_.GoodAt;
			desciption->SetText(friend_.GoodAt);

			// 修改desciption控件的长度，让他不要覆盖
			int nLen = strDesciption.GetLength();
			if (nLen == 0)
				desciption->SetFixedWidth(1);
			else
				desciption->SetFixedWidth(nLen * WLEN_TO_PIX);
		}
	}
	else if (eDIALOG_RYPublic == eDialogType)
	{
		RecvPublicInfo();
	}

	bIsInitWindow = true;
	return;
}

void ChatDialog::SetRichEditCallback(CRichEditUI* m_pEdit)
{
	IRichEditOleCallback2* pRichEditOleCallback = NULL;
	HRESULT hr = ::CoCreateInstance(CLSID_ImageOle, NULL, CLSCTX_INPROC_SERVER, __uuidof(IRichEditOleCallback2), (void**)&pRichEditOleCallback);
	if (SUCCEEDED(hr))
	{
		pRichEditOleCallback->SetNotifyHwnd(m_hWnd);
		ITextServices * pTextServices = m_pEdit->GetTextServices();
		pRichEditOleCallback->SetTextServices(pTextServices);
		pTextServices->Release();
		m_pEdit->SetOleCallback(pRichEditOleCallback);
		pRichEditOleCallback->Release();
	}
	else
		USER_LOG("# SETRICHEDITCALLBACK NOT SUCCEEDED 1 [%x]", hr);

	if (m_pEdit == m_pSendEdit)
	{
		// 对于Edit添加拖拽响应
		IDropTarget *pdt = m_pEdit->GetTxDropTarget();
		hr = ::RegisterDragDrop(m_hWnd, pdt);
		pdt->Release();
	}
}

void ChatDialog::OnTimer(TNotifyUI& msg)
{
	switch(msg.wParam)
	{
	case T_CHATBOX:
		{
			time_t tmThis = time(NULL);
			double timedif = difftime(tmThis, tmStart_);
			if (timedif > (double)30.00 && !bsendPubconClose)
			{			
				//客户关闭对话窗口
				g_MyClient.SendPubconCloseReq(m_PublicSveID, nPublicID, nRouteID, 1);
				bsendPubconClose = TRUE;	
			}
		}
		break;
	case T_REFRESHSTATUS:
		{
			GetAllFXSState();
		}
		break;
	case T_REFRESH:
		{
			OnRefreshPic();
		}
		break;
	case T_RECORDSHOW:
		{
			KillTimer(T_RECORDSHOW);
			ShowRecord();
		}
		break;
	default:
		break;
	}
}

BOOL ChatDialog::Receive(SkinChangedParam param)
{
	bgimage_ = param.bgimage;
	bkcolor_ = param.bkcolor;
	if (m_pBackground != NULL)
	{
		if (!param.bgimage.IsEmpty())
		{
			TCHAR szBuf[MAX_PATH] = {0};
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' corner='600,200,1,1'"), param.bgimage);
			m_pBackground->SetBkImage(szBuf);
		}
		else
			m_pBackground->SetBkImage(_T(""));

		m_pBackground->SetBkColor(param.bkcolor);
	}

	return TRUE;
}

CDuiString ChatDialog::GetSkinFile()
{
	// 个人聊天
	if (eDialogType == eDIALOG_Single)
		return _T("dlg_ChatBox.xml");
	// 群组聊天
	else if (eDialogType == eDIALOG_Group)
		return _T("dlg_GroupChatBox.xml");
	// 公众号
	else if (eDialogType == eDIALOG_Public)
		return _T("dlg_Public.xml");
	// 消息群发，讨论组
	else if (eDialogType == eDIALOG_GroupSend || eDialogType == eDIALOG_Team)
		return _T("dlg_Team.xml");

	else if (eDialogType == eDIALOG_ChatTG)
		return _T("dlg_ChatBox_TG.xml");

	else if (eDialogType == eDIALOG_Analys)
	{
		if (VER_UPIM_NUMS == 3)
			return _T("dlg_ChatBox.xml");
		if (VER_UPIM_NUMS == 4)
			return _T("dlg_ChatBox_Client.xml");
	}
	
	return _T("dlg_ChatBox.xml");
}

CDuiString ChatDialog::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

CControlUI* ChatDialog::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("MemberList")) == 0)
	{
		return new CGroupsUI(m_PaintManager, eUI_Member);
	}
	return NULL;
}

LRESULT ChatDialog::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN) 
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		// 投顾客户端，屏蔽ESC消息
		if (eDialogType != eDIALOG_ChatTG)
		{
			//窗口关闭，将保存的全局用户id-窗口句柄对应关系清除
			allopenwnd[friend_.id] = NULL;			
			Close();
			return TRUE;
		}
	}
	return FALSE;
}

LRESULT ChatDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		if (!bZoomed)
		{
			if (m_pMaxSysBtn != NULL)
				m_pMaxSysBtn->SetVisible(false);
			if (m_pRestoreSysBtn != NULL)
				m_pRestoreSysBtn->SetVisible(true);
		}
		else 
		{
			if (m_pMaxSysBtn != NULL)
				m_pMaxSysBtn->SetVisible(true);
			if (m_pRestoreSysBtn != NULL)
				m_pRestoreSysBtn->SetVisible(false);
		}
	}
	return 0;
}

LRESULT ChatDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::RevokeDragDrop(m_hWnd);
	m_FaceList.Reset();
	bHandled = FALSE;
	return 0;
}

void ChatDialog::OnFinalMessage(HWND hWnd)
{
	RemoveObserver();
	WindowImplBase::OnFinalMessage(hWnd);	
	// wuchao add at 2016-03-25  
	// 解决XP环境下 开启多人聊天对话框  
	// 关掉任意一个  客户端以及其他聊天对话框 都关闭  
	// 原因不详  以后遇到其他问题再解决！！！！  
	//delete this;
}

LRESULT ChatDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_MOUSEMOVE)
	{
		//OnMouseMove(uMsg, wParam, lParam);
	}
	else if (uMsg == WM_CLOSE)
	{
		allopenwnd[friend_.id] = NULL;			
		Close();
	}
	else if (uMsg == WM_RBUTTONDOWN)
	{
		LRESULT lRes = __super::HandleMessage(uMsg, wParam, lParam);
		OnRButtonDown(uMsg, wParam, lParam);
		return lRes;
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		LRESULT lRes = __super::HandleMessage(uMsg, wParam, lParam);
		OnLButtonDown(uMsg, wParam, lParam);
		return lRes;
	}
	else if (uMsg == WM_LBUTTONDBLCLK)
	{
		OnLButtonDblClk(uMsg, wParam, lParam);
	}
	else if (uMsg == WM_NOTIFY )
	{
		if (EN_PASTE == ((LPNMHDR)lParam)->code)
		{
			ITextServices * pTextServices = m_pSendEdit->GetTextServices();
			if ((UINT)pTextServices == ((LPNMHDR)lParam)->idFrom)
			{
				LPWSTR lpzData = ((NMRICHEDITOLECALLBACK *)lParam)->lpszText;
				LPCTSTR lpText = UnicodeToAnsi(lpzData);
				AddMsgToSendEdit(lpText, TRUE);
				delete lpText;
			}
			if (pTextServices != NULL)
				pTextServices->Release();
		}
		else if (EN_LINK == ((LPNMHDR)lParam)->code)
		{
			// 截获richedit的Link消息，在左键点击的情况下响应消息
			ENLINK *pEnLink = (ENLINK *)lParam;
			if(pEnLink->msg == WM_LBUTTONDOWN)  
			{ 
				CDuiString strLink = m_pRecvEdit->GetTextRange(pEnLink->chrg.cpMin, pEnLink->chrg.cpMax);
				OnRichEditLink(strLink);
			}
		}
	}
	else if (uMsg == UM_FEEDBACKMSG)
	{
		int nMsgNum = (int)wParam;
		if (nMsgNum == 3)
		{
			m_pSendEdit->PasteSpecial(CF_TEXT);
			return TRUE;
		}
	}
	else if (uMsg == FACE_CTRL_SEL)
	{
		return OnFaceCtrlSel(uMsg, wParam, lParam);
	}
	else if ( uMsg == WM_CHOOSEDATETIMEINHISTORYRECORD )
	{
		return OnChooseHisRecordByDateTime(uMsg,wParam ,lParam) ; 
	}
	// 发送消息框的Ctrl+V消息
// 	if ((m_pSendEdit != NULL) && m_pSendEdit->IsFocused() && ((uMsg == WM_KEYDOWN) && (wParam == 'V') && (lParam & VK_CONTROL)))	
// 	{
// 		m_pSendEdit->PasteSpecial(CF_TEXT);
// 		return TRUE;
// 	}


	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT ChatDialog::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	switch (uMsg)
	{
	case UM_USER_ADDROOMMEMBER:
		OnLoadRoomList(wParam, lParam);
		break;
	case UM_USER_PUBLICINFO:
		RecvPublicInfo();
		break;
	case UM_USER_RECVMSG:
		InsertRecvMsg();
		break;
	case UM_USER_HISMSG:
		InsertHisMsg();
		break;
	case UM_USER_HISANDRECVMSG:
		{
			InsertHisMsg();
			InsertRecvMsg();
			break;
		}
	case UM_USER_RIGHTHISMSG:
		ShowRightHisRecord();
		break;
	case UM_USER_SHOWRECORD:
		ShowRecord();
		break;
	case UM_USER_UISETINITSIZE:
		SetUIInitSize();
		break;
	case WM_MENUCLICK:
		OnMenuClick(wParam, lParam);
		break;
	case UM_USER_USERSTATE:
		OnUserState(wParam, lParam);
		break;

	case WM_KEYUP:
		OnKeyUp(uMsg,wParam ,lParam,bHandled) ;
		break ; 
	default:
		bHandled = FALSE;
		break;
	}
	return 0;
}

void ChatDialog::OnUserState(WPARAM wParam , LPARAM lParam)
{
	int nState = (int)wParam;
	CString *pStrName = (CString *)lParam ; 

	CButtonUI* log_button = static_cast<CButtonUI*>(m_PaintManager.FindControl(kLogoButtonControlName));
	if (log_button != NULL)
	{
		CDuiString strIMG = "";
		CDuiString strOnlinePic = _T(" ") ,strOfflinePic = _T(" ") ;

		strOnlinePic.Format(_T("%s\\%s_n.jpg"), g_config.szUserHead,  pStrName->GetString());
		strOfflinePic.Format(_T("%s\\%s_g.jpg"), g_config.szUserHead,  pStrName->GetString());
		
		// wuchao modify at 2016-03-26  设置对话框中头像的显示 
		if( (GetFileAttributes(strOnlinePic.GetData()) != -1) || (GetFileAttributes(strOfflinePic.GetData()) != -1) )
		{
			if (nState == 1)
				strIMG = strOnlinePic;
			else
				strIMG = strOfflinePic;
		}
		else
		{
			if( nState == 1 ) 
				strIMG = _T("public.png") ; 
			else
				strIMG = _T("public_g.png") ; 
		}
		friend_.nstate = nState;
		log_button->SetNormalImage(strIMG.GetData());
	}

	delete pStrName  ;
	return;
}

void ChatDialog::OnMenuClick(WPARAM wParam , LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	CDuiString *strMenuName = (CDuiString*)wParam;
	bool bChecked = (lParam == 1);		 
	TNotifyUI msg;

	if (*strMenuName == _T("Menu_Cut"))
		OnMenu_Cut(msg);
	else if (*strMenuName == _T("Menu_Copy_Recv"))
		OnMenu_Copy_Recv();
	else if (*strMenuName == _T("Menu_Copy_Send"))
		OnMenu_Copy_Send();
	else if (*strMenuName == _T("Menu_Paste"))
		OnMenu_Paste(msg);
	else if (*strMenuName == _T("Menu_SelAll_Recv"))
		OnMenu_SelAll_Recv();
	else if (*strMenuName == _T("Menu_SelAll_Send"))
		OnMenu_SelAll_Send();
	else if (*strMenuName == _T("Menu_Clear"))
		OnMenu_Clear(msg);
	else if (*strMenuName == _T("Menu_ZoomRatio_400") 
		|| *strMenuName == _T("Menu_ZoomRatio_200") 
		|| *strMenuName == _T("Menu_ZoomRatio_150")
		|| *strMenuName == _T("Menu_ZoomRatio_125") 	
		|| *strMenuName == _T("Menu_ZoomRatio_100") 	
		|| *strMenuName == _T("Menu_ZoomRatio_75") 
		|| *strMenuName == _T("Menu_ZoomRatio_50"))
		OnMenu_ZoomRatio((*strMenuName).GetData());

	else if (*strMenuName == _T("Menu_SaveAs_Recv"))
		OnMenu_SaveAs(0);
	else if (*strMenuName == _T("Menu_SaveAs_Send"))
		OnMenu_SaveAs(1);
	else if (*strMenuName == _T("Menu_Font"))
		m_pFontBtn->Activate();
	else if (*strMenuName == _T("Group_Mem_Send"))
		RoomMember_ChooseChat(msg);
	else if (*strMenuName == _T("Group_Mem_Add"))
		RoomMember_Add(msg);
	else if (*strMenuName == _T("Group_Mem_Del"))
		RoomMember_Del(msg);
	TDEL(strMenuName);
	return;
}

// 公众号中的文本超链接
void ChatDialog::OnRichEditLink(CDuiString strLink)
{
#ifndef VER_UPIM_RONGYUN
	PUB_ITER iter_ = m_mapPublicInfo.find(atoi(friend_.numid));
	// 搜寻问题路由，看点击的是哪个问题
	if (iter_->second.pVtRoute != NULL)
	{
		std::vector<ROUTE_INFO>::iterator iter_info;
		for (iter_info = iter_->second.pVtRoute->begin(); iter_info != iter_->second.pVtRoute->end(); iter_info++)
		{
			CString strRName = _T("");
			strRName.Format("%s", iter_info->m_RName);
			if (-1 != strLink.Find(strRName))
			{
				CString strTmp = _T("");
				strTmp.Format("  您选择的是[%s]，请直接输入您的问题，我们将为您转接服务专员。", strRName, iter_info->nID);
				nRouteID = iter_info->nID;
				AddMsgToRichEdit_More(m_pRecvEdit, strTmp, g_userconfig.m_fontInfo, false, false, friend_.id.GetData(), friend_.nick_name.GetData(), "");
			}
		}
	}
#else
	map<CString,NEWPUBLIC_INFO>::iterator iter_ = g_mapNewPublicInfo.find(friend_.numid.GetData());
	if (iter_->second.pIncode != NULL)
	{
		std::vector<IncodeInfo>::iterator iter_info;
		for (iter_info = iter_->second.pIncode->begin(); iter_info != iter_->second.pIncode->end(); iter_info++)
		{
			CString strRName = _T("");
			strRName.Format("%s", iter_info->accessname);
			if (-1 != strLink.Find(strRName))
			{
				CString strTmp = _T("");
				strTmp.Format("  您选择的是[%s]，请直接输入您的问题，我们将为您转接服务专员。", strRName, iter_info->accesscode);
				m_pIncodeID = iter_info->accesscode;
				AddMsgToRichEdit_More(m_pRecvEdit, strTmp, g_userconfig.m_fontInfo, false, false, friend_.id.GetData(), friend_.nick_name.GetData(), "");
			}
		}
	}
#endif

}

void ChatDialog::AddTimer(UINT nTimerID, UINT uElapse)
{
	m_PaintManager.SetTimer(m_pBackground, nTimerID, uElapse);
}

void ChatDialog::KillTimer(UINT nTimerID)
{
	m_PaintManager.KillTimer(m_pBackground, nTimerID);
}

void ChatDialog::UISetInitSize()
{
	CRect rectInit(0, 0, 0, 0);
	if(g_hVessel)	
		::GetClientRect(g_hVessel, &rectInit);
	
	if (m_PaintManager.GetPaintWindow() != NULL)
	{
		m_PaintManager.GetRoot()->SetPos(rectInit);
		::MoveWindow (m_hWnd, rectInit.left, rectInit.top, rectInit.right-rectInit.left, rectInit.bottom-rectInit.top, TRUE);
		//TRACE("ChatDialog::UISetInitSize\n");
	}
}

void ChatDialog::SetUIInitSize()
{
	if (g_pCreateTG1)
	{
		g_pCreateTG1->UISetInitSize();
	}
	if (g_pCreateTG2)
	{
		g_pCreateTG2->UISetInitSize();
	}
	return;
}


// 加载消息群发，或者讨论组的成员列表
void ChatDialog::LoadTeamMember()
{
	if (pGroupsList != NULL)
	{
		for (int nIndex = 0; nIndex<(int)m_vtUPIMID.size(); nIndex++)
		{
			// 如果列表中没有这个成员，则添加
			Node* mNode = pGroupsList->SelectNodeByID((CString)m_vtUPIMID[nIndex].m_szUSERID);
			if (mNode == NULL)
			{
				FriendListItemInfo item;
				item.nodetype = eNode_Single;
				item.folder = false;
				item.id = m_vtUPIMID[nIndex].m_szUSERID;
				item.logo = _T("default.png");
				item.nick_name = alluserNickname[item.id];
				if (item.nick_name.IsEmpty())
					item.showname = m_vtUPIMID[nIndex].m_szUSERID;
				else
					item.showname.Format("%s(%s)", item.nick_name.GetData(), item.id.GetData());

				item.description = _T("");
				pGroupsList->AddNode(item, NULL);
			}
		}
	}
}

LRESULT ChatDialog::OnLoadRoomList(WPARAM wParam, LPARAM lParam)
{
	//////////////////////////////////////////////////////////////////////////
	if (pGroupsList != NULL)
	{
		GCR_ITER iter_ = m_mapChatRoom.find(atoi(friend_.numid));
		std::vector<GUSER_INFO>::iterator iter_info;
		for (iter_info = iter_->second.pVtUser->begin(); iter_info != iter_->second.pVtUser->end(); iter_info++)
		{
			// 如果群组列表中没有这个成员，则添加
			Node* mNode = pGroupsList->SelectNodeByID(iter_info->m_szIMID);
			if (mNode == NULL)
			{
				FriendListItemInfo item;
				item.nodetype = eNode_Single;
				item.folder = false;
				item.id = iter_info->m_szIMID;
				item.logo = _T("default.png");
				item.nick_name = alluserNickname[item.id];
				if (item.nick_name.IsEmpty())
					item.showname = iter_info->m_szIMID;
				else
					item.showname.Format("%s(%s)", item.nick_name.GetData(), item.id.GetData());
				item.rusertype = iter_info->m_nUserType;
				item.description = _T("");
				pGroupsList->AddNode(item, NULL);
			}
		}
	}
	return 1;
}

void ChatDialog::OnExit()
{
	// 释放pMsg
	if ( pMsg )
	{
		for(int i=0; i<(int)pMsg->size(); i++)
		{
			TDELARRAY( (*pMsg)[i].msg );
		}
		TDEL(pMsg);
	}

	// 释放m_arrII
	m_arrII.RemoveAll();

	// 清除pWebHandle
	if (pWebHandle)
		TDEL(pWebHandle);

	allopenwnd[friend_.id] = NULL;			//窗口关闭，将保存的全局用户id-窗口句柄对应关系清除
	Close();
}

void ChatDialog::OnPrepare(TNotifyUI& msg)
{
	TCHAR szBuf[MAX_PATH] = {0};

	// 设置窗口背景图片
	if (m_pBackground != NULL)
	{
		if (VER_UPIM_NUMS == 3 || VER_UPIM_NUMS == 4)
		{
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='chat_back.png' corner='600,200,1,1'"));
			m_pBackground->SetBkImage(szBuf);
		}
		else
		{
			if (eDialogType == eDIALOG_ChatTG)
			{
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='chattg.jpg' corner='600,200,1,1'"));
				m_pBackground->SetBkImage(szBuf);
			}
			else
			{
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' corner='600,200,1,1'"), bgimage_);
				m_pBackground->SetBkImage(szBuf);
				m_pBackground->SetBkColor(bkcolor_);
			}
		}
	}

	// 设置聊天窗口左上角的图标
	CButtonUI* log_button = static_cast<CButtonUI*>(m_PaintManager.FindControl(kLogoButtonControlName));
	if (log_button != NULL)
	{
		if (eDialogType == eDIALOG_GroupSend || eDialogType == eDIALOG_Team)
			_stprintf_s(szBuf, MAX_PATH - 1, _T("default5.png"));
		else
			_stprintf_s(szBuf, MAX_PATH - 1, _T("%s"), friend_.logo);
		log_button->SetBkImage(szBuf);
	}

	// 光标移动到输入框
	m_pSendEdit->SetSel(-1, -1);
	m_pSendEdit->ReplaceSel(_T(""), false);
	m_pBackground->SetFocus();
	m_pSendEdit->SetFocus();
	tmStart_ = time(NULL);
#ifndef VER_UPIM_RONGYUN
	if (g_MyClient.IsClientLoginIn() && eDialogType == eDIALOG_Public)
	{
		AddTimer(T_CHATBOX, eTE_Chatbox);
	}
#endif
	if (eDialogType == eDIALOG_ChatTG)
	{
		AddTimer(T_REFRESHSTATUS, eTE_RefreshStatus);
	}

	if (eDialogType == eDIALOG_Analys)
	{
		if (VER_UPIM_NUMS == 3 || VER_UPIM_NUMS == 4)
		{
			PostMessage(UM_USER_RECVMSG, 0, 0);
		}
		else
			PostMessage(UM_USER_HISANDRECVMSG, 0, 0);

	}
	else
	{
		// 填充收到的消息
		//InsertRecvMsg();
		PostMessage(UM_USER_RECVMSG, 0, 0);
	}
}

//sqlite测试
void ChatDialog::TestSqlite()
{
	CString strSql;
	strSql.Format("select * from chatrecord where toid='069000000412' and type=0 order by seq asc limit 0,100");
	CDbSQLite::ResultTable res;
	BOOL brtn = g_SqliteOp.m_sqlite.SelectStatement(strSql, res);
	int nRecord = res.m_records.size();

	for (int nindex = 0; nindex <nRecord; nindex++)
	{
		CDbSQLite::ResultRecord *record = res.next();
		if (record)
		{
			CString strseq = record->m_fields[0];
			CString strseq2 = record->m_fields[1];
			CString strseq3 = record->m_fields[2];
			CString strseq4 = record->m_fields[3];
			CString strseq5 = record->m_fields[4];
			CString strseq6 = record->m_fields[5];
			CString strseq7 = record->m_fields[6];
			CString strseq8 = record->m_fields[7];
			CString strseq9 = record->m_fields[8];
		}
	}
}

CDuiString GetCurrentTimeString()
{
	SYSTEMTIME time = {0};
	TCHAR szTime[MAX_PATH] = {0};
	::GetLocalTime( &time );
	_stprintf_s( szTime, MAX_PATH, _T("%04d年%02d月%02d日 %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return szTime;
}

//消息循环
void ChatDialog::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("killfocus")) == 0)
	{
	}
	else if (_tcsicmp(msg.sType, _T("link")) == 0)
	{
	}
	else if(msg.sType == _T("setfocus"))
	{
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (msg.pSender == m_pMinSysBtn)
			OnBtn_Min(msg);
		else if (msg.pSender == m_pMaxSysBtn)
			OnBtn_Max(msg);
		else if (msg.pSender == m_pRestoreSysBtn)
			OnBtn_Restore(msg);
		else if (msg.pSender == m_pCloseSysBtn)
			OnBtn_Close(msg);
		else if (msg.pSender == m_pCloseBtn)
			OnBtn_Close(msg);
		else if (msg.pSender == m_pBoldBtn)
			OnBtn_Bold(msg);
		else if (msg.pSender == m_pItalicBtn)
			OnBtn_Italic(msg);
		else if (msg.pSender == m_pUnderLineBtn)
			OnBtn_UnderLine(msg);
		else if (msg.pSender == m_pColorBtn)
			OnBtn_Color(msg);
		else if (msg.pSender == m_pSendBtn)
			SendMsg_More();
		else if (msg.pSender == m_pFontBtn)
			OnBtn_Font(msg);
		else if (msg.pSender == m_pFaceBtn)
			OnBtn_Face(msg);
		else if (msg.pSender == m_pImageBtn)
			OnBtn_Image(msg);
		else if (msg.pSender == m_pScreenshotsBtn)
			OnBtn_Screenshots(msg);
		else if (msg.pSender == m_pShakeBtn)
			OnBtn_Shake(msg);

		//IM2.0
		else if (msg.pSender == m_pPinZhongBtn)
			OnBtn_PinZhong(msg);
		else if (msg.pSender == m_pHisRecordBtn)
			OnBtn_HisRecord(msg);
		else if (msg.pSender == m_pWenZhangBtn)
			OnBtn_WenZhang(msg);
		else if (msg.pSender == m_pRight_search)
			OnBtn_Right_SearchBar(msg);
		else if (msg.pSender == m_pRight_rili)
			OnBtn_Right_RiLi(msg);
		else if (msg.pSender == m_pRight_begin)
			OnBtn_Right_PageBegin(msg);
		else if (msg.pSender == m_pRight_up)
			OnBtn_Right_PageUp(msg);
		else if (msg.pSender == m_pRight_down)
			OnBtn_Right_PageDown(msg);
		else if (msg.pSender == m_pRight_end)
			OnBtn_Right_PageEnd(msg);
		else if (msg.pSender == m_pRight_searchok)
			OnBtn_Right_SearchOK(msg);
		else if (msg.pSender == m_pRight_close)
			OnBtn_Right_Close(msg);
		else if(msg.pSender == m_pBYTSBtn)
		{
			// 表扬投诉分析师
			OnBtn_BYTSAnalyst(msg) ; 
		}
		else if( msg.pSender == m_pShouCangBtn)
		{
			// 收藏分析师
			OnBtn_ShouCangAnalyst(msg) ; 
		}
		for (int nIndex = 0; nIndex<TG_LAOSHINUM; nIndex++)
		{
			// 如果是分析师头像的SETFOCUS
			if (msg.pSender == m_pTgHead[nIndex])
			{
				TRACE("click - %d \n", nIndex);
				ChangeLaoshi(nIndex);
			}
		}

	}
	//else if( _tcsicmp(msg.sType, _T("return")) == 0 ) 
	//{
	//	if (_tcsicmp(msg.pSender->GetName(), kInputRichEditControlName) == 0)
	//	{
	//		SendMsg_More();
	//	}
	//}
	else if (_tcsicmp(msg.sType, _T("itemactivate")) == 0)
	{
		RoomMemberClick(msg);
	}
	else if (msg.sType == _T("itemselect"))
	{
		if (msg.pSender == m_pFontNameCombo)
			OnCbo_SelChange_FontName(msg);
		else if (msg.pSender == m_pFontSizeCombo)
			OnCbo_SelChange_FontSize(msg);
	}
	else if (msg.sType == _T("itemclick"))
	{
		RoomMember(msg);
	}
	else if (msg.sType == _T("rbuttondown"))
	{
		RoomMember(msg);
		RoomMemberRClick(msg);
	}
	else if (msg.sType == _T("timer"))
	{
		OnTimer(msg);
	}
	
}

//最小化按钮
void ChatDialog::OnBtn_Min(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

//最大化按钮
void ChatDialog::OnBtn_Max(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

//还原窗口按钮
void ChatDialog::OnBtn_Restore(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
}

//关闭窗口按钮
void ChatDialog::OnBtn_Close(TNotifyUI& msg)
{
	//关闭定时器
	m_PaintManager.KillTimer(m_pBackground);

	if (g_MyClient.IsClientLoginIn() && eDialogType == eDIALOG_Public)
	{
		//客户关闭对话窗口
		g_MyClient.SendPubconCloseReq(m_PublicSveID, nPublicID, nRouteID, 1);
	}

	
	// 释放m_arrII
	m_arrII.RemoveAll();
	 
	// 清除pWebHandle
	if (pWebHandle)
		TDEL(pWebHandle);
	//窗口关闭，将保存的全局用户id-窗口句柄对应关系清除
	allopenwnd[friend_.id] = NULL;	

	g_pFrame->CloseWhoSendMsg() ;
	Close();
}

//改变输入字体
void ChatDialog::OnCbo_SelChange_FontName(TNotifyUI& msg)
{
	tstring strText = (TCHAR *)m_pFontNameCombo->GetText().GetData();
	if (g_userconfig.m_fontInfo.m_strName != strText)
	{
		g_userconfig.m_fontInfo.m_strName = strText;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, g_userconfig.m_fontInfo.m_strName.c_str(),
			g_userconfig.m_fontInfo.m_nSize, g_userconfig.m_fontInfo.m_clrText, g_userconfig.m_fontInfo.m_bBold,
			g_userconfig.m_fontInfo.m_bItalic, g_userconfig.m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_strName = g_userconfig.m_fontInfo.m_strName;

		SaveUserConfig();
	}
}

//改变输入字号
void ChatDialog::OnCbo_SelChange_FontSize(TNotifyUI& msg)
{
	tstring strText = (TCHAR *)m_pFontSizeCombo->GetText().GetData();

	int nSize = _tcstol(strText.c_str(), NULL, 10);
	if (g_userconfig.m_fontInfo.m_nSize != nSize)
	{
		g_userconfig.m_fontInfo.m_nSize = nSize;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, g_userconfig.m_fontInfo.m_strName.c_str(),
			g_userconfig.m_fontInfo.m_nSize, g_userconfig.m_fontInfo.m_clrText, g_userconfig.m_fontInfo.m_bBold,
			g_userconfig.m_fontInfo.m_bItalic, g_userconfig.m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_nSize = g_userconfig.m_fontInfo.m_nSize;

		SaveUserConfig();
	}
}

//输入字体加粗
void ChatDialog::OnBtn_Bold(TNotifyUI& msg)
{
	BOOL bBold = !m_pBoldBtn->IsSelected();
	if (g_userconfig.m_fontInfo.m_bBold != bBold)
	{
		g_userconfig.m_fontInfo.m_bBold = bBold;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, g_userconfig.m_fontInfo.m_strName.c_str(),
			g_userconfig.m_fontInfo.m_nSize, g_userconfig.m_fontInfo.m_clrText, g_userconfig.m_fontInfo.m_bBold,
			g_userconfig.m_fontInfo.m_bItalic, g_userconfig.m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_bBold = g_userconfig.m_fontInfo.m_bBold;

		SaveUserConfig();
	}
}

//输入字体斜体
void ChatDialog::OnBtn_Italic(TNotifyUI& msg)
{
	BOOL bItalic = !m_pItalicBtn->IsSelected();
	if (g_userconfig.m_fontInfo.m_bItalic != bItalic)
	{
		g_userconfig.m_fontInfo.m_bItalic = bItalic;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, g_userconfig.m_fontInfo.m_strName.c_str(),
			g_userconfig.m_fontInfo.m_nSize, g_userconfig.m_fontInfo.m_clrText, g_userconfig.m_fontInfo.m_bBold,
			g_userconfig.m_fontInfo.m_bItalic, g_userconfig.m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_bItalic = g_userconfig.m_fontInfo.m_bItalic;

		SaveUserConfig();
	}
}

//输入字体下划线
void ChatDialog::OnBtn_UnderLine(TNotifyUI& msg)
{
	BOOL bUnderLine = !m_pUnderLineBtn->IsSelected();
	if (g_userconfig.m_fontInfo.m_bUnderLine != bUnderLine)
	{
		g_userconfig.m_fontInfo.m_bUnderLine = bUnderLine;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, g_userconfig.m_fontInfo.m_strName.c_str(),
			g_userconfig.m_fontInfo.m_nSize, g_userconfig.m_fontInfo.m_clrText, g_userconfig.m_fontInfo.m_bBold,
			g_userconfig.m_fontInfo.m_bItalic, g_userconfig.m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_bUnderLine = g_userconfig.m_fontInfo.m_bUnderLine;

		SaveUserConfig();
	}
}

//输入字体颜色
void ChatDialog::OnBtn_Color(TNotifyUI& msg)
{
	static COLORREF rgbCustomColors[16] =
	{
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
	};

	CHOOSECOLOR cc = {0};
	cc.lStructSize = sizeof(cc);
	cc.lpCustColors = rgbCustomColors;
	cc.hwndOwner = m_hWnd;
	cc.Flags = CC_RGBINIT;

	BOOL bRet = ::ChooseColor(&cc);
	if (bRet)
	{
		COLORREF clrText = cc.rgbResult;
		if (g_userconfig.m_fontInfo.m_clrText != clrText)
		{
			g_userconfig.m_fontInfo.m_clrText = clrText;
			ITextServices * pTextServices = m_pSendEdit->GetTextServices();
			RichEdit_SetDefFont(pTextServices, g_userconfig.m_fontInfo.m_strName.c_str(),
				g_userconfig.m_fontInfo.m_nSize, g_userconfig.m_fontInfo.m_clrText, g_userconfig.m_fontInfo.m_bBold,
				g_userconfig.m_fontInfo.m_bItalic, g_userconfig.m_fontInfo.m_bUnderLine, FALSE);
			pTextServices->Release();
			g_buddyFontInfo.m_clrText = g_userconfig.m_fontInfo.m_clrText;

			SaveUserConfig();
		}
	}
}

//字体“A”按钮
void ChatDialog::OnBtn_Font(TNotifyUI& msg)
{
	CContainerUI* pFontbar = static_cast<CContainerUI*>(m_PaintManager.FindControl(kFontbarControlName));
	if (pFontbar != NULL)
		pFontbar->SetVisible(!pFontbar->IsVisible());

	m_pFontNameCombo->GetItemBkImage();
}

//默认表情按钮
void ChatDialog::OnBtn_Face(TNotifyUI& msg)
{
	m_FaceSelDlg.SetFaceList(&m_FaceList);
	if (!::IsWindow((HWND)m_FaceSelDlg))
	{
		m_FaceSelDlg.Create(m_hWnd, NULL, WS_CHILD|WS_POPUP, WS_EX_TOOLWINDOW);
		// 将启动表情选择框的窗口指针传进去。如果不这样做，在DLL模式下的投顾端，::GetParent(m_hWnd)将不能得到ChatDialog的hwnd
		m_FaceSelDlg.SetParentWnd(m_hWnd);

		RECT rcBtn = m_pFaceBtn->GetPos();
		::ClientToScreen(m_hWnd, (LPPOINT)&rcBtn);

		int cx = 432;
		int cy = 236;
		int x = rcBtn.left - cx / 2;
		int y = rcBtn.top - cy;

		::SetWindowPos((HWND)m_FaceSelDlg, NULL, x, y, cx, cy, NULL);
		::ShowWindow((HWND)m_FaceSelDlg, SW_SHOW);
	}
}

// 插入图片按钮
void ChatDialog::OnBtn_Image(TNotifyUI& msg)
{
	if (NULL == m_pSendEdit)
		return;

	LPCTSTR lpszFilter = _T("Image file(*.bmp;*.jpg;*.jpeg;*.gif;*.png)\0*.bmp;*.jpg;*.jpeg;*.gif;*.png\0\0");

	CFileDialogEx fileDlg;
	fileDlg.SetFilter(lpszFilter);
	fileDlg.SetParentWnd(m_hWnd);
	if (fileDlg.ShowOpenFileDlg())
	{
		_RichEdit_InsertFace(m_pSendEdit, fileDlg.GetPathName().c_str(), -1, -1);
		m_pSendEdit->SetFocus();
	}
}

// 显示已收到的历史消息
void ChatDialog::InsertHisMsg()
{
	ShowHisRecord();
}

//////////////////////////////////////////////////////////////////////////
// 聊天界面右侧显示聊天记录
// 此处代码没写完，复杂模式，按日期查找，按关键词查找，上一页下一页等等，在此处扩展，此处暂时显示所有的历史消息
void ChatDialog::ShowRightHisRecord()
{
	m_pHisRecordEdit->SetText(_T(""));
	std::vector<CHAT_RECORD2> vtRecordset;

	CString m_strfromID = myselft_.id.GetData();
	CString m_strToID = friend_.numid.GetData();
	long lroomid = atol(friend_.Parentid.GetData());

	// 得到总条数
	long nAllRecord = g_SqliteOp.QueryRecordsetCount(eRecord_TgRoom, m_strfromID, m_strToID, FALSE, 0, lroomid);

	// wuchao  modify at 2016-03-28  修改聊天记录默认显示第一页 
	//// 聊天记录的起止位置  
	//m_nBeginDisplay = 0 ; 
	//m_nEndDisplay = nAllRecord ; 


	if (nAllRecord > 0)
	{
		// 取前N条记录 展示
 		int nEnd = nAllRecord;
		// wuchao modify at 20160413 
		// 应该从第一条开始取  
 		//int nStart = (nAllRecord - TG_RECORD_NUM) > 0 ? (nAllRecord - TG_RECORD_NUM) : 0;
		int nStart = 0 ;

 		if(!g_SqliteOp.QueryOnePageRecord(vtRecordset, eRecord_TgRoom, nStart, nEnd, m_strfromID, m_strToID, FALSE, 0, lroomid))
 			return ;

		int iRecordSetSize = (int)vtRecordset.size() ; 

		if (vtRecordset.size() > 0)	
		{
			for(int index=0; index < iRecordSetSize; ++index)
			{
				InsertChatHistory_Formdb(m_pHisRecordEdit, &vtRecordset[index], m_strToID, m_strToID);
				TDELARRAY((&vtRecordset[index])->msg);
			}
		}
	}
	vtRecordset.clear();

	// wuchao modify at 2016-03-28  
	// 根据测试要求，点击聊天记录  显示第一页

	m_nBeginDisplay = 0 ; 
	if( nAllRecord > TG_RECORD_NUM)
	{
		ShowHisRecordPageByPage(0,TG_DISAPLY_NUM) ;
		// 聊天记录的起止位置  
		m_nEndDisplay = TG_DISAPLY_NUM ; 
	}
	else
	{
		ShowHisRecordPageByPage(0,nAllRecord) ;
		// 聊天记录的起止位置  
		m_nEndDisplay = nAllRecord ; 
	}


	//获取当前日期 2016-02-19 
	CString str; 
	CTime tm;
	tm=CTime::GetCurrentTime();   
	str=tm.Format("%Y-%m-%d");
	
	if( m_pDateTimeLabel )
	{
		m_pDateTimeLabel->SetText(str) ; 
	}
}

// 显示聊天记录
void ChatDialog::ShowHisRecord()
{
	std::vector<CHAT_RECORD2> vtRecordset;

	CString m_strfromID = myselft_.id.GetData();
	CString m_strToID = friend_.numid.GetData();
	long lroomid = atol(friend_.Parentid.GetData());
	
	// 得到总条数
	long nAllRecord = g_SqliteOp.QueryRecordsetCount(eRecord_TgRoom, m_strfromID, m_strToID, FALSE, 0, lroomid);

	if (nAllRecord > 0)
	{
		// 取前N条记录 展示
		int nEnd = nAllRecord;
		int nStart = (nAllRecord - TG_RECORD_NUM) > 0 ? (nAllRecord - TG_RECORD_NUM) : 0;

		if(!g_SqliteOp.QueryOnePageRecord(vtRecordset, eRecord_TgRoom, nStart, nEnd, m_strfromID, m_strToID, FALSE, 0, lroomid))
			return ;

		int iSize =  (int)vtRecordset.size() ; 

		if ( iSize > 0)	
		{
			for(int index=0; index < iSize; ++index)
			{
				InsertChatHistory_Formdb(m_pRecvEdit, &vtRecordset[index], m_strToID, m_strToID);
				TDELARRAY((&vtRecordset[index])->msg);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 显示“------以上是历史消息------”
		AddMsgToRecvEdit_Text("------以上是历史消息------");
	}
}


// 获取聊天记录的总数目 
long  ChatDialog::GetHisRecordCount()       
{
	std::vector<CHAT_RECORD2> vtRecordset;

	CString strfromID = myselft_.id.GetData();
	CString strToID = friend_.numid.GetData();
	long lroomid = atol(friend_.Parentid.GetData());

	// 得到总条数
	long nAllRecord = g_SqliteOp.QueryRecordsetCount(eRecord_TgRoom, strfromID, strToID, FALSE, 0, lroomid);

	return nAllRecord ; 
}
//// 聊天记录分页显示 
//// nStart 起始显示条数ID 
//// nEnd   最后显示条数ID
void ChatDialog::ShowHisRecordPageByPage( int nStart ,int nEnd )
{
	if( nStart > nEnd )
		return ; 
	std::vector<CHAT_RECORD2> vtRecordset;

	CString m_strfromID = myselft_.id.GetData();
	CString m_strToID = friend_.numid.GetData();
	long lroomid = atol(friend_.Parentid.GetData());
	// 获取聊天记录总条数 
	long nAllRecordCount = GetHisRecordCount() ; 

	// 边界处理 
	nEnd > nAllRecordCount ? nAllRecordCount : nEnd ; 
	
	if(!g_SqliteOp.QueryOnePageRecord(vtRecordset, eRecord_TgRoom, nStart, nEnd, m_strfromID, m_strToID, FALSE, 0, lroomid))
		return ;
	

	// 清空当前显示的聊天记录 
	m_pHisRecordEdit->SetText(" ") ; 
	int  iSize = vtRecordset.size() ;
	if (iSize > 0)	
	{
		for(int index=0; index < iSize; ++index)
		{
			InsertChatHistory_Formdb(m_pHisRecordEdit, &vtRecordset[index], m_strToID, m_strToID);
			TDELARRAY((&vtRecordset[index])->msg);
		}
	}

}
// 显示已收到的消息
void ChatDialog::InsertRecvMsg()
{
	//打开聊天窗口之后，填充发来的消息
	if (eDialogType == eDIALOG_ChatTG)
	{
		if(pMsg == NULL)
			return;
		//m_csLock.Lock();
		RECEIVED_MSG msg;
		while(GetNextMsg_More(pMsg, &msg))
		{
			CString strRoomID = msg.innerID;
			CString strfxsID = msg.imid;
			if ( strCurrentID.CompareNoCase(strfxsID) == 0 )
				InsertChatHistory_More(&msg, strCurrentName, strfxsID);
			tmStart_ = time(NULL);
			TDEL(msg.msg);
		}
		//m_csLock.Unlock();
	}
	else
	{
		if(friend_iterator->pMsg == NULL)
			return;
		//friend_iterator->Lock();
		RECEIVED_MSG msg;
		while(GetNextMsg(friend_iterator, &msg))
		{
			FlashWindow(m_hWnd, TRUE);
			if (eDialogType == eDIALOG_Group)
				InsertChatHistory_More(&msg, msg.nickname, msg.imid);
			else
				InsertChatHistory_More(&msg, friend_iterator->nick_name, friend_iterator->id);

			tmStart_ = time(NULL);
			TDEL(msg.msg);
		}
		//friend_iterator->Unlock();
	}
}

void ChatDialog::ShakeWindow()
{
	//BUG 窗口最小化之后不能抖动
	//::ShowWindow(m_hWnd, SW_SHOWNORMAL);
	ShowWindow(TRUE);

	int ty = 2;  
	CRect m_rect;     
	::GetWindowRect(m_hWnd, &m_rect);    
	int recordy = m_rect.left;  
	int recordx = m_rect.top;  

	for(int i=0; i<2; i++)  
	{  
		m_rect.left = recordy;  
		m_rect.top = recordx;  
		m_rect.top = m_rect.top + ty;    
		m_rect.left = m_rect.left - ty;  
		::SetWindowPos(m_hWnd, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOSIZE );  
		Sleep(SHAKE_TIME_SPAN);  
		m_rect.top = m_rect.top - ty;  
		::SetWindowPos(m_hWnd, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOSIZE );  
		Sleep(SHAKE_TIME_SPAN);  
		m_rect.top = m_rect.top -2*ty;  
		::SetWindowPos(m_hWnd, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOSIZE );  
		Sleep(SHAKE_TIME_SPAN);  
		m_rect.left=m_rect.left + ty;  
		::SetWindowPos(m_hWnd, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOSIZE );  
		Sleep(SHAKE_TIME_SPAN);  
		m_rect.left=m_rect.left + 2*ty;  
		::SetWindowPos(m_hWnd, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOSIZE );  
		Sleep(SHAKE_TIME_SPAN);  
		m_rect.top = m_rect.top + ty;    
		::SetWindowPos(m_hWnd, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOSIZE );  
		Sleep(SHAKE_TIME_SPAN);  
		m_rect.top=m_rect.top + 2*ty;  
		::SetWindowPos(m_hWnd, NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOSIZE );  
		Sleep(SHAKE_TIME_SPAN);  
		::SetWindowPos(m_hWnd, NULL, recordy, recordx, 0, 0, SWP_NOSIZE );  
		Sleep(SHAKE_TIME_SPAN);  
	}
	return;
}

//窗口抖动按钮
void ChatDialog::OnBtn_Shake(TNotifyUI& msg)
{
	if (1)	//如果用户允许窗口抖动
	{
		COMPLEX_MSG_DATA* pData = new COMPLEX_MSG_DATA;
		if(!pData) return;
		memset(pData,0,sizeof(COMPLEX_MSG_DATA));
		pData->sSend = -1;

		g_MyClient.m_BigPackageSend.CreateMsg(friend_.id, pData, eMsgFormat_Shake);

		g_pFrame->PostMessage(UM_SOUND_PLAY, IDR_WAVE_SHAKE);
		//抖动自己的窗口
		ShakeWindow();
	}
	return;
}

// 屏幕截图按钮
void ChatDialog::OnBtn_Screenshots(TNotifyUI& msg)
{
	IUPIM_PupupDialog_(m_hWnd);
}

// 文章按钮
void ChatDialog::OnBtn_WenZhang(TNotifyUI& msg)
{
	return;
}

// 品种按钮
void ChatDialog::OnBtn_PinZhong(TNotifyUI& msg)
{
	return;
}

// 搜索历史消息
void ChatDialog::OnBtn_Right_SearchBar(TNotifyUI& msg)
{
	CContainerUI* pSearchbar = static_cast<CContainerUI*>(m_PaintManager.FindControl(_T("chatbox_right_searchBar")));
	if (pSearchbar != NULL)
		pSearchbar->SetVisible(!pSearchbar->IsVisible());

	m_pSearchTimeCombo->GetItemBkImage();
}

// 历史消息栏 - 日历
void ChatDialog::OnBtn_Right_RiLi(TNotifyUI& msg)
{
	if (!::IsWindow((HWND)m_pCalendaryDlg))
	{
		m_pCalendaryDlg.Create(m_hWnd, NULL, WS_CHILD|WS_POPUP, WS_EX_TOOLWINDOW);
		m_pCalendaryDlg.SetParentWnd(m_hWnd);
		if( m_pRight_rili)
		{
			RECT rcBtn = m_pRight_rili->GetPos();
			::ClientToScreen(m_hWnd, (LPPOINT)&rcBtn);
			
			//int cx = 185;
			//int cy = 182;
			//int x = rcBtn.left + 5;
			//int y = rcBtn.top - cy - 5 ;
			int cx = 177;
			int cy = 176;
			int x = rcBtn.left  +5;
			int y = rcBtn.top - cy - 5 ;

			::SetWindowPos((HWND)m_pCalendaryDlg, NULL, x, y, cx, cy, NULL);
			::ShowWindow((HWND)m_pCalendaryDlg, SW_SHOW);
		}
	}
	return ;
}

// 历史消息栏 - 最首页
// 显示聊天记录的前20条即可 
void ChatDialog::OnBtn_Right_PageBegin(TNotifyUI& msg)
{
	// 显示0-20条 
	ShowHisRecordPageByPage(0,TG_DISAPLY_NUM) ; 
	m_nBeginDisplay = 0 ; 
	m_nEndDisplay = TG_DISAPLY_NUM ; 

	//// 点击之后,首页和上一页按钮都置灰，待点击上一页、最后一页按钮，恢复    
	//m_pRight_begin->SetEnabled(false) ; 
	//m_pRight_up->SetEnabled(false) ;
	//m_pRight_down->SetEnabled(TRUE) ; 
	//m_pRight_end->SetEnabled(TRUE) ; 
	return;
}

// 历史消息栏 - 上一页
void ChatDialog::OnBtn_Right_PageUp(TNotifyUI& msg)
{
	//m_pRight_down->SetEnabled(TRUE) ; 
	//m_pRight_end->SetEnabled(TRUE) ; 

	// 起始位置 
	int temp =  m_nEndDisplay- TG_DISAPLY_NUM ; 

	if( temp < 0 )
	{
		temp = 0 ; 
	}
	else
	{
		m_nBeginDisplay = m_nEndDisplay - TG_DISAPLY_NUM ;
	}
	ShowHisRecordPageByPage(m_nBeginDisplay ,m_nEndDisplay) ; 
	m_nEndDisplay -= TG_DISAPLY_NUM  ; 
	if( m_nEndDisplay < TG_DISAPLY_NUM )
	{
		m_nEndDisplay = TG_DISAPLY_NUM ; 
	}
	return;
}

// 历史消息栏 - 下一页
void ChatDialog::OnBtn_Right_PageDown(TNotifyUI& msg)
{
	m_nBeginDisplay = m_nEndDisplay ;
	int nAllRecordCount = GetHisRecordCount() ;
	if((m_nBeginDisplay + TG_DISAPLY_NUM) < nAllRecordCount ) 
	{
		m_nEndDisplay   = m_nBeginDisplay + TG_DISAPLY_NUM ; 
	}
	else
	{
		// 下一页显示数量 < TG_DISAPLY_NUM
		m_nEndDisplay = GetHisRecordCount() ;
		//m_pRight_down->SetEnabled(FALSE) ; // 将下一页按钮置灰 
	}
	if( m_nEndDisplay <= nAllRecordCount && m_nBeginDisplay != m_nEndDisplay)
		ShowHisRecordPageByPage(m_nBeginDisplay,m_nEndDisplay) ;  

	//m_pRight_begin->SetEnabled(TRUE) ; 
	//m_pRight_up->SetEnabled(TRUE) ; 
	return;
}

// 历史消息栏 - 最末页
void ChatDialog::OnBtn_Right_PageEnd(TNotifyUI& msg)
{
	// 
	//m_pRight_begin->SetEnabled(TRUE) ; 
	
	// 获取聊天记录的总条数 
	long nHisRecordCount = GetHisRecordCount() ; 

	// TG_DISAPLY_NUM
	long lastPageDisplayCount = nHisRecordCount % TG_DISAPLY_NUM ; 

	if( lastPageDisplayCount > 0 )
	{
		ShowHisRecordPageByPage(nHisRecordCount - lastPageDisplayCount  ,nHisRecordCount) ; 
		m_nBeginDisplay = nHisRecordCount - lastPageDisplayCount ; 
		m_nEndDisplay   = nHisRecordCount ; 
	}
	else if( lastPageDisplayCount == 0 )
	{
		ShowHisRecordPageByPage(nHisRecordCount - TG_DISAPLY_NUM  ,nHisRecordCount) ; 
		m_nBeginDisplay = nHisRecordCount - TG_DISAPLY_NUM ; 
		m_nEndDisplay   = nHisRecordCount ; 
	}
	else
	{

	}
	//// 最后一页置灰 
	//m_pRight_end->SetEnabled(FALSE) ; 
	//// 下一页置灰 
	//m_pRight_down->SetEnabled(FALSE) ; 
	//// 上一页恢复 
	//m_pRight_up->SetEnabled(TRUE) ; 
	return;
}

// 历史消息栏 - 搜索
// wuchao  
void ChatDialog::OnBtn_Right_SearchOK(TNotifyUI& msg)
{
	CString strTime ; 
	CString strContent ; 

	//（1）获取时间范围
	if( m_pSearchTimeCombo)
	{
		CDuiString strTime_ = m_pSearchTimeCombo->GetText() ;
		strTime = strTime_.GetData() ; 
	}
	 
	//（2） 获取内容 
	if( m_pSearchContent)
	{
		CDuiString strContent_ = m_pSearchContent->GetText();
		strContent = strContent_.GetData() ; 
	}
	if( strContent.IsEmpty())
	{
		// 提示查找内容为空 
		 ::MessageBox(NULL,  _T("请输入要查询的内容"), _T("提示"),NULL);
	}
	//（3） 查询数据库 
	
	//CString year("2015") ;
	//CString month("04") ; 
	//CString day("01") ; 
	//GetCurrentDate( year ,month ,day ) ; 

	m_pHisRecordEdit->SetText(_T(""));
	std::vector<CHAT_RECORD2> vtRecordset;

	CString m_strfromID = myselft_.id.GetData();
	CString m_strToID = friend_.numid.GetData();
	long lroomid = atol(friend_.Parentid.GetData());

	// 得到总条数
	long nAllRecord = g_SqliteOp.QueryRecordsetCount(eRecord_TgRoom, m_strfromID, m_strToID, FALSE, 0, lroomid);

	if (nAllRecord > 0)
	{
		int nEnd = nAllRecord;
		int nStart = 0;

		// 获取当前时间  
		time_t currTime = time(NULL) ; 
		struct tm *ptm = localtime(&currTime) ;
		long lCurrTime = (ptm->tm_year+1900)*10000+(ptm->tm_mon+1)*100+ptm->tm_mday; 
		
		if( strTime == "全部")
		{
			if(!g_SqliteOp.QueryOnePageRecord(vtRecordset, eRecord_TgRoom, nStart, nEnd, m_strfromID, m_strToID, FALSE, 0, lroomid))
				return ;
		}
		else if(strTime == "最近一个月")
		{
			long lNextTime = (ptm->tm_year+1900)*10000+(ptm->tm_mon+1 - 1 )*100+ptm->tm_mday;

			if( !g_SqliteOp.QueryTimeQuantumRecord(vtRecordset,eRecord_TgRoom,m_strfromID,m_strToID,lCurrTime,lNextTime,lroomid))
				return ; 
		}
		else if( strTime == "最近三个月")
		{
			long lNextTime = (ptm->tm_year+1900)*10000+(ptm->tm_mon+1 - 3 )*100+ptm->tm_mday;

			if( !g_SqliteOp.QueryTimeQuantumRecord(vtRecordset,eRecord_TgRoom,m_strfromID,m_strToID,lCurrTime,lNextTime,lroomid))
				return ;
		}
		else if(strTime == "最近一年")
		{
			long lNextTime = (ptm->tm_year+1900 - 1 )*10000+(ptm->tm_mon+1 )*100+ptm->tm_mday;

			if( !g_SqliteOp.QueryTimeQuantumRecord(vtRecordset,eRecord_TgRoom,m_strfromID,m_strToID,lCurrTime,lNextTime,lroomid))
				return ;
		}
		
		int iRecordSetSize = (int)vtRecordset.size() ; 

		if (vtRecordset.size() > 0)	
		{
			for(int index=0; index < iRecordSetSize; ++index)
			{

				char *msgBuf = vtRecordset[index].msg ; 
				CString strMsg ; 
				strMsg.Format("%s",msgBuf) ; 
				int index1 = strMsg.Find(strContent) ; 
				if( index1 != -1)
				{
					InsertChatHistory_Formdb(m_pHisRecordEdit, &vtRecordset[index], m_strToID, m_strToID);
					TDELARRAY((&vtRecordset[index])->msg);
				}
				
			}
		}
	}
	vtRecordset.clear();

	

	//（4） 显示内容  
	return;
}

//////////////////////////////////////////////////////////////////////
// wuchao add at 20160411 
// 表扬投诉分析师功能   
//////////////////////////////////////////////////////////////////////
void ChatDialog::OnBtn_BYTSAnalyst(TNotifyUI& msg)
{
	// 要将分析师的ID和username传进去 
	LPCTSTR lpMyselfID = g_khID ; /*g_khID*/
	LPCTSTR lpMyselfName = g_MyClient.m_strUserName ; 
	
	LPCTSTR lpAnayID = friend_iterator->numid.GetData() ;
	LPCTSTR lpAnayName = friend_iterator->nick_name.GetData() ; 

	if( strlen(lpMyselfID) <= 0  )
	{
		int len = strlen(lpMyselfName) ; 
		memset((void*)lpMyselfID ,0 ,len + 1 ) ; 

		memcpy((void*)lpMyselfID ,(void*)lpMyselfName,len + 1 ); 
	}
	if( strlen(lpMyselfName) <= 0 )
	{
		int len = strlen(lpMyselfID) ; 
		memset((void*)lpMyselfName ,0 ,len+1) ; 

		memcpy((void*)lpMyselfName ,(void*)lpMyselfID,len+1 );
	}
	//CBytsDialog* pUserOpenRights = new CBytsDialog(strMyselfName ,strMyselefID ,strName ,strID);

	CBytsDialog * pUserOpenRights = new CBytsDialog(lpMyselfName ,lpMyselfID ,lpAnayName ,lpAnayID); 
	if( pUserOpenRights == NULL )
		return;

	pUserOpenRights->Create(m_hWnd, "", UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 476, 345);
	pUserOpenRights->CenterWindow();
	pUserOpenRights->ShowModal();

	return;
}
//////////////////////////////////////////////////////////////////////
// wuchao add at 20160411 
// 收藏分析师   
//////////////////////////////////////////////////////////////////////
void ChatDialog::OnBtn_ShouCangAnalyst(TNotifyUI& msg)
{
	m_pShouCangBtn->SetText(_T("已收藏")) ;
	m_pShouCangBtn->SetEnabled(false) ; 
}
// 历史消息 - 关闭
void ChatDialog::OnBtn_Right_Close(TNotifyUI& msg)
{
	CControlUI* prightpart = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("chatbox_right_part")));
	if (m_pControlSwitch)
	{
		m_pControlSwitch->SelectItem(0);
		nRightSwitchType = 0;
		if (prightpart)
		{
			RECT rc_old, rc_new;
			GetWindowRect (m_hWnd, &rc_old);
			rc_new.left = rc_old.left;
			rc_new.right = rc_old.right - 160;
			rc_new.bottom = rc_old.bottom;
			rc_new.top = rc_old.top;
			if (m_PaintManager.GetPaintWindow() != NULL)
			{
				prightpart->SetMinWidth(240);
				m_PaintManager.GetRoot()->SetPos(rc_new);
				::MoveWindow (m_hWnd, rc_new.left, rc_new.top, rc_new.right-rc_new.left, rc_new.bottom-rc_new.top, TRUE);
			}
		}
		if(m_pBYTSBtn)
			m_pBYTSBtn->SetVisible(TRUE) ; 
		if(m_pShouCangBtn)
			m_pShouCangBtn->SetVisible(TRUE) ; 
	}
	return;
}

// 历史消息按钮
void ChatDialog::OnBtn_HisRecord(TNotifyUI& msg)
{
	//nRightSwitchType 如果是显示的右侧信息区
	CControlUI* prightpart = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("chatbox_right_part")));

	// 聊天记录的下一页上一页等功能按钮处理 

	long nHisRecordCount = GetHisRecordCount() ; 
	SIZE sz1 = m_pRecvEdit->GetScrollPos() ;
	//// 将上一页和下一个 首页 尾页按钮 置灰 
	//if( nHisRecordCount < TG_DISAPLY_NUM )
	//{
	//	m_pRight_begin->SetEnabled(FALSE) ;
	//	m_pRight_up->SetEnabled(FALSE) ;
	//	m_pRight_down->SetEnabled(FALSE) ;
	//	m_pRight_end->SetEnabled(FALSE) ;
	//}
	//else
	//{
	//	// 首页和上一页恢复 
	//	m_pRight_begin->SetEnabled(TRUE) ;
	//	m_pRight_up->SetEnabled(TRUE) ;

	//	// 尾页和下页置灰 
	//	m_pRight_down->SetEnabled(FALSE) ;
	//	m_pRight_end->SetEnabled(FALSE) ;
	//}
	if (nRightSwitchType == 0)
	{
		if (m_pControlSwitch)
		{
			m_pControlSwitch->SelectItem(1);
			nRightSwitchType = 1;
			if (prightpart)
			{
				RECT rc_old, rc_new;
				GetWindowRect (m_hWnd, &rc_old);
				rc_new.left = rc_old.left;
				rc_new.right = rc_old.right + 160;
				rc_new.bottom = rc_old.bottom;
				rc_new.top = rc_old.top;

				//RECT rt ; 
				//rt = m_pRecvEdit->GetPos() ; 
				//RECT rt ; 
				//rt.left = rc_new.right - 400 ;
				//rt.top= rc_new.top ;
				//rt.right = rc_new.right ;
				//rt.bottom = rc_new.bottom ; 

				if (m_PaintManager.GetPaintWindow() != NULL)
				{
					prightpart->SetMinWidth(400);
					m_PaintManager.GetRoot()->SetPos(rc_new);
					::MoveWindow (m_hWnd, rc_new.left, rc_new.top, rc_new.right-rc_new.left, rc_new.bottom-rc_new.top, TRUE);
					PostMessage(UM_USER_RIGHTHISMSG, 0, 0);
				}

				// wuchao add at 20160411 
				if( m_pBYTSBtn)
					m_pBYTSBtn->SetVisible(FALSE) ; 
				
				if( m_pShouCangBtn)
					m_pShouCangBtn->SetVisible(FALSE) ; 
			}
		}
	} // end of if(nRightSwitchType== 0)
	else 
	{
		if (m_pControlSwitch)
		{
			m_pControlSwitch->SelectItem(0);
			nRightSwitchType = 0;
			if (prightpart)
			{
				RECT rc_old, rc_new;
				GetWindowRect (m_hWnd, &rc_old);
				rc_new.left = rc_old.left;
				rc_new.right = rc_old.right - 160;
				rc_new.bottom = rc_old.bottom;
				rc_new.top = rc_old.top;

				//RECT rt ; 
				//rt.left = rc_new.right - 400 ;
				//rt.top= rc_new.top ;
				//rt.right = rc_new.right ;
				//rt.bottom = rc_new.bottom ; 

				if (m_PaintManager.GetPaintWindow() != NULL)
				{
					prightpart->SetMinWidth(240);
					m_PaintManager.GetRoot()->SetPos(rc_new);
					//m_PaintManager.GetRoot()->SetPos(rt);
					::MoveWindow (m_hWnd, rc_new.left, rc_new.top, rc_new.right-rc_new.left, rc_new.bottom-rc_new.top, TRUE);
				}
				// wuchao add at 20160411 
				if( m_pBYTSBtn)
					m_pBYTSBtn->SetVisible(TRUE) ; 

				if( m_pShouCangBtn)
					m_pShouCangBtn->SetVisible(TRUE) ;
			}
		}
	}

	//m_pRecvEdit->EndDown();
	SIZE sz = m_pRecvEdit->GetScrollPos() ; 
	return;
}

// “剪切”菜单
void ChatDialog::OnMenu_Cut(TNotifyUI& msg)
{
	m_pSendEdit->Cut();
}

// “复制”菜单
void ChatDialog::OnMenu_Copy_Recv()
{
	m_pRecvEdit->Copy();
}

// “复制”菜单
void ChatDialog::OnMenu_Copy_Send()
{
	m_pSendEdit->Copy();
}

// “粘贴”菜单
void ChatDialog::OnMenu_Paste(TNotifyUI& msg)
{
	m_pSendEdit->PasteSpecial(CF_TEXT);
}

// “全部选择”菜单
void ChatDialog::OnMenu_SelAll_Recv()
{
	m_pRecvEdit->SetSel(0, -1);
}

// “全部选择”菜单
void ChatDialog::OnMenu_SelAll_Send()
{
	m_pSendEdit->SetSel(0, -1);
}

// “清屏”菜单
void ChatDialog::OnMenu_Clear(TNotifyUI& msg)
{
	m_pRecvEdit->SetText(_T(""));
}

// “显示比例”菜单
void ChatDialog::OnMenu_ZoomRatio(LPCTSTR lpZoom)
{
	if (strcmp(m_strCheckMenu.c_str(), lpZoom) == 0)
		return;
	else
	{
		m_strCheckMenu = lpZoom;
	}

	if (strcmp(lpZoom, _T("Menu_ZoomRatio_400")) == 0)
		m_pRecvEdit->SetZoom(16, 4);
	else if (strcmp(lpZoom, _T("Menu_ZoomRatio_200")) == 0)
		m_pRecvEdit->SetZoom(8, 4);
	else if (strcmp(lpZoom, _T("Menu_ZoomRatio_150")) == 0)
		m_pRecvEdit->SetZoom(6, 4);
	else if (strcmp(lpZoom, _T("Menu_ZoomRatio_125")) == 0)
		m_pRecvEdit->SetZoom(5, 4);
	else if (strcmp(lpZoom, _T("Menu_ZoomRatio_100")) == 0)
		m_pRecvEdit->SetZoom(0, 0);
	else if (strcmp(lpZoom, _T("Menu_ZoomRatio_75")) == 0)
		m_pRecvEdit->SetZoom(3, 4);
	else if (strcmp(lpZoom, _T("Menu_ZoomRatio_50")) == 0)
		m_pRecvEdit->SetZoom(1, 2);
	else
		return;
}

// “另存为”菜单
void ChatDialog::OnMenu_SaveAs(int nType)
{
	ITextServices * pTextServices = NULL;
	if (nType == 0)
		pTextServices = m_pRecvEdit->GetTextServices();
	else
		pTextServices = m_pSendEdit->GetTextServices();

	IImageOle * pImageOle = NULL;
	BOOL bRet = RichEdit_GetImageOle(pTextServices, m_ptRBtnDown, &pImageOle);
	pTextServices->Release();
	if (!bRet || NULL == pImageOle)
		return;

	tstring strFileName;

	BSTR bstrFileName = NULL;
	HRESULT hr = pImageOle->GetFileName(&bstrFileName);
	if (SUCCEEDED(hr))
		strFileName = COLE2T(bstrFileName);
	if (bstrFileName != NULL)
		::SysFreeString(bstrFileName);

	LPCTSTR lpszDefExt;
	LPCTSTR lpszFileName = _T("未命名");
	DWORD dwFlags = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR|OFN_EXTENSIONDIFFERENT;
	LPCTSTR lpszFilter;
	HWND hWndParent = m_hWnd;

	GUID guid = {0};
	hr = pImageOle->GetRawFormat(&guid);

	if (InlineIsEqualGUID(guid, Gdiplus::ImageFormatJPEG))
	{
		lpszDefExt = _T(".jpg");
		lpszFilter = _T("Image file(*.jpg)\0*.jpg\0Image file(*.bmp)\0*.bmp\0\0");
	}
	else if (InlineIsEqualGUID(guid, Gdiplus::ImageFormatPNG))
	{
		lpszDefExt = _T(".png");
		lpszFilter = _T("Image file(*.png)\0*.png\0\0");
	}
	else if (InlineIsEqualGUID(guid, Gdiplus::ImageFormatGIF))
	{
		lpszDefExt = _T(".gif");
		lpszFilter = _T("Image file(*.gif)\0*.gif\0Image file(*.jpg)\0*.jpg\0Image file(*.bmp)\0*.bmp\0\0");
	}
	else
	{
		lpszDefExt = _T(".jpg");
		lpszFilter = _T("Image file(*.jpg)\0*.jpg\0Image file(*.bmp)\0*.bmp\0\0");
	}

	CFileDialogEx fileDlg;
	fileDlg.SetDefExt(lpszDefExt);
	fileDlg.SetFileName(lpszFileName);
	fileDlg.SetFlags(dwFlags);
	fileDlg.SetFilter(lpszFilter);
	fileDlg.SetParentWnd(m_hWnd);
	fileDlg.SetTitle(_T("保存图片"));
	if (fileDlg.ShowSaveFileDlg())
	{
		tstring strSavePath = fileDlg.GetPathName();
		tstring strExtName = _T(".") + ZYM::CPath::GetExtension(strSavePath.c_str());
		GUID guid2 = GetFileTypeGuidByExtension(strExtName.c_str());

		if (InlineIsEqualGUID(guid, guid2))
		{
			CopyFile(strFileName.c_str(), strSavePath.c_str(), FALSE);
		}
		else
		{
			BSTR bstrSavePath = ::SysAllocString(CT2OLE(strSavePath.c_str()));
			if (bstrSavePath != NULL)
			{
				pImageOle->SaveAsFile(bstrSavePath);
				::SysFreeString(bstrSavePath);
			}
		}
	}

	if (pImageOle != NULL)
		pImageOle->Release();
}

LRESULT ChatDialog::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	IImageOle * pNewImageOle = NULL;
	RECT rc = {0};

	POINT pt = {LOWORD(lParam), HIWORD(lParam)};
	CControlUI * pControl = this->m_PaintManager.FindControl(pt);
	if (pControl != NULL)
	{
		if (pControl == m_pSendEdit || pControl == m_pRecvEdit)
		{
			ITextServices * pTextServices = ((CRichEditUI *)pControl)->GetTextServices();

			IImageOle * pImageOle = NULL;
			BOOL bRet = RichEdit_GetImageOle(pTextServices, pt, &pImageOle);
			if (bRet && pImageOle != NULL)
			{
				pNewImageOle = pImageOle;
				pImageOle->GetObjectRect(&rc);
			}
			if (pImageOle != NULL)
				pImageOle->Release();

			pTextServices->Release();
		}
	}

	return 0;
}

LRESULT ChatDialog::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	CControlUI * pControl = this->m_PaintManager.FindControl(pt);

	if (pControl != NULL)
	{	
		if (pControl == m_pSendEdit)
		{
			//////////////////////////////////////////////////////////////////////////
			// DLL模式下 因为跟客户端焦点冲突的问题，需要下列代码
			if (VER_UPIM_NUMS == 1)
			{
				OutputDebugString("### OnLButtonDown m_pSendEdit\r\n");

				// note:当焦点切换到SendEdit外再切回来之后，虽然m_pSendEdit上有焦点，但是却无法输入！
				// 当鼠标点击的时候，来回切换焦点，m_pSendEdit能正常输入
				bool bisSendEditFocused = m_pSendEdit->IsFocused();

				m_pSendEdit->TxSendMessage(EN_SETFOCUS, 0, 0, NULL);
				m_pBackground->SetFocus();
				m_pSendEdit->SetFocus();
			}
		}
	}
	return 0;
}

LRESULT ChatDialog::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	CControlUI * pControl = this->m_PaintManager.FindControl(pt);

	if (pControl != NULL)
	{	
		if (pControl == m_pRecvEdit)
		{
			m_ptRBtnDown = pt;

			CMenuWnd* pMenu = new CMenuWnd();
			::ClientToScreen(m_hWnd, &pt);
			pMenu->Init(NULL, _T("menu_RecvEdit.xml"), pt, &m_PaintManager );
			BOOL bSel = (m_pRecvEdit->GetSelectionType() != SEL_EMPTY);
			pMenu->EnableMenuItem(_T("Menu_Copy_Recv"), bSel);
			pMenu->SetIconAndSize(m_strCheckMenu.c_str(), _T("right.png"), 9, 9);

			ITextServices * pTextServices = m_pRecvEdit->GetTextServices();
			IImageOle * pImageOle = NULL;
			BOOL bRet = RichEdit_GetImageOle(pTextServices, m_ptRBtnDown, &pImageOle);
			BOOL bSaveAs = (bRet && pImageOle != NULL);
			pMenu->EnableMenuItem(_T("Menu_SaveAs_Recv"), bSaveAs);
			if (pImageOle != NULL)
				pImageOle->Release();

			pTextServices->Release();
		}
		else if (pControl == m_pSendEdit)
		{
			m_ptRBtnDown = pt;

			CMenuWnd* pMenu = new CMenuWnd();
			::ClientToScreen(m_hWnd, &pt);
			pMenu->Init(NULL, _T("menu_SendEdit.xml"), pt, &m_PaintManager, &m_MenuCheckInfo );
			BOOL bSel = (m_pSendEdit->GetSelectionType() != SEL_EMPTY);
			pMenu->EnableMenuItem(_T("Menu_Cut"), bSel);
			pMenu->EnableMenuItem(_T("Menu_Copy_Send"), bSel);

			BOOL bPaste = m_pSendEdit->CanPaste();
			pMenu->EnableMenuItem(_T("Menu_Paste"), bPaste);

			ITextServices * pTextServices = m_pSendEdit->GetTextServices();

			IImageOle * pImageOle = NULL;
			BOOL bRet = RichEdit_GetImageOle(pTextServices, m_ptRBtnDown, &pImageOle);
			BOOL bSaveAs = (bRet && pImageOle != NULL);
			pMenu->EnableMenuItem(_T("Menu_SaveAs_Send"), bSaveAs);
			if (pImageOle != NULL)
				pImageOle->Release();

			pTextServices->Release();
		}
		else if (pControl == pGroupsList)
		{
		}
		else
		{
			m_ptRBtnDown = pt;
		}
	}

	return 0;
}

LRESULT ChatDialog::OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	CControlUI * pControl = this->m_PaintManager.FindControl(pt);
	if (pControl != NULL)
	{
		if (pControl == m_pSendEdit || pControl == m_pRecvEdit)
		{
			ITextServices * pTextServices = ((CRichEditUI *)pControl)->GetTextServices();

			IImageOle * pImageOle = NULL;
			BOOL bRet = RichEdit_GetImageOle(pTextServices, pt, &pImageOle);
			if (bRet && pImageOle != NULL)
			{
				BSTR bstrFileName = NULL;
				HRESULT hr = pImageOle->GetFileName(&bstrFileName);
				if (SUCCEEDED(hr))
					::ShellExecute(NULL, _T("open"), COLE2T(bstrFileName), NULL, NULL, SW_SHOWNORMAL);
				if (bstrFileName != NULL)
					::SysFreeString(bstrFileName);
			}
			if (pImageOle != NULL)
				pImageOle->Release();

			pTextServices->Release();
		}
	}
	return 0;
}

// “表情”控件选取消息
LRESULT ChatDialog::OnFaceCtrlSel(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nFaceId = m_FaceSelDlg.GetSelFaceId();
	int nFaceIndex = m_FaceSelDlg.GetSelFaceIndex();
	tstring strFileName = m_FaceSelDlg.GetSelFaceFileName();
	if (!strFileName.empty())
	{
		_RichEdit_InsertFace(m_pSendEdit, strFileName.c_str(), nFaceId, nFaceIndex);
		m_pSendEdit->SetFocus();
	}
	((COptionUI *)m_pFaceBtn)->Selected(false);
	return 0;
}

///////////////////////////////////////////////////////////////////////////
//wuchao add at 2016-04-05  按下Enter键 发送消息 
//////////////////////////////////////////////////////////////////////////
LRESULT ChatDialog::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Enter键响应事件 
	if( wParam == VK_RETURN )
	{
		SendMsg_More();
		return 1 ; 
	}
	return 0 ; 
}
LRESULT ChatDialog::OnChooseHisRecordByDateTime( UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
	SYSTEMTIME dateTime = m_pCalendaryDlg.GetDateTime() ;

	if( m_pDateTimeLabel )
	{
		CString strYear ,strMonth ,strDay  ;
		CString strDate ;

		strYear.Format("%d" ,dateTime.wYear) ; 
		strMonth.Format("%02d",dateTime.wMonth) ; 
		strDay.Format("%02d" ,dateTime.wDay) ; 

		strDate = strYear + "-" + strMonth + "-" + strDay ; 

		m_pDateTimeLabel->SetText(strDate) ;
	}
	m_pHisRecordEdit->SetText(_T(""));
	std::vector<CHAT_RECORD2> vtRecordset;

	CString m_strfromID = myselft_.id.GetData();
	CString m_strToID = friend_.numid.GetData();
	long lroomid = atol(friend_.Parentid.GetData());

	// 得到总条数
	long nAllRecord = g_SqliteOp.QueryRecordsetCount(eRecord_TgRoom, m_strfromID, m_strToID, FALSE, 0, lroomid);

	// 聊天记录的起止位置  
	m_nBeginDisplay = 0 ; 
	m_nEndDisplay = nAllRecord ; 

	long lTime = (dateTime.wYear)*10000+(dateTime.wMonth )*100+dateTime.wDay;

	if (nAllRecord > 0)
	{
		// 取前N条记录 展示
		int nEnd = nAllRecord;
		int nStart = (nAllRecord - TG_RECORD_NUM) > 0 ? (nAllRecord - TG_RECORD_NUM) : 0;

		if(!g_SqliteOp.QueryTimeQuantumRecord(vtRecordset,eRecord_TgRoom,m_strfromID,m_strToID,lTime,lTime,lroomid))
			return 0 ;

		int iRecordSetSize = (int)vtRecordset.size() ; 

		if (vtRecordset.size() > 0)	
		{
			for(int index=0; index < iRecordSetSize; ++index)
			{
				InsertChatHistory_Formdb(m_pHisRecordEdit, &vtRecordset[index], m_strToID, m_strToID);
				TDELARRAY((&vtRecordset[index])->msg);
			}
		}
	}
	vtRecordset.clear();

	return 0 ; 
}
//群成员列表右击事件、菜单
void ChatDialog::RoomMemberRClick(TNotifyUI& msg)
{
	POINT pt = {msg.ptMouse.x, msg.ptMouse.y};

	CMenuWnd* pMenu = new CMenuWnd();
	::ClientToScreen(m_hWnd, &pt);

	//只允许群组模式才出现右键菜单
	if (eDialogType == eDIALOG_Group)
	{
		pMenu->Init(NULL, _T("menu_GroupMemList.xml"), pt, &m_PaintManager );
		// 如果是创建者或者管理员，才允许出现删除用户或者添加用户菜单
		ROOMID mRoomID = atol(friend_.numid);
		GCR_ITER iter_ = m_mapChatRoom.find(mRoomID);
		if (strcmp(iter_->second.m_szMasterID, g_MyClient.m_strUserid) == 0)
		{
			pMenu->EnableMenuItem(_T("Group_Mem_Add"), TRUE);
			pMenu->EnableMenuItem(_T("Group_Mem_Del"), TRUE);
		}
	}
}

// 群成员列表右键菜单-单独聊天
void ChatDialog::RoomMember_ChooseChat(TNotifyUI& msg)
{
	//此处是获取不到节点信息的，使用点击时候存储的点击node信息
	memset(g_szOpenUID, 0, sizeof(g_szOpenUID));
	strncpy(g_szOpenUID, m_selectNode.m_szID, strlen(m_selectNode.m_szID));
	g_pFrame->PostMessage(UM_USER_OPENWND, (WPARAM)eMSG_Single, (LPARAM)0);
	return;
}

// 添加群成员对话框，模态
void ChatDialog::RoomMember_Add(TNotifyUI& msg)
{
	AddMemberDialog* pAddMemberDialog = new AddMemberDialog(bgimage_.GetData(), bkcolor_, atol(friend_.numid), friend_.roomtype);
	if( pAddMemberDialog == NULL )
		return;

	pAddMemberDialog->Create(m_hWnd, "添加联系人", UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 600, 800);
	g_pFrame->skin_changed_observer_.AddReceiver(pAddMemberDialog);

	pAddMemberDialog->CenterWindow();
	pAddMemberDialog->ShowModal();
}

void ChatDialog::RoomMember_Del(TNotifyUI& msg)
{
	if (strcmp(m_selectNode.m_szID, g_MyClient.m_strUserid) == 0)
	{
		USER_LOG("您不能删除自己");
		return;
	}
	g_MyClient.SendGcKickReq(atol(friend_.numid), friend_.roomtype, m_selectNode.m_szID, m_selectNode.m_nSelUserType);
	return;
}

//若是删除成功标记返回，在列表中移除用户
void ChatDialog::DeleteMember(LPCTSTR szID, short nUserType)
{
	if (pGroupsList)
	{
		Node* thisPrs = pGroupsList->SelectNodeByID(szID);
		if (thisPrs)
		{
			pGroupsList->RemoveNode(thisPrs);
			pGroupsList->RemoveNodeMapInfo(thisPrs);
		}
	}
	return;
}

//群成员点击选中，记录下用用户信息
void ChatDialog::RoomMember(TNotifyUI& msg)
{
	//////////////////////////////////////////////////////////////////////////
	if ((pGroupsList != NULL) &&  pGroupsList->GetItemIndex(msg.pSender) != -1)
	{
		if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
		{
			Node* node = (Node*)msg.pSender->GetTag();
			//////////////////////////////////////////////////////////////////////////
			//
			if ( !pGroupsList->CanExpand(node) && (m_pBackground != NULL))
			{
				COPYSTRARRAY(m_selectNode.m_szID, node->data().value);
				m_selectNode.m_nSelUserType = (EUSERTYPE)node->data().rusertype;
			}
		}
	}
}

//群成员列表双击事件
void ChatDialog::RoomMemberClick(TNotifyUI& msg)
{
	if ((pGroupsList != NULL) &&  pGroupsList->GetItemIndex(msg.pSender) != -1)
	{
		if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
		{
			Node* node = (Node*)msg.pSender->GetTag();
			//////////////////////////////////////////////////////////////////////////
			//
			if ( !pGroupsList->CanExpand(node) && (m_pBackground != NULL))
			{
				CDuiString strtmp = node->data().value;
				OpenChatWnd(strtmp.GetData(), eMSG_Single);
			}
		}
	}
}

void ChatDialog::_RichEdit_ReplaceSel(CRichEditUI * pRichEdit, LPCTSTR lpszNewText)
{
	ITextServices * pTextServices = pRichEdit->GetTextServices();
	if (pRichEdit == m_pRecvEdit || pRichEdit == m_pHisRecordEdit)
	{
		RichEdit_ReplaceSel(pTextServices, lpszNewText, 
			g_userconfig.m_fontInfo.m_strName.c_str(), g_userconfig.m_fontInfo.m_nSize, 
			g_userconfig.m_fontInfo.m_clrText, g_userconfig.m_fontInfo.m_bBold, g_userconfig.m_fontInfo.m_bItalic, 
			g_userconfig.m_fontInfo.m_bUnderLine, FALSE, 300);
	}
	else
	{
		RichEdit_ReplaceSel(pTextServices, lpszNewText);
	}
	pTextServices->Release();
}


void ChatDialog::_RichEdit_ReplaceSel_More(CRichEditUI * pRichEdit, LPCTSTR lpszNewText, CFontInfo mfontInfo)
{
	ITextServices * pTextServices = pRichEdit->GetTextServices();
	if (pRichEdit == m_pRecvEdit || pRichEdit == m_pHisRecordEdit)
	{
		RichEdit_ReplaceSel(pTextServices, lpszNewText, 
			mfontInfo.m_strName.c_str(), mfontInfo.m_nSize, 
			mfontInfo.m_clrText, mfontInfo.m_bBold, mfontInfo.m_bItalic, 
			mfontInfo.m_bUnderLine, FALSE, 300);
	}
	else
	{
		RichEdit_ReplaceSel(pTextServices, lpszNewText);
	}
	pTextServices->Release();
}

BOOL ChatDialog::_RichEdit_InsertFace(CRichEditUI * pRichEdit, LPCTSTR lpszFileName, int nFaceId, int nFaceIndex, IMAGE_INFO *pII /* = NULL */)
{
	BOOL bRet = FALSE;

	if (NULL == pRichEdit || NULL == lpszFileName || NULL == *lpszFileName)
		return FALSE;

	ITextServices * pTextServices = pRichEdit->GetTextServices();
	ITextHost * pTextHost = pRichEdit->GetTextHost();
	if (pTextServices != NULL && pTextHost != NULL)
	{
		if (pRichEdit == m_pRecvEdit || pRichEdit == m_pHisRecordEdit)
			RichEdit_SetStartIndent(pTextServices, 300);
		bRet = RichEdit_InsertFace(pTextServices, pTextHost, 
			lpszFileName, nFaceId, nFaceIndex, RGB(255,255,255), TRUE, 40, pII);
	}

	if (pTextServices != NULL)
		pTextServices->Release();
	if (pTextHost != NULL)
		pTextHost->Release();

	return bRet;
}

BOOL ChatDialog::HandleSysFaceId(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	int nFaceId = GetBetweenInt(p+2, cStart, cEnd, -1);

	CFaceInfo * lpFaceInfo = m_FaceList.GetFaceInfoById(nFaceId);
	if (lpFaceInfo != NULL)
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel(pRichEdit, strText.c_str()); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, lpFaceInfo->m_strFileName.c_str(), 
			lpFaceInfo->m_nId, lpFaceInfo->m_nIndex);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}


BOOL ChatDialog::HandleSysFaceId_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	int nFaceId = GetBetweenInt(p+2, cStart, cEnd, -1);

	CFaceInfo * lpFaceInfo = m_FaceList.GetFaceInfoById(nFaceId);
	if (lpFaceInfo != NULL)
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel_More(pRichEdit, strText.c_str(), mfontInfo); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, lpFaceInfo->m_strFileName.c_str(), 
			lpFaceInfo->m_nId, lpFaceInfo->m_nIndex);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}

BOOL ChatDialog::HandleSysFaceIndex(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	int nFaceIndex = GetBetweenInt(p+2, cStart, cEnd, -1);

	CFaceInfo * lpFaceInfo = m_FaceList.GetFaceInfoByIndex(nFaceIndex);
	if (lpFaceInfo != NULL)
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel(pRichEdit, strText.c_str()); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, lpFaceInfo->m_strFileName.c_str(), 
			lpFaceInfo->m_nId, lpFaceInfo->m_nIndex);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}


BOOL ChatDialog::HandleSysFaceIndex_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	int nFaceIndex = GetBetweenInt(p+2, cStart, cEnd, -1);

	CFaceInfo * lpFaceInfo = m_FaceList.GetFaceInfoByIndex(nFaceIndex);
	if (lpFaceInfo != NULL)
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel_More(pRichEdit, strText.c_str(), mfontInfo); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, lpFaceInfo->m_strFileName.c_str(), 
			lpFaceInfo->m_nId, lpFaceInfo->m_nIndex);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}

BOOL ChatDialog::HandleCustomPic(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, BOOL bPaste /* = FALSE */)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	tstring strFileName = GetBetweenString(p+2, cStart, cEnd);

	if (!strFileName.empty())
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel(pRichEdit, strText.c_str()); 
			strText = _T("");
		}

		//如果是粘贴进来的，是完整的路径，直接显示
		if (bPaste)
			_RichEdit_InsertFace(pRichEdit, strFileName.c_str(), -1, -1);
		else
		{
			CString strAllFile = "";
			strAllFile.Format("%s\\%s", g_config.szUserImage, strFileName.c_str());
			_RichEdit_InsertFace(pRichEdit, strAllFile, -1, -1);
		}

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}

BOOL ChatDialog::HandleCustomPic_More2(CRichEditUI * pRichEdit, tstring strFileName, BOOL bSend /* = FALSE */, BOOL bTimeout /* = FALSE */)
{
	CFileFind ff;
	BOOL bSuccess = TRUE;
	BOOL m_bTimeoutImage = FALSE;
	
	//////////////////////////////////////////////////////////////////////////
	// 检测文件文件是否存在
	CString strAllFile = "";
	strAllFile.Format("%s\\%s", g_config.szUserImage, strFileName.c_str());
	
	IMAGE_INFO *pII = new IMAGE_INFO;
	pII->bSend = FALSE;
	strcpy(pII->filename, strFileName.c_str());
	
	if (bTimeout)
	{
		strAllFile.Format("%s\\%s", g_config.szUserDisplay, "timeout.gif");
		pII->bFinish = TRUE;
	}
	else if (!ff.FindFile(strAllFile))
	{
		if(m_bTimeoutImage)
		{
			strAllFile.Format("%s\\%s", g_config.szUserDisplay, "timeout.gif");
			pII->bFinish = TRUE;
		}
		else
		{
			pII->bFinish = FALSE;
			bSuccess = FALSE;
			strAllFile.Format("%s\\%s", g_config.szUserDisplay, "loading.gif");
			if (!bSend)
				pII->tmR = time(NULL);
		}
	}
	else
		pII->bFinish = TRUE;

	m_arrII.Add(pII);

	_RichEdit_InsertFace(pRichEdit, strAllFile, -1, -1, pII);
	return bSuccess;
}

BOOL ChatDialog::HandleCustomPic_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	tstring strFileName = GetBetweenString(p+2, cStart, cEnd);

	if (!strFileName.empty())
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel_More(pRichEdit, strText.c_str(), mfontInfo); 
			strText = _T("");
		}

		if (!HandleCustomPic_More2(pRichEdit, strFileName))
		{
			// 如果有图片不存在，就刷新啊刷新
			if (!m_bRefreshing)
			{
				m_bRefreshing = TRUE;
				AddTimer(T_REFRESH, eTE_Refresh);
			}	
		}
		//_RichEdit_InsertFace(pRichEdit, strFileName.c_str(), -1, -1);
		//////////////////////////////////////////////////////////////////////////

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}


// "/f["系统表情id"]/s["系统表情index"]/c["自定义图片路径"]"
void ChatDialog::AddMsg_More(CRichEditUI * pRichEdit, LPCTSTR lpText, CFontInfo mfontInfo)
{
	if (NULL == pRichEdit || NULL == lpText || NULL == *lpText)
		return;

	tstring strText;

	for (LPCTSTR p = lpText; *p != _T('\0'); p++)
	{
		if (*p == _T('/'))
		{
			if (*(p+1) == _T('/'))
			{
				strText += *p;
				p++;
				continue;
			}
			else if (*(p+1) == _T('f'))
			{
				if (HandleSysFaceId_More(pRichEdit, p, strText, mfontInfo))
					continue;
			}
			else if (*(p+1) == _T('s'))
			{
				if (HandleSysFaceIndex_More(pRichEdit, p, strText, mfontInfo))
					continue;
			}
			else if (*(p+1) == _T('c'))
			{
				if (HandleCustomPic_More(pRichEdit, p, strText, mfontInfo))
					continue;
			}
		}
		strText += *p;
	}

	if (!strText.empty())
		_RichEdit_ReplaceSel_More(pRichEdit, strText.c_str(), mfontInfo);
}

// "/f["系统表情id"]/s["系统表情index"]/c["自定义图片路径"]"
void ChatDialog::AddMsg(CRichEditUI * pRichEdit, LPCTSTR lpText, BOOL bPaste /* = FALSE */)
{
	if (NULL == pRichEdit || NULL == lpText || NULL == *lpText)
		return;

	tstring strText;

	for (LPCTSTR p = lpText; *p != _T('\0'); p++)
	{
		if (*p == _T('/'))
		{
			if (*(p+1) == _T('/'))
			{
				strText += *p;
				p++;
				continue;
			}
			else if (*(p+1) == _T('f'))
			{
				if (HandleSysFaceId(pRichEdit, p, strText))
					continue;
			}
			else if (*(p+1) == _T('s'))
			{
				if (HandleSysFaceIndex(pRichEdit, p, strText))
					continue;
			}
			else if (*(p+1) == _T('c'))
			{
				if (HandleCustomPic(pRichEdit, p, strText, bPaste))
					continue;
			}
		}
		strText += *p;
	}

	if (!strText.empty())
		_RichEdit_ReplaceSel(pRichEdit, strText.c_str());
}

void ChatDialog::AddMsgToSendEdit(LPCTSTR lpText, BOOL bPaste /* = FALSE */)
{
	AddMsg(m_pSendEdit, lpText, bPaste);
	m_pSendEdit->EndDown();
}

void ChatDialog::AddMsgToRecvEdit_Text(LPCTSTR lpText)
{
	if (NULL == lpText || NULL == *lpText)
		return;

	m_pRecvEdit->SetAutoURLDetect(true);

	tstring strTime;
	strTime = FormatTime(time(NULL), _T("%H:%M:%S"));

	ITextServices * pTextServices = m_pRecvEdit->GetTextServices();
	RichEdit_SetSel(pTextServices, -1, -1);

	RichEdit_ReplaceSel(pTextServices, _T("\r\n"));
	RichEdit_ReplaceSel(pTextServices, lpText, _T("宋体"), 9, RGB(114, 114, 114), FALSE, FALSE, FALSE, FALSE, 300);
	RichEdit_ReplaceSel(pTextServices, _T("\r\n"));
	RichEdit_ReplaceSel(pTextServices, _T("\r\n"));
	RichEdit_SetStartIndent(pTextServices, 0);
	m_pRecvEdit->EndDown();

	pTextServices->Release();
}

void ChatDialog::AddMsgToRecvEdit(LPCTSTR lpText)
{
	if (NULL == lpText || NULL == *lpText)
		return;

	m_pRecvEdit->SetAutoURLDetect(true);

	tstring strTime;
	strTime = FormatTime(time(NULL), _T("%H:%M:%S"));

	ITextServices * pTextServices = m_pRecvEdit->GetTextServices();
	RichEdit_SetSel(pTextServices, -1, -1);

	TCHAR cText[2048] = {0};
	wsprintf(cText, _T("%s("), _T("王彭东"));
	RichEdit_ReplaceSel(pTextServices, cText, _T("宋体"), 9, RGB(0, 0, 255), FALSE, FALSE, FALSE, FALSE, 0);

	wsprintf(cText, _T("%u"), 23774312);
	RichEdit_ReplaceSel(pTextServices, cText, _T("宋体"), 9, RGB(0, 114, 193), FALSE, FALSE, TRUE, TRUE, 0);

	wsprintf(cText, _T(")  %s\r\n"), strTime.c_str());
	RichEdit_ReplaceSel(pTextServices, cText, _T("宋体"), 9, RGB(0, 0, 255), FALSE, FALSE, FALSE, FALSE, 0);

	AddMsg(m_pRecvEdit, lpText);

	RichEdit_ReplaceSel(pTextServices, _T("\r\n"));
	RichEdit_SetStartIndent(pTextServices, 0);
	m_pRecvEdit->EndDown();

	pTextServices->Release();
}

void ChatDialog::AddMsgToRecvEdit_Link(LPCTSTR lpText)
{
	if (NULL == lpText || NULL == *lpText)
		return;

	m_pRecvEdit->SetAutoURLDetect(true);
	ITextServices * pTextServices = m_pRecvEdit->GetTextServices();

	RichEdit_SetSel(pTextServices, -1, -1);
	RichEdit_ReplaceSel(pTextServices, lpText, STR_DEFAULT_FONT, 9, RGB(0, 114, 193), FALSE, FALSE, TRUE, TRUE, 500);

	RichEdit_ReplaceSel(pTextServices, _T("\r\n"));
	RichEdit_SetStartIndent(pTextServices, 0);
	m_pRecvEdit->SetEventMask(ENM_LINK);
	m_pRecvEdit->EndDown();

	pTextServices->Release();
}

void ChatDialog::AddMsgToRichEdit_More(CRichEditUI* m_pEdit, LPCTSTR lpText, CFontInfo mfontInfo, bool bisselfmsg/* =false */, bool bshowid/* =true */, CDuiString sid/* ="" */, CDuiString sname/* ="" */, CDuiString strSendTime/* ="" */, bool bFromXML/* =false */)
{
	if (NULL == lpText || NULL == *lpText || NULL == m_pEdit)
		return;
	// 公众号/投顾聊天室/投顾客户端 不显示ID
	if (eDialogType == eDIALOG_Public || eDialogType == eDIALOG_ChatTG || eDialogType == eDIALOG_Analys)
		bshowid = false;

	// 投顾客户端 对于疑似手机号码的字符特殊处理
	if (eDialogType == eDIALOG_Analys)
	{
#ifdef NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
		CString str1 = sname.GetData();
		CString strUser = ConvertIdToNoPhonrNum(str1);
		sname = strUser;
#endif // NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
	}

	m_pEdit->SetAutoURLDetect(true);

	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	CDuiString strTime = "";
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	ITextServices * pTextServices = m_pEdit->GetTextServices();
	RichEdit_SetSel(pTextServices, -1, -1);

	TCHAR cText[2048] = {0};

	if (bisselfmsg)
	{
		if (bshowid)
		{
			sprintf(cText, _T("%s("), myselft_.nick_name);
			RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 128, 64), FALSE, FALSE, FALSE, FALSE, 0);
			sprintf(cText, _T("%s"), myselft_.id);
			RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 114, 193), FALSE, FALSE, TRUE, TRUE, 0);
			if (strSendTime.IsEmpty())
				sprintf(cText, _T(") %s\r\n"), strTime.GetData());
			else
				sprintf(cText, _T(") %s\r\n"), strSendTime.GetData());
			RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 128, 64), FALSE, FALSE, FALSE, FALSE, 0);
		}
		else
		{
			sprintf(cText, _T("%s"), myselft_.nick_name);
			RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 128, 64), FALSE, FALSE, FALSE, FALSE, 0);
			if (strSendTime.IsEmpty())
				sprintf(cText, _T(" %s\r\n"), strTime.GetData());
			else
				sprintf(cText, _T(" %s\r\n"), strSendTime.GetData());
			RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 128, 64), FALSE, FALSE, FALSE, FALSE, 0);
		}
		if (bFromXML)
			AddMsg_More(m_pEdit, lpText, mfontInfo);
		else
			AddMsg(m_pEdit, lpText);
	}
	else
	{
		if (bshowid)
		{
			sprintf(cText, _T("%s("), sname);
			RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 0, 255), FALSE, FALSE, FALSE, FALSE, 0);
			sprintf(cText, _T("%s"), sid);
			RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 114, 193), FALSE, FALSE, TRUE, TRUE, 0);
			if (strSendTime.IsEmpty())
				sprintf(cText, _T(") %s\r\n"), strTime.GetData());
			else
				sprintf(cText, _T(") %s\r\n"), strSendTime.GetData());
			RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 0, 255), FALSE, FALSE, FALSE, FALSE, 0);
		}
		else
		{
			sprintf(cText, _T("%s"), sname);
			RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 0, 255), FALSE, FALSE, FALSE, FALSE, 0);
			if (strSendTime.IsEmpty())
				sprintf(cText, _T(" %s\r\n"), strTime.GetData());
			else
				sprintf(cText, _T(" %s\r\n"), strSendTime.GetData());
			RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 0, 255), FALSE, FALSE, FALSE, FALSE, 0);
		}
		AddMsg_More(m_pEdit, lpText, mfontInfo);
	}

	RichEdit_ReplaceSel(pTextServices, _T("\r\n"));
	RichEdit_SetStartIndent(pTextServices, 0);
	m_pEdit->EndDown();

	pTextServices->Release();
}

void ChatDialog::OnBtn_Send(TNotifyUI& msg)
{
	ITextServices * pTextServices = m_pSendEdit->GetTextServices();

	long lenth = m_pSendEdit->GetTextLength();
	tstring strText;
	RichEdit_GetText(pTextServices, strText);
	pTextServices->Release();

	if (strText.size() <= 0)
		return;

	m_pSendEdit->SetText(_T(""));
	m_pSendEdit->SetFocus();

	AddMsgToRecvEdit(strText.c_str());
}

BOOL ChatDialog::CheckTextInput()
{
	if( m_pSendEdit == NULL ) 
		return FALSE;
	// wuchao add at 2016-04-06 当输入内容为空时  不能发送 
	CDuiString str = m_pSendEdit->GetText() ; 
	if( str.IsEmpty() )
	{
		return FALSE ; 
	}
	long lenth = m_pSendEdit->GetTextLength();
	if (lenth <= 0)
		return FALSE;
	else if (lenth > MAX_TEXT_LEN)
	{
		::MessageBox(m_hWnd, _T("您输入的文本太长，无法发送！"), _T("提示"), MB_OK);
		return FALSE;
	}
#ifndef VER_UPIM_RONGYUN
	if (!g_MyClient.m_bConnect)
	{

		if (eDialogType == eDIALOG_ChatTG && !g_bUserKickOff)
		{
			// 如果是投顾聊天室，直接重新连接服务器
			ReConnect();
		}
		else
		{
			//int ret = ::MessageBox(m_hWnd, _T("您已离线，请确保您网络是否畅通!"), _T("提示"), MB_OK);

#ifdef IM_EXE_TO_UP
			// 不能是被踢下来  
			if(!g_bUserKickOff )
			{
				// 重连  
				ReConnect() ; 

				// 重连成功  才让发送消息 
				if( g_MyClient.m_bConnect)
					return TRUE ; 
				else 
				{
					::MessageBox(m_hWnd, _T("您已离线，请连接网络后,再发送消息!"), _T("提示"), MB_OK);
					return FALSE ;
				}
			}
			else
				return FALSE ; 
#else 
			 ::MessageBox(m_hWnd, _T("您已离线，请重新连接服务器!"), _T("提示"), MB_OK);
			return FALSE;
#endif 
		}
	}
#endif

	if (eDialogType == eDIALOG_ChatTG)
	{
		if (bControlImg)
		{
			::MessageBox(m_hWnd, _T("没有分析师在线，无法交流！"), _T("提示"), MB_OK);
			return FALSE;
		}
		if (bControlImg2)
		{
			::MessageBox(m_hWnd, _T("请选择分析师，进行交流！"), _T("提示"), MB_OK);
			return FALSE;
		}
		if (m_roomid == -1)
		{
			::MessageBox(m_hWnd, _T("请选择聊天室房间，进行交流！"), _T("提示"), MB_OK);
			return FALSE;
		}
	}

	return TRUE;
}

void ChatDialog::SendMsg_More()
{
	if (!CheckTextInput())
		return;
		
	//////////////////////////////////////////////////////////////////////////
	tmStart_ = time(NULL);


	ITextServices * pTextServices = m_pSendEdit->GetTextServices();
	tstring strText;
	RichEdit_GetText(pTextServices, strText);
	pTextServices->Release();

	COMPLEX_MSG_DATA* pData = GetMsgData_More(strText);
	if(!pData) 
	{
		if(m_nErrorCode != eMsgData_Null)
			::MessageBox(m_hWnd, GetLastErrorStr(), _T("提示"), MB_OK);
		return;
	}
#ifdef VER_UPIM_RONGYUN
	if (m_pIncodeID.IsEmpty())
	{
		::MessageBox(m_hWnd, "请选择相应问题后再进行提问！", _T("提示"), MB_OK);
		return;
	}

#endif
	//个人聊天，原始途径发送
	if (eDialogType == eDIALOG_Single)
	{
		g_MyClient.m_BigPackageSend.CreateMsg(friend_.id, pData);
	}
	//群发消息，批量途径发送
	else if (eDialogType == eDIALOG_GroupSend)
	{
		g_MyClient.m_MsgQueueSendMng.PushMsg(pData, eMsgFormat_Def, m_vtUPIMID, 0);
	}
	// 群组消息发送
	else if (eDialogType == eDIALOG_Group)
	{
		g_MyClient.m_BigPackageSend.CreateGroupMsg(atoi(friend_.numid), friend_.roomtype, pData);
	}
	// 公众号消息发送
	else if (eDialogType == eDIALOG_Public)
	{
		if (g_MyClient.IsClientLoginIn())
		{
			// 说明:客户发送消息的时候，第一次是不带接受者ID，待服务器有消息返回的时候，再填入。
			// innerID中复用为公众号ID
			g_MyClient.m_BigPackageSend.CreateMsg(m_PublicSveID, pData, eMsgFormat_Def, eMsgSend_Public, NULL, atoi(friend_.numid.GetData()), nRouteID);
		}
		else
		{
			// 说明:客服通过公众号收到消息，直接发回给这个客户
			// innerID中复用为公众号ID
			g_MyClient.m_BigPackageSend.CreateMsg(friend_.numid, pData, eMsgFormat_Def, eMsgSend_Public, NULL, atoi(friend_.Parentid.GetData()) );
		}
	}
	// 如果选择的是投顾，则看上面选中的老师是谁，发送给他
	else if (eDialogType == eDIALOG_ChatTG)
	{
		// innerID中房间ID
		CString strRoomID = "";
		strRoomID.Format("%d", m_roomid);
		g_MyClient.m_BigPackageSend.CreateMsg(strCurrentID, pData, eMsgFormat_Def, eMsgSend_Analyst, strRoomID);
	}
	else if (eDialogType == eDIALOG_Analys)
	{
		// innerID中房间ID
		g_MyClient.m_BigPackageSend.CreateMsg(friend_.numid, pData, eMsgFormat_Def, eMsgSend_Analyst, friend_.Parentid.GetData());
	}

	else if (eDialogType == eDIALOG_RYPublic)
	{
		Value jobj;
		FastWriter writer;
		CDuiString numid;
		if (m_PublicSveID.IsEmpty())
		{
			CString extrastr;
			extrastr.Format("%s|%s",friend_.numid.GetData(),m_pIncodeID.GetData());
			jobj["content"] = strText.c_str();
			jobj["extra"] = extrastr.GetBuffer(extrastr.GetLength());
			numid = friend_.numid;
		}
		else{
			jobj["content"] = strText.c_str();
			jobj["extra"] = "";
			numid = m_PublicSveID;
		}
		string strjson = writer.write(jobj);
		int messageId = 0;

		messageId = g_RongCloudDll.m_pSaveMessage(numid, newMSG_PRIVATE, "RC:TxtMsg", numid, txtutil::convert_ansi_to_wcs(strjson).c_str(), "", "");
		g_RongCloudDll.m_psendMessage(numid,newMSG_PRIVATE,newMSG_N,"RC:TxtMsg",
			txtutil::convert_ansi_to_wcs(strjson).c_str(),"", " ",messageId,(PublishAckListenerCallback)&sendMessageCallback);
	}

	m_pSendEdit->SetText(_T(""));
	m_pSendEdit->SetFocus();

	AddMsgToRichEdit_More(m_pRecvEdit, strText.c_str(), g_userconfig.m_fontInfo, true);

	//////////////////////////////////////////////////////////////////////////
	// 文本消息中转换成简要信息显示在界面上，图片显示[图片],表情显示[表情]
	CString strDispairMsg = ConvertMsgToChiefMsg(eMsgFormat_Txt, strText.c_str());
	// 填充最近联系人
#ifndef VER_UPIM_RONGYUN
	g_pFrame->ModifyDisRecentInfo(CDuiString(friend_.numid.GetData()), ""/*CDuiString(friend_.nick_name.GetData())*/, CDuiString(strDispairMsg), GetNowTimeHourAndMin(), atoi(friend_.Parentid.GetData()));
#endif
}

//发送消息，在这里处理
void ChatDialog::SendMsg()
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kInputRichEditControlName));
	if( pRichEdit == NULL ) return;
	pRichEdit->SetFocus();
	CDuiString sText = pRichEdit->GetTextRange(0, pRichEdit->GetTextLength());
	if( sText.IsEmpty() ) return;

	long mTextLen = pRichEdit->GetTextLength();
	if (mTextLen > MAX_TEXT_LEN)
	{
		::MessageBox(m_hWnd, _T("您输入的文本太长，无法发送！"), _T("提示"), MB_OK);
		return;
	}

	//群聊，批量途径发送
	if (eDialogType == eDIALOG_GroupSend)
	{
		g_MyClient.m_MsgQueueSendMng.PushMsg("", eMsgFormat_Txt, m_vtUPIMID, 0);
	}
	//个人聊天，原始途径发送
	else
	{
		COMPLEX_MSG_DATA* pData = GetMsgData();	
		g_MyClient.m_BigPackageSend.CreateMsg(friend_.id, pData);
	}
	pRichEdit->SetText(_T(""));

	//////////////////////////////////////////////////////////////////////////
	pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kViewRichEditControlName));
	if( pRichEdit == NULL ) return;
	long lSelBegin = 0, lSelEnd = 0;

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);
	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	CDuiString strTime = "";
	strTime.Format("%s %04d-%02d-%02d %02d:%02d:%02d\n", myselft_.nick_name.GetData(), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	pRichEdit->ReplaceSel(strTime, false);

	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0, 128, 64);
	cf.dwEffects = 0;

	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(sText.GetData(), false);

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(_T("\n"), false);

	cf.crTextColor = RGB(0, 0, 0);
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 220;
	pRichEdit->SetParaFormat(pf);

	pRichEdit->EndDown();
}

/*
* 用XML来存储复杂消息内容，包括，自定义的字体类型，文本，图片信息，表情等
*/
COMPLEX_MSG_DATA* ChatDialog::GetMsgData(BOOL bOnLine/* =TRUE */,CString strFileInfo/* ="" */,CString strProductName/* ="" */)
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kInputRichEditControlName));
	if( pRichEdit == NULL ) return NULL;
	pRichEdit->SetFocus();

	CDuiString sText = pRichEdit->GetTextRange(0, pRichEdit->GetTextLength());
	if( sText.IsEmpty() ) return NULL;
	long nLen = sText.GetLength();

	if(nLen<=0) 
	{
		m_nErrorCode = eMsgData_Null;
		return NULL;
	}
	if(nLen>=MAX_TEXT_LEN)
	{
		m_nErrorCode = eMsgData_TextMax;
		return NULL;
	}

	COMPLEX_MSG_DATA* pData = new COMPLEX_MSG_DATA;
	if(!pData) return NULL;
	memset(pData,0,sizeof(COMPLEX_MSG_DATA));

	TiXmlDocument xmlDoc;
	TiXmlElement rootEle("msg");
	TiXmlNode* pNode = xmlDoc.InsertEndChild(rootEle);
	TiXmlElement* pRootElm = pNode->ToElement();

	TiXmlElement textElm("text");
	TiXmlText xmlText(sText.GetData());
	xmlText.SetCDATA(true);
	TiXmlNode *pTextNode = pRootElm->InsertEndChild(textElm);
	pTextNode->InsertEndChild(xmlText);
	TiXmlElement iamgeElm("image");
	TiXmlElement* pChildeElm = NULL;

	TiXmlPrinter printer;
	printer.SetIndent("");
	printer.SetLineBreak("");
	xmlDoc.Accept( &printer );
	LPCTSTR pBuf = printer.CStr();
	nLen = printer.Size();
	if(nLen <= 0) 
	{
		m_nErrorCode = eMsgData_Xml;
		DeleteMsgData(pData);
		return NULL;
	}
	if(nLen >= MAX_TEXT_LEN)
	{
		m_nErrorCode = eMsgData_TextMax;
		DeleteMsgData(pData);
		return NULL;
	}
	pData->pBuf = new char[nLen+1];
	strncpy(pData->pBuf, pBuf, nLen);
	pData->pBuf[nLen] = 0;
	pData->nTextLen = nLen;
	pData->sSend = -1;

	return pData;
}

/*
* 用XML来存储复杂消息内容，包括，自定义的字体类型，文本，图片信息，表情等
*/
COMPLEX_MSG_DATA* ChatDialog::GetMsgData_More(tstring strText, BOOL bOnLine/* =TRUE */,CString strFileInfo/* ="" */,CString strProductName/* ="" */)
{
	if( m_pSendEdit == NULL ) return NULL;

	CDuiString sText = strText.c_str();
	if( sText.IsEmpty() ) return NULL;
	long nLen = sText.GetLength();

	if(nLen <= 0) 
	{
		m_nErrorCode = eMsgData_Null;
		return NULL;
	}
	else if(nLen >= MAX_TEXT_LEN)
	{
		m_nErrorCode = eMsgData_TextMax;
		return NULL;
	}

	COMPLEX_MSG_DATA* pData = new COMPLEX_MSG_DATA;
	if(!pData) return NULL;
	memset(pData, 0, sizeof(COMPLEX_MSG_DATA));

	TiXmlDocument xmlDoc;
	TiXmlElement rootEle("msg");
	TiXmlNode* pNode = xmlDoc.InsertEndChild(rootEle);
	TiXmlElement* pRootElm = pNode->ToElement();

	//////////////////////////////////////////////////////////////////////////
	pRootElm->SetAttribute("f", g_userconfig.m_fontInfo.m_strName.c_str());		// 字体
	pRootElm->SetAttribute("b", g_userconfig.m_fontInfo.m_bBold);				// 粗细
	pRootElm->SetAttribute("i", g_userconfig.m_fontInfo.m_bItalic);				// 斜体
	pRootElm->SetAttribute("u", g_userconfig.m_fontInfo.m_bUnderLine);			// 下划线
	pRootElm->SetAttribute("s", g_userconfig.m_fontInfo.m_nSize);				// 字体大小
	CString strTmpFmt = "";
	strTmpFmt.Format("%i", g_userconfig.m_fontInfo.m_clrText);
	pRootElm->SetAttribute("c",strTmpFmt);							// 颜色
	//////////////////////////////////////////////////////////////////////////
	TiXmlElement textElm("text");
	TiXmlText xmlText(sText.GetData());
	xmlText.SetCDATA(true);
	TiXmlNode *pTextNode = pRootElm->InsertEndChild(textElm);
	pTextNode->InsertEndChild(xmlText);
	TiXmlElement iamgeElm("image");
	TiXmlElement* pChildeElm = NULL;

	TiXmlPrinter printer;
	printer.SetIndent("");
	printer.SetLineBreak("");
	xmlDoc.Accept( &printer );
	LPCTSTR pBuf = printer.CStr();
	nLen = printer.Size();
	if(nLen <= 0) 
	{
		m_nErrorCode = eMsgData_Xml;
		DeleteMsgData(pData);
		return NULL;
	}
	if(nLen >= MAX_TEXT_LEN)
	{
		m_nErrorCode = eMsgData_TextMax;
		DeleteMsgData(pData);
		return NULL;
	}
	pData->pBuf = new char[nLen+1];
	strncpy(pData->pBuf, pBuf, nLen);
	pData->pBuf[nLen] = 0;
	pData->nTextLen = nLen;
	pData->sSend = -1;

	HandleObjectInfo(strText, pData);
	return pData;
}

BOOL ChatDialog::GetObjectInfoPic(LPCTSTR& p, tstring& strFileName)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	strFileName = GetBetweenString(p+2, cStart, cEnd);

	if (!strFileName.empty())
	{
		CString strAllFile = "";
		strAllFile.Format("%s\\%s", g_config.szUserImage, strFileName.c_str());

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}

void ChatDialog::HandleObjectInfo(tstring strText, COMPLEX_MSG_DATA* pData)
{
	//////////////////////////////////////////////////////////////////////////
	//根据strText中的/c[]来判断图片信息，存进pData
	int nObjCount = 0;
	LPCTSTR lpText = strText.c_str();
	for (LPCTSTR p = lpText; *p != _T('\0'); p++)
	{
		if (*p == _T('/') && *(p+1) == _T('c'))
		{
			tstring strFileName = "";
			if (GetObjectInfoPic(p, strFileName))
			{
				nObjCount ++;
				continue;
			}
		}	
	}

	pData->nObjCount = nObjCount;
	pData->pObjectInfo = new PIC_OBJECT_INFO[nObjCount];
	memset(pData->pObjectInfo, 0, sizeof(PIC_OBJECT_INFO)*nObjCount);

	int nthisObj = 0;
	for (LPCTSTR p = lpText; *p != _T('\0'); p++)
	{
		if (*p == _T('/') && *(p+1) == _T('c'))
		{
			tstring strFileName = "";
			if (GetObjectInfoPic(p, strFileName))
			{
				pData->pObjectInfo[nthisObj].nType = 1;
				pData->pObjectInfo[nthisObj].ucState = 1;
				strcpy(pData->pObjectInfo[nthisObj].szFile, strFileName.c_str());
				nthisObj ++;
				continue;
			}
		}	
	}
	return;
}


void ChatDialog::DeleteMsgData(COMPLEX_MSG_DATA*& pMsgData)
{
	if(!pMsgData) return;
	if(pMsgData->pObjectInfo)
	{
		for(int j=0; j<pMsgData->nObjCount; j++)
			TDEL(pMsgData->pObjectInfo[j].pBuf);
		TDELARRAY(pMsgData->pObjectInfo);
	}
	TDEL(pMsgData->pBuf);
	TDEL(pMsgData);
}

BOOL ChatDialog::ReadXmlMsgData(LPCSTR pszBuf, CRichEditUI* pRichEdit)
{
	TiXmlDocument xmlDoc;
	xmlDoc.Parse(pszBuf);

	if(xmlDoc.Error()) 
	{
		printf( "Error in %s: %s\n", xmlDoc.Value(), xmlDoc.ErrorDesc() );
		return FALSE;
	}

	CString strTmpFmt,strOld;
	TiXmlNode* pRootNode = xmlDoc.FirstChild("msg");
	if(!pRootNode) 	return FALSE;
	TiXmlElement* pRootElm =pRootNode->ToElement();
	if(!pRootElm) 	return FALSE;
	TiXmlNode* pItemNode = pRootNode->FirstChild("text"); 
	if(!pItemNode) return FALSE;
	TiXmlNode* pTextNode = pItemNode->FirstChild();
	if(!pTextNode) return FALSE;

	CString strText = pTextNode->Value();
	strText += "\r\n";

	//////////////////////////////////////////////////////////////////////////
	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	long lSelBegin = 0, lSelEnd = 0;
	lSelEnd = lSelBegin = pRichEdit->GetTextLength();

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(strText, false);
	pRichEdit->SetSel(-1, -1);

	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0, 0, 0);
	cf.dwEffects = 0;

	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 220;
	pRichEdit->SetParaFormat(pf);

	return TRUE;
}

void ChatDialog::InsertChatHistory_More(RECEIVED_MSG *pMsg, LPCTSTR szSendName, LPCTSTR szSendId)
{
	CDuiString strTime = "";
	CDuiString strspName = "";
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", 
		pMsg->senddate/10000, (pMsg->senddate%10000)/100, pMsg->senddate%100, pMsg->sendtime/10000, (pMsg->sendtime%10000)/100, pMsg->sendtime%100);

	//从公众号信息中 判断当前发来消息的客服ID m_PublicSveID
	if (pMsg->msgtype == eMsgSend_Public||eMsgSend_NewPublic == pMsg->msgtype)
		m_PublicSveID = pMsg->imid;

	if (pMsg->format == eMsgFormat_System)
		strspName = _T("系统消息");
	else
		strspName = szSendName;

	if (pMsg->format == eMsgFormat_Txt || pMsg->format == eMsgFormat_AutoReply)
	{
		AddMsgToRichEdit_More(m_pRecvEdit, pMsg->msg, g_userconfig.m_fontInfo, false, true, szSendId, strspName, strTime);
	}
	if (pMsg->format == eMsgFormat_Shake)
	{
		ShakeWindow();
		AddMsgToRichEdit_More(m_pRecvEdit, "对方给您发送了一个窗口抖动", g_userconfig.m_fontInfo, false, true, szSendId, strspName, strTime);
	}
	if (pMsg->format == eMsgFormat_Def)
	{
		TiXmlDocument xmlDoc;
		xmlDoc.Parse(pMsg->msg);

		if(xmlDoc.Error()) 
		{
			printf( "Error in %s: %s\n", xmlDoc.Value(), xmlDoc.ErrorDesc() );
			return;
		}

		CString strTmpFmt,strOld;
		TiXmlNode* pRootNode = xmlDoc.FirstChild("msg");
		if(!pRootNode) 	return;
		TiXmlElement* pRootElm =pRootNode->ToElement();
		if(!pRootElm) 	return;

		//////////////////////////////////////////////////////////////////////////
		CFontInfo m_ufontInfo;
		LPCSTR pszValue = "";
		pszValue = pRootElm->Attribute("f");				// 字体
		m_ufontInfo.m_strName = pszValue;
		pszValue = pRootElm->Attribute("b");				// 粗细
		m_ufontInfo.m_bBold = atoi(pszValue);
		pszValue = pRootElm->Attribute("i");				// 斜体
		m_ufontInfo.m_bItalic = atoi(pszValue);
		pszValue = pRootElm->Attribute("u");				// 下划线
		m_ufontInfo.m_bUnderLine = atoi(pszValue);
		pszValue = pRootElm->Attribute("s");				// 字体大小
		m_ufontInfo.m_nSize = atoi(pszValue);
		pszValue = pRootElm->Attribute("c");				// 颜色
		m_ufontInfo.m_clrText = atoi(pszValue);

		//////////////////////////////////////////////////////////////////////////

		TiXmlNode* pItemNode = pRootNode->FirstChild("text"); 
		if(!pItemNode) return;
		TiXmlNode* pTextNode = pItemNode->FirstChild();
		if(!pTextNode) return;

		CString strText = pTextNode->Value();
		AddMsgToRichEdit_More(m_pRecvEdit, strText, m_ufontInfo, false, true, szSendId, strspName, strTime);
	}
}

void ChatDialog::InsertChatHistory(RECEIVED_MSG *pMsg, LPCTSTR szSendName)
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kViewRichEditControlName));
	if( pRichEdit == NULL ) return;
	long lSelBegin = 0, lSelEnd = 0;

	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);

	CDuiString strTime = "";
	CDuiString strTimeAndName = "";
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d\n", 
		pMsg->senddate/10000, (pMsg->senddate%10000)/100, pMsg->senddate%100, pMsg->sendtime/10000, (pMsg->sendtime%10000)/100, pMsg->sendtime%100);
	if (pMsg->format == eMsgFormat_System)
		strTimeAndName.Format("系统消息 %s",strTime);
	else
		strTimeAndName.Format("%s %s", szSendName, strTime);

	pRichEdit->ReplaceSel(strTimeAndName, false);

	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0, 0, 255);
	cf.dwEffects = 0;

	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);
	pRichEdit->SetSel(-1, -1);

	if (pMsg->format == eMsgFormat_Txt || pMsg->format == eMsgFormat_AutoReply)
	{
		CString strMsg = "";
		strMsg.Format("%s\n",pMsg->msg);
		pRichEdit->ReplaceSel(strMsg, false);
	}
	if (pMsg->format == eMsgFormat_Def)
	{
		ReadXmlMsgData(pMsg->msg, pRichEdit);
	}

	pRichEdit->EndDown();
}

LPCTSTR  ChatDialog::GetLastErrorStr()
{
	switch (m_nErrorCode)
	{
	case eMsgData_Suc:
		return "成功";
	case eMsgData_Null:
		return "消息不能为空";
	case eMsgData_TextMax:
		return "发送文本过长";
	case eMsgData_ImgMax:
		return "一次发送图片数量过多";
	case eMsgData_Xml:
		return "消息发送处理出错";
	case eMsgData_OfflineImg:
		return "离线不能发送图片";
	}
	return "";
}

/*
公众号响应消息规则：
1.问题路由，如果点击链接，直接转接客服专员。
2.其余路由，如果发送消息，再转接到客服专员。
*/
void ChatDialog::RecvPublicInfo()
{
#ifndef VER_UPIM_RONGYUN

	if (g_MyClient.IsClientLoginIn())
	{
		PUB_ITER iter_ = m_mapPublicInfo.find(atoi(friend_.numid));

		AddMsgToRichEdit_More(m_pRecvEdit, iter_->second.szWelcome, g_userconfig.m_fontInfo, false, false, friend_.id.GetData(), friend_.nick_name.GetData(), "");

		// 没有问题路由，就没有选项让你选择
		if (iter_->second.pVtRoute != NULL)
		{
			std::vector<ROUTE_INFO>::iterator iter_info;
			for (iter_info = iter_->second.pVtRoute->begin(); iter_info != iter_->second.pVtRoute->end(); iter_info++)
			{
				CString strTmp = _T("");
				strTmp.Format("%d.%s", iter_info->nProblemtype, iter_info->m_RName);
				AddMsgToRecvEdit_Link(strTmp);
			}
		}
	}
	else
	{

	}
#else
	std::map<CString,NEWPUBLIC_INFO>::iterator iter_ = g_mapNewPublicInfo.find(friend_.numid.GetData());
	if (iter_!=g_mapNewPublicInfo.end())
	{
		char welcomstr[] = "您好，欢迎使用在线客服。请点击下列连接，快速准确获取答案!";
		AddMsgToRichEdit_More(m_pRecvEdit, welcomstr, g_userconfig.m_fontInfo, false, false, friend_.id.GetData(), friend_.nick_name.GetData(), "");
		if (iter_->second.pIncode != NULL)
		{
			std::vector<IncodeInfo>::iterator iter_info;
			for (iter_info = iter_->second.pIncode->begin(); iter_info != iter_->second.pIncode->end(); iter_info++)
			{
				CString strTmp = _T("");
				strTmp.Format("%d.%s", iter_info->accesscode, iter_info->accessname);
				AddMsgToRecvEdit_Link(strTmp);
			}
		}
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 测试
BOOL ChatDialog::LoginSuccess()
{
	myselft_.id = g_loginname;
	myselft_.nick_name = g_loginname;

	if (g_dwRoomID > 0)
		ChangeChatRoom(g_dwRoomID);

	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

// 改变聊天室 
void ChatDialog::ChangeChatRoom(DWORD rid)
{
	if (rid == m_roomid)
		return;
	m_vtInfo.clear();
	nAnalystCtrIndex = 0;
	m_analystid = -1;
	ShowFXS(FALSE);
	m_roomid = rid;
	
	bFristReturn = TRUE;
	bControlImg = FALSE;
	bControlImg2 = FALSE;
	bselectFXS = FALSE;
	// 单点击新的聊天室，需要重新取分析师数据

	// 第一步：网络接口取得所有的分析师信息
	GetAllFXSInfo(rid);
	if (m_vtInfo.size() <= 0)
	{
		// 取不到分析师，就显示图片“房间暂无分析师”
		ShowPic(1, 1);
		ShowPic(2, 0); // 隐藏请选择分析师
	}
	else
	{
		// 第二步，根据接口中的分析师头像url，下载分析师头像(如果文件已经下载过，不需要下载)
		DownLoadAllFXSPic();

		// 第三步，获取分析师的在线状态(首次协议取得，后面定时刷新)
		GetAllFXSState();

		// 第四步，控制显示(根据返回值信息)
	}
	return;
}

// 更换选择的老师
// 需要在接收区域重新显示与此老师的对话
void ChatDialog::ChangeLaoshi(int nindex)
{
	if (nindex == m_analystid)
		return;
	m_analystid = nindex;
	if (m_pTgHead[nindex])
	{
		if (!m_pTgHead[nindex]->IsFocused())
			m_pTgHead[nindex]->Selected(TRUE);
	}
	
	bselectFXS = TRUE;
	CString strTmp = "";
	strTmp.Format("您选择了分析师 [%d]", nindex);
	m_pRecvEdit->SetText(_T(""));
	//AddMsgToRecvEdit_Link(strTmp);

	//strCurrentID
	for (std::vector<AnalystInfo>::iterator iterator = m_vtInfo.begin(); iterator != m_vtInfo.end(); iterator++)
	{
		if (nindex == iterator->nControlIndex)
		{
			strCurrentID = IntToStr(iterator->AnalystId);
			strCurrentName = iterator->AnalystName;

			// 将每个房间选择的分析师ID记录下来
			m_mapRoomAnalys[m_roomid] = iterator->AnalystId;
		}
	}
	if (bControlImg2)
	{
		ShowPic(2, 0); //如果有人在线就显示聊天框
	}

	//////////////////////////////////////////////////////////////////////////
	// addtitle
// 	if (!m_pRecvEdit)
// 		InitWindow();
// 	if (m_pRecvEdit)
//		ShowRecord();

 //	if (m_pRecvEdit)
 //		PostMessage(UM_USER_SHOWRECORD, 0, 0);

	AddTimer(T_RECORDSHOW, eTE_RecordShow);

	return;
}

//////////////////////////////////////////////////////////////////////////
// 显示图片逻辑：
BOOL ChatDialog::ShowPic(int nType, int nValue /* = 1 */)
{
	// nValue = 0 表示显示控件 / nValue = 1 表示显示图片
	if (nType == 1)
	{
		bControlImg = (nValue == 1) ? 1 : 0;
		bool bShow =  (nValue == 1) ? true : false;
		if (g_pCreateTG2)
			g_pCreateTG2->ShowWindow(bShow);
	}
	else if (nType == 2)
	{
		bControlImg2 = (nValue == 1) ? 1 : 0;
		bool bShow =  (nValue == 1) ? true : false;
		if (g_pCreateTG1)
			g_pCreateTG1->ShowWindow(bShow);
	}
	return TRUE;
}

void ChatDialog::GetRootRect(CRect &rectInit)
{
	RECT m_rcItem = m_PaintManager.GetRoot()->GetPos();
	rectInit.left = m_rcItem.left;
	rectInit.right = m_rcItem.right;
	rectInit.bottom  = m_rcItem.bottom;
	rectInit.top = m_rcItem.top;
	return;
}

//////////////////////////////////////////////////////////////////////////
// 这个是dll模式老版本中嵌入客户端投顾聊天界面看见的历史消息
void ChatDialog::ShowRecord()
{
	std::vector<CHAT_RECORD2> vtRecordset;

	CString m_strfromID;
	CString m_strToID;
	// 个人节点
	m_strfromID.Format("%s", strCurrentID);
	m_strToID.Format("%s", g_loginname);
	m_strToID.Trim();

	// 得到总条数
	long nAllRecord = g_SqliteOp.QueryRecordsetCount(eRecord_TgRoom, m_strfromID, m_strToID, FALSE, 0, m_roomid);

	if (nAllRecord == 0)
	{
		if (eDialogType == eDIALOG_ChatTG)
		{
			AddMsgToRichEdit_More(m_pRecvEdit, "感谢您对我的关注！！！", g_userconfig.m_fontInfo, false, true, (CDuiString)strCurrentID, (CDuiString)strCurrentName);
		}
	}
	else
	{
		// 取前N条记录 展示
		int nEnd = nAllRecord;
		int nStart = (nAllRecord - TG_RECORD_NUM) > 0 ? (nAllRecord - TG_RECORD_NUM) : 0;

		if(!g_SqliteOp.QueryOnePageRecord(vtRecordset, eRecord_TgRoom, nStart, nEnd, m_strfromID, m_strToID, FALSE, 0, m_roomid))
			return ;

		if (vtRecordset.size() > 0)	
		{
			for(int index=0; index < (int)vtRecordset.size(); index++)
			{
				InsertChatHistory_Formdb(m_pRecvEdit, &vtRecordset[index], strCurrentName, strCurrentID);
				TDELARRAY((&vtRecordset[index])->msg);
			}
		}
	}
}

void ChatDialog::InsertChatHistory_Formdb(CRichEditUI* m_pEdit, CHAT_RECORD2 *pRec, LPCTSTR szSendName, LPCTSTR szSendId)
{
	long nseq = pRec->seq;
	long senddate = pRec->senddate;
	long sendtime = pRec->sendtime;

	CDuiString strTime = "";
	CDuiString strspName = "";
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", 
		senddate/10000, (senddate%10000)/100, senddate%100, sendtime/10000, (sendtime%10000)/100, sendtime%100);

	if (pRec->type != eRecord_TgRoom)
		return;

	if (pRec->format == eMsgFormat_Def)
	{
		TiXmlDocument xmlDoc;
		xmlDoc.Parse(pRec->msg);

		if(xmlDoc.Error()) 
		{
			printf( "Error in %s: %s\n", xmlDoc.Value(), xmlDoc.ErrorDesc() );
			return;
		}

		CString strTmpFmt,strOld;
		TiXmlNode* pRootNode = xmlDoc.FirstChild("msg");
		if(!pRootNode) 	return;
		TiXmlElement* pRootElm =pRootNode->ToElement();
		if(!pRootElm) 	return;

		//////////////////////////////////////////////////////////////////////////
		CFontInfo m_ufontInfo;
		LPCSTR pszValue = "";
		pszValue = pRootElm->Attribute("f");				// 字体
		m_ufontInfo.m_strName = pszValue;
		pszValue = pRootElm->Attribute("b");				// 粗细
		m_ufontInfo.m_bBold = atoi(pszValue);
		pszValue = pRootElm->Attribute("i");				// 斜体
		m_ufontInfo.m_bItalic = atoi(pszValue);
		pszValue = pRootElm->Attribute("u");				// 下划线
		m_ufontInfo.m_bUnderLine = atoi(pszValue);
		pszValue = pRootElm->Attribute("s");				// 字体大小
		m_ufontInfo.m_nSize = atoi(pszValue);
		pszValue = pRootElm->Attribute("c");				// 颜色
		m_ufontInfo.m_clrText = atoi(pszValue);

		//////////////////////////////////////////////////////////////////////////

		TiXmlNode* pItemNode = pRootNode->FirstChild("text"); 
		if(!pItemNode) return;
		TiXmlNode* pTextNode = pItemNode->FirstChild();
		if(!pTextNode) return;

		CString strText = pTextNode->Value();

		bool bisselfmsg = EqualUPID(pRec->fromid, g_MyClient.m_strUserid) ? 1 : 0;
		// 如果记录中是自己的ID就显示自己的用户名。如果是他人的ID就显示friend_.nick_name
		//strspName = bisselfmsg ? g_loginname : szSendName;
		strspName = bisselfmsg ? g_loginname : friend_.nick_name.GetData();

		AddMsgToRichEdit_More(m_pEdit, strText, m_ufontInfo, bisselfmsg, false, pRec->fromid, strspName, strTime, true);
	}
}

void ChatDialog::OnUserStateChange(WPARAM wParam, LPARAM lParam)
{
	BOOL bNeedAutoSelect = FALSE;
	if (bFristReturn)
		bNeedAutoSelect = TRUE;

	ShowFXS(TRUE, (int)wParam, (int)lParam);

	// 将保存下来的已经选择的分析师信息
	if (bNeedAutoSelect)
	{
		// 看这个房间之前是否已经选择了分析师
		DWORD dwAnalysID = -1;

		map <DWORD, DWORD>::iterator mpiterator = m_mapRoomAnalys.find(m_roomid);
		if (mpiterator != m_mapRoomAnalys.end())
		{
			dwAnalysID = mpiterator->second;
			for (std::vector<AnalystInfo>::iterator iterator = m_vtInfo.begin(); iterator != m_vtInfo.end(); ++iterator)
			{
				if (dwAnalysID == iterator->AnalystId)
				{
					ChangeLaoshi(iterator->nControlIndex);
					return;
				}
			}			
		}
	}
	return;
}

void ChatDialog::ShowFXS(BOOL bShow /* = TRUE */, int bChange /* = 0 */, int bNoOneOnline /* = 0 */)
{
	if (!bShow)
	{
		// 所有都隐藏
		for (int nIndex = 0; nIndex < TG_LAOSHINUM; nIndex++)
		{
			if (m_pTgHead[nIndex])
			{
				m_pTgHead[nIndex]->SetVisible(false);
				m_pTgHead[nIndex]->Selected(false);
			}
		}
	}
	else
	{
		// 收到用户状态包之后处理
		if (m_vtInfo.size() <=0) 
			return;

		if (bFristReturn)
		{
			//////////////////////////////////////////////////////////////////////////
			// 如果是进了房间第一次请求用户是否在线
			if (bNoOneOnline == 1)
			{
				if (!bControlImg)
				{
					ShowPic(1, 1); // 就显示图片“房间暂无分析师
					ShowPic(2, 0); // 隐藏请选择分析师
				}
			}
			else
			{
				ShowPic(1, 0); //如果有人在线就隐藏图片“房间暂无分析师
				if (!bControlImg2 && !bselectFXS)
					ShowPic(2, 1); //如果有人在线就显示图片选择分析师
			}
			bFristReturn = FALSE;
		}
		else
		{
			if (!bNoOneOnline)
			{
				if (bControlImg)
					ShowPic(1, 0); //如果有人在线就隐藏图片“房间暂无分析师
				if (!bControlImg2 && !bselectFXS)
					ShowPic(2, 1); //如果有人在线就显示图片选择分析师
			}
		}

		// 如果原来的分析师是在线的，现在离线了，图像灰掉/ 如果原来是离线的，现在上线了，显示出头像
		for (std::vector<AnalystInfo>::iterator iterator = m_vtInfo.begin(); iterator != m_vtInfo.end(); ++iterator)
		{
			DWORD AnalystId = iterator->AnalystId;
			CDuiString strAnalyst = IntToStr(AnalystId);
			int nState = allUserState[strAnalyst];

			CString strHead = "";		// 在线的正常头像
			CString strGrayHead = "";		// 离线时的灰化头像

			if (strlen(g_config.szUserHead) == 0)
			{
				// g_config.szUserHead 路径 某种情况下可能为空,手动调用，获取正确目录
				LoginSuc_Ex();
				USER_LOG("# AnalystInfo UserHead DIR IS NULL");
			}
			strHead.Format("%s\\%s", g_config.szUserHead, iterator->sLocalImg);
			strGrayHead.Format("%s\\%s", g_config.szUserHead, iterator->sLocalGrayImg);
			TCHAR szBuf[MAX_PATH] = {0};
			// 如果原来的分析师是在线的，现在离线了，图像灰掉
			if (nState == 0 && iterator->bIsOnLine == 1)
			{
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' dest='5,5,67,67'"), strGrayHead);
				if (m_pTgHead[iterator->nControlIndex])
				{
					m_pTgHead[iterator->nControlIndex]->SetForeImage(szBuf);
					iterator->bIsOnLine = FALSE;
					iterator->bIsShow = TRUE;
				}
			}
			// 如果原来是离线的，现在上线了，显示出头像
			if (nState == 1 && iterator->bIsOnLine == 0)
			{
				// 如果已经显示了，点亮图标
				if (iterator->bIsShow)
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' dest='5,5,67,67'"), strHead);
					if (m_pTgHead[iterator->nControlIndex])
					{
						m_pTgHead[iterator->nControlIndex]->SetForeImage(szBuf);
						iterator->bIsOnLine = TRUE;
						iterator->bIsShow = TRUE;
					}
				}
				// 如果没显示，添加
				else
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' dest='5,5,67,67'"), strHead);
					int nControlIndex = nAnalystCtrIndex;
					if (m_pTgHead[nControlIndex])
					{
						m_pTgHead[nControlIndex]->SetForeImage(szBuf);
						m_pTgHead[nControlIndex]->SetText(iterator->AnalystName);
						m_pTgHead[nControlIndex]->SetVisible();

						iterator->bIsOnLine = TRUE;
						iterator->bIsShow = TRUE;
						iterator->nControlIndex = nAnalystCtrIndex;
						nAnalystCtrIndex++;
					}
				}
			}
		}
	}
}

BOOL ChatDialog::DownLoadAllFXSPic()
{
	for (std::vector<AnalystInfo>::iterator iterator = m_vtInfo.begin(); iterator != m_vtInfo.end(); ++iterator)
	{
		DWORD AnalystId = iterator->AnalystId;
		CString strIMG = iterator->sImg;
		if (strIMG.IsEmpty())	// 如果没有URL
		{
			iterator->sImg = "http://weixin.upchinafund.com/weixin/images/head2.jpg";
			iterator->sDownImg = "head.jpg";
			iterator->sLocalImg = "head_n.jpg";
			iterator->sLocalGrayImg = "head_g.jpg";
		}
		else
		{
			iterator->sDownImg.Format("%d.jpg", AnalystId);
			iterator->sLocalImg.Format("%d_n.jpg", AnalystId);
			iterator->sLocalGrayImg.Format("%d_g.jpg", AnalystId);
		}
		CString strDown = "";		// 下载的头像
		CString strHead = "";		// 在线的正常头像
		CString strHead2 = "";		// 离线时的灰化头像
		if (strlen(g_config.szUserHead) == 0)
		{
			// g_config.szUserHead 路径 某种情况下可能为空,手动调用，获取正确目录
			LoginSuc_Ex();
			USER_LOG("# UserHead DIR IS NULL");
		}
		strDown.Format("%s\\%s", g_config.szUserHead, iterator->sDownImg);
		strHead.Format("%s\\%s", g_config.szUserHead, iterator->sLocalImg);
		strHead2.Format("%s\\%s", g_config.szUserHead, iterator->sLocalGrayImg);
		// 检测文件是否存在
		if(GetFileAttributes(strHead) == -1)
		{
			iterator->bIsImgDownLoad = DownLoadPic(iterator->sImg, strDown);	// 头像下载成功 
			//如果从url下载头像失败了
			if (!iterator->bIsImgDownLoad)
			{
				iterator->sImg = "http://weixin.upchinafund.com/weixin/images/head2.jpg";
				iterator->sDownImg = "head.jpg";
				iterator->sLocalImg = "head_n.jpg";
				iterator->sLocalGrayImg = "head_g.jpg";
				strDown.Format("%s\\%s", g_config.szUserHead, iterator->sDownImg);
				strHead.Format("%s\\%s", g_config.szUserHead, iterator->sLocalImg);
				strHead2.Format("%s\\%s", g_config.szUserHead, iterator->sLocalGrayImg);

				if(GetFileAttributes(strHead) == -1)
					iterator->bIsImgDownLoad = DownLoadPic(iterator->sImg, strDown);
			}
		}
		else
			iterator->bIsImgDownLoad = TRUE;									// 头像存在 
		if(GetFileAttributes(strHead2) == -1 && iterator->bIsImgDownLoad)
		{
			//将下载的头像文件进行处理
			HandleHeadImage(strDown, iterator->sLocalImg, iterator->sLocalGrayImg);
		//	CreateGrayPngFromColorPng(strHead, strHead2 );						// 图像灰化
		}
	}

	return TRUE;
}

#include "../GenericHTTPClient.h"
// 分析师头像下载，将URL中的图片下载下来，存成头像文件
BOOL ChatDialog::DownLoadPic(CString strUrl, CString strFile)
{
	GenericHTTPClient httpClient;
	if(httpClient.Request(strUrl, GenericHTTPClient::RequestGetMethod, ""))
	{
		LPCTSTR szResult;
		szResult = httpClient.QueryHTTPResponse();    //返回的数据流
		int nSize = httpClient.GetResponseBodySize();
		if (nSize > 1200)
		{
			CFile file;
			if(file.Open((LPCTSTR)strFile, CFile::modeCreate|CFile::modeWrite))
			{
				file.Write((PBYTE)szResult, httpClient.GetResponseBodySize());
				USER_LOG("FILE: [%s]", strFile);
			}
			file.Close();
		}
		else
		{
			USER_LOG("Pic size Error [%s](%d)", strUrl, nSize);
			return FALSE;
		}
	}
	else
	{
		USER_LOG("http req Error [%s]", strUrl);
		return FALSE;
	}

	return TRUE;
}

void ChatDialog::GetAllFXSState()
{
	if (m_vtInfo.size() <= 0)
		return;
	Value data_value;
	Value item;  
	Value array;
	FastWriter writer;
	string strdata;
	int nIndex = 0;
	for (std::vector<AnalystInfo>::iterator iterator = m_vtInfo.begin(); iterator != m_vtInfo.end(); ++iterator)
	{
		item = (int)iterator->AnalystId;
		array.append(item);

		data_value["ct"] = ++nIndex;		// count
		data_value["it"] = array;			// item
	}

	strdata = writer.write(data_value);
	int packlen = strdata.length();

	g_MyClient.SendGetUserState(strdata.c_str(), 999);
}

//#define ANALYS_WEB_ADD "http://121.14.69.14:8094"
#define ANALYS_WEB_ADD "http://upia.upchinafund.com"

// 根据选择的房间号，获取所有的分析师信息
void ChatDialog::GetAllFXSInfo(DWORD rid)
{
	// 通过网页接口，取回json格式的数据，存到map中
	//http://121.14.69.14:8094/api/live/getanalystinfobyroom?roomid=1143
	//http://upia.upchinafund.com/api/live/getanalystinfobyroom?roomid=1143
	CString strURL = "";
	strURL.Format("%s/api/live/getanalystinfobyroom?roomid=%d", ANALYS_WEB_ADD, rid);
	GenericHTTPClient httpClient;
	if(httpClient.Request(strURL))
	{
		LPCTSTR szResult = httpClient.QueryHTTPResponse();    //返回的数据流

		wchar_t*  uStr = httpClient.UTF8ToUnicode( (const char*)szResult, CP_UTF8);
		char* aStr = httpClient.UnicodeToACP( uStr, CP_ACP);

		Parser(aStr, m_vtInfo);
		TDEL(uStr);
		TDEL(aStr);
	}
}

// json串，转换成结构体
BOOL ChatDialog::Parser(CString strJson, vector <AnalystInfo> &m_vtAnalystInfo)
{
	Reader reader;
	Value value;
	string ssValue = strJson;
	// 数组类型结果返回
	if (reader.parse(ssValue, value))
	{
		Value val_array = value["Msg"];  
		int nSize = val_array.size();
		if (nSize < 1)
			return FALSE;

		for (int i=0; i<nSize; i++)
		{
			AnalystInfo ainfo = {0};
			ainfo.AnalystId = val_array[i]["AnalystId"].asInt();
			ainfo.AnalystName = val_array[i]["AnalystName"].asString().c_str();
			ainfo.sDescription = val_array[i]["Description"].asString().c_str();
			ainfo.sImg = val_array[i]["AnalystImg"].asString().c_str();
			ainfo.nControlIndex = -1;

			m_vtAnalystInfo.push_back(ainfo);
		}
	}
	return TRUE;
}

// 定时器刷新图片
void ChatDialog::OnRefreshPic()
{
	m_pRecvEdit->SetReadOnly(false);

	ITextServices * pTextServices = NULL;
	pTextServices = m_pRecvEdit->GetTextServices();
	
	IRichEditOle * pRichEditOle;
	REOBJECT reobject;
	HRESULT hr;
	IMAGE_INFO *pII = NULL;
	BOOL bNeedTimer = FALSE;
	CFileFind ff;
	BOOL bRet = FALSE;

	pRichEditOle = RichEdit_GetOleInterface(pTextServices);
	if (NULL == pRichEditOle)
		return;

	int nCount = pRichEditOle->GetObjectCount();
	for (int i = 0; i < nCount; i++)
	{
		memset(&reobject, 0, sizeof(REOBJECT));
		reobject.cbStruct = sizeof(REOBJECT);
		hr = pRichEditOle->GetObject(i, &reobject, REO_GETOBJ_POLEOBJ);
		if (S_OK == hr)
		{
			if(reobject.dwUser == 0) continue;
			pII = (IMAGE_INFO*)reobject.dwUser;		//dwUser插入的时候存入结构体，对图片的状态进行定时刷新
			if (pII && !(pII->bFinish))
			{
				int cp = reobject.cp;
				m_pRecvEdit->SetSel(cp, cp+1);
				time_t tmNow = time(NULL);
				CString strUserImage = "";
				strUserImage.Format("%s\\%s",g_config.szUserImage, pII->filename);

				//////////////////////////////////////////////////////////////////////////
				//超时
				if (pII->tmR!=0 && tmNow-pII->tmR > IMAGE_TIMEOUT)
					HandleCustomPic_More2(m_pRecvEdit, (LPCTSTR)pII->filename, FALSE, TRUE );
				//图片依旧不存在
				else if (!ff.FindFile( strUserImage ))
					bNeedTimer = TRUE;
				//图片来了，去旧图、插新图
				else 
				{
					bool bNeedScroolToEnd=false;
					HandleCustomPic_More2(m_pRecvEdit, (LPCTSTR)pII->filename);
					//m_pRecvEdit->EndDown();
				}
			}
		}
	}
	if (!bNeedTimer)
	{
		if (m_bRefreshing)
		{
			m_bRefreshing = FALSE;
			KillTimer(T_REFRESH);
		}
	}

	pTextServices->Release();
	m_pRecvEdit->SetReadOnly(true);

	return;
}