/**********************************************************
*��Ȩ����:xiaowei.han
*�ļ�����:CustomGrid.h
*��������:�Զ����Grid�ؼ�
*����:xiaowei.han
*����:2017/04/22
**********************************************************/
#ifndef CUSTOM_GRID_H
#define CUSTOM_GRID_H
#include <vector>
#include <vector>
//�����е�����
typedef struct _header_info
{
	//�е�����
	CString strHeadName;

	//�����е�����
	std::vector<CString> ContainColumnIndexArray;

	_header_info()
	{
		strHeadName = _T("");
		ContainColumnIndexArray.clear();
	}
	void Reset(void)
	{
		strHeadName = _T("");
		ContainColumnIndexArray.clear();
	}

}HEADRER_INFO,* LP_HEADRER_INFO;

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
	void SetHeaderInfoArray(const std::vector<HEADRER_INFO>& HeaderInfoArray);

	//�����ͷ��Ϣ
	void AddHeaderInfo(const HEADRER_INFO& HeaderInfo);

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
	void GetHeaderInfo(std::vector<HEADRER_INFO>& HeaderInfoArray);

	//�������
	void FillData(void);
private:
	//��������Ϣ
	void CreateHeaderInfo(void);
	//��������Ϣ
	void CreateColumnInfo(void);
protected:
	std::vector<HEADRER_INFO> m_HeaderInfoArray;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#endif