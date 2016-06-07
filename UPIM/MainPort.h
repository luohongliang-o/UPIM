#pragma once

VOID ReConnect();					//重连接
VOID InitConnect();					//初始化连接
VOID StopConnect();					//断开连接
VOID UnInitRichEditCom();			//反初始化RichEdit的com组件
VOID InitRichEditCom();				//初始化RichEdit的com组件
VOID LoginIN();						//登录
VOID InitMainFrame();				//主窗口初始化
VOID OpenChatWnd(CString strUID, int nType);	//打开对话框