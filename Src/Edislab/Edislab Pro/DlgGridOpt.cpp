// DlgGridOpt.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgGridOpt.h"


// CDlgGridOpt �Ի���

IMPLEMENT_DYNAMIC(CDlgGridOpt, CBaseDialog)

CDlgGridOpt::CDlgGridOpt(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgGridOpt::IDD, pParent)
{
	m_GridDisplayArray.clear();
}

CDlgGridOpt::~CDlgGridOpt()
{
}

void CDlgGridOpt::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_DISPLAY_TREE, m_GridDisplayItemTree);
}


BEGIN_MESSAGE_MAP(CDlgGridOpt, CBaseDialog)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgGridOpt::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgGridOpt::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CDlgGridOpt ��Ϣ�������


BOOL CDlgGridOpt::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitDisplay();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

/*******************************************************************
*��������:InitDisplay
*��������:��ʼ���Ի�����ʾ
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/04 15:04:56
*******************************************************************/
void CDlgGridOpt::InitDisplay(void)
{
	if (NULL == m_GridDisplayItemTree.GetSafeHwnd())
	{
		return;
	}
}


void CDlgGridOpt::OnBnClickedBtnApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}


void CDlgGridOpt::OnBnClickedBtnCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CDlgGridOpt::SetDisplayInfo(const std::vector<SHOW_COLUMN_GROUP_INFO>& GridDisplayArray)
{
	m_GridDisplayArray = GridDisplayArray;
}

void CDlgGridOpt::GetDisplayInfo(std::vector<SHOW_COLUMN_GROUP_INFO>& GridDisplayArray)
{
	GridDisplayArray.clear();

	GridDisplayArray = m_GridDisplayArray;
}
