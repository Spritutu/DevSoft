#ifndef TYPE_H
#define TYPE_H
#include <vector>
#include "Macro.h"
class CSplitBar;
class CEdislabProView;
typedef std::vector<CWnd*> WidgetArray;
typedef std::vector<CWnd*>::iterator WidgetArrayIter;
typedef std::vector<CSplitBar*> SplitBarArray;
typedef std::vector<CSplitBar*>::iterator SplitBarArrayIter;

//�ָ����Ľ�����Ϣ
typedef struct _split_intersect_rect
{
	CRect InterSectRect;

	//����2�������ཻ
	//ˮƽ�ָ���
	CRect* pRect1;
	//��ֱ�ָ���
	CRect* pRect2;
	//��ֱ�ָ���������
	int nVerIndex;
	//ˮƽ�ָ���������
	int nHorIndex;

	_split_intersect_rect(void)
	{
		pRect1 = NULL;
		pRect2 = NULL;
		nVerIndex = -1;
		nHorIndex = -1;
	}

}SPLIT_INTERSECT_RECT,* LP_SPLIT_INTERSECT_RECT;

//���״̬
enum MOUSE_STATE
{
	MOVE_NO_STATE = 0,
	//ˮƽ�ƶ�
	MOVE_HOR,
	//��ֱ�ƶ�
	MOVE_VER,
	//ˮƽ�ʹ�ֱ�ƶ�
	MOVE_ALL

};

//��¼�ƶ��ָ�������Ϣ
typedef struct _split_bar_info
{
	//�ָ�������
	MOUSE_STATE Type;
	//ռ�õ�����
	unsigned int nIndex;

	_split_bar_info(void)
	{
		Type = MOVE_NO_STATE;
		nIndex = -1;
	}

}SPLIT_BAR_INFO,* LP_SPLIT_BAR_INFO;


enum NEW_PAGE_OPT
{
	NEW_PAGE_EMPTY = 0,
	NEW_PAGE_COPY,
	NEW_PAGE_CUSTOM
};

//����Ԫ�ص����õĽṹ��
typedef struct _add_page_element
{
	//�½�ҳ��ѡ��
	NEW_PAGE_OPT eumOpt;
	//ҳ������
	CString strPageName;
	
	int nGridNum;

	int nDiagramNum;

	int nDeviceNum;

	_add_page_element(void)
	{
		eumOpt = NEW_PAGE_COPY;
		nGridNum = 1;
		nDeviceNum = 1;
		nDiagramNum = 1;
		strPageName = DEFAULT_PAGE_NAME;
	}

}ADD_PAGE_ELEMENT,* LP_ADD_PAGE_ELEMENT;



//����ָ�붨��
typedef void (*pCommandEntry)(CEdislabProView* pView);
//���º���ָ�붨��
typedef void (*pUpdateCommandEntry)(CEdislabProView* pView,CCmdUI* pCmdUI);

#endif