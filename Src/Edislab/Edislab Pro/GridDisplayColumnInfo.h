/**********************************************************
*��Ȩ����:xiaowei.han
*�ļ�����:GridDisplayColumnInfo.h
*��������:Grid��ʾ����Ϣ���ݹ�����
*����:xiaowei.han
*����:2017/05/14
**********************************************************/
#pragma once
#include <vector>

typedef struct _column_info
{
	CString strColumnName;
	int nSensorID;

	_column_info()
	{
		strColumnName = _T("");
		nSensorID = -1;
	}

	void Reset()
	{
		strColumnName = _T("");
		nSensorID = -1;
	}
}COLUMN_INFO,* LP_COLUMN_INFO;

typedef struct _grid_display_info
{
	//�е�����
	CString strHeadName;

	//�����е�����
	std::vector<COLUMN_INFO> ContainColumnIndexArray;

	_grid_display_info()
	{
		strHeadName = _T("");
		ContainColumnIndexArray.clear();
	}

	void Reset()
	{
		strHeadName = _T("");
		ContainColumnIndexArray.clear();
	}

}GRID_DISPLAY_INFO,* LP_GRID_DISPLAY_INFO;

class CGridDisplayColumnInfo
{
public:
	static CGridDisplayColumnInfo& CreateInstance();

	void GetGridDisplayInfo(std::vector<GRID_DISPLAY_INFO>& DisplayInfoArray);

	//��ѯ��ͷ�����Ƿ��Ѿ�����
	bool IsHeaderNameExist(const CString& strHeaderName);

	//�ж�ָ����ͷ�����Ƿ��Ѿ�����
	bool IsColumnExistInFixedHeader(const CString& strHeaderName,const CString& strColumnName);

	//���������ƻ�ȡ��Ӧ�Ĵ�����ID
	int QuerySensorIDByColumnName(const CString& strColumnName);

	//�������Ϣ
	void AddDisplayColumnInfo(const GRID_DISPLAY_INFO& GridColumnInfo);

	//��ĳ��ͷ�����һ����Ϣ
	void AddDisplayColumnInfo(const CString& strHeaderName,const COLUMN_INFO& AddColumnInfo);

    //�Ƴ�ĳһ��
	void RemoveColumnInfo(const CString& strHeaderName,const CString& strColumnName);
	
	//�Ƴ�ĳһ��ͷ
	void RemoveHeader(const CString& strHeaderName);
protected:
	CGridDisplayColumnInfo(void);
	~CGridDisplayColumnInfo(void);
private:
	void InitGridDisplayInfo();
protected:
	//Grid����ʾ��Ϣ
	std::vector<GRID_DISPLAY_INFO> m_HeaderInfoArray;
private:
	static CGridDisplayColumnInfo s_obj;

};

