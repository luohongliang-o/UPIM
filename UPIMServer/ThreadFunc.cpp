#include "StdAfx.h"
#include "ThreadFunc.h"
#include "MainFrm.h"

#define	GLOBALCOUNT		4000
#define INTERVAL_CHECKFILE	15
#define USERID_COUNT		500
#define GLOBAL_COUNT	100
#define TQID_PER		50

class CAutoADOCommand
{
public:
	CAutoADOCommand(){ m_pAdoCmd = NULL;}
	CAutoADOCommand(CADODatabase* pAdoDatabase, LPCSTR pszCommandText=_T(""), CADOCommand::cadoCommandType eCommandType = CADOCommand::typeCmdStoredProc) 
	{
		m_pAdoCmd = NULL;
		m_strCommandText = pszCommandText;
		m_eCommandType = eCommandType;
		if(pAdoDatabase)
			m_pAdoCmd = new CADOCommand(pAdoDatabase, pszCommandText, eCommandType);
	}
	virtual ~CAutoADOCommand() { TDEL(m_pAdoCmd);}
	VOID SetConnect(CADODatabase* pAdoDatabase)
	{	
		if(m_pAdoCmd!=NULL) 
			TDEL(m_pAdoCmd);
		if(pAdoDatabase)
			m_pAdoCmd = new CADOCommand(pAdoDatabase, m_strCommandText, m_eCommandType);
	}
	BOOL IsValid() { return m_pAdoCmd!=NULL; }
	operator CADOCommand* () const { return m_pAdoCmd; }
	CADOCommand* operator->() { VERIFY(m_pAdoCmd!=NULL); return m_pAdoCmd; }
protected:
	CADOCommand* m_pAdoCmd;
	CADOCommand::cadoCommandType m_eCommandType;
	CString m_strCommandText;
};

// 用户状态推送
DWORD WINAPI _AfxSendUserState(LPVOID lpParam)
{
	UNREFERENCED_PARAMETER(lpParam);
	CString strValue;
	USERSTATE uUserState = {0};
	SPUPIMID	aUPID[GLOBALCOUNT] = {0};
	long nFriendTotal=0, nFriendNum = 0,nFriendIdx=0;
	STATE_REFLASH_ paToUser={0};

	int nLenAll = 0;
	char pSend[ANSBUFFER_LEN] = {0};
	int nLen = sizeof(online_s_ans);
	IMANSBUFFER m_ansbuf = {0};
	InitAnsBufHead(m_ansbuf, UPIM_REQ_S_ONLINE, nLen, nLen);

	online_s_ans m_ans = {0};
	while (!g_threadInfo.bStartExit)
	{
		if(!g_nInitSuc)
		{
			Sleep(1000);
			continue;
		}
		while(g_pList->PopUSER(uUserState))
		{
			if(g_threadInfo.bStartExit) break;
			// ToUser集合为空
			if (strlen(uUserState.m_szToUser)<=0)
			{
				// 推送给员工
				m_ans.nUserstate = uUserState.m_ucState;
				COPYSTRARRAY(m_ans.userid, uUserState.m_szUSERID);
				memcpy(&(m_ansbuf.AnsBuffer), &m_ans, nLen);
				nLenAll = sizeof(IMANSHEADER) + nLen;
				memcpy(pSend, &m_ansbuf, nLenAll);

				nFriendTotal = 0;
				nFriendNum = GLOBALCOUNT;
				while(GLOBALCOUNT == nFriendNum)
				{
					// 如果是分析师上线的话，那么要获取分析师的客户列表，在线的客户就要推送分析师的上线信息给在线的客户 
					
					nFriendNum = g_custSvrMng.GetEmpInfo(nFriendTotal, aUPID, GLOBALCOUNT);
					nFriendTotal += nFriendNum;
					for(nFriendIdx=0; nFriendIdx<nFriendNum; nFriendIdx++)
					{
						if(g_threadInfo.bStartExit) break;

						MYSTATE m_state = (MYSTATE)0;
						DWORD dwConnectID = 0;
 						if (_stricmp(aUPID[nFriendIdx].m_szUID, uUserState.m_szUSERID) != 0)
						{
							g_MongoDBMgr.GetUserState(aUPID[nFriendIdx].m_szUID, m_state, dwConnectID);

							if (m_state == MYSTATE_NORMAL)
							{
								// 推送在线状态数据包
								g_MySever.m_Server.Send((CONNID)dwConnectID, (LPBYTE)pSend, nLenAll);

								CString strTmp = "";
								strTmp.Format("1#[%s] 在线状态 [%d] 推送状态至 [%s] CID:%d",uUserState.m_szUSERID, uUserState.m_ucState, aUPID[nFriendIdx].m_szUID, dwConnectID);
								//OP_LOG_INFO(strTmp.GetBuffer(0));
							}
						}

						if(0 == nFriendIdx%200)
							Sleep(5);
					}
					if(g_threadInfo.bStartExit) break;
				}
				continue;
			}
			// ToUser集合不为空
			else
			{
				// 解析JSON
				nFriendNum = ParseJsonFields(uUserState.m_szToUser, paToUser, REFRESH_USER_COUNT);
				nFriendNum = min(GLOBALCOUNT, nFriendNum);

				m_ans.nUserstate = uUserState.m_ucState;
				COPYSTRARRAY(m_ans.userid, uUserState.m_szUSERID);
				memcpy(&(m_ansbuf.AnsBuffer), &m_ans, nLen);
				nLenAll = sizeof(IMANSHEADER) + nLen;
				memcpy(pSend, &m_ansbuf, nLenAll);

				for(nFriendIdx=0; nFriendIdx<nFriendNum; nFriendIdx++)
				{
					if(g_threadInfo.bStartExit) break;
					if (strlen(paToUser.mIDVT[nFriendIdx].m_szUID) < 0) continue;
					if(EqualUPID(paToUser.mIDVT[nFriendIdx].m_szUID, uUserState.m_szUSERID)) continue;

					MYSTATE m_state = (MYSTATE)0;
					DWORD dwConnectID = 0;
					g_MongoDBMgr.GetUserState(paToUser.mIDVT[nFriendIdx].m_szUID, m_state, dwConnectID);
					if (m_state == MYSTATE_NORMAL)
					{
						// 推送在线状态数据包
						//g_MySever.m_Server.Send((CONNID)dwConnectID, (LPBYTE)pSend, nLenAll);

						CString strTmp = "";
						strTmp.Format("2#[%s] 在线状态 [%d] 推送状态至 [%s] CID:%d", uUserState.m_szUSERID, uUserState.m_ucState, paToUser.mIDVT[nFriendIdx].m_szUID, dwConnectID);
						//OP_LOG_INFO(strTmp.GetBuffer(0));
					}
					else 
					{
						CString strTmp = "";
						strTmp.Format("3#[%s] 在线状态 [%d] 推送状态至 [%s] CID:%d", uUserState.m_szUSERID, uUserState.m_ucState, paToUser.mIDVT[nFriendIdx].m_szUID, dwConnectID);
						//OP_LOG_INFO(strTmp.GetBuffer(0));

					}
					g_MySever.m_Server.Send((CONNID)dwConnectID, (LPBYTE)pSend, nLenAll);
				}
			}
			Sleep(5);
		}
		Sleep(50);
	}
	return 0;
}

// 离线消息入数据库
DWORD WINAPI _AfxAddOffMsg(LPVOID lpParam)
{
	UNREFERENCED_PARAMETER(lpParam);
	LPOPLINK pLinkInfo = g_LinkMangaer.GetOffMsgLink();
	OFFMSG offMsg = {0};
	ROOMOFFMSG roomOffMsg={0};
	CAutoADOCommand adoCmd(NULL, "icq_add_offmsg");
	
	BOOL bRet = FALSE;
	while (!g_threadInfo.bStartExit)
	{
		if(!g_nInitSuc)
		{
			Sleep(1000);
			continue;
		}

		while(g_pList->PopOffMsg(offMsg))
		{
			if(!pLinkInfo || !pLinkInfo->bConnected)
			{
				pLinkInfo = g_LinkMangaer.GetOffMsgLink();
				if(pLinkInfo)
					adoCmd.SetConnect(pLinkInfo->pAdodb);
			}
			if(!pLinkInfo) 
			{
				if(g_threadInfo.bStartExit) break;
				g_pList->PushOffMsg(&offMsg);
				Sleep(400);
				continue;
			}

			bRet = FALSE;
			if(adoCmd.IsValid())
			{	
				adoCmd->ResetParameter();
				adoCmd->AddInputParameter("@dst_id", strlen(offMsg.m_szDesID), (CString)offMsg.m_szDesID);
				adoCmd->AddInputParameter("@src_id", strlen(offMsg.m_szSrcID), (CString)offMsg.m_szSrcID);
				adoCmd->AddInputParameter("@type", (int)offMsg.m_ucFormat);
				adoCmd->AddInputParameter("@time", (long)offMsg.m_nSendTime);
				adoCmd->AddInputParameter("@msg", strlen(offMsg.m_szMsg), (CString)offMsg.m_szMsg);
				bRet = adoCmd->Execute();
			}
			if(!bRet)
			{
				g_pList->PushOffMsg(&offMsg);
				g_LinkMangaer.CheckConnect(pLinkInfo);
				Sleep(400);
			}
			if(g_threadInfo.bStartExit) break;
		}
		// 群组离线消息入库
		while(g_pList->PopRoomOffMsg(roomOffMsg))
		{
			if(!pLinkInfo || !pLinkInfo->bConnected)
			{
				pLinkInfo = g_LinkMangaer.GetOffMsgLink();
				if(pLinkInfo)
					adoCmd.SetConnect(pLinkInfo->pAdodb);
			}
			if(!pLinkInfo) 
			{
				if(g_threadInfo.bStartExit) break;
				g_pList->PushRoomOffMsg(&roomOffMsg);
				Sleep(400);
				continue;
			}
			bRet = FALSE;
			if(adoCmd.IsValid())
			{
				adoCmd->ResetParameter();
				adoCmd->AddInputParameter("@dst_id", strlen(roomOffMsg.m_szDesID), (CString)roomOffMsg.m_szDesID);
				adoCmd->AddInputParameter("@src_id", strlen(roomOffMsg.m_szSrcID), (CString)roomOffMsg.m_szSrcID);
				adoCmd->AddInputParameter("@type", (int)roomOffMsg.m_ucFormat);
				adoCmd->AddInputParameter("@time", (long)roomOffMsg.m_nSendTime);
				adoCmd->AddInputParameter("@msg", strlen(roomOffMsg.m_szMsg), (CString)roomOffMsg.m_szMsg);
				adoCmd->AddInputParameter("@src_name", strlen(roomOffMsg.m_szSrcName), (CString)roomOffMsg.m_szSrcName);
				adoCmd->AddInputParameter("@roomid", (int)roomOffMsg.m_nRoomid);
				adoCmd->AddInputParameter("@roomtype", (int)roomOffMsg.m_ucRoomType);
				bRet = adoCmd->Execute();
			}
			if(!bRet)
			{
				g_pList->PushRoomOffMsg(&roomOffMsg);
				g_LinkMangaer.CheckConnect(pLinkInfo);
				Sleep(400);
			}
			if(g_threadInfo.bStartExit) break;
		}
		Sleep(400);
	}
	return 0;
}

// 历史消息入数据库
DWORD WINAPI _AfxAddHisMsg(LPVOID lpParam)
{
	UNREFERENCED_PARAMETER(lpParam);
	LPOPLINK pLinkInfo = g_LinkMangaer.GetHisMsgLink();
	HISMSG hisMsg = {0};
	CAutoADOCommand adoCmd(NULL,"icq_add_msg");
	CAutoADOCommand adoCmdG(NULL,"icq_add_groupmsg");
	BOOL bRet = FALSE;
	while (!g_threadInfo.bStartExit)
	{
		if(!g_nInitSuc)
		{
			Sleep(1000);
			continue;
		}
		while(g_pList->PopHisMsg(hisMsg))
		{
			if(!pLinkInfo || !pLinkInfo->bConnected)
			{
				pLinkInfo = g_LinkMangaer.GetHisMsgLink();
				if(pLinkInfo)
				{
					adoCmd.SetConnect(pLinkInfo->pAdodb);
					adoCmdG.SetConnect(pLinkInfo->pAdodb);
				}
			}
			if(!pLinkInfo) 
			{
				if(g_threadInfo.bStartExit) break;
				g_pList->PushHisMsg(&hisMsg);
				Sleep(400);
				continue;
			}
			bRet = FALSE;
			if(hisMsg.m_ucMsgType==0)
			{
				if(adoCmd.IsValid())
				{
					adoCmd->ResetParameter();
					adoCmd->AddInputParameter("@dst_id", strlen(hisMsg.m_szDesID), (CString)hisMsg.m_szDesID);
					adoCmd->AddInputParameter("@src_id", strlen(hisMsg.m_szSrcID), (CString)hisMsg.m_szSrcID);
					adoCmd->AddInputParameter("@type", (int)hisMsg.m_ucFormat);
					adoCmd->AddInputParameter("@date", (long)hisMsg.m_nSendDate);
					adoCmd->AddInputParameter("@time", (long)hisMsg.m_nSendTime);
					// 若是窗口抖动消息，消息体msg可能为空
					adoCmd->AddInputParameter("@message", sizeof(hisMsg.m_szMsg), (CString)hisMsg.m_szMsg);
					bRet = adoCmd->Execute();
				}
			}
			else
			{
				if(adoCmdG.IsValid())
				{
					adoCmdG->ResetParameter();
					adoCmdG->AddInputParameter("@roomid", (long)hisMsg.m_szDesID);
					adoCmdG->AddInputParameter("@roomtype", (int)hisMsg.m_ucMsgType);
					adoCmdG->AddInputParameter("@src_id", strlen(hisMsg.m_szSrcID), (CString)hisMsg.m_szSrcID);
					adoCmdG->AddInputParameter("@date", (long)hisMsg.m_nSendDate);
					adoCmdG->AddInputParameter("@time", (long)hisMsg.m_nSendTime);
					adoCmdG->AddInputParameter("@message", strlen(hisMsg.m_szMsg), (CString)hisMsg.m_szMsg);
					bRet = adoCmdG->Execute();
				}
			}
			if(!bRet)
			{
				g_pList->PushHisMsg(&hisMsg);
				g_LinkMangaer.CheckConnect(pLinkInfo);
				Sleep(400);
			}
			if(g_threadInfo.bStartExit) break;
		}
		Sleep(100);
	}
	return 0;
}

// 外部群组消息以及其他
DWORD WINAPI _AfxSendMsg( LPVOID lpParam )
{
	UNREFERENCED_PARAMETER(lpParam);
	MSGGUSER aMsgGUser[USERID_COUNT] = {0};
	VUPID  aRoomUser[GROUPUSER_COUNT] = {0};
	int nRoomUserCount=0;

	ROOMMSG roomMsg = {0};
	PRDTMSG prdtMsg = {0};
	char szUserId[USERID_SIZE] = {0};

	long nError = 0,nErrorIdx=0;
	char szError[256] = {0};
	BOOL bRet = TRUE;
	CString strValue;

	while (!g_threadInfo.bStartExit)
	{
		if(!g_nInitSuc)
		{
			Sleep(1000);
			continue;
		}
		while(g_pList->PopRoomMsg(roomMsg))
		{
			if(g_threadInfo.bStartExit) break;
			nRoomUserCount = GROUPUSER_COUNT;
			if(eGCError_Success != g_GroupChatMng.GetUser(roomMsg.m_nRoomID, roomMsg.m_ucRoomType, aRoomUser, nRoomUserCount))
			{
				g_pList->PushRoomMsg(&roomMsg);
				break;
			}
			if(nRoomUserCount<=0) continue;

			// 外部群组的消息，通过发送单人消息的通道
			IMANSBUFFER m_ansbuf = {0};
			InitAnsBufHead(m_ansbuf, UPIM_REQ_S_SENDMSG);

			char buf[MAX_SEND_BUF_LEN] = {0};
			s_sendmsg_ans *pans_sendmsg = (s_sendmsg_ans *)buf;
			pans_sendmsg->dwMsgSeq = 0;
			COPYSTRARRAY(pans_sendmsg->cFromuserid, roomMsg.m_szSrcID);
			pans_sendmsg->tmSend = roomMsg.m_tmSend;
			pans_sendmsg->format = roomMsg.m_ucFormat;
			int nMsgLen = strlen(roomMsg.m_szMsg);
			memcpy(pans_sendmsg->msg, roomMsg.m_szMsg, nMsgLen);
			pans_sendmsg->msglen = nMsgLen;

			int nLen = sizeof(s_sendmsg_ans) + pans_sendmsg->msglen/*- sizeof(char)*/;
			m_ansbuf.AnsHeader.PacketLen	= nLen;
			m_ansbuf.AnsHeader.RawLen		= nLen;
			memcpy(&(m_ansbuf.AnsBuffer), pans_sendmsg, nLen);
			int nLenAll = sizeof(IMANSHEADER) + nLen;

			g_pList->PushHisMsg(roomMsg.m_szSrcID, roomMsg.m_nRoomID, roomMsg.m_ucRoomType, roomMsg.m_ucFormat, roomMsg.m_tmSend, roomMsg.m_szMsg);
			for (long nKhhIdx=0; nKhhIdx<nRoomUserCount; nKhhIdx++)
			{
				if(EqualUPID(aRoomUser[nKhhIdx].m_szIMID, roomMsg.m_szSrcID)) continue;

				// 看此用户是否在线，在线的话立即发送，不在线的话转到离线消息库
				MYSTATE m_state = (MYSTATE)0;
				DWORD dwConnectID = 0;
				g_MongoDBMgr.GetUserState(aRoomUser[nKhhIdx].m_szIMID, m_state, dwConnectID);
				if (m_state == MYSTATE_NORMAL)
				{
					BOOL bSendSuess = g_MySever.m_Server.Send((CONNID)dwConnectID, (LPBYTE)&m_ansbuf, nLenAll);
					if (!bSendSuess)
					{
						g_pList->PushOffMsg(roomMsg.m_szSrcID, aRoomUser[nKhhIdx].m_szIMID, roomMsg.m_ucFormat, roomMsg.m_tmSend, roomMsg.m_szMsg);
					}
				}
				else
				{
					g_pList->PushOffMsg(roomMsg.m_szSrcID, aRoomUser[nKhhIdx].m_szIMID, roomMsg.m_ucFormat, roomMsg.m_tmSend, roomMsg.m_szMsg);
				}
			}
			Sleep(5);
		}
		Sleep(300);
	}

	return 0;
}


// 初始化服务线程
DWORD WINAPI _AfxInitSvr(LPVOID lpParam)
{
	HANDLE	EventExit = g_threadInfo.hEventExit;
	LPOPLINK pLinkInfo = NULL;
	char szError[256] = {0};
	while (WaitForSingleObject(EventExit,15000) != WAIT_OBJECT_0)
	{
		pLinkInfo = g_LinkMangaer.GetLink(szError, sizeof(szError), TRUE);
		if(!pLinkInfo)  
		{
			continue;
		}
		// 初始化群组信息模块
		g_GroupChatMng.Init(pLinkInfo->pAdodb);
		// 初始化公众号处理模块
#ifdef PUBLICMNG
		g_PublicMng.Init(pLinkInfo->pAdodb);
#endif
		
		// 初始化客户信息模块
		g_custSvrMng.Init(pLinkInfo->pAdodb);

		g_LinkMangaer.FreeLink(pLinkInfo);
		InterlockedExchange(&g_nInitSuc, 1);
		return 0;
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//RUBBISH

// 得到数据库消息(组)
BOOL GetDBGMsg(CADODatabase* pAdoDb)
{
	if (g_pList->IsDbGMsgListFull()) return TRUE;

	CADOCommand adoCmd(pAdoDb,"icq_get_gmsg");
	adoCmd.AddInputParameter("@seqid", (long)g_config.nSeqGID);

	CADORecordset	adoRs( pAdoDb);
	CString strValue;
	if (!adoRs.Execute(&adoCmd)) 
	{
		strValue = adoRs.GetLastErrorString();
		if(IsDBDisConn(strValue)) 
			return FALSE;
		else return TRUE;
	}
	int nRowCount = adoRs.GetRecordCount();
	if(nRowCount<=0) return FALSE;
	DBGMSG dbmsg = {0};
	COleDateTime timeValue;
	for(int nRowIdx=0;nRowIdx<nRowCount;adoRs.MoveNext(),nRowIdx++)
	{
		adoRs.GetFieldValue("id",strValue);
		dbmsg.m_nID = _atoi64(strValue);

		adoRs.GetFieldValue("grouptype",(int&)dbmsg.m_ucGroupType);
		adoRs.GetFieldValue("groupid",strValue);
		COPYSTRARRAY(dbmsg.m_szGroupID,strValue);
		adoRs.GetFieldValue("attr",(int&)dbmsg.m_ucAttr);
		adoRs.GetFieldValue("endtime",timeValue);
		if(timeValue.GetStatus()==COleDateTime::valid)
		{
			dbmsg.m_nEndDate = timeValue.GetYear()*10000+timeValue.GetMonth()*100+timeValue.GetDay();
			dbmsg.m_nEndTime = timeValue.GetHour()*10000+timeValue.GetMinute()*100+timeValue.GetSecond();
		}
		else
		{
			dbmsg.m_nEndDate = 0;
			dbmsg.m_nEndTime = 0;
		}
		adoRs.GetFieldValue("cnt",strValue);
		COPYSTRARRAY(dbmsg.m_szText,strValue);
		if (g_pList->PushDBGMsg(&dbmsg))
		{
			g_config.nSeqGID = dbmsg.m_nID;
		}
		else break;
	}
	adoRs.Close();
	return TRUE;
}


// 得到数据库消息(个人)
BOOL GetDBPMsg(CADODatabase* pAdoDb)
{
	CADOCommand adoCmd(pAdoDb,"icq_get_pmsg");
	adoCmd.AddInputParameter("@seqid", (long)g_config.nSeqGID);

	CADORecordset	adoRs( pAdoDb);
	CString strValue;
	if (!adoRs.Execute(&adoCmd)) 
	{
		strValue = adoRs.GetLastErrorString();
		if(IsDBDisConn(strValue)) 
			return FALSE;
		return TRUE;
	}
	int nRowCount = adoRs.GetRecordCount();
	if(nRowCount<=0) return TRUE;
	DBPMSG dbmsg = {0};
	COleDateTime timeValue;
	BOOL bRet = FALSE;
	for(int nRowIdx=0;nRowIdx<nRowCount;adoRs.MoveNext(),nRowIdx++)
	{
		adoRs.GetFieldValue("id",strValue);
		dbmsg.m_nID = _atoi64(strValue);
		adoRs.GetFieldValue("userno",strValue);
		COPYSTRARRAY(dbmsg.m_szUSERID,strValue);
		adoRs.GetFieldValue("yyb",strValue);
		COPYSTRARRAY(dbmsg.m_szYyb,strValue);
		adoRs.GetFieldValue("attr",(int&)dbmsg.m_ucAttr);	
		adoRs.GetFieldValue("flag",(int&)dbmsg.m_ucFlag);	
		adoRs.GetFieldValue("src_id",strValue);
		COPYSTRARRAY(dbmsg.m_szSrcID,strValue);
		adoRs.GetFieldValue("cnt",strValue);
		COPYSTRARRAY(dbmsg.m_szText,strValue);
		if(dbmsg.m_ucFlag==1)
			bRet = g_pList->PushPrdtMsg(dbmsg.m_szUSERID,dbmsg.m_szSrcID,eMsgFormat_Txt,time(NULL),strValue);
		else bRet = g_pList->PushDBPMsg(&dbmsg);
		if (bRet)
		{
			g_config.nSeqPID = dbmsg.m_nID;
		}
		else break;
	}
	adoRs.Close();
	return TRUE;
}

// 得到数据库分组消息分组用户
BOOL GetDBGMsgUser(CADODatabase* pAdoDb,LPCTSTR pszGroupId,__int64 nSeqId,LPMSGGUSER pMsgGUser,long& nSize)
{
	if (!pAdoDb || !pMsgGUser || nSize<=0) return TRUE;
	CADOCommand adoCmd(pAdoDb, "icq_get_msg_guser");
	adoCmd.AddInputParameter("@groupid", strlen(pszGroupId), (CString)pszGroupId);
	adoCmd.AddInputParameter("@seqid", (long)nSeqId);
	
	CADORecordset adoRs(pAdoDb);
	if(!adoRs.Execute(&adoCmd)) return FALSE;

	long nRowCount = adoRs.GetRecordCount();
	nRowCount = min(nRowCount,nSize);
	nSize = 0;
	if(nRowCount<=0) return TRUE;
	CString strValue;
	long nRowIdx =0;
	for (;nRowIdx<nRowCount;adoRs.MoveNext(),nRowIdx++)
	{
		adoRs.GetFieldValue(0,(long&)pMsgGUser[nRowIdx].m_nSeqId);
		adoRs.GetFieldValue(1,strValue);
		COPYSTRARRAY(pMsgGUser[nRowIdx].m_szUSERID,strValue);
	}
	nSize = nRowIdx;
	return TRUE;
}


DWORD	WINAPI	_AfxOnTimer(LPVOID lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	HANDLE	EventExit = g_threadInfo.hEventExit;
	DWORD	nTimeInterval = g_config.dwMsgInterval;
	SYSTEMTIME st;
	DWORD nThisDay = 0;
	DWORD nThisTime = 0;
	DWORD nDBMsgLastUpdateTime = 0;
	LPOPLINK pLinkInfo = NULL;
	char szMessage[128] = {0};
	int nCheckFileIdx=0;	
	long nErrorCount = 0;
	while (WaitForSingleObject(EventExit,1000)!=WAIT_OBJECT_0)
	{
		if(!g_nInitSuc)
		{
			Sleep(1000);
			continue;
		}
		GetLocalTime(&st);
		nThisDay = st.wYear*10000+st.wMonth*100+st.wDay;
		nThisTime = st.wHour*3600+st.wMinute*60+st.wSecond;

		// 消息更新
		if ( (g_config.bPushMsg || g_config.bPushPrdtMsg)
			&& nThisTime>=g_config.dwMsgBeginTime 
			&& nThisTime<=g_config.dwMsgEndTime 
			&& (nThisTime-g_config.dwMsgBeginTime)%nTimeInterval==0
			)
		{
			pLinkInfo = g_LinkMangaer.GetLink(szMessage,sizeof(szMessage),TRUE);
			if(pLinkInfo)
			{
				try
				{
					if(!GetDBPMsg(pLinkInfo->pAdodb))
					{
						g_LinkMangaer.DisConnect(pLinkInfo);
						continue;
					}
					if(g_threadInfo.bStartExit)
					{
						g_LinkMangaer.FreeLink(pLinkInfo);
						break;
					}
					if(g_config.bPushMsg)
					{
						if(!GetDBGMsg(pLinkInfo->pAdodb))
						{
							g_LinkMangaer.DisConnect(pLinkInfo);
							continue;
						}
					}
					g_LinkMangaer.FreeLink(pLinkInfo);

				}
				catch (...)
				{
					sprintf(szMessage,"数据库消息同步发生异常");
					g_LinkMangaer.DisConnect(pLinkInfo);
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////

		if(g_threadInfo.bStartExit) break;

/*		// 文件管理
		// 文件超时没有响应,释放相应的缓冲区
		nCheckFileIdx++;
		if(nCheckFileIdx>=INTERVAL_CHECKFILE)
		{
			nCheckFileIdx = 0;
			g_fileMng.CheckFileInfo();
		}
		
		// 员工信息同步
		if( g_svrConfig.dwSynLastDate != nThisDay
			&& nThisTime<=g_svrConfig.dwSynEndTime
			&& nThisTime>=g_svrConfig.dwSynBeginTime)
		{
			pLinkInfo = NULL;
			nErrorCount = 0;
			while(NULL==pLinkInfo && nErrorCount<LINKERROR_NUM)
			{
				pLinkInfo = g_LinkMangaer.GetLink(NULL,0);
				if(!pLinkInfo) Sleep(10);
				nErrorCount++;
			}
			if(!pLinkInfo) continue;
			nErrorCount = g_custSvrMng.LoadEmpInfo(pLinkInfo->pAdodb);
			if(nErrorCount<=0) g_LinkMangaer.CheckConnect(pLinkInfo);
			g_LinkMangaer.FreeLink(pLinkInfo);
		}
		*/
	}
	return 0;
}

DWORD WINAPI _AfxUpdateGroupRoom(LPVOID lParam)
{
	return 0;
}


// 发送
DWORD WINAPI _AfxSendDBMsg( LPVOID lpParam )
{
	return 0;
}

// 写日志
DWORD WINAPI _AfxWriteLog(LPVOID lpParam)
{
	return 0;
}
