/****************************************************************************************
*��Ȩ����:
*�ļ���:Observable.h
*����:�۲���ģʽ--���۲���.ͬʱҲ��Ϊȫ�����ݵĲ�����Ļ���
*����:2016.9
******************************************************************************************/
#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <afx.h>
#include <set>
using std::set;

//����۲���
class CObserver;

//���۲�����
class CObservable
{
public:
	CObservable(void);
	virtual ~CObservable(void);

public:
	
	//ע��۲���
	void Attach(CObserver* pObs);
	//ע���۲���
	void Detach(CObserver* pObs);
	//ע�����й۲���
	void DetachAll();
	//��״̬�仯��������۲��ߣ����֪ͨ����
	void Notify(void* pArg = NULL);
	//����Ŀ��״̬�Ƿ�仯
	bool HasChanged();
	//��ȡ�۲�������
	int GetObserversCount();

public:
/************************ȫ�����ݲ���**********************************************/
/*****���ݲ��������֣�	1�� ������������/ɾ���� 2�� �û�Grid�༭������ɾ���ģ�*******/

/***********************************������*******************************/
	//��Ӵ��������յ�������
	void PushData(int nLab ,int nSensorID ,double dValue);
	void PushData(int nLab ,int nSensorID ,int dValue);
	void PushData(int nLab ,int nSensorID ,CString szValue);
	//ɾ������������
	void PopData(int nLab ,int nSensorID);

/*********************************�û�Grid�༭*******************************/
	//����������
	void CreateRowData(int nLabIndex ,int nRowIndex );
	//������
	void CreateColumnData(int nLabIndex ,int nSensorID );
	//����/����ĳ���ж�Ӧ��Itemֵ
	void InsertItemData(int nLabIndex ,int nRow ,int nCol ,CString szValue );
	//ɾ��������
	void DeleteRowData(int nLabIndex ,int nRowIndex );
	//ɾ��������
	void DeleteColumnData(int nLabIndex ,int nColIndex );

/*********************************ʵ�����**********************************/
	//��ȡ��ǰʵ�����
	int GetCurrLabTotal()const;
	//����ʵ��
	bool AddLab(int& nLabIndex);
	//ɾ��ʵ��
	void DeleteLab(int nLabIndex);

/***********************************����ȫ������ͨ�ú���**********************/
	
 
public:
 
	//����״̬�仯!!!����̳�CObservable��������Ŀ��״̬
	void SetChanged();
	//��ʼ��Ŀ��Ϊδ�仯״̬
	void ClearChanged();

private:

	//״̬
	bool m_bChanged;
	//set��֤Ŀ��Ψһ��
	set<CObserver*> m_setObs;

	//ʵ�����
	int	m_nLabCount;

private:

	//���Ҵ�����ID����Ӧ������������
	bool FindColIndex(int nLab ,int nSensorID ,int& nCol);

	void AdapterLabData(int nLab );
};
#endif