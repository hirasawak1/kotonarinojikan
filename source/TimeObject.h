#ifndef _TIMEOBJECT_H_
#define _TIMEOBJECT_H_


// ���v�E�X�C�E���g���m�[���̊��N���X
class TimeObject
{
public:
	virtual ~TimeObject() {}

	virtual void Update()=0;
	virtual void Render()=0;
	virtual bool CheckWithin(const POINT& p, const int& startX, const int& startY, const int size) { return false; };
	
};

#endif