#ifndef SERIAL_PORT_SERVICE_H
#define SERIAL_PORT_SERVICE_H
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/atomic.hpp>
#include <boost/function.hpp>
#include <string>
#include "SerialPort.h"

//�豸�������߻ص�����
typedef boost::function<void(const std::string&,bool)> pDeviceCallBack;


class CSerialPortService
{

public:
	static CSerialPortService& CreateInstance();
private:
	CSerialPortService();
	~CSerialPortService();
public:
	void SetSerialPortOption(int nPort, const COMPROPERTY& Options)
	{
		m_nSerialPort = nPort;
		m_SerialPortOpt = Options;
	}
	//��������
	void StartSerialPortService(void);
	void StopSerialPortService(void);

	//��ʼ�ɼ�����
	void StartSensorCollect(const std::string& strSensorName);
	//ֹͣ�ɼ�
	void StopSensorCollect(const std::string& strSensorName);

	//�����ϱ�����
	void SetSensorFrequence(const std::string& strSensorName,int nMillSecond);

	//ע���豸�����߻ص�
	void RegisterDeviceCallBack(pDeviceCallBack DeviceCallback)
	{
		m_pCallBack = DeviceCallback;
	}
private:
	void ReceiveProc(void);

	//�������ߺ�������Ϣ
	void HandleDeviceOnOffMsg(void);

	//�������������Ϣ
	void HandleDeviceDataMsg(void);

	//����CRCУ���
	BYTE CalCRC8(BYTE* pBuf,unsigned int nsize);

	//������������
	void CopyDeviceName(BYTE* pData,int nDataLength);
private:
	//����豸���Ƶ�����
	BYTE* m_pDeviceNameBuffer;

	//����豸�������ݵĳ���
	int m_nDeviceNameBufferLength;

	//���ں�
	int m_nSerialPort;

	//��������
	COMPROPERTY m_SerialPortOpt;

	boost::shared_ptr<boost::thread> m_pReceThread;

	boost::atomic_bool m_bLoop;

	//������
	CHandleCom m_Com;

	//�Ƿ񿽱��豸����
	bool m_bCopyDeviceName;

	static CSerialPortService s_obj;

	//�豸�������߻ص�
	pDeviceCallBack m_pCallBack;
};

#endif