#pragma once
#include "Abstractsplitalgorithm.h"

//����һ�����ڵĲ��ַָ��㷨
class COneWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	COneWidgetSplitAlgorithm(void);
	~COneWidgetSplitAlgorithm(void);


public:
	//�������
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight) {}
	//�������
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight) {}
	//����ƶ�
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight) {}
private:
	//����ˮƽ�ƶ�
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight) {}
	//����ֱ�ƶ�
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight) {}
	//����ֱ��ˮƽ�ƶ�
	void HandleHorAndVerMove(int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight) {}
	//ˮƽ�ƶ�֮���������
	void CalculateCoordinateAfterHorMove(int nXMove,int nCurrentIndex,int nWidth,int nHeight) {}
	//��ֱ�ƶ�֮���������
	void CalculateCoordinateAfterVerMove(int nYMove,int nCurrentIndex,int nWidth,int nHeight) {}
	//��ֱ��ˮƽ�ƶ�֮���������
	void CalculateCoordinateAfterHorAndVerMove(int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight) {}
	//���¼��㽻���λ��
	void ReCalculateIntersetRect(void) {}

protected:
	virtual void AdjustLayout(int nWidth,int nHeight);

	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
};

//����2�����ڵĲ��ַָ��㷨
class CTwoWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CTwoWidgetSplitAlgorithm(void);
	~CTwoWidgetSplitAlgorithm(void);

	//�������
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//�������
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//����ƶ�
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
protected:
	//����ˮƽ�ƶ�
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//����ֱ�ƶ�
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight) {}
	//����ֱ��ˮƽ�ƶ�
	void HandleHorAndVerMove(int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight) {}
	//ˮƽ�ƶ�֮���������
	void CalculateCoordinateAfterHorMove(int nXMove,int nCurrentIndex,int nWidth,int nHeight);
	//��ֱ�ƶ�֮���������
	void CalculateCoordinateAfterVerMove(int nYMove,int nCurrentIndex,int nWidth,int nHeight) {}
	//��ֱ��ˮƽ�ƶ�֮���������
	void CalculateCoordinateAfterHorAndVerMove(int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight) {}
	//���¼��㽻���λ��
	void ReCalculateIntersetRect(void) {}
protected:
	//��������λ�ò�ѯ״̬
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);

	virtual void AdjustLayout(int nWidth,int nHeight);

	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
protected:
	//ˮƽ�ָ���
	CRect m_HorSplitBarRect[1];
};

//����3�����ڵĲ��ַָ��㷨
class CThreeWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CThreeWidgetSplitAlgorithm(void);
	~CThreeWidgetSplitAlgorithm(void);
public:
	//�������
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//�������
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//����ƶ�
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
protected:
	//����ˮƽ�ƶ�
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//����ֱ�ƶ�
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
protected:
	//��������λ�ò�ѯ״̬
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);

	virtual void AdjustLayout(int nWidth,int nHeight);

	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
private:
	//����ര����ǰ��Ĳ���
	void AdjustLayoutFront(int nWidth,int nHeight);
	//����ര���ں���Ĳ���
	void AdjustLayoutBack(int nWidth,int nHeight);
private:
	//2���ָ���
	CRect m_HorSplitBarRect[1];
	CRect m_VerSplitBarRect[1];
};

//����4�����ڵĲ��ַָ��㷨
class CFourWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CFourWidgetSplitAlgorithm(void);
	~CFourWidgetSplitAlgorithm(void);
public:
	//�������
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//�������
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//����ƶ�
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
protected:
	//����ˮƽ�ƶ�
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
protected:
	//��������λ�ò�ѯ״̬
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);
protected:
	virtual void AdjustLayout(int nWidth,int nHeight);

	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
private:
	//����ര����ǰ��Ĳ���
	void AdjustLayoutFront(int nWidth,int nHeight);
	//����ര���ں���Ĳ���
	void AdjustLayoutBack(int nWidth,int nHeight);
	//���¼���ѡ�к�ķָ�����λ��
	void CalculateNextSplitBar(int nCurrentIndex,int nXMove,int nWidth,int nHeight);
private:
	CRect m_HorSplitBarRect[3];
};

//����5�����ڵĲ��ַָ��㷨
class CFiveWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CFiveWidgetSplitAlgorithm(void);
	~CFiveWidgetSplitAlgorithm(void);
public:
	//�������
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//�������
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

	//����ƶ�
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

protected:
	//����ˮƽ�ƶ�
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//����ֱ�ƶ�
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//����ֱ��ˮƽ�ƶ�
	void HandleHorAndVerMove(int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight);
	//ˮƽ�ƶ�֮���������
	void CalculateCoordinateAfterHorMove(int nXMove,int nCurrentIndex,int nWidth,int nHeight);
	//��ֱ�ƶ�֮���������
	void CalculateCoordinateAfterVerMove(int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//��ֱ��ˮƽ�ƶ�֮���������
	void CalculateCoordinateAfterHorAndVerMove(int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//���¼��㽻���λ��
	void ReCalculateIntersetRect(void);
protected:
	//��������λ�ò�ѯ״̬
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);
protected:
	virtual void AdjustLayout(int nWidth,int nHeight);

	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
private:
	//����ര����ǰ��Ĳ���
	void AdjustLayoutFront(int nWidth,int nHeight);
	//����ര���ں���Ĳ���
	void AdjustLayoutBack(int nWidth,int nHeight);
private:
	//3���ָ���
	CRect m_HorSplitBarRect[2];
	CRect m_VerSplitBarRect[1];
	//1���ָ����Ľ���
	SPLIT_INTERSECT_RECT m_IntersectRect[1];
};

//����6�����ڵĲ��ַָ��㷨
class CSixWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CSixWidgetSplitAlgorithm(void);
	~CSixWidgetSplitAlgorithm(void);
public:
	//�������
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//�������
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//����ƶ�
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);

private:
	//����ˮƽ�ƶ�
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//����ֱ�ƶ�
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//����ֱ��ˮƽ�ƶ�
	void HandleHorAndVerMove(int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight);
	//ˮƽ�ƶ�֮���������
	void CalculateCoordinateAfterHorMove(int nXMove,int nCurrentIndex,int nWidth,int nHeight);
	//��ֱ�ƶ�֮���������
	void CalculateCoordinateAfterVerMove(int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//��ֱ��ˮƽ�ƶ�֮���������
	void CalculateCoordinateAfterHorAndVerMove(int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//���¼��㽻���λ��
	void ReCalculateIntersetRect(void);
protected:
	virtual void AdjustLayout(int nWidth,int nHeight);
	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
	//��������λ�ò�ѯ״̬
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);
private:
	//����ര����ǰ��Ĳ���
	void AdjustLayoutFront(int nWidth,int nHeight);
	//����ര���ں���Ĳ���
	void AdjustLayoutBack(int nWidth,int nHeight);
private:
	//3���ָ���
	CRect m_HorSplitBarRect[2];
	CRect m_VerSplitBarRect[1];
	//2���ָ����Ľ���
	SPLIT_INTERSECT_RECT m_IntersectRect[2];
};

//����7�����ڵĲ��ַָ��㷨
class CSevenWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CSevenWidgetSplitAlgorithm(void);
	~CSevenWidgetSplitAlgorithm(void);
public:
	//�������
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//�������
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//����ƶ�
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
private:
	//����ˮƽ�ƶ�
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//����ֱ�ƶ�
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//����ֱ��ˮƽ�ƶ�
	void HandleHorAndVerMove(int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight);
	//ˮƽ�ƶ�֮���������
	void CalculateCoordinateAfterHorMove(int nXMove,int nCurrentIndex,int nWidth,int nHeight);
	//��ֱ�ƶ�֮���������
	void CalculateCoordinateAfterVerMove(int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//��ֱ��ˮƽ�ƶ�֮���������
	void CalculateCoordinateAfterHorAndVerMove(int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//���¼��㽻���λ��
	void ReCalculateIntersetRect(void);
protected:
	virtual void AdjustLayout(int nWidth,int nHeight);
	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
	//��������λ�ò�ѯ״̬
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);
private:
	//����ര����ǰ��Ĳ���
	void AdjustLayoutFront(int nWidth,int nHeight);
	//����ര���ں���Ĳ���
	void AdjustLayoutBack(int nWidth,int nHeight);
private:
	//4���ָ���
	CRect m_HorSplitBarRect[3];
	CRect m_VerSplitBarRect[1];
	//2���ָ����Ľ���
	SPLIT_INTERSECT_RECT m_IntersectRect[2];
};

//����8�����ڵĲ��ַָ��㷨
class CEightWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CEightWidgetSplitAlgorithm(void);
	~CEightWidgetSplitAlgorithm(void);
public:
	//�������
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//�������
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//����ƶ�
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
private:
	//����ˮƽ�ƶ�
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//����ֱ�ƶ�
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//����ֱ��ˮƽ�ƶ�
	void HandleHorAndVerMove(int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight);
	//ˮƽ�ƶ�֮���������
	void CalculateCoordinateAfterHorMove(int nXMove,int nCurrentIndex,int nWidth,int nHeight);
	//��ֱ�ƶ�֮���������
	void CalculateCoordinateAfterVerMove(int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//��ֱ��ˮƽ�ƶ�֮���������
	void CalculateCoordinateAfterHorAndVerMove(int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//���¼��㽻���λ��
	void ReCalculateIntersetRect(void);
protected:
	virtual void AdjustLayout(int nWidth,int nHeight);
	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
	//��������λ�ò�ѯ״̬
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);
private:
	//����ര����ǰ��Ĳ���
	void AdjustLayoutFront(int nWidth,int nHeight);
	//����ര���ں���Ĳ���
	void AdjustLayoutBack(int nWidth,int nHeight);
private:
	//4���ָ���
	CRect m_HorSplitBarRect[3];
	CRect m_VerSplitBarRect[1];
	//2���ָ����Ľ���
	SPLIT_INTERSECT_RECT m_IntersectRect[3];
};

//����9�����ڵĲ��ַָ��㷨
class CNineWidgetSplitAlgorithm : public CAbstractSplitAlgorithm
{
public:
	CNineWidgetSplitAlgorithm(void);
	~CNineWidgetSplitAlgorithm(void);
public:
	//�������
	virtual void HandleLButtonDown(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//�������
	virtual void HandleLButtonUp(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
	//����ƶ�
	virtual void HandleMouseMove(UINT nFlags,const CPoint& pt,int nWidth,int nHeight);
private:
	//����ˮƽ�ƶ�
	void HandleHorMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//����ֱ�ƶ�
	void HandleVerMove(int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight);
	//����ֱ��ˮƽ�ƶ�
	void HandleHorAndVerMove(int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight);
	//ˮƽ�ƶ�֮���������
	void CalculateCoordinateAfterHorMove(int nXMove,int nCurrentIndex,int nWidth,int nHeight);
	//��ֱ�ƶ�֮���������
	void CalculateCoordinateAfterVerMove(int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//��ֱ��ˮƽ�ƶ�֮���������
	void CalculateCoordinateAfterHorAndVerMove(int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight);
	//���¼��㽻���λ��
	void ReCalculateIntersetRect(void);
protected:
	virtual void AdjustLayout(int nWidth,int nHeight);
	virtual void CalculateLayoutSize(int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray);
	//��������λ�ò�ѯ״̬
	virtual MOUSE_STATE QueryPosStatus(const CPoint& pt);
private:
	//����ര����ǰ��Ĳ���
	void AdjustLayoutFront(int nWidth,int nHeight);
	//����ര���ں���Ĳ���
	void AdjustLayoutBack(int nWidth,int nHeight);
private:
	//4���ָ���
	CRect m_HorSplitBarRect[2];
	CRect m_VerSplitBarRect[2];
	//2���ָ����Ľ���
	SPLIT_INTERSECT_RECT m_IntersectRect[4];
};