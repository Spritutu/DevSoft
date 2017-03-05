// DlgPageNameConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgPageNameConfig.h"
#include "Utility.h"
// CDlgPageNameConfig �Ի���

IMPLEMENT_DYNAMIC(CDlgPageNameConfig, CBaseDialog)

CDlgPageNameConfig::CDlgPageNameConfig( const CString& strPageName,CWnd* pParent /*= NULL*/ )
	: CBaseDialog(CDlgPageNameConfig::IDD, pParent),
	m_strPageName(strPageName)
{

}


CDlgPageNameConfig::~CDlgPageNameConfig()
{
}

void CDlgPageNameConfig::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PAGE_NAME, m_EditPageName);
}


BEGIN_MESSAGE_MAP(CDlgPageNameConfig, CBaseDialog)
	ON_BN_CLICKED(IDC_BTN_OK, &CDlgPageNameConfig::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgPageNameConfig::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CDlgPageNameConfig ��Ϣ�������


BOOL CDlgPageNameConfig::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitDisplay();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgPageNameConfig::InitDisplay( void )
{
	if (NULL != m_EditPageName.GetSafeHwnd())
	{
		if (!m_strPageName.IsEmpty())
		{
			m_EditPageName.SetWindowText(m_strPageName);
		}
	}
}


void CDlgPageNameConfig::OnBnClickedBtnOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (NULL == m_EditPageName.GetSafeHwnd())
	{
		return;
	}

	m_EditPageName.GetWindowText(m_strPageName);

	if (m_strPageName.IsEmpty())
	{
		Utility::AfxBCGPMessageBox(_T("ҳ�����Ʋ���Ϊ��!"),MB_OK);
		return;
	}
	OnOK();
}


void CDlgPageNameConfig::OnBnClickedBtnCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

CString CDlgPageNameConfig::GetPageName( void ) const
{
	return m_strPageName;
}
