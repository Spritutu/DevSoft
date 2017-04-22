#pragma once
#include "BaseDialog.h"

// CDlgSensorChoose �Ի���

class CDlgSensorChoose : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgSensorChoose)

public:
	CDlgSensorChoose(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSensorChoose();

// �Ի�������
	enum { IDD = IDD_DLG_SENSOR_CHOOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	void InitCtrls();

private:
	// ��Ӱ�ť
	CButton m_BtnAdd;
	
	// ɾ����ť
	CButton m_BtnDel;

	// ȫɾ��ť
	CButton m_BtnDelAll;

	// ѡ�ѽӰ�ť
	CButton m_BtnChooseCon;

	// ���������������˵�
	CComboBox m_CmbSensorType;

	// �Զ�ʶ��
	CButton m_CheckAutoChoose;

	// ����
	CComboBox m_CmbRange;

	// �������б�
	CListBox m_ListSensor;
    
	// ��ѡ�������б�
	CListBox m_ListChoosedSensor;

	// �ֱ�����ֵ�����ִ������ķֱ��ʲ��������ı���ʾ�ģ����ı�����ṩλ����Ϣ��
	CStatic m_StaticResolutionRatio;
public:
	afx_msg void OnBnClickedCheckAutoRecognize();
};
