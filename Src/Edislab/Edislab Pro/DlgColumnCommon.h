#pragma once


// CDlgColumnCommon �Ի���
#include "BaseDialog.h"
class CDlgColumnCommon : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgColumnCommon)

public:
	CDlgColumnCommon(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgColumnCommon();

// �Ի�������
	enum { IDD = IDD_DLG_COLUMN_COMMON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
