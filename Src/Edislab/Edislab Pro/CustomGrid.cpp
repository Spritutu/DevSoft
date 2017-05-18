/**********************************************************
*��Ȩ����:xiaowei.han
*�ļ�����:CustomGrid.cpp
*��������:�Զ����Grid�ؼ�
*����:xiaowei.han
*����:2017/04/22
**********************************************************/
#include "stdafx.h"
#include "CustomGrid.h"
#include <algorithm>
#include <boost/foreach.hpp>
//Ĭ�������еĿ��
const int DEFAULT_COLUMN_WIDTH = 20;
IMPLEMENT_DYNCREATE(CCustomGrid, CBCGPGridCtrl)
CCustomGrid::CCustomGrid():m_nDisplayVitrualRows(60),
m_pCallBack(nullptr)
{
	m_HeaderInfoArray.clear();
}

CCustomGrid::~CCustomGrid()
{
	m_HeaderInfoArray.clear();
}

CBCGPGridColumnsInfo& CCustomGrid::GetColumnsInfo()
{
	return m_ColumnsEx;
}

const CBCGPGridColumnsInfo& CCustomGrid::GetColumnsInfo() const
{
	return m_ColumnsEx;
}

void CCustomGrid::DrawHeaderItem(CDC* pDC, CRect rect, CBCGPHeaderItem* pHeaderItem)
{
	CBCGPGridCtrl::DrawHeaderItem(pDC,rect,pHeaderItem);
}

int CCustomGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	EnableMarkSortedColumn(TRUE, FALSE);
	EnableHeader(TRUE, BCGP_GRID_HEADER_HIDE_ITEMS);
	SetClearInplaceEditOnEnter(FALSE);
	EnableInvertSelOnCtrl();
	SetScrollBarsStyle(CBCGPScrollBar::BCGP_SBSTYLE_VISUAL_MANAGER);
	ShowHorzScrollBar(TRUE);
	ShowVertScrollBar(TRUE);
	EnableRowHeader(TRUE);
	EnableLineNumbers(TRUE);
	SetEditFirstClick(FALSE);
	SetWholeRowSel(TRUE);
	m_ColumnsEx.EnableAutoSize(TRUE);

	if (nullptr != m_pCallBack)
	{
		EnableVirtualMode(m_pCallBack,(LPARAM)this);
		SetVirtualRows(m_nDisplayVitrualRows);
	}

	CreateHeaderInfo();
	CreateColumnInfo();
	AdjustLayout();
	return 0;
}
CRect CCustomGrid::OnGetHeaderRect (CDC* pDC, const CRect& rectDraw)
{
	CRect rect = CBCGPGridCtrl::OnGetHeaderRect(pDC, rectDraw);
	rect.bottom = rect.top + rect.Height() * m_ColumnsEx.GetHeaderLineCount();
	return rect;
}

void CCustomGrid::OnDrawHeader (CDC* pDC)
{
	if (nullptr == pDC)
	{
		return;
	}
	m_ColumnsEx.PrepareDrawHeader();
	CBCGPGridCtrl::OnDrawHeader(pDC);
}

void CCustomGrid::OnPrintHeader(CDC* pDC, CPrintInfo* pInfo)
{
	m_ColumnsEx.PrepareDrawHeader();
	CBCGPGridCtrl::OnPrintHeader(pDC, pInfo);
}

void CCustomGrid::OnPosSizeChanged()
{
	CBCGPGridCtrl::OnPosSizeChanged();
	m_ColumnsEx.ReposHeaderItems();
}
BEGIN_MESSAGE_MAP(CCustomGrid, CBCGPGridCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/*******************************************************************
*��������:SetHeaderInfoArray
*��������:�����б���ͷ��Ϣ(��δ����֮ǰ����)
*�������:HeaderInfoArray:��ͷ��Ϣ
*�������:None
*����ֵ:None
*����:xiaowei.han
*����:2017/05/13 15:56:55
*******************************************************************/
void CCustomGrid::SetHeaderInfoArray(const std::vector<HEADRER_INFO>& HeaderInfoArray)
{
	if (HeaderInfoArray.empty())
	{
		return;
	}
	m_HeaderInfoArray.assign(HeaderInfoArray.begin(),HeaderInfoArray.end());
}

/*******************************************************************
*��������:AddHeaderInfo
*��������:��̬���ӱ�ͷ��Ϣ
*�������:HeaderInfo:��ͷ��Ϣ
*�������:None
*����ֵ:None
*����:xiaowei.han
*����:2017/05/13 15:57:53
*******************************************************************/
void CCustomGrid::AddHeaderInfo(const HEADRER_INFO& HeaderInfo)
{
	//�����Ϸ����ж�
	if (HeaderInfo.ContainColumnIndexArray.empty() || NULL == GetSafeHwnd())
	{
		return;
	}
	auto Pred = [&HeaderInfo](const HEADRER_INFO& Info)->bool
	{
		if (HeaderInfo.strHeadName == Info.strHeadName)
		{
			return true;
		}
		return false;
	};
	auto Iter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),Pred);
	//����Ѿ�����
	if (Iter != m_HeaderInfoArray.end())
	{	
		return;
	}
	m_HeaderInfoArray.push_back(HeaderInfo);
	//��ȡǰ���еĸ���
	int nTotalColumnCounts = m_ColumnsEx.GetColumnCount();
	//��Ҫ�ϲ�������Ϣ
	CArray<int, int> MergeColumnsArray;
	MergeColumnsArray.RemoveAll();
	int nColumnIdex = nTotalColumnCounts;
	BOOST_FOREACH(auto& ColumnName,HeaderInfo.ContainColumnIndexArray)
	{

		m_ColumnsEx.InsertColumn(nColumnIdex,ColumnName,DEFAULT_COLUMN_WIDTH);
		m_ColumnsEx.SetColumnWidthAutoSize(nColumnIdex,TRUE);
		MergeColumnsArray.Add(nColumnIdex);
		++nColumnIdex;
	}
	CArray<int, int> MergeLinesArray;
	MergeLinesArray.RemoveAll();
	MergeLinesArray.Add (0);
	m_ColumnsEx.AddHeaderItem (&MergeColumnsArray,&MergeLinesArray,-1,HeaderInfo.strHeadName,HDF_CENTER);
	AdjustLayout();
}

/*******************************************************************
*��������:RemoveHeaderInfo
*��������:��̬�Ƴ���ͷ
*�������:strHeaderName�����Ƴ��ı�ͷ����
*�������:None
*����ֵ:None
*����:xiaowei.han
*����:2017/05/13 15:59:47
*******************************************************************/
void CCustomGrid::RemoveHeaderInfo(const CString& strHeaderName)
{
	if (TRUE == strHeaderName.IsEmpty())
	{
		return;
	}
	//������ȫ��ɾ����ͷ
	if (1 == m_HeaderInfoArray.size())
	{
		return;
	}
	//���ڲ��ҵ�lamda���ʽ
	auto Pred = [&strHeaderName](const HEADRER_INFO& Info)->bool
	{
		if (strHeaderName == Info.strHeadName)
		{
			return true;
		}
		return false;
	};
	auto Iter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),Pred);
	//���û���ҵ�
	if (Iter == m_HeaderInfoArray.end())
	{
		return;
	}
	//����ҵ�
	m_HeaderInfoArray.erase(Iter);
	//ɾ����������Ϣ
	m_ColumnsEx.RemoveAllHeaderItems();
	m_ColumnsEx.DeleteAllColumns();
	//����������
	CreateHeaderInfo();
	CreateColumnInfo();
	AdjustLayout();
}

/*******************************************************************
*��������:RemoveColumn
*��������:��̬�Ƴ���
*�������:strColumnName�����Ƴ���������
*�������:None
*����ֵ:None
*����:xiaowei.han
*����:2017/05/13 15:59:47
*******************************************************************/
void CCustomGrid::RemoveColumn(const CString& strColumnName)
{
	if (TRUE == strColumnName.IsEmpty() || NULL == GetSafeHwnd())
	{
		return;
	}
	//����ɾ�����е���
	RemoveAll();
	//����ĳ�е������Ƿ����Ҫɾ����
	auto Pred = [&strColumnName](const HEADRER_INFO& Info)->bool
	{
		bool bResult = false;
		for (int i = 0; i < (int)Info.ContainColumnIndexArray.size(); ++i)
		{
			if (Info.ContainColumnIndexArray[i] == strColumnName)
			{
				bResult = true;
				break;
			}
		}
		return bResult;
	};
	

	auto Iter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),Pred);

	if (Iter == m_HeaderInfoArray.end())
	{
		return;
	}

	//�ҵ��Ļ�
	auto FindIter = std::find(Iter->ContainColumnIndexArray.begin(),Iter->ContainColumnIndexArray.end(),strColumnName);

	if (FindIter != Iter->ContainColumnIndexArray.end())
	{
		Iter->ContainColumnIndexArray.erase(FindIter);
	}

	m_ColumnsEx.RemoveAllHeaderItems();
	m_ColumnsEx.DeleteAllColumns();
	CreateHeaderInfo();
	CreateColumnInfo();
	AdjustLayout();
}

/*******************************************************************
*��������:AddColumnInfo
*��������:��̬������
*�������:strColumnName�������ӵ������� strHeaderName:������������ͷ������
*�������:None
*����ֵ:None
*����:xiaowei.han
*����:2017/05/13 15:59:47
*******************************************************************/
void CCustomGrid::AddColumnInfo(const CString& strHeaderName,const CString& strColumnName)
{
	if (strHeaderName.IsEmpty() || strColumnName.IsEmpty())
	{
		return;
	}
	//������ͷ
	//����ɾ�����е���
	RemoveAll();
	//����ĳ�е������Ƿ����Ҫɾ����
	auto HeaderPred = [&strHeaderName](const HEADRER_INFO& Info)->bool
	{
		if (Info.strHeadName == strHeaderName)
		{
			return true;
		}
		return false;
	};
	auto HeaderIter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),HeaderPred);
	if (HeaderIter == m_HeaderInfoArray.end())
	{
		return;
	}


	auto ColumnPred = [&strColumnName](const CString& strName)->bool
	{
		if (strName == strColumnName)
		{
			return true;
		}
		return false;
	};

	auto ColumnIter = std::find_if(HeaderIter->ContainColumnIndexArray.begin(),HeaderIter->ContainColumnIndexArray.end(),ColumnPred);
	//������
	if (ColumnIter == HeaderIter->ContainColumnIndexArray.end())
	{
		HeaderIter->ContainColumnIndexArray.push_back(strColumnName);
	}

	//ɾ����������Ϣ
	m_ColumnsEx.RemoveAllHeaderItems();
	m_ColumnsEx.DeleteAllColumns();
	//����������
	CreateHeaderInfo();
	CreateColumnInfo();

	AdjustLayout();
}

/*******************************************************************
*��������:IsHeaderNameExist
*��������:�ж���ͷ�Ƿ��Ѿ�����
*�������:strHeaderName:��ͷ������
*�������:None
*����ֵ:true:���� false:������
*����:xiaowei.han
*����:2017/05/13 15:59:47
*******************************************************************/
bool CCustomGrid::IsHeaderNameExist(const CString& strHeaderName)
{
	if (strHeaderName.IsEmpty())
	{
		return false;
	}
	//������ͷ��Ϣ
	auto HeaderPred = [&strHeaderName](const HEADRER_INFO& Info)->bool
	{
		if (Info.strHeadName == strHeaderName)
		{
			return true;
		}
		return false;
	};
	auto HeaderIter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),HeaderPred);
	if (HeaderIter != m_HeaderInfoArray.end())
	{
		return true;
	}
	return false;
}

/*******************************************************************
*��������:IsColumnExistInFixedHeader
*��������:�ж����Ƿ��Ѿ�����
*�������:strHeaderName:��ͷ������ strColumnName:��ͷ����
*�������:None
*����ֵ:true:���� false:������
*����:xiaowei.han
*����:2017/05/13 15:59:47
*******************************************************************/
bool CCustomGrid::IsColumnExistInFixedHeader(const CString& strHeaderName,const CString& strColumnName)
{
	if (strHeaderName.IsEmpty() || strColumnName.IsEmpty())
	{
		return false;
	}

	//����ĳ�е������Ƿ����Ҫɾ����
	auto HeaderPred = [&strHeaderName](const HEADRER_INFO& Info)->bool
	{
		if (Info.strHeadName == strHeaderName)
		{
			return true;
		}
		return false;
	};
	auto HeaderIter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),HeaderPred);
	if (HeaderIter == m_HeaderInfoArray.end())
	{
		return false;
	}

	auto ColumnPred = [&strColumnName](const CString& strName)->bool
	{
		if (strName == strColumnName)
		{
			return true;
		}
		return false;
	};

	auto ColumnIter = std::find_if(HeaderIter->ContainColumnIndexArray.begin(),HeaderIter->ContainColumnIndexArray.end(),ColumnPred);
	//������
	if (ColumnIter == HeaderIter->ContainColumnIndexArray.end())
	{
		return false;
	}
	return true;
}

/*******************************************************************
*��������:GetHeaderInfo
*��������:��ȡ��ͷ��Ϣ
*�������:None
*�������:HeaderInfoArray��������ͷ����Ϣ
*����ֵ:None
*����:xiaowei.han
*����:2017/05/13 16:03:08
*******************************************************************/
void CCustomGrid::GetHeaderInfo(std::vector<HEADRER_INFO>& HeaderInfoArray)
{
	HeaderInfoArray.clear();

	HeaderInfoArray.assign(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end());
}


void CCustomGrid::CreateColumnInfo(void)
{

	if (m_HeaderInfoArray.empty())
	{
		return;
	}
	int nColumnIdex = 0;
	BOOST_FOREACH(auto& Element,m_HeaderInfoArray)
	{	
		BOOST_FOREACH(auto& ColumnName,Element.ContainColumnIndexArray)
		{
			m_ColumnsEx.InsertColumn(nColumnIdex,ColumnName,20);
			m_ColumnsEx.SetColumnWidthAutoSize(nColumnIdex,TRUE);
			++nColumnIdex;
		}
	}

}

void CCustomGrid::CreateHeaderInfo(void)
{
	if (m_HeaderInfoArray.empty())
	{
		return;
	}
	m_ColumnsEx.SetHeaderLineCount(2);
	int nColumnIdex = 0;
	//��Ҫ�ϲ�������Ϣ
	CArray<int, int> MergeColumnsArray;
	CArray<int, int> MergeLinesArray;
	BOOST_FOREACH(auto& Element,m_HeaderInfoArray)
	{
		//��֤���ͷ���в�Ϊ��
		if (!Element.ContainColumnIndexArray.empty())
		{
			MergeColumnsArray.RemoveAll();
			BOOST_FOREACH(auto& ColumnName,Element.ContainColumnIndexArray)
			{
				MergeColumnsArray.Add(nColumnIdex++);
			}
			MergeLinesArray.RemoveAll();
			MergeLinesArray.Add (0);
			m_ColumnsEx.AddHeaderItem (&MergeColumnsArray,&MergeLinesArray,-1,Element.strHeadName,HDF_CENTER);
		}
	}
}

//Grid�ؼ���ID(����)
unsigned int CCustomGrid::s_GridID = 0;

void CCustomGrid::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridCtrl::OnSize(nType, cx, cy);

	if (NULL != GetSafeHwnd())
	{
		int nColumnNum = m_ColumnsEx.GetColumnCount();

		if (nColumnNum > 0)
		{
			AdjustLayout();
			Invalidate(TRUE);	
		}
	}
}




void CCustomGrid::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CBCGPGridCtrl::OnTimer(nIDEvent);
}


BOOL CCustomGrid::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	return CBCGPGridCtrl::PreTranslateMessage(pMsg);
}
