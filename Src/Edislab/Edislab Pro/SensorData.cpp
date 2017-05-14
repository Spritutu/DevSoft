#include "StdAfx.h"
#include "SensorData.h"
#include <boost/thread/lock_guard.hpp>
#include "Log.h"
//Ĭ�ϸ���
const int DEFAULT_MAX_LENGTH = 60;


CSensorData::CSensorData(void):
m_nSensorID(-1),
m_nMaxDataLength(DEFAULT_MAX_LENGTH)
{
	m_SensorDataList.clear();
}


CSensorData::~CSensorData(void)
{
	m_SensorDataList.clear();
}

void CSensorData::AddSensorData(float fData)
{
	if (m_nSensorID < 0)
	{
		ERROR_LOG("the sensor ID is invalid.");
		return;
	}
	using namespace boost;
	lock_guard<mutex> Lock(m_DataLock);

	//������������
	if (m_nMaxDataLength <= (int)m_SensorDataList.size())
	{
		//ɾ����һ��
		m_SensorDataList.pop_front();
	}

	m_SensorDataList.push_back(fData);

}

void CSensorData::GetSensorData(std::list<float>& SensorDataList)
{
	SensorDataList.clear();
	if (m_nSensorID < 0)
	{
		ERROR_LOG("the sensor ID is invalid.");
		return;
	}
	using namespace boost;
	lock_guard<mutex> Lock(m_DataLock);

	SensorDataList.assign(m_SensorDataList.begin(),m_SensorDataList.end());
}
