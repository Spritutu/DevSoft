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
#include "GridDisplayColumnInfo.h"
#include "SensorData.h"
#include "SensorDataManager.h"
#include "DlgTabPanel.h"
#include "Msg.h"
#include "Global.h"
const int TIMER_ID = 1;
const int TIMER_GAP = 3000;
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
				//��ȡ������
				CString strColumnName = pGridCtrl->GetColumnName(nCol);

				int nSensorID = CGridDisplayColumnInfo::CreateInstance().QuerySensorIDByColumnName(strColumnName);
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
	return TRUE;
}

IMPLEMENT_DYNAMIC(CDlgGridContainer, CBCGPDialog)

CDlgGridContainer::CDlgGridContainer(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgGridContainer::IDD, pParent),
     m_bLoop(false)
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
END_MESSAGE_MAP()


// CDlgGridContainer ��Ϣ�������


BOOL CDlgGridContainer::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	EnableVisualManagerStyle(TRUE,TRUE);
	//if (!m_pYieldDataThread)
	//{
	//	m_bLoop = true;

	//	m_pYieldDataThread = boost::make_shared<boost::thread>(boost::bind(&CDlgGridContainer::YieldDataProc,this));
	//}
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		std::vector<HEADRER_INFO> HeaderInfoArray;
		std::vector<GRID_DISPLAY_INFO> GridDisplayInfoArray;
		//��ȡҪ��ʾ������Ϣ
		CGridDisplayColumnInfo::CreateInstance().GetGridDisplayInfo(GridDisplayInfoArray);
		//��ʾ��ϢΪ���򷵻�
		if (!GridDisplayInfoArray.empty())
		{
			HEADRER_INFO TempHeaderInfo;
			BOOST_FOREACH(auto& V,GridDisplayInfoArray)
			{
				TempHeaderInfo.strHeadName = V.strHeadName;
				BOOST_FOREACH(auto& ColumnElement,V.ContainColumnIndexArray)
				{
					TempHeaderInfo.ContainColumnIndexArray.push_back(ColumnElement.strColumnName);
				}
				HeaderInfoArray.push_back(TempHeaderInfo);
				TempHeaderInfo.Reset();
			}
			m_DisplayGrid.SetHeaderInfoArray(HeaderInfoArray);
			m_DisplayGrid.SetDisplayVirtualRows(600);
			m_DisplayGrid.SetCallBack(GridCallback);
			m_DisplayGrid.Create(WS_VISIBLE | WS_CHILD,CRect(0,0,0,0),this,CCustomGrid::s_GridID++);
		}
	}
	SetTimer(TIMER_ID,TIMER_GAP,NULL);
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

	if (WM_LBUTTONDOWN == pMsg->message|| WM_RBUTTONDOWN == pMsg->message)
	{
		CWnd* pWnd = AfxGetMainWnd();
		if (NULL != pWnd)
		{
			pWnd->PostMessage(WM_NOTIFY_ACTIVE_WND_TYPE,0,0);
		}
	}

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
	if (m_DisplayGrid.GetSafeHwnd() == NULL)
	{
		return;
	}

	if (strDeviceName.empty())
	{
		return;
	}

	//����
	if (nOnFlag)
	{
		m_DisplayGrid.AddColumnInfo(_T("��ǰ"),CString(strDeviceName.c_str()));
	}
	//����
	else
	{
		m_DisplayGrid.RemoveColumn(CString(strDeviceName.c_str()));
	}
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
			if(this == pTabPanel->GetActiveDlg())//��ǰ���ڼ���
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
