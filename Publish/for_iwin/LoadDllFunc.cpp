#include "StdAfx.h"
#include "LoadDllFunc.h"

CString g_strHomePath;
IMDLL	g_infoDll = {0};

BOOL LoadDLLFunc()
{
	CString strFileName;
	strFileName.Format("%sUPIM.dll",g_strHomePath);

	g_infoDll.m_hModule = AfxLoadLibrary( strFileName );
	if( !g_infoDll.m_hModule ) return FALSE;	

	g_infoDll.m_pUEXP_Init			= (UEXP_Init)GetProcAddress(g_infoDll.m_hModule, "UEXP_Init" );
	g_infoDll.m_pUEXP_SetLoginType	= (UEXP_SetLoginType)GetProcAddress(g_infoDll.m_hModule, "UEXP_SetLoginType" );
	g_infoDll.m_pUEXP_Login			= (UEXP_Login)GetProcAddress(g_infoDll.m_hModule, "UEXP_Login" );
	g_infoDll.m_pUEXP_Resize		= (UEXP_Resize)GetProcAddress(g_infoDll.m_hModule, "UEXP_Resize" );
	g_infoDll.m_pUEXP_Uninit		= (UEXP_Uninit)GetProcAddress(g_infoDll.m_hModule, "UEXP_Uninit" );
	
	if(!g_infoDll.m_pUEXP_Init || !g_infoDll.m_pUEXP_SetLoginType || !g_infoDll.m_pUEXP_Login || !g_infoDll.m_pUEXP_Resize || !g_infoDll.m_pUEXP_Uninit)
		return FALSE;

	return TRUE;
}