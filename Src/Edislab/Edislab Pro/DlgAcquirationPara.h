#pragma once
#include "BaseDialog.h"

// CDlgAcquirationPara �Ի���

class CDlgAcquirationPara : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgAcquirationPara)

public:
	CDlgAcquirationPara(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAcquirationPara();

// �Ի�������
	enum { IDD = IDD_DLG_ACQUISITION_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
