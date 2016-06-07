#include "StdAfx.h"
#include "FaceList.h"
#include "Utils.h"
#include "Path.h"

CFaceInfo::CFaceInfo(void)
{
	m_nId = -1;
	m_nIndex = -1;
	m_strTip = _T("");
	m_strFileName = _T("");
}

CFaceInfo::~CFaceInfo(void)
{

}

CFaceList::CFaceList(void)
{
	Reset();
}

CFaceList::~CFaceList(void)
{
	Reset();
}

void CFaceList::Reset()
{
// 	m_nWidth = 28;
// 	m_nHeight = 28;
// 	m_nZoomWidth = 86;
// 	m_nZoomHeight = 86;
// 	m_nCol = 15;
	m_nItemWidth = m_nItemHeight = 0;
	m_nZoomWidth = m_nZoomHeight = 0;
	m_nRow = m_nCol = 0;
	
	for (int i = 0; i < (int)m_arrFaceInfo.size(); i++)
	{
		CFaceInfo * lpFaceInfo = m_arrFaceInfo[i];
		if (lpFaceInfo != NULL)
			delete lpFaceInfo;
	}
	m_arrFaceInfo.clear();
}

BOOL CFaceList::LoadConfigFile(LPCTSTR lpszFileName)
{
	Reset();
	TiXmlDocument xmlDoc;
	BOOL bRet = xmlDoc.LoadFile(lpszFileName);
	if (!bRet)
		return FALSE;
	else
	{
		xmlDoc.Print();
		CString strRow = _T("");
		CString strCol = _T("");
		CString strItem_width = _T("");
		CString strItem_height = _T("");
		CString strZoom_width = _T("");
		CString strZoom_height = _T("");
		CString strID = _T("");
		CString strtip = _T("");
		CString strfile = _T("");
		TiXmlElement* rootElement = xmlDoc.RootElement();							// faceconfig元素  

		TiXmlAttribute* attributeOf_row = rootElement->FirstAttribute();			// 获得row属性  
		strRow = attributeOf_row->Value();
		TiXmlAttribute* attributeOf_col = attributeOf_row->Next();					// 获得col属性
		strCol = attributeOf_col->Value();
		TiXmlAttribute* attributeOf_item_width = attributeOf_col->Next();			// 获得item_width属性
		strItem_width = attributeOf_item_width->Value();
		TiXmlAttribute* attributeOf_item_height = attributeOf_item_width->Next();	// 获得item_height属性
		strItem_height = attributeOf_item_height->Value();
		TiXmlAttribute* attributeOf_zoom_width = attributeOf_item_height->Next();	// 获得zoom_width属性
		strZoom_width = attributeOf_zoom_width->Value();
		TiXmlAttribute* attributeOf_zoom_height = attributeOf_zoom_width->Next();	// 获得zoom_height属性
		strZoom_height = attributeOf_zoom_height->Value();
	
		m_nItemWidth = atoi(strItem_width);
		m_nItemHeight =atoi(strItem_height);
		m_nZoomWidth = atoi(strZoom_width);
		m_nZoomHeight = atoi(strZoom_height);
		m_nRow = atoi(strRow);
		m_nCol = atoi(strCol);
		
		TiXmlElement* FaceElement = rootElement->FirstChildElement();				//Face  
		for (; FaceElement != NULL; FaceElement = FaceElement->NextSiblingElement() ) 
		{
			TiXmlAttribute* attributeOf_id = FaceElement->FirstAttribute();			//获得id属性  
			strID = attributeOf_id->Value();
			TiXmlAttribute* attributeOf_tip = attributeOf_id->Next();				//获得tip属性  
			strtip = attributeOf_tip->Value();
			TiXmlAttribute* attributeOf_file = attributeOf_tip->Next();				//获得file属性  
			CString strtmp = attributeOf_file->Value();
			strfile.Format("\\%s",strtmp);

			CFaceInfo * lpFaceInfo = new CFaceInfo;
			if (lpFaceInfo != NULL)
			{
				lpFaceInfo->m_nId = atoi(strID);
				lpFaceInfo->m_strTip = strtip;
				lpFaceInfo->m_strFileName = g_config.szHomePath + strfile;
 				tstring strIndex = ZYM::CPath::GetFileNameWithoutExtension(lpFaceInfo->m_strFileName.c_str());
 				if (IsDigit(strIndex.c_str()))
 					lpFaceInfo->m_nIndex = _tcstol(strIndex.c_str(), NULL, 10);
				m_arrFaceInfo.push_back(lpFaceInfo);
			}
		}
	}

	return TRUE;
}

CFaceInfo * CFaceList::GetFaceInfo(int nIndex)
{
	if (nIndex >= 0 && nIndex < (int)m_arrFaceInfo[nIndex])
		return m_arrFaceInfo[nIndex];
	else
		return NULL;
}

CFaceInfo * CFaceList::GetFaceInfoById(int nFaceId)
{
	for (int i = 0; i < (int)m_arrFaceInfo.size(); i++)
	{
		CFaceInfo * lpFaceInfo = m_arrFaceInfo[i];
		if (lpFaceInfo != NULL && lpFaceInfo->m_nId == nFaceId)
			return lpFaceInfo;
	}

	return NULL;
}

CFaceInfo * CFaceList::GetFaceInfoByIndex(int nFaceIndex)
{
	for (int i = 0; i < (int)m_arrFaceInfo.size(); i++)
	{
		CFaceInfo * lpFaceInfo = m_arrFaceInfo[i];
		if (lpFaceInfo != NULL && lpFaceInfo->m_nIndex == nFaceIndex)
			return lpFaceInfo;
	}

	return NULL;
}