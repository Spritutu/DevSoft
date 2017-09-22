// DlgDiagramPanel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgDiagramPanel.h"
#include <algorithm>
#include <boost/checked_delete.hpp>
#include <boost/foreach.hpp>
#include "ChartFigureDlg.h"
#include "Macro.h"
#include "SensorManager.h"
// CDlgDiagramPanel �Ի���

IMPLEMENT_DYNAMIC(CDlgDiagramPanel, CBaseDialog)

CDlgDiagramPanel::CDlgDiagramPanel(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgDiagramPanel::IDD, pParent)
	, m_WidgetLayout(this, true)
{
	
}

CDlgDiagramPanel::~CDlgDiagramPanel()
{
	for (UINT i = 0; i < m_vecPanel.size(); ++i)
	{
		boost::checked_delete(m_vecPanel[i]);
	}
	m_vecPanel.clear();
}

void CDlgDiagramPanel::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDiagramPanel, CBaseDialog)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgDiagramPanel ��Ϣ�������
BOOL CDlgDiagramPanel::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//CreatePanel();
	m_WidgetLayout.InitLayout(this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgDiagramPanel::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	AdjustPanelLayout(cx,cy);
}

void CDlgDiagramPanel::addPanel()
{
	ChartFigureDlg* pDiagramPanel = new ChartFigureDlg;
	if (nullptr != pDiagramPanel)
	{
		pDiagramPanel->Create(ChartFigureDlg::IDD,this);
		m_vecPanel.push_back(pDiagramPanel);
		m_WidgetLayout.AddWidget(pDiagramPanel);
		//m_dataManager.addChartDlg(pDiagramPanel);
		CRect rc;
		GetClientRect(&rc);
		m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
	}
}
void CDlgDiagramPanel::delPanel(CWnd* pDlg)
{
	if(nullptr != pDlg)
	{
		auto FindPred = [pDlg](const ChartFigureDlg* pElement)->bool
		{
			if (pDlg == pElement)
			{
				return true;
			}
			return false;
		};

		auto Iter = std::find_if(m_vecPanel.begin(),m_vecPanel.end(),FindPred);
		if (Iter != m_vecPanel.end())
		{		
			if ((*Iter)->GetSafeHwnd() != NULL)
			{
				(*Iter)->DestroyWindow();
			}
			m_WidgetLayout.DelWidget(*Iter);
			boost::checked_delete(*Iter);
			m_vecPanel.erase(Iter);	

			CRect rc;
			GetClientRect(&rc);
			m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
		}		
	}
}

void CDlgDiagramPanel::AdjustPanelLayout( int nWidth,int nHeight )
{
	m_WidgetLayout.AdjustLayout(nWidth,nHeight);
}

void CDlgDiagramPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SetCapture();
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleLButtonDown(nFlags,point,rc.Width(),rc.Height());

	CBCGPDialog::OnLButtonDown(nFlags, point);
}


void CDlgDiagramPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleMouseMove(nFlags,point,rc.Width(),rc.Height());
	CBCGPDialog::OnMouseMove(nFlags, point);
}


void CDlgDiagramPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleLButtonUp(nFlags,point,rc.Width(),rc.Height());
	ReleaseCapture();
	CBCGPDialog::OnLButtonUp(nFlags, point);
}

int CDlgDiagramPanel::GetWidgetNum( void ) const
{
	return m_WidgetLayout.GetWidgetNum();
}

void CDlgDiagramPanel::NotifyDisplayPanelChange(const LP_SENSOR_TYPE_KEY pSensor,int nFlag)
{
	if (nullptr == pSensor)
	{
		return;
	}

	//��ȡ��ʾ���ĸ���
	int nPanelNum = (int)m_vecPanel.size();

	//�������
	if (1 == nFlag)
	{
		//����������
		if (nPanelNum >= MAX_DIAGRAM_PANEL_NUM)
		{
			return;
		}

		//���ֻ��һ�����
		if (1 == nPanelNum)
		{
			if (nullptr != m_vecPanel[0])
			{
				SENSOR_TYPE_KEY KeyID = m_vecPanel[0]->GetXAxisSensorID();
				//�Ƿ�������
				if (!KeyID.IsValid())
				{
					m_vecPanel[0]->SetXAxisSensorID(CSensorManager::CreateInstance().GetSpecialSensorID());
					m_vecPanel[0]->SetYAxisSensorID(*pSensor);
				}
				//����һ�����
				else
				{
					ChartFigureDlg* pDiagramPanel = new ChartFigureDlg;
					if (nullptr != pDiagramPanel)
					{		
						pDiagramPanel->Create(ChartFigureDlg::IDD,this);
						pDiagramPanel->SetXAxisSensorID(CSensorManager::CreateInstance().GetSpecialSensorID());
						pDiagramPanel->SetYAxisSensorID(*pSensor);
						m_vecPanel.push_back(pDiagramPanel);
						m_WidgetLayout.AddWidget(pDiagramPanel);
					}
				}
			}
		}
		else
		{
			ChartFigureDlg* pDiagramPanel = new ChartFigureDlg;
			if (nullptr != pDiagramPanel)
			{
				pDiagramPanel->Create(ChartFigureDlg::IDD,this);
				pDiagramPanel->SetXAxisSensorID(CSensorManager::CreateInstance().GetSpecialSensorID());
				pDiagramPanel->SetYAxisSensorID(*pSensor);
				m_vecPanel.push_back(pDiagramPanel);
				m_WidgetLayout.AddWidget(pDiagramPanel);
			}
		}
	}
	//ɾ�����
	else
	{
		if (1 == nPanelNum)
		{
			if (nullptr != m_vecPanel[0])
			{
				m_vecPanel[0]->SetXAxisSensorID(SENSOR_TYPE_KEY());
				m_vecPanel[0]->RestYAxisSensorID();
			}
		}
		else
		{

			auto FindPred = [pSensor](const ChartFigureDlg* pElement)->bool
			{
				if (pElement->IsYAxisSensorIDExist(*pSensor))
				{
					return true;
				}
				return false;
			};

			auto Iter = std::find_if(m_vecPanel.begin(),m_vecPanel.end(),FindPred);
			if (Iter != m_vecPanel.end())
			{		
				if ((*Iter)->GetSafeHwnd() != NULL)
				{
					(*Iter)->DestroyWindow();
				}
				m_WidgetLayout.DelWidget(*Iter);
				boost::checked_delete(*Iter);
				m_vecPanel.erase(Iter);	
			}		
		}	
	}
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
}

void CDlgDiagramPanel::NotifyControlsStartRefresh()
{
	BOOST_FOREACH(auto& pGridWnd,m_vecPanel)
	{
		if (pGridWnd != nullptr)
		{
			pGridWnd->NotifyControlsStartRefresh();
		}
	}
}

void CDlgDiagramPanel::NotifyControlsStopRefresh()
{
	BOOST_FOREACH(auto& pGridWnd,m_vecPanel)
	{
		if (pGridWnd != nullptr)
		{
			pGridWnd->NotifyControlsStopRefresh();
		}
	}
}
