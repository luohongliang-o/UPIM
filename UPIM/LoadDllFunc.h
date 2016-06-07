#pragma once
#include "StdAfx.h"

// 加载截图dll中的PupupDialog接口
typedef void (*IUPIM_PupupDialog)(HWND m_hwnd);
extern IUPIM_PupupDialog	IUPIM_PupupDialog_;
extern HMODULE	II_mod;

extern BOOL LoadDLLFunc();
extern void UnLoadDLLFunc();