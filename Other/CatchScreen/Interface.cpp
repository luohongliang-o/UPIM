#include "StdAfx.h"
#include "Interface.h"
#include "CatchScreenDlg.h"
extern HWND g_QQWnd;
void PupupDialog(HWND Hwnd)
{
	g_QQWnd = Hwnd;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CCatchScreenDlg dlg;
	dlg.DoModal();
}

