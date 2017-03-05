// DlgAddPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgAddPage.h"
#include "Utility.h"
// CDlgAddPage �Ի���

IMPLEMENT_DYNAMIC(CDlgAddPage, CBaseDialog)

CDlgAddPage::CDlgAddPage(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAddPage::IDD, pParent)
{

}

CDlgAddPage::~CDlgAddPage()
{
}

void CDlgAddPage::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PAGE_NAME, m_PageNameEdit);
	DDX_Control(pDX, IDC_COMBO_GRID, m_GridCombox);
	DDX_Control(pDX, IDC_COMBO_DIAGRAM, m_DiagramCombox);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_DeviceCombox);
}


BEGIN_MESSAGE_MAP(CDlgAddPage, CBaseDialog)
	ON_BN_CLICKED(IDC_RADIO_EMPTY_PAGE, &CDlgAddPage::OnBnClickedRadioEmptyPage)
	ON_BN_CLICKED(IDC_RADIO_COPY_PAGE, &CDlgAddPage::OnBnClickedRadioCopyPage)
	ON_BN_CLICKED(IDC_RADIO_CUSTOM_PAGE, &CDlgAddPage::OnBnClickedRadioCustomPage)
	ON_BN_CLICKED(IDC_BTN_OK, &CDlgAddPage::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgAddPage::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CDlgAddPage ��Ϣ�������


BOOL CDlgAddPage::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitDisplay();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

ADD_PAGE_ELEMENT CDlgAddPage::GetAddPageElement( void ) const
{
	return m_AddPageElement;
}

void CDlgAddPage::InitDisplay( void )
{
	if (NULL != m_PageNameEdit.GetSafeHwnd())
	{
		m_PageNameEdit.SetWindowText(DEFAULT_PAGE_NAME);
	}

	if (NULL != m_GridCombox.GetSafeHwnd())
	{
		for (int i = 0; i < 10; ++i)
		{
			CString strIndex;
			strIndex.Format(_T("%d"),i);
			m_GridCombox.AddString(strIndex);
		}
		m_GridCombox.SetCurSel(1);
	}

	if (NULL != m_DiagramCombox.GetSafeHwnd())
	{
		for (int i = 0; i < 10; ++i)
		{
			CString strIndex;
			strIndex.Format(_T("%d"),i);
			m_DiagramCombox.AddString(strIndex);
		}
		m_DiagramCombox.SetCurSel(1);
	}

	if (NULL != m_DeviceCombox.GetSafeHwnd())
	{
		for (int i = 0; i < 10; ++i)
		{
			CString strIndex;
			strIndex.Format(_T("%d"),i);
			m_DeviceCombox.AddString(strIndex);
		}
		m_DeviceCombox.SetCurSel(1);
	}


	CBCGPButton* pButton = (CBCGPButton*)GetDlgItem(IDC_RADIO_CUSTOM_PAGE);

	if (NULL != pButton && NULL != pButton->GetSafeHwnd())
	{
		if (BST_CHECKED == pButton->GetCheck())
		{
			m_DeviceCombox.EnableWindow(TRUE);
			m_DiagramCombox.EnableWindow(TRUE);
			m_GridCombox.EnableWindow(TRUE);
		}
		else
		{
			m_DeviceCombox.EnableWindow(FALSE);
			m_DiagramCombox.EnableWindow(FALSE);
			m_GridCombox.EnableWindow(FALSE);

		}
	}

	pButton = (CBCGPButton*)GetDlgItem(IDC_RADIO_COPY_PAGE);

	if (NULL != pButton && NULL != pButton->GetSafeHwnd())
	{
		pButton->SetCheck(BST_CHECKED);
	}

}


void CDlgAddPage::OnBnClickedRadioEmptyPage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DeviceCombox.EnableWindow(FALSE);
	m_DiagramCombox.EnableWindow(FALSE);
	m_GridCombox.EnableWindow(FALSE);
}


void CDlgAddPage::OnBnClickedRadioCopyPage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DeviceCombox.EnableWindow(FALSE);
	m_DiagramCombox.EnableWindow(FALSE);
	m_GridCombox.EnableWindow(FALSE);
}


void CDlgAddPage::OnBnClickedRadioCustomPage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CBCGPButton* pButton = (CBCGPButton*)GetDlgItem(IDC_RADIO_CUSTOM_PAGE);

	if (NULL != pButton && NULL != pButton->GetSafeHwnd())
	{
		if (BST_CHECKED == pButton->GetCheck())
		{
			m_DeviceCombox.EnableWindow(TRUE);
			m_DiagramCombox.EnableWindow(TRUE);
			m_GridCombox.EnableWindow(TRUE);
		}
	}
}


void CDlgAddPage::OnBnClickedBtnOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (HandleAddPage())
	{
		OnOK();
	}
	
}


void CDlgAddPage::OnBnClickedBtnCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

bool CDlgAddPage::HandleAddPage( void )
{
	//��ȡʵ��ҳ������
	CString strPageName;
	if (NULL != m_PageNameEdit.GetSafeHwnd())
	{
		m_PageNameEdit.GetWindowText(strPageName);
	}
	if (strPageName.IsEmpty())
	{
		Utility::AfxBCGPMessageBox(_T("ҳ�����Ʋ���Ϊ��!"),MB_OK);
		return false;
	}

	//�Ƿ����հ�ҳ
	CBCGPButton* pButton = (CBCGPButton*)GetDlgItem(IDC_RADIO_EMPTY_PAGE);

	if (NULL != pButton && NULL != pButton->GetSafeHwnd())
	{
		if (BST_CHECKED == pButton->GetCheck())
		{
			//�������������հ�ҳ
			return HandleAddEmptyPage(strPageName);
		}
	}

	pButton = (CBCGPButton*)GetDlgItem(IDC_RADIO_COPY_PAGE);

	if (NULL != pButton && NULL != pButton->GetSafeHwnd())
	{
		if (BST_CHECKED == pButton->GetCheck())
		{
			//�������������հ�ҳ
			return HandleAddCopyPage(strPageName);
		}
	}

	pButton = (CBCGPButton*)GetDlgItem(IDC_RADIO_CUSTOM_PAGE);

	if (NULL != pButton && NULL != pButton->GetSafeHwnd())
	{
		if (BST_CHECKED == pButton->GetCheck())
		{
			//�������������Զ���ҳ
			return HandleCustomPage(strPageName);
		}
	}
	return false;
}

bool CDlgAddPage::HandleAddEmptyPage( const CString& strPageName )
{
	m_AddPageElement.strPageName = strPageName;
	m_AddPageElement.eumOpt = NEW_PAGE_EMPTY;
	m_AddPageElement.nDeviceNum = 0;
	m_AddPageElement.nDiagramNum = 0;
	m_AddPageElement.nGridNum = 0;
	return true;
}

bool CDlgAddPage::HandleAddCopyPage( const CString& strPageName )
{
	m_AddPageElement.strPageName = strPageName;
	m_AddPageElement.eumOpt = NEW_PAGE_COPY;
	return true;
}

bool CDlgAddPage::HandleCustomPage( const CString& strPageName )
{
	//��ȡ����Grid
	if (NULL == m_GridCombox.GetSafeHwnd() || NULL == m_DiagramCombox.GetSafeHwnd() || NULL == m_DeviceCombox.GetSafeHwnd())
	{
		return false;
	}

	int nGridNum = m_GridCombox.GetCurSel();
	int nDiagramNum = m_DiagramCombox.GetCurSel();
	int nDeviceNum = m_DeviceCombox.GetCurSel();

	m_AddPageElement.strPageName = strPageName;
	m_AddPageElement.eumOpt = NEW_PAGE_CUSTOM;
	m_AddPageElement.nDeviceNum = nDeviceNum;
	m_AddPageElement.nDiagramNum = nDiagramNum;
	m_AddPageElement.nGridNum = nGridNum;

	return true;
}
