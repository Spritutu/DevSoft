#pragma once


// CDlgColumnDisplay �Ի���
#include "BaseDialog.h"
class CDlgColumnDisplay : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgColumnDisplay)

public:
	CDlgColumnDisplay(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgColumnDisplay();

// �Ի�������
	enum { IDD = IDD_DLG_COLUMN_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
