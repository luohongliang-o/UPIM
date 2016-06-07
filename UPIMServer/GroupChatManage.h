#pragma once

#pragma pack(push,1)
const char* GetErrorStr(int nError);
typedef map<ROOMID, GROUP_CHAT_ROOM>::iterator GCR_ITER;

class CGroupChatManage  
{
public:
	CGroupChatManage();
	virtual ~CGroupChatManage();
	
public:
	// return:E_GROUPCHAT_ERROR
	LONG Init(CADODatabase* pAdoDb);
	VOID Uninit();
	
	/************************************************************************/
	// 创建新聊天室
	// return:E_GROUPCHAT_ERROR
	// 参数:
	// name:群组名称;type:群类型;masterid:管理员ID;pAdoDb:数据库连接;
	// roomid:output, 群组ID
	/************************************************************************/
	LONG CreateRoom(const char *name, int type, LPCUSERID masterid, CADODatabase* pAdoDb, ROOMID& id);
	
	/************************************************************************/
	// 进入某个聊天室
	// id:聊天室ID;roomtype:聊天室类型;icqid:进入群组用户ID;vtUpid:进入群组用户ID集合 
	/************************************************************************/
	LONG EnterRoom(ROOMID id, int roomtype, vector<VUPID>& vtUpid, CADODatabase* pAdoDb);

	// 某个人离开某个聊天室
	// 当upid=-1或者为masterid的时候,解散该群组
 	LONG LeaveRoom(ROOMID id, int roomtype, LPCUSERID imid, CADODatabase* pAdoDb);

	// 根据群组id得到当前群组成员
	LONG GetUser(ROOMID id, int roomtype, vector<VUPID> & user, CADODatabase* pAdoDb=NULL);
	// nCount[in,out] in(VUPID最大大小),out(实际VUPID个数)
	LONG GetUser(ROOMID id, int roomtype, VUPID* pUser, int& nCount, CADODatabase* pAdoDb=NULL);

	// 得到我的群组
	LONG GetMyChatroom(LPCUSERID imid, vector<GROUP_CHAT_BASE> &gci, CADODatabase* pAdoDb);
	// 添加我的群组
	LONG AddMyChatroom(LPCUSERID imid, ROOMID roomid, CADODatabase* pAdoDb);
	// 删除我的群组
	LONG DelMyChatroom(LPCUSERID imid, ROOMID roomid, CADODatabase* pAdoDb);

	// 根据用户id得到用户所在的固定聊天室
	LONG GetMyChatroom2(LPCUSERID imid, vector<GROUP_CHAT_BASE>& gci, CADODatabase* pAdoDb);

	// 得到群组的一些基本信息
	LONG GetChatroomInfo(GROUP_CHAT_BASE& chatInfo);

	// 修改群公告
	LONG UpdateNote(ROOMID roomid, int roomtype, LPCSTR pszNote, CADODatabase* pAdoDb);

	// 修改群组名称
	LONG UpdateName(ROOMID roomid, int roomtype, LPCSTR pszName, CADODatabase* pAdoDb);

	//////////////////////////////////////////////////////////////////////////
	// 同步更新群组
	// 得到更新的群组信息
	LONG GetChatRoomInfo_U(CADODatabase* pAdoDb, GROUP_CHAT_BASE* pGcb, int& nSize);
	// 更新群组信息
	LONG UpdateChatRoomInfo(GROUP_CHAT_BASE* pGcb, CADODatabase* pAdoDb);

private:
	// 销毁所有聊天室
	VOID DestroyAll();

	LONG _GetUser(ROOMID roomid, CADODatabase* pAdoDb, vector<VUPID> & user);
	LONG _GetChatRoom(ROOMID roomid, int roomtype, GCR_ITER& iter);
	LONG _GetChatRoom2(ROOMID roomid, int roomtype, GCR_ITER& iter);
	
	CCriticalSection m_cs;
	char		m_szTimeTime[32];
	LONG		m_nMaxRoomID;			// 群组ID自增

	map<ROOMID, GROUP_CHAT_ROOM>	m_mapChatroom;
	map<ROOMID, GROUP_CHAT_ROOM>	m_mapTemproom;
};
#pragma pack(pop)