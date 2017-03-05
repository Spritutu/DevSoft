/*******************************************************************************/
/*����:Grid�ؼ���ͼ������
/*����:
/*����:2015.8
/*******************************************************************************/
#ifndef CUSTOM_LAB_GRID_CTRL_H
#define CUSTOM_LAB_GRID_CTRL_H

#include "GridCtrlBase.h"
#include "GridDataManager.h"
#include "GridViewManager.h"

class CCustomLabGridCtrl :public CBCGPGridCtrl
{
public:
	CCustomLabGridCtrl(void);
	~CCustomLabGridCtrl(void);

	CGridViewManager*	m_GridViewManager;

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnTrackHeader ()
	{
		return FALSE; // do not resize columns inside client area by mouse, only at header
	}


public:
	virtual CBCGPGridColumnsInfo& GetColumnsInfo ()
	{
		return m_ColumnsEx;
	}

	virtual const CBCGPGridColumnsInfo& GetColumnsInfo () const
	{
		return m_ColumnsEx;
	}

	virtual void OnItemChanged (CBCGPGridItem* pItem, int nRow, int nColumn);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

	virtual CRect OnGetHeaderRect (CDC* pDC, const CRect& rectDraw);
	virtual void OnDrawHeader (CDC* pDC);
	virtual void OnPrintHeader(CDC* pDC, CPrintInfo* pInfo);

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnPosSizeChanged ();

public:

	void UpdataGrid();

	void SetViewManager(CGridViewManager* pViewManager);
	//����gridͷ   ����չ
	void SetGridHeader();

	//���ù����ص�����
	void SetCallBack(BCGPGRID_CALLBACK pCallBack);

	int GetVirtualRows(void) const;

	//����������
	void SetDisplayVirtualRows(int nVirtualRows);

	//ˢ��
	void Refresh();

	//����
	virtual void Sort(int nColumn, BOOL bAscending = TRUE, BOOL bAdd = FALSE);
	virtual void OnFilterBarChanged(int nColumn, const CString& strNewFilter);
	virtual BOOL IsRowFilteredByFilterBar(const CBCGPGridRow* pRow);
	virtual BOOL IsItemFilteredByFilterBar(CBCGPGridItem* pItem, int nColumn, const CString& strFilter);

private:

	void InitGridStyle();

	BCGPGRID_CALLBACK m_pCallBack;

	CBCGPGridColumnsInfoEx	m_ColumnsEx;

protected:

	//�������
	int m_nVitrualRows;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif
