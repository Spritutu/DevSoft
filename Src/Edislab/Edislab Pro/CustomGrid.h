/**********************************************************
*��Ȩ����:xiaowei.han
*�ļ�����:CustomGrid.h
*��������:�Զ����Grid�ؼ�
*����:xiaowei.han
*����:2017/04/22
**********************************************************/
#ifndef CUSTOM_GRID_H
#define CUSTOM_GRID_H
#include "GridGroupType.h"
class CCustomGrid : public CBCGPGridCtrl
{
public:
	CCustomGrid();
	virtual ~CCustomGrid();
	DECLARE_DYNCREATE(CCustomGrid)

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnTrackHeader ()
	{
		return FALSE; // do not resize columns inside client area by mouse, only at header
	}

	// Merged header items:
public:
	virtual void OnPosSizeChanged();

protected:
	virtual CRect OnGetHeaderRect (CDC* pDC, const CRect& rectDraw);
	virtual void OnDrawHeader (CDC* pDC);
	virtual void OnPrintHeader(CDC* pDC, CPrintInfo* pInfo);
public:
	virtual CBCGPGridColumnsInfo& GetColumnsInfo();
	virtual const CBCGPGridColumnsInfo& GetColumnsInfo() const;
	void DrawHeaderItem (CDC* pDC, CRect rect, CBCGPHeaderItem* pHeaderItem);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	// other header implementation
	CBCGPGridColumnsInfoEx m_ColumnsEx;	
public:
	//������grid��ID
	static unsigned int s_GridID;

public:
	//������ͷ��Ϣ(�ڴ���Grid֮ǰ����)
	void SetHeaderInfoArray(const std::vector<COLUMN_GROUP_INFO>& HeaderInfoArray);

	//��̬����grid����ͷ��Ϣ
	void DynamicSetHeaderInfoArray(const std::vector<COLUMN_GROUP_INFO>& HeaderInfoArray);

	//�����ͷ��Ϣ
	void AddHeaderInfo(const COLUMN_GROUP_INFO& HeaderInfo);

	//ɾ����ͷ��Ϣ
	void RemoveHeaderInfo(const CString& strHeaderName);

	//ɾ��ĳһ��
	void RemoveColumn(const CString& strColumnName);

	//����ĳһ��
	void AddColumnInfo(const CString& strHeaderName,const CString& strColumnName);

	//��ѯ��ͷ�����Ƿ��Ѿ�����
	bool IsHeaderNameExist(const CString& strHeaderName);

	//�ж�ָ����ͷ�����Ƿ��Ѿ�����
	bool IsColumnExistInFixedHeader(const CString& strHeaderName,const CString& strColumnName);

	//��ȡ��ͷ��Ϣ
	void GetHeaderInfo(std::vector<COLUMN_GROUP_INFO>& HeaderInfoArray);

	//���ûص�����
	void SetCallBack(BCGPGRID_CALLBACK pCallBack)
	{
		m_pCallBack = pCallBack;
	}
	//���ó�ʼ��ʾ������
	void SetInitDisplayRow(int nRows)
	{
		m_nDisplayRows = nRows;
	}

	//��������
	void SetDisplayRow(int nRow);
	//ˢ��Grid��ʾ
	void Refresh(void);
	//��ȡ�з������ʾ��Ϣ
	void GetColumnGroupDisplayInfo(std::vector<SHOW_COLUMN_GROUP_INFO>& DisplayArray);
	//���÷������ʾ��Ϣ
	void SetColumnGroupDisplayInfo(const std::vector<SHOW_COLUMN_GROUP_INFO>& DisplayArray);
	//��������(���ڷ����ģʽ��)
	void SetColumnData(int nRowIndex,int nColumnIndex,float fData);
	//����һ��
	void InsertRow(int nRow,bool bBefore = true);
	//�Ƿ������ģʽ
	bool IsVirtualGrid(void);
private:
	//��������Ϣ
	void CreateHeaderInfo(void);
	//��������Ϣ
	void CreateColumnInfo(void);
	//������������Ϣ
	void CreateRowInfo(void);
protected:
	std::vector<COLUMN_GROUP_INFO> m_HeaderInfoArray;
	BCGPGRID_CALLBACK m_pCallBack;
	int m_nDisplayRows;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};

#endif