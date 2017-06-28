// DlgAddColumn.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgAddColumn.h"
// CDlgAddColumn �Ի���

IMPLEMENT_DYNAMIC(CDlgAddColumn, CBaseDialog)

CDlgAddColumn::CDlgAddColumn(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAddColumn::IDD, pParent)
{

}

CDlgAddColumn::~CDlgAddColumn()
{
}

void CDlgAddColumn::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAddColumn, CBaseDialog)
END_MESSAGE_MAP()


// CDlgAddColumn ��Ϣ�������


BOOL CDlgAddColumn::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitDisplay();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgAddColumn::InitDisplay( void )
{
	CWnd* pWnd = GetDlgItem(IDC_TAB_POS);
	if (nullptr != pWnd && NULL != pWnd->GetSafeHwnd())
	{
		pWnd->ShowWindow(SW_HIDE);
		//��ȡλ��
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		if (NULL == m_TabWnd)
		{
			if (FALSE == m_TabWnd.Create(CBCGPTabWnd::STYLE_3D,rc,this,CBaseTabWnd::s_nTabBaseID++
				,CBCGPBaseTabWnd::LOCATION_TOP))
			{
				return;
			}
		}
	}
}
