/**********************************************************
*��Ȩ����:xiaowei.han
*�ļ�����:CustomGrid.cpp
*��������:�Զ����Grid�ؼ�
*����:xiaowei.han
*����:2017/04/22
**********************************************************/
#include "stdafx.h"
#include "Edislab Pro.h"
#include "CustomGrid.h"
#include <algorithm>
#include <boost/foreach.hpp>
#include "SensorIDGenerator.h"
#include "Utility.h"
#include "Msg.h"
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
	//EnableMarkSortedColumn(TRUE,TRUE);
	EnableHeader(TRUE, BCGP_GRID_HEADER_HIDE_ITEMS | BCGP_GRID_HEADER_SELECT);
	EnableRowHeader (TRUE, BCGP_GRID_HEADER_MOVE_ITEMS | BCGP_GRID_HEADER_SELECT);
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
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
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
void CCustomGrid::SetHeaderInfoArray(const std::vector<COLUMN_GROUP_INFO>& HeaderInfoArray)
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
void CCustomGrid::AddHeaderInfo(const COLUMN_GROUP_INFO& HeaderInfo)
{
	//�����Ϸ����ж�
	if (HeaderInfo.ColumnArray.empty() || NULL == GetSafeHwnd())
	{
		return;
	}
	auto Pred = [&HeaderInfo](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (HeaderInfo.strGroupName == Info.strGroupName)
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

	//����ɾ�����е���
	RemoveAll();
	//ɾ����������Ϣ
	m_ColumnsEx.RemoveAllHeaderItems();
	m_ColumnsEx.DeleteAllColumns();
	//����������
	CreateHeaderInfo();
	CreateColumnInfo();
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
	auto Pred = [&strHeaderName](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (strHeaderName == Info.strGroupName)
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
	//����ɾ�����е���
	RemoveAll();
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
	//����ĳ�е������Ƿ����Ҫɾ����
	auto Pred = [&strColumnName](const COLUMN_GROUP_INFO& Info)->bool
	{
		bool bResult = false;
		for (int i = 0; i < (int)Info.ColumnArray.size(); ++i)
		{
			if (Info.ColumnArray[i].strColumnName == strColumnName)
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


	auto ColumnPred = [&strColumnName](const COLUMN_INFO& ColumnElement)->bool
	{
		if (ColumnElement.strColumnName == strColumnName)
		{
			return true;
		}
		return false;
	};


	//�ҵ��Ļ�
	auto FindIter = std::find_if(Iter->ColumnArray.begin(),Iter->ColumnArray.end(),ColumnPred);

	if (FindIter != Iter->ColumnArray.end())
	{
		Iter->ColumnArray.erase(FindIter);
	}
	//����ɾ�����е���
	RemoveAll();
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
	//����ĳ�е������Ƿ����Ҫɾ����
	auto HeaderPred = [&strHeaderName](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == strHeaderName)
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


	auto ColumnPred = [&strColumnName](const COLUMN_INFO& ColumnElement)->bool
	{
		CString strAddColumnName = ColumnElement.strColumnName;
		std::string strTempColumnName = Utility::WideChar2MultiByte(strAddColumnName.GetBuffer(0));
		if (ColumnElement.strColumnName == strColumnName || CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(strTempColumnName) < 0)
		{
			return true;
		}
		return false;
	};

	auto ColumnIter = std::find_if(HeaderIter->ColumnArray.begin(),HeaderIter->ColumnArray.end(),ColumnPred);
	//������
	if (ColumnIter == HeaderIter->ColumnArray.end())
	{
		COLUMN_INFO TempColumnInfo;
		TempColumnInfo.strColumnName = strColumnName;
		HeaderIter->ColumnArray.push_back(TempColumnInfo);
	}
	//����
	else
	{
		//Ϊ�˷�ֹ�ҵ�X Y֮���
		if (ColumnIter->strColumnName != strColumnName)
		{
			ColumnIter->strColumnName = strColumnName;
		}
	}

	//����ɾ�����е���
	RemoveAll();
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
	auto HeaderPred = [&strHeaderName](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == strHeaderName)
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
	auto HeaderPred = [&strHeaderName](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == strHeaderName)
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

	auto ColumnPred = [&strColumnName](const COLUMN_INFO& ColumnElement)->bool
	{
		if (ColumnElement.strColumnName == strColumnName)
		{
			return true;
		}
		return false;
	};

	auto ColumnIter = std::find_if(HeaderIter->ColumnArray.begin(),HeaderIter->ColumnArray.end(),ColumnPred);
	//������
	if (ColumnIter == HeaderIter->ColumnArray.end())
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
void CCustomGrid::GetHeaderInfo(std::vector<COLUMN_GROUP_INFO>& HeaderInfoArray)
{
	HeaderInfoArray.clear();

	HeaderInfoArray.assign(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end());
}


void CCustomGrid::GetColumnGroupDisplayInfo(std::vector<SHOW_COLUMN_GROUP_INFO>& DisplayArray)
{
	DisplayArray.clear();
}

void CCustomGrid::SetColumnGroupDisplayInfo(const std::vector<SHOW_COLUMN_GROUP_INFO>& DisplayArray)
{
	if (DisplayArray.empty())
	{
		return;
	}
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
		BOOST_FOREACH(auto& ColumnElement,Element.ColumnArray)
		{
			m_ColumnsEx.InsertColumn(nColumnIdex,ColumnElement.strColumnName,20);
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
		if (!Element.ColumnArray.empty())
		{
			MergeColumnsArray.RemoveAll();
			BOOST_FOREACH(auto& ColumnName,Element.ColumnArray)
			{
				MergeColumnsArray.Add(nColumnIdex++);
			}
			MergeLinesArray.RemoveAll();
			MergeLinesArray.Add (0);
			m_ColumnsEx.AddHeaderItem (&MergeColumnsArray,&MergeLinesArray,-1,Element.strGroupName,HDF_CENTER);
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


void CCustomGrid::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CBCGPGridCtrl::OnRButtonDown(nFlags, point);
}


void CCustomGrid::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: �ڴ˴������Ϣ����������
	CWnd* pParentWnd = GetParent();

	if (nullptr != pParentWnd && NULL != pParentWnd->GetSafeHwnd())
	{
		pParentWnd->PostMessage(WM_NOTIFY_RBUTTON_DOWN,(WPARAM)point.x,(LPARAM)point.y);
	}
}
