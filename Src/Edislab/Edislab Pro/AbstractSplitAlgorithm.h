#pragma once
#include <vector>
#include "Type.h"
class CWidgetLayout;
class CAbstractSplitAlgorithm
{
public:
	CAbstractSplitAlgorithm(void);
	virtual ~CAbstractSplitAlgorithm(void);

	void SetWidgetArray(const WidgetArray& Array);

	void SetWndFront(bool bWndFront);

	void SetLayout(CWidgetLayout* pLayout);

	//������������ָ��
	void SetHostWnd(CWnd* pHostWnd);

	//���¼��㲼��
	void RecalculateLayout(int nWidth,int nHeight);

	//�ı䲼��
	void ChangeLayout(int nWidth,int nHeight);

	//�������
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//�������
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//����ƶ�
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
protected:
	//�ָ������������ڷָ�������б�
	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray) {}
	//��������
	virtual void AdjustLayout(int nWidth,int nHeight) {}
	//��������λ�ò�ѯ״̬
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);
	//��ȡָ��������ǰһ������
	int GetPrexHorSplitBarIndex(int nCurrentIndex);
	//��ȡ��ֱ�ָ�����ǰһ������
	int GetPrexVerSplitBarIndex(int nCurrentIndex);
	//����ˮƽ�ָ���
	void ShowHorSplitBar(bool bShow);
	//���ش�ֱ�ָ���
	void ShowVerSplitBar(bool bShow);
protected:
	//�Ӵ��ڵĸ���
	WidgetArray m_WidgetArray;
	//�Ƿ�ǰ��
	bool m_bWndFront;
	//�������ڵ�ָ��
	CWnd* m_pHostWnd;
	//��¼���ѡ�еķָ���
	SPLIT_BAR_INFO m_SelectSplitBarInfo;
	//�������
	CPoint m_StartPoint;
	//������������
	LPCTSTR m_CursorState;
	//���ھֲ���
	CWidgetLayout* m_pLayout;
};

