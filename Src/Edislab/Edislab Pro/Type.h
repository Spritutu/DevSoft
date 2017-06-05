#ifndef TYPE_H
#define TYPE_H
#include <vector>
#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/shared_lock_guard.hpp>
#include <boost/function.hpp>
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

enum SENSOR_TYPE{
	SENSOR_PHYSICS = 1,
	SENSOR_CHEMISTRY = 2,
	SENSOR_BIOLOGY = 4,
	SENSOR_ALL = 0xFFFFFF
};

// ������������Ϣ
typedef struct _sensor_type_info
{
	SENSOR_TYPE enumType;
	std::string strTypeName;

	_sensor_type_info()
	{
		enumType = SENSOR_ALL;
	}
}SENSOR_TYPE_INFO, *LP_SENSOR_TYPE_INFO;


//��������Ϣ����
typedef struct _sensor_com_config_element
{
	//������ID
	int nSensorID;
	//����������
	std::string strSensorName;
	//COM������
	std::string strSerialPort;
	//������
	unsigned int nBaudRate;
	//ͨ���ֽ�λ��
	int nDataBits;
	//ֹͣλ
	int nStopBits;//(0:1��ֹͣλ 1:1.5��ֹͣλ(Linux�ݲ�֧��1.5��ֹͣλ) 2:2��ֹͣλ)
	//У������
	int nPairty;//(0:��ʹ��У��  1:����У�� 2:ż��У�� 3:���У�飨Linux��û�д�� 4:�ո�У��)
	//�Ƿ�ʹ������
	bool bUseFlowControl;

	_sensor_com_config_element(void)
	{
		nSensorID = -1;
		strSensorName = "";
		nBaudRate = 9600;
		nDataBits = 8;
		nStopBits = 0;
		nPairty = 0;
		strSerialPort = "";
		bUseFlowControl = false;
	}
}SENSOR_COM_CONFIG_ELEMENT,* LP_SENSOR_COM_CONFIG_ELEMENT;

//������������Ϣ
typedef struct _sensor_range_info_element
{
	//��������
	std::string strRangeName;
	//У׼ֵ
	std::string strCalibrationvalue;
	//int nCalibrationvalue;
	//Ĭ��ֵ
	int nDefaultvalue;
	//���ֵ
	int nMaxvalue;
	//��Сֵ
	int nMinvalue;
	//monitorֵ
	std::string strMonitorvalue;
	//����
	int nAccuracy;
	//��λ
	std::string strUnitName;
	//ת����ʽ
	std::string strConverformula;
	//ת���ص�����
	boost::function<float(float,float,float)> pConverformulaFunc;

}SENSOR_RANGE_INFO_ELEMENT,* LP_SENSOR_RANGE_INFO_ELEMENT;

//���������ýṹ�嶨��
typedef struct _sensor_config_element
{
	//��������ID
	unsigned int nSensorID;
	//��������� (1:���� 2:��ѧ 4:���� 7:����)
	int nSensorCategory;
	//�������ͺ�
	std::string strSensorModelName;
	//����������
	std::string strSensorName;
	//����������
	std::string strSensorSymbol;
	//������˵��
	std::string strSensorDescription;
	//��������COMͨ��������Ϣ
	//SENSOR_COM_CONFIG_ELEMENT SensorComConfigInfo;
	// ��ǰѡ���������Ϣ   add by tangmh
	int nCurRangeIndex;
	// end add
	//������Ϣ
	std::vector<SENSOR_RANGE_INFO_ELEMENT> SensorRangeInfoArray;
	//Ĭ��Ƶ��
	//int nSensorDefaultfrequency;
	float fSensorDefaultfrequency;
	//���Ƶ��
	float nSensorMaxfrequency;
	//��СƵ��
	float nSensorMinfrequency;
	//ʱ�䳤��
	int nSensorTimeLength;
	//�����
	bool bMultiSample;
	//����������
	std::string strSensorTypeName;
	//�Ƿ���Ҫ��ӵ��б���
	bool bAddtoSensorlist;

	_sensor_config_element(void)
	{
		nCurRangeIndex = -1;
		nSensorID = -1;
		bAddtoSensorlist = true;
		SensorRangeInfoArray.clear();
	}
}SENSOR_CONFIG_ELEMENT,* LP_SENSOR_CONFIG_ELEMENT;

//����
typedef boost::shared_lock_guard<boost::mutex> ReadLock;

//д��
typedef boost::lock_guard<boost::mutex> WriteLock;


//����������
typedef struct _sensor_type_info_element
{
	std::string strSensorName;
	int nSensorID;

	_sensor_type_info_element(void)
	{
		strSensorName = "";
		nSensorID = -1;
	}
}SENSOR_TYPE_INFO_ELEMENT,* LP_SENSOR_TYPE_INFO_ELEMENT;

//����ָ�붨��
typedef void (*pCommandEntry)(CEdislabProView* pView);
//���º���ָ�붨��
typedef void (*pUpdateCommandEntry)(CEdislabProView* pView,CCmdUI* pCmdUI);

enum RECORD_TYPE{
	FIXED_FREQUENCY_RC_TYPE,
	PHOTO_GATE_RC_TYPE,
	TRIGER_RC_TYPE,
	SPECIALIZED_RC_TYPE,
	MANUAL_RC_TYPE
};

typedef struct _sensor_record_info
{
	// ʵ�����ݼ�¼ģʽ
	RECORD_TYPE enumRecordType;
	
	// ����Ƶ��
	float fFrequency;

	// �Ƿ��޶�ʱ��
	bool bLimitTime;

	// �޶���ʱ��
	float fLimitTime;

	// �Ƿ���0ʱ����
	bool bSampleAtStart;

	// �Ƿ�����
	bool bMultiSample;

	// ���ٵ����ֵ
	int nAverageNums;

	_sensor_record_info()
	{
        enumRecordType = FIXED_FREQUENCY_RC_TYPE;
		fFrequency = 0.0f;
		bLimitTime = false;
		fLimitTime = 0.0f;
		bSampleAtStart = false;
		bMultiSample = false;
		nAverageNums = 1;
	}

}SENSOR_RECORD_INFO, *LPSENSOR_RECORD_INFO;


#endif