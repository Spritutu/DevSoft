#pragma once



// CGridView ������ͼ

class CGridView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CGridView)

protected:
	CGridView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CGridView();

public:
	enum { IDD = IDD_DLG_GRID };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


