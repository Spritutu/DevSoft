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
	//���ջ������Ĵ�С
	enum {MAX_BUFFER_SIZE = 1024};

private:
	void ReceiveProc(void);

	//�������ߺ�������Ϣ
	void HandleDeviceOnOffMsg(void);

	//�������������Ϣ
	void HandleDeviceDataMsg(void);

	//����CRCУ���
	BYTE CalCRC8(BYTE* pBuf,unsigned int nsize);
private:
	//Ĭ�ϵĽ��ջ�������С
	int m_nRecvBufferSize;

	//�������Ѿ����յ����ݳ���
	int m_nRecvDataLength;

	//���ջ�����
	BYTE m_RecvBuffer[MAX_BUFFER_SIZE];

	//���ں�
	int m_nSerialPort;

	//��������
	COMPROPERTY m_SerialPortOpt;

	boost::shared_ptr<boost::thread> m_pReceThread;

	boost::atomic_bool m_bLoop;

	//������
	CHandleCom m_Com;
};

#endif