/**********************************************************
*��Ȩ����:xiaowei.han
*�ļ�����:GridGroupType.h
*��������:Grid������ʾ�����Ͷ���
*����:xiaowei.han
*����:2017/06/04
**********************************************************/

#ifndef GRID_GROUP_H
#define GRID_GROUP_H
#include <vector>
//Grid�е��е���Ϣ
typedef struct _column_info
{
	CString strColumnName;
	_column_info()
	{
		strColumnName = _T("");
	}
	void Reset()
	{
		strColumnName = _T("");
	}
}COLUMN_INFO,* LP_COLUMN_INFO;

//Grid�ؼ����еķ�����Ϣ
typedef struct _column_group_info
{
	//���������
	CString strGroupName;

	//����Ŀ����Ϣ
	std::vector<COLUMN_INFO> ColumnArray;

	_column_group_info()
	{
		strGroupName = _T("");
		ColumnArray.clear();
	}

	void Reset()
	{
		strGroupName = _T("");
		ColumnArray.clear();
	}

}COLUMN_GROUP_INFO,* LP_COLUMN_GROUP_INFO;


//�е���ʾ��Ϣ
typedef struct _show_column_info
{
	CString strColumnName;
	bool bShow;
	_show_column_info()
	{
		strColumnName = _T("");
		bShow = true;
	}
	void Reset()
	{
		strColumnName = _T("");
		bShow = true;
	}
}SHOW_COLUMN_INFO,* LP_SHOW_COLUMN_INFO;

//�еķ�����ʾ��Ϣ
typedef struct _show_column_group_info
{
	//���������
	CString strGroupName;

	//����Ŀ����Ϣ
	std::vector<SHOW_COLUMN_INFO> ColumnArray;

	_show_column_group_info()
	{
		strGroupName = _T("");
		ColumnArray.clear();
	}

	void Reset()
	{
		strGroupName = _T("");
		ColumnArray.clear();
	}

}SHOW_COLUMN_GROUP_INFO,* LP_SHOW_COLUMN_GROUP_INFO;

#endif