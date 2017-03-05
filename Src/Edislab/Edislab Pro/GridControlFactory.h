/***********************************************************************************/
/*����:Grid�ؼ�����(����ͼGrid���)
/*����:
/*����:2015.8
/************************************************************************************/
#ifndef GRID_CTRL_FACTORY_H
#define GRID_CTRL_FACTORY_H

#include <windef.h>
#include "GridViewManager.h"
#include "GridDataManager.h"

class CGridControlFactory
{
public:
	CGridControlFactory(void);
	virtual ~CGridControlFactory(void);

public:
	//Ϊ�������Grid�ؼ�
	CWnd* AddGridCtrl(CWnd* _Parent);
	//�Ƴ�����Grid�ؼ�
	void RemoveGridCtrl(CWnd* pWnd);

	//���ʵ���飨���Ƶ�ǰ�飩
	//void AddLabGroup();
	//���ʵ���У���/������
	void AddLabColumn();

private:
	//��ʼ��Grid��Ϣ
	void InitGridInfo();
	//����Grid��Ϣ
	void CopyGridInfo();

private:

	CGridViewManager*	m_ViewManager;
	CGridDataManager*	m_DataManager;
};

#endif