#pragma once

#include "BaseDialog.h"
#include "WidgetLayout.h"
//#include "ChartManager.h"
// CDlgDiagramPanel �Ի���

class CDlgDiagramPanel : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgDiagramPanel)

public:
	CDlgDiagramPanel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDiagramPanel();

// �Ի�������
	enum { IDD = IDD_DLG_DIAGRAM };
	void addPanel();
	void delPanel(CWnd* pDlg);

	//��ȡGrid�ĸ���
	int GetWidgetNum(void) const;
private:
	//��������
	//ChartManager m_dataManager;
	//������
	CWidgetLayout m_WidgetLayout;
	//����
	std::vector<CBaseDialog*> m_vecPanel;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	//�������
	void CreatePanel(void);
	//�������
	void DestroyPanel(void);
	//�������λ��
	void AdjustPanelLayout(int nWidth,int nHeight);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
