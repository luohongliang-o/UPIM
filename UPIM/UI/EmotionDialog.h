#ifndef EMOTIONDIALOG_H
#define EMOTIONDIALOG_H

#include <mmsystem.h>
#include "../stdafx.h"

class MainFrame;
using namespace DuiLib;

class EmotionDialog : public WindowImplBase
{
public:
	EmotionDialog();
	~EmotionDialog();
public:
	virtual LPCTSTR GetWindowClassName() const ;
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };
	virtual void InitWindow();
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam); 
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass); 
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);	
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:	
	void Notify(TNotifyUI& msg); 
	void OnExit(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
}; 

#endif //EMOTIONDIALOG_H