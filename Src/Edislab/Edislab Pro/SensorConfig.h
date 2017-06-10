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
	//������ʵ��
	static CSensorConfig& CreateInstance(void);
private:
	CSensorConfig(void);
	~CSensorConfig(void);
public:
	//����������Ϣ
	bool LoadSensorConfig(void);

	//��ȡCOM����
	const SENSOR_COM_CONFIG_ELEMENT& GetComConfig()
	{
		return m_SensorComConfig;
	}

	//��ȡ���д������б�
	void GetSensorList(std::vector<SENSOR_CONFIG_ELEMENT> &vecSensorArry, SENSOR_TYPE enumType);

	// ��ȡ������������Ϣ
	const std::vector<SENSOR_TYPE_INFO>& GetSensorTypeInfo()
	{
		return m_SensorTypeInfo;
	}

	//���ݴ�����ID���ش�������Ϣ
	SENSOR_CONFIG_ELEMENT GetSensorInfo(int nSensorID);
	
	//��ȡ����������������Ϣ
	const SENSOR_RECORD_INFO& GetSensorRecordInfo();

	//���ô���������������Ϣ
	void SetSensorRecordInfo(SENSOR_RECORD_INFO& _recordInfo);
private:
	//���ش������б���Ϣ
	bool LoadSensorList(rapidjson::Document& Parser);
	//���ش�����ͨ��������Ϣ
	bool LoadSensorComList(rapidjson::Document& Parser);
	//��
	boost::mutex m_RWLock;
	//���洫������������Ϣ
	std::vector<SENSOR_CONFIG_ELEMENT> m_SensorConfigArray;
	//������COMͨ������
	SENSOR_COM_CONFIG_ELEMENT m_SensorComConfig;
	// ���洫�����ɼ�������Ϣ
	SENSOR_RECORD_INFO m_SensorRecordInfo;
	//
	std::vector<SENSOR_TYPE_INFO> m_SensorTypeInfo;
private:
	static CSensorConfig s_SensorConfig;
};

