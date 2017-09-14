#include "StdAfx.h"
#include "Edislab Pro.h"
#include "GaugeDlg.h"
#include "GaugeManager.h"
//#include "GlobalDataManager.h"
using namespace std;
GaugeManager::GaugeManager(void)
{
	//CGlobalDataManager::CreateInstance().Attach(this);
}


GaugeManager::~GaugeManager(void)
{
	//CGlobalDataManager::CreateInstance().Detach(this);
}
//�������ݣ����ݱ仯ʱ����,���±���
void GaugeManager::Update(CObservable* pObs, void* pArg)
{
	//CGlobalDataManager* dbMgr = dynamic_cast<CGlobalDataManager*>(pObs);
	//if(NULL==dbMgr)//����������
	//{
	//	return;
	//}
	//���µ����¼������
	for(UINT i= 0; i<m_vecGaugeDlg.size(); ++i)
	{
		if(m_vecGaugeDlg[i])
		{
			m_vecGaugeDlg[i]->updateData();
		}
	}
}
//���ӱ���
void GaugeManager::addGaugeDlg(GaugeDlg* dlg)
{
	bool bExist = false;
	vector<GaugeDlg*>::iterator itr = m_vecGaugeDlg.begin();
	while(itr!=m_vecGaugeDlg.end())
	{
		if(*itr==dlg)
		{
			bExist = true;
			break;
		}
		++itr;
	}
	if(!bExist)
	{
		m_vecGaugeDlg.push_back(dlg);
	}
}
//ɾ������
void GaugeManager::delGaugeDlg(GaugeDlg* dlg)
{
	vector<GaugeDlg*>::iterator itr = m_vecGaugeDlg.begin();
	while(itr!=m_vecGaugeDlg.end())
	{
		if(*itr==dlg)
		{
			m_vecGaugeDlg.erase(itr);
			break;
		}
		++itr;
	}
}
//��������
int GaugeManager::saveData()
{
	return 0;
}
//��ȡ����
int GaugeManager::readData()
{
	return 0;
}
