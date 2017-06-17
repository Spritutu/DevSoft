// GaugeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "GaugeDlg.h"
#include "DlgDeviceSet.h"
#include "math.h"
#include "GlobalDataManager.h"
#include "Global.h"
#include "DlgTabPanel.h"
#include "Msg.h"
#include "GridColumnGroupManager.h"
#include "SensorIDGenerator.h"
#include "SensorData.h"
#include "SensorDataManager.h"
#include "Utility.h"
// GaugeDlg dialog
#pragma warning(disable:4267)
IMPLEMENT_DYNAMIC(GaugeDlg, CBaseDialog)
#define TIMER_GAUGE_EVENT (1000012)
#define TIMER_GAUGE (1000)
GaugeDlg::GaugeDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(GaugeDlg::IDD, pParent),
	m_bActiveFlag(FALSE)
{
	enableWarning = false;
	minRange = 0.0;
	maxRange = 200.0;
	maxWarningValue = DBL_MAX;//0X7FFFFFFF;
	minWarningValue = -DBL_MAX;//-9999999999;
}

GaugeDlg::~GaugeDlg()
{
}

void GaugeDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAUGE, m_wndGauge);
}


BEGIN_MESSAGE_MAP(GaugeDlg, CBaseDialog)
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_GAUGE_SET, &GaugeDlg::OnGaugeSet)
	ON_COMMAND(ID_GAUGE_DELETE, &GaugeDlg::OnGaugeDelete)
	ON_COMMAND(ID_GAUGE_ZERO, &GaugeDlg::OnGaugeZero)
	ON_UPDATE_COMMAND_UI(ID_GAUGE_ZERO, &GaugeDlg::OnUpdateGaugeZero)
	ON_COMMAND(ID_GAUGE_CORRECT, &GaugeDlg::OnGaugeCorrect)
	ON_UPDATE_COMMAND_UI(ID_GAUGE_CORRECT, &GaugeDlg::OnUpdateGaugeCorrect)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_MESSAGE(WM_SET_DLG_ACTIVE,&GaugeDlg::NotifyActive)
END_MESSAGE_MAP()

void GaugeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CBaseDialog::OnActivate(nState, pWndOther, bMinimized);
}

BOOL GaugeDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	m_wndGauge.GetGauge()->SetRange(minRange,maxRange);
	m_wndGauge.GetGauge()->SetValue(0.0);
	updateData(NULL);
	Invalidate(TRUE);
	SetTimer(TIMER_GAUGE_EVENT, TIMER_GAUGE, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void GaugeDlg::OnUpdateGauge() 
{
	CBCGPCircularGaugeImpl* pGauge = m_wndGauge.GetGauge();
	ASSERT_VALID(pGauge);
	CRect rc;
	GetClientRect(rc);
	double minE = min(rc.Width(), rc.Height());
	//pGauge->SetFrameSize(minE/30);
	//pGauge->SetCapSize(minE/12);

	pGauge->RemoveAllColoredRanges();

	//if (m_bRange)
	{
		double width1 = 1.;
		double width2 = minE/20;
		double opacity = .5;

		CBCGPBrush brFrame;	// empty, no frame
		CBCGPBrush brGreen(CBCGPColor::White, CBCGPColor(CBCGPColor::Green, opacity), CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT);
		CBCGPBrush brYellow(CBCGPColor::Gold);
		CBCGPBrush brRed(CBCGPColor(CBCGPColor::White, opacity), CBCGPColor::DarkRed, CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT);

		int nScale = 0;
		//����
		if(enableWarning&&(m_wndGauge.GetGauge()->GetValue()<minWarningValue||m_wndGauge.GetGauge()->GetValue()>maxWarningValue))
		{
			pGauge->ModifyPointer(0, CBCGPCircularGaugePointer(
				CBCGPBrush(CBCGPColor(CBCGPColor::Red), CBCGPColor(CBCGPColor::White), CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT), 
				CBCGPBrush(CBCGPColor(CBCGPColor::Red))));
		}else
		{
			pGauge->ModifyPointer(0, CBCGPCircularGaugePointer(
				CBCGPBrush(CBCGPColor(CBCGPColor::BurlyWood), CBCGPColor(CBCGPColor::White), CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT), 
				CBCGPBrush(CBCGPColor(CBCGPColor::BurlyWood))));
		}
		
		pGauge->AddColoredRange(minRange, minRange + (maxRange - minRange) / 3, brGreen, brFrame, nScale, width1, width2);
		pGauge->AddColoredRange(minRange + (maxRange - minRange) / 3, minRange + (maxRange-minRange) * 2 / 3, brYellow, brFrame, nScale, width2);
		pGauge->AddColoredRange(minRange + (maxRange - minRange) * 2 / 3, maxRange, brRed, brFrame, nScale, width2, width1);
	}

	pGauge->SetTextLabelFormat(_T("")/*_T("%.0f")*/);

	pGauge->SetTickMarkSize(minE/30, TRUE);
	pGauge->SetTickMarkSize(minE/60 , FALSE);

	pGauge->RemoveAllSubGauges();
	int fontSize = min(rc.Width(), rc.Height())/10;
	CString valueStr;
	valueStr.Format(_T("%.2f"), m_wndGauge.GetGauge()->GetValue());
	valueStr += m_strUnit;
	m_pValueGauge = new CBCGPTextGaugeImpl(valueStr, CBCGPColor::LawnGreen);
	CBCGPTextFormat TextFormat = m_pValueGauge->GetTextFormat();
	TextFormat.SetFontSize(float(fontSize));
	m_pValueGauge->SetTextFormat(TextFormat);

	m_pNameGauge = new CBCGPTextGaugeImpl(m_strTitle, CBCGPColor::DarkGray);
	TextFormat = m_pValueGauge->GetTextFormat();
	TextFormat.SetFontSize(float(fontSize));
	m_pNameGauge->SetTextFormat(TextFormat);
	//m_pValueGauge->SetText(m_strText);
	pGauge->AddSubGauge(m_pValueGauge, 
		CBCGPGaugeImpl::BCGP_SUB_GAUGE_TOP, CBCGPSize(0, 0), CBCGPPoint(0, 0));
	pGauge->AddSubGauge(m_pNameGauge, 
		CBCGPGaugeImpl::BCGP_SUB_GAUGE_BOTTOM, CBCGPSize(0, 0), CBCGPPoint(0, 0));
	//if (m_bIcon)
	{
		//pGauge->AddSubGauge(new CBCGPImageGaugeImpl(IDB_IMAGE1), CBCGPGaugeImpl::BCGP_SUB_GAUGE_BOTTOM);
	}

	m_wndGauge.RedrawWindow();
}
// GaugeDlg message handlers



void GaugeDlg::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_wndGauge.GetSafeHwnd())
	{
		m_wndGauge.MoveWindow(5, 5, cx-10, cy-10);
	}
	Invalidate(TRUE);
}


void GaugeDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CBaseDialog::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
	Invalidate(TRUE);
}
void GaugeDlg::setRange(double min, double max)
{
	minRange = min;
	maxRange = max;
}
void GaugeDlg::getRange(double& min, double& max)
{
	min = minRange;
	max = maxRange;
}
void GaugeDlg::setWarningValue(double min, double max)
{
	minWarningValue = min;
	maxWarningValue = max;
}
void GaugeDlg::getWarningValue(double& min, double& max)
{
	min = minWarningValue;
	max = maxWarningValue;
}
void GaugeDlg::setShowDataColumn(int ColumnID)
{
	dataColumnID = ColumnID;
}
int GaugeDlg::getShowDataColumn()
{
	return dataColumnID;
}
void GaugeDlg::setValue(double value)
{
	m_wndGauge.GetGauge()->SetValue(value);
	Invalidate(TRUE);
}
double GaugeDlg::getValue()
{
	return m_wndGauge.GetGauge()->GetValue();
}
void GaugeDlg::setEnableWarning(bool warning)
{
	enableWarning = warning;
}
bool GaugeDlg::isEnableWarning()
{
	return enableWarning;
}
//��������
int GaugeDlg::saveData()
{
	//���浱ǰ���ڵ�����
	return 0;
}
//��ȡ����
int GaugeDlg::readData()
{
	//��ȡ��ǰ���ڵ�����
	return 0;
}


void GaugeDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CBCGPContextMenuManager* pContexMenuManager = theApp.GetContextMenuManager();
	if (nullptr != pContexMenuManager)
	{
		pContexMenuManager->ShowPopupMenu(IDR_MENU_GAUGE,point.x,point.y,this,TRUE);
	}	
}


void GaugeDlg::OnGaugeSet()
{
	// TODO: Add your command handler code here
	DlgDeviceSet dlgSet(this);
	//���ó�ʼֵ
	dlgSet.setDataColumnID(dataColumnID);
	dlgSet.setWarningState(enableWarning);
	if(enableWarning)
	{
		dlgSet.setWarningType(maxWarningValue<DBL_MAX?0:1);
		dlgSet.setWarningValue(maxWarningValue<DBL_MAX?maxWarningValue:minWarningValue);
	}
	if(IDOK == dlgSet.DoModal())
	{
		//��ȡֵ
		dataColumnID = dlgSet.getDataColumnID();
		enableWarning = dlgSet.getWarningState();
		if(enableWarning)
		{
			int warningType = dlgSet.getWarningType();
			if(0==warningType)//max
			{
				maxWarningValue = dlgSet.getWarningValue();
				minWarningValue = -DBL_MAX;
			}else//min
			{
				minWarningValue = dlgSet.getWarningValue();
				maxWarningValue = DBL_MAX;
			}
		}
		else
		{
			minWarningValue = -DBL_MAX;
			maxWarningValue = DBL_MAX;
		}
		//����
		updateData(NULL);
	}
}


void GaugeDlg::OnGaugeDelete()
{
	// TODO: Add your command handler code here
	//ɾ����ǰ����
}


void GaugeDlg::OnGaugeZero()
{
	// TODO: Add your command handler code here
}


void GaugeDlg::OnUpdateGaugeZero(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void GaugeDlg::OnGaugeCorrect()
{
	// TODO: Add your command handler code here
}


void GaugeDlg::OnUpdateGaugeCorrect(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void GaugeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CBaseDialog::OnPaint() for painting messages
	OnUpdateGauge();
	//���߿�
	CWnd* parent = GetParent();
	if(nullptr != parent)
	{
		parent = parent->GetParent();
		CDlgTabPanel* pTabPanel = dynamic_cast<CDlgTabPanel*>(parent);
		if(pTabPanel)
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
void GaugeDlg::updateData(CGlobalDataManager* dbMgr)
{
	//���µ�ǰֵ
	std::string strSensorName = CSensorIDGenerator::CreateInstance().QueryrNameBySensorID(dataColumnID);
	if (m_strTitle != CString(strSensorName.c_str()))
	{
		m_strTitle = CString(strSensorName.c_str());
		OnUpdateGauge();
	}
	CSensorData* pData = CSensorDataManager::CreateInstance().GetSensorDataBySensorID(dataColumnID);
	if (nullptr != pData)
	{
		std::vector<float> cData;
		pData->GetSensorData(cData);
		if(!cData.empty())
		{
			for(UINT v = cData.size() - 1; v >= 0; --v)
			{
				if (getValue() != cData[v])
				{
					setValue(cData[v]);
				}
				break;
			}
		}	
	}
	//end ldh 0610
	//Invalidate(TRUE);
}

BOOL GaugeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class


	//begin add by hanxiaowei

	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
			CWnd* pDevicePanel = GetParent();
			if (nullptr != pDevicePanel)
			{
				CDlgTabPanel* pTabWnd = dynamic_cast<CDlgTabPanel*>(pDevicePanel->GetParent());

				if (nullptr != pTabWnd)
				{
					pTabWnd->SetActive(CDlgTabPanel::DEVICE_INDEX,this);
				}	
			}
		}
		break;
	default:
		break;
	}
	//end add by hanxiaowei


	if (WM_LBUTTONDOWN == pMsg->message || WM_RBUTTONDOWN == pMsg->message)
	{
		CWnd* pWnd = AfxGetMainWnd();
		if (NULL != pWnd)
		{
			pWnd->PostMessage(WM_NOTIFY_ACTIVE_WND_TYPE,1,0);
		}
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}


void GaugeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//��������
	if (TIMER_GAUGE_EVENT == nIDEvent)
	{
		updateData(NULL);
	}
	CBaseDialog::OnTimer(nIDEvent);
}

LRESULT GaugeDlg::NotifyActive( WPARAM wp,LPARAM lp )
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
