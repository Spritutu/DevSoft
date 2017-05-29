
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "BaseRibbonBar.h"
class CMainFrame : public CBCGPFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CBCGPMenuBar       m_wndMenuBar;
	CBCGPToolBar       m_wndToolBar;
	CBCGPStatusBar     m_wndStatusBar;
	CBCGPToolBarImages m_UserImages;
	//CBCGPRibbonStatusBar m_wndStatusBar;
protected:
	void CreateRibbonBar(void);
	//���ӿ�ʼCategory
	void AddStartCategory(void);
	//������չCategory
	void AddExternCategory(void);
	//����ҳ��Category
	void AddPageCategory(void);
	//�����Ǳ�Category
	void AddDeviceCategory(void);
	//���ӱ���category
	void AddGridCategory(void);
	//����ͼ���category
	void AddDiagramCategory(void);
	//��Ӵ����������ݵ����
	void AddSensorDataPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//���ҳ��Ԫ�����
	void AddStartPageElementPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//���ҳ��Ԫ�����
	void AddPageElementPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//��ӽ�������
	void AddRunTestPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//��ӻط�����
	void AddReplyTestPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//�������������
	void AddOtherSoftWarePanel(CBCGPRibbonCategory* pCategory);
	//���ҳ�����
	void AddPagePanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//����ѡ�����
	void AddOptionPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//����У׼���
	void AddFixedPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//���ӱ��ѡ�����
	void AddGridOptionPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//���Ӽ��������
	void AddGridClipBoardPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//���Ӹ������
	void AddGridCellPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//�����������
	void AddGridDataPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//���ӱ������
	void AddGridReportPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//����ͼ��ѡ�����
	void AddDiagramOptionPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//�����������
	void AddDiagramZoomPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//���ӷ������
	void AddDiagramAnalyzePanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//���ӱ������
	void AddDiagramReportPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//��Ӷ��������
	void AddAdditionalContent(void);
	//����״̬��
	void CreateStatusBar(void);
	//����λͼ
	void LoadRibbonBitmap(void);
	//���ؿ�ʼRibbonҳ��ͼƬ
	void LoadStartPageRibbonBitmap(void);
	//����ҳ���Ribbonҳ��ͼƬ
	void LoadPageRibbonBitmap(void);
	//���ر���Ribbonҳ��ͼƬ
	void LoadGridPageRibbonBitmap(void);
	//�����Ǳ��Ribbonҳ��ͼƬ
	void LoadDevicePageRibbonBitmap(void);
	//����ͼ���Ribbonҳ��ͻ��
	void LoadDiagramPageRibbonBitmap(void);
	//����MainCategoryͼ��
	void LoadMainCategoryBitmap(void);
protected:
	CBaseRibbonBar m_wndRibbonBar;
	CBCGPRibbonMainButton m_MainButton;
	//�Ǳ��Category
	CBCGPRibbonCategory* m_pDeviceCategory;
	//ͼ���Category
	CBCGPRibbonCategory* m_pDiagramCategory;
	//����Category
	CBCGPRibbonCategory* m_pGridCategory;
	//��ʼRibbonҳ��Largeλͼ
	CBCGPToolBarImages m_RibbonStartPageLargeBitmap;
	//��ʼRibbonҳ��Smallλͼ
	CBCGPToolBarImages m_RibbonStartPageSmallBitmap;
	//ҳ��Ribbonҳ��Largeλͼ
	CBCGPToolBarImages m_RibbonPageLargeBitmap;
	//ҳ��Ribbonҳ��Smallλͼ
	CBCGPToolBarImages m_RibbonPageSmallBitmap;
	//���Ribbonҳ��Largeλͼ
	CBCGPToolBarImages m_RibbonGridPageLargeBitmap;
	//���Ribbonҳ��Smallλͼ
	CBCGPToolBarImages m_RibbonGridPageSmallBitmap;
	//�Ǳ�Ribbonҳ��Largeλͼ
	CBCGPToolBarImages m_RibbonDevicePageLargeBitmap;
	//�Ǳ�Ribbonҳ��Smallλͼ
	CBCGPToolBarImages m_RibbonDevicePageSmallBitmap;
	//ͼ��Ribbonҳ��Largeλͼ
	CBCGPToolBarImages m_RibbonDiagramPageLargeBitmap;
	//ͼ��Ribbonҳ��Smallλͼ
	CBCGPToolBarImages m_RibbonDiagramPageSmallBitmap;
	//MainCategory��Largeλͼ
	CBCGPToolBarImages m_MainCategoryLargeBitmap;
	//MainCategory��Smallλͼ
	CBCGPToolBarImages m_MainCategorySmallBitmap;
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	afx_msg void HandleRadioClick(UINT nID);
	afx_msg void OnUpdateRadioButton(CCmdUI* pCmdUI);
#if 0
protected:
	//Ribbon��Ϣ��Ӧ����
	//ѡ������
	afx_msg void HandleSelctSensor(void) {}
	//�ɼ�����
	afx_msg void HandleCaptureParam(void) {}
	//��������
	afx_msg void HandleConfigData(void) {}
	
	//��ӱ��
	afx_msg void HandleAddTable(void) {}
	//���ͼ
	afx_msg void HandleAddImage(void) {}
	//����Ǳ�
	afx_msg void HandleAddDevice(void) {}
	//ɾ��Ԫ��
	afx_msg void HandleDelElement(void) {}


	//��ʼ
	afx_msg void HandleStart(void) {}
	//�ǵ�
	afx_msg void HandleRecordPoint(void) {}
	//����
	afx_msg void HandleFixedZero(void) {}
	//У׼
	afx_msg void HandleCalibration(void) {}
	//ȡ�������У׼
	afx_msg void HandleCancelZeroAndCalibration(void) {}

	//��ʼ�ط�
	afx_msg void HandleStartReplay(void) {}
	//��ͣ�ط�
	afx_msg void HandlePauseReplay(void) {}
	//ѡ��������
	afx_msg void HandleSelectDataArray(void) {}
	//ѡ���ٶ�
	afx_msg void HandleSelectSpeed(void) {}

	//ʾ����
	afx_msg void HandleOscilloscope(void) {}
	//2ά�˶��ϳ���ֽ�
	afx_msg void Handle2DComposeAndDecompose(void) {}

	//���ҳ��
	afx_msg void HandleAddPage(void) {}
	//ɾ��ҳ��
	afx_msg void HandleDelPage(void) {}
	//ҳ������
	afx_msg void HandlePageName(void) {}
	//��תҳ��
	afx_msg void HandleGotoPage(void) {}

	//ѡ��
	afx_msg void HandleOption(void) {}
	//��ֵ���
	afx_msg void HandleNumStyle(void) {}
	//���̷��
	afx_msg void HandleWatchStyle(void) {}
	//��һ������
	afx_msg void HandleNextDataColumn(void) {}
#endif
protected:
	UINT m_nCurrentRadioID;
public:
	afx_msg void OnMenuEnglish();
	afx_msg void OnUpdateMenuEnglish(CCmdUI *pCmdUI);
	afx_msg void OnMenuChinese();
	afx_msg void OnUpdateMenuChinese(CCmdUI *pCmdUI);


protected:
	CBCGPSplitterWnd m_SpliterWnd;
	CBCGPSplitterWnd m_SpliterWndSecond;
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CCreateContext* m_pContext;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();


	afx_msg void EnableStatusBar(void) {}

	afx_msg LRESULT NotifyActiveWnd(WPARAM wp,LPARAM lp);


private:
	void ShowRibbonCatagory(CBCGPRibbonCategory* pCatagory,bool bShow = false);

	//�豸���ߵĻص�
	afx_msg LRESULT NotifyDeviceOnOrOff(WPARAM wp,LPARAM lp);

};


