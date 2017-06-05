#pragma once
#include "BaseDialog.h"
#include "GridGroupType.h"
class CDlgGridOpt : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgGridOpt)

public:
	CDlgGridOpt(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgGridOpt();

// �Ի�������
	enum { IDD = IDD_DLG_GRID_OPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CBCGPTreeCtrlEx m_GridDisplayItemTree;

private:
	//��ʼ����ʾ
	void InitDisplay(void);
public:
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnBnClickedBtnCancel();

	//������ʾѡ��
	void SetDisplayInfo(const std::vector<SHOW_COLUMN_GROUP_INFO>& GridDisplayArray);
	//��ȡ��ʾѡ��
	void GetDisplayInfo(std::vector<SHOW_COLUMN_GROUP_INFO>& GridDisplayArray);
private:
	std::vector<SHOW_COLUMN_GROUP_INFO> m_GridDisplayArray;
};
