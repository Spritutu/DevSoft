#pragma once
#if 0
#include <vector>
#include "Observer.h"
class GaugeDlg;
class GaugeManager:public CObserver
{
public:
	GaugeManager(void);
	~GaugeManager(void);
	//�������ݣ����ݱ仯ʱ����,���±���
	virtual void Update(class CObservable* pObs, void* pArg = NULL);
	//���ӱ���
	void addGaugeDlg(GaugeDlg* dlg);
	//ɾ������
	void delGaugeDlg(GaugeDlg* dlg);
	//��������
	int saveData();
	//��ȡ����
	int readData();
private:
	//���б��̶Ի���
	std::vector<GaugeDlg*> m_vecGaugeDlg;
};
#endif
