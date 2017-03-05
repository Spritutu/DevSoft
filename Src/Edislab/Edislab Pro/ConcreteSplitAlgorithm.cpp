#include "StdAfx.h"
#include "ConcreteSplitAlgorithm.h"
#include "WidgetLayout.h"
#include "Utility.h"
const int DEFAULT_GAP = 4;
COneWidgetSplitAlgorithm::COneWidgetSplitAlgorithm(void)
{
}


COneWidgetSplitAlgorithm::~COneWidgetSplitAlgorithm(void)
{
}

/*****************************************************************************
@FunctionName : AdjustLayout
@FunctionDescription : ����Ӧ����
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void COneWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{

	if (m_WidgetArray.empty())
	{
		return;
	}

	//�ƶ��͸�����һ����
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget)
	{
		pWidget->MoveWindow(0,0,nWidth,nHeight,TRUE);
		pWidget->ShowWindow(SW_SHOW);
	}

}

void COneWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	CRect WidgetRect;

	WidgetRect.left = 0;
	WidgetRect.top = 0;
	WidgetRect.right = nWidth;
	WidgetRect.bottom = nHeight;

	LayoutSizeArray.push_back(WidgetRect);
}

CTwoWidgetSplitAlgorithm::CTwoWidgetSplitAlgorithm( void )
{	
}

CTwoWidgetSplitAlgorithm::~CTwoWidgetSplitAlgorithm( void )
{
}

void CTwoWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 2)
	{
		return;
	}

	CRect rect1;
	rect1.left = 0;
	rect1.top = 0;
	rect1.right = rect1.left + (nWidth - DEFAULT_GAP) / 2;
	rect1.bottom = rect1.top + nHeight;

	CWnd* pWidget = m_WidgetArray[0];

	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rect1,TRUE);
		pWidget->ShowWindow(SW_SHOW);
	}

	CRect rect2;
	rect2.left = rect1.right + DEFAULT_GAP;
	rect2.top = rect1.top;
	rect2.right = nWidth;
	rect2.bottom = rect1.bottom;

	pWidget = m_WidgetArray[1];

	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rect2,TRUE);
		pWidget->ShowWindow(SW_SHOW);
	}

	//���¼���ָ�����λ��
	CRect SplitRect;
	SplitRect.left = (nWidth - DEFAULT_GAP) / 2;
	SplitRect.top = 0;
	SplitRect.right = SplitRect.left + DEFAULT_GAP;
	SplitRect.bottom = SplitRect.top + nHeight;
	m_HorSplitBarRect[0] = SplitRect;
}

void CTwoWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	CRect WidgetRect1;

	//�����һ��widget�ĵĴ���λ��
	WidgetRect1.left = 0;
	WidgetRect1.top = m_HorSplitBarRect[0].top;
	WidgetRect1.right = m_HorSplitBarRect[0].left;
	WidgetRect1.bottom = m_HorSplitBarRect[0].bottom;

	LayoutSizeArray.push_back(WidgetRect1);


	CRect WidgetRect2;

	//����ڶ���widget�ĵĴ���λ��
	WidgetRect2.left = m_HorSplitBarRect[0].right;
	WidgetRect2.top = m_HorSplitBarRect[0].top;
	WidgetRect2.right = nWidth;
	WidgetRect2.bottom = m_HorSplitBarRect[0].bottom;

	LayoutSizeArray.push_back(WidgetRect2);
}

MOUSE_STATE CTwoWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{

	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}
	

	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

void CTwoWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

void CTwoWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	ClipCursor(NULL);
	//���طָ���
	ShowHorSplitBar(false);
	ShowVerSplitBar(false);
	//�����϶��ָ���֮��Ĳ���
	switch (m_SelectSplitBarInfo.Type)
	{
	case MOVE_HOR:
		{
			int nXMove = pt.x - m_StartPoint.x;
			CalculateCoordinateAfterHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_VER:
		{
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_ALL:
		{
			int nXMove = pt.x - m_StartPoint.x;
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	default:
		break;
	}

	//���¼��㽻���λ��
	ReCalculateIntersetRect();
	//�ı䲼��
	ChangeLayout(nWidth,nHeight);
}

void CTwoWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{

	//����ƶ�
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			int nXMove = pt.x - m_StartPoint.x;
			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		if (MOVE_VER == m_SelectSplitBarInfo.Type)
		{
			int nYMove = pt.y - m_StartPoint.y;
			HandleVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		if (MOVE_ALL == m_SelectSplitBarInfo.Type)
		{
			int nYMove = pt.y - m_StartPoint.y;
			int nXMove = pt.x - m_StartPoint.x;
			HandleHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}
	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

void CTwoWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	//�����Ϸ����ж�
	if (NULL == m_pLayout)
	{
		return;
	}
	//��ȡ�϶��ķָ����λ��
	CRect rc = m_HorSplitBarRect[nCurrentIndex];
	//����λ��X�᷽���ϵ�ƫ��
	rc.OffsetRect(nMoveDistance,0);

	RECT Rect;
	Rect.left = 0 + DEFAULT_GAP;
	Rect.top = 0;
	Rect.bottom = nHeight;
	Rect.right = nWidth - DEFAULT_GAP;

	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&Rect);
		ClipCursor(&Rect);
	}
	//��ȡˮƽ�ָ���
	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		//�ƶ��ָ�������Ӧλ��
		pLayout->MoveWindow(&rc,TRUE);
		//ˢ��
		pLayout->Invalidate(TRUE);
		//��ʾ�ָ���
		pLayout->ShowWindow(SW_SHOW);
	}		
}

void CTwoWidgetSplitAlgorithm::CalculateCoordinateAfterHorMove( int nXMove,int nCurrentIndex,int nWidth,int nHeight )
{
	//����ָ����ľ���
	m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
}

CThreeWidgetSplitAlgorithm::CThreeWidgetSplitAlgorithm( void )
{

}

CThreeWidgetSplitAlgorithm::~CThreeWidgetSplitAlgorithm( void )
{

}

void CThreeWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 3)
	{
		return;
	}

	if (m_bWndFront)
	{
		AdjustLayoutFront(nWidth,nHeight);
	}
	else
	{
		AdjustLayoutBack(nWidth,nHeight);
	}


}

void CThreeWidgetSplitAlgorithm::AdjustLayoutBack( int nWidth,int nHeight )
{
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP) / 2;
	rc1.bottom = rc1.top + (nHeight - DEFAULT_GAP) / 2;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}
	CRect rc2;
	rc2.left = rc1.left;
	rc2.top = rc1.bottom + DEFAULT_GAP;
	rc2.right = rc2.left + (nWidth - DEFAULT_GAP) / 2;
	rc2.bottom = nHeight;
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}
	pWidget = m_WidgetArray[2];
	CRect rc3;
	rc3.left = rc1.right + DEFAULT_GAP;
	rc3.top = rc1.top;
	rc3.right = nWidth;
	rc3.bottom = nHeight;

	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}


	//��һ���ָ���λ��
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = rc3.left;
	m_HorSplitBarRect[0].bottom = rc3.bottom;

	//�ڶ����ָ���λ��
	m_VerSplitBarRect[0].left = rc1.left;
	m_VerSplitBarRect[0].top = rc1.bottom;
	m_VerSplitBarRect[0].right = rc1.right;
	m_VerSplitBarRect[0].bottom = rc2.top;

}

void CThreeWidgetSplitAlgorithm::AdjustLayoutFront( int nWidth,int nHeight )
{
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP) / 2;
	rc1.bottom = nHeight;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}


	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = nWidth;
	rc2.bottom = rc2.top + (nHeight - DEFAULT_GAP) / 2;
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	pWidget = m_WidgetArray[2];
	CRect rc3;
	rc3.left = rc2.left;
	rc3.top = rc2.bottom + DEFAULT_GAP;
	rc3.right = rc2.right;
	rc3.bottom = nHeight;

	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	//��һ���ָ���λ��
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = m_HorSplitBarRect[0].left + DEFAULT_GAP;
	m_HorSplitBarRect[0].bottom = m_HorSplitBarRect[0].top + rc1.Width();

	//�ڶ����ָ���λ��
	m_VerSplitBarRect[0].left = rc2.left;
	m_VerSplitBarRect[0].top = rc2.bottom;
	m_VerSplitBarRect[0].right = nWidth;
	m_VerSplitBarRect[0].bottom = rc3.top;
}

void CThreeWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	if (m_bWndFront)
	{

		//��һ������λ��
		CRect WidgetRect1;
		WidgetRect1.left = 0;
		WidgetRect1.top = m_HorSplitBarRect[0].top;
		WidgetRect1.right = m_HorSplitBarRect[0].left;
		WidgetRect1.bottom = m_HorSplitBarRect[0].bottom;

		LayoutSizeArray.push_back(WidgetRect1);

		//�ڶ�������λ��
		CRect WidgetRect2;
		WidgetRect2.left = m_HorSplitBarRect[0].right;
		WidgetRect2.top = m_HorSplitBarRect[0].top;
		WidgetRect2.right = nWidth;
		WidgetRect2.bottom = m_VerSplitBarRect[0].top;

		LayoutSizeArray.push_back(WidgetRect2);

		//����������λ��
		CRect WidgetRect3;
		WidgetRect3.left = m_HorSplitBarRect[0].right;
		WidgetRect3.top = m_VerSplitBarRect[0].bottom;
		WidgetRect3.right = nWidth;
		WidgetRect3.bottom = nHeight;

		LayoutSizeArray.push_back(WidgetRect3);
	}
	else
	{
		//��һ������λ��
		CRect WidgetRect1;
		WidgetRect1.left = 0;
		WidgetRect1.top = 0;
		WidgetRect1.right = m_HorSplitBarRect[0].left;
		WidgetRect1.bottom = m_VerSplitBarRect[0].top;

		LayoutSizeArray.push_back(WidgetRect1);

		//�ڶ�������λ��
		CRect WidgetRect2;
		WidgetRect2.left = m_VerSplitBarRect[0].left;
		WidgetRect2.top = m_VerSplitBarRect[0].bottom;
		WidgetRect2.right = m_HorSplitBarRect[0].left;
		WidgetRect2.bottom = nHeight;

		LayoutSizeArray.push_back(WidgetRect2);

		//����������λ��
		CRect WidgetRect3;
		WidgetRect3.left = m_HorSplitBarRect[0].right;
		WidgetRect3.top = m_HorSplitBarRect[0].top;
		WidgetRect3.right = nWidth;
		WidgetRect3.bottom = nHeight;

		LayoutSizeArray.push_back(WidgetRect3);

	}
}

MOUSE_STATE CThreeWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{

	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}
	

	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		if (TRUE == m_VerSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}
	}
	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

void CThreeWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

void CThreeWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	ClipCursor(NULL);
	if (NULL == m_pLayout)
	{
		return;
	}
	//�����ˮƽ�ƶ�
	if (MOVE_HOR == m_SelectSplitBarInfo.Type)
	{
		//��ȡˮƽ�ƶ�������
		int nXMove = pt.x - m_StartPoint.x;
		//����ָ����ľ���
		m_HorSplitBarRect[m_SelectSplitBarInfo.nIndex].OffsetRect(nXMove,0);
		//��ȡˮƽ�ָ���
		CWnd* pHorSplit = m_pLayout->GetMoveHorSplitBar();
		if (NULL != pHorSplit && NULL != pHorSplit->GetSafeHwnd())
		{
			pHorSplit->ShowWindow(SW_HIDE);
		}

		//����󴰿���ǰ��
		if (m_bWndFront)
		{
			//�޸Ĺ����Ĵ�ֱ�ָ���
			m_VerSplitBarRect[0].left += (nXMove);
		}
		//����󴰿��ں���
		else
		{
			//�޸Ĺ����Ĵ�ֱ�ָ���
			m_VerSplitBarRect[0].right += (nXMove);
		}
	}
	//����Ǵ�ֱ�ƶ�
	if (MOVE_VER == m_SelectSplitBarInfo.Type)
	{
		//��ȡ��ֱ�ƶ�������
		int nYMove = pt.y - m_StartPoint.y;
		//����ָ����ľ���
		m_VerSplitBarRect[m_SelectSplitBarInfo.nIndex].OffsetRect(0,nYMove);
		CWnd* pVerSplit = m_pLayout->GetMoveVerSplitBar();
		if (NULL != pVerSplit && NULL != pVerSplit->GetSafeHwnd())
		{
			pVerSplit->ShowWindow(SW_HIDE);
		}	
	}
	//���¼��㲼��
	ChangeLayout(nWidth,nHeight);	
}

void CThreeWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//����ƶ�
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			int nXMove = pt.x - m_StartPoint.x;
			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		if (MOVE_VER == m_SelectSplitBarInfo.Type)
		{
			int nYMove = pt.y - m_StartPoint.y;
			HandleVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}

	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

void CThreeWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}

	CRect rc = m_HorSplitBarRect[nCurrentIndex];
	rc.OffsetRect(nMoveDistance,0);


	RECT Rect;
	Rect.left = 0 + DEFAULT_GAP;
	Rect.top = 0;
	Rect.bottom = nHeight;
	Rect.right = nWidth - DEFAULT_GAP;

	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&Rect);
		ClipCursor(&Rect);
	}

	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}		
}

void CThreeWidgetSplitAlgorithm::HandleVerMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}

	CRect rc = m_VerSplitBarRect[nCurrentIndex];
	rc.OffsetRect(0,nMoveDistance);

	RECT Rect;
	Rect.left = 0;
	Rect.top = DEFAULT_GAP;
	Rect.bottom = nHeight - DEFAULT_GAP;
	Rect.right = nWidth;

	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&Rect);
		ClipCursor(&Rect);
	}

	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	
}

CFourWidgetSplitAlgorithm::CFourWidgetSplitAlgorithm( void )
{
}

CFourWidgetSplitAlgorithm::~CFourWidgetSplitAlgorithm( void )
{
}

void CFourWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 4)
	{
		return;
	}

	if (m_bWndFront)
	{
		AdjustLayoutFront(nWidth,nHeight);
	}
	else
	{
		AdjustLayoutBack(nWidth,nHeight);
	}
}

void CFourWidgetSplitAlgorithm::AdjustLayoutFront( int nWidth,int nHeight )
{
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 3) / 4;
	rc1.bottom = nHeight;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + (nWidth - DEFAULT_GAP * 3) / 4;
	rc2.bottom = nHeight;
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	pWidget = m_WidgetArray[2];
	CRect rc3;
	rc3.left = rc2.right + DEFAULT_GAP;
	rc3.top = rc2.top;
	rc3.right = rc3.left + (nWidth - DEFAULT_GAP * 3) / 4 ;
	rc3.bottom = nHeight;

	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	pWidget = m_WidgetArray[3];
	CRect rc4;
	rc4.left = rc3.right + DEFAULT_GAP;
	rc4.top = rc3.top;
	rc4.right = nWidth;
	rc4.bottom = nHeight;

	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//����ָ���λ��
	//��һ���ָ���
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = rc2.left;
	m_HorSplitBarRect[0].bottom = rc1.bottom;

	//�ڶ����ָ���
	m_HorSplitBarRect[1].left = rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = rc3.left;
	m_HorSplitBarRect[1].bottom = rc2.bottom;

	//�������ָ���
	m_HorSplitBarRect[2].left = rc3.right;
	m_HorSplitBarRect[2].top = rc3.top;
	m_HorSplitBarRect[2].right = rc4.left;
	m_HorSplitBarRect[2].bottom = rc3.bottom;
}

void CFourWidgetSplitAlgorithm::AdjustLayoutBack( int nWidth,int nHeight )
{
	AdjustLayoutFront(nWidth,nHeight);
}

void CFourWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	//��һ��Widget����λ��
	CRect WidgetRect1;
	WidgetRect1.left = 0;
	WidgetRect1.top = m_HorSplitBarRect[0].top;
	WidgetRect1.right = m_HorSplitBarRect[0].left;
	WidgetRect1.bottom = nHeight;
	LayoutSizeArray.push_back(WidgetRect1);

	//�ڶ���Widget����λ��
	CRect WidgetRect2;
	WidgetRect2.left = m_HorSplitBarRect[0].right;
	WidgetRect2.top = m_HorSplitBarRect[0].top;
	WidgetRect2.right = m_HorSplitBarRect[1].left;
	WidgetRect2.bottom = nHeight;
	LayoutSizeArray.push_back(WidgetRect2);

	//������Widget����λ��
	CRect WidgetRect3;
	WidgetRect3.left = m_HorSplitBarRect[1].right;
	WidgetRect3.top = m_HorSplitBarRect[1].top;
	WidgetRect3.right = m_HorSplitBarRect[2].left;
	WidgetRect3.bottom = nHeight;
	LayoutSizeArray.push_back(WidgetRect3);

	//���ĸ�Widget����λ��
	CRect WidgetRect4;
	WidgetRect4.left = m_HorSplitBarRect[2].right;
	WidgetRect4.top = m_HorSplitBarRect[2].top;
	WidgetRect4.right = nWidth;
	WidgetRect4.bottom = nHeight;
	LayoutSizeArray.push_back(WidgetRect4);

}

MOUSE_STATE CFourWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{

	
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}

	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

void CFourWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

void CFourWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{

	if (MOVE_HOR == m_SelectSplitBarInfo.Type)
	{
		int nXMove = pt.x - m_StartPoint.x;

		m_HorSplitBarRect[m_SelectSplitBarInfo.nIndex].OffsetRect(nXMove,0);

		if (NULL != m_pLayout)
		{
			CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();

			if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
			{
				pLayout->ShowWindow(SW_HIDE);
			}
		}
		CalculateNextSplitBar(m_SelectSplitBarInfo.nIndex,nXMove,nWidth,nHeight);
	}

	ChangeLayout(nWidth,nHeight);

	ClipCursor(NULL);
}

void CFourWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//����ƶ�
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			int nXMove = pt.x - m_StartPoint.x;

			//����ˮƽ�ƶ�
			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}
	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

void CFourWidgetSplitAlgorithm::CalculateNextSplitBar( int nCurrentIndex,int nXMove,int nWidth,int nHeight )
{
	//�ж�����Ӱ��ķָ���
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		//��ѡ��ĺ���
		if (i > nCurrentIndex)
		{
			m_HorSplitBarRect[i].OffsetRect(nXMove,0);
		}
	}
}

void CFourWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}

	CRect rc = m_HorSplitBarRect[nCurrentIndex];

	rc.OffsetRect(nMoveDistance,0);

	//��ȡǰһ�����ڵ�����
	int nPrexIndex = GetPrexHorSplitBarIndex(m_SelectSplitBarInfo.nIndex);

	//������귶Χ
	RECT CursorRect;
	if (nPrexIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPrexIndex].right + DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	else
	{
		CursorRect.left = 0 + DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}

	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		ClipCursor(&CursorRect);
	}

	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();

	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	
}


CFiveWidgetSplitAlgorithm::CFiveWidgetSplitAlgorithm( void )
{

}

CFiveWidgetSplitAlgorithm::~CFiveWidgetSplitAlgorithm( void )
{

}

void CFiveWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 5)
	{
		return;
	}

	if (m_bWndFront)
	{
		AdjustLayoutFront(nWidth,nHeight);
	}
	else
	{
		AdjustLayoutBack(nWidth,nHeight);
	}
}

void CFiveWidgetSplitAlgorithm::AdjustLayoutFront( int nWidth,int nHeight )
{

	//�ƶ���һ������λ��
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 2) / 3;
	rc1.bottom = nHeight;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ��ڶ�������λ��
	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + (nWidth - DEFAULT_GAP * 2) / 3;
	rc2.bottom = rc2.top + (nHeight - DEFAULT_GAP) / 2;
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ�����������λ��
	CRect rc3;
	rc3.left = rc2.right + DEFAULT_GAP;
	rc3.top = rc2.top;
	rc3.right = rc3.left + (nWidth - DEFAULT_GAP * 2) / 3;
	rc3.bottom = rc3.top + (nHeight - DEFAULT_GAP) / 2;
	pWidget = m_WidgetArray[2];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ����ĸ�����λ��
	CRect rc4;
	rc4.left = rc2.left;
	rc4.top = rc2.bottom + DEFAULT_GAP;
	rc4.right = rc4.left + rc2.Width();
	rc4.bottom = rc4.top + rc2.Height();
	pWidget = m_WidgetArray[3];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���5������λ��
	CRect rc5;
	rc5.left = rc4.right + DEFAULT_GAP;
	rc5.top = rc4.top;
	rc5.right = rc5.left + rc4.Width();
	rc5.bottom = rc5.top + rc4.Height();
	pWidget = m_WidgetArray[4];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc5);
		pWidget->ShowWindow(SW_SHOW);
	}


	//����ָ�����λ��
	//��һ���ָ���
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = rc2.left;
	m_HorSplitBarRect[0].bottom = rc1.bottom;

	//�ڶ����ָ���
	m_HorSplitBarRect[1].left = rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = rc3.left;
	m_HorSplitBarRect[1].bottom = rc1.bottom;

	//�������ָ���
	m_VerSplitBarRect[0].left = rc2.left;
	m_VerSplitBarRect[0].top = rc2.bottom;
	m_VerSplitBarRect[0].right = nWidth;
	m_VerSplitBarRect[0].bottom = rc4.top;

	//���㽻��λ��
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[0].pRect1 = &m_HorSplitBarRect[1];
	m_IntersectRect[0].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[0].nHorIndex = 1;
	m_IntersectRect[0].nVerIndex = 0;

}

void CFiveWidgetSplitAlgorithm::AdjustLayoutBack( int nWidth,int nHeight )
{

	//�ƶ���һ������λ��
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 2) / 3;
	rc1.bottom = rc1.top + (nHeight - DEFAULT_GAP) / 2;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ��ڶ�������λ��
	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + rc1.Width();
	rc2.bottom = rc2.top + rc1.Height();
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ�����������λ��
	CRect rc3;
	rc3.left = rc1.left;
	rc3.top = rc1.bottom + DEFAULT_GAP;
	rc3.right = rc3.left + rc1.Width();
	rc3.bottom = rc3.top + rc1.Height();
	pWidget = m_WidgetArray[2];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ����ĸ�����λ��
	CRect rc4;
	rc4.left = rc3.right + DEFAULT_GAP;
	rc4.top = rc3.top;
	rc4.right = rc4.left + rc3.Width();
	rc4.bottom = rc4.top + rc3.Height();
	pWidget = m_WidgetArray[3];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���5������λ��
	CRect rc5;
	rc5.left = rc2.right + DEFAULT_GAP;
	rc5.top = rc2.top;
	rc5.right = nWidth;
	rc5.bottom = nHeight;
	pWidget = m_WidgetArray[4];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc5);
		pWidget->ShowWindow(SW_SHOW);
	}


	//����ָ�����λ��
	//��һ���ָ���
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = rc2.left;
	m_HorSplitBarRect[0].bottom = nHeight;

	//�ڶ����ָ���
	m_HorSplitBarRect[1].left = rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = rc5.left;
	m_HorSplitBarRect[1].bottom = nHeight;

	//�������ָ���
	m_VerSplitBarRect[0].left = rc1.left;
	m_VerSplitBarRect[0].top = rc1.bottom;
	m_VerSplitBarRect[0].right = rc4.right;
	m_VerSplitBarRect[0].bottom = rc4.top;


	//���㽻��λ��
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[0].pRect1 = &m_HorSplitBarRect[0];
	m_IntersectRect[0].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[0].nHorIndex = 0;
	m_IntersectRect[0].nVerIndex = 0;
}

void CFiveWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{

	LayoutSizeArray.clear();

	//�󴰿���ǰ��
	if (m_bWndFront)
	{
		CRect Widget1;
		Widget1.left = 0;
		Widget1.top = m_HorSplitBarRect[0].top;
		Widget1.right = m_HorSplitBarRect[0].left;
		Widget1.bottom = m_HorSplitBarRect[0].bottom;
		LayoutSizeArray.push_back(Widget1);

		CRect Widget2;
		Widget2.left = m_HorSplitBarRect[0].right;
		Widget2.top = m_HorSplitBarRect[0].top;
		Widget2.right = m_HorSplitBarRect[1].left;
		Widget2.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget2);

		CRect Widget3;
		Widget3.left = m_HorSplitBarRect[1].right;
		Widget3.top = m_HorSplitBarRect[1].top;
		Widget3.right = nWidth;
		Widget3.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget3);

		CRect Widget4;
		Widget4.left = m_HorSplitBarRect[0].right;
		Widget4.top = m_VerSplitBarRect[0].bottom;
		Widget4.right = m_HorSplitBarRect[1].left;
		Widget4.bottom = nHeight;
		LayoutSizeArray.push_back(Widget4);

		CRect Widget5;
		Widget5.left = m_HorSplitBarRect[1].right;
		Widget5.top = m_VerSplitBarRect[0].bottom;
		Widget5.right = nWidth;
		Widget5.bottom = nHeight;
		LayoutSizeArray.push_back(Widget5);

	}
	else
	{
		CRect Widget1;
		Widget1.left = 0;
		Widget1.top = m_HorSplitBarRect[0].top;
		Widget1.right = m_HorSplitBarRect[0].left;
		Widget1.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget1);

		CRect Widget2;
		Widget2.left = m_HorSplitBarRect[0].right;
		Widget2.top = m_HorSplitBarRect[0].top;
		Widget2.right = m_HorSplitBarRect[1].left;
		Widget2.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget2);

		CRect Widget3;
		Widget3.left = m_VerSplitBarRect[0].left;
		Widget3.top = m_VerSplitBarRect[0].bottom;
		Widget3.right = m_HorSplitBarRect[0].left;
		Widget3.bottom = nHeight;
		LayoutSizeArray.push_back(Widget3);

		CRect Widget4;
		Widget4.left = m_HorSplitBarRect[0].right;
		Widget4.top = m_VerSplitBarRect[0].bottom;
		Widget4.right = m_HorSplitBarRect[1].left;
		Widget4.bottom = nHeight;
		LayoutSizeArray.push_back(Widget4);

		CRect Widget5;
		Widget5.left = m_HorSplitBarRect[1].right;
		Widget5.top = m_HorSplitBarRect[1].top;
		Widget5.right = nWidth;
		Widget5.bottom = nHeight;
		LayoutSizeArray.push_back(Widget5);

	}
}

MOUSE_STATE CFiveWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{
	
#if 0
	//������ǰ
	if (m_bWndFront)
	{
		for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
		{
			if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_ALL;
				return MOVE_ALL;
			}
		}

		//�ж��Ƿ���������ƶ�
		for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
		{
			if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_HOR;
				return MOVE_HOR;
			}
		}

		for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
		{
			if (TRUE == m_VerSplitBarRect[i].PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_VER;
				return MOVE_VER;
			}
		}
	}
	//�����ں�
	else
	{
		for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
		{
			if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_ALL;
				return MOVE_ALL;
			}
		}

		//�ж��Ƿ���������ƶ�
		nNum = sizeof(m_SplitBarRect) / sizeof(m_SplitBarRect[0]);

		for (int i = 0; i < nNum - 1; ++i)
		{
			if (TRUE == m_SplitBarRect[i].PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_HOR;
				return MOVE_HOR;
			}
		}


		if (TRUE == m_SplitBarRect[nNum - 1].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = nNum - 1;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}
	}
#endif

	for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
	{
		if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_ALL;
			return MOVE_ALL;
		}
	}

	//�ж��Ƿ���������ƶ�
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}

	//�Ƿ���������ƶ�
	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		if (TRUE == m_VerSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}
	}
	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

void CFiveWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

void CFiveWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//ȡ���������
	ClipCursor(NULL);
	//�����Ϸ����ж�
	if (NULL == m_pLayout)
	{
		return;
	}
	//����ˮƽ�ָ���
	ShowHorSplitBar(false);
	//���ش�ֱ�ָ���
	ShowVerSplitBar(false);
	//��������ת��
	switch (m_SelectSplitBarInfo.Type)
	{
	case MOVE_HOR:
		{
			int nXMove = pt.x - m_StartPoint.x;
			CalculateCoordinateAfterHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_VER:
		{
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_ALL:
		{
			int nXMove = pt.x - m_StartPoint.x;
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	default:
		break;
	}

	//���¼��㽻���λ��
	ReCalculateIntersetRect();
	//���¸ı䲼��
	ChangeLayout(nWidth,nHeight);
}

void CFiveWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{

	//����ƶ�
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			int nXMove = pt.x - m_StartPoint.x;

			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		if (MOVE_VER == m_SelectSplitBarInfo.Type)
		{
			int nYMove = pt.y - m_StartPoint.y;

			HandleVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		if (MOVE_ALL == m_SelectSplitBarInfo.Type)
		{
			int nYMove = pt.y - m_StartPoint.y;
			int nXMove = pt.x - m_StartPoint.x;
			HandleHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}

	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

void CFiveWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{

	if (NULL == m_pLayout)
	{
		return;
	}
	CRect rc = m_HorSplitBarRect[nCurrentIndex];
	rc.OffsetRect(nMoveDistance,0);


	//��ȡǰһ�����ڵ�����
	int nPrexIndex = GetPrexHorSplitBarIndex(nCurrentIndex);

	//������귶Χ
	RECT CursorRect;
	//�����ǰһ���ָ���
	if (nPrexIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPrexIndex].right + DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//���û�зָ���
	else
	{
		CursorRect.left = DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}

	//�ͻ������ת��Ϊ��������
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//���ƹ��
		ClipCursor(&CursorRect);
	}
	//��ȡˮƽ�ָ���
	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();

	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	
}

void CFiveWidgetSplitAlgorithm::HandleVerMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}

	CRect rc = m_VerSplitBarRect[nCurrentIndex];

	rc.OffsetRect(0,nMoveDistance);

	//������귶Χ
	RECT CursorRect;
	CursorRect.left = 0;
	CursorRect.top = DEFAULT_GAP;
	CursorRect.bottom = nHeight - DEFAULT_GAP;
	CursorRect.right = nWidth;
	//�ͻ������ת��Ϊ��������
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//���ƹ��
		ClipCursor(&CursorRect);
	}
	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

void CFiveWidgetSplitAlgorithm::HandleHorAndVerMove( int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}
	CRect HorRect = *(m_IntersectRect[nCurrentIndex].pRect1);
	CRect VerRect = *(m_IntersectRect[nCurrentIndex].pRect2);
	HorRect.OffsetRect(nMoveX,0);
	VerRect.OffsetRect(0,nMoveY);

	//�󴰿���ǰ��
	if (m_bWndFront)
	{
		//������귶Χ
		RECT CursorRect;
		CursorRect.left = m_HorSplitBarRect[0].right + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
		//�ͻ������ת��Ϊ��������
		if (NULL != m_pHostWnd)
		{
			m_pHostWnd->ClientToScreen(&CursorRect);
			//���ƹ��
			ClipCursor(&CursorRect);
		}
	}
	else
	{
		//������귶Χ
		RECT CursorRect;
		CursorRect.left = 0 + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = m_HorSplitBarRect[1].left - DEFAULT_GAP;
		//�ͻ������ת��Ϊ��������
		if (NULL != m_pHostWnd)
		{
			m_pHostWnd->ClientToScreen(&CursorRect);
			//���ƹ��
			ClipCursor(&CursorRect);
		}
	}


	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&VerRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}

	pLayout = m_pLayout->GetMoveHorSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&HorRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	
}

void CFiveWidgetSplitAlgorithm::CalculateCoordinateAfterHorMove( int nXMove,int nCurrentIndex,int nWidth,int nHeight )
{

	//�󴰿��ں���
	if (m_bWndFront)
	{
		//����ǵ�һ��λ��
		if (0 == nCurrentIndex)
		{
			m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
			//��ֱ�ָ���λ��Ҫ�ı�
			m_VerSplitBarRect[0].left += nXMove;
			//��������λ��
			m_HorSplitBarRect[1].OffsetRect(nXMove,0);
		}
		else
		{
			m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
		}
	}
	else
	{
		//�ж��Ƿ������һ��λ��
		if (nCurrentIndex == 1)
		{
			m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
			m_VerSplitBarRect[0].right += nXMove;
		}
		//����������һ��
		else
		{
			m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
			//��������λ��
			m_HorSplitBarRect[1].OffsetRect(nXMove,0);
			m_VerSplitBarRect[0].right += nXMove;
		}
	}
}

void CFiveWidgetSplitAlgorithm::CalculateCoordinateAfterVerMove( int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_VerSplitBarRect[nCurrentIndex].OffsetRect(0,nYMove);
}

void CFiveWidgetSplitAlgorithm::CalculateCoordinateAfterHorAndVerMove( int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_IntersectRect[nCurrentIndex].pRect1->OffsetRect(nXMove,0);
	m_IntersectRect[nCurrentIndex].pRect2->OffsetRect(0,nYMove);
}

void CFiveWidgetSplitAlgorithm::ReCalculateIntersetRect( void )
{
	//���¼��㽻���
	if (m_bWndFront)
	{
		m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	}
	else
	{
		m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_HorSplitBarRect[0]);
	}
}

CSixWidgetSplitAlgorithm::CSixWidgetSplitAlgorithm( void )
{

}

CSixWidgetSplitAlgorithm::~CSixWidgetSplitAlgorithm( void )
{

}

void CSixWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 6)
	{
		return;
	}

	if (m_bWndFront)
	{
		AdjustLayoutFront(nWidth,nHeight);
	}
	else
	{
		AdjustLayoutBack(nWidth,nHeight);
	}
}

void CSixWidgetSplitAlgorithm::AdjustLayoutFront( int nWidth,int nHeight )
{

	//�ƶ���һ������λ��
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 2) / 3;
	rc1.bottom = rc1.top + (nHeight - DEFAULT_GAP) / 2;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ��ڶ�������λ��
	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + rc1.Width();
	rc2.bottom = rc2.top + rc1.Height();
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ�����������λ��
	CRect rc3;
	rc3.left = rc2.right + DEFAULT_GAP;
	rc3.top = rc1.top;
	rc3.right = rc3.left + rc1.Width();
	rc3.bottom = rc3.top + rc1.Height();
	pWidget = m_WidgetArray[2];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ����ĸ�����λ��
	CRect rc4;
	rc4.left = rc1.left;
	rc4.top = rc1.bottom + DEFAULT_GAP;
	rc4.right = rc4.left + rc3.Width();
	rc4.bottom = rc4.top + rc3.Height();
	pWidget = m_WidgetArray[3];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���5������λ��
	CRect rc5;
	rc5.left = rc4.right + DEFAULT_GAP;
	rc5.top = rc4.top;
	rc5.right = rc5.left + rc4.Width();
	rc5.bottom = rc5.top + rc4.Height();
	pWidget = m_WidgetArray[4];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc5);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���6������λ��
	CRect rc6;
	rc6.left = rc5.right + DEFAULT_GAP;
	rc6.top = rc5.top;
	rc6.right = rc6.left + rc5.Width();
	rc6.bottom = rc6.top + rc5.Height();
	pWidget = m_WidgetArray[5];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc6);
		pWidget->ShowWindow(SW_SHOW);
	}


	//����ָ���λ��
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = rc2.left;
	m_HorSplitBarRect[0].bottom = rc4.bottom;

	m_HorSplitBarRect[1].left = rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = rc3.left;
	m_HorSplitBarRect[1].bottom = rc4.bottom;

	m_VerSplitBarRect[0].left = rc1.left;
	m_VerSplitBarRect[0].top = rc1.bottom;
	m_VerSplitBarRect[0].right = rc3.right;
	m_VerSplitBarRect[0].bottom = rc4.top;

	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[0].pRect1 = &m_HorSplitBarRect[0];
	m_IntersectRect[0].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[0].nHorIndex = 0;
	m_IntersectRect[0].nVerIndex = 0;

	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].pRect1 = &m_HorSplitBarRect[1];
	m_IntersectRect[1].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[1].nHorIndex = 1;
	m_IntersectRect[1].nVerIndex = 0;

}

void CSixWidgetSplitAlgorithm::AdjustLayoutBack( int nWidth,int nHeight )
{
	AdjustLayoutFront(nWidth,nHeight);
}

void CSixWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	CRect Widget1;
	Widget1.left = 0;
	Widget1.top = m_HorSplitBarRect[0].top;
	Widget1.right = m_HorSplitBarRect[0].left;
	Widget1.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget1);

	CRect Widget2;
	Widget2.left = m_HorSplitBarRect[0].right;
	Widget2.top = m_HorSplitBarRect[0].top;
	Widget2.right = m_HorSplitBarRect[1].left;
	Widget2.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget2);

	CRect Widget3;
	Widget3.left = m_HorSplitBarRect[1].right;
	Widget3.top = m_HorSplitBarRect[1].top;
	Widget3.right = nWidth;
	Widget3.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget3);

	CRect Widget4;
	Widget4.left = 0;
	Widget4.top = m_VerSplitBarRect[0].bottom;
	Widget4.right = m_HorSplitBarRect[0].left;
	Widget4.bottom = nHeight;
	LayoutSizeArray.push_back(Widget4);

	CRect Widget5;
	Widget5.left = m_HorSplitBarRect[0].right;
	Widget5.top = m_VerSplitBarRect[0].bottom;
	Widget5.right = m_HorSplitBarRect[1].left;
	Widget5.bottom = nHeight;
	LayoutSizeArray.push_back(Widget5);

	CRect Widget6;
	Widget6.left = m_HorSplitBarRect[1].right;
	Widget6.top = m_VerSplitBarRect[0].bottom;
	Widget6.right = nWidth;
	Widget6.bottom = nHeight;
	LayoutSizeArray.push_back(Widget6);
}

MOUSE_STATE CSixWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{
	
	//�ж��Ƿ����λ���ڽ����
	for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
	{
		if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_ALL;
			return MOVE_ALL;
		}
	}

	//�ж��Ƿ���������ƶ�
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}

	//�ж��Ƿ���Դ�ֱ�ƶ�
	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		if (TRUE == m_VerSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}
	}
	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

/*****************************************************************************
@FunctionName : ����������������Ϣ
@FunctionDescription : ����������������Ϣ
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CSixWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

/*****************************************************************************
@FunctionName : ����������������Ϣ
@FunctionDescription : ����������������Ϣ
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CSixWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//ȡ������ƶ���Χ
	ClipCursor(NULL);
	//�����϶���
	ShowHorSplitBar(false);
	ShowVerSplitBar(false);
	//�����϶��ָ���֮��Ĳ���
	switch (m_SelectSplitBarInfo.Type)
	{
	case MOVE_HOR:
		{
			int nXMove = pt.x - m_StartPoint.x;
			CalculateCoordinateAfterHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_VER:
		{
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_ALL:
		{
			int nXMove = pt.x - m_StartPoint.x;
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	default:
		break;
	}

	//���¼��㽻���λ��
	ReCalculateIntersetRect();
	//�ı䲼��
	ChangeLayout(nWidth,nHeight);
}

/*****************************************************************************
@FunctionName : ��������ƶ���Ϣ
@FunctionDescription : ��������ƶ���Ϣ
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CSixWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{

	//��������ƶ����
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		//�����ˮƽ�ƶ�
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			//����ˮƽ�����ƶ���������
			int nXMove = pt.x - m_StartPoint.x;

			//����ˮƽ�ƶ�
			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//����Ǵ�ֱ�����ƶ�
		if (MOVE_VER == m_SelectSplitBarInfo.Type)
		{
			//���㴹ֱ�����ƶ���������
			int nYMove = pt.y - m_StartPoint.y;

			//����ֱ�ƶ�
			HandleVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//����Ǵ�ֱ��ˮƽ�ƶ�
		if (MOVE_ALL == m_SelectSplitBarInfo.Type)
		{
			//���㴹ֱ�ƶ���������
			int nYMove = pt.y - m_StartPoint.y;
			//����ˮƽ�ƶ���������
			int nXMove = pt.x - m_StartPoint.x;
			//���㴹ֱ��ˮƽ�ƶ���������
			HandleHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}

	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

/*****************************************************************************
@FunctionName : ����ˮƽ�϶�
@FunctionDescription : ����ˮƽ�϶�
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CSixWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{

	if (NULL == m_pLayout)
	{
		return;
	}
	CRect rc = m_HorSplitBarRect[nCurrentIndex];
	rc.OffsetRect(nMoveDistance,0);


	//��ȡǰһ�����ڵ�����
	int nPrexIndex = GetPrexHorSplitBarIndex(nCurrentIndex);

	//������귶Χ
	RECT CursorRect;
	//�����ǰһ���ָ���
	if (nPrexIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPrexIndex].right + DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//���û�зָ���
	else
	{
		CursorRect.left = DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}

	//�ͻ������ת��Ϊ��������
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//���ƹ��
		ClipCursor(&CursorRect);
	}
	//��ȡˮƽ�ָ���
	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();

	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

/*****************************************************************************
@FunctionName : ����ֱ�ƶ�
@FunctionDescription : ����ֱ�ƶ�
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CSixWidgetSplitAlgorithm::HandleVerMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{

	if (NULL == m_pLayout)
	{
		return;
	}
	CRect rc = m_VerSplitBarRect[nCurrentIndex];
	rc.OffsetRect(0,nMoveDistance);
	//������귶Χ
	RECT CursorRect;
	CursorRect.left = 0;
	CursorRect.top = DEFAULT_GAP;
	CursorRect.bottom = nHeight - DEFAULT_GAP;
	CursorRect.right = nWidth;
	//�ͻ������ת��Ϊ��������
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//���ƹ��
		ClipCursor(&CursorRect);
	}
	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}	
}

/*****************************************************************************
@FunctionName : ����ֱ��ˮƽ�ƶ�
@FunctionDescription : ����ֱ��ˮƽ�ƶ�
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CSixWidgetSplitAlgorithm::HandleHorAndVerMove( int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}
	CRect HorRect = *(m_IntersectRect[nCurrentIndex].pRect1);
	CRect VerRect = *(m_IntersectRect[nCurrentIndex].pRect2);
	//�ж��Ƿ������һ����
	if (ARRAY_SIZE(m_IntersectRect) - 1 == nCurrentIndex)
	{
		//������귶Χ
		RECT CursorRect;
		CursorRect.left = m_HorSplitBarRect[0].right + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth;
		//�ͻ������ת��Ϊ��������
		if (NULL != m_pHostWnd)
		{
			m_pHostWnd->ClientToScreen(&CursorRect);
			//���ƹ��
			ClipCursor(&CursorRect);
		}
	}
	else
	{
		//������귶Χ
		RECT CursorRect;
		CursorRect.left = 0 + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
		//�ͻ������ת��Ϊ��������
		if (NULL != m_pHostWnd)
		{
			m_pHostWnd->ClientToScreen(&CursorRect);
			//���ƹ��
			ClipCursor(&CursorRect);
		}
	}

	HorRect.OffsetRect(nMoveX,0);
	VerRect.OffsetRect(0,nMoveY);

	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&VerRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	pLayout = m_pLayout->GetMoveHorSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&HorRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

void CSixWidgetSplitAlgorithm::CalculateCoordinateAfterHorMove( int nXMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
}

void CSixWidgetSplitAlgorithm::CalculateCoordinateAfterVerMove( int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_VerSplitBarRect[nCurrentIndex].OffsetRect(0,nYMove);
}

void CSixWidgetSplitAlgorithm::CalculateCoordinateAfterHorAndVerMove( int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{

	CRect* pRect = m_IntersectRect[nCurrentIndex].pRect1;

	if (NULL != pRect)
	{
		pRect->OffsetRect(nXMove,0);
	}

	//���ķָ���Ҳ�ƶ�
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (i > m_IntersectRect[nCurrentIndex].nHorIndex)
		{
			m_HorSplitBarRect[i].OffsetRect(nXMove,0);
		}
	}
	
	pRect = m_IntersectRect[nCurrentIndex].pRect2;

	if (NULL != pRect)
	{
		pRect->OffsetRect(0,nYMove);
	}
	
}

void CSixWidgetSplitAlgorithm::ReCalculateIntersetRect( void )
{
	//���¼��㽻���
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
}

CSevenWidgetSplitAlgorithm::CSevenWidgetSplitAlgorithm( void )
{

}

CSevenWidgetSplitAlgorithm::~CSevenWidgetSplitAlgorithm( void )
{

}

void CSevenWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 7)
	{
		return;
	}

	if (m_bWndFront)
	{
		AdjustLayoutFront(nWidth,nHeight);
	}
	else
	{
		AdjustLayoutBack(nWidth,nHeight);
	}
}

void CSevenWidgetSplitAlgorithm::AdjustLayoutFront( int nWidth,int nHeight )
{
	//�ƶ���һ������λ��
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 3) / 4;
	rc1.bottom = rc1.top + nHeight;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ��ڶ�������λ��
	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + rc1.Width();
	rc2.bottom = rc2.top + (nHeight - DEFAULT_GAP) / 2;
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ�����������λ��
	CRect rc3;
	rc3.left = rc2.right + DEFAULT_GAP;
	rc3.top = rc1.top;
	rc3.right = rc3.left + rc2.Width();
	rc3.bottom = rc3.top + rc2.Height();
	pWidget = m_WidgetArray[2];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ����ĸ�����λ��
	CRect rc4;
	rc4.left = rc3.right + DEFAULT_GAP;
	rc4.top = rc3.top;
	rc4.right = rc4.left + rc3.Width();
	rc4.bottom = rc4.top + rc3.Height();
	pWidget = m_WidgetArray[3];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���5������λ��
	CRect rc5;
	rc5.left = rc2.left;
	rc5.top = rc2.bottom + DEFAULT_GAP;
	rc5.right = rc5.left + rc2.Width();
	rc5.bottom = rc5.top + rc2.Height();
	pWidget = m_WidgetArray[4];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc5);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���6������λ��
	CRect rc6;
	rc6.left = rc5.right + DEFAULT_GAP;
	rc6.top = rc5.top;
	rc6.right = rc6.left + rc5.Width();
	rc6.bottom = rc6.top + rc5.Height();
	pWidget = m_WidgetArray[5];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc6);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���7������λ��
	CRect rc7;
	rc7.left = rc6.right + DEFAULT_GAP;
	rc7.top = rc6.top;
	rc7.right = rc7.left + rc6.Width();
	rc7.bottom = rc7.top + rc6.Height();
	pWidget = m_WidgetArray[6];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc7);
		pWidget->ShowWindow(SW_SHOW);
	}

	//����ָ�����λ��
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = rc2.left;
	m_HorSplitBarRect[0].bottom = rc1.bottom;

	m_HorSplitBarRect[1].left = rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = rc3.left;
	m_HorSplitBarRect[1].bottom = rc1.bottom;

	m_HorSplitBarRect[2].left = rc3.right;
	m_HorSplitBarRect[2].top = rc3.top;
	m_HorSplitBarRect[2].right = rc4.left;
	m_HorSplitBarRect[2].bottom = rc1.bottom;

	m_VerSplitBarRect[0].left = rc2.left;
	m_VerSplitBarRect[0].top = rc2.bottom;
	m_VerSplitBarRect[0].right = rc4.right;
	m_VerSplitBarRect[0].bottom = rc5.top;

	//�����ཻ��
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[0].pRect1 = &m_HorSplitBarRect[1];
	m_IntersectRect[0].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[0].nHorIndex = 1;
	m_IntersectRect[0].nVerIndex = 0;

	//�����ཻ��
	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[2],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].pRect1 = &m_HorSplitBarRect[2];
	m_IntersectRect[1].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[1].nHorIndex = 2;
	m_IntersectRect[1].nVerIndex = 0;

}

void CSevenWidgetSplitAlgorithm::AdjustLayoutBack( int nWidth,int nHeight )
{

	//�ƶ���һ������λ��
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 3) / 4;
	rc1.bottom = rc1.top + (nHeight - DEFAULT_GAP) / 2;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ��ڶ�������λ��
	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + rc1.Width();
	rc2.bottom = rc2.top + rc1.Height();
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ�����������λ��
	CRect rc3;
	rc3.left = rc2.right + DEFAULT_GAP;
	rc3.top = rc2.top;
	rc3.right = rc3.left + rc2.Width();
	rc3.bottom = rc3.top + rc2.Height();
	pWidget = m_WidgetArray[2];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ����ĸ�����λ��
	CRect rc4;
	rc4.left = rc1.left;
	rc4.top = rc1.bottom + DEFAULT_GAP;
	rc4.right = rc4.left + rc3.Width();
	rc4.bottom = rc4.top + rc3.Height();
	pWidget = m_WidgetArray[3];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���5������λ��
	CRect rc5;
	rc5.left = rc4.right + DEFAULT_GAP;
	rc5.top = rc4.top;
	rc5.right = rc5.left + rc4.Width();
	rc5.bottom = rc5.top + rc4.Height();
	pWidget = m_WidgetArray[4];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc5);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���6������λ��
	CRect rc6;
	rc6.left = rc5.right + DEFAULT_GAP;
	rc6.top = rc5.top;
	rc6.right = rc6.left + rc5.Width();
	rc6.bottom = rc6.top + rc5.Height();
	pWidget = m_WidgetArray[5];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc6);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���7������λ��
	CRect rc7;
	rc7.left = rc6.right + DEFAULT_GAP;
	rc7.top = rc1.top;
	rc7.right = rc7.left + rc6.Width();
	rc7.bottom = rc7.top + nHeight;
	pWidget = m_WidgetArray[6];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc7);
		pWidget->ShowWindow(SW_SHOW);
	}



	//����ָ�����λ��
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = m_HorSplitBarRect[0].left + DEFAULT_GAP;
	m_HorSplitBarRect[0].bottom = rc4.bottom;

	m_HorSplitBarRect[1].left = rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = m_HorSplitBarRect[1].left + DEFAULT_GAP;
	m_HorSplitBarRect[1].bottom = rc5.bottom;

	m_HorSplitBarRect[2].left = rc3.right;
	m_HorSplitBarRect[2].top = rc3.top;
	m_HorSplitBarRect[2].right = m_HorSplitBarRect[2].left + DEFAULT_GAP;
	m_HorSplitBarRect[2].bottom = rc6.bottom;

	m_VerSplitBarRect[0].left = rc1.left;
	m_VerSplitBarRect[0].top = rc1.bottom;
	m_VerSplitBarRect[0].right = rc3.right;
	m_VerSplitBarRect[0].bottom = rc4.top;

	//�����ཻ��
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[0].pRect1 = &m_HorSplitBarRect[0];
	m_IntersectRect[0].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[0].nHorIndex = 0;
	m_IntersectRect[0].nVerIndex = 0;

	//�����ཻ��
	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].pRect1 = &m_HorSplitBarRect[1];
	m_IntersectRect[1].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[1].nHorIndex = 1;
	m_IntersectRect[1].nVerIndex = 0;
}

void CSevenWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	if (m_bWndFront)
	{
		//��һ�����ڵ�λ�ö�λ
		CRect Widget1;
		Widget1.left = 0;
		Widget1.top = m_HorSplitBarRect[0].top;
		Widget1.right = m_HorSplitBarRect[0].left;
		Widget1.bottom = m_HorSplitBarRect[0].bottom;
		LayoutSizeArray.push_back(Widget1);

		//�ڶ������ڵ�λ�ö�λ
		CRect Widget2;
		Widget2.left = m_HorSplitBarRect[0].right;
		Widget2.top = m_HorSplitBarRect[0].top;
		Widget2.right = m_HorSplitBarRect[1].left;
		Widget2.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget2);

		//���������ڵ�λ�ö�λ
		CRect Widget3;
		Widget3.left = m_HorSplitBarRect[1].right;
		Widget3.top = m_HorSplitBarRect[1].top;
		Widget3.right = m_HorSplitBarRect[2].left;
		Widget3.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget3);

		//���ĸ����ڵ�λ�ö�λ
		CRect Widget4;
		Widget4.left = m_HorSplitBarRect[2].right;
		Widget4.top = m_HorSplitBarRect[2].top;
		Widget4.right = nWidth;
		Widget4.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget4);

		//��������ڵ�λ�ö�λ
		CRect Widget5;
		Widget5.left = m_HorSplitBarRect[0].right;
		Widget5.top = m_VerSplitBarRect[0].bottom;
		Widget5.right = m_HorSplitBarRect[1].left;
		Widget5.bottom = nHeight;
		LayoutSizeArray.push_back(Widget5);

		//���������ڵ�λ�ö�λ
		CRect Widget6;
		Widget6.left = m_HorSplitBarRect[1].right;
		Widget6.top = m_VerSplitBarRect[0].bottom;
		Widget6.right = m_HorSplitBarRect[2].left;
		Widget6.bottom = nHeight;
		LayoutSizeArray.push_back(Widget6);

		//���߸����ڵ�λ�ö�λ
		CRect Widget7;
		Widget7.left = m_HorSplitBarRect[2].right;
		Widget7.top = m_VerSplitBarRect[0].bottom;
		Widget7.right = nWidth;
		Widget7.bottom = nHeight;
		LayoutSizeArray.push_back(Widget7);
	}
	else
	{
		//��һ�����ڵ�λ�ö�λ
		CRect Widget1;
		Widget1.left = 0;
		Widget1.top = m_HorSplitBarRect[0].top;
		Widget1.right = m_HorSplitBarRect[0].left;
		Widget1.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget1);

		//�ڶ������ڵ�λ�ö�λ
		CRect Widget2;
		Widget2.left = m_HorSplitBarRect[0].right;
		Widget2.top = m_HorSplitBarRect[0].top;
		Widget2.right = m_HorSplitBarRect[1].left;
		Widget2.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget2);

		//���������ڵ�λ�ö�λ
		CRect Widget3;
		Widget3.left = m_HorSplitBarRect[1].right;
		Widget3.top = m_HorSplitBarRect[1].top;
		Widget3.right = m_HorSplitBarRect[2].left;
		Widget3.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget3);

		//���ĸ����ڵ�λ�ö�λ
		CRect Widget4;
		Widget4.left = 0;
		Widget4.top = m_VerSplitBarRect[0].bottom;
		Widget4.right = m_HorSplitBarRect[0].left;
		Widget4.bottom = nHeight;
		LayoutSizeArray.push_back(Widget4);

		//��������ڵ�λ�ö�λ
		CRect Widget5;
		Widget5.left = m_HorSplitBarRect[0].right;
		Widget5.top = m_VerSplitBarRect[0].bottom;
		Widget5.right = m_HorSplitBarRect[1].left;
		Widget5.bottom = nHeight;
		LayoutSizeArray.push_back(Widget5);

		//���������ڵ�λ�ö�λ
		CRect Widget6;
		Widget6.left = m_HorSplitBarRect[1].right;
		Widget6.top = m_VerSplitBarRect[0].bottom;
		Widget6.right = m_HorSplitBarRect[2].left;
		Widget6.bottom = nHeight;
		LayoutSizeArray.push_back(Widget6);

		//���߸����ڵ�λ�ö�λ
		CRect Widget7;
		Widget7.left = m_HorSplitBarRect[2].right;
		Widget7.top = m_HorSplitBarRect[2].top;
		Widget7.right = nWidth;
		Widget7.bottom = nHeight;
		LayoutSizeArray.push_back(Widget7);

	}
}

MOUSE_STATE CSevenWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{

#if 0
	if (m_bWndFront)
	{
		//�ж��Ƿ��ǽ����
		for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
		{
			if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_ALL;
				return MOVE_ALL;
			}

		}

		//�ж��Ƿ�ˮƽ�ƶ�
		for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
		{
			if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_HOR;
				return MOVE_HOR;
			}
		}

		for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
		{
			//�ж��Ƿ�ֱ�ƶ�
			if (TRUE == m_VerSplitBarRect[i].PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_VER;
				return MOVE_VER;
			}
		}
	}
	else
	{
		//�ж��Ƿ��ǽ����
		int nNum = sizeof(m_IntersectRect) / sizeof(m_IntersectRect[0]);

		for (int i = 0; i < nNum; ++i)
		{
			if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_ALL;
				return MOVE_ALL;
			}

		}

		//�ж��Ƿ�ˮƽ�ƶ�
		nNum = sizeof(m_SplitBarRect) / sizeof(m_SplitBarRect[0]);

		for (int i = 0; i < nNum -1; ++i)
		{
			if (TRUE == m_SplitBarRect[i].PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_HOR;
				return MOVE_HOR;
			}
		}

		//�ж��Ƿ�ֱ�ƶ�
		if (TRUE == m_SplitBarRect[nNum - 1].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = nNum - 1;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}

	}


#endif

	//�ж��Ƿ��ǽ����
	for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
	{
		if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_ALL;
			return MOVE_ALL;
		}

	}

	//�ж��Ƿ�ˮƽ�ƶ�
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}

	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		//�ж��Ƿ�ֱ�ƶ�
		if (TRUE == m_VerSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}
	}

	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

/*****************************************************************************
@FunctionName : �����������
@FunctionDescription : �����������
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CSevenWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

void CSevenWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	ClipCursor(NULL);

	//���طָ���
	ShowHorSplitBar(false);
	ShowVerSplitBar(false);

	//�����϶��ָ���֮��Ĳ���
	switch (m_SelectSplitBarInfo.Type)
	{
	case MOVE_HOR:
		{
			int nXMove = pt.x - m_StartPoint.x;
			CalculateCoordinateAfterHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_VER:
		{
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_ALL:
		{
			int nXMove = pt.x - m_StartPoint.x;
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	default:
		break;
	}

	//���¼��㽻���λ��
	ReCalculateIntersetRect();
	//�ı䲼��
	ChangeLayout(nWidth,nHeight);
}

void CSevenWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//��������ƶ����
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		//�����ˮƽ�ƶ�
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			//����ˮƽ�����ƶ���������
			int nXMove = pt.x - m_StartPoint.x;

			//����ˮƽ�ƶ�
			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//����Ǵ�ֱ�����ƶ�
		if (MOVE_VER == m_SelectSplitBarInfo.Type)
		{
			//���㴹ֱ�����ƶ���������
			int nYMove = pt.y - m_StartPoint.y;

			//����ֱ�ƶ�
			HandleVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//����Ǵ�ֱ��ˮƽ�ƶ�
		if (MOVE_ALL == m_SelectSplitBarInfo.Type)
		{
			//���㴹ֱ�ƶ���������
			int nYMove = pt.y - m_StartPoint.y;
			//����ˮƽ�ƶ���������
			int nXMove = pt.x - m_StartPoint.x;
			//���㴹ֱ��ˮƽ�ƶ���������
			HandleHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}

	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

void CSevenWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}
	CRect rc = m_HorSplitBarRect[nCurrentIndex];
	rc.OffsetRect(nMoveDistance,0);


	//��ȡǰһ�����ڵ�����
	int nPrexIndex = GetPrexHorSplitBarIndex(nCurrentIndex);

	//������귶Χ
	RECT CursorRect;
	//�����ǰһ���ָ���
	if (nPrexIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPrexIndex].right + DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//���û�зָ���
	else
	{
		CursorRect.left = DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}

	//�ͻ������ת��Ϊ��������
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//���ƹ��
		ClipCursor(&CursorRect);
	}
	//��ȡˮƽ�ָ���
	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();

	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

void CSevenWidgetSplitAlgorithm::HandleVerMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}
	CRect rc = m_VerSplitBarRect[nCurrentIndex];
	rc.OffsetRect(0,nMoveDistance);
	//������귶Χ
	RECT CursorRect;
	CursorRect.left = 0;
	CursorRect.top = DEFAULT_GAP;
	CursorRect.bottom = nHeight - DEFAULT_GAP;
	CursorRect.right = nWidth;
	//�ͻ������ת��Ϊ��������
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//���ƹ��
		ClipCursor(&CursorRect);
	}
	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

void CSevenWidgetSplitAlgorithm::HandleHorAndVerMove( int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}
	CRect HorRect = *(m_IntersectRect[nCurrentIndex].pRect1);
	CRect VerRect = *(m_IntersectRect[nCurrentIndex].pRect2);
	HorRect.OffsetRect(nMoveX,0);
	VerRect.OffsetRect(0,nMoveY);


	//������귶Χ
	RECT CursorRect;
	int nPreIndex = GetPrexHorSplitBarIndex(m_IntersectRect[nCurrentIndex].nHorIndex);
	if (nPreIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPreIndex].right + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	else
	{
		CursorRect.left = 0 + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//�ͻ������ת��Ϊ��������
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//���ƹ��
		ClipCursor(&CursorRect);
	}
	
	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&VerRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	pLayout = m_pLayout->GetMoveHorSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&HorRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

void CSevenWidgetSplitAlgorithm::CalculateCoordinateAfterHorMove( int nXMove,int nCurrentIndex,int nWidth,int nHeight )
{
	//�󴰿��ں���
	if (m_bWndFront)
	{
		m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
		//����ǵ�һ��λ��
		if (0 == nCurrentIndex)
		{
			//��ֱ�ָ���λ��Ҫ�ı�
			m_VerSplitBarRect[0].left += nXMove;
		}
		for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
		{
			if (i > nCurrentIndex)
			{
				//��������λ��
				m_HorSplitBarRect[i].OffsetRect(nXMove,0);
			}
		}
	}
	else
	{
		m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
		m_VerSplitBarRect[0].right += nXMove;
		//�����ķָ�����Ӧ����
		for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
		{
			if (i > nCurrentIndex)
			{
				m_HorSplitBarRect[i].OffsetRect(nXMove,0);
			}
		}		
	}
}

void CSevenWidgetSplitAlgorithm::CalculateCoordinateAfterVerMove( int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_VerSplitBarRect[nCurrentIndex].OffsetRect(0,nYMove);
}

void CSevenWidgetSplitAlgorithm::CalculateCoordinateAfterHorAndVerMove( int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	CRect* pRect = m_IntersectRect[nCurrentIndex].pRect1;
	if (NULL != pRect)
	{
		pRect->OffsetRect(nXMove,0);
	}

	//����ƶ�����ķָ���
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (i > m_IntersectRect[nCurrentIndex].nHorIndex)
		{
			m_HorSplitBarRect[i].OffsetRect(nXMove,0);
		}
	}
	pRect = m_IntersectRect[nCurrentIndex].pRect2;
	if (NULL != pRect)
	{
		pRect->OffsetRect(0,nYMove);
	}
}

void CSevenWidgetSplitAlgorithm::ReCalculateIntersetRect( void )
{
	//���¼��㽻���
	if (m_bWndFront)
	{
		m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
		m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[2],&m_VerSplitBarRect[0]);
	}
	else
	{
		m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
		m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	}
}

CEightWidgetSplitAlgorithm::CEightWidgetSplitAlgorithm( void )
{

}

CEightWidgetSplitAlgorithm::~CEightWidgetSplitAlgorithm( void )
{

}

void CEightWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 8)
	{
		return;
	}

	if (m_bWndFront)
	{
		AdjustLayoutFront(nWidth,nHeight);
	}
	else
	{
		AdjustLayoutBack(nWidth,nHeight);
	}
}

void CEightWidgetSplitAlgorithm::AdjustLayoutFront( int nWidth,int nHeight )
{

	//�ƶ���һ������λ��
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 3) / 4;
	rc1.bottom = rc1.top + (nHeight - DEFAULT_GAP) / 2;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ��ڶ�������λ��
	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + rc1.Width();
	rc2.bottom = rc2.top + rc1.Height();
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ�����������λ��
	CRect rc3;
	rc3.left = rc2.right + DEFAULT_GAP;
	rc3.top = rc2.top;
	rc3.right = rc3.left + rc2.Width();
	rc3.bottom = rc3.top + rc2.Height();
	pWidget = m_WidgetArray[2];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}


	//�ƶ����ĸ�����λ��
	CRect rc4;
	rc4.left = rc3.right + DEFAULT_GAP;
	rc4.top = rc3.top;
	rc4.right = rc4.left + rc3.Width();
	rc4.bottom = rc4.top + rc3.Height();
	pWidget = m_WidgetArray[3];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���5������λ��
	CRect rc5;
	rc5.left = rc1.left;
	rc5.top = rc1.bottom + DEFAULT_GAP;
	rc5.right = rc5.left + rc4.Width();
	rc5.bottom = rc5.top + rc4.Height();
	pWidget = m_WidgetArray[4];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc5);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���6������λ��
	CRect rc6;
	rc6.left = rc5.right + DEFAULT_GAP;
	rc6.top = rc5.top;
	rc6.right = rc6.left + rc5.Width();
	rc6.bottom = rc6.top + rc5.Height();
	pWidget = m_WidgetArray[5];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc6);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���7������λ��
	CRect rc7;
	rc7.left = rc6.right + DEFAULT_GAP;
	rc7.top = rc6.top;
	rc7.right = rc7.left + rc6.Width();
	rc7.bottom = rc7.top + rc6.Height();
	pWidget = m_WidgetArray[6];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc7);
		pWidget->ShowWindow(SW_SHOW);
	}
	//�ƶ���8������λ��
	CRect rc8;
	rc8.left = rc7.right + DEFAULT_GAP;
	rc8.top = rc7.top;
	rc8.right = rc8.left + rc7.Width();
	rc8.bottom = rc8.top + rc7.Height();
	pWidget = m_WidgetArray[7];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc8);
		pWidget->ShowWindow(SW_SHOW);
	}

	//���÷ָ�����λ��
	m_HorSplitBarRect[0].left =rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = m_HorSplitBarRect[0].left + DEFAULT_GAP;
	m_HorSplitBarRect[0].bottom = nHeight;

	m_HorSplitBarRect[1].left =rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = m_HorSplitBarRect[1].left + DEFAULT_GAP;
	m_HorSplitBarRect[1].bottom = nHeight;

	m_HorSplitBarRect[2].left =rc3.right;
	m_HorSplitBarRect[2].top = rc3.top;
	m_HorSplitBarRect[2].right = m_HorSplitBarRect[2].left + DEFAULT_GAP;
	m_HorSplitBarRect[2].bottom = nHeight;

	m_VerSplitBarRect[0].left =rc1.left;
	m_VerSplitBarRect[0].top = rc1.bottom;
	m_VerSplitBarRect[0].right = nWidth;
	m_VerSplitBarRect[0].bottom = m_VerSplitBarRect[0].top + DEFAULT_GAP;

	//�����ཻ�ľ���
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[0].pRect1 = &m_HorSplitBarRect[0];
	m_IntersectRect[0].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[0].nHorIndex = 0;
	m_IntersectRect[0].nVerIndex = 0;

	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].pRect1 = &m_HorSplitBarRect[1];
	m_IntersectRect[1].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[1].nHorIndex = 1;
	m_IntersectRect[1].nVerIndex = 0;

	m_IntersectRect[2].InterSectRect.IntersectRect(&m_HorSplitBarRect[2],&m_VerSplitBarRect[0]);
	m_IntersectRect[2].pRect1 = &m_HorSplitBarRect[2];
	m_IntersectRect[2].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[2].nHorIndex = 2;
	m_IntersectRect[2].nVerIndex = 0;
}

void CEightWidgetSplitAlgorithm::AdjustLayoutBack( int nWidth,int nHeight )
{
	AdjustLayoutFront(nWidth,nHeight);
}

void CEightWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	CRect Widget1;
	Widget1.left = 0;
	Widget1.top = 0;
	Widget1.right = m_HorSplitBarRect[0].left;
	Widget1.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget1);

	CRect Widget2;
	Widget2.left = m_HorSplitBarRect[0].right;
	Widget2.top = m_HorSplitBarRect[0].top;
	Widget2.right = m_HorSplitBarRect[1].left;
	Widget2.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget2);

	CRect Widget3;
	Widget3.left = m_HorSplitBarRect[1].right;
	Widget3.top = m_HorSplitBarRect[1].top;
	Widget3.right = m_HorSplitBarRect[2].left;
	Widget3.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget3);

	CRect Widget4;
	Widget4.left = m_HorSplitBarRect[2].right;
	Widget4.top = m_HorSplitBarRect[2].top;
	Widget4.right = nWidth;
	Widget4.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget4);

	CRect Widget5;
	Widget5.left = 0;
	Widget5.top = m_VerSplitBarRect[0].bottom;
	Widget5.right = m_HorSplitBarRect[0].left;
	Widget5.bottom = nHeight;
	LayoutSizeArray.push_back(Widget5);

	CRect Widget6;
	Widget6.left = m_HorSplitBarRect[0].right;
	Widget6.top = m_VerSplitBarRect[0].bottom;
	Widget6.right = m_HorSplitBarRect[1].left;
	Widget6.bottom = nHeight;
	LayoutSizeArray.push_back(Widget6);

	CRect Widget7;
	Widget7.left = m_HorSplitBarRect[1].right;
	Widget7.top = m_VerSplitBarRect[0].bottom;
	Widget7.right = m_HorSplitBarRect[2].left;
	Widget7.bottom = nHeight;
	LayoutSizeArray.push_back(Widget7);

	CRect Widget8;
	Widget8.left = m_HorSplitBarRect[2].right;
	Widget8.top = m_VerSplitBarRect[0].bottom;
	Widget8.right = nWidth;
	Widget8.bottom = nHeight;
	LayoutSizeArray.push_back(Widget8);
}

MOUSE_STATE CEightWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{
	//�ж��Ƿ��Ƿָ��߽����

	for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
	{
		if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
		{
			//��¼ѡ����ε�λ��
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_ALL;
			return MOVE_ALL;
		}
	}

	//�ж��Ƿ����ˮƽ�ƶ���
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
		{
			//��¼ѡ����ε�λ��
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}

	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		if (TRUE == m_VerSplitBarRect[i].PtInRect(pt))
		{
			//��¼ѡ����ε�λ��
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}
	}

	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

void CEightWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

void CEightWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	ClipCursor(NULL);
	//���طָ���
	ShowHorSplitBar(false);
	ShowVerSplitBar(false);

	//�����϶��ָ���֮��Ĳ���
	switch (m_SelectSplitBarInfo.Type)
	{
	case MOVE_HOR:
		{
			int nXMove = pt.x - m_StartPoint.x;
			CalculateCoordinateAfterHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_VER:
		{
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_ALL:
		{
			int nXMove = pt.x - m_StartPoint.x;
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	default:
		break;
	}

	//���¼��㽻���λ��
	ReCalculateIntersetRect();
	//�ı䲼��
	ChangeLayout(nWidth,nHeight);
}

/*****************************************************************************
@FunctionName : HandleMouseMove
@FunctionDescription : �ж�8�������µ�����ƶ�
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CEightWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//��������ƶ����
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		//�����ˮƽ�ƶ�
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			//����ˮƽ�����ƶ���������
			int nXMove = pt.x - m_StartPoint.x;

			//����ˮƽ�ƶ�
			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//����Ǵ�ֱ�����ƶ�
		if (MOVE_VER == m_SelectSplitBarInfo.Type)
		{
			//���㴹ֱ�����ƶ���������
			int nYMove = pt.y - m_StartPoint.y;

			//����ֱ�ƶ�
			HandleVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//����Ǵ�ֱ��ˮƽ�ƶ�
		if (MOVE_ALL == m_SelectSplitBarInfo.Type)
		{
			//���㴹ֱ�ƶ���������
			int nYMove = pt.y - m_StartPoint.y;
			//����ˮƽ�ƶ���������
			int nXMove = pt.x - m_StartPoint.x;
			//���㴹ֱ��ˮƽ�ƶ���������
			HandleHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}

	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

/*****************************************************************************
@FunctionName : HandleHorMove
@FunctionDescription : ����8�������µķָ����ƶ�
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CEightWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	//�����Ϸ����ж�
	if (NULL == m_pLayout)
	{
		return;
	}
	//��ȡѡ�еķָ�����λ��
	CRect rc = m_HorSplitBarRect[nCurrentIndex];
	rc.OffsetRect(nMoveDistance,0);
	//��ȡǰһ�����ڵ�����
	int nPrexIndex = GetPrexHorSplitBarIndex(nCurrentIndex);

	//������귶Χ
	RECT CursorRect;
	//�����ǰһ���ָ���
	if (nPrexIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPrexIndex].right + DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//���û�зָ���
	else
	{
		CursorRect.left = DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}

	//�ͻ������ת��Ϊ��������
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//���ƹ��
		ClipCursor(&CursorRect);
	}
	//��ȡˮƽ�ָ���
	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();

	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}

}

void CEightWidgetSplitAlgorithm::HandleVerMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	//�����Ϸ����ж�
	if (NULL == m_pLayout)
	{
		return;
	}
	CRect rc = m_VerSplitBarRect[nCurrentIndex];
	rc.OffsetRect(0,nMoveDistance);
	//������귶Χ
	RECT CursorRect;
	CursorRect.left = 0;
	CursorRect.top = DEFAULT_GAP;
	CursorRect.bottom = nHeight - DEFAULT_GAP;
	CursorRect.right = nWidth;
	//�ͻ������ת��Ϊ��������
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//���ƹ��
		ClipCursor(&CursorRect);
	}
	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}	
}

/*****************************************************************************
@FunctionName : HandleHorAndVerMove
@FunctionDescription : ����8�����ڵ�ˮƽ�ʹ�ֱ�ƶ�
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CEightWidgetSplitAlgorithm::HandleHorAndVerMove( int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}
	CRect HorRect = *(m_IntersectRect[nCurrentIndex].pRect1);
	CRect VerRect = *(m_IntersectRect[nCurrentIndex].pRect2);
	HorRect.OffsetRect(nMoveX,0);
	VerRect.OffsetRect(0,nMoveY);


	//������귶Χ
	RECT CursorRect;
	int nPreIndex = GetPrexHorSplitBarIndex(m_IntersectRect[nCurrentIndex].nHorIndex);
	if (nPreIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPreIndex].right + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	else
	{
		CursorRect.left = 0 + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//�ͻ������ת��Ϊ��������
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//���ƹ��
		ClipCursor(&CursorRect);
	}

	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&VerRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	pLayout = m_pLayout->GetMoveHorSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&HorRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}


}

void CEightWidgetSplitAlgorithm::CalculateCoordinateAfterHorMove( int nXMove,int nCurrentIndex,int nWidth,int nHeight )
{

	m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
	//�˷ָ�����ķָ�����Ҫ����ƫ��
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (i > nCurrentIndex)
		{
			//��������λ��
			m_HorSplitBarRect[i].OffsetRect(nXMove,0);
		}
	}

}

void CEightWidgetSplitAlgorithm::CalculateCoordinateAfterVerMove( int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_VerSplitBarRect[nCurrentIndex].OffsetRect(0,nYMove);
}

void CEightWidgetSplitAlgorithm::CalculateCoordinateAfterHorAndVerMove( int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	CRect* pRect = m_IntersectRect[nCurrentIndex].pRect1;
	if (NULL != pRect)
	{
		pRect->OffsetRect(nXMove,0);
	}

	//����ƶ�����ķָ���
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (i > m_IntersectRect[nCurrentIndex].nHorIndex)
		{
			m_HorSplitBarRect[i].OffsetRect(nXMove,0);
		}
	}
	pRect = m_IntersectRect[nCurrentIndex].pRect2;
	if (NULL != pRect)
	{
		pRect->OffsetRect(0,nYMove);
	}
}

void CEightWidgetSplitAlgorithm::ReCalculateIntersetRect( void )
{
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[2].InterSectRect.IntersectRect(&m_HorSplitBarRect[2],&m_VerSplitBarRect[0]);
}

CNineWidgetSplitAlgorithm::CNineWidgetSplitAlgorithm( void )
{

}

CNineWidgetSplitAlgorithm::~CNineWidgetSplitAlgorithm( void )
{

}

void CNineWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 9)
	{
		return;
	}

	if (m_bWndFront)
	{
		AdjustLayoutFront(nWidth,nHeight);
	}
	else
	{
		AdjustLayoutBack(nWidth,nHeight);
	}
}

void CNineWidgetSplitAlgorithm::AdjustLayoutFront( int nWidth,int nHeight )
{

	//�ƶ���һ������λ��
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 2) / 3;
	rc1.bottom = rc1.top + (nHeight - DEFAULT_GAP * 2) / 3;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ��ڶ�������λ��
	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + rc1.Width();
	rc2.bottom = rc2.top + rc1.Height();
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ�����������λ��
	CRect rc3;
	rc3.left = rc2.right + DEFAULT_GAP;
	rc3.top = rc2.top;
	rc3.right = rc3.left + rc2.Width();
	rc3.bottom = rc3.top + rc2.Height();
	pWidget = m_WidgetArray[2];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ����ĸ�����λ��
	CRect rc4;
	rc4.left = rc1.left;
	rc4.top = rc1.bottom + DEFAULT_GAP;
	rc4.right = rc4.left + rc3.Width();
	rc4.bottom = rc4.top + rc3.Height();
	pWidget = m_WidgetArray[3];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���5������λ��
	CRect rc5;
	rc5.left = rc4.right + DEFAULT_GAP;
	rc5.top = rc4.top;
	rc5.right = rc5.left + rc4.Width();
	rc5.bottom = rc5.top + rc4.Height();
	pWidget = m_WidgetArray[4];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc5);
		pWidget->ShowWindow(SW_SHOW);
	}
	//�ƶ���6������λ��
	CRect rc6;
	rc6.left = rc5.right + DEFAULT_GAP;
	rc6.top = rc5.top;
	rc6.right = rc6.left + rc5.Width();
	rc6.bottom = rc6.top + rc5.Height();
	pWidget = m_WidgetArray[5];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc6);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���7������λ��
	CRect rc7;
	rc7.left = rc4.left;
	rc7.top = rc4.bottom + DEFAULT_GAP;
	rc7.right = rc7.left + rc6.Width();
	rc7.bottom = rc7.top + rc6.Height();
	pWidget = m_WidgetArray[6];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc7);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���8������λ��
	CRect rc8;
	rc8.left = rc7.right + DEFAULT_GAP;
	rc8.top = rc7.top;
	rc8.right = rc8.left + rc7.Width();
	rc8.bottom = rc8.top + rc7.Height();
	pWidget = m_WidgetArray[7];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc8);
		pWidget->ShowWindow(SW_SHOW);
	}

	//�ƶ���9������λ��
	CRect rc9;
	rc9.left = rc8.right + DEFAULT_GAP;
	rc9.top = rc8.top;
	rc9.right = rc9.left + rc8.Width();
	rc9.bottom = rc9.top + rc8.Height();
	pWidget = m_WidgetArray[8];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc9);
		pWidget->ShowWindow(SW_SHOW);
	}

	//����ָ��ߵ�λ��
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = m_HorSplitBarRect[0].left + DEFAULT_GAP;
	m_HorSplitBarRect[0].bottom = nHeight;

	m_HorSplitBarRect[1].left = rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = m_HorSplitBarRect[1].left + DEFAULT_GAP;
	m_HorSplitBarRect[1].bottom = nHeight;


	m_VerSplitBarRect[0].left = rc1.left;
	m_VerSplitBarRect[0].top = rc1.bottom;
	m_VerSplitBarRect[0].right = nWidth;
	m_VerSplitBarRect[0].bottom = m_VerSplitBarRect[0].top + DEFAULT_GAP;


	m_VerSplitBarRect[1].left = rc4.left;
	m_VerSplitBarRect[1].top = rc4.bottom;
	m_VerSplitBarRect[1].right = nWidth;
	m_VerSplitBarRect[1].bottom = m_VerSplitBarRect[1].top + DEFAULT_GAP;

	//�����ཻ�ľ���
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[0].pRect1 = &m_HorSplitBarRect[0];
	m_IntersectRect[0].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[0].nHorIndex = 0;
	m_IntersectRect[0].nVerIndex = 0;

	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].pRect1 = &m_HorSplitBarRect[1];
	m_IntersectRect[1].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[1].nHorIndex = 1;
	m_IntersectRect[1].nVerIndex = 0;

	m_IntersectRect[2].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[1]);
	m_IntersectRect[2].pRect1 = &m_HorSplitBarRect[0];
	m_IntersectRect[2].pRect2 = &m_VerSplitBarRect[1];
	m_IntersectRect[2].nHorIndex = 0;
	m_IntersectRect[2].nVerIndex = 1;

	m_IntersectRect[3].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[1]);
	m_IntersectRect[3].pRect1 = &m_HorSplitBarRect[1];
	m_IntersectRect[3].pRect2 = &m_VerSplitBarRect[1];
	m_IntersectRect[3].nHorIndex = 1;
	m_IntersectRect[3].nVerIndex = 1;
}

void CNineWidgetSplitAlgorithm::AdjustLayoutBack( int nWidth,int nHeight )
{
	AdjustLayoutFront(nWidth,nHeight);
}

void CNineWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	CRect Widget1;
	Widget1.left = 0;
	Widget1.top = m_HorSplitBarRect[0].top;
	Widget1.right = m_HorSplitBarRect[0].left;
	Widget1.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget1);

	CRect Widget2;
	Widget2.left = m_HorSplitBarRect[0].right;
	Widget2.top = m_HorSplitBarRect[0].top;
	Widget2.right = m_HorSplitBarRect[1].left;
	Widget2.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget2);

	CRect Widget3;
	Widget3.left = m_HorSplitBarRect[1].right;
	Widget3.top = m_HorSplitBarRect[1].top;
	Widget3.right = nWidth;
	Widget3.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget3);

	CRect Widget4;
	Widget4.left = m_VerSplitBarRect[0].left;
	Widget4.top = m_VerSplitBarRect[0].bottom;
	Widget4.right = m_HorSplitBarRect[0].left;
	Widget4.bottom = m_VerSplitBarRect[1].top;
	LayoutSizeArray.push_back(Widget4);

	CRect Widget5;
	Widget5.left = m_HorSplitBarRect[0].right;
	Widget5.top = m_VerSplitBarRect[0].bottom;
	Widget5.right = m_HorSplitBarRect[1].left;
	Widget5.bottom = m_VerSplitBarRect[1].top;
	LayoutSizeArray.push_back(Widget5);

	CRect Widget6;
	Widget6.left = m_HorSplitBarRect[1].right;
	Widget6.top = m_VerSplitBarRect[0].bottom;
	Widget6.right = nWidth;
	Widget6.bottom = m_VerSplitBarRect[1].top;
	LayoutSizeArray.push_back(Widget6);

	CRect Widget7;
	Widget7.left = m_VerSplitBarRect[1].left;
	Widget7.top = m_VerSplitBarRect[1].bottom;
	Widget7.right = m_HorSplitBarRect[0].left;
	Widget7.bottom = nHeight;
	LayoutSizeArray.push_back(Widget7);

	CRect Widget8;
	Widget8.left = m_HorSplitBarRect[0].right;
	Widget8.top = m_VerSplitBarRect[1].bottom;
	Widget8.right = m_HorSplitBarRect[1].left;
	Widget8.bottom = nHeight;
	LayoutSizeArray.push_back(Widget8);

	CRect Widget9;
	Widget9.left = m_HorSplitBarRect[1].right;
	Widget9.top = m_VerSplitBarRect[1].bottom;
	Widget9.right = nWidth;
	Widget9.bottom = nHeight;
	LayoutSizeArray.push_back(Widget9);

}

MOUSE_STATE CNineWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{
	//�ж��Ƿ���4�����㴦
	for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
	{
		if (m_IntersectRect[i].InterSectRect.PtInRect(pt) == TRUE)
		{
			//��¼ѡ����ε�λ��
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_ALL;
			return MOVE_ALL;
		}
	}

	//�Ƿ����ˮƽ�ƶ�
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (m_HorSplitBarRect[i].PtInRect(pt) == TRUE)
		{
			//��¼ѡ����ε�λ��
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}

	//�Ƿ���Դ�ֱ�ƶ�
	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		if (m_VerSplitBarRect[i].PtInRect(pt) == TRUE)
		{
			//��¼ѡ����ε�λ��
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}
	}
	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

void CNineWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

void CNineWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	ClipCursor(NULL);
	//���طָ���
	ShowHorSplitBar(false);
	ShowVerSplitBar(false);

	//�����϶��ָ���֮��Ĳ���
	switch (m_SelectSplitBarInfo.Type)
	{
	case MOVE_HOR:
		{
			int nXMove = pt.x - m_StartPoint.x;
			CalculateCoordinateAfterHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_VER:
		{
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_ALL:
		{
			int nXMove = pt.x - m_StartPoint.x;
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	default:
		break;
	}

	//���¼��㽻���λ��
	ReCalculateIntersetRect();
	//�ı䲼��
	ChangeLayout(nWidth,nHeight);
}

void CNineWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//��������ƶ����
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		//�����ˮƽ�ƶ�
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			//����ˮƽ�����ƶ���������
			int nXMove = pt.x - m_StartPoint.x;

			//����ˮƽ�ƶ�
			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//����Ǵ�ֱ�����ƶ�
		if (MOVE_VER == m_SelectSplitBarInfo.Type)
		{
			//���㴹ֱ�����ƶ���������
			int nYMove = pt.y - m_StartPoint.y;

			//����ֱ�ƶ�
			HandleVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//����Ǵ�ֱ��ˮƽ�ƶ�
		if (MOVE_ALL == m_SelectSplitBarInfo.Type)
		{
			//���㴹ֱ�ƶ���������
			int nYMove = pt.y - m_StartPoint.y;
			//����ˮƽ�ƶ���������
			int nXMove = pt.x - m_StartPoint.x;
			//���㴹ֱ��ˮƽ�ƶ���������
			HandleHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}

	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

void CNineWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	//�����Ϸ����ж�
	if (NULL == m_pLayout)
	{
		return;
	}
	//��ȡѡ�еķָ�����λ��
	CRect rc = m_HorSplitBarRect[nCurrentIndex];
	rc.OffsetRect(nMoveDistance,0);
	//��ȡǰһ�����ڵ�����
	int nPrexIndex = GetPrexHorSplitBarIndex(nCurrentIndex);

	//������귶Χ
	RECT CursorRect;
	//�����ǰһ���ָ���
	if (nPrexIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPrexIndex].right + DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//���û�зָ���
	else
	{
		CursorRect.left = DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}

	//�ͻ������ת��Ϊ��������
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//���ƹ��
		ClipCursor(&CursorRect);
	}
	//��ȡˮƽ�ָ���
	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();

	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

void CNineWidgetSplitAlgorithm::HandleVerMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	//�����Ϸ����ж�
	if (NULL == m_pLayout)
	{
		return;
	}
	//��ȡѡ�еķָ�����λ��
	CRect rc = m_VerSplitBarRect[nCurrentIndex];
	rc.OffsetRect(0,nMoveDistance);
	//��ȡǰһ�����ڵ�����
	int nPrexIndex = GetPrexVerSplitBarIndex(nCurrentIndex);

	//������귶Χ
	RECT CursorRect;
	//�����ǰһ���ָ���
	if (nPrexIndex >= 0)
	{
		CursorRect.left = 0;
		CursorRect.top = m_VerSplitBarRect[nPrexIndex].bottom + DEFAULT_GAP;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//���û�зָ���
	else
	{
		CursorRect.left = 0;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth;
	}

	//�ͻ������ת��Ϊ��������
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//���ƹ��
		ClipCursor(&CursorRect);
	}
	//��ȡˮƽ�ָ���
	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();

	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

void CNineWidgetSplitAlgorithm::HandleHorAndVerMove( int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight )
{

	if (NULL == m_pLayout)
	{
		return;
	}
	CRect HorRect = *(m_IntersectRect[nCurrentIndex].pRect1);
	CRect VerRect = *(m_IntersectRect[nCurrentIndex].pRect2);
	HorRect.OffsetRect(nMoveX,0);
	VerRect.OffsetRect(0,nMoveY);


	//������귶Χ
	RECT CursorRect;
	int nPreIndex = GetPrexHorSplitBarIndex(m_IntersectRect[nCurrentIndex].nHorIndex);
	if (nPreIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPreIndex].right + DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	else
	{
		CursorRect.left = 0 + DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}


	nPreIndex = GetPrexVerSplitBarIndex(m_IntersectRect[nCurrentIndex].nVerIndex);
	if (nPreIndex >= 0)
	{	
		CursorRect.top = m_VerSplitBarRect[nPreIndex].bottom + DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
	}
	else
	{
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
	}

	//�ͻ������ת��Ϊ��������
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//���ƹ��
		ClipCursor(&CursorRect);
	}

	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&VerRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	pLayout = m_pLayout->GetMoveHorSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&HorRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}

}

void CNineWidgetSplitAlgorithm::CalculateCoordinateAfterHorMove( int nXMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
	//�˷ָ�����ķָ�����Ҫ����ƫ��
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (i > nCurrentIndex)
		{
			//��������λ��
			m_HorSplitBarRect[i].OffsetRect(nXMove,0);
		}
	}
}

void CNineWidgetSplitAlgorithm::CalculateCoordinateAfterVerMove( int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_VerSplitBarRect[nCurrentIndex].OffsetRect(0,nYMove);
	//�˷ָ�����ķָ�����Ҫ����ƫ��
	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		if (i > nCurrentIndex)
		{
			//��������λ��
			m_VerSplitBarRect[i].OffsetRect(0,nYMove);
		}
	}
}

void CNineWidgetSplitAlgorithm::CalculateCoordinateAfterHorAndVerMove( int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	CRect* pRect = m_IntersectRect[nCurrentIndex].pRect1;
	if (NULL != pRect)
	{
		pRect->OffsetRect(nXMove,0);
	}

	//����ƶ�����ķָ���
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (i > m_IntersectRect[nCurrentIndex].nHorIndex)
		{
			m_HorSplitBarRect[i].OffsetRect(nXMove,0);
		}
	}
	pRect = m_IntersectRect[nCurrentIndex].pRect2;
	if (NULL != pRect)
	{
		pRect->OffsetRect(0,nYMove);
	}

	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		if (i > m_IntersectRect[nCurrentIndex].nVerIndex)
		{
			m_VerSplitBarRect[i].OffsetRect(0,nYMove);
		}
	}

}

void CNineWidgetSplitAlgorithm::ReCalculateIntersetRect( void )
{
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[2].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[1]);
	m_IntersectRect[3].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[1]);
}
