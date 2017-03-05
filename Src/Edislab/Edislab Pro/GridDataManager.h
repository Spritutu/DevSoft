/*******************************************************************************/
/*����:Grid���ݹ�����
/*����:
/*����:2015.8
/*******************************************************************************/

#ifndef GRID_DATA_MANAGER_H
#define GRID_DATA_MANAGER_H

#include <vector>
using std::vector;
#include "GridDefine.h"

class CGridDataManager
{
public:
	CGridDataManager(void);
	virtual ~CGridDataManager(void);

public:

	void InitGroupInfo();

	void SetGroupInfo(GROUPDATA ColData);

	void InitColumnInfo();

	void SetColumnInfo(int nColIndex,COLUMNDATA ColData);

	//���Grid Item����
	void GetGridItemData(int nRow,int nColumn,_variant_t& ItemValue);

	void AddColumnInfo();

	GROUPDATA& GetGridGroupInfo();

	vector <COLUMNDATA>& GetGridColummnInfo();

	vector<vector<CString>>& GetGridData();

private:
	//Grid�������ݣ�����ͷ��
	vector<vector<CString>> m_vecGridData;

/**********************************************************************/
//Ŀǰ֧�ֵ�һ��

	//Grid��������Ϣ
	GROUPDATA	m_GridGroupInfo;
	//Grid��������Ϣ������Item value��ֵ��
	vector <COLUMNDATA> m_vecGridColumnInfo;
/**********************************************************************/
};

#endif