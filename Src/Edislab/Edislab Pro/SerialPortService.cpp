#include "stdafx.h"
#include "SerialPortService.h"
#include <boost/bind.hpp>
#include "Log.h"
#include "SensorIDGenerator.h"
#include "SensorDataManager.h"
#include "SensorData.h"
CSerialPortService::CSerialPortService():
	m_bLoop(false),
	m_nSerialPort(-1)
{
}


CSerialPortService::~CSerialPortService()
{
}

//�������ڷ���
void CSerialPortService::StartSerialPortService(void)
{
	char szCom[100] = { 0 };

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
	static bool bSample = false;
	BYTE szDeviceName[100] = { 0 };
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
							if (0x00 == pData[chLength])
							{
								NECESSARY_LOG("the device [%s] is off.",szDeviceName);
								//�Ӵ�����ID������ɾ��
								CSensorIDGenerator::CreateInstance().DelSensor(std::string((char*)szDeviceName));
							}
							else
							{
								NECESSARY_LOG("the device [%s] is on.",szDeviceName);
								//��ʼ�ɼ�
								if (!bSample)
								{ 
									BYTE Bffer[18] = { 0 };
									Bffer[0] = 0xAB;
									Bffer[1] = 0x10;
									Bffer[2] = 0x0D;
									Bffer[3] = 0xB4;
									Bffer[4] = 0xF3;
									Bffer[5] = 0xC6;
									Bffer[6] = 0xF8;
									Bffer[7] = 0xD1;
									Bffer[8] = 0xB9;
									Bffer[9] = 0xC7;
									Bffer[10] = 0xBF;
									Bffer[11] = 0x5B;
									Bffer[12] = 0x4B;
									Bffer[13] = 0x70;
									Bffer[14] = 0x61;
									Bffer[15] = 0x5D;
									Bffer[16] = 0x00;
									Bffer[17] = 0x83;
									
									unsigned int nWriteLength = 0;
									m_Com.Write(Bffer, sizeof(Bffer), nWriteLength);

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

void CSerialPortService::HandleDeviceDataMsg(void)
{
	BYTE szDeviceName[100] = { 0 };
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

