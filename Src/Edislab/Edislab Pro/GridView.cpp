// GridView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "GridView.h"


// CGridView

IMPLEMENT_DYNCREATE(CGridView, CBCGPFormView)

CGridView::CGridView()
	: CBCGPFormView(CGridView::IDD)
{

}

CGridView::~CGridView()
{
}

void CGridView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGridView, CBCGPFormView)
END_MESSAGE_MAP()


// CGridView ���

#ifdef _DEBUG
void CGridView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CGridView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGridView ��Ϣ�������
