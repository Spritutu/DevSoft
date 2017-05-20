// BaseLib.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "SerialPort.h"

//����Ĭ�ϵĽ��ܻ�������С
#define  COM_IN_SIZE       (1024 * 1024)
//����Ĭ�ϵķ��ͻ�������С
#define  COM_OUT_SIZE   (1024 * 1024)
//��ʹ��������
# define   NO_USE_FLOW_CONTROL            (0)
//ʹ��Ӳ��������(RTS/CTS��ʽ)
#define    USE_HARDWARE_FLOW_CONTROL          (1)
//����COM�����ʵ��
CHandleCom::CHandleCom(void):
m_handle(INVALID_HANDLE_VALUE)
{
}

CHandleCom::~CHandleCom(void)
{

}

/*****************************************************************************************************************************************
��������:Open
��������:��COM��
�������:szName:Ҫ�򿪵Ĵ�������
�������:��
����ֵ:true:�ɹ� false:ʧ��
*****************************************************************************************************************************************/
bool CHandleCom::Open(const char* szName)
{
	if (NULL == szName)
	{
		return false;
	}
	//�����жϴ����Ƿ��
	if (IsOpen())
	{
		return true;
	}
	//ͬ���򿪴���
	m_handle = CreateFileA(szName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);

	if (INVALID_HANDLE_VALUE == m_handle)
	{
		return false;
	}
	//���ô��ڵĽ��ܻ���ͷ��ͻ���
	SetupComm(m_handle,COM_IN_SIZE,COM_OUT_SIZE);
	//��մ��ڵĽ��ջ���ͷ��ͻ���
	PurgeComm(m_handle,PURGE_RXCLEAR | PURGE_TXCLEAR);
	return true;

}

/*****************************************************************************************************************************************
��������:Close
��������:�ر�COM��
�������:��
�������:��
����ֵ:true:�ɹ� false:ʧ��
*****************************************************************************************************************************************/
void CHandleCom::Close(void)
{
	if (INVALID_HANDLE_VALUE != m_handle)
	{
		CloseHandle(m_handle);
		m_handle = INVALID_HANDLE_VALUE;
	}
}

/*****************************************************************************************************************************************
��������:Write
��������:�򴮿�д������
�������:pData:Ҫд������� nDataLen:д�����ݵĳ��� 
�������:uWriteLen:ʵ��д������ݵĳ���
����ֵ:true:�ɹ� false:ʧ��
*****************************************************************************************************************************************/
bool CHandleCom::Write(void* pData,unsigned int nDataLen,unsigned int& uWriteLen)
{
	if (NULL == pData || 0 == nDataLen || nDataLen >= COM_OUT_SIZE)
	{
		return false;
	}
	uWriteLen = 0;
	//�жϴ����Ƿ��Ѿ���
	if (IsOpen())
	{
		//ʹ����Ӳ��������
		if (USE_HARDWARE_FLOW_CONTROL == m_comproperty.nFlowControl)
		{
			//�鿴CTS�ĵ�ƽ�ߵ�
			while (1)
			{
				DWORD dwStatus = 0;
				if (GetCommModemStatus(m_handle,&dwStatus))
				{
					//CTSΪ�ߵ�ƽ
					if (dwStatus == MS_CTS_ON)
					{
						break;
					}
					else
					{
						Sleep(100);
						continue;
					}
				}
				//��ȡʧ��
				else
				{
					return false;
				}
			}
		}

		//ʵ��д����ֽ���
		DWORD dwWriteByte = 0;
		if (WriteFile(m_handle,pData,nDataLen,&dwWriteByte,NULL))
		{
			uWriteLen = (unsigned int)dwWriteByte;
			return true;
		}
		//���д��ʧ�ܣ�����false
		return false;
	}
	//�����δ�򿪴���
	return false;
}

/*****************************************************************************************************************************************
��������:Read
��������:�Ӵ��ڶ�ȡ����
�������:pData:��Ŷ�ȡ���ݵĻ����� nDataLen:Ҫ��ȡ���ֽ���
�������:uReadLen:ʵ�ʶ�ȡ�����ݵĳ���
����ֵ:true:�ɹ� false:ʧ��
*****************************************************************************************************************************************/
bool CHandleCom::Read(void* pData,unsigned int nDataLen,unsigned int& uReadLen)
{
	if (NULL == pData || 0 == nDataLen)
	{
		return false;
	}
	uReadLen = 0;
	//�жϴ����Ƿ��Ѿ���
	if (!IsOpen())
	{
		return false;
	}

#ifdef _WIN32

	//��־���Խ���
#if 0
	DWORD dwStatus = SETRTS;
	if (!EscapeCommFunction(m_handle,dwStatus))
	{
		return false;
	}
#endif
	//ʵ�ʶ�����ֽ���
	DWORD dwReadByte = 0;
	if (ReadFile(m_handle,pData,nDataLen,&dwReadByte,NULL))
	{
		uReadLen = (unsigned int)dwReadByte;
		//PurgeComm(m_handle,PURGE_RXCLEAR | PURGE_TXCLEAR);
		return true;
	}
#else
	//��¼ʵ�ʶ�ȡ���ֽ���
	unsigned int nReadByte = 0;
	//���ж��Ĳ���
	nReadByte = read(m_handle,pData,nDataLen);
	//��ȡ�ɹ�
	if (nReadByte != -1)
	{
		uReadLen = nReadByte;
		return true;
	}
#endif
	return false;
}

/*****************************************************************************************************************************************
��������:IsOpen
��������:�ж�COM�Ƿ��Ѿ���
�������:��
�������:��
����ֵ:true:�Ѿ��� false:û�д�
*****************************************************************************************************************************************/
bool CHandleCom::IsOpen(void)
{
	if (INVALID_HANDLE_VALUE == m_handle)
	{
		return false;
	}
	return true;
}

/*****************************************************************************************************************************************
��������:GetComProperty
��������:��ȡ�򿪴��ڵĵ�ǰ����
�������:��
�������:pProperty:���洮�ڵ�����
����ֵ:true:��ȡ�ɹ� false:��ȡʧ��
*****************************************************************************************************************************************/
bool CHandleCom::GetComProperty(LPCOMPROPERTY pProperty)
{
	if (NULL == pProperty)
	{
		return false;
	}

	*pProperty = m_comproperty;
	return true;

}

/****************************************************************************************************************************************
��������:SetComProperty
��������:���ô򿪴��ڵĵ�ǰ����
�������:pProperty:���ô��ڵ�����
�������:��
����ֵ:true:���óɹ� false:����ʧ��
*****************************************************************************************************************************************/
bool CHandleCom::SetComProperty(LPCOMPROPERTY pProperty)
{
	if (NULL == pProperty)
	{
		return false;
	}

	//������ڻ�û�д�
	if (!IsOpen())
	{
		return false;
	}
#ifdef _WIN32
	DCB ComDcb;
	//��ȡ��ǰ���ڵ�����
	if (!GetCommState(m_handle,&ComDcb))
	{
		return false;
	}

	switch (pProperty->nFlowControl)
	{
		//��ʹ������
	case NO_USE_FLOW_CONTROL:
		{
			ComDcb.fOutxCtsFlow = 0;
			ComDcb.fOutxDsrFlow = 0;
			ComDcb.fDsrSensitivity = 0;
			ComDcb.fRtsControl = 0;
			ComDcb.fDtrControl = 0;

			ComDcb.fInX = 0; 
			ComDcb.fOutX = 0;
			ComDcb.XoffChar = 0;
			ComDcb.XonChar = 0;
			ComDcb.XoffLim = 0;
			ComDcb.XonLim = 0;
		}

		break;
		//ʹ��Ӳ������RTS/CTS��ʽ
	case USE_HARDWARE_FLOW_CONTROL:
		{
			ComDcb.fOutxCtsFlow = 1;
			ComDcb.fOutxDsrFlow = 0;
			ComDcb.fDsrSensitivity = 0;
			ComDcb.fRtsControl = RTS_CONTROL_ENABLE;
			ComDcb.fDtrControl = 0;

			ComDcb.fInX = 0; 
			ComDcb.fOutX = 0;
			ComDcb.XoffChar = 0;
			ComDcb.XonChar = 0;
			ComDcb.XoffLim = 0;
			ComDcb.XonLim = 0;
		}
		break;
	default:
		break;
	}
	//���ò�����
	ComDcb.BaudRate = pProperty->nBaudRate;
	//��������λ
	ComDcb.ByteSize = pProperty->nDataBits;
	//����ֹͣλ
	ComDcb.StopBits = pProperty->nStopBits;
	//����У��λ
	ComDcb.Parity = pProperty->nPairty;


	//���ô���
	if (!SetCommState(m_handle,&ComDcb))
	{
		return false;
	}
#else
	//��ȡ���ڵ�ǰ����
	struct termios newtio;
	//struct termios oldtio;
	memset(&newtio,0,sizeof(newtio));
	//memset(&oldtio,0,sizeof(oldtio));
	//��ȡʧ��
	if (tcgetattr(m_handle,&newtio) != 0)
	{
		return false;
	}
	newtio.c_cflag |= (CLOCAL | CREAD);
	newtio.c_cflag &= ~CSIZE;
	//����λ
	switch (pProperty->nDataBits)
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	default:
		break;
	}
	//У��λ
	switch (pProperty->nPairty)
	{
		//û��У��λ
	case 0:
		newtio.c_cflag &= ~PARENB;
		newtio.c_iflag &= ~INPCK;
		break;
		//��У��
	case 1:
		newtio.c_cflag |= (PARODD | PARENB);
		newtio.c_iflag |= INPCK;
		break;
		//żУ��
	case 2:
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		newtio.c_iflag |= INPCK;
		break;
		//�ո�У��
	case 4:
		newtio.c_cflag &= ~PARENB;
		newtio.c_cflag &= ~CSTOPB;
		newtio.c_iflag |= INPCK;
		break;
	default:
		break;
	}
	//ֹͣλ
	switch (pProperty->nPairty)
	{
		//1��ֹͣλ
	case 0:
		newtio.c_cflag &= ~CSTOPB;
		break;
		//2��ֹͣλ
	case 2:
		newtio.c_cflag |= CSTOPB;
		break;
	default:
		break;
	}

	//���ò�����
	cfsetispeed(&newtio,pProperty->nBaudRate);
	cfsetospeed(&newtio,pProperty->nBaudRate);
	//���ô���
	if (tcsetattr(m_handle,TCSANOW,&newtio) != 0)
	{
		return false;
	}
#endif
	m_comproperty = *pProperty;
	return true;
}

/****************************************************************************************************************************************
��������:SetTimeout
��������:���ô��ڵĶ�д����ʱ��
�������:nMillSeconds:��ʱʱ�䣬��λ�Ǻ���
�������:��
����ֵ:true:���óɹ� false:����ʧ��
*****************************************************************************************************************************************/
bool CHandleCom::SetTimeout(unsigned int nMillSeconds)
{  
	//�жϴ����Ƿ��Ѿ���
	if (!IsOpen())
	{
		return false;
	}

#ifdef _WIN32
	COMMTIMEOUTS Timeouts;
	ZeroMemory(&Timeouts,sizeof(COMMTIMEOUTS));
	Timeouts.ReadTotalTimeoutConstant = nMillSeconds;
	Timeouts.WriteTotalTimeoutConstant = nMillSeconds;

	//����ʧ��
	if (!SetCommTimeouts(m_handle,&Timeouts))
	{
		return false;
	}
#else
	//��ȡ���ڵ�ǰ����
	struct termios newtio;
	memset(&newtio,0,sizeof(newtio));
	//��ȡʧ��
	if (tcgetattr(m_handle,&newtio) != 0)
	{
		return false;
	}

	newtio.[VTIME] = nMillSeconds / 100;
	newtio.c_cc[VMIN] = 0;

	if (tcsetattr(m_handle,TCSANOW,&newtio) != 0)
	{
		return false;
	}
#endif
	return true;
}

void CHandleCom::ResetSendBuffer( void )
{
	if (IsOpen())
	{
#ifdef _WIN32
		PurgeComm(m_handle,PURGE_TXCLEAR);
#else
		tcflush(m_handle,TCOFLUSH);
#endif
	}

}

void CHandleCom::ResetRecvBuffer( void )
{
	if (IsOpen())
	{
#ifdef _WIN32
		PurgeComm(m_handle,PURGE_RXCLEAR);
#else
		tcflush(m_handle,TCIFLUSH);
#endif
	}

}
