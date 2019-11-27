#include "pch.h"
#include "readcsv.h"
#include "log.h"
CReadCsv::CReadCsv()
{
	memset(m_arrVal, 0, sizeof(m_arrVal));
	m_nIndex = 0;
	memset(m_buffer, 0, sizeof(m_buffer));
	m_fp = nullptr;
}
void CReadCsv::Close()
{
	if (m_fp != nullptr)
	{
		fclose(m_fp);
		m_fp = nullptr;
	}
}
bool CReadCsv::OpenFile(const std::string &strPath)
{
	Close();
	m_fp = Fopen(strPath.data(),"r");
	if(nullptr == m_fp)
		WRITE_ERROR_LOG(" open cfg has error name=%s",strPath.data());
	return m_fp != nullptr;
}
CReadCsv::~CReadCsv()
{
	Close();
}
bool CReadCsv::ParseHead(uint16_t nCol)
{
	if (feof(m_fp) || nullptr == fgets(m_buffer, MAXBUFFSIZE, m_fp))
	{
		Close();
		WRITE_ERROR_LOG("cfg has error no head info");
		return false;
	}
	if(m_buffer[0] == 0xEF && m_buffer[1] == 0xBB && m_buffer[2]==0xBF)
		Parse(3);//BOMͷ����0xef 0xbb 0xbf �����Ӧ�ôӵ��ĸ��ֽڿ�ʼ���� window��csvû��bomͷ���Ļ���ʾ����
	else
		Parse();
	if (m_nIndex < MAXINDEX && m_nIndex == ((nCol << 1) - 1))return true;
	Close();
	WRITE_ERROR_LOG("cfg head col not same needcol=%u currcol=%u buff=%s", nCol, (m_nIndex >> 1) + 1, m_buffer);
	return false;
}
bool CReadCsv::FetchRow(uint16_t nCol)
{
	if(feof(m_fp) || nullptr == fgets(m_buffer, MAXBUFFSIZE,m_fp))
	{
		Close();
		return false;
	}
	if (0 == m_nRow && m_buffer[0] == 0xEF && m_buffer[1] == 0xBB && m_buffer[2] == 0xBF)
		Parse(3);//BOMͷ����0xef 0xbb 0xbf �����Ӧ�ôӵ��ĸ��ֽڿ�ʼ���� window��csvû��bomͷ���Ļ���ʾ����
	else
		Parse();
	if(m_nIndex < MAXINDEX && m_nIndex == ((nCol<<1)-1))return true;
	Close();
	WRITE_ERROR_LOG("cfg col not same row=%u needcol=%u currcol=%u buff=%s", m_nRow, nCol, (m_nIndex >> 1) + 1, m_buffer);
	return false;
}
bool CReadCsv::FetchRow()
{
	if (feof(m_fp) || nullptr == fgets(m_buffer, MAXBUFFSIZE, m_fp))
	{
		Close();
		return false;
	}
	Parse();
	return true;
}
void CReadCsv::Parse(uint16_t begin /*= 0*/ )
{
	++m_nRow;
	bool flag = false;
	if (m_buffer[0] == '"')
	{
		begin += 1;
		flag = true;
	}
	m_nIndex = 0;
	m_arrVal[m_nIndex++] = begin;
	uint16_t replace = 0;
	while (true)
	{
		if (m_buffer[begin] == '\0')
		{
			m_arrVal[m_nIndex++] = begin;
			break;
		}
		if (flag)//�ַ���������˫����
		{
			if (m_buffer[begin++] != '"')continue;
			if (m_buffer[begin] == '"')//����˫���ű�ʾת��
			{
				if (0 == replace)
					replace = begin;
				++begin;
			}
			else//",��"\0��ʾ�ֶν���
			{
				if (replace > 0)
				{
					for (uint16_t j = replace + 1; j <  begin - 1; ++j)
					{
						if (m_buffer[j] == '"')
							++j;
						m_buffer[replace++] = m_buffer[j];
					}
					m_arrVal[m_nIndex++] = replace;
					replace = 0;
				}
				else
					m_arrVal[m_nIndex++] = begin - 1;
				if (m_buffer[begin++] == '\0')break;
				if (m_buffer[begin] == '\0')//����и����ű�ʾ�и����ַ�
				{
					m_arrVal[m_nIndex++] = begin;
					m_arrVal[m_nIndex++] = begin;
					break;
				}
				if (m_buffer[begin] == '"')
					++begin;
				else
					flag = false;
				m_arrVal[m_nIndex++] = begin;
			}
		}
		else if (m_buffer[begin] == ',')
		{
			m_arrVal[m_nIndex++] = begin;
			m_buffer[begin] = '\0';//�����ַ���תʵ��
			if (m_buffer[++begin] == '\0')//����и����ű�ʾ�и����ַ�
			{
				m_arrVal[m_nIndex++] = begin;
				m_arrVal[m_nIndex++] = begin;
				break;
			}
			if (m_buffer[begin] == '"')
			{
				++begin;
				flag = true;
			}
			m_arrVal[m_nIndex++] = begin;
		}
		else
			++begin;
	}
#if 0
	for (int i = 0; i < m_nIndex; i += 2)
		printf("size=%u val[%u]=%s\n",m_arrVal[i + 1] - m_arrVal[i],i,
		std::string(&m_buffer[m_arrVal[i]],m_arrVal[i+1]-m_arrVal[i]).data());
#endif
}
