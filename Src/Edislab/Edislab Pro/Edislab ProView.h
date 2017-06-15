
// Edislab ProView.h : CEdislabProView ��Ľӿ�
//

#pragma once
#include <string>
#include "ConcreteTabWnd.h"

class CDlgTabPanel;
class CEdislabProDoc;
class CEdislabProView : public CView
{
protected: // �������л�����
	CEdislabProView();
	DECLARE_DYNCREATE(CEdislabProView)


// ����
public:
	CEdislabProDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CEdislabProView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	void CreateDisplayTabWnd(void);


protected:
	CConcreteTabWnd m_TabWnd;
public:
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();

	afx_msg void HandleAllRibbonCommand(UINT nCommand);
	afx_msg void HandleUpdateAllRibbonCommand(CCmdUI *pCmdUI);

public:
	//�����µ�ҳ
	bool AddNewTabWnd(const CString& strTabWndName,int nGridNum,int nDiagramNum,int nDeviceNum);
	//��ȡ��ǰ�ҳ��ָ��
	CDlgTabPanel* GetCurrentPage(void);
	//��ȡ��ǰ�ҳ������
	CString GetCurrentPageName(void);
	//���õ�ǰ�ҳ������
	void SetCurrentPageName(const CString& strPageName);
	//��ȡҳ����
	int GetPageNum(void);
	//�������һҳΪ�ҳ
	void SetActivePage(int nPageIndex);
	//ɾ����ǰҳ
	void DeleteCurrentPage(void);
	//��ȡ��ĸ���
	int GetGridNum(void);
	//��ȡ�Ǳ�ĸ���
	int GetDeviceNum(void);
	//��ȡͼ�ĸ���
	int GetDiagramNum(void);

	void AddGrid(void);

	void AddDiagram(void);

	void AddDevice(void);
public:
	static CEdislabProView* GetCurrentView(void);



	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();

	//֪ͨˢ������Tabҳ�е�grid��Ϣ
	void NotifyDetectDevice(const std::string& strDeviceName,int nOnFlag);

	//֪ͨgridˢ����
	void NotifyGridChangeRows(int nRows);

	//֪ͨ��Ӧ�Ŀؼ���ʼˢ��
	void NotifyControlsStartRefresh();

	//֪ͨ��Ӧ�Ŀؼ�ֹͣˢ��
	void NotifyControlsStopRefresh();

	//ɾ��Ԫ��
	void DeleteElement();

	//֪ͨGrid�ķ�����Ϣ�ı�
	void NotifyGridGroupInfoChange();
};

#ifndef _DEBUG  // Edislab ProView.cpp �еĵ��԰汾
inline CEdislabProDoc* CEdislabProView::GetDocument() const
   { return reinterpret_cast<CEdislabProDoc*>(m_pDocument); }
#endif

