#include "stdafx.h"
#include "Lock.h"
CLock::CLock()
{
	InitializeCriticalSection(&cs);//��ʼ���ٽ��� 
}


CLock::~CLock()
{
	DeleteCriticalSection(&cs);//ɾ���ٽ���
}


void CLock::Lock()
{
	EnterCriticalSection(&cs);//�����ٽ��� 
}


void CLock::UnLock()
{
	LeaveCriticalSection(&cs);//�뿪�ٽ��� 
}
