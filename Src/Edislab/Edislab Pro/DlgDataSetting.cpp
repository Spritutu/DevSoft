// DlgDataSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgDataSetting.h"
#include <boost/foreach.hpp>
#include <algorithm>
#include "GridColumnGroupManager.h"
#include "Utility.h"
#include "Msg.h"
#include "DlgDataGroupProperty.h"
//����ID
static int s_nTreeID = 10001;

const int TREE_MAX_GROUP_NUM = 8;

// CDlgDataSetting �Ի���

IMPLEMENT_DYNAMIC(CDlgDataSetting, CBaseDialog)

CDlgDataSetting::CDlgDataSetting(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgDataSetting::IDD, pParent)
{
	m_strDefaultGroupNameArray.clear();

	DEFAULT_GROUP_NAME TempGroupName;
	TempGroupName.strName = _T("��ʷ��_1");
	m_strDefaultGroupNameArray.push_back(TempGroupName);

	TempGroupName.strName = _T("��ʷ��_2");
	m_strDefaultGroupNameArray.push_back(TempGroupName);

	TempGroupName.strName = _T("��ʷ��_3");
	m_strDefaultGroupNameArray.push_back(TempGroupName);

	TempGroupName.strName = _T("��ʷ��_4");
	m_strDefaultGroupNameArray.push_back(TempGroupName);

	TempGroupName.strName = _T("��ʷ��_5");
	m_strDefaultGroupNameArray.push_back(TempGroupName);

	TempGroupName.strName = _T("��ʷ��_6");
	m_strDefaultGroupNameArray.push_back(TempGroupName);

	TempGroupName.strName = _T("��ʷ��_7");
	m_strDefaultGroupNameArray.push_back(TempGroupName);
}

CDlgDataSetting::~CDlgDataSetting()
{
}

void CDlgDataSetting::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_POS, m_TreeRect);
}


BEGIN_MESSAGE_MAP(CDlgDataSetting, CBaseDialog)
	ON_BN_CLICKED(IDC_BTN_QUIT, &CDlgDataSetting::OnBnClickedBtnQuit)
	ON_BN_CLICKED(IDC_BTN_DEL, &CDlgDataSetting::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_ADD_DATA_GROUP, &CDlgDataSetting::OnBnClickedBtnAddDataGroup)
	ON_BN_CLICKED(IDC_BTN_ADD_DATA_COLUMN, &CDlgDataSetting::OnBnClickedBtnAddDataColumn)
	ON_BN_CLICKED(IDC_BTN_OPT, &CDlgDataSetting::OnBnClickedBtnOpt)
END_MESSAGE_MAP()


// CDlgDataSetting ��Ϣ�������


BOOL CDlgDataSetting::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitDisplay();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgDataSetting::InitDisplay( void )
{
	CWnd* pWnd = GetDlgItem(IDC_BTN_SAVE_DATA);
	if (nullptr != pWnd && NULL != pWnd->GetSafeHwnd())
	{
		pWnd->EnableWindow(FALSE);
	}
	if (NULL == m_TreeRect.GetSafeHwnd())
	{
		return;
	}
	int nIndex = 1;
	int nChildIndex = 0;
	CRect rectTree;
	m_TreeRect.GetClientRect (&rectTree);
	m_TreeRect.MapWindowPoints(this, &rectTree);
	UINT nTreeStyles = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | TVS_LINESATROOT | TVS_HASLINES | TVS_HASBUTTONS;
	m_Tree.Create(nTreeStyles,rectTree,this,s_nTreeID++);
	std::vector<COLUMN_GROUP_INFO> DisplayInfoArray;
	DisplayInfoArray.clear();
	CGridColumnGroupManager::CreateInstance().GetGridDisplayInfo(DisplayInfoArray);
	BOOST_FOREACH(auto& GroupElement,DisplayInfoArray)
	{
		HTREEITEM hGroupRoot = m_Tree.InsertItem(GroupElement.strGroupName);
		m_Tree.SetItemData(hGroupRoot,nIndex);
		++nIndex;
		BOOST_FOREACH(auto& ColumnElement,GroupElement.ColumnArray)
		{
			HTREEITEM hItem = m_Tree.InsertItem(ColumnElement.strColumnName,hGroupRoot);
			m_Tree.SetItemData(hItem,100 * nIndex + nChildIndex);
			++nChildIndex;
		}
	}

	m_Tree.SetVisualManagerColorTheme(TRUE);
	m_Tree.AdjustLayout ();
}

void CDlgDataSetting::OnBnClickedBtnQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}


void CDlgDataSetting::OnBnClickedBtnDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hSelectItem = m_Tree.GetSelectedItem();
	if (hSelectItem == NULL)
	{
		return;
	}

	//����ǵ�ǰ��
	if (1 == m_Tree.GetItemData(hSelectItem))
	{
		Utility::AfxBCGPMessageBox(_T("����ɾ����ǰ��!"),MB_OK | MB_ICONASTERISK);
		return;
	}

	//�ж�ѡ��İ����ӽڵ�Ľڵ�
	if (TRUE == m_Tree.ItemHasChildren(hSelectItem))
	{
		//��ȡ���������
		CString strGroupName = m_Tree.GetItemText(hSelectItem);

		//��Ĭ�Ϸ����ҵ�����һ����
		auto FindPred = [&strGroupName](const DEFAULT_GROUP_NAME& GroupElement)->bool
		{
			if (strGroupName == GroupElement.strName)
			{
				return true;
			}
			return false;
		};
		auto Iter = std::find_if(m_strDefaultGroupNameArray.begin(),m_strDefaultGroupNameArray.end(),FindPred);
		if (Iter != m_strDefaultGroupNameArray.end())
		{
			Iter->bUse = false;
		}
		m_Tree.DeleteItem(hSelectItem);
		m_Tree.AdjustLayout();
		CGridColumnGroupManager::CreateInstance().RemoveHeader(strGroupName);

		//֪ͨGridˢ��
		CWnd* pWnd = AfxGetMainWnd();
		if (nullptr != pWnd)
		{
			pWnd->PostMessage(WM_NOTIFY_GRID_GROUP_INFO_CHANGE,0,0);
		}
		return;
	}

	//�����Ҷ�ӽڵ�
	//��ȡ����������
	HTREEITEM hParentSelectItem = m_Tree.GetParentItem(hSelectItem);
	CString strColumnName = m_Tree.GetItemText(hSelectItem);
	CString strGroupName = m_Tree.GetItemText(hParentSelectItem);
	m_Tree.DeleteItem(hSelectItem);

	//�ж��丸�ڵ��Ƿ���ȫɾ����
	int nChildItemNum = 0;

	HTREEITEM hChildItem = m_Tree.GetChildItem(hParentSelectItem);

	while (hChildItem)
	{
		if (m_Tree.ItemHasChildren(hChildItem) == FALSE && hParentSelectItem == m_Tree.GetParentItem(hChildItem))
		{
			++nChildItemNum;
		}

		hChildItem = m_Tree.GetNextSiblingItem(hChildItem);
	}

	//���ڵ��Ѿ�û���κ��ӽڵ���
	if (nChildItemNum == 0)
	{
		m_Tree.DeleteItem(hParentSelectItem);

		//��Ĭ�Ϸ����ҵ�����һ����
		auto FindPred = [&strGroupName](const DEFAULT_GROUP_NAME& GroupElement)->bool
		{
			if (strGroupName == GroupElement.strName)
			{
				return true;
			}
			return false;
		};
		auto Iter = std::find_if(m_strDefaultGroupNameArray.begin(),m_strDefaultGroupNameArray.end(),FindPred);
		if (Iter != m_strDefaultGroupNameArray.end())
		{
			Iter->bUse = false;
		}
	}
	m_Tree.AdjustLayout();
	CGridColumnGroupManager::CreateInstance().RemoveColumnInfo(strGroupName,strColumnName);
	//֪ͨGridˢ��
	CWnd* pWnd = AfxGetMainWnd();
	if (nullptr != pWnd)
	{
		pWnd->PostMessage(WM_NOTIFY_GRID_GROUP_INFO_CHANGE,0,0);
	}
}


void CDlgDataSetting::OnBnClickedBtnAddDataGroup()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (TREE_MAX_GROUP_NUM == GetTotalRootNum())
	{
		Utility::AfxBCGPMessageBox(_T("�޷����,��������,���֧��8��������."),MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	int nRootNum = GetTotalRootNum();
	int nIndex = 0;
	auto FindAvailableName = [](const DEFAULT_GROUP_NAME& Element)->bool
	{
		//û��ʹ�õ�
		if (!Element.bUse)
		{
			return true;
		}
		return false;
	};

	//��ȡ����Ϣ
	std::vector<COLUMN_GROUP_INFO> DisplayInfoArray;
	DisplayInfoArray.clear();
	CGridColumnGroupManager::CreateInstance().GetGridDisplayInfo(DisplayInfoArray);
	if (DisplayInfoArray.empty())
	{
		return;
	}
	COLUMN_GROUP_INFO TempleteGroup = DisplayInfoArray[0];
	auto FindIter = std::find_if(m_strDefaultGroupNameArray.begin(),m_strDefaultGroupNameArray.end(),FindAvailableName);
	if (FindIter != m_strDefaultGroupNameArray.end())
	{
		FindIter->bUse = true;
		HTREEITEM hItem = m_Tree.InsertItem(FindIter->strName);
		m_Tree.SetItemData(hItem,++nRootNum);
		BOOST_FOREACH(auto& ColumnElement,TempleteGroup.ColumnArray)
		{
			HTREEITEM hChildItem = m_Tree.InsertItem(ColumnElement.strColumnName,hItem);
			m_Tree.SetItemData(hChildItem,nRootNum * 100 + nIndex);
			++nIndex;
		}
		m_Tree.AdjustLayout();
		//��ӷ�����Ϣ
		TempleteGroup.strGroupName = FindIter->strName;
		CGridColumnGroupManager::CreateInstance().AddDisplayColumnInfo(TempleteGroup);
		//֪ͨGridˢ��
		CWnd* pWnd = AfxGetMainWnd();
		if (nullptr != pWnd)
		{
			pWnd->PostMessage(WM_NOTIFY_GRID_GROUP_INFO_CHANGE,0,0);
		}
	}
}

void CDlgDataSetting::OnBnClickedBtnAddDataColumn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

int CDlgDataSetting::GetTotalRootNum( void )
{
	if (NULL == m_Tree.GetSafeHwnd())
	{
		return 0;
	}
	int nNum = 0;

	HTREEITEM hItem = m_Tree.GetRootItem();

	while (hItem)
	{
		if (m_Tree.ItemHasChildren(hItem))
		{
			++nNum;
		}

		hItem = m_Tree.GetNextSiblingItem(hItem);
	}

	return nNum;
}


void CDlgDataSetting::OnBnClickedBtnOpt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (NULL == m_Tree.GetSafeHwnd())
	{
		return;
	}
	HTREEITEM hSelectItem = m_Tree.GetSelectedItem();

	if (NULL == hSelectItem)
	{
		return;
	}

	//�ж��Ƿ�����ڵ�
	if (TRUE == m_Tree.ItemHasChildren(hSelectItem))
	{
		CString strGroupName = m_Tree.GetItemText(hSelectItem);

		CDlgDataGroupProperty Dlg(strGroupName);

		if (IDOK == Dlg.DoModal())
		{
			strGroupName = Dlg.GetGroupName();

			m_Tree.SetItemText(hSelectItem,strGroupName);

			m_Tree.AdjustLayout();

			//֪ͨGrid�����޸�
		}
	}

}
