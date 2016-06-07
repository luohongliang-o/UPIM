#pragma once
extern BOOL InitReqBufHead(IMREQBUFFER& m_reqbuf, long PacketLen, long RawLen);
extern WORD	GetBuildNo( LPSTR lpszDateTime, const WORD wSize );					// 获取编译序列号
extern bool CreateGUID(char * guid);											// 创建一个全球唯一标识
extern CString MakeGuid(char* strguid = NULL);									// 创建一个全球唯一标识的字符串
extern void ConvertTimeToLong(time_t tmTime, long& nRetDate, long& nRetTime);
extern LPSTR SafeStrcpy( LPSTR lpszDest, LPCSTR lpcszSrc, const WORD wDestSize );
// date 20190204 
// time 11:10:10表示成一个时间戳 
void ConvertTimeToLong2(int date ,int time ,long& nRetTime) ;      // 将日期和时间转化为时间戳 

extern void LoginSuc_Ex();														// 用户登录成功之后操作
extern VOID LoadUserConfig();													// 加载用户配置文件
extern VOID SaveUserConfig();													// 保存用户配置文件
extern VOID LoadConfig();														// 加载系统配置文件
extern VOID InitSQLITE();

extern UINT PlaySoundThread(void* l);											// 播放声音
extern void WriteUserLog(const char *fmt, ...);									// 日志
extern void WriteUserLog_Ex(const char *fmt, ...);								// 日志
extern bool TestFileExist(CString filename);									// 文件或路径是否存在
extern CString IntToStr(int nVal);												// 10位字符以内的int转string

//MD5操作
extern void MD5StringTo32(char *p, char *pRet/*32*/);
extern int InsertVtUser(vector<GUSER_INFO> &mvt, GUSER_INFO m_info);
extern int DeleteVtUser(vector<GUSER_INFO> &mvt, GUSER_INFO m_info);

extern	CString	UTF8Convert(CString &str, int sourceCodepage, int targetCodepage);
extern	bool GetNextMsg(FriendList_ITER iterator, RECEIVED_MSG *msg);
extern	bool GetNextMsg_More(vector<RECEIVED_MSG> *pMsg, RECEIVED_MSG *msg);

extern CString ConvertIdToNoPhonrNum(CString strID);
extern void UpdateVtClientInfo(CString strUPID, int nGroupID);
extern void UpdateGroupFlag(int nGroupID);
extern void SetGroupFlag(int nGroupID ,bool falg) ;
extern BOOL IsAllGroupLoad();
extern int GetGroupClientNums(int nGroupID ); 
extern int GetRoomClientOnlineNums() ;  // 获取分析师下面某个房间客户上线总数 
extern int GetGroupClientOnlineNums(int nGroupID) ;  // 获取分组下面在线人数 

extern int GetGroupIDByName(CString strGroupName);
extern int GetNowTimeHourAndMin();
extern CString ConvertMsgToChiefMsg(BYTE format, LPCSTR lpMsg);
extern CString ConvertMsgContent(LPCSTR lpMsg);
extern BOOL IsInvtRoomClient(CString strUserID);

//编码解码API 将二进制流编码成字符串流 或将字符串流解码为原始的二进制流
//编码或解码后的数据放在pResultBuf,注意nMaxResultBufLen的长度至少应为nRawDataLen的2倍以上
//bEncode为TRUE,表示此函数用于编码,否则用于解码
//如果编码或解码失败,返回-1,成功则返回编码或解码后的长度
extern long Base64_Encode(BYTE *pRawData, long nRawDataLen, BYTE *pResultBuf, long nMaxResultBufLen, BOOL bEncode);

extern CClientPackage * CreatePackage(unsigned short protocol);

// 图片的灰度处理，对于离线的用户，将头像灰化
extern BOOL CreateGrayPngFromColorPng(LPCTSTR lpColorPath, LPCTSTR lpGrayImagePath);
extern int CaculateAshValue(int & nR, int & nG, int & nB);
extern bool GetEncoderClsid(const WCHAR* pszFormat, CLSID* pClsid);
// 尺寸压缩
extern bool CompressImagePixel(const WCHAR* pszOriFilePath, const WCHAR* pszDestFilePah, UINT ulNewHeigth, UINT ulNewWidth, WCHAR* pszFormat = L"image/jpeg");
// 质量压缩
extern bool CompressImageQuality(const WCHAR* pszOriFilePath, const WCHAR* pszDestFilePah, ULONG quality);
// 获取图像信息
extern bool HandleImage(LPCTSTR lpOriFilePath, LPCTSTR lpDestFilePah);
// 半透明画图
extern void SetPictureAlpha(LPCTSTR lpOriFilePath, LPCTSTR FileName1, float f_a);
// 获取图像信息
extern bool HandleHeadImage(LPCTSTR lpOriFilePath, LPCTSTR FileName1, LPCTSTR FileName2 );
// 处理图像信息
extern bool HandleHeadImage2(LPCTSTR lpOriFilePath, LPCTSTR FileName1, LPCTSTR FileName2 );

extern bool HandleHeadImage3(LPCTSTR lpOriFilePath, LPCTSTR FileName1, LPCTSTR FileName2 ) ;

extern BOOL GDIPlus_CombineImage(const WCHAR *format, WCHAR* strDestImage,	WCHAR* strSrcImage1,WCHAR* strSrcImage2);

extern CString HrToMessage( HRESULT hr );
// 检验权限列表看是否符合
extern BOOL IsHasRights(int nRight);

// 获取当前时间   
extern void GetCurrentDate(CString &year ,CString &month ,CString &day) ; 

extern BOOL SortByName(const ROOMANALYS_INFO &data1,const ROOMANALYS_INFO &data2) ; 

extern BOOL SortByNameAndOnline(const ROOMANALYS_INFO &data1,const ROOMANALYS_INFO &data2) ;

// 分析师房间内客户排序 
extern BOOL SortByNameAndOnlineClient(const USER_ROOMCLIENT_INFO &data1,const USER_ROOMCLIENT_INFO &data2) ;

// 分析师中 根据节点的ID查找节点 
extern USER_ROOMCLIENT_INFO* FindClientInRoom(CString strName) ; 

// 客户中  找到房间号为nRoomID的分析师 返回分析师节点的引用  
extern void FindAnalysInRoom(int nRoomID ,ALLCLIENT_ROOM_INFO &retClientRoom) ;

// 当客户登录后，通知分析师已上线，还要更改m_vtRoomClient设置客户上线状态  
extern void SetClientOnLine(CString strClientName ,bool bUserState) ; 

// 在mapRoomAnalys中寻找strAnalysName的分析师房间ID 
extern int  WhitchRoomForAnalys( std::map<TGROOMID ,ALLCLIENT_ROOM_INFO> mapRoomAnalys ,CString strAnalysName) ; 

// wuchao add at 2016-03-29 
// 获取分组里面在线和离线用户信息 
extern void GetClientInfoInGroup(Node*pGroupNode,std::vector<USER_ROOMCLIENT_INFO> &vecOnline ,std::vector<USER_ROOMCLIENT_INFO> &vecOffline ) ; 


extern int astr_ustr( const char *ansistr, WCHAR *unicodestr ) ; 

extern std::string _EncodeURL(  char *str) ; 