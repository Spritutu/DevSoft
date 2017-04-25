/************************************************************************************
Copyright(C):self
FileName:SensorConfig.h
Description:�����������࣬���д��������ôӸ����ȡ
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
************************************************************************************/
#pragma once
#include <vector>
#include <boost/thread/mutex.hpp>
#include <string>
#include "Type.h"
#define COM_TEST (1)
class CSensorConfig
{

public:
	static CSensorConfig& CreateInstance(void);

private:
	CSensorConfig(void);
	~CSensorConfig(void);

public:
	//����������Ϣ
	bool LoadSensorConfig(void);

	//���ݴ��������ƻ�ȡ������COM������
	bool GetSensorConfigBySensorName(const std::string& strSensorName,LP_SENSOR_CONFIG_ELEMENT pConfig);

#ifdef COM_TEST
	bool GetFirstSensorConfig(LP_SENSOR_CONFIG_ELEMENT pConfig);
#endif
private:
	//��
	boost::mutex m_RWLock;
	//���洫������������Ϣ
	std::vector<SENSOR_CONFIG_ELEMENT> m_SensorConfigArray;
	//���洫������COMͨ������
	std::vector<SENSOR_COM_CONFIG_ELEMENT> m_SensorComConfigArray;
private:
	static CSensorConfig s_SensorConfig;
};

