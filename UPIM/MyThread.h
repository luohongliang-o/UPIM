#pragma once
#include "afxwin.h"

/*
 * CMyThread类是用来启动线程对话框的类.
 * 些类可以用在DLL中，嵌入UP客户端时使用，使得消息循环不影响不是主线程的消息。
 */
#define WM_MYTHREAD_INITMAIN	WM_USER + 6000
#define WM_MYTHREAD_CREATE		WM_USER + 6001
#define WM_MYTHREAD_DELALL		WM_USER + 6002
#define WM_MYTHREAD_RESIZE		WM_USER + 6003
#define WM_MYTHREAD_CLOSE		WM_USER + 6004

class CMyThread : public CWinThread
{
	DECLARE_DYNCREATE(CMyThread);
public:
	CMyThread(void);
	~CMyThread(void);

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL CreateDlg();
	BOOL Resize();
	BOOL ChangeRoom();
	BOOL Init();
	BOOL InitMain();
};

