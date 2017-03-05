/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:Msg.h
Description:��Ϣͷ����
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#ifndef MSG_H
#define MSG_H

enum MESSAGE
{
	WM_BASE_INDEX = WM_USER + 100,

	//֪ͨ����ܵ����ʲô
	WM_NOTIFY_ACTIVE_WND_TYPE
	
};

enum COMMAND
{
	ID_BASE_COMMAND = WM_USER + 2000,

	//����ʵ��
	ID_TYPICAL_TEST,

	//���ʵ�鱨��
	ID_OUTPUT_TEST_REPORT,

	//ѡ�񴫸���
	ID_SELECT_SENSOR,

	//�Զ�ʶ��
	ID_AUTO_SELECT,

	//�ֶ�ѡ��
	ID_MANUAL_SELECT,

	//�ɼ�����
	ID_COLLECT_PARAM,

	//��������
	ID_CONFIG_DATA,

	//���ӱ���
	ID_ADD_TABLE,

	//����ͼ
	ID_ADD_IMAGE,

	//�����Ǳ�
	ID_ADD_DEVICE,

	//ɾ��Ԫ��
	ID_DEL_ELEMENT,

	//��ʼ
	ID_START,

	//�ǵ�
	ID_RECORD_POINT,

	//����
	ID_ZERO,

	//У׼
	ID_CALIBRATION,

	//ȡ��������У׼
	ID_CANCEL_ZERO_AND_CALIBRATION,

	//��ʼ�ط�
	ID_START_REPLAY,

	//��ͣ�ط�
	ID_PAUSE_REPLAY,

	//����
	ID_SELECT_ARRAY,

	//�ٶ�
	ID_SELECT_SPEED,

	//ʾ����
	ID_OSCILLOSCOPE,

	//��ά�˶��ϳ���ֽ�
	ID_2D_COMPOSE_AND_DECOMPOSE,

	//����ҳ��
	ID_ADD_PAGE,

	//ɾ��ҳ��
	ID_DEL_PAGE,

	//ҳ������
	ID_SET_PAGE_NAME,

	//��תҳ��
	ID_GOTO_PAGE,

	//ѡ��
	ID_OPTION,

	//��ֵ���
	ID_NUM_STYLE,

	//���̷��
	ID_WATCH_STYLE,

	//��һ��������
	ID_NEXT_DATA_COLUMN,

	//����
	ID_COPY,

	//ճ��
	ID_PASTE,

	//��һ��
	ID_FIRST_ROW,

	//���һ��
	ID_LAST_ROW,

	//������
	ID_INSERT_ROW,

	//ɾ������
	ID_DEL_CELL,

	//����������
	ID_ADD_DATA_COLUMN,

	//��������
	ID_YIELD_DATA,

	//����
	ID_CALCULATE,

	//���������
	ID_CLEAR_CHILD_DATA,

	//�洢ΪExcel
	ID_SAVE_EXCEL,

	//��ӡ
	ID_PRINT,

	//��ӡԤ��
	ID_PRINT_PREVIEW,

	//�϶�ģʽ
	ID_DRAG_MODE,

	//ѡ��ģʽ
	ID_SELECT_MODE,

	//�Զ�����
	ID_AUTO_SCROLL,

	//�Զ�����
	ID_AUTO_ZOOM,

	//������
	ID_NO_SCROLL,

	//����
	ID_LINE,

	//��
	ID_POINT,

	//������
	ID_POINT_TO_LINE,

	//100%
	ID_100_PERCENT,

	//�Ŵ�
	ID_ZOOM_IN,

	//��С
	ID_ZOOM_OUT,

	//�鿴
	ID_VIEW,

	//����
	ID_TANGENT,

	//����
	ID_INTEGRAL,

	//ͳ��
	ID_STATISTICS,

	//ֱ�����
	ID_LINEAR_FITTING,

	//���
	ID_FITTING,

	//����ͻ��
	ID_SAVE_IMAGE,

	//������ʵ�鱨��
	ID_EXPORT_EXPERIMENT_REPORT,

	//ʵ��ָ��
	ID_EXPERIMENT_GUIDE,

	ID_COMMAND_TOTAL,

	ID_MIN_COMMAND = ID_BASE_COMMAND + 1,
	ID_MAX_COMMAND = ID_COMMAND_TOTAL - 1
};

#endif