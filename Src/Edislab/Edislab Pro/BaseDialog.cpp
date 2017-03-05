/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:BaseDialog.cpp
Description:Dialog�Ļ���,���жԻ��򶼴Ӵ�����
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#include "stdafx.h"
#include "BaseDialog.h"
#include "resource.h"
#include "DlgTabPanel.h"
#include "Global.h"
IMPLEMENT_DYNAMIC(CBaseDialog,CBCGPDialog)
CBaseDialog::CBaseDialog( UINT nIDTemplate, CWnd* pParentWnd,BOOL bEnableLayout):
CBCGPDialog(nIDTemplate,pParentWnd),
m_bActive(false)
{
	EnableLayout(bEnableLayout);
}

CBaseDialog::~CBaseDialog( void )
{
	//CBCGPMaskEdit
}


BOOL CBaseDialog::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	EnableVisualManagerStyle(TRUE,TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
BEGIN_MESSAGE_MAP(CBaseDialog, CBCGPDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


void CBaseDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	GetClientRect(&rc);
	
	if (m_bActive)
	{
		CPen BoradrPen;
		BoradrPen.CreatePen(PS_SOLID,4,ActiveColor);
		CPen* pOldPen = dc.SelectObject(&BoradrPen);
		CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));  
		CBrush *pOldBrush = dc.SelectObject(pBrush);  
		dc.Rectangle(&rc);
		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);
		BoradrPen.DeleteObject();
	}
	else
	{	
		CPen BoradrPen;
		BoradrPen.CreatePen(PS_SOLID,2,UnActiveColor);
		CPen* pOldPen = dc.SelectObject(&BoradrPen);
		CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));  
		CBrush *pOldBrush = dc.SelectObject(pBrush);  
		dc.Rectangle(&rc);
		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);
		BoradrPen.DeleteObject();
	}
}

BOOL CBaseDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	}
#if 1
	if (WM_LBUTTONDOWN == pMsg->message||WM_RBUTTONDOWN==pMsg->message)
	{
		CWnd* parent = GetParent();
		if(parent)
		{
			parent = parent->GetParent();
			CDlgTabPanel* pTabPanel = dynamic_cast<CDlgTabPanel*>(parent);
			if(pTabPanel)
			{
				pTabPanel->SetActiveDlg(this);
			}
		}
	}
#endif

	return CBCGPDialog::PreTranslateMessage(pMsg);
}

void CBaseDialog::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	Invalidate(TRUE);
}


void CBaseDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CBCGPDialog::OnLButtonDown(nFlags, point);
}

