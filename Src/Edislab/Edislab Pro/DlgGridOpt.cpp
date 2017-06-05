// DlgGridOpt.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgGridOpt.h"
#include <boost/foreach.hpp>
// CDlgGridOpt �Ի���
//����ID
static int s_nTreeID = 10000;

IMPLEMENT_DYNAMIC(CDlgGridOpt, CBaseDialog)

CDlgGridOpt::CDlgGridOpt(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgGridOpt::IDD, pParent)
{
	m_GridDisplayArray.clear();
}

CDlgGridOpt::~CDlgGridOpt()
{
}

void CDlgGridOpt::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_LOCATION, m_DisplayItemTreeLocation);
}


BEGIN_MESSAGE_MAP(CDlgGridOpt, CBaseDialog)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgGridOpt::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgGridOpt::OnBnClickedBtnCancel)
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(BCGM_GRID_HEADERCHECKBOX_CLICK,&CDlgGridOpt::OnHeaderCheckClick)
	ON_REGISTERED_MESSAGE(BCGM_GRID_ROW_CHECKBOX_CLICK, &CDlgGridOpt::OnCheckClick)
END_MESSAGE_MAP()


// CDlgGridOpt ��Ϣ�������


BOOL CDlgGridOpt::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitDisplay();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

/*******************************************************************
*��������:InitDisplay
*��������:��ʼ���Ի�����ʾ
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/04 15:04:56
*******************************************************************/
void CDlgGridOpt::InitDisplay(void)
{
	if (NULL == m_DisplayItemTreeLocation.GetSafeHwnd())
	{
		return;
	}
	CRect rectTree;
	m_DisplayItemTreeLocation.GetClientRect (&rectTree);
	m_DisplayItemTreeLocation.MapWindowPoints(this, &rectTree);

	UINT nTreeStyles = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | TVS_LINESATROOT |
		TVS_HASLINES | TVS_HASBUTTONS | TVS_CHECKBOXES;
	m_GridDisplayItemTree.Create(nTreeStyles,rectTree,this,s_nTreeID++);

	//������ṹ
	BOOST_FOREACH(auto& V,m_GridDisplayArray)
	{
		HTREEITEM hGroupRoot = m_GridDisplayItemTree.InsertItem(V.strGroupName);


		if (NULL != hGroupRoot)
		{
			BOOST_FOREACH(auto& ColumnElement,V.ColumnArray)
			{
				HTREEITEM hSubItem = m_GridDisplayItemTree.InsertItem(ColumnElement.strColumnName,hGroupRoot);
				if (NULL != hSubItem)
				{
					if (ColumnElement.bShow)
					{
						m_GridDisplayItemTree.SetCheck(hSubItem,TRUE);
					}
					else
					{
						m_GridDisplayItemTree.SetCheck(hSubItem,FALSE);
					}
				}

			}
		}
	}

	for (int i = 0; i < m_GridDisplayItemTree.GetRowCount(); ++i)
	{
		CBCGPGridRow* pRow = m_GridDisplayItemTree.GetRow(i);

		if (NULL != pRow)
		{
			//�ж��Ƿ��Ƿ�ΪҶ�ӽڵ�
			if (pRow->GetSubItemsCount() == 0)
			{
				pRow->UpdateParentCheckbox();
			}
		}
	}
	m_GridDisplayItemTree.SetVisualManagerColorTheme(TRUE);
	m_GridDisplayItemTree.AdjustLayout ();
}


void CDlgGridOpt::OnBnClickedBtnApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}


void CDlgGridOpt::OnBnClickedBtnCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CDlgGridOpt::SetDisplayInfo(const std::vector<SHOW_COLUMN_GROUP_INFO>& GridDisplayArray)
{
	m_GridDisplayArray = GridDisplayArray;
}

void CDlgGridOpt::GetDisplayInfo(std::vector<SHOW_COLUMN_GROUP_INFO>& GridDisplayArray)
{
	GridDisplayArray.clear();

	GridDisplayArray = m_GridDisplayArray;
}


void CDlgGridOpt::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);
}

LRESULT CDlgGridOpt::OnHeaderCheckClick( WPARAM wp, LPARAM lp )
{
	BOOL bChecked = m_GridDisplayItemTree.GetColumnsInfo().GetCheckBoxState();

	m_GridDisplayItemTree.GetColumnsInfo().SetCheckBoxState(!bChecked);
	m_GridDisplayItemTree.CheckAll(!bChecked);

	return TRUE; // disable the default implementation
}

LRESULT CDlgGridOpt::OnCheckClick( WPARAM wp, LPARAM lp )
{
	CBCGPGridRow* pRow = (CBCGPGridRow*)lp;
	if (nullptr == pRow)
	{
		return 0;
	}

	ASSERT_VALID(pRow);

	if (pRow->HasCheckBox())
	{
		BOOL bChecked = pRow->GetCheck();

		// check the row
		pRow->SetCheck(!bChecked);

		// check the subitems
		pRow->CheckSubItems(!bChecked);

		// update parent items
		pRow->UpdateParentCheckbox(TRUE/* use 3 state checkboxes */);

		// update the header control
		m_GridDisplayItemTree.UpdateHeaderCheckbox();
	}

	return TRUE; // disable the default implementation
}
