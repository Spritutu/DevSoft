#pragma once

#include "BaseDialog.h"
// CSplitBar �Ի���

class CSplitBar : public CBaseDialog
{
	DECLARE_DYNAMIC(CSplitBar)

public:
	CSplitBar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSplitBar();

// �Ի�������
	enum { IDD = IDD_DLG_SPLI_BAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	void SetSplitBarBkColor(COLORREF BkColor);

private:
	//����ɫ
	COLORREF m_BkColor;
};
