/************************************************************************************
Copyright(C):selp
FileName:SensorConfig.cpp
Description:�����������࣬���д��������ôӸ����ȡ
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
************************************************************************************/
#include "StdAfx.h"
#include "SensorConfig.h"
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/smart_ptr.hpp>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include "Log.h"
#include "Utility.h"
#include "SensorIDGenerator.h"
#include "SensorDataManager.h"
#include "SerialPortService.h"
//���Ƶ�ɾ����
static void DeleteFunction(std::ifstream* pReader)
{
	if (nullptr != pReader)
	{
		if (pReader->is_open())
		{
			pReader->close();
		}

		delete pReader;
		pReader = nullptr;
	}
}

CSensorConfig::CSensorConfig(void)
{
	m_SensorConfigArray.clear();
}


CSensorConfig::~CSensorConfig(void)
{
	m_SensorConfigArray.clear();
}

/*****************************************************************************
* @FunctionName : CreateInstance
* @FunctionDestription : ������ʵ��
* @author : xiaowei.han
* @date : 2017/3/25 10:26
* @version : ver 1.0
* @inparam :
* @outparam: 
* @return : 
*****************************************************************************/
CSensorConfig& CSensorConfig::CreateInstance( void )
{
	return s_SensorConfig;
}

/*****************************************************************************
* @FunctionName : LoadSensorConfig
* @FunctionDestription : ���ش�����������Ϣ
* @author : xiaowei.han
* @date : 2017/3/25 10:26
* @version : ver 1.0
* @inparam :
* @outparam: 
* @return : 
*****************************************************************************/
bool CSensorConfig::LoadSensorConfig( void )
{
	// ��Ӵ�����������Ϣ
	SENSOR_TYPE_INFO typeInfo;
	typeInfo.enumType = SENSOR_PHYSICS;
	typeInfo.strTypeName = "����";
	m_SensorTypeInfo.push_back(typeInfo);
	typeInfo.enumType = SENSOR_CHEMISTRY;
	typeInfo.strTypeName = "��ѧ";
	m_SensorTypeInfo.push_back(typeInfo);
	typeInfo.enumType = SENSOR_BIOLOGY;
	typeInfo.strTypeName = "����";
	m_SensorTypeInfo.push_back(typeInfo);
	typeInfo.enumType = SENSOR_ALL;
	typeInfo.strTypeName = "����";
	m_SensorTypeInfo.push_back(typeInfo);

	std::string strConfigPath = Utility::GetExeDirecory() + std::string("\\SensorConfig.json");

	//�ж��ļ��Ƿ����
	boost::filesystem::path ConfigPath(strConfigPath);

	try
	{
		//�ж������ļ��Ƿ����
		if (!boost::filesystem::exists(ConfigPath))
		{
			ERROR_LOG("the [%s] is not exist!",strConfigPath.c_str());
			return false;
		}
		//��ʼ����json�ļ�����
		boost::shared_ptr<std::ifstream> pConfigReader(new std::ifstream(strConfigPath), DeleteFunction);

		if (!pConfigReader)
		{
			ERROR_LOG("open [%s] file failed.", strConfigPath.c_str());
			return false;
		}

		rapidjson::IStreamWrapper JsonReader(*pConfigReader);
		rapidjson::Document Parser;
		Parser.ParseStream(JsonReader);

		//���ش������б�
		if (!LoadSensorList(Parser))
		{
			return false;
		}
		//���ش�����ͨ������
		if (!LoadSensorComList(Parser))
		{
			return false;
		}
	}
	catch (boost::filesystem::filesystem_error& e)
	{
		ERROR_LOG("oh,shit,this is something wrong in filesystem,the error is [%s].",e.what());

		return false;
	}
	return true;
}

/*******************************************************************
*��������:LoadSensorList
*��������:���ش������б�
*�������:
*�������:
*����ֵ:true:succeed false:failed
*����:xiaowei.han
*����:2017/06/10 8:39:02
*******************************************************************/
bool CSensorConfig::LoadSensorList( rapidjson::Document& Parser )
{
	// ��������������
	if (!Parser.HasMember("SensorConfig"))
	{
		return false;
	}

	const rapidjson::Value& ProcessArray = Parser["SensorConfig"];

	if (ProcessArray.IsArray())
	{
		for (auto Iter = ProcessArray.Begin(); Iter != ProcessArray.End(); ++Iter)
		{
			SENSOR_CONFIG_ELEMENT Element;
			if (Iter->IsObject())
			{
				// ������ID
				if (!Iter->HasMember("SensorID") || !(*Iter)["SensorID"].IsInt())
				{
					continue;
				}

				Element.nSensorID = (*Iter)["SensorID"].GetInt();

				// ��������
				if (!Iter->HasMember("SensorName") || !(*Iter)["SensorName"].IsString())
				{
					continue;
				}

				Element.strSensorName = Utility::ConverUTF8ToGB2312((*Iter)["SensorName"].GetString());

				// �Ƿ���ӵ��б�
				if (!Iter->HasMember("AddtoSensorlist") || !(*Iter)["AddtoSensorlist"].IsBool())
				{
					continue;
				}

				Element.bAddtoSensorlist = (*Iter)["AddtoSensorlist"].GetBool();

				// ��������
				if (!Iter->HasMember("TypeName") || !(*Iter)["TypeName"].IsString())
				{
					continue;
				}

				Element.strSensorTypeName =  Utility::ConverUTF8ToGB2312((*Iter)["TypeName"].GetString());

				// �Ƿ�����
				if (!Iter->HasMember("MultiSample") || !(*Iter)["MultiSample"].IsBool())
				{
					continue;
				}

				Element.bMultiSample =  (*Iter)["MultiSample"].GetBool();

				// �������ͺ�
				if (!Iter->HasMember("SensorModelName") || !(*Iter)["SensorModelName"].IsString())
				{
					continue;
				}

				Element.strSensorModelName =  Utility::ConverUTF8ToGB2312((*Iter)["SensorModelName"].GetString());

				// ����������
				if (!Iter->HasMember("Symbol") || !(*Iter)["Symbol"].IsString())
				{
					continue;
				}

				Element.strSensorSymbol =  Utility::ConverUTF8ToGB2312((*Iter)["Symbol"].GetString());

				// ����������
				if (!Iter->HasMember("Description") || !(*Iter)["Description"].IsString())
				{
					continue;
				}

				Element.strSensorDescription =  Utility::ConverUTF8ToGB2312((*Iter)["Description"].GetString());

				// ���������ͣ�����\��ѧ\�������ȫ������
				if (!Iter->HasMember("SensorCategory") || !(*Iter)["SensorCategory"].IsInt())
				{
					continue;
				}

				Element.nSensorCategory =  (*Iter)["SensorCategory"].GetInt();

				// ������Ϣ
				if(!Iter->HasMember("RangeInfo") || !(*Iter)["RangeInfo"].IsArray())
				{
					continue;
				}

				for(auto rangeIter = (*Iter)["RangeInfo"].Begin(); rangeIter != (*Iter)["RangeInfo"].End() ; ++rangeIter)
				{
					SENSOR_RANGE_INFO_ELEMENT RangeInfo;
					// ������
					if (!rangeIter->HasMember("Name") || !(*rangeIter)["Name"].IsString())
					{
						continue;
					}

					std::string strRangeName = (*rangeIter)["Name"].GetString();
					RangeInfo.strRangeName =  Utility::ConverUTF8ToASCII(strRangeName);

					// У׼ֵ
					if (!rangeIter->HasMember("Calibrationvalue") || !(*rangeIter)["Calibrationvalue"].IsString())
					{
						continue;
					}

					RangeInfo.strCalibrationvalue =  (*rangeIter)["Calibrationvalue"].IsString();

					// Ĭ��ֵ
					if (!rangeIter->HasMember("Defaultvalue") || !(*rangeIter)["Defaultvalue"].IsInt())
					{
						continue;
					}

					RangeInfo.nDefaultvalue = (*rangeIter)["Defaultvalue"].GetInt();

					// ��Сֵ
					if (!rangeIter->HasMember("Minvalue") || !(*rangeIter)["Minvalue"].IsInt())
					{
						continue;
					}

					RangeInfo.nMinvalue = (*rangeIter)["Minvalue"].GetInt();

					// ���ֵ
					if (!rangeIter->HasMember("Maxvalue") || !(*rangeIter)["Maxvalue"].IsInt())
					{
						continue;
					}

					RangeInfo.nMaxvalue = (*rangeIter)["Maxvalue"].GetInt();

					// monitor
					if (!rangeIter->HasMember("Monitor") || !(*rangeIter)["Monitor"].IsString())
					{
						continue;
					}

					RangeInfo.strMonitorvalue = Utility::ConverUTF8ToGB2312((*rangeIter)["Monitor"].GetString());

					// ����
					if (!rangeIter->HasMember("Accuracy") || !(*rangeIter)["Accuracy"].IsInt())
					{
						continue;
					}

					RangeInfo.nAccuracy = (*rangeIter)["Accuracy"].GetInt();

					// ��λ
					if (!rangeIter->HasMember("Unit") || !(*rangeIter)["Unit"].IsString())
					{
						continue;
					}

					RangeInfo.strUnitName = Utility::ConverUTF8ToGB2312((*rangeIter)["Unit"].GetString());

					// ת����ʽ
					if (!rangeIter->HasMember("Converformula") || !(*rangeIter)["Converformula"].IsString())
					{
						continue;
					}

					RangeInfo.strConverformula = Utility::ConverUTF8ToGB2312((*rangeIter)["Converformula"].GetString());

					Element.SensorRangeInfoArray.push_back(RangeInfo);
				}

				// Ĭ��Ƶ��
				if (!Iter->HasMember("Defaultfrequency") || !(*Iter)["Defaultfrequency"].IsFloat())
				{
					continue;
				}

				Element.fSensorDefaultfrequency =  (*Iter)["SensorCategory"].GetFloat();

				// ��СƵ��
				if (!Iter->HasMember("Minfrequency") || !(*Iter)["Minfrequency"].IsFloat())
				{
					continue;
				}

				Element.nSensorMinfrequency =  (*Iter)["Minfrequency"].GetFloat();

				// ���Ƶ��
				if (!Iter->HasMember("Maxfrequency") || !(*Iter)["Maxfrequency"].IsFloat())
				{
					continue;
				}

				Element.nSensorMaxfrequency =  (*Iter)["Maxfrequency"].GetFloat();

				// ʱ��
				if (!Iter->HasMember("Timelength") || !(*Iter)["Timelength"].IsInt())
				{
					continue;
				}

				Element.nSensorTimeLength =  (*Iter)["Timelength"].GetInt();

				m_SensorConfigArray.push_back(Element);
			}
		}
	}

	return true;
}

/*******************************************************************
*��������:LoadSensorComList
*��������:���ش�����COMͨ������
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/10 8:39:02
*******************************************************************/
bool CSensorConfig::LoadSensorComList( rapidjson::Document& Parser )
{

	if (!Parser.HasMember("ComConfig"))
	{
		return false;
	}
	const rapidjson::Value& ComArray = Parser["ComConfig"];

	SENSOR_COM_CONFIG_ELEMENT ComElement;
	if (ComArray.IsObject())
	{
		// �˿ں�
		if (!ComArray.HasMember("PortName") || !ComArray["PortName"].IsString())
		{
			return false;
		}

		ComElement.strSerialPort = ComArray["PortName"].GetString();

		// У������
		if (!ComArray.HasMember("Pairty") || !ComArray["Pairty"].IsInt())
		{
			return false;
		}

		ComElement.nPairty = ComArray["Pairty"].GetInt();

		// ֹͣλ
		if (!ComArray.HasMember("StopBits") || !ComArray["StopBits"].IsInt())
		{
			return false;
		}

		ComElement.nStopBits = ComArray["StopBits"].GetInt();

		// ͨ���ֽ�λ��
		if (!ComArray.HasMember("DataBits") || !ComArray["DataBits"].IsInt())
		{
			return false;
		}

		ComElement.nDataBits = ComArray["DataBits"].GetInt();

		// ������
		if (!ComArray.HasMember("BaudRate") || !ComArray["BaudRate"].IsInt())
		{
			return false;
		}

		ComElement.nBaudRate = ComArray["BaudRate"].GetInt();

		// �Ƿ�ʹ������
		if (!ComArray.HasMember("UseFlowControl") || !ComArray["UseFlowControl"].IsBool())
		{
			return false;
		}
		ComElement.bUseFlowControl = ComArray["UseFlowControl"].GetBool();

		m_SensorComConfig = ComElement;
	}
	return true;
}

/*******************************************************************
*��������:GetSensorList
*��������:��ȡ��������Ϣ
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/10 8:39:02
*******************************************************************/
void CSensorConfig::GetSensorList(std::vector<SENSOR_CONFIG_ELEMENT> &vecSensorArry, SENSOR_TYPE enumType)
{
      BOOST_FOREACH(auto &sensor, m_SensorConfigArray)
	  {
	       if (sensor.nSensorCategory & enumType)
	       {
			   vecSensorArry.push_back(sensor);
	       }
	  }
}

/*******************************************************************
*��������:GetSensorInfo
*��������:���ݴ�����ID��ȡ��������Ϣ
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/10 8:39:02
*******************************************************************/
SENSOR_CONFIG_ELEMENT CSensorConfig::GetSensorInfo(int nSensorID)
{
	BOOST_FOREACH(auto &sensor, m_SensorConfigArray)
	{
		if (nSensorID == sensor.nSensorID)
		{
			return sensor;
		}
	}

	return SENSOR_CONFIG_ELEMENT();
}

/*******************************************************************
*��������:GetSensorRecordInfo
*��������:��ȡ������������Ϣ
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/10 8:39:02
*******************************************************************/
const SENSOR_RECORD_INFO& CSensorConfig::GetSensorRecordInfo()
{
	return m_SensorRecordInfo;
}

/*******************************************************************
*��������:SetSensorRecordInfo
*��������:���ô�����������Ϣ
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/10 8:39:02
*******************************************************************/
void CSensorConfig::SetSensorRecordInfo(SENSOR_RECORD_INFO& _recordInfo)
{
	m_SensorRecordInfo = _recordInfo;
}






CSensorConfig CSensorConfig::s_SensorConfig;
