#include "stdafx.h"
#include "BaseRibbonBar.h"


CBaseRibbonBar::CBaseRibbonBar(void)
{
}


CBaseRibbonBar::~CBaseRibbonBar(void)
{
}
BEGIN_MESSAGE_MAP(CBaseRibbonBar, CBCGPRibbonBar)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


void CBaseRibbonBar::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: �ڴ˴������Ϣ����������
}
