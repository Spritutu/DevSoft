/****************************************************************************************
*��Ȩ����:
*�ļ���:ComImple.h
*����:����ͨ�Žӿ���
*����:2016.9
******************************************************************************************/

#ifndef COM_IMPL_H
#define COM_IMPL_H

//#include <windows.h>
#include <deque>

using std::deque;

enum
{
	// �����з����ж�
	WM_COMM_BREAK_DETECTED = WM_USER + 100,
	// CTS�ź�״̬�����仯
	WM_COMM_CTS_DETECTED,	
	// The DSR (data-set-ready) signal changed state.
	WM_COMM_DSR_DETECTED,	
	// ������·״̬����
	WM_COMM_ERR_DETECTED,
	// ��⵽����ָʾ
	WM_COMM_RING_DETECTED,
	// The RLSD (receive-line-signal-detect) signal changed state. 
	WM_COMM_RLSD_DETECTED,
	// A character was received and placed in the input buffer.
	WM_COMM_RXCHAR,		
	// ���յ��¼��ַ������������뻺������
	WM_COMM_RXFLAG_DETECTED,	
	// The last character in the output buffer was sent.  
	WM_COMM_TXEMPTY_DETECTED
};

#if 0
#define WM_COMM_BREAK_DETECTED		(WM_USER+1)	// �����з����ж�
#define WM_COMM_CTS_DETECTED		(WM_USER+2)	// CTS�ź�״̬�����仯
#define WM_COMM_DSR_DETECTED		WM_USER+3	// The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED		WM_USER+4	// ������·״̬���� 
#define WM_COMM_RING_DETECTED		WM_USER+5	// ��⵽����ָʾ
#define WM_COMM_RLSD_DETECTED		WM_USER+6	// The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR				WM_USER+7	// A character was received and placed in the input buffer. 
#define WM_COMM_RXFLAG_DETECTED		WM_USER+8	// ���յ��¼��ַ������������뻺������	 
#define WM_COMM_TXEMPTY_DETECTED	WM_USER+9	// The last character in the output buffer was sent.  
#endif
//����
#define MaxSerialPortNum  (20)   ///��Ч�Ĵ����ܸ��������Ǵ��ڵĺ�

#define  MAX_THREAD_WAIT_TIME  (0xFFFFFFFF)  //�̵߳ȴ��˳������ȴ�ʱ��

#define WRITE_BUFFER_SIZE			(2048)		//д��������С

#define COMIPLE		CComImple::CreateInstance()

//����ͨ��
class CSerialPort
{
public:
	CSerialPort(void);
	virtual ~CSerialPort(void);

	//���ڳ�ʼ��
	BOOL InitPort(CWnd* pPortOwner, //���ھ��
		UINT portnr = 1, //�˿ں�
		UINT baud = 9600, //������
		UINT parity = 0, //��żУ��
		UINT databits = 8, //����λ
		UINT stopbits = 1, //ֹͣλ
		bool bUseFlowControl = false,//�Ƿ�ʹ������
		DWORD dwCommEvents = EV_RXCHAR, //��Ϣ����
		UINT writebuffersize = WRITE_BUFFER_SIZE//д����
		);

	//���ƴ��ڼ����߳�
	BOOL		 StartMonitoring();//��ʼ����
	BOOL		 RestartMonitoring();//���¼���
	BOOL		 StopMonitoring();//ֹͣ����

	//��ȡд�����С
	DWORD		 GetWriteBufferSize();
	//��ȡ�¼�
	DWORD		 GetCommEvents();
	//��ȡDCB
	DCB			 GetDCB();
	//��ȡ��ʱ
	COMMTIMEOUTS GetCommTimeOuts();
	//���ó�ʱ
	BOOL         SetCommTimeOuts(COMMTIMEOUTS * lpTimeOuts);

	//д���ݵ�����
	void		WriteToPort(char* string);
	void		WriteToPort(char* string,int n);
	void		WriteToPort(LPCTSTR string,int n);
	void        WriteToPort(BYTE* Buffer, int n);

	//�رմ���
	void		ClosePort();


	//���ڷ��ͺ��������ã�
	BOOL        RecvData(LPTSTR lpszData, const int nSize);
	//���ڽ��պ��������ã�
	void        SendData(LPCTSTR lpszData, const int nLength);
protected:
	//������Ϣ���
	void ProcessErrorMessage(char* ErrorText);
	//�̺߳���
	static UINT	CommThread(LPVOID pParam);
	//�����ַ�
	static void	ReceiveChar(CSerialPort* pCom, COMSTAT comstat);
	//д�ַ�
	static void	WriteChar(CSerialPort* pCom);

	//add by xiaowei.han 2017-3-25
	static void ReceiveReportData(BYTE* pData,int nDataLength);
	static void ReceiveIdentifyData(BYTE* pData,int nDataLength);
	//end add by xiaowei.han 2017-3-25
	//�߳�
	//�����߳�ָ��
	CWinThread*			m_Thread;
	//�����߳��Ƿ����
	BOOL                m_bIsSuspened;
	//�ٽ���Դ
	CRITICAL_SECTION	m_csCommunicationSync;
	//�����߳����б�־
	BOOL				m_bThreadAlive;
	//д�¼����
	HANDLE				m_hWriteEvent;
	//���ھ��
	HANDLE				m_hComm;
	//�ر��¼����
	HANDLE				m_hShutdownEvent;
	//�¼����飬����һ��д�¼��������¼����ر��¼�
	//һ��Ԫ������һ���¼����������¼��̴߳���˿ڡ�
	//д�¼��ͽ����ַ��¼�λ��overlapped�ṹ�壨m_ov.hEvent����
	//���˿ڹر�ʱ����һ��ͨ�õĹرա�
	HANDLE				m_hEventArray[3];
	//�ṹ��
	//�첽I/O
	OVERLAPPED			m_ov;
	//��ʱ����
	COMMTIMEOUTS		m_CommTimeouts;
	//�豸���ƿ�
	DCB					m_dcb;
	//������ָ��
	CWnd*				m_pOwner;
	//������
	UINT				m_nPortNr;
	//д������
	char*				m_szWriteBuffer;
	DWORD				m_dwCommEvents;
	//д��������С
	DWORD				m_nWriteBufferSize;
	//д���ֽ���
	int m_nWriteSize;
	//���ջ�����
	char m_recvBuffer[WRITE_BUFFER_SIZE];
	//��������
	char m_procBuffer[WRITE_BUFFER_SIZE];
	//дָ��
	int m_nWritePos;
	//��ָ��
	int m_nReadPos;

private:
	//�������������߳̾��
	HANDLE m_HandleDecode;
	//�������������߳�
	static DWORD WINAPI DecodeComDataProc(LPVOID lpParam);
	//�̵߳�ѭ����ʶ
	static bool s_bloop;
};


class CComImple
{
public:
	static CComImple& CreateInstance();
private:
	CComImple(void);
	~CComImple(void);
public:
	//��������ͨ��
	void StartCom(CWnd* pWnd);
	//�رմ���ͨ��
	void StopCom(void);
	//�򴮿ڷ�������
	void SendComData(BYTE* Buffer, int nLength);
private:
	//����ͨ��ʵ��
	CSerialPort m_SerialPort;
	static CComImple s_ComImple;
};

#endif