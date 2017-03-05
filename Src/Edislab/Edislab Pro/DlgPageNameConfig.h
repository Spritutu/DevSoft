#pragma once

#include "BaseDialog.h"
#include "afxwin.h"
// CDlgPageNameConfig �Ի���

class CDlgPageNameConfig : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgPageNameConfig)

public:
	CDlgPageNameConfig(const CString& strPageName,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPageNameConfig();

// �Ի�������
	enum { IDD = IDD_DLG_PAGE_NAME_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CBCGPEdit m_EditPageName;

	CString m_strPageName;
public:
	virtual BOOL OnInitDialog();

private:
	void InitDisplay(void);
public:
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();

public:
	CString GetPageName(void) const;
};
