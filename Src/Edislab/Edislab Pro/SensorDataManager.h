/**********************************************************
*��Ȩ����:xiaowei.han
*�ļ�����:SensorDataManager.h
*��������:���������ݹ�����
*����:xiaowei.han
*����:2017/05/14
**********************************************************/
#pragma once
#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>
class CSensorData;
class CSensorDataManager
{
public:
	static CSensorDataManager& CreateInstance();

	//����һ������������
	void AddSensorData(int nSensorID);

	//ɾ��һ������������
	void DelSensorData(int nSensorID);

	//���ݴ�����ID��ȡ����������
	CSensorData* GetSensorDataBySensorID(int nSensorID);
private:
	CSensorDataManager(void);
	~CSensorDataManager(void);
private:
	//���������ݼ�������Ϊ���������
	boost::unordered_map<int,CSensorData*> m_SensorDataMap;
	//��
	boost::mutex m_SensorDataMapLock;
private:
	static CSensorDataManager s_obj;
};

