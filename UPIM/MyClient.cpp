#include "StdAfx.h"
#include "MyClient.h"
#include "Func.h"
#include "ProcessAns.h"
#include "MainPort.h"

CMyClient::CMyClient(void):m_Client(this)
{
	m_nUserType = -1;
	m_bMainWndClose = FALSE;
	m_bConnect = FALSE;
	m_bLoginSuccess = FALSE;
}

CMyClient::~CMyClient(void)
{
}

//初始化
void CMyClient::InitClient()
{
	m_MsgQueueSendMng.Init(this);
	return;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//供socket回调的5个接口
EnHandleResult CMyClient::OnConnect(CONNID dwConnID)
{
	TCHAR szAddress[40];
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
	USHORT usPort;

	m_Client.GetLocalAddress(szAddress, iAddressLen, usPort);
	return HR_OK;
}

EnHandleResult CMyClient::OnSend(CONNID dwConnID, const BYTE* pData, int iLength)
{
	return HR_OK;
}

EnHandleResult CMyClient::OnReceive(CONNID dwConnID, const BYTE* pData, int iLength)
{
#ifdef __NEW_MODE

	// wuchao add at 20160421 
	// 解决一个缓冲区  有多个包的情况    粘包解决方案  
	if( g_pack_info.bNeedRecvMore)
	{
		// 需要接收的大小   分包  
		int nNeedRecv = g_pack_info.nNeedRecvLen - g_pack_info.nRecvLen ; 
		iBufferIndex = nNeedRecv ; 

		if( nNeedRecv <= iLength )
		{
			// 解析上面一个包剩下的数据    
			memcpy(g_pack_info.m_btInfo + g_pack_info.nRecvLen, pData, nNeedRecv);
			g_pack_info.nRecvLen = g_pack_info.nRecvLen + nNeedRecv;
			int nProcessSuc = ReciveDataFromSVR(g_pack_info.m_btInfo, g_pack_info.nNeedRecvLen);
			memset(&g_pack_info, 0 , sizeof(PKGRECVINFO));
			USER_LOG("### 解析： %d-%d", nNeedRecv, nNeedRecv);

			// 这里还要处理缓冲区剩下的数据包    
			int nLeftPackLen = iLength - nNeedRecv ; 

			// 解析这个包中剩余的数据 一直要解析玩  
			while( nLeftPackLen > 0 )
			{
				struct tagIMAnsHeader* pHead = (struct tagIMAnsHeader*)(char*)(pData + iBufferIndex)   ;  // 剩下的数据包起始位置  就是一个偏移量
				int CompLen = pHead->PacketLen + sizeof(IMANSHEADER)  ; // 一个完整包的长度  
				// 错误处理 
				if( pHead->Separator != SEPARATOR_NUM) 
				{
					USER_LOG("### Packet Separator ERROR!");
					return HR_OK;
				}
				
				// 解析过程 
				// (1) 剩余包大小比完整包小  
				if( nLeftPackLen < CompLen ) 
				{
					// 这次只能接收部分包 大小为restLen  存储到g_strpkgrecvinfo 
					g_pack_info.bNeedRecvMore = TRUE;
					g_pack_info.nRecvLen = nLeftPackLen;									// 包的实际接收长度
					g_pack_info.nNeedRecvLen = pHead->PacketLen + sizeof(IMANSHEADER);
					memcpy(g_pack_info.m_btInfo, pData + iBufferIndex, nLeftPackLen); 
					iBufferIndex = iBufferIndex + nLeftPackLen ; 
					USER_LOG("小于 iBufferIndex = %d " ,iBufferIndex ) ; 
					nLeftPackLen = 0 ; 
				}
				// (2) 刚好相等   
				else if( nLeftPackLen == CompLen) 
				{
					int nProcessSuc = ReciveDataFromSVR(pData + iBufferIndex ,nLeftPackLen) ; 
					iBufferIndex = iBufferIndex + nLeftPackLen ;
					USER_LOG("相等 iBufferIndex = %d " ,iBufferIndex ) ;
					nLeftPackLen = 0 ;
				}
				// (3) 大于 
				else if( nLeftPackLen > CompLen)
				{
					// 先把完整包数据度出去    
					int nProcessSuc = ReciveDataFromSVR(pData + iBufferIndex ,CompLen) ;
					iBufferIndex = iBufferIndex+ CompLen ; 
					USER_LOG("大于 iBufferIndex = %d " ,iBufferIndex ) ;
					nLeftPackLen =  nLeftPackLen - CompLen ;
				} 
			}
		}
	}
	// 如果需要再次接收
	else if (g_strpkgrecvinfo.bNeedRecvMore)
	{
		int nNeedrecv = g_strpkgrecvinfo.nNeedRecvLen - g_strpkgrecvinfo.nRecvLen;
		// 接收完毕
		if (nNeedrecv == iLength)
		{
			memcpy(g_strpkgrecvinfo.m_btInfo + g_strpkgrecvinfo.nRecvLen, pData, iLength);
			g_strpkgrecvinfo.nRecvLen = g_strpkgrecvinfo.nRecvLen + iLength;
			int nProcessSuc = ReciveDataFromSVR(g_strpkgrecvinfo.m_btInfo, g_strpkgrecvinfo.nNeedRecvLen);
			memset(&g_strpkgrecvinfo, 0 , sizeof(PKGRECVINFO));
			USER_LOG("### bNeedRecvMore # 0, %d-%d", nNeedrecv, iLength);
		}
		// 包没有接收完
		if (nNeedrecv > iLength)
		{
			memcpy(g_strpkgrecvinfo.m_btInfo + g_strpkgrecvinfo.nRecvLen, pData, iLength);
			g_strpkgrecvinfo.nRecvLen = g_strpkgrecvinfo.nRecvLen + iLength;
			USER_LOG("### bNeedRecvMore # 1, %d-%d", nNeedrecv, iLength);
		}
		//我屮艸芔茻，接收到的长度比我需要的大！！！ 碎包后面又粘包？
		if (nNeedrecv < iLength)
		{
			memcpy(g_strpkgrecvinfo.m_btInfo + g_strpkgrecvinfo.nRecvLen, pData, iLength);
			g_strpkgrecvinfo.nRecvLen = g_strpkgrecvinfo.nRecvLen + iLength;
			int nProcessSuc = ReciveDataFromSVR(g_strpkgrecvinfo.m_btInfo, g_strpkgrecvinfo.nNeedRecvLen);
			memset(&g_strpkgrecvinfo, 0 , sizeof(PKGRECVINFO));
			USER_LOG("### 我屮艸芔茻,粗大事了！！！, %d-%d", nNeedrecv, iLength);
		}
	}
	else
	{
		// 组包，有可能是复杂网络环境下，一次OnReceive没有发送完所有的数据，是分次发送的，此时我的pkglen>iLength,需要吧接收到的数据存下来，等数据到齐之后在处理，处理完了，删除缓冲区
		
		struct tagIMAnsHeader* pHead ; 
		/*if( iBufferIndex > 0 )
			pHead = (struct tagIMAnsHeader*)(char*)(pData+iBufferIndex);
		else*/
			pHead = (struct tagIMAnsHeader*)(char*)(pData) ; 

		// 服务器发来的数据包应该是完整的。假如是正常的那么Separator的值应该为7654321
		if (pHead->Separator != SEPARATOR_NUM)
		{
			USER_LOG("### Packet Separator ERROR!");
			return HR_OK;
		}

		int CompLen = pHead->PacketLen + sizeof(IMANSHEADER);
		if (CompLen > iLength)
		{
			if (pHead->PacketLen < ANSBUFFER_LEN)
			{
				g_strpkgrecvinfo.bNeedRecvMore = TRUE;
				g_strpkgrecvinfo.nRecvLen = iLength;									// 包的实际接收长度
				g_strpkgrecvinfo.nNeedRecvLen = pHead->PacketLen + sizeof(IMANSHEADER);
				memcpy(g_strpkgrecvinfo.m_btInfo, pData, iLength);
			}
			else
			{
				USER_LOG("### PacketLen ERROR!");
			}
		}
		// 刚好一个包,正确
		else if (CompLen == iLength)
		{
			int nProcessSuc = ReciveDataFromSVR(pData, iLength);
		}
		// 解包，此处怕粘包现象，根据包长度进行解包
		else
		{
			int nIndex =0;
			int restLen = iLength;
			while (restLen > 0)
			{
				struct tagIMAnsHeader* pHead = (struct tagIMAnsHeader*)(char*)(pData+nIndex);
				if (pHead->PacketLen < ANSBUFFER_LEN && pHead->Separator == SEPARATOR_NUM)
				{
					BYTE* pAllData = new BYTE[64*1024];
					memset(pAllData, 0, 64*1024);
					int thisLength = pHead->PacketLen + sizeof(IMANSHEADER);

					if( restLen < thisLength)
					{
						// 这次只能接收部分包 大小为restLen  存储到g_strpkgrecvinfo 
						g_pack_info.bNeedRecvMore = TRUE;
						g_pack_info.nRecvLen = restLen;									// 包的实际接收长度
						g_pack_info.nNeedRecvLen = pHead->PacketLen + sizeof(IMANSHEADER);
						memcpy(g_pack_info.m_btInfo, pData + nIndex, restLen); 
						restLen = 0 ; // 复位  
					}
					else  // 缓冲区有足够空间储存一个完整的包 
					{
						memcpy(pAllData, pData + nIndex, thisLength);
						int nProcessSuc = ReciveDataFromSVR(pAllData, thisLength);
						nIndex = nIndex + thisLength;
						restLen = restLen - thisLength;
						TDEL(pAllData);
					}
				}
				else
					break;
			}
		}
	}
#else
	// 解包，此处怕粘包现象，根据包长度进行解包
	int nIndex =0;
	int restLen = iLength;
	while (restLen > 0)
	{
		struct tagIMAnsHeader* pHead = (struct tagIMAnsHeader*)(char*)(pData+nIndex);
		if (pHead->PacketLen < ANSBUFFER_LEN  && pHead->Separator == SEPARATOR_NUM)
		{
			BYTE* pAllData = new BYTE[64*1024];
			memset(pAllData, 0, 64*1024);

			int thisLength = pHead->PacketLen + sizeof(IMANSHEADER);
			memcpy(pAllData, pData + nIndex, thisLength);

			int nProcessSuc = ReciveDataFromSVR(pAllData, thisLength);
			nIndex = nIndex + thisLength;
			restLen = restLen - thisLength;
			TDEL(pAllData);
		}
		else
			break;
	}
#endif
	return HR_OK;
}

// 服务器关闭/客户端主动退出
EnHandleResult CMyClient::OnClose(CONNID dwConnID)
{
	USER_LOG("[%s]Socket Close ConnID:%d", m_Client.GetLastErrorDesc(), dwConnID);

	m_bConnect = FALSE;
	// 主窗口主动关闭
	if (g_MyClient.m_bMainWndClose)
	{
		USER_LOG("1# CMyClient::OnClose");
	}
	// 连接被服务器断开
	else
	{
		USER_LOG("2# CMyClient::OnClose");
		//离线状态处理
		if(g_pFrame)
			g_pFrame->PostMessage(UM_USER_OFFLINE, 0, 0);
	}
	
	return HR_OK;
}

// 连接异常关闭
EnHandleResult CMyClient::OnError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	USER_LOG("Socket ERROR ConnID:%d ErrorCode:%d SocketOperation:%d", dwConnID, iErrorCode, enOperation);
	m_bConnect = FALSE;
	g_pFrame->PostMessage(UM_USER_OFFLINE, 0, 0);
	USER_LOG("CMyClient::OnError");
	
	return HR_OK;
}

//////////////////////////////////////////////////////////////////////////
//连接服务器
BOOL CMyClient::StartClient(LPCTSTR pszRemoteAddress, USHORT usPortt, BOOL bAsyncConnect /* = FALSE */)
{
	BOOL bConnect = m_Client.Start(pszRemoteAddress, usPortt, bAsyncConnect);
	if (!bConnect)
	{
		CString strErrorStr = _T("");
		strErrorStr.Format("服务器连接失败:%s", m_Client.GetLastErrorDesc());
		USER_LOG(strErrorStr);
	}
	
	::LogClientStartFail(m_Client.GetLastError(), m_Client.GetLastErrorDesc());
	return bConnect;
}

//手动停止连接
BOOL CMyClient::StopClient()
{
	USER_LOG("StopClient");
	m_bConnect = FALSE;
	m_MsgQueueSendMng.Exit();
	m_CreateGroupParam.ClearMapCreate();

	return m_Client.Stop();
}

//////////////////////////////////////////////////////////////////////////
//利用连接发送数据到服务器
BOOL CMyClient::SendClientMsg(const BYTE* pBuffer, int iLength, int iOffset /* = 0 */)
{
	if (m_Client.GetState() != SS_STARTED)
	{
		if (m_nUserType == eUser_UPClient && !g_bUserKickOff)
		{
			ReConnect();
			USER_LOG("ReConnect Start!");
		}
		// wuchao add at 20160324  IM EXE嵌入到UP中  断线重连 
		else if( m_nUserType == eUser_UPClient2 && !g_bUserKickOff )
		{
			ReConnect();
			USER_LOG("ReConnect Start!");
		}
		// 先直接返回，不自动重连，手动连接
		else
			return FALSE;
	}

	return m_Client.Send(pBuffer, iLength, iOffset);
}

// 设置客户信息
// 后面继续扩充，从配置文件中读取
BOOL CMyClient::SetClientInfo(short nUserType)
{
	m_nUserType = nUserType;
	return TRUE;
}

// UPIM_REQ_CHECKPWD
// 发送用户登录消息
BOOL CMyClient::SendCheckPwdReq(LPCTSTR UID, LPCTSTR PWD, short nCheckType)
{
	BOOL bSendSuc = FALSE;
	
	int nLen = sizeof(login_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	login_req loginreq = {0};
	loginreq.req = UPIM_REQ_CHECKPWD ;
	strncpy(loginreq.userid, UID, sizeof(loginreq.userid));
	strncpy(loginreq.userpwd, PWD, sizeof(loginreq.userpwd));
	loginreq.usertype = nCheckType;

	memcpy(&(m_reqbuf.ReqBuffer), &loginreq, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	//USER_LOG("Send:[UPIM_REQ_CHECKPWD] UID:%s PWD:%s CheckType:%d", UID, PWD, nCheckType);
	return bSendSuc;
}

// UPIM_REQ_GUSER
// 获取好友/用户列表
BOOL CMyClient::SendGUserReq(short nGroupID, short nType)
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(guser_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	guser_req guserreq = {0};
	// UPIM_REQ_GUSER是老模式，结构体冗余较大，不适用
	// UPIM_REQ_GUSER2主要数据是json模式的传输，适合于变长结构
	guserreq.req = UPIM_REQ_GUSER2 ;			
	strcpy(guserreq.userid, m_strUserid);
	guserreq.ngroupid = nGroupID;
	guserreq.ntype = nType;
	memcpy(&(m_reqbuf.ReqBuffer), &guserreq, nLen);

	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);
	USER_LOG("Send:[UPIM_REQ_GUSER] GroupID:%d Type:%d", nGroupID, nType);
	return bSendSuc;
}

// UPIM_REQ_SENDMSG
// 发送单人聊天消息
BOOL CMyClient::SendMsgReq(sendmsg_req *preq)
{
	BOOL bSendSuc = FALSE;

	int nLen = sizeof(sendmsg_req) + preq->msglen/*- sizeof(char)*/;
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	preq->req = UPIM_REQ_SENDMSG ;
	COPY_USERID(preq->cFromuserid, m_strUserid);
	if (preq->dwMsgSeq == 0)
		preq->dwMsgSeq = InterlockedIncrement(&g_nMsgSeq);
	preq->nUsertype = eUser_Client;

	memcpy(&(m_reqbuf.ReqBuffer), preq, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	//USER_LOG("Send:[UPIM_REQ_SENDMSG] DesID:%s nLen:%d msg:%s", preq->cTouserid, nLen, preq->msg);
	return bSendSuc;
}

// UPIM_REQ_GCSENDMSG
// 发送群消息
BOOL CMyClient::SendGroupMsgReq(gcsendmsg_req *preq)
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(gcsendmsg_req) + preq->msglen/*- sizeof(char)*/;
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	preq->req = UPIM_REQ_GCSENDMSG ;
	COPY_USERID(preq->cFromuserid, m_strUserid);
	preq->nUsertype = m_nUserType;
	COPY_NICKNAME(preq->nickname, m_strUserName);

	if (preq->dwMsgSeq == 0)
		preq->dwMsgSeq = InterlockedIncrement(&g_nMsgSeq);
	
	memcpy(&(m_reqbuf.ReqBuffer), preq, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	//USER_LOG("Send:[UPIM_REQ_GCSENDMSG] DesID:%s msg:%s", preq->cFromuserid, preq->msg);
	return bSendSuc;
}

// UPIM_REQ_GLIST
// 取得分组
BOOL CMyClient::SendGListReq(short nGrouptype)
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(glist_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	glist_req glistreq = {0};
	glistreq.req = UPIM_REQ_GLIST ;
	strcpy(glistreq.userid, m_strUserid);
	glistreq.ngrouptype = nGrouptype;

	memcpy(&(m_reqbuf.ReqBuffer), &glistreq, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_GLIST] Grouptype:%d", nGrouptype);
	return bSendSuc;
}

// UPIM_REQ_LIST
// 根据分组取用户列表
BOOL CMyClient::SendFriendListReq(short nGrouptype)
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(list_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	list_req listreq = {0};
	// UPIM_REQ_LIST 是老模式，结构冗余
	// UPIM_REQ_LIST2是JSON模式，核心数据用json传输
	listreq.req = UPIM_REQ_LIST2 ;
	strcpy(listreq.userid, m_strUserid);
	listreq.ngrouptype = nGrouptype;

	memcpy(&(m_reqbuf.ReqBuffer), &listreq, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_LIST] Grouptype:%d", nGrouptype);
	return TRUE;
}

// UPIM_REQ_DEPT
// 部门分组列表
BOOL CMyClient::SendDeptGroupReq()
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(dept_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	dept_req deptreq = {0};
	deptreq.req = UPIM_REQ_DEPT ;
	strcpy(deptreq.userid, m_strUserid);
	deptreq.ndeptid = m_nDeptID;

	memcpy(&(m_reqbuf.ReqBuffer), &deptreq, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_DEPT] deptid:%d", m_nDeptID);
	return TRUE;
}

// UPIM_REQ_GCINFO
// 群组列表
BOOL CMyClient::SendGcInfoReq()
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(dept_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	gcinfo_req gcinforeq = {0};
	//UPIM_REQ_GCINFO为老结构的取群组协议
	//UPIM_REQ_GCINFO2为新的JSON结构的取群组协议
	gcinforeq.req = UPIM_REQ_GCINFO2;
	strcpy(gcinforeq.userid, m_strUserid);
	gcinforeq.type = IsClientLoginIn();

	memcpy(&(m_reqbuf.ReqBuffer), &gcinforeq, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_GCINFO] userid:%s", m_strUserid);
	return TRUE;
}

// UPIM_REQ_GCGETLIST
// 得到群成员列表
BOOL CMyClient::SendGcUserListReq(ROOMID nRoomID, int RoomType, int nStartPos)
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(gcgetlist_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	gcgetlist_req gcgetlistreq = {0};
	gcgetlistreq.req = UPIM_REQ_GCGETLIST;
	gcgetlistreq.Roomtype = RoomType;
	gcgetlistreq.RoomId = nRoomID;
	gcgetlistreq.nStartPos = nStartPos;

	memcpy(&(m_reqbuf.ReqBuffer), &gcgetlistreq, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_GCGETLIST] ROOMID:%d", nRoomID);
	return TRUE;
}

// UPIM_REQ_GCCREATE
// 新创建群组
BOOL CMyClient::SendCreateGroupReq(LPCTSTR strGroupName, short nGrouptype, std::vector<GUPID>* pVtFriend/* =NULL */)
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(gccreate_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	gccreate_req gccreatereq = {0};
	gccreatereq.req = UPIM_REQ_GCCREATE;
	gccreatereq.roomtype = nGrouptype;
	strcpy(gccreatereq.createuserid, m_strUserid);
	strcpy(gccreatereq.roomname, strGroupName);
	int lParam = m_CreateGroupParam.SetMapCreateGroupParam(nGrouptype, strGroupName, m_strUserid, pVtFriend);
	gccreatereq.nroomParam = lParam;

	memcpy(&(m_reqbuf.ReqBuffer), &gccreatereq, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_GCCREATE] ROOMNAME:%s", strGroupName);
	return TRUE;
}

// UPIM_REQ_GCADD
// 向群中添加成员
BOOL CMyClient::SendGcAddReq(ROOMID groupid, short grouptype, LPCTSTR pszupid)
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(gcadd_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	gcadd_req gcaddreq = {0};
	gcaddreq.req = UPIM_REQ_GCADD;
	gcaddreq.nRoomID = groupid;
	gcaddreq.roomtype = grouptype;

	memcpy(&(m_reqbuf.ReqBuffer), &gcaddreq, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_GCADD] ROOMID:%d", groupid);
	return TRUE;
}

// UPIM_REQ_GCADD
// 向群中添加成员
BOOL CMyClient::SendGcAddReq(GROUP_CHAT_ADD_STU m_stu)
{
	BOOL bSendSuc = FALSE;

	char buf[MAX_SEND_BUF_LEN] = {0};
	gcadd_req *gcaddreq=(gcadd_req*)buf;
	gcaddreq->req = UPIM_REQ_GCADD;
	gcaddreq->nRoomID = m_stu.roomid;
	gcaddreq->roomtype = m_stu.roomtype;

	//////////////////////////////////////////////////////////////////////////
	// JSON
	Reader read;  
	Value value;  
	value["count"] = IntToStr(m_stu.count).GetBuffer(0);  
	Value item;  
	int nType = eUser_Broker;
	Value array;  
	for (int i=0; i<m_stu.count; i++)
	{
		nType = m_stu.agUpid[i].m_nUserType;
		item["tp"] = IntToStr(nType).GetBuffer(0);  
		item["id"] = m_stu.agUpid[i].m_szUSERID;  
		array.append( item );   
	}
	value["item"] = array;  
	FastWriter writer;
	string tmp = writer.write(value);
	//////////////////////////////////////////////////////////////////////////
	gcaddreq->szlen = tmp.length();
	strncpy(gcaddreq->szUpID, tmp.c_str(), gcaddreq->szlen);

	int nLen = sizeof(gcadd_req) + gcaddreq->szlen/*- sizeof(char)*/;
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	memcpy(&(m_reqbuf.ReqBuffer), gcaddreq, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_GCADD] ROOMID:%d", m_stu.roomid);
	return TRUE;
}

// UPIM_REQ_GCKICK
// 群组聊天,踢人
BOOL CMyClient::SendGcKickReq(ROOMID groupid, short grouptype, LPCUSERID upid, short nUsertype)
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(gcleave_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	gcleave_req m_req = {0};
	m_req.req = UPIM_REQ_GCKICK;
	m_req.nRoomID = groupid;
	m_req.roomtype = grouptype;
	strcpy(m_req.userid, upid);
	m_req.nUsertype = nUsertype;

	memcpy(&(m_reqbuf.ReqBuffer), &m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_GCKICK] ROOMID:%d", groupid);
	return TRUE;
}

// UPIM_REQ_GCLEAVE
// 群组聊天,离开
BOOL CMyClient::SendGcLeaveReq(ROOMID groupid, short grouptype)
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(gcleave_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	gcleave_req m_req = {0};
	m_req.req = UPIM_REQ_GCLEAVE;
	m_req.nRoomID = groupid;
	m_req.roomtype = grouptype;
	strcpy(m_req.userid, m_strUserid);

	memcpy(&(m_reqbuf.ReqBuffer), &m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_GCKICK] ROOMID:%d", groupid);
	return TRUE;
}

// UPIM_REQ_GPUBLIC
// 获取公众号
BOOL CMyClient::SendGetPublicReq()
{
#ifdef PUBLICMNG
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(public_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	public_req m_req = {0};
	m_req.req = UPIM_REQ_GPUBLIC;
	strcpy(m_req.userid, m_strUserid);
	m_req.nUsertype = m_nUserType;

	memcpy(&(m_reqbuf.ReqBuffer), &m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_GPUBLIC] userid:%s", m_req.userid);
#endif
	return TRUE;
}

// UPIM_REQ_PUBLICINFO
// 获取公众号信息，如果是问题路由，则在主界面显示
BOOL CMyClient::SendGetPublicInfoReq(int nPublicID)
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(publicinfo_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	publicinfo_req m_req = {0};
	m_req.req = UPIM_REQ_PUBLICINFO;
	m_req.publicID = nPublicID;

	memcpy(&(m_reqbuf.ReqBuffer), &m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_PUBLICINFO] PublicId:%d", m_req.publicID);
	return TRUE;
}

// UPIM_REQ_PUBCONCLOSE
// 用户跟公众号的会话断开
BOOL CMyClient::SendPubconCloseReq(LPCUSERID svcid, int publicID, int routeID, int closetype)
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(pubconclose_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	pubconclose_req m_req = {0};
	m_req.req = UPIM_REQ_PUBCONCLOSE;
	m_req.publicID = publicID;
	m_req.routeID = routeID;
	m_req.closeType = closetype;
	COPYSTRARRAY(m_req.SevcId, svcid);
	COPYSTRARRAY(m_req.userid, m_strUserid);

	memcpy(&(m_reqbuf.ReqBuffer), &m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_PUBCONCLOSE] PublicId:%d, svcid:%s", publicID, svcid);
	return bSendSuc;
}

// UPIM_REQ_RECONNECT
// 断线重连之后通知服务器
BOOL CMyClient::SendReconnentReq()
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(reconnect_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	if (g_MyClient.m_strUserid.IsEmpty())
		return FALSE;

	reconnect_req m_req = {0};
	m_req.req = UPIM_REQ_RECONNECT;
	strcpy(m_req.userid, m_strUserid);
	m_req.nUsertype = m_nUserType;

	memcpy(&(m_reqbuf.ReqBuffer), &m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_RECONNECT] userid:%s", m_req.userid);
	return TRUE;
}

// UPIM_REQ_M_USERINFO
// 修改用户信息
BOOL CMyClient::SendModifyUserInfoReq(MODIFY_USERINFO_STU m_stu)
{
	BOOL bSendSuc = FALSE;
	char buf[MAX_SEND_BUF_LEN] = {0};
	m_userinfo_req *m_req=(m_userinfo_req*)buf;
	m_req->req = UPIM_REQ_M_USERINFO;

	//////////////////////////////////////////////////////////////////////////
	// JSON
	Reader read;  
	Value value; 
	value["userid"] = m_stu.userid;
	value["nickname"] = m_stu.nickname;
	value["sex"] = m_stu.sex;
	value["birthday"] = m_stu.birthday;

	FastWriter writer;
	string tmp = writer.write(value);
	//////////////////////////////////////////////////////////////////////////
	m_req->len = tmp.length();
	strncpy(m_req->headp, tmp.c_str(), m_req->len);

	int nLen = sizeof(m_userinfo_req) + m_req->len/*- sizeof(char)*/;
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	memcpy(&(m_reqbuf.ReqBuffer), m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_M_USERINFO]");
	return TRUE;
}

// UPIM_REQ_OLNOTIFY
// 将自己的状态改变推送到在线的好友
BOOL CMyClient::SendStateNotifyReq(string sendid_json, int nstate)
{
	if (sendid_json.length() < 1)
		return FALSE;

	BOOL bSendSuc = FALSE;
	char buf[MAX_SEND_BUF_LEN] = {0};
	olnotify_req *m_req=(olnotify_req*)buf;
	m_req->req = UPIM_REQ_OLNOTIFY;
	strcpy(m_req->userid, m_strUserid);
	m_req->nUserstate = nstate;
	m_req->len = sendid_json.size();
	strncpy(m_req->headp, sendid_json.c_str(), m_req->len);

	int nLen = sizeof(olnotify_req) + m_req->len/*- sizeof(char)*/;
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	memcpy(&(m_reqbuf.ReqBuffer), m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_OLNOTIFY] userid:%s", sendid_json.c_str());
	return TRUE;
}

// UPIM_REQ_OFFMSG
// 用户上线之后从服务器上取得离线消息
BOOL CMyClient::SendOFFMSGReq()
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(offmsg_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	offmsg_req m_req = {0};
	m_req.req = UPIM_REQ_OFFMSG;
	strcpy(m_req.userid, m_strUserid);
	m_req.nUsertype = m_nUserType;

	memcpy(&(m_reqbuf.ReqBuffer), &m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_OFFMSG] userid:%s", m_req.userid);
	return TRUE;
}

// UPIM_REQ_PKG
// 用户向另一用户发送数据包
BOOL CMyClient::SendPkgReq(LPCUSERID tqid, bool request, CClientPackage *pkg)
{
	BOOL bSendSuc = FALSE;
	char buf[MAX_SEND_BUF_LEN] = {0};
	send_pkg_req *m_req = (send_pkg_req*)buf;

	CPackageHandle pkgHandle(pkg);
	DWORD dwLen = 0;						//二进制数据的长度
	char* pBuf = pkg->GetPkBuffer(dwLen);
	if(pBuf==NULL || dwLen<=0) return FALSE;

	m_req->req = UPIM_REQ_PKG;
	COPY_USERID(m_req->cFromuserid, m_strUserid);
	COPY_USERID(m_req->cTouserid, tqid);
	m_req->pkglen = dwLen;
	m_req->nrequest = request;
	memcpy(m_req->pkg, pBuf, dwLen);
	
	int nLen = sizeof(send_pkg_req) + dwLen/*- sizeof(char)*/;
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	memcpy(&(m_reqbuf.ReqBuffer), m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);
	
	return bSendSuc;
}

// UPIM_REQ_ALLTGROOM
// 投顾获取聊天室
BOOL CMyClient::SendGetAllRoomReq()
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(alltgroom_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	alltgroom_req m_req = {0};
	m_req.req = UPIM_REQ_ALLTGROOM;
	strcpy(m_req.userid, m_strUserid);
	m_req.nUsertype = m_nUserType;

	memcpy(&(m_reqbuf.ReqBuffer), &m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_ALLTGROOM] userid:%s", m_req.userid);
	return TRUE;
}

// UPIM_REQ_ALLTGROOMEX
// im2.0版本中客户端能看见所有的房间，所有的分析师
BOOL CMyClient::SendGetAllTgRoomReq(int nMarket)
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(alltgroomEx_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	alltgroomEx_req m_req = {0};
	m_req.req = UPIM_REQ_ALLTGROOMEX;
	m_req.nMarket = nMarket;
	strcpy(m_req.userid, m_strUserid);
	m_req.nUsertype = m_nUserType;

	memcpy(&(m_reqbuf.ReqBuffer), &m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_ALLTGROOMEX] userid:%s market:%d", m_req.userid, nMarket);
	return TRUE;
}

// UPIM_REQ_ALLROOMTOUSER
// im2.0版本中服务人员能看见该房间下所有的客户信息
BOOL CMyClient::SendGetRoomUsersReq(int nRoomID)
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(tggetuserbyroomid_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	tggetuserbyroomid_req m_req = {0};
	m_req.req = UPIM_REQ_ALLROOMTOUSER;
	m_req.roomid = nRoomID;
	strcpy(m_req.userid, m_strUserid);
	m_req.nUsertype = m_nUserType;

	memcpy(&(m_reqbuf.ReqBuffer), &m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_ALLROOMTOUSER] userid:%s roomid:%d", m_req.userid, nRoomID);
	return TRUE;
}

// UPIM_REQ_GETGROUP
// im2.0版本中服务人员自定义分组
BOOL CMyClient::SendGetSelfGroupReq()
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(getgroup_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	getgroup_req m_req = {0};
	m_req.req = UPIM_REQ_GETGROUP;
	strcpy(m_req.userid, m_strUserid);
	m_req.nUsertype = m_nUserType;

	memcpy(&(m_reqbuf.ReqBuffer), &m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_GETGROUP] userid:%s", m_req.userid);
	return TRUE;
}

// UPIM_REQ_GETGUSER
// im2.0版本中服务人员取得自定义分组下的客户列表
BOOL CMyClient::SendGetSelfGroupUsersReq(int nGroupID)
{
	BOOL bSendSuc = FALSE;
	int nLen = sizeof(getguser_req);
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	getguser_req m_req = {0};
	m_req.req = UPIM_REQ_GETGUSER;
	strcpy(m_req.userid, m_strUserid);
	m_req.nUsertype = m_nUserType;
	m_req.nGroupID = nGroupID;

	memcpy(&(m_reqbuf.ReqBuffer), &m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_GETGUSER] userid:%s ngroupid:%d", m_req.userid, nGroupID);
	return TRUE;
}

// UPIM_REQ_ADDGROUP
// im2.0版本中服务人员编辑自定义分组
BOOL CMyClient::SendSetSelfGroupReq(string sjson)
{
	if (sjson.length() < 1)
		return FALSE;

	BOOL bSendSuc = FALSE;
	char buf[MAX_SEND_BUF_LEN] = {0};
	addgroup_req *m_req=(addgroup_req*)buf;
	m_req->req = UPIM_REQ_ADDGROUP;
	strcpy(m_req->userid, m_strUserid);
	m_req->len = sjson.size();
	strncpy(m_req->headp, sjson.c_str(), m_req->len);

	int nLen = sizeof(addgroup_req) + m_req->len/*- sizeof(char)*/;
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	memcpy(&(m_reqbuf.ReqBuffer), m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_ADDGROUP] info:%s", sjson.c_str());
	return TRUE;
}

// UPIM_REQ_MODGUSER
// im2.0版本中服务人员编辑自定义分组的客户列表
BOOL CMyClient::SendSetSelfGroupUserReq(string sjson)
{
	if (sjson.length() < 1)
		return FALSE;

	BOOL bSendSuc = FALSE;
	char buf[MAX_SEND_BUF_LEN] = {0};
	modguser_req *m_req=(modguser_req*)buf;
	m_req->req = UPIM_REQ_MODGUSER;
	strcpy(m_req->userid, m_strUserid);
	m_req->len = sjson.size();
	strncpy(m_req->headp, sjson.c_str(), m_req->len);

	int nLen = sizeof(modguser_req) + m_req->len/*- sizeof(char)*/;
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	memcpy(&(m_reqbuf.ReqBuffer), m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	USER_LOG("Send:[UPIM_REQ_MODGUSER] info:%s", sjson.c_str());
	return TRUE;
}

// UPIM_REQ_ONLINE
// 主动查询好友状态。多个好友用,分隔
BOOL CMyClient::SendGetUserState(LPCUSERID userids, int nFlag /* = 0 */)
{
	BOOL bSendSuc = FALSE;
	char buf[MAX_SEND_BUF_LEN] = {0};
	userstate_req *m_req=(userstate_req*)buf;
	m_req->req = UPIM_REQ_ONLINE;
	strcpy(m_req->userid, m_strUserid);
	m_req->nUsertype = m_nUserType;
	m_req->flag = nFlag;
	strncpy(m_req->headp, userids, strlen(userids));
	m_req->len = strlen(userids);

	int nLen = sizeof(userstate_req) + m_req->len/*- sizeof(char)*/;
	IMREQBUFFER m_reqbuf = {0};
	InitReqBufHead(m_reqbuf, nLen, nLen);

	memcpy(&(m_reqbuf.ReqBuffer), m_req, nLen);
	int nLenAll = sizeof(IMREQHEADER) + nLen;
	bSendSuc = SendClientMsg((LPBYTE)&m_reqbuf, nLenAll);

	//USER_LOG("Send:[UPIM_REQ_ONLINE] userids:%s", userids);

	return TRUE;
}