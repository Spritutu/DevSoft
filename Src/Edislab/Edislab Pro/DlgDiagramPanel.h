#pragma once
#include "BaseDialog.h"
#include "WidgetLayout.h"
#include "Type.h"
// CDlgDiagramPanel �Ի���
class ChartFigureDlg;
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
	//������
	CWidgetLayout m_WidgetLayout;
	//����
	std::vector<ChartFigureDlg*> m_vecPanel;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	//�������λ��
	void AdjustPanelLayout(int nWidth,int nHeight);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//�޸���ʾ�������ĸı�
	void NotifyDisplayPanelChange(const LP_SENSOR_TYPE_KEY pSensor,int nFlag);
	//֪ͨ��Ӧ�Ŀؼ���ʼˢ��
	void NotifyControlsStartRefresh();
	//֪ͨ��Ӧ�Ŀؼ�ֹͣˢ��
	void NotifyControlsStopRefresh();
};
