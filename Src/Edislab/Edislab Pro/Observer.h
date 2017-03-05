/****************************************************************************************
*��Ȩ����:
*�ļ���:Observer.h
*����:�۲���ģʽ--�۲���
*����:2016.9
******************************************************************************************/
#ifndef OBSERVER_H
#define OBSERVER_H

#include <stdio.h>

class CObservable;

//�۲��ߣ��������
//��Ϊ�۲�����̳д˴�����࣬do your something
class CObserver
{
public:
	
    CObserver();
    virtual ~CObserver();
 
public:
    //�����۲��Ŀ�귢���仯ʱ��֪ͨ���ø÷���
    //���Ա��۲���pObs, ��չ����ΪpArg
    virtual void Update(CObservable* pObs, void* pArg = NULL) = 0;
};

#endif