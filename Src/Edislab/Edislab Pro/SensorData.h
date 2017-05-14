/**********************************************************
*��Ȩ����:xiaowei.han
*�ļ�����:SensorData.h
*��������:�������ɼ�������
*����:xiaowei.han
*����:2017/05/14
**********************************************************/
#pragma once
#include <list>
#include <boost/thread/mutex.hpp>
class CSensorData
{
public:
	CSensorData(void);
	~CSensorData(void);

	//�������
	void AddSensorData(float fData);

	//���ñ������ݵ���󳤶�
	void SetDataMaxLength(int nDtatMaxLength)
	{
		m_nMaxDataLength = nDtatMaxLength;
	}

	//���ô�����ID
	void SetSensorID(int nSensorID)
	{
		m_nSensorID = nSensorID;
	}

	//��ȡ����������
	void GetSensorData(std::list<float>& SensorDataList);

private:
	//����ͬ������
	boost::mutex m_DataLock;
	//����
	std::list<float> m_SensorDataList;
	//���ݵ�������
	int m_nMaxDataLength;
	//��Ӧ�������ı��
	int m_nSensorID;
};

