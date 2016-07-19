#pragma once

// 加载截图dll中的PupupDialog接口
typedef void (*IUPIM_PupupDialog)(HWND m_hwnd);
extern IUPIM_PupupDialog	IUPIM_PupupDialog_;
extern HMODULE	II_mod;

typedef int(*IMInitClient)(const char *appId, const char* appName, 
	const char* deviceId, const wchar_t* localPath, const wchar_t* databasePath);         //初始化
typedef int(*IMSetDeviceInfo)(const char* manufacturer, const char* model, 
	const char* osVersion, const char* network, const char* networkOperator);             //设置设备信息
typedef int(*IMConnect)(const char* token, ConnectAckListenerCallback callback,bool ipv6);  //连接服务器
typedef int(*IMRegisterMessageType)(const char* clazzName, const unsigned int operateBits); //注册消息类型
typedef int(*IMSetMessageListener)(MessageListenerCallback callback);                       //设置消息监听  
typedef int(*IMSetExceptionListener)(ExceptionListenerCallback callback);                   //设置网络异常监听
typedef int(*IMDisconnect)(int);
typedef int(*IMSaveMessage)(const char* targetId, int categoryId, const char* clazzName, 
	const char* senderId, const wchar_t* message, const char* push, const char* appData); //发消息前存储消息
typedef int(*IMsendMessage)(const char* targetId, int categoryId, int transferType, 
	const char* clazzname, const wchar_t* message, const char* push, const char* appData,
	int messageId, PublishAckListenerCallback callback);                                   //发送消息
typedef void (*IMGetUserInfo)(const char* userId, UserInfoListenerCallback callback);      //获取用户信息
typedef int (*IMGetUnreadCount)(const char* targetId, int categoryId);                     //获取未读消息数
typedef bool(*IMClearMessagesUnreadStatus)(const char* targetId, int categoryId);          //清除会话消息的未读状态
typedef void (*IMUploadUserInfo)(const char* name, const char* info, bool flag, TokdenListenerCallback callback);

typedef struct tagIMDLL
{
	HINSTANCE	                   m_hModule;
	IMInitClient                   m_pInitClient;
	IMSetDeviceInfo                m_pSetDeviceInfo;
	IMConnect                      m_pConnect;
	IMRegisterMessageType          m_pRegisterMessageType;
	IMSetMessageListener           m_pSetMessageListener;
	IMSetExceptionListener         m_pSetExceptionListener;
	IMDisconnect                   m_pDisconnect;
	IMSaveMessage                  m_pSaveMessage;
	IMsendMessage                  m_psendMessage;
	IMGetUserInfo                  m_pGetUserInfo;
	IMGetUnreadCount               m_pGetUnreadCount;
	IMClearMessagesUnreadStatus    m_pClearMessagesUnreadStatus;
	IMUploadUserInfo               m_pUploadUserInfo;
}IMRongCloudDLL;

extern IMRongCloudDLL g_RongCloudDll;

extern BOOL LoadDLLFunc();
extern void UnLoadDLLFunc();
extern void sendMessageCallback(const wchar_t* json_str) ;