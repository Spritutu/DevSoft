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
#include <iterator>
#include "SensorTypeTable.h"

const std::string DEFAULT_TIME_NAME = "t(s)ʱ��";
const int SPECIAL_SENSOR_TYPE_ID = 10000;

CSensorManager::CSensorManager(void)
{
	m_SensorInfoArray.clear();
	//��ǰԤ��
	SENSOR_TYPE_INFO_ELEMENT SensorElement;
	SensorElement.strSensorName = DEFAULT_TIME_NAME;
	SensorElement.nSensorID = SPECIAL_SENSOR_TYPE_ID;
	SensorElement.nSensorSerialID = 0;
	m_SensorInfoArray.push_back(SensorElement);
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
	auto strSensorName = CSensorTypeTable::CreateInstance().QuerySensorNameByID(nSensorTypeID);
	return RegisterSensor(SENSOR_TYPE_INFO_ELEMENT(strSensorName.strSensorTypeName,nSensorTypeID,nSensorSerialID));
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

/*********************************************************
FunctionName:CreateInstance
FunctionDesc:������ʵ��
InputParam:
OutputParam:
ResultValue:
Author:xiaowei.han
*********************************************************/
CSensorManager& CSensorManager::CreateInstance(void)
{
	return s_obj;
}

/*********************************************************
FunctionName:QuerySensorIDByName
FunctionDesc:���ݴ����������ƻ�ȡ������ID��Ϣ
InputParam:
OutputParam:
ResultValue:
Author:xiaowei.han
*********************************************************/
bool CSensorManager::QuerySensorIDByName(const std::string& strSensorName,SENSOR_TYPE_KEY& SensorKeyID)
{
	auto Lock = boost::make_unique_lock(m_Lock);

	auto FindPred = [strSensorName](const SENSOR_TYPE_INFO_ELEMENT& SensorElement)->bool
	{

		if (strSensorName == SensorElement.strSensorName)
		{
			return true;
		}
		return false;
	};

	//�����Ƿ����
	auto Iter = std::find_if(m_SensorInfoArray.begin(),m_SensorInfoArray.end(),FindPred);

	if (Iter != m_SensorInfoArray.end())
	{
		SensorKeyID.nSensorID = Iter->nSensorID;
		SensorKeyID.nSensorSerialID = Iter->nSensorSerialID;
		return true;
	}
	return false;
}

/*********************************************************
FunctionName:GetSensorList
FunctionDesc:��ȡ���д�������Ϣ
InputParam:
OutputParam:
ResultValue:
Author:xiaowei.han
*********************************************************/
void CSensorManager::GetSensorList(std::vector<SENSOR_TYPE_INFO_ELEMENT>& SensorList,bool bInculde /*= false*/)
{
	SensorList.clear();
	auto Lock = boost::make_unique_lock(m_Lock);
	//�Ƿ��������Ĵ�����
	if (bInculde)
	{
		SensorList.assign(m_SensorInfoArray.begin(),m_SensorInfoArray.end());
	}
	else
	{
		auto FilterPred = [](const SENSOR_TYPE_INFO_ELEMENT& SensorElement)->bool
		{
			//�ҵ�����Ĵ�����
			if (SensorElement.nSensorID == SPECIAL_SENSOR_TYPE_ID && SensorElement.nSensorSerialID == 0)
			{
				return false;
			}
			return true;
		};
		std::copy_if(m_SensorInfoArray.begin(),m_SensorInfoArray.end(),std::back_inserter(SensorList),FilterPred);
	}
}

/*********************************************************
FunctionName:GetSpecialSensorID
FunctionDesc:
InputParam:
OutputParam:
ResultValue:
Author:xiaowei.han
*********************************************************/
SENSOR_TYPE_KEY CSensorManager::GetSpecialSensorID(void)
{
	return SENSOR_TYPE_KEY(SPECIAL_SENSOR_TYPE_ID,0);
}

/*********************************************************
FunctionName:IsSpecialSensorID
FunctionDesc:
InputParam:
OutputParam:
ResultValue:
Author:xiaowei.han
*********************************************************/
bool CSensorManager::IsSpecialSensorID(const SENSOR_TYPE_KEY& SensorKeyID)
{
	if (SensorKeyID.nSensorID == SPECIAL_SENSOR_TYPE_ID)
	{
		return true;
	}
	return false;
}

/*********************************************************
FunctionName:QuerySensorNameByID
FunctionDesc:
InputParam:
OutputParam:
ResultValue:
Author:xiaowei.han
*********************************************************/
std::string CSensorManager::QuerySensorNameByID(const SENSOR_TYPE_KEY& SensorKeyID)
{
	auto Lock = boost::make_unique_lock(m_Lock);

	auto FilterPred = [SensorKeyID](const SENSOR_TYPE_INFO_ELEMENT& SensorElement)->bool
	{
		if (SensorElement.nSensorID == SensorKeyID.nSensorID && SensorElement.nSensorSerialID == SensorKeyID.nSensorSerialID)
		{
			return true;
		}
		return false;
	};
	auto Iter = std::find_if(m_SensorInfoArray.begin(),m_SensorInfoArray.end(),FilterPred);

	if (Iter != m_SensorInfoArray.end())
	{
		return Iter->strSensorName;
	}

	return std::string("");
}

CSensorManager CSensorManager::s_obj;
