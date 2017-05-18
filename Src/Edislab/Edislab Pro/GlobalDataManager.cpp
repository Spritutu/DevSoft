/****************************************************************************************
*��Ȩ����:
*�ļ���:GlobalDataManager.cpp
*����:ȫ�����ݹ�����
*����:2016.9
******************************************************************************************/
#include "stdafx.h"
#include "GlobalDataManager.h"
#pragma warning(push)
#pragma warning(disable:4018)
/*************************************************************************************************
��������:CreateInstance
��������:����ͨ����ʵ����
�������:��
�������:��
����ֵ:ʵ��
***************************************************************************************************/
CGlobalDataManager& CGlobalDataManager::CreateInstance()
{
	static CGlobalDataManager s_Manager;
	return s_Manager;
}

/*************************************************************************************************
��������:CGlobalDataManager
��������:���캯��
�������:��
�������:��
����ֵ:
***************************************************************************************************/
CGlobalDataManager::CGlobalDataManager(void)
{
	m_bHaveData = false;

	//��ʼ����������
	Init();
}

/*************************************************************************************************
��������:~CGlobalDataManager
��������:����ͨ���๹�캯��
�������:��
�������:��
����ֵ:
***************************************************************************************************/
CGlobalDataManager::~CGlobalDataManager(void)
{
}

/*************************************************************************************************
��������:IsHaveData
��������:�Ƿ�������
�������:��
�������:��
����ֵ:
***************************************************************************************************/
bool CGlobalDataManager::IsHaveData()
{
	return m_bHaveData;
}

/*************************************************************************************************
��������:SetHaveDataStatus
��������:����Ϊ������״̬
�������:��
�������:��
����ֵ:
***************************************************************************************************/
void CGlobalDataManager::SetHaveDataStatus()
{
	m_bHaveData = true;
}

/*************************************************************************************************
��������:SetNotHaveDataStatus
��������:���ó�������״̬
�������:��
�������:��
����ֵ:
***************************************************************************************************/
void CGlobalDataManager::SetNotHaveDataStatus()
{
	m_bHaveData = false;
}

/*************************************************************************************************
��������:IsHaveSensorCol
��������:��ǰ���Ƿ��д�����������
�������:��
�������:��
����ֵ:
***************************************************************************************************/
bool CGlobalDataManager::IsHaveSensorCol()
{
	if(!m_allData.size())
	{
		return false;
	}
	else
	{
		//�������е����е���
		for(int i=0; i<m_allData.size(); ++i)
		{
			for(int c=0; c<m_allData[i].vecColData.size(); ++c)
			{
				//�д�����
				if(TYPE_SENSOR==m_allData[i].vecColData[c].emType)
				{
					return true;
				}
			}
		}
		/*for (int i = 0;i < m_allData[0].vecColData.size(); ++i)
		{
		if( g_LabDataAttribution[0].vecColData[i].nColumnID > 0 
		&& g_LabDataAttribution[0].vecColData[i].nColumnID <= SENSOR_MAX_ID)
		{
		return true;
		}
		}*/
		return false;
	}
}

/*************************************************************************************************
��������:FindColumnID
��������:ͨ��ͨ������������ID
�������:nSensorChan:������ͨ��
�������:��
����ֵ:������ID
***************************************************************************************************/
int CGlobalDataManager::FindTimeID()
{
	m_cs.Lock();
	int nResult = -1;
	if(m_allData.size())
	{
		//���������ݸ���ֻ�����ڵ�ǰ��
		for(unsigned int i  = 0; i < m_allData.size();++i )
		{
			if(m_allData[i].bDefault)//��ǰ��
			{
				for(unsigned int j  = 0; j < m_allData[i].vecColData.size();++j )
				{
					if(m_allData[i].vecColData[j].emType == TYPE_TIME)
					{
						nResult =  m_allData[i].vecColData[j].nColumnID;
						break;
					}
				}
			}
		}
	}
	m_cs.UnLock();

	return nResult;
}

/*************************************************************************************************
��������:Init
��������:��ʼ����������
�������:��
�������:��
����ֵ:��
***************************************************************************************************/
void CGlobalDataManager::Init()
{
	//Ĭ��״̬���� X��Y���У�100�С����޾�������
	m_cs.Lock();
	//�������ݹ���/����
	GROUPDATA _group;
	_group.bDefault = true;
	_group.nGroupID = IDManager::CreateInstance()->generateID();
	//��������
	for (int iCol = 0; iCol < DEFAULT_COL_COUNT; ++iCol)
	{
		COLUMNDATA _column;
		_column.nColumnID = IDManager::CreateInstance()->generateID();
		//for (int iRow = 0 ; iRow < DEFAULT_ROW_COUNT ; ++iRow)
		//{
		//	_column.data.push_back(DEFAULT_DATA);
		//}

		if(!iCol)
		{
			_column.strColumnName = DEFUALT_COLUMN_NAME_X;
		}
		else
		{
			_column.strColumnName = DEFUALT_COLUMN_NAME_Y;
		}

		_group.vecColData.push_back(_column);
	}
	m_allData.push_back(_group);
	m_cs.UnLock();
}

/*************************************************************************************************
��������:AppendGroup
��������:�����
�������:��
�������:��
����ֵ:��
***************************************************************************************************/
void CGlobalDataManager::AppendGroup()
{
	m_cs.Lock();
	//����
	GROUPDATA _group;
	//���Ƶ�ǰ����������
	for(int i=0; i<m_allData.size(); ++i)
	{
		if(m_allData[i].bDefault)
		{
			_group = m_allData[i];
			break;
		}
	}
	//�޸Ĳ�����Ϣ
	_group.bDefault = false;
	_group.nGroupID = IDManager::CreateInstance()->generateID();
	_group.strGroupName = NEW_GROUP;
	//�޸�������
	for(int i=0; i<_group.vecColData.size(); ++i)
	{
		_group.vecColData[i].nColumnID = IDManager::CreateInstance()->generateID();
		_group.vecColData[i].nSensorID = -1;
		_group.vecColData[i].nSensorChanel = -1;
		//�ɱ༭
		if(COLUMN_EDIT_FORBID==_group.vecColData[i].emEdit)
		{
			_group.vecColData[i].emEdit = COLUMN_EDIT_ONLY_NUM;
		}
		//����
		if(TYPE_SENSOR==_group.vecColData[i].emType||TYPE_TIME==_group.vecColData[i].emType)
		{
			_group.vecColData[i].emType = TYPE_HANDWORK;
		}
		//��ɫ
	}
	m_cs.UnLock();
}

//��Ӵ�����������
void CGlobalDataManager::AppendColumn(int nSensorID,int nSensorChanel)
{
	m_cs.Lock();
	//ȡ�õ�ǰ��
	if(m_allData.size())
	{
		COLUMNDATA _column;
	
		_column.nColumnID = IDManager::CreateInstance()->generateID();
		_column.strColumnName = _T("F(kg)\n��");
		_column.nSensorID = nSensorID;
		_column.nSensorChanel = nSensorChanel;
		_column.emType = TYPE_SENSOR;
		_column.emEdit = COLUMN_EDIT_FORBID;

		m_allData[0].vecColData.push_back(_column);
	}

	m_cs.UnLock();
}

//begin add by xiaowei.han 2017-3-25
//��Ӵ�����������
void CGlobalDataManager::AppendColumn(int nSensorID,int nSensorChanel,const std::string& strUnit)
{
	m_cs.Lock();
	//ȡ�õ�ǰ��
	if(m_allData.size())
	{
		COLUMNDATA _column;

		_column.nColumnID = IDManager::CreateInstance()->generateID();
		_column.strColumnName = CString(strUnit.c_str());
		_column.nSensorID = nSensorID;
		_column.nSensorChanel = nSensorChanel;
		_column.emType = TYPE_SENSOR;
		_column.emEdit = COLUMN_EDIT_FORBID;

		m_allData[0].vecColData.push_back(_column);
	}

	m_cs.UnLock();
}
//end add by xiaowei.han 2017-3-25

//���ʱ��������
void CGlobalDataManager::AppendTimeColumn()
{
	m_cs.Lock();
	//ȡ�õ�ǰ��
	if(m_allData.size())
	{
		COLUMNDATA _column;
	
		_column.nColumnID = IDManager::CreateInstance()->generateID();
		_column.strColumnName = _T("t(s)\nʱ��");
		_column.emType = TYPE_TIME;
		_column.emEdit = COLUMN_EDIT_FORBID;

		m_allData[0].vecColData.push_back(_column);
	}

	m_cs.UnLock();
}

//������� һ���в�һ��
void CGlobalDataManager::PushData(int nColID ,int nRowPos)
{
	m_cs.Lock();
	//�������е����е��� �ҵ�IDһ������
	for(int i=0; i<m_allData.size(); ++i)
	{
		bool bMod = false;
		for(int c=0; c<m_allData[i].vecColData.size(); ++c)
		{
			if(nColID == m_allData[i].vecColData[c].nColumnID)
			{
				//��Ϊ������ǿհ�����,����������������Ŀ��Բ�����
				if(nRowPos<m_allData[i].vecColData.size())
				{
					VEC_STRING::iterator itrr = m_allData[i].vecColData[c].data.begin();
					//��ָ����ǰ����
					m_allData[i].vecColData[c].data.insert(itrr+nRowPos, DEFAULT_DATA);
				}
				////���ݲ���,�м����
				//while(m_allData[i].vecColData[c].data.size()<nRowPos)
				//{
				//	m_allData[i].vecColData[c].data.push_back(DEFAULT_DATA);
				//}
				//m_allData[i].vecColData[c].data[nRowPos] = szValue;
				//VEC_STRING::iterator itrr = m_allData[i].vecColData[c].data.begin();
				////��ָ����ǰ����
				//m_allData[i].vecColData[c].data.insert(itrr+nRowPos, szValue);
				bMod = true;
				break;
			}
		}
		if(bMod)
		{
			break;
		}
	}
	m_cs.UnLock();
}

//ͨ��ͨ���Ų�������
void CGlobalDataManager::PushData(int nSensorChan,short nValue)
{
	m_cs.Lock();

	//ͨ��ͨ������������ID
	//int nSensorID = FindColumnID(nSensorChan);

	//if(nSensorID == -1)
	//	return;

	CString strValue;
	strValue.Format(_T("%.2f"),(float)nValue/100.);

	int nSensorID;

	//�������е����е��� �ҵ�IDһ������
	for(int i=0; i<m_allData.size(); ++i)
	{
		if(m_allData[i].bDefault)
		{
			for(unsigned int j  = 0; j < m_allData[i].vecColData.size();++j )
			{
				if(m_allData[i].vecColData[j].nSensorChanel == nSensorChan)
				{
					nSensorID =  m_allData[i].vecColData[j].nColumnID;

					m_allData[i].vecColData[j].data.push_back(strValue);

					break;
				}
			}
		}
	}
	m_cs.UnLock();
}

//begin add by xiaowei.han 2017-3-25
void CGlobalDataManager::PushData(int nSensorChan,float fValue)
{
	m_cs.Lock();

	//ͨ��ͨ������������ID
	//int nSensorID = FindColumnID(nSensorChan);

	//if(nSensorID == -1)
	//	return;

	CString strValue;
	strValue.Format(_T("%.2f"),fValue);

	int nSensorID;

	//�������е����е��� �ҵ�IDһ������
	for(int i=0; i<m_allData.size(); ++i)
	{
		if(m_allData[i].bDefault)
		{
			for(unsigned int j  = 0; j < m_allData[i].vecColData.size();++j )
			{
				if(m_allData[i].vecColData[j].nSensorChanel == nSensorChan)
				{
					nSensorID =  m_allData[i].vecColData[j].nColumnID;

					m_allData[i].vecColData[j].data.push_back(strValue);

					break;
				}
			}
		}
	}
	m_cs.UnLock();
}
//end add by xiaowei.han 2017-3-25

//һ����Ӷ��� û�д˹��� һ��ֻ������һ��
//void CGlobalDataManager::PushData(int nColID ,int nStartRowPos ,int nEndRowPos ,VEC_STRING vecValue)
//{
//}
//���������һ��
void CGlobalDataManager::PushData(int nRowPos)
{
	m_cs.Lock();
	//�������е����е��� �ҵ�IDһ������
	for(int i=0; i<m_allData.size(); ++i)
	{
		for(int c=0; c<m_allData[i].vecColData.size(); ++c)
		{
			//��Ϊ������ǿհ�����,����������������Ŀ��Բ�����
			if(nRowPos<m_allData[i].vecColData.size())
			{
				VEC_STRING::iterator itrr = m_allData[i].vecColData[c].data.begin();
				//��ָ����ǰ����
				m_allData[i].vecColData[c].data.insert(itrr+nRowPos, DEFAULT_DATA);
			}
		}
	}
	m_cs.UnLock();
}

//�޸����� �޸ĵ�����Ԫ������
void CGlobalDataManager::ModifyData(int nColID ,int nRowPos ,CString szValue)
{
	m_cs.Lock();
	//�������е����е��� �ҵ�IDһ������
	for(int i=0; i<m_allData.size(); ++i)
	{
		bool bMod = false;
		for(int c=0; c<m_allData[i].vecColData.size(); ++c)
		{
			if(nColID == m_allData[i].vecColData[c].nColumnID)
			{
				//���ݲ���,�м����
				while(m_allData[i].vecColData[c].data.size()<=nRowPos)
				{
					m_allData[i].vecColData[c].data.push_back(DEFAULT_DATA);
				}
				m_allData[i].vecColData[c].data[nRowPos] = szValue;
				bMod = true;
				break;
			}
		}
		if(bMod)
		{
			break;
		}
	}
	m_cs.UnLock();
}
//�޸�һ���еĶ�������
void CGlobalDataManager::ModifyData(int nColID ,int nStartRowPos ,int nEndRowPos ,VEC_STRING vecValue)
{
	m_cs.Lock();
	//�������е����е��� �ҵ�IDһ������
	for(int i=0; i<m_allData.size(); ++i)
	{
		bool bMod = false;
		for(int c=0; c<m_allData[i].vecColData.size(); ++c)
		{
			if(nColID == m_allData[i].vecColData[c].nColumnID)
			{
				//���ݲ���,�м����
				while(m_allData[i].vecColData[c].data.size()<=nEndRowPos)
				{
					m_allData[i].vecColData[c].data.push_back(DEFAULT_DATA);
				}
				for(int v=0; v<vecValue.size(); ++v)
				{
					int index = nStartRowPos+v;
					if(index<=nEndRowPos)
					{
						m_allData[i].vecColData[c].data[index] = vecValue[v];
					}
					else
					{
						break;
					}
				}
				bMod = true;
				break;
			}
		}
		if(bMod)
		{
			break;
		}
	}
	m_cs.UnLock();
}
//ɾ������
void CGlobalDataManager::PopAllData()
{
	m_cs.Lock();
	//����ɾ������������
	VECTOR<GROUPDATA>::iterator itr = m_allData.begin();
	while(itr!=m_allData.end())
	{
		VECTOR<COLUMNDATA>::iterator itrc = itr->vecColData.begin(); 
		while(itrc!=itr->vecColData.end())
		{
			int id = itrc->nColumnID;
			itr->vecColData.erase(itrc);
			//ɾ��ID
			IDManager::CreateInstance()->delID(id);
		}
		//ɾ����
		int id = itr->nGroupID;
		m_allData.erase(itr);
		IDManager::CreateInstance()->delID(id);
	}
}
//ɾ��һ��
void CGlobalDataManager::PopData(int nColID)
{
	m_cs.Lock();
	for(int i=0; i<m_allData.size(); ++i)
	{
		bool bDel = false;
		for(VECTOR<COLUMNDATA>::iterator itr = m_allData[i].vecColData.begin(); 
			itr!=m_allData[i].vecColData.end(); ++itr)
		{
			if(nColID==itr->nColumnID)
			{
				//ɾ����
				m_allData[i].vecColData.erase(itr);
				//ɾ��ID
				IDManager::CreateInstance()->delID(nColID);
				bDel = true;
				break;
			}
		}
		if(bDel)
		{
			break;
		}
	}
	m_cs.UnLock();
}
//ɾ��һ��
void CGlobalDataManager::PopGroupData(const int nGroupID)
{
	m_cs.Lock();
	for(VECTOR<GROUPDATA>::iterator itr = m_allData.begin(); itr!=m_allData.end(); ++itr)
	{
		if(nGroupID==itr->nGroupID)
		{
			VECTOR<COLUMNDATA>::iterator itrc = itr->vecColData.begin(); 
			while(itrc!=itr->vecColData.end())
			{
				int id = itrc->nColumnID;
				itr->vecColData.erase(itrc);
				//ɾ��ID
				IDManager::CreateInstance()->delID(id);
			}
			//ɾ����
			m_allData.erase(itr);
			IDManager::CreateInstance()->delID(nGroupID);
			break;
		}
	}
}
//ɾ��һ������
void CGlobalDataManager::PopData(int nColID ,int nRowPos)
{
	m_cs.Lock();
	bool bFind = false;
	for(VECTOR<GROUPDATA>::iterator itr = m_allData.begin(); itr!=m_allData.end(); ++itr)
	{
		VECTOR<COLUMNDATA>::iterator itrc = itr->vecColData.begin();
		//������
		while(itrc!=itr->vecColData.end())
		{
			//�ҵ���
			if(nColID==itrc->nColumnID)
			{
				VEC_STRING::iterator itrr = itrc->data.begin();
				int row = 0;
				while(itrr!=itrc->data.end())
				{
					//�ҵ���
					if(row==nRowPos)
					{
						itrc->data.erase(itrr);
						bFind = true;
						break;
					}
					++row;
				}
				break;
			}
		}
		if(bFind)
		{
			break;
		}
	}
	m_cs.UnLock();
}
void CGlobalDataManager::PopData(int nColID ,int nStartRowPos ,int nEndRowPos)
{
	m_cs.Lock();
	bool bFind = false;
	for(VECTOR<GROUPDATA>::iterator itr = m_allData.begin(); itr!=m_allData.end(); ++itr)
	{
		VECTOR<COLUMNDATA>::iterator itrc = itr->vecColData.begin();
		//������
		while(itrc!=itr->vecColData.end())
		{
			//�ҵ���
			if(nColID==itrc->nColumnID)
			{
				VEC_STRING::iterator itrr = itrc->data.begin();
				int row = 0;
				while(itrr!=itrc->data.end())
				{
					//�ҵ���
					if(row>=nStartRowPos && row<=nEndRowPos)
					{
						itrc->data.erase(itrr);
						bFind = true;
					}
					++row;
					if(row>nEndRowPos)
					{
						break;
					}
				}
				break;
			}
		}
		if(bFind)
		{
			break;
		}
	}
	m_cs.UnLock();
}

//ɾ����������
void CGlobalDataManager::DeleteAllContainer()
{
}
void CGlobalDataManager::DeleteContainer(int nColID)
{
}

//ȡ��ÿ��Item����
void CGlobalDataManager::GetItemData(int nCol,int nRow,CString& strValue)
{
	m_cs.Lock();

	strValue.Empty();

	int nGroupCount = static_cast<int>(m_allData.size());

	int nFind = 0;
	int nColumnID = 0;

	for (int nGroup = 0; nGroup < nGroupCount; nGroup++)
	{
		int nColumnCount = static_cast<int>(m_allData[nGroup].vecColData.size());
		for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
		{
			if(nFind == nCol)
			{
				nColumnID = m_allData[nGroup].vecColData[nColumn].nColumnID;

				if(m_allData[nGroup].vecColData[nColumn].data.size() > nRow)
				{
					strValue = m_allData[nGroup].vecColData[nColumn].data[nRow];
				}

				break;
			}
			nFind++;
		}
	}

	m_cs.UnLock();
}

//ȡ���������
int CGlobalDataManager::GetMaxRow()
{
	m_cs.Lock();

	int nMaxRow = 0;

	int nGroupCount = static_cast<int>(m_allData.size());

	for (int nGroup = 0; nGroup < nGroupCount; nGroup++)
	{
		int nColumnCount = static_cast<int>(m_allData[nGroup].vecColData.size());
		for (int nCol = 0; nCol < nColumnCount; nCol++)
		{
			int nSize = static_cast<int>(m_allData[nGroup].vecColData[nCol].data.size());
			nMaxRow = max(nMaxRow,nSize);
		}
	}

	m_cs.UnLock();

	return nMaxRow;
}
#pragma warning(pop)
