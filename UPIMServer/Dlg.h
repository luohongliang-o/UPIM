#pragma once
#include "StdAfx.h"
#include <queue>
typedef struct StatisticsData
{
	long SendFlow;     //流量
	long SendPack;     //包
	long SendPackPeak; //最大请求包数
	long SendPeak;     //最大发送流量

	long ReceiveFlow;
	long ReceivePack;  
	long ReceivePackPeak;//最大接收包数
	long RecievePeak;    //最大接收流量

	long CurConnectNum; //连接数
	long ConnectPeak;   //最大连接数
	StatisticsData(long sf,long sp,long spp,long spk,long rf,long rp,long rpp,long rpk,long ccn,long cp)
	{
		SendFlow = sf;
		SendPack = sp;
		SendPackPeak = spp;
		SendPeak = spk;
		ReceiveFlow = rf;
		ReceivePack = rp;
		ReceivePackPeak = rpp;
		RecievePeak = rpk;
		CurConnectNum = ccn;
		ConnectPeak = cp;
	}
}SDATA,*PSDATA;



class CStatisticsDlg : public CDialog
{

public:
	CStatisticsDlg();
public:
	void UpdateData(PSDATA SData );
	void LoadUserOnLineInfo();
	void UpdateListData();
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
protected:
	BOOL AddListItem(const UserNode& userNode,int nCurItem);
	
private:
	std::hash_map<DWORD,UserNode> m_mapUserNode;
};