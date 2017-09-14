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
#include <string>
#include "Type.h"
class CSensorManager
{

public:
	static CSensorManager& CreateInstance(void);
private:
	CSensorManager(void);
	~CSensorManager(void);
public:
	//��ʼע��һ��������
	bool RegisterSensor(const SENSOR_TYPE_INFO_ELEMENT& SensorElement);
	//����
	bool RegisterSensor(int nSensorTypeID,int nSensorSerialID);
	//��ע��һ��������
	bool UnRegisterSensor(const SENSOR_TYPE_INFO_ELEMENT& SensorElement);
	//����
	bool UnRegisterSensor(int nSensorTypeID,int nSensorSerialID);
	//���ݴ��������ƻ�ȡ������ID
	bool QuerySensorIDByName(const std::string& strSensorName,SENSOR_TYPE_KEY& SensorKeyID);
	//��ȡ���д�������Ϣ
	void GetSensorList(std::vector<SENSOR_TYPE_INFO_ELEMENT>& SensorList,bool bInculde = false);
	//��ȡ���⴫������ID
	SENSOR_TYPE_KEY GetSpecialSensorID(void);
	//�ж��Ƿ�������Ĵ�����ID
	bool IsSpecialSensorID(const SENSOR_TYPE_KEY& SensorKeyID);
	//���ݴ�����ID��ȡ����������
	std::string QuerySensorNameByID(const SENSOR_TYPE_KEY& SensorKeyID);
private:
	//��������Ϣ�б�
	std::vector<SENSOR_TYPE_INFO_ELEMENT> m_SensorInfoArray;
	//��
	boost::mutex m_Lock;
	//ȫ��Ψһʵ��
	static CSensorManager s_obj;
};

