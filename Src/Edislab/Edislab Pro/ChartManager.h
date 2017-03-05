#pragma once
#include <vector>
#include "Observer.h"

class ChartFigureDlg;
class ChartXYData;
class ChartManager:public CObserver
{
public:
	ChartManager(void);
	virtual ~ChartManager(void);
	//�������ݣ���ȫ��ʵ�����ݸ��µ���ͼר��������
	virtual void Update(class CObservable* pObs, void* pArg = NULL);
	//��ȡ���ݣ��������
	//const std::vector<ChartXYData*> getChartData();
	//����ͼ��
	void addChartDlg(const ChartFigureDlg* dlg);
	//ɾ��ͼ��
	void delChartDlg(const ChartFigureDlg* dlg);
	//��������
	int saveData();
	//��ȡ����
	int readData();
private:
	//ͼ��������
	//std::vector<ChartXYData*> m_vecChartData;
	//����ͼ��Ի���
	std::vector<const ChartFigureDlg*> m_vecChartDlg;
};
