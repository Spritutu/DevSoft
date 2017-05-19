#include "stdafx.h"
#include "SerialPortService.h"
#include <boost/bind.hpp>
#include "Log.h"
#include "SensorIDGenerator.h"
#include "SensorDataManager.h"
#include "SensorData.h"
#include "Utility.h"
#include "Msg.h"
//���ջ������Ĵ�С
const int MAX_BUFFER_SIZE = 100;
const int DEFAULT_TIME_OUT = 1000;
CSerialPortService& CSerialPortService::CreateInstance()
{
	return s_obj;
}

CSerialPortService::CSerialPortService():
	m_bLoop(false),
	m_nSerialPort(-1),
	m_pDeviceNameBuffer(nullptr),
	m_nDeviceNameBufferLength(0),
	m_bCopyDeviceName(true)
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
	if (!m_Com.SetTimeout(DEFAULT_TIME_OUT))
	{
		ERROR_LOG("SetTimeout failed.");
		return;
	}

	//������ȡ�߳�
	if (!m_pReceThread)
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

void CSerialPortService::StartSensorCollect(const std::string& strSensorName)
{
	//if (nullptr == m_pDeviceNameBuffer || 0 == m_nDeviceNameBufferLength)
	//{
	//	return;
	//}

	if (strSensorName.empty())
	{
		return;
	}

	int nSensorNameLength = (int)strSensorName.length();
	int nFrameLength =  1 + 1 + 1 + nSensorNameLength;
	int nTotalLength = nFrameLength + 2;
	BYTE* pData = new BYTE[nTotalLength];
	if (nullptr == pData)
	{
		return;
	}
	pData[0] = 0xAB;
	pData[1] = (BYTE)nFrameLength;
	pData[2] = (BYTE)nSensorNameLength;
	memcpy(pData + 3,strSensorName.c_str(),nSensorNameLength);
	pData[nSensorNameLength + 3] = 0x00;
	pData[nSensorNameLength + 4] = Utility::CalCRC8(pData,nTotalLength - 1);
	unsigned int nWriteBytes = 0;
	if (m_Com.Write(pData,nTotalLength,nWriteBytes))
	{
		if (nWriteBytes != nTotalLength)
		{
			ERROR_LOG("StartSensorCollect [%s] failed.",strSensorName.c_str());
		}
	}
	else
	{
		ERROR_LOG("StartSensorCollect [%s] failed.",strSensorName.c_str());
	}
	DELETE_ARRAY_POINTER(pData);
}

void CSerialPortService::StopSensorCollect(const std::string& strSensorName)
{
	//if (nullptr == m_pDeviceNameBuffer || 0 == m_nDeviceNameBufferLength)
	//{
	//	return;
	//}
	if (strSensorName.empty())
	{
		return;
	}
	int nSensorNameLength = (int)strSensorName.length();
	int nFrameLength =  1 + 1 + 1 + nSensorNameLength;
	int nTotalLength = nFrameLength + 2;
	BYTE* pData = new BYTE[nTotalLength];
	if (nullptr == pData)
	{
		return;
	}
	pData[0] = 0xAB;
	pData[1] = (BYTE)nFrameLength;
	pData[2] = (BYTE)nSensorNameLength;
	memcpy(pData + 3,strSensorName.c_str(),nSensorNameLength);
	pData[nSensorNameLength + 3] = 0x01;
	pData[nSensorNameLength + 4] = Utility::CalCRC8(pData,nTotalLength - 1);
	unsigned int nWriteBytes = 0;
	if (m_Com.Write(pData,nTotalLength,nWriteBytes))
	{
		if (nWriteBytes != nTotalLength)
		{
			ERROR_LOG("StopSensorCollect [%s] failed.",strSensorName.c_str());
		}
	}
	else
	{
		ERROR_LOG("StopSensorCollect [%s] failed.",strSensorName.c_str());
	}
	DELETE_ARRAY_POINTER(pData);
}

void CSerialPortService::SetSensorFrequence(const std::string& strSensorName,int nMillSecond)
{
	if (strSensorName.empty())
	{
		return;
	}

	//���ò�������
	int nSensorNameLength = (int)strSensorName.length();
	int nMsgLength = 6 + nSensorNameLength;

	BYTE* pSendBuffer = new BYTE[nMsgLength];

	if (nullptr == pSendBuffer)
	{
		return;
	}


	ZeroMemory(pSendBuffer,nMsgLength);
	pSendBuffer[0] = 0xAF;
	pSendBuffer[1] = nMsgLength - 2;
	pSendBuffer[2] = nSensorNameLength;
	memcpy(pSendBuffer + 3,strSensorName.c_str(),nSensorNameLength);
	pSendBuffer[nMsgLength - 3] = (BYTE)(nMillSecond & 0x00FF);
	pSendBuffer[nMsgLength - 2] = (BYTE)((nMillSecond & 0xFF00) >> 8);
	pSendBuffer[nMsgLength - 1] = Utility::CalCRC8(pSendBuffer,nMsgLength - 1);


	unsigned int nWriteBytes = 0;
	if (m_Com.Write(pSendBuffer,nMsgLength,nWriteBytes))
	{
		if (nWriteBytes != nMsgLength)
		{
			ERROR_LOG("SetSensorFrequence [%s] failed.",strSensorName.c_str());
		}
	}
	else
	{
		ERROR_LOG("SetSensorFrequence [%s] failed.",strSensorName.c_str());
	}
	DELETE_ARRAY_POINTER(pSendBuffer);
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
							//CopyDeviceName(pData + 3,nDeviceNameLength);
							//����֪ͨ
							if (0x00 == pData[chLength])
							{
								DEBUG_LOG("the device [%s] is off.",szDeviceName);
								//�������Ʋ�ѯ���豸��ID
								int nSensorID = CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(std::string((char*)szDeviceName));
								//�����豸IDɾ���豸������
								if (nSensorID >= 0)
								{
									CSensorDataManager::CreateInstance().DelSensorData(nSensorID);
								}
								//�Ӵ�����ID������ɾ��
								CSensorIDGenerator::CreateInstance().DelSensor(std::string((char*)szDeviceName));
							
								std::string* pDeviceName = new std::string((char*)szDeviceName);
								//֪ͨ�豸����
								::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_NOTIFY_DETECT_DEVICE,(WPARAM)pDeviceName,0);
							}
							//�豸����֪ͨ
							else
							{
								DEBUG_LOG("the device [%s] is on.",szDeviceName);
								//��Ӵ�����
								int nSensorID = CSensorIDGenerator::CreateInstance().AddSensor(std::string((char*)szDeviceName));
								if (nSensorID >= 0)
								{
									//��Ӷ�ӦSensorID������
									CSensorDataManager::CreateInstance().AddSensorData(nSensorID);
								}

								//֪ͨ�豸����
								std::string* pDeviceName = new std::string((char*)szDeviceName);
								//֪ͨ�豸����
								::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_NOTIFY_DETECT_DEVICE,(WPARAM)pDeviceName,1);
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
							//CopyDeviceName(pData + 3,nDeviceNameLength);
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

CSerialPortService CSerialPortService::s_obj;

