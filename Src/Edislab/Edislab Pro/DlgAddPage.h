#pragma once
#include "BaseDialog.h"
#include "Type.h"
class CDlgAddPage : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgAddPage)

public:
	CDlgAddPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAddPage();

// �Ի�������
	enum { IDD = IDD_DLG_ADD_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	ADD_PAGE_ELEMENT GetAddPageElement(void) const;

private:
	void InitDisplay(void);
	CBCGPEdit m_PageNameEdit;
	CComboBox m_GridCombox;
	CComboBox m_DiagramCombox;
	CComboBox m_DeviceCombox;
public:
	afx_msg void OnBnClickedRadioEmptyPage();
	afx_msg void OnBnClickedRadioCopyPage();
	afx_msg void OnBnClickedRadioCustomPage();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();

private:
	//��������
	bool HandleAddPage(void);

	//���������հ�ҳ
	bool HandleAddEmptyPage(const CString& strPageName);

	//�����������Ƶ�ǰҳ
	bool HandleAddCopyPage(const CString& strPageName);

	//���������Զ���ҳ
	bool HandleCustomPage(const CString& strPageName);

private:
	ADD_PAGE_ELEMENT m_AddPageElement;
};
