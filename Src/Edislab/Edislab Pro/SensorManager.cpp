/*********************************************************
Copyright(C):
FileName:SensorManager.cpp
Descripton:ȫ�ִ�����������
Author:xiaowei.han
Data:2017/09/11
Others:
History:
Version:1.0
*********************************************************/
#include "StdAfx.h"
#include "SensorManager.h"
#include <boost/thread/lock_factories.hpp>
#include <algorithm>
#include <string>

CSensorManager::CSensorManager(void)
{
	m_SensorInfoArray.clear();
}


CSensorManager::~CSensorManager(void)
{
	m_SensorInfoArray.clear();
}

/*********************************************************
FunctionName:RegisterSensor
FunctionDesc:ע�ᴫ����
InputParam:
OutputParam:
ResultValue:
Author:xiaowei.han
*********************************************************/
bool CSensorManager::RegisterSensor(const SENSOR_TYPE_INFO_ELEMENT& SensorElement)
{
	auto Lock = boost::make_unique_lock(m_Lock);
	auto Iter = std::find(m_SensorInfoArray.begin(),m_SensorInfoArray.end(),SensorElement);
	//����ҵ�
	if (Iter != m_SensorInfoArray.end())
	{
		return false;
	}
	m_SensorInfoArray.push_back(SensorElement);
	return true;
}

/*********************************************************
FunctionName:RegisterSensor
FunctionDesc:ע�ᴫ����
InputParam:
OutputParam:
ResultValue:
Author:xiaowei.han
*********************************************************/
bool CSensorManager::RegisterSensor(int nSensorTypeID,int nSensorSerialID)
{
	using namespace std;
	//���ݴ��������͵�ID��ѯ������������
	string strSensorName;
	return RegisterSensor(SENSOR_TYPE_INFO_ELEMENT(strSensorName,nSensorTypeID,nSensorSerialID));
}

/*********************************************************
FunctionName:RegisterSensor
FunctionDesc:ע�ᴫ����
InputParam:
OutputParam:
ResultValue:
Author:xiaowei.han
*********************************************************/
bool CSensorManager::UnRegisterSensor(const SENSOR_TYPE_INFO_ELEMENT& SensorElement)
{
	auto Lock = boost::make_unique_lock(m_Lock);
	auto Iter = std::find(m_SensorInfoArray.begin(),m_SensorInfoArray.end(),SensorElement);

	//�ҵ���Ӧ��Ԫ��
	if (Iter != m_SensorInfoArray.end())
	{
		m_SensorInfoArray.erase(Iter);
	}
	
	return true;
}

/*********************************************************
FunctionName:RegisterSensor
FunctionDesc:ע�ᴫ����
InputParam:
OutputParam:
ResultValue:
Author:xiaowei.han
*********************************************************/
bool CSensorManager::UnRegisterSensor(int nSensorTypeID,int nSensorSerialID)
{
	return UnRegisterSensor(SENSOR_TYPE_INFO_ELEMENT("",nSensorTypeID,nSensorSerialID));
}

CSensorManager& CSensorManager::CreateInstance(void)
{
	return s_obj;
}

CSensorManager CSensorManager::s_obj;
