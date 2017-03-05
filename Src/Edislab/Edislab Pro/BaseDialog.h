/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:BaseDialog.h
Description:Dialog�Ļ���,���жԻ��򶼴Ӵ�����
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#pragma once


class CBaseDialog : public CBCGPDialog
{
	DECLARE_DYNAMIC(CBaseDialog)
public:
	CBaseDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL,BOOL bEnableLayout = FALSE);
	~CBaseDialog(void);

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

private:
	bool m_bActive;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

