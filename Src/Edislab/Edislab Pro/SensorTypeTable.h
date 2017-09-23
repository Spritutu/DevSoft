/*********************************************************
Copyright(C):
FileName:SensorTypeTable.h
Descripton:Ԥ�ô��������ͱ�
Author:xiaowei.han
Data:2017/09/11
Others:
History:
Version:1.0
*********************************************************/
#pragma once
#include <boost/thread/mutex.hpp>
#include <boost/unordered_map.hpp>
#include <string>
#include <rapidjson/document.h>
class CSensorTypeTable
{

public:
	typedef struct _sensor_type_value
	{
		int nSensorTypeID;
		std::string strSensorTypeName;
		std::string strSensorUnit;

		_sensor_type_value()
		{
			nSensorTypeID = -1;
		}

		_sensor_type_value(int nSensorTypeID,const std::string& strSensorTypeName,const std::string& strSensorUnit)
		{
			this->nSensorTypeID = nSensorTypeID;
			this->strSensorTypeName = strSensorTypeName;
			this->strSensorUnit = strSensorUnit;
		}

	}SENSOR_TYPE_VALUE,* LP_SENSOR_TYPE_VALUE;

public:
	static CSensorTypeTable& CreateInstance(void);
private:
	CSensorTypeTable(void);
	~CSensorTypeTable(void);

public:
	SENSOR_TYPE_VALUE QuerySensorNameByID(int nSensorTypeID);
	//�������ļ����ش���������
	bool LoadSensorTypeListFromFile(void);
	//����Ĭ������
	void LoadDefaultSensorType(void);
private:
	void InitSensorTypeTable(void);
	bool LoadSensorTypeList(rapidjson::Document& Parser);
private:
	boost::mutex m_Lock;

	boost::unordered_map<int,SENSOR_TYPE_VALUE> m_SensorTypeMap;

	static CSensorTypeTable s_obj;
};

