/**********************************************************
*��Ȩ����:xiaowei.han
*�ļ�����:GridDisplayColumnInfo.cpp
*��������:Grid��ʾ����Ϣ���ݹ�����
*����:xiaowei.han
*����:2017/05/14
**********************************************************/
#include "StdAfx.h"
#include "GridDisplayColumnInfo.h"
#include <algorithm>
#include <boost/foreach.hpp>
CGridDisplayColumnInfo& CGridDisplayColumnInfo::CreateInstance()
{
	return s_obj;
}

void CGridDisplayColumnInfo::GetGridDisplayInfo(std::vector<GRID_DISPLAY_INFO>& DisplayInfoArray)
{
	DisplayInfoArray.clear();
	DisplayInfoArray.assign(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end());
}

CGridDisplayColumnInfo::CGridDisplayColumnInfo(void)
{
	m_HeaderInfoArray.clear();
	InitGridDisplayInfo();
}


CGridDisplayColumnInfo::~CGridDisplayColumnInfo(void)
{
	m_HeaderInfoArray.clear();
}

void CGridDisplayColumnInfo::InitGridDisplayInfo()
{
	GRID_DISPLAY_INFO TempInfo;
	TempInfo.strHeadName = _T("��ǰ");
	COLUMN_INFO TempColumnInfo;
	TempColumnInfo.strColumnName = _T("t(s)ʱ��");
	TempInfo.ContainColumnIndexArray.push_back(TempColumnInfo);
	TempColumnInfo.Reset();
	TempColumnInfo.strColumnName = _T("p(kpa)ѹǿ");
	TempColumnInfo.nSensorID = 1;
	TempInfo.ContainColumnIndexArray.push_back(TempColumnInfo);
	m_HeaderInfoArray.push_back(TempInfo);
}

/*******************************************************************
*��������:IsHeaderNameExist
*��������:�ж���ͷ�Ƿ��Ѿ�����
*�������:strHeaderName:��ͷ������
*�������:None
*����ֵ:true:���� false:������
*����:xiaowei.han
*����:2017/05/13 15:59:47
*******************************************************************/
bool CGridDisplayColumnInfo::IsHeaderNameExist(const CString& strHeaderName)
{
	if (strHeaderName.IsEmpty())
	{
		return false;
	}
	//������ͷ��Ϣ
	auto HeaderPred = [&strHeaderName](const GRID_DISPLAY_INFO& Info)->bool
	{
		if (Info.strHeadName == strHeaderName)
		{
			return true;
		}
		return false;
	};
	auto HeaderIter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),HeaderPred);
	if (HeaderIter != m_HeaderInfoArray.end())
	{
		return true;
	}
	return false;
}

/*******************************************************************
*��������:IsColumnExistInFixedHeader
*��������:�ж����Ƿ��Ѿ�����
*�������:strHeaderName:��ͷ������ strColumnName:��ͷ����
*�������:None
*����ֵ:true:���� false:������
*����:xiaowei.han
*����:2017/05/13 15:59:47
*******************************************************************/
bool CGridDisplayColumnInfo::IsColumnExistInFixedHeader(const CString& strHeaderName,const CString& strColumnName)
{
	if (strHeaderName.IsEmpty() || strColumnName.IsEmpty())
	{
		return false;
	}

	//����ĳ�е������Ƿ����Ҫɾ����
	auto HeaderPred = [&strHeaderName](const GRID_DISPLAY_INFO& Info)->bool
	{
		if (Info.strHeadName == strHeaderName)
		{
			return true;
		}
		return false;
	};
	auto HeaderIter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),HeaderPred);
	if (HeaderIter == m_HeaderInfoArray.end())
	{
		return false;
	}

	auto ColumnPred = [&strColumnName](const COLUMN_INFO& ColumnInfo)->bool
	{
		if (ColumnInfo.strColumnName == strColumnName)
		{
			return true;
		}
		return false;
	};

	auto ColumnIter = std::find_if(HeaderIter->ContainColumnIndexArray.begin(),HeaderIter->ContainColumnIndexArray.end(),ColumnPred);
	//������
	if (ColumnIter == HeaderIter->ContainColumnIndexArray.end())
	{
		return false;
	}
	return true;
}

int CGridDisplayColumnInfo::QuerySensorIDByColumnName(const CString& strColumnName)
{
	if (strColumnName.IsEmpty())
	{
		return -1;
	}

	BOOST_FOREACH(auto& HeaderElement,m_HeaderInfoArray)
	{
		BOOST_FOREACH(auto& ColumnElement,HeaderElement.ContainColumnIndexArray)
		{
			if (ColumnElement.strColumnName == strColumnName)
			{
				return ColumnElement.nSensorID;
			}
		}
	}
	return -1;
}

CGridDisplayColumnInfo CGridDisplayColumnInfo::s_obj;
