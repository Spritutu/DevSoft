/*******************************************************************************/
/*����:Grid�ؼ���ͼ������
/*����:
/*����:2015.8
/*******************************************************************************/
#ifndef GRID_VIEW_MANAGE_H
#define GRID_VIEW_MANAGE_H

#include "GridDefine.h"
#include "Observer.h"
#include <list>
using std::list;

//Grid�ؼ���ͼ������
class CGridViewManager: public CObserver
{
public:
	CGridViewManager(void);
	virtual ~CGridViewManager(void);

public:
	
	void AddGridView(CCustomLabGridCtrl* pGridView);
	
	void AddGridViewAttibute(CCustomLabGridCtrl* pGridView,GRID_FONT_TYPE type,vector<int> vecCol);

	void RemoveGridView(CCustomLabGridCtrl* pGridView);

	void GetGridViewList(list<VIEWMANAGER>& ViewList);

	int GetGridViewCount()const;

	void GridViewCountSub();

	void RefreshAll(CCustomLabGridCtrl* pGrid);

	void Update(CObservable* pObs, void* pArg = NULL);

private:
	list<VIEWMANAGER> m_GridViewList;
	CRITICAL_SECTION m_cs;
};

#endif