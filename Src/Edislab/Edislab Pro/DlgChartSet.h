#pragma once

// DlgChartSet dialog
#include "ChartXY.h"
class DlgChartSet : public CDialog
{
	DECLARE_DYNAMIC(DlgChartSet)

public:
	DlgChartSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgChartSet();

// Dialog Data
	enum { IDD = IDD_DLG_CHART_SET };
	int m_nXID;//x��ID
	std::set<int> m_setShowID;//��ʾ��ID
	ChartType	m_eChartType;//��ʾ��ʽ
	LineStyle	m_eLineStyle;//�ߵķ�ʽ
	MoveStyle	m_eMoveStyle;//ʵ��ʱ�ķ�ʽ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_combX;
	CBCGPTreeCtrl m_treeY;
	CComboBox m_combMoveStyle;
	CComboBox m_combChartType;
	CComboBox m_combLineStyle;
	afx_msg void OnBnClickedOk();
};
