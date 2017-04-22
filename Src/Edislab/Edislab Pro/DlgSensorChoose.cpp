// DlgSensorChoose.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgSensorChoose.h"
#include <boost/foreach.hpp>
#include "Global.h"
// CDlgSensorChoose �Ի���

IMPLEMENT_DYNAMIC(CDlgSensorChoose, CBaseDialog)

CDlgSensorChoose::CDlgSensorChoose(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSensorChoose::IDD, pParent)
{

}

CDlgSensorChoose::~CDlgSensorChoose()
{
}

void CDlgSensorChoose::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_SENSOR_TYPE, m_CmbSensorType);
	DDX_Control(pDX, IDC_CMB_RANGE,       m_CmbRange);
	DDX_Control(pDX, IDC_BTN_ADD,         m_BtnAdd);
	DDX_Control(pDX, IDC_BTN_DELETE,      m_BtnDel);
	DDX_Control(pDX, IDC_BTN_DELETE_ALL,  m_BtnDelAll);
	DDX_Control(pDX, IDC_BTN_CHOOSE_CONNECTED, m_BtnChooseCon);
	DDX_Control(pDX, IDC_SENSOR_LIST,           m_ListSensor);
	DDX_Control(pDX, IDC_SELECTED_SENSOR_LIST,           m_ListChoosedSensor);
	DDX_Control(pDX, IDC_STATIC_RESOLUTION_RATIO, m_StaticResolutionRatio);
	DDX_Control(pDX, IDC_CHECK_AUTO_RECOGNIZE, m_CheckAutoChoose);
}


BEGIN_MESSAGE_MAP(CDlgSensorChoose, CDialog)
	ON_BN_CLICKED(IDC_CHECK_AUTO_RECOGNIZE, &CDlgSensorChoose::OnBnClickedCheckAutoRecognize)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgSensorChoose::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CDlgSensorChoose::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_DELETE_ALL, &CDlgSensorChoose::OnBnClickedBtnDeleteAll)
	ON_BN_CLICKED(IDC_BTN_CHOOSE_CONNECTED, &CDlgSensorChoose::OnBnClickedBtnChooseConnected)
	ON_BN_CLICKED(ID_BTN_OK, &CDlgSensorChoose::OnBnClickedBtnOk)
END_MESSAGE_MAP()


// CDlgSensorChoose ��Ϣ�������
void CDlgSensorChoose::InitCtrls()
{

	//begin add by xiaowei.han at 2017/04/23 0:18:43
	if (m_BtnChooseCon.GetSafeHwnd())
	{
		if (g_bAutoSelect)
		{
			m_BtnChooseCon.SetCheck(BST_CHECKED);
		}
		else
		{
			m_BtnChooseCon.SetCheck(BST_CHECKED);
		}
		
	}
	if (m_CmbSensorType.GetSafeHwnd())
	{
		CString strCategoryArray[] = {_T("����"),_T("����"),_T("��ѧ"),_T("����")};

		BOOST_FOREACH(auto& Element,strCategoryArray)
		{
			m_CmbSensorType.AddString(Element);
		}

		m_CmbSensorType.SetCurSel(0);
	}
	//end add by xiaowei.han at 2017/04/23 0:18:43

}



void CDlgSensorChoose::OnBnClickedCheckAutoRecognize()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(BST_CHECKED == m_CheckAutoChoose.GetCheck())
	{
		m_BtnAdd.EnableWindow(false);
		m_BtnDel.EnableWindow(false);
		m_BtnDelAll.EnableWindow(false);
		m_BtnChooseCon.EnableWindow(false);
	}
	else
	{
		m_BtnAdd.EnableWindow(true);
		m_BtnDel.EnableWindow(true);
		m_BtnDelAll.EnableWindow(true);
		m_BtnChooseCon.EnableWindow(true);
	}

	// ��Ӵ�����ʶ�𲿷ִ���


}


BOOL CDlgSensorChoose::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitCtrls();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgSensorChoose::OnBnClickedBtnAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDlgSensorChoose::OnBnClickedBtnDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDlgSensorChoose::OnBnClickedBtnDeleteAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDlgSensorChoose::OnBnClickedBtnChooseConnected()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDlgSensorChoose::OnBnClickedBtnOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
