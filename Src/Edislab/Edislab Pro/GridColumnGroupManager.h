/**********************************************************
*��Ȩ����:xiaowei.han
*�ļ�����:GridDisplayColumnInfo.h
*��������:Grid��ʾ����Ϣ���ݹ�����
*����:xiaowei.han
*����:2017/05/14
**********************************************************/
#pragma once
#include "GridGroupType.h"

class CGridColumnGroupManager
{
public:
	static CGridColumnGroupManager& CreateInstance();

	void GetGridDisplayInfo(std::vector<COLUMN_GROUP_INFO>& DisplayInfoArray);

	//��ѯ��ͷ�����Ƿ��Ѿ�����
	bool IsHeaderNameExist(const CString& strHeaderName);

	//�ж�ָ����ͷ�����Ƿ��Ѿ�����
	bool IsColumnExistInFixedHeader(const CString& strHeaderName,const CString& strColumnName);

	//�������Ϣ
	void AddDisplayColumnInfo(const COLUMN_GROUP_INFO& GridColumnInfo);

	//��ĳ��ͷ�����һ����Ϣ
	void AddDisplayColumnInfo(const CString& strHeaderName,const COLUMN_INFO& AddColumnInfo);

    //�Ƴ�ĳһ��
	void RemoveColumnInfo(const CString& strHeaderName,const CString& strColumnName);
	
	//�Ƴ�ĳһ��ͷ
	void RemoveHeader(const CString& strHeaderName);

	//�޸�����
	void ModifyHeaderInfo(const CString& strOldName,const CString& strNewName);
protected:
	CGridColumnGroupManager(void);
	~CGridColumnGroupManager(void);
private:
	void InitGridDisplayInfo();
protected:
	//Grid�ķ�����ʾ��Ϣ
	std::vector<COLUMN_GROUP_INFO> m_HeaderInfoArray;
private:
	static CGridColumnGroupManager s_obj;

};

