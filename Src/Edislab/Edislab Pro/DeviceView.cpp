// DeviceView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DeviceView.h"


// CDeviceView

IMPLEMENT_DYNCREATE(CDeviceView, CBCGPFormView)

CDeviceView::CDeviceView()
	: CBCGPFormView(CDeviceView::IDD)
{

}

CDeviceView::~CDeviceView()
{
}

void CDeviceView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDeviceView, CBCGPFormView)
END_MESSAGE_MAP()


// CDeviceView ���

#ifdef _DEBUG
void CDeviceView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDeviceView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDeviceView ��Ϣ�������
