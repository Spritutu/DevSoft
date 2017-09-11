/*********************************************************
Copyright(C):
FileName:SensorManager.h
Descripton:ȫ�ִ�����������
Author:xiaowei.han
Data:2017/09/11
Others:
History:
Version:1.0
*********************************************************/
#pragma once
#include <vector>
#include <boost/thread/mutex.hpp>
#include "Type.h"
class CSensorManager
{

public:
	static CSensorManager& CreateInstance(void);
private:
	CSensorManager(void);
	virtual ~CSensorManager(void);

	//��ʼע��һ��������
	bool RegisterSensor(const SENSOR_TYPE_INFO_ELEMENT& SensorElement);
	//����
	bool RegisterSensor(int nSensorTypeID,int nSensorSerialID);
	//��ע��һ��������
	bool UnRegisterSensor(const SENSOR_TYPE_INFO_ELEMENT& SensorElement);
	//����
	bool UnRegisterSensor(int nSensorTypeID,int nSensorSerialID);

private:
	//��������Ϣ�б�
	std::vector<SENSOR_TYPE_INFO_ELEMENT> m_SensorInfoArray;
	//��
	boost::mutex m_Lock;
	//ȫ��Ψһʵ��
	static CSensorManager s_obj;
};

