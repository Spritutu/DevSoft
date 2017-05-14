// ChartFigureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "ChartFigureDlg.h"
#include "afxdialogex.h"
#include "DlgChartSet.h"
#include "GlobalDataManager.h"
#include "DlgTabPanel.h"
#include "Msg.h"
// ChartFigureDlg �Ի���
IMPLEMENT_DYNAMIC(ChartFigureDlg, CBaseDialog)

ChartFigureDlg::ChartFigureDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(ChartFigureDlg::IDD, pParent)
{
	m_charxy = NULL;
}

ChartFigureDlg::~ChartFigureDlg()
{
	if (m_charxy != NULL)
	{
		delete m_charxy;
		m_charxy = NULL;
	}
}

void ChartFigureDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ChartFigureDlg, CBaseDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_ACTIVATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CHART_SET, &ChartFigureDlg::OnChartSet)
	ON_COMMAND(ID_CHART_DEL, &ChartFigureDlg::OnChartDel)
	ON_COMMAND(ID_CHART_ZOOM_IN, &ChartFigureDlg::OnChartZoomIn)
	ON_COMMAND(ID_CHART_ZOOM_OUT, &ChartFigureDlg::OnChartZoomOut)
	ON_COMMAND(ID_CHART_SHOW_ALL, &ChartFigureDlg::OnChartShowAll)
END_MESSAGE_MAP()

BOOL ChartFigureDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CDC* pDC = GetDC();
	m_charxy = new ChartXY(pDC->m_hDC);

	//ChartXYData* line = new ChartXYData();
	//std::vector<CMeDPoint> linePts;
	//linePts.push_back(CMeDPoint(0, 1));
	//linePts.push_back(CMeDPoint(2, 1));
	//linePts.push_back(CMeDPoint(3, 10));
	//linePts.push_back(CMeDPoint(4, 7));
	//line->setXYData(linePts);
	//m_charxy->addChartData(line);
	CRect rect;
	GetClientRect(rect);
	CSize size(rect.Width(), rect.Height());
	m_charxy->resize(size);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
void ChartFigureDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CBaseDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: �ڴ˴������Ϣ����������
	TRACE("[GaugeDlg] OnActivate!\r\n");
}
// ChartFigureDlg ��Ϣ�������
void ChartFigureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SetCapture();
	if (m_charxy)
	{
		m_charxy->mousePressEvent(point);
		//Invalidate();
	}

	CWnd* pWnd = AfxGetMainWnd();
	if (NULL != pWnd)
	{
		pWnd->PostMessage(WM_NOTIFY_ACTIVE_WND_TYPE,2,0);
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void ChartFigureDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_charxy)
	{
		m_charxy->mouseReleaseEvent(point);
		//Invalidate();
	}
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}


BOOL ChartFigureDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_charxy)
	{
		CPoint pts = pt;
		ScreenToClient(&pts);
		m_charxy->wheelEvent(pts, zDelta);
		Invalidate();
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}


void ChartFigureDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_charxy)
	{
		m_charxy->mouseMoveEvent(point);
		//Invalidate();
	}
	CWnd::OnMouseMove(nFlags, point);
}


void ChartFigureDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	CDialog::OnPaint();
	if (m_charxy)
		m_charxy->paintEvent();
	//���߿�
	CWnd* parent = GetParent();
	if(parent)
	{
		parent = parent->GetParent();
		CDlgTabPanel* pTabPanel = dynamic_cast<CDlgTabPanel*>(parent);
		if(pTabPanel)
		{
			CRect rc;
			GetClientRect(rc);
			CRgn rgn;
			rgn.CreateRectRgnIndirect(rc);
			dc.SelectClipRgn(&rgn);
			if(this==pTabPanel->GetActiveDlg())//��ǰ���ڼ���
			{
				CPen BoradrPen;
				BoradrPen.CreatePen(PS_SOLID,5,ActiveColor);
				CPen* pOldPen = dc.SelectObject(&BoradrPen);
				CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
				CBrush *pOldBrush = dc.SelectObject(pBrush);  
				dc.Rectangle(&rc);
				dc.SelectObject(pOldPen);
				dc.SelectObject(pOldBrush);
				BoradrPen.DeleteObject();
			}else
			{
				CPen BoradrPen;
				BoradrPen.CreatePen(PS_SOLID,5,UnActiveColor);
				CPen* pOldPen = dc.SelectObject(&BoradrPen);
				CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
				CBrush *pOldBrush = dc.SelectObject(pBrush);
				dc.Rectangle(&rc);
				dc.SelectObject(pOldPen);
				dc.SelectObject(pOldBrush);
				BoradrPen.DeleteObject();
			}
		}
	}
}


void ChartFigureDlg::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (m_charxy)
	{
		m_charxy->resize(CSize(cx, cy));
		Invalidate();
	}
}


void ChartFigureDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CWnd::OnSizing(fwSide, pRect);

	// TODO: �ڴ˴������Ϣ����������
	Invalidate();
}

void ChartFigureDlg::setChartMgr(ChartManager* mgr)
{
	if (m_charxy)
	{
		m_charxy->setChartMgr(mgr);
	}
}
const ChartManager* ChartFigureDlg::getChartMgr()
{
	if (m_charxy)
	{
		return m_charxy->getChartMgr();
	}
	return NULL;
}
void ChartFigureDlg::updateData(CGlobalDataManager* dbMgr) const
{
	if (m_charxy)
	{
		return m_charxy->updateData(dbMgr);
	}
}
//��������
int ChartFigureDlg::saveData()const
{
	//���浱ǰͼ�������
	return 0;
}
//��ȡ����
int ChartFigureDlg::readData()
{
	//��ȡ��ǰͼ�������
	return 0;
}


void ChartFigureDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CMenu   menu;   //��������Ҫ�õ���cmenu����
	menu.LoadMenu(IDR_MENU_CHART); //װ���Զ�����Ҽ��˵� 
	CMenu   *pContextMenu=menu.GetSubMenu(0); //��ȡ��һ�������˵������Ե�һ���˵��������Ӳ˵� 
	if(pContextMenu)
	{
		//CPoint point1;//����һ������ȷ�����λ�õ�λ��  
		//GetCursorPos(&point1);//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����  
		pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this); 
	}
}


void ChartFigureDlg::OnChartSet()
{
	// TODO: Add your command handler code here
	DlgChartSet dlgSet(this);
	//��ʼ����ǰֵ
	dlgSet.m_nXID = m_charxy->getXID();
	dlgSet.m_eLineStyle = m_charxy->getLineStyle();
	dlgSet.m_eMoveStyle = m_charxy->getMoveStyle();
	dlgSet.m_eChartType = m_charxy->getChartType();
	std::map<int, bool> mapV = m_charxy->getMapVisible();
	dlgSet.m_setShowID.clear();
	for(std::map<int, bool>::iterator itr = mapV.begin(); itr!=mapV.end(); ++itr)
	{
		if(itr->second)
		{
			dlgSet.m_setShowID.insert(itr->first);
		}
	}
	if(IDOK==dlgSet.DoModal())
	{
		std::map<int, bool> mapV;
		SET<int> setV = dlgSet.m_setShowID;
		for(SET<int>::iterator itr = setV.begin(); itr!=setV.end(); ++itr)
		{
			mapV[*itr] = true;
		}
		m_charxy->setMapVisible(mapV);

		m_charxy->setXID(dlgSet.m_nXID);
		m_charxy->setMoveStyle(dlgSet.m_eMoveStyle);
		m_charxy->setChartType(dlgSet.m_eChartType);
		m_charxy->setLineStyle(dlgSet.m_eLineStyle);
		m_charxy->refreshData();
		Invalidate();
	}
}


void ChartFigureDlg::OnChartDel()
{
	// TODO: Add your command handler code here
	int a;
	 a = 0;
}


void ChartFigureDlg::OnChartZoomIn()
{
	// TODO: Add your command handler code here
}


void ChartFigureDlg::OnChartZoomOut()
{
	// TODO: Add your command handler code here
}


void ChartFigureDlg::OnChartShowAll()
{
	// TODO: Add your command handler code here
}
