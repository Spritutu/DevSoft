/**********************************************************
*��Ȩ����:xiaowei.han
*�ļ�����:GridDisplayColumnInfo.cpp
*��������:Grid��ʾ����Ϣ���ݹ�����
*����:xiaowei.han
*����:2017/05/14
**********************************************************/
#include "StdAfx.h"
#include "GridColumnGroupManager.h"
#include <algorithm>
#include <boost/foreach.hpp>
#include "Utility.h"
CGridColumnGroupManager& CGridColumnGroupManager::CreateInstance()
{
	return s_obj;
}

/*******************************************************************
*��������:GetGridDisplayInfo
*��������:��ȡ�з�����Ϣ
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/04 21:48:09
*******************************************************************/
void CGridColumnGroupManager::GetGridDisplayInfo(std::vector<COLUMN_GROUP_INFO>& DisplayInfoArray)
{
	DisplayInfoArray.clear();
	DisplayInfoArray.assign(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end());
}

CGridColumnGroupManager::CGridColumnGroupManager(void)
{
	m_HeaderInfoArray.clear();
	InitGridDisplayInfo();
}


CGridColumnGroupManager::~CGridColumnGroupManager(void)
{
	m_HeaderInfoArray.clear();
}

/*******************************************************************
*��������:InitGridDisplayInfo
*��������:��ʼ���з�����Ϣ
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/04 21:48:09
*******************************************************************/
void CGridColumnGroupManager::InitGridDisplayInfo()
{

#if 0
	//��ʼ����ʾ
	COLUMN_GROUP_INFO TempInfo;
	TempInfo.strGroupName = _T("��ǰ");
	COLUMN_INFO TempColumnInfo;
	TempColumnInfo.strColumnName = _T("X");
	TempInfo.ColumnArray.push_back(TempColumnInfo);
	TempColumnInfo.Reset();
	TempColumnInfo.strColumnName = _T("Y");
	TempInfo.ColumnArray.push_back(TempColumnInfo);	
	
	m_HeaderInfoArray.push_back(TempInfo);
#endif
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
bool CGridColumnGroupManager::IsHeaderNameExist(const CString& strHeaderName)
{
	if (strHeaderName.IsEmpty())
	{
		return false;
	}
	//������ͷ��Ϣ
	auto HeaderPred = [&strHeaderName](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == strHeaderName)
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
bool CGridColumnGroupManager::IsColumnExistInFixedHeader(const CString& strHeaderName,const CString& strColumnName)
{
	if (strHeaderName.IsEmpty() || strColumnName.IsEmpty())
	{
		return false;
	}

	//����ĳ�е������Ƿ����Ҫɾ����
	auto HeaderPred = [&strHeaderName](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == strHeaderName)
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

	auto ColumnIter = std::find_if(HeaderIter->ColumnArray.begin(),HeaderIter->ColumnArray.end(),ColumnPred);
	//������
	if (ColumnIter == HeaderIter->ColumnArray.end())
	{
		return false;
	}
	return true;
}

/*******************************************************************
*��������:AddDisplayColumnInfo
*��������:����з������Ϣ
*�������:None
*�������:None
*����ֵ:None
*����:xiaowei.han
*����:2017/05/13 15:59:47
*******************************************************************/
void CGridColumnGroupManager::AddDisplayColumnInfo( const COLUMN_GROUP_INFO& GridColumnInfo )
{
	//������ͷ��Ϣ
	auto HeaderPred = [&GridColumnInfo](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == GridColumnInfo.strGroupName)
		{
			return true;
		}
		return false;
	};

	auto HeaderIter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),HeaderPred);
	//�������
	if (HeaderIter != m_HeaderInfoArray.end())
	{
		return;
	}

	m_HeaderInfoArray.push_back(GridColumnInfo);
}

/*******************************************************************
*��������:AddDisplayColumnInfo
*��������:����е���֪�������Ϣ
*�������:None
*�������:None
*����ֵ:None
*����:xiaowei.han
*����:2017/05/13 15:59:47
*******************************************************************/
void CGridColumnGroupManager::AddDisplayColumnInfo( const CString& strHeaderName,const COLUMN_INFO& AddColumnInfo )
{
	//������ͷ��Ϣ
	auto HeaderPred = [&strHeaderName](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == strHeaderName)
		{
			return true;
		}
		return false;
	};

	//Ѱ�������Ƶ�lamda���ʽ
	auto ColumnPred = [&AddColumnInfo](const COLUMN_INFO& ColumnInfo)->bool
	{
		//��������ͬ
		if (ColumnInfo.strColumnName == AddColumnInfo.strColumnName)
		{
			return true;
		}
		return false;
	};

	auto HeaderIter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),HeaderPred);
	
	//����
	if (HeaderIter != m_HeaderInfoArray.end())
	{
		
		//�ڴ��в���IDС��0������
		auto ColumnIter = std::find_if(HeaderIter->ColumnArray.begin(),HeaderIter->ColumnArray.end(),ColumnPred);
		//����
		if (ColumnIter != HeaderIter->ColumnArray.end())
		{
			return;
		}
		else
		{
			HeaderIter->ColumnArray.push_back(AddColumnInfo);
		}
	}
	//û�������һ��
	else
	{
		COLUMN_GROUP_INFO NewGroupInfo;
		NewGroupInfo.strGroupName = strHeaderName;
		NewGroupInfo.ColumnArray.push_back(AddColumnInfo);
		m_HeaderInfoArray.push_back(NewGroupInfo);
	}
}

/*******************************************************************
*��������:RemoveColumnInfo
*��������:�Ƴ�����Ϣ
*�������:None
*�������:None
*����ֵ:None
*����:xiaowei.han
*����:2017/05/13 15:59:47
*******************************************************************/
void CGridColumnGroupManager::RemoveColumnInfo( const CString& strHeaderName,const CString& strColumnName )
{

	//������ͷ��Ϣ
	auto HeaderPred = [&strHeaderName](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == strHeaderName)
		{
			return true;
		}
		return false;
	};

	auto ColumnPred = [&strColumnName](const COLUMN_INFO& ColumnInfo)->bool
	{
		if (ColumnInfo.strColumnName == strColumnName)
		{
			return true;
		}
		return false;
	};

	auto HeaderIter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),HeaderPred);

	//����
	if (HeaderIter != m_HeaderInfoArray.end())
	{

		//�ڴ��в���IDС��0������
		auto ColumnIter = std::find_if(HeaderIter->ColumnArray.begin(),HeaderIter->ColumnArray.end(),ColumnPred);
		//����
		if (ColumnIter != HeaderIter->ColumnArray.end())
		{
			HeaderIter->ColumnArray.erase(ColumnIter);

			//���Ԫ��Ϊ���ˣ����Ƴ�����
			if (HeaderIter->ColumnArray.empty())
			{
				m_HeaderInfoArray.erase(HeaderIter);
			}
		}
	}
}

/*******************************************************************
*��������:RemoveHeader
*��������:�Ƴ�������Ϣ
*�������:
*�������:
*����ֵ:
*����:xiaowei.han
*����:2017/06/04 21:50:34
*******************************************************************/
void CGridColumnGroupManager::RemoveHeader( const CString& strHeaderName )
{
	//������ͷ��Ϣ
	auto HeaderPred = [&strHeaderName](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == strHeaderName)
		{
			return true;
		}
		return false;
	};

	auto HeaderIter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),HeaderPred);

	//����
	if (HeaderIter != m_HeaderInfoArray.end())
	{
		m_HeaderInfoArray.erase(HeaderIter);
	}
}

CGridColumnGroupManager CGridColumnGroupManager::s_obj;
