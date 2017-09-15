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
#include "StdAfx.h"
#include "SensorTypeTable.h"
#include <boost/thread/lock_factories.hpp>
#include <boost/assign.hpp>
#include <string>
#include <boost/scope_exit.hpp>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/encodedstream.h>
#include <boost/filesystem.hpp>
#include "Utility.h"
#include "Log.h"
CSensorTypeTable::CSensorTypeTable(void)
{
	m_SensorTypeMap.clear();
	//InitSensorTypeTable();
}


CSensorTypeTable::~CSensorTypeTable(void)
{
	m_SensorTypeMap.clear();
}

#if 0
void CSensorTypeTable::InitSensorTypeTable(void)
{
	int nSensorID = 0;
	std::string strSensorName = "����������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);
	
	nSensorID = 1;
	strSensorName = "΢��������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 2;
	strSensorName = "��ѹʽ���ӳ�";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 3;
	strSensorName = "��ѹ��������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 4;
	strSensorName = "����Ŵ�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 5;
	strSensorName = "���񶯴�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 6;
	strSensorName = "���նȴ�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 7;
	strSensorName = "����������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 8;
	strSensorName = "��ѹ������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 9;
	strSensorName = "΢����������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 10;
	strSensorName = "��ͨ�¶ȴ�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 11;
	strSensorName = "�Ÿ�Ӧǿ�ȴ�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 12;
	strSensorName = "����ѹǿ������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 13;
	strSensorName = "G-M������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 14;
	strSensorName = "λ�ƣ����壩";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 15;
	strSensorName = "��ǿ�ȷֲ�������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 16;
	strSensorName = "Զ�̲�ഫ����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 17;
	strSensorName = "���ٶȴ�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 18;
	strSensorName = "���´�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 19;
	strSensorName = "����������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 20;
	strSensorName = "�����ʴ�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 21;
	strSensorName = "PHֵ������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);
	
	
	nSensorID = 22;
	strSensorName = "������ԭ";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 23;
	strSensorName = "�����Ӵ�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 24;
	strSensorName = "�����������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 25;
	strSensorName = "����������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 26;
	strSensorName = "�����Ӵ�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);


	nSensorID = 27;
	strSensorName = "�絼�ʴ�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 28;
	strSensorName = "���ζȴ�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 29;
	strSensorName = "�ܽ���������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 30;
	strSensorName = "������̼������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 31;
	strSensorName = "�ƾ�������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 32;
	strSensorName = "���ʴ�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 33;
	strSensorName = "�ĵ�ͼ������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 34;
	strSensorName = "ʪ�ȴ�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);
	
	nSensorID = 35;
	strSensorName = "ɫ�ȼ�";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);


	nSensorID = 36;
	strSensorName = "�Ƕȼ�";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 37;
	strSensorName = "����/���տ��ƴ�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 38;
	strSensorName = "�ȷ��䴫����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 39;
	strSensorName = "��������/����/һ����̼������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 40;
	strSensorName = "�Ƕȴ�����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 41;
	strSensorName = "����/���鴫����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 42;
	strSensorName = "΢��ѹ������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 43;
	strSensorName = "�����̵���������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 44;
	strSensorName = "�����̵�ѹ������";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 45;
	strSensorName = "����/��ѹ���崫����";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);
}
#endif

CSensorTypeTable& CSensorTypeTable::CreateInstance(void)
{
	return s_obj;
}


CSensorTypeTable::SENSOR_TYPE_VALUE CSensorTypeTable::QuerySensorNameByID(int nSensorTypeID)
{
	SENSOR_TYPE_VALUE Result;

	auto Lock = boost::make_unique_lock(m_Lock);

	auto Iter = m_SensorTypeMap.find(nSensorTypeID);

	if (Iter != m_SensorTypeMap.end())
	{
		Result = Iter->second;
	}

	return Result;
}

bool CSensorTypeTable::LoadSensorTypeListFromFile(void)
{
	using namespace std;
	using namespace rapidjson;
	std::string strConfigPath = Utility::GetExeDirecory() + std::string("\\SensorType.json");

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
		FILE* fp = fopen(strConfigPath.c_str(),"rb");

		if (nullptr == fp)
		{
			ERROR_LOG("open [%s] file failed.", strConfigPath.c_str());
			return false;
		}
		BOOST_SCOPE_EXIT(&fp)
		{
			if (nullptr != fp)
			{
				fclose(fp);
				fp = nullptr;
			}

		}BOOST_SCOPE_EXIT_END
		//��ȡ�ļ��ĳ���
		fseek(fp,0,SEEK_END);
		int nFileLength = (int)ftell(fp);
		if (nFileLength < 0)
		{
			ERROR_LOG("get [%s] file length failed.", strConfigPath.c_str());
			return false;
		}
		//�����ڴ�
		char* pBuffer =  new char[nFileLength];
		if (nullptr == pBuffer)
		{
			ERROR_LOG("Allocate memory failed.");
			return false;
		}
		BOOST_SCOPE_EXIT(&pBuffer)
		{
			if (nullptr != pBuffer)
			{
				delete []pBuffer;
			}
		}BOOST_SCOPE_EXIT_END
		ZeroMemory(pBuffer,nFileLength);
		fseek(fp,0,SEEK_SET);
		FileReadStream bis(fp,pBuffer,nFileLength);
		AutoUTFInputStream<unsigned, FileReadStream> eis(bis);
		Document Parser;      
		Parser.ParseStream<0, AutoUTF<unsigned> >(eis);
		if (Parser.HasParseError())  
		{  
			ERROR_LOG("Parse json file [%s] failed,the error [%d].", strConfigPath.c_str(),Parser.GetParseError());
			return false;
		}  

		//���ش����������б�
		if (!LoadSensorTypeList(Parser))
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

bool CSensorTypeTable::LoadSensorTypeList(rapidjson::Document& Parser)
{
	const rapidjson::Value& SensorTypeArray = Parser["SensorTypeList"];

	if (!SensorTypeArray.IsArray())
	{
		return false;
	}


	for (auto Iter = SensorTypeArray.Begin(); Iter != SensorTypeArray.End(); ++Iter)
	{
		if (Iter->IsObject())
		{
			SENSOR_TYPE_VALUE SensorElement;
			// ������ID
			if (!Iter->HasMember("SensorTypeID") || !(*Iter)["SensorTypeID"].IsInt())
			{
				continue;
			}

			SensorElement.nSensorTypeID = (*Iter)["SensorTypeID"].GetInt();

			// ��������
			if (!Iter->HasMember("SensorTypeName") || !(*Iter)["SensorTypeName"].IsString())
			{
				continue;
			}
			SensorElement.strSensorTypeName = Utility::ConverUTF8ToGB2312((*Iter)["SensorTypeName"].GetString());

			// ��������λ
			if (!Iter->HasMember("SensorTypeUnit") || !(*Iter)["SensorTypeUnit"].IsString())
			{
				continue;
			}
			SensorElement.strSensorUnit = (*Iter)["SensorTypeUnit"].GetString();

			m_SensorTypeMap.emplace(SensorElement.nSensorTypeID,SensorElement);
		}
	}
	return true;
}

CSensorTypeTable CSensorTypeTable::s_obj;
