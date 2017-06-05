#pragma once
#include "BaseDialog.h"
#include <map>
#include "Type.h"
#include "SensorConfig.h"

// CDlgAcquirationPara �Ի���

class CDlgAcquirationPara : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgAcquirationPara)

public:
	CDlgAcquirationPara(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAcquirationPara();

// �Ի�������
	enum { IDD = IDD_DLG_ACQUISITION_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	
	afx_msg void OnBnClickedRadioFixedFrequency();

private:
	void InitCtrls();

	void InitFrequencyMap();

	void InitLimitTimeMap();

	int CalSampleNum();

	void RefreshCtrl();

	int GetAverageSampleNum(float fFrequency);

private:
	// Radio Group�󶨵ı���
	int m_nGroupRadioValue;

	// Ƶ����ʾ
	CBCGPStatic m_staticFrequency;

	// Ƶ��ѡ�񻬶���
	/*  ��λ����/��
	  0.001��/�룬 0.002��  0.004��  0.005��  0.008��
	  0.01�� 0.02,  0.04,  0.05, 0.08,  0.1, 0.2,  0.4,  0.5,  0.8,
	  1,  2,  4,  5,  8,  10,  20,  40,  50,  100,  200,  500,  1000,
	  2500,  3125,  5000,  6250,  12500,   25000,  50000,  100000,
	*/
	CScrollBar m_barFrequency;

	// �Ƿ��޶�ʱ��
	CBCGPButton m_checkLimitTime;

	// ��ʾ�޶���ʱ��
	CBCGPStatic m_staticLimitTime;

	// �޶�ʱ��ѡ�������
	/* ��ֵѡ��Χ
	   100ms, 500ms,
	   1s, 2s, 3s, 4s, 5s, 6s, 7s, 8s, 9s, 10s,
	   15s, 20s, 25s, 30s, 35s, 40s, 45s, 50s, 
	   60s, 70s, 80s, 90s, 100s, 
	   2m, 3m, 4m, 5m, 6m, 7m, 8m, 9m, 10m, 
	   12m, 14m, 16m, 18m, 20m,
	   25m,
	   30m, 45m, 60m, 90m, 
	   2h
	*/
	CScrollBar m_barLimitTime;

	// �Ƿ�����ʱ����
	CBCGPButton m_checkSampleAtStart;

	// �����
	CBCGPButton m_checkMultiSample;

	// �������ʽ
	CBCGPStatic m_staticMultiSample;

	// ������Ŀ
	CBCGPStatic m_staticSampleNum;

	//��ʾ��Ϣ
	CBCGPStatic m_staticWarningInfo;

	// Ƶ�ʹ�������λ�����Ӧ����Ϣ
	std::map<int, float> m_mapFrequency;

	// �޶�ʱ���������λ�����Ӧ����Ϣ
	std::map<int, float> m_mapLimitTime;
public:
// 	afx_msg void OnNMThemeChangedScrbarFrequency(NMHDR *pNMHDR, LRESULT *pResult);
// 	afx_msg void OnNMThemeChangedScrbarLimitTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckSampleStart();
	afx_msg void OnBnClickedCheckMultiSample();
	afx_msg void OnBnClickedCheckLimitTime();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();
};
