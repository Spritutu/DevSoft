#pragma once



// CDeviceView ������ͼ

class CDeviceView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CDeviceView)

protected:
	CDeviceView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDeviceView();

public:
	enum { IDD = IDD_DLG_DEVICE };
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


