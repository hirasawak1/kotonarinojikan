
#ifndef		__BASEOBJ_H__
#define	__BASEOBJ_H__

//****************************************************************************************
//
//	BaseObj�N���X
//
//****************************************************************************************
class BaseObj
{
private:

public:
	//	�������E���
	BaseObj(void) {};
	virtual ~BaseObj(void) {};
	
	//	�X�V�E�`��
	virtual void	Update(void) {};
	virtual void	Render(void) {};
};

//****************************************************************************************
#endif // !__BASEOBJ_H__
