#pragma once



// CDiagramView ������ͼ

class CDiagramView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CDiagramView)

protected:
	CDiagramView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDiagramView();

public:
	enum { IDD = IDD_DLG_DIAGRAM };
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


