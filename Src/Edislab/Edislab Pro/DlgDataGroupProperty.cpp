// DlgDataGroupProperty.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgDataGroupProperty.h"
#include "Utility.h"
#include "GridColumnGroupManager.h"
// CDlgDataGroupProperty �Ի���

IMPLEMENT_DYNAMIC(CDlgDataGroupProperty, CBaseDialog)

CDlgDataGroupProperty::CDlgDataGroupProperty(const CString& strGroupName,CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgDataGroupProperty::IDD, pParent),
	m_strGroupName(strGroupName)
{

}

CDlgDataGroupProperty::~CDlgDataGroupProperty()
{
}

void CDlgDataGroupProperty::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDataGroupProperty, CBaseDialog)
	ON_BN_CLICKED(ID_BTN_OK, &CDlgDataGroupProperty::OnBnClickedBtnOk)
	ON_BN_CLICKED(ID_BTN_CANCEL, &CDlgDataGroupProperty::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CDlgDataGroupProperty ��Ϣ�������


BOOL CDlgDataGroupProperty::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitDisplay();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgDataGroupProperty::InitDisplay()
{
	CWnd* pWnd = GetDlgItem(IDC_DATA_GROUP_NAME);
	if (nullptr != pWnd && NULL != pWnd->GetSafeHwnd())
	{
		pWnd->SetWindowText(m_strGroupName);
	}

	CTime CurrentTime = CTime::GetCurrentTime();

	CString strTime = CurrentTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	pWnd = GetDlgItem(IDC_DATA_YIELD_TIME);

	if (nullptr != pWnd && NULL != pWnd->GetSafeHwnd())
	{
		pWnd->SetWindowText(strTime);
	}
}


void CDlgDataGroupProperty::OnBnClickedBtnOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWnd* pWnd = GetDlgItem(IDC_DATA_GROUP_NAME);
	if (nullptr != pWnd && NULL != pWnd->GetSafeHwnd())
	{
		CString strGroupName;
		pWnd->GetWindowText(strGroupName);

		if (strGroupName.IsEmpty())
		{
			Utility::AfxBCGPMessageBox(_T("������������Ϊ��!"),MB_OK | MB_ICONERROR);
			return;
		}

		//�����δ��ĳ�ָı�
		if (strGroupName == m_strGroupName)
		{
			OnOK();
			return;
		}

		if (CGridColumnGroupManager::CreateInstance().IsHeaderNameExist(strGroupName))
		{
			Utility::AfxBCGPMessageBox(_T("���������Ѿ�����,����������һ������!"),MB_OK | MB_ICONERROR);
			return;
		}
		m_strGroupName = strGroupName;
		OnOK();
	}

}


void CDlgDataGroupProperty::OnBnClickedBtnCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

CString CDlgDataGroupProperty::GetGroupName(void)
{
	return m_strGroupName;
}


BOOL CDlgDataGroupProperty::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_RETURN == pMsg->wParam)
		{
			OnBnClickedBtnOk();
			return TRUE;
		}
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
