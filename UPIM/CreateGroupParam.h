#pragma once

class CCreateGroupParam
{
public:
	CCreateGroupParam(void);
	~CCreateGroupParam(void);

	int			SetMapCreateGroupParam(int grouptype, const char* roomname, LPCUSERID masterid, std::vector<GUPID>* pVtFriend);
	LPCGPARAM	GetDataFromCreateMapKey(int nKey);
	void		RemoveGroupParamFromKey(int nKey);
	void		ClearMapCreate();
private:
	int	m_nCreateGroupKey;
	std::map<int, LPCGPARAM> m_mapCreteGroup;
};

