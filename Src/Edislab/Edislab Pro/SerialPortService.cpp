#include "stdafx.h"
#include "SerialPortService.h"
#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "Log.h"
#include "SensorIDGenerator.h"
#include "SensorDataManager.h"
#include "SensorData.h"
#include "Utility.h"
#include "Msg.h"
//���ջ������Ĵ�С(1MB)
const int MAX_BUFFER_SIZE = (1 << 20);
//��Ŵ��������Ƶ������С
const int MAX_SENSOR_NAME_LENGTH = 100;

CSerialPortService& CSerialPortService::CreateInstance()
{
	return s_obj;
}

CSerialPortService::CSerialPortService():
	m_Work(m_IoService),
	m_SerialPort(m_IoService),
	m_nUseBufferBytes(0),
	m_pRecvBuffer(nullptr)
{

	if (nullptr == m_pRecvBuffer)
	{
		m_pRecvBuffer = new BYTE[MAX_BUFFER_SIZE];
	}

}


CSerialPortService::~CSerialPortService()
{
	if (nullptr != m_pRecvBuffer)
	{
		delete[]m_pRecvBuffer;
		m_pRecvBuffer = nullptr;
	}
	m_nUseBufferBytes = 0;
}

void CSerialPortService::StartSerialPortService(void)
{
	//��ʼ������
	if (!InitSerialPort())
	{
		ERROR_LOG("InitSerialPort failed.");
		return;
	}
	//��ʼ�첽��ȡ����
	m_SerialPort.async_read_some(boost::asio::buffer(m_pRecvBuffer + m_nUseBufferBytes, MAX_BUFFER_SIZE - m_nUseBufferBytes), boost::bind(&CSerialPortService::ReadHandler, this, boost::asio::placeholders::error,
		                                              boost::asio::placeholders::bytes_transferred));
	
	boost::system::error_code ec;
	m_IoService.run(ec);
}

void CSerialPortService::StopSerialPortService(void)
{
	m_IoService.stop();
	m_SerialPort.close();
}


void CSerialPortService::StartSensorCollect( const std::string& strSensorName )
{
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
	
	AsyncWriteData(pData,nTotalLength);
}

void CSerialPortService::StopSensorCollect( const std::string& strSensorName )
{
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
	
	AsyncWriteData(pData,nTotalLength);
}

void CSerialPortService::SetSensorFrequence( const std::string& strSensorName,int nMillSecond )
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

	AsyncWriteData(pSendBuffer,nMsgLength);
}

void CSerialPortService::AsyncWriteData(BYTE* pData, int nDataLength)
{
	if (nullptr == pData || 0 == nDataLength)
	{
		return;
	}

	boost::this_thread::sleep(boost::posix_time::seconds(1));
	m_SerialPort.async_write_some(boost::asio::buffer(pData,nDataLength),boost::bind(&CSerialPortService::WriteHandler,this,pData,nDataLength, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

//����֪ͨ��ɻص�
void CSerialPortService::ReadHandler(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
	if (ec)
	{
		return;
	}
	//�����Ƿ����������ݰ�
	int nRecvBytes = static_cast<int>(bytes_transferred);
	m_nUseBufferBytes += nRecvBytes;
	//��������
	int nHandledBytes = HandlerData(m_pRecvBuffer,m_nUseBufferBytes);
	//�ƶ����ݿ�
	memmove(m_pRecvBuffer, m_pRecvBuffer + nHandledBytes, (m_nUseBufferBytes - nHandledBytes));
	m_nUseBufferBytes -= nHandledBytes;
	m_SerialPort.async_read_some(boost::asio::buffer(m_pRecvBuffer + m_nUseBufferBytes, MAX_BUFFER_SIZE - m_nUseBufferBytes), boost::bind(&CSerialPortService::ReadHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

/**************************************************************************
@FunctionName:HandlerData
@FunctionDestription:��������
@InputParam:
@OutPutParam:
@ReturnValue:�����Ѿ�������ֽ���
**************************************************************************/
int CSerialPortService::HandlerData(BYTE* pData, int nDataLength)
{
	int nHandledBytes = 0;
	if (nullptr == pData || 0 == nDataLength)
	{
		return 0;
	}
	char szSensorName[MAX_SENSOR_NAME_LENGTH] = { 0 };
	int nIndex = 0;

	while (nIndex < nDataLength)
	{	
		//������������֪ͨ
		if (pData[nIndex] == 0xBA)
		{
			//ȷ�����鲻Խ��
			if (nIndex + 1 < nDataLength)
			{
				//��ȡ���ݳ���
				int nFrameLength = (int)pData[nIndex + 1];
				//�ж���������
				if ((nIndex + nFrameLength + 1) < nDataLength)
				{
					if (Utility::CalCRC8(&pData[nIndex], nFrameLength + 1) == pData[nIndex + nFrameLength + 1])
					{
						int nSensorNameLength = (int)pData[nIndex + 1 + 1];
						//��ȡ����������
						memcpy(szSensorName, &pData[nIndex + 1 + 1 + 1], nSensorNameLength);
						//����������
						if (pData[nIndex + 1 + 1 + 1 + nSensorNameLength] == 0x01)
						{
							DEBUG_LOG("the device [%s] is on.",szSensorName);

							//�жϴ������Ƿ��Ѿ�����
							if (!CSensorIDGenerator::CreateInstance().IsSensorExist(szSensorName))
							{
								//��Ӵ�����
								int nSensorID = CSensorIDGenerator::CreateInstance().AddSensor(szSensorName);
								if (nSensorID >= 0)
								{
									//��Ӷ�ӦSensorID������
									CSensorDataManager::CreateInstance().AddSensorData(nSensorID);
								}

								//֪ͨ�豸����
								std::string* pDeviceName = new std::string(szSensorName);
								//֪ͨ�豸����
								::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_NOTIFY_DETECT_DEVICE,(WPARAM)pDeviceName,1);
							}
						}
						//����������
						else
						{
							DEBUG_LOG("the device [%s] is off.",szSensorName);
							//�������Ʋ�ѯ���豸��ID
							int nSensorID = CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(szSensorName);
							//�����豸IDɾ���豸������
							if (nSensorID >= 0)
							{
								CSensorDataManager::CreateInstance().DelSensorData(nSensorID);
							}
							//�Ӵ�����ID������ɾ��
							CSensorIDGenerator::CreateInstance().DelSensor(szSensorName);

							std::string* pDeviceName = new std::string(szSensorName);
							//֪ͨ�豸����
							::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_NOTIFY_DETECT_DEVICE,(WPARAM)pDeviceName,0);
						}
						nIndex += (nFrameLength + 2);
						//�޸����ݵĴ���
						nHandledBytes += (nFrameLength + 2);
						continue;
					}
				}
			}
		}

		//�������ϱ�����֪ͨ
		else if (pData[nIndex] == 0xBD)
		{
			//���������ϱ�֪ͨ
			if (nIndex + 1 < nDataLength)
			{
				//��ȡ���ݳ���
				int nFrameLength = (int)pData[nIndex + 1];
				//�ж���������
				if ((nIndex + nFrameLength + 1) < nDataLength)
				{
					if (Utility::CalCRC8(&pData[nIndex], nFrameLength + 1) == pData[nIndex + nFrameLength + 1])
					{
						int nSensorNameLength = (int)pData[nIndex + 1 + 1];
						//��ȡ����������
						memcpy(szSensorName, &pData[nIndex + 1 + 1 + 1], nSensorNameLength);
						float fValue = 0.0f;
						memcpy(&fValue, &pData[nIndex + 1 + 1 + 1 + nSensorNameLength], 4);
						nIndex += (nFrameLength + 2);
						//�޸����ݵĴ���
						nHandledBytes += (nFrameLength + 2);
						
						//��������
						if (fValue > 0)
						{
							NECESSARY_LOG("the device [%s] data is [%.2f].",szSensorName,fValue);
							//���ݴ��������ƻ�ȡID
							int nSensorID = CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(szSensorName);
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
						//boost::posix_time::ptime p = boost::posix_time::second_clock::local_time();
						//std::cout << "["<<p<<"] "<<"the sensor [" << szSensorName << "] value is [" << fValue << "]" << std::endl;
						continue;
					}
				}
			}
		}
		else
		{		
		}
		++nIndex;
	}
	return nHandledBytes;
}

void CSerialPortService::WriteHandler(BYTE* pData, int nDataLength, const boost::system::error_code& ec, std::size_t bytes_transferred)
{
	using namespace std;
	if (ec)
	{
		return;
	}

	if (nDataLength != bytes_transferred)
	{
		ERROR_LOG("WriteHandler failed.");
		return;
	}

	DELETE_ARRAY_POINTER(pData);
}

bool CSerialPortService::InitSerialPort(void)
{

	boost::system::error_code ec;
	m_SerialPort.open(m_SerialOption.strSerialPort, ec);
	if (ec)
	{
		return false;
	}
	//������
	m_SerialPort.set_option(boost::asio::serial_port::baud_rate(m_SerialOption.nBaudRate),ec);    
	if (ec)
	{
		return false;
	}

	if (m_SerialOption.bFlowControl)
	{
		m_SerialPort.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::hardware));  //������
	}
	else
	{
		m_SerialPort.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));  //������
	}
	if (ec)
	{
		return false;
	}

	switch (m_SerialOption.nPairty)
	{
		//��ʹ��У��
	case 0:
		m_SerialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none),ec); //��żУ��
		break;
		//����У��
	case 1:
		m_SerialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::odd), ec);
		break;
		//ż��У��
	case 2:
		m_SerialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::even), ec);
		break;
	default:
		m_SerialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none), ec);
		break;
	}
	if (ec)
	{
		return false;
	}

	switch (m_SerialOption.nStopBits)
	{
	case 0:
		m_SerialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one),ec); //ֹͣλ
		break;
	case 1:
		m_SerialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::onepointfive),ec); //ֹͣλ
		break;
	case 2:
		m_SerialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::two),ec); //ֹͣλ
		break;
	default:
		m_SerialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one), ec);
		break;
	}

	if (ec)
	{
		return false;
	}

	m_SerialPort.set_option(boost::asio::serial_port::character_size(m_SerialOption.nDataBits),ec);// ͨ���ֽ�λ����4��8
	if (ec)
	{
		return false;
	}
	return true;
}

CSerialPortService CSerialPortService::s_obj;

