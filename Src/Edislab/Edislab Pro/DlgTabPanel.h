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
	//����ҳ����
	void SetPageName(const CString& strPageName);
	//��ȡҳ����
	CString GetPageName(void) const;
	//��ȡGrid����
	int GetGridNum(void) const;
	//����Grid����
	void SetGridNum(int nGridNum);
	//��ȡDevice����
	int GetDeviceNum(void) const;
	//����Device����
	void SetDeviceNum(int nDeviceNum);
	//��ȡDiagram����
	int GetDiagramNum(void) const;
	//����Diagram����
	void SetDiagramNum(int DiagramNum);
	//��ӱ��
	void AddGrid();
	//���ͼ��
	void AddChart();
	//����Ǳ�
	void AddGauge();
	//֪ͨ�����´�����
	void NotifyDectectSensor(const std::string& strDeviceName,int nOnFlag);
	//֪ͨ�����ı�
	void NotifyGridChangeRows(int nRows);
	//֪ͨ��Ӧ�Ŀؼ���ʼˢ��
	void NotifyControlsStartRefresh();
	//֪ͨ��Ӧ�Ŀؼ�ֹͣˢ��
	void NotifyControlsStopRefresh();
	//��ȡ��ǰ��Ĵ���
	CWnd* GetActiveDlg();
	//���õ�ǰ�����
	void SetActive(int nType,CWnd* pActiveWnd);
	void DelWnd(void);
	//׼����ʾԪ��
	void PrepareDisplayElement(void);
	//֪ͨGrid�ķ�����Ϣ�ı�
	void NotifyGridGroupInfoChange();
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

	//��ǰ�����
	CWnd* m_pActiveDlg;

	//����ڵ�����
	int m_nActiveWndType;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void PostNcDestroy();
	afx_msg void OnPaint();
};
