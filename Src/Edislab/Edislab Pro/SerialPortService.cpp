#include "stdafx.h"
#include "SerialPortService.h"
#include <boost/bind.hpp>
#include "Log.h"
#include "SensorIDGenerator.h"
#include "SensorDataManager.h"
#include "SensorData.h"
#include "Utility.h"
//���ջ������Ĵ�С
const int MAX_BUFFER_SIZE = 100;
CSerialPortService::CSerialPortService():
	m_bLoop(false),
	m_nSerialPort(-1),
	m_pDeviceNameBuffer(nullptr),
	m_nDeviceNameBufferLength(0),
	m_bCopyDeviceName(false)
{
}


CSerialPortService::~CSerialPortService()
{
	DELETE_ARRAY_POINTER(m_pDeviceNameBuffer);
	m_nDeviceNameBufferLength = 0;
}

//�������ڷ���
void CSerialPortService::StartSerialPortService(void)
{
	char szCom[MAX_BUFFER_SIZE] = { 0 };

	sprintf_s(szCom, "\\\\.\\COM%d", m_nSerialPort);

	//�򿪴���
	if (!m_Com.Open(szCom))
	{
		ERROR_LOG("open com failed.");
		return;
	}

	//����ͨ��ѡ��
	if (!m_Com.SetComProperty(&m_SerialPortOpt))
	{
		ERROR_LOG("SetComProperty failed.");
		return;
	}

	//���ö�ȡ�ĳ�ʱʱ��
	if (!m_Com.SetTimeout(1000))
	{
		ERROR_LOG("SetTimeout failed.");
		return;
	}

	//������ȡ�߳�
	if (m_pReceThread)
	{
		m_bLoop = true;
		//��ʼͨ��
		m_pReceThread = boost::make_shared<boost::thread>(boost::bind(&CSerialPortService::ReceiveProc,this));
	}
}

//ֹͣ���ڷ���
void CSerialPortService::StopSerialPortService(void)
{
	m_bLoop = false;
	if (m_pReceThread)
	{
		m_pReceThread->join();
	}
	m_Com.Close();
}

//���������߳�
void CSerialPortService::ReceiveProc(void)
{
	BYTE chHead = 0;
	unsigned int nReadByte = 0;
	while (m_bLoop)
	{
		//��ȡͷ
		if (m_Com.Read(&chHead,sizeof(chHead),nReadByte))
		{
			if (nReadByte == sizeof(chHead))
			{
				switch (chHead)
				{
					//���߻�������
				case 0xBA:
					HandleDeviceOnOffMsg();
					break;
					//�����ϱ�����
				case 0xBD:
					HandleDeviceDataMsg();
					break;
				default:
					break;
				}
			}	
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::seconds(1));
		}
	}
}

void CSerialPortService::HandleDeviceOnOffMsg(void)
{
	BYTE szDeviceName[MAX_BUFFER_SIZE] = { 0 };
	//�ٽ���һ���ֽ�
	BYTE chLength = 0;
	unsigned int nReadByte = 0;
	if (m_Com.Read(&chLength, sizeof(chLength), nReadByte))
	{
		if (nReadByte == sizeof(chLength))
		{
			//����ʣ�µ�����
			BYTE* pData = new BYTE[chLength + 2];
			if (nullptr != pData)
			{
				pData[0] = 0xBA;
				pData[1] = chLength;
				if (m_Com.Read(pData + 2, chLength, nReadByte))
				{
					if (chLength == nReadByte)
					{
						//�ж��Ƿ����CRC
						if (CalCRC8(pData, chLength + 1) == pData[chLength + 1])
						{
							//��ȡ���������Ƴ���
							int nDeviceNameLength = pData[2];
							memcpy(szDeviceName, pData + 3, nDeviceNameLength);
							//������������
							CopyDeviceName(pData + 3,nDeviceNameLength);
							//����֪ͨ
							if (0x00 == pData[chLength])
							{
								NECESSARY_LOG("the device [%s] is off.",szDeviceName);
								//�Ӵ�����ID������ɾ��
								CSensorIDGenerator::CreateInstance().DelSensor(std::string((char*)szDeviceName));
							}
							//����֪ͨ
							else
							{
								NECESSARY_LOG("the device [%s] is on.",szDeviceName);
							}
						}
					}
				}
				delete pData;
				pData = nullptr;
			}
		}
	}
}

void CSerialPortService::HandleDeviceDataMsg(void)
{
	BYTE szDeviceName[MAX_BUFFER_SIZE] = { 0 };
	//�ٽ���һ���ֽ�
	BYTE chLength = 0;
	unsigned int nReadByte = 0;
	if (m_Com.Read(&chLength, sizeof(chLength), nReadByte))
	{
		if (nReadByte == sizeof(chLength))
		{
			//����ʣ�µ�����
			BYTE* pData = new BYTE[chLength + 2];
			if (nullptr != pData)
			{
				pData[0] = 0xBD;
				pData[1] = chLength;
				if (m_Com.Read(pData + 2, chLength, nReadByte))
				{
					if (chLength == nReadByte)
					{
						//�ж��Ƿ����CRC
						if (CalCRC8(pData, chLength + 1) == pData[chLength + 1])
						{
							//��ȡ���������Ƴ���
							int nDeviceNameLength = pData[2];
							memcpy(szDeviceName, pData + 3, nDeviceNameLength);
							//������������
							CopyDeviceName(pData + 3,nDeviceNameLength);
							//��ȡ����
							float fValue = 0.0f;
							memcpy(&fValue, pData + 3 + nDeviceNameLength, 4);
							if (fValue > 0)
							{
								NECESSARY_LOG("the device [%s] data is [%.1f].",szDeviceName,fValue);
								//���ݴ��������ƻ�ȡID
								int nSensorID = CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(std::string((char*)szDeviceName));
								if (nSensorID >= 0)
								{
									//����ID��ȡ����
									CSensorData* pSensorData = CSensorDataManager::CreateInstance().GetSensorDataBySensorID(nSensorID);
									if (nullptr != pSensorData)
									{
										pSensorData->AddSensorData(fValue);
									}
								}
							}			
						}
					}
				}
				delete pData;
				pData = nullptr;
			}
		}
	}


}

BYTE CSerialPortService::CalCRC8( BYTE* pBuf,unsigned int nsize )
{
	BYTE crc = 0;

	if (nullptr == pBuf || 0 == nsize)
	{
		return crc;
	}

	while (nsize--)
	{
		crc ^= *pBuf;
		pBuf++;
	}
	return crc;
}

void CSerialPortService::CopyDeviceName( BYTE* pData,int nDataLength )
{
	if (nullptr == pData || 0 == nDataLength)
	{
		return;
	}

	if (m_bCopyDeviceName)
	{
		DELETE_ARRAY_POINTER(m_pDeviceNameBuffer);
		m_nDeviceNameBufferLength = 0;
		m_pDeviceNameBuffer = new BYTE[nDataLength];
		if (nullptr != m_pDeviceNameBuffer)
		{
			memcpy(m_pDeviceNameBuffer,pData,nDataLength);
			m_nDeviceNameBufferLength = nDataLength;
			m_bCopyDeviceName = false;
		}
	}
}

