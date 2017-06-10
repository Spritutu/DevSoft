// DlgGridContainer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgGridContainer.h"
#include <boost/random.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include "Log.h"
#include "GridColumnGroupManager.h"
#include "SensorIDGenerator.h"
#include "SensorData.h"
#include "SensorDataManager.h"
#include "SensorConfig.h"
#include "DlgTabPanel.h"
#include "Msg.h"
#include "Global.h"
#include "Utility.h"
#include "DlgGridOpt.h"
const int TIMER_ID = 1;
const int TIMER_GAP = 1000;
// CDlgGridContainer �Ի���
#pragma warning(disable:4244)
static BOOL CALLBACK GridCallback (BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	if (nullptr != pdi)
	{
		CCustomGrid* pGridCtrl = (CCustomGrid*)lp;
		if (nullptr != pGridCtrl)
		{
			//������
			int nRow = pdi->item.nRow;
			//������
			int nCol = pdi->item.nCol;
			if (nCol >= 0 && nRow >= 0)
			{
				if (0 == nCol)
				{
					CString strTempColumnName = pGridCtrl->GetColumnName(nCol);
					//������Ȳ���ʾ
					if (strTempColumnName == _T("t(s)ʱ��"))
					{
						//�������ڻ�ȡ
						const SENSOR_RECORD_INFO& SampleInfo = CSensorConfig::CreateInstance().GetSensorRecordInfo();
						double fPeriod = 1.0 / (SampleInfo.fFrequency);
						CString strContent;
						strContent.Format(_T("%.6f"),fPeriod * nRow);
						
						pdi->item.varValue = strContent;
					}
				}
				else
				{
					//��ȡ������
					CString strTempColumnName = pGridCtrl->GetColumnName(nCol);
					std::string strColumnName = Utility::WideChar2MultiByte(strTempColumnName.GetBuffer(0));

					int nSensorID = CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(strColumnName);
					if (nSensorID >= 0)
					{
						//���ݴ�����ID��ȡ����������
						CSensorData* pData = CSensorDataManager::CreateInstance().GetSensorDataBySensorID(nSensorID);
						if (nullptr != pData)
						{
							float fValue = 0.0f;
							if (pData->GetSensorData(nRow,fValue))
							{
								pdi->item.varValue = fValue;
							}		
						}
					}

				}	
			}
		}
	}
	return TRUE;
}

IMPLEMENT_DYNAMIC(CDlgGridContainer, CBCGPDialog)

CDlgGridContainer::CDlgGridContainer(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgGridContainer::IDD, pParent),
     m_bLoop(false),
	 m_bActiveFlag(FALSE)
{
}

CDlgGridContainer::~CDlgGridContainer()
{
}

void CDlgGridContainer::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgGridContainer, CBCGPDialog)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_MESSAGE(WM_NOTIFY_RBUTTON_DOWN,&CDlgGridContainer::NotifyGridClickRButton)
	ON_COMMAND(ID_MENU_GRID_OPTION, &CDlgGridContainer::OnMenuGridOption)
	ON_MESSAGE(WM_SET_DLG_ACTIVE,&CDlgGridContainer::NotifyActive)
	ON_COMMAND(ID_MENU_GRID_COPY, &CDlgGridContainer::OnMenuGridCopy)
	ON_COMMAND(ID_MENU_GRID_PASTE, &CDlgGridContainer::OnMenuGridPaste)
	ON_COMMAND(ID_MENU_GRID_DEL_CELL, &CDlgGridContainer::OnMenuGridDelCell)
	ON_COMMAND(ID_MENU_GRID_FIRST_ROW, &CDlgGridContainer::OnMenuGridFirstRow)
	ON_COMMAND(ID_MENU_GRID_LAST_ROW, &CDlgGridContainer::OnMenuGridLastRow)
END_MESSAGE_MAP()


// CDlgGridContainer ��Ϣ�������


BOOL CDlgGridContainer::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	EnableVisualManagerStyle(TRUE,TRUE);
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		std::vector<COLUMN_GROUP_INFO> ColumnGroupArray;
		//��ȡҪ��ʾ������Ϣ
		CGridColumnGroupManager::CreateInstance().GetGridDisplayInfo(ColumnGroupArray);
		//��ʾ��ϢΪ���򷵻�
		if (ColumnGroupArray.empty())
		{
			COLUMN_GROUP_INFO TempGroupInfo;
			TempGroupInfo.strGroupName = _T("��ǰ");
			COLUMN_INFO TempColumnInfo;
			TempColumnInfo.strColumnName = _T("X");
			TempGroupInfo.ColumnArray.push_back(TempColumnInfo);
			TempColumnInfo.Reset();
			TempColumnInfo.strColumnName = _T("Y");
			TempGroupInfo.ColumnArray.push_back(TempColumnInfo);
			ColumnGroupArray.push_back(TempGroupInfo);
		}

		const SENSOR_RECORD_INFO& SampleInfo = CSensorConfig::CreateInstance().GetSensorRecordInfo();

		//���õ���
		int nTotalRows = (int)(SampleInfo.fFrequency * SampleInfo.fLimitTime);
		m_DisplayGrid.SetHeaderInfoArray(ColumnGroupArray);
		nTotalRows = (std::max)(nTotalRows,60);
		m_DisplayGrid.SetDisplayVirtualRows(nTotalRows);
		m_DisplayGrid.SetCallBack(GridCallback);
		m_DisplayGrid.Create(WS_VISIBLE | WS_CHILD,CRect(0,0,0,0),this,CCustomGrid::s_GridID++);
	}
	//SetTimer(TIMER_ID,TIMER_GAP,NULL);
	return TRUE;
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgGridContainer::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (NULL != m_DisplayGrid.GetSafeHwnd())
	{
		CRect rc(0,0,cx,cy);
		rc.DeflateRect(5,5);
		m_DisplayGrid.MoveWindow(&rc);
	}
}


BOOL CDlgGridContainer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	//���߲��ֹ�������ǰ��Ĵ���
	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
			CWnd* pGridPanel = GetParent();

			if (nullptr != pGridPanel)
			{
				if (nullptr != pGridPanel)
				{
					CDlgTabPanel* pTabWnd = dynamic_cast<CDlgTabPanel*>(pGridPanel->GetParent());

					if (nullptr != pTabWnd)
					{
						pTabWnd->SetActive(CDlgTabPanel::GRID_INDEX,this);
					}
				}
			}
		}
		break;
	default:
		break;
	}

	//�������ڷ�����Ϣ���������ڵ�ǰ�ҳ
	if (WM_LBUTTONDOWN == pMsg->message|| WM_RBUTTONDOWN == pMsg->message)
	{
		CWnd* pWnd = AfxGetMainWnd();
		if (NULL != pWnd)
		{
			pWnd->PostMessage(WM_NOTIFY_ACTIVE_WND_TYPE,0,0);
		}
	}

	//����Enter��ESC
	if (pMsg->message == WM_KEYDOWN)
	{
		if (VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam)
		{
			return TRUE;
		}
	}

	return CBCGPDialog::PreTranslateMessage(pMsg);
}


void CDlgGridContainer::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//��ʱˢ������
	if (nIDEvent == TIMER_ID)
	{
		RefreshGrid();
	}

	CBCGPDialog::OnTimer(nIDEvent);
}


void CDlgGridContainer::OnDestroy()
{
	CBCGPDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������

	m_bLoop = false;

	if (m_pYieldDataThread)
	{
		m_pYieldDataThread->join();
	}
	KillTimer(TIMER_ID);
}

void CDlgGridContainer::NotifyDetectSensor(const std::string& strDeviceName,int nOnFlag)
{
	if (m_DisplayGrid.GetSafeHwnd() == NULL || strDeviceName.empty())
	{
		return;
	}
	//��ȡ��ǰ��Ҫ��ʾ�ķ�����Ϣ
	std::vector<COLUMN_GROUP_INFO> ColumnGroupArray;
	CGridColumnGroupManager::CreateInstance().GetGridDisplayInfo(ColumnGroupArray);
	if (ColumnGroupArray.empty())
	{
		COLUMN_GROUP_INFO TempGroupInfo;
		TempGroupInfo.strGroupName = _T("��ǰ");
		COLUMN_INFO TempColumnInfo;
		TempColumnInfo.strColumnName = _T("X");
		TempGroupInfo.ColumnArray.push_back(TempColumnInfo);
		TempColumnInfo.Reset();
		TempColumnInfo.strColumnName = _T("Y");
		TempGroupInfo.ColumnArray.push_back(TempColumnInfo);
		ColumnGroupArray.push_back(TempGroupInfo);
	}
	m_DisplayGrid.DynamicSetHeaderInfoArray(ColumnGroupArray);

#if 0
	//����
	if (nOnFlag)
	{
		//m_DisplayGrid.AddColumnInfo(_T("��ǰ"),CString(strDeviceName.c_str()));
		


	}
	//����
	else
	{
		//m_DisplayGrid.RemoveColumn(CString(strDeviceName.c_str()));
	}
#endif
}

void CDlgGridContainer::NotifyGridChangeRows(int nRows)
{
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}
	m_DisplayGrid.SetVirtualRows((std::max)(nRows,60));

	m_DisplayGrid.AdjustLayout();
}

void CDlgGridContainer::NotifyControlsStartRefresh()
{
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}
	SetTimer(TIMER_ID,TIMER_GAP,NULL);
}

void CDlgGridContainer::NotifyControlsStopRefresh()
{
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}
	KillTimer(TIMER_ID);
}

LRESULT CDlgGridContainer::NotifyGridClickRButton(WPARAM wp,LPARAM lp)
{
	CPoint pt;
	pt.x = (int)wp;
	pt.y = (int)lp;
	CBCGPContextMenuManager* pContexMenuManager = theApp.GetContextMenuManager();
	if (nullptr != pContexMenuManager)
	{
		pContexMenuManager->ShowPopupMenu(IDR_MENU_GRID,pt.x,pt.y,this,TRUE);
	}
	return 0L;
}

void CDlgGridContainer::RefreshGrid(void)
{
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}
	m_DisplayGrid.Refresh();
}


void CDlgGridContainer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//���߿�
	CWnd* pGridPanel = GetParent();
	if(nullptr != pGridPanel)
	{
		CDlgTabPanel* pTabPanel = dynamic_cast<CDlgTabPanel*>(pGridPanel->GetParent());
		if(nullptr != pTabPanel)
		{
			CRect rc;
			GetClientRect(rc);
			//if(this == pTabPanel->GetActiveDlg())//��ǰ���ڼ���
			if (TRUE == m_bActiveFlag)
			{
				CPen BoradrPen;
				BoradrPen.CreatePen(PS_SOLID,5,ActiveColor);
				CPen* pOldPen = dc.SelectObject(&BoradrPen);
				
				dc.MoveTo(rc.left,rc.top);
				dc.LineTo(rc.right,rc.top);

				dc.MoveTo(rc.right,rc.top);
				dc.LineTo(rc.right,rc.bottom);

				dc.MoveTo(rc.right,rc.bottom);
				dc.LineTo(rc.left,rc.bottom);

				dc.MoveTo(rc.left,rc.bottom);
				dc.LineTo(rc.left,rc.top);

				dc.SelectObject(pOldPen);
				BoradrPen.DeleteObject();
			}
			else
			{
				CPen BoradrPen;
				BoradrPen.CreatePen(PS_SOLID,5,UnActiveColor);
				CPen* pOldPen = dc.SelectObject(&BoradrPen);
				dc.MoveTo(rc.left,rc.top);
				dc.LineTo(rc.right,rc.top);

				dc.MoveTo(rc.right,rc.top);
				dc.LineTo(rc.right,rc.bottom);

				dc.MoveTo(rc.right,rc.bottom);
				dc.LineTo(rc.left,rc.bottom);

				dc.MoveTo(rc.left,rc.bottom);
				dc.LineTo(rc.left,rc.top);

				dc.SelectObject(pOldPen);
				BoradrPen.DeleteObject();
			}
		}
	}
}

void CDlgGridContainer::YieldDataProc( void )
{
	//ģ����������
	CSensorData* pData = CSensorDataManager::CreateInstance().GetSensorDataBySensorID(1);

	if (nullptr == pData)
	{
		return;
	}
	boost::mt19937 rng(time(nullptr));
	boost::random::uniform_real_distribution<float> Ui(100.0f,110.0f);
	while (m_bLoop)
	{
		pData->AddSensorData(Ui(rng));	
		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}
}


void CDlgGridContainer::OnMenuGridOption()
{

	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}

	//��ȡ�������ʾ��Ϣ
	std::vector<SHOW_COLUMN_GROUP_INFO> ShowColumnArray;
	m_DisplayGrid.GetColumnGroupDisplayInfo(ShowColumnArray);
	//����ѡ��Ի���
	CDlgGridOpt Dlg(this);
	Dlg.SetDisplayInfo(ShowColumnArray);
	if (IDOK == Dlg.DoModal())
	{
		//��ȡ��ʾ��Ϣ
		ShowColumnArray.clear();
		//��ȡ����
		Dlg.GetDisplayInfo(ShowColumnArray);

		//������ʾ
		m_DisplayGrid.SetColumnGroupDisplayInfo(ShowColumnArray);
	}
}

LRESULT CDlgGridContainer::NotifyActive( WPARAM wp,LPARAM lp )
{
	int nActiveFlag = (int)wp;

	if (nActiveFlag)
	{
		m_bActiveFlag = TRUE;
	}
	else
	{
		m_bActiveFlag = FALSE;
	}
	return 0L;
}


void CDlgGridContainer::OnMenuGridCopy()
{
	// TODO: �ڴ���������������
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}

	m_DisplayGrid.Copy();
}


void CDlgGridContainer::OnMenuGridPaste()
{
	// TODO: �ڴ���������������
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}

	m_DisplayGrid.Paste();
}


void CDlgGridContainer::OnMenuGridDelCell()
{
	// TODO: �ڴ���������������
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}

	m_DisplayGrid.Clear(FALSE);
}


void CDlgGridContainer::OnMenuGridFirstRow()
{
	// TODO: �ڴ���������������
	if (NULL != m_DisplayGrid.GetSafeHwnd())
	{
		int nTotalRow = m_DisplayGrid.GetTotalRowCount();
		if (nTotalRow > 0)
		{
			CBCGPGridRow *pFirstRow = m_DisplayGrid.GetRow(0);
			if (nullptr != pFirstRow)
			{
				m_DisplayGrid.EnsureVisible(pFirstRow);
				m_DisplayGrid.SetCurSel(pFirstRow);
			}
		}
	}
}


void CDlgGridContainer::OnMenuGridLastRow()
{
	// TODO: �ڴ���������������
	if (NULL != m_DisplayGrid.GetSafeHwnd())
	{
		int nTotalRow = m_DisplayGrid.GetTotalRowCount();
		if (nTotalRow > 0)
		{
			CBCGPGridRow *pLastRow = m_DisplayGrid.GetRow(nTotalRow - 1);

			if (nullptr != pLastRow)
			{
				m_DisplayGrid.EnsureVisible(pLastRow);
				m_DisplayGrid.SetCurSel(pLastRow);
			}
		}
	}
}
