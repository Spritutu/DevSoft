// DlgAcquirationPara.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgAcquirationPara.h"
#pragma warning(disable:4267)


// CDlgAcquirationPara �Ի���

const int MAX_SAMPLE_NUM = 26000;

IMPLEMENT_DYNAMIC(CDlgAcquirationPara, CBaseDialog)

CDlgAcquirationPara::CDlgAcquirationPara(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAcquirationPara::IDD, pParent)
	, m_nGroupRadioValue(0)
{
}

CDlgAcquirationPara::~CDlgAcquirationPara()
{
}

void CDlgAcquirationPara::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_FIXED_FREQUENCY, m_nGroupRadioValue);
	DDX_Control(pDX, IDC_STATIC_FREQUENCY, m_staticFrequency);
	DDX_Control(pDX, IDC_SCRBAR_FREQUENCY, m_barFrequency);
	DDX_Control(pDX, IDC_CHECK_LIMIT_TIME, m_checkLimitTime);
	DDX_Control(pDX, IDC_STATIC_LIMIT_TIME, m_staticLimitTime);
	DDX_Control(pDX, IDC_SCRBAR_LIMIT_TIME, m_barLimitTime);
	DDX_Control(pDX, IDC_CHECK_SAMPLE_START, m_checkSampleAtStart);
	DDX_Control(pDX, IDC_CHECK_MULTI_SAMPLE, m_checkMultiSample);
	DDX_Control(pDX, IDC_STATIC_MULTI_SAMPLE, m_staticMultiSample);
	DDX_Control(pDX, IDC_STATIC_SAM_NUM, m_staticSampleNum);
	DDX_Control(pDX, IDC_STATIC_WARNING, m_staticWarningInfo);
}


BEGIN_MESSAGE_MAP(CDlgAcquirationPara, CBaseDialog)
	ON_BN_CLICKED(IDC_RADIO_FIXED_FREQUENCY, &CDlgAcquirationPara::OnBnClickedRadioFixedFrequency)
	ON_BN_CLICKED(IDC_CHECK_SAMPLE_START, &CDlgAcquirationPara::OnBnClickedCheckSampleStart)
	ON_BN_CLICKED(IDC_CHECK_MULTI_SAMPLE, &CDlgAcquirationPara::OnBnClickedCheckMultiSample)
	ON_BN_CLICKED(IDC_CHECK_LIMIT_TIME, &CDlgAcquirationPara::OnBnClickedCheckLimitTime)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDlgAcquirationPara ��Ϣ�������


void CDlgAcquirationPara::OnBnClickedRadioFixedFrequency()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	switch (m_nGroupRadioValue)
	{
		// ��Ƶ�ʼ�¼
		case 0:
			
			break;
		// ������¼
		case 1:
				
			break;
		// �ֶ���¼
		case 2:

			break;
		// ����Ŵ���
		case 3:

			break;
	    // ר��ʵ��
		case 4:

			break;
		// δ֪ѡ��
		default:
			break;
	}
}


void CDlgAcquirationPara::InitCtrls()
{
	// Ƶ�ʷ�Χ��������36��
	InitFrequencyMap();
	m_barFrequency.SetScrollRange(0, m_mapFrequency.size() - 1, TRUE);

	//�޶�ʱ��ѡ��������� 45��
	InitLimitTimeMap();
	m_barLimitTime.SetScrollRange(0, m_mapLimitTime.size() - 1, TRUE);

	m_barLimitTime.EnableWindow(FALSE);
	m_staticSampleNum.SetWindowTextW(_T("26000"));
}

void CDlgAcquirationPara::InitFrequencyMap()
{
	// Ƶ��ѡ������� ��λ����/��
	m_mapFrequency[0] = 0.001f;
	m_mapFrequency[1] = 0.002f;
	m_mapFrequency[2] = 0.004f;
    m_mapFrequency[3] = 0.005f;
	m_mapFrequency[4] = 0.008f;

	m_mapFrequency[5] = 0.01f;
	m_mapFrequency[6] = 0.02f;
	m_mapFrequency[7] = 0.04f;
	m_mapFrequency[8] = 0.05f;
	m_mapFrequency[9] = 0.08f;

	m_mapFrequency[10] = 0.1f;
	m_mapFrequency[11] = 0.2f;
	m_mapFrequency[12] = 0.4f;
	m_mapFrequency[13] = 0.5f;
	m_mapFrequency[14] = 0.8f;

	m_mapFrequency[15] = 1.0f;
	m_mapFrequency[16] = 2.0f;
	m_mapFrequency[17] = 4.0f;
	m_mapFrequency[18] = 5.0f;
	m_mapFrequency[19] = 8.0f;
	m_mapFrequency[20] = 10.0f;

	m_mapFrequency[21] = 20.0f;
	m_mapFrequency[22] = 40.0f;
	m_mapFrequency[23] = 50.0f;
	m_mapFrequency[24] = 100.0f;
	m_mapFrequency[25] = 200.0f;
	m_mapFrequency[26] = 500.0f;
	m_mapFrequency[27] = 1000.0f;
	m_mapFrequency[28] = 2500.0f;
	m_mapFrequency[29] = 3125.0f;
	m_mapFrequency[30] = 5000.0f;
	m_mapFrequency[31] = 6250.0f;
	m_mapFrequency[32] = 12500.0f;
	m_mapFrequency[33] = 25000.0f;
	m_mapFrequency[34] = 50000.0f;
	m_mapFrequency[35] = 100000.0f;
}

void CDlgAcquirationPara::InitLimitTimeMap()
{
	// �޶�ʱ������� ��λ���루s��
	m_mapLimitTime[0] = 0.1f;
	m_mapLimitTime[1] = 0.5f;
	m_mapLimitTime[2] = 1.0f;
	m_mapLimitTime[3] = 2.0f;
	m_mapLimitTime[4] = 3.0f;
	m_mapLimitTime[5] = 4.0f;
	m_mapLimitTime[6] = 5.0f;
	m_mapLimitTime[7] = 6.0f;
	m_mapLimitTime[8] = 7.0f;
	m_mapLimitTime[9] = 8.0f;
	m_mapLimitTime[10] = 9.0f;
	m_mapLimitTime[11] = 10.0f;
	m_mapLimitTime[12] = 15.0f;
	m_mapLimitTime[13] = 20.0f;
	m_mapLimitTime[14] = 25.0f;
    m_mapLimitTime[15] = 30.0f;
	m_mapLimitTime[16] = 35.0f;
	m_mapLimitTime[17] = 40.0f;
	m_mapLimitTime[18] = 45.0f;
	m_mapLimitTime[19] = 50.0f;
	m_mapLimitTime[20] = 60.0f;
	m_mapLimitTime[21] = 70.0f;
	m_mapLimitTime[22] = 80.0f;
	m_mapLimitTime[23] = 90.0f;
	m_mapLimitTime[24] = 100.0f;
	m_mapLimitTime[25] = 120.0f;
	m_mapLimitTime[26] = 180.0f;
	m_mapLimitTime[27] = 240.0f;
	m_mapLimitTime[28] = 300.0f;
	m_mapLimitTime[29] = 360.0f;
	m_mapLimitTime[30] = 420.0f;
	m_mapLimitTime[31] = 480.0f;
	m_mapLimitTime[32] = 540.0f;
	m_mapLimitTime[33] = 600.0f;
	m_mapLimitTime[34] = 720.0f;
	m_mapLimitTime[35] = 840.0f;
	m_mapLimitTime[36] = 960.0f;
	m_mapLimitTime[37] = 1080.0f;
	m_mapLimitTime[38] = 1200.0f;
	m_mapLimitTime[39] = 1500.0f;
	m_mapLimitTime[40] = 1800.0f;
	m_mapLimitTime[41] = 2700.0f;
	m_mapLimitTime[42] = 3600.0f;
	m_mapLimitTime[43] = 5400.0f;
	m_mapLimitTime[44] = 7200.0f;
}

int CDlgAcquirationPara::CalSampleNum()
{
	using namespace std;
	if (BST_CHECKED != m_checkLimitTime.GetCheck())
	{
		return MAX_SAMPLE_NUM;
	}

	float fFrequency = 1.0f;
	float fTime = 1.0f;
	map<int, float>::iterator iter = m_mapFrequency.find(m_barFrequency.GetScrollPos());
	if(m_mapFrequency.end() != iter)
	{
		fFrequency = iter->second;
	}

	iter = m_mapLimitTime.find(m_barLimitTime.GetScrollPos());
	if (m_mapLimitTime.end() != iter)
	{
		fTime = iter->second;
	}

	// С��1ʱǿ����Ϊ1�����ǡ���0ʱ��������ѡʱ
	int nSampleNum = (int)(fTime*fFrequency);
	if (nSampleNum < 1)
	{
		nSampleNum = 1;
	}
	else if (BST_CHECKED == m_checkSampleAtStart.GetCheck())
	{
		nSampleNum++;
	}

	return nSampleNum;
}

void CDlgAcquirationPara::RefreshCtrl()
{
	using namespace std;
	int nSampleNum = CalSampleNum();
	CString strText;
	// ������Ŀ
	if (nSampleNum > MAX_SAMPLE_NUM)
	{
		strText.Format(_T("%d(>%d)"), nSampleNum, MAX_SAMPLE_NUM);
	}
	else
	{
		strText.Format(_T("%d"), nSampleNum);
	}
	m_staticSampleNum,SetWindowTextW(strText);

	// �����
	map<int, float>::iterator iter = m_mapFrequency.find(m_barFrequency.GetScrollPos());
	strText = _T("");
	if (iter != m_mapFrequency.end())
	{
		if (BST_CHECKED == m_checkMultiSample.GetCheck())
		{
			strText.Format(_T("ÿ��%d����ƽ��ֵ"), GetAverageSampleNum(iter->second));
		}
	}
	m_staticMultiSample.SetWindowText(strText);

	// ���·�������Ϣ
	strText = _T("");
	if (nSampleNum > MAX_SAMPLE_NUM)
	{
		strText = _T("������Ŀ�Ѿ�����ϵͳ�����26000���㣬���ڳ�������ʱ���Զ�ֹͣʵ�顣");
	}
	m_staticWarningInfo.SetWindowText(strText);
}

int CDlgAcquirationPara::GetAverageSampleNum(float fFrequency)
{
	if (fFrequency <= 4.0f)
	{
		return 50;
	}
	else if (fFrequency <= 5.0f)
	{
		return 40;
	}
	else if (fFrequency <= 8.0f)
	{
		return 25;
	}
	else if (fFrequency <= 10.0f)
	{
		return 20;
	}
	else if (fFrequency <= 20.0f)
	{
		return 10;
	}
	else if (fFrequency <= 40.0f)
	{
		return 5;
	}
	else if (fFrequency <= 50.0f)
	{
		return 4;
	}
	else if (fFrequency <= 100.0f)
	{
		return 2;
	}
	else 
	{
		return 1;
	}
}

void CDlgAcquirationPara::OnBnClickedCheckSampleStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	RefreshCtrl();
}


void CDlgAcquirationPara::OnBnClickedCheckMultiSample()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	RefreshCtrl();
}


void CDlgAcquirationPara::OnBnClickedCheckLimitTime()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (BST_CHECKED == m_checkLimitTime.GetCheck())
	{
		m_barLimitTime.EnableWindow(TRUE);
	}
	else
	{
		m_barLimitTime.EnableWindow(FALSE);
	}
}


BOOL CDlgAcquirationPara::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgAcquirationPara::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int pos = pScrollBar->GetScrollPos();    // ��ȡˮƽ��������ǰλ��   

	switch (nSBCode)    
	{    
		// ����������һ�У���pos��1   
	case SB_LINEUP:    
		pos -= 1;    
		break;    
		// ������ҹ���һ�У���pos��1   
	case SB_LINEDOWN:    
		pos  += 1;    
		break;    
		// ����������һҳ����pos��10   
	case SB_PAGEUP:    
		pos -= 10;    
		break;    
		// ������ҹ���һҳ����pos��10   
	case SB_PAGEDOWN:    
		pos  += 10;    
		break;    
		// �������������ˣ���posΪ1   
	case SB_TOP:    
		pos = 1;    
		break;    
		// ������������Ҷˣ���posΪ100   
	case SB_BOTTOM:    
		pos = 100;    
		break;      
		// ����϶������������ָ��λ�ã���pos��ֵΪnPos��ֵ   
	case SB_THUMBPOSITION:    
		pos = nPos;    
		break;    
		// �����m_horiScrollbar.SetScrollPos(pos);ִ��ʱ��ڶ��ν���˺���������ȷ��������λ�ã����һ�ֱ�ӵ�default��֧�������ڴ˴����ñ༭������ʾ��ֵ   
	default:     
		return;    
	}    

	// ���ù�����λ��   
	pScrollBar->SetScrollPos(pos);   

	using namespace std;
    CString strStatic;
	map<int, float>::iterator iter = m_mapFrequency.find(m_barFrequency.GetScrollPos());
	if (iter != m_mapFrequency.end())
	{

		strStatic.Format(_T("%f��/�룬��%f��/��"), iter->second, 1.0f/iter->second);
		m_staticFrequency.SetWindowText(strStatic);

		strStatic = _T("");
		if (BST_CHECKED == m_checkMultiSample.GetCheck())
		{
			strStatic.Format(_T("ÿ��%d����ƽ��ֵ"), GetAverageSampleNum(iter->second));
		}

		m_staticMultiSample.SetWindowText(strStatic);
	}

	strStatic = _T("");
	if(m_checkLimitTime.GetCheck())
	{
		/*map<int, float>::iterator*/ iter = m_mapLimitTime.find(m_barLimitTime.GetScrollPos());
		if (m_mapLimitTime.end() != iter)
		{
			// ����ʱ�䵥λ
			if (iter->second < 1.0f)
			{
				strStatic.Format(_T("%f����"), iter->second * 100);
			}
			else if (iter->second < 101.0f)
			{
				strStatic.Format(_T("%f��"), iter->second);
			}
			else if (iter->second < 5401.0f )
			{
				strStatic.Format(_T("%f��"), iter->second/60.0f);
			}
			else
			{
				strStatic.Format(_T("%fСʱ"), iter->second/3600.0f);
			}
		}
	}

	m_staticLimitTime.SetWindowText(strStatic);

	RefreshCtrl();

	CBaseDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
