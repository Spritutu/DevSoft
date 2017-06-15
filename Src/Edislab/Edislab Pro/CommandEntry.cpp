#include "StdAfx.h"
#include "Edislab Pro.h"
#include "Edislab ProView.h"
#include "CommandEntry.h"
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include "Msg.h"
#include "DlgAddPage.h"
#include "DlgTabPanel.h"
#include "DlgPageNameConfig.h"
#include "Utility.h"
#include "CApplication.h"
#include "CDocuments.h"
#include "CFont0.h"
#include "CSelection.h"
#include "CParagraphFormat.h"
#include "DlgAcquirationPara.h"
#include "DlgSensorChoose.h"
#include "DlgDataSetting.h"
#include "Global.h"
#include "SensorConfig.h"
#include "Log.h"
#include "SensorIDGenerator.h"
#include "SerialPortService.h"
#include "SensorData.h"
#include "SensorDataManager.h"
//���ҳ����
const int MAX_PAGE_NUM = 4;
//����������
const int MAX_GRID_NUM = 9;
const int MAX_DEVICE_NUM = 9;
const int MAX_DIAGRAM_NUM = 9;
//�Ƿ�ʼ�ɼ�
static bool s_bStartCapture = false;
//�������ҳ��
static void HandleAddPage(CEdislabProView* pView);
//�������ҳ��
static void UpdateHandleAddPage(CEdislabProView* pView,CCmdUI* pCmdUI);
//ɾ��ҳ��
static void HandleDelPage(CEdislabProView* pView);
//����ɾ��ҳ��
static void UpdateHandleDelPage(CEdislabProView* pView,CCmdUI* pCmdUI);
//ҳ������
static void HandlePageName(CEdislabProView* pView);
//����ҳ������
static void UpdateHandlePageName(CEdislabProView* pView,CCmdUI* pCmdUI);
//��תҳ��
static void HandleJumpPage(CEdislabProView* pView);
//������ת����
static void UpdateHandleJumpPage(CEdislabProView* pView,CCmdUI* pCmdUI);
//��ӱ��
void HandleAddTable(CEdislabProView* pView);
//������ӱ��
void UpdateHandleAddTable(CEdislabProView* pView,CCmdUI* pCmdUI);
//���ͼ
void HandleAddImage(CEdislabProView* pView);
//�������ͼ
void UpdateHandleAddImage(CEdislabProView* pView,CCmdUI* pCmdUI);
//����Ǳ�
void HandleAddDevice(CEdislabProView* pView);
//��������Ǳ�
void UpdateHandleAddDevice(CEdislabProView* pView,CCmdUI* pCmdUI);
//ɾ��Ԫ��
void HandleDelElement(CEdislabProView* pView);
//����ɾ��Ԫ��
void UpdateHandleDelElement(CEdislabProView* pView,CCmdUI* pCmdUI);
//��ʼ�ɼ�
void HandleStart(CEdislabProView* pView);
void UpdateHandleStart(CEdislabProView* pView,CCmdUI* pCmdUI);
//�Զ�ʶ��
void HandleAutoSelect(CEdislabProView* pView);
void UpdateHandleAutoSelect(CEdislabProView* pView,CCmdUI* pCmdUI);
//�ֶ�ʶ��
void HandleManualSelect(CEdislabProView* pView);
void UpdateHandleManualSelect(CEdislabProView* pView,CCmdUI* pCmdUI);
//���ʵ�鱨��
void HandleOutputTestReport(CEdislabProView* pView);
//ɾ��Ԫ��
void HandleChooseDevice(CEdislabProView* pView);
//����ɾ��Ԫ��
void UpdateHandleChooseDevice(CEdislabProView* pView,CCmdUI* pCmdUI);
//ɾ��Ԫ��
void HandleAcquirePara(CEdislabProView* pView);
//����ɾ��Ԫ��
void UpdateHandleAcquirePara(CEdislabProView* pView,CCmdUI* pCmdUI);

//����Ӧ��Ribbon�˵�
//ѡ��˵�
void HandleGridOption(CEdislabProView* pView);
//���Ʋ˵�
void HandleGridCopy(CEdislabProView* pView);
//ճ���˵�
void HandleGridPaste(CEdislabProView* pView);
//��һ�в˵�
void HandleGridFirstRow(CEdislabProView* pView);
//���һ�в˵�
void HandleGridLastRow(CEdislabProView* pView);
//������
void HandleGridInsertRow(CEdislabProView* pView);
//ɾ������
void HandleGridDelCell(CEdislabProView* pView);
//����������
void HandleGridAddDataColumn(CEdislabProView* pView);
//��������
void HandleGridYieldData(CEdislabProView* pView);
//����
void HandleGridCalculate(CEdislabProView* pView);
//�����������
void HandleGridClearCellData(CEdislabProView* pView);
//�洢ΪExcel
void HandleGridSaveAsExcel(CEdislabProView* pView);
//��ӡ
void HandleGridPrint(CEdislabProView* pView);
//��ӡԤ��
void HandleGridPrintPreview(CEdislabProView* pView);

//��������
void HandleDataSetting(CEdislabProView* pView);

CCommandEntry& CCommandEntry::CreateInstance( void )
{
	static CCommandEntry s_Entry;
	return s_Entry;
}

CCommandEntry::CCommandEntry(void)
{
	InitCommandEntry();
	InitUpdateCommandEntry();
}


CCommandEntry::~CCommandEntry(void)
{
	UnInitCommandEntry();
	UnInitUpdateCommandEntry();
}

/*****************************************************************************
@FunctionName : GetCommandEntryByCommanID
@FunctionDescription : ��ȡ��������ӿ�
@inparam  : nCommandID:����ID
@outparam :  
@return : ���������ַ
*****************************************************************************/
pCommandEntry CCommandEntry::GetCommandEntryByCommanID( unsigned int nCommandID )
{

	auto Iter = m_CommandEntryMap.find(nCommandID);

	if (Iter == m_CommandEntryMap.end())
	{
		return NULL;
	}

	return Iter->second;

}

/*****************************************************************************
@FunctionName : GetUpdateCommandEntryByCommanID
@FunctionDescription : ��ȡ������´������ӿ�
@inparam  : nCommandID:����ID
@outparam :  
@return : ������´�������ַ
*****************************************************************************/
pUpdateCommandEntry CCommandEntry::GetUpdateCommandEntryByCommanID( unsigned int nCommandID )
{
	auto Iter = m_UpdateCommandEntryMap.find(nCommandID);

	if (Iter == m_UpdateCommandEntryMap.end())
	{
		return NULL;
	}

	return Iter->second;
}

/*****************************************************************************
@FunctionName : InitCommandEntry
@FunctionDescription : ��ʼ�������ӿ�����
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CCommandEntry::InitCommandEntry( void )
{
	m_CommandEntryMap.clear();

	m_CommandEntryMap[ID_ADD_PAGE] = HandleAddPage;
	m_CommandEntryMap[ID_DEL_PAGE] = HandleDelPage;
	m_CommandEntryMap[ID_SET_PAGE_NAME] = HandlePageName;
	m_CommandEntryMap[ID_GOTO_PAGE] = HandleJumpPage;
	m_CommandEntryMap[ID_ADD_TABLE] = HandleAddTable;
	m_CommandEntryMap[ID_DEL_ELEMENT] = HandleDelElement;
	m_CommandEntryMap[ID_ADD_IMAGE] = HandleAddImage;
	m_CommandEntryMap[ID_ADD_DEVICE] = HandleAddDevice;
	m_CommandEntryMap[ID_AUTO_SELECT] = HandleAutoSelect;
	m_CommandEntryMap[ID_START] = HandleStart;
	m_CommandEntryMap[ID_MANUAL_SELECT] = HandleManualSelect;
	m_CommandEntryMap[ID_OUTPUT_TEST_REPORT] = HandleOutputTestReport;

	m_CommandEntryMap[ID_SELECT_SENSOR] = HandleChooseDevice;
	m_CommandEntryMap[ID_COLLECT_PARAM] = HandleAcquirePara;

	//ѡ��
	m_CommandEntryMap[ID_OPTION] = HandleGridOption;
	m_CommandEntryMap[ID_COPY] = HandleGridCopy;
	m_CommandEntryMap[ID_PASTE] = HandleGridPaste;
	m_CommandEntryMap[ID_FIRST_ROW] = HandleGridFirstRow;
	m_CommandEntryMap[ID_LAST_ROW] = HandleGridLastRow;
	m_CommandEntryMap[ID_INSERT_ROW] = HandleGridInsertRow;
	m_CommandEntryMap[ID_DEL_CELL] = HandleGridDelCell;
	m_CommandEntryMap[ID_ADD_DATA_COLUMN] = HandleGridAddDataColumn;

	m_CommandEntryMap[ID_YIELD_DATA] = HandleGridYieldData;
	m_CommandEntryMap[ID_CALCULATE] = HandleGridCalculate;

	m_CommandEntryMap[ID_CLEAR_CHILD_DATA] = HandleGridClearCellData;
	m_CommandEntryMap[ID_SAVE_EXCEL] = HandleGridSaveAsExcel;

	m_CommandEntryMap[ID_PRINT] = HandleGridPrint;
	m_CommandEntryMap[ID_PRINT_PREVIEW] = HandleGridPrintPreview;

	//��������
	m_CommandEntryMap[ID_CONFIG_DATA] = HandleDataSetting;
}

/*****************************************************************************
@FunctionName : InitUpdateCommandEntry
@FunctionDescription : ��ʼ�����������ӿ�����
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CCommandEntry::InitUpdateCommandEntry( void )
{
	m_UpdateCommandEntryMap.clear();
	m_UpdateCommandEntryMap[ID_ADD_PAGE] = UpdateHandleAddPage;
	m_UpdateCommandEntryMap[ID_DEL_PAGE] = UpdateHandleDelPage;
	m_UpdateCommandEntryMap[ID_SET_PAGE_NAME] = UpdateHandlePageName;
	m_UpdateCommandEntryMap[ID_GOTO_PAGE] = UpdateHandleJumpPage;
	m_UpdateCommandEntryMap[ID_ADD_TABLE] = UpdateHandleAddTable;
	m_UpdateCommandEntryMap[ID_DEL_ELEMENT] = UpdateHandleDelElement;
	m_UpdateCommandEntryMap[ID_ADD_IMAGE] = UpdateHandleAddImage;
	m_UpdateCommandEntryMap[ID_ADD_DEVICE] = UpdateHandleAddDevice;

	m_UpdateCommandEntryMap[ID_SELECT_SENSOR] = UpdateHandleChooseDevice;
	m_UpdateCommandEntryMap[ID_COLLECT_PARAM] = UpdateHandleAcquirePara;

	//m_UpdateCommandEntryMap[ID_START] = UpdateHandleStart;
	m_UpdateCommandEntryMap[ID_MANUAL_SELECT] = UpdateHandleManualSelect;
	m_UpdateCommandEntryMap[ID_AUTO_SELECT] = UpdateHandleAutoSelect;
}

/*****************************************************************************
@FunctionName : UnInitCommandEntry
@FunctionDescription : ��������ӿ�����
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CCommandEntry::UnInitCommandEntry( void )
{
	m_CommandEntryMap.clear();
}

/*****************************************************************************
@FunctionName : UnInitUpdateCommandEntry
@FunctionDescription : ��ո��������ӿ�����
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CCommandEntry::UnInitUpdateCommandEntry( void )
{
	m_UpdateCommandEntryMap.clear();
}

/*****************************************************************************
@FunctionName : HandleAddPage
@FunctionDescription : ��������ҳ�����Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void HandleAddPage( CEdislabProView* pView )
{
	if (nullptr == pView)
	{
		return;
	}

	CDlgAddPage AddDlg;
	if (IDOK == AddDlg.DoModal())
	{
		//��ȡ��ӵĶ���
		ADD_PAGE_ELEMENT AddPageElement = AddDlg.GetAddPageElement();
		if (NEW_PAGE_COPY == AddPageElement.eumOpt)
		{
			CDlgTabPanel* pPanel = pView->GetCurrentPage();
			if (nullptr != pPanel)
			{
				AddPageElement.nGridNum = pPanel->GetGridNum();
				AddPageElement.nDiagramNum = pPanel->GetDiagramNum();
				AddPageElement.nDeviceNum = pPanel->GetDeviceNum();
			}
		}		
		if (pView->AddNewTabWnd(AddPageElement.strPageName,AddPageElement.nGridNum,AddPageElement.nDiagramNum,AddPageElement.nDeviceNum))
		{
			//�������һ��TabΪ���Tab
			int nPageNum = pView->GetPageNum();
			pView->SetActivePage(nPageNum - 1);
		}
	}
}

/*****************************************************************************
@FunctionName : UpdateHandleAddPage
@FunctionDescription : ���´�������ҳ�����Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void UpdateHandleAddPage( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (nullptr == pView || nullptr == pCmdUI)
	{
		return;
	}

	int nNum = pView->GetPageNum();

	if (nNum < MAX_PAGE_NUM)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}

}

/*****************************************************************************
@FunctionName : HandleDelPage
@FunctionDescription : ����ɾ��ҳ�����Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void HandleDelPage( CEdislabProView* pView )
{
	if (nullptr == pView)
	{
		return;
	}


	if (IDYES == Utility::AfxBCGPMessageBox(_T("ȷ��Ҫɾ��ҳ��?"),MB_YESNO))
	{
		pView->DeleteCurrentPage();
	}

}

/*****************************************************************************
@FunctionName : UpdateHandleDelPage
@FunctionDescription : ���´���ɾ��ҳ�����Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void UpdateHandleDelPage( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (nullptr == pView || nullptr == pCmdUI)
	{
		return;
	}

	int nNum = pView->GetPageNum();

	if (nNum <= 1)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}

}

/*****************************************************************************
@FunctionName : HandlePageName
@FunctionDescription : ����ҳ���������õ���Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void HandlePageName( CEdislabProView* pView )
{
	if (nullptr == pView)
	{
		return;
	}

	CString strPageName = pView->GetCurrentPageName();


	CDlgPageNameConfig PageNameConfigDlg(strPageName);

	if (IDOK == PageNameConfigDlg.DoModal())
	{
		strPageName = PageNameConfigDlg.GetPageName();
		//��������
		pView->SetCurrentPageName(strPageName);
	}

}

/*****************************************************************************
@FunctionName : UpdateHandlePageName
@FunctionDescription : ����ҳ���������õ���Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void UpdateHandlePageName( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (nullptr == pView || nullptr == pCmdUI)
	{
		return;
	}

	CDlgTabPanel* pWnd = pView->GetCurrentPage();

	if (nullptr == pWnd)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}


/*****************************************************************************
@FunctionName : HandleJumpPage
@FunctionDescription : ������תҳ�����Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void HandleJumpPage( CEdislabProView* pView )
{
	if (nullptr == pView)
	{
		return;
	}
}

/*****************************************************************************
@FunctionName : UpdateHandleJumpPage
@FunctionDescription : ������תҳ�����Ӧ
@inparam  : pView:��ͼָ��
@outparam :  
@return : 
*****************************************************************************/
void UpdateHandleJumpPage( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (nullptr == pView || nullptr == pCmdUI)
	{
		return;
	}

	int nNum = pView->GetPageNum();

	if (nNum <= 1)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}
//��ӱ��
void HandleAddTable(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}

	pView->AddGrid();
}
//���ͼ
void HandleAddImage(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}

	pView->AddDiagram();
	
}
//����Ǳ�
void HandleAddDevice(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}

	pView->AddDevice();
}
//ɾ��Ԫ��
void HandleDelElement(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}
	pView->DeleteElement();
}

void UpdateHandleAddTable( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (nullptr == pView || nullptr == pCmdUI)
	{
		return;
	}

	if (pView->GetGridNum() >= MAX_GRID_NUM)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void UpdateHandleAddImage( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (nullptr == pView || nullptr == pCmdUI)
	{
		return;
	}

	if (pView->GetDiagramNum() >= MAX_DIAGRAM_NUM)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void UpdateHandleAddDevice( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (nullptr == pView || nullptr == pCmdUI)
	{
		return;
	}

	if (pView->GetDeviceNum() >= MAX_DEVICE_NUM)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void UpdateHandleDelElement( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (nullptr == pView || nullptr == pCmdUI)
	{
		return;
	}

	if (0 == pView->GetGridNum() && 0 == pView->GetDeviceNum() && 0 == pView->GetDiagramNum())
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

//��ʼ�ɼ�
void HandleStart(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}

	std::vector<std::string> SensorNameArray;
	CSensorIDGenerator::CreateInstance().GetAllSensorName(SensorNameArray,false);
	if (s_bStartCapture)
	{
		//֪ͨ������Ҫֹͣˢ�µĿؼ�ֹͣˢ��
		pView->NotifyControlsStopRefresh();
		BOOST_FOREACH(auto& V,SensorNameArray)
		{
			CSerialPortService::CreateInstance().StopSensorCollect(V);
		}
	}
	else
	{
		//֪ͨ������Ҫ��ʼˢ�µĿؼ���ʼˢ��
		pView->NotifyControlsStartRefresh();
		BOOST_FOREACH(auto& V,SensorNameArray)
		{
			CSerialPortService::CreateInstance().StartSensorCollect(V);
		}
	}
	s_bStartCapture = !s_bStartCapture;
}

//�Զ�ʶ��
void HandleAutoSelect(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}
	
	CWnd* pWnd = AfxGetMainWnd();
	if(NULL == pWnd)
	{
		return;
	}

	g_bAutoSelect = TRUE;
	//COMIPLE.StartCom(pWnd);
	
}

//���ʵ�鱨��
void HandleOutputTestReport( CEdislabProView* pView )
{
	if (nullptr == pView)
	{
		return;
	}

	CApplication app;  
	COleVariant vTrue((short)TRUE), vFalse((short)FALSE);  
	app.CreateDispatch(_T("Word.Application"));
	app.put_Visible(FALSE); 
	//Create New Doc  
	CDocuments docs = app.get_Documents();  
	CComVariant tpl(_T("")),Visble,DocType(0),NewTemplate(false);  
	docs.Add(&tpl,&NewTemplate,&DocType,&Visble);  
	//Add Content:Text  
	CSelection sel = app.get_Selection(); 

	CParagraphFormat ParaFormat = sel.get_ParagraphFormat();
	CFont0 SelFont = sel.get_Font();
	SelFont.put_Bold(1);
	SelFont.put_Size(16);
	SelFont.put_Name(_T("����"));
	sel.TypeText(_T("�ޱ�������"));
	ParaFormat.put_Alignment(1);
	sel.TypeParagraph();
	sel.TypeParagraph();
	SelFont.put_Size(12);
	SelFont.put_Bold(0);
	ParaFormat.put_Alignment(0);
	sel.TypeText(_T("ʵ��༶_____________ʵ������_____________����_____________"));
	sel.TypeParagraph();
	sel.TypeParagraph();
	SelFont.put_Bold(1);
	sel.TypeText(_T("ʵ��Ŀ��:"));

	int nTimes = 5;
	for (int i = 0; i < nTimes; ++i)
	{
		sel.TypeParagraph();
	}
	sel.TypeText(_T("ʵ��Ŀ��:"));
	for (int i = 0; i < nTimes; ++i)
	{
		sel.TypeParagraph();
	}
	sel.TypeText(_T("ʵ��ԭ��:"));
	for (int i = 0; i < nTimes; ++i)
	{
		sel.TypeParagraph();
	}

	sel.TypeText(_T("ʵ������:"));
	for (int i = 0; i < nTimes; ++i)
	{
		sel.TypeParagraph();
	}

	sel.TypeText(_T("ʵ�鲽��:"));
	for (int i = 0; i < nTimes; ++i)
	{
		sel.TypeParagraph();
	}

	sel.TypeText(_T("ʵ���������"));
	for (int i = 0; i < nTimes; ++i)
	{
		sel.TypeParagraph();
	}
	sel.ReleaseDispatch();  
	ParaFormat.ReleaseDispatch();
	docs.ReleaseDispatch();  
	app.put_Visible(TRUE);  
	app.ReleaseDispatch();  
}

// ѡ������
void HandleChooseDevice(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}

    CDlgSensorChoose dlgChooseSensor;
	dlgChooseSensor.DoModal();
}

void UpdateHandleChooseDevice( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}
}

// ��ȡ����
void HandleAcquirePara(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}

	CDlgAcquirationPara dlgSelectPara;

	//begin modify by xiaowei.han 2017_6_7
	if (IDOK == dlgSelectPara.DoModal())
	{
		//��ȡ����Ƶ��
		const SENSOR_RECORD_INFO& SampleInfo = CSensorConfig::CreateInstance().GetSensorRecordInfo();
		//���������(��λ:ms)
		int nPeriod = (int)(1.0f / (SampleInfo.fFrequency)) * 1000;
		//���ô�����ʱ�����ݸı�
		CSensorData* pData = CSensorDataManager::CreateInstance().GetSensorDataBySensorID(CSensorIDGenerator::CreateInstance().GetSpecialSensorID());
		if (nullptr != pData)
		{
			pData->ClearSensorData();
			int nSize = (int)(SampleInfo.fFrequency * SampleInfo.fLimitTime);
			float fPeriod = 1.0f / (SampleInfo.fFrequency);
			for (int i = 0; i < nSize; ++i)
			{
				pData->AddSensorData(i * fPeriod);
			}
		}
		std::vector<std::string> SensorNameArray;
		CSensorIDGenerator::CreateInstance().GetAllSensorName(SensorNameArray,false);
		BOOST_FOREACH(auto& V,SensorNameArray)
		{
			CSerialPortService::CreateInstance().SetSensorFrequence(V,nPeriod);
		}
		if (SampleInfo.bLimitTime)
		{
			int nRow = (int)(SampleInfo.fFrequency * SampleInfo.fLimitTime);
			pView->NotifyGridChangeRows(nRow);
		}
	}
	//end modify by xiaowei.han 2017_6_7
}

void UpdateHandleAcquirePara( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}
}

void HandleManualSelect( CEdislabProView* pView )
{
	g_bAutoSelect = FALSE;
}

void UpdateHandleManualSelect( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}
	pCmdUI->SetCheck(!g_bAutoSelect);
}

void UpdateHandleAutoSelect(CEdislabProView* pView,CCmdUI* pCmdUI)
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}
	pCmdUI->SetCheck(g_bAutoSelect);
}

//ѡ��˵�
void HandleGridOption(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}

	//��ȡ��ǰTabҳ
	CDlgTabPanel* pTabPanel = pView->GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		//��ȡTabPanel�еĻ����
		CWnd* pActiveWnd = pTabPanel->GetActiveDlg();
		if (nullptr != pActiveWnd)
		{
			pActiveWnd->PostMessage(WM_NOTIFY_GRID_OPTION,0,0);
		}
	}
}

//���Ʋ˵�
void HandleGridCopy(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}

	//��ȡ��ǰTabҳ
	CDlgTabPanel* pTabPanel = pView->GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		//��ȡTabPanel�еĻ����
		CWnd* pActiveWnd = pTabPanel->GetActiveDlg();
		if (nullptr != pActiveWnd)
		{
			pActiveWnd->PostMessage(WM_NOTIFY_GRID_COPY,0,0);
		}
	}
}

//ճ���˵�
void HandleGridPaste(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}
	//��ȡ��ǰTabҳ
	CDlgTabPanel* pTabPanel = pView->GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		//��ȡTabPanel�еĻ����
		CWnd* pActiveWnd = pTabPanel->GetActiveDlg();
		if (nullptr != pActiveWnd)
		{
			pActiveWnd->PostMessage(WM_NOTIFY_GRID_PASTE,0,0);
		}
	}
}

//��һ�в˵�
void HandleGridFirstRow(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}
	//��ȡ��ǰTabҳ
	CDlgTabPanel* pTabPanel = pView->GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		//��ȡTabPanel�еĻ����
		CWnd* pActiveWnd = pTabPanel->GetActiveDlg();
		if (nullptr != pActiveWnd)
		{
			pActiveWnd->PostMessage(WM_NOTIFY_GRID_FIRST_ROW,0,0);
		}
	}
}

//���һ�в˵�
void HandleGridLastRow(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}
	//��ȡ��ǰTabҳ
	CDlgTabPanel* pTabPanel = pView->GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		//��ȡTabPanel�еĻ����
		CWnd* pActiveWnd = pTabPanel->GetActiveDlg();
		if (nullptr != pActiveWnd)
		{
			pActiveWnd->PostMessage(WM_NOTIFY_GRID_LAST_ROW,0,0);
		}
	}
}

//������
void HandleGridInsertRow(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}
	//��ȡ��ǰTabҳ
	CDlgTabPanel* pTabPanel = pView->GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		//��ȡTabPanel�еĻ����
		CWnd* pActiveWnd = pTabPanel->GetActiveDlg();
		if (nullptr != pActiveWnd)
		{
			pActiveWnd->PostMessage(WM_NOTIFY_GRID_INSERT_ROW,0,0);
		}
	}
}

//ɾ������
void HandleGridDelCell(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}
	//��ȡ��ǰTabҳ
	CDlgTabPanel* pTabPanel = pView->GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		//��ȡTabPanel�еĻ����
		CWnd* pActiveWnd = pTabPanel->GetActiveDlg();
		if (nullptr != pActiveWnd)
		{
			pActiveWnd->PostMessage(WM_NOTIFY_GRID_DEL_CELL,0,0);
		}
	}
}

//����������
void HandleGridAddDataColumn(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}
	//��ȡ��ǰTabҳ
	CDlgTabPanel* pTabPanel = pView->GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		//��ȡTabPanel�еĻ����
		CWnd* pActiveWnd = pTabPanel->GetActiveDlg();
		if (nullptr != pActiveWnd)
		{
			pActiveWnd->PostMessage(WM_NOTIFY_GRID_ADD_DATA_COLUMN,0,0);
		}
	}
}

//��������
void HandleGridYieldData(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}
	//��ȡ��ǰTabҳ
	CDlgTabPanel* pTabPanel = pView->GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		//��ȡTabPanel�еĻ����
		CWnd* pActiveWnd = pTabPanel->GetActiveDlg();
		if (nullptr != pActiveWnd)
		{
			pActiveWnd->PostMessage(WM_NOTIFY_GRID_YIELD_DATA,0,0);
		}
	}
}

//����
void HandleGridCalculate(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}
	//��ȡ��ǰTabҳ
	CDlgTabPanel* pTabPanel = pView->GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		//��ȡTabPanel�еĻ����
		CWnd* pActiveWnd = pTabPanel->GetActiveDlg();
		if (nullptr != pActiveWnd)
		{
			pActiveWnd->PostMessage(WM_NOTIFY_GRID_CALCULATE,0,0);
		}
	}
}

//�����������
void HandleGridClearCellData(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}
	//��ȡ��ǰTabҳ
	CDlgTabPanel* pTabPanel = pView->GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		//��ȡTabPanel�еĻ����
		CWnd* pActiveWnd = pTabPanel->GetActiveDlg();
		if (nullptr != pActiveWnd)
		{
			pActiveWnd->PostMessage(WM_NOTIFY_GRID_CLEAR_CELL_DATA,0,0);
		}
	}
}

//�洢ΪExcel
void HandleGridSaveAsExcel(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}
	//��ȡ��ǰTabҳ
	CDlgTabPanel* pTabPanel = pView->GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		//��ȡTabPanel�еĻ����
		CWnd* pActiveWnd = pTabPanel->GetActiveDlg();
		if (nullptr != pActiveWnd)
		{
			pActiveWnd->PostMessage(WM_NOTIFY_GRID_SAVE_AS_EXCEL,0,0);
		}
	}
}

//��ӡ
void HandleGridPrint(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}
	//��ȡ��ǰTabҳ
	CDlgTabPanel* pTabPanel = pView->GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		//��ȡTabPanel�еĻ����
		CWnd* pActiveWnd = pTabPanel->GetActiveDlg();
		if (nullptr != pActiveWnd)
		{
			pActiveWnd->PostMessage(WM_NOTIFY_GRID_PRINT,0,0);
		}
	}
}

//��ӡԤ��
void HandleGridPrintPreview(CEdislabProView* pView)
{
	if (nullptr == pView)
	{
		return;
	}
	//��ȡ��ǰTabҳ
	CDlgTabPanel* pTabPanel = pView->GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		//��ȡTabPanel�еĻ����
		CWnd* pActiveWnd = pTabPanel->GetActiveDlg();
		if (nullptr != pActiveWnd)
		{
			pActiveWnd->PostMessage(WM_NOTIFY_GRID_PRINT_PREVIEW,0,0);
		}
	}
}

void HandleDataSetting( CEdislabProView* pView )
{
	if (nullptr == pView)
	{
		return;
	}

	CDlgDataSetting Dlg;
	if (IDOK == Dlg.DoModal())
	{

	}
}