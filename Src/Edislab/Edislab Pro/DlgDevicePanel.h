#pragma once
#include "BaseDialog.h"
#include "WidgetLayout.h"
#include "GaugeManager.h"
// CDlgDevicePanel �Ի���

class CDlgDevicePanel : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgDevicePanel)

public:
	CDlgDevicePanel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDevicePanel();

// �Ի�������
	enum { IDD = IDD_DLG_DEVICE };
	void addPanel();
	void delPanel(CWnd* pDlg);

	//��ȡGrid�ĸ���
	int GetWidgetNum(void) const;

private:
	//��������
	GaugeManager m_dataManager;
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
