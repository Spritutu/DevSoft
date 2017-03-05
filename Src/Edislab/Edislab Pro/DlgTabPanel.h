#pragma once


// CDlgTabPanel �Ի���
#include "WidgetLayout.h"
class CBaseDialog;
class CDlgTabPanel : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgTabPanel)

public:
	CDlgTabPanel(const CString&strPageName,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTabPanel();

	static const int PANEL_NUM = 3;
// �Ի�������
	enum { IDD = IDD_DLG_PANEL };

	enum 
	{
		GRID_INDEX = 0,
		DEVICE_INDEX,
		DIAGRAM_INDEX
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void SetPageName(const CString& strPageName);

	CString GetPageName(void) const;

	//��ȡGrid����
	int GetGridNum(void) const;

	void SetGridNum(int nGridNum);

	//��ȡDevice����
	int GetDeviceNum(void) const;

	void SetDeviceNum(int nDeviceNum);

	//��ȡDiagram����
	int GetDiagramNum(void) const;

	void SetDiagramNum(int DiagramNum);

	//��ӱ��
	void AddGrid();
	//���ͼ��
	void AddChart();
	//����Ǳ�
	void AddGauge();
	//ɾ������
	void DelWnd(CWnd* dlg);
	void SetActive(int type);
	//
	void SetActiveDlg(CWnd* dlg);
	CWnd* GetActiveDlg();
	//׼����ʾԪ��
	void PrepareDisplayElement(void);
protected:
	//�������
	void CreatePanel(void);

	//�������
	void DestroyPanel(void);

	//�������λ��
	void AdjustPanelLayout(int nWidth,int nHeight);

private:
	//���
	CBaseDialog* m_pPanel[PANEL_NUM];

	//������
	CWidgetLayout m_WidgetLayout;

	//ҳ������
	CString m_strPageName;

	//Grid������
	int m_nGridNum;

	//Device������
	int m_nDeviceNum;

	//Diagram������
	int m_nDiagramNum;

	class CWnd* m_pActiveDlg;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void PostNcDestroy();
};
