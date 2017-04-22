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
	CBCGPButton m_BtnAdd;
	
	// ɾ����ť
	CBCGPButton m_BtnDel;

	// ȫɾ��ť
	CBCGPButton m_BtnDelAll;

	// ѡ�ѽӰ�ť
	CBCGPButton m_BtnChooseCon;

	// ���������������˵�
	CBCGPComboBox m_CmbSensorType;

	// �Զ�ʶ��
	CBCGPButton m_CheckAutoChoose;

	// ����
	CBCGPComboBox m_CmbRange;

	// �������б�
	CBCGPListBox m_ListSensor;
    
	// ��ѡ�������б�
	CBCGPListBox m_ListChoosedSensor;

	// �ֱ�����ֵ�����ִ������ķֱ��ʲ��������ı���ʾ�ģ����ı�����ṩλ����Ϣ��
	CBCGPStatic m_StaticResolutionRatio;
public:
	afx_msg void OnBnClickedCheckAutoRecognize();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnDeleteAll();
	afx_msg void OnBnClickedBtnChooseConnected();
	afx_msg void OnBnClickedBtnOk();
};
