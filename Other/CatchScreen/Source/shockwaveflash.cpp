//////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：shockwaveflash.cpp
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：操作Splash控件，显示Splash动画文件的模块文件(在CSplashWnd类中调用)
*======================================================
* 版本变更记录:
*      v1.0  2002-11-24   9：00  
*
* 
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\INCLUDE\\shockwaveflash.h"

/////////////////////////////////////////////////////////////////////////////
// CShockwaveFlash

IMPLEMENT_DYNCREATE(CShockwaveFlash, CWnd)

/////////////////////////////////////////////////////////////////////////////
/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetReadyState()
*  功能描述     :得到Flash准备状态
*  输入参数描述	:
*  返回变量说明 :状态值
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
long CShockwaveFlash::GetReadyState()
{
	long result;
	InvokeHelper(DISPID_READYSTATE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetTotalFrames()
*  功能描述     :得到要播放的
*  输入参数描述	:
*  返回变量说明 :Flash的总帧数
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
long CShockwaveFlash::GetTotalFrames()
{
	long result;
	InvokeHelper(0x7c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetPlaying()
*  功能描述     :得到是否正在播放Flash的状态
*  输入参数描述	:
*  返回变量说明 :返回TRUE表示当前正在播放Flash
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL CShockwaveFlash::GetPlaying()
{
	BOOL result;
	InvokeHelper(0x7d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetPlaying
*  功能描述     :设置是否要播放Flash
*  输入参数描述	:
*    BOOL bNewValue:设为TRUE表示正在播放Flash，设为FALSE表示不播放Flash
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetPlaying(BOOL bNewValue)
{
	static BYTE parms[] = VTS_BOOL;
	InvokeHelper(0x7d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetQuality()
*  功能描述     :得到播放Flash的播放质量值
*  输入参数描述	:
*  返回变量说明 :Flash的播放质量值
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
long CShockwaveFlash::GetQuality()
{
	long result;
	InvokeHelper(0x69, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetQuality
*  功能描述     :设置播放Flash的播放质量值
*  输入参数描述	:
*  long nNewValue： Flash的播放质量值
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetQuality(long nNewValue)
{
	static BYTE parms[] = VTS_I4;
	InvokeHelper(0x69, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetScaleMode()
*  功能描述     :得到显示Flash动画的比例模式
*  输入参数描述	:
*  返回变量说明 :Flash动画的比例模式值
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
long CShockwaveFlash::GetScaleMode()
{
	long result;
	InvokeHelper(0x78, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetScaleMode
*  功能描述     :设置显示Flash动画的比例模式
*  输入参数描述	:
*   long nNewValue： Flash动画的比例模式值
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetScaleMode(long nNewValue)
{
	static BYTE parms[] = VTS_I4;
	InvokeHelper(0x78, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetAlignMode()
*  功能描述     :得到显示Flash动画的对齐模式
*  输入参数描述	:
*  返回变量说明 :显示Flash动画的对齐模式值
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
long CShockwaveFlash::GetAlignMode()
{
	long result;
	InvokeHelper(0x79, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetAlignMode
*  功能描述     :设置显示Flash动画的对齐模式
*  输入参数描述	:
*   long nNewValue：显示Flash动画的对齐模式值
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetAlignMode(long nNewValue)
{
	static BYTE parms[] = VTS_I4;
	InvokeHelper(0x79, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetBackgroundColor()
*  功能描述     :得到播放Flash动画时的背景色
*  输入参数描述	:
*  返回变量说明 :播放Flash动画时的背景色值
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
long CShockwaveFlash::GetBackgroundColor()
{
	long result;
	InvokeHelper(0x7b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetBackgroundColor
*  功能描述     :设置播放Flash动画时的背景色
*  输入参数描述	:
*    long nNewValue:播放Flash动画时的背景色值
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetBackgroundColor(long nNewValue)
{
	static BYTE parms[] = VTS_I4;
	InvokeHelper(0x7b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetLoop()
*  功能描述     :得到当前播放的Flash动画是否循环播放
*  输入参数描述	:
*  返回变量说明 :返回TRUE表示当前循环播放，返回FALSE表示不循环播放
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL CShockwaveFlash::GetLoop()
{
	BOOL result;
	InvokeHelper(0x6a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetLoop
*  功能描述     :设置当前播放的Flash动画是否循环播放
*  输入参数描述	:
*     BOOL bNewValue:设为TRUE表示当前循环播放，设为FALSE表示不循环播放
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetLoop(BOOL bNewValue)
{
	static BYTE parms[] = VTS_BOOL;
	InvokeHelper(0x6a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetMovie()
*  功能描述     :得到播放的Flash的文件名称
*  输入参数描述	:
*  返回变量说明 :当前播放的Flash的文件名称
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CString CShockwaveFlash::GetMovie()
{
	CString result;
	InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetMovie
*  功能描述     :设置播放的Flash的文件名称
*  输入参数描述	:
*   LPCTSTR lpszNewValue:当前要播放的Flash的文件名称
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetMovie(LPCTSTR lpszNewValue)
{
	static BYTE parms[] = VTS_BSTR;
	InvokeHelper(0x66, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetFrameNum()
*  功能描述     :得到当前播放的Flash的帖序号数
*  输入参数描述	:
*  返回变量说明 :当前播放的Flash的帖序号数
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
long CShockwaveFlash::GetFrameNum()
{
	long result;
	InvokeHelper(0x6b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetFrameNum
*  功能描述     :设置当前播放的Flash的帖序号数
*  输入参数描述	:
*   要设置当前播放的Flash的帖序号数
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetFrameNum(long nNewValue)
{
	static BYTE parms[] = VTS_I4;
	InvokeHelper(0x6b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetZoomRect
*  功能描述     :设置当前播放的Flash动画的缩放区域
*  输入参数描述	:
*	long left：矩形区左
*	long top：矩形区顶
*	long right：矩形区右
*	long bottom：矩形区底
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetZoomRect(long left, long top, long right, long bottom)
{
	static BYTE parms[] =		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x6d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 left, top, right, bottom);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :Zoom
*  功能描述     :执行缩放
*  输入参数描述	:
*   long factor ：缩放比例值
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::Zoom(long factor)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x76, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 factor);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :Pan
*  功能描述     :移动帖
*  输入参数描述	:
*	long x ：移动的X坐标
*	long y：移动的Y坐标
*	long mode：移动模式
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::Pan(long x, long y, long mode)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x77, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y, mode);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :Play()
*  功能描述     :播放Flash动画
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::Play()
{
	InvokeHelper(0x70, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :Stop()
*  功能描述     :暂停播放Flash动画
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::Stop()
{
	InvokeHelper(0x71, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :Back()
*  功能描述     :往后播放Flash动画
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::Back()
{
	InvokeHelper(0x72, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :Forward()
*  功能描述     :向前播放Flash动画
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::Forward()
{
	InvokeHelper(0x73, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :Rewind()
*  功能描述     :重新从开头播放Flash动画
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::Rewind()
{
	InvokeHelper(0x74, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :StopPlay()
*  功能描述     :停止播放Flash动画
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::StopPlay()
{
	InvokeHelper(0x7e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GotoFrame
*  功能描述     :到指定帖播放Flash动画
*  输入参数描述	:
*   long FrameNum：指定的帖序号数
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::GotoFrame(long FrameNum)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FrameNum);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :CurrentFrame()
*  功能描述     :得到当前正播放的Flash动画的帖序号数
*  输入参数描述	:
*  返回变量说明 :前正播放的Flash动画的帖序号数
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
long CShockwaveFlash::CurrentFrame()
{
	long result;
	InvokeHelper(0x80, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :IsPlaying()
*  功能描述     :判断是否正在播放Flash动画
*  输入参数描述	:
*  返回变量说明 :返回TRUE表示正在播放Flash动画，否则表示当前没有播放
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL CShockwaveFlash::IsPlaying()
{
	BOOL result;
	InvokeHelper(0x81, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :PercentLoaded()
*  功能描述     :得到当前播放Flash动画的进度的百分比
*  输入参数描述	:
*  返回变量说明 :当前播放Flash动画的进度的百分比
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
long CShockwaveFlash::PercentLoaded()
{
	long result;
	InvokeHelper(0x82, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :FrameLoaded
*  功能描述     :设置要被导入的Flash动画的帖数
*  输入参数描述	:
*     long FrameNum：要被导入的Flash动画的帖数
*  返回变量说明 :
*   返回TRUE表示设置成功，返回FALSE表示失败
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL CShockwaveFlash::FrameLoaded(long FrameNum)
{
	BOOL result;
	static BYTE parms[] = VTS_I4;
	InvokeHelper(0x83, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		FrameNum);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :FlashVersion()
*  功能描述     :得到当前使用的Flash播放控件的版本号
*  输入参数描述	:
*  返回变量说明 :Flash播放控件的版本号
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
long CShockwaveFlash::FlashVersion()
{
	long result;
	InvokeHelper(0x84, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetWMode()
*  功能描述     :得到当前选用的播放模式
*  输入参数描述	:
*  返回变量说明 :
*     播放模式类型
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CString CShockwaveFlash::GetWMode()
{
	CString result;
	InvokeHelper(0x85, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetWMode
*  功能描述     :设置当前选用的播放模式
*  输入参数描述	:
*   LPCTSTR lpszNewValue： 播放模式类型
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetWMode(LPCTSTR lpszNewValue)
{
	static BYTE parms[] = VTS_BSTR;
	InvokeHelper(0x85, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetSAlign()
*  功能描述     :得到播放Flash动画的对齐模式
*  输入参数描述	:
*  返回变量说明 :Flash动画的对齐模式类型
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CString CShockwaveFlash::GetSAlign()
{
	CString result;
	InvokeHelper(0x86, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetSAlign
*  功能描述     :设置播放Flash动画的对齐模式
*  输入参数描述	:
*     LPCTSTR lpszNewValue：Flash动画的对齐模式类型
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetSAlign(LPCTSTR lpszNewValue)
{
	static BYTE parms[] = VTS_BSTR;
	InvokeHelper(0x86, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetMenu()
*  功能描述     :得到控件中的菜单是否可用
*  输入参数描述	:
*  返回变量说明 :返回TRUE表示可用，返回FALS表示不可用
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL CShockwaveFlash::GetMenu()
{
	BOOL result;
	InvokeHelper(0x87, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetMenu
*  功能描述     :设置控件中的菜单是否可用
*  输入参数描述	:
*   BOOL bNewValue：设为TRUE表示可用，设为FALS表示不可用
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetMenu(BOOL bNewValue)
{
	static BYTE parms[] = VTS_BOOL;
	InvokeHelper(0x87, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetBase()
*  功能描述     :得到关于Flash播放控件的基本信息
*  输入参数描述	:
*  返回变量说明 :关于Flash播放控件的基本信息
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CString CShockwaveFlash::GetBase()
{
	CString result;
	InvokeHelper(0x88, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetBase
*  功能描述     :设置关于Flash播放控件的基本信息
*  输入参数描述	:
*    LPCTSTR lpszNewValue: 关于Flash播放控件的基本信息
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetBase(LPCTSTR lpszNewValue)
{
	static BYTE parms[] = VTS_BSTR;
	InvokeHelper(0x88, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetScale()
*  功能描述     :得到播放比例
*  输入参数描述	:
*  返回变量说明 :播放比例值
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CString CShockwaveFlash::GetScale()
{
	CString result;
	InvokeHelper(0x89, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetScale
*  功能描述     :设置播放比例
*  输入参数描述	:
*   LPCTSTR lpszNewValue:播放比例值
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetScale(LPCTSTR lpszNewValue)
{
	static BYTE parms[] = VTS_BSTR;
	InvokeHelper(0x89, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetDeviceFont()
*  功能描述     :得到设备是否支持当前所采用的字体
*  输入参数描述	:
*  返回变量说明 :返回TRUE表示支持，返回FALSE表示不支持
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL CShockwaveFlash::GetDeviceFont()
{
	BOOL result;
	InvokeHelper(0x8a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetDeviceFont
*  功能描述     :设置设备是否支持当前所采用的字体
*  输入参数描述	:
*    BOOL bNewValue：设为TRUE表示支持，设为FALSE表示不支持
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetDeviceFont(BOOL bNewValue)
{
	static BYTE parms[] = VTS_BOOL;
	InvokeHelper(0x8a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetEmbedMovie()
*  功能描述     :得到是否嵌入的Flash动画影片
*  输入参数描述	:
*  返回变量说明 :返回TRUE表示嵌入了Flash动画影片，返回FALSE表示没有嵌入Flash动画影片
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL CShockwaveFlash::GetEmbedMovie()
{
	BOOL result;
	InvokeHelper(0x8b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetEmbedMovie
*  功能描述     :设置是否嵌入的Flash动画影片
*  输入参数描述	:
*     BOOL bNewValue：设为TRUE表示嵌入了Flash动画影片，设为FALSE表示不嵌入Flash动画影片
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetEmbedMovie(BOOL bNewValue)
{
	static BYTE parms[] = VTS_BOOL;
	InvokeHelper(0x8b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetBGColor()
*  功能描述     :得到Flash动画中的网格线采用的颜色
*  输入参数描述	:
*  返回变量说明 :Flash动画中的网格线采用的颜色值
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CString CShockwaveFlash::GetBGColor()
{
	CString result;
	InvokeHelper(0x8c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetBGColor
*  功能描述     :设置Flash动画中的网格线采用的颜色
*  输入参数描述	:
*   LPCTSTR lpszNewValue：Flash动画中的网格线采用的颜色值
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetBGColor(LPCTSTR lpszNewValue)
{
	static BYTE parms[] = VTS_BSTR;
	InvokeHelper(0x8c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetQuality2()
*  功能描述     :得到要播放的Flash动画的品质
*  输入参数描述	:
*  返回变量说明 :播放的Flash动画的品质
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CString CShockwaveFlash::GetQuality2()
{
	CString result;
	InvokeHelper(0x8d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetQuality2
*  功能描述     :设置要播放的Flash动画的品质
*  输入参数描述	:
*    LPCTSTR lpszNewValue：播放的Flash动画的品质
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetQuality2(LPCTSTR lpszNewValue)
{
	static BYTE parms[] = VTS_BSTR;
	InvokeHelper(0x8d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :LoadMovie
*  功能描述     :导入指定位置指定图层的Flash动画影片
*  输入参数描述	:
*    long layer:图层数
*    LPCTSTR url：动画的URL位置
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::LoadMovie(long layer, LPCTSTR url)
{
	static BYTE parms[] = VTS_I4 VTS_BSTR;
 	InvokeHelper(0x8e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 layer, url);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :TGotoFrame
*  功能描述     :转到指定目标位置的帧数位置
*  输入参数描述	:
*	LPCTSTR target:目标位置
*	long FrameNum：帖序号数
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::TGotoFrame(LPCTSTR target, long FrameNum)
{
	static BYTE parms[] = VTS_BSTR VTS_I4;
	InvokeHelper(0x8f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target, FrameNum);
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :TGotoLabel
*  功能描述     :转到指定目标位置的标签位置
*  输入参数描述	:
*	LPCTSTR target:目标位置
*	LPCTSTR label：标签值
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::TGotoLabel(LPCTSTR target, LPCTSTR label)
{
	static BYTE parms[] = VTS_BSTR VTS_BSTR;
	InvokeHelper(0x90, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target, label);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :TCurrentFrame
*  功能描述     :转到指定目标位置当前帧
*  输入参数描述	:
*     LPCTSTR target：目标位置
*  返回变量说明 :当前的帧序号
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
long CShockwaveFlash::TCurrentFrame(LPCTSTR target)
{
	long result;
	static BYTE parms[] = VTS_BSTR;
	InvokeHelper(0x91, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		target);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :TCurrentLabel
*  功能描述     :得到当前指定目标位置的标签
*  输入参数描述	:
*     LPCTSTR target：目标位置
*  返回变量说明 :当前标签值
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CString CShockwaveFlash::TCurrentLabel(LPCTSTR target)
{
	CString result;
	static BYTE parms[] = VTS_BSTR;
	InvokeHelper(0x92, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		target);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :TPlay
*  功能描述     :在指定目标位置播放Flash动画
*  输入参数描述	:
*   LPCTSTR target：目标位置
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::TPlay(LPCTSTR target)
{
	static BYTE parms[] = VTS_BSTR;
	InvokeHelper(0x93, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :TStopPlay
*  功能描述     :暂停在指定目标位置上播放Flash
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::TStopPlay(LPCTSTR target)
{
	static BYTE parms[] = VTS_BSTR;
	InvokeHelper(0x94, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetVariable
*  功能描述     :设置Flash控件支持的变量值
*  输入参数描述	:
*    LPCTSTR name：变量名称
*    LPCTSTR value：变量值
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetVariable(LPCTSTR name, LPCTSTR value)
{
	static BYTE parms[] = VTS_BSTR VTS_BSTR;
	InvokeHelper(0x97, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 name, value);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetVariable
*  功能描述     :得到Flash控件支持的变量值
*  输入参数描述	:
*    LPCTSTR name：变量名称
*  返回变量说明 :变量值
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CString CShockwaveFlash::GetVariable(LPCTSTR name)
{
	CString result;
	static BYTE parms[] = VTS_BSTR;
	InvokeHelper(0x98, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		name);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :TSetProperty
*  功能描述     :设置Flash控件支持的属性值
*  输入参数描述	:
*    LPCTSTR target:目标位置
*    long property：属性
*    LPCTSTR value：属性值
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::TSetProperty(LPCTSTR target, long property, LPCTSTR value)
{
	static BYTE parms[] = VTS_BSTR VTS_I4 VTS_BSTR;
	InvokeHelper(0x99, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target, property, value);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :TGetProperty
*  功能描述     :得到Flash控件支持的属性值
*  输入参数描述	:
*    LPCTSTR target:目标位置
*    long property：属性
*  返回变量说明 :属性值
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CString CShockwaveFlash::TGetProperty(LPCTSTR target, long property)
{
	CString result;
	static BYTE parms[] = VTS_BSTR VTS_I4;
	InvokeHelper(0x9a, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		target, property);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :TCallFrame
*  功能描述     :启用指定目标位置的指定帧
*  输入参数描述	:
*	LPCTSTR target：目标位置
*	long FrameNum：帧序号
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::TCallFrame(LPCTSTR target, long FrameNum)
{
	static BYTE parms[] = VTS_BSTR VTS_I4;
	InvokeHelper(0x9b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target, FrameNum);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :TCallLabel
*  功能描述     :启用指定目标位置的指定标签的帧
*  输入参数描述	:
*	LPCTSTR target：目标位置
*	long FrameNum：帧标签号
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::TCallLabel(LPCTSTR target, LPCTSTR label)
{
	static BYTE parms[] = VTS_BSTR VTS_BSTR;
	InvokeHelper(0x9c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target, label);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :TSetPropertyNum
*  功能描述     :设置目标位置的属性值
*  输入参数描述	:
*	LPCTSTR target：目标位置
*	long property：属性
*	double value：属性值
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::TSetPropertyNum(LPCTSTR target, long property, double value)
{
	static BYTE parms[] = VTS_BSTR VTS_I4 VTS_R8;
	InvokeHelper(0x9d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 target, property, value);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :TGetPropertyNum
*  功能描述     :得到目标位置的属性值
*  输入参数描述	:
*	LPCTSTR target：目标位置
*	long property：属性
*  返回变量说明 :属性值
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
double CShockwaveFlash::TGetPropertyNum(LPCTSTR target, long property)
{
	double result;
	static BYTE parms[] = VTS_BSTR VTS_I4;
	InvokeHelper(0x9e, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		target, property);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetSWRemote()
*  功能描述     :得到Flash中的其它细节参数
*  输入参数描述	:
*  返回变量说明 :细节参数
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CString CShockwaveFlash::GetSWRemote()
{
	CString result;
	InvokeHelper(0x9f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetSWRemote
*  功能描述     :设置Flash中的其它细节参数
*  输入参数描述	:
*    LPCTSTR lpszNewValue：细节参数值
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CShockwaveFlash::SetSWRemote(LPCTSTR lpszNewValue)
{
	static BYTE parms[] = VTS_BSTR;
	InvokeHelper(0x9f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :PreTranslateMessage
*  功能描述     :得到并处理WINDOWS消息
*  输入参数描述	:
*    MSG* pMsg：消息指针
*  返回变量说明 :处理成功返回TRUE，返回FALSE失败
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL CShockwaveFlash::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message ==WM_RBUTTONDOWN)
         return  1;
	if (pMsg->message ==WM_LBUTTONDOWN)
        Invalidate ();

	//屏蔽右健点击后的Flash控件默认的菜单。

	return CWnd::PreTranslateMessage (pMsg);

	return true;
}