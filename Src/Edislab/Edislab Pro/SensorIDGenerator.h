#pragma once
#include <vector>
#include <string>
#include <boost/thread/mutex.hpp>
#include "Type.h"
class CSensorIDGenerator
{
public:
	//������ʵ��
	static CSensorIDGenerator& CreateInstance(void);

	//�����豸���Ʋ�ѯ�豸���
	int QuerySensorTypeIDByName(const std::string& strSensorName);
	std::string QueryrNameBySensorID(int id);

	//�����豸
	int AddSensor(const std::string& strSensorName);

	//ɾ���豸
	void DelSensor(const std::string& strSensorName);

	//��ȡ���еĴ���������
	void GetAllSensorName(std::vector<std::string>& SensorNameArray);

	//�жϴ����������Ƿ����
	bool IsSensorExist(const std::string& strSensorName);

private:
	CSensorIDGenerator(void);
	virtual ~CSensorIDGenerator(void);
private:
	//Ψһ�ĵ�ʵ��
	static CSensorIDGenerator s_Instance;
	//�豸ID������
	static int s_nTypeIndex;
private:
	//�豸�������豸ID��
	std::vector<SENSOR_TYPE_INFO_ELEMENT> m_SensorTypeArray;
	//��
	boost::mutex m_Lock;
};

