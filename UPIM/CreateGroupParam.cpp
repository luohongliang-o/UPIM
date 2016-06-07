#include "StdAfx.h"
#include "CreateGroupParam.h"

CCreateGroupParam::CCreateGroupParam(void)
{
	m_nCreateGroupKey = 0;
}

CCreateGroupParam::~CCreateGroupParam(void)
{
	ClearMapCreate();
}

int	CCreateGroupParam::SetMapCreateGroupParam(int grouptype, const char* roomname, LPCUSERID masterid, std::vector<GUPID>* pVtFriend)
{
	LPCGPARAM pCGParam = new CGPARAM;
	pCGParam->m_nRoomID = 0;
	pCGParam->m_nRoomType = grouptype;
	COPY_USERID(pCGParam->m_szMasterID, masterid);
	COPYSTRARRAY(pCGParam->m_szRoomName, roomname);
	pCGParam->m_vtUpID.clear();
	if (pVtFriend) 
		pCGParam->m_vtUpID.assign(pVtFriend->begin(), pVtFriend->end());
	m_nCreateGroupKey++;
	m_mapCreteGroup[m_nCreateGroupKey] = pCGParam;
	return m_nCreateGroupKey;
}

LPCGPARAM CCreateGroupParam::GetDataFromCreateMapKey(int nKey)
{
	if(nKey<=0) 
		return NULL;
	std::map<int, LPCGPARAM>::iterator iter = m_mapCreteGroup.find(nKey);
	if(iter!=m_mapCreteGroup.end())
	{
		LPCGPARAM pCGParam = iter->second;
		return pCGParam;
	}
	return NULL;
}

void CCreateGroupParam::RemoveGroupParamFromKey(int nKey)
{
	if(nKey<=0) 
		return ;
	std::map<int, LPCGPARAM>::iterator iter = m_mapCreteGroup.find(nKey);
	if(iter!=m_mapCreteGroup.end())
	{
		LPCGPARAM pCGParam = iter->second;
		TDEL(pCGParam);
		m_mapCreteGroup.erase(iter);
	}
}

void CCreateGroupParam::ClearMapCreate()
{
	std::map<int, LPCGPARAM>::iterator iter = m_mapCreteGroup.begin();
	LPCGPARAM pCGParam = NULL;
	while (iter != m_mapCreteGroup.end())
	{
		pCGParam = iter->second;
		TDEL(pCGParam);
		iter++;
	}
	m_mapCreteGroup.clear();
}
