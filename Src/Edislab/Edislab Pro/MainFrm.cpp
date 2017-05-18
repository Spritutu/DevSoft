
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "MainFrm.h"
//#include "LogImpl.h"
#include "Msg.h"
#include "Utility.h"
//#include "GlobalDataManager.h"
#include "SerialPortService.h"
#include "Global.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define HAVE_PICTURE (1)
//Combox������Ŀ��
const int DEFAULT_COMBOX_WIDTH = 75;
//��ʱ�����
const int TIMER_GAP = 1000;
//��ʱ��ID
const int TIMER_ID = 1;
//��ͼ��ĳ��ȺͿ��
const int LARGE_ICON_WIDTH = 32;
const int LARGE_ICON_HEIGHT = 32;
//Сͼ��ĳ��ȺͿ��
const int SMALL_ICON_WIDTH = 16;
const int SMALL_ICON_HEIGHT = 16;
//Ĭ�ϵ�͸��ɫ
const COLORREF TRANS_COLOR = RGB(0,0,0);

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_NOTIFY_ACTIVE_WND_TYPE,&CMainFrame::NotifyActiveWnd)
#if 0
	ON_COMMAND(ID_SELECT_SENSOR, &CMainFrame::HandleSelctSensor)
	ON_COMMAND(ID_COLLECT_PARAM, &CMainFrame::HandleCaptureParam)
	ON_COMMAND(ID_CONFIG_DATA, &CMainFrame::HandleConfigData)

	ON_COMMAND(ID_ADD_TABLE, &CMainFrame::HandleAddTable)
	ON_COMMAND(ID_ADD_IMAGE, &CMainFrame::HandleAddImage)
	ON_COMMAND(ID_ADD_DEVICE, &CMainFrame::HandleAddDevice)
	ON_COMMAND(ID_DEL_ELEMENT, &CMainFrame::HandleDelElement)

	ON_COMMAND(ID_START, &CMainFrame::HandleStart)
	ON_COMMAND(ID_RECORD_POINT, &CMainFrame::HandleRecordPoint)
	ON_COMMAND(ID_ZERO, &CMainFrame::HandleFixedZero)
	ON_COMMAND(ID_CALIBRATION, &CMainFrame::HandleCalibration)
	ON_COMMAND(ID_CANCEL_ZERO_AND_CALIBRATION, &CMainFrame::HandleCancelZeroAndCalibration)

	ON_COMMAND(ID_START_REPLAY, &CMainFrame::HandleStartReplay)
	ON_COMMAND(ID_PAUSE_REPLAY, &CMainFrame::HandlePauseReplay)
	ON_COMMAND(ID_SELECT_ARRAY, &CMainFrame::HandleSelectDataArray)
	ON_COMMAND(ID_SELECT_SPEED, &CMainFrame::HandleSelectSpeed)

	ON_COMMAND(ID_ADD_PAGE, &CMainFrame::HandleAddPage)
	ON_COMMAND(ID_DEL_PAGE, &CMainFrame::HandleDelPage)
	ON_COMMAND(ID_SET_PAGE_NAME, &CMainFrame::HandlePageName)
	ON_COMMAND(ID_GOTO_PAGE, &CMainFrame::HandleGotoPage)

	ON_COMMAND(ID_OPTION, &CMainFrame::HandleOption)
	ON_COMMAND(ID_NUM_STYLE, &CMainFrame::HandleNumStyle)
	ON_COMMAND(ID_WATCH_STYLE, &CMainFrame::HandleWatchStyle)
	ON_COMMAND(ID_NEXT_DATA_COLUMN, &CMainFrame::HandleNextDataColumn)


	ON_COMMAND(ID_OSCILLOSCOPE, &CMainFrame::HandleOscilloscope)
	ON_COMMAND(ID_2D_COMPOSE_AND_DECOMPOSE, &CMainFrame::Handle2DComposeAndDecompose)
#endif
	ON_COMMAND_RANGE(ID_AUTO_SELECT,ID_MANUAL_SELECT,&CMainFrame::HandleRadioClick)
	ON_UPDATE_COMMAND_UI_RANGE(ID_AUTO_SELECT,ID_MANUAL_SELECT,&CMainFrame::OnUpdateRadioButton)
	ON_COMMAND(ID_MENU_ENGLISH, &CMainFrame::OnMenuEnglish)
	ON_UPDATE_COMMAND_UI(ID_MENU_ENGLISH, &CMainFrame::OnUpdateMenuEnglish)
	ON_COMMAND(ID_MENU_CHINESE, &CMainFrame::OnMenuChinese)
	ON_UPDATE_COMMAND_UI(ID_MENU_CHINESE, &CMainFrame::OnUpdateMenuChinese)
	ON_COMMAND(ID_DISPLAY_CURRENT_TIME,&CMainFrame::EnableStatusBar)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��

	ID_STATUS_1,
	//ID_STATUS_2,
	ID_DISPLAY_CURRENT_TIME,
};

// CMainFrame ����/����

CMainFrame::CMainFrame():
m_nCurrentRadioID(0),
m_pDeviceCategory(NULL),
m_pDiagramCategory(NULL),
m_pGridCategory(NULL)
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
#if 0	
	CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
	CBCGPDockManager* pDockManager = GetDockManager();
	if (pDockManager != NULL)
	{
		pDockManager->AdjustBarFrames();
	}
	CBCGPTabbedControlBar::ResetTabs();
	RecalcLayout ();
	RedrawWindow (NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

	CBCGPToolBar::SetSizes(CSize(32,32),CSize(24,24));
	CBCGPToolBar::SetMenuSizes(CSize (24,21),CSize(16,15));
	CBCGPMenuBar::EnableMenuShadows(FALSE);
	if (FALSE == m_wndMenuBar.Create(this))
	{
		CLogImpl::CreateInstance().Error_Log("δ�ܴ����˵���");
		return -1;     
	}
	if (NULL != m_wndMenuBar.GetSafeHwnd())
	{
		m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);
	}
	// ��ֹ�˵����ڼ���ʱ��ý���
	CBCGPPopupMenu::SetForceMenuFocus(FALSE);
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		CLogImpl::CreateInstance().Error_Log("δ�ܴ���������");
		return -1;
	}
	CString strToolBarName;
	BOOL bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);
	// �����û�����Ĺ���������:
	CBCGPToolBar::EnableQuickCustomization();
	if (FALSE == m_wndStatusBar.Create(this))
	{
		CLogImpl::CreateInstance().Error_Log("δ�ܴ���״̬��");
		return -1;
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	// TODO: �������ϣ���������Ͳ˵�����ͣ������ɾ��������
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);
#endif
	LoadRibbonBitmap();
	//����Ribbon����״̬��
	CreateStatusBar();
	//����RibbonBar
	CreateRibbonBar();
	SetTimer(TIMER_ID,TIMER_GAP,NULL);
	if (g_bAutoSelect)
	{	
		CSerialPortService::CreateInstance().StartSerialPortService();
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
}
#endif //_DEBUG

//����RibbonBar
void CMainFrame::CreateRibbonBar( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		m_wndRibbonBar.Create(this);
	}
	m_MainButton.SetImage(IDB_RIBBON_LOGO);
	m_MainButton.SetToolTipText (_T("File"));
	m_MainButton.SetText (_T("\nf"));
	m_wndRibbonBar.SetMainButton(&m_MainButton,CSize(48, 48));
	CBCGPRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(_T("�ļ�"),IDB_RIBBON_BUTTON_SMALL,IDB_RIBBON_BUTTON_LARGE);
	if (NULL != pMainPanel)
	{
		int nIndex = 0;
		pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_NEW, _T("�½�(&N)"),m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_TYPICAL_TEST, _T("����ʵ��"),m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_OPEN, _T("��(&O)"),m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_SAVE, _T("����(&S)"), m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_SAVE_AS, _T("���Ϊ(&A)"),m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_OUTPUT_TEST_REPORT, _T("���ʵ�鱨��"),m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_PRINT, _T("��ӡ(&P)"),m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_PRINT_PREVIEW, _T("��ӡԤ��(&V)"), m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_APP_EXIT, _T("�˳�(&E)"), m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
	}
	pMainPanel->EnableCommandSearch (TRUE, _T("Search Command"), _T("Q"), 150);
	AddStartCategory();
	AddExternCategory();
	AddPageCategory();
	AddDeviceCategory();
	AddGridCategory();
	AddDiagramCategory();
	AddAdditionalContent();
}

//���ӿ�ʼ
void CMainFrame::AddStartCategory( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}

	int nBaseImageIndex = 0;
	CBCGPRibbonCategory* pStartCategory = m_wndRibbonBar.AddCategory(_T("��ʼ"),IDB_RIBBON_START_SMALL,IDB_RIBBON_START_LARGE);
	if (NULL != pStartCategory)
	{
		//��Ӵ��������������
		AddSensorDataPanel(pStartCategory,nBaseImageIndex);
		//���ҳ��Ԫ���������
		AddStartPageElementPanel(pStartCategory,nBaseImageIndex);
		//��ӽ����������
		AddRunTestPanel(pStartCategory,nBaseImageIndex);
		//��ӻط�������塢
		AddReplyTestPanel(pStartCategory,nBaseImageIndex);
	}


}

//������չ
void CMainFrame::AddExternCategory( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}
	CBCGPRibbonCategory* pExternCategory = m_wndRibbonBar.AddCategory(_T("��չ"),IDB_RIBBON_START_SMALL,IDB_RIBBON_START_LARGE);
	if (NULL != pExternCategory)
	{
		AddOtherSoftWarePanel(pExternCategory);
	}
}

//����ҳ��
void CMainFrame::AddPageCategory( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}
	int nBaseImageIndex = 0;
	CBCGPRibbonCategory* pExternCategory = m_wndRibbonBar.AddCategory(_T("ҳ��"),IDB_RIBBON_PAGE_ELEMENT_SMALL,IDB_RIBBON_PAGE_ELEMENT_LARGE);
	if (NULL != pExternCategory)
	{
		AddPagePanel(pExternCategory,nBaseImageIndex);
		AddPageElementPanel(pExternCategory,nBaseImageIndex);
	}
}

void CMainFrame::HandleRadioClick( UINT nID )
{
	m_nCurrentRadioID = nID;
}

void CMainFrame::AddSensorDataPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{

	if (NULL != pCategory)
	{
		CBCGPRibbonPanel* pSensorAndDataPanel = pCategory->AddPanel (_T("������������"));

		if (NULL != pSensorAndDataPanel)
		{
#ifdef HAVE_PICTURE
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_SELECT_SENSOR, _T("ѡ������"),m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_SELECT_SENSOR, _T("ѡ������"),1,1);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("ѡ������"));
				pBtn->SetAlwaysLargeImage();
				pSensorAndDataPanel->Add(pBtn);
			}

			pSensorAndDataPanel->AddSeparator();
			CBCGPRibbonRadioButton* pRadioBtn = new CBCGPRibbonRadioButton(ID_AUTO_SELECT, _T("�Զ�ʶ��"));
			if (NULL != pRadioBtn)
			{
				pRadioBtn->SetToolTipText(_T("�Զ�ʶ��"));
				pSensorAndDataPanel->Add(pRadioBtn);
			}
			
			pRadioBtn = new CBCGPRibbonRadioButton(ID_MANUAL_SELECT, _T("�ֶ�ѡ��"));
			if (NULL != pRadioBtn)
			{
				pRadioBtn->SetToolTipText(_T("�ֶ�ѡ��"));
				pSensorAndDataPanel->Add(pRadioBtn);
			}
			
			pSensorAndDataPanel->AddSeparator();
#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_COLLECT_PARAM, _T("�ɼ�����"),m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_COLLECT_PARAM, _T("�ɼ�����"),1,1);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("�ɼ�����"));
				pBtn->SetAlwaysLargeImage ();
				pSensorAndDataPanel->Add(pBtn);
			}
#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_CONFIG_DATA, _T("��������"),m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_CONFIG_DATA, _T("��������"),1,1);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("��������"));
				pBtn->SetAlwaysLargeImage ();
				pSensorAndDataPanel->Add(pBtn);
			}
		}
	}
}

void CMainFrame::AddStartPageElementPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL != pCategory)
	{
		CBCGPRibbonPanel* pPageElementPanel = pCategory->AddPanel (_T("ҳ��Ԫ��"));

		if (NULL != pPageElementPanel)
		{
#ifdef HAVE_PICTURE
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_TABLE, _T("��ӱ��"),m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_TABLE, _T("��ӱ��"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("��ӱ��"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}

#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_ADD_IMAGE, _T("���ͼ"),m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_ADD_IMAGE, _T("���ͼ"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("���ͼ"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}


#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_ADD_DEVICE, _T("����Ǳ�"),m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_ADD_DEVICE, _T("����Ǳ�"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("����Ǳ�"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}


#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_DEL_ELEMENT, _T("ɾ��Ԫ��"),m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_DEL_ELEMENT, _T("ɾ��Ԫ��"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("ɾ��Ԫ��"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}
		}
	}
}

//���ҳ��Ԫ��
void CMainFrame::AddPageElementPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{


	if (NULL != pCategory)
	{
		CBCGPRibbonPanel* pPageElementPanel = pCategory->AddPanel (_T("ҳ��Ԫ��"));

		if (NULL != pPageElementPanel)
		{
#ifdef HAVE_PICTURE
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_TABLE, _T("��ӱ��"),m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_TABLE, _T("��ӱ��"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("��ӱ��"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}

#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_ADD_IMAGE, _T("���ͼ"),m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_ADD_IMAGE, _T("���ͼ"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("���ͼ"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}


#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_ADD_DEVICE, _T("����Ǳ�"),m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_ADD_DEVICE, _T("����Ǳ�"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("����Ǳ�"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}


#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_DEL_ELEMENT, _T("ɾ��Ԫ��"),m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_DEL_ELEMENT, _T("ɾ��Ԫ��"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("ɾ��Ԫ��"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}
		}
	}
}

void CMainFrame::AddRunTestPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{

	if (NULL != pCategory)
	{
		CBCGPRibbonPanel* pRunTestPanel = pCategory->AddPanel (_T("����ʵ��"));

		if (NULL != pRunTestPanel)
		{
#ifdef HAVE_PICTURE
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_START, _T("��ʼ"), m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_START, _T("��ʼ"),5,5);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("��ʼ"));
				pBtn->SetAlwaysLargeImage ();
				pRunTestPanel->Add(pBtn);
			}

#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_RECORD_POINT, _T("�ǵ�"), m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_RECORD_POINT, _T("�ǵ�"),5,5);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("�ǵ�"));
				pBtn->SetAlwaysLargeImage ();
				pRunTestPanel->Add(pBtn);
			}

			pRunTestPanel->AddSeparator();
#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_ZERO, _T("����"), m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_ZERO, _T("����"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("����"));
				pBtn->SetAlwaysLargeImage ();
				pRunTestPanel->Add(pBtn);
			}


#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_CALIBRATION, _T("У׼"), m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_CALIBRATION, _T("У׼"),6,6);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("У׼"));
				pBtn->SetAlwaysLargeImage ();
				pRunTestPanel->Add(pBtn);
			}

#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_CANCEL_ZERO_AND_CALIBRATION, _T("ȡ�������У׼"), m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_CANCEL_ZERO_AND_CALIBRATION, _T("ȡ�������У׼"),6,6);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("ȡ�������У׼"));
				pBtn->SetAlwaysLargeImage ();
				pRunTestPanel->Add(pBtn);
			}

		}
	}

}

void CMainFrame::AddReplyTestPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL != pCategory)
	{
		CBCGPRibbonPanel* pReplayTestPanel = pCategory->AddPanel (_T("�ط�ʵ��"));

		if (NULL != pReplayTestPanel)
		{
#ifdef HAVE_PICTURE
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_START_REPLAY, _T("��ʼ�ط�"), m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_START_REPLAY, _T("��ʼ�ط�"),7,7);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("��ʼ�ط�"));
				pBtn->SetAlwaysLargeImage ();
				pReplayTestPanel->Add(pBtn);
			}

#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_PAUSE_REPLAY, _T("��ͣ�ط�"), m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_PAUSE_REPLAY, _T("��ͣ�ط�"),5,5);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("��ͣ�ط�"));
				pBtn->SetAlwaysLargeImage ();
				pReplayTestPanel->Add(pBtn);
			}

			//��ӻط�����
			CBCGPRibbonLabel* pLabel = new CBCGPRibbonLabel(_T("�ط�����"));
			if (NULL != pLabel)
			{
				pReplayTestPanel->Add(pLabel);
			}


			CBCGPRibbonComboBox* pCombox = new CBCGPRibbonComboBox(ID_SELECT_ARRAY,FALSE,DEFAULT_COMBOX_WIDTH);

			if (NULL != pCombox)
			{
				pCombox->SetPrompt(_T("������"));

				pCombox->AddItem(_T("��ǰ"));

				pReplayTestPanel->Add(pCombox);
			}

			pCombox = new CBCGPRibbonComboBox(ID_SELECT_SPEED,FALSE,DEFAULT_COMBOX_WIDTH);
			if (NULL != pCombox)
			{

				pCombox->SetPrompt(_T("�ٶ�"));

				CString strElement = _T("1/100x");;
				pCombox->AddItem(strElement);

				strElement = _T("1/50x");;
				pCombox->AddItem(strElement);

				strElement = _T("1/10x");;
				pCombox->AddItem(strElement);

				strElement = _T("1/6x");;
				pCombox->AddItem(strElement);

				strElement = _T("1/4x");;
				pCombox->AddItem(strElement);

				strElement = _T("1x");;
				pCombox->AddItem(strElement);

				strElement = _T("2x");;
				pCombox->AddItem(strElement);

				strElement = _T("4x");;
				pCombox->AddItem(strElement);

				strElement = _T("6x");;
				pCombox->AddItem(strElement);

				strElement = _T("8x");;
				pCombox->AddItem(strElement);

				strElement = _T("10x");;
				pCombox->AddItem(strElement);

				strElement = _T("50x");;
				pCombox->AddItem(strElement);

				strElement = _T("100x");;
				pCombox->AddItem(strElement);

				pReplayTestPanel->Add(pCombox);
			}
		
		}
	}
}

void CMainFrame::OnUpdateRadioButton( CCmdUI* pCmdUI )
{
	if (NULL != pCmdUI)
	{
		pCmdUI->SetRadio(m_nCurrentRadioID == pCmdUI->m_nID);
	}
	
}

void CMainFrame::AddOtherSoftWarePanel( CBCGPRibbonCategory* pCategory )
{

	if (NULL != pCategory)
	{
		CBCGPRibbonPanel* pOtherSoftWarePanel = pCategory->AddPanel (_T("�������"));

		if (NULL != pOtherSoftWarePanel)
		{
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_OSCILLOSCOPE,_T("ʾ����"));

			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("ʾ����"));
				pOtherSoftWarePanel->Add(pBtn);
			}

			pBtn = new CBCGPRibbonButton(ID_2D_COMPOSE_AND_DECOMPOSE,_T("��ά�˶��ϳ���ֽ�"));
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("��ά�˶��ϳ���ֽ�"));
				pOtherSoftWarePanel->Add(pBtn);
			}

		}
	}
}


void CMainFrame::AddPagePanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL != pCategory)
	{
		CBCGPRibbonPanel* pPagePanel = pCategory->AddPanel (_T("ҳ��"));

		if (NULL != pPagePanel)
		{
#ifdef HAVE_PICTURE
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_PAGE, _T("���ҳ��"), m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_PAGE, _T("���ҳ��"),1,1);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("���ҳ��"));
				pBtn->SetAlwaysLargeImage ();
				pPagePanel->Add(pBtn);
			}

#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_DEL_PAGE, _T("ɾ��ҳ��"), m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_DEL_PAGE, _T("ɾ��ҳ��"),2,2);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("ɾ��ҳ��"));
				pBtn->SetAlwaysLargeImage ();
				pPagePanel->Add(pBtn);
			}

#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_SET_PAGE_NAME, _T("ҳ������"), m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_SET_PAGE_NAME, _T("ҳ������"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("ҳ������"));
				pBtn->SetAlwaysLargeImage ();
				pPagePanel->Add(pBtn);
			}


#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_GOTO_PAGE, _T("��תҳ��"), m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_GOTO_PAGE, _T("��תҳ��"),3,3);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("��תҳ��"));
				pBtn->SetAlwaysLargeImage ();
				pPagePanel->Add(pBtn);
			}
		}
	}
}

//�����Ǳ�
void CMainFrame::AddDeviceCategory( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}

	int nBaseImageIndex = 0;
	CBCGPRibbonCategory* pDeviceCategory = m_wndRibbonBar.AddCategory(_T("�Ǳ�"),IDB_RIBBON_DEVICE_SMALL,IDB_RIBBON_DEVICE_LARGE);
	if (NULL != pDeviceCategory)
	{
		m_pDeviceCategory = pDeviceCategory;
		AddOptionPanel(pDeviceCategory,nBaseImageIndex);
		AddFixedPanel(pDeviceCategory,nBaseImageIndex);
	}
}

//���ӱ��
void CMainFrame::AddGridCategory( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}

	int nBaseImageIndex = 0;
	CBCGPRibbonCategory* pGridCategory = m_wndRibbonBar.AddCategory(_T("���"),IDB_RIBBON_GRID_SMALL,IDB_RIBBON_GRID_LARGE);
	if (NULL != pGridCategory)
	{
		m_pGridCategory = pGridCategory;
		AddGridOptionPanel(pGridCategory,nBaseImageIndex);
		AddGridClipBoardPanel(pGridCategory,nBaseImageIndex);
		AddGridCellPanel(pGridCategory,nBaseImageIndex);
		AddGridDataPanel(pGridCategory,nBaseImageIndex);
		AddGridReportPanel(pGridCategory,nBaseImageIndex);
	}
}

//����ͼ��
void CMainFrame::AddDiagramCategory( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}

	int nBaseImageIndex = 0;
	CBCGPRibbonCategory* pDigramCategory = m_wndRibbonBar.AddCategory(_T("ͼ��"),IDB_RIBBON_DIAGRAM_SMALL,IDB_RIBBON_DIAGRAM_LARGE);
	if (NULL != pDigramCategory)
	{
		m_pDiagramCategory = pDigramCategory;
		AddDiagramOptionPanel(pDigramCategory,nBaseImageIndex);
		AddDiagramZoomPanel(pDigramCategory,nBaseImageIndex);
		AddDiagramAnalyzePanel(pDigramCategory,nBaseImageIndex);
		AddDiagramReportPanel(pDigramCategory,nBaseImageIndex);
	}
}

//ѡ�����
void CMainFrame::AddOptionPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pOptionPanel = pCategory->AddPanel (_T("ѡ��"));

	if (NULL != pOptionPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_OPTION, _T("ѡ��"), m_RibbonDevicePageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDevicePageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_OPTION, _T("ѡ��"),1,1);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("ѡ��"));
			pBtn->SetAlwaysLargeImage ();
			pOptionPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_NUM_STYLE, _T("��ֵ���"), m_RibbonDevicePageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDevicePageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_NUM_STYLE, _T("��ֵ���"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("��ֵ���"));
			pBtn->SetAlwaysLargeImage ();
			pOptionPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_WATCH_STYLE, _T("���̷��"), m_RibbonDevicePageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDevicePageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_WATCH_STYLE, _T("���̷��"),4,4);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("���̷��"));
			pBtn->SetAlwaysLargeImage ();
			pOptionPanel->Add(pBtn);
		}


#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_NEXT_DATA_COLUMN, _T("��һ������"), m_RibbonDevicePageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDevicePageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_NEXT_DATA_COLUMN, _T("��һ������"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("��һ������"));
			pBtn->SetAlwaysLargeImage ();
			pOptionPanel->Add(pBtn);
		}
	}
	
}

//У׼���
void CMainFrame::AddFixedPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pFixedPanel = pCategory->AddPanel (_T("У׼"));

	if (NULL != pFixedPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ZERO, _T("����"), m_RibbonDevicePageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDevicePageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ZERO, _T("����"),0,0);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("����"));
			pBtn->SetAlwaysLargeImage ();
			pFixedPanel->Add(pBtn);
		}


#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_CALIBRATION, _T("У׼"), m_RibbonDevicePageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDevicePageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_CALIBRATION, _T("У׼"),1,1);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("У׼"));
			pBtn->SetAlwaysLargeImage ();
			pFixedPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_CANCEL_ZERO_AND_CALIBRATION, _T("ȡ�������У׼"), m_RibbonDevicePageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDevicePageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_CANCEL_ZERO_AND_CALIBRATION, _T("ȡ�������У׼"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("ȡ�������У׼"));
			pBtn->SetAlwaysLargeImage ();
			pFixedPanel->Add(pBtn);
		}
	}

}

//ѡ�����
void CMainFrame::AddGridOptionPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pGridOptionPanel = pCategory->AddPanel(_T("ѡ��"));

	if (NULL != pGridOptionPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_OPTION, _T("ѡ��"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_OPTION, _T("ѡ��"),0,0);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("ѡ��"));
			pBtn->SetAlwaysLargeImage();
			pGridOptionPanel->Add(pBtn);
		}
	}

}

//���������
void CMainFrame::AddGridClipBoardPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{

	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pGridClipBoardPanel = pCategory->AddPanel(_T("������"));

	if (NULL != pGridClipBoardPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_COPY, _T("����"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_COPY, _T("����"),1,1);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("����"));
			pBtn->SetAlwaysLargeImage();
			pGridClipBoardPanel->Add(pBtn);
		}


#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_PASTE, _T("���"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_PASTE, _T("���"),1,1);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("���"));
			pBtn->SetAlwaysLargeImage();
			pGridClipBoardPanel->Add(pBtn);
		}
	}
}

//�������
void CMainFrame::AddGridCellPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pGridCellPanel = pCategory->AddPanel(_T("����"));

	if (NULL != pGridCellPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_FIRST_ROW, _T("��һ��"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_FIRST_ROW, _T("��һ��"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("��һ��"));
			pBtn->SetAlwaysLargeImage();
			pGridCellPanel->Add(pBtn);
		}


#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_LAST_ROW, _T("�����"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_LAST_ROW, _T("�����"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("�����"));
			pBtn->SetAlwaysLargeImage();
			pGridCellPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_INSERT_ROW, _T("������"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_INSERT_ROW, _T("������"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("������"));
			pBtn->SetAlwaysLargeImage();
			pGridCellPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_DEL_CELL, _T("ɾ������"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_DEL_CELL, _T("ɾ������"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("ɾ������"));
			pBtn->SetAlwaysLargeImage();
			pGridCellPanel->Add(pBtn);
		}

	}
}

//�������
void CMainFrame::AddGridDataPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pGridDataPanel = pCategory->AddPanel(_T("����"));

	if (NULL != pGridDataPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_DATA_COLUMN, _T("����������"),m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_DATA_COLUMN, _T("����������"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("����������"));
			pBtn->SetAlwaysLargeImage();
			pGridDataPanel->Add(pBtn);
		}


#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_YIELD_DATA, _T("��������"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_YIELD_DATA, _T("��������"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("��������"));
			pBtn->SetAlwaysLargeImage();
			pGridDataPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_CALCULATE, _T("����"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_CALCULATE, _T("����"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("����"));
			pBtn->SetAlwaysLargeImage();
			pGridDataPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_CLEAR_CHILD_DATA, _T("���������"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_CLEAR_CHILD_DATA, _T("���������"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("���������"));
			pBtn->SetAlwaysLargeImage();
			pGridDataPanel->Add(pBtn);
		}
	}
}

//�������
void CMainFrame::AddGridReportPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pGridReportPanel = pCategory->AddPanel(_T("����"));

	if (NULL != pGridReportPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_SAVE_EXCEL, _T("�洢ΪExcel"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_SAVE_EXCEL, _T("�洢ΪExcel"),4,4);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("�洢ΪExcel"));
			pBtn->SetAlwaysLargeImage();
			pGridReportPanel->Add(pBtn);
		}


#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_PRINT, _T("��ӡ"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_PRINT, _T("��ӡ"),4,4);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("��ӡ"));
			pBtn->SetAlwaysLargeImage();
			pGridReportPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_PRINT_PREVIEW, _T("��ӡԤ��"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_PRINT_PREVIEW, _T("��ӡԤ��"),4,4);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("��ӡԤ��"));
			pBtn->SetAlwaysLargeImage();
			pGridReportPanel->Add(pBtn);
		}
	}
}

//����ѡ�����
void CMainFrame::AddDiagramOptionPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}
	CBCGPRibbonPanel* pDiagramOptionPanel = pCategory->AddPanel(_T("ѡ��"));

	if (NULL != pDiagramOptionPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_OPTION, _T("ѡ��"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_OPTION, _T("ѡ��"),0,0);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("ѡ��"));
			pBtn->SetAlwaysLargeImage();
			pDiagramOptionPanel->Add(pBtn);
		}

		//���ӷ�����
		pDiagramOptionPanel->AddSeparator();

		CBCGPRibbonRadioButton* pRadioBtn = new CBCGPRibbonRadioButton(ID_DRAG_MODE, _T("�϶�ģʽ"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("�϶�ģʽ"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

		pRadioBtn = new CBCGPRibbonRadioButton(ID_SELECT_MODE, _T("ѡ��ģʽ"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("ѡ��ģʽ"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

		pDiagramOptionPanel->AddSeparator();

		pRadioBtn = new CBCGPRibbonRadioButton(ID_AUTO_SCROLL, _T("�Զ�����"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("�Զ�����"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

		pRadioBtn = new CBCGPRibbonRadioButton(ID_AUTO_ZOOM, _T("�Զ�����"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("�Զ�����"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

		pRadioBtn = new CBCGPRibbonRadioButton(ID_NO_SCROLL, _T("������"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("������"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

		pDiagramOptionPanel->AddSeparator();

		pRadioBtn = new CBCGPRibbonRadioButton(ID_LINE, _T("����"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("����"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

		pRadioBtn = new CBCGPRibbonRadioButton(ID_POINT, _T("��"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("��"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

		pRadioBtn = new CBCGPRibbonRadioButton(ID_POINT_TO_LINE, _T("������"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("������"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

	}

}

//�����������
void CMainFrame::AddDiagramZoomPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pDiagramZoomPanel = pCategory->AddPanel(_T("����"));

	if (NULL != pDiagramZoomPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_100_PERCENT, _T("100%"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_100_PERCENT, _T("100%"),1,1);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("100%"));
			pBtn->SetAlwaysLargeImage();
			pDiagramZoomPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_ZOOM_IN, _T("�Ŵ�"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_ZOOM_IN, _T("�Ŵ�"),1,1);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("�Ŵ�"));
			pBtn->SetAlwaysLargeImage();
			pDiagramZoomPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_ZOOM_OUT, _T("��С"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_ZOOM_OUT, _T("��С"),1,1);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("��С"));
			pBtn->SetAlwaysLargeImage();
			pDiagramZoomPanel->Add(pBtn);
		}
	}
}

//���ӷ������
void CMainFrame::AddDiagramAnalyzePanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pDiagramAnalyzePanel = pCategory->AddPanel(_T("����"));

	if (NULL != pDiagramAnalyzePanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_VIEW, _T("�鿴"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_VIEW, _T("�鿴"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("�鿴"));
			pBtn->SetAlwaysLargeImage();
			pDiagramAnalyzePanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_TANGENT, _T("����"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_TANGENT, _T("����"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("����"));
			pBtn->SetAlwaysLargeImage();
			pDiagramAnalyzePanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_INTEGRAL, _T("����"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_INTEGRAL, _T("����"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("����"));
			pBtn->SetAlwaysLargeImage();
			pDiagramAnalyzePanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_STATISTICS, _T("ͳ��"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_STATISTICS, _T("ͳ��"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("ͳ��"));
			pBtn->SetAlwaysLargeImage();
			pDiagramAnalyzePanel->Add(pBtn);
		}


#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_LINEAR_FITTING, _T("ֱ�����"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_LINEAR_FITTING, _T("ֱ�����"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("ֱ�����"));
			pBtn->SetAlwaysLargeImage();
			pDiagramAnalyzePanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_FITTING, _T("���"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_FITTING, _T("���"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("���"));
			pBtn->SetAlwaysLargeImage();
			pDiagramAnalyzePanel->Add(pBtn);
		}
	}
}

//���ӱ������
void CMainFrame::AddDiagramReportPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pDiagramReportPanel = pCategory->AddPanel(_T("����"));

	if (NULL != pDiagramReportPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_SAVE_IMAGE, _T("����ͼƬ"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_SAVE_IMAGE, _T("����ͼƬ"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("����ͼƬ"));
			pBtn->SetAlwaysLargeImage();
			pDiagramReportPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_EXPORT_EXPERIMENT_REPORT, _T("������ʵ�鱨��"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_EXPORT_EXPERIMENT_REPORT, _T("������ʵ�鱨��"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("������ʵ�鱨��"));
			pBtn->SetAlwaysLargeImage();
			pDiagramReportPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_PRINT, _T("��ӡ"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_PRINT, _T("��ӡ"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("��ӡ"));
			pBtn->SetAlwaysLargeImage();
			pDiagramReportPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_PRINT_PREVIEW, _T("��ӡԤ��"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_PRINT_PREVIEW, _T("��ӡԤ��"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("��ӡԤ��"));
			pBtn->SetAlwaysLargeImage();
			pDiagramReportPanel->Add(pBtn);
		}
	}
}

void CMainFrame::AddAdditionalContent( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}

	CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_EXPERIMENT_GUIDE, _T("ʵ��ָ��"));

	if (NULL != pBtn)
	{
		pBtn->SetToolTipText(_T("ʵ��ָ��"));
		m_wndRibbonBar.AddToTabs(pBtn);
	}

	//�������

	pBtn = new CBCGPRibbonButton((UINT)-1,_T("����"));

	if (NULL != pBtn)
	{
		pBtn->SetMenu(IDR_MENU_LANGUAGE,TRUE);
		pBtn->SetToolTipText(_T("����"));
		m_wndRibbonBar.AddToTabs(pBtn);
	}
	

	pBtn = new CBCGPRibbonButton (ID_APP_ABOUT,_T("����"));

	if (NULL != pBtn)
	{
		pBtn->SetToolTipText(_T("����"));
		m_wndRibbonBar.AddToTabs(pBtn);
	}	
}

void CMainFrame::CreateStatusBar( void )
{

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		//CLogImpl::CreateInstance().Error_Log("Create Status Bar failed.");
		return;
	}

	CTime CurrentTime = CTime::GetCurrentTime();
	CString strTime = CurrentTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	int nIndex = m_wndStatusBar.CommandToIndex(ID_DISPLAY_CURRENT_TIME);
	m_wndStatusBar.SetPaneText(nIndex,strTime);
	m_wndStatusBar.SetPaneWidth(nIndex,110);
	m_wndStatusBar.SetPaneTextColor(nIndex,RGB(255,0,0));
}

/*****************************************************************************
@FunctionName : ����Ribbon��Ҫ��λͼ��Դ
@FunctionDescription : ����Ribbon��Ҫ��λͼ��Դ
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CMainFrame::LoadRibbonBitmap( void )
{
	//���ؿ�ʼҳ��ͼ��
	LoadStartPageRibbonBitmap();
	//����ҳ��ҳ��ͼ��
	LoadPageRibbonBitmap();
	//���ر��ҳ���ͼ��
	LoadGridPageRibbonBitmap();
	//�����Ǳ�ҳ���ͼ��
	LoadDevicePageRibbonBitmap();
	//����ͼ��ҳ���ͼ��
	LoadDiagramPageRibbonBitmap();
	//����MainCategoryͼ��
	LoadMainCategoryBitmap();
}

/*****************************************************************************
@FunctionName : ���ؿ�ʼRibbon����ͼ��
@FunctionDescription : ���ؿ�ʼRibbon����ͼ��
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CMainFrame::LoadStartPageRibbonBitmap( void )
{
	//Ribbon��ʼ�����λͼ
	std::string strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonStartLarge.bmp");
	HBITMAP hLargeBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hLargeBitmap)
	{
		m_RibbonStartPageLargeBitmap.SetImageSize(CSize(LARGE_ICON_WIDTH,LARGE_ICON_HEIGHT));
		m_RibbonStartPageLargeBitmap.AddImage(hLargeBitmap,FALSE);
		m_RibbonStartPageLargeBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hLargeBitmap);
	}
	strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonStartSmall.bmp");
	HBITMAP hSmallBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hSmallBitmap)
	{
		m_RibbonStartPageSmallBitmap.SetImageSize(CSize(SMALL_ICON_WIDTH,SMALL_ICON_HEIGHT));
		m_RibbonStartPageSmallBitmap.AddImage(hSmallBitmap,FALSE);
		m_RibbonStartPageSmallBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hSmallBitmap);
	}
}

/*****************************************************************************
@FunctionName : ����ҳ��Ribbon����ͼ��
@FunctionDescription : ����ҳ��Ribbon����ͼ��
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CMainFrame::LoadPageRibbonBitmap( void )
{
	//Ribbonҳ������λͼ
	std::string strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonPageElementLarge .bmp");
	HBITMAP hLargeBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hLargeBitmap)
	{
		m_RibbonPageLargeBitmap.SetImageSize(CSize(LARGE_ICON_WIDTH,LARGE_ICON_HEIGHT));
		m_RibbonPageLargeBitmap.AddImage(hLargeBitmap,FALSE);
		m_RibbonPageLargeBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hLargeBitmap);
	}
	strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonPageElementSmall.bmp");
	HBITMAP hSmallBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hSmallBitmap)
	{
		m_RibbonPageSmallBitmap.SetImageSize(CSize(SMALL_ICON_WIDTH,SMALL_ICON_HEIGHT));
		m_RibbonPageSmallBitmap.AddImage(hSmallBitmap,FALSE);
		m_RibbonPageSmallBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hSmallBitmap);
	}
}

/*****************************************************************************
@FunctionName : ���ر��Ribbon����ͼ��
@FunctionDescription : ����ҳ��Ribbon����ͼ��
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CMainFrame::LoadGridPageRibbonBitmap( void )
{
	//���Ribbonҳ������λͼ
	std::string strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonGridPageLarge.bmp");
	HBITMAP hLargeBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hLargeBitmap)
	{
		m_RibbonGridPageLargeBitmap.SetImageSize(CSize(LARGE_ICON_WIDTH,LARGE_ICON_HEIGHT));
		m_RibbonGridPageLargeBitmap.AddImage(hLargeBitmap,FALSE);
		m_RibbonGridPageLargeBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hLargeBitmap);
	}
	strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonGridPageSmall.bmp");
	HBITMAP hSmallBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hSmallBitmap)
	{
		m_RibbonGridPageSmallBitmap.SetImageSize(CSize(SMALL_ICON_WIDTH,SMALL_ICON_HEIGHT));
		m_RibbonGridPageSmallBitmap.AddImage(hSmallBitmap,FALSE);
		m_RibbonGridPageSmallBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hSmallBitmap);
	}
}

/*****************************************************************************
@FunctionName : �����Ǳ�Ribbon����ͼ��
@FunctionDescription : ����ҳ��Ribbon����ͼ��
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CMainFrame::LoadDevicePageRibbonBitmap( void )
{
	//�Ǳ�Ribbonҳ������λͼ
	std::string strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonDevicePageLarge.bmp");
	HBITMAP hLargeBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hLargeBitmap)
	{
		m_RibbonDevicePageLargeBitmap.SetImageSize(CSize(LARGE_ICON_WIDTH,LARGE_ICON_HEIGHT));
		m_RibbonDevicePageLargeBitmap.AddImage(hLargeBitmap,FALSE);
		m_RibbonDevicePageLargeBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hLargeBitmap);
	}
	strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonGridPageSmall.bmp");
	HBITMAP hSmallBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hSmallBitmap)
	{
		m_RibbonDevicePageSmallBitmap.SetImageSize(CSize(SMALL_ICON_WIDTH,SMALL_ICON_HEIGHT));
		m_RibbonDevicePageSmallBitmap.AddImage(hSmallBitmap,FALSE);
		m_RibbonDevicePageSmallBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hSmallBitmap);
	}
}

/*****************************************************************************
@FunctionName : ����ͼ��Ribbon����ͼ��
@FunctionDescription : ����ҳ��Ribbon����ͼ��
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CMainFrame::LoadDiagramPageRibbonBitmap( void )
{
	//�Ǳ�Ribbonҳ������λͼ
	std::string strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonDiagramPageLarge.bmp");
	HBITMAP hLargeBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hLargeBitmap)
	{
		m_RibbonDiagramPageLargeBitmap.SetImageSize(CSize(LARGE_ICON_WIDTH,LARGE_ICON_HEIGHT));
		m_RibbonDiagramPageLargeBitmap.AddImage(hLargeBitmap,FALSE);
		m_RibbonDiagramPageLargeBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hLargeBitmap);
	}
	strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonDiagramPageSmall.bmp");
	HBITMAP hSmallBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hSmallBitmap)
	{
		m_RibbonDiagramPageSmallBitmap.SetImageSize(CSize(SMALL_ICON_WIDTH,SMALL_ICON_HEIGHT));
		m_RibbonDiagramPageSmallBitmap.AddImage(hSmallBitmap,FALSE);
		m_RibbonDiagramPageSmallBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hSmallBitmap);
	}
}

void CMainFrame::LoadMainCategoryBitmap( void )
{
	//�Ǳ�Ribbonҳ������λͼ
	std::string strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\MainCategoryLarge.bmp");
	HBITMAP hLargeBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hLargeBitmap)
	{
		m_MainCategoryLargeBitmap.SetImageSize(CSize(LARGE_ICON_WIDTH,LARGE_ICON_HEIGHT));
		m_MainCategoryLargeBitmap.AddImage(hLargeBitmap,FALSE);
		//m_MainCategoryLargeBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hLargeBitmap);
	}
	strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\MainCategorySmall.bmp");
	HBITMAP hSmallBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hSmallBitmap)
	{
		m_MainCategorySmallBitmap.SetImageSize(CSize(SMALL_ICON_WIDTH,SMALL_ICON_HEIGHT));
		m_MainCategorySmallBitmap.AddImage(hSmallBitmap,FALSE);
		//m_MainCategorySmallBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hSmallBitmap);
	}

}

void CMainFrame::OnMenuEnglish()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnUpdateMenuEnglish(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CMainFrame::OnMenuChinese()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnUpdateMenuChinese(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if (NULL != pCmdUI)
	{
		pCmdUI->SetCheck(TRUE);
	}
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CBCGPFrameWnd::OnCreateClient(lpcs,pContext);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFrameWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ


	if (nIDEvent == TIMER_ID)
	{

		CTime CurrentTime = CTime::GetCurrentTime();
		CString strTime = CurrentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		int nIndex = m_wndStatusBar.CommandToIndex(ID_DISPLAY_CURRENT_TIME);
		m_wndStatusBar.SetPaneText(nIndex,strTime,TRUE);

		//ˢ��Grid
		//GLOBAL_DATA.SetChanged();
		//GLOBAL_DATA.Notify();
	}

	CBCGPFrameWnd::OnTimer(nIDEvent);
}


void CMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(TIMER_ID);

	CSerialPortService::CreateInstance().StopSerialPortService();
	CBCGPFrameWnd::OnClose();
}

LRESULT CMainFrame::NotifyActiveWnd( WPARAM wp,LPARAM lp )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return 0L;
	}
	
	//��ȡ��������
	int nActiveType = (int)wp;

	switch (nActiveType)
	{
		//���
	case 0:
		{
			ShowRibbonCatagory(m_pGridCategory,true);
			ShowRibbonCatagory(m_pDeviceCategory);
			ShowRibbonCatagory(m_pDiagramCategory);

			CBCGPRibbonCategory* pActiveCategory = m_wndRibbonBar.GetActiveCategory();

			if (pActiveCategory == m_pDeviceCategory || pActiveCategory == m_pDiagramCategory)
			{
				m_wndRibbonBar.SetActiveCategory(m_pGridCategory);
			}
		}
		break;
		//�Ǳ�
	case 1:
		{
			ShowRibbonCatagory(m_pGridCategory);
			ShowRibbonCatagory(m_pDeviceCategory,true);
			ShowRibbonCatagory(m_pDiagramCategory);

			CBCGPRibbonCategory* pActiveCategory = m_wndRibbonBar.GetActiveCategory();

			if (pActiveCategory == m_pGridCategory || pActiveCategory == m_pDiagramCategory)
			{
				m_wndRibbonBar.SetActiveCategory(m_pDeviceCategory);
			}
		}
		break;
		//ͼ��
	case 2:
		{
			ShowRibbonCatagory(m_pGridCategory);
			ShowRibbonCatagory(m_pDeviceCategory);
			ShowRibbonCatagory(m_pDiagramCategory,true);	

			CBCGPRibbonCategory* pActiveCategory = m_wndRibbonBar.GetActiveCategory();

			if (pActiveCategory == m_pGridCategory || pActiveCategory == m_pDeviceCategory)
			{
				m_wndRibbonBar.SetActiveCategory(m_pDiagramCategory);
			}
		}
		break;
	default:
		break;
	}

	m_wndRibbonBar.RecalcLayout();
	return 0L;
}

void CMainFrame::ShowRibbonCatagory( CBCGPRibbonCategory* pCatagory,bool bShow /*= false*/ )
{

	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}
	int nCategoryNum = m_wndRibbonBar.GetCategoryCount();

	if (NULL != pCatagory)
	{
		int nIndex = m_wndRibbonBar.GetCategoryIndex(pCatagory);

		if (nIndex >= 0 && nIndex < nCategoryNum)
		{
			if (bShow)
			{
				m_wndRibbonBar.ShowCategory(nIndex,TRUE);
			}
			else
			{
				m_wndRibbonBar.ShowCategory(nIndex,FALSE);
			}		
		}
	}
}


