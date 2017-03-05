#include "StdAfx.h"
#include "Edislab Pro.h"
#include "ChartManager.h"
#include "ChartFigureDlg.h"
#include "GlobalDataManager.h"
using namespace std;

ChartManager::ChartManager(void)
{
	CGlobalDataManager::CreateInstance().Attach(this);
}


ChartManager::~ChartManager(void)
{
	CGlobalDataManager::CreateInstance().Detach(this);
}

//�������ݣ���ȫ��ʵ�����ݸ��µ���ͼר��������
void ChartManager::Update(CObservable* pObs, void* pArg)
{
	CGlobalDataManager* dbMgr = dynamic_cast<CGlobalDataManager*>(pObs);
	if(NULL==dbMgr)//����������
	{
		return;
	}
	//���µ����¼������
	for(UINT i= 0; i<m_vecChartDlg.size(); ++i)
	{
		if(m_vecChartDlg[i])
		{
			m_vecChartDlg[i]->updateData(dbMgr);
		}
	}
}
//��ȡ���ݣ��������
//const std::vector<ChartXYData*> ChartManager::getChartData()
//{
//	return m_vecChartData;
//}
//����ͼ��
void ChartManager::addChartDlg(const ChartFigureDlg* dlg)
{
	bool bExist = false;
	vector<const ChartFigureDlg*>::iterator itr = m_vecChartDlg.begin();
	while(itr!=m_vecChartDlg.end())
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
		m_vecChartDlg.push_back(dlg);
	}
}
//ɾ��ͼ��
void ChartManager::delChartDlg(const ChartFigureDlg* dlg)
{
	//m_chartDlg.erase(dlg);
	vector<const ChartFigureDlg*>::iterator itr = m_vecChartDlg.begin();
	while(itr!=m_vecChartDlg.end())
	{
		if(*itr==dlg)
		{
			m_vecChartDlg.erase(itr);
			break;
		}
		++itr;
	}
}
//��������
int ChartManager::saveData()
{
	//���µ����¼������
	for(UINT i= 0; i<m_vecChartDlg.size(); ++i)
	{
		if(m_vecChartDlg[i])
		{
			m_vecChartDlg[i]->saveData();
		}
	}
	return 0;
}
//��ȡ����
int ChartManager::readData()
{
	//��ȡ���ݲ�������Ӧ�ĶԻ���
	return 0;
}
