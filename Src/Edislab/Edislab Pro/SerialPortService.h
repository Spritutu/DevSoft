#ifndef SERIAL_PORT_SERVICE_H
#define SERIAL_PORT_SERVICE_H
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/atomic.hpp>
#include "SerialPort.h"
class CSerialPortService
{
public:
	CSerialPortService();
	~CSerialPortService();
	void SetSerialPortOption(int nPort, const COMPROPERTY& Options)
	{
		m_nSerialPort = nPort;
		m_SerialPortOpt = Options;
	}
	//��������
	void StartSerialPortService(void);
	void StopSerialPortService(void);
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
};

#endif