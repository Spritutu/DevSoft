#pragma once


// CDlgAddColumn �Ի���
#include "BaseDialog.h"
#include "ConcreteTabWnd.h"
class CDlgAddColumn : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgAddColumn)

public:
	CDlgAddColumn(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAddColumn();

// �Ի�������
	enum { IDD = IDD_DLG_ADD_COLUMN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


private:
	void InitDisplay(void);

private:
	//Tab�ռ�
	CConcreteTabWnd m_TabWnd;
};
