#include "StdAfx.h"
#include "SensorIDGenerator.h"
#include <boost/foreach.hpp>
#include <boost/thread/lock_guard.hpp>
#include <algorithm>
#include <string>
//ʱ�����ID
const int DEFAULT_TIME_AXIS_ID = 1000;
const std::string DEFAULT_TIME_NAME = "t(s)ʱ��";
CSensorIDGenerator::CSensorIDGenerator(void)
{
	SENSOR_TYPE_INFO_ELEMENT TempElement;
	TempElement.strSensorName = DEFAULT_TIME_NAME;
	TempElement.nSensorID = DEFAULT_TIME_AXIS_ID;
	m_SensorTypeArray.push_back(TempElement);
}

CSensorIDGenerator::~CSensorIDGenerator(void)
{
}

CSensorIDGenerator& CSensorIDGenerator::CreateInstance( void )
{
	return s_Instance;
}

/*******************************************************************
*��������:GetSpecialSensorID
*��������:��ȡ�����豸��ID
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/10 11:26:04
*******************************************************************/
int CSensorIDGenerator::GetSpecialSensorID(void)
{
	return DEFAULT_TIME_AXIS_ID;
}

/*******************************************************************
*��������:IsSpecialSensorID
*��������:�Ƿ��������豸��ID
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/10 11:26:07
*******************************************************************/
bool CSensorIDGenerator::IsSpecialSensorID(int nSensorID)
{
	if (nSensorID == DEFAULT_TIME_AXIS_ID)
	{
		return true;
	}

	return false;
}

/*******************************************************************
*��������:QuerySensorTypeIDByName
*��������:���ݴ��������ƻ�ȡ��Ӧ��ID
*�������:
*�������:
*����ֵ:������ID
*����:xiaowei.han
*����:2017/06/10 8:45:49
*******************************************************************/
int CSensorIDGenerator::QuerySensorTypeIDByName( const std::string& strSensorName )
{
	using namespace boost;
	if (strSensorName.empty())
	{
		return -1;
	}
	auto FindPred = [&strSensorName](const SENSOR_TYPE_INFO_ELEMENT& Element)->bool
	{
		if (Element.strSensorName == strSensorName)
		{
			return true;
		}
		return false;
	};
	int nSensorID = -1;
	lock_guard<mutex> Lock(m_Lock);
	auto FindIter = std::find_if(m_SensorTypeArray.begin(),m_SensorTypeArray.end(),FindPred);
	if (FindIter != m_SensorTypeArray.end())
	{
		nSensorID = FindIter->nSensorID;
	}
	return nSensorID;
}

/*******************************************************************
*��������:QueryrNameBySensorID
*��������:���ݴ�����ID��ȡ����������
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/10 8:46:21
*******************************************************************/
std::string CSensorIDGenerator::QueryrNameBySensorID(int nID)
{
	using namespace boost;
	std::string strSensorName;

	auto FindPred = [&nID](const SENSOR_TYPE_INFO_ELEMENT& Element)->bool
	{
		if (Element.nSensorID == nID)
		{
			return true;
		}
		return false;
	};

	lock_guard<mutex> Lock(m_Lock);
	auto FindIter = std::find_if(m_SensorTypeArray.begin(),m_SensorTypeArray.end(),FindPred);
	//����ҵ�
	if (FindIter != m_SensorTypeArray.end())
	{
		strSensorName = FindIter->strSensorName;
	}
	return strSensorName;
}

/*******************************************************************
*��������:AddSensor
*��������:����µĴ�����
*�������:
*�������:
*����ֵ:�µĴ�������Ӧ��ID
*����:xiaowei.han
*����:2017/06/10 8:47:25
*******************************************************************/
int CSensorIDGenerator::AddSensor( const std::string& strSensorName)
{
	using namespace boost;
	if (strSensorName.empty())
	{
		return -1;
	}
	auto FindPred = [&strSensorName](const SENSOR_TYPE_INFO_ELEMENT& Element)->bool
	{
		if (Element.strSensorName == strSensorName)
		{
			return true;
		}
		return false;
	};
	lock_guard<mutex> Lock(m_Lock);
	auto Iter = std::find_if(m_SensorTypeArray.begin(),m_SensorTypeArray.end(),FindPred);
	if (Iter != m_SensorTypeArray.end())
	{
		return Iter->nSensorID;
	}
	//���������
	SENSOR_TYPE_INFO_ELEMENT TempElement;
	TempElement.strSensorName = strSensorName;
	TempElement.nSensorID = s_nTypeIndex++;
	m_SensorTypeArray.push_back(TempElement);
	return TempElement.nSensorID;
}

/*******************************************************************
*��������:DelSensor
*��������:ɾ��������
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/10 8:47:49
*******************************************************************/
void CSensorIDGenerator::DelSensor( const std::string& strSensorName )
{
	using namespace boost;
	if (strSensorName.empty())
	{
		return;
	}

	auto FindPred = [&strSensorName](const SENSOR_TYPE_INFO_ELEMENT& Element)->bool
	{
		if (Element.strSensorName == strSensorName)
		{
			return true;
		}
		return false;
	};
	
	lock_guard<mutex> Lock(m_Lock);

	auto Iter = std::find_if(m_SensorTypeArray.begin(),m_SensorTypeArray.end(),FindPred);

	if (Iter != m_SensorTypeArray.end())
	{
		m_SensorTypeArray.erase(Iter);
	}

}

/*******************************************************************
*��������:GetAllSensorName
*��������:��ȡ���д�����������
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/10 8:48:12
*******************************************************************/
void CSensorIDGenerator::GetAllSensorName(std::vector<std::string>& SensorNameArray,bool bInclude /*= false*/)
{
	using namespace boost;
	SensorNameArray.clear();
	lock_guard<mutex> Lock(m_Lock);

	if (bInclude)
	{
		BOOST_FOREACH(auto& Element,m_SensorTypeArray)
		{
			SensorNameArray.push_back(Element.strSensorName);
		}
	}
	else
	{
		BOOST_FOREACH(auto& Element,m_SensorTypeArray)
		{
			if (Element.nSensorID != DEFAULT_TIME_AXIS_ID)
			{
				SensorNameArray.push_back(Element.strSensorName);
			}		
		}
	}


}

/*******************************************************************
*��������:IsSensorExist
*��������:�жϴ������Ƿ����
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/10 8:48:39
*******************************************************************/
bool CSensorIDGenerator::IsSensorExist(const std::string& strSensorName)
{
	using namespace boost;
	bool bResult = false;
	auto FindPred = [&strSensorName](const SENSOR_TYPE_INFO_ELEMENT& Element)->bool
	{
		if (Element.strSensorName == strSensorName)
		{
			return true;
		}
		return false;
	};

	lock_guard<mutex> Lock(m_Lock);
	auto FindIter = std::find_if(m_SensorTypeArray.begin(),m_SensorTypeArray.end(),FindPred);
	//����ҵ�
	if (FindIter != m_SensorTypeArray.end())
	{
		bResult = true;
	}
	return bResult;
}

int CSensorIDGenerator::s_nTypeIndex = 1;

CSensorIDGenerator CSensorIDGenerator::s_Instance;
