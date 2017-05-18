
// Edislab Pro.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include <string>
#include <boost/filesystem.hpp>
#include "MainFrm.h"
#include "Edislab ProDoc.h"
#include "Edislab ProView.h"
#include "DumpFileSwitch.h"
#include "Utility.h"
#include "BaseDialog.h"
#include "ComImple.h"
#include "Log.h"
#include "SensorConfig.h"
using std::string;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEdislabProApp

BEGIN_MESSAGE_MAP(CEdislabProApp, CBCGPWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CEdislabProApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	//ON_COMMAND(ID_FILE_NEW, &CBCGPWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CBCGPWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CBCGPWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CEdislabProApp ����

CEdislabProApp::CEdislabProApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Edislab Pro.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CEdislabProApp ����

CEdislabProApp theApp;


// CEdislabProApp ��ʼ��

BOOL CEdislabProApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CBCGPWinApp::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	EnableTaskbarInteraction(FALSE);
	Init();
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4); 
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEdislabProDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CEdislabProView));
	if (!pDocTemplate)
	{
		return FALSE;
	}
	AddDocTemplate(pDocTemplate);
	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
	{
		return FALSE;
	}
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->SetWindowText(_T("Edislab Pro"));
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	SetVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_BLUE);

	//��������ͨ��
	//CComImple::CreateInstance().StartCom(NULL);

	return TRUE;
}

int CEdislabProApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);
	::BCGCBProCleanUp();
	CleanState();
	return CWinApp::ExitInstance();
}

// CEdislabProApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CBaseDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CBaseDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBaseDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CEdislabProApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CEdislabProApp �Զ������/���淽��

void CEdislabProApp::PreLoadState()
{
}

void CEdislabProApp::LoadCustomState()
{
}

void CEdislabProApp::SaveCustomState()
{
}

void CEdislabProApp::Init( void )
{

	//��ʼ����ӡ��־
	std::string strLogDir = Utility::GetExeDirecory() + std::string("\\log\\");

	boost::filesystem::create_directories(strLogDir);


	CLog::CreateInstance().SetLogPath(strLogDir.c_str());
	CLog::CreateInstance().SetLogNamePrefix("Edislab");
#ifdef _DEBUG
	CLog::CreateInstance().SetLogLevel(LOG_DEBUG);
#else
	CLog::CreateInstance().SetLogLevel(LOG_ERROR);
#endif
	//�������ɵ�dump�ļ�·��
	std::string strDumpFilePath = Utility::GetExeDirecory() + std::string("\\Edislab.dmp");
	CDumpFileSwitch::CreateInstance().SetDumpFilePath(strDumpFilePath.c_str());
	CDumpFileSwitch::CreateInstance().OpenSwitch();

	//���ش����������ļ�
	if (!CSensorConfig::CreateInstance().LoadSensorConfig())
	{
		ERROR_LOG("LoadSensorConfig failed!");
	}
}

// CEdislabProApp ��Ϣ�������


