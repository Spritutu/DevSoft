#ifndef _GRID_DEFINE_H
#define _GRID_DEFINE_H
#include "stdafx.h"
#include "GlobalDataManager.h"
#include <vector>
using std::vector;
class CCustomLabGridCtrl;
///************************************************************************/
//�ؼ���ͼ����
///************************************************************************/
#define DEFUALT_VALUE		0

//��������
enum GRID_FONT_TYPE
{
	NORMAL = 1,	
	BOLD,
	ITALIC
};

//�ؼ����ڹ���Ľṹ�����
typedef struct _ViewManager
{
	//�ؼ����ھ��
	CCustomLabGridCtrl* pGrid;

	//�����ھ��
	//CWnd* hWnd;

	//�ؼ�λ��
	CRect rtClient;

	//��������
	GRID_FONT_TYPE emFontType;

	//������
	vector<int> vecHideColIndex;

	_ViewManager(void)
	{
		pGrid = NULL;
		emFontType = NORMAL;
		vecHideColIndex.clear();
	}
}VIEWMANAGER,* LPVIEWMANAGER;



#endif