#pragma once
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/atomic.hpp>
#include "CustomGrid.h"
// CDlgGridContainer �Ի���

class CDlgGridContainer : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgGridContainer)

public:
	CDlgGridContainer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgGridContainer();

// �Ի�������
	enum { IDD = IDD_DLG_GRID_CONTAINER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CCustomGrid m_DisplayGrid;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	void NotifyDetectSensor(const std::string& strDeviceName,int nOnFlag);
private:
	void RefreshGrid(void);
public:
	afx_msg void OnPaint();

private:
	//ģ��������ݵ��߳�
	boost::shared_ptr<boost::thread> m_pYieldDataThread;
	boost::atomic_bool m_bLoop;
	void YieldDataProc(void);
};
