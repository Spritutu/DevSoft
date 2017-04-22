/*************************************************************************************************
*��Ȩ����:
*�ļ���:ComImple.cpp
*����:����ͨ�Žӿ���ʵ��
*����:2016.9
****************************************************************************************************/

#include "stdafx.h"
#include "ComImple.h"
#include <process.h>
#include "Utility.h"
#include "GlobalDataManager.h"
#include "Log.h"
//begin add by xiaowei.han 2017-3-25
#include "SensorConfig.h"
#include "SensorIDGenerator.h"
//end add by xiaowei.han 2017-3-25
#pragma warning(push)
#pragma warning(disable:4018)
/*************************************************************************************************
��������:CreateInstance
��������:����ͨ�Žӿ���ʵ����
�������:��
�������:��
����ֵ:����ͨ��ʵ��
***************************************************************************************************/
CComImple& CComImple::CreateInstance()
{
	//static CComImple s_ComImple;
	return s_ComImple;
}

/*************************************************************************************************
��������:CComImple
��������:����ͨ�Žӿ��๹�캯��
�������:��
�������:��
����ֵ:
***************************************************************************************************/
CComImple::CComImple(void)
{
	
}

/*************************************************************************************************
��������:~CComImple
��������:����ͨ�Žӿ�����������
�������:��
�������:��
����ֵ:
***************************************************************************************************/
CComImple::~CComImple(void)
{
	
}

/*************************************************************************************************
��������:StartCom
��������:��������ͨ��
�������:hWnd:���ھ�����򴰿ڷ�����Ϣ
�������:��
����ֵ:
***************************************************************************************************/
void CComImple::StartCom(CWnd* pWnd)
{

	//begin add by xiaowei.han 2017-3-25
	//��ȡ�����ļ��еĴ�������Ϣ
	SENSOR_CONFIG_ELEMENT SensorInfo;
	if (!CSensorConfig::CreateInstance().GetFirstSensorConfig(&SensorInfo))
	{
		ERROR_LOG("GetFirstSensorConfig failed.");
		return;
	}
	//end add by xiaowei.han 2017-3-25
	UINT nComPort = static_cast<UINT>(SensorInfo.nComIndex);
	UINT nBandRate = static_cast<UINT>(SensorInfo.nBaudRate);
	UINT nDataBits = static_cast<UINT>(SensorInfo.nDataBits);
	UINT nStopBits = static_cast<UINT>(SensorInfo.nStopBits);
	UINT nPairty = static_cast<UINT>(SensorInfo.nPairty);
	//��ʼ������ͨ��
	if(FALSE == m_SerialPort.InitPort(pWnd,nComPort,nBandRate,nPairty,nDataBits,nStopBits,SensorInfo.bUseFlowControl))
	{
		ERROR_LOG("InitPort failed!");
		return;
	}
	//�������ƴ��ڼ����߳�
	m_SerialPort.StartMonitoring();
}

/*************************************************************************************************
��������:StopCom
��������:�رմ���ͨ��
�������:��
�������:��
����ֵ:
***************************************************************************************************/
void CComImple::StopCom()
{
	//ֹͣ����
	m_SerialPort.StopMonitoring();

	//ֹͣ�����߳�
	//s_bloop = false;

	//���ն����ͷ���
	//m_SerialPort.m_RecvQueue.ReleaseLock();

	//�ȴ����������߳̽���
	/*if (NULL != m_HandleDecode)
	{
		WaitForSingleObject(m_HandleDecode,MAX_THREAD_WAIT_TIME);
		CloseHandle(m_HandleDecode);
		m_HandleDecode = NULL;
	}*/

	//�رմ���
	m_SerialPort.ClosePort();
}

/*************************************************************************************************
��������:SendComData
��������:�򴮿ڷ�������
�������:��
�������:��
����ֵ:
***************************************************************************************************/
void CComImple::SendComData(BYTE* Buffer, int nLength)
{
	//�жϲ�����Ч��
	if (NULL == Buffer||nLength <= 0)
	{
		return;
	}

	//��������
	m_SerialPort.WriteToPort(Buffer,nLength);
}

CComImple CComImple::s_ComImple;

/*******************************************************************************************************************************
//����ͨ����ʵ��
/*******************************************************************************************************************************

/*************************************************************************************************
��������:CSerialPort
��������:����ͨ���๹�캯��
�������:��
�������:��
����ֵ:
***************************************************************************************************/
CSerialPort::CSerialPort(void)
{
	//��ʼ���ٽ���Դ
	InitializeCriticalSection(&m_csCommunicationSync);

	//��ʼ�����ھ��
	m_hComm = NULL;

	//��ʼ���첽�ṹ��
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;

	//�����¼�
	m_ov.hEvent = NULL;
	m_hWriteEvent = NULL;
	m_hShutdownEvent = NULL;

	m_szWriteBuffer = NULL;

	m_bThreadAlive = FALSE;

	m_nWriteSize=1;
	m_bIsSuspened = FALSE;

	m_nWritePos = 0;
	m_nReadPos = 0;

	//�����߳̾��
	m_HandleDecode = NULL;

	//�����������ݴ����߳�
	DWORD dwThreadID = 0;

	s_bloop = true;

	m_HandleDecode = CreateThread(NULL,0,DecodeComDataProc,this,0,&dwThreadID);

	if (NULL == m_HandleDecode)
	{
		return;
	}
}

/*************************************************************************************************
��������:~CSerialPort
��������:����ͨ������������
�������:��
�������:��
����ֵ:
***************************************************************************************************/
CSerialPort::~CSerialPort(void)
{
	do
	{
		SetEvent(m_hShutdownEvent);
	} while (m_bThreadAlive);


	// �˿�����򿪵ģ��ر�֮��
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	//�رն���д���ر� 
	if(m_hShutdownEvent!=NULL)
		CloseHandle( m_hShutdownEvent); 
	if(m_ov.hEvent!=NULL)
		CloseHandle( m_ov.hEvent ); 
	if(m_hWriteEvent!=NULL)
		CloseHandle( m_hWriteEvent ); 

	//�����߳�

	delete [] m_szWriteBuffer;

	//��ʼ���ٽ���Դ
	DeleteCriticalSection(&m_csCommunicationSync);
}

/*************************************************************************************************
��������:InitPort
��������:��ʼ������
�������:��������
�������:��
����ֵ: TRUE/�ɹ� FALSE/ʧ��
***************************************************************************************************/
//begin delete by xiaowei.han 2017-3-25
#if 0
BOOL CSerialPort::InitPort(CWnd* pPortOwner,	//�����ڴ��ھ�������ڽ�����Ϣ�ã�
						   UINT  portnr,		// �˿ں�
						   UINT  baud,			// ������
						   char  parity,		// ��żУ�� 
						   UINT  databits,		// ����λ 
						   UINT  stopbits,		// ֹͣλ 
						   DWORD dwCommEvents,	// �¼� EV_RXCHAR, EV_CTS ��
						   UINT  writebuffersize)	// д�����С
#endif
//end delete by xiaowei.han 2017-3-25
BOOL CSerialPort::InitPort( CWnd* pPortOwner, /*���ھ�� */ UINT portnr /*= 1*/, /*�˿ں� */ UINT baud /*= 9600*/, /*������ */ UINT parity /*= 'N'*/, /*��żУ�� */ UINT databits /*= 8*/, /*����λ */ UINT stopbits /*= 1*/, /*ֹͣλ */ bool bUseFlowControl /*= false*/,/*�Ƿ�ʹ������ */ DWORD dwCommEvents /*= EV_RXCHAR*/, /*��Ϣ���� */ UINT writebuffersize /*= WRITE_BUFFER_SIZE/*д���� */ )
{
	//�ж϶˿� ����� ֧��20����
	if(portnr < 0 || portnr > MaxSerialPortNum + 1)
	{
		return FALSE;
	}
	
	//����̴߳��ڣ���ص�����
	if (m_bThreadAlive)
	{
		do
		{
			SetEvent(m_hShutdownEvent);
		} while (m_bThreadAlive);
	}

	//�����¼�
	//�첽I/O
	if (m_ov.hEvent != NULL)
	{
		ResetEvent(m_ov.hEvent);
	}
	else
	{
		m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	//д�¼�
	if (m_hWriteEvent != NULL)
	{
		ResetEvent(m_hWriteEvent);
	}
	else
	{
		m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	
	//�ر��¼�
	if (m_hShutdownEvent != NULL)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	//�¼������ʼ�����趨���ȼ���
	m_hEventArray[0] = m_hShutdownEvent;	//���λУ��
	m_hEventArray[1] = m_ov.hEvent;
	m_hEventArray[2] = m_hWriteEvent;

	//������
	m_pOwner = pPortOwner;

	if (NULL != m_szWriteBuffer)
	{
		delete [] m_szWriteBuffer;
	}
	
	m_szWriteBuffer = new char[writebuffersize];

	m_nPortNr = portnr;

	m_nWriteBufferSize = writebuffersize;
	m_dwCommEvents = dwCommEvents;

	BOOL bResult = FALSE;
	TCHAR *szPort = new TCHAR[50];
	TCHAR *szBaud = new TCHAR[50];

	//�����ٽ���
	EnterCriticalSection(&m_csCommunicationSync);

	//�����Ѵ򿪾͹ص�
	if (NULL != m_hComm)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	//���ڲ�������COM10���ϵ�
	wsprintf(szPort, _T("\\\\.\\COM%d"), portnr);
	//wsprintf(szBaud, _T("baud=%d parity=%c data=%d stop=%d"), baud, parity, databits, stopbits);

	//�򿪴���
	m_hComm = CreateFile((LPCTSTR)szPort,				// ��������
					     GENERIC_READ | GENERIC_WRITE,	// ��д��ʽ
					     0,								// 
					     NULL,							// 
					     OPEN_EXISTING,					// ���ڱ���ʹ�� OPEN_EXISTING
					     FILE_FLAG_OVERLAPPED,			// �첽 I/O
					     0);							// ����Ϊ0

	//�������ʧ��
	if (INVALID_HANDLE_VALUE == m_hComm)
	{
		//�ͷ���Դ
		delete [] szPort;
		delete [] szBaud;

		return FALSE;
	}

	//���ó�ʱ
	m_CommTimeouts.ReadIntervalTimeout = 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant = 1000;

	//����
	//�ֱ����Windows API���ô��ڲ���
	//���ó�ʱ
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{
		
		// ���Զ˿����ݵ���Ӧʱ��Ҫ����ϸ񣬿ɲ����¼�������ʽ��
		// �����ʹ��115200�����ʣ��Ѿ���USBת���ڵļ��ޣ������ϻ���ھ���ͬ��������ݴ�λ�����ǡ����������⡣
		// ������.......
		// Windows������9�ִ���ͨ���¼����ϳ��õ����������֣�
		//		EV_RXCHAR:���յ�һ���ֽڣ����������뻺������
		//		EV_TXEMPTY:����������е����һ���ַ������ͳ�ȥ��
		//		EV_RXFLAG:���յ��¼��ַ�(DCB�ṹ��EvtChar��Ա)���������뻺����
		// ����SetCommMask()ָ�������õ��¼���Ӧ�ó���ɵ���WaitCommEvent()���ȴ��¼��ķ�����
		// SetCommMask(hComm,0)��ʹWaitCommEvent()��ֹ
		
		//����ͨ���¼�
		if (SetCommMask(m_hComm, dwCommEvents))
		{
			//��ȡ��ǰDCB����
			if (GetCommState(m_hComm, &m_dcb))
			{
				//�����ּ��ַ�
				//m_dcb.EvtChar = 'q';

				if (bUseFlowControl)
				{
					//����RTS��λ
					m_dcb.fRtsControl = RTS_CONTROL_ENABLE;
					m_dcb.fOutxCtsFlow = 1;
					m_dcb.fOutxDsrFlow = 0;
					m_dcb.fDsrSensitivity = 0;
					//m_dcb.fRtsControl = RTS_CONTROL_ENABLE;
					m_dcb.fDtrControl = 0;
					m_dcb.fInX = 0; 
					m_dcb.fOutX = 0;
					m_dcb.XoffChar = 0;
					m_dcb.XonChar = 0;
					m_dcb.XoffLim = 0;
					m_dcb.XonLim = 0;
				}
				else
				{
					m_dcb.fOutxCtsFlow = 0;
					m_dcb.fOutxDsrFlow = 0;
					m_dcb.fDsrSensitivity = 0;
					m_dcb.fRtsControl = 0;
					m_dcb.fDtrControl = 0;
					m_dcb.fInX = 0; 
					m_dcb.fOutX = 0;
					m_dcb.XoffChar = 0;
					m_dcb.XonChar = 0;
					m_dcb.XoffLim = 0;
					m_dcb.XonLim = 0;
				}

				//���ò�����
				m_dcb.BaudRate = baud;
				//��������λ
				m_dcb.ByteSize = databits;
				//����ֹͣλ
				m_dcb.StopBits = stopbits;
				//����У��λ
				m_dcb.Parity = parity;
				//��дDCB�ṹ
				//if (BuildCommDCB((LPCWSTR)szBaud, &m_dcb))
				//{
					//����DCB
					if (!SetCommState(m_hComm, &m_dcb))
					{
						ProcessErrorMessage("SetCommState()");
					}
				//}
				//else
				//{
				//	ProcessErrorMessage("BuildCommDCB()");
				//}
			}
			else
			{
				ProcessErrorMessage("GetCommState()");
			}
		}
		else
		{
			ProcessErrorMessage("SetCommMask()");
		}
	}
	else
	{
		ProcessErrorMessage("SetCommTimeouts()");
	}

	//�ͷ���Դ
	delete [] szPort;
	delete [] szBaud;

	//��ֹ��д����ս��պͷ���
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	//�ͷ��ٽ���Դ
	LeaveCriticalSection(&m_csCommunicationSync);

	return TRUE;
}


/*************************************************************************************************
��������:CommThread
��������:�����߳�
�������:pParam : comͨ�Žӿ���ָ��
�������:��
����ֵ: 0������չ��
��ע: �¼��������ģ���鴮��-->����ѭ��{WaitCommEvent(������ѯ��)ѯ���¼�-->������¼�����-->����Ӧ����(�ر�\��\д)}
***************************************************************************************************/
UINT CSerialPort::CommThread(LPVOID pParam)
{
	//comͨ�Žӿ���ָ��
	CSerialPort *pSerialPort = (CSerialPort*)pParam;

	if(NULL == pSerialPort)
	{
		//�����߳�
		return 0;
	}
	
	//TRUE��ʾ�߳���������
	pSerialPort->m_bThreadAlive = TRUE;	
		
	DWORD BytesTransfered = 0; 
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	COMSTAT comstat;
	BOOL  bResult = TRUE;
		
	//��ʼʱ������ڻ���
	//��鴮���Ƿ��
	if (pSerialPort->m_hComm)
	{
		PurgeComm(pSerialPort->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	}

	//ֻҪ�̴߳��ھͲ��϶�ȡ����
	while (TRUE) 
	{ 
		//��ʾ�ú������첽��
		bResult = WaitCommEvent(pSerialPort->m_hComm, &Event, &pSerialPort->m_ov);
		
		if (!bResult)  
		{ 
			//���WaitCommEvent����ErrorΪFALSE�����ѯ������Ϣ
			switch (dwError = GetLastError()) 
			{ 
			case ERROR_IO_PENDING: 	//���������û���ַ��ɶ�
				{ 
					break;
				}
			case ERROR_INVALID_PARAMETER://ϵͳ����
				{
					break;
				}
			default://�����������󣬰��������ڶ�д�жϿ��������ӵĴ���
				{
					pSerialPort->ProcessErrorMessage("WaitCommEvent()");
					break;
				}
			}
		}
		else
		{
			bResult = ClearCommError(pSerialPort->m_hComm, &dwError, &comstat);

			if (comstat.cbInQue == 0)
				continue;
		}

		//���ȴ��������������߳�
		//�ȴ�3���¼����ض�/��/д
		Event = WaitForMultipleObjects(3,	//3���¼�
			pSerialPort->m_hEventArray,			//�¼�����
			FALSE,							//��һ���¼������ͷ���
			INFINITE);						//��ʱʱ��

		switch (Event)
		{
		case 0:	//�ض��¼����رմ���
			{
				CloseHandle(pSerialPort->m_hComm);
				pSerialPort->m_hComm=NULL;
				pSerialPort->m_bThreadAlive = FALSE;
				
				AfxEndThread(100);
				break;
			}
		case 1:	//���¼� ������ĸ�����Ϣ���ͳ�ȥ
			{
				memset(&comstat, 0, sizeof(COMSTAT)); 
				GetCommMask(pSerialPort->m_hComm, &CommEvent);

				//���յ��ַ������������뻺������
				if (CommEvent & EV_RXCHAR)
				{
					ReceiveChar(pSerialPort, comstat);
				}

				//CTS�ź�״̬�����仯
				if (CommEvent & EV_CTS)
				{
					//��ӡ��־
				}

				//�����з����ж�
				if (CommEvent & EV_BREAK)
				{
					//��ӡ��־
				}

				//������·״̬������·״̬�������CE_FRAME,CE_OVERRUN��CE_RXPARITY 
				if (CommEvent & EV_ERR)
				{
					//��ӡ��־
				}

				//��⵽����ָʾ	
				if (CommEvent & EV_RING)
				{
					//��ӡ��־
				}

				//���յ��¼��ַ������������뻺������	
				if (CommEvent & EV_RXFLAG)
				{
					//��ӡ��־
				}
				break;
			}  
		case 2: //д�¼�
			{
				WriteChar(pSerialPort);
				break;
			}
		default:
			{
				//���մ���
				//��ӡ��־
				break;
			}

		}

	}

	return 0;
}

/*************************************************************************************************
��������:StartMonitoring
��������:���������߳�
�������:��
�������:��
����ֵ: TRUE/�ɹ� FALSE/ʧ��
***************************************************************************************************/
BOOL CSerialPort::StartMonitoring()
{
	//���������߳�
	if (!(m_Thread = AfxBeginThread(CommThread, this)))
	{
		return FALSE;
	}

	m_bIsSuspened = false;

	return TRUE;	
}

/*************************************************************************************************
��������:RestartMonitoring
��������:��λ�����߳�
�������:��
�������:��
����ֵ: TRUE/�ɹ� FALSE/ʧ��
***************************************************************************************************/
BOOL CSerialPort::RestartMonitoring()
{
	m_bIsSuspened = false;

	m_Thread->ResumeThread();

	return TRUE;	
}

/*************************************************************************************************
��������:StopMonitoring
��������:��������߳�
�������:��
�������:��
����ֵ: TRUE/�ɹ� FALSE/ʧ��
***************************************************************************************************/
BOOL CSerialPort::StopMonitoring()
{
	m_bIsSuspened = true;

	m_Thread->SuspendThread();

	return TRUE;
}

/*************************************************************************************************
��������:ProcessErrorMessage
��������:������Ϣ��ʾ
�������:ErrorText ������Ϣ
�������:��
����ֵ: ��
***************************************************************************************************/
void CSerialPort::ProcessErrorMessage(char* ErrorText)
{
	if(NULL == ErrorText)
	{
		return;
	}

	char *Temp = new char[256];
	
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	sprintf(Temp, "����:  %s ʧ�ܿ�������������: \n%s\nPort: %d\n", (char*)ErrorText, lpMsgBuf, m_nPortNr);

	//��ӡ��־ or ����
	MessageBox(NULL, (LPCWSTR)Temp, _T("Application Error"), MB_ICONSTOP);

	LocalFree(lpMsgBuf);
	delete [] Temp;

	return;
}

/*************************************************************************************************
��������:WriteChar
��������:д����
�������:pCom comͨ�Žӿ���ָ��
�������:��
����ֵ: ��
***************************************************************************************************/
void CSerialPort::WriteChar(CSerialPort* pCom)
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;

	DWORD BytesSent = 0;

	//��λд�¼����
	ResetEvent(pCom->m_hWriteEvent);

	//�����ٽ���
	EnterCriticalSection(&pCom->m_csCommunicationSync);

	if (bWrite)
	{
		//��ʼ������
		pCom->m_ov.Offset = 0;
		pCom->m_ov.OffsetHigh = 0;

		//��ջ���
		PurgeComm(pCom->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

		//����д��
		bResult = WriteFile(pCom->m_hComm,							// ���ھ��
							pCom->m_szWriteBuffer,					// ��Ϣ����
							pCom->m_nWriteSize,						// ������Ϣ����
							&BytesSent,								// �����ֽ���
							&pCom->m_ov);

		if (!bResult)  //�����������
		{
			DWORD dwError = GetLastError();
			switch (dwError)
			{
				case ERROR_IO_PENDING:
					{
						BytesSent = 0;
						bWrite = FALSE;
						break;
					}
				default:
					{
						//��ӡ��־ or ������Ϣ
						pCom->ProcessErrorMessage("WriteFile()");
						break;
					}
			}
		} 
		else
		{
			LeaveCriticalSection(&pCom->m_csCommunicationSync);
		}
	}

	if (!bWrite)
	{
		bWrite = TRUE;
	
		bResult = GetOverlappedResult(pCom->m_hComm,	// ���ھ��
									  &pCom->m_ov,		// Overlapped �ṹ
									  &BytesSent,		// ��¼�ֽ���
									  TRUE); 			// �ȴ���־

		LeaveCriticalSection(&pCom->m_csCommunicationSync);

		if (!bResult)
		{
			//��ӡ��־ or ������Ϣ
			pCom->ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
		}	
	}

	if (BytesSent != pCom->m_nWriteSize)
	{
		//ʵ�ʷ��ͳ��Ȳ�һ��
		//��ӡ��־
	}
}

/*************************************************************************************************
��������:ReceiveChar
��������:������
�������:pCom comͨ�Žӿ���ָ�� comstat com�ṹ
�������:��
����ֵ: ��
***************************************************************************************************/
static int nTime = 0;
void CSerialPort::ReceiveChar(CSerialPort* pCom, COMSTAT comstat)
{
	BOOL  bRead = TRUE; 
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	unsigned char RXBuff[WRITE_BUFFER_SIZE];

	while (true) 
	{ 
		//��ֹ����
		if(WaitForSingleObject(pCom->m_hShutdownEvent,0) == WAIT_OBJECT_0)
		{
            return;
		} 
		
		EnterCriticalSection(&pCom->m_csCommunicationSync);

		//����COMSTAT
		bResult = ClearCommError(pCom->m_hComm, &dwError, &comstat);

		LeaveCriticalSection(&pCom->m_csCommunicationSync);
		
		//�����ַ������������ж�ѭ��
		if (comstat.cbInQue == 0)
		{
			break;
		}
						
		EnterCriticalSection(&pCom->m_csCommunicationSync);

		if (bRead)
		{
			//���ڶ������������������ֽ�
			bResult = ReadFile(pCom->m_hComm,		// ���ھ�� 
							   &RXBuff,				// ��ȡ������
							   WRITE_BUFFER_SIZE,	// ��һ���ֽ�
							   &BytesRead,			// ��ȡ�ֽ���
							   &pCom->m_ov);

			///�����ش��󣬴�����
			if (!bResult)  
			{ 
				switch (dwError = GetLastError()) 
				{ 
					case ERROR_IO_PENDING: 	
						{ 
							//�첽IO���ڽ���
							bRead = FALSE;
							break;
						}
					default:
						{
							//������� or ��ӡ��־
							pCom->ProcessErrorMessage("ReadFile()");
							break;
						} 
				}
			}
			else
			{
				bRead = TRUE;
			}
		}

		//�첽IO�������ڽ��У���Ҫ����GetOverlappedResult��ѯ
		if (!bRead)
		{
			bRead = TRUE;
			bResult = GetOverlappedResult(pCom->m_hComm,
										  &pCom->m_ov,
										  &BytesRead,
										  TRUE);

			if (!bResult)  
			{
				//���������Ϣ or ��ӡ��־
				pCom->ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
			}	
		}
				
		LeaveCriticalSection(&pCom->m_csCommunicationSync);

		//���յ�������Ϣ ���浽RXBuff
		//���͵������ڴ���
		//::SendMessage((pCom->m_pOwner)->m_hWnd, WM_COMM_RXCHAR, (WPARAM) RXBuff, (LPARAM) port->m_nPortNr);
		//��ӵ�������� -> ���� ->����
		//m_RecvQueue.

		/*BYTE sendbuf[10];
		sendbuf[0] = 0x11;
		pCom->WriteToPort(sendbuf,1);*/

		//memcpy(pCom->m_recvBuffer,RXBuff,BytesRead);

		//pCom->m_nWritePos += BytesRead >= WRITE_BUFFER_SIZE? pCom->m_nWritePos - WRITE_BUFFER_SIZE : pCom->m_nWritePos;
		//begin add by xiaowei.han 2017-3-25
		ReceiveReportData(RXBuff,(int)BytesRead);
		ReceiveIdentifyData(RXBuff,(int)BytesRead);
		//end add by xiaowei.han 2017-3-25

		//begin modify by xiaowei.han 2017-3-25
#if 0
		if(BytesRead==6)
		{
			if(RXBuff[0] == 0xAA)
			{
				if(RXBuff[1] == 0x04)//����
				{
					short nData = 0;
					memcpy(&nData,RXBuff+3,sizeof(short));
					BYTE nSensorChan = 0;
					memcpy(&nSensorChan,RXBuff+2,sizeof(BYTE));

					GLOBAL_DATA.PushData(nSensorChan,nData);
					CString strTemp;
					strTemp.Format(_T("%d"),nTime);
					GLOBAL_DATA.ModifyData(GLOBAL_DATA.FindTimeID(),nTime++,strTemp);
				}
				else if(RXBuff[1] == 0x01)//ʶ��
				{

					nTime = 0;

					short nSensorID = 0;
					memcpy(&nSensorID,RXBuff+3,sizeof(short));
					BYTE nSensorChan = 0;
					memcpy(&nSensorChan,RXBuff+2,sizeof(BYTE));

					//���������
					GLOBAL_DATA.AppendTimeColumn();
					GLOBAL_DATA.AppendColumn(nSensorID,nSensorChan);

					BYTE bHz[6] = {0xAA,0x02,0x01,0x00,0xAA,0xFF};

					pCom->WriteToPort(bHz,6);

					GLOBAL_DATA.SetChanged();
					
					GLOBAL_DATA.Notify((void*)1);
				}
			}
		}
#endif
		//end modify by xiaowei.han 2017-3-25
	}

}

/*************************************************************************************************
��������:WriteToPort
��������:�򴮿�д���ַ���
�������:string �� �ַ���
�������:��
����ֵ: ��
***************************************************************************************************/
void CSerialPort::WriteToPort(char* string)
{
	if(NULL == m_hComm)
	{
		return;
	}
	
	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	strcpy(m_szWriteBuffer, string);
	m_nWriteSize=strlen(string);

	// ����д�¼� д���ַ���
	SetEvent(m_hWriteEvent);
}

/*************************************************************************************************
��������:WriteToPort
��������:�򴮿�д���ַ���
�������:string �� �ַ��� n���ַ�������
�������:��
����ֵ: ��
***************************************************************************************************/
void CSerialPort::WriteToPort(char* string,int n)
{		
	if(NULL == m_hComm)
	{
		return;
	}

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	memcpy(m_szWriteBuffer, string, n);
	m_nWriteSize=n;

	// ����д�¼� д���ַ���
	SetEvent(m_hWriteEvent);
}

/*************************************************************************************************
��������:WriteToPort
��������:�򴮿�д���ַ���
�������:string �� �ַ��� n���ַ�������
�������:��
����ֵ: ��
***************************************************************************************************/
void CSerialPort::WriteToPort(LPCTSTR string,int n)
{		
	if(NULL == m_hComm)
	{
		return;
	}

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	memcpy(m_szWriteBuffer, string, n);
	m_nWriteSize = n;

	// ����д�¼� д���ַ���
	SetEvent(m_hWriteEvent);
}

/*************************************************************************************************
��������:WriteToPort
��������:�򴮿�д���ַ���
�������:Buffer �� �ַ����� n���ַ�������
�������:��
����ֵ: ��
***************************************************************************************************/
void CSerialPort::WriteToPort(BYTE* Buffer, int n)
{
	if(NULL == m_hComm)
	{
		return;
	}

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	int i;
	for(i=0; i<n; i++)
	{
		m_szWriteBuffer[i] = Buffer[i];
	}
	m_nWriteSize=n;
	
	// ����д�¼� д���ַ���
	SetEvent(m_hWriteEvent);
}

/*************************************************************************************************
��������:GetDCB
��������:ȡ��DCB��Ϣ
�������:��
�������:��
����ֵ: DCB�ṹ��Ϣ
***************************************************************************************************/
DCB CSerialPort::GetDCB()
{
	return m_dcb;
}

/*************************************************************************************************
��������:GetCommEvents
��������:��ȡ�¼�
�������:��
�������:��
����ֵ: �¼�����
***************************************************************************************************/
DWORD CSerialPort::GetCommEvents()
{
	return m_dwCommEvents;
}

/*************************************************************************************************
��������:GetWriteBufferSize
��������:��ȡд��������С
�������:��
�������:��
����ֵ: ��������С
***************************************************************************************************/
DWORD CSerialPort::GetWriteBufferSize()
{
	return m_nWriteBufferSize;
}

/*************************************************************************************************
��������:ClosePort
��������:�رմ���
�������:��
�������:��
����ֵ: ��
***************************************************************************************************/
void CSerialPort::ClosePort()
{
    if(m_bIsSuspened)
	{
		RestartMonitoring();
	}

    if (m_bThreadAlive)
    {
        MSG message;
        while (m_bThreadAlive)
        {
			//��ֹ����
            if(::PeekMessage(&message, m_pOwner->m_hWnd, 0, 0, PM_REMOVE))
            {
                ::TranslateMessage(&message);
                ::DispatchMessage(&message);
            }

            SetEvent(m_hShutdownEvent);
        }
    }

    if(NULL != m_szWriteBuffer)
    {
        delete [] m_szWriteBuffer;
        m_szWriteBuffer = NULL;
    }

    if(NULL != m_hComm)
    {
        CloseHandle(m_hComm);
        m_hComm = NULL;
    }

	//�ر��¼�
	if(NULL != m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}

	if(NULL != m_ov.hEvent)
	{
		ResetEvent(m_ov.hEvent);
	}

	if(NULL != m_hWriteEvent)
	{
		ResetEvent(m_hWriteEvent);
	}
}

/*************************************************************************************************
��������:SendData
��������:��������
�������:��
�������:��
����ֵ: ��
***************************************************************************************************/
void CSerialPort::SendData(LPCTSTR lpszData, const int nLength)
{
	if(NULL == m_hComm)
	{
		return;
	}

	//�������д�뻺������С
	if(nLength > WRITE_BUFFER_SIZE)
	{
		return;
	}

	memset(m_szWriteBuffer, 0, nLength);
	memcpy(m_szWriteBuffer, lpszData, nLength);
	m_nWriteSize=nLength;

	// ����д�¼� д���ַ���
	SetEvent(m_hWriteEvent);
}

/*************************************************************************************************
��������:RecvData
��������:����ָ����������
�������:��
�������:��
����ֵ: ��
***************************************************************************************************/
BOOL CSerialPort::RecvData(LPTSTR lpszData, const int nSize)
{
	if(NULL == m_hComm)
	{
		return FALSE;
	}

    memset(lpszData,0,nSize);

    DWORD mylen  = 0;
    DWORD mylen2 = 0;
    while (mylen < nSize) 
	{
        if(!ReadFile(m_hComm,lpszData,nSize,&mylen2,NULL)) 
            return FALSE;
        mylen += mylen2;   
    }
    
    return TRUE;
}

/*************************************************************************************************
��������:GetCommTimeOuts
��������:��ó�ʱʱ��ṹ
�������:��
�������:��
����ֵ: ��ʱʱ��ṹ��Ϣ
***************************************************************************************************/
COMMTIMEOUTS CSerialPort:: GetCommTimeOuts()
{
	return m_CommTimeouts;
}

/*************************************************************************************************
��������:SetCommTimeOuts
��������:���ó�ʱ
�������:��
�������:��
����ֵ: TRUE
***************************************************************************************************/
BOOL CSerialPort::SetCommTimeOuts(COMMTIMEOUTS *lpTimeOuts)
{
	//���ó�ʱ 
    SetCommTimeouts(m_hComm, lpTimeOuts);

	return TRUE;
}


///*******************************************************************************************************************************
////�������ݽ��ն�����ʵ��
///*******************************************************************************************************************************
//
///*******************************************************************************************************************************
//��������:CComDataQueue
//��������:���Ͷ�����Ĺ��캯��
//�������:��
//�������:��
//����ֵ:��
//*******************************************************************************************************************************/
//
//CComDataQueue::CComDataQueue( void ):
//m_SemphoreHandle(NULL)
//{
//	//��ʼ���ٽ���
//	InitializeCriticalSection(&m_cs);
//
//	//��ն���
//	m_Queue.clear();
//
//	//�����ź���
//	m_SemphoreHandle = CreateSemaphore(NULL,0,1,NULL);
//}
//
///*******************************************************************************************************************************
//��������:~CComDataQueue
//��������:���Ͷ�����Ĺ��캯��
//�������:��
//�������:��
//����ֵ:��
//*******************************************************************************************************************************/
//CComDataQueue::~CComDataQueue( void )
//{
//	//����ڴ�
//	FreeQueueMemory();
//	//ɾ���ٽ���
//	DeleteCriticalSection(&m_cs);
//	CloseHandle(m_SemphoreHandle);
//	m_SemphoreHandle = NULL;
//}
//
///*******************************************************************************************************************************
//��������:IsQueueEmpty
//��������:�жϷ��Ͷ����Ƿ�Ϊ��
//�������:��
//�������:��
//����ֵ:true:����Ϊ��  false:���в�Ϊ��
//*******************************************************************************************************************************/
//bool CComDataQueue::IsQueueEmpty( void )
//{
//	bool bEmpty = false;
//	EnterCriticalSection(&m_cs);
//
//	//�ж϶����Ƿ�Ϊ��
//	bEmpty = m_Queue.empty();
//
//	LeaveCriticalSection(&m_cs);
//
//	return bEmpty;
//}
//
///*******************************************************************************************************************************
//��������:GetQueueElementNum
//��������:��ȡ������Ԫ�صĸ���
//�������:��
//�������:��
//����ֵ:���ض�����Ԫ�صĸ���
//*******************************************************************************************************************************/
//int CComDataQueue::GetQueueElementNum( void )
//{
//	int nSize = 0;
//
//	EnterCriticalSection(&m_cs);
//
//	//��ȡ���еĳ���
//	nSize = (int)m_Queue.size();
//
//	LeaveCriticalSection(&m_cs);
//
//	return nSize;
//}
//
///*******************************************************************************************************************************
//��������:PushElement
//��������:������з���Ԫ��
//�������:pBuffer:���ݵ����� nLength:pBuffer�ĳ���
//�������:��
//����ֵ:��
//*******************************************************************************************************************************/
//void CComDataQueue::PushElement( const char* pBuffer,int nLength )
//{
//	if (NULL == pBuffer )
//	{
//		return;
//	}
//
//	//�����ڴ�
//	LPCOMDATA pContentData = new COMDATA;
//
//	//�����ڴ�ʧ��
//	if (NULL == pContentData)
//	{
//		return;
//	}
//
//	//��¼�������ݵĳ���
//	pContentData->nDataLength = nLength;
//
//	//�����ڴ�
//	pContentData->pData = new char[pContentData->nDataLength];
//
//	//�����ڴ�ʧ��
//	if (NULL == pContentData->pData)
//	{
//		//�ͷ��ڴ�
//		DELETE_POINTER(pContentData);
//		return;
//	}
//
//	//�����ڴ�
//	memcpy(pContentData->pData,pBuffer,nLength);
//
//	EnterCriticalSection(&m_cs);
//
//	//�������β��
//	m_Queue.push_back(pContentData);
//
//	LeaveCriticalSection(&m_cs);
//
//	ReleaseSemaphore(m_SemphoreHandle,1,NULL);
//
//}
//
///*******************************************************************************************************************************
//��������:PopElement
//��������:�������ɾ��һ��Ԫ��
//�������:��
//�������:��
//����ֵ:��
//*******************************************************************************************************************************/
//void CComDataQueue::PopElement( void )
//{
//	//�ж��Ƿ�Ϊ��
//	if (IsQueueEmpty())
//	{
//		return;
//	}
//
//	EnterCriticalSection(&m_cs);
//
//	LPCOMDATA p = m_Queue.front();
//	m_Queue.pop_front();
//
//	LeaveCriticalSection(&m_cs);
//
//	DELETE_POINTER(p);
//
//}
//
///*******************************************************************************************************************************
//��������:GetHeadElement
//��������:���ض����е�ͷԪ��ָ��
//�������:��
//�������:��
//����ֵ:LPCOMDATA:ָ������е�ͷԪ��
//*******************************************************************************************************************************/
//CComDataQueue::LPCOMDATA CComDataQueue::GetHeadElement( void )
//{
//
//	WaitForSingleObject(m_SemphoreHandle,INFINITE);
//
//	LPCOMDATA pHeadElement = NULL;
//
//	//�ж϶����Ƿ�Ϊ��
//	if (IsQueueEmpty())
//	{
//		return pHeadElement;
//	}
//
//	EnterCriticalSection(&m_cs);
//
//	//��ȡ����ͷԪ��
//	pHeadElement = m_Queue.front();
//
//	LeaveCriticalSection(&m_cs);
//
//	return pHeadElement;
//}
//
///*******************************************************************************************************************************
//��������:FreeQueueMemory
//��������:�ͷŶ������ڴ�
//�������:��
//�������:��
//����ֵ:��
//*******************************************************************************************************************************/
//void CComDataQueue::FreeQueueMemory( void )
//{
//	
//	//�ж϶����Ƿ�Ϊ��
//	if (IsQueueEmpty())
//	{
//		return;
//	}
//
//	
//	EnterCriticalSection(&m_cs);
//
//	deque<LPCOMDATA>::iterator Iter = m_Queue.begin();
//
//	while (Iter != m_Queue.end())
//	{
//		//�ͷ��ڴ�
//		DELETE_POINTER(*Iter);
//		++Iter;
//	}
//
//	m_Queue.clear();
//	LeaveCriticalSection(&m_cs);
//}
//
//void CComDataQueue::ReleaseLock( void )
//{
//	if (NULL != m_SemphoreHandle)
//	{
//		ReleaseSemaphore(m_SemphoreHandle,1,NULL);
//	}
//}

//�߳�ѭ����ʶ
bool CSerialPort::s_bloop = true;
/*******************************************************************************************************************************
��������:DecodeComDataProc
��������:�������������߳�
�������:lpParam:
�������:��
����ֵ:DWORD
*******************************************************************************************************************************/
DWORD WINAPI CSerialPort::DecodeComDataProc( LPVOID lpParam )
{
	DWORD dwResult = 0;

	CSerialPort* pArg = (CSerialPort*)lpParam;

	if (NULL == pArg)
	{
		return dwResult;
	}

	while (s_bloop)
	{
		//if(pArg->m_nReadPos != 0)
		//{
		//	memcpy(pArg->m_procBuffer +  pArg->m_nReadPos,pArg->m_recvBuffer,pArg->m_nReadPos);

		//	//����

		//}
		//CComDataQueue::LPCOMDATA pData = pArg->m_RecvQueue.GetHeadElement();
		//������ݲ�Ϊ��
		//if (NULL != pData)
		{
			//��������---->GlobalDataManager
			//pData


			//����Ԫ��
			//pArg->m_RecvQueue.PopElement();
		}
	}
	//DELETE_POINTER(pArg);
	return dwResult;
}

void CSerialPort::ReceiveReportData( BYTE* pData,int nDataLength )
{
	if (nullptr == pData || 0 == nDataLength)
	{
		return;
	}

	if (nDataLength < 8)
	{
		ERROR_LOG("[report msg] the report msg is not valid.");
		return;
	}
	//������ϱ�����
	if (pData[0] == 0xBD)
	{

		//��ȡ���ݳ���
		int nTempDataLength = pData[1];

		if (nTempDataLength + 2 == nDataLength)
		{
			//��ȡ���������Ƴ���
			int nSensorNameLength = pData[2];
			char szSensorName[MAX_PATH] = {0};
			memcpy(szSensorName,pData + 3,nSensorNameLength);
			NECESSARY_LOG("[report msg] the sensor name is [%s].",szSensorName);
			//�ж�CRC�Ƿ�Ϸ�
			if (pData[nDataLength - 1] == Utility::CalCRC8(pData,nDataLength - 1))
			{
				//ȡ����
				float fValue = 0.0f;
				memcpy(&fValue,pData + 1 + 1 + 1 + nSensorNameLength,4);

				NECESSARY_LOG("receive valud data [%.2f kpa].",fValue);

				GLOBAL_DATA.PushData(0,fValue);
				CString strTemp;
				strTemp.Format(_T("%d"),nTime);
				GLOBAL_DATA.ModifyData(GLOBAL_DATA.FindTimeID(),nTime++,strTemp);
			}
			else
			{
				ERROR_LOG("[report msg] the crc sum is not match!");
			}
		}
		else
		{
			ERROR_LOG("[report msg] the msg length is not valid.");
		}
	}

}

void CSerialPort::ReceiveIdentifyData( BYTE* pData,int nDataLength )
{
	if (nullptr == pData || 0 == nDataLength)
	{
		return;
	}

	if (nDataLength < 5)
	{
		ERROR_LOG("[identify msg] the identify msg is not valid.");
		return;
	}
	//������ϱ�����
	if (pData[0] == 0xBA)
	{

		//��ȡ���ݳ���
		int nTempDataLength = pData[1];

		if (nTempDataLength + 2 == nDataLength)
		{
			//��ȡ���������Ƴ���
			int nSensorNameLength = pData[2];
			char szSensorName[MAX_PATH] = {0};
			memcpy(szSensorName,pData + 3,nSensorNameLength);
			NECESSARY_LOG("[identify msg] the sensor name is [%s].",szSensorName);
			//�ж�CRC�Ƿ�Ϸ�
			if (pData[nDataLength - 1] == Utility::CalCRC8(pData,nDataLength - 1))
			{
				//�ж����߻�������
				if (0x01 == pData[nDataLength - 2])
				{
					NECESSARY_LOG("[identify msg] [%s] is onLine!",szSensorName);

					//���ʱ����
					GLOBAL_DATA.AppendTimeColumn();
					//��ӵ�λ��
					GLOBAL_DATA.AppendColumn(CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(std::string(szSensorName)),0,"p(kpa)\nѹǿ");

					//���ò�������
					int nMsgLength = 6 + nSensorNameLength;

					BYTE* pSendBuffer = new BYTE[nMsgLength];
					if (nullptr != pSendBuffer)
					{
						ZeroMemory(pSendBuffer,nMsgLength);
						pSendBuffer[0] = 0xAF;
						pSendBuffer[1] = nMsgLength - 2;
						pSendBuffer[2] = nSensorNameLength;
						memcpy(pSendBuffer + 3,szSensorName,nSensorNameLength);
						pSendBuffer[nMsgLength - 3] = 0x0A;
						pSendBuffer[nMsgLength - 2] = 0x00;
						pSendBuffer[nMsgLength - 1] = Utility::CalCRC8(pSendBuffer,nMsgLength - 1);

						COMIPLE.SendComData(pSendBuffer,nMsgLength);

						delete []pSendBuffer;
						pSendBuffer = nullptr;

						GLOBAL_DATA.SetChanged();
						GLOBAL_DATA.Notify((void*)1);

					}

				}
				else if (0x00 == pData[nDataLength - 2])
				{
					NECESSARY_LOG("[identify msg] [%s] is offLine!",szSensorName);
				}
				else
				{

				}
			}
			else
			{
				ERROR_LOG("[identify msg] the crc sum is not match!");
			}
		}
		else
		{
			ERROR_LOG("[identify msg] the msg length is not valid.");
		}
	}

}

#pragma warning(pop)