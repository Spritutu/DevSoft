/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:ErrorCodeManager.cpp
Description:�����������
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#include "stdafx.h"
#include "ErrorCodeManager.h"
#include "ErrorCode.h"

/*****************************************************************************
* @brief : ʵ�ִ����������ĵ�ʵ��
* @author : xiaowei.han
* @date : 2016/6/27 11:10
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
CErrorCodeManager& CErrorCodeManager::CreateInstance( void )
{
    static CErrorCodeManager s_ErrorCodeManager;
    return s_ErrorCodeManager;
}

/*****************************************************************************
* @brief : ������������ʼ��
* @author : xiaowei.han
* @date : 2016/6/27 11:11
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
void CErrorCodeManager::Init( void )
{
    int nCode = ERROR_PARAM_INVALID;
    std::string strCodeInfo = "�������Ϸ�";
    m_ErrorCodeMap.insert(std::map<int,std::string>::value_type(nCode,strCodeInfo));
}

CErrorCodeManager::CErrorCodeManager( void )
{
    Init();
}

CErrorCodeManager::~CErrorCodeManager( void )
{
    UnInit();
}

/*****************************************************************************
* @brief : ����������෴��ʼ��
* @author : xiaowei.han
* @date : 2016/6/27 11:11
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
void CErrorCodeManager::UnInit( void )
{
    m_ErrorCodeMap.clear();
}

/*****************************************************************************
* @brief : ���ݴ������ѯ��������
* @author : xiaowei.han
* @date : 2016/6/27 11:12
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
std::string CErrorCodeManager::QueryErrorInfoByErrorCode( int nErrorCode )
{
    std::string strErrorInfo;

    std::map<int,std::string>::iterator Iter = m_ErrorCodeMap.find(nErrorCode);

    if (Iter != m_ErrorCodeMap.end())
    {
        strErrorInfo = Iter->second;
    }

    return strErrorInfo;
}
