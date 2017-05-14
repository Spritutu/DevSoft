#include "StdAfx.h"
#include "SensorDataManager.h"
#include <boost/foreach.hpp>
#include <boost/thread/lock_guard.hpp>
#include "SensorData.h"
void CSensorDataManager::AddSensorData(int nSensorID)
{
	using namespace boost;
	lock_guard<mutex> Lock(m_SensorDataMapLock);

    auto Iter = m_SensorDataMap.find(nSensorID);
	//�����ڲ�����
	if (Iter == m_SensorDataMap.end())
	{
		CSensorData* pData = new CSensorData;
		if (nullptr != pData)
		{
			//���ô�������ID
			pData->SetSensorID(nSensorID);
			m_SensorDataMap.emplace(nSensorID,pData);
		}
	}
}

void CSensorDataManager::DelSensorData(int nSensorID)
{
	using namespace boost;
	lock_guard<mutex> Lock(m_SensorDataMapLock);

	auto Iter = m_SensorDataMap.find(nSensorID);
	//�����ڲ�����
	if (Iter == m_SensorDataMap.end())
	{
		return;
	}

	if (nullptr != Iter->second)
	{
		delete Iter->second;
		Iter->second = nullptr;
	}

	m_SensorDataMap.erase(Iter);
}

CSensorDataManager::CSensorDataManager(void)
{
	m_SensorDataMap.clear();
}


CSensorDataManager::~CSensorDataManager(void)
{
	BOOST_FOREACH(auto& Element,m_SensorDataMap)
	{
		if (nullptr != Element.second)
		{
			delete Element.second;
			Element.second = nullptr;
		}
	}

	m_SensorDataMap.clear();
}