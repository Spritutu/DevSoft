#include "stdafx.h"
#include "ConverEncode.h"
#include "wininet.h"
#pragma warning(disable:4996)
using std::string;
ConverEncode::ConverEncode(void)
{
}

ConverEncode::~ConverEncode(void)
{
}

/************************************************************************************************************
��������:Gb2312ToUnicode
��������:���ַ���ΪGB2312�����ת��ΪUnicode������ַ���
�������:gbBuffer:GB2312������ַ���
�������:pOut:����ת������Unicode������ַ���
����ֵ:��
************************************************************************************************************/
void ConverEncode::Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer)
{
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
	return;
}

/************************************************************************************************************
��������:UTF_8ToUnicode
��������:���ַ���ΪUTF-8�����ת��ΪUnicode������ַ���
�������:pText:UTF-8������ַ���
�������:pOut:����ת������Unicode������ַ���
����ֵ:��
************************************************************************************************************/
void ConverEncode::UTF_8ToUnicode(WCHAR* pOut,char *pText)
{
	char* uchar = (char *)pOut;

	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

	return;
}

/************************************************************************************************************
��������:UnicodeToUTF_8
��������:���ַ���ΪUnicode�����ת��ΪUTF-8������ַ���
�������:pText:Unicode������ַ���
�������:pOut:����ת������UTF-8������ַ���
����ֵ:��
************************************************************************************************************/
void ConverEncode::UnicodeToUTF_8(char* pOut,WCHAR* pText)
{
	// ע�� WCHAR�ߵ��ֵ�˳��,���ֽ���ǰ�����ֽ��ں�
	char* pchar = (char *)pText;

	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));

	return;
}

/************************************************************************************************************
��������:UnicodeToGB2312
��������:���ַ���ΪUnicode�����ת��ΪGB2312������ַ���
�������:uData:Unicode������ַ���
�������:pOut:����ת������GB2312������ַ���
����ֵ:��
************************************************************************************************************/
void ConverEncode::UnicodeToGB2312(char* pOut,WCHAR uData)
{
	WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(WCHAR),NULL,NULL);
	return;
}

//��Ϊ��Urlʹ��
char ConverEncode:: CharToInt(char ch)
{
	if(ch>='0' && ch<='9')return (char)(ch-'0');
	if(ch>='a' && ch<='f')return (char)(ch-'a'+10);
	if(ch>='A' && ch<='F')return (char)(ch-'A'+10);
	return -1;
}

char ConverEncode::StrToBin(char *str)
{
	char tempWord[2];
	char chn;

	tempWord[0] = CharToInt(str[0]);                         //make the B to 11 -- 00001011
	tempWord[1] = CharToInt(str[1]);                         //make the 0 to 0  -- 00000000

	chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000

	return chn;
}

void ConverEncode::UTF_8ToGB2312( std::string &strGB2312Content, char* pSrc, int nSrcLength )
{
	char buf[4] = {0};
	int nDestLength = nSrcLength + (nSrcLength >> 2) + 2;
	char* rst = new char[nDestLength];
	if (NULL == rst)
	{
		return;
	}
	memset(rst,0,nDestLength);

	int i =0;
	int j = 0;

	while(i < nSrcLength)
	{
		if(*(pSrc + i) >= 0)
		{

			rst[j++] = pSrc[i++];
		}
		else                 
		{
			WCHAR Wtemp;


			UTF_8ToUnicode(&Wtemp,pSrc + i);

			UnicodeToGB2312(buf,Wtemp);

			unsigned short int tmp = 0;
			tmp = rst[j] = buf[0];
			tmp = rst[j+1] = buf[1];
			tmp = rst[j+2] = buf[2];
			i += 3;    
			j += 2;   
		}

	}
	rst[j] = '\0';
	strGB2312Content = rst; 
	delete []rst;
	rst = NULL;
}

//GB2312 תΪ UTF-8
void ConverEncode::GB2312ToUTF_8( std::string& pOut, char* szSrc, int pLen )
{
	char buf[4] = {0};

	pOut.clear();

	int i = 0;
	while(i < pLen)
	{
		//�����Ӣ��ֱ�Ӹ��ƾͿ���
		if(szSrc[i] >= 0)
		{
			char asciistr[2]={0};
			asciistr[0] = (szSrc[i++]);
			pOut.append(asciistr);
		}
		else
		{
			WCHAR pbuffer;
			Gb2312ToUnicode(&pbuffer,szSrc+i);

			UnicodeToUTF_8(buf,&pbuffer);

			pOut.append(buf);

			i += 2;
		}
	}

	return;
}

//��str����Ϊ��ҳ�е� GB2312 url encode ,Ӣ�Ĳ��䣬����˫�ֽ�  ��%3D%AE%88
std::string ConverEncode::UrlGB2312( char* str )
{
	string dd;
	size_t len = strlen(str);
	for (size_t i=0;i<len;i++)
	{
		if(isalnum((BYTE)str[i]))
		{
			char tempbuff[2];
			sprintf(tempbuff,"%c",str[i]);
			dd.append(tempbuff);
		}
		else if (isspace((BYTE)str[i]))
		{
			dd.append("+");
		}
		else
		{
			char tempbuff[4];
			sprintf(tempbuff,"%%%X%X",((BYTE*)str)[i] >>4,((BYTE*)str)[i] %16);
			dd.append(tempbuff);
		}

	}
	return dd;
}

//��str����Ϊ��ҳ�е� UTF-8 url encode ,Ӣ�Ĳ��䣬�������ֽ�  ��%3D%AE%88
std::string ConverEncode::UrlUTF8( char* str )
{
	string tt;
	string dd;
	GB2312ToUTF_8(tt,str,(int)strlen(str));

	size_t len=tt.length();
	for (size_t i=0;i<len;i++)
	{
		if(isalnum((BYTE)tt.at(i)))
		{
			char tempbuff[2]={0};
			sprintf(tempbuff,"%c",(BYTE)tt.at(i));
			dd.append(tempbuff);
		}
		else if (isspace((BYTE)tt.at(i)))
		{
			dd.append("+");
		}
		else
		{
			char tempbuff[4];
			sprintf(tempbuff,"%%%X%X",((BYTE)tt.at(i)) >>4,((BYTE)tt.at(i)) %16);
			dd.append(tempbuff);
		}

	}
	return dd;
}

//��url GB2312����
std::string ConverEncode::UrlGB2312Decode( std::string str )
{
	string output = "";
	char tmp[2];
	int i = 0;
	int idx = 0;
	int len = (int)str.length();

	while(i < len)
	{
		if(str[i] == '%')
		{
			tmp[0] = str[i+1];
			tmp[1] = str[i+2];
			output += StrToBin(tmp);
			i = i + 3;
		}
		else if(str[i] == '+')
		{
			output += ' ';
			i++;
		}
		else
		{
			output+=str[i];
			i++;
		}
	}

	return output;
}

//��url utf8����
std::string ConverEncode::UrlUTF8Decode( std::string str )
{
	string output = "";
	string temp = UrlGB2312Decode(str);//
	UTF_8ToGB2312(output,(char *)temp.data(),(int)strlen(temp.data()));
	return output;
}

