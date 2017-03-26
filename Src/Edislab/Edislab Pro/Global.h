/****************************************************************************************
*��Ȩ����:
*�ļ���:Global.h
*����:ȫ�ֱ�������
*����:2016.9
******************************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H
#include <set>
#include <vector>

#define SET std::set
#define VECTOR std::vector 
#include <afx.h>		//�����ã���ɾ����û��Ҫ�����ã�
///************************************************************************/
//Grid��������
///************************************************************************/
const COLORREF UnActiveColor = RGB(200,200,200);
const COLORREF ActiveColor = RGB(0,0,25);
#define		DEFUALT_COLUMN_NAME_X	_T("X")
#define		DEFUALT_COLUMN_NAME_Y	_T("Y")
#define		CURRENT_GROUP			_T("��ǰ��")
#define		NEW_GROUP				_T("����")
#define		NEW_COLUMN				_T("����")
#define		GRID_HEADER_LINES		2
#define		DEFAULT_COLUMN_WIDTH	100
#define		INIT_ROW_NUM			100
/***********************************ȫ�����ݻ�����������**********************************************/
#define GROUP_ID				0	//��ID��0��ʼ
#define	VEC_STRING				VECTOR<CString>
#define SENSOR_MAX_ID			70	//�������豸��ID(1~70)
#define USE_COL_ID				101	//�û��Զ�����ID��101��ʼ���ۼ�
#define DEFAULT_COL_COUNT		2	//Ĭ������2��
#define DEFAULT_ROW_COUNT		100	//Ĭ������100��
#define DEFAULT_DATA			_T("")//Ĭ�Ͽ�����
#define MAX_GROUP_NUM			20	//���֧��20��
#define MAX_COLUMN_NUM			20	//ÿ�����֧��20��
#define LAB_MAX_COUNT			6
#define MAX_WIDGET_NUM			9
/******************************************************************************/
/////////////////////////////////ʵ�����ݶ�����//////////////////////////////////
/******************************************************************************/
///************************************************************************/
//�ؼ����ݶ���
///************************************************************************/
//��ߴ�--ʵ������������
typedef enum _COLUMN_POINT_SIZE
{
	//����
	POINTSIZE_HUGE=0, 
	//��
	POINTSIZE_LARGE, 
	//�ϴ�
	POINTSIZE_LARGER, 
	//��ͨ
	POINTSIZE_NORMAL,
	//С
	POINTSIZE_SMALL
}COLUMN_POINT_SIZE;

//����--ʵ������������
typedef enum _COLUMN_TYPE
{
	//�ֹ���
	TYPE_HANDWORK=0, 
	//������
	TYPE_CALCULATE,
	//��������
	TYPE_SENSOR,
	//ʱ����
	TYPE_TIME,
}COLUMN_USE_TYPE;

//С����--ʵ������������
typedef enum _COLUMN_DECIMAL_POINT
{
	//�Զ�
	DECIMAL_POINT_AUTO=-1, 
	DECIMAL_POINT_0, 
	DECIMAL_POINT_1, 
	DECIMAL_POINT_2,
	DECIMAL_POINT_3,
	DECIMAL_POINT_4,
	DECIMAL_POINT_5,
	DECIMAL_POINT_6,
	DECIMAL_POINT_7,
	DECIMAL_POINT_8,
	DECIMAL_POINT_9
}COLUMN_DECIMAL_POINT;

//�༭--ʵ������������
typedef enum _COLUMN_EDIT
{
	//����
	COLUMN_EDIT_PERMIT=0, 
	//������
	COLUMN_EDIT_ONLY_NUM,
	//��ֹ
	COLUMN_EDIT_FORBID
}COLUMN_EDIT;
//Ĭ����ɫ��
//ID����
class IDManager
{
public:
	static IDManager* CreateInstance()
	{
		static IDManager s_IDManager;
		return &s_IDManager;
	}
private:
	IDManager(void){}
	virtual ~IDManager(void){}

	SET<int> idSet;
public:
	//����һ��ID
	int generateID()
	{
		int id=-1;
		for(int i=0; i<INT_MAX; ++i)
		{
			if(idSet.find(i)==idSet.end())
			{
				id = i;
				idSet.insert(id);
				break;
			}
		}
		return id;
	}
	//ɾ��һ��ID
	void delID(int id)
	{
		idSet.erase(id);
	}
};
//Grid�� ���ݽṹ����
typedef struct _ColumnData
{
	//������ID
	int		nColumnID;
	//������ID
	int		nSensorID;
	//ͨ��ID
	int		nSensorChanel;
	//����������
	CString strColumnName;
	//ע��
	CString strColumnUnit;
	//����ɫ
	COLORREF	nColColor;
	//��ߴ�
	COLUMN_POINT_SIZE	emPointSize;

	//����
	COLUMN_USE_TYPE		emType;
	//С����
	COLUMN_DECIMAL_POINT	emDecimalPoint;
	//�༭
	COLUMN_EDIT		emEdit;
	//����
	VEC_STRING data;
	_ColumnData(void)
	{
		nColumnID = -1;//IDManager::CreateInstance()->generateID();
		nSensorID = -1;
		nSensorChanel = -1;
		strColumnName.Empty();
		strColumnUnit.Empty();
		nColColor = 0;
		emPointSize = POINTSIZE_NORMAL;
		emType = TYPE_HANDWORK;
		emDecimalPoint = DECIMAL_POINT_AUTO;
		emEdit = COLUMN_EDIT_PERMIT;
	}
	~_ColumnData(){
		//IDManager::CreateInstance()->delID(nColumnID);
	}

}COLUMNDATA,* LPCOLUMNDATA;

//Grid�� ���ݽṹ����
typedef struct _GroupData
{
	//������ID
	int			nGroupID;
	//����������
	CString		strGroupName;
	//ע��
	CString		strGroupNote;
	//���ʱ��
	SYSTEMTIME	sysAddTime;
	//Ĭ���� //��ǰ����������
	bool		bDefault;

	VECTOR<COLUMNDATA> vecColData;

	_GroupData(void)
	{
		nGroupID = -1;//IDManager::CreateInstance()->generateID();
		strGroupName = CURRENT_GROUP;
		strGroupNote.Empty();
		GetLocalTime(&sysAddTime);
		bDefault = false;

		vecColData.clear();
	}
	~_GroupData()
	{
		//IDManager::CreateInstance()->delID(nGroupID);
	}

}GROUPDATA,* LPGROUPDATA;

//���������������Ľṹ
typedef struct _PairIndex
{
	//������
	int	nGroup;

	//����Ϣ����
	int	nID;

	//����������
	int nColIndex;

	//ÿ�е�ʵ�����ݸ���
	int nColDataCount;

	_PairIndex()
	{
		nID = 0;
		nColIndex = 0;
		nColDataCount = 0;
	}

}PAIRINDEX,* LPPAIRINDEX;

//��������
//extern vector<VEC_STRING>		g_LabData;
//
////�������ݹ���/����
//extern vector<GROUPDATA>		g_LabDataAttribution;
//
////ʵ����������
//extern int						g_LabDataRowCount;
//
////����ͷ
//extern vector<PAIRINDEX>		g_LabDataIndex;

extern BOOL g_bAutoSelect;
#endif
