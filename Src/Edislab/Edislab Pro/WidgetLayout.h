#pragma once
#include "Type.h"
class CAbstractSplitAlgorithm;
class CWidgetLayout
{
public:
	//�ָ�������
	static const int SPLIT_BAR_NUM = 2;
	CWidgetLayout(CWnd* pHostWnd,bool bWndFront = false);
	virtual ~CWidgetLayout(void);
	//������
	void AddWidget(CWnd* pWidget);
	//ɾ�����
	void DelWidget(CWnd* pWidget);
	//��������
	void AdjustLayout(int nWidth,int nHeight);
	//�������
	void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//�������
	void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//����ƶ�
	void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//��ȡˮƽ�ƶ��ָ���
	CWnd* GetMoveHorSplitBar(void);

	//��ȡ��ֱ�ƶ������
	CWnd* GetMoveVerSplitBar(void);

	void InitLayout(CWnd* pWnd);

	//��ȡwidget����
	int GetWidgetNum(void) const;
private:
	//�����ָ���
	void CreateSplitBar(CWnd* pWnd);

	//���ٷָ���
	void DestroySplitBar(void);

	void InitSplitBar(void);

	void UnInitSplitBar(void);
private:
	//���������Ĵ���ָ��
	CWnd* m_pHostWnd;
	//���ִ����б�
	WidgetArray m_WidgetArray;
	//���µĴ�����ǰ�����ں�ı�־
	bool m_bRemainingWndFront;
	//�ڲ��ָ���
	SplitBarArray m_SplitBarArray;
	//�ָ��㷨
	CAbstractSplitAlgorithm* m_pSplitAlgorithm;
	//һ���ָ���
	CSplitBar* m_SplitArray[SPLIT_BAR_NUM];
};

