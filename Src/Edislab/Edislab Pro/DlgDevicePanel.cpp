// DlgDevicePanel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgDevicePanel.h"
#include <boost/checked_delete.hpp>
#include <boost/foreach.hpp>
#include "GaugeDlg.h"
#include "Macro.h"
// CDlgDevicePanel �Ի���

IMPLEMENT_DYNAMIC(CDlgDevicePanel, CBaseDialog)

CDlgDevicePanel::CDlgDevicePanel(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgDevicePanel::IDD, pParent)
	, m_WidgetLayout(this, true)
{
}

CDlgDevicePanel::~CDlgDevicePanel()
{
	for (auto i = 0; i < (int)m_vecPanel.size(); ++i)
	{
		if (NULL != m_vecPanel[i])
		{
			boost::checked_delete(m_vecPanel[i]);
		}
	}
	m_vecPanel.clear();
}

void CDlgDevicePanel::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDevicePanel, CBaseDialog)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgDevicePanel ��Ϣ�������
BOOL CDlgDevicePanel::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_WidgetLayout.InitLayout(this);
	//CreatePanel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgDevicePanel::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	AdjustPanelLayout(cx,cy);
}

void CDlgDevicePanel::CreatePanel( void )
{
	GaugeDlg* pDevicePanel = new GaugeDlg;
	if (NULL != pDevicePanel)
	{
		pDevicePanel->Create(GaugeDlg::IDD,this);
		m_vecPanel.push_back(pDevicePanel);
		m_WidgetLayout.AddWidget(pDevicePanel);
		//m_dataManager.addGaugeDlg(pDevicePanel);
	}
}
void CDlgDevicePanel::addPanel()
{
	GaugeDlg* pDevicePanel = new GaugeDlg;
	if (nullptr != pDevicePanel)
	{
		pDevicePanel->Create(GaugeDlg::IDD,this);
		m_vecPanel.push_back(pDevicePanel);
		m_WidgetLayout.AddWidget(pDevicePanel);
		CRect rc;
		GetClientRect(&rc);
		m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
	}

}
void CDlgDevicePanel::delPanel(CWnd* pDlg)
{
	if(nullptr != pDlg)
	{
		m_WidgetLayout.DelWidget(pDlg);
		CRect rc;
		GetClientRect(&rc);
		m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
		for(auto Iter = m_vecPanel.begin();Iter != m_vecPanel.end(); ++Iter)
		{
			if(pDlg == *Iter)
			{
				if (pDlg->GetSafeHwnd() != NULL)
				{
					pDlg->DestroyWindow();
				}
				m_vecPanel.erase(Iter);
				break;
			}
		}
		boost::checked_delete(pDlg);
	}	
}
void CDlgDevicePanel::DestroyPanel( void )
{

}

void CDlgDevicePanel::AdjustPanelLayout( int nWidth,int nHeight )
{
	m_WidgetLayout.AdjustLayout(nWidth,nHeight);
}

void CDlgDevicePanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SetCapture();
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleLButtonDown(nFlags,point,rc.Width(),rc.Height());

	CBCGPDialog::OnLButtonDown(nFlags, point);
}


void CDlgDevicePanel::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleMouseMove(nFlags,point,rc.Width(),rc.Height());
	CBCGPDialog::OnMouseMove(nFlags, point);
}


void CDlgDevicePanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleLButtonUp(nFlags,point,rc.Width(),rc.Height());
	ReleaseCapture();
	CBCGPDialog::OnLButtonUp(nFlags, point);
}

int CDlgDevicePanel::GetWidgetNum( void ) const
{
	return m_WidgetLayout.GetWidgetNum();
}


void CDlgDevicePanel::NotifyDisplayPanelChange(const LP_SENSOR_TYPE_KEY pSensor,int nFlag)
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
		if (nPanelNum >= MAX_DEVICE_PANEL_NUM)
		{
			return;
		}

		//���ֻ��һ����壬�鿴�Ƿ�������ʹ�õ�
		if (1 == nPanelNum)
		{
			if (nullptr != m_vecPanel[0])
			{
				//��ȡ�Ѿ����ڵ���ʾ��������ID
				SENSOR_TYPE_KEY KeyID = m_vecPanel[0]->getShowDataColumn();

				//�Ƿ��Ĵ�����ID
				if (KeyID.nSensorID < 0 && KeyID.nSensorSerialID < 0)
				{
					m_vecPanel[0]->setShowDataColumn(SENSOR_TYPE_KEY(pSensor->nSensorID,pSensor->nSensorSerialID));
				}
				//����ǺϷ��Ĵ���������ô������һ����ʾ���
				else
				{
					GaugeDlg* pDevicePanel = new GaugeDlg;
					if (nullptr != pDevicePanel)
					{
						pDevicePanel->setShowDataColumn(*pSensor);
						pDevicePanel->Create(GaugeDlg::IDD,this);
						m_vecPanel.push_back(pDevicePanel);
						m_WidgetLayout.AddWidget(pDevicePanel);
						CRect rc;
						GetClientRect(&rc);
						m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
					}
				}
			}
		}
		else
		{
			GaugeDlg* pDevicePanel = new GaugeDlg;
			if (nullptr != pDevicePanel)
			{
				pDevicePanel->setShowDataColumn(*pSensor);
				pDevicePanel->Create(GaugeDlg::IDD,this);
				m_vecPanel.push_back(pDevicePanel);
				m_WidgetLayout.AddWidget(pDevicePanel);
				CRect rc;
				GetClientRect(&rc);
				m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
			}
		}
	}
	//ɾ�����
	else
	{
		//���ֻʣ���һ��
		if (1 == nPanelNum)
		{
			if (nullptr != m_vecPanel[0])
			{
				m_vecPanel[0]->setShowDataColumn(SENSOR_TYPE_KEY());
			}
		}
		else
		{	
			auto FindPred = [&pSensor](const GaugeDlg* pElement)->bool
			{
				if (pElement->getShowDataColumn() == *pSensor)
				{
					return true;
				}
				return false;
			};
			//�ҵ��Ի���
			auto Iter = std::find_if(m_vecPanel.begin(),m_vecPanel.end(),FindPred);
			if (Iter != m_vecPanel.end())
			{
				m_WidgetLayout.DelWidget(*Iter);
				if ((*Iter)->GetSafeHwnd() != NULL)
				{
					(*Iter)->DestroyWindow();
				}
				boost::checked_delete((*Iter));
				m_vecPanel.erase(Iter);
				CRect rc;
				GetClientRect(&rc);
				m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
			}
		}
	}
}
