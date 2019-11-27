#ifndef _PATH_H_
#define _PATH_H_
#include <vector>
#include<cstdint>
//A*Ѱ·�㷨
struct SPoint
{
	uint8_t x;
	uint8_t y;
	SPoint() :x(0), y(0) {}
	SPoint(uint8_t xx,uint8_t yy):x(xx),y(yy){}
	SPoint(const SPoint& point):x(point.x),y(point.y){}
};
class CPath
{
private:
	enum ElementStat:uint8_t
	{
		enmNew		= 0,	// ��ʼ״̬
		enmOpen		= 1,	// ��open����
		enmClosed	= 2		// closed״̬
	};
	struct SElement				
	{
		uint8_t x;			
		uint8_t y;			
		ElementStat stat;		// ״̬
		uint16_t g;				// g������ʵ�ʴ���
		uint16_t total;			// h+g
		uint16_t index;
		SElement* father;		// ���ڵ�
	};
public:
	CPath();
	~CPath();
	bool FindPath(const uint8_t* attribute,uint8_t width,uint8_t height,
		const SPoint& src, const SPoint& tgt, std::vector<SPoint>& path)
	{
		Init(attribute,width,height);
		return FindPath(src,tgt,path);
	}
private:
	void Init(const uint8_t* attribute,uint8_t width,uint8_t height);
	bool FindPath(const SPoint& src, const SPoint& tgt, std::vector<SPoint>& path);
	void InsertOpen(SElement* pEle);
	bool CheckPoint(const SPoint& point)const{return point.x < m_width && point.y < m_height;}
	bool CheckPoint(uint8_t x,uint8_t y) const{return x < m_width && y < m_height;}
	SElement* FindPath(uint8_t sx, uint8_t sy, uint8_t tx, uint8_t ty);
private:
	 uint8_t m_height = 0;
	 uint8_t m_width = 0;
	 const uint8_t *m_Attr = nullptr;
	 uint16_t m_OpenCount = 0;
	 SElement* m_arrOpenTable[65536];
	 uint16_t m_checkedNum = 0;
	 SElement* m_arrCheckedTable[65536];//Ϊ�˽�Լ�ڴ�Ҳ����ֱ����һ��SPoint���� ��Ҫ�����ڱȽ�
	 SElement m_mapElement[256][256];
	 int8_t m_dir[8][2];
};
#endif