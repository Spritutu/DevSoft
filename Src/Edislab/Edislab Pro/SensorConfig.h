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
#include <rapidjson/document.h>
#include "Type.h"
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
private:
	//���ش������б���Ϣ
	bool LoadSensorList(rapidjson::Document& Parser);
	//���ش�����ͨ��������Ϣ
	bool LoadSensorComList(rapidjson::Document& Parser);
	//��
	boost::mutex m_RWLock;
	//���洫������������Ϣ
	std::vector<SENSOR_CONFIG_ELEMENT> m_SensorConfigArray;
	//���洫������COMͨ������
	//std::vector<SENSOR_COM_CONFIG_ELEMENT> m_SensorComConfigArray;
	SENSOR_COM_CONFIG_ELEMENT m_SensorComConfig;
private:
	static CSensorConfig s_SensorConfig;
};

