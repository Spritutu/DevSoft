#include "stdafx.h"
#include "GridCtrlBase.h"


CGridCtrlBase::CGridCtrlBase(void)
{
}


CGridCtrlBase::~CGridCtrlBase(void)
{
}

BEGIN_MESSAGE_MAP(CGridCtrlBase, CBCGPGridCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CBCGPGridRow* CGridCtrlBase::CreateNewRow()
{
	//��ȡ����
	int nColumnNum = GetColumnCount();

	//�����µ�һ��
	CBCGPGridRow* pRow = CreateRow(nColumnNum);

	CBCGPGridItem* pItem = NULL;
	if (NULL != pRow)
	{
		for (int i = 0; i < nColumnNum; ++i)
		{
			pItem = pRow->GetItem(i);
			if (NULL != pItem)
			{
				pItem->AllowEdit (FALSE);
			}	
		}
	}
	return pRow;
}

int CGridCtrlBase::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	EnableMarkSortedColumn(FALSE);
	EnableHeader(TRUE,BCGP_GRID_HEADER_SELECT);
	EnableRowHeader(TRUE );
	EnableLineNumbers(TRUE);
	SetClearInplaceEditOnEnter(FALSE);
	EnableInvertSelOnCtrl (FALSE);
	SetScrollBarsStyle (CBCGPScrollBar::BCGP_SBSTYLE_VISUAL_MANAGER);
	SetSingleSel(TRUE);
	SetGridItemReadOnly(true);
	//������ͷ
	CRect rc;
	GetClientRect(rc);
	int nColumnNum = static_cast<int>(m_ColumnTitleArray.GetSize());
	for (int i = 0; i < nColumnNum; ++i)
	{
		InsertColumn (i,m_ColumnTitleArray[i], rc.Width() / nColumnNum);
	}

	return 0;
}

void CGridCtrlBase::OnLButtonDown( UINT nFlags, CPoint point )
{
	if (FALSE == m_bReadOnly)
	{
		SetGridItemReadOnly(true);
	}

	CBCGPGridCtrl::OnLButtonDown(nFlags, point);
}

void CGridCtrlBase::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	SetGridItemReadOnly(false);
	CBCGPGridCtrl::OnLButtonDblClk(nFlags, point);
}

void CGridCtrlBase::OnContextMenu( CWnd* pWnd, CPoint point )
{
	CBCGPGridCtrl::OnContextMenu(pWnd, point);
}

/************************************************************************************************************
��������:AddColumnTitle
��������:����������
�������:strColumTitle:�е�����  nIndex:Ҫ���������
�������:��
����ֵ:��
************************************************************************************************************/
void CGridCtrlBase::AddColumnTitle( const CString& strColumTitle,int nIndex /*= -1*/ )
{
	if (-1 == nIndex)
	{
		m_ColumnTitleArray.Add(strColumTitle);
	}
	else
	{
		m_ColumnTitleArray.InsertAt(nIndex,strColumTitle);
	}
}

void CGridCtrlBase::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridCtrl::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	int nColumnNum = GetColumnCount();

	//�����п�
	for (int i = 0; i < nColumnNum; ++i)
	{
		SetColumnWidth(i,(cx - 30) / nColumnNum);
	}
}

/************************************************************************************************************
��������:OnItemChanged
��������:Grid��ı䴦����
�������:��
�������:��
����ֵ:��
************************************************************************************************************/
void CGridCtrlBase::OnItemChanged( CBCGPGridItem*pItem,int nRow,int nColumn )
{
	//�����Ϸ����ж�
	if (NULL != pItem)
	{
		//������Ϣ���߸�����
		CWnd* pParentWnd = GetParent();

		if (NULL != pParentWnd)
		{
			//pParentWnd->SendMessage(WM_NOTIFY_GRID_ITEM_CHANGE_MSG,(WPARAM)nRow,(LPARAM)nColumn);
		}
	}
}

/************************************************************************************************************
��������:OnLButtonUp
��������:��Ӧ�������
�������:strColumTitle:�е�����  nIndex:Ҫ���������
�������:��
����ֵ:��
************************************************************************************************************/
void CGridCtrlBase::OnLButtonUp(UINT nFlags, CPoint point)
{
	//�����ж���ͷ�Ƿ�ѡ��
	CBCGPGridRow* pRow = HitTestRowHeader(point);

	if (NULL != pRow)
	{
		if (TRUE == pRow->IsSelected())
		{
			//������Ϣ���߸�����
			CWnd* pParentWnd = GetParent();

			if (NULL != pParentWnd)
			{
				//pParentWnd->SendMessage(WM_NOTIFY_GRID_SELECT_ROW_MSG,(WPARAM)(pRow->GetRowId()),0);	
			}
		}
		CBCGPGridCtrl::OnLButtonUp(nFlags, point);
		return;
	}
	//�ж��Ƿ�ѡ��ĳһ��
	CBCGPGridItemID ItemID;
	CBCGPGridItem* pItem = NULL;
	pRow = HitTest(point,ItemID,pItem);

	//���ѡ��ĳһ��
	if (NULL != pRow)
	{
		if (TRUE == pRow->IsSelected())
		{
			//������Ϣ���߸�����
			CWnd* pParentWnd = GetParent();

			//ѡ����ĳһ��ŷ�����Ϣ��ȥ
			if (NULL != pItem)
			{
				if (NULL != pParentWnd)
				{
					//pParentWnd->SendMessage(WM_NOTIFY_GRID_SELECT_ROW_MSG,(WPARAM)(ItemID.m_nRow),0);
				}	
			}
		}
	}
	CBCGPGridCtrl::OnLButtonUp(nFlags, point);
}

/************************************************************************************************************
��������:SetGridItemReadOnly
��������:����Grid�Ƿ�ɶ�
�������:bReadOnly:true:�ɶ� false:���ɶ�
�������:��
����ֵ:��
************************************************************************************************************/
void CGridCtrlBase::SetGridItemReadOnly( bool bReadOnly )
{
	if (bReadOnly)
	{
		m_bReadOnly = TRUE;
	}
	else
	{
		m_bReadOnly = FALSE;
	}
}

int CGridCtrlBase::s_nGridID = 10000;