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
	ON_CBN_SELCHANGE(IDC_CMB_SENSOR_TYPE, &CDlgSensorChoose::OnCbnSelchangeCmbSensorType)
	ON_LBN_SELCHANGE(IDC_SELECTED_SENSOR_LIST, &CDlgSensorChoose::OnLbnSelchangeSelectedSensorList)
	ON_CBN_SELCHANGE(IDC_CMB_RANGE, &CDlgSensorChoose::OnCbnSelchangeCmbRange)
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
		//CString strCategoryArray[] = {_T("����"),_T("����"),_T("��ѧ"),_T("����")};
		std::vector<SENSOR_TYPE_INFO> vecSensorType = CSensorConfig::CreateInstance().GetSensorTypeInfo();
		CString strName;
		int nIndex = 0;
		BOOST_FOREACH(auto& Element,vecSensorType)
		{
			strName = Element.strTypeName.c_str();
			nIndex = m_CmbSensorType.AddString(strName);
			m_CmbSensorType.SetItemData(nIndex, Element.enumType);
		}

		m_CmbSensorType.SetCurSel(nIndex);

		RefreshSensorList();
	}
	//end add by xiaowei.han at 2017/04/23 0:18:43

	if (m_ListChoosedSensor.GetSafeHwnd())
	{
		m_ListChoosedSensor.EnableItemDescription(TRUE, 1);
		m_ListChoosedSensor.SetImageList(IDB_SENSOR_ICON_LIST, 48);
	}

	if (m_CheckAutoChoose.GetSafeHwnd())
	{
		m_CheckAutoChoose.SetCheck(TRUE);
		OnBnClickedCheckAutoRecognize();
	}

	if (m_ListSensor.GetSafeHwnd())
	{
		m_ListSensor.SetImageList(IDB_SENSOR_ICON_LIST, 48);
	}

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
	int nIndex = m_ListSensor.GetCurSel();
	nIndex = (int)m_ListSensor.GetItemData(nIndex);
	std::map<int, SENSOR_CONFIG_ELEMENT>::iterator iter = m_mapCurrentSensor.find(nIndex);
	if (m_mapCurrentSensor.cend() != iter)
	{
		CString str;
        // ƴ��������ͷ
		int nNum = (int)m_setChooseSensorID.count(iter->second.nSensorID);
		if (nNum == 0)
		{
			str.Format(_T("�����У�%s(%s)"), CString(iter->second.strSensorSymbol.c_str()), CString(iter->second.strSensorName.c_str()));
		}
		else
		{
            str.Format(_T("�����У�%s_%d(%s)"), CString(iter->second.strSensorSymbol.c_str()), nNum, CString(iter->second.strSensorName.c_str()));
		}
		
		// ƴ���������������
		m_setChooseSensorID.insert(iter->second.nSensorID);
		if (iter->second.SensorRangeInfoArray.size() > 0)
		{
			nIndex = m_ListChoosedSensor.AddString(str);
			str = iter->second.SensorRangeInfoArray[0].strRangeName.c_str();
			int nCharPos = str.Find('(');
			str = str.Left(nCharPos);
			str = _T("���̣�") + str;
			
			m_ListChoosedSensor.SetItemDescription(nIndex, str);
			// ��sensor ID��Ϊitem Data
			m_ListChoosedSensor.SetItemData(nIndex, iter->second.nSensorID);

			m_ListChoosedSensor.SetItemImage(nIndex, iter->second.nSensorID);
		}
	}
}


void CDlgSensorChoose::OnBnClickedBtnDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_ListChoosedSensor.GetCurSel();
	if (-1 != nIndex)
	{
		nIndex = (int)m_ListChoosedSensor.GetItemData(nIndex);
		if (m_setChooseSensorID.lower_bound(nIndex) != m_setChooseSensorID.end())
		{
			m_setChooseSensorID.erase(m_setChooseSensorID.lower_bound(nIndex));
		}
		
		m_ListChoosedSensor.DeleteString(m_ListChoosedSensor.GetCurSel());
	}
}


void CDlgSensorChoose::OnBnClickedBtnDeleteAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_mapChooseSensor.clear();
	m_ListChoosedSensor.CleanUp();
	m_setChooseSensorID.clear();
}


void CDlgSensorChoose::OnBnClickedBtnChooseConnected()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDlgSensorChoose::OnBnClickedBtnOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDlgSensorChoose::OnCbnSelchangeCmbSensorType()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	RefreshSensorList();
}


void CDlgSensorChoose::OnLbnSelchangeSelectedSensorList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	RefreshRange();
}

// ˢ�±��
void CDlgSensorChoose::RefreshSensorList()
{
	m_ListSensor.CleanUp();
	m_mapCurrentSensor.clear();

	m_ListSensor.EnableItemDescription(TRUE, 1);
	m_ListSensor.SetImageList(IDB_SENSOR_ICON_LIST, 48);
	std::vector<SENSOR_CONFIG_ELEMENT> vecSensorList;
	int nSensorType = m_CmbSensorType.GetCurSel();
	if (-1 == nSensorType)
	{
		return;
	}

	nSensorType = (int)m_CmbSensorType.GetItemData(nSensorType);
	SENSOR_TYPE type;
	if (1 == nSensorType)
	{
		type = SENSOR_PHYSICS;
	} 
	else if (2 == nSensorType)
	{
		type = SENSOR_CHEMISTRY;
	}
	else if (4 == nSensorType)
	{
		type = SENSOR_BIOLOGY;
	}
	else
	{
		type = SENSOR_ALL;
	}

	CSensorConfig::CreateInstance().GetSensorList(vecSensorList, type);
	BOOST_FOREACH(auto &sensor, vecSensorList)
	{
		CString str(sensor.strSensorName.c_str());
		int nIndex = m_ListSensor.AddString(str);
		str = sensor.strSensorModelName.c_str();
		m_ListSensor.SetItemDescription(nIndex, str);
		m_ListSensor.SetItemData(nIndex, sensor.nSensorID);
		m_ListSensor.SetItemImage(nIndex, sensor.nSensorID);
		m_mapCurrentSensor[sensor.nSensorID] = sensor;
	}
}

void CDlgSensorChoose::RefreshRange()
{
	int nIndex = m_ListChoosedSensor.GetCurSel();
	if ( -1 != nIndex)
	{
		nIndex = (int)m_ListChoosedSensor.GetItemData(nIndex);
		const SENSOR_CONFIG_ELEMENT &sensor = CSensorConfig::CreateInstance().GetSensorInfo(nIndex);
		if (-1 != sensor.nSensorID)
		{
			m_CmbRange.ResetContent();
			m_CmbRange.ShowWindow(TRUE);
			if(sensor.SensorRangeInfoArray.size() == 1)
			{
				m_CmbRange.EnableWindow(FALSE);
			}
			else
			{
				m_CmbRange.EnableWindow(TRUE);
			}

			CString str;
			int nIndex = 0;
			for (int n = 0; n < (int)sensor.SensorRangeInfoArray.size(); ++n)
			{
				str = sensor.SensorRangeInfoArray[n].strRangeName.c_str();
				nIndex = str.Find('(');
				str.Left(nIndex);
				nIndex = m_CmbRange.AddString(str);
				m_CmbRange.SetItemData(nIndex, n);

				str = sensor.SensorRangeInfoArray[n].strRangeName.c_str();
				nIndex = str.Find('(');
				str = str.Right(str.GetLength() - nIndex);
				str.Remove('(');
				str.Remove(')');
				m_StaticResolutionRatio.SetWindowText(str);
			}

			m_CmbRange.SetCurSel(0);
		}
		else
		{
			m_CmbRange.ShowWindow(FALSE);
			m_StaticResolutionRatio.SetWindowText(_T(""));
		}
	}
}

void CDlgSensorChoose::OnCbnSelchangeCmbRange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nCurSel = m_ListChoosedSensor.GetCurSel();

	SENSOR_CONFIG_ELEMENT &sensor = CSensorConfig::CreateInstance().GetSensorInfo((int)m_ListChoosedSensor.GetItemData(nCurSel));
	if (-1 != sensor.nSensorID)
	{
		nCurSel = m_CmbRange.GetCurSel();
		int nIndex = (int)m_CmbRange.GetItemData(nCurSel);
		// �޸ĵ�ǰѡ���������Ϣ
		sensor.nCurRangeIndex = nIndex;
		if (nIndex < (int)sensor.SensorRangeInfoArray.size())
		{
			// �޸ķֱ�����Ϣ
			CString str (sensor.SensorRangeInfoArray[nIndex].strRangeName.c_str());
			nCurSel = str.Find('(');
			str = str.Right(str.GetLength() - nCurSel);
			str.Remove('(');
			str.Remove(')');
			m_StaticResolutionRatio.SetWindowText(str);

			// �޸���ѡ��Ĵ������Ϸ���ʾ��������Ϣ
			str = sensor.SensorRangeInfoArray[nIndex].strRangeName.c_str();
			int nCharPos = str.Find('(');
			str = str.Left(nCharPos);
			str = _T("���̣�") + str;
			m_ListChoosedSensor.SetItemDescription(m_ListChoosedSensor.GetCurSel(), str);
			m_ListChoosedSensor.Invalidate(TRUE);

			return;
		}
	}

	m_StaticResolutionRatio.SetWindowText(_T(""));
	m_CmbRange.EnableWindow(FALSE);
}
