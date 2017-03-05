/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:ErrorCodeManager.h
Description:�����������
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#ifndef ERROR_CODE_MANAGER_H
#define ERROR_CODE_MANAGER_H
#include <map>
#include <string>
class CErrorCodeManager
{

public:
    //��ʵ��
    static CErrorCodeManager& CreateInstance(void);  
public:
    //���ݴ������ѯ��������
    std::string QueryErrorInfoByErrorCode(int nErrorCode);
private:
    CErrorCodeManager(void);
    ~CErrorCodeManager(void);
private:
    //��ʼ��
    void Init(void);
    //����ʼ��
    void UnInit(void);
private:
    std::map<int,std::string> m_ErrorCodeMap;

};

#endif