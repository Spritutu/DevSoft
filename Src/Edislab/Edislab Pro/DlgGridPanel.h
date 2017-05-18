#pragma once

#include "BaseDialog.h"
#include "WidgetLayout.h"
// CDlgGridPanel �Ի���

class CDlgGridPanel : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgGridPanel)

public:
	CDlgGridPanel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgGridPanel();

// �Ի�������
	enum { IDD = IDD_DLG_GRID };

	void addPanel();
	void delPanel(CWnd* pDlg);

	//��ȡGrid�ĸ���
	int GetWidgetNum(void) const;

private:
	//������
	CWidgetLayout m_WidgetLayout;
	//����
	std::vector<CWnd*> m_vecPanel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
protected:
#if 0
	//�������
	void CreatePanel(void);
	//�������
	void DestroyPanel(void);
#endif
	//�������λ��
	void AdjustPanelLayout(int nWidth,int nHeight);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
