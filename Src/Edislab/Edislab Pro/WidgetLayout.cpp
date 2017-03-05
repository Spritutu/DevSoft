#include "StdAfx.h"
#include "Edislab Pro.h"
#include "WidgetLayout.h"
#include <algorithm>
#include "ConcreteSplitAlgorithm.h"
//#include "LogImpl.h"
#include "SplitBar.h"
#include "Utility.h"
#if 0
//1�����ڷָ��㷨
static COneWidgetSplitAlgorithm s_OneSplit;
//2�����ڷָ��㷨
static CTwoWidgetSplitAlgorithm s_TwoSplit;
//3�����ڷָ��㷨
static CThreeWidgetSplitAlgorithm s_ThreeSplit;
//4�����ڷָ��㷨
static CFourWidgetSplitAlgorithm s_FourSplit;
//5�����ڷָ��㷨
static CFiveWidgetSplitAlgorithm s_FiveSplit;
//6�����ڷָ��㷨
static CSixWidgetSplitAlgorithm s_SixSplit;
//7�����ڷָ��㷨
static CSevenWidgetSplitAlgorithm s_SevenSplit;
//8�����ڷָ��㷨
static CEightWidgetSplitAlgorithm s_EightSplit;
//9�����ڷָ��㷨
static CNineWidgetSplitAlgorithm s_NineSplit;
#endif
//��󴰿���Ŀ
static int MAX_WIDGET_NUM = 9;
CWidgetLayout::CWidgetLayout( CWnd* pHostWnd,bool bWndFront /*= false*/ ):
m_bRemainingWndFront(bWndFront),
m_pHostWnd(pHostWnd),
m_pSplitAlgorithm(NULL)
{
	m_WidgetArray.clear();
	InitSplitBar();

}


CWidgetLayout::~CWidgetLayout(void)
{

	if (NULL != m_pSplitAlgorithm)
	{
		delete m_pSplitAlgorithm;
		m_pSplitAlgorithm = NULL;
	}
	m_WidgetArray.clear();
	UnInitSplitBar();
}

void CWidgetLayout::AddWidget( CWnd* pWidget )
{
	if (NULL == pWidget || NULL == m_pHostWnd || NULL == m_pHostWnd->GetSafeHwnd())
	{
		return;
	}

	//�ж��Ƿ����������ڵ��Ӵ��ڣ���������Ӵ����򲻽��в���
	if (NULL != m_pHostWnd)
	{
		if (FALSE == m_pHostWnd->IsChild(pWidget))
		{
			return;
		}
	}

	//���ܳ���9��
	if (static_cast<int>(m_WidgetArray.size()) >= MAX_WIDGET_NUM)
	{
		//CLogImpl::CreateInstance().Error_Log("the Widget Array is Full.");
		return;
	}

	WidgetArrayIter FindIter = std::find(m_WidgetArray.begin(),m_WidgetArray.end(),pWidget);

	if (FindIter == m_WidgetArray.end())
	{
		m_WidgetArray.push_back(pWidget);
	}
}

void CWidgetLayout::DelWidget( CWnd* pWidget )
{
	if (NULL == pWidget)
	{
		return;
	}

	WidgetArrayIter FindIter = std::find(m_WidgetArray.begin(),m_WidgetArray.end(),pWidget);

	if (FindIter != m_WidgetArray.end())
	{
		m_WidgetArray.erase(FindIter);
	}
}

void CWidgetLayout::AdjustLayout( int nWidth,int nHeight )
{
	
	if (NULL == m_pHostWnd)
	{
		return;
	}
	//��ȡ�����ڵĿͻ���
	CRect HostClientRc;
	m_pHostWnd->GetClientRect(&HostClientRc);


	//��ȡ�Ӵ��ڵĸ���
	int nWidghtNum = static_cast<int>(m_WidgetArray.size());

	switch (nWidghtNum)
	{
	case 1:
		{
			if (NULL != m_pSplitAlgorithm)
			{
				delete m_pSplitAlgorithm;
				m_pSplitAlgorithm = NULL;
			}
			m_pSplitAlgorithm = new COneWidgetSplitAlgorithm;
		}
		//m_pSplitAlgorithm = &s_OneSplit;
		break;
	case 2:
		{
			if (NULL != m_pSplitAlgorithm)
			{
				delete m_pSplitAlgorithm;
				m_pSplitAlgorithm = NULL;
			}
			m_pSplitAlgorithm = new CTwoWidgetSplitAlgorithm;
		}
		//m_pSplitAlgorithm = &s_TwoSplit;
		break;
	case 3:
		{
			if (NULL != m_pSplitAlgorithm)
			{
				delete m_pSplitAlgorithm;
				m_pSplitAlgorithm = NULL;
			}
			m_pSplitAlgorithm = new CThreeWidgetSplitAlgorithm;

		}
		//m_pSplitAlgorithm = &s_ThreeSplit;
		break;
	case 4:
		{
			if (NULL != m_pSplitAlgorithm)
			{
				delete m_pSplitAlgorithm;
				m_pSplitAlgorithm = NULL;
			}
			m_pSplitAlgorithm = new CFourWidgetSplitAlgorithm;
		}
		//m_pSplitAlgorithm = &s_FourSplit;
		break;
	case 5:
		{
			if (NULL != m_pSplitAlgorithm)
			{
				delete m_pSplitAlgorithm;
				m_pSplitAlgorithm = NULL;
			}
			m_pSplitAlgorithm = new CFiveWidgetSplitAlgorithm;
		}
		//m_pSplitAlgorithm = &s_FiveSplit;
		break;
	case 6:
		{
			if (NULL != m_pSplitAlgorithm)
			{
				delete m_pSplitAlgorithm;
				m_pSplitAlgorithm = NULL;
			}
			m_pSplitAlgorithm = new CSixWidgetSplitAlgorithm;
		}
		//m_pSplitAlgorithm = &s_SixSplit;
		break;
	case 7:
		{
			if (NULL != m_pSplitAlgorithm)
			{
				delete m_pSplitAlgorithm;
				m_pSplitAlgorithm = NULL;
			}
			m_pSplitAlgorithm = new CSevenWidgetSplitAlgorithm;
		}
		//m_pSplitAlgorithm = &s_SevenSplit;
		break;
	case 8:
		{
			if (NULL != m_pSplitAlgorithm)
			{
				delete m_pSplitAlgorithm;
				m_pSplitAlgorithm = NULL;
			}
			m_pSplitAlgorithm = new CEightWidgetSplitAlgorithm;
		}
		//m_pSplitAlgorithm = &s_EightSplit;
		break;
	case 9:
		{
			if (NULL != m_pSplitAlgorithm)
			{
				delete m_pSplitAlgorithm;
				m_pSplitAlgorithm = NULL;
			}
			m_pSplitAlgorithm = new CNineWidgetSplitAlgorithm;
		}
		//m_pSplitAlgorithm = &s_NineSplit;
		break;
	default:
		break;
	}

	if (NULL != m_pSplitAlgorithm)
	{
		m_pSplitAlgorithm->SetLayout(this);
		m_pSplitAlgorithm->SetHostWnd(m_pHostWnd);
		m_pSplitAlgorithm->SetWidgetArray(m_WidgetArray);
		m_pSplitAlgorithm->SetWndFront(m_bRemainingWndFront);
		m_pSplitAlgorithm->RecalculateLayout(nWidth,nHeight);
	}
}

void CWidgetLayout::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	if (NULL != m_pSplitAlgorithm)
	{
		m_pSplitAlgorithm->HandleLButtonDown(nFlags,pt,nWidth,nHeight);
	}
}

void CWidgetLayout::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	if (NULL != m_pSplitAlgorithm)
	{
		m_pSplitAlgorithm->HandleLButtonUp(nFlags,pt,nWidth,nHeight);
	}
}

void CWidgetLayout::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	if (NULL != m_pSplitAlgorithm)
	{
		m_pSplitAlgorithm->HandleMouseMove(nFlags,pt,nWidth,nHeight);
	}
}

void CWidgetLayout::CreateSplitBar( CWnd* pWnd )
{
	for (int i  = 0; i < SPLIT_BAR_NUM; ++i)
	{
		//�ָ�����δ����
		if (NULL != m_SplitArray[i] && NULL == m_SplitArray[i]->GetSafeHwnd())
		{
			m_SplitArray[i]->Create(CSplitBar::IDD,pWnd);
			m_SplitArray[i]->ShowWindow(SW_HIDE);
		}
	}
}

void CWidgetLayout::DestroySplitBar( void )
{
	for (int i  = 0; i < SPLIT_BAR_NUM; ++i)
	{
		//�ָ����Ѿ�����
		if (NULL != m_SplitArray[i] && NULL != m_SplitArray[i]->GetSafeHwnd())
		{
			m_SplitArray[i]->DestroyWindow();
		}
	}
}

void CWidgetLayout::InitSplitBar( void )
{
	for (int i = 0; i < SPLIT_BAR_NUM; ++i)
	{
		m_SplitArray[i] = new CSplitBar;
	}
}

void CWidgetLayout::UnInitSplitBar( void )
{
	for (int i = 0; i < SPLIT_BAR_NUM; ++i)
	{
		DELETE_POINTER(m_SplitArray[i]);
	}
}

CWnd* CWidgetLayout::GetMoveHorSplitBar( void )
{
	return m_SplitArray[0];
}

CWnd* CWidgetLayout::GetMoveVerSplitBar( void )
{
	return m_SplitArray[1];
}

void CWidgetLayout::InitLayout( CWnd* pWnd )
{
	CreateSplitBar(pWnd);
}

int CWidgetLayout::GetWidgetNum( void ) const
{
	return static_cast<int>(m_WidgetArray.size());
}
