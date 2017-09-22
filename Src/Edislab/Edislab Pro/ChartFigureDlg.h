#pragma once
#include "BaseDialog.h"
#include "ChartXY.h"
#include "Type.h"
// ChartFigureDlg �Ի���
class ChartFigureDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(ChartFigureDlg)

public:
	ChartFigureDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ChartFigureDlg();

// �Ի�������
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CHARTFIGURE };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
public:
	ChartXY* m_charxy;
	CPoint m_ptContext;
	//������ʾ��������ID
	void SetXAxisSensorID(const SENSOR_TYPE_KEY& XAxisID);
	void SetYAxisSensorID(const SENSOR_TYPE_KEY& YAxisID);
	void RestYAxisSensorID(void);
	bool IsYAxisSensorIDExist(const SENSOR_TYPE_KEY& YAxisID) const;
	SENSOR_TYPE_KEY GetXAxisSensorID(void);
	//��������
	int saveData()const;
	//��ȡ����
	int readData();
	void updateData() const;
	void ChartSet();
	//֪ͨ��Ӧ�Ŀؼ���ʼˢ��
	void NotifyControlsStartRefresh();
	//֪ͨ��Ӧ�Ŀؼ�ֹͣˢ��
	void NotifyControlsStopRefresh();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnChartSet();
	afx_msg void OnChartDel();
	afx_msg void OnChartZoomIn();
	afx_msg void OnChartZoomOut();
	afx_msg void OnChartShowAll();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//begin add by xiaowei.han
	afx_msg LRESULT NotifyActive(WPARAM wp,LPARAM lp);
private:
	BOOL m_bActiveFlag;
	//end add by xiaowei.han
};
