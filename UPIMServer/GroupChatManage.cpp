#include "stdafx.h"
#include "MainFrm.h"
#include "GroupChatManage.h"

const char* GetErrorStr(int nError)
{
	const char* error = NULL;
	switch(nError)
	{
	case eGCError_Success:
		error = "成功";
		break;
	case eGCError_DbCon:
		error = "没有可用的数据库连接";
		break;
	case eGCError_DbExe:
		error = "数据库执行失败";
	    break;
	case eGCError_DbOp:
		error = "数据库操作失败";
	    break;
	case eGCError_RoomType:
		error = "无此群组类型";
		break;
	case eGCError_UpUserMax:
		error = "群人数已满";
		break;
	case eGCError_Delete:
		error = "该群已被删除";
		break;
	default:
		error = "未定义的错误类型";
	    break;
	}
	return error;
}

CGroupChatManage::CGroupChatManage()
{
}

CGroupChatManage::~CGroupChatManage()
{
}

LONG CGroupChatManage::Init(CADODatabase* pAdoDb)
{
	if(!pAdoDb) 
		return eGCError_DbCon;
	sprintf(m_szTimeTime,"2011-01-01 00:00:00.000");
	m_nMaxRoomID = 0;

	CString strValue="";
	CADORecordset adoRs(pAdoDb);
	CADOCommand adoCmd(pAdoDb,"icq_get_chatroom");
	
	if(adoRs.Execute(&adoCmd))
	{
		long nRecordCount = adoRs.GetRecordCount();
		nRecordCount = min(nRecordCount,MAX_ROOM_COUNT);
		for (long nIdx=0;nIdx<nRecordCount;nIdx++,adoRs.MoveNext())
		{
			GROUP_CHAT_ROOM chatRoom = {0};
			adoRs.GetFieldValue("roomid", chatRoom.m_nRoomId);
			adoRs.GetFieldValue("roomtype", strValue);
			chatRoom.m_nType = atoi(strValue);
			adoRs.GetFieldValue("masterid", strValue);
			strValue.Trim();
			COPYSTRARRAY(chatRoom.m_szMasterID, strValue);
			adoRs.GetFieldValue("chatroomname", strValue);
			strValue.Trim();
			COPYSTRARRAY(chatRoom.m_szRoomName, strValue);
			adoRs.GetFieldValue("note", strValue);
			strValue.Trim();
			COPYSTRARRAY(chatRoom.m_szNote, strValue);
			adoRs.GetFieldValue("modtime", strValue);
			if(strcmp(m_szTimeTime, strValue)<0) 
				COPYSTRARRAY(m_szTimeTime, strValue);
			if(chatRoom.m_nRoomId > m_nMaxRoomID) 
				m_nMaxRoomID = chatRoom.m_nRoomId;

			InitializeCriticalSection(&chatRoom.m_csRoom);
			chatRoom.m_bDelFlag = false;
			chatRoom.m_vtUser.clear();
			chatRoom.m_bUserFlag = false;
			chatRoom.m_nBusy = 0;
			if(chatRoom.m_nType == eCR_Group)
				m_mapChatroom[chatRoom.m_nRoomId] = chatRoom;
			else if(chatRoom.m_nType == eCR_Group_Client)
				m_mapTemproom[chatRoom.m_nRoomId] = chatRoom;
		}
	}
	else
	{
		return eGCError_DbExe;
	}
	
	return eGCError_Success;
}

VOID CGroupChatManage::Uninit()
{
	DestroyAll();
}

// 创建新聊天室
LONG CGroupChatManage::CreateRoom(const char *name, int type, LPCUSERID masterid, CADODatabase* pAdoDb, ROOMID& id)
{	
	if(!pAdoDb)		
		return eGCError_DbCon;
	if(type != eCR_Group && type != eCR_Group_Client) 
		return eGCError_RoomType;
	
	CString strValue="";
	CADORecordset adoRs(pAdoDb);
	CADOCommand adoCmd(pAdoDb, "icq_add_chatroom");
	adoCmd.AddInputParameter("@name", strlen(name), (CString)name);
	adoCmd.AddInputParameter("@masterid", strlen(masterid), (CString)masterid);
	adoCmd.AddInputParameter("@type", (int)type);
	try
	{
		if(adoRs.Execute(&adoCmd))
		{
			if (adoRs.IsOpen())
			{
				adoRs.MoveFirst();
				DWORD rscount = adoRs.GetRecordCount();
				int ret=0;
				if(!adoRs.IsEOF() && adoRs.GetFieldValue("ret",ret) && ret==0)
				{
					return eGCError_DbOp;
				}
			}
			else 
			{
				return eGCError_RsClose;
			}
			
		}
		else 
			return eGCError_DbExe;
	}
	catch (_com_error err)
	{
		CString errMsg;
		errMsg.Format("操作失败：%s,%s", err.ErrorMessage(), err.Description());
		AfxMessageBox(errMsg);
	}

	if (adoRs.GetFieldCount() == 0)
		return eGCError_RsNULL;

	int ret=0;
	long roomid = -1;

	adoRs.GetFieldValue("ret", ret);
	adoRs.GetFieldValue("roomid", roomid);
	if(roomid <= 0) 
		return eGCError_DbExe;

	id = roomid;

	GROUP_CHAT_ROOM chatRoom;
	chatRoom.m_nRoomId = roomid;
	chatRoom.m_nType = type;
	COPYSTRARRAY(chatRoom.m_szMasterID,masterid);
	COPYSTRARRAY(chatRoom.m_szRoomName,name);
	COPYSTRARRAY(chatRoom.m_szNote,DEFAULT_NOTE);
	chatRoom.m_bDelFlag = false;
	chatRoom.m_bUserFlag = true;
	chatRoom.m_vtUser.clear();
	chatRoom.m_nBusy = 0;

	VUPID tupid = {0};
	CopyUPID(tupid, masterid);
	// 只能是管理员/服务人员才能创建群
	tupid.m_nUserType = 1;
	chatRoom.m_vtUser.push_back(tupid);
	InitializeCriticalSection(&chatRoom.m_csRoom);
	switch(type) 
	{
	case eCR_Group:
		{
			m_cs.Lock();
			if(roomid>m_nMaxRoomID)
			{
				m_mapChatroom[roomid] = chatRoom;
				m_nMaxRoomID = roomid;
			}
			m_cs.Unlock();
		}
		break;
	case eCR_Group_Client:
		{
			m_cs.Lock();
			if(roomid > m_nMaxRoomID)
			{
				m_mapTemproom[roomid] = chatRoom;
				m_nMaxRoomID = roomid;
			}
			m_cs.Unlock();
		}
		break;
	}
	return eGCError_Success;
}

LONG CGroupChatManage::EnterRoom(ROOMID id, int roomtype, vector<VUPID>& vtUpid, CADODatabase* pAdoDb)
{
	if(!pAdoDb)	return eGCError_DbCon;

	LONG nError = eGCError_Success;
	m_cs.Lock();
	GCR_ITER iter;
	nError = _GetChatRoom(id,roomtype,iter);
	if (eGCError_Success != nError)
	{
		m_cs.Unlock();
		return nError;
	}
	EnterCriticalSection(&iter->second.m_csRoom);
	m_cs.Unlock();

	VUPID vupid={0};
	for (UINT nIdx=0;nIdx<vtUpid.size();nIdx++)
	{
		CopyUPID(vupid, vtUpid[nIdx]);
		
		if(iter->second.m_vtUser.size()>=GROUPUSER_COUNT)
		{
			LeaveCriticalSection(&iter->second.m_csRoom);
			return eGCError_UpUserMax;
		}
		UINT j = 0;
		for(j=0; j<iter->second.m_vtUser.size(); j++)
		{
			if(EqualUPID(iter->second.m_vtUser[j], vupid))//用户已经在此聊天室中了,不必再次进入
			{
				break;
			}
		}
		if(j==iter->second.m_vtUser.size())
		{
			iter->second.m_vtUser.push_back(vupid);
			CADORecordset adoRs(pAdoDb);
			CADOCommand adoCmd(pAdoDb,"icq_add_roommember");
			int ret=0;

			adoCmd.AddInputParameter("@id", (int)id);
			adoCmd.AddInputParameter("@imid", strlen(vupid.m_szIMID), (CString)vupid.m_szIMID);
			adoCmd.AddInputParameter("@usertype", (int)vupid.m_nUserType);
			if(adoRs.Execute(&adoCmd))
			{
				if(adoRs.GetFieldValue("ret",ret) && ret==1)
					;
				else nError = eGCError_DbOp;
			}
			else nError = eGCError_DbExe;
		}
	}
	CADOCommand adoCmd(pAdoDb,"icq_mod_roomtime");
	adoCmd.AddInputParameter("@id", (int)id);
	adoCmd.Execute();
	LeaveCriticalSection(&iter->second.m_csRoom);
	return eGCError_Success;
}

// 某个人离开某个聊天室
LONG CGroupChatManage::LeaveRoom(ROOMID id, int roomtype, LPCUSERID imid, CADODatabase* pAdoDb)
{
	if(!pAdoDb) return eGCError_DbCon;
	int nError = eGCError_Success;
	m_cs.Lock();
	BOOL bDelRoom = FALSE;
	GCR_ITER iter;
	nError = _GetChatRoom(id, roomtype, iter);
	if(eGCError_Success != nError) 
	{
		m_cs.Unlock();
		return nError;
	}
	// 解散
	if ( strcmp(imid, "-1")==0 || EqualUPID(imid, iter->second.m_szMasterID) )
	{
		iter->second.m_vtUser.clear();
		iter->second.m_bDelFlag = true;
		bDelRoom = TRUE;
	}
	EnterCriticalSection(&(iter->second.m_csRoom));
	m_cs.Unlock();

	if(bDelRoom)
	{
		try
		{
			CADOCommand adoCmd(pAdoDb, "icq_del_chatroom");
			adoCmd.AddInputParameter("@id", (int)id);
			if(!adoCmd.Execute()) nError = eGCError_DbExe;
		}
		catch (...)
		{
			nError = eGCError_DbExe;
		}
	}
	else
	{
		// 只删除用户
		UINT j=0;
		for(j=0;j<iter->second.m_vtUser.size();j++)
		{
			if(EqualUPID(iter->second.m_vtUser[j], imid))
			{
				iter->second.m_vtUser.erase(iter->second.m_vtUser.begin() + j);

				CADORecordset adoRs(pAdoDb);
				CADOCommand adoCmd(pAdoDb,"icq_del_roommember");
				adoCmd.AddInputParameter("@id", (int)id);
				adoCmd.AddInputParameter("@imid", strlen(imid), (CString)imid);

				int ret = 0;
				if(adoRs.Execute(&adoCmd))
				{
					if(!adoRs.IsEOF() && adoRs.GetFieldValue("ret",ret) && ret!=0)
						;
					else nError = eGCError_DbOp;
				}
				else nError = eGCError_DbExe;
				break;
			}
		}
	}
	LeaveCriticalSection(&(iter->second.m_csRoom));

	if(bDelRoom)
	{
		m_cs.Lock();
		DeleteCriticalSection(&(iter->second.m_csRoom));
		if(roomtype==eCR_Group) m_mapChatroom.erase(iter);
		else if(roomtype==eCR_Group_Client) m_mapTemproom.erase(iter);
		m_cs.Unlock();
	}

	return nError;
}

// 根据id得到当前聊天室成员
LONG CGroupChatManage::GetUser(ROOMID id,int roomtype,vector<VUPID> & user,CADODatabase* pAdoDb)
{
	user.clear();
	m_cs.Lock();
	LONG nError = eGCError_Success;
	GCR_ITER iter;
	nError = _GetChatRoom(id,roomtype,iter);
	if (eGCError_Success != nError)
	{
		m_cs.Unlock();
		return nError;
	}
	if(iter->second.m_bUserFlag)
	{
		// 用户列表已经加载
		user.assign(iter->second.m_vtUser.begin(),iter->second.m_vtUser.end());
		m_cs.Unlock();
		return nError;
	}

	// 用户列表没有加载
	EnterCriticalSection(&(iter->second.m_csRoom));
	m_cs.Unlock();
	if(!pAdoDb)
	{
		LPOPLINK pLinkInfo = g_LinkMangaer.GetLink(NULL,0,TRUE);
		if(!pLinkInfo)
		{
			LeaveCriticalSection(&(iter->second.m_csRoom));
			return eGCError_DbCon;
		}
		nError = _GetUser(id,pLinkInfo->pAdodb,iter->second.m_vtUser);
		if(nError==eGCError_DbExe)
			g_LinkMangaer.CheckConnect(pLinkInfo);
		g_LinkMangaer.FreeLink(pLinkInfo);
	}
	else
	{
		nError = _GetUser(id,pAdoDb,iter->second.m_vtUser);
	}
	
	if (eGCError_Success == nError)
	{
		iter->second.m_bUserFlag = true;
		user.assign(iter->second.m_vtUser.begin(),iter->second.m_vtUser.end());
	}
	LeaveCriticalSection(&(iter->second.m_csRoom));
	return nError;
}

// 根据id得到当前聊天室成员
LONG CGroupChatManage::GetUser(ROOMID id,int roomtype,VUPID* pUser,int& nCount,CADODatabase* pAdoDb)
{
	if(!pUser || nCount<=0) 
		return eGCError_NoDefine;
	m_cs.Lock();
	LONG nError = eGCError_Success;
	GCR_ITER iter;
	nError = _GetChatRoom(id,roomtype,iter);
	if (eGCError_Success != nError)
	{
		m_cs.Unlock();
		return nError;
	}
	if(iter->second.m_bUserFlag)
	{
		// 用户列表已经加载
		int nSize = min(nCount, (int)iter->second.m_vtUser.size());
		int i=0;
		for (i=0; i<nSize; i++)
		{
			CopyUPID(pUser[i], iter->second.m_vtUser[i]);
		}
		nCount = i;
		m_cs.Unlock();
		return nError;
	}

	// 用户列表没有加载
	EnterCriticalSection(&(iter->second.m_csRoom));
	m_cs.Unlock();

	if(!pAdoDb)
	{
		LPOPLINK pLinkInfo = g_LinkMangaer.GetLink(NULL,0,TRUE);
		if(!pLinkInfo)
		{
			LeaveCriticalSection(&(iter->second.m_csRoom));
			return eGCError_DbCon;
		}
		nError = _GetUser(id,pLinkInfo->pAdodb,iter->second.m_vtUser);
		if(nError==eGCError_DbExe)
			g_LinkMangaer.CheckConnect(pLinkInfo);
		g_LinkMangaer.FreeLink(pLinkInfo);
	}
	else
	{
		nError = _GetUser(id,pAdoDb,iter->second.m_vtUser);
	}

	if (eGCError_Success == nError)
	{
		iter->second.m_bUserFlag = true;
		int nSize = min(nCount, (int)iter->second.m_vtUser.size());
		int i=0;
		for (i=0; i<nSize; i++)
		{
			CopyUPID(pUser[i],iter->second.m_vtUser[i]);
		}
		nCount = i;
	}
	LeaveCriticalSection(&(iter->second.m_csRoom));
	return nError;
}

// 得到我的群组
LONG CGroupChatManage::GetMyChatroom(LPCUSERID imid, vector<GROUP_CHAT_BASE> &gci, CADODatabase* pAdoDb)
{
	if(!pAdoDb) return eGCError_DbCon;
	LONG nError = eGCError_Success;
	CADORecordset adoRs(pAdoDb);
	CADOCommand adoCmd(pAdoDb,"icq_get_mychatroom");
	adoCmd.AddInputParameter("@imid", strlen(imid), (CString)imid);

	if(!adoRs.Execute(&adoCmd)) return eGCError_DbExe;
	gci.clear();
	int nRowCount = adoRs.GetRecordCount();
	if (nRowCount<=0) return eGCError_Success;
	
	int roomid = 0;
	GROUP_CHAT_BASE chatInfo = {0};
	for (long nIdx=0; nIdx<nRowCount; nIdx++,adoRs.MoveNext())
	{
		if(adoRs.GetFieldValue("roomid", roomid))
		chatInfo.m_nRoomId = roomid;
		gci.push_back(chatInfo);
	}
	adoRs.Close();
	
	m_cs.Lock();
	GCR_ITER iter;
	for (UINT i=0;i<gci.size();i++)
	{
		nError = _GetChatRoom(gci[i].m_nRoomId,eCR_Group,iter);
		if(eGCError_Success == nError)
		{
			gci[i].m_nType= eCR_Group;
			COPYSTRARRAY(gci[i].m_szMasterID,iter->second.m_szMasterID);
			COPYSTRARRAY(gci[i].m_szRoomName,iter->second.m_szRoomName);
			COPYSTRARRAY(gci[i].m_szNote,iter->second.m_szNote);
		}		
	}
	m_cs.Unlock();
	
	return eGCError_Success;
}

// 添加我的群组
LONG CGroupChatManage::AddMyChatroom(LPCUSERID imid, ROOMID roomid, CADODatabase* pAdoDb)
{
	if(!pAdoDb) return eGCError_DbCon;
	LONG nError = eGCError_Success;
	CADORecordset adoRs(pAdoDb);
	CADOCommand adoCmd(pAdoDb,"icq_add_mychatroom");
	int ret = 0;
	adoCmd.AddInputParameter("@imid", strlen(imid), (CString)imid);
	adoCmd.AddInputParameter("@roomid", (int)roomid);
	if(adoRs.Execute(&adoCmd))
	{
		if(!adoRs.IsEOF() && adoRs.GetFieldValue("ret", ret) && ret==1)
			;
		else nError = eGCError_DbOp;
	}
	else nError = eGCError_DbExe;
	return nError;
}

// 删除我的群组
LONG CGroupChatManage::DelMyChatroom(LPCUSERID imid, ROOMID roomid, CADODatabase* pAdoDb)
{
	if(!pAdoDb) return eGCError_DbCon;
	LONG nError = eGCError_Success;
	CADORecordset adoRs(pAdoDb);
	CADOCommand adoCmd(pAdoDb,"icq_del_mychatroom");
	int ret = 0;
	adoCmd.AddInputParameter("@imid", strlen(imid), (CString)imid);
	adoCmd.AddInputParameter("@roomid", (int)roomid);
	if(adoRs.Execute(&adoCmd))
	{
		if(!adoRs.IsEOF() && adoRs.GetFieldValue("ret", ret) && ret==1)
			;
		else nError = eGCError_DbOp;
	}
	else nError = eGCError_DbExe;
	return nError;
}


// 得到用户所在的固定聊天室
LONG CGroupChatManage::GetMyChatroom2(LPCUSERID imid, vector<GROUP_CHAT_BASE>& gci, CADODatabase* pAdoDb)
{
	if(!pAdoDb) return eGCError_DbCon;
	LONG nError = eGCError_Success;
	CADORecordset adoRs(pAdoDb);
	CADOCommand adoCmd(pAdoDb,"icq_get_mychatroom2");
	adoCmd.AddInputParameter("@imid", strlen(imid), (CString)imid);

	if(!adoRs.Execute(&adoCmd)) return eGCError_DbExe;
	gci.clear();
	int nRowCount = adoRs.GetRecordCount();
	if (nRowCount<=0) return eGCError_Success;
	nRowCount = min(nRowCount,USER_MAX_ROOM_COUNT);
	
	CString strValue = "";
	for (long nIdx=0;nIdx<nRowCount;nIdx++,adoRs.MoveNext())
	{
		GROUP_CHAT_BASE chatInfo = {0};
 		adoRs.GetFieldValue("roomid", chatInfo.m_nRoomId);
 		adoRs.GetFieldValue("roomtype", strValue);
		chatInfo.m_nType = atoi(strValue);

 		adoRs.GetFieldValue("chatroomname", strValue);
		strValue.Trim();
		strncpy(chatInfo.m_szRoomName, strValue, strValue.GetLength());

 		adoRs.GetFieldValue("masterid", strValue);
		strValue.Trim();
		strncpy(chatInfo.m_szMasterID, strValue, strValue.GetLength());

 		adoRs.GetFieldValue("note", strValue);
		strValue.Trim();
		strncpy(chatInfo.m_szNote, strValue, strValue.GetLength());

		gci.push_back(chatInfo);
	}
	adoRs.Close();

	return eGCError_Success;
}

// 销毁所有聊天室
VOID CGroupChatManage::DestroyAll()
{
	m_cs.Lock();
	GCR_ITER iter;
	for(iter = m_mapChatroom.begin();iter!=m_mapChatroom.end();iter++)
	{
		DeleteCriticalSection(&iter->second.m_csRoom);
	}
	m_mapChatroom.clear();

	for(iter=m_mapTemproom.begin();iter!=m_mapTemproom.end();iter++)
	{
		DeleteCriticalSection(&iter->second.m_csRoom);
	}
	m_mapTemproom.clear();

	m_cs.Unlock();
}

LONG CGroupChatManage::GetChatroomInfo(GROUP_CHAT_BASE& chatInfo)
{
	m_cs.Lock();
	LONG nError = eGCError_Success;
	GCR_ITER iter;
	nError = _GetChatRoom(chatInfo.m_nRoomId,chatInfo.m_nType,iter);
	if (eGCError_Success != nError)
	{
		m_cs.Unlock();
		return nError;
	}
	
	COPYSTRARRAY(chatInfo.m_szRoomName,iter->second.m_szRoomName);
	COPYSTRARRAY(chatInfo.m_szNote,iter->second.m_szNote);
	COPYSTRARRAY(chatInfo.m_szMasterID,iter->second.m_szMasterID);
			
	m_cs.Unlock();
	return nError;
}

// 修改群公告
LONG CGroupChatManage::UpdateNote(ROOMID roomid, int roomtype, LPCSTR pszNote, CADODatabase* pAdoDb)
{
	if(roomtype==eCR_Group && !pAdoDb) return eGCError_DbCon;

	LONG nError = eGCError_Success;
	
	CADOCommand adoCmd(pAdoDb,"icq_mod_roomnote");
	adoCmd.AddInputParameter("@roomid", (int)roomid);
	adoCmd.AddInputParameter("@note", strlen(pszNote), (CString)pszNote);
	
	CADORecordset adoRs(pAdoDb);
	if (!adoRs.Execute(&adoCmd))
	{
		return eGCError_DbExe;
	}
	int nRet = 0;
	adoRs.GetFieldValue("ret",nRet);
	if (nRet==0)
	{
		// 错误
		return eGCError_DbOp;
	}

	return nError;
}

// 修改群组名称
LONG CGroupChatManage::UpdateName(ROOMID roomid, int roomtype, LPCSTR pszName, CADODatabase* pAdoDb)
{
	if(roomtype==eCR_Group && !pAdoDb) return eGCError_DbCon;

	LONG nError = eGCError_Success;
	
	CADOCommand adoCmd(pAdoDb,"icq_mod_roomname");
	adoCmd.AddInputParameter("@roomid", (int)roomid);
	adoCmd.AddInputParameter("@name", strlen(pszName), (CString)pszName);
	CADORecordset adoRs(pAdoDb);
	if (!adoRs.Execute(&adoCmd))
	{
		return eGCError_DbExe;
	}
	int nRet = 0;
	adoRs.GetFieldValue("ret",nRet);
	if (nRet==0)
	{
		// 错误
		return eGCError_DbOp;
	}

	return nError;
}

LONG CGroupChatManage::GetChatRoomInfo_U(CADODatabase* pAdoDb, GROUP_CHAT_BASE* pGcb, int& nSize)
{
	if(!pAdoDb) return eGCError_DbCon;
	if(!pGcb || nSize<=0) return eGCError_NoDefine;
	CADORecordset adoRs(pAdoDb);
	CADOCommand adoCmd(pAdoDb,"icq_get_chatroom_u");
	adoCmd.AddInputParameter("@modtime", strlen(m_szTimeTime), (CString)m_szTimeTime);

	if(!adoRs.Execute(&adoCmd)) return eGCError_DbExe;
	long nRowCount = adoRs.GetRecordCount();
	nSize = min(nSize,nRowCount);
	CString strValue;
	for (long nIdx=0;nIdx<nSize;nIdx++,adoRs.MoveNext())
	{
		adoRs.GetFieldValue("roomid",pGcb[nIdx].m_nRoomId);
		adoRs.GetFieldValue("roomtype",pGcb[nIdx].m_nType);
		adoRs.GetFieldValue("masterid",strValue);
		COPYSTRARRAY(pGcb[nIdx].m_szMasterID,strValue);
		adoRs.GetFieldValue("chatroomname",strValue);
		COPYSTRARRAY(pGcb[nIdx].m_szRoomName,strValue);
		adoRs.GetFieldValue("delflag",pGcb[nIdx].m_bDelFlag);
		adoRs.GetFieldValue("note",strValue);
		COPYSTRARRAY(pGcb[nIdx].m_szNote,strValue);
		adoRs.GetFieldValue("modtime",strValue);
		if(strcmp(m_szTimeTime,strValue)<0) COPYSTRARRAY(m_szTimeTime,strValue);
	}
	adoRs.Close();

	return eGCError_Success;
}

LONG CGroupChatManage::UpdateChatRoomInfo(GROUP_CHAT_BASE* pGcb,CADODatabase* pAdoDb)
{
	if(!pGcb) return eGCError_NoDefine;
	m_cs.Lock();
	LONG nError = eGCError_Success;
	GCR_ITER iter;
	nError = _GetChatRoom2(pGcb->m_nRoomId,pGcb->m_nType,iter);
	if (eGCError_Success != nError)
	{
		GROUP_CHAT_ROOM chatRoom;
		chatRoom.m_nRoomId = pGcb->m_nRoomId;
		chatRoom.m_nType = pGcb->m_nType;
		COPYSTRARRAY(chatRoom.m_szMasterID,pGcb->m_szMasterID);
		COPYSTRARRAY(chatRoom.m_szRoomName,pGcb->m_szRoomName);
		COPYSTRARRAY(chatRoom.m_szNote,pGcb->m_szNote);
		chatRoom.m_bDelFlag = pGcb->m_bDelFlag;
		chatRoom.m_bUserFlag = false;
		chatRoom.m_vtUser.clear();
		InitializeCriticalSection(&chatRoom.m_csRoom);

		if(chatRoom.m_nRoomId > m_nMaxRoomID) m_nMaxRoomID = chatRoom.m_nRoomId;

		if(chatRoom.m_nType==eCR_Group)
			m_mapChatroom[chatRoom.m_nRoomId] = chatRoom;
		else if(chatRoom.m_nType==eCR_Group_Client)
			m_mapTemproom[chatRoom.m_nRoomId] = chatRoom;

		m_cs.Unlock();
		return eGCError_Success;
	}
	EnterCriticalSection(&(iter->second.m_csRoom));
	m_cs.Unlock();
	COPYSTRARRAY(iter->second.m_szRoomName,pGcb->m_szRoomName);
	COPYSTRARRAY(iter->second.m_szNote,pGcb->m_szNote);
	iter->second.m_bDelFlag = pGcb->m_bDelFlag;
	if(iter->second.m_bUserFlag)
	{
		_GetUser(iter->second.m_nRoomId,pAdoDb,iter->second.m_vtUser);
	}
	LeaveCriticalSection(&(iter->second.m_csRoom));

	return eGCError_Success;
}

LONG CGroupChatManage::_GetUser(ROOMID roomid,CADODatabase* pAdoDb,vector<VUPID>& user)
{
	if(!pAdoDb) 		return eGCError_DbCon;
	
	CADORecordset adoRs(pAdoDb);
	CADOCommand adoCmd(pAdoDb,"icq_get_chatroommember");
	adoCmd.AddInputParameter("@roomid", (int)roomid);
	user.clear();
	CString strValue;
	VUPID tupid={0};
	if(adoRs.Execute(&adoCmd))
	{
		long nRowCount = adoRs.GetRecordCount();
		nRowCount = min(nRowCount, GROUPUSER_COUNT);
		for (long nIdx=0; nIdx<nRowCount; nIdx++,adoRs.MoveNext())
		{
			if(adoRs.GetFieldValue("userid", strValue))
			{
				strValue.Trim();
				CopyUPID(tupid, strValue);
			}
			else	
				return eGCError_DbOp;

			if(!adoRs.GetFieldValue("memtype", strValue))
				return eGCError_DbOp;
			tupid.m_nUserType = atoi(strValue);
			user.push_back(tupid);
		}
	}
	else return eGCError_DbExe;

	return eGCError_Success;
}

LONG CGroupChatManage::_GetChatRoom(ROOMID roomid,int roomtype,GCR_ITER& iter)
{
	if (roomtype==eCR_Group)
	{
		iter = m_mapChatroom.find(roomid);
		if (iter != m_mapChatroom.end())
		{
			if (iter->second.m_bDelFlag) return eGCError_Delete;
			
			return eGCError_Success; 
		}
	}
	else if (roomtype==eCR_Group_Client)
	{
		iter = m_mapTemproom.find(roomid);
		if (iter != m_mapTemproom.end())
		{
			if (iter->second.m_bDelFlag) return eGCError_Delete;

			return eGCError_Success; 
		}
	}
	else return eGCError_RoomType;

	return eGCError_Delete;
}


LONG CGroupChatManage::_GetChatRoom2(ROOMID roomid,int roomtype,GCR_ITER& iter)
{
	if (roomtype==eCR_Group)
	{
		iter = m_mapChatroom.find(roomid);
		if (iter != m_mapChatroom.end())
		{	
			return eGCError_Success; 
		}
	}
	else if (roomtype==eCR_Group_Client)
	{
		iter = m_mapTemproom.find(roomid);
		if (iter != m_mapTemproom.end())
		{
			return eGCError_Success; 
		}
	}
	else return eGCError_RoomType;

	return eGCError_Delete;
}
