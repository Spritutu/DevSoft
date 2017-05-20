#ifndef SENSOR_COM_MANAGER_H
#define SENSOR_COM_MANAGER_H
#include <boost/thread/mutex.hpp>
#include <boost/unordered_map.hpp>
class CSerialPortService;
class CSensorComManager
{
public:
	//������ʵ��
	static CSensorComManager& CreateInstance(void);

	//���Ӵ�����ID��Ӧ��ͨ����
	void AddSensorCom(int nSensorID);

	//ɾ��������ID��Ӧ��ͨ����
	void DelSensorCom(int nSensorID);

	//�����豸��ID�Ҷ�Ӧ��ͨ����
	CSerialPortService* QueryComBySensorID(int nSensorID);

	//���д�������ʼ�ɼ�
	void StartSensorCollect();

	void StopSensorCollect();
private:
	CSensorComManager(void);
	~CSensorComManager(void);
private:
	boost::unordered_map<int,CSerialPortService*> m_SensorComMap;
	boost::mutex m_Lock;
private:
	static CSensorComManager s_obj;
};

#endif