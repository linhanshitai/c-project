#ifndef _H_NO_COPY_
#define _H_NO_COPY_
//���ɸ��Ƶ�������Ҫ�̳б���
class NoCopy
{
protected:
	NoCopy(){};
	~NoCopy(){};
private:
	NoCopy(const NoCopy&);
	const NoCopy &operator=(const NoCopy&);
};
#endif