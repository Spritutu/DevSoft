/**************************************************************************************************************************************************
*����:Grid�ؼ�����(����ͼGrid���)
*����:
*����:2015.8
**************************************************************************************************************************************************/

#include "stdafx.h"
#include "GridControlFactory.h"
#include "CustomLabGridCtrl.h"

/*************************************************************************************************
��������:GridCallback
��������:grid�ص�����
�������:
�������:
����ֵ: 
***************************************************************************************************/
static BOOL CALLBACK GridCallback (BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT (pdi != NULL);

	CCustomLabGridCtrl* pGridCtrl = (CCustomLabGridCtrl*) lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item

	//if(nCol>= 0&&nCol<100)
		//pdi->item.varValue = (pGridCtrl->m_GridDataManager->GetGridData())[nCol][nRow];

	//ˢ������
	if(nCol>= 0)
	{
		CString strTemp;
		GLOBAL_DATA.GetItemData(nCol,nRow,strTemp);
		pdi->item.varValue = strTemp;
	}

	return TRUE;
}

/*************************************************************************************************
��������:CGridControlFactory
��������:���캯��
�������:
�������:��
����ֵ: ��
***************************************************************************************************/
CGridControlFactory::CGridControlFactory(void)
{
	m_ViewManager = NULL;

	m_ViewManager = new CGridViewManager();
}

/*************************************************************************************************
��������:CGridControlFactory
��������:��������
�������:
�������:��
����ֵ: ��
***************************************************************************************************/
CGridControlFactory::~CGridControlFactory(void)
{

}

/*************************************************************************************************
��������:CreateGridCtrl
��������:Ϊ�������Grid�ؼ�
�������:_Parent : �����ھ��
�������:��
����ֵ: ��
***************************************************************************************************/
CWnd* CGridControlFactory::AddGridCtrl(CWnd* _Parent)
{
	if (NULL == _Parent)
		return NULL;

	CCustomLabGridCtrl* pGrid = NULL;
	pGrid = new CCustomLabGridCtrl();

	if (NULL == pGrid)
		return NULL;

	pGrid->SetViewManager(m_ViewManager);

	pGrid->SetCallBack(GridCallback);

	pGrid->Create (WS_VISIBLE |WS_CHILD, CRect(0,0,0,0), _Parent, 1);
	pGrid->ShowWindow(TRUE);

	m_ViewManager->AddGridView(pGrid);

	return pGrid;
}

//�Ƴ�����Grid�ؼ�
void CGridControlFactory::RemoveGridCtrl(CWnd* pWnd)
{
	//�ж���Ч��
	if(NULL != pWnd)
	{
		m_ViewManager->RemoveGridView((CCustomLabGridCtrl*)pWnd);
	}
}
