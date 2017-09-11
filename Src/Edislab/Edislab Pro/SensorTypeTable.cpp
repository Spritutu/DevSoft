/*********************************************************
Copyright(C):
FileName:SensorTypeTable.h
Descripton:Ԥ֧���������ͱ�
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
CSensorTypeTable::CSensorTypeTable(void)
{
	m_SensorTypeMap.clear();
	InitSensorTypeTable();
}


CSensorTypeTable::~CSensorTypeTable(void)
{
	m_SensorTypeMap.clear();
}

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

CSensorTypeTable& CSensorTypeTable::CreateInstance(void)
{
	return s_obj;
}

std::string CSensorTypeTable::QuerySensorNameByID(int nSensorTypeID)
{
	auto Lock = boost::make_unique_lock(m_Lock);

	auto Iter = m_SensorTypeMap.find(nSensorTypeID);

	if (Iter != m_SensorTypeMap.end())
	{
		return Iter->second;
	}

	return "";
}

CSensorTypeTable CSensorTypeTable::s_obj;
