/****************************************************************************************
*��Ȩ����:
*�ļ���:GlobalDataManager.h
*����:ȫ�����ݹ�����
*����:2016.9
******************************************************************************************/

#ifndef GLOBAL_DATA_MANAGER_H
#define GLOBAL_DATA_MANAGER_H

#include "Observable.h"
#include "Global.h"
#include "Lock.h"

#define GLOBAL_DATA		CGlobalDataManager::CreateInstance()
//ȫ�����ݹ�����
class CGlobalDataManager :public CObservable
{
public:
	static CGlobalDataManager& CreateInstance();
private:
	CGlobalDataManager(void);
	virtual ~CGlobalDataManager(void);

public:
	//���ݲ�������

	//��ʼ������
	void Init();

	//�����
	void AppendGroup();

	//��Ӵ�����������
	void AppendColumn(int nSensorID,int nSensorChanel);
	void AppendColumn(int nSensorID,int nSensorChanel,const std::string& strUnit);
	//���ʱ��������
	void AppendTimeColumn();

	//�������
	void PushData(int nColID ,int nRowPos);
	void PushData(int nSensorChan,short nValue);
	//void PushData(int nColID ,int nStartRowPos ,int nEndRowPos ,VEC_STRING vecValue);
	void PushData(int nRowPos );
	void PushData(int nSensorChan,float fValue);
	//�޸�����
	void ModifyData(int nColID ,int nRowPos ,CString szValue);
	void ModifyData(int nColID ,int nStartRowPos ,int nEndRowPos ,VEC_STRING vecValue);
	//ɾ������
	void PopAllData();
	void PopGroupData(const int nGroupID);
	void PopData(int nColID);
	void PopData(int nColID ,int nRowPos);
	void PopData(int nColID ,int nStartRowPos ,int nEndRowPos);

	//ɾ����������
	void DeleteAllContainer();
	void DeleteContainer(int nColID);

	//ȡ���������
	int GetMaxRow();

	//ȡ��ÿ��Item����
	void GetItemData(int nCol,int nRow,CString& strValue);

	VECTOR<GROUPDATA> getAllData(){ return m_allData;}

	//ͨ��ͨ������������ID
	int FindTimeID();

private:
	//�Ƿ�������
	bool IsHaveData();
	//����Ϊ������״̬
	void SetHaveDataStatus();
	//���ó�������״̬
	void SetNotHaveDataStatus();

	//��ǰ���Ƿ��д�����������
	bool IsHaveSensorCol();

private:
	//����ȫ�����ݵ��ٽ���
	CLock m_cs;

	bool m_bHaveData;
	//ȫ������
	VECTOR<GROUPDATA> m_allData; //ÿ�д�����������ܲ�һ��
};

#endif
