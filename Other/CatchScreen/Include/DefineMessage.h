#ifndef OWN_DEFINEMESSAGE
#define OWN_DEFINEMESSAGE

//以下为自定义消息
//
//通知显示当前选取的画笔颜色
#define UWM_SHOWPENCOLOR1 (WM_APP+0x900)
#define UWM_SHOWPENCOLOR2 (WM_APP+0x910)

//在主对话框中定义发送消息体函数到画布通知调色板已改变
#define UWM_REALIZEPAL    (WM_APP + 0x920)

//在卡通类CCartoon中当OnMove()或OnSize()后发送消息到画板类和对话框类中通知刷新画布
#define UWM_REDRAWCANVAS  (WM_APP + 0x930)

#define WM_OWNDOWNMESSAGE  (WM_APP + 0x940)

// 浮动窗口移动时发送此消息，以通知画布剪切掉矩形区域
#define WM_CUTCAVASDIBRECT  (WM_APP + 0x950)


//当滑块按纽窗体正移动窗口时OnMoving发送消息通知主对话框更新窗体同时更新滑变量值
#define WM_MOVESIZEBTN  (WM_APP + 0x960)

//在按纽上单击了右键
#define UWM_RBUTTONDOWN  (WM_APP + 0x970)

// 用于画册上显示图形的窗体类向画册窗口类发送消息
#define UWM_GETPREPICTURE  (WM_APP + 0x980)


// 变暗未选择区域
#define WM_DARKUNSELECTEDRECT  (WM_APP + 0x990)

//发送消息到调用的EXE的主窗体，将画板中的图象内容传过去
#define WM_SENDIMAGETOEXE  (WM_APP + 0x993)

#define		UM_FEEDBACKMSG	WM_USER+0x987	// 关闭截图消息

#endif   // OWN_DEFINEMESSAGE