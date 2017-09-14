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
#include "Type.h"
class CSensorData;
class CSensorDataManager
{
public:
	static CSensorDataManager& CreateInstance();
#ifndef NEW_VERSION
	//����һ������������
	void AddSensorData(int nSensorID);

	//ɾ��һ������������
	void DelSensorData(int nSensorID);

	//���ݴ�����ID��ȡ����������
	CSensorData* GetSensorDataBySensorID(int nSensorID);
#else
	//����һ������������
	void AddSensorData(const SENSOR_TYPE_KEY& KeyElement);

	//ɾ��һ������������
	void DelSensorData(const SENSOR_TYPE_KEY& KeyElement);

	//���ݴ�����ID��ȡ����������
	CSensorData* GetSensorDataBySensorID(const SENSOR_TYPE_KEY& KeyElement);
#endif


private:
	CSensorDataManager(void);
	~CSensorDataManager(void);
private:
#ifndef NEW_VERSION
	//���������ݼ�������Ϊ���������
	boost::unordered_map<int,CSensorData*> m_SensorDataMap;
#else
	boost::unordered_map<SENSOR_TYPE_KEY,CSensorData*> m_SensorDataMap;
#endif
	//��
	boost::mutex m_SensorDataMapLock;
private:
	static CSensorDataManager s_obj;
};

