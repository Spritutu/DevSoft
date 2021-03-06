#pragma once
#include "BaseDialog.h"
#include "GridGroupType.h"
class CDlgGridOpt : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgGridOpt)

public:
	CDlgGridOpt(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgGridOpt();

// 对话框数据
	enum { IDD = IDD_DLG_GRID_OPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CBCGPTreeCtrlEx m_GridDisplayItemTree;
	CStatic m_DisplayItemTreeLocation;
private:
	//初始化显示
	void InitDisplay(void);
public:
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnBnClickedBtnCancel();

	//设置显示选项
	void SetDisplayInfo(const std::vector<SHOW_COLUMN_GROUP_INFO>& GridDisplayArray);
	//获取显示选项
	void GetDisplayInfo(std::vector<SHOW_COLUMN_GROUP_INFO>& GridDisplayArray);
private:
	std::vector<SHOW_COLUMN_GROUP_INFO> m_GridDisplayArray;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg LRESULT OnHeaderCheckClick(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnCheckClick(WPARAM wp, LPARAM lp);
};
