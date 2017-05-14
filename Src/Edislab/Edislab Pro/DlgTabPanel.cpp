// DlgTabPanel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgTabPanel.h"
#include "DlgDevicePanel.h"
#include "DlgDiagramPanel.h"
#include "DlgGridPanel.h"
#include "Type.h"
#include "ChartFigureDlg.h"
#include "GaugeDlg.h"
#include "CustomLabGridCtrl.h"
// CDlgTabPanel �Ի���

IMPLEMENT_DYNAMIC(CDlgTabPanel, CBCGPDialog)

CDlgTabPanel::CDlgTabPanel( const CString&strPageName,CWnd* pParent /*= NULL*/ )
	: CBCGPDialog(CDlgTabPanel::IDD, pParent),
	m_WidgetLayout(this,true),
	m_strPageName(strPageName),
	m_nDiagramNum(1),
	m_nGridNum(1),
	m_nDeviceNum(1),
	m_pActiveDlg(NULL)
{
	// TODO: �ڴ˴���ӹ������
	for (int i = 0; i < PANEL_NUM; ++i)
	{
		m_pPanel[i] = NULL;
	}
}


CDlgTabPanel::~CDlgTabPanel()
{
	for (int i = 0; i < PANEL_NUM; ++i)
	{
		if (NULL != m_pPanel[i])
		{
			delete m_pPanel[i];
			m_pPanel[i] = NULL;
		}
	}
}

void CDlgTabPanel::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTabPanel, CBCGPDialog)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgTabPanel ��Ϣ�������


BOOL CDlgTabPanel::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	}
	return CBCGPDialog::PreTranslateMessage(pMsg);
}


void CDlgTabPanel::SetPageName( const CString& strPageName )
{
	m_strPageName = strPageName;
}

CString CDlgTabPanel::GetPageName( void ) const
{
	return m_strPageName;
}

int CDlgTabPanel::GetGridNum( void ) const
{
	
	CDlgGridPanel* panle = dynamic_cast<CDlgGridPanel*>(m_pPanel[GRID_INDEX]);
	if(nullptr != panle)
	{
		return panle->GetWidgetNum();
	}

	return 0;
}

void CDlgTabPanel::SetGridNum( int nGridNum )
{
	m_nGridNum = nGridNum;
	
}

int CDlgTabPanel::GetDeviceNum( void ) const
{
	//return m_nDeviceNum;
	CDlgDevicePanel* panle = dynamic_cast<CDlgDevicePanel*>(m_pPanel[DEVICE_INDEX]);
	if(nullptr != panle)
	{
		return panle->GetWidgetNum();
	}

	return 0;
}

void CDlgTabPanel::SetDeviceNum( int nDeviceNum )
{
	m_nDeviceNum = nDeviceNum;
}

int CDlgTabPanel::GetDiagramNum( void ) const
{
	CDlgDiagramPanel* panle = dynamic_cast<CDlgDiagramPanel*>(m_pPanel[DIAGRAM_INDEX]);
	if(nullptr != panle)
	{
		return panle->GetWidgetNum();
	}

	return 0;
}

void CDlgTabPanel::SetDiagramNum( int DiagramNum )
{
	m_nDiagramNum = DiagramNum;
}

BOOL CDlgTabPanel::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CreatePanel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgTabPanel::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	AdjustPanelLayout(cx,cy);
}

//��ӱ��
void CDlgTabPanel::AddGrid()
{
	CDlgGridPanel* panle = dynamic_cast<CDlgGridPanel*>(m_pPanel[GRID_INDEX]);
	if(panle)
	{
		panle->addPanel();
	}
}
//���ͼ��
void CDlgTabPanel::AddChart()
{
	CDlgDiagramPanel* panle = dynamic_cast<CDlgDiagramPanel*>(m_pPanel[DIAGRAM_INDEX]);
	if(panle)
	{
		panle->addPanel();
	}
}
//����Ǳ�
void CDlgTabPanel::AddGauge()
{
	CDlgDevicePanel* panle = dynamic_cast<CDlgDevicePanel*>(m_pPanel[DEVICE_INDEX]);
	if(panle)
	{
		panle->addPanel();
	}
}
//ɾ������
void CDlgTabPanel::DelWnd(CWnd* dlg)
{
	if(dlg)
	{
		m_pActiveDlg = NULL;
		int type = GRID_INDEX;
		//��grid��ɾ��
		if(dynamic_cast<CCustomLabGridCtrl*>(dlg))
		{
			CDlgGridPanel* panleDiagram = dynamic_cast<CDlgGridPanel*>(m_pPanel[GRID_INDEX]);
			if(panleDiagram)
			{
				panleDiagram->delPanel(dlg);
			}
		}
		//��ͼ����ɾ��
		else if(dynamic_cast<ChartFigureDlg*>(dlg))		{
			CDlgDiagramPanel* panleDiagram = dynamic_cast<CDlgDiagramPanel*>(m_pPanel[DIAGRAM_INDEX]);
			if(panleDiagram)
			{
				panleDiagram->delPanel(dynamic_cast<ChartFigureDlg*>(dlg));
			}
			type = DIAGRAM_INDEX;
		}
		//���Ǳ���ɾ��
		else if(dynamic_cast<GaugeDlg*>(dlg))
		{
			CDlgDevicePanel* panleDevice = dynamic_cast<CDlgDevicePanel*>(m_pPanel[DEVICE_INDEX]);
			if(panleDevice)
			{
				panleDevice->delPanel(dynamic_cast<GaugeDlg*>(dlg));
			}
			type = DEVICE_INDEX;
		}
		SetActive(type);
	}
}
void CDlgTabPanel::SetActive(int type)
{
	m_pActiveDlg = NULL;
}
void CDlgTabPanel::CreatePanel( void )
{
	m_WidgetLayout.InitLayout(this);

	CDlgGridPanel* pGridPanel = new CDlgGridPanel;

	if (NULL != pGridPanel)
	{
		pGridPanel->Create(CDlgGridPanel::IDD,this);
		m_pPanel[GRID_INDEX] = pGridPanel;

		if (m_nGridNum > 0)
		{
			m_WidgetLayout.AddWidget(pGridPanel);
		}	
	}
	CDlgDiagramPanel* pDiagramPanel = new CDlgDiagramPanel;
	if (NULL != pDiagramPanel)
	{
		pDiagramPanel->Create(CDlgDiagramPanel::IDD,this);
		m_pPanel[DIAGRAM_INDEX] = pDiagramPanel;

		if (m_nDiagramNum > 0)
		{
			m_WidgetLayout.AddWidget(pDiagramPanel);
		}		
	}

	CDlgDevicePanel* pDevicePanel = new CDlgDevicePanel;
	if (NULL != pDevicePanel)
	{
		pDevicePanel->Create(CDlgDevicePanel::IDD,this);
		m_pPanel[DEVICE_INDEX] = pDevicePanel;

		if (m_nDeviceNum > 0)
		{
			m_WidgetLayout.AddWidget(pDevicePanel);
		}

	}

#if 0


	CString strContent;
	for (int i = 1; i <= 5; ++i)
	{
		CBCGPButton* pEidt = new CBCGPButton;

		if (NULL != pEidt)
		{
			strContent.Format(_T("����%d"),i);
			pEidt->Create(strContent,WS_VISIBLE | WS_CHILD,CRect(0,0,0,0),this,i);
			m_WidgetLayout.AddWidget(pEidt);
		}

	}
#endif

}

void CDlgTabPanel::DestroyPanel( void )
{
	for (int i = 0; i < PANEL_NUM; ++i)
	{
		if (NULL != m_pPanel[i])
		{
			if (m_pPanel[i]->GetSafeHwnd() != NULL)
			{
				m_pPanel[i]->DestroyWindow();
			}
		}
	}
}

void CDlgTabPanel::AdjustPanelLayout( int nWidth,int nHeight )
{
#if 0
	CRect GridRect;
	GridRect.left = 0;
	GridRect.top = 0;
	GridRect.right = GridRect.left + nWidth / 3;
	GridRect.bottom = GridRect.top + nHeight / 2;

	CRect DeviceRect;
	DeviceRect.left = GridRect.left;
	DeviceRect.top = GridRect.bottom + DEFAULT_GAP;
	DeviceRect.right = DeviceRect.left + GridRect.right;
	DeviceRect.bottom = DeviceRect.top + GridRect.Width();

	CRect DiagramRect;
	DiagramRect.left = GridRect.right + DEFAULT_GAP;
	DiagramRect.top = GridRect.top;
	DiagramRect.right = nWidth;
	DiagramRect.bottom = nHeight;

	if (NULL != m_pPanel[0])
	{
		if (NULL != m_pPanel[0]->GetSafeHwnd())
		{
			m_pPanel[0]->MoveWindow(&GridRect,TRUE);

			m_pPanel[0]->ShowWindow(SW_SHOW);
		}
	}

	if (NULL != m_pPanel[1])
	{
		if (NULL != m_pPanel[1]->GetSafeHwnd())
		{

			m_pPanel[1]->MoveWindow(&DeviceRect,TRUE);

			m_pPanel[1]->ShowWindow(SW_SHOW);
		}
	}

	if (NULL != m_pPanel[2])
	{
		if (NULL != m_pPanel[2]->GetSafeHwnd())
		{

			m_pPanel[2]->MoveWindow(&DiagramRect,TRUE);

			m_pPanel[2]->ShowWindow(SW_SHOW);
		}
	}
#endif

	m_WidgetLayout.AdjustLayout(nWidth,nHeight);
}

void CDlgTabPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SetCapture();
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleLButtonDown(nFlags,point,rc.Width(),rc.Height());

	CBCGPDialog::OnLButtonDown(nFlags, point);
}


void CDlgTabPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleMouseMove(nFlags,point,rc.Width(),rc.Height());
	CBCGPDialog::OnMouseMove(nFlags, point);
}


void CDlgTabPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleLButtonUp(nFlags,point,rc.Width(),rc.Height());
	ReleaseCapture();
	CBCGPDialog::OnLButtonUp(nFlags, point);
}

void CDlgTabPanel::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	delete this;
	CBCGPDialog::PostNcDestroy();
}
//���õ�ǰ�����
void CDlgTabPanel::SetActiveDlg(CWnd* dlg)
{
	if(m_pActiveDlg)
	{
		//ȡ������ �ߴ��仯
		m_pActiveDlg->Invalidate();
	}
	m_pActiveDlg = dlg;
	if(m_pActiveDlg)
	{
		//���� �ߴ��仯
		m_pActiveDlg->Invalidate();
	}
}
CWnd* CDlgTabPanel::GetActiveDlg()
{
	return m_pActiveDlg;
}

void CDlgTabPanel::PrepareDisplayElement( void )
{
	for (int i = 0; i < m_nDiagramNum; ++i)
	{
		AddChart();
	}

	for (int i = 0; i < m_nGridNum; ++i)
	{
		AddGrid();
	}

	for (int i = 0; i < m_nDeviceNum; ++i)
	{
		AddGauge();
	}
}
