
// Edislab Pro.h : Edislab Pro Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CEdislabProApp:
// �йش����ʵ�֣������ Edislab Pro.cpp
//

class CEdislabProApp : public CBCGPWinApp
{
public:
	CEdislabProApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	//��ʼ��׼��
	void Init(void);
};

extern CEdislabProApp theApp;
